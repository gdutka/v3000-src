/** @file

;******************************************************************************
;* Copyright (c) 2013, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

//
// uPEP hook to EC
//   Arg0 - hook point id
//          2-5 for function 2, 3, 4 and 5 respectively
//
Method (UPHK, 1, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.UPHK Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  And ( Arg0, 0x000F, Arg0 )
  ECPC( Or (0x000000F0, Arg0) ) // PC EC0000Fx where x is Fun ID.

  If(LAnd(LGreaterEqual(Arg0, 2), LLessEqual(Arg0, 5))) {
    Store(Arg0, SYSK)      // Notift EC which function hook is executing

    switch(Arg0) {
      case(3) {            // Display Off

      }
      case(4) {            // Display On
         KBRS()            // do KB restore
      }
      default {

      }
    }
  }
}
