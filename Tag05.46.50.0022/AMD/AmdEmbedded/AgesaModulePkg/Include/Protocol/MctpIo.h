/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/

#ifndef _MCTP_IO_H
#define _MCTP_IO_H

#include "Mctp.h"

typedef struct _DASH_MCTP_IO_PROTOCOL DASH_MCTP_IO_PROTOCOL;

#define DASH_MCTP_IO_PROTOCOL_GUID \
  {0xf375d9f0, 0x4ec6, 0x491d, {0x91, 0x86, 0x32, 0x34, 0x68, 0xf3, 0x48, 0x55}}

typedef
EFI_STATUS
(EFIAPI *MCTP_SEND_MSG) (
  IN DASH_MCTP_IO_PROTOCOL  *This,
  IN UINT32                 MessageDataSize,
  IN CONST MCTP_MSG         *MessageData,
  OUT UINT32                *ReplySize,
  OUT MCTP_MSG              *Reply
  );

typedef struct _DASH_MCTP_IO_PROTOCOL {
  UINT32        Size;
  MCTP_SEND_MSG SendMessage;
} DASH_MCTP_IO_PROTOCOL;

extern EFI_GUID gDashMctpIoProtocolGuid;
#endif
