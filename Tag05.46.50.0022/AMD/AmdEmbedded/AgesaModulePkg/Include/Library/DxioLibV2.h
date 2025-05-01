/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************

*
*/

#ifndef _DXIOLIBV2_H_
#define _DXIOLIBV2_H_

#pragma pack (push, 1)

#include <Pi/PiPeiCis.h>
#include <Ppi/NbioSmuServicesPpi.h>

#define DXIO_MBOX_RETURN_NONPOSTED              0
#define DXIO_MBOX_RETURN_POSTED                 1
#define DXIO_MBOX_RETURN_POSTED_DEFRD           3

#define DXIO_MSG_INIT_INIT                      0x0
#define DXIO_MSG_INIT_SETMACPCSPRESENT          0x1
#define DXIO_MSG_INIT_SCANCAPABILITIES          0x2
#define DXIO_MSG_INIT_LOADENGINECONFIG          (0x3 + (DXIO_MBOX_RETURN_POSTED_DEFRD << 8))
#define DXIO_MSG_INIT_CANNEDCONFIG              0x4
#define DXIO_MSG_INIT_CANNEDCONFIGANDGO         0x5
#define DXIO_MSG_INIT_CANNEDCONFIG_SRAM         0x6
#define DXIO_MSG_LISM_GO                        (0x7 + (DXIO_MBOX_RETURN_POSTED_DEFRD << 8))
#define DXIO_MSG_LISM_RESUME                    (0x8 + (DXIO_MBOX_RETURN_POSTED_DEFRD << 8))
#define DXIO_MSG_LISM_COLLECTRESULTS            0x9
#define DXIO_MSG_INIT_DMAENGINECONIFGTODRAM     0x14
#define DXIO_MSG_LISM_RELOAD                    45

//HPSM Messages
#define DXIO_MSG_HPSM_UPDATENOTIFICATION        0xF        /*!< notify DXIO of a change in a GPIO for hotplug */
#define DXIO_MSG_HPSM_ENGINEINITCOMPLETE        0x15       /*!< all engine init is complete ... set runtime flag */
#define DXIO_MSG_HPSM_RESUME                    0x16       /*!< resume the current HPSM operation */
#define DXIO_MSG_HPSM_COLLECTRESULTS            0x17       /*!< collect response of the previous HPSM operation */

#define DXIO_MSG_PHYS_TARGETFREQUPDATE          0x10
#define DXIO_MSG_LOCK_SETLOCKSTATE              0x11
#define DXIO_MSG_LOCK_GETLOCKSTATE              0x12

#define DXIO_MSG_DBUG_GETVERSION                0x13
#define DXIO_MSG_DBUG_SETSCRATCH                0x22

#define DXIO_MSG_INIT_LOADHEAP                  0x23       /*!< DMA a heap section from DRAM or SRAM into the DXIO table */
#define DXIO_MSG_INIT_LOADCAPABILITIES          0x24       /*!< DMA the capabilities from DRAM or SRAM into the DXIO table */
#define DXIO_MSG_INIT_DXIOXGMI18GACOFC          0x40

#define DXIO_MSG_RNTM_PCIERETRAIN               41
#define DXIO_MSG_RNTM_PCIEDISABLE               42
#define DXIO_MSG_RNTM_DMADELI                   43
#define DXIO_MSG_RNTM_CONFIGDONE                44
#define DXIO_MSG_RNTM_ENABLEFEATURE             52
#define DXIO_MSG_RNTM_MBOXROUTINES              58
#define DXIO_MSG_RNTM_PCIELINKMANAGEMENT        70
#define DXIO_MSG_RNTM_PCIELINKSPEEDLIMIT        71
#define DXIO_MSG_RNTM_PCIELINKTRAINING          72
#define DXIO_MSG_RNTM_ACDC_SWITCH               73
#define DXIO_MSG_RNTM_CHANGESPEEDPERPSPPPOLICY  74
#define DXIO_MSG_RNTM_INITESM                   83

