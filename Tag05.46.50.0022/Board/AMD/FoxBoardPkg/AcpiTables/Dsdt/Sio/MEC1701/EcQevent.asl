/** @file

;******************************************************************************
;* Copyright (c) 2012, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

//0x24 - DOCK is connected
//0x25 - DOCK is disconnected
//0x26 - Lid Open, LID_SWITCH L to H
//0x27 - Lid Close, LID_SWITCH H to L
//0x35 - AC Power in/out
//0x39 - Battery in/out
//0x3E - Battery Trip Point
//0x40 - (Runtime generation in CPM) Odd plugin falling edge
//0x41 - (Runtime generation in CPM) Odd plugin rising edge
//0x42 - (Runtime generation in CPM) Odd DA falling edge
//0x43 - (Runtime generation in CPM) Odd DA rising edge
//0x45 - Battery charge done

#define QEVENT_DEBUG

#ifdef  QEVENT_DEBUG
#define QEVENT_DEBUG_DELAY      1
#else
#define QEVENT_DEBUG_DELAY      1
#endif

//
//Dock is connected
//
Method(_Q24,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q24 DOCK ON Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC24, P80H)
#endif
    If (LEqual(DPTC,0x01)) 
    {
      CreateWordField(DPTI, 0x00, SSZE)         // Structure size field
      CreateByteField(DPTI, 0x02, MSID)         // ID
      CreateDWordField(DPTI, 0x03, MSDV)        // VALUE

      Store(0x7, SSZE)                      // table size 7
      
      CpmDebugPrint ("PLA-ASL- DOCK ON proc DPTC ALIB Call\n", 0, 0, 0, 0, 0, 0)
      
      Store(0x5, MSID)                      // SustainedPowerLimit, 25000
      Store(25000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x6, MSID)                      // FastPPTLimit, 48000
      Store(48000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x7, MSID)                      // SlowPPTLimit, 37000
      Store(37000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x8, MSID)                      // SlowPPTTimeConstant, 4
      Store(4, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x22, MSID)                     // BIOSSMC_MSG_Set_STT_SKIN_TEMPERATURE_LIMIT_APU, 90
      Store(90, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0xB, MSID)                      // BIOSSMC_MSG_SetVrmVddCurrentLimit, 48000
      Store(48000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0xC, MSID)                      // BIOSSMC_MSG_SetVrmVddMaximumCurrentLimit, 105000
      Store(105000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0xE, MSID)                      // BIOSSMC_MSG_SetVrmSocCurrentLimit, 14000
      Store(14000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x9, MSID)                      // SetProchotDeassertionRampTime, 10
      Store(10, MSDV)
      \_SB.ALIB(0xC, DPTI)
  }
  
//  \DOCK (0)
}

//
//Dock is disconnected
//
Method(_Q25,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q25 DOCK OFF Start\n", 0, 0, 0, 0, 0, 0)
#ifdef   QEVENT_DEBUG
  Store(0xEC25, P80H)
#endif
    If (LEqual(DPTC,0x01)) 
    {
      CreateWordField(DPTI, 0x00, SSZE)         // Structure size field
      CreateByteField(DPTI, 0x02, MSID)         // ID
      CreateDWordField(DPTI, 0x03, MSDV)        // VALUE

      Store(0x7, SSZE)                      // table size 7
      
       CpmDebugPrint ("PLA-ASL- DOCK OFF proc DPTC ALIB Call\n", 0, 0, 0, 0, 0, 0)
      
      Store(0x5, MSID)                      // SustainedPowerLimit, 15000
      Store(15000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x6, MSID)                      // FastPPTLimit, 30000
      Store(30000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x7, MSID)                      // SlowPPTLimit, 25000
      Store(25000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x8, MSID)                      // SlowPPTTimeConstant, 5
      Store(5, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x22, MSID)                     // BIOSSMC_MSG_Set_STT_SKIN_TEMPERATURE_LIMIT_APU, 100
      Store(100, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0xB, MSID)                      // BIOSSMC_MSG_SetVrmVddCurrentLimit, 40000
      Store(40000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0xC, MSID)                      // BIOSSMC_MSG_SetVrmVddMaximumCurrentLimit, 90000
      Store(90000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0xE, MSID)                      // BIOSSMC_MSG_SetVrmSocCurrentLimit, 13000
      Store(13000, MSDV)
      \_SB.ALIB(0xC, DPTI)
      
      Store(0x9, MSID)                      // SetProchotDeassertionRampTime, 20
      Store(20, MSDV)
      \_SB.ALIB(0xC, DPTI)
  }
//  \DOCK (1)
}

//
//Lid open
//
Method(_Q26,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q26 Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC26, P80H)
#endif
  Notify(\_SB.LID, 0x80)
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q26 End\n", 0, 0, 0, 0, 0, 0)
}

//
//Lid close
//
Method(_Q27,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q27 Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC27, P80H)
#endif
  Notify(\_SB.LID, 0x80)
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q27 End\n", 0, 0, 0, 0, 0, 0)
}

//
//AC Power Insertion/Removal
//
Method(_Q35,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q35 Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC35, P80H)
#endif
  if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.QEVT, 300))) {
    Notify(\_SB.ACAD, 0x80)
    Notify(\_SB.BATT, 0x80)
    Notify(\_SB.BATT, 0x81)
    Release(\_SB.PCI0.LPC0.EC0.QEVT)
  }
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q35 End\n", 0, 0, 0, 0, 0, 0)
}

//
// Battery Insertion
//
Method(_Q39,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q39 Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC39, P80H)
#endif
  if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.QEVT,300))) {
    Notify(\_SB.ACAD, 0x80)
    Notify(\_SB.BATT, 0x80)
    Notify(\_SB.BATT, 0x81)
    Release(\_SB.PCI0.LPC0.EC0.QEVT)
  }
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q39 End\n", 0, 0, 0, 0, 0, 0)
}

//
// Battery Trip Point
//
Method(_Q3E,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q3E Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC3E, P80H)
#endif

  // Write 1 to bit 7 of BATS to clear the event.
  if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {

    Store(\_SB.PCI0.LPC0.EC0.BATS, Local0)
    Or(Local0, 0x80, Local0)
    Store(Local0, \_SB.PCI0.LPC0.EC0.BATS)

    Release(\_SB.PCI0.LPC0.EC0.Z009)
  }

  if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.QEVT,300))) {
    Notify(\_SB.BATT, 0x80)
    Release(\_SB.PCI0.LPC0.EC0.QEVT)
  }
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q3E End\n", 0, 0, 0, 0, 0, 0)
}

//
// Battery charge done
//
Method(_Q45,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q45 Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC45, P80H)
#endif

  Notify(\_SB.BATT, 0x80)
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q45 End\n", 0, 0, 0, 0, 0, 0)
}

Name(DPTI, Buffer(0x7){})

//
// DPTC
//
Method(_Q49,0,NotSerialized)
{
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q49 Start\n", 0, 0, 0, 0, 0, 0)
#ifdef  QEVENT_DEBUG
  Store(0xEC49, P80H)
#endif
  If (LEqual(DPTC,0x01)) {
    CreateWordField(DPTI, 0, SSZE)        // Structure size field
    CreateByteField(DPTI, 2, MSID)
    CreateByteField(DPTI, 3, DECI)
    CreateByteField(DPTI, 4, INTE)
    CreateByteField(DPTI, 5, EP00)
    CreateByteField(DPTI, 6, EP01)

    Store(0x7, SSZE)                      // table size 7
    Store(0xA, MSID)
    Store(0x0, EP00)
    Store(0x0, EP01)
    if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {

      Store(\_SB.PCI0.LPC0.EC0.MDEC,DECI)   // decimal part
      Store(\_SB.PCI0.LPC0.EC0.MINT,INTE)   // integer part

      \_SB.ALIB(0xC, DPTI)

      Release(\_SB.PCI0.LPC0.EC0.Z009)
    }
  }
  CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.LPC0.EC0._Q49 End\n", 0, 0, 0, 0, 0, 0)
}

