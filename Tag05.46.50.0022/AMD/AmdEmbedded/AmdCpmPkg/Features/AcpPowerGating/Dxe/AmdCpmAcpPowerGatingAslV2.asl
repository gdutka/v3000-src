/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "AcpPowerGatingAsl.aml",        // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMACPV2",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_ACP_POWER_GATING  1

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External (\_SB.PCI0.PBC.ACP, DeviceObj)
    External (\_SB.PCI0.PBC.AZAL, DeviceObj)
    External (\_SB.ALIB, MethodObj)

    Name (CpmAcpPresentState, 1)    // ACP  present state
    Name (CpmAzaliaPresentState, 1) // AZALIA present state
    Name (CpmAcpController, 1)      // ACP Controller Enable/Disable
    Name (APGE, 1)                  // ACP Power Gating Enable/Disable
    Name (ACGE, 1)                  // ACP Clock Gating Enable/Disable

    Mutex (CpmAcpMutex, 0)

    Method (CpmPowerGateOn, 0) {
      CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-Start\n", 0, 0, 0, 0, 0, 0)
      If (LEqual (CpmAcpController, 0)) {                             // ACP Controller is disable
        Store (0, CpmAcpPresentState)
      }

      CpmDebugPrint ("  CpmAcpPresentState    = %d\n", CpmAcpPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  CpmAzaliaPresentState = %d\n", CpmAzaliaPresentState, 0, 0, 0, 0, 0)
      If (LOr (LEqual (CpmAcpPresentState, 1), LEqual (CpmAzaliaPresentState, 1))) {
        // Power on ACP
        Store (CpmReadPci (0, 8, 1, 0x19, 0, 8), Local0)              // Local0 = ACP Bus number
        Store (CpmReadPci32 (Local0, 0, 5, 0), Local1)                // Local1 = ACP DID
        If (LEqual (CpmAcpController, 0)) {                           // ACP Controller is disable
          Store (0x15E21022, Local1)
        }

        If (LAnd (LNotEqual (Local0, 0x00), LNotEqual (Local0, 0xFF))) {
          If (LNotEqual (Local1, 0xFFFFFFFF)) {
            CpmSetPostCode (CpmTpAcpAzaliaPowerGateOnStart)

            // For ACLK Clock Start
            If (LNotEqual (ACGE, 0x0)) {                                    // Enable Clock Gating based on setup option.
              Store (\_SB.ALIB (0x12, 0xC8), Local2)
            }

            If (LNotEqual (APGE, 0x0)) {                                    // Enable Power Gating based on setup option.
              // For Power Gating
              // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SW_CONTROL = 1
              // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 1                  // 0:PD, 1:PU, 2:WR, 3:RD
              // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SELECT = 1
              CpmWriteSmnRegister (0, 0, 0, 0x900E784, 0x103)

              // Polling PGFSM (internal register) power status register
              Store (1, Local4)
              Store (100, Local5)
              While (LAnd (LNotEqual (Local4, 0), Local5)) {
                Store (CpmReadSmnRegister (0, 0, 0, 0x0900E784), Local4)    // Read RSMU_PGFSM_CONTROL_ACP
                Or (Local4, 0x00000006, Local4)                             // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 3
                And (Local4, 0xFFFFFF0F, Local4)                            // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_ADDR = 0
                CpmWriteSmnRegister (0, 0, 0, 0x0900E784, Local4)           // Write to RSMU_PGFSM_CONTROL_ACP
                Store (CpmReadSmnRegister (0, 0, 0, 0x0900E78C), Local4)    // Read RSMU_PGFSM_RD_DATA_ACP
                And (Local4, 3, Local4)
                Decrement (Local5)
                Stall (99)                                                  // 99us * 100 = 10ms
              }
              CpmWriteSmnRegister (0, 0, 0, 0x0900E784, 0)                  // Write to RSMU_PGFSM_CONTROL_ACP = 0

              // Power off ACP
              CpmSetPostCode (CpmTpAcpAzaliaPowerGateOnEnd)
            } // End of If (LNotEqual (APGE, 0x0))
          } // End of If (LNotEqual (Local1, 0xFFFFFFFF))
        } // End of If (LAnd (LNotEqual (Local0, 0x00), LNotEqual (Local0, 0xFF)))
      } // End of If (LOr (LEqual (CpmAcpPresentState, 1), LEqual (CpmAzaliaPresentState, 1)))

      CpmDebugPrint ("FEA-ASL-CpmPowerGateOn-End\n", 0, 0, 0, 0, 0, 0)
    } // End of Method (CpmPowerGateOn, 0)

    Method (CpmPowerGateOff, 0) {
      CpmDebugPrint ("FEA-ASL-CpmPowerGateOff-Start\n", 0, 0, 0, 0, 0, 0)
      If (LEqual (CpmAcpController, 0)) {                             // ACP Controller is disable
        Store (0, CpmAcpPresentState)
      }

      CpmDebugPrint ("  CpmAcpPresentState    = %d\n", CpmAcpPresentState, 0, 0, 0, 0, 0)
      CpmDebugPrint ("  CpmAzaliaPresentState = %d\n", CpmAzaliaPresentState, 0, 0, 0, 0, 0)
      If (LAnd (LEqual (CpmAcpPresentState, 0), LEqual (CpmAzaliaPresentState, 0))) {
        // Power on ACP
        Store (CpmReadPci (0, 8, 1, 0x19, 0, 8), Local0)              // Local0 = ACP Bus number
        Store (CpmReadPci32 (Local0, 0, 5, 0), Local1)                // Local1 = ACP DID
        If (LEqual (CpmAcpController, 0)) {                           // ACP Controller is disable
          Store (0x15E21022, Local1)
        }

        If (LAnd (LNotEqual (Local0, 0x00), LNotEqual (Local0, 0xFF))) {
          If (LNotEqual (Local1, 0xFFFFFFFF)) {
            CpmSetPostCode (CpmTpAcpAzaliaPowerGateOffStart)

            // For ACLK Clock Stop
            If (LNotEqual (ACGE, 0x0)) {                                    // Enable Clock Gating based on setup option.
              Store (\_SB.ALIB (0x12, 0x00), Local2)
            }

            If (LNotEqual (APGE, 0x0)) {                                    // Enable Power Gating based on setup option.
              // For Power Gating
              // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SW_CONTROL = 1
              // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 0                  // 0:PD, 1:PU, 2:WR, 3:RD
              // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_SELECT = 1
              CpmWriteSmnRegister (0, 0, 0, 0x0900E784, 0x101)

              // Polling PGFSM (internal register) power status register
              Store (1, Local4)
              Store (100, Local5)
              While (LAnd (LNotEqual (Local4, 2), Local5)) {
                Store (CpmReadSmnRegister (0, 0, 0, 0x0900E784), Local4)    // Read RSMU_PGFSM_CONTROL_ACP
                Or (Local4, 0x00000006, Local4)                             // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_CMD = 3
                And (Local4, 0xFFFFFF0F, Local4)                            // RSMU_PGFSM_CONTROL_ACP.RSMU_PGFSM_ADDR = 0
                CpmWriteSmnRegister (0, 0, 0, 0x0900E784, Local4)           // Write to RSMU_PGFSM_CONTROL_ACP
                Store (CpmReadSmnRegister (0, 0, 0, 0x0900E78C), Local4)    // Read RSMU_PGFSM_RD_DATA_ACP
                And (Local4, 3, Local4)
                Decrement (Local5)
                Stall (99)                                                  // 99us * 100 = 10ms
              }
              CpmWriteSmnRegister (0, 0, 0, 0x0900E784, 0)                  // Write to RSMU_PGFSM_CONTROL_ACP = 0

              // Power off ACP
              CpmSetPostCode (CpmTpAcpAzaliaPowerGateOffEnd)
            } // End of If (LNotEqual (APGE, 0x0))
          } // End of If (LNotEqual (Local1, 0xFFFFFFFF))
        } // End of If (LAnd (LNotEqual (Local0, 0x00), LNotEqual (Local0, 0xFF)))
      } // End of If (LAnd (LEqual (CpmAcpPresentState, 0), LEqual (CpmAzaliaPresentState, 0)))

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