/// Heap Type defines for DXIO_MSG_INIT_LOADHEAP
#define DXIO_HEAPTYPE_EMPTY                     0
#define DXIO_HEAPTYPE_FABINIT                   1
#define DXIO_HEAPTYPE_MACPCS                    2
#define DXIO_HEAPTYPE_CFGPTR                    3
#define DXIO_HEAPTYPE_CAPPTR                    4
#define DXIO_HEAPTYPE_GPIOPTR                   5
#define DXIO_HEAPTYPE_ANCDATA                   6

#define DXIO_MBOX_RETVAL_NULL                   0
#define DXIO_MBOX_RETVAL_TIMEOUT                1
#define DXIO_MBOX_RETVAL_ERROR                  2
#define DXIO_MBOX_RETVAL_OK                     3
#define DXIO_MBOX_RETVAL_UNKNOWN_LOCK           4
#define DXIO_MBOX_RETVAL_TRYAGAIN               5
#define DXIO_MBOX_RETVAL_MBOX_IDLE              6
#define DXIO_MBOX_RETVAL_MBOX_BUSY              7
#define DXIO_MBOX_RETVAL_MBOX_DONE              8

#define DXIO_MBOX_RETPARAMS_NONE                0
#define DXIO_MBOX_RETPARAMS_GENERIC             1
#define DXIO_MBOX_RETPARAMS_LISM                2
#define DXIO_MBOX_RETPARAMS_HPSM                3
#define DXIO_MBOX_RETPARAMS_REQ_RESET           4

#define DXIO_ARG_RNTM_ENABLEFEATURE_XGMIADAPTATION     00

#define DXIO_LinkInitState_init                 0
#define DXIO_LinkInitState_disabled             1
#define DXIO_LinkInitState_scanned              2
#define DXIO_LinkInitState_canned               3
#define DXIO_LinkInitState_loaded               4
#define DXIO_LinkInitState_configured           5
#define DXIO_LinkInitState_earlyTraining        6
#define DXIO_LinkInitState_earlyTrained         7
#define DXIO_LinkInitState_vetting              8
#define DXIO_LinkInitState_getvet               9
#define DXIO_LinkInitState_noVetting            10
#define DXIO_LinkInitState_gpiosInit            11
#define DXIO_LinkInitState_NHPTraining          12
#define DXIO_LinkInitState_done                 13
#define DXIO_LinkInitState_error                14
#define DXIO_LinkInitState_mapped               15

#define DXIO_EngineTrainingState_IdleUnconn     0
#define DXIO_EngineTrainingState_IdleConn       1
#define DXIO_EngineTrainingState_ResetRequested 2
#define DXIO_EngineTrainingState_Training       3
#define DXIO_EngineTrainingState_ActiveConn     4
#define DXIO_EngineTrainingState_ActiveUnconn   5
#define DXIO_EngineTrainingState_IdlePhyReset   7
#define DXIO_EngineTrainingState_Error          8
#define DXIO_EngineTrainingState_UNKNOWN        9

#define DXIO_freqUpdType_restoreOnly            0 /*!< use previously saved values and do not apply per-lane resets. */
#define DXIO_freqUpdType_all                    1 /*!< update all engines of the given type */
#define DXIO_freqUpdType_single                 2 /*!< update single engine of the given type */


#define DXIO_LinkStateResetAssert           0        ///< Assert port GPIO reset
#define DXIO_LinkStateResetDuration         1        ///< Timeout for reset duration
#define DXIO_LinkStateResetExit             2        ///< Deassert port GPIO reset
#define DXIO_LinkTrainingResetEnter         3        ///< Entry state before Port GPIO reset timeout
#define DXIO_LinkTrainingResetTimeout       4        ///< Port GPIO reset timeout
#define DXIO_LinkStateReleaseTraining       5        ///< Release link training
#define DXIO_LinkStateDetectPresence        6        ///< Detect device presence
#define DXIO_LinkStateDetecting             7        ///< Detect link training.
#define DXIO_LinkStateBrokenLane            8        ///< Check and handle broken lane
#define DXIO_LinkStateGen2Fail              9        ///< Check and handle device that fail training if GEN2 capability advertised
#define DXIO_LinkStateL0                   10        ///< Device trained to L0
#define DXIO_LinkStateVcoNegotiation       11        ///< Check VCO negotiation complete
#define DXIO_LinkStateRetrain              12        ///< Force retrain link.
#define DXIO_LinkStateTrainingFail         13        ///< Link training fail
#define DXIO_LinkStateTrainingSuccess      14        ///< Link training success
#define DXIO_LinkStateGfxWorkaround        15        ///< GFX workaround
#define DXIO_LinkStateCompliance           16        ///< Link in compliance mode
#define DXIO_LinkStateDeviceNotPresent     17        ///< Link is not connected
#define DXIO_LinkStateTrainingCompleted    18        ///< Link training completed


