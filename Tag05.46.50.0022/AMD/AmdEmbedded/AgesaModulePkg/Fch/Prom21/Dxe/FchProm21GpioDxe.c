/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#include <Filecode.h>

#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include <Protocol/FchProm21GpioDxeProtocol.h>
#include <Protocol/FchProm21InitProtocol.h>
#include "FchProm21GpioDxe.h"

#define FILECODE FCH_PROM21_DXE_FCHPROM21GPIODXE_FILECODE

// Write GPIO DXE
EFI_STATUS
EFIAPI
PTGpioWriteDxe (
  IN      UINT8     Pin,
  IN      UINT8     Value
  )
{
  EFI_STATUS        Status;

    Status = EFI_SUCCESS;
//  PT21_INIT_PROTOCOL  *AmdFchInit;
//  UINT32            *GpioMmio = NULL;
//  UINT32            GppPcieAddress;
//  UINT8             GppBus;
//  UINT8             GppDev;
//  UINT8             GppFun;
//  PT_PCI_SAVE_RESTORE_TABLE       PtPciTable;
//
//  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-Start\n", __FUNCTION__));
//
//  // Check if PT GPIO was exist
//  if (PcdGet32 (PcdPT21GpioID) == 0) {
//    DEBUG ((DEBUG_INFO, "  GPIO DXE: PT GPIO is NOT FOUND!\n"));
//    return EFI_NOT_FOUND;
//  }
//
//  Status = gBS->LocateProtocol (
//                  &gAmdFchProm21InitProtocolGuid,
//                  NULL,
//                  &AmdFchInit
//                  );
//  if (EFI_ERROR (Status)) {
//    DEBUG ((DEBUG_INFO, "  GPIO DXE: gAmdFchProm21InitProtocolGuid is NOT FOUND!\n"));
//    return EFI_ABORTED;
//  }
//
//  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
//  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
//  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
//  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);
//
//  // Save/Init UspBus and UspCmd
//  SaveInitPromBus (&PtPciTable, 0x11);
//
//  /////////////////
//  // Write Gpio  //
//  /////////////////
//  GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR);
//
//  if ((*GpioMmio & GpioBit[Pin]) == 0) {
//    // The pin doesn't programmed as output pin
//    Status = RETURN_UNSUPPORTED;
//  } else {
//    GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR + 8);
//    if (Value) {
//      *GpioMmio |= GpioBit[Pin];
//    } else {
//      *GpioMmio &= ~(GpioBit[Pin]);
//    }
//    Status = EFI_SUCCESS;
//  }
//
//  // Restore UspBus and UspCmd
//  RestorePromBus (&PtPciTable, 0x11);
//
//  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-End\n", __FUNCTION__));
  return Status;
}

// Read GPIO PEI
EFI_STATUS
EFIAPI
PTGpioReadDxe (
  IN      UINT8     Pin,
     OUT  UINT8     *Value
  )
{
  EFI_STATUS        Status;

    Status = EFI_SUCCESS;
//  PT21_INIT_PROTOCOL  *AmdFchInit;
//  UINT32            *GpioMmio = NULL;
//  UINT32            GppPcieAddress;
//  UINT8             GppBus;
//  UINT8             GppDev;
//  UINT8             GppFun;
//  PT_PCI_SAVE_RESTORE_TABLE       PtPciTable;
//
//  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-Start\n", __FUNCTION__));
//
//  // Check if PT GPIO was exist
//  if (PcdGet32 (PcdPT21GpioID) == 0) {
//    DEBUG ((DEBUG_INFO, "  GPIO DXE: PT GPIO is NOT FOUND!\n"));
//    return EFI_NOT_FOUND;
//  }
//
//  Status = gBS->LocateProtocol (
//                  &gAmdFchProm21InitProtocolGuid,
//                  NULL,
//                  &AmdFchInit
//                  );
//  if (EFI_ERROR (Status)) {
//    DEBUG ((DEBUG_INFO, "  GPIO DXE: gAmdFchProm21InitProtocolGuid is NOT FOUND!\n"));
//    return EFI_ABORTED;
//  }
//
//  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
//  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
//  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
//  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);
//
//  // Save/Init UspBus and UspCmd
//  SaveInitPromBus (&PtPciTable, 0x11);
//
//  /////////////////
//  // Read  Gpio  //
//  /////////////////
//  GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR);
//
//  if ((*GpioMmio & GpioBit[Pin]) != 0) {
//    // The pin doesn't programmed as input pin
//    Status = RETURN_UNSUPPORTED;
//  } else {
//    GpioMmio = (UINT32 *) (UINTN) (GPIO_IOAPIC_BAR + 4);
//    *Value = (UINT8) ((*GpioMmio & GpioBit[Pin]) >> Pin);
//    Status = EFI_SUCCESS;
//  }
//
//  // Restore UspBus and UspCmd
//  RestorePromBus (&PtPciTable, 0x11);
//
//  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-End\n", __FUNCTION__));
  return Status;
}

