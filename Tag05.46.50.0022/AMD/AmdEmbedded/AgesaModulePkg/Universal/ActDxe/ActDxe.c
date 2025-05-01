/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AgesaConfigLib.h>
#include <Filecode.h>

#define FILECODE UNIVERSAL_ACTDXE_ACTDXE_FILECODE
/**
 * @brief This function is invoked by gEfiEventReadyToBootGuid.
 *        Before booting OS dump all AGESA configuration values
 * 
 * @param Event 
 * @param Context 
 * @return VOID 
 */
VOID
EFIAPI
ActDxeReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                  Status;
  AGESA_CONFIG_DB_HEADER_V1   *ActDbHdr;
  UINTN                       DataSize;
  AGESA_TESTPOINT (TpPspDxeV2RTBCallbackEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "ActDxeReadyToBoot\n");

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  ActDbHdr = NULL;
  Status = gRT->GetVariable (
                  AGESA_CONFIG_DB_POINTER_VARIABLE_NAME,
                  &gAgesaConfigDbPointerVariableGuid,
                  NULL,
                  &DataSize,
                  (VOID *) &ActDbHdr
                  );
  if (EFI_ERROR (Status) || (ActDbHdr == NULL)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Get ActDbHdr variable fail\n");
    return;
  }
  DumpActDb (ActDbHdr);
}
/**
 * @brief Main entry of Act Dxe driver
 *
 * @details Locate ActDb Heap pointer and save to UEFI variable
 *          Dump all configuration value at RTB
 * @param ImageHandle 
 * @param SystemTable 
 * @return EFI_STATUS 
 */
EFI_STATUS
EFIAPI
ActDxeDriverEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_EVENT                   Event;
  EFI_STATUS                  Status;
  AGESA_CONFIG_DB_HEADER_V1   *ActDbHdr;

  ActDbHdr  = NULL;
  IDS_HDT_CONSOLE (MAIN_FLOW, "ActDxeDriverEntry\n");
  IDS_HDT_CONSOLE (MAIN_FLOW, "\tSave ACT DB pointer to UEFI variable\n");
  Status = LocateActDb (&ActDbHdr);
  if (Status != AGESA_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ActDb locate fail <<\n");
  }
  Status = gRT->SetVariable (
                  AGESA_CONFIG_DB_POINTER_VARIABLE_NAME,
                  &gAgesaConfigDbPointerVariableGuid,
                  EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (AGESA_CONFIG_DB_HEADER_V1   *),
                  (VOID *) &ActDbHdr
                  );

  IDS_HDT_CONSOLE (MAIN_FLOW, "\tCreate ReadyToBoot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ActDxeReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "ActDxeDriverEntry Exit\n");
  return EFI_SUCCESS;
}


