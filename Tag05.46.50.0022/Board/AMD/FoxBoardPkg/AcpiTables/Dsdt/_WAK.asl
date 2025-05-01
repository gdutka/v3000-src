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

#ifndef _WAK_
#define _WAK_

  Method(\_WAK, 1) {
    CpmDebugPrint ("PLA-ASL-\\_WAK Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
    CpmDebugPrint ("PLA-ASL-\\_WAK call SWAK\n", 0, 0, 0, 0, 0, 0)
    SWAK (Arg0)

    CpmDebugPrint ("PLA-ASL-\\_WAK call \\_SB.AWAK\n", 0, 0, 0, 0, 0, 0)
    \_SB.AWAK (Arg0)

    If (Lor(LEqual(ARG0, 0x03), LEqual(ARG0, 0x04))) {
      If (LNotEqual(\GPIC,0)) {
        CpmDebugPrint ("PLA-ASL-\\_WAK call \\_SB.DSPI\n", 0, 0, 0, 0, 0, 0)
        \_SB.DSPI()
        If (\NAPC) {
          CpmDebugPrint ("PLA-ASL-\\_WAK call \\_SB.PCI0.NAPE\n", 0, 0, 0, 0, 0, 0)
          \_SB.PCI0.NAPE()    // Clear D0F0xFC_x00[IoapicSbFeatureEn]
        }
      }
    }

    If (LEqual(ARG0, 0x03)) {
      \_SB.S80H(0xE3)
//[-start-220826-IB14740258-modify]//
      Notify (\_SB.SLPB, 0x2)
//[-end-220826-IB14740258-modify]//
    }

    If (LEqual(ARG0, 0x04)) {
      \_SB.S80H(0xE4)
//[-start-220826-IB14740258-modify]//
      Notify (\_SB.SLPB, 0x2)
//[-end-220826-IB14740258-modify]//
    }

    // EC EnableAcpi
//[-start-220217-IB14740213-remove]//
//    if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {
//      Store(\_SB.PCI0.LPC0.EC0.STAS, Local0)
//      Or(Local0, 0x04, Local1)
//      Store(Local1, \_SB.PCI0.LPC0.EC0.STAS)
//      Release(\_SB.PCI0.LPC0.EC0.Z009)
//    }
//[-end-220217-IB14740213-remove]//

//[-start-220217-IB14740213-remove]//
//    Store(0xFF, \_SB.ACAD.ACDC) //Update AC/DC status to SMU when wake.
//[-end-220217-IB14740213-remove]//

    CpmDebugPrint ("PLA-ASL-\\_WAK call MWAK\n", 0, 0, 0, 0, 0, 0)
    MWAK (Arg0)
    // TODO: To verify LID function after wake-up
    CpmDebugPrint ("PLA-ASL-\\_WAK End\n", 0, 0, 0, 0, 0, 0)
    return (0)
  }

#endif
