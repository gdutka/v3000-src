/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/**
 * @file AmdCfgPcdBuf.h
 *
 * @brief Cache PCD setting to RT data memory, and provide method to retrieve it.
 *
 * @details In PEI phase use PCD routine directly. 
 * Post PEI phase, cache PCD setting to local structure, and get the value from local structure.
 * Note: Set the dynamic PCDs, Post PEI phase, may not take effect.
 *
 */

#ifndef _AMD_CFG_PCD_BUF_LIB_H_
#define _AMD_CFG_PCD_BUF_LIB_H_

/**
 * @brief Switch Pcd Buffer Dump or not.
 * @details 0: Turn Off Pcd buffer Dump;
 *          1: Turn On Pcd buffer Dump;
 */
#define AMD_CFG_PCD_BUF_DUMP 0

/**
 * @brief The Emun of Pcd which was used in AmdCfgPcdBufGetxx function.
 * @details For AmdCfgPcdBufGetxx function, use Enum value as a inuput Paramater.
 */
typedef enum {
  EnumPcdAgesaTestPointEnable       = 0x01,
  EnumPcdAgesaTestPointWidth        = 0x02,
  EnumPcdIdsDebugPort               = 0x03,
  EnumPcdAgesaAssertEnable          = 0x04,
  EnumPcdAgesaTestPointToStb        = 0x05,

  EnumPcdStbEnable                  = 0x06,
  EnumPcdGlobalStbVerbosityControl  = 0x07,
  EnumPcdStbBiosVerbosityControl    = 0x08,
  EnumPcdStbSmuVerbosityControl     = 0x09,
  EnumPcdStbPspVerbosityControl     = 0x0A,
  EnumPcdStbSmnAddress              = 0x0B,
  EnumPcdStbFilterMaskEnable        = 0x0C,
  EnumPcdMpPostcodeIp0SmnAddress    = 0x0D,
  EnumPcdMpPostcodeConfigSmnAddress = 0x0E,
  EnumPcdStbIbvSourceId             = 0x0F,
  EnumPcdMax                        = 0x10,
} CFG_PCD_BUF_ENUM;

#pragma  pack (push, 1)

/**
 * @brief The structure used to cache PCD settings.
 * @details In Dxe/Smm Phase, the struct is used to cache PCD settings.
 */
typedef struct _CFG_PCD_BUF_STRUCT {
  BOOLEAN BufPcdAgesaTestPointEnable;       // The buffer of PcdAgesaTestPointEnable
  UINT8   BufPcdAgesaTestPointWidth;        // The buffer of PcdAgesaTestPointWidth
  UINT16  BufPcdIdsDebugPort;               // The buffer of PcdIdsDebugPort
  BOOLEAN BufPcdAgesaAssertEnable;          // The buffer of PcdAgesaAssertEnable
  BOOLEAN BufPcdAgesaTestPointToStb;        // The buffer of PcdAgesaTestPointToStb

  BOOLEAN BufPcdStbEnable;                  // The buffer of PcdStbEnable
  UINT8   BufPcdGlobalStbVerbosityControl;  // The buffer of PcdGlobalStbVerbosityControl
  UINT8   BufPcdStbBiosVerbosityControl;    // The buffer of PcdStbBiosVerbosityControl
  UINT8   BufPcdStbSmuVerbosityControl;     // The buffer of PcdStbSmuVerbosityControl
  UINT8   BufPcdStbPspVerbosityControl;     // The buffer of PcdStbPspVerbosityControl
  UINT32  BufPcdStbSmnAddress;              // The buffer of PcdStbSmnAddress
  BOOLEAN BufPcdStbFilterMaskEnable;        // The buffer of PcdStbFilterMaskEnable
  UINT32  BufPcdMpPostcodeIp0SmnAddress;    // The buffer of PcdMpPostcodeIp0SmnAddress
  UINT32  BufPcdMpPostcodeConfigSmnAddress; // The buffer of PcdMpPostcodeConfigSmnAddress
  UINT8   BufPcdStbIbvSourceId;             // The buffer of PcdStbIbvSourceId
} CFG_PCD_BUF_STRUCT;

#pragma  pack (pop)


/**
 * @brief Get BOOLEAN Pcd Value
 * @details Get BOOLEAN Pcd Value from PcdGetxx in Pei Phase.
 *          Get BOOLEAN Pcd Value from Buffer in Dxe/Smm Phase.
 * @param[in]       CFG_PCD_BUF_ENUM  Enum Value
 *
 * @return BOOLEAN  Return BOOLEAN Value
 */
BOOLEAN
AmdCfgPcdBufGetBool (
 CFG_PCD_BUF_ENUM CfgPcdBufEnum
);


/**
 * @brief Get UINT8 Pcd Value
 * @details Get UINT8 Pcd Value from PcdGetxx in Pei Phase.
 *          Get UINT8 Pcd Value from Buffer in Dxe/Smm Phase.
 * @param[in]       CFG_PCD_BUF_ENUM  Enum Value
 *
 * @return UINT8    Return UINT8 Value
 */
UINT8
AmdCfgPcdBufGet8 (
 CFG_PCD_BUF_ENUM CfgPcdBufEnum
);


/**
 * @brief Get UINT16 Pcd Value
 * @details Get UINT16 Pcd Value from PcdGetxx in Pei Phase.
 *          Get UINT16 Pcd Value from Buffer in Dxe/Smm Phase.
 * @param[in]       CFG_PCD_BUF_ENUM  Enum Value
 *
 * @return UINT16   Return UINT16 Value
 */
UINT16
AmdCfgPcdBufGet16 (
 CFG_PCD_BUF_ENUM CfgPcdBufEnum
);


/**
 * @brief Get UINT32 Pcd Value
 * @details Get UINT32 Pcd Value from PcdGetxx in Pei Phase.
 *          Get UINT32 Pcd Value from Buffer in Dxe/Smm Phase.
 * @param[in]       CFG_PCD_BUF_ENUM  Enum Value
 *
 * @return UINT32   Return UINT32 Value
 */
UINT32
AmdCfgPcdBufGet32 (
 CFG_PCD_BUF_ENUM CfgPcdBufEnum
);

#endif //_AMD_CFG_PCD_BUF_LIB_H_

