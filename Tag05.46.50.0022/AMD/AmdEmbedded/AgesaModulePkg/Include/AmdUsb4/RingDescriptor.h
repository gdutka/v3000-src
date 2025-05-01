/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* @file
  Constants definitions for Usb4 Control Packet.
*/

#ifndef _AMD_USB4_RING_DESCRIPTOR_H
#define _AMD_USB4_RING_DESCRIPTOR_H

#define DESCRIPTOR_TIMEOUT_US 1000  //TODO - to decide on descriptor wait time

#pragma  pack (push, 1)

//
//  Transmit Bits of Descriptor
//
typedef struct {
  UINT32      DataLength:12;
  UINT32      EOFPDF:4;
  UINT32      SOFPDF:4;
  UINT32      Rsvd:1;
  UINT32      DescriptorDone:1;
  UINT32      RequestStatus:1;
  UINT32      InterruptEnable:1;
  UINT32      Offset:8;
} AMD_USB4_TRANSMIT_DESCRIPTOR_DATA;

//
//  A Receive Bits of Descriptor from Host Memory
//
typedef struct {
  UINT32      Rsvd:21;
  UINT32      DescriptorDone:1;
  UINT32      RequestStatus:1;
  UINT32      InterruptEnable:1;
  UINT32      Offset:8;
} AMD_USB4_RECIEVE_DESCRIPTOR_DATA_HOST;

//
//  A Receive Bits of Descriptor from Host By Adapter layer
//
typedef struct {
  UINT32      DataLength:12;
  UINT32      EOFPDF:4;
  UINT32      SOFPDF:4;
  UINT32      CRCError:1;
  UINT32      DescriptorDone:1;
  UINT32      BufferOverflow:1;
  UINT32      InterruptEnable:1;
  UINT32      Offset:8;
} AMD_USB4_RECIEVE_DESCRIPTOR_DATA_HOST_ADAPTER_LAYER;

typedef union {
  UINT32                                                 Dword;
  AMD_USB4_TRANSMIT_DESCRIPTOR_DATA                      TxDesc;
  AMD_USB4_RECIEVE_DESCRIPTOR_DATA_HOST                  RxHostDesc;
  AMD_USB4_RECIEVE_DESCRIPTOR_DATA_HOST_ADAPTER_LAYER    RxHostAdaptLayerDesc;
} AMD_USB4_DESCRIPTOR_DATA;

//
// USB4 Descriptor structure
//
typedef struct {
  UINT32                             AddressLow;
  UINT32                             AddressHigh;
  AMD_USB4_DESCRIPTOR_DATA           DescriptorData;
  UINT32                             Rsvd;
} AMD_USB4_DESCRIPTOR;

#pragma  pack (pop)

#endif
