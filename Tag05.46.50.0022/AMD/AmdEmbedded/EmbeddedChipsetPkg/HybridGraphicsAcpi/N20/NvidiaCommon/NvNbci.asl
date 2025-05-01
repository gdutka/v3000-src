/** @file

;******************************************************************************
;* Copyright (c) 2020 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

/*
    == Readme ==
    For NBCI support, SBIOS has to adjust below code to meet platform:
    - Fill GSV1, GSV2(if need) and GSDR buffer from NVIDIA AE
    - \_SB.PCI0.PEG0.DEID -> Device ID of Host Bridge
    - Customer must update 'GSDR' for G-Sync platform (and correspondng VK key)
    - Please clear 'GSDR' for N17x (and earlier) platform
*/

Scope (DGPU_SCOPE)
{
    // NVIDIA NBCI related start
    Name (GSV1, Buffer(0x01)                 // Buffer for VK key#1
    {
        0x00
    })
    Name (GSV2, Buffer(0x01)                 // Buffer for VK key#2 (Optional)
    {
        0x00
    })
    Name(GSDR, Buffer(0xA1)                  // Buffer for DR key
    {
        0x57, 0x74, 0xDC, 0x86, 0x75, 0x84, 0xEC, 0xE7, 
        0x52, 0x44, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x01, 
        0x00, 0x00, 0x00, 0x00, 0xDE, 0x10, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x00, 
        0x00, 0x00, 0x01, 0x00, 0x47, 0x00, 0x00, 0x00, 
        0x02, 0x00, 0x45, 0x00, 0x00, 0x00, 0x03, 0x00, 
        0x51, 0x00, 0x00, 0x00, 0x04, 0x00, 0x4F, 0x00, 
        0x00, 0x00, 0x05, 0x00, 0x4D, 0x00, 0x00, 0x00, 
        0x06, 0x00, 0x4B, 0x00, 0x00, 0x00, 0x07, 0x00, 
        0x49, 0x00, 0x00, 0x00, 0x08, 0x00, 0x47, 0x00, 
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD9, 0x1C, 
        0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
        0x41, 0x5D, 0xC9, 0x00, 0x01, 0x24, 0x2E, 0x00, 
        0x02, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x01, 
        0x00, 0x00, 0x00, 0xD9, 0x1C, 0x04, 0x00, 0x00, 
        0x00, 0x01, 0x00, 0x00, 0x00, 0x60, 0x68, 0x9E, 
        0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00
    })

    Method (NBCI, 4, Serialized)
    {
        Store ("------- NV NBCI DSM --------", Debug)
        If (LNotEqual (Arg1, 0x0102))
        {
            Store (" NBCI DSM: NOT SUPPORTED!", Debug)
            Return (STATUS_ERROR_UNSUPPORTED)
        }
        //
        // Sub-Function 0: NBCI_FUNC_SUPPORT
        //
        If (LEqual (Arg2, NBCI_FUNC_SUPPORT))
        {
            Store ("   NBCI fun0 NBCI_FUNC_SUPPORT", Debug)
            Return (Buffer (0x04)
            {
                0x01, 0x00, 0x11, 0x00                // sub-func: 0,16,20 supported
            })
        }
        //
        // Sub-Function 16: NBCI_FUNC_GETOBJBYTYPE
        //
        If (LEqual (Arg2, NBCI_FUNC_GETOBJBYTYPE))
        {
            Store ("   NBCI fun16 NBCI_FUNC_GETOBJBYTYPE", Debug)
            CreateWordField (Arg3, 0x02, BFF0)
            If (LEqual (BFF0, 0x564B))                // Check VK object
            {
                // OEM_customize: VK Key is an object per panel/GPU/HostBridge. OEM SBIOS need to check corresponding info then pick correct VK object to return.
//                If (LEqual (DEID, 0x0D04))          // Example: VK for host bridge DID == 0x0D04
//                {
//                    Return (GSV1)
//                }
//                If (LEqual (DEID, 0x0C04))          // Example: VK for host bridge DID == 0x0C04 (if need)
//                {
//                    Return (GSV2)
//                }
            }
            If (LEqual (BFF0, 0x4452)) // Check DR object
            {
                // OEM_customize: DR Key is an data Object which might vary on different eDP panel. OEM SBIOS need to check corresponding info then pick correct DR object to return.
                Return (GSDR)
            }
        }
        //
        // Sub-Function 20: NBCI_FUNC_GETBACKLIGHT
        //
        If (LEqual (Arg2, NBCI_FUNC_GETBACKLIGHT))
        {
            Store ("   NBCI fun20 NBCI_FUNC_GETBACKLIGHT", Debug)
            // OEM_customize: Package returned in this interface is panel related. OEM SBIOS need to follow definition in NBSI Spec to reflect real panel feature in this pacakge.
            Return (Package (32)
            {
                0x8000A450, 0x0200, Zero, Zero, 0x05, One, 0xC8, 0x32, 0x03E8, 0x0B,
                0x32, 0x64, 0x96, 0xC8, 0x012C, 0x0190, 0x01FE, 0x0276, 0x02F8, 0x0366,
                0x03E8, Zero, 0x64, 0xC8, 0x012C, 0x0190, 0x01F4, 0x0258, 0x02BC,
                0x0320, 0x0384, 0x03E8
            })
        }
    }
}
