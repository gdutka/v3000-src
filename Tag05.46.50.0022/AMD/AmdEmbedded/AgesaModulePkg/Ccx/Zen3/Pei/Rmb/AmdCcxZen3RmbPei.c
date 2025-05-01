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
 * AMD CCX Zen3 API, and related functions.
 *
 * Contains code that initializes the core complex
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CCX
 * @e \$Revision$   @e \$Date$
 *
 */
/*++
Module Name:

  AmdCcxZen3Pei.c
  Init CCX interface

Abstract:
--*/

#include <PiPei.h>
#include <AGESA.h>
#include "CcxZen3CacheInit.h"
#include "CcxZen3DownCoreInit.h"
#include "CcxZen3BrandString.h"
#include "CcxRegistersZen3.h"
#include <Library/CcxRolesLib.h>
#include <Library/CcxDownCoreLib.h>
#include <Library/CcxResetTablesLibV2.h>
#include <Library/CcxApicLib.h>
#include <Library/CcxPeiSmmAccessLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/IdsLib.h>
#include <Library/FchBaseLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/CcxSetMcaLib.h>
#include <Library/CcxHaltLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/CcxMicrocodePatchLib.h>
#include <Library/FabricWdtLib.h>
#include <Ppi/AmdCcxPpi.h>
#include <Ppi/AmdCoreTopologyServicesV2Ppi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/SmmAccess.h>
#include <PspResumeMemInfo.h>
#include "AmdCcxZen3RmbPei.h"
#include <Filecode.h>

#define FILECODE CCX_ZEN3_PEI_RMB_AMDCCXZEN3RMBPEI_FILECODE

STATIC PEI_AMD_CCX_INIT_COMPLETE_PPI mCcxPeiInitCompletePpi = {
  AMD_CCX_PPI_REVISION
};

STATIC EFI_PEI_PPI_DESCRIPTOR mCcxPeiInitCompletePpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCcxPeiInitCompletePpiGuid,
  &mCcxPeiInitCompletePpi
};

EFI_STATUS
CcxInitWithMpServices (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
CcxEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiPeiMpServicesPpiGuid,
    CcxInitWithMpServices,
  }, {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    CcxEndOfPei
  }
};

extern EFI_GUID gApSyncFlagNvVariableGuid;

