/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains implementation for the SMM Access Ppi
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision:  $   @e \$Date:  $
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <PiDxe.h>
#include <Filecode.h>
#include <IdsHookId.h>
#include <cpuRegisters.h>
#include <Ppi/SmmAccess.h>
#include <Ppi/MpServices.h>
#include <Library/CcxPeiSmmAccessLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IdsLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Guid/SmramMemoryReserve.h>
#include <Library/AmdPspBaseLibV2.h>

#include <Library/IdsLib.h>

#define FILECODE LIBRARY_PEICCXSMMACCESSLIB_PEICCXSMMACCESSLIB_FILECODE

typedef struct {
  UINT32                    Signature;
  PEI_SMM_ACCESS_PPI        SmmAccess;
  EFI_SMRAM_DESCRIPTOR      *SmramMap;
  EFI_PEI_MP_SERVICES_PPI   *MpServices;
  UINTN                     NumberOfRegions;
  BOOLEAN                   SetSmmLock;
} PEI_SMM_ACCESS_INSTANCE;

#define PEI_SMM_ACCESS_INSTANCE_SIGNATURE     SIGNATURE_32('p', 's', 'a', '0')
#define PEI_SMM_ACCESS_INSTANCE_FROM_THIS(a)  CR (a, PEI_SMM_ACCESS_INSTANCE, SmmAccess, PEI_SMM_ACCESS_INSTANCE_SIGNATURE)

PEI_SMM_ACCESS_INSTANCE mPeiSmmAccessPrivateTemplate = {
  PEI_SMM_ACCESS_INSTANCE_SIGNATURE,
  {
    CcxSmmOpen,
    CcxSmmClose,
    CcxSmmLock,
    CcxSmmGetCapabilities,
    FALSE,
    FALSE
  },
  NULL,
  NULL,
  0,
  FALSE
};

/*---------------------------------------------------------------------------------------*/
/**
 *  Helper routine to open SMRAM on cores
 *
 *  @param[in]     Unused                  Unused
 *
 */
