/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmSmm.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Protocol/SmmVariable.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Pei/AmdCpmOemInitPei/AmdCpmOemInitPeim.h>
#include <Protocol/FchSmmSwDispatch2.h>
#include <Protocol/FchSmmPeriodicalDispatch2.h>
#include <FchRegistersCommon.h>

CPM_DEFINE_gBS;
CPM_DEFINE_gSmst;

STATIC AMD_CPM_TABLE_PROTOCOL *gCpmTableSmmProtocol;
UINT8                         PSxStatus = 0x80;
UINT32                        PSxCounter = 0x0;


EFI_STATUS
EFIAPI
AmdCpmOemS3Callback (
  IN        EFI_HANDLE  DispatchHandle,
  IN        CONST VOID  *DispatchContext      OPTIONAL,
  IN OUT    VOID        *CommBuffer           OPTIONAL,
  IN OUT    UINTN       *CommBufferSize       OPTIONAL
  );

EFI_STATUS
EFIAPI
AmdCpmOemS4Callback (
  IN        EFI_HANDLE  DispatchHandle,
  IN        CONST VOID  *DispatchContext      OPTIONAL,
  IN OUT    VOID        *CommBuffer           OPTIONAL,
  IN OUT    UINTN       *CommBufferSize       OPTIONAL
  );

EFI_STATUS
EFIAPI
AmdCpmOemS5Callback (
  IN        EFI_HANDLE  DispatchHandle,
  IN        CONST VOID  *DispatchContext      OPTIONAL,
  IN OUT    VOID        *CommBuffer           OPTIONAL,
  IN OUT    UINTN       *CommBufferSize       OPTIONAL
  );

EFI_STATUS
EFIAPI
PowerButtonCallback (
  IN        EFI_HANDLE  DispatchHandle,
  IN        CONST VOID  *DispatchContext      OPTIONAL,
  IN OUT    VOID        *CommBuffer           OPTIONAL,
  IN OUT    UINTN       *CommBufferSize       OPTIONAL
  );

STATIC
EFI_STATUS
EFIAPI
RestoreSpecificQCommWLANGpioWake (
  IN        EFI_HANDLE                        DispatchHandle,
  IN        CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT    FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT    UINTN                             *SizeOfSwContext
  );

STATIC
EFI_STATUS
EFIAPI
UpdateUsb4DepSupport (
  IN        EFI_HANDLE                        DispatchHandle,
  IN        CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT    FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT    UINTN                             *SizeOfSwContext
  );

STATIC
EFI_STATUS
EFIAPI
UpdateUsb4PSxSupport (
  IN        EFI_HANDLE                        DispatchHandle,
  IN        CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT    FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT    UINTN                             *SizeOfSwContext
  );

