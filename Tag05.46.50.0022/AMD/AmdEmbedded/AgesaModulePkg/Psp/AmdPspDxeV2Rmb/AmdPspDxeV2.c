/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision$   @e \$Date$
 *
 */
#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/MpService.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Protocol/SmmCommunication.h>
#include <Protocol/AmdCoreTopologyV2Protocol.h>
#include <Protocol/FabricTopologyServices2.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <Protocol/AmdPspCommonServiceProtocol.h>
#include <Protocol/Tcg2Protocol.h>
#include <Protocol/DxeSmmReadyToLock.h>
//#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#define APCB_V3_OPERATION    1
#if APCB_V3_OPERATION
#include "Porting.h"
#include <Addendum/Apcb/Inc/RMB/ApcbV3Arch.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3Priority.h>
#include <Addendum/Apcb/Inc/RMB/ApcbMemDimmConfig.h>
#include <Addendum/Apcb/Inc/RMB/ApcbDataGroups.h>
#include <Library/ApcbChecksumLibV3.h>
#else // APCB_V3_OPERATION
#include <Library/ApcbZpLib.h>
#endif // APCB_V3_OPERATION
#include <Library/MemRestore.h>
#include <Addendum/Apcb/Inc/RMB/APOB.h>
#include <Library/AmdPspApobLib.h>
#include <Protocol/PspPlatformProtocol.h>
#include <Library/CcxPspLib.h>
#include <Library/AmdPspFlashUpdateLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/ApobCommonServiceLib.h>
#include <AmdPspSmmCommunication.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Filecode.h>
#include <Library/DxeServicesLib.h>
#include <Protocol/AcpiTable.h>
#include <Library/AmdPspFlashAccLib.h>
#include <Gnb.h>
#include <GnbRegistersRMB.h>
#include <Library/SmnAccessLib.h>
#include <Library/MpmLib.h>
#include <Library/MpmFuncLib.h>
#include <Library/MpmMboxLib.h>
#include <Library/AmdPspFwImageHeaderLib.h>
#include <Library/IoLib.h>

#define FILECODE PSP_AMDPSPDXEV2_AMDPSPDXEV2_FILECODE

#define APOB_UNIQUE_APCB_INSTANCE_INVALID_VALUE     0xDEADBEEFul

#define PSP_BAR_SIZE                        0x100000ul             ///< Size of PSP BAR
#define NBMSIC_SMN_BASE                     0x13B10000ul           ///< Define the SMN Base address for NB MISC register
#define NBMSIC_PSP_BASE_ADDR_LO_OFFSET      0x2E0                ///< Define the offset of PSP_BASE_ADDR_LO in NB MISC block
#define NBMSIC_PSP_BASE_ADDR_HI_OFFSET      0x2E4                ///< Define the offset of PSP_BASE_ADDR_HI in NB MISC block

#define ABL_MEM_PMU_TRAIN_ERROR 0x4001
#define ABL_MEM_AGESA_MEMORY_TEST_ERROR 0x4003

#define MP0_PUBLIC0_SMN_BASE                0x3800000

#define ACPI_MMIO_BASE       0xFED80000ul
#define PMIO_BASE            0x300
#define FCH_PMIOA_REG5B      0x5B

#define AMD_DRTM_SSDT_GUID \
  { 0xb2b0063d, 0x9326, 0x46f0, 0x83, 0x65, 0x25, 0x48, 0x48, 0x96, 0xf5, 0x1b }

#define IOHC0NBCFG_SMN_ADDR                 0x13B00000ul           ///< Define the SMN Base address for NB MISC register
#define FASTREG_BASE_ADDR_LO_OFFSET         0x10300ul              ///< Define the offset of CCP_BASE_ADDR_LO in NB MISC block
#define FASTREG_BASE_ADDR_HI_OFFSET         0x10304ul              ///< Define the offset of CCP_BASE_ADDR_HI in NB MISC block
#define CCP_BASE_ADDR_LO_OFFSET             0x102D8ul              ///< Define the offset of CCP_BASE_ADDR_LO in NB MISC block
#define CCP_BASE_ADDR_HI_OFFSET             0x102DCul              ///< Define the offset of CCP_BASE_ADDR_HI in NB MISC block
#define CCP_BAR_SIZE                        0x100000ul             ///< Size of CCP BAR

#define TCG_LOG_DMA_PROTECTION_DISABLED         \
  "DMA Protection Disabled"

extern EFI_GUID gPspFlashAccSmmCommReadyProtocolGuid;
extern EFI_GUID gAmdApcbDxeServiceProtocolGuid;

EFI_SMM_CONTROL2_PROTOCOL              *mSmmControl;
PSP_PLATFORM_PROTOCOL                 *mPspPlatfromProtocol;

EFI_STATUS
ReadC2PMsgRegRn (
  IN       UINTN      Socket,
  IN       UINTN      Die,
  IN       UINT32     C2pMsgOffset,
  IN OUT   UINT32     *RegValue
  );

VOID AsmSaveS3Context (
  IN OUT VOID *
);

typedef struct _APCB_RECOVERY_MAP
{
  BOOLEAN                     Optional;       //Ignore the existence check for Optional Instance
  UINT8                       SourceType;
  UINT8                       SourceInstance;
  UINT8                       DestType;
  UINT8                       DestInstance;
} APCB_RECOVERY_MAP;

STATIC AMD_PSP_COMMON_SERVICE_PROTOCOL mPspCommonServiceProtocol = {
  GetFtpmControlAreaV2,
  SwitchPspMmioDecodeV2,
  CheckPspDevicePresentV2,
  CheckFtpmCapsV2,
  PSPEntryInfoV2,
  PspLibTimeOutV2,
  ReadC2PMsgRegRn
};

/*
 *  Routine to read a C2P register.
 *
 * @param[in] Socket             Processor socket to read from
 * @param[in] Die                Die number on Socket to read from
 * @param[in] C2pMsgOffset       SMN address offset, e.g. MP0_C2PMSG_37_SMN_OFFSET
 * @param[in] Instance           Instance ID of the target fabric device
 * @retval    AGESA_BOUNDS_CHK   Invalid Socket or Die
 * @retval    AGESA_SUCCESS      Value retrieved successfully
 */
EFI_STATUS
ReadC2PMsgRegRn (
  IN       UINTN      Socket,
  IN       UINTN      Die,
  IN       UINT32     C2pMsgOffset,
  IN OUT   UINT32     *RegValue
  )
{
  EFI_STATUS                           Status;
  UINT8                                SmuRegInstanceId;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL   *NbioSmuServices;

  SmuRegInstanceId = (UINT8) Socket;
  NbioSmuServices = NULL;
  Status = gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, &NbioSmuServices);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate SmuServices Protocol fail\n");
    return Status;
  }

  NbioSmuServices->AmdSmuRegisterRead (NbioSmuServices, SmuRegInstanceId, MP0_PUBLIC0_SMN_BASE + C2pMsgOffset, RegValue);

  return EFI_SUCCESS;
}

/*++

Routine Description:

  This function is invoked by EFI_EVENT_SIGNAL_LEGACY_BOOT.
  Before booting to legacy OS, inform SMM

Arguments:

  Event   - The triggered event.
  Context - Context for this event.

Returns:

  None

--*/
VOID
PspNotifySmmDrivers (
  VOID
  )
{

  UINT8 Cmd;
  EFI_STATUS Status;
  //Below two protocols haven't been added to driver's depex, assume it should be ready at RTB
  Status = gBS->LocateProtocol (&gEfiSmmControl2ProtocolGuid, NULL, &mSmmControl);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate SmmCtrl Protocol Fail\n");
    return;
  }

  //Locate Psp Platform Protocol to get customized information
  IDS_HDT_CONSOLE_PSP_TRACE ("Locate Psp Platform Protocol\n");
  Status = gBS->LocateProtocol (
                    &gPspPlatformProtocolGuid,
                    NULL,
                    &mPspPlatfromProtocol
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate Psp Platform Protocol Fail\n");
    return;
  }

  Cmd = mPspPlatfromProtocol->SwSmiCmdtoBuildContext;

  IDS_HDT_CONSOLE_PSP_TRACE ("PspNotifySmmDrivers Cmd[%x]\n", Cmd);
  mSmmControl->Trigger (mSmmControl, &Cmd, NULL, FALSE, 0);
}

