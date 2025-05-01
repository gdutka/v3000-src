/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
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
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>

#include <Library/AmdCbsSetupLib.h>
#include <Protocol/AmdCbsHookProtocol.h>
#include <Guid/AmdCbsConfig.h>
#include "AmdCbsVariable.h"
#include "AmdCbsFormID.h"
#include "AmdSoc.h"

#include "PiDxe.h"
#include "Porting.h"
#include "SocLogicalId.h"
#include "Addendum/Apcb/Inc/SSP/APCB.h"
#include "ApcbCommon.h"
#include "Protocol/AmdApcbProtocol.h"
#include "Library/AmdPspBaseLibV2.h"
#include "CbsCustomCorePstates.h"
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <Library/AmdPspMboxLibV2.h>

#define MAX_DEST_LENGTH  0x100

EFI_STATUS
UpdateCbsApcbTokens (
  VOID *CbsVariable,
  AMD_APCB_SERVICE_PROTOCOL *ApcbProtocol
  );

UINT32
GetApcbHash (
  VOID *CbsVariable,
  AMD_APCB_SERVICE_PROTOCOL *ApcbProtocol
  );

EFI_STATUS
CbsSuppressConditionUpdate (
  IN CBS_CONFIG   *Setup_Config
  );

extern  EFI_BOOT_SERVICES *gBS;
extern  EFI_GUID gAmdApcbDxeServiceProtocolGuid;

UINTN CbsVariableSize = sizeof(CBS_CONFIG);
DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL   *mNbioSmuServices = NULL;

VOID
UpdateHelpTextForFarSwitch (
  IN OUT CHAR16        *OptionsHelpString,
  OUT CBS_CONFIG       *pSetup_Config,
  IN  EFI_HII_HANDLE   HiiHandle
) {

  UINT32 HSTIState = 0;
  UINT32 InitSplForFar = pSetup_Config->CbsCmnSocInitSplForFar;
  if (pSetup_Config->CbsCmnSocFarSwitch == 1) {
    PspMboxBiosQueryHSTIState (&HSTIState);
    if (InitSplForFar == 0) {
      if ((HSTIState & PSP_ANTI_ROLLBACK_STATUS) != 0) {
        StrCatS(OptionsHelpString, MAX_DEST_LENGTH, L"BIOS will update SPL fuse to SPL value in the SPL table at next boot.");
      } else {
        StrCatS(OptionsHelpString, MAX_DEST_LENGTH, L"BIOS will enforce FAR and set SPL fuse to SPL value in the SPL table at next boot.");
      }
    } else {
      if ((HSTIState & PSP_ANTI_ROLLBACK_STATUS) != 0) {
        StrCatS(OptionsHelpString, MAX_DEST_LENGTH, L"BIOS will keep SPL fuse as it is at next boot.");
      } else {
        StrCatS(OptionsHelpString, MAX_DEST_LENGTH, L"BIOS will enforce FAR and set SPL fuse to initial SPL value at next boot.");
      }
    }
    HiiSetString(HiiHandle, STRING_TOKEN(AMD_CBS_STR_FAR_SWITCH_HELP), OptionsHelpString, NULL);
  } else if (pSetup_Config->CbsCmnSocFarSwitch == 0) {
    StrCatS(OptionsHelpString, MAX_DEST_LENGTH, L"BIOS will NOT change FAR enforcement state and NOT set SPL fuse at next boot.");
    HiiSetString(HiiHandle, STRING_TOKEN(AMD_CBS_STR_FAR_SWITCH_HELP), OptionsHelpString, NULL);
  }
}

