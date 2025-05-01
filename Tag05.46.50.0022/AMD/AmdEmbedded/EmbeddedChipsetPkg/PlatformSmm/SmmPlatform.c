/** @file
  A SMM driver for SMM platform initialization.

;******************************************************************************
;* Copyright (c) 2013 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "SmmPlatform.h"
#include <Protocol/FchSmmSxDispatch2.h>
#ifdef REDUCED_HARDWARE_SUPPORT
 #include <Protocol/SmmExitBootServices.h>
#endif
#include <Library/ReportStatusCodeLib.h>

#define PROGRESS_CODE_S3_SUSPEND_START  PcdGet32 (PcdProgressCodeS3SuspendStart)
#define PROGRESS_CODE_S3_SUSPEND_END    PcdGet32 (PcdProgressCodeS3SuspendEnd)

//
// Global variables.
//
UINT16                              mAcpiPmBaseAddr;
SYSTEM_CONFIGURATION                *gSetupVariable;
AMD_PBS_SETUP_OPTION                gAmdPbsConfiguration;
/**
 This function for setting Wake_L event control bit

 @retval EFI_SUCCESS

**/
EFI_STATUS
WakeLSetting (
  VOID
  )
{
  if (PcdGetBool(PcdH2OCrbOnlySupported)) {
    UINT32                        Value;
    //
    //Wake_L event setting
    //
    Value = MmioRead32 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8);
    Value |= BIT0;
    MmioWrite32(ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGA8, Value);
  }

  return EFI_SUCCESS;
}

/**
 This function either writes to or read from global register table the data of
 Gpio, SVID, LevelEdge, Pci, CpuState, Mtrr, Sio, Kbc, and Pic registers.

 @param [in]   SaveRestoreFlag  True: write data to SMM IO registers.
                                False: read data from IO to global registers.

 @retval EFI_SUCCESS

**/
EFI_STATUS
SaveRestoreState (
  IN BOOLEAN                        SaveRestoreFlag
  )
{
  EFI_STATUS              Status;

  Status = SaveRestoreCpu (SaveRestoreFlag);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  The notification of AcpiEnableCallbackStart used for enabling ACPI mode.

  Dispatched on reads from APM port with value EFI_ACPI_ENABLE_SW_SMI
  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then enabled.
  Disable SW SMI Timer
  Clear all ACPI event status and disable all ACPI events
  Disable PM sources except power button
  Clear status bits
  Disable GPE0 sources
  Clear status bits
  Disable GPE1 sources
  Clear status bits
  Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  Enable SCI

  @param [in] Protocol              Points to the protocol's unique identifier.
  @param [in] Interface             Points to the interface instance.
  @param [in] Handle                The handle on which the interface was installed.

  @retval EFI_SUCCESS               This routine was handled successfully.

**/
EFI_STATUS
EnableAcpiCallbackStart (
  IN     CONST EFI_GUID                *Protocol,
  IN     VOID                          *Interface,
  IN     EFI_HANDLE                    Handle
  )
{
  UINT8       Value8;
  Value8 = 0;

  //
  // To disable Power Button SMI.
  //
  Value8 = MmioRead8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC);
  Value8 &= ~(BIT6 + BIT7);
  MmioWrite8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC, Value8);
  Value8 = MmioRead8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB2);
  Value8 &= ~(BIT4 + BIT5);
  MmioWrite8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB2, Value8);

    OemSvcEcAcpiMode (TRUE);

  // Todo MSR C0010010 access blocked by policy
  if (PcdGetBool (PcdSmmIsolationEnable) == FALSE) {
    SaveRestoreState (FALSE);
  }

  return EFI_SUCCESS;
}

