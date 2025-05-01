/** @file
 DXE Chipset Services Library.

 This file contains only one function that is DxeCsSvcSkipGetPciRom().
 The function DxeCsSvcSkipGetPciRom() use chipset services to return
 a PCI ROM image for the device is represented or not.

***************************************************************************
* Copyright (c) 2013 - 2021, Insyde Software Corp. All Rights Reserved.
*
* You may not reproduce, distribute, publish, display, perform, modify, adapt,
* transmit, broadcast, present, recite, release, license or otherwise exploit
* any part of this publication in any form, by any means, without the prior
* written permission of Insyde Software Corporation.
*
******************************************************************************
*/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AcpiPlatformLib.h>
#include <Library/BaseLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/DxeOemSvcChipsetLib.h>
#include <Library/PciExpressLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Guid/EventGroup.h>
#include <Guid/H2OBdsCheckPoint.h>

#include <Protocol/AllPciIoPrtclsInstlFinished.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/AcpiSupport.h>
#include <Protocol/MpService.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/AmdCoreTopologyProtocol.h>
#include <Protocol/HddPasswordService.h>
#include <Protocol/PciIo.h>
#include <Protocol/AmdCpmTableProtocol/AmdCpmTableProtocol.h>
//[-start-220126-IB14740206-add]//
#include <Protocol/AMDMcmInfo.h>
//[-end-220126-IB14740206-add]//

#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/WatchdogResourceTable.h>

#include <KernelSetupConfig.h>
#include <ChipsetSetupConfig.h>
#include <FchRegistersCommon.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/BdsCpLib.h>

#include <Library/VariableLib.h>

#define LVL_C2_EN                         0x12
#define LVL_C3_EN                         0x53
#define LVL_C2_DIS                        101
#define LVL_C3_DIS                        1001
//[-start-220826-IB14740258-modify]//
#define P_LVL2_LAT                        0x65
//[-end-220826-IB14740258-modify]//
#define P_LVL3_LAT                        0x03e9

GLOBAL_REMOVE_IF_UNREFERENCED UINTN       mNumEnabledCPUs;

#define PCI_CLASS_MASS_STORAGE_NVM        0x08  // mass storage sub-class non-volatile memory.
#define PCI_IF_NVMHCI                     0x02  // mass storage programming interface NVMHCI.
#define NVME_CLASS_CODE                   (UINT32) ( (PCI_CLASS_MASS_STORAGE << 24) | \
                                                     (PCI_CLASS_MASS_STORAGE_NVM << 16) | \
                                                     (PCI_IF_NVMHCI << 8) )  //0x01080200

//[-start-220126-IB14740206-add]//
AMD_MCM_INFO_PROTOCOL                     *mAmdMcmInfoProtocol;
//[-end-220126-IB14740206-add]//

UINT8
GetEndBusNumber (
  VOID
  );

EFI_STATUS
RemoveAcpitable (
  UINT32       Signature
  );

BOOLEAN
IsPco (
  VOID
  )
{
  UINT32        TempData32;

  TempData32 =  (PcdGet32 (PcdGetRVFamilyOPN) >> 30) & 0x3;

  if (TempData32 == 1) {
    return FALSE;
  } else if (TempData32 == 3) {
    return FALSE;
  }
  return TRUE;  
}

BOOLEAN
IsDualSource (
  VOID
  )
{
  UINT32        TempData32;

  TempData32 =  (PcdGet32 (PcdGetRVFamilyOPN) >> 30) & 0x3;

  if (TempData32 == 2) {
    return TRUE;
  } else if (TempData32 == 3) {
    return TRUE;
  }
  return FALSE;  
}

