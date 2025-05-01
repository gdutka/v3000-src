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

#ifndef _GP18_
#define _GP18_

// GP18 (Bus 0 Dev 8 Fn 2)
Device(GP18)
{

  Name(_ADR, 0x00080002)

  #include "BusPRT/B14PRT.asl"
  #include "Sata.asl"
}

#endif

