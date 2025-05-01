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
 * PSP APOB related functions
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309172 $   @e \$Date: 2014-12-10 21:13:24 +0800 (Wed, 10 Dec 2014) $
 */
#include "Uefi.h"
#include <Pi/PiMultiPhase.h>
#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdPspApobLib.h>
#include <RMB/APOB.h>
#include <ApobCommonServicePub.h>
#include <Filecode.h>

#define FILECODE LIBRARY_APOBRMBLIB_APOBRMBLIB_FILECODE


/**
 *  Return ApobApcbRecoveryFlag
 *
 *  @param[in, out] APCB Recovery Flag
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetApcbRecoveryFlagRmb (
  IN OUT   BOOLEAN  *ApcbRecoveryFlag
  )
{
  EFI_STATUS Status;
  APOB_APCB_BOOT_INFO_STRUCT *ApobEntry;
  ApobEntry = NULL;
  *ApcbRecoveryFlag = FALSE;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_APCB_BOOT_INFO_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Fail to get ApobApcbRecoveryFlag\n");
    return EFI_NOT_FOUND;
  }
  *ApcbRecoveryFlag = ApobEntry->ApcbRecoveryFlag;
  return Status;
}

/*
/**
 *  Return APCB Active Instance
 *
 *  @param[in, out] APCB Active Instance
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetApcbInstanceRmb (
  IN OUT   UINT8  *ApcbInstance
  )
{
  EFI_STATUS Status;
  APOB_APCB_BOOT_INFO_STRUCT *ApobEntry;
  ApobEntry = NULL;
  *ApcbInstance = 0;
  IDS_HDT_CONSOLE_PSP_TRACE ("GetApcbInstanceRn\n");

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_APCB_BOOT_INFO_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Fail to get ApobApcbInstance\n");
    return EFI_NOT_FOUND;
  }
  *ApcbInstance = (UINT8) ApobEntry->ApcbActiveInstance;
  return Status;
}

/**
 *  Return DimmCfgUpdatedFlag
 *
 *  @param[in, out] DimmCfgUpdatedFlag
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetDimmCfgUpdatedFlagRmb (
  IN OUT   BOOLEAN  *DimmCfgUpdatedFlag
  )
{
  EFI_STATUS Status;
  APOB_APCB_BOOT_INFO_STRUCT *ApobEntry;

  ApobEntry = NULL;
  *DimmCfgUpdatedFlag = FALSE;

  IDS_HDT_CONSOLE_PSP_TRACE ("GetApcbInstanceRn\n");
  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_APCB_BOOT_INFO_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Fail to get DimmCfgUpdatedFlag\n");
    return EFI_NOT_FOUND;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("DimmCfgUpdatedFlag %x\n", ApobEntry->DimmConfigurationUpdated);
  *DimmCfgUpdatedFlag = ApobEntry->DimmConfigurationUpdated;

  return Status;
}
/**
 *  Get the physical CCD number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap
 *  @param[in, out] PhysComplexNumber
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetPhysCcdNumberRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN OUT   UINT8 *PhysCcdNumber
  )
{
  EFI_STATUS Status;

  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *PhysCcdNumber = ApobEntry->CcdMap[CcdIndex].PhysCcdNumber;
  }
  return Status;
}
/**
 *  Get the physical complex number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap
 *  @param[in, out] PhysComplexNumber
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetPhysComplexNumberRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN       UINT32 CcxIndex,
  IN OUT   UINT8 *PhysComplexNumber
  )
{
  EFI_STATUS Status;

  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *PhysComplexNumber = ApobEntry->CcdMap[CcdIndex].ComplexMap[CcxIndex].PhysComplexNumber;
  }
  return Status;
}

/**
 *  Get the physical core number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap
 *  @param[in] CoreIndex used access CoreInfo
 *  @param[in, out] PhysCoreNumber
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetPhysCoreNumberRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN       UINT32 CcxIndex,
  IN       UINT32 CoreIndex,
  IN OUT   UINT8  *PhysCoreNumber
  )
{
  EFI_STATUS Status;

  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *PhysCoreNumber = ApobEntry->CcdMap[CcdIndex].ComplexMap[CcxIndex].CoreInfo[CoreIndex].PhysCoreNumber;
  }
  return Status;
}

/**
 *  Check if the thread is enable by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap
 *  @param[in] CoreIndex used access CoreInfo
 *  @param[in] ThreadIndex used access IsThreadEnabled
 *  @param[in, out] IsThreadEnabled
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetIsThreadEnabledRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN       UINT32 CcxIndex,
  IN       UINT32 CoreIndex,
  IN       UINT32 ThreadIndex,
  IN OUT   BOOLEAN *IsThreadEnabled
  )
{
  EFI_STATUS Status;

  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *IsThreadEnabled = ApobEntry->CcdMap[CcdIndex].ComplexMap[CcxIndex].CoreInfo[CoreIndex].IsThreadEnabled[ThreadIndex];
  }
  return Status;
}

/**
 *  Check EccEnable by reading APOB APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] ChannelIndex used access EccEnable arrary
 *  @param[in, out] EccEnable
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetEccEnableRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 ChannelIndex,
  IN OUT   BOOLEAN *EccEnable
  )
{
  EFI_STATUS Status;
  APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *EccEnable = ApobEntry->EccEnable[ChannelIndex];
  }
  return Status;
}

/**
 *  Get NvdimmPresentInSystem by reading APOB APOB_MEM_NVDIMM_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] NvdimmPresentInSystem
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetNvdimmPresentInSystemRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   BOOLEAN *NvdimmPresentInSystem
  )
{
  EFI_STATUS Status;
  APOB_MEM_NVDIMM_INFO_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_NVDIMM_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *NvdimmPresentInSystem = ApobEntry->NvdimmPresentInSystem;
  }
  return Status;
}

/**
 *  Get NvdimmPresentInSystem by reading APOB APOB_MEM_NVDIMM_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] NvdimmInfo
 *  @param[in, out] NvdimmInfoSize, on Input size used for hold NvdimmInfo, on Output the real size used in APOB
 *
 *  @retval       EFI_SUCCESS             Function succeed
 *  @retval       EFI_BUFFER_TOO_SMALL    NvdimmInfoSize not enough size
 *  @retval       other NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetNvdimmInfoRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT32 **NvdimmInfo,
  IN OUT   UINT32 *NvdimmInfoSize
  )
{
  EFI_STATUS Status;
  APOB_MEM_NVDIMM_INFO_STRUCT *ApobEntry;

 *NvdimmInfoSize = sizeof (ApobEntry->NvdimmInfo);

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_NVDIMM_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
     *NvdimmInfo = &ApobEntry->NvdimmInfo[0][0];
  }
  return Status;
}

/**
 *  Get MaxDimmsPerChannel of SPD structure by reading APOB APOB_MEM_DIMM_SPD_DATA_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] MaxDimmsPerChannel
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetMaxDimmsPerChannelRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT8 *MaxDimmsPerChannel
  )
{
  EFI_STATUS Status;
  APOB_MEM_DIMM_SPD_DATA_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_DIMM_SPD_DATA_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *MaxDimmsPerChannel = ApobEntry->MaxDimmsPerChannel;
  }
  return Status;
}

/**
 *  Get MaxChannelsPerDie
 *
 *  @param[in] ApobInstanceId not used
 *  @param[in, out] MaxDimmsPerChannel
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetMaxChannelsPerDieRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT8 *MaxChannelsPerDie
  )
{
  *MaxChannelsPerDie = ABL_APOB_MAX_CHANNELS_PER_DIE;
  return EFI_SUCCESS;
}
/**
 *  Get MaxChannelsPerSocket of SPD structure by reading APOB APOB_MEM_DIMM_SPD_DATA_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] MaxChannelsPerSocket
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetMaxChannelsPerSocketRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT8 *MaxChannelsPerSocket
  )
{
  EFI_STATUS Status;
  APOB_MEM_DIMM_SPD_DATA_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_DIMM_SPD_DATA_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *MaxChannelsPerSocket = ApobEntry->MaxChannelsPerSocket;
  }
  return Status;
}

/**
 *  Get DimmSmbusInfo of SPD structure by reading APOB APOB_MEM_DIMM_SPD_DATA_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] Index used to access DimmSmbusInfo structure
 *  @param[in, out] DimmSmbusInfo
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetDimmSmbusInfoRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 Index,
  IN OUT   APOB_SPD_STRUCT **DimmSmbusInfo
  )
{
  EFI_STATUS Status;
  APOB_MEM_DIMM_SPD_DATA_STRUCT *ApobEntry;

  if (Index > ABL_APOB_MAX_CHANNELS_PER_DIE * ABL_APOB_MAX_DIMMS_PER_CHANNEL) {
    return EFI_NOT_FOUND;
  }

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_DIMM_SPD_DATA_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *DimmSmbusInfo = &ApobEntry->DimmSmbusInfo[Index];
  }
  return Status;
}


/**
 *  Get MemClkFreq of SPD structure by reading APOB APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] MemClkFreq
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetMemClkFreqRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT16 *MemClkFreq
  )
{
  EFI_STATUS Status;
  APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *MemClkFreq = ApobEntry->MemClkFreq;
  }
  return Status;
}

/**
 *  Get DdrMaxRate of SPD structure by reading APOB APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] DdrMaxRate
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetDdrMaxRateRmb (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT16 *DdrMaxRate
  )
{
  EFI_STATUS Status;
  APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *DdrMaxRate = ApobEntry->DdrMaxRate;
  }
  return Status;
}


/**
 *  Get MemGenInfoElement by reading APOB APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] ElementId refer to APOB_MEM_GEN_INFO_ELEMENT_ID
 *  @param[in, out] ElementSizeInByte
 *  @param[in, out] ElementValue
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetMemGenInfoElementRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 ElementId,
  IN OUT   UINT16 *ElementSizeInByte,
  IN OUT   UINT64 *ElementValue
  )
{
  EFI_STATUS                                        Status;
  APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE_STRUCT   *ApobEntry;
  APOB_MEM_SYSTEM_CONFIGURATION_INFO_TYPE_STRUCT    *ApobSysCfgEntry;
  UINT8                                             Dct;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_GENERAL_CONFIGURATION_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    for (Dct = 0; Dct < ABL_APOB_MAX_CHANNELS_PER_DIE; Dct ++) {
      if (ApobEntry->MemCtrllerProcOdtDddr4Current[Dct] != 0) {
        break;
      }
    }
    if (Dct < ABL_APOB_MAX_CHANNELS_PER_DIE) {
      switch (ElementId) {
        case MemCtrllerProcOdtDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerProcOdtDddr4Current[Dct];
          break;

        case MemCtrllerRttNomDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerRttNomDddr4Current[Dct];
          break;

        case MemCtrllerRttWrDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerRttWrDddr4Current[Dct];
          break;

        case MemCtrllerRttParkDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerRttParkDddr4Current[Dct];
          break;

        case MemCtrllerAddrCmdSetupDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerAddrCmdSetupDddr4Current[Dct];
          break;

        case MemCtrllerCsOdtSetupDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerCsOdtSetupDddr4Current[Dct];
          break;

        case MemCtrllerCkeSetupDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerCkeSetupDddr4Current[Dct];
          break;

        case MemCtrllerCadBusAddrCmdDrvStrenDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerCadBusAddrCmdDrvStrenDddr4Current[Dct];
          break;

        case MemCtrllerCadBusClkDrvStrenDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerCadBusClkDrvStrenDddr4Current[Dct];
          break;

        case MemCtrllerCsOdtCmdDrvStrenDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerCsOdtCmdDrvStrenDddr4Current[Dct];
          break;

        case MemCtrllerCkeDrvStrenDddr4Current:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->MemCtrllerCkeDrvStrenDddr4Current[Dct];
          break;

        case InterLeaveCapability:
          *ElementSizeInByte = 4;
          *ElementValue = ApobEntry->InterLeaveCapability.Value;
          break;

        case InterLeaveCurrentMode:
          *ElementSizeInByte = 4;
          *ElementValue = ApobEntry->InterLeaveCurrentMode.Value;
          break;

        case InterLeaveSize:
          *ElementSizeInByte = 1;
          *ElementValue = ApobEntry->InterLeaveSize;
          break;

        default:
          Status = EFI_NOT_FOUND;
          break;
      }
    } else {
      // ASSERT (FALSE);
      Status = EFI_NOT_FOUND;
    }
  }
  if (Status != EFI_SUCCESS) {
    Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_SYSTEM_CONFIGURATION_INFO_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobSysCfgEntry);
    if (Status == EFI_SUCCESS) {
      for (Dct = 0; Dct < ABL_APOB_MAX_CHANNELS_PER_DIE; Dct ++) {
        if (ApobSysCfgEntry->ChannelBusCfgInfo[Dct].DimmPresent) {
          break;
        }
      }
      if (Dct < ABL_APOB_MAX_CHANNELS_PER_DIE) {
        switch (ElementId) {
          case MemBusCfgCaTimingMode:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CaTimingMode;
            break;
          case MemBusCfgRttNomRd:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].RttNomRd;
            break;
          case MemBusCfgRttNomWr:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].RttNomWr;
            break;
          case MemBusCfgRttWr:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].RttWr;
            break;
          case MemBusCfgRttPark:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].RttPark;
            break;
          case MemBusCfgDqsRttPark:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].DqsRttPark;
            break;
          case MemBusCfgDramDrv:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].DramDrv;
            break;
          case MemBusCfgCkOdtA:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CkOdtA;
            break;
          case MemBusCfgCsOdtA:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CsOdtA;
            break;
          case MemBusCfgCaOdtA:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CaOdtA;
            break;
          case MemBusCfgCkOdtB:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CkOdtB;
            break;
          case MemBusCfgCsOdtB:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CsOdtB;
            break;
          case MemBusCfgCaOdtB:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CaOdtB;
            break;
          case MemBusCfgPOdt:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].POdt;
            break;
          case MemBusCfgDqDrv:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].DqDrv;
            break;
          case MemBusCfgAlertPu:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].AlertPu;
            break;
          case MemBusCfgCaDrv:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CaDrv;
            break;
          case MemBusCfgPhyVref:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].PhyVref;
            break;
          case MemBusCfgDqVref:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].DqVref;
            break;
          case MemBusCfgCaVref:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CaVref;
            break;
          case MemBusCfgCsVref:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].CsVref;
            break;
          case MemBusCfgRxDfe:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].RxDfe;
            break;
          case MemBusCfgTxDfe:
            *ElementSizeInByte = 1;
            *ElementValue = ApobSysCfgEntry->ChannelBusCfgInfo[Dct].TxDfe;
            break;
          default:
            Status = EFI_NOT_FOUND;
            break;
        }
      } else {
        Status = EFI_NOT_FOUND;
      }
    }
  }
  return Status;
}

/**
 *  Get NumberOfValidThresholds by reading APOB APOB_EDC_THROTTLE_THRESHOLD_INFO_STRUCT
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] NumberOfValidThresholds
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetNumberOfValidThresholdsRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 *NumberOfValidThresholds
  )
{
  EFI_STATUS Status;
  APOB_EDC_THROTTLE_THRESHOLD_INFO_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCX_EDC_THROTTLE_THRESH_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);

  if (Status == EFI_SUCCESS) {
    *NumberOfValidThresholds = ApobEntry->NumberOfValidThresholds;
  }

  return Status;
}

/**
 *  Get Thresholds by reading APOB APOB_EDC_THROTTLE_THRESHOLD_INFO_STRUCT
 *
 *  @param[in] ApobInstanceId
 *  @param[in] Index
 *  @param[in, out] Thresholds
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetThresholdsRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 Index,
  IN       EDC_THROTTLE_THRESHOLD **Thresholds
  )
{
  EFI_STATUS Status;
  APOB_EDC_THROTTLE_THRESHOLD_INFO_STRUCT *ApobEntry;

  if (Index > MAX_NUMBER_OF_THROTTLERS) {
    return EFI_NOT_FOUND;
  }

  Status = AmdPspGetApobEntryInstance (APOB_CCX, APOB_CCX_EDC_THROTTLE_THRESH_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *Thresholds = &ApobEntry->Thresholds[Index];
  }

  return Status;
}

/**
 *  Get BoardMask by reading APOB APOB_GEN_CONFIGURATION_INFO_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in, out] BoardMask
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetBoardMaskRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT16 *BoardMask
  )
{
  EFI_STATUS Status;
  APOB_GEN_INFO_TYPE_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_GEN, APOB_GEN_CONFIGURATION_INFO_TYPE, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    *BoardMask = ApobEntry->ApobParamInfo.BoardMask;
  }

  return Status;
}
/**
 * @brief Get SubProgram through read APOB_GEN_CONFIGURATION_INFO_TYPE.SubProgram
 *
 * @param SubProgram Point to the address to hold SubProgram
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
EFI_STATUS
ApobGetSubProgramRmb (
  OUT     UINT32 *SubProgram
  )
{
  return EFI_UNSUPPORTED;
}
/**
 * @brief Get specific SPD data by reading APOB APOB_MEM_DIMM_SPD_DATA_TYPE
 *
 * @param ApobInstanceId
 * @param Socket
 * @param Channel
 * @param Dimm
 * @param BufSize
 * @param SpdBufPtr Point to buffer to hold SPD data
 *
 * @retval       EFI_SUCCESS       Function succeed
 * @retval       NON-ZERO          Error occurs
 */
