/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _APCB_MEM_DRAM_DQ_PIN_MAP_H_
#define _APCB_MEM_DRAM_DQ_PIN_MAP_H_
//
// @APCB_START
//


typedef enum {
  MEM_MX_DATA_00,
  MEM_MX_DATA_01,
  MEM_MX_DATA_02,
  MEM_MX_DATA_03,
  MEM_MX_DATA_04,
  MEM_MX_DATA_05,
  MEM_MX_DATA_06,
  MEM_MX_DATA_07,
  MEM_MX_DATA_08,
  MEM_MX_DATA_09,
  MEM_MX_DATA_10,
  MEM_MX_DATA_11,
  MEM_MX_DATA_12,
  MEM_MX_DATA_13,
  MEM_MX_DATA_14,
  MEM_MX_DATA_15,
  MEM_MX_DATA_16,
  MEM_MX_DATA_17,
  MEM_MX_DATA_18,
  MEM_MX_DATA_19,
  MEM_MX_DATA_20,
  MEM_MX_DATA_21,
  MEM_MX_DATA_22,
  MEM_MX_DATA_23,
  MEM_MX_DATA_24,
  MEM_MX_DATA_25,
  MEM_MX_DATA_26,
  MEM_MX_DATA_27,
  MEM_MX_DATA_28,
  MEM_MX_DATA_29,
  MEM_MX_DATA_30,
  MEM_MX_DATA_31,
} MEM_32B_PIN_NAME;

#define MEM_BYTE_LANE_PIN_MAX    (8)

typedef struct {
  UINT8 DramDqLn0[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn1[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn2[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn3[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn4[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn5[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn6[MEM_BYTE_LANE_PIN_MAX];
  UINT8 DramDqLn7[MEM_BYTE_LANE_PIN_MAX];
} DRAM_DQ_PIN_MAP;

typedef enum {
  MEM_MX_CA_00,
  MEM_MX_CA_01,
  MEM_MX_CA_02,
  MEM_MX_CA_03,
  MEM_MX_CA_04,
  MEM_MX_CA_05,
  MEM_MX_CA_06,
  MEM_MX_CA_07,
  MEM_MX_CA_08,
  MEM_MX_CA_09,
  MEM_MX_CA_10,
  MEM_MX_CA_11,
  MEM_MX_CA_12,
  MEM_MX_CA_13,
  MEM_MX_CA_NC = 0xFF,
} MEM_CA_PIN_NAME;

#define MEM_CA_LANE_PIN_MAX      (7)

typedef struct {
  UINT8 DramCaLn0[MEM_CA_LANE_PIN_MAX];
  UINT8 DramCaLn1[MEM_CA_LANE_PIN_MAX];
} DRAM_CA_PIN_MAP;

//
// DDR5
//
#define MEM_CA_LANE_PIN_MAX_DDDR5 (14)

typedef struct {
  UINT8 DramCaLn0[MEM_CA_LANE_PIN_MAX_DDDR5];
  UINT8 DramCaLn1[MEM_CA_LANE_PIN_MAX_DDDR5];
} DRAM_CA_PIN_MAP_DDR;

#endif // _APCB_MEM_DRAM_DQ_PIN_MAP_H_



