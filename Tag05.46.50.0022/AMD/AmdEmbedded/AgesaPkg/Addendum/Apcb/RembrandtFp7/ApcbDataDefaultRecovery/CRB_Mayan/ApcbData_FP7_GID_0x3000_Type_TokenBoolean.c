/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <MiscMemDefines.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>
#include <ApcbV3TokenUid.h>


APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_TOKEN,
  APCB_TYPE_TOKEN_BOOLEAN,
  sizeof(ApcbTypeHeader),                       // SizeOfType, will be fixed up by tool
  0,                          // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_TOKEN,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_ASCENDING,
    APCB_TYPE_ATTR_UNITSIZE_TOKEN_V3,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,
    APCB_TYPE_ATTR_KEYSIZE_TOKEN_V3,
    APCB_TYPE_ATTR_KEYPOS_TOKEN_V3,
    BLDCFG_APCB_DATA_BOARD_MASK
  }
};  // SizeOfType will be fixed up by tool



APCB_TOKEN_PAIR_BOOL TokeList[] = {
// Two macros can be used for token definitions
//  APCB_TOKEN_BOOL (APCB_TOKEN_UID_NAME, APCB_TOKEN_UID_NAME_VALUE),
//  APCB_TOKEN_VAL_BOOL(APCB_TOKEN_UID_NAME), // Expand to use APCB_TOKEN_UID_NAME_VALUE

APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_PSP_TP_PORT),
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_PSP_ERROR_DISPLAY),
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_PSP_EVENT_LOG_DISPLAY),
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_PSP_STOP_ON_ERROR),
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMHOLEREMAPPING),                                                                   //< MemHoleRemapping - Memory Hole Remapping (1-bit).
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_LIMITMEMORYTOBELOW1TB),                                                              //< Limit memory address space to below 1 TB
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLECHIPSELECTINTLV),                                                              //< EnableChipSelectIntlv - Dram chip select Interleaving (1-bit).
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLECHANNELINTLV),                                                                 //< EnableChannelIntlv - Channel Interleaving (1-bit).
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEECCFEATURE),                                                                   //< EnableEccFeature - enable ECC error to go into MCE.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEPOWERDOWN),                                                                    //< EnablePowerDown - CKE based power down mode (1-bit).
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEPARITY),                                                                       //< EnableParity - Parity control.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEBANKSWIZZLE),                                                                  //< EnableBankSwizzle - BankSwizzle control.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEMEMCLR),                                                                       //< EnableMemClr - Memory Clear functionality control.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMRESTORECTL),                                                                      //< MemRestoreCtl - Memory context restore control
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ISCAPSULEMODE),                                                                      //< IsCapsuleMode - Capsule reboot control
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_AMPENABLE),                                                                          //< AmpEnable - Specify the average time between refresh requests to all DRAM devices.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATE),                                                              //< DramDoubleRefreshRate - PMU Training Mode
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ECCREDIRECTION),                                                                     //< CfgEccRedirection; ///< ECC Redirection.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ECCSYNCFLOOD),                                                                       //< CfgEccSyncFlood -  ECC Sync Flood.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_UECC_RETRY_DDR4),                                                                    //< CfgUEccRetry - Uncorrected ECC retry
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DQSTRAININGCONTROL),                                                                 //< CfgDqsTrainingControl -  Dqs Training Control.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_UMAABOVE4G),                                                                         //< CfgUmaAbove4G -  Uma Above 4G Support
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYALLCLOCKSON),                                                                  //< CfgMemoryAllClocksOn -  Memory All Clocks On.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_IGNORESPDCHECKSUM),                                                                  //< CfgIgnoreSpdChecksum -  Ignore Spd Checksum.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYMODEUNGANGED),                                                                 //< CfgMemoryModeUnganged -  Memory Mode Unganged.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYQUADRANKCAPABLE),                                                              //< CfgMemoryQuadRankCapable -  Memory Quad Rank Capable.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYRDIMMCAPABLE),                                                                 //< CfgMemoryRDimmCapable -  Memory RDIMM Capable.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYLRDIMMCAPABLE),                                                                //< CfgMemoryLRDimmCapable -  Memory LRDIMM Capable.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYUDIMMCAPABLE),                                                                 //< CfgMemoryUDimmCapable -  Memory UDIMM Capable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEMORYSODIMMCAPABLE),                                                                //< CfgMemorySODimmCapable - Memory SODimm Capable.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DRAMDOUBLEREFRESHRATEEN),                                                            //< CfgDramDoubleRefreshRateEn - Double DRAM refresh rate
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DIMMTYPEDDDR3CAPABLE),                                                               //< DimmTypeDddr3Capable -  Indicates that the system is DDR3 Capable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DIMMTYPELPDDDR3CAPABLE),                                                             //< DimmTypeLpDddr3Capable -  Indicates that the system is LPDDR3 Capable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ODTSCMDTHROTEN),                                                                     //< UMC::CH::ThrottleCtrl[OdtsCmdThrotEn]
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_SWCMDTHROTEN),                                                                       //< UMC::CH::ThrottleCtrl[SwCmdThrotEn]
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_FORCEPWRDOWNTHROTEN),                                                                //< UMC::CH::ThrottleCtrl[ForcePwrDownThrotEn]
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEMEMPSTATE),                                                                    //< Memory Pstate Enable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DDRROUTEBALANCEDTEE),                                                                //< CfgDdrRouteBalancedTee - Motherboard DDR Routing control.
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_REFRESH_EN),                                                     //< Dram Temperature Controlled Refresh Rate Enable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ENABLEBANKGROUPSWAPALT),                                                             //< Bank Group Swap Alt control
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEM_TEMP_CONTROLLED_EXTENDED_REFRESH_EN),                                            //< MR4[2] Temperature Controlled Refresh Range
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEM_MBIST_AGGR_STATIC_LANE_CTRL),                                                    //< Aggressor Static Lane Select Control
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEM_MBIST_TGT_STATIC_LANE_CTRL),                                                     //< Target Static Lane Select Control
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_POST_PACKAGE_REPAIR_ENABLE),                                                         //< Enable/Disable Post Package Repair
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_PCIE_RESET_CONTROL),                                                                 //< PCIe Reset Control
//APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_DISPLAY_PMU_TRAIN_RESULTS),                                                        //< Display PMU training results
//APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_GROUP_D_PLATFORM),                                                                   //< Group D platform selection
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEM_FORCE_DATA_MASK_DISABLE_DDR4),                                                   // Force Data Mask Disable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MEM_TSME_ENABLE),                                                                    // TSME enable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_ESPI_ABL_INIT_ENABLE),                                                               // Enable eSPI ABL init
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_GPU_TRANSLATION_CACHE),                                                              // Gpu Translation Cache Enable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_SMU_DPM_LEVEL_TRIMMING),                                                             // Trimming the p-state level to make P0=P1
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_SMU_DPM_MP3_DEGRADE),                                                                // Degrade MP3 frequency to 333/333/667
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_MODERN_STANDBY_ENABLE),                                                              // Sync with PcdModernStandbyEnable
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_WIRELESS_MANAGEABILITY),                                                             // Wireless Manageability sync with ABL
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY),                                                            // KVM Wired Manageability sync with ABL
APCB_TOKEN_VAL_BOOL (APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY),                                                         // KVM Wireless Manageability sync with ABL
//  APCB_TOKEN_BOOL (0x00000000, FALSE) // Null token, remove this when there's other token defined
};


APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

