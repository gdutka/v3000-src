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

Device(BATT)
{
  Name(_HID, EISAID("PNP0C0A"))
  Name(_UID, 0x01)
  Name(_PCL, Package(0x01){
    \_SB
  })

  Name(PBIX, Package(0x15){
    0x01,        // 0x00, Revision - current revision: 1 (ACPI 6.2)
    0x00,        // 0x01, Power Unit - 0:mW, 1:mA
    3420,        // 0x02, Design Capacity - 3420 mAh;              reg 0x18 F_BAT_REG_DesignCapacity
    3400,        // 0x03, Last Full Charge Capacity                reg 0x10 F_BAT_REG_FullChargeCapacity
    0x01,        // 0x04, Battery Technology - rechargeable
    15400,       // 0x05, Design Voltage - 15400 mV;               reg 0x19 F_BAT_REG_DesignVoltage
    500,         // 0x06, Design Capacity of Warning
    300,         // 0x07, Design Capacity of Low
    0xFFFFFFFF,  // 0x08, Cycle Count                              reg 0x17 F_BAT_REG_CycleCount
    95000,       // 0x09, Measurement Accuracy - 95.000%
    600000,      // 0x0A, Max Sampling Time - 10 mins
    30000,       // 0x0B, Min Sampling Time - 30 s
    60000,       // 0x0C, Max Averaging Interval - 1 min
    60000,       // 0x0D, Min Averaging Interval - 1 min
    0x00000001,  // 0x0E, Battery Capacity Granularity 1 - low to warning
    0x00000001,  // 0x0F, Battery Capacity Granularity 2 - waring to full
    "BA-05",     // 0x10, Model Number
    "486298-01", // 0x11, Serial Number
    "Li-ion",    // 0x12, Battery Type
    "AMD",       // 0x13, OEM Information
    0x00000000   // 0x14, Battery swapping capability - Non swappable
  })

#ifdef EC_ACPI_REGISTER_DUMP
  #include "EcDbg.asl"
#endif

  Method(_BTP,1,NotSerialized)
  {
    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BTP Arg0 = 0x%X\n", ToInteger(Arg0), 0, 0, 0, 0, 0)
    If(\_SB.PCI0.LPC0.ECOK())
    {
      if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009, 300))) {

        if (LEqual(Arg0,0)) {
          Store(0, Local0)
          Store(0, Local1)
        }
        Else
        {
          And(Arg0, 0xFF, Local0)
          ShiftRight(Arg0, 8, Local1)
          And(Local1, 0xFF, Local1)
        }

        Store (Local0, \_SB.PCI0.LPC0.EC0.BTPL)
        Store (Local1, \_SB.PCI0.LPC0.EC0.BTPH)

        #ifdef EC_ACPI_REGISTER_DUMP
          store ("_BTP", debug)
          ECDB()
        #endif

        Release(\_SB.PCI0.LPC0.EC0.Z009)
      }
    }
  }

  Name(PBST, Package(0x04){
    0x00000000, // [0] - discharging; [1] - charging; [2] - critical;
    0,          // Current (mA)             - reg 0x0A F_BAT_REG_Current; must be 0x0 to 0x7FFFFFFFF; direction depends on the Battery Statue value
    2800,       // remaining capacity (mAh) - reg 0x0F F_BAT_REG_RemainingCapacity
    14500       // Voltage (mV)             - reg 0x09 F_BAT_REG_Voltage
  })

  Method(_STA, 0, NotSerialized)
  {
    If(\_SB.PCI0.LPC0.ECOK())
    {
      if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {
        Store(\_SB.PCI0.LPC0.EC0.CHSH, Local0)
        Release(\_SB.PCI0.LPC0.EC0.Z009)

        If(And(Local0, 0x40)) {
          CpmDebugPrint ("PLA-ASL-\\_SB.BATT._STA = 0x1F\n", 0, 0, 0, 0, 0, 0)
          Return(0x1F) // has battery
        } Else {
          CpmDebugPrint ("PLA-ASL-\\_SB.BATT._STA = 0x0F\n", 0, 0, 0, 0, 0, 0)
          Return(0x0F)
        }
      }
    }

    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._STA 1 = 0x0F\n", 0, 0, 0, 0, 0, 0)
    Return(0x0F)
  }

  Method(_BIX, 0, NotSerialized)
  {
    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BIX Start\n", 0, 0, 0, 0, 0, 0)
    If(\_SB.PCI0.LPC0.ECOK())
    {
      if (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {

        Store(\_SB.PCI0.LPC0.EC0.CHSH, Local0)
        If( LNot ( And(Local0, 0x40) ) ) {
          // Battery is not present, restore to the default
          Store(3420,       Index(PBIX, 0x02)) // 0x02, Design Capacity
          Store(3400,       Index(PBIX, 0x03)) // 0x03, Last Full Charge Capacity
          Store(15400,      Index(PBIX, 0x05)) // 0x05, Design Voltage
          Store(0xFFFFFFFF, Index(PBIX, 0x08)) // 0x08, Cycle Count
          Store(95000,      Index(PBIX, 0x09)) // 0x09, Measurement Accuracy

          Release(\_SB.PCI0.LPC0.EC0.Z009)
          CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BIX End 1\n", 0, 0, 0, 0, 0, 0)
          Return(PBIX)
        }

        // F_BAT_REG_DesignCapacity   DECL/DECH
        Store(\_SB.PCI0.LPC0.EC0.DECH, Local1)
        ShiftLeft(Local1, 8, Local1)
        And(Local1, 0xFF00, Local1)
        Store(\_SB.PCI0.LPC0.EC0.DECL, Local0)
        And(Local0, 0x00FF, Local0)
        Or(Local1, Local0, Local0)
        Store(Local0, Index(PBIX, 0x02)) // 0x02, Design Capacity

        // F_BAT_REG_FullChargeCapacity LFCL/LFCH
        Store(\_SB.PCI0.LPC0.EC0.LFCH, Local1)
        ShiftLeft(Local1, 8, Local1)
        And(Local1, 0xFF00, Local1)
        Store(\_SB.PCI0.LPC0.EC0.LFCL, Local0)
        And(Local0, 0x00FF, Local0)
        Or(Local1, Local0, Local0)
        Store(Local0, Index(PBIX, 0x03)) // 0x03, Last Full Charge Capacity

        // F_BAT_REG_DesignVoltage DEVL/DEVH
        Store(\_SB.PCI0.LPC0.EC0.DEVH, Local1)
        ShiftLeft(Local1, 8, Local1)
        And(Local1, 0xFF00, Local1)
        Store(\_SB.PCI0.LPC0.EC0.DEVL, Local0)
        And(Local0, 0x00FF, Local0)
        Or(Local1, Local0, Local0)
        Store(Local0, Index(PBIX, 0x05)) // 0x05, Design Voltage

        // F_BAT_REG_CycleCount  BCCL/BCCH
        Store(\_SB.PCI0.LPC0.EC0.BCCH, Local1)
        ShiftLeft(Local1, 8, Local1)
        And(Local1, 0xFF00, Local1)
        Store(\_SB.PCI0.LPC0.EC0.BCCL, Local0)
        And(Local0, 0x00FF, Local0)
        Or(Local1, Local0, Local0)
        Store(Local0, Index(PBIX, 0x08)) // 0x08, Cycle Count

        // Measurement Accuracy  MERL/MERH
        Store(\_SB.PCI0.LPC0.EC0.MERH, Local1)
        ShiftLeft(Local1, 8, Local1)
        And(Local1, 0xFF00, Local1)
        Store(\_SB.PCI0.LPC0.EC0.MERL, Local0)
        And(Local0, 0x00FF, Local0)
        Or(Local1, Local0, Local0)
        ShiftLeft(Local0, 8, Local0)     // EC reports an RShift 8 value, shift it back here
        Store(Local0, Index(PBIX, 0x09)) // 0x09, Measurement Accuracy

#ifdef EC_ACPI_REGISTER_DUMP
        store ("_BIX():", debug)
        ECDB()
#endif
        Release(\_SB.PCI0.LPC0.EC0.Z009)
      }
    }
    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BIX End 2\n", 0, 0, 0, 0, 0, 0)
    Return(PBIX)
  }

  Method(_BST, 0, NotSerialized)
  {
    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BST Start\n", 0, 0, 0, 0, 0, 0)
    If(\_SB.PCI0.LPC0.ECOK())
    {
      If (LNot(Acquire(\_SB.PCI0.LPC0.EC0.Z009,300))) {

        Store(\_SB.PCI0.LPC0.EC0.CHSH, Local0)
        If( LNot ( And(Local0, 0x40) ) ) {
          // Battery is not present, restore to the default
          Store(0x00000000, Index(PBST, 0x00)) // 0x00, [0] - discharging; [1] - charging; [2] - critical;
          Store(0,          Index(PBST, 0x01)) // 0x01, Current (mA)
          Store(2800,       Index(PBST, 0x02)) // 0x02, remaining capacity (mAh)
          Store(14500,      Index(PBST, 0x03)) // 0x03, Voltage (mV)

          Release(\_SB.PCI0.LPC0.EC0.Z009)
          CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BST End 1\n", 0, 0, 0, 0, 0, 0)
          Return(PBST)
        }

        //   F_BAT_REG_RemainingCapacity
        Store(\_SB.PCI0.LPC0.EC0.CAPH, Local1) // MSB of battery remaining capacity; read to lock low byte
        Store(\_SB.PCI0.LPC0.EC0.CAPL, Local0) // LSB of battery remaining capacity

        And (Local0, 0x00FF, Local0)
        ShiftLeft(Local1, 8, Local1)
        And (Local1, 0xFF00, Local1)
        Or  (Local0, Local1, Local6)           // Local6 is remaining capacity

        Store(Local6, Index(PBST, 0x02))

        //   F_BAT_REG_Current
        Store(\_SB.PCI0.LPC0.EC0.BTCH, Local1) // MSB of current value; read to lock low byte
        Store(\_SB.PCI0.LPC0.EC0.BTCL, Local0) // LSB of current value

        And (Local0, 0x00FF, Local0)
        ShiftLeft(Local1, 8, Local1)
        And (Local1, 0xFF00, Local1)
        Or  (Local0, Local1, Local5)           // Local5 is current
        If ( And (Local5, 0x8000) ) {
          And(Not(Local5), 0x7FFF, Local5)
          Store(0x00000001, Local4)            // discharging
        } ElseIf ( LEqual (Local5, 0x0000) ) {
          Store(0x00000000, Local4)            // idle
        } Else {
          Store(0x00000002, Local4)            // charging
        }
        If ( LLess (Local6, 50) ) {            // critical flag
          Or(Local4, 0x00000004, Local4)
        }

        Store(Local4, Index(PBST, 0x00))
        Store(Local5, Index(PBST, 0x01))

        //   F_BAT_REG_Voltage
        Store(\_SB.PCI0.LPC0.EC0.BTVH, Local1) // LSB of voltage value; read to lock low byte
        Store(\_SB.PCI0.LPC0.EC0.BTVL, Local0) // LSB of voltage value

        And (Local0, 0x00FF, Local0)
        ShiftLeft(Local1, 8, Local1)
        And (Local1, 0xFF00, Local1)
        Or  (Local0, Local1, Local7)           // Local7 is voltage

        Store(Local7, Index(PBST, 0x03))

#ifdef EC_ACPI_REGISTER_DUMP
        store ("_BST():", debug)
        ECDB()
#endif
        Release(\_SB.PCI0.LPC0.EC0.Z009)
      }
    }
    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._BST End 2\n", 0, 0, 0, 0, 0, 0)
    Return(PBST)
  }

  Name(BCTR, 0x01) // Battery charge throttle for _DSM Fun 1

  //
  // see https://docs.microsoft.com/en-us/windows-hardware/design/component-guidelines/battery-and-charging
  //
  // Arg0:   UUID      Unique function identifier
  // Arg1:   Integer   Revision ID
  // Arg2:   Integer   Function Index (0 = Return Supported Functions)
  // Arg3:   Package   Parameters
  //
  Method (_DSM, 4, Serialized) {
    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._DSM Start\n", 0, 0, 0, 0, 0, 0)
    // UUID: 4c2067e3-887d-475c-9720-4af1d3ed602e
    // Revision ID: 0
    If (LAnd (LEqual (Arg0, ToUUID ("4c2067e3-887d-475c-9720-4af1d3ed602e")), LEqual (ToInteger (Arg1), 0))) {
      CpmDebugPrint ("PLA-ASL-\\_SB.BATT._DSM UUID = 4c2067e3\n", 0, 0, 0, 0, 0, 0)
      //
      // Fun 0: To return supportted functions (2,3)
      //
      If (LEqual (ToInteger (Arg2), 0)) {
        CpmDebugPrint ("PLA-ASL-\\_SB.BATT._DSM Return = 0xD\n", 0, 0, 0, 0, 0, 0)
         Return( Buffer(One){0x0D} )

      //
      // Fun 1: Set battery charge throttle
      //
      } ElseIf (LEqual (ToInteger (Arg2), 1)) {
        Store (Arg3, BCTR)

        And (BCTR, 0xFF, Local0)
        Store (Local0, \_SB.PCI0.LPC0.EC0.BCTR)

      //
      // Fun 2: to determine if the battery device
      //        is user-serviceable or not.
      //
      } ElseIf (LEqual (ToInteger (Arg2), 2)) {
        // 0x0 if the battery is not user-serviceable and cannot
        // be replaced by the end user, or can be replaced by the
        // end user with additional tools.
        CpmDebugPrint ("PLA-ASL-\\_SB.BATT._DSM Return = 0x0\n", 0, 0, 0, 0, 0, 0)
        Return( Package(One) {0x0} )

      //
      // Fun 3: to determine if the control method battery requires
      //        periodic watchdog resetting to maintain high current
      //        charging and the period at which the watchdog must
      //        be reset
      //
      } ElseIf (LEqual (ToInteger (Arg2), 3)) {
        CpmDebugPrint ("PLA-ASL-\\_SB.BATT._DSM Return 1 = 0x0\n", 0, 0, 0, 0, 0, 0)
        Return( Package(One) {0x0} )
      }
    }

    CpmDebugPrint ("PLA-ASL-\\_SB.BATT._DSM Return 2 = 0x0\n", 0, 0, 0, 0, 0, 0)
    Return( Buffer(One){0x0} )
  }
}