EFI_STATUS
ApobGetDimmSpdDataRmb (
  IN       UINT32 ApobInstanceId,
  IN       UINT8  Socket,
  IN       UINT8  Channel,
  IN       UINT8  Dimm,
  IN       UINT32 BufSize,
  IN OUT   UINT8  *SpdBufPtr
  )
{
  UINT8             Index;
  UINT32            IndexCount;
  UINT32            TypeSize;
  EFI_STATUS        Status;
  APOB_TYPE_HEADER  *ApobTypeHeader;
  APOB_MEM_DIMM_D5_SPD_DATA_STRUCT *ApobEntry;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_DIMM_SPD_DATA_TYPE, ApobInstanceId, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  ApobTypeHeader = (APOB_TYPE_HEADER *)ApobEntry;
  if (Status == EFI_SUCCESS) {
    TypeSize = ApobTypeHeader->TypeSize;
    IDS_HDT_CONSOLE (MAIN_FLOW, "%x %x %x\n", TypeSize, OFFSET_OF(APOB_MEM_DIMM_D5_SPD_DATA_STRUCT, DimmSmbusInfo), sizeof (APOB_D5_SPD_STRUCT));
    IndexCount    = (TypeSize - OFFSET_OF(APOB_MEM_DIMM_D5_SPD_DATA_STRUCT, DimmSmbusInfo)) / sizeof (APOB_D5_SPD_STRUCT);
    Status = EFI_NOT_FOUND;
    for (Index = 0; Index < IndexCount; Index ++) {
      if (ApobEntry->DimmSmbusInfo[Index].SocketNumber == Socket &&
          ApobEntry->DimmSmbusInfo[Index].ChannelNumber == Channel &&
          ApobEntry->DimmSmbusInfo[Index].DimmNumber == Dimm &&
          ApobEntry->DimmSmbusInfo[Index].DimmPresent) {
        LibAmdMemCopy (SpdBufPtr, ApobEntry->DimmSmbusInfo[Index].Data, (BufSize > 1024) ? 1024 : BufSize, NULL);
        Status = EFI_SUCCESS;
      }
    }
  }
  return Status;
}
