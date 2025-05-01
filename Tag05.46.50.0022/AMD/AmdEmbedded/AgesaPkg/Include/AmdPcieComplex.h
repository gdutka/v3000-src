/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Various PCI service routines.
 *
 *
  * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 *
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef _AMD_PCIE_COMPLEX_H_
#define _AMD_PCIE_COMPLEX_H_
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
/**
 * @brief GNB PCIe configuration info
 * @details
 */
#define DESCRIPTOR_TERMINATE_LIST           0x80000000ull
#define DESCRIPTOR_IGNORE                   0x40000000ull

#define PCIE_PORT_PARAMETER_COUNT  64

/**
 * @brief PCIe link initialization
 * @details
 */
typedef enum {
  DxioEndpointDetect = 0,                                    ///< Detect endpoint presence
  DxioEndpointNotPresent                                     ///< Endpoint not present (or connected). Used in case there is alternative way to determine
                                                             ///< if device present on board or in slot. For example GPIO can be used to determine device presence.
} DXIO_ENDPOINT_STATUS;

/**
 * @brief DXIO ANCILLARY DATA FOR PSPP
 * @details
 */
#define PsppThreshold0Percent     0
#define PsppThreshold5Percent     1
#define PsppThreshold10Percent    2
#define PsppThreshold15Percent    3
#define PsppThreshold20Percent    4
#define PsppThreshold25Percent    5
#define PsppThreshold30Percent    6
#define PsppThreshold40Percent    7
#define PsppThreshold50Percent    8
#define PsppThreshold60Percent    9
#define PsppThreshold70Percent    10
#define PsppThreshold75Percent    11
#define PsppThreshold80Percent    12
#define PsppThreshold90Percent    13
#define PsppThreshold95Percent    14
#define PsppThreshold100Percent   15

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef union {
  struct {
    UINT32         lowBWThreshold    : 4 ; ///< Low Bandwidth Threshold
    UINT32         HighBWThreshold   : 4 ; ///< High Bandwidth Threshold
    UINT32         lowBWThresholdG2  : 4 ; ///< Low Bandwidth Threshold
    UINT32         HighBWThresholdG2 : 4 ; ///< High Bandwidth Threshold
    UINT32         lowBWThresholdG3  : 4 ; ///< Low Bandwidth Threshold
    UINT32         HighBWThresholdG3 : 4 ; ///< High Bandwidth Threshold
    UINT32         reserved          : 5 ; ///< Reserved for future purpose
    UINT32         BWHintCount       : 3 ; ///< Bandwidth Count
  } Field;                                                              ///<  Union with threshold values
  UINT32 Value;                                                         ///<  Value
} DXIO_ANCILLARYPORTDATA_PSPP;


typedef union {
  struct {
    UINT32         lowBWThresholdG4  : 4 ; ///< Low Bandwidth Threshold
    UINT32         HighBWThresholdG4 : 4 ; ///< High Bandwidth Threshold
    UINT32         lowBWThresholdG5  : 4 ; ///< Low Bandwidth Threshold
    UINT32         HighBWThresholdG5 : 4 ; ///< High Bandwidth Threshold
    UINT32         Reserved1         : 16; ///< Reservered for future purpose
 } Field;                                  ///< Union with threshold values
  UINT32 Value;                            ///< Value
} DXIO_ANCILLARYPORTDATA_PSPP_2;

/**
 * @brief PHY Programming Data
 * @details
 */
typedef struct {
  IN       UINT16               ParamType;                ///< Param type
  IN       UINT16               ParamValue;               ///< Param value
} PORT_PARAM;

typedef struct {
  IN       PORT_PARAM           PhyParam [PCIE_PORT_PARAMETER_COUNT];       ///< port parameter
} PORT_PARAMS;

#define PORT_PARAMS_START  ,{{
#define PORT_PARAMS_END    }}
#define PORT_PARAM(x,y)    {x, y}
#define NO_PORT_PARAMS_DATA      PORT_PARAMS_START PORT_PARAM(0,0) PORT_PARAMS_END

/**
 *  @brief Port parameter override enumerator
 *
 *  @details This enumerator provides a parmeter type for platform topology override values.
 */
