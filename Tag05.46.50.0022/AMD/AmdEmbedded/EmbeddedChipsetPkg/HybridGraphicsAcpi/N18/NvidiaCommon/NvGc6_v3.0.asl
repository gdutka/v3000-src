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
    - This table is for GC6 3.0 support. Cannot co-exist with NvGc6_v2.x.asl
    - Hybrid display only, discrete mode is still under working
    - In sub-func#3 GPPC=0 case, customer need to config "PWGD" (power good) GPIO input pin.
*/
External(\_SB.APPM, MethodObj)
Scope (PCI_SCOPE)
{
    Name (TGPC, Buffer(0x04) {0x00}) // save GPC parameter
    Name (TDGC, Zero) // The flag is for DFGC : Defer GC6 Enter/Exit until D3-Cold
    Name (DGCX, Zero)
    Name (L23B, Buffer (5) {})  // L23 Buffer

    Method (GC6I, 0, Serialized)
    {
        Store ("   JT GC6I", Debug)

        Store (DGPU_BRIDGE_SCOPE.LREN, DGPU_SCOPE.LTRE)
        CreateWordField(L23B, 0, BSIZ)	 // Structure size field(2Byte)
        CreateField(L23B, 16, 3, BFUN)	 // func field
        CreateField(L23B, 19, 5, BDEV)	 // dev field
        CreateByteField(L23B, 3, BBUS)	 // bus field
        CreateByteField(L23B, 4, APPM)	 // 1=Enter_L23, 0=Exit from RC port
        Store(5, BSIZ)
        // Gpp Bridge 0 
        Store(0, BBUS)
        Store(1, BDEV)
        Store(1, BFUN)
        Store(1, APPM)   // 1=Enter_L23, 0=Exit from RC port
        \_SB.APPM (L23B)

        M010 (REST, One)
        Sleep (0x0A)
    }

    Method (GC6O, 0, Serialized)
    {
        M010 (REST, Zero)
        CreateWordField(L23B, 0, BSIZ)	 // Structure size field(2Byte)
        CreateField(L23B, 16, 3, BFUN)	 // func field
        CreateField(L23B, 19, 5, BDEV)	 // dev field
        CreateByteField(L23B, 3, BBUS)	 // bus field
        CreateByteField(L23B, 4, APPM)	 // 1=Enter_L23, 0=Exit from RC port
        Store(5, BSIZ)
        // Gpp Bridge 0 
        Store(0, BBUS)
        Store(1, BDEV)
        Store(1, BFUN)
        Store(0, APPM)   // 1=Enter_L23, 0=Exit from RC port
        \_SB.APPM (L23B)

        Or (DGPU_BRIDGE_SCOPE.CMDR, 0x4, DGPU_BRIDGE_SCOPE.CMDR)   // Bus Master Enable
        Store (DGPU_SCOPE.LTRE, DGPU_BRIDGE_SCOPE.LREN)
        Store (One, DGPU_BRIDGE_SCOPE.CEDR)
    }
}

