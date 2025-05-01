/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "AtpxAtifDGpu1.aml",            // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CDFAADG1",                     // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_DISPLAY_FEATURE_DGPU  1

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"

    External(\_SB.PCI0.PB2.VGA, DeviceObj)
    External(\_SB.ALIB, MethodObj)
    External(\_SB.LID._LID, MethodObj)

    Scope(\_SB) {
        Name (MACO, 0)                                   // MACO Enable Flag
    }

    Scope(\_SB.PCI0.PB2.VGA) {
        #include "../Common/ATPX.asi"
        #include "../Common/ATIF.asi"
    }
}
