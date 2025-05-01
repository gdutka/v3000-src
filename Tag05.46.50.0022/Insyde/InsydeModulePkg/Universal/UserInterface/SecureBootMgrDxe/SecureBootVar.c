/** @file
  The secure boot variable function implementation.

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "SecureBootVar.h"

#define TAG_TYPE_VALUE_OID             0x06    /*  6: Object Identifier */
#define TAG_TYPE_VALUE_SEQUENCE        0x30    /* 16: Sequence/sequence of */
#define TAG_TYPE_VALUE_SET             0x31    /* 17: Set/set of */
#define TAG_TYPE_VALUE_UTF8STRING      0x0C    /* 12: UTF8 string */
#define TAG_TYPE_VALUE_PRINTABLESTRING 0x13    /* 19: Printable string (ASCII subset) */

#define LEN_XTND                       0x80    /* Indefinite or long form */
#define LEN_MASK                       0x7F    /* Bits 7 - 1 */

STATIC UINT8                           mSubjectCommonNameOID[] = {0x55, 0x04, 0x03};    /* 2.5.4.3 */

typedef struct {
  EFI_GUID                             *SignatureType;
  EFI_STRING_ID                        StringId;
} SECURE_BOOT_SIGNATURE_TYPE_HII_INFO;

STATIC SECURE_BOOT_SIGNATURE_TYPE_HII_INFO  mSigTypeHiiInfo[] = {
  {&gEfiCertRsa2048Guid   , STRING_TOKEN (STR_SIGNATURE_TYPE_RSA2048_SHA256)},
  {&gEfiCertX509Guid      , STRING_TOKEN (STR_SIGNATURE_TYPE_PCKS7)         },
  {&gEfiCertSha1Guid      , STRING_TOKEN (STR_SIGNATURE_TYPE_SHA1)          },
  {&gEfiCertSha256Guid    , STRING_TOKEN (STR_SIGNATURE_TYPE_SHA256)        },
  {&gEfiCertSha512Guid    , STRING_TOKEN (STR_SIGNATURE_TYPE_SHA512)        },
  {&gEfiCertX509Sha256Guid, STRING_TOKEN (STR_SIGNATURE_TYPE_X509_SHA256)   },
  {&gEfiCertX509Sha384Guid, STRING_TOKEN (STR_SIGNATURE_TYPE_X509_SHA384)   },
  {&gEfiCertX509Sha512Guid, STRING_TOKEN (STR_SIGNATURE_TYPE_X509_SHA512)   }
  };

typedef struct {
  UINT8                *VarData;
  UINTN                VarDataSize;
} SECURE_BOOT_VAR_INFO;

STATIC SECURE_BOOT_VAR_INFO            mPkVarInfo  = {NULL, 0};
STATIC SECURE_BOOT_VAR_INFO            mKekVarInfo = {NULL, 0};
STATIC SECURE_BOOT_VAR_INFO            mDbVarInfo  = {NULL, 0};
STATIC SECURE_BOOT_VAR_INFO            mDbxVarInfo = {NULL, 0};
STATIC SECURE_BOOT_VAR_INFO            mDbtVarInfo = {NULL, 0};
STATIC SECURE_BOOT_VAR_INFO            mDbrVarInfo = {NULL, 0};

STRING_DEPOSITORY                      *mSecureBootSigOptionStrDepository;

