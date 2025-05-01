/** @file
 PEI Chipset Services Library.

 This file contains only one function that is PeiCsSvcStage2MemoryDiscoverCallback().
 The function PeiCsSvcStage2MemoryDiscoverCallback() use chipset services to install
 Firmware Volume Hob's once there is main memory.

***************************************************************************
* Copyright (c) 2016 - 2021, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <PiPei.h>
#include <ChipsetSetupConfig.h>
//
// Libraries
//
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PciExpressLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/FlashRegionLib.h>
#include <Library/MtrrLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/AmdFtpmPpi.h>
#include <Ppi/FirmwareVolumeInfo.h>


//
// Consume Ppi
//
#include <Ppi/PlatformMemoryRange.h>

#include <AGESA.h>
#include <Hudson-2Reg.h>

EFI_STATUS
CheckPspFtpmFactoryReset (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_PEI_NOTIFY_DESCRIPTOR   mFtpmFactoryResetPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFtpmFactoryResetPpiGuid,
  CheckPspFtpmFactoryReset
};

typedef struct _PEI_SET_MEMORY_RANGE{
  UINTN                         Address;
  UINTN                         Length;
  MTRR_MEMORY_CACHE_TYPE        Type;
} PEI_SET_MEMORY_RANGE;

PEI_SET_MEMORY_RANGE    mPeiSetMemory[] = {
  {0x00000,  0xA0000,  CacheWriteBack},
  {0xA0000,  0x20000,  CacheUncacheable},
  {0xC0000,  0x40000,  CacheWriteThrough},
};

typedef struct _MEM_MAP {
  EFI_PHYSICAL_ADDRESS          BaseAddress;
  UINT64                        Length;
} MEM_MAP;

/**
 Retrieve the base address for the SPI ROM controller
 for LPC Bridge Configuration Registers.

 @param [in]        None

 @retval            EFI_PHYSICAL_ADDRESS The base address for the SPI ROM controller
*/
EFI_PHYSICAL_ADDRESS
GetFchSpiBar (
  VOID
  )
{
  EFI_PHYSICAL_ADDRESS  SpiBar;

  SpiBar = PciExpressRead32 (PCI_EXPRESS_LIB_ADDRESS (FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, FCH_LPC_REGA0));
  SpiBar &= 0x00000000FFFFFF00; // Raven: Bit[31:8]: Spi_eSpi_BaseAddr.

  return SpiBar;
}

/**
 Turn off system if needed

 @param [in]        PeiServices         Describes the list of possible PEI Services
 @param [in]        CpuIo               A pointer to CPU IO Services PPI.
 @param [in]        StartMemoryAddr     Start Address of memory which wants to cache
 @param [in]        MemoryLength        Memory length want to be cache

 @retval            None
*/
VOID
CheckPowerOffNow (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_CPU_IO_PPI           *CpuIo
  )
{
  UINT16  PmBase;
  UINT16  Pm1Sts;

  PmBase = MmioRead16 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG60);
  //
  // Read the ACPI registers
  //
  Pm1Sts = IoRead16 (PmBase + R_FCH_ACPI_PM1_STATUS);
  if ((Pm1Sts & B_PWR_BTN_STATUS) == B_PWR_BTN_STATUS) {
    IoWrite16 (PmBase + R_FCH_ACPI_PM1_STATUS, B_PWR_BTN_STATUS);
    IoWrite16 (PmBase + R_FCH_ACPI_PM_CONTROL, V_SLP_TYPE_S5);
    IoWrite16 (PmBase + R_FCH_ACPI_PM_CONTROL, V_SLP_TYPE_S5 + V_ACPI_SLP_EN);
  }
}

/**
 Set Variable Mtrr by StartMemoryAddr and MemoryLength

 @param [in]        PeiServices         Describes the list of possible PEI Services
 @param [in]        CachePpi            A pointer to Cache Services PPI
 @param [in]        StartMemoryAddr     Start Address of memory which wants to cache
 @param [in]        MemoryLength        Memory length want to be cache

 @retval            None
*/
VOID
SetVariableMtrr (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN       UINT64                       StartMemoryAddr,
  IN       UINT64                       MemoryLength,
  IN       MTRR_MEMORY_CACHE_TYPE       MemoryCacheType
  )
{
  UINT64 Memory1, Memory2, Memory3, Memory4;

  Memory1 = Memory2 = Memory3 = Memory4 = 0;
  Memory1 = MemoryLength;
  Memory2 = StartMemoryAddr;
  do {
    Memory3 = GetPowerOfTwo64 (Memory1);
    //
    // Set Cache.
    //
    MtrrSetMemoryAttribute (Memory2, Memory3, MemoryCacheType);
    Memory4 = Memory1 - Memory3;
    Memory2 += Memory3;
    Memory1 = Memory4;
  } while (Memory1 > 0);
}


