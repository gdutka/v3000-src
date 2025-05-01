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

#ifndef HII_STRING_ID_MANAGE_PRIVATE_DATA_H_
#define HII_STRING_ID_MANAGE_PRIVATE_DATA_H_

#include <Uefi.h>
#include <Uefi/UefiInternalFormRepresentation.h>

//
// StringIdPool array breakdown
//
// Start index        End index              Use
// StartIndices[N-1]  (StartIndices[N-2]-1)  String ID list for FormIndex (N-1)
// StartIndices[N-2]  (StartIndices[N-1]-1)  String ID list for FormIndex (N-2)
// ...                ...                    ...
// StartIndices[1]    (StartIndices[0]-1)    String ID list for FormIndex 1
// StartIndices[0]    (UnusedStartIndex-1)   String ID list for FormIndex 0
// UnusedStartIndex   (PoolLength-1)         String IDs unallocated to any form
//
// StartIndices[N-1] always equals 0.
//

#pragma pack(1)

typedef struct {
  EFI_HII_HANDLE  HiiHandle;
  EFI_STRING_ID   *StringIdPool;
  UINT16          NumberOfForms;
  UINT16          StringIdPoolLength;
  UINT16          UnusedStringIdStartIndex;
  //
  //
  //
  // UINT16       StringIdStartIndices[NumberOfForms];
} HII_STRING_ID_MANAGE_PRIVATE_DATA;

#pragma pack()

#endif // HII_STRING_ID_MANAGE_PRIVATE_DATA_H_
