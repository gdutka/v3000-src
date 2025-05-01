/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Library/BaseLib.h>
#include <GnbRegistersRMB.h>
#include <Library/NbioHandleLib.h>
#include <Library/SmnAccessLib.h>
#include "RASControl.h"
#include <Filecode.h>
#include <Library/SmnTableLib.h>
#include <Library/PcieConfigLib.h>

#define FILECODE NBIO_RMB_DXE_RASCONTROL_FILECODE
/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// Group types for use in the table below
#define UCP_GROUP_TYPE        0

// 0h - Value used for first poll
#define PARITY_CONTROL_INIT                         0x00
/// Encoding for Uncorrectable and Correctable Parity Wrappers:
#define PARITY_WRAPPER_WRITESIDE                1  // Error Injection on Write Side
#define PARITY_WRAPPER_READSIDE                 2  // Error Injection on Read Side
#define PARITY_WRAPPER_DISABLEREPORT            4  // Disable Error Reporting
#define PARITY_WRAPPER_ENABLEREPORT             5  // Enable Error Reporting
/// Encoding for ECC Parity Wrappers:
#define ECC_PARITY_WRAPPER_EN_ECC_DET_CORR      1  // Enable ECC Error Detection and Correction
#define ECC_PARITY_WRAPPER_SEC_INJ              2  // SEC Error Injection
#define ECC_PARITY_WRAPPER_DIS_SEC_REPORT       3  // Disable SEC Error Reporting
#define ECC_PARITY_WRAPPER_EN_SEC_REPORT        4  // Enable SEC Error Reporting
#define ECC_PARITY_WRAPPER_DED_INJ              5  // DED Error Injection
#define ECC_PARITY_WRAPPER_DIS_DED_REPORT       6  // Disable DED Error Reporting
#define ECC_PARITY_WRAPPER_EN_DED_REPORT        7  // Enable DED Error Reporting
#define ECC_PARITY_WRAPPER_DIS_DED_CONV         8  // Disable Conversion of DED Errors to Poisoned Data
#define ECC_PARITY_WRAPPER_EN_UCB_CONV          9  // Enable Conversion of DED Errors to Poisoned Data
#define ECC_PARITY_WRAPPER_UCP_INJ              10 // UCP Error Injection
#define ECC_PARITY_WRAPPER_DIS_UCP_REPORT       11 // Disable UCP Error Reporting
#define ECC_PARITY_WRAPPER_EN_UCP_REPORT        12 // Enable UCP Error Reporting

#define PROPERTY_DISABLE_RAS                           0x00000001ull
#define PROPERTY_ENABLE_RAS_MCA                        0x00000002ull
#define PROPERTY_SYNC_FLOOD_TO_APML_ENABLED            0x00000004ull
#define PROPERTY_SYNC_FLOOD_TO_APML_DISABLED           0x00000008ull
#define PROPERTY_POISON_CONSUMPTION_ENABLED            0x00000010ull

#define PROPERTY_PRESENT_IOHC                           0x00010000ull
#define PROPERTY_PRESENT_SYSHUB                         0x00020000ull
#define PROPERTY_PRESENT_NBIF0                          0x00040000ull
#define PROPERTY_PRESENT_NBIF1                          0x00080000ull
#define PROPERTY_PRESENT_NBIF2                          0x00100000ull
#define PROPERTY_PRESENT_IOMMU                          0x00200000ull
#define PROPERTY_PRESENT_L1IOAGR                        0x00400000ull
#define PROPERTY_PRESENT_L1PCIE0                        0x00800000ull
#define PROPERTY_PRESENT_L1PCIE2                        0x01000000ull
#define PROPERTY_PRESENT_L1PCIE3                        0x02000000ull

#define PROPERTY_DEFAULT_DEVICE_PRESENCE  PROPERTY_PRESENT_IOHC   | \
                                          PROPERTY_PRESENT_NBIF0  | \
                                          PROPERTY_PRESENT_L1IOAGR| \
                                          PROPERTY_PRESENT_L1PCIE0| \
                                          PROPERTY_PRESENT_L1PCIE2| \
                                          PROPERTY_PRESENT_L1PCIE3