VOID
SetPspAddrMsrTask (
  IN  VOID                                     *Buffer
  )
{
  UINT64 Tmp64;
  //Get PSP BAR Address
  Tmp64 = 0;
  Tmp64 = *((UINT64 *)Buffer);

  UpdatePspAddr (Tmp64);
}

/**
 * @brief Get MMIO for CCP
 *
 * @return EFI_STATUS
 */
EFI_STATUS
GetCcpMmioBase (
  UINT64                 *CcpMmioBase
  )
{
  UINT32                 MmioBaseLo;
  UINT32                 MmioBaseHi;
  if (SocFamilyIdentificationCheckV2 (F19_RMB_B0_FAMILY_ID_BASE_MODEL)) {
    //In RMB B0 and onward we use CCP BAR
    MmioBaseLo = ReadSmn (0, IOHC0NBCFG_SMN_ADDR + CCP_BASE_ADDR_LO_OFFSET);
    MmioBaseLo &= 0xFFF00000;
    if (MmioBaseLo == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("CCP MMIO not initialized\n");
      //Not initialed
      return EFI_NOT_READY;
    }
    MmioBaseHi = ReadSmn (0, IOHC0NBCFG_SMN_ADDR + CCP_BASE_ADDR_HI_OFFSET);
  } else {
    //In RMB A0 we use FASTREGBAR as CCP BAR
    MmioBaseLo = ReadSmn (0, IOHC0NBCFG_SMN_ADDR + FASTREG_BASE_ADDR_LO_OFFSET);
    MmioBaseLo &= 0xFFF00000;
    if (MmioBaseLo == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("CCP MMIO not initialized\n");
      //Not initialed
      return EFI_NOT_READY;
    }
    MmioBaseHi = ReadSmn (0, IOHC0NBCFG_SMN_ADDR + FASTREG_BASE_ADDR_HI_OFFSET);
  }
  *CcpMmioBase = ((UINT64) MmioBaseHi << 32) + MmioBaseLo;
  return EFI_SUCCESS;
}

/**
 * @brief Enable RDRAND RDSEED Instruction
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EnableRdInstruction (
  )
{
  EFI_STATUS  Status;
  EFI_MP_SERVICES_PROTOCOL                *MpServices;
  UINT64                                   CcpMmioBase;


  IDS_HDT_CONSOLE_PSP_TRACE ("EnableRdInstruction\n");
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);

  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate MP Protocol fail\n");
    return EFI_UNSUPPORTED;
  }
  //Enable CCP BAR
  Status = GetCcpMmioBase (&CcpMmioBase);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //Set PspAddrMsr on all cores
  IDS_HDT_CONSOLE_PSP_TRACE ("Set PspAddrMsr %x for APs\n", CcpMmioBase);
  MpServices->StartupAllAPs (
                        MpServices,                                    // EFI_MP_SERVICES_PROTOCOL *this
                        (EFI_AP_PROCEDURE) SetPspAddrMsrTask,         // EFI_AP_PROCEDURE
                        FALSE,
                        NULL,                                          // EFI_EVENT WaitEvent OPTIONAL
                        0,                                             // UINTN Timeout (Unsupported)
                        &CcpMmioBase,                                  // VOID *ProcArguments OPTIONAL
                        NULL                                           // Failed CPUList OPTIONAL (unsupported)
                        );
  return EFI_SUCCESS;
}

VOID
PspMpServiceCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Interface;

  AGESA_TESTPOINT (TpPspDxeV2MpCallBackEntry, NULL);
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  Status = EnableRdInstruction ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tEnableRd Failed\n");
    AGESA_TESTPOINT (TpEnableRdFail, NULL);
  }
  DumpRdInstructionInfo ();
  AGESA_TESTPOINT (TpPspDxeV2MpCallBackExit, NULL);
}

/**
 *  Prepare the APOB Buffer and write to BIOS Directory Entry 0x63
 *
 *
 *  @retval EFI_SUCCESS       The Data save to FLASH successfully
 *
 **/
EFI_STATUS
PspWriteApobS3Entry (
  VOID
  )
{
  APOB_HEADER                     *ApobDramPtr;
  BOOLEAN                         ApobEntryExist;
  TYPE_ATTRIB                     TypeAttrib;
  UINT64                          EntryAddress;
  UINT32                          EntrySize;
  UINT64                          EntryDest;
  APOBLIB_INFO                    *ApobInfo;
  UINT8                           *Buffer;

  Buffer = NULL;
  IDS_HDT_CONSOLE_PSP_TRACE ("PspWriteApobS3Entry\n");

  if (AmdPspGetApobInfo (&ApobInfo) != EFI_SUCCESS) {
    IDS_HDT_CONSOLE_PSP_TRACE ("No APOB memory copy detected\n");
    return EFI_UNSUPPORTED;
  }

  ApobDramPtr = (APOB_HEADER *) (UINTN) ApobInfo->ApobAddr;

  //Check the existence and size of APOB NV entry 0x63
  ApobEntryExist = BIOSEntryInfo (APOB_NV_COPY, INSTANCE_IGNORED, &TypeAttrib, &EntryAddress, &EntrySize, &EntryDest);
  if (!ApobEntryExist) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PspWriteApobS3Entry Entry not exist\n");
    return EFI_UNSUPPORTED;
  }
  //Check the size of APOB DRAM copy
  if (ApobDramPtr->Size > EntrySize) {
    IDS_HDT_CONSOLE_PSP_TRACE ("APOB SPI Entry too small\n");
    return EFI_UNSUPPORTED;
  }
  Buffer = AllocateZeroPool (ApobDramPtr->Size);
  ASSERT (Buffer != NULL);
  if (Buffer == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate temp buffer fail\n");
    return EFI_OUT_OF_RESOURCES;
  }
  MapSpiDataToBuffer ((UINT32) (UINTN) EntryAddress, Buffer, ApobDramPtr->Size);
  //Compare if any changes
  if (CompareMem (ApobDramPtr, Buffer, ApobDramPtr->Size)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Data difference between APOB DRAM copy & SPI copy\n");
    PspUpdateFlash ((UINT32) EntryAddress, ApobDramPtr->Size, ApobDramPtr);
  }
  if (Buffer != NULL) {
    FreePool (Buffer);
  }
  return EFI_SUCCESS;
}

//if clear NV RAM success, return TRUE, otherwise, return FALSE
BOOLEAN
ClearNvRam (
  IN UINT32                               NVRamType,
  IN UINT64                               NvEntryAddr,
  IN UINT32                               NvEntrySize,
  IN UINTN                                FlashBlockSize
  )
{
  UINTN                           NumBytes;
  EFI_STATUS                      Status;
  UINT8                           *Buffer;

  ASSERT (FlashBlockSize > 0);
  Buffer = NULL;

  if ((NvEntryAddr != 0) && (NvEntrySize != 0)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("    Erasing NV RAM 0x%x at %lx, Size:%x...\n", NVRamType, NvEntryAddr, NvEntrySize);
      NumBytes = NvEntrySize;

      if (((NvEntryAddr % FlashBlockSize) == 0) && ((NumBytes % FlashBlockSize) == 0)) {
        Status = PspEraseFlash ((UINTN)NvEntryAddr, &NumBytes);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("    Erase NV RAM 0x%x Failed\n", NVRamType);
          ASSERT (FALSE);
          return FALSE;
        } else {
          return TRUE;
        }
      } else { //address unaligned
        IDS_HDT_CONSOLE_PSP_TRACE ("    Warning: Entry Addr %lx or Size %x of NV RAM 0x%x is NOT aligned with block size %lx\n", NvEntryAddr, NvEntrySize, NVRamType, FlashBlockSize);
        Buffer = AllocatePool (NumBytes);
        SetMem (Buffer, NumBytes, 0xFF);
        PspWriteFlash ((UINTN)NvEntryAddr, &NumBytes, Buffer);
        FreePool (Buffer);
        return TRUE;
      }
  }
  return FALSE;
}


/**
 *  Clear PSP NV RAM and reset system if fTPM is disabled and PSP NV RAM is corrupted
 *
 **/
