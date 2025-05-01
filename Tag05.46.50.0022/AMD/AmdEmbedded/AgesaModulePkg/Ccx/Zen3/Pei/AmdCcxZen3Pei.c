/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
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
#include <Library/CcxRolesLib.h>
#include <Library/CcxDownCoreLib.h>
#include <Library/CcxResetTablesLib.h>
#include <Library/AmdTableLib.h>
#include <Library/CcxApicLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/IdsLib.h>
#include <Library/FchBaseLib.h>
#include <Library/AmdCapsuleLib.h>
#include <Library/CcxSetMcaLib.h>
#include <Ppi/AmdCcxPpi.h>
#include <Ppi/AmdCoreTopologyServicesV2Ppi.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <PspResumeMemInfo.h>
#include <Filecode.h>

#define FILECODE CCX_ZEN3_PEI_AMDCCXZEN3PEI_FILECODE

STATIC PEI_AMD_CCX_INIT_COMPLETE_PPI mCcxPeiInitCompletePpi = {
  AMD_CCX_PPI_REVISION
};

STATIC EFI_PEI_PPI_DESCRIPTOR mCcxPeiInitCompletePpiList =
{
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCcxPeiInitCompletePpiGuid,
  &mCcxPeiInitCompletePpi
};

extern EFI_GUID gApSyncFlagNvVariableGuid;

