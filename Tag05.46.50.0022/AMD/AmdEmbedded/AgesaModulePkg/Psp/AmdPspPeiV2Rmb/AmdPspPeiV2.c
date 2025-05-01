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
 * AMD CPU POST API, and related functions.
 *
 * Contains code that initialized the CPU after memory init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */
/*++
Module Name:

  PspPei.c
  Init PSP device
  Intercept MemoryDiscovery protocol and
    a) Send mailbox to PSP to inform the DRAM information

Abstract:
--*/

#include "PiPei.h"
#include <Library/HobLib.h>
#include <Ppi/AmdPspRecoveryDetectPpi.h>
#include <Ppi/AmdPspCommonServicePpi.h>
#include <Ppi/EndOfPeiPhase.h>

#include "AGESA.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Addendum/Apcb/Inc/APOBCMN.h>
#include <Library/AmdPspApobLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Library/FchBaseLib.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Library/AmdPspPsbFusingLib.h>
#include <Library/AmdPspHstiStateLib.h>
#include <Library/AmdStbLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Filecode.h>

#define FILECODE PSP_AMDPSPPEIV2_AMDPSPPEIV2_FILECODE
#define MP0_PUBLIC0_SMN_BASE      (0x3800000)
#define SPECIFIC_WARM_RESET_STR   L"Specific Warm Reset"
//32 charecters at most for warm reset description
#define WARM_RESET_DATA_SIZE      (sizeof(CHAR16) * 32 + sizeof(EFI_GUID))

#define IOHC0NBCFG_SMN_ADDR                 0x13B00000ul           ///< Define the SMN Base address for NB MISC register
#define FASTREG_BASE_ADDR_LO_OFFSET         0x10300ul              ///< Define the offset of CCP_BASE_ADDR_LO in NB MISC block
#define FASTREG_BASE_ADDR_HI_OFFSET         0x10304ul              ///< Define the offset of CCP_BASE_ADDR_HI in NB MISC block
#define CCP_BAR_SIZE                        0x100000ul             ///< Size of CCP BAR
#define CCP_BASE_ADDR_LO_OFFSET             0x102D8ul              ///< Define the offset of CCP_BASE_ADDR_LO in NB MISC block
#define CCP_BASE_ADDR_HI_OFFSET             0x102DCul              ///< Define the offset of CCP_BASE_ADDR_HI in NB MISC block

extern EFI_GUID gEfiAmdAgesaSpecificWarmResetGuid;

EFI_STATUS
ReadC2PMsgRegV2 (
  IN       UINTN      Socket,
  IN       UINTN      Die,
  IN       UINT32     C2pMsgOffset,
  IN OUT   UINT32     *RegValue
  );

STATIC AMD_PSP_COMMON_SERVICE_PPI mPspCommonServicePpi = {
  GetFtpmControlAreaV2,
  SwitchPspMmioDecodeV2,
  CheckPspDevicePresentV2,
  CheckFtpmCapsV2,
  PSPEntryInfoV2,
  PspLibTimeOutV2,
  ReadC2PMsgRegV2,
  AcquirePspSmiRegMutexV2,
  ReleasePspSmiRegMutexV2,
  CheckPspCapsV2
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPspCommonServicePpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPspCommonServicePpiGuid,
  &mPspCommonServicePpi
};

STATIC AMD_PSP_RECOVERY_DETECT_PPI mPspRecoveryDetectPpi = {
  0x01
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPspRecoveryDetectedPpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdPspRecoveryDetectPpiGuid,
  &mPspRecoveryDetectPpi
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
ReadC2PMsgRegV2 (
  IN       UINTN      Socket,
  IN       UINTN      Die,
  IN       UINT32     C2pMsgOffset,
  IN OUT   UINT32     *RegValue
  )
{
  EFI_STATUS                        Status;
  CONST EFI_PEI_SERVICES            **PeiServices;
  UINT8                             SmuRegInstanceId;
  PEI_AMD_NBIO_SMU_SERVICES_PPI     *SmuServicesPpi;

  SmuRegInstanceId = (UINT8) Die;   //Only one socket (0), and use InstanceId as the die number for ZP
  PeiServices = GetPeiServicesTablePointer();

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &SmuServicesPpi);
  if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("  Locate Nbio Smu Services Ppi fail\n");
    return Status;
  }

  SmuServicesPpi->SmuRegisterRead (SmuServicesPpi, SmuRegInstanceId, MP0_PUBLIC0_SMN_BASE + C2pMsgOffset, RegValue);

  return EFI_SUCCESS;
}


