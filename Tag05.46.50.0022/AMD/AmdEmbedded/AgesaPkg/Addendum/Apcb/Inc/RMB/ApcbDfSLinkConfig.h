/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _APCB_DF_SLINK_CONFIG_H_
#define _APCB_DF_SLINK_CONFIG_H_

//
// @APCB_START
//

//
// @CONSTANT
//
#define MAX_SLINK_REGIONS  4

typedef struct _SLINK_REGION_DESCRIPTOR {
  UINT64   Size;          ///< Raw size.  A value of zero means the entry should be ignored.
  UINT8    Alignment;     ///< Required alignment.
  UINT8    Socket;        ///< Zero means socket 0, One means socket 1
  UINT8    PhysNbioMap;   ///< Bit n set means NBIO n should be included in this region.  Invalid values on Rome are 0, 5, 7, 0xA, 0xB, 0xD, 0xE, and 0xF.
  UINT8    IntlvSize;     ///< see DF_MEM_INTLV_SIZE_VALIDVAL
} SLINK_REGION_DESCRIPTOR;


/** SLINK_CONFIG
*    @GROUPID: APCB_GROUP_DF
*    @TYPEID: APCB_DF_TYPE_SLINK_CONFIG
*    This data structure is used to S-link region info
*/
typedef struct _SLINK_CONFIG {
  SLINK_REGION_DESCRIPTOR  Region[MAX_SLINK_REGIONS];
} SLINK_CONFIG;

#endif // _APCB_DF_SLINK_CONFIG_H_

