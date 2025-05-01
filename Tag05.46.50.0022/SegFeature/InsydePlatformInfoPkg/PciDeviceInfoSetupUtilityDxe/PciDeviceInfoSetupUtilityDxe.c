/** @file
  Implementation of PciDeviceInfoSetupUtilityDxe.

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "PciDeviceInfoSetupUtilityDxe.h"

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/VariableLib.h>
#include <Library/DeviceInfoLookupDataLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciCapabilityIntnTableLib.h>
#include <Library/DxeSetupVariableLib.h>

#include <Protocol/DevicePath.h>
#include <Protocol/H2OPciDeviceInfo.h>
#include <Protocol/H2OPciLookupTable.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SetupUtilityApplication.h>

#include <Guid/GlobalVariable.h>
#include <Guid/MdeModuleHii.h>
#include <Guid/PciDeviceInfoSetupUtilityHii.h>

#include "PciDeviceInfoSetupNVDataStruc.h"

extern UINT8  PciDeviceInfoVfrBin[];
extern UINT8  PciDeviceInfoSetupUtilityDxeStrings[];

#define DEFAULT_PCI_CAPABILITY_STRING_ID_BUFFER_SIZE         100
#define DEFAULT_PCIE_EXTEND_CAPABILITY_STRING_ID_BUFFER_SIZE 100

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

STATIC  PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA  *mPrivateData = NULL;
STATIC  HII_VENDOR_DEVICE_PATH                      mHiiVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    PCI_DEVICE_INFO_SETUP_FORMSET_GUID
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
STATIC  PCI_DEVICE_INFO             *mCurrentPciDeviceInfo      = NULL;
STATIC  CHAR16                      *mVariableName              = PCI_DEVICE_INFO_CONFIGURATION_VARIABLE_NAME;

STATIC  LIST_ENTRY                  *mPciDeviceInfoPool         = NULL;

STATIC  UINTN                       mNumberOfPciDevices         = 0;
STATIC  EFI_STRING_ID               *mPciDeviceStringIds        = NULL;
STATIC  EFI_STRING_ID               *mPciDeviceHelpStringIds    = NULL;

STATIC  CAPABILITY_DISPLAY_CONTROL_TABLE_ENTRY  *mCapDisplayCtrlTable                   = NULL;
STATIC  UINTN                                   mCapDisplayCtrlTableEntryCount          = 0;

STATIC  EFI_STRING_ID                           mPciCapabilityStringIds[DEFAULT_PCI_CAPABILITY_STRING_ID_BUFFER_SIZE]                    = {0};
STATIC  EFI_STRING_ID                           mPciCapabilityHelpStringIds[DEFAULT_PCI_CAPABILITY_STRING_ID_BUFFER_SIZE]                = {0};
STATIC  EFI_STRING_ID                           mPcieExtCapabilityStringIds[DEFAULT_PCIE_EXTEND_CAPABILITY_STRING_ID_BUFFER_SIZE]        = {0};
STATIC  EFI_STRING_ID                           mPcieExtCapabilityHelpStringIds[DEFAULT_PCIE_EXTEND_CAPABILITY_STRING_ID_BUFFER_SIZE]    = {0};

STATIC  UINTN                       mIgnoreTableCount           = 0;
STATIC  IGNORE_DEVICE_TABLE_ENTRY   *mIgnoreTable               = NULL;

STATIC  PCI_CAPABILITY_PCIEXP       mPciExpCap;
STATIC  UINTN                       mCapIndex;
STATIC  UINTN                       mInterpretationIndex;


/**
  This function frees a pool of memory if the address is non-NULL and then resets
  the address to NULL so that future calls will do nothing.

  @param[in,out]  Buffer  On entry, optional pointer to the
                          address of the buffer to free or NULL.
                          On exit, optional pointer to NULL.

**/
VOID
InternalFreePool (
  IN OUT VOID **Buffer OPTIONAL
  )
{
  if (Buffer != NULL && *Buffer != NULL) {
    FreePool (*Buffer);
    *Buffer = NULL;
  }
}