/**
  The notification of AcpiDisableCallbackStart used for disabling ACPI mode.

  Dispatched on reads from APM port with value 0xA1
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then disabled.
  Clear all ACPI event status and disable all ACPI events
  Disable PM sources except power button
  Clear status bits
  Disable GPE0 sources
  Clear status bits
  Disable GPE1 sources
  Clear status bits
  Disable SCI

  @param [in] Protocol              Points to the protocol's unique identifier.
  @param [in] Interface             Points to the interface instance.
  @param [in] Handle                The handle on which the interface was installed.

  @retval EFI_SUCCESS               This routine was handled successfully.

**/
EFI_STATUS
DisableAcpiCallbackStart (
  IN     CONST EFI_GUID                *Protocol,
  IN     VOID                          *Interface,
  IN     EFI_HANDLE                    Handle
  )
{
  OemSvcEcAcpiMode (FALSE);

  return EFI_SUCCESS;
}

/**
  SetSataDmaCapable.

  @param None.

  @retval void.

**/
VOID
SetSataDmaCapable (
  VOID
  )
{
  UINT8       BusB;
  UINT16      IoBase;

  IoBase = 0;

  //
  // Internal PCIe GPP Bridge 0 to Bus B. (PPR.1 Table 13)
  //
  BusB = PciExpressRead8 (
            PCI_EXPRESS_LIB_ADDRESS (
            0,
            8,
            2,
            0x19
            )
         );
  //
  // In AMD CBS settings, if disabled the SATA controller, PCI bridge will also be disabled.
  // Once PCI Bridge has disabled ,then its secondary bus register(0x19) value will be 0xFF.
  //
  if (BusB == 0xFF) {
    return;
  }

  if ((PciExpressRead32 (
         PCI_EXPRESS_LIB_ADDRESS (
           BusB,
           0,
           0,
           R_VENDOR_ID
           )
         ) != 0xFFFFFFFF)) {

    IoBase = (PciExpressRead16 (
                PCI_EXPRESS_LIB_ADDRESS (
                  BusB,
                  0,
                  0,
                  R_BAR4
                  )
                ) & 0xFFF0);
    IoWrite8 (
      (IoBase + 0x02),
      (IoRead8 (IoBase + 0x02) | (BIT5 + BIT6))
      );
    IoWrite8 (
      (IoBase + 0x0a),
      (IoRead8 (IoBase + 0x0a) | (BIT5 + BIT6))
      );
  }

  return ;
}

/**
  UpdatePmeSetting

  @param None.

  @retval void.

**/
VOID
UpdatePmeSetting (
  VOID
  )
{
  UINT8 Buffer;

  // Program LAN PME_En bit.
  S5WakeOnLanSetting();

  // Update PciExpWakeDis bit
  Buffer = IoRead8 (mAcpiPmBaseAddr + PM1_EN_HIGH_BYTE);
  if (gAmdPbsConfiguration.WakeOnPME) {
    Buffer &= ~BIT6;
  } else {
    Buffer |= BIT6;
  }
  IoWrite8 (mAcpiPmBaseAddr + PM1_EN_HIGH_BYTE, Buffer);
}

/**
  The notification of AcpiRestoreCallbackStart used for restoring ACPI mode.

  @param [in] Protocol              Points to the protocol's unique identifier.
  @param [in] Interface             Points to the interface instance.
  @param [in] Handle                The handle on which the interface was installed.

  @retval EFI_SUCCESS               This routine was handled successfully.

**/
EFI_STATUS
RestoreAcpiCallbackStart (
  IN     CONST EFI_GUID                *Protocol,
  IN     VOID                          *Interface,
  IN     EFI_HANDLE                    Handle
  )
{
  // Todo MSR C0010010 access blocked by policy
  if (PcdGetBool (PcdSmmIsolationEnable) == FALSE) {
    SaveRestoreState (TRUE);
  }
  SetSataDmaCapable ();

  return EFI_SUCCESS;
}

