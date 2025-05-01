/****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTSATA_KTSATALIB_FILECODE

#define MAX_RETRY_NUM   200
#define SATA_PORT_DETECTION_MAX_TRY 10
#include "GnbDxio.h"
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>

VOID
SataEnableWriteAccessKT (
  IN  UINT32              DieBusNum,
  IN  UINT32              Controller
  )
{
  FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00, 0xFFFFFFFE, BIT0, NULL);
}

VOID
SataDisableWriteAccessKT (
  IN  UINT32              DieBusNum,
  IN  UINT32              Controller
  )
{
  FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00, 0xFFFFFFFE, 0, NULL);
}

/**
 * FchKTSataInitEnableSata - Enable Sata Controller
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitEnableSata (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, 0xfffffffe, 0, NULL);
}

/**
 * FchKTSataInitDisableSata - Disable Sata Controller
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitDisableSata (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  //FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, 0xfffffffe, BIT0, LocalCfgPtr->StdHeader);
  //FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT11, BIT11, LocalCfgPtr->StdHeader);
  FchKTSmuService (DieBusNum, BIOSSMC_MSG_DisableSataController, Controller);
  LocalCfgPtr->FchRunTime.FchDeviceD3ColdMap &= ~(UINT32)(BIT28);    //Disable SATA D3Cold Support
}

/**
 * FchKTSataInitBootUpSata - Boot Up Sata Controller
 *
 * BIOS should send BIOSSMC_MSG_BootupSata message to PMFW
 * in boot sequence to boot up SATA:
 * 1) For platform with SataController Disabled, set Arg = 1.
 * 2) For platform with SataController enabled, set Arg = 0.

 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitBootUpSata (
  IN  UINT32   DieBusNum,
  IN  BOOLEAN  Enable,
  IN  VOID     *FchDataPtr
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];

  status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  if (Enable) {
    SmuArg[0] = 0;
  } else {
    SmuArg[0] = 1;
  }

  if (NbioSmuServiceRequestV13 (NbioPciAddress, BIOSSMC_MSG_BootupSata, SmuArg,0)) {
    status = TRUE;
  }

  return;
}

/**
 * FchKTSataInitHideSataPci - Disable Sata Controller PCI
 * configuration space.
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitHideSataPci (
  IN  UINT32   DieBusNum,
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;

  if ( LocalCfgPtr->Sata[0].SataEnable == FALSE ) {
    if (LocalCfgPtr->Xgbe.Port[0].XgbePortConfig || LocalCfgPtr->Xgbe.Port[1].XgbePortConfig) {
      //dummy Function 0
      FchSmnRW (DieBusNum, FCH_KT_RCC_DEV1_PORT_STRAP0, ~(UINT32) BIT31, BIT31, NULL);
      //RCC_STRAP:RCC_DEV1_EPF0_STRAP0
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP0, ~(UINT32) BIT28, 0, NULL);
      //RCC_STRAP:RCC_DEV1_EPF0_STRAP13
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP13, 0xFF000000, 0x00130000, NULL);
      //RCC_STRAP:RCC_DEV1_EPF0_STRAP4
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP4, 0x807FFFFF, 0, NULL);
      //RCC_STRAP:RCC_DEV1_EPF0_STRAP3
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP3, ~(UINT32) (BIT18 + BIT20 + BIT25), BIT25, NULL);
      //RCC_STRAP: need check RN NBIO team
      FchSmnRW (DieBusNum, 0x1013A204ul, ~(UINT32) (BIT0 + BIT1), (BIT0 + BIT1), NULL);
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP0, 0xFFFF0000, 0x145A, NULL);
    } else {
      FchSmnRW (DieBusNum, FCH_KT_NBIF1DEVINDCFG_IOHC_Bridge_CNTL, 0xFFFFFFFF, 0x7, NULL);
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP0, ~(UINT32)BIT28, 0, NULL);
    }
  }
}

/**
 * FchKTSataInitPortOffline - Put Sata Port in Offline mode
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] PortNum    Sata Port to put in offline.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitPortOffline (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  UINT32   PortNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                      Retry;
  UINT32                      PortStatus;
  UINT32                      SataRegAddr;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNum * 0x80;
  FchSmnRW (DieBusNum, SataRegAddr, 0xFFFFFFF0, 0x4, NULL);

  for (Retry = 0; Retry < MAX_RETRY_NUM; Retry++) {
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG128 + PortNum * 0x80;
    FchSmnRead (DieBusNum, SataRegAddr, &PortStatus, NULL);
    if ((PortStatus & 0x0F) == 0x4) {
      break;
    }
    FchStall (10, NULL);
  }
}

/**
 * FchKTSataInitPortActive - Put Sata Port back to active from
 * Offline mode
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] PortNum    Sata Port to put in offline.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitPortActive (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  UINT32   PortNum,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                      SataRegAddr;
  FCH_RESET_DATA_BLOCK        *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  SataRegAddr = FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00;
  FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) (BIT16 << PortNum), 0, NULL);
  SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNum * 0x80;
  FchSmnRW (DieBusNum, SataRegAddr, 0xFFFFFFF0, 0x1, NULL);
  FchStall (1000, NULL);
  SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNum * 0x80;
  FchSmnRW (DieBusNum, SataRegAddr, 0xFFFFFFF0, 0x0, NULL);
}


/**
 * FchKTSataInitStaggeredSpinStep2  - Step 2 of optional
 * Staggered Spin Up feature.
 *
 *
 * @param[in] DieBusNum  IOCH bus number on current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitStaggeredSpinStep2 (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  UINT32                    PortNum;
  FCH_RESET_DATA_BLOCK      *LocalCfgPtr;
  LocalCfgPtr = (FCH_RESET_DATA_BLOCK *)FchDataPtr;

  for (PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++) {
    FchSmnRW (DieBusNum, FCH_KT_SATA_AOAC_CONTROL, ~(UINT32) (BIT0 << PortNum), 0x00, NULL);
  }
}

/**
 * FchKTSataInitEnableErr - Enable Error reporting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitEnableErr (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN VOID      *FchDataPtr
  )
{
  UINT32                 PortNum;
  UINT32                 SataRegAddr;
  BOOLEAN                FchD2HFifoParityErr;
  BOOLEAN                FchH2DFifoParityErr;
  BOOLEAN                FchContextMemoryParityErr;
  BOOLEAN                FchSataRasSupport;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchSataRasSupport = LocalCfgPtr->Sata[0].SataRasSupport;
  FchD2HFifoParityErr = TRUE;          //Optional
  FchH2DFifoParityErr = TRUE;          //Optional
  FchContextMemoryParityErr = TRUE;    //Optional

  if ( FchSataRasSupport ) {
    //
    // (Optional) Enable Report D2H FIFO Parity Error to System for each port.
    //
    if ( FchD2HFifoParityErr ) {
      for ( PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++ ) {
        SataRegAddr = FCH_KT_SMN_SATA_CONTROL_SLOR + PortNum * 0x20 + 0x88;
        FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) BIT4, BIT4, StdHeader);
      }
    }

    //
    // (Optional) Enable Report H2D FIFO Parity Error to System for each port
    //
    if ( FchH2DFifoParityErr ) {
      for ( PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++ ) {
        SataRegAddr = FCH_KT_SMN_SATA_CONTROL_SLOR + PortNum * 0x20 + 0x88;
        FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) BIT5, BIT5, StdHeader);
      }
    }

    //
    // (Optional) Enable Report Context Memory Parity Error to System for each port
    //
    if ( FchContextMemoryParityErr ) {
      for ( PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++ ) {
        SataRegAddr = FCH_KT_SMN_SATA_CONTROL_SLOR + PortNum * 0x20 + 0x88;
        FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) BIT6, BIT6, StdHeader);
      }
    }
  }

  //
  //Clear error status
  //
  for ( PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++ ) {
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG130 + PortNum * 0x80;
    FchSmnRW (DieBusNum, SataRegAddr, 0xFFFFFFFF, 0xFFFFFFFF, StdHeader);
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG110 + PortNum * 0x80;
    FchSmnRW (DieBusNum, SataRegAddr, 0xFFFFFFFF, 0x00, StdHeader);
  }
}

/**
 * FchKTSataInitEsata - Enable eSATA port
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitEsata (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN VOID      *FchDataPtr
  )
{
  UINT32                 SataRegAddr;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // SATA ESP port setting
  // These config bits are set for SATA driver to identify which ports are external SATA ports and need to
  // support hotplug. If a port is set as an external SATA port and need to support hotplug, then driver will
  // not enable power management (HIPM & DIPM) for these ports.
  //
  if ( LocalCfgPtr->Sata[0].SataEspPort != 0 ) {
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF8;
    FchSmnRW (DieBusNum, SataRegAddr, ~(LocalCfgPtr->Sata[0].SataEspPort), 0, StdHeader);
    FchSmnRW (DieBusNum, SataRegAddr, 0xFF00FFFF, (LocalCfgPtr->Sata[0].SataEspPort << 16), StdHeader);
    //
    // External SATA Port Indication Registers
    // If any of the ports was programmed as an external port, HCAP.SXS should also be set
    //
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGFC;
    FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) (BIT20), BIT20, StdHeader);
    //
    // Enable  eSATA PHY power-saving when no device presented.
    //
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_SLOR + 0x10;
    FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) (BIT29), BIT29, StdHeader);
  } else {
    //
    // External SATA Port Indication Registers
    // If any of the ports was programmed as an external port, HCAP.SXS should also be set (Clear for no ESP port)
    //
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF8;
    FchSmnRW (DieBusNum, SataRegAddr, 0xFF00FF00, 0x00, StdHeader);
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGFC;
    FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) (BIT20), 0x00, StdHeader);
    SataRegAddr = FCH_KT_SMN_SATA_CONTROL_SLOR + 0x10;
    FchSmnRW (DieBusNum, SataRegAddr, ~(UINT32) (BIT29), 0, StdHeader);

    //
    // clear PME
    //
    switch ( Controller ) {
    case 0:    //Sata0
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP4, 0xF07FFFFF, 0x00, StdHeader);  //Clear SATA0 PME_SUPPORT
      break;

    default:
      break;
    }
  }
}

/**
 * FchKTSataInitDevSlpPad - init DevSlp PAD pin
 *
 *
 * @param[in] DieBusNum       Bus Number of current Die
 * @param[in] PadNum          Pad number to be enabled
 * @param[in] ControllerNum   Controller number which decide the IOMUX to PadNum
 *
 */
