/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/**
 * @file AmdPspDtpmPpi.h
 *
 * @brief  PSP discrete TPM PPI prototype definition
 *
 * @details Provide generic PSP based dTPM external interface accorss programs
 *
 */
#ifndef _PSP_DTPM_PPI_H_
#define _PSP_DTPM_PPI_H_

typedef struct _PSP_DTPM_PPI PSP_DTPM_PPI;

///
/// Current PPI revision
/// 
#define PSP_DTPM_PPI_REVISION   0x01

/**
 * @enum define various enum type for returns status of the dTPM operation performed by the PSP
 */
typedef enum _DTPM_INFO_CONFIG_STATUS
{
  DTPM_INFO_CONFIG_STATUS_SUCCESS             = 0x00,         ///< The configuration request successed
  DTPM_INFO_CONFIG_STATUS_NO_DTPM             = 0x01,         ///< No DTPM connected
  DTPM_INFO_CONFIG_STATUS_INVALID_CONFIG      = 0x02,         ///< Invalid configuration
  DTPM_INFO_CONFIG_STATUS_DTPM_CONFIG_INVALID = 0x03,         ///< DTPM configuration invalid
} DTPM_INFO_CONFIG_STATUS;

/**
 * @brief Define function prototype: Get discrete TPM status
 *
 * @param[out]     DesiredConfig            point to DesiredConfig.return dTPM configuration requested by ABL
 * @param[out]     ConfigStatus             point to ConfigStatus. 0 - success. Non-zero failure.
 * @param[in, out] LogDataSize              point to LogDataSize. Size of the LogData buffer, return actual need size from PSP.
 * @param[out]     LogData                  Point to LogData buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_WARN_BUFFER_TOO_SMALL  Buffer too small to hold the logdata
 * @return       EFI_INVALID_PARAMETER;     Invalid Address
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 *
 */
typedef
EFI_STATUS
(*GET_DTPM_STATUS) (
  OUT    UINT32 *DesiredConfig,
  OUT    UINT32 *ConfigStatus,
  IN OUT UINT32 *LogDataSize,
  OUT    VOID   *LogData
  );

/**
 * @brief PSP_DTPM_PPI prototype
 *
 * @details Defines PSP_DTPM_PPI. This PPI is used to get dtpm status info
 *          Get TPM configuration statuts and log data
*/
typedef struct _PSP_DTPM_PPI {
  UINT8             Revision;           ///< PPI Revision
  GET_DTPM_STATUS   GetDtpmStatus;      ///< Get dTPM status and event log
} PSP_DTPM_PPI;


extern EFI_GUID gAmdPspDtpmPpiGuid;

#endif  //_PSP_DTPM_PPI_H_
