/** @file
;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

Scope (DGPU_SCOPE) {
    OperationRegion (NOPR, SystemMemory, 0x55AA55AA, 0x55AA55AA)
    Field (NOPR, AnyAcc, Lock, Preserve)
    {
        DHPS,       8,  // Optimus dGPU HotPlug Support
        DPCS,       8,  // Optimus dGPU Power Control Support
        GPSS,       8,  // GPS Feature Support
        VENS,       8,  // Ventura Feature Support
        NBCS,       8,  // Optimus NBCI Feature Support
        GC6S,       8,  // Optimus GC6 Feature Support
        NVSR,       8,  // Optimus GC6 NVSR Feature Support
        NPCS,       8,  // Optimus NPCF Feature Support
        NCTP,       8,  // N20 CTGP Feature Support        
        SLVS,       8,  // Slave dGPU Support
        PBCM,       8,  // SpbConfig
        EXMD,       8,  // Expansion Mode
        MXBS,      32,  // MXM bin file Size
        MXMB,   32768,  // 0x1000 bytes in bits
        SMXS,      32,  // Slave MXM bin file Size
        SMXB,   32768,  // Slave MXM binary 0x1000 bytes in bits
        FBEN,      32,  // Gc6 FB_EN
        ENVT,      32,  // Gc6 Evnet
        PWGD,      32,  // Power Good pin
        DMMP,      32,  // MUX_MODE_PIN: PCH GPIO for display switch MUX
        DLRP,      32,  // LCD_FORCE_RESET_PIN: PCH GPIO for LCD force reset pin        
    }
}
