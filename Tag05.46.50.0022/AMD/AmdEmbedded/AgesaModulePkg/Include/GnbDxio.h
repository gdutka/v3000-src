/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe component definitions.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */

#ifndef _GNBDXIO_H_
#define _GNBDXIO_H_

#pragma pack (push, 1)

#include <AGESA.h>
#include <Gnb.h>

#define MAX_NUMBER_OF_COMPLEXES                12

#define DESCRIPTOR_TERMINATE_GNB               0x40000000ull
#define DESCRIPTOR_TERMINATE_TOPOLOGY          0x20000000ull
#define DESCRIPTOR_ALLOCATED                   0x10000000ull
#define DESCRIPTOR_PLATFORM                    0x08000000ull
#define DESCRIPTOR_COMPLEX                     0x04000000ull
#define DESCRIPTOR_SILICON                     0x02000000ull
#define DESCRIPTOR_PCIE_WRAPPER                0x01000000ull
#define DESCRIPTOR_PCIE_ENGINE                 0x00800000ull
#define DESCRIPTOR_DDI_ENGINE                  0x00400000ull
#define DESCRIPTOR_CXL_ENGINE                  0x00200000ull

#define DESCRIPTOR_ALL_WRAPPERS                (DESCRIPTOR_PCIE_WRAPPER)
#define DESCRIPTOR_ALL_ENGINES                 (DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_CXL_ENGINE)

#define DESCRIPTOR_ALL_TYPES                   (DESCRIPTOR_ALL_WRAPPERS | DESCRIPTOR_ALL_ENGINES | DESCRIPTOR_SILICON | DESCRIPTOR_PLATFORM)

#define UNUSED_LANE_ID                         255

#define SILICON_NOT_PRESENT_IOHC               0x00000001ull
#define SILICON_NOT_PRESENT_SYSHUB             0x00000002ull
#define SILICON_NOT_PRESENT_NBIF0              0x00000004ull
#define SILICON_NOT_PRESENT_NBIF1              0x00000008ull
#define SILICON_NOT_PRESENT_NBIF2              0x00000010ull
#define SILICON_NOT_PRESENT_IOMMU              0x00000020ull
#define SILICON_NOT_PRESENT_L1IOAGR            0x00000040ull
#define SILICON_NOT_PRESENT_L1PCIE0            0x00000080ull
#define SILICON_NOT_PRESENT_L1PCIE1            0x00000100ull
#define SILICON_NOT_PRESENT_L1NBIF0            0x00000200ull
#define SILICON_NOT_PRESENT_NB                 0x00000400ull
#define SILICON_NOT_PRESENT_PCIE0              0x00000800ull
#define SILICON_NOT_PRESENT_PCIE1              0x00001000ull
#define SILICON_NOT_PRESENT_PCIE2              0x00002000ull
#define SILICON_NOT_PRESENT_SDPMUX             0x00004000ull
#define SILICON_NOT_PRESENT_SST                0x00008000ull


/// PCIe Link Training State
typedef enum {
  LinkStateResetAssert,                                 ///< Assert port GPIO reset
  LinkStateResetDuration,                               ///< Timeout for reset duration
  LinkStateResetExit,                                   ///< Deassert port GPIO reset
  LinkTrainingResetTimeout,                             ///< Port GPIO reset timeout
  LinkStateReleaseTraining,                             ///< Release link training
  LinkStateDetectPresence,                              ///< Detect device presence
  LinkStateDetecting,                                   ///< Detect link training.
  LinkStateBrokenLane,                                  ///< Check and handle broken lane
  LinkStateGen2Fail,                                    ///< Check and handle device that fail training if GEN2 capability advertised
  LinkStateL0,                                          ///< Device trained to L0
  LinkStateVcoNegotiation,                              ///< Check VCO negotiation complete
  LinkStateRetrain,                                     ///< Force retrain link.
  LinkStateTrainingFail,                                ///< Link training fail
  LinkStateTrainingSuccess,                             ///< Link training success
  LinkStateGfxWorkaround,                               ///< GFX workaround
  LinkStateCompliance,                                  ///< Link in compliance mode
  LinkStateDeviceNotPresent,                            ///< Link is not connected
  LinkStateTrainingCompleted                            ///< Link training completed
} PCIE_LINK_TRAINING_STATE;

/// PCIe PSPP Power policy
typedef enum  {
  AgesaPsppDisabled,                                           ///< PSPP disabled
  AgesaPsppPerformance = 1,                                    ///< Performance
  AgesaPsppBalance,                                            ///< Balance
  AgesaPsppPowerSaving,                                        ///< Power Saving
  AgesaMaxPspp                                                 ///< Max Pspp for boundary check
} PCIE_PSPP_POLICY;