VOID ClearNvRamIfCorrupted (
 )
{
  UINT64                               PspNvEntryAddr;
  UINT32                               PspNvEntrySize;
  UINTN                                BlockSize;

  PspNvEntryAddr = 0;
  PspNvEntrySize = 0;

  // if PSP/Pluton fTPM is not enabled
  if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_PSP_FTPM &&
      PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_HSP_FTPM) {
    if ((PcdGetBool (PcdAmdPspNvramClearRequest) == TRUE)) {
      PSPEntryInfoV2 (PSP_NVRAM, &PspNvEntryAddr, &PspNvEntrySize);
      //Get the block size
      PspGetFlashBlockSize (&BlockSize);

      if (ClearNvRam (PSP_NVRAM, PspNvEntryAddr, PspNvEntrySize, BlockSize) == TRUE) {
        IDS_HDT_CONSOLE_PSP_TRACE ("PSP NVRAM clearred, reset system\n");
        gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
        //Should not come here
        CpuDeadLoop ();
      }
    }
  }
}

VOID
ApcbRecoveryByInstance (
  UINT8             ApcbInstance
  )
{
  TYPE_ATTRIB       TypeAttrib;
  UINT64            ActiveApcbEntryAddress;
  UINT64            BackupApcbEntryAddress;
  UINT32            ActiveApcbSize;
  UINT32            BackUpApcbSize;
  UINT64            EntryDest;
  UINT8             *Buffer;
  UINT32            SubProgram;


  IDS_HDT_CONSOLE_PSP_TRACE ("Recover Active APCB Instance %x\n", ApcbInstance);
  SubProgram = 0;
  ApobGetSubProgram (&SubProgram);
  IDS_HDT_CONSOLE_PSP_TRACE ("APCB SubProgram = %x\n", SubProgram);
  TypeAttrib.SubProgram = SubProgram & 0x7;
  //Query APCB Backup instace information
  if (BIOSEntryInfo (BIOS_APCB_INFO_BACKUP, ApcbInstance, &TypeAttrib, &BackupApcbEntryAddress, &BackUpApcbSize, &EntryDest) == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Fatal Error: Backup Apcb Not Detected\n");
    ASSERT (FALSE);
    AGESA_TESTPOINT (TpPspRecoverApcbFail, NULL);
    return;
  }
  //Query APCB active instace information
  if (BIOSEntryInfo (BIOS_APCB_INFO, ApcbInstance, &TypeAttrib, &ActiveApcbEntryAddress, &ActiveApcbSize, &EntryDest) == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Fatal Error: Active Apcb instance Not Detected\n");
    ASSERT (FALSE);
    AGESA_TESTPOINT (TpPspRecoverApcbFail, NULL);
    return;
  }
  //Do size check between APCB active instace & APCB Backup instace
  ASSERT (ActiveApcbSize >= BackUpApcbSize);
  if (ActiveApcbSize < BackUpApcbSize) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ActiveApcbSize too small\n");
    ASSERT (FALSE);
    AGESA_TESTPOINT (TpPspRecoverApcbFail, NULL);
    return;
  }
  //Start APCB recovery
  //Allocate temp memory to hold backup apcb
  Buffer = NULL;
  Buffer = AllocateZeroPool (BackUpApcbSize);
  ASSERT (Buffer != NULL);
  if (Buffer == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate temp buffer fail\n");
    return;
  }
  MapSpiDataToBuffer ((UINT32) (UINTN) BackupApcbEntryAddress, Buffer, BackUpApcbSize);
  //Restore APCB active instance SPI region
  IDS_HDT_CONSOLE_PSP_TRACE ("Restore APCB active instance SPI region [0x%x] with 0x%x bytes from Buffer [0x%x]\n", ActiveApcbEntryAddress, BackUpApcbSize, Buffer);
  PspUpdateFlash ((UINT32) ActiveApcbEntryAddress, BackUpApcbSize, Buffer);
  IDS_HDT_CONSOLE_PSP_TRACE ("Restore APCB Instance %x Completed, Restart the system", ApcbInstance);
  FreePool (Buffer);
}

APCB_RECOVERY_MAP mApcbRecMap[] = {
// Optional SourceType                   SourceInstance    DestType          DestInstance
  {FALSE,   BIOS_APCB_INFO_BACKUP,       0x08,             BIOS_APCB_INFO,   0x00},
  {TRUE,    BIOS_APCB_INFO_BACKUP,       0x09,             BIOS_APCB_INFO,   0x01},
};