/**
  The notification of AcpiRestoreCallbackDone used for restoring ACPI mode.

  @param [in] Protocol              Points to the protocol's unique identifier.
  @param [in] Interface             Points to the interface instance.
  @param [in] Handle                The handle on which the interface was installed.

  @retval EFI_SUCCESS               This routine was handled successfully.

**/
EFI_STATUS
RestoreAcpiCallbackDone (
  IN     CONST EFI_GUID                *Protocol,
  IN     VOID                          *Interface,
  IN     EFI_HANDLE                    Handle
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RegisterAcpiCallbacks (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_EVENT         AcpiEnableCallbackStartEvent;
  EFI_EVENT         AcpiDisableCallbackStartEvent;
  EFI_EVENT         AcpiRestoreCallbackStartEvent;
  EFI_EVENT         AcpiRestoreCallbackDoneEvent;

  //
  // Register ACPI enable handler
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gAcpiEnableCallbackStartProtocolGuid,
                    EnableAcpiCallbackStart,
                    &AcpiEnableCallbackStartEvent
                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Register ACPI disable handler
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gAcpiDisableCallbackStartProtocolGuid,
                    DisableAcpiCallbackStart,
                    &AcpiDisableCallbackStartEvent
                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Register ACPI restore handler
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gAcpiRestoreCallbackStartProtocolGuid,
                    RestoreAcpiCallbackStart,
                    &AcpiRestoreCallbackStartEvent
                    );
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmRegisterProtocolNotify (
                    &gAcpiRestoreCallbackDoneProtocolGuid,
                    RestoreAcpiCallbackDone,
                    &AcpiRestoreCallbackDoneEvent
                    );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  S3 sleep entry callback for save boot current to CMOS using gEfiSmmSxDispatch2ProtocolGuid

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context         Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The S3 sleep entry callabck was handled successfully.

**/
EFI_STATUS
EFIAPI
S3SleepEntryCallBack (
  IN       EFI_HANDLE                          DispatchHandle,
  IN CONST VOID                                *DispatchContext,
  IN OUT   VOID                                *CommBuffer OPTIONAL,
  IN OUT   UINTN                               *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS        Status;
  // UINT32            Value32;

  //
  // needed to record suspend start time for performance builds
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PROGRESS_CODE_S3_SUSPEND_START);

  WakeLSetting();

  // Move to OemSvcS3Callback();
  // //Blink function
  // Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x3E4);
  // Value32 &= ~(BIT0 + BIT1);
  // Value32 |= BIT0;
  // MmioWrite32 (ACPI_MMIO_BASE + 0x3E4, Value32);

  POST_CODE (SMM_S3_SLEEP_CALLBACK);

  AsmWbinvd ();

  UpdatePmeSetting();

  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcS3Callback \n"));
  Status = OemSvcS3Callback ();
  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcS3Callback Status: %r\n", Status));

  WakeToProcessPendingCapsule (mAcpiPmBaseAddr, FixedPcdGet16 (PcdH2OSecureFlashWakeFromS3Time));

  return EFI_SUCCESS;
}



/**
  S3 sleep final callback for using gFchSmmSxDispatch2ProtocolGuid

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      DispatchContext Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The S3 sleep entry callabck was handled successfully.

**/
EFI_STATUS
EFIAPI
S3SleepFinalCallBack (
  IN       EFI_HANDLE                          DispatchHandle,
  IN CONST FCH_SMM_SX_REGISTER_CONTEXT         *DispatchContext,
  IN OUT   VOID                                *CommBuffer OPTIONAL,
  IN OUT   UINTN                               *CommBufferSize  OPTIONAL
  )
{
  ///
  /// Report status code before goto S3 sleep
  ///
  DEBUG ((DEBUG_INFO, "S3SleepFinalCallBack \n"));
  
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PROGRESS_CODE_S3_SUSPEND_END);
  
  return EFI_SUCCESS;
}



