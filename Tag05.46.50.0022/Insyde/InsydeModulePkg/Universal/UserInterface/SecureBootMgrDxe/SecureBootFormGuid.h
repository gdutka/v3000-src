/** @file
  Formset guids, form id and VarStore data structure for Boot Maintenance Manager.

;******************************************************************************
;* Copyright (c) 2012 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _SECURE_BOOT_FORM_GUID_H_
#define _SECURE_BOOT_FORM_GUID_H_


#define SECURE_BOOT_FORMSET_GUID \
  { \
    0xaa1305b9, 0x1f3, 0x4afb, 0x92, 0xe, 0xc9, 0xb9, 0x79, 0xa8, 0x52, 0xfd\
  }
//
// Secure Boot Manager rlative definitions
//
#define SECURE_BOOT_FORM_ID                  0x1002
#define FORM_ENROLL_HASH_ID                  0x1000
#define FORM_ID_PK                           0x1003
#define FORM_ID_PK_ENROLL_SIGNATURE          0x1004
#define FORM_ID_PK_DELETE_SIGNATURE          0x1005
#define FORM_ID_KEK                          0x1006
#define FORM_ID_KEK_ENROLL_SIGNATURE         0x1007
#define FORM_ID_KEK_DELETE_SIGNATURE         0x1008
#define FORM_ID_DB                           0x1009
#define FORM_ID_DB_ENROLL_SIGNATURE          0x100A
#define FORM_ID_DB_DELETE_SIGNATURE          0x100B
#define FORM_ID_DBX                          0x100C
#define FORM_ID_DBX_ENROLL_SIGNATURE         0x100D
#define FORM_ID_DBX_DELETE_SIGNATURE         0x100E
#define FORM_ID_DBT                          0x100F
#define FORM_ID_DBT_ENROLL_SIGNATURE         0x1010
#define FORM_ID_DBT_DELETE_SIGNATURE         0x1011
#define FORM_ID_DBR                          0x1012
#define FORM_ID_DBR_ENROLL_SIGNATURE         0x1013
#define FORM_ID_DBR_DELETE_SIGNATURE         0x1014
//
// For Security Formset
//
#define KEY_ENFORCE_SECURE_BOOT              0x1919
#define KEY_ENFORCE_SECURE_BOOT_GRAYOUT      0x191A
#define KEY_CLEAR_SECURE_SETTINGS            0x191B
#define KEY_RESOTRE_FACTORY_SETTINGS         0x191C
#define KEY_KEK_DELETE_SIGNATURE             0x191D
#define KEY_DB_DELETE_SIGNATURE              0x191E
#define KEY_DBX_DELETE_SIGNATURE             0x190E
#define KEY_DBT_DELETE_SIGNATURE             0x190F
#define KEY_DBR_DELETE_SIGNATURE             0x1911
#define KEY_ADD_UEFI_CA_SIGNATURE            0x191F

#define KEY_ROOT_FORM                        0x4431
#define KEY_ENROLL_HASH                      0x4432
#define KEY_PK_FORM                          0x4433
#define KEY_KEK_FORM                         0x4434
#define KEY_DB_FORM                          0x4435
#define KEY_DBX_FORM                         0x4436
#define KEY_DBT_FORM                         0x4437
#define KEY_DBR_FORM                         0x4438
#define KEY_ENROLL_PK                        0x4439
#define KEY_ENROLL_KEK                       0x443A
#define KEY_ENROLL_DB                        0x443B
#define KEY_ENROLL_DBX                       0x443C
#define KEY_ENROLL_DBT                       0x443D
#define KEY_ENROLL_DBR                       0x443E
#define KEY_DELETE_PK                        0x443F
#define KEY_DELETE_SIGNATURE_QUESTION_BASE   0x5000
#define KEY_DELETE_SIGNATURE_QUESTION_LENGTH 0x1000

#define SECURE_BOOT_CLASS              0x00
#define SECURE_BOOT_SUBCLASS           0x03

#define VARSTORE_ID_SECURE_BOOT        0x1003


#define FORM_ENROLL_FILE_ID                  0x1034
#define FORM_ENROLL_FILE_END_ID              0x1035
#define LABEL_ID_SIGNATURE_LIST_START        0x1036
#define LABEL_ID_SIGNATURE_LIST_END          0x1037
#define LABEL_ID_DELETE_SIGNATURE_LIST_START 0x1038
#define LABEL_ID_DELETE_SIGNATURE_LIST_END   0x1039

#define GUID_STRING_LENGTH                   36 ///< GUID string format: L"12345678-1234-1234-1234-1234567890ab"
#define GUID_STRING_STORAGE_LENGTH           37

typedef struct {
  UINT8   SetupMode;
  UINT8   SecureBoot;
  UINT8   AuditMode;
  UINT8   DeployedMode;
  UINT8   CustomSecurity;
  UINT8   EnforceSecureBoot;
  UINT8   ClearSecureSettings;
  UINT8   ResotreFactorySettings;
  UINT8   SelectSecureBootMode;
  UINT8   BackupSelectSecureBootMode;
  UINT8   ResotreBackupSettings;
  UINT8   UefiCaCertificateInDb;
  UINT8   AddUefiCaCertificate;
  CHAR16  EnrollSigGuidStr[GUID_STRING_STORAGE_LENGTH];
} SECURE_BOOT_NV_DATA;

#endif

