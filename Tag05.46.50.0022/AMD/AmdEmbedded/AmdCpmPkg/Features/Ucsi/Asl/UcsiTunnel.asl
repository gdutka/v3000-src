/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "Ucsi.aml",                                         // Output file
    "SSDT",                                             // Signature
    0x01,                                               // SSDT Revision
    "AMD",                                              // OEM ID
    "CPMUCSI",                                          // OEM Table ID
    0x1                                                 // OEM Revision
    )
{
    #define CPM_UCSI

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(CpmUcsiOpm2Ppm, MethodObj)
    External(CpmUcsiPpm2Opm, MethodObj)
    External(CpmUcsiLoadVersion, MethodObj)

    Scope (\_SB) {
        Device (UBTC) {
            Name (_HID, EISAID ("USBC000"))
            Name (_CID, EISAID ("PNP0CA0"))
            Name (_UID, Zero)
            Name (_DDN, "USB Type C")
            Name (_ADR, Zero)
            Name (CpmUcsiPldBuffer, Buffer(0x14) {
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            })
            Name (CRS, ResourceTemplate () {
                Memory32Fixed (ReadWrite, 0x00000000, 0x00001000)
            })
            Device (CR01) {
                Name (_ADR, 0)
                Method (_PLD, 0, NotSerialized) {
                    CreateDwordField (CpmUcsiPldBuffer,  0, CpmUcsiPldBufferDW0)
                    CreateDwordField (CpmUcsiPldBuffer,  4, CpmUcsiPldBufferDW1)
                    CreateDwordField (CpmUcsiPldBuffer,  8, CpmUcsiPldBufferDW2)
                    CreateDwordField (CpmUcsiPldBuffer, 12, CpmUcsiPldBufferDW3)
                    CreateDwordField (CpmUcsiPldBuffer, 16, CpmUcsiPldBufferDW4)
                    Store (CpmUcsiTunnelTable, Local0)
                    If (Local0) {
                        Add (Local0, CpmUcsiTunnelPldInfo0, Local0)
                        Store (CpmReadMem32 (Add (Local0,  0), 0, 0, 32), CpmUcsiPldBufferDW0)
                        Store (CpmReadMem32 (Add (Local0,  4), 0, 0, 32), CpmUcsiPldBufferDW1)
                        Store (CpmReadMem32 (Add (Local0,  8), 0, 0, 32), CpmUcsiPldBufferDW2)
                        Store (CpmReadMem32 (Add (Local0, 12), 0, 0, 32), CpmUcsiPldBufferDW3)
                        Store (CpmReadMem32 (Add (Local0, 16), 0, 0, 32), CpmUcsiPldBufferDW4)
                    }
                    Return (CpmUcsiPldBuffer)
                }
            }
            Device (CR02) {
                Name (_ADR, 1)
                Method (_PLD, 0, NotSerialized) {
                    // _PLD method, do not expose it if its parent device is not present
                    CreateDwordField (CpmUcsiPldBuffer,  0, CpmUcsiPldBufferDW0)
                    CreateDwordField (CpmUcsiPldBuffer,  4, CpmUcsiPldBufferDW1)
                    CreateDwordField (CpmUcsiPldBuffer,  8, CpmUcsiPldBufferDW2)
                    CreateDwordField (CpmUcsiPldBuffer, 12, CpmUcsiPldBufferDW3)
                    CreateDwordField (CpmUcsiPldBuffer, 16, CpmUcsiPldBufferDW4)
                    Store (CpmUcsiTunnelTable, Local0)
                    If (Local0) {
                        Add (Local0, CpmUcsiTunnelPldInfo1, Local0)
                        Store (CpmReadMem32 (Add (Local0,  0), 0, 0, 32), CpmUcsiPldBufferDW0)
                        Store (CpmReadMem32 (Add (Local0,  4), 0, 0, 32), CpmUcsiPldBufferDW1)
                        Store (CpmReadMem32 (Add (Local0,  8), 0, 0, 32), CpmUcsiPldBufferDW2)
                        Store (CpmReadMem32 (Add (Local0, 12), 0, 0, 32), CpmUcsiPldBufferDW3)
                        Store (CpmReadMem32 (Add (Local0, 16), 0, 0, 32), CpmUcsiPldBufferDW4)
                    }
                    Return (CpmUcsiPldBuffer)
                }
            }
            Device (CR03) {
                Name (_ADR, 2)
                Method (_PLD, 0, NotSerialized) {
                    // _PLD method, do not expose it if its parent device is not present
                    CreateDwordField (CpmUcsiPldBuffer,  0, CpmUcsiPldBufferDW0)
                    CreateDwordField (CpmUcsiPldBuffer,  4, CpmUcsiPldBufferDW1)
                    CreateDwordField (CpmUcsiPldBuffer,  8, CpmUcsiPldBufferDW2)
                    CreateDwordField (CpmUcsiPldBuffer, 12, CpmUcsiPldBufferDW3)
                    CreateDwordField (CpmUcsiPldBuffer, 16, CpmUcsiPldBufferDW4)
                    Store (CpmUcsiTunnelTable, Local0)
                    If (Local0) {
                        Add (Local0, CpmUcsiTunnelPldInfo2, Local0)
                        Store (CpmReadMem32 (Add (Local0,  0), 0, 0, 32), CpmUcsiPldBufferDW0)
                        Store (CpmReadMem32 (Add (Local0,  4), 0, 0, 32), CpmUcsiPldBufferDW1)
                        Store (CpmReadMem32 (Add (Local0,  8), 0, 0, 32), CpmUcsiPldBufferDW2)
                        Store (CpmReadMem32 (Add (Local0, 12), 0, 0, 32), CpmUcsiPldBufferDW3)
                        Store (CpmReadMem32 (Add (Local0, 16), 0, 0, 32), CpmUcsiPldBufferDW4)
                    }
                    Return (CpmUcsiPldBuffer)
                }
            }
            Method (_CRS, 0, Serialized) {
                CreateDWordField (CRS, 0x04, CpmUcsiOperationRegionBufferBegin)
                Store (CpmUcsiTunnelTable, Local0)
                Add (Local0, CpmUcsiTunnelOperationRegionBegin, CpmUcsiOperationRegionBufferBegin)
                Return (CRS)
            }
            Method (_STA, 0, NotSerialized) {
                If (LEqual (CpmReadTable(CpmMainTable, CpmUcsiEn), 1)) {
                    CpmDebugPrintString("  \\_SB.UBTC._STA = 0xF\n", 0)
                    Return (0xF)
                } Else {
                    CpmDebugPrintString("  \\_SB.UBTC._STA = 0 unsupported\n", 0)
                    Return (0x0)
                }
            }

            OperationRegion(PM08, SystemMemory, CpmUcsiOPMData, 0x30)
            Field(PM08, ByteAcc, Lock, Preserve) {
                VER0, 8,
                VER1, 8,
                RSV0, 8,
                RSV1, 8,
                CCI0, 8,
                CCI1, 8,
                CCI2, 8,
                CCI3, 8,
                CTL0, 8,
                CTL1, 8,
                CTL2, 8,
                CTL3, 8,
                CTL4, 8,
                CTL5, 8,
                CTL6, 8,
                CTL7, 8,
                MGI0, 8,
                MGI1, 8,
                MGI2, 8,
                MGI3, 8,
                MGI4, 8,
                MGI5, 8,
                MGI6, 8,
                MGI7, 8,
                MGI8, 8,
                MGI9, 8,
                MGIA, 8,
                MGIB, 8,
                MGIC, 8,
                MGID, 8,
                MGIE, 8,
                MGIF, 8,
                MGO0, 8,
                MGO1, 8,
                MGO2, 8,
                MGO3, 8,
                MGO4, 8,
                MGO5, 8,
                MGO6, 8,
                MGO7, 8,
                MGO8, 8,
                MGO9, 8,
                MGOA, 8,
                MGOB, 8,
                MGOC, 8,
                MGOD, 8,
                MGOE, 8,
                MGOF, 8,
            }

            //
            // _DSM Device Specific Method
            //
            // Arg0:   UUID      Unique function identifier
            // Arg1:   Integer   Revision Level
            // Arg2:   Integer   Function Index (0 = Return Supported Functions)
            // Arg3:   Package   Parameters
            //
            Method (_DSM, 4, Serialized) {
                If (LEqual (Arg0, ToUUID ("6F8398C2-7CA4-11E4-AD36-631042B5008F"))) {
                    //
                    // Function 0:
                    //
                    If (LEqual (ToInteger (Arg2), 0)) {

                        If (CondRefOf(CpmUcsiLoadVersion)) {
                            CpmUcsiLoadVersion()
                        }
                        CpmDebugPrint ("  \\_SB.UBTC.VER = %X.%X\n", VER1, VER0, 0, 0, 0, 0)

                        Return (Buffer (1) {0x0F})

                    //
                    // Function 1:
                    //
                    } ElseIf (LEqual (ToInteger (Arg2), 1)) {
                        CpmSetPostCode(CpmTpUcsiDsmF1Start)
                        If (CondRefOf (CpmUcsiOpm2Ppm)) {
                            CpmUcsiOpm2Ppm ()
                        }
                        CpmSetPostCode(CpmTpUcsiDsmF1End)

                    //
                    // Function 2:
                    //
                    } ElseIf (LEqual (ToInteger (Arg2), 2)) {
                        CpmSetPostCode(CpmTpUcsiDsmF2Start)
                        If (CondRefOf (CpmUcsiPpm2Opm)) {
                            CpmUcsiPpm2Opm ()
                        }
                        CpmSetPostCode(CpmTpUcsiDsmF2End)
                    }
                }
                Return(0)
            } // End of Method (_DSM, 4, Serialized)
        } // End of Device (UBTC)
    } // End of Scope (\_SB)
} // End of DefinitionBlock()