/**
  S4 sleep entry callback for save boot current to CMOS using gEfiSmmSxDispatch2ProtocolGuid

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context         Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The S4 sleep entry callabck was handled successfully.

**/
EFI_STATUS
EFIAPI
S4SleepEntryCallBack (
  IN       EFI_HANDLE                          DispatchHandle,
  IN CONST VOID                                *DispatchContext,
  IN OUT   VOID                                *CommBuffer OPTIONAL,
  IN OUT   UINTN                               *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS        Status;
  // UINT32            Value32;

  WakeLSetting();

  // Move to OemSvcS4Callback();
  // //Blink function
  // Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x3E4);
  // Value32 &= ~(BIT0 + BIT1);
  // MmioWrite32 (ACPI_MMIO_BASE + 0x3E4, Value32);

  // Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x152C);
  // Value32 &= ~(BIT22 + BIT23);
  // Value32 |= (BIT22 + BIT23);
  // MmioWrite32 (ACPI_MMIO_BASE + 0x152C, Value32);

  POST_CODE (SMM_S4_SLEEP_CALLBACK);

  UpdatePmeSetting();

  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcS4Callback \n"));
  Status = OemSvcS4Callback ();
  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcS4Callback Status: %r\n", Status));

  return EFI_SUCCESS;
}

/**
  S5 sleep entry callback for save boot current to CMOS using gEfiSmmSxDispatch2ProtocolGuid

  @param[in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context         Points to an optional handler context which was specified when the
                                  handler was registered.
  @param[in, out] CommBuffer      A pointer to a collection of data in memory that will
                                  be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS             The S5 sleep entry callabck was handled successfully.

**/
EFI_STATUS
EFIAPI
S5SleepEntryCallBack (
  IN       EFI_HANDLE                          DispatchHandle,
  IN CONST VOID                                *DispatchContext,
  IN OUT   VOID                                *CommBuffer OPTIONAL,
  IN OUT   UINTN                               *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS        Status;
  UINT8             Buffer;
  // UINT32            Value32;

  WakeLSetting();

  // Move to OemSvcS5Callback();
  // //Blink function
  // Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x3E4);
  // Value32 &= ~(BIT0 + BIT1);
  // MmioWrite32 (ACPI_MMIO_BASE + 0x3E4, Value32);

  // Value32 = MmioRead32 (ACPI_MMIO_BASE + 0x152C);
  // Value32 &= ~(BIT22 + BIT23);
  // Value32 |= (BIT22 + BIT23);
  // MmioWrite32 (ACPI_MMIO_BASE + 0x152C, Value32);

  POST_CODE (SMM_S5_SLEEP_CALLBACK);

  //
  // To see if S5LongRunTest is enabled or not.
  //
  if (gSetupVariable->S5LongRunTest == TRUE) {
    IoWrite8 (PCAT_RTC_ADDRESS_REGISTER, RTC_ADDRESS_REGISTER_B);
    Buffer = IoRead8 (PCAT_RTC_DATA_REGISTER);
    Buffer |= ALARM_INTERRUPT_ENABLE;
    IoWrite8 (PCAT_RTC_DATA_REGISTER, Buffer);
    Buffer = IoRead8 (mAcpiPmBaseAddr + PM1_EN_HIGH_BYTE);
    Buffer |= RTC_EVENT_ENABLE;
    IoWrite8 (mAcpiPmBaseAddr + PM1_EN_HIGH_BYTE, Buffer);
  }

  UpdatePmeSetting();

  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcS5Callback \n"));
  Status = OemSvcS5Callback ();
  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcS5Callback Status: %r\n", Status));

  return EFI_SUCCESS;
}

/**
  S5 Sleep AcLoss callabck.

  @param [in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param [in]      Context         Points to an optional handler context which was specified when the
                                   handler was registered.
  @param [in, out] CommBuffer      A pointer to a collection of data in memory that will
                                   be conveyed from a non-SMM environment into an SMM environment.
  @param [in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS              The S5 Sleep AcLoss callaback was handled successfully.

**/
EFI_STATUS
EFIAPI
S5SleepAcLossCallBack (
  IN       EFI_HANDLE                          DispatchHandle,
  IN CONST VOID                                *DispatchContext,
  IN OUT   VOID                                *CommBuffer OPTIONAL,
  IN OUT   UINTN                               *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS        Status;

  POST_CODE (SMM_S5_SLEEP_CALLBACK);

  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcS5AcLossCallback \n"));
  Status = OemSvcS5AcLossCallback ();
  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcS5AcLossCallback Status: %r\n", Status));

  return EFI_SUCCESS;
}

