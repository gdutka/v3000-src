
DefinitionBlock (
         "FchKTUsb4.aml",           // Output Filename
         "SSDT",                   // Signature
          0x01,                    // DSDT Compliance Revision
         "AMD",                    // OEMID
         "KTU4",                  // OEM TABLE ID
         0x1000                    // OEM Revision
  )
{
  External(\_SB.PCI0, DeviceObj)
  External(\_SB.PCI0.GP19, DeviceObj)
  External(\_SB.A011, MethodObj)
  External(M000, MethodObj)
  
  OperationRegion(U4PC, SystemIo, 0x80, 0x04)
  Field(U4PC, AnyAcc, NoLock, Preserve)
  {
    PT80, 32,
  }

  Scope (\_SB)
  {

    Name(U4D3, 0x01)

    Method (_OSC, 4) {
      Name(CDWV, 0x5a)
      CreateDWordField (Arg3, 0, CDW1)
      CreateDWordField (Arg3, 4, CDW2)
      If (LEqual (CDWV, 0)) {
        Or (CDW1, 4, CDW1)
      } Else {
        If (LEqual (Arg0, ToUUID ("23A0D13A-26AB-486C-9C5F-0FFA525A575A"))) {
          CreateDWordField (Arg3, 8, CDW3)
          Store (CDWV, CDW3)      // Bit[0] USB Tunneling
                                  // Bit[1] DisplayPort Tunneling
                                  // Bit[2] PCI Express Tunneling
                                  // Bit[3] Inter-domain USB4
        } ElseIf (LEqual (Arg0, ToUUID ("0811B06E-4A27-44F9-8D60-3CBBC22E7B48"))) {
          Or (CDW2, 0x40000, CDW2)  // Bit[18] Native USB4 Support
        } Else {
          Or (CDW1, 4, CDW1)        // Unrecognized UUID
        }
      }
      Return (Arg3)
    }
  }

  Scope (\_SB.PCI0.GP19)
  {
    //
    //Host interface instance 0 (nBIF0Port2.Fn5)
    //
    Device (NHI0)
    {
      Name(_ADR, 0x00000005)

      Method (_RST, 0x0, NotSerialized) {
        // HIA Reset
        // Message to SMU
        // BIOSSMC_MSG_Usb4RouterHostReset 0x65

        Store (Buffer (24) {}, Local7)
        CreateDWordField (Local7, 0,  CDW0)
        CreateDWordField (Local7, 4,  CDW1)
        CreateDWordField (Local7, 8,  CDW2)
        CreateDWordField (Local7, 12, CDW3)
        CreateDWordField (Local7, 16, CDW4)
        CreateDWordField (Local7, 20, CDW5)
        Store (0, CDW0)

        M000 (0x10)
        Sleep (1)
        M000 (0x11)
        Sleep (1)
        M000 (0x12)
        Sleep (1)
        M000 (0x13)
        Sleep (1)
        M000 (0x14)
        Sleep (1)
        M000 (0x15)
        Sleep (1)
        \_SB.A011 (0x65, Local7)
      }

      Method(_S0W, 0) {
        if (LEqual(U4D3, one)) {
          Return(4)
        } else {
          Return(0)
        }
      }

      Method(_PS0, 0) {
        if (LEqual(U4D3, one)) {
        }
      }

      Method(_PS3, 0) {
        if (LEqual(U4D3, one)) {
          // BIOSSMC_MSG_Usb4PS3SleepEvent     0x61

          Store (Buffer (24) {}, Local7)
          CreateDWordField (Local7, 0,  CDW0)
          CreateDWordField (Local7, 4,  CDW1)
          CreateDWordField (Local7, 8,  CDW2)
          CreateDWordField (Local7, 12, CDW3)
          CreateDWordField (Local7, 16, CDW4)
          CreateDWordField (Local7, 20, CDW5)
          Store (1, CDW0)
          M000 (0x20)
          Sleep (1)
          M000 (0x21)
          Sleep (1)
          M000 (0x22)
          Sleep (1)
          M000 (0x23)
          Sleep (1)
          M000 (0x24)
          Sleep (1)
          M000 (0x25)
          Sleep (1)
          \_SB.A011 (0x61, Local7)
          M000 (0x28)
          Sleep (1)
          M000 (0x29)
          Sleep (1)
          M000 (0x2A)
          Sleep (1)
          M000 (0x2B)
          Sleep (1)
          M000 (0x2C)
          Sleep (1)
          M000 (0x2D)
          Sleep (1)
        }
      }
    }

    Device (XHC3) //Tunneled XHCI instance (nBIF0Port2.Fn3)
    {
      Name(_ADR, 0x00000003)

      Device(RHUB)
      {
        Name(_ADR, 0x00000000)

        // xHCI Port #1 - USB2.0, Stack USB Connecter port;
        Device(PRT1) {
          Name(_ADR, 0x00000001)
          Name(UPC1, Package(){
              0xFF,         // Port is connetable;
              0x09,         // Type C;
              0x0000003D,   // Retimer:1, TBT3 or PCIE Tunneling, DP Alt Mode, TPT3 Support
              0x00000000    // Reserved 1;
          })

          Name(PLD1, Package(1){
            Buffer(0x14) {
              0x82,                   // Revision 2, Ignore color;
              0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
              0x00, 0x00, 0x00, 0x00, // Width & Height;
              0x69, 0x90, 0x00, 0x05, //User visible, Group Token =0;
                                      // Group Position 10;
              0x01, 0x00, 0x00, 0x00,
              0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            }

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

        // xHCI Port #2 - USB3 port
        Device(PRT2) {
          Name(_ADR, 0x00000002)
          Name(UPC2, Package(){
              0xFF,         // Port is connetable;
              0x09,         // Type C;
              0x0000003D,   // Retimer:1, TBT3 or PCIE Tunneling, DP Alt Mode, TPT3 Support
              0x00000000    // Reserved 1;
          })

          Name(PLD2, Package(1){
            Buffer(0x14) {
              0x82,                   // Revision 2, Ignore color;
              0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
              0x00, 0x00, 0x00, 0x00, // Width & Height;
              0x69, 0x90, 0x00, 0x05, //User visible, Group Token =0;
                                      // Group Position 10;
              0x01, 0x00, 0x00, 0x00,
              0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            }

          })

          Method (_UPC,0,Serialized)
          {
            Return (UPC2)
          }

          Method (_PLD,0,Serialized)
          {
            Return (PLD2)
          }

          Name (_DSD, Package () {
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), //Device Properties UUID
            Package () {
              Package () { "usb4-host-interface", \_SB.PCI0.GP19.NHI0 },
              Package () { "usb4-port-number", 1},
            }
          })
        }
      }
    }

    Device (NHI1)
    {
      Name(_ADR, 0x00000006)

      Method (_RST, 0x0, NotSerialized) {
        // HIA Reset
        // Message to SMU
        // BIOSSMC_MSG_Usb4RouterHostReset 0x65
        Store (Buffer (24) {}, Local7)
        CreateDWordField (Local7, 0,  CDW0)
        CreateDWordField (Local7, 4,  CDW1)
        CreateDWordField (Local7, 8,  CDW2)
        CreateDWordField (Local7, 12, CDW3)
        CreateDWordField (Local7, 16, CDW4)
        CreateDWordField (Local7, 20, CDW5)
        Store (1, CDW0)
        M000 (0x30)
        Sleep (1)
        M000 (0x31)
        Sleep (1)
        M000 (0x32)
        Sleep (1)
        M000 (0x33)
        Sleep (1)
        M000 (0x34)
        Sleep (1)
        M000 (0x35)
        Sleep (1)
        \_SB.A011 (0x65, Local7)
      }

      Method(_S0W, 0) {
        if (LEqual(U4D3, one)) {
          Return(4)
        } else {
          Return(0)
        }
      }

      Method(_PS0, 0) {
        if (LEqual(U4D3, one)) {
        }
      }

      Method(_PS3, 0) {
        if (LEqual(U4D3, one)) {
          Store (Buffer (24) {}, Local7)
          CreateDWordField (Local7, 0,  CDW0)
          CreateDWordField (Local7, 4,  CDW1)
          CreateDWordField (Local7, 8,  CDW2)
          CreateDWordField (Local7, 12, CDW3)
          CreateDWordField (Local7, 16, CDW4)
          CreateDWordField (Local7, 20, CDW5)
          Store (2, CDW0)
          M000 (0x40)
          Sleep (1)
          M000 (0x41)
          Sleep (1)
          M000 (0x42)
          Sleep (1)
          M000 (0x43)
          Sleep (1)
          M000 (0x44)
          Sleep (1)
          M000 (0x45)
          Sleep (1)
          \_SB.A011 (0x61, Local7)
          M000 (0x48)
          Sleep (1)
          M000 (0x49)
          Sleep (1)
          M000 (0x4A)
          Sleep (1)
          M000 (0x4B)
          Sleep (1)
          M000 (0x4C)
          Sleep (1)
          M000 (0x4D)
          Sleep (1)
        }
      }
    }

    Device (XHC4) //Tunneled XHCI instance (nBIF0Port2.Fn4)
    {
      Name(_ADR, 0x00000004)

      Device(RHUB)
      {
        Name(_ADR, 0x00000000)

        // xHCI Port #1, USB 2.0, Stack USB Connecter port;
        Device(PRT1) {
          Name(_ADR, 0x00000001)
          Name(UPC3, Package(){
              0xFF,         // Port is connetable;
              0x09,         // Type C;
              0x0000003D,   // Retimer:1, TBT3 or PCIE Tunneling, DP Alt Mode, TPT3 Support
              0x00000000    // Reserved 1;
          })

          Name(PLD3, Package(1){
            Buffer(0x14) {
              0x82,                   // Revision 2, Ignore color;
              0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
              0x00, 0x00, 0x00, 0x00, // Width & Height;
              0x69, 0x90, 0x80, 0x05, //User visible, Group Token =0;
                                      // Group Position 11;
              0x01, 0x00, 0x00, 0x00,
              0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            }

          })

          Method (_UPC,0,Serialized)
          {
            Return (UPC3)
          }

          Method (_PLD,0,Serialized)
          {
            Return (PLD3)
          }
        }

        // xHCI Port #2 - USB3 port
        Device(PRT2) {
          Name(_ADR, 0x00000002)
          Name(UPC4, Package(){
              0xFF,         // Port is connetable;
              0x09,         // Type C;
              0x0000003D,   // Retimer:1, TBT3 or PCIE Tunneling, DP Alt Mode, TPT3 Support
              0x00000000    // Reserved 1;
          })

          Name(PLD4, Package(1){
            Buffer(0x14) {
              0x82,                   // Revision 2, Ignore color;
              0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
              0x00, 0x00, 0x00, 0x00, // Width & Height;
              0x69, 0x90, 0x80, 0x05, //User visible, Group Token =0;
                                      // Group Position 10;
              0x01, 0x00, 0x00, 0x00,
              0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            }

          })

          Method (_UPC,0,Serialized)
          {
            Return (UPC4)
          }

          Method (_PLD,0,Serialized)
          {
            Return (PLD4)
          }

          Name (_DSD, Package () {
            ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), //Device Properties UUID
            Package () {
              Package () { "usb4-host-interface", \_SB.PCI0.GP19.NHI1 },
              Package () { "usb4-port-number", 1},
            }
          })
        }
      }
    }
  }

  Scope (\_SB.PCI0)
  {
    Device (DSB0) //Tunneled PCIe port instance (PCIE2 Dev3 Fn1)
    {
      Name(_ADR, 0x00030001)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), //Device Properties UUID
        Package () {
          Package () { "usb4-host-interface", \_SB.PCI0.GP19.NHI0 },
          Package () { "usb4-port-number", 1},
        }
      })

      Method (_DSM, 0x4, Serialized) {
        If (LEqual (Arg0, ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
          Switch (ToInteger(Arg2)) {

            //
            // Function 05h:
            //   PCI Firmware v3.2: Ignoring PCI Boot Configurations
            //   PCI Firmware v3.3: Preserving PCI Boot Configurations
            //
            // Return:
            //    0h - The operating system shall not ignore the PCI configuration
            //         that firmware has done at boot time. However, the operating system
            //         is free to configure the devices in this hierarchy that have not
            //         been configured by the firmware. There may be a reduced level of
            //         hot plug capability support in this hierarchy due to resource
            //         constraints. This situation is the same as the legacy situation
            //         where this _DSM is not provided.
            //    1h - The operating system may ignore the PCI configuration that
            //         the firmware has done at boot time, and reconfigure/rebalance the
            //         resources in the hierarchy.
            Case (0x05) {
              If (LGreaterEqual(Arg1, 1)) { // check Arg1 for revision
                Return (0x1)
              }
            }

            //
            // Function 06h: Latency Tolerance Reporting
            //
            // Return:
            //   A Package of four integers corresponding with the LTR encoding
            //   defined in the PCI Express Base Specification, as follows:
            //     Integer 0: Maximum Snoop Latency Scale
            //     Integer 1: Maximum Snoop Latency Value
            //     Integer 2: Maximum No-Snoop Latency Scale
            //     Integer 3: Maximum No-Snoop Latency Value
            //   These values correspond directly to the LTR Extended Capability Structure fields
            //   described in the PCI Express Base Specification.
            Case(0x06) {
              if (LGreaterEqual(Arg1, 2)) { // check Arg1 for revision
                Return (Package() {0x02, 0xC8, 0x02, 0xC8})
              }
            }

          } // end of Switch (ToInteger(Arg2))
        } // end of checking ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D")
      } // end of Method (_DSM...

    }

    Device (DSB1) //Tunneled PCIe port instance (PCIE3 Dev4 Fn1)
    {
      Name(_ADR, 0x00040001)
      Name (_DSD, Package () {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"), //Device Properties UUID
        Package () {
          Package () { "usb4-host-interface", \_SB.PCI0.GP19.NHI1 },
          Package () { "usb4-port-number", 1},
        }
      })

      Method (_DSM, 0x4, Serialized) {
        If (LEqual (Arg0, ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D"))) {
          Switch (ToInteger(Arg2)) {

            //
            // Function 05h:
            //   PCI Firmware v3.2: Ignoring PCI Boot Configurations
            //   PCI Firmware v3.3: Preserving PCI Boot Configurations
            //
            // Return:
            //    0h - The operating system shall not ignore the PCI configuration
            //         that firmware has done at boot time. However, the operating system
            //         is free to configure the devices in this hierarchy that have not
            //         been configured by the firmware. There may be a reduced level of
            //         hot plug capability support in this hierarchy due to resource
            //         constraints. This situation is the same as the legacy situation
            //         where this _DSM is not provided.
            //    1h - The operating system may ignore the PCI configuration that
            //         the firmware has done at boot time, and reconfigure/rebalance the
            //         resources in the hierarchy.
            Case (0x05) {
              If (LGreaterEqual(Arg1, 1)) { // check Arg1 for revision
                Return (0x1)
              }
            }

            //
            // Function 06h: Latency Tolerance Reporting
            //
            // Return:
            //   A Package of four integers corresponding with the LTR encoding
            //   defined in the PCI Express Base Specification, as follows:
            //     Integer 0: Maximum Snoop Latency Scale
            //     Integer 1: Maximum Snoop Latency Value
            //     Integer 2: Maximum No-Snoop Latency Scale
            //     Integer 3: Maximum No-Snoop Latency Value
            //   These values correspond directly to the LTR Extended Capability Structure fields
            //   described in the PCI Express Base Specification.
            Case(0x06) {
              if (LGreaterEqual(Arg1, 2)) { // check Arg1 for revision
                Return (Package() {0x02, 0xC8, 0x02, 0xC8})
              }
            }

          } // end of Switch (ToInteger(Arg2))
        } // end of checking ToUUID("E5C937D0-3553-4D7A-9117-EA4D19C3434D")
      } // end of Method (_DSM...

    }
  }
}

