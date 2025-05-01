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

#ifndef _XHCI0_
#define _XHCI0_

Device(XHC0)
{
  Name(_ADR, 0x00000003)

  // Method(_PRW,0,NotSerialized) {
  //   CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0._PRW Return GPRW (0x19, 0x4)\n", 0, 0, 0, 0, 0, 0)
  //   Return (GPRW (0x19,0x04))
  // }

  Device(RHUB)
  {
    Name(_ADR, 0x00000000)

    // Port #1, Stack USB Connecter port; 
    // map to Port #5 (Type-c)
    Device(PRT1) 
    {
      Name(_ADR, 0x00000001)

      Name(UPC1, Package(4) { 0x0, 0x00, 0x00000000, 0x00000000 }) //Not used after B0 revision

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT1._UPC\n", 0, 0, 0, 0, 0, 0)
        Return (UPC1)
      }

//      Name(PLD1, Package() {
//        Buffer(0x14) {
//          0x82,                   // Revision 2, Ignore color;
//          0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
//          0x00, 0x00, 0x00, 0x00, // Width & Height;
//          0x11, 0x0C, 0x80, 0x00, // User visible, Group Token =0;
//                                  // Group Position 1st;
//          0x01, 0x00, 0x00, 0x00,
//          0xFF, 0xFF, 0xFF, 0xFF  // Reserve
//        }
//      })
      
      // A0
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
          PLD_GroupPosition = 0x01,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT1_PLD\n", 0, 0, 0, 0, 0, 0)

        Return (PLD1)
      }
    }

    // Port #2,;  
    // Map to Port #6 (USB-C);
    Device(PRT2) {
      Name(_ADR, 0x00000002)

      Name(UPC1, Package(4) { 0x0, 0x00, 0x00000000, 0x00000000 } ) //Not used after B0 revision

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT2._UPC\n", 0, 0, 0, 0, 0, 0)
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
          PLD_GroupPosition = 0x02,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT2._PLD\n", 0, 0, 0, 0, 0, 0)
        Return (PLD1)
      }
    }

    // Port #3, Single USB Connecter port;
    Device(PRT3) {
      Name(_ADR, 0x00000003)

      Name(UPC1, Package(4) { 0xFF, 0x00, 0x00000000, 0x00000000 } ) //A0, Standard-A Connector, USB 2.0

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT3._UPC\n", 0, 0, 0, 0, 0, 0)
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
          PLD_GroupPosition = 0x03,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT3._PLD\n", 0, 0, 0, 0, 0, 0)
        Return (PLD1)
      }
    }

    // Port #4
    Device(PRT4) {
      Name(_ADR, 0x00000004)

      Name(UPC1, Package(4) { 0xFF, 0x00, 0x00000000, 0x00000000 } ) //A0, Standard-A Connector, USB 2.0

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT4._UPC\n", 0, 0, 0, 0, 0, 0)
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
          PLD_GroupPosition = 0x04,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT4._PLD\n", 0, 0, 0, 0, 0, 0)
        Return (PLD1)
      }
    }

    // Port #5 , USB-C with SS
    Device(PRT5) {
      Name(_ADR, 0x00000005)

      Name(UPC1, Package(4) { 0x0, 0x00, 0x00000000, 0x00000000 } ) //Not used after B0 revision

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT5._UPC\n", 0, 0, 0, 0, 0, 0)
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
          PLD_GroupPosition = 0x01,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT5._PLD\n", 0, 0, 0, 0, 0, 0)
        Return (PLD1)
      }
    }

    // Port #6, USB-C with SS
    Device(PRT6) {
      Name(_ADR, 0x00000006)

      Name(UPC1, Package(4) { 0x0, 0x00, 0x00000000, 0x00000000 } ) //Not used after B0 revision

      Method (_UPC,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT6._UPC\n", 0, 0, 0, 0, 0, 0)
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
          PLD_GroupPosition = 0x02,
          PLD_Ejectable = 0x01,
          PLD_EjectRequired = 0x01 )
      })

      Method (_PLD,0,Serialized)
      {
        CpmDebugPrint ("PLA-ASL-\\_SB.PCI0.GP17.XHC0.RHUB.PRT6._PLD\n", 0, 0, 0, 0, 0, 0)
        Return (PLD1)
      }
    }
  }
} // XHC0


#endif
