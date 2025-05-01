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

#ifndef _PCI0_
#define _PCI0_

#define GetPcieCapabilityReg      PXCR
#define SavePcieConfiguration     SPCF
#define RestorePcieConfiguration  RPCF
#define UnlockNvmePassword        UPWD

Device(PCI0)
{
  Name(_HID,EISAID("PNP0A08"))            // PCI Express Bus
  Name(_CID,EISAID("PNP0A03"))
  Name(_UID,0x01)
  Name(_BBN,0x00)
  Name(_ADR, 0x00000000)                  // HW:dev#, LW:fnct#=0

  Name(NBRI, 0x00)
  Name(NBAR, 0x00)
  Name(NCMD, 0x00)
  Name(PXDC, 0x00)
  Name(PXLC, 0x00)
  Name(PXD2, 0x00)

  //
  // _INI method
  //
  #include "_INI.asl"

  Name(SUPP,0) // PCI _OSC Support Field value
  Name(CTRL,0) // PCI _OSC Control Field value
  Method(_OSC, 4) {
    // Note: Since these DwordFields are accessed from the else
    // condition of UUID check, they cannot be created inside the
    // If(LEqual .... ) scope.  If put inside the LEqual, it will
    // cause problem if the code ever executes the Else (unrecognized UUID)
    // condition.

    // Create DWord-adressable fields from the Capabilities Buffer
    CreateDWordField(Arg0,0,UID0)
    CreateDWordField(Arg3,0,CDW1)
    CreateDWordField(Arg3,4,CDW2)
    CreateDWordField(Arg3,8,CDW3)
    If(LEqual(Arg2, One))
    {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._OSC UUID 0x%X Start  CDW1 = 0x%X\n", UID0, CDW1, Zero, Zero, Zero, Zero)
    }
    Else
    {
      If(LEqual(Arg2, 0x02))
      {
        Store(CDW2, Local2)
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._OSC UUID 0x%X Start  CDW1 = 0x%X CDW2 = 0x%X\n", UID0, CDW1, CDW2, Zero, Zero, Zero)
      }
      Else
      {
        Store(CDW3, Local3)
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._OSC UUID 0x%X Start  CDW1 = 0x%X CDW2 = 0x%X CDW3 = 0x%X\n", UID0, CDW1, CDW2, CDW3, Zero, Zero)
      }
    }
    // Check for proper UUID
    If(LEqual(Arg0,ToUUID("33DB4D5B-1FF7-401C-9657-7441C03DD766"))) {
      // Save Capabilities DWord2 & 3
      Store(CDW2,SUPP)
      Store(CDW3,CTRL)
      if (LEqual (TBEN, 1)) {
        if (LNotEqual (TBNH, 0)) {          // Enable PCIe Native Hot Plug in ACPI _OSC Arg3 Bit0
          And(CTRL,0xFFFFFFF5,CTRL)
        } else {                            // Disable PCIe Native Hot Plug in ACPI _OSC Arg3 Bit0
          And(CTRL,0xFFFFFFF4,CTRL)
//          Or(CDW1,4,CDW1) // Unrecognized UUID
//          Return(Arg3)
        }
      }
      // Only allow native hot plug control if OS supports:
      // * ASPM
      // * Clock PM
      // * MSI/MSI-X
      If(LNotEqual(And(SUPP, 0x16), 0x16)) {
        And(CTRL,0xFFFFFFFE,CTRL) // Mask bit 0 (and undefined bits)
      }

      // Always allow native PME
      // Never allow SHPC (no SHPC controller in this system)
      // Remove AER Control (Bit 3)
      And(CTRL,0xFFFFFFF5,CTRL)

      If(Not(And(CDW1,1))) { // Query flag clear?
        // Disable GPEs for features granted native control.
        If(And(CTRL,0x01)) { // Hot plug control granted?
          //Store(0,HPCE) // clear the hot plug SCI enable bit
          //Store(1,HPCS) // clear the hot plug SCI status bit
        }

        //                                               Native    Legacy
        // PMIO:0x74[24] - PcieNative            1         0
        // PMIO:0x74[25] - PcieWakMask        0         0
        // PMIO:0x74[27] - WakePinAsGevent  1         0
        // PMIO:0xBA[14] - WakePinEnable      0         1
        If (And(CTRL,0x04)) { // Native PME control granted?
          // Handled during POST according to user option
        }

        If(And(CTRL,0x10)) {// OS restoring PCIe cap structure?
          // Set status to not restore PCIe cap structure
          // upon resume from S3
        }
      }

      If(LNotEqual(Arg1,One)) {
        // Unknown revision
        Or(CDW1,0x08,CDW1)
      }

      If(LNotEqual(CDW3,CTRL)) {
        // Capabilities bits were masked
        Or(CDW1,0x10,CDW1)
      }

      // Update DWORD3 in the buffer
      Store(CTRL,CDW3)
    } Else {
      Or(CDW1,4,CDW1) // Unrecognized UUID
    }
    If(LEqual(Arg2, One))
    {
      CpmDebugPrint ("PLA-ASL-\\_SB._OSC UUID 0x%X Return CDW1 = 0x%X\n", UID0, CDW1, Zero, Zero, Zero, Zero)
    }
    Else
    {
      If(LEqual(Arg2, 0x02))
      {
        CpmDebugPrint ("PLA-ASL-\\_SB._OSC UUID 0x%X Return CDW1 = 0x%X CDW2 = 0x%X\n", UID0, CDW1, CDW2, Zero, Zero, Zero)
      }
      Else
      {
        CpmDebugPrint ("PLA-ASL-\\_SB._OSC UUID 0x%X Return CDW1 = 0x%X CDW2 = 0x%X CDW3 = 0x%X\n", UID0, CDW1, CDW2, CDW3, Zero, Zero)
      }
    }
    Return(Arg3)
  } // End _OSC

  Method(TOM) {
    Multiply(TOML, 0x10000, Local0)
    Multiply(TOMH, 0x1000000, Local1)
    Add(Local0, Local1, Local0)
    Return(Local0)
  } // END of TOM

  // Bridge _CRS
  Name(CRES, ResourceTemplate() {
    WORDBusNumber(
      ResourceProducer,
      MinFixed,               // _MIF
      MaxFixed,               // _MAF
      SubDecode,              // _DEC
      0x00,                   // _GRA
      0x00,                   // _MIN
      0x0FF,                  // _MAX
      0x00,                   // _TRA
      0x0100,                 // _LEN
      0x00, , WBS             // name of this descriptor
    )

    WORDIO(ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange, 0x0, 0x0, 0xCF7, 0x0, 0xCF8, 0x0, , BCF8)
    WORDIO(ResourceProducer, MinFixed, MaxFixed, PosDecode, EntireRange, 0x0, 0xD00, 0xFFFF, 0x0, 0xF300)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite, 0x0, 0xA0000, 0xBFFFF, 0x0, 0x20000, 0x0, , A000)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable,  ReadOnly, 0x0, 0xC0000, 0xC3FFF, 0x0, 0x04000, 0x0, , C000)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable,  ReadOnly, 0x0, 0xC4000, 0xC7FFF, 0x0, 0x04000, 0x0, , C400)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadOnly, 0x0, 0xC8000, 0xCBFFF, 0x0, 0x04000, 0x0, , C800)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadOnly, 0x0, 0xCC000, 0xCFFFF, 0x0, 0x04000, 0x0, , CC00)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite, 0x0, 0xD0000, 0xD3FFF, 0x0, 0x04000, 0x0, , D000)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite, 0x0, 0xD4000, 0xD7FFF, 0x0, 0x04000, 0x0, , D400)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite, 0x0, 0xD8000, 0xDBFFF, 0x0, 0x04000, 0x0, , D800)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite, 0x0, 0xDC000, 0xDFFFF, 0x0, 0x04000, 0x0, , DC00)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0xE0000, 0xE3FFF, 0x0, 0x04000, 0x0, , E000)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0xE4000, 0xE7FFF, 0x0, 0x04000, 0x0, , E400)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0xE8000, 0xEBFFF, 0x0, 0x04000, 0x0, , E800)
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0xEC000, 0xEFFFF, 0x0, 0x04000, 0x0, , EC00)
//[-start-220126-IB14740206-modify]//
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0xFB000000, 0xFCFFFFFF, 0x0, 0x02000000, , , M32N)
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0xD0000000, 0xEFFFFFFF, 0x0, 0x20000000, , , M32P)
    IO(Decode16, 0xCF8, 0xCF8, 0x1, 0x8, CF8)