STATIC EFI_GUID                        mUefiCaCertificateOwnerGuid = { 0x77fa9abd, 0x0359, 0x4d32, 0xbd, 0x60, 0x28, 0xf4, 0xe7, 0x8f, 0x78, 0x4b };
STATIC UINT8                           mUefiCa2011Certificate[] = {
	0x30, 0x82, 0x06, 0x10, 0x30, 0x82, 0x03, 0xF8, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x0A, 0x61,
	0x08, 0xD3, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48,
	0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x30, 0x81, 0x91, 0x31, 0x0B, 0x30, 0x09, 0x06,
	0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04,
	0x08, 0x13, 0x0A, 0x57, 0x61, 0x73, 0x68, 0x69, 0x6E, 0x67, 0x74, 0x6F, 0x6E, 0x31, 0x10, 0x30,
	0x0E, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x07, 0x52, 0x65, 0x64, 0x6D, 0x6F, 0x6E, 0x64, 0x31,
	0x1E, 0x30, 0x1C, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x15, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73,
	0x6F, 0x66, 0x74, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x31,
	0x3B, 0x30, 0x39, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x32, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73,
	0x6F, 0x66, 0x74, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20,
	0x54, 0x68, 0x69, 0x72, 0x64, 0x20, 0x50, 0x61, 0x72, 0x74, 0x79, 0x20, 0x4D, 0x61, 0x72, 0x6B,
	0x65, 0x74, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x20, 0x52, 0x6F, 0x6F, 0x74, 0x30, 0x1E, 0x17, 0x0D,
	0x31, 0x31, 0x30, 0x36, 0x32, 0x37, 0x32, 0x31, 0x32, 0x32, 0x34, 0x35, 0x5A, 0x17, 0x0D, 0x32,
	0x36, 0x30, 0x36, 0x32, 0x37, 0x32, 0x31, 0x33, 0x32, 0x34, 0x35, 0x5A, 0x30, 0x81, 0x81, 0x31,
	0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13, 0x30, 0x11,
	0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x0A, 0x57, 0x61, 0x73, 0x68, 0x69, 0x6E, 0x67, 0x74, 0x6F,
	0x6E, 0x31, 0x10, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x07, 0x52, 0x65, 0x64, 0x6D,
	0x6F, 0x6E, 0x64, 0x31, 0x1E, 0x30, 0x1C, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x15, 0x4D, 0x69,
	0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F, 0x72, 0x61, 0x74,
	0x69, 0x6F, 0x6E, 0x31, 0x2B, 0x30, 0x29, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x22, 0x4D, 0x69,
	0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F, 0x72, 0x61, 0x74,
	0x69, 0x6F, 0x6E, 0x20, 0x55, 0x45, 0x46, 0x49, 0x20, 0x43, 0x41, 0x20, 0x32, 0x30, 0x31, 0x31,
	0x30, 0x82, 0x01, 0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01,
	0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01,
	0x00, 0xA5, 0x08, 0x6C, 0x4C, 0xC7, 0x45, 0x09, 0x6A, 0x4B, 0x0C, 0xA4, 0xC0, 0x87, 0x7F, 0x06,
	0x75, 0x0C, 0x43, 0x01, 0x54, 0x64, 0xE0, 0x16, 0x7F, 0x07, 0xED, 0x92, 0x7D, 0x0B, 0xB2, 0x73,
	0xBF, 0x0C, 0x0A, 0xC6, 0x4A, 0x45, 0x61, 0xA0, 0xC5, 0x16, 0x2D, 0x96, 0xD3, 0xF5, 0x2B, 0xA0,
	0xFB, 0x4D, 0x49, 0x9B, 0x41, 0x80, 0x90, 0x3C, 0xB9, 0x54, 0xFD, 0xE6, 0xBC, 0xD1, 0x9D, 0xC4,
	0xA4, 0x18, 0x8A, 0x7F, 0x41, 0x8A, 0x5C, 0x59, 0x83, 0x68, 0x32, 0xBB, 0x8C, 0x47, 0xC9, 0xEE,
	0x71, 0xBC, 0x21, 0x4F, 0x9A, 0x8A, 0x7C, 0xFF, 0x44, 0x3F, 0x8D, 0x8F, 0x32, 0xB2, 0x26, 0x48,
	0xAE, 0x75, 0xB5, 0xEE, 0xC9, 0x4C, 0x1E, 0x4A, 0x19, 0x7E, 0xE4, 0x82, 0x9A, 0x1D, 0x78, 0x77,
	0x4D, 0x0C, 0xB0, 0xBD, 0xF6, 0x0F, 0xD3, 0x16, 0xD3, 0xBC, 0xFA, 0x2B, 0xA5, 0x51, 0x38, 0x5D,
	0xF5, 0xFB, 0xBA, 0xDB, 0x78, 0x02, 0xDB, 0xFF, 0xEC, 0x0A, 0x1B, 0x96, 0xD5, 0x83, 0xB8, 0x19,
	0x13, 0xE9, 0xB6, 0xC0, 0x7B, 0x40, 0x7B, 0xE1, 0x1F, 0x28, 0x27, 0xC9, 0xFA, 0xEF, 0x56, 0x5E,
	0x1C, 0xE6, 0x7E, 0x94, 0x7E, 0xC0, 0xF0, 0x44, 0xB2, 0x79, 0x39, 0xE5, 0xDA, 0xB2, 0x62, 0x8B,
	0x4D, 0xBF, 0x38, 0x70, 0xE2, 0x68, 0x24, 0x14, 0xC9, 0x33, 0xA4, 0x08, 0x37, 0xD5, 0x58, 0x69,
	0x5E, 0xD3, 0x7C, 0xED, 0xC1, 0x04, 0x53, 0x08, 0xE7, 0x4E, 0xB0, 0x2A, 0x87, 0x63, 0x08, 0x61,
	0x6F, 0x63, 0x15, 0x59, 0xEA, 0xB2, 0x2B, 0x79, 0xD7, 0x0C, 0x61, 0x67, 0x8A, 0x5B, 0xFD, 0x5E,
	0xAD, 0x87, 0x7F, 0xBA, 0x86, 0x67, 0x4F, 0x71, 0x58, 0x12, 0x22, 0x04, 0x22, 0x22, 0xCE, 0x8B,
	0xEF, 0x54, 0x71, 0x00, 0xCE, 0x50, 0x35, 0x58, 0x76, 0x95, 0x08, 0xEE, 0x6A, 0xB1, 0xA2, 0x01,
	0xD5, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x82, 0x01, 0x76, 0x30, 0x82, 0x01, 0x72, 0x30, 0x12,
	0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x15, 0x01, 0x04, 0x05, 0x02, 0x03, 0x01,
	0x00, 0x01, 0x30, 0x23, 0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x15, 0x02, 0x04,
	0x16, 0x04, 0x14, 0xF8, 0xC1, 0x6B, 0xB7, 0x7F, 0x77, 0x53, 0x4A, 0xF3, 0x25, 0x37, 0x1D, 0x4E,
	0xA1, 0x26, 0x7B, 0x0F, 0x20, 0x70, 0x80, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16,
	0x04, 0x14, 0x13, 0xAD, 0xBF, 0x43, 0x09, 0xBD, 0x82, 0x70, 0x9C, 0x8C, 0xD5, 0x4F, 0x31, 0x6E,
	0xD5, 0x22, 0x98, 0x8A, 0x1B, 0xD4, 0x30, 0x19, 0x06, 0x09, 0x2B, 0x06, 0x01, 0x04, 0x01, 0x82,
	0x37, 0x14, 0x02, 0x04, 0x0C, 0x1E, 0x0A, 0x00, 0x53, 0x00, 0x75, 0x00, 0x62, 0x00, 0x43, 0x00,
	0x41, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x04, 0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x0F,
	0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF, 0x30,
	0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x45, 0x66, 0x52, 0x43,
	0xE1, 0x7E, 0x58, 0x11, 0xBF, 0xD6, 0x4E, 0x9E, 0x23, 0x55, 0x08, 0x3B, 0x3A, 0x22, 0x6A, 0xA8,
	0x30, 0x5C, 0x06, 0x03, 0x55, 0x1D, 0x1F, 0x04, 0x55, 0x30, 0x53, 0x30, 0x51, 0xA0, 0x4F, 0xA0,
	0x4D, 0x86, 0x4B, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x63, 0x72, 0x6C, 0x2E, 0x6D, 0x69,
	0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x70, 0x6B, 0x69, 0x2F,
	0x63, 0x72, 0x6C, 0x2F, 0x70, 0x72, 0x6F, 0x64, 0x75, 0x63, 0x74, 0x73, 0x2F, 0x4D, 0x69, 0x63,
	0x43, 0x6F, 0x72, 0x54, 0x68, 0x69, 0x50, 0x61, 0x72, 0x4D, 0x61, 0x72, 0x52, 0x6F, 0x6F, 0x5F,
	0x32, 0x30, 0x31, 0x30, 0x2D, 0x31, 0x30, 0x2D, 0x30, 0x35, 0x2E, 0x63, 0x72, 0x6C, 0x30, 0x60,
	0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x01, 0x01, 0x04, 0x54, 0x30, 0x52, 0x30, 0x50,
	0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x02, 0x86, 0x44, 0x68, 0x74, 0x74, 0x70,
	0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x6D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74,
	0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x70, 0x6B, 0x69, 0x2F, 0x63, 0x65, 0x72, 0x74, 0x73, 0x2F, 0x4D,
	0x69, 0x63, 0x43, 0x6F, 0x72, 0x54, 0x68, 0x69, 0x50, 0x61, 0x72, 0x4D, 0x61, 0x72, 0x52, 0x6F,
	0x6F, 0x5F, 0x32, 0x30, 0x31, 0x30, 0x2D, 0x31, 0x30, 0x2D, 0x30, 0x35, 0x2E, 0x63, 0x72, 0x74,
	0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x03,
	0x82, 0x02, 0x01, 0x00, 0x35, 0x08, 0x42, 0xFF, 0x30, 0xCC, 0xCE, 0xF7, 0x76, 0x0C, 0xAD, 0x10,
	0x68, 0x58, 0x35, 0x29, 0x46, 0x32, 0x76, 0x27, 0x7C, 0xEF, 0x12, 0x41, 0x27, 0x42, 0x1B, 0x4A,
	0xAA, 0x6D, 0x81, 0x38, 0x48, 0x59, 0x13, 0x55, 0xF3, 0xE9, 0x58, 0x34, 0xA6, 0x16, 0x0B, 0x82,
	0xAA, 0x5D, 0xAD, 0x82, 0xDA, 0x80, 0x83, 0x41, 0x06, 0x8F, 0xB4, 0x1D, 0xF2, 0x03, 0xB9, 0xF3,
	0x1A, 0x5D, 0x1B, 0xF1, 0x50, 0x90, 0xF9, 0xB3, 0x55, 0x84, 0x42, 0x28, 0x1C, 0x20, 0xBD, 0xB2,
	0xAE, 0x51, 0x14, 0xC5, 0xC0, 0xAC, 0x97, 0x95, 0x21, 0x1C, 0x90, 0xDB, 0x0F, 0xFC, 0x77, 0x9E,
	0x95, 0x73, 0x91, 0x88, 0xCA, 0xBD, 0xBD, 0x52, 0xB9, 0x05, 0x50, 0x0D, 0xDF, 0x57, 0x9E, 0xA0,
	0x61, 0xED, 0x0D, 0xE5, 0x6D, 0x25, 0xD9, 0x40, 0x0F, 0x17, 0x40, 0xC8, 0xCE, 0xA3, 0x4A, 0xC2,
	0x4D, 0xAF, 0x9A, 0x12, 0x1D, 0x08, 0x54, 0x8F, 0xBD, 0xC7, 0xBC, 0xB9, 0x2B, 0x3D, 0x49, 0x2B,
	0x1F, 0x32, 0xFC, 0x6A, 0x21, 0x69, 0x4F, 0x9B, 0xC8, 0x7E, 0x42, 0x34, 0xFC, 0x36, 0x06, 0x17,
	0x8B, 0x8F, 0x20, 0x40, 0xC0, 0xB3, 0x9A, 0x25, 0x75, 0x27, 0xCD, 0xC9, 0x03, 0xA3, 0xF6, 0x5D,
	0xD1, 0xE7, 0x36, 0x54, 0x7A, 0xB9, 0x50, 0xB5, 0xD3, 0x12, 0xD1, 0x07, 0xBF, 0xBB, 0x74, 0xDF,
	0xDC, 0x1E, 0x8F, 0x80, 0xD5, 0xED, 0x18, 0xF4, 0x2F, 0x14, 0x16, 0x6B, 0x2F, 0xDE, 0x66, 0x8C,
	0xB0, 0x23, 0xE5, 0xC7, 0x84, 0xD8, 0xED, 0xEA, 0xC1, 0x33, 0x82, 0xAD, 0x56, 0x4B, 0x18, 0x2D,
	0xF1, 0x68, 0x95, 0x07, 0xCD, 0xCF, 0xF0, 0x72, 0xF0, 0xAE, 0xBB, 0xDD, 0x86, 0x85, 0x98, 0x2C,
	0x21, 0x4C, 0x33, 0x2B, 0xF0, 0x0F, 0x4A, 0xF0, 0x68, 0x87, 0xB5, 0x92, 0x55, 0x32, 0x75, 0xA1,
	0x6A, 0x82, 0x6A, 0x3C, 0xA3, 0x25, 0x11, 0xA4, 0xED, 0xAD, 0xD7, 0x04, 0xAE, 0xCB, 0xD8, 0x40,
	0x59, 0xA0, 0x84, 0xD1, 0x95, 0x4C, 0x62, 0x91, 0x22, 0x1A, 0x74, 0x1D, 0x8C, 0x3D, 0x47, 0x0E,
	0x44, 0xA6, 0xE4, 0xB0, 0x9B, 0x34, 0x35, 0xB1, 0xFA, 0xB6, 0x53, 0xA8, 0x2C, 0x81, 0xEC, 0xA4,
	0x05, 0x71, 0xC8, 0x9D, 0xB8, 0xBA, 0xE8, 0x1B, 0x44, 0x66, 0xE4, 0x47, 0x54, 0x0E, 0x8E, 0x56,
	0x7F, 0xB3, 0x9F, 0x16, 0x98, 0xB2, 0x86, 0xD0, 0x68, 0x3E, 0x90, 0x23, 0xB5, 0x2F, 0x5E, 0x8F,
	0x50, 0x85, 0x8D, 0xC6, 0x8D, 0x82, 0x5F, 0x41, 0xA1, 0xF4, 0x2E, 0x0D, 0xE0, 0x99, 0xD2, 0x6C,
	0x75, 0xE4, 0xB6, 0x69, 0xB5, 0x21, 0x86, 0xFA, 0x07, 0xD1, 0xF6, 0xE2, 0x4D, 0xD1, 0xDA, 0xAD,
	0x2C, 0x77, 0x53, 0x1E, 0x25, 0x32, 0x37, 0xC7, 0x6C, 0x52, 0x72, 0x95, 0x86, 0xB0, 0xF1, 0x35,
	0x61, 0x6A, 0x19, 0xF5, 0xB2, 0x3B, 0x81, 0x50, 0x56, 0xA6, 0x32, 0x2D, 0xFE, 0xA2, 0x89, 0xF9,
	0x42, 0x86, 0x27, 0x18, 0x55, 0xA1, 0x82, 0xCA, 0x5A, 0x9B, 0xF8, 0x30, 0x98, 0x54, 0x14, 0xA6,
	0x47, 0x96, 0x25, 0x2F, 0xC8, 0x26, 0xE4, 0x41, 0x94, 0x1A, 0x5C, 0x02, 0x3F, 0xE5, 0x96, 0xE3,
	0x85, 0x5B, 0x3C, 0x3E, 0x3F, 0xBB, 0x47, 0x16, 0x72, 0x55, 0xE2, 0x25, 0x22, 0xB1, 0xD9, 0x7B,
	0xE7, 0x03, 0x06, 0x2A, 0xA3, 0xF7, 0x1E, 0x90, 0x46, 0xC3, 0x00, 0x0D, 0xD6, 0x19, 0x89, 0xE3,
	0x0E, 0x35, 0x27, 0x62, 0x03, 0x71, 0x15, 0xA6, 0xEF, 0xD0, 0x27, 0xA0, 0xA0, 0x59, 0x37, 0x60,
	0xF8, 0x38, 0x94, 0xB8, 0xE0, 0x78, 0x70, 0xF8, 0xBA, 0x4C, 0x86, 0x87, 0x94, 0xF6, 0xE0, 0xAE,
	0x02, 0x45, 0xEE, 0x65, 0xC2, 0xB6, 0xA3, 0x7E, 0x69, 0x16, 0x75, 0x07, 0x92, 0x9B, 0xF5, 0xA6,
	0xBC, 0x59, 0x83, 0x58
};