#define INIT_STATUS_PCIE_PORT_GEN2_RECOVERY          0x00000001ull
#define INIT_STATUS_PCIE_PORT_BROKEN_LANE_RECOVERY   0x00000002ull
#define INIT_STATUS_PCIE_PORT_TRAINING_FAIL          0x00000004ull
#define INIT_STATUS_PCIE_TRAINING_SUCCESS            0x00000008ull
#define INIT_STATUS_PCIE_EP_NOT_PRESENT              0x00000010ull
#define INIT_STATUS_PCIE_PORT_IN_COMPLIANCE          0x00000020ull
#define INIT_STATUS_DDI_ACTIVE                       0x00000040ull
#define INIT_STATUS_ALLOCATED                        0x00000080ull
#define INIT_STATUS_PCIE_PORT_BROKEN_LANE_X1         0x00000100ull
#define INIT_STATUS_PCIE_PORT_ALWAYS_EXPOSE          0x00000200ull

#define PcieLibIsEngineAllocated(Descriptor) ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_ALLOCATED) != 0) : FALSE)
// Check if descriptor a PCIe engine
#define PcieLibIsPcieEngine(Descriptor) ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_PCIE_ENGINE) != 0) : FALSE)
// Check if descriptor a PCIe wrapper
#define PcieLibGetNextDescriptor(Descriptor) ((Descriptor == NULL) ? NULL : ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : (Descriptor + 1))

// PCIe functions to avoid hardcoding these values where they are used
#define PCIE_FUNCTION0  0
#define PCIE_FUNCTION1  1
#define PCIE_FUNCTION2  2
#define PCIE_FUNCTION3  3
#define PCIE_FUNCTION4  4
#define PCIE_FUNCTION5  5
#define PCIE_FUNCTION6  6
#define PCIE_FUNCTION7  7

/// PCIe port State
typedef enum {
  UnhidePorts,                                          ///< Command to unhide port
  HidePorts,                                            ///< Command to hide unused ports
} PCIE_PORT_VISIBILITY;

/// Table Register Entry
typedef struct {
  UINT16          Reg;                                  ///< Address
  UINT32          Mask;                                 ///< Mask
  UINT32          Data;                                 ///< Data
} PCIE_PORT_REGISTER_ENTRY;

/// Table Register Entry
typedef struct {
  PCIE_PORT_REGISTER_ENTRY  *Table;                     ///< Table
  UINT32                    Length;                     ///< Length
} PCIE_PORT_REGISTER_TABLE_HEADER;

/// Table Register Entry
typedef struct {
  UINT32          Reg;                                  ///< Address
  UINT32          Mask;                                 ///< Mask
  UINT32          Data;                                 ///< Data
} PCIE_HOST_REGISTER_ENTRY;

/// Table Register Entry
typedef struct {
  PCIE_HOST_REGISTER_ENTRY  *Table;                     ///< Table
  UINT32                    Length;                     ///< Length
} PCIE_HOST_REGISTER_TABLE_HEADER;

///Link ASPM info
typedef struct {
  PCI_ADDR        DownstreamPort;                       ///< PCI address of downstream port
  PCIE_ASPM_TYPE  DownstreamAspm;                       ///< Downstream Device Aspm
  PCI_ADDR        UpstreamPort;                         ///< PCI address of upstream port
  PCIE_ASPM_TYPE  UpstreamAspm;                         ///< Upstream Device Capability
  PCIE_ASPM_TYPE  RequestedAspm;                        ///< Requested ASPM
  BOOLEAN         BlackList;                            ///< Blacklist device
} PCIe_LINK_ASPM;

///PCIe ASPM Latency Information
typedef struct {
  UINT8  MaxL0sExitLatency;                             ///< Max L0s exit latency in us
  UINT8  MaxL1ExitLatency;                              ///< Max L1 exit latency in us
} PCIe_ASPM_LATENCY_INFO;

/// PCI address association
typedef struct {
  UINT8 NewDeviceAddress;                                ///< New PCI address (Device,Fucntion)
  UINT8 NativeDeviceAddress;                             ///< Native PCI address (Device,Fucntion)
} PCI_ADDR_LIST;

typedef UINT16 PCIe_ENGINE_INIT_STATUS;