extern  EFI_GUID  gAmdFabricTopologyServices2PpiGuid;
EFI_STATUS
EFIAPI
EndOfPeiSignalPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  );


EFI_PEI_NOTIFY_DESCRIPTOR   mEndOfPeiSignalPpiCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  EndOfPeiSignalPpiCallback
};

VOID
PspAntiRollbackSplFusing (
  )
{
  PSB_HSTI_STATUS_1           PsbHstiStatus1;
  EFI_STATUS                  Status;
  UINT32                      SplValue;
  UINT32                      HSTIState;

  // Check if Anti-rollback token is enabled
  if (PcdGetBool (PcdAmdPspAntiRollbackLateSplFuse) == FALSE) {
    return;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp AntiRollback Feature\n");

  GetPsbHstiStatus1 (&PsbHstiStatus1);
  IDS_HDT_CONSOLE_PSP_TRACE ("SPL FUSE Request: %X\n", PsbHstiStatus1.SplFuseReq);
  // Check PSB status
  if (PsbHstiStatus1.SplTableIsMissing == 1) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: Table containing required SPL values of critical firmware entries is not present\n");
    return;
  }
  if (PsbHstiStatus1.SplEntryError == 1) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: Entry of critical firmware not found in table, or SPL value of present firmware does not meet requirements of the table entry\n");
    return;
  }
  if (PsbHstiStatus1.SplFuseError == 1) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: SPL value of table itself does not meet requirements as set in fuse\n");
    return;
  }
  if (PsbHstiStatus1.SplFuseReq == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Exit: SPL FUSE Update is not required\n");
    return;
  }
  SplValue = PcdGet32 (PcdAmdPspAntiRollbackInitialSpl);
  IDS_HDT_CONSOLE_PSP_TRACE ("Initial SPL Value: 0x%x\n", SplValue);
  // if no initial SPL value specified, the system will always follow the SPL value in the SPL table
  if (SplValue == 0) {
    IDS_HDT_CONSOLE_PSP_TRACE ("SPL fuse will be upgraded to the SPL value in the SPL table\n");
    Status = PspMboxLateSplFusing (SplValue);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SPL fusing command fail:%r\n", Status);
    }
  } else {
    HSTIState = 0;
    PspMboxBiosQueryHSTIState (&HSTIState);
    // if initial SPL value is specified, and firmware anti-rollback enforcement is NOT enabled,
    // set SPL fuse to initial SPL value
    if ((HSTIState & PSP_ANTI_ROLLBACK_STATUS) == 0) {
      IDS_HDT_CONSOLE_PSP_TRACE ("SPL fuse will be set to 0x%x, SPL fuse mask will be:0x%lx\n",
                                 SplValue, ((1 << (UINT64)SplValue) - 1));
      Status = PspMboxLateSplFusing (SplValue);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("SPL fusing command fail:%r\n", Status);
      }
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp AntiRollback Feature Exit\n");
}

/**
 * @brief Set the Psb Fusing to enable/disable as needed
 *
 * @return VOID no return value
 */