/**
  Check if the device should be ignored.

  @param  VendorId  The Vendor ID of the device.
  @param  DeviceId  The Device ID of the device.

  @retval TRUE    The device should be ignored.
  @retval FALSE   The device should not be ignored.

**/
BOOLEAN
IsIgnored (
  IN  UINT16  VendorId,
  IN  UINT16  DeviceId
  )
{
  UINTN Index;

  if (VendorId == 0xffff && DeviceId == 0xffff) {
    return TRUE;
  }

  if (mIgnoreTable != NULL) {
    for (Index = 0; Index < mIgnoreTableCount; Index++) {
      if (VendorId == mIgnoreTable[Index].VendorId) {
        if ((mIgnoreTable[Index].DeviceId == 0xffff) ||
            DeviceId == mIgnoreTable[Index].DeviceId) {
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

VOID
GetPciCapabilityDisplayControlTable (
  VOID
  )
{
  UINT8   *PcdData;
  UINTN   PcdDataSize;

  PcdData     = PcdGetPtr (PcdH2OPciCapabilityDisplayControl);
  PcdDataSize = PcdGetSize (PcdH2OPciCapabilityDisplayControl);

  if ((PcdDataSize < sizeof (CAPABILITY_DISPLAY_CONTROL_TABLE_ENTRY)) ||
      (PcdDataSize % sizeof (CAPABILITY_DISPLAY_CONTROL_TABLE_ENTRY) != 0)) {
    DEBUG ((EFI_D_ERROR, "PcdH2OPciCapabilityDisplayControl does not match a valid data format.\n"));
    return;
  }

  mCapDisplayCtrlTable = (CAPABILITY_DISPLAY_CONTROL_TABLE_ENTRY *) PcdData;
  mCapDisplayCtrlTableEntryCount = PcdDataSize / sizeof (CAPABILITY_DISPLAY_CONTROL_TABLE_ENTRY);
}

CAPABILITY_DISPLAY_CONTROL
GetPciCapbilityDisplayControlMode (
  IN  BOOLEAN   IsPcie,
  IN  UINT16    CapabilityId
  )
{
  UINTN   Index;

  for (Index = 0; Index < mCapDisplayCtrlTableEntryCount; Index++) {
    if (!((BOOLEAN) mCapDisplayCtrlTable[Index].IsPcieExtCapability) &&
         ((mCapDisplayCtrlTable[Index].CapabilityId & 0xFF00) != 0)) {
      continue;
    }

    if ((BOOLEAN) mCapDisplayCtrlTable[Index].IsPcieExtCapability == IsPcie &&
        mCapDisplayCtrlTable[Index].CapabilityId == CapabilityId) {
      return mCapDisplayCtrlTable[Index].DisplayControl;
    }
  }

  return CapabilityDisplayControlDefault;
}

/**
  Helper function to create opcode(s) for given PciDeviceInfo.

  @param[in]  PciDeviceInfo       The pointer to the PCI Device Info.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.
  @param[in]  Level               The integer that indicates the recursion level.

**/
VOID
CreateOpCodeForPciDevice (
  IN  PCI_DEVICE_INFO   *PciDeviceInfo,
  IN  VOID              *StartOpCodeHandle,
  IN  UINTN             Level
  )
{
  UINT32                          SerialNumber;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL   *PciLookupTable;
  CHAR16                          *VendorNameStr;
  CHAR16                          *DeviceNameStr;
  UINT32                          ClassCode;
  CHAR16                          *BaseClassStr;
  CHAR16                          *SubClassStr;
  CHAR16                          *InterfaceStr;
  CHAR16                          IndentStr[0x200];
  CHAR16                          StrBuffer[0x100];
  UINTN                           DestMax;
  UINTN                           Index;

  VendorNameStr = NULL;
  DeviceNameStr = NULL;
  BaseClassStr  = NULL;
  SubClassStr   = NULL;
  InterfaceStr  = NULL;

  if (PciDeviceInfo == NULL) {
    return;
  }

  if (IsIgnored (
        PciDeviceInfo->PciConfigSpace.Hdr.VendorId,
        PciDeviceInfo->PciConfigSpace.Hdr.DeviceId
        )) {
    return;
  }

  SerialNumber = PciDeviceInfo->SerialNumber;

  if (PciDeviceInfo->IsRootBridge)
  {
    if (mPrivateData->Configuration.ShowDeviceTree) {
      HiiCreateGrayOutIfOpCodeEx (StartOpCodeHandle, 1);
      HiiCreateTrueOpCode (StartOpCodeHandle);
      HiiCreateTextOpCode (StartOpCodeHandle, mPciDeviceStringIds[SerialNumber], 0, 0);
      HiiCreateEndOpCode (StartOpCodeHandle);
    }
    return;
  }

  PciLookupTable = mPrivateData->PciLookupTable;
  PciLookupTable->PCIeId2Str (
                    PciDeviceInfo->PciConfigSpace.Hdr.VendorId,
                    PciDeviceInfo->PciConfigSpace.Hdr.DeviceId,
                    &VendorNameStr,
                    &DeviceNameStr,
                    NULL
                    );
  if (DeviceNameStr == NULL || !mPrivateData->Configuration.ShowActualDeviceName) {
    if (DeviceNameStr != NULL) {
      InternalFreePool ((VOID **) &DeviceNameStr);
    }

    ClassCode = PCI_CLASS_CODE_TO_CLASS_CODE_32 (PciDeviceInfo->PciConfigSpace.Hdr.ClassCode);
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
      PciDeviceInfo->PciConfigSpace.Hdr.VendorId,
      DeviceNameStr
      );
  } else if (VendorNameStr == NULL && DeviceNameStr == NULL) {
    UnicodeSPrint (
      StrBuffer,
      sizeof (StrBuffer),
      L"Vendor:0x%04x, Device:0x%04x",
      PciDeviceInfo->PciConfigSpace.Hdr.VendorId,
      PciDeviceInfo->PciConfigSpace.Hdr.DeviceId
      );
  }

  InternalFreePool ((VOID **) &VendorNameStr);
  InternalFreePool ((VOID **) &DeviceNameStr);
  InternalFreePool ((VOID **) &BaseClassStr);
  InternalFreePool ((VOID **) &SubClassStr);
  InternalFreePool ((VOID **) &InterfaceStr);

  ZeroMem (IndentStr, sizeof (IndentStr));
  DestMax = sizeof (IndentStr) / sizeof (CHAR16);
  if (mPrivateData->Configuration.ShowDeviceTree) {
    StrnCatS (IndentStr, DestMax, L"|", 1);
    for (Index = 0; Index < Level; Index++) {
      StrnCatS (IndentStr, DestMax, L"-", 1);
    }
  }

  StrnCatS (IndentStr, DestMax, StrBuffer, StrnLenS (StrBuffer, StrnSizeS (StrBuffer, sizeof (StrBuffer))));

  mPciDeviceStringIds[SerialNumber] = HiiSetString (
                                        mHiiHandle,
                                        mPciDeviceStringIds[SerialNumber],
                                        IndentStr,
                                        NULL
                                        );

  if (PciDeviceInfo->PcieCapOffset != 0) {
    HiiCreateSuppressIfOpCodeEx (StartOpCodeHandle, 1);
    HiiCreateIdEqualOpCodeEx (StartOpCodeHandle, H2O_PCI_DEVICE_INFO_FILTERING_MODE_QUESTION_ID, PciDeviceOnly);
  } else {
    HiiCreateSuppressIfOpCodeEx (StartOpCodeHandle, 1);
    HiiCreateIdEqualOpCodeEx (StartOpCodeHandle, H2O_PCI_DEVICE_INFO_FILTERING_MODE_QUESTION_ID, PcieDeviceOnly);
  }

  HiiCreateGotoOpCode (
    StartOpCodeHandle,
    H2O_PCI_DEVICE_INFO_DEVICE_INFO_FORM_ID,
    mPciDeviceStringIds[SerialNumber],
    mPciDeviceHelpStringIds[SerialNumber],
    EFI_IFR_FLAG_CALLBACK,
    (EFI_QUESTION_ID) (H2O_PCI_DEVICE_INFO_DYNAMIC_DEVICE_BASE_QUESTION_ID + SerialNumber)
    );

  HiiCreateEndOpCode (StartOpCodeHandle);
}

/**
  Helper function to traverse all the PCI Device Info below
  the PCI Root Bridge or the PCI Bridge and then create Opcodes for show.

  @param[in]  PciDeviceInfo       The pointer to the PCI Device Info for traverse.
  @param[in]  StartOpCodeHandle   The context used to insert opcode.
  @param[in]  Level               The integer that indicates the recursion level.

**/
VOID
TraverseAndCreateOpCodeForPciDevice (
  IN  PCI_DEVICE_INFO *PciDeviceInfo,
  IN  VOID            *StartOpCodeHandle,
  IN  UINTN           Level
  )
{
  LIST_ENTRY      *ChildLink;
  PCI_DEVICE_INFO *ChildPciDeviceInfo;

  if (PciDeviceInfo == NULL) {
    return;
  }

  CreateOpCodeForPciDevice (PciDeviceInfo, StartOpCodeHandle, Level);

  if (!IsListEmpty (&PciDeviceInfo->ChildList)) {
    BASE_LIST_FOR_EACH (ChildLink, &PciDeviceInfo->ChildList) {
      ChildPciDeviceInfo = PCI_DEVICE_INFO_FROM_CHILD_LINK (ChildLink);
      TraverseAndCreateOpCodeForPciDevice (ChildPciDeviceInfo, StartOpCodeHandle, Level + 1);
    }
  }
}


/**
  This function updates device help strings into database.

**/
VOID
UpdateDeviceHelpStrings (
  VOID
  )
{
  LIST_ENTRY                      *Entry;
  PCI_DEVICE_INFO                 *PciDeviceInfo;
  UINT32                          SerialNumber;
  CHAR16                          *DevicePathStr;
  CHAR16                          PciLocationStr[13];
  CHAR16                          *HelpStringFormat;
  CHAR16                          StrBuffer[0x100];

  HelpStringFormat  = NULL;
  BASE_LIST_FOR_EACH (Entry, mPciDeviceInfoPool) {
    PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
    SerialNumber  = PciDeviceInfo->SerialNumber;

    if (PciDeviceInfo->IsRootBridge) {
      DevicePathStr = ConvertDevicePathToText (PciDeviceInfo->DevicePath, TRUE, FALSE);

      mPciDeviceStringIds[SerialNumber] = HiiSetString (
                                            mHiiHandle,
                                            mPciDeviceStringIds[SerialNumber],
                                            DevicePathStr,
                                            NULL
                                            );

      FreePool (DevicePathStr);
    } else {
      DevicePathStr = ConvertDevicePathToText (PciDeviceInfo->DevicePath, TRUE, FALSE);

      UnicodeSPrint (
        PciLocationStr,
        sizeof (PciLocationStr),
        L"%04x:%02x:%02x.%01x",
        SEGMENT_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress),
        BUS_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress),
        DEVICE_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress),
        FUNCTION_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress)
        );

      HelpStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_FORMAT_PCI_DEVICE_HELP_STRING), NULL);

      ZeroMem (StrBuffer, sizeof (StrBuffer));
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        HelpStringFormat,
        (PciDeviceInfo->PcieCapOffset == 0)? L"PCI" : L"PCIe",
        DevicePathStr,
        PciLocationStr,
        PciDeviceInfo->SlotNameStr
        );

      FreePool (HelpStringFormat);

      mPciDeviceHelpStringIds[SerialNumber] = HiiSetString (
                                                mHiiHandle,
                                                mPciDeviceHelpStringIds[SerialNumber],
                                                StrBuffer,
                                                NULL
                                                );
    }
  }

  return;
}


