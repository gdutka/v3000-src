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

#ifndef _PTS_
#define _PTS_

#define UNLOCK_SPI_STATE                0x90
External(\_SB.TPM2.PTS, MethodObj)

Method(\_PTS, 1) {
  SPTS (Arg0)
  CpmDebugPrint ("PLA-ASL-\\_PTS Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  If (LEqual(ARG0, 0x01)) {
    \_SB.S80H(0x51)
  }
  If (LEqual(ARG0, 0x03)) {
    \_SB.S80H(0x53)
    Store(One,\SLPS)
    Store (0x40, Local1)  // EC STAS [7:6] = 1 for S3
  }
  If (LEqual(ARG0, 0x04)) {
    \_SB.S80H(0x54)
    Store(One,\SLPS)
    Store(One, RSTU)
    Store (0x80, Local1) // EC STAS [7:6] = 2 for S4
  }
  If (LEqual(ARG0, 0x05)) {
    \_SB.S80H(0x55)
    If (LEqual (WKPM, 0x01)) {
      Store(one, \PWDE)
    }
    //
    // Trigger SMI to clear SPI lock bits before reboot and shutdown.
    //
    Store(UNLOCK_SPI_STATE, BCMD)
    CpmDebugPrint ("PLA-ASL-\\_PTS call \\_SB.BSMI(0x0)\n", 0, 0, 0, 0, 0, 0)
    \_SB.BSMI(0)  // Trigger SMI
    CpmDebugPrint ("PLA-ASL-\\_PTS call \\_SB.GSMI(0x3)\n", 0, 0, 0, 0, 0, 0)
    \_SB.GSMI(0x03)
    Store (0xC0, Local1) // EC STAS [7:6] = 3 for S5
  }

//[-start-220217-IB14740213-remove]//
//  if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {
//    And(\_SB.PCI0.LPC0.EC0.STAS, 0x3F, Local0) // Get EC STAS to Local0
//#ifdef MODERN_STANDBY_SUPPORT
//    If (LEqual(CNSB, 0x01)) {
//      And(Local0, 0xFB, Local0)                // Disable EC AcpiEnable
//    }
//#endif
//    Or (Local0, Local1, Local0)                // Set field of EC sleep type
//    Store (Local0, \_SB.PCI0.LPC0.EC0.STAS)    // Set back STAS
//    Release(\_SB.PCI0.LPC0.EC0.Z009)
//  }
//[-end-220217-IB14740213-remove]//
  
  //
  // Check whether TPM Module support is included in the project or not
  //
  If(CondRefOf(\_SB.TPM2.PTS))
  {
    //
    // Call TPM PTS method
    //
    CpmDebugPrint ("PLA-ASL-\\_PTS call \\_SB.TPM2.PTS\n", 0, 0, 0, 0, 0, 0)
    \_SB.TPM2.PTS (Arg0)
  }

  CpmDebugPrint ("PLA-ASL-\\_PTS call \\_SB.APTS\n", 0, 0, 0, 0, 0, 0)
  \_SB.APTS (Arg0)

  CpmDebugPrint ("PLA-ASL-\\_PTS call MPTS\n", 0, 0, 0, 0, 0, 0)
  MPTS (Arg0)
  CpmDebugPrint ("PLA-ASL-\\_PTS End\n", 0, 0, 0, 0, 0, 0)
} //End of \_PTS

#endif
