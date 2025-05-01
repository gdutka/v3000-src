/*****************************************************************************
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Memory API, and related functions.
 *
 * Contains code that initializes memory
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision:  $   @e \$Date: $
 *
 */
/*++
Module Name:

  AmdMbistPei.c
  Memory MBIST Pei Module

Abstract:
--*/

#include "PiPei.h"
#include <Agesa.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/AmdPspApobLib.h>
#include <Library/HobLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/FchBaseLib.h>
#include <Library/MemRestore.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/AmdErrorLogPpi.h>
#include <Ppi/Reset2.h>
#include <Addendum/Apcb/Inc/GN/APOB.h>
#include "Filecode.h"

#define FILECODE MEM_AMDMEMTESTEXPEI_AMDMEMTESTEXPEI_FILECODE

typedef union {
  UINT8   umc_fail[2][8];       ///< [socket][umc]
  UINT32  umc_fail32[4];        ///<
} AGESA_MEM_TEST_UMC_FAIL_RESULT;

#define ABL_MEM_AGESA_MEMORY_TEST_ERROR 0x4003

#define MEM_ANY_DIE_UMC_MCA_FAIL  0x554d4346   ///< 'UMCF'
#define PEI_STACK_SIZE            0x200000

/*++

Routine Description:

  This function will get MemTest results from ABL

Arguments:

  AgesaMemTestUmcFailResult     -     Pointer to AGESA_MEM_TEST_UMC_FAIL_RESULT

Returns:
  None
--*/

VOID
EFIAPI
GetAblMemTestResults (
    IN OUT  AGESA_MEM_TEST_UMC_FAIL_RESULT  *AgesaMemTestUmcFailResult
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  APOB_TYPE_HEADER      *ApobEntries[APOB_ENTRY_INSTANCE_MAX];
  UINT32                NumofEntry;
  EVENT_LOG_STRUCT      *EventLogPtr;
  UINT16                i;
  UINT16                entryIndex;
  UINT8                 Die, Channel, Cs;

  Status = AmdPspGetApobEntry (APOB_GEN, APOB_GEN_EVENT_LOG_TYPE, FALSE, &NumofEntry, ApobEntries);
  if(Status == EFI_SUCCESS) {
    for(entryIndex = 0; entryIndex < NumofEntry; entryIndex++) {
      EventLogPtr = (EVENT_LOG_STRUCT *) ApobEntries[entryIndex];
      for (i = 0; i < EventLogPtr->Count; i++) {
        if(EventLogPtr->ApobEventStruct.AgesaEvent[i].EventInfo == ABL_MEM_AGESA_MEMORY_TEST_ERROR){
          Die = EventLogPtr->ApobEventStruct.AgesaEvent[i].DataA & 1;
          Channel = (EventLogPtr->ApobEventStruct.AgesaEvent[i].DataA >> 8) & 7;
          Cs = (EventLogPtr->ApobEventStruct.AgesaEvent[i].DataA >> 16) & 3;
          AgesaMemTestUmcFailResult->umc_fail[Die][Channel] = TRUE | (Cs << 4);
          IDS_HDT_CONSOLE (MAIN_FLOW, "ABL Memory Test fail at Die = %d, Channel %d, Dimm %d\n",Die, Channel, (Cs >> 1));
        }
      }
    }
  }
}

