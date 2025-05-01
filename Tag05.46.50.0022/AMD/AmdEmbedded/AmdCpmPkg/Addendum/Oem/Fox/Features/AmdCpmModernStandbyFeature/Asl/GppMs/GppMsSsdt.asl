/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "GppMsSsdt.aml", // Output file
    "SSDT",          // Signature
    0x01,            // SSDT Revision
    "AMD",           // OEM ID
    "GpMsSsdt",      // OEM Table ID
    0x1              // OEM Revision
    )
{

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.GPP1, DeviceObj)
    External(\_SB.PCI0.GPP2, DeviceObj)
    External(\_SB.PCI0.GPP2.WWAN, DeviceObj)

    External(\_SB.PCI0.GPP5, DeviceObj)
    External(\_SB.PCI0.GPP5.RTL8, DeviceObj)

    External(\_SB.PCI0.GPP6, DeviceObj)
    External(\_SB.PCI0.GPP6.WLAN, DeviceObj)

    External(\_SB.PCI0.GPP7, DeviceObj)
    External(\_SB.PCI0.GPP7.DEV0, DeviceObj)

    #define QCOM_VENDOR_ID                 0x17CB
    #define QCOM_DEVICE_NFA765_ID          0x1103

    // GPP1 (Bus 0 Dev 1 Fn 2)
    Scope(\_SB.PCI0.GPP1) {
        Method(_PRW,0,NotSerialized) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP1._PRW Return GPRW (0x8, 0x4)\n", 0, 0, 0, 0, 0, 0)
            Return (Package (2) {0x08, 0x04})
        }
    }

    // GPP2 (Bus 0 Dev 1 Fn 3)
    Scope(\_SB.PCI0.GPP2) {
        Method(_PRW,0,NotSerialized) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP2._PRW Return GPRW (0x8, 0x4)\n", 0, 0, 0, 0, 0, 0)
            Return (Package (2) {0x08, 0x04})
        }
    }

    Scope(\_SB.PCI0.GPP2.WWAN) {
        Name(_S0W, 4)
    }

    // GPP5 (Bus 0 Dev 2 Fn 1)
    Scope(\_SB.PCI0.GPP5) {
        Name(EWPM, 0xFF)
        Method(_PRW,0,NotSerialized) {
          if (Lequal(EWPM, 0x1)) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP5._PRW Return GPRW (0xF, 0x4)\n", 0, 0, 0, 0, 0, 0)
            Return (Package (2) {0x0F, 0x04})
          } else {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP5._PRW Return GPRW (0xF, 0x0)\n", 0, 0, 0, 0, 0, 0)
            Return (Package (2) {0x0F, 0x00})
          }
        }
    }

    Scope(\_SB.PCI0.GPP5.RTL8) {
        Name(_S0W, 4)
    }

    // GPP6 (Bus 0 Dev 2 Fn 2)
    Scope(\_SB.PCI0.GPP6) {
        Name(_S0W, 0)
        Method(_PRW,0,NotSerialized) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6._PRW Return GPRW (0xE, 0x4)\n", 0, 0, 0, 0, 0, 0)
            Return (Package (2) {0x0E, 0x04})
        }
        Method(QDSW, 0x3, NotSerialized) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6._DSW\n", 0, 0, 0, 0, 0, 0)
        }
    }

    Scope(\_SB.PCI0.GPP6.WLAN) {
        OperationRegion(PCIC,PCI_Config, 0x00, 0x4)
        Field(PCIC, WordAcc, NoLock, Preserve) {
            DVID, 16, // Device ID
            DDID, 16, // Vendor ID
        }

        Method(_S0W, 0) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6.WLAN._S0W - DID:%x VID:%x\n", DVID, DDID, 0, 0, 0, 0)
            If(LAnd(LEqual(DVID, QCOM_VENDOR_ID), LEqual(DDID, QCOM_DEVICE_NFA765_ID))) {
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6.WLAN._S0W : 3\n", 0, 0, 0, 0, 0, 0)
                Return (3)
            } else {
                CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6.WLAN._S0W : 4\n", 0, 0, 0, 0, 0, 0)
                Return (4)
            }
        }
        Method(QDSW, 0x3, NotSerialized) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6.WLAN._DSW\n", 0, 0, 0, 0, 0, 0)
        }
    }
    // GPP7 (Bus 0 Dev 2 Fn 3)
    Scope(\_SB.PCI0.GPP7) {
        Method(_PRW,0,NotSerialized) {
            CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP7._PRW Return GPRW (0x2, 0x4)\n", 0, 0, 0, 0, 0, 0)
            Return (Package (2) {0x02, 0x04})
        }
    }

    Scope(\_SB.PCI0.GPP7.DEV0) {
        Name(_S0W, 4)
    }
}