EFI_STATUS
HelpTextUpdateCallBack (
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
  CHAR16  *OptionsHelpString;
  CHAR16  *StringBuffer;

  StringBuffer = AllocateZeroPool (MAX_DEST_LENGTH * sizeof(CHAR16));
  if (StringBuffer == NULL) {
    return EFI_UNSUPPORTED;
  }
  OptionsHelpString = AllocateZeroPool (MAX_DEST_LENGTH * sizeof(CHAR16));
  if (OptionsHelpString == NULL) {
    return EFI_UNSUPPORTED;
  }

  switch (Action) {
    case EFI_BROWSER_ACTION_RETRIEVE:
      switch (QuestionId) {
      case KEY_CBS_CMN_GNB_NB_IOMMU:
        if (Value->u8 == 0xf) {
          StrCatS (OptionsHelpString, MAX_DEST_LENGTH, L"Enable/Disable IOMMU");
          if (PcdGetBool (PcdCfgIommuSupport)) {
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Enable");
          } else {
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Disable");
          }
          StrCatS (OptionsHelpString, MAX_DEST_LENGTH, StringBuffer);
          HiiSetString (HiiHandle, STRING_TOKEN (AMD_CBS_STR_IOMMU_HELP), OptionsHelpString, NULL);
        }
        break;
      case KEY_CBS_CMN_GNB_PCIE_PSPP_POLICY:
        if (Value->u8 == 0xf) {
          StrCatS (OptionsHelpString, MAX_DEST_LENGTH, L"Pspp policy");
          switch (PcdGet8 (PcdPsppPolicy)) {
          case 0:  // Performance
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Disable");
            break;
          case 1:  // Performance
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Performance");
            break;
          case 2:  // Balanced
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Balanced");
            break;
          case 3:  // Power Saving
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Power Saving");
            break;
          }
          StrCatS (OptionsHelpString, MAX_DEST_LENGTH, StringBuffer);
          HiiSetString (HiiHandle, STRING_TOKEN (AMD_CBS_STR_PSPP_POLICY_HELP), OptionsHelpString, NULL);
        }
        break;
      case KEY_CBS_CMN_GNB_GFX_NB_AZALIA:
        if (Value->u8 == 0xf) {
          StrCatS (OptionsHelpString, MAX_DEST_LENGTH, L"Enable Integrate HD Audio controller");
          if (PcdGetBool (PcdCfgAzaliaEnable)) {
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Enable");
          } else {
            StrCatS (StringBuffer, MAX_DEST_LENGTH, L"\nAuto=Disable");
          }
          StrCatS (OptionsHelpString, MAX_DEST_LENGTH, StringBuffer);
          HiiSetString (HiiHandle, STRING_TOKEN (AMD_CBS_STR_NB_AZALIA_HELP), OptionsHelpString, NULL);
        }
        break;
      case KEY_CBS_CMN_SOC_FAR_SWITCH:
        UpdateHelpTextForFarSwitch (OptionsHelpString, pSetup_Config, HiiHandle);
        break;
      default:
        break;
      }
      break;
    default:
      break;
  }

  if (StringBuffer != NULL)
    gBS->FreePool(StringBuffer);
  if (OptionsHelpString != NULL)
    gBS->FreePool(OptionsHelpString);

  return EFI_UNSUPPORTED;
}

EFI_STATUS
CustomFchSataClass (
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
  if (pSetup_Config->CbsCmnFchSataEnable == 0xf) {
    pSetup_Config->CbsCmnFchSataClass = 0x2;
  }

  return EFI_SUCCESS;
}

// SPL Fuse = (1 << SPL) - 1
/*
For example:
+-----+----------------------------------------------------------+
| SPL |                         SPL Fuse                         |
+-----+----------------------------------------------------------+
|   0 | 0x0  = b0000 0000                                        |
|   1 | 0x1  = b0000 0001                                        |
|   2 | 0x3  = b0000 0011                                        |
|   3 | 0x7  = b0000 0111                                        |
|   4 | 0xF  = b0000 1111                                        |
|   5 | 0x1F = b0001 1111                                        |
|   6 | 0x3F = b0011 1111                                        |
| ... | ...                                                      |
|  30 | 0x7FFFFFFFF                                              |
|  31 | 0xFFFFFFFF                                               |
|  32 | (SplFuse1=0x1, SplFuse0=0xFFFFFFFF)                      |
|  33 | (SplFuse1=0x3, SplFuse0=0xFFFFFFFF)                      |
| ... | ...                                                      |
|  63 | (SplFuse1=0xFFFFFFFF, SplFuse0=0xFFFFFFFF)               |
|  64 | (SplFuse2=0x1, SplFuse1=0xFFFFFFFF, SplFuse0=0xFFFFFFFF) |
+-----+----------------------------------------------------------+
*/
UINT32 GetSpl (UINT32 SplFuse)
{
  UINT32 SPL = 0;
  while (SplFuse != 0)
  {
      SplFuse = SplFuse & (SplFuse - 1);
      SPL++;
  }
  return SPL;
}