typedef enum {
  PP_DEVICE = 1             ,  /**< Request a specific device number for the root port
                                *  This should be a valid PCI device number within the default
                                *  range of the PCIe root ports for this program, typically 1-4.
                                */
  PP_FUNCTION               ,  /**< Request a specific function number for the root port
                                *  This should be a valid PCI device number within the default
                                *  range of the PCIe root ports for this program, typically 1-7.
                                */
  PP_PORT_PRESENT           ,  ///< Define whether training should be attempted.
  PP_LINK_SPEED_CAP         ,  /**< Limit the maximum link speed supported
                                *  @li @b 0 - Maximum supported by silicon
                                *  @li @b 1 - Gen1
                                *  @li @b 2 - Gen2
                                *  @li @b 3 - Gen3
                                *  @li @b 4 - Gen4
                                *  @li @b 5 - Gen5
                                */
  PP_LINK_ASPM              ,  /**< Define the ASPM capability.
                                *  @li @b 0 - DxioAspmDisabled - Disabled
                                *  @li @b 1 - DxioAspmL0s - PCIe L0s link state
                                *  @li @b 2 - DxioAspmL1 - PCIe L1 link state
                                *  @li @b 3 - DxioAspmL0sL1 - PCIe L0s & L1 link state
                                */
  PP_HOTPLUG_TYPE           ,  /**< Assign a hotplug type.
                                *   NOTE: UBM hotplug type is identified by auto-discovery
                                *  @li @b 0 - DxioHotplugDisabled,
                                *  @li @b 1 - DxioHotplugBasic,
                                *  @li @b 2 - DxioHotplugServerExpres
                                *  @li @b 3 - DxioHotplugEnhanced,
                                *  @li @b 4 - DxioHotplugInboard,
                                *  @li @b 5 - DxioHotplugServerEntSSD
                                */
  PP_CLKREQ                 ,  /**< Specify a clock request line.
                                *  @li DxioClkReqNone - Default None
                                *  @li DxioClkReq0 - CLKREQ0 signal
                                *  @li DxioClkReq1 - CLKREQ1 signal
                                *  @li DxioClkReq2 - CLKREQ2 signal
                                *  @li DxioClkReq3 - CLKREQ3 signal
                                *  @li DxioClkReqG - CLKREQG/CLKREQ4 signal
                                *  @li DxioClkReq5 - CLKREQ5 signal
                                *  @li DxioClkReq6 - CLKREQ6 signal
                                *  @li DxioClkReqInvalid = 0x8,  Reserved
                                *  @li DxioGpp0ClkReq - gpp0_clkreq signal - MISC2
                                *  @li DxioGpp1ClkReq - gpp1_clkreq signal - MISC2
                                *  @li DxioGpp2ClkReq - gpp2_clkreq signal - MISC2
                                *  @li DxioGpp3ClkReq - gpp3_clkreq signal - MISC2
                                *  @li DxioGpp4ClkReq - gpp4_clkreq signal - MISC2
                                */
  PP_ASPM_L1_1              ,  ///< (__BOOLEAN__) Define the L1.1 substate capability.
  PP_ASPM_L1_2              ,  ///< (__BOOLEAN__) Define the L1.2 substate capability.
  PP_COMPLIANCE             ,  ///< (__BOOLEAN__) Request compliance mode.
  PP_SAFE_MODE              ,  ///< (__BOOLEAN__) Request safe mode.
  PP_CHIPSET_LINK           ,  ///< (__BOOLEAN__) Identify this as the link connected to the chipset.
  PP_CLOCK_PM               ,  ///< (__BOOLEAN__) Enable clock power management.
  PP_CHANNELTYPE            ,  /**< Specify a SATA channel type.
                                *  @li __SATA ONLY__
                                */
  PP_TURN_OFF_UNUSED_LANES  ,  ///< (__BOOLEAN__) Enable powerdown of unused lanes.
  PP_APIC_GROUPMAP          ,  ///< Override the IOAPIC group map assignment.
  PP_APIC_SWIZZLE           ,  ///< Override the IOAPIC swizzling.
  PP_APIC_BRIDGEINT         ,  ///< Override the IOAPIC bridge interrupt.
  PP_MASTER_PLL             ,  ///< (__BOOLEAN__) Associate this port as the master PLL.
  PP_SLOT_NUM               ,  ///< Specify a SLOT NUMBER value.
  PP_PHY_PARAM              ,  ///< Specifies a PHY sub-parameter. __SEE DXIO_PHY_PARAM_TYPE__
  PP_ESM                    ,  ///< Enable support for Enhanced Speed Mode.
  PP_CCIX                   ,  /**< Enable the port as S-Link capable.
                                *  @li __Deprecated for Genoa__ Replaced by CXL suppport
                                */
  PP_CXL                    ,  ///< Enable the port as CXL capable.
  PP_GEN3_DS_TX_PRESET      ,  ///< Gen3 Downstream Tx Preset. __4 bit value__
  PP_GEN3_DS_RX_PRESET_HINT ,  ///< Gen3 Downstream Rx Preset Hint. __3 bit value__
  PP_GEN3_US_TX_PRESET      ,  ///< (__BOOLEAN__) Gen3 Upstream Tx Preset. __4 bit value__
  PP_GEN3_US_RX_PRESET_HINT ,  ///< Gen3 Upstream Rx Preset Hint. __3 bit value__
  PP_GEN4_DS_TX_PRESET      ,  ///< Gen4 Downstream Tx Preset. __4 bit value__
  PP_GEN4_US_TX_PRESET      ,  ///< Gen4 Upstream Tx Preset. __4 bit value__
  PP_GEN3_FIXED_PRESET      ,  ///< (__BOOLEAN__) Gen3 Fixed Preset.
  PP_GEN4_FIXED_PRESET      ,  ///< (__BOOLEAN__) Gen4 Fixed Preset.
  PP_PSPP_DC                ,  /**< Pspp DC control.
                                *   @li __Applicable for client programs__
                                */
  PP_PSPP_AC                ,  /**< PSPP AC control.
                                *   @li __Applicable for client programs__
                                */
  PP_GEN2_DEEMPHASIS        ,  /**< Specify the deemphasis value for Gen2.
                                *  @li @b TX_DEEMPHASIS_CSR = 0 - Use de-emphasis from CSR
                                *  @li @b TX_DEEMPHASIS_US = 0x1 - Use de-emphasis requested by upstream component
                                *  @li @b TX_DEEMPHASIS_60 = 0x2 - Use -6.0 dB
                                *  @li @b TX_DEEMPHASIS_35 = 0x3 - Use -3.5 dB
                                *  @li @b TX_DEEMPHASIS_DIS = 0xFF - Skip override setting
                                */
  PP_INVERT_POLARITY        ,  ///< (__BOOLEAN__) Request inverted polarity.
  PP_TARGET_LINK_SPEED      ,  /**< Set the desired operating link speed.
                                *  @li @b 0 - Maximum supported by silicon
                                *  @li @b 1 - Gen1
                                *  @li @b 2 - Gen2
                                *  @li @b 3 - Gen3
                                *  @li @b 4 - Gen4
                                *  @li @b 5 - Gen5
                                */
  PP_GEN4_DLF_CAP_DISABLE   ,  ///< (__BOOLEAN__) Disable GEN4 DLF Capability.
  PP_GEN4_DLF_EXCHG_DISABLE ,  ///< (__BOOLEAN__) Disable GEN4 DLF Exchange.
  PP_I2C_EXPANDER_ADDRESS   ,  /**< (UINT8) I2C Expander Address for server features
                                */
  PP_I2C_EXPANDER_TYPE      ,  /**< (UINT8) I2C Expander Type for server features
                                * @li __New for Genoa__
                                */
  PP_UBM_SWITCH0_ADDR       ,  /**< (UINT8) I2C Address of UBM Switch __(required)__
                                * @li __New for Genoa__
                                */
  PP_UBM_SWITCH0_SELECT     ,  /**< (UINT8) I2C Select of UBM Switch __(required)__
                                *   Valid values are 0-3
                                * @li __New for Genoa__
                                */
  PP_UBM_SWITCH0_TYPE      ,  /**< (UINT8) I2C Type of UBM Switch __(required)__
                                *   Valid values are 0-3
                                * @li __New for Genoa__
                                */
  PP_UBM_SWITCH1_ADDR       ,  /**< (UINT8) I2C Address of second UBM Switch __(optional)__
                                * @li __New for Genoa__
                                */
  PP_UBM_SWITCH1_SELECT     ,  /**< (UINT8) I2C Select of second UBM Switch __(optional)__
                                *   Valid values are 0-3
                                * @li __New for Genoa__
                                */
  PP_UBM_SWITCH1_TYPE      ,  /**< (UINT8) I2C Type of UBM Switch __(required)__
                                *   Valid values are 0-3
                                * @li __New for Genoa__
                                */
  PP_UBM_HFC_INDEX          ,  /**< (UINT8) HFC Index for this port
                                *   Valid values are 0-0x31
                                * @li __New for Genoa__
                                */
  PP_UBM_DFC_INDEX          ,  /**< (UINT8) DFC Index for this port
                                *   Valid values are 0-0xFF
                                * @li __New for Genoa__
                                */
  PP_GPIOx_I2C_RESET        ,  /**< (UINT8) Pin number on I2C Expander of UBM reset
                                * @li __New for Genoa__
                                */
  PP_GPIOx_BP_TYPE          ,  /**< (UINT8) Pin number of U.3 IFDET2 pin on I2C Expander
                                * @li __New for Genoa__
                                */
  PP_START_LANE             ,  /**< (UINT8) Starting lane for MPIO discovered ports
                                * @li __New for Genoa__
                                */
  PP_OCP_PRESENT_START      ,  /**< (UINT8) Pin number on I2C Expander of first OCP PRESENT pin
                                * @li __New for Genoa__
                                */
  PP_OCP_PRESENT_COUNT      ,  /**< (UINT8) Number of OCP PRESENT pins on I2C Expander
                                * @li __New for Genoa__
                                */
  PP_U3_PRESENT_PIN         ,  /**< (UINT8) Pin number of U.3 PRESENT pin on I2C Expander
                                * @li __New for Genoa__
                                */
  PP_U3_IFDET_PIN           ,  /**< (UINT8) Pin number of U.3 IFDET pin on I2C Expander
                                * @li __New for Genoa__
                                */
  PP_U3_IFDET2_PIN          ,  /**< (UINT8) Pin number of U.3 IFDET2 pin on I2C Expander
                                * @li __New for Genoa__
                                */
  PP_ALWAYS_EXPOSE          ,  /**< (__BOOLEAN__) Always Expose Unused PCIE port
                                *  @li @b Disable = 0 - Not exposed
                                *  @li @b Enable = 1 - Always exposed
                                */
  PP_SRIS_ENABLED           ,  /**< (UINT8) SRIS Capability Enabled
                                *  @li @b Disable = 0
                                *  @li @b Enable = 1
                                */
  PP_SRIS_SKIP_INTERVAL     ,  /**< (UINT8) Controls SRIS SKP generation interval
                                *  @li @b 0 - 1506; 144; 6050; 640
                                *  @li @b 1 - 1538; 154; 6068; 656
                                *  @li @b 2 - 1358; 128; 6032; 624
                                *  @li @b 3 - 1180; 112; 5996; 608
                                */
  PP_SRIS_LOWER_OS_GEN_SUP  ,  /**< (UINT8) Controls LOWER_SKP_OS_GEN_SUPPORT
                                *  @li @b 0x0 - Disable
                                *  @li @b 0x1 - Gen1
                                *  @li @b 0x3 - Gen2
                                *  @li @b 0x7 - Gen3
                                *  @li @b 0xF - Gen4
                                */
  PP_SRIS_LOWER_OS_RCV_SUP  ,  /**< (UINT8) Controls LOWER_SKP_OS_RCV_SUPPORT
                                *  @li @b 0x0 - Disable
                                *  @li @b 0x1 - Gen1
                                *  @li @b 0x3 - Gen2
                                *  @li @b 0x7 - Gen3
                                *  @li @b 0xF - Gen4
                                */
  PP_SRIS_AUTODECTECT_MODE  ,  /**< (UINT8) Controls SRIS Autodetect mode
                                *  @li @b Disable = 0
                                *  @li @b Enable = 1
                                */
  PP_SRIS_SKP_INTERVAL_SEL  ,  /**< (UINT8) Controls SRIS SKP Interval Selection Mode
                                *  @li @b 0x0 - SKP ordered set Interval Lock Mode
                                *  @li @b 0x1 - Dynamic SKP ordered set Interval Mode
                                *  @li @b 0x2 - Far End Nominal Empty Mode
                                */
  PP_SRIS_AUTODECTECT_FACTOR,  /**< (UINT8) Controls Autodetect Factor
                                *  @li @b 0x0 - 1x
                                *  @li @b 0x1 - 0.95x
                                *  @li @b 0x2 - 0.9x
                                *  @li @b 0x3 - 0.85x
                                */
  PP_LEGACY_SWITCH0_ADDR,      /**< (UINT8) I2C Address of Legacy I2c Switch if __(optional)__
                                *   Defines an 9545 or 9546 I2c witch on the hotplug I2c bus
                                *   *BEFORE* the switch defined in the legacy hotplug descriptor
                                *   tables.  This switch will be accessed before the switch in
                                *   the descriptor tables.
                                *   Valid values are 0x70-0x77
                                * @li __New for Genoa__
                                */
  PP_LEGACY_SWITCH0_SELECT     /**< (UINT8) I2C Select of Legacy I2c Switch __(optional)__
                                *   Valid values are 0-3
                                * @li __New for Genoa__
                                */
} DXIO_PORT_PARAM_TYPE;