/**
  This function updates root form.

**/
VOID
UpdateForm (
  VOID
  )
{
  VOID                *StartOpCodeHandle;
  VOID                *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL  *StartLabel;
  EFI_IFR_GUID_LABEL  *EndLabel;
  LIST_ENTRY          *Entry;
  PCI_DEVICE_INFO     *PciDeviceInfo;
  EFI_STATUS          Status;

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
  StartLabel->Number        = H2O_PCI_DEVICE_INFO_DEVICE_LIST_LABEL_START;

  EndLabel                  = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode    = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number          = H2O_PCI_DEVICE_INFO_DEVICE_LIST_LABEL_END;

  if (mPrivateData->Configuration.ShowDeviceTree) {
    BASE_LIST_FOR_EACH (Entry, mPciDeviceInfoPool) {
      PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
      if (PciDeviceInfo->IsRootBridge) {
        TraverseAndCreateOpCodeForPciDevice (PciDeviceInfo, StartOpCodeHandle, 0);
      }
    }
  } else {
    BASE_LIST_FOR_EACH (Entry, mPciDeviceInfoPool) {
      PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
      CreateOpCodeForPciDevice (PciDeviceInfo, StartOpCodeHandle, 0);
    }
  }

  Status = HiiUpdateForm (
             mHiiHandle,
             &gH2OPciDeviceInfoSetupFormsetGuid,
             H2O_PCI_DEVICE_INFO_ROOT_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HiiUpdateForm, Status: %r\n", __FUNCTION__, Status));
  }

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
}

