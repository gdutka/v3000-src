/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _PRESILICON_CONTROL_RMB_LIB_H_
#define _PRESILICON_CONTROL_RMB_LIB_H_

#define MP0_C2PMSG_97               0x03810A84ul
#define MP0_C2PMSG_98               0x03810A88ul

// Message Definitions:
//
// C2PMSG_97  (SMN address 0x3810A84)
//
typedef union {
  struct {
    UINT32 Environment:4;                  ///< Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite.  Replaces UserDataPattern0 force
    UINT32 CoreClNum:3;                    ///< Core CL selector: Varies from release to release.  Replaces DF scratch 30[23:0] force
    UINT32 DisablePmuDevinitUseSwmra:1;    ///< Disable DEVINIT and use SWMRA instead. Replaces UmcCfgEco[19] force
    UINT32 OsBackdoorLoad:1;               ///< OS backdoor load to DRAM.  New feature bit.
    UINT32 SkipAllUSBControllerAccess:1;   ///< When set FW will skip all USB Control init communication from the FW
    UINT32 SkipAllUSBPhyAccess:1;          ///< When set FW will skip all USB phy communication from the FW
    UINT32 SkipDxioInitializaion:1;        ///<When set FW will skip DXIO Initialization from the FW>
    UINT32 :20;                            ///< Reserved
  } Field;
  UINT32  Value;
} PRESIL_CTRL0_RMB;

#define PRESIL_CTRL_ENV_HW            0
#define PRESIL_CTRL_ENV_SOC_EMULATION 1
#define PRESIL_CTRL_ENV_COBRA         2
#define PRESIL_CTRL_ENV_GIO           3
#define PRESIL_CTRL_ENV_GC_LITE       4

//
// C2PMSG_98  (SMN address 0x3810A88)
//
typedef union {
  struct {
    UINT32 DisableMemtest:1;               ///< Disable the memory test.  Replaces UmcCfgEco[16] force.  DEPRECATING: Will move to APCB
    UINT32 DisableMemclear:1;              ///< Disable the memory clear.  Replaces DF scratch 30[31] force.  DEPRECATING: Already in APCB
    UINT32 DisableMemSetReg:1;             ///< Controls logging of UMC/PMU register writes.  Replaces UmcCfgEco[17] force.  DEPRECATING: Will create a filter in APCB
    UINT32 DisableMemtestVerbose:1;        ///< Controls logging of memtest.  Replaces UmcCfgEco[17] force.  DEPRECATING: Will create a filter in APCB
    UINT32 DisableExtSpd:1;                ///< Disable SPD read attempts across SMBus.  Replaces UmcCfgEco[20] force.  DEPRECATING: Will move to APCB
    UINT32 DisableRrwMemtest:1;            ///< Disable reliable read-write memtest.  Replaces UmcCfgEco[21] force.  DEPRECATING: Will move to APCB
    UINT32 DisableSettingUnusedChannel:1;  ///< Disable accesses to mem channels other than 0.  Replaces UmcCfgEco[22] force.  DEPRECATING: Will consume harvest fuses
    UINT32 DisableDxioPhyFwLoad:1;         ///< Disable MP0 DXIO phy firmware loading process.  Replaces DF scratch 30[31] force.  DEPRECATING: Will try to move to APCB
    UINT32 DisableMemToUseDvSettings:1;    ///< Design verification settings.  Replaces UmcCfgEco[23] force.  DEPRECATING: Will be a build time switch
    UINT32 EnableAblVerboseMessaging:1;    ///< verbose message control; 0- no override (controlled by build config), 1- enable>
    UINT32 DisableSerialOut:1;             ///< serial out control; 0- no override (controlled by build config), 1- disable>
    UINT32 EnableIoRedirect:1;             ///< Port 80 control. Replaces C2PMSG_82[31] force; 0- no override (controlled by build config), 1- enable>
    UINT32 FASTSIM_GIO:1;                  ///< FASTSIM_GIO
    UINT32 FASTSIM_DXIO:1;                 ///< FASTSIM_DXIO
    UINT32 FASTSIM_DFLT_TBL:1;             ///< FASTSIM_DFLT_TBL
    UINT32 FASTSIM_SMU_MSGS:1;             ///< FASTSIM_SMU_MSGS
    UINT32 DISABLE_EC:1;                   ///< DISABLE_EC
    UINT32 FASTSIM_PEI_LOG:1;              ///< FASTSIM_PEI_LOG
    UINT32 FASTSIM_DXE_LOG:1;              ///< FASTSIM_DXE_LOG
    UINT32 UseUmcHardcode:1;               ///< Use Hardcoded UMC Settings
    UINT32 UseDdrPhyHardcode:1;            ///< Use Hardcoded DDR PHY Settings
    UINT32 EmulatorType:2;                 ///< Emulator Type. 00 - Palladium. 01 - Veloce.
    UINT32 UseDfHardcode:1;                ///< Use Hardcoded DV DF settings
    UINT32 DisableMp1DxioPhyFwLoad:1;      ///< Disable phy FW loading for MP1
    UINT32 :7;                             ///< Reserved
  } Field;
  UINT32  Value;
} PRESIL_CTRL1_RMB;

typedef struct {
  PRESIL_CTRL0_RMB               PreSilCtrl0;
  PRESIL_CTRL1_RMB              PreSilCtrl1;
} ENV_FLAGS_STRUCT_RMB;

 /**
  *      initialize Emulation Switch Flag
  *
  *
  *  @param[in]    VOID
  *
  *  @return EFI_SUCCESS   The function always returns EFI_SUCCESS.
  *
  **/
EFI_STATUS
InitEmulationFlagRmb (
  VOID
);

#endif

