/** @file

;******************************************************************************
;* Copyright (c) 2013, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

OperationRegion(ESPM, SystemMemory, 0xFEC20000, 0x100)
Field(ESPM, DwordAcc, NoLock, Preserve) {
             // dis_espi_masctl_reg_wr bit can protect below eSPI register from x86 access
             //   0x30~0x37, 0x40~0x73, 0x80~0x9B and 0xA4~0xCB
             //   0x00~0x0C (ongoing)
  ES00, 32,  // Restrict access from x86 for security reason
  ES04, 32,  // Restrict access from x86 for security reason

  Offset (0x68),
  ES68, 32,  // Restrict access from x86 for security reason

  Offset (0x9C),
  ES9C, 32,
}

OperationRegion(PMES, SystemMemory, 0xFED80340, 4)
Field(PMES, DwordAcc, NoLock, Preserve) {
      ,  1,
  EMI1,  1,   // VW IRQ#1 mask
      , 30,
}

OperationRegion (KBCS, SystemIO, 0x60, 5)
Field (KBCS, ByteAcc, NoLock, Preserve) {
  KB60, 8,
  Offset (0x04),
  KB64, 8,
}

OperationRegion (SC64, SystemIO, 0x64, 1)
Field (SC64, ByteAcc, NoLock, Preserve) {
  KOBF, 1,
  KIBF, 1,
}

//
// 8042 KBC restore flow
//
Method (KBRS, 0, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.KBRS Start\n", 0, 0, 0, 0, 0, 0)
  Store(KBFG, Local0)  // read KB restore flag from EC
  ECPC(Or(0x00F10000, Local0)) // PC 0xECF100xx

  //
  // Which subroutines will be executed are fixed for KBC wake vs. none KBC wake cases
  //
  //   KBC wake     := it is triggered by _Q70, executes ENKB + SIQ1(1) (i.e. KBFG is 0x22)
  //                   CRB EC is able to detect this scenario and fires _Q70 when
  //                     a) SoC in S0i3, or 
  //                     b) SoC is waken up from S0i3 for events that don't have to turn on screen (for example battery event)
  //                := do nothing when display on
  //   None KBC wake:= No Q-Events, but call ENKB + EMBF (i.e. KBFG is 0x42) when display turning on from uPEP hook.
  //

  If(LNotEqual(Local0, 0xFF)) {
    If(And(Local0, 0x01)) {
      ENVW()           // Currently this routine is doing by ABL
    }
    If(And(Local0, 0x02)) {
      ENKB()
    }
    If(And(Local0, 0x04)) {
      WKB4(0x00)       // Put a dummy data to Iox64. It results IRQ#1 assert
    }
    If(And(Local0, 0x08)) {
      SIQ1(1)
    }
    If(And(Local0, 0x10)) {
      SIQ1(0)
    }
    If(And(Local0, 0x20)) {
      SIQ1(1)
    }
    If(And(Local0, 0x40)) {
      EMBF(100)
    }
  }

  ECPC(Or(0x00E10000, Local0)) // PC 0xECE100xx
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.KBRS End\n", 0, 0, 0, 0, 0, 0)
}

//
// Enable VW channel
//
Method (ENVW, 0, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.ENVW Start\n", 0, 0, 0, 0, 0, 0)
  ECPC(0x00F20000)       // PC 0xECF20000

  //
  // Check if ES00, ES04 and ES68 are accessable
  //
  Store (ES00, Local0)
  If(LEqual(Local0, 0xFFFFFFFF)) {
    Store (ES04, Local0)
    If(LEqual(Local0, 0xFFFFFFFF)) {
       Store (ES68, Local0)
         If(LEqual(Local0, 0xFFFFFFFF)) {
         ECPC(0x00E2FFFF)       // PC 0xECE2FFFF
         CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.ENVW ES00, ES0C and ES68 access deny\n", 0, 0, 0, 0, 0, 0)
         return
       }
    }
  }

  // Read VW cfg word by issuing GET_CONFIGURATION 0x20
  Store (0x00000000, ES04)
  Store (0x00200009, ES00)

  // Wait for command done (at max 200 loops)
  Store (ES00, Local0)
  Store (200,  Local2)
  While (LAnd (Local2, And(Local0, 0x00000008))) {
    Sleep(1)
    Store(ES00, Local0)
    Decrement(Local2)
  }

  // just for debug
  ECPC(Local2);
  ECPC(Local0);

  // Set VW enable bit of 0x20 cfg word
  Store (ES04, Local0)
  Or(Local0, 0x00000001, Local1)
  // Do the write only if it was not enabled
  If(LNotEqual(Local0, Local1)) {
    Store (Local1, ES04)
    Store (0x00200008, ES00)

    // Wait for command done (at max 200 loops)
    Store (ES00, Local0)
    Store (200,  Local2)
    While (LAnd (Local2, And(Local0, 0x00000008))) {
      Sleep(1)
      Store(ES00, Local0)
      Decrement(Local2)
    }
  }

  // Set VW_EN (eSPIx68 bit 2) as well
  Store (ES68, Local0)

  // postcode ECA2HHHH and ECB2LLLL
  ShiftRight(Local0, 16, Local3)
  ECPC (Or(Local3, 0x00A20000))
  And(Local0, 0x0000FFFF, Local3)
  ECPC (Or(Local3, 0x00B20000))

  // Write ES68 only if the bit is not set
  Or (Local0, 0x00000004, Local1)
  If (LNotEqual(Local0, Local1)) {
    Store (Local1, ES68)
  }

  ECPC(0x00E20000)       // PC 0xECE20000
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.ENVW End\n", 0, 0, 0, 0, 0, 0)
}

//
// Read eSPI irq line status
//   Arg0 - irq line number
//
Method (RIQS, 1, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.RIQS Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  And(Arg0, 0x0000001F, Arg0)

  Store(ES9C, Local0)
  And(Local0, 0xFFFFFFE0, Local1)
  Or (Local1, Arg0, Local1)

  // Write ES9C only if the line was not selected
  If (LNotEqual(Local0, Local1)) {
    Store(Local1, ES9C)
    Store(ES9C, Local0)

    // one more trial
    If (LEqual(Local0, Local1)) {
      Store(Local1, ES9C)
      Store(ES9C, Local0)
    }
  }

  And(Local0, 0x000000E0, Local1)
  ShiftRight(Local1, 5, Local1)// Local1 holds the return value

//  ShiftLeft(Arg0, 8, Arg0)
//  Or(Arg0, Local1, Local0)
//  ECPC(Or(0x00F30000, Local0)) // PC 0xECE30x0y where x is IRQ# and y is status

  //
  // Return value:
  //   0 - IRQ keep 0 unchanged
  //   1 - IRQ keep 1 unchanged
  //   2 - IRQ changed from 1 to 0
  //   3 - IRQ changed from 0 to 1
  //
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.RIQS End Return 0x%X\n", Local1, 0, 0, 0, 0, 0)
  Return (Local1)
}

//
// Sync up IRQ#1 line status
//   Assuming KBC_IRQ#1 and VW ch are enabled
//   Arg0 - Leave IRQ#1 as 1 or 0
//
// Effect
//     PMx40 bit 1
//   eSPIx9C
//
Method (SIQ1, 1, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.SIQ1 Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  And (Arg0, 0x000F, Arg0)
  ECPC(Or (0x00F40000, Arg0)) // PC 0xECF4000x

  // Make sure IRQ#1 is unmasked before addtional sync
  Store(0, EMI1)

  // Check if IRQ#1 keep in HIGH
  Store(0, Local3)         // Local3 == 0 means IRQ#1 is not keeping in HIGH
  Store(RIQS(1), Local0)   // Local0 is IRQ#1 status
  If(LEqual(Local0, 3)) {  // Do the polling only if IRQ#1 had asserted already

    Store(1, Local3)       // Local3 == 1 means IRQ#1 is keeping in HIGH
    Store(160, Local2)     // At max 160 x 2 ms
    While(LNotEqual(Local2, 0)) {
      Sleep(2)             // Wait 2ms in a loop

      Store(RIQS(1), Local0)
      If (LNotEqual(Local0, 3)) {
        Store(0, Local3)   // Clear the flag (Local3)
        break
      }
      Decrement(Local2)
    }
  }

  If (LEqual(Local3, 1)) { // IRQ#1 is keeping in HIGH
                           // means IRQ#1 is not sync up with I/O APIC

    //
    // Need IRQ#1 high before make falling edge
    //
    Store(KB64, Local0)    // Read KB_SC
    If (LNot(And(Local0, 0x01))) {
      WKB4(0x00)           // Put a dummy data to Iox64. It results IRQ#1 assert
    }

    // just for debug purpose
    ECPC (Or(Local0, 0x00016400)) // postcode EC0164xx - before
    ECPC (Or(KB64,   0x00026400)) // postcode EC0064xx - after

    EMBF(100)              // Empty OBF, so de-assert IRQ#1

    // re-assert IRQ#1 if Arg0 is 1
    If (Arg0) {
      WKB4(0x00)           // Put a dummy data to Iox64. It results IRQ#1 assert
    }

    ECPC(Or (0x00E4EEE0, Arg0)) // PC 0xECE4EEEx
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.SIQ1 End Return 1\n", 0, 0, 0, 0, 0, 0)
    Return (1)
  }

  //
  // IRQ#1 is not HIGH
  //
  Store(1, EMI1)           // Mask off IRQ#1 from eSPI

  //
  // State machine to travel IRQ1 low/high status
  //
  Store (10, Local2)       // at max 10 loops
  Store (0,  Local4)       // exit flag

  Store(RIQS(1), Local0)   // Local0 is IRQ#1 status
  ECPC(Or(Local0, 0x000300C0))

  While(Local2) {
    Switch (Local0) {

      Case (0) {
        ECPC(0x000400D0)

        If (And(KB64, 0x01)) {
           EMBF(50)        // Empty OBF, so de-assert IRQ#1
        } Else {
           WKB4(0x00)      // Put a dummy data to Iox64. It results IRQ#1 assert
        }
        Sleep(5)
      }

      Case (2) {
        ECPC(0x000400D2)

        WKB4(0x00)         // Put a dummy data to Iox64. It results IRQ#1 assert

        // Set exit flag and exit if both case 2 and 3 had reached
        Or(Local4, 0x01, Local4)
        If (LEqual(Local4, 0x03)) {
          Store (1, Local2) // exit the while loop
        }
      }

      Case (3) {
        ECPC(0x000400D3)

        EMBF(50)           // Empty OBF, so de-assert IRQ#1

        // Set exit flag and exit if both case 2 and 3 had reached
        Or(Local4, 0x02, Local4)
        If (LEqual(Local4, 0x03)) {
          Store (1, Local2) // exit the while loop
        }
      }

      Default {
        ECPC(0x000400DF)

        ENKB()             // Enable KBC
        WKB4(0x00)         // Put a dummy data to Iox64. It results IRQ#1 assert
        EMBF(50)           // Empty OBF, so de-assert IRQ#1
      }
    }

    ECPC (Or(0x00056400, KB64))

    Sleep(1)
    Store(RIQS(1), Local0) // Local0 is IRQ#1 status
    ECPC (Or(Local0, 0x000600C0))

    Decrement(Local2)
  }

  //
  // Assuming VW IRQ1 is aligned with I/O APIC
  //

  EMBF(50)                 // Empty OBF, so de-assert IRQ#1
  Store(0, EMI1)           // Unmask IRQ#1

  If(LNotEqual(Arg0, 0)) { // If the request is keep IRQ in assert
    WKB4(0x00)             // Put a dummy data to Iox64. It results IRQ#1 assert
  }

  ECPC(Or(0x00E4FFF0, Arg0)) // PC 0xECE4FFFx
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.SIQ1 End Return 0\n", 0, 0, 0, 0, 0, 0)
  Return (0)
}

//
// Empty 8042 KBC input buffer
//   Arg0 - loop counter, for example 200
//
Method (EMBF, 1, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.EMBF Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  Store (Arg0, Local2)
  Store (0x01, Local0)
  While (LAnd (Local2, And(Local0, 0x01))) {

    // Empty output buffer by read IOx60 until OBF is de-asserted in IOx64.
    Sleep(1)
    Store(KB60, Local1)
    ECPC(Or(0x00F56000, Local1))

    Sleep(1)
    Store(KB64, Local0)
    ECPC(Or(0x00F56400, Local1))

    Decrement(Local2)
  }
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.EMBF End\n", 0, 0, 0, 0, 0, 0)
}

//
// Wait IBE at max 50 ms
//
Method (WIBE, 0, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WIBE Start\n", 0, 0, 0, 0, 0, 0)
  Store(50, Local2)
  While(Local2) {
    If(LNot(KIBF)) {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WIBE End Return 0\n", 0, 0, 0, 0, 0, 0)
      Return (0)
    }

    Sleep(1)
    Decrement(Local2)
  }

  // debug purpose
  // print a code if input buffer full in the end of the polling
  If(KIBF) {
    ECPC(0x00F600EE)
  }

  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WIBE End Return 1\n", 0, 0, 0, 0, 0, 0)
  Return (1)
}

//
// Write KBC SC port with checking IBF (bit 1)
//
Method (WKB4, 1, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WKB4 Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  And(Arg0, 0xFF, Local0)
  ECPC(Or(Local0, 0x00F76400))

  WIBE()

  Store(Local0, KB64)

  WIBE() // wake for IBE before exit

  ECPC(Or(Local0, 0x00E76400))
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WKB4 End\n", 0, 0, 0, 0, 0, 0)
}

//
// Write KBC data port with checking IBF (bit 1)
//
Method (WKB0, 1, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WKB0 Start Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
  And(Arg0, 0xFF, Local0)
  ECPC(Or(Local0, 0x00F86000))

  WIBE()

  Store(Local0, KB60)

  WIBE() // wake for IBE before exit

  ECPC(Or(Local0, 0x00E86000))
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.WKB0 End\n", 0, 0, 0, 0, 0, 0)
}

//
// Enable 8042 KBC
//
// Meaning of write 0x65 to IOx60
//    i.   Bit6 = 1; Scan code set PC/XT
//    ii.  Bit5 = 1; Disable auxiliary device, i.e. no mouse.
//    iii. Bit4 = 0; Enable KB.
//    iv.  Bit3 = 0; allow inhibit; no concrete meaning by our KBC implementation
//    v.   Bit2 = 1; set status register system; no concrete meaning by our KBC implementation
//    vi.  Bit1 = 0; should be 0
//    vii. Bit0 = 1; enable output register full interrupt, i.e IRQ#1 will asserted once output register is full
//
Method (ENKB, 0, Serialized) {
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.ENKB Start\n", 0, 0, 0, 0, 0, 0)
  ECPC(0x00F90000)

  WKB4(0x60)        // Write 60 to IOx64, so the next byte written to port 60h is placed in 8042 command register

  WKB0(0x65)        // This is the 'next byte'. See above for 0x65 meaning.
                    // Key point here is Scan code set to PC/XT and enable output register full interrupt (IRQ#1)

  ECPC(0x00E90000)
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0.ENKB End\n", 0, 0, 0, 0, 0, 0)
}

Method(_Q70,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q70 Start\n", 0, 0, 0, 0, 0, 0)
  ECPC(0x00000070)

  Store(0x70, SYSK) // Notify EC _Q70 is executing

  KBRS()

  ECPC(0x0000EE70)
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q70 End\n", 0, 0, 0, 0, 0, 0)
}
