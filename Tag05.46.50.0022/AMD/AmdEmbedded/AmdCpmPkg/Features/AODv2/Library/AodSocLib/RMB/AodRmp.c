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
#include "AodRmp.h"

AOD_MOUDLE_NAME ("AodRmp")
AOD_SOC_NAME ("RMB")
extern EFI_GUID mAodFormSetGuid;
extern
UINT8 *
EFIAPI
InternalHiiCreateOpCodeExtended (
  IN VOID   *OpCodeHandle,
  IN VOID   *OpCodeTemplate,
  IN UINT8  OpCode,
  IN UINTN  OpCodeSize,
  IN UINTN  ExtensionSize,
  IN UINT8  Scope
  );
/**
  This function used to Get Rmp data from APOB

  @param[OUT]  RmpData                - the Rmp data from APOB

  @retval EFI_SUCCESS       read Rmp data successfully
          EFI_UNSUPPORTED   Rmp data is invalid
**/
EFI_STATUS
EFIAPI
AodGetRmpData (
  OUT  VOID              **RmpData
  )
{
  EFI_STATUS                        Status;
  APOB_MEM_RMP_INFO_STRUCT          *ApobEntry;
  BOOLEAN                           IsRmpAvailable = FALSE;

  Status = AmdPspGetApobEntryInstance (APOB_MEM, APOB_MEM_RMP_INFO, 0, FALSE, (APOB_TYPE_HEADER **) &ApobEntry);
  if (Status == EFI_SUCCESS) {
    IsRmpAvailable = ApobEntry->IsRmpAvailable;
  }
  if (IsRmpAvailable) {
    *RmpData = ApobEntry;
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

/**
  This function used to generate Rmp options if Rmp Supported

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateRmpOptions (
  IN  EFI_HII_HANDLE              HiiHandle
  )
{
  UINT8                            Index;
  CHAR16                           String[100];
  EFI_STRING_ID                    StringId = 0;
  VOID                             *StartOpCodeHandle;
  VOID                             *EndOpCodeHandle;
  VOID                             *OptionsOpCodeHandle;
  VOID                             *OptionsOpCodeHandle1;
  EFI_IFR_GUID_LABEL               *StartLabel;
  EFI_IFR_GUID_LABEL               *EndLabel;
  EFI_STATUS                       Status;
  EFI_IFR_SUPPRESS_IF              SuprressifOpCode;
  EFI_IFR_EQ_ID_VAL                EqualValOpCode;
  EFI_IFR_NOT                      NotOpCode;
  EFI_IFR_AND                      AndOpCode;
  EFI_IFR_NOT                      EndOpCode;
  APOB_MEM_RMP_INFO_STRUCT         *Rmp;

  if (EFI_ERROR (AodGetRmpData ((VOID **)&Rmp))) {
    return EFI_UNSUPPORTED;
  }
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();
  OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
  OptionsOpCodeHandle1 = HiiAllocateOpCodeHandle ();
  if ((StartOpCodeHandle == NULL) || (EndOpCodeHandle == NULL) || (OptionsOpCodeHandle == NULL) || (OptionsOpCodeHandle1 == NULL)) {
    return EFI_OUT_OF_RESOURCES;
  }
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if ((StartLabel == NULL) || (EndLabel == NULL)) {
    return EFI_OUT_OF_RESOURCES;
  }
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number = LABEL_CBS_AOD_RMP_START;
  EndLabel->Number = LABEL_CBS_AOD_RMP_END;

  //
  // Add OneOfOptions "Auto"
  //
  HiiCreateOneOfOptionOpCode (
    OptionsOpCodeHandle,
    STRING_TOKEN (STR_AOD_AUTO),
    EFI_IFR_OPTION_DEFAULT,
    EFI_IFR_TYPE_NUM_SIZE_8,
    0xff
    );
  //
  // Add OneOfOptions "Enable"
  //
  //HiiCreateOneOfOptionOpCode (
  //  OptionsOpCodeHandle,
  //  STRING_TOKEN (STR_AOD_ENABLE),
  //  0,
  //  EFI_IFR_TYPE_NUM_SIZE_8,
  //  1
  //  );
  //
  // Add OneOfOptions "Disable"
  //
  //HiiCreateOneOfOptionOpCode (
  //  OptionsOpCodeHandle,
  //  STRING_TOKEN (STR_AOD_DISABLE),
  //  0,
  //  EFI_IFR_TYPE_NUM_SIZE_8,
  //  0
  //  );
  //
  // Add OneOfOptions "DDR OC Profile to Manual DDR OC"
  //
  HiiCreateOneOfOptionOpCode (
    OptionsOpCodeHandle,
    STRING_TOKEN (STR_AOD_ENABLE),
    0,
    EFI_IFR_TYPE_NUM_SIZE_8,
    2
    );
  //
  // Add OneOf "Rmp Profile"
  //
  HiiCreateOneOfOpCode (
    StartOpCodeHandle,
    KEY_AOD_RMP_MODE,
    VARSTORE_ID_AODSETUP,
    (UINT16)OFFSET_OF(AOD_CONFIG, AodExpo),
    STRING_TOKEN (STR_AOD_EXPO),
    STRING_TOKEN (STR_AOD_EXPO_HELP),
    EFI_IFR_FLAG_RESET_REQUIRED | EFI_IFR_FLAG_CALLBACK,
    EFI_IFR_NUMERIC_SIZE_1,
    OptionsOpCodeHandle,
    NULL
    );
  //
  // Create SupressIf Opcode
  //
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &SuprressifOpCode,
    EFI_IFR_SUPPRESS_IF_OP,
    sizeof (SuprressifOpCode),
    0,
    1
    );
  //
  // Create EqualVal Opcode with scope
  //
  EqualValOpCode.QuestionId = KEY_AOD_RMP_MODE;
  EqualValOpCode.Value = 1; //Rmp Mode = Enable
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &EqualValOpCode,
    EFI_IFR_EQ_ID_VAL_OP,
    sizeof (EqualValOpCode),
    0,
    1
    );
  //
  // Create Not Opcode
  //
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &NotOpCode,
    EFI_IFR_NOT_OP,
    sizeof (NotOpCode),
    0,
    0
    );
  //
  // Create EqualVal Opcode without scope
  //
  EqualValOpCode.QuestionId = KEY_AOD_RMP_MODE;
  EqualValOpCode.Value = 2; //Rmp Mode = RMP Profile to OC
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &EqualValOpCode,
    EFI_IFR_EQ_ID_VAL_OP,
    sizeof (EqualValOpCode),
    0,
    0
    );
  //
  // Create Not Opcode
  //
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &NotOpCode,
    EFI_IFR_NOT_OP,
    sizeof (NotOpCode),
    0,
    0
    );
  //
  // Create And Opcode
  //
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &AndOpCode,
    EFI_IFR_AND_OP,
    sizeof(EFI_IFR_OR),
    0,
    0
    );
  //
  // Create End Opcode for EuqalVal
  //
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &EndOpCode,
    EFI_IFR_END_OP,
    sizeof (EndOpCode),
    0,
    0
    );
  //
  // Create Suppressif opcode
  //
  for (Index = 0; Index < Rmp->NumberOfProfiles; Index++) {
    ZeroMem(String, sizeof(String));
    //
    // Add OneOfOptions like format "Profile0 - DDR4800 CL48"
    //
    UnicodeSPrint (String, sizeof(String), L"Profile%d - DDR%04d CL%02d", Index,\
      (UINT16)(Rmp->Profiles[Index].MemClk * 2), Rmp->Profiles[Index].Tcl);
    StringId = HiiSetString (HiiHandle, 0, String, NULL);
    if (StringId != 0) {
      HiiCreateOneOfOptionOpCode (
        OptionsOpCodeHandle1,
        StringId,
        EFI_IFR_FLAG_RESET_REQUIRED,
        EFI_IFR_TYPE_NUM_SIZE_8,
        Index
        );
    }
  }
  //
  // Add OneOf "Rmp Profile"
  //
  HiiCreateOneOfOpCode (
    StartOpCodeHandle,
    KEY_AOD_RMP_PROFILE,
    VARSTORE_ID_AODSETUP,
    (UINT16)OFFSET_OF(AOD_CONFIG, AodExpoProfile),
    STRING_TOKEN (STR_AOD_EXPO_PROFILE),
    STRING_TOKEN (STR_AOD_EXPO_PROFILE_HELP),
    EFI_IFR_FLAG_RESET_REQUIRED | EFI_IFR_FLAG_CALLBACK,
    EFI_IFR_NUMERIC_SIZE_1,
    OptionsOpCodeHandle1,
    NULL
    );
  //
  // Create End Opcode for SupressIf
  //
  InternalHiiCreateOpCodeExtended (
    StartOpCodeHandle,
    &EndOpCode,
    EFI_IFR_END_OP,
    sizeof (EndOpCode),
    0,
    0
    );
  Status = HiiUpdateForm (
             HiiHandle,
             &mAodFormSetGuid,
             SETUP_DDR_TIMING_CONFIGURATION_LABLE,
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
  if (OptionsOpCodeHandle1 != NULL) {
    HiiFreeOpCodeHandle (OptionsOpCodeHandle1);
  }
  return EFI_SUCCESS;
}

/**

  Determine the Clock Timing, and check it against any clock minimum.

  @param[in]     Memclk          Current CK Frequency @todo: Should this just be CK Cycletime?
  @param[in]     MinCycleTime    Minimum Number of Picoseconds, 0 if nCK value is desired
  @param[in]     MinClocks       Minimum nCK, if 0 no Minimum

  @retval        AC Timing Value in nCK
**/
UINT32
AodCalcMemTimingnCk (
  IN  UINT32        Memclk,
  IN  UINT32        MinCycleTime,
  IN  UINT32        MinClocks
  )
{
  UINT32 nCK;
  //
  // If MinCycleTime is NonZero, Calculate the clocks at the
  // supplied frequency and return the greater of nCK and
  // MinClocks
  //
  if (MinCycleTime != 0) {
    //
    // Convert the pS value to clocks
    //
    nCK = AOD_PS_TO_CLKs (Memclk, MinCycleTime);
    //
    // Check the lower clock limit
    //
    if (nCK < MinClocks) {
      nCK = (INT32) MinClocks;
    }
  } else {
    //
    // If MinCycleTIme is Zero, then just pass MinClocks
    //
    nCK = (INT32) MinClocks;
  }
  return (UINT32)nCK;
}

/**

  Load RMP Profile and Apply it into Manual OC Options

  @param[in]     ApplyRmp         Apply RMP settings or not
  @param[in]     ProfileSelect    Profile number of RMP
  @param[in]     Rmp              Rmp data from APOB
  @param[in]     Variable         AodVariable

  @retval        EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
AodRmpCalulateParameter (
  IN  BOOLEAN                           ApplyRmp,
  IN  UINT8                             ProfileSelect,
  IN  APOB_MEM_RMP_INFO_STRUCT          *Rmp,
  IN  VOID                              *Variable
  )
{
  AOD_CONFIG                      *AodConfig;
  UINT16                          MemClk;
  RMP_PROFILE                     *Profile;
  RMP_PROFILE_BLOCK_0             *ProfileBlock0;
  RMP_PROFILE_BLOCK_1             *ProfileBlock1;

  Profile = (RMP_PROFILE *)&Rmp->RmpSpdData[0];
  ProfileBlock0 = &Profile->RmpProfileBlock[ProfileSelect].Block0;
  ProfileBlock1 = &Profile->RmpProfileBlock[ProfileSelect].Block1;
  AodConfig     = (AOD_CONFIG *)Variable;
  if (ApplyRmp) {
    // ================================= Block0 =================================
    //
    // SDRAM VDD
    //
    AodConfig->AodVddIoCtrl = 1;
    AodConfig->AodApuVddIo = AOD_VOLTAGE (ProfileBlock0->VDD.Raw);
    AodConfig->AodDimmVdd  = AOD_VOLTAGE (ProfileBlock0->VDD.Raw);
    AodConfig->AodDimmVddq = AOD_VOLTAGE (ProfileBlock0->VDDQ.Raw);
    // VPP
    AodConfig->AodVppCtrl  = 1;
    AodConfig->AodVppVoltage  = AOD_VOLTAGE (ProfileBlock0->VPP.Raw);
    // VDDQ:TBD
    //CtCKAVGmin
    AodConfig->AodMemTimingSettingDdr = 1;
    AodConfig->AodMemTargetSpeedDdr = Rmp->Profiles[ProfileSelect].MemClk * 2;
    MemClk                          = Rmp->Profiles[ProfileSelect].MemClk;
    //
    // If DDR speed > DDR 4800, set VDD_MEM to 950mV by default
    //
    if ((Rmp->Profiles[ProfileSelect].MemClk * 2) > 4800) {
      AodConfig->AodVddMemCtrl = 1;
      AodConfig->AodVddMemAdjust = 950;
    }
    //tAAmin
    AodConfig->AodMemTimingTclCtrlDdr = 1;
    AodConfig->AodMemTimingTclDdr   = Rmp->Profiles[ProfileSelect].Tcl;
    AOD_MEM_ROUND_DOWN_BY_STEP (AodConfig->AodMemTimingTclDdr, 2);
    //tRCDmin
    if (ProfileBlock0->tRCDmin.Raw) {
      AodConfig->AodMemTimingTrcdCtrlDdr = 1;
      AodConfig->AodMemTimingTrcdDdr   = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock0->tRCDmin.Raw, 0);
    }
    //tRPmin
    if (ProfileBlock0->tRPmin.Raw) {
      AodConfig->AodMemTimingTrpCtrlDdr = 1;
      AodConfig->AodMemTimingTrpDdr   = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock0->tRPmin.Raw, 0);
    }
    //tRASmin
    if (ProfileBlock0->tRASmin.Raw) {
      AodConfig->AodMemTimingTrasCtrlDdr = 1;
      AodConfig->AodMemTimingTrasDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock0->tRASmin.Raw, 0);
    }
    //tRCmin
    if (ProfileBlock0->tRCmin.Raw) {
      AodConfig->AodMemTimingTrcCtrlDdr = 1;
      AodConfig->AodMemTimingTrcDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock0->tRCmin.Raw, 0);
    }
    //tWRmin
    if (ProfileBlock0->tWRmin.Raw) {
      AodConfig->AodMemTimingTwrCtrlDdr = 1;
      AodConfig->AodMemTimingTwrDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock0->tWRmin.Raw, 0);
    }
    //tRFC1min
    if (ProfileBlock0->tRFC1min.Raw) {
      AodConfig->AodMemTimingTrfc1CtrlDdr = 1;
      AodConfig->AodMemTimingTrfc1Ddr = (UINT16)AodCalcMemTimingnCk (MemClk, (ProfileBlock0->tRFC1min.Raw * 1000), 0);
    }
    //tRFC2min
    if (ProfileBlock0->tRFC2min.Raw) {
      AodConfig->AodMemTimingTrfc2CtrlDdr = 1;
      AodConfig->AodMemTimingTrfc2Ddr = (UINT16)AodCalcMemTimingnCk (MemClk, (ProfileBlock0->tRFC2min.Raw * 1000), 0);
    }
    //tRFCsbmin
    if (ProfileBlock0->tRFCsbmin.Raw) {
      AodConfig->AodMemTimingTrfcSbCtrlDdr = 1;
      AodConfig->AodMemTimingTrfcSbDdr = (UINT16)AodCalcMemTimingnCk (MemClk, (ProfileBlock0->tRFCsbmin.Raw * 1000), 0);
    }
    AOD_DEBUG_SOC ((EFI_D_INFO, "Profile %d to OC settings : \n", ProfileSelect));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  SDRAM VDD [6:5]=%x,[4:1]=%x,[0]=%x transfer to ApuVDDIO = %dmv, Dimm VDD = %dmv\n",
      ProfileBlock0->VDD.Bits.VDD_1,
      ProfileBlock0->VDD.Bits.VDD_0_DOT_1,
      ProfileBlock0->VDD.Bits.VDD_0_DOT_05,
      AodConfig->AodApuVddIo,
      AodConfig->AodDimmVdd
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  SDRAM VDDQ [6:5]=%x,[4:1]=%x,[0]=%x transfer to Dimm VDDQ = %dmv\n",
      ProfileBlock0->VDDQ.Bits.VDD_1,
      ProfileBlock0->VDDQ.Bits.VDD_0_DOT_1,
      ProfileBlock0->VDDQ.Bits.VDD_0_DOT_05,
      AodConfig->AodDimmVdd
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  SDRAM VPP [6:5]=%x, [4:1]=%x, [0]=%x transfer to VPP = %x mv\n",
      ProfileBlock0->VPP.Bits.VDD_1,
      ProfileBlock0->VPP.Bits.VDD_0_DOT_1,
      ProfileBlock0->VPP.Bits.VDD_0_DOT_05,
      AodConfig->AodVppVoltage
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tCKAVGmin LSB=%x,MSB=%x transfer to Memory Target Speed = %d MTs Vdd = %dmv\n",
      ProfileBlock0->tCKAVGmin.Bits.Lsb,
      ProfileBlock0->tCKAVGmin.Bits.Msb,
      MemClk * 2,
      AodConfig->AodVddMemAdjust
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tAAmin LSB=%x,MSB=%x transfer to tCL = %d CLK\n",
      ProfileBlock0->tAAmin.Bits.Lsb,
      ProfileBlock0->tAAmin.Bits.Msb,
      AodConfig->AodMemTimingTclDdr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRCDmin LSB=%x,MSB=%x transfer to TrcdD = %d CLK\n",
      ProfileBlock0->tRCDmin.Bits.Lsb,
      ProfileBlock0->tRCDmin.Bits.Msb,
      AodConfig->AodMemTimingTrcdDdr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRPmin LSB=%x,MSB=%x transfer to Trp = %d CLK\n",
      ProfileBlock0->tRPmin.Bits.Lsb,
      ProfileBlock0->tRPmin.Bits.Msb,
      AodConfig->AodMemTimingTrpDdr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRASmin LSB=%x,MSB=%x transfer to Tras = %d CLK\n",
      ProfileBlock0->tRASmin.Bits.Lsb,
      ProfileBlock0->tRASmin.Bits.Msb,
      AodConfig->AodMemTimingTrasDdr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRCmin LSB=%x,MSB=%x transfer to Trc = %d CLK\n",
      ProfileBlock0->tRCmin.Bits.Lsb,
      ProfileBlock0->tRCmin.Bits.Msb,
      AodConfig->AodMemTimingTrcDdr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tWRmin LSB=%x,MSB=%x transfer to Twr = %d CLK\n",
      ProfileBlock0->tWRmin.Bits.Lsb,
      ProfileBlock0->tWRmin.Bits.Msb,
      AodConfig->AodMemTimingTwrDdr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRFC1min LSB=%x,MSB=%x transfer to Trfc1 = %d CLK\n",
      ProfileBlock0->tRFC1min.Bits.Lsb,
      ProfileBlock0->tRFC1min.Bits.Msb,
      AodConfig->AodMemTimingTrfc1Ddr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRFC2min LSB=%x,MSB=%x transfer to Trfc2 = %d CLK\n",
      ProfileBlock0->tRFC1min.Bits.Lsb,
      ProfileBlock0->tRFC1min.Bits.Msb,
      AodConfig->AodMemTimingTrfc2Ddr
      ));
    AOD_DEBUG_SOC ((EFI_D_INFO, "  tRFCsbmin LSB=%x,MSB=%x transfer to TrfcSb = %d CLK\n",
      ProfileBlock0->tRFCsbmin.Bits.Lsb,
      ProfileBlock0->tRFCsbmin.Bits.Msb,
      AodConfig->AodMemTimingTrfcSbDdr
      ));
    // ================================= Block1 =================================
    if (((Profile->Header.RmpOptionalBlock.Profile0Block1Support == 1) && (ProfileSelect == 0)) ||
        ((Profile->Header.RmpOptionalBlock.Profile1Block1Support == 1) && (ProfileSelect == 1))) {
      // tRRD_Lmin
      if (ProfileBlock1->tRRD_Lmin.Raw) {
        AodConfig->AodMemTimingTrrdLCtrlDdr = 1;
        AodConfig->AodMemTimingTrrdLDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock1->tRRD_Lmin.Raw, 0);
      }
      AOD_DEBUG_SOC ((EFI_D_INFO, "  tRRD_Lmin LSB=%x,tRRD_Lmin MSB=%x transfer to TrrdL = %d CLK\n",
        ProfileBlock1->tRRD_Lmin.Bits.Lsb,
        ProfileBlock1->tRRD_Lmin.Bits.Msb,
        AodConfig->AodMemTimingTrrdLDdr
        ));
      //TBD: tCCD_Lmin tCCD_L_WRmin tCCD_L_WR2min
      //tFAWmin
      if (ProfileBlock1->tFAWmin.Raw) {
        AodConfig->AodMemTimingTfawCtrlDdr = 1;
        AodConfig->AodMemTimingTfawDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock1->tFAWmin.Raw, 0);
      }
      AOD_DEBUG_SOC ((EFI_D_INFO, "  tFAWmin LSB=%x,tFAWmin MSB=%x transfer to Tfaw = %d CLK\n",
        ProfileBlock1->tFAWmin.Bits.Lsb,
        ProfileBlock1->tFAWmin.Bits.Msb,
        AodConfig->AodMemTimingTfawDdr
        ));
      //tWTR_Lmin
      if (ProfileBlock1->tWTR_Lmin.Raw) {
        AodConfig->AodMemTimingTwtrLCtrlDdr = 1;
        AodConfig->AodMemTimingTwtrLDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock1->tWTR_Lmin.Raw, 0);
      }
      AOD_DEBUG_SOC ((EFI_D_INFO, "  tWTR_Lmin LSB=%x,tWTR_Lmin MSB=%x transfer to TwtrL = %d CLK\n",
        ProfileBlock1->tRRD_Lmin.Bits.Lsb,
        ProfileBlock1->tRRD_Lmin.Bits.Msb,
        AodConfig->AodMemTimingTwtrLDdr
        ));
      //tWTR_Smin
      if ( ProfileBlock1->tWTR_Smin.Raw) {
        AodConfig->AodMemTimingTwtrSCtrlDdr = 1;
        AodConfig->AodMemTimingTwtrSDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock1->tWTR_Smin.Raw, 0);
      }
      AOD_DEBUG_SOC ((EFI_D_INFO, "  tWTR_Smin LSB=%x,tWTR_Smin MSB=%x transfer to TwtrS = %d CLK\n",
        ProfileBlock1->tRRD_Lmin.Bits.Lsb,
        ProfileBlock1->tRRD_Lmin.Bits.Msb,
        AodConfig->AodMemTimingTwtrSDdr
        ));
      //tRTPmin
      if ( ProfileBlock1->tRTPmin.Raw) {
        AodConfig->AodMemTimingTrtpCtrlDdr = 1;
        AodConfig->AodMemTimingTrtpDdr = (UINT16)AodCalcMemTimingnCk (MemClk, ProfileBlock1->tRTPmin.Raw, 0);
      }
      AOD_DEBUG_SOC ((EFI_D_INFO, "  tRTPmin LSB=%x,tRTPmin MSB=%x transfer to TwtrS = %d CLK\n",
        ProfileBlock1->tRRD_Lmin.Bits.Lsb,
        ProfileBlock1->tRRD_Lmin.Bits.Msb,
        AodConfig->AodMemTimingTrtpDdr
        ));
    }
  }

  return EFI_SUCCESS;
}
/**
  This function used to set VDDIO Phy voltage, when select a Rmp mode.

  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodExpoCallback (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  APOB_MEM_RMP_INFO_STRUCT         *Rmp;
  AOD_CONFIG                       *AodConfig;
  RMP_PROFILE                      *Profile;
  RMP_PROFILE_BLOCK_0              *ProfileBlock0;
  if ((Action != EFI_BROWSER_ACTION_CHANGED) || (IfrData == NULL)) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != KEY_AOD_RMP_MODE) {
    return EFI_UNSUPPORTED;
  }
  if (EFI_ERROR (AodGetRmpData ((VOID **)&Rmp))) {
    return EFI_UNSUPPORTED;
  }
  AodConfig = (AOD_CONFIG *)IfrData;
  if ((Value->u8 == 1) && (AodConfig->AodExpoProfile < Rmp->NumberOfProfiles)) {
    AodConfig->AodVddIoCtrl = 2;
    Profile = (RMP_PROFILE *)&Rmp->RmpSpdData[0];
    ProfileBlock0 = &Profile->RmpProfileBlock[AodConfig->AodExpoProfile].Block0;
    AodConfig->AodDimmVdd  = AOD_VOLTAGE (ProfileBlock0->VDD.Raw);
    AodConfig->AodDimmVddq = AOD_VOLTAGE (ProfileBlock0->VDDQ.Raw);
    AodConfig->AodApuVddIo = AodConfig->AodDimmVdd;
    //
    // If DDR speed > DDR 4800, set VDD_MEM to 950mV by default
    //
    if ((Rmp->Profiles[AodConfig->AodExpoProfile].MemClk * 2) > 4800) {
      AodConfig->AodVddMemCtrl = 1;
      AodConfig->AodVddMemAdjust = 950;
    } else {
      AodConfig->AodVddMemCtrl = 0;
      AodConfig->AodVddMemAdjust = 780;
    }
  }
  if (Value->u8 == 2) {
    AodRmpCalulateParameter (TRUE, AodConfig->AodExpoProfile, Rmp, IfrData);
  }

  return EFI_SUCCESS;
}
/**
  This function used to set VDDIO Phy voltage, when select a Rmp profile.

  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
  @retval EFI_UNSUPPORTED if not action changing or key value not matched
          EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodExpoProfileSelectCallback (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  APOB_MEM_RMP_INFO_STRUCT         *Rmp;
  AOD_CONFIG                       *AodConfig;
  RMP_PROFILE                      *Profile;
  RMP_PROFILE_BLOCK_0              *ProfileBlock0;
  if ((Action != EFI_BROWSER_ACTION_CHANGED) || (IfrData == NULL)) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != KEY_AOD_RMP_PROFILE) {
    return EFI_UNSUPPORTED;
  }
  if (EFI_ERROR (AodGetRmpData ((VOID **)&Rmp))) {
    return EFI_UNSUPPORTED;
  }
  AodConfig = (AOD_CONFIG *)IfrData;
  if (Value->u8 < Rmp->NumberOfProfiles) {
    AodConfig->AodVddIoCtrl = 2;
    Profile = (RMP_PROFILE *)&Rmp->RmpSpdData[0];
    ProfileBlock0 = &Profile->RmpProfileBlock[AodConfig->AodExpoProfile].Block0;
    AodConfig->AodDimmVdd  = AOD_VOLTAGE (ProfileBlock0->VDD.Raw);
    AodConfig->AodDimmVddq = AOD_VOLTAGE (ProfileBlock0->VDDQ.Raw);
    AodConfig->AodApuVddIo = AodConfig->AodDimmVdd;
    //
    // If DDR speed > DDR 4800, set VDD_MEM to 950mV by default
    //
    if ((Rmp->Profiles[Value->u8].MemClk * 2) > 4800) {
      AodConfig->AodVddMemCtrl = 1;
      AodConfig->AodVddMemAdjust = 950;
    } else {
      AodConfig->AodVddMemCtrl = 0;
      AodConfig->AodVddMemAdjust = 780;
    }
  }
  if ((AodConfig->AodExpo == 2) && (Value->u8 < Rmp->NumberOfProfiles)) {
    AodRmpCalulateParameter (TRUE, Value->u8, Rmp, IfrData);
  }
  return EFI_SUCCESS;
}

/**
  Update WMI Class RMPPackage
  class RMPPackage
{
        [WmiDataId(1),
        read, write,
        Description("Is RMP available")
        ] uint8 IsRmpAvailable;
        [WmiDataId(2),
        read, write,
        Description("Number of RMP Profiles")
        ] uint8 NumberOfProfiles;
        [WmiDataId(3),
        read, write,
        Description("Basic Info : Profile Enable State in RMP SPD ")
        ] uint8 IsProfileEnable[2];
        [WmiDataId(4),
        read, write,
        Description("Basic Info : Profile Memory Clock units MHz")
        ] uint16 MemoryClock[2];
        [WmiDataId(5),
        read, write,
        Description("Basic Info : CAS Latency in CLK")
        ] uint16 CASLatency[2];
        [WmiDataId(6),
         read, write,
         Description("RMP SPD Data")
        ] uint8 Data[128];
};

  @param[in]  Nvs     the AOD global NVS

  @retval EFI_SUCCESS if callback is successful
          others by callee
**/
EFI_STATUS
EFIAPI
AodRMPUpdateNvs (
  IN  VOID            *Nvs
  )
{
  APOB_MEM_RMP_INFO_STRUCT         *Rmp;
  AodNvAreaStruct                  *AodNvs;
  UINT8                             Index;
  if (Nvs == NULL) return EFI_INVALID_PARAMETER;
  if (EFI_ERROR (AodGetRmpData ((VOID **)&Rmp))) {
    return EFI_UNSUPPORTED;
  }
  AodNvs = (AodNvAreaStruct  *)Nvs;
  AodNvs->AodRmpPackData.IsRmpAvailable = Rmp->IsRmpAvailable;
  AodNvs->AodRmpPackData.NumberOfProfiles = (UINT8)Rmp->NumberOfProfiles;
  for (Index = 0; Index < AodNvs->AodRmpPackData.NumberOfProfiles; Index ++) {
    AodNvs->AodRmpPackData.IsProfileEnable[Index] = Rmp->Profiles[Index].ProfileEn;
    AodNvs->AodRmpPackData.MemoryClock[Index] = Rmp->Profiles[Index].MemClk;
    AodNvs->AodRmpPackData.CASLatency[Index] = Rmp->Profiles[Index].Tcl;
  }
  CopyMem (AodNvs->AodRmpPackData.RMPSpdData, Rmp->RmpSpdData, 128);
  return EFI_SUCCESS;
}
