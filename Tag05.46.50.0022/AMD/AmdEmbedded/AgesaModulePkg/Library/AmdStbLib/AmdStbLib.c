/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */


 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/PciLib.h>
#include <Library/AmdStbLib.h>
#include <Library/AmdCfgPcdBufLib.h>
#include <Filecode.h>
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDSTBLIB_AMDSTBLIB_FILECODE
//
// MP2_POSTCODE_IP_0 Register Bitfields:
//
typedef union {
  struct {
    UINT32                                                      IP:8 ; ///<
    UINT32                                                   VALID:1 ; ///<
    UINT32                                           Reserved_31_9:23; ///<
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} MP2_POSTCODE_IP_0_STRUCT;

typedef union {
  struct {
    UINT32                                         IGNORE_IP_CHECK:1 ; ///<
    UINT32                                    IGNORE_FEATURE_CHECK:1 ; ///<
    UINT32                                           Reserved_31_2:30; ///<
  } Field;                                                             ///<
  UINT32 Value;                                                        ///<
} MP2_POSTCODE_CONFIG_STRUCT;



#define STB_SOURCE_ID_MP1_FW           0x50
#define STB_SOURCE_ID_BIOS_AGESA       0xB0
#define STB_SOURCE_ID_PSP_AGESA        0xEA
#define STB_SOURCE_ID_PSP_FTPM         0xEB
#define STB_SOURCE_ID_PSP_DRTM         0xEC
#define STB_SOURCE_ID_PSP_BL1          0xED
#define STB_SOURCE_ID_PSP_BL2          0xEE
#define STB_SOURCE_ID_PSP_TOS          0xEF

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

#define NB_SMN_INDEX_2_PCI_ADDR (PCI_LIB_ADDRESS (0, 0, 0, 0xB8))  ///< PCI Addr of NB_SMN_INDEX_2
#define NB_SMN_DATA_2_PCI_ADDR (PCI_LIB_ADDRESS (0, 0, 0, 0xBC))  ///< PCI Addr of NB_SMN_DATA_2

/**
  Internal work function for get verbosity
  @param[in]  point to Verbosity
  @param[in]  Global Verbosity
  @param[in]  Ip Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
STATIC
EFI_STATUS
GetVerbosityInternal (
  UINT8 *Verbosity,
  UINT8 GlobalVerbosity,
  UINT8 IpVerbosity
  )
{
  UINT8  VerbosityCfg;

  if (GlobalVerbosity == STB_CTRL_PER_IP) {
    VerbosityCfg = IpVerbosity;
  } else {
    VerbosityCfg = GlobalVerbosity;
  }

  if ((VerbosityCfg == STB_CTRL_DISABLE) ||
      (VerbosityCfg == LOW_LEVEL_VERBOSITY) ||
      (VerbosityCfg == PROD_LEVEL_VERBOSITY) ||
      (VerbosityCfg == HIGH_LEVEL_VERBOSITY)) {
    *Verbosity = VerbosityCfg;
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

/**
  GetBiosVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetBiosVerbosity (
  UINT8 *Verbosity
  )
{
  return GetVerbosityInternal (Verbosity, AmdCfgPcdBufGet8 (EnumPcdGlobalStbVerbosityControl),  AmdCfgPcdBufGet8 (EnumPcdStbBiosVerbosityControl));
}

/**
  GetPspVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetPspVerbosity (
  UINT8 *Verbosity
  )
{
  return GetVerbosityInternal (Verbosity, AmdCfgPcdBufGet8 (EnumPcdGlobalStbVerbosityControl),  AmdCfgPcdBufGet8 (EnumPcdStbPspVerbosityControl));
}

/**
  GetSmuVerbosity
  @param[in]  point to Verbosity

  @retval EFI_SUCCESS Verbosity get success
  @retval EFI_UNSUPPORTED Invalid Verbosity value

**/
EFI_STATUS
GetSmuVerbosity (
  UINT8 *Verbosity
  )
{
  return GetVerbosityInternal (Verbosity, AmdCfgPcdBufGet8 (EnumPcdGlobalStbVerbosityControl),  AmdCfgPcdBufGet8 (EnumPcdStbSmuVerbosityControl));
}

/**
  Light version version SMN write with less depx

  @param[in]  Address, SMN address
  @param[in]  Value, Value to be writen to the address

**/
VOID
SmnRegWrite (
  UINT32 Address,
  UINT32 Value
  )
{
  UINTN                  PciAddress;

  PciAddress = NB_SMN_INDEX_2_PCI_ADDR;
  PciWrite32 (PciAddress, Address);
  PciAddress = NB_SMN_DATA_2_PCI_ADDR;
  PciWrite32 (PciAddress, Value);
}

/**
  Write Value to STB (Smart trace buffer)
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
  @param[in]  Verbosity of the write, Valid value: 0-3
**/
VOID
AmdStbWrite (
  UINT32 Value,
  UINT8  Verbosity
  )
{
  UINT8  VerbosityCfg;
  UINT32 StbSmnAddress;
  EFI_STATUS Status;

  if (AmdCfgPcdBufGetBool (EnumPcdStbEnable) == FALSE) {
    return;
  }

  Status = GetBiosVerbosity (&VerbosityCfg);
  //Ignore invalide VerbosityCfg
  if (Status == EFI_SUCCESS) {
    // if input Verbosity large than configured verbosity, ignore
    if (Verbosity > VerbosityCfg) {
      return;
    }
    // Write value to STB register
    StbSmnAddress = AmdCfgPcdBufGet32 (EnumPcdStbSmnAddress);
    SmnRegWrite (StbSmnAddress, Value);
  }
}


/**
  Write Value to STB (Smart trace buffer) with verbosity LOW_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseLow (
  UINT32 Value
  )
{
  AmdStbWrite (Value, LOW_LEVEL_VERBOSITY);
}

/**
  Write Value to STB (Smart trace buffer) with verbosity PROD_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseProd (
  UINT32 Value
  )
{
  AmdStbWrite (Value, PROD_LEVEL_VERBOSITY);
}

/**
  Write Value to STB (Smart trace buffer) with verbosity HIGH_LEVEL_VERBOSITY
  if PcdGlobalStbVerbosityControl is set to [0-3], and Verbosity is larger than PcdGlobalStbVerbosityControl, the write will be ignored
  If PcdGlobalStbVerbosityControl is set to PerIPControl, and Verbosity is larger than PcdStbBiosVerbosityControl, the write will be ignored

  @param[in]  Value to be written to STB
**/
VOID
AmdStbWriteVerboseHigh (
  UINT32 Value
  )
{
  AmdStbWrite (Value, HIGH_LEVEL_VERBOSITY);
}

/**
  Programing STB filter mask register when Per IP Verbosity Control selected, and any source IP verbosity select to disable
  When register been programmed, STB agent will not save any source IP postcode to STB
**/
VOID
AmdStbFilterMaskProgramming (
  VOID
  )
{
  UINT8 BiosVerbosity;
  UINT8 PspVerbosity;
  UINT8 SmuVerbosity;
  UINT32 Mp2PostcodeIpSmnAddress;
  MP2_POSTCODE_IP_0_STRUCT Mp2PostCodeIp;
  BOOLEAN  IpCheck;
  MP2_POSTCODE_CONFIG_STRUCT Mp2PostCodeCfg;


  if (AmdCfgPcdBufGetBool (EnumPcdStbEnable) == FALSE) {
    return;
  }

  if (AmdCfgPcdBufGetBool (EnumPcdStbFilterMaskEnable) == FALSE) {
    return;
  }

  //Initial with default
  IpCheck  = FALSE;
  BiosVerbosity = 0xFF;
  PspVerbosity = 0xFF;
  SmuVerbosity = 0xFF;
  Mp2PostcodeIpSmnAddress = AmdCfgPcdBufGet32 (EnumPcdMpPostcodeIp0SmnAddress);

  if (AmdCfgPcdBufGet8 (EnumPcdGlobalStbVerbosityControl) == STB_CTRL_PER_IP) {
    GetBiosVerbosity (&BiosVerbosity);
    GetPspVerbosity (&PspVerbosity);
    GetSmuVerbosity (&SmuVerbosity);
    //SMU Source ID
    if (SmuVerbosity == STB_CTRL_DISABLE) {
      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_MP1_FW;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;
      IpCheck = TRUE;
    }
    //BIOS source IDs
    if (BiosVerbosity == STB_CTRL_DISABLE) {
      //AGESA postcode
      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_BIOS_AGESA;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;
      //IBV postcode
      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = AmdCfgPcdBufGet8 (EnumPcdStbIbvSourceId);
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;
      IpCheck = TRUE;
    }
    //PSP source IDs
    if (PspVerbosity == STB_CTRL_DISABLE) {
      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_PSP_AGESA;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;

      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_PSP_FTPM;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;

      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_PSP_DRTM;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;

      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_PSP_BL1;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;

      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_PSP_BL2;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;

      Mp2PostCodeIp.Value = 0;
      Mp2PostCodeIp.Field.IP = STB_SOURCE_ID_PSP_TOS;
      Mp2PostCodeIp.Field.VALID = 1;
      SmnRegWrite (Mp2PostcodeIpSmnAddress, Mp2PostCodeIp.Value);
      Mp2PostcodeIpSmnAddress += 4;
      IpCheck = TRUE;
    }
    //if any ip has been disable, program the config register
    if (IpCheck == TRUE) {
      Mp2PostCodeCfg.Value = 0;
      Mp2PostCodeCfg.Field.IGNORE_FEATURE_CHECK = 1;
      SmnRegWrite (AmdCfgPcdBufGet32 (EnumPcdMpPostcodeConfigSmnAddress), Mp2PostCodeCfg.Value);
    }
  }
}