#define DBUG_SETSCRATCHFLAG_SKIPPSPMESSAGES             13
#define DBUG_SETSCRATCHFLAG_RETURNAFTERMAPPING          14
#define DBUG_SETSCRATCHFLAG_RETURNAFTERCONFIG           15
#define DBUG_SETSCRATCHFLAG_V1ANCILLARYDATA             16
#define DBUG_SETSCRATCHFLAG_NTBHOTPLUGENABLED           17

#define DBUG_SETSCRATCHFLAG_SAVERESTOREMODE             19
#define DBUG_SETSCRATCHFLAG_MAP_TO_WHICH_CONTROLLER     34

#define DXIO_E_MBOXMSG_DBUG_SETSCRATCH                                      34

#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_INDEX_REG_END                    12
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_SKIPPSPMESSAGES                  13
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_LISM_RETURNAFTERMAPPING          14
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_LISM_RETURNAFTERCONFIG           15
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_V1ANCILLARYDATA                  16
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_NTBHOTPLUGENABLED                17
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_MATCHPCIEPORTSIZEEXACTLY         18
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_SAVERESTOREMODE                  19
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_USEPHYSRAMFLAG                   20
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_DISABLECCA                       22
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_REC_CNTL                         23
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_SKIP_RX_DFE_ADAPT                24
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_EXPERIMENT_NUMBER                25
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_CLK_GATING        26
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_STATIC_PWR_GATING 27
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_REFCLK_SHUTDOWN   28
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_GPIO26_GENERIC_RST               29
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_GPIO40_NVME_RST                  30
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_VALIDPHYFWFLAG                   31
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_CBSOPTIONS_ENABLEPOWERMANAGEMENT 32
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_PMA_POWER_GATING  33
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PWRMNGMT_PRFRM_PMA_CLOCK_GATING  34
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_PCIE_ALLOW_COMPLETION_PASS       35
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_CBSOPTIONS_ALLOWPOINTERSLIPINTERVAL  36
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_UNKNOWN                      37
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_GEN3_INIT_RX_SETTING             38
#define DXIO_E_MBOXMSG_DBUG_SETSCRATCHFLAG_GEN4_INIT_RX_SETTING             39
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_GPIO_EP2_RESET               40
// RMB+
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_GPIO_GENERIC_RESET           29
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_GPIO_EP1_RESET               30
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_SATA_Gen1_Settings           41
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_SATA_Gen2_Settings           42
#define DXIO_E_MBOXMESSAGE_DBUG_SETSCRATCHFLAG_SATA_Gen3_Settings           43

///DXIO Return Parameters
typedef struct {
  UINT32  MP1Mbox_RetVal;                ///< Return value
  UINT32  RetParams_Type:8;              ///< Type
  UINT32  RetParams_NumParams:8;         ///< NumParams
  UINT32  RetParams_Reserved:16;         ///< Reserved
  UINT32  RetParams_Param0;              ///< Param0
  UINT32  RetParams_Param1;              ///< Param1
  UINT32  RetParams_Param2;              ///< Param2
  UINT32  RetParams_Param3;              ///< Param3
} DXIO_RETURN_PARAMS;

///DXIO LISM Return Paramaters
typedef struct {
  UINT32  CurrentState;                  ///< Current State
  UINT32  RetParams_Param1;              ///< Param1
  UINT32  RetParams_Param2;              ///< Param2
  UINT32  RetParams_Param3;              ///< Param3
} LISM_RETURN_PARAMS;

///DXIO Reset Request Return Paramaters
typedef struct {
  UINT32  resetAssertDeassert;           ///< 0=Assert, 1=De-Assert
  UINT32  GpioId;                        ///< GpioId
  UINT32  RetParams_Param2;              ///< Param2
  UINT32  RetParams_Param3;              ///< Param3
} RESET_REQUEST_PARAMS;

