/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef  _MPM_MBOXLIB_H_
#define _MPM_MBOXLIB_H_
// MPM Control Area definition (C2PMSG_xx register index).
#define BIOS_MPM_NUM_REGS                 6
#include <IndustryStandard/Pci.h>
#include <Library/MpmLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/AmdPspKvmServiceProtocol.h>

// BIOS to MPM mailbox register for all the commands except "MPM_READ_TEXT_MSG" and "MPM_GET_RX_BUFFER_OFFSET_SIZE"
#define BIOS_MPM_C2PMSG_REG_0        0   // C2PMSG[0]: [31] -> Ready, [23:16] -> Command, [15:0] -> Status
#define BIOS_MPM_C2PMSG_REG_24        24  // C2PMSG[24]
#define BIOS_MPM_C2PMSG_REG_25            25  // C2PMSG[25]

//---------------- BIOS to MPM mailbox register for all the commands except "MPM_READ_TEXT_MSG" and "MPM_GET_RX_BUFFER_OFFSET_SIZE"----
// Bit fields of "BIOS_MPM_C2PMSG_REG_0" registers
#define BIOS_MPM_MBOX_READY_SHIFT      31  // Ready flag
#define BIOS_MPM_MBOX_READY_SIZE      1
#define BIOS_MPM_MBOX_READY_MASK      (BIOS_MPM_MBOX_READY_SIZE << BIOS_MPM_MBOX_READY_SHIFT)
#define BIOS_MPM_MBOX_CMD_SHIFT        16  // Command ID, sent by BIOS(x86) to AMF (MPM)
#define BIOS_MPM_MBOX_CMD_SIZE        0xFF
#define BIOS_MPM_MBOX_CMD_MASK        (BIOS_MPM_MBOX_CMD_SIZE << BIOS_MPM_MBOX_CMD_SHIFT)
#define BIOS_MPM_MBOX_STATUS_SHIFT      0   // Command execution status, returned by AMF (MPM) to BIOS(x86)
#define BIOS_MPM_MBOX_STATUS_SIZE      0xFFFF
#define BIOS_MPM_MBOX_STATUS_MASK      (BIOS_MPM_MBOX_STATUS_SIZE << BIOS_MPM_MBOX_STATUS_SHIFT)


// MPM_C2PMSG_0 register:  PR Parameters of "Command [24]" field
// "Command [24]" field will be set by AMF (MPM) and read/consumed by BIOS
// Used with MPM_CMD_PLDM, PR set means Response buffer will be provided for this
// PLDM command
#define BIOS_MPM_PLDM_NO_RESPONSE      0x0
#define BIOS_MPM_PLDM_RESPONSE      0x1
// MPM_C2PMSG_0 register:  Text (T) Parameters of "Command [25]" field
// "Command [25]" field will be set by AMF (MPM) and read/consumed by BIOS
// Used with MPM_CMD_PENDING_TASK, set to 1 if TEXT Re-Direction is pending
#define BIOS_MPM_TEXT_NOT_PENDING      0x0
#define BIOS_MPM_TEXT_PENDING        0x1

// "Command [26]" field will be set by AMF (MPM) and read/consumed by BIOS
// Used with MPM_CMD_PENDING_TASK, set to 1 if KVM Re-Direction is pending
#define BIOS_MPM_KVM_NOT_PENDING      0x0
#define BIOS_MPM_KVM_PENDING        0x1

// MPM_C2PMSG_0 register:  Indication (I) Parameters of "Command [27]" field
// "Command [27]" field will be set by AMF (MPM) and read/consumed by BIOS
// Used with MPM_CMD_PENDING_TASK, set to 1 if Indication is pending
#define BIOS_MPM_INDICATION_NOT_PENDING    0x0
#define BIOS_MPM_INDICATION_PENDING      0x1

// MPM_C2PMSG_0 register:  Parameters of "R [31]" (Ready) field
// "R [31]" (Ready) field will be set to 0 by AMF (MPM) to indicate new command and
// set to 1 by MPM functional driver when the previous command is finished
#define BIOS_MPM_COMMAND_READY      0 // New command for MPM functional driver
#define BIOS_MPM_RESPONSE_READY      1 // Response of previous command
#pragma pack (push, 1)
///
/// X86 to MPM command buffer mapping from MPM_C2PMSG_0,
///
typedef volatile struct {
  UINT32                    Status:16;      ///< Set by the target to indicate the execution status of last command
  UINT32                    CommandId:8;    ///< Command ID set by host
  UINT32                    Response:1;     ///< Refer to BIOS_MPM_PLDM_RESPONSE
  UINT32                    WirelessTxrPending:1;   ///< Refer to BIOS_MPM_TEXT_PENDING
  UINT32                    WirelessKvmPending:1;   ///< Refer to BIOS_MPM_KVM_PENDING
  UINT32                    Reserved:4;     ///< Reserved
  UINT32                    Ready:1;        ///< Set by the target to indicate the mailbox interface state.
} BIOS_MPM_MBOX_REG0_FIELD;

