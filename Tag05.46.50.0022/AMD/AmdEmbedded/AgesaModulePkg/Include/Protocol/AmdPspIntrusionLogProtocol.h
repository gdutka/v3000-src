/*****************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Intrusion Log PROTOCOL prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 */
#ifndef _AMD_PSP_INTRUSION_LOG_PROTOCOL_H_
#define _AMD_PSP_INTRUSION_LOG_PROTOCOL_H_

#define INTRUSION_LOG_STATUS_UNKNOWN   0xFF
#define INTRUSION_LOG_STATUS_GOOD      0x0
#define INTRUSION_LOG_STATUS_BAD       0x1

/// Structure for AMD_PSP_INTRUSION_LOG_PROTOCOL protocol
typedef struct  _AMD_PSP_INTRUSION_LOG_PROTOCOL {
  UINT8                           IntrusionLogStatus;        // intrusion log status
} AMD_PSP_INTRUSION_LOG_PROTOCOL;

extern EFI_GUID gAmdPspIntrusionLogProtocolGuid;

#endif //_AMD_PSP_INTRUSION_LOG_PROTOCOL_H_