/*++

Routine Description:

  Zen3 Rmb Driver Entry.  Initialize the core complex.

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdCcxZen3RmbPeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  UINT32                                  i;
  UINTN                                   Ccd;
  UINTN                                   Ccx;
  UINTN                                   Core;
  UINTN                                   Thread;
  UINTN                                   NumberOfCcds;
  UINTN                                   NumberOfComplexes;
  UINTN                                   NumberOfCores;
  UINTN                                   NumberOfThreads;
  UINTN                                   VariableSize;
  VOLATILE UINT32                         *ApSyncFlag;
  UINT32                                  ApNumBfLaunch;
  EFI_STATUS                              Status;
  EFI_STATUS                              CalledStatus;
  AMD_CONFIG_PARAMS                       StdHeader;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PPI       *CoreTopologyServices;
  PEI_AMD_NBIO_SMU_SERVICES_PPI           *NbioSmuServices;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI         *Variable;
  PSP_SMM_HDR_DATA                        *PspSmmHdrData;
  EFI_PHYSICAL_ADDRESS                    ApStartupVector;
  VOID                                    *MemoryContentCopy;
  VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA  ApLaunchGlobalData;
  VOLATILE AP_MTRR_SETTINGS     ApMtrrSyncList[] =
  {
    { 0xC0010058,               0x0000000000000000  },
    { AMD_AP_MTRR_FIX64k_00000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX16k_80000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX16k_A0000, 0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_C0000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_C8000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_D0000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_D8000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_E0000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_E8000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_F0000,  0x0000000000000000  },
    { AMD_AP_MTRR_FIX4k_F8000,  0x0000000000000000  },
    { 0x00000200,               0x0000000000000000  },
    { 0x00000201,               0x0000000000000000  },
    { 0x00000202,               0x0000000000000000  },
    { 0x00000203,               0x0000000000000000  },
    { 0x00000204,               0x0000000000000000  },
    { 0x00000205,               0x0000000000000000  },
    { 0x00000206,               0x0000000000000000  },
    { 0x00000207,               0x0000000000000000  },
    { 0x00000208,               0x0000000000000000  },
    { 0x00000209,               0x0000000000000000  },
    { 0x0000020A,               0x0000000000000000  },
    { 0x0000020B,               0x0000000000000000  },
    { 0x0000020C,               0x0000000000000000  },
    { 0x0000020D,               0x0000000000000000  },
    { 0x0000020E,               0x0000000000000000  },
    { 0x0000020F,               0x0000000000000000  },
    { 0x000002FF,               0x0000000000000000  },
    { CPU_LIST_TERMINAL                             }
  };

  VOLATILE AP_MSR_SYNC          ApMsrSyncList[] =
  {
    { 0xC0010030, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3Brandstring
    { 0xC0010031, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3Brandstring
    { 0xC0010032, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3Brandstring
    { 0xC0010033, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3Brandstring
    { 0xC0010034, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3Brandstring
    { 0xC0010035, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3BrandString
    { 0xC0011022, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbPrefetcher/CcxZen3RmbInitializePrefetchMode
    { 0xC001102B, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbPrefetcher/CcxZen3RmbInitializePrefetchMode
    { 0xC0010010, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbEnableSmee
    { 0xC0010296, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbInitializeC6
    { 0xC0010073, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbInitializeC6
    { 0xC0010015, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbInitializeCpb
    { 0xC0011029, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF  },  // CcxZen3RmbInitializePrefetchMode/PcdAmdRedirectForReturnDis
    { 0xC0011021, 0x0000000000000000, BIT5   },               // PcdAmdOpcacheCtrl
    { 0xC0011020, 0x0000000000000000, BIT28  },               // PcdAmdStreamingStoresCtrl/PcdAmdIbsHardwareEn
    { 0xC00110DF, 0x0000000000000000, BIT36  },               // PcdAmdEnableFSRM
    { 0xC0011002, 0x0000000000000000, BIT9   },               // PcdAmdEnableERMS
    { CPU_LIST_TERMINAL, 0, 0}
  };

  AGESA_TESTPOINT (TpCcxPeiEntry, NULL);
  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3RmbPeiInit Entry\n");

  Status = EFI_SUCCESS;
  SetMem ((VOID *) &ApLaunchGlobalData, sizeof (AMD_CCX_AP_LAUNCH_GLOBAL_DATA), 0);

  if (CcxIsBsp (&StdHeader)) {
    CcxZen3DownCoreInit (PeiServices);

    IDS_HOOK (IDS_HOOK_CCX_PEI, NULL, NULL);

    if ((FchReadSleepType () == 0x3) || AmdCapsuleGetStatus ()) {
      ApLaunchGlobalData.SleepType = 3;
    }

    if (ApLaunchGlobalData.SleepType != 3) {
      CcxZen3CacheInit (&StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3CacheInit Done\n");

      CcxZen3SetBrandString (&StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3SetBrandString Done\n");
    }

    CalledStatus = (*PeiServices)->LocatePpi (
                                   PeiServices,
                                   &gAmdCoreTopologyServicesV2PpiGuid,
                                   0,
                                   NULL,
                                   &CoreTopologyServices
                                   );
    ASSERT (CalledStatus == EFI_SUCCESS);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    ApNumBfLaunch = 0;
    ApSyncFlag = NULL;
    PspSmmHdrData = (PSP_SMM_HDR_DATA *)AsmReadMsr64(0xC0010112); // SMMADDR_ADDRESS - Start of TSEG

    if (ApLaunchGlobalData.SleepType == 3) {
      CalledStatus = (**PeiServices).LocatePpi (PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, &Variable);
      ASSERT (CalledStatus == EFI_SUCCESS);

      if (PcdGetBool (PcdAmdPspS3WakeFromSmm)) {
        VariableSize = sizeof (UINT32);
        CalledStatus = Variable->GetVariable (
                        Variable,
                        L"ApSyncFlagNv",
                        &gApSyncFlagNvVariableGuid,
                        NULL,
                        &VariableSize,
                        (VOID *) &ApSyncFlag
                        );
        ASSERT (CalledStatus == EFI_SUCCESS);
        ASSERT (ApSyncFlag != NULL);
        IDS_HDT_CONSOLE (CPU_TRACE, " ApSyncFlagAddr %x\n", ApSyncFlag);
      } else {
        ApSyncFlag = (VOLATILE UINT32*)&PspSmmHdrData->ApSyncFlag;
        ASSERT (ApSyncFlag != NULL);
        *ApSyncFlag = 0;
        PspSmmHdrData->ApStackTop = PspSmmHdrData->PspSmmRsmMemInfo.StackPtr + PspSmmHdrData->PspSmmRsmMemInfo.BspStackSize + PspSmmHdrData->PspSmmRsmMemInfo.ApStackSize;
        PspSmmHdrData->CcxEarlyInit = (EFI_PHYSICAL_ADDRESS)CcxSetMca;  // Call for APs when they are launched.
        CcxSetMca (); // Call for BSP.
      }
    } else {
      ApLaunchGlobalData.ApMtrrSyncList = ApMtrrSyncList;
      ApLaunchGlobalData.SizeOfApMtrr = sizeof (ApMtrrSyncList);
      ApLaunchGlobalData.ApMsrSyncList = ApMsrSyncList;

      // Load microcode on the BSP
      IDS_SKIP_HOOK (IDS_HOOK_CCX_SKIP_UCODE_PATCH, NULL, NULL) {
        CcxZen3RmbMicrocodeInit ((UINT64 *) &ApLaunchGlobalData.UcodePatchAddr, &StdHeader);
      }

      // Reset Table
      GetPlatformFeatures (&StdHeader, (PLATFORM_FEATS *) &ApLaunchGlobalData.ResetTableCriteria.PlatformFeats);
      GetPerformanceFeatures (&StdHeader, (PROFILE_FEATS *) &ApLaunchGlobalData.ResetTableCriteria.ProfileFeats);
      GetSocLogicalIdOnCurrentCore (&StdHeader, (SOC_LOGICAL_ID *) &ApLaunchGlobalData.ResetTableCriteria.SocLogicalId);
      GetCoreLogicalIdOnCurrentCore (&StdHeader, (CORE_LOGICAL_ID *) &ApLaunchGlobalData.ResetTableCriteria.CoreLogicalId);
      CcxProgramTablesAtReset (ApLaunchGlobalData.SleepType, (ENTRY_CRITERIA *) &ApLaunchGlobalData.ResetTableCriteria, &StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcxProgramTablesAtReset Done\n");

      // L1, L2 HW Stream Prefetcher
      CcxZen3RmbPrefetcher ();

      // MSR misc setting
      CcxZen3RmbSetMiscMsrs ();

      // Enable SMEE
      CcxZen3RmbEnableSmee ();

      // Get PCD setting for CPU WDT
      ApLaunchGlobalData.CpuWdt.CpuWdtEn = PcdGetBool (PcdAmdCpuWdtEn);
      ApLaunchGlobalData.CpuWdt.CpuWdtTimeOut = PcdGet16 (PcdAmdCpuWdtTimeout);
      if (ApLaunchGlobalData.CpuWdt.CpuWdtTimeOut == 0xFFFF) {
        // If it's 'Auto', set timeout to 2.682s to make sure it's smaller than DF WDT
        ApLaunchGlobalData.CpuWdt.CpuWdtTimeOut = 0x100;
      }
      ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity = PcdGet8 (PcdAmdCpuWdtSeverity);
      if ((ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity != 0xFF) && (ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity > 5)) {
        // 0 ~ 5 is valid
        ApLaunchGlobalData.CpuWdt.CpuWdTmrCfgSeverity = 0xFF;
      }
      // Enable watchdog timer
      CcxZen3RmbEnableWdt ((CORE_WATCHDOG *) &ApLaunchGlobalData.CpuWdt);

      // Mca initialization
      CcxSetMca ();

      CalledStatus = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &NbioSmuServices);
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      // CacWeights initialization is split into two routines as APs will not be getting
      // CacWeigts, but will be setting up CacWeights in AP flow
      CcxZen3RmbGetCacWeights (NbioSmuServices, (UINT64 *) &ApLaunchGlobalData.CacWeights[0]);
      CcxZen3RmbSetCacWeights ((UINT64 *) &ApLaunchGlobalData.CacWeights[0]);

      CcxZen3RmbInitializeC6 (&StdHeader);

      CcxZen3RmbInitializeCpb (&StdHeader);

      CcxZen3RmbInitializePrefetchMode (&StdHeader);

      for (i = 0; ApLaunchGlobalData.ApMsrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
        ApLaunchGlobalData.ApMsrSyncList[i].MsrData = AsmReadMsr64 (ApLaunchGlobalData.ApMsrSyncList[i].MsrAddr);
      }

      SetupApStartupRegion (&ApLaunchGlobalData, &ApStartupVector, &MemoryContentCopy);
      ApSyncFlag = (VOLATILE UINT32*) ((UINTN) ApLaunchGlobalData.AllowToLaunchNextThreadLocation);

    }

    AGESA_TESTPOINT (TpCcxPeiStartLaunchApsForS3, NULL);
    IDS_HDT_CONSOLE (CPU_TRACE, "    Launching APs in PEI\n");

    CoreTopologyServices->GetCoreTopologyOnDie (0, 0, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
    for (Ccd = 0; Ccd < NumberOfCcds; Ccd++) {
      for (Ccx = 0; Ccx < NumberOfComplexes; Ccx++) {
        for (Core = 0; Core < NumberOfCores; Core++) {
          for (Thread = 0; Thread < NumberOfThreads; Thread++) {
            if (!((Ccd == 0) && (Ccx == 0) && (Core == 0) && (Thread == 0))) {
              IDS_HDT_CONSOLE (CPU_TRACE, "      Launch ccd %X complex %X core %X thread %X\n", Ccd, Ccx, Core, Thread);
              ApNumBfLaunch++;
              CoreTopologyServices->LaunchThread (0, 0, Ccd, Ccx, Core, Thread);

              // Wait until the core launch
              if (ApSyncFlag != NULL) {
                while (*ApSyncFlag != ApNumBfLaunch) {
                  ;
                }
              }
              if (!PcdGetBool (PcdAmdPspS3WakeFromSmm)) {
                PspSmmHdrData->ApStackTop += PspSmmHdrData->PspSmmRsmMemInfo.ApStackSize;
              }
            }
          }
        }
      }
    }

    // MSRC001_1023[49, TwCfgCombineCr0Cd] = 1
    AsmMsrOr64 (0xC0011023, BIT49);

    // Restore the data located at the reset vector
    if (ApLaunchGlobalData.SleepType != 3) {
      RestoreResetVector (&ApLaunchGlobalData, (UINT16) ApNumBfLaunch, &ApStartupVector, &MemoryContentCopy);
    }

    AGESA_TESTPOINT (TpCcxPeiEndLaunchApsForS3, NULL);

    CalledStatus = (**PeiServices).InstallPpi (PeiServices, &mCcxPeiInitCompletePpiList);
    Status = (CalledStatus > Status) ? CalledStatus : Status;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  (**PeiServices).NotifyPpi (PeiServices, mNotifyList);

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3RmbPeiInit End\n");

  AGESA_TESTPOINT (TpCcxPeiExit, NULL);

  return (Status);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  SetupApStartupRegion
 *
 *
 *  Description:
 *    This routine sets up the necessary code and data to launch APs.
 *
 */
