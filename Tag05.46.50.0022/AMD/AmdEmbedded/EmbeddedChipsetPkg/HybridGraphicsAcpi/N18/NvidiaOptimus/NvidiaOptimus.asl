/** @file

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
#include <NvidiaOptimus.asi>

DefinitionBlock (
    "NvidiaOptimus.aml",
    "SSDT",
    1,
    "Insyde",
    "NvdTable",
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
    // External (CPU0_SCOPE, DeviceObj)
    // External (CPU0_SCOPE._PSS, MethodObj)
    // External (CPU0_SCOPE._PPC, MethodObj)
    // External (CPU1_SCOPE._PPC, MethodObj)
    // External (CPU2_SCOPE._PPC, MethodObj)
    // External (CPU3_SCOPE._PPC, MethodObj)
    External (PCI_SCOPE, DeviceObj)
    External (DGPU_BRIDGE_SCOPE, DeviceObj)
    // External (DGPU_BRIDGE_SCOPE.PEGP, DeviceObj)
    // External (DGPU_BRIDGE_SCOPE.PEGP.GPRF)
    // External (DGPU_BRIDGE_SCOPE.PEGP.NVOP, MethodObj)
    // External (DGPU_BRIDGE_SCOPE.PEGP.NBCI, MethodObj)
    // External (DGPU_BRIDGE_SCOPE._ADR)
    // External (DGPU_BRIDGE_SCOPE._ON, MethodObj)
    // External (DGPU_BRIDGE_SCOPE._OFF, MethodObj)
    External (DGPU_SCOPE, DeviceObj)
    // External (DGPU_SCOPE._ADR)
    // External (DGPU_SCOPE.DGPS)
    // External (DGPU_SCOPE.OMPR)
    // External (DGPU_SCOPE.GPRF)
    // External (DGPU_SCOPE.NVOP, MethodObj)
    External (DGPU_BRIDGE_SCOPE.VGA._ADR)
    External (DGPU_BRIDGE_SCOPE.HDAU._ADR)
    External (IGPU_SCOPE, DeviceObj)
    External (M000, MethodObj)
    External (M009, MethodObj)
    External (M010, MethodObj)
    External (M017, MethodObj)
    External (M019, MethodObj)
    External (M020, MethodObj)
//  For debug purpose
    External (MBGS, MethodObj)
    External (P8XH, MethodObj)
   
//  From GlbNvs.asl
//    External (\IGDS)

//  From GloblNvs.asl    
    External (\TCNT, FieldUnitObj)

Scope (DGPU_BRIDGE_SCOPE)
{
    Device (PEGP)
    {
        Name(_ADR, 0x00)
    }

	Device(NHDA)
	{
		Name(_ADR, One)
	}
}
    //
    // Include OpRegion.
    // NvidiaOpRegion.asl must be included at the end because the algorithm to
    // initialize each OpRegions will be ended once NOPR is found.
    // (The above algorithm is in HybridGraphicsDxe.c -> InitializeOpRegion())
    //
    #include <../OpRegion/HgOpRegion.asl>
    #include <../OpRegion/VbiosOpRegion.asl>
    #include <../OpRegion/NvidiaOpRegion.asl>

    Include ("../NvidiaCommon/NvNbci.asl")
    Include ("../NvidiaCommon/NvCommon.asl")
    Include ("../NvidiaCommon/NvOp.asl")
    Include ("../NvidiaCommon/NvGps.asl")
    Include ("../NvidiaCommon/NvGc6_v3.0.asl")
 #if FixedPcdGetBool (PcdHgNvidiaNpcfFeatureSupport)
     Include ("../NvidiaCommon/NvPcf.asl")
//    Include ("../NvidiaCommon/NVPcf_ITBM.asl")     
 #endif
 #if FixedPcdGetBool (PcdHgNvidiaVenturaFeatureSupport)
     Include ("NvSpb.asl")
 #endif
 #if FeaturePcdGet (PcdHgNvidiaDdsFeatureSupport)
     Include ("../NvidiaCommon/NvDds.asl")
 #endif
     Include ("Mxm.asl")
     Include ("NvHg.asl")
     Include ("NvHgDsm.asl")
}
