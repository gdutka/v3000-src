/** @file
  CPU Device Info Protocol Dxe

;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "CpuDeviceInfoDxe.h"

/**
  Get the number of SMBIOS type 4 and record.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[out]     NumberOfCpuInfo          The number of SMBIOS type 4 returned in Buffer.
  @param[out]     NumberOfInstallCpu       The number of CPU are installed returned in Buffer.
  @param[out]     SmbiosTableType4s        A pointer to a allocated buffer that contains SmbiosTableType4
                                           It is caller's responsibility to free this buffer.

  @retval         EFI_INVALID_PARAMETER    The Param this is NULL.
  @retval         EFI_NOT_FOUND            Cannot find SMBIOS type 4.
  @retval         EFI_OUT_OF_RESOURCES     Allocated buffer for SmbiosTableType4 fail.
  @retval         EFI_SUCCESS              Get device information successfully.
**/
EFI_STATUS
EFIAPI
GetAllCpuDeviceInfo (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  OUT    UINTN                             *NumberOfCpuInfo,
  OUT    UINTN                             *NumberOfInstallCpu,
  OUT    SMBIOS_TABLE_TYPE4                ***SmbiosTableType4s OPTIONAL
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;
  SMBIOS_TABLE_TYPE4                       **TempSmbiosTableType4s;
  UINTN                                    Index;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The CPU device info protocol is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  TempSmbiosTableType4s = NULL;
  *NumberOfCpuInfo      = 0;
  *NumberOfInstallCpu   = 0;

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (PrivateData->NumberOfCpuInfo == 0) {
    DEBUG ((EFI_D_INFO, "%a() - The CPU Devices(Smbios type4) are not found in system\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  *NumberOfCpuInfo    = PrivateData->NumberOfCpuInfo;
  *NumberOfInstallCpu = PrivateData->NumberOfInstallCpu;

  if (SmbiosTableType4s == NULL) {
    return EFI_SUCCESS;
  }

  TempSmbiosTableType4s = (SMBIOS_TABLE_TYPE4 **) AllocateZeroPool (PrivateData->NumberOfCpuInfo * sizeof (SMBIOS_TABLE_TYPE4 *));
  if (TempSmbiosTableType4s == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate TempSmbiosTableType4s buffer\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < PrivateData->NumberOfCpuInfo; Index++) {
    TempSmbiosTableType4s[Index] = PrivateData->CpuInfo[Index].Processor;
  }

  *SmbiosTableType4s = TempSmbiosTableType4s;

  return EFI_SUCCESS;
}

/**
  Get CPU install status.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.

  @retval         TRUE                     The CPU are installed in slot.
  @retval         FALSE                    The CPU are not installed in slot.
**/
BOOLEAN
EFIAPI
GetCpuInstallStatus (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;

  if (This == NULL) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return FALSE;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);

  return PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled;
}

/**
  Get the Processor Version string.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[out]     ProcessorVersionString   The Processor Version string. Caller should free it after use.

  @retval         EFI_INVALID_PARAMETER    The param This or ProcessorVersionString is NULL.
  @retval         EFI_NOT_FOUND            Cannot find SMBIOS type 4.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get CPU type string successfully.
**/
EFI_STATUS
EFIAPI
GetProcessorVersionString (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT CHAR16                            **ProcessorVersionString
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;

  if ((This == NULL) || (ProcessorVersionString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/ProcessorVersionString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  *ProcessorVersionString = SmbiosGetStringByNumber (
                              (EFI_SMBIOS_TABLE_HEADER *) PrivateData->CpuInfo[CpuInfoIndex].Processor,
                              PrivateData->CpuInfo[CpuInfoIndex].Processor->ProcessorVersion
                              );
  if (*ProcessorVersionString == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to get part number string\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Get CPU ID.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[out]     CpuId                    The contains CPU ID pointer.

  @retval         EFI_INVALID_PARAMETER    The Param This or CpuId is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get CPU ID successfully.
**/
EFI_STATUS
EFIAPI
GetCpuId (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT32                            *CpuId
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;
  UINT32                                   ProcessorSignature;

  if ((This == NULL) || (CpuId == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CpuId is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  CopyMem (
    &ProcessorSignature,
    &(PrivateData->CpuInfo[CpuInfoIndex].Processor->ProcessorId.Signature),
    sizeof (UINT32)
    );

  *CpuId = ProcessorSignature;

  return EFI_SUCCESS;
}

/**
  Get current speed.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[out]     CurrentSpeed             The contains current speed pointer.

  @retval         EFI_INVALID_PARAMETER    The Param This or CurrentSpeed is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get current speed successfully.
**/
EFI_STATUS
EFIAPI
GetCurrentSpeed (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT16                            *CurrentSpeed
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;

  if ((This == NULL) || (CurrentSpeed == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CurrentSpeed is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  *CurrentSpeed = PrivateData->CpuInfo[CpuInfoIndex].Processor->CurrentSpeed;

  return EFI_SUCCESS;
}

/**
  Get voltage.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[out]     Voltage                  The contains voltage pointer.

  @retval         EFI_INVALID_PARAMETER    The Param This or Voltage is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get voltage successfully.
**/
EFI_STATUS
EFIAPI
GetVoltage (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT8                             *Voltage
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;
  PROCESSOR_VOLTAGE_CONVERSION             VoltageConversion;

  if ((This == NULL) || (Voltage == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CpuId is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  VoltageConversion.ProcessorVoltageField = PrivateData->CpuInfo[CpuInfoIndex].Processor->Voltage;
  *Voltage = VoltageConversion.ProcessorVoltage;

  return EFI_SUCCESS;
}

/**
  Get installed cache size.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[in]      CacheLevel               Value that specifies the cache level.
  @param[out]     InstalledSize            Installed cache size.

  @retval         EFI_INVALID_PARAMETER    The Param This or CacheConfiguration is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get cache configuration successfully.
**/
EFI_STATUS
EFIAPI
GetInstalledCacheSize (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
  IN     UINT8                             CacheLevel,
     OUT UINT32                            *InstalledSize
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;
  SMBIOS_TABLE_TYPE7                       *SmbiosTableType7;

  if ((This == NULL) || (InstalledSize == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CacheConfiguration is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  SmbiosTableType7 = PrivateData->CpuInfo[CpuInfoIndex].Cache[CacheLevel];

  if ((SmbiosTableType7 != NULL) && (SmbiosTableType7->InstalledSize != 0)) {
    if (SmbiosTableType7->InstalledSize == 0xFFFF) {
      *InstalledSize = (((SmbiosTableType7->InstalledSize2 & BIT31) == BIT31) ? 64 : 1) *
                         (SmbiosTableType7->InstalledSize2 & ~BIT31);
    } else {
      *InstalledSize = (UINT32) (((SmbiosTableType7->InstalledSize & BIT15) == BIT15) ? 64 : 1) *
                         (SmbiosTableType7->InstalledSize & (UINT16) ~BIT15);
    }
  } else {
    *InstalledSize = 0;
  }

  return EFI_SUCCESS;
}

/**
  Get core thread num string.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[out]     CoreThreadNumString      Get core thread num string.

  @retval         EFI_INVALID_PARAMETER    The Param This or CoreThreadNumString is NULL.
  @retval         EFI_OUT_OF_RESOURCES     Allocate memory failed.
  @retval         EFI_SUCCESS              Success.
**/
EFI_STATUS
EFIAPI
GetCoreThreadNumString (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT CHAR16                            **CoreThreadNumString
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;
  CHAR16                                   TempString[MAX_STR_SIZE] = {0};
  CHAR16                                   *String = NULL;
  UINTN                                    StringSize;
  UINT16                                   EnabledCoreCount;
  UINT16                                   ThreadCount;

  if ((This == NULL) || (CoreThreadNumString == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CoreThreadNumString is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  EnabledCoreCount = PrivateData->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount != 0xFF ? \
                     PrivateData->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount : \
                     PrivateData->CpuInfo[CpuInfoIndex].Processor->EnabledCoreCount2;

  ThreadCount = PrivateData->CpuInfo[CpuInfoIndex].Processor->ThreadCount != 0xFF ? \
                PrivateData->CpuInfo[CpuInfoIndex].Processor->ThreadCount : \
                PrivateData->CpuInfo[CpuInfoIndex].Processor->ThreadCount2;

  UnicodeSPrint (
    TempString,
    MAX_STR_SIZE,
    L"%d Core(s) / %d Thread(s)",
    EnabledCoreCount,
    ThreadCount
    );

  StringSize = StrSize (TempString);
  String = AllocateZeroPool (StringSize);
  if (String == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (String, TempString, StringSize);
  *CoreThreadNumString = String;

  return EFI_SUCCESS;
}

/**
  Get CPU stepping.

  @param[in]      This                     Points to the H2O_CPU_DEVICE_INFO_PROTOCOL.
  @param[in]      CpuInfoIndex             Which CPU info index to get.
  @param[out]     CpuStepping              The contains CPU stepping pointer.

  @retval         EFI_INVALID_PARAMETER    The Param This or CpuStepping is NULL.
  @retval         EFI_NOT_READY            The SmbiosType4 exist but CPU not install.
  @retval         EFI_SUCCESS              Get CPU stepping successfully.
**/
EFI_STATUS
EFIAPI
GetCpuStepping (
  IN     H2O_CPU_DEVICE_INFO_PROTOCOL      *This,
  IN     UINTN                             CpuInfoIndex,
     OUT UINT32                            *CpuStepping
  )
{
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA         *PrivateData;
  UINT32                                   ProcessorSignature;

  if ((This == NULL) || (CpuStepping == NULL)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - The This/CpuStepping is NULL, please check input Param\n",
      __FUNCTION__
      ));
    return EFI_INVALID_PARAMETER;
  }

  PrivateData = H2O_CPU_DEVICE_INFO_PRIVATE_FROM_THIS (This);
  if (!PrivateData->CpuInfo[CpuInfoIndex].CpuInstalled) {
    return EFI_NOT_READY;
  }

  CopyMem (
    &ProcessorSignature,
    &(PrivateData->CpuInfo[CpuInfoIndex].Processor->ProcessorId.Signature),
    sizeof (UINT32)
    );
  *CpuStepping = ProcessorSignature;

  return EFI_SUCCESS;
}

/**
 Initialize CPU Device Info Protocol.

 @param[in] None.
**/
STATIC
EFI_STATUS
InitializeCpuDeviceProtocol (
  VOID
  )
{
  EFI_STATUS                        Status;
  H2O_CPU_DEVICE_INFO_PRIVATE_DATA  *Private;
  EFI_SMBIOS_PROTOCOL               *Smbios;
  EFI_SMBIOS_HANDLE                 SmbiosHandle;
  EFI_SMBIOS_TABLE_HEADER           *Record;
  UINT8                             NumberOfCpuInfo;
  CPU_INFO                          *CpuInfo;
  UINT8                             NumberOfInstallCpu;
  EFI_SMBIOS_HANDLE                 CacheHandle;
  EFI_SMBIOS_TYPE                   SmbiosType;
  SMBIOS_TABLE_TYPE7                *SmbiosTableType7;

  DEBUG ((EFI_D_INFO, "%a() - Start\n", __FUNCTION__));

  NumberOfCpuInfo    = 0;
  NumberOfInstallCpu = 0;
  CpuInfo            = NULL;

  Status = gBS->LocateProtocol (
              &gEfiSmbiosProtocolGuid,
              NULL,
              (VOID **) &Smbios
              );
  DEBUG ((EFI_D_INFO, "LocateProtocol gEfiSmbiosProtocolGuid(%g) - (%r)\n", &gEfiSmbiosProtocolGuid, Status));
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to locate protocol gEfiSmbiosProtocolGuid(%g), Status: %r\n",
      __FUNCTION__,
      gEfiSmbiosProtocolGuid,
      Status
      ));
    return Status;
  }

  Private = (H2O_CPU_DEVICE_INFO_PRIVATE_DATA *) AllocateZeroPool (sizeof (H2O_CPU_DEVICE_INFO_PRIVATE_DATA));
  if (Private == NULL) {
    DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate Private data\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (Record->Type == EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION) {
      CpuInfo = (CPU_INFO *) ReallocatePool (
                               NumberOfCpuInfo * sizeof (CPU_INFO),
                               (NumberOfCpuInfo + 1) * sizeof (CPU_INFO),
                               (VOID *) CpuInfo
                               );
      if (CpuInfo == NULL) {
        DEBUG ((EFI_D_ERROR, "%a() - Fail to allocate CpuInfo buffer\n", __FUNCTION__));
        continue;
      }

      CpuInfo[NumberOfCpuInfo].Processor = (SMBIOS_TABLE_TYPE4 *) Record;
      if ((CpuInfo[NumberOfCpuInfo].Processor->Status & BIT6) == BIT6) { //CPU Socket Populated
        CpuInfo[NumberOfCpuInfo].CpuInstalled = TRUE;
        NumberOfInstallCpu++;
      } else {
        CpuInfo[NumberOfCpuInfo].CpuInstalled = FALSE;
      }

      //
      // Find corresponding cache information
      //
      if (CpuInfo[NumberOfCpuInfo].Processor->L1CacheHandle != 0xFFFF) {
        CacheHandle = CpuInfo[NumberOfCpuInfo].Processor->L1CacheHandle;
        SmbiosType  = SMBIOS_TYPE_CACHE_INFORMATION;
        Status      = LocateSmbiosRecord (&CacheHandle, &SmbiosType, (EFI_SMBIOS_TABLE_HEADER **) &SmbiosTableType7);
        if (!EFI_ERROR (Status)) {
          if ((SmbiosTableType7->CacheConfiguration & B_SMBIOS_TABLE_TYPE7_CACHE_LEVEL) != EfiCacheLevel1) {
            DEBUG ((EFI_D_INFO, "%a() - Cache Information (Handle %4xh) is not a L1 Cache.\n", __FUNCTION__, CacheHandle));
          }
          CpuInfo[NumberOfCpuInfo].Cache[EfiCacheLevel1] = SmbiosTableType7;
        }
      }
      if (CpuInfo[NumberOfCpuInfo].Processor->L2CacheHandle != 0xFFFF) {
        CacheHandle = CpuInfo[NumberOfCpuInfo].Processor->L2CacheHandle;
        SmbiosType  = SMBIOS_TYPE_CACHE_INFORMATION;
        Status      = LocateSmbiosRecord (&CacheHandle, &SmbiosType, (EFI_SMBIOS_TABLE_HEADER **) &SmbiosTableType7);
        if (!EFI_ERROR (Status)) {
          if ((SmbiosTableType7->CacheConfiguration & B_SMBIOS_TABLE_TYPE7_CACHE_LEVEL) != EfiCacheLevel2) {
            DEBUG ((EFI_D_INFO, "%a() - Cache Information (Handle %4xh) is not a L2 Cache.\n", __FUNCTION__, CacheHandle));
          }
          CpuInfo[NumberOfCpuInfo].Cache[EfiCacheLevel2] = SmbiosTableType7;
        }
      }
      if (CpuInfo[NumberOfCpuInfo].Processor->L3CacheHandle != 0xFFFF) {
        CacheHandle = CpuInfo[NumberOfCpuInfo].Processor->L3CacheHandle;
        SmbiosType  = SMBIOS_TYPE_CACHE_INFORMATION;
        Status      = LocateSmbiosRecord (&CacheHandle, &SmbiosType, (EFI_SMBIOS_TABLE_HEADER **) &SmbiosTableType7);
        if (!EFI_ERROR (Status)) {
          if ((SmbiosTableType7->CacheConfiguration & B_SMBIOS_TABLE_TYPE7_CACHE_LEVEL) != EfiCacheLevel3) {
            DEBUG ((EFI_D_INFO, "%a() - Cache Information (Handle %4xh) is not a L3 Cache.\n", __FUNCTION__, CacheHandle));
          }
          CpuInfo[NumberOfCpuInfo].Cache[EfiCacheLevel3] = SmbiosTableType7;
        }
      }

      NumberOfCpuInfo++;
    }
  } while (Status == EFI_SUCCESS);

  Private->Signature = H2O_CPU_DEVICE_INFO_PRIVATE_DATA_SIGNATURE;
  Private->NumberOfCpuInfo = NumberOfCpuInfo;
  Private->CpuInfo = CpuInfo;
  Private->NumberOfInstallCpu = NumberOfInstallCpu;

  //
  // Install protocol
  //
  Private->CpuDeviceInfo.GetAllCpuDeviceInfo             = GetAllCpuDeviceInfo;
  Private->CpuDeviceInfo.GetCpuInstallStatus             = GetCpuInstallStatus;
  Private->CpuDeviceInfo.GetProcessorVersionString       = GetProcessorVersionString;
  Private->CpuDeviceInfo.GetCpuId                        = GetCpuId;
  Private->CpuDeviceInfo.GetCurrentSpeed                 = GetCurrentSpeed;
  Private->CpuDeviceInfo.GetCpuTDP                       = GetCpuTDP;
  Private->CpuDeviceInfo.GetVoltage                      = GetVoltage;
  Private->CpuDeviceInfo.GetCoreL1DataCache              = GetCoreL1DataCache;
  Private->CpuDeviceInfo.GetCoreL1InsructionCache        = GetCoreL1InsructionCache;
  Private->CpuDeviceInfo.GetCoreL2Cache                  = GetCoreL2Cache;
  Private->CpuDeviceInfo.GetCoreL3Cache                  = GetCoreL3Cache;
  Private->CpuDeviceInfo.GetInstalledCacheSize           = GetInstalledCacheSize;
  Private->CpuDeviceInfo.GetCoreThreadNumString          = GetCoreThreadNumString;
  Private->CpuDeviceInfo.GetMicrocodeVersion             = GetMicrocodeVersion;
  Private->CpuDeviceInfo.GetCpuStepping                  = GetCpuStepping;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->Handle,
                  &gH2OCpuDeviceInfoProtocolGuid,
                  &Private->CpuDeviceInfo,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      EFI_D_ERROR,
      "%a() - Fail to InstallMultipleProtocolInterfaces for gH2OCpuDeviceInfoProtocolGuid(%g), Status = %r\n",
      __FUNCTION__,
      gH2OCpuDeviceInfoProtocolGuid,
      Status
      ));
    FreePool (Private);
  }

  DEBUG ((EFI_D_INFO, "%a() - End\n", __FUNCTION__));

  return Status;
}

/**
 Notify Initialize CPU Device Info Protocol.

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
CpuDeviceInfoProtocolInitNotifyFunc (
  IN     EFI_EVENT            Event,
  IN     H2O_CP_HANDLE        Handle
  )
{
  H2OCpUnregisterHandler (Handle);
  InitializeCpuDeviceProtocol ();
}

/**
  The entry point for the driver.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CpuDeviceInfoEntry (
  IN     EFI_HANDLE           ImageHandle,
  IN     EFI_SYSTEM_TABLE     *SystemTable
  )
{
  H2O_CP_HANDLE                       CpHandle;
  EFI_STATUS                          Status;

  if (FeaturePcdGet (PcdH2OBdsCpDisplayBeforeSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpDisplayBeforeGuid,
               CpuDeviceInfoProtocolInitNotifyFunc,
               H2O_CP_MEDIUM,
               &CpHandle
               );
    DEBUG ((EFI_D_INFO, "Checkpoint Registered : %g (%r)\n", &gH2OBdsCpDisplayBeforeGuid, Status));

    return Status;
  }

  return EFI_UNSUPPORTED;
}