//>>EnableAbove4GBMmio++
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0x069739200000, 0x07FCFFFFFFFF, 0x0, 0x0165C6E00000, , , M64N)
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite, 0x0, 0x010020200000, 0x0697391FFFFF, 0x0, 0x059719000000, , , M64P)
//<<EnableAbove4GBMmio++
//[-end-220126-IB14740206-modify]//
  }) //End of PCI0.CRES

//[-start-220126-IB14740206-modify]//
  Method(_CRS,0) {
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._CRS Start\n", 0, 0, 0, 0, 0, 0)
    CreateDWordField(^CRES, ^M32N._MIN, M3BN)
    CreateDWordField(^CRES, ^M32N._MAX, M3FN)
    CreateDWordField(^CRES, ^M32N._LEN, M3DN)
    Store(\MUBN, M3BN)
    Store(\MULN, M3FN)
//[-start-220525-IB14740243-modify]//
    If (LGreater(M3BN, M3FN)) {
      Store(Zero, M3DN)
    } Else {
      Subtract(M3FN, M3BN, M3DN)
      Add(M3DN, 1, M3DN)
    }
//[-end-220525-IB14740243-modify]//
    CreateDWordField(^CRES, ^M32P._MIN, M3BP)
    CreateDWordField(^CRES, ^M32P._MAX, M3FP)
    CreateDWordField(^CRES, ^M32P._LEN, M3DP)
    Store(\MUBP, M3BP)
    Store(\MULP, M3FP)
//[-start-220525-IB14740243-modify]//
    If (LGreater(M3BP, M3FP)) {
      Store(Zero, M3DP)
    } Else {
      Subtract(M3FP, M3BP, M3DP)
//[-start-220523-IB14740242-modify]//
      Add(M3DP, 1, M3DP)
//[-end-220523-IB14740242-modify]//
    }
//[-end-220525-IB14740243-modify]//
    CreateQWordField(^CRES, ^M64N._MIN, M6BN)
    CreateQWordField(^CRES, ^M64N._MAX, M6FN)
    CreateQWordField(^CRES, ^M64N._LEN, M6DN)
    Store(\MABN, M6BN)
    Store(\MALN, M6FN)
    Store(\MASN, M6DN)
    CreateQWordField(^CRES, ^M64P._MIN, M6BP)
    CreateQWordField(^CRES, ^M64P._MAX, M6FP)
    CreateQWordField(^CRES, ^M64P._LEN, M6DP)
    Store(\MABP, M6BP)
    Store(\MALP, M6FP)
    Store(\MASP, M6DP)
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0._CRS End\n", 0, 0, 0, 0, 0, 0)
    Return(CRES)
  } //End of PCI0._CRS
