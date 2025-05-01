/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
  "WirelessLanRecovery.aml", // Output file
  "SSDT",                    // Signature
  0x01,                      // SSDT Revision
  "AMD",                     // OEM ID
  "CPMWLRC",                 // OEM Table ID
  0x1                        // OEM Revision
  )
{

  #include "CmnDef.asi"
  #include "CmnExt.asi"
  #include "PostCode.asi"

  External (\_SB.PCDD.GPEE.USFF.DSGG.DEHH, DeviceObj)
  External (\_SB.PCXX.GPXX.USXX.DSXX.XHCX.HUBX.PTXX, DeviceObj)

  If (CondRefOf (\_SB.PCDD.GPEE.USFF.DSGG.DEHH)) {
    Scope (\_SB.PCDD.GPEE.USFF.DSGG.DEHH) {
      Name (YPRR, Package () {\_SB.PRWL})

      Method (YRMV, 0, NotSerialized) {
        Return (0)
      }
    } // End of Scope (\_SB.PCDD.GPEE.USFF.DSGG.DEHH)
  }

  If (CondRefOf (\_SB.PCXX.GPXX.USXX.DSXX.XHCX.HUBX.PTXX)) {
    Scope (\_SB.PCXX.GPXX.USXX.DSXX.XHCX.HUBX.PTXX) {
      Name (ZPRR, Package () {\_SB.PRWB})
    } // End of Scope (\_SB.PCXX.GPXX.USXX.DSXX.XHCX.HUBX.PTXX)
  }

  Scope (\_SB) {
    Name (WLPS, 1)

    PowerResource (PRWL, 0, 0) {
      Method (_RST, 0x0, NotSerialized) {
        CpmSetPostCode (CpmTpWirelessLanRecoveryResetStart)
        Store (CpmReadTable32 (CpmWirelessLanRecoveryTable, CpmWirelessLanRecoverySupport), Local2)
        If (LNotEqual(Local2, 2))                 // Not Dummy Reset
        {
          Store (CpmReadTable (CpmWirelessLanRecoveryTable, CpmWirelessLanRecoveryDeviceId), Local0)
          Store (CpmReadTable32 (CpmWirelessLanRecoveryTable, CpmWirelessLanRecoveryResetDelay), Local1)
          CpmSetDeviceReset (Local0, 0)         // Assert Reset
          Sleep (Local1)
          CpmSetDeviceReset (Local0, 1)         // De-assert Reset
        }
        CpmSetPostCode (CpmTpWirelessLanRecoveryResetEnd)
      }

      Method (_STA, 0x0, NotSerialized) {
        Return (WLPS)
      }

      Method (_ON, 0x0, NotSerialized) {
        Store (1, WLPS)
      }

      Method (_OFF, 0x0, NotSerialized) {
        Store (0, WLPS)
      }
    } // End of PowerResource (PRWL, 0, 0)


    Name (BLPS, 1)

    PowerResource (PRWB, 0, 0) {
      Method (_RST, 0x0, NotSerialized) {
        CpmSetPostCode (CpmTpBluetoothRecoveryResetStart)
        Store (CpmReadTable32 (CpmWirelessLanRecoveryTable, CpmBluetoothRecoverySupport), Local2)
        If (LNotEqual(Local2, 2))               // Not Dummy Reset
        {
          Store (CpmReadTable (CpmWirelessLanRecoveryTable, CpmBluetoothRecoveryDeviceId), Local0)
          Store (CpmReadTable32 (CpmWirelessLanRecoveryTable, CpmBluetoothRecoveryDelay), Local1)
          CpmSetDeviceReset (Local0, 0)         // Assert Reset
          Sleep (Local1)
          CpmSetDeviceReset (Local0, 1)         // De-assert Reset
        }
        CpmSetPostCode (CpmTpBluetoothRecoveryResetEnd)
      }

      Method (_STA, 0x0, NotSerialized) {
        Return (BLPS)
      }

      Method (_ON, 0x0, NotSerialized) {
        Store (1, BLPS)
      }

      Method (_OFF, 0x0, NotSerialized) {
        Store (0, BLPS)
      }
    } // End of PowerResource (PRWB, 0, 0)
  } // End of Scope (\_SB)
}

