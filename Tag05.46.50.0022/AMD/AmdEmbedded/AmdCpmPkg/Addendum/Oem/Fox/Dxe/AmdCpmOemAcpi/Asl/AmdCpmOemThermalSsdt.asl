/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
// Thermal SSDT
DefinitionBlock (
    "Thermal.aml",                // Output file
    "SSDT",                       // Signature
    0x01,                         // SSDT Revision
    "AMD",                        // OEM ID
    "THERMAL0",                   // OEM Table ID = ('T','H','E','R','M','A','L','0')
    0x1                           // OEM Revision
    )
{
    #define CPM_THERMAL_0_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"

    External (\_SB.PCI0.LPC0.EC0.OKEC, IntObj)
    External (\_SB.PCI0.LPC0.EC0.CpmKbcMutex, MutexObj)
    External (\_SB.PCI0.LPC0.EC0.DIEH, FieldUnitObj)
    External (\_SB.PLTF.C000, DeviceObj)
    External (\_SB.PLTF.C001, DeviceObj)
    External (\_SB.PLTF.C002, DeviceObj)
    External (\_SB.PLTF.C003, DeviceObj)
    External (\_SB.PLTF.C004, DeviceObj)
    External (\_SB.PLTF.C005, DeviceObj)
    External (\_SB.PLTF.C006, DeviceObj)
    External (\_SB.PLTF.C007, DeviceObj)
    External (\_SB.PLTF.C008, DeviceObj)
    External (\_SB.PLTF.C009, DeviceObj)
    External (\_SB.PLTF.C00A, DeviceObj)
    External (\_SB.PLTF.C00B, DeviceObj)
    External (\_SB.PLTF.C00C, DeviceObj)
    External (\_SB.PLTF.C00D, DeviceObj)
    External (\_SB.PLTF.C00E, DeviceObj)
    External (\_SB.PLTF.C00F, DeviceObj)

    //
    // Thermal device
    //
    Scope (\_TZ) {
        ThermalZone (TZ01) {
            Name (CRTT, 110)        // Default _CRT temperature.
            Name (PSVT, 50)         // Default _PSV temperature.
            Name (TSPS, 20)         // Default 2s for _TSP time.

            Method (_TMP, 0, Serialized) {
                Store (2932, Local0)
                If (CondRefOf (\_SB.PCI0.LPC0.EC0.OKEC)) {
                    If (LEqual (\_SB.PCI0.LPC0.EC0.OKEC, 0x01)) {
                        If (CondRefOf (\_SB.PCI0.LPC0.EC0.CpmKbcMutex)) {
                            Acquire (\_SB.PCI0.LPC0.EC0.CpmKbcMutex, 0xFFFF)
                            If (CondRefOf (\_SB.PCI0.LPC0.EC0.DIEH)) {
                                Store (\_SB.PCI0.LPC0.EC0.DIEH, Local0)
                                Add (2732, Multiply (Local0, 10), Local0)
                            }
                            Release (\_SB.PCI0.LPC0.EC0.CpmKbcMutex)
                        }
                    }
                }
                Return (Local0)     // Default 20 degrees C
            }

            Method (_PSL, 0, Serialized) {
                Return (Package () {
                    \_SB.PLTF.C000,
                    \_SB.PLTF.C001,
                    \_SB.PLTF.C002,
                    \_SB.PLTF.C003,
                    \_SB.PLTF.C004,
                    \_SB.PLTF.C005,
                    \_SB.PLTF.C006,
                    \_SB.PLTF.C007,
                    \_SB.PLTF.C008,
                    \_SB.PLTF.C009,
                    \_SB.PLTF.C00A,
                    \_SB.PLTF.C00B,
                    \_SB.PLTF.C00C,
                    \_SB.PLTF.C00D,
                    \_SB.PLTF.C00E,
                    \_SB.PLTF.C00F
                })
            }

            Method (_CRT, 0, Serialized) {
                Add (2732, Multiply (CRTT, 10), Local0)
                Return (Local0)
            }

            //
            // DP [%] = _TC1 * ( Tn - Tn-1 ) + _TC2 * (Tn - Tt)
            //
            Method (_TC1, 0, Serialized) {
                Return (1)
            }

            Method (_TC2, 0, Serialized) {
                Return (2)
            }

            Method (_TSP, 0, Serialized) {
                Return (TSPS)
            }
        } // End of ThermalZone (TZ01)
    } // End of Scope (\_TZ)
} // End of DefinitionBlock

