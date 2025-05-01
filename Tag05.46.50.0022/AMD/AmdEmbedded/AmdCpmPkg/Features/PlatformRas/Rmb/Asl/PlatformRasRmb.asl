/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 * ***************************************************************************
 */

 DefinitionBlock (
    "PlatformRasRmb.aml",           // Output file
    "SSDT",                         // Signature
    0x02,                           // DSDT Revision
    "AMD   ",                       // OEM ID
    "CPMRAS",                       // OEM Table ID
    0x1                             // OEM Revision
    )
{
  #include "OscApei.asi"
}