typedef enum {
  TX_DEEMPHASIS_CSR = 0,         ///< Use de-emphasis from CSR
  TX_DEEMPHASIS_US = 0x1,        ///< Use de-emphasis requested by upstream component
  TX_DEEMPHASIS_60 = 0x2,        ///< Use -6.0 dB
  TX_DEEMPHASIS_35 = 0x3,        ///< Use -3.5 dB
  TX_DEEMPHASIS_DIS = 0xFF,      ///< Skip override setting
} GEN2_TX_DEEMPHASIS;

/**
 *  @brief GPIO Expander Type
 *
 *  This enumerator defines the GPIO expander types.
 */
typedef enum {
  UBM_GPIO_EXPANDER_TYPE_9539 = 0,
  UBM_GPIO_EXPANDER_TYPE_9535,
  UBM_GPIO_EXPANDER_TYPE_9506
} UBM_GPIO_EXPANDER_TYPE;

/**
 *  @brief GPIO Switch Type
 *
 *  This enumerator defines the GPIO switch types.
 */
typedef enum {
  UBM_GPIO_SWITCH_TYPE_9545 = 0,
  UBM_GPIO_SWITCH_TYPE_9546
} UBM_GPIO_SWITCH_TYPE;

#define PORT_PHY_PARAM(x,y) PORT_PARAM(PP_PHY_PARAM, ((y << 8) + x))
/**
 * @brief PHY Programming Data
 * @details
 */

typedef struct {
  IN       UINT16               ParamType;      ///< parameter type
  IN       UINT16               ParamValue;     ///< parameter value
} DXIO_PHY_PARAM;

typedef struct {
  IN       DXIO_PHY_PARAM       PhyParam [44];      ///< physical parameter
} PHY_DATA;

#define PHY_PARAMS_START  ,{{
#define PHY_PARAMS_END    }}
#define PHY_PARAM(x,y)    {x, y}
#define NO_PHY_PARAMS_DATA      PHY_PARAMS_START PHY_PARAM(0,0) PHY_PARAMS_END

typedef enum {
  rxX_los_threshold = 1     ,
  rx_vref_ctrl              ,
  SSC_OFF_FRUG1             ,
  SSC_OFF_PHUG1             ,

  GEN1_txX_eq_pre = (1 << 6),
  GEN1_txX_eq_main          ,
  GEN1_txX_eq_post          ,
  GEN1_txX_iboost_lvl       ,
  GEN1_txX_vboost_en        ,
  GEN1_Unsupported1         ,
  GEN1_Unsupported2         ,
  GEN1_rxX_eq_att_lvl       ,
  GEN1_rxX_eq_vga1_gain     ,
  GEN1_rxX_eq_vga2_gain     ,
  GEN1_rxX_eq_ctle_pole     ,
  GEN1_rxX_eq_ctle_boost    ,
  GEN1_rxX_eq_dfe_tap1      ,
  GEN1_tx_vboost_lvl        ,
  GEN1_SSC_OFF_FRUG1        ,
  GEN1_SSC_OFF_PHUG1        ,
  GEN1_SSC_ON_FRUG1         ,
  GEN1_SSC_ON_PHUG1         ,
  GEN1_txX_pre_deemphasis   ,
  GEN1_txX_post_deemphasis  ,

  GEN2_txX_eq_pre = (2 << 6),
  GEN2_txX_eq_main          ,
  GEN2_txX_eq_post          ,
  GEN2_txX_iboost_lvl       ,
  GEN2_txX_vboost_en        ,
  GEN2_Unsupported1         ,
  GEN2_Unsupported2         ,
  GEN2_rxX_eq_att_lvl       ,
  GEN2_rxX_eq_vga1_gain     ,
  GEN2_rxX_eq_vga2_gain     ,
  GEN2_rxX_eq_ctle_pole     ,
  GEN2_rxX_eq_ctle_boost    ,
  GEN2_rxX_eq_dfe_tap1      ,
  GEN2_tx_vboost_lvl        ,
  GEN2_SSC_OFF_FRUG1        ,
  GEN2_SSC_OFF_PHUG1        ,
  GEN2_SSC_ON_FRUG1         ,
  GEN2_SSC_ON_PHUG1         ,
  GEN2_txX_pre_deemphasis   ,
  GEN2_txX_post_deemphasis  ,

  GEN3_txX_eq_pre = (3 << 6),
  GEN3_txX_eq_main          ,
  GEN3_txX_eq_post          ,
  GEN3_txX_iboost_lvl       ,
  GEN3_txX_vboost_en        ,
  GEN3_rxX_adapt_afe_en     ,
  GEN3_rxX_adapt_dfe_en     ,
  GEN3_rxX_eq_att_lvl       ,
  GEN3_rxX_eq_vga1_gain     ,
  GEN3_rxX_eq_vga2_gain     ,
  GEN3_rxX_eq_ctle_pole     ,
  GEN3_rxX_eq_ctle_boost    ,
  GEN3_rxX_eq_dfe_tap1      ,
  GEN3_tx_vboost_lvl        ,
  GEN3_SSC_OFF_FRUG1        ,
  GEN3_SSC_OFF_PHUG1        ,
  GEN3_SSC_ON_FRUG1         ,
  GEN3_SSC_ON_PHUG1         ,
  GEN3_txX_pre_deemphasis   ,
  GEN3_txX_post_deemphasis  ,

  GEN4_txX_eq_pre = (4 << 6),
  GEN4_txX_eq_main          ,
  GEN4_txX_eq_post          ,
  GEN4_txX_iboost_lvl       ,
  GEN4_txX_vboost_en        ,
  GEN4_rxX_adapt_afe_en     ,
  GEN4_rxX_adapt_dfe_en     ,
  GEN4_rxX_eq_att_lvl       ,
  GEN4_rxX_eq_vga1_gain     ,
  GEN4_rxX_eq_vga2_gain     ,
  GEN4_rxX_eq_ctle_pole     ,
  GEN4_rxX_eq_ctle_boost    ,
  GEN4_rxX_eq_dfe_tap1      ,
  GEN4_tx_vboost_lvl        ,
  GEN4_SSC_OFF_FRUG1        ,
  GEN4_SSC_OFF_PHUG1        ,
  GEN4_SSC_ON_FRUG1         ,
  GEN4_SSC_ON_PHUG1         ,
  GEN4_txX_pre_deemphasis   ,
  GEN4_txX_post_deemphasis
} DXIO_PHY_PARAM_TYPE;


#define MAX_SIZE_OVERRIDE_TABLE  256      ///< Maximum size of the override table

typedef struct {
  UINT32         size;                            //size
  UINT8          data[MAX_SIZE_OVERRIDE_TABLE];   //data array of maximum size of override table
} DXIO_PHYOVR_TABLE;

/**
 * @brief PCIe specific data structures
 * @details PCIe port misc extended controls
 */
typedef struct  {
  IN      UINT8                     LinkComplianceMode     :1;  ///< Force port into compliance mode (device will not be trained, port output compliance pattern)
  IN      UINT8                     LinkSafeMode           :1;  /**< Safe mode PCIe capability. (Parameter may limit PCIe speed requested through DXIO_PORT_DATA::LinkSpeedCapability)
                                                                 *  @li @b 0 - port can advertize muximum supported capability
                                                                 *  @li @b 1 - port limit advertized capability and speed to PCIe Gen1
                                                                 */
  IN      UINT8                     SbLink                 :1;  /**< PCIe link type
                                                                 *  @li @b 0 - General purpose port
                                                                 *  @li @b 1 - Port connected to SB
                                                                 */
  IN      UINT8                     ClkPmSupport           :1;  /**< Clock Power Management Support
                                                                 *  @li @b 0 - Clock Power Management not configured
                                                                 *  @li @b 1 - Clock Power Management configured according to PCIe device capability
                                                                 */
  IN      UINT8                     ChannelType            :3;  /**< Channel Type
                                                                 *  @li @b 0 - Channel Type Not Specified
                                                                 *  @li @b 1 - Channel Type Short Trace
                                                                 *  @li @b 2 - Channel Type Long Trace
                                                                 */
  IN      UINT8                     TurnOffUnusedLanes     :1;   /**< Turn Off Unused Lanes
                                                                 *  @li @b 0 -
                                                                 *  @li @b 1 -
                                                                 */
} DXIO_PORT_MISC_CONTROL;

