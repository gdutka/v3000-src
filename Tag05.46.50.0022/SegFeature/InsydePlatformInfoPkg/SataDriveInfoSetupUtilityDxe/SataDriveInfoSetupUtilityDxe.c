/** @file
  Implementation of SataDriveInfoSetupUtilityDxe.

;******************************************************************************
;* Copyright (c) 2023 - 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "SataDriveInfoSetupUtilityDxe.h"

#include <Uefi.h>

#include <IndustryStandard/Atapi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/HiiExLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/SataDriveInfoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/VariableLib.h>
#include <Library/DeviceInfoLookupDataLib.h>
#include <Library/DxeSetupVariableLib.h>

#include <Protocol/AtaPassThru.h>
#include <Protocol/DevicePath.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/IdeControllerInit.h>
#include <Protocol/PciIo.h>
#include <Protocol/SetupUtilityApplication.h>

#include <Protocol/H2OPciLookupTable.h>
#include <Protocol/H2OSataDriveInfo.h>

#include <Guid/GlobalVariable.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/SataDriveInfoSetupUtilityHii.h>

#include "SataDriveInfoSetupNVDataStruc.h"

#define PCI_CLASS_CODE_TO_CLASS_CODE_32(Ptr) \
  ( \
    ((((UINT32)(*(Ptr + 2))) << 16) & 0xff0000) | \
    ((((UINT32)(*(Ptr + 1))) << 8) & 0xff00) | \
    (((UINT32)(*Ptr)) & 0xff) \
  )

#ifndef BASE_LIST_FOR_EACH
#define BASE_LIST_FOR_EACH(Entry, ListHead)    \
  for(Entry = (ListHead)->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)
#endif

extern UINT8  SataDriveInfoVfrBin[];
extern UINT8  SataDriveInfoSetupUtilityDxeStrings[];

STATIC  SATA_DRIVE_INFO_SETUP_PRIVATE_DATA  *mPrivateData = NULL;
STATIC  HII_VENDOR_DEVICE_PATH              mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    SATA_DRIVE_INFO_SETUP_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

STATIC  EFI_HII_HANDLE              mHiiHandle                  = NULL;
STATIC  CHAR16                      *mVariableName              = SATA_DRIVE_INFO_CONFIGURATION_VARIABLE_NAME;

STATIC  UINTN                       mNumberOfControllers        = 0;
STATIC  LIST_ENTRY                  *mControllerInfoPool        = NULL;
STATIC  EFI_STRING_ID               *mControllerStringIds       = NULL;
STATIC  EFI_STRING_ID               *mControllerHelpStringIds   = NULL;

STATIC  UINTN                       mNumberOfDrives             = 0;
STATIC  LIST_ENTRY                  *mDriveInfoPool             = NULL;
STATIC  EFI_STRING_ID               *mDriveStringIds            = NULL;
STATIC  EFI_STRING_ID               *mDriveHelpStringIds        = NULL;

STATIC  UINTN                                    mNumberOfPhysicalPorts  = 0;
STATIC  H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  *mPhysicalPortNameTable = NULL;
STATIC  EFI_STRING_ID                            *mPhysicalPortStringIds = NULL;

STATIC  EFI_STRING_ID               mSmartAttributeStringIds[MAX_SMART_ATTRIBUTE_ENTRY]      = {0};
STATIC  EFI_STRING_ID               mSmartAttributeHelpStringIds[MAX_SMART_ATTRIBUTE_ENTRY]  = {0};

STATIC  CHAR8                       *mPlatformLang              = NULL;

/**
  Compare two device pathes to check if they are exactly same.

  @param DevicePath1    A pointer to the first device path data structure.
  @param DevicePath2    A pointer to the second device path data structure.

  @retval TRUE    They are same.
  @retval FALSE   They are not same.

**/
STATIC
BOOLEAN
EfiCompareDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath1,
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath2
  )
{
  UINTN Size1;
  UINTN Size2;

  Size1 = GetDevicePathSize (DevicePath1);
  Size2 = GetDevicePathSize (DevicePath2);

  if (Size1 != Size2) {
    return FALSE;
  }

  if (CompareMem (DevicePath1, DevicePath2, Size1) != 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  This function frees a pool of memory if the address is non-NULL and then resets
  the address to NULL so that future calls will do nothing.

  @param[in out]    Buffer              On entry, optional pointer to the
                                        address of the buffer to free or NULL.
                                        On exit, optional pointer to NULL.

**/
STATIC
VOID
InternalFreePool (
  IN  VOID  **Buffer
  )
{
  if (Buffer != NULL && *Buffer != NULL) {
    FreePool (*Buffer);
    *Buffer = NULL;
  }
}

/**
  Helper function to get the controller name.

  @param[in]  ControllerInfo  The pointer to the BUS_CONTROLLER_INFO instance.

  @retval The pointer to the Controller Name. It is the caller's
          responsibility to free this buffer.

**/
CHAR16 *
GetControllerName (
  IN  BUS_CONTROLLER_INFO   *ControllerInfo
  )
{
  EFI_STATUS                      Status;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL   *PciLookupTable;
  CHAR16                          StrBuffer[0x100];
  CHAR16                          *ControllerNameFormat;
  CHAR16                          *ControllerName;
  CHAR16                          *VendorNameStr;
  CHAR16                          *DeviceNameStr;
  UINT32                          ClassCode;
  CHAR16                          *BaseClassStr;
  CHAR16                          *SubClassStr;
  CHAR16                          *InterfaceStr;

  PciLookupTable  = NULL;
  ControllerName  = NULL;
  VendorNameStr   = NULL;
  DeviceNameStr   = NULL;
  BaseClassStr    = NULL;
  SubClassStr     = NULL;
  InterfaceStr    = NULL;

  Status = gBS->LocateProtocol (
                  &gH2OPciLookupTableProtocolGuid,
                  NULL,
                  (VOID **) &PciLookupTable
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to LocateProtocol %g, Status: %r\n", __FUNCTION__, &gH2OPciLookupTableProtocolGuid, Status));

    ControllerNameFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_CONTROLLER_PROMPT_FORMAT), NULL);

    ZeroMem (StrBuffer, sizeof (StrBuffer));
    UnicodeSPrint (
      StrBuffer,
      sizeof (StrBuffer),
      ControllerNameFormat,
      ControllerInfo->SerialNumber,
      ControllerInfo->PciConfigSpace.Hdr.VendorId,
      ControllerInfo->PciConfigSpace.Hdr.DeviceId
      );
    ControllerName = AllocateCopyPool (StrnSizeS (StrBuffer, sizeof (StrBuffer)), StrBuffer);

    InternalFreePool ((VOID **) &ControllerNameFormat);
    return ControllerName;
  }

  PciLookupTable->PCIeId2Str (
                    ControllerInfo->PciConfigSpace.Hdr.VendorId,
                    ControllerInfo->PciConfigSpace.Hdr.DeviceId,
                    &VendorNameStr,
                    &DeviceNameStr,
                    NULL
                    );
  if (DeviceNameStr == NULL) {
    ClassCode = PCI_CLASS_CODE_TO_CLASS_CODE_32 (ControllerInfo->PciConfigSpace.Hdr.ClassCode);
    PciLookupTable->PciGetClassStrings (
                      ClassCode,
                      &BaseClassStr,
                      &SubClassStr,
                      &InterfaceStr,
                      NULL
                      );
    if (SubClassStr != NULL) {
      DeviceNameStr = (CHAR16 *) AllocateCopyPool (StrSize (SubClassStr), SubClassStr);
    }
  }

  ZeroMem (StrBuffer, sizeof (StrBuffer));
  if (VendorNameStr != NULL && DeviceNameStr != NULL) {
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"%s %s", VendorNameStr, DeviceNameStr);
  } else if (VendorNameStr != NULL && DeviceNameStr == NULL) {
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"%s Unknown Device", VendorNameStr);
  } else if (VendorNameStr == NULL && DeviceNameStr != NULL) {
    UnicodeSPrint (
      StrBuffer,
      sizeof (StrBuffer),
      L"Vendor:0x%04x, %s",
      ControllerInfo->PciConfigSpace.Hdr.VendorId,
      DeviceNameStr
      );
  } else if (VendorNameStr == NULL && DeviceNameStr == NULL) {
    UnicodeSPrint (
      StrBuffer,
      sizeof (StrBuffer),
      L"Vendor:0x%04x, Device:0x%04x",
      ControllerInfo->PciConfigSpace.Hdr.VendorId,
      ControllerInfo->PciConfigSpace.Hdr.DeviceId
      );
  }

  InternalFreePool ((VOID **) &VendorNameStr);
  InternalFreePool ((VOID **) &DeviceNameStr);
  InternalFreePool ((VOID **) &BaseClassStr);
  InternalFreePool ((VOID **) &SubClassStr);
  InternalFreePool ((VOID **) &InterfaceStr);

  ControllerName = AllocateCopyPool (StrnSizeS (StrBuffer, sizeof (StrBuffer)), StrBuffer);

  return ControllerName;
}

