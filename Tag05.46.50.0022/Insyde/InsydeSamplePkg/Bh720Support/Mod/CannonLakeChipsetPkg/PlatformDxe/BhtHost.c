/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include "BhtHost.h"

int           g_deviceId        = 0;
BOOLEAN       mRegistered       = FALSE;
EFI_HANDLE    mControllerHandle = NULL;

/**
  BH EMMC init done handler.

  @param[in]  Event        Event whose notification function is being invoked
  @param[in]  Context      Pointer to the notification function's context

**/
VOID
EFIAPI
BhtHostInitializeDoneHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS    Status;
  VOID          *Interface;

  Status = gBS->HandleProtocol (
                mControllerHandle,
                &gEfiSdMmcPassThruProtocolGuid,
                (VOID **) &Interface
                );
  if (EFI_ERROR (Status)) {
    return;
  }

  gBS->CloseEvent (Event);
  //
  // Stall for stable.
  //
  gBS->Stall (6000000);
}

/**
  Read/Write specified SD/MMC host controller mmio register.

  @param[in]      PciIo        The PCI IO protocol instance.
  @param[in]      BarIndex     The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in]      Offset       The offset within the selected BAR to start the
                               memory operation.
  @param[in]      Read         A boolean to indicate it's read or write operation.
  @param[in]      Count        The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in, out] Data         For read operations, the destination buffer to store
                               the results. For write operations, the source buffer
                               to write data from. The caller is responsible for
                               having ownership of the data buffer and ensuring its
                               size not less than Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or Data is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The read/write operation succeeds.
  @retval Others                The read/write operation fails.

**/
EFI_STATUS
EFIAPI
SdMmcHcRwMmio (
  IN     EFI_PCI_IO_PROTOCOL   *PciIo,
  IN     UINT8                 BarIndex,
  IN     UINT32                Offset,
  IN     BOOLEAN               Read,
  IN     UINT8                 Count,
  IN OUT VOID                  *Data
  )
{
  EFI_STATUS                   Status;

  if ((PciIo == NULL) || (Data == NULL))  {
    return EFI_INVALID_PARAMETER;
  }

  if ((Count != 1) && (Count != 2) && (Count != 4) && (Count != 8)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Read) {
    Status = PciIo->Mem.Read (
                          PciIo,
                          EfiPciIoWidthUint8,
                          BarIndex,
                          (UINT64) Offset,
                          Count,
                          Data
                          );
  } else {
    Status = PciIo->Mem.Write (
                          PciIo,
                          EfiPciIoWidthUint8,
                          BarIndex,
                          (UINT64) Offset,
                          Count,
                          Data
                          );
  }

  return Status;
}

/**
  Do AND operation with the value of the specified SD/MMC host controller mmio register.

  @param[in] PciIo             The PCI IO protocol instance.
  @param[in] BarIndex          The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in] Offset            The offset within the selected BAR to start the
                               memory operation.
  @param[in] Count             The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in] AndData           The pointer to the data used to do AND operation.
                               The caller is responsible for having ownership of
                               the data buffer and ensuring its size not less than
                               Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or AndData is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The AND operation succeeds.
  @retval Others                The AND operation fails.

**/
EFI_STATUS
EFIAPI
SdMmcHcAndMmio (
  IN  EFI_PCI_IO_PROTOCOL      *PciIo,
  IN  UINT8                    BarIndex,
  IN  UINT32                   Offset,
  IN  UINT8                    Count,
  IN  VOID                     *AndData
  )
{
  EFI_STATUS                   Status;
  UINT64                       Data;
  UINT64                       And;

  Status = SdMmcHcRwMmio (PciIo, BarIndex, Offset, TRUE, Count, &Data);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Count == 1) {
    And = *(UINT8*) AndData;
  } else if (Count == 2) {
    And = *(UINT16*) AndData;
  } else if (Count == 4) {
    And = *(UINT32*) AndData;
  } else if (Count == 8) {
    And = *(UINT64*) AndData;
  } else {
    return EFI_INVALID_PARAMETER;
  }

  Data  &= And;
  Status = SdMmcHcRwMmio (PciIo, BarIndex, Offset, FALSE, Count, &Data);

  return Status;
}

