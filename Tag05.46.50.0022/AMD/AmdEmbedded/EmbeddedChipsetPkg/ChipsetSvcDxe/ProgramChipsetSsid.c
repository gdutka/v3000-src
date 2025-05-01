/** @file
 DXE Chipset Services Library.

 This file contains only one function that is DxeCsSvcProgramChipsetSsid().
 The function DxeCsSvcProgramChipsetSsid() use chipset services to program subsystem vendor identification.

***************************************************************************
* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PciExpressLib.h>

#include <Protocol/AmdCpmDisplayFeatureProtocol/AmdCpmDisplayFeatureProtocol.h>
#include <FchRegs.h>

#define DEVICE_ID_DONT_CARE           0xFFFF

typedef
VOID
(*SPECIAL_PROGRAM_MECHANISM) (
  IN     UINT8    Bus,
  IN     UINT8    Dev,
  IN     UINT8    Func,
  IN     UINT32   SsidSvid
  );

typedef struct {
  UINT16                       VendorId;
  UINT16                       DeviceId;
  SPECIAL_PROGRAM_MECHANISM    SpecialSsidSvidFunction;
} SPECIFIC_SSID_SVID_TABLE;

EFI_STATUS
DisableAmdCpmSetGpuSsidSvid (
  )
{
  EFI_STATUS                       Status;
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL *CpmDisplayFeatureProtocolPtr;
  AMD_CPM_DISPLAY_FEATURE_TABLE    *DisplayFeatureTablePtr;

  Status = gBS->LocateProtocol (
                  &gAmdCpmDisplayFeatureProtocolGuid,
                  NULL,
                  (VOID**)&CpmDisplayFeatureProtocolPtr
                  );
  if (!EFI_ERROR (Status)) {
    DisplayFeatureTablePtr = CpmDisplayFeatureProtocolPtr->DisplayFeatureData.DisplayFeatureTablePtr;
    DisplayFeatureTablePtr->FunctionDisableMask |= BIT0;
  }

  return Status;
}

EFI_STATUS
OemProgramIgdAdapterIdRegSsidSvid (
  IN     UINT8    Bus,
  IN     UINT8    Dev,
  IN     UINT8    Func,
  IN OUT UINT32   *SsidSvid
  )
{
  EFI_STATUS Status;
  UINT64     BootScriptPciAddress;

  Status = EFI_SUCCESS;

  //
  // Program SSID / SSVID
  //
  PciExpressWrite32 (PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Func, 0x4C), *SsidSvid);

  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus, Dev, Func, 0x4C);
  S3BootScriptSavePciCfgWrite (
    S3BootScriptWidthUint32,
    BootScriptPciAddress,
    1,
    SsidSvid
    );

  Status = DisableAmdCpmSetGpuSsidSvid ();

  return Status;
}

VOID
ProgramAdapterIdRegSsidSvid (
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Func,
  IN UINT32   SsidSvid
  )
{
  UINT64     BootScriptPciAddress;
  //
  // Program SSID / SSVID
  //
  PciExpressWrite32 (PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Func, 0x4C), SsidSvid);

  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus, Dev, Func, 0x4C);
  S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      BootScriptPciAddress,
      1,
      &SsidSvid);

  return;
}



EFI_STATUS
ProgramSsidSvid0xFFFF10DE (
  IN     UINT8    Bus,
  IN     UINT8    Dev,
  IN     UINT8    Func,
  IN     UINT32   SsidSvid
  )
{
  UINT64     BootScriptPciAddress;
  //
  // Program SSID / SSVID
  //
  PciExpressWrite32 (PCI_EXPRESS_LIB_ADDRESS(Bus, Dev, Func, 0x40), SsidSvid);

  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus, Dev, Func, 0x40);
  S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint32,
      BootScriptPciAddress,
      1,
      &SsidSvid);

  return EFI_SUCCESS;
}

//
// According to the description of 11.6.5.3 NBIFEPFNCFG Registers in A0_1.50_PPR_NRB_NDA.pdf,
// BXXD00F2x04C(ADAPTER_ID_W) Read-write. Controls the Subsystem ID value.
// 1022h 1649h,1022h 163Ah,1022h 163Bh, 1022h 15E2h,
// 1022h 15E3h,1022h 164Ah,1022h 149Dh, 1022h 163Dh, 1022h 163Eh
//
SPECIFIC_SSID_SVID_TABLE mSpecificSsidSvidTable[] = {
  {AMD_FCH_VID,         0x1649,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x163A,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x163B,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x15E2,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x15E3,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x164A,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x149D,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x163D,              ProgramAdapterIdRegSsidSvid},
  {AMD_FCH_VID,         0x163E,              ProgramAdapterIdRegSsidSvid},
  {DEVICE_ID_DONT_CARE, DEVICE_ID_DONT_CARE, NULL}
};

/**
 Program Chipset SSID

 @param[in]         Bus                 PCI Bus number
 @param[in]         Dev                 PCI Device number
 @param[in]         Func                PCI Function number
 @param[in]         VendorId            Vendor ID
 @param[in]         DeviceId            Device ID
 @param[in]         SsidVid             SsidVid

 @retval            EFI_SUCCESS         Function returns successfully
 @retval            EFI_UNSUPPORTED     1. The specific ID is not find.
                                        2. The specific device can not be set SSID.
*/
EFI_STATUS
ProgramChipsetSsid (
  IN UINT8                               Bus,
  IN UINT8                               Dev,
  IN UINT8                               Func,
  IN UINT16                              VendorId,
  IN UINT16                              DeviceId,
  IN UINT32                              SsidSvid
  )
{
  UINT8                               Index;

  for (Index = 0; mSpecificSsidSvidTable[Index].SpecialSsidSvidFunction != NULL; Index++) {
    if (mSpecificSsidSvidTable[Index].VendorId == VendorId) {
      if ((mSpecificSsidSvidTable[Index].DeviceId == DEVICE_ID_DONT_CARE)
        || (mSpecificSsidSvidTable[Index].DeviceId == DeviceId)) {
        mSpecificSsidSvidTable[Index].SpecialSsidSvidFunction (Bus, Dev, Func, SsidSvid);
        return EFI_SUCCESS;
      }
    }
  }
  return EFI_UNSUPPORTED;
}