/**
  This function installs static strings into database.

**/
VOID
InstallHiiStrings (
  VOID
  )
{
  CHAR16                *ControllerHelpStringFormat;
  CHAR16                *DriveHelpStringFormat;
  CHAR16                *EmptyPortFormat;
  LIST_ENTRY            *Node;
  BUS_CONTROLLER_INFO   *ControllerInfo;
  CHAR16                *ControllerNameStr;
  CHAR16                *DevicePathStr;
  CHAR16                StrBuffer[0x100];
  UINT32                SerialNumber;
  DISK_DRIVE_INFO       *DriveInfo;
  CHAR16                *ModelNameStr;
  CHAR16                *CapacityStr;
  CHAR16                *FormFactorStr;
  CHAR16                *RotationRateStr;
  UINTN                 Index;
  BOOLEAN               IsEmpty;

  ControllerNameStr = NULL;
  DevicePathStr     = NULL;
  ModelNameStr      = NULL;
  CapacityStr       = NULL;
  FormFactorStr     = NULL;
  RotationRateStr   = NULL;
  DriveInfo         = NULL;

  ControllerHelpStringFormat  = HiiGetString (mHiiHandle, STRING_TOKEN (STR_CONTROLLER_HELP_FORMAT), NULL);
  DriveHelpStringFormat       = HiiGetString (mHiiHandle, STRING_TOKEN (STR_DRIVE_HELP_FORMAT), NULL);
  EmptyPortFormat             = HiiGetString (mHiiHandle, STRING_TOKEN (STR_EMPTY_PORT_FORMAT), NULL);

  BASE_LIST_FOR_EACH (Node, mControllerInfoPool) {
    ControllerInfo = BUS_CONTROLLER_INFO_FROM_LINK (Node);
    SerialNumber = ControllerInfo->SerialNumber;

    ControllerNameStr = GetControllerName (ControllerInfo);
    mControllerStringIds[SerialNumber] = HiiSetString (
                                           mHiiHandle,
                                           mControllerStringIds[SerialNumber],
                                           ControllerNameStr,
                                           NULL
                                           );
    InternalFreePool ((VOID **) &ControllerNameStr);

    DevicePathStr = ConvertDevicePathToText (ControllerInfo->DevicePath, TRUE, FALSE);

    ZeroMem (StrBuffer, sizeof (StrBuffer));
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), ControllerHelpStringFormat, DevicePathStr);
    InternalFreePool ((VOID **) &DevicePathStr);

    mControllerHelpStringIds[SerialNumber] = HiiSetString (
                                               mHiiHandle,
                                               mControllerHelpStringIds[SerialNumber],
                                               StrBuffer,
                                               NULL
                                               );
  }

  BASE_LIST_FOR_EACH (Node, mDriveInfoPool) {
    DriveInfo = DISK_DRIVE_INFO_FROM_LINK (Node);
    SerialNumber = DriveInfo->SerialNumber;

    ModelNameStr = RetrieveModelNameFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
    mDriveStringIds[SerialNumber] = HiiSetString (
                                      mHiiHandle,
                                      mDriveStringIds[SerialNumber],
                                      ModelNameStr,
                                      NULL
                                      );
    InternalFreePool ((VOID **) &ModelNameStr);

    DevicePathStr   = ConvertDevicePathToText (DriveInfo->DevicePath, TRUE, FALSE);
    CapacityStr     = RetrieveCapacityFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
    FormFactorStr   = RetrieveFormFactorFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
    RotationRateStr = RetrieveRotationRateFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);

    ZeroMem (StrBuffer, sizeof (StrBuffer));
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), DriveHelpStringFormat, DevicePathStr, CapacityStr, FormFactorStr, RotationRateStr);
    InternalFreePool ((VOID **) &DevicePathStr);
    InternalFreePool ((VOID **) &CapacityStr);
    InternalFreePool ((VOID **) &FormFactorStr);
    InternalFreePool ((VOID **) &RotationRateStr);

    mDriveHelpStringIds[SerialNumber] = HiiSetString (
                                          mHiiHandle,
                                          mDriveHelpStringIds[SerialNumber],
                                          StrBuffer,
                                          NULL
                                          );
  }

  for (Index = 0; Index < mNumberOfPhysicalPorts; Index++) {
    if (mPhysicalPortNameTable[Index].DisplayName == NULL) {
      continue;
    }

    IsEmpty = TRUE;
    BASE_LIST_FOR_EACH (Node, mDriveInfoPool) {
      DriveInfo = DISK_DRIVE_INFO_FROM_LINK (Node);
      if (EfiCompareDevicePath (DriveInfo->DevicePath, mPhysicalPortNameTable[Index].DevicePath)) {
        IsEmpty = FALSE;
        break;
      }
    }

    ZeroMem (StrBuffer, sizeof (StrBuffer));
    if (IsEmpty) {
      UnicodeSPrint (StrBuffer, sizeof (StrBuffer), EmptyPortFormat, mPhysicalPortNameTable[Index].DisplayName);
    } else {
      ModelNameStr = RetrieveModelNameFromIdentifyData ((EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData);
      UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"%s: %s", mPhysicalPortNameTable[Index].DisplayName, ModelNameStr);
      InternalFreePool ((VOID **) &ModelNameStr);
    }

    mPhysicalPortStringIds[Index] = HiiSetString (mHiiHandle, mPhysicalPortStringIds[Index], StrBuffer, NULL);
  }

  InternalFreePool ((VOID **) &ControllerHelpStringFormat);
  InternalFreePool ((VOID **) &DriveHelpStringFormat);
  InternalFreePool ((VOID **) &EmptyPortFormat);
}