/**
 Update MADT table content
 Depends on MPService return data to setup MADT core enable flag and APIC ID.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update table success
 @retval            EFI_UNSUPPORTED     APIC Mode disabled
*/
STATIC
EFI_STATUS
UpdateMadt (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  EFI_STATUS                                      Status;
  EFI_ACPI_1_0_PROCESSOR_LOCAL_APIC_STRUCTURE     *ApicPtr = NULL;
  EFI_ACPI_1_0_IO_APIC_STRUCTURE                  *IoApicPtr = NULL;
  UINT8                                           *CurrPtr = NULL;
  UINT8                                           CurrProcessor = 0;
  EFI_PROCESSOR_INFORMATION                       ProcessorInfo;
  EFI_MP_SERVICES_PROTOCOL                        *MpService;
  UINTN                                           NumCPUs = 1;
  KERNEL_CONFIGURATION                            *SetupNVRamKernel;
  UINT32                                          RegEax;

  SetupNVRamKernel  = (KERNEL_CONFIGURATION *)SetupNVRam;
  if (SetupNVRamKernel->IoApicMode == 0) {
    Status = EFI_UNSUPPORTED;
  }
  //
  // Find the MP Protocol. This is an MP platform, so MP protocol must be
  // there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID**)&MpService
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
               MpService,
               &NumCPUs,
               &mNumEnabledCPUs
               );

  CurrPtr = (UINT8*) &((EFI_ACPI_DESCRIPTION_HEADER*) (Table))[1];
  CurrPtr = CurrPtr + 8; // Size of Local APIC Address & Flag
  while (CurrPtr < ((UINT8*) (Table) + ((Table)->Length))) {
    ApicPtr = (EFI_ACPI_1_0_PROCESSOR_LOCAL_APIC_STRUCTURE*) CurrPtr;
    //
    // Check table entry type
    //
    if (ApicPtr->Type == EFI_ACPI_1_0_PROCESSOR_LOCAL_APIC) {
      ApicPtr->Flags  = 0;
      ApicPtr->ApicId = 0;

      //
      // Look up this processor information
      //
      Status = MpService->GetProcessorInfo (
                            MpService,
                            CurrProcessor,
                            &ProcessorInfo
                            );
      if (!EFI_ERROR (Status)) {
        if (ProcessorInfo.StatusFlag & PROCESSOR_ENABLED_BIT) {
          ApicPtr->Flags = EFI_ACPI_1_0_LOCAL_APIC_ENABLED;
        }
        ApicPtr->ApicId = (UINT8) (ProcessorInfo.ProcessorId);
      }

      //
      // Increment the procesor count
      //
      CurrProcessor++;
    }

    // Update IO APIC ID for family 17 only
    // Make sure IO APIC ID in MADT is aligned with AGESA IVRS table
    AsmCpuid (0x80000001, &RegEax, NULL, NULL, NULL);
    if (((RegEax >> 20) & 0xFF) != 0x6) {
      IoApicPtr = (EFI_ACPI_1_0_IO_APIC_STRUCTURE *) CurrPtr;
      if (IoApicPtr->Type == EFI_ACPI_1_0_IO_APIC) {
        if (IoApicPtr->IoApicId == 4) {
          IoApicPtr->IoApicId = PcdGet8 (PcdCfgFchIoapicId);
        } else if (IoApicPtr->IoApicId == 5) {
          IoApicPtr->IoApicId = PcdGet8 (PcdCfgGnbIoapicId);
        }
      }
    }
    //
    // Go to the next structure in the APIC table
    //
    CurrPtr += (ApicPtr->Length);
  }

  return EFI_SUCCESS;
}

/**
 Update FACP Table according to platform specific.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update table success
*/
STATIC
EFI_STATUS
UpdateFacp (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE   *FadtPointer1;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE   *FadtPointer3;
  EFI_STATUS                                  Status;
  AMD_PBS_SETUP_OPTION                        AmdPbsConfiguration;

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  FadtPointer1 = (EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE*) Table;
  FadtPointer3 = (EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE*) Table;

  // C2 Latency value in FACP.
  FadtPointer1->PLvl2Lat = (UINT16) P_LVL2_LAT;
  // C3 Latency value in FACP.
  FadtPointer1->PLvl3Lat = (UINT16) P_LVL3_LAT;

  FadtPointer1->Flags |= EFI_ACPI_1_0_P_LVL2_UP;
  if (FeaturePcdGet (PcdH2OLegacyFreeSupported)) {
    FadtPointer1->Flags |= EFI_ACPI_2_0_RESET_REG_SUP;
  }

  if (AmdPbsConfiguration.WakeOnPME) {
    FadtPointer3->Flags |= EFI_ACPI_3_0_PCI_EXP_WAK;
  } else {
    FadtPointer3->Flags &= ~EFI_ACPI_3_0_PCI_EXP_WAK;  // PCI Express* Wake Disable(PCIEXPWAK_DIS) 
  }
  
 // The following HW_REDUCED_ACPI and LOW_POWER_S0_IDLE_CAPABLE flags are supposed to be
 // set up by FchD3ColdTbl in AGESA, but it is currently broken.  Let's do them here until
 // that bug is resolved.
 // debug {
#ifdef MODERN_STANDBY_SUPPORT
  if (FeaturePcdGet (PcdModernStandbySupport)) {
    if (AmdPbsConfiguration.ModernStandbyEn == 1) {
      FadtPointer1->Flags|=BIT21;           // Set LOW_POWER_S0_IDLE_CAPABLE flag
      FadtPointer1->Flags|=BIT4;            // Set PWR_BUTTON flag to 1 to use Control method power button
    } else {
      FadtPointer1->Flags&=~BIT21;          // Reset LOW_POWER_S0_IDLE_CAPABLE flag
      FadtPointer1->Flags&=~BIT4;           // Set PWR_BUTTON flag to 0 to use Fixed hardware power button
    }
  }
#endif

#ifdef REDUCED_HARDWARE_SUPPORT
  if (FeaturePcdGet (PcdReduceHardwareSupport)) {
    if (AmdPbsConfiguration.ReduceHardwareEn == 1) {
      FadtPointer1->Flags|=BIT20;           // Set HW_REDUCED_ACPI flag
    } else {
      FadtPointer1->Flags&=~BIT20;          // Reset HW_REDUCED_ACPI flag
    }
  }
#endif
  return EFI_SUCCESS;
}

VOID
EFIAPI
UpdateI2cTouchPadResource (
  IN OUT UINT8  *DsdtPointer,
  IN AMD_PBS_SETUP_OPTION             *AmdPbsConfiguration
  )
{
  UINT8                               *Operation;
  UINT8                               *RegionAddress;
  DEBUG((DEBUG_INFO, "[%a] Check ...\n"  , __FUNCTION__));
  do
  {
    if (1 != AmdPbsConfiguration->TouchPad)
      break;
    Operation = (DsdtPointer - 1);
    if ((*Operation == AML_NAME_OP))
    {
      RegionAddress = (UINT8 *)(DsdtPointer + 0x19);
      if (*RegionAddress == 0x2C) // Default will be 0x2C
      {
        *RegionAddress = (UINT8)(AmdPbsConfiguration->TouchPadSlaveAddr);
        DEBUG((DEBUG_INFO, "[%a] Update SlaveAddr\n"  , __FUNCTION__));
      }
    }
  } while (FALSE);
  return;
}

