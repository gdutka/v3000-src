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
Name(PR12, Package() {
//
Package(){ 0x0000FFFF, 0, LNKG, 0 },
Package(){ 0x0000FFFF, 1, LNKH, 0 },
Package(){ 0x0000FFFF, 2, LNKE, 0 },
Package(){ 0x0000FFFF, 3, LNKF, 0 },
})
Name(AR12, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 22 },
Package(){ 0x0000FFFF, 1, 0, 23 },
Package(){ 0x0000FFFF, 2, 0, 20 },
Package(){ 0x0000FFFF, 3, 0, 21 },
})

Name(NR12, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 38 },
Package(){ 0x0000FFFF, 1, 0, 39 },
Package(){ 0x0000FFFF, 2, 0, 36 },
Package(){ 0x0000FFFF, 3, 0, 37 },
})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NR12)      // NB IOAPIC Enabled
    } Else {
      Return(AR12)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PR12)       // PIC mode
  }
}