typedef  union {
  IN  UINT32                     Value;               ///< Cmd register value
  IN  BIOS_MPM_MBOX_REG0_FIELD    Field;                    ///< Extended Cmd register with field definition
} BIOS_MPM_MBOX_REG0;

typedef struct {
  BIOS_MPM_MBOX_REG0 Reg0;
  UINT32 Reg1;  //[31:16]Reserved  [15:0]Size of the Buffer
  UINT32 Reg2;  //Offset in the MPM reserved DRTM
} BIOS_MPM_MBOX_COMMON;

// "BIOS_MPM_C2PMSG_REG_0" register:  Parameters of "Status[15:0]" field
// "Status[15:0]" field will be set by AMF(MPM) and read/consumed by BIOS(x86)
#define BIOS_MPM_NO_ERROR                   0x00000000
#define BIOS_MPM_NOT_SUPPORTED              0x00000001
#define BIOS_MPM_GENERIC_ERROR              0x00000002
#define BIOS_MPM_INVALID_SERVICE_ID_ERROR   0x00000003
#define BIOS_MPM_MEMORY_UNALIGNED_ERROR     0x00000004
#define BIOS_MPM_MINIMUM_SIZE_ERROR         0x00000005
#define BIOS_MPM_MAXIMUM_SIZE_ERROR         0x00000006
#define BIOS_MPM_SMBIOS_FAILED        0x00000007
#define BIOS_MPM_BOOT_CONTROL_FAILED      0x00000008
#define BIOS_MPM_BIOS_INFO_FAILED          0x00000009
#define BIOS_MPM_EVENT_LOG_FAILED          0x0000000A
#define BIOS_MPM_BOOT_PROGRESS_FAILED        0x0000000B
#define BIOS_MPM_FW_INFO_FAILED          0x0000000C
#define BIOS_MPM_TEXT_REDIRECTION_FAILED  0x0000000D

// "BIOS_MPM_C2PMSG_REG_0" register:  Parameters of "Command[23:16]" field
// "Command[23:16]" field will be set by BIOS(x86) side and read/consumed by AMF(MPM)
#define BIOS_MPM_CMD_INIT_COMMAND      0x1 // AMF set bit 25,26 and 27 in C2PMSG_0
                                          // AMF send Size/Offset via C2P 24 & 25 // for Battery, Version and PCIE
                                          // information transfer from BIOS-MPM
#define BIOS_MPM_GET_PLDM_BUFFER_OFFSET_SIZE  0x2 // AMF send Size/Offset via C2P 24 & 25
#define BIOS_MPM_CMD_PLDM        0x3
#define BIOS_MPM_GET_ALERT_BUFFER_OFFSET_SIZE   0x4 // AMF send Size/Offset via C2P 24 & 25
#define BIOS_MPM_CMD_ALERT        0x5
#define BIOS_MPM_GET_KVM_BUFFER_OFFSET_SIZE    0x6 // AMF send Size/Offset via C2P 24 & 25
#define BIOS_MPM_CMD_QUERY_KVM      0x7
#define BIOS_MPM_CMD_START_KVM      0x8
#define BIOS_MPM_CMD_TERMINATE_KVM      0x9
#define BIOS_MPM_GET_TEXT_BUFFER_OFFSET_SIZE  0xA // AMF send Size/Offset via C2P 24 & 25
#define BIOS_MPM_CMD_QUERY_TEXT      0xB
#define BIOS_MPM_CMD_START_TEXT      0xC
#define BIOS_MPM_CMD_TERMINATE_TEXT      0xD
#define BIOS_MPM_CMD_BATTERY_MSG      0xE
#define BIOS_MPM_CMD_AGESA_VER      0xF
#define BIOS_MPM_CMD_PCIE_MSG        0x10
#define BIOS_MPM_CMD_BOOT_DONE_MSG      0x11
#define BIOS_MPM_CMD_UN_PROVISION_MSG   0x12  //Un-provision the system
#define BIOS_MPM_CMD_DISABLE_UN_PROVISION_MSG   0x13  //Disable Un-provision