VOID
EFIAPI
UpdateI2cTouchPadHidAddress (
  IN OUT UINT8  *DsdtPointer,
  IN AMD_PBS_SETUP_OPTION             *AmdPbsConfiguration
  )
{
  UINT8                               *Operation;
  UINT8                               *RegionAddress;
  DEBUG((DEBUG_INFO, "[%a] Check ...\n"  , __FUNCTION__));
  do
  {
    if (1 != AmdPbsConfiguration->TouchPad)
      break;
    Operation = (DsdtPointer - 1);
    if ((*Operation == AML_NAME_OP)) {
      RegionAddress = (UINT8*)(DsdtPointer + 0x5);
      if (*RegionAddress == 0x20) // Default will be 0x20
      {
        *RegionAddress = (AmdPbsConfiguration->TouchPadHidAddr);
        DEBUG((DEBUG_INFO, "[%a] Update HID\n"  , __FUNCTION__));
      }
    }
  } while (FALSE);

  return;
}

VOID
EFIAPI
UpdatePspBar3 (
  IN EFI_EVENT                        Event,
  IN VOID                             *Context
  )
{
  EFI_STATUS                          Status;
  EFI_ACPI_SUPPORT_PROTOCOL           *AcpiSupport;
  INTN                                Index;
  UINTN                               Handle;
  EFI_ACPI_TABLE_VERSION              Version;
  EFI_ACPI_DESCRIPTION_HEADER         *Table;
  UINT8                               *Operation;
  UINT8                               *DsdtPointer;
  UINT32                              *RegionAddress;
  UINT32                              *Signature;
  BOOLEAN                             IsFound;

  DEBUG ((EFI_D_ERROR, "    UpdatePspBar3\n"));
  IsFound = FALSE;

  //
  // Found DSDT table
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSupportProtocolGuid,
                  NULL,
                  (VOID**)&AcpiSupport
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "      ERR: Failed to locate gEfiAcpiSupportProtocolGuid\n"));
    return;
  }

  Index  = 0;
  Handle = 0;
  do {
    Table  = NULL;
    Status = AcpiSupport->GetAcpiTable (
                            AcpiSupport,
                            Index,
                            (VOID**)&Table,
                            &Version,
                            &Handle
                            );
    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Check Signture and update DSDT table
    //
    if (Table->Signature == SIGNATURE_32('D', 'S', 'D', 'T')) {
      for (DsdtPointer = (UINT8 *)((UINTN)Table + sizeof (EFI_ACPI_COMMON_HEADER)); DsdtPointer <= (UINT8 *)((UINTN)Table + (Table->Length)); DsdtPointer ++) {
        Signature = (UINT32*) DsdtPointer;

        switch (*Signature) {

        case (SIGNATURE_32 ('B', 'A', 'R', '3')):
          Operation = (DsdtPointer - 1);
          if ((*Operation == AML_NAME_OP)) {
            RegionAddress = (UINT32 *)(DsdtPointer + 5);

            if (GetPspMmioBase (RegionAddress)) {
              IsFound = TRUE;
              DEBUG ((EFI_D_ERROR, "    PSP BAR3 ADDR = 0x%x\n", *RegionAddress));
            } else {
              ASSERT (FALSE);
            }

          }
          break;

        default:
          break;
        }
      }

      //
      // Great, we have found and update the table.
      // Publish it now.
      //
      if (IsFound) {
        Status = AcpiSupport->SetAcpiTable (
                                AcpiSupport,
                                Table,
                                TRUE,
                                Version,
                                &Handle
                                );
        if (!EFI_ERROR (Status)) {
          Status = AcpiSupport->PublishTables (
                                  AcpiSupport,
                                  EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0 | EFI_ACPI_TABLE_VERSION_4_0
                                  );
        }
        gBS->FreePool (Table);

        return;
      }
    }

    //
    // Not found, get next
    //
    gBS->FreePool (Table);
    Index++;
  } while (TRUE);

  return;
}

BOOLEAN
CheckNvmeDevice ( 
  IN UINT8        Device,
  IN UINT8        Function
  )
{
  UINT32          ClassCode;
  UINT8           SecBusNum;

  //write the Bus number.
  PciExpressWrite8 (PCI_EXPRESS_LIB_ADDRESS(0, Device, Function, 0x19),0x3);
  PciExpressWrite8 (PCI_EXPRESS_LIB_ADDRESS(0, Device, Function, 0x1A),0x3);

  SecBusNum = PciExpressRead8 (PCI_EXPRESS_LIB_ADDRESS (0, Device, Function, 0x19));

  ClassCode = PciExpressRead32 (PCI_EXPRESS_LIB_ADDRESS (SecBusNum, 0, 0, 0x08));
  DEBUG ((EFI_D_INFO, " ClassCode read is  %x :: NVME_CLASS_CODE is %x \n", ClassCode, NVME_CLASS_CODE));

  //Restore the Bus number.
  PciExpressWrite8 (PCI_EXPRESS_LIB_ADDRESS (0, Device, Function, 0x19),0x0);
  PciExpressWrite8 (PCI_EXPRESS_LIB_ADDRESS (0, Device, Function, 0x1A),0x0);
  if ((ClassCode & 0xFFFFFF00) == NVME_CLASS_CODE) {
    return TRUE;
  }

  return FALSE;
}

