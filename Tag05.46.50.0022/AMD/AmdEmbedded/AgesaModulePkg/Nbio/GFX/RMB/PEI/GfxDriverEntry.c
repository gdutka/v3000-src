/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <AMD.h>
#include <Filecode.h>
#include <PiPei.h>
#include <GnbDxio.h>
#include <GnbRegistersRMB.h>
#include <IdsHookId.h>
#include <GfxInfoRegisters.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Guid/GnbNbioBaseZPInfoHob.h>
#include <Guid/AmdMemoryInfoHob.h>
#include <Guid/GnbIntegratedSysInfoV4Hob.h>
#include <Guid/GnbPeiGopHob.h>
#include <Library/PeiServicesLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioRegisterAccLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <NbioGfxV4.h>
#include "GfxEnumConnectors.h"
#include <Ppi/Graphics.h>
#include <Ppi/NbioGfxPpi.h>
#include <Ppi/SocLogicalIdPpi.h>
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>

#define FILECODE        NBIO_GFX_RMB_PEI_GFXDRIVERENTRY_FILECODE
/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
#define MAX_DMCUB_SIZE                (256 * 1024) //256KB is max DMCUB size
#define MAX_IP_DISCOVERY_SIZE         (64 * 1024) //64KB is max IP discovery size

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern
VOID
PopulateSystemInfoTable (
  ATOM_FUSION_SYSTEM_INFO_V6      *SysInfoBuffer
  );

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


STATIC EFI_PEI_PPI_DESCRIPTOR mNbioGfxServicesPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdNbioGfxPpiGuid,
  NULL
};

