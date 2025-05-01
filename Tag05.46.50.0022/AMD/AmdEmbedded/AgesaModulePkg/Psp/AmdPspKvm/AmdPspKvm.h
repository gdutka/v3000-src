/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_PSP_KVM_H_
#define _AMD_PSP_KVM_H_

#include <IndustryStandard/Pci22.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/SimplePointer.h>
#include <Protocol/AbsolutePointer.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>


#include <AGESA.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Protocol/AmdPspKvmServiceProtocol.h>


#include "AmdPspKvmKeyboard.h"
#include "AmdPspKvmMouse.h"

#define AMD_PSP_KVM_DEV_SIGNATURE       SIGNATURE_32 ('p', 'k', 'v', 'm')


typedef struct {
  UINT32                                    NetCardBar0BaseLo;
  UINT32                                    NetCardBar0BaseHi;
  UINT32                                    NetCardBar2BaseLo;
  UINT32                                    NetCardBar2BaseHi;
  UINT32                                    NetCardDidVid;
  UINT32                                    NetCardClassRevID;
} KVM_NETCARD_INFO;

typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_DEV_GET_MMIO_BASE) (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
  );

typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_DEV_GET_MMIO_SIZE) (
  IN     EFI_HANDLE           *KvmDevHandle,
  IN OUT UINT32               *NetCardMmioSize
  );

typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_REG_FIELD_ACCESS) (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

typedef
EFI_STATUS
(EFIAPI *AMD_PSP_KVM_DEV_GET_NETCARD_BAR_BASE) (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  );

typedef struct {
  AMD_PSP_KVM_DEV_GET_MMIO_BASE         GetMmioBase;
  AMD_PSP_KVM_DEV_GET_MMIO_SIZE         GetMmioSize;
  AMD_PSP_KVM_REG_FIELD_ACCESS          ReadKvmRequest;
  AMD_PSP_KVM_REG_FIELD_ACCESS          WriteKvmRequest;
  AMD_PSP_KVM_REG_FIELD_ACCESS          ReadKvmSessionProgress;
  AMD_PSP_KVM_REG_FIELD_ACCESS          WriteKvmSessionProgress;
  AMD_PSP_KVM_DEV_GET_NETCARD_BAR_BASE  GetNetCardBarBase;
} KVM_DEV_PROTOCOL;

typedef struct {
  UINT32                                    VendorID;
  UINT32                                    DeviceID;
  UINT32                                    ClassRevID;
  KVM_DEV_PROTOCOL                          *KvmDevProtocol;
} KVM_DEV_INFO;

typedef struct {
  UINTN                               Signature;
  KVM_KEYBOARD_CONSOLE_IN_DEV         *AmdPspKvmKbDev;
  KVM_MOUSE_DEV                       *AmdPspKvmMsDev;
  EFI_UNICODE_STRING_TABLE            *ControllerNameTable;
  EFI_HANDLE                          *KvmDevHandle;
  KVM_NETCARD_INFO                    NetCardInfo;

  UINT64                              NetCardMmioBase; ///< NetCard PCIE MMIO base
  UINT32                              NetCardMmioSize; ///< NetCard PCIE MMIO base
  UINT32                              KvmRequest;      ///< Kvm Request status
  UINT64                              KvmDmaAddr;      ///< Kvm Dma buffer base
  UINT64                              KvmEntryAddress; ///< Kvm binary entry address
  UINT32                              KvmEntrySize;    ///< Kvm binary size
  KVM_DEV_PROTOCOL                    *KvmDevProtocol;      ///< Kvm Register offset
} AMD_PSP_KVM_DEV;

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

typedef enum {
  KVM_FIELD_REQUEST,
  KVM_FIELD_SESSION
} KVM_REG_FIELD;

/*
 * About the KVM Buffer allocated ,detail as following
 *        ---------------  DMA: 16M
 *       |               |    : OS params etc 5M
 *       |               |    : KvmEngine
 *        ---------------  KB Buffer:32K
 *       |               |
 *        ---------------  MS Buffer:32K
 *       |               |
 *        ---------------  NicCoherency Buffer: 256K
 *       |               |
 *        ---------------
 */
typedef struct {
  UINT32                                    NetCardMmioBaseLo;              ///< NetCard PCIE MMIO base
  UINT32                                    NetCardMmioBaseHi;              ///< NetCard PCIE MMIO base
  UINT32                                    NetCardMmioSize;              ///< NetCard PCIE MMIO base
  UINT32                                    FrameBufferBaseLo;              ///< Frame buffer base
  UINT32                                    FrameBufferBaseHi;              ///< Frame buffer base
  UINT32                                    FrameBufferSize;              ///< Frame buffer size
  UINT32                                    KvmKbBufferAddrLo;            ///< Low 32 Bit of KVM Keyboard Buffer Addr
  UINT32                                    KvmKbBufferAddrHi;            ///< High 32 Bit of KVM Keyboard Buffer Addr
  UINT32                                    KvmKbBufferSize;              ///< KVM Keyboard Buffer Size
  UINT32                                    KvmMsBufferAddrLo;            ///< Low 32 Bit of KVM Mouse Buffer Addr
  UINT32                                    KvmMsBufferAddrHi;            ///< High 32 Bit of KVM Mouse Buffer Addr
  UINT32                                    KvmMsBufferSize;              ///< KVM Mouse Buffer Size
  UINT32                                    KvmNicCoherencyBufferAddrLo;  ///< Low 32 Bit of KVM NicCoherencyBuffer Addr
  UINT32                                    KvmNicCoherencyBufferAddrHi;  ///< High 32 Bit of KVM NicCoherencyBuffer Addr
  UINT32                                    KvmNicCoherencyBufferSize;    ///< KVM NicCoherencyBuffer Size
  UINT32                                    KvmMouseProtocolSelection;    ///< 0: Absolute Pointer Protocol; 1: Simple Pointer Protocol
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION      GModeInfo;                    ///< Graphics mode info
  KVM_IP_INFO                               KvmIpInfo;
  UINT32                                    NetCardBar0BaseLo;
  UINT32                                    NetCardBar0BaseHi;
  UINT32                                    NetCardBar2BaseLo;
  UINT32                                    NetCardBar2BaseHi;
  UINT32                                    NetCardDidVid;
  UINT32                                    NetCardClassRevID;              ///< Class code and RevID mask in PCI config space
  UINT32                                    DMArEnabled;                    ///<set to 1 if enabled; set to 0 if disabled

  UINT32                                    KvmDmaHpaAddrLo;                ///< Low 32 Bit of HPA of KVM DMA Addr
  UINT32                                    KvmDmaHpaAddrHi;                ///< High 32 Bit of HPA of KVM DMA Addr
  UINT32                                    KvmNicCoherentBufferHpaAddrLo;  ///< Low 32 Bit of HPA of KvmNicCoherencyBuffer
  UINT32                                    KvmNicCoherentBufferHpaAddrHi ; ///< High 32 Bit of HPA of KvmNicCoherencyBuffer

  UINT32                                    KvmDmaGpaAddrLo;                ///< Low 32 Bit of GPA of KVM DMA buffer
  UINT32                                    KvmDmaGpaAddrHi;                ///< High 32 Bit of GPA of KVM DMA buffer
  UINT32                                    KvmNicCoherentBufferGpaAddrLo;  ///< Low 32 Bit of GPA of KvmNicCoherencyBuffer
  UINT32                                    KvmNicCoherentBufferGpaAddrHi ; ///< High 32 Bit of GPA of KvmNicCoherencyBuffer
} KVM_OS_PARAM;

typedef union {
  struct {
    UINT32  VendorID:16;
    UINT32  DeviceID:16;
  } Field;
  UINT32 VendorDeviceID;
} KVM_DEVID;

#define KVM_BCM_STATUS_REG_OFFSET  0x8
#define KVM_RTK_STATUS_REG_OFFSET  0x0C

#define RTK_F_REG_WRITE_BASE_INDEX 0xBC021000
#define RTK_F_REG_READ_BASE_INDEX  0x3C021000
#define RTK_REG_WRITE_BASE_INDEX   0x80421100
#define RTK_REG_READ_BASE_INDEX    0x00421100
#define RTK_REG_INDEX_REG_OFFSET   0x74
#define RTK_REG_DATA_REG_OFFSET    0x70

#define AMD_PSP_KVM_DEV_FROM_THIS(a)  CR (a, AMD_PSP_KVM_DEV, AmdPspKvmKbDev, AMD_PSP_KVM_DEV_SIGNATURE)

#define AMD_RV_PSP_VID        0x1022
#define AMD_RV_PSP_DID        0x15DF
#define AMD_PR_PSP_VID        0x1022
#define AMD_PR_PSP_DID        0x1456
#define AMD_SSP_PSP_VID       0x1022
#define AMD_SSP_PSP_DID       0x1486

#define KVM_UNSUPPORTED       0x0
#define BCM_VID               0x14E4
#define BCM5762_DID           0x1687
#define BCM5762_RID           0x10
#define RTK_VID               0x10EC
#define RTK8168_DID           0x8168
#define RTK8168_E_RID         0x0E
#define RTK8168_F_RID         0x1A
#define AQUANTIA_5G_RID       0x02
#define AQUANTIA_10G_RID      0x02
#define RTK8168_BAR2_OFFSET   0x18
#define BCM_5762_DEVID        0x168714E4ul
#define BCM5762_BAR0_OFFSET   0x10
#define BCM5762_BAR2_OFFSET   0x18

#define AQUANTIA_VID            0x1D6A
#define AQUANTIA_10G_DID        0x07B1
#define AQUANTIA_5G_DID         0x08B1
#define KVM_AQUANTIA_STATUS_REG_OFFSET   0x380
#define AQUANTIA_BAR0_OFFSET    0x10
#define AQUANTIA_BAR2_OFFSET    0x18

#define KVM_ENGINE_BINARY_OFFSET (5 * 1024 * 1024)

#define KVM_ABSOLUTE_MOUSE_PROTOCOL  0x0
#define KVM_SIMPLE_MOUSE_PROTOCOL  0x1

// #define ASSERT_EFI_ERROR(x) ASSERT (!EFI_ERROR (x))

#define KVM_BUFFER_ALIGNMENT     0x8000000ul // Must be 128M aligned

extern KVM_MOUSE_DEV    mKvmMouseDev;
extern KB_RING_BUFFER   *mKbRingBuffer;
extern KVM_MOUSE_BUFFER *mMRingBuffer;

EFI_STATUS
EFIAPI
AmdPspKvmDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
AmdPspKvmDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
AmdPspKvmDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN  EFI_HANDLE                     Controller,
  IN  UINTN                          NumberOfChildren,
  IN  EFI_HANDLE                     *ChildHandleBuffer
  );