/**
  Do OR operation with the value of the specified SD/MMC host controller mmio register.

  @param[in] PciIo             The PCI IO protocol instance.
  @param[in] BarIndex          The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in] Offset            The offset within the selected BAR to start the
                               memory operation.
  @param[in] Count             The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in] OrData            The pointer to the data used to do OR operation.
                               The caller is responsible for having ownership of
                               the data buffer and ensuring its size not less than
                               Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or OrData is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The OR operation succeeds.
  @retval Others                The OR operation fails.

**/
EFI_STATUS
EFIAPI
SdMmcHcOrMmio (
  IN  EFI_PCI_IO_PROTOCOL      *PciIo,
  IN  UINT8                    BarIndex,
  IN  UINT32                   Offset,
  IN  UINT8                    Count,
  IN  VOID                     *OrData
  )
{
  EFI_STATUS                   Status;
  UINT64                       Data;
  UINT64                       Or;

  Status = SdMmcHcRwMmio (PciIo, BarIndex, Offset, TRUE, Count, &Data);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Count == 1) {
    Or = *(UINT8*) OrData;
  } else if (Count == 2) {
    Or = *(UINT16*) OrData;
  } else if (Count == 4) {
    Or = *(UINT32*) OrData;
  } else if (Count == 8) {
    Or = *(UINT64*) OrData;
  } else {
    return EFI_INVALID_PARAMETER;
  }

  Data  |= Or;
  Status = SdMmcHcRwMmio (PciIo, BarIndex, Offset, FALSE, Count, &Data);

  return Status;
}

/**
  SD/MMC bus power control.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.3 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] PowerCtrl      The value setting to the power control register.

  @retval TRUE              There is a SD/MMC card attached.
  @retval FALSE             There is no a SD/MMC card attached.

**/
EFI_STATUS
SdMmcHcPowerControl (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINT8                  Slot,
  IN UINT8                  PowerCtrl
  )
{
  EFI_STATUS                Status;

  //
  // Clr SD Bus Power
  //
  PowerCtrl &= (UINT8)~BIT0;
  Status = SdMmcHcRwMmio (PciIo, Slot, 0x29, FALSE, sizeof (PowerCtrl), &PowerCtrl);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Set SD Bus Voltage Select and SD Bus Power fields in Power Control Register
  //
  PowerCtrl |= BIT0;
  Status = SdMmcHcRwMmio (PciIo, Slot, 0x29, FALSE, sizeof (PowerCtrl), &PowerCtrl);

  return Status;
}

