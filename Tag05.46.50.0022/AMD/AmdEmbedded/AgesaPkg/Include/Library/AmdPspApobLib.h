/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdPspApobLib.h
 *
 * @brief  AGESA external Lib for APOB related
 *
 * @details Get the APOB raw data through APOB type, caller need to render the data by reference program
 *          specific APOB header file
 *
 */
#ifndef _AMDPSP_APOBLIB_H_
#define _AMDPSP_APOBLIB_H_
#define APOB_ENTRY_INSTANCE_MAX 0xFF
#include <Addendum/Apcb/Inc/APOBCMN.h>
///
///The data struct used by APOBLIB as global variable
///
typedef struct _APOBLIB_INFO {
  BOOLEAN  Supported;                ///<  Specify if APOB supported
  UINT32    ApobSize;                 ///<  ApobSize
  UINT64    ApobAddr;                 ///<  The Address of APOB
} APOBLIB_INFO;

/**
 *  Get APOBLib Info Data
 *
 * @param[in,out]  **ApobInfo      Pointer to fill the APOB Info
 *
 *  @retval EFI_SUCCESS       The APOB pointer get successfully
 *
 **/
EFI_STATUS
AmdPspGetApobInfo (
  IN OUT   APOBLIB_INFO  **ApobInfo
  );

/**
 *  Return Array of Entry instance as specified GroupId, DataTypeId
 *
 *  @param[in] GroupID GroupId of Apob entry
 *  @param[in] DataTypeID DataTypeID of Apob entry
 *  @param[in] ReadFromSpiCopy TRUE force read from SPI copy
 *  @param[out] NumofEntry Number of entry found of specific GroupId, DataTypeId
 *  @param[out] ApobEntries Point to the arrary which used to hold the ApobEntries
 *              ApobEntries arrary need be allocated outside of the routine
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
AmdPspGetApobEntry (
  IN       UINT32  GroupID,
  IN       UINT32  DataTypeID,
  IN       BOOLEAN  ReadFromSpiCopy,
     OUT   UINT32  *NumofEntry,
     OUT   APOB_TYPE_HEADER **ApobEntries
  );

/**
 *  Return Entry instance as specified GroupId, DataTypeId, InstanceId
 *
 *  @param[in] GroupID GroupId of Apob entry
 *  @param[in] DataTypeID DataTypeID of Apob entry
 *  @param[out] InstanceID InstanceID of Apob entry
 *  @param[in] ReadFromSpiCopy TRUE force read from SPI copy
 *  @param[out] ApobEntry Point to the arrary which used to hold the ApobEntries
 *              ApobEntries arrary need be allocated outside of the routine
 *
 *  @retval       EFI_SUCCESS       Function succeed
 *  @retval       NON-ZERO          Error occurs
*/
EFI_STATUS
AmdPspGetApobEntryInstance (
  IN       UINT32  GroupID,
  IN       UINT32  DataTypeID,
  IN       UINT32  InstanceID,
  IN       BOOLEAN  ReadFromSpiCopy,
     OUT   APOB_TYPE_HEADER **ApobEntry
  );

/**
 * @brief  AmdPspApobLib Constructor
 *
 * @details Get APOB address, and save to the HOB
 *
 *  @retval EFI_SUCCESS       The APOB save to HOB successfully
 *
 **/
EFI_STATUS
EFIAPI
AmdPspApobLibConstructor (
  VOID
  );
#endif //_AMDPSP_APOBLIB_H_


