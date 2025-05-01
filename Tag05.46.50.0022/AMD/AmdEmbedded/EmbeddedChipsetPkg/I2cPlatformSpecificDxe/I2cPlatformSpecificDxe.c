/** @file
  For I2C I2cPlatform Specific Dxe Driver

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "I2cPlatformSpecificDxe.h"

BOOLEAN
AmdGpioEventArrived (
  IN  I2C_BUS_INSTANCE   *I2cBusInstance,
  IN  UINT8              GpioController,
  IN  UINT8              GpioIntPin,
  IN  UINT8              ActiveLevel
  )
{
  UINT32  Gplr;

  Gplr = MmioRead32 (I2cBusInstance->GpioBase + (GpioIntPin * 0x04));

  if (((Gplr >> 16) & 1) == ActiveLevel) {
    return TRUE;
  }

  return FALSE;
}

VOID
AmdI2cwitchDevMode (
  IN  I2C_BUS_INSTANCE     *I2cBusInstance,
  IN UINT32                DeviceMode
  )
{
  return;
}

UINTN
AmdI2cGetDevMode (
  IN  I2C_BUS_INSTANCE   *I2cBusInstance
  )
{
  return I2C_DEVICE_MODE_MMIO;
}

/**
  Get the amount of I2c bus(bridge) on your platform.

  This routine must be called at or below TPL_NOTIFY.


  @param[out]   TotalControllerNumber  The max number of I2c bus.

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_ERROR         The function operation failed.

**/
EFI_STATUS
EFIAPI
GetMaxI2cControllerNumber (
  OUT UINT8              *TotalControllerNumber
 )
{
  EFI_STATUS             Status;

  Status = EFI_SUCCESS;

  *TotalControllerNumber = PcdGet8(I2cTotalControllerNumber);

  return Status;
}

VOID
GetOemConfigI2cSclSda (
  IN OUT  I2C_BUS_INSTANCE    *I2cBusInstance
  )
{
  I2cBusInstance->Info.SpeedClockInfo[0].SclHcnt = PcdGet16(I2cIcSsSclHcnt);
  I2cBusInstance->Info.SpeedClockInfo[0].SclLcnt = PcdGet16(I2cIcSsSclLcnt);
  I2cBusInstance->Info.SpeedClockInfo[0].SdaHold = PcdGet8(I2cIcSsSdaHold);
  I2cBusInstance->Info.SpeedClockInfo[1].SclHcnt = PcdGet16(I2cIcFsSclHcnt);
  I2cBusInstance->Info.SpeedClockInfo[1].SclLcnt = PcdGet16(I2cIcFsSclLcnt);
  I2cBusInstance->Info.SpeedClockInfo[1].SdaHold = PcdGet8(I2cIcFsSdaHold);
  I2cBusInstance->Info.SpeedClockInfo[2].SclHcnt = PcdGet16(I2cIcHsSclHcnt);
  I2cBusInstance->Info.SpeedClockInfo[2].SclLcnt = PcdGet16(I2cIcHsSclLcnt);
  I2cBusInstance->Info.SpeedClockInfo[2].SdaHold = PcdGet8(I2cIcHsSdaHold);

  return;
}