VOID
PTGpioInitial (
  IN      VOID      *PTGpioPtr
  )
{
  UINT8             i;
  UINT32            GpioPin;
  UINT32            *GpioMmio;
  UINT32            GpioReg[8];
  Promontory_GPIO_INIT_TABLE      *PTGpioDataPtr;
  Promontory_GPIO_ITEM            *TablePtr;
  Promontory_GPIO_COMMON_SETTING  *CommonTablePtr;

  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-Start\n", __FUNCTION__));

  GpioMmio = NULL;
  SetMem (&GpioReg, sizeof (GpioReg), 0);
  PTGpioDataPtr = (Promontory_GPIO_INIT_TABLE*)PTGpioPtr;

  if (PTGpioDataPtr) {
    TablePtr = &PTGpioDataPtr->GpioList[0];

    while (TablePtr->Pin != 0xFF) {
      // Gpio pin setting
      GpioPin = 1 << (TablePtr->Pin);

      if (TablePtr->Setting.Gpio.OutEnB) {
        GpioReg[0]|= GpioPin;//GpioBit[GpioPin];
      } else {
        GpioReg[0]&= ~GpioPin;//(GpioBit[GpioPin]);
      }

      if (TablePtr->Setting.Gpio.OutEnB) {
        if (TablePtr->Setting.Gpio.Out) {
          GpioReg[2] |=  GpioPin;//GpioBit[GpioPin];
        } else {
          GpioReg[2] &= ~GpioPin;//(GpioBit[GpioPin]);
        }
      }

//      if (TablePtr->Setting.Gpio.interruptEnable) {
//        GpioReg[4] |= GpioBit[GpioPin];
//      } else {
//        GpioReg[4] &= ~(GpioBit[GpioPin]);
//      }
//
//      GpioReg[5] |= (TablePtr->Setting.Gpio.interruptType << (GpioPin * 2));
//      GpioReg[5] |= (TablePtr->Setting.Gpio.interruptLevelTrigType << (GpioPin + 16));
//
//      if (TablePtr->Setting.Gpio.interruptMask) {
//        GpioReg[7] |= GpioBit[GpioPin];
//      } else {
//        GpioReg[7]&= ~(GpioBit[GpioPin]);
//      }

      TablePtr++;
    } // End of while (TablePtr->Pin != 0xFF)

    // Common Pin Setting
    CommonTablePtr = &PTGpioDataPtr->GpioCommon;
//    GpioReg[3] |= CommonTablePtr->Common.GpioDeBounceTimeoutTh;
//    GpioReg[3] |= (CommonTablePtr->Common.GpioDeBounceTimer << 3);
//    GpioReg[5] |= (CommonTablePtr->Common.GPIOInterruptMode << 24);
//    GpioReg[5] |= (CommonTablePtr->Common.GpioInterruptActLevel << 25);
//    GpioReg[5] |= (CommonTablePtr->Common.GpioInterruptOutputEn << 26);
  } // End of if (PTGpioDataPtr)

  // Write GPIO setting to GPIO register.
  for (i = 0; i < 8; i++) {
    GpioMmio = (UINT32 *)(UINTN)(GPIO_IOAPIC_BAR + (i * 4));
    *GpioMmio = GpioReg[i];
  }

  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-End\n", __FUNCTION__));
}

