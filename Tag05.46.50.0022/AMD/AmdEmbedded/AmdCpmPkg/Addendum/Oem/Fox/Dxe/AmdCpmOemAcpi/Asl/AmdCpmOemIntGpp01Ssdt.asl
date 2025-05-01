/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
// Internal GPP [0:8:1] SSDT
DefinitionBlock (
    "InternalGpp01.aml",          // Output file
    "SSDT",                       // Signature
    0x01,                         // SSDT Revision
    "AMD",                        // OEM ID
    "INTGPP01",                   // OEM Table ID = ('I','N','T','G','P','P','0','1')
    0x1                           // OEM Revision
    )
{
    #define CPM_INTERNAL_GPP_01_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External (\_SB.PCI0.GP17, DeviceObj)
    External (\_SB.PCI0.GP17.VGA, DeviceObj)
    External (\_SB.PCI0.GP17.HDAU, DeviceObj)
    External (\_SB.PCI0.GP17.XHC0, DeviceObj)
    External (\_SB.PCI0.GP17.XHC1, DeviceObj)
    External (\_SB.PCI0.GP17.ACP, DeviceObj)
    External (\_SB.PCI0.GP17.AZAL, DeviceObj)

    External (\_SB.PCI0.EBUS, IntObj)
    External (\_SB.PCI0.DADR, IntObj)
    External (_ADR, IntObj)

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
        Name (P1D3, 0x55)
        Name (F0D3, 0x55)
        Name (F3D3, 0x55)
        Name (F1D3, 0x55)
        Name (F5D3, 0x55)
        Name (F6D3, 0x55)
        Name (F5WV, 0x55)
    } // End of Scope (\)

    Scope (\_SB) {
        Method (CK17, 0) {
            Store (\_SB.PCI0.GP17.RPRM (Add (\_SB.PCI0.GP17.DADR, 0x54), 2), Local0)
            \_SB.PCI0.GP17.WPRM (Add (\_SB.PCI0.GP17.DADR, 0x54), 2, And (Local0, 0x7FFC))
            Store (\_SB.PCI0.GP17.VGA.RPRM (Add (\_SB.PCI0.GP17.VGA.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP17.HDAU.RPRM (Add (\_SB.PCI0.GP17.HDAU.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP17.XHC0.RPRM (Add (\_SB.PCI0.GP17.XHC0.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP17.XHC1.RPRM (Add (\_SB.PCI0.GP17.XHC1.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP17.ACP.RPRM (Add (\_SB.PCI0.GP17.ACP.DADR, 0x54), 2), Local1)
            Store (\_SB.PCI0.GP17.AZAL.RPRM (Add (\_SB.PCI0.GP17.AZAL.DADR, 0x54), 2), Local1)
            \_SB.PCI0.GP17.WPRM (Add (\_SB.PCI0.GP17.DADR, 0x54), 2, And (Local0, 0x7FFF))
        } // End of Method (CK17)
    } // End of Scope (\_SB)

    Scope (\_SB.PCI0.GP17) {
        Method (ZS0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\P1D3, 0x55)) {
                Store (\P1D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (ZPRW, 0, Serialized) {
            Return (Package (2) {0x19, 0x04})
        }
        // Don't expose _PS0/_PS3 or PowerResource _ON/_OFF in internal GPP, because
        // OSPM will clear SCI_0x19 enable bit at S0 and cause S0i3 cannot wake up by SCI_0x19.
        //
        // BUT to fix Audio cannot wake up from D3, we must expose _PS0/_PS3 or PowerResource _ON/_OFF
        // in this internal GPP, and use GPIO 58/59 to fix SCI_0x19 cannot wake up from S0i3 issue.
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP17)

    // VGA
    Scope (\_SB.PCI0.GP17.VGA) {
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F0D3, 0x55)) {
                Store (\F0D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP17.VGA)

    // Device 0, Function 1 Display HD Audio Controller
    Scope (\_SB.PCI0.GP17.HDAU) {
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F1D3, 0x55)) {
                Store (\F1D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP17.HDAU)

    // xhci0
    Scope (\_SB.PCI0.GP17.XHC0) {
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F3D3, 0x55)) {
                Store (\F3D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x19, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (2) {0x19, 0x04})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci0Ps0Method)
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci0Ps3Method)
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci0Pr0StaMethod)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci0Pr0OnMethod)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci0Pr0OffMethod)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP17.XHC0)

    // xhci1
    Scope (\_SB.PCI0.GP17.XHC1) {
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F3D3, 0x55)) {
                Store (\F3D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_PRW, 0, Serialized) {
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._PRW ()  Return Package (2) {0x19, 0x04}\n", DADR, 0, 0, 0, 0, 0)
            Return (Package (2) {0x19, 0x04})
        }
        Method (PPS0, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci1Ps0Method)
        }
        Method (PPS3, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci1Ps3Method)
        }
        Method (PSTA, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci1Pr0StaMethod)
        }
        Method (P_ON, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci1Pr0OnMethod)
        }
        Method (P_OF, 0, Serialized) {
            CpmSetPostCode (CpmTpMsXhci1Pr0OffMethod)
        }
        #include "AmdCpmOemCommon_PRx_PSx.asi"
    } // End of Scope (\_SB.PCI0.GP17.XHC1)

    // Device 0, Function 5
    Scope (\_SB.PCI0.GP17.ACP) {
        Method (_WOV, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F5WV, 0x55)) {
                Store (\F5WV, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._WOV ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F5D3, 0x55)) {
                Store (\F5D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        #include "AmdCpmOemCommon_PRx.asi"
    } // End of Scope (\_SB.PCI0.GP17.ACP)

    // Device 0, Function 6
    Scope (\_SB.PCI0.GP17.AZAL) {
        Method (_S0W, 0, Serialized) {
            Store (0, Local0)
            If (LNotEqual (\F6D3, 0x55)) {
                Store (\F6D3, Local0)
            }
            CpmDebugPrint ("  OEM-ASL-PCIe Address (0x%X)._S0W ()  Return %d\n", DADR, Local0, 0, 0, 0, 0)
            Return (Local0)
        }
        #include "AmdCpmOemCommon_PRx.asi"
    } // End of Scope (\_SB.PCI0.GP17.AZAL)

    Scope (\_GPE) {
        Method (_L19) {
            CpmDebugPrint ("  OEM-ASL-\\_GPE._L19\n", 0, 0, 0, 0, 0, 0)
            If (CondRefOf (\_SB.PCI0.GP17)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP17, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP17.XHC0)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.XHC0, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP17.XHC0, 0x2)
            }
            If (CondRefOf (\_SB.PCI0.GP17.XHC1)) {
                CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.XHC1, 0x2)\n", 0, 0, 0, 0, 0, 0)
                Notify (\_SB.PCI0.GP17.XHC1, 0x2)
            }
        } // End of Method (_L19)
    } // End of Scope (\_GPE)

} // End of DefinitionBlock