EFI_STATUS
EFIAPI
Usb4PSxPeriodicSmiCallback (
  IN        EFI_HANDLE                                DispatchHandle,
  IN        CONST FCH_SMM_PERIODICAL_REGISTER_CONTEXT *RegisterContext,
  IN OUT    EFI_SMM_PERIODIC_TIMER_CONTEXT            *PeriodicTimerContext,
  IN OUT    UINTN                                     *SizeOfContext
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Adaptive S4 SMM driver
 *
 * This function registers the callback function before go to S4.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
AmdCpmOemSmmEntryPoint (
  IN      EFI_HANDLE                  ImageHandle,
  IN      EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                              Status;
  CPM_SMM_SX_DISPATCH_PROTOCOL            *SxDispatch;
  CPM_SMM_SX_DISPATCH_CONTEXT             SxContext;
  EFI_HANDLE                              SxHandle;
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *PowerButtonDispatch2;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT   PowerButtonContext;
  EFI_HANDLE                              PowerButtonHandle;
  FCH_SMM_SW_DISPATCH2_PROTOCOL           *AmdSwDispatch;
  EFI_HANDLE                              SwHandle;
  FCH_SMM_SW_REGISTER_CONTEXT             SwRegisterContext;
  UINT32                                  VidDid;
  EFI_SMM_VARIABLE_PROTOCOL               *SmmVariable;
  UINTN                                   VarSize;
  AMD_PBS_SETUP_OPTION                    AmdPbsConfiguration;

  If_CpmInSmm (ImageHandle, SystemTable, gBS, gSmst, Status) {

    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
    //
    // Initialize global variables
    //
    Status = gSmst->SmmLocateProtocol ( &gAmdCpmTableSmmProtocolGuid,
                                        NULL,
                                        (VOID**)&gCpmTableSmmProtocol );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    Status = gSmst->SmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                        NULL,
                                        (VOID **)&SmmVariable );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    VarSize = sizeof (AMD_PBS_SETUP_OPTION);
    Status = SmmVariable->SmmGetVariable (AMD_PBS_SETUP_VARIABLE_NAME,
                                          &gAmdPbsSystemConfigurationGuid,
                                          NULL,
                                          &VarSize,
                                          &AmdPbsConfiguration );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    //
    //  Locate SMM Sx dispatch protocol
    //
    Status = CpmSmmLocateProtocol ( &gCpmSmmSxDispatchProtocolGuid,
                                    NULL,
                                    (VOID**)&SxDispatch );
    ASSERT_EFI_ERROR (Status);

    //
    //  Locate SMM Power Button  dispatch protocol
    //
    Status = CpmSmmLocateProtocol ( &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                                    NULL,
                                    (VOID**)&PowerButtonDispatch2 );
    ASSERT_EFI_ERROR (Status);

    Status = CpmSmmLocateProtocol ( &gFchSmmSwDispatch2ProtocolGuid,
                                    NULL,
                                    &AmdSwDispatch);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-4-Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    SxContext.Type  = SxS3;
    SxContext.Phase = SxEntry;
    Status = SxDispatch->Register ( SxDispatch,
                                    AmdCpmOemS3Callback,
                                    &SxContext,
                                    &SxHandle );
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-AmdCpmOemS3Callback %r\n", __FUNCTION__, Status));

    SxContext.Type  = SxS4;
    SxContext.Phase = SxEntry;
    Status = SxDispatch->Register ( SxDispatch,
                                    AmdCpmOemS4Callback,
                                    &SxContext,
                                    &SxHandle );
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-AmdCpmOemS4Callback %r\n", __FUNCTION__, Status));

    SxContext.Type  = SxS5;
    SxContext.Phase = SxEntry;
    Status = SxDispatch->Register ( SxDispatch,
                                    AmdCpmOemS5Callback,
                                    &SxContext,
                                    &SxHandle );
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-AmdCpmOemS5Callback %r\n", __FUNCTION__, Status));

    //
    // Register the power button SMM event
    //
    PowerButtonContext.Phase = EfiPowerButtonExit;
    Status = PowerButtonDispatch2->Register ( PowerButtonDispatch2,
                                              PowerButtonCallback,
                                              &PowerButtonContext,
                                              &PowerButtonHandle );
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-PowerButtonCallback %r\n", __FUNCTION__, Status));

    //
    // Register for USB4 ACPI _DEP Support SW SMI
    //
    SwRegisterContext.AmdSwValue = (UINTN) PcdGet8 (SwSmiCmdUsb4DepSupport);
    SwRegisterContext.Order      = 0x80;
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-SW SMI 0x%X for USB4 ACPI _DEP Support\n", __FUNCTION__, (UINTN) SwRegisterContext.AmdSwValue));
    Status = AmdSwDispatch->Register (AmdSwDispatch,
                                      UpdateUsb4DepSupport,
                                      &SwRegisterContext,
                                      &SwHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-5-Status = %r\n", __FUNCTION__, Status));
      return Status;
    }

    //
    // Register for USB4 ACPI _PSx Support SW SMI
    //
    if (AmdPbsConfiguration.IntUsb4PcieTunnelingD3 != 0) {
      SwRegisterContext.AmdSwValue = (UINTN) PcdGet8 (SwSmiCmdUsb4PSxSupport);
      SwRegisterContext.Order      = 0x80;
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-SW SMI 0x%X for USB4 ACPI _PSx Support\n", __FUNCTION__, (UINTN) SwRegisterContext.AmdSwValue));
      Status = AmdSwDispatch->Register (AmdSwDispatch,
                                        UpdateUsb4PSxSupport,
                                        &SwRegisterContext,
                                        &SwHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-6-Status = %r\n", __FUNCTION__, Status));
        return Status;
      }
    } // End of if (AmdPbsConfiguration.IntUsb4PcieTunnelingD3 != 0)

    if (PcdGetBool (PcdMpmEnable) == FALSE) {
      //
      // Register for QComm WLAN Gpio Wake
      //
      VidDid = gCpmTableSmmProtocol->CommonFunction.GetPcieEndPointDeviceID (gCpmTableSmmProtocol, 0, 2, 2);
      if (VidDid == 0x110317CB) {
        // Specific Qcomm Wlan VID/DID 0x110317CB
        SwRegisterContext.AmdSwValue = CFG_CPM_ACPI_ENABLE_SW_SMI;
        SwRegisterContext.Order      = 0x80;
        DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Register-SW SMI 0x%X for RestoreSpecificQCommWLANGpioWake\n", __FUNCTION__, (UINTN) SwRegisterContext.AmdSwValue));
        Status = AmdSwDispatch->Register (AmdSwDispatch,
                                          RestoreSpecificQCommWLANGpioWake,
                                          &SwRegisterContext,
                                          &SwHandle);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-7-Status = %r\n", __FUNCTION__, Status));
          return Status;
        }
      }
    } // End of if (PcdGetBool (PcdMpmEnable) == FALSE)
  } // End of If_CpmInSmm (ImageHandle, SystemTable, gBS, gSmst, Status)

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-8-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
AmdCpmOemS3Callback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *DispatchContext OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                      Status;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT32                          Value32;
  UINTN                           VarSize;
  EFI_SMM_VARIABLE_PROTOCOL       *SmmVariable;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
  Status = CpmSmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                  NULL,
                                  (VOID **)&SmmVariable );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  VarSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = SmmVariable->SmmGetVariable ( AMD_PBS_SETUP_VARIABLE_NAME,
                                         &gAmdPbsSystemConfigurationGuid,
                                         NULL,
                                         &VarSize,
                                         &AmdPbsConfiguration );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (AmdPbsConfiguration.AmdBlinkLed == 1) {
    Value32 = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (0xFED80300 + 0xE4);
    Value32 &= ~(UINT32)(BIT1 + BIT0);
    Value32 |= BIT0;
    gCpmTableSmmProtocol->CommonFunction.MmioWrite32 (0xFED80300 + 0xE4, Value32);
  }

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