/// Engine Configuration
typedef struct {
  IN       UINT8                EngineType;               /**< Engine type
                                                           *  @li @b 0 -  Ignore engine configuration
                                                           *  @li @b 1 -  PCIe port
                                                           *  @li @b 2 -  DDI
                                                           */
  IN       UINT16               StartLane;                /**< Start Lane ID (in reversed configuration StartLane > EndLane)
                                                           * Refer to lane descriptions and supported configurations in BKDG
                                                           */
  IN       UINT16               EndLane;                  /**< End lane ID (in reversed configuration StartLane > EndLane)
                                                           * Refer to lane descriptions and supported configurations in BKDG
                                                           */

} PCIe_ENGINE;

/// PCIe port misc extended controls
typedef struct  {
  IN      UINT8                     SbLink             :1;  /**< PCIe link type
                                                             *  @li @b 0 - General purpose port
                                                             *  @li @b 1 - Port connected to SB
                                                             */
  IN      UINT8                     ClkPmSupport       :1;  /**< Clock Power Management Support
                                                             *  @li @b 0 - Clock Power Management not configured
                                                             *  @li @b 1 - Clock Power Management configured according to PCIe device capability
                                                             */
  IN      UINT8                     CsLink             :1;  /**< PCIe link type
                                                             *  @li @b 0 - General purpose port
                                                             *  @li @b 1 - Port connected to chipset
                                                             */
  IN      UINT8                     Reserved0          :5;  ///< Unused space
} PORT_MISC_CONTROL;



/// PCIe port configuration data
typedef struct  {
  IN       UINT8                   PortPresent;              ///< Enable PCIe port for initialization.
  IN       UINT8                   FunctionNumber       :3;  ///< Reserved for future use
  IN       UINT8                   DeviceNumber         :5;   /**< PCI Device number for port.
                                                               *   @li @b 0 - Native port device number
                                                               *   @li @b N - Port device number (See available configurations in BKDG
                                                               */
  IN       UINT8                   LinkSpeedCapability  :4;   /**< PCIe link speed/
                                                               *  @li @b 0 - Maximum supported by silicon
                                                               *  @li @b 1 - Gen1
                                                               *  @li @b 2 - Gen2
                                                               *  @li @b 3 - Gen3
                                                               */
  IN       UINT8                   LinkAspm             :4;   /**< ASPM control. (see AgesaPcieLinkAspm for additional option to control ASPM)
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - L0s only
                                                               *  @li @b 2 - L1 only
                                                               *  @li @b 3 - L0s and L1
                                                               */
  IN       UINT8                   LinkHotplug;               /**< Hotplug control.
                                                               *  @li @b 0 - Disabled
                                                               *  @li @b 1 - Basic
                                                               *  @li @b 2 - Server
                                                               *  @li @b 3 - Enhanced
                                                               */
  IN       UINT16                  SlotNum;                  ///< Physical Slot Number
  IN       PORT_MISC_CONTROL       MiscControls;             ///< Misc extended controls
  IN       UINT8                   Reserved1;                ///< Reserved for future use
} PORT_DATA;

typedef struct {
    UINT8                LinkSpeed;
    UINT8                MaxPayloadSupport;
    UINT8                AspmCapability;
    UINT8                PciPmL1_1;
    UINT8                PciPmL1_2;
    UINT8                AspmL1_1;
    UINT8                AspmL1_2;
    UINT8                EsmSupport;
    UINT8                LtrSupport;
    UINT8                SurpriseDownErrorReport;
    UINT8                TenBitTagSupport;
    UINT8                AriForwarding;
    UINT8                AcsSupport;
    UINT8                AcsSourceValidation;
    UINT8                AcsTranslationBlocking;
    UINT8                AcsP2pRequestRedirect;
    UINT8                AcsP2pCompletionRedirect;
    UINT8                AcsUpstreamForwarding;
    UINT8                AcsP2pEgressControl;
    UINT8                AcsDirectTranslatedP2p;
    UINT8                LaneMargining;
    UINT8                DataLinkFeature;
    UINT8                DownstreamPortContainment;
    UINT8                AdvancedErrorReporting;
    UINT8                ECRCSupport;
    UINT8                MulticastEnable;
    UINT8                NativePCIeEnclosureManagement;
    UINT8                Capability1Address;
    UINT8                Capability1Data;
    UINT8                Capability2Address;
    UINT8                Capability2Data;
    UINT8                Capability3Address;
    UINT8                Capability3Data;
    UINT8                Capability4Address;
    UINT8                Capability4Data;
} PORT_CAPABILITIES;

