/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/**
 * @file AmdFtpmPpi.h
 *
 * @brief  AMD fTPM PPI prototype definition
 *
 * @details Provide generic AMD based fTPM external interface accorss programs
 *
 */
#ifndef _AMD_FTPM_PPI_H_
#define _AMD_FTPM_PPI_H_

typedef struct _AMD_FTPM_PPI AMD_FTPM_PPI;

/**
 * @brief Define function prototype: Execute a TPM command
 *
 * @param[in]    This              Point to AMD_FTPM_PPI itself
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
  IN     AMD_FTPM_PPI         *This,
  IN     VOID                 *CommandBuffer,
  IN     UINTN                CommandSize,
  IN OUT VOID                 *ResponseBuffer,
  IN OUT UINTN                *ResponseSize
  );

/**
 *  Define function prototype: GET TPM related Info
 *
 * @param[in]    This                     Point to AMD_FTPM_PPI itself
 * @param[in,out] FtpmStatus              Used to hold more detail info (Unused Currently)
 *
 * @return       EFI_SUCCESS              Ftpm function supported
 * @return       EFI_UNSUPPORTED          Ftpm function unsupported
 *
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_CHECK_STATUS) (
  IN     AMD_FTPM_PPI         *This,
  IN OUT UINTN                *FtpmStatus
  );


/**
 *  Define function prototype: Send a TPM command
 *
 * @param[in]    This                       Point to AMD_FTPM_PPI itself
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
  IN     AMD_FTPM_PPI         *This,
  IN     VOID                 *CommandBuffer,
  IN     UINTN                 CommandSize
  );

/**
 *  Define function prototype: Get a TPM command's response
 *
 * @param[in]    This                       Point to AMD_FTPM_PPI itself
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
  IN     AMD_FTPM_PPI          *This,
  IN OUT VOID                  *ResponseBuffer,
  IN OUT UINTN                 *ResponseSize
  );

/**
 * @brief Define function prototype: Get TCG Logs
 *
 * @details This function only implemeted on HSP fTPM
 */
typedef
EFI_STATUS
(EFIAPI *FTPM_GET_TCG_LOGS) (
  IN     AMD_FTPM_PPI          *This,
  IN OUT VOID                  *ResponseBuffer,
  IN OUT UINTN                 *ResponseSize
  );


/**
 * @brief AMD_FTPM_PPI prototype
 *
 * @details Defines AMD_FTPM_PPI. This PPI is used to get Ftpm info
 *          Send TPM command, Get TPM command's response, Execute TPM command(Include send & get response)
 */
typedef struct _AMD_FTPM_PPI {
  FTPM_EXECUTE                          Execute;          ///< Execute TPM command, include send & get response
  FTPM_CHECK_STATUS                     CheckStatus;      ///< Check TPM Status
  FTPM_SEND_COMMAND                     SendCommand;      ///< Send TPM command
  FTPM_GET_RESPONSE                     GetResponse;      ///< Get Last TPM command response
  FTPM_GET_TCG_LOGS                     GetTcgLogs;       ///< Get TCG Logs
} AMD_FTPM_PPI;

extern EFI_GUID gAmdFtpmPpiGuid;
extern EFI_GUID gAmdFtpmFactoryResetPpiGuid;
/**
 * @brief AMD_FTPM_FACTORY_RESET_PPI prototype
 *
 * @details SBIOS need Notify this PPI, if this PPI has been installed, denote fTPM on target system
 * require a factory reset, the Nvram can't be recovered. SBIOS need erase the fTPM Nvram with user warning pop up.
 */
typedef struct _AMD_FTPM_FACTORY_RESET_PPI {
  UINT8       Version;     ///< PPI Version
} AMD_FTPM_FACTORY_RESET_PPI;


#endif