VOID
UpdatePciCapbilityList (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  UINT8                           PciConfigSpace[0x100];
  UINT8                           CapabilityPtr;
  EFI_PCI_CAPABILITY_HDR          *CapabilityHdr;
  VOID                            *StartOpCodeHandle;
  VOID                            *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL              *StartLabel;
  EFI_IFR_GUID_LABEL              *EndLabel;
  UINTN                           Index;
  CHAR16                          *PciCapabilityStringFormat;
  CHAR16                          *PciCapabilityHelpStringFormat;
  CHAR16                          *CapabilityStr;
  CHAR16                          StrBuffer[0x100];
  H2O_PCI_LOOKUP_TABLE_PROTOCOL   *PciLookupTable;
  EFI_STATUS                      CreatePciCapabilityStatus;

  if (mCurrentPciDeviceInfo == NULL) {
    return;
  }

  Index = 0;
  PciIo = mCurrentPciDeviceInfo->PciIo;
  PciLookupTable = mPrivateData->PciLookupTable;
  PciCapabilityStringFormat     = NULL;
  PciCapabilityHelpStringFormat = NULL;
  CreatePciCapabilityStatus = EFI_SUCCESS;
  ZeroMem (PciConfigSpace, sizeof (PciConfigSpace));

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
  StartLabel->Number        = H2O_PCI_DEVICE_INFO_PCI_CAPABILITY_LIST_LABEL_START;

  EndLabel                  = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode    = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number          = H2O_PCI_DEVICE_INFO_PCI_CAPABILITY_LIST_LABEL_END;

  Status = PciIo->Pci.Read (PciIo, EfiPciWidthUint8, 0, sizeof (PciConfigSpace), PciConfigSpace);
  if (EFI_ERROR (Status)) {
    goto UpdateDynamicRegion;
  }
  CapabilityPtr = ((PCI_TYPE00 *) PciConfigSpace)->Device.CapabilityPtr;

  while (CapabilityPtr != 0) {

    if (Index >= DEFAULT_PCI_CAPABILITY_STRING_ID_BUFFER_SIZE) {
      DEBUG((EFI_D_ERROR, "The access pcie capability string id buffer exceeds the maximum limit.\n"));
      break;
    }

    CapabilityHdr = (EFI_PCI_CAPABILITY_HDR *)(PciConfigSpace + CapabilityPtr);

    if (CapabilityHdr->CapabilityID == EFI_PCI_CAPABILITY_ID_PCIEXP) {
      CopyMem (&mPciExpCap, CapabilityHdr, sizeof (PCI_CAPABILITY_PCIEXP));
    }

    if (GetPciCapbilityDisplayControlMode (FALSE, CapabilityHdr->CapabilityID) == CapabilityDisplayControlHide) {
      CapabilityPtr = CapabilityHdr->NextItemPtr;
      continue;
    }

    CapabilityStr = NULL;
    ZeroMem (StrBuffer, sizeof(StrBuffer));
    PciLookupTable->GetCapabilityStr (CapabilityHdr->CapabilityID, &CapabilityStr, NULL);
    if (CapabilityStr != NULL) {
      PciCapabilityStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_CAPABILITY_STRING_FORMAT), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityStringFormat,
        CapabilityStr,
        CapabilityHdr->CapabilityID
        );
    } else {
      PciCapabilityStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_CAPABILITY_NO_STRING_FORMAT), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityStringFormat,
        CapabilityHdr->CapabilityID
        );
    }
    mPciCapabilityStringIds[Index] = HiiSetString (mHiiHandle, mPciCapabilityStringIds[Index], StrBuffer, NULL);

    ZeroMem (StrBuffer, sizeof(StrBuffer));
    if (CapabilityStr != NULL) {
      PciCapabilityHelpStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_CAPABILITY_STRING_HELP), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityHelpStringFormat,
        CapabilityStr,
        CapabilityHdr->CapabilityID,
        CapabilityPtr
        );
    } else {
      PciCapabilityHelpStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_CAPABILITY_NO_STRING_HELP), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityHelpStringFormat,
        CapabilityHdr->CapabilityID,
        CapabilityPtr
        );
    }
    mPciCapabilityHelpStringIds[Index] = HiiSetString (mHiiHandle, mPciCapabilityHelpStringIds[Index], StrBuffer, NULL);
    InternalFreePool ((VOID **) &CapabilityStr);
    InternalFreePool ((VOID **) &PciCapabilityStringFormat);
    InternalFreePool ((VOID **) &PciCapabilityHelpStringFormat);

    HiiCreateActionOpCode (
      StartOpCodeHandle,
      (EFI_QUESTION_ID)(H2O_PCI_DEVICE_INFO_DYNAMIC_PCI_CAPABILITY_BASE_QUESTION_ID + mCapIndex),
      mPciCapabilityStringIds[Index],
      mPciCapabilityHelpStringIds[Index],
      EFI_IFR_FLAG_CALLBACK,
      0
      );

    if (mPrivateData->Configuration.DisplayCapabilityIntpn &&
        (GetPciCapbilityDisplayControlMode (FALSE, CapabilityHdr->CapabilityID) != CapabilityDisplayControlHideIntpnOnly) &&
        !EFI_ERROR (CreatePciCapabilityStatus)) {
      CreatePciCapabilityStatus = LibCreatePciCapabilityInterpretation (
                                    mHiiHandle,
                                    mCurrentPciDeviceInfo,
                                    StartOpCodeHandle,
                                    CapabilityHdr,
                                    &mInterpretationIndex,
                                    H2O_PCI_DEVICE_INFO_DYNAMIC_CAPABILITY_INFO_BASE_QUESTION_ID,
                                    NULL
                                    );
    }

    mCapIndex++;
    Index++;
    CapabilityPtr = CapabilityHdr->NextItemPtr;
  }

UpdateDynamicRegion:
  if (mCapIndex == 0) {
    HiiCreateActionOpCode (
      StartOpCodeHandle,
      (EFI_QUESTION_ID)(H2O_PCI_DEVICE_INFO_DYNAMIC_PCI_CAPABILITY_BASE_QUESTION_ID + mCapIndex),
      STRING_TOKEN(STR_PCI_NO_CAPABILITY_STRING_PROMPT),
      STRING_TOKEN(STR_PCI_NO_CAPABILITY_STRING_HELP),
      EFI_IFR_FLAG_CALLBACK,
      0
      );
  }

  Status =  HiiUpdateForm (
              mHiiHandle,
              &gH2OPciDeviceInfoSetupFormsetGuid,
              H2O_PCI_DEVICE_INFO_DEVICE_INFO_FORM_ID,
              StartOpCodeHandle,
              EndOpCodeHandle
              );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HiiUpdateForm, Status: %r\n", __FUNCTION__, Status));
  }

  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  if (PciCapabilityStringFormat != NULL) {
    FreePool (PciCapabilityStringFormat);
  }
  if (PciCapabilityHelpStringFormat) {
    FreePool (PciCapabilityHelpStringFormat);
  }
}