/**
 Check whether gAmdFtpmFactoryResetPpiGuid PPI is installed or not

 @param [in]        PeiServices         Describes the list of possible PEI Services

 @retval            EFI_SUCCESS         gAmdFtpmFactoryResetPpiGuid have been installed
*/
EFI_STATUS
CheckPspFtpmFactoryReset (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi  
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *VariableServices;
  UINTN                                 VariableSize;
  VOID                                  *SystemConfiguration;
  EFI_GUID                              SystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
  AMD_FTPM_FACTORY_RESET_PPI            *AmdPspFtpmFactoryResetPpi;

  //
  // By default set PcdAmdPspFactoryReset to FALSE, set it to TRUE only
  // when both setup option is enabled and gAmdFtpmFactoryResetPpiGuid is published
  //
  Status = PcdSetBoolS (PcdAmdPspFactoryReset, FALSE);
  ASSERT_EFI_ERROR (Status);

  VariableServices = NULL;
  //
  // Locate Variable Ppi
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Get Setup Variable
  //
  VariableSize = sizeof(CHIPSET_CONFIGURATION);
  SystemConfiguration = AllocateZeroPool (VariableSize);
  if (SystemConfiguration == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &SystemConfigurationGuid,
                               NULL,
                               &VariableSize,
                               SystemConfiguration
                               );

  if (EFI_ERROR (Status)) {
    return Status;
  }



  if(((CHIPSET_CONFIGURATION *)SystemConfiguration)->ErasefTPMNV != 1) {
  	return EFI_NOT_FOUND;
  }
  DEBUG ((EFI_D_INFO, "ErasefTPMNV of SETUP iS enable\n"));

  Status = (**PeiServices).LocatePpi (
                             (CONST EFI_PEI_SERVICES**)PeiServices,
                             &gAmdFtpmFactoryResetPpiGuid,
                             0,
                             NULL,
                             (VOID**)&AmdPspFtpmFactoryResetPpi
                             );
  if (EFI_ERROR (Status)) {
   return Status;
  }

  DEBUG ((EFI_D_INFO, "gAmdFtpmFactoryResetPpiGuid is install, PcdAmdPspFactoryReset will be set TRUE\n"));
  Status = PcdSetBoolS (PcdAmdPspFactoryReset, TRUE);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;

}

/**
  Internal function to build FV HOB and verify the FV is whether valid according to information
  in FDM.

  @param[in] RegionType      EFI_GUID that specifies the type of region that N-th entry is.
  @param[in] Instance        Unsigned integer that specifies entry instance of FDM

**/
STATIC
VOID
BuildVerifiedFvHob (
  CONST IN  EFI_GUID  *RegionType,
  CONST IN  UINT8     Instance
  )
{
  EFI_PHYSICAL_ADDRESS                  MemoryBaseAddr;
  EFI_FIRMWARE_VOLUME_HEADER            *FvHeader;
  UINT64                                FvAddr;
  UINT64                                FvSize;
  UINTN                                 FvInfoInstance;
  EFI_STATUS                            Status;
  BOOLEAN                               IsFvInoPpiHasInstalled;
  EFI_PEI_FIRMWARE_VOLUME_INFO_PPI      *FvInfoPpi;

  if (FeaturePcdGet (PcdH2OFdmChainOfTrustSupported)) {
    Status = FdmVerifyNAt (RegionType, Instance);
    if (EFI_ERROR(Status)) {
      return;
    }
  }

  FvInfoInstance = 0;
  IsFvInoPpiHasInstalled = FALSE;
  FvAddr = FdmGetNAtAddr (RegionType, Instance);
  if (FvAddr == 0) {
    return;
  }

  FvSize = FdmGetNAtSize (RegionType, Instance);

  while (TRUE){
    Status = PeiServicesLocatePpi (&gEfiPeiFirmwareVolumeInfoPpiGuid, FvInfoInstance, NULL, (VOID**)&FvInfoPpi);
    if (EFI_ERROR(Status)){
      break;
    }

    if ((FvAddr == (UINT64)(UINTN) FvInfoPpi->FvInfo) && (FvInfoPpi->FvInfoSize == (UINT32) FvSize)){
      IsFvInoPpiHasInstalled = TRUE;
      break;
    }
    FvInfoInstance++;
  }

  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES ((UINT32)FvSize),
             &MemoryBaseAddr
             );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {
    CopyMem (
      (UINT8*)(UINTN)MemoryBaseAddr,
      (UINT8*)(UINTN)FvAddr,
      (UINTN)FvSize
    );
    BuildFvHob (
      (UINTN)MemoryBaseAddr,
      FvSize
    );

  if (!IsFvInoPpiHasInstalled){
    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER*)(UINTN) FvAddr;
    PeiServicesInstallFvInfoPpi (
      &FvHeader->FileSystemGuid,
      (VOID*) (UINTN) MemoryBaseAddr,
      (UINT32) FvSize,
      NULL,
      NULL
      );
  }

  } else {
    //
    // Allocate memory fail, so update Hob info from Fdm.
    //
    BuildFvHob (
      (EFI_PHYSICAL_ADDRESS) FvAddr,
      FvSize
      );

    if (!IsFvInoPpiHasInstalled) {
      FvHeader = (EFI_FIRMWARE_VOLUME_HEADER*)(UINTN) FvAddr;
      PeiServicesInstallFvInfoPpi (
        &FvHeader->FileSystemGuid,
        (VOID*) (UINTN) FvAddr,
        (UINT32) FvSize,
        NULL,
        NULL
        );
    }
  }

}

