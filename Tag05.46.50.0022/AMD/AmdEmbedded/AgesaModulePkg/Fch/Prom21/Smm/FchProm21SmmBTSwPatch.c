/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Protocol/SmmBase2.h>
#include <Library/debugLib.h>

#include <Library/FchBaseLib.h>
#include <Filecode.h>
//#include <Protocol/FchSmmPwrBtnDispatch2.h>

#include <Library/FchProm21Common.h>
#include "FchProm21Smm.h"

#define FILECODE FCH_PROM21_SMM_FCHPROM21SMMBTSWPATCH_FILECODE

extern UINT32                       GppPcieAddr;

EFI_STATUS
PT21SmiBTSwPatch (
  VOID
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT8           PT21Bus = 0;
  UINT8           PT21CmdReg = 0;
  UINT8           RetryTimer = 20;
  UINT32          TempValue = 0;
  UINT64          XhciMmioAddr = 0;

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-Start\n", __FUNCTION__));

  TempValue = PciRead32 (GppPcieAddr);
  DEBUG ((DEBUG_INFO, "PT-SMM-%a-GppPcieAddr = 0x%X PT21GppDeviceId = 0x%X\n", __FUNCTION__, GppPcieAddr, TempValue));

  if (TempValue != 0xFFFFFFFF) {
    //
    // PTPlus Upper Gpp Pcie Bridge exist
    //
    PT21Bus = PciRead8 (GppPcieAddr + 0x19);
    if (PT21Bus != 0x00) {
      DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21Bus = 0x%X\n", __FUNCTION__, PT21Bus));
      TempValue = PciRead32 (PCI_LIB_ADDRESS (PT21Bus, 0, 0, 0x0));
      if ((TempValue != 0) && (TempValue != 0xFFFFFFFF)) {
        if ((TempValue == PT21_XHCI_ID_L1) || (TempValue == PT21_XHCI_ID_L2) || (TempValue == PT21_XHCI_ID_L3)) {
          //
          // PT XHCI exist, save XHCI Command Register, and enable XHCI Command BusMasterEn & MemAccessEn Bit
          //
          DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21XhciDeviceId = 0x%X\n", __FUNCTION__, TempValue));
          PT21CmdReg = PciRead8 (PCI_LIB_ADDRESS (PT21Bus, 0, 0, 0x4));
          PciWrite8 (PCI_LIB_ADDRESS (PT21Bus, 0, 0, 0x4), PT21CmdReg | (BIT2 + BIT1));
          XhciMmioAddr = (UINT64) PciRead32 (PCI_LIB_ADDRESS (PT21Bus, 0, 0, 0x14));
          XhciMmioAddr <<= 32;
          XhciMmioAddr |= (UINT64) PciRead32 (PCI_LIB_ADDRESS (PT21Bus, 0, 0, 0x10));
          XhciMmioAddr &= 0xFFFFFFFFFFFFFFF0;
          if (XhciMmioAddr != 0) {
            TempValue = MmioRead32 (XhciMmioAddr + 0x20);
            DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21 MmioAddr = 0x%LX USBCMD = 0x%X\n", __FUNCTION__, XhciMmioAddr, TempValue));
            if ((TempValue != 0) && (TempValue != 0xFFFFFFFF)) {
              do {
                //
                // Host Controller Capability Registers can be accessed
                // Clear USBCMD Register Bit0 Run/Stop bit, and wait 100ms
                // Check USBSTS Register Bit0 HCHalted bit to 1, and exit if 2sec timeout
                //
                MmioAndThenOr32 (XhciMmioAddr + 0x20, 0xFFFFFFFE, 0x00);
                FchStall (100000, NULL);
                TempValue = MmioRead32 (XhciMmioAddr + 0x24) & BIT0;
                RetryTimer--;
              } while ((TempValue == 0) && (RetryTimer > 0));
              DEBUG ((DEBUG_INFO, "PT-SMM-%a-PT21 USBSTS = 0x%X\n", __FUNCTION__, TempValue));
              DEBUG ((DEBUG_INFO, "PT-SMM-%a-Clear Run_Stop RetryTimer = %d ms\n", __FUNCTION__, (20 - RetryTimer) * 100));
            } // End of if ((TempValue != 0) && (TempValue != 0xFFFFFFFF))
          }
          //
          // Restore XHCI Command Register
          //
          PciWrite8 (PCI_LIB_ADDRESS (PT21Bus, 0, 0, 0x4), PT21CmdReg);
        } // End of if ((TempValue == PROM19_XHCI_A) || (TempValue == PROM19_XHCI_C) || (TempValue == PROM19_XHCI_D))
      } // End of if ((TempValue != 0) && (TempValue != 0xFFFFFFFF))
    } // End of if (PTPlusBus != 0x00)
  } // End of if (TempValue != 0xFFFFFFFF)

  DEBUG ((DEBUG_INFO, "PT-SMM-%a-End-Status = %r\n", __FUNCTION__, Status));
  return Status;
}

