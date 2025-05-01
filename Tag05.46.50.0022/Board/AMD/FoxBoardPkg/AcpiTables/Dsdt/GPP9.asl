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

#ifndef _GPP9_
#define _GPP9_

// GPP9 (Bus 0 Dev 2 Fn 5)
Device(GPP9)
{

  Name(_ADR, 0x00020005)

  #include "BusPRT/B11PRT.asl"

}

#endif