EFI_STATUS
AmdCbsSetupCallbackRoutine (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest,
  OUT UINT8                                  *IfrData,
  IN  EFI_HII_HANDLE                         HiiHandle
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  CBS_CONFIG *pSetup_Config;
  UINT32     HSTIState = 0;
  UINT32     SplFuse0 = 0;
  UINT32     SplFuse1 = 0;
  UINT32     SplFuse2 = 0;
  UINT32     SplFuse3 = 0;
  UINT32     SplFuseValue = 0;
  UINT32     SplTableValue = 0;
  CHAR16     *OptionsHelpString;

  OptionsHelpString = AllocateZeroPool(MAX_DEST_LENGTH * sizeof(CHAR16));
  if (OptionsHelpString == NULL) {
    return EFI_UNSUPPORTED;
  }

  pSetup_Config = (CBS_CONFIG *)IfrData;

  switch (Action) {
    case EFI_BROWSER_ACTION_FORM_OPEN:
      switch (QuestionId) {
      case KEY_CBS_CMN_SCPC_ATTR_CNTL:
        pSetup_Config->CbsCmnScpcAttrCntl = PcdGet8 (PcdScpcLevel);
        break;
      case KEY_CBS_CMN_SOC_INIT_SPL_FOR_FAR:
        // set default value for initial SPL
        pSetup_Config->CbsCmnSocInitSplForFar = PcdGet32(PcdAmdPspAntiRollbackInitialSpl);
        break;
      case KEY_CBS_CMN_SOC_SPL_FUSE:
        PspMboxQuerySplFuse (&SplFuse0, &SplFuse1, &SplFuse2, &SplFuse3);
        SplFuseValue = GetSpl (SplFuse0) + GetSpl (SplFuse1) + GetSpl (SplFuse2) + GetSpl (SplFuse3);
        pSetup_Config->CbsCmnSocSplFuse = SplFuseValue;
        break;
      case KEY_CBS_CMN_SOC_SPL_VALUE_IN_TBL:
        PspMboxQuerySplValue (&SplTableValue);
        pSetup_Config->CbsCmnSocSplValueInTbl = SplTableValue;
        break;
      case KEY_CBS_CMN_SOC_FAR_ENFORCED:
        PspMboxBiosQueryHSTIState(&HSTIState);
        if ((HSTIState & PSP_ANTI_ROLLBACK_STATUS) != 0) {
          pSetup_Config->CbsCmnSocFarEnforced = 1;
        }
        break;
      case KEY_CBS_CMN_SOC_FAR_SWITCH:
        pSetup_Config->CbsCmnSocFarSwitch = (UINT8) PcdGetBool (PcdAmdPspAntiRollbackLateSplFuse);
        break;
      case KEY_CBS_CMN_SO_C_MISC_MPM_SUPPORT:
        pSetup_Config->CbsCmnSoCMiscMpmSupport = (UINT8)PcdGetBool (PcdMpmEnable);
        pSetup_Config->CbsCmnSoCMiscWiredKvm = (UINT8)PcdGetBool (PcdMpmWiredKvmEnable);
        pSetup_Config->CbsCmnSoCMiscWirelessManageability = (UINT8)PcdGetBool (PcdMpmWirelessManageability);
        pSetup_Config->CbsCmnSoCMiscWirelessKvm = (UINT8)PcdGetBool (PcdMpmWirelessKvmEnable);
        pSetup_Config->CbsCmnSoCMiscWirelessTcr = (UINT8)PcdGetBool (PcdMpmWirelessTcrEnable);
        pSetup_Config->CbsCmnSocMiscMpmAttrCntl = (UINT8)(PcdGetBool (PcdMpmWirelessNicDetected) | (PcdGetBool (PcdMpmWiredNicDetected) << 1));
        break;
      }
      break;
    case EFI_BROWSER_ACTION_RETRIEVE:
      Status = HelpTextUpdateCallBack (This, Action, QuestionId, Type, Value, ActionRequest, pSetup_Config, HiiHandle);
      Status = OcMode (This, Action, QuestionId, Type, Value, ActionRequest, pSetup_Config, HiiHandle);
      break;
    case EFI_BROWSER_ACTION_CHANGING:
    case EFI_BROWSER_ACTION_CHANGED:
      switch (QuestionId) {
      case KEY_CBS_CMN_CPU_OC_MODE:
      case KEY_CBS_CPU_PST_CUSTOM_P0:
        CustomCorePstate (This, Action, QuestionId, Type, Value, ActionRequest, pSetup_Config, HiiHandle);
        break;
      case KEY_CBS_CMN_FCH_SATA_ENABLE:
        CustomFchSataClass (This, Action, QuestionId, Type, Value, ActionRequest, pSetup_Config, HiiHandle);
        break;
      case KEY_CBS_CMN_SOC_INIT_SPL_FOR_FAR:
      case KEY_CBS_CMN_SOC_FAR_SWITCH:
        UpdateHelpTextForFarSwitch (OptionsHelpString, pSetup_Config, HiiHandle);
        break;
      default:
        break;
      }

      break;
    case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
//[-start-220906-IB14740262-remove]//
//      CbsWriteDefalutValue(IfrData);
//[-end-220906-IB14740262-remove]//
      CbsComboIdentify (IfrData);
      switch (QuestionId) {
      default:
//[-start-220906-IB14740262-add]//
        Status = EFI_UNSUPPORTED;
//[-end-220906-IB14740262-add]//
        break;
      }
      break;
    default:
      break;
  }

  if (OptionsHelpString != NULL)
    gBS->FreePool(OptionsHelpString);

  return Status;
}

