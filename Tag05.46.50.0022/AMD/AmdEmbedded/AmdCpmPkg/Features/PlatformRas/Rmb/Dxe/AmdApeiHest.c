/*****************************************************************************
 *
 * Copyright (C) 2018-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmdRasApeiRmbProtocol.h>
#include "AmdPlatformRasRmbDxe.h"
#include "AmdApeiHest.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
UpdateMcBankStructure(
  IN       EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE* McBankStruc,
  IN       AMD_MC_BANK_INIT_DATA* AmdMcBankInitData,
  IN       UINT8   MaxBankCnt,
  OUT      UINT8** HestTableEnd
  );

EFI_STATUS
AddHestGhes (
  IN       UINT16   SourceId,
  IN       UINT16   RelatedSourceId,
  IN       UINT32   ErrDataEntrySize,
  IN       EFI_GUID *ErrorSectGuid,
  IN       EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE      *GenErrEntry,
  IN       EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *GenErrSrc,
  IN       EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE   *NotificationStructure,
  OUT      EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE          **BootErrBlk
 );

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

//This is a template for MCA init. the actual bank number will deremine in runtime.
AMD_MC_BANK_INIT_DATA McBankInitData[] = {
  {0xFFFFFFFFFFFFFFFF,0xC0002000,0xC0002001,0xC0002002,0xC0002003},      //Bank 0
};

AMD_MC_INIT_DATA McaInitData = {
  0x0000000000000000,       // AmdGlobCapInitData, update in runtime.
  0x0000000000000000,       // AmdGlobCtrlInitData, update in runtime.
  0x00,                     // AmdMcbClrStatusOnInit
  0x02,                     // AmdMcbStatusDataFormat
  0x00,                     // Reserved
  (sizeof (McBankInitData) / sizeof (McBankInitData[0])), // McBankNum
  &McBankInitData[0]   // Pointer to Initial data of MC Bank
};

STATIC EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER HestHeaderStruct =
{
  {EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_SIGNATURE,
   0,
   ACPI_REV1,
   0,
   {'A','M','D',' ',' ',' '},
   AMD_HEST_TABLE_ID,
   1,
   AMD_CREATOR_ID,
   1
  },
  0
};

STATIC EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE McaErrorNofication =
{
  HARDWARE_ERROR_NOTIFICATION_POLLED,                            // UINT8  NotifiyType = Polled
  sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE),   // UINT8  ErrNotifyLen
  {0,0,0,0,0,0,0},                                               // UINT16 ConfigWrite
  5000,                                                          // UINT32 PollInterval = 5000 ms
  0,                                                             // UINT32 Interrupt Vector
  0,                                                             // UINT32 SwitchPollingThreshVal
  0,                                                             // UINT32 SwitchPollingThreshWindow
  1,                                                             // UINT32 ErrThresholdVal;
  0                                                              // UINT32 ErrThresholdWindow
};

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern EFI_BOOT_SERVICES                *gBS;
extern AMD_RAS_APEIRMB_PROTOCOL         *AmdRasApeiRmbProtocol;

extern GENERIC_NBIO_ERR_ENTRY_GN        gGenNbioErrEntry;
extern EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gNbioErrSrc;
extern GENERIC_PCIE_AER_ERR_ENTRY_GN    gGenPcieErrEntry;
extern EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gPcieGenErrSrc;
extern GENERIC_PROC_ERR_ENTRY_GN gGenProcErrEntry;
extern EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gGenProcErrSrc;
extern GENERIC_MEM_ERR_ENTRY_GN gGenMemErrEntry;
extern EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE gGenMemErrSrc;

/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE HestErrRegTbl = {
  {0,0,0,0,0},                                                     // UINT32 BlockStatus - WHEA Platform Design Guide Table 3-2 Block Status
  sizeof (EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE),            // UINT32 RawDataOffset - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0,                                                               // UINT32 RawDataLength - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  0,                                                               // UINT32 DataLength - WHEA Platform Design Guide Table 3-2 Raw Data Offset
  EFI_ACPI_6_3_ERROR_SEVERITY_NONE                                 // UINT32 ErrorSeverity - WHEA Platform Design Guide Table 3-2 Raw Data Offset (Initialize Severity to [None] = 0x03)
};


/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiHestInit
 *
 *  Description:
 *     Install ACPI APEI HEST table.
 *
 *  Parameters:
 *    @retval        EFI_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
ApeiHestInstall ( VOID )
{
  EFI_STATUS Status;
  UINT8  MaxBankCnt;
  UINT8* HestTableEnd;
  UINT32 HestTableSize;
  UINTN TableKey;
  EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_EXCEPTION_STRUCTURE* MceStruc;
  EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE* McBankStruc;
  AMD_MC_BANK_INIT_DATA* AmdMcBankInitData;
  EFI_ACPI_6_3_IA32_ARCHITECTURE_CORRECTED_MACHINE_CHECK_STRUCTURE* CmcStruc;
  EFI_ACPI_6_3_IA32_ARCHITECTURE_DEFERRED_MACHINE_CHECK_STRUCTURE* DmcStruc;
  EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER* HestTableHdr;

  AmdMcBankInitData = McaInitData.McBankInitData;

  SearchMaxMcaBankCnt(&MaxBankCnt);

  DEBUG ((EFI_D_ERROR, "[RAS] MCA Bank Count: %d\n", MaxBankCnt));

  HestTableSize = sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER) + \
                  sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_EXCEPTION_STRUCTURE) + \
                  (sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE) * MaxBankCnt) + \
                  sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_CORRECTED_MACHINE_CHECK_STRUCTURE) + \
                  (sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE) * MaxBankCnt);

  if (mPlatformApeiPrivate->PlatRasPolicy.HestDMCStrucEn) {
    HestTableSize = HestTableSize + sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_DEFERRED_MACHINE_CHECK_STRUCTURE) + \
                  (sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE) * MaxBankCnt);
  }

  Status = gBS->AllocatePool(EfiBootServicesData,
                              HestTableSize ,
                              &HestTableHdr);
  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    //clear instances content
    gBS->SetMem (HestTableHdr, HestTableSize, 0);
  }

  gBS->CopyMem (&HestHeaderStruct.Header.OemTableId, (UINT64 *)PcdGetPtr (PcdAmdAcpiHestTableHeaderOemTableId), sizeof (UINT64));
  gBS->CopyMem (HestTableHdr,
               &HestHeaderStruct,
               sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER)
               );

  ((EFI_ACPI_DESCRIPTION_HEADER*) (&HestTableHdr->Header))->Length = HestTableSize;
  HestTableEnd = (UINT8*)HestTableHdr;

  HestTableEnd = (UINT8*)HestTableHdr + sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_SOURCE_TABLE_HEADER);

  //Init Machine Check Exception Structure
  MceStruc = (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_EXCEPTION_STRUCTURE*)HestTableEnd;
  MceStruc->Type = EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_EXCEPTION;
  MceStruc->SourceId = MCE_SOURCE_ID0;
  MceStruc->Flags = 0;
  if (mPlatformApeiPrivate->PlatRasPolicy.CpmGhesAssistEnable) {
    MceStruc->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GHES_ASSIST;
  }
  MceStruc->Enabled = 1;
  MceStruc->NumberOfRecordsToPreAllocate = RECORDS_PREALLOCATE;
  MceStruc->MaxSectionsPerRecord = RECORDS_PREALLOCATE;
  MceStruc->GlobalCapabilityInitData = AsmReadMsr64 (MSR_MCG_CAP);//McaInitData.GlobCapInitData;
  MceStruc->GlobalControlInitData = (1 << MaxBankCnt) - 1 & ~(1 << MCA_EMPTY0_BANK);//McaInitData.GlobCtrlInitData
  MceStruc->NumberOfHardwareBanks = MaxBankCnt;

  //Point to Machine Check Bank Structure
  HestTableEnd = HestTableEnd + sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_EXCEPTION_STRUCTURE);

  McBankStruc = (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE*)HestTableEnd;

  UpdateMcBankStructure(McBankStruc, AmdMcBankInitData, MaxBankCnt, &HestTableEnd);
  HestTableHdr->ErrorSourceCount++;

  //Init Corrected Machine Check Structure
  CmcStruc = (EFI_ACPI_6_3_IA32_ARCHITECTURE_CORRECTED_MACHINE_CHECK_STRUCTURE*)HestTableEnd;
  CmcStruc->Type = EFI_ACPI_6_3_IA32_ARCHITECTURE_CORRECTED_MACHINE_CHECK;
  CmcStruc->SourceId = CMC_SOURCE_ID0;
  CmcStruc->Flags = 0;
  if (mPlatformApeiPrivate->PlatRasPolicy.CpmGhesAssistEnable) {
    CmcStruc->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GHES_ASSIST;
  }
  CmcStruc->Enabled = 1;
  CmcStruc->NumberOfRecordsToPreAllocate = RECORDS_PREALLOCATE;
  CmcStruc->MaxSectionsPerRecord = SECTIONS_PER_RECORD;
  gBS->CopyMem (&CmcStruc->NotificationStructure,
               &McaErrorNofication,
               sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE)
               );

  CmcStruc->NotificationStructure.ErrorThresholdValue = 0xFFF - mPlatformApeiPrivate->PlatRasPolicy.McaErrThreshCount;
  CmcStruc->NotificationStructure.PollInterval = mPlatformApeiPrivate->PlatRasPolicy.McaPollInterval;

  //reset McBankInitData point
  CmcStruc->NumberOfHardwareBanks = MaxBankCnt;

  HestTableEnd = HestTableEnd + sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_CORRECTED_MACHINE_CHECK_STRUCTURE);

  //Reuse McBankStruc pointer and setup Corrected Machine Check Bank Structure.
  McBankStruc = (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE*)HestTableEnd;

  UpdateMcBankStructure(McBankStruc, AmdMcBankInitData, MaxBankCnt, &HestTableEnd);
  HestTableHdr->ErrorSourceCount++;

  if (mPlatformApeiPrivate->PlatRasPolicy.HestDMCStrucEn) {
    //Init Deferred Machine Check Structure
    DmcStruc = (EFI_ACPI_6_3_IA32_ARCHITECTURE_DEFERRED_MACHINE_CHECK_STRUCTURE*)HestTableEnd;
    DmcStruc->Type = EFI_ACPI_6_3_IA32_ARCHITECTURE_DEFERRED_MACHINE_CHECK;
    DmcStruc->SourceId = DMC_SOURCE_ID0;
    DmcStruc->Flags = 0;
    if (mPlatformApeiPrivate->PlatRasPolicy.CpmGhesAssistEnable) {
      DmcStruc->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GHES_ASSIST;
    }
    DmcStruc->Enabled = 1;
    DmcStruc->NumberOfRecordsToPreAllocate = RECORDS_PREALLOCATE;
    DmcStruc->MaxSectionsPerRecord = SECTIONS_PER_RECORD;
    gBS->CopyMem (&DmcStruc->NotificationStructure,
                 &McaErrorNofication,
                 sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE)
                 );

    DmcStruc->NotificationStructure.ErrorThresholdValue = 0xFFF - mPlatformApeiPrivate->PlatRasPolicy.McaErrThreshCount;
    DmcStruc->NotificationStructure.PollInterval = mPlatformApeiPrivate->PlatRasPolicy.McaPollInterval;

    //reset McBankInitData point
    DmcStruc->NumberOfHardwareBanks = MaxBankCnt;

    HestTableEnd = HestTableEnd + sizeof (EFI_ACPI_6_3_IA32_ARCHITECTURE_DEFERRED_MACHINE_CHECK_STRUCTURE);

    //Reuse McBankStruc pointer and setup Corrected Machine Check Bank Structure.
    McBankStruc = (EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE*)HestTableEnd;

    UpdateMcBankStructure(McBankStruc, AmdMcBankInitData, MaxBankCnt, &HestTableEnd);
    HestTableHdr->ErrorSourceCount++;
  }

  // Update HEST header Checksum
  ChecksumAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *) HestTableHdr);

  TableKey = 0;
  AcpiTableProtocol->InstallAcpiTable (
                  AcpiTableProtocol,
                  HestTableHdr,
                  HestTableSize,
                  &TableKey
                  );

  gBS->FreePool (HestTableHdr);

  return Status;
}

EFI_STATUS
SetupGhesAssist(VOID)
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_GUID      ProcErrorSectGuid = PROCESSOR_SPECIFIC_X86_SECT_GUID;
  EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE* NotificationStructure;

  //Setup GHES and link to MCE/CMC/DMC structure
  Status = gBS->AllocatePool (EfiBootServicesData,
                              sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE) ,
                              &NotificationStructure);
  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    //clear instances content
    gBS->SetMem (NotificationStructure, sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE), 0);
  }

  NotificationStructure->Type = HARDWARE_ERROR_NOTIFICATION_POLLED;
  NotificationStructure->Length = sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE);
  NotificationStructure->PollInterval = mPlatformApeiPrivate->PlatRasPolicy.McaPollInterval;

  AddHestGhes (PROC_MCE_SOURCE_ID,
               MCE_SOURCE_ID0,
               sizeof (GENERIC_PROC_ERR_ENTRY_GN),
               &ProcErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenProcErrEntry,
               &gGenProcErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdProcMceErrBlk
               );
  mPlatformApeiPrivate->AmdMemMceErrBlk = mPlatformApeiPrivate->AmdProcMceErrBlk;
  DEBUG ((EFI_D_ERROR, "[RAS]MCE Error Data Block Base Address: 0x%08x\n",mPlatformApeiPrivate->AmdMemMceErrBlk));

  AddHestGhes (PROC_CMC_SOURCE_ID,
               CMC_SOURCE_ID0,
               sizeof (GENERIC_PROC_ERR_ENTRY_GN),
               &ProcErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenProcErrEntry,
               &gGenProcErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdProcCmcErrBlk
               );
  mPlatformApeiPrivate->AmdMemCmcErrBlk = mPlatformApeiPrivate->AmdProcCmcErrBlk;
  DEBUG ((EFI_D_ERROR, "[RAS]CMC Error Data Block Base Address: 0x%08x\n",mPlatformApeiPrivate->AmdMemCmcErrBlk));

  if (mPlatformApeiPrivate->PlatRasPolicy.HestDMCStrucEn) {
      AddHestGhes(PROC_DMC_SOURCE_ID,
                   DMC_SOURCE_ID0,
                   sizeof (GENERIC_PROC_ERR_ENTRY_GN),
                   &ProcErrorSectGuid,
                   (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenProcErrEntry,
                   &gGenProcErrSrc,
                   NotificationStructure,
                   &mPlatformApeiPrivate->AmdProcDmcErrBlk
                   );
      mPlatformApeiPrivate->AmdMemDmcErrBlk = mPlatformApeiPrivate->AmdProcDmcErrBlk;
      DEBUG ((EFI_D_ERROR, "[RAS]DMC Error Data Block Base Address: 0x%08x\n",mPlatformApeiPrivate->AmdMemDmcErrBlk));
  }

  gBS->FreePool (NotificationStructure);

  return Status;
}

EFI_STATUS
UpdateMcBankStructure(
  IN       EFI_ACPI_6_3_IA32_ARCHITECTURE_MACHINE_CHECK_ERROR_BANK_STRUCTURE* McBankStruc,
  IN       AMD_MC_BANK_INIT_DATA* AmdMcBankInitData,
  IN       UINT8   MaxBankCnt,
  OUT      UINT8** HestTableEnd
  )
{
  UINT8 BankIndex;
  EFI_STATUS  Status = EFI_SUCCESS;

  for (BankIndex = 0; BankIndex < MaxBankCnt; BankIndex++) {
    McBankStruc->BankNumber = BankIndex;
    McBankStruc->ClearStatusOnInitialization = McaInitData.ClrStatusOnInit;
    McBankStruc->StatusDataFormat = McaInitData.StatusDataFormat;

    if (MCA_EMPTY0_BANK == BankIndex) {
      McBankStruc->ControlInitData = 0;
    } else {
    McBankStruc->ControlInitData = AmdMcBankInitData->CtrlInitData;
    }
    McBankStruc->ControlRegisterMsrAddress = AmdMcBankInitData->CtrlRegMSRAddr + (BankIndex * SMCA_REG_PER_BANK);
    McBankStruc->StatusRegisterMsrAddress = AmdMcBankInitData->StatRegMSRAddr + (BankIndex * SMCA_REG_PER_BANK);
    McBankStruc->AddressRegisterMsrAddress = AmdMcBankInitData->AddrRegMSRAddr + (BankIndex * SMCA_REG_PER_BANK);
    McBankStruc->MiscRegisterMsrAddress = AmdMcBankInitData->MiscRegMSRAddr + (BankIndex * SMCA_REG_PER_BANK);
    McBankStruc++;  //Point to next bank
  }
  *HestTableEnd = (UINT8*)McBankStruc;
  return Status;
}

EFI_STATUS
AddHestGhes ( 
  IN       UINT16   SourceId,
  IN       UINT16   RelatedSourceId,
  IN       UINT32   ErrDataEntrySize,
  IN       EFI_GUID *ErrorSectGuid,
  IN       EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE      *GenErrEntry,
  IN       EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE *GenErrSrc,
  IN       EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE   *NotificationStructure,
  OUT      EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE          **BootErrBlk
 )
{
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE           *GenBootErrBlk;
  EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE           *GenBootErrBlkStart;
  EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE       *ErrDataEntry;
  UINT64                                                *ErrBlkAddress;
  EFI_STATUS                                            Status = EFI_SUCCESS;
  UINT32                                                McaGhesAssistTblCount;
  UINT32                                                Index;
  EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE  *GenErrSrcForMcaGhesAssistStart;
  EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE  *GenErrSrcForMcaGhesAssist;
  BOOLEAN                                               IsMcaGhesAssist;
  UINT32                                                ErrorBlockSize;


  IsMcaGhesAssist = FALSE;
  McaGhesAssistTblCount = 0;
  Index = 0;
  ErrorBlockSize = MAX_ERROR_BLOCK_SIZE;
  GenErrSrcForMcaGhesAssistStart = NULL;
  GenErrSrcForMcaGhesAssist = NULL;

  // PROC_MCE_SOURCE_ID: 0x1000, PROC_CMC_SOURCE_ID: 0x5000, PROC_DMC_SOURCE_ID: 0x9000
  if ((SourceId == PROC_MCE_SOURCE_ID) || 
      (SourceId == PROC_CMC_SOURCE_ID) ||
      (SourceId == PROC_DMC_SOURCE_ID)) {
    McaGhesAssistTblCount = (mPlatformApeiPrivate->McaLocalApicIdMappingTable->TableEntryNum) * mPlatformApeiPrivate->MaxMcaBankCount;
    Status = gBS->AllocatePool (
                    EfiReservedMemoryType,
                    McaGhesAssistTblCount * sizeof (EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE),
                    (VOID **)&GenErrSrcForMcaGhesAssistStart
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
    GenErrSrcForMcaGhesAssist = GenErrSrcForMcaGhesAssistStart;
    IsMcaGhesAssist = TRUE;    
  }

  if (IsMcaGhesAssist) {
    ErrorBlockSize = MAX_MCA_ERROR_BLOCK_SIZE;
  }

  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  ((IsMcaGhesAssist ? McaGhesAssistTblCount : 1) * ErrorBlockSize),
                  (VOID **)&GenBootErrBlkStart
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ZeroMem (GenBootErrBlkStart, ((IsMcaGhesAssist? McaGhesAssistTblCount : 1) * ErrorBlockSize));
  GenBootErrBlk = GenBootErrBlkStart;

  do {
    //
    //  Allocate memory and Initialize memory blocks for Error Data block and Error Data block pointer
    //

    Status = gBS->AllocatePool (
                    EfiReservedMemoryType,
                    sizeof (UINT64),
                    &ErrBlkAddress
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    //Init Hest Generic error status block.
    gBS->CopyMem (GenBootErrBlk, &HestErrRegTbl, sizeof (EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE));

    //Init Hest Generic error data entry
    ErrDataEntry = (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*) (GenBootErrBlk + 1);
    gBS->CopyMem (ErrDataEntry, GenErrEntry, ErrDataEntrySize);

    //Update Error section GUID
    gBS->CopyMem (&ErrDataEntry->SectionType[0], ErrorSectGuid, sizeof (EFI_GUID));

    //Update pointer and install error source record.
    *ErrBlkAddress = (UINT64) GenBootErrBlk;
    GenErrSrc->ErrorStatusAddress.Address =  (UINT64)ErrBlkAddress;

    //Update notification structure.

    gBS->CopyMem (&GenErrSrc->NotificationStructure, NotificationStructure, sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE));

    GenErrSrc->SourceId = SourceId + (IsMcaGhesAssist ? (UINT16)Index : 0);
    GenErrSrc->RelatedSourceId = RelatedSourceId;
    GenErrSrc->MaxRawDataLength = ErrorBlockSize;
    GenErrSrc->ErrorStatusBlockLength = ErrorBlockSize;

    if (IsMcaGhesAssist) {
      gBS->CopyMem (GenErrSrcForMcaGhesAssist, GenErrSrc, sizeof (EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE));
      GenErrSrcForMcaGhesAssist++;
      GenBootErrBlk = (EFI_ACPI_6_3_GENERIC_ERROR_STATUS_STRUCTURE *)((UINT8 *)GenBootErrBlk + ErrorBlockSize);
      Index++;
    }
  } while (IsMcaGhesAssist && (Index < McaGhesAssistTblCount));

  if (IsMcaGhesAssist) {
    AmdRasApeiRmbProtocol->AddMultipleHestErrorSourceEntry ((UINT8*)GenErrSrcForMcaGhesAssistStart, 
                                                           McaGhesAssistTblCount * sizeof (EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE),
                                                           McaGhesAssistTblCount);
  } else {
    AmdRasApeiRmbProtocol->AddHestErrorSourceEntry ((UINT8*)GenErrSrc, sizeof (EFI_ACPI_6_3_GENERIC_HARDWARE_ERROR_SOURCE_STRUCTURE));
  }

  gBS->FreePool (GenErrSrcForMcaGhesAssistStart);

  *BootErrBlk = GenBootErrBlkStart;
  return Status;
}

EFI_STATUS
NbioHestUpdate ( VOID )
{
  EFI_STATUS                                 Status = EFI_SUCCESS;
  EFI_GUID                                   NbioErrorSectGuid = NBIO_ERROR_SECT_GUID;
  EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE* NotificationStructure;
  UINT8                                             CorrNotifyType;
  UINT8                                             UnCorrNotifyType;


  DEBUG ((EFI_D_ERROR, "[RAS]Install NBIO HEST table\n"));

  Status = gBS->AllocatePool (EfiBootServicesData,
                              sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE) ,
                              &NotificationStructure);
  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    //clear instances content
    gBS->SetMem (NotificationStructure, sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE), 0);
  }

  //Setup Notification Type for Root Port Correctable Error structure
  if (mPlatformApeiPrivate->PlatRasPolicy.GhesNotifyType) {
    CorrNotifyType = HARDWARE_ERROR_NOTIFICATION_SCI;
  } else {
    CorrNotifyType = HARDWARE_ERROR_NOTIFICATION_POLLED;
  }

  if (mPlatformApeiPrivate->PlatRasPolicy.GhesUnCorrNotifyType) {
    UnCorrNotifyType = HARDWARE_ERROR_NOTIFICATION_NMI;
  } else {
    UnCorrNotifyType = HARDWARE_ERROR_NOTIFICATION_POLLED;
  }

  //Setup Notification Type here
  NotificationStructure->Type = CorrNotifyType;
  NotificationStructure->Length = sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE);
  NotificationStructure->PollInterval = 5000;

  AddHestGhes (NBIO_CORR_SOURCE_ID,
               0xFFFF,
               sizeof (GENERIC_NBIO_ERR_ENTRY_GN),
               &NbioErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenNbioErrEntry,
               &gNbioErrSrc,
               NotificationStructure, 
               &mPlatformApeiPrivate->AmdNbioErrBlk
               );

  NotificationStructure->Type = UnCorrNotifyType;
  AddHestGhes (NBIO_UNCORR_SOURCE_ID,
               0xFFFF,
               sizeof (GENERIC_NBIO_ERR_ENTRY_GN),
               &NbioErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenNbioErrEntry,
               &gNbioErrSrc,
               NotificationStructure, 
               &mPlatformApeiPrivate->AmdNbioUnErrBlk
               );

  gBS->FreePool (NotificationStructure);

  return Status;
}


EFI_STATUS
PcieAerHestUpdate ( VOID )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  EFI_ACPI_6_3_PCI_EXPRESS_ROOT_PORT_AER_STRUCTURE  *PcieAerHestTableHead;
  EFI_ACPI_6_3_PCI_EXPRESS_ROOT_PORT_AER_STRUCTURE  *PcieRpAerHestTable;
  EFI_ACPI_6_3_PCI_EXPRESS_DEVICE_AER_STRUCTURE     *PcieDevAerHestTable;
  EFI_ACPI_6_3_PCI_EXPRESS_BRIDGE_AER_STRUCTURE     *PcieBridgeAerHestTable;
  UINT32                                            PcieAerHestTableSize;
  UINT8                                             AerReportPcd;
  EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE* NotificationStructure;
  EFI_GUID                                          PcieErrorSectGuid = PCIE_SECT_GUID;
  UINT8                                             CorrNotifyType;
  UINT8                                             UnCorrNotifyType;
  UINT32                                            ErrSrcCount;

  PcieAerHestTableSize = sizeof (EFI_ACPI_6_3_PCI_EXPRESS_ROOT_PORT_AER_STRUCTURE) + \
                         sizeof (EFI_ACPI_6_3_PCI_EXPRESS_DEVICE_AER_STRUCTURE) + \
                         sizeof (EFI_ACPI_6_3_PCI_EXPRESS_BRIDGE_AER_STRUCTURE);

  Status = gBS->AllocatePool (EfiBootServicesData,
                              PcieAerHestTableSize ,
                              &PcieAerHestTableHead);
  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    //clear instances content
    gBS->SetMem (PcieAerHestTableHead, PcieAerHestTableSize, 0);
  }
  DEBUG ((EFI_D_ERROR, "[RAS]Install PCI Express AER HEST table\n"));

  AerReportPcd = PcdGet8(PcdAmdPcieAerReportMechanism); // (0) MCA reporting; (1) OS First; (2) FW First

  ErrSrcCount = 0;

  //Setup Root Port AER Structure
  PcieRpAerHestTable = PcieAerHestTableHead;

  PcieRpAerHestTable->Type = EFI_ACPI_6_3_PCI_EXPRESS_ROOT_PORT_AER;
  PcieRpAerHestTable->SourceId = PCIE_ROOT_PORT_SRC_ID;

  if ((AerReportPcd == 0) || (AerReportPcd == 2)) {
    PcieRpAerHestTable->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GLOBAL | EFI_ACPI_6_3_ERROR_SOURCE_FLAG_FIRMWARE_FIRST;
    mPlatformApeiPrivate->RasAcpiSmmData->PcieOscCtrl.Field.PcieAerCtrl = 0;  //Take AER control from OS.
  } else {
    PcieRpAerHestTable->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GLOBAL;
  }

  PcieRpAerHestTable->Enabled = 1;
  PcieRpAerHestTable->NumberOfRecordsToPreAllocate = 0x1;
  PcieRpAerHestTable->MaxSectionsPerRecord = 0x10;
  PcieRpAerHestTable->DeviceControl = 0x7;
  PcieRpAerHestTable->UncorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieRpUnCorrectedErrorMask;
  PcieRpAerHestTable->UncorrectableErrorSeverity = mPlatformApeiPrivate->PlatRasPolicy.PcieRpUnCorrectedErrorSeverity;
  PcieRpAerHestTable->CorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieRpCorrectedErrorMask;
  PcieRpAerHestTable->AdvancedErrorCapabilitiesAndControl = 0;
  PcieRpAerHestTable->RootErrorCommand = 0;
  if (1 == mPlatformApeiPrivate->PlatRasPolicy.PcieAerReportMechanism) {
    PcieRpAerHestTable->RootErrorCommand = 7;
  }
  ErrSrcCount++;  //Create PcieRpAerHestTable - done

  //Setup Device AER Structure
  PcieRpAerHestTable++;
  PcieDevAerHestTable = (EFI_ACPI_6_3_PCI_EXPRESS_DEVICE_AER_STRUCTURE*)(PcieRpAerHestTable);

  PcieDevAerHestTable->Type = EFI_ACPI_6_3_PCI_EXPRESS_DEVICE_AER;
  PcieDevAerHestTable->SourceId = PCIE_DEVICE_SOURCE_ID;

  if ((AerReportPcd == 0) || (AerReportPcd == 2)) {
    PcieDevAerHestTable->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GLOBAL | EFI_ACPI_6_3_ERROR_SOURCE_FLAG_FIRMWARE_FIRST;
  } else {
    PcieDevAerHestTable->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GLOBAL;
  }
  PcieDevAerHestTable->Enabled = 1;
  PcieDevAerHestTable->NumberOfRecordsToPreAllocate = 0x1;
  PcieDevAerHestTable->MaxSectionsPerRecord = 0x10;
  PcieDevAerHestTable->DeviceControl = 0x7;
  PcieDevAerHestTable->UncorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorMask;
  PcieDevAerHestTable->UncorrectableErrorSeverity = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorSeverity;
  PcieDevAerHestTable->CorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevCorrectedErrorMask;
  PcieDevAerHestTable->AdvancedErrorCapabilitiesAndControl = 0;
  ErrSrcCount++;  //Create PcieDevAerHestTable - done.

  //Setup Bridge AER Structure
  PcieDevAerHestTable++;
  PcieBridgeAerHestTable = (EFI_ACPI_6_3_PCI_EXPRESS_BRIDGE_AER_STRUCTURE*)(PcieDevAerHestTable);

  PcieBridgeAerHestTable->Type = EFI_ACPI_6_3_PCI_EXPRESS_BRIDGE_AER;
  PcieBridgeAerHestTable->SourceId = PCIE_BRIDGE_SOURCE_ID;

  if ((AerReportPcd == 0) || (AerReportPcd == 2)) {
    PcieBridgeAerHestTable->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GLOBAL | EFI_ACPI_6_3_ERROR_SOURCE_FLAG_FIRMWARE_FIRST;
  } else {
    PcieBridgeAerHestTable->Flags = EFI_ACPI_6_3_ERROR_SOURCE_FLAG_GLOBAL;
  }
  PcieBridgeAerHestTable->Enabled = 1;
  PcieBridgeAerHestTable->NumberOfRecordsToPreAllocate = 0x1;
  PcieBridgeAerHestTable->MaxSectionsPerRecord = 0x10;
  PcieBridgeAerHestTable->DeviceControl = 0x7;
  PcieBridgeAerHestTable->UncorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorMask;
  PcieBridgeAerHestTable->UncorrectableErrorSeverity = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorSeverity;
  PcieBridgeAerHestTable->CorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevCorrectedErrorMask;
  PcieBridgeAerHestTable->AdvancedErrorCapabilitiesAndControl = 0;
  PcieBridgeAerHestTable->SecondaryUncorrectableErrorMask = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorMask;
  PcieBridgeAerHestTable->SecondaryUncorrectableErrorSeverity = mPlatformApeiPrivate->PlatRasPolicy.PcieDevUnCorrectedErrorSeverity;
  PcieBridgeAerHestTable->SecondaryAdvancedErrorCapabilitiesAndControl = 0;
  ErrSrcCount++;  //Create PcieBridgeAerHestTable - done.

  //Add structure to HEST table
  AmdRasApeiRmbProtocol->AddMultipleHestErrorSourceEntry ((UINT8*)PcieAerHestTableHead, PcieAerHestTableSize, ErrSrcCount);

  gBS->FreePool (PcieAerHestTableHead);

  //Set PCIE GHES and link to PCIE AER structure
  Status = gBS->AllocatePool (EfiBootServicesData,
                              sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE) ,
                              &NotificationStructure);
  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    //clear instances content
    gBS->SetMem (NotificationStructure, sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE), 0);
  }

  //Setup Notification Type for Root Port Correctable Error structure
  if (mPlatformApeiPrivate->PlatRasPolicy.PcieGhesNotifyType) {
    CorrNotifyType = HARDWARE_ERROR_NOTIFICATION_SCI;
  } else {
    CorrNotifyType = HARDWARE_ERROR_NOTIFICATION_POLLED;
  }

  if (mPlatformApeiPrivate->PlatRasPolicy.PcieUnCorrGhesNotifyType) {
    UnCorrNotifyType = HARDWARE_ERROR_NOTIFICATION_NMI;
  } else {
    UnCorrNotifyType = HARDWARE_ERROR_NOTIFICATION_POLLED;
  }

  NotificationStructure->Type = CorrNotifyType;
  NotificationStructure->Length = sizeof (EFI_ACPI_6_3_HARDWARE_ERROR_NOTIFICATION_STRUCTURE);
  NotificationStructure->PollInterval = mPlatformApeiPrivate->PlatRasPolicy.PciePollInterval;

  //Add Root Port GHES Assist
  AddHestGhes (PCIE_RP_GHES_CORR_SRC_ID,
               PCIE_ROOT_PORT_SRC_ID,
               sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),
               &PcieErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenPcieErrEntry,
               &gPcieGenErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdPcieAerErrBlk
               );
  DEBUG ((EFI_D_ERROR, "Root Port Correctable Error Block Address: 0x%08x\n", (UINTN)mPlatformApeiPrivate->AmdPcieAerErrBlk));

  NotificationStructure->Type = UnCorrNotifyType;
  AddHestGhes (PCIE_RP_GHES_UNCORR_SRC_ID,
               PCIE_ROOT_PORT_SRC_ID,
               sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),
               &PcieErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenPcieErrEntry,
               &gPcieGenErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdPcieAerUnErrBlk
               );
  DEBUG ((EFI_D_ERROR, "Root Port Un-Correctable Error Block Address: 0x%08x\n", (UINTN)mPlatformApeiPrivate->AmdPcieAerUnErrBlk));

  //Add Device GHES Assist
  NotificationStructure->Type = CorrNotifyType;
  AddHestGhes (PCIE_DEV_GHES_CORR_SRC_ID,
               PCIE_DEVICE_SOURCE_ID,
               sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),
               &PcieErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenPcieErrEntry,
               &gPcieGenErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdPcieDevAerErrBlk
               );
  DEBUG ((EFI_D_ERROR, "Devices Correctable Error Block Address: 0x%08x\n", (UINTN)mPlatformApeiPrivate->AmdPcieDevAerErrBlk));

  NotificationStructure->Type = UnCorrNotifyType;
  AddHestGhes (PCIE_DEV_GHES_UNCORR_SRC_ID,
               PCIE_DEVICE_SOURCE_ID,
               sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),
               &PcieErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenPcieErrEntry,
               &gPcieGenErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdPcieDevAerUnErrBlk
               );
  DEBUG ((EFI_D_ERROR, "Devices Un-Correctable Error Block Address: 0x%08x\n", (UINTN)mPlatformApeiPrivate->AmdPcieDevAerUnErrBlk));

  //Add Bridge GHES Assist
  NotificationStructure->Type = CorrNotifyType;
  AddHestGhes (PCIE_BRIDGE_GHES_CORR_SRC_ID,
               PCIE_BRIDGE_SOURCE_ID,
               sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),
               &PcieErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenPcieErrEntry,
               &gPcieGenErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdPcieBridgeAerErrBlk
               );

  NotificationStructure->Type = UnCorrNotifyType;
  AddHestGhes (PCIE_BRIDGE_GHES_UNCORR_SRC_ID,
               PCIE_BRIDGE_SOURCE_ID,
               sizeof (GENERIC_PCIE_AER_ERR_ENTRY_GN),
               &PcieErrorSectGuid,
               (EFI_ACPI_6_3_GENERIC_ERROR_DATA_ENTRY_STRUCTURE*)&gGenPcieErrEntry,
               &gPcieGenErrSrc,
               NotificationStructure,
               &mPlatformApeiPrivate->AmdPcieBridgeAerUnErrBlk
               );

  gBS->FreePool (NotificationStructure);

  return Status;
}
