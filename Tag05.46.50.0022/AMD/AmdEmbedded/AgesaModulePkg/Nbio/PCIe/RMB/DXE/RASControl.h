/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _RASCONTROL_H_
#define _RASCONTROL_H_

typedef union {
  struct {
    UINT32                                                   Count:8 ; ///<
    UINT32                                                 McgCtlP:1 ; ///<
    UINT64                                           Reserved_63_9:55; ///<
  } Field;                                                             ///<
  UINT64 Value;                                                        ///<
} MCG_CAP_STRUCT;


#define PCD_RASCONTROL_DISABLED     0  // Enabled FALSE
#define PCD_RASCONTROL_MCA          1  // Enabled TRUE
#define PCD_RASCONTROL_LEGACY       2  // Enabled TRUE

#define PCD_AERREPORTING_MCA        0
#define PCD_AERREPORTING_OS_FIRST   1
#define PCD_AERREPORTING_FFIRST     2

// --------------------------------------------------
// PPR 5.8.1.2 SRAM ECC and Parity Error Severity
// --------------------------------------------------
// Set correctable parity and ECC errors to correctable severity:
// IOHC::PARITY_SEVERITY_CONTROL_CORR_0=0x0
// Set uncorrectable parity and ECC errors to fatal severity:
// IOHC::PARITY_SEVERITY_CONTROL_UNCORR_0=0xAAAA_AAAA
// Set uncorrectable data errors converted to poisoned data to correctable severity.
// This is used for notification only and is separate from consuming poisoned data:
// IOHC::PARITY_SEVERITY_CONTROL_UCP_0=0x0
// Set ErrEvent to fatal severity:
// IOHC::MISC_SEVERITY_CONTROL[ErrEventErrSev]=0x2

#define SRAM_ECC_AND_PARITY_TBL \
  SMN_ENTRY_WR ( \
    SMN_PARITY_SEVERITY_CONTROL_CORR_0_ADDRESS, \
    0 \
   ), \
  SMN_ENTRY_WR ( \
    SMN_PARITY_SEVERITY_CONTROL_UNCORR_0_ADDRESS, \
    0xAAAAAAAA \
    ), \
  SMN_ENTRY_WR ( \
    SMN_PARITY_SEVERITY_CONTROL_UCP_0_ADDRESS, \
    0 \
    ), \
  SMN_ENTRY_RMW ( \
    SMN_MISC_SEVERITY_CONTROL_ADDRESS,  \
    MISC_SEVERITY_CONTROL_ErrEventErrSev_MASK , \
    2 << MISC_SEVERITY_CONTROL_ErrEventErrSev_OFFSET \
    ),
// End of SRAM_ECC_AND_PARITY_TBL


// ------------------------------------------------------------------------------------
// ErrEvent APML
// If the new PCD PcdSyncFloodToApml is true, set ErrEvent_ACTION_CONTROL[APML_Err_En].
// Else, PcdAmdNbioRASControlV2 should clear ErrEvent_ACTION_CONTROL[APML_Err_En].
// ------------------------------------------------------------------------------------
#define ERREVENT_APML_ERR_TBL \
  SMN_ENTRY_PROPERTY_RMW ( \
    PROPERTY_SYNC_FLOOD_TO_APML_ENABLED, \
    SMN_ErrEvent_ACTION_CONTROL_ADDRESS, \
    ErrEvent_ACTION_CONTROL_APML_ERR_En_MASK, \
    (0x1 << ErrEvent_ACTION_CONTROL_APML_ERR_En_OFFSET) \
    ), \
  SMN_ENTRY_PROPERTY_RMW ( \
    PROPERTY_SYNC_FLOOD_TO_APML_DISABLED, \
    SMN_ErrEvent_ACTION_CONTROL_ADDRESS, \
    ErrEvent_ACTION_CONTROL_APML_ERR_En_MASK, \
    (0x0 << ErrEvent_ACTION_CONTROL_APML_ERR_En_OFFSET) \
    ), \
  SMN_ENTRY_PROPERTY_RMW ( \
    PROPERTY_SYNC_FLOOD_TO_APML_ENABLED, \
    SMN_ParityFatal_ACTION_CONTROL_ADDRESS, \
    ParityFatal_ACTION_CONTROL_APML_ERR_En_MASK, \
    (0x1 << ParityFatal_ACTION_CONTROL_APML_ERR_En_OFFSET) \
    ), \
  SMN_ENTRY_PROPERTY_RMW ( \
    PROPERTY_SYNC_FLOOD_TO_APML_DISABLED, \
    SMN_ParityFatal_ACTION_CONTROL_ADDRESS, \
    ParityFatal_ACTION_CONTROL_APML_ERR_En_MASK, \
    (0x0 << ParityFatal_ACTION_CONTROL_APML_ERR_En_OFFSET) \
    ),


