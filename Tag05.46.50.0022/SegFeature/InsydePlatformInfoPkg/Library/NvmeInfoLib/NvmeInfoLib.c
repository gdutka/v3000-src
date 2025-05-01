/** @file
  Implementation of NvmeInfoLib.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/
#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

#include <IndustryStandard/Nvme.h>
#include <IndustryStandard/Pci22.h>

#include <Protocol/DevicePath.h>
#include <Protocol/NvmExpressPassthru.h>
#include <Protocol/PciIo.h>
#include <Protocol/Smbios.h>
#include <Protocol/H2ONvmeInfo.h>             // InsydePlatformInfoPkg

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/NvmeInfoLib.h>              // InsydePlatformInfoPkg

extern CHAR8 *gEfiCallerBaseName; // From AutoGen.c

/**
  Check the Class Code to determine if the NVMe is Raid Controller.

  @retval TRUE        It is Raid Controller.
  @retval FALSE       It's not Raid Controller.

**/
BOOLEAN
IsNvmeRaidController (
  IN EFI_HANDLE   BcHandle
  )
{
  EFI_STATUS              Status;
  EFI_PCI_IO_PROTOCOL     *PciIo;
  PCI_TYPE00              Pci;

  Status = gBS->OpenProtocol (
                  BcHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo,
                  gImageHandle, // AgentHandle
                  NULL,         // ControllerHandle
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0, sizeof (Pci), &Pci);
  if (Pci.Hdr.ClassCode[1] == PCI_SUB_CLASS_CODE_RAID_CONTROLLER &&
      Pci.Hdr.ClassCode[2] == PCI_BASE_CLASS_CODE_STORAGE_CONTROLLER)
  {
    return TRUE;
  }

  return FALSE;
}

STATIC
EFI_DEVICE_PATH_PROTOCOL *
CreateNvmeNamespaceDevicePathNode (
  IN H2O_NVME_INFO_PROTOCOL              *NvmeInfo,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN UINT32                              NamespaceId
  )
{
  EFI_STATUS                 Status;
  EFI_DEVICE_PATH_PROTOCOL   *DevicePathNode;
  UINTN                      NodeSize;
  UINT64                     Eui64;
  NVME_ADMIN_NAMESPACE_DATA  *NamespaceData;

  if ((NvmeInfo == NULL) || (NvmePassThru == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmeInfo/NvmePassThru is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  Status = NvmePassThru->BuildDevicePath (
                           NvmePassThru,
                           NamespaceId,
                           &DevicePathNode
                           );
  if (!EFI_ERROR (Status)) {
    return DevicePathNode;
  }

  //
  // In case of a driver that doesn't support building the Device Path node
  // (It could happen with an Intel VMD controller driver.)
  //
  DEBUG ((
    EFI_D_WARN,
    "%a: NVMe Pass Thru Protocol %p BuildDevicePath(): %r\n",
    gEfiCallerBaseName,
    NvmePassThru,
    Status
    ));

  NodeSize = sizeof (NVME_NAMESPACE_DEVICE_PATH);
  DevicePathNode = AllocatePool (NodeSize);
  if (DevicePathNode == NULL) {
    return NULL;
  }

  Eui64 = 0;

  Status = NvmeInfo->IdentifyNamespace (
                       NvmePassThru,
                       NamespaceId,
                       &NamespaceData
                       );
  if (!EFI_ERROR (Status)) {
    //
    // Big endian to little endian
    //
    Eui64 = SwapBytes64 (NamespaceData->Eui64);
    FreePool (NamespaceData);
  }

  DevicePathNode->Type      = MESSAGING_DEVICE_PATH;
  DevicePathNode->SubType   = MSG_NVME_NAMESPACE_DP;
  DevicePathNode->Length[0] = (UINT8) (NodeSize);
  DevicePathNode->Length[1] = (UINT8) (NodeSize >> 8);
  ((NVME_NAMESPACE_DEVICE_PATH *) DevicePathNode)->NamespaceId = NamespaceId;
  ((NVME_NAMESPACE_DEVICE_PATH *) DevicePathNode)->NamespaceUuid = Eui64;

  return DevicePathNode;
}

EFI_DEVICE_PATH_PROTOCOL *
CreateNvmeNamespaceDevicePath (
  IN H2O_NVME_INFO_PROTOCOL              *NvmeInfo,
  IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassThru,
  IN CONST EFI_DEVICE_PATH_PROTOCOL      *DevicePathPrefix,
  IN UINT32                              NamespaceId
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  if ((NvmeInfo == NULL) || (NvmePassThru == NULL) || (DevicePathPrefix == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NvmeInfo/NvmePassThru/DevicePathPrefix is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return NULL;
  }

  if (NamespaceId <= 0) {
    return NULL;
  }
  DevicePathNode = CreateNvmeNamespaceDevicePathNode (
                       NvmeInfo,
                       NvmePassThru,
                       NamespaceId
                       );
  if (DevicePathNode == NULL) {
    return NULL;
  }
  DevicePath = AppendDevicePathNode (DevicePathPrefix, DevicePathNode);
  FreePool (DevicePathNode);
  return DevicePath;
}

STATIC
UINTN
GetSpacePaddedStringLength (
  IN CONST CHAR8  *String,
  IN UINTN        MaxLength
  )
{
  UINTN  Length;
  UINTN  Index;

  if (String == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The String is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return 0;
  }

  Length = 0;
  for (Index = 0; Index < MaxLength; Index++) {
    //
    // Assume String is not sane.
    // ASCII string reported in any NVMe data structure only allows characters
    // in the range [0x20, 0x7E].
    //
    if (String[Index] < ' ' || String[Index] > '~') {
      break;
    }
    if (String[Index] > ' ') {
      Length = Index + 1;
    }
  }
  return Length;
}

STATIC
CHAR16 *
NvmeAsciiStringToUnicodeString (
  IN CONST CHAR8  *NvmeAsciiString,
  IN UINTN        MaxLength
  )
{
  UINTN   Length;
  UINTN   Index;
  CHAR16  *UnicodeString;

  Length = GetSpacePaddedStringLength (
             NvmeAsciiString,
             MaxLength
             );
  UnicodeString = AllocateZeroPool ((Length + 1) * sizeof (CHAR16));
  if (UnicodeString != NULL) {
    for (Index = 0; Index < Length; Index++) {
      UnicodeString[Index] = (CHAR16) NvmeAsciiString[Index];
    }
  }
  return UnicodeString;
}

/**
**/
CHAR16 *
GetNvmeSerialNumberFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  )
{
  return NvmeAsciiStringToUnicodeString (
           (CONST CHAR8 *) IdentifyControllerData->Sn,
           20 // MaxLength
           );
}

/**
**/
CHAR16 *
GetNvmeModelNumberFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  )
{
  return NvmeAsciiStringToUnicodeString (
           (CONST CHAR8 *) IdentifyControllerData->Mn,
           40 // MaxLength
           );
}

/**
**/
CHAR16 *
GetNvmeFwRevisionFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  )
{
  return NvmeAsciiStringToUnicodeString (
           (CONST CHAR8 *) IdentifyControllerData->Fr,
           8 // MaxLength
           );
}

