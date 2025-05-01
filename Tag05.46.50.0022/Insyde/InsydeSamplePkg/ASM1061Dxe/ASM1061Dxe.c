/** @file
  ASM1061Dxe driver will set ASM1061 related PCI setting for compatibility concerns

;******************************************************************************
;* Copyright (c) 2016, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PciExpressLib.h>
#include <Library/PcdLib.h>
#include <Library/VariableLib.h>
#include <Library/TimerLib.h>

#include <Protocol/PlatformPhaseNotify.h>


#define PrimayBusNumber          0x1c
#define PrimaryDeviceNumber      0x02
#define LINK_CAPABILTIES_REG     0x0c
#define Device_Control_REG       0x08
#define PCI_CAPBILITY_POINTER_OFFSET 0x34
#define EFI_PCI_CAPABILITY_ID_PCIEXP 0x10

#pragma optimize("", off)

/**
  A function to search the ASPM register location (offset) by using the capability register
  to trace and identify if the capability ID match 0x10.

  @param  Bus       The PCIE bus number of the searching device
  @param  Dev       The PCIE device number of the searching device
  @param  Fun       The PCIE function number of the searching device

  @retval 0         No PCIExpress capability register
  @retval Register  The PCIExpress capability register offset of given PCIE device
**/
UINT16
FindPCIECapOffset (
  IN UINT8     Bus,
  IN UINT8     Dev,
  IN UINT8     Fun
  )
{
  UINT16      Reg;
  UINT16      Cap;
  UINT16      Data16;

  Data16 = PciExpressRead16 (PCI_EXPRESS_LIB_ADDRESS (Bus, Dev, Fun, PCI_CAPBILITY_POINTER_OFFSET));

  if (((Data16 & 0x00FF) == 0x00) || ((Data16  & 0x00FF) == 0xFF)) {
    return 0x00;
  } else {
    Data16 &= 0x00FF;
    Data16 <<= 8;
  }

  do {
    Reg = (Data16 >> 8);
    Data16 = PciExpressRead16 (PCI_EXPRESS_LIB_ADDRESS (Bus, Dev, Fun, Reg));
    Cap = (Data16 & 0x00FF);
  } while ((Cap != EFI_PCI_CAPABILITY_ID_PCIEXP) && ( (Data16 >> 8) != 0x00));

  if (Cap != EFI_PCI_CAPABILITY_ID_PCIEXP) {
    return 0x00;
  } else {
    return Reg;
  }
}



STATIC
VOID
EFIAPI
ASM1061NotifyFunction (
  IN     EFI_EVENT                      Event,
  IN     VOID                           *Context
  )
{
  UINT32                          SubClassCode;
  UINT64                          BootScriptPciAddress;
  UINT8                           Register;
  UINT32                          *Bus;
  UINT8                           RevID;
  UINT16                          LINKCapOffset;
  UINT16                          LINKCap;
  UINT8                           Data;
  UINT8                           DataMask;
  EFI_STATUS                      Status;


  Bus = (UINT32*) PcdGetPtr (PcdAhciPcieBridgeAddress);

  //
  // PrimayBusNumber and PrimaryDeviceNumber are varied with different projects
  //
  LINKCapOffset = FindPCIECapOffset(PrimayBusNumber, PrimaryDeviceNumber, 0);

  //
  // below code follow ASM1061 A3 system BIOS Programming Supplement v1.0
  //
  RevID = PciExpressRead8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0x08));
  
  if (RevID >= 0x02) {
    //
    // recommended set to 1 to improve S4 wake-up time on Microsoft Window 8 operating system.
    //
    // 
    PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xfd), BIT0);
  
  
    //
    //Bit 7 is recommended to set to satisfy AHCI specification.
    //Bit 6 is a reserved bit. Don't set this bit.
    //Bit 5 is recommended to set to support ATAPI device that capacity is greater than 800 GB
    //Bit 4 is recommended to set to support ATA and ATAPI device are connected on ASM1061
    //Bit 3, 2 can be set to disable SATA 6G support. It is not recommended to disable SATA 6G
    //
    PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xfe), BIT7 | BIT5 | BIT4);


    if (LINKCapOffset > 0) {
      LINKCap = PciExpressRead16(LINKCapOffset + LINK_CAPABILTIES_REG);
      if (LINKCap && 0x0800)
        //
        //Bit 1, 0 are recommended to set if PCIE ASPM L1 is supported on platform, 
        //
        PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xfe), 0x03);
    }
      
    //
    //    Bit 7 is recommended to set to satisfy AHCI specification.
    //    Bit 6 can be set on ASM1061 A3 only. Don't set this bit on A2 or previous version.
    //    Bit 5, 4, 3, 2 are reserved. Don't program these bit
    //    Bit 1, 0 can be set to modify AHCI PxCMD.ESP and HPCP bit. Please refer to below
    //
    PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xff), BIT7 | BIT6);

    //
    //AHCI specification defines that CAP.SXS and PxCMD.HPCP are read
    //only bits. They can't be modified by software. ASM106x provides other PCI
    //registers to modify these AHCI read only bits - RxE5C[5] for modifying
    //CAP.SXS, RxFF[0] for modifying Port 0 PxCMD.HCPC and RxFF[1] for
    //modifying Port 1 PxCMD.HCPC.
    //

    //
    // To disable eSATA and hot-plug for AHCI Mode, please set 
    //
    PciExpressAnd8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xE5C), (UINT8)(~BIT5));
    PciExpressAnd8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xff), (UINT8)(~BIT0));
    PciExpressAnd8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xff), (UINT8)(~BIT1));

    
  }


  
  //
  // 2. Set PCI Subclass Code for SATA Controller
  //
  SubClassCode = PciExpressRead8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0x0A));
  if (SubClassCode == 0x01) {
    //
    // enable subclass to be programmed
    //
    PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xec), BIT2);
    BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xec);
    Data = BIT2;
    DataMask = (UINT8)(~BIT2);
    Status = S3BootScriptSavePciCfgReadWrite(
      S3BootScriptWidthUint8, 
      BootScriptPciAddress, 
      &Data, 
      &DataMask);
    
    //
    // Set Subclass Code
    //
    PciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xa), 0x06);
    BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xa);
    Register             = 0x6;
    Status = S3BootScriptSavePciCfgWrite (
      S3BootScriptWidthUint8,
      BootScriptPciAddress,
      1,
      &Register
      );

    //
    // disable subclass to be programmed
    //
    PciExpressAnd8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xec), (UINT8)(~BIT2));
    BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xec);
    Data = 0;
    DataMask = (UINT8)(~BIT2);
    Status = S3BootScriptSavePciCfgReadWrite(
      S3BootScriptWidthUint8, 
      BootScriptPciAddress, 
      &Data, 
      &DataMask);



  }


  //
  // SATA Optimal Setting for Devices Compatibility
  // all should be set before Spin-up and PCI resources assigned
  //
  
  PciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xcae), 0x92);
  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xcae);
  Register             = 0x92;
  Status = S3BootScriptSavePciCfgWrite (
           S3BootScriptWidthUint8,
           BootScriptPciAddress,
           1,
           &Register
           );
  
  PciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xda5), 0x26);
  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xda5);
  Register             = 0x26;
  Status = S3BootScriptSavePciCfgWrite (
           S3BootScriptWidthUint8,
           BootScriptPciAddress,
           1,
           &Register
           );
  
  PciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xdae), 0x92);
  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xdae);
  Register             = 0x92;
  Status = S3BootScriptSavePciCfgWrite (
           S3BootScriptWidthUint8,
           BootScriptPciAddress,
           1,
           &Register
           );
  
  PciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(0x01, 0, 0, 0xca4), 0xab);
  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xca4);
  Register             = 0xab;
  Status = S3BootScriptSavePciCfgWrite (
           S3BootScriptWidthUint8,
           BootScriptPciAddress,
           1,
           &Register
           );
  
  PciExpressWrite8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xda4), 0xab);
  BootScriptPciAddress = S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (Bus[0], 0, 0, 0xda4);
  Register             = 0xab;
  Status = S3BootScriptSavePciCfgWrite (
           S3BootScriptWidthUint8,
           BootScriptPciAddress,
           1,
           &Register
           );

  //
  // L0s state must be disabled in ASM106x for PCIE compatibility. Set
  //PCI RxA01[0] = 0 to disable ASM106x PCIE L0s active state power
  //management.
  //
