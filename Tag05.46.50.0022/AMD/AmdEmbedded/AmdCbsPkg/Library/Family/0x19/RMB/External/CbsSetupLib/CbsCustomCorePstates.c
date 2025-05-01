/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

#include <Base.h>
#include <Uefi.h>

#include <Protocol/HiiConfigRouting.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>

#include <Guid/MdeModuleHii.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>

#include <Protocol/AmdNbioSmuServicesProtocol.h>

#include "AmdCbsVariable.h"
#include "AmdCbsFormID.h"
#include "CbsCustomCorePstates.h"

#define MAX_DEST_LENGTH  0x100

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
CustomCorePstate (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest,
  OUT CBS_CONFIG                             *pSetup_Config,
  IN  EFI_HII_HANDLE                         HiiHandle
  )
{
  switch (QuestionId) {
  case KEY_CBS_CMN_CPU_OC_MODE:
    OcMode (This, Action, QuestionId, Type, Value, ActionRequest, pSetup_Config, HiiHandle);
    break;
  case KEY_CBS_CPU_PST_CUSTOM_P0:
    CustomCorePstate0 (This, Action, QuestionId, Type, Value, ActionRequest, pSetup_Config, HiiHandle);
    break;
  default:
    break;
  }

  return EFI_SUCCESS;
}

CHAR16 mCbsOcModeWarningString [] = L"WARNING - DAMAGE CAUSED BY USE OF YOUR AMD PROCESSOR OUTSIDE OF SPECIFICATION OR IN EXCESS OF FACTORY SETTINGS ARE NOT COVERED UNDER YOUR AMD PRODUCT WARRANTY AND MAY NOT BE COVERED BY YOUR SYSTEM MANUFACTURER'S WARRANTY.\nOperating your AMD processor outside of specification or in excess of factory settings, including but not limited to overclocking, may damage or shorten the life of your processor or other system components, create system instabilities (e.g., data loss and corrupted images) and in extreme cases may result in total system failure. AMD does not provide support or service for issues or damages related to use of an AMD processor outside of processor specifications or in excess of factory settings.";

