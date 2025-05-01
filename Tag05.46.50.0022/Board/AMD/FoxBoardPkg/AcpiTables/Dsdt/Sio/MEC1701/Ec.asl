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

External(\_SB.ALIB, MethodObj)

// ECOK - Return TRUE if EC Namespace accessible.
Method(ECOK,0)
{
  If (LEqual(\_SB.PCI0.LPC0.EC0.OKEC, 0x01))
  {
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.ECOK = 1\n", 0, 0, 0, 0, 0, 0)
    Return (1)                      //return true
  }
  else
  {
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.ECOK = 0\n", 0, 0, 0, 0, 0, 0)
    Return (0)                      //return false
  }
}

Device(EC0)
{
  Name(_HID,  EISAID("PNP0C09"))  // PNP ID

//  Name(RHGP, 3)   // To be patched to Name(_GPE, 3) if HW_Reduced_ACPI is false
  Name(_GPE, 11)      // EC SCI Event 11

  Method(_STA,0,NotSerialized) {
    If (LEqual (CpmReadTable (CpmMainTable, CpmEcSupport), 1))
    {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._STA = 0xF\n", 0, 0, 0, 0, 0, 0)
      Return(0x0F)
    }
    else
    {
      CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._STA = 0x0\n", 0, 0, 0, 0, 0, 0)
      Return(0)
     }
  }
  Name(OKEC, Zero)

  // EC resources
  Method(_CRS, 0, NotSerialized) {
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._CRS\n", 0, 0, 0, 0, 0, 0)
    Name(BUF0,ResourceTemplate() {
      IO (Decode16, 0x662, 0x662, 0x1, 0x1)
      IO (Decode16, 0x666, 0x666, 0x1, 0x1)
    })
    Name(BUF1,ResourceTemplate() {
      IO(Decode16,0x662,0x662,0x1,0x1)
      IO(Decode16,0x666,0x666,0x1,0x1)
      GpioInt (
        Edge,
        ActiveLow,
        ExclusiveAndWake,
        PullUp,
        0,
        "\\_SB.GPIO",
        0x00,
        ResourceConsumer,
        ,
      ) {7}    // GPIO Interrupt Connection resource for AGPIO7_FCH_ACP_I2S_SDIN/GEVENT11
    })

    If(LEqual(RDHW, 0x0)) {   // HW_REDUCED_ACPI enabled?
      Return(BUF0)
    } Else {
      Return(BUF1)
    }
  }

  /* ------------------------------------------------------------------------*/
  // Name: _REG - This method is called by drivers to register installation
  //              and removal.  For example; OS calls _REG(3,1) to indicate
  //              to the ACPI BIOS that the Embedded Controller Driver is
  //              present and functional.
  //
  //              ARG0: 0=Memory
  //                    1=I/O
  //                    2=PCI Config
  //                    3=Embedded Controller
  //                    4=SMBus
  //
  //              ARG1: 0=Handler Not Connected
  //                    1=Handler Connected
  //
  Method(_REG, 2) {
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._REG Start Arg0 = 0x%X Arg1 = 0x%X\n", ToInteger(Arg0), ToInteger(Arg1), 0, 0, 0, 0)
    If (LEqual(Arg0, 0x03)) {
      // EC EnableAcpi
      if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {
        Store(\_SB.PCI0.LPC0.EC0.STAS, Local0)
        Or(Local0, 0x04, Local1)
        Store(Local1, \_SB.PCI0.LPC0.EC0.STAS)
        Release(\_SB.PCI0.LPC0.EC0.Z009)
      }

      Store(Arg1, OKEC)
      Notify(\_SB.ACAD, 0x80)
      Notify(\_SB.BATT, 0x80)
      Notify(\_SB.BATT, 0x81)   //Fix Battery information haven't display properly
      Notify(\_SB.LID, 0x80)
      if (OKEC) {
        ////DOCK INIT
        //Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)
        //Store(\_SB.PCI0.LPC0.EC0.DKIN, Local0)
        //And(Local0, 0x02, Local1)
        //Release(\_SB.PCI0.LPC0.EC0.Z009)
        //If (LEqual(Local1, 0x02)) {
        //  \DOCK (1)
        //} Else {
        //  \DOCK (0)
        //}

        ////enable on Windows 7
        //if(And(LGreaterEqual(TPOS, 0x50), LNotEqual(ALST, 0))) {
        //  Acquire(\_SB.PCI0.LPC0.EC0.Z009, 0xFFFF)
        //  Store(\_SB.PCI0.LPC0.EC0.BATS, Local1)
        //  Store(Or(Local1, 0x20), \_SB.PCI0.LPC0.EC0.BATS)
        //  Release(\_SB.PCI0.LPC0.EC0.Z009)
        //}
      }
    }
    CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._REG End\n", 0, 0, 0, 0, 0, 0)
  } // end of _REG

  //
  // Print 32-bit postcode to Port 80
  // the code print by this method will lead by 0xEC
  //
  Method(ECPC, 1, Serialized) {
    OperationRegion (PCPT, SystemIO, 0x80, 4)
    Field (PCPT, DwordAcc, NoLock, Preserve) {
      PC80, 32,
    }

    And(Arg0, 0x00FFFFFF, Local0)
    Or (Local0, 0xEC000000, PC80)
  }

  // EC RAM fields
  #include "EcName.asl"       // EC Name Space
  #include "EcQevent.asl"     // _Qxx Methods
  #include "uPepHook.asl"     // uPEP hook
  #include "kbcWake.asl"      // for S0i3 KBC wake

}// END device EC0

// Move to \_SB.I2CB bus.
//#ifndef LEGACY_FREE_SUPPORT
//    #include "ALSD.asl"
//#endif


