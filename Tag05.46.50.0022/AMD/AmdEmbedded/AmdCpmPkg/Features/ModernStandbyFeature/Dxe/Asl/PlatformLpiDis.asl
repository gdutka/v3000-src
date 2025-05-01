/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "PlatformLpi.aml",         // Output file
    "SSDT",                    // Signature
    0x01,                      // SSDT Revision
    "AMD",                     // OEM ID
    "MSLPIDIS",                // OEM Table ID
    0x1                        // OEM Revision
    )
{
  External(\_SB.PLTF, DeviceObj)

  Scope(\_SB.PLTF) {
      Name(_LPI, Package(4)
      {
          0x0000,
          0x0000000000000000,
          0x0001,
          Package(10)
          {
                  1000000,     // S0i3 residency = 1000000
                  50000,     // S0i3 wakeup latency = 50000
                  Zero,
                  Zero,
                  Zero,
                  Zero,
                  Zero,
                  ResourceTemplate(){Register(SystemMemory, 0x00, 0x00, 0x0000000000000000, 0x00,)},
                  ResourceTemplate(){Register(SystemMemory, 0x00, 0x00, 0x0000000000000000, 0x00,)},
                  "S0i3"
          }
      })
  }
}
