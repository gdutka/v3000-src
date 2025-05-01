/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include "MpmPldmBcc.h"
#include "BiosStringTable.h"
#include "BiosAttributeTable.h"
#include "BiosAttributeValueTable.h"
#include "BiosAttributePendingValueTable.h"

#define FILECODE MPM_MPMPLDM_MPMPLDMBCC_SETBIOSTABLESTAGS_FILECODE
#if defined(UNIT_TEST_LEVEL)
#include <Library/ShellCommandLib.h>
#endif

// Basic logic for setting up the BIOS Tables and Tags:
// 1. Get boot order then build boot order tables(include string table, AttrTable, AttrValueTable) and calculate tags(CRC32).
// 2. Get table tags from MPM via GetBiosTableTags
// 3. Compare the tags. if mismatch or no tags then SetBios tables and SetBiosTableTags.
// 4. if tags matched, check PendingValueTable, update tables and calculate table tags, then SetBiosTables and SetBIOSTableTags.
// -- Ref. DSP0247, version 1.0.0, Figure 3 - Example of BIOS Table Initialization

/**
  (1) Set BIOS Tables and Tags as well as (2) Update the pending value settings.

  Arguments:

  Event   - The triggered event.
  Context - Context for this event.

  Returns:

  None
**/
VOID
MpmPldmBccReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
)
{
  IDS_HDT_CONSOLE_PSP_TRACE ("%a, Enter %r\n", __FUNCTION__);
  EFI_STATUS Status;
  BIOS_STRING_TABLE_BUFFER gBiosStringTable = {0, 0, NULL};
  BIOS_ATTRIBUTE_TABLE_BUFFER gBiosAttributeTable = {0, 0, NULL};
  BIOS_ATTRIBUTE_VALUE_TABLE_BUFFER gBiosAttributeValueTable = {0, 0, NULL};
  VOID *BstBuffer, *BatBuffer, *BavtBuffer, *BavPendingtBuffer;
  UINT32 BavPendingtSize = sizeof (BIOS_ATTRIBUTE_PENDING_VALUE_TABLE);
  UINT32 BstBufferSize, BatBufferSize, BavtBufferSize;
  UINT32 LocalBiosStringTableTag;
  UINT32 LocalBiosAttributeTableTag;
  UINT32 LocalBiosAttributeValueTableTag;
  UINT32 RemoteBiosStringTableTag;
  UINT32 RemoteBiosAttributeTableTag;
  UINT32 RemoteBiosAttributeValueTableTag;
  EFI_STATUS PldmStatus;
  PldmStatus = 0;
  BstBuffer = NULL;
  BatBuffer = NULL;
  BavtBuffer = NULL;
  BstBufferSize = 0;
  BatBufferSize = 0;
  BavtBufferSize = 0;
  LocalBiosStringTableTag =0;
  LocalBiosAttributeTableTag = 0;
  LocalBiosAttributeValueTableTag = 0;
  RemoteBiosStringTableTag    = 0;
  RemoteBiosAttributeTableTag = 0;
  RemoteBiosAttributeValueTableTag = 0;

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  // Setup the local 3 BIOS tables.
  IDS_HDT_CONSOLE_PSP_TRACE ("Setup BIOS String Table, Attribute Table and AttibuteValue Tables:\n");
  Status = SetupBiosStringTables (&gBiosStringTable);
  if (EFI_ERROR (Status)) goto Exit;
  Status = Crc32BiosStringTable (&gBiosStringTable);
  if (EFI_ERROR (Status)) goto Exit;
  Status = GetBiosStringTableBuffer (&gBiosStringTable, &BstBuffer, &BstBufferSize);
  if (EFI_ERROR (Status)) goto Exit;

  Status = SetupBiosAttributeTables (&gBiosAttributeTable);
  if (EFI_ERROR (Status)) goto Exit;
  Status = Crc32BiosAttributeTable (&gBiosAttributeTable);
  if (EFI_ERROR (Status)) goto Exit;
  Status = GetBiosAttributeTableBuffer (&gBiosAttributeTable, &BatBuffer, &BatBufferSize);
  if (EFI_ERROR (Status)) goto Exit;

  Status = SetupBiosAttributeValueTables (&gBiosAttributeValueTable);
  if (EFI_ERROR (Status)) goto Exit;
  Status = Crc32BiosAttributeValueTable (&gBiosAttributeValueTable);
  if (EFI_ERROR (Status)) goto Exit;
  Status = GetBiosAttributeValueTableBuffer (&gBiosAttributeValueTable, &BavtBuffer, &BavtBufferSize);
  if (EFI_ERROR (Status)) goto Exit;
  IDS_HDT_CONSOLE_PSP_TRACE ("Setup BIOS String Table, Attribute Table and AttibuteValue Tables Status = Success\n");
  // Setup the local 3 BIOS table tags
  LocalBiosStringTableTag            = *(UINT32*)(((UINT8*)BstBuffer) + BstBufferSize - 4);
  LocalBiosAttributeTableTag         = *(UINT32*)(((UINT8*)BatBuffer) + BatBufferSize - 4);
  LocalBiosAttributeValueTableTag    = *(UINT32*)(((UINT8*)BavtBuffer) + BavtBufferSize - 4);

#if !defined(UNIT_TEST_LEVEL) || UNIT_TEST_LEVEL > 1
  AMD_PLDM_PROTOCOL *mPldmProtocol;

  Status = gBS->LocateProtocol (&gAmdPldmProtocolGuid, NULL, (VOID **) &mPldmProtocol);
  if (EFI_ERROR (Status)) goto Exit;

  // The mandatory "SetDateTime" call when starting any interaction session between MC and BIOS.
  // The following sequence of operations are described in PLDM for BIOS Control and Configuration Specification (DSP0247) section 9.
  Status = mPldmProtocol->SetDateTime (mPldmProtocol);
  IDS_HDT_CONSOLE_PSP_TRACE ("PldmSetDateTime Status = %r\n", Status);
  if (EFI_ERROR (Status)) goto Exit;

  // Get the 3 BIOS table tags from remote.
  PldmStatus |= mPldmProtocol->GetBiosTableTags (mPldmProtocol, PLDM_BIOS_STRING_TABLE, &RemoteBiosStringTableTag);
  IDS_HDT_CONSOLE_PSP_TRACE ("GetBiosTages: PLDM_BIOS_STRING_TABLE Status = %r\n", Status);
  PldmStatus |= mPldmProtocol->GetBiosTableTags (mPldmProtocol, PLDM_BIOS_ATTRIBUTE_TABLE, &RemoteBiosAttributeTableTag);
  IDS_HDT_CONSOLE_PSP_TRACE ("GetBiosTages: PLDM_BIOS_ATTRIBUTE_TABLE Status = %r\n", Status);
  PldmStatus |= mPldmProtocol->GetBiosTableTags (mPldmProtocol, PLDM_BIOS_ATTRIBUTE_VALUE_TABLE, &RemoteBiosAttributeValueTableTag);
  IDS_HDT_CONSOLE_PSP_TRACE ("GetBiosTages: PLDM_BIOS_ATTRIBUTE_VALUE_TABLE Status = %r\n", Status);

  // Update BIOS tables and tags when any tag-pair mismatches.
  if (EFI_ERROR (PldmStatus) ||
      (RemoteBiosStringTableTag         != LocalBiosStringTableTag)        ||
      (RemoteBiosAttributeTableTag      != LocalBiosAttributeTableTag)     ||
      (RemoteBiosAttributeValueTableTag != LocalBiosAttributeValueTableTag))
  {
    Status = mPldmProtocol->SetBiosTable (
                              mPldmProtocol,
                              PLDM_BIOS_STRING_TABLE,
                              BstBuffer,
                              &BstBufferSize);
    IDS_HDT_CONSOLE_PSP_TRACE ( "SetBiosTable: PLDM_BIOS_STRING_TABLE Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
    Status = mPldmProtocol->SetBiosTableTags (
                              mPldmProtocol,
                              PLDM_BIOS_STRING_TABLE,
                              LocalBiosStringTableTag);
    IDS_HDT_CONSOLE_PSP_TRACE ("SetBiosTableTags: PLDM_BIOS_STRING_TABLE Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;

    Status = mPldmProtocol->SetBiosTable (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_TABLE,
                              BatBuffer,
                              &BatBufferSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("SetBiosTable: PLDM_BIOS_ATTRIBUTE_TABLE Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
    Status = mPldmProtocol->SetBiosTableTags (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_TABLE,
                              LocalBiosAttributeTableTag);
    IDS_HDT_CONSOLE_PSP_TRACE ("SetBiosTableTags: PLDM_BIOS_ATTRIBUTE_TABLE Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;

    Status = mPldmProtocol->SetBiosTable (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_VALUE_TABLE,
                              BavtBuffer,
                              &BavtBufferSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("SetBiosTable: PLDM_BIOS_ATTRIBUTE_VALUE_TABLE Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
    Status = mPldmProtocol->SetBiosTableTags (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_VALUE_TABLE,
                              LocalBiosAttributeValueTableTag);
    IDS_HDT_CONSOLE_PSP_TRACE ("SetBiosTableTags: PLDM_BIOS_ATTRIBUTE_VALUE_TABLE Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
  }
  BavPendingtBuffer = (PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *) AllocatePool(BavPendingtSize);
  //Get Attribute Pending Value Table
   Status = mPldmProtocol->GetBiosTable (
                    mPldmProtocol,
                    PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE,
                    (PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer,
                    &BavPendingtSize
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    FreePool (BavPendingtBuffer);
    //Allocate buffer to the actual size
    BavPendingtBuffer = (PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT *) AllocatePool (BavPendingtSize);
    Status = mPldmProtocol->GetBiosTable (
                    mPldmProtocol,
                    PLDM_BIOS_ATTRIBUTE_PENDING_VALUE_TABLE,
                    (PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer,
                    &BavPendingtSize
                  );

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Get BIOS ATRIBUTE PENDING VALUE table failed\n");
      goto Exit;
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("Get BIOS ATRIBUTE PENDING VALUE table failed\n");
    goto Exit;
  }

  if ((((PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer)->CompletionCode == PLDM_COMPLETION_CODE_INVALID_DATA_TRANSFER_HANDLE) ||
     (((PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer)->CompletionCode == PLDM_COMPLETION_CODE_INVALID_TRANSFER_OPERATION_FLAG) ||
     (((PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer)->CompletionCode == PLDM_COMPLETION_CODE_BIOS_TABLE_UNAVAILABLE) ||
     (((PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer)->CompletionCode == PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_DATA_INTEGRITY_CHECK) ||
     (((PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer)->CompletionCode == PLDM_COMPLETION_CODE_INVALID_BIOS_TABLE_TYPE)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPending table not found, CompletionCode = 0x%x\n", ((PLDM_GET_BIOS_TABLE_RESPONSE_FORMAT*)&BavPendingtBuffer)->CompletionCode);
    goto Exit;
  } else {
    // Process the Pending Value if any.
    //Currently we only suppport BootOrder and Setup Password pending value table
    IDS_HDT_CONSOLE_PSP_TRACE ("Pending table found\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("Get BIOS Attribute Type = 0x%x\n", ((BIOS_ATTRIBUTE_PENDING_VALUE_TABLE*)&BavPendingtBuffer)->BapvteHeader.BiosAttributeType);
    BOOLEAN PendingIsUpdated = FALSE;
    if (((BIOS_ATTRIBUTE_PENDING_VALUE_TABLE*)&BavPendingtBuffer)->BapvteHeader.BiosAttributeType == PLDM_ATTRIBUTE_TYPE_BIOSBootConfigSetting) {
      Status = UpdatePendingBootConfigSetting (mPldmProtocol, &PendingIsUpdated);
      IDS_HDT_CONSOLE_PSP_TRACE ("UpdatePendingBootConfigSetting Status = %r\n", Status);
      if (EFI_ERROR (Status) || !PendingIsUpdated) goto Exit;
    }

    if (((BIOS_ATTRIBUTE_PENDING_VALUE_TABLE*)&BavPendingtBuffer)->BapvteHeader.BiosAttributeType == PLDM_ATTRIBUTE_TYPE_BIOSPassword) {
      Status = UpdatePendingOemBiosPasswordSetting (mPldmProtocol, &PendingIsUpdated);
      IDS_HDT_CONSOLE_PSP_TRACE ("Update PendingOemBiosSetupPassword Setting Status = %r\n", Status);
      if (EFI_ERROR (Status) || !PendingIsUpdated) goto Exit;
    }
    // Free the previously allocated buffers of BIOS tables and tags in order to start new ones.
    CleanUpBiosStringTable (&gBiosStringTable);
    CleanUpBiosAttributeTable (&gBiosAttributeTable);
    CleanUpBiosAttributeValueTable (&gBiosAttributeValueTable);

    // Setup the local 3 BIOS tables. (again, since the pending value has been updated.)
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: Setup BIOS String Table, Attribute Table and AttibuteValue Tables Status\n");
    Status = SetupBiosStringTables (&gBiosStringTable);
    if (EFI_ERROR (Status)) goto Exit;
    Status = Crc32BiosStringTable (&gBiosStringTable);
    if (EFI_ERROR (Status)) goto Exit;
    Status = GetBiosStringTableBuffer (&gBiosStringTable, &BstBuffer, &BstBufferSize);
    if (EFI_ERROR (Status)) goto Exit;

    Status = SetupBiosAttributeTables (&gBiosAttributeTable);
    if (EFI_ERROR (Status)) goto Exit;
    Status = Crc32BiosAttributeTable (&gBiosAttributeTable);
    if (EFI_ERROR (Status)) goto Exit;
    Status = GetBiosAttributeTableBuffer (&gBiosAttributeTable, &BatBuffer, &BatBufferSize);
    if (EFI_ERROR (Status)) goto Exit;

    Status = SetupBiosAttributeValueTables (&gBiosAttributeValueTable);
    if (EFI_ERROR (Status)) goto Exit;
    Status = Crc32BiosAttributeValueTable (&gBiosAttributeValueTable);
    if (EFI_ERROR (Status)) goto Exit;
    Status = GetBiosAttributeValueTableBuffer (&gBiosAttributeValueTable, &BavtBuffer, &BavtBufferSize);
    if (EFI_ERROR (Status)) goto Exit;
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: Setup BIOS String Table, Attribute Table and AttibuteValue Tables Status = Success\n");

    // Setup the local 3 BIOS table tags (again)
    LocalBiosStringTableTag            = *(UINT32*)(((UINT8*)BstBuffer) + BstBufferSize - 4);
    LocalBiosAttributeTableTag         = *(UINT32*)(((UINT8*)BatBuffer) + BatBufferSize - 4);
    LocalBiosAttributeValueTableTag    = *(UINT32*)(((UINT8*)BavtBuffer) + BavtBufferSize - 4);

    // Update BIOS tables and tags. (again)
    Status = mPldmProtocol->SetBiosTable (
                              mPldmProtocol,
                              PLDM_BIOS_STRING_TABLE,
                              BstBuffer,
                              &BstBufferSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: SetBiosTable: PLDM_BIOS_STRING_TABLE, Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
    Status = mPldmProtocol->SetBiosTableTags (
                              mPldmProtocol,
                              PLDM_BIOS_STRING_TABLE,
                              LocalBiosStringTableTag);
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: SetBiosTableTags: PLDM_BIOS_STRING_TABLE, Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;

    Status = mPldmProtocol->SetBiosTable (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_TABLE,
                              BatBuffer,
                              &BatBufferSize);
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: SetBiosTable: PLDM_BIOS_ATTRIBUTE_TABLE, Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
    Status = mPldmProtocol->SetBiosTableTags (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_TABLE,
                              LocalBiosAttributeTableTag);
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: SetBiosTableTags: PLDM_BIOS_ATTRIBUTE_TABLE, Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;

    Status = mPldmProtocol->SetBiosTable (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_VALUE_TABLE,
                              BavtBuffer,
                              &BavtBufferSize);
    if (EFI_ERROR (Status)) goto Exit;
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: SetBiosTable: PLDM_BIOS_ATTRIBUTE_VALUE_TABLE, Status = %r\n", Status);
    Status = mPldmProtocol->SetBiosTableTags (
                              mPldmProtocol,
                              PLDM_BIOS_ATTRIBUTE_VALUE_TABLE,
                              LocalBiosAttributeValueTableTag);
    IDS_HDT_CONSOLE_PSP_TRACE ("Again: SetBiosTableTags: PLDM_BIOS_ATTRIBUTE_VALUE_TABLE, Status = %r\n", Status);
    if (EFI_ERROR (Status)) goto Exit;
  }

#endif

Exit:

#if defined(UNIT_TEST_LEVEL)
    Print (L"\nBIOS String Table: Size=%d(0x%X)\n", BstBufferSize, BstBufferSize);
    DumpHex (2, 0, BstBufferSize, BstBuffer);
    Print (L"  CRC32(LocalBiosStringTableTag) = 0x%8X\n", LocalBiosStringTableTag);

    Print (L"\nBIOS Attribute Table: Size=%d(0x%X)\n", BatBufferSize, BatBufferSize);
    DumpHex (2, 0, BatBufferSize, BatBuffer);
    Print (L"  CRC32(LocalBiosAttributeTableTag) = 0x%8X\n", LocalBiosAttributeTableTag);

    Print (L"\nBIOS Attribute Value Table: Size=%d(0x%X)\n", BavtBufferSize, BavtBufferSize);
    DumpHex (2, 0, BavtBufferSize, BavtBuffer);
    Print (L"  CRC32(LocalBiosAttributeValueTableTag) = 0x%8X\n", LocalBiosAttributeValueTableTag);
#endif

  CleanUpBiosStringTable (&gBiosStringTable);
  CleanUpBiosAttributeTable (&gBiosAttributeTable);
  CleanUpBiosAttributeValueTable (&gBiosAttributeValueTable);

  return;
}

EFI_STATUS
EFIAPI
SetBiosTablesTags (VOID)
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate SetBiosTableTags ReadyToBoot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MpmPldmBccReadyToBoot,
                  NULL,
                  &gAmdMpmPldmBccProtocolGuid,
                  &Event
                  );
  return Status;
}
