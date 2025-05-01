/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "Gpp0.aml",         // Output file
    "SSDT",             // Signature
    0x01,               // SSDT Revision
    "AMD",              // OEM ID
    "GPP0",             // OEM Table ID
    0x1                 // OEM Revision
    )
{
  External(\_SB.PCI0.GPP0, DeviceObj)

  Scope(\_SB.PCI0.GPP0)
  {
    Device(DEV0)
    {
      Name(_ADR, 0x00)
    }
  }

}