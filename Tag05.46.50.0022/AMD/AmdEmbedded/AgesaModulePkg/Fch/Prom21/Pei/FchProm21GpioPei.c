/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Library/DebugLib.h>

#include <Filecode.h>

#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include <Ppi/FchProm21GpioPpi.h>
#include <Ppi/FchProm21InitPpi.h>
#include "FchProm21GpioPei.h"

#define FILECODE FCH_PROM21_PEI_FCHPROM21GPIOPEI_FILECODE

// Write GPIO PEI
EFI_STATUS
EFIAPI
PTGpioWritePei (
  IN      EFI_PEI_SERVICES    **PeiServices,
  IN      UINT8     Pin,
  IN      UINT8     Value
  )
{
  EFI_STATUS        Status;

    Status = EFI_SUCCESS;

  return Status;
}
//{
//  EFI_STATUS        Status;
//  UINT32            *GpioMmio = NULL ;
//  UINT32            Data32;
//  UINT32            GpioPin;
//  FCH_PROM21_INIT_PPI         *FchProm21InitPpi;
//  PT_PCI_SAVE_RESTORE_TABLE   PtPciTable;
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-Start\n", __FUNCTION__));
//
//  GpioPin = 1 << (TablePtr->Pin);
//
//  if (PcdGet32 (PcdPT21GpioID) == 0) {
//    DEBUG ((DEBUG_INFO, "  GPIO PEI: PT GPIO is NOT FOUND!\n"));
//    return EFI_NOT_FOUND;
//  }
//
//  Status = (*PeiServices)->LocatePpi (
//                               PeiServices,
//                               &gAmdFchProm21InitPpiGuid,
//                               0,
//                               NULL,
//                               &FchProm21InitPpi
//                               );
//  ASSERT_EFI_ERROR ( Status);
//
//  // Save/Init UspBus and UspCmd
//  SaveInitPromBus (&PtPciTable, 0x11);
//
//  // Read the PT PCIE USP to see if GPIO device exists
//  if (PtPciRead32 (PtPciTable.PromBus, 0, 0, 0) != PT21_USP_ID) {
//    // Restore UspBus and UspCmd
//    RestorePromBus (&PtPciTable, 0x11);
//    DEBUG ((EFI_D_ERROR, "  PT GPIO PEI: GPIO is NOT FOUND!\n"));
//    return EFI_NOT_FOUND;
//  }
//
//  /////////////////
//  // Write Gpio  //
//  /////////////////
//  GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR);
//  Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR);
//
////  if ((*GpioMmio & GpioBit[Pin]) == 0) {
//  if ((Data32 & GpioBit[Pin]) == 0) {
//    // The pin doesn't programmed as Output Pin
//    DEBUG ((DEBUG_INFO, "  Pin %d is not programmed as Output pin\n", Pin));
//    Status = RETURN_UNSUPPORTED;
//  } else {
//    GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR + 8);
//    if (Value) {
//      DEBUG ((DEBUG_INFO, "  Program Pin %d as Output High\n", Pin));
////      *GpioMmio |= GpioBit[Pin];
//      Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR + 8);
//      Data32 |= GpioBit[Pin];
//      PtMmioWrite32 (GPIO_IOAPIC_BAR + 8, Data32);
//      Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR + 8);
//    } else {
//      DEBUG ((DEBUG_INFO, "  Program Pin %d as Output Low\n", Pin));
////      *GpioMmio &= ~(GpioBit[Pin]);
//      Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR + 8);
//      Data32 &= ~(GpioBit[Pin]);
//      PtMmioWrite32 (GPIO_IOAPIC_BAR + 8, Data32);
//      Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR + 8);
//    }
//    Status = EFI_SUCCESS;
//  }
//
//  // Restore UspBus and UspCmd
//  RestorePromBus (&PtPciTable, 0x11);
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-End\n", __FUNCTION__));
//  return Status;
//}

