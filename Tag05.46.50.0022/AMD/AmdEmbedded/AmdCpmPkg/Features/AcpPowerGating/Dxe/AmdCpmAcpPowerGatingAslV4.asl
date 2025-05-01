/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#define mmACP_DSP0_RUNSTALL                     0x1240414
#define mmACP_PGFSM_CONTROL                     0x124101C
#define mmACP_PGFSM_STATUS                      0x1241020

#define mmRSMU_PGFSM_CONTROL_ACP                0x900E784
#define mmRSMU_PGFSM_STATUS_ACP                 0x900E790

#define mmMP1_SMN_C2PMSG_76                     0x58A30
#define mmMP1_SMN_C2PMSG_120                    0x58AE0
#define mmMP1_SMN_C2PMSG_121                    0x58AE4

#define mmACP_MSG_REG                           mmMP1_SMN_C2PMSG_76
#define mmACP_ARG_REG                           mmMP1_SMN_C2PMSG_120
#define mmACP_RESP_REG                          mmMP1_SMN_C2PMSG_121

#define WAIT_STALL_US                           99
#define WAIT_RESPONSE_TIMEOUT                   100 // 99us * 100 = 10ms

DefinitionBlock (
    "AcpPowerGatingAsl.aml",        // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMACPV4",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_ACP_POWER_GATING  1

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.PBC.ACP, DeviceObj)
    External(\_SB.PCI0.PBC.AZAL, DeviceObj)

    Name (CpmAcpPresentState, 1)    // ACP  present state
    Name (CpmAzaliaPresentState, 1) // AZALIA present state
    Name (CpmAcpController, 1)      // ACP    Controller Enable/Disable
    Name (CpmAzaliaController, 1)   // AZALIA Controller Enable/Disable
    Name (APGE, 1)                  // ACP    Power Gating Enable/Disable
    Name (ACGE, 1)                  // ACP    Clock Gating Enable/Disable

    Mutex (CpmAcpMutex, 0)

    Method (CpmAcpRemovePowerGating, 0) {

      CpmDebugPrint ("FEA-ASL-CpmAcpRemovePowerGating-Start\n", 0, 0, 0, 0, 0, 0)

      Store (CpmReadSmnRegister (0, 0, 0, mmACP_DSP0_RUNSTALL), Local0)
      CpmDebugPrint ("  ACP_DSP0_RUNSTALL   = %d\n", Local0, 0, 0, 0, 0, 0)

      If (LNotEqual (Local0, 0)) {
        //
        // Remove P2 power gating if HIFI 5 DSP is not running
        //
        CpmWriteSmnRegister (0, 0, 0, mmACP_PGFSM_CONTROL, 0x00000001)

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
    }

    Method (CpmAcpApplyPowerGating, 0) {

      CpmDebugPrint ("FEA-ASL-CpmAcpApplyPowerGating-Start\n", 0, 0, 0, 0, 0, 0)

      //
      // Access All registers in this method using SMN access
      //
      Store (CpmReadSmnRegister (0, 0, 0, mmACP_DSP0_RUNSTALL), Local0)
      CpmDebugPrint ("  ACP_DSP0_RUNSTALL   = %d\n", Local0, 0, 0, 0, 0, 0)

      If (LNotEqual (Local0, 0)) {
        //
        // Apply P2 power gating if DSP is not running
        //
        CpmWriteSmnRegister (0, 0, 0, mmACP_PGFSM_CONTROL, 0x00000000)

        //
        //wait until PGFSM status equals to 0x2
        //
        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        Store (CpmReadSmnRegister (0, 0, 0, mmACP_PGFSM_STATUS), Local1)
        While (LNotEqual (Local1, 0x2)) {
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
    }

    Method (CpmPowerGateOn, 0, Serialized) {

      // Acquire lock to access common resources between ACP and Azalia.
      // It is possible that ACP _PS0 is called while AZAL _PS3 is in executing and vice-versa.
      // That's why we think lock is good idea to have in these methods.
      CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-Start\n", 0, 0, 0, 0, 0, 0)

      Acquire (CpmAcpMutex, 0xFFFF)

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

        // Release lock to access common resources between ACP and Azalia
        Release (CpmAcpMutex)
        Return ()
      }

      //
      // Enable Clock Gating based on setup option.
      //
      If (LEqual (ACGE, 0x1)) {
        //
        // For ACLK Clock Start
        //
        Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)    // Read mmMP1_C2PMSG_121,    Check Read Response

        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        While (LEqual (Local4, 0)) {
          Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)
          Decrement (Local5)
          If (LEqual (Local5, 0)) {
            CpmDebugPrint ("  Wait ACLK Clock Start status timeout", 0, 0, 0, 0, 0, 0)
            Break
          }
          Stall (WAIT_STALL_US)
        }

        If (LEqual (Local4, 0x1)){
            CpmWriteSmnRegister (0, 0, 0, mmACP_RESP_REG, 0x0)              // mmMP1_SMN_C2PMSG_121 = 0x0,   Clear response register
            CpmWriteSmnRegister (0, 0, 0, mmACP_ARG_REG, 0xC8)              // mmMP1_SMN_C2PMSG_120 = 0xC8,  Write Clock Value
            CpmWriteSmnRegister (0, 0, 0, mmACP_MSG_REG, 0x3)               // mmMP1_SMN_C2PMSG_76 = 0x3,    Write Clock Type
            Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)    // Read mmMP1_C2PMSG_121,    Check Read Response

            Store (WAIT_RESPONSE_TIMEOUT, Local5)
            While (LEqual (Local4, 0)) {
            Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)
            Decrement (Local5)
            If (LEqual (Local5, 0)) {
                CpmDebugPrint ("  Wait ACLK Clock Start status timeout", 0, 0, 0, 0, 0, 0)
                Break
                }
            Stall (WAIT_STALL_US)
            }
        }  Else {
              CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-mmACP_RESP_REG = %d\n", Local4, 0, 0, 0, 0, 0)
        }
      }

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
        } ElseIf (LEqual (CpmAzaliaController, 1)) {

          //
          // Add RSMU powergating logic
          //
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SW_CONTROL = 1;
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 1;//0:PD,1:PU,2:WR,3:RD
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SELECT = 0x1;
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
        }
        CpmSetPostCode (CpmTpAcpAzaliaPowerGateOnEnd)
      }

      CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-End\n", 0, 0, 0, 0, 0, 0)

      //
      // Release lock to access common resources between ACP and Azalia
      //
      Release (CpmAcpMutex)
    }

    Method (CpmPowerGateOff, 0, Serialized) {

      //
      // Acquire lock to access common resources between ACP and Azalia
      //
      CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-Start\n", 0, 0, 0, 0, 0, 0)

      Acquire (CpmAcpMutex, 0xFFFF)

      CpmDebugPrint ("  CpmAcpPresentState    = %d\n", CpmAcpPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  CpmAzaliaPresentState = %d\n", CpmAzaliaPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  APGE                  = %d\n", APGE, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  ACGE                  = %d\n", ACGE, 0, 0, 0, 0, 0)

      //
      // Power OFF ACP
      //
      If (LNot (Land (LEqual (CpmAcpPresentState, 0), LEqual (CpmAzaliaPresentState, 0)))) {
        CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-1-End\n", 0, 0, 0, 0, 0, 0)
        Release (CpmAcpMutex)
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
        } ElseIf (Lequal (CpmAzaliaController, 1)) {

          //
          // Add RSMU powergating logic
          //
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SW_CONTROL = 1;
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 0;//0:PD,1:PU,2:WR,3:RD
          // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SELECT = 0x1;
          //
          CpmWriteSmnRegister (0, 0, 0, mmRSMU_PGFSM_CONTROL_ACP, 0x301)

          //Get mmRSMU_PGFSM_STATUS_ACP at offest 0x0x900E790 and wait unitl it becomes 0x2;
          Store (WAIT_RESPONSE_TIMEOUT, Local5)
          Store (CpmReadSmnRegister (0, 0, 0, mmRSMU_PGFSM_STATUS_ACP), Local4)
          And (Local4, 0xF, Local4)
          While (LNotEqual (Local4, 0x02)) {
            Store (CpmReadSmnRegister (0, 0, 0, mmRSMU_PGFSM_STATUS_ACP), Local4)
            And (Local4, 0xF, Local4)
            Decrement (Local5)
            If (LEqual (Local5, 0)) {
              CpmDebugPrint ("  Wait mmRSMU_PGFSM_STATUS_ACP status timeout", 0, 0, 0, 0, 0, 0)
              Break
            }
            Stall (WAIT_STALL_US)
          }
        }
        CpmSetPostCode (CpmTpAcpAzaliaPowerGateOffEnd)
      }

      //
      // Enable clock Gating based on setup option.
      //
      If (LEqual (ACGE, 0x1)) {

        // For ACLK Clock Stop

        Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)    // Read mmMP1_C2PMSG_121,    Check Read Response

        Store (WAIT_RESPONSE_TIMEOUT, Local5)
        While (LEqual (Local4, 0)) {
          Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)
          Decrement (Local5)
          If (LEqual (Local5, 0)) {
            CpmDebugPrint ("  Wait ACLK Clock Start status timeout", 0, 0, 0, 0, 0, 0)
            Break
          }
          Stall (WAIT_STALL_US)
        }

        If (LEqual (Local4, 0x1)){

            CpmWriteSmnRegister (0, 0, 0, mmACP_RESP_REG, 0x0)              // mmMP1_SMN_C2PMSG_121 = 0x0,  Clear response register
            CpmWriteSmnRegister (0, 0, 0, mmACP_ARG_REG, 0x0)               // mmMP1_SMN_C2PMSG_120 = 0x0,  Write Clock Value
            CpmWriteSmnRegister (0, 0, 0, mmACP_MSG_REG, 0x3)               // mmMP1_SMN_C2PMSG_76 = 0x3,   Write Clock Type
            Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)    // Read mmMP1_C2PMSG_121,   Check Read Response

            Store (WAIT_RESPONSE_TIMEOUT, Local5)
            While (LEqual (Local4, 0)) {
              Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)
              Decrement (Local5)
              If (LEqual (Local5, 0)) {
                CpmDebugPrint ("  Wait mmRSMU_PGFSM_STATUS_ACP status timeout", 0, 0, 0, 0, 0, 0)
                Break
              }
              Stall (WAIT_STALL_US)
            }
        } Else {
              CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-mmACP_RESP_REG = %d\n", Local4, 0, 0, 0, 0, 0)
        }
      }

      CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-End\n", 0, 0, 0, 0, 0, 0)

      // Release lock to access common resources between ACP and Azalia1
      Release (CpmAcpMutex)
    }

    Scope (\_SB.PCI0.PBC.ACP) {
      Method (_PS0, 0) {
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.ACP._PS0 CpmAcpPresentState = 1\n", 0, 0, 0, 0, 0, 0)
        Store (1, CpmAcpPresentState)
        CpmPowerGateOn ()
      }
      Method (_PS3, 0) {
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.ACP._PS3 CpmAcpPresentState = 0\n", 0, 0, 0, 0, 0, 0)
        Store (0, CpmAcpPresentState)
        CpmPowerGateOff ()
      }
    }

    Scope (\_SB.PCI0.PBC.AZAL) {
      Method (_PS0, 0) {
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.AZAL._PS0 CpmAzaliaPresentState = 1\n", 0, 0, 0, 0, 0, 0)
        Store (1, CpmAzaliaPresentState)
        CpmPowerGateOn ()
      }
      Method (_PS3, 0) {
        CpmDebugPrint ("FEA-ASL-\\_SB.PCI0.PBC.AZAL._PS3 CpmAzaliaPresentState = 0\n", 0, 0, 0, 0, 0, 0)
        Store (0, CpmAzaliaPresentState)
        CpmPowerGateOff ()
      }
    }

    Scope (\_SB.PCI0.PBC.ACP) {
     Method (MSG0, 3, Serialized)
      {
        CpmDebugPrint ("FEA-ASL-MSG0-Start\n", 0, 0, 0, 0, 0, 0)
        CpmSetPostCode(0x5511)

        OperationRegion(varM, SystemIO, 0x80, 0x4)
         Field(varM, DWordAcc, NoLock, Preserve) {
            varR, 32
        }

        if (LNotEqual (Arg2, 0x09))
        {
              // Message to SMU
            CpmSetPostCode(0x5518)
            Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)    // Read mmMP1_C2PMSG_121,    Check Read Response

            Store (WAIT_RESPONSE_TIMEOUT, Local5)
            While (LEqual (Local4, 0)) {
              Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local4)
              Decrement (Local5)
              If (LEqual (Local5, 0)) {
                CpmDebugPrint ("  Wait ACLK Clock Start status timeout", 0, 0, 0, 0, 0, 0)
                Break
              }
              Stall (WAIT_STALL_US)
            }
            If (LEqual (Local4, 0x1)){

              CpmWriteSmnRegister (0, 0, 0, mmACP_RESP_REG, arg0)               // mmMP1_SMN_C2PMSG_121 = 0x0,  Clear response register
              CpmWriteSmnRegister (0, 0, 0, mmACP_ARG_REG, arg1)                // mmMP1_SMN_C2PMSG_120 = arg1,  Write Clock Value
              CpmWriteSmnRegister (0, 0, 0, mmACP_MSG_REG, arg2)                // mmMP1_SMN_C2PMSG_76 = 0x3,   Write Clock Type
              Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local0)      // Read mmMP1_SMN_C2PMSG_121,   Check Read Response
              While (LEqual (Local0, 0))
              {
                Store (CpmReadSmnRegister (0, 0, 0, mmACP_RESP_REG), Local0)
                if (LNotEqual (Local0, 0)) {
                  break
                }
              }
              CpmSetPostCode(0x5519)
              Store (CpmReadSmnRegister (0, 0, 0, mmACP_ARG_REG), Local1)
              CpmDebugPrint ("FEA-ASL-MSG0-End-1-Return 0x%X\n", Local1, 0, 0, 0, 0, 0)
              return (Local1)  // Return value from SMU
              }
              Else {
                  CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-mmACP_RESP_REG = %d\n", Local4, 0, 0, 0, 0, 0)
                  return(0xFFFFFFFF)
            }
        }

        Name(MBOX, Buffer (4) {})

        //read C2PMSG_28
        Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)

        store (Local0, varR)

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
            CpmSetPostCode(0x5516)
            store (MBOX, varR)
        }

        Store (0, Local0)
        Store (Local0, MBOX)
        Store (0x0, REDY)
        Store (0x33, CMDI)

        CpmSetPostCode(0x5514)
        Store (MBOX, Local0)
        Store (Local0, varR)
        CpmWriteSmnRegister (0, 0, 0, 0x03810570, Local0)       // Send command

        Sleep (1)
        CpmSetPostCode(0x5515)
        Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)
        Store (Local0, MBOX)
        Store (MBOX, varR)

        //waiting for command done
        While (LNotEqual (CMDI, 0x00)) {
            Store (CpmReadSmnRegister (0, 0, 0, 0x03810570), Local0)
            Store (Local0, MBOX)
            CpmSetPostCode(0x5517)
            Store (MBOX, varR)
        }

        if (LNotEqual (STAS, 0x00)) {
            CpmSetPostCode(0x5513)      // Command fail
            CpmDebugPrint ("FEA-ASL-MSG0-Command Fail\n", 0, 0, 0, 0, 0, 0)
        }

        CpmSetPostCode(0x5512)
        CpmDebugPrint ("FEA-ASL-MSG0-End-2-Return 0x%X\n", Local0, 0, 0, 0, 0, 0)
        return (Local0)                 // Return value
      }
    }
}
