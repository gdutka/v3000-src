/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <AmdCpmSmm.h>
#include <Protocol/AmdCpmABRecoveryProtocol.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Features/ABRecovery/Include/ABRecoveryBaseLib.h>

CPM_DEFINE_gBS;
CPM_DEFINE_gSmst;

EFI_STATUS
RebuildSharedB (
  IN  EFI_PHYSICAL_ADDRESS   *BiosImageMemoryAddr
  );

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_HANDLE                      AmdABRecoveryHandle;

AMD_CPM_ABRECOVERY_PROTOCOL AmdCpmABRecoveryProtocol = {
  RebuildSharedB
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

BOOLEAN
CheckSLOTSIDENTICAL(
  IN       UINT32                      ISHActiveAddress,
  IN       UINT32                      ISHAlternativeAddress,
  IN       EFI_PHYSICAL_ADDRESS        BiosBaseAddr
  )
{
  EFI_PHYSICAL_ADDRESS            ISHActiveBuffer;
  EFI_PHYSICAL_ADDRESS            ISHAlternativeBuffer;
  IMAGE_SLOT_HEADER_V2            *ISHActiveHeader;
  IMAGE_SLOT_HEADER_V2            *ISHAlternativeHeader;
  EFI_STATUS                      Status;
  BOOLEAN                         retStatus;
  retStatus = TRUE;

  Status = gSmst->SmmAllocatePages (
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINT32)sizeof(IMAGE_SLOT_HEADER_V2)),
                    &ISHActiveBuffer
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Allocate ISHActiveBuffer fail\n", __FUNCTION__));
    retStatus = FALSE;
  }

  Status = gSmst->SmmAllocatePages (
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINT32)sizeof(IMAGE_SLOT_HEADER_V2)),
                    &ISHAlternativeBuffer
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Allocate ISHAlternativeBuffer fail\n", __FUNCTION__));
    retStatus = FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ISHActiveAddress=%x\n", __FUNCTION__, ISHActiveAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ISHAlternativeAddress=%x\n", __FUNCTION__, ISHAlternativeAddress));

  CopyMem ((UINT8*)(UINTN)ISHActiveBuffer, (VOID *)(UINTN)(ISHActiveAddress + BiosBaseAddr), sizeof(IMAGE_SLOT_HEADER_V2));
  ISHActiveHeader = (IMAGE_SLOT_HEADER_V2*)ISHActiveBuffer;

  CopyMem ((UINT8*)(UINTN)ISHAlternativeBuffer, (VOID *)(UINTN)(ISHAlternativeAddress + BiosBaseAddr), sizeof(IMAGE_SLOT_HEADER_V2));
  ISHAlternativeHeader = (IMAGE_SLOT_HEADER_V2*)ISHAlternativeBuffer;

  if (ISHActiveHeader->PspId != ISHAlternativeHeader->PspId) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-SLOTA and SLOTB are not Identical \n", __FUNCTION__));
    retStatus = FALSE;
  }

  Status = gSmst->SmmFreePages (
                    ISHActiveBuffer,
                    EFI_SIZE_TO_PAGES ((UINT32)sizeof(IMAGE_SLOT_HEADER_V2))
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Free Buffer fail\n", __FUNCTION__));
  }

  Status = gSmst->SmmFreePages (
                    ISHAlternativeBuffer,
                    EFI_SIZE_TO_PAGES ((UINT32)sizeof(IMAGE_SLOT_HEADER_V2))
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Free Buffer fail\n", __FUNCTION__));
  }

  return retStatus;
}

BOOLEAN
RestoreISHBHeader(
  IN       UINT32                      ISHActiveAddress,
  IN       UINT32                      ISHAlternativeAddress,
  IN       EFI_PHYSICAL_ADDRESS        BiosBaseAddr
  )
{
  IMAGE_SLOT_HEADER_V2            *ISHActiveHeader          = (IMAGE_SLOT_HEADER_V2 *)(BiosBaseAddr + ISHActiveAddress);
  IMAGE_SLOT_HEADER_V2            *ISHAlternativeHeader     = (IMAGE_SLOT_HEADER_V2 *)(BiosBaseAddr + ISHAlternativeAddress);

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ISHActiveAddress=%x\n", __FUNCTION__, ISHActiveAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ISHAlternativeAddress=%x\n", __FUNCTION__, ISHAlternativeAddress));

  ISHAlternativeHeader->PspId = ISHActiveHeader->PspId;
  ISHAlternativeHeader->CheckSum = Fletcher32 ((UINT16*)(UINTN)&ISHAlternativeHeader->Priority, 0xE);

  return TRUE;
}