//  PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xa01), BIT0);
  

  //
  //Microsoft WHCK will check the L0s supported bit in PCIE Link
  //Capabilities Register (offset 0x0C bit 10), PCI RxA01[0] must be set 1 to
  //pass WHCK test.  
  //
  PciExpressAnd8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xa01), (UINT8)(~BIT1));
  Data = 0;
  DataMask = (UINT8)(~BIT1);
  Status =  S3BootScriptSavePciCfgReadWrite(
    S3BootScriptWidthUint8, 
    BootScriptPciAddress, 
    &Data, 
    &DataMask);
  

  //
  // Internal Regulator Setting
  //
  PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xa12), BIT4|BIT3);
  Data = BIT4|BIT3;
  DataMask = 0Xff;
  Status = S3BootScriptSavePciCfgReadWrite(
    S3BootScriptWidthUint8, 
    BootScriptPciAddress, 
    &Data, 
    &DataMask);
  

  //
  //S4 wake up improved
  //
  PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xfd), BIT0);
  Data = BIT0;
  DataMask = 0Xff;
  Status = S3BootScriptSavePciCfgReadWrite(
    S3BootScriptWidthUint8, 
    BootScriptPciAddress, 
    &Data, 
    &DataMask);

  //
  //SSC mode set to 0~-1250ppm can reduce ElectroMagnetic Interference
  //(EMI). The SSC enable bit must be delayed at least 100 ns to set after other
  //bits are set
  //

  //
  //  1. Set RxA10[4:2] = 2'011b, SSC mode 0~-1250ppm, 1:1 modulation
  //
  PciExpressAndThenOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xa10), (UINT8)(~BIT4), BIT2 | BIT3);

  //
  //  2. Delay 100 ns at least
  //
  NanoSecondDelay(100);

  //
  //3. Set RxA10[0] = 1, SSC enable
  //
  PciExpressOr8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0xa10), BIT0);

  //
  //15. PCIE Max Payload Size Compatibility
  //strong recommend that don't set the Max Payload Size and remain
  //the default value 3'000b for ASM1061.
  //
  if (PciExpressRead8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0x88) && 0xE0) > 0) {
    PciExpressAnd8(PciExpressRead8(PCI_EXPRESS_LIB_ADDRESS(Bus[0], 0, 0, 0x88), 0x1f);
  }
  
  gBS->CloseEvent (Event);


}




EFI_STATUS
EFIAPI
ASM1061DxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     Event;
  VOID          *Registration;

  //
  // Registers the gEfiPciEnumerationCompleteProtocolGuid protocol notification for PCIe port switching
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ASM1061NotifyFunction,
                  NULL,
                  &Event
                  );
  if (Status == EFI_SUCCESS) {
    Status = gBS->RegisterProtocolNotify (
                    &gEfiPciHostBridgeEndResourceAllocationNotifyGuid,
                    Event,
                    &Registration
                    );
  }
  return Status;
}
#pragma optimize("", on)
