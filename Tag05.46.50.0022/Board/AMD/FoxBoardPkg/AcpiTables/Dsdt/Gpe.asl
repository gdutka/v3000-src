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

#ifndef _GPE_ASI_
#define _GPE_ASI_

Scope(\_GPE) {
  Method(XL08, 0x00, Notserialized) {   // PCIe
    TPST (Gpe08EventHandling)
    CpmDebugPrint ("PLA-ASL-\\_GPE._L08\n", 0, 0, 0, 0, 0, 0)
    if (LEqual (TBEN, 0)) {             // For Non-TB
      Notify (\_SB.PCI0.GPP0, 0x02)     // For Non-TB
      Notify (\_SB.PCI0.GPP1, 0x02)     // For Non-TB
    }                                   // For Non-TB
    Notify (\_SB.PCI0.GPP2, 0x02)
    Notify (\_SB.PCI0.GP18, 0x02)
  }

  Method(XL0D) {
    TPST (Gpe0DEventHandling)
    CpmDebugPrint ("PLA-ASL-\\_GPE._L0D\n", 0, 0, 0, 0, 0, 0)
    Notify (\_SB.PCI0.GPP2, 0x02)  //WWAN
  }

  Method(XL0E) {
    TPST (Gpe0EEventHandling)
    CpmDebugPrint ("PLA-ASL-\\_GPE._L0E\n", 0, 0, 0, 0, 0, 0)
    Notify (\_SB.PCI0.GPP6, 0x02)  //WLAN
  }

  Method(XL0F) {
    TPST (Gpe0FEventHandling)
    CpmDebugPrint ("PLA-ASL-\\_GPE._L0F\n", 0, 0, 0, 0, 0, 0)
    Notify (\_SB.PCI0.GPP5, 0x02)  //GIGA LAN
  }

  // Method(XL19) {                        // XHCI
  //   TPST (Gpe19EventHandling)
  //   CpmDebugPrint ("PLA-ASL-\\_GPE._L19\n", 0, 0, 0, 0, 0, 0)
  //   Notify (\_SB.PCI0.GP17, 0x02)
  //   Notify (\_SB.PCI0.GP17.XHC0,0x2)
  //   Notify (\_SB.PCI0.GP17.XHC1,0x2)
  // }

  // Method(XL1A) {
  //   TPST (Gpe1AEventHandling)
  //   CpmDebugPrint ("PLA-ASL-\\_GPE._L1A\n", 0, 0, 0, 0, 0, 0)
  //   Notify (\_SB.PCI0.GP19, 0x02)
  //   Notify (\_SB.PCI0.GP19.XHC2,0x2)
  //    Notify (\_SB.PCI0.GP19.NHI0,0x2)
  //    Notify (\_SB.PCI0.GP19.NHI1,0x2)
  //    Notify (\_SB.PCI0.GP19.XHC3,0x2)
  //    Notify (\_SB.PCI0.GP19.XHC4,0x2)
  // }

} //End of GPE

#endif
