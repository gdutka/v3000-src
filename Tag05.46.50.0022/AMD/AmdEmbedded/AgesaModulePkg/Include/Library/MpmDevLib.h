/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
//Lib used by MPM modules internally
#ifndef  _MPM_DEV_H_
#define _MPM_DEV_H_
#include <Uefi.h>
#include <Library/MpmFuncLib.h>

#ifndef KVM_NOT_REQUESTED
  #define KVM_NOT_REQUESTED 0x00000000ul
#endif

#ifndef KVM_REQUESTED
  #define KVM_REQUESTED 0x00000001ul
#endif

#ifndef TCR_NOT_REQUESTED
  #define TCR_NOT_REQUESTED 0x00000000ul
#endif

#ifndef TCR_REQUESTED
  #define TCR_REQUESTED 0x00000001ul
#endif

typedef struct {
  UINT32                                    NetCardBar0BaseLo;
  UINT32                                    NetCardBar0BaseHi;
  UINT32                                    NetCardBar2BaseLo;
  UINT32                                    NetCardBar2BaseHi;
  UINT32                                    NetCardDidVid;
  UINT32                                    NetCardClassRevID;
} KVM_NETCARD_INFO;

typedef
BOOLEAN
(EFIAPI *MPM_DEV_IS_SUPPORTED) (
  IN     EFI_HANDLE         *KvmDevHandle
  );

typedef
EFI_STATUS
(EFIAPI *MPM_DEV_GET_MMIO_BASE) (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioBase
  );

typedef
EFI_STATUS
(EFIAPI *MPM_DEV_GET_MMIO_SIZE) (
  IN     EFI_HANDLE           *KvmDevHandle,
  IN OUT UINT32               *NetCardMmioSize
  );

typedef
EFI_STATUS
(EFIAPI *MPM_DEV_REG_FIELD_ACCESS) (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

typedef
EFI_STATUS
(EFIAPI *MPM_DEV_GET_NETCARD_BAR_BASE) (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  );

typedef enum {
  MPM_NIC_WIRELESS   = 0x1,               ///< Wireless KVM
  MPM_NIC_WIRED      = 0x2,               ///< Wired KVM
} MPM_NIC_DEV_TYPE;

typedef struct {
  MPM_NIC_DEV_TYPE              DeviceType;
  MPM_DEV_IS_SUPPORTED              IsSupported;
  MPM_DEV_GET_MMIO_BASE         GetMmioBase;
  MPM_DEV_GET_MMIO_SIZE         GetMmioSize;
  MPM_DEV_REG_FIELD_ACCESS          ReadKvmRequest;
  MPM_DEV_REG_FIELD_ACCESS          WriteKvmRequest;
  MPM_DEV_REG_FIELD_ACCESS          ReadKvmSessionProgress;
  MPM_DEV_REG_FIELD_ACCESS          WriteKvmSessionProgress;
  MPM_DEV_GET_NETCARD_BAR_BASE  GetNetCardBarBase;
  MPM_DEV_REG_FIELD_ACCESS          ReadTcrRequest;
} MPM_DEV_PROTOCOL;

typedef union {
  struct {
    UINT32  Reserved_23_0:24;       ///< [23:0] Reserved
    UINT32  KvmRequest:1;           ///< [24] KvmRequest
    UINT32  Reserved_27_25:3;       ///< [27:25] Reserved
    UINT32  KvmSessionProgress:1;   ///< [28] KvmSessionProgress
    UINT32  Reserved_31_29:3;       ///< [31:29] Reserved
  } BrcmField; // UINT32
  struct {
    UINT8   KvmRequest:1;           ///< [0] KVMRequest
    UINT8   KvmSessionProgress:1;   ///< [1] KvmSessionProgress
    UINT8   Reserved_7_2:6;         ///< [7:2] KvmRequest
  } RtkField; // UINT8
  struct {
    UINT32   KvmRequest:1;           ///< [0] KVMRequest
    UINT32   KvmSessionProgress:1;   ///< [1] KvmSessionProgress
    UINT32   Reserved_31_2:30;       ///< [31:2] Reserved
  } AquantiaField; // UINT32
  UINT32 Value;
} KVM_STATUS_REG;

extern MPM_DEV_PROTOCOL mAquantiaProtocol;
extern MPM_DEV_PROTOCOL mRtk8168F_Protocol;
extern MPM_DEV_PROTOCOL mRtk8168E_Protocol;
extern MPM_DEV_PROTOCOL mBrcm5762Protocol;
extern MPM_DEV_PROTOCOL mQcaHastingsProtocol;
extern MPM_DEV_PROTOCOL mMediatekProtocol;

extern MPM_DEV_PROTOCOL * mMpmDevProtocols [];
/**
 * @brief Get the Pci Bar Base value
 *
 * @param PciHandle PciIoProtocol handle
 * @param Offset    Offset of BAR register in PCI CFG space
 * @param PciBarBase point to PCI BAR value
 * @return EFI_STATUS
 */
EFI_STATUS
GetPciBarBase (
  IN     EFI_HANDLE         *PciHandle,
  IN     UINT32             Offset,
  IN OUT UINT64             *PciBarBase
  );

/**
 * @brief Get the Nic Command PCI register
 *
 * @param KvmDevHandle
 * @param Command
 * @return EFI_STATUS
 */
EFI_STATUS
GetNicCommand (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT16             *Command
  );

/**
 * @brief Set the Nic Command PCI register
 *
 * @param KvmDevHandle
 * @param Command
 * @return EFI_STATUS
 */
EFI_STATUS
SetNicCommand (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT16             *Command
  );

UINT32
GetMpmDevProtocolNum (
  );
#endif //_MPM_DEV_H_