/**
  Helper function to create the Physical Port Name Table.

  @param[in,out]  PrivateData             The pointer to the private data.
  @param[out]     PhysicalPortNameTable   The pointer to the Physical Port Name Table.
                                          It is the caller's responsibility to free this buffer.
  @param[out]     NumberOfPhysicalPorts   Number of the entries in the table.

  @retval EFI_SUCCESS   Successfully create the table.
  @retval others        Other failure occurs.

**/
STATIC
EFI_STATUS
CreatePhysicalPortNameTable (
  IN OUT SATA_DRIVE_INFO_SETUP_PRIVATE_DATA    *PrivateData,
  OUT H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  **PhysicalPortNameTable,
  OUT UINTN                                    *NumberOfPhysicalPorts
  )
{
  H2O_DEVICE_INFO_LOOKUP_PROTOCOL          *DeviceInfoLookup;
  H2O_DEVICE_INFO_LOOKUP_ATTRIBUTES        DeviceInfoLookupAttributes;
  EFI_STATUS                               Status;
  UINTN                                    NumOfEntries;
  UINTN                                    Index;
  H2O_DEVICE_INFO_LOOKUP_NAME_TABLE_ENTRY  EntryBuffer;

  if ((PrivateData == NULL) || (PhysicalPortNameTable == NULL) || (NumberOfPhysicalPorts == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The PrivateData/PhysicalPortNameTable/NumberOfPhysicalPorts is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  *PhysicalPortNameTable = NULL;
  *NumberOfPhysicalPorts = 0;

  InitDeviceInfoLookupProtocol (&PrivateData->DeviceInfoLookup);

  DeviceInfoLookup = PrivateData->DeviceInfoLookup;
  if (DeviceInfoLookup == NULL) {
    return EFI_NOT_FOUND;
  }

  DeviceInfoLookupAttributes.Uint8 = 0;
  DeviceInfoLookupAttributes.Bits.IsSata = 1;

  Status = DeviceInfoLookup->CreateNameTable (
                               DeviceInfoLookup,
                               DeviceInfoLookupAttributes.Uint8,
                               PhysicalPortNameTable,
                               &NumOfEntries
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to CreateNameTable, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  for (Index = 0; Index < NumOfEntries; Index++) {
    if ((*PhysicalPortNameTable)[Index].DisplayName == NULL) {
      continue;
    }

    ASSERT ((*PhysicalPortNameTable)[Index].DevicePath != NULL);
    Status = FindDevicePathNodeByType (
               (*PhysicalPortNameTable)[Index].DevicePath,
               MESSAGING_DEVICE_PATH,
               MSG_SATA_DP,
               (UINT16) sizeof (SATA_DEVICE_PATH),
               NULL // Offset
               );
    if (EFI_ERROR (Status)) {
      Status = FindDevicePathNodeByType (
                 (*PhysicalPortNameTable)[Index].DevicePath,
                 MESSAGING_DEVICE_PATH,
                 MSG_SCSI_DP,
                 (UINT16) sizeof (SCSI_DEVICE_PATH),
                 NULL // Offset
                 );
      if (EFI_ERROR (Status)) {
        continue;
      }
      //
      // It's a SATA host controller, not a port.
      //
      // continue;
    }

    ASSERT (Index >= *NumberOfPhysicalPorts);
    if (Index > *NumberOfPhysicalPorts) {
      //
      // Swap entries instead of just copy. Otherwise bad things (e.g. double
      // free) can happen when we call DestroyNameTable().
      //
      EntryBuffer = (*PhysicalPortNameTable)[*NumberOfPhysicalPorts];
      (*PhysicalPortNameTable)[*NumberOfPhysicalPorts] =
        (*PhysicalPortNameTable)[Index];
      (*PhysicalPortNameTable)[Index] = EntryBuffer;
    }
    ++(*NumberOfPhysicalPorts);
  }

  return EFI_SUCCESS;
}

/**
  This function updates global data.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.
  @retval others                  Other failure occurs.

**/
EFI_STATUS
UpdateGlobalData (
  VOID
  )
{
  H2O_SATA_DRIVE_INFO_PROTOCOL    *SataDriveInfo;
  LIST_ENTRY                      *Node;
  EFI_STATUS                      Status;
  UINTN                           ConfigVarDataSize;
  UINT8                           *ConfigVarData;
  SATA_DRIVE_INFO_CONFIGURATION   *Configuration;

  SataDriveInfo = mPrivateData->SataDriveInfo;
  if (SataDriveInfo != NULL) {
    Status = SataDriveInfo->GetControllerInfoPool (SataDriveInfo, &mControllerInfoPool);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to GetControllerInfoPool, Status: %r\n", __FUNCTION__, Status));
      return Status;
    }

    Status = SataDriveInfo->GetDriveInfoPool (SataDriveInfo, &mDriveInfoPool);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to GetDriveInfoPool, Status: %r\n", __FUNCTION__, Status));
      return Status;
    }
  }

  if (mNumberOfControllers == 0) {
    BASE_LIST_FOR_EACH (Node, mControllerInfoPool) {
      mNumberOfControllers++;
    }
  }

  if (mNumberOfDrives == 0) {
    BASE_LIST_FOR_EACH (Node, mDriveInfoPool) {
      mNumberOfDrives++;
    }
  }

  if (mControllerStringIds == NULL && mNumberOfControllers > 0) {
    mControllerStringIds = AllocateZeroPool (mNumberOfControllers * sizeof (EFI_STRING_ID));
    mControllerHelpStringIds = AllocateZeroPool (mNumberOfControllers * sizeof (EFI_STRING_ID));
    if (mControllerStringIds == NULL || mControllerHelpStringIds == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  if (mDriveHelpStringIds == NULL && mNumberOfDrives > 0) {
    mDriveStringIds = AllocateZeroPool (mNumberOfDrives * sizeof (EFI_STRING_ID));
    mDriveHelpStringIds = AllocateZeroPool (mNumberOfDrives * sizeof (EFI_STRING_ID));
    if (mDriveStringIds == NULL || mDriveHelpStringIds == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  if (mPhysicalPortNameTable == NULL) {
    Status = CreatePhysicalPortNameTable (
                mPrivateData,
                &mPhysicalPortNameTable,
                &mNumberOfPhysicalPorts
                );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "%a() - Fail to CreatePhysicalPortNameTable, Status: %r\n", __FUNCTION__, Status));
    }
  }

  if (mPhysicalPortStringIds == NULL && mNumberOfPhysicalPorts > 0) {
    mPhysicalPortStringIds = AllocateZeroPool (mNumberOfPhysicalPorts * sizeof (EFI_STRING_ID));
    if (mPhysicalPortStringIds == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }

  InstallHiiStrings ();

  Status = CommonGetVariableDataAndSize (
             mVariableName,
             &gH2OSataDriveInfoConfigurationGuid,
             &ConfigVarDataSize,
             (VOID **) &ConfigVarData
             );
  if (!EFI_ERROR (Status)) {
    Configuration = (SATA_DRIVE_INFO_CONFIGURATION *) ConfigVarData;
    Configuration->NumberOfPhysicalPorts = (UINT8) mNumberOfPhysicalPorts;
    if (mNumberOfPhysicalPorts > 0xFF) {
      Configuration->NumberOfPhysicalPorts = 0xFF;
    }
    CommonSetVariable (
      mVariableName,
      &gH2OSataDriveInfoConfigurationGuid,
      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
      sizeof (SATA_DRIVE_INFO_CONFIGURATION),
      Configuration
      );
    FreePool (Configuration);
  }

  DEBUG ((EFI_D_ERROR, "%a() - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Helper function to create opcodes for the Bus Controller Info.

  @param[in]  ControllerInfo      The pointer to the BUS_CONTROLLER_INFO instance.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
CreateOpCodeForController (
  IN  BUS_CONTROLLER_INFO *ControllerInfo,
  IN  VOID                *StartOpCodeHandle
  )
{
  UINT32  SerialNumber;

  SerialNumber = ControllerInfo->SerialNumber;

  HiiCreateActionOpCode (
    StartOpCodeHandle,
    (EFI_QUESTION_ID)(H2O_SATA_DRIVE_INFO_DYNAMIC_CONTROLLER_BASE_QUESTION_ID + SerialNumber),
    mControllerStringIds[SerialNumber],
    mControllerHelpStringIds[SerialNumber],
    EFI_IFR_FLAG_CALLBACK,
    0
    );
}

/**
  Helper function to create opcodes for the Drive.

  @param[in]  DriveInfo           The pointer to the DISK_DRIVE_INFO instance.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
CreateOpCodeForDrive (
  IN  DISK_DRIVE_INFO *DriveInfo,
  IN  VOID            *StartOpCodeHandle
  )
{
  UINT32  SerialNumber;

  SerialNumber = DriveInfo->SerialNumber;

  HiiCreateGotoOpCode (
    StartOpCodeHandle,
    H2O_SATA_DRIVE_INFO_DRIVE_INFO_FORM_ID,
    mDriveStringIds[SerialNumber],
    mDriveHelpStringIds[SerialNumber],
    EFI_IFR_FLAG_CALLBACK,
    (EFI_QUESTION_ID)(H2O_SATA_DRIVE_INFO_DYNAMIC_DRIVE_BASE_QUESTION_ID + SerialNumber)
    );
}

/**
  This function traverse the controllers in the pool and
  create opcodes for them and each child.

  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
ListDrivesByControllers (
  IN  VOID  *StartOpCodeHandle
  )
{
  LIST_ENTRY              *ControllerNode;
  BUS_CONTROLLER_INFO     *ControllerInfo;
  LIST_ENTRY              *ChildNode;
  DISK_DRIVE_INFO         *DriveInfo;

  BASE_LIST_FOR_EACH (ControllerNode, mControllerInfoPool) {
    ControllerInfo = BUS_CONTROLLER_INFO_FROM_LINK (ControllerNode);
    CreateOpCodeForController (ControllerInfo, StartOpCodeHandle);

    if (IsListEmpty (&ControllerInfo->ChildList)) {
      HiiCreateGrayOutIfOpCodeEx (StartOpCodeHandle, 1);
      HiiCreateTrueOpCode (StartOpCodeHandle);
      HiiCreateActionOpCode (StartOpCodeHandle, 0, STRING_TOKEN (STR_NO_DRIVE_DETECTED), 0, 0, 0);
      HiiCreateEndOpCode (StartOpCodeHandle);
    } else {
      BASE_LIST_FOR_EACH (ChildNode, &ControllerInfo->ChildList) {
        DriveInfo = DISK_DRIVE_INFO_FROM_CHILD_LINK (ChildNode);
        CreateOpCodeForDrive (DriveInfo, StartOpCodeHandle);
      }
    }

    HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_EMPTY), 0, 0, 1);
  }
}

/**
  Helper function to create opcodes for the Physical Port.

  @param[in]  Index               The index of the Physical Port in the Table.
  @param[in]  DriveInfo           The pointer to the DISK_DRIVE_INFO instance.
                                  If there is no drive on this port, this field can be set to NULL.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
CreateOpCodeForPhysicalPort (
  IN  UINTN           Index,
  IN  DISK_DRIVE_INFO *DriveInfo,         OPTIONAL
  IN  VOID            *StartOpCodeHandle
  )
{
  if (DriveInfo != NULL) {
    HiiCreateGotoOpCode (
      StartOpCodeHandle,
      H2O_SATA_DRIVE_INFO_DRIVE_INFO_FORM_ID,
      mPhysicalPortStringIds[Index],
      mDriveHelpStringIds[DriveInfo->SerialNumber],
      EFI_IFR_FLAG_CALLBACK,
      (EFI_QUESTION_ID)(H2O_SATA_DRIVE_INFO_DYNAMIC_DRIVE_BASE_QUESTION_ID + DriveInfo->SerialNumber)
      );
  } else {
    if (!mPrivateData->Configuration.HideEmptyPorts) {
      HiiCreateGotoOpCode (
        StartOpCodeHandle,
        H2O_SATA_DRIVE_INFO_ROOT_FORM_ID,
        mPhysicalPortStringIds[Index],
        0,
        EFI_IFR_FLAG_CALLBACK,
        (EFI_QUESTION_ID)(H2O_SATA_DRIVE_INFO_DYNAMIC_PORT_BASE_QUESTION_ID + Index)
        );
    }
  }
}

/**
  This function traverse the physical ports in the table and
  create opcodes for them.

  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
ListDrivesByPhysicalPorts (
  IN  VOID  *StartOpCodeHandle
  )
{
  UINTN             Index;
  LIST_ENTRY        *DriveNode;
  DISK_DRIVE_INFO   *DriveInfo;
  BOOLEAN           IsEmpty;
  CHAR16            *EmptyPortFormatStr;

  DriveInfo = NULL;
  EmptyPortFormatStr = NULL;

  for (Index = 0; Index < mNumberOfPhysicalPorts; Index++) {
    if (mPhysicalPortNameTable[Index].DisplayName == NULL) {
      continue;
    }

    IsEmpty = TRUE;
    BASE_LIST_FOR_EACH (DriveNode, mDriveInfoPool) {
      DriveInfo = DISK_DRIVE_INFO_FROM_LINK (DriveNode);
      if (EfiCompareDevicePath (DriveInfo->DevicePath, mPhysicalPortNameTable[Index].DevicePath)) {
        IsEmpty = FALSE;
        break;
      }
    }

    CreateOpCodeForPhysicalPort (Index, (!IsEmpty)? DriveInfo : NULL, StartOpCodeHandle);
  }
}

/**
  This function updates root form.

**/
VOID
UpdateForm (
  VOID
  )
{
  CHAR16              *StatisticsFormatByControllers;
  CHAR16              *StatisticsFormatByPhysicalPorts;
  VOID                *StartOpCodeHandle;
  VOID                *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  EFI_IFR_GUID_LABEL  *EndLabel;
  CHAR16              StrBuffer[100];
  EFI_STATUS          Status;

  StatisticsFormatByControllers   = HiiGetString (mHiiHandle, STRING_TOKEN (STR_STATISTICS_FORMAT_BY_CONTROLLERS), NULL);
  StatisticsFormatByPhysicalPorts = HiiGetString (mHiiHandle, STRING_TOKEN (STR_STATISTICS_FORMAT_BY_PHYSICAL_PORTS), NULL);

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel                = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode  = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number        = H2O_SATA_DRIVE_INFO_DRIVE_LIST_LABEL_START;

  EndLabel                  = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode    = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number          = H2O_SATA_DRIVE_INFO_DRIVE_LIST_LABEL_END;

  ZeroMem (StrBuffer, sizeof (StrBuffer));

  switch (mPrivateData->Configuration.DisplayMode) {

  case ByControllers:
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), StatisticsFormatByControllers, mNumberOfControllers, mNumberOfDrives);
    ListDrivesByControllers (StartOpCodeHandle);
    break;

  case ByPhysicalPorts:
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), StatisticsFormatByPhysicalPorts, mNumberOfPhysicalPorts);
    ListDrivesByPhysicalPorts (StartOpCodeHandle);
    break;

  default:
    ASSERT (FALSE);   // should not reach here
    break;
  }

  InternalFreePool ((VOID **) &StatisticsFormatByControllers);
  InternalFreePool ((VOID **) &StatisticsFormatByPhysicalPorts);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_STATISTICS), StrBuffer, NULL);

  Status = HiiUpdateForm (
             mHiiHandle,
             &gH2OSataDriveInfoSetupFormsetGuid,
             H2O_SATA_DRIVE_INFO_ROOT_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HiiUpdateForm, Status: %r\n", __FUNCTION__, Status));
  }

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  This function installs static strings into database for the sub form.

  @param[in]  DriveInfo   The pointer to the DISK_DRIVE_INFO instance.

**/
VOID
InstallHiiStringsForSubForm (
  IN  DISK_DRIVE_INFO   *DriveInfo
  )
{
  EFI_IDENTIFY_DATA           *EfiIdentifyData;
  CHAR16                      *ModelNameStr;
  CHAR16                      *DevicePathStr;
  CHAR16                      *ParentDevicePathStr;
  CHAR16                      *SerialNoStr;
  CHAR16                      *CapacityStr;
  CHAR16                      *FormFactorStr;
  CHAR16                      *RotationRateStr;
  ATA_SMART_ATTRIBUTE_ENTRY   *SmartAttrib;
  UINTN                       Index;
  CHAR16                      StrBuffer[0x100];
  CHAR16                      *SmartAttribNameStr;
  CHAR16                      *SmartAttribHelpStrFormat;

  if (DriveInfo == NULL) {
    return;
  }

  EfiIdentifyData           = (EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData;
  ModelNameStr              = NULL;
  DevicePathStr             = NULL;
  ParentDevicePathStr       = NULL;
  SerialNoStr               = NULL;
  CapacityStr               = NULL;
  FormFactorStr             = NULL;
  RotationRateStr           = NULL;
  SmartAttrib               = NULL;
  SmartAttribNameStr        = NULL;
  SmartAttribHelpStrFormat  = NULL;

  ModelNameStr        = RetrieveModelNameFromIdentifyData (EfiIdentifyData);
  DevicePathStr       = ConvertDevicePathToText (DriveInfo->DevicePath, TRUE, FALSE);
  ParentDevicePathStr = ConvertDevicePathToText (DriveInfo->Parent->DevicePath, TRUE, FALSE);
  SerialNoStr         = RetrieveSerialNumberFromIdentifyData (EfiIdentifyData);
  CapacityStr         = RetrieveCapacityFromIdentifyData (EfiIdentifyData);
  FormFactorStr       = RetrieveFormFactorFromIdentifyData (EfiIdentifyData);
  RotationRateStr     = RetrieveRotationRateFromIdentifyData (EfiIdentifyData);

  HiiSetString (mHiiHandle, STRING_TOKEN (STR_SUBFORM_TITLE), ModelNameStr, NULL);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_DEVICE_PATH_VALUE), DevicePathStr, NULL);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_PARENT_DEVICE_PATH_VALUE), ParentDevicePathStr, NULL);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_SERIAL_NUMBER_VALUE), SerialNoStr, NULL);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_CAPACITY_VALUE), CapacityStr, NULL);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_FORM_FACTOR_VALUE), FormFactorStr, NULL);
  HiiSetString (mHiiHandle, STRING_TOKEN (STR_ROTATION_RATE_VALUE), RotationRateStr, NULL);

  InternalFreePool ((VOID **) &ModelNameStr);
  InternalFreePool ((VOID **) &DevicePathStr);
  InternalFreePool ((VOID **) &ParentDevicePathStr);
  InternalFreePool ((VOID **) &SerialNoStr);
  InternalFreePool ((VOID **) &CapacityStr);
  InternalFreePool ((VOID **) &FormFactorStr);
  InternalFreePool ((VOID **) &RotationRateStr);

  if (DriveInfo->SmartData != NULL) {
    SmartAttribHelpStrFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_SMART_ATTRIBUTE_HELP_FORMAT), NULL);

    SmartAttrib = DriveInfo->SmartData->Attrib;
    for (Index = 0; Index < MAX_SMART_ATTRIBUTE_ENTRY; Index++) {
      if (SmartAttrib[Index].Id == 0) {
        continue;
      }

      SmartAttribNameStr = GetSmartAttributeNameById (SmartAttrib[Index].Id);

      ZeroMem (StrBuffer, sizeof (StrBuffer));
      UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"[%02xh] %s", SmartAttrib[Index].Id, SmartAttribNameStr);
      InternalFreePool ((VOID **) &SmartAttribNameStr);

      mSmartAttributeStringIds[Index] = HiiSetString (
                                          mHiiHandle,
                                          mSmartAttributeStringIds[Index],
                                          StrBuffer,
                                          NULL
                                          );

      ZeroMem (StrBuffer, sizeof (StrBuffer));
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        SmartAttribHelpStrFormat,
        SmartAttrib[Index].Flags,
        SmartAttrib[Index].Value,
        SmartAttrib[Index].Raw
        );

      mSmartAttributeHelpStringIds[Index] = HiiSetString (
                                              mHiiHandle,
                                              mSmartAttributeHelpStringIds[Index],
                                              StrBuffer,
                                              NULL
                                              );
    }

    InternalFreePool ((VOID **) &SmartAttribHelpStrFormat);
  }
}

