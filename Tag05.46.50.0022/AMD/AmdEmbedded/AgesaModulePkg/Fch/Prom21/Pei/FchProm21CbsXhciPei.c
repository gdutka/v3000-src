/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>

#include <Filecode.h>

#include <Ppi/FchProm21InitPpi.h>
#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include "FchProm21CbsPei.h"

#define FILECODE FCH_PROM21_PEI_FCHPROM21CBSXHCIPEI_FILECODE

VOID FchProm21XhciGen (
IN       UINT32   XhciMmio,
IN       BOOLEAN  Gen2En,
IN       UINT8    PortNum
)
{
  UINT32 Address;
  UINT8  Value8;

  Address = r_force_superspeed[PortNum];

  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X @ Address 0x%08X\n", __FUNCTION__, Value8, Address));

  switch(Gen2En)
  {
    case 1:
      Value8 = 0x7;
      break;
    case 0:
      Value8 = 0x4;
      break;
    default:
      Value8 = 0x3;
      break;
  }
  FchProm21XhciWriteByte(XhciMmio, Address, Value8);

  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  Address = r_warm_rst_assert[PortNum];

  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  FchProm21XhciWriteByte(XhciMmio, Address, (Value8 | BIT2));
}

VOID
FchProm21UsbCbsPei (
  IN       UINT32 XhciMmio
  )
{
  UINT8      i;
  UINT8      Data8;
  volatile   UINT32     *UsbSts = NULL ;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  // Check if Host Reset  = 0  before process PT vendor command.
  UsbSts = (UINT32 *)(UINTN)(XhciMmio + 0x20);
  DEBUG ((EFI_D_INFO, "-->  Usb Status = 0x%x\n", *UsbSts));
  while (*UsbSts & 0x00000002) {};

  if (PcdGet8 (PcdPT21XhciP0Gen) != 0xf) {
    if (PcdGet8 (PcdPT21XhciP0Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 0);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 0);
    }
  }

  if (PcdGet8 (PcdPT21XhciP1Gen) != 0xf) {
    if (PcdGet8 (PcdPT21XhciP1Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 1);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 1);
    }
  }

  if (PcdGet8 (PcdPT21XhciP2Gen) != 0xf) {
    if (PcdGet8 (PcdPT21XhciP2Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 2);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 2);
    }
  }

  if (PcdGet8 (PcdPT21XhciP3Gen) != 0xf) {
    if (PcdGet8 (PcdPT21XhciP3Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 3);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 3);
    }
  }

  if (PcdGet8 (PcdPT21XhciP4Gen) != 0xf) {
    if (PcdGet8 (PcdPT21XhciP4Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 4);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 4);
    }
  }

  if (PcdGet8 (PcdPT21XhciP5Gen) != 0xf) {
    if (PcdGet8 (PcdPT21XhciP5Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 5);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 5);
    }
  }

  if (PcdGet8 (PcdPT21HW_LPM) != 0xf) {
    if (PcdGet8 (PcdPT21HW_LPM) == 1) {
      for (i = 0; i < 3; i++) {
        Data8 = FchProm21XhciReadByte (XhciMmio, hw_lpm_en[i]);
        FchProm21XhciWriteByte (XhciMmio, hw_lpm_en[i], (Data8 | 0x1E));
      }
    } else {
      for (i = 0; i < 3; i++) {
        Data8 = FchProm21XhciReadByte (XhciMmio, hw_lpm_en[i]);
        FchProm21XhciWriteByte (XhciMmio, hw_lpm_en[i], (Data8 & (~0x1E)));
      }
    }
  }

  if (PcdGet8 (PcdPT21XHC_PME) != 0xf) {
    if (PcdGet8 (PcdPT21XHC_PME) == 1) {
      Data8 = FchProm21XhciReadByte (XhciMmio, XHCI_REG_CLR_PME_EN);
      FchProm21XhciWriteByte (XhciMmio, XHCI_REG_CLR_PME_EN, (Data8 & (~BIT7)));
    } else {
      Data8 = FchProm21XhciReadByte (XhciMmio, XHCI_REG_CLR_PME_EN);
      FchProm21XhciWriteByte (XhciMmio, XHCI_REG_CLR_PME_EN, (Data8 | BIT7));
    }
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}

VOID
FchSecProm21UsbCbsPei (
  IN       UINT32 XhciMmio
  )
{
  UINT8      i;
  UINT8      Data8;
  volatile   UINT32     *UsbSts = NULL ;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  // Check if Host Reset  = 0  before process PT vendor command.
  UsbSts = (UINT32 *)(UINTN)(XhciMmio + 0x20);
  DEBUG ((EFI_D_INFO, "-->  Usb Status = 0x%x\n", *UsbSts));
  while (*UsbSts & 0x00000002) {};

  if (PcdGet8 (PcdSecPT21XhciP0Gen) != 0xf) {
    if (PcdGet8 (PcdSecPT21XhciP0Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 0);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 0);
    }
  }

  if (PcdGet8 (PcdSecPT21XhciP1Gen) != 0xf) {
    if (PcdGet8 (PcdSecPT21XhciP1Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 1);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 1);
    }
  }

  if (PcdGet8 (PcdSecPT21XhciP2Gen) != 0xf) {
    if (PcdGet8 (PcdSecPT21XhciP2Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 2);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 2);
    }
  }

  if (PcdGet8 (PcdSecPT21XhciP3Gen) != 0xf) {
    if (PcdGet8 (PcdSecPT21XhciP3Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 3);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 3);
    }
  }

  if (PcdGet8 (PcdSecPT21XhciP4Gen) != 0xf) {
    if (PcdGet8 (PcdSecPT21XhciP4Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 4);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 4);
    }
  }

  if (PcdGet8 (PcdSecPT21XhciP5Gen) != 0xf) {
    if (PcdGet8 (PcdSecPT21XhciP5Gen) == 1) {
      FchProm21XhciGen(XhciMmio, 1, 5);
    } else {
      FchProm21XhciGen(XhciMmio, 0, 5);
    }
  }

  if (PcdGet8 (PcdSecPT21HW_LPM) != 0xf) {
    if (PcdGet8 (PcdSecPT21HW_LPM) == 1) {
      for (i = 0; i < 3; i++) {
        Data8 = FchProm21XhciReadByte (XhciMmio, hw_lpm_en[i]);
        FchProm21XhciWriteByte (XhciMmio, hw_lpm_en[i], (Data8 | 0x1E));
      }
    } else {
      for (i = 0; i < 3; i++) {
        Data8 = FchProm21XhciReadByte (XhciMmio, hw_lpm_en[i]);
        FchProm21XhciWriteByte (XhciMmio, hw_lpm_en[i], (Data8 & (~0x1E)));
      }
    }
  }

  if (PcdGet8 (PcdSecPT21XHC_PME) != 0xf) {
    if (PcdGet8 (PcdSecPT21XHC_PME) == 1) {
      Data8 = FchProm21XhciReadByte (XhciMmio, XHCI_REG_CLR_PME_EN);
      FchProm21XhciWriteByte (XhciMmio, XHCI_REG_CLR_PME_EN, (Data8 & (~BIT7)));
    } else {
      Data8 = FchProm21XhciReadByte (XhciMmio, XHCI_REG_CLR_PME_EN);
      FchProm21XhciWriteByte (XhciMmio, XHCI_REG_CLR_PME_EN, (Data8 | BIT7));
    }
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}
