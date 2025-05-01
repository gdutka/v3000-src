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
Name(PR15, Package() {
// Bus C; Device 0; Function 0; WLAN
// Bus C; Device 0; Function 1; Bluetooth
// Bus C; Device 0; Function 2; I2S/AC'97 Audio
Package(){ 0x0000FFFF, 0, LNKC, 0 },
Package(){ 0x0000FFFF, 1, LNKD, 0 },
Package(){ 0x0000FFFF, 2, LNKA, 0 },
Package(){ 0x0000FFFF, 3, LNKB, 0 },
})

Name(AR15, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 18 },
Package(){ 0x0000FFFF, 1, 0, 19 },
Package(){ 0x0000FFFF, 2, 0, 16 },
Package(){ 0x0000FFFF, 3, 0, 17 },
})

Name(NR15, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 26 },
Package(){ 0x0000FFFF, 1, 0, 27 },
Package(){ 0x0000FFFF, 2, 0, 24 },
Package(){ 0x0000FFFF, 3, 0, 25 },
})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NR15)      // NB IOAPIC Enabled
    } Else {
      Return(AR15)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PR15)       // PIC mode
  }
}

