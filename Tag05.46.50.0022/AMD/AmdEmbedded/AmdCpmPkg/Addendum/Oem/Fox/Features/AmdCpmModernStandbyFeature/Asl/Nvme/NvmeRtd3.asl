/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "Rtd3Nvme.aml",  // Output file
    "SSDT",          // Signature
    0x01,            // SSDT Revision
    "AMD",           // OEM ID
    "NVMERTD3",      // OEM Table ID
    0x1              // OEM Revision
    )
{
    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.GPPX, DeviceObj)
    External(\_SB.PCI0.GPPX._ADR, IntObj)
    External(\_SB.PCI0.GPPX.WD3C, IntObj)

    #define ASL_GPIO_BASE    0xFED81500

    Scope(\_SB.PCI0.GPPX) {
        Name(PDEV, 0x00)
        Name(PFUN, 0x00)
        Name(PPWR, 0x00)
        Name(RSTM, 0x00)
        Name(CMSK, 0x00)
        Name(TPPL, 0xFFFF)
        Name(TRCF, 0xFFFF)
        Name(LCR1, 0xFFFFFFFF)
        Name(LCR2, 0xFFFFFFFF)

        Method(_INI) {
            CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX._INI\n", 0, 0, 0, 0, 0, 0)
            If (LNotEqual (CpmNvmeRtd3Table, 0)) {
                Store (0, Local0)                                                             // Initial Index
                Store (0, Local1)                                                             // Initial Offset
                Store (CpmReadTable (CpmNvmeRtd3Table, 0x11), Local2)                         // Get NvmeD3cConfigSize from NvmeList
                Store (CpmReadTable (CpmNvmeRtd3Table, 0x13), Local3)                         // Get Dev from NvmeList[0]
                While (LNotEqual(Local3, 0xFF)) {
                    Store (CpmReadTable (CpmNvmeRtd3Table, Add(Local1, 0x14)), Local4)        // Get Function from NvmeList[Index].Fun
                    If (LEqual(_ADR, Add(ShiftLeft(Local3, 16), Local4))) {
                        Store (Local3, PDEV)                                                  // Get PDEV from NvmeList[Index].Dev
                        Store (Local4, PFUN)                                                  // Get PFUN from NvmeList[Index].Fun
                        Store (CpmReadTable16 (CpmNvmeRtd3Table, Add(Local1, 0x15)), TPPL)    // Get TPPL from NvmeList[Index].Tpvperl
                        Store (CpmReadTable16 (CpmNvmeRtd3Table, Add(Local1, 0x17)), TRCF)    // Get TRCF from NvmeList[Index].Trstcfg
                        Store (CpmReadTable (CpmNvmeRtd3Table, Add(Local1, 0x19)), RSTM)      // Get RSTM from NvmeList[Index].RstAuxPin
                        ShiftLeft (RSTM, 2, RSTM)
                        Add (RSTM, ASL_GPIO_BASE, RSTM)
                        Store (CpmReadTable16 (CpmNvmeRtd3Table, Add(Local1, 0x1A)), PPWR)    // Get PPWR from NvmeList[Index].Power (EC RAM)
                        Store (CpmReadTable32 (CpmNvmeRtd3Table, Add(Local1, 0x1C)), CMSK)    // Get CMSK from NvmeList[Index].GppClkMask
                        break
                    }
                    Increment(Local0)                                                         // Update Index
                    Store (Multiply(Local0, Local2), Local1)                                  // Update Offset
                    Store(CpmReadTable (CpmNvmeRtd3Table, Add(Local1, 0x13)), Local3)         // Get next NvmeList[Index].Dev
                }
            }
        }

        // Turn on power
        Method(PWEN) {
            CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.PWEN\n", 0, 0, 0, 0, 0, 0)
            OperationRegion(vaMM, SystemMemory, RSTM, 0x4)
            Field(vaMM, ByteAcc, NoLock, Preserve) {
                offset(0x2),
                PRST, 8,
            }

            // MISC GPPCLKCONTROL CLK_REQ
            OperationRegion(vaMN, SystemMemory, 0xFED80E00, 0x4)
            Field(vaMN, ByteAcc, NoLock, Preserve) {
                CLKR, 32,
            }

            And (PRST, 0x01, Local0)                                                  // RESET GPIO
            Store (CpmReadGpio (PPWR), Local1)                                        // POWER GPIO
            Store (CLKR, Local2)                                                      // Save Clock Request
            IF (LAnd (LEqual (Local0, 0), LEqual (Local1, 0))) {
                Store(0x01, WD3C)                                                     // Set flag of wake from S0I3
                CpmWritePci32 (0, PDEV, PFUN, 0x88, LCR2)                             // Restore Link Control 2 register
                CpmWritePci32 (0, PDEV, PFUN, 0x68, LCR1)                             // Restore Link Control register
                And (CLKR, CMSK, CLKR)                                                // Disable Clock Request
                CpmProgramHoldTraining (0, PDEV, PFUN, 0)                             // Program Hold Training
                CpmWriteGpio (PPWR, 1)                                                // Power on Device
                Store (Local2, CLKR)                                                  // Enable Clock Request
                If (LNotEqual (TPPL, 0)) {
                    Sleep (TPPL)                                                      // Wait for Tpvperl
                } Else {
                    Sleep (100)                                                       // Delay 100 ms
                }
                Or (PRST, 0xC0, PRST)                                                 // De-assert Device Reset

                Store (CpmFindPcieRootPortNumber (0, PDEV, PFUN), Local4)             // Get PCIe Root Port Number
                If (LGreaterEqual (Local4, 5)) {
                    // PCIE1
                    Add (0x11240294, ShiftLeft (Subtract (Local4, 5), 12), Local6)    // LC_CURRENT_STATE
                } Else {
                    // PCIE0
                    Add (0x11140294, ShiftLeft (Local4, 12), Local6)                  // LC_CURRENT_STATE
                }
                Store (CpmReadSmnRegister (0, 0, 0, Local6), Local3)
                Store (2000, Local5)
                While (LAnd (LGreater (Local5, 0), LLess (And (Local3, 0x3F), 0x10))) {
                    Store (Subtract (Local5, 1), Local5)
                    Sleep (1)                                                         // Delay 1 ms, max loop 2 sec.
                    Store (CpmReadSmnRegister (0, 0, 0, Local6), Local3)              // LC_CURRENT_STATE
                }

                Sleep (TRCF)                                                          // Wait for Trst-cfg
                Store (CpmReadPci32 (0, PDEV, PFUN, 0x18), Local0)
                ShiftRight (And (Local0, 0xFF00), 8, Local1)
                Store (CpmReadPci32 (Local1, 0, 0, 0x00), Local1)
            }
        }

        // Turn off power
        Method(PWDI) {
            CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.PWDI\n", 0, 0, 0, 0, 0, 0)
            // Save Link Control Reg
            Store (CpmReadPci32 (0, PDEV, PFUN, 0x68), LCR1)
            CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.PWDI.LCR1 : %x\n", LCR1, 0, 0, 0, 0, 0)
            // Save Link Control 2 Reg
            Store (CpmReadPci32 (0, PDEV, PFUN, 0x88), LCR2)
            CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.PWDI.LCR2 : %x\n", LCR2, 0, 0, 0, 0, 0)
        }
    }
}
