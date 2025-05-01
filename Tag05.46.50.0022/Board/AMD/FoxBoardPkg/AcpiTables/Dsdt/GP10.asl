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

#ifndef _GP10_
#define _GP10_

// GPP10 (Bus 0 Dev 2 Fn 6)
Device(GP10)
{

  Name(_ADR, 0x00020006)

  #include "BusPRT/B12PRT.asl"

}

#endif

