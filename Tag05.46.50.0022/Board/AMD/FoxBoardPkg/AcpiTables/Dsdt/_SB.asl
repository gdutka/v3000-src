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

#ifndef _SB_
#define _SB_

Scope(\_SB)                     // Start \_SB scope
{
  //
  // Power Button
  //
  #include "PowerButton.asl"

//[-start-220826-IB14740258-add]//
  //
  // Sleep Button
  //
  #include "SleepButton.asl"
//[-end-220826-IB14740258-add]//

  //
  // PCI Host Bridge(PCI0)
  //
  #include "PCI0.asl"

  //
  // _LNKx method
  //
  #include "_LNKx.asl"

  //
  // General SoftWare Smi
  //
  #include "GSMI.asl"

//[-start-220217-IB14740213-remove]//
//  #include "Sio\MEC1701\Ac.asl"
//  #include "Sio\MEC1701\Battery.asl"
//  #include "Lid.asl"
//[-end-220217-IB14740213-remove]//

} // End \_SB scope

#endif
