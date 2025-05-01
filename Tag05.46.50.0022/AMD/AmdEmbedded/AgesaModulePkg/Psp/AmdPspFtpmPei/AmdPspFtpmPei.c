/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Ppi/AmdFtpmPpi.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/AmdPspRecoveryDetectPpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/AmdFtpmLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/FchBaseLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPFTPMPEI_AMDPSPFTPMPEI_FILECODE

EFI_STATUS
EFIAPI
FtpmMemoryDiscoveredPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );

EFI_STATUS
GetFtpmStatus (
  IN       AMD_FTPM_PPI         *This,
  IN OUT   UINTN                *FtpmStatus
  )
{
  return (FtpmGetInfo (FtpmStatus));
}

EFI_STATUS
SendFtpmCommand (
  IN     AMD_FTPM_PPI         *This,
  IN     VOID                 *CommandBuffer,
  IN     UINTN                 CommandSize
  )
{
  return FtpmSendCommand (
      CommandBuffer,
      CommandSize);
}

EFI_STATUS
GetFtpmResponse (
  IN       AMD_FTPM_PPI          *This,
  IN OUT   VOID                  *ResponseBuffer,
  IN OUT   UINTN                 *ResponseSize
  )
{

  EFI_STATUS Status;
  Status = FtpmGetResponse (
                            ResponseBuffer,
                            ResponseSize
                          );
  return (Status);
}

EFI_STATUS
ExecuteFtpmCommand (
  IN       AMD_FTPM_PPI         *This,
  IN       VOID                 *CommandBuffer,
  IN       UINTN                CommandSize,
  IN OUT   VOID                 *ResponseBuffer,
  IN OUT   UINTN                *ResponseSize
  )
{
  return FtpmExecuteCommand (
                              CommandBuffer,
                              CommandSize,
                              ResponseBuffer,
                              ResponseSize
                             );
}


STATIC AMD_FTPM_PPI mAmdPspFtpmPpi = {
  ExecuteFtpmCommand,
  GetFtpmStatus,
  SendFtpmCommand,
  GetFtpmResponse,
};

STATIC AMD_FTPM_FACTORY_RESET_PPI mAmdPspFtpmFactoryResetPpi = {
  0x00000001ul
};

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdPspFtpmPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFtpmPpiGuid,
  &mAmdPspFtpmPpi
};

STATIC EFI_PEI_PPI_DESCRIPTOR mAmdPspFtpmFactoryResetPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdFtpmFactoryResetPpiGuid,
  &mAmdPspFtpmFactoryResetPpi
};

EFI_PEI_NOTIFY_DESCRIPTOR   mMemoryDicoverPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  FtpmMemoryDiscoveredPpiCallback
};

STATIC AMD_PSP_RECOVERY_DETECT_PPI mPspRecoveryDetectPpi = {
  0x01
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPspRecoveryDetectedPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPspRecoveryDetectPpiGuid,
  &mPspRecoveryDetectPpi
};

/*++

Routine Description:

  Check PSP NVRAM status which used for RPMC feature

Arguments:

Returns:

  EFI_STATUS

--*/

EFI_STATUS
PspCapabilitiesCheck (
  )
{
  EFI_STATUS                  Status;
  UINT32                      PspCapabilities;
  UINT64                      PspNvEntryAddr;
  UINT32                      PspNvEntrySize;


  PspNvEntryAddr = 0;
  PspNvEntrySize = 0;
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.PspCapabilitiesCheck start\n");

  PspCapabilities = 0;
  Status = CheckPspCaps (&PspCapabilities);

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Get PSP Capabilities fail, status = 0x%x\n", Status);
    return Status;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("PSP Capabilities is 0x%x\n", PspCapabilities);

  //if PSP NVRAM Exists but not healthy, set PcdAmdPspNvramClearRequest to TRUE
  if ((PspCapabilities & PSP_NVRAM_HEALTH) == 0) {
    if (PSPEntryInfoV2 (PSP_NVRAM, &PspNvEntryAddr, &PspNvEntrySize) == TRUE) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PSP NVRAM clear is required\n");
      //Set PCD for PSP NVRAM clear
      PcdSetBoolS(PcdAmdPspNvramClearRequest, TRUE);
    }
  } else {
    PcdSetBoolS(PcdAmdPspNvramClearRequest, FALSE);
  }

  if ((PspCapabilities & PSP_NVRAM_USING_RPMC_PROTECTION) == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP RPMC is fail or NOT enabled\n");
    PcdSetBoolS(PcdAmdPspNvramUsingRpmc, FALSE);
  } else {
    PcdSetBoolS(PcdAmdPspNvramUsingRpmc, TRUE);
  }

  return Status;
}