/**
  The setting which is for S5WakeOnLan.

  @param                          None.

  @retval void.

**/
VOID
S5WakeOnLanSetting (
  VOID
  )
{
  UINTN  Bus, Dev, Func, Reg;
  UINTN  BusLimit, FuncLimit;
  UINT8  Value8, PmCap;
  UINT16 PmCsr, Value16;

  Bus = Dev = Func = Reg = 0;
  BusLimit = 0;
  FuncLimit = 0;
  Value8 = PmCap= 0;
  PmCsr = Value16 = 0;

  for ( Bus = 0; Bus <= BusLimit; Bus++) {
    for ( Dev = 0; Dev <= 0x1f; Dev++) {
      for ( Func = 0; Func <= FuncLimit; Func++ ) {
        Value16 = PciExpressRead16 (
                    PCI_EXPRESS_LIB_ADDRESS (
                      Bus,
                      Dev,
                      Func,
                      R_CLASS_CODE
                      )
                    );
        Value8 = PciExpressRead8 (
                    PCI_EXPRESS_LIB_ADDRESS (
                      Bus,
                      Dev,
                      Func,
                      R_MULTIFUNC_CHECK
                      )
                    );
        if (Value8 == 0xFF) {
          continue;
        }
        if (Func == 0 && (Value8 & 0x80)) {  // multi-function
          FuncLimit = 0x07;
        }
        Value8 = PciExpressRead8 (
                   PCI_EXPRESS_LIB_ADDRESS (
                     Bus,
                     Dev,
                     Func,
                     R_BASE_CODE
                     )
                   );
        if (Value8 == 0x02) {  // network controller
          PmCap = PciExpressRead8 (
                    PCI_EXPRESS_LIB_ADDRESS (
                      Bus,
                      Dev,
                      Func,
                      R_CAPABILITY_POINTER
                      )
                    );
          if (gAmdPbsConfiguration.WakeOnPME) {
            PmCsr = PciExpressOr16 (
                      PCI_EXPRESS_LIB_ADDRESS (
                        Bus,
                        Dev,
                        Func,
                        PmCap + 0x04
                        ),
                      (0x8000 + 0x0100)
                      );
          } else {
            PciExpressAnd16 (
              PCI_EXPRESS_LIB_ADDRESS (
                Bus,
                Dev,
                Func,
                PmCap + 0x04
                ),
              (0xFEFF) // ~BIT8
              );
            PciExpressOr16 (
              PCI_EXPRESS_LIB_ADDRESS (
                Bus,
                Dev,
                Func,
                PmCap + 0x04
                ),
              0x8000 // BIT15
              );
          }
        }
        if (Value16 == 0x0604) {
          if ((PciExpressRead8 (
                 PCI_EXPRESS_LIB_ADDRESS (
                   Bus,
                   Dev,
                   Func,
                   R_SUB_BUS_NUM
                   )
                 ) > BusLimit)) {
            BusLimit = PciExpressRead8 (
                         PCI_EXPRESS_LIB_ADDRESS (
                           Bus,
                           Dev,
                           Func,
                           0x1A
                           )
                         );
          }
        }
      }
      FuncLimit = 0;
    }
  }
}

/**
  S5 Sleep AcLoss callabck.

  @param [in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param [in]      Context         Points to an optional handler context which was specified when the
                                   handler was registered.
  @param [in, out] CommBuffer      A pointer to a collection of data in memory that will
                                   be conveyed from a non-SMM environment into an SMM environment.
  @param [in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS              The S5SleepWakeOnLanCallBack was handled successfully.

**/
EFI_STATUS
EFIAPI
S5SleepWakeOnLanCallBack (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  S5WakeOnLanSetting ();

  return EFI_SUCCESS;
}

