/** @file
  SATA Drive Info Library Definition.

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

#ifndef __SATA_DRIVE_INFO_LIB_H__
#define __SATA_DRIVE_INFO_LIB_H__

#include <Uefi.h>

#include <Protocol/IdeControllerInit.h>

#define MAX_MODEL_NAME_LEN              40
#define MAX_SERIAL_NUMBER_LEN           20

#define B_NOMINAL_FORM_FACTOR_FIELD     (BIT3 | BIT2 | BIT1 | BIT0)

#define B_SMART_SUPPORTED               BIT0  // word 82
#define B_SMART_ENABLED                 BIT0  // word 85

typedef enum {
  AtaNominalFormFactorNotReported    = 0x0,
  AtaNominalFormFactor5Inch25        = 0x1,
  AtaNominalFormFactor3Inch5         = 0x2,
  AtaNominalFormFactor2Inch5         = 0x3,
  AtaNominalFormFactor1Inch8         = 0x4,
  AtaNominalFormFactorLessThan1Inch8 = 0x5,
  AtaNominalFormFactorMiniSata       = 0x6, ///< mSATA
  AtaNominalFormFactorMdot2          = 0x7, ///< M.2
  AtaNominalFormFactorMicroSSD       = 0x8,
  AtaNominalFormFactorCFast          = 0x9,
} ATA_NOMINAL_FORM_FACTOR;

#define ATA_DRIVE_ROTATION_RATE_MINIMUM  0x0401
#define ATA_DRIVE_ROTATION_RATE_MAXIMUM  0xFFFE

typedef enum {
  RotationRateNotReported       = 0,
  RotationRateNonRotatingMedia  = 1,
} ATA_DRIVE_ROTATION_RATE;

/**
  Helper function to retrieve Model Name from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Model Name.

**/
CHAR16 *
RetrieveModelNameFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to retrieve Serial Number from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Model Name.

**/
CHAR16 *
RetrieveSerialNumberFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to retrieve Capacity from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Capacity string.

**/
CHAR16 *
RetrieveCapacityFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to retrieve Form Factor from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Form Factor string.

**/
CHAR16 *
RetrieveFormFactorFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to retrieve Rotation Rate from Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval Rotation Rate string.

**/
CHAR16 *
RetrieveRotationRateFromIdentifyData (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to check if S.M.A.R.T feature set is supported
  by parsing Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval TRUE    S.M.A.R.T feature set is supported by this drive.
  @retval FALSE   S.M.A.R.T feature set is NOT supported by this drive.

**/
BOOLEAN
IsSmartSupported (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to check if S.M.A.R.T feature set is enabled
  by parsing Identify Data.

  @param[in]  IdentifyData  Identify Data.

  @retval TRUE    S.M.A.R.T feature set is enabled on this drive.
  @retval FALSE   S.M.A.R.T feature set is disabled on this drive.

**/
BOOLEAN
IsSmartEnabled (
  IN  EFI_IDENTIFY_DATA   *IdentifyData
  );

/**
  Helper function to get S.M.A.R.T attribute name by ID.

  This is just a sample imeplementation, all informations are from Wikipedia.

  @param[in]  AttributeId   Attribute ID.

  @retval Attribute name string.

**/
CHAR16 *
GetSmartAttributeNameById (
  IN  UINT8   AttributeId
  );

#endif