// "BIOS_MPM_C2PMSG_REG_0 or BIOS_MPM_C2PMSG_REG_3" register:  Parameters of "R[31]" (Ready) field
//  "R[31]" (Ready) field will be set to 0 by BIOS to indicate new command and set to 1 by AMF (MPM) when the previous command is finished
#define BIOS_MPM_COMMAND_READY              0   // New command for AMF (MPM)
#define BIOS_MPM_RESPONSE_READY             1   // Response of previous command for BIOS

// Bit fields of "BIOS_MPM_C2PMSG_REG_24" registers

// Bit fields of "BIOS_MPM_C2PMSG_REG_25" registers

// Bit fields of "BIOS_MPM_C2PMSG_REG_30" registers

// Bit fields of "BIOS_MPM_C2PMSG_REG_31" registers

#define IOHC_MPM_BASE_ADDR_LO_SMN_ADDR    0x13B10320
#define IOHC_MPM_BASE_ADDR_HI_SMN_ADDR    0x13B10324

#define BIOS_MPM_CMD_AGESA_VER_MAX_SIZE  256

typedef struct {
  UINT64 PciAddr;       //Encode by EFI_PCI_ADDRESS
  PCI_TYPE_GENERIC Cfg; //PCI config space data
} PCI_DEV;

// Single structure to hold multiple PCI device information
typedef struct {
  UINT32   NumberOfDevice;    // Total number of PCI device
  PCI_DEV  PciDev[1];
} PCI_INFO;

/*this structure is same for both LAN  and WLAN, for LAN BIOS will take the dicission to send MBOX_START_KVM if LAN KVM is pending and ignore the KvmPending bit*/
typedef struct {
  UINT32    DidVid;    //DID and VID mask of the WLAN; Set by AMF if KvmPending is true; This is        needed for BIOS to send BDF for the appropriate WLAN chip
  UINT32    KeyboardBufOffset;
  UINT32    KeyboardBufSize;
  UINT32    MouseBufOffset;
  UINT32    MouseBufSize;
  UINT32    WlanBufOffset;
  UINT32    WlanBufSize;
} MBOX_QUERY_KVM;

/**
 * @brief Structure for Start KVM command
 *
 */
typedef struct{
  UINT32                                NetCardPciCfgAddr;              ///< BDF mask, <27-20> - PCI Bus number <19-15> - PCI Device number, <14-12> - PCI Function number
  UINT32                                FrameBufferBaseLo;              ///< Frame buffer base
  UINT32                                FrameBufferBaseHi;              ///< Frame buffer base
  UINT32                                FrameBufferSize;                ///< display frame buffer size
  UINT32                                KvmMouseProtocolSelection;      ///< 0 = Absolute pointer and 1 = Simple pointer protocol
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  GfxModeInfo;
  UINT32                                DMArEnabled;                    ///< set to 1 if enabled; set to 0 if disabled
  UINT32                                KvmDmaGpaAddrLo;                ///< Valid when DMArEnabled is true, Low 32 Bit of GPA of KVM DMA buffer
  UINT32                                KvmDmaGpaAddrHi;                ///< Valid when DMArEnabled is true, High 32 Bit of GPA of KVM DMA buffer
  UINT32                                KvmDmaHpaAddrLo;         ///< Low 32 Bit HPA of Shared DMAble Memory buffer (KVM/Text Case) allocated by BIOS
  UINT32                                KvmDmaHpaAddrHi;           ///< High 32 Bit HPA of Shared DMAble Memory buffer (KVM/Text Case) allocated by BIOS
  UINT32                                KvmMpmC2pMsg2GpaAddrLo;           ///< Valid when DMArEnabled is true, Low 32 Bit of GPA of Mpm c2p message 2
  UINT32                                KvmMpmC2pMsg2GpaAddrHi;           ///< Valid when DMArEnabled is true, High 32 Bit of GPA of Mpm c2p message 2
  UINT32                                KvmMpmC2pMsg2HpaAddrLo;           ///< Valid when DMArEnabled is true, Low 32 Bit of HPA of Mpm c2p message 2
  UINT32                                KvmMpmC2pMsg2HpaAddrHi;           ///< Valid when DMArEnabled is true, High 32 Bit of HPA of Mpm c2p message 2
  UINT64                                PcieMmioCfgBaseAddr;            ///<MSRC001_0058 [MMIO Configuration Base Address]>
  KVM_IP_INFO                           KvmIpInfo;                      ///<KVM Ip info >
} MBOX_START_KVM;


