/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "DGpuScope1.aml",               // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMDFDG1",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_DISPLAY_FEATURE_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"

    External (\_SB.PCI0.PB2, DeviceObj)
    External (\_SB.PCI0.PB2.CpmDgpuDsmFunBData, IntObj)
    External (\_SB.PCI0.PB2.CpmDgpuSetPower, MethodObj)

    Scope (\_SB.PCI0.PB2) {
        PowerResource (CpmDgpuPowerResourceVcc, 0, 0) {
            Name (CpmDgpuPowerResourceVccFlag, 1)
            Method (_STA) {
                Return (CpmDgpuPowerResourceVccFlag)
            }

            Method (_ON) {
                If (LEqual (CpmDgpuPowerResourceVccFlag, 0)) {
                    If (CondRefOf (\_SB.PCI0.PB2.CpmDgpuSetPower)) {
                        \_SB.PCI0.PB2.CpmDgpuSetPower (CPM_POWER_ON)
                    }
                }
                Store (0x01, CpmDgpuPowerResourceVccFlag)
            }

            Method (_OFF) {
                If (LEqual (CpmDgpuPowerResourceVccFlag, 1)) {
                    If (CondRefOf (\_SB.PCI0.PB2.CpmDgpuSetPower)) {
                        \_SB.PCI0.PB2.CpmDgpuSetPower (CPM_POWER_OFF)
                    }
                }
                Store (0x00, CpmDgpuPowerResourceVccFlag)
            }
        }

        Name (XPR0, Package() {CpmDgpuPowerResourceVcc})                        // Power resources required for D0
        Name (XPR2, Package() {CpmDgpuPowerResourceVcc})                        // Power resources required for D2
        Name (XPR3, Package() {CpmDgpuPowerResourceVcc})                        // Power resources required for D3
        Name (XS0W, 4)

        Device (VGA) {
            Name (_ADR, 0x00)

            Method (_RMV, 0, NotSerialized) {
                Return (0)
            }
        } // End of Device (VGA)

        Device (HDAU) {
            Name (_ADR, 0x01)

            Method (_RMV, 0, NotSerialized) {
                Return (0)
            }

            Method (_STA, 0) {
                If (LNotEqual (CpmdGpuAudioDisable, 0)) { // PX enabled
                    Return (0x0)                          // Disable Audio device
                } Else {
                    Return (0xF)                          // Enable Audio device
                }
            }
        } // End of Device (HDAU)
        #include "../Common/DgpuDsd.asi"
        #include "../Common/DgpuDsm.asi"
    } // End of Scope (\_SB.PCI0.PB2)
} // End of DefinitionBlock
