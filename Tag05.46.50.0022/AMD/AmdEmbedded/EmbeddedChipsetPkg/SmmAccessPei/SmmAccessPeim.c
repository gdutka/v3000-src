/** @file
  This is the driver that publishes the SMM Access Ppi
  instance.

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

#include "SmmAccessPeim.h"
#include <AGESA.h>
#include <Library/ChipsetConfigLib.h>
#include <ChipsetSetupConfig.h>
#include <Guid/AcpiS3Context.h>
#include <Ppi/MasterBootMode.h>

static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMasterBootModePpiGuid,
    MasterBootModeNotifyCallback
  }
};

/**
  CpuOpenSMRAM - read/write A0000-BFFFF

  @param         VOID                  None.

  @retval        VOID                  None.
**/
VOID
EFIAPI
CpuOpenSMRAM (
  VOID
  )
{
  UINT64          Data64;

  //
  // Clear ASeg and TSeg Valid
  //
  Data64 = AsmReadMsr64 (0xC0010113); //SMM_MASKHLR
  Data64 &= (~BIT0); //SMMAValid
  Data64 &= (~BIT1); //SMMTValid
  AsmWriteMsr64 (0xC0010113, Data64);

  //
  // Enable FixMtrr mode
  //
  AsmWriteMsr64 (SYS_CFG, (AsmReadMsr64 (SYS_CFG) | (1 << 19)));

  //
  // Enable read/write from A0000
  //
  Data64 = AsmReadMsr64 (AMD_AP_MTRR_FIX16k_A0000);
  Data64 |= 0x1010101010101010;
  Data64 |= 0x0808080808080808;
  AsmWriteMsr64 (AMD_AP_MTRR_FIX16k_A0000, Data64);

  //
  // Disable FixMtrr mode
  //
  AsmWriteMsr64 (SYS_CFG, (AsmReadMsr64 (SYS_CFG) & (~(1 << 19))));
}

/**
  CpuSmramWP - write protect from A0000-BFFFF

  @param         VOID                  None.

  @retval        VOID                  None.
**/
VOID
EFIAPI
CpuSmramWP (
  VOID
  )
{
  UINT64          Data64;

  //
  // Enable FixMtrr mode
  //
  AsmWriteMsr64 (SYS_CFG, (AsmReadMsr64 (SYS_CFG) | (1 << 19)));

  //
  // Disable read/write from A0000
  //
  Data64 = AsmReadMsr64 (AMD_AP_MTRR_FIX16k_A0000);
  Data64 &= 0xEFEFEFEFEFEFEFEF;
  Data64 &= 0xF7F7F7F7F7F7F7F7;
  AsmWriteMsr64 (AMD_AP_MTRR_FIX16k_A0000, Data64);

  //
  // Disable FixMtrr mode
  //
  AsmWriteMsr64 (SYS_CFG, (AsmReadMsr64 (SYS_CFG) & (~(1 << 19))));

  //
  // Now it is leave SMI, we need to set SMMAValid and SMMTValid bit in SMM_MASKHLR
  //
  Data64 = AsmReadMsr64 (0xC0010113); //SMM_MASKHLR
  Data64 |= BIT0; //SMMAValid
  Data64 |= BIT1; //SMMTValid
  AsmWriteMsr64 (0xC0010113, Data64);
}

/**
  Setting the bit0 of MSRC001_0015 Hardware Configuration (HWCR) to do SMM code lock.

  @param         VOID                  None.

  @retval        VOID                  None.
**/
VOID
EFIAPI
DoSmmLock (
  VOID
  )
{
  UINT64          Data64;

  Data64 = AsmReadMsr64 (HWCR);
  Data64 |= BIT0; //SMM_LOCK
  AsmWriteMsr64 (HWCR, Data64);
}