EFI_STATUS
EFIAPI
AmdMemTestExEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                            Status;
  PEI_AMD_NBIO_SMU_SERVICES_PPI         *NbioSmuServices;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServices;
  EFI_PEI_RESET2_PPI                    *Reset2;
  UINTN                                 Socket;
  UINTN                                 Die;
  UINTN                                 DieCount;
  UINTN                                 SystemDieCount;
  UINTN                                 SocketCount;
  UINT32                                SystemDieLoop;
  AGESA_MEM_TEST_UMC_FAIL_RESULT        AgesaMemTestUmcFailResult;
  UINT32                                TOM, Index, Data32, Value32;
  UINT8                                 i;
  volatile UINT32                       *MemPtr;
  BOOLEAN                               MemTestPassed = TRUE;
  UINT64                                data64;
  UINT64                                mca_status_umc[2][8];
  UINT64                                mca_addr_umc[2][8];
  UINT64                                mca_ipid_umc[2][8];
  UINT64                                mca_synd_umc[2][8];
  UINT64                                mca_destat_umc[2][8];
  UINT64                                mca_deaddr_umc[2][8];
  UINT64                                mca_misc0_umc[2][8];
  UINT64                                mca_misc1_umc[2][8];
  UINT64                                mca_pspstat_umc[2][8];
  UINT64                                mca_ctl_umc[2][8];
  BOOLEAN                               umc_fail[2][8];
  UINT32                                umc;
  UINT8                                 mca_synd_umc_cs[2][8];

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioSmuServicesPpiGuid, 0, NULL, &NbioSmuServices);
  if(EFI_ERROR(Status))
    return Status;

  Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdFabricTopologyServices2PpiGuid, 0, NULL, &FabricTopologyServices);
  if(EFI_ERROR(Status))
    return Status;

  FabricTopologyServices->GetSystemInfo (&SocketCount, &SystemDieCount, NULL, NULL, NULL);

  (*PeiServices)->SetMem(&AgesaMemTestUmcFailResult, sizeof(AgesaMemTestUmcFailResult), 0);
  GetAblMemTestResults(&AgesaMemTestUmcFailResult);

  SystemDieLoop = 0;
  for (Socket = 0; Socket < SocketCount; Socket++) {
    FabricTopologyServices->GetProcessorInfo (Socket, &DieCount, NULL);
    for (Die = 0; Die < DieCount; Die++) {
      for(umc = 0; umc < ABL_APOB_MAX_CHANNELS_PER_DIE; umc++){
        // (1) Save following register before test
        // MSR0000_043D [UMC Machine Check Status ] (MCA_STATUS_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f08 | (umc << 20)), (UINT32 *)&mca_status_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f0c | (umc << 20)), (UINT32 *)&mca_status_umc[SystemDieLoop][umc] + 1);

        // MSR0000_0446 [UMC Machine Check Address ] (MCA_ADDR_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f10 | (umc << 20)), (UINT32 *)&mca_addr_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f14 | (umc << 20)), (UINT32 *)&mca_addr_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2115 [UMC IP Identification] (MCA_IPID_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f28 | (umc << 20)), (UINT32 *)&mca_ipid_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f2c | (umc << 20)), (UINT32 *)&mca_ipid_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2116 [UMC Machine Check Syndrome ] (MCA_SYND_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f30 | (umc << 20)), (UINT32 *)&mca_synd_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f34 | (umc << 20)), (UINT32 *)&mca_synd_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2118 [UMC Machine Check Deferred Error Status ] (MCA_DESTAT_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f40 | (umc << 20)), (UINT32 *)&mca_destat_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f44 | (umc << 20)), (UINT32 *)&mca_destat_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2119 [UMC Deferred Error Address ] (MCA_DEADDR_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f48 | (umc << 20)), (UINT32 *)&mca_deaddr_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f4c | (umc << 20)), (UINT32 *)&mca_deaddr_umc[SystemDieLoop][umc] + 1);

        // MSR0000_0447 [UMC Machine Check Miscellaneous 0 ] (MCA_MISC0_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f18 | (umc << 20)), (UINT32 *)&mca_misc0_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f1c | (umc << 20)), (UINT32 *)&mca_misc0_umc[SystemDieLoop][umc] + 1);

        // MSRC000_211A [UMC Machine Check Miscellaneous 1 ] (MCA_MISC1_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f50 | (umc << 20)), (UINT32 *)&mca_misc1_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f54 | (umc << 20)), (UINT32 *)&mca_misc1_umc[SystemDieLoop][umc] + 1);

        // UMC0CHx00000F90 [UMC Machine Check PSP Status] (MCA_PSPSTAT_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f90 | (umc << 20)), (UINT32 *)&mca_pspstat_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f94 | (umc << 20)), (UINT32 *)&mca_pspstat_umc[SystemDieLoop][umc] + 1);

        // (2) Enable MCA bank
        // UMC0CHx00000F88 [UMC Machine Check Microcode Control ] (MCG_CTL_SHADOW_UMC)
        data64 = 1;
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f88 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f8c | (umc << 20)), (UINT32 *)&data64 + 1);

        // (3) Set MCA_CTL_UMC[Enable]=1 to enable MCA logging (but not interrupts)
        // MSR0000_0444 [UMC Machine Check Control] (MCA_CTL_UMC)
        data64 = 1;
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f00 | (umc << 20)), (UINT32 *)&mca_ctl_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f04 | (umc << 20)), (UINT32 *)&mca_ctl_umc[SystemDieLoop][umc] + 1);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f00 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f04 | (umc << 20)), (UINT32 *)&data64 + 1);

        // (6) Clear following register
        // MSR0000_0445 [UMC Machine Check Status ] (MCA_STATUS_UMC)
        data64 = 0;
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f08 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f0c | (umc << 20)), (UINT32 *)&data64 + 1);

        // MSRC000_2118 [UMC Machine Check Deferred Error Status ] (MCA_DESTAT_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f40 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f44 | (umc << 20)), (UINT32 *)&data64 + 1);

      }
      SystemDieLoop++;
    }
  }

  MemPtr = (volatile UINT32 *)0;
  TOM = (UINT32)AsmReadMsr64 (TOP_MEM);
  for(Index = (PEI_STACK_SIZE/sizeof(UINT32)); Index < (TOM/sizeof(UINT32)); Index++){
    Data32 = MemPtr[Index];
  }

  SystemDieLoop = 0;
  for (Socket = 0; Socket < SocketCount; Socket++) {
    FabricTopologyServices->GetProcessorInfo (Socket, &DieCount, NULL);
    for (Die = 0; Die < DieCount; Die++) {
      for(umc = 0; umc < ABL_APOB_MAX_CHANNELS_PER_DIE; umc++){
        umc_fail[SystemDieLoop][umc] = FALSE;

        // (7) Check MCA status
        // MSRC000_2118 [UMC Machine Check Deferred Error Status ] (MCA_DESTAT_UMC)
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f40 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f44 | (umc << 20)), (UINT32 *)&data64 + 1);
        IDS_HDT_CONSOLE (MAIN_FLOW, "\tDie %d MCA_DESTAT_UMC %d = %08x_%08x\n", SystemDieLoop, umc, (UINT32)(data64 >> 32), (UINT32)(data64 & 0xffffffff));

        mca_synd_umc_cs[SystemDieLoop][umc] = 0;
        if ((data64 >> 63) && ((data64 >> 44) & 1)) {
          umc_fail[SystemDieLoop][umc] = TRUE;
          // MSRC000_2116 [UMC Machine Check Syndrome ] (MCA_SYND_UMC)
          NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f30 | (umc << 20)), (UINT32 *)&data64);
          NbioSmuServices->SmuRegisterRead (NbioSmuServices, SystemDieLoop, (0x050f34 | (umc << 20)), (UINT32 *)&data64 + 1);
          mca_synd_umc_cs[SystemDieLoop][umc] = data64 & 7;    // [2:0] = chip select
          IDS_HDT_CONSOLE (MAIN_FLOW, "\tDie %d MCA_SYND_UMC   %d = %08x_%08x\n", SystemDieLoop, umc, (UINT32)(data64 >> 32), (UINT32)(data64 & 0xffffffff));
          IDS_HDT_CONSOLE (MAIN_FLOW, "\t\tMachine Check Deferred Error at Die %d UMC %d\n", SystemDieLoop, umc);
          if(AgesaMemTestUmcFailResult.umc_fail[SystemDieLoop][umc] == 0){
            MemTestPassed = FALSE;
            AgesaMemTestUmcFailResult.umc_fail[SystemDieLoop][umc] = (mca_synd_umc_cs[SystemDieLoop][umc] << 4) | umc_fail[SystemDieLoop][umc];
          }
        }

        // (8) Restore following register after test
        data64 = 0;
        // MSR0000_0444 [UMC Machine Check Control] (MCA_CTL_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f00 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f04 | (umc << 20)), (UINT32 *)&data64 + 1);

        // UMC0CHx00000F88 [UMC Machine Check Microcode Control ] (MCG_CTL_SHADOW_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f88 | (umc << 20)), (UINT32 *)&data64);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f8c | (umc << 20)), (UINT32 *)&data64 + 1);

        // MSR0000_0445 [UMC Machine Check Status ] (MCA_STATUS_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f08 | (umc << 20)), (UINT32 *)&mca_status_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f0c | (umc << 20)), (UINT32 *)&mca_status_umc[SystemDieLoop][umc] + 1);

        // MSR0000_0446 [UMC Machine Check Address ] (MCA_ADDR_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f10 | (umc << 20)), (UINT32 *)&mca_addr_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f14 | (umc << 20)), (UINT32 *)&mca_addr_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2115 [UMC IP Identification] (MCA_IPID_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f28 | (umc << 20)), (UINT32 *)&mca_ipid_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f2c | (umc << 20)), (UINT32 *)&mca_ipid_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2116 [UMC Machine Check Syndrome ] (MCA_SYND_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f30 | (umc << 20)), (UINT32 *)&mca_synd_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f34 | (umc << 20)), (UINT32 *)&mca_synd_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2118 [UMC Machine Check Deferred Error Status ] (MCA_DESTAT_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f40 | (umc << 20)), (UINT32 *)&mca_destat_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f44 | (umc << 20)), (UINT32 *)&mca_destat_umc[SystemDieLoop][umc] + 1);

        // MSRC000_2119 [UMC Deferred Error Address ] (MCA_DEADDR_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f48 | (umc << 20)), (UINT32 *)&mca_deaddr_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f4c | (umc << 20)), (UINT32 *)&mca_deaddr_umc[SystemDieLoop][umc] + 1);

        // MSR0000_0447 [UMC Machine Check Miscellaneous 0 ] (MCA_MISC0_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f18 | (umc << 20)), (UINT32 *)&mca_misc0_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f1c | (umc << 20)), (UINT32 *)&mca_misc0_umc[SystemDieLoop][umc] + 1);

        // MSRC000_211A[UMC Machine Check Miscellaneous 1 ] (MCA_MISC1_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f50 | (umc << 20)), (UINT32 *)&mca_misc1_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f54 | (umc << 20)), (UINT32 *)&mca_misc1_umc[SystemDieLoop][umc] + 1);

        // UMC0CHx00000F90 [UMC Machine Check PSP Status] (MCA_PSPSTAT_UMC)
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f90 | (umc << 20)), (UINT32 *)&mca_pspstat_umc[SystemDieLoop][umc]);
        NbioSmuServices->SmuRegisterWrite (NbioSmuServices, SystemDieLoop, (0x050f94 | (umc << 20)), (UINT32 *)&mca_pspstat_umc[SystemDieLoop][umc] + 1);

      }

      SystemDieLoop++;
    }
  }

  if(!MemTestPassed){
    Value32 = MEM_ANY_DIE_UMC_MCA_FAIL;
    FchWriteBiosram(0, 4, &Value32, NULL);
    for (i = 0; i < 4 ; i++) {
      Value32 = AgesaMemTestUmcFailResult.umc_fail32[i];
      FchWriteBiosram(4 + (i * 4), 4, &Value32, NULL);
    }

    Reset2 = NULL;
    (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiReset2PpiGuid, 0, NULL, &Reset2);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Issuing warm reset to map out bad DIMMs.\n");
    AmdMemRestoreDiscardCurrentMemContext();
    if (Reset2 != NULL) {
      Reset2->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    } else {
      (**PeiServices).ResetSystem (PeiServices);
    }
  }

  return Status;
}