// Different IP types for ECC and Parity Enablement Table
typedef enum {
  RASCONTROL_IP_L1IMUPCIE0,
  RASCONTROL_IP_L1IMUPCIE2,
  RASCONTROL_IP_L1IMUPCIE3,
  RASCONTROL_IP_L1IMUIOAGR,
  RASCONTROL_IP_IOHC,
  RASCONTROL_IP_NBIF
} RASCONTROL_IP_TYPE;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

typedef struct {
  UINT8        GroupType;       ///< Parity Group ID
  UINT8        GroupID;         ///< Group Type
  UINT8        StructureID;     ///< Structure ID
  UINT8        IpType;          ///< IP Type
} NBIO_PARITY_TABLE;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

SMN_TABLE ROMDATA NbioRasControlTableMain [] = {
  SRAM_ECC_AND_PARITY_TBL
  SMN_ENTRY_TERMINATE
};

SMN_TABLE ROMDATA NbioPoisonConsumptionTable [] = {
  POISON_CONSUMPTION_TBL                    // Configure poison consumption
  SMN_ENTRY_TERMINATE
};

SMN_TABLE ROMDATA NbioApmlErrTable [] = {
  ERREVENT_APML_ERR_TBL                     // Enable APML error reporting (supported through SMU)
  SMN_ENTRY_TERMINATE
};

///
/// PPR Reference: 5.8.1 SRAM ECC and Parity Enablement
///
NBIO_PARITY_TABLE mNbioParityTable[] = {
  {UCP_GROUP_TYPE, 0, 0, RASCONTROL_IP_L1IMUPCIE0},
  {UCP_GROUP_TYPE, 1, 0, RASCONTROL_IP_L1IMUPCIE2},
  {UCP_GROUP_TYPE, 2, 0, RASCONTROL_IP_L1IMUPCIE3},
  {UCP_GROUP_TYPE, 3, 0, RASCONTROL_IP_L1IMUIOAGR},
  {UCP_GROUP_TYPE, 4, 0, RASCONTROL_IP_IOHC},
  {UCP_GROUP_TYPE, 4, 1, RASCONTROL_IP_IOHC},
  {UCP_GROUP_TYPE, 5, 0, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 5, 1, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 5, 2, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 6, 0, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 6, 1, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 6, 2, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 6, 3, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 6, 4, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 6, 5, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 7, 0, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 7, 1, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 7, 2, RASCONTROL_IP_NBIF},
  {UCP_GROUP_TYPE, 7, 3, RASCONTROL_IP_NBIF}
 };

/*----------------------------------------------------------------------------------------*/
/**
 * Identify active PCIe core for NBIO RAS Control
 *
 *
 *
 * @param[in]  GnbHandle      GNB Handle
 * @param[in]  PcieCore       Pcie core number
 */

