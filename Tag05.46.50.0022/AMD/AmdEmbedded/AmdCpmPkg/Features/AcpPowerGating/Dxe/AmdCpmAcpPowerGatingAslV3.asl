/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#define mmACP_DSP_FUSION_RUNSTALL               0x1240644
#define mmACP_P1_FUTURE_REG_ACLK_1              0x1241A48
#define mmACP_FUSION_DSP_SW_INTR_TRIG           0x1241A24
#define mmACP_PGFSM_CONTROL                     0x1241024
#define mmACP_PGFSM_STATUS                      0x1241028

#define mmRSMU_PGFSM_CONTROL_ACP                0x900E784
#define mmRSMU_PGFSM_STATUS_ACP                 0x900E790

#define WAIT_STALL_US                           99
#define WAIT_RESPONSE_TIMEOUT                   100 // 99us * 100 = 10ms

DefinitionBlock (
    "AcpPowerGatingAsl.aml",        // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMACPV3",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_ACP_POWER_GATING  1

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External (\_SB.PCI0.PBC.ACP, DeviceObj)
    External (\_SB.PCI0.PBC.AZAL, DeviceObj)
    External(\_SB.ALIB, MethodObj)

    Name (CpmAcpPresentState, 1)    // ACP  present state
    Name (CpmAzaliaPresentState, 1) // AZALIA present state
    Name (CpmAcpController, 1)      // ACP Controller Enable/Disable
    Name (CpmAzaliaController, 1)   // ACP Controller Enable/Disable
    Name (APGE, 1)                  // ACP Power Gating Enable/Disable
    Name (ACGE, 1)                  // ACP Clock Gating Enable/Disable

    Mutex (CpmAcpMutex, 0)

    Method (CpmAcpRemovePowerGating, 0) {
      CpmDebugPrint ("FEA-ASL-CpmAcpRemovePowerGating-Start\n", 0, 0, 0, 0, 0, 0)

      Store (CpmReadSmnRegister (0, 0, 0, mmACP_DSP_FUSION_RUNSTALL), Local0)
      CpmDebugPrint ("  ACP_DSP_FUSION_RUNSTALL   = %d\n", Local0, 0, 0, 0, 0, 0)

      If (LEqual (Local0, 0)) {
        //
        // Remove only P2 power gating by triggering fusion DSP interrupt to ACP FW
        //
        CpmWriteSmnRegister (0, 0, 0, mmACP_P1_FUTURE_REG_ACLK_1, 0x1)
        CpmWriteSmnRegister (0, 0, 0, mmACP_FUSION_DSP_SW_INTR_TRIG, 0x2)

        //
        // Wait for mmACP_P1_FUTURE_REG_ACLK_1 status to be set to 0x101;
        //
        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        Store (CpmReadSmnRegister (0, 0, 0, mmACP_P1_FUTURE_REG_ACLK_1), Local1)
        While (LNotEqual (Local1, 0x101)) {
          Store (CpmReadSmnRegister (0, 0, 0, mmACP_P1_FUTURE_REG_ACLK_1), Local1)
          Decrement (Local5)
          If (LEqual (Local5, 0)) {
            CpmDebugPrint ("  Wait PGFSM status timeout", 0, 0, 0, 0, 0, 0)
            Break
          }
          Stall (WAIT_STALL_US)
        }

      } Else {
        //
        // Remove P1 and P2 power gating if fusion DSP is not running
        //
        CpmWriteSmnRegister (0, 0, 0, mmACP_PGFSM_CONTROL, 0x00000007)

        //
        //wait until PGFSM status equals to 0x0
        //
        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        Store (CpmReadSmnRegister (0, 0, 0, mmACP_PGFSM_STATUS), Local1)
        While (LNotEqual (Local1, 0x0)) {
          Store (CpmReadSmnRegister (0, 0, 0, mmACP_PGFSM_STATUS), Local1)
          Decrement (Local5)
          If (LEqual (Local5, 0)) {
            CpmDebugPrint ("  Wait PGFSM status timeout", 0, 0, 0, 0, 0, 0)
            Break
          }
          Stall (WAIT_STALL_US)
        }
      }

      CpmDebugPrint ("FEA-ASL-CpmAcpRemovePowerGating-End\n", 0, 0, 0, 0, 0, 0)
    } // End of Method (CpmAcpRemovePowerGating, 0)

    Method (CpmAcpApplyPowerGating, 0) {
      CpmDebugPrint ("FEA-ASL-CpmAcpApplyPowerGating-Start\n", 0, 0, 0, 0, 0, 0)

      //
      // Access All registers in this method using SMN access
      //
      Store (CpmReadSmnRegister (0, 0, 0, mmACP_DSP_FUSION_RUNSTALL), Local0)
      CpmDebugPrint ("  ACP_DSP_FUSION_RUNSTALL   = %d\n", Local0, 0, 0, 0, 0, 0)

      If (LEqual (Local0, 0)) {
        //
        // Apply only P2 power gating by triggering fusion DSP interrupt to ACP FW
        //

        //
        // To apply P2 power gating
        //
        CpmWriteSmnRegister (0, 0, 0, mmACP_P1_FUTURE_REG_ACLK_1, 0x2)
        CpmWriteSmnRegister (0, 0, 0, mmACP_FUSION_DSP_SW_INTR_TRIG, 0x2)

        //
        // Wait for mmACP_P1_FUTURE_REG_ACLK_1 status to be set to 0x202;
        //
        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        Store (CpmReadSmnRegister (0, 0, 0, mmACP_P1_FUTURE_REG_ACLK_1), Local1)
        While (LNotEqual (Local1, 0x202)) {
          Store (CpmReadSmnRegister (0, 0, 0, mmACP_P1_FUTURE_REG_ACLK_1), Local1)
          Decrement (Local5)
          If (LEqual (Local5, 0)) {
            CpmDebugPrint ("  Wait mmACP_P1_FUTURE_REG_ACLK_1 status timeout", 0, 0, 0, 0, 0, 0)
            Break
          }
          Stall (WAIT_STALL_US)
        }

      } Else {
        //
        // Apply P1 and P2 power gating if fusion DSP is not running
        //
        CpmWriteSmnRegister (0, 0, 0, mmACP_PGFSM_CONTROL, 0x00000006)

        //
        //wait until PGFSM status equals to 0xA
        //
        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        Store (CpmReadSmnRegister (0, 0, 0, mmACP_PGFSM_STATUS), Local1)
        While (LNotEqual (Local1, 0xA)) {
          Store (CpmReadSmnRegister (0, 0, 0, mmACP_PGFSM_STATUS), Local1)
          Decrement (Local5)
          If (LEqual (Local5, 0)) {
            CpmDebugPrint ("  Wait PGFSM status timeout", 0, 0, 0, 0, 0, 0)
            Break
          }
          Stall (WAIT_STALL_US)
        }
      }

      CpmDebugPrint ("FEA-ASL-CpmAcpApplyPowerGating-End\n", 0, 0, 0, 0, 0, 0)
    } // End of Method (CpmAcpApplyPowerGating, 0)

    Method (CpmPowerGateOn, 0) {
      CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-Start\n", 0, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  CpmAcpPresentState    = %d\n", CpmAcpPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  CpmAzaliaPresentState = %d\n", CpmAzaliaPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  ACGE                  = %d\n", ACGE, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  APGE                  = %d\n", APGE, 0, 0, 0, 0, 0)

      //
      // Power on ACP
      // XOR operation is required as if any of AZAL or ACP is on no need to switch on again.
      //
      If (LNot (Xor (CpmAcpPresentState, CpmAzaliaPresentState))) {
        CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-1-End\n", 0, 0, 0, 0, 0, 0)
        Return ()
      } // End of If (LNot (Xor (CpmAcpPresentState, CpmAzaliaPresentState)))

      //
      // Enable Clock Gating based on setup option.
      //
      If (LEqual (ACGE, 0x1)) {
        //
        // For ACLK Clock Start
        //
        // Arg0:  ALIB Function 0x16 - send BIOSSMC_MSG_SetHardMinAclkByFreq
        // Arg1:  Aclk Frequency     - 0x0  : Aclk Deep sleep
        //                           - 0xC8 : Aclk frequency
        //
        Store (\_SB.ALIB (0x16, 0xC8), Local6)
      } // End of If (LEqual (ACGE, 0x1))

      //
      // Enable Power Gating based on setup option.
      //
      If (LEqual (APGE, 0x1)) {
        CpmSetPostCode (CpmTpAcpAzaliaPowerGateOnStart)
        If (LEqual (CpmAcpController, 1)) {
          Store (CpmReadPci (0, 8, 1, 0x19, 0, 8), Local0)              // Local0 = ACP Bus number
          Store (CpmReadPci32 (Local0, 0, 5, 0), Local1)                // Local1 = ACP DID
          If (Land (LNotEqual (Local0, 0x00), LNotEqual (Local0, 0xFF))) {
            If (LNotEqual (Local1, 0xFFFFFFFF)) {
                // Power ON ACP
                CpmAcpRemovePowerGating ()
            }
          }

        } ElseIf (LEqual (CpmAzaliaController, 1)) { // End of If (LEqual (CpmAcpController, 1))
          //
          // Add RSMU powergating logic
          //
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SW_CONTROL = 1;
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 1;//0:PD,1:PU,2:WR,3:RD
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SELECT = 0x3;
          //
          CpmWriteSmnRegister (0, 0, 0, mmRSMU_PGFSM_CONTROL_ACP, 0x303)

          //Get mmRSMU_PGFSM_STATUS_ACP at offest 0x0x900E790 and wait unitl it becomes 0x0;
          Store (WAIT_RESPONSE_TIMEOUT, Local5)
          Store (CpmReadSmnRegister (0, 0, 0, mmRSMU_PGFSM_STATUS_ACP), Local4)
          And (Local4, 0xF, Local4)
          While (LNotEqual (Local4, 0)) {
            Store (CpmReadSmnRegister (0, 0, 0, mmRSMU_PGFSM_STATUS_ACP), Local4)
            And (Local4, 0xF, Local4)
            Decrement (Local5)
            If (LEqual (Local5, 0)) {
              CpmDebugPrint ("  Wait mmRSMU_PGFSM_STATUS_ACP status timeout", 0, 0, 0, 0, 0, 0)
              Break
            }
            Stall (WAIT_STALL_US)
          }
        } // End of ElseIf (LEqual (CpmAzaliaController, 1))
        CpmSetPostCode (CpmTpAcpAzaliaPowerGateOnEnd)
      } // End of If (LEqual (APGE, 0x1))

      CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-End\n", 0, 0, 0, 0, 0, 0)
    } // End of Method (CpmPowerGateOn, 0)

    Method (CpmPowerGateOff, 0) {
      CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-Start\n", 0, 0, 0, 0, 0, 0)

      CpmDebugPrint ("  CpmAcpPresentState    = %d\n", CpmAcpPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  CpmAzaliaPresentState = %d\n", CpmAzaliaPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  APGE                  = %d\n", APGE, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  ACGE                  = %d\n", ACGE, 0, 0, 0, 0, 0)

      //
      // Power OFF ACP
      //
      If (LNot (Land (LEqual (CpmAcpPresentState, 0), LEqual (CpmAzaliaPresentState, 0)))) {
        CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-1-End\n", 0, 0, 0, 0, 0, 0)
        Return ()
      }

      //
      // Enable Power Gating based on setup option.
      //
      If (LEqual (APGE, 0x1)) {
        CpmSetPostCode (CpmTpAcpAzaliaPowerGateOffStart)

        If (LEqual (CpmAcpController, 1)) {
          Store (CpmReadPci (0, 8, 1, 0x19, 0, 8), Local0)              // Local0 = ACP Bus number
          Store (CpmReadPci32 (Local0, 0, 5, 0), Local1)                // Local1 = ACP DID
          If (Land (LNotEqual (Local0, 0x00), LNotEqual (Local0, 0xFF))) {
            If (LNotEqual (Local1, 0xFFFFFFFF)) {
              // Power Off ACP
              CpmAcpApplyPowerGating ()
            }
          }

        } ElseIf (Lequal (CpmAzaliaController, 1)) { // End of If (LEqual (CpmAcpController, 1))
          //
          // Add RSMU powergating logic
          //
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SW_CONTROL = 1;
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 0;//0:PD,1:PU,2:WR,3:RD
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SELECT = 0x3;
          //
          CpmWriteSmnRegister (0, 0, 0, mmRSMU_PGFSM_CONTROL_ACP, 0x301)

          //Get mmRSMU_PGFSM_STATUS_ACP at offest 0x0x900E790 and wait unitl it becomes 0xA;
          Store (WAIT_RESPONSE_TIMEOUT, Local5)
          Store (CpmReadSmnRegister (0, 0, 0, mmRSMU_PGFSM_STATUS_ACP), Local4)
          And (Local4, 0xF, Local4)
          While (LNotEqual (Local4, 0x0A)) {
            Store (CpmReadSmnRegister (0, 0, 0, mmRSMU_PGFSM_STATUS_ACP), Local4)
            And (Local4, 0xF, Local4)
            Decrement (Local5)
            If (LEqual (Local5, 0)) {
              CpmDebugPrint ("  Wait mmRSMU_PGFSM_STATUS_ACP status timeout", 0, 0, 0, 0, 0, 0)
              Break
            }
            Stall (WAIT_STALL_US)
          }
        } // End of ElseIf (Lequal (CpmAzaliaController, 1))
        CpmSetPostCode (CpmTpAcpAzaliaPowerGateOffEnd)
      } // End of If (LEqual (APGE, 0x1))

      //
      // Enable clock Gating based on setup option.
      //
      If (LEqual (ACGE, 0x1)) {
        //
        // For ACLK Clock Stop
        //
        // Arg0:  ALIB Function 0x16 - send BIOSSMC_MSG_SetHardMinAclkByFreq
        // Arg1:  Aclk Frequency     - 0x0  : Aclk Deep sleep
        //                           - 0xC8 : Aclk frequency
        //
        Store (\_SB.ALIB (0x16, 0x0), Local6)
      } // End of If (LEqual (ACGE, 0x1))

      CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-End\n", 0, 0, 0, 0, 0, 0)
    } // End of Method (CpmPowerGateOff, 0)

    Scope (\_SB.PCI0.PBC.ACP) {
      Method (_PS0, 0) {
        // Acquire lock to access common resources between ACP and Azalia
        Acquire (CpmAcpMutex, 0xFFFF)
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.ACP._PS0 CpmAcpPresentState = 1\n", 0, 0, 0, 0, 0, 0)

        Store (1, CpmAcpPresentState)
        CpmPowerGateOn ()

        // Release lock to access common resources between ACP and Azalia
        Release (CpmAcpMutex)
      }

      Method (_PS3, 0) {
        // Acquire lock to access common resources between ACP and Azalia
        Acquire (CpmAcpMutex, 0xFFFF)
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.ACP._PS3 CpmAcpPresentState = 0\n", 0, 0, 0, 0, 0, 0)

        Store (0, CpmAcpPresentState)
        CpmPowerGateOff ()

        // Release lock to access common resources between ACP and Azalia
        Release (CpmAcpMutex)
      }

      Method (MSG0, 3, Serialized) {
        CpmDebugPrint ("FEA-ASL-MSG0-Start\n", 0, 0, 0, 0, 0, 0)
        CpmSetPostCode (0x5511)

        OperationRegion (varM, SystemIO, 0x80, 0x4)
        Field (varM, DWordAcc, NoLock, Preserve) {
          varR, 32
        }

        If (LNotEqual (Arg2, 0x09)) {
          // Message to SMU
          CpmSetPostCode (0x5518)
          CpmWriteSmnRegister (0, 0, 0, 0x00058A74, arg0)                // MP1_SMN_C2PMSG_93         Response Register
          CpmWriteSmnRegister (0, 0, 0, 0x00058A54, arg1)                // MP1_SMN_C2PMSG_85         Argument Register
          CpmWriteSmnRegister (0, 0, 0, 0x00058A14, arg2)                // MP1_SMN_C2PMSG_69         Message Register
          Store (CpmReadSmnRegister (0, 0, 0, 0x00058A74), Local0)       // Read MP1_SMN_C2PMSG_93    Check Response Register
          While (LEqual (Local0, 0)) {
            Store (CpmReadSmnRegister (0, 0, 0, 0x00058A74), Local0)
            If (LNotEqual (Local0, 0)) {
              Break
            }
          }
          CpmSetPostCode (0x5519)
          Store (CpmReadSmnRegister (0, 0, 0, 0x00058A54), Local1)
          CpmDebugPrint ("FEA-ASL-MSG0-End-1-Return 0x%X\n", Local1, 0, 0, 0, 0, 0)
          // Return value from SMU
          return (Local1)
        }


        Name (MBOX, Buffer (4) {})

        // Read C2PMSG_28
        Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)
        Store (Local0, varR)
        Store (Local0, MBOX)

        CreateWordField (MBOX, 0, STAS)   // Status, Set by the target to indicate the execution status of last command
        CreateByteField (MBOX, 2, CMDI)   // Command ID set by host
        CreateField (MBOX, 24, 5, RESV)   // Reserved
        CreateBitField (MBOX, 29, RSET)   // Reset rerquest
        CreateBitField (MBOX, 30, RCOV)   // Recovery request
        CreateBitField (MBOX, 31, REDY)   // Set by the target to indicate the mailbox interface state.

        store (MBOX, varR)

        While (LOr (LNotEqual (REDY, 0x1), LNotEqual (CMDI, 0x00))) {
            Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)
            Store (Local0, MBOX)
            CpmSetPostCode (0x5516)
            Store (MBOX, varR)
        }

        Store (0, Local0)
        Store (Local0, MBOX)
        Store (0x0, REDY)
        Store (0x33, CMDI)

        CpmSetPostCode (0x5514)
        Store (MBOX, Local0)
        Store (Local0, varR)

        // Send command
        CpmWriteSmnRegister (0, 0, 0, 0x03810570, Local0)

        Sleep (1)
        CpmSetPostCode (0x5515)
        Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)
        Store (Local0, MBOX)
        Store (MBOX, varR)

        // Waiting for command done
        While (LNotEqual (CMDI, 0x00)) {
            Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)
            Store (Local0, MBOX)
            CpmSetPostCode (0x5517)
            Store (MBOX, varR)
        }

        If (LNotEqual (STAS, 0x00)) {
            // Command fail
            CpmSetPostCode (0x5513)
            CpmDebugPrint ("FEA-ASL-MSG0-Command Fail\n", 0, 0, 0, 0, 0, 0)
        }

        CpmSetPostCode (0x5512)
        CpmDebugPrint ("FEA-ASL-MSG0-End-2-Return 0x%X\n", Local0, 0, 0, 0, 0, 0)
        // Return value
        return (Local0)
      } // End of Method (MSG0, 3, Serialized)
    } // End of Scope (\_SB.PCI0.PBC.ACP)

    Scope (\_SB.PCI0.PBC.AZAL) {
      Method (_PS0, 0) {
        // Acquire lock to access common resources between ACP and Azalia
        Acquire (CpmAcpMutex, 0xFFFF)
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.AZAL._PS0 CpmAzaliaPresentState = 1\n", 0, 0, 0, 0, 0, 0)

        Store (1, CpmAzaliaPresentState)
        CpmPowerGateOn ()

        // Release lock to access common resources between ACP and Azalia
        Release (CpmAcpMutex)
      }

      Method (_PS3, 0) {
        // Acquire lock to access common resources between ACP and Azalia
        Acquire (CpmAcpMutex, 0xFFFF)
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.AZAL._PS3 CpmAzaliaPresentState = 0\n", 0, 0, 0, 0, 0, 0)

        Store (0, CpmAzaliaPresentState)
        CpmPowerGateOff ()

        // Release lock to access common resources between ACP and Azalia
        Release (CpmAcpMutex)
      }
    } // End of Scope (\_SB.PCI0.PBC.AZAL)
} // End of DefinitionBlock