typedef struct {
    UINT8                LinkSpeedControl;
    UINT8                MaxPayloadSizeControl;
    UINT8                ESMControl;
    UINT8                LTRControl;
    UINT8                DataLinkFeatureExchangeControl;
    UINT8                TenBitTagControl;
    UINT8                ARIControl;
    UINT8                ACSControl;
    UINT8                RxLaneMarginingControl;
    UINT8                DynLanesPwrState;
    UINT8                L1PowerDown;
    UINT8                L11PowerDown;
    UINT8                L12PowerDown;
    UINT8                AutoSpdChngEn;
    UINT8                TurnOffUnusedLanes;
} PORT_FEATURES;

/// PCIe port configuration info
typedef struct {
  PORT_DATA               PortData;                     ///< Port data
  UINT8                   StartCoreLane;                ///< Start Core Lane
  UINT8                   EndCoreLane;                  ///< End Core lane
  UINT8                   NativeDevNumber :5;           ///< Native PCI device number of the port
  UINT8                   NativeFunNumber :3;           ///< Native PCI function number of the port
  UINT8                   CoreId :4;                    ///< PCIe core ID
  UINT8                   PortId :4;                    ///< Port ID on wrapper
  PCI_ADDR                Address;                      ///< PCI address of the port
  UINT8                   PcieBridgeId:7;               ///< IOC PCIe bridge ID
  UINT8                   ClkReqFilterEn:1;             ///< Controls filtering of CLKREQb signal in LC in order to avoid false L1 substate entries/exits.
  UINT8                   LogicalBridgeId;              ///< Logical Bridge ID
  UINT8                   SlotPowerLimit;               ///< Slot Power Limit
  UINT8                   MaxPayloadSize;               ///< Max_Payload_Size

  UINT8                   TXDeEmphasis:4;               ///< TX De-emphasis
  UINT8                   TXMargin:3;                   ///< TX Margin
  UINT8                   UNUSED1:1;                    ///< Currently unassigned - for alignment

  UINT8                   EqSearchMode:2;               ///< Equalization Search Mode
  UINT8                   BypassGen3EQ:1;               ///< BypassGen3EQ
  UINT8                   DisGen3EQPhase:1;             ///< Disable Gen3 EQ Phase2/3
  UINT8                   Gen3FixedPreset:4;            ///< Gen3 Fixed Preset value

  UINT8                   EqSearchModeGen4:2;           ///< Equalization Search Mode for Gen4
  UINT8                   BypassGen4EQ:1;               ///< Gen4 Bypass phase3 EQ
  UINT8                   DisGen4EQPhase:1;             ///< Gen4 Bypass phase2/3 EQ
  UINT8                   Gen4FixedPreset:4;            ///< Gen4 Fixed Preset value
  UINT8                   EqSearchModeGen5:2;           ///< Equalization Search Mode for Gen5
  UINT8                   BypassGen5EQ:1;               ///< Gen5 Bypass phase3 EQ
  UINT8                   DisGen5EQPhase:1;             ///< Gen5 Bypass phase2/3 EQ
  UINT8                   Gen5FixedPreset:4;            ///< Gen5 Fixed Preset value

  UINT8                   ClkReq:4;                     ///< ClkReq:[0:3]
  UINT8                   EqPreset:4;                   ///< EqPreset:[4:7]

  struct {
    UINT8                 SpcGen1:1;                    ///< SPC Mode 2P5GT
    UINT8                 SpcGen2:1;                    ///< SPC Mode 5GT
    UINT8                 SpcGen3:2;                    ///< SPC Mode 8GT
    UINT8                 SpcGen4:2;                    ///< SPC Mode 16GT
    UINT8                 SpcGen5:2;                    ///< SPC Mode 32GT
  } SpcMode;
  struct {
    UINT16                DsTxPreset:4;                 ///< Gen3 Downstream Tx Preset
    UINT16                DsRxPresetHint:3;             ///< Gen3 Downstream Rx Preset Hint
    UINT16                UsTxPreset:4;                 ///< Gen3 Upstream Tx Preset
    UINT16                UsRxPresetHint:3;             ///< Gen3 Upstream Rx Preset Hint
  } LaneEqualizationCntl;
  struct {
    UINT8                 DsTxPreset:4;                 ///< Gen4 Downstream Tx Preset
    UINT8                 UsTxPreset:4;                 ///< Gen4 Upstream Tx Preset
  } Gen4LaneEqualizationCntl;
  struct {
    UINT8                 DsTxPreset:4;                 ///< Gen5 Downstream Tx Preset
    UINT8                 UsTxPreset:4;                 ///< Gen5 Upstream Tx Preset
  } Gen5LaneEqualizationCntl;
  UINT8                   SrisEnableMode:4;             ///< 0:Disable 1:SRIS 2:SRNS 3:SRNS in SRIS 0xF:Auto
  UINT8                   SRIS_SRNS:1;                  ///< SRIS SRNS
  UINT8                   SRIS_LowerSKPSupport:1;       ///< SRIS Lower SKP Support
  UINT8                   EsmControl:1;                 ///< Bit to enable/disable ESM
  UINT8                   CcixControl:1;                ///< Bit to enable/disable ESM
  UINT8                   LowerSkpOsGenSup;             ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  UINT8                   LowerSkpOsRcvSup;             ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  UINT8                   CxlControl:1;                 ///< Bit to enable CXL Capability
  UINT8                   AlwaysExpose:1;               ///< Always expose unused PCIE port
  UINT8                   SrisSkipInterval:2;           ///< Controls SRIS SKP generation interval
  UINT8                   SrisAutoDetectMode:4;         ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  UINT8                   SrisSkpIntervalSel:2;         ///< Controls SRIS SKIP Interval Selection Mode
  UINT8                   SrisAutodetectFactor:2;       ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  UINT8                   LinkAspmL1_1:1;               ///< Enable PM L1 SS L1.1
  UINT8                   LinkAspmL1_2:1;               ///< Enable PM L1 SS L1.2
  UINT8                   SlotPowerLimitScale:2;        ///< Slot Power Limit Scale

  UINT8                   RxMarginPersistence:1;        ///< Bit to enable/disable Rx Margin persistence mode
  UINT8                   SetGen3FixedPreset:1;         ///< Gen3 Fixed Preset Set
  UINT8                   SetGen4FixedPreset:1;         ///< Gen4 Fixed Preset Set
  UINT8                   SetGen5FixedPreset:1;         ///< Gen5 Fixed Preset Set
  UINT8                   TxVetting:1;                  ///< Gen4 Tx Vetting
  UINT8                   RxVetting:1;                  ///< Gen4 Rx Vetting
  UINT8                   TxVettingGen5:1;              ///< Gen5 Tx Vetting
  UINT8                   RxVettingGen5:1;              ///< Gen5 Rx Vetting

  UINT8                   IsMasterPLL:1;                ///< IsMasterPLL
  UINT8                   TargetLinkSpeed:3;            ///< Target Link Speed
  UINT8                   DlfCapDisable:1;              ///< DLF Capability 1:Disable 0:Enable
  UINT8                   DlfExchangeDisable:1;         ///< DLF Exchange 1:Disable 0:Enable
  UINT8                   InvertPolarity:1;             ///< Invert RX Polarity
  UINT8                   InvertPolarity2:1;            ///< Invert TX Polarity

  UINT8                   EsmSpeedBump;                 ///< Speed bump for ESM
  UINT16                  PsppPolicyDC;                 ///< Pspp Policy DC
  UINT16                  PsppPolicyAC;                 ///< Pspp Policy AC
  UINT8                   PsppDeviceType;               ///< Pspp Device Type
  UINT8                   I2CMuxInfo;                   ///< First I2c Mux on Bus
  PORT_CAPABILITIES       PortCapabilities;             ///< Port Capabilities CBS
  PORT_FEATURES           PortFeatures;                 ///< Port Features CBS
} PCIe_PORT_CONFIG;


