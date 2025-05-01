/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>

#include <Library/IdsLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Ppi/NbioEarlyLinkPpi.h>
#include <AmdPcieComplex.h>
#include <Library/FchSpiAccessLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <AmdPspDirectory.h>
#include <Filecode.h>

#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include "FchProm21Pei.h"

#define FILECODE FCH_PROM21_PEI_FCHPROM21PEILOADFW_FILECODE

VOID
FwChecksum (
  IN      UINT8     *PtFwAddress,
  IN      UINT32    PtFwSize,
  IN OUT  UINT32    *Checksum
  )
{
  UINT32  Counter;

  for (Counter = 0; Counter < (PtFwSize - 0x4C); Counter++) {
    // PtFwSize = Length of PROM21 firmware binary file
    // FW Header Length = 12 = 0x0C
    // Digital Signature Length = 64 = 0x40
    // Length of PROM21 pure firmware = PtFwSize - 0x4C
    *Checksum += (UINT32) *(PtFwAddress + 0x0C + Counter);
  }
}

VOID FchProm21XhciCtrl (
IN       UINT32   XhciMmio,
IN       BOOLEAN  Enable,
IN       UINTN    PortNum
)
{
  UINT32  Address;
  UINT8 Value8;
  BOOLEAN EnableGen2;

  Value8 = 0;
  Address = 0;

  EnableGen2 = (PcdGet8 (PcdPT21Usb3P0) == 0x2) ? TRUE : FALSE;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- %d, PortNum = %d\n", __FUNCTION__, EnableGen2, PortNum));

  switch(PortNum)
  {
    case Usb3Port0:
    case Usb3Port1:
    case Usb3Port2:
    case Usb3Port3:
    case Usb3Port4:
    case Usb3Port5:
      Address = 0x1C51C;
      break;
    case Usb2Port0:
    case Usb2Port1:
    case Usb2Port2:
    case Usb2Port3:
    case Usb2Port4:
    case Usb2Port5:
    case Usb2Port6:
    case Usb2Port7:
      Address = 0x1C51D;
      break;
    case Usb2Port8:
    case Usb2Port9:
    case Usb2Port10:
    case Usb2Port11:
      Address = 0x1C51E;
      break;
    default:
      break;
  }

  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  DEBUG ((EFI_D_INFO, "Read Value8 = 0x%x, Address = 0x%x\n", Value8, Address));

  if (EnableGen2) {
    Value8 &= ~(UINT8)(BIT0); // Bit0 = 0 to set Gen2x2
  } else {
    Value8 |= (UINT8)(BIT0);
  }

  switch(PortNum)
  {
    case Usb3Port0:
      if(!Enable) {
        Value8 |= (UINT8)(BIT1);
      } else {
        Value8 &= ~(UINT8)(BIT1);
      }
      break;
    case Usb3Port1:
      if(!EnableGen2) { // This port is disable when set Gen2x2
        if(!Enable) {
          Value8 |= (UINT8)(BIT2);
        } else {
          Value8 &= ~(UINT8)(BIT2);
        }
      }
      break;
    case Usb3Port2:
      if(!Enable) {
        Value8 |= (UINT8)(BIT3);
      } else {
        Value8 &= ~(UINT8)(BIT3);
      }
      break;
    case Usb3Port3:
      if(!Enable) {
        Value8 |= (UINT8)(BIT4);
      } else {
        Value8 &= ~(UINT8)(BIT4);
      }
      break;
    case Usb3Port4:
      if(!Enable) {
        Value8 |= (UINT8)(BIT5);
      } else {
        Value8 &= ~(UINT8)(BIT5);
      }
      break;
    case Usb3Port5:
      if(!Enable) {
        Value8 |= (UINT8)(BIT6);
      } else {
        Value8 &= ~(UINT8)(BIT6);
      }
      break;
    case Usb2Port0:
      if(!Enable) {
        Value8 |= (UINT8)(BIT0);
      } else {
        Value8 &= ~(UINT8)(BIT0);
      }
      break;
    case Usb2Port1:
      if(!EnableGen2) { // This port is disable when set Gen2x2
        if(!Enable) {
          Value8 |= (UINT8)(BIT1);
        } else {
          Value8 &= ~(UINT8)(BIT1);
        }
      }
      break;
    case Usb2Port2:
      if(!Enable) {
        Value8 |= (UINT8)(BIT2);
      } else {
        Value8 &= ~(UINT8)(BIT2);
      }
      break;
    case Usb2Port3:
      if(!Enable) {
        Value8 |= (UINT8)(BIT3);
      } else {
        Value8 &= ~(UINT8)(BIT3);
      }
      break;
    case Usb2Port4:
      if(!Enable) {
        Value8 |= (UINT8)(BIT4);
      } else {
        Value8 &= ~(UINT8)(BIT4);
      }
      break;
    case Usb2Port5:
      if(!Enable) {
        Value8 |= (UINT8)(BIT5);
      } else {
        Value8 &= ~(UINT8)(BIT5);
      }
      break;
    case Usb2Port6:
      if(!Enable) {
        Value8 |= (UINT8)(BIT6);
      } else {
        Value8 &= ~(UINT8)(BIT6);
      }
      break;
    case Usb2Port7:
      if(!Enable) {
        Value8 |= (UINT8)(BIT7);
      } else {
        Value8 &= ~(UINT8)(BIT7);
      }
      break;
    case Usb2Port8:
      if(!Enable) {
        Value8 |= (UINT8)(BIT0);
      } else {
        Value8 &= ~(UINT8)(BIT0);
      }
      break;
    case Usb2Port9:
      if(!Enable) {
        Value8 |= (UINT8)(BIT1);
      } else {
        Value8 &= ~(UINT8)(BIT1);
      }
      break;
    case Usb2Port10:
      if(!Enable) {
        Value8 |= (UINT8)(BIT2);
      } else {
        Value8 &= ~(UINT8)(BIT2);
      }
      break;
    case Usb2Port11:
      if(!Enable) {
        Value8 |= (UINT8)(BIT3);
      } else {
        Value8 &= ~(UINT8)(BIT3);
      }
      break;
    default:
      break;
  }

  DEBUG ((EFI_D_INFO, "Write Value8 = 0x%x, Address = 0x%x\n", Value8, Address));

  // Set specific features
  FchProm21XhciWriteDWord (XhciMmio, 0x1C520, 0x12345678);
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);
}

