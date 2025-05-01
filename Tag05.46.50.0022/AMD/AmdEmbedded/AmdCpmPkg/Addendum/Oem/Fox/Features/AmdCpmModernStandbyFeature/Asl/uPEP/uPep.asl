/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
DefinitionBlock (
    "uPEP.aml",                 // Output file
    "SSDT",                     // Signature
    0x01,                       // SSDT Revision
    "AMD",                      // OEM ID
    "UPEP",                     // OEM Table ID
    0x1                         // OEM Revision
    )
{
    #define CPM_MS_UPEP_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External (\_SB.PCI0, DeviceObj)
    External (\_SB.PCI0.GPP7.DEV0, DeviceObj)
    External (\P1D3, IntObj)
    External (\P3D3, IntObj)
    External (\P4D3, IntObj)
    External (\H0D3, IntObj)
    External (\F0D3, IntObj)
    External (\F1D3, IntObj)
    External (\F3D3, IntObj)
    External (\F5D3, IntObj)
    External (\F6D3, IntObj)
    External (\G0D3, IntObj)
    External (\G3D3, IntObj)
    External (\G5D3, IntObj)
    External (CpmReadSmnRegister, MethodObj)
    External (CpmWriteSmnRegister, MethodObj)

    //
    // Method (UPHK, 1, Serialized) - uPEP hook of EC
    //   EC knows OS DRIP and display on/off status through this hook
    //   For example EC can set KB backlight status depend on display on/off
    //   CRB EC also relies display-on for eSPI VW and IRQ#1 restore
    //
    //   !!NOTE!!
    //   Customer has to implement themselves version of UPHK.
    //   Please remove all of reference on UPHK for platforms that don't have
    //   method UPHK in \_SB.PCI0.LPC0.EC0 scope
    //
    //   Arguments definition of this method is following convention of the
    //   UUID of 11E00D56-CE64-47CE-837B-1F898F9AA461. i.e.
    //      3 - Display OFF notification
    //      4 - Display ON  notification
    //      5 - Low Power State Entry notification
    //      6 - Low Power State Exit notification.
    //      7 - Modern Standby Entry notification
    //      8 - Modern Standby Exit notification
    //
    External (\_SB.PCI0.LPC0.EC0.UPHK, MethodObj)

    Scope (\_SB.PCI0) {
        Name (_DEP, Package (1) {
            \_SB.PEP
        })
    }

    Scope (\_SB) {
        // Declare AMD uPEP device, this is needed for loading the AMD uPEP driver
        Device (PEP) {
            Name (_HID, "AMDI0007")
            Name (_CID, EISAID ("PNP0D80"))
            Name (_UID, One)
            Name (WWSW, 0xFF)

            Name (DEVL, Package (3) {
                0,   // Structure revision
                42,  // 47 devices need be monitored, 16 processors, 7 GPP bridge, 21 devices, 3 UART/I2C
                Package (42) {
                    Package (4) {0, "\\_SB.PCI0.GP11", 0, 0},   // GP11, D3 state
                    Package (4) {0, "\\_SB.PCI0.GP12", 0, 0},   // GP12, D3 state
                    Package (4) {0, "\\_SB.PCI0.GP17", 0, 0},   // GP17, D3 state
                    Package (4) {0, "\\_SB.PCI0.GP19", 0, 0},   // GP19, D3 state
                    Package (4) {1, "\\_SB.PCI0.GPP0", 0, 3},   // GPP0, D3 state
                    Package (4) {1, "\\_SB.PCI0.GPP2", 0, 3},   // GPP2, D3 state
                    Package (4) {1, "\\_SB.PCI0.GPP6", 2, 1},   // GPP4, F1 state

                    Package (4) {1, "\\_SB.PCI0.GP11.SWUS", 0, 3},  // GP11.SWUS, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP12.SWUS", 0, 3},  // GP12.SWUS, D3 state
                    Package (4) {1, "\\_SB.PCI0.GPP1.NVME", 0, 3},  // NVME, D3 state
                    //Package (4) {1, "\\_SB.PCI0.GPP2.WWAN", 0, 3},  // WWAN, D3 state
                    //Package (4) {1, "\\_SB.PCI0.GPP5.RTL8", 0, 3},  // RTL8, D3 state
                    //Package (4) {1, "\\_SB.PCI0.GPP5.RUSB", 0, 3},  // RTL Virtaul USB, D3 state
                    //Package (4) {1, "\\_SB.PCI0.GPP6.WLAN", 0, 3},  // TRL8, D3 state
                    Package (4) {1, "\\_SB.PCI0.GPP8.NVME", 0, 3},  // NVME, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP17.VGA",  0, 3},  // iGPU
                    Package (4) {1, "\\_SB.PCI0.GP17.AZAL", 0, 3},  // Device 0, Function 6 AZALIA
                    Package (4) {1, "\\_SB.PCI0.GP17.ACP",  0, 3},  // Device 0, Function 5
                    Package (4) {1, "\\_SB.PCI0.GP17.HDAU", 0, 3},  // Device 0, Function 1 Display HD Audio Controller
                    Package (4) {1, "\\_SB.PCI0.GP17.XHC0", 0, 3},  // XHCI0, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP17.XHC1", 0, 3},  // XHCI1, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP17.XHC1.RHUB.PRT2.BUTH", 0, 3}, // BT, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP18.SATA", 0, 3},  // SATA, D3 state, is d3-cold needed
                    Package (4) {1, "\\_SB.PCI0.GP19.XHC2", 0, 3},  // XHCI2, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP19.XHC3", 0, 3},  // XHCI3, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP19.XHC4", 0, 3},  // XHCI4, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP19.NHI0", 0, 3},  // CIO0, D3 state
                    Package (4) {1, "\\_SB.PCI0.GP19.NHI1", 0, 3},  // CIO1, D3 state

                    //Package (4) {1, "\\_SB.I2CC.NFC1",      0, 3},  // I2CC, D3 state -s0 power rail
                    Package (4) {1, "\\_SB.FAR0",           0, 3},  // UAR1, D3 state -Disabled(BY A0AC) during post as it's header on board
                    Package (4) {1, "\\_SB.FAR1",           0, 3},  // UAR2, D3 state -Disabled(BY A0AC) during post as it's header on board

                    Package (4) {1, "\\_SB.PLTF.C000", 1, 3},   // Core 0, C3 state
                    Package (4) {1, "\\_SB.PLTF.C001", 1, 3},   // Core 1, C3 state
                    Package (4) {1, "\\_SB.PLTF.C002", 1, 3},   // Core 2, C3 state
                    Package (4) {1, "\\_SB.PLTF.C003", 1, 3},   // Core 3, C3 state
                    Package (4) {1, "\\_SB.PLTF.C004", 1, 3},   // Core 4, C3 state
                    Package (4) {1, "\\_SB.PLTF.C005", 1, 3},   // Core 5, C3 state
                    Package (4) {1, "\\_SB.PLTF.C006", 1, 3},   // Core 6, C3 state
                    Package (4) {1, "\\_SB.PLTF.C007", 1, 3},   // Core 7, C3 state
                    Package (4) {1, "\\_SB.PLTF.C008", 1, 3},   // Core 8, C3 state
                    Package (4) {1, "\\_SB.PLTF.C009", 1, 3},   // Core 9, C3 state
                    Package (4) {1, "\\_SB.PLTF.C00A", 1, 3},   // Core A, C3 state
                    Package (4) {1, "\\_SB.PLTF.C00B", 1, 3},   // Core B, C3 state
                    Package (4) {1, "\\_SB.PLTF.C00C", 1, 3},   // Core C, C3 state
                    Package (4) {1, "\\_SB.PLTF.C00D", 1, 3},   // Core D, C3 state
                    Package (4) {1, "\\_SB.PLTF.C00E", 1, 3},   // Core E, C3 state
                    Package (4) {1, "\\_SB.PLTF.C00F", 1, 3},   // Core F, C3 state
                }
            }) // End of Name (DEVL, Package ()

            Method (_STA, 0, NotSerialized) {
                If (CpmIsWin8()) {
                    // If Win8 & Connected Standby is enabled
                    CpmDebugPrint ("  OEM-ASL-\\_SB.PEP._STA = 0xF\n", 0, 0, 0, 0, 0, 0)
                    Return (0x0F)
                } Else {
                    CpmDebugPrint ("  OEM-ASL-\\_SB.PEP._STA = 0x0\n", 0, 0, 0, 0, 0, 0)
                    Return (0x0)
                }
            } // Method (_STA, 0, NotSerialized)

            //
            //  Modeify uPEP DevList
            //  Arg0 - Integer - Enabled   - 0 = Disable, 1 = Enable. uPEP will ignore the device entry if set to 0.
            //  Arg1 - String  - DeviceStr - ACPI device name string
            //  Arg2 - Integer - Flag      - 0 = D State (Device object), 1 = C State (Processor object), 2 = F state (Device Object).
            //  Arg3 - Integer - MinState  - 0 = D0/C0/F0, 1 = D1/C1/F1, 2 = D2/C2/F2, 3 = D3/C3.
            //
            Method (MPEP, 4) {
                Store (DerefOf (Index (DEVL, 0x01)), Local1)
                Store (0x00, Local0)
                While (LLess (Local0 , Local1)) {
                    // Check the device path
                    If (LEqual (DerefOf (Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x01)), Arg1)) {
                        // Change power state
                        Store (Arg0, Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x00))
                        Store (Arg2, Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x02))
                        Store (Arg3, Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x03))
                        Break
                    }
                    Increment (Local0)
                }
            } // End of Method (MPEP, 4)

            Method (_DSM, 0x4, Serialized) {
                CreateDWordField (Arg0, 0, UID0)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PEP._DSM (UUID 0x%X, 0x%X, 0x%X)\n", UID0, Arg1, Arg2, 0, 0, 0)
                If (LEqual (Arg0, ToUUID ("E3F32452-FEBC-43CE-9039-932122D37721"))) {
                    Switch (ToInteger (Arg2)) {
                        // Function 0: Query function, return based on revision
                        Case (0) {
                            // DSM Revision
                            Switch (ToInteger (Arg1)) {
                                // Revision 0: Function 1 supported
                                Case (0) {
                                    CpmDebugPrint ("    Return (Buffer (1) {0x03})\n", 0, 0, 0, 0, 0, 0)
                                    Return (Buffer (1) {0x03})
                                }
                                Case (1) {
                                    CpmDebugPrint ("    Return (Buffer (1) {0x03})\n", 0, 0, 0, 0, 0, 0)
                                    Return (Buffer (1) {0x03})
                                }
                                // Revision 2 is used for Linux, supporting function 1,2,3,4,5
                                Case (2) {
                                    CpmDebugPrint ("    Return (Buffer (1) {0x3F})\n", 0, 0, 0, 0, 0, 0)
                                    Return (Buffer (1) {0x3F})
                                }
                                // Revision 3+: no functions supported
                                Default {
                                    CpmDebugPrint ("    Return (Buffer (1) {0x00})\n", 0, 0, 0, 0, 0, 0)
                                    Return (Buffer (1) {0x00})
                                }
                            }
                        }
                        // Function 1 : Query device dependency list
                        Case (1) {
                            If (CondRefOf (\P4D3)) {
                                If (LNotEqual (\P4D3, 0x55)) {
                                    MPEP (1, "\\_SB.PCI0.GP11", 0, 3)
                                    MPEP (1, "\\_SB.PCI0.GP12", 0, 3)
                                }
                            }
                            If (CondRefOf (\P1D3)) {
                                If (LNotEqual (\P1D3, 0x55)) {
                                    MPEP (1, "\\_SB.PCI0.GP17", 0, 3)
                                }
                            }
                            If (CondRefOf (\P3D3)) {
                                If (LNotEqual (\P3D3, 0x55)) {
                                    MPEP (1, "\\_SB.PCI0.GP19", 0, 3)
                                }
                            }
                            If (CondRefOf (\H0D3)) {
                                If (LNotEqual (\H0D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP11.SWUS", 0, 0)
                                    MPEP (0, "\\_SB.PCI0.GP12.SWUS", 0, 0)
                                }
                            }
                            If (CondRefOf (\F0D3)) {
                                If (LNotEqual (\F0D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP17.VGA", 0, 0)
                                }
                            }
                            If (CondRefOf (\F1D3)) {
                                If (LNotEqual (\F1D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP17.HDAU", 0, 0)
                                }
                            }
                            If (CondRefOf (\F3D3)) {
                                If (LNotEqual (\F3D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP17.XHC0", 0, 0)
                                    MPEP (0, "\\_SB.PCI0.GP17.XHC1", 0, 0)
                                }
                            }
                            If (CondRefOf (\F5D3)) {
                                If (LNotEqual (\F5D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP17.ACP", 0, 0)
                                }
                            }
                            If (CondRefOf (\F6D3)) {
                                If (LNotEqual (\F6D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP17.AZAL", 0, 0)
                                }
                            }
                            If (CondRefOf (\G0D3)) {
                                If (LNotEqual (\G0D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP19.XHC2", 0, 0)
                                }
                            }
                            If (CondRefOf (\G3D3)) {
                                If (LNotEqual (\G3D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP19.XHC3", 0, 0)
                                    MPEP (0, "\\_SB.PCI0.GP19.XHC4", 0, 0)
                                }
                            }
                            If (CondRefOf (\G5D3)) {
                                If (LNotEqual (\G5D3, 0x4)) {
                                    MPEP (0, "\\_SB.PCI0.GP19.NHI0", 0, 0)
                                    MPEP (0, "\\_SB.PCI0.GP19.NHI1", 0, 0)
                                }
                            }
                            If (LEqual (WWSW, 0x1)) {
                                CpmDebugPrint ("  OEM-ASL-Modify uPep DevList to enable \\_SB.PCI0.GPP2 D3 for Fibocom L860 R+\n", 0, 0, 0, 0, 0, 0)
                                MPEP (1, "\\_SB.PCI0.GPP2", 0, 3)
                            }

                            ToInteger (CpmDebugPrintSerialAddr, Local7)
                            If (LNotEqual (Local7, 0)) {
                                CpmDebugPrint ("    OEM-ASL-DEVL:\n", 0, 0, 0, 0, 0, 0)
                                Store (DerefOf (Index (DEVL, 0x01)), Local1)
                                Store (0x00, Local0)
                                While (LLess (Local0 , Local1)) {
                                    Store (DerefOf (Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x00)), Local2)
                                    Store (DerefOf (Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x01)), Local3)
                                    Store (DerefOf (Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x02)), Local4)
                                    Store (DerefOf (Index (DerefOf (Index (DerefOf (Index (DEVL, 0x02)), Local0)), 0x03)), Local5)
                                    CpmDebugPrint ("      %d - %d %S %d %d\n", ToInteger (Local0), Local2, Local3, Local4, Local5, 0)
                                    Increment (Local0)
                                }
                            }

                            CpmDebugPrint ("    Return (DEVL)\n", 0, 0, 0, 0, 0, 0)
                            Return (DEVL)
                        }
                        // Functions 2: Linux Display Off Notification
                        Case (2) {
                            CpmSetPostCode (CpmTpMsuPEPDisplayOffHook)
                            If (CondRefOf (\_SB.PCI0.LPC0.EC0.UPHK)) {
                                \_SB.PCI0.LPC0.EC0.UPHK (3)  // Convert to Windows convention ID.
                            }
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 3: Linux Display On Notification
                        Case (3) {
                            CpmSetPostCode (CpmTpMsuPEPDisplayOnHook)
                            If (CondRefOf (\_SB.PCI0.LPC0.EC0.UPHK)) {
                                \_SB.PCI0.LPC0.EC0.UPHK (4)  // Convert to Windows convention ID.
                            }
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 4: Linux DRIPS Entry Notification
                        Case (4) {
                            CpmSetPostCode (CpmTpMsuPEPDripsEntryHook)
                            If (CondRefOf (\_SB.PCI0.LPC0.EC0.UPHK)) {
                                \_SB.PCI0.LPC0.EC0.UPHK (5)  // Map to Windows convention IDs.
                                Sleep(1)
                                \_SB.PCI0.LPC0.EC0.UPHK (7)  // Map to Windows convention IDs.
                            }
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 5: Linux DRIPS Exit Notification
                        Case (5) {
                            CpmSetPostCode (CpmTpMsuPEPDripsExitHook)
                            If (CondRefOf (\_SB.PCI0.LPC0.EC0.UPHK)) {
                                \_SB.PCI0.LPC0.EC0.UPHK (8)  // Map to Windows convention IDs.
                                Sleep(1)
                                \_SB.PCI0.LPC0.EC0.UPHK (6)  // Map to Windows convention IDs.
                            }
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        Default {
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                    } // End of Switch (ToInteger (Arg2))

                }  ElseIf (LEqual (Arg0, ToUUID ("11E00D56-CE64-47CE-837B-1F898F9AA461"))) {
                    //
                    // Method (UPHK, 1, Serialized) - uPEP hook of EC
                    //   EC knows OS DRIP and display on/off status through this hook
                    //   For example EC can set KB backlight status depend on display on/off
                    //   CRB EC also relies display-on for eSPI VW and IRQ#1 restore
                    //
                    //   !!NOTE!!
                    //   Customer has to implement themselves version of UPHK.
                    //   Please remove all of reference on UPHK for platforms that don't have
                    //   method UPHK in \_SB.PCI0.LPC0.EC0 scope
                    //
                    //   Arguments definition of this method is following convention of the
                    //   UUID of 11E00D56-CE64-47CE-837B-1F898F9AA461. i.e.
                    //      3 - Display OFF notification
                    //      4 - Display ON  notification
                    //      5 - Low Power State Entry notification
                    //      6 - Low Power State Exit notification.
                    //      7 - Modern Standby Entry notification
                    //      8 - Modern Standby Exit notification
                    //
                    If (CondRefOf (\_SB.PCI0.LPC0.EC0.UPHK)) {
                        \_SB.PCI0.LPC0.EC0.UPHK (ToInteger (Arg2))
                    }

                    Switch (ToInteger (Arg2)) {
                        // Function 0: Query function, return based on revision
                        Case (0) {
                            // DSM Revision
                            Switch (ToInteger (Arg1)) {
                                // Revision 0: Function 3-8 supported
                                Case (0) {
                                    CpmDebugPrint ("    Return (Buffer (2) {0xF9, 0x01})\n", 0, 0, 0, 0, 0, 0)
                                    Return (Buffer (2) {0xF9, 0x01})
                                }
                                // Revision 1+: no functions supported
                                Default {
                                    CpmDebugPrint ("    Return (Buffer (1) {0x00})\n", 0, 0, 0, 0, 0, 0)
                                    Return (Buffer (1) {0x00})
                                }
                            }
                        }
                        // Functions 3: Display Off Notification
                        Case (3) {
                            CpmSetPostCode (CpmTpMsuPEPDisplayOffNotification)
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 4: Display On Notification
                        Case (4) {
                            CpmSetPostCode (CpmTpMsuPEPDisplayOnNotification)
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 5: Lowest Power State Entry Notification
                        Case (5) {
                            CpmSetPostCode (CpmTpMsuPEPLPSEntryNotification)
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 6: Lowest Power State Exit Notification
                        Case (6) {
                            CpmSetPostCode (CpmTpMsuPEPLPSExitNotification)
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        // Functions 7: Modern Standby Entry Notification
                        Case (7) {
                            CpmSetPostCode (CpmTpMsuPEPMoStEntryNotification)
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Store(CpmReadSmnRegister(0,0,0,0x13F01308), local0)
                            OR(local0, 1, local0)
                            CpmWriteSmnRegister(0,0,0, 0x13F01308, local0)

                            Return (0x00)
                        }
                        // Functions 8: Modern Standby Exit Notification
                        Case (8) {
                            CpmSetPostCode (CpmTpMsuPEPMoStExitNotification)
                            If (CondRefOf (\_SB.PCI0.GPP7.DEV0)) {
                                CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP7.DEV0, 0x1)\n", 0, 0, 0, 0, 0, 0)
                                Notify (\_SB.PCI0.GPP7.DEV0, 0x01)
                            }
                            Store(CpmReadSmnRegister(0,0,0,0x13F01308), local0)
                            AND(local0, 0xFFFFFFFE, local0)
                            CpmWriteSmnRegister(0,0,0, 0x13F01308, local0)
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                        Default {
                            CpmDebugPrint ("    Return (0x00)\n", 0, 0, 0, 0, 0, 0)
                            Return (0x00)
                        }
                    } // End of Switch (ToInteger (Arg2))

                } Else {
                    CpmDebugPrint ("    Return (Buffer (1) {0x00})\n", 0, 0, 0, 0, 0, 0)
                    Return (Buffer (1) {0x00})
                }
            } // End of Method (_DSM, 0x4, Serialized)
        } // End of Device (PEP)
    } // End of Scope (\_SB)
} // End of DefinitionBlock

