/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU SMBIOS functions.
 *
 * Contains code for collecting SMBIOS information
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Ccx
 * @e \$Revision$   @e \$Date$
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Porting.h>
#include <AMD.h>
#include <Filecode.h>
#include <cpuRegisters.h>
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/CcxBaseX86Lib.h>
#include <Library/CcxSmbiosLib.h>
#include <Library/CcxPstatesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/SocZen3ServicesProtocol.h>
#include <CcxRegistersZen3.h>

#define FILECODE CCX_ZEN3_DXE_CCXZEN3SMBIOSDXE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
Zen3GetCoreDmiInfo (
  IN       AMD_CCX_SMBIOS_SERVICES_PROTOCOL       *This,
  IN       UINTN                                   Socket,
     OUT   AMD_CCX_CORE_DMI_INFO                  *CoreDmiInfo
  );

EFI_STATUS
EFIAPI
Zen3GetCacheDmiInfo (
  IN       AMD_CCX_SMBIOS_SERVICES_PROTOCOL       *This,
  IN       UINTN                                   Socket,
     OUT   AMD_CACHE_DMI_INFO                     *CacheDmiInfo
  );

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
STATIC AMD_CCX_SMBIOS_SERVICES_PROTOCOL   mZen3DmiServicesProtocol = {
  SMBIOS_3_2_0,      // Support SMBIOS 3.2.0
  Zen3GetCoreDmiInfo,
  Zen3GetCacheDmiInfo
};

CHAR8 ROMDATA str_ProcManufacturer[] = "Advanced Micro Devices, Inc.";

UINT16 ROMDATA CacheConfigTable[CpuLmaxCache] = {
  CACHE_CFG_L1,
  CACHE_CFG_L2,
  CACHE_CFG_L3
};


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
EFIAPI
CcxZen3SmbiosServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // Install SMBIOS services protocol
  return gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdCcxSmbiosServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mZen3DmiServicesProtocol
                );
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/**
 *  This service retrieves DMI information about the core.
 *
 * @param[in]  This                                 A pointer to the
 *                                                  AMD_CCX_SMBIOS_SERVICES_PROTOCOL instance.
 * @param[in]  Socket                               Zero-based socket number to check.
 * @param[out] CoreDmiInfo                          Contains core DMI information
 *
 * @retval EFI_SUCCESS                              The core DMI information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER                    CoreDmiInfo is NULL.
 *
 **/