/*
  Update NVMe passwrod status to the table of CPM. (Refer : NvmeD3.asl)

*/
VOID
UpdateNvmePwdStatusToGnvs (
  IN EFI_EVENT                          Event,
  IN H2O_CP_HANDLE                      Handle
  )
{
  EFI_STATUS                            Status;
  EFI_HDD_PASSWORD_SERVICE_PROTOCOL     *HddPasswordService;
  HDD_PASSWORD_HDD_INFO                 *HddInfoArray;
  UINTN                                 NumOfHdd;
  UINTN                                 Index;
  UINT8                                 ClassCode[3];

  AMD_CPM_TABLE_PROTOCOL                *CpmTableProtocolPtr;
  AMD_CPM_NVME_RTD3_TABLE               *NvmeRtd3TablePtr;

  DEBUG ((DEBUG_INFO, "UpdateNvmePwdStatusToGnvs..\n"));

  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID**)&CpmTableProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "  Locate gAmdCpmTableProtocolGuid failed:(%r)\n", Status));
    return;
  }

  Status = gBS->LocateProtocol (
                  &gEfiHddPasswordServiceProtocolGuid,
                  NULL,
                  (VOID **)&HddPasswordService
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "  Locate gEfiHddPasswordServiceProtocolGuid failed:(%r)\n", Status));
    return;
  }

  NvmeRtd3TablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_NVME_STD3);

  Status = HddPasswordService->GetHddInfo (
                                 HddPasswordService,
                                 &HddInfoArray,
                                 &NumOfHdd
                                 );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "  GetHddInfo failed:(%r)\n", Status));
    return;
  }

  DEBUG ((DEBUG_INFO, "  NumOfHdd: 0x%x\n", NumOfHdd));
  for (Index = 0; Index < NumOfHdd; Index++ ) {
    DEBUG ((DEBUG_INFO, "  Hdd[%x]: Bus:0x%x, Dev:0x%x, Function:0x%x \n", Index, HddInfoArray[Index].PciBus, HddInfoArray[Index].PciDevice, HddInfoArray[Index].PciFunction));
    ClassCode[0]  = PciRead8 (PCI_LIB_ADDRESS (HddInfoArray[Index].PciBus, HddInfoArray[Index].PciDevice, HddInfoArray[Index].PciFunction, PCI_CLASSCODE_OFFSET));
    ClassCode[1]  = PciRead8 (PCI_LIB_ADDRESS (HddInfoArray[Index].PciBus, HddInfoArray[Index].PciDevice, HddInfoArray[Index].PciFunction, PCI_CLASSCODE_OFFSET + 1));
    ClassCode[2]  = PciRead8 (PCI_LIB_ADDRESS (HddInfoArray[Index].PciBus, HddInfoArray[Index].PciDevice, HddInfoArray[Index].PciFunction, PCI_CLASSCODE_OFFSET + 2));

    if (((ClassCode[0] == 0x2) || (ClassCode[0] == 0x3)) && (ClassCode[1] == 0x8) && (ClassCode[2] == 0x1)) {
      DEBUG ((DEBUG_INFO, "    Is NVMe.\n"));
      if ((HddInfoArray[Index].HddSecurityStatus & HDD_ENABLE_BIT) == HDD_ENABLE_BIT) {
        DEBUG ((DEBUG_INFO, "    NVMe security is set.\n"));
        if (NvmeRtd3TablePtr) {
          NvmeRtd3TablePtr->IsSetNvmePassword = 1;
        }
      }
    }
  }
}

