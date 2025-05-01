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

#ifndef _GPP7_
#define _GPP7_

// GPP7 (Bus 0 Dev 2 Fn 3)
Device(GPP7)
{

  Name(_ADR, 0x00020003)

  Method(_PRW,0,NotSerialized) {
    If (LEqual (WKPM, 0x01)) {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP7._PRW Return GPRW (0x2, 0x4)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0x2, 0x04))
    } Else {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GPP7._PRW Return GPRW (0x2, 0x0)\n", 0, 0, 0, 0, 0, 0)
      Return (GPRW (0x2, 0x00))
    }
  }

  #include "BusPRT/B9PRT.asl"
  
  Device(DEV0)
  {
    Name(_ADR, 0x00)
    
//    Method (_RMV, 0, NotSerialized)
//    {
//      Return (1)
//    }
  }

}

#endif