/**
 * @brief The IO APIC Interrupt Mapping Info
 * @details
 */
typedef struct {
  IN      UINT8                     GroupMap;               /**< Group mapping for slot or endpoint device (connected to PCIE port) interrupts .
                                                             *  @li <b>0</b> - IGNORE THIS STRUCTURE AND USE RECOMMENDED SETTINGS
                                                             *  @li <b>1</b> - mapped to Grp 0 (Interrupts 0..3   of IO APIC redirection table)
                                                             *  @li <b>2</b> - mapped to Grp 1 (Interrupts 4..7   of IO APIC redirection table)
                                                             *  @li ...
                                                             *  @li <b>8</b> - mapped to Grp 7 (Interrupts 28..31 of IO APIC redirection table)
                                                             */
  IN      UINT8                     Swizzle;                /**< Swizzle interrupt in the Group.
                                                             *  @li <b>0</b> - ABCD
                                                             *  @li <b>1</b> - BCDA
                                                             *  @li <b>2</b> - CDAB
                                                             *  @li <b>3</b> - DABC
                                                             */
  IN      UINT8                     BridgeInt;              /**< IOAPIC redirection table entry for PCIE bridge interrupt
                                                             *  @li <b>0</b>  - Entry 0  of IO APIC redirection table
                                                             *  @li <b>1</b>  - Entry 1  of IO APIC redirection table
                                                             *  @li ...
                                                             *  @li <b>31</b> - Entry 31 of IO APIC redirection table
                                                             */
} DXIO_APIC_DEVICE_INFO;

/**
 * @brief PCIe port configuration data
 * @details
 */
typedef struct  {
  IN       UINT8                   PortPresent         :1 ;   ///< Enable PCIe port for initialization.
  IN       UINT8                   Reserved1           :2 ;   /// Unused bits
  IN       UINT8                   DeviceNumber        :5 ;   /**< PCI Device number for port.
                                                               *   @li @b 0 - Native port device number
                                                               *   @li @b N - Port device number (See available configurations in BKDG
                                                               */
  IN       UINT8                   FunctionNumber      :3 ;   ///< Reserved for future use
  IN       UINT8                   LinkSpeedCapability :3 ;   /**< PCIe link speed/
                                                               *  @li @b 0 - Maximum supported by silicon
                                                               *  @li @b 1 - Gen1
                                                               *  @li @b 2 - Gen2
                                                               *  @li @b 3 - Gen3
                                                               *  @li @b 4 - Gen4
                                                               *  @li @b 5 - Gen5
                                                               */
  IN       UINT8                   AutoSpdChng         :2 ;   /**< Upstread Auto Speed Change Allowed/
                                                               *  @li @b 0 - Use default implementation (Disabled for Gen1, Enabled for Gen2/3)
                                                               *  @li @b 1 - Always Disabled
                                                               *  @li @b 2 - Always Enabled
                                                               *  @li @b 3 - Reserved
                                                               */
  IN       UINT8                   EqPreset            :4 ;   /**< Gen3 Equalization Preset */
  IN       UINT8                   LinkAspm            :2 ;   /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - L0s only
                                                               *  @li @b 2 - L1 only
                                                               *  @li @b 3 - L0s and L1
                                                               */
  IN       UINT8                   LinkAspmL1_1        :1 ;   /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Enabled
                                                               */
  IN       UINT8                   LinkAspmL1_2        :1 ;   /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Enabled
                                                               */
  IN       UINT8                   ClkReq              :4 ;   /**< ASPM Reserved Field
                                                               *  @li @b 0 - NONE
                                                               *  @li @b 1 - CLKREQ0 signal
                                                               *  @li @b 2 - CLKREQ1 signal
                                                               *  @li @b 3 - CLKREQ2 signal
                                                               *  @li @b 4 - CLKREQ3 signal
                                                               *  @li @b 5 - CLKREQG signal
                                                               */
  IN       UINT8                   LinkHotplug         :4 ;   /**< Hotplug control.
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Basic
                                                               *  @li @b 2 - Server
                                                               *  @li @b 3 - Enhanced
                                                               */
  IN       UINT8                   SlotPowerLimit;           ///< PCIe slot power limit.
  IN       UINT8                   SlotPowerLimitScale :2 ;  /**< PCIe slot power limit Scale.
                                                               *  @li @b 00b = 1.0x
                                                               *  @li @b 01b = 0.1x
                                                               *  @li @b 10b = 0.01x
                                                               *  @li @b 11b = 0.001x
                                                               */
  IN       UINT8                   IsMasterPLL         :1 ;  ///< IsMasterPLL
  IN       UINT8                   Gen4Features        :5 ;  /**< Unused bits
                                                               *< BIT0(DLF_Exchange) 1 - Disable, 0 - Enable
                                                               *< BIT1(DLF_Capability) 1 - Disable, 0 - Enable
                                                               */
  IN       UINT16                  SlotNum             :13;  ///< PHYSICAL_SLOT_NUM
  IN       UINT16                  CsLink              :3 ;  //   Reserved for internal use
  IN       DXIO_PORT_MISC_CONTROL  MiscControls;             ///< Misc extended controls
  IN       DXIO_APIC_DEVICE_INFO   ApicDeviceInfo;           ///< IOAPIC device programming info
  IN       DXIO_ENDPOINT_STATUS    EndpointStatus;           ///< PCIe endpoint (device connected to PCIe port) status
  IN       UINT8                   EsmSpeedBump;             ///< Speed bump for ESM
  IN       UINT8                   EsmControl          :1 ;  ///< Enhanced speed mode control
  IN       UINT8                   CcixControl         :1 ;  ///< Ccix/Cxl control
  IN       UINT8                   TxVetting           :1;   ///< Tx Vetting
  IN       UINT8                   RxVetting           :1;   ///< Rx Vetting
  IN       UINT8                   InvertPolarity      :1;   ///< Invert RX Polarity
  IN       UINT8                   InvertPolarity2     :1;   ///< Invert TX Polarity
  IN       UINT8                   SetGen3FixedPreset  :1;   ///< Gen3 Fixed Preset Set
  IN       UINT8                   SetGen4FixedPreset  :1;   ///< Gen4 Fixed Preset Set
  IN       UINT8                   Gen3FixedPreset     :4;   ///< Gen3 Fixed Preset
  IN       UINT8                   Gen4FixedPreset     :4;   ///< Gen4 Fixed Preset
  IN       UINT16                  PsppPolicyDC        ;     ///< Pspp DC control
  IN       UINT16                  PsppPolicyAC        ;     ///< PSPP AC control
  IN       UINT8                   PsppDeviceType      ;     ///< Pspp Device Type
  IN       UINT8                   NtbHotplug          :1;   /**< NTB Hotplug flag
                                                              *  @li @b 0b = Disabled
                                                              *  @li @b 1b = Enabled
                                                              */
  IN       UINT8                   DisGen3EQPhase      :1;   ///< Gen3 Bypass phase2/3 EQ
  IN       UINT8                   DisGen4EQPhase      :1;   ///< Gen4 Bypass phase2/3 EQ
  IN       UINT8                   TXDeEmphasisOride   :1;   ///< Override Gen2 DXIO deemphasis default
  IN       UINT8                   TXDeEmphasis        :2;   ///< Gen2 DXIO deemphasis setting
  IN       UINT8                   SetGen3LaneEq       :1;   ///< Gen3 Lane Equalization Set
  IN       UINT8                   SetGen4LaneEq       :1;   ///< Gen4 Lane Equalization Set
  struct {
    IN     UINT16                  DsTxPreset          :4;   ///< Gen3 Downstream Tx Preset
    IN     UINT16                  DsRxPresetHint      :3;   ///< Gen3 Downstream Rx Preset Hint
    IN     UINT16                  UsTxPreset          :4;   ///< Gen3 Upstream Tx Preset
    IN     UINT16                  UsRxPresetHint      :3;   ///< Gen3 Upstream Rx Preset Hint
    IN     UINT16                  Reserved1           :2;   ///< Unused bits
  } LaneEqualizationCntl;                                    ///< Lane equalization control structure used for Gen3 values
  struct {
    IN     UINT8                   DsTxPreset          :4;   ///< Gen4 Downstream Tx Preset
    IN     UINT8                   UsTxPreset          :4;   ///< Gen4 Upstream Tx Preset
  } Gen4LaneEqualizationCntl;                                ///< Lane equalization control structure used for Gen4 values
  IN       UINT8                   TargetLinkSpeed     :3;   ///< Target Link Speed
  IN       UINT8                   BypassGen3EQ        :1;   ///< Bypass Gen3 equalization
  IN       UINT8                   BypassGen4EQ        :1;   ///< Bypass Gen4 equalization
  IN       UINT8                   AlwaysExpose        :1;   ///< Always expose unused PCIE port
  IN       UINT8                   SrisSkipInterval    :2;   ///< Controls SRIS SKP generation interval
  IN       UINT8                   SrisEnableMode      :4;   ///< 0:Disable 1:Enable 0xF:Auto
  IN       UINT8                   SrisAutoDetectMode  :4;   ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  IN       UINT8                   LowerSkpOsGenSup    ;     ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  IN       UINT8                   LowerSkpOsRcvSup    ;     ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  IN       UINT8                   SrisSkpIntervalSel  :2;   ///< Controls SRIS SKIP Interval Selection Mode
  IN       UINT8                   SrisAutodetectFactor:2;   ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  IN       UINT8                   I2CMuxInfo          :6;   ///< Legacy I2c switch
  IN       UINT8                   Reserved2           :4;   ///< Unused bits
} DXIO_PORT_DATA;