VOID
UpdatePcieCapbilityList (
  VOID
  )
{
  EFI_PCI_IO_PROTOCOL             *PciIo;
  UINT8                           PciConfigSpace[0x1000];
  UINT32                          CapabilityPtr;
  PCI_EXP_EXT_HDR                 *CapabilityHdr;
  VOID                            *StartOpCodeHandle;
  VOID                            *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL              *StartLabel;
  EFI_IFR_GUID_LABEL              *EndLabel;
  UINTN                           Index;
  CHAR16                          *PciCapabilityStringFormat;
  CHAR16                          *PciCapabilityStringHelp;
  CHAR16                          *CapabilityStr;
  CHAR16                          StrBuffer[0x100];
  H2O_PCI_LOOKUP_TABLE_PROTOCOL   *PciLookupTable;
  EFI_STATUS                      Status;
  EFI_STATUS                      CreatePcieExtCapabilityStatus;

  if (mCurrentPciDeviceInfo == NULL) {
    return;
  }

  Index = 0;
  PciIo = mCurrentPciDeviceInfo->PciIo;
  PciLookupTable = mPrivateData->PciLookupTable;
  PciCapabilityStringFormat = NULL;
  PciCapabilityStringHelp   = NULL;
  CreatePcieExtCapabilityStatus = EFI_SUCCESS;
  ZeroMem (PciConfigSpace, sizeof (PciConfigSpace));

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
  StartLabel->Number        = H2O_PCI_DEVICE_INFO_PCIE_CAPABILITY_LIST_LABEL_START;

  EndLabel                  = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode    = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number          = H2O_PCI_DEVICE_INFO_PCIE_CAPABILITY_LIST_LABEL_END;

  if (mCurrentPciDeviceInfo->PcieCapOffset == 0) {
    DEBUG((EFI_D_ERROR, "Not a PCIe Device\n"));
    goto UpdateDynamicRegion;
  }

  PciSegmentReadBuffer (mCurrentPciDeviceInfo->BaseAddress, sizeof (PciConfigSpace), PciConfigSpace);
  if (IsZeroBuffer (&PciConfigSpace, sizeof (PciConfigSpace))) {
    DEBUG((EFI_D_ERROR, "PCIe Read Failed\n"));
    goto UpdateDynamicRegion;
  }

  CapabilityPtr = EFI_PCIE_CAPABILITY_BASE_OFFSET;
  CapabilityHdr = (PCI_EXP_EXT_HDR *)(PciConfigSpace + CapabilityPtr);

  if ((CapabilityHdr->CapabilityId == 0x0000) &&
      (CapabilityHdr->CapabilityVersion == 0x0) &&
      (CapabilityHdr->NextCapabilityOffset == 0x000)) {
    //
    // Refer PCIE 6.0 Spec
    //
    // 7.6.1 Extended Capabilities in Configuration Space
    //
    // Extended Capabilities in Configuration Space always begin at offset 100h with a PCI Express Extended Capability header
    // (Section 7.6.3). Absence of any Extended Capabilities is required to be indicated by an Extended Capability header with
    // a Capability ID of 0000h, a Capability Version of 0h, and a Next Capability Offset of 000h.
    //
    DEBUG((EFI_D_INFO, "Absence of any PCIE Extended Capability.\n"));
    goto UpdateDynamicRegion;
  }

  if ((CapabilityHdr->CapabilityId == 0xFFFF) &&
      (CapabilityHdr->CapabilityVersion == 0xF) &&
      (CapabilityHdr->NextCapabilityOffset == 0xFFF)) {
    DEBUG((EFI_D_ERROR, "PCIE Extended Capability invalid.\n"));
    goto UpdateDynamicRegion;
  }

  HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_EMPTY), 0, 0, 0);
  HiiCreateSubTitleOpCode (StartOpCodeHandle, STRING_TOKEN (STR_PCIE_EXTENDED_CAPABILITIES), 0, 0, 0);

  while (CapabilityPtr != 0) {

    if (Index >= DEFAULT_PCIE_EXTEND_CAPABILITY_STRING_ID_BUFFER_SIZE) {
      DEBUG((EFI_D_ERROR, "The access pcie extend capability string id buffer exceeds the maximum limit.\n"));
      break;
    }

    CapabilityHdr = (PCI_EXP_EXT_HDR *)(PciConfigSpace + CapabilityPtr);

    if (GetPciCapbilityDisplayControlMode (TRUE, (UINT16) CapabilityHdr->CapabilityId) == CapabilityDisplayControlHide) {
      CapabilityPtr = CapabilityHdr->NextCapabilityOffset;
      continue;
    }

    CapabilityStr = NULL;
    ZeroMem (StrBuffer, sizeof(StrBuffer));
    PciLookupTable->GetExtCapabilityStr ((UINT16) CapabilityHdr->CapabilityId, &CapabilityStr, NULL);
    if (CapabilityStr != NULL) {
      PciCapabilityStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_EXT_CAPABILITY_STRING_FORMAT), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityStringFormat,
        CapabilityStr,
        CapabilityHdr->CapabilityId
        );
    } else {
      PciCapabilityStringFormat = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_EXT_CAPABILITY_NO_STRING_FORMAT), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityStringFormat,
        CapabilityHdr->CapabilityId
        );
    }
    mPcieExtCapabilityStringIds[Index] = HiiSetString (mHiiHandle, mPcieExtCapabilityStringIds[Index], StrBuffer, NULL);

    ZeroMem (StrBuffer, sizeof(StrBuffer));
    if (CapabilityStr != NULL) {
      PciCapabilityStringHelp = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_EXT_CAPABILITY_STRING_HELP), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityStringHelp,
        CapabilityStr,
        CapabilityHdr->CapabilityId,
        CapabilityPtr
        );
    } else {
      PciCapabilityStringHelp = HiiGetString (mHiiHandle, STRING_TOKEN (STR_PCI_EXT_CAPABILITY_NO_STRING_HELP), NULL);
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        PciCapabilityStringHelp,
        CapabilityHdr->CapabilityId,
        CapabilityPtr
        );
    }
    mPcieExtCapabilityHelpStringIds[Index] = HiiSetString (mHiiHandle, mPcieExtCapabilityHelpStringIds[Index], StrBuffer, NULL);
    InternalFreePool ((VOID **) &CapabilityStr);
    InternalFreePool ((VOID **) &PciCapabilityStringFormat);
    InternalFreePool ((VOID **) &PciCapabilityStringHelp);

    HiiCreateActionOpCode (
      StartOpCodeHandle,
      (EFI_QUESTION_ID)(H2O_PCI_DEVICE_INFO_DYNAMIC_PCI_CAPABILITY_BASE_QUESTION_ID + mCapIndex),
      mPcieExtCapabilityStringIds[Index],
      mPcieExtCapabilityHelpStringIds[Index],
      EFI_IFR_FLAG_CALLBACK,
      0
      );

    if (mPrivateData->Configuration.DisplayCapabilityIntpn &&
        (GetPciCapbilityDisplayControlMode (TRUE, (UINT16) CapabilityHdr->CapabilityId) != CapabilityDisplayControlHideIntpnOnly) &&
        !EFI_ERROR (CreatePcieExtCapabilityStatus)) {
      CreatePcieExtCapabilityStatus = LibCreatePcieExtCapabilityInterpretation (
                                        mHiiHandle,
                                        mCurrentPciDeviceInfo,
                                        StartOpCodeHandle,
                                        CapabilityHdr,
                                        &mInterpretationIndex,
                                        H2O_PCI_DEVICE_INFO_DYNAMIC_CAPABILITY_INFO_BASE_QUESTION_ID,
                                        &mPciExpCap,
                                        NULL
                                        );
    }

    mCapIndex++;
    Index++;
    CapabilityPtr = CapabilityHdr->NextCapabilityOffset;
  }

UpdateDynamicRegion:
  Status = HiiUpdateForm (
             mHiiHandle,
             &gH2OPciDeviceInfoSetupFormsetGuid,
             H2O_PCI_DEVICE_INFO_DEVICE_INFO_FORM_ID,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to HiiUpdateForm, Status: %r\n", __FUNCTION__, Status));
  }
  HiiFreeOpCodeHandle (StartOpCodeHandle);
  HiiFreeOpCodeHandle (EndOpCodeHandle);
  if (PciCapabilityStringFormat != NULL) {
    FreePool (PciCapabilityStringFormat);
  }
  if (PciCapabilityStringHelp) {
    FreePool (PciCapabilityStringHelp);
  }
}

