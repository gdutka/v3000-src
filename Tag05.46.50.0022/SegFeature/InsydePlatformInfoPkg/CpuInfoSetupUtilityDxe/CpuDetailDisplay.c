/** @file
  CPU Detail Display Function

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "CpuInfoSetupUtilityDxe.h"

extern H2O_CPU_DEVICE_INFO_PROTOCOL  *mCpuDeviceInfoProtocol;

/**
  Update Processor Version string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     ProcessorVersionString   The Processor Version string. Caller should free it after use.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateProcessorVersionString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **ProcessorVersionString
  )
{
  EFI_STATUS                  Status;
  CHAR16                      *String;

  String = NULL;

  Status = mCpuDeviceInfoProtocol->GetProcessorVersionString (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &String
                                     );

  *ProcessorVersionString = String;

  return EFI_SUCCESS;
}

/**
  Update CPU ID string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuIdString              Get CPU ID string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuIdString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuIdString
  )
{
  EFI_STATUS                  Status;
  UINT32                      ProcessorSignature;
  CHAR16                      *String;

  ProcessorSignature = 0;
  String             = NULL;

  Status = mCpuDeviceInfoProtocol->GetCpuId (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &ProcessorSignature
                                     );
  if (!EFI_ERROR (Status)) {
    String = CpuIdStringFormat (ProcessorSignature);
  } else {
    return EFI_NOT_FOUND;
  }

  *CpuIdString = String;

  return EFI_SUCCESS;
}

/**
  Update CPU speed string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuSpeedString           Get CPU speed string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuSpeedString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuSpeedString
  )
{
  EFI_STATUS                  Status;
  UINT16                      CurrentSpeed;
  CHAR16                      *String;

  CurrentSpeed = 0;
  String       = NULL;

  Status = mCpuDeviceInfoProtocol->GetCurrentSpeed (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &CurrentSpeed
                                     );
  if (!EFI_ERROR (Status)) {
    String = CurrentSpeedStringFormat (CurrentSpeed);
  } else {
    return EFI_NOT_FOUND;
  }

  *CpuSpeedString = String;

  return EFI_SUCCESS;
}

/**
  Update CPU TDP string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuTDPString             Get CPU TDP string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuTDPString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuTDPString
  )
{
  EFI_STATUS                  Status;
  UINT32                      CpuTDP;
  CHAR16                      *String;

  CpuTDP = 0;
  String = NULL;

  Status = mCpuDeviceInfoProtocol->GetCpuTDP (
                                     mCpuDeviceInfoProtocol,
                                     &CpuTDP
                                     );
  if (!EFI_ERROR (Status)) {
    String = TDPStringFormat (CpuTDP);
  } else {
    return EFI_NOT_FOUND;
  }

  *CpuTDPString = String;

  return EFI_SUCCESS;
}

/**
  Update CPU voltage string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuVoltageString         Get CPU voltage string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuVoltageString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuVoltageString
  )
{
  EFI_STATUS                  Status;
  UINT8                       Voltage;
  CHAR16                      *String;

  Voltage = 0;
  String  = NULL;

  Status = mCpuDeviceInfoProtocol->GetVoltage (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &Voltage
                                     );
  if (!EFI_ERROR (Status)) {
    String = VoltageStringFormat (Voltage);
  } else {
    return EFI_NOT_FOUND;
  }

  *CpuVoltageString = String;

  return EFI_SUCCESS;
}

/**
  Update CPU stepping string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CpuSteppingString        Get CPU stepping string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCpuSteppingString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CpuSteppingString
  )
{
  EFI_STATUS                  Status;
  UINT32                      CpuStepping;
  CHAR16                      *String;

  CpuStepping = 0;
  String      = NULL;

  Status = mCpuDeviceInfoProtocol->GetCpuStepping (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &CpuStepping
                                     );
  if (!EFI_ERROR (Status)) {
    String = CpuSteppingStringFormat (CpuStepping);
  } else {
    return EFI_NOT_FOUND;
  }

  *CpuSteppingString = String;

  return EFI_SUCCESS;
}

/**
  Update core l1 data cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL1DataCacheString    Get core l1 data cache string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreL1DataCacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL1DataCacheString
  )
{
  EFI_STATUS                  Status;
  UINT32                      CacheSize;
  UINT32                      CacheAssociativity;
  CHAR16                      *String;

  CacheSize          = 0;
  CacheAssociativity = 0;
  String             = NULL;

  Status = mCpuDeviceInfoProtocol->GetCoreL1DataCache (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &CacheSize,
                                     &CacheAssociativity
                                     );
  if (!EFI_ERROR (Status)) {
    String = CoreL1DataCacheStringFormat (CacheSize, CacheAssociativity);
  } else {
    return EFI_NOT_FOUND;
  }

  *CoreL1DataCacheString = String;

  return EFI_SUCCESS;
}

/**
  Update core l1 insruction cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL1InsructionCacheString Get core l1 insruction cache string.

  @retval         EFI_SUCCESS                 Success.
**/
EFI_STATUS
UpdateCoreL1InsructionCacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL1InsructionCacheString
  )
{
  EFI_STATUS                  Status;
  UINT32                      CacheSize;
  UINT32                      CacheAssociativity;
  CHAR16                      *String;

  CacheSize          = 0;
  CacheAssociativity = 0;
  String             = NULL;

  Status = mCpuDeviceInfoProtocol->GetCoreL1InsructionCache (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &CacheSize,
                                     &CacheAssociativity
                                     );
  if (!EFI_ERROR (Status)) {
    String = CoreL1InsructionCacheStringFormat (CacheSize, CacheAssociativity);
  } else {
    return EFI_NOT_FOUND;
  }

  *CoreL1InsructionCacheString = String;

  return EFI_SUCCESS;
}

