/** @file

;******************************************************************************
;* Copyright (c) 2018 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

Name (EDPE, 1) // EDP Exist or not?

Scope (DGPU_SCOPE)
{
//    OperationRegion (NVID, PCI_Config, 0, 0x08)
//    Field (NVID, ByteAcc, Lock, Preserve)
//    {
//        Offset (0x02),
//        PDID, 16 // PCI Device ID
//    }

    OperationRegion (GPPB, PCI_Config, 0xB4, 0x14)
    Field (GPPB, ByteAcc, NoLock, Preserve)
    {
        PBCI, 8,   // Caps
        PBNP, 8,   // Next Ptr
        PBLS, 8,   // Size
        PBCC, 8,   // CONTROL
        PBCN, 32,  // Command
        PBDI, 32,  // Data In
        PBDO, 32,  // Data Out
        PBMX, 32   // Mutex
    }

//    Method (_INI)
//    {
//        Store (Zero, IGDS) // Integated Graphics Device Disabled
//    }

    // Method: NINT
    // Arguments:
    // Arg0: Type of command
    //       1 - Get Current Brightness level
    //       2 - to set brightness level
    // Arg1: Target brightness level. Percentage ranged from 0 to 100
    Method (NINT, 2, NotSerialized)
    {
        If (LEqual (PBCC, One))
        {
            If (LEqual (Arg0, One))
            {
                Store (One, PBMX)
                Store (Zero, Local0)
                While (LLess (Local0, 0x03E8))
                {
                    Store (0xC5100000, PBCN)
                    Stall (0x64)
                    And (PBCN, 0xFF, Local1)
                    Increment (Local0)
                    If (LEqual (Local1, One))
                    {
                        Store (0x03E8, Local0)
                    }
                }
                Store (Zero, PBCN)
                Store (Zero, PBMX)
                Store (PBDO, Local2)
                Return (Local2)
            }
            If (LEqual (Arg0, 0x02))
            {
                Store (Arg1, Local1)
                Store (Zero, Local0)
                Multiply (Local1, 0x0a, Local3)
                ShiftLeft (Local3, 0x08, Local3)
                Store (Local3, PBDI)
                While (LLess (Local0, 0x03E8))
                {
                    Store (One, PBMX)
                    Store (0x86100000, PBCN)
                    Stall (0x64)
                    And (PBCN, 0xFF, Local2)
                    Increment (Local0)
                    If (LEqual (Local2, One))
                    {
                        Store(0x03E8, Local0)
                    }
                }
                Store (Zero, PBCN)
                Store (Zero, PBMX)
                Return (Zero)
            }
        }
        Return (Zero)
    }

    Method (GBUS, 0x0)
    {
        Return (DGPU_BRIDGE_SCOPE.PRBN) // Return DGPU bus number.
    }

    //
    // For WMI wrapper code.
    // Return supported MXM version.
    // Arg0 = MXM version.
    //
    Method (MXMI, 1)
    {
        If (LEqual (Arg0, 0x20)) // 2.0?
        {
            Return (Arg0)
        }
        If (LEqual (Arg0, 0x21)) // 2.1?
        {
            Return (Arg0)
        }
        If (LEqual (Arg0, 0x30)) // 3.0?
        {
            Return (Arg0)
        }
        If (LEqual (Arg0, 0x0)) // Return highest supported version.
        {
            Return (0x30)
        }
        Return (0x0) // Other versions, not supported.
    }

    //
    // For WMI wrapper code.
    // Return MXM structure.
    // Arg0 = MXM version.
    //
    Method (MXMS, 1)
    {
        If (LEqual (Arg0, 0x30))
        {
            If (LNotEqual (MXBS, 0))
            {
                Name (MXM3, Buffer(MXBS) {0x00})
                Store (MXMB, MXM3)
                Return (MXM3)
            }
        }
        Else
        {
            Return (Buffer(8) {0x0}) // Return zeros.
        }
    }

    Name (CTOI, 1) // Current Toggle Index
    Name (NTOI, 1) // Next Toggle Index
    Name (MADL, 0) // Current attached display device
    Name (MSTE, 0) // Current actived display device
    Name (DPFL, 1) // DP or TMDS flag?
    Name (DSFL, 0) // Display Switch flag?
    Name (CHOI, 0) // choice Toggle Index

    Method (GETD, 2, SERIALIZED)
    {
        // Check current attached/actived display device status
        Store (Arg0, MADL)
        Store (Arg1, MSTE)
        // MDTL Index
        //  (0x01) HDMI only
        //  (0x02) CRT only
        //  (0x03) HDMI + CRT
        //  (0x04) DP only
        //  (0x05) HDMI + DP
        //  (0x06) CRT + DP
        //  (0x07) HDMI + CRT + DP
        //  (0x08) LCD only
        //  (0x09) HDMI + LCD
        //  (0x0A) CRT + LCD
        //  (0x0B) HDMI + CRT + LCD
        //  (0x0C) DP + LCD
        //  (0x0D) HDMI + DP + LCD
        //  (0x0E) CRT + DP + LCD
        // BitMap
        //  Bit0=1, HDMI port was connected
        //  Bit1=1, VGA port was connected
        //  Bit2=1, Display port was connected
        //  Bit3=1, LCD port was connected
        Switch (ToInteger (MADL))
        {
            case (0x0F) // HDMI + CRT + DP + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x0E) // CRT + DP + LCD
                    {
                        Store (0x0E, CTOI)
                        Store (0x01, NTOI) // HDMI
                    }
                    case (0x0F)
                    {
                        Store (0x08, CTOI) // LCD
                        Store (0x08, NTOI) // LCD
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 1), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x0F
            case (0x0E) // CRT + DP + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x0C) // DP + LCD
                    {
                        Store (0x0C, CTOI) // DP + LCD
                        Store (0x0E, NTOI) // CRT + DP + LCD
                    }
                    case (0x0E) // CRT + DP + LCD
                    {
                        Store (0x0E, CTOI) // CRT + DP + LCD
                        Store (0x02, NTOI) // CRT
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 2), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x0E
            case (0x0D) // HDMI + DP + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x0D) // HDMI + DP + LCD
                    {
                        Store (0x0D, CTOI) // HDMI + DP + LCD
                        Store (0x01, NTOI) // HDMI
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        And (CTOI, 0x1, Local0)
                        If (Local0)
                        {
                            Store (Add (CTOI, 3), NTOI)
                        }
                        Else
                        {
                            Store (Add (CTOI, 1), NTOI)
                        }
                    }
                }
            } // End of Switch MADL Case 0x0D
            case (0x0C) // DP + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x0C) // DP + LCD
                    {
                        Store (0x0C, CTOI) // DP + LCD
                        Store (0x04, NTOI) // DP
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 4), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x0C
            case (0x0B) // HDMI + CRT + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x03) // HDMI + CRT
                    {
                        Store (0x03, CTOI) // HDMI + CRT
                        Store (0x08, NTOI) // LCD
                    }
                    case (0x0B) // HDMI + CRT + LCD
                    {
                        Store (0x0B, CTOI) // HDMI + CRT + LCD
                        Store (0x01, NTOI) // HDMI
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 1), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x0B
            case (0x0A) // CRT + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x02) // CRT
                    {
                        Store (0x02, CTOI) // CRT
                        Store (0x08, NTOI) // LCD
                    }
                    case (0x08) // LCD
                    {
                        Store (0x08, CTOI) // LCD
                        Store (0x0A, NTOI) // CRT + LCD
                    }
                    case (0x0A) // CRT + LCD
                    {
                        Store (0x0A, CTOI) // CRT + LCD
                        Store (0x02, NTOI) // CRT
                    }
                }
            } // End of Switch MADL Case 0x0A
            case (0x09) // HDMI + LCD attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x01) // HDMI
                    {
                        Store (0x01, CTOI) // HDMI
                        Store (0x08, NTOI) // LCD
                    }
                    case (0x08) // LCD
                    {
                        Store (0x08, CTOI) // LCD
                        Store (0x09, NTOI) // HDMI + LCD
                    }
                    case (0x09) // HDMI + LCD
                    {
                        Store (0x09, CTOI) // HDMI + LCD
                        Store (0x01, NTOI) // HDMI
                    }
                }
            } // End of Switch MADL Case 0x09
            case (0x07) // HDMI + CRT + DP attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x07) // HDMI + CRT + DP
                    {
                        Store (0x07, CTOI) // HDMI + CRT + DP
                        Store (0x01, NTOI) // HDMI
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 1), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x07
            case (0x06) // CRT + DP attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x06) // CRT + DP
                    {
                        Store (0x06, CTOI)
                        Store (0x02, NTOI) // CRT
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 2), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x06
            case (0x05) // HDMI + DP attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x01) // HDMI
                    {
                        Store (0x01, CTOI)
                        Store (0x04, NTOI) // DP
                    }
                    case (0x04) // DP
                    {
                        Store (0x04, CTOI)
                        Store (0x05, NTOI) // HDMI + DP
                    }
                    case (0x05) // HDMI + DP
                    {
                        Store (0x05, CTOI)
                        Store (0x01, NTOI) // HDMI
                    }
                }
            } // End of Switch MADL Case 0x05
            case (0x03) // HDMI + CRT attached
            {
                Switch (ToInteger (MSTE))
                {
                    case (0x03) // HDMI + CRT
                    {
                        Store (0x03, CTOI)
                        Store (0x01, NTOI) // HDMI
                    }
                    default
                    {
                        Store (ToInteger (MSTE), CTOI)
                        Store (Add (CTOI, 1), NTOI)
                    }
                }
            } // End of Switch MADL Case 0x03
            default
            {
                Store (ToInteger (MSTE), CTOI)
                Store (CTOI, NTOI)
            } // End of Switch MADL Case default
        } // End of Switch MADL
    } // End of Method(GETD, 2, SERIALIZED)

    Method (MXM, 4, Serialized)
    {
        If (LNot (LEqual (Arg1, 0x300)))
        {
            Return (STATUS_ERROR_UNSUPPORTED) // Unsupported error.
        }

        Switch (ToInteger (Arg2))
        {
            //
            // Function 0: MXM_FUNC_MXSS
            //
            case (MXM_FUNC_MXSS)
            {
                // Sub-Functions 0, 5, 6, 16, 24, 25 are supported
                Return (ToBuffer (0x03010061))
            }

            //
            // Function 5: MXM_FUNC_MXDP - Get/Set Display Config.
            //
            case (MXM_FUNC_MXDP)
            {
                Name (TMP5, Buffer() {0x00, 0x00, 0x00, 0x00})
                CreateField (TMP5,  0,  4, DAVF)  // Display ACPI event
                CreateField (TMP5,  4,  1, LIDF)  // LID event state
                CreateField (TMP5,  8,  6, TOGN)  // Toggle List Sequence Number
                CreateField (Arg3,  0, 12, ATTD)  // Attached Displays
                CreateField (Arg3, 12, 12, ACTD)  // Active Displays
                CreateField (Arg3, 24,  1, DIMK)  // Display Mask
                CreateField (Arg3, 25,  5, NCSN)  // Next Combination Sequence Number
                CreateField (Arg3, 31,  1, NCSM)  // Next Combination Sequence Mask

                If (ToInteger (NCSM))
                {
                    // Next Combination Sequence Mask Set
                    Store (ToInteger (NCSN), NTOI)
                }
                Elseif (ToInteger (DIMK))
                {
                    // Display Mask Set
                    GETD (ToInteger (ATTD), ToInteger (ACTD))
                }
                If (ToInteger (DSFL))
                {
                    If (ToInteger (CHOI))
                    {
                        Store (ToInteger (CHOI), NTOI)
                        Store (0, CHOI)
                    }
                    Store (NTOI, TOGN)
                    Store (1, DAVF)
                }
                Else
                {
                    Store (CTOI, TOGN)
                    Store( 0, DAVF)
                }
                Store (0, DSFL)
                Return (TMP5)
            }

            //
            // Function 6: MXM_FUNC_MDTL - Get Display Toggle List.
            //
            case (MXM_FUNC_MDTL)
            {
                // Display Toggle List
                If (LEqual (EDPE, One))
                {
                    Return (Package() {
                        ACPI_ID_HDMI, 0x2C,                             // HDMI
                        ACPI_ID_CRT,  0x2C,                             // CRT
                        ACPI_ID_HDMI, ACPI_ID_CRT,  0x2C,               // HDMI + CRT
                        ACPI_ID_DP_B, 0x2C,                             // DPB
                        ACPI_ID_HDMI, ACPI_ID_DP_B, 0x2C,               // HDMI + DPB
                        ACPI_ID_CRT,  ACPI_ID_DP_B, 0x2C,               // CRT + DPB
                        ACPI_ID_HDMI, ACPI_ID_CRT,  ACPI_ID_DP_B, 0x2C, // HDMI + CRT + DPB
                        ACPI_ID_EDP,  0x2C,                             // LVDS
                        ACPI_ID_HDMI, ACPI_ID_EDP,  0x2C,               // HDMI + LVDS
                        ACPI_ID_CRT,  ACPI_ID_EDP,  0x2C,               // CRT + LVDS
                        ACPI_ID_HDMI, ACPI_ID_CRT,  ACPI_ID_EDP,  0x2C, // HDMI + CRT + LVDS
                        ACPI_ID_DP_B, ACPI_ID_EDP,  0x2C,               // DPB + LVDS
                        ACPI_ID_HDMI, ACPI_ID_DP_B, ACPI_ID_EDP,  0x2C, // HDMI + DPB + LVDS
                        ACPI_ID_CRT,  ACPI_ID_DP_B, ACPI_ID_EDP,  0x2C  // CRT + DPB + LVDS
                    })
                }
                Else
                {
                    Return (Package() {
                        ACPI_ID_HDMI, 0x2C,                             // HDMI
                        ACPI_ID_CRT,  0x2C,                             // CRT
                        ACPI_ID_HDMI, ACPI_ID_CRT,  0x2C,               // HDMI + CRT
                        ACPI_ID_DP_B, 0x2C,                             // DPB
                        ACPI_ID_HDMI, ACPI_ID_DP_B, 0x2C,               // HDMI + DPB
                        ACPI_ID_CRT,  ACPI_ID_DP_B, 0x2C,               // CRT + DPB
                        ACPI_ID_HDMI, ACPI_ID_CRT,  ACPI_ID_DP_B, 0x2C, // HDMI + CRT + DPB
                        ACPI_ID_LCD,  0x2C,                             // LVDS
                        ACPI_ID_HDMI, ACPI_ID_LCD,  0x2C,               // HDMI + LVDS
                        ACPI_ID_CRT,  ACPI_ID_LCD,  0x2C,               // CRT + LVDS
                        ACPI_ID_HDMI, ACPI_ID_CRT,  ACPI_ID_LCD,  0x2C, // HDMI + CRT + LVDS
                        ACPI_ID_DP_B, ACPI_ID_LCD,  0x2C,               // DPB + LVDS
                        ACPI_ID_HDMI, ACPI_ID_DP_B, ACPI_ID_LCD,  0x2C, // HDMI + DPB + LVDS
                        ACPI_ID_CRT,  ACPI_ID_DP_B, ACPI_ID_LCD,  0x2C  // CRT + DPB + LVDS
                    })
                }
            }

            //
            // Function 16: MXM_FUNC_MXMS
            //
            case (MXM_FUNC_MXMS)
            {
                If (LNotEqual (MXBS, 0))
                {
                    Name (MXM3, Buffer(MXBS) {0x00})
                    Store (MXMB, MXM3)
                    Return (MXM3)
                }
            }

            //
            // Function 24: MXM_FUNC_MXMI
            //
            case (MXM_FUNC_MXMI)
            {
                Return (ToBuffer (0x30))
            }

            //
            // Function 25: MXM_FUNC_MXCB
            //
            case (MXM_FUNC_MXCB)
            {
                Return (ToBuffer (0x600))
            }
        }
        Return (STATUS_ERROR_UNSUPPORTED) // Unsupported error.
    }

    //
    // Standard VGA device methods
    //
    Method (_DOD, 0)
    {
        If (LEqual (EDPE, One))
        {
            Return (Package() {
                // BitMap
                // Bit0=1, HDMI port was connected
                // Bit1=1, VGA port was connected
                // Bit2=1, Display port was connected
                // Bit3=1, LCD port was connected
                ACPI_ID_HDMI, // DPA HDMI
                ACPI_ID_DP_B, // DPB
                ACPI_ID_EDP,  // EDP
            })
        }
        Else
        {
            Return (Package() {
                ACPI_ID_HDMI, // DPA HDMI
                ACPI_ID_DP_B, // DPB
                ACPI_ID_LCD,  // LCD
            })
        }
    }

#if !FeaturePcdGet (PcdHgNvidiaDdsFeatureSupport)
    //
    // Display devices
    //
    Device (LCD0)
    {
        Name (_ADR, ACPI_ID_LCD)

        Method (MXMX, 1)
        {
            Return (0x1) // No mutex needed, returns success
        }

        Method (_DGS, 0) // Device Get State.
        {
            And (MSTE, 0x8, Local0)
            If (Local0)
            {
                Return (1)
            }
            Else
            {
                Return (0)
            }
        }

        Method (_BCL, 0)
        {
            // Note: According to the OS type to change the base brightness levels, Win8 = 10, Win7 = 7.
            // Need modify \_SB.PCI0._INI for update the OS type.
            If (LEqual (OSTB, 0x60))
            {
                // Note: Follow XPS to modify brightness level to 101 levels.
                Return (Package() {
                    100, 60, 0,
                     1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
                })
            }
            Else
            {
                Return (Package() {100, 60, 12, 24, 36, 48, 60, 72, 84, 100}) // taylor
            }
        }

        Method (_BCM, 1)
        {
//            Store (Arg0, BRTL)
            NINT (2, Arg0)
            If (LEqual (Arg0, 100))
            {
                Subtract (Arg0, 4, Arg0)
            }
            Divide (Arg0, 12, Local0, Local1)
            Decrement (Local1)
//            Store (Local1, EC_SCOPE.BRTS)
            Store (Local1, BRTL)
        }

        Method (_BQC, 0) // Brightness Query Current level.
        {
            Store (BRTL, Local0)
            Store (NINT (1, Local0), BRTL)
            Return (BRTL)
        }
    }
#endif

    Device (CRT0)
    {
        Name (_ADR, ACPI_ID_CRT)

        Method (MXMX, 1)
        {
            Return (0x1) // No mutex needed, returns success
        }

        Method (_DGS, 0)
        {
            And (MSTE, 0x2, Local0)
            If (Local0)
            {
                Return (1)
            }
            Else
            {
                Return (0)
            }
        }
    }

    Device (HDM0)
    {
        Name (_ADR, ACPI_ID_HDMI)

        Method (MXMX, 1)
        {
            Return (0x1) // No mutex needed, returns success
        }

        Method (_DGS, 0)
        {
            And (MSTE, 0x1, Local0)
            If (Local0)
            {
                Return (1)
            }
            Else
            {
                Return (0)
            }
        }
    }

    Device (DSP0) // DisplayPort
    {
        Name (_ADR, ACPI_ID_DP_B)

        Method (MXMX, 1)
        {
            Return (0x1) // No mutex needed, returns success
        }

        Method (_DGS, 0)
        {
            If (Lequal (DPFL, 0x1)) // DP connected?
            {
                And (MSTE, 0x4, Local0)
                If (Local0)
                {
                    Return (0x1)
                }
                Else
                {
                    Return (0x0)
                }
            }
            Else
            {
                Return (0x0)
            }
        }
    }

    Device (TMDS) // TMDS
    {
        Name (_ADR, ACPI_ID_TMDS)

        Method (MXMX, 1)
        {
            Return (0x1) // No mutex needed, returns success
        }

        Method (_DGS, 0)
        {
            If (LEqual (DPFL, 0x02))
            {
                And (MSTE, 0x4, Local0) // TMDS connected?
                If (Local0)
                {
                    Return (0x1)
                }
                Else
                {
                    Return (0x0)
                }
            }
            Else
            {
                Return (0x0)
            }
        }
    }

    Device (DSP1) // e-DisplayPort
    {
        Name (_ADR, ACPI_ID_EDP)

        Method (MXMX, 1)
        {
            Return (0x1) // No mutex needed, returns success
        }

        Method (_DGS, 0)
        {
            And (MSTE, 0x8, Local0)
            If (Local0)
            {
                Return (1)
            }
            Else
            {
                Return (0)
            }
        }

        Method (_BCL, 0)
        {
            If (LEqual (OSTB, 0x60))
            {
                // Note: Follow XPS to modify brightness level to 101 levels.
                Return (Package() {
                    100, 60, 0,
                     1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
                    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
                    81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100
                })
            }
            Else
            {
                Return (Package() {100, 60, 12, 24, 36, 48, 60, 72, 84, 100})
            }
        }

        Method (_BCM, 1)
        {
//            Store (Arg0, BRTL)
            NINT (2, Arg0)
            If (LEqual (Arg0, 100))
            {
                Subtract (Arg0, 4, Arg0)
            }
            Divide (Arg0, 12, Local0, Local1)
            Decrement (Local1)
//            Store (Local1, EC_SCOPE.BRTS)
            Store (Local1, BRTL)
        }

        Method (_BQC,0) // Brightness Query Current level.
        {
            Store (BRTL, Local0)
            Store (NINT (1, Local0), BRTL)
            Return (BRTL)
        }
    }
}