/**
  This function updates drive info form.

  @param[in]  QuestionId  Question ID.

**/
VOID
UpdateSubForm (
  IN  EFI_QUESTION_ID   QuestionId
  )
{
  UINT32                        SerialNumber;
  CHAR16                        *SubFormTitleStr;
  LIST_ENTRY                    *Entry;
  PCI_DEVICE_INFO               *PciDeviceInfo;
  CHAR16                        *DevicePathStr;
  CHAR16                        StrBuffer[0x100];
  EFI_STATUS                    Status;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL *PciLookupTable;
  CHAR16                        *HeaderTypeStr;
  CHAR16                        *MultiFuncStr;
  UINT32                        ClassCode;
  CHAR16                        *BaseClassStr;
  CHAR16                        *SubClassStr;
  CHAR16                        *InterfaceStr;
  UINTN                         DestMax;
  CHAR16                        *LinkSpeedStr;
  CHAR16                        *PcieRevisionStr;

  PciDeviceInfo   = NULL;
  SubFormTitleStr = NULL;
  HeaderTypeStr   = NULL;
  MultiFuncStr    = NULL;
  BaseClassStr    = NULL;
  SubClassStr     = NULL;
  InterfaceStr    = NULL;
  LinkSpeedStr    = NULL;
  PcieRevisionStr = NULL;

  SerialNumber    = (UINT32) (QuestionId & B_SETUP_POLICY_SERIAL);
  PciLookupTable  = mPrivateData->PciLookupTable;

  BASE_LIST_FOR_EACH (Entry, mPciDeviceInfoPool) {
    PciDeviceInfo = PCI_DEVICE_INFO_FROM_LINK (Entry);
    if (PciDeviceInfo->SerialNumber == SerialNumber) {
      break;
    }
  }

  if (Entry == mPciDeviceInfoPool || PciDeviceInfo == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Device not found. SerialNumber = %d\n", __FUNCTION__, SerialNumber));
    return;
  }

  mCurrentPciDeviceInfo = PciDeviceInfo;

  SubFormTitleStr = HiiGetString (mHiiHandle, mPciDeviceStringIds[SerialNumber], NULL);
  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_SUBFORM_TITLE), SubFormTitleStr, NULL);
  FreePool (SubFormTitleStr);

  DevicePathStr = ConvertDevicePathToText (PciDeviceInfo->DevicePath, TRUE, FALSE);
  HiiSetString (mHiiHandle,(EFI_STRING_ID) STRING_TOKEN (STR_DEVICE_PATH_VALUE), DevicePathStr, NULL);
  FreePool (DevicePathStr);

  ZeroMem (StrBuffer, sizeof (StrBuffer));
  UnicodeSPrint (
    StrBuffer,
    sizeof (StrBuffer),
    L"%04x:%02x:%02x.%01x",
    SEGMENT_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress),
    BUS_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress),
    DEVICE_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress),
    FUNCTION_NUMBER_FROM_PCI_SEGMENT_LIB_ADDRESS (PciDeviceInfo->BaseAddress)
    );
  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_PCI_LOCATION_VALUE), StrBuffer, NULL);

  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_SLOT_NAME_VALUE), PciDeviceInfo->SlotNameStr, NULL);

  ZeroMem (StrBuffer, sizeof (StrBuffer));
  UnicodeSPrint (
    StrBuffer,
    sizeof (StrBuffer),
    L"0x%04x",
    PciDeviceInfo->PciConfigSpace.Hdr.VendorId
    );
  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_VENDOR_ID_VALUE), StrBuffer, NULL);

  ZeroMem (StrBuffer, sizeof (StrBuffer));
  UnicodeSPrint (
    StrBuffer,
    sizeof (StrBuffer),
    L"0x%04x",
    PciDeviceInfo->PciConfigSpace.Hdr.DeviceId
    );
  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_DEVICE_ID_VALUE), StrBuffer, NULL);

  Status = PciLookupTable->GetHeaderTypeStr (
                             PciDeviceInfo->PciConfigSpace.Hdr.HeaderType,
                             &HeaderTypeStr,
                             &MultiFuncStr,
                             NULL
                             );
  ZeroMem (StrBuffer, sizeof (StrBuffer));
  if (!EFI_ERROR (Status)) {
    UnicodeSPrint (
      StrBuffer,
      sizeof (StrBuffer),
      L"%s, %s",
      HeaderTypeStr,
      MultiFuncStr
      );
    InternalFreePool ((VOID **) &HeaderTypeStr);
    InternalFreePool ((VOID **) &MultiFuncStr);
  } else {
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"N/A");
  }
  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_HEADER_TYPE_VALUE), StrBuffer, NULL);

  ClassCode = PCI_CLASS_CODE_TO_CLASS_CODE_32 (PciDeviceInfo->PciConfigSpace.Hdr.ClassCode);
  PciLookupTable->PciGetClassStrings (ClassCode, &BaseClassStr, &SubClassStr, &InterfaceStr, NULL);

  ZeroMem (StrBuffer, sizeof (StrBuffer));
  DestMax = sizeof (StrBuffer) / sizeof (CHAR16);
  if (BaseClassStr != NULL) {
    StrnCatS (StrBuffer, DestMax, BaseClassStr, StrLen (BaseClassStr));
    InternalFreePool ((VOID **) &BaseClassStr);
    if (SubClassStr != NULL) {
      StrnCatS (StrBuffer, sizeof (StrBuffer), L" - ", 4);
      StrnCatS (StrBuffer, DestMax, SubClassStr, StrLen (SubClassStr));
      InternalFreePool ((VOID **) &SubClassStr);
    }
    if (InterfaceStr != NULL) {
      StrnCatS (StrBuffer, sizeof (StrBuffer), L" - ", 4);
      StrnCatS (StrBuffer, DestMax, InterfaceStr, StrLen (InterfaceStr));
      InternalFreePool ((VOID **) &InterfaceStr);
    }
  } else {
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"N/A");
  }
  HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_CLASS_CODE_VALUE), StrBuffer, NULL);

  if (PciDeviceInfo->PcieCapOffset != 0) {
    UINT32  MaxLinkWidth;
    UINT32  CurrentLinkWidth;
    UINT32  MaxLinkSpeed;
    UINT32  CurrentLinkSpeed;

    MaxLinkWidth      = PciDeviceInfo->PcieCap.LinkCapability.Bits.MaxLinkWidth;
    CurrentLinkWidth  = PciDeviceInfo->PcieCap.LinkStatus.Bits.NegotiatedLinkWidth;
    MaxLinkSpeed      = PciDeviceInfo->PcieCap.LinkCapability.Bits.MaxLinkSpeed;
    CurrentLinkSpeed  = PciDeviceInfo->PcieCap.LinkStatus.Bits.CurrentLinkSpeed;

    ZeroMem (StrBuffer, sizeof (StrBuffer));
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"x%d", MaxLinkWidth);
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_MAX_LINK_WIDTH_VALUE), StrBuffer, NULL);

    ZeroMem (StrBuffer, sizeof (StrBuffer));
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"x%d", CurrentLinkWidth);
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_CURRENT_LINK_WIDTH_VALUE), StrBuffer, NULL);

    Status = PciLookupTable->GetVendorLinkSpeedStr (MaxLinkSpeed, &LinkSpeedStr, &PcieRevisionStr, NULL);
    ZeroMem (StrBuffer, sizeof (StrBuffer));
    if (!EFI_ERROR (Status)) {
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        L"%s (%s)",
        LinkSpeedStr,
        PcieRevisionStr
        );
    } else {
      UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"N/A");
    }
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_MAX_LINK_SPEED_VALUE), StrBuffer, NULL);

    Status = PciLookupTable->GetVendorLinkSpeedStr (CurrentLinkSpeed, &LinkSpeedStr, &PcieRevisionStr, NULL);
    ZeroMem (StrBuffer, sizeof (StrBuffer));
    if (!EFI_ERROR (Status)) {
      UnicodeSPrint (
        StrBuffer,
        sizeof (StrBuffer),
        L"%s (%s)",
        LinkSpeedStr,
        PcieRevisionStr
        );
    } else {
      UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"N/A");
    }
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_CURRENT_LINK_SPEED_VALUE), StrBuffer, NULL);
  } else {
    ZeroMem (StrBuffer, sizeof (StrBuffer));
    UnicodeSPrint (StrBuffer, sizeof (StrBuffer), L"Not Support");
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_MAX_LINK_WIDTH_VALUE), StrBuffer, NULL);
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_CURRENT_LINK_WIDTH_VALUE), StrBuffer, NULL);
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_MAX_LINK_SPEED_VALUE), StrBuffer, NULL);
    HiiSetString (mHiiHandle, (EFI_STRING_ID) STRING_TOKEN (STR_CURRENT_LINK_SPEED_VALUE), StrBuffer, NULL);
  }

  mInterpretationIndex = 0;
  mCapIndex            = 0;
  UpdatePciCapbilityList ();
  UpdatePcieCapbilityList ();
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
  PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA  *PrivateData;

  if (((Value == NULL) && (Action != EFI_BROWSER_ACTION_FORM_OPEN) && (Action != EFI_BROWSER_ACTION_FORM_CLOSE))||
    (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA_FROM_THIS (This);

  switch (Action) {

   case EFI_BROWSER_ACTION_FORM_OPEN:
    if (QuestionId == H2O_PCI_DEVICE_INFO_FORM_REFRESH_QUESTION_ID) {
      UpdateDeviceHelpStrings();
      UpdateForm ();
    }
    return EFI_UNSUPPORTED;

  case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
    if (QuestionId == H2O_PCI_DEVICE_INFO_FORM_REFRESH_QUESTION_ID) {
      HiiGetBrowserData (
        &gH2OPciDeviceInfoConfigurationGuid,
        mVariableName,
        sizeof (PCI_DEVICE_INFO_CONFIGURATION),
        (UINT8 *) &PrivateData->Configuration
        );
      UpdateDeviceHelpStrings();
      UpdateForm ();
    }
    if (QuestionId == H2O_PCI_DEVICE_INFO_DISPLAY_CAPABILITY_INTPN_QUESTION_ID) {
      mInterpretationIndex = 0;
      mCapIndex            = 0;
      UpdatePciCapbilityList ();
      UpdatePcieCapbilityList ();
    }
    return EFI_UNSUPPORTED;

  case EFI_BROWSER_ACTION_CHANGING:
    if (QuestionId == H2O_PCI_DEVICE_INFO_SHOW_ACTUAL_DEVICE_NAME_QUESTION_ID) {
      PrivateData->Configuration.ShowActualDeviceName = Value->u8;
      UpdateForm ();
    }
    if (QuestionId == H2O_PCI_DEVICE_INFO_SHOW_DEVICE_TREE_QUESTION_ID) {
      PrivateData->Configuration.ShowDeviceTree = Value->u8;
      UpdateForm ();
    }
    if (QuestionId == H2O_PCI_DEVICE_INFO_DISPLAY_CAPABILITY_INTPN_QUESTION_ID) {
      PrivateData->Configuration.DisplayCapabilityIntpn = Value->u8;
      mInterpretationIndex = 0;
      mCapIndex            = 0;
      UpdatePciCapbilityList ();
      UpdatePcieCapbilityList ();
    }
    if ((QuestionId & B_SETUP_POLICY_DYNAMIC_BASE) == H2O_PCI_DEVICE_INFO_DYNAMIC_DEVICE_BASE_QUESTION_ID) {
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
  EFI_STATUS                                  Status;
  UINTN                                       BufferSize;
  PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA  *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL             *HiiConfigRouting;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "PciDeviceInfoSetupUtility %a: Start\n", __FUNCTION__));

  PrivateData       = PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA_FROM_THIS (This);
  HiiConfigRouting  = PrivateData->HiiConfigRouting;
  *Progress         = Configuration;

  if (!HiiIsConfigHdrMatch(Configuration, &gH2OPciDeviceInfoConfigurationGuid, mVariableName)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get Buffer Storage data from EFI variable
  //
  BufferSize = sizeof (PCI_DEVICE_INFO_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OPciDeviceInfoConfigurationGuid,
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
  BufferSize = sizeof (PCI_DEVICE_INFO_CONFIGURATION);
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
             &gH2OPciDeviceInfoConfigurationGuid,
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
             sizeof (PCI_DEVICE_INFO_CONFIGURATION),
             &PrivateData->Configuration
             );

  DEBUG ((EFI_D_INFO, "PciDeviceInfoSetupUtility %a: End, Status: %r\n", __FUNCTION__, Status));

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
  EFI_STATUS                                  Status;
  UINTN                                       BufferSize;
  PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA  *PrivateData;
  EFI_HII_CONFIG_ROUTING_PROTOCOL             *HiiConfigRouting;
  EFI_STRING                                  ConfigRequest;
  EFI_STRING                                  ConfigRequestHdr;
  UINTN                                       Size;
  CHAR16                                      *StrPointer;
  BOOLEAN                                     AllocatedRequest;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((EFI_D_INFO, "PciDeviceInfoSetupUtility %a: Start\n", __FUNCTION__));

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr  = NULL;
  ConfigRequest     = Request;
  Size              = 0;
  *Progress         = Request;
  AllocatedRequest  = FALSE;

  PrivateData       = PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA_FROM_THIS (This);
  HiiConfigRouting  = PrivateData->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (PCI_DEVICE_INFO_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OPciDeviceInfoConfigurationGuid,
             &BufferSize,
             &PrivateData->Configuration
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "%a() - Fail to CommonGetVariable, Status = %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  if (Request == NULL) {
    //
    // Request has no request element, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gH2OPciDeviceInfoConfigurationGuid, mVariableName, PrivateData->DriverHandle);
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
    if (!HiiIsConfigHdrMatch (Request, &gH2OPciDeviceInfoConfigurationGuid, mVariableName)) {
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

  DEBUG ((EFI_D_INFO, "PciDeviceInfoSetupUtility %a: End, Status: %r\n", __FUNCTION__, Status));

  return Status;
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
  EFI_STATUS  Status;
  LIST_ENTRY  *Entry;
  UINTN       IgnoreTableSize;

  Status = mPrivateData->PciDeviceInfo->GetPciDeviceInfoPool (
             mPrivateData->PciDeviceInfo,
             &mPciDeviceInfoPool
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to GetPciDeviceInfoPool, Status: %r\n", __FUNCTION__, Status));
    return Status;
  }

  mNumberOfPciDevices = 0;
  BASE_LIST_FOR_EACH (Entry, mPciDeviceInfoPool) {
    ++mNumberOfPciDevices;
  }
  mPciDeviceStringIds     = AllocateZeroPool (mNumberOfPciDevices * sizeof (EFI_STRING_ID));
  mPciDeviceHelpStringIds = AllocateZeroPool (mNumberOfPciDevices * sizeof (EFI_STRING_ID));

  UpdateDeviceHelpStrings ();

  mIgnoreTable      = (IGNORE_DEVICE_TABLE_ENTRY *) PcdGetPtr (PcdH2OPciSkipDeviceTable);
  IgnoreTableSize   = PcdGetSize (PcdH2OPciSkipDeviceTable);
  mIgnoreTableCount = IgnoreTableSize / sizeof (IGNORE_DEVICE_TABLE_ENTRY);

  return EFI_SUCCESS;
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
  H2O_PCI_DEVICE_INFO_PROTOCOL            *PciDeviceInfo;
  H2O_PCI_LOOKUP_TABLE_PROTOCOL           *PciLookupTable;

  DEBUG ((EFI_D_INFO, "PciDeviceInfoSetupUtility %a: Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gEfiSetupUtilityApplicationProtocolGuid, NULL, (VOID **) &SetupUtilityApp);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, "SetupUtilityApp->VfrDriverState = %d\n", SetupUtilityApp->VfrDriverState));

  if (SetupUtilityApp->VfrDriverState == InitializeSetupUtility) {
    Status = gBS->LocateProtocol (&gH2OPciDeviceInfoProtocolGuid, NULL, (VOID **) &PciDeviceInfo);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_INFO,
        "%a() - Fail to locate protocol %g, Status: %r\n",
        __FUNCTION__,
        gH2OPciDeviceInfoProtocolGuid,
        Status
        ));
      return;
    }

    Status = gBS->LocateProtocol (&gH2OPciLookupTableProtocolGuid, NULL, (VOID **) &PciLookupTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        EFI_D_INFO,
        "%a() - Fail to locate protocol %g, Status: %r\n",
        __FUNCTION__,
        gH2OPciLookupTableProtocolGuid,
        Status
        ));
      return;
    }

    mPrivateData->PciDeviceInfo   = PciDeviceInfo;
    mPrivateData->PciLookupTable  = PciLookupTable;

    UpdateGlobalData ();
    UpdateForm ();
  }

  DEBUG ((EFI_D_INFO, "PciDeviceInfoSetupUtility %a: End\n", __FUNCTION__));
}

/**
  Entry point of PciDeviceInfoSetupUtilityDxe

  @param[in]  ImageHandle     The image handle.
  @param[in]  SystemTable     The system table.

  @retval   EFI_SUCCESS     Function complete successfully.
  @retval   others          Function complete unsuccessfully.

**/
EFI_STATUS
PciDeviceInfoSetupUtilityDxeEntry (
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

  GetPciCapabilityDisplayControlTable ();

  mPrivateData = AllocateZeroPool (sizeof (PCI_DEVICE_INFO_SETUP_UTILITY_PRIVATE_DATA));
  if (mPrivateData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  mPrivateData->Signature                   = PCI_DEVICE_INFO_SETUP_UTILITY_SIGNATURE;
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
                              &gH2OPciDeviceInfoSetupFormsetGuid,
                              mPrivateData->DriverHandle,
                              PciDeviceInfoVfrBin,
                              PciDeviceInfoSetupUtilityDxeStrings,
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

  //
  // Capability Interpretation String initialized
  //
  LibPciCapabilityIntnStringInit();

  mHiiHandle = mPrivateData->HiiHandle;

  //
  // Check the status of the setup variable. Note that it has a read-only
  // default value stored in the variable defaults region.
  //
  BufferSize = sizeof (PCI_DEVICE_INFO_CONFIGURATION);
  Status = CommonGetVariable (
             mVariableName,
             &gH2OPciDeviceInfoConfigurationGuid,
             &BufferSize,
             &mPrivateData->Configuration
             );
  if (EFI_ERROR (Status) ||
      !IsVariableInVariableStoreRegion (mVariableName, &gH2OPciDeviceInfoConfigurationGuid)) {
    BufferSize = sizeof (PCI_DEVICE_INFO_CONFIGURATION);
    Status = InitVarDefault (
               mVariableName,
               &gH2OPciDeviceInfoConfigurationGuid,
               &BufferSize,
               &mPrivateData->Configuration
               );
    if (EFI_ERROR (Status)) {
      goto ErrorExit;
    }
  }

  ConfigRequestHdr = HiiConstructConfigHdr (
                       &gH2OPciDeviceInfoConfigurationGuid,
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