/**
 Update Dsdt Table according to platform specific.

 @param [in, out]   Table               The table to update
 @param [in]        SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update Table Success
*/
STATIC
EFI_STATUS
UpdateDsdt (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  EFI_STATUS                          Status;
  UINT8                               *DsdtPointer;
  UINT32                              *Signature;
  EFI_GLOBAL_NVS_AREA_PROTOCOL        *GlobalNvsAreaProtocol;
  EFI_GLOBAL_NVS_AREA                 *mGlobalNvsArea;
  EFI_OEM_GLOBAL_NVS_AREA             *mOemGlobalNvsArea;
  KERNEL_CONFIGURATION                *SetupNVRamKernel;
  CHIPSET_CONFIGURATION               *SetupNVRamChipset;
  UINT64                              TopMem;
  EFI_EVENT                           Event;
  VOID                                *Registration;
  AMD_PBS_SETUP_OPTION                AmdPbsConfiguration;
//[-start-220208-IB14740208-modify]//
#ifdef REDUCED_HARDWARE_SUPPORT
  UINT64                              PkgLength;
#endif
//[-end-220208-IB14740208-modify]//
  H2O_CP_HANDLE                       CpHandle;

  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, (VOID**)&GlobalNvsAreaProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mGlobalNvsArea    = GlobalNvsAreaProtocol->Area;
  mOemGlobalNvsArea = GlobalNvsAreaProtocol->OemArea;

  SetupNVRamKernel  = (KERNEL_CONFIGURATION *)SetupNVRam;
  SetupNVRamChipset = (CHIPSET_CONFIGURATION *)SetupNVRam;

  //
  // Initial Global NVS Data - Start
  //
  if (AmdPbsConfiguration.ModernStandbyEn == 1) {
    mGlobalNvsArea->DisableAcpiS3 = 0;
  } else {
    if (PcdGetBool (PcdAmdAcpiS3Support) == FALSE) {
      mGlobalNvsArea->DisableAcpiS3 = 0;
    } else { 
      mGlobalNvsArea->DisableAcpiS3 = 1;
    }
  }
  mGlobalNvsArea->amdThermalSensorSelect = SetupNVRamChipset->amdThermalSensorSelect;
  mGlobalNvsArea->amdTNBHystersis = SetupNVRamChipset->amdCpuHysteresisInfo;
  mGlobalNvsArea->amdTCPU0 = SetupNVRamChipset->amdCpuAC1;
  mGlobalNvsArea->amdTCPU1 = SetupNVRamChipset->amdCpuAC0;
  mGlobalNvsArea->amdPwmCPU0 = SetupNVRamChipset->amdCpuAL1;
  mGlobalNvsArea->amdPwmCPU1 = SetupNVRamChipset->amdCpuAL0;
  mGlobalNvsArea->amdLcdPanelType = SetupNVRamChipset->amdLcdPanelType;
  mGlobalNvsArea->WakeOnPME = AmdPbsConfiguration.WakeOnPME;
  mGlobalNvsArea->AmdDptcControl = AmdPbsConfiguration.AmdDptcControl;
  mGlobalNvsArea->LightSensor = AmdPbsConfiguration.amdLightSensor;
  mGlobalNvsArea->amdExternalUSBController = SetupNVRamChipset->amdExternalUSBController;

  mGlobalNvsArea->AcpiThermalZone = SetupNVRamChipset->ThermalFanControl;
  mGlobalNvsArea->PcieBaseAddress = (UINT32) PcdGet64 (PcdPciExpressBaseAddress);
  mGlobalNvsArea->PcieBaseLimit = (UINT32) (PcdGet64 (PcdPciExpressBaseAddress) + ((GetEndBusNumber () + 1) * 0x100000) - 1);
  mGlobalNvsArea->MITTWITTControl = AmdPbsConfiguration.MITTWITTControl;
  mGlobalNvsArea->AmdNfcSensor = AmdPbsConfiguration.NfcSensor;
  mGlobalNvsArea->WakeOnVoiceSupport = AmdPbsConfiguration.WoV;

  if (FeaturePcdGet (PcdH2ONbIoApicSupport)) {
    mGlobalNvsArea->NbIoApic = TRUE;
  } else {
    mGlobalNvsArea->NbIoApic = FALSE;
  }
  mGlobalNvsArea->BoardId = PcdGet16 (OriginalPlatformId);
  mGlobalNvsArea->KbcSupport = AmdPbsConfiguration.KbcSupport;
//[-start-220307-IB14740222-modify]//
  mGlobalNvsArea->EcSupport = 0;
//[-end-220307-IB14740222-modify]//

  TopMem = AsmReadMsr64 (0x0C0010058);
  mOemGlobalNvsArea->PcieBar = (UINT8)(TopMem >> 24);
  mOemGlobalNvsArea->TouchPanel = AmdPbsConfiguration.TouchPanel;
  mOemGlobalNvsArea->TouchPad = AmdPbsConfiguration.TouchPad;
#ifdef THUNDERBOLT_SUPPORT
  mOemGlobalNvsArea->TBSupport           = AmdPbsConfiguration.TBSupport;
  mOemGlobalNvsArea->TBPCIeNativeHotPlug = AmdPbsConfiguration.TBPCIeNativeHotPlug;
#endif
  mOemGlobalNvsArea->SerialDbgMsgUnderOs = AmdPbsConfiguration.SerialDbgMsgUnderOs;
  mOemGlobalNvsArea->M2WlanEn = AmdPbsConfiguration.WlanPowerControl;
  mOemGlobalNvsArea->DtSlotEn = AmdPbsConfiguration.DtSlotPowerControl;
  if (IsPco ()){
    mOemGlobalNvsArea->RV2Identifier = 0;
  } else {
    mOemGlobalNvsArea->RV2Identifier = 1;
  }
  if (IsDualSource ()) {
    mOemGlobalNvsArea->IsDualSource = 1;
  } else {
    mOemGlobalNvsArea->IsDualSource = 0;
  }      
  mGlobalNvsArea->BatteryValue = DEFAULT_BATTERY_VALUE;
  mGlobalNvsArea->IgdStatus = PRIMARY_DISPLAY;
  if (!FeaturePcdGet (PcdH2OLegacyFreeTiSupport)) {
    mGlobalNvsArea->BrightnessLevel = DEFAULT_BRIGHTNESS_LEVEL;
  }
  mGlobalNvsArea->CurrentDispState = (PIPE_A_CRT | (PIPE_B_CRT));
  mGlobalNvsArea->ConnectStandbySupport = 0;
  mGlobalNvsArea->AcpiReduceHwSupport = 0;
//[-start-220126-IB14740206-add]//
  mGlobalNvsArea->Rb0Mem32Base = (UINT32)mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioBase32;
  mGlobalNvsArea->Rb0Mem32Limit = (UINT32)mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioLimit32;
  mGlobalNvsArea->Rb0Mem32BasePrefetchable = (UINT32)mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioBase32;
  mGlobalNvsArea->Rb0Mem32LimitPrefetchable = (UINT32)mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioLimit32;
  mGlobalNvsArea->Rb0Mem64Base = mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioBase64;
  mGlobalNvsArea->Rb0Mem64Limit = mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioLimit64;
//[-start-220525-IB14740243-modify]//
  mGlobalNvsArea->Rb0Mem64Size = 0;
  if (mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioLimit64 > mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioBase64) {
    mGlobalNvsArea->Rb0Mem64Size = (mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioLimit64 - mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].MmioBase64 + 1);
  }
