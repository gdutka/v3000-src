/****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/**
 * @file AmdFtpmProtocol.h
 *
 * @brief  Amd FTPM  prototype definition
 *
 * @details Provide generic AMD based fTPM external interface accorss programs
 *
 */
#ifndef _FTPM_PROTOCOL_H_
#define _FTPM_PROTOCOL_H_

//
// GUID definition
//
extern EFI_GUID gAmdFtpmProtocolGuid;
/**
 * @brief structure definition for HSP mailbox
 *
 */
typedef struct {
  // C2H_TPM_L0
  UINT64  TPM_L0_Address;           ///< Mailbox address
  UINT64  TPM_L0_C2H_MSG_Address;   ///< Doorbell address CPU->HSP
  UINT64  TPM_L0_H2C_MSG_Address;   ///< Doorbell address HSP->CPU

  // C2H_HSP_L0(VLT0)
  UINT64  VLT0_Address;             ///< Mailbox address
  UINT64  VLT0_C2H_MSG_Address;     ///< Doorbell address CPU->HSP
  UINT64  VLT0_H2C_MSG_Address;     ///< Doorbell address HSP->CPU

  //C2H_HSP_L1(VLT1)
  UINT64  VLT1_Address;             ///< Mailbox address
  UINT64  VLT1_C2H_MSG_Address;     ///< Doorbell address CPU->HSP
  UINT64  VLT1_HSC_MSG_Address;     ///< Doorbell address HSP->CPU

  //C2H_HSP_L1(VLT2)
  UINT64  VLT2_Address;             ///< Mailbox address
  UINT64  VLT2_C2H_MSG_Address;     ///< Doorbell address CPU->HSP
  UINT64  VLT2_H2C_MSG_Address;     ///< Doorbell address HSP->CPU

  //C2H_HSP_L1(VLT3)
  UINT64  VLT3_Address;             ///< Mailbox address
  UINT64  VLT3_C2H_MSG_Address;     ///< Doorbell address CPU->HSP
  UINT64  VLT3_H2C_MSG_Address;     ///< Doorbell address HSP->CPU

  // Interrupt Information
  UINT8   Gsi[8];                   ///< Gsi[0] is for HSP Channel 0 TPM
                                    ///< Gsi[1] is for HSP Channel 1 VTL0
                                    ///< Gsi[2] is for HSP Channel 2 VTL1
                                    ///< Gsi[3] is for HSP
                                    ///< Gsi[4] is for HSP
                                    ///< Gsi[7:5] is reserved
} HSP_MAILBOX_ADDRESS, *PHSP_MAILBOX_ADDRESS;
//todo refine to more generic declaration, or move to HSP only interface
typedef union {
  HSP_MAILBOX_ADDRESS HSP_info;
} FTPM_INFO;


typedef struct _AMD_FTPM_PROTOCOL AMD_FTPM_PROTOCOL;
/**
 * @brief Define function prototype: Execute a TPM command
 *
 * @param[in]    This              Point to AMD_FTPM_PROTOCOL itself
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_EXECUTE) (
  IN     AMD_FTPM_PROTOCOL    *This,
  IN     VOID                 *CommandBuffer,
  IN     UINT32                CommandSize,
  IN OUT VOID                 *ResponseBuffer,
  IN OUT UINT32               *ResponseSize
  );

/**
 *  Define function prototype: GET TPM related Info
 *
 * @param[in]    This                     Point to AMD_FTPM_PROTOCOL itself
 * @param[in,out] FtpmStatus              Used to hold more detail info (Unused Currently)
 *
 * @return       EFI_SUCCESS              Ftpm function supported
 * @return       EFI_UNSUPPORTED          Ftpm function unsupported
 *
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_CHECK_STATUS) (
  IN     AMD_FTPM_PROTOCOL    *This,
  IN OUT UINTN                *FtpmStatus
  );


/**
 *  Define function prototype: Send a TPM command
 *
 * @param[in]    This                       Point to AMD_FTPM_PROTOCOL itself
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_SEND_COMMAND) (
  IN     AMD_FTPM_PROTOCOL    *This,
  IN     VOID                 *CommandBuffer,
  IN     UINT32                CommandSize
  );

/**
 *  Define function prototype: Get a TPM command's response
 *
 * @param[in]    This                       Point to AMD_FTPM_PROTOCOL itself
 * @param[in]    ResponseBuffer             Point to the TPM response buffer
 * @param[in]    ResponseSize               Size of the TPM response buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            Device unsupported
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 * @return       EFI_BUFFER_TOO_SMALL       Response buffer too small to hold the response
 *
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_GET_RESPONSE) (
  IN     AMD_FTPM_PROTOCOL     *This,
  IN OUT VOID                  *ResponseBuffer,
  IN OUT UINT32                *ResponseSize
  );

/**
 * @brief Define function prototype: Get TCG Logs
 *
 * @details This function only implemeted on HSP fTPM
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_GET_TCG_LOGS) (
  IN     AMD_FTPM_PROTOCOL     *This,
  IN OUT VOID                  *ResponseBuffer,
  IN OUT UINTN                 *ResponseSize
  );


/**
 * @brief Function prototype for GetHspfTPMInfo
 *
 * @details  Return HSP mailbox base address to SBIOS.
 * SBIOS should call this procedure after PCI Enumeration Complete.
 *
 * @param This      Point to AMD_FTPM_PROTOCOL itself
 * @param FtpmInfo  Point to HSP mailbox base address
 *
 * @return EFI_STATUS
 *             EFI_SUCCESS           - Success
 *             EFI_INVALID_PARAMETER - Input parameter is invalid
 *             EFI_NOT_READY         - HSP device BAR0 MMIO is not ready.
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_GET_TPM_INFO) (
  IN     AMD_FTPM_PROTOCOL     *This,
  IN OUT FTPM_INFO             *FtpmInfo
  );

/**
 * @brief AMD_FTPM_PROTOCOL prototype
 *
 * @details Defines AMD_FTPM_PROTOCOL. This protocol is used to get Ftpm info
 *          Send TPM command, Get TPM command's response, Execute TPM command(Include send & get response)
 */
typedef struct _AMD_FTPM_PROTOCOL {
  FTPM_EXECUTE                          Execute;          ///< Execute TPM command, include send & get response
  FTPM_CHECK_STATUS                     CheckStatus;      ///< Check TPM Status
  FTPM_SEND_COMMAND                     SendCommand;      ///< Send TPM command
  FTPM_GET_RESPONSE                     GetResponse;      ///< Get Last TPM command response
  FTPM_GET_TCG_LOGS                     GetTcgLogs;       ///< Get TCG Logs
  FTPM_GET_TPM_INFO                     GetInfo;          ///< Get TPM info
} AMD_FTPM_PROTOCOL;

#endif //_FTPM_PROTOCOL_H_