// Read GPIO PEI
EFI_STATUS
EFIAPI
PTGpioReadPei (
  IN      EFI_PEI_SERVICES    **PeiServices,
  IN      UINT8     Pin,
     OUT  UINT8     *Value
  )
{
  EFI_STATUS        Status;

    Status = EFI_SUCCESS;

  return Status;
}
//{
//  EFI_STATUS        Status;
//  UINT32            *GpioMmio = NULL;
//  UINT32            Data32;
//  FCH_PROM21_INIT_PPI         *FchProm21InitPpi;
//  PT_PCI_SAVE_RESTORE_TABLE   PtPciTable;
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-Start\n", __FUNCTION__));
//
//  if (PcdGet32 (PcdPT21GpioID) == 0) {
//    DEBUG ((DEBUG_INFO, "  GPIO PEI: PT GPIO is NOT FOUND!\n"));
//    return EFI_NOT_FOUND;
//  }
//
//  Status = (*PeiServices)->LocatePpi (
//                                PeiServices,
//                                &gAmdFchProm21InitPpiGuid,
//                                0,
//                                NULL,
//                                &FchProm21InitPpi
//                                );
//  ASSERT_EFI_ERROR (Status);
//
//  // Save/Init UspBus and UspCmd
//  SaveInitPromBus (&PtPciTable, 0x11);
//
//  // Read the PT PCIE USP to see if GPIO device exists
//  if (PtPciRead32 (PtPciTable.PromBus, 0, 0, 0) != PT21_USP_ID) {
//    // Restore UspBus and UspCmd
//    RestorePromBus (&PtPciTable, 0x11);
//    DEBUG ((EFI_D_ERROR, "PT GPIO PEI: PT GPIO device NOT FOUND!\n"));
//    return EFI_NOT_FOUND;
//  }
//
//  /////////////////
//  // Read  Gpio  //
//  /////////////////
//  GpioMmio = (UINT32 *)(UINTN)(GPIO_IOAPIC_BAR);
//  Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR);
//
////  if ((*GpioMmio & GpioBit[Pin]) != 0) {
//  if ((Data32 & GpioBit[Pin]) != 0) {
//    // The pin doesn't programmed as input Pin
//    DEBUG ((DEBUG_INFO, "  Pin %d is not programmed as Input pin\n", Pin));
//    Status = RETURN_UNSUPPORTED;
//  } else {
//    GpioMmio = (UINT32 *)(UINTN)(GPIO_IOAPIC_BAR + 4);
////    *Value= (UINT8)((*GpioMmio & GpioBit[Pin]) >> Pin);
//    Data32 = PtMmioRead32 (GPIO_IOAPIC_BAR + 4);
//    *Value = (UINT8)((Data32 & GpioBit[Pin]) >> Pin);
//    DEBUG ((DEBUG_INFO, "  Pin %d Status is %d\n", Pin, *Value));
//
//    Status = EFI_SUCCESS;
//  }
//
//  // Restore UspBus and UspCmd
//  RestorePromBus (&PtPciTable, 0x11);
//
//  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-End\n", __FUNCTION__));
//  return Status;
//}

VOID
PTGpioInitial (
  IN      VOID      *PTGpioPtr
  )
{
  UINT32            GpioPin;
  UINT16            MaxGpioPin = PROM21_MAX_GPIO_PIN_NUMBER;
  PT_GPIO_REG       GpioReg;
  PT_GPIO_REG       GpioRegData;

  Promontory_GPIO_INIT_TABLE      *PTGpioDataPtr;
  Promontory_GPIO_ITEM            *TablePtr;
  Promontory_GPIO_COMMON_SETTING  *CommonTablePtr;

  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-Start\n", __FUNCTION__));

  GpioReg.GpioPinDir = 0;
  GpioReg.GpioInputData = 0;
  GpioReg.GpioOutputData = 0;

  PTGpioDataPtr = (Promontory_GPIO_INIT_TABLE*)PTGpioPtr;

  if (PTGpioDataPtr) {
    TablePtr = &PTGpioDataPtr->GpioList[0];

    while ((TablePtr->Pin != 0xFF) && (TablePtr->Pin <= MaxGpioPin)) {
      // Gpio pin setting
      GpioPin = 1 << (TablePtr->Pin);
      DEBUG ((DEBUG_INFO, "PT GPIO Pin Setting = %d (0x%x)\n", TablePtr->Pin, GpioPin));
      if (TablePtr->Setting.Gpio.OutEnB) {
        GpioReg.GpioPinDir |= GpioPin;
        if (TablePtr->Setting.Gpio.Out) {
          GpioReg.GpioOutputData |= GpioPin;
        } else {
          GpioReg.GpioOutputData &= ~GpioPin;
      }
      } else {
        GpioReg.GpioPinDir &= ~GpioPin;
      }

      TablePtr++;
    }

    // Common Pin Setting
    CommonTablePtr = &PTGpioDataPtr->GpioCommon;
    DEBUG ((DEBUG_INFO, "PT GPIO Pin Setting = %x, %x\n", GpioReg.GpioPinDir, GpioReg.GpioOutputData));
  // Write GPIO setting to GPIO register.
    GpioRegData.GpioPinDir = PtMmioRead32 (GPIO_IOAPIC_BAR);
    PtMmioWrite32 (GPIO_IOAPIC_BAR, GpioReg.GpioPinDir);

    GpioRegData.GpioInputData = PtMmioRead32 (GPIO_IOAPIC_BAR + 0x4);

    GpioRegData.GpioOutputData = PtMmioRead32 (GPIO_IOAPIC_BAR + 0x8);
    PtMmioWrite32 (GPIO_IOAPIC_BAR + 0x8, GpioReg.GpioOutputData);
  }

  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-End\n", __FUNCTION__));
}

