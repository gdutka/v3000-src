/** @file

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
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
    For NVPCF 2.0 support, SBIOS has to adjust below code to meet platform:
    * OEM/IBV should check "_customize" keyword for TO-DO *
    - Please refer to NVIDIA SW design guide (DG-09954-001) to generate the NVPCF buffer in sub-func#1
      Remember to update the checksum byte (Sum of Bytes and 2s Complement) if NVPCF buffer changed
    - NVPCF is a virture device under \_SB, do not change the device path
    - Support CTGP Only, check "CTGP_ONLY_SUPPORT" keyword for TO-DO.
      The CTGP Mode2-only controller is automatically disabled when the system is running on DC,
      and automatically enabled when on AC.
      When the CTGP offset (ACBT -> AC:Baseline TGP) is zero, then CTGP Mode 2 is considered disabled,
      even if the controller is enabled.
    - OEM has to create a EC IO address by "IOBS" for the DYNAMIC BOOST 2.0 access
    - for Dynamic Boost, OEM has to update the objects which might be returned by _CPC method in ASL,
      including but not limited to CPC2 and CPOC. Please consult NVIDIA SW AE for more detail
    - Whisper mode 2.0 (WM2)
      a. Require DYNAMIC BOOST 2.0
      b. OEM should to update some information to support
*/