/**
 * @brief EtherNet specific data structures
 * @details
 */
typedef struct  {
  IN       UINT32               PortNum           :8;     ///< Port Number
  IN       UINT32               PlatConf          :4;     /**< Platform Config
                                                            *  0 = Reserved
                                                            *  1 = 10G/1G BackPlane
                                                            *  2 = 2.5G BackPlane
                                                            *  3= Soldered down 1000Base-T
                                                            *  4 = Soldered down 1000Base-X
                                                            *  5 = Soldered down NBase-T
                                                            *  6 = Soldered down 10GBase-T
                                                            *  7 = Soldered down 10GBase-r
                                                            *  8 = SFP+ Connector
                                                            */
  IN       UINT32               Reserved1         :4;     ///< Unused 12-15
  IN       UINT32               MdioId            :5;     ///< MDIO ID when MDIO Side band is used
  IN       UINT32               Reserved2         :1;     ///< Unused 21
  IN       UINT32               SuppSpeed         :5;     /**< Supported Speeds by Platform
                                                            *  1 = 10M Supported
                                                            *  2 = 100M Supported
                                                            *  4 = 1G Supported
                                                            *  8 = 2.5G Supported
                                                            *  16 = 10G Supported
                                                            */
  IN       UINT32               Reserved3         :1;     ///< Unused 27
  IN       UINT32               ConnType          :3;     /**< Supported Speeds by Platform
                                                            *  0 = Port not Used
                                                            *  1 = SFP+ Connection I2C interface
                                                            *  2 = MDIO PHY
                                                            *  4 = Backplane Connection
                                                            */
  IN       UINT32               Reserved4         :1;     ///< Unused 31
} ETH_PORT_PROPERTY0;

typedef struct  {
  IN       UINT32               MdioReset         :2;     /**< MDIO Reset Type
                                                            *  0 = None
                                                            *  1 = I2C GPIO
                                                            *  2 = Integrated GPIO
                                                            *  3 = Reserved
                                                            */
  IN       UINT32               Reserved1         :2;     ///< Unused 2-3
  IN       UINT32               MdioGpioResetNum  :2;     ///< Integrated GPIO number for reset
  IN       UINT32               Reserved2         :2;     ///< Unused 6-7
  IN       UINT32               SfpGpioAdd        :3;     ///< Lower I2C address of GPIO Expander PCA9535
  IN       UINT32               Reserved3         :1;     ///< Unused 11
  IN       UINT32               TxFault           :4;     ///< TX FAULT
  IN       UINT32               Rs                :4;     ///< RS Signal
  IN       UINT32               ModAbs            :4;     ///< MOD_ABS signal
  IN       UINT32               RxLoss            :4;     ///< Rx_LOS signal
  IN       UINT32               SfpGpioMask       :4;     ///< SFP+ sideband signals that are not supported by platform
} ETH_PORT_PROPERTY3;

typedef struct  {
  IN       UINT32               SfpMux            :3;     ///< Lower address of Mux PCA 9545
  IN       UINT32               Reserved1         :1;     ///< Unused 3
  IN       UINT32               SfpBusSeg         :3;     ///< SFP BUS Segment. Downstream channels of PCA9545
  IN       UINT32               Reserved2         :1;     ///< Unused 7
  IN       UINT32               SfpMuxUpAdd       :5;     ///< Upper address of Mux PCA 9545
  IN       UINT32               Reserved3         :3;     ///< Unused 13-15
  IN       UINT32               RedriverAddress   :7;     ///< Address of ReDriver
  IN       UINT32               RedriverInterface :1;     ///< ReDriver Interface Descriptor
  IN       UINT32               RedriverLane      :3;     ///< ReDriver Lane number
  IN       UINT32               Reserved4         :1;     ///< Unused 27
  IN       UINT32               RedriverModel     :3;     ///< ReDriver Model
  IN       UINT32               RedriverPresent   :1;     ///< Redriver Present
} ETH_PORT_PROPERTY4;

typedef struct  {
  IN       UINT32               TxEqPre           :6;     ///< TX EQ PRE
  IN       UINT32               Reserved1         :2;     ///< Unused 7-6
  IN       UINT32               TxEqMain          :6;     ///< TX EQ MAIN
  IN       UINT32               Reserved2         :2;     ///< Unused 15-14
  IN       UINT32               TxEqPost          :6;     ///< TX EQ POST
  IN       UINT32               Reserved3        :7;     ///< Unused 28-22
  IN       UINT32               TxEqApplicable    :1;     ///< TxEqApplicable
  IN       UINT32               TxEqOvrRide       :1;     ///< TX_EQ_OVR_RIDE  //VR_XS_PMA_MP_12G_16G_25G_TX_EQ_CTRL1[6]
  IN       UINT32               TxEqDefCtrl       :1;     ///< TX_EQ_DEF_CTRL  //VR_XS_PMA_MP_12G_16G_25G_TX_EQ_CTRL1[7]

} ETH_PORT_TXEQ;

typedef struct  {
  IN       UINT32               RxEqApplicable    :1;     ///< This bit will decide to consider below values or not.
  IN       UINT32               CtleBoost0        :5;     ///< CTLE_BOOST_0  //XPCSDWCXPCS::VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0_instXPCS0_aliasSMN[4:0]
  IN       UINT32               CtlePole0         :2;     ///< CTLE_POLE_0   //XPCSDWCXPCS::VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0_instXPCS0_aliasSMN[6:5]
  IN       UINT32               Vga2Gain0         :3;     ///< VGA2_GAIN_0   //XPCSDWCXPCS::VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0_instXPCS0_aliasSMN[10:8]
  IN       UINT32               Vga1Gain0         :3;     ///< VGA1_GAIN_0   //XPCSDWCXPCS::VR_XS_PMA_MP_16G_25G_RX_EQ_CTRL0_instXPCS0_aliasSMN[14:12]
  IN       UINT32               RxAdReq           :1;     ///< RX_AD_REQ     //XPCSDWCXPCS::VR_XS_PMA_MP_12G_16G_25G_RX_EQ_CTRL4_instXPCS0_aliasSMN[12]
  IN       UINT32               RxDfeByp0         :1;     ///< RX_DFE_BYP_0  //XPCSDWCXPCS:: VR_XS_PMA_MP_16G_25G_RX_GENCTRL4 _instXPCS0_aliasSMN[8]
  IN       UINT32               DfeTap1_0         :8;     ///< DFE_TAP1_0    //XPCSDWCXPCS::VR_XS_PMA_MP_12G_16G_25G_DFE_TAP_CTRL0_instXPCS0_aliasSMN[7:0]
  IN       UINT32               Rx0EqAttLvl       :3;     ///< RX0_EQ_ATT_LVL  //XPCSDWCXPCS::VR_XS_PMA_MP_12G_16G_25G_RX_ATTN_CTRL_instXPCS0_aliasSMN[2:0]
  IN       UINT32               Reserved          :5;     ///< reserved
} ETH_PORT_RXEQ;