VOID
AmdCpmWwanPwrDwn (
  UINT8   WWANDeviceSupport
  )
{
  UINT8   Value8;

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));

  switch (WWANDeviceSupport) {
  case 0:
    break;
  case 1: // Fibocom L860 R+
    Value8 = gCpmTableSmmProtocol->CommonFunction.GetGpio (gCpmTableSmmProtocol, WWAN_RESET);
    if (Value8 == 0) {
      //
      // PERST# is low, Device is in D3 L2/3
      //
      DEBUG ((DEBUG_INFO, "  PERST# is low, Device is in D3 L2/3\n"));
      gCpmTableSmmProtocol->CommonFunction.Stall(gCpmTableSmmProtocol, 30000);
      gCpmTableSmmProtocol->CommonFunction.SetGpio (gCpmTableSmmProtocol, WWAN_MODULE_RESET, 0);
      gCpmTableSmmProtocol->CommonFunction.Stall(gCpmTableSmmProtocol, 10000);
      gCpmTableSmmProtocol->CommonFunction.SetGpio (gCpmTableSmmProtocol, WWAN_CARD_POWER_OFF_L, 0);
      gCpmTableSmmProtocol->CommonFunction.Stall(gCpmTableSmmProtocol, 10000);
    }
    break;
  default:
    break;
  }

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-\n", __FUNCTION__));
  return;
}

