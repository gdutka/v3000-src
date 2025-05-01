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
Name(PR10, Package() {
//
Package(){ 0x0000FFFF, 0, LNKG, 0 },
Package(){ 0x0000FFFF, 1, LNKH, 0 },
Package(){ 0x0000FFFF, 2, LNKE, 0 },
Package(){ 0x0000FFFF, 3, LNKF, 0 },
})
Name(AR10, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 22 },
Package(){ 0x0000FFFF, 1, 0, 23 },
Package(){ 0x0000FFFF, 2, 0, 20 },
Package(){ 0x0000FFFF, 3, 0, 21 },
})

Name(NR10, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 46 },
Package(){ 0x0000FFFF, 1, 0, 47 },
Package(){ 0x0000FFFF, 2, 0, 44 },
Package(){ 0x0000FFFF, 3, 0, 45 },
})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NR10)      // NB IOAPIC Enabled
    } Else {
      Return(AR10)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PR10)       // PIC mode
  }
}
