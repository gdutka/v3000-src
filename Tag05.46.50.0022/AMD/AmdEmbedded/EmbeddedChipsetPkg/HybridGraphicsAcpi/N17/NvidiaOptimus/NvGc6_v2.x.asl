/** @file

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/*
== Readme ==
For NVSR support, SBIOS has to adjust below code to meet platform:
- GPIO config:
    FB_EN:
        - Input pin for PCH
        - Set HostSW_Own: 1 = GPIO Driver Mode
        - Clear GPI General Purpose Events Status before boot into OS
        - ACPI code will config this pin for GPE event for NVSR
        - Change the L-event number to meet FB_EN GPIO pin#
    GPU_EVENT#:
        - Output pin for PCH, default HIGH
        - Set HostSW_Own: 1 = GPIO Driver Mode

- Suggest IBV to make the "NVSR_SUPPORT" token default disabled. Customer can enable that themselves if they are going to enable NVSR platform.
*/

Scope (PCI_SCOPE)
{
    Name (TGPC, Buffer(0x04) {0x00}) // save GPC parameter
    Name (TDGC, Zero) // The flag is for DFGC : Defer GC6 Enter/Exit until D3-Cold
    Name (DGCX, Zero)
    If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
    {
        Name (GEIF, Zero) // GEI support flag
        Name (GSWF, Zero) // GSW support flag
        Mutex (LG6O, 0) // expand LG6O Mutex to both GC6I and GC6O
    }
//
// Not Support
//
//     Method (GC6I, 0, Serialized)
//     {
//         If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//         {
//             Acquire (LG6O, 0xFFFF)
//         }

//         Store (DGPU_BRIDGE_SCOPE.LREN, DGPU_SCOPE.LTRE)
// //        Sleep (10)
//         Store (Zero, DGPU_BRIDGE_SCOPE.PMCT) // PCIE link disabling.

//         // If (LEqual (DGPU_SCOPE.NVSR, Zero)) // GC6 NVSR Not Support
//         // {
//         //     While (LNotEqual (\_SB.PCI0.P0LS, Zero))
//         //     {
//         //         Sleep (One)
//         //     }
//         // }
//         //
//         // access P0RM/P0AP after Link Disable successfully
//         // override "robust squelch mechanism"
//         // Program AFEOVR.RXSQDETOVR: Set 0:1:0 0xc20 BIT4 = 1 & BIT5 = 1: 11 = Squelch detector output overridden to 1
//         //
//         // Store (One, \_SB.PCI0.P0RM)
//         // Store (0x03, \_SB.PCI0.P0AP)
// /* Need to Transfer
//         While (LNotEqual (\_SB.GGIV (DGPU_SCOPE.FBEN), One))
//         {
//             If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//             {
//                 Sleep (0x32) // Use Stall() to reduce GC6 latency for NVSR case
//             }
//             Else
//             {
//                 Sleep (One) // Wait for for FB_EN to assert
//             }
//         }
// */
// //        Sleep (10)
// //         If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
// //         {
// // //            Store (1 , EC_SCOPE.EC6I) // notify EC to prepare GC6 entry.
// // //            Store (0 , EC_SCOPE.EC6O) // Clean GC6 exit bit in EC ram
// // //            SIOT (1)
// //             If (LEqual (GSWF, One))
// //             {
// //                 // bit[26:25] = RX Level/Edge Configuration, bit[23] = RX Invert,
// //                 // bit[19] = GPIO Input Route SCI
// //                 // For Falling Edge triggering
// //                 // bit[26:25] = 01b : Edge mode
// //                 // bit[23] = 8 RX-Invert mode
// //                 // FB_EN is HIGH now. Enable GPE on FB_EN falling edge
// //                 \_SB.SPC0 (DGPU_SCOPE.FBEN, Or (And (\_SB.GPC0 (DGPU_SCOPE.FBEN), 0xF977FFFF), 0x02880000)) // Drive '0', inversion, Routing can cause SCI
// //                 \_SB.SHPO (DGPU_SCOPE.FBEN, Zero) // Switch to ACPI mode to generate interrupt by GPI_INI_STS
// //                 \_SB.CAGS (DGPU_SCOPE.FBEN)       // Clear GPI General Purpose Events Status
// //                 Stall (0x64)
// //             }
//              Release (LG6O)
//          }
//     }
//
// Not Support
//
//     Method (GC6O, 0, Serialized)
//     {
//         If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//         {
//             Acquire (LG6O, 0xFFFF)
// //            SIOT (0)
//             CreateField (PCI_SCOPE.TGPC, 0x13, One, EGEI)
//             If (LEqual (GSWF, One))
//             {
//                 If (LEqual (And (\_SB.GPC0 (DGPU_SCOPE.FBEN), 0x00080000), 0x00080000)) // check bit[19] = GPIO Input Route SCI
//                 {
//                     // \_SB.SHPO (DGPU_SCOPE.FBEN, One) // switch to GPIO_Driver mode
//                     // \_SB.SPC0 (DGPU_SCOPE.FBEN, Or (And (\_SB.GPC0 (DGPU_SCOPE.FBEN), 0xFFF7FFFF), 0x00000000)) // Disable FB_EN SCI
//  /*
//                     If (LEqual (\_SB.GGIV (DGPU_SCOPE.FBEN), Zero))
//                     {
//                         // FB_EN is LOW already! Change polarity and clear GPE_STS
//                         \_SB.SPC0(DGPU_SCOPE.FBEN, Or(And(\_SB.GPC0(DGPU_SCOPE.FBEN), 0xFF7FFFFF), 0x00000000)) // Change to HIGH ACTIVE
//                         \_SB.CAGS(DGPU_SCOPE.FBEN) // Clear GPI General Purpose Events Status
//                     }
// */                    
//                 }
//                 Else
//                 {
//                     // This is not the first GC6O. return directly
//                     // Since it's not first GC6O. Clear SCI as well to prevent continuous SCI
//                     // \_SB.SPC0 (DGPU_SCOPE.FBEN, Or (And (\_SB.GPC0 (DGPU_SCOPE.FBEN), 0xFF7FFFFF), 0x00000000)) // No Invert FB_EN polarity
//                     // \_SB.CAGS (DGPU_SCOPE.FBEN) // Clear GPI General Purpose Events Status
//                     Release (LG6O)
//                     Return (One)
//                 }
//             }
//         }

//         // access P0RM/P0AP after Link Disable successfully
//         // Store (Zero, \_SB.PCI0.P0RM)
//         // Store (Zero, \_SB.PCI0.P0AP)

//         // Store (Zero, \_SB.PCI0.P0LD) // PCIE link enabling
//         // While (LLESS (\_SB.PCI0.P0LS, 7)) // Wait for dGPU PWR OK                        
//         // {
//         //   Sleep (One)
//         // }
//  /*       
//         If (LEqual (\_SB.GGIV (DGPU_SCOPE.FBEN), One))
//         {
//             \_SB.SGOV (DGPU_SCOPE.ENVT, Zero) // assert GPU_EVENT#
//             While (LNotEqual (\_SB.GGIV (DGPU_SCOPE.FBEN), Zero))
//             {
//                 If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//                 {
//                     Stall (0x32) // Use Stall() to reduce GC6 latency for NVSR case
//                 }
//                 Else
//                 {
//                     Sleep (One) // Wait for FB_EN to de-assert
//                 }
//             }
//             \_SB.SGOV (DGPU_SCOPE.ENVT, One) // de-assert GPU_EVENT#
//         }
// */
//         If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//         {
//             // FB_EN is HIGH already! Change polarity and clear GPE_STS
//             // \_SB.SPC0 (DGPU_SCOPE.FBEN, Or (And (\_SB.GPC0 (DGPU_SCOPE.FBEN), 0xFF7FFFFF), 0x00800000)) // Invert FB_EN polarity
//             // \_SB.CAGS (DGPU_SCOPE.FBEN) // Clear GPI General Purpose Events Status

//             // for self wakeup, write PCI register 0x00 to trigger INTR to RM
//             If (LEqual (ToInteger (EGEI), One))
//             {
//                 OperationRegion (PTMP, SystemMemory, PCI_SCOPE.DGDA, 0x04)
//                 Field (PTMP, DWordAcc, NoLock, Preserve)
//                 {
//                     TEMP, 8
//                 }
//                 Name (PTM1, Zero)
//                 Store (TEMP, PTM1)
//                 Store (PTM1, TEMP)
//             }
//         }

//         If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//         {
//             While (LLess (\_SB.PCI0.P0LS, 0x03))
//             {
//                 Stall (0x32) // Use Stall() to reduce GC6 latency for NVSR case
//             }
//         }
//         Else
//         {
//             While (LLess (\_SB.PCI0.P0LS, 0x07))
//             {
//                 Sleep (One)
//             }
//         }

//         Store (DGPU_SCOPE.LTRE, DGPU_BRIDGE_SCOPE.LREN)
//         Store (One, DGPU_BRIDGE_SCOPE.CEDR)

// //        Sleep(10)
// //        Store (1 , EC_SCOPE.EC6O) // notify EC to prepare GC6 exit.
// //        Store (0 , EC_SCOPE.EC6I) // Clean GC6 entry bit in EC ram

//         If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//         {
//             Release (LG6O)
//         }
//     }
}