VOID
ApcbRecovery (
  )
{
  EFI_STATUS        Status;
  UINT32            i;
  BOOLEAN           ApcbRecoveryFlag;
  BOOLEAN           RecoverySucceed;
  TYPE_ATTRIB       TypeAttrib;
  UINT64            DestApcbEntryAddress;
  UINT64            BackupApcbEntryAddress;
  UINT32            DestApcbSize;
  UINT32            BackUpApcbSize;
  UINT64            EntryDest;
  UINT8             *Buffer;
  APCB_HEADER       *ApcbHeader;
  UINT8             Value8;
  UINT32            PspFwImageHeaderLen;

  ApcbRecoveryFlag = FALSE;
  RecoverySucceed = FALSE;
  if (PcdGetBool (PcdAmdPspApcbRecoveryEnable) == FALSE) {
    // if ApcbRecovery feature disabled,
    //Update the CMOS[6:7] with flag 0x5555 to indicate the APCB recovery diabled by X86
    //On following boot, ABL should always read APCB from type 0x60,
    //and never set the ApcbRecoveryFlag except CMOS been cleared
    IDS_HDT_CONSOLE_PSP_TRACE ("PcdAmdPspApcbRecoveryEnable Set, Ignore recovery, set CMOS flag 0x5555 to notify ABL\n");

    Value8 = 0x06;
    LibAmdIoWrite (AccessWidth8, 0x72, &Value8, NULL);
    Value8 = 0x55;
    LibAmdIoWrite (AccessWidth8, 0x73, &Value8, NULL);

    Value8 = 0x07;
    LibAmdIoWrite (AccessWidth8, 0x72, &Value8, NULL);
    Value8 = 0x55;
    LibAmdIoWrite (AccessWidth8, 0x73, &Value8, NULL);
    return;
  }

  Status = ApobGetApcbRecoveryFlag (&ApcbRecoveryFlag);
  ASSERT (Status == EFI_SUCCESS);
  if (ApcbRecoveryFlag) {
    IDS_HDT_CONSOLE_PSP_TRACE ("APCB.RecoveryFlag Set\n");
    //Loop the mapping structure
    for (i = 0; i < sizeof (mApcbRecMap)/sizeof (APCB_RECOVERY_MAP); i++) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Recover Source Type %x Instance %x to Dest Type %x Instance %x\n", mApcbRecMap[i].SourceType, mApcbRecMap[i].SourceInstance, mApcbRecMap[i].DestType, mApcbRecMap[i].DestInstance);
      if (BIOSEntryInfo (mApcbRecMap[i].SourceType, mApcbRecMap[i].SourceInstance, &TypeAttrib, &BackupApcbEntryAddress, &BackUpApcbSize, &EntryDest) == FALSE) {
        //Performe the existence check if optional filed set to FALSE
        if (mApcbRecMap[i].Optional == FALSE) {
            IDS_HDT_CONSOLE_PSP_TRACE ("Fatal Error: Required Backup Apcb Type[%X] Instance[%X] Not Detected\n", mApcbRecMap[i].SourceType, mApcbRecMap[i].SourceInstance);
            ASSERT (FALSE);
            AGESA_TESTPOINT (TpPspRecoverApcbFail, NULL);
            return;
        } else {
          IDS_HDT_CONSOLE_PSP_TRACE ("Optional Backup Apcb Type[%X] Instance[%X] Not Detected, continue\n", mApcbRecMap[i].SourceType, mApcbRecMap[i].SourceInstance);
          continue;
        }
      }
      //Source exist, then check the exist of target
      if (BIOSEntryInfo (mApcbRecMap[i].DestType, mApcbRecMap[i].DestInstance, &TypeAttrib, &DestApcbEntryAddress, &DestApcbSize, &EntryDest) == FALSE) {
        //Performe the existence check if optional filed set to FALSE
        if (mApcbRecMap[i].Optional == FALSE) {
            IDS_HDT_CONSOLE_PSP_TRACE ("Fatal Error: Required Dest Apcb Type[%X] Instance[%X] Not Detected\n", mApcbRecMap[i].SourceType, mApcbRecMap[i].SourceInstance);
            ASSERT (FALSE);
            AGESA_TESTPOINT (TpPspRecoverApcbFail, NULL);
            return;
        } else {
          IDS_HDT_CONSOLE_PSP_TRACE ("Optional Backup Apcb Type[%X] Instance[%X] Not Detected, continue\n", mApcbRecMap[i].SourceType, mApcbRecMap[i].SourceInstance);
          continue;
        }
      }
      //Both Dest and Target APCB intance exist, check if target size is enough to hold the source
      // ASSERT (DestApcbSize >= BackUpApcbSize);
      if (DestApcbSize < BackUpApcbSize) {
        IDS_HDT_CONSOLE_PSP_TRACE ("DestApcbSize too small\n");
        AGESA_TESTPOINT (TpPspRecoverApcbFail, NULL);
        continue;
      }
      //Start APCB recovery
      IDS_HDT_CONSOLE_PSP_TRACE ("Start APCB recovery\n");
      //Allocate temp memory to hold APCB recovery instance
      Buffer = NULL;
      Buffer = AllocateZeroPool (DestApcbSize);
      ASSERT (Buffer != NULL);
      if (Buffer == NULL) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Allocate temp buffer fail\n");
        continue;
      }
      //Copy Recovery APCB instance from SPI to DRAM
      MapSpiDataToBuffer ((UINT32) (UINTN) BackupApcbEntryAddress, Buffer, BackUpApcbSize);
      //Update the UniqueApcbInstance with new random value
      ApcbHeader = (APCB_HEADER *) Buffer;
      // Check Apcb Data has PSP Fw Image Header or not
      if (ApcbFwImageHeaderCheck (BackupApcbEntryAddress, &PspFwImageHeaderLen) == TRUE) {
        ApcbHeader = (APCB_HEADER *) (Buffer + PspFwImageHeaderLen);
      }
      IDS_HDT_CONSOLE_PSP_TRACE ("UniqueApcbInstance Update from %x", ApcbHeader->UniqueApcbInstance);
      ApcbHeader->UniqueApcbInstance = (UINT32) (AsmReadTsc () & 0x00FFFFFFul);
      IDS_HDT_CONSOLE_PSP_TRACE (" to  %x\n", ApcbHeader->UniqueApcbInstance);
      //Update the checksum
      ApcbUpdateCheckSum (ApcbHeader, ApcbHeader->SizeOfApcb, mApcbRecMap[i].DestType, mApcbRecMap[i].DestInstance);
      //Recover APCB active instance SPI region
      IDS_HDT_CONSOLE_PSP_TRACE ("Recover APCB instance SPI region [0x%x] with 0x%x bytes from Buffer [0x%x]\n", DestApcbEntryAddress, DestApcbSize, Buffer);
      Status = PspUpdateFlash ((UINT32) DestApcbEntryAddress, DestApcbSize, Buffer);
      IDS_HDT_CONSOLE_PSP_TRACE ("Recover Status %x\n", Status);
      if (Buffer != NULL) {
        FreePool (Buffer);
      }
      if (Status == EFI_SUCCESS) {
        RecoverySucceed = TRUE;
      }
    }
    if (RecoverySucceed) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Recover Succeed, set CMOS flag, reset system\n");
      //Update the CMOS[6:7] with flag 0xA55A to indicate the APCB recovery succeed
      Value8 = 0x06;
      LibAmdIoWrite (AccessWidth8, 0x72, &Value8, NULL);
      Value8 = 0x5A;
      LibAmdIoWrite (AccessWidth8, 0x73, &Value8, NULL);

      Value8 = 0x07;
      LibAmdIoWrite (AccessWidth8, 0x72, &Value8, NULL);
      Value8 = 0xA5;
      LibAmdIoWrite (AccessWidth8, 0x73, &Value8, NULL);

      gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
      //Should not come here
      CpuDeadLoop ();
    }
  }
}

VOID
SyncSystemTpmConfig (
  IN AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol,
  OUT BOOLEAN                        *IsTpmConfigChanged
  )
{
  EFI_STATUS                      Status;
  UINT8                           ApcbTpmConfig;
  UINT8                           SystemTpmConfig;
  UINT8                           ApcbPurpose;
  UINT8                           ApcbTpmAlgSmx;
  UINT8                           SystemTpmAlgSmx;

  // Check if System TPM config synchronization feature is enabled
  if (PcdGetBool (PcdAmdPspTpmConfigSynchronizationEnabled) == FALSE) {
    return;
  }

  *IsTpmConfigChanged = FALSE;
  ASSERT (ApcbDxeServiceProtocol != NULL);

  //system TPM config is set by SBIOS in PEI phase
  //step 1: sync TPM SMx algo flag between SBIOS and APCB
  SystemTpmAlgSmx = PcdGet8 (PcdAmdPspSystemTpmAlgSmx);
  //read APCB config
  ApcbTpmAlgSmx = SYSTEM_TPM_ALG_SMX_UNSUPPORTED;
  Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX, &ApcbTpmAlgSmx);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("1.Unable to get APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX, Status:%r\n", Status);
  } else {
    if (SystemTpmAlgSmx != ApcbTpmAlgSmx) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SystemTpmAlgSmx: %d, ApcbTpmAlgSmx: %d, update TPM SMx flag in APCB\n", SystemTpmAlgSmx, ApcbTpmAlgSmx);
      Status = ApcbDxeServiceProtocol->ApcbSetToken8 (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_NORMAL, APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX, SystemTpmAlgSmx);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX, Status:%r\n", Status);
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX, &ApcbTpmAlgSmx);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("2.Unable to get APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX, Status:%r\n", Status);
        } else {
          if (SystemTpmAlgSmx == ApcbTpmAlgSmx) { //update successfully
            *IsTpmConfigChanged = TRUE;
          } else { //update failed
            IDS_HDT_CONSOLE_PSP_TRACE ("Critial error: update to APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_ALG_SMX doesn't take effect\n");
          }
        }
      }
    }
  }

  //step 2: sync TPM config
  //system TPM config is set by SBIOS in PEI phase
  SystemTpmConfig = PcdGet8 (PcdAmdPspSystemTpmConfig);
  //read APCB TPM config
  ApcbTpmConfig = SYSTEM_TPM_CONFIG_NONE;
  Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, &ApcbTpmConfig);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("1.Unable to get APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, Status:%r\n", Status);
  } else {
    if (SystemTpmConfig != ApcbTpmConfig) { //ABL TPM config is out of date, update it and reset system
      IDS_HDT_CONSOLE_PSP_TRACE ("SystemTpmConfig: %d, ApcbTpmConfig: %d, update TPM config in APCB\n", SystemTpmConfig, ApcbTpmConfig);
      Status = ApcbDxeServiceProtocol->ApcbSetToken8 (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_NORMAL, APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, SystemTpmConfig);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, Status:%r\n", Status);
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, &ApcbTpmConfig);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("2.Unable to get APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG, Status:%r\n", Status);
        } else {
          if (SystemTpmConfig == ApcbTpmConfig) { //update successfully
            *IsTpmConfigChanged = TRUE;
          } else { //update failed
            IDS_HDT_CONSOLE_PSP_TRACE ("Critial error: update to APCB token APCB_TOKEN_UID_PSP_SYSTEM_TPM_CONFIG doesn't take effect\n");
          }
        }
      }
    }
  }
}