BOOLEAN
GetPSPDirectorySize (
  IN       EFI_PHYSICAL_ADDRESS       BiosBaseAddr,
  IN  OUT  UINT32                     *Size
  )
{
  EFI_PHYSICAL_ADDRESS            PspDirBuffer;
  PSP_DIRECTORY                   *PspDir;
  EFI_STATUS                      Status;

  //Allocate temp memory to hold backup
  Status = gSmst->SmmAllocatePages (
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize),
                    &PspDirBuffer
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Allocate temp Buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Size=%x BiosBaseAddr = %lx \n", __FUNCTION__, Size,BiosBaseAddr));

  CopyMem ((UINT8*)(UINTN)PspDirBuffer, (VOID *)(UINTN)(BiosBaseAddr), MaxPspDirSize);

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PspDirBuffer=%lx \n", __FUNCTION__, PspDirBuffer));

  PspDir = (PSP_DIRECTORY*)PspDirBuffer;
//  *Size = (UINT32)((PspDir->Header.Reserved & 0x3FF) << 12 );
  *Size = PspDir->Header.AdditionalInfo.Field.MaxSize * SIZE_4KB;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Size=%x \n", __FUNCTION__, *Size));

  Status = gSmst->SmmFreePages (
                    PspDirBuffer,
                    EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize)
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Free Buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
GetBIOSDirectorySize (
  IN       EFI_PHYSICAL_ADDRESS       BiosBaseAddr,
  IN  OUT  UINT32                      *Size
  )
{
  EFI_PHYSICAL_ADDRESS            BiosDirBuffer;
  BIOS_DIRECTORY                   *BiosDir;
  EFI_STATUS                      Status;

  //Allocate temp memory to hold backup
  Status = gSmst->SmmAllocatePages (
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize),
                    &BiosDirBuffer
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Allocate temp Buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  CopyMem ((UINT8*)(UINTN)BiosDirBuffer, (VOID *)(UINTN)(BiosBaseAddr), MaxPspDirSize);

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BiosDirBuffer=%lx \n", __FUNCTION__, BiosDirBuffer));

  BiosDir = (BIOS_DIRECTORY*)BiosDirBuffer;
//  *Size = (UINT32)((BiosDir->Header.Reserved & 0x3FF) << 12 );
  *Size = BiosDir->Header.AdditionalInfo.Field.MaxSize * SIZE_4KB;
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Size=%x \n", __FUNCTION__, *Size));

  Status = gSmst->SmmFreePages (
                    BiosDirBuffer,
                    EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize)
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Free Buffer fail\n", __FUNCTION__));
    return FALSE;
  }

  return TRUE;
}

BOOLEAN
SetupPSPL2DirBuffer (
  IN OUT  EFI_PHYSICAL_ADDRESS       *DirectoryMemoryAddr,
  IN      EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  )
{
  EFI_STATUS                      Status;
  EFI_PHYSICAL_ADDRESS            GlobalDirBufferAddr;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Enter \n", __FUNCTION__));

  //Allocate Global Directory buffer
  Status = gSmst->SmmAllocatePages (
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize),
                    &GlobalDirBufferAddr
                    );
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Status1 %x \n", __FUNCTION__, Status));

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Allocate global buffer fail \n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-GlobalPSPL2DirBufferAddr %lx \n", __FUNCTION__, GlobalDirBufferAddr));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BiosBaseAddr %lx \n", __FUNCTION__, BiosBaseAddr));

  CopyMem ((VOID *)GlobalDirBufferAddr, (VOID *)(UINTN)(BiosBaseAddr), MaxPspDirSize);
  *DirectoryMemoryAddr = GlobalDirBufferAddr;

  return TRUE;
}

