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
    For NVPCF 2.0 support with ITBM, SBIOS has to adjust below code to meet platform:
    * OEM/IBV should check "_customize" keyword for TO-DO *
    - Please refer to NVIDIA SW design guide (DG-09954-001) to generate the NVPCF buffer in sub-func#1
      Remember to update the checksum byte (Sum of Bytes and 2s Complement) if NVPCF buffer changed
    - NVPCF is a virture device under \_SB, do not change the device path
    - OEM has to create a EC IO address by "IOBS" for the DYNAMIC BOOST 2.0 access
    - OEM has to update the CPC2 method. Please consult NVIDIA SW AE for for more detail
    - Whisper mode 2.0 (WM2)
      a. Require DYNAMIC BOOST 2.0
      b. OEM should to update some information to support
*/

#define ITBMT_IOTRAP    0x820

// OEM_customize Start
/* OEM need add "\_SB.PCI0.PEG0.PEGP.CAFL()" into ITBM SCI event handler */
//Scope (\_GPE)
//{
//    //
//    // Software GPE caused the ITBM event.
//    //
//    Method(_L62)
//    {
//        ///
//        /// Handle Intel Turbo Boost Max Technology 3.0 SCI event
//        ///
//        If (LEqual(\_SB.ITBI, One)) {
//            If (CondRefOf(\_GPE.ITBH)) {
//                \_SB.PCI0.PEG0.PEGP.CAFL()     /* IBV_customize: NVIDIA added */
//                \_GPE.ITBH()
//            }
//            ///
//            /// Clear interrupt status
//            ///
//        }
//    }
//}
// OEM_customize End

