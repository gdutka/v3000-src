/** @file

;******************************************************************************
;* Copyright (c) 2020 - 2021, Insyde Software Corp. All Rights Reserved.
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
    For Advanced Optimus aka. Dynamic Display Switching (DDS) support, doc ID: DA-09546-001. SBIOS has to adjust below code to meet platform:
    - MUX_MODE_PIN: PCH GPIO for display switch Mux
    - LCD_FORCE_RESET_PIN: PCH GPIO for LCD force reset pin
    - DIS_MODE_FLAG: ACPI flag reflect to SBIOS option. 0: Error, (hidden in SBIOS menu)
                                                        1: Integrated, (hidden in SBIOS menu. For debug only: dGPU disabled, switch the eDP MUX to iGPU)
                                                        2: Discrete, (iGPU disabled, switch the eDP MUX to dGPU)
                                                        3: MSHybrid, (traditional SG mode. Expose only if platform does not support Dynamin mode)
                                                        4: Dynamic (default, switch the eDP MUX to iGPU)
    - PWM_BL_FLAG: SBIOS/EC flag reflect to the PWM source for current backlight level
    - PWM_MAX_BL_FLAG: SBIOS/EC flag reflect to the PWM source for max backlight level
    - PWM_BL_SOURCE_FLAG: SBIOS/EC flag reflect to the PWM source. 0: Invalid brightness source,
                                                                   1: GPU PWM (either iGPU or dGPU),
                                                                   2: SBIOS/EC PWM,
                                                                   3: Panel (PWM over AUX or HDR)
    - The _DDC of LCD0 should be presented for dGPU/Dyanmic mode
*/

Scope (DGPU_SCOPE)
{
    /*
    * The internal compatible part ID shall be provided by NVIDIA
    * Compliant Mux device listed in the NVIDIA Advanced Optimus
    * (Dynamic Display Switching) Recommended Vendors List document (RVL-09552-001)
    * For example, Parade Mux PS8461E value should be 0x10003
    */
    Name (MMID, Package(2)
    {
        Package(3) {Zero, "PS8461E", 0x00010003},
        Package(3) {One, "NON-MUX or Error", Zero}
    })

    Method (_DOD, 0, NotSerialized)
    {
        Return (Package(1) {0x8000A450})                 //OEM_customize: _DOD for eDP
    }

    //
    // Operation region for SMI port access
    //
    OperationRegion (SMIP, SystemIO, 0xB0, 1)
    Field (SMIP, ByteAcc, NoLock, Preserve)
    {
      IOB0, 8
    }

    Device (LCD0)                                        // Declare the eDP from GPU, make sure it's not duplicated
    {
        Method (_ADR, 0, Serialized)
        {
            Return (0x8000A450)                          // OEM_customize: _ADR for eDP
        }
        Method (_DDC, 1, Serialized)
        {
            // Return (\_SB.HDDC (Arg0))                    // OEM_customize: return internal panel EDID
                                                            //Mandatory EDID read and cache in _DDC.
                                                            //And SBIOS should not alter EDID data captured
                                                            //from panel to avoid DDS function invalid
        }
        // Method (MXDS, 1, NotSerialized)
        // {
        //     Store (Arg0, Local0)
        //     And (Local0, 0x0F, Local1)
        //     And (Local0, 0x10, Local2)
        //     If (LEqual (Local1, Zero))                   // Get Mux State Input Format
        //     {
        //         If (LEqual (\_SB.GGOV (DMMP), Zero))     // DMMP = MUX_MODE_PIN
        //         {
        //             Return (One)                         // Return: Mux switched to iGPU
        //         }
        //         Else
        //         {
        //             Return (0x02)                        // Return: Mux switched to dGPU
        //         }
        //     }
        //     Else
        //     {
        //         If (LEqual (Local1, One))                // Set Mux State Input Format
        //         {
        //             If (LEqual (Local2, 0x10))
        //             {
        //                 \_SB.SGOV (DMMP, One)            // Switch Mux to dGPU
        //             }
        //             Else
        //             {
        //                 \_SB.SGOV (DMMP, Zero)           // Switch Mux to iGPU
        //             }
        //             Return (One)                         // Return Success
        //         }
        //         Else
        //         {
        //             Return (Zero)                        // Return Failure
        //         }
        //     }
        // }
        Method (MXDM, 1, NotSerialized)
        {
            Store (Arg0, Local0)
            And (Local0, 0x07, Local1)
            If (LEqual (Local1, Zero))                   // Get Mux State Operation
            {
                Store (DISM, Local2)                     // DISM = DIS_MODE_FLAG, Return: MUX mode for this display device
                Return (Local2)
            }
            Else
            {
                If (LLess (Local1, 0x05))                // Set Mux Mode Input Format
                {
                    Store (Local1, DISM)
                    Store (0x7C, IOB0)
                }
                Else
                {
                    Return (Zero)                        // Return Failure
                }
            }
            Return (One)                                 // Return Success
        }
        Method (MXID, 1, NotSerialized)
        {
            If (LEqual (Arg0, Zero))                     // Get Mux ID Input Format
            {
                Store (DerefOf (Index (DerefOf (Index (MMID, Zero)), 0x02)), Local0)
                Return (Local0)
            }
        }
        // Method (LRST, 1, NotSerialized)
        // {
        //     Store (Arg0, Local0)
        //     And (Local0, 0x07, Local1)
        //     If (LEqual (Local1, Zero))                   // Get LCD force reset state Operation
        //     {
        //         If (LEqual (\_SB.GGOV (DLRP), Zero))     // DLRP = LCD_FORCE_RESET_PIN
        //         {
        //             Return (One)                         // Set to LOW
        //         }
        //         Else
        //         {
        //             If (LEqual (\_SB.GGOV (DLRP), One))
        //             {
        //                 Return (0x02)                    // Set to HIGH
        //             }
        //             Else
        //             {
        //                 Return (Zero)                    // Return Error
        //             }
        //         }
        //     }
        //     Else
        //     {
        //         If (LEqual (Local1, One))                // Force LCD reset to LOW
        //         {
        //             \_SB.SGOV (DLRP, Zero)               // DLRP = LCD_FORCE_RESET_PIN
        //         }
        //         Else
        //         {
        //             If (LEqual (Local1, 0x02))           // Force LCD reset to HIGH
        //             {
        //                 \_SB.SGOV (DLRP, One)
        //             }
        //             Else
        //             {
        //                 Return (Zero)                    // Return Error
        //             }
        //         }
        //     }
        // }
    }
}