typedef struct  {
  IN       UINT32               BoostApplicable   :1;     ///< This bit will decide to consider below values or not.
  IN       UINT32               VboostEn0         :1;     ///< VBOOST_EN_0  //XPCSDWCXPCS::VR_XS_PMA_MP_12G_16G_25G_TX_GENCTRL1_instXPCS0_aliasSMN[4]
  IN       UINT32               Tx0Iboost         :4;     ///< TX0_IBOOST  //XPCSDWCXPCS::VR_XS_PMA_MP_12G_16G_25G_TX_BOOST_CTRL_instXPCS0_aliasSMN[3:0]
  IN       UINT32               TxVboostLvl       :3;     ///< tx_vboost_lvl   //( This is for Phy. Not for port)  PCS::DXIO::PCS_PMACONFIG_CONTROL10[2:0]
  IN       UINT32               Reserved1         :23;
} ETH_PORT_BOOST;

typedef struct  {
  IN       ETH_PORT_RXEQ     EthPortRxEq;              ///< RX EQ Settings
  IN       ETH_PORT_BOOST    EthPortBoost;             ///< BOOST Settings
} ETHERNET_PORT_DATA_EXT;

/// Ethernet PCIe port configuration data
typedef struct  {
  IN       ETH_PORT_PROPERTY0   EthPortProp0;             ///< XGBE_PORT_PROPERTY_0
  IN       ETH_PORT_PROPERTY3   EthPortProp3;             ///< XGBE_PORT_PROPERTY_3
  IN       ETH_PORT_PROPERTY4   EthPortProp4;             ///< XGBE_PORT_PROPERTY_4
  IN       UINT32               PadMux0;                  ///< PadMux0 Setting (8 bits)
  IN       UINT32               PadMux1;                  ///< PadMux1 Setting (8 bits)
  IN       UINT32               MacAddressLo;             ///< Lower 32 bits of MAC Address
  IN       UINT32               MacAddressHi;             ///< Upper 32 bits of MAC Address
  IN       ETH_PORT_TXEQ        EthPortTxEq;              ///< TX EQ Settings
} ETHERNET_PORT_DATA;

/**
 * @brief High level data structures for passing topology from platform to AGESA
 * @details Engine Configuration
 */
typedef struct {
  IN       UINT8                EngineType;               /**< Engine type
                                                           *  @li @b 0 -  Ignore engine configuration
                                                           *  @li @b 1 -  PCIe port
                                                           *  @li @b 2 -  DDI
                                                           */
  IN       UINT8                HotPluggable  :1;         /**<HotPluggable
                                                           *  @li @b 0 - Link is NOT Hot-Switchable
                                                           *  @li @b 1 - Link IS Hot-Switchable
                                                           */
  IN       UINT8                Reserved1     :7;         /**< Unused field, leave as 0
                                                           */
  IN       UINT8                StartLane;                /**< Start Lane ID (in reversed configuration StartLane > EndLane)
                                                           * Refer to lane descriptions and supported configurations in BKDG
                                                           */
  IN       UINT8                EndLane;                  /**< End lane ID (in reversed configuration StartLane > EndLane)
                                                           * Refer to lane descriptions and supported configurations in BKDG
                                                           */
  IN       UINT8                GpioGroupId;              /**< Unique identifier for the GPIO or GPIO group associated with
                                                           * this engine.  GPIOs are used for hotplug notification and link
                                                           * type (e.g SATA Express or PCIe)
                                                           */
  IN       UINT8                DxioStartLane;            /**< Internal coding of start lane
                                                           */
  IN       UINT8                DxioEndLane;              /**< Internal coding of end lane
                                                           */
  IN       UINT8                SearchDepth;              /**< SearchDepth only uses 1 bit - always initialize to 0 will be updated dynamically
                                                           */
} DXIO_ENGINE_DATA;

/**
 * @brief DDI channel lane mapping
 * @details
 */

typedef struct {                                          ///< Structure that discribe lane mapping
  IN      UINT8              Lane0   :2;                  /**< Lane 0 mapping
                                                           *  @li @b 0 - Map to lane 0
                                                           *  @li @b 1 - Map to lane 1
                                                           *  @li @b 2 - Map to lane 2
                                                           *  @li @b 2 - Map to lane 3
                                                           */
  IN      UINT8              Lane1   :2;                  ///< Lane 1 mapping (see "Lane 0 mapping")
  IN      UINT8              Lane2   :2;                  ///< Lane 2 mapping (see "Lane 0 mapping")
  IN      UINT8              Lane3   :2;                  ///< Lane 3 mapping (see "Lane 0 mapping")
} DXIO_CHANNEL_MAPPING;

/**
 * @brief Common channel mapping
 * @details
 */
typedef union {
  IN      UINT8                ChannelMappingValue;       ///< Raw lane mapping
  IN      DXIO_CHANNEL_MAPPING      ChannelMapping;            ///< Channel mapping
} DXIO_CONN_CHANNEL_MAPPING;
/**
 * @brief DDI configuration data
 * @details
 */
typedef struct  {
  IN       UINT8                ConnectorType;            /**< Display Connector Type
                                                            *  @li @b 0 - DP
                                                            *  @li @b 1 - eDP
                                                            *  @li @b 2 - Single Link DVI-D
                                                            *  @li @b 3 - Dual  Link DVI-D (see @ref DualLinkDviDescription "Example Dual Link DVI connector description")
                                                            *  @li @b 4 - HDMI
                                                            *  @li @b 5 - DP-to-VGA
                                                            *  @li @b 6 - DP-to-LVDS
                                                            *  @li @b 7 - Hudson-2 NutMeg DP-to-VGA
                                                            *  @li @b 8 - Single Link DVI-I
                                                            *  @li @b 9 - Native CRT (Family 0x14)
                                                            *  @li @b 10 - Native LVDS (Family 0x14)
                                                            *  @li @b 11 - Auto detect LCD panel connector type. VBIOS is able to auto detect the LVDS connector type: native LVDS, eDP or DP-to-LVDS
                                                            *              The auto detection method only support panel with EDID.
                                                            */
  IN       UINT8                AuxIndex;                 /**< Indicates which AUX or DDC Line is used
                                                            *  @li @b 0 - AUX1
                                                            *  @li @b 1 - AUX2
                                                            *  @li @b 2 - AUX3
                                                            *  @li @b 3 - AUX4
                                                            *  @li @b 4 - AUX5
                                                            *  @li @b 5 - AUX6
                                                            */
  IN       UINT8                HdpIndex;                 /**< Indicates which HDP pin is used
                                                            *  @li @b 0 - HDP1
                                                            *  @li @b 1 - HDP2
                                                            *  @li @b 2 - HDP3
                                                            *  @li @b 3 - HDP4
                                                            *  @li @b 4 - HDP5
                                                            *  @li @b 5 - HDP6
                                                            */
  IN       DXIO_CONN_CHANNEL_MAPPING Mapping[2];               /**< Set specific mapping of lanes to connector pins
                                                            *  @li Mapping[0] define mapping for group of 4 lanes starting at PCIe_ENGINE_DATA.StartLane
                                                            *  @li Mapping[1] define mapping for group of 4 lanes ending at PCIe_ENGINE_DATA.EndLane (only applicable for Dual DDI link)
                                                            *  if Mapping[x] set to 0 than default mapping assumed
                                                            */
  IN       UINT8                LanePnInversionMask;      /**< Specifies whether to invert the state of P and N for each lane. Each bit represents a PCIe lane on the DDI port.
                                                            *  @li 0 - Do not invert (default)
                                                            *  @li 1 - Invert P and N on this lane
                                                            */
  IN       UINT8                Flags;                    /**< Capabilities flags
                                                            *  @li Flags bit[0] DDI_DATA_FLAGS_DP1_1_ONLY Selects downgrade PHY link to DP1.1
                                                            *  @li Flags bit[7:1] Reserved
                                                            */
} DDI_DATA;

/**
 * @brief PCIe port descriptor
 * @details
 */
typedef struct {
  IN       UINT32               Flags;                    /**< Descriptor flags
                                                           * @li @b Bit31 - last descriptor in complex
                                                           */
  IN       DXIO_ENGINE_DATA     EngineData;               ///< Engine data
  IN       DXIO_PORT_DATA       Port;                     ///< PCIe port specific configuration info
  IN       ETHERNET_PORT_DATA   EtherNet;                 ///< Ancillary data for EtherNet
  IN       ETHERNET_PORT_DATA_EXT EtherNetExt;            ///< Ancillary data for EtherNet RX and Boost Parameters
  IN       PHY_DATA             Phy;                      ///< Ancillary data for PHY programming customization
  IN       PORT_PARAMS          PortParams;               ///< Extensible port parameter list for simplified topology structure
} DXIO_PORT_DESCRIPTOR;

