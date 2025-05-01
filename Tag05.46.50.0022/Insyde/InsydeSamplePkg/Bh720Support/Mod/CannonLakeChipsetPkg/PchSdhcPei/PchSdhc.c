/** @file
  Implementation of PchSdhcPei module for Crisis Recovery

;******************************************************************************
;* Copyright (c) 2016 - 2018, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "PchSdhc.h"

/**
  Retrieve SDHC controller information.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   This                     Pointer to PEI_SD_CONTROLLER_PPI
  @param [in]   SdControllerId           SDHC Controller ID
  @param [out]  ControllerType           Result SDHC controller type
  @param [out]  PciCfgAddress            Result SDHC base address
 
  @retval EFI_SUCCESS                    SDHC controller information is retrieved successfully
  @retval EFI_INVALID_PARAMETER          Invalid SdControllerId is given
  @retval Others                         Operation failed

**/
EFI_STATUS
EFIAPI
GetSdhcController (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       PEI_SD_CONTROLLER_PPI      *This,
  IN       UINT8                      SdControllerId,
  OUT      UINTN                      *ControllerType,
  OUT      UINT64                     *PciCfgAddress
  );  

/**
  Register notify ppi to reset the SDHC.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure
  @param[in] Ppi               Address of the PPI that was installed

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
STATIC  
EFI_STATUS  
EFIAPI  
EndOfPeiPpiNotifyCallback (  
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN CONST EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN       VOID                       *Ppi
  );                                

STATIC PEI_SD_CONTROLLER_PPI mSdControllerPpi = { GetSdhcController };

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiSdControllerPpiGuid,
  NULL
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  EndOfPeiPpiNotifyCallback
};

/**
  Enable the bridge connects to the SDHC.

  @param [in]   PeiServices              Pointer to the PEI Services Table
  @param [in]   Address                  Target SDHC PCI Cfg Adreess
  @param [in]   BaseAddress              Target SDHC Base Address
  @param [out]  BridgeAddress            The bridge PCI Cfg Address
 
  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
STATIC
EFI_STATUS
EnableBridgeConnectToSdhc (
  IN  CONST EFI_PEI_SERVICES           **PeiServices,
  IN        UINT32                     Address,
  IN        UINT32                     BaseAddress,
  OUT       UINT32                     *BridgeAddress
  )
{
  EFI_PEI_PCI_CFG2_PPI      *PciCfgPpi;
  UINTN                     Index;
  UINT32                    Bridge;
  UINT32                    OrgSecBusNum;
  UINT32                    Register;
  UINT8                     Bus;
  UINT8                     BarType;

  PciCfgPpi = (**PeiServices).PciCfg;

  //
  // Discover SDHC through PCIE bridge
  //
  for (Index = 0; Index < ICH_NUMBER_OF_PCIE_BRIDGES; Index++) {
    //
    // Setup appropriate value to PCIE bridge
    //
    Bridge = ICH_PCIE_BRIDGE_ADDRESS + (Index << 8);

    Register = 0;
    PciCfgPpi->Read (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 Bridge | ROOT_BRIDGE_BUS_REGISTER,
                 &Register
                 );
    if (Register == 0xFFFFFFFF) {
      continue;
    }

    //
    // Backup the orignal secondary and subordinary bus number
    //
    OrgSecBusNum = Register;

    //
    // Assign bus number to PCIE bridge
    //
    Bus = (UINT8)(Address >> 24);
    Register = (Bus << 8) + (Bus << 16);
    PciCfgPpi->Write (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 Bridge | ROOT_BRIDGE_BUS_REGISTER,
                 &Register
                 );

    //
    // Discover SDHC. Check the class code 
    //
    Register = 0;
    PciCfgPpi->Read (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 Address | CLASSC_REGISTER,
                 &Register
                 );
    Register &= 0xFFFFFF00;
    if (Register != SDC_CLASSC) {
      //
      // Not found, clear bus number to PCIE bridge
      //
      Register = OrgSecBusNum;
      PciCfgPpi->Write (
                   PeiServices,
                   PciCfgPpi,
                   EfiPeiPciCfgWidthUint32,
                   Bridge | ROOT_BRIDGE_BUS_REGISTER,
                   &Register
                   );
      continue;
    }

    //
    // Check BAR type
    //
    Register = 0;
    PciCfgPpi->Read (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 Address | BASE_ADDRESS_REGISTER_0,
                 &Register
                 );
    BarType = (UINT8)(Register & 0x000000FF);
    if ((BarType & BIT0) != 0) {
      //
      // I/O space, not supported.
      //
      continue;
    } else {
      //
      // Memory space
      //
      if ((BarType & BIT3) == 0) {
        //
        // Assign non prefetchable address range
        //
        Register = ((BaseAddress + 0x00100000) & 0xFFF00000) + ((BaseAddress & 0xFFF00000) >> 16);
        PciCfgPpi->Write (
                     PeiServices,
                     PciCfgPpi,
                     EfiPeiPciCfgWidthUint32,
                     Bridge | ROOT_BRIDGE_ADDRESS_REGISTER,
                     &Register
                     );
      } else {
        //
        // Assign prefetchable address range
        //
        Register = ((BaseAddress + 0x00100000) & 0xFFF00000) + ((BaseAddress & 0xFFF00000) >> 16);
        PciCfgPpi->Write (
                     PeiServices,
                     PciCfgPpi,
                     EfiPeiPciCfgWidthUint32,
                     Bridge | ROOT_BRIDGE_ADDRESS_REGISTER + 4,
                     &Register
                     );
      }
      //
      // Enable root bridge
      //
      Register = 0;
      PciCfgPpi->Read (
                   PeiServices,
                   PciCfgPpi,
                   EfiPeiPciCfgWidthUint16,
                   Bridge | COMMAND_REGISTER,
                   &Register
                   );
      Register |= 0x06;
      PciCfgPpi->Write (
                   PeiServices,
                   PciCfgPpi,
                   EfiPeiPciCfgWidthUint16,
                   Bridge | COMMAND_REGISTER,
                   &Register
                   );      
    }
    *BridgeAddress = Bridge;
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

/**
  Configure SDHC and install PEI_SD_CONTROLLER_PPI.

  @param [in]   PeiServices              Pointer to the PEI Services Table
  @param [in]   Address                  Target SDHC PCI Cfg Adreess
  @param [in]   BaseAddress              Target SDHC Base Address
  @param [in]   BehindBridge             Whether enabling bridge is required
 
  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
STATIC
EFI_STATUS
InstallSdControllerPpi (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       UINT32                     Address,
  IN       UINT32                     BaseAddress,
  IN       BOOLEAN                    BehindBridge
  )
{
  EFI_STATUS                Status;
  ICH_SDHC_DEVICE           *PeiPchSdhcDev;
  EFI_PEI_PCI_CFG2_PPI      *PciCfgPpi;
  UINT32                    Register;
  UINT32                    BridgeAddress;

  PciCfgPpi = (**PeiServices).PciCfg;

  PeiPchSdhcDev = (ICH_SDHC_DEVICE *)AllocatePages (1);
  ASSERT (PeiPchSdhcDev != NULL);
  ZeroMem (PeiPchSdhcDev, sizeof (ICH_SDHC_DEVICE));

  BridgeAddress = 0;
  if (BehindBridge) {
    Status = EnableBridgeConnectToSdhc (PeiServices, Address, BaseAddress, &BridgeAddress);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  }

  //
  // Discover SDHC. Check the class code 
  //
  Register = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint32,
               Address | CLASSC_REGISTER,
               &Register
               );
  Register &= 0xFFFFFF00;
  if (Register != SDC_CLASSC) {
    //
    // SDHC not found.
    //
    return EFI_UNSUPPORTED;
  }

  Register = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint32,
               Address | BASE_ADDRESS_REGISTER_0,
               &Register
               );
  if ((Register & BIT2) == BIT2) {
    //
    // Support mapping 64-bit address space
    // Zero out BAR1. Always assign 32-bit bar address in PEI
    //
    Register = 0;
    PciCfgPpi->Write (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 Address | BASE_ADDRESS_REGISTER_1,
                 &Register
                 );
  }

  //
  // Assign base address register to SDHC
  //
  Register = BaseAddress;
  PciCfgPpi->Write (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint32,
               Address | BASE_ADDRESS_REGISTER_0,
               &Register
               );

  //
  // Enable SDHC
  //
  Register = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint16,
               Address | COMMAND_REGISTER,
               &Register
               );
  Register |= 0x06;
  PciCfgPpi->Write (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint16,
               Address | COMMAND_REGISTER,
               &Register
               );

  PeiPchSdhcDev->Signature            = PEI_ICH_SDHC_SIGNATURE;
  PeiPchSdhcDev->SdControllerPpi      = mSdControllerPpi;
  PeiPchSdhcDev->PpiList              = mPpiList;
  PeiPchSdhcDev->PpiList.Ppi          = &PeiPchSdhcDev->SdControllerPpi;
  PeiPchSdhcDev->NotifyList           = mNotifyList;
  PeiPchSdhcDev->PciCfgPpi            = PciCfgPpi;
  PeiPchSdhcDev->TotalSdControllers   = 1;
  PeiPchSdhcDev->MemBase              = BaseAddress;
  PeiPchSdhcDev->RootBridge           = BridgeAddress;
  PeiPchSdhcDev->PciCfgAddress        = Address;

  //
  // Install SD Controller PPI
  //
  Status = PeiServicesInstallPpi (&PeiPchSdhcDev->PpiList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Install notification in order to reset the SDHC
  //
  Status = PeiServicesNotifyPpi (&PeiPchSdhcDev->NotifyList);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Retrieve SDHC controller information.
 
  @param [in]   PeiServices              Pointer to the PEI Services Table.
  @param [in]   This                     Pointer to PEI_SD_CONTROLLER_PPI
  @param [in]   SdControllerId           SDHC Controller ID
  @param [out]  ControllerType           Result SDHC controller type
  @param [out]  PciCfgAddress            Result SDHC base address
 
  @retval EFI_SUCCESS                    SDHC controller information is retrieved successfully
  @retval EFI_INVALID_PARAMETER          Invalid SdControllerId is given
  @retval Others                         Operation failed

**/
EFI_STATUS
EFIAPI
GetSdhcController (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN       PEI_SD_CONTROLLER_PPI      *This,
  IN       UINT8                      SdControllerId,
  OUT      UINTN                      *ControllerType,
  OUT      UINT64                     *PciCfgAddress
  )
{
  ICH_SDHC_DEVICE         *PeiPchSdhcDev;

  PeiPchSdhcDev = ICH_SDHC_DEVICE_FROM_THIS (This);

  if (SdControllerId >= PeiPchSdhcDev->TotalSdControllers) {
    return EFI_INVALID_PARAMETER;
  }
  *ControllerType = PEI_SDHC_CONTROLLER;
  *PciCfgAddress  = PeiPchSdhcDev->PciCfgAddress;

  return EFI_SUCCESS;
}