VOID
FchKTSataEnableDevSlpPad (
  IN  UINT32   DieBusNum,
  IN  UINT8    PadNum,
  IN  UINT8    ControllerNum
  )
{
  if ( PadNum == 0 ) {
    if (ControllerNum == 0){
      //When SATA0 port0/1 map to DEVSLP pad0, configure pad as DEVSLP0 which only for SATA0
      RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG05, AccessWidth8, 0x00, 0x1);
    }
    // Set Internal PU
    RwMem (
      ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_014_BP_AGPIO5,
      AccessWidth32,
      ~(UINT32) (BIT20 + BIT21 + BIT22 + BIT23),
      BIT20 + BIT23
      );
  } else if ( PadNum == 1 ) {
    if (ControllerNum == 0){
      //When SATA0 port0/1 map to DEVSLP pad1, configure pad as DEVSLP1 which only for SATA0
      RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG06, AccessWidth8, 0x00, 0x1);
    }
    // Set Internal PU
    RwMem (
      ACPI_MMIO_BASE + GPIO_BANK0_BASE + FCH_GPIO_018_BP_AGPIO6,
      AccessWidth32,
      ~(UINT32) (BIT20 + BIT21 + BIT22 + BIT23),
      BIT20 + BIT23
      );
  }
}

/**
 * FchKTSataDisableDevSlpPad - init DevSlp PAD pin
 *
 *
 * @param[in] DieBusNum       Bus Number of current Die
 * @param[in] PadNum          Pad number to be disabled
 *
 */
