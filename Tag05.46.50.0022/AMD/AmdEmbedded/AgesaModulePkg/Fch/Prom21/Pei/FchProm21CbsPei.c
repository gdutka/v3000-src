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

#define FILECODE FCH_PROM21_PEI_FCHPROM21CBSPEI_FILECODE

VOID
FchPromontorySvidSsid (
  IN  UINT32      XhciMmio
  )
{
  UINT32          TempSsid;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  // Write XHCI SSID
  TempSsid = PcdGet32 (PcdPT21XhciSsid);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-PcdPT21XhciSsid = 0x%08X\n", __FUNCTION__, TempSsid));
  FchProm21XhciWriteDWord (XhciMmio, PROM21_XHCI_SSID_REG, TempSsid);

  // Write AHCI SSID
  TempSsid = PcdGet32 (PcdPT21AhciSsid);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-PcdPT21AhciSsid = 0x%08X\n", __FUNCTION__, TempSsid));
  FchProm21XhciWriteDWord (XhciMmio, PROM21_AHCI_SSID_REG, TempSsid);

  // Write PCIe USP SSID
  TempSsid = PcdGet32 (PcdPT21PcieUspSsid);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-PcdPT21PcieUspSsid = 0x%08X\n", __FUNCTION__, TempSsid));
  FchProm21XhciWriteDWord (XhciMmio, PROM21_USP_SSID_REG, TempSsid);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}

VOID
FchSecPromontorySvidSsid (
  IN  UINT32      XhciMmio
  )
{
  UINT32          TempSsid;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  // Write XHCI SSID
  TempSsid = PcdGet32 (PcdSecPT21XhciSsid);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-PcdPT21XhciSsid = 0x%08X\n", __FUNCTION__, TempSsid));
  FchProm21XhciWriteDWord (XhciMmio, PROM21_XHCI_SSID_REG, TempSsid);

  // Write AHCI SSID
  TempSsid = PcdGet32 (PcdSecPT21AhciSsid);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-PcdPT21AhciSsid = 0x%08X\n", __FUNCTION__, TempSsid));
  FchProm21XhciWriteDWord (XhciMmio, PROM21_AHCI_SSID_REG, TempSsid);

  // Write PCIe USP SSID
  TempSsid = PcdGet32 (PcdSecPT21PcieUspSsid);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-PcdPT21PcieUspSsid = 0x%08X\n", __FUNCTION__, TempSsid));
  FchProm21XhciWriteDWord (XhciMmio, PROM21_USP_SSID_REG, TempSsid);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}

VOID
FchSecProm21ThermalPei (
  IN      UINT32    XhciMmio
)
{
  UINT8 ThermalThresholdVal;
  UINT8 RegMisc;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  if (PcdGet8(PcdSecPT21ThermalThrottle) != 0xf) {
    if (PcdGet8(PcdSecPT21ThermalThrottle) == 1) {
      ThermalThresholdVal = PcdGet8(PcdSecPT21ThermalThreshold);
      // Set specific features
      FchProm21XhciWriteDWord (XhciMmio, 0x1C520, 0x12345678);
      FchProm21XhciWriteByte(XhciMmio, 0x1E521, ThermalThresholdVal);
      RegMisc = FchProm21XhciReadByte(XhciMmio, 0x1C51F);
      FchProm21XhciWriteByte(XhciMmio, 0x1C51F, RegMisc|BIT1);
    } else {
      FchProm21XhciWriteDWord (XhciMmio, 0x1C520, 0x12345678);
      RegMisc = FchProm21XhciReadByte(XhciMmio, 0x1C51F);
      FchProm21XhciWriteByte(XhciMmio, 0x1C51F, RegMisc&(~BIT1));
    }
  }
}

VOID
FchProm21ThermalPei (
  IN      UINT32   XhciMmio
)
{
  UINT8 ThermalThresholdVal;
  UINT8 RegMisc;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  if (PcdGet8(PcdPT21ThermalThrottle) != 0xf) {
    if (PcdGet8(PcdPT21ThermalThrottle) == 1) {
      ThermalThresholdVal = PcdGet8(PcdPT21ThermalThreshold);
      // Set specific features
      FchProm21XhciWriteDWord (XhciMmio, 0x1C520, 0x12345678);
      FchProm21XhciWriteByte(XhciMmio, 0x1E521, ThermalThresholdVal);
      RegMisc = FchProm21XhciReadByte(XhciMmio, 0x1C51F);
      FchProm21XhciWriteByte(XhciMmio, 0x1C51F, RegMisc|BIT1);
    } else {
      FchProm21XhciWriteDWord (XhciMmio, 0x1C520, 0x12345678);
      RegMisc = FchProm21XhciReadByte(XhciMmio, 0x1C51F);
      FchProm21XhciWriteByte(XhciMmio, 0x1C51F, RegMisc&(~BIT1));
    }
  }
}