VOID
AmdSaveCbsConfigData (
  IN UINT8   *IfrData
  )
{
  EFI_STATUS                      Status;
  UINT32                          EAX_Reg;
  UINT32                          EBX_Reg;
  UINT32                          ECX_Reg;
  UINT32                          EDX_Reg;
  UINT64                          SocFamilyID;
  AMD_APCB_SERVICE_PROTOCOL       *mApcbDxeServiceProtocol;

  AsmCpuid (
      0x80000001,
      &EAX_Reg,
      &EBX_Reg,
      &ECX_Reg,
      &EDX_Reg
      );
  SocFamilyID = EAX_Reg & RAW_FAMILY_ID_MASK;

  if (SocFamilyID != F15_BR_RAW_ID) {
    DEBUG ((EFI_D_ERROR, "Update CBS linked APCB data\n"));
    Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Unable to locate APCB Protocol\n"));
    }
    //Call the auto generated code
    UpdateCbsApcbTokens (IfrData, mApcbDxeServiceProtocol);
  }

  return;
}

EFI_STATUS
EFIAPI
CbsSetupLoadDefaultFunc (
  )
{
  EFI_STATUS Status;
  CBS_CONFIG *AmdCbsConfig;

  Status = EFI_SUCCESS;
  // Load CBS default value
  AmdCbsConfig = AllocateZeroPool (CbsVariableSize);
  if (AmdCbsConfig != NULL) {
    CbsWriteDefalutValue ((UINT8 *) AmdCbsConfig);
    CbsComboIdentify ((UINT8 *) AmdCbsConfig);

    Status = gRT->SetVariable (
                CBS_SYSTEM_CONFIGURATION_NAME,
                &gCbsSystemConfigurationGuid,
                FixedPcdGet32(PcdAmdCbsVariableAttribute),
                CbsVariableSize,
                (VOID *)AmdCbsConfig
                );
    gBS->FreePool (AmdCbsConfig);
  }
  return Status;
}

