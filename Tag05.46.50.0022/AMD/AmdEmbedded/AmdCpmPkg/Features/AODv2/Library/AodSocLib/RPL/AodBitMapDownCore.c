

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
#include "AodBitMapDownCore.h"

AOD_MOUDLE_NAME ("AodBitMapDownCore")
AOD_SOC_NAME ("RPL")
#define MAX_DISPLAY_STRING_LEN             0x80
#define MAX_UNI_STRING_LEN                 0x800
#define AOD_POPUP_MAX_SIZE                 0x20
#define KEY_AOD_DOWN_CORE_DYNAMIC          0x1000  //AodDownCoreDynamic
#define KEY_AOD_DOWN_CORE_SAVE_DYNAMIC     0x1010  //AodDownCoreDynamic
#define KEY_AOD_DOWN_CORE_DISCARD_DYNAMIC  0x1011  //AodDownCoreDynamic
GLOBAL_REMOVE_IF_UNREFERENCED UINTN        gRow,gColumn;
extern EFI_GUID mAodFormSetGuid;
AMD_APCB_SERVICE_PROTOCOL        *mApcbDxeServiceProtocol = NULL;

/**
  This function used to set TPL

  @param[in]  Tpl                 Input raised TPL level

  @retval return current TPL

**/
EFI_TPL
AodSetTplRpl (
  EFI_TPL Tpl
  )
{
  EFI_TPL CurrentTpl;

  CurrentTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  gBS->RestoreTPL (Tpl);

  return CurrentTpl;
}

/**
  This function used to read the key Stroke

  @param[out]  Key                 output key

  @retval EFI_SUCCESS    if read key successfully
          EFI_TIME_OUT   time out wait for key
          others by callee
**/
EFI_STATUS
EFIAPI
AodReadKeyStrokesRpl (
  OUT  EFI_INPUT_KEY            *Key
  )
{
  EFI_STATUS                    Status;
  EFI_EVENT                     TimerEvent;
  EFI_EVENT                     WaitList[2];
  UINTN                         Index;
  EFI_TPL                       CurrentTpl;

  CurrentTpl = AodSetTplRpl (TPL_APPLICATION);
  do {
    do {
      Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimerEvent);
      //
      // Set a timer event of 1 second expiration
      //
      gBS->SetTimer (
             TimerEvent,
             TimerRelative,
             (1000*1000)
             );

      //
      // Wait for the keystroke event or the timer
      //
      WaitList[0] = gST->ConIn->WaitForKey;
      WaitList[1] = TimerEvent;
      Status      = gBS->WaitForEvent (2, WaitList, &Index);

      //
      // Check for the timer expiration
      //
      if (!EFI_ERROR (Status) && Index == 1) {
        Status = EFI_TIMEOUT;
      }

      gBS->CloseEvent (TimerEvent);
    } while (Status == EFI_TIMEOUT);

    Status = gST->ConIn->ReadKeyStroke (gST->ConIn, Key);

  } while (EFI_ERROR (Status));

  AodSetTplRpl (CurrentTpl);     // Restore to original TPL.

  return Status;
}