/**
 * @brief DDI descriptor
 * @details
 */
typedef struct {
  IN       UINT32          Flags;                    /**< Descriptor flags
                                                           * @li @b Bit31 - last descriptor in complex
                                                           */
  IN       DDI_DATA        Ddi;                      ///< DDI port specific configuration info
} DDI_DESCRIPTOR;

/**
 * @brief PCIe Complex descriptor
 * @details
 */
typedef struct {
  IN       UINT32               Flags;                    /**< Descriptor flags
                                                           * @li @b Bit31 - last descriptor in topology
                                                           */
  IN       UINT32               SocketId;                 ///< Socket Id
  IN       DXIO_PORT_DESCRIPTOR *PciePortList;            ///< Pointer to array of PCIe port descriptors or NULL (Last element of array must be terminated with DESCRIPTOR_TERMINATE_LIST).
  IN       DDI_DESCRIPTOR       *DdiLinkList;             ///< Reserved for compatibility with FM15
  IN       VOID                 *Reserved2;               ///< Reserved for future use
  IN       UINT8                BmcLinkLocation;          ///< Identifies the socket/die location of a BMC link (Used by AGESA, input not required)
  IN       UINT8                BmcLinkLaneNum;           ///< Identifies the socket/die location of a BMC Lane number
  IN       UINT8                Reserved3[2];             ///< Reserved for future
} DXIO_COMPLEX_DESCRIPTOR;

/**
 * @brief Socket Number Enumeration
 * @details
 */
typedef enum {
  DxioSocket0 = 0,                                        ///< Socket 0
  DxioSocket1 = 1,                                        ///< Socket 1
  DxioBixbySocket = 0x5                                   ///< Bixby Socket
} DXIO_SOCKET_ENUM;

/**
 * @brief Engine descriptor type
 * @details
 */
typedef enum {
  DxioUnusedEngine = 0,                                   ///< Unused descriptor Excluded from configuration
  DxioPcieEngine = 1,                                     ///< PCIe port
  DxioUSBEngine = 2,                                      /**< USB port
                                                           *   @li __Deprecated__
                                                           */
  DxioSATAEngine = 3,                                     ///< SATA
  DxioUSB_OVER_PCIE = 4,                                  //   USB4 PCIe (internal use only)
  DxioUBMHFCEngine = 5,                                   ///< New for Genoa UBM HFC Connector for auto-discovery
  DxioOCP3Engine = 6,                                     ///< New for Genoa OCP3 Bifurcatable Connector
  DxioUdot3Engine = 7,                                    ///< New for Genoa U.3 Multiprotocol Connector
  DxioDPEngine = 8,                                       ///< Digital Display __For APU display connector list__
  DxioEthernetEngine = 0x10,                              /**< Ethernet (GBe, XGBe)
                                                           *   @li __Deprecated__
                                                           */
  DxioGOPEngine = 0x20,                                   /**< GOP
                                                           *   @li __Deprecated__
                                                           */
  DxioNTBDevice = 0x60,                                   //  For NBIF NTB Enable (internal use only)
  DxioHDaudioEngine,                                      //  For NBIF HDaudtio Enable (internal use only)
  DxioACPEngine,                                          //  For NBIF ACP Enable (internal use only)
  DxioMP2Engine,                                          //  For NBIF MP2 Enable (internal use only)
  DxioMaxPcieEngine                                       ///< Max engine type for boundary check.
} DXIO_ENGINE_TYPE;


/**
 * @brief PCIe link capability/speed
 * @details
 */
typedef enum  {
  DxioGenMaxSupported,                                    ///< Maximum supported
  DxioGen1 = 1,                                           ///< Gen1
  DxioGen2,                                               ///< Gen2
  DxioGen3,                                               ///< Gen3
  DxioGen4,                                               ///< Gen4
  DxioGen5,                                               ///< Gen5
  MaxDxioGen                                              ///< Max Gen for boundary check
} DXIO_LINK_SPEED_CAP;

/**
 * @brief  PCIe link ASPM
 * @details
 */
typedef enum {
  DxioAspmDisabled,                                           ///< Disabled
  DxioAspmL0s,                                                ///< PCIe L0s link state
  DxioAspmL1,                                                 ///< PCIe L1 link state
  DxioAspmL0sL1,                                              ///< PCIe L0s & L1 link state
  DxioMaxAspm                                                 ///< Not valid value, used to verify input
} DXIO_ASPM_TYPE;

/**
 * @brief  PCIe link hotplug support
 * @details NOTE: UBM HFC hotplug types are identified during auto-discovery
 */
typedef enum {
  DxioHotplugDisabled,                                        ///< Hotplug disable
  DxioHotplugBasic,                                           ///< Basic Hotplug
  DxioHotplugServerExpress,                                   ///< Server Hotplug Express Module
  DxioHotplugEnhanced,                                        ///< Enhanced
  DxioHotplugInboard,                                         ///< Inboard
  DxioHotplugServerEntSSD,                                    ///< Server Hotplug Enterprise SSD
  DxioHotplugUBM,                                             ///< UBM Backplane
  DxioHotplugOCP,                                             ///< OCP NIC 3.0
  MaxDxioHotplug                                              //   Not valid value, used to verify input
} DXIO_LINK_HOTPLUG_TYPE;

/**
 * @brief  PCIe link initialization
 * @details NOTE: UBM HFC hotplug types are identified during auto-discovery
 */
typedef enum {
  DxioPortDisabled,                                           ///< Disable
  DxioPortEnabled                                             ///< Enable
} DXIO_PORT_ENABLE;

/**
 * @brief  PCIe ClkPmSupport initialization
 * @details NOTE: UBM HFC hotplug types are identified during auto-discovery
 */
typedef enum {
  DxioClkPmSupportDisabled,                                   ///< Disable
  DxioClkPmSupportEnabled                                     ///< Enable
} DXIO_CLKPM_SUPPORT_ENABLE;

/**
 * @brief  SATA ChannelType initialization
 * @details NOTE:
 */

typedef enum {
  DxioSataChannelOther,                                       ///< Default Channel Type
  DxioSataChannelShort,                                       ///< Short Trace Channel Type
  DxioSataChannelLong                                         ///< Long Trace Channel Type
} DXIO_SATA_CHANNEL_TYPE;

/**
 * @brief  CLKREQ signal Type initialization, CLKREQ only has 4 bits in DXIO_PORT_DATA
 * @details NOTE:
 */
typedef enum {
  DxioClkReqNone,                                             ///< Default None
  DxioClkReq0,                                                ///< CLKREQ0 signal
  DxioClkReq1,                                                ///< CLKREQ1 signal
  DxioClkReq2,                                                ///< CLKREQ2 signal
  DxioClkReq3,                                                ///< CLKREQ3 signal
  DxioClkReqG,                                                ///< CLKREQG/CLKREQ4 signal
  DxioClkReq5,                                                ///< CLKREQ5 signal
  DxioClkReq6,                                                ///< CLKREQ6 signal
  DxioClkReqInvalid = 0x8,                                    ///< Invalid
  DxioGpp0ClkReq,                                             ///< gpp0_clkreq signal - MISC2
  DxioGpp1ClkReq,                                             ///< gpp1_clkreq signal - MISC2
  DxioGpp2ClkReq,                                             ///< gpp2_clkreq signal - MISC2
  DxioGpp3ClkReq,                                             ///< gpp3_clkreq signal - MISC2
  DxioGpp4ClkReq,                                             ///< gpp4_clkreq signal - MISC2
} DXIO_CLKREQ_TYPE;

#define DxioClkReq4  DxioClkReqG
/**
 * @brief  Equalization Preset Search mode options
 * @details NOTE:
 */
typedef enum {
  DxioEqPresetSearchDirectional,                              ///< Eq Preset Search Mode - Directional - do not use
  DxioEqPresetSearchExhaustive,                               ///< Eq Preset Search Mode - Exhaustive
  DxioEqPresetSearchUnsupported,                              ///< Eq Preset Search Mode - Unsupported - do not use
  DxioEqPresetSearchPreset,                                   ///< Eq Preset Search Mode - Preset
} DXIO_EQPRESET_TYPE;

/**
 * @brief  Master PLL
 * @details
 */
typedef enum {
  DxioMasterPllDisabled,                                      ///< Master PLL Disable
  DxioMasterPllEnabled                                        ///< Master PLL Enable
} DXIO_MASTER_PLL;


