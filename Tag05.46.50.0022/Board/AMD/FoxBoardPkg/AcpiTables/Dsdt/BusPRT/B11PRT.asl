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
Name(PR11, Package() {
//
Package(){ 0x0000FFFF, 0, LNKC, 0 },
Package(){ 0x0000FFFF, 1, LNKD, 0 },
Package(){ 0x0000FFFF, 2, LNKA, 0 },
Package(){ 0x0000FFFF, 3, LNKB, 0 },
})
Name(AR11, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 18 },
Package(){ 0x0000FFFF, 1, 0, 19 },
Package(){ 0x0000FFFF, 2, 0, 16 },
Package(){ 0x0000FFFF, 3, 0, 17 },
})

Name(NR11, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 42 },
Package(){ 0x0000FFFF, 1, 0, 43 },
Package(){ 0x0000FFFF, 2, 0, 40 },
Package(){ 0x0000FFFF, 3, 0, 41 },
})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NR11)      // NB IOAPIC Enabled
    } Else {
      Return(AR11)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PR11)       // PIC mode
  }
}