/**
  This function used to read updated core information timely

  @param[in/out]  StringPtr                 Input core inforamtion string like
                                            *-*-*-*-*-*-*-* ==> 1-0-1-0-1-0-1-0
  @param[in]      StringSize                string size of input string

  @retval EFI_SUCCESS       if enter prased and core info string updated
          EFI_DEVICE_ERROR   Esc pressed
          others by callee
**/
EFI_STATUS
EFIAPI
AodReadCoreInfoRpl (
  IN OUT  CHAR16      *StringPtr,
  IN      UINTN       StringSize
  )
{
  UINTN                            Index;
  CHAR16                           *TempString;
  UINTN                            Maximum;
  EFI_INPUT_KEY                    Key;
  EFI_STATUS                       Status;
  UINT32                           CoreIndex = 0;
  UINT32                           CoreTemp = 0;
  Maximum = StringSize;

  TempString = AllocateCopyPool(StrnSizeS (StringPtr, 0x1000), StringPtr);
  if (TempString == NULL) return EFI_OUT_OF_RESOURCES;
  do {
    Status = AodReadKeyStrokesRpl (&Key);
    ASSERT_EFI_ERROR (Status);
    switch (Key.UnicodeChar) {
      case CHAR_NULL:
        switch (Key.ScanCode) {
          case SCAN_LEFT:
            break;
          case SCAN_RIGHT:
            break;
          case SCAN_ESC:
            FreePool (TempString);
            return EFI_DEVICE_ERROR;
          default:
            break;
        }
        break;
      case L'1':
        Index = 0;
        if (CoreIndex > Maximum) break;
        while (TempString[Index] != 0) {
          if (TempString[Index] == L'*') {
            TempString[Index] = L'1';
            CoreIndex ++;
            break;
          }
          Index ++;
        }
        gST->ConOut->SetCursorPosition (gST->ConOut, gColumn, gRow);
        gST->ConOut->OutputString (gST->ConOut, TempString);
        break;
      case L'0':
        Index = 0;
        if (CoreIndex > Maximum) break;
        while (TempString[Index] != 0) {
          if (TempString[Index] == L'*') {
            TempString[Index] = L'0';
            CoreIndex ++;
            break;
          }
          Index ++;
        }
        gST->ConOut->SetCursorPosition (gST->ConOut, gColumn, gRow);
        gST->ConOut->OutputString (gST->ConOut, TempString);
        break;
      case CHAR_CARRIAGE_RETURN: //enter
        if (CoreIndex == Maximum) {
          CopyMem (StringPtr, TempString, StrnSizeS (StringPtr, 0x1000));
          FreePool (TempString);
          return EFI_SUCCESS;
        }
        break;

      case CHAR_BACKSPACE:
        if (CoreIndex == 0) break;
        CoreTemp = 0;
        Index = 0;
        while (TempString[Index] != 0) {
          if ((TempString[Index] == L'1') || (TempString[Index] == L'0')){
            CoreTemp ++;
          }
          if (CoreIndex == CoreTemp) {
            TempString[Index] = L'*';
            CoreIndex --;
            break;
          }
          Index ++;
        }
        gST->ConOut->SetCursorPosition (gST->ConOut, gColumn, gRow);
        gST->ConOut->OutputString (gST->ConOut, TempString);
        break;

      default:
        break;
    }
  } while(TRUE);

  return Status;
}