STATIC UINT8                           mUefiCa2023Certificate[] = {
  0x30, 0x82, 0x05, 0xA4, 0x30, 0x82, 0x03, 0x8C, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x13, 0x33,
  0x00, 0x00, 0x00, 0x16, 0x36, 0xBF, 0x36, 0x89, 0x9F, 0x15, 0x75, 0xCC, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x16, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05,
  0x00, 0x30, 0x5A, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53,
  0x31, 0x1E, 0x30, 0x1C, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x15, 0x4D, 0x69, 0x63, 0x72, 0x6F,
  0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E,
  0x31, 0x2B, 0x30, 0x29, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x22, 0x4D, 0x69, 0x63, 0x72, 0x6F,
  0x73, 0x6F, 0x66, 0x74, 0x20, 0x52, 0x53, 0x41, 0x20, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x73,
  0x20, 0x52, 0x6F, 0x6F, 0x74, 0x20, 0x43, 0x41, 0x20, 0x32, 0x30, 0x32, 0x31, 0x30, 0x1E, 0x17,
  0x0D, 0x32, 0x33, 0x30, 0x36, 0x31, 0x33, 0x31, 0x39, 0x32, 0x31, 0x34, 0x37, 0x5A, 0x17, 0x0D,
  0x33, 0x38, 0x30, 0x36, 0x31, 0x33, 0x31, 0x39, 0x33, 0x31, 0x34, 0x37, 0x5A, 0x30, 0x4E, 0x31,
  0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x1E, 0x30, 0x1C,
  0x06, 0x03, 0x55, 0x04, 0x0A, 0x13, 0x15, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74,
  0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x31, 0x1F, 0x30, 0x1D,
  0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x16, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74,
  0x20, 0x55, 0x45, 0x46, 0x49, 0x20, 0x43, 0x41, 0x20, 0x32, 0x30, 0x32, 0x33, 0x30, 0x82, 0x01,
  0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00,
  0x03, 0x82, 0x01, 0x0F, 0x00, 0x30, 0x82, 0x01, 0x0A, 0x02, 0x82, 0x01, 0x01, 0x00, 0xBD, 0x22,
  0x2A, 0xAE, 0xEF, 0x1A, 0x31, 0x85, 0x13, 0x78, 0x51, 0xA7, 0x9B, 0xFD, 0xFC, 0x78, 0xD1, 0x63,
  0xB8, 0x1A, 0x9B, 0x63, 0xF5, 0x12, 0x06, 0xDB, 0x4B, 0x41, 0x35, 0x6A, 0x6F, 0xAB, 0xF5, 0x6A,
  0x04, 0xCC, 0x97, 0xCF, 0xBB, 0xD4, 0x08, 0x09, 0x1A, 0x61, 0x3A, 0x0D, 0xE6, 0xB3, 0xA0, 0x46,
  0xFF, 0x09, 0xAD, 0xDE, 0x80, 0x24, 0xDC, 0x12, 0x80, 0xF2, 0x5F, 0xD9, 0x16, 0xED, 0xE2, 0x42,
  0x9D, 0xCD, 0x2F, 0x4D, 0x61, 0x02, 0x61, 0x8A, 0x1C, 0x4B, 0x1D, 0x18, 0x62, 0x39, 0x86, 0x97,
  0x71, 0xAD, 0x3E, 0x7F, 0x5D, 0x71, 0x13, 0x4B, 0xE9, 0x2A, 0x00, 0xC1, 0xBE, 0xD5, 0xB7, 0x00,
  0x9F, 0x5E, 0x65, 0xB2, 0x2C, 0x1A, 0xFF, 0x74, 0xED, 0xEA, 0x83, 0xD2, 0x39, 0x89, 0x33, 0x35,
  0x73, 0x7D, 0xA0, 0xA2, 0xFA, 0x40, 0xE4, 0x66, 0x50, 0x58, 0xAA, 0xFC, 0x87, 0xE8, 0x5C, 0x20,
  0x83, 0x34, 0xEC, 0xAB, 0xE2, 0x0B, 0xC5, 0x5F, 0x3E, 0xFF, 0x48, 0x2B, 0x11, 0x91, 0x26, 0xEF,
  0x18, 0x6E, 0x57, 0xC5, 0x9F, 0x18, 0x73, 0x99, 0xEF, 0xE1, 0x6A, 0x74, 0x2B, 0xBB, 0x2F, 0x7F,
  0x50, 0x8E, 0x1D, 0xDA, 0x3D, 0x76, 0xB6, 0x04, 0xE5, 0xCC, 0x2E, 0x10, 0xC7, 0x83, 0x1B, 0x83,
  0xA3, 0xE4, 0xA5, 0x13, 0x13, 0x71, 0x6E, 0x33, 0x78, 0xA3, 0xA8, 0x3C, 0xEC, 0x48, 0x26, 0x5E,
  0xC7, 0xC6, 0x5E, 0x0D, 0x87, 0x9A, 0xAA, 0xCC, 0x55, 0x34, 0x81, 0xAD, 0x9D, 0x90, 0xF5, 0xE6,
  0x96, 0x63, 0xA6, 0xE8, 0x07, 0x20, 0x17, 0xC8, 0x93, 0x1E, 0xD2, 0xAE, 0xA4, 0xDC, 0xAE, 0x7D,
  0x59, 0xBF, 0x88, 0x5E, 0x62, 0x0C, 0xAE, 0x5B, 0xF2, 0x29, 0x40, 0x56, 0x1D, 0x26, 0x40, 0xDE,
  0x85, 0xA6, 0xAD, 0x56, 0xD1, 0xCF, 0x55, 0x47, 0x76, 0x5F, 0x9C, 0x39, 0xDB, 0x03, 0x02, 0x03,
  0x01, 0x00, 0x01, 0xA3, 0x82, 0x01, 0x6D, 0x30, 0x82, 0x01, 0x69, 0x30, 0x0E, 0x06, 0x03, 0x55,
  0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x10, 0x06, 0x09, 0x2B,
  0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x15, 0x01, 0x04, 0x03, 0x02, 0x01, 0x00, 0x30, 0x1D, 0x06,
  0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x81, 0xAA, 0x6B, 0x32, 0x44, 0xC9, 0x35, 0xBC,
  0xE0, 0xD6, 0x62, 0x8A, 0xF3, 0x98, 0x27, 0x42, 0x1E, 0x32, 0x49, 0x7D, 0x30, 0x19, 0x06, 0x09,
  0x2B, 0x06, 0x01, 0x04, 0x01, 0x82, 0x37, 0x14, 0x02, 0x04, 0x0C, 0x1E, 0x0A, 0x00, 0x53, 0x00,
  0x75, 0x00, 0x62, 0x00, 0x43, 0x00, 0x41, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01,
  0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04,
  0x18, 0x30, 0x16, 0x80, 0x14, 0x84, 0x44, 0x86, 0x06, 0x00, 0x98, 0x3F, 0x2C, 0xAA, 0xB3, 0xC5,
  0x89, 0xF3, 0xAC, 0x2E, 0xC9, 0xE6, 0x9D, 0x09, 0x03, 0x30, 0x65, 0x06, 0x03, 0x55, 0x1D, 0x1F,
  0x04, 0x5E, 0x30, 0x5C, 0x30, 0x5A, 0xA0, 0x58, 0xA0, 0x56, 0x86, 0x54, 0x68, 0x74, 0x74, 0x70,
  0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x6D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74,
  0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x70, 0x6B, 0x69, 0x6F, 0x70, 0x73, 0x2F, 0x63, 0x72, 0x6C, 0x2F,
  0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x25, 0x32, 0x30, 0x52, 0x53, 0x41, 0x25,
  0x32, 0x30, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x73, 0x25, 0x32, 0x30, 0x52, 0x6F, 0x6F, 0x74,
  0x25, 0x32, 0x30, 0x43, 0x41, 0x25, 0x32, 0x30, 0x32, 0x30, 0x32, 0x31, 0x2E, 0x63, 0x72, 0x6C,
  0x30, 0x72, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x01, 0x01, 0x04, 0x66, 0x30, 0x64,
  0x30, 0x62, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x02, 0x86, 0x56, 0x68, 0x74,
  0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x6D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F,
  0x66, 0x74, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0x70, 0x6B, 0x69, 0x6F, 0x70, 0x73, 0x2F, 0x63, 0x65,
  0x72, 0x74, 0x73, 0x2F, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x25, 0x32, 0x30,
  0x52, 0x53, 0x41, 0x25, 0x32, 0x30, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x73, 0x25, 0x32, 0x30,
  0x52, 0x6F, 0x6F, 0x74, 0x25, 0x32, 0x30, 0x43, 0x41, 0x25, 0x32, 0x30, 0x32, 0x30, 0x32, 0x31,
  0x2E, 0x63, 0x72, 0x74, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01,
  0x0B, 0x05, 0x00, 0x03, 0x82, 0x02, 0x01, 0x00, 0x07, 0x60, 0x13, 0x2A, 0x53, 0x87, 0x12, 0x0F,
  0x1A, 0xF3, 0x5A, 0x14, 0x95, 0x17, 0xE5, 0xD8, 0xD7, 0x95, 0x54, 0x9B, 0x8B, 0x0E, 0xDD, 0x91,
  0xA5, 0xED, 0xC7, 0x5D, 0x47, 0x50, 0x93, 0x45, 0xB7, 0x95, 0x88, 0x5F, 0x17, 0x19, 0x41, 0x63,
  0x76, 0xB5, 0x82, 0xB0, 0xA8, 0xC5, 0x9D, 0x99, 0x15, 0x36, 0x89, 0x49, 0xBE, 0x12, 0xC2, 0x66,
  0xFB, 0x83, 0x0C, 0xB0, 0x81, 0xCE, 0xE5, 0xA4, 0xAB, 0xC2, 0xA0, 0x9A, 0xEB, 0xF5, 0x07, 0x3C,
  0xFE, 0x21, 0xF8, 0x9A, 0xDC, 0x19, 0x21, 0x0C, 0x9E, 0x24, 0x2C, 0xD1, 0x5C, 0xA2, 0x16, 0x0A,
  0x4B, 0xEB, 0xEC, 0x48, 0x9C, 0xB1, 0x5B, 0x74, 0xDB, 0x01, 0x64, 0xC2, 0xE3, 0x80, 0x6A, 0xAB,
  0x1A, 0xCD, 0x77, 0x1B, 0x6A, 0x39, 0x9A, 0xB7, 0xBA, 0x70, 0x44, 0xFF, 0x67, 0x94, 0xC5, 0x81,
  0x06, 0xF0, 0xCB, 0x81, 0x04, 0x93, 0x27, 0x21, 0x99, 0xBD, 0x87, 0x88, 0x14, 0x9C, 0x22, 0x71,
  0x0E, 0x0B, 0x2F, 0x5C, 0xBE, 0xB8, 0x90, 0x54, 0x7C, 0xC0, 0x1E, 0xBC, 0x2B, 0x9B, 0xA3, 0x56,
  0x17, 0x4B, 0x97, 0xE7, 0xE3, 0x7F, 0x13, 0x34, 0xFA, 0xB0, 0x34, 0x6B, 0x9B, 0xF6, 0xB2, 0x2D,
  0xF7, 0xD8, 0x7B, 0xD8, 0x20, 0xD3, 0x5C, 0xA7, 0x95, 0x4C, 0x4F, 0x2A, 0xF9, 0xE7, 0x1E, 0x68,
  0xAF, 0xFC, 0x6C, 0x8F, 0xC8, 0x86, 0x3D, 0x9F, 0xC8, 0xD1, 0xEF, 0x4D, 0x1A, 0xC8, 0xD1, 0xF6,
  0xFD, 0x2D, 0x7C, 0xE3, 0xE8, 0x41, 0xC1, 0xEA, 0x27, 0xC1, 0xFB, 0x8E, 0x25, 0x86, 0x5A, 0x89,
  0xA6, 0x10, 0xBE, 0xCE, 0xE3, 0x8F, 0xA5, 0x7B, 0xC4, 0x1A, 0xA0, 0xE8, 0x75, 0x90, 0xFD, 0x21,
  0xB0, 0xC1, 0xA3, 0xC5, 0x16, 0x23, 0x5E, 0x3C, 0xCE, 0x2F, 0xFE, 0x8C, 0x98, 0xBF, 0x08, 0x5C,
  0xF6, 0xB9, 0xC5, 0xB2, 0x3C, 0xB6, 0xCC, 0xC8, 0xEC, 0x7F, 0xD2, 0x77, 0x74, 0xCB, 0xED, 0xF3,
  0x96, 0xC9, 0x8B, 0x8D, 0x1C, 0x2A, 0x89, 0x0F, 0xA3, 0x8F, 0xBD, 0xCE, 0x2A, 0x85, 0x46, 0x9A,
  0x23, 0xA2, 0x8F, 0x42, 0xC0, 0x99, 0xD6, 0xEA, 0x85, 0x1F, 0x61, 0x19, 0xBE, 0x16, 0x35, 0xB7,
  0x75, 0xA0, 0x95, 0x80, 0x65, 0x06, 0x87, 0xD4, 0x0B, 0x35, 0xC8, 0xC4, 0xAA, 0x0E, 0xCE, 0xA2,
  0x0A, 0x63, 0x60, 0xCA, 0x4B, 0x2B, 0x5C, 0x27, 0x04, 0x82, 0xAF, 0x3E, 0x58, 0x83, 0x7A, 0x5A,
  0xD8, 0x67, 0x3F, 0x10, 0x53, 0xF5, 0x0C, 0x16, 0xF7, 0x26, 0x4B, 0x8A, 0x80, 0xB9, 0xC5, 0x1F,
  0xA0, 0xDE, 0xD8, 0xD3, 0x61, 0x44, 0x14, 0x45, 0xA7, 0xF5, 0xAB, 0x9A, 0x88, 0x17, 0xFD, 0xB7,
  0x94, 0x54, 0x02, 0x8B, 0xE4, 0xB7, 0x53, 0xA1, 0x3E, 0x8D, 0x9E, 0x50, 0x82, 0xA8, 0x00, 0xE0,
  0x78, 0x94, 0x1B, 0xBE, 0xB3, 0xC4, 0x30, 0x1F, 0xB2, 0x0E, 0xDB, 0xF0, 0x46, 0x90, 0xC1, 0xE6,
  0x57, 0xFE, 0x7C, 0xC1, 0x70, 0xB2, 0x1C, 0x4B, 0x64, 0xD9, 0x10, 0x03, 0x1B, 0x34, 0xFB, 0x66,
  0xCF, 0x82, 0x6E, 0x9E, 0x40, 0xA8, 0x11, 0x37, 0xF2, 0x65, 0x8B, 0x21, 0x09, 0xAF, 0x3C, 0x93,
  0x62, 0x3D, 0xF3, 0xBC, 0x83, 0xDD, 0x3F, 0x55, 0x90, 0x15, 0xD2, 0x31, 0xAF, 0x11, 0xE7, 0xF8,
  0xCA, 0xA0, 0x82, 0xE1, 0xB9, 0xCF, 0xB3, 0x57, 0x93, 0xC7, 0x55, 0x37, 0xAC, 0x7F, 0x41, 0xBF,
  0x1F, 0x96, 0x3C, 0xF3, 0x26, 0x94, 0xF9, 0xD8, 0xD2, 0x55, 0x24, 0x8A, 0x8A, 0xB6, 0x41, 0xF0,
  0xE0, 0x16, 0xC0, 0x23, 0x92, 0x8C, 0x71, 0x0A, 0x4C, 0x6A, 0x0D, 0x19, 0x55, 0xF7, 0x3A, 0x9C,
  0x92, 0x21, 0x96, 0xA1, 0xD5, 0xF8, 0x0A, 0x8C, 0x9D, 0xBF, 0xC9, 0xEB, 0xCA, 0x88, 0x42, 0xFC,
  0x4B, 0xB4, 0xEF, 0xFF, 0x27, 0x30, 0x21, 0x61
};