// Initial GPIO PEI
EFI_STATUS
EFIAPI
PTGpioInitialDxe (
  IN      VOID      *PTGpioPtr
  )
{
  EFI_STATUS        Status;
  PT21_INIT_PROTOCOL  *AmdFchInit;
  UINT32            GppPcieAddress;
  UINT8             GppBus;
  UINT8             GppDev;
  UINT8             GppFun;
  PT_PCI_SAVE_RESTORE_TABLE       PtPciTable;
  Promontory_GPIO_INIT_TABLE      *PTGpioDataPtr;

  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-Start\n", __FUNCTION__));

  // Check if PT GPIO was exist
  if (PcdGet32 (PcdPT21GpioID) == 0) {
    DEBUG ((DEBUG_INFO, "  GPIO DXE: PT GPIO is NOT FOUND!\n"));
    return EFI_NOT_FOUND;
  }

  Status = gBS->LocateProtocol (
                  &gAmdFchProm21InitProtocolGuid,
                  NULL,
                  &AmdFchInit
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "  GPIO DXE: gAmdFchProm21InitProtocolGuid is NOT FOUND!\n"));
    return EFI_ABORTED;
  }

  PTGpioDataPtr = (Promontory_GPIO_INIT_TABLE*)PTGpioPtr;
  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);

  // Save/Init UspBus and UspCmd
  SaveInitPromBus (&PtPciTable, 0x11);

  /////////////////
  // Program Gpio  //
  /////////////////
  PTGpioInitial (PTGpioDataPtr);

  // Restore UspBus and UspCmd
  RestorePromBus (&PtPciTable, 0x11);

  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchProm21GpioDxeInit
 *
 * Description
 *   Entry point of the AMD FCH PROMONTORY GPIO DXE driver
 *   Perform the configuration init, resource reservation, early post init
 *   and install all the supported protocol
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchProm21GpioDxeInit (
  IN      EFI_HANDLE          ImageHandle,
  IN      EFI_SYSTEM_TABLE    *SystemTable
  )
{
  PT_DXE_PRIVATE    *PtPrivate;
  EFI_HANDLE        Handle;
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-Start\n", __FUNCTION__));

  // Initialize the configuration structure and private data area
  //
  // Allocate memory for the private data
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  sizeof (PT_DXE_PRIVATE),
                  &PtPrivate
                  );
  DEBUG ((DEBUG_INFO, "  AllocatePool EfiACPIMemoryNVS Size = 0x%X\n", sizeof (PT_DXE_PRIVATE)));
  ASSERT_EFI_ERROR (Status);

  // Initialize the private data structure
  PtPrivate->Signature = PT_DXE_PRIVATE_DATA_SIGNATURE;
  PtPrivate->PtGpioProtocol.PTInitiialGpio = PTGpioInitialDxe;
  PtPrivate->PtGpioProtocol.PTWriteGpio = PTGpioWriteDxe;
  PtPrivate->PtGpioProtocol.PTReadGpio = PTGpioReadDxe;

  //
  // Publish the PtInit protocol
  //
  Handle = ImageHandle;
  DEBUG ((DEBUG_INFO, "  InstallProtocolInterface gEfiPromontoryGpioProtocolGuid\n"));
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiPromontoryGpioProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &PtPrivate->PtGpioProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a(%d)-End-ERROR: Install gEfiPromontoryGpioProtocolGuid FAIL!\n", __FUNCTION__, __LINE__));
    return (Status);
  }

  DEBUG ((DEBUG_INFO, "PT-GPIODXE-%a-End\n", __FUNCTION__));
  Status = EFI_SUCCESS;
  return Status;
}

