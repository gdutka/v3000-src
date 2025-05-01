/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _MPM_ASF_H_
#define _MPM_ASF_H_
#include <AmdMpmAsf.h>

//
// Watchdog
//
#define ASF_BIOS_WATCHDOG_TIMEOUT_VALUE       0x100u  // TBD
#define ASF_OS_WATCHDOG_TIMEOUT_VALUE         0x100u  // TBD

#pragma pack(push, 2)

typedef struct {
  ASF_MSG_ID        MsgId;
  CHAR16            MsgName[128];
}ASF_MSG_NAME_MAP;
#pragma pack(pop)

ASF_MESSAGE mAsfMsgList[] = {
  {
    MsgMemIntiExit,
    ASF_MSG_TYPE_PROGRESS,
    {0}
  },
  {
    MsgCacheInitExit,
    ASF_MSG_TYPE_PROGRESS,
    {0}
  },
  {
    MsgHddInitExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_IO_BUS_ATA_ATAPI | EFI_IOB_PC_DETECT}
  },
  {
    MsgSecondProcessorInitExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_COMPUTING_UNIT_HOST_PROCESSOR | EFI_CU_HP_PC_AP_INIT}
  },
  {
    MsgUserInitiateSetupExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_PC_USER_SETUP}
  },
  {
    MsgUsbResourceConfigExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_IO_BUS_USB | EFI_P_PC_ENABLE}
  },
    {
    MsgPciResourceConfigExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_IO_BUS_PCI | EFI_IOB_PCI_BUS_ENUM}
  },
  {
    MsgVideoInitExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_PC_INIT}
  },
  {
    MsgKbdInitExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_PERIPHERAL_KEYBOARD | EFI_P_PC_INIT}
  },
  {
    MsgKbdTestExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_PERIPHERAL_KEYBOARD | EFI_P_KEYBOARD_PC_SELF_TEST}
  },
  {
    MsgCallOSWakeVectorEntry,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_SOFTWARE_PEI_MODULE | EFI_SW_PEI_PC_OS_WAKE}
  },
  {
    MsgStartOSBootEntry,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_DXE_BS_PC_LEGACY_BOOT_EVENT}
  },
  {
    MsgMotherBoardInitExit,
    ASF_MSG_TYPE_PROGRESS,
    {EFI_COMPUTING_UNIT_CHIPSET | EFI_CU_PC_INIT_BEGIN}
  },
  {
    MsgBiosBootCompleteExit,
    ASF_MSG_TYPE_PROGRESS,
    {0}
  },
  {
    MsgNoVideo,
    ASF_MSG_TYPE_ERROR,
    {EFI_PERIPHERAL_LOCAL_CONSOLE | EFI_P_EC_NOT_DETECTED}
  },
  {
    MsgKbdFailure,
    ASF_MSG_TYPE_ERROR,
    {EFI_PERIPHERAL_KEYBOARD | EFI_P_EC_NOT_DETECTED}
  },
  {
    MsgHddFailure,
    ASF_MSG_TYPE_ERROR,
    {EFI_PERIPHERAL_FIXED_MEDIA | EFI_P_EC_NOT_DETECTED}
  },
  {
    MsgNoBootMedia,
    ASF_MSG_TYPE_ERROR,
    {0}
  },
  {
    MsgSystemStateS0,
    ASF_MSG_TYPE_SYSTEM_STATE,
    {0}
  },
  {
    MsgSystemStateS3,
    ASF_MSG_TYPE_SYSTEM_STATE,
    {3}
  },
  {
    MsgSystemStateS4,
    ASF_MSG_TYPE_SYSTEM_STATE,
    {4}
  },
  {
    MsgSystemStateS5,
    ASF_MSG_TYPE_SYSTEM_STATE,
    {5}
  },
  {
    MsgStartBiosWdt,
    ASF_MSG_TYPE_START_WATCHDOG,
    {ASF_BIOS_WATCHDOG_TIMEOUT_VALUE}
  },
  {
    MsgStartOsWdt,
    ASF_MSG_TYPE_START_WATCHDOG,
    {ASF_OS_WATCHDOG_TIMEOUT_VALUE}
  },
  {
    MsgStopWdt,
    ASF_MSG_TYPE_STOP_WATCHDOG,
    {0}
  },
  {
    MsgWlanFwLoadError,
    ASF_MSG_TYPE_ERROR,
    {0x1A0}
  }
};

ASF_MSG_NAME_MAP gAsfMsgNameMap[] = {
  {MsgHddInitExit,                L"ASF - PROGRESS (Exit) - HDD INIT"},
  {MsgSecondProcessorInitExit,    L"ASF - PROGRESS (Exit) - AP Init"},
  {MsgUserInitiateSetupExit,      L"ASF - PROGRESS (Exit) - User Init Setup"},
  {MsgUsbResourceConfigExit,      L"ASF - PROGRESS (Exit) - USB Resource Config"},
  {MsgPciResourceConfigExit,      L"ASF - PROGRESS (Exit) - PCI Resource Config"},
  {MsgVideoInitExit,              L"ASF - PROGRESS (Exit) - PCI Resource Config"},
  {MsgKbdInitExit,                L"ASF - PROGRESS (Exit) - Keyboard Init"},
  {MsgKbdTestExit,                L"ASF - PROGRESS (Exit) - Keyboard Test"},
  {MsgCallOSWakeVectorEntry,      L"ASF - PROGRESS (Entry) - Call OS Wake Vector"},
  {MsgStartOSBootEntry,           L"ASF - PROGRESS (Entry) - OS Boot"},
  {MsgMotherBoardInitExit,        L"ASF - PROGRESS (Exit) - Mother Board Init"},
  {MsgBiosBootCompleteExit,       L"ASF - PROGRESS (Exit) - BIOS Boot Complete"},
  {MsgNoVideo,                    L"ASF - ERROR - NO Vdieo"},
  {MsgKbdFailure,                 L"ASF - ERROR - Keyboard Failure"},
  {MsgHddFailure,                 L"ASF - ERROR - HDD Failure"},
  {MsgNoBootMedia,                L"ASF - ERROR - No Boot Media"},
  {MsgSystemStateS0,              L"ASF - MESSAGE - System State S0"},
  {MsgSystemStateS3,              L"ASF - MESSAGE - System State S3"},
  {MsgSystemStateS4,              L"ASF - MESSAGE - System State S4"},
  {MsgSystemStateS5,              L"ASF - MESSAGE - System State S5"},
  {MsgStartBiosWdt,               L"ASF - Start System Firmware Watchdog Timer"},
  {MsgStartOsWdt,                 L"ASF - Start OS Watchdog Timer"},
  {MsgStopWdt,                    L"ASF - Stop Watchdog Timer"}
};

#endif
