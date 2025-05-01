/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
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
    For GPS support, SBIOS has to adjust below code to meet platform:
    - \_ SB.PCI0.LPCB.EC.GPUP -> EC flag to release GPIO 12
*/

Scope (DGPU_SCOPE)
{
    Name (NLIM, Zero)     // System parameter limit update
                          // 0: No new limits
                          // 1: System requesting new limits (GPU Boost will call GPS_FUNC_PSHAREPARAMS)

    Name (PSLS, Zero)     // Power supply limit support
                          // 0: Basic AC/battery mode control only (default)
                          // 1: System uses both a GPIO and ACPI notification codes to set multiple GPU power limits (to support handling power supply overdraw events)

    Name (CTGP, Zero)     // OEM_customize // Current GPU user opt-in configurable TGP mode status 
                          // 0: GPU user opt-in configurable TGP mode disabled
                          // 1: GPU user opt-in configurable TGP mode enabled

    Name (VPSC, One)      // Sample flag for GPS_FUNC_PCONTROL, IBV_customize/OEM_customize can change the mechanism per design

//    Name (PSAP, 0)

    Method (GPS, 4, Serialized)
    {
        // Only Interface Revision 0x0100 is supported
        If (LNotEqual (Arg1, 0x100))
        {
            Return (STATUS_ERROR_UNSUPPORTED)
        }

        // (Arg2) Sub-Function
        Switch (ToInteger (Arg2))
        {
            //
            // Function 0: GPS_FUNC_SUPPORT - Bit list of supported functions.
            //
            Case (GPS_FUNC_SUPPORT)
            {
//                // Sub-Functions 0, 19, 32, 33, 34, 35, 42 support
//                Return (Buffer(0x08) {0x01, 0x00, 0x08, 0x00, 0x0F, 0x04, 0x00, 0x00})
                // Sub-Functions 0, 19, 28, 32, 42 support
                Return (Buffer(0x08) {0x00, 0x00, 0x08, 0x10, 0x01, 0x04, 0x00, 0x00})
            }
//            //
//            // Function 19: Get system callbacks.
//            //
//            Case (GPS_FUNC_GETCALLBACKS)
//            {
//                CreateDWordField (Arg3, Zero, TEMP)
//                If (LEqual (TEMP, Zero))                 // first call , sbios need set which event had callback.
//                {
//                    Return (0x4)
//                }
//                And (TEMP, 0x0F, TEMP)                   // just mask bit 0-bit 3
//                IF (TEMP, 0x04)                          // bit 2 just for power state transition
//                {
//                    Store (0x2, \_ SB.PCI0.LPCB.EC.GPUP) // IBV_customize: EC flag. Need to release GPIO12
//                    Return (Arg3)
//                }
//            }
            //
            // Function 28: GPU power control.
            //
            Case (GPS_FUNC_PCONTROL)
            {
                CreateField (Arg3, Zero, 0x04, RTFS)
                CreateField (Arg3, 0x08, 0x08, VPS0)
                CreateField (Arg3, 0x18, 0x08, VPS1)
                If (LEqual (ToInteger (RTFS), Zero))      // Responding to SBIOS event (GPS_NOTIFY_PCONTROL)
                {
                    Store (0x02, Local0)                  // Request type=2; SBIOS request to set new vP-state
                    If (LEqual (VPSC, Zero))              // IBV_customize/OEM_customize, customer can change the mechanism per design
                    {
                        Or (0x0600, Local0, Local0)       // example to set max vP-state=6
                    }
                    Else
                    {
                        Or (Zero, Local0, Local0)         // example to set max vP-state=0
                    }
                    Return (Local0)
                }
                Else
                {
                    If (LEqual (ToInteger (RTFS), 0x02))  // Responding to SBIOS request to set new vP-state
                    {
                        Return (Zero)                     // After new vP-state set, driver will callback here
                    }
                }
            }
            //
            // Function 32: GPS_FUNC_PSHARESTATUS - Get system requested Power Steering settings.
            //
            Case (GPS_FUNC_PSHARESTATUS)
            {
/*
                CreateBitField (Arg3, 24, NRIT) // New Requested IGP turbo state (vaild if bits 25 is set)
                CreateBitField (Arg3, 25, NRIS) // Request new IGP turbo state (bit 24 is valid)
                CreateField (Arg3, 26, 3, PSTS) // Power Steering status (vaild if bits 29 is set)
                CreateBitField (Arg3, 29, NPSS) // Notify Power Steering status (bits 28:26 vaild)
                CreateBitField (Arg3, 30, PSES) // Power Steering enable status

                Name (GB32, Buffer(4) {0x00})
                CreateBitField (GB32,  0, SPLU) // system parameter limit update
                CreateBitField (GB32,  1, PSLS) // Power supply limit support
                CreateBitField (GB32, 23, RT23)
                CreateBitField (GB32, 24, CITS) // Current IGP turbo state
                CreateBitField (GB32, 30, PSER) // Power Steering enable request
                //
                // Request new IGP turbo state
                //
                If (NRIS)
                {
                    If (NRIT)
                    {
                        Store (1, CITS)
                    }
                    else
                    {
                        Store (0, CITS) // disable IGP turbo boost
                    }
                }
                //
                // Workaround: Always return IGP turbo boost enabled,
                // it because can't enable/disable or get IGP turbo boost state.
                //
//                Store (1, CITS)
                Store (1, PSER) // if this machine support GPS
                If (NLIM)
                {
                    //
                    // system requesting new limits (Power Steering will call GPS_FUNC_PSHAREPARAMS)
                    //
                    Store (1, SPLU)
                }
                IF (LEqual (\_SB.HWPV, 0x2))
                {
                    Store (1, RT23)
                }

                Return (GB32)
*/
                Name (RET1, Zero)                   // Notify dGPU 0xC0 then driver will query this sub-function
                CreateBitField (Arg3, 2, SPBI)      // SMBPBI (SMBUS to GPU postbox registers) interface support (GPU side)

                If (NLIM)
                {
                    Or (RET1, 0x00000001, RET1)     // System requesting new limits
                }

                If (PSLS)
                {
                    Or (RET1, 0x00000002, RET1)     // System uses both a GPIO and ACPI notification codes to set multiple GPU power limits
                }

                If (CTGP)
                {
                    Or(RET1, 0x00400000, RET1)      // GPU user opt-in configurable TGP mode enabled.
                }

                Return (RET1)
            }
/*
            //
            // Function 33: GPS_FUNC_GETPSS - Get _PPS object.
            //
            Case (GPS_FUNC_GETPSS)
            {
                Return (CPU0_SCOPE._PSS)
            }
            //
            // Function 34: GPS_FUNC_SETPPC - Set _PPC object.
            //
            Case (GPS_FUNC_SETPPC)
            {
                CreateDwordField (Arg3, 0, PCAP) // New GPS _PPC limit
                Store (PCAP, \_SB.CPPC)
                PNOT ()
                Store (PCAP, PSAP)
                Return (PCAP)
            }
            //
            // Function 35: GPS_FUNC_GETPPC - Get _PPC object.
            //
            Case (GPS_FUNC_GETPPC)
            {
                Return (PSAP) // Current GPS _PPC limit
            }
*/
            //
            // Function 42: GPS_FUNC_PSHAREPARAMS - Get sensor information and capabilities.
            //
            Case (GPS_FUNC_PSHAREPARAMS)
            {
//                CreateByteField (Arg3,  0, PSH0)
//                CreateByteField (Arg3,  1, PSH1)
//                CreateBitField  (Arg3,  8, GPUT)
//                CreateBitField  (Arg3,  9, CPUT)
//                CreateBitField  (Arg3, 10, FANS)
//                CreateBitField  (Arg3, 11, SKIN)
//                CreateBitField  (Arg3, 12, ENGR)
//                CreateBitField  (Arg3, 13, SEN1)
//                CreateBitField  (Arg3, 14, SEN2)
                CreateField (Arg3, 0, 4, PSH0)
                CreateBitField (Arg3, 8, GPUT)

                Name (GPSP, Buffer(0x24) {})
                CreateDWordField (GPSP,  0, RETN)
                CreateDWordField (GPSP,  4, VRV1)
                CreateDWordField (GPSP,  8, TGPU)
                CreateDWordField (GPSP, 12, PDTS)
                CreateDWordField (GPSP, 16, SFAN)
                CreateDWordField (GPSP, 20, SKNT)
                CreateDWordField (GPSP, 24, CPUE)
                CreateDWordField (GPSP, 28, TMP1)
                CreateDWordField (GPSP, 32, TMP2)
                Store (0x00010000, VRV1)

                Switch (ToInteger (PSH0))
                {
                    Case (0) // Query Type 0: Get current status
                    {
//                        If (CPUT)
//                        {
//                            Store (0x00000200, RETN)
//                            Or (RETN, ToInteger(PSH0), RETN)
//                            Store (\_TZ.TZ01._TMP, PDTS) // IBV_customize/OEM_customize: Please return CPU or EC tempture to PDTS
//                        }
                        Return (GPSP)
                    }
                    Case (1) // Query Type 1: Get supported fields
                    {
//                        // if GPU temperature is set by SBIOS, then store 0x300 to RETN
//                        Store (0x00000300, RETN)
                        Store (0x00000100, RETN)
                        Or(RETN, ToInteger(PSH0), RETN)
//                        Store (1000, PDTS) // minimum supported sample interval: 1000ms
                        Return (GPSP)
                    }
                    Case (2) // Query Type 2: Get Current opreting limits
                    {
//                        Store (0x102, RETN) // bit 0-3 need the same input value and set bit 8 for GPU temp
//                        Store (0x0000005C, TGPU)
//                        Store (0x00000000, PDTS)
//                        Store (0x00000000, SFAN)
//                        Store (0x00000000, CPUE)
//                        Store (0x00000000, SKNT)
//                        Store (0x00000000, TMP1)
//                        Store (0x00000000, TMP2)
//                        //
//                        // If Return bit is clear, the system doesn't currently wish to control this parameter,
//                        // Power Steering will use its default behavior for this parameter
//                        //
//                        Return(GPSP)
                        Store (0x102, RETN)          // [3:0]=2:Request system to provide current system requested limits.  [8] to limit GPU target temperature
                        Store (0x00000000, TGPU)     // The GPU Target Temperature must in range of "Product Specification". Set 'Zero' if no specific limit
                        Return (GPSP)
                    }
                }
            }
            default
            {
                //
                // FunctionCode or SubFunctionCode not supported
                //
                Return (STATUS_ERROR_UNSUPPORTED)
            }
        }
        Return (STATUS_ERROR_UNSUPPORTED)
    }
}