// Enable/Disable USB 3.0/2.0 Port
VOID
AmdSecPtUsbPort (
  IN      UINT32    XhciMmio
  )
{
  UINT32  DidVid;

  DidVid = PcdGet32 (PcdSecPT21XhciID);

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "PT-PEI-%a-XHCI DidVid == 0x%04X == PT21_XHCI_ID\n", __FUNCTION__, DidVid));

  if ((DidVid == PT21_XHCI_ID_L1) || (DidVid == PT21_XHCI_ID_L2) || (DidVid == PT21_XHCI_ID_L3)){

    if (PcdGet8 (PcdSecPT21Usb3P0) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb3P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port0);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb3P1) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb3P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port1);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb3P2) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb3P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port2);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb3P3) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb3P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port3);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb3P4) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb3P4) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port4);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port4);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb3P5) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb3P5) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port5);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port5);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P0) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port0);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P1) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port1);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P2) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port2);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P3) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port3);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P4) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P4) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port4);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port4);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P5) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P5) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port5);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port5);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P6) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P6) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port6);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port6);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P7) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P7) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port7);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port7);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P8) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P8) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port8);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port8);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P9) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P9) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port9);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port9);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P10) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P10) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port10);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port10);
      }
    }

    if (PcdGet8 (PcdSecPT21Usb2P11) != 0xf) {
      if (PcdGet8 (PcdSecPT21Usb2P11) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port11);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port11);
      }
    }

  }else if(DidVid == PT21_XHCI_ID_L4){

    if (PcdGet8 (PcdSecPT21L4Usb3P0) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb3P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port0);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb3P1) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb3P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port1);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb3P2) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb3P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port2);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb3P3) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb3P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port3);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P0) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port0);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P1) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port1);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P2) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port2);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P3) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port3);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P4) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P4) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port6);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port6);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P5) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P5) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port7);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port7);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P6) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P6) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port8);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port8);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P7) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P7) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port9);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port9);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P8) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P8) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port10);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port10);
      }
    }

    if (PcdGet8 (PcdSecPT21L4Usb2P9) != 0xf) {
      if (PcdGet8 (PcdSecPT21L4Usb2P9) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port11);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port11);
      }
    }
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}


// Enable/Disable USB 3.0/2.0 Port
VOID
AmdPtUsbPort (
  IN      UINT32    XhciMmio
  )
{
  UINT32  DidVid;

  DidVid = PcdGet32 (PcdPT21XhciID);

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "PT-PEI-%a-XHCI DidVid == 0x%04X == PT21_XHCI_ID\n", __FUNCTION__, DidVid));

  if ((DidVid == PT21_XHCI_ID_L1) || (DidVid == PT21_XHCI_ID_L2) || (DidVid == PT21_XHCI_ID_L3)){

    if (PcdGet8 (PcdPT21Usb3P0) != 0xf) {
      if (PcdGet8 (PcdPT21Usb3P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port0);
      }
    }

    if (PcdGet8 (PcdPT21Usb3P1) != 0xf) {
      if (PcdGet8 (PcdPT21Usb3P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port1);
      }
    }

    if (PcdGet8 (PcdPT21Usb3P2) != 0xf) {
      if (PcdGet8 (PcdPT21Usb3P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port2);
      }
    }

    if (PcdGet8 (PcdPT21Usb3P3) != 0xf) {
      if (PcdGet8 (PcdPT21Usb3P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port3);
      }
    }

    if (PcdGet8 (PcdPT21Usb3P4) != 0xf) {
      if (PcdGet8 (PcdPT21Usb3P4) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port4);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port4);
      }
    }

    if (PcdGet8 (PcdPT21Usb3P5) != 0xf) {
      if (PcdGet8 (PcdPT21Usb3P5) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port5);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port5);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P0) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port0);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P1) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port1);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P2) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port2);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P3) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port3);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P4) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P4) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port4);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port4);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P5) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P5) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port5);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port5);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P6) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P6) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port6);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port6);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P7) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P7) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port7);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port7);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P8) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P8) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port8);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port8);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P9) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P9) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port9);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port9);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P10) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P10) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port10);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port10);
      }
    }

    if (PcdGet8 (PcdPT21Usb2P11) != 0xf) {
      if (PcdGet8 (PcdPT21Usb2P11) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port11);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port11);
      }
    }

  }else if(DidVid == PT21_XHCI_ID_L4){

    if (PcdGet8 (PcdPT21L4Usb3P0) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb3P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port0);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb3P1) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb3P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port1);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb3P2) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb3P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port2);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb3P3) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb3P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb3Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb3Port3);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P0) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P0) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port0);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port0);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P1) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P1) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port1);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port1);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P2) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P2) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port2);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port2);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P3) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P3) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port3);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port3);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P4) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P4) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port6);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port6);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P5) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P5) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port7);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port7);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P6) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P6) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port8);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port8);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P7) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P7) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port9);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port9);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P8) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P8) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port10);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port10);
      }
    }

    if (PcdGet8 (PcdPT21L4Usb2P9) != 0xf) {
      if (PcdGet8 (PcdPT21L4Usb2P9) != 0) {
        FchProm21XhciCtrl(XhciMmio, 1, Usb2Port11);
      }
      else {
        FchProm21XhciCtrl(XhciMmio, 0, Usb2Port11);
      }
    }
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}

