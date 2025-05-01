/** @file
 VFR application

;******************************************************************************
;* Copyright (c) 2015 - 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************

*/

#ifndef _VFR_APP_NVDATASTRUC_H_
#define _VFR_APP_NVDATASTRUC_H_

#include <Guid/HiiPlatformSetupFormset.h>
#include <Guid/HiiFormMapMethodGuid.h>
#include <Guid/DriverSampleHii.h>
#include <Guid/ZeroGuid.h>

#define VFR_APP_CONFIGURATION_VARSTORE_ID    0x1000
#define VFR_APP_MAX_CON_OUT_DEV_NUM  10

#define VFR_APP_FORMSET_GUID \
  { \
    0xA04A27f4, 0xDF00, 0x1234, {0xB5, 0x52, 0x39, 0x51, 0x13, 0x02, 0x11, 0x3F} \
  }

#pragma pack(1)
typedef struct {
  UINT8   TestH2ODialog;
  UINT8   TestHiiPopupDialog;
  UINT8   ActiveDisplayEngine;
  UINT8   ConOutDESetting[VFR_APP_MAX_CON_OUT_DEV_NUM];

  UINT8   Numeric001;
  UINT64  Numeric002;
  UINT64  Numeric003;
  UINT8   Numeric004;
  UINT8   Numeric005;
  UINT16  Numeric006;
  UINT32  Numeric007;
  UINT64  Numeric008;

  UINT8   RefreshNumeric001;
  UINT8   RefreshNumeric002;

  UINT8   OneOf001;
  UINT8   OneOf002;
  UINT8   OneOf003;
  UINT8   OneOf004;

  UINT16  String001[10];
  UINT16  String002[200];
  UINT16  String003[10];

  UINT16  Password001[20];


  UINT8   OneOf1;
  UINT8   OneOf2;
  UINT8   OneOf3;
  UINT8   OneOf4;

  UINT8   Order1[37];
  UINT8   Order2[24];

  UINT8   Checkbox0001;

  EFI_HII_TIME  Time001;
  EFI_HII_DATE  Date001;

  // Setup Page panel related questions
  UINT8   SetupPagePanelOneOf0001;
  UINT8   SetupPagePanelOneOf0002;
  UINT8   SetupPagePanelOneOf0003;
  UINT8   SetupPagePanelOneOf0004;
  UINT8   SetupPagePanelOneOf0005;
  UINT8   SetupPagePanelOneOf0006;
  UINT8   SetupPagePanelOneOf0007;
  UINT8   SetupPagePanelOneOf0008;
  UINT8   SetupPagePanelOneOf0009;
  UINT8   SetupPagePanelOneOf0010;
  UINT8   SetupPagePanelOneOf0011;
  UINT8   SetupPagePanelOneOf0012;
  UINT8   SetupPagePanelOneOf0013;
  UINT8   SetupPagePanelOneOf0014;
  // Help Text panel related questions
  UINT8   HelpTextPanelOneOf0001;
  UINT8   HelpTextPanelOneOf0002;
  UINT8   HelpTextPanelOneOf0003;
  UINT8   HelpTextPanelOneOf0004;
  UINT8   HelpTextPanelOneOf0005;
  // Hot Key panel related questions
  UINT8   HotKeyPanelNumeric0001;
  UINT8   HotKeyPanelNumeric0002;
  UINT8   HotKeyPanelNumeric0003;
  UINT8   HotKeyPanelNumeric0004;
  UINT8   HotKeyPanelNumeric0005;
  UINT8   HotKeyPanelNumeric0006;
  UINT8   HotKeyPanelNumeric0007;
  UINT8   HotKeyPanelNumeric0008;
  UINT8   HotKeyPanelNumeric0009;
  UINT8   HotKeyPanelNumeric0010;
  UINT8   HotKeyPanelNumeric0011;
  UINT8   HotKeyPanelNumeric0012;
  UINT8   HotKeyPanelOneOf0001;
  UINT8   HotKeyPanelOneOf0002;
  UINT8   HotKeyPanelOneOf0003;
  UINT8   HotKeyPanelOneOf0004;
  UINT8   HotKeyPanelOneOf0005;
  UINT8   HotKeyPanelOneOf0006;
  UINT16  HotKeyPanelString0001[10];
  UINT16  HotKeyPanelPassword0001[20];
  UINT8   HotKeyPanelOrder1[10];
  UINT8   HotKeyPanelCheckbox0001;

  //
  // Legacy bios settings
  //
  UINT8   FloppyBootDevNum;
  UINT8   HddBootDevNum;
  UINT8   CdRomBootDevNum;
  UINT8   PcmciaBootDevNum;
  UINT8   UsbBootDevNum;
  UINT8   NetworkBootDevNum;
  UINT8   BevBootDevNum;

  //
  //
  UINT16  WhatIsThePassword[20];
  UINT16  WhatIsThePassword2[20];
  UINT16  MyStringData[40];
  UINT16  PasswordClearText[20];
  UINT16  SomethingHiddenForHtml;
  UINT8   HowOldAreYouInYearsManual;
  UINT16  HowTallAreYouManual;
  UINT8   HowOldAreYouInYears;
  UINT16  HowTallAreYou;
  UINT8   MyFavoriteNumber;
  UINT8   TestLateCheck;
  UINT8   TestLateCheck2;
  UINT8   QuestionAboutTreeHugging;
  UINT8   ChooseToActivateNuclearWeaponry;
  UINT8   SuppressGrayOutSomething;
  UINT8   OrderedList[8];
  UINT16  BootOrder[8];
  UINT8   BootOrderLarge;
  UINT8   DynamicRefresh;
  UINT8   DynamicOneof;
  UINT8   DynamicOrderedList[5];
  UINT8   Reserved;
  UINT8   NameValueVar0;
  UINT16  NameValueVar1;
  UINT16  NameValueVar2[20];
  UINT8   SerialPortNo;
  UINT8   SerialPortStatus;
  UINT16  SerialPortIo;
  UINT8   SerialPortIrq;
  UINT8   GetDefaultValueFromCallBack;
  UINT8   GetDefaultValueFromAccess;
  UINT8   RefreshGuidCount;
  //
  UINT8   OrderedListDefault[4];
} VFR_APP_CONFIGURATION;