/**
  S5 Sleep WakeOnTime callBack.

  @param [in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param [in]      Context         Points to an optional handler context which was specified when the
                                   handler was registered.
  @param [in, out] CommBuffer      A pointer to a collection of data in memory that will
                                   be conveyed from a non-SMM environment into an SMM environment.
  @param [in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS              The S5SleepWakeOnTimeCallBack was handled successfully.

**/
EFI_STATUS
EFIAPI
S5SleepWakeOnTimeCallBack (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  S5WakeUpSetting (
    mAcpiPmBaseAddr,
    gSetupVariable
    );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RegisterSxHandlers (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_SMM_SX_DISPATCH2_PROTOCOL  *SmmSxDispatch2;
  FCH_SMM_SX_DISPATCH2_PROTOCOL  *FchSxDispatch2;
  EFI_SMM_SX_REGISTER_CONTEXT    SmmSxDispatchContext;
  FCH_SMM_SX_REGISTER_CONTEXT    FchSxRegisterContext;
  EFI_HANDLE                     DispatchHandle;
  EFI_HANDLE                     WakeOnAcLossHandle;
  EFI_HANDLE                     WakeOnLanHandle;
  EFI_HANDLE                     WakeOnTimeHandle;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSxDispatch2ProtocolGuid,
                    NULL,
                    (VOID**)&SmmSxDispatch2
                    );
  ASSERT_EFI_ERROR(Status);

  SmmSxDispatchContext.Type  = SxS3;
  SmmSxDispatchContext.Phase = SxEntry;
  Status = SmmSxDispatch2->Register (
                             SmmSxDispatch2,
                             S3SleepEntryCallBack,
                             &SmmSxDispatchContext,
                             &DispatchHandle
                             );
  ASSERT_EFI_ERROR(Status);

  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmSxDispatch2ProtocolGuid,
                    NULL,
                    (VOID**)&FchSxDispatch2
                    );
  ASSERT_EFI_ERROR(Status);
    
  if (!EFI_ERROR (Status)) {
    //
    // Register a callback on S3 entry as one of the last hooks to run
    //
    FchSxRegisterContext.Type  = SxS3;
    FchSxRegisterContext.Phase = SxEntry;
    FchSxRegisterContext.Order = 0xFF;
    Status = FchSxDispatch2->Register (
                               FchSxDispatch2,
                               S3SleepFinalCallBack,
                               &FchSxRegisterContext,
                               &DispatchHandle
                               );
    ASSERT_EFI_ERROR(Status);
  }

  SmmSxDispatchContext.Type  = SxS4;
  SmmSxDispatchContext.Phase = SxEntry;
  Status = SmmSxDispatch2->Register (
                             SmmSxDispatch2,
                             S4SleepEntryCallBack,
                             &SmmSxDispatchContext,
                             &DispatchHandle
                             );
  ASSERT_EFI_ERROR(Status);

  SmmSxDispatchContext.Type  = SxS5;
  SmmSxDispatchContext.Phase = SxEntry;
  Status = SmmSxDispatch2->Register (
                             SmmSxDispatch2,
                             S5SleepEntryCallBack,
                             &SmmSxDispatchContext,
                             &DispatchHandle
                             );
  ASSERT_EFI_ERROR(Status);

  Status = SmmSxDispatch2->Register (
                             SmmSxDispatch2,
                             S5SleepAcLossCallBack,
                             &SmmSxDispatchContext,
                             &WakeOnAcLossHandle
                             );
  ASSERT_EFI_ERROR(Status);

  Status = SmmSxDispatch2->Register (
                             SmmSxDispatch2,
                             S5SleepWakeOnLanCallBack,
                             &SmmSxDispatchContext,
                             &WakeOnLanHandle
                             );
  ASSERT_EFI_ERROR(Status);

  Status = SmmSxDispatch2->Register (
                             SmmSxDispatch2,
                             S5SleepWakeOnTimeCallBack,
                             &SmmSxDispatchContext,
                             &WakeOnTimeHandle
                             );
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}