/**
**/
CHAR16 *
GetNvmeIeeeOuiFromIdentifyData (
  IN CONST NVME_ADMIN_CONTROLLER_DATA  *IdentifyControllerData
  )
{
  return CatSPrint (
           NULL,
           L"%02X-%02X-%02X",
           IdentifyControllerData->Ieee_oui[2],
           IdentifyControllerData->Ieee_oui[1],
           IdentifyControllerData->Ieee_oui[0]
           );
}

/**
  Retrieves capacity and LBA data size from an NVMe Identify Namespace data.

  Retrieves capacity in number of logical blocks and LBA data size in bytes
  from NVMe Identify Namespace data structure.

**/
VOID
GetNvmeNamespaceCapacity (
  IN CONST NVME_ADMIN_NAMESPACE_DATA  *NamespaceData,
  OUT UINT32                          *BlockSize,
  OUT UINT64                          *NumberOfBlocks
  )
{
  UINT8                 LbaFormatIndex;
  CONST NVME_LBAFORMAT  *LbaFormat;

  if ((NamespaceData == NULL) || (BlockSize == NULL) || (NumberOfBlocks == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The NamespaceData/BlockSize/NumberOfBlocks is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return;
  }

  //
  // Note: The "NVM Capacity (NVMCAP)" (bytes 48 - 63) is Optional field.
  // See NVMe specification for when the field is unsupported.
  //

  *BlockSize = 0;
  *NumberOfBlocks = 0;

  LbaFormatIndex = NamespaceData->Flbas;
  LbaFormatIndex = (LbaFormatIndex & 0x0F) | ((LbaFormatIndex >> 1) & 0x30);

  //
  // NVME_ADMIN_NAMESPACE_DATA defined in <IndustryStandard/Nvme.h> might not
  // be updated for NVMe 2.0 specification (which extends maximum number of
  // LBA Formats from 16 to 64).
  //
  LbaFormat = (NamespaceData->LbaFormat + LbaFormatIndex);

  if (LbaFormat->Lbads < 9 || LbaFormat->Lbads > 31) {
    DEBUG ((
      EFI_D_ERROR,
      "%a: Unsupported LBA Data Size value (%u)\n",
      gEfiCallerBaseName,
      (unsigned int) LbaFormat->Lbads
      ));
    return;
  }

  *BlockSize      = (1 << LbaFormat->Lbads);
  *NumberOfBlocks = NamespaceData->Ncap;
  return;
}

/**
  Converts IEEE EUI-64 data to a Unicode string.

  Converts IEEE Extended Unique Identifier (EUI-64) data to a Unicode string.

  @param[in]  Eui64  Pointer to the EUI-64 value. The value must be in
                     big endian.

  @retval  NULL    Not enough memory to allocate the string.
  @retval  Others  Converted string.

**/
CHAR16 *
ConvertEui64ToString (
  IN CONST UINT8  Eui64[8]
  )
{
  return CatSPrint (
           NULL,
           L"%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X",
           Eui64[0],
           Eui64[1],
           Eui64[2],
           Eui64[3],
           Eui64[4],
           Eui64[5],
           Eui64[6],
           Eui64[7]
           );
}