EFI_STATUS
EFIAPI
AmdCpmOemS4Callback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *DispatchContext OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                      Status;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT32                          Value32;
  UINTN                           VarSize;
  EFI_SMM_VARIABLE_PROTOCOL       *SmmVariable;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
  Status = CpmSmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                  NULL,
                                  (VOID **)&SmmVariable );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  VarSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = SmmVariable->SmmGetVariable ( AMD_PBS_SETUP_VARIABLE_NAME,
                                         &gAmdPbsSystemConfigurationGuid,
                                         NULL,
                                         &VarSize,
                                         &AmdPbsConfiguration );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (AmdPbsConfiguration.AmdBlinkLed == 1) {
    Value32 = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (0xFED80300 + 0xE4);
    Value32 &= ~(UINT32)(BIT1 + BIT0);
    gCpmTableSmmProtocol->CommonFunction.MmioWrite32 (0xFED80300 + 0xE4, Value32);
  }

  AmdCpmWwanPwrDwn (AmdPbsConfiguration.PbsWWANDeviceSupport);

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
  return Status;
}


EFI_STATUS
EFIAPI
AmdCpmOemS5Callback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *DispatchContext OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  EFI_STATUS                      Status;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT32                          Value32;
  UINTN                           VarSize;
  EFI_SMM_VARIABLE_PROTOCOL       *SmmVariable;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
  Status = CpmSmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                  NULL,
                                  (VOID **)&SmmVariable );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  VarSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = SmmVariable->SmmGetVariable ( AMD_PBS_SETUP_VARIABLE_NAME,
                                         &gAmdPbsSystemConfigurationGuid,
                                         NULL,
                                         &VarSize,
                                         &AmdPbsConfiguration );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (AmdPbsConfiguration.AmdBlinkLed == 1) {
    Value32 = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (0xFED80300 + 0xE4);
    Value32 &= ~(UINT32)(BIT1 + BIT0);
    gCpmTableSmmProtocol->CommonFunction.MmioWrite32 (0xFED80300 + 0xE4, Value32);
  }

  AmdCpmWwanPwrDwn (AmdPbsConfiguration.PbsWWANDeviceSupport);

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
  return Status;
}


EFI_STATUS
EFIAPI
PowerButtonCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *DispatchContext OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{

  EFI_STATUS                      Status;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT32                          Value32;
  UINTN                           VarSize;
  EFI_SMM_VARIABLE_PROTOCOL       *SmmVariable;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
  Status = CpmSmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                  NULL,
                                  (VOID **)&SmmVariable );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  VarSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = SmmVariable->SmmGetVariable ( AMD_PBS_SETUP_VARIABLE_NAME,
                                         &gAmdPbsSystemConfigurationGuid,
                                         NULL,
                                         &VarSize,
                                         &AmdPbsConfiguration );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (AmdPbsConfiguration.AmdBlinkLed == 1) {
    Value32 = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (0xFED80300 + 0xE4);
    Value32 &= ~(UINT32)(BIT1 + BIT0);
    gCpmTableSmmProtocol->CommonFunction.MmioWrite32 (0xFED80300 + 0xE4, Value32);
  }

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
 SMI handler used for enabling WLAN S0i3/S3/S4/S5 wake capability.
**/
STATIC
EFI_STATUS
EFIAPI
RestoreSpecificQCommWLANGpioWake (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
  //
  // To enable Wlan wake in s0i3/s3/s4/s5
  //
  MmioWrite8 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x49, (UINT8) ((MmioRead8 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x49) & 0x00) | 0xE3));
  MmioWrite8 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x4B, (UINT8) ((MmioRead8 (ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x4B) & 0xFF) | 0x30));

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
 SMI handler for USB4 ACPI _DEP Support
