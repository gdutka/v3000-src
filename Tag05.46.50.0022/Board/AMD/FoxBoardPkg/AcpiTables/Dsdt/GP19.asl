/** @file

;*******************************************************************************
;* Copyright (c) 2013, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#ifndef _GP19_
#define _GP19_
External(\_SB.ALIB, MethodObj)

// GP19 (Bus 0 Dev 8 Fn 3)
Device(GP19)
{

  Name(_ADR, 0x00080003)
  Method (_DSM, 0x4, Serialized) {
    If (LEqual (Arg0, ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
      Switch (ToInteger(Arg2)) {
        // Function 0: Query function, return based on revision
       Case (0x00) {
          Return (Buffer() {0xFF})
        }
        // Function 05h: Ignoring PCI Boot Configurations
        // Arg3: Integer.
        // Return:
        //    0h - No (The operating system shall not ignore the PCI configuration
        //         that firmware has done at boot time. However, the operating system
        //         is free to configure the devices in this hierarchy that have not
        //         been configured by the firmware. There may be a reduced level of
        //         hot plug capability support in this hierarchy due to resource
        //         constraints. This situation is the same as the legacy situation
        //         where this _DSM is not provided.)
        //    1h - Yes (The operating system may ignore the PCI configuration that
        //         the firmware has done at boot time, and reconfigure/rebalance the
        //         resources in the hierarchy.)
        Case (0x05) {
          Return (0x0)
        }
        // Other function no support
        default {
          Return (0x00)
        }
      } // End of Switch (ToInteger(Arg2))
    } Else {
      Return (Buffer (One) { 0x00 })
    }
  }

  // Method(_S0W,0) {
  //   Return(0)
  // }

  // Method(NPRW,0,NotSerialized) {
  //   If (LEqual (WKPM, 0x01)) {
  //     CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP19._PRW Return GPRW (0x1A, 0x4)\n", 0, 0, 0, 0, 0, 0)
  //     Return (GPRW (0x1A, 0x04))
  //   } Else {
  //     CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP19._PRW Return GPRW (0x1A, 0x0)\n", 0, 0, 0, 0, 0, 0)
  //     Return (GPRW (0x1A, 0x00))
  //   }
  // }

  #include "BusPRT/B15PRT.asl"
  #include "xHci2.asl"

  #include "NHI0.asl"
  #include "xHci3.asl"
  #include "NHI1.asl"
  #include "xHci4.asl"
}

#endif