/**
  Wait for the value of the specified MMIO register set to the test value.

  @param[in]  PciIo         The PCI IO protocol instance.
  @param[in]  BarIndex      The BAR index of the standard PCI Configuration
                            header to use as the base address for the memory
                            operation to perform.
  @param[in]  Offset        The offset within the selected BAR to start the
                            memory operation.
  @param[in]  Count         The width of the mmio register in bytes.
                            Must be 1, 2, 4 or 8 bytes.
  @param[in]  MaskValue     The mask value of memory.
  @param[in]  TestValue     The test value of memory.

  @retval EFI_NOT_READY     The MMIO register hasn't set to the expected value.
  @retval EFI_SUCCESS       The MMIO register has expected value.
  @retval Others            The MMIO operation fails.

**/
EFI_STATUS
EFIAPI
SdMmcHcCheckMmioSet (
  IN  EFI_PCI_IO_PROTOCOL       *PciIo,
  IN  UINT8                     BarIndex,
  IN  UINT32                    Offset,
  IN  UINT8                     Count,
  IN  UINT64                    MaskValue,
  IN  UINT64                    TestValue
  )
{
  EFI_STATUS            Status;
  UINT64                Value;

  //
  // Access PCI MMIO space to see if the value is the tested one.
  //
  Value  = 0;
  Status = SdMmcHcRwMmio (PciIo, BarIndex, Offset, TRUE, Count, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value &= MaskValue;

  if (Value == TestValue) {
    return EFI_SUCCESS;
  }

  return EFI_NOT_READY;
}

/**
  Wait for the value of the specified MMIO register set to the test value.

  @param[in]  PciIo         The PCI IO protocol instance.
  @param[in]  BarIndex      The BAR index of the standard PCI Configuration
                            header to use as the base address for the memory
                            operation to perform.
  @param[in]  Offset        The offset within the selected BAR to start the
                            memory operation.
  @param[in]  Count         The width of the mmio register in bytes.
                            Must be 1, 2, 4 or 8 bytes.
  @param[in]  MaskValue     The mask value of memory.
  @param[in]  TestValue     The test value of memory.
  @param[in]  Timeout       The time out value for wait memory set, uses 1
                            microsecond as a unit.

  @retval EFI_TIMEOUT       The MMIO register hasn't expected value in timeout
                            range.
  @retval EFI_SUCCESS       The MMIO register has expected value.
  @retval Others            The MMIO operation fails.

**/
EFI_STATUS
EFIAPI
SdMmcHcWaitMmioSet (
  IN  EFI_PCI_IO_PROTOCOL       *PciIo,
  IN  UINT8                     BarIndex,
  IN  UINT32                    Offset,
  IN  UINT8                     Count,
  IN  UINT64                    MaskValue,
  IN  UINT64                    TestValue,
  IN  UINT64                    Timeout
  )
{
  EFI_STATUS            Status;
  BOOLEAN               InfiniteWait;

  if (Timeout == 0) {
    InfiniteWait = TRUE;
  } else {
    InfiniteWait = FALSE;
  }

  while (InfiniteWait || (Timeout > 0)) {
    Status = SdMmcHcCheckMmioSet (
               PciIo,
               BarIndex,
               Offset,
               Count,
               MaskValue,
               TestValue
               );
    if (Status != EFI_NOT_READY) {
      return Status;
    }

    //
    // Stall for 1 microsecond.
    //
    gBS->Stall (1);

    Timeout--;
  }

  return EFI_TIMEOUT;
} 

/**
  Stop SD/MMC card clock.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.2.2 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       Succeed to stop SD/MMC clock.
  @retval Others            Fail to stop SD/MMC clock.

**/
EFI_STATUS
SdMmcHcStopClock (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINT8                  Slot
  )
{
  EFI_STATUS                Status;
  UINT32                    PresentState;
  UINT16                    ClockCtrl;

  //
  // Ensure no SD transactions are occurring on the SD Bus by
  // waiting for Command Inhibit (DAT) and Command Inhibit (CMD)
  // in the Present State register to be 0.
  //
  Status = SdMmcHcWaitMmioSet (
             PciIo,
             Slot,
             0x24,
             sizeof (PresentState),
             BIT0 | BIT1,
             0,
             SD_MMC_HC_GENERIC_TIMEOUT
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Set SD Clock Enable in the Clock Control register to 0
  //
  ClockCtrl = (UINT16)~BIT2;
  Status = SdMmcHcAndMmio (PciIo, Slot, 0x2C, sizeof (ClockCtrl), &ClockCtrl);

  return Status;
}

/**
  SD/MMC card clock supply.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.2.1 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] ClockFreq      The max clock frequency to be set. The unit is KHz.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
SdMmcHcClockSupply (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINT8                  Slot,
  IN UINT64                 ClockFreq,
  IN SD_MMC_HC_SLOT_CAP     Capability
  )
{
  EFI_STATUS                Status;
  UINT32                    BaseClkFreq;
  UINT32                    SettingFreq;
  UINT32                    Divisor;
  UINT32                    Remainder;
  UINT16                    ControllerVer;
  UINT16                    ClockCtrl;

  //
  // Calculate a divisor for SD clock frequency
  //
  ASSERT (Capability.BaseClkFreq != 0);

  BaseClkFreq = Capability.BaseClkFreq;
  if (ClockFreq == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (ClockFreq > (BaseClkFreq * 1000)) {
    ClockFreq = BaseClkFreq * 1000;
  }

  //
  // Calculate the divisor of base frequency.
  //
  Divisor     = 0;
  SettingFreq = BaseClkFreq * 1000;
  while (ClockFreq < SettingFreq) {
    Divisor++;

    SettingFreq = (BaseClkFreq * 1000) / (2 * Divisor);
    Remainder   = (BaseClkFreq * 1000) % (2 * Divisor);
    if ((ClockFreq == SettingFreq) && (Remainder == 0)) {
      break;
    }
    if ((ClockFreq == SettingFreq) && (Remainder != 0)) {
      SettingFreq ++;
    }
  }

  DEBUG ((DEBUG_INFO, "BaseClkFreq %dMHz Divisor %d ClockFreq %dKhz\n", BaseClkFreq, Divisor, ClockFreq));

  Status = SdMmcHcRwMmio (PciIo, Slot, 0xFE, TRUE, sizeof (ControllerVer), &ControllerVer);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Set SDCLK Frequency Select and Internal Clock Enable fields in Clock Control register.
  //
  if (((ControllerVer & 0xFF) == 2) || ((ControllerVer & 0xFF) == 3)) {
    ASSERT (Divisor <= 0x3FF);
    ClockCtrl = ((Divisor & 0xFF) << 8) | ((Divisor & 0x300) >> 2);
  } else if (((ControllerVer & 0xFF) == 0) || ((ControllerVer & 0xFF) == 1)) {
    //
    // Only the most significant bit can be used as divisor.
    //
    if (((Divisor - 1) & Divisor) != 0) {
      Divisor = 1 << (HighBitSet32 (Divisor) + 1);
    }
    ASSERT (Divisor <= 0x80);
    ClockCtrl = (Divisor & 0xFF) << 8;
  } else {
    DEBUG ((DEBUG_ERROR, "Unknown SD Host Controller Spec version [0x%x]!!!\n", ControllerVer));
    return EFI_UNSUPPORTED;
  }

  //
  // Stop bus clock at first
  //
  Status = SdMmcHcStopClock (PciIo, Slot);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Supply clock frequency with specified divisor
  //
  ClockCtrl |= BIT0;
  Status = SdMmcHcRwMmio (PciIo, Slot, 0x2C, FALSE, sizeof (ClockCtrl), &ClockCtrl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set SDCLK Frequency Select and Internal Clock Enable fields fails\n"));
    return Status;
  }

  //
  // Wait Internal Clock Stable in the Clock Control register to be 1
  //
  Status = SdMmcHcWaitMmioSet (
             PciIo,
             Slot,
             0x2C,
             sizeof (ClockCtrl),
             BIT1,
             BIT1,
             SD_MMC_HC_GENERIC_TIMEOUT
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Set SD Clock Enable in the Clock Control register to 1
  //
  ClockCtrl = BIT2;
  Status = SdMmcHcOrMmio (PciIo, Slot, 0x2C, sizeof (ClockCtrl), &ClockCtrl);

  return Status;
}

/**
  Supply SD/MMC card with lowest clock frequency at initialization.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
SdMmcHcInitClockFreq (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINT8                  Slot,
  IN SD_MMC_HC_SLOT_CAP     Capability
  )
{
  EFI_STATUS                Status;
  UINT32                    InitFreq;

  //
  // Calculate a divisor for SD clock frequency
  //
  if (Capability.BaseClkFreq == 0) {
    //
    // Don't support get Base Clock Frequency information via another method
    //
    return EFI_UNSUPPORTED;
  }
  //
  // Supply 400KHz clock frequency at initialization phase.
  //
  InitFreq = 400;
  Status = SdMmcHcClockSupply (PciIo, Slot, InitFreq, Capability);
  return Status;
}

/**
  Supply SD/MMC card with maximum voltage at initialization.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.3 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The voltage is supplied successfully.
  @retval Others            The voltage isn't supplied successfully.

**/
EFI_STATUS
SdMmcHcInitPowerVoltage (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINT8                  Slot,
  IN SD_MMC_HC_SLOT_CAP     Capability
  )
{
  EFI_STATUS                Status;
  UINT8                     MaxVoltage;
  UINT8                     HostCtrl2;

  //
  // Calculate supported maximum voltage according to SD Bus Voltage Select
  //
  if (Capability.Voltage33 != 0) {
    //
    // Support 3.3V
    //
    MaxVoltage = 0x0E;
  } else if (Capability.Voltage30 != 0) {
    //
    // Support 3.0V
    //
    MaxVoltage = 0x0C;
  } else if (Capability.Voltage18 != 0) {
    //
    // Support 1.8V
    //
    MaxVoltage = 0x0A;
    HostCtrl2  = BIT3;
    Status = SdMmcHcOrMmio (PciIo, Slot, 0x3E, sizeof (HostCtrl2), &HostCtrl2);
    gBS->Stall (5000);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    ASSERT (FALSE);
    return EFI_DEVICE_ERROR;
  }

  //
  // Set SD Bus Voltage Select and SD Bus Power fields in Power Control Register
  //
  Status = SdMmcHcPowerControl (PciIo, Slot, MaxVoltage);

   if (BhtHostPciSupport(PciIo) == 1){
	   // 1.8V signaling enable for emmc card
	   HostCtrl2  = BIT3;
	   Status = SdMmcHcOrMmio (PciIo, Slot, 0x3E, sizeof (HostCtrl2), &HostCtrl2);
	   gBS->Stall (5000);
	   if (EFI_ERROR (Status)) {
	 	return Status;
	   }	  
   }
   else
   {
    /*when disable 1.8v setting for SD card, driver need to delay 10ms
    waiting for card voltage stable.*/
      gBS->Stall(10000);
   }

  return Status;
}

/**
  Initialize the Timeout Control register with most conservative value at initialization.

  Refer to SD Host Controller Simplified spec 3.0 Section 2.2.15 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       The timeout control register is configured successfully.
  @retval Others            The timeout control register isn't configured successfully.

**/
EFI_STATUS
SdMmcHcInitTimeoutCtrl (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINT8                  Slot
  )
{
  EFI_STATUS                Status;
  UINT8                     Timeout;

  Timeout = 0x0E;
  Status  = SdMmcHcRwMmio (PciIo, Slot, 0x2E, FALSE, sizeof (Timeout), &Timeout);

  return Status;
}

/**

  Override function for SDHCI capability bits

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in,out]  SdMmcHcSlotCapability The SDHCI capability structure.
  @param[in,out]  BaseClkFreq           The base clock frequency value that
                                        optionally can be updated.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.
  @retval EFI_INVALID_PARAMETER SdMmcHcSlotCapability is NULL

**/
EFI_STATUS
EFIAPI
BhtHostOverrideCapability (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN OUT  VOID                            *SdMmcHcSlotCapability,
  IN OUT  UINT32                          *BaseClkFreq
  )
{
  EFI_STATUS                              Status;
  EFI_PCI_IO_PROTOCOL                     *PciIo;
//  SD_MMC_HC_SLOT_CAP                      *Cap;
  UINT16                                  EmmcDtrVar;
  UINTN                                   EmmcDtrVarSize;

  Status = gBS->HandleProtocol (
                ControllerHandle,
                &gEfiPciIoProtocolGuid,
                (VOID **) &PciIo
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (BhtHostPciSupport (PciIo)) {
//    Cap = (SD_MMC_HC_SLOT_CAP *)SdMmcHcSlotCapability;
//    //
//    // WA: Disable 8-bit bus width support
//    //
//    Cap->BusWidth8 = 0;
//    Cap->Hs400     = 0;
//    Cap->Sdr104    = 0;
//    Cap->Ddr50     = 0;
    //
    // Set DTR to 4-bit buswidth + HS200
    //
    EmmcDtrVar = 0x0417;
    EmmcDtrVarSize = sizeof (EmmcDtrVar);
    Status = gRT->SetVariable (
                    L"EMMC_DTR",
                    &gEfiGenericVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    EmmcDtrVarSize,
                    &EmmcDtrVar
                    );
    DEBUG((EFI_D_ERROR, "BhtHostOverrideCapability: Set EMMC_DTR Variable (%r) - DeviceType=0x%x, BusWidth=0x%x\n", Status, 0x17, 0x04));
  }

  return EFI_SUCCESS;
}

/**

  Override function for SDHCI controller operations

  @param[in]      ControllerHandle      The EFI_HANDLE of the controller.
  @param[in]      Slot                  The 0 based slot index.
  @param[in]      PhaseType             The type of operation and whether the
                                        hook is invoked right before (pre) or
                                        right after (post)
  @param[in,out]  PhaseData             The pointer to a phase-specific data.

  @retval EFI_SUCCESS           The override function completed successfully.
  @retval EFI_NOT_FOUND         The specified controller or slot does not exist.
  @retval EFI_INVALID_PARAMETER PhaseType is invalid

**/
EFI_STATUS
EFIAPI
BhtHostOverrideNotifyPhase (
  IN      EFI_HANDLE                      ControllerHandle,
  IN      UINT8                           Slot,
  IN      EDKII_SD_MMC_PHASE_TYPE         PhaseType,
  IN OUT  VOID                           *PhaseData
  )
{
  EFI_STATUS                              Status;
  EFI_PCI_IO_PROTOCOL                     *PciIo;
  UINT8                                   Value;
  UINT32                                  value32;
  UINT64                                  Cap;
  SD_MMC_HC_SLOT_CAP                      Capability;
  VOID                                    *Registration;

  Status = gBS->HandleProtocol (
                ControllerHandle,
                &gEfiPciIoProtocolGuid,
                (VOID **) &PciIo
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  switch (PhaseType) {
  case EdkiiSdMmcInitHostPre:
    if (BhtHostPciSupport (PciIo) == 1) {
      /* FET on */
      PciBhtOr32 (PciIo, 0xEC, 0x3);
      /* Led on */
      PciBhtAnd32 (PciIo, 0x334, (UINT32)~BIT13);
      PciBhtOr32 (PciIo, 0xD4, BIT6);
      /* Set 1.8v emmc signaling flag */
      PciBhtOr32 (PciIo, 0x308, BIT4);
      /* Set 200MBaseClock */
      PciBhtAnd32 (PciIo, 0x304,0x0000FFFF);
      PciBhtOr32 (PciIo, 0x304,0x25100000);	
      PciBhtOr32 (PciIo, 0x3E4, BIT22);
      //enable internal clk
      PciIo->Mem.Read (PciIo,EfiPciIoWidthUint8, 0, 0x2C, 1, &Value);
      Value |= BIT0;
      PciIo->Mem.Write (PciIo,EfiPciIoWidthUint8, 0, 0x2C, 1, &Value);
      //wait BaseClk stable 0x1CC bit14
      PciIo->Mem.Read (PciIo,EfiPciIoWidthUint8, 0, 0x1CD, 1, &Value);
      while(!(Value & BIT6)) {		
        PciIo->Mem.Read (PciIo,EfiPciIoWidthUint8, 0, 0x1CD, 1, &Value);
        DbgMsg (L"0x1CD=0x%x\n", Value);
      }
      //enable hardware tuning
      Value = (UINT8)(~0x10);
      PciIo->Mem.Write (PciIo,EfiPciIoWidthUint8, 0, 0x110, 1, &Value);
    } else if (BhtHostPciSupport (PciIo) == 2) {
      //Unlock PCR access
      PciBhtAnd32(PciIo, 0xd0, ~(BIT31));
  
      //PCR 0x304[31:16] = 16'h2510   (200MHz PLL)
      value32 = PciBhtRead32(PciIo, 0x304);
      value32 |= 0xffff0000;
      value32 &= 0x2510ffff;
      PciBhtWrite32(PciIo, 0x304, value32);
      
      //PCR 0x3E4[22] = 1'b1(Divide the clock to 1/4)
      value32 = PciBhtRead32(PciIo, 0x3E4);
      value32 |= (1 << 22);
      PciBhtWrite32(PciIo, 0x3E4, value32);
  
      //PCR 0xD0[31] = 1'b1(Lock PCR access)
      PciBhtOr32(PciIo, 0xd0, (BIT31));

      //enable internal clk
      value32 = BIT0;
      Status = SdMmcHcOrMmio(PciIo, Slot, 0x2C, sizeof(value32), &value32);
      //reset pll start	
      Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, TRUE, sizeof(value32), &value32);
      value32 |= BIT12;
      Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, FALSE, sizeof(value32), &value32);
      gBS->Stall(1);
  
      //reset pll end
      Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, TRUE, sizeof(value32), &value32);
      value32 &= ~BIT12;
      value32 |= BIT18;
      Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, FALSE, sizeof(value32), &value32);
  
      //wait BaseClk stable 0x1CC bit14	
      Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, TRUE, sizeof(value32), &value32);
      while (!(value32 & BIT14)) {
        gBS->Stall(100);
        Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, TRUE, sizeof(value32), &value32);
        //DbgMsg("1CC=0x%08x\n", value32);
      }
      
      if (value32 & BIT18) {
        //Wait 2nd Card Detect debounce Finished by wait twice of debounce max time
        while (1) {
          Status = SdMmcHcRwMmio(PciIo, Slot, 0x24, TRUE, sizeof(value32), &value32);
          if (((value32 >> 16) & 0x01) == ((value32 >> 18) & 0x01))
            break;
        }
        //force pll active end
        Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, TRUE, sizeof(value32), &value32);
        value32 &= ~BIT18;
        Status = SdMmcHcRwMmio(PciIo, Slot, 0x1CC, FALSE, sizeof(value32), &value32);
      }
    }
    break;
  case EdkiiSdMmcInitHostPost:
    if (BhtHostPciSupport (PciIo) == 1) {
      //
      // Enable 1.8V signaling
      //
      PciIo->Mem.Read (PciIo,EfiPciIoWidthUint8, 0, 0x3E, 1, &Value);
      Value |= BIT3;
      PciIo->Mem.Write (PciIo,EfiPciIoWidthUint8, 0, 0x3E, 1, &Value);
      gBS->Stall (5000);
      if (!mRegistered) {
        EfiCreateProtocolNotifyEvent (
          &gEfiSdMmcPassThruProtocolGuid,
          TPL_CALLBACK,
          BhtHostInitializeDoneHandler,
          NULL,
          (VOID **)&Registration
          );
        mRegistered= TRUE;
        mControllerHandle = ControllerHandle;
      }
    } else if (BhtHostPciSupport (PciIo) == 2) {
      Status = SdMmcHcRwMmio(PciIo, Slot, 0x40, TRUE, sizeof(Cap), &Cap);
      if (EFI_ERROR(Status)) {
        return Status;
      }
      CopyMem(&Capability, &Cap, sizeof(Cap));
      
      Status = SdMmcHcInitPowerVoltage (PciIo, Slot, Capability);
      if (EFI_ERROR(Status)) {
        //DbgMsg("emmc host init failure\n");
        return Status;
      }

      Status = SdMmcHcInitClockFreq (PciIo, Slot, Capability);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      Status = SdMmcHcInitTimeoutCtrl (PciIo, Slot);
      return Status;
    }

  default:
    break;
  }
  return EFI_SUCCESS;
}