VOID
EFIAPI
OpenSmramOnCore (
  IN       VOID *Unused
 )
{
  if (CcxIsComputeUnitPrimary (NULL)) {
    // Disable protection in ASeg and TSeg
    AsmMsrAnd64 (SMMMASK_ADDRESS, ~((UINT64) BIT1 | (UINT64) BIT0));

    // Enable FixMtrrModEn
    AsmMsrOr64 (SYS_CFG, LShiftU64(1, 19));

    // Enable Rd/Wr DRAM in ASeg
    AsmMsrOr64 (AMD_MTRR_FIX16k_A0000, 0x1818181818181818);

    // Disable FixMtrrModEn
    AsmMsrAnd64 (SYS_CFG, ~(LShiftU64 (1, 19)));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Helper routine to close SMRAM on cores
 *
 *  @param[in]     Unused                  Unused
 *
 */
VOID
EFIAPI
CloseSmramOnCore (
  IN       VOID *Unused
 )
{
  if (CcxIsComputeUnitPrimary (NULL)) {
    // Disable protection in ASeg and TSeg
    AsmMsrAnd64 (SMMMASK_ADDRESS, ~((UINT64) BIT1 | (UINT64) BIT0));

    // Enable FixMtrrModEn
    AsmMsrOr64 (SYS_CFG, LShiftU64 (1, 19));

    // Disable Rd/Wr DRAM in ASeg
    AsmMsrAnd64 (AMD_MTRR_FIX16k_A0000, 0xE7E7E7E7E7E7E7E7);

    // Disable FixMtrrModEn
    AsmMsrAnd64 (SYS_CFG, ~(LShiftU64 (1, 19)));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Helper routine to lock SMRAM on cores
 *
 *  @param[in]     PEI_SMM_ACCESS_INSTANCE
 *
 */
VOID
EFIAPI
LockSmramOnCore (
  IN PEI_SMM_ACCESS_INSTANCE *PeiSmmAccessInstance
 )
{
  AsmWbinvd ();

  AsmMsrOr64 (SMMMASK_ADDRESS, (BIT1 | BIT0));

  if (PeiSmmAccessInstance->SetSmmLock) {
    AsmMsrOr64 (MSR_HWCR, SMMLOCK|SMMBASELOCK);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Opens the SMRAM area to be accessible by a boot-service driver
 *
 *  @param[in]     This                  The EFI_SMM_ACCESS2_PROTOCOL instance
 *
 *  @retval        EFI_SUCCESS           The operation was successful
 *  @retval        EFI_DEVICE_ERROR      SMRAM cannot be opened, perhaps because it is locked
 *  @retval        EFI_INVALID_PARAMETER Invalid Descriptor
 *  @retval        EFI_UNSUPPORTED       The system does not support opening and closing of SMRAM
 */
EFI_STATUS
EFIAPI
CcxSmmOpen (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  )
{
  PEI_SMM_ACCESS_INSTANCE *PeiSmmAccessInstance = PEI_SMM_ACCESS_INSTANCE_FROM_THIS (This);

  if ((PeiSmmAccessInstance->SmmAccess.LockState) || (PeiSmmAccessInstance->MpServices == NULL)) {
    return EFI_DEVICE_ERROR;
  }

  if (DescriptorIndex >= PeiSmmAccessInstance->NumberOfRegions) {
    return EFI_INVALID_PARAMETER;
  }

   PeiSmmAccessInstance->SmramMap[DescriptorIndex].RegionState &= ~(EFI_SMRAM_CLOSED | EFI_ALLOCATED);
   PeiSmmAccessInstance->SmramMap[DescriptorIndex].RegionState |= EFI_SMRAM_OPEN;

  // Open SMM if any one region is opened.
  if (PeiSmmAccessInstance->SmmAccess.OpenState) {
    return EFI_SUCCESS;
  }

  AcquirePspSmiRegMutexV2 ();
  OpenSmramOnCore (NULL);
  PeiSmmAccessInstance->MpServices->StartupAllAPs (
                PeiServices,
                PeiSmmAccessInstance->MpServices,
                OpenSmramOnCore,
                FALSE,
                0,
                NULL
                );

  ReleasePspSmiRegMutexV2 ();

  PeiSmmAccessInstance->SmmAccess.OpenState = TRUE;

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Inhibits access to the SMRAM.
 *
 *  @param[in]     This                  The EFI_SMM_ACCESS2_PROTOCOL instance
 *
 *  @retval        EFI_SUCCESS           The operation was successful
 *  @retval        EFI_DEVICE_ERROR      SMRAM cannot be closed
 *  @retval        EFI_INVALID_PARAMETER Invalid Descriptor
 *  @retval        EFI_UNSUPPORTED       The system does not support opening and closing of SMRAM
 */
EFI_STATUS
EFIAPI
CcxSmmClose (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  )
{
  UINTN Index;

  PEI_SMM_ACCESS_INSTANCE *PeiSmmAccessInstance = PEI_SMM_ACCESS_INSTANCE_FROM_THIS (This);

  if (PeiSmmAccessInstance->SmmAccess.LockState) {
    return EFI_DEVICE_ERROR;
  }

  if (DescriptorIndex >= PeiSmmAccessInstance->NumberOfRegions) {
    return EFI_INVALID_PARAMETER;
  }

  PeiSmmAccessInstance->SmramMap[DescriptorIndex].RegionState &= ~EFI_SMRAM_OPEN;
  PeiSmmAccessInstance->SmramMap[DescriptorIndex].RegionState |= (EFI_SMRAM_CLOSED | EFI_ALLOCATED);

  // Only close SMM when all regions are closed.
  for (Index = 0; Index < PeiSmmAccessInstance->NumberOfRegions; Index++) {
    if (PeiSmmAccessInstance->SmramMap[Index].RegionState & EFI_SMRAM_OPEN) {
      return EFI_SUCCESS;
    }
  }

  AcquirePspSmiRegMutexV2 ();
  CloseSmramOnCore (NULL);
  PeiSmmAccessInstance->MpServices->StartupAllAPs (
                PeiServices,
                PeiSmmAccessInstance->MpServices,
                CloseSmramOnCore,
                FALSE,
                0,
                NULL
                );

  ReleasePspSmiRegMutexV2 ();

  PeiSmmAccessInstance->SmmAccess.OpenState = FALSE;

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Inhibits access to the SMRAM
 *
 *  @param[in]     This                  The EFI_SMM_ACCESS2_PROTOCOL instance
 *
 *  @retval        EFI_SUCCESS           The device was successfully locked
 *  @retval        EFI_INVALID_PARAMETER Invalid Descriptor
 *  @retval        EFI_UNSUPPORTED       The system does not support locking of SMRAM
 */
EFI_STATUS
EFIAPI
CcxSmmLock (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN UINTN                           DescriptorIndex
  )
{
  UINTN     Index;
  UINT32    *SMIx98;
  UINT64    FchAddress;

  PEI_SMM_ACCESS_INSTANCE *PeiSmmAccessInstance = PEI_SMM_ACCESS_INSTANCE_FROM_THIS (This);

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxSmmLock: LOCKING SMM\n");

  if (PeiSmmAccessInstance->SmmAccess.OpenState) {
    return EFI_DEVICE_ERROR;
  }

  if (PeiSmmAccessInstance->SmmAccess.LockState) {
    return EFI_SUCCESS;
  }

  if (DescriptorIndex >= PeiSmmAccessInstance->NumberOfRegions) {
    return EFI_INVALID_PARAMETER;
  }

  PeiSmmAccessInstance->SmramMap[DescriptorIndex].RegionState |= EFI_SMRAM_LOCKED;

  // Lock if all regions locked.
  for (Index = 0; Index < PeiSmmAccessInstance->NumberOfRegions; Index++) {
    if ((PeiSmmAccessInstance->SmramMap[Index].RegionState & EFI_SMRAM_LOCKED) == 0) {
      return EFI_SUCCESS;
    }
  }

  /// @todo Use FCH library to enable/disable SMI
  // Disable SMI generation
  FchAddress = FCH_MMIO_ADDRESS + SMI_REGISTER_SPACE + SMI_TRIG0_OFFSET;
  SMIx98 = (UINT32 *) FchAddress;
  *SMIx98 |= SMIENB;

  AcquirePspSmiRegMutexV2 ();
  PeiSmmAccessInstance->MpServices->StartupAllAPs (
                PeiServices,
                PeiSmmAccessInstance->MpServices,
                LockSmramOnCore,
                FALSE,
                0,
                PeiSmmAccessInstance
                );
  ReleasePspSmiRegMutexV2 ();

  LockSmramOnCore (PeiSmmAccessInstance);

  PeiSmmAccessInstance->SmmAccess.LockState = TRUE;

  // Re-enable SMI generation
  *SMIx98 &= ~SMIENB;

  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Queries the memory controller for the regions that will support SMRAM
 *
 *  @param[in]       This                The EFI_SMM_ACCESS2_PROTOCOL instance
 *  @param[in, out]  SmramMapSize        A pointer to the size, in bytes, of the SmramMemoryMap
 *                                       buffer. On input, this value is the size of the buffer
 *                                       that is allocated by the caller. On output, it is the
 *                                       size of the buffer that was returned if the buffer was
 *                                       large enough, or, if the buffer was too small, the size
 *                                       of the buffer that is needed to contain the map
 *  @param[in, out]  SmramMap            A pointer to the buffer in which firmware places the
 *                                       current memory map. The map is an array of EFI_SMRAM_DESCRIPTORs
 *
 *  @retval        EFI_SUCCESS           The operation was successful
 *  @retval        EFI_BUFFER_TOO_SMALL  SmramMapSize buffer is too small
 */
EFI_STATUS
EFIAPI
CcxSmmGetCapabilities (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN PEI_SMM_ACCESS_PPI              *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  )
{
  UINTN Size;
  EFI_STATUS Status;

  PEI_SMM_ACCESS_INSTANCE *PeiSmmAccessInstance = PEI_SMM_ACCESS_INSTANCE_FROM_THIS (This);

  Size = PeiSmmAccessInstance->NumberOfRegions * sizeof (EFI_SMRAM_DESCRIPTOR);
  Status = EFI_SUCCESS;

  if ((*SmramMapSize >= Size) && (SmramMap != NULL)) {
    CopyMem (SmramMap, PeiSmmAccessInstance->SmramMap, Size);
  } else {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxSmmGetCapabilities: SmramMapSize is too small!\n");
    Status = EFI_BUFFER_TOO_SMALL;
  }

  *SmramMapSize = Size;

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Helper routine to program TsegBase and TsegMask
 *
 *  @param[in]     TsegConfig                  Tseg configuration data
 *
 */
VOID
EFIAPI
SetupSmmRegs (
  IN       CCX_SMM_TSEG_CONFIG  *TsegConfig
  )
{
  if (CcxIsComputeUnitPrimary (NULL)) {
    // Setup TSegBase and TSegMask
    AsmWriteMsr64 (SMMADDR_ADDRESS, TsegConfig->TsegBase);
    AsmWriteMsr64 (SMMMASK_ADDRESS, TsegConfig->TsegMask);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  This library produces the SMM Access2 Protocol.
 *
 *
 *  @retval        EFI_SUCCESS           SMM Access2 Protocol installed successfully
 *  @retval        EFI_NOT_FOUND         SMM Access2 Protocol installation failed
 */
EFI_STATUS
EFIAPI
CcxSmmAccessPpiInstall (
    IN EFI_PEI_SERVICES                **PeiServices
  )
{
  UINTN                          Index;
  UINT64                         TotalSize;
  EFI_STATUS                     Status;
  EFI_HOB_GUID_TYPE              *GuidHob;
  CCX_SMM_TSEG_CONFIG            TsegConfig;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *SmramDescriptorBlock;
  EFI_PEI_MP_SERVICES_PPI        *MpServices;
  PEI_SMM_ACCESS_INSTANCE        *PeiSmmAccessInstance;
  EFI_PEI_PPI_DESCRIPTOR         *SmmAccessPpiDesc;

  Status = (*PeiServices)->LocatePpi (
                PeiServices,
                &gEfiPeiMpServicesPpiGuid,
                0,
                NULL,
                (VOID **) &MpServices
                );

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  GuidHob = GetFirstGuidHob (&gEfiSmmPeiSmramMemoryReserveGuid);

  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }

  PeiSmmAccessInstance = (PEI_SMM_ACCESS_INSTANCE*)AllocatePool (sizeof (PEI_SMM_ACCESS_INSTANCE));
  *PeiSmmAccessInstance = mPeiSmmAccessPrivateTemplate;

  PeiSmmAccessInstance->MpServices = MpServices;

  SmramDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) GET_GUID_HOB_DATA (GuidHob);

  PeiSmmAccessInstance->SmramMap = AllocateZeroPool ((SmramDescriptorBlock->NumberOfSmmReservedRegions) * sizeof (EFI_SMRAM_DESCRIPTOR));
  PeiSmmAccessInstance->NumberOfRegions = SmramDescriptorBlock->NumberOfSmmReservedRegions;

  TotalSize = 0;
  for (Index = 0; Index < PeiSmmAccessInstance->NumberOfRegions; Index++) {
    PeiSmmAccessInstance->SmramMap[Index].PhysicalStart = SmramDescriptorBlock->Descriptor[Index].PhysicalStart;
    PeiSmmAccessInstance->SmramMap[Index].CpuStart      = SmramDescriptorBlock->Descriptor[Index].CpuStart;
    PeiSmmAccessInstance->SmramMap[Index].PhysicalSize  = SmramDescriptorBlock->Descriptor[Index].PhysicalSize;
    PeiSmmAccessInstance->SmramMap[Index].RegionState   = SmramDescriptorBlock->Descriptor[Index].RegionState;
    TotalSize += PeiSmmAccessInstance->SmramMap[Index].PhysicalSize;
  }

  TsegConfig.TsegBase = PeiSmmAccessInstance->SmramMap[0].PhysicalStart;
  TsegConfig.TsegMask = ~(TotalSize - 1) | 0x6600;

  SetupSmmRegs (&TsegConfig);

  MpServices->StartupAllAPs (
                PeiServices,
                MpServices,
                SetupSmmRegs,
                FALSE,
                0,
                (VOID *) &TsegConfig
                );

  PeiSmmAccessInstance->SetSmmLock = PcdGetBool (PcdAmdSmmLock);

  SmmAccessPpiDesc = (EFI_PEI_PPI_DESCRIPTOR*)AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  SmmAccessPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SmmAccessPpiDesc->Guid  = &gPeiSmmAccessPpiGuid;
  SmmAccessPpiDesc->Ppi   = &PeiSmmAccessInstance->SmmAccess;

  Status = (*PeiServices)->InstallPpi (
    PeiServices,
    SmmAccessPpiDesc
  );

  return Status;
}