EFI_STATUS
EFIAPI
AmdPspKvmComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );


EFI_STATUS
EFIAPI
AmdPspKvmComponentNameGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL                     *This,
  IN  EFI_HANDLE                                      ControllerHandle,
  IN  EFI_HANDLE                                      ChildHandle        OPTIONAL,
  IN  CHAR8                                           *Language,
  OUT CHAR16                                          **ControllerName
  );

///Get value of KvmRequest through PCI address
EFI_STATUS
AmdPspKvmGetRequestPciAddr (
  IN     PCI_ADDR    PciAddr,
  IN     UINT32                *KvmRequest
  );

///Get value of KvmRequest
EFI_STATUS
AmdPspKvmGetRequest (
  IN     UINT32                *KvmRequest
  );

///Set value of KvmRequest
EFI_STATUS
AmdPspKvmSetRequest (
  IN     UINT32                *KvmRequest
  );

///Get value of KvmSessionProgress
EFI_STATUS
AmdPspKvmGetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  );

///Set value of KvmSessionProgress
EFI_STATUS
AmdPspKvmSetSessionProgress (
  IN     UINT32                *KvmSessionProgress
  );

EFI_STATUS
AmdPspKvmInitiateKvm (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *ModeInfo,
  IN KVM_IP_INFO                       *KvmIpInfo
  );

EFI_STATUS
AmdPspKvmTerminateKvm (
  VOID
  );

EFI_STATUS
AmdPspKvmRetrieveSmbiosTable (
  IN    UINT8                    KvmEnabled,
  OUT   SMBIOS_TABLE_TYPEA0    **SmbiosTableTypeA0
  );

EFI_STATUS
GetBrcmNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  );

EFI_STATUS
GetAquantiaNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  );

EFI_STATUS
GetBrcmMmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
  );

EFI_STATUS
GetBrcmMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  );

EFI_STATUS
GetBrcmBaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  );

EFI_STATUS
GetAquantiaBaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  );

EFI_STATUS
ReadBrcmKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteBrcmKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  );

EFI_STATUS
ReadBrcmKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteBrcmKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
ReadBrcmKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteBrcmKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
GetRtkNetCardBarBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    KVM_NETCARD_INFO   *NetCardInfo
  );

EFI_STATUS
GetRtkMmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
  );

EFI_STATUS
GetRtkMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  );

EFI_STATUS
GetRtkBaseAddr (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *KvmBaseAddr
  );

EFI_STATUS
ReadRtkKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteRtkKvmReg (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN     UINT32             *value
  );

EFI_STATUS
ReadRtkKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteRtkKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
ReadRtkKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteRtkKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
ReadRtkKvmRequest_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteRtkKvmRequest_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
ReadRtkKvmSessionProgress_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteRtkKvmSessionProgress_F (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
GetAquantiaMmioBase (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT64             *NetCardMmioBase
  );

EFI_STATUS
GetAquantiaMmioSize (
  IN     EFI_HANDLE         *KvmDevHandle,
  IN OUT UINT32             *NetCardMmioSize
  );

EFI_STATUS
ReadAquantiaKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteAquantiaKvmRequest (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
ReadAquantiaKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

EFI_STATUS
WriteAquantiaKvmSessionProgress (
  IN     EFI_HANDLE         *KvmDevHandle,
  OUT    UINT32             *value
  );

#endif


