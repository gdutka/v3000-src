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

#ifndef _GPP6_
#define _GPP6_

// GPP6 (Bus 0 Dev 2 Fn 2)
Device(GPP6)
{

  Name(_ADR, 0x00020002)

  Method(_PRW,0,NotSerialized) {
    If (LEqual (WKPM, 0x01)) {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6._PRW Return GPRW (0xE, 0x4)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0xE, 0x04))
    } Else {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP6._PRW Return GPRW (0xE, 0x0)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0xE, 0x00))
    }
  }

  #include "BusPRT/B8PRT.asl"
  #include "BlueTooth.asl"
}

#endif