///DXIO Link Capabilities
typedef struct {
  UINT32            portPresent: 1;            ///< portPresent
  UINT32         earlyTrainLink: 1;            ///< earlyTrainLink
  UINT32     linkComplianceMode: 1;            ///< linkComplianceMode
  UINT32           linkReversal: 1;            ///< linkReversal
  UINT32        maxLinkSpeedCap: 3;            ///< maxLinkSpeedCap
  UINT32         endpointStatus: 1;            ///< endpointStatus
  UINT32            linkHotplug: 3;            ///< linkHotplug
  UINT32               portSize: 5;            ///< portSize
  UINT32    maxLinkSpeedTrained: 3;            ///< maxLinkSpeedTrained
  UINT32  enableTurnOffSettings: 1;            ///< enableTurnOffSettings
  UINT32     turnOffUnusedLanes: 1;            ///< enableTurnOffSettings
  UINT32             ntbHotPlug: 1;            ///< NTB Hotplug Support
  UINT32           CurrentSpeed: 2;            ///< PSPP Current Speed
  UINT32               PSPPMode: 3;            ///< PSPP Mode
  UINT32      partnerDeviceType: 2;            ///< Partner Device Type
  UINT32   autoSpeedChangeCntrl: 2;            ///< Auto Speed Change Control
  UINT32            isMasterPLL: 1;            ///< isMasterPLL
  UINT32           eqSearchMode: 2;            ///< eqSearchMode
  UINT32      eqSearchModeOride: 1;            ///< eqSearchModeOride
  UINT32         invertPolarity: 1;            ///< invertPolarityRx
  UINT32              txVetting: 1;            ///< txVetting
  UINT32              rxVetting: 1;            ///< rxVetting
  UINT32           TXDeEmphasis: 2;            ///< TXDeEmphasis value
  UINT32      TXDeEmphasisOride: 1;            ///< TXDeEmphasisOride
  UINT32        invertPolarity2: 1;            ///< invertPolarityTx
  UINT32        targetLinkSpeed: 3;            ///< targetLinkSpeed
  UINT32           bypassGen3EQ :1;            ///< bypassGen3EQ
  UINT32           bypassGen4EQ :1;            ///< bypassGen4EQ
  UINT32          capabilities1:17;            ///< capabilities1
} DXIO_LINK_CAP;

///DXIO Link Descriptor Base
typedef struct {
  UINT32          ChannelType: 8;              ///< ChannelType
  UINT32        ChannelDescID: 8;              ///< ChannelDescID
  UINT32  AncillaryDataOffset:16;              ///< Reserved3
  UINT32         DevFunNumber:32;              ///< DevFunNumber
  DXIO_LINK_CAP     Capabilities;              ///< Capabilities
  UINT32                MacID: 8;              ///< MacID
  UINT32            MacPortID: 8;              ///< MacPortID
  UINT32        StartCoreLane: 8;              ///< StartCoreLane
  UINT32          EndCoreLane: 8;              ///< EndCoreLane
  UINT32                PcsID: 8;              ///< PcsID
  UINT32           Reserved1: 24;              ///< Reserved1
} DXIO_LINK_DESC_BASE;

///DXIO Link Descriptor Eth
typedef struct {
  UINT32          ChannelType: 8;              ///< ChannelType
  UINT32            Reserved1: 8;              ///< Reserved1
  UINT32  AncillaryDataOffset:16;              ///< AncillaryDataOffset
  UINT32         DevFunNumber:32;              ///< DevFunNumber
  DXIO_LINK_CAP     Capabilities;              ///< Capabilities
  UINT32            Reserved2:32;              ///< Reserved2
  UINT32            Reserved3:32;              ///< Reserved3
} DXIO_LINK_DESC_ETH;

