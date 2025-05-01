/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ***************************************************************************/

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
#include "AodCurveOptimizer.h"
AOD_MOUDLE_NAME ("AodCurveOptimizer")
AOD_SOC_NAME ("RMB")

extern EFI_GUID mAodFormSetGuid;
/**
  This function will dynamically add VFR contents to NBIO PBO page using HII library functions.

  @param[in]  HiiHandle     Hii Handle of AOD Formset

**/
VOID
GenPerCoreCurveOptimizerOptions(
  IN  EFI_HII_HANDLE              HiiHandle
)
{
  UINT8                            Index;
  CHAR16                           String[100];
  EFI_STRING_ID                    StringId = 0;
  VOID                             *StartOpCodeHandle;
  VOID                             *EndOpCodeHandle;
  VOID                             *DefaultOpCodeHandle;
  EFI_IFR_GUID_LABEL               *StartLabel;
  EFI_IFR_GUID_LABEL               *EndLabel;
  VOID                             *OptionsOpCodeHandle;
  UINTN                            CoreNumber;
  AOD_CORE_INFO                    AodInfo;
  EFI_STATUS                       Status;
  Status = AodGetCoreInfo (&AodInfo);
  if (EFI_ERROR (Status)) {
    return;
  }
  CoreNumber = AodInfo.AodLogicalCoreCount;
  AOD_DEBUG_SOC ((EFI_D_INFO, "AodLogicalActivceCoreNumber=%d\n", CoreNumber));

  if ((CoreNumber == 0) || (CoreNumber > 64)) {
    return;
  }

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  if ((StartOpCodeHandle == NULL) || (EndOpCodeHandle == NULL) || (OptionsOpCodeHandle == NULL)) {
    return;
  }

  // Create Hii Extended Label OpCode as the start and end opcode
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if(StartLabel == NULL || EndLabel == NULL) {
    return;
  }

  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_CBS_AOD_PER_CORE_CURVE_OPTIMIZER_DYNAMIC_START;
  EndLabel->Number = LABEL_CBS_AOD_PER_CORE_CURVE_OPTIMIZER_DYNAMIC_END;

  //Create Options for AodPerCoreCurveOptimizerSign
  HiiCreateOneOfOptionOpCode (
    OptionsOpCodeHandle,
    STRING_TOKEN (STR_AOD_CURVE_OPTIMIZER_SIGN_POSITIVE),
    EFI_IFR_OPTION_DEFAULT,
    EFI_IFR_TYPE_NUM_SIZE_8,
    0
    );

  HiiCreateOneOfOptionOpCode (
    OptionsOpCodeHandle,
    STRING_TOKEN (STR_AOD_CURVE_OPTIMIZER_SIGN_NEGATIVE),
    0,
    EFI_IFR_TYPE_NUM_SIZE_8,
    1
    );

  DefaultOpCodeHandle = HiiAllocateOpCodeHandle ();
  if (DefaultOpCodeHandle == NULL) return;
  HiiCreateDefaultOpCode (DefaultOpCodeHandle, EFI_HII_DEFAULT_CLASS_STANDARD, EFI_IFR_TYPE_NUM_SIZE_16, 0);
  for (Index = 0; Index < CoreNumber; Index++) {
    ZeroMem(String, sizeof(String));
    UnicodeSPrint (String, sizeof(String), L"Core %d Curve Optimizer Sign", Index);
    StringId = HiiSetString (HiiHandle, 0, String, NULL);
    HiiCreateOneOfOpCode (
      StartOpCodeHandle,
      (EFI_QUESTION_ID) OFFSET_OF(AOD_CONFIG, AodPerCoreCurveOptimizerSign[Index]),
      VARSTORE_ID_AODSETUP,
      (UINT16)OFFSET_OF(AOD_CONFIG, AodPerCoreCurveOptimizerSign[Index]),
      StringId,
      STRING_TOKEN (STR_AOD_PER_CORE_CURVE_OPTIMIZER_SIGN_HELP),
      EFI_IFR_FLAG_RESET_REQUIRED,
      EFI_IFR_NUMERIC_SIZE_1,
      OptionsOpCodeHandle,
      NULL
      );

    ZeroMem(String, sizeof(String));
    UnicodeSPrint (String, sizeof(String), L"Core %d Curve Optimizer Magnitude", Index);
    StringId = HiiSetString (HiiHandle, 0, String, NULL);
    HiiCreateNumericOpCode (
      StartOpCodeHandle,
      (EFI_QUESTION_ID) OFFSET_OF(AOD_CONFIG, AodPerCoreCurveOptimizerMagnitude[Index]),
      VARSTORE_ID_AODSETUP,
      (UINT16)OFFSET_OF(AOD_CONFIG, AodPerCoreCurveOptimizerMagnitude[Index]),
      StringId,
      STRING_TOKEN (STR_AOD_PER_CORE_CURVE_OPTIMIZER_MAGNITUDE_HELP),
      EFI_IFR_FLAG_RESET_REQUIRED,
      EFI_IFR_NUMERIC_SIZE_2 | EFI_IFR_DISPLAY_UINT_DEC,
      0,
      30,
      1,
      DefaultOpCodeHandle
      );
  }

  Status = HiiUpdateForm (
             HiiHandle,
             &mAodFormSetGuid,
             SETUP_CURVE_OPTIMIZER_LABLE,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "HiiUpdateForm Status = %r \n", Status));
    return;
  }
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