/// CXL port configuration info
typedef struct {
  PORT_DATA               PortData;                     ///< Port data
  UINT8                   StartCoreLane;                ///< Start Core Lane
  UINT8                   EndCoreLane;                  ///< End Core lane
  UINT8                   NativeDevNumber :5;           ///< Native PCI device number of the port
  UINT8                   NativeFunNumber :3;           ///< Native PCI function number of the port
  UINT8                   CoreId :4;                    ///< PCIe core ID
  UINT8                   PortId :4;                    ///< Port ID on wrapper
  PCI_ADDR                Address;                      ///< PCI address of the port
  UINT8                   PcieBridgeId:7;               ///< IOC PCIe bridge ID
  UINT8                   UNUSED0:1;                    ///< Currently unassigned - for alignment
  UINT8                   LogicalBridgeId;              ///< Logical Bridge ID
  UINT8                   SlotPowerLimit;               ///< Slot Power Limit
  UINT8                   MaxPayloadSize;               ///< Max_Payload_Size

  UINT8                   UnusedTXDeEmphasis:4;         ///< TX De-emphasis
  UINT8                   UnusedTXMargin:3;             ///< TX Margin
  UINT8                   UnusedUNUSED1:1;              ///< Currently unassigned - for alignment

  UINT8                   UnusedEqSearchMode:2;         ///< Equalization Search Mode
  UINT8                   UnusedBypassGen3EQ:1;         ///< BypassGen3EQ
  UINT8                   UnusedDisGen3EQPhase:1;       ///< Disable Gen3 EQ Phase2/3
  UINT8                   UnusedGen3FixedPreset:4;      ///< Gen3 Fixed Preset value

  UINT8                   UnusedEqSearchModeGen4:2;     ///< Equalization Search Mode for Gen4
  UINT8                   UnusedBypassGen4EQ:1;         ///< Gen4 Bypass phase3 EQ
  UINT8                   UnusedDisGen4EQPhase:1;       ///< Gen4 Bypass phase2/3 EQ
  UINT8                   UnusedGen4FixedPreset:4;      ///< Gen4 Fixed Preset value
  UINT8                   UnusedEqSearchModeGen5:2;     ///< Equalization Search Mode for Gen5
  UINT8                   UnusedBypassGen5EQ:1;         ///< Gen5 Bypass phase3 EQ
  UINT8                   UnusedDisGen5EQPhase:1;       ///< Gen5 Bypass phase2/3 EQ
  UINT8                   UnusedGen5FixedPreset:4;      ///< Gen5 Fixed Preset value

  UINT8                   UnusedClkReq:4;               ///< ClkReq:[0:3]
  UINT8                   UnusedEqPreset:4;             ///< EqPreset:[4:7]

  struct {
    UINT8                 SpcGen1:1;                    ///< SPC Mode 2P5GT
    UINT8                 SpcGen2:1;                    ///< SPC Mode 5GT
    UINT8                 SpcGen3:2;                    ///< SPC Mode 8GT
    UINT8                 SpcGen4:2;                    ///< SPC Mode 16GT
    UINT8                 SpcGen5:2;                    ///< SPC Mode 32GT
  } SpcMode;
  struct {
    UINT16                UnusedDsTxPreset:4;           ///< Gen3 Downstream Tx Preset
    UINT16                UnusedDsRxPresetHint:3;       ///< Gen3 Downstream Rx Preset Hint
    UINT16                UnusedUsTxPreset:4;           ///< Gen3 Upstream Tx Preset
    UINT16                UnusedUsRxPresetHint:3;       ///< Gen3 Upstream Rx Preset Hint
  } LaneEqualizationCntl;
  struct {
    UINT8                 UnusedDsTxPreset:4;           ///< Gen4 Downstream Tx Preset
    UINT8                 UnusedUsTxPreset:4;           ///< Gen4 Upstream Tx Preset
  } Gen4LaneEqualizationCntl;
  struct {
    UINT8                 UnusedDsTxPreset:4;           ///< Gen5 Downstream Tx Preset
    UINT8                 UnusedUsTxPreset:4;           ///< Gen5 Upstream Tx Preset
  } Gen5LaneEqualizationCntl;
  UINT8                   SrisEnableMode:4;             ///< 0:Disable 1:SRIS 2:SRNS 3:SRNS in SRIS 0xF:Auto
  UINT8                   SRIS_SRNS:1;                  ///< SRIS SRNS
  UINT8                   SRIS_LowerSKPSupport:1;       ///< SRIS Lower SKP Support
  UINT8                   EsmControl:1;                 ///< Bit to enable/disable ESM
  UINT8                   CcixControl:1;                ///< Bit to enable/disable ESM
  UINT8                   LowerSkpOsGenSup;             ///< Controls LOWER_SKP_OS_GEN_SUPPORT
  UINT8                   LowerSkpOsRcvSup;             ///< Controls LOWER_SKP_OS_RCV_SUPPORT
  UINT8                   UNUSED3:2;                    ///< Currently Unassigned for alignment
  UINT8                   SrisSkipInterval:2;           ///< Controls SRIS SKP generation interval
  UINT8                   SrisAutoDetectMode:4;         ///< Controls SRIS Autodetect mode 0:Disable 1:Enable 0xF:Auto
  UINT8                   SrisSkpIntervalSel:2;         ///< Controls SRIS SKIP Interval Selection Mode
  UINT8                   SrisAutodetectFactor:2;       ///< Controls the multiplier for SKP ordered set interval when generated based on elasticity buffer pointer slip feedback from PCS
  UINT8                   LinkAspmL1_1:1;               ///< Enable PM L1 SS L1.1
  UINT8                   LinkAspmL1_2:1;               ///< Enable PM L1 SS L1.2
  UINT8                   SlotPowerLimitScale:2;        ///< Slot Power Limit Scale

  UINT8                   RxMarginPersistence:1;        ///< Bit to enable/disable Rx Margin persistence mode
  UINT8                   SetGen3FixedPreset:1;         ///< Gen3 Fixed Preset Set
  UINT8                   SetGen4FixedPreset:1;         ///< Gen4 Fixed Preset Set
  UINT8                   SetGen5FixedPreset:1;         ///< Gen5 Fixed Preset Set
  UINT8                   TxVetting:1;                  ///< Gen4 Tx Vetting
  UINT8                   RxVetting:1;                  ///< Gen4 Rx Vetting
  UINT8                   TxVettingGen5:1;              ///< Gen5 Tx Vetting
  UINT8                   RxVettingGen5:1;              ///< Gen5 Rx Vetting

  UINT8                   IsMasterPLL:1;                ///< IsMasterPLL
  UINT8                   TargetLinkSpeed:3;            ///< Target Link Speed
  UINT8                   DlfCapDisable:1;              ///< DLF Capability 1:Disable 0:Enable
  UINT8                   DlfExchangeDisable:1;         ///< DLF Exchange 1:Disable 0:Enable
  UINT8                   UnusedInvertPolarity:1;       ///< Invert RX Polarity
  UINT8                   UnusedInvertPolarity2:1;      ///< Invert TX Polarity

  UINT8                   EsmSpeedBump;                 ///< Speed bump for ESM
  UINT32                  UsRcrb;                       ///< Upstream Port RCRB address
  UINT32                  DsRcrb;                       ///< Downstream Port RCRB address
  UINT32                  UsMemBar0;                    ///< Upstream port MEMBAR0
  UINT32                  DsMemBar0;                    ///< Downstream port MEMBAR0
} PCIe_CXL_CONFIG;