//[-end-220525-IB14740243-modify]//
  mGlobalNvsArea->Rb0Mem64BasePrefetchable = mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioBase64;
  mGlobalNvsArea->Rb0Mem64LimitPrefetchable = mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioLimit64;
//[-start-220525-IB14740243-modify]//
  mGlobalNvsArea->Rb0Mem64SizePrefetchable = 0;
  if (mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioLimit64 > mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioBase64) {
    mGlobalNvsArea->Rb0Mem64SizePrefetchable = (mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioLimit64 - mAmdMcmInfoProtocol->AmdMcmInfoDatas->RbInfo[0].PMmioBase64 + 1);
  }
//[-end-220525-IB14740243-modify]//
//[-end-220126-IB14740206-add]//
  mGlobalNvsArea->ThreadCount = (UINT8) mNumEnabledCPUs;
  if (FeaturePcdGet (PcdModernStandbySupport)) {
    mGlobalNvsArea->ConnectStandbySupport = AmdPbsConfiguration.ModernStandbyEn;
    //
    //  Register a callback for setting mGlobalNvsArea->NvmePwdSet
    //
    if (FeaturePcdGet (PcdH2OBdsCpBootDeviceEnumAfterSupported) && PcdGetBool(PcdModernStandbyEnable)) {
      Status = H2OCpRegisterHandler (
                 &gH2OBdsCpBootDeviceEnumAfterGuid,
                 UpdateNvmePwdStatusToGnvs,
                 H2O_CP_MEDIUM,
                 &CpHandle
                 );
      DEBUG ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpBootDeviceEnumAfterGuid, Status));
    }
  }

  if (FeaturePcdGet (PcdReduceHardwareSupport)) {
    mGlobalNvsArea->AcpiReduceHwSupport = AmdPbsConfiguration.ReduceHardwareEn;
  }

  for (DsdtPointer = (UINT8 *)((UINTN)Table + sizeof (EFI_ACPI_DESCRIPTION_HEADER)); DsdtPointer <= (UINT8 *)((UINTN)Table + (UINTN)(Table->Length)); DsdtPointer++) {
    Signature = (UINT32*) DsdtPointer;

    switch (*Signature) {
    case (SIGNATURE_32 ('_', 'O', 'S', 'C')): // _OSC operation region
      if (SetupNVRamChipset->OscSwitch == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('X', 'O', 'S', 'C');
      }
      break;

    case (SIGNATURE_32 ('P', 'E', 'B', 'A')): // PEBA operation region
      if (IsAmlOpRegionObject (DsdtPointer)) {
      }
      break;

    case (SIGNATURE_32 ('X', 'P', 'E', 'X')): // XPEX operation region
      if (IsAmlOpRegionObject (DsdtPointer)) {
      }
      break;

    case (SIGNATURE_32 ('S', 'T', 'S', 'I')):
      if (IsAmlOpRegionObject (DsdtPointer)) {
      }
      break;

    case (SIGNATURE_32 ('G', 'N', 'V', 'S')):
      if (IsAmlOpRegionObject (DsdtPointer)) {
        SetOpRegion (DsdtPointer, mGlobalNvsArea, sizeof (EFI_GLOBAL_NVS_AREA));
      }
      break;

    case (SIGNATURE_32 ('O', 'G', 'N', 'S')):
      if (IsAmlOpRegionObject (DsdtPointer)) {
        SetOpRegion (DsdtPointer, mOemGlobalNvsArea, sizeof (EFI_OEM_GLOBAL_NVS_AREA));
      }
      break;

    case (SIGNATURE_32 ('F', 'U', '3', 'E')): //xHCI Enable
      break;

    case (SIGNATURE_32 ('B', 'A', 'R', '3')):
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      UpdatePspBar3,
                      NULL,
                      &Event
                      );
      if (!EFI_ERROR (Status)) {
        Status = gBS->RegisterProtocolNotify (
                        &gEfiPciEnumerationCompleteProtocolGuid,
                        Event,
                        &Registration
                        );
      }
      break;

    case (SIGNATURE_32 ('T', 'P', 'R', 'S')):
      UpdateI2cTouchPadResource (DsdtPointer, &AmdPbsConfiguration);
      break;
    case (SIGNATURE_32 ('T', 'P', 'H', 'A')):
      UpdateI2cTouchPadHidAddress (DsdtPointer, &AmdPbsConfiguration);
      break;

    //
    //For Moedern standby and Reduced hardware start
    //
#ifdef REDUCED_HARDWARE_SUPPORT
    case (SIGNATURE_32 ('N', 'P', 'R', 'W')):   // GP19, for wake on xhci2
      // if (AmdPbsConfiguration.WakeOnXhci2 == 0) {
      //   *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'P', 'R', 'W');
      // }
      break;
    case (SIGNATURE_32 ('X', 'P', 'R', 'W')):   // Xhci2, for wake on xhci2
      // if (AmdPbsConfiguration.WakeOnXhci2 == 1) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'P', 'R', 'W');
      // }
      break;
    case (SIGNATURE_32 ('_', 'P', 'R', 'W')):   // HW_Reduced_ACPI related _PRW declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 1) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('R', 'H', 'R', 'W');
      }
      break;
    case (SIGNATURE_32 ('R', 'H', 'R', 'S')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 1) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'C', 'R', 'S');
      }
      break;
    case (SIGNATURE_32 ('R', 'H', 'R', '1')):   // HW_Reduced_ACPI related _CRS declaration
      if ( (AmdPbsConfiguration.ModernStandbyEn == 1) && (IsAmlMethodObject (DsdtPointer, &PkgLength )))  {
         if ( CheckNvmeDevice(1,2) != TRUE ) { //Use Hardcoded value in CPM Table (X4Slot: D1::F2)
            *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'C', 'R', 'S');
         }
      }
      break;
    case (SIGNATURE_32 ('R', 'H', 'R', '2')):   // HW_Reduced_ACPI related _CRS declaration
      if ( (AmdPbsConfiguration.ModernStandbyEn == 1) && (IsAmlMethodObject (DsdtPointer, &PkgLength )))  {
         if ( CheckNvmeDevice(1,3) != TRUE ) { //Use Hardcoded value in CPM Table (X1Slot: D1::F3)
            *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'C', 'R', 'S');
         }
      }
      break;
    case (SIGNATURE_32 ('M', 'P', 'R', 'W')):   // Declare _PRW for GP17 as workaround
      *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'P', 'R', 'W');
      break;
    case (SIGNATURE_32 ('R', 'H', 'G', 'P')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ReduceHardwareEn == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'G', 'P', 'E');
        if (PcdGet8(PcdEmmcEnable) == 0) {
          *((UINT8*)DsdtPointer+5) = 3; //Name (_GPE, 3)
        } else {
          *((UINT8*)DsdtPointer+5) = 9; //Name (_GPE, 9)
        }      
      }
      break;
    case (SIGNATURE_32 ('X', 'L', '0', '8')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'L', '0', '8');
      }
      break;
    case (SIGNATURE_32 ('X', 'L', '0', 'D')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'L', '0', 'D');
      }
      break;
    case (SIGNATURE_32 ('X', 'L', '0', 'E')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'L', '0', 'E');
      }
      break;
    case (SIGNATURE_32 ('X', 'L', '0', 'F')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'L', '0', 'F');
      }
      break;
    case (SIGNATURE_32 ('X', 'L', '1', '9')):   // HW_Reduced_ACPI related _CRS declaration
      if (AmdPbsConfiguration.ModernStandbyEn == 0) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'L', '1', '9');
      }
      break;
    case (SIGNATURE_32 ('X', 'L', '1', 'A')):   // Gpe, for Wake on Xhci2
      // if (AmdPbsConfiguration.ModernStandbyEn == 0 || AmdPbsConfiguration.WakeOnXhci2 == 1) {
        *((UINT32*)DsdtPointer) = SIGNATURE_32 ('_', 'L', '1', 'A');
      // }
      break;