typedef struct {
    MPM_TEXT_RING_BUFFER TxTcrRingBuf;
    MPM_TEXT_RING_BUFFER RxTcrRingBuf;
} TCR_RING_BUFFER_T;


/**
 * @brief Structure for BIOS_MPM_CMD_QUERY_TEXT
 *
 */
typedef struct {
  UINT32    DidVid;           // DID and VID mask of the WLAN; Set by AMF if TextPending is true; This is needed                 // for BIOS to send BDF for the appropriate WLAN chip
  UINT32    TcrBufOffset; // Offset of the start of TCR_RING_BUFFER_T buffer
  UINT32    TcrBufSize;    // Size TCR_RING_BUFFER_T buffer
  UINT32    WlanBufOffset;
  UINT32    WlanBufSize;
} MBOX_QUERY_TEXT;

/**
 * @brief Structure for BIOS_MPM_CMD_START_TEXT
 *
 */
typedef struct{
  UINT32                                NetCardPciCfgAddr;              ///< BDF mask, <27-20> - PCI Bus number <19-15> - PCI Device number, <14-12> - PCI Function number
  UINT32                                DMArEnabled;                    ///< set to 1 if enabled; set to 0 if disabled
  UINT32                                TcrDmaGpaAddrLo;                ///< Valid when DMArEnabled is true, Low 32 Bit of GPA of Tcr DMA buffer
  UINT32                                TcrDmaGpaAddrHi;                ///< Valid when DMArEnabled is true, High 32 Bit of GPA of Tcr DMA buffer
  UINT32                                TcrDmaHpaAddrLo;                ///< Low 32 Bit HPA of Shared DMAble Memory buffer (Tcr/Text Case) allocated by BIOS
  UINT32                                TcrDmaHpaAddrHi;                ///< High 32 Bit HPA of Shared DMAble Memory buffer (Tcr/Text Case) allocated by BIOS
  UINT32                                TcrMpmC2pMsg2GpaAddrLo;         ///< Valid when DMArEnabled is true, Low 32 Bit of GPA of Mpm c2p message 2
  UINT32                                TcrMpmC2pMsg2GpaAddrHi;         ///< Valid when DMArEnabled is true, High 32 Bit of GPA of Mpm c2p message 2
  UINT32                                TcrMpmC2pMsg2HpaAddrLo;         ///< Valid when DMArEnabled is true, Low 32 Bit of HPA of Mpm c2p message 2
  UINT32                                TcrMpmC2pMsg2HpaAddrHi;         ///< Valid when DMArEnabled is true, High 32 Bit of HPA of Mpm c2p message 2
  UINT64                                PcieMmioCfgBaseAddr;            ///<MSRC001_0058 [MMIO Configuration Base Address]>
} MBOX_START_TEXT;

#pragma pack (pop)
/**
  BIOS send command BIOS_MPM_GET_PLDM_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_PLDM
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetPldmBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  );

/**
  BIOS send command BIOS_MPM_GET_ALERT_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_ALERT
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetAlertBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  );

/**
  BIOS send command BIOS_MPM_GET_TEXT_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_TEXT
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetTextBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  );

/**
  BIOS send command BIOS_MPM_GET_KVM_BUFFER_OFFSET_SIZE to MPM
  to get the buffer which used for sending BIOS_MPM_CMD_KVM
  * @param[in,out]  BufferOffset  Offset of MPM reserved memory used as command buffer
  * @param[in,out]  BufferSize    Size in MPM reserved memory used as command buffer

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdGetKvmBufferOffsetSize (
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  );

/**
  BIOS send command BIOS_MPM_CMD_INIT_COMMAND to MPM to get the bit mask of pending tasks and buffer to send NON-PLDM,ASF command
  * @param[in,out]  PendingTask  Point to the address to hold PendingTask bit masks

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdInit (
  IN OUT BIOS_MPM_MBOX_REG0_FIELD *PendingTask,
  IN OUT UINT32 *BufferOffset,
  IN OUT UINT32 *BufferSize
  );

/**
  BIOS send command BIOS_MPM_CMD_PLDM to MPM to process PLDM message
  * @param[in]   RequestBuffer         Point to the PLDM request message memory buffer,
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the PLDM request message memory buffer, including header
  * @param[out]  ResponseBuffer        Point to the address of PLDM response message memory buffer,
                                       Keep as NULL, if message doesn't require response
                                       This field will be ignored and keep untouched, if response bit is not set by MPM
                                       For message require response, Caller need allocate the buffer prior calling this routine
                                       Callee will copy the content from MPM dedicate mailbox buffer the caller allocate one.
  * @param[out]  ResponseBufferSize    This field will be ignored and keep untouched, if response bit is not set by MPM
                                       On input, Point to the size of ResponseBuffer
                                       On Output, if ResponseBufferSize is smaller than required, update to the required value with
                                       Status EFI_BUFFER_TOO_SMALL

   @retval EFI_STATUS                  0: Success, NonZero Error
**/
//todo  verify especaily the response field
EFI_STATUS
BiosMpmCmdPldmMsg (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize,
  IN OUT VOID   *ResponseBuffer,
  IN OUT UINT32 *ResponseBufferSize
  );