VOID
SyncSystemMpmConfig (
  IN AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol,
  OUT BOOLEAN                        *IsMpmConfigChanged
  )
{
  EFI_STATUS                      Status;
  UINT8                           ApcbPurpose;
  UINT8                           ApcbWirelessManageabilityValue;
  UINT8                           PcdMpmWirelessManageabilityValue;
  UINT8                           PcdMpmWiredKvmEnableValue;
  UINT8                           ApcbMpmWiredKvmEnableValue;
  UINT8                           PcdMpmWirelessKvmEnableValue;
  UINT8                           ApcbMpmWirelessKvmEnableValue;
  // Check if System TPM config synchronization feature is enabled
  if (PcdGetBool (PcdAmdMpmConfigSynchronizationEnabled) == FALSE) {
    return;
  }

  // Check MPM Enable
  if ((!IsMpmEnabled()) || (PcdGetBool (PcdMpmWirelessManageability) == FALSE)) {
    return;
  }

  *IsMpmConfigChanged = FALSE;
  ASSERT (ApcbDxeServiceProtocol != NULL);

  //sync MpmWirelessManageability between SBIOS and APCB
  PcdMpmWirelessManageabilityValue = PcdGetBool (PcdMpmWirelessManageability);
  //read APCB config
  ApcbWirelessManageabilityValue = 0;
  Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, &ApcbWirelessManageabilityValue);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("1.Unable to get APCB token APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, Status:%r\n", Status);
  } else {
    if (PcdMpmWirelessManageabilityValue != ApcbWirelessManageabilityValue) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PcdMpmWirelessManageabilityValue: %d\n", PcdMpmWirelessManageabilityValue);
      IDS_HDT_CONSOLE_PSP_TRACE ("ApcbWirelessManageabilityValue: %d\n", ApcbWirelessManageabilityValue);
      IDS_HDT_CONSOLE_PSP_TRACE ("update MpmWirelessManageability in APCB\n");
      Status = ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_NORMAL, APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, PcdMpmWirelessManageabilityValue);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set APCB token APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, Status:%r\n", Status);
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, &ApcbWirelessManageabilityValue);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("Unable to get APCB token APCB_TOKEN_UID_WIRELESS_MANAGEABILITY, Status:%r\n", Status);
        } else {
          if (PcdMpmWirelessManageabilityValue == ApcbWirelessManageabilityValue) { //update successfully
            *IsMpmConfigChanged = TRUE;
          } else { //update failed
            IDS_HDT_CONSOLE_PSP_TRACE ("Critial error: update to APCB token APCB_TOKEN_UID_WIRELESS_MANAGEABILITY doesn't take effect\n");
          }
        }
      }
    }
  }

  //sync PcdMpmWiredKvmEnable between SBIOS and APCB
  PcdMpmWiredKvmEnableValue = PcdGetBool (PcdMpmWiredKvmEnable);
  //read APCB config
  ApcbMpmWiredKvmEnableValue = 0;
  Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, &ApcbMpmWiredKvmEnableValue);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("1.Unable to get APCB token APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, Status:%r\n", Status);
  } else {
    if (PcdMpmWiredKvmEnableValue != ApcbMpmWiredKvmEnableValue) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PcdMpmWiredKvmEnableValue: %d, ApcbMpmWiredKvmEnableValue: %d, update MpmWiredKvmEnableValue in APCB\n", PcdMpmWiredKvmEnableValue, ApcbMpmWiredKvmEnableValue);
      Status = ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_NORMAL, APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, PcdMpmWiredKvmEnableValue);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set APCB token APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, Status:%r\n", Status);
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, &ApcbMpmWiredKvmEnableValue);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("Unable to get APCB token APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, Status:%r\n", Status);
        } else {
          if (PcdMpmWiredKvmEnableValue == ApcbMpmWiredKvmEnableValue) { //update successfully
            *IsMpmConfigChanged = TRUE;
          } else { //update failed
            IDS_HDT_CONSOLE_PSP_TRACE ("Critial error: update to APCB token APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY doesn't take effect\n");
          }
        }
      }
    }
  }

  //sync MpmWirelessKvmEnable between SBIOS and APCB
  PcdMpmWirelessKvmEnableValue = PcdGetBool (PcdMpmWirelessKvmEnable);
  //read APCB config
  ApcbMpmWirelessKvmEnableValue = 0;
  Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY, &ApcbMpmWirelessKvmEnableValue);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("1.Unable to get APCB token APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY, Status:%r\n", Status);
  } else {
    if (PcdMpmWirelessKvmEnableValue != ApcbMpmWirelessKvmEnableValue) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PcdMpmWirelessKvmEnableValue: %d\n", PcdMpmWirelessKvmEnableValue);
      IDS_HDT_CONSOLE_PSP_TRACE ("ApcbMpmWirelessKvmEnableValue: %d\n", ApcbMpmWirelessKvmEnableValue);
      IDS_HDT_CONSOLE_PSP_TRACE ("update MpmWirelessKvmEnableValue in APCB\n");
      Status = ApcbDxeServiceProtocol->ApcbSetTokenBool (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_NORMAL, APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY, PcdMpmWirelessKvmEnableValue);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set APCB token APCB_TOKEN_UID_KVM_WIRED_MANAGEABILITY, Status:%r\n", Status);
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetTokenBool (ApcbDxeServiceProtocol, &ApcbPurpose, APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY, &ApcbMpmWirelessKvmEnableValue);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("Unable to get APCB token APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY, Status:%r\n", Status);
        } else {
          if (PcdMpmWirelessKvmEnableValue == ApcbMpmWirelessKvmEnableValue) { //update successfully
            *IsMpmConfigChanged = TRUE;
          } else { //update failed
            IDS_HDT_CONSOLE_PSP_TRACE ("Critial error: update to APCB token APCB_TOKEN_UID_KVM_WIRELESS_MANAGEABILITY doesn't take effect\n");
          }
        }
      }
    }
  }
}

VOID
SyncSystemDevicesConfig (
  IN AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol
  )
{
  BOOLEAN            TpmConfigChanged = FALSE;
  BOOLEAN            MpmConfigChanged = FALSE;
  SyncSystemTpmConfig (ApcbDxeServiceProtocol, &TpmConfigChanged);
  SyncSystemMpmConfig (ApcbDxeServiceProtocol, &MpmConfigChanged);
  if ((TpmConfigChanged) || (MpmConfigChanged)) {
    //Write back updated data to SPI
    ApcbDxeServiceProtocol->ApcbFlushData (ApcbDxeServiceProtocol);

    IDS_HDT_CONSOLE_PSP_TRACE ("Issue warm reset since TPM or MPM config changed...\n");
    gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    //Should not come here
    CpuDeadLoop ();
  }
}

VOID
ApcbAccReadyCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                      Status;
  UINT32                          SizeInByte;
  BOOLEAN                         MemRestoreEnabled;
  AMD_APCB_SERVICE_PROTOCOL       *mApcbDxeServiceProtocol;

  AGESA_TESTPOINT (TpPspDxeV2ApcbAccCallBackEntry, NULL);

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unable to locate APCB Protocol");
    return ;
  }

  SyncSystemDevicesConfig (mApcbDxeServiceProtocol);

  MemRestoreEnabled = FALSE;

  Status = mApcbDxeServiceProtocol->ApcbGetConfigParameter (mApcbDxeServiceProtocol, APCB_ID_CONFIG_MEMRESTORECTL, &SizeInByte, (UINT64 *)&MemRestoreEnabled);
  IDS_HOOK (IDS_HOOK_PSP_MEM_RESTORE_ENABLED, NULL, (VOID *)&MemRestoreEnabled);
  IDS_HDT_CONSOLE_PSP_TRACE ("Memory Context Restore requested: %d\n", MemRestoreEnabled);

  if ((MemRestoreEnabled && !AmdMemRestoreGetFailure ())) {
    //Save APOB to NV
    PspWriteApobS3Entry ();
  }

  //Do APCB recovery if APOB.APCBRecovery Flag set
  ApcbRecovery ();

  AGESA_TESTPOINT (TpPspDxeV2ApcbAccCallBackExit, NULL);
}

VOID
FlashAccSmmReadyCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       SecondaryEvent;
  VOID                            *Interface;
  VOID                            *Registration;

  AGESA_TESTPOINT (TpPspDxeV2FlashAccCallBackEntry, NULL);
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gPspFlashAccSmmCommReadyProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  //if NV RAM corrupted, we need to clear it when necessary
  ClearNvRamIfCorrupted ();

  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate Protocol event for Install APCB Protocol\n");
  SecondaryEvent = NULL;
  SecondaryEvent = EfiCreateProtocolNotifyEvent (
                      &gAmdApcbDxeServiceProtocolGuid,
                      TPL_NOTIFY,
                      ApcbAccReadyCallBack,
                      NULL,
                      &Registration
                      );
  ASSERT (SecondaryEvent != NULL);
  if (SecondaryEvent == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate event fail\n");
  }

  AGESA_TESTPOINT (TpPspDxeV2FlashAccCallBackExit, NULL);
}