VOID
FchKTSataDisableDevSlpPad (
  IN  UINT32   DieBusNum,
  IN  UINT8    PadNum
  )
{
  if ( PadNum == 0 ) {
    // Disable Pad0
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG05, AccessWidth8, 0x00, 0x00);
  } else if ( PadNum == 1 ) {
    // Dsiable Pad1
    RwMem (ACPI_MMIO_BASE + IOMUX_BASE + FCH_REG06, AccessWidth8, 0x00, 0x00);
  }
}

/**
 * FchKTSataInitDevSlp - init DevSlp configuration
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitDevSlp (
  IN  UINT32   DieBusNum,
  IN VOID      *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT32                 PortToPad0;              //Port number that will map to DEVSLP Pad0
  UINT32                 PortToPad1;              //Port number that will map to DEVSLP Pad1

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  PortToPad0 = LocalCfgPtr->Sata[0].SataDevSlpPort0Num % 8;
  PortToPad1 = LocalCfgPtr->Sata[0].SataDevSlpPort1Num % 8;

  if ((!LocalCfgPtr->Sata[0].SataDevSlpPort0) && (!LocalCfgPtr->Sata[0].SataDevSlpPort1)){
    // Clear PxDEVSLP_DSP and CFG_CAP2_SDS.
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF4, 0xFFFF00E0, 0x00, StdHeader);
    return;    //no DEVSLP supported, returned here
  }

  if ((LocalCfgPtr->Sata[0].SataDevSlpPort0) && (LocalCfgPtr->Sata[0].SataDevSlpPort1)){
    if (PortToPad1 == PortToPad0){
      return;   //same SATA port map to both DEVSLP pads, returned here
    }
  }

  //
  // 1, Enable the DEVLSP related GPIO pad and put the it into open-drain mode
  // 2, Enable the capability bit for DevSlp support
  // 3, Program CFG_PxCMD2.DSP bit for the port which will support DEVSLP.
  // 4, Program PortX_DEVSLP_MAP to route any two ports to external PAD.
  //
  if ( LocalCfgPtr->Sata[0].SataDevSlpPort0 ) {
    FchKTSataEnableDevSlpPad (DieBusNum, 0, 0);
    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF4,
      0xFFFFFFEF,
      BIT4 + (BIT8 << PortToPad0),
      StdHeader
      );
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x8C, 0xFFFFEFFF, (UINT32) (PortToPad0 << 12), StdHeader);
  } else {
    FchKTSataDisableDevSlpPad (DieBusNum, 0);
    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF4,
      ~(UINT32) (BIT8 << PortToPad0),
      0x00,
      StdHeader
      );
  }

  if ( LocalCfgPtr->Sata[0].SataDevSlpPort1 ) {
    FchKTSataEnableDevSlpPad (DieBusNum, 1, 0);
    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF4,
      0xFFFFFFEF,
      BIT4 + (BIT8 << PortToPad1),
      StdHeader
      );
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0xAC, 0xFFFFEFFF, (UINT32) (PortToPad1 << 12), StdHeader);
  } else {
    FchKTSataDisableDevSlpPad (DieBusNum, 1);
    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF4,
      ~(UINT32) (BIT8 << PortToPad1),
      0x00,
      StdHeader
      );
  }
}

/**
 * FchKTSataInitMpssMap - Program Mechanical Presence Switch
 * State Mapping.
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitMpssMap (
  IN  UINT32   DieBusNum,
  IN VOID      *FchDataPtr
  )
{
  UINT32                 PortNum;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  for (PortNum = 0; PortNum < KEITH_SATA_PORT_NUM; PortNum++) {
    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_SLOR + PortNum * 0x20 + 0x8C,
      0xFFFFF8FF,
      (UINT32) (PortNum << 8),
      StdHeader
      );
  }
}

/**
 * FchKTSataInitRsmuCtrl - Sata Rsmu Control register setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitRsmuCtrl (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN VOID      *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  //
  // SATA controller operates in maximum Gen2
  //
  if ( LocalCfgPtr->Sata[0].SataSetMaxGen2 ) {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT2, BIT2, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT2, 0x00, StdHeader);
  }

  //
  // Sata Target Support 8 devices function
  //
  if ( LocalCfgPtr->Sata[0].SataTargetSupport8Device ) {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT3, BIT3, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT3, 0x00, StdHeader);
  }

  //
  // Sata Generic Mode setting
  //
  if ( LocalCfgPtr->Sata[0].SataDisableGenericMode ) {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT1, BIT1, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT1, 0x00, StdHeader);
  }

  //
  // OOB Detection Enhancement
  //
  if ( LocalCfgPtr->Sata[0].SataOobDetectionEnh ) {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT24, BIT24, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT24, 0x00, StdHeader);
  }
}

/**
 * FchKTSataInitCtrlReg - Sata Rsmu Control register setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitCtrlReg (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN VOID      *FchDataPtr
  )
{
  UINT32       AndMaskDword;
  UINT32       OrMaskDword;
  UINT8        FchSataAggrLinkPmCap;
  UINT8        FchSataPortMultCap;
  UINT8        FchSataPscCap;
  UINT8        FchSataSscCap;
  UINT8        FchSataFisBasedSwitching;
  UINT8        FchSataCccSupport;
  UINT8        FchSataAhciEnclosureManagement;
  BOOLEAN      FchSataMsiEnable;
  UINT8        PortNumByte;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  FchSataAggrLinkPmCap = (UINT8) LocalCfgPtr->Sata[0].SataAggrLinkPmCap;
  FchSataPortMultCap = (UINT8) LocalCfgPtr->Sata[0].SataPortMultCap;
  FchSataPscCap = (UINT8) LocalCfgPtr->Sata[0].SataPscCap;
  FchSataSscCap = (UINT8) LocalCfgPtr->Sata[0].SataSscCap;
  FchSataFisBasedSwitching = (UINT8) LocalCfgPtr->Sata[0].SataFisBasedSwitching;
  FchSataCccSupport = (UINT8) LocalCfgPtr->Sata[0].SataCccSupport;
  FchSataAhciEnclosureManagement = (UINT8) LocalCfgPtr->Sata[0].SataAhciEnclosureManagement;
  FchSataMsiEnable = LocalCfgPtr->Sata[0].SataMsiEnable;

  AndMaskDword = 0;
  OrMaskDword = 0;

  if ( !FchSataPortMultCap ) {
    AndMaskDword |= BIT12;
  }

  if ( FchSataFisBasedSwitching ) {
    OrMaskDword |= BIT10;
  } else {
    AndMaskDword |= BIT10;
  }

  if ( FchSataAggrLinkPmCap ) {
    OrMaskDword |= BIT11;
  } else {
    AndMaskDword |= BIT11;
  }

  if ( FchSataPscCap ) {
    OrMaskDword |= BIT1;
  } else {
    AndMaskDword |= BIT1;
  }

  if ( FchSataSscCap ) {
    OrMaskDword |= BIT26;
  } else {
    AndMaskDword |= BIT26;
  }

  //
  // Disabling CCC (Command Completion Coalescing) support.
  //
  if ( FchSataCccSupport ) {
    OrMaskDword |= BIT19;
  } else {
    AndMaskDword |= BIT19;
  }

  if ( FchSataAhciEnclosureManagement ) {
    OrMaskDword |= BIT27;
  } else {
    AndMaskDword |= BIT27;
  }

  FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGFC, ~AndMaskDword, OrMaskDword, StdHeader);

  if ( FchSataFisBasedSwitching ) {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT12, BIT12, StdHeader);
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x10, ~(UINT32) BIT9, BIT9, StdHeader);
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF8, 0x00FFFFFF, 0xFF000000, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SATA_MISC_CONTROL, ~(UINT32) BIT12, 0x00, StdHeader);
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x10, ~(UINT32) BIT9, 0x00, StdHeader);
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REGF8, 0x00FFFFFF, 0x00, StdHeader);
  }

  if ( LocalCfgPtr->Sata[0].BiosOsHandOff == 1 ) {
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG24, ~(UINT32) BIT0, BIT0, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG24, ~(UINT32) BIT0, 0x00, StdHeader);
  }

  //
  // Disable Prefetch In Ahci Mode
  //
  if ( LocalCfgPtr->Sata[0].SataAhciDisPrefetchFunction ) {
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00, ~(UINT32) BIT13, BIT13, StdHeader);
  } else {
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00, ~(UINT32) BIT13, 0x00, StdHeader);
  }

  //
  // MSI Capability could be disabled by setting register nBIF1:RCC_DEV0_EPF2_STRAP3.STRAP_MSI_EN_DEV0_F2 to 0.
  //
  if ( FchSataMsiEnable ) {
    switch ( Controller ) {
    case 0:    //Sata0
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP3, ~(UINT32) BIT18, BIT18, StdHeader);
      break;

    default:
      break;
    }
  } else {
    switch ( Controller ) {
    case 0:    //Sata0
      FchSmnRW (DieBusNum, FCH_KT_SMN_SATA0_NBIF_STRAP3, ~(UINT32) BIT18, 0x00, StdHeader);
      break;

    default:
      break;
    }
  }

  //
  // Shutdown ports
  //
  for ( PortNumByte = 0; PortNumByte < KEITH_SATA_PORT_NUM; PortNumByte++   ) {
    if ( (LocalCfgPtr->Sata[0].SataPortPower) & (1 << PortNumByte) ) {
      FchKTSataInitPortOffline (DieBusNum, Controller, PortNumByte, FchDataPtr);
    }
  }
  FchSmnRW (
    DieBusNum,
    FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00,
    0xFF00FFFF,
    (LocalCfgPtr->Sata[0].SataPortPower << 16),
    StdHeader
    );

  //
  // Mark PHY_Ownedby_Sata
  //
  FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_SLOR + 0x200, ~(UINT32)BIT0, BIT0, StdHeader);
}

/**
 * FchKTSataInitMMC - Sata MSI MMC setting
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataInitMMC (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN VOID      *FchDataPtr
  )
{
  UINT32                 SataPciMmc;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8    BusId;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  SataPciMmc = 0;

  if ( LocalCfgPtr->Sata[0].SataMsiEnable ) {
    //
    // Get Sata PCI MSI.MMC setting
    //
    switch ( Controller ) {
    case 0:    //Sata0
      FchSmnRead (DieBusNum, FCH_KT_SATA0_SMN_PCICFG + 0xA0, &SataPciMmc, StdHeader);
      break;
    case 1:    //Sata1
      FchSmnRead (DieBusNum, FCH_KT_SATA1_SMN_PCICFG + 0xA0, &SataPciMmc, StdHeader);
      break;
    default:
      break;
    }
    //
    // Sata MMC programming
    //
    // nBif MMC at bit[19:17], SATA_Rsmux0A MMC bits at [26:24]
    SataPciMmc = (SataPciMmc & 0x000E0000) << 7;
    FchSmnRW (DieBusNum, FCH_KT_SATA_EVENT_SELECT, 0xF8FFFFFF, SataPciMmc, StdHeader);
#define BIOSSMC_MSG_SetupSataPciMsiEnTrap               0x41
//Arg0 = Address_low  //address[31:0]
//Arg0 = Address_high //address[48:32]
//BIOS needs to send the address low (in this case = 0x1050_0000)
//and PMFW will add the address high and the offset itself:
//Trap Addr = {0xffff_fffe, 0x1,read_dev0_cfg(BDF is 0:8.2).
//offset 0x18[15:8](SUB_BUS_NUMBER_LATENCY. SECONDARY_BUS),00,0xa0}

    ReadPci ((((8 << 3) + 2) << 16) + FCH_CFG_REG18 + 1, AccessWidth8, &BusId, NULL);

    FchKTSmuService (0, BIOSSMC_MSG_SetupSataPciMsiEnTrap, 0x10000000 | (BusId << 20) );

  }
}

/**
 * FchKTSataControllerSetPortGenMode - Set Sata port mode (each)
 * for Gen1/Gen2/Gen3
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] PortMode   Sata Port Mode (Gen1/2/3) per port.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataControllerSetPortGenMode (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  UINT16   PortMode,
  IN  VOID     *FchDataPtr
  )
{
  UINT8        PortNumByte;
  UINT8        PortModeByte;
  UINT16       SataPortMode;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  SataPortMode = PortMode;
  PortNumByte = 0;

  while ( PortNumByte < KEITH_SATA_PORT_NUM ) {
    PortModeByte = (UINT8) (SataPortMode & 3);
    if ( (PortModeByte == BIT0) || (PortModeByte == BIT1) ) {
      if ( PortModeByte == BIT0 ) {
        //
        // set GEN 1
        //
        FchSmnRW (
          DieBusNum,
          FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80,
          0xFFFFFF0F,
          0x10,
          StdHeader
          );
      }

      if ( PortModeByte == BIT1 ) {
        //
        // set GEN2 (default is GEN3)
        //
        FchSmnRW (
          DieBusNum,
          FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80,
          0xFFFFFF0F,
          0x20,
          StdHeader
          );
      }

      FchSmnRW (
        DieBusNum,
        FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80,
        0xFFFFFFFF,
        0x01,
        StdHeader
        );
    }

    SataPortMode >>= 2;
    PortNumByte ++;
  }

  FchStall (1000, StdHeader);
  SataPortMode = PortMode;
  PortNumByte = 0;

  while ( PortNumByte < KEITH_SATA_PORT_NUM ) {
    PortModeByte = (UINT8) (SataPortMode & 3);

    if ( (PortModeByte == BIT0) || (PortModeByte == BIT1) ) {
      FchSmnRW (
        DieBusNum,
        FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG12C + PortNumByte * 0x80,
        0xFFFFFFFE,
        0x00,
        StdHeader
        );
    }

    PortNumByte ++;
    SataPortMode >>= 2;
  }
}

/**
 * FchKTSataSetPortGenMode - Set Sata port mode (each) for
 * Gen1/Gen2/Gen3
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataSetPortGenMode (
  IN  UINT32   DieBusNum,
  IN  UINT32   Controller,
  IN  VOID     *FchDataPtr
  )
{
  UINT16       SataPortMode;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  SataPortMode = (UINT16) LocalCfgPtr->Sata[0].SataPortMd;
  FchKTSataControllerSetPortGenMode (DieBusNum, Controller, SataPortMode, FchDataPtr);
}

/**
 * FchKTSataShutdownUnconnectedSataPortClock - Shutdown
 * unconnected Sata port clock
 *
 * @param[in] DieBusNum  Bus Number of current Die.
 * @param[in] Controller Sata controller number.
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchKTSataShutdownUnconnectedSataPortClock (
  IN  UINT32     DieBusNum,
  IN  UINT32     Controller,
  IN  VOID       *FchDataPtr
  )
{
  UINT8                  PortNumByte;
  UINT8                  PortSataStatusByte;
  UINT8                  NumOfPorts;
  UINT8                  FchSataClkAutoOff;
  UINT32                 PortStatusDword;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  FchSataClkAutoOff = (UINT8) LocalCfgPtr->Sata[0].SataClkAutoOff;

  NumOfPorts = 0;
  //
  // Enable SATA auto clock control by default
  //
  if ( FchSataClkAutoOff ) {
    if ((ReadFchSleepType (StdHeader) != ACPI_SLPTYP_S3)) {
      for ( PortNumByte = 0; PortNumByte < KEITH_SATA_PORT_NUM; PortNumByte++ ) {
        FchSmnRead (
          DieBusNum,
          FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG128 + PortNumByte * 0x80,
          &PortStatusDword,
          StdHeader
          );
        PortSataStatusByte = (UINT8) (PortStatusDword & 0xFF);
        //
        // Shutdown the clock for the port and do the necessary port reporting changes.
        // Error port status should be 1 not 3
        //
        if ( ((PortSataStatusByte & 0x0F) != 0x03) && (! ((LocalCfgPtr->Sata[0].SataEspPort) & (1 << PortNumByte))) ) {
          // disable port in data structure
          LocalCfgPtr->Sata[0].SataPortPower |= (1 << PortNumByte);
        }
      }                                            ///end of for (PortNumByte=0;PortNumByte<6;PortNumByte++)
    }

    // Shutdown ports
    for ( PortNumByte = 0; PortNumByte < KEITH_SATA_PORT_NUM; PortNumByte++   ) {
      if ( (LocalCfgPtr->Sata[0].SataPortPower) & (1 << PortNumByte) ) {
        FchKTSataInitPortOffline (DieBusNum, Controller, PortNumByte, FchDataPtr);
      }
    }
    // Shut port clock
    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_SLOR + 0x00,
      0xFF00FFFF,
      (LocalCfgPtr->Sata[0].SataPortPower << 16),
      StdHeader
      );

    FchSmnRW (
      DieBusNum,
      FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG0C,
      ~(UINT32) (LocalCfgPtr->Sata[0].SataPortPower),
      00,
      StdHeader
      );
  }

  FchSmnRead (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG0C, &PortStatusDword, StdHeader);
  PortSataStatusByte = (UINT8) (PortStatusDword & 0xFF);

  //
  //if all ports are in disabled state, report at least one port
  //
  if ( (PortSataStatusByte & 0xFF) == 0) {
    FchSmnRW (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG0C, 0xFFFFFF00, 0x01, StdHeader);
  }

  FchSmnRead (DieBusNum, FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG0C, &PortStatusDword, StdHeader);
  PortSataStatusByte = (UINT8) (PortStatusDword & 0xFF);

  for (PortNumByte = 0; PortNumByte < KEITH_SATA_PORT_NUM; PortNumByte ++) {
    if (PortSataStatusByte & (1 << PortNumByte)) {
      NumOfPorts++;
    }
  }

  if ( NumOfPorts == 0) {
    NumOfPorts = 0x01;
  }

  FchSmnRW (
    DieBusNum,
    FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG00,
    0xFFFFFFE0,
    (UINT32) (NumOfPorts - 1),
    StdHeader
    );
}

VOID
FchKTSataAutoShutdownController (
  IN  UINT32     DieBusNum,
  IN  UINT32     Controller,
  IN  VOID       *FchDataPtr
  )
{
  UINT8                  PortNumByte;
  UINT8                  PortSataStatusByte;
  UINT8                  PortDetectionRetry;
  UINT32                 PortStatusDword;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  if (ReadFchSleepType (StdHeader) != ACPI_SLPTYP_S3) {
    if ((LocalCfgPtr->Sata[0].SataControllerAutoShutdown) && (LocalCfgPtr->Sata[0].SataEspPort == 0)) {
      if (LocalCfgPtr->Sata[0].SataDevSlpPort0 || LocalCfgPtr->Sata[0].SataDevSlpPort1) {
        // DevSlp device need more time to exit DevSlp mode
        FchStall (150000, StdHeader);
      }

      PortDetectionRetry = 0;
      do {
        for ( PortNumByte = 0; PortNumByte < KEITH_SATA_PORT_NUM; PortNumByte++ ) {
          FchSmnRead (
            DieBusNum,
            FCH_KT_SMN_SATA_CONTROL_BAR5 + FCH_SATA_BAR5_REG128 + PortNumByte * 0x80 + \
            Controller * FCH_KT_SMN_SATA_CONTROL_STEP,
            &PortStatusDword,
            StdHeader
            );
          PortSataStatusByte = (UINT8)(PortStatusDword & 0xFF);
          if ((PortSataStatusByte & 0x0F) == 0x03) {
            return;                         // do not hide/shut Sata if detect device
          }
        }
        FchStall (10000, StdHeader);        //delay 10ms for each detection attempt
        PortDetectionRetry++;
      } while (PortDetectionRetry < SATA_PORT_DETECTION_MAX_TRY);
      FchKTSataInitDisableSata (DieBusNum, Controller, FchDataPtr);
      LocalCfgPtr->Sata[0].SataEnable = FALSE;
    }
  }
}

/**
 * sataSetIrqIntResource - Config SATA IRQ/INT# resource
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 * @param[in] StdHeader
 *
 */
VOID
SataSetIrqIntResource (
  IN  VOID                 *FchDataPtr,
  IN  AMD_CONFIG_PARAMS    *StdHeader
  )
{
  UINT8                  ValueByte;
  FCH_DATA_BLOCK         *LocalCfgPtr;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  //
  // IRQ14/IRQ15 come from IDE or SATA
  //
  ValueByte = 0x08;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &ValueByte, StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGC01, &ValueByte, StdHeader);
  ValueByte = ValueByte & 0x0F;
  ValueByte = ValueByte | 0xF0;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC01, &ValueByte, StdHeader);
}

/**
 * FchKTSataRecovery - Sata Programing for recovery path
 *
 *
 * @param[in] DieBusNum  Bus Number of current Die
 *
 */
VOID
FchKTSataRecovery (
  IN  UINT32     DieBusNum
  )
{
  //Static programming

}