///DXIO Link Descriptor PCIe
typedef struct {
  UINT32          ChannelType: 8;              ///< ChannelType
  UINT32        ChannelDescID: 8;              ///< ChannelDescID
  UINT32  AncillaryDataOffset:16;              ///< Reserved3
  UINT32         DevFunNumber:32;              ///< DevFunNumber
  DXIO_LINK_CAP     Capabilities;              ///< Capabilities
  UINT32                MacID: 8;              ///< MacID
  UINT32            MacPortID: 8;              ///< MacPortID
  UINT32        StartCoreLane: 8;              ///< StartCoreLane
  UINT32          EndCoreLane: 8;              ///< EndCoreLane
  UINT32                PcsID: 8;              ///< PcsID
  UINT32    linkTrainingState: 8;              ///< linkTrainingState
  UINT32           Reserved1: 16;              ///< Reserved1
} DXIO_LINK_DESC_PCIE;

typedef DXIO_LINK_DESC_BASE   DXIO_LINK_DESC_SATA;
typedef DXIO_LINK_DESC_BASE   DXIO_LINK_DESC_USB;
typedef DXIO_LINK_DESC_BASE   DXIO_LINK_DESC_DP;
typedef DXIO_LINK_DESC_BASE   DXIO_LINK_DESC_GOP;
typedef DXIO_LINK_DESC_BASE   DXIO_LINK_DESC_GMI;

///DXIO Link Descriptor
typedef union {
  DXIO_LINK_DESC_BASE   Base;              ///< DXIO Link Descriptor Base
  DXIO_LINK_DESC_PCIE   Pcie;              ///< DXIO Link Descriptor PCIe
  DXIO_LINK_DESC_SATA   Sata;              ///< DXIO Link Descriptor Sata
  DXIO_LINK_DESC_USB    Usb;               ///< DXIO Link Descriptor Usb
  DXIO_LINK_DESC_DP     Dp;                ///< DXIO Link Descriptor Dp
  DXIO_LINK_DESC_ETH    Eth;               ///< DXIO Link Descriptor Eth
  DXIO_LINK_DESC_GOP    Gop;               ///< DXIO Link Descriptor Gop
  DXIO_LINK_DESC_GMI    Gmi;               ///< DXIO Link Descriptor Gmi
} DXIO_LINK_DESC;

///DXIO Engine Descriptor
typedef struct {
  UINT32           EngineType: 8;              ///< EngineType
  UINT32         HotPluggable: 1;              ///< HotPluggable
  UINT32            Reserved3: 7;              ///< Reserved3
  UINT32            StartLane: 8;              ///< StartLane
  UINT32              EndLane: 8;              ///< EndLane
  UINT32          GpioGroupID: 8;              ///< GpioGroupID
  UINT32        engineResetID: 8;              ///< engineResetID
  UINT32         searchDepth : 1;              ///< searchDepth
  UINT32      forceKpnpReset : 1;              ///< forceKpnpReset
  UINT32            Reserved2:14;              ///< Reserved2
  DXIO_LINK_DESC        LinkDesc;              ///< LinkDesc
  UINT32         MacEnginePtr:16;              ///< MacEnginePtr
  UINT32          FirstLgdNum: 8;              ///< FirstLgdNum
  UINT32           LastLgdNum: 8;              ///< LastLgdNum
  UINT32        TrainingState: 4;              ///< TrainingState
  UINT32            Reserved1:28;              ///< Reserved1
} DXIO_ENGINE_DESCRIPTOR;

///DXIO Platform Descriptor
typedef struct {
  UINT32          PlatformType:16;              ///< PlatformType
  UINT32             Reserved4:16;              ///< Reserved4
  UINT32             Reserved3:32;              ///< Reserved3
  UINT32             Reserved2:32;              ///< Reserved2
  UINT32         NumEngineDesc:16;              ///< NumEngineDesc
  UINT32             Reserved1:16;              ///< Reserved1
} DXIO_PLATFORM_DESCRIPTOR;

//------------------------------------------------------------------------------
// SpeedSet Overrides
//------------------------------------------------------------------------------