///Descriptor header
typedef struct {
  UINT32                  DescriptorFlags;              ///< Descriptor flags
  UINT16                  Parent;                       ///< Offset of parent descriptor
  UINT16                  Peer;                         ///< Offset of the peer descriptor
  UINT16                  Child;                        ///< Offset of the list of child descriptors
} PCIe_DESCRIPTOR_HEADER;

/// DDI (Digital Display Interface) configuration info
typedef struct {
  PCIe_DDI_DATA           DdiData;                      ///< DDI Data
  UINT8                   DisplayPriorityIndex;         ///< Display priority index
  UINT8                   ConnectorId;                  ///< Connector id determined by enumeration
  UINT8                   DisplayDeviceId;              ///< Display device id determined by enumeration
} PCIe_DDI_CONFIG;

/// Engine configuration data
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor header
  PCIe_ENGINE             EngineData;                   ///< Engine Data
  PCIe_ENGINE_INIT_STATUS InitStatus;                   ///< Initialization Status
  UINT8                   Scratch;                      ///< Scratch pad
  union {
    PCIe_PORT_CONFIG      Port;                         ///< PCIe port configuration data
    PCIe_DDI_CONFIG       Ddi;                          ///< DDI configuration data
    PCIe_CXL_CONFIG       Cxl;                          ///< CXL Configuration data
  } Type;
} PCIe_ENGINE_CONFIG;

