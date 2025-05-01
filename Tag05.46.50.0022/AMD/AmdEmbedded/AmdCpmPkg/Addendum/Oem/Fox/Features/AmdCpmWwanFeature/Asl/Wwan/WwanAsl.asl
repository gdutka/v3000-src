/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "WwanSsdt.aml",  // Output file
    "SSDT",          // Signature
    0x02,            // SSDT Revision
    "AMD",           // OEM ID
    "WwanSsdt",      // OEM Table ID
    0x1              // OEM Revision
    )
{

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.GPP2, DeviceObj)
    External(\_SB.PCI0.GPP2.WWAN, DeviceObj)

    External(TPST, MethodObj)
    External(\_SB.PCI0.LPC0.EC0.Z009, MutexObj)
    External(\_SB.PCI0.LPC0.ECOK, MethodObj)
    External(\_SB.PCI0.LPC0.EC0.GPO3, FieldUnitObj)
    External(\_SB.PCI0.LPC0.EC0.GPI7, FieldUnitObj)
    External(\_SB.PCI0.LPC0.EC0.GPIB, FieldUnitObj)

    // GPP2 (Bus 0 Dev 1 Fn 3)
    Scope(\_SB.PCI0.GPP2) {
        Name(_S0W, 4)

        Name(_PR0, Package(1) {P0WW})
        Name(_PR3, Package(1) {P0WW})


        Name (_PRR, Package () {\_SB.PCI0.GPP2.P0WW})
        Name(D0WW, 1)
        PowerResource(P0WW, 0, 0) {
            Method(_RST, 0, NotSerialized)
            {
                TPST(0x9955)
                Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)
                Store (\_SB.PCI0.LPC0.EC0.GPO3, Local0)	//PERST#
                And(Local0, 0xEF, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPO3)
                Sleep(30)

                Store (\_SB.PCI0.LPC0.EC0.GPI7, Local0)	//RESET#
                And(Local0, 0xDF, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPI7)
                Sleep(10)

                Store (\_SB.PCI0.LPC0.EC0.GPIB, Local0)	//FCPO# (FULL_CARD_POWER_OFF#)
                And(Local0, 0x7F, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPIB)
                Release(\_SB.PCI0.LPC0.EC0.Z009)
                Sleep(500)
                Notify (WWAN, 1)  //Device removal

                Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)
                Store (\_SB.PCI0.LPC0.EC0.GPIB, Local0)	//FCPO# (FULL_CARD_POWER_OFF#)
                Or(Local0, 0x80, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPIB)
                Sleep(30)

                Store (\_SB.PCI0.LPC0.EC0.GPI7, Local0)	//RESET#
                Or(Local0, 0x20, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPI7)
                Sleep(70)

                Store (\_SB.PCI0.LPC0.EC0.GPO3, Local0)	//PERST#
                Or(Local0, 0x10, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPO3)
                Notify (WWAN, 1)  //Device insert
                Release(\_SB.PCI0.LPC0.EC0.Z009)

                TPST(0x99AA)
            }

            Method(_STA)
            {
                TPST(0x305A)
                Return(D0WW)
            }

            Method(_ON)
            {
                TPST(0x30D0)
                If(LNotEqual (D0WW, 1))
                {
                    CpmDebugPrint ("OEM-ASL-\\_SB.PCI0.GPP2.P0WW._ON\n", 0, 0, 0, 0, 0, 0)
                    WWON()
                    Store(0x01, D0WW)
                }
            }
            Method(_OFF) {
                TPST(0x30D3)
                If(LNotEqual (D0WW, 0))
                {
                    CpmDebugPrint ("OEM-ASL-\\_SB.PCI0.GPP2.P0WW._OFF\n", 0, 0, 0, 0, 0, 0)
                    WOFF()
                    Store(0x00, D0WW)
                }
            }
        }

        Method (WWON, 0, NotSerialized)
        {
            TPST(0x20D0)
            If(\_SB.PCI0.LPC0.ECOK()) {

                //Reset#
                Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)

                Store (\_SB.PCI0.LPC0.EC0.GPO3, Local0)	//PERST#

                If (LNotEqual(And(Local0, 0x10), 0x10)) {
                CpmProgramHoldTraining (0, 1, 3, 0)
                Or(Local0, 0x10, Local1)
                Store (Local1, \_SB.PCI0.LPC0.EC0.GPO3)
                Sleep (0x20)

                TPST(0x202E)
                CpmWakeLink (0, 1, 3, 1)
                Sleep (0x20)
                // Check DL_ACTIVE = 1 and LINK_TRAINING = 0
                Store (0, Local1)
                Store (10101, Local2)
                While ( LAnd ( LNotEqual (And (Local1, 0x28), 0x20), LGreater (Local2, 0) ) ) {
                    Store (CpmReadPci (0, 1, 3, 0x6B, 0, 8), Local1)
                    Store (Subtract (Local2, 1), Local2)
                    Stall (99)                              // 99us * 10101 = Max 999ms time out
                } // End of While (LNotEqual (And (Local1, 0x28), 0x20))
                CpmWakeLink (0, 1, 3, 0)
                Sleep (0x20)
                TPST(0x202F)
                }
                Release(\_SB.PCI0.LPC0.EC0.Z009)
            }
            TPST(0x21D0)
        }
        Method (WOFF, 0, NotSerialized)
        {
            TPST(0x20D3)
            If(\_SB.PCI0.LPC0.ECOK()) {
                Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)
                Store (\_SB.PCI0.LPC0.EC0.GPO3, Local0)	//PERST#
                If (LEqual(And(Local0, 0x10), 0x10)) {

                    //APPM
                    TPST(0x212E)
                    CpmSendPmeTurnOff(0, 1, 3)
                    TPST(0x212F)

                    Sleep (10)
                    Store (\_SB.PCI0.LPC0.EC0.GPO3, Local0)	//PERST#
                    And(Local0, 0xEF, Local1)
                    Store (Local1, \_SB.PCI0.LPC0.EC0.GPO3)
                    Sleep (30)
                }

                Release(\_SB.PCI0.LPC0.EC0.Z009)
                }
            TPST(0x21D3)
        }

    }

    Scope(\_SB.PCI0.GPP2.WWAN) {

        Method(_DSW, 3, NotSerialized)
        {
            Add (Arg0, 0x2D00, Local0)
            TPST (Local0)
        }
        Method(_RST, 0, NotSerialized)
        {
            TPST(0x2055)
            Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)

            Store (\_SB.PCI0.LPC0.EC0.GPI7, Local0)	//RESET#
            And(Local0, 0xDF, Local1)
            Store (Local1, \_SB.PCI0.LPC0.EC0.GPI7)
            Sleep(0xC8)
            Notify (WWAN, 1)  //Device removal

            Store (\_SB.PCI0.LPC0.EC0.GPI7, Local0)	//RESET#
            Or(Local0, 0x20, Local1)
            Store (Local1, \_SB.PCI0.LPC0.EC0.GPI7)
            Sleep(0x180)
            Notify (WWAN, 1)  //Device insert

            Release(\_SB.PCI0.LPC0.EC0.Z009)
            TPST(0x20AA)
        }

        Method(_DSM, 4, Serialized)
        {
            If(LEqual(Arg0, ToUUID("BAD01B75-22A8-4F48-8792-BDDE9467747D")))
            {
            If(LEqual(Arg2, Zero))
            {
                Return(Buffer(One) {0x09})
            }
            If(LEqual(Arg2, One)) {}
            If(LEqual(Arg2, 0x02)) {}
            If(LEqual(Arg2, 0x03)) {Return(0x03)}
            }
            Return(Buffer(One) {0x00})
        }

    }

}