// Enable/Disable Specific Features
VOID
AmdSecPtSpecificFeatrue (
  IN      UINT32    XhciMmio
  )
{
  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  if(PcdGet8(PcdSecPT21Msi) != 0xf) {
    if(PcdGet8(PcdSecPT21Msi) != 0) {
      FchProm21XhciWriteByte (XhciMmio, 0x23834, 0x50);
    }
    else {
      FchProm21XhciWriteByte (XhciMmio, 0x23834, 0x68);
    }
  }

  if(PcdGet8(PcdSecPT21Msix) != 0xf) {
    if(PcdGet8(PcdSecPT21Msix) != 0) {
      FchProm21XhciWriteByte (XhciMmio, 0x23851, 0x68);
    }
    else {
      FchProm21XhciWriteByte (XhciMmio, 0x23851, 0x78);
    }
  }

  if(PcdGet8(PcdSecPT21DbC) != 0xf) {
    if(PcdGet8(PcdSecPT21DbC) != 0) {
      FchProm21XhciWriteByte (XhciMmio, 0x18A61, 8);
    }
    else {
      FchProm21XhciWriteByte (XhciMmio, 0x18A61, 16);
    }
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}


VOID
AmdPtSpecificFeatrue (
  IN      UINT32    XhciMmio
  )
{
  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  if(PcdGet8(PcdPT21Msi) != 0xf) {
    if(PcdGet8(PcdPT21Msi) != 0) {
      FchProm21XhciWriteByte (XhciMmio, 0x23834, 0x50);
    }
    else {
      FchProm21XhciWriteByte (XhciMmio, 0x23834, 0x68);
    }
  }

  if(PcdGet8(PcdPT21Msix) != 0xf) {
    if(PcdGet8(PcdPT21Msix) != 0) {
      FchProm21XhciWriteByte (XhciMmio, 0x23851, 0x68);
    }
    else {
      FchProm21XhciWriteByte (XhciMmio, 0x23851, 0x78);
    }
  }

  if(PcdGet8(PcdPT21DbC) != 0xf) {
    if(PcdGet8(PcdPT21DbC) != 0) {
      FchProm21XhciWriteByte (XhciMmio, 0x18A61, 8);
    }
    else {
      FchProm21XhciWriteByte (XhciMmio, 0x18A61, 16);
    }
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}

EFI_STATUS
LibFchLpPtXhciProceedFwLoad (
  IN      EFI_PEI_SERVICES    **PeiServices,
  IN      UINT8     *PtFwInRamPtr,
  IN      UINT8     XhciBus,
  IN      UINT8     XhciDev,
  IN      UINT8     XhciFun,
  IN      UINT32    XhciMmio
  )
{
  UINT8             DSResult;
  UINT8             Parameter;
  UINT16            wPrg_Addr;
  UINT32            Data32;
  UINT32            UsbSts;
  UINT32            Address;
  UINT32            Counter;
  UINT32            FirstCounter;
  UINT32            LoadCounter;
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start-0x%08X-[%02X:%02X:%02X]-0x%08X\n", __FUNCTION__, PtFwInRamPtr, XhciBus, XhciDev, XhciFun, XhciMmio));

  // Check if CPU is ROM code or RAM code
  Parameter = FchProm21XhciReadByteV2 (XhciMmio, 0x15041);
  DEBUG ((DEBUG_INFO, "    PT CPU runs in %a code\n", (Parameter & BIT0) ? "RAM" : "ROM"));

  if ((Parameter & BIT0) || (PcdGet8 (PcdPT21DbgLoadFw) == 1)) {
    DEBUG ((DEBUG_INFO, "  It's RAM code already, do NOT need to load PT FW by BIOS. Return Success !!!\n"));
    DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  AGESA_TESTPOINT (TpFchPTLoadFWDelay1Sec, NULL);

  // 3. BIOS forces PCIe Gen 1 link with PROM21 and reset PROM21 to ROM code mode
  // Program Digital Signature Parameter1
  DEBUG ((DEBUG_INFO, "  Program Digital Signature Parameter 1 Offset 0x%08X = 0x%X\n", PtFwInRamPtr + 0x20002, *(PtFwInRamPtr + 0x20002)));
  Parameter = FchProm21XhciReadByteV2 (XhciMmio, 0x15180);
  Parameter &= 0x0F;
  Parameter |= ((*(PtFwInRamPtr + 0x20002) & 0x3C) << 2);
  FchProm21XhciWriteByte (XhciMmio, 0x15180, Parameter);

  Address = (UINT32) (PcdGet64 (PcdPciExpressBaseAddress) + (XhciBus << 20) + (XhciDev << 15) + (XhciFun << 12) + PCIE_CFG_RAM_ADDR0_REG);

  AGESA_TESTPOINT (TpFchPTLoadFWDelay2Sec, NULL);

  LoadCounter = 0;
  do {
    DEBUG ((DEBUG_INFO, "    Loading PT FW Starts LoadCounter = %d !!!\n", LoadCounter));

    FirstCounter = 0;
    do {
      // Set CPU to ROM code and runs at 62.5MHz
      Parameter = FchProm21XhciReadByteV2 (XhciMmio, 0x15040);
      Parameter &= 0xFC;
      Parameter |= 0x02;
      FchProm21XhciWriteByte (XhciMmio, 0x15040, Parameter);

      // Reset CPU
      Parameter = FchProm21XhciReadByteV2 (XhciMmio, 0x15042);
      Parameter &= 0xFE;
      Parameter |= 0x01;
      FchProm21XhciWriteByte (XhciMmio, 0x15042, Parameter);

      PtDelayUs (85 * 1000);                                                      // Delay 85 ms

      Counter = 0;
      do {
        // Check if Host Reset = 0
        UsbSts = PtMmioRead32 (XhciMmio + 0x20);
        DEBUG ((DEBUG_INFO, "    Usb Status (Before) = 0x%X Counter = %d\n", UsbSts, Counter));
        if (UsbSts & BIT1) {
          PtDelayUs (20 * 1000);                                                  // Delay 20 ms
        }
        Counter++;
      } while ((UsbSts & BIT1) && (Counter < 10));

      // Check if CPU is ROM code or RAM code
      Parameter = FchProm21XhciReadByteV2 (XhciMmio, 0x15041);
      DEBUG ((DEBUG_INFO, "    PT CPU runs in %a code\n", (Parameter & BIT0) ? "RAM" : "ROM"));

      FirstCounter++;
    } while (((UsbSts & BIT1) || (Parameter & BIT0)) && (FirstCounter < 10));

    AGESA_TESTPOINT (TpFchPTLoadFWDelay3Sec, NULL);

    if (FirstCounter == 10) {
      Counter = 1;
      LoadCounter = 10;
    } else {
      // Set Digital Signature Parameter2
      Data32 = 0x2001F;
      Data32 += (UINT32) ((*(PtFwInRamPtr + 0x20001) & 0x7E) >> 1);
      DEBUG ((DEBUG_INFO, "    Set Digital Signature Parameter 2 Offset 0x%08X = 0x%X -> Address = 0x%08X\n", PtFwInRamPtr + 0x20001, *(PtFwInRamPtr + 0x20001), Data32));
      for (Counter = 0; Counter < 32; Counter++) {
        FchProm21XhciWriteByte (XhciMmio, 0x15140 + Counter, *(PtFwInRamPtr + Data32 - Counter));
      }

      AGESA_TESTPOINT (TpFchPTLoadFWDelay4Sec, NULL);

      DEBUG ((DEBUG_INFO, "    Write the first 64K FW data to RAM\n"));
      // Init Program Address for the first 64K FW
      PtPciWrite16 (XhciBus, XhciDev, XhciFun, PCIE_CFG_RAM_ADDR0_REG, 0);          // PCI Reg 0xE2
      while (*(volatile UINT16*) Address != 0) {}                                   // PCI Reg 0xE2

      // Write the first 64K FW data to RAM
      for (Counter = 0; Counter <= 0x3FFF ; Counter++) {
        // Counter = 0x0000 ~ 0x3FFF , wPrg_Addr = 0x0000 ~ 0x7FFE
        wPrg_Addr = (UINT16) (Counter << 1);

        Data32  = (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0);
        Data32 |= (UINT32) *(PtFwInRamPtr + wPrg_Addr + 1) << 8;
        Data32 |= (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0x8000) << 16;
        Data32 |= (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0x8001) << 24;
        *(volatile UINT32 *) (XhciMmio + PCIE_MEM_RAM_WDATA_OFFSET) = Data32;       // xData Reg 0x3010
        while (*(volatile UINT16*) Address == wPrg_Addr) {}                         // PCI Reg 0xE2
      } // End of for (Counter = 0; Counter <= 0x3FFF ; Counter++)

      AGESA_TESTPOINT (TpFchPTLoadFWDelay5Sec, NULL);

      DEBUG ((DEBUG_INFO, "    Write the second 64K FW data to RAM\n"));
      // Init Program Address for the second 64K FW
      PtPciWrite16 (XhciBus, XhciDev, XhciFun, PCIE_CFG_RAM_ADDR0_REG, 0x8000);     // PCI Reg 0xE2
      while (*(volatile UINT16*) Address != 0x8000) {}                              // PCI Reg 0xE2

      // Write the second 64K FW data to RAM
      for (Counter = 0x4000; Counter <= 0x7FFF; Counter++) {
        // Counter = 0x4000 ~ 0x7FFF , wPrg_Addr = 0x8000 ~ 0xFFFE
        wPrg_Addr = (UINT16) (Counter << 1);

        Data32  = (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0x08000);
        Data32 |= (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0x08001) << 8;
        Data32 |= (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0x10000) << 16;
        Data32 |= (UINT32) *(PtFwInRamPtr + wPrg_Addr + 0x10001) << 24;
        *(volatile UINT32 *) (XhciMmio + PCIE_MEM_RAM_WDATA_OFFSET) = Data32;       // xData Reg 0x3010
        while (*(volatile UINT16*) Address == wPrg_Addr) {}                         // PCI Reg 0xE2
      } // End of for (Counter = 0x4000; Counter <= 0x7FFF; Counter++)

      AGESA_TESTPOINT (TpFchPTLoadFWDelay6Sec, NULL);

      PtDelayUs (144 * 1000);                                                       // Delay 144 ms
      Counter = 0;
      do {
        // Check Digital Signature Verification Result
        DSResult = FchProm21XhciReadByteV2 (XhciMmio, 0x1A51C);
        DEBUG ((DEBUG_INFO, "    Digital Signature Verification Result = %a\n", (DSResult & BIT5) ? "Success" : "Fail"));

        // Check if Host Reset = 0
        UsbSts = PtMmioRead32 (XhciMmio + 0x20);
        DEBUG ((DEBUG_INFO, "    Usb Status (After) = 0x%X Counter = %d\n", UsbSts, Counter));
        if (UsbSts & BIT1) {
          PtDelayUs (20 * 1000);                                                    // Delay 20 ms
        }

        // Check if CPU is ROM code or RAM code
        Parameter = FchProm21XhciReadByteV2 (XhciMmio, 0x15041);
        DEBUG ((DEBUG_INFO, "    CPU runs in %a code\n", (Parameter & BIT0) ? "RAM" : "ROM"));

        Counter++;
      } while (((UsbSts & BIT1) || ((Parameter & BIT0) == 0) || ((DSResult & BIT5) == 0)) && (Counter < 10));
      LoadCounter++;
    }
  } while ((Counter == 10) && (LoadCounter < 10));

  AGESA_TESTPOINT (TpFchPTLoadFWDelay7Sec, NULL);

  if ((Counter == 10) && (LoadCounter == 10)) {
    DEBUG ((DEBUG_INFO, "  Loading PT FW Fail !!!\n"));
    Status = EFI_UNSUPPORTED;
  } else {
    DEBUG ((DEBUG_INFO, "  Loading PT FW Success !!!\n"));
    Status = EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
  return Status;
}

EFI_STATUS
AmdPTPlusFwLoadPei (
  IN      EFI_PEI_SERVICES    **PeiServices
  )
{
  UINT8             GppBus;
  UINT8             GppDev;
  UINT8             GppFun;
  UINT8             SecGppBus;
  UINT8             SecGppDev;
  UINT8             SecGppFun;
  UINT8             UspBus;
  UINT8             DspBus;
  UINT8             CurrentSpeed;
  UINT8             SecCurrentSpeed;
  UINT8             DspDevNum;
  UINT8             XhciLock;
  UINT8             FindPspDir = 0;
  UINT8             RomSigMainLoop;
  UINT8             RomSigSubLoop;
  UINT8             PTTargetSpeed;
  UINT8             SecTargetSpeed;
  UINT8             DspTargetSpeed;
  UINT8             SecPortNum;
  UINT8             *PtFwInRamPtr;
  UINT32            *PtFwInRomPtr;
  UINT32            PTFwSize;
  UINT32            PtFwChecksum;
  UINT32            Checksum;
  UINT32            XhciMmio;
  UINT32            SecXhciMmio;
  UINT32            GppPcieAddress;
  UINT32            SecGppPcieAddress;
  UINT32            Counter;
  UINT32            LcResetLinkAddress;
  UINT32            UsbSts;
  UINT64            F17MmioBase;
  UINT64            F17MmioSize;
  UINT64            FwVersion;
  UINT32            Index;
  EFI_STATUS        Status;
  EFI_BOOT_MODE     BootMode;
  LOAD_FW_CONFIG_DATA             FwConfigBuf;
  PT_PCI_SAVE_RESTORE_TABLE       PtPciTable;
  FABRIC_TARGET                   MmioTarget;
  FABRIC_MMIO_ATTRIBUTE           MmioAttr;
  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *PcieServicesPpi;
  FIRMWARE_ENTRY_TABLEV2          FirmwareTableBase;
  CONST                           UINT32 RomSigOffsetTable[] =
  {
    0xFA0000, //  --> 512KB base
    0xF20000, //  --> 1MB base
    0xE20000, //  --> 2MB base
    0xC20000, //  --> 4MB base
    0x820000, //  --> 8MB base
    0x020000  //  --> 16MB base
  };

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  Checksum = 0;
  PtFwInRomPtr = NULL;
  PtFwInRamPtr = (UINT8 *) PcdGet32 (PcdPT21FwInRamAddress);

  AGESA_TESTPOINT (TpFchPTLoadFWEntry, NULL);

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "  GetBootMode Error !!!  Set BootMode to BOOT_WITH_FULL_CONFIGURATION\n"));
    BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }

  if (BootMode != BOOT_ON_S3_RESUME) {
    DEBUG ((DEBUG_INFO, "  BootMode != BOOT_ON_S3_RESUME, copy PT FW from Rom to Ram !!!\n"));

    // Reserve PT FW 160KB For S3
    BuildMemoryAllocationHob (
                    PcdGet32 (PcdPT21FwInRamAddress),
                    (128 + 4) * 1024,
                    EfiReservedMemoryType
                    );

    if (PcdGet8 (PcdPT21FWLoading) == 1) {
      // Get PT FW from PSP Directory
      // OperationCode;              ///< Operation Code 0x01: Decrypt and copy PROM21 FW without PSP header to available memory
      // AvailableMemoryAddress;     ///< Available memory address
      // AvailableMemorySize;        ///< Available memory size
      FwConfigBuf.OperationCode = 0x01;
      FwConfigBuf.AvailableMemoryAddress = PcdGet32 (PcdPT21FwInRamAddress);
      FwConfigBuf.AvailableMemorySize = (128 + 4) * 1024;
      Status = PspMboxBiosCmdLoadFwConfig (&FwConfigBuf);

      if (Status == EFI_NOT_FOUND) {
        DEBUG ((DEBUG_INFO, "PT-PEI-%a(%d)-End-ERROR: Get PT FW from PSP Directory FAIL !!!\n", __FUNCTION__, __LINE__));
        AGESA_TESTPOINT (TpFchPTLoadFWExit, NULL);
        ASSERT (FALSE);
        return Status;
      }

    } else {
      // Search beyond ROM2 to find ROMSIG
      for (RomSigMainLoop = 0; RomSigMainLoop < 4; RomSigMainLoop++) {
        for (RomSigSubLoop = 0; RomSigSubLoop < sizeof (RomSigOffsetTable) / sizeof (UINT32); RomSigSubLoop++) {
          DEBUG ((DEBUG_INFO, "  PSP Dir Rom address = 0x%08X\n", RomSigOffsetTable[RomSigSubLoop] + 0x1000000 * RomSigMainLoop));
          // Get the ROM SIG from SPI ROM
          if (FchSpiRomRead ((RomSigOffsetTable[RomSigSubLoop] + 0x1000000 * RomSigMainLoop), (UINT8 *)&FirmwareTableBase, sizeof (FIRMWARE_ENTRY_TABLEV2)) != TRUE) {
            DEBUG ((DEBUG_INFO, "PT-PEI-%a(%d)-End-ERROR: FchSpiRomRead FAIL !!!\n", __FUNCTION__, __LINE__));
            AGESA_TESTPOINT (TpFchPTLoadFWExit, NULL);
            ASSERT (FALSE);
            return EFI_NOT_FOUND;
          }

          if (FirmwareTableBase.Signature == FIRMWARE_TABLE_SIGNATURE) {
            // PT_RomOffset + 0x38
            FindPspDir = 1;
            PtFwInRomPtr = (UINT32 *)FirmwareTableBase.PT19FW;
            DEBUG ((DEBUG_INFO, "  PT FW in ROM = 0x%08X; FirmwareTableBase.PT19FW = 0x%08X\n", PtFwInRomPtr, FirmwareTableBase.PT19FW));
            break;
          }
        }
        if (FindPspDir == 1) {
          break;
        }
      }

      if ((RomSigMainLoop == 4) || (PtFwInRomPtr == (UINT32 *) 0x00000000) || (PtFwInRomPtr == (UINT32 *) 0xFFFFFFFF)) {
        DEBUG ((DEBUG_INFO, "PT-PEI-%a(%d)-End-ERROR: PT FW NOT FOUND in ROM SIG !!!\n", __FUNCTION__, __LINE__));
        AGESA_TESTPOINT (TpFchPTLoadFWExit, NULL);
        ASSERT (FALSE);
        return EFI_NOT_FOUND;
      }

      DEBUG ((DEBUG_INFO, "  FirmwareTableBase.PT19FW = 0x%08X\n", FirmwareTableBase.PT19FW));
      FchSpiRomRead(FirmwareTableBase.PT19FW + 0x04, (UINT8 *)&PTFwSize, sizeof(PTFwSize));
      DEBUG ((DEBUG_INFO, "  PT FW Rom Address    = 0x%08X, PTFWSize = 0x%x\n", PtFwInRomPtr, PTFwSize));
      for (Index = 0; Index < PTFwSize; Index++, PtFwInRamPtr++)
      {
        FchSpiRomRead( FirmwareTableBase.PT19FW + Index, PtFwInRamPtr, sizeof(UINT8));
      }
      DEBUG ((DEBUG_INFO, "PtFwInRamPtr = 0x%08x \n", PtFwInRamPtr));
    }
  } // End of if (BootMode != BOOT_ON_S3_RESUME)

  PtFwInRamPtr = (UINT8 *) PcdGet32 (PcdPT21FwInRamAddress);
  // Get PT FW Size from RAM
  PTFwSize  = ((UINT32) *(PtFwInRamPtr + 0x07)) << 24;
  PTFwSize |= ((UINT32) *(PtFwInRamPtr + 0x06)) << 16;
  PTFwSize |= ((UINT32) *(PtFwInRamPtr + 0x05)) << 8;
  PTFwSize |= ((UINT32) *(PtFwInRamPtr + 0x04)) << 0;

  // Get PT FW Checksum from RAM
  PtFwChecksum  = ((UINT32) *(PtFwInRamPtr + 0x0B)) << 24;
  PtFwChecksum |= ((UINT32) *(PtFwInRamPtr + 0x0A)) << 16;
  PtFwChecksum |= ((UINT32) *(PtFwInRamPtr + 0x09)) << 8;
  PtFwChecksum |= ((UINT32) *(PtFwInRamPtr + 0x08)) << 0;

  // PROM21 Load FW Procedure
  // 1. Read PROM21 pure firmware from BIOS SPI ROM and calculate checksum
  FwChecksum (PtFwInRamPtr, PTFwSize, &Checksum);

  DEBUG ((DEBUG_INFO, "  PT FW Ram Address    = 0x%08X\n", PtFwInRamPtr));
  DEBUG ((DEBUG_INFO, "  PT FW Size           = 0x%08X\n", PTFwSize));
  DEBUG ((DEBUG_INFO, "  PT FW Checksum       = 0x%08X\n", PtFwChecksum));
  DEBUG ((DEBUG_INFO, "  Checksum Calculation = 0x%08X\n", Checksum));

  // 2. Check if this checksum is the same as checksum in header
  if (PtFwChecksum != Checksum) {
    DEBUG ((DEBUG_INFO, "PT-PEI-%a(%d)-End-ERROR: PT FW Checksum ERROR !!!\n", __FUNCTION__, __LINE__));
    AGESA_TESTPOINT (TpFchPTLoadFWExit, NULL);
    return EFI_UNSUPPORTED;
  }

  // Save FW version
  FwVersion  = ((UINT64) *(PtFwInRamPtr + 0x8C)) << 40;
  FwVersion |= ((UINT64) *(PtFwInRamPtr + 0x8D)) << 32;
  FwVersion |= ((UINT64) *(PtFwInRamPtr + 0x8E)) << 24;
  FwVersion |= ((UINT64) *(PtFwInRamPtr + 0x8F)) << 16;
  FwVersion |= ((UINT64) *(PtFwInRamPtr + 0x90)) << 8;
  FwVersion |= ((UINT64) *(PtFwInRamPtr + 0x91)) << 0;
  PcdSet64S (PcdPT21FwVersion, FwVersion);
  DEBUG ((DEBUG_INFO, "  Promontory FwVersion = 0x%016LX\n", FwVersion));

  GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);

  SecGppPcieAddress = PcdGet32 (PcdSecPT21GppPcieAddress);
  SecGppBus = (UINT8) ((SecGppPcieAddress >> 20) & 0xFF);
  SecGppDev = (UINT8) ((SecGppPcieAddress >> 15) & 0x1F);
  SecGppFun = (UINT8) ((SecGppPcieAddress >> 12) & 0x07);

  // Allocate MMIO resources from Fabric
  MmioTarget.TgtType = TARGET_RB;
  MmioTarget.SocketNum = 0;
  MmioTarget.RbNum = PcdGet8 (PcdPT21RootBridgeNum);
  F17MmioSize = SIZE_2MB;
  MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
  FabricAllocateMmio (&F17MmioBase, &F17MmioSize, ALIGN_1M, MmioTarget, &MmioAttr);
  XhciMmio = (UINT32) F17MmioBase;
  SecXhciMmio = (UINT32) (F17MmioBase + SIZE_1MB);
  PcdSet32S (PcdPT21XhciMmio, XhciMmio);
  PcdSet32S (PcdPT21SecXhciMmio, SecXhciMmio);
  DEBUG ((DEBUG_INFO, "  Allocate MMIO resources from Fabric 0x%08X Size 0x%08X\n", XhciMmio, F17MmioSize));

  // LocatePpi gAmdNbioPcieServicesPpiGuid to get PcieServicesPpi
  Status = (*PeiServices)->LocatePpi (PeiServices,
                                      &gAmdNbioPcieServicesPpiGuid,
                                      0,
                                      NULL,
                                      (VOID **)&PcieServicesPpi);
  ASSERT_EFI_ERROR (Status);

  if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
    // Save/Init UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    // Save/Init SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd, SecXhciCmd and SecASPM
    SaveInitPromBus (&PtPciTable, 0xBBBB);
  } else {
    // Save/Init UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    SaveInitPromBus (&PtPciTable, 0xBB);
  }

  // Check if XHCI is locked or not
  XhciLock = FchProm21XhciReadByteV2 (XhciMmio, XHCI_REG_PCIE_HW_RW);
  if ((XhciLock & BIT4) == 0) {
    PcdSet8S (PcdPT21XhciLock, 1);
    DEBUG ((DEBUG_INFO, "    Set PcdPT21XhciLock = %d\n", PcdGet8 (PcdPT21XhciLock)));
  }

  if (XhciLock & BIT4) {
    DEBUG ((DEBUG_INFO, "  XHCI is unlock\n"));

    // Force Gen1 before load FW
    CurrentSpeed = PtPciRead8 (GppBus, GppDev, GppFun, 0x6A);                   // GPP LINK_STATUS
    if ((CurrentSpeed & 0x0F) != DxioGen1) {
      if (Status == EFI_SUCCESS) {
        PcieServicesPpi->PcieSetSpeed (PcieServicesPpi, PeiServices, GppDev, GppFun, DxioGen1);
      }
      DEBUG ((DEBUG_INFO, "  Set PT speed to Gen1\n"));
    } else {
      DEBUG ((DEBUG_INFO, "  PT speed is Gen1 already\n"));
    }

    // Enable/Disable USB 3.0/2.0 Port
    DEBUG ((DEBUG_INFO, "  Enable/Disable USB 3.0/2.0 Port\n"));
    AmdPtUsbPort (XhciMmio);

    // Enable/Disable Specific Features
    DEBUG ((DEBUG_INFO, "  Enable/disble Specific Features\n"));
    AmdPtSpecificFeatrue (XhciMmio);

    AGESA_TESTPOINT (TpFchPTStartToLoadFW, NULL);

    // Load PT-Plus FW
    Status = LibFchLpPtXhciProceedFwLoad (PeiServices,
                                          PtFwInRamPtr + 0x0C,
                                          PtPciTable.PromBus + 2, 0, 0,
                                          XhciMmio);

    AGESA_TESTPOINT (TpFchPTLoadFWDelay8Sec, NULL);

    if (Status == EFI_SUCCESS) {
      if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
        // Restore SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd, SecXhciCmd and SecASPM
        RestorePromBus (&PtPciTable, 0xBB00);
        // Save/Init SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd, SecXhciCmd and SecASPM
        SaveInitPromBus (&PtPciTable, 0xBB00);

        // Check if Secondary PT XHCI is locked or not
        XhciLock = FchProm21XhciReadByteV2 (SecXhciMmio, XHCI_REG_PCIE_HW_RW);
        if ((XhciLock & BIT4) == 0) {
          PcdSet8S (PcdSecPT21XhciLock, 1);
          DEBUG ((DEBUG_INFO, "    Set PcdSecPT21XhciLock = %d\n", PcdGet8 (PcdSecPT21XhciLock)));
        }

        if (XhciLock & BIT4) {
          DEBUG ((DEBUG_INFO, "  Secondary PT XHCI is unlock\n"));

          // Force PT DSP Gen1 before load Secondary FW
          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);   // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF));

          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0xB0);    // PT DSP LINK_CTRL_2
          SecTargetSpeed &= 0xF0;
          SecTargetSpeed |= DxioGen1;
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0xB0, SecTargetSpeed);

          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x90);    // PT DSP LINK_CTRL
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecTargetSpeed | BIT5);

          for (Counter = 0; Counter < 50; Counter++) {
          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);    // PT DSP LINK_STATUS
          if ((SecTargetSpeed & 0x0F) == DxioGen1) {
              break;
            }
            PtDelayUs (20 * 1000);                                                // Delay 20 ms
          }

          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);    // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d Counter = %d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF, Counter));
          if ((SecCurrentSpeed & 0x0F) == DxioGen1) {
            DEBUG ((DEBUG_INFO, "  Secondary PT DSP set to Gen1 Success !!!\n"));
          } else {
            DEBUG ((DEBUG_INFO, "  Secondary PT DSP set to Gen1 Fail !!!\n"));
          }

          // Enable/Disable Secondary PROM21 USB 3.0/2.0 Port
          DEBUG ((DEBUG_INFO, "  Enable/Disable Secondary PROM21 USB 3.0/2.0 Port\n"));
          AmdSecPtUsbPort (SecXhciMmio);

          // Enable/Disable Secondary PROM21 Specific Features
          DEBUG ((DEBUG_INFO, "  Enable/disble Secondary PROM21 Specific Features\n"));
          AmdSecPtSpecificFeatrue (SecXhciMmio);

          AGESA_TESTPOINT (TpFchSecPTStartToLoadFW, NULL);

          // Load Secondary PT-Plus FW
          Status = LibFchLpPtXhciProceedFwLoad (PeiServices,
                                                PtFwInRamPtr + 0x0C,
                                                PtPciTable.PromBus + 6, 0, 0,
                                                SecXhciMmio);

          AGESA_TESTPOINT (TpFchPTLoadFWDelay9Sec, NULL);

          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);   // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF));

            SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x90);   // PT DSP LINK_CTRL
            PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecCurrentSpeed | BIT4);
            PtDelayUs (10 * 1000);                                                  // Delay 10 ms
            SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x3E);    // PT DSP BRIDGE_CNTL
            PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x3E, SecTargetSpeed | BIT6);
            PtDelayUs (10 * 1000);                                                  // Delay 10 ms
            PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x3E, SecTargetSpeed);    // PT DSP BRIDGE_CNTL
          PtDelayUs (10 * 1000);                                                  // Delay 10 ms
            PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecCurrentSpeed);

            do {
              // Check DL_ACTIVE = 1 and LINK_TRAINING = 0
              SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x93);  // PT DSP LINK_STATUS
            } while ((SecTargetSpeed & (BIT5 + BIT3)) != BIT5);

          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);    // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF));

          if ((SecCurrentSpeed & 0x0F) != (PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F)) {
          // Restore Original PT DSP Gen speed after loading Secondary FW
          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0xB0);    // PT DSP LINK_CTRL_2
          SecTargetSpeed &= 0xF0;
          SecTargetSpeed |= PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F;
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0xB0, SecTargetSpeed);

          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x90);    // PT DSP LINK_CTRL
            PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecTargetSpeed | BIT5);

          for (Counter = 0; Counter < 50; Counter++) {
            SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);  // PT DSP LINK_STATUS
            if ((SecTargetSpeed & 0x0F) == (PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F)) {
              break;
            }
            PtDelayUs (20 * 1000);                                                // Delay 20 ms
          }

            SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);   // PT DSP LINK_STATUS
            DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d Counter = %d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF, Counter));
            if ((SecCurrentSpeed & 0x0F) == (PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F)) {
              DEBUG ((DEBUG_INFO, "  Restore Secondary PT DSP speed to Gen%d Success !!!\n", SecCurrentSpeed & 0x0F));
          } else {
              DEBUG ((DEBUG_INFO, "  Restore Secondary PT DSP speed to Gen%d Fail !!! It's Gen%d now\n", PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F, SecCurrentSpeed & 0x0F));
            }
          }

        } else {
          DEBUG ((DEBUG_INFO, "  Secondary PT XHCI is lock: bypass PT code !!!\n"));
        } // End of if (XhciLock & BIT4)
      } // End of if (PcdGet32 (PcdPT21SecGppPcieAddress))
    } // End of if (Status == EFI_SUCCESS)

  } else {
    DEBUG ((DEBUG_INFO, "  XHCI is lock: bypass PT code !!!\n"));
  }

  AGESA_TESTPOINT (TpFchPTLoadFWDelay10Sec, NULL);

  if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
    // Restore UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    // Restore SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd, SecXhciCmd and SecASPM
    RestorePromBus (&PtPciTable, 0xBBBB);
  } else {
    // Restore UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    RestorePromBus (&PtPciTable, 0xBB);
  }

  CurrentSpeed = PtPciRead8 (GppBus, GppDev, GppFun, 0x6A);                     // GPP LINK_STATUS
  DEBUG ((DEBUG_INFO, "  PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", CurrentSpeed >> 4, CurrentSpeed & 0xF));

  // force DSP port 0 and port 8 to GEN1
  if (PcdGet32 (PcdSecPT21GppPcieAddress) == 0) {
    // Save/Init UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    SaveInitPromBus (&PtPciTable, 0xBBBB);

    // Force DSP 8 to GEN1
    UspBus = PtPciRead8 (GppBus, GppDev, GppFun, 0x19);
    DspBus = PtPciRead8 (UspBus, 0, 0, 0x19);

    if(PcdGet8(PcdPT21SecondPortNumber) == 0xf) {
      SecPortNum = 0x8;
    } else {
      SecPortNum = PcdGet8(PcdPT21SecondPortNumber);
    }
    DspTargetSpeed = PtPciRead8 (DspBus, 0, SecPortNum, 0xB0);    // PT DSP LINK_CTRL_2
    DspTargetSpeed &= 0xF0;
    DspTargetSpeed |= 0x1;
    PtPciWrite8 (DspBus, 0, SecPortNum, 0xB0, DspTargetSpeed);
    DEBUG ((DEBUG_INFO, "  PT UspBus = 0x%x DspBus = 0x%x, Port %x DspTargetSpeed = 0x%x\n", UspBus, DspBus, SecPortNum, DspTargetSpeed));
  }

  // Link down WA
  LcResetLinkAddress = FindPcieRegisterAddress (GppBus, GppDev, GppFun, PCIEPORT) + PCIE_LC_CNTL;
  DEBUG ((DEBUG_INFO, "  PT USP PCIe LC_RESET_LINK SMN Address is 0x%08X\n", LcResetLinkAddress));

    GppPcieAddress = PtSmnRead (0, LcResetLinkAddress);
    PtSmnWrite (0, LcResetLinkAddress, GppPcieAddress | BIT3);
  PtDelayUs (20 * 1000);                                                      // Delay 20 ms
    PtSmnWrite (0, LcResetLinkAddress, GppPcieAddress);

    do {
      // Check DL_ACTIVE = 1 and LINK_TRAINING = 0
      CurrentSpeed = PtPciRead8 (GppBus, GppDev, GppFun, 0x6B);                 // GPP LINK_STATUS
    } while ((CurrentSpeed & (BIT5 + BIT3)) != BIT5);
    CurrentSpeed = PtPciRead8 (GppBus, GppDev, GppFun, 0x6A);                   // GPP LINK_STATUS
  DEBUG ((DEBUG_INFO, "  PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", CurrentSpeed >> 4, CurrentSpeed & 0xF));

  // Retrain PCIe Link Speed to PcdPT21PcieTargetSpeed
  CurrentSpeed = PtPciRead8 (GppBus, GppDev, GppFun, 0x6A);                     // GPP LINK_STATUS
  DEBUG ((DEBUG_INFO, "  PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", CurrentSpeed >> 4, CurrentSpeed & 0xF));
  PTTargetSpeed = PcdGet8 (PcdPT21PcieTargetSpeed);
  if ((CurrentSpeed & 0xF) != PTTargetSpeed) {
    DEBUG ((DEBUG_INFO, "    Set PT USP PCIe Link Speed to Gen%d\n", PTTargetSpeed));
    PcieServicesPpi->PcieSetSpeed (PcieServicesPpi, PeiServices, GppDev, GppFun, PTTargetSpeed);
  }

  CurrentSpeed = PtPciRead8 (GppBus, GppDev, GppFun, 0x6A);                     // GPP LINK_STATUS
  DEBUG ((DEBUG_INFO, "  PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", CurrentSpeed >> 4, CurrentSpeed & 0xF));

  PtDelayUs (100 * 1000);  // Delay 100 ms

  // Load Secondary PT21
  if (PcdGet32 (PcdSecPT21GppPcieAddress) == 0) {
    // Restore UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    RestorePromBus (&PtPciTable, 0xBBBB);
    // Save/Init UspBus, XhciBus, XhciMmio, UspCmd, XhciCmd and ASPM
    SaveInitPromBus (&PtPciTable, 0x00BB);

    // Check USB status
    DEBUG ((DEBUG_INFO, "  Check USB status before scan second PT\n"));
    Counter = 0;
    do {
      // Check if Host Reset = 0
      UsbSts = PtMmioRead32 (XhciMmio + 0x20);
      DEBUG ((DEBUG_INFO, "    Usb Status = 0x%X Counter = %d\n", UsbSts, Counter));
      if (UsbSts & BIT1) {
        PtDelayUs (20 * 1000);                                                  // Delay 20 ms
      }
      Counter++;
    } while ((UsbSts & BIT1) && (Counter < 50));

    for (DspDevNum = 0; DspDevNum < 12; DspDevNum++) {
      ScanSecXhciDidVid (PtPciTable.PromBus + 1, DspDevNum, 0, PtPciTable.PromBus + 4);
      if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
        break;
      }
    }

    if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
      SecGppPcieAddress = PcdGet32 (PcdSecPT21GppPcieAddress);
      SecGppBus = (UINT8) ((SecGppPcieAddress >> 20) & 0xFF);
      SecGppDev = (UINT8) ((SecGppPcieAddress >> 15) & 0x1F);
      SecGppFun = (UINT8) ((SecGppPcieAddress >> 12) & 0x07);

      // Restore SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd, SecXhciCmd and SecASPM
      RestorePromBus (&PtPciTable, 0xBB00);
      // Save/Init SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd, SecXhciCmd and SecASPM
      SaveInitPromBus (&PtPciTable, 0xBB00);

      // Check if Secondary PT XHCI is locked or not
      XhciLock = FchProm21XhciReadByteV2 (SecXhciMmio, XHCI_REG_PCIE_HW_RW);
      if ((XhciLock & BIT4) == 0) {
        PcdSet8S (PcdSecPT21XhciLock, 1);
        DEBUG ((DEBUG_INFO, "    Set PcdSecPT21XhciLock = %d\n", PcdGet8 (PcdSecPT21XhciLock)));
      }

      if ((Status == EFI_SUCCESS) && (XhciLock & BIT4)) {
        DEBUG ((DEBUG_INFO, "  Secondary PT XHCI is unlock\n"));

        // Force PT DSP Gen1 before load Secondary FW
        SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);    // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d Counter = %d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF, Counter));
          if ((SecCurrentSpeed & 0x0F) == DxioGen1) {
          DEBUG ((DEBUG_INFO, "  Secondary PT DSP set to Gen1 Success !!!\n"));
        } else {
          DEBUG ((DEBUG_INFO, "  Secondary PT DSP set to Gen1 Fail !!!\n"));
        }

        // Enable/Disable Secondary PROM21 USB 3.0/2.0 Port
        DEBUG ((DEBUG_INFO, "  Enable/Disable Secondary PROM21 USB 3.0/2.0 Port\n"));
        AmdSecPtUsbPort (SecXhciMmio);

        // Enable/Disable Secondary PROM21 Specific Features
        DEBUG ((DEBUG_INFO, "  Enable/disble Secondary PROM21 Specific Features\n"));
        AmdSecPtSpecificFeatrue (SecXhciMmio);

        AGESA_TESTPOINT (TpFchSecPTStartToLoadFW, NULL);

          // Load Secondary PT-Plus FW
        Status = LibFchLpPtXhciProceedFwLoad (PeiServices,
                                              PtFwInRamPtr + 0x0C,
                                              PtPciTable.PromBus + 6, 0, 0,
                                              SecXhciMmio);

          AGESA_TESTPOINT (TpFchPTLoadFWDelay9Sec, NULL);

          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);   // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF));

          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x90);   // PT DSP LINK_CTRL
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecCurrentSpeed | BIT4);
          PtDelayUs (10 * 1000);                                                  // Delay 10 ms
          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x3E);    // PT DSP BRIDGE_CNTL
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x3E, SecTargetSpeed | BIT6);
          PtDelayUs (10 * 1000);                                                  // Delay 10 ms
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x3E, SecTargetSpeed);    // PT DSP BRIDGE_CNTL
          PtDelayUs (10 * 1000);                                                  // Delay 10 ms
          PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecCurrentSpeed);

          do {
            // Check DL_ACTIVE = 1 and LINK_TRAINING = 0
            SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x93);  // PT DSP LINK_STATUS
          } while ((SecTargetSpeed & (BIT5 + BIT3)) != BIT5);

          SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);    // PT DSP LINK_STATUS
          DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF));

          if ((SecCurrentSpeed & 0x0F) != (PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F)) {
        // Restore Original PT DSP Gen speed after loading Secondary FW
        SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0xB0);    // PT DSP LINK_CTRL_2
        SecTargetSpeed &= 0xF0;
        SecTargetSpeed |= PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F;
        PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0xB0, SecTargetSpeed);

        SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x90);    // PT DSP LINK_CTRL
            PtPciWrite8 (SecGppBus, SecGppDev, SecGppFun, 0x90, SecTargetSpeed | BIT5);

        for (Counter = 0; Counter < 50; Counter++) {
          SecTargetSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);  // PT DSP LINK_STATUS
          if ((SecTargetSpeed & 0x0F) == (PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F)) {
            break;
          }
          PtDelayUs (20 * 1000);                                                // Delay 20 ms
        }

            SecCurrentSpeed = PtPciRead8 (SecGppBus, SecGppDev, SecGppFun, 0x92);   // PT DSP LINK_STATUS
            DEBUG ((DEBUG_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d Counter = %d\n", SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF, Counter));

            if ((SecCurrentSpeed & 0x0F) == (PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F)) {
              DEBUG ((DEBUG_INFO, "  Restore Secondary PT DSP speed to Gen%d Success !!!\n", SecCurrentSpeed & 0x0F));
        } else {
              DEBUG ((DEBUG_INFO, "  Restore Secondary PT DSP speed to Gen%d Fail !!! It's Gen%d now\n", PcdGet8 (PcdPT21PcieTargetSpeed) & 0x0F, SecCurrentSpeed & 0x0F));
            }
        }

      } else {
        DEBUG ((DEBUG_INFO, "  Secondary PT XHCI is lock: bypass PT code !!!\n"));
        } // End of if (XhciLock & BIT4)
        RestorePromBus (&PtPciTable, 0xBB00);
    }
    RestorePromBus (&PtPciTable, 0x00BB);
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
  AGESA_TESTPOINT (TpFchPTLoadFWExit, NULL);

  return Status;
}

