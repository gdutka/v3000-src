/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD SOC BIST Logging V2 Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "Uefi.h"
#include <Library/BaseLib.h>
#include "AMD.h"
#include "Filecode.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/PeiSocBistLib.h>
#include <Ppi/FabricTopologyServices2Ppi.h>
#include <Ppi/AmdCoreTopologyServicesV2Ppi.h>
#include <Ppi/AmdErrorLogPpi.h>

#define FILECODE LIBRARY_PEISOCBISTLOGGING2LIB_SOCBISTLOGGING2LIB_FILECODE


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

 /*----------------------------------------------------------------------------------------
  *                          E X P O R T E D    F U N C T I O N S
  *----------------------------------------------------------------------------------------
 */

 /*---------------------------------------------------------------------------------------*/
 /**
  *
  * Logs an error if BIST failures are detected
  *
  * @param[in]      PeiServices            Pointer to PEI services
  *
  */
VOID
LogBistStatusV2 (
  IN       CONST EFI_PEI_SERVICES **PeiServices
  )
{
  UINTN                                 i;
  UINTN                                 j;
  UINTN                                 k;
  UINTN                                 l;
  UINTN                                 NumberOfSockets;
  UINTN                                 NumberOfDies;
  UINTN                                 CcdCount;
  UINTN                                 ComplexCount;
  UINTN                                 CurrSysDieCount;
  UINTN                                 Unused;
  UINT32                                BistData;
  EFI_STATUS                            CalledStatus;
  AGESA_STATUS                          BistStatus;
  PEI_AMD_ERROR_LOG_PPI                 *AmdErrorLog;
  AMD_PEI_FABRIC_TOPOLOGY_SERVICES2_PPI *FabricTopologyServices;
  AMD_CORE_TOPOLOGY_SERVICES_V2_PPI     *CoreTopologyServices;

  CalledStatus = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gAmdFabricTopologyServices2PpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&FabricTopologyServices
                                 );

  ASSERT (CalledStatus == EFI_SUCCESS);

  CalledStatus = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gAmdCoreTopologyServicesV2PpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&CoreTopologyServices
                                 );

  if (CalledStatus != EFI_SUCCESS) {
    return;
  }

  CalledStatus = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gAmdErrorLogPpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&AmdErrorLog
                                 );

  ASSERT (CalledStatus == EFI_SUCCESS);

  FabricTopologyServices->GetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);

  CurrSysDieCount = 0;

  for (i = 0; i < NumberOfSockets; i++) {
    FabricTopologyServices->GetProcessorInfo (i, &NumberOfDies, NULL);
    for (j = 0; j < NumberOfDies; j++) {
      CoreTopologyServices->GetCoreTopologyOnDie (i, j, &CcdCount, &ComplexCount, &Unused, &Unused);
      for (k = 0; k < CcdCount; k++) {
        BistStatus = ReadNonCcxBistData (i, j, k, CurrSysDieCount, &BistData, PeiServices);
        if (BistStatus != AGESA_SUCCESS) {
          IDS_HDT_CONSOLE (MAIN_FLOW, "***************************** !!Non-CCX BIST ERROR!! Socket %d Die %d CCD %d *********************\n", i, j, k);

          AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                       AGESA_ALERT,
                                       CPU_EVENT_NON_CCX_BIST_FAILURE,
                                       i,
                                       j,
                                       k,
                                       BistData
                                       );
        }

        for (l = 0; l < ComplexCount; l++) {
          // Read Bist
          BistStatus = ReadCcxBistData (i, j, k, l, CurrSysDieCount, ComplexCount, &BistData, PeiServices);

          // If there's a BIST failure, log it in error log
          if (BistStatus != AGESA_SUCCESS) {

            IDS_HDT_CONSOLE (MAIN_FLOW, "***************************** !!CCX BIST ERROR!! Socket %d CCD %d Complex %d *********************\n", i, k, l);

            AmdErrorLog->AmdErrorLogPei (AmdErrorLog,
                                         AGESA_ALERT,
                                         CPU_EVENT_CCX_BIST_FAILURE,
                                         i,
                                         k,
                                         l,
                                         BistData
                                        );
          }
        }
      }
      CurrSysDieCount++;
    }
  }
}