/**
  Update core l2 cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL2CacheString        Get core l2 cache string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreL2CacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL2CacheString
  )
{
  EFI_STATUS                  Status;
  UINT32                      CacheSize;
  UINT32                      CacheAssociativity;
  CHAR16                      *String;

  CacheSize          = 0;
  CacheAssociativity = 0;
  String             = NULL;

  Status = mCpuDeviceInfoProtocol->GetCoreL2Cache (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &CacheSize,
                                     &CacheAssociativity
                                     );
  if (!EFI_ERROR (Status)) {
    String = CoreL2CacheStringFormat (CacheSize, CacheAssociativity);
  } else {
    return EFI_NOT_FOUND;
  }

  *CoreL2CacheString = String;

  return EFI_SUCCESS;
}

/**
  Update core l3 cache string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreL3CacheString        Get core l3 cache string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreL3CacheString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreL3CacheString
  )
{
  EFI_STATUS                  Status;
  UINT32                      CacheSize;
  UINT32                      CacheAssociativity;
  UINT32                      CacheNumWays;
  CHAR16                      *String;

  CacheSize          = 0;
  CacheAssociativity = 0;
  CacheNumWays       = 0;
  String             = NULL;

  Status = mCpuDeviceInfoProtocol->GetCoreL3Cache (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &CacheSize,
                                     &CacheAssociativity,
                                     &CacheNumWays
                                     );
  if (!EFI_ERROR (Status)) {
    String = CoreL3CacheStringFormat (CacheSize, CacheAssociativity, CacheNumWays);
  } else {
    return EFI_NOT_FOUND;
  }

  *CoreL3CacheString = String;

  return EFI_SUCCESS;
}

/**
  Update core thread num string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     CoreThreadNumString      Get core thread num string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateCoreThreadNumString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **CoreThreadNumString
  )
{
  EFI_STATUS                         Status;
  CHAR16                             *String = NULL;

  Status = mCpuDeviceInfoProtocol->GetCoreThreadNumString (
                                     mCpuDeviceInfoProtocol,
                                     CpuInfoIndex,
                                     &String
                                     );

  *CoreThreadNumString = String;

  return EFI_SUCCESS;
}


/**
  Update microcode version string.

  @param[in]      CpuInfoIndex             Which CPU info index to update.
  @param[out]     MicrocodeVersionString   Get microcode version string.

  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
UpdateMicrocodeVersionString (
  IN     UINTN                CpuInfoIndex,
     OUT CHAR16               **MicrocodeVersionString
  )
{
  EFI_STATUS                         Status;
  UINT64                             MicrocodeVersion;
  CHAR16                             *String;

  String = NULL;

  Status = mCpuDeviceInfoProtocol->GetMicrocodeVersion (
                                     mCpuDeviceInfoProtocol,
                                     &MicrocodeVersion
                                     );
  if (!EFI_ERROR (Status)) {
    String = MicrocodeVersionStringFormat (MicrocodeVersion);
  } else {
    return EFI_NOT_FOUND;
  }

  *MicrocodeVersionString = String;

  return EFI_SUCCESS;
}
