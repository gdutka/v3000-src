/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains definitions for ApcbLib
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
 */
#ifndef _APCB_LIB_H_
#define _APCB_LIB_H_
#include <AGESA.h>
#include <APCB.h>

/**
 *  APCB replace type
 *  Function to update the APCB parameters of a given type
 *
 *  @param[in]     GroupId         APCB Group ID
 *  @param[in]     ApcbParamType   APCB Parameter type
 *  @param[in]     InstanceId      APCB Instance ID
 *  @param[in]     TypeDataStream  APCB Type data stream
 *  @param[in]     TypeDataSize    APCB Type Data size
 *  @param[in,out] NewApcb         New APCB type replaced
 *
 *  @retval TRUE    APCB Type replaced successfully
 *  @retval FALSE   Failed to replace APCB type
 *
 **/
BOOLEAN ApcbReplaceType (
  IN     UINT16          GroupId,
  IN     APCB_PARAM_TYPE ApcbParamType,
  IN     UINT16          InstanceId,
  IN     UINT8           *TypeDataStream,
  IN     UINT32          TypeDataSize,
  IN OUT UINT8           *NewApcb
  );

/**
 *
 *  Get APCB config parameters
 *  This function always get the parameters from shadow memory copy instead of read from real SPI
 *
 *  @param[in]     TokenId      APCB token ID defined in APCB_PARAM_CONFIG_TOKEN of APCB.h
 *  @param[in,out] SizeInByte   Set to non-NULL to get Size in bytes of the APCB token
 *  @param[in,out] Value        Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *  @retval EFI_NOT_FOUND  Can't find the according APCB token
 *  @retval EFI_INVALID_PARAMETER  Invalid parameters
 *
 **/
EFI_STATUS
AmdPspApcbGetCfgParameter (
  IN       APCB_PARAM_CONFIG_TOKEN TokenId,
  IN OUT   UINT32           *SizeInByte,
  IN OUT   UINT64          *Value
  );

/**
 *
 *  Set APCB config parameters
 *  This function always Set the parameters to shadow memory copy
 *
 *  @param[in]     TokenId      APCB token ID defined in APCB_PARAM_CONFIG_TOKEN of APCB.h
 *  @param[in,out] SizeInByte   Set to non-NULL to set Size in bytes of the APCB token
 *  @param[in,out] Value        Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    APCB value Set successfully
 *  @retval EFI_NOT_FOUND  Can't find the according APCB token
 *  @retval EFI_INVALID_PARAMETER  Invalid parameters
 *
 **/
EFI_STATUS
AmdPspApcbSetCfgParameter (
  IN       APCB_PARAM_CONFIG_TOKEN TokenId,
  IN       UINT32           *SizeInByte,
  IN       UINT64          *Value
  );

/**
 *
 *  Get Common CBS config parameters
 *  This function always get the parameters from shadow memory copy instead of read from real SPI
 *
 *  @param[in]     TokenId      APCB token ID defined in APCB_PARAM_CONFIG_TOKEN of APCB.h
 *  @param[in,out] SizeInByte   Set to non-NULL to get Size in bytes of the APCB token
 *  @param[in,out] Value        Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Get APCB value successfully
 *  @retval EFI_NOT_FOUND  Can't find the according APCB token
 *  @retval EFI_INVALID_PARAMETER  Invalid parameters
 *
 **/
EFI_STATUS
AmdPspApcbGetCbsCmnParameter (
  IN       UINT16          TokenId,
  IN OUT   UINT32          *SizeInByte,
  IN OUT   UINT64          *Value
  );

/**
 *
 *  Set Common CBS config parameters
 *  This function always Set the parameters to shadow memory copy
 *
 *  @param[in]     TokenId      APCB token ID defined in APCB_PARAM_CONFIG_TOKEN of APCB.h
 *  @param[in,out] SizeInByte   Set to non-NULL to set Size in bytes of the APCB token
 *  @param[in,out] Value        Value of APCB token ID
 *
 *  @retval EFI_SUCCESS    Set APCB value successfully
 *  @retval EFI_NOT_FOUND  Can't find the according APCB token
 *  @retval EFI_INVALID_PARAMETER  Invalid parameters
 *
 **/
EFI_STATUS
AmdPspApcbSetCbsCmnParameter (
  IN       UINT16          TokenId,
  IN OUT   UINT32          *SizeInByte,
  IN OUT   UINT64          *Value
  );

/**
 *  Write APCB data to BIOS Directory APCB Entry 0x60
 *
 *
 *  @retval EFI_SUCCESS       The Data save to FLASH successfully
 *  @retval Other             Some error occurs when executing this function.
 *
 **/
EFI_STATUS
AmdPspWriteBackApcbShadowCopy (
  VOID
  );

#endif      /* _APCB_LIB_H_ */