// Initial GPIO PEI
EFI_STATUS
EFIAPI
PTGpioInitialPei (
  IN      EFI_PEI_SERVICES    **PeiServices,
  IN      VOID                *PTGpioPtr
  )
{
  FCH_PROM21_INIT_PPI         *FchProm21InitPpi;
  Promontory_GPIO_INIT_TABLE  *PTGpioDataPtr;
  EFI_STATUS        Status;
  UINT32            Value32;
  UINT32            IohcBridgeCntlSmnReg;
  UINT32            GppPcieAddress;
  PT_PCI_SAVE_RESTORE_TABLE       PtPciTable;

  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-Start\n", __FUNCTION__));

  if (PcdGet32 (PcdPT21GpioID) == 0) {
    DEBUG ((DEBUG_INFO, "  GPIO PEI: PT GPIO is NOT FOUND!\n"));
    return EFI_NOT_FOUND;
  }

  PTGpioDataPtr = (Promontory_GPIO_INIT_TABLE*)PTGpioPtr;
  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);

  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdFchProm21InitPpiGuid,
                             0,
                             NULL,
                             &FchProm21InitPpi
                             );
  ASSERT_EFI_ERROR ( Status);

  // Save/Init UspBus and UspCmd
  SaveInitPromBus (&PtPciTable, 0x11);

  // Read the PT PCIE USP to see if GPIO device exists
  if (PtPciRead32 (PtPciTable.PromBus, 0, 0, 0) != PT21_USP_ID) {
    // Restore UspBus and UspCmd
    RestorePromBus (&PtPciTable, 0x11);
    DEBUG ((EFI_D_ERROR, "PT GPIO PEI: PT GPIO device NOT FOUND!\n"));
    return EFI_NOT_FOUND;
  }

  ////
  // Assign resources for GPIO device
  ////
  // 1. Defined  IoApic base address as 0xFEC30000 for GPP which connected to GPIO device
  // Use range whcih defined in GPIO_IOAPIC_BAR
  IohcBridgeCntlSmnReg = PcdGet32 (PcdPT21IohcBridgeCntl);
  PtPciWrite32 (0, 0, 0, 0x60, IohcBridgeCntlSmnReg);
  Value32 = PtPciRead32 (0, 0, 0, 0x64);
  Value32 &= 0x007FFFFF;
  Value32 |= ((((GPIO_IOAPIC_BAR >> 12) & 0xFF) << 24) + (1 << 23));
  PtPciWrite32 (0, 0, 0, 0x64, Value32);

  // 2. Set 0xFEC30000 in Promontory PCIE device pci offset 0x40 ~ 47
  Value32 = GPIO_IOAPIC_BAR;
  PtPciWrite32 (PtPciTable.PromBus, 0, 0, 0x40, Value32);
  Value32 = 0x0;
  PtPciWrite32 (PtPciTable.PromBus, 0, 0, 0x44, Value32);

  //3. Set bit 0 as 1 in Promontory PCIE device pci offset 0x48
  Value32 = PtPciRead32 (PtPciTable.PromBus, 0, 0, 0x48);
  Value32 |= 0x01;
  PtPciWrite32 (PtPciTable.PromBus, 0, 0, 0x48, Value32);

  //////////////////
  // Program Gpio //
  //////////////////
  PTGpioInitial (PTGpioDataPtr);

  // Restore UspBus and UspCmd
  RestorePromBus (&PtPciTable, 0x11);

  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/*********************************************************************************
 * Name: FchProm21GpioPeiInit
 *
 * Description:
 *
 * Input
 *   FfsHeader   : pointer to the firmware file system header
 *   PeiServices : pointer to the PEI service table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchProm21GpioPeiInit (
  IN      EFI_PEI_FILE_HANDLE           FileHandle,
  IN      CONST EFI_PEI_SERVICES        **PeiServices
  )
{
  EFI_STATUS                  Status;
  PT_PEI_PRIVATE              *PtPrivate;
  EFI_PEI_PPI_DESCRIPTOR      *PpiListPtGpio;

  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-Start\n", __FUNCTION__));

  if (PcdGet32 (PcdPT21XhciID) == 0) {
    DEBUG ((EFI_D_ERROR, "PT-GPIOPEI-%a-End !! Promontory is NOT FOUND !!\n"));
    return EFI_SUCCESS;
  }

  //
  // Create the PPIs
  //
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (PT_PEI_PRIVATE),
                             &PtPrivate
                             );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  PtPrivate->Signature = PT_PEI_PRIVATE_DATA_SIGNATURE;
  PtPrivate->PtGpioPpi.PTInitiialGpio = PTGpioInitialPei;
  PtPrivate->PtGpioPpi.PTWriteGpio = PTGpioWritePei;
  PtPrivate->PtGpioPpi.PTReadGpio = PTGpioReadPei;

  //
  // Install the PPIs
  //
  Status = (*PeiServices)->AllocatePool (
                               PeiServices,
                               sizeof (EFI_PEI_PPI_DESCRIPTOR),
                               &PpiListPtGpio
                               );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Create the PPI descriptor
  //
  PpiListPtGpio->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListPtGpio->Guid  = &gEfiPeiPromotoryGpioPpiGuid;
  PpiListPtGpio->Ppi   = &PtPrivate->PtGpioPpi;

  //
  // Publish the PPI
  //
  Status = (*PeiServices)->InstallPpi (
                               PeiServices,
                               PpiListPtGpio
                               );

  DEBUG ((DEBUG_INFO, "PT-GPIOPEI-%a-End\n", __FUNCTION__));
  return Status;
}