Scope (\_SB)                            // DO NOT MODIFY THE PATH
{
    Device (NPCF)
    {
        // OEM_customize Start
        Name(ACBT, 0x0050)              // AC:Baseline TGP (1/8-watt units)
                                        // +10W => 80

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
            Store("------- NVPCF DSM --------", Debug)
            // Revision 0x200 is supported
            If (LNotEqual (ToInteger(Arg1), 0x200))
            {
                Return (STATUS_ERROR_UNSPECIFIED)
            }
            Switch (ToInteger(Arg2))
            {
                //
                // Sub-Function 0: NVPCF_FUNC_GET_SUPPORTED
                //
                Case (NVPCF_FUNC_GET_SUPPORTED)
                {
                    Store ("   NVPCF sub-func#0", Debug)
                    // sub-func: 0, 1, 2, 3, 4, 5, 6 and 7 supported
                    Return (Buffer() {0xFF, 0x00, 0x00, 0x00})
                }

                //
                // Sub-Function 1: NVPCF_FUNC_GET_STATIC_CONFIG_TABLES
                //
                Case (NVPCF_FUNC_GET_STATIC_CONFIG_TABLES)
                {
                    Store("   NVPCF sub-func#1", Debug)
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
                        // System Controller Table Header (2.1)
                        //   One Controller Entries
                        0x21,0x05,0x02,0x0F,0x01,
                        // System Controller Table Common Settings
                        //   Period 100 milliseconds
                        0x64,0x00,
                        // System Controller Table Entry 0
                        //   Type = Controller Class
                        //     0x0 = Entry Disabled
                        //     0x1 = Dynamic Boost Controller
                        //     0x2 = Configurable TGP-only Controller
                        // OEM_customize need modify to 2 if support CTGP Only
                        0x01,
                        //   Sampling Multiplier: 0x0001
                        0x01,0x00,
                        //   Filter parameters: 0x0000E800
                        //     [7:0] Filter Function
                        //       0x0 = Exponentially Weighed Moving Average (EWMA)
                        //       0x1 = Moving Max
                        //     [15:8] Filter param = 0xE8 (10 sampling period time constant (tau))
                        //       EWMA: Smoothing factor N
                        //             tau = -1 * sampling period / ln(N / 256)
                        //       Moving Max: Filter size in samples
                        0x00,0xE8,0x00,0x00,
                        //   Param0: 0x10000E66
                        //     Class = Dynamic Boost Controller
                        //       [15:0] Increase Gain in UFXP4_12 = 0x0e66 (0.9)
                        //       [31:16] Decrease Gain in UFXP4_12 = 0x1000 (1.0)
                        //     Class = CTGP-only Controller, Reserved. Must be 0.
                        //   Param1: 0x00000000
                        //     Class = Dynamic Boost Controller
                        //       [0:0] DC Support
                        //         0x0 = Not Supported
                        //         0x1 = supported
                        //     Class = CTGP-only Controller, Reserved. Must be 0.
                        // OEM_customize need to modify the Dynamic Boost support DC mode
                        0x66,0x0E,0x00,0x10,
                        0x01,0x00,0x00,0x00,
                        // Single byte checksum value, Sum of Bytes and 2s Complement
                        0xD1
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
                    //   Table Version (0x21). (Value should match the System Controller Table Header)
                    //   Table Header in bytes (0x5)
                    //   Size of Common Status in bytes (0x10)
                    //   Size of Controller Entry in bytes (0x1C)
                    //   Number of controller Entries (0x1)
                    Store(0x21,PTV2)
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

                    CreateField(Arg3, 0x28, 2, NIGS)    // Params0 [1:0] of Common Status, Input
                    CreateByteField(Arg3, 0x15, IORC)   // Params0 [7:0] of controller Entry, Input
                    CreateField(Arg3, 0xB0, 1, PWCS)    // Params0 [8:8] of controller Entry, Input
                    CreateField(Arg3, 0xB1, 1, PWTS)    // Params0 [9:9] of controller Entry, Input
                    CreateField(Arg3, 0xB2, 1, CGPS)    // Params0 [10:10] of controller Entry, Input

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
                        If (LEqual(ToInteger(PWCS), 0x1))
                        {
                            //Power control capability Enabled
                        } Else {
                            //Power control capability Disabled
                        }

                        If (LEqual(ToInteger(PWTS), 0x1))
                        {
                            //Power transfer Enabled
                        } Else {
                            //Power transfer Disabled
                        }

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
                        //       3   |    x    |   x   |  x
                        //       4   |    x    |   x   |  x
                        //       5   |    x    |   x   |  x
                        //       6   |    x    |   x   |  x
                        //       7   |    x    |   x   |  x
                        //       8   |    x    |   x   |  x
                        0x05,
                        0xFF,0x00,0x3C,0x41,0x41,0x46,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
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
                        0x11,0x04,0x25,0x01,
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
                        //     Fan Group Index | GFG0 | GFG1 | GFG2
                        //           CFG0      | SL0  | SL2  | SL3
                        //           CFG1      | SL0  | SL2  | SL3
                        //           CFG2      | SL0  | SL2  | SL3
                        //           CFG3      | SL0  | SL2  | SL3
                        //           CFG4      | SL0  | SL2  | SL3
                        //           CFG5      | SL1  | SL2  | SL3
                        //           CFG6      | SL2  | SL2  | SL3
                        //           CFG7      | SL3  | SL3  | SL3
                        //           CFG8      | SL4  | SL4  | SL4
                        0x00,0x02,0x03,
                        0x00,0x02,0x03,
                        0x00,0x02,0x03,
                        0x00,0x02,0x03,
                        0x00,0x02,0x03,
                        0x01,0x02,0x03,
                        0x02,0x02,0x03,
                        0x03,0x03,0x03,
                        0x04,0x04,0x04
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
                            //   [0:0] System Notify WM2 Status Update
                            //     0 = WM2 Disabled, 1 = WM2 Enabled
                            //   [1:1] System Notify WM2 Params Supported Status Update
                            //     0 = No Update, 1 = Reevaluate and Update
                            //   [2:2] System Notify WM2 Tband Tables Update
                            //     0 = No Update, 1 = Reevaluate and Update
                            //   [3:3] System Notify WM2 SL Map Tables Update
                            //     0 = No Update, 1 = Reevaluate and Update
                            //   [31:4] Reserved. Must be 0
                            Store(0x0000,F5O0)

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

                                If(LNotEqual(ToInteger(NCHP), 0xFF))
                                {
                                    Store(Zero, DGPU_SCOPE.AFST) // Set Activated AutoFL_State
                                    // Params1 of CPU Control Entry, Input
                                    //   [7:0] New CPPC Highest Performance value
                                    Store(NCHP, CPUC) // Store HighestPerformance value to Embedded IO Space
                                } Else {
                                    Store(0xFF, DGPU_SCOPE.AFST) // Set InActivate AutoFL_State
                                    /* IBV_customize/OEM_customize need store the MSR 0x1AD value to EC IO */
                                    Store(CMPL, CPUC)
                                }


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
                }
            }
            Return (STATUS_ERROR_UNSUPPORTED)
        }
    }
}

/* Add the code for ITBM supported */
Scope (DGPU_SCOPE)
{
    Name (AFST, 0xFF)    // Set default InActivate for AutoFL_State

    /*
     * Check AutoFL state
     * Read ITBM CPU ratio value & store to EC IO if AutoFL_State InActivate
     */
    Method (CAFL, 0, Serialized)
    {
        /* AutoFL InActivate, Store the CPU Ratio to EC-IO */
        If (LEqual (AFST, 0xFF))
        {
            /* IBV_customize/OEM_customize need define ITBMT_IOTRAP */
            OperationRegion (SMIP, SystemIO, ITBMT_IOTRAP, 1)
            Field (SMIP, ByteAcc, NoLock, Preserve)
            {
                IOB2, 8
            }
            /* EC IO */
            OperationRegion (ECIO, SystemIO, \_SB.NPCF.IOBS, 1)
            Field (ECIO, ByteAcc, NoLock, Preserve)
            {
                ECTP, 8
            }
            /* Read CPU Ratio */
            Store (IOB2, Local0)
            /* Store CPU Ratio to EC IO */
            Store (Local0, ECTP)
        }
    }
}