/**
  Helper function to create opcodes for showing the S.M.A.R.T Information,
  including the supported/enabled state and the Return Status.

  @param[in]  DriveInfo           The pointer to the DISK_DRIVE_INFO instance.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
CreateOpCodeForSmartStatus (
  IN  DISK_DRIVE_INFO   *DriveInfo,
  IN  VOID              *StartOpCodeHandle
  )
{
  EFI_IDENTIFY_DATA   *EfiIdentifyData;
  EFI_STRING_ID       SmartReturnStatusStringId;

  EfiIdentifyData = (EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData;

  HiiCreateActionOpCodeEx (
    StartOpCodeHandle,
    H2O_SATA_DRIVE_INFO_SMART_SUPPORTED_QUESTION_ID,
    STRING_TOKEN (STR_SMART_SUPPORTED_TEXT),
    STRING_TOKEN (STR_SMART_SUPPORTED_HELP),
    IsSmartSupported (EfiIdentifyData)? STRING_TOKEN (STR_SUPPORTED) : STRING_TOKEN (STR_NOT_SUPPORTED),
    0,
    0
    );

  if (IsSmartSupported (EfiIdentifyData)) {
    HiiCreateActionOpCodeEx (
      StartOpCodeHandle,
      H2O_SATA_DRIVE_INFO_SMART_ENABLED_QUESTION_ID,
      STRING_TOKEN (STR_SMART_ENABLED_TEXT),
      STRING_TOKEN (STR_SMART_ENABLED_HELP),
      IsSmartEnabled (EfiIdentifyData)? STRING_TOKEN (STR_ENABLED) : STRING_TOKEN (STR_DISABLED),
      0,
      0
      );

    SmartReturnStatusStringId = STRING_TOKEN (STR_NOT_AVAILABLE);
    if (DriveInfo->SmartReturnStatus == EFI_SUCCESS) {
      SmartReturnStatusStringId = STRING_TOKEN (STR_SMART_NO_ERROR);
    } else if (DriveInfo->SmartReturnStatus == EFI_DEVICE_ERROR) {
        SmartReturnStatusStringId = STRING_TOKEN (STR_SMART_DEVICE_ERROR);
    }

    if (IsSmartEnabled (EfiIdentifyData)) {
      HiiCreateActionOpCodeEx (
        StartOpCodeHandle,
        H2O_SATA_DRIVE_INFO_SMART_STATUS_QUESTION_ID,
        (EFI_STRING_ID) STRING_TOKEN (STR_SMART_STATUS_TEXT),
        (EFI_STRING_ID) STRING_TOKEN (STR_SMART_STATUS_HELP),
        SmartReturnStatusStringId,
        0,
        0
        );
    }
  }
}

/**
  Helper function to create opcodes for showing the S.M.A.R.T Attributes.

  @param[in]  DriveInfo           The pointer to the DISK_DRIVE_INFO instance.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.

**/
VOID
CreateOpCodeForSmartAttrib (
  IN  DISK_DRIVE_INFO   *DriveInfo,
  IN  VOID              *StartOpCodeHandle
  )
{
  UINTN   Index;

  if (DriveInfo->SmartData != NULL) {
    for (Index = 0; Index < MAX_SMART_ATTRIBUTE_ENTRY; Index++) {
      if (DriveInfo->SmartData->Attrib[Index].Id == 0) {
        continue;
      }

      HiiCreateActionOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(H2O_SATA_DRIVE_INFO_DYNAMIC_SMART_ATTRIBUTE_BASE_QUESTION_ID + Index),
        mSmartAttributeStringIds[Index],
        mSmartAttributeHelpStringIds[Index],
        0,
        0
        );
    }
  }
}

