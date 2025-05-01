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

#ifndef _XHCI1_
#define _XHCI1_

Device(XHC1)
{
  Name(_ADR, 0x00000004)

  // Method(_PRW,0,NotSerialized) {
  //   CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC1._PRW Return GPRW (0x19, 0x4)\n", 0, 0, 0, 0, 0, 0)
  //   Return (GPRW (0x19,0x04))
  // }

  Device(RHUB)
  {
    Name(_ADR, 0x00000000)

    // Port #1, 
    Device(PRT1) {
      Name(_ADR, 0x00000001)

      Name(UPC1, Package(4) { 0xFF, 0x09, 0x00000000, 0x00000000 } ) // A0, (USB-C Combo USB2.0)

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
          PLD_UserVisible = 0x01,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x05,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
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

    // Port #2;
    Device(PRT2) {
      Name(_ADR, 0x00000002)

      Name(UPC1, Package(4) { 0xFF, 0x03, 0x00000000, 0x00000000 } ) // A0, (USB-A Combo USB2.0)

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
          PLD_UserVisible = 0x01,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x06,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
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

    // Port #3, Signal USB-A Connecter port;
    Device(PRT3) {
      Name(_ADR, 0x00000003)
      
      Name(UPC1, Package(4) { 0xFF, 0x00, 0x00000000, 0x00000000 } ) //A0, Standard-A Connector, USB 2.0

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC1.RHUB._UPC\n", 0, 0, 0, 0, 0, 0)
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
          PLD_UserVisible = 0x01,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x07,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        Return (PLD1)
      }
    }

    // Port #4, USB-C with SS
    Device(PRT4) {
      Name(_ADR, 0x00000004)

      Name(UPC1, Package(4) { 0xFF, 0x09, 0x00000000, 0x00000000 } ) // A0, Type C connector - USB-C and SS with Switch

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
          PLD_UserVisible = 0x01,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x05,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
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

    Device(PRT5) {
      Name(_ADR, 0x00000005)

      Name(UPC1, Package(4) { 0xFF, 0x03, 0x00000000, 0x00000000 } ) // A0, Type A 3.0 connector

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
          PLD_UserVisible = 0x01,
          PLD_Dock = 0x00,
          PLD_GroupToken = 0x00,
          PLD_GroupPosition = 0x06,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
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
} // XHC1


#endif