/// Wrapper configuration data
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  UINT8                   WrapId;                       ///< Wrapper ID
  UINT8                   CcixCoreConfig;               ///< Ccix CORE Configuration
  UINT8                   StartPhyLane;                 ///< Start PHY Lane
  UINT8                   EndPhyLane;                   ///< End PHY Lane
  UINT8                   StartDxioLane;                ///< Start Dxio Lane (Translated)
  UINT8                   EndDxioLane;                  ///< End Dxio Lane (Translated)
  struct {
    UINT8                 PowerOffUnusedLanes:1;        ///< Power Off unused lanes
    UINT8                 PowerOffUnusedPlls:1;         ///< Power Off unused Plls
    UINT8                 ClkGating:1;                  ///< TXCLK gating
    UINT8                 LclkGating:1;                 ///< LCLK gating
    UINT8                 TxclkGatingPllPowerDown:1;    ///< TXCLK clock gating PLL power down
    UINT8                 PllOffInL1:1;                 ///< PLL off in L1
    UINT8                 AccessEncoding:1;             ///< Reg access encoding
    UINT8                 CoreReversed:1;               ///< Indicates lanes are reversed in package connection
  } Features;
  UINT8                   MasterPll;                    ///< Bitmap of master PLL
  UINT32                   AcsSupport:1;                    ///< Acs Support
  UINT32                   LtrSupport:1;                    ///< LTR Support
  UINT32                   AriForwarding:1;                 ///< ARI Forwarding
  UINT32                   LaneMargining:1;                 ///< Lane Margining
  UINT32                   NativePCIeEnclosureManagement:1; ///< NPEM
  UINT32                   DownstreamPortContainment:1;     ///< Downstream port containment
  UINT32                   AdvancedErrorReporting:1;        ///< Advacned Error Reporting
  UINT32                   ECRCSupport:2;                   ///< ECRC Capability
  UINT32                   Reserved:23;                     ///< Reserved bits
} PCIe_WRAPPER_CONFIG;