//[-end-220126-IB14740206-modify]//

  Device(MEMR)
  {
    Name(_HID,EISAID("PNP0C02")) // Motherboard Resources
    Name(BAR3, 0xFFF00000)
    Name(MEM1,ResourceTemplate()
    {
      Memory32Fixed(ReadWrite,0x00000000,0x00000000)
      Memory32Fixed(ReadWrite,0x00000000,0x00000000)
      Memory32Fixed(ReadWrite,0x00000000,0x00000000)
    })
    Method(_CRS,0,NotSerialized)
    {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.MEMR._CRS Start\n", 0, 0, 0, 0, 0, 0)
      CreateDwordField(MEM1,0x04,MB01)
      CreateDwordField(MEM1,0x08,ML01)
      CreateDwordField(MEM1,0x10,MB02)
      CreateDwordField(MEM1,0x14,ML02)
      CreateDwordField(MEM1,0x1C,MB03)
      CreateDwordField(MEM1,0x20,ML03)
      If(GPIC)
      {
        Store(0xFEC00000,MB01)
        Store(0xFEE00000,MB02)
        Store(0x1000,ML01)
        IF(\NAPC) {
          Add(ML01, 0x1000, ML01)
        }
        Store(0x1000,ML02)
      }
      If (LNotEqual (BAR3,0xFFF00000)) {
        Store (BAR3, MB03)
        Store (0x100000, ML03)
      }
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.MEMR._CRS End\n", 0, 0, 0, 0, 0, 0)
      Return(MEM1)
    }
  }