VOID
InitializeCbsApcbDefault (
  IN UINT8   *IfrData
  )
{
  EFI_STATUS                      Status;
  CBS_CONFIG                      *Setup_Config;
  AMD_APCB_SERVICE_PROTOCOL       *pApcbDxeServiceProtocol;
  UINT32                          ApcbSyncSignature;
  UINT32                          NewSignature;
  UINT8                           ApcbPurpose;

  pApcbDxeServiceProtocol = NULL;
  ApcbSyncSignature = 0;
  NewSignature = 0;
  ApcbPurpose = 0;
  Setup_Config = (CBS_CONFIG *)IfrData;

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &pApcbDxeServiceProtocol);
  if (!EFI_ERROR (Status)) {
    // Initialize Cbs Apcb default for APCB token options.
    Status = pApcbDxeServiceProtocol->ApcbGetToken32 (pApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_CBS_SYNC_SIGNATURE, &ApcbSyncSignature);
    DEBUG ((EFI_D_ERROR, "Status = 0x%x, InitApcb: Header.ApcbVariableHash=0x%x, ApcbTokenHash=0x%x\n", Status, Setup_Config->Header.ApcbVariableHash, ApcbSyncSignature));
    NewSignature = GetApcbHash ((VOID *)Setup_Config, pApcbDxeServiceProtocol);
    DEBUG ((EFI_D_ERROR, "new ApcbTokenHash=0x%x\n", NewSignature));
    if ((EFI_ERROR (Status)) || (Setup_Config->Header.ApcbVariableHash != ApcbSyncSignature)) {
      Setup_Config->Header.ApcbVariableHash = NewSignature;
      //Call the auto generated code to update APCB token.
      UpdateCbsApcbTokens ((VOID *)Setup_Config, pApcbDxeServiceProtocol);
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Unable to locate APCB Protocol\n"));
  }

  return;
}

#define OPTION_SYSCFG_NUMBERAM5 3
#define OPTION_SYSCFG_NUMBERFP7 8

