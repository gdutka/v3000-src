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
AOD_SOC_NAME ("RPL")

extern EFI_GUID mAodFormSetGuid;
/**
  This function used to generate per core curve optimizer

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGeneratePerCoreCurveOptionsRpl (
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
  if (EFI_ERROR (Status)) return Status;

  CoreNumber = AodInfo.AodLogicalCoreCount;
  AOD_DEBUG_SOC ((EFI_D_INFO, "AodLogicalActivceCoreNumber=%d\n", CoreNumber));

  if ((CoreNumber == 0) || (CoreNumber > 64)) {
    return EFI_UNSUPPORTED;
  }

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  DefaultOpCodeHandle = HiiAllocateOpCodeHandle ();
  if ((StartOpCodeHandle == NULL) || (EndOpCodeHandle == NULL) ||\
      (OptionsOpCodeHandle == NULL) || (DefaultOpCodeHandle == NULL)) {
    return EFI_OUT_OF_RESOURCES;
  }

  // Create Hii Extended Label OpCode as the start and end opcode
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if(StartLabel == NULL || EndLabel == NULL) {
    return EFI_OUT_OF_RESOURCES;
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
    return Status;
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
  return EFI_SUCCESS;
}

/**
  This function used to generate per CCD Freq optimizer

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateCcdFreqOptimizerOptionsRpl (
  IN  EFI_HII_HANDLE              HiiHandle
  )
{
  VOID                             *StartOpCodeHandle;
  VOID                             *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL               *StartLabel;
  EFI_IFR_GUID_LABEL               *EndLabel;
  VOID                             *DefaultOpCodeHandle;
  AOD_CONFIG                       *AodConfig;
  EFI_STATUS                       Status;
  CHAR16                           pTemp[0x40];
  AOD_CORE_INFO                    CoreInfo;
  UINT8                            CcdIndex;

  Status = AodGetConfiguration (&AodConfig);
  if (EFI_ERROR (Status)) return Status;

  Status = AodGetCoreInfo (&CoreInfo);
  if (EFI_ERROR (Status)) return Status;

  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  DefaultOpCodeHandle = HiiAllocateOpCodeHandle ();
  if ((StartOpCodeHandle == NULL) || (EndOpCodeHandle == NULL) || (DefaultOpCodeHandle == NULL)) {
    return EFI_OUT_OF_RESOURCES;
  }
  // Create Hii Extended Label OpCode as the start and end opcode
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if(StartLabel == NULL || EndLabel == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_CBS_AOD_CCD_FREQUENCY_ADJUSTMENT_START;
  EndLabel->Number = LABEL_CBS_AOD_CCD_FREQUENCY_ADJUSTMENT_END;
  HiiCreateDefaultOpCode (DefaultOpCodeHandle, EFI_HII_DEFAULT_CLASS_STANDARD, EFI_IFR_TYPE_NUM_SIZE_16, 0);
  for (CcdIndex = 0; CcdIndex < CoreInfo.AodPhysicalCcdCount; CcdIndex ++) {
    if ((CoreInfo.AodCcdBitMap & (1 << CcdIndex)) == 0) continue;
    ZeroMem (pTemp, 0x80);
    UnicodeSPrint(pTemp, 0x80, L"CCD%d Frequency Adjustment\n",CcdIndex);
    HiiCreateNumericOpCode (
      StartOpCodeHandle,
      (UINT16) OFFSET_OF (AOD_CONFIG, AodCcdOptimizerFreq[CcdIndex]),
      VARSTORE_ID_AODSETUP,
      (UINT16) OFFSET_OF (AOD_CONFIG, AodCcdOptimizerFreq[CcdIndex]),
      HiiSetString (HiiHandle, 0, pTemp, NULL),
      STRING_TOKEN (STR_AOD_PER_CDD_FREQ_OPT_HELP),
      EFI_IFR_FLAG_RESET_REQUIRED,
      EFI_IFR_NUMERIC_SIZE_2 | EFI_IFR_DISPLAY_UINT_DEC,
      0,
      0xffff,
      1,
      DefaultOpCodeHandle
      );
  }

  Status = HiiUpdateForm (
             HiiHandle,
             &mAodFormSetGuid,
             SETUP_CCD_FREQUENCY_OPTIMIZER_LABLE,
             StartOpCodeHandle,
             EndOpCodeHandle
             );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "HiiUpdateForm Status = %r \n", Status));
    return Status;
  }
  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
  }
  if (EndOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }
  if (DefaultOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (DefaultOpCodeHandle);
  }
  return EFI_SUCCESS;
}