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

#ifndef _XHCI2_
#define _XHCI2_

Device(XHC2)
{
  Name(_ADR, 0x00000000)

  // Method(XPRW,0,NotSerialized) {
  //   CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC2._PRW Return GPRW (0x1A, 0x4)\n", 0, 0, 0, 0, 0, 0)
  //   Return (GPRW (0x1A,0x04))
  // }

  Device(RHUB)
  {
    Name(_ADR, 0x00000000)
    // HS port 0
    Device(PRT1) {
      Name(_ADR, 0x00000001)
       
      Name(UPC1, Package(4) { 0xFF, 0x00, 0x00000000, 0x00000000 } )
       
      Method (_UPC,0,Serialized)
      {
          Return (UPC1)
      }

      Name (PLD1, Package()
      {
        ToPLD (
          PLD_Revision = 0x2,
          PLD_IgnoreColor = 0x01,
          PLD_Red = 0x00,
          PLD_Green = 0x00,
          PLD_Blue = 0x00,
          PLD_Width = 0x00,
          PLD_Height = 0x00,
          PLD_UserVisible = 0x00,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x08,
          PLD_Ejectable = 0x00,
          PLD_EjectRequired = 0x00 )
      })

      Method (_PLD,0,Serialized)
      {
         Return (PLD1)
      }

      Device(CAM0) {
        Name(_ADR, 0x3)
      }

      Device(CAM1) {
        Name(_ADR, 0x1)
      }

    }
    
    // Port #2, SS non report
    Device(PRT2) {
      Name(_ADR, 0x00000002)

      Name(UPC1, Package(4) { 0x00, 0x00, 0x00000000, 0x00000000 } ) // non-Connect

      Name (PLD1, Package()
      {
        ToPLD (
          PLD_Revision = 0x2,
          PLD_IgnoreColor = 0x01,
          PLD_Red = 0x00,
          PLD_Green = 0x00,
          PLD_Blue = 0x00,
          PLD_Width = 0x00,
          PLD_Height = 0x00,
          PLD_UserVisible = 0x00,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x08,
          PLD_Ejectable = 0x00,
          PLD_EjectRequired = 0x00 )
      })
      
      Method (_UPC,0,Serialized)
      {
        Return (UPC1)
      }

      Method (_PLD,0,Serialized)
      {
        Return (PLD1)
      }
    }
  }
} // XHC2

#endif
