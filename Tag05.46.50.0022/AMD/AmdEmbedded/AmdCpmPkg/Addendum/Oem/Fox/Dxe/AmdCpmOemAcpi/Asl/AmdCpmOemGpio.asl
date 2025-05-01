/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

// GPIO SSDT
DefinitionBlock (
    "Gpio.aml",                     // Output file
    "SSDT",                         // Signature
    0x01,                           // SSDT Revision
    "AMD",                          // OEM ID
    "CPMGPIO0",                     // OEM Table ID = ('C','P','M','G','P','I','O','0')
    0x1                             // OEM Revision
    )
{
  #define CPM_GPIO_1_ASL

  #include "CmnDef.asi"
  #include "CmnExt.asi"
  #include "PostCode.asi"

  External (\_SB.GPIO, DeviceObj)
  External (\_SB.PWRB, DeviceObj)
  External (\_SB.PCI0.GPP0, DeviceObj)
  External (\_SB.PCI0.GPP1, DeviceObj)
  External (\_SB.PCI0.GPP2, DeviceObj)
  External (\_SB.PCI0.GPP3, DeviceObj)
  External (\_SB.PCI0.GPP4, DeviceObj)
  External (\_SB.PCI0.GPP5, DeviceObj)
  External (\_SB.PCI0.GPP6, DeviceObj)
  External (\_SB.PCI0.GPP7, DeviceObj)
  External (\_SB.PCI0.GP17.ACP, DeviceObj)
  External (\_SB.PCI0.GP17.AZAL, DeviceObj)
  External (\_SB.PCI0.GP17.XHC0, DeviceObj)
  External (\_SB.PCI0.GP17.XHC1, DeviceObj)
  External (\_SB.PCI0.GP17.MP2C, DeviceObj)

//[-start-220217-IB14740213-remove]//
//  External (\_SB.PCI0.LPC0.EC0, DeviceObj)
//  External (\_SB.PCI0.LPC0.EC0.OKEC, IntObj)
//  External (\_SB.PCI0.LPC0.EC0.CpmKbcMutex, MutexObj)
//[-end-220217-IB14740213-remove]//

//[-start-220217-IB14740213-remove]//
//  Scope (\_SB.PCI0.LPC0.EC0) {
//    OperationRegion (ECRM, EmbeddedControl, 0, 0xFF)
//    Field (ECRM, ByteAcc, NoLock, Preserve) {
//      Offset(0xCC),
//      PBCN, 8,
//    }
//  }
//[-end-220217-IB14740213-remove]//

  Scope (\_SB.GPIO) {
    Method (_AEI) {
      Name (BUF0, ResourceTemplate () {
        GpioInt (Edge,  ActiveHigh, ExclusiveAndWake, PullDefault, 5000,   "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {00}   // Index  0: Power button, PWR_BTN_L
        GpioInt (Level, ActiveHigh, ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {61}   // Index  1: through FCH input port AZ_FCH_AzWake
        GpioInt (Level, ActiveHigh, ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {62}   // Index  2: through FCH input port ACP_FCH_AzWake
        GpioInt (Level, ActiveHigh, ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {58}   // Index  3: Int_usb_xhc_0_acpi_pme
        GpioInt (Level, ActiveHigh, ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {59}   // Index  4: Int_usb_xhc_1_acpi_pme
        GpioInt (Edge,  ActiveLow,  ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {02}   // Index  5: PCIe DT x8/x2/x1 Slot
//        GpioInt (Edge,  ActiveLow,  ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {03}   // Index  6: PCIe SD Card Reader
        GpioInt (Edge,  ActiveLow,  ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {17}   // Index  7: PCIe WWAN
        GpioInt (Edge,  ActiveLow,  ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {18}   // Index  8: PCIe WLAN
        GpioInt (Edge,  ActiveLow,  ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {24}   // Index  9: PCIe GIGA LAN
        GpioInt (Edge,  ActiveHigh, ExclusiveAndWake, PullNone,    0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , ) {54}   // Index 10: MP2 wake up
      })

      CpmDebugPrint ("  OEM-ASL-\\_SB.GPIO._AEI\n", 0, 0, 0, 0, 0, 0)
      Return (BUF0)
    } // End of Method (_AEI)

    Method (_EVT, 0x1, Serialized) {          // Handle all ACPI Events signaled by GPIO Controller GPI2
      Name (HPDW, 0x55)
      CpmDebugPrint ("  OEM-ASL-\\_SB.GPIO._EVT-Start Case %d\n", ToInteger (Arg0), 0, 0, 0, 0, 0)
      Switch (ToInteger (Arg0)) {
        Case (0) {
//[-start-220217-IB14740213-remove]//
//          If (LEqual (\_SB.PCI0.LPC0.EC0.OKEC, 0x01)) {
//            Acquire (\_SB.PCI0.LPC0.EC0.CpmKbcMutex, 0xFFFF)
//            Store (\_SB.PCI0.LPC0.EC0.PBCN, Local0)
//            Release (\_SB.PCI0.LPC0.EC0.CpmKbcMutex)
//          }
//          And (Local0, 0x01, Local0)
//          if (LEqual (Local0, 0x01)) {
//            CpmSetPostCode (CpmTpMsGpio00InterruptEvent)
//            CpmDebugPrint ("    Notify (\\_SB.PWRB, 0x80)\n", 0, 0, 0, 0, 0, 0)
//            Notify (\_SB.PWRB, 0x80)
//          }
//[-end-220217-IB14740213-remove]//
        }
        Case (2) {
          CpmSetPostCode (CpmTpMsGpio02InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP0, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP0, 0x02)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP1, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP1, 0x02)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP2, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP2, 0x02)
        }
        Case (3) {
          CpmSetPostCode (CpmTpMsGpio02InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP7, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP7, 0x02)
        }
        Case (17) {
          CpmSetPostCode (CpmTpMsGpio17InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP2, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP2, 0x02)
        }
        Case (18) {
          CpmSetPostCode (CpmTpMsGpio18InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP6, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP6, 0x02)
        }
        Case (24) {
          CpmSetPostCode (CpmTpMsGpio24InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP5, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP5, 0x02)
        }
        Case (54) {
          CpmSetPostCode (CpmTpMsGpio54InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.MP2C, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GP17.MP2C, 0x02)
          If (LEqual (HPDW, 1)) {
            CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.MP2C(HPDW):PWRB 0x80)\n", 0, 0, 0, 0, 0, 0)
            Notify (\_SB.PWRB, 0x80)
          }
        }
        Case (58) {
          CpmSetPostCode (CpmTpMsGpio58InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.XHC0, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GP17.XHC0, 0x02)
        }
        Case (59) {
          CpmSetPostCode (CpmTpMsGpio59InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.XHC1, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GP17.XHC1, 0x02)
        }
        Case (42) {
          CpmSetPostCode (CpmTpMsGpio42InterruptEvent)
          //
          // At least check the SD_PRSNT# to High to Low
          // The Eage triggle meaning that there will a device plug-in (SD Express or Legacy SD PCIE device)
          //
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GPP7, 0x00)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GPP7, 0x00)
        }
        Case (61) {
          CpmSetPostCode (CpmTpMsGpio61InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.AZAL, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GP17.AZAL, 0x02)
        }
        Case (62) {
          CpmSetPostCode (CpmTpMsGpio61InterruptEvent)
          CpmDebugPrint ("    Notify (\\_SB.PCI0.GP17.ACP, 0x02)\n", 0, 0, 0, 0, 0, 0)
          Notify (\_SB.PCI0.GP17.ACP, 0x02)
        }
      }
      CpmDebugPrint ("  OEM-ASL-\\_SB.GPIO._EVT-End Case %d\n", ToInteger (Arg0), 0, 0, 0, 0, 0)
    } // End of Method (_EVT)
  }
}