//  OperationRegion(NAPC, PCI_Config, 0xB8, 0x08)
//  Field(NAPC,DWordAcc,NoLock,Preserve)
//  {
//    NAPX, 32,
//    NAPD, 32
//  }

  Mutex(NAPM, 0)
  Method(NAPE, 0) {
    Acquire(NAPM, 0xFFFF)
    Add(PCBA, 0xB8, Local0)
    OperationRegion(varM, SystemMemory, Local0, 0x8)
    Field(varM, DWordAcc, NoLock, Preserve) {
      NAPX, 32,
      NAPD, 32
    }
    Store(NAPX, Local1)
    Store(0x14300000, NAPX)
    Store(NAPD, Local0)
    And(Local0, 0xFFFFFFEF, Local0)
    Store(Local0, NAPD)
    Store(Local1, NAPX)
    Release(NAPM)
  }

  Method(GetPcieCapabilityReg, 3, Serialized) {
      CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.PXCR\n", 0, 0, 0, 0, 0, 0)
      Store (0, Local0)
      Store (CpmReadPci (Arg0, Arg1, Arg2, 0x34, 0, 8), Local1)
      While (LNotEqual (Local1, 0)) {
          Store (CpmReadPci (Arg0, Arg1, Arg2, Local1, 0, 8), Local2)
          If (LOr(LEqual(Local2, 0x00), LEqual(Local2, 0xFF))) {
              break
          }
          If (LEqual(Local2, 0x10)) {
              Store (Local1, Local0)
              break
          }
          Store (CpmReadPci (Arg0, Arg1, Arg2, Add(Local1,1), 0, 8), Local1)
      }
      Return (Local0)
  }

  Method(SavePcieConfiguration, 1) {
      CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.SPCF\n", 0, 0, 0, 0, 0, 0)
      Store (CpmReadPci32 (0, ShiftRight(Arg0, 16), And(Arg0, 0x000000FF), 0x18), Local0)
      ShiftRight (And (Local0, 0xFF00), 8, NBRI)
      Store (CpmReadPci32 (NBRI, 0, 0, 4), NCMD)
      Store (CpmReadPci32 (NBRI, 0, 0, 0x10), NBAR)
      Store (GetPcieCapabilityReg (NBRI, 0, 0), Local1)
      Store (CpmReadPci32 (NBRI, 0, 0, Add(Local1, 0x8)), PXDC)
      Store (CpmReadPci32 (NBRI, 0, 0, Add(Local1, 0x10)), PXLC)
      Store (CpmReadPci32 (NBRI, 0, 0, Add(Local1, 0x28)), PXD2)
  }

  Method(RestorePcieConfiguration) {
      CpmDebugPrint ("PLA-ASL-\_SB.PCI0.GPPX.RPCF\n", 0, 0, 0, 0, 0, 0)
      Store (GetPcieCapabilityReg(NBRI, 0, 0), Local1)
      CpmWritePci32 (NBRI, 0, 0, Add(Local1, 0x8), PXDC)
      CpmWritePci32 (NBRI, 0, 0, Add(Local1, 0x10), And(PXLC, 0xFFFFFEFC))  //Clear bit0,1,8
      CpmWritePci32 (NBRI, 0, 0, Add(Local1, 0x28), PXD2)
      CpmWritePci32 (NBRI, 0, 0, 0x10, NBAR)
      CpmWritePci32 (NBRI, 0, 0, 4, Or(NCMD, 0x6))
  }

  Method(UnlockNvmePassword) {
      CpmDebugPrint ("PLA-ASL-\_SB.PCI0.UPWD\n", 0, 0, 0, 0, 0, 0)
      //
      // SMI_CMD Port
      //
      OperationRegion (PSMI, SystemIO, 0xB0, 0x2)
      Field (PSMI, ByteAcc, NoLock, Preserve) {
          SMIC, 8,
          SMID, 8
      }
      Store (0xE3, SMIC)
  }

  //
  // HOST Bridge PCI Routing Table (_PRT)
  //
  #include "_PRT.asl"

  //
  // PCIE P2P Bridge (Dev 1 Fn 1)
  //
  #include "GPP0.asl"

  //
  // PCIE P2P Bridge (Dev 1 Fn 2)
  //
  #include "GPP1.asl"

  //
  // PCIE P2P Bridge (Dev 1 Fn 3)
  //
  #include "GPP2.asl"

  //
  // PCIE P2P Bridge (Dev 1 Fn 4)
  //
  #include "GPP3.asl"

  //
  // PCIE P2P Bridge (Dev 1 Fn 5)
  //
  #include "GPP4.asl"

  //
  // PCIE P2P Bridge (Dev 2 Fn 1)
  //
  #include "GPP5.asl"

  //
  // PCIE P2P Bridge (Dev 2 Fn 2)
  //
  #include "GPP6.asl"

  //
  // PCIE P2P Bridge (Dev 2 Fn 3)
  //
  #include "GPP7.asl"

  //
  // PCIE P2P Bridge (Dev 2 Fn 4)
  //
  #include "GPP8.asl"

  //
  // PCIE P2P Bridge (Dev 2 Fn 5)
  //
  #include "GPP9.asl"

  //
  // PCIE P2P Bridge (Dev 2 Fn 6)
  //
  #include "GP10.asl"

  //
  // PCIE P2P Bridge (Dev 3 Fn 1)
  //
  #include "GP11.asl"

  //
  // PCIE P2P Bridge (Dev 4 Fn 1)
  //
  #include "GP12.asl"

  //
  // PCIE PCIE GPP Bridge 0 to Bus A (Dev 8 Fn 1)
  //
  #include "GP17.asl"

  //
  // PCIE PCIE GPP Bridge 0 to Bus B (Dev 8 Fn 2)
  //
  #include "GP18.asl"

  //
  // PCIE PCIE GPP Bridge 0 to Bus C (Dev 8 Fn 3)
  //
  #include "GP19.asl"

  //
  // High Precision Event Timer
  //
  #include "HPET.asl"

  //
  // SMBUS and ACPI Controller
  //
  #include "Smbs.asl"

  //
  // LPC CONTROLLER
  //
  #include "Lpc.asl"


} // End PCI Bus 0

#endif