VOID
SetupApStartupRegion (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  IN       EFI_PHYSICAL_ADDRESS                   *ApStartupVector,
  IN       VOID                                   **MemoryContentCopy
  )
{
  UINT8             i;
  IA32_DESCRIPTOR   BspGdtr;
  TYPE_ATTRIB       TypeAttrib;
  UINT64            EntryAddress;
  UINT32            EntrySize;
  UINT64            EntryDest;
  UINT32            Segment;
//  UINT8             ApcbPurpose;
//  UINT32            Apcb32;
  UINT32            FarJmpOffset;
  UINT32            ApEntryInCOffset;
  VOID              *ApGdt;

  UINT8 ApStartupCode[] =
  {
  //[000] [001] [002]
    0xBE, 0xF4, 0xFF,                     // mov si, 0FFF4h
  //[003]
    0x66,
  //[004] [005] [006] [007]
    0x2E, 0x0F, 0x01, 0x14,               // lgdt       fword ptr cs:[si]
  //[008] [009] [010]
    0x0F, 0x20, 0xC0,                     // mov        eax, cr0                    ; Get control register 0
  //[011] [012] [013] [014]
    0x66, 0x83, 0xC8, 0x03,               // or         eax, 000000003h             ; Set PE bit (bit #0)
  //[015] [016] [017]
    0x0F, 0x22, 0xC0,                     // mov        cr0, eax
  //[018] [019] [020]
    0x0F, 0x20, 0xE0,                     // mov        eax, cr4
  //[021] [022] [023] [024] [025] [026]
    0x66, 0x0D, 0x00, 0x06, 0x00, 0x00,   // or         eax, 00000600h
  //[027] [028] [029]
    0x0F, 0x22, 0xE0,                     // mov        cr4, eax
    // Protected Mode Start
  //[030] [031] [032]
    0xB8, 0x18, 0x00,                     // mov        ax,  18h
  //[033] [034]
    0x8E, 0xD8,                           // mov        ds,  ax
  //[035] [036]
    0x8E, 0xC0,                           // mov        es,  ax
  //[037] [038]
    0x8E, 0xE0,                           // mov        fs,  ax
  //[039] [040]
    0x8E, 0xE8,                           // mov        gs,  ax
  //[041] [042]
    0x8E, 0xD0,                           // mov        ss,  ax
  //[043] [044] [045]
    0x66, 0x67, 0xEA,                     // Far jump  0010:[49:46]
  //[046] [047] [048] [049]
    0x00, 0x00, 0x00, 0x00,               // Byte [48], [49] will be replaced with segment from BIOS Directory
                                          // Byte [46], [47] will be replaced with offset of [52]
  //[050] [051]
    0x10, 0x00,

  //[052] [053] [054] [055] [056]
    0xB8, 0xFF, 0xFF, 0xFF, 0xFF,         // mov eax, offset ApEntryInCOffset
  //[057] [058] [059] [060] [061]
    0xBF, 0xFF, 0xFF, 0xFF, 0xFF,         // mov edi, offset ApLaunchGlobalData

  //[062] [063]
    0xFF, 0xE0                            // jmp eax
  };
  UINT64 GdtEntries[] =
  {
    0x0000000000000000,  // [00h] Null descriptor
    0x00CF92000000FFFF,  // [08h] Linear data segment descriptor
    0x00CF9A000000FFFF,  // [10h] Linear code segment descriptor
    0x00CF92000000FFFF,  // [18h] System data segment descriptor
    0x00CF9A000000FFFF,  // [20h] System code segment descriptor
    0x0000000000000000,  // [28h] Spare segment descriptor
    0x00CF93000000FFFF,  // [30h] System data segment descriptor
    0x00AF9B000000FFFF,  // [38h] System code segment descriptor
    0x0000000000000000   // [40h] Spare segment descriptor
  };
  UINT8 AsmNearJump[] =
  {
    //[00]
    0x90,         // nop
    //[01]
    0xE9,         // near jmp
    //[02], [03]
    0x00, 0x00    // 0x10000 + (0xFFF0 - AP_STARTUP_CODE_OFFSET) - 0xFFF4
  };

  if (BIOSEntryInfo (BIOS_FIRMWARE, INSTANCE_IGNORED, &TypeAttrib, &EntryAddress, &EntrySize, &EntryDest) == FALSE) {
   IDS_HDT_CONSOLE (CPU_TRACE, "Fail to get the correct Entry of BIOS firmware\n");
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "EntryAddress %X EntrySize %X EntryDest %X\n", EntryAddress, EntrySize, EntryDest);
  if (TypeAttrib.Copy == 0) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  First fetch is pointing to SPI\n");

    // Get AP base address from APCB
//    if (ApcbGet32 (APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE, Apcb32)) {
//      IDS_HDT_CONSOLE (CPU_TRACE, "APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE Found = 0x%x\n", Apcb32);
//      EntryDest = Apcb32 & 0xFFFF0000;
//    } else {
      EntryDest = 0x10000000;  // This is default value if there's no APCB APCB_TOKEN_UID_CPU_FETCH_FROM_SPI_AP_BASE
                               // This value MUST match value of FETCH_FROM_SPI_AP_BASE which is defined in ABL
//    }
    EntrySize = 0x100000;

    //IDS_HDT_CONSOLE (CPU_TRACE, "  AP CS Base = 0x%x\n", ((UINT32) EntryDest + EntrySize - 0x10000));
  }

  Segment = ((UINT32) EntryDest + EntrySize - 0x10000);
  IDS_HDT_CONSOLE (CPU_TRACE, "Segment %X\n", Segment);
  *ApStartupVector = (EFI_PHYSICAL_ADDRESS) (((UINT32) EntryDest + EntrySize - 0x10000) + 0xFFF0);

  IDS_HDT_CONSOLE (CPU_TRACE, "  ApStartupVector = 0x%x\n", ApStartupVector);

  // Fixup ApStartupCode
  ApStartupCode[48] = (UINT8) ((Segment >> 16) & 0xFF);
  ApStartupCode[49] = (UINT8) ((Segment >> 24) & 0xFF);

  FarJmpOffset = 0xFFF0 - AP_STARTUP_CODE_OFFSET + 52; // [46], [47] should be replaced with offset of [52]
  ApStartupCode[46] = (UINT8) (FarJmpOffset & 0xFF);
  ApStartupCode[47] = (UINT8) ((FarJmpOffset >> 8) & 0xFF);

  ApEntryInCOffset = (UINT32) ApAsmCode;
  *((UINT32*) &ApStartupCode[53]) = ApEntryInCOffset;
  *((UINT32*) &ApStartupCode[58]) = (UINT32) ApLaunchGlobalData;

  // Allocate some space for APs to use as stack space
  ApLaunchGlobalData->ApStackBasePtr = AllocatePool (RMB_MAX_NUMBER_OF_APS * AP_STACK_SIZE);
  ASSERT (ApLaunchGlobalData->ApStackBasePtr != NULL);

  // Allocate space to store data at reset vector
  *MemoryContentCopy = AllocatePool (AP_TEMP_BUFFER_SIZE);
  ASSERT (*MemoryContentCopy != NULL);

  SetMem (*MemoryContentCopy, AP_TEMP_BUFFER_SIZE, 0);

  // Copy data at reset vector to temporary buffer so we
  // can temporarily replace it with AP start up code.
  CopyMem (*MemoryContentCopy, (VOID *) (*ApStartupVector - AP_STARTUP_CODE_OFFSET), AP_TEMP_BUFFER_SIZE);
  SetMem ((VOID *) (*ApStartupVector - AP_STARTUP_CODE_OFFSET), AP_TEMP_BUFFER_SIZE, 0);

  // Copy AP start up code to Segment + 0xFFF0 - AP_STARTUP_CODE_OFFSET
  CopyMem ((VOID *) (*ApStartupVector - AP_STARTUP_CODE_OFFSET), (VOID *) &ApStartupCode, sizeof (ApStartupCode));

  ApLaunchGlobalData->ApSyncLocation = (UINT32) (*ApStartupVector + 0xE);
  ApLaunchGlobalData->AllowToLaunchNextThreadLocation = (UINT32) (*ApStartupVector + 0xA);

  // Fixup AsmNearJump
  // 0x10000 + (0xFFF0 - AP_STARTUP_CODE_OFFSET) - 0xFFF4
  *((UINT16 *) &AsmNearJump[2]) = 0x10000 + (0xFFF0 - AP_STARTUP_CODE_OFFSET) - 0xFFF4;

  // Copy the near jump to AP startup code to reset vector. The near jump
  // forces execution to start from CS:FFF0 - AP_STARTUP_CODE_OFFSET
  CopyMem ((VOID *) *ApStartupVector, (VOID *) AsmNearJump, sizeof (AsmNearJump));

  IDS_HDT_CONSOLE (CPU_TRACE, "  ApStartupVector Content = 0x%x\n", *ApStartupVector);
  // Copy GDT Entries to Segment + 0xFFF0 - BSP_GDT_OFFSET
  CopyMem ((VOID *) (*ApStartupVector - BSP_GDT_OFFSET), (VOID *) &GdtEntries, sizeof (GdtEntries));

  // Sync Fixed-MTRRs with BSP
  AsmMsrOr64 (0xC0010010, BIT19);

  for (i = 0; ApLaunchGlobalData->ApMtrrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    ApLaunchGlobalData->ApMtrrSyncList[i].MsrData = AsmReadMsr64 (ApLaunchGlobalData->ApMtrrSyncList[i].MsrAddr);
  }

  // Some Fixed-MTRRs should be set according to PCDs
  UpdateApMtrrSettings ((AP_MTRR_SETTINGS *) ApLaunchGlobalData->ApMtrrSyncList);

  AsmMsrAnd64 (0xC0010010, ~((UINT64) BIT19));

  // Copy MTRRs setting to Segment + 0xFFF0 - BSP_MSR_OFFSET
  ApLaunchGlobalData->BspMsrLocation = (UINT32) (*ApStartupVector - BSP_MSR_OFFSET);
  ASSERT (ApLaunchGlobalData->SizeOfApMtrr <= BSP_MSR_SIZE);
  CopyMem ((VOID *) (*ApStartupVector - BSP_MSR_OFFSET), (VOID *) ApLaunchGlobalData->ApMtrrSyncList, ApLaunchGlobalData->SizeOfApMtrr);

  BspGdtr.Limit = sizeof (GdtEntries) - 1;
  BspGdtr.Base = (UINTN) *ApStartupVector - BSP_GDT_OFFSET;

  // Copy pointer to GDT entries to Segment + 0xFFF4
  CopyMem ((VOID *) (*ApStartupVector + sizeof (AsmNearJump)), (VOID *) &BspGdtr, sizeof (BspGdtr));

  // Save BSP's patch level so that AP can use it to determine whether microcode patch
  // loading should be skipped
  ApLaunchGlobalData->BspPatchLevel = AsmReadMsr64 (0x0000008B);

  ApGdt = AllocateRuntimePages (EFI_SIZE_TO_PAGES (sizeof (GdtEntries)));
  ASSERT (ApGdt != NULL);
  CopyMem (ApGdt, (VOID *) &GdtEntries[0], sizeof (GdtEntries));

  ApLaunchGlobalData->ApGdtDescriptor.Size = (sizeof (GdtEntries)) - 1;
  ApLaunchGlobalData->ApGdtDescriptor.Pointer = (UINT64)(UINTN)ApGdt;

  AsmWbinvd ();
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  RestoreResetVector
 *
 *  @param[in] TotalCoresLaunched      The number of cores that were launched by the BSC
 *
 *  Description:
 *    This routine restores the code in the AP reset vector once all the APs that
 *    were launched are done running AP code
 *
 */
