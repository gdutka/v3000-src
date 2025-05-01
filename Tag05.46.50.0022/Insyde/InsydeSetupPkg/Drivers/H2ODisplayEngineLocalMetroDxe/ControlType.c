/** @file

  Functions for Control Type

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Guid/H2OControlType.h>
#include "H2ODisplayEngineLocalMetro.h"

typedef struct {
  EFI_GUID                            *Id;
  CHAR16                              *ClassName;
} CONTROL_TYPE_NODE;

CONTROL_TYPE_NODE                     mControlTypeList[] = {
  {&gH2OSetupLmdeActionButtonGuid         , L"ActionButton"         },
  {&gH2OSetupLmdeButtonGuid               , L"SetupMenuItem"        },
//{&gH2OSetupLmdeCheckboxPopupGuid        , L"H2OCheckboxDialog"    },
//{&gH2OSetupLmdeCheckboxImageGuid        , L"CheckboxImage"        },
  {&gH2OSetupLmdeDatePopupGuid            , L"H2ODateTimeDialog"    },
  {&gH2OSetupLmdeDateRollerGuid           , L"DateRoller"           },
//{&gH2OSetupLmdeDateFormattedGuid        , L"DateFormatted",       },
  {&gH2OSetupLmdeEditBoxGuid              , L"UiEdit"               },
//{&gH2OSetupLmdeGroupGuid                , L"Grop"                 },
  {&gH2OSetupLmdeHotkeyGuid               , L"HotkeyItem"           },
  {&gH2OSetupLmdeNumericInputGuid         , L"NumericInput"         },
  {&gH2OSetupLmdeNumericPopupGuid         , L"H2ONumericDialog"     },
  {&gH2OSetupLmdeOneOfPopupGuid           , L"H2OOneOfInputDialog"  },
  {&gH2OSetupLmdeOptionListGuid           , L"OptionList",          },
  {&gH2OSetupLmdeOrderedListPopupGuid     , L"H2OOrderedListDialog" },
  {&gH2OSetupLmdeOrderedListOptionListGuid, L"OrderedListOptionList"},
  {&gH2OSetupLmdePasswordInputGuid        , L"PasswordInput"        },
  {&gH2OSetupLmdePasswordPopupGuid        , L"H2OPasswordDialog"    },
  {&gH2OSetupLmdeStringInputGuid          , L"StringInput"          },
  {&gH2OSetupLmdeStringPopupGuid          , L"H2OStringDialog"      },
//{&gH2OSetupLmdeSubtitleGuid             , L"SubtitleItem"         },
//{&gH2OSetupLmdeTextGuid                 , L"TextItem"             },
  {&gH2OSetupLmdeTimePopupGuid            , L"H2ODateTimeDialog"    },
  {&gH2OSetupLmdeTimeRollerGuid           , L"TimeRoller"           },
  };

CHAR16 *
GetClassNameByControlType (
  IN EFI_GUID                           *ControlTypeId
  )
{
  UINT32                                Index;

  if (ControlTypeId == NULL) {
    return NULL;
  }

  for (Index = 0; Index < ARRAY_SIZE(mControlTypeList); Index++) {
    if (CompareGuid (ControlTypeId, mControlTypeList[Index].Id)) {
      return mControlTypeList[Index].ClassName;
    }
  }

  DEBUG ((EFI_D_WARN, "[LMDE] Control Type %g is not supported.\n", ControlTypeId));

  return NULL;
}

CHAR16 *
GetClassNameBySetupPageStatementControlType (
  IN H2O_FORM_BROWSER_S                *Statement
  )
{
  EFI_STATUS                           Status;
  H2O_PANEL_INFO                       *Panel;
  H2O_PROPERTY_VALUE                   ControlTypePropValue;

  if (Statement == NULL) {
    return NULL;
  }

  if (Statement->Signature != H2O_FORM_BROWSER_STATEMENT_SIGNATURE) {
    return NULL;
  }

  Status = GetPanelById (&Statement->FormsetGuid, &mMetroPrivate->DisplayEngine.Id, SETUP_PAGE_PANEL_ID, &Panel);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = GetStatementPropValue ("control-type", Statement, Panel, 0, 0, NULL, &ControlTypePropValue);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return GetClassNameByControlType (&ControlTypePropValue.H2OValue.Value.Guid);
}