//
// 2nd NV data structure definition
//
typedef struct {
  UINT8         Field8;
  UINT16        Field16;
  UINT8         OrderedList[3];
} VFR_APP_EFI_VARSTORE_DATA;

#define VFR_APP_FORM_ROOT                                                         0x0001
#define VFR_APP_FORM_FB_FUNC_VFR_REFRESH_INTERVAL                                 0x0002
#define VFR_APP_FORM_NUMERIC_OPCODE                                               0x1002
#define VFR_APP_FORM_ONE_OF_OPCODE                                                0x1003
#define VFR_APP_FORM_STRING_OPCODE                                                0x1004
#define VFR_APP_FORM_PASSWORD_OPCODE                                              0x1005
#define VFR_APP_FORM_ORDERED_LIST_OPCODE                                          0x1006
#define VFR_APP_FORM_TIME_OPCODE                                                  0x1007
#define VFR_APP_FORM_CHECKBOX_OPCODE                                              0x1008
#define VFR_APP_FORM_LEGACY_BIOS                                                  0x1009
#define VFR_APP_FORM_GOTO_OPCODE                                                  0x100A
#define VFR_APP_FORM_DATE_OPCODE                                                  0x100B
#define VFR_APP_FORM_ORDERED_LIST_DEFAULT_OPCODE                                  0x100C
#define VFR_APP_FORM_REFRESH_OPCODE                                               0x100D
#define VFR_APP_FORM_SUBTITLE_OPCODE                                              0x100E

