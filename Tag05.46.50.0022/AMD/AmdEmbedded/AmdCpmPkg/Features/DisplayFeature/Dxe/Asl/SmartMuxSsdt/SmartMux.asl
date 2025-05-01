/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "SmartMux.aml",                 // Output file
    "SSDT",                         // Signature
    0x02,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMSMMUX",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_DISPLAY_FEATURE_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"

    External (\_SB.PCI0.PBC_.VGA, DeviceObj)
    External (\_SB.PCI0.PB2_.SWUS.SWDS.VGA, DeviceObj)

    Scope (\_SB) {
        Device (MUX1) {
            Name (_HID, "SMUX1234")
            Name (_UID, 0)

            Name (CpmMuxSelDeviceId, 0x55)
            Name (CpmDgpuMuxSetting, 0x55)

            Method(_STA) {
                //Bit[3] - 0 = SAG1.5, 1 = SAG2.0
                if (LEqual (And (ShiftRight (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureMuxFlag), 3), 0x1), 0x1)) {
                    Return (0x0F)
                } else {
                    Return (0x0B)
                }
            }

            //
            // DMQU (Display Mux Query), this returns the current state of the mux
            //
            Method (AMQU, 1, Serialized) {
                CpmDebugPrint ("  FEA-ASL-\\_SB.MUX1.DMQU (0x%X)\n", ToInteger (Arg0), 0, 0, 0, 0, 0)
                Switch (ToInteger (Arg0)) {
                    Case (1) {
                        Store (CpmCheckDevicePowerState (CpmMuxSelDeviceId, CpmDgpuMuxSetting), Local0)
                        If (LEqual (Local0, 0)) {
                            CpmDebugPrint ("    Return (\\_SB.PCI0.PBC_.VGA.EDP1)\n", 0, 0, 0, 0, 0, 0)
                            Return ("\_SB.PCI0.PBC_.VGA.EDP1")
                        } Else {
                            CpmDebugPrint ("    Return (\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2)\n", 0, 0, 0, 0, 0, 0)
                            Return ("\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2")
                        }
                    } // End of Case (1)
                    Case (2) {
                        Store (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureSmartMuxMDMSupportLevel), Local0)
                        CpmDebugPrint ("    Return (%d)\n", Local0, 0, 0, 0, 0, 0)
                        Return (Local0)
                    } // End of Case (2)
                    Case (3) {
                        Store (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureSmartMuxFirstConnectedGPU), Local0)
                        If (LEqual (Local0, 0)) {
                            CpmDebugPrint ("    Return (\\_SB.PCI0.PBC_.VGA.EDP1)\n", 0, 0, 0, 0, 0, 0)
                            Return ("\_SB.PCI0.PBC_.VGA.EDP1")
                        } Else {
                            CpmDebugPrint ("    Return (\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2)\n", 0, 0, 0, 0, 0, 0)
                            Return ("\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2")
                        }
                    } // End of Case (3)
                    Case (4) {
                        Store (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureSmartMuxFirstConnectedGPU), Local0)
                        If (LEqual (Local0, 0)) {
                            CpmDebugPrint ("    Return (\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2)\n", 0, 0, 0, 0, 0, 0)
                            Return ("\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2")
                        } Else {
                            CpmDebugPrint ("    Return (\\_SB.PCI0.PBC_.VGA.EDP1)\n", 0, 0, 0, 0, 0, 0)
                            Return ("\_SB.PCI0.PBC_.VGA.EDP1")
                        }
                    } // End of Case (4)
                    Default {
                        CpmDebugPrint ("    Return ()\n", 0, 0, 0, 0, 0, 0)
                        Return ("")
                    }
                } // End of Switch (ToInteger (Arg0))
            } // End of Method (DMQU, 1, Serialized)

            //
            // DMCF (Display Mux Configure), this is used to configure the mux
            //
            Method (AMCF, 1, Serialized) {
                Store (SizeOf (Arg0), Local0)
                CpmDebugPrint ("  FEA-ASL-\\_SB.MUX1.DMCF (%S) String Length = %d\n", Arg0, Local0, 0, 0, 0, 0)
                If (LGreater (Local0, 28)) {
                    // Mux switch to dGPU
                    CpmDebugPrint ("    Mux switch to dGPU\n", 0, 0, 0, 0, 0, 0)
                    CpmSetDevicePower (CpmMuxSelDeviceId, CpmDgpuMuxSetting)
                    CpmDebugPrint ("    Return (0)\n", Arg0, 0, 0, 0, 0, 0)
                    Return (0)
                } Else {
                    // Mux switch to iGPU
                    CpmDebugPrint ("    Mux switch to iGPU\n", 0, 0, 0, 0, 0, 0)
                    CpmSetDevicePower (CpmMuxSelDeviceId, And (Xor (CpmDgpuMuxSetting, 0x1), 0x1))
                    CpmDebugPrint ("    Return (0)\n", Arg0, 0, 0, 0, 0, 0)
                    Return (0)
                }
                CpmDebugPrint ("    Return (2)\n", Arg0, 0, 0, 0, 0, 0)
                Return (2)
            } // End of Method (DMCF, 1, Serialized)
        } // End of Device (MUX1)
    } // End of Scope (\_SB)

    Scope (\_SB.PCI0.PBC_.VGA) {
        Method (_DEP, 0, Serialized) {
            If (LEqual (CpmCurrentOSType, 0xAA)) {
                Store (CpmIsWin8 (), Local0)
            }
            If (LAnd (LEqual (And (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureMuxFlag), 0x20), 0x20), LAnd (CondRefOf (\_SB.MUX1), LGreaterEqual (CpmCurrentOSType, 0x0B)))) {
                CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PBC_.VGA._DEP () Return (Package (1) {\\_SB.MUX1})\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.MUX1})
            } Else {
                CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PBC_.VGA._DEP () Return (Package (1) {\\_SB.PCI0})\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0})
            }
        } // End of Method (_DEP, 0, NotSerialized)

        Device (EDP1) {
            Name (_ADR, 0x400)

            Method (DMID, 0, Serialized) {
                CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PBC_.VGA.EDP1.DMID () Return (\\_SB.MUX1)\n", 0, 0, 0, 0, 0, 0)
                Return ("\_SB.MUX1")
            } // End of Method (DMID, 0, Serialized)
        } // End of Device (EDP1)
    } // End of Scope (\_SB.PCI0.PBC_.VGA)

    Scope (\_SB.PCI0.PB2_.SWUS.SWDS.VGA) {
        Method (_DEP, 0, Serialized) {
            If (LEqual (CpmCurrentOSType, 0xAA)) {
                Store (CpmIsWin8 (), Local0)
            }
            If (LAnd (LEqual (And (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureMuxFlag), 0x20), 0x20), LAnd (CondRefOf (\_SB.MUX1), LGreaterEqual (CpmCurrentOSType, 0x0B)))) {
                CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PB2_.SWUS.SWDS.VGA._DEP () Return (Package (1) {\\_SB.MUX1})\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.MUX1})
            } Else {
                CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PB2_.SWUS.SWDS.VGA._DEP () Return (Package (1) {\\_SB.PCI0})\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0})
            }
        } // End of Method (_DEP, 0, NotSerialized)

        Name (CpmMuxSelDeviceIdInIgpu, 0x55)
        Name (CpmDgpuMuxSettingInIgpu, 0x55)

        //
        // DMQU (Display Mux Query), this returns the current state of the mux
        //
        Method (BMQU, 1, Serialized) {
            CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.DMQU (0x%X)\n", ToInteger (Arg0), 0, 0, 0, 0, 0)
            Switch (ToInteger (Arg0)) {
                Case (1) {
                    Store (CpmCheckDevicePowerState (CpmMuxSelDeviceIdInIgpu, CpmDgpuMuxSettingInIgpu), Local0)
                    If (LEqual (Local0, 0)) {
                        CpmDebugPrint ("    Return (\\_SB.PCI0.PBC_.VGA.EDP1)\n", 0, 0, 0, 0, 0, 0)
                        Return ("\_SB.PCI0.PBC_.VGA.EDP1")
                    } Else {
                        CpmDebugPrint ("    Return (\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2)\n", 0, 0, 0, 0, 0, 0)
                        Return ("\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2")
                    }
                } // End of Case (1)
                Case (2) {
                    Store (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureSmartMuxMDMSupportLevel), Local0)
                    CpmDebugPrint ("    Return (%d)\n", Local0, 0, 0, 0, 0, 0)
                    Return (Local0)
                } // End of Case (2)
                Case (3) {
                    Store (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureSmartMuxFirstConnectedGPU), Local0)
                    If (LEqual (Local0, 0)) {
                        CpmDebugPrint ("    Return (\\_SB.PCI0.PBC_.VGA.EDP1)\n", 0, 0, 0, 0, 0, 0)
                        Return ("\_SB.PCI0.PBC_.VGA.EDP1")
                    } Else {
                        CpmDebugPrint ("    Return (\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2)\n", 0, 0, 0, 0, 0, 0)
                        Return ("\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2")
                    }
                } // End of Case (3)
                Case (4) {
                    Store (CpmReadTable (CpmDisplayFeatureTable, CpmDisplayFeatureSmartMuxFirstConnectedGPU), Local0)
                    If (LEqual (Local0, 0)) {
                        CpmDebugPrint ("    Return (\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2)\n", 0, 0, 0, 0, 0, 0)
                        Return ("\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2")
                    } Else {
                        CpmDebugPrint ("    Return (\\_SB.PCI0.PBC_.VGA.EDP1)\n", 0, 0, 0, 0, 0, 0)
                        Return ("\_SB.PCI0.PBC_.VGA.EDP1")
                    }
                } // End of Case (4)
                Default {
                    CpmDebugPrint ("    Return ()\n", 0, 0, 0, 0, 0, 0)
                    Return ("")
                }
            } // End of Switch (ToInteger (Arg0))
        } // End of Method (DMQU, 1, Serialized)

        //
        // DMCF (Display Mux Configure), this is used to configure the mux
        //
        Method (BMCF, 1, Serialized) {
            Store (SizeOf (Arg0), Local0)
            CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.DMCF (%S) String Length = %d\n", Arg0, Local0, 0, 0, 0, 0)
            If (LGreater (Local0, 28)) {
                // Mux switch to dGPU
                CpmDebugPrint ("    Mux switch to dGPU\n", 0, 0, 0, 0, 0, 0)
                CpmSetDevicePower (CpmMuxSelDeviceIdInIgpu, CpmDgpuMuxSettingInIgpu)
                CpmDebugPrint ("    Return (0)\n", Arg0, 0, 0, 0, 0, 0)
                Return (0)
            } Else {
                // Mux switch to iGPU
                CpmDebugPrint ("    Mux switch to iGPU\n", 0, 0, 0, 0, 0, 0)
                CpmSetDevicePower (CpmMuxSelDeviceIdInIgpu, And (Xor (CpmDgpuMuxSettingInIgpu, 0x1), 0x1))
                CpmDebugPrint ("    Return (0)\n", Arg0, 0, 0, 0, 0, 0)
                Return (0)
            }
            CpmDebugPrint ("    Return (2)\n", Arg0, 0, 0, 0, 0, 0)
            Return (2)
        } // End of Method (DMCF, 1, Serialized)

        Device (EDP2) {
            Name (_ADR, 0x400)

            Method (DMID, 0, Serialized) {
                CpmDebugPrint ("  FEA-ASL-\\_SB.PCI0.PB2_.SWUS.SWDS.VGA.EDP2.DMID () Return (\\_SB.MUX1)\n", 0, 0, 0, 0, 0, 0)
                Return ("\_SB.MUX1")
            } // End of Method (DMID, 0, Serialized)
        } // End of Device (EDP2)
    } // End of Scope (\_SB.PCI0.PB2_.SWUS.SWDS.VGA)

} // End of DefinitionBlock
