/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _WIFI_ENROLL_FILE_H_
#define _WIFI_ENROLL_FILE_H_

#include <Protocol/LoadFile.h>
#include <Protocol/BlockIo.h>
#include <Guid/FileSystemVolumeLabelInfo.h>
#include <Guid/FileInfo.h>

#define WIFI_FE_MENU_ENTRY_SIGNATURE   SIGNATURE_32 ('W', 'F', 'M', 'E')
#define WIFI_FE_FILE_CONTEXT_SELECT    0x1

#define EM_FILE_SYSTEM                 0x0001
#define EM_DIRECTORY                   0x0002
#define EM_UNKNOWN_CONTEXT             0x0003

#define MAX_CHAR                       480

typedef struct {
  EFI_HII_HANDLE                       HiiHandle;
  UINTN                                EmDisplayContext;
  EFI_QUESTION_ID                      EnrollFormId;
} WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA;

typedef struct {
  UINTN                                Signature;
  LIST_ENTRY                           Head;
  UINTN                                MenuNumber;
} ENROLL_MENU_OPTION;

typedef struct {
  UINTN                                Signature;
  LIST_ENTRY                           Link;
  UINTN                                OptionNumber;
  UINT16                               *DisplayString;
  UINT16                               *HelpString;
  EFI_STRING_ID                        DisplayStringToken;
  EFI_STRING_ID                        HelpStringToken;
  UINTN                                ContextSelection;
  VOID                                 *VariableContext;
} ENROLL_MENU_ENTRY;

typedef struct {
  EFI_HANDLE                           Handle;
  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;
  EFI_FILE_HANDLE                      FHandle;
  UINT16                               *FileName;
  EFI_FILE_SYSTEM_VOLUME_LABEL         *Info;
  BOOLEAN                              IsRoot;
  BOOLEAN                              IsDir;
  BOOLEAN                              IsRemovableMedia;
  BOOLEAN                              IsLoadFile;
  BOOLEAN                              IsBootLegacy;
} EM_FILE_CONTEXT;

typedef struct {
  EFI_HANDLE                           Handle;
  EFI_DEVICE_PATH_PROTOCOL             *DevicePath;
  EFI_FILE_HANDLE                      FHandle;
  UINT16                               *FileName;
  EFI_FILE_SYSTEM_VOLUME_LABEL         *Info;

  BOOLEAN                              IsRoot;
  BOOLEAN                              IsDir;
  BOOLEAN                              IsRemovableMedia;
  BOOLEAN                              IsLoadFile;
  BOOLEAN                              IsBootLegacy;
} BM_FILE_CONTEXT;

typedef struct _STRING_LIST_NODE STRING_LIST_NODE;

struct _STRING_LIST_NODE {
  EFI_STRING_ID                        StringToken;
  STRING_LIST_NODE                     *Next;
};

typedef struct _STRING_DEPOSITORY {
  UINTN                                TotalNodeNumber;
  STRING_LIST_NODE                     *CurrentNode;
  STRING_LIST_NODE                     *ListHead;
} STRING_DEPOSITORY;

EFI_STATUS
InitializeEnrollMenu (
  IN EFI_FORM_ID                       EnrollFormId
  );

BOOLEAN
UpdateEmFileExplorer (
  IN UINT16                            KeyValue,
  OUT EFI_DEVICE_PATH_PROTOCOL         **FilePath
  );

EFI_STATUS
WifiMgrFileExplorerInit (
  IN WIFI_MGR_PRIVATE_DATA             *Private
  );

extern WIFI_MGR_FILE_EXPLORER_PRIVATE_DATA  mWifiMgrFileExplorerData;

#endif
