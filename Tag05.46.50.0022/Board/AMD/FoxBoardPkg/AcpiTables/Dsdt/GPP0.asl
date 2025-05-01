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

#ifndef _GPP0_
#define _GPP0_

// GPP0 (Bus 0 Dev 1 Fn 1)
Device(GPP0)
{

  Name(_ADR, 0x00010001)

  Method(MPRW,0,NotSerialized) {
    If (LEqual (WKPM, 0x01)) {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP0._PRW Return GPRW (0x8, 0x4)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0x8, 0x04))
    } Else {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP0._PRW Return GPRW (0x8, 0x0)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0x8, 0x00))
    }
  }

  #include "BusPRT/B0PRT.asl"
}

#endif