BOOLEAN
SetupBIOSL2DirBuffer (
  IN OUT  EFI_PHYSICAL_ADDRESS       *DirectoryMemoryAddr,
  IN      EFI_PHYSICAL_ADDRESS       BiosBaseAddr
  )
{
  EFI_STATUS                      Status;
  EFI_PHYSICAL_ADDRESS            GlobalDirBufferAddr;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Enter \n", __FUNCTION__));

  //Allocate Global Directory buffer
  Status = gSmst->SmmAllocatePages (
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES ((UINT32)MaxPspDirSize),
                    &GlobalDirBufferAddr
                    );
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Status1 %x \n", __FUNCTION__, Status));

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-6-Allocate global buffer fail \n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-GlobalDirBufferAddr %lx \n", __FUNCTION__, GlobalDirBufferAddr));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BiosBaseAddr %lx \n", __FUNCTION__, BiosBaseAddr));

  CopyMem ((VOID *)GlobalDirBufferAddr, (VOID *)(UINTN)(BiosBaseAddr), MaxPspDirSize);
  *DirectoryMemoryAddr = GlobalDirBufferAddr;

  return TRUE;
}

BOOLEAN
UpdateL2BFromL2A (
  EFI_PHYSICAL_ADDRESS            *BiosImageMemoryAddr
  )
{
  BOOLEAN                         APIStatus;
  PSP_DIRECTORY                   *PspDir;
  IMAGE_SLOT_INFO                 *Slot;
  UINT64                          ActivePSPLevel2BaseAddress;
  UINT64                          AlternativePSPLevel2BaseAddress;
  UINT64                          ActiveBiosLevel2BaseAddress;
  UINT64                          OrgAlternativeBIOSLevel2BaseAddress;
  UINT64                          AlternativeBIOSLevel2BaseAddress;
  UINT64                          SourceEntryAddress;
  UINT64                          DestinationEntryAddress;
  UINT32                          ActiveEntrySize;
  UINT8                           ActiveSlotIndex;
  UINT8                           AlternativeSlotIndex;
  UINT32                          PSPSourceDirectorySize;
  UINT32                          BIOSSourceDirectorySize;
  UINT32                          PSPDestDirectorySize;
  UINT32                          BIOSDestDirectorySize;
  EFI_PHYSICAL_ADDRESS            DirectoryMemoryAddr;
  EFI_PHYSICAL_ADDRESS            PSPL2SourceMemoryAddr;
  EFI_PHYSICAL_ADDRESS            PSPL2DestMemoryAddr;
  EFI_PHYSICAL_ADDRESS            BIOSL2SourceMemoryAddr;
  EFI_PHYSICAL_ADDRESS            BIOSL2DestMemoryAddr;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));
  ActiveSlotIndex = 0;
  AlternativeSlotIndex = 1;
  ActivePSPLevel2BaseAddress = 0;
  AlternativePSPLevel2BaseAddress = 0;

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-BiosImageMemoryAddr %lx \n", __FUNCTION__, (*BiosImageMemoryAddr)));

  DirectoryMemoryAddr = (*BiosImageMemoryAddr);

  //Get PSPL1 Directory from the Directory Buffer setup above
  if (FetchPspDirBaseV2 (&PspDir, DirectoryMemoryAddr) != TRUE) {
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-FetchPspDirBaseV2 Fail\n", __FUNCTION__));
    return FALSE;
  }

  //Check if we have Both Slots
  if (VerifyTwoSlotAbRecovery (PspDir) != TRUE )  {
     DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-VerifyTwoSlotAbRecovery Fail\n", __FUNCTION__));
     return FALSE;
  }

  // Fetch the ISH info for both Slots.
  if (FetchImageSlotInfo (PspDir, &Slot, DirectoryMemoryAddr) != TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-FetchImageSlotInfo Fail\n", __FUNCTION__));
     return FALSE;
  }

  ActivePSPLevel2BaseAddress = Slot->SlotAHeader.ImageSlotAddr;
  AlternativePSPLevel2BaseAddress = Slot->SlotBHeader.ImageSlotAddr;
  ActiveSlotIndex = Slot->SlotAIndex;
  AlternativeSlotIndex = Slot->SlotBIndex;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ActivePSPLevel2BaseAddress=%x\n", __FUNCTION__, ActivePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-AlternativePSPLevel2BaseAddress=%x\n", __FUNCTION__, AlternativePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ISH-ActiveSlotIndex=%x\n", __FUNCTION__, ActiveSlotIndex));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-ISH-AlternativeSlotIndex=%x\n", __FUNCTION__, AlternativeSlotIndex));

  SourceEntryAddress = ((PspDir->PspEntry)+ActiveSlotIndex)->Location;
  DestinationEntryAddress = ((PspDir->PspEntry)+AlternativeSlotIndex)->Location;

  if (CheckSLOTSIDENTICAL ((UINT32)SourceEntryAddress,(UINT32)DestinationEntryAddress, DirectoryMemoryAddr) == TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-% a SLOT A and SLOT B are identical \n", __FUNCTION__));
     return TRUE;
  }

  //Setup PSPL2 Directory Header Buffer
  SetupPSPL2DirBuffer (&PSPL2SourceMemoryAddr, ((*BiosImageMemoryAddr) + ActivePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2SourceMemoryAddr=%lx\n", __FUNCTION__, PSPL2SourceMemoryAddr));

  SetupPSPL2DirBuffer (&PSPL2DestMemoryAddr, ((*BiosImageMemoryAddr) + AlternativePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2DestMemoryAddr=%lx\n", __FUNCTION__, PSPL2DestMemoryAddr));

  if (GetPSPDirectorySize (PSPL2SourceMemoryAddr, &PSPSourceDirectorySize) != TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-% a Get PSP Source Directory Size  failed \n", __FUNCTION__));
     return FALSE;
  }

  if (GetPSPDirectorySize (PSPL2DestMemoryAddr, &PSPDestDirectorySize) != TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-% a Get PSP Dest Directory Size  failed \n", __FUNCTION__));
     return FALSE;
  }

  //Setup BIOSL2 Directory Header Buffer
  APIStatus = FetchLevel2PSPEntryInfo (BIOS_REGION_DIR, ActivePSPLevel2BaseAddress,
                                       &ActiveBiosLevel2BaseAddress, &ActiveEntrySize, (*BiosImageMemoryAddr));

  AlternativeBIOSLevel2BaseAddress = AlternativePSPLevel2BaseAddress + (ActiveBiosLevel2BaseAddress - ActivePSPLevel2BaseAddress);

  APIStatus = FetchLevel2PSPEntryInfo (BIOS_REGION_DIR, AlternativePSPLevel2BaseAddress,
                                       &OrgAlternativeBIOSLevel2BaseAddress, &ActiveEntrySize, (*BiosImageMemoryAddr));

  SetupBIOSL2DirBuffer (&BIOSL2SourceMemoryAddr, ((*BiosImageMemoryAddr) + ActiveBiosLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2SourceMemoryAddr=%lx\n", __FUNCTION__, BIOSL2SourceMemoryAddr));

  SetupBIOSL2DirBuffer (&BIOSL2DestMemoryAddr, ((*BiosImageMemoryAddr) + OrgAlternativeBIOSLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-OrgAlternativeBIOSLevel2BaseAddress=%lx\n", __FUNCTION__, OrgAlternativeBIOSLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2DestMemoryAddr=%lx\n", __FUNCTION__, BIOSL2DestMemoryAddr));

  if (GetBIOSDirectorySize (BIOSL2SourceMemoryAddr, &BIOSSourceDirectorySize) != TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-% a Get Source BIOS Directory Size failed \n", __FUNCTION__));
    return FALSE;
  }

  if (GetBIOSDirectorySize (BIOSL2DestMemoryAddr, &BIOSDestDirectorySize) != TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-% a Get Dest BIOS Directory Size failed \n", __FUNCTION__));
    return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2-DirectorySize %lx \n", __FUNCTION__, PSPSourceDirectorySize));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2-DirectorySize=%x\n", __FUNCTION__, BIOSSourceDirectorySize));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2SourceAddr=%lx\n", __FUNCTION__, (*BiosImageMemoryAddr) + ActivePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2-ActivePSPLevel2BaseAddress=%x\n", __FUNCTION__, ActivePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2SourceAddr=%lx\n", __FUNCTION__, ((*BiosImageMemoryAddr) + ActivePSPLevel2BaseAddress)));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2-AlternativeBIOSLevel2BaseAddress=%x\n", __FUNCTION__, AlternativePSPLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-PSPL2DestAddr=%lx\n", __FUNCTION__, ((*BiosImageMemoryAddr) + AlternativePSPLevel2BaseAddress)));

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2-ActiveBiosLevel2BaseAddress=%x\n", __FUNCTION__, ActiveBiosLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2SourceAddr=%lx\n", __FUNCTION__, ((*BiosImageMemoryAddr) + ActiveBiosLevel2BaseAddress)));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2-AlternativeBIOSLevel2BaseAddress=%x\n", __FUNCTION__, AlternativeBIOSLevel2BaseAddress));
  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOSL2DestAddr=%lx\n", __FUNCTION__, ((*BiosImageMemoryAddr) + AlternativeBIOSLevel2BaseAddress)));

  //Clear Destination PSPL2 and BIOSL2.
  SetMem ((UINT8 *)(UINTN)(((*BiosImageMemoryAddr) + AlternativePSPLevel2BaseAddress)), PSPDestDirectorySize, 0);
  SetMem ((UINT8 *)(UINTN)(((*BiosImageMemoryAddr) + OrgAlternativeBIOSLevel2BaseAddress)), BIOSDestDirectorySize, 0);

  //Restore PSPL2B Header.
  //Copy entire PSPL2A to PSPL2B
  CopyMem ((UINT8 *)(UINTN)(((*BiosImageMemoryAddr) + AlternativePSPLevel2BaseAddress)),
           (VOID *)(UINTN)(((*BiosImageMemoryAddr) + ActivePSPLevel2BaseAddress)),
           PSPSourceDirectorySize);

  //Restore BIOSL2B Header.
  //Copy entire BIOSL2A to BIOSL2B
  CopyMem ((UINT8 *)(UINTN)(((*BiosImageMemoryAddr) + AlternativeBIOSLevel2BaseAddress)),
           (VOID *)(UINTN)(((*BiosImageMemoryAddr) + ActiveBiosLevel2BaseAddress)),
           BIOSSourceDirectorySize);

  //Restore ISHB.
  if (RestoreISHBHeader ((UINT32)SourceEntryAddress,(UINT32)DestinationEntryAddress, DirectoryMemoryAddr) != TRUE) {
     DEBUG ((DEBUG_INFO, "FEA-SMM-% a Restore SLOT B failed \n", __FUNCTION__));
     return FALSE;
  }

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-BIOS L2B Restore Completed \n", __FUNCTION__));

  return TRUE;
}

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
RebuildSharedB (
  IN  EFI_PHYSICAL_ADDRESS   *BiosImageMemoryAddr
  )
{
  EFI_STATUS           Status;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  Status = UpdateL2BFromL2A (BiosImageMemoryAddr);

  DEBUG((DEBUG_INFO, "FEA-SMM-%a-Status-%x BiosImageMemoryAddr %lx \n", __FUNCTION__, Status, (*BiosImageMemoryAddr)));

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End \n", __FUNCTION__));

  return Status;
}

/*********************************************************************************
 * Name: AmdCpmABRecoverySmmInit
 *
 * Description
 *   Entry point of the AMD AB Recovery SMM driver
 *
 * Input
 *   ImageHandle : EFI Image Handle for the SMM driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
AmdCpmABRecoverySmmInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  EFI_STATUS        Status;

  DEBUG ((DEBUG_INFO, "FEA-SMM-%a-Start\n", __FUNCTION__));

  If_CpmInSmm (ImageHandle, SystemTable, gBS, gSmst, Status) {
    AmdABRecoveryHandle = NULL;
    Status = gSmst->SmmInstallProtocolInterface (
                      &AmdABRecoveryHandle,
                      &gAmdCpmABRecoveryProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &AmdCpmABRecoveryProtocol
                      );
    DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-1-Install\n", __FUNCTION__));

    if (EFI_ERROR (Status)) {
      Status = gSmst->SmmUninstallProtocolInterface (
                        AmdABRecoveryHandle,
                        &gAmdCpmABRecoveryProtocolGuid,
                        &AmdCpmABRecoveryProtocol
                        );
      DEBUG ((DEBUG_INFO, "FEA-SMM-%a-End-1-EFI_ERROR\n", __FUNCTION__));
      return Status;
    }
  } //If_CpmInSmm

  return Status;
}
