/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "NvmeD3.aml",    // Output file
    "SSDT",          // Signature
    0x01,            // SSDT Revision
    "AMD",           // OEM ID
    "NVME",          // OEM Table ID
    0x1              // OEM Revision
    )
{

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.SPCF, MethodObj)
    External(\_SB.PCI0.RPCF, MethodObj)
    External(\_SB.PCI0.UPWD, MethodObj)

    External(\_SB.PCI0.GPPX, DeviceObj)
    External(\_SB.PCI0.GPPX._ADR, IntObj)
    External(\_SB.PCI0.GPPX.PWEN, MethodObj)
    External(\_SB.PCI0.GPPX.PWDI, MethodObj)

    Scope(\_SB.PCI0.GPPX) {
        Name(WD3C, 0x00)

        PowerResource(P0NV, 0, 0) {
            Name(D0NV, 1)

            Method(_STA) {
                CpmSetPostCode (CpmTpMsNvmeStaMethod)
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPPX.P0NV._STA\n", 0, 0, 0, 0, 0, 0)
                Return(D0NV)
            }

            Method(_ON) {
                CpmSetPostCode (CpmTpMsNvmeOnMethodEntry)
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPPX.P0NV._ON\n", 0, 0, 0, 0, 0, 0)
                If (LNotEqual (CpmNvmeRtd3Table, 0)) {
                    // D3 cold enable
                    If (LEqual(CpmReadTable (CpmNvmeRtd3Table, 0x10), 1)) {
                        If (CondRefOf(PWEN)) {
                            PWEN()
                        }

                        If (LEqual(WD3C, 1)) {
                            // Check IsSetNvmePassword
                            If (LEqual(CpmReadTable (CpmNvmeRtd3Table, 0x12), 1)) {
                                // Restore PCI configuration register
                                RPCF()
                                // Unlock Nvme Password
                                UPWD()
                                Store(0x00, WD3C)
                            }
                        }
                    }
                }
                Store(0x01, D0NV)
                CpmSetPostCode (CpmTpMsNvmeOnMethodExit)
            }

            Method(_OFF) {
                CpmSetPostCode (CpmTpMsNvmeOffMethodEntry)
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPPX.P0NV._OFF\n", 0, 0, 0, 0, 0, 0)
                If (LNotEqual (CpmNvmeRtd3Table, 0)) {
                    // D3 cold enable
                    If (LEqual(CpmReadTable (CpmNvmeRtd3Table, 0x10), 1)) {
                        If (CondRefOf(PWDI)) {
                            PWDI()
                        }
                        // Check IsSetNvmePassword
                        If (LEqual(CpmReadTable (CpmNvmeRtd3Table, 0x12), 1)) {
                            // Save PCI configuration register
                            SPCF(_ADR)
                        }
                    }
                }
                Store(0x00, D0NV)
                CpmSetPostCode (CpmTpMsNvmeOffMethodExit)
            }
        }

        Device(NVME) {
            Name(_ADR, 0x00)
            Name(_S0W, 4)
            Name(_PR0, Package() {P0NV})
            Name(_PR2, Package() {P0NV})
            Name(_PR3, Package() {P0NV})

            Method(_PS0, 0) {
                CpmSetPostCode (CpmTpMsNvmePs0Method)
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPPX.NVME._PS0\n", 0, 0, 0, 0, 0, 0)
            }

            Method(_PS3, 0) {
                CpmSetPostCode (CpmTpMsNvmePs3Method)
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPPX.NVME._PS3\n", 0, 0, 0, 0, 0, 0)
            }

            Name (_DSD, Package () {
                ToUUID("5025030F-842F-4AB4-A561-99A5189762D0"),
                Package () {
                    Package (2) {"StorageD3Enable", 1}, // 1 - Enable; 0 - Disable
                }
            }) //_DSD end
        }
    } // GPPX end
}
