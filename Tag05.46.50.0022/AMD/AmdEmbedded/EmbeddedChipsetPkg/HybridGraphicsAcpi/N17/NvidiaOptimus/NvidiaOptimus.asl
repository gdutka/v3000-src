/** @file

;******************************************************************************
;* Copyright (c) 2018 - 2019, Insyde Software Corp. All Rights Reserved.
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
    // External (DGPU_BRIDGE_SCOPE.LREN)
    // External (DGPU_BRIDGE_SCOPE.CEDR)
    // External (DGPU_BRIDGE_SCOPE.CMDR)
    // External (DGPU_BRIDGE_SCOPE.D0ST)
    // External (DGPU_BRIDGE_SCOPE._ON, MethodObj)
    // External (DGPU_BRIDGE_SCOPE._OFF, MethodObj)
    External (DGPU_SCOPE, DeviceObj)
    // External (DGPU_SCOPE._ADR)
    // External (DGPU_SCOPE.DGPS)
    // External (DGPU_SCOPE.OMPR)
    // External (DGPU_SCOPE.GPRF)
    // External (DGPU_SCOPE.LTRE)
    // External (DGPU_SCOPE.NVOP, MethodObj)
    // External (DGPU_SCOPE.NBCI, MethodObj)
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

Scope (DGPU_BRIDGE_SCOPE)
{
    Device (PEGP)
    {
        Name(_ADR, 0x00)
    
        Method(_RMV,0,NotSerialized) {
          Return(0x00)
        }  

    }

	  Device(NHDA)
	  {
	  	Name(_ADR, One)
	  	Method(_RMV, 0, NotSerialized)
	  	{
	  		Return(Zero)
	  	}
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

    Include ("Optimus.asl")
    Include ("NvHg.asl")
    Include ("NvHgDsm.asl")
    // not supported (so far)
    // Include ("NvGps.asl")
    // not supported GC6
    // Include ("NvGc6_v2.x.asl")
    // Include ("NvSpb.asl")
}