VOID FchProm21DSPortSetting(
IN    UINT32   XhciMmio,
IN    BOOLEAN  Enable,
IN    UINTN    PortNum
)
{
  UINT32  Address;
  UINT16  Data16;
  UINT8   Value8;

  if(PortNum > PROM21_MAX_DSP_NUMBER) {
    return;
  }
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  //Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  //
  Address = PROM21_DSP_DISABLE_REG;
  Data16 = FchProm21XhciReadWord(XhciMmio, Address);

  switch(Enable)
  {
    case 1:
      Data16 |= (1 << PortNum);
      break;
    case 0:
      Data16 &= (~ (1 << PortNum));
      break;
    default:
      Data16 |= (1 << PortNum);
      break;
  }

  FchProm21XhciWriteWord(XhciMmio, Address, Data16);

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));

}

VOID FchProm21PcieConfig (
IN    UINT32  XhciMmio
)
{
  if(PcdGet8 (PcdPT21PciePort0Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort0Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 0);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 0);
    }
  }

  if(PcdGet8 (PcdPT21PciePort1Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort1Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 1);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 1);
    }
  }

  if(PcdGet8 (PcdPT21PciePort2Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort2Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 2);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 2);
    }
  }

  if(PcdGet8 (PcdPT21PciePort3Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort3Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 3);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 3);
    }
  }

  if(PcdGet8 (PcdPT21PciePort4Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort4Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 4);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 4);
    }
  }

  if(PcdGet8 (PcdPT21PciePort5Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort5Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 5);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 5);
    }
  }

  if(PcdGet8 (PcdPT21PciePort6Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort6Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 6);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 6);
    }
  }

  if(PcdGet8 (PcdPT21PciePort7Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort7Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 7);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 7);
    }
  }

  if(PcdGet8 (PcdPT21PciePort8Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort8Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 8);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 8);
    }
  }

  if(PcdGet8 (PcdPT21PciePort9Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort9Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 9);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 9);
    }
  }

  if(PcdGet8 (PcdPT21PciePort10Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort10Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 10);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 10);
    }
  }

  if(PcdGet8 (PcdPT21PciePort11Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort11Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 11);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 11);
    }
  }

  if(PcdGet8 (PcdPT21PciePort12Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort12Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 12);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 12);
    }
  }

  if(PcdGet8 (PcdPT21PciePort13Enable) != 0xf) {
    if(PcdGet8 (PcdPT21PciePort13Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 13);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 13);
    }
  }
}

VOID FchSecProm21PcieConfig (
IN    UINT32  XhciMmio
)
{
  if(PcdGet8 (PcdSecPT21PciePort0Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort0Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 0);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 0);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort1Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort1Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 1);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 1);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort2Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort2Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 2);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 2);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort3Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort3Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 3);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 3);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort4Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort4Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 4);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 4);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort5Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort5Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 5);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 5);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort6Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort6Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 6);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 6);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort7Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort7Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 7);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 7);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort8Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort8Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 8);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 8);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort9Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort9Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 9);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 9);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort10Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort10Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 10);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 10);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort11Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort11Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 11);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 11);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort12Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort12Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 12);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 12);
    }
  }

  if(PcdGet8 (PcdSecPT21PciePort13Enable) != 0xf) {
    if(PcdGet8 (PcdSecPT21PciePort13Enable) == 1) {
      FchProm21DSPortSetting(XhciMmio, 1, 13);
    } else {
      FchProm21DSPortSetting(XhciMmio, 0, 13);
    }
  }
}

