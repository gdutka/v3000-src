/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
DefinitionBlock (
    "Mpm.aml",                 // Output file
    "SSDT",                     // Signature
    0x01,                       // SSDT Revision
    "AMD",                      // OEM ID
    "MPM0",                     // OEM Table ID
    0x1                         // OEM Revision
    )
{
    Scope(\_SB)     // Start \_SB scope
    {
      Device(MPM0) {
        Name(_HID, "AMDI0090")
        Name(_UID, Zero)
        Name(_CRS, ResourceTemplate()
        {
          Interrupt (ResourceConsumer, Level, ActiveLow, Exclusive, , , ) {0x36,}
          Memory32Fixed(ReadWrite, 0xAAAAAAAA, 0x00020000)//update the address properly for MPM registers base address
          Memory32Fixed(ReadWrite, 0xBBBBBBBB, 0x00000004)//update the address properly for PSP registers base address
          Memory32Fixed(ReadWrite, 0xCCCCCCCC, 0x00000004)//update the address properly for PSP registers base address
        })

        Method(_STA, 0, NotSerialized)
        {
          Return (0x0F)
        }
      } // End Device MPM
    }
}

