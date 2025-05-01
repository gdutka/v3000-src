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

/*
    == Readme ==
    For NVOP support, SBIOS has to adjust below code to meet platform:
    - Modify the method path of \_SB.PCI0.PGON and \_SB.PCI0.PGOF to Intel RC dGPU ON/OFF method
    - Modify enable the GPI event for illustrating the GPE event from HPD to notify 0x81
      to dGPU for HPD handling when dGPU is Off. 
*/

// IBV should be to follow the below defines to support HPD notification.
// Example:
// HDMI or DP HotPlug evnet GPIO pin
//   1. Connect to PCH
//     GPI event number: _L61
//     #define HPD_L_EVENT_NUMBER _L61
//     #define EVENT_DEVICE_PATH \_GPE
//
//   2. Connect to EC
//     GPI evennt enumber: Q21
//     #define HPD_L_EVENT_NUMBER Q21
//     #define EVENT_DEVICE_PATH \_SB.PCI0.SBRG.EC0
//
// DGPU device path: \_SB.PCI0.PEG0.PEGP
//   #define DGPU_DEVICE_PATH \_SB.PCI0.PEG0.PEGP
// 
// In AmdCpmOemTable.c, AMD_CPM_DXIO_TOPOLOGY_TABLE Entry0, Hotplug value is DxioHotplugDisabled.
//     GPI event number: XL08
//     #define HPD_L_EVENT_NUMBER XL08
//     #define EVENT_DEVICE_PATH \_GPE
// 
//    External (EVENT_DEVICE_PATH, DeviceObj)
//    Scope (EVENT_DEVICE_PATH)
//    {
//        Method (HPD_L_EVENT_NUMBER, 0, NotSerialized)  // _Lxx: Level-Triggered GPE
//        {
//            Notify (DGPU_DEVICE_PATH, 0x81) // Information Change
//        }
//    }

Scope (DGPU_SCOPE)
{
    Method (NVOP, 4, Serialized)
    {
        If (LEqual (Arg2, Zero))
        {
            Return (Buffer (0x04)
            {
                0x01, 0x00, 0x00, 0x04
            })
        }
        Else
        {
            If (LEqual (Arg2, 0x1A))
            {
                CreateField (Arg3, 0x18, 0x02, OMPR)
                CreateField (Arg3, Zero, One, FLCH)
                CreateField (Arg3, One, One, DVSR)
                CreateField (Arg3, 0x02, One, DVSC)
                If (ToInteger (FLCH))
                {
                    Store (OMPR, \_SB.PCI0.GPP0.PEGP.OPCE)
                }

                // Definition of return buffer.
                // bit 0     - Optimus Enabled
                //             0 : Optimus Graphics Disabled
                //             1 : Optimus Graphics Enabled (default)
                // bit 4:3   - Current GPU Control Status
                //             0 : GPU is powered off
                //             3 : GPU power has stabilized (default)
                // bit 6     - Shared discrete GPU Hot-Plug Capabilities
                //             1 : There are discrete GPU Display Hot-Plug signals co-connected to the platform
                // bit 8     - PCIe Configuration Space Owner Actual
                //             0 : SBIOS
                //             1 : GPU Driver
                // bit 26:24 - Optimus Capabilities
                //             0 : No special platform capabilities
                //             1 : Platform has dynamic GPU power control
                // bit 27:28 - Optimus HD Audio Codec Capabilities
                //             0 : No audio codec-related capabilities
                //             1 : Platform does not use HD audio
                //             2 : Platform supports Optimus dynamic codec control
                Store (Buffer(4) {0, 0, 0, 0}, Local0)
                CreateField (Local0,  0, 1, OPEN)
                CreateField (Local0,  3, 2, CGCS)
                CreateField (Local0,  6, 1, SHPC) // Shared discrete GPU Hot-Plug Capabilities
                CreateField (Local0,  8, 1, SNSR) // Modify Optimus DSM 0x1A for GC6 TDR support.
                CreateField (Local0, 24, 3, DGPC) // Optimus Power Capabilities
                CreateField (Local0, 27, 2, OHAC) // Optimus HD Audio Codec Capabilities

                Store (One, OPEN) // Optimus Enabled
                Store (One, SHPC) // Set '1' indicates there are discrete GPU Display Hot-Plug signals co-connected to the platform
                Store (One, DGPC) // Optimus Power Capabilities
                                  // 0: No special platform capabilities
                                  // 1: Dynamic GPU Power Control
                Store (0x3, OHAC) // Optimus HD Audio Codec Capabilities
                                  // 0: No audio codec-related capabilities
                                  // 1: Platform does not use HD audio (SBIOS will always disable audio codecs)
                                  // 2: Optimus dynamic codec control
                                  // 3: Dynamic power state reporting

                If (ToInteger (DVSC))
                {
                    If (ToInteger (DVSR))
                    {
                        Store (One, \_SB.PCI0.GPP0.PEGP.GPRF)
                    }
                    Else
                    {
                        Store (Zero, \_SB.PCI0.GPP0.PEGP.GPRF)
                    }
                }
                Store (\_SB.PCI0.GPP0.PEGP.GPRF, SNSR)
                If (LNotEqual (\_SB.PCI0.GPP0.PEGP.SGST(), Zero))
                {
                    Store (0x03, CGCS)
                }
                Return (Local0)
            }
        }
        Return (0x80000002)
    }
}