/**
  This function updates the sub form.

  @param[in]  QuestionId  Question ID.

**/
VOID
UpdateSubForm (
  IN  EFI_QUESTION_ID   QuestionId
  )
{
  UINT32              SerialNumber;
  LIST_ENTRY          *Node;
  DISK_DRIVE_INFO     *DriveInfo;
  VOID                *StartOpCodeHandle;
  VOID                *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  EFI_IFR_GUID_LABEL  *EndLabel;
  EFI_IDENTIFY_DATA   *EfiIdentifyData;
  EFI_STATUS          Status;

  DriveInfo = NULL;
  SerialNumber = (UINT32) QuestionId & B_SETUP_POLICY_SERIAL;

  BASE_LIST_FOR_EACH (Node, mDriveInfoPool) {
    DriveInfo = DISK_DRIVE_INFO_FROM_LINK (Node);
    if (DriveInfo->SerialNumber == SerialNumber) {
      break;
    }
  }

  if ((Node == mDriveInfoPool) || (DriveInfo == NULL)) {
    return;
  }

  InstallHiiStringsForSubForm (DriveInfo);

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (StartOpCodeHandle)\n", __FUNCTION__));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to AllocateOpCodeHandle (EndOpCodeHandle)\n", __FUNCTION__));
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    return;
  }

  StartLabel                = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode  = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number        = H2O_SATA_DRIVE_INFO_SMART_ATTRIB_LIST_LABEL_START;

  EndLabel                  = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode    = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number          = H2O_SATA_DRIVE_INFO_SMART_ATTRIB_LIST_LABEL_END;

  CreateOpCodeForSmartStatus (DriveInfo, StartOpCodeHandle);
  EfiIdentifyData = (EFI_IDENTIFY_DATA *) DriveInfo->IdentifyData;
  if (IsSmartSupported (EfiIdentifyData) && IsSmartEnabled (EfiIdentifyData)) {
    HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_EMPTY), 0, 0, 1);
    CreateOpCodeForSmartAttrib (DriveInfo, StartOpCodeHandle);
  }

  Status = HiiUpdateForm (
             mHiiHandle,
             &gH2OSataDriveInfoSetupFormsetGuid,
             H2O_SATA_DRIVE_INFO_DRIVE_INFO_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HiiUpdateForm, Status: %r\n", __FUNCTION__, Status));
  }

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

