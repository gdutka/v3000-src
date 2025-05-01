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
Name(PR14, Package() {
// Bus B; Device 0; Function 0; SATA
// Bus B; Device 0; Function 1; 10Gbe Controller Port 1
// Bus B; Device 0; Function 2; 10Gbe Controller Port 2
Package(){ 0x0000FFFF, 0, LNKG, 0 },
Package(){ 0x0000FFFF, 1, LNKH, 0 },
Package(){ 0x0000FFFF, 2, LNKE, 0 },
Package(){ 0x0000FFFF, 3, LNKF, 0 },
})

Name(AR14, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 22 },
Package(){ 0x0000FFFF, 1, 0, 23 },
Package(){ 0x0000FFFF, 2, 0, 20 },
Package(){ 0x0000FFFF, 3, 0, 21 },
})

Name(NR14, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 30 },
Package(){ 0x0000FFFF, 1, 0, 31 },
Package(){ 0x0000FFFF, 2, 0, 28 },
Package(){ 0x0000FFFF, 3, 0, 29 },
})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NR14)      // NB IOAPIC Enabled
    } Else {
      Return(AR14)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PR14)       // PIC mode
  }
}