/**
 *  Close Psp Smm Communication Interface
 *
 *
 *  @retval EFI_SUCCESS       The Data save to FLASH successfully
 *
 **/
EFI_STATUS
ClosePspSmmCommunication (
  )
{
  EFI_STATUS Status;
  UINT8       *Buffer;
  EFI_SMM_COMMUNICATE_HEADER *SmmCommBuff;
  PSP_SMM_COMMUNICATION_CMN  *PspSmmCloseInterfaceBuff;
  UINTN       SmmCommBufferSize;
  EFI_SMM_COMMUNICATION_PROTOCOL *SmmCommunication = NULL;

  IDS_HDT_CONSOLE_PSP_TRACE ("ClosePspSmmCommunication\n");
  //Assume gEfiSmmCommunicationProtocolGuid should be ready at ExitBs
  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
  ASSERT (Status == EFI_SUCCESS);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Locate SmmCommunicationProtocol fail \n");
    return Status;
  }

  //Init SMM communication buffer header
  Buffer = (UINT8 *) (UINTN) PcdGet64 (PcdAmdSmmCommunicationAddress);
  ASSERT (Buffer != NULL);
  if (Buffer != NULL) {
    ZeroMem (Buffer, PSP_CMM_COMM_CLOSE_INTERFACE_BUFFER_SIZE);
    SmmCommBuff = (EFI_SMM_COMMUNICATE_HEADER *) Buffer;
    CopyGuid (&SmmCommBuff->HeaderGuid, &gPspSmmCommHandleGuid);
    SmmCommBuff->MessageLength = sizeof (PSP_SMM_COMMUNICATION_CMN);
    //Init PSP SMM communicate private data
    PspSmmCloseInterfaceBuff = (PSP_SMM_COMMUNICATION_CMN *) &(((EFI_SMM_COMMUNICATE_HEADER *) Buffer)->Data);
    PspSmmCloseInterfaceBuff->id = PSP_SMM_COMM_ID_CLOSE_INTERFACE;

    //Communicate AMD SMM communication handler to close the FlashACC SMI interface
    SmmCommBufferSize = PSP_CMM_COMM_CLOSE_INTERFACE_BUFFER_SIZE;
    SmmCommunication->Communicate (SmmCommunication, Buffer, &SmmCommBufferSize);
  }
  return EFI_SUCCESS;
}