**/
STATIC
EFI_STATUS
EFIAPI
UpdateUsb4DepSupport (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  EFI_STATUS                      Status;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT32                          Attributes;
  UINTN                           VarSize;
  UINT8                           SmiData;
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  EFI_SMM_VARIABLE_PROTOCOL       *SmmVariable;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start\n", __FUNCTION__));
  Status = CpmSmmLocateProtocol ( &gEfiSmmVariableProtocolGuid,
                                  NULL,
                                  (VOID **)&SmmVariable );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  VarSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = SmmVariable->SmmGetVariable ( AMD_PBS_SETUP_VARIABLE_NAME,
                                         &gAmdPbsSystemConfigurationGuid,
                                         &Attributes,
                                         &VarSize,
                                         &AmdPbsConfiguration );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (AmdPbsConfiguration.Usb4DepSupport == 0) {
    DEBUG ((DEBUG_INFO, "  Set PBS option Usb4DepSupport to 1\n"));
    AmdPbsConfiguration.Usb4DepSupport = 1;
    Status = SmmVariable->SmmSetVariable (AMD_PBS_SETUP_VARIABLE_NAME,
                                          &gAmdPbsSystemConfigurationGuid,
                                          Attributes,
                                          VarSize,
                                          &AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r\n", __FUNCTION__, Status));
      return Status;
    }
  } else {
    MainTablePtr = gCpmTableSmmProtocol->MainTablePtr;
    SmiData = gCpmTableSmmProtocol->CommonFunction.IoRead8 (MainTablePtr->SwSmiPort + 1);
    if (SmiData == 0x55) {
      DEBUG ((DEBUG_INFO, "  Set PBS option Usb4DepSupport to 0\n"));
      AmdPbsConfiguration.Usb4DepSupport = 0;
      Status = SmmVariable->SmmSetVariable (AMD_PBS_SETUP_VARIABLE_NAME,
                                            &gAmdPbsSystemConfigurationGuid,
                                            Attributes,
                                            VarSize,
                                            &AmdPbsConfiguration);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-5-Status = %r\n", __FUNCTION__, Status));
        return Status;
      }
    }
  }

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-4-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
 SMI handler for USB4 ACPI _PSx Support
**/
STATIC
EFI_STATUS
EFIAPI
UpdateUsb4PSxSupport (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  EFI_STATUS                                  Status;
  UINT8                                       SmiData;
  AMD_CPM_MAIN_TABLE                          *MainTablePtr;
  FCH_SMM_PERIODICAL_DISPATCH2_PROTOCOL       *PeriodicalDispatch;
  FCH_SMM_PERIODICAL_REGISTER_CONTEXT         PeriodicalRegisterContext;
  STATIC EFI_HANDLE                           PciePeriodicalHandle;

  Status = EFI_SUCCESS;
  PeriodicalRegisterContext.SmiTickInterval = LONG_TIMER_SMI_INTERVAL;
  PeriodicalRegisterContext.Period          = 500 * LONG_TIMER_SMI_INTERVAL;
  PeriodicalRegisterContext.StartNow        = 1;

  MainTablePtr = gCpmTableSmmProtocol->MainTablePtr;
  SmiData = gCpmTableSmmProtocol->CommonFunction.IoRead8 (MainTablePtr->SwSmiPort + 1);

  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-Start PSxStatus = 0x%X SmiData = 0x%X\n", __FUNCTION__, PSxStatus, SmiData));

  //
  // Periodic Timer SMI Registration
  //
  Status = gSmst->SmmLocateProtocol ( &gFchSmmPeriodicalDispatch2ProtocolGuid,
                                      NULL,
                                      (VOID **) &PeriodicalDispatch);
  if (EFI_ERROR (Status)) {
    PSxStatus = SmiData;
    DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-1-Status = %r PSxStatus = 0x%X SmiData = 0x%X\n", __FUNCTION__, Status, PSxStatus, SmiData));
    return Status;
  }

  if (((PSxStatus & 0x03) == 0x00) && ((SmiData & 0x03) != 0x00)) {
    DEBUG ((DEBUG_INFO, "  Register Periodic Smi\n"));
    Status = PeriodicalDispatch->Register ( PeriodicalDispatch,
                                            Usb4PSxPeriodicSmiCallback,
                                            &PeriodicalRegisterContext,
                                            &PciePeriodicalHandle);
    if (EFI_ERROR (Status)) {
      PSxStatus = SmiData;
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-2-Status = %r PSxStatus = 0x%X SmiData = 0x%X\n", __FUNCTION__, Status, PSxStatus, SmiData));
      return Status;
    }

  } else if (((PSxStatus & 0x03) != 0x00) && ((SmiData & 0x03) == 0x00)) {
    DEBUG ((DEBUG_INFO, "  Unregister Periodic Smi\n"));
    PSxCounter = 0x0;
    Status = PeriodicalDispatch->UnRegister ( PeriodicalDispatch,
                                              PciePeriodicalHandle);
    if (EFI_ERROR (Status)) {
      PSxStatus = SmiData;
      DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-3-Status = %r PSxStatus = 0x%X SmiData = 0x%X\n", __FUNCTION__, Status, PSxStatus, SmiData));
      return Status;
    }
  }

  PSxStatus = SmiData;
  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-End-4-Status = %r PSxStatus = 0x%X SmiData = 0x%X\n", __FUNCTION__, Status, PSxStatus, SmiData));
  return Status;
}

