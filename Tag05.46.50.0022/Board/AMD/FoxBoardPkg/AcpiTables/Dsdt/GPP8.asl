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

#ifndef _GPP8_
#define _GPP8_

// GPP8 (Bus 0 Dev 2 Fn 4)
Device(GPP8)
{

  Name(_ADR, 0x00020004)

  #include "BusPRT/B10PRT.asl"

}

#endif