VOID
NbioRASIdentifyPcieCore (
    IN      GNB_HANDLE              *GnbHandle,
    IN      UINT8                   *PcieCore
  )
{
  PCIe_ENGINE_CONFIG             *PcieEngine;
  PCIe_WRAPPER_CONFIG            *PcieWrapper;

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);

  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);

    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine) &&
          PcieLibIsPcieEngine(PcieEngine) &&
          (PcieEngine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS))
      {
        *PcieCore |= (UINT8) (1 << PcieWrapper->WrapId);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * Program PARITY_CONTROL_1 feature
 *
 *
 * @param[in]  GnbHandle      GNB Handle
 * @param[in]  Pc1ToSet       Feature data
 * @param[in]  ErrGenCmd      Feature data
 */
VOID
PollAndSetNbioRasFeature (
  IN GNB_HANDLE                 *GnbHandle,
  IN PARITY_CONTROL_1_STRUCT    *Pc1ToSet,
  IN UINT8                      ErrGenCmd
  )
{
  PARITY_CONTROL_1_STRUCT EmptyPc1;

  EmptyPc1.Value = 0;

  if (ErrGenCmd == PARITY_CONTROL_INIT) {
    // Write 0 to register
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      SMN_PARITY_CONTROL_1_ADDRESS,
                      &(EmptyPc1.Value),
                      GNB_REG_ACC_FLAG_S3SAVE
                      );
    // Poll for ack
    do {
      SmnRegisterRead (GnbHandle->Address.Address.Bus,
                       SMN_PARITY_CONTROL_1_ADDRESS,
                       &(EmptyPc1.Value)
                       );
    } while (EmptyPc1.Field.ParityErrGenInjectAllow != 0x1);
  }
  else {
    ASSERT(Pc1ToSet != NULL);
    Pc1ToSet->Field.ParityErrGenTrigger = 0;
    Pc1ToSet->Field.ParityErrGenCmd = ErrGenCmd;
    // Write feature
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      SMN_PARITY_CONTROL_1_ADDRESS,
                      &(Pc1ToSet->Value),
                      GNB_REG_ACC_FLAG_S3SAVE
                      );
    // Set trigger bit
    Pc1ToSet->Field.ParityErrGenTrigger = 1;
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      NBIO_SPACE(GnbHandle, SMN_PARITY_CONTROL_1_ADDRESS), &(Pc1ToSet->Value),
                      GNB_REG_ACC_FLAG_S3SAVE
                      );
    // Poll for ack
    do {
      SmnRegisterRead (GnbHandle->Address.Address.Bus,
                       SMN_PARITY_CONTROL_1_ADDRESS,
                       &(Pc1ToSet->Value)
                       );
    } while (Pc1ToSet->Field.ParityErrGenInjectAllow != 0x1);

  }
}