/**
 *  @brief Macro for statically initializing various structures.
 *         Macro for defining DXIO_ENGINE_DATA in a DXIO topology entry.
 *
 *  @param [in] mType Specifies the DXIO_ENGINE_TYPE
 *  @param [in] mStartLane Identifies the socket-relative Start Lane (lowest numbered lane unless reversed)
 *  @param [in] mEndLane Identifies the socket-relative End Lane
 *  @param [in] mResetId A value that identifies a specific GPIO that provides PERST#
 *
 *  @details This is a simplified engine descriptor that has removed unused fields (see DXIO_ENGINE_DATA_INITIALIZER for past implementation)
 */
#define  DXIO_ENGINE_INITIALIZER(mType, mStartLane, mEndLane, mResetId) {mType, 0, 0, mStartLane, mEndLane, mResetId, 0, 0, 0}
/**
 *  @brief Macro for defining the DXIO_PORT_DATA in a DXIO topology entry.
 *
 *  @param [in] mPortPresent (__BOOLEAN__) Defines whether training should be attempted for this port, or if it should be ignored
 *
 *  @details This is an extensible macro that replaces multiple versions of DXIO_PORT_DATA_INITIALIZER_xxxx that used a fixed list of arguments.
 *           This macro uses and extensible enumerated list of paramaters associated with a topology entry and should be used in place of the
 *           versions used in the past.  While the older versions are supported, this is the preferred implementation.
 */
#define  DXIO_PORT_INITIALIZER(mPortPresent) \
                               {mPortPresent, 0, 0, 0, DxioGenMaxSupported, 0, 0, AspmL0sL1, DxioAspmDisabled, DxioAspmDisabled, 0, DxioHotplugDisabled, \
                               0, 0, 0, 0, 0, 0, {0, DxioGenMaxSupported, 0, DxioClkPmSupportDisabled, 0, 1}, {0, 0, 0}, DxioEndpointDetect}, \
                               {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}, {{{0, 0}}}

/**
 * @brief  Traditional versions
 * @details
 */
#define  DXIO_ENGINE_DATA_INITIALIZER(mType, mStartLane, mEndLane, mHotplug, mGpioGroupId) {mType, mHotplug, 0, mStartLane, mEndLane, mGpioGroupId, 0, 0, 0}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, 0, mHotplug, 0, 0, 0, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE_V2(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mClkReq) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, mClkReq, mHotplug, 0, 0, 0, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE_V3(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mClkReq, mAutoSpdChange) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, mAutoSpdChange, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, mClkReq, mHotplug, 0, 0, 0, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE_V4(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mClkReq, mAutoSpdChange, mSlotPowerLimit, mSlotPowerLimitScale) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, mAutoSpdChange, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, mClkReq, mHotplug, mSlotPowerLimit, mSlotPowerLimitScale, 0, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE_V5(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mEqPreset) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, mEqPreset, mAspm, mAspmL1_1, \
                                         mAspmL1_2, 0, mHotplug, 0, 0, 0, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}
/**
 * @brief  Enable or Disable DLF_Exchange
 * @details
 */
#define  DXIO_PORT_DATA_INITIALIZER_PCIE_V6(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mClkReq, mDlfExDisable) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, mClkReq, mHotplug, 0, 0, 0, mDlfExDisable, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE_V7(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mClkReq, mEqPreset) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, mEqPreset, mAspm, mAspmL1_1, \
                                         mAspmL1_2, mClkReq, mHotplug, 0, 0, 0, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_PCIE_MASTERPLL(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport, mIsMasterPLL) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, 0, mHotplug, 0, 0, mIsMasterPLL, 0, 0, 0, {0, mMaxLinkCap, 0, mClkPmSupport, 0, 1}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_CHIP(mPortPresent, mDevAddress, mDevFunction, mHotplug, mMaxLinkSpeed, mMaxLinkCap, \
                                         mAspm, mAspmL1_1, mAspmL1_2,  mClkPmSupport) \
                                        {mPortPresent, 0, mDevAddress, mDevFunction, mMaxLinkSpeed, 0, 0, mAspm, mAspmL1_1, \
                                         mAspmL1_2, 0, mHotplug, 0, 0, 0, 0, 0, 0, {0, mMaxLinkCap, 1, mClkPmSupport, 0, 0}, {0, 0, 0}, \
                                         DxioEndpointDetect}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_SATA(mPortPresent) {mPortPresent, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0}, 0}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_SATA_V2(mPortPresent, mChannelType) {mPortPresent, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, mChannelType, 0}, {0, 0, 0}, 0}, \
                                         {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}},{{0},{0}}

#define  DXIO_PORT_DATA_INITIALIZER_ENET(mPortPresent, mPortNum, mPlatConf, mMdioId, mSuppSpeed, mConnType, mMdioReset, mMdioGpioResetNum, \
                                         mSfpGpioAdd, mTxFault, mRs, mModAbs, mRxLoss, mSfpGpioMask, mSfpMux, mSfpBusSeg, \
                                         mSfpMuxUpAdd, mRedriverAddress, mRedriverInterface, mRedriverLane, mRedriverModel, \
                                         mRedriverPresent, mPadMux0, mPadMux1, mMacAddressLo, mMacAddressHi, \
                                         mTxEqPre, mTxEqMain, mTxEqPost) \
                                        {mPortPresent, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0}, 0}, \
                                        {{mPortNum, mPlatConf, 0, mMdioId, 0, mSuppSpeed, 0, mConnType, 0}, \
                                         {mMdioReset, 0, mMdioGpioResetNum, 0, mSfpGpioAdd, 0, mTxFault, mRs, mModAbs, mRxLoss, mSfpGpioMask}, \
                                         {mSfpMux, 0, mSfpBusSeg, 0, mSfpMuxUpAdd, 0, mRedriverAddress, mRedriverInterface, mRedriverLane, 0, mRedriverModel, mRedriverPresent}, \
                                         mPadMux0, mPadMux1, mMacAddressLo, mMacAddressHi, \
                                         {mTxEqPre, 0, mTxEqMain, 0, mTxEqPost, 0}},{{0},{0}}


#define  DXIO_PORT_DATA_INITIALIZER_ENET_V1(mPortPresent, mPortNum, mPlatConf, mMdioId, mSuppSpeed, mConnType, mMdioReset, mMdioGpioResetNum, \
                                         mSfpGpioAdd, mTxFault, mRs, mModAbs, mRxLoss, mSfpGpioMask, mSfpMux, mSfpBusSeg, \
                                         mSfpMuxUpAdd, mRedriverAddress, mRedriverInterface, mRedriverLane, mRedriverModel, \
                                         mRedriverPresent, mPadMux0, mPadMux1, mMacAddressLo, mMacAddressHi, \
                                         mTxEqPre,mTxEqMain, mTxEqPost, mTxEqApplicable,mTxEqOvrRide, mTxEqDefCtrl, \
                                         mRxEqApplicable, mCtleBoost0, mCtlePole0, mVga2Gain0, mVga1Gain0, mRxAdReq, mRxDfeByp0, mDfeTap1_0, mRx0EqAttLvl, \
                                         mBoostApplicable, mVboostEn0, mTx0Iboost, mTxVboostLvl) \
                                        {mPortPresent, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0, 0}, {0, 0, 0}, 0}, \
                                        {{mPortNum, mPlatConf, 0, mMdioId, 0, mSuppSpeed, 0, mConnType, 0}, \
                                         {mMdioReset, 0, mMdioGpioResetNum, 0, mSfpGpioAdd, 0, mTxFault, mRs, mModAbs, mRxLoss, mSfpGpioMask}, \
                                         {mSfpMux, 0, mSfpBusSeg, 0, mSfpMuxUpAdd, 0, mRedriverAddress, mRedriverInterface, mRedriverLane, 0, mRedriverModel, mRedriverPresent}, \
                                         mPadMux0, mPadMux1, mMacAddressLo, mMacAddressHi, \
                                         {mTxEqPre, 0, mTxEqMain, 0, mTxEqPost,0,mTxEqApplicable, mTxEqOvrRide, mTxEqDefCtrl}}, \
                                         {{mRxEqApplicable, mCtleBoost0, mCtlePole0, mVga2Gain0, mVga1Gain0, mRxAdReq, mRxDfeByp0, mDfeTap1_0, mRx0EqAttLvl, 0}, \
                                         {mBoostApplicable, mVboostEn0, mTx0Iboost, mTxVboostLvl, 0}}
#endif // _AGESA_H_