/**
  BIOS send command BIOS_MPM_CMD_ALERT to MPM to process Alert message
  * @param[in]   RequestBuffer         Point to the Alert request message memory buffer
                                       Caller need prepare it before calling this function
  * @param[in]   RequestBufferSize     Size of the Alert request message memory buffer, including header
   @retval EFI_STATUS                  0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdAlertMsg (
  IN     VOID   *RequestBuffer,
  IN     UINT32 RequestBufferSize
  );

/**
  BIOS send command BIOS_MPM_CMD_TERMINATE_KVM to MPM

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdTerminateKvm (
  );

/**
  BIOS send command BIOS_MPM_CMD_TERMINATE_TEXT to MPM

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdTerminateText (
  );

/**
  BIOS send command BIOS_MPM_CMD_QUERY_KVM to MPM
  * @param[in,out]  Para  Point to the address to hold MBOX_QUERY_KVM struct

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdQueryKvm (
  IN OUT MBOX_QUERY_KVM *Para
  );


/**
  BIOS send command BIOS_MPM_CMD_START_KVM to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdStartKvm (
  IN OUT MBOX_START_KVM *Para
  );

/**
 * @brief   BIOS send command BIOS_MPM_CMD_QUERY_TEXT to MPM
 *
 * @param Para Point to the address to hold Para
 * @return EFI_STATUS  0: Success, NonZero Error
 */
EFI_STATUS
BiosMpmCmdQueryText (
  IN OUT MBOX_QUERY_TEXT *Para
  );

/**
  BIOS send command BIOS_MPM_CMD_START_TEXT to MPM
  * @param[in,out]  Para  Point to the address to hold Para

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdStartText (
  IN OUT MBOX_START_TEXT *Para
  );

/**
  BIOS send command BIOS_MPM_CMD_BOOT_DONE_MSG to MPM to inform BIOS boot done

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdBootDone (
  VOID
  );

/**
  BIOS send command BIOS_MPM_CMD_TERMINATE_TEXT to MPM to terminate Tcr

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdTerminateTcr (
  VOID
  );

/**
  BIOS send command BIOS_MPM_CMD_PCIE_MSG to MPM to pass PCI information
  * @param[in,out]  PciInfo  Point to PCI_INFO structure

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdPcieMsg (
  PCI_INFO *PciInfo
  );

/**
  BIOS send command BIOS_MPM_CMD_AGESA_VER to MPM to pass AGESA version
  * @param[in,out]  AgesaVerString  AGESA version string Null-terminated, max size 256 bytes

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdAgesaVersion (
  CHAR8 * AgesaVerString
  );

/**
  BIOS send command BIOS_MPM_CMD_BATTERY_MSG to MPM to pass battery information
  * @param[in,out]  BatteryInfo  Point to the structure contain battery information

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdBatteryMsg (
  BATTERY_INFO_T * BatteryInfo
  );

/**
  Return the MPM MMIO location

  @retval MPM MMIO location, Zero if error occurs
**/
UINT32
GetMpmBarAddr (
  VOID
  );

/**
  BIOS send command BIOS_MPM_CMD_UN_PROVISION_MSG to MPM to un-provision the system

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdUnProvision (
  VOID
  );

/**
  BIOS send command BIOS_MPM_CMD_DISABLE_UN_PROVISION_MSG to MPM to diabled un-provision the system

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
BiosMpmCmdDisableUnProvision (
  VOID
  );
#endif //_MPM_MBOXLIB_H_