Scope (DGPU_SCOPE)
{
    Method (CHDA, 0, Serialized)
    {
        Store (Zero, DGPU_SCOPE.HDAE)
    }
    Method (EHDA, 0, Serialized)
    {
        Store (One, DGPU_SCOPE.HDAE)
    }

    Method (_INI, 0, NotSerialized)
    {
        If (LAnd (LEqual (HGMD, 3), LNotEqual (IGDS, Zero))) { // HG Mode is dGPU only and IGD doesn't disable.
            Store (Zero, IGDS) // Integated Graphics Device Disabled
        }
//        Store (Zero, DGPU_SCOPE.HDAE) // Always disable HDA device after boot

        // // IBV_customize/OEM_customize: need to do below GPIO initialization during POST time +++
        // // Initialize setting for SCI of FB_EN - Disable, LOW_Active and clear GPE_STS
        // // Host Software Pad Ownership
        // \_SB.SHPO (FBEN, One) // HostSW_Own: 1 = GPIO Driver Mode
        // \_SB.SHPO (ENVT, One) // HostSW_Own: 1 = GPIO Driver Mode

        // // Pad Configuration DW0
        // // bit[11:10] = GPIO control the Pad, bit[9] = GPIO RX Disable, bit[8] = GPIO TX Disable, bit[0] = GPIO TX State
        // \_SB.SPC0 (FBEN, Or (And (\_SB.GPC0 (FBEN), 0xFFFFF0FF), 0x100))
        // \_SB.SPC0 (ENVT, Or (And (\_SB.GPC0 (ENVT), 0xFFFFF0FE), 0x201)) // GPU_EVENT default high

        If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
        {
            Store (One, PCI_SCOPE.GEIF) // Enable GEI
            Store (One, PCI_SCOPE.GSWF) // Enable GSW
            // \_SB.CAGS (FBEN)            // Clear GPI General Purpose Events Status
        }

        // IBV_customize/OEM_customize: need to do above GPIO initialization during POST time +++
    }

    // //
    // // Name: SIOT
    // // Description: Switch IO trap on 0x3CA and IO cycle of root port command register
    // // Input: Arg0 -> 0: Pause IO trap, Enable IO cycle of root port command register
    // //                1: Resume IO trap, clear IO cycle of root port command register
    // //
    // Method (SIOT, 1, Serialized)
    // {
    //   Store (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT1), local0)
    //   Store (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT2), local1)
    //   Store (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT3), local2)
    //   Store (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT4), local3)
    //   Store (Shiftright (And (local0, 0xFFFC), 2), local0)
    //   Store (Shiftright (And (local1, 0xFFFC), 2), local1)
    //   Store (Shiftright (And (local2, 0xFFFC), 2), local2)
    //   Store (Shiftright (And (local3, 0xFFFC), 2), local3)

    //   If (LEqual (Arg0, 1))
    //   {
    //       Store (0x06, CMDR)
    //       if (LEqual (local0, 0x3CA))
    //       {
    //           Store (OR (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT1), 0x1), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT1, local4)
    //       }
    //       if (LEqual (local1, 0x3CA))
    //       {
    //           Store (OR (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT2), 0x1), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG1, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT2, local4)
    //       }
    //       if (LEqual (local2, 0x3CA))
    //       {
    //           Store (OR (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT3), 0x1), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG2, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT3, local4)
    //       }
    //       if (LEqual (local3, 0x3CA))
    //       {
    //           Store (OR (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT4),0x1), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG3, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT4, local4)
    //       }
    //   }
    //   Else
    //   {
    //       if (LEqual (local0, 0x3CA))
    //       {
    //           Store (And (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT1 ), 0xFFFFFFFE), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG0, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT1, local4)
    //       }
    //       if (LEqual (local1, 0x3CA))
    //       {
    //           Store (And (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT2 ), 0xFFFFFFFE), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG1, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT2, local4)
    //       }
    //       if (LEqual (local2, 0x3CA))
    //       {
    //           Store (And (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT3 ), 0xFFFFFFFE), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG2, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT3, local4)
    //       }
    //       if (LEqual (local3, 0x3CA))
    //       {
    //           Store (And (PCRR (PID_DMI, R_PCH_PCR_DMI_IOT4 ), 0xFFFFFFFE), local4)
    //           PCRW (PID_PSTH, R_PCH_PCR_PSTH_TRPREG3, local4)
    //           PCRW (PID_DMI, R_PCH_PCR_DMI_IOT4, local4)
    //       }
    //       Store (0x07, CMDR)
    //   }
    // }

//     Method (NVJT, 4, SERIALIZED)
//     {
//         If (LLess (ToInteger (Arg1), JT_REVISION_ID))
//         {
//             Return (STATUS_ERROR_UNSPECIFIED)
//         }

//         Switch (ToInteger (Arg2))
//         {
//             //
//             // Sub-Function 0: JT_FUNC_SUPPORT
//             //
//             case (JT_FUNC_SUPPORT)
//             {
//                 //
//                 // Sub-Functions 0-6 are supported
//                 //
//                 // 6:1 = JT_FUNC_MDTL Supported
//                 // 5:1 = JT_FUNC_DISPLAYSTATUS Supported
//                 // 4:1 = JT_FUNC_PLATPOLICY not required
//                 // 3:1 = JT_FUNC_POWERCONTROL Supported
//                 // 2:0 = JT_FUNC_POLICYSELECT not required
//                 // 1:1 = JT_FUNC_CAPS Supported
//                 // 0:1 = JT_FUNC_SUPPORT Supported
// //                Return (Buffer(4) {0x3B, 0x00, 0x00, 0x00})
//                 //
//                 // Sub-Functions 0,1,3,4 are supported
//                 //
//                 Return (Buffer(4) {0x1B, 0x00, 0x00, 0x00})
//             }
//             //
//             // Sub-Function 1: JT_FUNC_CAPS
//             //
//             case (JT_FUNC_CAPS)
//             {
//                 MBGS ("JT_CAPS")
//                 Name (JTCA, Buffer(4) {0x00})
//                 CreateField (JTCA, 0,  1,  JTEN) // JT Enable
//                 CreateField (JTCA, 1,  2,  SREN) // NVSR Enable
//                 CreateField (JTCA, 3,  2,  PLPR) // Panel Power Rail
//                 CreateField (JTCA, 5,  1,  SRPR) // Self-Refresh Controller Power Rail
//                 CreateField (JTCA, 6,  2,  FBPR) // FB Power Rail
//                 CreateField (JTCA, 8,  2,  GUPR) // GPU Power Rail
//                 CreateField (JTCA, 10, 1,  GC6R) // GC6 ROM
//                 CreateField (JTCA, 11, 1,  PTRH) // Panic Trap Handler
//                 CreateField (JTCA, 13, 1,  MHYB) // MS Hybrid supported
//                 CreateField (JTCA, 14, 1,  RPCL) // Root Port control
//                 CreateField (JTCA, 15, 2,  GC6V) // GC6 Version
//                 CreateField (JTCA, 17, 1,  GEIS) // GC6 Exit ISR Support flag
//                 CreateField (JTCA, 18, 1,  GSWS) // GC6 Self Wakeup Support
//                 CreateField (JTCA, 20, 12, JTRV) // JT Revision

//                 Store (1, JTEN) // JT Enable
//                 Store (0, GC6R) // GC6 integrated ROM
//                 Store (1, RPCL) // Supports fine grain Root Port Control PLON, PRPC

//                 Store (0, FBPR)
//                 Store (0, GUPR)
//                 Store (0, PTRH) // No SMI Handler
//                 Store (1, MHYB) // IBV_customize/OEM_customize: 0: discrete NVSR support, 1: MShybrid
//                                 // Supports deferred GC6Enter/Exit in _PSx/_PRx, as per DFGC bits

//                 If (LEqual (DGPU_SCOPE.NVSR, One)) // GC6 NVSR Support
//                 {
//                     // CONDITION 1 : Following setting is for NVSR
//                     Store (0x0, SREN) // 0 = NVSR enabled.
//                     Store (0x2, PLPR) // PanelPowerRail = 2 , for NVSR support
//                     Store (0x1, SRPR) // SelfRefreshControllerPowerRail = 1, for NVSR support

//                     // for Selfwakeup support
//                     Store (0x01, GC6V) // Support GC6-A
//                     Store (PCI_SCOPE.GEIF, GEIS) // Enable GEI Support
//                     Store (PCI_SCOPE.GSWF, GSWS) // Enable GSW Support
//                 }
//                 Else
//                 {
//                     // CONDITION 2 : Following settings is for non NVSR
//                     Store (0x1,  SREN)    // 1 = NVSR disabled.
//                     Store (0x2,  PLPR)    // PanelPowerRail = 2 , for NVSR support
//                     Store (0x0,  SRPR)    // SelfRefreshControllerPowerRail = 0
//                 }

//                 Store (0x103, JTRV) // JT Rev

//                 Return (JTCA)
//             }
//             //
//             // Sub-Function 2: JT_FUNC_POLICYSELECT
//             //
//             case (JT_FUNC_POLICYSELECT)
//             {
//                 Return (STATUS_ERROR_UNSUPPORTED)
//             }
//             //
//             // Sub-Function 3: JT_FUNC_POWERCONTROL
//             //
//             case (JT_FUNC_POWERCONTROL)
//             {
//                 MBGS("JT_PowerC")
//                 CreateField (Arg3,  0, 3, GPPC) // GPU Power Control
//                 CreateField (Arg3,  4, 1, PLPC) // Panel Power Control
//                 CreateField (Arg3,  7, 1, ECOC) // Notify on complete
//                 CreateField (Arg3, 14, 2, DFGC) // Defer GC6 Enter/Exit until D3-Cold
//                 CreateField (Arg3, 16, 3, GPCX) // Deferred GC6 Exit Control
//                 Store (Arg3, PCI_SCOPE.TGPC) // Store GC6 control input for GC6I GC6O

//                 If (LOr (LNotEqual (ToInteger (GPPC), Zero), LNotEqual (ToInteger (DFGC), Zero)))
//                 {
//                     Store (ToInteger (DFGC), PCI_SCOPE.TDGC) // Defer GPC & GPCX to be processed when setting Device Power State (D0/D3-Hot/Cold)
//                     Store (ToInteger (GPCX), PCI_SCOPE.DGCX)
//                 }
//                 Name (JTPC, Buffer(4) {0x00})
//                 CreateField (JTPC, 0, 3, GUPS) // dGPU Power Status
//                 CreateField (JTPC, 3, 1, GPWO) // dGPU Power OK Status
//                 CreateField (JTPC, 7, 1, PLST) // Panel State

//                 If (LNotEqual (ToInteger (DFGC), Zero))
//                 {
//                     // dGPU On
//                     Store(One, GPWO)
//                     Store(One, GUPS)
//                     Return(JTPC)
//                 }
//                 If (LEqual (ToInteger (GPPC), One))
//                 {
//                     //
//                     // Enter GC6 and no Panel Self-Refresh
//                     // Panel is powered down with GPU
//                     //
//                     PCI_SCOPE.GC6I () // dGPU enter GC6 entry
//                     Store (One, PLST)  // Panel Status is powered off
//                     Store (Zero, GUPS) // dGPU Status is Completely Off
//                 }
//                 ElseIf (LEqual (ToInteger (GPPC), 0x02))
//                 {
//                     //
//                     // Enter GC6 and stay in Panel Self-Refresh
//                     // Panel and FB remain powered while GPU is shutdown
//                     //
//                     PCI_SCOPE.GC6I () // dGPU through EC enter GC6
//                     If (LEqual (ToInteger (PLPC), Zero)) // Panel remains powered while GPU is in GC6
//                     {
//                         Store (Zero, PLST) // Panel Status is powered on
//                     }
//                     Store (Zero, GUPS) // GC6 (dGPU off w/FB_CLAMP assert)
//                 }
//                 ElseIf (LEqual (ToInteger (GPPC), 0x03))
//                 {
//                     //
//                     // Exit GC6 and exit Self-Refresh
//                     // GPU is powered on, modeset and link train, GPU drives FB to the screen
//                     //
//                     PCI_SCOPE.GC6O () // dGPU through EC exit GC6
//                     //
//                     // The panel always powered on in Optimus platform
//                     //
//                     If (LNotEqual (ToInteger (PLPC), Zero)) // Panel remains powered while GPU is in GC6
//                     {
//                         //
//                         // Power On the Panel here
//                         //
//                         Store (Zero, PLST) // Panel is powered on
//                     }
//                     // dGPU On
//                     Store (One, GPWO)
//                     Store (One, GUPS)
//                 }
//                 ElseIf (LEqual (ToInteger (GPPC), 0x04))
//                 {
//                     //
//                     // Exit GC6 but remain in Self-Refresh
//                     // GPU is powered on, modeset and link train, SRC continues to drive the screen
//                     //
//                     PCI_SCOPE.GC6O () // dGPU through EC exit GC6
//                     //
//                     // The panel always powered on in Optimus platform
//                     //
//                     If (LNotEqual (ToInteger (PLPC), Zero)) // Panel remains powered while GPU is in GC6
//                     {
//                         Store (Zero, PLST) // Panel is powered on
//                     }
//                     // dGPU On
//                     Store (One, GPWO)
//                     Store (One, GUPS)
//                 }
//                 Else
//                 {
//                 /*Still need to modify    
//                     //
//                     // Get the current GPU GCx Sleep Status
//                     //
//                     If (LEqual(\_SB.GGIV (FBEN), Zero))
//                     {
//                         Store (One, GPWO)
//                         Store (One, GUPS)
//                     }
//                     Else
//                     {
//                         Store (Zero, GPWO)
//                         Store (0x3, GUPS)
//                     }
//                 */    
//                 }
//                 Return (JTPC)
//             }
//             //
//             // Sub-Function 4: JT_FUNC_PLATPOLICY
//             //
//             case (JT_FUNC_PLATPOLICY)
//             {
//                 CreateField (Arg3, 2, 1, PAUD) // AUD : Azalia Audio Device
//                 CreateField (Arg3, 3, 1, PADM) // ADM : Audio Disable Mask

//                 Store (Zero, Local0)
//                 If (LEqual (ToInteger (PADM), One))
//                 {
//                     If (LEqual (ToInteger (PAUD), Zero))
//                     {
//                         Store (Zero, OPTF) // IBV_customize: save a CMOS flag for S3/S4 resume HDA disable
//                     }
//                     Else
//                     {
//                         Store (One, OPTF) // IBV_customize: save a CMOS flag for S3/S4 resume HDA enable
//                     }
//                 }

//                 // IBV_customize:
//                 // SBIOS have to set the dGPU HDA control register (func:0,reg:0x488,bit:25) by this CMOS flag before
//                 // OS resume from S3/S4 (_WAK is acceptable). The HDA_CMOS_FLAG is also be used in PG00._ON

//                 ShiftLeft (DGPU_SCOPE.HDAE, 0x02, Local0)   // Returns the currnet status of the audio device
//                 Return (Local0)
//             }
//             //
//             // Sub-Function 5: JT_FUNC_DISPLAYSTATUS
//             //
//             case (JT_FUNC_DISPLAYSTATUS)
//             {
//                 // CreateField (Arg3, 31, 1, NCSM) // Check the next combination sequence mask bit 31.
//                 Store (0, Local0)
//                 // Force LID status return open
//                 Or (Local0, 0x00100010, Local0) // bit4 = 1 : LID info inlcuded, bit20 = 1 LID opened
//                 Return (Local0)
//             }
//             //
//             // Sub-Function 6: JT_FUNC_MDTL
//             //
//             case (JT_FUNC_MDTL)
//             {
//                 // Display Toggle List
//                 Name (TMP6, Package()
//                 {
//                     ones, 0x2C,        // LVDS
//                     ones, 0x2C,        // CRT
//                     ones, 0x2C,        // HDMI
//                     ones, ones, 0x2C,  // LVDS + CRT
//                     ones, ones, 0x2C,  // LVDS + HDMI
//                     ones, ones, 0x2C   // CRT  + HDMI
//                 })

//                 // Update Display Toggle List
//                 // Store (DID2, Index (TMP6, 0))  // LVDS
//                 // Store (DID1, Index (TMP6, 2))  // CRT
//                 // Store (DID4, Index (TMP6, 4))  // HDMI

//                 // Store (DID2, Index (TMP6, 6))  // LVDS + CRT
//                 // Store (DID1, Index (TMP6, 7))

//                 // Store (DID2, Index (TMP6, 9))  // LVDS + HDMI
//                 // Store (DID4, Index (TMP6, 10))

//                 // Store (DID1, Index (TMP6, 12)) // CRT + HDMI
//                 // Store (DID4, Index (TMP6, 13))

//                 Return (TMP6)
//             }
//         }
//         Return (STATUS_ERROR_UNSUPPORTED)
//     }
}

#if FixedPcdGetBool (PcdHgNvidiaOptimusGc6NvsrFeatureSupport)
Scope (_GPE)
{
    Method (_L08, 0x0, NotSerialized) // IBV_customize/OEM_customize: need to adjust the L-event number for FBEN pin
    {
        // If (LEqual (And (\_SB.GPC0(DGPU_SCOPE.FBEN), 0x00800000), 0x00800000)) // if RX Invert bit(RXINV) = 1
        // {
        //     PCI_SCOPE.GC6O()
        // }
    }
}
#endif