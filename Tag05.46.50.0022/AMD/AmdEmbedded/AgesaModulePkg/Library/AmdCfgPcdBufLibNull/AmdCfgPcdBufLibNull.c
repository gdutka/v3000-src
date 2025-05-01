/*
*****************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Service procedure to cache PCD setting.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: LIB
 * @e \$Revision: 321005 $   @e \$Date: 2021-09-07 14:15:16 +0800 (Tues, 7 Sep 2021) $
 *
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/IdsLib.h>
#include <Library/AmdCfgPcdBufLib.h>

#include <Filecode.h>

#define FILECODE LIBRARY_AMDCFGPCDBUFLIBNULL_AMDCFGPCDBUFLIBNULL_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief Get Pcd Variable
 * @details Get Pcd Value from PcdGetxx in Pei Phase.
 *          Get Pcd Value from Buffer in Dxe/Smm Phase.
 * @param[in]           CFG_PCD_BUF_ENUM  Enum Value
 * @param[in]           *VOID             The Pointer of Value
 * @param[in]           UINTN             The Value Size
 *
 * @return  EFI_STATUS  EFI_SUCCESS       Success
 *                      EFI_UNSUPPORTED   Function Unsupported
 */
EFI_STATUS
AmdCfgPcdBufGetVariable (
  CFG_PCD_BUF_ENUM  CfgPcdBufEnum,
  VOID             *Value,
  UINTN            ValueSize
)
{
  return AGESA_UNSUPPORTED;
}


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
)
{
  return 0;
}

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
)
{
  return 0;
}


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
)
{
  return 0;
}


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
)
{
  return 0;
}