#endif
  
    //
    //For Moedern standby and Reduced hardware end
    //
    default:
      break;
    }
  }
  Status = OemSvcUpdateGlobalNvs (mGlobalNvsArea, mOemGlobalNvsArea);

  return EFI_SUCCESS;
}

/**
 Update Ssdt Table according to platform specific.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update Table Success
*/
STATIC
EFI_STATUS
UpdateSsdt (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  return EFI_SUCCESS;
}

/**
 Update HPET Table according to platform specific.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update Table Success
*/
STATIC
EFI_STATUS
UpdateHpet (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  KERNEL_CONFIGURATION                *SetupNVRamKernel;

  SetupNVRamKernel = (KERNEL_CONFIGURATION *)SetupNVRam;

  //
  // If HPET is disabled in setup, don't publish the table.
  //
  if (!SetupNVRamKernel->HPETSupport) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

UINT8
GetEndBusNumber (
  VOID
  )
{
  UINT8                               Value8;
  UINT64                              MsrData;
  UINT8                               EndBusNumber;

  MsrData = AsmReadMsr64 (0xC0010058);
  Value8 = (UINT8)((MsrData >> 2) & 0x0F);
  EndBusNumber = (UINT8)((1 << Value8) - 1);

  return EndBusNumber;
}

/**
 Update MCFG Table according to platform specific.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update Table Success
*/
STATIC
EFI_STATUS
UpdateMcfg (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE *McfgTbl;
  UINT32                              TableSize;
  UINT32                              Index;
  UINTN                               TableAddress;

  TableSize = ((Table)->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER) - sizeof (UINT64));
  TableSize /= sizeof (EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE);
  TableAddress = (UINTN)(Table);
  TableAddress += sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  TableAddress += sizeof (UINT64);
  for (Index = 0; Index < TableSize; Index++) {
    McfgTbl = (EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE*)(UINTN)TableAddress;
    McfgTbl->BaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
    McfgTbl->EndBusNumber = GetEndBusNumber ();
    TableAddress += sizeof (EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
RemoveAcpitable (
  UINT32       Signature
  )
{
  EFI_STATUS                              Status;
  EFI_ACPI_SUPPORT_PROTOCOL               *AcpiSupport;
  INTN                                    Index;
  UINTN                                   Handle;
  EFI_ACPI_TABLE_VERSION                  Version;
  EFI_ACPI_DESCRIPTION_HEADER             *Table;

  //
  // Found  table
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiSupportProtocolGuid,
                  NULL,
                  (VOID**)&AcpiSupport
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Index  = 0;
  Handle = 0;
  do {
    Table = NULL;
    Status = AcpiSupport->GetAcpiTable (
                            AcpiSupport,
                            Index,
                            (VOID**)&Table,
                            &Version,
                            &Handle
                            );
    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // Check Signture and update table
    //
    if (Table->Signature == Signature) {
      //
      // Delete the table
      //
      gBS->FreePool (Table);
      Table = NULL;
      Status = AcpiSupport->SetAcpiTable (
                              AcpiSupport,
                              Table,
                              TRUE,
                              Version,
                              &Handle
                              );
      return Status;
    }
    //
    // Not found, get next
    //
    gBS->FreePool (Table);
    Index++;
  } while (TRUE);

  return Status;
}

/**
  Remove WDRT TABLE

  @param[in]        Event               Event whose notification function is being invoked.
  @param[in]        Context             Pointer to the notification function's context.

*/
VOID
EFIAPI
RemoveWdrtEventNotify (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                              Status;

  Status = RemoveAcpitable (EFI_ACPI_3_0_WATCHDOG_RESOURCE_TABLE_SIGNATURE);
  gBS->CloseEvent (Event);
}

/**
  Remove WDAT TABLE

  @param[in]        Event               Event whose notification function is being invoked.
  @param[in]        Context             Pointer to the notification function's context.

*/
VOID
EFIAPI
RemoveWdatEventNotify (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                              Status;

  Status = RemoveAcpitable (EFI_ACPI_3_0_WATCHDOG_ACTION_TABLE_SIGNATURE);
  gBS->CloseEvent (Event);
}

STATIC
VOID
GetWatchDogBase (
  OUT UINT32                          *WatchDogBase
  )
{
  *WatchDogBase = PcdGet32 (PcdH2OWatchdogTimerBase);
}

/**
 Update Wdrt Table according to platform specific.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update Table Success
*/
STATIC
EFI_STATUS
UpdateWdrt (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  EFI_STATUS                              Status;
  EFI_ACPI_WATCHDOG_RESOURCE_1_0_TABLE    *WdrtPointer;
  UINT32                                  ControlRegisterAddress;
  EFI_EVENT                               RemoveWdrtEvent;

  Status = EFI_SUCCESS;
  if (MmioRead32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00) & BIT7) {
    GetWatchDogBase (&ControlRegisterAddress);

    WdrtPointer = (EFI_ACPI_WATCHDOG_RESOURCE_1_0_TABLE*) Table;
    WdrtPointer->ControlRegisterAddress.Address = ControlRegisterAddress;
    WdrtPointer->CountRegisterAddress.Address   = ControlRegisterAddress + 4;
  } else {
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    RemoveWdrtEventNotify,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &RemoveWdrtEvent
                    );
  }

  return Status;
}

/**
 Update Wdat Table according to platform specific.

 @param[in, out]    Table               The table to update
 @param[in]         SetupNVRam          SETUP Variable pointer

 @retval            EFI_SUCCESS         Update Table Success
*/
STATIC
EFI_STATUS
UpdateWdat (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Table,
  IN UINT8                            *SetupNVRam
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               RemoveWdatEvent;
  
  Status = EFI_SUCCESS;
  if (!(MmioRead32 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG00) & BIT7)) {
     Status = gBS->CreateEventEx (
                     EVT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     RemoveWdatEventNotify,
                     NULL,
                     &gEfiEventReadyToBootGuid,
                     &RemoveWdatEvent
                     );
  }

  return Status;
}