VOID
RestoreResetVector (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  IN       UINT16 TotalApCoresLaunched,
  IN       EFI_PHYSICAL_ADDRESS *ApStartupVector,
  IN       VOID                **MemoryContentCopy
  )
{
  UINT16  ApSyncCoreNumber;

  ApSyncCoreNumber = 0xFFFF;

  // Check whether the last core has completed running the AP Startup code
  do {
    CopyMem ((VOID *) &ApSyncCoreNumber, (VOID *) (EFI_PHYSICAL_ADDRESS) (ApLaunchGlobalData->ApSyncLocation), sizeof (ApSyncCoreNumber));
  } while (ApSyncCoreNumber != TotalApCoresLaunched);

  CopyMem ((VOID *) (*ApStartupVector - AP_STARTUP_CODE_OFFSET), *MemoryContentCopy, AP_TEMP_BUFFER_SIZE);

  // Clean up memory allocations
  // PEI phase doesn't support FreePool
  // FreePool (*MemoryContentCopy);
  // FreePool (ApLaunchGlobalData->ApStackBasePtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  RegSettingBeforeLaunchingNextThread
 *
 *  Description:
 *    Necessary register setting before launching next thread
 *
 */
VOID
RegSettingBeforeLaunchingNextThread (
  )
{
  AMD_CONFIG_PARAMS    StdHeader;
  CPUID_DATA           CpuId;
  BOOLEAN              IsSmtDisabled;

  AsmCpuidEx (
      0x8000001D,
      0,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );

  IsSmtDisabled = (BOOLEAN) (((CpuId.EAX_Reg >> 14) & 0xFFF) == 0);

  // Thread 0s are launched before thread 1s.  We want the last thread of a core to set the bit.
  // When SMT is disabled, thread 0 should set the bit.
  if (IsSmtDisabled || !CcxIsComputeUnitPrimary (&StdHeader)) {
    // MSRC001_1023[49, TwCfgCombineCr0Cd] = 1
    AsmMsrOr64 (0xC0011023, BIT49);
  }

  if (!CcxIsComputeUnitPrimary (&StdHeader)) {
    // MSR0000_0048[2] = 1
    AsmMsrOr64 (0x48, BIT2);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  ApEntryPointInC
 *
 *  Description:
 *    This routine is the C entry point for APs and is called from ApAsmCode
 *
 */
VOID
ApEntryPointInC (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  )
{
  AMD_CONFIG_PARAMS    StdHeader;

  // Enable watchdog timer
  if (CcxIsComputeUnitLast (&StdHeader)) {
     CcxZen3RmbEnableWdt ((CORE_WATCHDOG *) &ApLaunchGlobalData->CpuWdt);
  }

  // Skip loading microcode patch on AP if BSP's patch level is 0.
  if (ApLaunchGlobalData->BspPatchLevel != 0) {
    // Using the address saved by BSP previously
    if (ApLaunchGlobalData->UcodePatchAddr != 0) {
      if (CcxIsComputeUnitPrimary (&StdHeader)) {
        AsmWriteMsr64 (0xC0010020, ApLaunchGlobalData->UcodePatchAddr);
      }
    }
  }

  CcxProgramTablesAtReset (ApLaunchGlobalData->SleepType, (ENTRY_CRITERIA *) &ApLaunchGlobalData->ResetTableCriteria, &StdHeader);

  // Mca initialization
  CcxSetMca ();

  // Cac Weights initialization
  CcxZen3RmbSetCacWeights ((UINT64 *) &ApLaunchGlobalData->CacWeights[0]);

  // Last step: Sync up MSRs with BSP
  CcxZen3RmbSyncMiscMsrs (ApLaunchGlobalData);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbSetMiscMsrs
 *
 *  This routine is only executed on the BSP. APs will sync the applicable MSRs
 *  settings through ApMsrSyncList

 *  Description:
 *    This routine sets miscellaneous MSRs
 *
 */
VOID
CcxZen3RmbSetMiscMsrs (
  )
{
  UINT64    LocalMsrRegister;
  UINT8     RedirectForReturnDis;
  UINT8     OpCacheCtrl;
  UINT8     StreamingStoresCtrl;
  BOOLEAN   EnableFSRM;
  BOOLEAN   EnableERMS;
  BOOLEAN   IbsHardwareEn;
  BOOLEAN   EnableRMSS;

  RedirectForReturnDis = PcdGet8 (PcdAmdRedirectForReturnDis);
  OpCacheCtrl          = PcdGet8 (PcdAmdOpcacheCtrl);
  StreamingStoresCtrl  = PcdGet8 (PcdAmdStreamingStoresCtrl);
  EnableFSRM           = PcdGetBool (PcdAmdEnableFSRM);
  EnableERMS           = PcdGetBool (PcdAmdEnableERMS);
  IbsHardwareEn        = PcdGetBool (PcdAmdIbsHardwareEn);
  EnableRMSS           = PcdGetBool (PcdAmdEnableRMSS);

  // Force recalc of TSC on all threads after loading patch
  LocalMsrRegister = AsmReadMsr64 (0xC0010064);
  AsmWriteMsr64 (0xC0010064, LocalMsrRegister);

  if (CcxIsComputeUnitPrimary (NULL)) {
    // MSRC001_1023[49, TwCfgCombineCr0Cd] = 1
    AsmMsrOr64 (0xC0011023, BIT49);
  }

  // MSR_C001_1029[14]
  if (RedirectForReturnDis != 0xFF) {
    AsmMsrAndThenOr64 (0xC0011029, ~(UINT64) BIT14, LShiftU64 ((RedirectForReturnDis & 1), 14));
  }

  // MSR_C001_1021[5]
  if (OpCacheCtrl != 0xFF) {
    AsmMsrAndThenOr64 (0xC0011021, ~(UINT64) BIT5, LShiftU64 ((OpCacheCtrl & 1), 5));
  }

  // MSR_C001_1020[28]
  if (StreamingStoresCtrl != 0xFF) {
    AsmMsrAndThenOr64 (0xC0011020, ~(UINT64) BIT28, LShiftU64 ((StreamingStoresCtrl & 1), 28));
  }

  // MSR_C001_10DF[36]
  if (EnableFSRM) {
    AsmMsrOr64 (0xC00110DF, (UINT64) BIT36);
  } else {
    AsmMsrAnd64 (0xC00110DF, ~((UINT64) BIT36));
  }

  // MSR_C001_1002[9] : ERMS
  if (EnableERMS) {
    AsmMsrOr64 (0xC0011002, (UINT64) BIT9);
  } else {
    AsmMsrAnd64 (0xC0011002, ~((UINT64) BIT9));
  }

  // MSR_C001_1000[15]
  if (EnableRMSS) {
    AsmMsrOr64 (0xC0011000, (UINT64) BIT15);
  } else {
    AsmMsrAnd64 (0xC0011000, ~((UINT64) BIT15));
  }

  // MSR_C001_1020[54]
  if (IbsHardwareEn) {
    AsmMsrOr64 (0xC0011020, (UINT64) BIT54);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbPrefetcher
 *
 *  This routine is only executed on the BSP. APs will sync the applicable MSRs
 *  settings through ApMsrSyncList
 *
 *  Description:
 *    This routine initializes L1/L2 prefetcher per user configurations
 *
 */
VOID
CcxZen3RmbPrefetcher (
  )
{
  // L1 Stream HW Prefetcher
  if (!PcdGetBool (PcdAmdL1StreamPrefetcher)) {
    // MSR C001_1022[16] = 1
    AsmMsrOr64 (0xC0011022, BIT16);
  }

  // L1 Stride Prefetcher
  if (!PcdGetBool (PcdAmdL1StridePrefetcher)) {
    // MSR C001_1022[17] = 1
    AsmMsrOr64 (0xC0011022, BIT17);
  }

  // L1 Region Prefetcher
  if (!PcdGetBool (PcdAmdL1RegionPrefetcher)) {
    // MSR C001_1022[18] = 1
    AsmMsrOr64 (0xC0011022, BIT18);
  }

  // L2 Stream Prefetcher
  if (!PcdGetBool (PcdAmdL2StreamPrefetcher)) {
    // MSR C001_102B[0] = 0
    AsmMsrAnd64 (0xC001102B, ~((UINT64)BIT0));
  }

  // L2 Up/Down Prefetcher
  if (!PcdGetBool (PcdAmdL2UpDownPrefetcher)) {
    // MSR C001_102B[2] = 0
    AsmMsrAnd64 (0xC001102B, ~((UINT64)BIT2));
  }

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbEnableSmee
 *
 *  This routine is only executed on the BSP. APs will sync the applicable MSRs
 *  settings through ApMsrSyncList
 *
 *  Description:
 *    This routine enables secure memory encryption
 *
 */
VOID
CcxZen3RmbEnableSmee (
  )
{
  if (PcdGetBool (PcdAmdSmee)) {
    AsmMsrOr64 (0xC0010010, BIT23);
  }
}

/*++

Routine Description:

  Zen3 Microcode Patch loading

Arguments:
 *  @param[in,out] UcodePatchAddr        - The selected UcodePatch address, return 0 if not found
 *  @param[in] StdHeader                 - Config handle for library and services.
Returns:

--*/
VOID
CcxZen3RmbMicrocodeInit (
  IN OUT   UINT64             *UcodePatchAddr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  LoadMicrocodePatchV2 (UcodePatchAddr, StdHeader);
}

UINT32 CpuWdtCountSelDecode [] =
{
  4095,
  2047,
  1023,
  511,
  255,
  127,
  63,
  31,
  8191,
  16383
};
#define NumberOfCpuWdtCountSel (sizeof (CpuWdtCountSelDecode) / sizeof (CpuWdtCountSelDecode[0]))
#define MinCpuWdtCountSel 7
#define MaxCpuWdtCountSel 9

UINT8 CpuWdtCountSelBumpUp [] =
{
  7,
  6,
  5,
  4,
  3,
  2,
  1,
  0,
  8,
  9
};

UINT64 CpuWdtTimeBaseDecode [] =
{
  1310000,
  1280
};
#define NumberOfCpuWdtTimeBase (sizeof (CpuWdtTimeBaseDecode) / sizeof (CpuWdtTimeBaseDecode[0]))
#define MinCpuWdtTimeBase 1
#define MaxCpuWdtTimeBase 0


/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbEnableWdt
 *
 *  Description:
 *    This routine enables watchdog
 *
 */
VOID
CcxZen3RmbEnableWdt (
  IN       CORE_WATCHDOG  *CpuWdt
  )
{
  UINT8           BumpUpIndex;
  UINT16          CpuWdtTimeBase;
  UINT16          CpuWdtCountSel;
  UINT64          CpuWdtTime;
  UINT64          DfCcmWdtTime;
  CPU_WDT_CFG_MSR CpuWdtCfg;


  if ((CcxIsBsp (NULL)) && (FabricGetCcmWdtInfo (&DfCcmWdtTime)) && (CpuWdt->CpuWdtEn)) {
    CpuWdtTimeBase = CpuWdt->CpuWdtTimeOut & 0xFF;
    CpuWdtCountSel = (CpuWdt->CpuWdtTimeOut & 0xFF00) >> 8;
    ASSERT (CpuWdtTimeBase < NumberOfCpuWdtTimeBase);
    ASSERT (CpuWdtCountSel < NumberOfCpuWdtCountSel);

    // RESTRICTION: When both CPU WDT & DF WDT are enable, the DF CCM WDT timeout must be greater than the CPU WDT timeout limit
    CpuWdtTime = MultU64x64 (CpuWdtTimeBaseDecode[CpuWdtTimeBase], CpuWdtCountSelDecode[CpuWdtCountSel]);
    if (DfCcmWdtTime <= CpuWdtTime) {
      //IDS_HDT_CONSOLE (CPU_TRACE, "  WARNING: CPU WDT (%ld.%ld S) is greater than or equal to the DF CCM WDT (%ld.%ld S)\n", (CpuWdtTime / 1000000000), (CpuWdtTime % 1000000000), (DfCcmWdtTime / 1000000000), (DfCcmWdtTime % 1000000000));
      IDS_HDT_CONSOLE (CPU_TRACE, "  Current CPU WDT setting: CpuWdtCountSel %X, CpuWdtTimeBase %X\n", CpuWdtCountSel, CpuWdtTimeBase);
      // slow down the CPU WDT to at least the value of the DF CCM WDT
      IDS_HDT_CONSOLE (CPU_TRACE, "  Try to slow down CPU WDT\n");
      CpuWdt->CpuWdtEn = FALSE; // Disable CPU WDT
      while ((CpuWdtCountSel != MinCpuWdtCountSel) || (CpuWdtTimeBase != MinCpuWdtTimeBase)) {
        if ((CpuWdtCountSel == MinCpuWdtCountSel) && (CpuWdtTimeBase != MinCpuWdtTimeBase)) {
          // Try to change TimeBase
          CpuWdtCountSel = MaxCpuWdtCountSel;
          CpuWdtTimeBase = MinCpuWdtTimeBase;
        } else {
          // Try to change CountSel
          for (BumpUpIndex = (NumberOfCpuWdtCountSel - 1); BumpUpIndex > 0; BumpUpIndex--) {
            if (CpuWdtCountSelBumpUp[BumpUpIndex] == CpuWdtCountSel) {
              CpuWdtCountSel = CpuWdtCountSelBumpUp[BumpUpIndex - 1];
              break;
            }
          }
        }

        CpuWdtTime = MultU64x64 (CpuWdtTimeBaseDecode[CpuWdtTimeBase], CpuWdtCountSelDecode[CpuWdtCountSel]);

        if (DfCcmWdtTime > CpuWdtTime) {
          // Slow down succeed, enable CPU WDT
          CpuWdt->CpuWdtEn = TRUE;
          CpuWdt->CpuWdtTimeOut = CpuWdtTimeBase | (CpuWdtCountSel << 8);
          //IDS_HDT_CONSOLE (CPU_TRACE, "  Succeed! New CPU WDT %ld.%ld S\n", (CpuWdtTime / 1000000000), (CpuWdtTime % 1000000000));
          IDS_HDT_CONSOLE (CPU_TRACE, "  New CPU WDT setting: CpuWdtCountSel %X, CpuWdtTimeBase %X\n", CpuWdtCountSel, CpuWdtTimeBase);
          break;
        }
      }
      if (!CpuWdt->CpuWdtEn) {
        IDS_HDT_CONSOLE (CPU_TRACE, "  Failed, we would not enable CPU WDT\n");
      }
    }
  }

  CpuWdtCfg.Value = AsmReadMsr64 (0xC0010074);
  if (CpuWdt->CpuWdtEn) {
    CpuWdtCfg.Field.CpuWdTmrCfgEn = 1;
    CpuWdtCfg.Field.CpuWdTmrTimebaseSel = CpuWdt->CpuWdtTimeOut & 0xFF;
    CpuWdtCfg.Field.CpuWdTmrCfgCount = (CpuWdt->CpuWdtTimeOut & 0xFF00) >> 8;
    if (CpuWdt->CpuWdTmrCfgSeverity != 0xFF) {
      CpuWdtCfg.Field.CpuWdTmrCfgSeverity = CpuWdt->CpuWdTmrCfgSeverity;
    }
  } else {
    CpuWdtCfg.Field.CpuWdTmrCfgEn = 0;
    CpuWdtCfg.Field.CpuWdTmrTimebaseSel = 0;
    CpuWdtCfg.Field.CpuWdTmrCfgCount = 0;
  }
  AsmWriteMsr64 (0xC0010074, CpuWdtCfg.Value);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbGetCacWeights
 *
 *  This routine is executed by BSP and data will be shared with APs through
 *  ApLaunchGlobalData
 *
 *  @param[in] NbioSmuServices        SMU services
 *
 *  Description:
 *    This routine gets CAC weights from SMU
 *
 */
VOID
CcxZen3RmbGetCacWeights (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *NbioSmuServices,
  IN       UINT64                         *CacWeights
  )
{
  NbioSmuServices->SmuReadCacWeights (NbioSmuServices, ZEN3_CAC_WEIGHT_NUM, CacWeights);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbSetCacWeights
 *
 *  This routine is executed by both the BSP and AP(s)
 *
 *  Description:
 *    This routine sets all CAC weights
 *
 */
VOID
CcxZen3RmbSetCacWeights (
  IN       UINT64 *CacWeights
  )
{
  UINT8  WeightIndex;
  UINT64 LocalMsr;

  if (CcxIsComputeUnitPrimary (NULL)) {
    LocalMsr = AsmReadMsr64 (0xC0011074);
    AsmWriteMsr64 (0xC0011074, 0);

    for (WeightIndex = 0; WeightIndex < ZEN3_CAC_WEIGHT_NUM; WeightIndex++) {
      AsmWriteMsr64 (0xC0011076, WeightIndex);
      AsmWriteMsr64 (0xC0011077, CacWeights[WeightIndex]);
    }

    AsmWriteMsr64 (0xC0011074, (LocalMsr | BIT63));
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set up the BIOS owned registers associated with CC6 according to the PPR
 *
 * This routine is only executed on the BSP. APs will sync the applicable MSRs
 * settings through ApMsrSyncList
 *
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
CcxZen3RmbInitializeC6 (
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8                     CStateMode;
  CSTATE_CFG_MSR            CstateCfg;
  CSTATE_ADDRESS_MSR        CstateAddr;
  UINT8                     Cc6Enable;


  CStateMode = PcdGet8 (PcdAmdCStateMode);

  ASSERT (CStateMode <= 1);
  Cc6Enable = (PcdGet8 (PcdAmdCc6Ctrl) & 1);

  if (CStateMode == 1) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitializeC6 - Enable C6\n");

    if (CcxIsComputeUnitPrimary (StdHeader)) {
      CstateCfg.Value = 0;
      CstateCfg.Field.CCR0_CC1DFSID = 8;
      CstateCfg.Field.CCR0_CC6EN = Cc6Enable;
      CstateCfg.Field.CCR1_CC1DFSID = 8;
      CstateCfg.Field.CCR1_CC6EN = Cc6Enable;
      CstateCfg.Field.CCR2_CC1DFSID = 8;
      CstateCfg.Field.CCR2_CC6EN = Cc6Enable;
      AsmWriteMsr64 (MSR_CSTATE_CFG, CstateCfg.Value);
    }

    CstateAddr.Value = 0;
    CstateAddr.Field.CstateAddr = (UINT32) PcdGet16 (PcdAmdCStateIoBaseAddress);
    AsmWriteMsr64 (MSR_CSTATE_ADDRESS, CstateAddr.Value);
  }
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Initializes Core Performance Boost.
 *
 * Set up D18F4x15C[BoostSrc] and start the PDMs according to the BKDG.
 *
 * This routine is only executed on the BSP. APs will sync applicable
 * settings through ApMsrSyncList
 *
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
CcxZen3RmbInitializeCpb (
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8                     CpbMode;
  EFI_STATUS                Status;

  Status = AGESA_SUCCESS;

  CpbMode = PcdGet8 (PcdAmdCpbMode);

  if (CpbMode == 0) {
    AsmMsrOr64 (MSR_HWCR, BIT25);
  }
  return Status;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3InitializePrefetchMode
 *
 *  Description:
 *    This function provides for performance tuning to optimize for specific
 *    workloads. For general performance use the recommended settings.
 *
 *  Parameters:
 *    @param[in]  StdHeader                 Config handle for library and services
 *
 *    @retval     AGESA_STATUS
 *
 *
 */
AGESA_STATUS
CcxZen3RmbInitializePrefetchMode (
  IN       AMD_CONFIG_PARAMS                *StdHeader
  )
{
  CCX_PREFETCH_MODE         PrefetchMode;

  PrefetchMode.HardwarePrefetchMode = PcdGet8 (PcdAmdHardwarePrefetchMode);
  PrefetchMode.SoftwarePrefetchMode = PcdGet8 (PcdAmdSoftwarePrefetchMode);
  if ((PrefetchMode.HardwarePrefetchMode != HARDWARE_PREFETCHER_AUTO) ||
      (PrefetchMode.SoftwarePrefetchMode != SOFTWARE_PREFETCHES_AUTO)) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitializePrefetchMode:\n");
    IDS_HDT_CONSOLE (CPU_TRACE, "    HardwarePrefetchMode - %d\n", PrefetchMode.HardwarePrefetchMode);
    IDS_HDT_CONSOLE (CPU_TRACE, "    SoftwarePrefetchMode - %d\n", PrefetchMode.SoftwarePrefetchMode);

    switch (PrefetchMode.HardwarePrefetchMode) {
    case DISABLE_HARDWARE_PREFETCH:
      // DC_CFG (MSR_C001_1022)
      //  [13] = 1
      //  [15] = 1
      AsmMsrOr64 (MSR_DC_CFG, (BIT13 | BIT15));
      // CU_CFG3 (MSR_C001_102B)
      //  [3]  = 1
      //  [16] = 1
      //  [17] = 1
      //  [18] = 1
      if (CcxIsComputeUnitPrimary (NULL)) {
        AsmMsrOr64 (MSR_CU_CFG3, (BIT3 | BIT16 | BIT17 | BIT18));
      }
      break;
    case DISABLE_L2_STRIDE_PREFETCHER:
      // For DISABLE_L2_STRIDE_PREFETCHER, also implement the following :
      // DISABLE_L1_PREFETCHER
      // DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES
      // Hence, no break to allow code to flow through.
    case DISABLE_L1_PREFETCHER:
      // For this setting, also implement DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES.
      // Hence, no break to allow code to flow through.
      // CU_CFG3 (MSR_C001_102B)
      //  [3] = 1
      if (CcxIsComputeUnitPrimary (NULL)) {
        AsmMsrOr64 (MSR_CU_CFG3, BIT3);
      }
    case DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES:
      // DC_CFG (MSR_C001_1022)
      //  [15] = 1
      AsmMsrOr64 (MSR_DC_CFG, BIT15);
      break;
    default:
      break;
    }
    // DISABLE_SOFTWARE_PREFETCHES
    if (PrefetchMode.SoftwarePrefetchMode == DISABLE_SOFTWARE_PREFETCHES) {
      // MSR_DE_CFG (MSR_C001_1029)
      //  [7:2] = 0x3F
      if (CcxIsComputeUnitPrimary (NULL)) {
        AsmMsrOr64 (MSR_DE_CFG, 0xFC);
      }
    }
  }
  return AGESA_SUCCESS;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3RmbSyncMiscMsrs
 *
 *  This routine is only executed by the APs to sync MSR data to match BSP
 *  Description:
 *    This routine synchronizes the MSRs in ApLaunchGlobalData->ApMsrSyncList across all APs
 *
 */
VOID
CcxZen3RmbSyncMiscMsrs (
  IN       VOLATILE AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData
  )
{
  UINTN  i;

  for (i = 0; ApLaunchGlobalData->ApMsrSyncList[i].MsrAddr != CPU_LIST_TERMINAL; i++) {
    AsmMsrAndThenOr64 (
        ApLaunchGlobalData->ApMsrSyncList[i].MsrAddr,
        ~(ApLaunchGlobalData->ApMsrSyncList[i].MsrMask),
        (ApLaunchGlobalData->ApMsrSyncList[i].MsrData & ApLaunchGlobalData->ApMsrSyncList[i].MsrMask)
        );
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *  Notification function called when MP Services Ppi is installed.
 *
 *  This installs Smm Access PPI
 */
EFI_STATUS
CcxInitWithMpServices (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  CcxSmmAccessPpiInstall (PeiServices);
  return EFI_SUCCESS;
}

/* -----------------------------------------------------------------------------*/
/**
 *  Notification function called at End of PEI.
 *
 *  This locks SMRAM on S3 resume.
 */
EFI_STATUS
CcxEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  PEI_SMM_ACCESS_PPI    *SmmAccess;
  UINTN                 SmramMapSize;
  UINTN                 Index;
  UINTN                 NumOfDesc;
  EFI_BOOT_MODE         BootMode;
  EFI_STATUS            Status;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT (Status == EFI_SUCCESS);

  if (BootMode == BOOT_ON_S3_RESUME) {
    Status = PeiServicesLocatePpi (
      &gPeiSmmAccessPpiGuid,
      0,
      NULL,
      &SmmAccess
    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    SmramMapSize = 0;
    Status = SmmAccess->GetCapabilities (
      PeiServices,
      SmmAccess,
      &SmramMapSize,
      NULL
    );
    if (Status != EFI_BUFFER_TOO_SMALL) {
      return Status;
    }
    NumOfDesc = SmramMapSize / sizeof (EFI_SMRAM_DESCRIPTOR);

    for (Index = 0; Index < NumOfDesc; Index++) {
      SmmAccess->Lock (PeiServices, SmmAccess, Index);
    }
  }

  return EFI_SUCCESS;
}
