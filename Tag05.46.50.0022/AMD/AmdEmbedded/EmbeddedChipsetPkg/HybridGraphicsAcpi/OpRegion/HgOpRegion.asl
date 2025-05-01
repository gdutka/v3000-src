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

Scope (PCI_SCOPE) {
    OperationRegion (HGOP, SystemMemory, 0x55AA55AA, 0x55AA55AA)
    Field (HGOP, AnyAcc, Lock, Preserve)
    {
        DGDA,  32,  // dGPU device's PCIe Base Address
        DGBA,  32,  // dGPU bridge's PCIe Base Address
        DGPV,  16,  // dGPU Vendor ID
        OPTF,   8,  // Optimus flag
        NVGE,   8,  // Nvidia dGPU Generation
        DSSV,  32,  // dGPU SSID SVID
        DISM,   8,  // DIS_MODE_FLAG: ACPI flag reflect to SBIOS option.        
    }
}
