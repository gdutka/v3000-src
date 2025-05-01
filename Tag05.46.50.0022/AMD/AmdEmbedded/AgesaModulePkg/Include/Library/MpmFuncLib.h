/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
//Lib used by MPM modules internally
#ifndef  _MPM_FUNC_H_
#define _MPM_FUNC_H_
#include <Uefi.h>

#define MPM_PRVIATE_HEAP_DATA_DFT_32     0xFFFFFFFFul
#define MPM_PRVIATE_HEAP_DATA_DFT_64     0xFFFFFFFFul

#define MPM_WLAN_TEMP_BUF_SIZE   (2 * 1024 * 1024)  //2M bytes
#define MPM_DMA_HPA_BUF_SIZE   (64 * 1024 * 1024)  //64M bytes
#define MPM_DRAM_TOTAL_SIZE    (16 * 1024 * 1024) //16M bytes
#define MPM_C2PMSG_2_SIZE     4 //4 Bytes
#define MPM_C2PMSG_2_OFFSET   0x10508

//Bit field for ManageabilityConfig
// #define BIOS2PSP_MPM_WM_FUSE_ENABLE_SHIFT  0  // "Manageability Enabled by Fuse" bit
// #define BIOS2PSP_MPM_WM_FUSE_ENABLE_SIZE  1
// #define BIOS2PSP_MPM_WM_FUSE_ENABLE_MASK  (BIOS2PSP_MPM_WM_FUSE_ENABLE_SIZE << BIOS2PSP_MPM_WM_FUSE_ENABLE_SHIFT)
// #define BIOS2PSP_MPM_AIM_T_SHIFT     1  // AIM-T bit
// #define BIOS2PSP_MPM_AIM_T_SIZE      1
// #define BIOS2PSP_MPM_AIM_T_MASK      (BIOS2PSP_MPM_AIM_T_SIZE << BIOS2PSP_MPM_AIM_T_SHIFT)
#define  AIM_T_CAPABLE_MASK    0x3

#define     MPM_COMMAND_IN_PROGRESS         0x55555555
#define     MPM_COMMAND_IDLE                0xEEEEEEEE

typedef struct _MPM_PRVIATE_HEAP_DATA_V1 {
  UINT32              Version;                  // 1 version 1
  BOOLEAN             WirelessTxrPending;       // Wireless Text Redirection task is pending
  BOOLEAN             WirelessKvmPending;       // Wireless KVM task is pending
  UINT32              ManageabilityConfig;      // Manaeability Configuration and event word
  UINT64              MpmDramAddr;              // 64-bit address for reserved MPM DRAM region
  UINT64              PldmBufferAddr;           // Address of buffer used to send PLDM message
  UINT32              PldmBufferOffset;         // Offset to MpmDramAddr for sending PLDM message
  UINT32              PldmBufferSize;           // Size of PLDM Buffer
  UINT64              AlertBufferAddr;          // Address of buffer used to send Alert message
  UINT32              AlertBufferOffset;        // Offset to MpmDramAddr for sending Alert message
  UINT32              AlertBufferSize;          // Size of Alert Buffer
  UINT64              TextBufferAddr;           // Address of buffer used to send Text message
  UINT32              TextBufferOffset;         // Offset to MpmDramAddr for sending Text message
  UINT32              TextBufferSize;           // Size of Text Buffer
  UINT64              KvmBufferAddr;            // Address of buffer used to send KVM message
  UINT32              KvmBufferOffset;          // Offset to MpmDramAddr for sending Kvm message
  UINT32              KvmBufferSize;            // Size of Kvm Buffer
  UINT64              TextTxBufferAddr;         // Tx buffer address for text redirection
  UINT32              TextTxBufferSize;         // Tx buffer address for text redirection
  UINT64              TextRxBufferAddr;         // Rx buffer address for text redirection
  UINT32              TextRxBufferSize;         // Rx buffer address for text redirection
  UINT64              CmnBufferAddr;            // Address of buffer used to send Non-PLDM,ASF message
  UINT32              CmnBufferOffset;          // Offset to MpmDramAddr for sending Non-PLDM,ASF message
  UINT32              CmnBufferSize;            // Size of Non-PLDM,ASF Buffer
  UINT32              TcrDidVid;           // DID and VID mask of the WLAN; Set by AMF if TextPending is true; This is needed                 // for BIOS to send BDF for the appropriate WLAN chip
  UINT32              TcrBufOffset; // Offset of the start of TCR_RING_BUFFER_T buffer
  UINT32              TcrBufSize;    // Size TCR_RING_BUFFER_T buffer
  UINT64              TcrBufAddr; // Address of the start of TCR_RING_BUFFER_T buffer
  UINT32              KvmDidVid;    //DID and VID mask of the WLAN; Set by AMF if KvmPending is true; This is        needed for BIOS to send BDF for the appropriate WLAN chip
  UINT32              KvmKeyboardBufOffset;
  UINT32              KvmKeyboardBufSize;
  UINT64              KvmKeyboardBufAddr;
  UINT32              KvmMouseBufOffset;
  UINT32              KvmMouseBufSize;
  UINT64              KvmMouseBufAddr;
  UINT64              WirelessDmaBufAddr; //Allocated Buffer for MPM when KVM or TCR pending detected
  UINT32              MpmCommandInProgressFlag;  //Flag to indicate Mpm Command is in progress to avoid Mpm Command re-enter
  UINT32              WirelessKvmFlag; //WirelessKvm requested 1: WirelessKvm Pending 0: WiredKVM request
} MPM_PRVIATE_HEAP_DATA_V1;


/**
  Locate Mpm PrivateData, the data is stored in AGESA HEAP
  * @param[in,out]  Data  Point to the MPM global prviate data

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
LocateMpmPrivateData (
  MPM_PRVIATE_HEAP_DATA_V1 **Data
  );

/**
  Initial Mpm PrivateData, the data is stored in AGESA HEAP
  * @param[in,out]  Data  Point to the MPM global prviate data

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
InitMpmPrivateData (
  MPM_PRVIATE_HEAP_DATA_V1 **Data
  );

#endif //_MPM_FUNC_H_
