/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


/* @file
  Constants definitions for Usb4 Control Packet.
*/

#ifndef _AMD_USB4_CONTROL_PACKET_H
#define _AMD_USB4_CONTROL_PACKET_H

#define MAX_CONTROL_PACKET_PAYLOAD_SIZE_IN_DW   60
#define CONTROL_PACKET_CRC_SIZE_IN_DW           1

#define GET_SIZE_FOR_CRC32(S)                   (sizeof(S)-4)
#define MASK_CM_BIT(A)                          (A & 0x00ffffff)
#define TOPOLOGICID64(Hi,Lo)                    ( (UINT64)( ( ((UINT64)(Hi & 0x00FFFFFF)) << 32 ) | Lo ) )

#define CONTROL_PACKET_DEFAULT_SN     0

#define GET_TOPOLOGY_ID(LVL6, LVL5, LVL4, LVL3, LVL2, LVL1, LVL0) ( \
  ( ( LVL0 & 0x3Full ) <<  0 ) | \
  ( ( LVL1 & 0x3Full ) <<  8 ) | \
  ( ( LVL2 & 0x3Full ) << 16 ) | \
  ( ( LVL3 & 0x3Full ) << 24 ) | \
  ( ( LVL4 & 0x3Full ) << 32 ) | \
  ( ( LVL5 & 0x3Full ) << 40 ) | \
  ( ( LVL6 & 0x3Full ) << 48 ) )

#define UPG_HOT_PLUG          0
#define UPG_HOT_UNPLUG        1

#define PG_NOTIFICATION       0
#define PG_HOT_PLUG_EVENT     2
#define PG_HOT_UNPLUG_EVENT   3

#define PDF_READ_REQUEST          1
#define PDF_READ_RESPONSE         1
#define PDF_WRITE_REQUEST         2
#define PDF_WRITE_RESPONSE        2
#define PDF_NOTIFICATION          3
#define PDF_NOTIFICATION_ACK      4
#define PDF_HOTPLUG_EVENT         5
#define PDF_INTERDOMAIN_REQUEST   6
#define PDF_INTERDOMAIN_RESPONSE  7

typedef enum {
  ERR_CONN  = 0,
  ERR_LINK,
  ERR_ADDR,
  ERR_ADP   = 4,
  HP_ACK    = 7,
  ERR_ENUM  = 8,
  ERR_NUA,
  ERR_LEN   = 11,
  ERR_HEC,
  ERR_FC,
  ERR_PLUG,
  ERR_LOCK
} AMD_USB4_NOTIFICATION_EVENT_ERROR_CODE;

typedef enum {
  CS_TARGET_PATH      = 0,
  CS_TARGET_ADAPTER   = 1,
  CS_TARGET_ROUTER    = 2,
  CS_TARGET_COUNTERS  = 3,
} AMD_USB4_CONFIGURATION_SPACE_TARGET;

#pragma  pack (push, 1)

typedef union {
  UINT32     Value;
  struct {
    UINT32   HdrErrCtrl:8;        //Head error control
    UINT32   Length:8;
    UINT32   HopID:11;
    UINT32   SuppID:1;
    UINT32   ProtocolDefField:4;  //Protocol Defined Field
  } Field;
} AMD_USB4_CONTROL_PACKET_HEADER;


typedef  union {
  UINT32     Value;
  struct {
    UINT32   Address:13;
    UINT32   Size:6;
    UINT32   AdapterNum:6;
    UINT32   ConfigurationSpace:2;
    UINT32   SequenceNumber:2;
    UINT32   Rsvd:3;
  } Field;
} AMD_USB4_READ_WRITE_HEADER;


//Connection Manager uses Read Requests to read from a Configuration Space
typedef struct {
  UINT32                           RouteStringHigh;
  UINT32                           RouteStringLow;
  AMD_USB4_READ_WRITE_HEADER       ReadRequest;
  UINT32                           CRC;
} AMD_USB4_READ_REQUEST_PAYLOAD;


//Router uses a Read Response to respond to a Read Request
typedef struct {
  UINT32                           RouteStringHigh;
  UINT32                           RouteStringLow;
  AMD_USB4_READ_WRITE_HEADER       ReadResponse;
  UINT32                           ReadDataAndCRC[61];
} AMD_USB4_READ_RESPONSE_PAYLOAD;


//A Connection Manager uses Write Requests to write to a Configuration Space.
typedef struct {
  UINT32                            RouteStringHigh;
  UINT32                            RouteStringLow;
  AMD_USB4_READ_WRITE_HEADER        WriteRequest;
  UINT32                            WriteDataAndCRC[61];
} AMD_USB4_WRITE_REQUEST_PAYLOAD;


//A Router uses a Write Response to respond to a Write Request
typedef struct {
  UINT32                            RouteStringHigh;
  UINT32                            RouteStringLow;
  AMD_USB4_READ_WRITE_HEADER        WriteResponse;
  UINT32                            CRC;
} AMD_USB4_WRITE_RESPONSE_PAYLOAD;


typedef struct {
  UINT32                   RouteStringHigh;
  UINT32                   RouteStringLow;
  UINT32                   EventCode:8;
  UINT32                   AdapterNum:6;
  UINT32                   Rsvd:16;
  UINT32                   PG:2;    // HotPlug/Unplug status
  UINT32                   CRC;
} AMD_USB4_NOTIFICATION_PACKET_PAYLOAD;


typedef struct {
  UINT32                            RouteStringHigh;
  UINT32                            RouteStringLow;
  UINT32                            CRC;
} AMD_USB4_NOTIFICATION_ACK_PACKET_PAYLOAD;


//
//  Hotplug event packet is used by a Router to notify
//  a Connection Manager that a Hot Plug or Hot Unplug
//  Event has occurred
//
typedef struct {
  UINT32                            RouteStringHigh;
  UINT32                            RouteStringLow;
  UINT32                            AdapterNum:6;
  UINT32                            Rsvd:25;
  UINT32                            UPG:1;
  UINT32                            CRC;
} AMD_USB4_HOTPLUG_EVENT_PACKET_PAYLOAD;


//
//  An Inter-Domain Request is used for Inter-Domain
//  communication between two Connection Managers.
//
typedef struct {
  UINT32                            RouteStringHigh;
  UINT32                            RouteStringLow;
  UINT32                            InterDomainData[60];
  UINT32                            CRC;
} AMD_USB4_INTER_DOMAIN_REQUEST_PAYLOAD;


typedef struct {
  AMD_USB4_CONTROL_PACKET_HEADER    Hdr;
  UINT32                            RouteStringHigh;
  UINT32                            RouteStringLow;
  UINT32                            InterDomainData[60];
  UINT32                            CRC;
} AMD_USB4_INTER_DOMAIN_RESPONSE_PAYLOAD;

#pragma  pack (pop)

#endif