Scope (DGPU_SCOPE)
{


    Method (NVJT, 4, SERIALIZED)
    {
        Store ("------- NV JT DSM --------", Debug)
        If (LLess (ToInteger (Arg1), JT_REVISION_ID))
        {
            Return (STATUS_ERROR_UNSPECIFIED)
        }

        Switch (ToInteger (Arg2))
        {
            //
            // Sub-Function 0: JT_FUNC_SUPPORT
            //
            case (JT_FUNC_SUPPORT)
            {
                //
                // Sub-Functions 0,1,3,4 are supported
                //
                Store ("   JT fun0 JT_FUNC_SUPPORT", Debug)
                Return (Buffer(4) {0x1B, 0x00, 0x00, 0x00})
            }
            //
            // Sub-Function 1: JT_FUNC_CAPS
            //
            case (JT_FUNC_CAPS)
            {
                Store ("   JT fun1 JT_FUNC_CAPS", Debug)
                Name (JTCA, Buffer(4) {0x00})
                CreateField (JTCA, 0,  1,  JTEN) // JT Enable
                CreateField (JTCA, 1,  2,  SREN) // NVSR Enable
                CreateField (JTCA, 3,  2,  PLPR) // Panel Power Rail
                CreateField (JTCA, 5,  1,  SRPR) // Self-Refresh Controller Power Rail
                CreateField (JTCA, 6,  2,  FBPR) // FB Power Rail
                CreateField (JTCA, 8,  2,  GUPR) // GPU Power Rail
                CreateField (JTCA, 10, 1,  GC6R) // GC6 ROM
                CreateField (JTCA, 11, 1,  PTRH) // Panic Trap Handler
                CreateField (JTCA, 13, 1,  MHYB) // MS Hybrid supported
                CreateField (JTCA, 14, 1,  RPCL) // Root Port control
                CreateField (JTCA, 15, 2,  GC6V) // GC6 Version
                CreateField (JTCA, 17, 1,  GEIS) // GC6 Exit ISR Support flag
                CreateField (JTCA, 18, 1,  GSWS) // GC6 Self Wakeup Support
                CreateField (JTCA, 20, 12, JTRV) // JT Revision

                Store (1, JTEN) // JT Enable
                Store (0, GC6R) // GC6 integrated ROM
                Store (1, RPCL) // Supports fine grain Root Port Control PLON, PRPC
                Store (1, SREN) // 1 = NVSR disabled.

                Store (0, FBPR)
                Store (1, MHYB) // IBV_customize/OEM_customize: 0: discrete NVSR support, 1: MShybrid
                                // Supports deferred GC6Enter/Exit in _PSx/_PRx, as per DFGC bits

                Store (0x2, GC6V)   // Support GC6-R
                Store (0x200, JTRV) // Update NVJT _DSM version to 0x200 for RTD3-GC6

                Return (JTCA)
            }
            //
            // Sub-Function 2: JT_FUNC_POLICYSELECT
            //
            case (JT_FUNC_POLICYSELECT)
            {
                Store("   JT fun2 JT_FUNC_POLICYSELECT", Debug)
                Return (STATUS_ERROR_UNSUPPORTED)
            }
            //
            // Sub-Function 3: JT_FUNC_POWERCONTROL
            //
            case (JT_FUNC_POWERCONTROL)
            {
                Store("   JT fun3 JT_FUNC_POWERCONTROL", Debug)
                CreateField (Arg3,  0, 3, GPPC) // GPU Power Control
                CreateField (Arg3,  4, 1, PLPC) // Panel Power Control
                CreateField (Arg3,  7, 1, ECOC) // Notify on complete
                CreateField (Arg3, 14, 2, DFGC) // Defer GC6 Enter/Exit until D3-Cold
                CreateField (Arg3, 16, 3, GPCX) // Deferred GC6 Exit Control
                Store (Arg3, PCI_SCOPE.TGPC) // Store GC6 control input for GC6I GC6O

                If (LOr (LNotEqual (ToInteger (GPPC), Zero), LNotEqual (ToInteger (DFGC), Zero)))
                {
                    Store (ToInteger (DFGC), PCI_SCOPE.TDGC) // Defer GPC & GPCX to be processed when setting Device Power State (D0/D3-Hot/Cold)
                    Store (ToInteger (GPCX), PCI_SCOPE.DGCX)
                }

                Name (JTPC, Buffer(4) {0x00})
                CreateField (JTPC, 0, 3, GUPS) // dGPU Power Status
                CreateField (JTPC, 3, 1, GPWO) // dGPU Power OK Status
                CreateField (JTPC, 7, 1, PLST) // Panel State

                If (LNotEqual (ToInteger (DFGC), Zero))
                {
                    // dGPU On
                    Store(One, GPWO)
                    Store(One, GUPS)
                    Return(JTPC)
                }

                Store ("   JT fun3 GPPC=", Debug)
                Store (ToInteger(GPPC), Debug)

                If (LEqual (ToInteger (GPPC), One))
                {
                    //
                    // Enter GC6 and no Panel Self-Refresh
                    // Panel is powered down with GPU
                    //
                    PCI_SCOPE.GC6I () // dGPU enter GC6 entry
                    Store (One, PLST)  // Panel Status is powered off
                    Store (Zero, GUPS) // dGPU Status is Completely Off
                }
                ElseIf (LEqual (ToInteger (GPPC), 0x02))
                {
                    //
                    // Enter GC6 and stay in Panel Self-Refresh
                    // Panel and FB remain powered while GPU is shutdown
                    //
                    PCI_SCOPE.GC6I () // dGPU through EC enter GC6
                    If (LEqual (ToInteger (PLPC), Zero)) // Panel remains powered while GPU is in GC6
                    {
                        Store (Zero, PLST) // Panel Status is powered on
                    }
                    Store (Zero, GUPS) // GC6 (dGPU off w/FB_CLAMP assert)
                }
                ElseIf (LEqual (ToInteger (GPPC), 0x03))
                {
                    //
                    // Exit GC6 and exit Self-Refresh
                    // GPU is powered on, modeset and link train, GPU drives FB to the screen
                    //
                    PCI_SCOPE.GC6O () // dGPU through EC exit GC6
                    //
                    // The panel always powered on in Optimus platform
                    //
                    If (LNotEqual (ToInteger (PLPC), Zero)) // Panel remains powered while GPU is in GC6
                    {
                        //
                        // Power On the Panel here
                        //
                        Store (Zero, PLST) // Panel is powered on
                    }
                    // dGPU On
                    Store (One, GPWO)
                    Store (One, GUPS)
                }
                ElseIf (LEqual (ToInteger (GPPC), 0x04))
                {
                    //
                    // Exit GC6 but remain in Self-Refresh
                    // GPU is powered on, modeset and link train, SRC continues to drive the screen
                    //
                    PCI_SCOPE.GC6O () // dGPU through EC exit GC6
                    //
                    // The panel always powered on in Optimus platform
                    //
                    If (LNotEqual (ToInteger (PLPC), Zero)) // Panel remains powered while GPU is in GC6
                    {
                        Store (Zero, PLST) // Panel is powered on
                    }
                    // dGPU On
                    Store (One, GPWO)
                    Store (One, GUPS)
                }
                Else
                {
                    //
                    // Get the current GPU GCx Sleep Status
                    //
                    If (LEqual (M009(PWGD), One))
                    {
                        Store ("   JT GETS() return 0x1", Debug)
                        Store (One, GPWO)
                        Store (One, GUPS)
                    }
                    Else
                    {
                        Store ("   JT GETS() return 0x3", Debug)
                        Store (Zero, GPWO)
                        Store (0x3, GUPS)
                    }
                }
                Return (JTPC)
            }
            //
            // Sub-Function 4: JT_FUNC_PLATPOLICY
            //
            case (JT_FUNC_PLATPOLICY)
            {
                Store ("   JT fun4 JT_FUNC_PLATPOLICY", Debug)
                CreateField (Arg3, 2, 1, PAUD) // AUD : Azalia Audio Device
                CreateField (Arg3, 3, 1, PADM) // ADM : Audio Disable Mask
                CreateField (Arg3, 4, 4, PDGS) // DGS : Driver expected State Mask

                Store (Zero, Local0)
                ShiftLeft (DGPU_SCOPE.HDAE, 0x02, Local0)   // Returns the currnet status of the audio device
                Return (Local0)
            }
        }
        Return (STATUS_ERROR_UNSUPPORTED)
    }
}