VOID
UpdateSystemConfig(
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_HII_HANDLE                       HiiHandle
)
{
  EFI_STATUS                  Status;
  VOID                        *StartOpCodeHandle;
  VOID                        *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL          *StartGuidLabel;
  EFI_IFR_GUID_LABEL          *EndGuidLabel;
  VOID                        *OptionsOpCodeHandle;

  UINT16                      StringSysConfigAM5[OPTION_SYSCFG_NUMBERAM5] = {
                                STRING_TOKEN (AMD_CBS_STR_35W),
                                STRING_TOKEN (AMD_CBS_STR_45W)
                              };
  UINT8                       SysConfigValueAM5[OPTION_SYSCFG_NUMBERAM5] = {1, 2, 3};

  UINT16                      StringSysConfigFP7[OPTION_SYSCFG_NUMBERFP7] = {
                                STRING_TOKEN (AMD_CBS_STR_10W),
                                STRING_TOKEN (AMD_CBS_STR_15W),
                                STRING_TOKEN (AMD_CBS_STR_25W),
                                STRING_TOKEN (AMD_CBS_STR_35W),
                                STRING_TOKEN (AMD_CBS_STR_45W),
                                STRING_TOKEN (AMD_CBS_STR_54W)
                              };
  UINT8                       SysConfigValueFP7[OPTION_SYSCFG_NUMBERFP7] = {1, 2, 3, 6, 7, 8};
  UINT16                      *pStringSysConfig;
  UINT8                       *pSysConfigValue;
  UINT8                       NumberOfOption;
  UINT8                       Index;
  UINT8                       RawPkgType;
  UINT32                      EAX_Reg;
  UINT32                      EBX_Reg;
  UINT32                      ECX_Reg;
  UINT32                      EDX_Reg;

  AsmCpuid (0x80000001, &EAX_Reg, &EBX_Reg, &ECX_Reg, &EDX_Reg);
  RawPkgType   = (EBX_Reg >> 28) & 0x0F;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "UpdateSystemConfig HiiAllocateOpCodeHandle StartOpCodeHandle fail"));
    return;
  }

  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (EndOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "UpdateSystemConfig HiiAllocateOpCodeHandle EndOpCodeHandle fail"));
    return;
  }

  // Create Hii Extended Label OpCode as the start and end opcode
  StartGuidLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  StartGuidLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartGuidLabel->Number = LABEL_CBS_CBS_CMN_GNB_SMU_SYSTEM_CONFIG_START;

  EndGuidLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndGuidLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndGuidLabel->Number = LABEL_CBS_CBS_CMN_GNB_SMU_SYSTEM_CONFIG_END;

  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (OptionsOpCodeHandle == NULL) {
    DEBUG ((EFI_D_ERROR, "UpdateSystemConfig HiiAllocateOpCodeHandle OptionsOpCodeHandle fail"));
    return;
  }

  //Create Options for CbsCmnGnbSMUSystemConfig
  HiiCreateOneOfOptionOpCode (
    OptionsOpCodeHandle,
    STRING_TOKEN (AMD_CBS_STR_AUTO),
    EFI_IFR_OPTION_DEFAULT,
    EFI_IFR_TYPE_NUM_SIZE_8,
    0xF
    );

  pStringSysConfig = &StringSysConfigFP7[0];
  pSysConfigValue = &SysConfigValueFP7[0];
  NumberOfOption = OPTION_SYSCFG_NUMBERFP7;
  if (RawPkgType == ZEN3_PKG_AM5) {
    pStringSysConfig = &StringSysConfigAM5[0];
    pSysConfigValue = &SysConfigValueAM5[0];
    NumberOfOption = OPTION_SYSCFG_NUMBERAM5;
  }

  for (Index = 0; Index < NumberOfOption; Index++) {
    HiiCreateOneOfOptionOpCode (
      OptionsOpCodeHandle,
      pStringSysConfig[Index],
      0,
      EFI_IFR_TYPE_NUM_SIZE_8,
      pSysConfigValue[Index]
      );
  }

  HiiCreateOneOfOpCode (
    StartOpCodeHandle,
    (EFI_QUESTION_ID) KEY_CBS_CMN_GNB_SMU_SYSTEM_CONFIG,
    CBS_CONFIGURATION_VARSTORE_ID,
    (UINT16)OFFSET_OF(CBS_CONFIG, CbsCmnGnbSMUSystemConfig),
    STRING_TOKEN (AMD_CBS_STR_SYSTEM_CONFIGURATION),
    STRING_TOKEN (AMD_CBS_STR_SYSTEM_CONFIGURATION_HELP),
    EFI_IFR_FLAG_RESET_REQUIRED,
    EFI_IFR_NUMERIC_SIZE_1,
    OptionsOpCodeHandle,
    NULL
    );

  Status = HiiUpdateForm (
    HiiHandle,
    &gAmdCbsFormsetIDGuid,
    SETUP_SMU_COMMON_OPTIONS_LABLE,
    StartOpCodeHandle,
    EndOpCodeHandle
    );
  DEBUG ((EFI_D_ERROR, "UpdateSystemConfig HiiUpdateForm: Status=0x%x\n", Status));

  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
  }
  if (EndOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }
  if (OptionsOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (OptionsOpCodeHandle);
  }
}

EFI_STATUS
AmdCbsFormDynamicUpdate (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest,
  IN  UINT8                                  *IfrData,
  IN  EFI_HII_HANDLE                         HiiHandle
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  CBS_CONFIG *Setup_Config;

  Setup_Config = (CBS_CONFIG *)IfrData;

  switch (Action) {
    case EFI_BROWSER_ACTION_FORM_OPEN:
      UpdateSystemConfig (This, HiiHandle);
      break;
    default:
      break;
  }

  return Status;
}

