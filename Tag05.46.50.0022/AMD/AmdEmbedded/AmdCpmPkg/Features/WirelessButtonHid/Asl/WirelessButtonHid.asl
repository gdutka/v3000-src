/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
  "WirelessButtonHid.aml",       // Output file
  "SSDT",                        // Signature
  0x01,                          // SSDT Revision
  "AMD",                         // OEM ID
  "CPMWBH",                      // OEM Table ID
  0x1                            // OEM Revision
  )
{

  #include "CmnDef.asi"
  #include "CmnExt.asi"
  #include "PostCode.asi"

  External(\_SB.PCI0.LPC0.EC0, DeviceObj)

  Scope (\_SB) {
    Device (HDWB)
    {
      Name (_HID, "AMDI0051")
      Name (_CID, "AMDI0051")
      Name (_UID, 0)
    }
  } // End of Scope (\_SB)

  Scope (\_SB.PCI0.LPC0.EC0) {
    Method (XQ28, 0, NotSerialized) {
      CpmSetPostCode(CpmTpWirelessButtonHidDevXQ28Start)
      Notify (\_SB.HDWB, 0x80)
      CpmSetPostCode(CpmTpWirelessButtonHidDevXQ28End)
    }
  } // End of Scope (\_SB.PCI0.LPC0.EC0)
}