/**
 Update ACPI table content according to platform specific

 @param[in, out]    TableHeader             Pointer of the table to update
 @param[in, out]    CommonCodeReturnStatus  Return Status from Common Code
*/
VOID
UpdateAcpiTable (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER      *TableHeader,
  IN OUT EFI_STATUS                       *CommonCodeReturnStatus
  )
{
  UINT8                                   *SetupNVRam;
//[-start-220126-IB14740206-add]//
  EFI_STATUS                              Status;
//[-end-220126-IB14740206-add]//

  SetupNVRam = NULL;

  SetupNVRam = CommonGetVariableData (SETUP_VARIABLE_NAME, &gSystemConfigurationGuid);
  if (SetupNVRam == NULL) {
    return;
  }

//[-start-220126-IB14740206-add]//
  Status = gBS->LocateProtocol (&gAmdMcmInfoProtocolGuid, NULL, (VOID **)&mAmdMcmInfoProtocol);
//[-end-220126-IB14740206-add]//

  switch ((TableHeader)->Signature) {

  case EFI_ACPI_3_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateMadt (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateFacp (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateDsdt (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateSsdt (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateHpet (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateMcfg (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_WATCHDOG_RESOURCE_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateWdrt (TableHeader, SetupNVRam);
    break;

  case EFI_ACPI_3_0_WATCHDOG_ACTION_TABLE_SIGNATURE:
    *CommonCodeReturnStatus = UpdateWdat (TableHeader, SetupNVRam);
    break;
  }
}
