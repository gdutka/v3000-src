/** @file

;*******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#ifndef _SLPB_
#define _SLPB_

Device (SLPB)
{
  Name (_HID, EISAID ("PNP0C0E"))
  Name (_STA, 0x0B)
  // Bit0 - the device is present: Yes.
  // Bit1 - the device is enabled and decoding its resources: Yes.
  // Bit2 - the device should be shown in the UI: No.
  // Bit3 - the device is functioning properly: Yes.
  // Bit4 - the battery is present: N/A
}

#endif
