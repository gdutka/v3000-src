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
Name(PR13, Package() {
// Bus A; Device 0; Function 0; Internal GPU
// Bus A; Device 0; Function 1; Display HD Audio Controller
// Bus A; Device 0; Function 2; Cryptographic Coprocessor
// Bus A; Device 0; Function 3; USB 3.1
// Bus A; Device 0; Function 4; USB 3.1
// Bus A; Device 0; Function 5; Audio Processor
// Bus A; Device 0; Function 6; Audio Processor ¡V HD Audio Controller
// Bus A; Device 0; Function 7; Sensor Fusion Hub
Package(){ 0x0000FFFF, 0, LNKC, 0 },
Package(){ 0x0000FFFF, 1, LNKD, 0 },
Package(){ 0x0000FFFF, 2, LNKA, 0 },
Package(){ 0x0000FFFF, 3, LNKB, 0 },
})
Name(AR13, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 18 },
Package(){ 0x0000FFFF, 1, 0, 19 },
Package(){ 0x0000FFFF, 2, 0, 16 },
Package(){ 0x0000FFFF, 3, 0, 17 },
})

Name(NR13, Package() {
//
Package(){ 0x0000FFFF, 0, 0, 34 },
Package(){ 0x0000FFFF, 1, 0, 35 },
Package(){ 0x0000FFFF, 2, 0, 32 },
Package(){ 0x0000FFFF, 3, 0, 33 },
})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NR13)      // NB IOAPIC Enabled
    } Else {
      Return(AR13)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PR13)       // PIC mode
  }
}