//
// 180W 16C OC setting
//
CBS_OC_TABLE mCbsOcMode180W16C [] = {
  {   0,    DOWNCORE_AUTO,        0,    0,    0}, // Normal Operation
  {   1,    DOWNCORE_AUTO,     0x90, 0x08, 0x22}, // OC1 - 16C 1.33750V 3.6G
  {   2,    DOWNCORE_FOUR_2_2, 0x94, 0x08, 0x1D}, // OC2 -  8C 1.36875V 3.7G
  {   3,    DOWNCORE_TWO_1_1 , 0x96, 0x08, 0x1C}, // OC3 -  4C 1.37500V 3.75G
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
CHAR16 mCbsOcMode180W16CHelpString [] =  L"180W 16C OC setting:\nOC1 - 16C 1.33750V 3.6\nOC2 -  8C 1.36875V 3.7G\nOC3 -  4C 1.37500V 3.75G\n\n";

//
// 180W 12C OC setting
//
CBS_OC_TABLE mCbsOcMode180W12C [] = {
  {   0,    DOWNCORE_AUTO,        0,    0,    0}, // Normal Operation
  {   1,    DOWNCORE_AUTO,     0x90, 0x08, 0x22}, // OC1 - 12C 1.33750V 3.6G
  {   2,    DOWNCORE_FOUR_2_2, 0x94, 0x08, 0x1D}, // OC2 -  8C 1.36875V 3.7G
  {   3,    DOWNCORE_TWO_1_1,  0x96, 0x08, 0x1C}, // OC3 -  4C 1.37500V 3.75G
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
CHAR16 mCbsOcMode180W12CHelpString [] =  L"180W 12C OC setting:\nOC1 - 12C 1.33750V 3.6\nOC2 -  8C 1.36875V 3.7G\nOC3 -  4C 1.37500V 3.75G\n\n";

//
// 180W 8C OC setting
//
CBS_OC_TABLE mCbsOcMode180W8C [] = {
  {   0,    DOWNCORE_AUTO,        0,    0,    0}, // Normal Operation
  {   1,    DOWNCORE_AUTO,     0x9A, 0x08, 0x1D}, // OC1 - 8C 1.36875V 3.85G
  {   2,    DOWNCORE_AUTO,     0x9A, 0x08, 0x1D}, // OC2 - 8C 1.36875V 3.85G
  {   3,    DOWNCORE_TWO_2_0,  0x9C, 0x08, 0x1B}, // OC3 - 4C 1.38125V 3.90G
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
CBS_OC_TABLE mCbsOcMode180W8C_2CCX [] = {
  {   0,    DOWNCORE_AUTO,        0,    0,    0}, // Normal Operation
  {   1,    DOWNCORE_AUTO,     0x9A, 0x08, 0x1D}, // OC1 - 8C 1.36875V 3.85G
  {   2,    DOWNCORE_AUTO,     0x9A, 0x08, 0x1D}, // OC2 - 8C 1.36875V 3.85G
  {   3,    DOWNCORE_TWO_1_1,  0x9C, 0x08, 0x1B}, // OC3 - 4C 1.38125V 3.90G
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
CHAR16 mCbsOcMode180W8CHelpString [] =  L"180W 8C OC setting:\nOC1 - 8C 1.36875V 3.85G\nOC2 - 8C 1.36875V 3.85G\nOC3 - 4C 1.38125V 3.90G\n\n";

//
// 140W 12C OC setting
//
CBS_OC_TABLE mCbsOcMode140W12C [] = {
  {   0,    DOWNCORE_AUTO,        0,    0,    0}, // Normal Operation
  {   1,    DOWNCORE_AUTO,     0x8C, 0x08, 0x1F}, // OC1 - 12C 1.35625V 3.5G
  {   2,    DOWNCORE_FOUR_2_2, 0x8E, 0x08, 0x1D}, // OC2 -  8C 1.36875V 3.55G
  {   3,    DOWNCORE_TWO_1_1,  0x8F, 0x08, 0x1C}, // OC3 -  4C 1.37500V 3.575G
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
CHAR16 mCbsOcMode140W12CHelpString [] =  L"140W 12C OC setting:\nOC1 - 12C 1.35625V 3.5G\nOC2 -  8C 1.36875V 3.55G\nOC3 -  4C 1.37500V 3.575G\n\n";

CBS_OC_TABLE_SELECTION mCbsOcModeTableSelection [] = {
  {ZP_SOCKET_SP3r2, CORE_NUM_8, CCX_NUM_2, 180, mCbsOcMode180W16C, &mCbsOcMode180W16CHelpString[0]}, // 180W 16C
  {ZP_SOCKET_SP3r2, CORE_NUM_6, CCX_NUM_2, 180, mCbsOcMode180W12C, &mCbsOcMode180W12CHelpString[0]}, // 180W 12C
  {ZP_SOCKET_SP3r2, CORE_NUM_4, CCX_NUM_1, 180, mCbsOcMode180W8C,  &mCbsOcMode180W8CHelpString[0]},  // 180W  8C
  {ZP_SOCKET_SP3r2, CORE_NUM_4, CCX_NUM_2, 180, mCbsOcMode180W8C_2CCX,  &mCbsOcMode180W8CHelpString[0]},  // 180W  8C
  {ZP_SOCKET_SP3r2, CORE_NUM_6, CCX_NUM_2, 140, mCbsOcMode140W12C, &mCbsOcMode140W12CHelpString[0]}, // 140W 12C
  {0, 0, 0, 0, NULL, NULL}
};

CBS_OC_TABLE *mOcModeTable = NULL;

EFI_STATUS
OcMode (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest,
  OUT CBS_CONFIG                             *pSetup_Config,
  IN  EFI_HII_HANDLE                         HiiHandle
  )
{
  UINT32                                RegEbx;
  UINT32                                CoreDisFusePerDie;
  UINT8                                 CoreDisByFuseCountPerDie;
  UINT8                                 OpnCoreNumberPerDie;
  UINT8                                 OpnCoreCcxNumberPerDie;
  UINT32                                FuseValue;
  UINT8                                 PackageType;
  UINT32                                OpnMaxTdp;
  CHAR16                               *StringBuffer;
  CHAR16                               *CbsOcModeHelpString;
  CBS_OC_TABLE_SELECTION               *TableSelection;
  CBS_OC_TABLE                         *OcModeTable;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL   *NbioSmuServices;

  // Locate gAmdNbioSmuServicesProtocolGuid
  if (mOcModeTable == NULL) {
    NbioSmuServices = NULL;
    gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, &NbioSmuServices);
    if (NbioSmuServices == NULL) {
      return EFI_SUCCESS;
    }

    // Get PackageType
    AsmCpuid (0x80000001, NULL, &RegEbx, NULL, NULL);
    PackageType = (UINT8) (RegEbx >> 28) & 0xF; // bit 31:28

    if (PackageType == ZP_SOCKET_SP4) {
      NbioSmuServices->AmdSmuRegisterRead (NbioSmuServices, 0, 0x5D258, &FuseValue);
      if ((FuseValue & BIT30) != 0) {
        PackageType = ZP_SOCKET_SP3r2;
      }
    }

    // Get OPN core number
    // OpnCoreNumberPerDie = 8 - CoreDisByFuseCountPerDie
    CoreDisByFuseCountPerDie = 0;
    NbioSmuServices->AmdSmuRegisterRead (NbioSmuServices, 0, 0x5D25C, &CoreDisFusePerDie);
    CoreDisFusePerDie &= 0xFF; // bits[7:0]
    OpnCoreCcxNumberPerDie = 0;
    if ((CoreDisFusePerDie & 0xF) != 0xF) {
      // Ccx0 is enabled
      OpnCoreCcxNumberPerDie++;
    }
    if ((CoreDisFusePerDie & 0xF0) != 0xF0) {
      // Ccx1 is enabled
      OpnCoreCcxNumberPerDie++;
    }

    while (CoreDisFusePerDie != 0) {
      if ((CoreDisFusePerDie & 1) == 1) {
        CoreDisByFuseCountPerDie++;
      }
      CoreDisFusePerDie = CoreDisFusePerDie >> 1;
    }

    OpnCoreNumberPerDie = 8 - CoreDisByFuseCountPerDie;

    // Get OPN TDP
    NbioSmuServices->AmdSmuRegisterRead (NbioSmuServices, 0, 0x5D378, &FuseValue);
    OpnMaxTdp = (FuseValue >> 1) & 0xFF;

    // Find out OC mode table
    TableSelection = mCbsOcModeTableSelection;

    while (TableSelection->OcTable != NULL) {
      if ((TableSelection->PackageType == PackageType) &&
          (TableSelection->CoreNumberPerDie == OpnCoreNumberPerDie) &&
          (TableSelection->CcxNumberPerDie == OpnCoreCcxNumberPerDie) &&
          (TableSelection->Tdp == OpnMaxTdp)) {
        mOcModeTable = TableSelection->OcTable;
        CbsOcModeHelpString = TableSelection->HelpString;

        // Update help string
        StringBuffer = AllocateZeroPool (2048);

        if (StringBuffer != NULL) {
          StrCatS (StringBuffer, MAX_DEST_LENGTH, CbsOcModeHelpString);
          StrCatS (StringBuffer, MAX_DEST_LENGTH, mCbsOcModeWarningString);
          HiiSetString (HiiHandle, STRING_TOKEN (AMD_CBS_STR_OC_MODE_HELP), StringBuffer, NULL);
          gBS->FreePool(StringBuffer);
        }

        break;
      }
      TableSelection++;
    }
  }

  OcModeTable = mOcModeTable;

  // Apply OC table
  if (OcModeTable != NULL ) {
    while (OcModeTable->OcMode != 0xFF) {
      if (pSetup_Config->CbsCmnCpuOcMode == OcModeTable->OcMode) {
        // Update down core mode
//        pSetup_Config->CbsCpuCoreCtrl = OcModeTable->DownCoreMode;
//        pSetup_Config->CbsCpuSmtCtrl = 1;

        // Update P0
        if (OcModeTable->P0Fid == 0) {
          pSetup_Config->CbsCpuPstCustomP0 = AUTO_PSTATE;
        } else {
          pSetup_Config->CbsCpuPstCustomP0 = CUSTOM_PSTATE;
        }

        break;
      }
      OcModeTable++;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
CustomCorePstate0 (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest,
  OUT CBS_CONFIG                             *pSetup_Config,
  IN  EFI_HII_HANDLE                         HiiHandle
  )
{
  PSTATE_MSR PstateMsr;

  // Custom
  if (pSetup_Config->CbsCpuPstCustomP0 == CUSTOM_PSTATE) {
    PstateMsr.Value = AsmReadMsr64 (MSR_PSTATE_0);
    pSetup_Config->CbsCpuPst0Freq = GetPstateInfoFeq ((UINT8) PstateMsr.Field.CpuFid_7_0, (UINT8) PstateMsr.Field.CpuDid);
    pSetup_Config->CbsCpuPst0Vid = (UINT8) PstateMsr.Field.CpuVid;
  }

  return EFI_SUCCESS;
}

UINT32
GetPstateInfoFeq (
  IN  UINT8 Fid,
  IN  UINT8 Did
  )
{
  return (UINT32) (DivU64x32 (MultU64x64 (200, Fid), Did));
}

UINT32
GetPstateInfoVoltage (
  IN  UINT8 Vid
  )
{
  return (UINT32) (1550000L - MultU64x64 (6250, Vid));
}

UINT64
HexToDec (
  IN  UINT64 Hex
  )
{
  UINT8  i;
  UINT64 Dec;
  UINT64 Temp;

  Dec = 0;
  i = 0;
  while (Hex != 0) {
    Temp = Hex % 10;
    Temp = LShiftU64 (Temp, i);
    Dec |= Temp;
    Hex = Hex / 10;
    i = i + 4;
  }

  return Dec;
}

