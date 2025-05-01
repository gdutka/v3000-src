/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
DefinitionBlock (
    "DRTM.aml",                 // Output file
    "SSDT",                     // Signature
    0x01,                       // SSDT Revision
    "AMD",                      // OEM ID
    "DRTM",                     // OEM Table ID
    0x1                         // OEM Revision
    )
{

    Scope(\_SB)     // Start \_SB scope
    {
      Device(DRTM) {
        Name (_HID, "DRTM0001")
        Name(_UID, 0)

        Method(_STA, 0, NotSerialized) {
          Return (0x0F)
        }
      } // End Device DRTM
    }

}