VOID
SetPsbFusing (
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT32                      Result = 0;
  BOOLEAN                     PsbEnableRequired = FALSE;
  BOOLEAN                     PsbDisableRequired = FALSE;
  PSB_HSTI_STATUS_1           PsbHstiStatus1;
  PSB_STATUS_2                PsbStatus2;

  // Check PCD master token is enabled
  PsbEnableRequired = PcdGetBool (PcdAmdPspAutoPsb);
  PsbDisableRequired = PcdGetBool (PcdAmdPspPsbDisable);

  if (PsbEnableRequired && PsbDisableRequired) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Error:Cannot enable AutoPsb and PsbDisablement at the same time!\n");
    ASSERT (FALSE);  // Assertion in the debug build
    return;
  }
  if (PsbEnableRequired) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp AutoPsb Feature\n");
    if (IsPsbFusingRequired ()) {
      DoPsbFusing ();
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("Psp AutoPsb Feature Exit\n");
  }

  if (PsbDisableRequired) {
    GetPsbStatus2 (&PsbStatus2);
    IDS_HDT_CONSOLE_PSP_TRACE ("PSB_STATUS_2:\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPlatformVendorId: %X\n", PsbStatus2.PlatformVendorId);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPlatformModelId: %X\n", PsbStatus2.PlatformModelId);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tBiosKeyRevisionId: %X\n", PsbStatus2.BiosKeyRevisionId);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tRootKeySelect: %X\n", PsbStatus2.RootKeySelect);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPlatformSecureBootEn: %X\n", PsbStatus2.PlatformSecureBootEn);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDisableBiosKeyAntiRollback: %X\n", PsbStatus2.DisableBiosKeyAntiRollback);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDisableAmdKeyUsage: %X\n", PsbStatus2.DisableAmdKeyUsage);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tDisableSecureDebugUnlock: %X\n", PsbStatus2.DisableSecureDebugUnlock);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tCustomerKeyLock: %X\n", PsbStatus2.CustomerKeyLock);

    // Check if PSB fuse already burned
    if (PsbStatus2.PlatformSecureBootEn == 1) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Exit: PSB fuse already burned\n");
      return;
    }

    // Check if PSB has been disabled
    if ((PsbStatus2.CustomerKeyLock == 1)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Exit: PSB has been permanently disabled\n");
      return;
    }

    GetPsbHstiStatus1 (&PsbHstiStatus1);
    IDS_HDT_CONSOLE_PSP_TRACE ("PSB_HSTI_STATUS:\n");
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPsbTestStatus: %X\n", PsbHstiStatus1.PsbTestStatus);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tPsbFusingReadiness: %X\n", PsbHstiStatus1.PsbFusingReadiness);
    IDS_HDT_CONSOLE_PSP_TRACE ("\tHstiState: %X\n", PsbHstiStatus1.HstiState);

    IDS_HDT_CONSOLE_PSP_TRACE ("Perform PSB Disabling\n");
    Status = PspMboxBiosDisablePsb (&Result);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Disable PSB failed:%r\n", Status);
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("Disable PSB successfully\n");
    }
  }
}

/**
  Check PSP Reset Flag
  Target will set Reset flag if some requirements.

  @retval BOOLEAN  0: Reset Flag is cleared, 1: Reset Flag has been set

**/
BOOLEAN
CheckPspResetFlag (
  VOID
  )
{
  PSP_MBOX_V2 *PspMbox;
  PspMbox = NULL;

  // Waiting tOS set ready bit
  // Get PspMbox location. fail if not found
  if (GetPspMboxLocation (&PspMbox) == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("GetPspMboxLocation Error\n");
    ASSERT (FALSE); // Assertion in the debug build
    return (FALSE);
  }
  // Wait till tOS set ready bit, ready bit indicate that tOS already reach the idle state
  IDS_HDT_CONSOLE_PSP_TRACE ("Wait till ready bit set by tOS\n");
  while ((!PspMbox->Cmd.Field.Ready)) {
    ;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp mailbox is ready, and ResetRequired flag = 0x%x\n", PspMbox->Cmd.Field.ResetRequired);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp mailbox is ready, and Recovery flag = 0x%x\n", PspMbox->Cmd.Field.Recovery);
  if ((PspMbox->Cmd.Field.ResetRequired == 1) && (PspMbox->Cmd.Field.Recovery == 0)) {
    return (TRUE);
  } else {
    return (FALSE);
  }
}

VOID
IssueWarmReset (
  IN  EFI_PEI_SERVICES                **PeiServices
)
{
  EFI_STATUS            Status;
  UINT8                 WarmResetData [WARM_RESET_DATA_SIZE];
  UINTN                 SizeInBytes;
  Status = EFI_SUCCESS;
  //do warm reset
  Status = StrCpyS (
              (CHAR16 *)WarmResetData,
              WARM_RESET_DATA_SIZE,
              SPECIFIC_WARM_RESET_STR
              );
  ASSERT (Status == EFI_SUCCESS);

  SizeInBytes = StrnSizeS ((CHAR16 *) WarmResetData, (WARM_RESET_DATA_SIZE / sizeof (CHAR16)));
  ASSERT ((SizeInBytes + sizeof(EFI_GUID)) <= WARM_RESET_DATA_SIZE);

  CopyGuid (
        (EFI_GUID *) (WarmResetData + SizeInBytes),
        &gEfiAmdAgesaSpecificWarmResetGuid
        );
  SizeInBytes += sizeof(EFI_GUID);
  (**PeiServices).ResetSystem2 (EfiResetPlatformSpecific, EFI_SUCCESS, SizeInBytes, WarmResetData);

  //Should not run here
  ASSERT (FALSE);
}