VOID
SetSecProm21PciePortsPcd (
  IN    UINTN    PortNum,
  IN    UINT8    PcdValue
)
{
  if(PortNum > PROM21_MAX_DSP_NUMBER) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- %d, %x\n", __FUNCTION__, PortNum, PcdValue));
  switch (PortNum) {
    case 0:
      PcdSet8S (PcdSecPT21PciePort0Enable, PcdValue);
      break;
    case 1:
      PcdSet8S (PcdSecPT21PciePort1Enable, PcdValue);
      break;
    case 2:
      PcdSet8S (PcdSecPT21PciePort2Enable, PcdValue);
      break;
    case 3:
      PcdSet8S (PcdSecPT21PciePort3Enable, PcdValue);
      break;
    case 4:
      PcdSet8S (PcdSecPT21PciePort4Enable, PcdValue);
      break;
    case 5:
      PcdSet8S (PcdSecPT21PciePort5Enable, PcdValue);
      break;
    case 6:
      PcdSet8S (PcdSecPT21PciePort6Enable, PcdValue);
      break;
    case 7:
      PcdSet8S (PcdSecPT21PciePort7Enable, PcdValue);
      break;
    case 8:
      PcdSet8S (PcdSecPT21PciePort8Enable, PcdValue);
      break;
    case 9:
      PcdSet8S (PcdSecPT21PciePort9Enable, PcdValue);
      break;
    case 10:
      PcdSet8S (PcdSecPT21PciePort10Enable, PcdValue);
      break;
    case 11:
      PcdSet8S (PcdSecPT21PciePort11Enable, PcdValue);
      break;
    case 12:
      PcdSet8S (PcdSecPT21PciePort12Enable, PcdValue);
      break;
    case 13:
      PcdSet8S (PcdSecPT21PciePort13Enable, PcdValue);
      break;
  }
}


VOID
SetProm21PciePortsPcd (
  IN    UINTN    PortNum,
  IN    UINT8    PcdValue
)
{
  if(PortNum > PROM21_MAX_DSP_NUMBER) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- %d, %x\n", __FUNCTION__, PortNum, PcdValue));
  switch (PortNum) {
    case 0:
      PcdSet8S (PcdPT21PciePort0Enable, PcdValue);
      break;
    case 1:
      PcdSet8S (PcdPT21PciePort1Enable, PcdValue);
      break;
    case 2:
      PcdSet8S (PcdPT21PciePort2Enable, PcdValue);
      break;
    case 3:
      PcdSet8S (PcdPT21PciePort3Enable, PcdValue);
      break;
    case 4:
      PcdSet8S (PcdPT21PciePort4Enable, PcdValue);
      break;
    case 5:
      PcdSet8S (PcdPT21PciePort5Enable, PcdValue);
      break;
    case 6:
      PcdSet8S (PcdPT21PciePort6Enable, PcdValue);
      break;
    case 7:
      PcdSet8S (PcdPT21PciePort7Enable, PcdValue);
      break;
    case 8:
      PcdSet8S (PcdPT21PciePort8Enable, PcdValue);
      break;
    case 9:
      PcdSet8S (PcdPT21PciePort9Enable, PcdValue);
      break;
    case 10:
      PcdSet8S (PcdPT21PciePort10Enable, PcdValue);
      break;
    case 11:
      PcdSet8S (PcdPT21PciePort11Enable, PcdValue);
      break;
    case 12:
      PcdSet8S (PcdPT21PciePort12Enable, PcdValue);
      break;
    case 13:
      PcdSet8S (PcdPT21PciePort13Enable, PcdValue);
      break;
  }
}

UINT16
SetGetPcdPciePort (
  IN    UINT8   DevNum,
  IN    UINT8   Selection
  )
{
  UINT16        PciePortMap = 0;

  if (DevNum <= PROM21_MAX_DSP_NUMBER) {
    if (Selection < 2) {
      switch (DevNum) {
        case 0:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort0Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort0Enable, Selection);
          break;
        case 1:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort1Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort1Enable, Selection);
          break;
        case 2:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort2Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort2Enable, Selection);
          break;
        case 3:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort3Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort3Enable, Selection);
          break;
        case 4:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort4Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort4Enable, Selection);
          break;
        case 5:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort5Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort5Enable, Selection);
          break;
        case 6:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort6Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort6Enable, Selection);
          break;
        case 7:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort7Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort7Enable, Selection);
          break;
        case 8:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort8Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort8Enable, Selection);
          break;
        case 9:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort9Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort9Enable, Selection);
          break;
        case 10:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort10Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort10Enable, Selection);
          break;
        case 11:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort11Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort11Enable, Selection);
          break;
        case 12:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort12Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort12Enable, Selection);
          break;
        case 13:
          DEBUG ((DEBUG_INFO, "  PcdSet8S (PcdPT21PciePort13Enable, %d)\n", Selection));
          PcdSet8S (PcdPT21PciePort13Enable, Selection);
          break;
      }
    } else {
      switch (DevNum) {
        case 0:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort0Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort0Enable) = 0x%04X\n", PciePortMap));
          break;
        case 1:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort1Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort1Enable) = 0x%04X\n", PciePortMap));
          break;
        case 2:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort2Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort2Enable) = 0x%04X\n", PciePortMap));
          break;
        case 3:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort3Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort3Enable) = 0x%04X\n", PciePortMap));
          break;
        case 4:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort4Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort4Enable) = 0x%04X\n", PciePortMap));
          break;
        case 5:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort5Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort5Enable) = 0x%04X\n", PciePortMap));
          break;
        case 6:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort6Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort6Enable) = 0x%04X\n", PciePortMap));
          break;
        case 7:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort7Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort7Enable) = 0x%04X\n", PciePortMap));
          break;
        case 8:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort8Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort8Enable) = 0x%04X\n", PciePortMap));
          break;
        case 9:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort9Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort9Enable) = 0x%04X\n", PciePortMap));
          break;
        case 10:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort10Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort10Enable) = 0x%04X\n", PciePortMap));
          break;
        case 11:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort11Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort11Enable) = 0x%04X\n", PciePortMap));
          break;
        case 12:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort12Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort12Enable) = 0x%04X\n", PciePortMap));
          break;
        case 13:
          PciePortMap = (UINT16) PcdGet8 (PcdPT21PciePort13Enable);
          DEBUG ((DEBUG_INFO, "  PcdGet8 (PcdPT21PciePort13Enable) = 0x%04X\n", PciePortMap));
          break;
      }
      PciePortMap <<= DevNum;
      DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-DevNum = %d Selection = %d Return = 0x%04X\n", __FUNCTION__, DevNum, Selection, PciePortMap));
    } // End of if (Selection < 2)
  } // End of if (DevNum < 10)

  return PciePortMap;
}

