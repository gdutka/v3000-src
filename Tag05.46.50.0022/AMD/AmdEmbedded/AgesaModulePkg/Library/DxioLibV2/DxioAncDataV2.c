/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * DXIO Ancillary Data handlers
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <PiPei.h>
#include <AmdPcieComplex.h>
#include <GnbDxio.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Library/BaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/DxioLibV2.h>

#define FILECODE LIBRARY_DXIOLIBV2_DXIOANCDATAV2_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Function to calculate the ancillary data size for a PCIe/SATA engine
 *
 *
 * @param[in]  PortPointer   Pointer to port descriptor
 */
UINT32
DxioAncDataCalcSize (
  IN       DXIO_PORT_DESCRIPTOR    *PortPointer
  )
{
  DXIO_PHY_PARAM    *PhyParam;
  UINT32            DataSize;
  UINT32            LaneCount;

  PhyParam = &PortPointer->Phy.PhyParam[0];

  DataSize = 0;
  while (PhyParam->ParamType != 0) {
    switch (PhyParam->ParamType) {
      case   rx_vref_ctrl:
      case   SSC_OFF_FRUG1:
      case   SSC_OFF_PHUG1:
      case   GEN1_txX_iboost_lvl:
      case   GEN1_txX_vboost_en:
      case   GEN1_Unsupported1:
      case   GEN1_Unsupported2:
      case   GEN1_rxX_eq_att_lvl:
      case   GEN1_rxX_eq_vga1_gain:
      case   GEN1_rxX_eq_vga2_gain:
      case   GEN1_rxX_eq_dfe_tap1:
      case   GEN1_tx_vboost_lvl:
      case   GEN2_txX_iboost_lvl:
      case   GEN2_txX_vboost_en:
      case   GEN2_Unsupported1:
      case   GEN2_Unsupported2:
      case   GEN2_rxX_eq_att_lvl:
      case   GEN2_rxX_eq_vga1_gain:
      case   GEN2_rxX_eq_vga2_gain:
      case   GEN2_rxX_eq_dfe_tap1:
      case   GEN2_tx_vboost_lvl:
      case   GEN3_txX_iboost_lvl:
      case   GEN3_txX_vboost_en:
      case   GEN3_rxX_adapt_afe_en:
      case   GEN3_rxX_adapt_dfe_en:
      case   GEN3_rxX_eq_att_lvl:
      case   GEN3_rxX_eq_vga1_gain:
      case   GEN3_rxX_eq_vga2_gain:
      case   GEN3_rxX_eq_dfe_tap1:
      case   GEN3_tx_vboost_lvl:
      case   GEN4_txX_iboost_lvl:
      case   GEN4_txX_vboost_en:
      case   GEN4_rxX_adapt_afe_en:
      case   GEN4_rxX_adapt_dfe_en:
      case   GEN4_rxX_eq_att_lvl:
      case   GEN4_rxX_eq_vga1_gain:
      case   GEN4_rxX_eq_vga2_gain:
      case   GEN4_rxX_eq_dfe_tap1:
      case   GEN4_tx_vboost_lvl:
        // For a single-entry parameter
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        break;

      case   rxX_los_threshold:
      case   GEN1_txX_eq_pre:
      case   GEN1_txX_eq_main:
      case   GEN1_txX_eq_post:
      case   GEN1_rxX_eq_ctle_pole:
      case   GEN1_rxX_eq_ctle_boost:
      case   GEN1_txX_pre_deemphasis:
      case   GEN1_txX_post_deemphasis:
      case   GEN2_txX_eq_pre:
      case   GEN2_txX_eq_main:
      case   GEN2_txX_eq_post:
      case   GEN2_rxX_eq_ctle_pole:
      case   GEN2_rxX_eq_ctle_boost:
      case   GEN2_txX_pre_deemphasis:
      case   GEN2_txX_post_deemphasis:
      case   GEN3_txX_eq_pre:
      case   GEN3_txX_eq_main:
      case   GEN3_txX_eq_post:
      case   GEN3_rxX_eq_ctle_pole:
      case   GEN3_rxX_eq_ctle_boost:
      case   GEN3_txX_pre_deemphasis:
      case   GEN3_txX_post_deemphasis:
      case   GEN4_txX_eq_pre:
      case   GEN4_txX_eq_main:
      case   GEN4_txX_eq_post:
      case   GEN4_rxX_eq_ctle_pole:
      case   GEN4_rxX_eq_ctle_boost:
      case   GEN4_txX_pre_deemphasis:
      case   GEN4_txX_post_deemphasis:
        // For a per-lane parameter
        if (PortPointer->EngineData.StartLane > PortPointer->EngineData.EndLane) {
          LaneCount = (PortPointer->EngineData.StartLane - PortPointer->EngineData.EndLane) + 1;
        } else {
          LaneCount = (PortPointer->EngineData.EndLane - PortPointer->EngineData.StartLane) + 1;
        }
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        break;

      default:
        ASSERT (FALSE);
        break;
    }
    PhyParam++;
  }
  if (DataSize != 0) {
    DataSize += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
  }
  return DataSize;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Function to populate the ancillary data for a PCIe/SATA engine
 *
 *
 * @param[in]  EnginePointer   Pointer to engine descriptor
 * @param[in]  PortPointer     Pointer to engine topology
 * @param[in]  AncData         Pointer to next available location in ancillary data
 */
VOID
DxioAncEntryPopulate (
  IN       DXIO_OVRSSETV2_ENTRY    *AncDataEntry,
  IN       BOOLEAN                 ForceV1,
  IN       UINT32                  OverrideField,
  IN       UINT32                  FieldGen,
  IN       UINT32                  LaneNumber,
  IN       UINT32                  ParameterValue
  )
{
  DXIO_OVRSSETV1_ENTRY       *V1DataEntry;

  if (ForceV1) {
    V1DataEntry = (DXIO_OVRSSETV1_ENTRY *) AncDataEntry;
    V1DataEntry->overrideField = OverrideField;
    V1DataEntry->fieldGen = FieldGen;
    V1DataEntry->laneNumber = LaneNumber;
    V1DataEntry->value = ParameterValue;
  } else {
    AncDataEntry->overrideField = OverrideField;
    AncDataEntry->fieldGen = FieldGen;
    AncDataEntry->laneNumber = LaneNumber;
    AncDataEntry->value = ParameterValue;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Function to populate the ancillary data for a PCIe/SATA engine
 *
 *
 * @param[in]  EnginePointer   Pointer to engine descriptor
 * @param[in]  PortPointer     Pointer to engine topology
 * @param[in]  AncData         Pointer to next available location in ancillary data
 */
UINT32
DxioAncDataPopulate (
  IN       DXIO_ENGINE_DESCRIPTOR  *EnginePointer,
  IN       DXIO_PORT_DESCRIPTOR    *PortPointer,
  IN       UINT8                   *AncData
  )
{
  DXIO_PHY_PARAM                *PhyParam;
  UINT32                        DataSize;
  UINT32                        Index;
  UINT32                        StartLane;
  UINT32                        EndLane;
  UINT32                        LaneCount;
  DXIO_OVRSSETV2_ENTRY          *AncDataEntry;
  DXIO_ANCILLARYDATA_SUBHEAP    *AncDataHeader;
  BOOLEAN                       ForceV1;

  ForceV1 = PcdGetBool (PcdDxioCompatibleV1Lib);
  AncDataEntry = (DXIO_OVRSSETV2_ENTRY *) (AncData + sizeof (DXIO_ANCILLARYDATA_SUBHEAP));
  PhyParam = &PortPointer->Phy.PhyParam[0];

  IDS_HDT_CONSOLE (GNB_TRACE, "AncDataPhy enter\n");
  IDS_HDT_CONSOLE (GNB_TRACE, "     Start Lane: %d\n", EnginePointer->StartLane);

  DataSize = 0;
  StartLane = EnginePointer->StartLane;
  EndLane = EnginePointer->EndLane;
  if (StartLane > EndLane) {
    //Handle the port reversal case
    StartLane = EnginePointer->EndLane;
    EndLane = EnginePointer->StartLane;
  }
  LaneCount = (EndLane - StartLane) + 1;
  while (PhyParam->ParamType != 0) {
    switch (PhyParam->ParamType) {
      case   SSC_OFF_FRUG1:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_SSC_OFF_FRUG1,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   SSC_OFF_PHUG1:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_SSC_OFF_PHUG1,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   rx_vref_ctrl:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rx_vref_ctrl,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_txX_iboost_lvl:
      case   GEN2_txX_iboost_lvl:
      case   GEN3_txX_iboost_lvl:
      case   GEN4_txX_iboost_lvl:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_txX_iboost_lvl,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_txX_vboost_en:
      case   GEN2_txX_vboost_en:
      case   GEN3_txX_vboost_en:
      case   GEN4_txX_vboost_en:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_txX_vboost_en,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_rxX_eq_att_lvl:
      case   GEN2_rxX_eq_att_lvl:
      case   GEN3_rxX_eq_att_lvl:
      case   GEN4_rxX_eq_att_lvl:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rxX_eq_att_lvl,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_rxX_eq_vga1_gain:
      case   GEN2_rxX_eq_vga1_gain:
      case   GEN3_rxX_eq_vga1_gain:
      case   GEN4_rxX_eq_vga1_gain:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rxX_eq_vga1_gain,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_rxX_eq_vga2_gain:
      case   GEN2_rxX_eq_vga2_gain:
      case   GEN3_rxX_eq_vga2_gain:
      case   GEN4_rxX_eq_vga2_gain:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rxX_eq_vga2_gain,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_rxX_eq_dfe_tap1:
      case   GEN2_rxX_eq_dfe_tap1:
      case   GEN3_rxX_eq_dfe_tap1:
      case   GEN4_rxX_eq_dfe_tap1:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rxX_eq_dfe_tap1,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN1_tx_vboost_lvl:
      case   GEN2_tx_vboost_lvl:
      case   GEN3_tx_vboost_lvl:
      case   GEN4_tx_vboost_lvl:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_tx_vboost_lvl,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN3_rxX_adapt_afe_en:
      case   GEN4_rxX_adapt_afe_en:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rxX_adapt_afe_en,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      case   GEN3_rxX_adapt_dfe_en:
      case   GEN4_rxX_adapt_dfe_en:
        DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
        DxioAncEntryPopulate (AncDataEntry,
                              ForceV1,
                              DXIO_OVRSSET_FLD_rxX_adapt_dfe_en,
                              PhyParam->ParamType >> 6,
                              StartLane,
                              PhyParam->ParamValue
                              );
        AncDataEntry++;
        break;

      // For a per-lane parameter
      case   rxX_los_threshold:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DataSize += sizeof (DXIO_OVRSSETV2_ENTRY);
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_rxX_los_threshold,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      case   GEN1_txX_eq_pre:
      case   GEN2_txX_eq_pre:
      case   GEN3_txX_eq_pre:
      case   GEN4_txX_eq_pre:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_txX_eq_pre,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      case   GEN1_txX_eq_main:
      case   GEN2_txX_eq_main:
      case   GEN3_txX_eq_main:
      case   GEN4_txX_eq_main:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_txX_eq_main,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      case   GEN1_txX_eq_post:
      case   GEN2_txX_eq_post:
      case   GEN3_txX_eq_post:
      case   GEN4_txX_eq_post:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_txX_eq_post,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;
      case   GEN1_rxX_eq_ctle_pole:
      case   GEN2_rxX_eq_ctle_pole:
      case   GEN3_rxX_eq_ctle_pole:
      case   GEN4_rxX_eq_ctle_pole:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_rxX_eq_ctle_pole,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      case   GEN1_rxX_eq_ctle_boost:
      case   GEN2_rxX_eq_ctle_boost:
      case   GEN3_rxX_eq_ctle_boost:
      case   GEN4_rxX_eq_ctle_boost:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_rxX_eq_ctle_boost,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      case   GEN1_txX_pre_deemphasis:
      case   GEN2_txX_pre_deemphasis:
      case   GEN3_txX_pre_deemphasis:
      case   GEN4_txX_pre_deemphasis:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_txX_pre_deemphasis,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      case   GEN1_txX_post_deemphasis:
      case   GEN2_txX_post_deemphasis:
      case   GEN3_txX_post_deemphasis:
      case   GEN4_txX_post_deemphasis:
        DataSize += LaneCount * sizeof (DXIO_OVRSSETV2_ENTRY);
        for (Index = StartLane; Index <= EndLane; Index++) {
          DxioAncEntryPopulate (AncDataEntry,
                                ForceV1,
                                DXIO_OVRSSET_FLD_txX_post_deemphasis,
                                PhyParam->ParamType >> 6,
                                Index,
                                PhyParam->ParamValue
                                );
          AncDataEntry++;
        }
        break;

      default:
        ASSERT (FALSE);
        break;
    }
    PhyParam++;
  }
  if (DataSize != 0) {
    DataSize += sizeof (DXIO_ANCILLARYDATA_SUBHEAP);
    AncDataHeader = (DXIO_ANCILLARYDATA_SUBHEAP *) AncData;
    AncDataHeader->descriptorType = DXIO_ANCDATA_SUBHEAPTYPE_OVRRSET;
    if (ForceV1) {
      AncDataHeader->version = DXIO_ANCILLARYDATA_SUBHEAP_VERSION;
    } else {
      AncDataHeader->version = DXIO_ANCILLARYDATA_REL_SUBHEAP_VERSION;
    }
    AncDataHeader->numDWORDs = DataSize >> 2;
  }
  return DataSize;
}



