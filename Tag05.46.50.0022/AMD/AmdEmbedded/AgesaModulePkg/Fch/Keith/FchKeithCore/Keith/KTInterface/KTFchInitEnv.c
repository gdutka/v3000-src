/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/

#include "FchPlatform.h"
#include "KTFchTaskLauncher.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTINTERFACE_KTFCHINITENV_FILECODE

extern FCH_INTERFACE     FchInterfaceDefault;

FCH_TASK_ENTRY *FchInitEnvTasksTable[] = {
        &FchInitEnvHwAcpiP,
        &FchInitEnvHwAcpi,
        &FchInitEnvLpc,
        &FchInitEnvSpi,
        &FchInitEnvSd,
        &FchInitEnvUsbXhci,
        &FchInitEnvSata,
        &FchInitEnvAb,
        &FchInitEnvUsb,
        &FchInitEnvXGbe,
        NULL
};

AGESA_STATUS
FchInitEnv (
  IN       FCH_DATA_BLOCK            *FchDataBlock
  );

/**
 * FchInitEnv - Config Fch before PCI emulation
 *
 *
 *
 * @param[in] FchDataBlock Fch configuration structure pointer
 *
 */
AGESA_STATUS
FchInitEnv (
  IN       FCH_DATA_BLOCK            *FchDataBlock
  )
{
  AGESA_STATUS        Status;
  //EMBDEV-16636_Start
  UINT32                 PortsPropertyReg;
  UINT32                 GpioValue;
  UINT32                 GpioAddress;
  //EMBDEV-16636_End

  // Override internal data with IDS (Optional, internal build only)
  //IDS_OPTION_CALLOUT (IDS_CALLOUT_FCH_INIT_ENV, FchParams, FchParams->StdHeader);

  //AgesaFchOemCallout (FchParams);
  Status = FchTaskLauncher (&FchInitEnvTasksTable[0], FchDataBlock, TpFchInitEnvDispatching);

  //EMBDEV-16636_Start
  GpioAddress = ACPI_MMIO_BASE + GPIO_BANK0_BASE + (PcdGet8(PcdXgbePort0Led1Gpio)*4);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT23), BIT23); //OutputEnable : 0-Output is disabled on the pin, 1-Output is enabled on the pin
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT22), 0);     //OutputValue  : 0-low, 1-high

  GpioAddress = ACPI_MMIO_BASE + GPIO_BANK0_BASE + (PcdGet8(PcdXgbePort0Led2Gpio)*4);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT23), BIT23);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT22), 0);

  GpioAddress = ACPI_MMIO_BASE + GPIO_BANK0_BASE + (PcdGet8(PcdXgbePort1Led1Gpio)*4);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT23), BIT23);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT22), 0);

  GpioAddress = ACPI_MMIO_BASE + GPIO_BANK0_BASE + (PcdGet8(PcdXgbePort1Led2Gpio)*4);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT23), BIT23);
  RwMem (GpioAddress,AccessWidth32, ~(UINT32)(BIT22), 0);

  //Writing GPIO numbers to Port Property register
  GpioValue   = (UINT32) PcdGet8(PcdXgbePort0Led2Gpio); //Default 0x5B(AGPIO091)
  GpioValue   = (GpioValue << 8) | ((UINT32)PcdGet8(PcdXgbePort0Led1Gpio)); //Default 0x0A(AGPIO010)
  FchSmnRead (0, FCH_KT_SMN_XGBE_MMIO0_BASE + 0x814, &PortsPropertyReg, NULL);
  PortsPropertyReg  = (PortsPropertyReg & 0xFFFF0000) | GpioValue;
  FchSmnWrite (0, FCH_KT_SMN_XGBE_MMIO0_BASE + 0x814, &PortsPropertyReg, NULL);

  GpioValue   = (UINT32) PcdGet8(PcdXgbePort1Led2Gpio);// Default 0x9D(AGPIO157)
  GpioValue   = (GpioValue << 8) | ((UINT32)PcdGet8(PcdXgbePort1Led1Gpio)); // Default 0x90(AGPIO144)
  FchSmnRead (0, FCH_KT_SMN_XGBE_MMIO0_BASE + 0x834, &PortsPropertyReg, NULL);
  PortsPropertyReg  = (PortsPropertyReg & 0xFFFF0000) | GpioValue;
  FchSmnWrite (0, FCH_KT_SMN_XGBE_MMIO0_BASE + 0x834, &PortsPropertyReg, NULL);
  //EMBDEV-16636_End

  IDS_HDT_CONSOLE (FCH_TRACE, "  FchInitEnv Exit... Status = [0x%x]\n", Status);
  return Status;
}