/**
  Opens the SMRAM area to be accessible by a PEIM driver.

  This function "opens" SMRAM so that it is visible while not inside of SMM. The function should
  return EFI_UNSUPPORTED if the hardware does not support hiding of SMRAM. The function
  should return EFI_DEVICE_ERROR if the SMRAM configuration is locked.

  @param  PeiServices            General purpose services available to every PEIM.
  @param  This                   The pointer to the SMM Access Interface.
  @param  DescriptorIndex        The region of SMRAM to Open.

  @retval EFI_SUCCESS            The region was successfully opened.
  @retval EFI_DEVICE_ERROR       The region could not be opened because locked by chipset.
  @retval EFI_INVALID_PARAMETER  The descriptor index was out of bounds.

**/
EFI_STATUS
EFIAPI
Open (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  )
{
  SMM_ACCESS_PRIVATE_DATA    *SmmAccess;

  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (DescriptorIndex >= SmmAccess->NumberRegions) {
    DEBUG ((EFI_D_WARN, "SMRAM region out of range\n"));
    return EFI_INVALID_PARAMETER;
  } else if (SmmAccess->SmramDesc[DescriptorIndex].RegionState & EFI_SMRAM_LOCKED) {
    //
    // Cannot open a "locked" region
    //
    DEBUG ((EFI_D_WARN, "Cannot open a locked SMRAM region\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // To open SMRAM.
  //
  CpuOpenSMRAM ();

  SmmAccess->SmramDesc[DescriptorIndex].RegionState &= ~(EFI_SMRAM_CLOSED | EFI_ALLOCATED);
  SmmAccess->SmramDesc[DescriptorIndex].RegionState |= EFI_SMRAM_OPEN;
  SmmAccess->SmmAccess.OpenState = TRUE;

  return EFI_SUCCESS;
}

/**
  Inhibits access to the SMRAM.

  This function "closes" SMRAM so that it is not visible while outside of SMM. The function should
  return EFI_UNSUPPORTED if the hardware does not support hiding of SMRAM.

  @param  PeiServices              General purpose services available to every PEIM.
  @param  This                     The pointer to the SMM Access Interface.
  @param  DescriptorIndex          The region of SMRAM to Close.

  @retval EFI_SUCCESS              The region was successfully closed.
  @retval EFI_DEVICE_ERROR         The region could not be closed because locked by chipset.
  @retval EFI_INVALID_PARAMETER    The descriptor index was out of bounds.

**/
EFI_STATUS
EFIAPI
Close (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  )
{
  SMM_ACCESS_PRIVATE_DATA    *SmmAccess;
  BOOLEAN                    OpenState;
  UINT8                      Index;

  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (DescriptorIndex >= SmmAccess->NumberRegions) {
    DEBUG ((EFI_D_WARN, "SMRAM region out of range\n"));
    return EFI_INVALID_PARAMETER;
  } else if (SmmAccess->SmramDesc[DescriptorIndex].RegionState & EFI_SMRAM_LOCKED) {
    //
    // Cannot close a "locked" region
    //
    DEBUG ((EFI_D_WARN, "Cannot close a locked SMRAM region\n"));
    return EFI_DEVICE_ERROR;
  }

  if (SmmAccess->SmramDesc[DescriptorIndex].RegionState & EFI_SMRAM_CLOSED) {
    return EFI_DEVICE_ERROR;
  }

  CpuSmramWP ();

  SmmAccess->SmramDesc[DescriptorIndex].RegionState &= ~EFI_SMRAM_OPEN;
  SmmAccess->SmramDesc[DescriptorIndex].RegionState |= (EFI_SMRAM_CLOSED | EFI_ALLOCATED);

  //
  // Find out if any regions are still open
  //
  OpenState = FALSE;
  for (Index = 0; Index < SmmAccess->NumberRegions; Index++) {
    if ((SmmAccess->SmramDesc[Index].RegionState & EFI_SMRAM_OPEN) == EFI_SMRAM_OPEN) {
      OpenState = TRUE;
    }
  }

  SmmAccess->SmmAccess.OpenState = OpenState;
  return EFI_SUCCESS;
}

/**
  Inhibits access to the SMRAM.

  This function prohibits access to the SMRAM region.  This function is usually implemented such
  that it is a write-once operation.

  @param  PeiServices            General purpose services available to every PEIM.
  @param  This                   The pointer to the SMM Access Interface.
  @param  DescriptorIndex        The region of SMRAM to Close.

  @retval EFI_SUCCESS            The region was successfully locked.
  @retval EFI_DEVICE_ERROR       The region could not be locked because at least
                                 one range is still open.
  @retval EFI_INVALID_PARAMETER  The descriptor index was out of bounds.

**/
EFI_STATUS
EFIAPI
Lock (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  )
{
  SMM_ACCESS_PRIVATE_DATA    *SmmAccess;
  CHIPSET_CONFIGURATION      ChipsetConfiguration;
  SmmAccess = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);

  if (DescriptorIndex >= SmmAccess->NumberRegions) {
    DEBUG ((EFI_D_WARN, "SMRAM region out of range\n"));
    return EFI_INVALID_PARAMETER;
  } else if (SmmAccess->SmmAccess.OpenState) {
    DEBUG ((EFI_D_WARN, "Cannot lock SMRAM when SMRAM regions are still open\n"));
    return EFI_DEVICE_ERROR;
  }
  GetChipsetConfiguration(&ChipsetConfiguration);
  if(ChipsetConfiguration.SMMCoreLock == 1){
    DoSmmLock ();

    SmmAccess->SmramDesc[DescriptorIndex].RegionState |= EFI_SMRAM_LOCKED;
    SmmAccess->SmmAccess.LockState = TRUE;
  }
  return EFI_SUCCESS;
}

/**
  Queries the memory controller for the possible regions that will support SMRAM.

  @param PeiServices            General purpose services available to every PEIM.
  @param This                   The pointer to the SmmAccessPpi Interface.
  @param SmramMapSize           The pointer to the variable containing size of the
                                buffer to contain the description information.
  @param SmramMap               The buffer containing the data describing the Smram
                                region descriptors.

  @retval EFI_BUFFER_TOO_SMALL  The user did not provide a sufficient buffer.
  @retval EFI_SUCCESS           The user provided a sufficiently-sized buffer.

**/
EFI_STATUS
EFIAPI
GetCapabilities (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  )
{
  EFI_STATUS                Status;
  SMM_ACCESS_PRIVATE_DATA   *SmmAccess;
  UINTN                     NecessaryBufferSize;

  SmmAccess           = SMM_ACCESS_PRIVATE_DATA_FROM_THIS (This);
  NecessaryBufferSize = SmmAccess->NumberRegions * sizeof (EFI_SMRAM_DESCRIPTOR);

  if (*SmramMapSize < NecessaryBufferSize) {
    DEBUG ((EFI_D_WARN, "SMRAM Map Buffer too small\n"));
    Status = EFI_BUFFER_TOO_SMALL;
  } else {
    CopyMem (SmramMap, SmmAccess->SmramDesc, NecessaryBufferSize);
    Status = EFI_SUCCESS;
  }

  *SmramMapSize = NecessaryBufferSize;
  return Status;
}

/**
  This routine will create AcpiVariable hob to point the reserved smram in S3 phase
  for PI SMM core.

  @retval EFI_SUCCESS           The gEfiAcpiVariableGuid is created successfully.
  @retval EFI_NOT_FOUND         The gEfiAcpiVariableGuid is not found.

**/
EFI_STATUS
EFIAPI
CreateAcpiVariableHob (
  VOID
  )
{
  EFI_SMRAM_DESCRIPTOR             *SmramDescriptor;
  UINT64                            TsegBase;

  DEBUG ((EFI_D_INFO, "CreateAcpiVariableHob\n"));
  //
  // Retrieve the GUID HOB data that contains the set of SMRAM descriptors
  //
  SmramDescriptor = BuildGuidHob (
                      &gEfiAcpiVariableGuid,
                      sizeof (EFI_SMRAM_DESCRIPTOR)
                      );

  if (SmramDescriptor == NULL){
    DEBUG ((EFI_D_INFO, "BuildGuidHob Fail\n"));
    return EFI_NOT_FOUND;
  }

  TsegBase = AsmReadMsr64 (0xC0010112) & 0xFFFFFFFE0000ul;

  SmramDescriptor->PhysicalStart = TsegBase;
  SmramDescriptor->CpuStart      = TsegBase;
  SmramDescriptor->PhysicalSize  = EFI_PAGE_SIZE; // It must 4KB Size in AcpiVariableHobOnSmramReserveHobThunk
  SmramDescriptor->RegionState   = EFI_ALLOCATED | EFI_SMRAM_CLOSED | EFI_CACHEABLE; // It must sync attribute in AcpiVariableHobOnSmramReserveHobThunk

  DEBUG ((EFI_D_ERROR, "[%a] AcpiVariable PhysicalStart : %lx\n", __FUNCTION__, SmramDescriptor->PhysicalStart));
  DEBUG ((EFI_D_ERROR, "[%a] AcpiVariable CpuStart      : %lx\n", __FUNCTION__, SmramDescriptor->CpuStart     ));
  DEBUG ((EFI_D_ERROR, "[%a] AcpiVariable PhysicalSize  : %lx\n", __FUNCTION__, SmramDescriptor->PhysicalSize ));
  DEBUG ((EFI_D_ERROR, "[%a] AcpiVariable RegionState   : %lx\n", __FUNCTION__, SmramDescriptor->RegionState  ));

  return EFI_SUCCESS;
}


/**
  This routine will create AcpiVariable hob to point the reserved smram in S3 phase
  for PI SMM core after MasterBootMode is set.

  @param PeiServices            General purpose services available to every PEIM.
  @param NotifyDescriptor       Pointer to EFI_PEI_NOTIFY_DESCRIPTOR
  @param Ppi                    EFI_PEI_FV_FILE_LOADER_PPI

  @retval   EFI_SUCCESS         if the interface could be successfully installed.

**/
EFI_STATUS
EFIAPI
MasterBootModeNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                      Status;
  EFI_BOOT_MODE                   BootMode;
  UINTN                           Index;
  EFI_PEI_PPI_DESCRIPTOR          *PpiList;
  SMM_ACCESS_PRIVATE_DATA         *SmmAccessPrivate;
  UINT64                          TsegBase;
  UINT64                          TsegSize;

  DEBUG ((EFI_D_ERROR, "[%a] Entry\n", __FUNCTION__));

  Status = (*PeiServices)->GetBootMode (PeiServices, &BootMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[%a] Exit - GetBootMode fail\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // If not in S3 boot path. do nothing
    //
    DEBUG ((EFI_D_ERROR, "[%a] Exit - BootMode != BOOT_ON_S3_RESUME\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  Status = CreateAcpiVariableHob ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[%a] Exit - CreateAcpiVariableHob %r \n", __FUNCTION__, Status));
    return EFI_NOT_STARTED;
  }

  //
  // Initialize private data
  //
  SmmAccessPrivate = AllocateZeroPool (sizeof (*SmmAccessPrivate));
  PpiList          = AllocateZeroPool (sizeof (*PpiList));
  if ((SmmAccessPrivate == NULL) || (PpiList == NULL)) {
    DEBUG ((EFI_D_ERROR, "[%a] Exit - AllocateZeroPool fail\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SmmAccessPrivate->Signature = SMM_ACCESS_PRIVATE_DATA_SIGNATURE;
  SmmAccessPrivate->Handle    = NULL;

  //
  // Alloc space for SmmAccessPrivate->SmramDesc
  //
  SmmAccessPrivate->SmramDesc = AllocateZeroPool ((1) * sizeof (EFI_SMRAM_DESCRIPTOR)); // We only implement the full SMRAM area for lock and unlock by using TSEG in S3.
  if (SmmAccessPrivate->SmramDesc == NULL) {
    DEBUG ((EFI_D_ERROR, "[%a] Exit - AllocateZeroPool for SmmAccessPrivate data fail\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  TsegBase = AsmReadMsr64 (0xC0010112) & 0xFFFFFFFE0000ul;
  TsegSize = (UINT64)FixedPcdGet32 (PcdH2OTsegSize);

  SmmAccessPrivate->SmramDesc[0].PhysicalStart = TsegBase;
  SmmAccessPrivate->SmramDesc[0].CpuStart      = TsegBase;
  SmmAccessPrivate->SmramDesc[0].PhysicalSize  = TsegSize;
  SmmAccessPrivate->SmramDesc[0].RegionState   = EFI_SMRAM_CLOSED | EFI_CACHEABLE;
  DEBUG ((EFI_D_ERROR, "[%a] SmmAccessPrivate PhysicalStart: %lx\n", SmmAccessPrivate->SmramDesc[0].PhysicalStart));
  DEBUG ((EFI_D_ERROR, "[%a] SmmAccessPrivate CpuStart     : %lx\n", SmmAccessPrivate->SmramDesc[0].CpuStart ));
  DEBUG ((EFI_D_ERROR, "[%a] SmmAccessPrivate PhysicalSize : %lx\n", SmmAccessPrivate->SmramDesc[0].PhysicalSize));
  DEBUG ((EFI_D_ERROR, "[%a] SmmAccessPrivate RegionState  : %lx\n", SmmAccessPrivate->SmramDesc[0].RegionState));

  Index = 1;

  SmmAccessPrivate->NumberRegions             = Index;
  SmmAccessPrivate->SmmAccess.Open            = Open;
  SmmAccessPrivate->SmmAccess.Close           = Close;
  SmmAccessPrivate->SmmAccess.Lock            = Lock;
  SmmAccessPrivate->SmmAccess.GetCapabilities = GetCapabilities;
  SmmAccessPrivate->SmmAccess.LockState       = FALSE;
  SmmAccessPrivate->SmmAccess.OpenState       = FALSE;

  //
  // Install PPI
  //
  PpiList->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiList->Guid  = &gPeiSmmAccessPpiGuid;
  PpiList->Ppi   = &SmmAccessPrivate->SmmAccess;

  Status         = PeiServicesInstallPpi (PpiList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_ERROR, "[%a] Exit\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Main entry for the SMM Access PEIM.

  This routine is to install PEI_SMM_ACCESS_PPI.

  @param  FileHandle            Handle of the file being invoked.
  @param  PeiServices           Pointer to PEI Services table.

  @retval EFI_SUCCESS           Returns EFI_SUCCESS by default.

**/
EFI_STATUS
EFIAPI
SmmAccessPeimEntryPoint (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                    Status;

  //
  // Notify NotifyPpiList
  //
  Status = PeiServicesNotifyPpi (mNotifyPpiList);
  DEBUG ((EFI_D_ERROR, "[%a] PeiServicesNotifyPpi %r\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}