/**
  This function processes the results of changes in configuration.
  (i.e. when SCU settings has been adjusted)

  @param[in]       This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Action                   Specifies the type of action taken by the browser.
  @param[in]       QuestionId               A unique value which is sent to the original
                                            exporting driver so that it can identify the type
                                            of data to expect.
  @param[in]       Type                     The type of value for the question.
  @param[in]       Value                    A pointer to the data being sent to the original
                                            exporting driver.
  @param[out]      ActionRequest            On return, points to the action requested by the
                                            callback function.

  @retval          EFI_SUCCESS              The callback successfully handled the action.
  @retval          EFI_OUT_OF_RESOURCES     Not enough storage is available to hold the
                                            variable and its data.
  @retval          EFI_DEVICE_ERROR         The variable could not be saved.
  @retval          EFI_UNSUPPORTED          The specified Action is not supported by the
                                            callback.
**/
EFI_STATUS
EFIAPI
DriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN        EFI_BROWSER_ACTION                Action,
  IN        EFI_QUESTION_ID                   QuestionId,
  IN        UINT8                             Type,
  IN        EFI_IFR_TYPE_VALUE                *Value,
  OUT       EFI_BROWSER_ACTION_REQUEST        *ActionRequest
  )
{
  SATA_DRIVE_INFO_SETUP_PRIVATE_DATA  *PrivateData;
  CHAR8                               *PlatformLangVar;

  if (((Value == NULL) && (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
    (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = SATA_DRIVE_INFO_PRIVATE_FROM_THIS (This);

  switch (Action) {

  case EFI_BROWSER_ACTION_FORM_OPEN:
    if (QuestionId == H2O_SATA_DRIVE_INFO_FORM_REFRESH_QUESTION_ID) {
      GetEfiGlobalVariable2 (EFI_PLATFORM_LANG_VARIABLE_NAME, (VOID **) &PlatformLangVar, NULL);
      if (PlatformLangVar != NULL) {
        if (mPlatformLang != NULL) {
          if (AsciiStrCmp (PlatformLangVar, mPlatformLang) != 0) {
            InternalFreePool ((VOID **) &mPlatformLang);
            mPlatformLang = PlatformLangVar;
            InstallHiiStrings ();
          }
        } else {
          mPlatformLang = PlatformLangVar;
          InstallHiiStrings ();
        }
      }
      HiiGetBrowserData (
        &gH2OSataDriveInfoConfigurationGuid,
        mVariableName,
        sizeof (SATA_DRIVE_INFO_CONFIGURATION),
        (UINT8 *) &PrivateData->Configuration
        );
      PrivateData->Configuration.NumberOfPhysicalPorts = (UINT8) mNumberOfPhysicalPorts;
      UpdateForm ();
    }
    return EFI_UNSUPPORTED;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    if (QuestionId == H2O_SATA_DRIVE_INFO_FORM_REFRESH_QUESTION_ID) {
      HiiGetBrowserData (
        &gH2OSataDriveInfoConfigurationGuid,
        mVariableName,
        sizeof (SATA_DRIVE_INFO_CONFIGURATION),
        (UINT8 *) &PrivateData->Configuration
        );
      PrivateData->Configuration.NumberOfPhysicalPorts = (UINT8) mNumberOfPhysicalPorts;
      UpdateForm ();
    } else if (QuestionId == H2O_SATA_DRIVE_INFO_NUMBER_OF_PHYSICAL_PORTS_QUESTION_ID) {
      Value->u8 = (UINT8) mNumberOfPhysicalPorts;
      PrivateData->Configuration.NumberOfPhysicalPorts = Value->u8;
      return EFI_SUCCESS;
    }
    return EFI_UNSUPPORTED;

  case EFI_BROWSER_ACTION_CHANGING:
    if (QuestionId == H2O_SATA_DRIVE_INFO_HIDE_EMPTY_PORTS_QUESTION_ID) {
      PrivateData->Configuration.HideEmptyPorts = Value->u8;
      UpdateForm ();
    }
    if (QuestionId == H2O_SATA_DRIVE_INFO_DISPLAY_MODE_QUESTION_ID) {
      PrivateData->Configuration.DisplayMode = Value->u8;
      UpdateForm ();
    }
    if ((QuestionId & B_SETUP_POLICY_DYNAMIC_BASE) == H2O_SATA_DRIVE_INFO_DYNAMIC_DRIVE_BASE_QUESTION_ID) {
      UpdateSubForm (QuestionId);
    }
    break;

  default:
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function processes the results of changes in configuration.
  (i.e. [F10] -> "Save and exit" has been pressed)

  @param[in]       This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Configuration            A null-terminated Unicode string in <ConfigResp>
                                            format.
  @param[out]      Progress                 A pointer to a string filled in with the offset of
                                            the most recent '&' before the first failing
                                            name/value pair (or the beginning of the string if
                                            the failure is in the first name/value pair) or
                                            the terminating NULL if all was successful.

  @retval          EFI_SUCCESS              The Results is processed successfully.
  @retval          EFI_INVALID_PARAMETER    Configuration is NULL.
  @retval          EFI_NOT_FOUND            Routing data doesn't match any storage in this
                                            driver.
*/
EFI_STATUS
EFIAPI
RouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Configuration,
  OUT       EFI_STRING                        *Progress
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;
  SATA_DRIVE_INFO_SETUP_PRIVATE_DATA  *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL     *HiiConfigRouting;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "SataDriveInfoSetupUtility %a: Start\n", __FUNCTION__));

  PrivateData       = SATA_DRIVE_INFO_PRIVATE_FROM_THIS (This);
  HiiConfigRouting  = PrivateData->HiiConfigRouting;
  *Progress         = Configuration;

  if (!HiiIsConfigHdrMatch(Configuration, &gH2OSataDriveInfoConfigurationGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (SATA_DRIVE_INFO_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OSataDriveInfoConfigurationGuid,
             &BufferSize,
             &PrivateData->Configuration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to CommonGetVariable, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Check if configuring Name/Value storage
  //
  if (StrStr (Configuration, L"OFFSET") == NULL) {
    return EFI_UNSUPPORTED;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (SATA_DRIVE_INFO_CONFIGURATION);
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               (UINT8 *) &PrivateData->Configuration,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to ConfigToBlock, Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  Status = CommonSetVariable (
             mVariableName,
             &gH2OSataDriveInfoConfigurationGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof (SATA_DRIVE_INFO_CONFIGURATION),
             &PrivateData->Configuration
             );

  DEBUG ((EFI_D_INFO, "SataDriveInfoSetupUtility %a: End, Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function allows a caller to extract the current configuration for one
  or more named elements from the target driver.
  (i.e. head in "Setup Utility" from front page)

  @param[in]       This                     Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Request                  A null-terminated Unicode string in <ConfigRequest> format.
  @param[out]      Progress                 On return, points to a character in the Request
                                            string. Points to the string's null terminator if
                                            request was successful. Points to the most recent
                                            '&' before the first failing name/value pair (or
                                            the beginning of the string if the failure is in
                                            the first name/value pair) if the request was not
                                            successful.
  @param[out]      Results                  A null-terminated Unicode string in
                                            <ConfigAltResp> format which has all values filled
                                            in for the names in the Request string. String to
                                            be allocated by the called function.

  @retval          EFI_SUCCESS              The Results is filled with the requested values.
  @retval          EFI_OUT_OF_RESOURCES     Not enough memory to store the results.
  @retval          EFI_INVALID_PARAMETER    Request is illegal syntax, or unknown name.
  @retval          EFI_NOT_FOUND            Routing data doesn't match any storage in this
                                            driver.
**/
EFI_STATUS
EFIAPI
ExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL    *This,
  IN  CONST EFI_STRING                        Request,
  OUT       EFI_STRING                        *Progress,
  OUT       EFI_STRING                        *Results
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;
  SATA_DRIVE_INFO_SETUP_PRIVATE_DATA  *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL     *HiiConfigRouting;
  EFI_STRING                          ConfigRequest;
  EFI_STRING                          ConfigRequestHdr;
  UINTN                               Size;
  CHAR16                              *StrPointer;
  BOOLEAN                             AllocatedRequest;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "SataDriveInfoSetupUtility %a: Start\n", __FUNCTION__));

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr  = NULL;
  ConfigRequest     = Request;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;

  PrivateData       = SATA_DRIVE_INFO_PRIVATE_FROM_THIS (This);
  HiiConfigRouting  = PrivateData->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (SATA_DRIVE_INFO_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OSataDriveInfoConfigurationGuid,
             &BufferSize,
             &PrivateData->Configuration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to CommonGetVariable, Status = %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  PrivateData->Configuration.NumberOfPhysicalPorts = (UINT8) mNumberOfPhysicalPorts;

  if (Request == NULL) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gH2OSataDriveInfoConfigurationGuid, mVariableName, PrivateData->DriverHandle);
    if (ConfigRequestHdr == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      FreePool (ConfigRequestHdr);
      return EFI_OUT_OF_RESOURCES;
    }
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64) BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &gH2OSataDriveInfoConfigurationGuid, mVariableName)) {
      DEBUG ((EFI_D_INFO, "%a() - ConfigHdr is not matched with this driver\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }

    //
    // Set Request to the unified request string.
    //
    ConfigRequest = Request;
    //
    // Check whether Request includes Request Element.
    //
    if (StrStr (Request, L"OFFSET") == NULL) {
      //
      // Check Request Element does exist in Request String
      //
      StrPointer = StrStr (Request, L"PATH");
      if (StrPointer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (StrStr (StrPointer, L"&") == NULL) {
        Size = (StrLen (Request) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest    = AllocateZeroPool (Size);
        if (ConfigRequest == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64) BufferSize);
      }
    }
  }

  if (StrStr (ConfigRequest, L"OFFSET") == NULL) {
    //
    // If requesting Name/Value storage, return not found.
    //
    return EFI_NOT_FOUND;
  }

  //
  // Convert buffer data to <ConfigResp> by helper function BlockToConfig()
  //
  Status = HiiConfigRouting->BlockToConfig (
                               HiiConfigRouting,
                               ConfigRequest,
                               (UINT8 *) &PrivateData->Configuration,
                               BufferSize,
                               Results,
                               Progress
                               );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  DEBUG ((EFI_D_INFO, "SataDriveInfoSetupUtility %a: End, Status: %r\n", __FUNCTION__, Status));

  return Status;
}

/**
  Handler to trigger form update process when entering SCU.

  @param[in]  Event   The Event that is being processed.
  @param[in]  Context The Event Context.

**/
VOID
SetupUtilityAppCallback (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  EFI_STATUS                              Status;
  EFI_SETUP_UTILITY_APPLICATION_PROTOCOL  *SetupUtilityApp;
  H2O_SATA_DRIVE_INFO_PROTOCOL            *SataDriveInfo;

  DEBUG ((EFI_D_INFO, "SataDriveInfoSetupUtility %a: Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gEfiSetupUtilityApplicationProtocolGuid, NULL, (VOID **) &SetupUtilityApp);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "SetupUtilityApp->VfrDriverState = %d\n", SetupUtilityApp->VfrDriverState));

  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    Status = gBS->LocateProtocol (&gH2OSataDriveInfoProtocolGuid, NULL, (VOID **) &SataDriveInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_ERROR,
        "%a() - Fail to locate protocol %g, Status: %r\n",
        __FUNCTION__,
        gH2OSataDriveInfoProtocolGuid,
        Status
        ));
      SataDriveInfo = NULL;
    }

    mPrivateData->SataDriveInfo = SataDriveInfo;

    UpdateGlobalData ();
    UpdateForm ();
  }

  DEBUG ((EFI_D_INFO, "SataDriveInfoSetupUtility %a: End\n", __FUNCTION__));
}

/**
  Entry point of SataDriveInfoSetupUtilityDxe

  @param[in]  ImageHandle     The image handle.
  @param[in]  SystemTable     The system table.

  @retval   EFI_SUCCESS     Function complete successfully.
  @retval   others          Function complete unsuccessfully.

**/
EFI_STATUS
SataDriveInfoSetupUtilityDxeEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status;
  VOID                            *Registration;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRouting;
  EFI_STRING                      ConfigRequestHdr;
  UINTN                           BufferSize;

  DEBUG ((EFI_D_ERROR, "%a() - Start\n", __FUNCTION__));

  mPrivateData = AllocateZeroPool (sizeof (SATA_DRIVE_INFO_SETUP_PRIVATE_DATA));
  if (mPrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mPrivateData->Signature                   = SATA_DRIVE_INFO_SETUP_SIGNATURE;
  mPrivateData->ConfigAccess.ExtractConfig  = ExtractConfig;
  mPrivateData->ConfigAccess.RouteConfig    = RouteConfig;
  mPrivateData->ConfigAccess.Callback       = DriverCallback;

  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **) &HiiConfigRouting);
  if (EFI_ERROR (Status)) {
    goto ErrorExit;
  }
  mPrivateData->HiiConfigRouting            = HiiConfigRouting;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPrivateData->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &mPrivateData->ConfigAccess,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to InstallMultipleProtocolInterfaces(), Status: %r\n", __FUNCTION__, Status));
    goto ErrorExit;
  }

  mPrivateData->HiiHandle = HiiAddPackages (
                              &gH2OSataDriveInfoSetupFormsetGuid,
                              mPrivateData->DriverHandle,
                              SataDriveInfoVfrBin,
                              SataDriveInfoSetupUtilityDxeStrings,
                              NULL
                              );
  if (mPrivateData->HiiHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HiiAddPackages()", __FUNCTION__));

    Status = gBS->UninstallMultipleProtocolInterfaces (
                    &mPrivateData->DriverHandle,
                    &gEfiDevicePathProtocolGuid,
                    &mHiiVendorDevicePath,
                    &gEfiHiiConfigAccessProtocolGuid,
                    &mPrivateData->ConfigAccess,
                    NULL
                    );

    goto ErrorExit;
  }

  mHiiHandle = mPrivateData->HiiHandle;

  //
  // Check the status of the setup variable. Note that it has a read-only
  // default value stored in the variable defaults region.
  //
  BufferSize = sizeof (SATA_DRIVE_INFO_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OSataDriveInfoConfigurationGuid,
             &BufferSize,
             &mPrivateData->Configuration
             );
  if (EFI_ERROR (Status) ||
      !IsVariableInVariableStoreRegion (mVariableName, &gH2OSataDriveInfoConfigurationGuid)) {
    BufferSize = sizeof (SATA_DRIVE_INFO_CONFIGURATION);
    Status = InitVarDefault (
               mVariableName,
               &gH2OSataDriveInfoConfigurationGuid,
               &BufferSize,
               &mPrivateData->Configuration
               );
    if (EFI_ERROR (Status)) {
      goto ErrorExit;
    }
  }

  ConfigRequestHdr = HiiConstructConfigHdr (
                       &gH2OSataDriveInfoConfigurationGuid,
                       mVariableName,
                       mPrivateData->DriverHandle
                       );
  if (ConfigRequestHdr == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ErrorExit;
  }

  if (!HiiValidateSettings (ConfigRequestHdr)) {
    FreePool (ConfigRequestHdr);
    Status = EFI_INVALID_PARAMETER;
    goto ErrorExit;
  }

  FreePool (ConfigRequestHdr);

  //
  // Register a callback function.
  //
  Status = EfiNamedEventListen (
             &gEfiSetupUtilityApplicationProtocolGuid,
             TPL_CALLBACK - 1,
             SetupUtilityAppCallback,
             NULL,
             &Registration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to listen gEfiSetupUtilityApplicationProtocolGuid(%g), Status: %r\n",
      __FUNCTION__,
      gEfiSetupUtilityApplicationProtocolGuid,
      Status
      ));
    return Status;
  }

  return EFI_SUCCESS;

ErrorExit:
  DEBUG ((EFI_D_ERROR, "%a() - End, Status = %r\n", __FUNCTION__, Status));

  FreePool (mPrivateData);

  return Status;
}