/**
  Worker function that prints an binary data into specified string buffer.

  @param[in]  BinaryBuffer        Pointer to binary buffer
  @param[in]  BinaryBufferSize    Size in bytes of binary buffer
  @param[out] StrBuffer           Pointer to output string buffer
  @param[in]  StrBufferSize       Size in bytes of output string buffer

  @retval EFI_SUCCESS             The binary data was successfully converted to the string buffer.
  @retval EFI_INVALID_PARAMETER   StrBuffer is NULL or StrBufferSize is zero.
**/
STATIC
EFI_STATUS
BinaryToString (
  IN UINT8                             *BinaryBuffer,
  IN UINTN                             BinaryBufferSize,
  IN CHAR16                            *StrBuffer,
  IN UINTN                             StrBufferSize
  )
{
  UINTN                                Index;
  UINTN                                Count;
  UINTN                                LenPerChar;

  if (StrBuffer == NULL || StrBufferSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (StrBuffer, StrBufferSize);
  if (BinaryBuffer == NULL || BinaryBufferSize == 0) {
    return EFI_SUCCESS;
  }

  LenPerChar = 3;
  Count = MIN(StrBufferSize / sizeof (CHAR16) / LenPerChar, BinaryBufferSize);
  for (Index = 0; Index < Count; Index++) {
    UnicodeSPrint (&StrBuffer[Index * LenPerChar], LenPerChar * sizeof (CHAR16), L"%02x", BinaryBuffer[Index]);
    StrBuffer[(Index + 1) * LenPerChar - 1] = ' ';
  }
  if (Index > 1 && BinaryBufferSize > Count) {
    StrBuffer[Index * LenPerChar - 2] = '.';
    StrBuffer[Index * LenPerChar - 3] = '.';
    StrBuffer[Index * LenPerChar - 4] = '.';
  }
  if (Index > 0) {
    StrBuffer[Index * LenPerChar - 1] = CHAR_NULL;
  }

  return EFI_SUCCESS;
}

/**
  Get the string of signature type GUID.

  @param[in]  HiiHandle           HII handle
  @param[in]  SignatureType       Pointer to signature type GUID

  @return The pointer of signature type string or NULL if fail to get string.
**/
STATIC
CHAR16 *
GetSigTypeStr (
  IN EFI_HII_HANDLE                    HiiHandle,
  IN EFI_GUID                          *SignatureType
  )
{
  UINTN                                Index;

  for (Index = 0; Index < sizeof (mSigTypeHiiInfo) / sizeof (SECURE_BOOT_SIGNATURE_TYPE_HII_INFO); Index++) {
    if (CompareGuid (SignatureType, mSigTypeHiiInfo[Index].SignatureType)) {
      return HiiGetString (HiiHandle, mSigTypeHiiInfo[Index].StringId, NULL);
    }
  }

  return HiiGetString (HiiHandle, STRING_TOKEN (STR_SIGNATURE_TYPE_UNKNOWN), NULL);
}

/**
  Get DER Encoded header info by ASN.1 TLV(Type-Length-Value) encoding from DER Encoded buffer.

  @param[in]  DerData             Pointer to DER buffer
  @param[in]  DerDataSize         Size in bytes of DER buffer
  @param[out] EncodedHeaderSize   Return the size of DER Encoded header
  @param[out] EncodedTag          Return the tag value of DER
  @param[out] EncodedValueSize    Return the size of DER Encoded value

  @retval EFI_SUCCESS             Get DER Encoded header info successfully.
  @retval EFI_INVALID_PARAMETER   Input parameter is invalid.
  @retval EFI_NOT_FOUND           DER Encoded header is incorrect.
**/
STATIC
EFI_STATUS
GetDerEncodedHeaderInfo (
  IN  UINT8                            *DerData,
  IN  UINTN                            DerDataSize,
  OUT UINTN                            *EncodedHeaderSize,
  OUT UINT8                            *EncodedTag,
  OUT UINT32                           *EncodedValueSize
  )
{
  UINTN                                Offset;
  UINT8                                Tag;
  UINT32                               Size;
  UINT32                               Length;
  UINT32                               Index;

  if (DerData == NULL || DerDataSize == 0 || EncodedHeaderSize == NULL || EncodedTag == NULL || EncodedValueSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Offset = 0;
  Tag    = DerData[Offset++];
  Length = DerData[Offset++];
  if ((Length & LEN_XTND) != 0) {
    Length &= LEN_MASK;
    if (Length > 4) {
      return EFI_NOT_FOUND;
    }

    Size = 0;
    for (Index = 0; Index < Length; Index++) {
      Size = (Size << 8) | DerData[Offset++];
    }
  } else {
    Size = Length;
  }

  *EncodedHeaderSize = Offset;
  *EncodedTag        = Tag;
  *EncodedValueSize  = Size;
  return EFI_SUCCESS;
}

/**
  Convert UTF8 string to UNICODE string.

  UTF8 Encoding Table
  Bits per Character | Unicode Character Range | Unicode Binary  Encoding | UTF8 Binary Encoding
        0-7          |     0x0000 - 0x007F     |     00000000 0xxxxxxx    |   0xxxxxxx
        8-11         |     0x0080 - 0x07FF     |     00000xxx xxxxxxxx    |   110xxxxx 10xxxxxx
       12-16         |     0x0800 - 0xFFFF     |     xxxxxxxx xxxxxxxx    |   1110xxxx 10xxxxxx 10xxxxxx


  @param[in]  Utf8Str             Pointer to UTF8 string.
  @param[in]  Utf8StrSize         Size of UTF8 string in bytes.
  @param[out] StrBuffer           Pointer to output string buffer
  @param[in]  StrBufferSize       Size in bytes of output string buffer
**/
STATIC
VOID
Utf8StrToUnicodeStr (
  IN   UINT8     *Utf8Str,
  IN   UINTN      Utf8StrSize,
  OUT  CHAR16    *StrBuffer,
  IN   UINTN     StrBufferSize
  )
{
  UINTN               Index;
  UINTN               Utf8Index;
  UINT8               UnicodeByte0;
  UINT8               UnicodeByte1;
  UINT8               Byte0;
  UINT8               Byte1;
  UINT8               Byte2;
  UINTN               Count;

  if (Utf8Str == NULL || StrBuffer == NULL) {
    return;
  }
  Count     = (StrBufferSize / sizeof (CHAR16) - 1);
  Utf8Index = 0;
  for (Index = 0; Index < Count; Index++) {
    if ((Utf8Str[Index] & 0x80) == 0) {
      if (Utf8Index + 1 > Utf8StrSize) {
        break;
      }
      StrBuffer[Index] = Utf8Str[Utf8Index];
      Utf8Index++;
    } else if ((Utf8Str[Index] & 0xE0) == 0xC0) {
      if (Utf8Index + 2 > Utf8StrSize) {
        break;
      }
      Byte0         = Utf8Str[Utf8Index + 1];
      Byte1         = Utf8Str[Utf8Index];

      UnicodeByte0      = (UINT8) ((Byte1 << 6) | (Byte0 & 0x3f));
      UnicodeByte1      = (UINT8) ((Byte1 >> 2) & 0x07);
      StrBuffer[Index]  = (CHAR16) (UnicodeByte0 | (UnicodeByte1 << 8));
      Utf8Index += 2;
    } else if ((Utf8Str[Index] & 0xF0) == 0xE0) {
      if (Utf8Index + 3 > Utf8StrSize) {
        break;
      }
      Byte0         = Utf8Str[Utf8Index + 2];
      Byte1         = Utf8Str[Utf8Index + 1];
      Byte2         = Utf8Str[Utf8Index];

      UnicodeByte0      = (UINT8) ((Byte1 << 6) | (Byte0 & 0x3f));
      UnicodeByte1      = (UINT8) ((Byte2 << 4) | ((Byte1 >> 2) & 0x0f));
      StrBuffer[Index]  = (CHAR16) (UnicodeByte0 | (UnicodeByte1 << 8));
      Utf8Index += 3;
    } else {
      break;
    }
  }
  StrBuffer[Index] = CHAR_NULL;
}

/**
  Get CN(Common Name) from subject buffer.

  @param[in]  Subject             Pointer to subject buffer
  @param[in]  SubjectSize         Size in bytes of subject buffer
  @param[out] StrBuffer           Pointer to output string buffer
  @param[in]  StrBufferSize       Size in bytes of output string buffer

  @retval EFI_SUCCESS             Get CN successfully.
  @retval EFI_INVALID_PARAMETER   Subject is NULL or SubjectSize is zero.
  @retval EFI_NOT_FOUND           CN is not found.
  @retval EFI_ABORTED             DER Encoded header is invalid.
**/
STATIC
EFI_STATUS
GetSubjectCommonName (
  IN  UINT8                            *Subject,
  IN  UINTN                            SubjectSize,
  OUT CHAR16                           *StrBuffer,
  IN  UINTN                            StrBufferSize
  )
{
  EFI_STATUS                           Status;
  UINTN                                HeaderSize;
  UINT8                                Tag;
  UINT32                               ValueSize;
  UINT8                                *DataPtr;
  UINTN                                DataSize;
  UINT8                                *ChildData;
  UINTN                                ChildDataSize;
  UINT8                                *GrandChildData;
  UINTN                                GrandChilddDataSize;
  UINT8                                *Oid;
  UINT32                               OidSize;
  CHAR8                                *AsciiStr;
  UINTN                                Index;
  UINTN                                Count;

  if (Subject == NULL || SubjectSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Parsing method is referenced from CERT_getSubjectCommonNameAux() in BaseCryptLib.
  //
  // now get the child with the COMMON NAME OID 2.5.4.3 */
  //   Name ::= SEQUENCE of RelativeDistinguishedName
  //   RelativeDistinguishedName = SET of AttributeValueAssertion
  //   AttributeValueAssertion = SEQUENCE { attributeType OID; attributeValue ANY }
  //
  /* Name is a sequence */
  Status = GetDerEncodedHeaderInfo (Subject, SubjectSize, &HeaderSize, &Tag, &ValueSize);
  if (EFI_ERROR (Status) || Tag != TAG_TYPE_VALUE_SEQUENCE) {
    return EFI_ABORTED;
  }

  DataPtr  = Subject + HeaderSize;
  DataSize = ValueSize;
  while (DataSize > 0) {
    /* child should be a SET */
    Status = GetDerEncodedHeaderInfo (DataPtr, DataSize, &HeaderSize, &Tag, &ValueSize);
    if (EFI_ERROR (Status) || Tag != TAG_TYPE_VALUE_SET) {
      return EFI_ABORTED;
    }
    ChildData     = DataPtr + HeaderSize;
    ChildDataSize = ValueSize;
    DataPtr       = DataPtr  + (HeaderSize + ChildDataSize);
    DataSize      = DataSize - (HeaderSize + ValueSize);

    /* GrandChild should be a SEQUENCE */
    Status = GetDerEncodedHeaderInfo (ChildData, ChildDataSize, &HeaderSize, &Tag, &ValueSize);
    if (EFI_ERROR (Status) || Tag != TAG_TYPE_VALUE_SEQUENCE) {
      return EFI_ABORTED;
    }
    GrandChildData      = ChildData + HeaderSize;
    GrandChilddDataSize = ValueSize;

    Status = GetDerEncodedHeaderInfo (GrandChildData, GrandChilddDataSize, &HeaderSize, &Tag, &ValueSize);
    if (EFI_ERROR (Status) || Tag != TAG_TYPE_VALUE_OID) {
      return EFI_ABORTED;
    }
    Oid     = GrandChildData + HeaderSize;
    OidSize = ValueSize;
    if (OidSize != sizeof (mSubjectCommonNameOID) ||
        CompareMem (Oid, mSubjectCommonNameOID, OidSize) != 0) {
      continue;
    }

    GrandChildData      += (HeaderSize + ValueSize);
    GrandChilddDataSize -= (HeaderSize + ValueSize);
    Status = GetDerEncodedHeaderInfo (GrandChildData, GrandChilddDataSize, &HeaderSize, &Tag, &ValueSize);
    if (EFI_ERROR (Status) || (Tag != TAG_TYPE_VALUE_PRINTABLESTRING && Tag != TAG_TYPE_VALUE_UTF8STRING)) {
      return EFI_ABORTED;
    }

    if (Tag == TAG_TYPE_VALUE_UTF8STRING) {
      Utf8StrToUnicodeStr (GrandChildData + HeaderSize, ValueSize, StrBuffer, StrBufferSize);
    } else {
      AsciiStr = (CHAR8 *) (GrandChildData + HeaderSize);
      Count    = MIN (ValueSize, (StrBufferSize / sizeof (CHAR16) - 1));
      for (Index = 0; Index < Count; Index++) {
        StrBuffer[Index] = (CHAR16) AsciiStr[Index];
      }
      StrBuffer[Index] = CHAR_NULL;
    }
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**
  Get the display string of PKCS7 signature buffer.
  Display string is determined by subject CN(Common Name).

  @param[in]  SignatureBuffer     Pointer to PKCS7 signature buffer
  @param[in]  SignatureBufferSize Size in bytes of PKCS7 signature buffer
  @param[out] StrBuffer           Pointer to output string buffer
  @param[in]  StrBufferSize       Size in bytes of output string buffer

  @retval EFI_SUCCESS             Get the display string successfully.
  @retval EFI_NOT_FOUND           Subject CN is not found.
  @retval EFI_OUT_OF_RESOURCES    Fail to allocate memory.
  @retval Other                   Fail to get CryptoServicesProtocol instance or get subject CN.
**/
STATIC
EFI_STATUS
GetPkcs7DisplayStr (
  IN  UINT8                            *SignatureBuffer,
  IN  UINTN                            SignatureBufferSize,
  OUT CHAR16                           *StrBuffer,
  IN  UINTN                            StrBufferSize
  )
{
  EFI_STATUS                           Status;
  CRYPTO_SERVICES_PROTOCOL             *CryptoService;
  UINT8                                *Subject;
  UINTN                                SubjectSize;
  UINT16                               *SubjectEx;
  UINTN                                SubjectExSize;
  BOOLEAN                              Success;

  Status = gBS->LocateProtocol (&gCryptoServicesProtocolGuid, NULL, (VOID **) &CryptoService);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Subject     = NULL;
  SubjectSize = 0;
  Success = CryptoService->X509GetSubjectName (
                             SignatureBuffer,
                             SignatureBufferSize,
                             Subject,
                             &SubjectSize
                             );
  if (Success || SubjectSize == 0) {
    Success = CryptoService->X509GetSubjectNameEx (
                               SignatureBuffer,
                               SignatureBufferSize,
                               &SubjectEx,
                               &SubjectExSize
                               );
    if (!Success) {
      return EFI_NOT_FOUND;
    }
    StrBuffer[(SubjectExSize / sizeof (CHAR16)) - 1] = CHAR_NULL;
    CopyMem (StrBuffer, SubjectEx, MIN (StrBufferSize - sizeof (CHAR16), SubjectExSize));
    FreePool (SubjectEx);
    return EFI_SUCCESS;
  }

  Subject = AllocatePool (SubjectSize);
  if (Subject == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Success = CryptoService->X509GetSubjectName (
                             SignatureBuffer,
                             SignatureBufferSize,
                             Subject,
                             &SubjectSize
                             );
  if (!Success) {
    FreePool (Subject);
    return EFI_NOT_FOUND;
  }

  Status = GetSubjectCommonName (Subject, SubjectSize, StrBuffer, StrBufferSize);
  FreePool (Subject);
  return Status;
}

/**
  Get the display string of signature buffer.

  @param[in]  SignatureType       Pointer to signature type GUID
  @param[in]  SignatureBuffer     Pointer to signature buffer
  @param[in]  SignatureBufferSize Size in bytes of signature buffer
  @param[out] StrBuffer           Pointer to output string buffer
  @param[in]  StrBufferSize       Size in bytes of output string buffer

  @retval EFI_SUCCESS             Get the display string successfully.
  @retval Other                   Fail to get PKCS7 display string or binary string.
**/
STATIC
EFI_STATUS
GetSignatureDisplayStr (
  IN  EFI_GUID                         *SignatureType,
  IN  UINT8                            *SignatureBuffer,
  IN  UINT32                           SignatureBufferSize,
  OUT CHAR16                           *StrBuffer,
  IN  UINTN                            StrBufferSize
  )
{
  if (CompareGuid (SignatureType, &gEfiCertX509Guid)) {
    return GetPkcs7DisplayStr (SignatureBuffer, SignatureBufferSize, StrBuffer, StrBufferSize);
  } else {
    return BinaryToString (SignatureBuffer, SignatureBufferSize, StrBuffer, StrBufferSize);
  }
}

/**
  Get the pointer of secure boot variable info by input secure boot manager state.

  @param[in] SecureBootMgrState      Secure boot manager state

  @return The pointer of secure boot variable info or NULL if secure boot manager state is invalid.
**/
STATIC
SECURE_BOOT_VAR_INFO *
GetSecureBootVarInfo (
  IN SECURE_BOOT_MANAGER_STATE         SecureBootMgrState
  )
{
  switch (SecureBootMgrState) {

  case PkForm:
    return &mPkVarInfo;

  case KekForm:
    return &mKekVarInfo;

  case HashImageForm:
  case DbForm:
    return &mDbVarInfo;

  case DbxForm:
    return &mDbxVarInfo;

  case DbtForm:
    return &mDbtVarInfo;

  case DbrForm:
    return &mDbrVarInfo;

  default:
    return NULL;
  }
}

/**
  Check to see if input secure boot variable is modified or not by comparing secure boot variable in NV storage.

  @param[in] VarName                 Pointer to secure boot variable name
  @param[in] VendorGuid              Pointer to secure boot variable vendor GUID
  @param[in] CurrVarData             Pointer to secure boot variable buffer
  @param[in] CurrVarDataSize         Size in bytes of secure boot variable buffer

  @retval TRUE   The input secure boot variable is modified.
  @retval FALSE  The input secure boot variable is not modified.
**/
STATIC
BOOLEAN
IsSecureBootVarModified (
  IN CHAR16                            *VarName,
  IN EFI_GUID                          *VendorGuid,
  IN UINT8                             *CurrVarData,
  IN UINTN                             CurrVarDataSize
  )
{
  EFI_STATUS                           Status;
  UINTN                                OrgVarDataSize;
  UINT8                                *OrgVarData;
  BOOLEAN                              IsModified;

  Status = CommonGetVariableDataAndSize (VarName, VendorGuid, &OrgVarDataSize, (VOID **) &OrgVarData);
  if (EFI_ERROR (Status)) {
    OrgVarData     = NULL;
    OrgVarDataSize = 0;
  }

  if (OrgVarDataSize != CurrVarDataSize) {
    IsModified = TRUE;
  } else if (OrgVarData != NULL && CurrVarData != NULL && CompareMem (OrgVarData, CurrVarData, CurrVarDataSize) != 0) {
    IsModified = TRUE;
  } else {
    IsModified = FALSE;
  }
  SecureBootSafeFreePool ((VOID **) &OrgVarData);

  return IsModified;
}

/**
  Check to see if input signature is existing in input signature list or not.

  @param[in] SigList                 Pointer to signature list
  @param[in] Sig                     Pointer to signature buffer
  @param[in] SigSize                 Size in bytes of signature buffer

  @retval TRUE   The input signature is existing in input signature list.
  @retval FALSE  The input signature is not existing in input signature list.
**/
STATIC
BOOLEAN
IsSignatureExistence (
  IN EFI_SIGNATURE_LIST                *SigList,
  IN EFI_SIGNATURE_DATA                *Sig,
  IN UINTN                             SigSize
  )
{
  UINT8                                *CurrSig;
  UINTN                                SigCount;
  UINTN                                Index;

  if (SigList == NULL || Sig == NULL || SigSize == 0) {
    return FALSE;
  }
  if (SigSize != SigList->SignatureSize) {
    return FALSE;
  }

  CurrSig  = ((UINT8 *) SigList) + sizeof (EFI_SIGNATURE_LIST) + SigList->SignatureHeaderSize;
  SigCount = (SigList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - SigList->SignatureHeaderSize) / SigList->SignatureSize;
  for (Index = 0; Index < SigCount; Index++) {
    if (CompareMem (CurrSig, Sig, SigSize) == 0) {
      return TRUE;
    }
    CurrSig += SigList->SignatureSize;
  }

  return FALSE;
}

/**
  Check to see if input signature is existing in input signature list or not.

  @param[in] SigList                 Pointer to signature list
  @param[in] Sig                     Pointer to signature buffer
  @param[in] SigSize                 Size in bytes of signature buffer

  @retval TRUE   The input signature is existing in input signature list.
  @retval FALSE  The input signature is not existing in input signature list.
**/
STATIC
BOOLEAN
IsShaSignatureType (
  IN EFI_GUID                          *SignatureType
  )
{
  if (CompareGuid (SignatureType, &gEfiCertSha1Guid  ) ||
      CompareGuid (SignatureType, &gEfiCertSha256Guid) ||
      CompareGuid (SignatureType, &gEfiCertSha512Guid)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Internal function to check the input signature buffer is whether a valid X509 certificate.

  @param[in]  SignatureBuffer        Pointer to PKCS7 signature buffer
  @param[in]  SignatureBufferSize    Size in bytes of PKCS7 signature buffer

  @retval TRUE                       Input buffer is a valid X509 certificate.
  @retval FALSE                      Input buffer isn't a valid X509 certificate.
**/
STATIC
BOOLEAN
IsValidCertificate (
  IN  UINT8                            *SignatureBuffer,
  IN  UINTN                            SignatureBufferSize
  )
{
  EFI_STATUS                           Status;
  CRYPTO_SERVICES_PROTOCOL             *CryptoService;
  UINT8                                *Issuer;
  UINTN                                IssuerSize;
  BOOLEAN                              Success;

  Status = gBS->LocateProtocol (&gCryptoServicesProtocolGuid, NULL, (VOID **) &CryptoService);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Issuer     = NULL;
  IssuerSize = 0;
  Success = CryptoService->X509GetIssuerName (
                             SignatureBuffer,
                             SignatureBufferSize,
                             Issuer,
                             &IssuerSize
                             );
  if (Success || IssuerSize == 0) {
    return FALSE;
  }
  return TRUE;
}


/**
  Add signature to specific secure boot variable determined by input secure boot manager state.

  @param[in] SecureBootMgrState      Secure boot manager state
  @param[in] SignatureType           Pointer to signature type GUID
  @param[in] SignatureOwnerGuid      Pointer to signature owner GUID
  @param[in] SignatureData           Pointer to signature data buffer
  @param[in] SignatureDataSize       Size in bytes of signature data buffer
  @param[in] UpdateForm              Need update state to form or not

  @retval EFI_SUCCESS                Add signature successfully.
  @retval EFI_NOT_FOUND              Target secure boot variable info is not found.
  @retval EFI_ABORTED                Reject to add signature because PK is existing.
  @retval EFI_ABORTED                There is an identical signature in target secure boot variable.
  @retval EFI_OUT_OF_RESOURCES       Fail to allocate memory.
**/
EFI_STATUS
SecureBootVarAddSignature (
  IN SECURE_BOOT_MANAGER_STATE         SecureBootMgrState,
  IN EFI_GUID                          *SignatureType,
  IN EFI_GUID                          *SignatureOwnerGuid,
  IN UINT8                             *SignatureData,
  IN UINTN                             SignatureDataSize,
  IN BOOLEAN                           UpdateForm
  )
{
  SECURE_BOOT_VAR_INFO                 *VarInfo;
  EFI_SIGNATURE_DATA                   *Sig;
  UINTN                                SigSize;
  UINTN                                RemainedSize;
  EFI_SIGNATURE_LIST                   *TargetSigList;
  EFI_SIGNATURE_LIST                   *SigList;
  EFI_SIGNATURE_LIST                   *NewSigList;
  UINT8                                *NewVarData;
  UINT8                                *NewVarDataPtr;
  UINTN                                NewVarDataSize;
  UINTN                                Size;

  if (SignatureType == NULL || SignatureOwnerGuid == NULL || SignatureData == NULL || SignatureDataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  VarInfo = GetSecureBootVarInfo (SecureBootMgrState);
  if (VarInfo == NULL) {
    return EFI_NOT_FOUND;
  }

  if (VarInfo == &mPkVarInfo && VarInfo->VarData != NULL) {
    SecureBootShowOkConfirmDlg (STRING_TOKEN (STR_PK_EXIST_ERROR_MESSAGE));
    return EFI_ABORTED;
  }
  if (CompareGuid (SignatureType, &gEfiCertX509Guid)) {
    if (!IsValidCertificate (SignatureData, SignatureDataSize)) {
      SecureBootShowOkConfirmDlg (STRING_TOKEN(STR_FILE_FORMAT_INCORRECT_MESSAGE));
      return EFI_INVALID_PARAMETER;
    }
  }
  //
  // Create signature.
  //
  SigSize = sizeof (EFI_SIGNATURE_DATA) - 1 + SignatureDataSize;
  Sig     = AllocatePool (SigSize);
  if (Sig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyGuid (&Sig->SignatureOwner, SignatureOwnerGuid);
  CopyMem (Sig->SignatureData, SignatureData, SignatureDataSize);

  RemainedSize  = VarInfo->VarDataSize;
  TargetSigList = NULL;
  if (VarInfo->VarData != NULL) {
    SigList = (EFI_SIGNATURE_LIST *) VarInfo->VarData;
    while ((RemainedSize > 0) && (RemainedSize >= SigList->SignatureListSize)) {
      if (SigList->SignatureListSize < sizeof (EFI_SIGNATURE_LIST)) {
        break;
      }

      if (SigList->SignatureSize == SigSize && CompareGuid (&SigList->SignatureType, SignatureType)) {
        if (IsSignatureExistence (SigList, Sig, SigSize)) {
          SecureBootShowOkConfirmDlg (STRING_TOKEN(STR_IDENTICAL_SIGNATURE_ERROR_MESSAGE));
          FreePool (Sig);
          return EFI_ABORTED;
        }

        if (IsShaSignatureType (SignatureType)) {
          TargetSigList = SigList;
          break;
        }
      }

      RemainedSize -= SigList->SignatureListSize;
      SigList = (EFI_SIGNATURE_LIST *) ((UINT8 *) SigList + SigList->SignatureListSize);
    }
  }
  if (TargetSigList == NULL) {
    //
    // Target signature list doesn't exist. Append new signature list in the end of variable.
    //
    NewVarDataSize = VarInfo->VarDataSize + sizeof (EFI_SIGNATURE_LIST) + SigSize;
    if (NewVarDataSize > MAX_VARIABLE_SIZE) {
      SecureBootShowOkConfirmDlg (STRING_TOKEN (STR_FILE_SIZE_TOO_LARGE_MESSAGE));
      return EFI_OUT_OF_RESOURCES;
    }
    NewVarData     = AllocatePool (NewVarDataSize);
    if (NewVarData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    if (VarInfo->VarData != NULL) {
      CopyMem (NewVarData, VarInfo->VarData, VarInfo->VarDataSize);
    }

    NewSigList = (EFI_SIGNATURE_LIST *) (NewVarData + VarInfo->VarDataSize);
    NewSigList->SignatureListSize   = (UINT32) (sizeof (EFI_SIGNATURE_LIST) + SigSize);
    NewSigList->SignatureHeaderSize = 0;
    NewSigList->SignatureSize       = (UINT32) SigSize;
    CopyGuid (
      &NewSigList->SignatureType,
      SignatureType
      );
    CopyMem (
      ((UINT8 *) NewSigList) + sizeof (EFI_SIGNATURE_LIST),
      Sig,
      SigSize
      );
  } else {
    //
    // Target signature list exist. Append new signature in the end of target signature list.
    //
    NewVarDataSize = VarInfo->VarDataSize + SigSize;
    if (NewVarDataSize > MAX_VARIABLE_SIZE) {
      SecureBootShowOkConfirmDlg (STRING_TOKEN (STR_FILE_SIZE_TOO_LARGE_MESSAGE));
      return EFI_OUT_OF_RESOURCES;
    }
    NewVarData     = AllocatePool (NewVarDataSize);
    if (NewVarData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    NewVarDataPtr = NewVarData;
    RemainedSize  = VarInfo->VarDataSize;

    Size = ((UINT8 *) TargetSigList) - VarInfo->VarData;
    CopyMem (NewVarDataPtr, VarInfo->VarData, Size);
    NewVarDataPtr += Size;
    RemainedSize  -= Size;

    NewSigList = (EFI_SIGNATURE_LIST *) NewVarDataPtr;
    Size       = TargetSigList->SignatureListSize;
    CopyMem (NewVarDataPtr, TargetSigList, Size);
    NewVarDataPtr += Size;
    RemainedSize  -= Size;
    CopyMem (NewVarDataPtr, Sig, SigSize);
    NewVarDataPtr += SigSize;
    NewSigList->SignatureListSize += (UINT32) SigSize;

    CopyMem (
      NewVarDataPtr,
      ((UINT8 *) TargetSigList) + TargetSigList->SignatureListSize,
      RemainedSize
      );
  }

  SecureBootSafeFreePool ((VOID **) &VarInfo->VarData);
  VarInfo->VarData     = NewVarData;
  VarInfo->VarDataSize = NewVarDataSize;

  if (UpdateForm) {
    SecureBootVarUpdateForm (mSecureBootPrivate.SecureBootMgrState);
  }

  FreePool (Sig);
  return EFI_SUCCESS;
}

/**
  Delete signature from specific secure boot variable determined by input secure boot manager state.

  @param[in] SecureBootMgrState      Secure boot manager state
  @param[in] SignatureIndex          The index value of signature which is wanted to be deleted

  @retval EFI_SUCCESS                Delete signature successfully.
  @retval EFI_NOT_FOUND              Target secure boot variable info is not found.
  @retval EFI_NOT_FOUND              Signatue is not found because index value is larger than the signature count
                                     in secure boot variable.
**/
EFI_STATUS
SecureBootVarDeleteSignature (
  IN SECURE_BOOT_MANAGER_STATE         SecureBootMgrState,
  IN UINTN                             SignatureIndex
  )
{
  SECURE_BOOT_VAR_INFO                 *VarInfo;
  UINTN                                Index;
  UINTN                                CurrIndex;
  UINTN                                RemainedSize;
  EFI_SIGNATURE_LIST                   *SigList;
  UINT8                                *Sig;
  UINTN                                SigCount;

  VarInfo = GetSecureBootVarInfo (SecureBootMgrState);
  if (VarInfo == NULL) {
    return EFI_NOT_FOUND;
  }

  CurrIndex    = 0;
  RemainedSize = VarInfo->VarDataSize;
  SigList      = (EFI_SIGNATURE_LIST *) VarInfo->VarData;
  while ((RemainedSize > 0) && (RemainedSize >= SigList->SignatureListSize)) {
    if (SigList->SignatureListSize < sizeof (EFI_SIGNATURE_LIST)) {
      break;
    }

    SigCount = (SigList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - SigList->SignatureHeaderSize) / SigList->SignatureSize;
    if (SignatureIndex >= CurrIndex &&
        SignatureIndex  < CurrIndex + SigCount) {
      if (SigCount <= 1) {
        //
        // There is only one signature in the signature list. Delete this signature list.
        //
        VarInfo->VarDataSize -= SigList->SignatureListSize;
        CopyMem (
          SigList,
          ((UINT8 *) SigList) + SigList->SignatureListSize,
          RemainedSize - SigList->SignatureListSize
          );
      } else {
        //
        // There are many signatures in the signature list. Delete this signature in the signature list.
        //
        Sig   = ((UINT8 *) SigList) + sizeof (EFI_SIGNATURE_LIST) + SigList->SignatureHeaderSize;
        Index = SignatureIndex - CurrIndex;
        CopyMem (
          Sig + SigList->SignatureSize * Index,
          Sig + SigList->SignatureSize * (Index + 1),
          SigList->SignatureSize * (SigCount - Index - 1) + (RemainedSize - SigList->SignatureListSize)
          );
        SigList->SignatureListSize -= SigList->SignatureSize;
        VarInfo->VarDataSize       -= SigList->SignatureSize;
      }

      if (VarInfo->VarDataSize == 0) {
        SecureBootSafeFreePool ((VOID **) &VarInfo->VarData);
      }

      SecureBootVarUpdateForm (mSecureBootPrivate.SecureBootMgrState);
      return EFI_SUCCESS;
    }

    CurrIndex += SigCount;
    RemainedSize -= SigList->SignatureListSize;
    SigList = (EFI_SIGNATURE_LIST *) ((UINT8 *) SigList + SigList->SignatureListSize);
  }

  return EFI_NOT_FOUND;
}

/**
  Enumerating all signature from variable data to update signature question list in target VFR form.

  @param[in] VarData                 Pointer to secure boot variable data
  @param[in] VarDataSize             Size in bytes of secure boot variable data
  @param[in] FormId                  Target form ID
  @param[in] LabelStartId            Target start label ID
  @param[in] LabelEndId              Target end label ID
  @param[in] IfrOpcode               Target IFR opcode of signature
  @param[in] QuestionIdBase          Question ID base value if QuestionOpcode is a question opcode.
  @param[in] QuestionIdLen           Available question ID length if QuestionOpcode is a question opcode.

  @retval EFI_SUCCESS                Update signature question list successfully.
  @retval EFI_OUT_OF_RESOURCES       Fail to create opcode or allocate memory.
**/
STATIC
EFI_STATUS
SecureBootVarUpdateQuestionList (
  IN UINT8                             *VarData,
  IN UINTN                             VarDataSize,
  IN EFI_FORM_ID                       FormId,
  IN UINT16                            LabelStartId,
  IN UINT16                            LabelEndId,
  IN UINT8                             IfrOpcode,
  IN EFI_QUESTION_ID                   QuestionIdBase,
  IN EFI_QUESTION_ID                   QuestionIdLen
  )
{
  EFI_STATUS                           Status;
  EFI_HII_HANDLE                       HiiHandle;
  VOID                                 *StartOpCodeHandle;
  VOID                                 *EndOpCodeHandle;
  EFI_IFR_GUID_LABEL                   *StartLabel;
  EFI_IFR_GUID_LABEL                   *EndLabel;
  UINTN                                Index;
  CHAR16                               *SigTypeStr;
  CHAR16                               *SigDataStr;
  UINTN                                SigDataStrSize;
  CHAR16                               *PromptStr;
  UINTN                                PromptStrSize;
  EFI_STRING_ID                        PromptStrId;
  CHAR16                               *HelpStr;
  UINTN                                HelpStrSize;
  EFI_STRING_ID                        HelpStrId;
  CHAR16                               GuidStr[37];
  UINTN                                QuestionIdIndex;
  UINTN                                RemainedSize;
  EFI_SIGNATURE_LIST                   *SigList;
  UINTN                                SigCount;
  EFI_SIGNATURE_DATA                   *Sig;
  CHAR16                               UnknownStr[] = L"Unknown";

  HiiHandle  = mSecureBootPrivate.HiiHandle;
  Status     = EFI_SUCCESS;
  SigDataStr = NULL;
  PromptStr  = NULL;
  HelpStr    = NULL;

  //
  // Initialize the container for dynamic opcodes.
  //
  StartOpCodeHandle = HiiAllocateOpCodeHandle ();
  EndOpCodeHandle   = HiiAllocateOpCodeHandle ();
  if (StartOpCodeHandle == NULL || EndOpCodeHandle == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (StartOpCodeHandle, &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  EndLabel   = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode (EndOpCodeHandle  , &gEfiIfrTianoGuid, NULL, sizeof (EFI_IFR_GUID_LABEL));
  if (StartLabel == NULL || EndLabel == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number       = LabelStartId;
  EndLabel->ExtendOpCode   = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number         = LabelEndId;

  if (VarData == NULL || VarDataSize == 0) {
    goto Exit;
  }

  SigDataStrSize = 100 * sizeof (CHAR16);
  SigDataStr     = AllocateZeroPool (SigDataStrSize);
  PromptStrSize  = SigDataStrSize + 100 * sizeof (CHAR16);
  PromptStr      = AllocateZeroPool (PromptStrSize);
  HelpStrSize    = 100 * sizeof (CHAR16);
  HelpStr        = AllocateZeroPool (HelpStrSize);
  if (PromptStr == NULL || SigDataStr == NULL || HelpStr == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  QuestionIdIndex = 0;
  RemainedSize    = VarDataSize;
  SigList = (EFI_SIGNATURE_LIST *) VarData;

  while ((RemainedSize > 0) && (RemainedSize >= SigList->SignatureListSize)) {
    if (SigList->SignatureListSize < sizeof (EFI_SIGNATURE_LIST)) {
      break;
    }

    SigTypeStr = GetSigTypeStr (HiiHandle, &SigList->SignatureType);
    if (SigTypeStr == NULL) {
      SigTypeStr = UnknownStr;
    }

    Sig      = (EFI_SIGNATURE_DATA *) (((UINT8 *) SigList) + sizeof (EFI_SIGNATURE_LIST) + SigList->SignatureHeaderSize);
    SigCount = (SigList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - SigList->SignatureHeaderSize) / SigList->SignatureSize;
    for (Index = 0; Index < SigCount; Index++, QuestionIdIndex++) {
      Status = GetSignatureDisplayStr (
                 &SigList->SignatureType,
                 Sig->SignatureData,
                 SigList->SignatureSize - OFFSET_OF(EFI_SIGNATURE_DATA, SignatureData),
                 SigDataStr,
                 SigDataStrSize
                 );
      if (EFI_ERROR (Status)) {
        ZeroMem (SigDataStr, SigDataStrSize);
      }

      UnicodeSPrint (PromptStr, PromptStrSize, L"%02d. [%s] %s", QuestionIdIndex + 1, SigTypeStr, SigDataStr);
      PromptStrId = SecureBootGetStrTokenFromDepository (&mSecureBootPrivate, mSecureBootSigOptionStrDepository);
      PromptStrId = HiiSetString (HiiHandle, PromptStrId, PromptStr, NULL);

      SecureBootGuidToStr (&Sig->SignatureOwner, GuidStr, sizeof(GuidStr));
      UnicodeSPrint (HelpStr, HelpStrSize, L"Owner_GUID: %s", GuidStr);
      HelpStrId = SecureBootGetStrTokenFromDepository (&mSecureBootPrivate, mSecureBootSigOptionStrDepository);
      HelpStrId = HiiSetString (HiiHandle, HelpStrId, HelpStr, NULL);

      switch (IfrOpcode) {

      case EFI_IFR_TEXT_OP:
        HiiCreateTextOpCode (
          StartOpCodeHandle,
          PromptStrId,
          HelpStrId,
          0
          );
        break;

      case EFI_IFR_ACTION_OP:
        if (QuestionIdIndex >= QuestionIdLen) {
          break;
        }
        HiiCreateActionOpCode (
          StartOpCodeHandle,
          (EFI_QUESTION_ID) (QuestionIdBase + QuestionIdIndex),
          PromptStrId,
          HelpStrId,
          EFI_IFR_FLAG_CALLBACK,
          0
          );
        break;
      }

      Sig = (EFI_SIGNATURE_DATA *) (((UINT8 *) Sig) + SigList->SignatureSize);
    }

    if (SigTypeStr != UnknownStr) {
      FreePool (SigTypeStr);
    }

    RemainedSize -= SigList->SignatureListSize;
    SigList = (EFI_SIGNATURE_LIST *) ((UINT8 *) SigList + SigList->SignatureListSize);
  }

Exit:
  if (StartOpCodeHandle != NULL && EndOpCodeHandle != NULL) {
    HiiUpdateForm (
      HiiHandle,
      &mSecureBootGuid,
      FormId,
      StartOpCodeHandle,
      EndOpCodeHandle
      );
  }

  if (StartOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (StartOpCodeHandle);
  }
  if (EndOpCodeHandle != NULL) {
    HiiFreeOpCodeHandle (EndOpCodeHandle);
  }
  SecureBootSafeFreePool ((VOID **) &SigDataStr);
  SecureBootSafeFreePool ((VOID **) &PromptStr);
  SecureBootSafeFreePool ((VOID **) &HelpStr);
  return Status;
}

/**
  Update the related forms of specified secure boot variable.

  @param[in] SecureBootMgrState      Secure boot manager state

  @retval EFI_SUCCESS                Update the related forms successfully.
  @retval EFI_NOT_FOUND              Fail to get secure boot vairable info or state is unsupported.
**/
EFI_STATUS
SecureBootVarUpdateForm (
  IN SECURE_BOOT_MANAGER_STATE         SecureBootMgrState
  )
{
  SECURE_BOOT_VAR_INFO                 *VarInfo;
  EFI_FORM_ID                          SigListFormId;
  EFI_FORM_ID                          DeleteSigFormId;

  VarInfo = GetSecureBootVarInfo (SecureBootMgrState);
  if (VarInfo == NULL) {
    return EFI_NOT_FOUND;
  }

  switch (SecureBootMgrState) {

  case PkForm:
    SigListFormId   = FORM_ID_PK;
    DeleteSigFormId = FORM_ID_PK_DELETE_SIGNATURE;
    break;

  case KekForm:
    SigListFormId   = FORM_ID_KEK;
    DeleteSigFormId = FORM_ID_KEK_DELETE_SIGNATURE;
    break;

  case DbForm:
    SigListFormId   = FORM_ID_DB;
    DeleteSigFormId = FORM_ID_DB_DELETE_SIGNATURE;
    break;

  case DbxForm:
    SigListFormId   = FORM_ID_DBX;
    DeleteSigFormId = FORM_ID_DBX_DELETE_SIGNATURE;
    break;

  case DbtForm:
    SigListFormId   = FORM_ID_DBT;
    DeleteSigFormId = FORM_ID_DBT_DELETE_SIGNATURE;
    break;

  case DbrForm:
    SigListFormId   = FORM_ID_DBR;
    DeleteSigFormId = FORM_ID_DBR_DELETE_SIGNATURE;
    break;


  default:
    return EFI_NOT_FOUND;
  }

  SecureBootReclaimStrDepository (mSecureBootSigOptionStrDepository);

  if (SigListFormId != 0) {
    SecureBootVarUpdateQuestionList (
      VarInfo->VarData,
      VarInfo->VarDataSize,
      SigListFormId,
      LABEL_ID_SIGNATURE_LIST_START,
      LABEL_ID_SIGNATURE_LIST_END,
      EFI_IFR_TEXT_OP,
      0,
      0
      );
  }

  if (DeleteSigFormId != 0) {
    SecureBootVarUpdateQuestionList (
      VarInfo->VarData,
      VarInfo->VarDataSize,
      DeleteSigFormId,
      LABEL_ID_DELETE_SIGNATURE_LIST_START,
      LABEL_ID_DELETE_SIGNATURE_LIST_END,
      EFI_IFR_ACTION_OP,
      KEY_DELETE_SIGNATURE_QUESTION_BASE,
      KEY_DELETE_SIGNATURE_QUESTION_LENGTH
      );
  }

  return EFI_SUCCESS;
}

/**
  Update each secure boot variable if it is modified.

  @retval EFI_SUCCESS          Update secure boot variables successfully.
**/
EFI_STATUS
SecureBootVarSaveChange (
  VOID
  )
{
  if (IsSecureBootVarModified (EFI_PLATFORM_KEY_NAME, &gEfiGlobalVariableGuid, mPkVarInfo.VarData, mPkVarInfo.VarDataSize)) {
    UpdatePkVariable (mPkVarInfo.VarData, mPkVarInfo.VarDataSize);
  }

  if (IsSecureBootVarModified (EFI_KEY_EXCHANGE_KEY_NAME, &gEfiGlobalVariableGuid, mKekVarInfo.VarData, mKekVarInfo.VarDataSize)) {
    UpdateKekVariable (mKekVarInfo.VarData, mKekVarInfo.VarDataSize, UPDATE_AUTHENTICATED_VARIABLE);
  }

  if (IsSecureBootVarModified (EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, mDbVarInfo.VarData, mDbVarInfo.VarDataSize)) {
    UpdateDbVariable (mDbVarInfo.VarData, mDbVarInfo.VarDataSize, UPDATE_AUTHENTICATED_VARIABLE);
  }

  if (IsSecureBootVarModified (EFI_IMAGE_SECURITY_DATABASE1, &gEfiImageSecurityDatabaseGuid, mDbxVarInfo.VarData, mDbxVarInfo.VarDataSize)) {
    UpdateDbxVariable (mDbxVarInfo.VarData, mDbxVarInfo.VarDataSize, UPDATE_AUTHENTICATED_VARIABLE);
  }

  if (IsSecureBootVarModified (EFI_IMAGE_SECURITY_DATABASE2, &gEfiImageSecurityDatabaseGuid, mDbtVarInfo.VarData, mDbtVarInfo.VarDataSize)) {
    UpdateDbtVariable (mDbtVarInfo.VarData, mDbtVarInfo.VarDataSize, UPDATE_AUTHENTICATED_VARIABLE);
  }

  if (IsSecureBootVarModified (EFI_IMAGE_SECURITY_DATABASE3, &gEfiImageSecurityDatabaseGuid, mDbrVarInfo.VarData, mDbrVarInfo.VarDataSize)) {
    UpdateDbrVariable (mDbrVarInfo.VarData, mDbrVarInfo.VarDataSize, UPDATE_AUTHENTICATED_VARIABLE);
  }

  return EFI_SUCCESS;
}

/**
  Initialize secure boot variable info.

  @retval EFI_SUCCESS          Initialize secure boot variable info successfully.
**/
EFI_STATUS
SecureBootVarInit (
  VOID
  )
{
  SecureBootSafeFreePool ((VOID **) &mPkVarInfo.VarData);
  SecureBootSafeFreePool ((VOID **) &mKekVarInfo.VarData);
  SecureBootSafeFreePool ((VOID **) &mDbVarInfo.VarData);
  SecureBootSafeFreePool ((VOID **) &mDbxVarInfo.VarData);
  SecureBootSafeFreePool ((VOID **) &mDbtVarInfo.VarData);
  SecureBootSafeFreePool ((VOID **) &mDbrVarInfo.VarData);

  mPkVarInfo.VarDataSize  = 0;
  mKekVarInfo.VarDataSize = 0;
  mDbVarInfo.VarDataSize  = 0;
  mDbxVarInfo.VarDataSize = 0;
  mDbtVarInfo.VarDataSize = 0;
  mDbrVarInfo.VarDataSize = 0;

  CommonGetVariableDataAndSize (EFI_PLATFORM_KEY_NAME       , &gEfiGlobalVariableGuid       , &mPkVarInfo.VarDataSize , (VOID **) &mPkVarInfo.VarData );
  CommonGetVariableDataAndSize (EFI_KEY_EXCHANGE_KEY_NAME   , &gEfiGlobalVariableGuid       , &mKekVarInfo.VarDataSize, (VOID **) &mKekVarInfo.VarData);
  CommonGetVariableDataAndSize (EFI_IMAGE_SECURITY_DATABASE , &gEfiImageSecurityDatabaseGuid, &mDbVarInfo.VarDataSize , (VOID **) &mDbVarInfo.VarData );
  CommonGetVariableDataAndSize (EFI_IMAGE_SECURITY_DATABASE1, &gEfiImageSecurityDatabaseGuid, &mDbxVarInfo.VarDataSize, (VOID **) &mDbxVarInfo.VarData);
  CommonGetVariableDataAndSize (EFI_IMAGE_SECURITY_DATABASE2, &gEfiImageSecurityDatabaseGuid, &mDbtVarInfo.VarDataSize, (VOID **) &mDbtVarInfo.VarData);
  CommonGetVariableDataAndSize (EFI_IMAGE_SECURITY_DATABASE3, &gEfiImageSecurityDatabaseGuid, &mDbrVarInfo.VarDataSize, (VOID **) &mDbrVarInfo.VarData);
  return EFI_SUCCESS;
}

/**
  Helper function to check if input certificate is wheterh in DB.

  @retval TRUE   Input certificate is in DB.
  @retval FALSE  Input certificate is not in DB.
**/
STATIC
BOOLEAN
IsSpecificCertificateInDb (
  IN   UINT8       *CertBuffer,
  IN   UINTN       CertBufferSize
  )
{

  EFI_SIGNATURE_LIST  *CertList;
  EFI_SIGNATURE_DATA  *Cert;
  UINTN               CertSize;
  UINTN               DataSize;


  if (CertBuffer == NULL || CertBufferSize == 0 ||  mDbVarInfo.VarDataSize == 0) {
    return FALSE;
  }
  DataSize = mDbVarInfo.VarDataSize;
  CertList = (EFI_SIGNATURE_LIST *) mDbVarInfo.VarData;
  while ((DataSize > 0) && (DataSize >= CertList->SignatureListSize)) {
    //
    // check the size of singature list size first to make sure singature data integrity
    //
    if (CertList->SignatureListSize  < sizeof (EFI_SIGNATURE_LIST)) {
      break;
    }
    if (CompareGuid (&CertList->SignatureType, &gEfiCertX509Guid)) {
      Cert = (EFI_SIGNATURE_DATA *) ((UINT8 *) CertList + sizeof (EFI_SIGNATURE_LIST) + CertList->SignatureHeaderSize);
      CertSize = CertList->SignatureSize - sizeof(EFI_GUID);
      if (CertSize == CertBufferSize && CompareMem (Cert->SignatureData, CertBuffer, CertSize) == 0) {
        return TRUE;
      }
    }
    DataSize -= CertList->SignatureListSize;
    CertList = (EFI_SIGNATURE_LIST *) ((UINT8 *) CertList + CertList->SignatureListSize);
  }
  return FALSE;
}

/**
  Helper function to check if UEFI CA 2011 is in DB.

  @retval TRUE   UEFI CA 2011 certificate is in DB
  @retval FALSE  UEFI CA 2011 certificate is not in DB
**/
STATIC
BOOLEAN
IsUefiCa2011CertificateInDb (
  VOID
  )
{
  return IsSpecificCertificateInDb (mUefiCa2011Certificate, sizeof (mUefiCa2011Certificate));
}

/**
  Helper function to check if UEFI CA 2023 is in DB.

  @retval TRUE   UEFI CA 2023 certificate is in DB
  @retval FALSE  UEFI CA 2023 certificate is not in DB
**/
STATIC
BOOLEAN
IsUefiCa2023CertificateInDb (
  VOID
  )
{
  return IsSpecificCertificateInDb (mUefiCa2023Certificate, sizeof (mUefiCa2023Certificate));
}

/**
  Helper function to check if all of UEFI CA certificates are all in DB.

  @retval TRUE   All of UEFI CA certificates are all in DB.
  @retval FALSE  At least one UEFI CA certificate is not in DB.
**/
BOOLEAN
IsUefiCaCertificateInDb (
  VOID
  )
{

  if (IsUefiCa2011CertificateInDb () && IsUefiCa2023CertificateInDb ()) {
    return TRUE;
  }
  return FALSE;
}

/**
  Helper function to add UEFI CA certificates to DB.
**/
VOID
AddUefiCaCertificateToDb (
  VOID
  )
{
  if (!IsUefiCa2011CertificateInDb ()) {
    SecureBootVarAddSignature (
      DbForm,
      &gEfiCertX509Guid,
      &mUefiCaCertificateOwnerGuid,
      mUefiCa2011Certificate,
      sizeof (mUefiCa2011Certificate),
      FALSE
      );
  }
  if (!IsUefiCa2023CertificateInDb ()) {
    SecureBootVarAddSignature (
      DbForm,
      &gEfiCertX509Guid,
      &mUefiCaCertificateOwnerGuid,
      mUefiCa2023Certificate,
      sizeof (mUefiCa2023Certificate),
      FALSE
      );
  }
}