/**
  This function clears the fields in PARITY_CONTROL_0_STRUCT
**/
VOID
ClearParityControl (
  IN GNB_HANDLE                 *GnbHandle
  )
{
    PARITY_CONTROL_0_STRUCT   ParityThresholdStruct;

    ParityThresholdStruct.Field.ParityUCPThreshold  = 0;
    ParityThresholdStruct.Field.ParityCorrThreshold = 0;

    SmnRegisterWrite (
    GnbHandle->Address.Address.Bus,
    SMN_PARITY_CONTROL_0_ADDRESS,
    &(ParityThresholdStruct.Value),
    GNB_REG_ACC_FLAG_S3SAVE
    );

    return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable NBIO RAS Control
 *
 * @param[in]  GnbHandle      GNB Handle
 */
VOID
NbioRASControl (
  IN GNB_HANDLE                 *GnbHandle
  )
{
  UINTN                       Index;
  PARITY_CONTROL_1_STRUCT     ParityControl1Data;
  UINT8                       PcieCore;
  UINT32                      Property;
  NBIF_SHUB_TODET_CTRL_STRUCT NbifTodetCtrlStruct;
  UINT32                      Value;
  UINT64                      UcpMask;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  ///
  /// Blast all tables first. Anything that does not fit easily in
  /// the tables will be coded below.
  ///
  Property = PROPERTY_DEFAULT_DEVICE_PRESENCE;
  IDS_HDT_CONSOLE (MAIN_FLOW, "Default properties for RB %d = 0x%X", GnbHandle->RBIndex, Property);
  Property &= ~(GnbHandle->Header.DescriptorFlags << 16);
  IDS_HDT_CONSOLE (MAIN_FLOW, "Properties for RB %d = 0x%X", GnbHandle->RBIndex, Property);
  if (PcdGet8 (PcdAmdNbioRASControlV2) == PCD_RASCONTROL_MCA) {
    Property |= PROPERTY_ENABLE_RAS_MCA;
  } else {
    Property |= PROPERTY_DISABLE_RAS;
  }
  if (PcdGetBool (PcdSyncFloodToApml)) {
    Property |= PROPERTY_SYNC_FLOOD_TO_APML_ENABLED;
  } else {
    Property |= PROPERTY_SYNC_FLOOD_TO_APML_DISABLED;
  }
  if (PcdGetBool(PcdAmdNbioPoisonConsumption)) {
    Property |= PROPERTY_POISON_CONSUMPTION_ENABLED;
  }

  NbioSmnTable (GnbHandle, NbioRasControlTableMain, NBIO_SPACE (GnbHandle, 0), Property, 0);

  ClearParityControl(GnbHandle);

  if (PcdGet8 (PcdAmdNbioRASControlV2) != PCD_RASCONTROL_DISABLED) {

    NbioSmnTable (GnbHandle, NbioPoisonConsumptionTable, NBIO_SPACE (GnbHandle, 0), Property, 0);

    if (!PcdGetBool(PcdAmdNbioPoisonConsumption)) {

      // Program EGRESS_POISON_MASK_LO[2] to 0.
      // Program the remaining bits of EGRESS_POISON_MASK_LO to the value of PcdAmdNbioEgressPoisonMaskLo
      Value = PcdGet32(PcdAmdNbioEgressPoisonMaskLo);
      Value &= ~(BIT2);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                        SMN_EGRESS_POISON_MASK_LO_ADDRESS,
                        &Value,
                        0
                       );
      // Program EGRESS_POISON_MASK_HI[17,16] to 0.
      // Program the remaining bits of EGRESS_POISON_MASK_HI to the value of PcdAmdNbioEgressPoisonMaskHi
      Value = PcdGet32(PcdAmdNbioEgressPoisonMaskHi);
      Value &= ~(BIT17 | BIT16);
      SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                        SMN_EGRESS_POISON_MASK_HI_ADDRESS,
                        &Value,
                        0
                       );
      // If Poison Severity is set then write the severity lo and hi values to the respective SMN register
      if (PcdGetBool (PcdEnableEgressPoisonSeverity)) {
        Value = PcdGet32(PcdEgressPoisonSeverityLo);
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          SMN_EGRESS_POISON_SEVERITY_LO_ADDRESS,
                          &Value,
                          0
                         );
        Value = PcdGet32(PcdEgressPoisonSeverityHi);
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                          SMN_EGRESS_POISON_SEVERITY_HI_ADDRESS,
                          &Value,
                          0
                         );
      }
    }

    //
    // Implement the NBIF Watchdog programming requirements from the NBIF MAS.
    // Sets up the NBIF Watchdog to sync flood on timed out transactions from: MP0, MP1, ATHUB by setting NBIF0 NBIFMM::NBIF_SHUB_TODET_SYNCFLOOD_CTRL to 0x25
    // Sets up the NBIF Watchdog to send an abort back to the originator on timed out transactions from: MP0, MP1 by setting NBIF0 NBIFMM::NBIF_SHUB_TODET_CLIENT_CTRL to 0x25
    // Sets the timeout interval by programming NBIFMM::NBIF_SHUB_TODET_CTRL[NBIF_SHUB_TIMEOUT_COUNT, NBIF_SHUB_TODET_TIMER_UNIT] to the value of PcdSyshubWdtTimerInterval in NBIF0
    // Enables the NBIF watchdog timeout by programming NBIFMM::NBIF_SHUB_TODET_CTRL[NBIF_SHUB_TODET_EN] to 1b in NBIF0.
    //
    Value = 0x25;
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      SMN_NBIF_SHUB_TODET_SYNCFLOOD_CTRL_ADDRESS,
                      &Value,
                      0
                      );
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      SMN_NBIF_SHUB_TODET_CLIENT_CTRL_ADDRESS,
                      &Value,
                      0
                      );
    SmnRegisterRead (GnbHandle->Address.Address.Bus,
                     SMN_NBIF_SHUB_TODET_CTRL_ADDRESS,
                     &(NbifTodetCtrlStruct.Value)
                     );
    NbifTodetCtrlStruct.Field.NBIF_SHUB_TIMEOUT_COUNT = (UINT16) PcdGet32(PcdSyshubWdtTimerInterval);
    SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                     SMN_NBIF_SHUB_TODET_CTRL_ADDRESS,
                     &(NbifTodetCtrlStruct.Value),
                     GNB_REG_ACC_FLAG_S3SAVE
                     );
    SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   SMN_NBIF_SHUB_TODET_CTRL_ADDRESS,
                   (UINT32) ~(NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_EN_MASK),
                   0x1 << NBIF_SHUB_TODET_CTRL_NBIF_SHUB_TODET_EN_OFFSET,
                   0
                   );

    PcieCore = 0;
    NbioRASIdentifyPcieCore (GnbHandle, &PcieCore);

    UcpMask = PcdGet32(PcdAmdNbioRASUcpMaskHi);
    UcpMask = UcpMask << (sizeof(UINT32) * 8);
    UcpMask |= PcdGet32(PcdAmdNbioRASUcpMaskLo);

    // Execute the programming sequence given in PPR section 5.8.1 SRAM ECC and Parity Enablement
    for ( Index = 0; Index < sizeof (mNbioParityTable) / sizeof (NBIO_PARITY_TABLE); Index++) {
      // only enable PCIe RAS if the PCIe core is in use
      if (mNbioParityTable[Index].IpType == RASCONTROL_IP_L1IMUPCIE0) {
        if ((PcieCore & BIT0) == 0) {
          continue;
        }
      }

      ParityControl1Data.Value = 0;
      ParityControl1Data.Field.ParityErrGenGroupSel = mNbioParityTable[Index].GroupID;
      ParityControl1Data.Field.ParityErrGenIdSel = mNbioParityTable[Index].StructureID;
      PollAndSetNbioRasFeature(GnbHandle, NULL, PARITY_CONTROL_INIT);

      switch (mNbioParityTable[Index].GroupType) {
        case UCP_GROUP_TYPE:
          ParityControl1Data.Field.ParityErrGenGroupTypeSel = 1; // CORR
          if ((UcpMask & LShiftU64(1, Index)) == 0) {            
            PollAndSetNbioRasFeature(GnbHandle, &ParityControl1Data, ECC_PARITY_WRAPPER_EN_UCB_CONV);
            PollAndSetNbioRasFeature(GnbHandle, &ParityControl1Data, ECC_PARITY_WRAPPER_EN_SEC_REPORT);
            PollAndSetNbioRasFeature(GnbHandle, &ParityControl1Data, ECC_PARITY_WRAPPER_EN_UCP_REPORT);
          } else {
            IDS_HDT_CONSOLE (MAIN_FLOW, "Masking UCP for GroupId = %d, StructureId = %d\n", mNbioParityTable[Index].GroupID, mNbioParityTable[Index].StructureID);
            PollAndSetNbioRasFeature(GnbHandle, &ParityControl1Data, ECC_PARITY_WRAPPER_EN_SEC_REPORT);
            PollAndSetNbioRasFeature(GnbHandle, &ParityControl1Data, ECC_PARITY_WRAPPER_EN_DED_REPORT);
          }
        break;
        default:
          IDS_HDT_CONSOLE (GNB_TRACE, "Unknown group type in %a, entry is %d\n",
                          __FUNCTION__, mNbioParityTable[Index].GroupType);
          ASSERT(FALSE);
          break;
      }
    }

    NbioSmnTable (GnbHandle, NbioApmlErrTable, NBIO_SPACE (GnbHandle, 0), Property, 0);
  }
  SmnRegisterRMW (
    GnbHandle->Address.Address.Bus,
    SMN_ErrEvent_ACTION_CONTROL_ADDRESS,
    (UINT32) ~(ErrEvent_ACTION_CONTROL_SyncFlood_En_MASK),
    (0x0 << ErrEvent_ACTION_CONTROL_SyncFlood_En_OFFSET),
    GNB_REG_ACC_FLAG_S3SAVE
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}