/**
  The callback which is for reseting the system.

  @param [in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param [in]      Context         Points to an optional handler context which was specified when the
                                   handler was registered.
  @param [in, out] CommBuffer      A pointer to a collection of data in memory that will
                                   be conveyed from a non-SMM environment into an SMM environment.
  @param [in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS              The OsResetSmi callback was handled successfully.

**/
EFI_STATUS
EFIAPI
OsResetSmi (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                     Status;

  Status = EFI_UNSUPPORTED;

  DEBUG ((EFI_D_INFO, "OsResetSmi Entry\n"));

  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcOsResetCallback \n"));
  Status = OemSvcOsResetCallback ();
  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcOsResetCallback Status: %r\n", Status));

  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  Status = ProcessPendingCapsule ();
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  Status = SmmCsSvcResetSystem (EfiResetWarm);

  DEBUG ((EFI_D_INFO, "OsResetSmi Exit\n"));

  return EFI_SUCCESS;

}

#ifdef REDUCED_HARDWARE_SUPPORT
/**
  The notification of EnableAcpiCallback for ReduceHardware Support.

  @param  Event        Event whose notification function is being invoked.
  @param  Context      Pointer to the notification function's context

  @retval EFI_SUCCESS               This routine was handled successfully.

**/
STATIC
EFI_STATUS
EFIAPI
EnableAcpiCallbackReduceHardware (
  IN CONST EFI_GUID           *Protocol,
  IN VOID                 *Interface,
  IN EFI_HANDLE             Handle
  )
{
  UINT8       Value8;
  Value8 = 0;

  //
  // To disable Power Button SMI.
  //
  Value8 = MmioRead8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC);
  Value8 &= ~(BIT6 + BIT7);
  MmioWrite8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGAC, Value8);
  Value8 = MmioRead8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB2);
  Value8 &= ~(BIT4 + BIT5);
  MmioWrite8 (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REGB2, Value8);

  OemSvcEcAcpiMode (TRUE);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RegisterAcpiCallbacksReduceHardware (
  VOID
  )
{
  EFI_STATUS                    Status;
  VOID                          *Registration;

  if (FeaturePcdGet (PcdReduceHardwareEnable)) {

  //
  // Register notify function for EVT_SIGNAL_EXIT_BOOT_SERVICES
  //
  Status = gSmst->SmmRegisterProtocolNotify (
       &gEdkiiSmmExitBootServicesProtocolGuid,
       EnableAcpiCallbackReduceHardware,
       &Registration
       );
   ASSERT_EFI_ERROR (Status);

  }

  return EFI_SUCCESS;
}
#endif

EFI_STATUS
EFIAPI
RegisterSwHandlers (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SmmSwDispatch2;
  EFI_SMM_SW_REGISTER_CONTEXT    SmmSwDispatchContext;
  EFI_HANDLE                     DispatchHandle;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSwDispatch2ProtocolGuid,
                    NULL,
                    (VOID**)&SmmSwDispatch2
                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Register the OS restart set features disable
  //
  SmmSwDispatchContext.SwSmiInputValue = SMM_OS_RESET_SMI_VALUE;
  Status = SmmSwDispatch2->Register (
                             SmmSwDispatch2,
                             OsResetSmi,
                             &SmmSwDispatchContext,
                             &DispatchHandle
                             );
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}

