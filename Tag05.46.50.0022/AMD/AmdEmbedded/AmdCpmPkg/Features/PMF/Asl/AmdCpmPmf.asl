/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
  "AmdCpmPmf.aml", // Output file
  "SSDT",                    // Signature
  0x01,                      // SSDT Revision
  "AMD",                     // OEM ID
  "CPMPMF",                  // OEM Table ID
  0x1                        // OEM Revision
  )
{

  #include "CmnDef.asi"
  #include "CmnExt.asi"
  #include "PostCode.asi"
  External (\_SB.OPM4, MethodObj)
  External (\_SB.OPM7, MethodObj)
  External (\_SB.OPM8, MethodObj)
  External (\_SB.OPMA, MethodObj)
  External (\_SB.OPME, MethodObj)

  Scope (\_SB) {

    Name (PF0A, 0x55AA55AA)
    Name (PF3A, 0x55AA55AA)
    Name (PF5A, 0x55AA55AA)
    Name (PF6A, 0x55AA55AA)
    Name (PF9A, 0x55AA55AA)
    Name (PFBA, 0x55AA55AA)
    Name (PFCA, 0x55AA55AA)
    Name (PFDA, 0x55AA55AA)

    OperationRegion(PMT0, SystemMemory, PF0A, 0xD)
    Field(PMT0, ByteAcc, Lock, Preserve)
    {
      CpmApmfSupportedNotificationsMask,  32,
      CpmApmfSupportedFunctionsBitVector, 32,
      CpmApmfPmfNotifyCommandCode,         8,
      CpmApmfHeartbeatTimeInterval,       32,
    }

    Device (PMF)
    {
      Name (_HID, "AMDI0100")
      Name (_UID, 0)
      Name(XCRS, ResourceTemplate()
      {
          Memory32Fixed(ReadWrite, 0xAAAAAAAA, 0x0004B000)
      })
      Name (CpmPmfPendingSystemBIOSRequests, 0)
      Name (CpmPmfPendingDytcCqlEvent, 0)
      Name (CpmPmfPendingDytcAmtEvent, 0)
      Name (CpmPmfPendingUpdatePowerLimitsfPPT, 0)
      Name (CpmPmfPendingUpdatePowerLimitssPPT, 0)
      Name (CpmPmfPendingUpdatePowerLimitssPPTAPUOnly, 0)
      Name (CpmPmfPendingUpdatePowerLimitsSPL, 0)
      Name (CpmPmfPendingUpdatePowerLimitsSTTMinLimit, 0)
      Name (CpmPmfPendingUpdatePowerLimitsSTTSkinTempLimitAPU, 0)
      Name (CpmPmfPendingUpdatePowerLimitsSTTSkinTempLimitHS2, 0)
      Name (CpmPmfPendingEnableDisableCnQF, 0)
      Name (CpmPmfPendingCustomPolicyInput1, 0)
      Name (CpmPmfPendingCustomPolicyInput2, 0)

      #include "APMF.asi"
      #include "ATST.asi"
    }

  } // End of Scope (\_SB)
}