#define VFR_APP_FORM_SETUP_PAGE_PANEL                                             0x2000
#define VFR_APP_FORM_SETUP_PAGE_PANEL_NO_ITEM_PAGE                                0x2001
#define VFR_APP_FORM_SETUP_PAGE_PANEL_NO_SELECTABLE_ITEM_IN_ONE_PAGE              0x2002
#define VFR_APP_FORM_SETUP_PAGE_PANEL_NO_SELECTABLE_ITEM_IN_THREE_PAGE            0x2003
#define VFR_APP_FORM_SETUP_PAGE_PANEL_ONE_SELECTABLE_ITEM_IN_ONE_PAGE             0x2004
#define VFR_APP_FORM_SETUP_PAGE_PANEL_ONE_SELECTABLE_ITEM_IN_ONE_THIRD_PAGE       0x2005
#define VFR_APP_FORM_SETUP_PAGE_PANEL_ONE_SELECTABLE_ITEM_IN_THREE_THIRD_PAGE     0x2006
#define VFR_APP_FORM_SETUP_PAGE_PANEL_ONE_SELECTABLE_ITEM_IN_THREE_THIRD_PAGE_2   0x200E
#define VFR_APP_FORM_SETUP_PAGE_PANEL_TWO_SELECTABLE_ITEM_IN_ONE_PAGE             0x2007
#define VFR_APP_FORM_SETUP_PAGE_PANEL_TWO_SELECTABLE_ITEM_IN_ONE_THIRD_PAGE       0x2008
#define VFR_APP_FORM_SETUP_PAGE_PANEL_TWO_SELECTABLE_ITEM_IN_ONE_TWO_THIRD_PAGE   0x2009
#define VFR_APP_FORM_SETUP_PAGE_PANEL_TWO_SELECTABLE_ITEM_IN_ONE_THREE_THIRD_PAGE 0x200A
#define VFR_APP_FORM_SETUP_PAGE_PANEL_NO_SELECTABLE_MULTI_LINE_ITEM               0x200B
#define VFR_APP_FORM_SETUP_PAGE_PANEL_THREE_SELECTABLE_MULTI_LINE_ITEM            0x200C
#define VFR_APP_FORM_SETUP_PAGE_PANEL_HIGHLIGHT_TEST                              0x200D
#define VFR_APP_FORM_HELP_TEXT_PANEL                                              0x3000
#define VFR_APP_FORM_HOT_KEY_PANEL                                                0x4000

//
// Labels definition
//
#define LABEL_UPDATE1               0x1234
#define LABEL_UPDATE2               0x2234
#define LABEL_UPDATE3               0x3234
#define LABEL_END                   0x2223

#define LABEL_UPDATE_CON_OUT_LIST_START  0x2224
#define LABEL_UPDATE_CON_OUT_LIST_END    0x2225
#define LABEL_UPDATE_SELECTABLE_QUESTION_LIST_START  0x2226
#define LABEL_UPDATE_SELECTABLE_QUESTION_LIST_END    0x2227

#define VFR_APP_KEY_HIDDEN_QUESTION      0x3000
#define VFR_APP_KEY_CON_OUT_DEV_BASE     0x3001
#define VFR_APP_KEY_SAME_QUESTION_ID     0x3002

#define VFR_APP_ACTIVE_DISPLAY_ENGINE_METRO   0
#define VFR_APP_ACTIVE_DISPLAY_ENGINE_TEXT    1
#define VFR_APP_ACTIVE_DISPLAY_ENGINE_ALL     2

//
// Question ID definition
//
#define VFR_APP_Q_ID_FB_FUNC_VFR_REFRESH_INTERVAL_ONE       0x8000
#define VFR_APP_Q_ID_FB_FUNC_VFR_REFRESH_INTERVAL_TWO       0x8001
#define VFR_APP_Q_ID_VFR_REFRESH_ID                         0x1247
#define VFR_APP_Q_ID_VFR_EXIT                               0x1237

#pragma pack()

#endif