VOID
CcxZen3PcdOverride (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

/*++

Routine Description:

  Zen3 Driver Entry.  Initialize the core complex.

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
EFIAPI
AmdCcxZen3PeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  UINTN                                  Socket;
  UINTN                                  Die;
  UINTN                                  Ccd;
  UINTN                                  Ccx;
  UINTN                                  Core;
  UINTN                                  Thread;
  UINTN                                  NumberOfSockets;
  UINTN                                  NumberOfDies;
  UINTN                                  NumberOfCcds;
  UINTN                                  NumberOfComplexes;
  UINTN                                  NumberOfCores;
  UINTN                                  NumberOfThreads;
  UINTN                                  VariableSize;
  VOLATILE UINT32                       *ApSyncFlag;
  UINT32                                 ApNumBfLaunch;
  BOOLEAN                                IsS3;
  EFI_STATUS                             Status;
  EFI_STATUS                             CalledStatus;
  AMD_CONFIG_PARAMS                      StdHeader;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PPI     *CoreTopologyServices;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServices;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *Variable;
  UINT8                                 ApicMode;
  PSP_SMM_HDR_DATA                      *PspSmmHdrData = NULL;

  AGESA_TESTPOINT (TpCcxPeiEntry, NULL);

  Status = EFI_SUCCESS;

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3PeiInit Entry\n");

  if (CcxIsBsp (&StdHeader)) {
    IDS_HOOK (IDS_HOOK_CCX_PEI, NULL, NULL);
    if ((FchReadSleepType () == 0x3) || AmdCapsuleGetStatus ()) {
      IsS3 = TRUE;
    } else {
      IsS3 = FALSE;
    }

    if (!IsS3) {
      CcxZen3CacheInit (&StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3CacheInit Done\n");
    } else if (AmdCapsuleGetStatus()) {
      InitializeVariableMTRRs (&StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "  InitializeVariableMTRRs Done\n");
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

    CalledStatus = (*PeiServices)->LocatePpi (
                                   PeiServices,
                                   &gAmdFabricTopologyServices2PpiGuid,
                                   0,
                                   NULL,
                                   &FabricTopologyServices
                                   );
    ASSERT (CalledStatus == EFI_SUCCESS);
    Status = (CalledStatus > Status) ? CalledStatus : Status;

    FabricTopologyServices->GetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);
    CoreTopologyServices->GetCoreTopologyOnDie (0, 0, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads); // get NumberOfComplexes, NumberOfCores, NumberOfThreads of Socket 0

    ApicMode = PcdGet8 (PcdAmdApicMode);
    switch (ApicMode) {
      case xApicMode:
        // xApic mode and total thread count is >= 255, downcore to support xApic mode
        if ((NumberOfSockets * NumberOfCcds * NumberOfComplexes * NumberOfCores * NumberOfThreads) >= XAPIC_ID_MAX) {
          IDS_HDT_CONSOLE (CPU_TRACE, "  PcdAmdDownCoreMode is changed to CCX_DOWN_CORE_7_0\n");
          PcdSet8S (PcdAmdDownCoreMode, CCX_DOWN_CORE_7_0);
        }
        break;
      case x2ApicMode:
        break;
      case ApicAutoMode:
        ApicMode = ((NumberOfSockets * NumberOfCcds * NumberOfComplexes * NumberOfCores * NumberOfThreads) < XAPIC_ID_MAX) ? xApicMode : x2ApicMode;
        PcdSet8S (PcdAmdApicMode, ApicMode);
        break;
      case ApicCompatibilityMode:
        // set xApic mode when total thread count is < 255, this must be done before CcxProgramTablesAtReset ()
        if ((NumberOfSockets * NumberOfCcds * NumberOfComplexes * NumberOfCores * NumberOfThreads) < XAPIC_ID_MAX) {
          ApicMode = xApicMode;
        }
        IDS_HDT_CONSOLE (CPU_TRACE, "  PcdAmdApicMode is changed to 0x%X\n", ApicMode);
        PcdSet8S (PcdAmdApicMode, ApicMode);
        break;
      default:
        // ERROR - Undefined Apic mode
        ASSERT(FALSE);
        break;
    }

    CcxZen3DownCoreInit (PeiServices);

    CcxProgramTablesAtReset ((IsS3 ? 3 : 0), &StdHeader);
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxProgramTablesAtReset Done\n");

    if (!IsS3) {
      CcxZen3SetBrandString (&StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3SetBrandString Done\n");
    } else {
      AGESA_TESTPOINT (TpCcxPeiStartLaunchApsForS3, NULL);
      IDS_HDT_CONSOLE (CPU_TRACE, "    Launching APs in PEI for S3\n");

      ApNumBfLaunch = 0;
      ApSyncFlag = NULL;
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
        PspSmmHdrData = (PSP_SMM_HDR_DATA *)AsmReadMsr64(0xC0010112); //SMMADDR_ADDRESS - Start of TSEG
        ApSyncFlag = (VOLATILE UINT32*)&PspSmmHdrData->ApSyncFlag;
        ASSERT (ApSyncFlag != NULL);
        *ApSyncFlag = 0;
        PspSmmHdrData->ApStackTop = PspSmmHdrData->PspSmmRsmMemInfo.StackPtr + PspSmmHdrData->PspSmmRsmMemInfo.BspStackSize + PspSmmHdrData->PspSmmRsmMemInfo.ApStackSize;
        PspSmmHdrData->CcxEarlyInit = (EFI_PHYSICAL_ADDRESS)CcxSetMca;  // Call for APs when they are launched.
        CcxSetMca (); // Call for BSP.
      }

      for (Socket = 0; Socket < NumberOfSockets; Socket++) {
        FabricTopologyServices->GetProcessorInfo (Socket, &NumberOfDies, NULL);
        for (Die = 0; Die < NumberOfDies; Die++) {
          CoreTopologyServices->GetCoreTopologyOnDie (Socket, Die, &NumberOfCcds, &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
          for (Ccd = 0; Ccd < NumberOfCcds; Ccd++) {
            for (Ccx = 0; Ccx < NumberOfComplexes; Ccx++) {
              for (Core = 0; Core < NumberOfCores; Core++) {
                for (Thread = 0; Thread < NumberOfThreads; Thread++) {
                  if (!((Socket == 0) && (Die == 0) && (Ccd == 0) && (Ccx == 0) && (Core == 0) && (Thread == 0))) {
                    IDS_HDT_CONSOLE (CPU_TRACE, "      Launch socket %X die %X ccd %X complex %X core %X thread %X\n", Socket, Die, Ccd, Ccx, Core, Thread);
                    if (ApSyncFlag != NULL) {
                      ApNumBfLaunch = *ApSyncFlag;
                    }
                    CoreTopologyServices->LaunchThread (Socket, Die, Ccd, Ccx, Core, Thread);
                    // Wait until the core launch
                    if (ApSyncFlag != NULL) {
                      while (*ApSyncFlag != (ApNumBfLaunch + 1)) {
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
        }
      }

      // MSRC001_1023[49, TwCfgCombineCr0Cd] = 1
      AsmMsrOr64 (0xC0011023, BIT49);

      AGESA_TESTPOINT (TpCcxPeiEndLaunchApsForS3, NULL);
    }

    CalledStatus = (**PeiServices).InstallPpi (PeiServices, &mCcxPeiInitCompletePpiList);
    Status = (CalledStatus > Status) ? CalledStatus : Status;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  CcxZen3PcdOverride (&StdHeader);

  IDS_HDT_CONSOLE (CPU_TRACE, "  AmdCcxZen3PeiInit End\n");

  AGESA_TESTPOINT (TpCcxPeiExit, NULL);

  return (Status);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3PcdOverride
 *
 *  Description:
 *    This routine overrides multiple Pcds.
 *
 */
VOID
CcxZen3PcdOverride (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  SOC_LOGICAL_ID      SocLogicalId = {0, 0};

  GetSocLogicalIdOnCurrentCore (StdHeader, &SocLogicalId);
  // Zen3 B0 only
  if ((((SocLogicalId.Family & AMD_FAMILY_19_VMR) != 0) &&
      ((SocLogicalId.Revision & AMD_REV_F19_VMR_B0) != 0))) {
    if (PcdGetBool (PcdLN2ModeEnable)) {
      PcdSet8S (PcdAmdCStateMode, 0);
    }
  }

}
