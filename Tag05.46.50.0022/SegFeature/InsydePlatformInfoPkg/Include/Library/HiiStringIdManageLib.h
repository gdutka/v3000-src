/** @file


;******************************************************************************
;* Copyright 2022 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef HII_STRING_ID_MANAGE_LIB_H_
#define HII_STRING_ID_MANAGE_LIB_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

typedef VOID  *HII_STRING_ID_MANAGER;

/**

  @param[in]  HiiHandle
  @param[in]  NumberOfForms

  @retval NULL
  @retval Others

**/
HII_STRING_ID_MANAGER
CreateHiiStringIdManager (
  IN EFI_HII_HANDLE  HiiHandle,
  IN UINT16          NumberOfForms
  );

/**

  @param[in]  HiiStringIdManager

  @return Void.

**/
VOID
DestroyHiiStringIdManager (
  IN HII_STRING_ID_MANAGER  HiiStringIdManager
  );

/**

  @param[in]  HiiStringIdManager
  @param[in]  FormIndex
  @param[out] DynamicStringCount
  @param[out] StringIdList

  @return Void.

**/
VOID
GetFormDynamicStringCount (
  IN CONST HII_STRING_ID_MANAGER  HiiStringIdManager,
  IN UINT16                       FormIndex,
  OUT UINT16                      *DynamicStringCount,
  OUT CONST EFI_STRING_ID         **StringIdList
  );

/**

  @param[in]  HiiStringIdManager

  @return

**/
UINT16
GetUnusedStringIdCount (
  IN CONST HII_STRING_ID_MANAGER  HiiStringIdManager
  );

/**

  @param[in,out] HiiStringIdManager
  @param[in]     FormIndex
  @param[in]     DynamicStringCount
  @param[out]    StringIdList

  @retval EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES

**/
EFI_STATUS
SetFormDynamicStringCount (
  IN OUT HII_STRING_ID_MANAGER  HiiStringIdManager,
  IN UINT16                     FormIndex,
  IN UINT16                     DynamicStringCount,
  OUT CONST EFI_STRING_ID       **StringIdList OPTIONAL
  );

/**

  @param[in,out] HiiStringIdManager
  @param[in]     FormIndex

  @retval 0
  @retval Others

**/
EFI_STRING_ID
AllocateStringIdForForm (
  IN OUT HII_STRING_ID_MANAGER  HiiStringIdManager,
  IN UINT16                     FormIndex
  );

#endif // HII_STRING_ID_MANAGE_LIB_H_