/**
  Get the bus(bridge) data on your platform.

  This routine must be called at or below TPL_NOTIFY.


  @param[in]     This       Pointer to an EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL structure.

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_ERROR         The function operation failed.

**/
EFI_STATUS
EFIAPI
GetI2cConfigData (
  IN       EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL   *This,
  IN OUT   I2C_BUS_INSTANCE                     *I2cBusInstance,
  IN       UINTN                                ControllerNum,
  OUT      UINT8                                *I2cHcType
 )
{
  UINT32                   Index;
  EFI_STATUS               Status;
  UINT32                   Eax;
  UINT32                   Ebx;
  UINT32                   Ecx;
  UINT32                   Edx;
  // UINTN                    PciCfgBase;
  // UINT64                   BaseAddress;

  Index  = 0;
  Status = EFI_SUCCESS;

  AsmCpuid ( 1, &Eax, &Ebx, &Ecx, &Edx); // Get CPUID_VERSION_INFO
  Eax &= 0x0FFF0FF0;
  I2cBusInstance->Info.Cpuid = Eax;
  DEBUG ((EFI_D_INFO, "--- Get I2C Device ConfigData ---, CPUID:0x%08x\n", I2cBusInstance->Info.Cpuid));

  // BaseAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, (UINT8)I2cBusInstance->PciI2c.Bus, (UINT8)I2cBusInstance->PciI2c.Dev, (UINT8)I2cBusInstance->PciI2c.Func, 0);
  // PciCfgBase  = (UINTN) GetSerialIoI2cPciCfg ((UINT8)ControllerNum);
  // if (mI2cBusInfo[ControllerNum].I2cPciDevId != PciSegmentRead16 (PciCfgBase + PCI_DEVICE_ID_OFFSET)) {
  //   return EFI_DEVICE_ERROR;
  // }
  while (mI2cBusInfo[Index].Cpuid != 0xFFFFFFFF) {
    if (mI2cBusInfo[Index].Cpuid == I2cBusInstance->Info.Cpuid) {
      CopyMem (&I2cBusInstance->Info, &(mI2cBusInfo[Index]), sizeof (I2C_INFO));
      break;
    }
    Index++;
  }

  if (mI2cBusInfo[Index].Cpuid == 0xFFFFFFFF) {
    return EFI_DEVICE_ERROR;
  }

  // I2cBusInstance->PciI2c.Seg  = 0;
  // I2cBusInstance->PciI2c.Bus  = 0;
  // I2cBusInstance->PciI2c.Dev  = 0;
  // I2cBusInstance->PciI2c.Func = 0;

  I2cBusInstance->I2cBase     =  PcdGet64 (I2cBase) + (UINT64)(0x1000 * ControllerNum);
  // I2cBusInstance->I2cBase     = GetSerialIoBar(PciCfgBase);
  I2cBusInstance->I2cBaseLen  =  0x1000;

  //
  //  For CFL/WHL/CML,
  //  Read GPIO value, MmioRead32 (PCH_PCR_ADDRESS (GpioGroupInfo[GroupIndex].Community, RegOffset));
  //
  I2cBusInstance->GpioBase    = PcdGet32 (I2cGpioBase);

  *I2cHcType = I2C_HC_TYPE_MMIO;

  if (PcdGetBool (OemHookI2cSclSdaEnable)) {
    GetOemConfigI2cSclSda (I2cBusInstance);
  }

  DEBUG ((EFI_D_INFO, "--- Get I2C Device ConfigData --- \n"));
  DEBUG ((EFI_D_INFO, "--- I2C Bus Bar0:0x%08x, Bar0 length:0x%08x, GpioBase:0x%08x\n", I2cBusInstance->I2cBase, I2cBusInstance->I2cBaseLen, I2cBusInstance->I2cBaseLen));
  DEBUG ((EFI_D_INFO, "--- I2C Bus Type (PCI:0/MMIO:1):%01x\n", *I2cHcType));
  DEBUG ((EFI_D_INFO, "--- SS I2C Clock SCL High Count: 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[0].SclHcnt));
  DEBUG ((EFI_D_INFO, "--- SS I2C Clock SCL Low Count : 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[0].SclLcnt));
  DEBUG ((EFI_D_INFO, "--- SS I2C Clock SDA Hold Time : 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[0].SdaHold));
  DEBUG ((EFI_D_INFO, "--- FS I2C Clock SCL High Count: 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[1].SclHcnt));
  DEBUG ((EFI_D_INFO, "--- FS I2C Clock SCL Low Count : 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[1].SclLcnt));
  DEBUG ((EFI_D_INFO, "--- FS I2C Clock SDA Hold Time : 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[1].SdaHold));
  DEBUG ((EFI_D_INFO, "--- HS I2C Clock SCL High Count: 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[2].SclHcnt));
  DEBUG ((EFI_D_INFO, "--- HS I2C Clock SCL Low Count : 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[2].SclLcnt));
  DEBUG ((EFI_D_INFO, "--- HS I2C Clock SDA Hold Time : 0x%04x\n", I2cBusInstance->Info.SpeedClockInfo[2].SdaHold));

  return Status;
}

/**
 Unloads an image.

 @param[in]         ImageHandle           Handle that identifies the image to be unloaded.

 @retval            EFI_SUCCESS           The image has been unloaded.
 @retval            EFI_INVALID_PARAMETER One of the protocol interfaces was not previously installed on ImageHandle.
**/
EFI_STATUS
EFIAPI
I2cPlatformSpecificDxeUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  EFI_STATUS  Status;

  //
  // Uninstall Driver Supported EFI Version Protocol from the image handle.
  //
  Status = gBS->UninstallProtocolInterface (
                  ImageHandle,
                  &gEfiI2cPlatformSpecificProtocolGuid,
                  NULL
                  );
  DEBUG ((EFI_D_INFO, "UninstallProtocolInterface gEfiI2cPlatformSpecificProtocolGuid : %x \n" , Status));

  return Status;
}
//
// The protocol interface is used to describe how to control or get platform specific data for I2C devices, controllers or bus.
//
EFI_STATUS
EFIAPI
I2cPlatformSpecificDxeEntry (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                          Status;
  EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL  *I2cPlatformSpecific;

  DEBUG ((EFI_D_INFO, "I2cPlatformSpecificDxeEntry Start. \n"));

  I2cPlatformSpecific = AllocateZeroPool (sizeof(EFI_I2C_PLATFORM_SPECIFIC_PROTOCOL));
  if (I2cPlatformSpecific == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  I2cPlatformSpecific->Revision                  = I2C_PLATFORM_SPECIFIC_VERSION;
  I2cPlatformSpecific->GetMaxI2cControllerNumber = GetMaxI2cControllerNumber;
  I2cPlatformSpecific->GetI2cConfigData          = GetI2cConfigData;

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gEfiI2cPlatformSpecificProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  I2cPlatformSpecific
                  );

  DEBUG ((EFI_D_INFO, "I2cPlatformSpecificDxeEntry End. \n"));
  return Status;
}
