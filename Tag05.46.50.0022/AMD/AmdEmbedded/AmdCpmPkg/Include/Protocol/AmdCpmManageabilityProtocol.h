/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_CPM_MANAGEABILITY_PROTOCOL_H_
#define _AMD_CPM_MANAGEABILITY_PROTOCOL_H_

#include <Uefi/UefiBaseType.h>

//
// Extern the GUID for protocol users
//
extern EFI_GUID gAmdCpmManageabilityProtocolGuid;

typedef struct _AMD_CPM_MANAGEABILITY_PROTOCOL AMD_CPM_MANAGEABILITY_PROTOCOL;


#define MANOS_TARGET_MAN_OS         0x37
#define MANOS_TARGET_RST_HOST_OS    0xFE
#define MANOS_TARGET_HOST_OS        0xFF

/**
  Check with EC whether the system should boot to manageability OS or not.

  @retval 1             The system should boot to manageability OS
  @retval other value   The system should not boot to manageablity OS
**/
typedef
UINT8
(EFIAPI *CHECK_MAN_OS_BOOT_FLAG) (
);

/**
  Get the manageability OS binary information on SPI

  @param[in] BiosEntryType        The type of BiosEntry
  @param[in] MemAddr              The memory address where should the manageability OS image be loaded
  @param[in] BufferSize           The size of the memory buffer


  @retval EFI_SUCCESS               Load the manageability OS image successfully
**/
typedef
EFI_STATUS
(EFIAPI *LOAD_IMAGE) (
  IN  UINT8   BiosEntryType,
  IN  VOID    *MemAddr,
  IN  UINT32  BufferSize
);

/**
  Set manageablity OS jump flag to EC

  @retval EFI_SUCCESS               Set the manageability OS jump flag successfully
**/
typedef
EFI_STATUS
(EFIAPI *SET_MAN_OS_JUMP_FLAG) (
);

/**
  Get manageablity OS jump flag to EC

  @param[out] JumpOffset

  @retval EFI_SUCCESS               Set the manageability OS jump flag successfully
**/
typedef
EFI_STATUS
(EFIAPI *GET_MAN_OS_JUMP_FLAG) (
  OUT UINT32  *JumpOffset
);

//
// Protocol Interface Structure
//
typedef struct _AMD_CPM_MANAGEABILITY_PROTOCOL
{
  UINTN                     Revision;               ///< Revision Number
  CHECK_MAN_OS_BOOT_FLAG    CheckManOsBootFlag;
  LOAD_IMAGE                LoadImage;
  SET_MAN_OS_JUMP_FLAG      SetManOsJumpFlag;
  GET_MAN_OS_JUMP_FLAG      GetManOsJumpFlag;
} AMD_CPM_MANAGEABILITY_PROTOCOL;

#define AMD_CPM_MANAGEABILITY_PROTOCOL_REVISION 0x00

#endif // _AMD_CPM_MANAGEABILITY_PROTOCOL_H_
