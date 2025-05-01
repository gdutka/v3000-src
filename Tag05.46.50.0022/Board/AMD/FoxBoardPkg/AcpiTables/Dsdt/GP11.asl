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

#ifndef _GP11_
#define _GP11_

// GP11 (Bus 0 Dev 3 Fn 1)
Device(GP11)
{

  Name(_ADR, 0x00030001)

  #include "BusPRT/B5PRT.asl"

}

#endif

