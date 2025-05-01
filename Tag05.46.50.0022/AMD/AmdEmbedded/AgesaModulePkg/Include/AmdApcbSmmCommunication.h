/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/**
 * @file
 *
 * AmdSmmCommunication.h
 *
 * Contains definition for AmdSmmCommunication
 *
 */

#ifndef _AMD_APCB_SMM_COMMUNICATION_H_
#define _AMD_APCB_SMM_COMMUNICATION_H_

extern EFI_GUID gApcbSmmCommHandleGuid;

#define CBS_APCB_COMM_ID_FLUSH_DATA                        0xA1CB0000     ///< ID for APCB FlushData
#define CBS_APCB_COMM_ID_GET_CONFIG                        0xA1CB0001     ///< ID for APCB GetConfigParameter
#define CBS_APCB_COMM_ID_SET_CONFIG                        0xA1CB0002     ///< ID for APCB SetConfigParameter
#define CBS_APCB_COMM_ID_ACQUIRE_MUTEX                     0xA1CB0003     ///< ID for APCB AcquireMutex
#define CBS_APCB_COMM_ID_RELEASE_MUTEX                     0xA1CB0004     ///< ID for APCB ReleaseMutex
#define CBS_APCB_COMM_ID_GET_TOKEN_BOOL                    0xA1CB0005     ///< ID for APCB GetTokenBool
#define CBS_APCB_COMM_ID_SET_TOKEN_BOOL                    0xA1CB0006     ///< ID for APCB SetTokenBool
#define CBS_APCB_COMM_ID_GET_TOKEN_8                       0xA1CB0007     ///< ID for APCB GetToken8
#define CBS_APCB_COMM_ID_SET_TOKEN_8                       0xA1CB0008     ///< ID for APCB SetToken8
#define CBS_APCB_COMM_ID_GET_TOKEN_16                      0xA1CB0009     ///< ID for APCB GetToken16
#define CBS_APCB_COMM_ID_SET_TOKEN_16                      0xA1CB000A     ///< ID for APCB SetToken16
#define CBS_APCB_COMM_ID_GET_TOKEN_32                      0xA1CB000B     ///< ID for APCB GetToken32
#define CBS_APCB_COMM_ID_SET_TOKEN_32                      0xA1CB000C     ///< ID for APCB SetToken32
#define CBS_APCB_COMM_ID_GET_TYPE                          0xA1CB000D     ///< ID for APCB GetType
#define CBS_APCB_COMM_ID_SET_TYPE                          0xA1CB000E     ///< ID for APCB SetType
#define CBS_APCB_COMM_ID_PURGE_ALL_TOKENS                  0xA1CB000F     ///< ID for APCB PurgeAllTokens
#define CBS_APCB_COMM_ID_PURGE_ALL_TYPES                   0xA1CB0010     ///< ID for APCB PurgeAllTypes
#define CBS_APCB_COMM_ID_PURGE_TOKENS                      0xA1CB0011     ///< ID for APCB PurgeTokens

#define CBS_APCB_COMM_ID_GET_DRAM_POST_PKG_REPAIR_ENTRY    0xA1CBD301     ///< ID for APCB GetDramPostPkgRepairEntry
#define CBS_APCB_COMM_ID_CLEAR_DRAM_POST_PKG_REPAIR_ENTRY  0xA1CBD302     ///< ID for APCB ClearDramPostPkgRepairEntry
#define CBS_APCB_COMM_ID_ADD_DRAM_POST_PKG_REPAIR_ENTRY    0xA1CBD303     ///< ID for APCB AddDramPostPkgRepairEntry
#define CBS_APCB_COMM_ID_REMOVE_DRAM_POST_PKG_REPAIR_ENTRY 0xA1CBD304     ///< ID for APCB RemoveDramPostPkgRepairEntry


#define APCB_SMM_COMM_NO_PARAMETER_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMMUNICATION_CMN))

/// APCB SMM communication common buffer
typedef struct _APCB_SMM_COMMUNICATION_CMN {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
} APCB_SMM_COMMUNICATION_CMN;


#define APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER))

/// APCB SMM communication access config buffer
typedef struct _APCB_SMM_ACCESS_CONFIG_PARAMETER {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
  UINT16                       TokenId;
  UINT32                       SizeInByte;
  UINT64                       Value;
} APCB_SMM_COMM_ACCESS_CONFIG_PARAMETER;

#define APCB_SMM_COMM_ACCESS_TOKEN_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_ACCESS_TOKEN))

/// APCB SMM communication access token buffer
typedef struct _APCB_SMM_COMM_ACCESS_TOKEN {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
  UINT8                        Purpose;
  UINT32                       Uid;
  UINT32                       Value;
} APCB_SMM_COMM_ACCESS_TOKEN;

#define APCB_SMM_COMM_ACCESS_TYPE_BUFFER_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_ACCESS_TYPE))

/// APCB SMM communication access type buffer
typedef struct _APCB_SMM_COMM_ACCESS_TYPE {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
  UINT8                        Purpose;
  UINT16                       GroupId;
  UINT16                       TypeId;
  UINT16                       InstanceId;
  UINT32                       DataSize;
  UINT8                        DataBuffer[2048];
} APCB_SMM_COMM_ACCESS_TYPE;

#define APCB_SMM_COMM_PURGE_TOKENS_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_PURGE_TOKENS))

/// APCB SMM communication purge tokens
typedef struct _APCB_SMM_COMM_PURGE_TOKENS {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
  UINT8                        Purpose;
  UINT32                       *ApcbTokenList;
  UINT32                       TokenCount;
} APCB_SMM_COMM_PURGE_TOKENS;

#define APCB_SMM_COMM_PURGE_ALL_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_PURGE_ALL))

/// APCB SMM communication purge all buffer
typedef struct _APCB_SMM_COMM_PURGE_ALL {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
  UINT8                        Purpose;
} APCB_SMM_COMM_PURGE_ALL;


#define APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY))

/// APCB SMM communication purge all buffer
typedef struct _APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY {
  UINT32                       id;         ///< ID of smm communication buffer
  EFI_STATUS                   Status;     ///< Return status
  DPPR_REPAIR_ENTRY_V2         *Entry;
  UINT32                       NumOfEntries; ///< For GetDramPostPkgRepairEntry
} APCB_SMM_COMM_ACCESS_DRAM_POST_PKG_REPAIR_ENTRY;

#endif // _AMD_APCB_SMM_COMMUNICATION_H_