Scope (\_SB)                            // DO NOT MODIFY THE PATH
{
    Device (NPCF)
    {
        // OEM_customize Start
        Name(ACBT, 0x0050)              // AC:Baseline TGP (1/8-watt units)
                                        // +10W => 80
#if !FixedPcdGetBool (PcdHgNvidiaN20CTGPFeatureSupport)
        Name(DCBT, 0x0000)              // DC:Baseline TGP (1/8-watt units)
                                        // Set Disable => 0

        Name(DBAC, Zero)                // Dynamic Boost on AC mode
                                        // 0: Enable
                                        // 1: Disable
        Name(DBDC, One)                 // Dynamic Boost on DC mode
                                        // 0: Enable
                                        // 1: Disable

        Name(AMAT, 0x0078)              // Maximum TGP on AC offset from baseline (1/8-watt units)
                                        // +15W => 120
        Name(AMIT, 0xFF88)              // Minimum TGP on AC offset frmo baseline (1/8-watt units)
                                        // -15W => -120

        Name(ATPP, 0x0168)              // Total Processing Power Target on AC offset from baseline (1/8-watt units)
                                        // +45W => 360
        Name(DTPP, 0x0000)              // Total Processing Power Target on DC offset from baseline (1/8-watt units)
                                        // Set Disable => 0
#else
        Name(DCBT, 0x0000)              // DC:Baseline TGP (1/8-watt units)
                                        // CTGP Only must be 0

        Name(DBAC, Zero)                // Dynamic Boost on AC mode
                                        // CTGP Only must be 0

        Name(DBDC, Zero)                // Dynamic Boost on DC mode
                                        // CTGP Only must be 0

        Name(AMAT, 0x0000)              // Maximum TGP on AC offset from baseline (1/8-watt units)
                                        // CTGP Only must be 0
        Name(AMIT, 0x0000)              // Minimum TGP on AC offset frmo baseline (1/8-watt units)
                                        // CTGP Only must be 0

        Name(ATPP, 0x0000)              // Total Processing Power Target on AC offset from baseline (1/8-watt units)
                                        // CTGP Only must be 0
        Name(DTPP, 0x0000)              // Total Processing Power Target on DC offset from baseline (1/8-watt units)
                                        // CTGP Only must be 0
#endif
        Name(IOBS, Zero)                // ** EC IO. Example port: 0x25F

        Name(CMPL,0x33)                 // ** CPPC Maximum Performance Level, MSR 0x1AD value
        Name(CNPL,0x10)                 // CPPC Nominal Performance Level
        // OEM_customize End

        Name(HPCT, 2)                   // Get Control Type of CPPC parameters
                                        // 0: NVIDIA Default
                                        // 1: Reserved
                                        // 2: Embedded Controller IO Space

        Name(CDIS, 0)                   // device disable state

        Name(CUSL, 0)                   // Save Set User Controllable SL Target

        Name(CUCT, 0)                   // Save Set CPU Tj limit Target

        Method (_HID) {                 // ACPI HW ID for NVPCF
            Store (0, CDIS)             // Clear Disabled bit
            Return("NVDA0820")
        }
        Name (_UID, "NPCF")
        Method (_STA, 0, NotSerialized)
        {
            If (LEqual (CDIS, 1))
            {
                Return (0xD)            // Disabled from _DIS 1101b - Present/Disabled (via Device Manager)
            }
            Return (0xF)
        }
        Method (_DIS, 0x0, NotSerialized)
        {
            Store (1, CDIS)             // Set Disabled bit
        }
        Method (_DSM, 4, Serialized)
        {
            // NVPCF_DSM_GUID = 36b49710-2483-11e7-9598-0800200c9a66
            If (LEqual (Arg0, ToUUID ("36b49710-2483-11e7-9598-0800200c9a66")))
            {
                If (LNotEqual (DGPU_SCOPE.NPCS, Zero))
                {
                    Return (NPCF (Arg0, Arg1, Arg2, Arg3))
                }
            }
        }
        Method (NPCF, 4, Serialized)
        {
            Store ("------- NVPCF DSM --------", Debug)
            // Revision 0x200 is supported
            If (LNotEqual (ToInteger (Arg1), 0x200))
            {
                Return (STATUS_ERROR_UNSPECIFIED)
            }
            Switch (ToInteger (Arg2))
            {
                //
                // Sub-Function 0: NVPCF_FUNC_GET_SUPPORTED
                //
                Case (NVPCF_FUNC_GET_SUPPORTED)
                {
                    Store ("   NVPCF sub-func#0", Debug)

#if !FixedPcdGetBool (PcdHgNvidiaN20CTGPFeatureSupport)
                      // sub-func: 0, 1, 2, 3, 4, 5, 6, 7, 8, and 9 supported
                      Return (Buffer() {0xFF, 0x03, 0x00, 0x00})
#else
                      // sub-func: 0, 1, 2 and 7 supported
                      Return (Buffer() {0x87, 0x00, 0x00, 0x00})
#endif
                }

                //
                // Sub-Function 1: NVPCF_FUNC_GET_STATIC_CONFIG_TABLES
                //
                Case (NVPCF_FUNC_GET_STATIC_CONFIG_TABLES)
                {
                    Store ("   NVPCF sub-func#1", Debug)
                    //OEM_customize for CPU type
                    Return (Buffer() {
                        // System Device Table Header (2.0)
                        0x20,0x03,0x01,
                        // System Device Table Entries
                        //   Intel + NVIDIA
                        //   [3:0] CPU Type
                        //     0x0 = Intel CPU
                        //     0x1 = AMD CPU
                        //   [7:4] GPU Type
                        //     0x0 = NVIDIA GPU
                        // OEM_customize need to modify the CPU type
                        0x00,
                        // System Controller Table Header (2.2)
                        //   One Controller Entries
                        0x22,0x04,0x05,0x01,
                        // System Controller Table Entry 0
                        //   Type = Controller Class
                        //     0x0 = Entry Disabled
                        //     0x1 = Dynamic Boost Controller
                        //     0x2 = Configurable TGP-only Controller
                        // OEM_customize need modify to 2 if support CTGP Only
#if !FixedPcdGetBool (PcdHgNvidiaN20CTGPFeatureSupport)
                        0x01,
                        //   Param0: 0x00000001
                        //     Class = Dynamic Boost Controller
                        //     [0:0] DC Support
                        //       0x0 = Not Supported
                        //       0x1 = supported
                        // OEM_customize need to modify the Dynamic Boost support DC mode
                        0x01,0x00,0x00,0x00,
                        // Single byte checksum value, Sum of Bytes and 2s Complement
                        // OEM_customize needs to update if have any modify the above data.
                        0xAE
#else
                        0x02,
                        //   Param0: 0x00000000
                        //     Class = CTGP Only
                        //       Reserved. Must be 0
                        0x00,0x00,0x00,0x00,
                        // Single byte checksum value, Sum of Bytes and 2s Complement
                        // OEM_customize needs to update if have any modify the above data.
                        0xAE
#endif
                    })
                }

                //
                // Sub-Function 2: NVPCF_FUNC_UPDATE_DYNAMIC_PARAMS (Update PPAB params)
                //
                Case (NVPCF_FUNC_UPDATE_DYNAMIC_PARAMS)
                {
                    Store("   NVPCF sub-func#2", Debug)
                    Name(PBD2, Buffer(0x31) {0x00})
                    CreateByteField(PBD2, Zero, PTV2)   // Table Version
                    CreateByteField(PBD2, 0x01, PHB2)   // Table Header in bytes
                    CreateByteField(PBD2, 0x02, GSB2)   // Size of Common Status in bytes
                    CreateByteField(PBD2, 0x03, CTB2)   // Size of Controller Entry in bytes
                    CreateByteField(PBD2, 0x04, NCE2)   // Number of controller Entries
                    // Dynamic Params Table Header
                    //   Table Version (0x22). (Value should match the System Controller Table Header)
                    //   Table Header in bytes (0x5)
                    //   Size of Common Status in bytes (0x10)
                    //   Size of Controller Entry in bytes (0x1C)
                    //   Number of controller Entries (0x1)
                    Store(0x22,PTV2)
                    Store(0x05,PHB2)
                    Store(0x10,GSB2)
                    Store(0x1C,CTB2)
                    Store(0x01,NCE2)
                    CreateWordField(PBD2, 0x05, TGPA)   // Params0 [15:0] of Common Status, Output
                    CreateWordField(PBD2, 0x07, TGPD)   // Params0 [31:16] of Common Status, Output
                    CreateByteField(PBD2, 0x15, PC01)   // Params0 [7:0] of controller Entry, Output
                    CreateByteField(PBD2, 0x16, PC02)   // Params0 [15:8] of controller Entry, Output
                    CreateWordField(PBD2, 0x19, TPPA)   // Params1 [15:0] of controller Entry, Output
                    CreateWordField(PBD2, 0x1B, TPPD)   // Params1 [31:16] of controller Entry, Output
                    CreateWordField(PBD2, 0x1D, MAGA)   // Params2 [15:0] of controller Entry, Output
                    CreateWordField(PBD2, 0x1F, MAGD)   // Params2 [31:16] of controller Entry, Output
                    CreateWordField(PBD2, 0x21, MIGA)   // Params3 [15:0] of controller Entry, Output
                    CreateWordField(PBD2, 0x23, MIGD)   // Params3 [31:16] of controller Entry, Output
                    CreateDWordField(PBD2, 0x25, PA4O)  // Params4 [31:0] of controller Entry, Output
                    CreateDWordField(PBD2, 0x29, PA5O)  // Params5 [31:0] of controller Entry, Output
                    CreateDWordField(PBD2, 0x2D, PA6O)  // Params6 [31:0] of controller Entry, Output

                    CreateField(Arg3, 0x28, 2, NIGS)// Params0 [1:0] of Common Status, Input
                    CreateByteField(Arg3, 0x15, IORC)   // Params0 [7:0] of controller Entry, Input
                    CreateField(Arg3, 0xB0, 1, PWCS)// Params0 [8:8] of controller Entry, Input
                    CreateField(Arg3, 0xB1, 1, PWTS)// Params0 [9:9] of controller Entry, Input
                    CreateField(Arg3, 0xB2, 1, CGPS)// Params0 [10:10] of controller Entry, Input

                    // Params0 of Common Status, Input
                    //   [1:0] Input Command
                    //     0x0 = Get Controller Params
                    If(LEqual(ToInteger(NIGS), 0x0))
                    {
                        // Dynamic Params Common Status, Output
                        //   [15:0] Baseline TGP on AC
                        //     0x00 -> Disable
                        //   expressed as a signed offset from default TGP in 1/8-watt units
                        Store(ACBT,TGPA)
                        // Dynamic Params Common Status, Output
                        //   [31:16] Baseline TGP on DC
                        //     0x00 -> Disable
                        //   expressed as a signed offset from default TGP in 1/8-watt units
                        Store(DCBT,TGPD)
#if !FixedPcdGetBool (PcdHgNvidiaN20CTGPFeatureSupport)                        
                        // Dynamic Params Controller Entry, Output (for Dynamic Boost Controller)
                        //   Index: 0
                        //   Params0 [7:0] Index of referenced controller
                        // OEM_customize, Need modify index number if have one more system control table.
                        Store(0x00,PC01)
                        //           [8:8] Enable/disable controller on AC
                        //             0x0 = Enable, 0x1 = Disable
                        //           [9:9] Enable/disable controller on DC
                        //             0x0 = Enable, 0x1 = Disable
                        Or(DBAC, ShiftLeft(DBDC, 1), PC02)
                        //   Params1 [15:0] Total Processing Power Target on AC,
                        Store(ATPP,TPPA)
                        //   Params1 [31:16] Total Processing Power Target on DC,
                        Store(DTPP,TPPD)
                        //   Params2 [15:0] Maximum TGP on AC,
                        Store(AMAT,MAGA)
                        //   Params3 [15:0] Minimum TGP on AC,
                        Store(AMIT,MIGA)
#endif
                    }

                    // Params0 of Common Status, Input
                    //   [1:0] Input Command
                    //     0x1 = Set Controller Params
                    If(LEqual(ToInteger(NIGS), 0x1))
                    {
                        // Dynamic Params Controller Entry, Input
                        //   Params0 [7:0] Index of referenced controller
                        //           [8:8] Power control capability status of the referenced controller
                        //             0x0 = Disabled; controller is currently unable to control power
                        //             0x1 = Enabled; controller is currently able to control power
                        //           [9:9] Power transfer status
                        //             0x0 = Power transfer disabled
                        //             0x1 = Power transfer enabled
                        //           [10:10] CTGP status of referenced controller
                        //             0x0 = CTGP control disabled, no offset applied
                        //             0x1 = CTGP control enabled, offset applied (requested offset may be zero)
                        //OEM_customize Start 
                        // This means NVPCF is ready to control power, even if Dynamic Boost / Qboost is currently disabled.
                        // NVPCF is not immediately ready after driver load; It cannot control power if it cannot communicate with the GPU driver.
                        If (LEqual(ToInteger(PWCS), 0x1))
                        {
                            //Power control capability Enabled 
                        } Else {
                            //Power control capability Disabled 
                        }

                        // This means Dynamic Boost/Qboost is currently enabled/active or disable/inactive.
                        // Basically Dynamic Boost has to be enabled *and* PWCS(bit8) has to be 1.
                        If (LEqual(ToInteger(PWTS), 0x1))
                        {
                            //Power transfer Enabled 
                        } Else {
                            //Power transfer Disabled
                        }

                        // This means CTGP offset is applied (even if the offset is 0).  PWCS(Bit8) has to be 1.
                        If (LEqual(ToInteger(CGPS), 0x1))
                        {
                            //CTGP control Enabled 
                        } Else {
                            //CTGP control Disabled 
                        }
                        //OEM_customize End

                        // Set to All Zero when the Input Command is "0x1" 
                        Store(0x00,TGPA)
                        Store(0x00,TGPD)
                        Store(0x00,PC01)
                        Store(0x00,PC02)
                        Store(0x0000,TPPA)
                        Store(0x0000,TPPD)
                        Store(0x0000,MAGA)
                        Store(0x0000,MIGA)
                        Store(0x0000,MAGD)
                        Store(0x0000,MIGD)
                    }
                    Return (PBD2)
                }

                //
                // Sub-Function 3: NVPCF_FUNC_GET_WM2_TBAND_TABLES
                //
                Case (NVPCF_FUNC_GET_WM2_TBAND_TABLES)
                {
                    Store("   NVPCF sub-func#3", Debug)
                    //OEM_customize
                    Return (Buffer() {
                        // Tband Table Header
                        0x11,0x04,0x13,0x03,
                        // Tband table entry 0 (CPU Tj, CPU Fan Group)
                        //   Flag0: [1:0] Temp Source
                        //            0 = CPU Tj
                        //          [2:2] Fan Group Control
                        //            0 = CPU Fan Group
                        //          [7:3] Reserved. Must be 0
                        //   Params0: Buffer(), 0xFF = Invalid Temperature Entry
                        //     Index | Tj-Down | Tj-Up | CPU Fan Group
                        //       0   |    x    |   0   |  CFG0
                        //       1   |    40   |   45  |  CFG1
                        //       2   |    45   |   51  |  CFG2
                        //       3   |    51   |   57  |  CFG3
                        //       4   |    57   |   63  |  CFG4
                        //       5   |    63   |   69  |  CFG5
                        //       6   |    66   |   75  |  CFG6
                        //       7   |    70   |   80  |  CFG7
                        //       8   |    x    |   x   |  x
                        0x00,
                        0xFF,0x00,0x28,0x2D,0x2D,0x33,0x33,0x39,0x39,0x3F,0x3F,0x45,0x42,0x4B,0x46,0x50,0xFF,0xFF,
                        // Tband table entry 1 (GPU Tj, GPU Fan Group)
                        //   Flag0: [1:0] Temp Source
                        //            1 = GPU Tj
                        //          [2:2] Fan Group Control
                        //            1 = GPU Fan Group
                        //          [7:3] Reserved. Must be 0
                        //   Params0: Buffer(), 0xFF = Invalid Temperature Entry
                        //     Index | Tj-Down | Tj-Up | GPU Fan Group
                        //       0   |    x    |   0   |  GFG0
                        //       1   |    60   |   65  |  GFG1
                        //       2   |    65   |   70  |  GFG2
                        //       3   |    70   |   75  |  GFG3
                        //       4   |    x    |   x   |  x
                        //       5   |    x    |   x   |  x
                        //       6   |    x    |   x   |  x
                        //       7   |    x    |   x   |  x
                        //       8   |    x    |   x   |  x
                        0x05,
                        0xFF,0x00,0x3C,0x41,0x41,0x46,0x46,0x48,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                        // Tband table entry 2 (Alternate Temp, CPU Fan Group)
                        //   Flag0: [1:0] Temp Source
                        //            2 = Alternate Temp
                        //          [2:2] Fan Group Control
                        //            0 = CPU Fan Group
                        //          [7:3] Reserved. Must be 0
                        //   Params0: Buffer(), 0xFF = Invalid Temperature Entry
                        //     Index | Tj-Down | Tj-Up | CPU Fan Group
                        //       0   |    x    |   x   |  x
                        //       1   |    x    |   x   |  x
                        //       2   |    x    |   x   |  x
                        //       3   |    x    |   x   |  x
                        //       4   |    x    |   x   |  x
                        //       5   |    x    |   x   |  x
                        //       6   |    x    |   0   |  CFG6
                        //       7   |    48   |   52  |  CFG7
                        //       8   |    52   |   58  |  CFG8
                        0x02,
                        0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x30,0x34,0x34,0x3A
                    })
                }

                //
                // Sub-Function 4: NVPCF_FUNC_GET_WM2_SL_MAP_TABLES
                //
                Case (NVPCF_FUNC_GET_WM2_SL_MAP_TABLES)
                {
                    Store("   NVPCF sub-func#4", Debug)
                    // OEM_customize
                    Return (Buffer() {
                        // SL Table Header
                        0x11,0x04,0x2E,0x01,
                        // SL Table Entry 0
                        //   Params0: 5
                        //     [3:0] SL count in the SL list array.
                        //     [7:4] Reserved and must be 0
                        0x05,
                        //   Params1: Buffer(0x00,0x01,0x02,0x03,0x04)
                        //     The SL list array specifing each SL value
                        0x00,0x01,0x02,0x03,0x04,
                        //   Params2: 0x3
                        //     SL count in the user controllable SL list array
                        0x03,
                        //   Params3: Buffer(0x01,0x02,0x03)
                        //     the user controllable SL list array
                        //     Params3[0] = SL value for Quieter Mode
                        //     Params3[1] = SL value for Quiet Mode
                        //     Params3[2] = SL value for Balanced Mode/OEM default
                        0x01,0x02,0x03,
                        //   Params4: Buffer()
                        //     Each entry in the matrix corresponds to the SL value
                        //     at a combination of the GPU Fan Group index (column)
                        //     and CPU Fan Group index (row)
                        //     Fan Group Index | GFG0 | GFG1 | GFG2 | CFG3
                        //           CFG0      | SL0  | SL2  | SL3  | SL4
                        //           CFG1      | SL0  | SL2  | SL3  | SL4
                        //           CFG2      | SL0  | SL2  | SL3  | SL4
                        //           CFG3      | SL0  | SL2  | SL3  | SL4
                        //           CFG4      | SL0  | SL2  | SL3  | SL4
                        //           CFG5      | SL1  | SL2  | SL3  | SL4
                        //           CFG6      | SL2  | SL2  | SL3  | SL4
                        //           CFG7      | SL3  | SL3  | SL3  | SL4
                        //           CFG8      | SL4  | SL4  | SL4  | SL4
                        // *CFG8 SL row is derived from Alternate Temp T-Band Table that controls CPU Fan Group.
                        // Note: There should be at least 1 SL value higher than balanced mode. And the right
                        // most column and bottom most row in the SL map table should be set to SL value higher
                        // than balanced mode. Otherwise, it would imply that balanced mode has no temperature
                        // limit, which can risk resulting in very high fan noise and/or thermal throttling.
                        0x00,0x02,0x03,0x04,
                        0x00,0x02,0x03,0x04,
                        0x00,0x02,0x03,0x04,
                        0x00,0x02,0x03,0x04,
                        0x00,0x02,0x03,0x04,
                        0x01,0x02,0x03,0x04,
                        0x02,0x02,0x03,0x04,
                        0x03,0x03,0x03,0x04,
                        0x04,0x04,0x04,0x04
                    })
                }

                //
                // Sub-Function 5: NVPCF_FUNC_GET_WM2_DYNAMIC_PARAMS (optional)
                //
                Case (NVPCF_FUNC_GET_WM2_DYNAMIC_PARAMS)
                {
                    Store("   NVPCF sub-func#5", Debug)
                    Name(PBD5, Buffer(0x28) {0x00})
                    CreateByteField(PBD5, Zero, PTV5)   // Table Version
                    CreateByteField(PBD5, 0x01, PHB5)   // Table Header in bytes
                    CreateByteField(PBD5, 0x02, TEB5)   // Table Entry in bytes 
                    CreateByteField(PBD5, 0x03, NTE5)   // Number of Table Entries
                    // Dynamic Params Table Header
                    //   Table Version (0x11)
                    //   Table Header in bytes (0x4)
                    //   Table Entry in bytes (0x24)
                    //   Number of Table Entries (0x1)
                    Store(0x11,PTV5)
                    Store(0x04,PHB5)
                    Store(0x24,TEB5)
                    Store(0x01,NTE5)
                    CreateDWordField(PBD5, 0x04, F5O0) // Params0 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x08, F5O1) // Params1 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x0C, F5O2) // Params2 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x10, F5O3) // Params3 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x14, F5O4) // Params4 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x18, F5O5) // Params5 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x1C, F5O6) // Params6 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x20, F5O7) // Params7 of Dynamic Params Table Entry, Output 
                    CreateDwordField(PBD5, 0x24, F5O8) // Params8 of Dynamic Params Table Entry, Output 

                    CreateField(Arg3, 0x20, 3, INC5)   // Params0 [2:0] of Dynamic Params Table Entry, Input
                    CreateDwordField(Arg3, 0x08, F5P1) // Params1 [31:0] of Dynamic Params Table Entry, Input
                    CreateDwordField(Arg3, 0x0C, F5P2) // Params2 [31:0] of Dynamic Params Table Entry, Input

                    // Params0 of Dynamic Params Table Entry, Input
                    //   [2:0] Input Commands
                    //
                    // Switch by Input Commands
                    //
                    Switch (ToInteger(INC5))
                    {
                        Case(0)
                        {
                            //
                            // 0 = System Notify
                            //
                            // OEM_customize need to do update following Bits
                            // Params0 of Dynamic Params Table Entry, Output
                            //   [0:0] System Notify WM2 Feature Allow Status
                            //     0 = WM2 feature is forcefully disabled. GFE and OEM UI cannot enable WM2
                            //     1 = WM2 feature is allowed. User can enable/disable WM2 and change the WM2 mode with GFE and OEM UI
                            //     Note: This WM2 system notification should be “1” for default shipping
                            //     configuration to allow WM2. Partner must enable/disable WM2 using NVAPI
                            //     function NvAPI_SYS_ClientJpacSetControl().NVIDIA driver keeps the WM2 in
                            //     disabled state for default shipping configuration. Partner and end user
                            //     can change the WM2 mode, enable/disable using GFE or/and OEM UI only
                            //     when this WM2 system notification is set to “1”. Partner can forcefully
                            //     disable the feature (by setting “0”) in case of custom or Turbo mode where
                            //     the system wants to control the fan speed and acoustic directly and cannot allow WM2.
                            //   [1:1] System Notify WM2 Params Supported Status Update
                            //     0 = No Update, 1 = Reevaluate and Update
                            //   [2:2] System Notify WM2 Tband Tables Update
                            //     0 = No Update, 1 = Reevaluate and Update
                            //   [3:3] System Notify WM2 SL Map Tables Update
                            //     0 = No Update, 1 = Reevaluate and Update
                            //   [31:4] Reserved. Must be 0
                            Store(0x0001,F5O0)

                            // Params1,2,3 of Dynamic Params Table Entry, Output
                            // All Reserved. Must be 0
                            Store(0x00000000,F5O1)
                            Store(0x00000000,F5O2)
                            Store(0x00000000,F5O3)
                        }
                        Case(1)
                        {
                            //
                            // 1 = Get WM2 Params Supported Status
                            //
                            // OEM_customize need to do update following Bits
                            // for Get WM2 Params Status support
                            // Params0 of Dynamic Params Table Entry, Output
                            //   [0:1] Reserved. Must be 0
                            //   [2:2] CPU Tj Supported Status
                            //     0 = Disabled, 1 = Enabled
                            //   [3:3] GPU Tj Supported Status
                            //     0 = Disabled, 1 = Enabled
                            //   [4:4] Total System Power Supported Status
                            //     0 = Disabled, 1 = Enabled
                            //   [31:5] Reserved. Must be 0
                            Store(0x0000,F5O0)

                            // Params1,2,3 of Dynamic Params Table Entry, Output
                            // All Reserved. Must be 0
                            Store(0x00000000,F5O1)
                            Store(0x00000000,F5O2)
                            Store(0x00000000,F5O3)
                        }
                        Case(2)
                        {
                            //
                            // 2 = Get WM2 Params Status
                            //
                            // OEM_customize need to do update following
                            // The temperature may from EC.
                            // Params0 of Dynamic Params Table Entry, Output
                            //   [31:0] Total System Power (Watts)
                            Store(0x00000000,F5O0)
                            
                            // Params1 of Dynamic Params Table Entry, Output
                            // And for the TBand table index for Entry 0
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O1)
                            
                            // Params2 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 1
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O2)
                            
                            // Params3 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 2
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O3)
                            
                            // Params4 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 3
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O4)
                            
                            // Params5 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 4
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O5)
                            
                            // Params6 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 5
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O6)
                            
                            // Params7 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 6
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O7)
                            
                            // Params8 of Dynamic Params Table Entry, Output
                            //   And for the TBand table index for Entry 7
                            //   [7:0] Current Tband index
                            //   [31:8] Tband temp in 1/256th degree Celsius
                            Store(0x00000000,F5O8)
                        }
                        Case(3)
                        {
                            //
                            // 3 = Set User Controllable SL target
                            //
                            // Params1 of Dynamic Params Table Entry, Input
                            //   [7:0] Set SL value from the user controllable SL list array
                            //     0xFF = User SL Target Disable
                            Store(And(F5P1, 0xFF),CUSL)    // Save SL value
                        }
                        Case(4)
                        {
                            //
                            // 3 = Set CPU Tj Limit Target
                            //
                            // Params2 of Dynamic Params Table Entry, Input
                            //   [31:0] CPU Tj Temperature in 1/256th degree Celsius
                            //     0x00000000 = CPU Tj Limit Target Disable
                            Store(F5P2,CUCT)    // Save CPU Tj limit value
                        }
                        Default
                        {
                            Return (STATUS_ERROR_UNSUPPORTED)
                        }
                    }
                    Return(PBD5)
                }

                //
                // Sub-Function 6: NVPCF_FUNC_CPU_CONTROL
                //
                Case (NVPCF_FUNC_CPU_CONTROL)
                {
                    Store("   NVPCF sub-func#6", Debug)
                    Name(PBD6, Buffer(0x11) {0x00})
                    CreateByteField(PBD6, Zero, CCHV)   // Table Version
                    CreateByteField(PBD6, 0x01, CCHB)   // Size of Table Header in bytes
                    CreateByteField(PBD6, 0x02, CCTB)   // Size of Table in bytes 
                    CreateByteField(PBD6, 0x03, RES0)   // Must be zero
                    CreateByteField(PBD6, 0x04, RES1)   // Must be zero
                    // CPU Control Header
                    //   Table Version (0x10)
                    //   Size of Table Header in bytes (0x5)
                    //   Size of Table in bytes (0x0C)
                    Store(0x10,CCHV)
                    Store(0x05,CCHB)
                    Store(0x0C,CCTB)
                    CreateField(PBD6, 0x28, 2, F6O0)// Params0 of CPU Control Entry, Output 
                    CreateByteField(PBD6, 0x09, F6MP)   // Params1 [7:0] of CPU Control Entry, Output 
                    CreateByteField(PBD6, 0x0A, F6NP)   // Params1 [15:8] of CPU Control Entry, Output 
                    CreateDwordField(PBD6, 0x0D, F6O2)  // Params2 of CPU Control Entry, Output 

                    CreateField(Arg3, 0x28, 2, INC6)// Params0 [1:0] of CPU Control Entry, Input
                    CreateByteField(Arg3, 0x09, NCHP)   // Params1 [7:0] of CPU Control Entry, Input

                    // Params0 of CPU Control Entry, Input
                    //   [1:0] Input Commands
                    //
                    // Switch by Input Commands
                    //
                    Switch (ToInteger(INC6))
                    {
                        Case(0)
                        {
                            //
                            // 0 = Get CPPC parameters
                            //
                            If(LNotEqual(IOBS,0x0))
                            {
                                // Params0 of CPU Control Entry, Output
                                //   [1:0] Control Type
                                //     0x0 = NV Default
                                //     0x1 = Reserved
                                //     0x2 = Embedded Controller IO Space 
                                Store(HPCT,F6O0)
                                // Params1 of CPU Control Entry, Output
                                //   [7:0] CPPC Maximum Performance Level
                                //     0x0 = Disabled
                                //   [15:8] CPPC Nominal Performance Level
                                Store(CMPL,F6MP)
                                Store(CNPL,F6NP)
                                // Params2 of CPU Control Entry, Output
                                //   [31:0] Register Address in IO Space
                                Store(IOBS,F6O2)
                            }
                        }
                        Case(1)
                        {
                            //
                            // 1 = Set CPPC parameters
                            //
                            If(LNotEqual(IOBS,0x0))
                            {
                                OperationRegion(NVIO, SystemIO, IOBS, 0x10)    // IBV_customize/OEM_customize // Example port: 0x25F
                                Field(NVIO, ByteAcc, NoLock, Preserve)
                                {
                                    CPUC,  8
                                }

                                // Params1 of CPU Control Entry, Input
                                //   [7:0] New CPPC Highest Performance value
                                Store(NCHP, CPUC) // Store HighestPerformance value to Embedded IO Space

                                // Params1,2 of CPU Control Entry, Output
                                // All Reserved. Must be 0
                                Store(0x00,F6MP)
                                Store(0x00,F6NP)
                                Store(0x00000000,F6O2)

                                /* IBV_customize/OEM_customize should add code to Notify All CPU core */
                                /* Example:
                                 * Notify(\_SB.CPU0, 0x85)
                                 * Notify(\_SB.CPU1, 0x85)
                                 * Notify(\_SB.CPU2, 0x85)
                                 * Notify(\_SB.CPU3, 0x85)
                                 * ...
                                 * Notify(\_SB.CPUX, 0x85)
                                 */
                                 Notify(CPU0_SCOPE, 0x85)   //Notify(\_SB.PLTF.C000, 0x85)
                                 Notify(CPU1_SCOPE, 0x85)   //Notify(\_SB.PLTF.C001, 0x85)
                                 Notify(CPU2_SCOPE, 0x85)   //Notify(\_SB.PLTF.C002, 0x85)
                                 Notify(CPU3_SCOPE, 0x85)   //Notify(\_SB.PLTF.C003, 0x85)
                                 Notify(CPU4_SCOPE, 0x85)   //Notify(\_SB.PLTF.C004, 0x85)
                                 Notify(CPU5_SCOPE, 0x85)   //Notify(\_SB.PLTF.C005, 0x85)
                                 Notify(CPU6_SCOPE, 0x85)   //Notify(\_SB.PLTF.C006, 0x85)
                                 Notify(CPU7_SCOPE, 0x85)   //Notify(\_SB.PLTF.C007, 0x85)
                                 Notify(CPU8_SCOPE, 0x85)   //Notify(\_SB.PLTF.C008, 0x85)
                                 Notify(CPU9_SCOPE, 0x85)   //Notify(\_SB.PLTF.C009, 0x85)
                                 Notify(CPUA_SCOPE, 0x85)   //Notify(\_SB.PLTF.C00A, 0x85)
                                 Notify(CPUB_SCOPE, 0x85)   //Notify(\_SB.PLTF.C00B, 0x85)
                                 Notify(CPUC_SCOPE, 0x85)   //Notify(\_SB.PLTF.C00C, 0x85)
                                 Notify(CPUD_SCOPE, 0x85)   //Notify(\_SB.PLTF.C00D, 0x85)
                                 Notify(CPUE_SCOPE, 0x85)   //Notify(\_SB.PLTF.C00E, 0x85)
                                 Notify(CPUF_SCOPE, 0x85)   //Notify(\_SB.PLTF.C00F, 0x85)                    
                            }
                        }
                        Default
                        {
                            Return (STATUS_ERROR_UNSUPPORTED)
                        }
                    }
                    Return(PBD6)
                }

                //
                // Sub-Function 7: NVPCF_FUNC_GPU_INFO
                //
                Case (NVPCF_FUNC_GPU_INFO)
                {
                    Store("   NVPCF sub-func#7", Debug)
                    CreateDwordField(Arg3, 0x05, AMAX)// Params0 of GPU Info Table Body, Input
                    CreateDwordField(Arg3, 0x09, ARAT)// Params1 of GPU Info Table Body, Input
                    CreateDwordField(Arg3, 0x0D, DMAX)// Params2 of GPU Info Table Body, Input
                    CreateDwordField(Arg3, 0x11, DRAT)// Params3 of GPU Info Table Body, Input
                    CreateDwordField(Arg3, 0x15, TGPM)// Params4 of GPU Info Table Body, Input
                    //OEM_customize Start
                    // AMAX - AC TGP maximum in milliwatts
                    // ARAT - AC TGP rated in milliwatts
                    // DMAX - DC TGP maximum in milliwatts
                    // DRAT - DC TGP rated in milliwatts
                    // TGPM - TGP minimum in milliwatts
                    //OEM_customize End

                    Return (STATUS_SUCCESS)  // NVPCF_ERROR_SUCCESS
                }

                //
                // Sub-Function 8: NVPCF_FUNC_GET_DC_SYSTEM_POWER_LIMITS_TABLE
                //
                Case (NVPCF_FUNC_GET_DC_SYSTEM_POWER_LIMITS_TABLE)
                {
                    Store("   NVPCF sub-func#8", Debug)
                    //OEM_customize
                    Return (Buffer() {
                    // DC System Power Limits Table Header
                    // (version, header, size, body size, entry count)
                    0x10,0x04,0x11,0x5,
                    // DC System Power Limits Table Entry
                    // Params0 [7:0] Battery state of charge threshold (percentage 0-100)
                    // Params1 [31:0] Battery current Limit (milliamps)
                    // Params2 [31:0] Rest of system reserved power (milliwatts)
                    // Params3 [31:0] Min CPU TDP (milliwatts)
                    // Params4 [31:0] Max CPU TDP (milliwatts)
                    //
                    // *The first entry in this table must have a threshold of 100%
                    // *An entry is "active" when the sensed battery state of charge falls below the
                    //  given entry's threshold but remains above the next entry's threshold
                    // *The last entry in table will be the "active" entry when battery state of charge
                    //  is between it's specified threshold and 0%
                    //
                    // Entry0, 100% threshold, 6500mA battery current limit, 30000mW ROS
                    // 19000mW min CPU TDP, 25000mW max CPU TDP
                    0x64,
                    0x64, 0x19, 0x00, 0x00,
                    0x30, 0x75, 0x00, 0x00,
                    0x38, 0x4A, 0x00, 0x00,
                    0xA8, 0x61, 0x00, 0x00,
                    // Entry1, 80% threshold, 6350mA battery current limit, 29000mW ROS
                    // 17000mW min CPU TDP, 23000mW max CPU TDP
                    0x50,
                    0xCE, 0x18, 0x00, 0x00,
                    0x48, 0x71, 0x00, 0x00,
                    0x68, 0x42, 0x00, 0x00,
                    0xD8, 0x59, 0x00, 0x00,
                    // Entry2, 60% threshold, 6200mA battery current limit, 29000mW ROS
                    // 16000mW min CPU TDP, 21000mW max CPU TDP
                    0x3C,
                    0x38, 0x18, 0x00, 0x00,
                    0x48, 0x71, 0x00, 0x00,
                    0x80, 0x3E, 0x00, 0x00,
                    0x08, 0x52, 0x00, 0x00,
                    // Entry3, 40% threshold, 6050mA battery current limit, 28000mW ROS
                    // 13000mW min CPU TDP, 17000mW max CPU TDP
                    0x28,
                    0xA2, 0x17, 0x00, 0x00,
                    0x60, 0x6D, 0x00, 0x00,
                    0xC8, 0x32, 0x00, 0x00,
                    0x68, 0x42, 0x00, 0x00,
                    // Entry4, 20% threshold, 5900mA battery current limit, 27000mW ROS
                    // 11000mW min CPU TDP, 14000mW max CPU TDP
                    0x14,
                    0x0C, 0x17, 0x00, 0x00,
                    0x78, 0x69, 0x00, 0x00,
                    0xF8, 0x2A, 0x00, 0x00,
                    0xB0, 0x36, 0x00, 0x00
                    })
                    //OEM_customize End
                }

                //
                // Sub-Function 9: NVPCF_FUNC_CPU_TDP_CONTROL
                //
                Case (NVPCF_FUNC_CPU_TDP_CONTROL)
                {
                    Store("   NVPCF sub-func#9", Debug)
                    CreateDwordField(Arg3, 0x03, CPTD)// Params0 of CPU TDP Control table Body, Input

                    //OEM_customize Start
                    // CPTD - CPU TDP limit to be set (milliwatts) by Driver
                    // Set CPU PL1 to CTDP in case incoming CPU TDP limit is smaller than current PL1 limit
                    // ** A value of ZERO (0x00) will be used to denote the release of CPU TDP control.
                    //    This will occur in the following use cases:
                    //    1. Following an DC to AC power transition
                    //    2. When DB on DC is disabled through Sub-func#2                    
                    //OEM_customize End

                    Return (STATUS_SUCCESS)  // NVPCF_ERROR_SUCCESS
                }
            }
            Return (STATUS_ERROR_UNSUPPORTED)
        }
    }
}