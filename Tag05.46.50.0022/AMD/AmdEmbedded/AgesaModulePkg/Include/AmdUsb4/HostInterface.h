/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include "AmdUsb4.h"

#ifndef _AMMD_USB4_HOSTINTERFACE_LIB_H
#define _AMMD_USB4_HOSTINTERFACE_LIB_H

//
// Host Interface Control Registers
//
#define HI_CAPABILITIES                       0x39640   // Host Interface Capabilities
#define HI_RESET                              0x39858   // Host Interface Reset
#define HI_CONTROL                            0x39864   // Host Interface Control
#define HI_CL1_ENABLE                         0x39880   // Host Interface CL1 Enable
#define HI_CL2_ENABLE                         0x39884   // Host Interface CL2 Enable

//
// Transmit Descriptor Ring Registers
//
#define TX_BASE_ADDRESS_LOW(n)          (0x00000 + 0x10 * n)    // Transmit Descriptor Ring Base Address Low
#define TX_BASE_ADDRESS_HIGH(n)         (0x00004 + 0x10 * n)    // Transmit Descriptor Ring Base Address High
#define TX_PRODUCER_CONSUMER_INDEXES(n) (0x00008 + 0x10 * n)    // Transmit Descriptor Ring Producer and Consumer Index
#define TX_RING_SIZE(n)                 (0x0000C + 0x10 * n)    // Transmit Descriptor Ring Size
#define TX_RING_CONTROL(n)              (0x19800 + 0x10 * n)    // Transmit Ring Control Register

//
// Recieve Descriptor Ring Registers
//
#define RX_BASE_ADDRESS_LOW(n)           (0x08000 + 0x10 * n)    // Receive Descriptor Ring Base Address Low
#define RX_BASE_ADDRESS_HIGH(n)          (0x08004 + 0x10 * n)    // Receive Descriptor Ring Base Address High
#define RX_PRODUCER_CONSUMER_INDEXES(n)  (0x08008 + 0x10 * n)    // Receive Descriptor Ring Producer and Consumer Index
#define RX_RING_AND_BUFFER_SIZE(n)       (0x0800C + 0x10 * n)    // Receive Descriptor Ring Size
#define RX_RING_CONTROL(n)               (0x29800 + 0x20 * n)    // Receive Ring Control Register
#define RX_PDF_BIT_MASKS(n)              (0x29804 + 0x20 * n)    // PDF Bit mask register

#pragma  pack (push, 1)

typedef struct {
  UINT32 TotalPaths:11;
  UINT32 Rsvd:21;
} AMD_USB4_HOST_INTERFACE_CAPABILITIES_REGISTER;

typedef struct {
  UINT32 RST:1;
  UINT32 Rsvd:31;
} AMD_USB4_HOST_INTERFACE_RESET_REGISTER;

typedef struct {
  UINT32 Rsvd:17;
  UINT32 DisableISRAutoClear :1;
  UINT32 Rsvd1:14;
} AMD_USB4_HOST_INTERFACE_CONTROL_REGISTER;

typedef struct {
  UINT32 HI_CL1_Enable_bits;
} AMD_USB4_HOST_INTERFACE_CL1_ENABLE;

typedef struct {
  UINT32 HI_CL2_Enable_bits;
} AMD_USB4_HOST_INTERFACE_CL2_ENABLE;

typedef union {
  UINT32 Index;
  struct {
    UINT16 Consumer_Index;
    UINT16 Producer_Index;
  } Consumer_Producer_Index;
} AMD_USB4_DESCRIPTOR_INDEX;

#define RAW_MODE_ENABLE                              TRUE
#define RAW_MODE_DISABLE                             FALSE
#define E2E_FLOW_ENABLE                              TRUE
#define E2E_FLOW_DISABLE                             FALSE
#define NO_SNOOP_ENABLE                              TRUE
#define NO_SNOOP_DISABLE                             FALSE

typedef union {
  UINT32 Data32;
  struct {
    UINT32 Rsvd:28;
    UINT32 E2E_Flow_Control_Enable:1;
    UINT32 NS:1;
    UINT32 RAW:1;
    UINT32 Ring_Valid:1;
  } Ring_Control_Register;
} AMD_USB4_TX_RING_CONTROL_REGISTER;

typedef union {
  UINT32 Data32;
  struct {
    UINT32 Rsvd:12;
    UINT32 TxE2EHopId:11;
    UINT32 Rsvd1:5;
    UINT32 E2E_Flow_Control_Enable:1;
    UINT32 NS:1;
    UINT32 RAW:1;
    UINT32 Ring_Valid:1;
  } Rx_Ring_Control_Register;
} AMD_USB4_RX_RING_CONTROL_REGISTER;

typedef struct {
  UINT32                    Address_Low;
  UINT32                    Address_High;
  AMD_USB4_DESCRIPTOR_INDEX DescIndex;
  UINT32                    RingSize : 16;
  UINT32                    Rsvd     : 16;
} AMD_USB4_TX_DESCRIPTOR_RING;

typedef struct {
  UINT32                            Address_Low;
  UINT32                            Address_High;
  AMD_USB4_DESCRIPTOR_INDEX         DescIndex;
  UINT32                            RingSize       : 16;
  UINT32                            DataBufferSize : 12;
  UINT32                            Rsvd           : 4;
} AMD_USB4_RX_DESCRIPTOR_RING;

#pragma  pack (pop)

#endif
