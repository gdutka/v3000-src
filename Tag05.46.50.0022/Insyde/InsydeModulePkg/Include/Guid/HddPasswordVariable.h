/** @file
  Definitions for HDD password feature

;******************************************************************************
;* Copyright (c) 2012 - 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _EFI_HDD_PASSWORD_VARIABLE_H_
#define _EFI_HDD_PASSWORD_VARIABLE_H_

#include <Uefi.h>

//
// Variable Name
//
#define SAVE_HDD_PASSWORD_VARIABLE_NAME        L"SaveHddPassword"
#define HDD_PASSWORD_RANDOM_NUM_SIZE           16
#define HDD_PASSWORD_ROOT_KEY_SIZE             32

//
// Variable Guid
//
#define HDD_PASSWORD_TABLE_GUID \
  { 0x86bbf7e3, 0xb772, 0x4d22, 0x80, 0xa9, 0xe7, 0xc5, 0x8c, 0x3c, 0x7f, 0xf0 }

#define VMD_PHY_SATA_DEVICE_PATH_GUID\
  {0x95c0e7c6, 0x1c1, 0x4937, 0x8f, 0x22, 0x8b, 0xc6, 0xc, 0xc9, 0xbc, 0xe4} 

#define HDD_PASSWORD_MAX_NUMBER         32

typedef struct _HDD_PASSWORD_TABLE {
  UINT16                                ControllerNumber;
  UINT16                                PortNumber;
  UINT16                                PortMulNumber;
  UINT16                                MasterPasswordIdentifier;   
  UINT8                                 PasswordType;
  CHAR16                                PasswordStr[HDD_PASSWORD_MAX_NUMBER + 1];
  UINT16                                ExtDataSize;
  UINT16                                DevicePathSize;
  UINT32                                TpmNvIndex;
//  UINT8                               ExtData[1];
//  EFI_DEVICE_PATH                     devicepath;
} HDD_PASSWORD_TABLE;

typedef struct _TCG_OPAL_EXT_DATA_ {
  UINT32                                Signature;
  CHAR16                                SIDPasswordStr[HDD_PASSWORD_MAX_NUMBER + 1];
  CHAR16                                Admin1PasswordStr[HDD_PASSWORD_MAX_NUMBER + 1];
  CHAR16                                User1PasswordStr[HDD_PASSWORD_MAX_NUMBER + 1];
  CHAR16                                User2PasswordStr[HDD_PASSWORD_MAX_NUMBER + 1];
} TCG_OPAL_EXT_DATA;

#define TCG_OPAL_EXT_DATA_SIGNATURE     SIGNATURE_32 ('o', 'p', 'a', 'l')

extern EFI_GUID gSaveHddPasswordGuid;
extern EFI_GUID gVmdPhySataDevicePathGuid;

#endif