EFI_STATUS
EFIAPI
Zen3GetCoreDmiInfo (
  IN       AMD_CCX_SMBIOS_SERVICES_PROTOCOL       *This,
  IN       UINTN                                   Socket,
     OUT   AMD_CCX_CORE_DMI_INFO                  *CoreDmiInfo
  )
{
  UINT64                                  MsrData;
  UINTN                                   Frequency;
  UINTN                                   VoltageInuV;
  UINTN                                   PowerInmW;
  EFI_STATUS                              Status;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;
  AMD_SOC_ZEN3_SERVICES_PROTOCOL         *SocZen3Services;
  UINT32                                  CcdCountFuse;
  UINT32                                  CcdPresentFuse;
  UINT32                                  CoreCountByFuse;
  UINT32                                  CoreDisFuse;
  UINT32                                  Fmax;
  CPUID_DATA                              CpuId;
  AMD_CONFIG_PARAMS                       StdHeader;
  UINT16                                  Index;
  UINT8                                   ByteIndexInUint64;
  BOOLEAN                                 SmtEnByFuse;

  FabricTopology  = NULL;
  SocZen3Services = NULL;
  Status = gBS->LocateProtocol (
                  &gAmdSocZen3ServicesProtocolGuid,
                  NULL,
                  &SocZen3Services
                  );
  ASSERT (!EFI_ERROR (Status));

  Status = gBS->LocateProtocol (
                  &gAmdFabricTopologyServices2ProtocolGuid,
                  NULL,
                  &FabricTopology
                  );
  ASSERT (!EFI_ERROR (Status));

  // Type 4 Offset 0x05, Processor Type
  CoreDmiInfo->ProcessorType = CENTRAL_PROCESSOR;

  // Type 4 Offset 0x06, Processor Family
  CoreDmiInfo->ProcessorFamily = CcxGetProcessorFamilyForSmbios (0, &StdHeader);

  // Type4 Offset 0x08, Processor ID
  AsmCpuid (
      AMD_CPUID_APICID_LPC_BID,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  CoreDmiInfo->ProcessorID.EaxFamilyId = CpuId.EAX_Reg;
  CoreDmiInfo->ProcessorID.EdxFeatureId = CpuId.EDX_Reg;

  // Type4 Offset 0x11/0x16, Voltage/Current Speed
  CcxGetPstateInfo (0, SwPstate0, &Frequency, &VoltageInuV, &PowerInmW, &StdHeader);
  CoreDmiInfo->Voltage = (UINT8) ((VoltageInuV + 50000) / 100000 + 0x80); // Voltage = 0x80 + Voltage * 10
  CoreDmiInfo->CurrentSpeed = (UINT16) Frequency;

  // Type4 Offset 0x14, Max Speed
  CoreDmiInfo->MaxSpeed = (SocZen3Services->GetOpnFmax (SocZen3Services, &Fmax) == EFI_SUCCESS) ? (UINT16) (Fmax * 25) : CoreDmiInfo->CurrentSpeed;

  // Type4 Offset 0x12, External Clock
  CoreDmiInfo->ExternalClock = EXTERNAL_CLOCK_100MHZ;

  // Type4 Offset 0x18, Status
  CoreDmiInfo->Status = SOCKET_POPULATED | CPU_STATUS_ENABLED;

  // Type4 Offset 0x19, Processor Upgrade
  CoreDmiInfo->ProcessorUpgrade = CcxGetProcessorUpgradeForSmbios (0, &StdHeader);

  // Type4 Offset 0x23/0x25, Core Count/Thread Count
  AsmCpuid (
      AMD_CPUID_ASIZE_PCCOUNT,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  if (SocZen3Services->GetOpnCorePresence (SocZen3Services, &CcdPresentFuse, &CoreDisFuse, &SmtEnByFuse) == EFI_SUCCESS) {
    // Get Core Count/Thread Count from FUSE
    CoreCountByFuse = 8;
    while (CoreDisFuse != 0) {
      if ((CoreDisFuse & 1) == 1) {
        CoreCountByFuse--;
      }
      CoreDisFuse >>= 1;
    }

    CcdCountFuse = 0;
    while (CcdPresentFuse != 0) {
      if ((CcdPresentFuse & 1) == 1) {
        CcdCountFuse++;
      }
      CcdPresentFuse >>= 1;
    }

    CoreDmiInfo->CoreCount = (UINT16) (CoreCountByFuse * CcdCountFuse);
    CoreDmiInfo->ThreadCount = (UINT16) CoreDmiInfo->CoreCount << (SmtEnByFuse ? 1 : 0);
  } else {
    // Get Core Count/Thread Count from registers
    CoreDmiInfo->ThreadCount = (UINT16) ((CpuId.ECX_Reg & 0xFF) + 1); // bit 7:0
    CoreDmiInfo->CoreCount = CoreDmiInfo->ThreadCount / CcxGetThreadsPerCore ();
  }

  // Type4 Offset 0x24 Core Enabled
  CoreDmiInfo->CoreEnabled = (UINT16) (((CpuId.ECX_Reg & 0xFF) + 1) / CcxGetThreadsPerCore ());

  // Type4 Offset 0x26, Processor Characteristics
  CoreDmiInfo->ProcessorCharacteristics = 0xFC;

  // Type4 ProcessorVersion
  for (Index = 0; Index <= 5; Index++) {
    MsrData = AsmReadMsr64 ((MSR_CPUID_NAME_STRING0 + Index));
    for (ByteIndexInUint64 = 0; ByteIndexInUint64 <= 7; ByteIndexInUint64++) {
      CoreDmiInfo->ProcessorVersion[Index * 8 + ByteIndexInUint64] = (UINT8) RShiftU64 (MsrData, (8 * ByteIndexInUint64));
    }
  }

  // Type4 ProcessorManufacturer
  ASSERT (PROC_MANU_LENGTH >= sizeof (str_ProcManufacturer));
  LibAmdMemCopy (CoreDmiInfo->ProcessorManufacturer, str_ProcManufacturer, sizeof (str_ProcManufacturer), &StdHeader);

  return EFI_SUCCESS;
}


/**
 * This service retrieves information about the cache.
 *
 * @param[in]  This                                 A pointer to the
 *                                                  AMD_CCX_SMBIOS_SERVICES_PROTOCOL instance.
 * @param[in]  Socket                               Zero-based socket number to check.
 * @param[out] CacheDmiInfo                         Contains cache DMI information
 *
 * @retval EFI_SUCCESS                              The cache DMI information was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER                    CacheDmiInfo is NULL.
 *
 **/
EFI_STATUS
EFIAPI
Zen3GetCacheDmiInfo (
  IN       AMD_CCX_SMBIOS_SERVICES_PROTOCOL       *This,
  IN       UINTN                                   Socket,
     OUT   AMD_CACHE_DMI_INFO                     *CacheDmiInfo
  )
{
  GET_CACHE_INFO     CacheInfo;
  AMD_CACHE_LEVEL    CacheLevel;
  AMD_CONFIG_PARAMS  StdHeader;

  CcxGetCacheInfo (0, &CacheInfo, &StdHeader);

  for (CacheLevel = CpuL1Cache; CacheLevel < CpuLmaxCache; CacheLevel++) {
    // Type7 Offset 0x05, Cache Configuration
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].CacheCfg = CacheConfigTable[CacheLevel];

    // Type7 Offset 0x07 and 09, Maximum Cache Size and Installed Size
    // Maximum size
    if (CacheInfo.CacheEachLevelInfo[CacheLevel].CacheSize > 0xFFFF) {
      CacheDmiInfo->CacheEachLevelInfo[CacheLevel].MaxCacheSize  = 0xFFFF;
      CacheDmiInfo->CacheEachLevelInfo[CacheLevel].MaxCacheSize2 = CacheInfo.CacheEachLevelInfo[CacheLevel].CacheSize;
    } else {
      CacheDmiInfo->CacheEachLevelInfo[CacheLevel].MaxCacheSize  = (UINT16) CacheInfo.CacheEachLevelInfo[CacheLevel].CacheSize;
      CacheDmiInfo->CacheEachLevelInfo[CacheLevel].MaxCacheSize2 = ((CacheInfo.CacheEachLevelInfo[CacheLevel].CacheSize & 0x7FFF) |
                                                                   ((CacheInfo.CacheEachLevelInfo[CacheLevel].CacheSize & 0x8000) << 16));
    }

    // Installed size
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].InstallSize  = CacheDmiInfo->CacheEachLevelInfo[CacheLevel].MaxCacheSize;
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].InstallSize2 = CacheDmiInfo->CacheEachLevelInfo[CacheLevel].MaxCacheSize2;

    // Type7 Offset 0x0B and 0D, Supported SRAM Type and Current SRAM Type
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].SupportedSramType = SRAM_TYPE_PIPELINE_BURST;
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].CurrentSramType   = SRAM_TYPE_PIPELINE_BURST;

    // Type7 Offset 0x0F, Cache Speed
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].CacheSpeed = 1;

    // Type7 Offset 0x10, Error Correction Type
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].ErrorCorrectionType = ERR_CORRECT_TYPE_MULTI_BIT_ECC;

    // Type7 Offset 0x11, System Cache Type
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].SystemCacheType = CACHE_TYPE_UNIFIED;

    // Type7 Offset 0x12, Associativity
    CacheDmiInfo->CacheEachLevelInfo[CacheLevel].Associativity = CacheInfo.CacheEachLevelInfo[CacheLevel].Associativity;
  }

  return EFI_SUCCESS;
}