EFI_STATUS
EFIAPI
EndOfPeiSignalPpiCallback (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
  IN  VOID                            *InvokePpi
  )
{
  PSP_DIRECTORY         *PspDir;
  BOOLEAN               AbRecoveryEnabled;
  UINT64                IgnoredPspRegionAEntry;
  UINT64                IgnoredPspRegionBEntry;

  PspDir = NULL;
  IgnoredPspRegionAEntry = 0;
  IgnoredPspRegionBEntry = 0;
  AbRecoveryEnabled = FALSE;

  //Send Command to PSP to lock DF register
  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspPeiV2.EndOfPeiSignalPpiCallback\n");
  if (FchReadSleepType () == 0x03) {
    PspMboxBiosLockDFReg ();
  } else {
    //Anti-rollback support
    PspAntiRollbackSplFusing ();

    //PSP OS is loading in parallel of BIOS PEI execution, set PSB fusing at the end of PEI
    SetPsbFusing ();
    //Check if it's A/B recovery mode
    if (GetPspDirBaseV2 (&PspDir) != TRUE) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PSP Dir Base not found\n");
      ASSERT (FALSE); // Assertion in the debug build
      return FALSE;
    }

    AbRecoveryEnabled = IsABrecovery (PspDir, &IgnoredPspRegionAEntry, &IgnoredPspRegionBEntry);
    if (AbRecoveryEnabled) {
      if (CheckPspResetFlag ()) {
        IDS_HDT_CONSOLE_PSP_TRACE ("  ResetRequired is set, issuing warm reset\n");
        //do warm reset
        IssueWarmReset (PeiServices);
      }
    }
  }
  return (EFI_SUCCESS);
}

/**
 * @brief Enable MMIO for CCP
 *
 * @return EFI_STATUS
 */
