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
    For GPS 2.0 support, SBIOS has to adjust below code to meet platform:
    * OEM/IBV should check "_customize" keyword for TO-DO *
    - \_ SB.PCI0.LPCB.EC.GPUP -> EC flag to release GPIO 12
*/

Scope (DGPU_SCOPE)
{
    Name (NLIM, Zero)     // OEM_customize
                          // System parameter limit update
                          // 0: No new limits
                          // 1: System requesting new limits (GPU Boost will call GPS_FUNC_PSHAREPARAMS)

    Name (PSLS, Zero)     // OEM_customize
                          // Power supply limit support
                          // 0: Basic AC/battery mode control only (default)
                          // 1: System uses both a GPIO and ACPI notification codes to set multiple GPU power limits (to support handling power supply overdraw events)

    Name (GPSP, Buffer(0x28) {})
    CreateDWordField (GPSP,  0, RETN)
    CreateDWordField (GPSP,  4, VRV1)
    CreateDWordField (GPSP,  8, TGPU)
    CreateDWordField (GPSP, 12, PDTS)
    CreateDWordField (GPSP, 16, SFAN)
    CreateDWordField (GPSP, 20, SKNT)
    CreateDWordField (GPSP, 24, CPUE)
    CreateDWordField (GPSP, 28, TMP1)
    CreateDWordField (GPSP, 32, TMP2)

    Method (GPS, 4, Serialized)
    {
        Store ("------- NV GPS DSM --------", Debug)
        If (LNotEqual (Arg1, 0x200))    // GPS_REVISION_ID
        {
            Return (STATUS_ERROR_UNSUPPORTED)
        }

        Switch (ToInteger (Arg2))    // GPS_FUNC_SUPPORT
        {
            //
            // Function 0: GPS_FUNC_SUPPORT - Bit list of supported functions.
            //
            Case (GPS_FUNC_SUPPORT)
            {
                Store ("   GPS fun 0", Debug)
                // Sub-Functions 0, 19, 32, 42 support
                Return (Buffer(0x08) {0x01, 0x00, 0x08, 0x00, 0x01, 0x04, 0x00, 0x00})
            }
            //
            // Function 19: Get system callbacks.
            //
            Case (GPS_FUNC_GETCALLBACKS)
            {
                Store ("   GPS fun 19", Debug)
                CreateDWordField (Arg3, Zero, TEMP)
                If (LEqual (TEMP, Zero))                 // first call , sbios need set which event had callback.
                {
                    Return (0x4)
                }
                If (LAnd (TEMP, 0x04))                   // bit 2 just for power state transition
                {
//                    Store (0x2, \_ SB.PCI0.LPCB.EC.GPUP) // IBV_customize: EC flag. Need to release GPIO12
                    Return (0x04)
                }
            }
            //
            // Function 32: GPS_FUNC_PSHARESTATUS - Get system requested Power Steering settings.
            //
            Case (GPS_FUNC_PSHARESTATUS)
            {
                Store ("   GPS fun 32", Debug)
                Name (RET1, Zero)                   // Notify dGPU 0xC0 then driver will query this sub-function
                CreateBitField (Arg3, 2, SPBI)      // SMBPBI (SMBUS to GPU postbox registers) interface support (GPU side)

                If (NLIM)
                {
                    Or (RET1, 0x00000001, RET1)     // bit[0]=1: System requesting new limits
                }

                If (PSLS)
                {
                    Or (RET1, 0x00000002, RET1)     // bit[1]=1: System uses both a GPIO and ACPI notification codes to set multiple GPU power limits
                }

                Return (RET1)
            }
            //
            // Function 42: GPS_FUNC_PSHAREPARAMS - Get sensor information and capabilities.
            //
            Case (GPS_FUNC_PSHAREPARAMS)
            {
                Store ("   GPS fun 42", Debug)
                CreateField (Arg3, 0, 4, PSH0)
                CreateBitField (Arg3, 8, GPUT)
                Store (0x00010000, VRV1)
                Switch (ToInteger (PSH0))
                {
                    Case (0) // Query Type 0: Get current status
                    {
                        Return (GPSP)
                    }
                    Case (1) // Query Type 1: Get supported fields
                    {
                        Store (0x00000100, RETN)
                        Or(RETN, ToInteger(PSH0), RETN)
                        Return (GPSP)
                    }
                    Case (2) // Query Type 2: Get Current opreting limits
                    {
                        Store (0x102, RETN)          // [3:0]=2:Request system to provide current system requested limits.  [8] to limit GPU target temperature
                        Store (0x00000000, TGPU)     // OEM_customize // The GPU Target Temperature must in range of "Product Specification". Set 'Zero' if no specific limit
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