///DXIO Speed Set Override Types
typedef enum {
  DXIO_OVRSSET_FLD_txX_eq_pre          ,
  DXIO_OVRSSET_FLD_txX_eq_main         ,
  DXIO_OVRSSET_FLD_txX_eq_post         ,
  DXIO_OVRSSET_FLD_txX_iboost_lvl      ,
  DXIO_OVRSSET_FLD_txX_vboost_en       ,
  DXIO_OVRSSET_FLD_rxX_adapt_afe_en    ,
  DXIO_OVRSSET_FLD_rxX_adapt_dfe_en    ,
  DXIO_OVRSSET_FLD_rxX_eq_att_lvl      ,
  DXIO_OVRSSET_FLD_rxX_eq_vga1_gain    ,
  DXIO_OVRSSET_FLD_rxX_eq_vga2_gain    ,
  DXIO_OVRSSET_FLD_rxX_eq_ctle_pole    ,
  DXIO_OVRSSET_FLD_rxX_eq_ctle_boost   ,
  DXIO_OVRSSET_FLD_rxX_eq_dfe_tap1     ,
  DXIO_OVRSSET_FLD_rxX_los_threshold   ,
  DXIO_OVRSSET_FLD_rx_vref_ctrl        ,
  DXIO_OVRSSET_FLD_tx_vboost_lvl       ,
  DXIO_OVRSSET_FLD_SSC_OFF_FRUG1       ,
  DXIO_OVRSSET_FLD_SSC_OFF_PHUG1       ,
  DXIO_OVRSSET_FLD_SSC_ON_FRUG1        ,
  DXIO_OVRSSET_FLD_SSC_ON_PHUG1        ,
  DXIO_OVRSSET_FLD_txX_pre_deemphasis  ,
  DXIO_OVRSSET_FLD_txX_post_deemphasis ,
  DXIO_OVRSSET_FLD_NUM_OVERRIDE_FIELDS
} ENUM_DXIO_OVRSSET_FIELDS;

/// structures for speedSet overrides
typedef struct {
  UINT32         overrideField    : 7  ; // which field is to be over-ridden
  UINT32         fieldGen         : 3  ; // protocol generation i.e. Pcie Gen1/2/3
  UINT32         laneNumber       : 6  ; // lane for over-ride
  UINT32         value            : 16 ; // new value for over-ride
} DXIO_OVRSSETV2_ENTRY;


/// structures for speedSet overrides
typedef struct {
  UINT32         overrideField    : 8  ; // which field is to be over-ridden
  UINT32         fieldGen         : 2  ; // protocol generation i.e. Pcie Gen1/2/3
  UINT32         laneNumber       : 6  ; // lane for over-ride
  UINT32         value            : 16 ; // new value for over-ride
} DXIO_OVRSSETV1_ENTRY;


///------------------------------------------------------------------------------
/// DXIO ANCILLARY DATA SUBHEAP
///------------------------------------------------------------------------------

#define DXIO_ANCILLARYDATA_SUBHEAP_VERSION        1
#define DXIO_ANCILLARYDATA_REL_SUBHEAP_VERSION    2

///DXIO Ancillary Data Subheap Types
typedef enum {
  DXIO_ANCDATA_SUBHEAPTYPE_XGBE = 1,
  DXIO_ANCDATA_SUBHEAPTYPE_HIER = 2,
  DXIO_ANCDATA_SUBHEAPTYPE_OVRRSET = 3,
  DXIO_ANCDATA_SUBHEAPTYPE_PSPP = 4,
  DXIO_ANCDATA_SUBHEAPTYPE_PHYCONFIG = 5,
  DXIO_ANCDATA_SUBHEAPTYPE_PHYVALUE = 6,
  DXIO_ANCDATA_SUBHEAPTYPE_PCIESTRAP = 7
} DXIO_ANCDATA_SUBHEAPTYPE;

///DXIO Ancillary Data Subheap Type Structure
typedef struct {
  UINT32          descriptorType  : 8 ;
  UINT32          version         : 4 ;
  UINT32          reserved1       : 4 ;
  UINT32          numDWORDs       : 8 ;
  UINT32          reserved2       : 8 ;
} DXIO_ANCILLARYDATA_SUBHEAP;

//------------------------------------------------------------------------------
// DXIO ANCILLARY DATA OVERALL STRUCTURE
//------------------------------------------------------------------------------

#define DXIO_ANCILLARYDATA_VERSION    0

typedef struct {
  UINT32          descriptorType : 8 ;
  UINT32          version        : 4 ;
  UINT32          reserved1      : 4 ;
  UINT32          numDWords      : 8 ;
  UINT32          reserved2      : 8 ;
} DXIO_ANCILLARYDATA;

