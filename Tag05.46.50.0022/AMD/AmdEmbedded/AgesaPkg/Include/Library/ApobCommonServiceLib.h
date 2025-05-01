/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file ApobCommonServiceLib.h
 *
 * @brief  AGESA external Lib for APOB related
 *
 * @details Extract most used data from APOB , provide the generic service through AmdPspApobLib
 *
 */
#ifndef _APOB_COMMON_SERVICELIB_H_
#define _APOB_COMMON_SERVICELIB_H_

#include "ApobCommonServicePub.h"
#include <Addendum/Apcb/Inc/RMB/APOB.h>

/**
 *  Return APCB Active Instance
 *
 *  @param[in, out] APCB Active Instance
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetApcbInstance (
  IN OUT   UINT8  *ApcbInstance
  );

/**
 *  Return ApobApcbRecoveryFlag
 *
 *  @param[in, out] APCB Recovery Flag
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetApcbRecoveryFlag (
  IN OUT   BOOLEAN  *ApcbRecoveryFlag
  );

/**
 *  Return DimmCfgUpdatedFlag
 *
 *  @param[in, out] DimmCfgUpdatedFlag
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetDimmCfgUpdatedFlag (
  IN OUT   BOOLEAN  *DimmCfgUpdatedFlag
  );

#define CCD_INDEX_NOT_USED      0xFFFFFFFFul
/**
 *  Get the physical CCD number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap, Note, CcdIndex will be ignored in Pre Family 17h Model 30h
 *  @param[in, out] PhysComplexNumber
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetPhysCcdNumber (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN OUT   UINT8 *PhysCcdNumber
);

/**
 *  Get the physical complex number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap, Note, CcdIndex will be ignored in Pre Family 17h Model 30h
 *  @param[in, out] PhysComplexNumber
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetPhysComplexNumber (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN       UINT32 CcxIndex,
  IN OUT   UINT8 *PhysComplexNumber
);

/**
 *  Get the physical core number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap, Note, CcdIndex will be ignored in Pre Family 17h Model 30h
 *  @param[in] CoreIndex used access CoreInfo
 *  @param[in, out] PhysCoreNumber
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetPhysCoreNumber (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN       UINT32 CcxIndex,
  IN       UINT32 CoreIndex,
  IN OUT   UINT8  *PhysCoreNumber
);

/**
 *  Check if the thread is enable by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap, Note, CcdIndex will be ignored in Pre Family 17h Model 30h
 *  @param[in] CoreIndex used access CoreInfo
 *  @param[in] ThreadIndex used access IsThreadEnabled
 *  @param[in, out] IsThreadEnabled
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
ApobGetIsThreadEnabled (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 CcdIndex,
  IN       UINT32 CcxIndex,
  IN       UINT32 CoreIndex,
  IN       UINT32 ThreadIndex,
  IN OUT   BOOLEAN *IsThreadEnabled
);

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
ApobGetEccEnable (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 ChannelIndex,
  IN OUT   BOOLEAN *EccEnable
);

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
ApobGetNvdimmPresentInSystem (
  IN       UINT32 ApobInstanceId,
  IN OUT   BOOLEAN *NvdimmPresentInSystem
);

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
ApobGetNvdimmInfo (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT32 **NvdimmInfo,
  IN OUT   UINT32 *NvdimmInfoSize
);

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
ApobGetMaxDimmsPerChannel (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT8 *MaxDimmsPerChannel
);

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
ApobGetMaxChannelsPerDie (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT8 *MaxChannelsPerDie
);

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
ApobGetMaxChannelsPerSocket (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT8 *MaxChannelsPerSocket
);

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
ApobGetDimmSmbusInfo (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 Index,
  IN OUT   APOB_SPD_STRUCT **DimmSmbusInfo
);

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
ApobGetMemClkFreq (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT16 *MemClkFreq
);

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
ApobGetDdrMaxRate (
  IN       UINT32 ApobInstanceId,
  IN OUT   UINT16 *DdrMaxRate
);

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
ApobGetMemGenInfoElement (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 ElementId,
  IN OUT   UINT16 *ElementSizeInByte,
  IN OUT   UINT64 *ElementValue
  );

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
ApobGetNumberOfValidThresholds (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 *NumberOfValidThresholds
  );

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
ApobGetThresholds (
  IN       UINT32 ApobInstanceId,
  IN       UINT32 Index,
  IN       EDC_THROTTLE_THRESHOLD **Thresholds
  );

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
ApobGetBoardMask (
  IN       UINT32 ApobInstanceId,
  IN       UINT16 *BoardMask
  );

/**
 * @brief Get SubProgram through read APOB_GEN_CONFIGURATION_INFO_TYPE.SubProgram
 *
 * @param SubProgram Point to the address to hold SubProgram
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
EFI_STATUS
ApobGetSubProgram (
  OUT      UINT32 *SubProgram
  );

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
ApobGetDimmSpdData (
  IN       UINT32 ApobInstanceId,
  IN       UINT8  Socket,
  IN       UINT8  Channel,
  IN       UINT8  Dimm,
  IN       UINT32 BufSize,
  IN OUT   UINT8  *SpdBufPtr
);
#endif //_APOB_COMMON_SERVICELIB_H_


