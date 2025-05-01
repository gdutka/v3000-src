/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/**
 * @file HspLib.h
 *
 * @brief  Generic Lib for HSP
 *
 * @details Provide generic Service for HSP
 *
 */

#ifndef _HSP_LIB_H_
#define _HSP_LIB_H_
VOID
HspDumpData (
  IN  VOID    *p,
  IN  UINT32  iSize
  );

/**
 *  Get value by SMN
 *
 * @param[in]    AddressSMN        SMN address
 * @return       UINT32            Value
 *
 */
UINT32
FtpmGetValueSMN (
  IN UINT32 AddressSMN
  );


/**
 *  Set value by SMN
 *
 * @param[in]    AddressSMN        SMN address
 * @param[in]    Value32           Value need to be set
 * @param[in]    Flags             Flags - BIT0 indicates S3 save/restore
 *
 */
VOID
FtpmSetValueSMN (
  IN UINT32 AddressSMN,
  IN UINT32 Value32,
  IN UINT32 Flags
  );

/**
 *  Get value by MMIO
 *
 * @param[in]    iOffset        SMN address
 * @return       UINT32         The value to write
 *
 */

UINT32
FtpmGetValueMmio (
  IN UINT32 iOffset
  );

/**
 *  Set value by MMIO
 *
 * @param[in]    iOffset           SMN address
 * @param[in]    iValue            Value need to be set
 *
 */

VOID
FtpmSetValueMmio (
  IN UINT32 iOffset,
  IN UINT32 iValue
  );

/**
 *  Check for HSP TPM CRB command's completeness
 *
 * @return       UINT32 TPM CRB status
 *
 */

UINT32
FtpmCheckStatus (
  );

/**
 *  Check if specific channel is ready.
 *
 * @param[in] pChannelNumber    HSP channel number
 *
 * @return    BOOLEAN           Channel ready or not.
 *
 */

BOOLEAN
IsHspReady (
  IN       VOID    *pChannelNumber
  );

/**
 *  Notify HSP firmware to do the command by HSP doorbell.
 *
 * @param[in] ChannelNumber     HSP channel number
 *
 */

VOID
NotifyHSP (
  IN       HSP_MAILBOX_CHANNEL    ChannelNumber
  );

/**
 *  Set Channel 0 TPM to idle mode.
 *
 * @param[in] ChannelNumber     HSP channel number
 *
 */

VOID
FtpmGotoIdle (
  IN       HSP_MAILBOX_CHANNEL    ChannelNumber
  );

/**
 *  Copy command to HSP mailbox buffer.
 *
 * @param[in] *CommandBuffer    Command buffer base address
 * @param[in] CommandSize       Command size
 *
 */

VOID
CopyToHSPBuffer (
  IN     VOID                 *CommandBuffer,
  IN     UINT32                CommandSize
  );

/**
 *  Copy data in HSP mailbox buffer to caller response buffer.
 *
 * @param[in] *ResponseBuffer    Response buffer base address
 * @param[in] ResponseSize       Response size
 *
 */

VOID
CopyFromHSPBuffer (
  IN     VOID                 *ResponseBuffer,
  IN     UINT32               ResponseSize
  );

/**
 *  SPI_REQUEST send command
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            HSP Device unsupported
 * @return       EFI_NOT_READY              HSP Device Channel 1 is not ready
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 *
 */

EFI_STATUS
SpiRequestSendCommand (
  IN      SPI_REQUEST           *CommandBuffer,
  IN      UINT32                CommandSize
  );

/**
 *  Send Irq information to HSP firmware
 *
 * @param[in]    IrqRegisterSMNAddress Address of IOAPIC::IRQ_PIN_ASSERTION_REGISTER
 * @param[in]    Irq                   Irq buffer
 * @param[in]    IrqAmount             Irq amount
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            HSP Device unsupported
 * @return       EFI_NOT_READY              HSP Device Channel 1 is not ready
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 *
 */
EFI_STATUS
SendIrq2Hsp (
  IN  UINT32  IrqRegisterSMNAddress,
  IN  UINT8   *Irq,
  IN  UINT32  IrqAmount
  );


/**
 *  Get address of HSP x86 3 channels
 *
 * @param[in]    Types      Specific which channel
 *
 * @return       UINT64                Address of specific channel.
 *
 */
UINT64
GetHspChannelOffset (
  IN  UINT32 Types
  );

VOID
SetVTL0TpmBaseSize ();

/**
 * @brief Show or Hide HSP Pcie Device
 * @details Show or Hide HSP Pcie Device
 *
 * @param[in] ShowHspPci       Show/hide HSP Pcie Device
 * @param[in] Flags       Flags - BIT0 indicates S3 save/restore
 *
 * @return    EFI_STATUS
 *              - EFI_SUCCESS           The Function Completed Successfully.
 *              - EFI_INVALID_PARAMETER Can't access NBIFMM::RCC_DEV2_EPF2_STRAP0
 */
EFI_STATUS
EFIAPI
ShowHspPciDevice (
  IN BOOLEAN ShowHspPci,
  IN UINT32  Flags
);

#endif //_HSP_LIB_H_

