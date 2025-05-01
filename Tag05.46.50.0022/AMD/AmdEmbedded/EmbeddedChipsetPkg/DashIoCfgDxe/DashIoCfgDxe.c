/** @file

;*******************************************************************************
;* Copyright (c) 2013 - 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <DashIoCfgDxe.h>
DASH_IO_CFG_CONTEXT            mDashIoCfgContext;
DASH_VERSION                   mDashVersion = {0, 92, 16, 5};
UINT16                         mDashIODeviceVendorID;

VOID
GetDashVersion (
  IN DASH_IO_CFG_PROTOCOL *This,
  OUT DASH_VERSION *Version
)
{
  *Version = mDashVersion;
}

UINT8
AsfReadReg (
  IN  DASH_IO_CFG_PROTOCOL     *This,
  IN  UINT8                      Reg
)
{
  UINT8                             Data;
  DASH_IO_CFG_CONTEXT                *DashIoCfgContext;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;

  DashIoCfgContext = DASH_IO_CFG_CONTEXT_FROM_THIS (This);
  IoDev = DashIoCfgContext->PciRootBridgeIo;
  IoDev->Io.Read (IoDev, EfiPciWidthUint8, DashIoCfgContext->AsfIoBase + Reg, 1, &Data);

  return Data;

}

VOID
AsfWriteReg (
  IN  DASH_IO_CFG_PROTOCOL     *This,
  IN  UINT8                   Reg,
  IN  UINT8                   Data
)
{
  DASH_IO_CFG_CONTEXT                *DashIoCfgContext;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;

  DashIoCfgContext = DASH_IO_CFG_CONTEXT_FROM_THIS (This);
  IoDev = DashIoCfgContext->PciRootBridgeIo;
  IoDev->Io.Write (IoDev, EfiPciWidthUint8, DashIoCfgContext->AsfIoBase + Reg, 1, &Data);

}

VOID
InitAsf(
  IN  DASH_IO_CFG_PROTOCOL     *This
  )
{
  UINT8   Data8;

  // Setting ASF slave address for remote power control
  This->AsfWriteReg (This, ASF_REMOTE_CTRL_ADR, ASF_HOST_SMBUS_SLAVE_ADDRESS);

  // Enable PEC
  This->AsfWriteReg (This, ASF_PEC, 0x01);

  // Clear SuspendSlave
  Data8 = This->AsfReadReg (This, ASF_SLAVE_EN);
  Data8 &= ~(UINT8) SUSPEND_SLAVE;
  This->AsfWriteReg (This, ASF_SLAVE_EN, Data8);

  //
  // Clear AsfSmMasterEn
//[-start-220126-IB14740207-modify]//
  Data8 = MmioRead8 (ACPI_MMIO_BASE + PMIO_BASE + 0x02);
  Data8 &= 0xFE;
  MmioWrite8 (ACPI_MMIO_BASE + PMIO_BASE + 0x02, Data8);
//[-end-220126-IB14740207-modify]//

}

UINT16
UartGetVendorId (
  VOID
)
{
  //return DASH_SOL_VENDOR_ID;
  return mDashIODeviceVendorID;
}

UINT16
UartGetDeviceId (
  VOID
)
{
  if (mDashIODeviceVendorID == PCI_VENDOR_ID_BROADCOM) {
    return DASH_SOL_DEVICE_ID_BROADCOM;
  }
  
  if (mDashIODeviceVendorID == PCI_VENDOR_ID_REALTEK) {
    return DASH_SOL_DEVICE_ID_REALTEK;
  }

  return 0xFFFF;
}

UINT8
GetHostSmbusSlaveAddr(
  VOID
)
{

  if (mDashIODeviceVendorID == PCI_VENDOR_ID_REALTEK) {
    return MCTP_HOST_SMBUS_SLAVE_ADD_REALTEK;
  }

  return MCTP_HOST_SMBUS_SLAVE_ADD_BROARDCOM;
}

UINT8
GetMcSmbusAddr(
  VOID
)
{
  if (mDashIODeviceVendorID == PCI_VENDOR_ID_REALTEK) {
    return MCTP_MC_SMBUS_ADDRESS_REALTEK;
  }

  return MCTP_MC_SMBUS_ADDRESS_BOARDCOM;
}

UINT8
GetHostEndpointId(
  VOID
)
{
  return MCTP_HOST_ENDPOINT_ID;
}

UINT8
GetMcEndpointId(
  VOID
)
{
  return MCTP_MC_ENDPOINT_ID;
}


BOOLEAN ManagementControllerFound (
  VOID
)
{
  EFI_PCI_IO_PROTOCOL  *PciIo;
  EFI_STATUS           Status;
  UINTN                NoPciIoHandles;
  EFI_HANDLE           *PciIoHandles;
  UINTN                Index;
  UINT16               VendorDevice[2];
  BOOLEAN              Found;
  BOOLEAN              BroadcomDeviceFound;
  BOOLEAN              RealtekDeviceFound;

  PciIoHandles = NULL;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &NoPciIoHandles,
                  &PciIoHandles
                  );

  if (EFI_ERROR(Status )) {
    return FALSE;
  }

  Found = FALSE;
  BroadcomDeviceFound = FALSE;
  RealtekDeviceFound = FALSE;
  for (Index = 0; Index < NoPciIoHandles; Index++) {
      Status = gBS->HandleProtocol (
                      PciIoHandles[Index],
                      &gEfiPciIoProtocolGuid,
                      &PciIo
                      );
      if (EFI_ERROR(Status))  continue;

      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint16,
                            0,
                            2,
                            VendorDevice
                           );
      if (Status == EFI_SUCCESS) {
        if (VendorDevice[0] == PCI_VENDOR_ID_BROADCOM &&
           (VendorDevice[1] == PCI_DEVICE_ID_B5761  ||
            VendorDevice[1] == PCI_DEVICE_ID_B5761E ||
            VendorDevice[1] == PCI_DEVICE_ID_B5762)) {
          BroadcomDeviceFound = TRUE;
          break;
        }
        if ((VendorDevice[0] == PCI_VENDOR_ID_REALTEK) &&
            (VendorDevice[1] == PCI_DEVICE_ID_8168)) {
          RealtekDeviceFound = TRUE;
        }
     }
  }

  if (BroadcomDeviceFound) {
    Found = TRUE;
    mDashIODeviceVendorID = PCI_VENDOR_ID_BROADCOM;
  } else if (RealtekDeviceFound) {
    Found = TRUE;
    mDashIODeviceVendorID = PCI_VENDOR_ID_REALTEK;
  }
  gBS->FreePool (PciIoHandles);
  return Found;
}

/**
  The user Entry Point for module DashIoCfgDxe.  The user code starts with this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
DashIoCfgDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                            Status;

  EFI_HANDLE                            Handle;
  UINTN                                 HandleSize;
  CHIPSET_CONFIGURATION                 ChipsetSetup;

  DEBUG ((EFI_D_ERROR, "\nDashIoCfgDxeEntryPoint Start\n"));
  Status = GetChipsetConfiguration (&ChipsetSetup);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
   DEBUG ((EFI_D_ERROR, "Dash support:%x\n", ChipsetSetup.DashSupport ));
  if (ChipsetSetup.DashSupport == 0) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &mDashIoCfgContext.PciRootBridgeIo
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (!ManagementControllerFound()) {
    return EFI_NOT_FOUND;
  }

  //
  // Duplicated driver installation check
  //
  HandleSize = sizeof (Handle);
  Status = gBS->LocateHandle (
                  ByProtocol,
                  &gDashIoCfgProtocolGuid,
                  NULL,
                  &HandleSize,
                  &Handle
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Driver found, return EFI_ALREADY_STARTED status
    //
    return EFI_ALREADY_STARTED;
  }

  mDashIoCfgContext.Signature                        = DASH_IO_CFG_SIGNATURE;
  mDashIoCfgContext.AsfIoBase                        = ASF_IO_BASE;
  mDashIoCfgContext.DashIoCfg.AsfReadReg             = AsfReadReg;
  mDashIoCfgContext.DashIoCfg.AsfWriteReg            = AsfWriteReg;
  mDashIoCfgContext.DashIoCfg.InitAsf                   = InitAsf;
  mDashIoCfgContext.DashIoCfg.UartGetDeviceId           = UartGetDeviceId;
  mDashIoCfgContext.DashIoCfg.UartGetVendorId           = UartGetVendorId;
  mDashIoCfgContext.DashIoCfg.GetHostSmbusSlaveAddr     = GetHostSmbusSlaveAddr;
  mDashIoCfgContext.DashIoCfg.GetMcSmbusAddr            = GetMcSmbusAddr;
  mDashIoCfgContext.DashIoCfg.GetHostEndpointId         = GetHostEndpointId;
  mDashIoCfgContext.DashIoCfg.GetMcEndpointId           = GetMcEndpointId;
  mDashIoCfgContext.DashIoCfg.GetDashVersion            = GetDashVersion;

  //
  // Protocol interface installation
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                                           &ImageHandle,
                                           &gDashIoCfgProtocolGuid,
                                           &mDashIoCfgContext.DashIoCfg,
                                           NULL
                                         );

  return Status;
}

