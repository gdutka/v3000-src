/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#ifndef _NBIOSMUV13LIB_H_
#define _NBIOSMUV13LIB_H_

//
// Assume standard IA-32 alignment.
//
#ifndef SMU_DATA_TYPES_H
#define SMU_DATA_TYPES_H
  typedef unsigned long long  uint64_t;
  typedef long long           int64_t;
  typedef unsigned int        uint32_t;
  typedef int                 int32_t;
  typedef unsigned short      uint16_t;
  typedef short               int16_t;
  typedef unsigned char       uint8_t;
  typedef signed char         int8_t;
#endif

#define INVALID_SMU_MESSAGE                  0xFF

// SMU Response Codes:
#define BIOSSMC_Result_OK                    0x1
#define BIOSSMC_Result_Failed                0xFF
#define BIOSSMC_Result_UnknownCmd            0xFE
#define BIOSSMC_Result_CmdRejectedPrereq     0xFD
#define BIOSSMC_Result_CmdRejectedBusy       0xFC

#define INVALID_SMU_MESSAGE                  0xFF

#ifndef BIOSSMC_Message_Count                  //For SmuV13 Common Lib
  #define BIOSSMC_Message_Count                0x6D
  #define BIOSSMC_MSG_TestMessage              0x1
  #define BIOSSMC_MSG_DxioTestMessage          0x9
  #define BIOSSMC_MSG_SleepEntry               0xB
#endif


typedef struct {
  uint16_t Frequency; //MHz
  uint16_t LogicalCoreIndex;
} BIOSSMC_MSG_SetBoostLimitFrequency_t;

typedef union {
  uint32_t val;
  BIOSSMC_MSG_SetBoostLimitFrequency_t f;
} BIOSSMC_MSG_SetBoostLimitFrequency_u;

typedef enum {
  CLUSTER_ON_DIE_MODE_COD_1,
  CLUSTER_ON_DIE_MODE_COD_1_2,
  CLUSTER_ON_DIE_MODE_COD_1_2_4
} CLUSTER_ON_DIE_MODE_e;

// Address
#define MP1_C2PMSG_MESSAGE_ADDRESS                         0x3B10528ul

// Address
#define MP1_C2PMSG_RESPONSE_ADDRESS                        0x3b10578UL

// Address
#define MP1_C2PMSG_ARGUMENT_0_ADDRESS                      0x3B10998ul
#define MP1_C2PMSG_ARGUMENT_1_ADDRESS                      0x3B1099Cul
#define MP1_C2PMSG_ARGUMENT_2_ADDRESS                      0x3B109A0UL
#define MP1_C2PMSG_ARGUMENT_3_ADDRESS                      0x3B109A4UL
#define MP1_C2PMSG_ARGUMENT_4_ADDRESS                      0x3B109A8ul
#define MP1_C2PMSG_ARGUMENT_5_ADDRESS                      0x3B109ACul

/// SMU TDP contect
typedef struct {
  UINT32            SmuTdp;        ///< TDP returned by SMU
  UINT32            cTdp;          ///< Config TDP
} SMU_TDP_INFO;

/// SMU V13 basic lib

VOID
NbioSmuServiceCommonInitArgumentsV13 (
  IN OUT   UINT32                   *SmuArg
  );

UINT32
NbioSmuServiceRequestV13 (
  IN       PCI_ADDR                 NbioPciAddress,
  IN       UINT32                   RequestId,
  IN OUT   UINT32                   *RequestArgument,
  IN       UINT32                   AccessFlags
  );

UINT32
NbioSmuServiceRequestOnlyV13 (
  IN       PCI_ADDR                 NbioPciAddress,
  IN       UINT32                   RequestId,
  IN OUT   UINT32                   *RequestArgument,
  IN       UINT32                   AccessFlags
  );

AGESA_STATUS
NbioSmuFirmwareTestV13 (
  IN       GNB_HANDLE               *NbioHandle
  );

AGESA_STATUS
NbioDxioServiceRequestV13 (
  IN       GNB_HANDLE               *NbioHandle,
  IN       UINT32                   RequestId,
  IN OUT   UINT32                   *RequestArgument,
  IN       UINT32                   AccessFlags
  );

VOID
SmuNotifyS3Entry (
  );

#endif