/**
  When a power button event happens, it shuts off the machine.

  @param [in]      DispatchHandle  The unique handle assigned to this handler by SmiHandlerRegister().
  @param [in]      Context         Points to an optional handler context which was specified when the
                                   handler was registered.
  @param [in, out] CommBuffer      A pointer to a collection of data in memory that will
                                   be conveyed from a non-SMM environment into an SMM environment.
  @param [in, out] CommBufferSize  The size of the CommBuffer.

  @retval EFI_SUCCESS              The Power Button callaback was handled successfully.

**/
EFI_STATUS
EFIAPI
PowerButtonCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS        Status;
  UINT8             Value8;

  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices Call: OemSvcPowerButtonCallback \n"));
  Status = OemSvcPowerButtonCallback ();
  DEBUG_OEM_SVC ((EFI_D_INFO, "OemKernelServices OemSvcPowerButtonCallback Status: %r\n", Status));

  // while ((IoRead16 (
  //           mAcpiPmBaseAddr + R_FCH_ACPI_PM1_STATUS) & B_PWR_BTN_STATUS) != B_PWR_BTN_STATUS) {
  // }

  //
  // To clear RTC wakeup status.
  //
  IoWrite8 (
    PCAT_RTC_ADDRESS_REGISTER,
    RTC_ADDRESS_REGISTER_C
    );
  Value8 = IoRead8 (PCAT_RTC_DATA_REGISTER);
  IoWrite16 (
    mAcpiPmBaseAddr + R_FCH_ACPI_PM1_STATUS,
    (UINT16) (IoRead16 (mAcpiPmBaseAddr + R_FCH_ACPI_PM1_STATUS) & B_RTC_STATUS)
    );

  S5WakeUpSetting (
    mAcpiPmBaseAddr,
    gSetupVariable
    );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
RegisterPowerButtonHandlers (
  VOID
  )
{
  EFI_STATUS                               Status;
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL  *PowerButtonDispatch2;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT    PowerButtonContext;
  EFI_HANDLE                               PowerButtonHandle;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                    NULL,
                    (VOID**)&PowerButtonDispatch2
                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Register the power button SMM event
  //
  PowerButtonContext.Phase = EfiPowerButtonExit;
  Status = PowerButtonDispatch2->Register (
                                   PowerButtonDispatch2,
                                   PowerButtonCallback,
                                   &PowerButtonContext,
                                   &PowerButtonHandle
                                   );
  ASSERT_EFI_ERROR(Status);

  return EFI_SUCCESS;
}

VOID
GetAcpiPmBase (
  )
{

  mAcpiPmBaseAddr = PcdGet16 (PcdAmdFchCfgAcpiPm1EvtBlkAddr);

  DEBUG_OEM_SVC ((EFI_D_INFO, "GetAcpiPmBase:: mAcpiPmBaseAddr = %x\n", mAcpiPmBaseAddr));
  return;
}

/**
  A SMM driver for SMM platform initialization.

  @param  ImageHandle    Handle for this drivers loaded image protocol.
  @param  SystemTable    EFI system table

  @retval EFI_SUCCESS    The driver installed without error.

**/
EFI_STATUS
SmmPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                    Status;

  POST_CODE (SMM_SMM_PLATFORM_INIT);

  //
  // Find Setup Variable
  //
  gSetupVariable = (SYSTEM_CONFIGURATION *)CommonGetVariableData (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid);
  if (gSetupVariable == NULL) {
    ASSERT_EFI_ERROR(EFI_NOT_FOUND);
  }

  //
  //Get PBS Variable
  //
  Status = GetAmdPbsConfiguration (&gAmdPbsConfiguration);

  //
  // To get PMx60 AcpiPm1EvtBlk Address.
  //
  GetAcpiPmBase ();

  //
  // Register power button SMI handler
  //
  Status = RegisterPowerButtonHandlers ();
  ASSERT_EFI_ERROR(Status);

  //
  // Register Sx-state SMI handler
  //
  Status = RegisterSxHandlers ();
  ASSERT_EFI_ERROR(Status);

  //
  // Register software SMI handler
  //
  Status = RegisterSwHandlers ();
  ASSERT_EFI_ERROR(Status);

  //
  // Register ACPI Callback handler
  //
  Status = RegisterAcpiCallbacks ();
  ASSERT_EFI_ERROR(Status);

  if (FeaturePcdGet (PcdNvidiaOptimusSupported)) {
    Status = RegisterSetDisplaymode ();
  }

#ifdef REDUCED_HARDWARE_SUPPORT
  if (FeaturePcdGet (PcdReduceHardwareSupport)) {
    //
    // Register ACPI Callback handler for ReduceHardwareSupport
    //
    Status = RegisterAcpiCallbacksReduceHardware ();
    ASSERT_EFI_ERROR(Status);
  }
#endif

  return EFI_SUCCESS;
}