EFI_STATUS
PeiGraphicInit (
  IN EFI_PEI_SERVICES                  **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN VOID                              *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mPeiGraphics[] = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiGraphicsPpiGuid,
  PeiGraphicInit
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
GfxDebugDumpDisplayPath (
  IN       EXT_DISPLAY_PATH            *DisplayPath
);

/*----------------------------------------------------------------------------------------
 *                    P P I   N O T I F Y   D E S C R I P T O R S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 *---------------------------------------------------------------------------------------
 *  ApplyIntegratedSysInfoOverride
 *
 *  Description:
 *     To apply IntegratedSysInfo Override
 *  Parameters:
 *    @param[in]     PeiServices   Pointer to EFI_PEI_SERVICES pointer
 *    @param[in]     LogicalId     To decide what CPU Family it is.
 *    @param[in]     PackageType   To decide what CPU Socket it is.
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
ApplyIntegratedSysInfoOverride (
  IN ATOM_FUSION_SYSTEM_INFO_V6     *SysInfoBuffer,
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN SOC_LOGICAL_ID                 LogicalId,
  IN UINT32                         PackageType,
  IN GNB_HANDLE                     *GnbHandle
  )
{
  UINT32           DdrType;
  UINT32           NumOfUmcPerSoc;
  UINT32           NumOfUmcPerChan;
  UINT32           NumOfActiveChan;
  UINT32           Umc;
  UINT32           AddrMaskDimm0;
  UINT32           AddrMaskDimm1;
  UINT32           UmcConfig;

  // Soc relevant
  NumOfUmcPerSoc = 4;
  NumOfUmcPerChan = 0;

  // Check Dram Type
  DdrType = 0;
  for (Umc = 0; Umc < NumOfUmcPerSoc; Umc++) {
    UmcConfig = 0;
    NbioRegisterRead (GnbHandle, TYPE_SMN, Umc0ConfigAddress + (Umc << 20), &UmcConfig, 0);
    DdrType |= (UmcConfig & 0x7);
  }


  switch (DdrType) {
  case 1:  // DDR5
    NumOfUmcPerChan = 2;
    SysInfoBuffer->sIntegratedSysInfo.memorytype = Ddr5MemType;
    break;
  case 6:  // LPDDR5
    NumOfUmcPerChan = 1;
    SysInfoBuffer->sIntegratedSysInfo.memorytype = LpDdr5MemType;
    break;
  default:
    IDS_HDT_CONSOLE (MAIN_FLOW, "Invalid Dram Type!\n");
    ASSERT (FALSE);
    break;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "Memory Type = 0x%x\n", SysInfoBuffer->sIntegratedSysInfo.memorytype);

  // Check Active Package Channel
  NumOfActiveChan = 0;
  for (Umc = 0; Umc < NumOfUmcPerSoc; Umc += NumOfUmcPerChan) {
    AddrMaskDimm0 = 0;
    AddrMaskDimm1 = 0;
    NbioRegisterRead (GnbHandle, TYPE_SMN, AddrMask_UMC0_Dimm0 + (Umc << 20), &AddrMaskDimm0, 0);
    NbioRegisterRead (GnbHandle, TYPE_SMN, AddrMask_UMC0_Dimm1 + (Umc << 20), &AddrMaskDimm1, 0);
    if ((AddrMaskDimm0 | AddrMaskDimm1) != 0) {
      NumOfActiveChan ++;
    }
  }

  SysInfoBuffer->sIntegratedSysInfo.umachannelnumber = (UINT8) NumOfActiveChan;
  IDS_HDT_CONSOLE (MAIN_FLOW, "Umachannelnumber %x\n", SysInfoBuffer->sIntegratedSysInfo.umachannelnumber);

  return;

}


/*----------------------------------------------------------------------------------------*/
/**
 * GetUmaInformation for RMB
 *
 *
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

BOOLEAN
EFIAPI
GetUmaInformation (
  IN  CONST EFI_PEI_SERVICES                   **PeiServices,
  OUT AMD_MEMORY_RANGE_DESCRIPTOR              *MemoryRange
)
{
  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *PcieServicesPpi;
  PCIe_PLATFORM_CONFIG            *Pcie;
  AMD_MEMORY_INFO_HOB             *AmdMemoryInfoHob;
  EFI_PEI_HOB_POINTERS            Hob;
  UINT32                          Index;
  AMD_MEMORY_RANGE_DESCRIPTOR     *Range;
  UINT32                          Property;
  EFI_STATUS                      Status;
  GNB_HANDLE                      *GnbHandle;
  AMD_CONFIG_PARAMS               *StdHeader;
  GNB_PCIE_INFORMATION_DATA_HOB   *PciePlatformConfigHob;
  BOOLEAN                         ProgramValues;
  UINT32                          NonIgpuData;

  StdHeader = NULL;
  GnbHandle = NULL;
  ProgramValues = FALSE;
  Range = NULL;
  NonIgpuData = 0;
  // Need topology structure
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieServicesPpiGuid,
                             0,
                             NULL,
                             (VOID **)&PcieServicesPpi
                             );
  PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);

  IDS_HOOK (IDS_HOOK_NBIO_GFX_INIT_CONFIG, (VOID *)GnbHandle, NULL);

  // Need debug options
  Property = TABLE_PROPERTY_DEFAULT;

  // Locate AMD_MEMORY_INFO_HOB
  AmdMemoryInfoHob = NULL;
  Status = PeiServicesGetHobList (&Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
        CompareGuid ( &Hob.Guid->Name, &gAmdMemoryInfoHobGuid)) {
      AmdMemoryInfoHob = (AMD_MEMORY_INFO_HOB *) (Hob.Raw +
                          sizeof (EFI_HOB_GENERIC_HEADER) +
                          sizeof (EFI_GUID));
      Status = EFI_SUCCESS;
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  if (AmdMemoryInfoHob != NULL) {
    // Adjust Property Gfxd on Debug Info and PCD's
    // From Earlier
    // Need to get UMA_BASE, UMA_SIZE to program these registers

    IDS_HDT_CONSOLE (MAIN_FLOW, "AMD_MEMORY_INFO_HOB at 0x%x\n", AmdMemoryInfoHob);
    IDS_HDT_CONSOLE (MAIN_FLOW, "  Version: 0x%x\n", AmdMemoryInfoHob->Version);
    IDS_HDT_CONSOLE (MAIN_FLOW, "  NumberOfDescriptor: 0x%x\n", AmdMemoryInfoHob->NumberOfDescriptor);
    for (Index = 0; Index < AmdMemoryInfoHob->NumberOfDescriptor; Index++) {
      Range = (AMD_MEMORY_RANGE_DESCRIPTOR*)&AmdMemoryInfoHob->Ranges[Index];
      IDS_HDT_CONSOLE (MAIN_FLOW, "  Range: %d\n", Index);
      IDS_HDT_CONSOLE (MAIN_FLOW, "    Base: 0x%lx\n", Range->Base);
      IDS_HDT_CONSOLE (MAIN_FLOW, "    Size: 0x%lx\n", Range->Size);
      IDS_HDT_CONSOLE (MAIN_FLOW, "    Attribute: 0x%x\n", Range->Attribute);
      if ((Range->Attribute) == AMD_MEMORY_ATTRIBUTE_UMA) {
        ProgramValues = TRUE;
        if (PcdGet8 (PcdCfgIgpuContorl) == 0) {
          Range->Base = 0;
          Range->Size = 0;
          NonIgpuData = 0;
          ProgramValues = FALSE;
        }
        CopyMem ((VOID*)MemoryRange, (VOID*)Range, sizeof(AMD_MEMORY_RANGE_DESCRIPTOR));
        return ProgramValues;
      }
    }
  } else {
    // For now, hardcode UMA at 0xD0000000 with size 256MB (0x10000000)
    IDS_HDT_CONSOLE (MAIN_FLOW, "Hardcoding UMA information\n");
    // CONFIG_MEMSIZE is the size of UMA in MB

    // This register defines the location of the frame buffer in the internal address space.
    // The internal address space has 48 address bits. Minimum Frame buffer size is 16 MB.
    // and the start location is required to be on a 16 MB boundary.
    // Therefore BASE(23:0) must be 0x000000 and TOP(23:0) must be 0xFFFFFF.
    // Only the 24 MSBs of each are loaded in the register.
    // If inside the TOP/BASE aperture, the address is corrected by subtracting BASE.
    // The register fields define bits (47:24) for both BASE and TOP
    MemoryRange->Base = UMA_BASE_HARDCODE;
    MemoryRange->Size = UMA_SIZE_HARDCODE;
    ProgramValues = TRUE;
  }

  return ProgramValues;
}


/*----------------------------------------------------------------------------------------*/
/**
 * ProgramUMARegister for RMB
 *
 *
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

VOID
EFIAPI
ProgramUMARegister (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN AMD_MEMORY_RANGE_DESCRIPTOR    *MemoryRange,
  IN GNB_HANDLE                      *GnbHandle,
  IN BOOLEAN                        IsRecovery
 )
{
  UINT32                          FbTopValue;
  UINT32                          FbBaseValue;
  UINT32                          ConfigMemsizeValue;
  UINT64                          TopOfMemory;
  UINT64                          TopOfMemory2;
  UINT32                          MMIOBase;
  UINT32                          MMIOLimit;
  NB_PCI_CTRL_STRUCT              PciCtrl;
  NB_PCI_ARB_STRUCT               PciArb;
  REG_DC_AUDIO_PORT_CONNECTIVITY_STRUCT  AudioPortField;
  UINT8                           MaxAudioEndpoints;
  UINT8                           AudioEPCount;
  EFI_STATUS                      Status;
  AMD_PEI_GOP_CONFIG              *AmdPeiGopConfigHob;

  IDS_HDT_CONSOLE (MAIN_FLOW, "ProgramUMARegister Entry\n");

  if (IsRecovery == TRUE) {
    ConfigMemsizeValue = (UINT32)(MemoryRange->Size >> 20);
    FbBaseValue = (UINT32) (MemoryRange->Base >> 24);
    FbTopValue = (UINT32) ((MemoryRange->Base + MemoryRange->Size - 1) >> 24);

    Status = PeiServicesAllocatePool(sizeof(AMD_PEI_GOP_CONFIG), (VOID **)&AmdPeiGopConfigHob);
    if (!EFI_ERROR(Status)) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "Create AmdPeiGopConfigHob HOB\n");
      AmdPeiGopConfigHob->ConfigMemsizeValue = ConfigMemsizeValue;
      AmdPeiGopConfigHob->FbTopValue = FbTopValue;
      AmdPeiGopConfigHob->FbBaseValue = FbBaseValue;
      BuildGuidDataHob (
        &gGnbPeiGopHobGuid,
        AmdPeiGopConfigHob,
        sizeof (AMD_PEI_GOP_CONFIG)
        );
    }
    PcdSet32S (PcdPeiGopConfigMemsize, ConfigMemsizeValue);
    PcdSet32S (PcdPeiGopVmFbOffset, FbBaseValue);
    PcdSet32S (PcdPeiGopVmFbLocationTop, FbTopValue);
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcdPeiGopConfigMemsize : %x \n", PcdGet32 (PcdPeiGopConfigMemsize));
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcdPeiGopVmFbOffset : %x \n", PcdGet32 (PcdPeiGopVmFbOffset));
    IDS_HDT_CONSOLE (MAIN_FLOW, "PcdPeiGopVmFbLocationTop : %x \n", PcdGet32 (PcdPeiGopVmFbLocationTop));

  }

  // TOM - #COPY# +MC_VM_NB_TOP_OF_DRAM_SLOT1.TOP_OF_DRAM=NB_TOP_OF_DRAM_SLOT1
  TopOfMemory = AsmReadMsr64 (TOP_MEM);
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_GCMC_VM_NB_TOP_OF_DRAM_SLOT1_ADDRESS,
    (UINT32) ~(GCMC_VM_NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_MASK),
    (UINT32) TopOfMemory,
    0
    );
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_MMMC_VM_NB_TOP_OF_DRAM_SLOT1_ADDRESS,
    (UINT32) ~(MMMC_VM_NB_TOP_OF_DRAM_SLOT1_TOP_OF_DRAM_MASK),
    (UINT32) TopOfMemory,
    0
    );

  // TOM2 - #COPY# +MC_VM_NB_LOWER_TOP_OF_DRAM2.LOWER_TOM2=NB_LOWER_TOP_OF_DRAM2
  //        #COPY# +MC_VM_NB_UPPER_TOP_OF_DRAM2.UPPER_TOM2=NB_UPPER_TOP_OF_DRAM2
  TopOfMemory2 = AsmReadMsr64 (TOP_MEM2);

  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_MMMC_VM_NB_LOWER_TOP_OF_DRAM2_ADDRESS,
    (UINT32)~(0xFF800001), // Bit23 ~ Bit31 and Bit0
    (UINT32)(TopOfMemory2 | BIT0),
    0
    );
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_GCMC_VM_NB_LOWER_TOP_OF_DRAM2_ADDRESS,
    (UINT32)~(0xFF800001), // Bit23 ~ Bit31 and Bit0
    (UINT32)(TopOfMemory2 | BIT0),
    0
    );
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_MMMC_VM_NB_UPPER_TOP_OF_DRAM2_ADDRESS,
    (UINT32)~(MMMC_VM_NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_MASK), // Bit0 ~ Bit11
    (UINT32)(RShiftU64 ((UINT64) TopOfMemory2, 32)),
    0
    );
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_GCMC_VM_NB_UPPER_TOP_OF_DRAM2_ADDRESS,
    (UINT32)~(GCMC_VM_NB_UPPER_TOP_OF_DRAM2_UPPER_TOM2_MASK), // Bit0 ~ Bit11
    (UINT32)(RShiftU64 ((UINT64) TopOfMemory2, 32)),
    0
    );

  // MMIO BASE - #COPY# +MC_VM_NB_MMIOBASE.MMIOBASE=NB_MMIOBASE
  NbioRegisterRead (GnbHandle, TYPE_SMN, SMN_NB_MMIOBASE_ADDRESS, &MMIOBase, 0);
  NbioRegisterWrite (GnbHandle, TYPE_SMN, SMN_MMMC_VM_NB_MMIOBASE_ADDRESS, &MMIOBase, 0);
  NbioRegisterWrite (GnbHandle, TYPE_SMN, SMN_GCMC_VM_NB_MMIOBASE_ADDRESS, &MMIOBase, 0);

  // MMIO Limit - #COPY# +MC_VM_NB_MMIOLIMIT.MMIOLIMIT=NB_MMIOLIMIT
  NbioRegisterRead (GnbHandle, TYPE_SMN, SMN_NB_MMIOLIMIT_ADDRESS, &MMIOLimit, 0);
  NbioRegisterWrite (GnbHandle, TYPE_SMN, SMN_MMMC_VM_NB_MMIOLIMIT_ADDRESS, &MMIOLimit, 0);
  NbioRegisterWrite (GnbHandle, TYPE_SMN, SMN_GCMC_VM_NB_MMIOLIMIT_ADDRESS, &MMIOLimit, 0);

  // VGA HOLE -  #COPY# +MC_VM_NB_PCI_ARB.VGA_HOLE=NB_PCI_ARB
  NbioRegisterRead (GnbHandle, TYPE_SMN, SMN_NB_PCI_ARB_ADDRESS, &PciArb.Value, 0);

  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_MMMC_VM_NB_PCI_ARB_ADDRESS,
    (UINT32) ~(MMMC_VM_NB_PCI_ARB_VGA_HOLE_MASK),
    (UINT32) (PciArb.Field.VGA_HOLE << MMMC_VM_NB_PCI_ARB_VGA_HOLE_OFFSET), // 1 means VGA range is decoded as MMIO region, 0 system memory.
    0
    );
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_GCMC_VM_NB_PCI_ARB_ADDRESS,
    (UINT32) ~(GCMC_VM_NB_PCI_ARB_VGA_HOLE_MASK),
    (UINT32) (PciArb.Field.VGA_HOLE << GCMC_VM_NB_PCI_ARB_VGA_HOLE_OFFSET), // 1 means VGA range is decoded as MMIO region, 0 system memory.
    0
    );

  // MMIO Enable -  #COPY# +MC_VM_NB_PCI_CTRL.MMIOENABLE=NB_PCI_CTRL
  NbioRegisterRead (GnbHandle, TYPE_SMN, SMN_NB_PCI_CTRL_ADDRESS, &PciCtrl.Value, 0);
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_MMMC_VM_NB_PCI_CTRL_ADDRESS,
    (UINT32) ~(MMMC_VM_NB_PCI_CTRL_MMIOENABLE_MASK),
    (UINT32) (PciCtrl.Field.MMIOEnable << MMMC_VM_NB_PCI_CTRL_MMIOENABLE_OFFSET),
    0
    );
  NbioRegisterRMW (
    GnbHandle,
    TYPE_SMN,
    SMN_GCMC_VM_NB_PCI_CTRL_ADDRESS,
    (UINT32) ~(GCMC_VM_NB_PCI_CTRL_MMIOENABLE_MASK),
    (UINT32) (PciCtrl.Field.MMIOEnable << GCMC_VM_NB_PCI_CTRL_MMIOENABLE_OFFSET),
    0
    );

  //
  // Audio feature callback
  //
  MaxAudioEndpoints = PcdGet8 (PcdCfgMaxNumAudioEndpoints);
  AudioEPCount = MaxAudioEndpoints;
  if (MaxAudioEndpoints != 0) {
    AudioEPCount = 7 - AudioEPCount; //   00 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_ALL : All
                                     //   01 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_6 : 6
                                     //   02 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_5 : 5
                                     //   03 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_4 : 4
                                     //   04 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_3 : 3
                                     //   05 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_2 : 2
                                     //   06 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_1 : 1
                                     //   07 - CC_RCU_DC_AUDIO_PORT_CONNECTIVITY_PORT_CONNECTIVITY_0 : 0
    AudioPortField.Field.REG_PORT_CONNECTIVITY = AudioEPCount;
    AudioPortField.Field.REG_PORT_CONNECTIVITY_OVERRIDE_ENABLE = 1;
    NbioRegisterWrite (
      GnbHandle,
      TYPE_SMN,
      SMN_REG_DC_AUDIO_PORT_CONNECTIVITY_ADDRESS,
      &AudioPortField,
      0
      );
  }
 //
 // Disable all audio end point
 //
 if (PcdGetBool (PcdCfgDisableAllNumAudioEndpoints) == TRUE) {
    AudioPortField.Field.REG_PORT_CONNECTIVITY = 7;
    AudioPortField.Field.REG_PORT_CONNECTIVITY_OVERRIDE_ENABLE = 1;
    NbioRegisterWrite (
      GnbHandle,
      TYPE_SMN,
      SMN_REG_DC_AUDIO_PORT_CONNECTIVITY_ADDRESS,
      &AudioPortField,
      0
      );
 }
  IDS_HDT_CONSOLE (MAIN_FLOW, "ProgramUMARegister Exit\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * PeiGraphicCallback for RMB
 *
 *
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

VOID
EFIAPI
PeiGraphicCallback (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN AMD_MEMORY_RANGE_DESCRIPTOR    *MemoryRange,
  IN GNB_HANDLE                      *GnbHandle
 )
{
  EFI_STATUS                        Status;

  IDS_HDT_CONSOLE (MAIN_FLOW, "PeiGraphicCallback..\n");
  //Recovery mode for PEI GOP
    Status = PeiServicesNotifyPpi (mPeiGraphics);
    Status = PeiServicesInstallPpi (&mNbioGfxServicesPpiList); //This driver is done.
}


EFI_STATUS
PeiGraphicInit (
  IN EFI_PEI_SERVICES                  **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR         *NotifyDescriptor,
  IN VOID                              *Ppi
  )
{
  IDS_HDT_CONSOLE (MAIN_FLOW, "PeiGraphicInit..\n");
  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * AmdNbiosGfx driver entry point for RMB
 *
 *
 *
 * @param[in]  FileHandle  Standard configuration header
 * @param[in]  PeiServices Pointer to EFI_PEI_SERVICES pointer
 * @retval     EFI_STATUS
 */

EFI_STATUS
EFIAPI
AmdNbioGfxRMBPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                      Status;
  ATOM_FUSION_SYSTEM_INFO_V6      *SystemInfoBufferHob;
  UINT32                          UMAPeiGopDestination;

  NBIO_Frame_Buffer_DATA_V4_HOB   *FrambufferHob;
  SOC_LOGICAL_ID                  LogicalId;
  AMD_PEI_SOC_LOGICAL_ID_PPI      *SocLogicalIdPpi;
  UINT32                          PackageType;
  EFI_BOOT_MODE                   BootMode;
  BOOLEAN                         ProgramValues;
  AMD_MEMORY_RANGE_DESCRIPTOR     Range = {0};
  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *PcieServicesPpi;
  GNB_PCIE_INFORMATION_DATA_HOB   *PciePlatformConfigHob;
  GNB_HANDLE                      *GnbHandle;
  PCIe_PLATFORM_CONFIG            *Pcie;
  BOOLEAN                         IsRecovery;
  PRESIL_CTRL1                    PreSilCtrl1;

  ATOM_COMMON_TABLE_HEADER        *TableHeaderPtr;
  ATOM_N6_DISPLAY_PHY_TUNING_SET  *DispPhyTuningDataPtr;
  BOOLEAN                         IsN6TableReady;

  AGESA_TESTPOINT (TpNbioGfxPeiEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "AmdNbioGfxRMBPei Entry\n");

  SystemInfoBufferHob = NULL;
  IsRecovery = FALSE;
  UMAPeiGopDestination = 0;
  IsN6TableReady = FALSE;
  //
  // Get Boot Mode
  //
  PeiServicesGetBootMode (&BootMode);
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    IsRecovery = TRUE;
  }
  //
  // Get CPU revision
  //
  SetMem ((VOID *)(&LogicalId), sizeof (LogicalId), 0);
  // Get package type
  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdSocLogicalIdPpiGuid,
                           0,
                           NULL,
                           &SocLogicalIdPpi
                           );
  if (!EFI_ERROR (Status)) {
    Status = SocLogicalIdPpi->GetLogicalIdOnCurrentCore (&LogicalId);
  }
  PackageType = LibAmdGetPackageType ((AMD_CONFIG_PARAMS *) NULL);

  // Get PCIe config hob
  (*PeiServices)->LocatePpi (
                    PeiServices,
                    &gAmdNbioPcieServicesPpiGuid,
                    0,
                    NULL,
                    (VOID **)&PcieServicesPpi
                    );
  PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);

  // Get memory location for system info table
  ProgramValues = GetUmaInformation (PeiServices, &Range);

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_GIO == 0) {
    if (ProgramValues == TRUE) {
      if (BootMode != BOOT_ON_S3_RESUME) {

        PeiServicesCreateHob (
          EFI_HOB_TYPE_GUID_EXTENSION,
          sizeof (NBIO_Frame_Buffer_DATA_V4_HOB),
          &FrambufferHob
          );

        FrambufferHob->EfiHobGuidType.Name = gGnbIntegratedSysInfoV4HobGuid;
        SystemInfoBufferHob = (ATOM_FUSION_SYSTEM_INFO_V6 *) &(FrambufferHob->AtomFusionSystemInfoV6);
        IDS_HDT_CONSOLE (MAIN_FLOW, "SystemInfoBufferHob %x\n", SystemInfoBufferHob);

        FrambufferHob->UMAInfo.Base = Range.Base;
        FrambufferHob->UMAInfo.Size = Range.Size;
        IDS_HDT_CONSOLE (MAIN_FLOW, "FrambufferHob->UMAInfo.Base %lx\n", FrambufferHob->UMAInfo.Base);
        IDS_HDT_CONSOLE (MAIN_FLOW, "FrambufferHob->UMAInfo.Size %lx\n", FrambufferHob->UMAInfo.Size);

        (**PeiServices).SetMem (
                          SystemInfoBufferHob,
                          sizeof (ATOM_FUSION_SYSTEM_INFO_V6),
                          0
                          );

        PopulateSystemInfoTable (SystemInfoBufferHob);
      }

      ApplyIntegratedSysInfoOverride (SystemInfoBufferHob, PeiServices, LogicalId, PackageType, GnbHandle);
      IDS_HOOK (IDS_HOOK_NBIO_GFX_INIT_FRAME_BUFFER, NULL, (VOID *)SystemInfoBufferHob);

      ProgramUMARegister (PeiServices, &Range, GnbHandle, IsRecovery);

      // PEI GOP feature
      if (BootMode == BOOT_IN_RECOVERY_MODE && PcdGetBool (PcdPeiGopEnable)) {
        //
        // In this case, UMA integrated table need report in PEI phase
        // Copy intergrated table from hob to UMA below4G - IP discovery - DMCUB

        UMAPeiGopDestination = (UINT32)(((FrambufferHob->UMAInfo.Base + FrambufferHob->UMAInfo.Size) - MAX_IP_DISCOVERY_SIZE - MAX_DMCUB_SIZE - sizeof(ATOM_FUSION_SYSTEM_INFO_V6)));
        IDS_HDT_CONSOLE (MAIN_FLOW, "UMAPeiGopDestination %x\n", UMAPeiGopDestination);
        CopyMem ((VOID*)UMAPeiGopDestination,
          &(FrambufferHob->AtomFusionSystemInfoV6),
          sizeof(ATOM_FUSION_SYSTEM_INFO_V6));

        //
        // UMA Top - IP Discovery - Integrated table - Phy header - phy tunning setting data
        //
        if (PcdGet32 (PcdAmdDisplayPhyTuningSettingTableHeader) != 0x00) {
          TableHeaderPtr = (ATOM_COMMON_TABLE_HEADER*) PcdGet32 (PcdAmdDisplayPhyTuningSettingTableHeader);
          DispPhyTuningDataPtr = (ATOM_N6_DISPLAY_PHY_TUNING_SET*)PcdGet32 (PcdAmdDisplayPhyTuningSettingTableContent);
          IsN6TableReady = TRUE;
        }

        UMAPeiGopDestination = UMAPeiGopDestination - sizeof(ATOM_COMMON_TABLE_HEADER);

        IDS_HDT_CONSOLE (MAIN_FLOW, "Done TableHeaderPtr->structuresize %x\n", TableHeaderPtr->structuresize);
        IDS_HDT_CONSOLE (MAIN_FLOW, "Done TableHeaderPtr->content_revision %x\n", TableHeaderPtr->content_revision);
        IDS_HDT_CONSOLE (MAIN_FLOW, "Done TableHeaderPtr->format_revision %x\n", TableHeaderPtr->format_revision);
        if (IsN6TableReady == TRUE) {
          CopyMem ((VOID*)UMAPeiGopDestination,
            (VOID *)TableHeaderPtr,
            sizeof(ATOM_COMMON_TABLE_HEADER));

          IDS_HDT_CONSOLE (MAIN_FLOW, "Done ATOM_COMMON_TABLE_HEADER %x\n", UMAPeiGopDestination);
          UMAPeiGopDestination =  (UMAPeiGopDestination + sizeof(ATOM_COMMON_TABLE_HEADER)) - (TableHeaderPtr->structuresize);
          CopyMem ((VOID*)UMAPeiGopDestination,
            (VOID *)DispPhyTuningDataPtr,
            (TableHeaderPtr->structuresize - sizeof(ATOM_COMMON_TABLE_HEADER)));

          // check here
          IDS_HDT_CONSOLE (MAIN_FLOW, "Done UMAPeiGopDestination %x\n", UMAPeiGopDestination);
        }
        PeiGraphicCallback (PeiServices, &Range, GnbHandle);
      }
    IDS_HOOK (IDS_HOOK_NBIO_GFX_VGA_REGISTER_CONFIG, (VOID *)GnbHandle, NULL);

    }
  }
  AGESA_TESTPOINT (TpNbioGfxPeiExit, NULL);

  return EFI_SUCCESS;
}
