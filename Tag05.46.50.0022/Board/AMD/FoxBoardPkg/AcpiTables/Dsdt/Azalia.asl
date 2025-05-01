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

#ifndef _AZAL_
#define _AZAL_

Device (ACP) {
  // Device 0, Function 5
  Name (_ADR, 0x00000005)

  // Method (_WOV, 0x0, NotSerialized)
  // {
  //   CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.ACP._WOV Return WOVS = 0x%X\n", WOVS, 0, 0, 0, 0, 0)
  //   Return(WOVS)
  // }

}

Device(AZAL) {
  // Device 0, Function 6
  Name(_ADR, 0x00000006)

  // TODO: Wake on voice support

}

Device(HDAU) {
  // Device 0, Function 1 Display HD Audio Controller
  Name(_ADR, 0x00000001)

}

#endif