/**
  Register notify ppi to reset the SDHC.
 
  @param[in] PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in] NotifyDescriptor  Address of the notification descriptor data structure
  @param[in] Ppi               Address of the PPI that was installed

  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
EFIAPI
EndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN CONST EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN       VOID                       *Ppi
  )
{
  EFI_PEI_PCI_CFG2_PPI        *PciCfgPpi;
  ICH_SDHC_DEVICE             *PeiPchSdhcDev;
  UINT32                      Register;
  UINT32                       *HostResetAddress;

  PeiPchSdhcDev = ICH_SDHC_DEVICE_FROM_NOTIFY_DESC (NotifyDescriptor);
  PciCfgPpi = PeiPchSdhcDev->PciCfgPpi;

  //
  // Reset the HC
  //
  HostResetAddress = (UINT32 *)(PeiPchSdhcDev->MemBase + SOFT_RESET_REGISTER);
  *HostResetAddress |= BIT0;

  MicroSecondDelay (1);

  //
  // Disable SDHC
  //
  Register = 0;
  PciCfgPpi->Read (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint16,
               PeiPchSdhcDev->PciCfgAddress | COMMAND_REGISTER,
               &Register
               );
  Register &= ~0x06;
  PciCfgPpi->Write (
               PeiServices,
               PciCfgPpi,
               EfiPeiPciCfgWidthUint16,
               PeiPchSdhcDev->PciCfgAddress | COMMAND_REGISTER,
               &Register
               );

  if (PeiPchSdhcDev->RootBridge != 0) {
    //
    // Disable root bridge
    //
    Register = 0;
    PciCfgPpi->Read (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint16,
                 PeiPchSdhcDev->RootBridge | COMMAND_REGISTER,
                 &Register
                 );
    Register &= ~0x06;
    PciCfgPpi->Write (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint16,
                 PeiPchSdhcDev->RootBridge | COMMAND_REGISTER,
                 &Register
                 );
    //
    // Clear bus number for root bridge
    //
    Register = 0;
    PciCfgPpi->Write (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 PeiPchSdhcDev->RootBridge | ROOT_BRIDGE_BUS_REGISTER,
                 &Register
                 );
    //
    // Clear address range for root bridge
    //
    Register = 0;
    PciCfgPpi->Write (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 PeiPchSdhcDev->RootBridge | ROOT_BRIDGE_ADDRESS_REGISTER,
                 &Register
                 );
    Register = 0;
    PciCfgPpi->Write (
                 PeiServices,
                 PciCfgPpi,
                 EfiPeiPciCfgWidthUint32,
                 PeiPchSdhcDev->RootBridge | ROOT_BRIDGE_ADDRESS_REGISTER + 4,
                 &Register
                 );
  }
  return EFI_SUCCESS;
}

/**
  Entry point of this module.
  
  @param[in] FileHandle   Handle of the file being invoked
  @param[in] PeiServices  Describes the list of possible PEI Services
 
  @retval EFI_SUCCESS     Operation completed successfully
  @retval Others          Operation failed

**/
EFI_STATUS
PeimInitializePchSdhc (
  IN       EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES           **PeiServices
  )
{
  //
  // EMMC
  //
  InstallSdControllerPpi (PeiServices, EMMC_HC_ADDRESS, EMMC_HC_BASE_ADDRESS, FALSE);
  //
  // SD Card
  //
  InstallSdControllerPpi (PeiServices, SDCARD_HC_ADDRESS, SDCARD_HC_BASE_ADDRESS, FALSE);
  //
  // External SDHC behind bridge
  //
  InstallSdControllerPpi (PeiServices, ADDON_SDHC_ADDRESS, ADDON_SDHC_BASE_ADDRESS, TRUE);

  if (BhtHostPciSupport (PeiServices, ADDON_SDHC_ADDRESS)) {
    EnableBht (PeiServices, ADDON_SDHC_ADDRESS);
  }

  return EFI_SUCCESS;
}

