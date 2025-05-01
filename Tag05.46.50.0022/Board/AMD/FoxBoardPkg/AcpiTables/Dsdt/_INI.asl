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

#ifndef _INI_
#define _INI_

  Method(_INI) {
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._INI Start\n", 0, 0, 0, 0, 0, 0)
    If (LNotEqual(GPIC,0)) {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._INI call \\_SB.DSPI\n", 0, 0, 0, 0, 0, 0)
      \_SB.DSPI()
      If (\NAPC) {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._INI call \\_SB.PCI0.NAPE\n", 0, 0, 0, 0, 0, 0)
        \_SB.PCI0.NAPE()    // Clear D0F0xFC_x00[IoapicSbFeatureEn]
      }
    }
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._INI call \\OSTP\n", 0, 0, 0, 0, 0, 0)
    \OSTP()
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._INI End\n", 0, 0, 0, 0, 0, 0)
  }

#endif