// ------------------------------------------------------------------------------------
// PPR 5.8.3 Data Poisoning
// -----------------------------------------------------------------------------------
// Always,
//   Sets POISON_ACTION_CONTROL[EgressPoisonHSAPMLErrEn, EgressPoisonHSLinkDisEn, EgressPoisonHSSyncFloodEn]=111b
//   Sets POISON_ACTION_CONTROL[IntPoisonSyncFloodEn, IntPoisonLinkDisEn, IntPoisonAPMLErrEn]=111b
// When PcdAmdNbioPoisonConsumption is TRUE,
//   Sets POISON_ACTION_CONTROL[EgressPoisonLSAPMLErrEn, EgressPoisonLSLinkDisEn, EgressPoisonLSSyncFloodEn]=111b
//   Sets EGRESS_POISON_MASK_{LO,HI} to all zeroes, overriding PcdAmdNbioEgressPoisonMaskLo/Hi
//   Sets EGRESS_POISON_SEVERITY{LO,HI} to 0x00000004 and 0x00030001
//     (ignoring  PcdEnableEgressPoisonSeverity, PcdEgressPoisonSeverityLo, and PcdEgressPoisonSeverityHi)
// When PcdAmdNbioPoisonConsumption is FALSE,
//   Enables PcdEnableEgressPoisonSeverity, PcdEgressPoisonSeverityLo, and PcdEgressPoisonSeverityHi
//   Enables PcdAmdNbioEgressPoisonMaskHi and PcdAmdNbioEgressPoisonMaskLo
// ------------------------------------------------------------------------------------
#define POISON_CONSUMPTION_TBL \
 SMN_ENTRY_RMW ( \
   SMN_POISON_ACTION_CONTROL_ADDRESS, \
   POISON_ACTION_CONTROL_EgressPoisonHSAPMLErrEn_MASK | \
   POISON_ACTION_CONTROL_EgressPoisonHSIntrGenSel_MASK | \
   POISON_ACTION_CONTROL_EgressPoisonHSLinkDisEn_MASK | \
   POISON_ACTION_CONTROL_EgressPoisonHSSyncFloodEn_MASK, \
   (0x1 << POISON_ACTION_CONTROL_EgressPoisonHSAPMLErrEn_OFFSET) | \
   (0x1 << POISON_ACTION_CONTROL_EgressPoisonHSLinkDisEn_OFFSET) | \
   (0x1 << POISON_ACTION_CONTROL_EgressPoisonHSSyncFloodEn_OFFSET) \
   ), \
  SMN_ENTRY_RMW ( \
    SMN_POISON_ACTION_CONTROL_ADDRESS, \
    POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_MASK | \
    POISON_ACTION_CONTROL_IntPoisonIntrGenSel_MASK | \
    POISON_ACTION_CONTROL_IntPoisonLinkDisEn_MASK | \
    POISON_ACTION_CONTROL_IntPoisonSyncFloodEn_MASK, \
    (0x1 << POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_OFFSET) | \
    (0x1 << POISON_ACTION_CONTROL_IntPoisonLinkDisEn_OFFSET) | \
    (0x1 << POISON_ACTION_CONTROL_IntPoisonSyncFloodEn_OFFSET) \
    ), \
  SMN_ENTRY_PROPERTY_RMW ( \
    PROPERTY_POISON_CONSUMPTION_ENABLED, \
    SMN_POISON_ACTION_CONTROL_ADDRESS, \
    POISON_ACTION_CONTROL_EgressPoisonLSAPMLErrEn_MASK | \
    POISON_ACTION_CONTROL_EgressPoisonLSIntrGenSel_MASK | \
    POISON_ACTION_CONTROL_EgressPoisonLSLinkDisEn_MASK | \
    POISON_ACTION_CONTROL_EgressPoisonLSSyncFloodEn_MASK, \
    (0x1 << POISON_ACTION_CONTROL_EgressPoisonLSAPMLErrEn_OFFSET) | \
    (0x0 << POISON_ACTION_CONTROL_EgressPoisonLSIntrGenSel_OFFSET) | \
    (0x1 << POISON_ACTION_CONTROL_EgressPoisonLSLinkDisEn_OFFSET) | \
    (0x1 << POISON_ACTION_CONTROL_EgressPoisonLSSyncFloodEn_OFFSET) \
    ), \
  SMN_ENTRY_PROPERTY_WR ( \
    PROPERTY_POISON_CONSUMPTION_ENABLED, \
    SMN_EGRESS_POISON_MASK_LO_ADDRESS, \
    0x00000000 \
    ), \
  SMN_ENTRY_PROPERTY_WR ( \
    PROPERTY_POISON_CONSUMPTION_ENABLED, \
    SMN_EGRESS_POISON_MASK_HI_ADDRESS, \
    0x00000000 \
    ), \
  SMN_ENTRY_PROPERTY_WR ( \
    PROPERTY_POISON_CONSUMPTION_ENABLED, \
    SMN_EGRESS_POISON_SEVERITY_LO_ADDRESS, \
    0x00000004 \
    ), \
  SMN_ENTRY_PROPERTY_WR ( \
    PROPERTY_POISON_CONSUMPTION_ENABLED, \
    SMN_EGRESS_POISON_SEVERITY_HI_ADDRESS, \
    0x00030000 \
    ),
 // End of NBIO Poison Consumption
 //


#endif
