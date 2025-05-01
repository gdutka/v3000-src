/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

DefinitionBlock (
  "HSPMSSDT.dat", // Output Filename
  "SSDT",         // Signature
  0x02,           // DSDT Compliance Revision
  "MSFT  ",       // OEMID
  "MHSP",         // TABLE ID
  0x00000004        // OEM Revision
)
{
  Scope (\_SB) {
    Device (MHSP) {
      Name (_ADR, 0x00)
      Name (_HID, "MSFT0200")
      Name (_UID, 0x01)
      Name (_DDN, "Microsoft Pluton Processor")
      Name (_STR, Unicode ("Microsoft Pluton Processor"))

      Method (_CRS, 0, NotSerialized) {
        Name (RBUF, ResourceTemplate () {
          Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {
            50,   // 24+26, 24 is the GSI base of NB IOAPIC, 26 is IRQ for MHSP.
            51,
            52,
            53
          }
        })
        Return (RBUF)
      }

      Method (_STA, 0, NotSerialized) {
        Return (0x0F)
      }
    }
  }
}