Scope (PCI_SCOPE)
{
    Device (AWMI)
    {
        Name (_HID, "PNP0C14")
        Name (_UID, "0x00")
        Name (_WDG, Buffer(0x28)
        {
            0x13, 0x96, 0x3E, 0x60, 0x25, 0xEF, 0x38, 0x43,
            0xA3, 0xD0, 0xC4, 0x61, 0x77, 0x51, 0x6D, 0xB7,
            0x41, 0x41, 0x01, 0x02, 0x21, 0x12, 0x90, 0x05,
            0x66, 0xD5, 0xD1, 0x11, 0xB2, 0xF0, 0x00, 0xA0,
            0xC9, 0x06, 0x29, 0x10, 0x30, 0x30, 0x01, 0x00
        })
        Method (WMAA, 3, Serialized)
        {
            Name (TEMP, Zero)
            CreateByteField (Arg2, Zero, MODF)
            CreateDWordField (Arg2, 0x04, LEDB)
            While (One)
            {
                Store (Arg1, TEMP)
                If (LEqual (TEMP, One))                   // Get/Set Current PWM Brightness
                {
                    If (LEqual (MODF, Zero))              // Get Current Backlight Level
                    {
                        // Return (PWM_BL_FLAG)           // OEM_customize
                    }
                    Else
                    {
                        If (LEqual (MODF, One))           // Set Current Backlight Level
                        {
                            // Store (LEDB, PWM_BL_FLAG)  // OEM_customize
                            Return (Zero)
                        }
                        Else
                        {
                            If (LEqual (MODF, 0x02))      // Get Max Backlight Level
                            {
                                // Return (PWM_MAX_BL_FLAG)  // OEM_customize
                            }
                            Else
                            {
                                Return (One)              // Return Failure
                            }
                        }
                    }
                }
                Else
                {
                    If (LEqual (TEMP, 0x02))              // Get/Set Display Brightness Source
                    {
                        If (LEqual (MODF, Zero))          // Get Current Display Brightness Source
                        {
                            // Return (PWM_BL_SOURCE_FLAG) // OEM_customize
                        }
                        Else
                        {
                            If (LEqual (MODF, One))       // Set/Clear Display Brightness Source Override
                            {
                                If (LLess (LEDB, 0x03))
                                {
                                    // Store (LEDB, PWM_BL_SOURCE_FLAG) // OEM_customize
                                    Return (Zero)         // Return Success
                                }
                                Return (One)              // Return Error
                            }
                            Else
                            {
                                Return (One)              // Return Error
                            }
                        }
                    }
                    Else
                    {
                        Return (One)
                    }
                }
                Break
            }
        }
    }
}
