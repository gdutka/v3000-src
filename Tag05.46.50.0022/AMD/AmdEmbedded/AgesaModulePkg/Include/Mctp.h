/*****************************************************************************
 * Copyright 2022 Advanced Micro Devices, Inc. All rights reserved.
 *****************************************************************************
*/

#ifndef _MCTP_H
#define _MCTP_H

#define MCTP_MSG_HDR_VER_1  0x01
#define MCTP_MSG_SOM        0x80
#define MCTP_MSG_EOM        0x40
#define MCTP_MSG_SOM_EOM    (MCTP_MSG_SOM|MCTP_MSG_EOM)
#define MCTP_MSG_MIDDLE     0x00
#define MCTP_MSG_TAG_OWNER  0x08

#define IsMctpSom(a)    ((((a)->Flags) & MCTP_MSG_SOM) == (MCTP_MSG_SOM))
#define IsMctpEom(a)    ((((a)->Flags) & MCTP_MSG_EOM) == (MCTP_MSG_EOM))
#define IsMctpSingle(a) ((((a)->Flags) & (MCTP_MSG_SOM_EOM)) == (MCTP_MSG_SOM_EOM))
#define IsMctpMiddle(a) ((((a)->Flags) & (MCTP_MSG_SOM_EOM)) == (MCTP_MSG_MIDDLE))

typedef struct _MCTP_MSG {
  UINT8 SlaveAddress;
  UINT8 MctpVersion;                  // [0:3] = Version, [4:7] = Reserved.
  UINT8 DestEid;                      // Destination Endpoint.
  UINT8 SrcEid;                       // Source Endpoint.
  UINT8 Flags;
} MCTP_MSG;

typedef struct _MCTP_MSG_TYPE {
  UINT8     MctpMsgType;
} MCTP_MSG_TYPE;

typedef struct _MCTP_PLDM_MSG {
  MCTP_MSG        MctpMsg;
  MCTP_MSG_TYPE   MsgType;
} MCTP_PLDM_MSG;

#define MCTP_MSG_SIZE                    sizeof(MCTP_MSG)
#define MCTP_SOURCE_SLAVE_ADDR           0x71
#define MCTP_SMBUS_COMMAND_CODE          0x0F
#define MCTP_SMBUS_BLOCK_LENGTH          0x20
#define MCTP_SMBUS_TRANSFER_DATA_LENGTH  (MCTP_SMBUS_BLOCK_LENGTH - MCTP_MSG_SIZE)
#define MCTP_PLDM_TYPE                   0x01

#endif