VOID
TpmMeasureTsmeStatus (
  )
{
  EFI_STATUS                  Status;
  EFI_TCG2_PROTOCOL           *Tcg2Protocol;
  UINT32                      HSTIState;
  UINT8                       TsmeStatus;
  EFI_TCG2_EVENT              *TcgEvent;

  // Check if Tsme status measurement is enabled
  if (PcdGetBool (PcdAmdPspTsmeStatusTpmMeasurement) == FALSE) {
    return;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Tpm Measure Tsme Status\n");

  //Locate gEfiTcg2ProtocolGuid protocol
  Status = gBS->LocateProtocol(&gEfiTcg2ProtocolGuid, NULL, (VOID **)&Tcg2Protocol);
  if (EFI_ERROR(Status))
  {
    IDS_HDT_CONSOLE_PSP_TRACE("Sorry, failed to locate Tcg2 Protocol\n");
    return;
  }

  //Check Ryzen Pro SKU
  HSTIState = 0;
  PspMboxBiosQueryHSTIState (&HSTIState);
  if ((HSTIState & PSP_RYZEN_PRO_SKU_FLAG) == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Tsme Status measurement is not supported in this SKU\n");
    return;
  }

  TsmeStatus = (HSTIState & PSP_TSME_STATUS) ? 1 : 0;
  IDS_HDT_CONSOLE_PSP_TRACE ("Tsme Status:%d.\n", TsmeStatus);

  TcgEvent = AllocateZeroPool(sizeof(EFI_TCG2_EVENT) - sizeof(TcgEvent->Event) + sizeof(TsmeStatus));
   if(TcgEvent == NULL) {
     IDS_HDT_CONSOLE_PSP_TRACE ("Allocate pool for TcgEvent failed in Tsme Status measurement\n");
     return;
   }

  TcgEvent->Header.HeaderSize = sizeof(EFI_TCG2_EVENT_HEADER);
  TcgEvent->Header.HeaderVersion = EFI_TCG2_EVENT_HEADER_VERSION;
  TcgEvent->Header.PCRIndex = 2;
  TcgEvent->Header.EventType = TCG_EVENT_AMD_BIOS_TSME_MEASUREMENT;
  TcgEvent->Size = sizeof (EFI_TCG2_EVENT) - sizeof (TcgEvent->Event) + sizeof (TsmeStatus);
  CopyMem (&TcgEvent->Event, &TsmeStatus, sizeof (TsmeStatus));

  //Extend the TSME state reported by PSP into PCR2 and generate a corresponding TCG event log entry
  Tcg2Protocol->HashLogExtendEvent (Tcg2Protocol, 0, (EFI_PHYSICAL_ADDRESS)&TsmeStatus, sizeof(TsmeStatus), TcgEvent);

  //if TSME state is disabled, extend the TSME state into PCR7 and generate a corresponding TCG event log entry
  if (TsmeStatus == 0) {
    TcgEvent->Header.HeaderSize = sizeof(EFI_TCG2_EVENT_HEADER);
    TcgEvent->Header.HeaderVersion = EFI_TCG2_EVENT_HEADER_VERSION;
    TcgEvent->Header.PCRIndex = 7;
    TcgEvent->Header.EventType = TCG_EVENT_AMD_BIOS_TSME_MEASUREMENT;
    TcgEvent->Size = sizeof (EFI_TCG2_EVENT) - sizeof (TcgEvent->Event) + sizeof (TsmeStatus);
    CopyMem (&TcgEvent->Event, &TsmeStatus, sizeof (TsmeStatus));
    Tcg2Protocol->HashLogExtendEvent (Tcg2Protocol, 0, (EFI_PHYSICAL_ADDRESS)&TsmeStatus, sizeof(TsmeStatus), TcgEvent);
  }

  FreePool (TcgEvent);
  IDS_HDT_CONSOLE_PSP_TRACE ("Tpm Measure Tsme Status Exit\n");
}

VOID
TpmMeasureDmaProtectionStatus (
  )
{
  EFI_STATUS                  Status;
  EFI_TCG2_PROTOCOL           *Tcg2Protocol;
  UINT32                      EventSize;
  CHAR8                       *TcgLogDmaProtectionDisabled;
  EFI_TCG2_EVENT              *TcgEvent;

  // Check if DMA Protection status measurement is enabled
  if (PcdGetBool (PcdAmdPspDmaProtectionStatusTpmMeasurement) == FALSE) {
    return;
  }

  // Check if TPM is disabled
  if (PcdGet8 (PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_NONE) {
    return;
  }

  if (PcdGetBool (PcdCfgIommuSupport) == TRUE && PcdGetBool (PcdDmaProtection) == TRUE) {
    return;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("Tpm Measure Dma Protection Status\n");
  TcgLogDmaProtectionDisabled = TCG_LOG_DMA_PROTECTION_DISABLED;

  //Locate gEfiTcg2ProtocolGuid protocol
  Status = gBS->LocateProtocol(&gEfiTcg2ProtocolGuid, NULL, (VOID **)&Tcg2Protocol);
  if (EFI_ERROR(Status))
  {
    IDS_HDT_CONSOLE_PSP_TRACE("Sorry, failed to locate Tcg2 Protocol\n");
    return;
  }
  EventSize = (UINT32)AsciiStrLen (TcgLogDmaProtectionDisabled);
  TcgEvent = AllocateZeroPool(sizeof(EFI_TCG2_EVENT) - sizeof(TcgEvent->Event) + EventSize);
  if(TcgEvent == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Allocate pool for TcgEvent failed in Dma Protection Status measurement\n");
    return;
  }

  // if DMA Protection is disabled, extend the DMA Protection status into PCR7
  // and generate a corresponding TCG event log entry
  TcgEvent->Header.HeaderSize = sizeof(EFI_TCG2_EVENT_HEADER);
  TcgEvent->Header.HeaderVersion = EFI_TCG2_EVENT_HEADER_VERSION;
  TcgEvent->Header.PCRIndex = 7;
  TcgEvent->Header.EventType = EV_EFI_ACTION;
  TcgEvent->Size = sizeof (EFI_TCG2_EVENT) - sizeof (TcgEvent->Event) + EventSize;
  CopyMem (&TcgEvent->Event, TcgLogDmaProtectionDisabled, EventSize);
  Tcg2Protocol->HashLogExtendEvent (Tcg2Protocol, 0, (EFI_PHYSICAL_ADDRESS)TcgLogDmaProtectionDisabled, EventSize, TcgEvent);

  FreePool (TcgEvent);
  IDS_HDT_CONSOLE_PSP_TRACE ("Tpm Measure Dma Protection Status Exit\n");
}

/*
  Save BSP and AP S3 context out of SMM
*/
VOID SaveS3Context ()
{
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  VOID        *S3BspEntryPoint;
  VOID        *S3ApEntryPoint;
  EFI_STATUS  Status;

  IDS_HDT_CONSOLE_PSP_TRACE ("Locate MP Protocol\n");

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    goto Exit;
  }


  //Locate Psp Platform Protocol to get customized information
  IDS_HDT_CONSOLE_PSP_TRACE ("Locate Psp Platform Protocol\n");
  Status = gBS->LocateProtocol (
                    &gPspPlatformProtocolGuid,
                    NULL,
                    &mPspPlatfromProtocol
                    );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    goto Exit;
  }


  S3BspEntryPoint = (VOID*)(UINTN)mPspPlatfromProtocol->RsmHandOffInfo->RsmEntryPoint;
  S3ApEntryPoint = S3BspEntryPoint;

  AsmSaveS3Context ((VOID*)S3BspEntryPoint);

  Status = MpServices->StartupAllAPs (
            MpServices,
            AsmSaveS3Context,
            TRUE, //Single thread
            NULL,
            0,
            S3ApEntryPoint,
            NULL
            );
Exit:
  IDS_HDT_CONSOLE_PSP_TRACE ("SaveS3Context exit %x\n", Status);
  return;
}

EFI_STATUS
EFIAPI
PublishDrtmSsdtTable (
  )
{
  EFI_STATUS                                Status;
  UINT32                                    HSTIState;
  UINTN                                     TableList;
  UINTN                                     Size;
  EFI_ACPI_COMMON_HEADER                    *CurrentTable;
  INTN                                      Instance;
  EFI_GUID                                  AmdDrtmSsdtGuid = AMD_DRTM_SSDT_GUID;
  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;

  //Check Ryzen Pro SKU or force publish Drtm virtual device
  HSTIState = 0;
  PspMboxBiosQueryHSTIState (&HSTIState);

  if (((HSTIState & PSP_RYZEN_PRO_SKU_FLAG) == PSP_RYZEN_PRO_SKU_FLAG) && \
  (PcdGetBool(PcdAmdPspDrtmVirtualDevice) == TRUE) && \
  (PcdGet8(PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_DTPM || \
  PcdGet8(PcdAmdPspSystemTpmConfig) == SYSTEM_TPM_CONFIG_HSP_FTPM)) {

    IDS_HDT_CONSOLE_PSP_TRACE ("DRTM virtual device is supported in this SKU\n");
    Status = gBS->LocateProtocol (
                    &gEfiAcpiTableProtocolGuid,
                    NULL,
                    &AcpiTableProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Instance     = 0;
    CurrentTable = NULL;
    Status = GetSectionFromAnyFv (
              &AmdDrtmSsdtGuid,
              EFI_SECTION_RAW,
              Instance,
              (VOID**)&CurrentTable,
              &Size
              );
    if (!EFI_ERROR (Status)) {
      if (((EFI_ACPI_DESCRIPTION_HEADER*) CurrentTable)->OemTableId != SIGNATURE_64 ('D', 'R', 'T', 'M', 0, 0, 0, 0)) {
        return EFI_NOT_FOUND;
      }
    } else {
      return EFI_NOT_FOUND;
    }

    TableList = 0;
    Status = AcpiTableProtocol->InstallAcpiTable (
              AcpiTableProtocol,
              CurrentTable,
              Size,
              &TableList);
    if (!EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Install DRTM SSDT Success.\n");
    }
    FreePool(CurrentTable);
    return Status;
  } else {
    return EFI_SUCCESS;
  }
}
#define SCPC_AUTO_ENABLEMENT_VARIABLE_NAME               L"BuiltAsSecuredCorePC"
// {77fa9abd-0359-4d32-bd60-28f4e78f784b}
EFI_GUID gScpcAutoEnablementVariableGuid = {0x77fa9abd, 0x0359, 0x4d32, {0xbd, 0x60, 0x28, 0xf4, 0xe7, 0x8f, 0x78, 0x4b}};

/**
 * @brief ScpcAutoEnablement by setting UEFI variable "BuiltAsSecuredCorePC"
 *
 * @details Read PCD value PcdScpcAutoEnablement
 *          if set to disable, AGESA will delete EFI variable "BuiltAsSecuredCorePC" if detected
 *          If set to enable, AGESA will set EFI variable "BuiltAsSecuredCorePC" to non-zero value, which
 *          works a identifier to OS, the Secured-core Auto enablement is turn on
 *
 * @return EFI_STATUS
 */
EFI_STATUS
ScpcAutoEnablement (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINTN                       VariableSize;
  UINT8                       ScpcAutoEnablementFlag;
  BOOLEAN                     SetVariableRequired;
  UINT8                       PcdScpcAutoEnablementVal;

  SetVariableRequired = FALSE;
  VariableSize = sizeof (UINT8);
  PcdScpcAutoEnablementVal = PcdGet8 (PcdScpcAutoEnablement);
  if (PcdScpcAutoEnablementVal == 1) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement Enable\n");
    //PCD enabled, check if variable has been set, and if the value is non-zero
    Status = gRT->GetVariable (
                    SCPC_AUTO_ENABLEMENT_VARIABLE_NAME,
                    &gScpcAutoEnablementVariableGuid,
                    NULL,
                    &VariableSize,
                    (VOID *) &ScpcAutoEnablementFlag
                    );
    if (Status == EFI_SUCCESS) {
      //Variable already exsited, check if the value is non-zero
      if (ScpcAutoEnablementFlag == 0) {
        //The value is zero, set the flag to set UEFI variable "BuiltAsSecuredCorePC" to 1
        SetVariableRequired = TRUE;
      }
    } else if (Status == EFI_BUFFER_TOO_SMALL) {
      //A variable with larger size detected, exit with warning message
      AGESA_TESTPOINT (TpScpcAutoEnFailBigSizeVar, NULL);
      IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement Fail: A variable with larger size detected\n");
      return Status;
    } else {
      //Variable not existed, set the flag to set UEFI variable "BuiltAsSecuredCorePC" to 1
      SetVariableRequired = TRUE;
    }
    //Do the flag check to avoid unnecessary SPI write
    if (SetVariableRequired) {
      //Set UEFI variable "BuiltAsSecuredCorePC" to 1
      ScpcAutoEnablementFlag = 1;
      Status = gRT->SetVariable (
                      SCPC_AUTO_ENABLEMENT_VARIABLE_NAME,
                      &gScpcAutoEnablementVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      VariableSize,
                      (VOID *) &ScpcAutoEnablementFlag
                      );
      if (EFI_ERROR (Status)) {
        AGESA_TESTPOINT (TpScpcAutoEnFailSetVar, NULL);
        IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement Fail: SetVariable %r\n", Status);
        return Status;
      }
    }
  } else if (PcdScpcAutoEnablementVal == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement disable\n");
    //PCD set to FALSE, if variable existed, need to delete the EFI variable by setting variable size to 0
    Status = gRT->GetVariable (
                    SCPC_AUTO_ENABLEMENT_VARIABLE_NAME,
                    &gScpcAutoEnablementVariableGuid,
                    NULL,
                    &VariableSize,
                    (VOID *) &ScpcAutoEnablementFlag
                    );
    if (Status == EFI_SUCCESS) {
      Status = gRT->SetVariable (
                      SCPC_AUTO_ENABLEMENT_VARIABLE_NAME,
                      &gScpcAutoEnablementVariableGuid,
                      0,
                      0,
                      NULL
                      );
      if (EFI_ERROR (Status)) {
        AGESA_TESTPOINT (TpScpcAutoEnFailDelVar, NULL);
        IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement Fail: DelVariable %r\n", Status);
        return Status;
      }
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement Exit\n");
  return EFI_SUCCESS;
}
/*++

Routine Description:

  This function is invoked by gEfiEventReadyToBootGuid.
  Before booting to legacy OS, inform PSP that BIOS is transitioning
  preparing for S3 usage.

Arguments:

  Event   - The triggered event.
  Context - Context for this event.

Returns:

  None

--*/
VOID
PspDxeReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                  Status;

  AGESA_TESTPOINT (TpPspDxeV2RTBCallbackEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("PspDxeReadyToBoot\n");

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  // Tpm measure Tsme status
  TpmMeasureTsmeStatus ();

  //ScpcAutoEnablement
  Status = ScpcAutoEnablement ();
  IDS_HDT_CONSOLE_PSP_TRACE ("ScpcAutoEnablement %r\n", Status);

  if (IsMpmEnabled ()) {
    BiosMpmCmdDisableUnProvision ();
    IDS_HDT_CONSOLE_PSP_TRACE ("BiosMpmCmdDisableUnProvision\n");
  }

  // Tpm measure Dma Protection status
  TpmMeasureDmaProtectionStatus ();

  //Notify PSP Driver about RTB, PSP SMM driver will save the Core register to S3 Save region
  //BSP and AP will wake inside of SMM
  PspNotifySmmDrivers ();

  if (!PcdGetBool (PcdAmdPspS3WakeFromSmm)) {
    // SaveS3Context enables BSP and APs to wake outside of SMM.
    SaveS3Context ();
  }

  //Notify PSP FW Boot done
   PspMboxBiosCmdExitBootServices ();
  //Publish DRTM SSDT table
  PublishDrtmSsdtTable ();
  AGESA_TESTPOINT (TpPspDxeV2RTBCallbackExit, NULL);
}

VOID
PspDxeExitBS (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  MPM_PRVIATE_HEAP_DATA_V1  *MpmData;
  EFI_STATUS                Status;
  UINT8                     Value = 0;
  AGESA_TESTPOINT (TpPspDxeV2ExitBsCallbackEntry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("PspDxeExitBS\n");

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  if (IsMpmEnabled () == TRUE) {
    Status = LocateMpmPrivateData (&MpmData);
    if (!EFI_ERROR (Status)) {
      if (MpmData->WirelessTxrPending) {
        BiosMpmCmdTerminateTcr();
      }
      BiosMpmCmdBootDone ();
      //WirelessKvmFlag = 0xFFFF by default
      //It will be set to 1 if there is wireless KVM pending, to 0 if wired KVM pending
      if ((MpmData->WirelessKvmFlag == 1) || (MpmData->WirelessKvmFlag == 0)) {
        PspMboxBiosMpmPciAccess (DISABLE_MPM_PCI_ACC, MpmData->WirelessKvmFlag);
      } else if (MpmData->WirelessTxrPending) {
        PspMboxBiosMpmPciAccess (DISABLE_MPM_PCI_ACC, 1); //TCR must be Wireless
      }
      //Set rtcshadow register to keep system off when power resumes
      Value = MmioRead8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5B);
      Value = Value & 0xFC; // set bit1:0 to 00' (reflect bit 5:4), always power off after power resumes
      MmioWrite8 (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5B, Value);
    }
  }
  //Close PSP SMM communication interface mainly Flash Access to void security vulnerability
  ClosePspSmmCommunication ();
  AGESA_TESTPOINT (TpPspDxeV2ExitBsCallbackExit, NULL);
}

VOID
PspPciEnumerationCompleteCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Interface;
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }
  //Hide HSP PCI CFG space
  //Todo uncomment the PCD check
  // if (PcdGet8 (PcdAmdPspSystemTpmConfig) != SYSTEM_TPM_CONFIG_HSP_FTPM) {
  // Move it AmdHspFtpmDxe.c
  //  SmnRegisterRMW (0, SMN_NBIF0INTERNAL_RCC_DEV2_EPF2_STRAP0_ADDRESS, (UINT32) ~RCC_DEV2_EPF2_STRAP0_STRAP_FUNC_EN_DEV2_F2_MASK, 0, GNB_REG_ACC_FLAG_S3SAVE);
  // }
  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  PspMboxBiosLockDFReg ();
  if (PcdGetBool(PcdAmdPspLockeSPIEnable)) {
    PspMboxBiosLockeSPIReg ();
  }
}

VOID
EFIAPI
PspDxeSmmReadyToLockCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Interface;
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //Make sure the hook ONLY called one time.
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  PspMboxBiosSmmLock ();

}

EFI_STATUS
EFIAPI
AmdPspDxeV2DriverEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_EVENT                   Event;
  EFI_STATUS                  Status;
  EFI_HANDLE                  Handle;
  VOID                        *Registration;
  UINT8                       *Buffer;

  AGESA_TESTPOINT (TpPspDxeV2Entry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.AmdPspDxeV2 Enter\n");
  Handle = NULL;
  //Install PSP Common service Protocol
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdPspCommonServiceProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPspCommonServiceProtocol
                  );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
  }

  //Always PSP ADDR MSR for RdRand Instruction
  if (AmdCapsuleGetStatus () == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate MpProtocol event for RdRand\n");
    Event = NULL;
    Event = EfiCreateProtocolNotifyEvent (
                        &gEfiMpServiceProtocolGuid,
                        TPL_NOTIFY,
                        PspMpServiceCallBack,
                        NULL,
                        &Registration
                        );
    ASSERT (Event != NULL);
    if (Event == NULL) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate event fail\n");
    }

    IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate Protocol event for Install FlashAcc Protocol\n");
    Event = NULL;
    Event = EfiCreateProtocolNotifyEvent (
                        &gPspFlashAccSmmCommReadyProtocolGuid,
                        TPL_CALLBACK,
                        FlashAccSmmReadyCallBack,
                        NULL,
                        &Registration
                        );
    ASSERT (Event != NULL);
    if (Event == NULL) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate event fail\n");
    }
  }

  //Allocate RT buffer for Smm communication before EndOfDxe Event to satisfie WSMT test
  Buffer = NULL;
  Buffer = AllocateRuntimePool (AMD_SMM_COMMUNICATION_BUFFER_SIZE);
  ASSERT (Buffer != NULL);
  //Save to PCD database
  if (Buffer != NULL) {
    PcdSet64S (PcdAmdSmmCommunicationAddress, (UINT64) (UINTN) Buffer);
  }
  //If PSP feature turn off, exit the driver
  if ((CheckPspDevicePresentV2 () == FALSE) ||
      (PcdGetBool (PcdAmdPspEnable) == FALSE)) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate PciEnumerationCompleteProtocol callback\n");
  Event = NULL;
  Event = EfiCreateProtocolNotifyEvent (
                      &gEfiPciEnumerationCompleteProtocolGuid,
                      TPL_NOTIFY,
                      PspPciEnumerationCompleteCallBack,
                      NULL,
                      &Registration
                      );
  ASSERT (Event != NULL);
  if (Event == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate event fail\n");
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate ReadyToBoot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  PspDxeReadyToBoot,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Event
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate ExitBootServicesEvent\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PspDxeExitBS,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &Event
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return (Status);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate DxeSmmReadyToLockProtocol callback\n");
  Event = NULL;
  Event = EfiCreateProtocolNotifyEvent (
                      &gEfiDxeSmmReadyToLockProtocolGuid,
                      TPL_CALLBACK,
                      PspDxeSmmReadyToLockCallBack,
                      NULL,
                      &Registration
                      );
  ASSERT (Event != NULL);
  if (Event == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tCreate event fail\n");
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspDxeV2 Exit\n");
  AGESA_TESTPOINT (TpPspDxeV2Exit, NULL);
  return EFI_SUCCESS;
}