EFI_STATUS
CcpBarInit (
  UINT64                 *CcpMmioBase
  )
{
  UINT32                 Value32;
  UINT64                 MmioBase;
  UINT64                 Length;
  FABRIC_TARGET          MmioTarget;
  EFI_STATUS             Status;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  UINT8                  RbNumber;


  RbNumber = 0;

  //Allocate MMIO Region from MMIO manager
  Length = CCP_BAR_SIZE;
  MmioTarget.TgtType = TARGET_RB;
  MmioTarget.SocketNum = 0;
  MmioTarget.RbNum = RbNumber;
  Attributes.ReadEnable = 1;
  Attributes.WriteEnable = 1;
  Attributes.NonPosted = 0;
  Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
  MmioBase  = 0;
  Status = FabricAllocateMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
  ASSERT (Status == EFI_SUCCESS );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("CcpBarInit Allocate MMIO Fail\n");
    return Status;
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("CcpBarInit Allocate MMIO @0x%lx\n", MmioBase);
  }

  if (SocFamilyIdentificationCheckV2 (F19_RMB_B0_FAMILY_ID_BASE_MODEL)) {
    //In RMB B0 and onward, we use CCP BAR
    Value32 = (UINT32) (MmioBase | (BIT0 + BIT8));
    SmnRegisterWrite (0, IOHC0NBCFG_SMN_ADDR + CCP_BASE_ADDR_LO_OFFSET, &Value32, 0);
    Value32 = (UINT32) RShiftU64 (MmioBase, 32);
    SmnRegisterWrite (0, IOHC0NBCFG_SMN_ADDR + CCP_BASE_ADDR_HI_OFFSET, &Value32, 0);
  } else {
    //In RMB A0 we use FASTREGBAR as CCP BAR
    Value32 = (UINT32) (MmioBase | (BIT0 + BIT1));
    SmnRegisterWrite (0, IOHC0NBCFG_SMN_ADDR + FASTREG_BASE_ADDR_LO_OFFSET, &Value32, 0);
    Value32 = (UINT32) RShiftU64 (MmioBase, 32);
    SmnRegisterWrite (0, IOHC0NBCFG_SMN_ADDR + FASTREG_BASE_ADDR_HI_OFFSET, &Value32, 0);
    //PSP will set IOHC::FASTREG_APERTURE.FASTREG_APERTURE_ID as CCP SMN address
  }
  *CcpMmioBase = MmioBase;
  return EFI_SUCCESS;
}
#define HW_DEFAULT_VALUE_PSP_BAR_MSR  0
/**
 * @brief Enable RdRand and RdSeed Instruction in early phase
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EnableRdInstructionEarly (
  VOID
  )
{
  UINT64                 CcpMmioBase;
  volatile UINT64                 PspMsrValue;
  EFI_STATUS             Status;

  Status = CcpBarInit (&CcpMmioBase);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  PspMsrValue = 0;
  //PSP BAR MSR can not be set twice, otherwise, it will cause CPU exception
  PspMsrValue = AsmReadMsr64 (0xC00110A2);
  if (PspMsrValue == HW_DEFAULT_VALUE_PSP_BAR_MSR) {
    //Setup PSP BAR MSR for master die
    IDS_HDT_CONSOLE_PSP_TRACE ("Set PspAddrMsr for BSP 0x%lx\n", CcpMmioBase);
    AsmWriteMsr64 (0xC00110A2, CcpMmioBase);
  }
  DumpRdInstructionInfo ();
  return EFI_SUCCESS;
}
/*++

Routine Description:

  PSP Driver Entry. Initialize PSP device and report DRAM info to PSP once found

Arguments:

Returns:

  EFI_STATUS

--*/
extern EFI_GUID gAmdCapsuleStatusHobGuid;
EFI_STATUS
EFIAPI
AmdPspPeiV2DriverEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT8                 Verbosity;

  //If PSP feature turn off, exit the driver
  if ((CheckPspDevicePresentV2 () == FALSE) ||
      (PcdGetBool (PcdAmdPspEnable) == FALSE)) {
    return EFI_SUCCESS;
  }

  //PEI_AMD_ERROR_LOG_SERVICE_PPI *ErrorLogService;
  //ERROR_LOG_DATA_STRUCT   ErrorLogData;
  AGESA_TESTPOINT (TpPspPeiV2Entry, NULL);
  IDS_HDT_CONSOLE_PSP_TRACE ("Psp.Drv.AmdPspPeiV2 Enter\n");

  //Call InitializeHstiState so that the HSTI state will be saved to heap for later use
  InitializeHstiState ();

  if (AmdCapsuleGetStatus ()) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Build CapsuleStatusHob\n");
   BuildGuidHob (&gAmdCapsuleStatusHobGuid, 1);
  }
  // Public PSP Common Service PPI V1 instance
  IDS_HDT_CONSOLE_PSP_TRACE ("\tInstall PSP common service PPI\n");
  Status = (**PeiServices).InstallPpi (PeiServices, &mPspCommonServicePpiList);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("InstallPpi fail\n");
    ASSERT (FALSE);
  }

  //Check PSP Recovery required Flag, if set publish PPI
  if ((FchReadSleepType () != 3) && (AmdCapsuleGetStatus () == FALSE)) {
    if (CheckPspRecoveryFlagV2 ()) {
      IDS_HDT_CONSOLE_PSP_TRACE ("Recovery Flag Detected\n");
      Status = (**PeiServices).InstallPpi (PeiServices, &mPspRecoveryDetectedPpiList);
      PcdSetBoolS (PcdAmdPspRecoveryDetected, TRUE);
      ASSERT (Status == EFI_SUCCESS);
    }
  }
  //
  // Always Initialize PSP BAR to support RdRand Instruction
  //
  IDS_HDT_CONSOLE_PSP_TRACE ("\tPsp BAR init\n");
  PspBarInitEarlyV2 ();

  //Enable RdRand at PEI phase
  Status = EnableRdInstructionEarly ();
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\tEnableRd Failed\n");
    AGESA_TESTPOINT (TpEnableRdFail, NULL);
  }

  //If STB has been enabled pass STB verbosity to PSP
  if (PcdGetBool (PcdStbEnable)) {
    if (GetPspVerbosity (&Verbosity) == EFI_SUCCESS) {
      PspMboxBiosStbVerbosity (Verbosity);
    }
  }

  //
  // Register end of PEI callback for S3
  //

  Status = (**PeiServices).NotifyPpi (PeiServices, &mEndOfPeiSignalPpiCallback);
  ASSERT (Status == EFI_SUCCESS);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Notify EndOfPei Ppi fail\n");
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("AmdPspPeiV2 Exit\n");
  AGESA_TESTPOINT (TpPspPeiV2Exit, NULL);

  return (Status);
}

