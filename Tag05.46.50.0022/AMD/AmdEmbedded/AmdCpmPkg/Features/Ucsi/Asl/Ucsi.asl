/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
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

    External(\_SB.PCI0.LPC0.EC0, DeviceObj)
    External(\_SB.PCI0.LPC0.EC0.OKEC, IntObj)
    External(\_SB.PCI0.LPC0.EC0.CpmKbcMutex, MutexObj)

    Scope (\_SB.PCI0.LPC0.EC0) {
        OperationRegion(PM09, EmbeddedControl, CpmUcsiEcVersionLocation, 0x2)
        Field(PM09, ByteAcc, Lock, Preserve) {
            VER0, 8,
            VER1, 8,
        }
        OperationRegion(PM00, EmbeddedControl, CpmUcsiEcCciLocation, 0x4)
        Field(PM00, ByteAcc, Lock, Preserve) {
            CCI0, 8,
            CCI1, 8,
            CCI2, 8,
            CCI3, 8,
        }
        OperationRegion(PM01, EmbeddedControl, CpmUcsiEcCtlLocation, 0x8)
        Field(PM01, ByteAcc, Lock, Preserve) {
            CTL0, 8,
            CTL1, 8,
            CTL2, 8,
            CTL3, 8,
            CTL4, 8,
            CTL5, 8,
            CTL6, 8,
            CTL7, 8,
        }
        OperationRegion(PM02, EmbeddedControl, CpmUcsiEcMgiLocation, 0x10)
        Field(PM02, ByteAcc, Lock, Preserve) {
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
        }
        OperationRegion(PM03, EmbeddedControl, CpmUcsiEcMgoLocation, 0x10)
        Field(PM03, ByteAcc, Lock, Preserve) {
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
        Method (XQ50, 0, NotSerialized) {
            If (LNotEqual(\_SB.CCI3, 0x8)) {
                CpmSetPostCode(CpmTpUcsiXQ50Start)
                \_SB.UBTC.CpmUcsiWriteFromEc ()
                CpmSetPostCode(CpmTpUcsiXQ50End)
                Notify (\_SB.UBTC, 0x80)
            }
        }
    } // End of Scope (\_SB.PCI0.LPC0.EC0)

    Scope (\_SB) {
        OperationRegion(PM0A, SystemMemory, CpmUcsiEcVersionLocation, 0x2)
        Field(PM0A, ByteAcc, Lock, Preserve) {
            VER0, 8,
            VER1, 8,
        }
        OperationRegion(PM04, SystemMemory, CpmUcsiEcCciLocation, 0x4)
        Field(PM04, ByteAcc, Lock, Preserve) {
            CCI0, 8,
            CCI1, 8,
            CCI2, 8,
            CCI3, 8,
        }
        OperationRegion(PM05, SystemMemory, CpmUcsiEcCtlLocation, 0x8)
        Field(PM05, ByteAcc, Lock, Preserve) {
            CTL0, 8,
            CTL1, 8,
            CTL2, 8,
            CTL3, 8,
            CTL4, 8,
            CTL5, 8,
            CTL6, 8,
            CTL7, 8,
        }
        OperationRegion(PM06, SystemMemory, CpmUcsiEcMgiLocation, 0x10)
        Field(PM06, ByteAcc, Lock, Preserve) {
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
        }
        OperationRegion(PM07, SystemMemory, CpmUcsiEcMgoLocation, 0x10)
        Field(PM07, ByteAcc, Lock, Preserve) {
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
        Device (UBTC) {
            Name (_HID, EISAID ("USBC000"))
            Name (_CID, EISAID ("PNP0CA0"))
            Name (_UID, Zero)
            Name (_DDN, "USB Type C")
            Name (_ADR, Zero)
            Name (_DEP, Package() {\_SB.PCI0.LPC0.EC0})
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
                    Store (CpmUcsiTable, Local0)
                    If (Local0) {
                        Add (Local0, CpmUcsiPldInfo0, Local0)
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
                    Store (CpmUcsiTable, Local0)
                    If (Local0) {
                        Add (Local0, CpmUcsiPldInfo1, Local0)
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
                Store (CpmUcsiTable, Local0)
                Add (Local0, CpmUcsiOperationRegionBegin, CpmUcsiOperationRegionBufferBegin)
                Return (CRS)
            }
            Method (_STA, 0, NotSerialized) {
                If (LEqual (CpmReadTable(CpmMainTable, CpmUcsiEn), 1)) {
                    Return (0xF)
                } Else {
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
            Method (CpmUcsiWriteFromEc, 0, Serialized) {
                If (LEqual (CpmUcsiEcUcsiLocation, 0)) {
                    If (LEqual(\_SB.PCI0.LPC0.EC0.OKEC, 0x01)) {
                        Acquire(\_SB.PCI0.LPC0.EC0.CpmKbcMutex, 0xFFFF)

                        Store (\_SB.PCI0.LPC0.EC0.MGI0, \_SB.UBTC.MGI0) // Offset 16
                        Store (\_SB.PCI0.LPC0.EC0.MGI1, \_SB.UBTC.MGI1) // Offset 17
                        Store (\_SB.PCI0.LPC0.EC0.MGI2, \_SB.UBTC.MGI2) // Offset 18
                        Store (\_SB.PCI0.LPC0.EC0.MGI3, \_SB.UBTC.MGI3) // Offset 19
                        Store (\_SB.PCI0.LPC0.EC0.MGI4, \_SB.UBTC.MGI4) // Offset 20
                        Store (\_SB.PCI0.LPC0.EC0.MGI5, \_SB.UBTC.MGI5) // Offset 21
                        Store (\_SB.PCI0.LPC0.EC0.MGI6, \_SB.UBTC.MGI6) // Offset 22
                        Store (\_SB.PCI0.LPC0.EC0.MGI7, \_SB.UBTC.MGI7) // Offset 23
                        Store (\_SB.PCI0.LPC0.EC0.MGI8, \_SB.UBTC.MGI8) // Offset 24
                        Store (\_SB.PCI0.LPC0.EC0.MGI9, \_SB.UBTC.MGI9) // Offset 25
                        Store (\_SB.PCI0.LPC0.EC0.MGIA, \_SB.UBTC.MGIA) // Offset 26
                        Store (\_SB.PCI0.LPC0.EC0.MGIB, \_SB.UBTC.MGIB) // Offset 27
                        Store (\_SB.PCI0.LPC0.EC0.MGIC, \_SB.UBTC.MGIC) // Offset 28
                        Store (\_SB.PCI0.LPC0.EC0.MGID, \_SB.UBTC.MGID) // Offset 29
                        Store (\_SB.PCI0.LPC0.EC0.MGIE, \_SB.UBTC.MGIE) // Offset 30
                        Store (\_SB.PCI0.LPC0.EC0.MGIF, \_SB.UBTC.MGIF) // Offset 31
                        Store (\_SB.PCI0.LPC0.EC0.CCI0, \_SB.UBTC.CCI0) // Offset  4
                        Store (\_SB.PCI0.LPC0.EC0.CCI1, \_SB.UBTC.CCI1) // Offset  5
                        Store (\_SB.PCI0.LPC0.EC0.CCI2, \_SB.UBTC.CCI2) // Offset  6
                        Store (\_SB.PCI0.LPC0.EC0.CCI3, \_SB.UBTC.CCI3) // Offset  7

                        Release(\_SB.PCI0.LPC0.EC0.CpmKbcMutex)
                    }
                } Else {
                    Store (\_SB.MGI0, \_SB.UBTC.MGI0) // Offset 16
                    Store (\_SB.MGI1, \_SB.UBTC.MGI1) // Offset 17
                    Store (\_SB.MGI2, \_SB.UBTC.MGI2) // Offset 18
                    Store (\_SB.MGI3, \_SB.UBTC.MGI3) // Offset 19
                    Store (\_SB.MGI4, \_SB.UBTC.MGI4) // Offset 20
                    Store (\_SB.MGI5, \_SB.UBTC.MGI5) // Offset 21
                    Store (\_SB.MGI6, \_SB.UBTC.MGI6) // Offset 22
                    Store (\_SB.MGI7, \_SB.UBTC.MGI7) // Offset 23
                    Store (\_SB.MGI8, \_SB.UBTC.MGI8) // Offset 24
                    Store (\_SB.MGI9, \_SB.UBTC.MGI9) // Offset 25
                    Store (\_SB.MGIA, \_SB.UBTC.MGIA) // Offset 26
                    Store (\_SB.MGIB, \_SB.UBTC.MGIB) // Offset 27
                    Store (\_SB.MGIC, \_SB.UBTC.MGIC) // Offset 28
                    Store (\_SB.MGID, \_SB.UBTC.MGID) // Offset 29
                    Store (\_SB.MGIE, \_SB.UBTC.MGIE) // Offset 30
                    Store (\_SB.MGIF, \_SB.UBTC.MGIF) // Offset 31
                    Store (\_SB.CCI0, \_SB.UBTC.CCI0) // Offset  4
                    Store (\_SB.CCI1, \_SB.UBTC.CCI1) // Offset  5
                    Store (\_SB.CCI2, \_SB.UBTC.CCI2) // Offset  6
                    Store (\_SB.CCI3, \_SB.UBTC.CCI3) // Offset  7
                }
                Add(CpmUcsiEcUcsiCommand, 1, Local1)
                CpmKbcCmd(CpmUcsiEcCommandIo, 0, Local1, 0) // READ_DONE command (ID = WRITE_DONE + 1)
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
                        Return (Buffer (1) {0x0F})
                    //
                    // Function 1:
                    //
                    } ElseIf (LEqual (ToInteger (Arg2), 1)) {
                        CpmSetPostCode(CpmTpUcsiDsmF1Start)
                        If (LEqual (CpmUcsiEcUcsiLocation, 0)) {
                            If (LEqual(\_SB.PCI0.LPC0.EC0.OKEC, 0x01)) {
                                Acquire(\_SB.PCI0.LPC0.EC0.CpmKbcMutex, 0xFFFF)

                                Store (\_SB.UBTC.MGO0, \_SB.PCI0.LPC0.EC0.MGO0) // Offset 32
                                Store (\_SB.UBTC.MGO1, \_SB.PCI0.LPC0.EC0.MGO1) // Offset 33
                                Store (\_SB.UBTC.MGO2, \_SB.PCI0.LPC0.EC0.MGO2) // Offset 34
                                Store (\_SB.UBTC.MGO3, \_SB.PCI0.LPC0.EC0.MGO3) // Offset 35
                                Store (\_SB.UBTC.MGO4, \_SB.PCI0.LPC0.EC0.MGO4) // Offset 36
                                Store (\_SB.UBTC.MGO5, \_SB.PCI0.LPC0.EC0.MGO5) // Offset 37
                                Store (\_SB.UBTC.MGO6, \_SB.PCI0.LPC0.EC0.MGO6) // Offset 38
                                Store (\_SB.UBTC.MGO7, \_SB.PCI0.LPC0.EC0.MGO7) // Offset 39
                                Store (\_SB.UBTC.MGO8, \_SB.PCI0.LPC0.EC0.MGO8) // Offset 40
                                Store (\_SB.UBTC.MGO9, \_SB.PCI0.LPC0.EC0.MGO9) // Offset 41
                                Store (\_SB.UBTC.MGOA, \_SB.PCI0.LPC0.EC0.MGOA) // Offset 42
                                Store (\_SB.UBTC.MGOB, \_SB.PCI0.LPC0.EC0.MGOB) // Offset 43
                                Store (\_SB.UBTC.MGOC, \_SB.PCI0.LPC0.EC0.MGOC) // Offset 44
                                Store (\_SB.UBTC.MGOD, \_SB.PCI0.LPC0.EC0.MGOD) // Offset 45
                                Store (\_SB.UBTC.MGOE, \_SB.PCI0.LPC0.EC0.MGOE) // Offset 46
                                Store (\_SB.UBTC.MGOF, \_SB.PCI0.LPC0.EC0.MGOF) // Offset 47
                                Store (\_SB.UBTC.CTL0, \_SB.PCI0.LPC0.EC0.CTL0) // Offset  8
                                Store (\_SB.UBTC.CTL1, \_SB.PCI0.LPC0.EC0.CTL1) // Offset  9
                                Store (\_SB.UBTC.CTL2, \_SB.PCI0.LPC0.EC0.CTL2) // Offset 10
                                Store (\_SB.UBTC.CTL3, \_SB.PCI0.LPC0.EC0.CTL3) // Offset 11
                                Store (\_SB.UBTC.CTL4, \_SB.PCI0.LPC0.EC0.CTL4) // Offset 12
                                Store (\_SB.UBTC.CTL5, \_SB.PCI0.LPC0.EC0.CTL5) // Offset 13
                                Store (\_SB.UBTC.CTL6, \_SB.PCI0.LPC0.EC0.CTL6) // Offset 14
                                Store (\_SB.UBTC.CTL7, \_SB.PCI0.LPC0.EC0.CTL7) // Offset 15
                                Store (0x00, \_SB.PCI0.LPC0.EC0.CCI0) // Offset  4  // Clear the previous status
                                Store (0x00, \_SB.PCI0.LPC0.EC0.CCI1) // Offset  5  // Clear the previous status
                                Store (0x00, \_SB.PCI0.LPC0.EC0.CCI2) // Offset  6  // Clear the previous status
                                Store (0x00, \_SB.PCI0.LPC0.EC0.CCI3) // Offset  7  // Clear the previous status

                                Release(\_SB.PCI0.LPC0.EC0.CpmKbcMutex)
                            }
                        } Else {
                            Store (\_SB.UBTC.MGO0, \_SB.MGO0) // Offset 32
                            Store (\_SB.UBTC.MGO1, \_SB.MGO1) // Offset 33
                            Store (\_SB.UBTC.MGO2, \_SB.MGO2) // Offset 34
                            Store (\_SB.UBTC.MGO3, \_SB.MGO3) // Offset 35
                            Store (\_SB.UBTC.MGO4, \_SB.MGO4) // Offset 36
                            Store (\_SB.UBTC.MGO5, \_SB.MGO5) // Offset 37
                            Store (\_SB.UBTC.MGO6, \_SB.MGO6) // Offset 38
                            Store (\_SB.UBTC.MGO7, \_SB.MGO7) // Offset 39
                            Store (\_SB.UBTC.MGO8, \_SB.MGO8) // Offset 40
                            Store (\_SB.UBTC.MGO9, \_SB.MGO9) // Offset 41
                            Store (\_SB.UBTC.MGOA, \_SB.MGOA) // Offset 42
                            Store (\_SB.UBTC.MGOB, \_SB.MGOB) // Offset 43
                            Store (\_SB.UBTC.MGOC, \_SB.MGOC) // Offset 44
                            Store (\_SB.UBTC.MGOD, \_SB.MGOD) // Offset 45
                            Store (\_SB.UBTC.MGOE, \_SB.MGOE) // Offset 46
                            Store (\_SB.UBTC.MGOF, \_SB.MGOF) // Offset 47
                            Store (\_SB.UBTC.CTL0, \_SB.CTL0) // Offset  8
                            Store (\_SB.UBTC.CTL1, \_SB.CTL1) // Offset  9
                            Store (\_SB.UBTC.CTL2, \_SB.CTL2) // Offset 10
                            Store (\_SB.UBTC.CTL3, \_SB.CTL3) // Offset 11
                            Store (\_SB.UBTC.CTL4, \_SB.CTL4) // Offset 12
                            Store (\_SB.UBTC.CTL5, \_SB.CTL5) // Offset 13
                            Store (\_SB.UBTC.CTL6, \_SB.CTL6) // Offset 14
                            Store (\_SB.UBTC.CTL7, \_SB.CTL7) // Offset 15
                            Store (0x00, \_SB.CCI0) // Offset  4  // Clear the previous status
                            Store (0x00, \_SB.CCI1) // Offset  5  // Clear the previous status
                            Store (0x00, \_SB.CCI2) // Offset  6  // Clear the previous status
                            Store (0x00, \_SB.CCI3) // Offset  7  // Clear the previous status
                        }
                        CpmKbcCmd(CpmUcsiEcCommandIo, 0, CpmUcsiEcUcsiCommand, 0) // WRITE_DONE command
                        CpmSetPostCode(CpmTpUcsiDsmF1End)

                    //
                    // Function 2:
                    //
                    } ElseIf (LEqual (ToInteger (Arg2), 2)) {
                        CpmSetPostCode(CpmTpUcsiDsmF2Start)
                        CpmUcsiWriteFromEc ()
                        CpmSetPostCode(CpmTpUcsiDsmF2End)
                    }
                }
                Return(0)
            } // End of Method (_DSM, 4, Serialized)
        } // End of Device (UBTC)
    } // End of Scope (\_SB)
} // End of DefinitionBlock()