typedef struct {
  UINT32          paramId : 8 ;
  UINT32          columnId: 8 ;
  UINT32          value   : 8 ;
  UINT32          laneNum : 8 ;
} DXIO_ANCILLARYDATA_PHYVALUE;

//------------------------------------------------------------------------------
// DXIO HEAP DATA CONTAINER
//------------------------------------------------------------------------------

#define DXIO_HEAPDATA_CONTAINER_VERSION    0

typedef struct {
  UINT32          heapType      : 8 ;
  UINT32          version       : 4 ;
  UINT32          reserved      : 4 ;
  UINT32          numDWords     : 16;
} DXIO_HEAPDATA_CONTAINER;

//------------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//------------------------------------------------------------------------------

AGESA_STATUS
DxioEarlyInitV2 (
  IN      CONST EFI_PEI_SERVICES          **PeiServices,
  IN      PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN      PCIe_PLATFORM_CONFIG            *Pcie,
  IN      GNB_HANDLE                      *StartHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR         *UserConfig
  );

VOID
DxioSetLinkSpeedV2 (
  IN      CONST EFI_PEI_SERVICES          **PeiServices,
  IN      PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN      UINT32                          InstanceId,
  IN      UINT32                          EngineId,
  IN      UINT32                          TargetSpeed
  );

UINT8
DxioLibServiceRequest (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI   *SmuServicesPpi,
  IN       UINT32                          InstanceId,
  IN       UINT32                          ServiceRequest,
  IN       UINT32                          DxioArg1,
  IN       UINT32                          DxioArg2,
  IN       UINT32                          DxioArg3,
  IN       UINT32                          DxioArg4,
  IN       UINT32                          DxioArg5
  );


///------------------------------------------------------------------------------
/// DXIO ANCILLARY DATA FOR STRAP SETTINGS
///------------------------------------------------------------------------------


typedef struct
{
  UINT32          numDwords           : 16;
  UINT32          ctrllerId           : 5;
  UINT32          reserved            : 9;
}
DXIO_STRAPSUBHEAP;

typedef enum{
    shortCtrllerStrap = 0,
    shortPerPortStrap = 1,
    shortAllPortStrap = 2,
    longCtrllerStrap  = 3,
    longPerPortStrap  = 4,
    longAllPortStrap  = 5,
    extStrap          = 6,
} DXIO_STRAPENTRYTYPE;

typedef struct
{
  UINT32       strapType: 3;
  UINT32       strapIdx : 10;
  UINT32       data     : 19;
} DXIO_STRAPENTRY_SHORT_CTRLLER;

typedef struct
{
  UINT32       strapType: 3;
  UINT32       engineId : 8;
  UINT32       strapIdx : 10;
  UINT32       data     : 11;
} DXIO_STRAPENTRY_SHORT_PERPORT;

typedef struct
{
  UINT32       strapType: 3;
  UINT32       strapIdx : 10;
  UINT32       data     : 19;
} DXIO_STRAPENTRY_SHORT_ALLPORT;

typedef struct
{
  UINT32       strapType: 3;
  UINT32       strapIdx : 10;
  UINT32       reserved : 19;
  UINT32       data     : 32;
} DXIO_STRAPENTRY_LONG_CTRLLER;

typedef struct
{
  UINT32       strapType: 3;
  UINT32       engineId : 8;
  UINT32       strapIdx : 10;
  UINT32       reserved : 11;
  UINT32       data     : 32;
} DXIO_STRAPENTRY_LONG_PERPORT;

typedef struct
{
  UINT32       strapType: 3;
  UINT32       strapIdx : 10;
  UINT32       reserved : 19;
  UINT32       data     : 32;
} DXIO_STRAPENTRY_LONG_ALLPORT;

typedef struct _S0i3GPIORst
{
  UINT32       AssertionSeq; //0:Disable, 1:DeAssert (Compatible with legacy ), 2:First Assert then Deassert
  UINT32       Address;
  UINT32       DeassertionValue;
  UINT32       AssertionValue;
} S0I3_GPIO_RST_EP;

#pragma pack (pop)

#endif


