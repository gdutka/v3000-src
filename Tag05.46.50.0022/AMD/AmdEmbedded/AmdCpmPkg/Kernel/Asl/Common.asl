/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

DefinitionBlock (
    "Common.aml",                   // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMCMN",                       // OEM Table ID
    0x1                             // OEM Revision
    )
{
    #define CPM_COMMON_ASL

    //
    // ALIB SMN Mutex
    //
    #define AlibSmnMutex            AM00
    External (\_SB.AlibSmnMutex, MutexObj)
    External (\_SB.ALIB, MethodObj)

    #include "CmnDef.asi"
    #include "CmnExt.asi"
    #include "PostCode.asi"
    #include "NvData.asi"
    #include "Function.asi"
    #include "PcieAccess.asi"
    #include "MemoryAccess.asi"
    #include "GpioAccess.asi"
    #include "SmbusAccess.asi"
    #include "I2CAccess.asi"
    #include "ScopeSbGpio.asi"
    #include "ScopeGpe.asi"
    #include "ScopeSb.asi"

}