// BOOLEAN BhtHostPciSupport(EFI_PCI_IO_PROTOCOL *PciIo)
// {
// 	PCI_TYPE00		Pci;

// 	PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32,		
// 				  0, sizeof Pci / sizeof (UINT32), &Pci);

// 	DEBUG ((DEBUG_INFO, "check device %04x:%04x\n", Pci.Hdr.VendorId, Pci.Hdr.DeviceId));

// 	if (Pci.Hdr.VendorId != 0x1217)
// 		goto end;

// 	switch (Pci.Hdr.DeviceId)
// 	{
// 		case 0x8420:	//PCI_DEV_ID_SDS0
// 		case 0x8421:	//PCI_DEV_ID_SDS1
// 		case 0x8520:	//PCI_DEV_ID_FJ2
// 		case 0x8620:	//PCI_DEV_ID_SB0
// 		case 0x8621:	//PCI_DEV_ID_SB1
// 			g_deviceId = Pci.Hdr.DeviceId;
// 			return 1;
// 		default:
// 			break;
// 	}

// 	end:
// 	return 0;
// }

BOOLEAN BhtHostPciSupport(EFI_PCI_IO_PROTOCOL *PciIo)
{
	PCI_TYPE00		Pci;

	PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32,		
				  0, sizeof Pci / sizeof (UINT32), &Pci);

	DEBUG ((DEBUG_INFO, "check device %04x:%04x\n", Pci.Hdr.VendorId, Pci.Hdr.DeviceId));

	if (Pci.Hdr.VendorId != 0x1217)
		goto end;

	switch (Pci.Hdr.DeviceId)
	{
		//case 0x8420:	//PCI_DEV_ID_SDS0
		//case 0x8421:	//PCI_DEV_ID_SDS1
		case 0x8520:	//PCI_DEV_ID_FJ2
		case 0x8621:	//PCI_DEV_ID_SB0
            g_deviceId = Pci.Hdr.DeviceId;
            return 2;
		case 0x8620:	//PCI_DEV_ID_SB1
			g_deviceId = Pci.Hdr.DeviceId;
			return 1;
		default:
			break;
	}

	end:
	return 0;
}

