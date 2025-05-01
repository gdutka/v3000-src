/** @file
  Header file for target info

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

#ifndef _FB_TARGET_INFO_H_
#define _FB_TARGET_INFO_H_

#include "InternalH2OFormBrowser.h"

#define IS_LINK_TARGET(Type)   ((BOOLEAN) (((Type) == VFR_LINK_TARGET  ) || ((Type) == VFCF_LINK_TARGET  )))
#define IS_IMPORT_TARGET(Type) ((BOOLEAN) (((Type) == VFR_IMPORT_TARGET) || ((Type) == VFCF_IMPORT_TARGET)))

#define H2O_FORM_BROWSER_TARGET_INFO_SIGNATURE  SIGNATURE_32 ('H', 'F', 'T', 'I')
typedef struct _H2O_FORM_BROWSER_TARGET_INFO {
  UINTN                           Signature;
  LIST_ENTRY                      Link;

  EFI_GUID                        FormsetGuid;
  UINT16                          FormId;

  IMPORT_INFO_TYPE                TargetType;
  EFI_GUID                        TargetFormsetGuid;
  UINT16                          TargetFormId;
  UINT16                          TargetLabelId;
  UINT16                          TargetPriority;
  BOOLEAN                         Enable;
} H2O_FORM_BROWSER_TARGET_INFO;
#define H2O_FORM_BROWSER_TARGET_INFO_FROM_LINK(a)  CR (a, H2O_FORM_BROWSER_TARGET_INFO, Link, H2O_FORM_BROWSER_TARGET_INFO_SIGNATURE)

#define H2O_VFR_TARGET_INFO_SIGNATURE  SIGNATURE_32 ('H', 'V', 'T', 'I')
typedef struct _H2O_VFR_TARGET_INFO {
  UINTN                           Signature;
  LIST_ENTRY                      Link;

  IMPORT_INFO_TYPE                TargetType;
  EFI_GUID                        TargetFormsetGuid;
  UINT16                          TargetFormId;
  UINT16                          TargetLabelId;

  LIST_ENTRY                      PropertyListHead;
} H2O_VFR_TARGET_INFO;
#define H2O_VFR_TARGET_INFO_FROM_LINK(a)  CR (a, H2O_VFR_TARGET_INFO, Link, H2O_VFR_TARGET_INFO_SIGNATURE)

H2O_VFR_TARGET_INFO *
CreateVfrTargetInfoByVfrLinkTarget (
  IN     H2O_IFR_GUID_LINK_TARGET             *LinkTargetOpcode,
  IN OUT FORM_BROWSER_FORM                    *Form
  );

H2O_VFR_TARGET_INFO *
CreateVfrTargetInfoByVfrImportTarget (
  IN     H2O_IFR_GUID_IMPORT_TARGET           *ImportTargetOpcode,
  IN OUT FORM_BROWSER_FORM                    *Form
  );

VOID
DestroyAllVfrTargetInfo (
  IN OUT FORM_BROWSER_FORM                    *Form
  );

BOOLEAN
HaveTargetInfo (
  IN EFI_GUID                                 *FormSetGuid,
  IN UINT16                                   FormId  OPTIONAL,
  IN UINT16                                   LabelId OPTIONAL
  );

EFI_STATUS
InitializeTargetInfoList (
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA        *Private
  );

VOID
DestroyTargetInfoList (
  IN OUT H2O_FORM_BROWSER_PRIVATE_DATA        *Private
  );

EFI_STATUS
GetImportInfoListFromTargetInfo (
  IN     FORM_BROWSER_FORMSET                 *FormSet,
  IN OUT IMPORT_INFO                          **ImportInfoList,
  IN OUT UINT16                               *ImportInfoCount
  );

#endif