/// Silicon configuration data
typedef struct  {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  UINT8                   SocketId;                     ///< Socket ID
  UINT8                   DieNumber;                    ///< Module ID
  UINT8                   RBIndex;                      ///< Physical Root Bridge
  UINT8                   InstanceId;                   ///< Logical Instance Identifier
  PCI_ADDR                Address;                      ///< PCI address of GNB host bridge
  UINT16                  StartLane;                    ///< Start Lane of this node
  UINT16                  EndLane;                      ///< End Lane of this node
  UINT8                   BusNumberLimit;               ///< Last Bus Number assigned to this node
  UINT8                   SbPresent:1;                  ///< Set to 1 if FCH connected to this NBIO
  UINT8                   SmuPresent:1;                 ///< Set to 1 if SMU connected to this NBIO
  UINT8                   Reserved1:6;                  ///< Reserved
  UINT8                   LogicalRBIndex;               ///< Logical Root Bridge
  UINT8                   NumEngineDesc;                ///< Total number of lane bifurcation descriptors
} PCIe_SILICON_CONFIG;

typedef PCIe_SILICON_CONFIG GNB_HANDLE;

/// Complex configuration data
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  UINT8                   NodeId;                       ///< Processor Node ID
  UINT8                   Reserved;                     ///< For alignment
} PCIe_COMPLEX_CONFIG;

/// PCIe platform configuration info
typedef struct {
  PCIe_DESCRIPTOR_HEADER  Header;                       ///< Descriptor Header
  PVOID                   StdHeader;                    ///< Standard configuration header
  UINT32                  LinkReceiverDetectionPooling; ///< Receiver pooling detection time in us.
  UINT32                  PhyConfigData;                ///< Phy Configuration Data
  UINT32                  LinkGpioResetAssertionTime;   ///< Gpio reset assertion time in us
  UINT32                  LinkResetToTrainingTime;      ///< Time duration between deassert GPIO reset and release training in us                                                      ///
  UINT32                  PsppTuningParams;             ///< Tuning parameters for PSPP
  UINT32                  PsppTuningParams2;            ///< Tuning parameters 2 for PSPP
  UINT8                   GfxCardWorkaround;            ///< GFX Card Workaround
  UINT8                   PsppPolicy;                   ///< PSPP policy
  UINT8                   TrainingExitState;            ///< State at which training should exit (see PCIE_LINK_TRAINING_STATE)
  UINT8                   TrainingAlgorithm;            ///< Training algorithm (see PCIE_TRAINING_ALGORITHM)
  PCIe_COMPLEX_CONFIG     ComplexList[MAX_NUMBER_OF_COMPLEXES];  ///< Complex
} PCIe_PLATFORM_CONFIG;


/// PCIe Engine Description

typedef struct {
  UINT32                  Flags;                        /**< Descriptor flags
                                                         * @li @b Bit31 - last descriptor on wrapper
                                                         * @li @b Bit30 - Descriptor allocated for PCIe port or DDI
                                                         */
  PCIe_ENGINE             EngineData;                   ///< Engine Data
} PCIe_ENGINE_DESCRIPTOR;

/// PSPP_Policy_For_Each_Port
typedef union {
  struct {                                                             ///<
    UINT32   DC_MinPreferredSpeed:4;
    UINT32   Reserved1:1;
    UINT32   DC_MaxPreferredSpeed:4;
    UINT32   DC_DirectIndirect:1;
    UINT32   AC_MinPreferredSpeed:4;
    UINT32   Reserved2:1;
    UINT32   AC_MaxPreferredSpeed:4;
    UINT32   AC_DirectIndirect:1;
    UINT32   Reserved3:12;
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} PSPP_Policy_STRUCT;

#pragma pack (pop)

#endif


