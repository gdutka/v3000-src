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
#include <Library/PcdLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdCfgPcdBufLib.h>

#include <Filecode.h>

#define FILECODE LIBRARY_AMDCFGPCDBUFLIBPEI_AMDCFGPCDBUFLIBPEI_FILECODE

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
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  switch (CfgPcdBufEnum) {
    case  EnumPcdAgesaTestPointEnable:
      ASSERT (ValueSize == sizeof(BOOLEAN));
      *(BOOLEAN *)Value = PcdGetBool (PcdAgesaTestPointEnable);
      break;

    case  EnumPcdAgesaTestPointWidth:
      ASSERT (ValueSize == sizeof(UINT8));
      *(UINT8 *)Value   = PcdGet8 (PcdAgesaTestPointWidth);
      break;

    case  EnumPcdIdsDebugPort:
      ASSERT (ValueSize == sizeof(UINT16));
      *(UINT16 *)Value  = PcdGet16 (PcdIdsDebugPort);
      break;

    case  EnumPcdAgesaAssertEnable:
      ASSERT (ValueSize == sizeof(BOOLEAN));
      *(BOOLEAN *)Value = PcdGetBool (PcdAgesaAssertEnable);
      break;

    case  EnumPcdAgesaTestPointToStb:
      ASSERT (ValueSize == sizeof(BOOLEAN));
      *(BOOLEAN *)Value = PcdGetBool (PcdAgesaTestPointToStb);
      break;

    case  EnumPcdStbEnable:
      ASSERT (ValueSize == sizeof(BOOLEAN));
      *(BOOLEAN *)Value = PcdGetBool (PcdStbEnable);
      break;

    case  EnumPcdGlobalStbVerbosityControl:
      ASSERT (ValueSize == sizeof(UINT8));
      *(UINT8 *)Value   = PcdGet8 (PcdGlobalStbVerbosityControl);
      break;

    case  EnumPcdStbBiosVerbosityControl:
      ASSERT (ValueSize == sizeof(UINT8));
      *(UINT8 *)Value   = PcdGet8 (PcdStbBiosVerbosityControl);
      break;

    case  EnumPcdStbSmuVerbosityControl:
      *(UINT8 *)Value   = PcdGet8 (PcdStbSmuVerbosityControl);
      break;

    case  EnumPcdStbPspVerbosityControl:
      ASSERT (ValueSize == sizeof(UINT8));
      *(UINT8 *)Value   = PcdGet8 (PcdStbPspVerbosityControl);
      break;

    case  EnumPcdStbSmnAddress:
      ASSERT (ValueSize == sizeof(UINT32));
      *(UINT32 *)Value  = PcdGet32 (PcdStbSmnAddress);
      break;

    case  EnumPcdStbFilterMaskEnable:
      ASSERT (ValueSize == sizeof(BOOLEAN));
      *(BOOLEAN *)Value = PcdGetBool (PcdStbFilterMaskEnable);
      break;

    case  EnumPcdMpPostcodeIp0SmnAddress:
      ASSERT (ValueSize == sizeof(UINT32));
      *(UINT32 *)Value  = PcdGet32 (PcdMpPostcodeIp0SmnAddress);
      break;

    case  EnumPcdMpPostcodeConfigSmnAddress:
      ASSERT (ValueSize == sizeof(UINT32));
      *(UINT32 *)Value  = PcdGet32 (PcdMpPostcodeConfigSmnAddress);
      break;

    case  EnumPcdStbIbvSourceId:
      ASSERT (ValueSize == sizeof(UINT8));
      *(UINT8 *)Value   = PcdGet8 (PcdStbIbvSourceId);
      break;

    default:
      ASSERT (0);
      break;
  }

  return Status;
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
  BOOLEAN Value;

  AmdCfgPcdBufGetVariable (
    CfgPcdBufEnum,
    (VOID *) &Value,
    (UINTN) sizeof (BOOLEAN)
    );

#if AMD_CFG_PCD_BUF_DUMP
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t|- [Pei] Get Bool, Enum [0x%02x] = 0x%08x\n", CfgPcdBufEnum, Value);
#endif

  return (BOOLEAN) Value;
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
  UINT8 Value;

  AmdCfgPcdBufGetVariable (
    CfgPcdBufEnum,
    (VOID *) &Value,
    (UINTN) sizeof (UINT8)
    );

#if AMD_CFG_PCD_BUF_DUMP
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t|- [Pei] Get 8,    Enum [0x%02x] = 0x%08x\n", CfgPcdBufEnum, Value);
#endif

  return (UINT8) Value;
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
  UINT16 Value;

  AmdCfgPcdBufGetVariable (
    CfgPcdBufEnum,
    (VOID *) &Value,
    (UINTN) sizeof (UINT16)
    );

#if AMD_CFG_PCD_BUF_DUMP
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t|- [Pei] Get 16,   Enum [0x%02x] = 0x%08x\n", CfgPcdBufEnum, Value);
#endif

  return (UINT16) Value;
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
  UINT32 Value;

  AmdCfgPcdBufGetVariable (
    CfgPcdBufEnum,
    (VOID *) &Value,
    (UINTN) sizeof (UINT32)
    );

#if AMD_CFG_PCD_BUF_DUMP
  IDS_HDT_CONSOLE (MAIN_FLOW, "\t|- [Pei] Get 32,   Enum [0x%02x] = 0x%08x\n", CfgPcdBufEnum, Value);
#endif

  return (UINT32) Value;
}