void DbgNull(IN CONST CHAR16 * fmt, ...)
{
}

UINT32 bht_readl(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset)
{
	UINT32 arg;
	PciIo->Mem.Read(PciIo,EfiPciIoWidthUint32,1,offset,1,&arg);
	return arg;
}

void bht_writel(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value)
{
	PciIo->Mem.Write(PciIo,EfiPciIoWidthUint32,1,offset,1,&value);
}


UINT32 PciBhtRead32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset)
{
	UINT32 i = 0;
	UINT32 tmp[3] = {0};

	if((g_deviceId == PCI_DEV_ID_SDS0) ||
			(g_deviceId == PCI_DEV_ID_SDS1) ||
			(g_deviceId == PCI_DEV_ID_FJ2) ||
			(g_deviceId == PCI_DEV_ID_SB0) ||
			(g_deviceId == PCI_DEV_ID_SB1))
	{
		// For Sandstorm, HW implement a mapping method by memory space reg to access PCI reg.
		// Enable mapping
	
		// Check function conflict
		if((g_deviceId == PCI_DEV_ID_SDS0) ||
				(g_deviceId == PCI_DEV_ID_FJ2) ||
				(g_deviceId == PCI_DEV_ID_SB0) ||
				(g_deviceId == PCI_DEV_ID_SB1))
		{
			i = 0;
			bht_writel(PciIo, BHT_PCIRMappingEn, 0x40000000);
			while((bht_readl(PciIo, BHT_PCIRMappingEn) & 0x40000000) == 0)
			{
				if(i == 5)
				{
					//DbgMsg((DRIVERNAME " - %s() function 0 can't lock!\n", __FUNCTION__));
					goto RD_DIS_MAPPING;
				}
						gBS->Stall(1000);
				i++;
					bht_writel(PciIo, BHT_PCIRMappingEn, 0x40000000);
	
			}
		}
		else if(g_deviceId == PCI_DEV_ID_SDS1)
		{
			i = 0;
			bht_writel(PciIo, BHT_PCIRMappingEn, 0x20000000);
			while((bht_readl(PciIo, BHT_PCIRMappingEn) & 0x20000000) == 0)
			{
				if(i == 5)
				{
					//DbgErr((DRIVERNAME " - %s() function 1 can't lock!\n", __FUNCTION__));
					goto RD_DIS_MAPPING;
				}
				gBS->Stall(1000);
				i++;
				bht_writel(PciIo, BHT_PCIRMappingEn, 0x20000000);
			}
		}
	
		// Check last operation is complete
		i = 0;
		while(bht_readl(PciIo, BHT_PCIRMappingCtl) & 0xc0000000)
		{
			if(i == 5)
			{
				//DbgErr((DRIVERNAME " - [204] = 0x%x\n", RegisterRead32(ELN_dPCIRMappingCtl)));
				//DbgErr((DRIVERNAME " - [208] = 0x%x\n", RegisterRead32(ELN_dPCIRMappingEn)));
				//DbgErr((DRIVERNAME " - %s() check last operation complete timeout!!!\n", __FUNCTION__));
				goto RD_DIS_MAPPING;
			}
			gBS->Stall(1000);
			i += 1;
		}
	
		// Set register address
		tmp[0] |= 0x40000000;
		tmp[0] |= offset;
		bht_writel(PciIo, BHT_PCIRMappingCtl, tmp[0]);
	
		// Check read is complete
		i = 0;
		while(bht_readl(PciIo, BHT_PCIRMappingCtl) & 0x40000000)
		{
			if(i == 5)
			{
				//DbgErr((DRIVERNAME " - %s() check read operation complete timeout!!!\n", __FUNCTION__));
				goto RD_DIS_MAPPING;
			}
			gBS->Stall(1000);
			i += 1;
		}
	
		// Get PCIR value
		tmp[1] = bht_readl(PciIo, BHT_PCIRMappingVal);
	
RD_DIS_MAPPING:
		// Disable mapping
		bht_writel(PciIo, BHT_PCIRMappingEn, 0x80000000);
	
		//DbgDebug(L"%s offset=%x Value:%x\n", __FUNCTION__, offset, tmp[1]);
		return tmp[1];
	}
	
	//DbgDebug(L"%s offset=%x Value:%x\n", __FUNCTION__, offset, tmp[0]);
	return tmp[0];	
}