UINT32
CpmOemSmmMmioRead32 (
  IN       UINTN   Address
  )
{
  UINT32  Value;

  Value = *(volatile UINT32*)Address;
  return Value;
}

EFI_STATUS
EFIAPI
Usb4PSxPeriodicSmiCallback (
  IN        EFI_HANDLE                                DispatchHandle,
  IN        CONST FCH_SMM_PERIODICAL_REGISTER_CONTEXT *RegisterContext,
  IN OUT    EFI_SMM_PERIODIC_TIMER_CONTEXT            *PeriodicTimerContext,
  IN OUT    UINTN                                     *SizeOfContext
  )
{
  UINT32    Address;
  UINT32    Value;
  BOOLEAN   IssueSwSci;

  IssueSwSci = FALSE;
//  DEBUG ((DEBUG_INFO, "OEM-SMM-%a-%d-PSxStatus = 0x%X\n", __FUNCTION__, ++PSxCounter, PSxStatus));

  if ((PSxStatus & 0x01) == 0x01) {
    Address = PcieAddress(gCpmTableSmmProtocol, 0, 3, 1, 0x78);             // PCIERCCFG::ROOT_STATUS
    Value = CpmOemSmmMmioRead32 (Address);
    if ((Value & 0xF0000) == 0x10000 || (Value & 0xF0000) == 0x30000) {
      IssueSwSci = TRUE;
    }
  }

  if ((PSxStatus & 0x02) == 0x02) {
    Address = PcieAddress(gCpmTableSmmProtocol, 0, 4, 1, 0x78);             // PCIERCCFG::ROOT_STATUS
    Value = CpmOemSmmMmioRead32 (Address);
    if ((Value & 0xF0000) == 0x10000 || (Value & 0xF0000) == 0x30000) {
      IssueSwSci = TRUE;
    }
  }

  if (IssueSwSci) {
    Value = gCpmTableSmmProtocol->CommonFunction.MmioRead32 (0xFED80264) >> 8;
    Value = 1 << (UINT8) (Value & 0x1F);
    gCpmTableSmmProtocol->CommonFunction.MmioAnd32 (0xFED8020C, ~Value);
    gCpmTableSmmProtocol->CommonFunction.MmioAndThenOr32 (0xFED80218, ~Value, Value);
    gCpmTableSmmProtocol->CommonFunction.MmioAndThenOr32 (0xFED8021C, ~Value, Value);
    gCpmTableSmmProtocol->CommonFunction.Stall (gCpmTableSmmProtocol, 1000);
    gCpmTableSmmProtocol->CommonFunction.MmioAnd32 (0xFED8021C, ~Value);
    gCpmTableSmmProtocol->CommonFunction.MmioAnd32 (0xFED80218, ~Value);
  }

  return EFI_SUCCESS;
}

