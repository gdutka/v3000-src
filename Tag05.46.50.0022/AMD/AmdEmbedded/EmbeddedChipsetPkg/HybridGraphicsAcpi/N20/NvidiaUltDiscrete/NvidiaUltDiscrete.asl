/** @file
  nVIDIA discrete only mode SSDT sample.

;******************************************************************************
;* Copyright (c) 2018 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <../NvidiaCommon/NvidiaCommon.asi>
#include <NvidiaUltDiscrete.asi>

DefinitionBlock (
    "NvidiaUltDiscrete.aml",
    "SSDT",
    1,
    "Insyde",
    "NvdUltTb",
    0x1000
    )
{
    External (CPU0_SCOPE, DeviceObj)
    External (CPU1_SCOPE, DeviceObj)
    External (CPU2_SCOPE, DeviceObj)
    External (CPU3_SCOPE, DeviceObj)
    External (CPU4_SCOPE, DeviceObj)
    External (CPU5_SCOPE, DeviceObj)
    External (CPU6_SCOPE, DeviceObj)
    External (CPU7_SCOPE, DeviceObj)
    External (CPU8_SCOPE, DeviceObj)
    External (CPU9_SCOPE, DeviceObj)
    External (CPU10_SCOPE, DeviceObj)
    External (CPU11_SCOPE, DeviceObj)
    External (CPU12_SCOPE, DeviceObj)
    External (CPU13_SCOPE, DeviceObj)
    External (CPU14_SCOPE, DeviceObj)
    External (CPU15_SCOPE, DeviceObj)
    External (CPU16_SCOPE, DeviceObj)
    External (CPU17_SCOPE, DeviceObj)
    External (CPU18_SCOPE, DeviceObj)
    External (CPU19_SCOPE, DeviceObj)
    External (CPU20_SCOPE, DeviceObj)
    External (CPU21_SCOPE, DeviceObj)
    External (CPU22_SCOPE, DeviceObj)
    External (CPU23_SCOPE, DeviceObj)    
    External (DGPU_BRIDGE_SCOPE, DeviceObj)
    External (DGPU_BRIDGE_SCOPE.DL23, MethodObj)
    External (DGPU_BRIDGE_SCOPE.L23D, MethodObj)
    External (DGPU2_BRIDGE_SCOPE, DeviceObj)
    External (DGPU_SCOPE, DeviceObj)
    External (IGPU_SCOPE.SPB, MethodObj)
    External (PCI_SCOPE, DeviceObj)
    External (PCI_SCOPE.HGOF, MethodObj)
    External (PCI_SCOPE.HGON, MethodObj)
    External (PCI_SCOPE.HGPO, MethodObj)
    External (PCI_SCOPE.LKDN, FieldUnitObj)
    External (PCI_SCOPE.LKSN, FieldUnitObj)
    External (PCI_SCOPE.PGOF, MethodObj)
    External (PCI_SCOPE.PGON, MethodObj)
    External (PCI_SCOPE.SCBN, FieldUnitObj)
    External (PCRR, MethodObj)
    External (PCRW, MethodObj)
//    External (\_GPE.ITBH, MethodObj)    // Interrupt handler for Intel Turbo Boost Max Technology 3.0

//  For debug purpose
    External (MBGS, MethodObj)
    External (P8XH, MethodObj)

//  From GpioLib.asl
    External (\_SB.CAGS, MethodObj)
    External (\_SB.GGIV, MethodObj)
    External (\_SB.GPC0, MethodObj)
    External (\_SB.SGOV, MethodObj)
    External (\_SB.SHPO, MethodObj)
    External (\_SB.SPC0, MethodObj)

//  From CpuNvs.asl
//    External (\_SB.ITBM, IntObj)    // Enable/Disable Intel Turbo Boost Max Technology 3.0.

//  From CheckOS.asl
    External (OSTB) // Note: According to the OS type to change the base brightness levels, Win8 = 10, Win7 = 7.
    External (\TCNT, FieldUnitObj)
    
//  From SaNvs.asl
    External (\BRTL)
    External (\DID1)
    External (\DID2)
    External (\DID4)
    External (\DLHR)
    External (\DLPW)
    External (\HRA0)
    External (\HRE0)
    External (\HRG0)
    External (\IGDS)
    External (\PWA0)
    External (\PWE0)
    External (\PWG0)

    //
    // Include OpRegion.
    // NvidiaOpRegion.asl must be included at the end because the algorithm to
    // initialize each OpRegions will be ended once NOPR is found.
    // (The above algorithm is in HybridGraphicsDxe.c -> InitializeOpRegion())
    //
    #include <../OpRegion/HgOpRegion.asl>
    #include <../OpRegion/VbiosOpRegion.asl>
    #include <../OpRegion/NvidiaOpRegion.asl>

    Include ("../NvidiaCommon/NvUltCommon.asl")
    Include ("../NvidiaCommon/NvOp.asl")
    // Include ("../NvidiaCommon/NvGps.asl")
    // Include ("../NvidiaCommon/NvUltGc6_v3.0.asl")
    Include ("../NvidiaCommon/NvNbci.asl")
#if FixedPcdGetBool (PcdHgNvidiaNpcfFeatureSupport)
    Include ("../NvidiaCommon/NvPcf.asl")
//    Include ("../NvidiaCommon/NVPCF_CML_ITBM.asl")
//    Include ("../NvidiaCommon/NVPCF_2_ECIO_ITBM.asl")  
#endif
#if FeaturePcdGet (PcdHgNvidiaDdsFeatureSupport)
    Include ("../NvidiaCommon/NvDds.asl")
#endif
    Include ("Mxm.asl")
//    Include ("NvDgpu2.asl")
    Include ("NvPort.asl")
    Include ("NvWmi.asl")
}
