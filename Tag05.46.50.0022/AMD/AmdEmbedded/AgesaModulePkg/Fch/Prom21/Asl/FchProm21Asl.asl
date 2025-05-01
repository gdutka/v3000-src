/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

DefinitionBlock (
  "FchProm21Asl.aml",   // Output Filename
  "SSDT",               // Signature
  0x01,                 // DSDT Compliance Revision
  "AMD",                // OEMID
  "FCHPT",              // OEM TABLE ID
  0x1000                // OEM Revision
  )
{
  Name (PTOS, Zero)

  If (CondRefOf (\_OSI)) {
    If (\_OSI ("Windows 2015")) {
      Store (0x70, PTOS)
    }
  }

  Scope (\_SB) {
    Device (PTIO) {
      Name (_HID, "AMDIF031")
      Name (_CID, "AMDIF031")
      Name (_UID, 0)

      Method (_CRS, 0) {
        Name (RBUF, ResourceTemplate () {
          Memory32Fixed (ReadWrite, 0xFEC30000, 0x00001000)
        })

        Return (RBUF)
      }

      Method (_STA, 0) {
        If (LGreaterEqual (PTOS, 0x70)) {
          Return (0x0F)
        } Else {
          Return (0x00)
        }
      } // End of Method (_STA, 0, NotSerialized)
    } // End of Device (PTIO)
  } // End of Scope (\_SB)
} // End of DefinitionBlock

