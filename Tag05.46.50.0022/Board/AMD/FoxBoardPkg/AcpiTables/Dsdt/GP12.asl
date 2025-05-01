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

#ifndef _GP12_
#define _GP12_

// GP12 (Bus 0 Dev 4 Fn 1)
Device(GP12)
{

  Name(_ADR, 0x00040001)

  #include "BusPRT/B6PRT.asl"

}

#endif

