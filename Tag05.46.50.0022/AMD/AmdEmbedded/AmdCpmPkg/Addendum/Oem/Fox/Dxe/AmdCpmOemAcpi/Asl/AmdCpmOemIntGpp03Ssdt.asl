/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
// Internal GPP [0:8:3] SSDT
DefinitionBlock (
    "InternalGpp03.aml",          // Output file
    "SSDT",                       // Signature
    0x01,                         // SSDT Revision
    "AMD",                        // OEM ID
    "INTGPP03",                   // OEM Table ID = ('I','N','T','G','P','P','0','3')
    0x1                           // OEM Revision
    )
{
    #define CPM_INTERNAL_GPP_03_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External (\_SB.PCI0.GP11, DeviceObj)
    External (\_SB.PCI0.GP12, DeviceObj)
    External (\_SB.PCI0.GP19, DeviceObj)
    External (\_SB.PCI0.GP19.XHC2, DeviceObj)
    External (\_SB.PCI0.GP19.XHC3, DeviceObj)
    External (\_SB.PCI0.GP19.XHC4, DeviceObj)
    External (\_SB.PCI0.GP19.NHI0, DeviceObj)
    External (\_SB.PCI0.GP19.NHI1, DeviceObj)
    External (\_SB.PCI0.GP19.XHC3.RHUB.PRT2, DeviceObj)
    External (\_SB.PCI0.GP19.XHC4.RHUB.PRT2, DeviceObj)

    External (\_SB.PCI0.EBUS, IntObj)
    External (\_SB.PCI0.DADR, IntObj)
    External (_ADR, IntObj)

    External (\_SB.ALIB, MethodObj)
    External (\_SB.CK17, MethodObj)
    External (\_SB.PCI0.SPCH, MethodObj)
    External (\_SB.PCI0.RPRM, MethodObj)
    External (SSTA, MethodObj)
    External (SINI, MethodObj)
    External (SREG, MethodObj)
    External (SDSW, MethodObj)
    External (PPS0, MethodObj)
    External (PPS3, MethodObj)
    External (PSTA, MethodObj)
    External (P_ON, MethodObj)
    External (P_OF, MethodObj)

    Scope (\) {
        Name (P3D3, 0x55)
        Name (G0D3, 0x55)
        Name (G3D3, 0x55)

        Name (RCRS, 0x55)
        Name (G5D3, 0x55)
        Name (P4D3, 0x55)
        Name (H0D3, 0x55)

        Name (CIMS, 0xCE)
        Name (CPSX, 0x80)
        Name (DIMS, 0xCF)
        Name (DDEP, 0x55)
    } // End of Scope (\)

    Scope (\_SB) {
        Method (CK19, 0) {
            Store (\_SB.PCI0.GP19.RPRM (Add (\_SB.PCI0.GP19.DADR, 0x54), 2), Local0)
            \_SB.PCI0.GP19.WPRM (Add (\_SB.PCI0.GP19.DADR, 0x54), 2, And (Local0, 0x7FFC))
            Store (\_SB.PCI0.GP19.XHC2.RPRM (Add (\_SB.PCI0.GP19.XHC2.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP19.XHC3.RPRM (Add (\_SB.PCI0.GP19.XHC3.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP19.XHC4.RPRM (Add (\_SB.PCI0.GP19.XHC4.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP19.NHI0.RPRM (Add (\_SB.PCI0.GP19.NHI0.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP19.NHI1.RPRM (Add (\_SB.PCI0.GP19.NHI1.DADR, 0x54), 2), Local1)
            \_SB.PCI0.GP19.WPRM (Add (\_SB.PCI0.GP19.DADR, 0x54), 2, And (Local0, 0x7FFF))

            Store (\_SB.PCI0.GP11.RPRM (Add (\_SB.PCI0.GP11.DADR, 0x54), 2), Local0)
            Store (\_SB.PCI0.GP12.RPRM (Add (\_SB.PCI0.GP12.DADR, 0x54), 2), Local0)
        } // End of Method (CK19)

        Method (OPTS, 1) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.OPTS (0x%X) Start\n", Arg0, 0, 0, 0, 0, 0)
            If (CondRefOf (\_SB.CK17)) {
                \_SB.CK17 ()
            }
            \_SB.CK19 ()
            CpmDebugPrint ("  OEM-ASL-\\_SB.OPTS (0x%X) End\n", Arg0, 0, 0, 0, 0, 0)
        } // End of Method (OPTS)

        Method (OWAK, 1) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.OWAK (0x%X) Start\n", Arg0, 0, 0, 0, 0, 0)
            If (CondRefOf (\_SB.CK17)) {
                \_SB.CK17 ()
            }
            \_SB.CK19 ()
            CpmDebugPrint ("  OEM-ASL-\\_SB.OWAK (0x%X) End\n", Arg0, 0, 0, 0, 0, 0)
        } // End of Method (OWAK)
    } // End of Scope (\_SB)

    Scope (\_SB.PCI0.GP19) {
        Method (YS0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\P3D3, 0x55)) {
                Store (\P3D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (YPRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x1A, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (2) {0x1A, 0x04})
        }
        // Don't expose _PS0/_PS3 or PowerResource _ON/_OFF in internal GPP, because
        // OSPM will clear SCI_0x1A enable bit at S0 and cause S0i3 cannot wake up by SCI_0x1A.
        #include "AmdCpmOemCommon.asi"
    } // End of Scope (\_SB.PCI0.GP19)

    // xhci2
    Scope (\_SB.PCI0.GP19.XHC2) {
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\G0D3, 0x55)) {
                Store (\G0D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x1A, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (2) {0x1A, 0x04})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci2Ps0Method)
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci2Ps3Method)
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci2Pr0StaMethod)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci2Pr0OnMethod)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci2Pr0OffMethod)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP19.XHC2)

    // xhci3
    Scope (\_SB.PCI0.GP19.XHC3) {
        Method (_S0W, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci3S0wMethod)
            Store (0, Local0)
            If (LNotEqual (\G3D3, 0x55)) {
                Store (\G3D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x1A, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (2) {0x1A, 0x04})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci3Ps0Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3._PS0 ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci3Ps3Method)
            If (LAnd (CondRefOf (\_SB.PCI0.GP11.PCSA), CondRefOf (\_SB.PCI0.GP19.NHI0.PCSA))) {
                CpmDebugPrint ("    GP11.PCSA = %d  XHC3.PCSA = %d  NHI0.PCSA = %d\n", \_SB.PCI0.GP11.PCSA, \_SB.PCI0.GP19.XHC3.PCSA, \_SB.PCI0.GP19.NHI0.PCSA, 0, 0, 0)
            }
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3._PS3 ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci3PrStaMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci3PrOnMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3.PWRS._ON ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci3PrOffMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3.PWRS._OFF ()\n", 0, 0, 0, 0, 0, 0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP19.XHC3)

    Scope (\_SB.PCI0.GP19.XHC3.RHUB.PRT2) {
        Method (_DEP, 0, Serialized) {
            If (LEqual (CpmCurrentOSType, 0xAA)) {
                Store (CpmIsWin8 (), Local0)
            }
            If (LAnd (LEqual (\DDEP, 0x01), LAnd (CondRefOf (\_SB.PCI0.GP19.NHI0), LLess (CpmCurrentOSType, 0x0B)))) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3.RHUB.PRT2._DEP  Return Package (1) {\\_SB.PCI0.GP19.NHI0}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0.GP19.NHI0})
            } Else {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3.RHUB.PRT2._DEP  Return Package (1) {\\_SB.PCI0}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0})
            }
        }
        // The _DSD method must be put in SSDT, please don't touch it.
        // the "USB4 router" may enters D3 state before XHCI enters D3 state if put in DSDT
        Name (XDSD, Package (2) {
            ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
            Package (2) {
                Package (2) {"usb4-host-interface", \_SB.PCI0.GP19.NHI0},
                Package (2) {"usb4-port-number", 1},
            }
        })
        Method (_DSM, 0x4, Serialized) {
            If (LEqual (Arg0, ToUUID ("CE2EE385-00E6-48CB-9F05-2EDB927C4899"))) {
                CreateDWordField (Arg0, 0, UID0)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC3.RHUB.PRT2._DSM (UUID 0x%X, 0x%X, 0x%X)\n", UID0, Arg1, Arg2, 0, 0, 0)
                Switch (Arg2) {
                    Case (0) {
                        If (LLess (CpmCurrentOSType, 0x0B)) {
                            // Windows 10: Support function 0 and 5
                            CpmDebugPrint ("    Return Buffer (1) {0x21}\n", 0, 0, 0, 0, 0, 0)
                            Return (Buffer (1) {0x21})
                        } Else {
                            // Windows 11+:
                            CpmDebugPrint ("    Return Buffer (1) {0x00}\n", 0, 0, 0, 0, 0, 0)
                            Return (Buffer (1) {0x00})
                        }
                    }
                    Case (5) {
                        If (LLess (CpmCurrentOSType, 0x0B)) {
                            // Windows 10:
                            CpmDebugPrint ("    Return Integer 0x01\n", 0, 0, 0, 0, 0, 0)
                            Return (0x01)
                        } Else {
                            // Windows 11+:
                            CpmDebugPrint ("    Unsupported function\n", 0, 0, 0, 0, 0, 0)
                        }
                    }
                    Default {
                        CpmDebugPrint ("    Unsupported function\n", 0, 0, 0, 0, 0, 0)
                    }
                } // End of Switch (Arg2)
            } Else {
                CpmDebugPrint ("    Return Buffer (1) {0x00} (Unsupported UUID)\n", 0, 0, 0, 0, 0, 0)
                Return (Buffer (1) {0x00})
            }
        } // End of Method (_DSM, 0x4, Serialized)
    } // End of Scope (\_SB.PCI0.GP19.XHC3.RHUB.PRT2)

    // xhci4
    Scope (\_SB.PCI0.GP19.XHC4) {
        Method (_S0W, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci4S0wMethod)
            Store (0, Local0)
            If (LNotEqual (\G3D3, 0x55)) {
                Store (\G3D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x1A, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (2) {0x1A, 0x04})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci4Ps0Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4._PS0 ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci4Ps3Method)
            If (LAnd (CondRefOf (\_SB.PCI0.GP12.PCSA), CondRefOf (\_SB.PCI0.GP19.NHI1.PCSA))) {
                CpmDebugPrint ("    GP12.PCSA = %d  XHC4.PCSA = %d  NHI1.PCSA = %d\n", \_SB.PCI0.GP12.PCSA, \_SB.PCI0.GP19.XHC4.PCSA, \_SB.PCI0.GP19.NHI1.PCSA, 0, 0, 0)
            }
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4._PS3 ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci4PrStaMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci4PrOnMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4.PWRS._ON ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci4PrOffMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4.PWRS._OFF ()\n", 0, 0, 0, 0, 0, 0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP19.XHC4)

    Scope (\_SB.PCI0.GP19.XHC4.RHUB.PRT2) {
        Method (_DEP, 0, Serialized) {
            If (LEqual (CpmCurrentOSType, 0xAA)) {
                Store (CpmIsWin8 (), Local0)
            }
            If (LAnd (LEqual (\DDEP, 0x01), LAnd (CondRefOf (\_SB.PCI0.GP19.NHI1), LLess (CpmCurrentOSType, 0x0B)))) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4.RHUB.PRT2._DEP  Return Package (1) {\\_SB.PCI0.GP19.NHI1}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0.GP19.NHI1})
            } Else {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4.RHUB.PRT2._DEP  Return Package (1) {\\_SB.PCI0}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0})
            }
        }
        // The _DSD method must be put in SSDT, please don't touch it.
        // the "USB4 router" may enters D3 state before XHCI enters D3 state if put in DSDT
        Name (YDSD, Package (2) {
            ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
            Package (2) {
                Package (2) {"usb4-host-interface", \_SB.PCI0.GP19.NHI1},
                Package (2) {"usb4-port-number", 2},
            }
        })
        Method (_DSM, 0x4, Serialized) {
            If (LEqual (Arg0, ToUUID ("CE2EE385-00E6-48CB-9F05-2EDB927C4899"))) {
                CreateDWordField (Arg0, 0, UID0)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.XHC4.RHUB.PRT2._DSM (UUID 0x%X, 0x%X, 0x%X)\n", UID0, Arg1, Arg2, 0, 0, 0)
                Switch (Arg2) {
                    Case (0) {
                        If (LLess (CpmCurrentOSType, 0x0B)) {
                            // Windows 10: Support function 0 and 5
                            CpmDebugPrint ("    Return Buffer (1) {0x21}\n", 0, 0, 0, 0, 0, 0)
                            Return (Buffer (1) {0x21})
                        } Else {
                            // Windows 11+:
                            CpmDebugPrint ("    Return Buffer (1) {0x00}\n", 0, 0, 0, 0, 0, 0)
                            Return (Buffer (1) {0x00})
                        }
                    }
                    Case (5) {
                        If (LLess (CpmCurrentOSType, 0x0B)) {
                            // Windows 10:
                            CpmDebugPrint ("    Return Integer 0x01\n", 0, 0, 0, 0, 0, 0)
                            Return (0x01)
                        } Else {
                            // Windows 11+:
                            CpmDebugPrint ("    Unsupported function\n", 0, 0, 0, 0, 0, 0)
                        }
                    }
                    Default {
                        CpmDebugPrint ("    Unsupported function\n", 0, 0, 0, 0, 0, 0)
                    }
                } // End of Switch (Arg2)
            } Else {
                CpmDebugPrint ("    Return Buffer (1) {0x00} (Unsupported UUID)\n", 0, 0, 0, 0, 0, 0)
                Return (Buffer (1) {0x00})
            }
        } // End of Method (_DSM, 0x4, Serialized)
    } // End of Scope (\_SB.PCI0.GP19.XHC4.RHUB.PRT2)

    Scope (\_SB.PCI0.GP19.NHI0) {
        Method (SDEP, 1, Serialized) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0.SDEP (%d)\n", Arg0, 0, 0, 0, 0, 0)
            If (LNotEqual (\DDEP, 0x01)) {
                Store (\DIMS, Local1)
                CpmTriggerSmi (Local1, Arg0, 0)
                Store (0x01, \DDEP)
            }
        }
        Method (_RST, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0._RST Call \\_SB.ALIB (0x15, 0)\n", 0, 0, 0, 0, 0, 0)
            \_SB.ALIB (0x15, 0)
        }
        Method (_S0W, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt0S0wMethod)
            Store (0, Local0)
            If (LNotEqual (\G5D3, 0x55)) {
                Store (\G5D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x1A, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (3) {0x1A, 0x04, PWRS})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt0Ps0Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0._PS0 ()\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                If (CondRefOf (\_SB.PCI0.GP11.SWUS.PCSA)) {
                    If (LEqual (\_SB.PCI0.GP11.SWUS.PCSA, 0x00)) {
                        // USB4 PCIe Tunneling is in D3.
                        Store (\CIMS, Local1)
                        If (LEqual (And (\CPSX, 0x01), 0x00)) {
                            // Issue SW SMI to register Periodic SMI.
                            Or (\CPSX, 0x01, \CPSX)
                            Store (\CPSX, Local2)
                            CpmTriggerSmi (Local1, Local2, 0)
                        }
                    }
                }
            }
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt0Ps3Method)
            If (LAnd (CondRefOf (\_SB.PCI0.GP11.PCSA), CondRefOf (\_SB.PCI0.GP19.XHC3.PCSA))) {
                CpmDebugPrint ("    GP11.PCSA = %d  XHC3.PCSA = %d  NHI0.PCSA = %d\n", \_SB.PCI0.GP11.PCSA, \_SB.PCI0.GP19.XHC3.PCSA, \_SB.PCI0.GP19.NHI0.PCSA, 0, 0, 0)
            }
            If (LEqual (\G5D3, 0x4)) {
                Store (CpmReadPci (0, 3, 1, 0x72, 0, 8), Local0)
                Store (0xC0, Local1)
                If (LEqual (And (Local0, 0xC0), 0x40)) {
                    CpmDebugPrint ("  PCIe GPP [0:3:1] EP is connected\n", 0, 0, 0, 0, 0, 0)
                    // To fix OS PCI driver issue during reboot.
                    // Check and put USB4 PCIe Tunneling to D3 when USB4 RT is in D3.
                    Store (\_SB.PCI0.GP11.RPRM (Add (\_SB.PCI0.GP11.DADR, 0x54), 2), Local2)
                    If (LEqual (And (Local2, 3), 0)) {
                        \_SB.PCI0.GP11.WPRM (Add (\_SB.PCI0.GP11.DADR, 0x54), 2, And (Or (Local2, 3), 0x7FFF))
                    }
                }
                CpmDebugPrint ("    Call \\_SB.ALIB (0x14, 1) - Start\n", 0, 0, 0, 0, 0, 0)
                \_SB.ALIB (0x14, 1)
                CpmDebugPrint ("    Call \\_SB.ALIB (0x14, 1) - End\n", 0, 0, 0, 0, 0, 0)
            }
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0._PS3 - End\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                Store (\CIMS, Local1)
                If (LEqual (And (\CPSX, 0x01), 0x01)) {
                    // Issue SW SMI to unregister Periodic SMI.
                    And (\CPSX, 0x7E, \CPSX)
                    Store (\CPSX, Local2)
                    CpmTriggerSmi (Local1, Local2, 0)
                }
            }
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt0PrStaMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt0PrOnMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0.PWRS.ON ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt0PrOffMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI0.PWRS.OFF ()\n", 0, 0, 0, 0, 0, 0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP19.NHI0)

    Scope (\_SB.PCI0.GP19.NHI1) {
        Method (SDEP, 1, Serialized) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1.SDEP (%d)\n", Arg0, 0, 0, 0, 0, 0)
            If (LNotEqual (\DDEP, 0x01)) {
                Store (\DIMS, Local1)
                CpmTriggerSmi (Local1, Arg0, 0)
                Store (0x01, \DDEP)
            }
        }
        Method (_RST, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1._RST Call \\_SB.ALIB (0x15, 1)\n", 0, 0, 0, 0, 0, 0)
            \_SB.ALIB (0x15, 1)
        }
        Method (_S0W, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt1S0wMethod)
            Store (0, Local0)
            If (LNotEqual (\G5D3, 0x55)) {
                Store (\G5D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x1A, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (3) {0x1A, 0x04, PWRS})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt1Ps0Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1._PS0 ()\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                If (CondRefOf (\_SB.PCI0.GP12.SWUS.PCSA)) {
                    If (LEqual (\_SB.PCI0.GP12.SWUS.PCSA, 0x00)) {
                        // USB4 PCIe Tunneling is in D3.
                        Store (\CIMS, Local1)
                        If (LEqual (And (\CPSX, 0x02), 0x00)) {
                            // Issue SW SMI to register Periodic SMI.
                            Or (\CPSX, 0x02, \CPSX)
                            Store (\CPSX, Local2)
                            CpmTriggerSmi (Local1, Local2, 0)
                        }
                    }
                }
            }
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt1Ps3Method)
            If (LAnd (CondRefOf (\_SB.PCI0.GP12.PCSA), CondRefOf (\_SB.PCI0.GP19.XHC4.PCSA))) {
                CpmDebugPrint ("    GP12.PCSA = %d  XHC4.PCSA = %d  NHI1.PCSA = %d\n", \_SB.PCI0.GP12.PCSA, \_SB.PCI0.GP19.XHC4.PCSA, \_SB.PCI0.GP19.NHI1.PCSA, 0, 0, 0)
            }
            If (LEqual (\G5D3, 0x4)) {
                Store (CpmReadPci (0, 4, 1, 0x72, 0, 8), Local0)
                Store (0xC0, Local1)
                If (LEqual (And (Local0, 0xC0), 0x40)) {
                    CpmDebugPrint ("  PCIe GPP [0:4:1] EP is connected\n", 0, 0, 0, 0, 0, 0)
                    // To fix OS PCI driver issue during reboot.
                    // Check and put USB4 PCIe Tunneling to D3 when USB4 RT is in D3.
                    Store (\_SB.PCI0.GP12.RPRM (Add (\_SB.PCI0.GP12.DADR, 0x54), 2), Local2)
                    If (LEqual (And (Local2, 3), 0)) {
                        \_SB.PCI0.GP12.WPRM (Add (\_SB.PCI0.GP12.DADR, 0x54), 2, And (Or (Local2, 3), 0x7FFF))
                    }
                }
                CpmDebugPrint ("    Call \\_SB.ALIB (0x14, 2) - Start\n", 0, 0, 0, 0, 0, 0)
                \_SB.ALIB (0x14, 2)
                CpmDebugPrint ("    Call \\_SB.ALIB (0x14, 2) - End\n", 0, 0, 0, 0, 0, 0)
            }
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1._PS3 - End\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                Store (\CIMS, Local1)
                If (LEqual (And (\CPSX, 0x02), 0x02)) {
                    // Issue SW SMI to unregister Periodic SMI.
                    And (\CPSX, 0x7D, \CPSX)
                    Store (\CPSX, Local2)
                    CpmTriggerSmi (Local1, Local2, 0)
                }
            }
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt1PrStaMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt1PrOnMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1.PWRS.ON ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Rt1PrOffMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP19.NHI1.PWRS.OFF ()\n", 0, 0, 0, 0, 0, 0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP19.NHI1)

    Scope (\_SB.PCI0.GP11) {
        Method (_DEP, 0, Serialized) {
            If (LEqual (CpmCurrentOSType, 0xAA)) {
                Store (CpmIsWin8 (), Local0)
            }
            If (LAnd (LEqual (\DDEP, 0x01), LAnd (CondRefOf (\_SB.PCI0.GP19.NHI0), LLess (CpmCurrentOSType, 0x0B)))) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11._DEP  Return Package (1) {\\_SB.PCI0.GP19.NHI0}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0.GP19.NHI0})
            } Else {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11._DEP  Return Package (1) {\\_SB.PCI0}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0})
            }
        }
        Method (_S0W, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie0S0wMethod)
            Store (0, Local0)
            If (LNotEqual (\P4D3, 0x55)) {
                Store (\P4D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11._PRW ()  Return Package (2) {0x1A, 0x04}\n", 0, 0, 0, 0, 0, 0)
            Return (Package (3) {0x1A, 0x04, PWRS})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie0Ps0Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11._PS0 ()\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                Store (\CIMS, Local1)
                If (LEqual (And (\CPSX, 0x01), 0x01)) {
                    // Issue SW SMI to unregister Periodic SMI.
                    And (\CPSX, 0x7E, \CPSX)
                    Store (\CPSX, Local2)
                    CpmTriggerSmi (Local1, Local2, 0)
                }
            }
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie0Ps3Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11._PS3 ()\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                If (CondRefOf (\_SB.PCI0.GP19.NHI0.PCSA)) {
                    If (LEqual (\_SB.PCI0.GP19.NHI0.PCSA, 0x01)) {
                        // USB4 RT is in D0.
                        Store (\CIMS, Local1)
                        If (LEqual (And (\CPSX, 0x01), 0x00)) {
                            // Issue SW SMI to register Periodic SMI.
                            Or (\CPSX, 0x01, \CPSX)
                            Store (\CPSX, Local2)
                            CpmTriggerSmi (Local1, Local2, 0)
                        }
                    }
                }
            }
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie0PrStaMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie0PrOnMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.PWRS._ON ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie0PrOffMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.PWRS._OFF ()\n", 0, 0, 0, 0, 0, 0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"

        Name (XDSD, Package (6) {
            ToUUID ("6211E2C0-58A3-4AF3-90E1-927A4E0C55A4"),
            Package (1) {
                Package (2) {"HotPlugSupportInD3", 1},
            },
            ToUUID ("EFCC06CC-73AC-4BC3-BFF0-76143807C389"),
            Package (2) {
                Package (2) {"ExternalFacingPort", 1},  // Property 1: This is an externally facing port/hierarchy
                Package (2) {"UID", 0},                 // Property 2: UID of the externally facing port on platform, range is: 0, 1, ..., n-1
            },
            ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
            Package (2) {
                Package (2) {"usb4-host-interface", \_SB.PCI0.GP19.NHI0},
                Package (2) {"usb4-port-number", 1},
            }
        })

        Method (_DSM, 0x4, Serialized) {
            If (LEqual (Arg0, ToUUID ("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
                CreateDWordField (Arg0, 0, UID0)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11._DSM (UUID 0x%X, 0x%X, 0x%X)\n", UID0, Arg1, Arg2, 0, 0, 0)
                Switch (ToInteger (Arg2)) {
                    //
                    // Function 00h:
                    //   Return supported functions by bitmask, based on revision
                    //
                    Case (0x0) {
                        Switch (Arg1) {
                            Case (0) {
                                //
                                // Revision 0: unsupported
                                //
                                CpmDebugPrint ("    Return 0x0\n", 0, 0, 0, 0, 0, 0)
                                Return (Buffer (1) {0x0})
                            }
                            Case (1) {
                                //
                                // Revision 1: function 5 supported
                                //
                                CpmDebugPrint ("    Return 0x21\n", 0, 0, 0, 0, 0, 0)
                                Return (Buffer (1) {0x21})
                            }
                        } // End of Switch (Arg1)

                        //
                        // Revision 2+: function 5, 6 supported
                        //
                        CpmDebugPrint ("    Return 0x61\n", 0, 0, 0, 0, 0, 0)
                        Return (Buffer (1) {0x61})
                    }

                    //
                    // Function 05h:
                    //   PCI Firmware v3.2: Ignoring PCI Boot Configurations
                    //   PCI Firmware v3.3: Preserving PCI Boot Configurations
                    //
                    // Return:
                    //    0h - The operating system shall not ignore the PCI configuration
                    //         that firmware has done at boot time. However, the operating system
                    //         is free to configure the devices in this hierarchy that have not
                    //         been configured by the firmware. There may be a reduced level of
                    //         hot plug capability support in this hierarchy due to resource
                    //         constraints. This situation is the same as the legacy situation
                    //         where this _DSM is not provided.
                    //    1h - The operating system may ignore the PCI configuration that
                    //         the firmware has done at boot time, and reconfigure/rebalance the
                    //         resources in the hierarchy.
                    Case (0x05) {
                        If (LGreaterEqual (Arg1, 1)) { // check Arg1 for revision
                            Store (0, Local0)
                            If (LNotEqual (\RCRS, 0x55)) {
                                Store (\RCRS, Local0)
                            }
                            CpmDebugPrint ("    Return %d\n", Local0, 0, 0, 0, 0, 0)
                            Return (Local0)
                        }
                    }

                    //
                    // Function 06h: Latency Tolerance Reporting
                    //
                    // Return:
                    //   A Package of four integers corresponding with the LTR encoding
                    //   defined in the PCI Express Base Specification, as follows:
                    //     Integer 0: Maximum Snoop Latency Scale
                    //     Integer 1: Maximum Snoop Latency Value
                    //     Integer 2: Maximum No-Snoop Latency Scale
                    //     Integer 3: Maximum No-Snoop Latency Value
                    //   These values correspond directly to the LTR Extended Capability Structure fields
                    //   described in the PCI Express Base Specification.
                    Case (0x06) {
                        If (LGreaterEqual (Arg1, 2)) { // check Arg1 for revision
                            CpmDebugPrint ("    Return Package (4) {0x02, 0xC8, 0x02, 0xC8}\n", 0, 0, 0, 0, 0, 0)
                            Return (Package (4) {0x02, 0xC8, 0x02, 0xC8})
                        }
                    }
                } // End of Switch (ToInteger (Arg2))
            } // End of If (LEqual (Arg0, ToUUID ("E5C937D0-3553-4D7A-9117-EA4D19C3434D")))
        } // End of Method (_DSM, 0x4, Serialized)

        Device (SWUS) {
            Name (_ADR, 0x00)
            Method (_RMV, 0, Serialized) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS._RMV ()  Return 0\n", 0, 0, 0, 0, 0, 0)
                Return (0)
            }
            Method (_S0W, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie0EpS0wMethod)
                Store (0, Local0)
                If (LNotEqual (\H0D3, 0x55)) {
                    Store (\H0D3, Local0)
                }
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS._S0W ()  Return %d\n", Local0, 0, 0, 0, 0, 0)
                Return (Local0)
            }
            Method (_PRW, 0, Serialized) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS._PRW ()  Return Package (2) {0x1A, 0x04}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (2) {0x1A, 0x04})
            }
            Method (PPS0, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie0EpPs0Method)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS._PS0 ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (PPS3, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie0EpPs3Method)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS._PS3 ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (PSTA, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie0EpPrStaMethod)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (P_ON, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie0EpPrOnMethod)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS.PWRS._ON ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (P_OF, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie0EpPrOffMethod)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP11.SWUS.PWRS._OFF ()\n", 0, 0, 0, 0, 0, 0)
            }
            #include "AmdCpmOemCommon_PRx_PSx.asi"
        } // End of Device (SWUS)
    } // End of Scope (\_SB.PCI0.GP11)

    Scope (\_SB.PCI0.GP12) {
        Method (_DEP, 0, Serialized) {
            If (LEqual (CpmCurrentOSType, 0xAA)) {
                Store (CpmIsWin8 (), Local0)
            }
            If (LAnd (LEqual (\DDEP, 0x01), LAnd (CondRefOf (\_SB.PCI0.GP19.NHI1), LLess (CpmCurrentOSType, 0x0B)))) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12._DEP  Return Package (1) {\\_SB.PCI0.GP19.NHI1}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0.GP19.NHI1})
            } Else {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12._DEP  Return Package (1) {\\_SB.PCI0}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (1) {\_SB.PCI0})
            }
        }
        Method (_S0W, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie1S0wMethod)
            Store (0, Local0)
            If (LNotEqual (\P4D3, 0x55)) {
                Store (\P4D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12._PRW ()  Return Package (2) {0x1A, 0x04}\n", 0, 0, 0, 0, 0, 0)
            Return (Package (3) {0x1A, 0x04, PWRS})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie1Ps0Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12._PS0 ()\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                Store (\CIMS, Local1)
                If (LEqual (And (\CPSX, 0x02), 0x02)) {
                    // Issue SW SMI to unregister Periodic SMI.
                    And (\CPSX, 0x7D, \CPSX)
                    Store (\CPSX, Local2)
                    CpmTriggerSmi (Local1, Local2, 0)
                }
            }
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie1Ps3Method)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12._PS3 ()\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (\P4D3, 0x55)) {
                If (CondRefOf (\_SB.PCI0.GP19.NHI1.PCSA)) {
                    If (LEqual (\_SB.PCI0.GP19.NHI1.PCSA, 0x01)) {
                        // USB4 RT is in D0.
                        Store (\CIMS, Local1)
                        If (LEqual (And (\CPSX, 0x02), 0x00)) {
                            // Issue SW SMI to register Periodic SMI.
                            Or (\CPSX, 0x02, \CPSX)
                            Store (\CPSX, Local2)
                            CpmTriggerSmi (Local1, Local2, 0)
                        }
                    }
                }
            }
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie1PrStaMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie1PrOnMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.PWRS._ON ()\n", 0, 0, 0, 0, 0, 0)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsUsb4Pcie1PrOffMethod)
            CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.PWRS._OFF ()\n", 0, 0, 0, 0, 0, 0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"

        Name (YDSD, Package (6) {
            ToUUID ("6211E2C0-58A3-4AF3-90E1-927A4E0C55A4"),
            Package (1) {
                Package (2) {"HotPlugSupportInD3", 1},
            },
            ToUUID ("EFCC06CC-73AC-4BC3-BFF0-76143807C389"),
            Package (2) {
                Package (2) {"ExternalFacingPort", 1},  // Property 1: This is an externally facing port/hierarchy
                Package (2) {"UID", 1},                 // Property 2: UID of the externally facing port on platform, range is: 0, 1, ..., n-1
            },
            ToUUID ("DAFFD814-6EBA-4D8C-8A91-BC9BBF4AA301"),
            Package (2) {
                Package (2) {"usb4-host-interface", \_SB.PCI0.GP19.NHI1},
                Package (2) {"usb4-port-number", 2},
            }
        })

        Method (_DSM, 0x4, Serialized) {
            If (LEqual (Arg0, ToUUID ("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
                CreateDWordField (Arg0, 0, UID0)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12._DSM (UUID 0x%X, 0x%X, 0x%X)\n", UID0, Arg1, Arg2, 0, 0, 0)
                Switch (ToInteger (Arg2)) {
                    //
                    // Function 00h:
                    //   Return supported functions by bitmask, based on revision
                    //
                    Case (0x0) {
                        Switch (Arg1) {
                            Case (0) {
                                //
                                // Revision 0: unsupported
                                //
                                CpmDebugPrint ("    Return 0x0\n", 0, 0, 0, 0, 0, 0)
                                Return (Buffer (1) {0x0})
                            }
                            Case (1) {
                                //
                                // Revision 1: function 5 supported
                                //
                                CpmDebugPrint ("    Return 0x21\n", 0, 0, 0, 0, 0, 0)
                                Return (Buffer (1) {0x21})
                            }
                        } // End of Switch (Arg1)

                        //
                        // Revision 2+: function 5, 6 supported
                        //
                        CpmDebugPrint ("    Return 0x61\n", 0, 0, 0, 0, 0, 0)
                        Return (Buffer (1) {0x61})
                    }

                    //
                    // Function 05h:
                    //   PCI Firmware v3.2: Ignoring PCI Boot Configurations
                    //   PCI Firmware v3.3: Preserving PCI Boot Configurations
                    //
                    // Return:
                    //    0h - The operating system shall not ignore the PCI configuration
                    //         that firmware has done at boot time. However, the operating system
                    //         is free to configure the devices in this hierarchy that have not
                    //         been configured by the firmware. There may be a reduced level of
                    //         hot plug capability support in this hierarchy due to resource
                    //         constraints. This situation is the same as the legacy situation
                    //         where this _DSM is not provided.
                    //    1h - The operating system may ignore the PCI configuration that
                    //         the firmware has done at boot time, and reconfigure/rebalance the
                    //         resources in the hierarchy.
                    Case (0x05) {
                        If (LGreaterEqual (Arg1, 1)) { // check Arg1 for revision
                            Store (0, Local0)
                            If (LNotEqual (\RCRS, 0x55)) {
                                Store (\RCRS, Local0)
                            }
                            CpmDebugPrint ("    Return %d\n", Local0, 0, 0, 0, 0, 0)
                            Return (Local0)
                        }
                    }

                    //
                    // Function 06h: Latency Tolerance Reporting
                    //
                    // Return:
                    //   A Package of four integers corresponding with the LTR encoding
                    //   defined in the PCI Express Base Specification, as follows:
                    //     Integer 0: Maximum Snoop Latency Scale
                    //     Integer 1: Maximum Snoop Latency Value
                    //     Integer 2: Maximum No-Snoop Latency Scale
                    //     Integer 3: Maximum No-Snoop Latency Value
                    //   These values correspond directly to the LTR Extended Capability Structure fields
                    //   described in the PCI Express Base Specification.
                    Case (0x06) {
                        If (LGreaterEqual (Arg1, 2)) { // check Arg1 for revision
                            CpmDebugPrint ("    Return Package (4) {0x02, 0xC8, 0x02, 0xC8}\n", 0, 0, 0, 0, 0, 0)
                            Return (Package (4) {0x02, 0xC8, 0x02, 0xC8})
                        }
                    }
                } // End of Switch (ToInteger (Arg2))
            } // End of If (LEqual (Arg0, ToUUID ("E5C937D0-3553-4D7A-9117-EA4D19C3434D")))
        } // End of Method (_DSM, 0x4, Serialized)

        Device (SWUS) {
            Name (_ADR, 0x00)
            Method (_RMV, 0, Serialized) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS._RMV ()  Return 0\n", 0, 0, 0, 0, 0, 0)
                Return (0)
            }
            Method (_S0W, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie1EpS0wMethod)
                Store (0, Local0)
                If (LNotEqual (\H0D3, 0x55)) {
                    Store (\H0D3, Local0)
                }
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS._S0W ()  Return %d\n", Local0, 0, 0, 0, 0, 0)
                Return (Local0)
            }
            Method (_PRW, 0, Serialized) {
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS._PRW ()  Return Package (2) {0x1A, 0x04}\n", 0, 0, 0, 0, 0, 0)
                Return (Package (2) {0x1A, 0x04})
            }
            Method (PPS0, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie1EpPs0Method)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS._PS0 ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (PPS3, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie1EpPs3Method)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS._PS3 ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (PSTA, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie1EpPrStaMethod)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS.PWRS._STA ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (P_ON, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie1EpPrOnMethod)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS.PWRS._ON ()\n", 0, 0, 0, 0, 0, 0)
            }
            Method (P_OF, 0, Serialized) {
                CpmSetPostCode (CpmTpMsUsb4Pcie1EpPrOffMethod)
                CpmDebugPrint ("  OEM-ASL-\\_SB.PCI0.GP12.SWUS.PWRS._OFF ()\n", 0, 0, 0, 0, 0, 0)
            }
            #include "AmdCpmOemCommon_PRx_PSx.asi"
        } // End of Device (SWUS)
    } // End of Scope (\_SB.PCI0.GP12)

    Scope (\_GPE) {
        Method (_L1A) {
            CpmDebugPrint ("  OEM-ASL-\\_GPE._L1A\n", 0, 0, 0, 0, 0, 0)
            If (CondRefOf (\_SB.PCI0.GP19)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP19, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP19, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP19.NHI0)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP19.NHI0, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP19.NHI0, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP19.NHI1)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP19.NHI1, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP19.NHI1, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP19.XHC2)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP19.XHC2, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP19.XHC2, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP19.XHC3)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP19.XHC3, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP19.XHC3, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP19.XHC4)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP19.XHC4, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP19.XHC4, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP11)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP11, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP11, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP11.SWUS)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP11.SWUS, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP11.SWUS, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP12)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP12, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP12, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP12.SWUS)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP12.SWUS, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP12.SWUS, 0x2)
            }
        } // End of Method (_L1A)
    } // End of Scope (\_GPE)

} // End of DefinitionBlock