VOID
FchProm21DisableUnusedPciePorts (
  IN    UINT32   GppPcieAddress,
  IN    UINTN    PromNo
  )
{
  UINT32                VidDid;
  UINT8                 DevNum;
  UINT8                 SlotStatus;
  UINT8                 HotPlugCapable;
  UINT8                 GppBus;
  UINT8                 GppDev;
  UINT8                 GppFun;
  UINT8                 PtUspBus;
  UINT8                 PtDspBus;

  GppBus = (UINT8) ((GppPcieAddress >> 20) & 0xFF);
  GppDev = (UINT8) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (UINT8) ((GppPcieAddress >> 12) & 0x07);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-[%02X:%02X:%02X]\n", __FUNCTION__, GppBus, GppDev, GppFun));

  PtUspBus = PtPciRead8 (GppBus, GppDev, GppFun, 0x19);
  VidDid = PtPciRead32 (PtUspBus, 0, 0, 0x00);
  DEBUG ((DEBUG_INFO, "PtUspBus %x, VidDid %x\n", PtUspBus, VidDid));

  if (VidDid == PT21_USP_ID) {
    PtDspBus = PtPciRead8 (PtUspBus, 0, 0, 0x19);
    VidDid = PtPciRead32 (PtDspBus, 0, 0, 0x00);
    DEBUG ((DEBUG_INFO, "PtDspBus %x, VidDid %x\n", PtDspBus, VidDid));
    if(VidDid == PT21_DSP_ID) {
      for (DevNum = 0; DevNum <= PROM21_MAX_DSP_NUMBER; DevNum ++) {
        SlotStatus = PtPciRead8 (PtDspBus, DevNum, 0, 0x9A);
        HotPlugCapable = PtPciRead8 (PtDspBus, DevNum, 0, 0x94);
        DEBUG ((DEBUG_INFO, "SlotStatus %x, HotPlugCapable %x at Dev0x%x\n", SlotStatus, HotPlugCapable, DevNum));
        if (((SlotStatus & 0x40) != 0x40) && ((HotPlugCapable & 0x40) != 0x40)) {
          // Disable downstream port if device is not presence and not hotplug capable
          switch (PromNo)
          {
            case 0:
              SetProm21PciePortsPcd(DevNum, 0);
              break;
            case 1:
              SetSecProm21PciePortsPcd(DevNum, 0);
              break;
          }
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}

// Promontory PCIE CBS setting
EFI_STATUS
EFIAPI
FchProm21CbsPei (
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  UINT32            XhciMmio;
  UINT32            SecXhciMmio;
  UINT32            GppPcieAddress;
  UINT32            SecGppPcieAddress;
  UINT8             Data8;
  EFI_STATUS        Status;
  EFI_BOOT_MODE     BootMode;
  PT_PCI_SAVE_RESTORE_TABLE       PtPciTable;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));
  XhciMmio = PcdGet32 (PcdPT21XhciMmio);
  SecXhciMmio = 0;

  if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
    // Save/Init UspBus, XhciBus, XhciMmio, UspCmd and XhciCmd
    // Save/Init SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd and SecXhciCmd
    SaveInitPromBus (&PtPciTable, 0x3B3B);
    SecXhciMmio = PcdGet32(PcdPT21SecXhciMmio);
  } else {
    // Save/Init UspBus, XhciBus, XhciMmio, UspCmd and XhciCmd
    SaveInitPromBus (&PtPciTable, 0x3B);
  }

  // Lock PT for S3 Resume
  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "  GetBootMode Error !!!  Set BootMode to BOOT_WITH_FULL_CONFIGURATION\n"));
    BootMode = BOOT_WITH_FULL_CONFIGURATION;
  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    DEBUG ((DEBUG_INFO, "  BootMode == BOOT_ON_S3_RESUME"));
    if(PcdGet8 (PcdPT21XhciLock)) {
    DEBUG ((EFI_D_INFO, "  Lock PT for S3 Resume !!!\n"));
    Data8 = FchProm21XhciReadByte (XhciMmio, XHCI_REG_PCIE_HW_RW);
    FchProm21XhciWriteByte (XhciMmio, XHCI_REG_PCIE_HW_RW, Data8 & ~BIT4);
  }
    if(PcdGet8 (PcdSecPT21XhciLock)) {
      DEBUG ((EFI_D_INFO, "  Lock Secondary PT for S3 Resume !!!\n"));
      Data8 = FchProm21XhciReadByte (SecXhciMmio, XHCI_REG_PCIE_HW_RW);
      FchProm21XhciWriteByte (SecXhciMmio, XHCI_REG_PCIE_HW_RW, Data8 & ~BIT4);
    }
  } else {
    if (PcdGet8(PcdPT21DisableUnusedPciePort) != 0xf) {
      if (PcdGet8(PcdPT21DisableUnusedPciePort) == 1) {
        GppPcieAddress = PcdGet32 (PcdPT21GppPcieAddress);
        SecGppPcieAddress = PcdGet32 (PcdSecPT21GppPcieAddress);
        if (GppPcieAddress) {
          FchProm21DisableUnusedPciePorts (GppPcieAddress, 0);
        }
        if (SecGppPcieAddress) {
          FchProm21DisableUnusedPciePorts (SecGppPcieAddress, 1);
        }
      }
    }
    /////////////
    // CBS Setting
    /////////////
    FchProm21UsbCbsPei (XhciMmio);
    FchProm21PcieConfig (XhciMmio);
    FchPromontorySvidSsid (XhciMmio);
    FchProm21SataCbsPei (XhciMmio);
    FchProm21ThermalPei(XhciMmio);

    if(SecXhciMmio != 0) {
      FchSecProm21UsbCbsPei (SecXhciMmio);
      FchSecProm21PcieConfig (SecXhciMmio);
      FchSecPromontorySvidSsid (SecXhciMmio);
      FchSecProm21SataCbsPei (SecXhciMmio);
      FchSecProm21ThermalPei(SecXhciMmio);
    }
  }

  if (PcdGet32 (PcdSecPT21GppPcieAddress)) {
    // Restore UspBus, XhciBus, XhciMmio, UspCmd and XhciCmd
    // Restore SecUspBus, SecXhciBus, SecXhciMmio, SecUspCmd and SecXhciCmd
    RestorePromBus (&PtPciTable, 0x3B3B);
  } else {
    // Restore UspBus, XhciBus, XhciMmio, UspCmd and XhciCmd
    RestorePromBus (&PtPciTable, 0x3B);
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchPromontoryCbsPeiInit
 *
 * Description:
 *
 * Input
 *   FfsHeader   : pointer to the firmware file system header
 *   PeiServices : pointer to the PEI service table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchProm21CbsPeiInit (
  IN        EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST  EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS                  Status;
  FCH_PROM21_INIT_PPI         *FchProm21InitPpi;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));

  if (PcdGet32 (PcdPT21XhciID) == 0) {
    DEBUG ((EFI_D_ERROR, "PT-CBSPEI-%a-End !! Promontory is NOT FOUND !!\n"));
    return EFI_SUCCESS;
  }

  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdFchProm21InitPpiGuid,
                             0,
                             NULL,
                             &FchProm21InitPpi
                             );
  ASSERT_EFI_ERROR ( Status);

  if (PcdGet8 (PcdPT21XhciLock) == 0x01) {
    DEBUG ((EFI_D_INFO, "  XHCI is lock == Skip FchProm21CbsPei!\n"));
  } else {
    DEBUG ((EFI_D_INFO, "  Call FchProm21CbsPei!\n"));
    Status = FchProm21CbsPei ((EFI_PEI_SERVICES **)PeiServices);
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return Status;
}