void PciBhtWrite32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value)
{
	UINT32 tmp = 0;
    UINT32 i = 0;

	if((g_deviceId == PCI_DEV_ID_SDS0) ||
			(g_deviceId == PCI_DEV_ID_SDS1) ||
			(g_deviceId == PCI_DEV_ID_FJ2) ||
			(g_deviceId == PCI_DEV_ID_SB0) ||
			(g_deviceId == PCI_DEV_ID_SB1))
    {
        // For Sandstorm, HW implement a mapping method by memory space reg to access PCI reg.
        // Upper caller doesn't need to set 0xD0.

        // Enable mapping

        // Check function conflict
		if((g_deviceId == PCI_DEV_ID_SDS0) ||
				(g_deviceId == PCI_DEV_ID_FJ2) ||
				(g_deviceId == PCI_DEV_ID_SB0) ||
				(g_deviceId == PCI_DEV_ID_SB1))
        {
            i = 0;
            bht_writel(PciIo, BHT_PCIRMappingEn, 0x40000000);
            while((bht_readl(PciIo, BHT_PCIRMappingEn) & 0x40000000) == 0)
            {
                if(i == 5)
                {
                    //DbgErr((DRIVERNAME " - %s() function 0 can't lock!\n", __FUNCTION__));
                    goto WR_DIS_MAPPING;
                }

                gBS->Stall(1000);
                i++;
                bht_writel(PciIo, BHT_PCIRMappingEn, 0x40000000);
            }
        }
        else if(g_deviceId == PCI_DEV_ID_SDS1)
        {
            i = 0;
            bht_writel(PciIo, BHT_PCIRMappingEn, 0x20000000);

            while((bht_readl(PciIo, BHT_PCIRMappingEn) & 0x20000000) == 0)
            {
                if(i == 5)
                {
                    //DbgErr((DRIVERNAME " - %s() function 0 can't lock!\n", __FUNCTION__));
                    goto WR_DIS_MAPPING;
                }

                gBS->Stall(1000);
                i++;
                bht_writel(PciIo, BHT_PCIRMappingEn, 0x20000000);
            }
        }

        // Enable MEM access
        bht_writel(PciIo, BHT_PCIRMappingVal, 0x80000000);
        bht_writel(PciIo, BHT_PCIRMappingCtl, 0x800000D0);

        // Check last operation is complete
        i = 0;
        while(bht_readl(PciIo, BHT_PCIRMappingCtl) & 0xc0000000)
        {
            if(i == 5)
            {
                //DbgErr((DRIVERNAME " - %s() check last operation complete timeout!!!\n", __FUNCTION__));
                goto WR_DIS_MAPPING;
            }
            gBS->Stall(1000);
            i += 1;
        }

        // Set write value
        bht_writel(PciIo, BHT_PCIRMappingVal, value);
        // Set register address
        tmp |= 0x80000000;
        tmp |= offset;
        bht_writel(PciIo, BHT_PCIRMappingCtl, tmp);

        // Check write is complete
        i = 0;
        while(bht_readl(PciIo, BHT_PCIRMappingCtl) & 0x80000000)
        {
            if(i == 5)
            {
                //DbgErr((DRIVERNAME " - %s() check write operation complete timeout!!!\n", __FUNCTION__));
                goto WR_DIS_MAPPING;
            }
            gBS->Stall(1000);
            i += 1;
        }

WR_DIS_MAPPING:
        // Disable MEM access
        bht_writel(PciIo, BHT_PCIRMappingVal, 0x80000001);
        bht_writel(PciIo, BHT_PCIRMappingCtl, 0x800000D0);

        // Check last operation is complete
        i = 0;
        while(bht_readl(PciIo, BHT_PCIRMappingCtl) & 0xc0000000)
        {
            if(i == 5)
            {
                //DbgErr((DRIVERNAME " - %s() check last operation complete timeout!!!\n", __FUNCTION__));
                break;
            }
            gBS->Stall(1000);
            i += 1;
        }

        // Disable function conflict

        // Disable mapping
        bht_writel(PciIo, BHT_PCIRMappingEn, 0x80000000);
    }
}

void PciBhtOr32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value)
{
	UINT32 arg;
	arg = PciBhtRead32(PciIo, offset);
	PciBhtWrite32(PciIo, offset, value | arg);
}

void PciBhtAnd32(EFI_PCI_IO_PROTOCOL *PciIo, UINT32 offset, UINT32 value)
{
	UINT32 arg;
	arg = PciBhtRead32(PciIo, offset);
	PciBhtWrite32(PciIo, offset, value & arg);
}


