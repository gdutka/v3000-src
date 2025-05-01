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

#ifndef _GPP5_
#define _GPP5_

// GPP5 (Bus 0 Dev 2 Fn 1)
Device(GPP5)
{

  Name(_ADR, 0x00020001)

  Method(_PRW,0,NotSerialized) {
    If (LEqual (WKPM, 0x01)) {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP5._PRW Return GPRW (0xF, 0x4)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0xF, 0x04))
    } Else {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP5._PRW Return GPRW (0xF, 0x0)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0xF, 0x00))
    }
  }

  #include "BusPRT/B7PRT.asl"

  Device(RTL8) {
    Name(_ADR, 0x00)
  }

  Device(RUSB) {
    Name(_ADR, 0x04)
  }

}

#endif