/**
  This function used to draw down core control popup

  @param[in]  Header                - Bit map core info header string
  @param[in]  CurrentInfo           - Current Bit map core info header string
  @param[in]  InputInfo             - Bit map core info header string
  @param[in]  CoreMax               - Max un-fused core number

  @retval EFI_SUCCESS means draw popup successfully
          others by callee
**/
EFI_STATUS
EFIAPI
AodDrawPopUpRpl (
  IN  CHAR16        *Header,
  IN  CHAR16        *CurrentInfo,
  IN  CHAR16        *InputInfo,
  IN  UINT8         CoreMax
  )
{
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
  EFI_SIMPLE_TEXT_OUTPUT_MODE      SavedConsoleMode;
  UINTN                            Columns;
  UINTN                            Rows;
  UINTN                            Column;
  UINTN                            Row;
  UINTN                            NumberOfLines;
  UINTN                            MaxLength;
  UINTN                            Length;
  CHAR16                           *Line;
  EFI_STATUS                       Status;
  CHAR16                           *StringBuffer;
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  MaxLength     = AOD_POPUP_MAX_SIZE + 4;
  NumberOfLines = 3;


  //
  // Cache a pointer to the Simple Text Output Protocol in the EFI System Table
  //
  ConOut = gST->ConOut;

  //
  // Save the current console cursor position and attributes
  //
  CopyMem (&SavedConsoleMode, ConOut->Mode, sizeof (SavedConsoleMode));

  //
  // Retrieve the number of columns and rows in the current console mode
  //
  ConOut->QueryMode (ConOut, SavedConsoleMode.Mode, &Columns, &Rows);

  //
  // Disable cursor and set the foreground and background colors specified by Attribute
  //
  ConOut->EnableCursor (ConOut, FALSE);
  ConOut->SetAttribute (ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE);

  //
  // Compute the starting row and starting column for the popup
  //
  Row    = (Rows - (NumberOfLines + 3)) / 2;
  Column = (Columns - (MaxLength + 2)) / 2;

  //
  // Allocate a buffer for a single line of the popup with borders and a Null-terminator
  //
  Line = AllocateZeroPool ((MaxLength + 3) * sizeof (CHAR16));
  ASSERT (Line != NULL);
  if (Line == NULL) return EFI_OUT_OF_RESOURCES;

  //
  // Draw top of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_DOWN_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_DOWN_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Draw middle of the popup with strings
  // Header
  //
  StringBuffer = AllocateZeroPool (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16));
  if (StringBuffer == NULL) return EFI_OUT_OF_RESOURCES;
  StrCpyS (StringBuffer, (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16)) / sizeof (CHAR16), Header);
  Length = StrLen (StringBuffer);
  SetMem16 (Line, (MaxLength + 2) * 2, L' ' );
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  CopyMem (Line + 1 + (MaxLength - Length) / 2, StringBuffer , Length * sizeof (CHAR16));
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Draw middle of the popup with strings
  // Current core info
  //
  ZeroMem (StringBuffer, MAX_DISPLAY_STRING_LEN * sizeof (CHAR16));
  StrCpyS (StringBuffer, (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16)) / sizeof (CHAR16), CurrentInfo);
  Length = StrLen (StringBuffer);
  SetMem16 (Line, (MaxLength + 2) * 2, L' ' );
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  CopyMem (Line + 1 + (MaxLength - Length) / 2, StringBuffer , Length * sizeof (CHAR16));
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Draw bottom of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_UP_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_UP_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Draw top of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_DOWN_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_DOWN_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  gRow    = Row;
  gColumn = Column + 1 + (MaxLength - Length) / 2;
  ZeroMem (StringBuffer, MAX_DISPLAY_STRING_LEN * sizeof (CHAR16));
  StrCpyS (StringBuffer, (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16)) / sizeof (CHAR16), InputInfo);
  Length = StrLen (StringBuffer);
  SetMem16 (Line, (MaxLength + 2) * 2, L' ' );
  Line[0]             = BOXDRAW_VERTICAL;
  Line[MaxLength + 1] = BOXDRAW_VERTICAL;
  Line[MaxLength + 2] = L'\0';
  CopyMem (Line + 1 + (MaxLength - Length) / 2, StringBuffer , Length * sizeof (CHAR16));
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);
  //
  // Draw bottom of popup box
  //
  SetMem16 (Line, (MaxLength + 2) * 2, BOXDRAW_HORIZONTAL);
  Line[0]             = BOXDRAW_UP_RIGHT;
  Line[MaxLength + 1] = BOXDRAW_UP_LEFT;
  Line[MaxLength + 2] = L'\0';
  ConOut->SetCursorPosition (ConOut, Column, Row++);
  ConOut->OutputString (ConOut, Line);

  //
  // Free the allocated line buffer
  //
  FreePool (Line);
  FreePool (StringBuffer);
  //
  // Read updated core information
  //
  Status = AodReadCoreInfoRpl (InputInfo, CoreMax);
  //
  // Restore the cursor visibility, position, and attributes
  //
  ConOut->EnableCursor      (ConOut, SavedConsoleMode.CursorVisible);
  ConOut->SetCursorPosition (ConOut, SavedConsoleMode.CursorColumn, SavedConsoleMode.CursorRow);
  ConOut->SetAttribute      (ConOut, SavedConsoleMode.Attribute);
  return Status;
}
/**
  This function used to draw the manage bit map core information

  @param[in]  HiiHandle                - the hii handle
  @param[in]  pInfo                    - the soc core info

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodUpdateCoreManageTextRpl (
  IN  EFI_HII_HANDLE            HiiHandle,
  IN  AOD_CORE_INFO             *pInfo
  )
{
  EFI_STATUS              Status;
  UINTN                   Index, CoreIndex, LogicalCcdIndex;
  CHAR16                  *pCoreInfoString;
  AOD_CORE_INFO           *pCurrentInfo;
  CHAR16                  pTemp[0x40];
  const CHAR16            *CoreBitHeader = L"AOD Core Bit Map information\n";
  UINTN                   VariableSize;

  pCurrentInfo = AllocateZeroPool(sizeof (AOD_CORE_INFO));
  VariableSize = sizeof (AOD_CORE_INFO);
  Status = gRT->GetVariable (
                  L"AodCoreInfo",
                  &gAodSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) pCurrentInfo
                  );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "Status = %r\n", Status));
    return Status;
  }
  pCoreInfoString = AllocateZeroPool (MAX_UNI_STRING_LEN);
  if (pCoreInfoString == NULL) return EFI_OUT_OF_RESOURCES;
  Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, CoreBitHeader);
  if (EFI_ERROR (Status)) return Status;
  for (Index = 0,LogicalCcdIndex = 0; Index < pInfo->AodPhysicalCcdCount; Index ++) {
    if (pInfo->AodCcdBitMap & (UINT32)(1 << Index)) {       // These ccd is not fused down
      // These ccd is not fused down
      ZeroMem (pTemp, 0x80);
      UnicodeSPrint(pTemp, 0x80, L"CCD%02d :",LogicalCcdIndex);
      LogicalCcdIndex ++;
      Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, pTemp);
      if (EFI_ERROR (Status)) return Status;

      for (CoreIndex = 0; CoreIndex < pInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
        ZeroMem (pTemp, 0x80);
        if (pInfo->AodCoreStatus[Index][CoreIndex] == AOD_CORE_FUSED_OFF) continue;
        switch (pInfo->AodCoreStatus[Index][CoreIndex]) {
          case AOD_CORE_ACTIVE:
            Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, \
            (pInfo->AodCoreStatus[Index][CoreIndex] == pCurrentInfo->AodCoreStatus[Index][CoreIndex])?L" 1 ":L"*1 ");
            if (EFI_ERROR (Status)) return Status;
            if (pInfo->AodLogicalCoreCountPerCcd > (CoreIndex + 1)) {
              Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, L"-");
              if (EFI_ERROR (Status)) return Status;
            }
            break;
          case AOD_CORE_SOFT_OFF:
            Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, \
            (pInfo->AodCoreStatus[Index][CoreIndex] == pCurrentInfo->AodCoreStatus[Index][CoreIndex])?L" 0 ":L"*0 ");
            if (EFI_ERROR (Status)) return Status;
            if (pInfo->AodLogicalCoreCountPerCcd > (CoreIndex + 1)) {
              Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, L"-");
              if (EFI_ERROR (Status)) return Status;
            }
            break;
          default:
            break;
        }
      }
      Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, L"\n");
      if (EFI_ERROR (Status)) return Status;
    } else {
      ZeroMem (pTemp, 0x80);
      UnicodeSPrint(pTemp, 0x80, L"CCD%02d : Not Present\n",Index);
      Status = StrCatS (pCoreInfoString, MAX_UNI_STRING_LEN, pTemp);
      if (EFI_ERROR (Status)) return Status;
    }
  }

  if (0 == HiiSetString (HiiHandle, STRING_TOKEN(STR_AOD_DOWNCORE_INFO_DYNAMIC), pCoreInfoString, NULL)) {
    return EFI_UNSUPPORTED;
  }
  //
  // Save core bit information here
  //
  Status = gRT->SetVariable (
                  L"AodCoreInfoTemp",
                  &gAodSystemConfigurationGuid,
                  EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof(AOD_CORE_INFO),
                  pInfo
                  );
  return Status;
}
/**
  This function used to sync the L"AodCoreInfo" variable into the core information
  in L"AodSetup".

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodSyncCoreInfoIntoVariableRpl (
  IN AOD_CORE_INFO                    *pInfo
  )
{
  EFI_STATUS          Status;
  AOD_CONFIG          *AodConfig;
  UINT8               CoreIndex,CcdIndex,LogicalCoreIndex;
  if (pInfo == NULL) return EFI_OUT_OF_RESOURCES;
  Status = AodGetConfiguration (&AodConfig);
  if (EFI_ERROR (Status)) return Status;
  //
  // Sync max ccd count/core count per ccd into L"AodSetup"
  //
  AodConfig->AodMaxCcdCount = pInfo->AodPhysicalCcdCount;
  AodConfig->AodMaxCorePerCcd = pInfo->AodPhysicalCoreCountPerCcd;
  AodConfig->AodCcdBitMap = (UINT8)pInfo->AodCcdBitMap;
  for (CcdIndex = 0; CcdIndex < AodConfig->AodMaxCcdCount; CcdIndex ++) {
    if ((AodConfig->AodCcdBitMap & (1 << CcdIndex)) == 0) continue;
    AodConfig->AodCoreBitMap[CcdIndex] = 0;
    LogicalCoreIndex = 0;
    for (CoreIndex = 0; CoreIndex < AodConfig->AodMaxCorePerCcd; CoreIndex ++) {
      if (pInfo->AodCoreStatus[CcdIndex][CoreIndex] == AOD_CORE_FUSED_OFF) continue;
      if (pInfo->AodCoreStatus[CcdIndex][CoreIndex] == AOD_CORE_SOFT_OFF) {
        AodConfig->AodCoreBitMap[CcdIndex] |= (1 << LogicalCoreIndex);
      }
      LogicalCoreIndex ++;
    }
  }
  Status = AodSetConfiguration (AodConfig, AodConfig->Header.AodVariableSize);
  return Status;
}
/**
  This function used to generate bit map down core options hii data

  @param[in]  HiiHandle                - the hii handle

  @retval EFI_SUCCESS
          others by callee
**/
EFI_STATUS
EFIAPI
AodGenerateCoreManageOptionsRpl (
  IN EFI_HII_HANDLE    HiiHandle
  )
{
  EFI_STATUS                       Status;
  VOID                             *StartOpCodeHandle;
  VOID                             *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL               *StartLabel;
  EFI_IFR_GUID_LABEL               *EndLabel;
  AOD_CORE_INFO                    Info;
  UINTN                            Index, LogicalIndex;
  CHAR16                           pTemp[0x40];
  //
  // Get Aod Core Info Hob
  //
  Status = AodGetCoreInfo (&Info);
  if (EFI_ERROR (Status)) return Status;
  //
  // Set Aod Core Info Hob into L"AodCoreInfo" Variable
  //
  Status = gRT->SetVariable (
                L"AodCoreInfo",
                &gAodSystemConfigurationGuid,
                EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                sizeof(AOD_CORE_INFO),
                &Info
                );
  if (EFI_ERROR (Status)) return Status;
  //
  // Sync Core information in L"AodCoreInfo" into L"AodSetup"
  //
  Status = AodSyncCoreInfoIntoVariableRpl (&Info);
  if (EFI_ERROR (Status)) return Status;

  //
  // Build Hii Front/String database for Bit Map Down Core
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle = HiiAllocateOpCodeHandle ();

  if(StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Create Hii Extended Label OpCode as the start and end opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
                 StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (
               EndOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));

  if(StartLabel == NULL || EndLabel == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;

  StartLabel->Number = LABEL_CBS_AOD_DOWN_CORE_DYNAMIC_START;
  EndLabel->Number = LABEL_CBS_AOD_DOWN_CORE_DYNAMIC_END;
  //
  // Create Subtile for bit map information
  //
  HiiCreateSubTitleOpCode (
    StartOpCodeHandle,
    STRING_TOKEN(STR_AOD_DOWNCORE_INFO_DYNAMIC),
    STRING_TOKEN(STR_AOD_BLANK),
    0,0
    );
  HiiCreateSubTitleOpCode (
    StartOpCodeHandle,
    STRING_TOKEN(STR_AOD_BLANK),
    STRING_TOKEN(STR_AOD_BLANK),
    0,0
    );
  //
  // Create goto Form for every CCD with callback flag and question id
  // start with KEY_AOD_DOWN_CORE_DYNAMIC and increase by Physical CCD index
  //
  for (Index = 0, LogicalIndex = 0; Index < Info.AodPhysicalCcdCount; Index ++) {
    if (Info.AodCcdBitMap & (UINT32)(1 << Index)) {       // These ccd is not fused down
      ZeroMem (pTemp, 0x80);
      UnicodeSPrint (pTemp, 0x80, L"CCD %02d Bit Map Down Core Control\n", LogicalIndex);
      LogicalIndex++;
      HiiCreateGotoOpCode (
        StartOpCodeHandle,
        SETUP_CPU_CORE_COUNT_CONTROL_LABLE,
        HiiSetString(HiiHandle, 0, pTemp, NULL),
        HiiSetString(HiiHandle, 0, L"1 : means core is enabled \n0 : means core is software down \n", NULL),
        EFI_IFR_FLAG_CALLBACK,
        (EFI_QUESTION_ID)(KEY_AOD_DOWN_CORE_DYNAMIC + Index)
        );
    }
  }
  //
  // Create a blank
  //
  HiiCreateSubTitleOpCode (
    StartOpCodeHandle,
    STRING_TOKEN(STR_AOD_BLANK),
    STRING_TOKEN(STR_AOD_BLANK),
    0,0
    );

  //
  // Create goto Form for Discard changes in L"AodCoreInfoTemp"
  //
  ZeroMem (pTemp, 0x80);
  UnicodeSPrint (pTemp, 0x80, L"Bit Map Down Core Discard Changes\n");
  HiiCreateGotoOpCode (
      StartOpCodeHandle,
      SETUP_CPU_CORE_COUNT_CONTROL_LABLE,
      HiiSetString(HiiHandle, 0, pTemp, NULL),
      HiiSetString(HiiHandle, 0, L"Discard Changes", NULL),
      EFI_IFR_FLAG_CALLBACK,
      KEY_AOD_DOWN_CORE_DISCARD_DYNAMIC
      );
  //
  // Create goto Form for Save changes in L"AodCoreInfoTemp" into L"AodCoreInfo"
  //
  ZeroMem (pTemp, 0x80);
  UnicodeSPrint (pTemp, 0x80, L"Bit Map Down Core Apply Changes\n",Index);
  HiiCreateGotoOpCode (
      StartOpCodeHandle,
      SETUP_CPU_CORE_COUNT_CONTROL_LABLE,
      HiiSetString(HiiHandle, 0, pTemp, NULL),
      HiiSetString(HiiHandle, 0, L"Check and Apply Changes, need make sure core number equaled in each CCD", NULL),
      EFI_IFR_FLAG_CALLBACK,
      KEY_AOD_DOWN_CORE_SAVE_DYNAMIC
      );
  HiiCreateSubTitleOpCode (
    StartOpCodeHandle,
    STRING_TOKEN(STR_AOD_BLANK),
    STRING_TOKEN(STR_AOD_BLANK),
    0,0
    );
  Status = HiiUpdateForm (
             HiiHandle,
             &mAodFormSetGuid,
             SETUP_CPU_CORE_COUNT_CONTROL_LABLE,
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
  //
  // Update Default L"AodCoreInfoTemp" and STR_AOD_DOWNCORE_INFO_DYNAMIC string
  //
  AodUpdateCoreManageTextRpl (HiiHandle, &Info);
  return Status;
}
/**
  This function used to generate bit map down core load default callback
  And will write bit map control info into APCB

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
AodDownCoreLoadDefaultCallbackRpl (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  AOD_CORE_INFO                     *pInfo;
  UINTN                             VariableSize;
  EFI_STATUS                        Status;
  UINT8                             Index;
  UINT8                             CoreIndex;
  if (Action != EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
    return EFI_UNSUPPORTED;
  }
  VariableSize = sizeof (AOD_CORE_INFO);
  pInfo = AllocateZeroPool(VariableSize);
  Status = gRT->GetVariable (
                  L"AodCoreInfo",
                  &gAodSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) pInfo
                  );
  for (Index = 0; Index < pInfo->AodPhysicalCcdCount; Index ++) {
    if ((pInfo->AodCcdBitMap & (UINT32)(1 << Index)) == 0) {// These ccd is fused down
      continue;
    }
    for (CoreIndex = 0; CoreIndex < pInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
      if (pInfo->AodCoreStatus[Index][CoreIndex] == AOD_CORE_SOFT_OFF) { //if is soft off, set core active
        pInfo->AodCoreStatus[Index][CoreIndex] = AOD_CORE_ACTIVE;
      }
    }
  }
  Status = gRT->SetVariable (
                  L"AodCoreInfo",
                  &gAodSystemConfigurationGuid,
                  EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof(AOD_CORE_INFO),
                  pInfo
                  );
  AodUpdateCoreManageTextRpl (Handle, pInfo);
  Status = AodSyncCoreInfoIntoVariableRpl (pInfo);
  if (EFI_ERROR (Status)) return Status;

  return EFI_SUCCESS;
}
/**
  This function used to generate bit map down core save goto callback
  And will write bit map control info into APCB

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
AodDownCoreSaveCallbackRpl (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  AOD_CORE_INFO                     *pInfo;
  UINTN                             VariableSize;
  UINTN                             Index;
  EFI_STATUS                        Status;
  UINTN                             CoreIndex;
  BOOLEAN                           IsMatched = TRUE;
  EFI_INPUT_KEY                     Key;
  UINT8                             CoreNumberA = 0;
  UINTN                             CcdIndex = 0;
  UINT8                             CoreNumberB = 0;
  AOD_CONFIG                        *AodConfig;
  CHAR16                            pTemp[0x100];
  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    return EFI_UNSUPPORTED;
  }

  if (KeyValue != KEY_AOD_DOWN_CORE_SAVE_DYNAMIC) {
    return EFI_UNSUPPORTED;
  }
  VariableSize = sizeof (AOD_CORE_INFO);
  pInfo = AllocateZeroPool(VariableSize);
  Status = gRT->GetVariable (
                  L"AodCoreInfoTemp",
                  &gAodSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) pInfo
                  );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "Status = %r\n", Status));
    return Status;
  }
  for (Index = 0; Index < pInfo->AodPhysicalCcdCount; Index ++) {
    if ((pInfo->AodCcdBitMap & (UINT32)(1 << Index)) == 0) {// These ccd is fused down
      continue;
    }
    for (CoreIndex = 0; CoreIndex < pInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
      if (pInfo->AodCoreStatus[Index][CoreIndex] == AOD_CORE_ACTIVE) {
        CoreNumberA ++;
        CcdIndex = Index;
      }
    }
    if (CoreNumberA != 0) break;
  }
  Index ++;
  for (; Index < pInfo->AodPhysicalCcdCount; Index ++) {
    CoreNumberB = 0;
    if ((pInfo->AodCcdBitMap & (UINT32)(1 << Index)) == 0) {// These ccd is fused down
      continue;
    }

    for (CoreIndex = 0; CoreIndex < pInfo->AodPhysicalCoreCountPerCcd; CoreIndex ++) {
      if (pInfo->AodCoreStatus[Index][CoreIndex] == AOD_CORE_ACTIVE) {
        CoreNumberB ++;
      }
    }
    if (CoreNumberB == 0) continue;
    if (CoreNumberB != CoreNumberA) {
      ZeroMem (pTemp, 0x100);
      UnicodeSPrint (pTemp, 0x100, L"%d Cores in CCD%d don't match %d Cores in CCD%d",
        CoreNumberA,
        CcdIndex,
        CoreNumberB,
        Index
        );
      IsMatched = FALSE;
      CreatePopUp (
        EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
        &Key,
        L"!!!ERROR!!!",
        pTemp,
        L"Press any key to continue and then Discard the settings",
        NULL
        );
      break;
    }
  }
  if (IsMatched) {
    CreatePopUp (
      EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
      &Key,
      L"Save and Apply the settings",
      L"Press any key to continue and Reboot",
      NULL
      );
    Status = gRT->SetVariable (
                    L"AodCoreInfo",
                    &gAodSystemConfigurationGuid,
                    EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(AOD_CORE_INFO),
                    pInfo
                    );
    AodUpdateCoreManageTextRpl (Handle, pInfo);
    Status = AodSyncCoreInfoIntoVariableRpl (pInfo);
    if (EFI_ERROR (Status)) return Status;
    Status = AodGetConfiguration (&AodConfig);
    if (EFI_ERROR (Status)) return Status;
    Status = AodSendCommunicateBuffer (AOD_SMM_COMM_VARIABLE_HOOK, AodConfig, AodConfig->Header.AodVariableSize);
    if (EFI_ERROR (Status)) {
      AOD_DEBUG_SOC ((EFI_D_ERROR, "Sync APCB failed status = %r\n", Status));
    }
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "End\n"));
  return EFI_SUCCESS;
}

/**
  This function used to generate bit map down core control in each CCD goto callback
  And the bit map information in each CCD will update the core manage text

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
AodDownCoreBitControlCallbackRpl (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  UINTN                         CcdIndex = 0;
  EFI_STATUS                    Status;
  UINTN                         Index = 0;
  UINTN                         VariableSize = 0;
  CHAR16                        *StringBuffer1;
  CHAR16                        *StringBuffer2;
  AOD_CORE_INFO                 *pCurrentInfo;
  CHAR16                        *pCoreString;
  UINT8                         CoreMax = 0;

  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    return EFI_UNSUPPORTED;
  }

  if ((KeyValue < 0x1000) || (KeyValue > 0x1007)) {
    return EFI_UNSUPPORTED;
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "Key = %X, Action = %x\n", KeyValue, Action));
  CcdIndex = KeyValue - KEY_AOD_DOWN_CORE_DYNAMIC;

  pCoreString = AllocateZeroPool (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16));
  ASSERT (pCoreString != NULL);
  pCurrentInfo = AllocateZeroPool (sizeof (AOD_CORE_INFO));
  ASSERT (pCurrentInfo != NULL);
  StringBuffer1 = AllocateZeroPool (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16));
  ASSERT (StringBuffer1 != NULL);
  StringBuffer2 = AllocateZeroPool (MAX_DISPLAY_STRING_LEN * sizeof (CHAR16));
  ASSERT (StringBuffer2 != NULL);
  UnicodeSPrint (StringBuffer1, 0x80, L"CCD %02d Down Core Bit Control",CcdIndex);
  VariableSize = sizeof (AOD_CORE_INFO);
  Status = gRT->GetVariable (
                L"AodCoreInfoTemp",
                &gAodSystemConfigurationGuid,
                NULL,
                &VariableSize,
                (VOID *) pCurrentInfo
                );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "Status = %r\n", Status));
    goto Exit;
  }
  Status = StrCatS (StringBuffer2, MAX_DISPLAY_STRING_LEN, L"Current :");
  if (EFI_ERROR (Status)) goto Exit;
  Status = StrCatS (pCoreString, MAX_DISPLAY_STRING_LEN, L"Modify  :");
  if (EFI_ERROR (Status)) goto Exit;
  for (Index = 0; Index < pCurrentInfo->AodPhysicalCoreCountPerCcd; Index ++) {
    if (pCurrentInfo->AodCoreStatus[CcdIndex][Index] == AOD_CORE_FUSED_OFF) continue;
    switch (pCurrentInfo->AodCoreStatus[CcdIndex][Index]) {
      case AOD_CORE_ACTIVE:
        Status = StrCatS (StringBuffer2, MAX_DISPLAY_STRING_LEN, L"1");
        if (EFI_ERROR (Status)) goto Exit;
        Status = StrCatS (pCoreString, MAX_DISPLAY_STRING_LEN, L"*");
        if (EFI_ERROR (Status)) goto Exit;
        CoreMax ++;
        if (Index < (pCurrentInfo->AodLogicalCoreCountPerCcd-1)) {
          Status = StrCatS (StringBuffer2, MAX_DISPLAY_STRING_LEN, L"-");
          if (EFI_ERROR (Status)) goto Exit;
          Status = StrCatS (pCoreString, MAX_DISPLAY_STRING_LEN, L"-");
          if (EFI_ERROR (Status)) goto Exit;
        }
        break;
      case AOD_CORE_SOFT_OFF:
        Status = StrCatS (StringBuffer2, MAX_DISPLAY_STRING_LEN, L"0");
        if (EFI_ERROR (Status)) goto Exit;
        Status = StrCatS (pCoreString, MAX_DISPLAY_STRING_LEN, L"*");
        if (EFI_ERROR (Status)) goto Exit;
        CoreMax ++;
        if (Index < (pCurrentInfo->AodLogicalCoreCountPerCcd-1)) {
          Status = StrCatS (StringBuffer2, MAX_DISPLAY_STRING_LEN, L"-");
          if (EFI_ERROR (Status)) goto Exit;
          Status = StrCatS (pCoreString, MAX_DISPLAY_STRING_LEN, L"-");
          if (EFI_ERROR (Status)) goto Exit;
        }
        break;
      default:
        break;
    }
  }

  Status = AodDrawPopUpRpl (StringBuffer1, StringBuffer2, pCoreString, CoreMax);
  if (!EFI_ERROR (Status)) {
    Index = 0;
    CoreMax = 0;
    while (pCoreString[Index] != 0) {
      while (pCurrentInfo->AodCoreStatus[CcdIndex][CoreMax] == AOD_CORE_FUSED_OFF) {
        CoreMax ++;
      }
      switch (pCoreString[Index]) {
        case L'0':
          pCurrentInfo->AodCoreStatus[CcdIndex][CoreMax] = AOD_CORE_SOFT_OFF;
          CoreMax ++;
          break;
        case L'1':
          pCurrentInfo->AodCoreStatus[CcdIndex][CoreMax] = AOD_CORE_ACTIVE;
          CoreMax ++;
          break;
        default:
          break;
      }
      Index ++;
    }
    AodUpdateCoreManageTextRpl (Handle, pCurrentInfo);
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "End\n"));
Exit:
  if (pCoreString != NULL) FreePool (pCoreString);
  if (pCurrentInfo != NULL) FreePool (pCurrentInfo);
  if (StringBuffer1 != NULL) FreePool (StringBuffer1);
  if (StringBuffer2 != NULL) FreePool (StringBuffer2);
  return Status;
}
/**
  This function used to generate bit map down core discard goto callback
  And this will discard all down core control before save goto callback.

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
AodDownCoreDiscardCallbackRpl (
  IN EFI_HII_HANDLE                       Handle,
  IN UINTN                                Action,
  IN UINT16                               KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  IN VOID                                 *IfrData,
  OUT UINTN                               *Req
  )
{
  AOD_CORE_INFO           *pCurrentInfo;
  EFI_STATUS              Status;
  UINTN                   VariableSize;

  AOD_DEBUG_SOC ((EFI_D_INFO, "Start\n"));
  if (Action != EFI_BROWSER_ACTION_CHANGING) {
    return EFI_UNSUPPORTED;
  }
  if (KeyValue != KEY_AOD_DOWN_CORE_DISCARD_DYNAMIC) {
    return EFI_UNSUPPORTED;
  }
  pCurrentInfo = AllocateZeroPool(sizeof (AOD_CORE_INFO));
  VariableSize = sizeof (AOD_CORE_INFO);
  Status = gRT->GetVariable (
                  L"AodCoreInfo",
                  &gAodSystemConfigurationGuid,
                  NULL,
                  &VariableSize,
                  (VOID *) pCurrentInfo
                  );
  if (EFI_ERROR (Status)) {
    AOD_DEBUG_SOC ((EFI_D_INFO, "Status = %r\n", Status));
    return Status;
  }
  AodUpdateCoreManageTextRpl (Handle, pCurrentInfo);

  if (pCurrentInfo != NULL) {
    FreePool (pCurrentInfo);
  }
  AOD_DEBUG_SOC ((EFI_D_INFO, "End\n"));
  return EFI_SUCCESS;
}




