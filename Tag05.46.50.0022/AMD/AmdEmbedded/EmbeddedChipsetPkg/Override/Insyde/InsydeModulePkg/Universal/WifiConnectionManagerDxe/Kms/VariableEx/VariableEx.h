/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/** @file
  The internal header file includes the common header files, defines
  internal structure and functions used by Variable modules.

Copyright (c) 2013 - 2017, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <Protocol/Variable.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/EventGroup.h>
#include <Guid/VariableFormat.h>

#include "VariableFormatEx.h"
#include "KeyLib.h"

//
// EDKII extension for Variable AttributesEx
//
#define EDKII_VARIABLE_KEY_AUTHENTICATED      0x01
#define EDKII_VARIABLE_KEY_ENCRYPTED          0x02

//
// EDKII extension
//
#define EDKII_VARIABLE_KEY_TYPE_RAW      0
#define EDKII_VARIABLE_KEY_TYPE_ASCII    1
#define EDKII_VARIABLE_KEY_TYPE_UNICODE  2
typedef struct {
  UINT32                      KeyType;
  UINT32                      KeySize;
  //  union {
  //    UINT8                       RawData[KeySize];
  //    CHAR8                       AsciiData[KeySize];
  //    CHAR16                      UnicodeData[KeySize/2];
  //  } Data;
} EDKII_VARIABLE_KEY_DATA;

#define EFI_VARIABLE_ATTRIBUTES_MASK (EFI_VARIABLE_NON_VOLATILE | \
                                      EFI_VARIABLE_BOOTSERVICE_ACCESS | \
                                      EFI_VARIABLE_RUNTIME_ACCESS | \
                                      EFI_VARIABLE_HARDWARE_ERROR_RECORD | \
                                      EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS | \
                                      EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS | \
                                      EFI_VARIABLE_APPEND_WRITE)

/**

  This code finds variable in storage blocks (Volatile or Non-Volatile).

  Caution: This function may receive untrusted input.
  This function may be invoked in SMM mode, and datasize is external input.
  This function will do basic validation, before parse the data.

  @param[in] VariableName               Name of Variable to be found.
  @param[in] VendorGuid                 Variable vendor GUID.
  @param[out] Attributes                Attribute value of the variable found.
  @param[in,out] AttributesEx           AttributeEx value of the variable found.
  @param[in,out] DataSize               Size of Data found. If size is less than the
                                          data, this value contains the required size.
  @param[out] Data                      The buffer to return the contents of the variable. May be NULL
                                          with a zero DataSize in order to determine the size buffer needed.

  @return EFI_INVALID_PARAMETER         Invalid parameter.
  @return EFI_SUCCESS                   Find the specified variable.
  @return EFI_NOT_FOUND                 Not found.
  @return EFI_BUFFER_TO_SMALL           DataSize is too small for the result.

**/
EFI_STATUS
EFIAPI
VariableServiceGetVariableEx (
  IN      CHAR16            *VariableName,
  IN      EFI_GUID          *VendorGuid,
  OUT     UINT32            *Attributes OPTIONAL,
  IN OUT  UINT8             *AttributesEx OPTIONAL,
  IN OUT  UINTN             *DataSize,
  OUT     VOID              *Data
  );

/**

  This code sets variable in storage blocks (Volatile or Non-Volatile).

  Caution: This function may receive untrusted input.
  This function may be invoked in SMM mode, and datasize and data are external input.
  This function will do basic validation, before parse the data.
  This function will parse the authentication carefully to avoid security issues, like
  buffer overflow, integer overflow.
  This function will check attribute carefully to avoid authentication bypass.

  @param[in] VariableName                     Name of Variable to be found.
  @param[in] VendorGuid                       Variable vendor GUID.
  @param[in] Attributes                       Attribute value of the variable found
  @param[in] AttributesEx                     AttributeEx value of the variable found
  @param[in] DataSize                         Size of Data found. If size is less than the
                                                data, this value contains the required size.
  @param[in] Data                             Data pointer.

  @return EFI_INVALID_PARAMETER               Invalid parameter.
  @return EFI_SUCCESS                         Set successfully.
  @return EFI_OUT_OF_RESOURCES                Resource not enough to set variable.
  @return EFI_NOT_FOUND                       Not found.
  @return EFI_WRITE_PROTECTED                 Variable is read-only.

**/
EFI_STATUS
EFIAPI
VariableServiceSetVariableEx (
  IN CHAR16                  *VariableName,
  IN EFI_GUID                *VendorGuid,
  IN UINT32                  Attributes,
  IN UINT8                   AttributesEx,
  IN UINTN                   DataSize,
  IN VOID                    *Data
  );

#endif
