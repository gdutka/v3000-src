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

Name(PRB0, Package() {

// Device 1 Func 1:5 --GPP[1:5]
Package(){ 0x0001FFFF, 0, LNKA, 0 },
Package(){ 0x0001FFFF, 1, LNKB, 0 },
Package(){ 0x0001FFFF, 2, LNKC, 0 },
Package(){ 0x0001FFFF, 3, LNKD, 0 },
//[-start-220826-IB14740258-remove]//
//Package(){ 0x0001FFFF, 4, LNKA, 0 },
//[-end-220826-IB14740258-remove]//

// Device 2 Func 1:6 --GPP[1:6]
Package(){ 0x0002FFFF, 0, LNKE, 0 },
Package(){ 0x0002FFFF, 1, LNKF, 0 },
Package(){ 0x0002FFFF, 2, LNKG, 0 },
Package(){ 0x0002FFFF, 3, LNKH, 0 },
//[-start-220826-IB14740258-remove]//
//Package(){ 0x0002FFFF, 4, LNKE, 0 },
//Package(){ 0x0002FFFF, 5, LNKF, 0 },
//[-end-220826-IB14740258-remove]//

// Device 3 Func 1 --GPP[1]
Package(){ 0x0003FFFF, 0, LNKA, 0 },

// Device 4 Func 1 --GPP[1]
Package(){ 0x0004FFFF, 0, LNKA, 0 },

// Device 8 Func 1:3 --Internal GPP [1:3]
Package(){ 0x0008FFFF, 0, LNKA, 0 },
Package(){ 0x0008FFFF, 1, LNKB, 0 },
Package(){ 0x0008FFFF, 2, LNKC, 0 },

// FCH On-Chip
Package(){ 0x0014FFFF, 0, LNKA, 0 },
Package(){ 0x0014FFFF, 1, LNKB, 0 },
Package(){ 0x0014FFFF, 2, LNKC, 0 },
Package(){ 0x0014FFFF, 3, LNKD, 0 },

})

Name(ARB0, Package() {

// Device 1 Func 1:5 --GPP[1:5]
Package(){ 0x0001FFFF, 0, 0, 16 },
Package(){ 0x0001FFFF, 1, 0, 17 },
Package(){ 0x0001FFFF, 2, 0, 18 },
Package(){ 0x0001FFFF, 3, 0, 19 },
//[-start-220826-IB14740258-remove]//
//Package(){ 0x0001FFFF, 4, 0, 16 },
//[-end-220826-IB14740258-remove]//

// Device 2 Func 1:6 --GPP[1:6]
Package(){ 0x0002FFFF, 0, 0, 20 },
Package(){ 0x0002FFFF, 1, 0, 21 },
Package(){ 0x0002FFFF, 2, 0, 22 },
Package(){ 0x0002FFFF, 3, 0, 23 },
//[-start-220826-IB14740258-remove]//
//Package(){ 0x0002FFFF, 4, 0, 20 },
//Package(){ 0x0002FFFF, 5, 0, 21 },
//[-end-220826-IB14740258-remove]//

// Device 3 Func 1 --GPP[1]
Package(){ 0x0003FFFF, 0, 0, 16 },

// Device 4 Func 1 --GPP[1]
Package(){ 0x0004FFFF, 0, 0, 16 },


// Device 8 Func 1:3 --Internal GPP [1:3]
Package(){ 0x0008FFFF, 0, 0, 16 },
Package(){ 0x0008FFFF, 1, 0, 17 },
Package(){ 0x0008FFFF, 2, 0, 18 },

// FCH On-Chip
Package(){ 0x0014FFFF, 0, 0, 16 },
Package(){ 0x0014FFFF, 1, 0, 17 },
Package(){ 0x0014FFFF, 2, 0, 18 },
Package(){ 0x0014FFFF, 3, 0, 19 },

})

Name(NRB0, Package() {

// Device 1 Func 1:5 --GPP[1:5]
Package(){ 0x0001FFFF, 0, 0, 24 },
Package(){ 0x0001FFFF, 1, 0, 25 },
Package(){ 0x0001FFFF, 2, 0, 26 },
Package(){ 0x0001FFFF, 3, 0, 27 },
//[-start-220826-IB14740258-remove]//
//Package(){ 0x0001FFFF, 4, 0, 24 },
//[-end-220826-IB14740258-remove]//

// Device 2 Func 1:6 --GPP[1:6]
Package(){ 0x0002FFFF, 0, 0, 28 },
Package(){ 0x0002FFFF, 1, 0, 29 },
Package(){ 0x0002FFFF, 2, 0, 30 },
Package(){ 0x0002FFFF, 3, 0, 31 },
//[-start-220826-IB14740258-remove]//
//Package(){ 0x0002FFFF, 4, 0, 28 },
//Package(){ 0x0002FFFF, 5, 0, 29 },
//[-end-220826-IB14740258-remove]//

// Device 3 Func 1 --GPP[1]
Package(){ 0x0003FFFF, 0, 0, 40 },

// Device 4 Func 1 --GPP[1]
Package(){ 0x0004FFFF, 0, 0, 40 },

// Device 8 Func 1:3 --Internal GPP [1:3]
Package(){ 0x0008FFFF, 0, 0, 32 },
Package(){ 0x0008FFFF, 1, 0, 33 },
Package(){ 0x0008FFFF, 2, 0, 34 },

// FCH On-Chip
Package(){ 0x0014FFFF, 0, 0, 16 },
Package(){ 0x0014FFFF, 1, 0, 17 },
Package(){ 0x0014FFFF, 2, 0, 18 },
Package(){ 0x0014FFFF, 3, 0, 19 },

})

Method(_PRT, 0) {
  If(PICM) {
    If (\NAPC) {
      Return(NRB0)      // NB IOAPIC Enabled
    } Else {
      Return(ARB0)      // NB IOAPIC Disabled
    }
  } Else {
    Return (PRB0)       // PIC mode
  }
}
