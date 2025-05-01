/*****************************************************************************
 *
 * Copyright (C) 2022-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "IGpuGlobalMethod4.aml",        // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMDFIG4",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_DISPLAY_FEATURE_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.PBC.VGA, DeviceObj)
    External(\_SB.ALIB, MethodObj)
    External(\_SB.LID._LID, MethodObj)

    External(\_SB.PCI0.PBC.VGA.AFN0, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN1, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN2, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN3, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN4, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN5, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN6, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN7, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFN8, MethodObj)
    External(\_SB.PCI0.PBC.VGA.AFNC, MethodObj)

    Scope(\_SB.PCI0.PBC.VGA)
    {
        #include "../Common/ATCS.asi"
    }

    Method(AFN0, 0, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN0)) {
            \_SB.PCI0.PBC.VGA.AFN0()
        }
    }
    Method(AFN1, 1, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN1)) {
            \_SB.PCI0.PBC.VGA.AFN1(Arg0)
        }
    }
    Method(AFN2, 2, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN2)) {
            \_SB.PCI0.PBC.VGA.AFN2(Arg0, Arg1)
        }
    }
    Method(AFN3, 2, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN3)) {
            \_SB.PCI0.PBC.VGA.AFN3(Arg0, Arg1)
        }
    }
    Method(AFN4, 1, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN4)) {
            \_SB.PCI0.PBC.VGA.AFN4(Arg0)
        }
    }
    Method(AFN5, 0, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN5)) {
            \_SB.PCI0.PBC.VGA.AFN5()
        }
    }
    Method(AFN6, 0, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN6)) {
            \_SB.PCI0.PBC.VGA.AFN6()
        }
    }
    Method(AFN7, 1, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN7)) {
            \_SB.PCI0.PBC.VGA.AFN7(Arg0)
        }
    }
    Method(AFN8, 0, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFN8)) {
            \_SB.PCI0.PBC.VGA.AFN8()
        }
    }
    Method(AFNC, 2, Serialized)
    {
        If(CondRefOf(\_SB.PCI0.PBC.VGA.AFNC)) {
            \_SB.PCI0.PBC.VGA.AFNC(Arg0, Arg1)
        }
    }

}