EFI_STATUS
EFIAPI
FtpmMemoryDiscoveredPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  EFI_STATUS                  Status;
  EFI_PHYSICAL_ADDRESS        FtpmCommandBuffer;
  EFI_PHYSICAL_ADDRESS        FtpmResponseBuffer;
  UINT32                      TpmCapabilities;

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.FtpmPei.MemoryDiscoveredPpiCallback\n");

  TpmCapabilities = 0;

  // Check PSP capabilities firstly
  Status = PspCapabilitiesCheck ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PspCapabilitiesCheck fail or not supported, PSP NVRAM is skipped\n");
  }

  //If PSP fTPM feature is NOT enabled, exit the callback.
  if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_PSP_FTPM) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PSP fTPM feature is NOT enabled, PSP ftpm is skipped\n");
    return EFI_SUCCESS;
  }

  AGESA_TESTPOINT (TpPspfTpmPeiMemCallbackEntry, NULL);
  //Send PSP mailbox command to ensure Ctrl Area have been initialed
  Status = CheckFtpmCaps (&TpmCapabilities);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Get fTPM cap fail\n");
    return Status;
  }
  //Supported & factory reset can't be set at the same time
  ASSERT (TpmCapabilities  != (PSP_CAP_TPM_SUPPORTED | PSP_CAP_TPM_REQ_FACTORY_RESET));
  ASSERT (TpmCapabilities  != (PSP_CAP_TPM_SUPPORTED | PSP_CAP_FTPM_NEED_RECOVERY));

  if ((TpmCapabilities & PSP_CAP_TPM_SUPPORTED) == PSP_CAP_TPM_SUPPORTED) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate memory for FTPM\n");
    Status = (*PeiServices)->AllocatePages((CONST EFI_PEI_SERVICES **)PeiServices,
                                           EfiBootServicesData,
                                           EFI_SIZE_TO_PAGES (FTPM_COMMAND_BUFFER_SIZE + \
                                           FTPM_RESPONSE_BUFFER_SIZE),
                                           &FtpmCommandBuffer);

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Allocate Memory fail\n");
      return Status;
    }

    FtpmResponseBuffer      = FtpmCommandBuffer + FTPM_COMMAND_BUFFER_SIZE;

    SetMem ((VOID *) (UINTN) FtpmCommandBuffer, FTPM_COMMAND_BUFFER_SIZE + FTPM_RESPONSE_BUFFER_SIZE, 0);

    IDS_HDT_CONSOLE_PSP_TRACE ("AssignMemory for FTPM\n");
    Status = FtpmAssignMemory (
      (UINTN)FtpmCommandBuffer,
      FTPM_COMMAND_BUFFER_SIZE,
      (UINTN)FtpmResponseBuffer,
      FTPM_RESPONSE_BUFFER_SIZE
    );

    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("FtpmAssignMemory fail\n");
      return Status;
    }

    IDS_HDT_CONSOLE_PSP_TRACE ("\tAllocate CmdBuf:0x%lx ResBuf:0x%lx\n", FtpmCommandBuffer, FtpmResponseBuffer);

    IDS_HDT_CONSOLE_PSP_TRACE ("\tInstall PspTis PPI\n");
    Status = (**PeiServices).InstallPpi((CONST EFI_PEI_SERVICES **)PeiServices, &mAmdPspFtpmPpiList);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("InstallPpi fail\n");
    }
  }

  if ((TpmCapabilities & PSP_CAP_TPM_REQ_FACTORY_RESET) == PSP_CAP_TPM_REQ_FACTORY_RESET) {
    IDS_HDT_CONSOLE_PSP_TRACE ("FTPM FACTORY_RESET detected\n");
    PcdSetBoolS (PcdAmdFtpmNvCorrupted, TRUE);
    //Install a PPI to notify SBIOS
    IDS_HDT_CONSOLE_PSP_TRACE ("\tInstall fTPM factory reset PPI\n");
    Status = (**PeiServices).InstallPpi((CONST EFI_PEI_SERVICES **)PeiServices, &mAmdPspFtpmFactoryResetPpiList);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("InstallPpi fail\n");
    }
  }

  if ((TpmCapabilities & PSP_CAP_FTPM_NEED_RECOVERY) == PSP_CAP_FTPM_NEED_RECOVERY) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Ftpm Recovery Flag Detected\n");
    Status = (**PeiServices).InstallPpi((CONST EFI_PEI_SERVICES **)PeiServices, &mPspRecoveryDetectedPpiList);
    ASSERT (Status == EFI_SUCCESS);
  }
  AGESA_TESTPOINT (TpPspfTpmPeiMemCallbackExit, NULL);
  return Status;
}


/*++

Routine Description:

  PSP Driver Entry. Initialize PSP device and report DRAM info to PSP once found

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
AmdPspFtpmPeiDriverEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT32                      Ignored;
  UINT64                      EntryValue;

  //If PSP feature turn off, exit the driver
  if ((CheckPspDevicePresent () == FALSE) ||
      (PcdGetBool (PcdAmdPspEnable) == 0) ||
      (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_PSP_FTPM)) {
    return EFI_SUCCESS;
  }

  // Skip fTPM if Bit5 in value entry has been set
  PSPEntryInfo (AMD_SOFT_FUSE_CHAIN_01, &EntryValue, &Ignored);
  if (EntryValue & BIT5) {
    return EFI_SUCCESS;
  }

  AGESA_TESTPOINT (TpPspfTpmPeiEntry, NULL);

  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.FtpmPei Enter\n");

  //
  // Check if Boot mode is boot then hook for MemoryDiscovery protocol
  //
  if ((FchReadSleepType () != 3) && (AmdCapsuleGetStatus () == FALSE)) {
    Status = (**PeiServices).NotifyPpi (PeiServices, &mMemoryDicoverPpiCallback);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("FtpmPei Exit\n");
  AGESA_TESTPOINT (TpPspfTpmPeiExit, NULL);

  return Status;
}