/**
 Install Firmware Volume Hob's once there is main memory

 @param [in]        BootMode            A pointer to boot mode status

 @retval            EFI_SUCCESS         if the interface could be successfully installed
*/
EFI_STATUS
Stage2MemoryDiscoverCallback (
  IN       EFI_BOOT_MODE                *BootMode
  )
{
  EFI_STATUS                            Status;
  CONST EFI_PEI_SERVICES                **PeiServices;
  PEI_PLATFORM_MEMORY_RANGE_PPI         *MemoryRangePpi;
  PEI_MEMORY_RANGE_GRAPHICS_MEMORY      GraphicsMemoryMask;
  PEI_MEMORY_RANGE_PCI_MEMORY           PciMemoryMask;
  PEI_MEMORY_RANGE_OPTION_ROM           OptionRomMask;
  PEI_MEMORY_RANGE_SMRAM                SmramMask;
  EFI_PEI_HOB_POINTERS                  Hob;
  UINT64                                MemoryLength;
  UINT64                                MaxMemoryLength;
  UINT64                                RemapMemoryLength;
  UINT64                                RemapMemoryBase;
  UINT64                                TSegmentSize;
  UINT32                                CC6SaveEn;
  UINT16                                Index;
  MEM_MAP                               MmioMap[6];
  UINT8                                 TpmType;
  MTRR_SETTINGS                         ZeroMtrr;

  PeiServices = GetPeiServicesTablePointer ();

  if (*BootMode != BOOT_ON_S3_RESUME) {
    CheckPowerOffNow (PeiServices, (**PeiServices).CpuIo);
  }

  //
  // reset all Mtrrs to 0 include fixed MTRR and variable MTRR
  //
  ZeroMem (&ZeroMtrr, sizeof (MTRR_SETTINGS));
  //
  // Clear the Cache as Ram (CAR) settings.
  //
  MtrrSetAllMtrrs (&ZeroMtrr);

  //
  // Mtrr : Bios Rom(Set Cache).
  //
  MtrrSetMemoryAttribute (
    (FixedPcdGet32 (PcdFlashAreaBaseAddress) < 0xFF000000) ? 0xFF000000 : FixedPcdGet32 (PcdFlashAreaBaseAddress),    //FixedPcdGet32 (PcdFlashAreaBaseAddress),
    (FixedPcdGet32 (PcdFlashAreaSize) > 0x1000000) ? 0x1000000 : FixedPcdGet32 (PcdFlashAreaSize),    //FixedPcdGet32 (PcdFlashAreaSize),
    CacheWriteProtected
    );

  //
  // Set to 1 MB. Since 1MB cacheability will always be set
  // until override by CSM
  //
  MemoryLength      = SIZE_1MB;
  RemapMemoryLength = 0;
  RemapMemoryBase   = BASE_4GB;
  Status = (*PeiServices)->GetHobList (PeiServices, (VOID**)&Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
        //
        // Skip the memory region below 1MB
        //
        if ((Hob.ResourceDescriptor->PhysicalStart >= SIZE_1MB) &&
            (Hob.ResourceDescriptor->PhysicalStart < SIZE_4GB)) {
          MemoryLength += Hob.ResourceDescriptor->ResourceLength;
        } else if (Hob.ResourceDescriptor->PhysicalStart >= SIZE_4GB) {
          if (Hob.ResourceDescriptor->PhysicalStart < RemapMemoryBase) {
            RemapMemoryBase = Hob.ResourceDescriptor->PhysicalStart;
          }
          RemapMemoryLength += Hob.ResourceDescriptor->ResourceLength;
        }
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }

  MaxMemoryLength = MemoryLength;
  TSegmentSize    = 0;

  //
  // Get platform memory range service
  //
  Status = (**PeiServices).LocatePpi (
                               PeiServices,
                               &gPeiPlatformMemoryRangePpiGuid,
                               0,
                               NULL,
                               (VOID**)&MemoryRangePpi
                               );
  ASSERT_EFI_ERROR (Status);

  Status = MemoryRangePpi->ChooseRanges (
                               (EFI_PEI_SERVICES**)PeiServices,
                               MemoryRangePpi,
                               &OptionRomMask,
                               &SmramMask,
                               &GraphicsMemoryMask,
                               &PciMemoryMask
                               );

  if (SmramMask & PEI_MR_SMRAM_TSEG_MASK) {
    TSegmentSize = ((SmramMask & PEI_MR_SMRAM_SIZE_MASK & (~(1 << 0))) * 128 * 1024);
  }
  DEBUG ((DEBUG_INFO, " RemapMemoryLength = 0x%LX\n", (UINT64)RemapMemoryLength));
  DEBUG ((DEBUG_INFO, " MaxMemoryLength = 0x%LX\n", (UINT64)MaxMemoryLength));
  DEBUG ((DEBUG_INFO, " TSegmentSize = 0x%LX\n", (UINT64)TSegmentSize));
  if (RemapMemoryLength == 0) { // No memory above 4G
    DEBUG ((DEBUG_INFO, " No memory above 4G\n"));
    CC6SaveEn = PciExpressRead32 (PCI_EXPRESS_LIB_ADDRESS (0, 0x18, 0x02, 0x118));
    DEBUG ((DEBUG_INFO, " CC6SaveEn = 0x%X\n", CC6SaveEn));

    if (CC6SaveEn & BIT18) { // C6 is enabled
      DEBUG ((DEBUG_INFO, " C6 is enabled\n"));
        //
        //  _2GB_Physical DRAM______________________________
        //  |         C6 stroage region (16MB)             |
        //  |TOM___________________________________________|
        //  |                                              |
        //  |         UMA               (512MB)            |
        //  |                                              |<-- MaxMemoryLength + TSegmentSize + MEMORY_SIZE_16M, under this address set to WB
        //  |______________________________________________|<-- This 16MB region will be set to WB, but UMA must be UC
        //  |         T-seg             (TSegmentSize)     |
        //  |_____________________________MaxMemoryLength__|
        //  |                                              |
        //  |         System memory                        |
        //  |                                              |
        //  |                                              |
        //  |                                              |
        //
        // C6 enabled and C6 storage region address is under 4G
        //
      SetVariableMtrr (PeiServices, 0, (MaxMemoryLength + TSegmentSize + SIZE_16MB), CacheWriteBack);
      //
      // Set overlap UMA region to UC
      //
      SetVariableMtrr (PeiServices, MaxMemoryLength + TSegmentSize, SIZE_16MB, CacheUncacheable);
    } else {
      DEBUG ((DEBUG_INFO, " C6 is disabled\n"));
      //
      // C6 disable when memory less than 4G
      //
      SetVariableMtrr (PeiServices, 0, (MaxMemoryLength + TSegmentSize), CacheWriteBack);
    }
  } else {
    DEBUG ((DEBUG_INFO, " Memory is Over than 4G\n"));
    //
    // Memory is Over than 4G
    //
    SetVariableMtrr (PeiServices, 0, (MaxMemoryLength + TSegmentSize), CacheWriteBack);
  }

  //
  // MTRR : Over 4G
  //
  if (RemapMemoryLength != 0) {
    //
    // Set AMD MSR SYS_CFG (0xC0010010) Bit 22 Tom2ForceMemTypeWB to 1 to instead of Variable MTRR setting
    //
    UINT64      MsrData;

    MsrData = AsmReadMsr64 (SYS_CFG);
    MsrData |= BIT22;
    AsmWriteMsr64 (SYS_CFG, MsrData);
  }

  //
  // MTRR : Below 1M
  //
  for (Index = 0; Index < (sizeof(mPeiSetMemory)/sizeof(PEI_SET_MEMORY_RANGE)); Index++) {
    //
	  // Set Cache.
	  //
    MtrrSetMemoryAttribute (mPeiSetMemory[Index].Address, mPeiSetMemory[Index].Length, mPeiSetMemory[Index].Type);
  }

  //
  // Install resource descriptor hobs for each Mmio range.
  //
  MmioMap [0].BaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
  MmioMap [0].Length      = PcdGet32 (PcdH2OPciExpressSize);
  MmioMap [1].BaseAddress = PcdGet32 (PcdIoApicBaseAddress);
  MmioMap [1].Length      = SIZE_8KB;                   // For both of EFI_IO_APIC_ADDRESS & EFI_IO_APIC_2_ADDRESS
  MmioMap [2].BaseAddress = PcdGet32 (PcdCpuLocalApicBaseAddress);
  MmioMap [2].Length      = SIZE_4KB;
  MmioMap [3].BaseAddress = ACPI_MMIO_BASE;
  MmioMap [3].Length      = SIZE_8KB;
  MmioMap [4].BaseAddress = GetFchSpiBar ();
  MmioMap [4].Length      = SIZE_4KB;
  MmioMap [5].BaseAddress = GetFchSpiBar () + 0x10000;  // For eSPI
  MmioMap [5].Length      = SIZE_4KB;                   // For eSPI

  for (Index = 0; Index < (sizeof(MmioMap)/(sizeof(MEM_MAP))); Index++) {
    DEBUG ((DEBUG_INFO, " BuildResourceDescriptorHob EFI_RESOURCE_MEMORY_MAPPED_IO from 0x%LX to 0x%LX\n", (UINT64)MmioMap[Index].BaseAddress, (UINT64)(MmioMap[Index].BaseAddress + MmioMap[Index].Length - 1)));
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_MAPPED_IO,
      (EFI_RESOURCE_ATTRIBUTE_PRESENT     |
       EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
       EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
      MmioMap[Index].BaseAddress,
      MmioMap[Index].Length
      );

    DEBUG ((DEBUG_INFO, " BuildMemoryAllocationHob EfiMemoryMappedIO from 0x%LX to 0x%LX\n", (UINT64)MmioMap[Index].BaseAddress, (UINT64)(MmioMap[Index].BaseAddress + MmioMap[Index].Length - 1)));
    BuildMemoryAllocationHob (
      MmioMap[Index].BaseAddress,
      MmioMap[Index].Length,
      EfiMemoryMappedIO
      );
  }

  DEBUG ((EFI_D_ERROR, "Reserved memory for BFV\n"));
  DEBUG ((DEBUG_INFO, " BuildMemoryAllocationHob EfiReservedMemoryType from 0x%LX to 0x%LX\n", (UINT64)PcdGet32(PcdH2OBootFirmwareVolumeBase), (UINT64)(PcdGet32(PcdH2OBootFirmwareVolumeBase) + PcdGet32(PcdH2OBootFirmwareVolumeSize) - 1)));
  BuildMemoryAllocationHob (
    PcdGet32(PcdH2OBootFirmwareVolumeBase),
    PcdGet32(PcdH2OBootFirmwareVolumeSize),
    EfiReservedMemoryType
    );

  // If TPM is enabled, copy FV_MAIN from SPI to memory before measurement
  TpmType = PcdGet8(PcdH2OTpmType);
  if (TpmType != 0) {
    if (*BootMode != BOOT_ON_S3_RESUME && *BootMode != BOOT_IN_RECOVERY_MODE) {
      for (Index = 1; FdmGetNAtSize (&gH2OFlashMapRegionFvGuid, (UINT8)Index) != 0; Index++) {
        BuildVerifiedFvHob (&gH2OFlashMapRegionFvGuid, (UINT8)Index);
      }
    }
  }

  Status = (**PeiServices).NotifyPpi (PeiServices, &mFtpmFactoryResetPpiCallback);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
