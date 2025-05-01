/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Implement Custom Core Pstates
 *
 * Contains code that Custom Core Pstates
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/CcxPstatesLib.h>
#include <Addendum/Apcb/Inc/RMB/APOB.h>
#include <Library/AmdPspApobLib.h>
#include <Library/CcxMpServicesLib.h>
#include <Library/CcxIdsCustomPstatesLib.h>
#include <Library/NbioHandleLib.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <Protocol/SocZen3ServicesProtocol.h>
#include <CcxRegistersZen3.h>
#include "CcxZen3RmbIdsCustomPstates.h"
#include <Filecode.h>

#define FILECODE LIBRARY_CCXZEN3RMBIDSHOOKLIB_DXE_CCXZEN3RMBIDSCUSTOMPSTATES_FILECODE

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
UpdatePstateOnAllCores (
  IN       UINT64 *Pstate
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/*---------------------------------------------------------------------------------------*/
/**
 * CcxZen3RmbIdsDxeCustomPstates - routine to read Custom Pstates information from IDS
 *
 *
 * @param[in]  HookId       IDS Hook Id
 * @param[in]  *Handle      Unused
 * @param[in]  *Data        Unused
 *
 * @retval     IDS_HOOK_STATUS
 *
 */
IDS_HOOK_STATUS
CcxZen3RmbIdsDxeCustomPstates (
  IN       HOOK_ID HookId,
  IN       VOID *Handle,
  IN       VOID *Data
  )
{
  UINT8   P0Vid;
  UINT32  P0Freq;
  UINT32  SmuArg[6];
  UINT32  SmuReturn[6];
  UINT16  OcVoltageMax;
  UINT16  OcFrequencyMax;
  PSTATE_MSR PstateMsr;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL      *NbioSmuServices;
  AMD_SOC_ZEN3_SERVICES_PROTOCOL          *SocZen3Services;
  SOC_ZEN3_OC_MSG_ID_TABLE                *OcMsgIdTable;
  EFI_STATUS                              Status;

  if ((!PcdGetBool (PcdOcDisable)) && (CcxIsBsp (NULL))) {
    IDS_HDT_CONSOLE (CPU_TRACE, "CcxZen3RmbIdsDxeCustomPstates Options\n");

    if (PcdGet8 (PcdAmdCcxP0Setting) == CCX_IDS_CORE_PSTATES_CUSTOM) {
      P0Freq = PcdGet32 (PcdAmdCcxP0Freq);
      P0Vid  = PcdGet8 (PcdAmdCcxP0Vid);
      OcVoltageMax = (PcdGet16 (PcdOcVoltageMax) == 0) ? 0xFFFF : PcdGet16 (PcdOcVoltageMax);
      OcFrequencyMax = (PcdGet16 (PcdOcFrequencyMax) == 0) ? 0xFFFF : PcdGet16 (PcdOcFrequencyMax);

      gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, &NbioSmuServices);

      Status = gBS->LocateProtocol (&gAmdSocZen3ServicesProtocolGuid, NULL, &SocZen3Services);
      ASSERT (SocZen3Services != NULL);
      if (!EFI_ERROR (Status)) {
        OcMsgIdTable = NULL;
        SocZen3Services->GetOcSmuMsgId (SocZen3Services, &OcMsgIdTable);
        ASSERT (OcMsgIdTable != NULL);
        ASSERT (OcMsgIdTable->Revision == SOC_OC_MSG_ID_MAP_REVISION);

        LibAmdMemFill (SmuArg, 0x00, 24, NULL);

        // Send BIOSSMC_MSG_EnableOverclocking
        NbioSmuServices->AmdSmuServiceRequest (NbioSmuServices, 0, OcMsgIdTable->EnableOverclocking, SmuArg, SmuReturn);

        PstateMsr.Value = AsmReadMsr64 (MSR_PSTATE_0);

        // Send BIOSSMC_MSG_SetOverclockVID
        if (P0Vid != 0xFF) {
          if (P0Vid > OcVoltageMax) {
            IDS_HDT_CONSOLE (CPU_TRACE, "  P0Vid %d exceeds OcVoltageMax, limit to %d\n", P0Vid, OcVoltageMax);
            P0Vid = (UINT8) OcVoltageMax;
          }
          SmuArg[0] = P0Vid;
          NbioSmuServices->AmdSmuServiceRequest (NbioSmuServices, 0, OcMsgIdTable->SetOverclockVID, SmuArg, SmuReturn);
          PstateMsr.Field.CpuVid = P0Vid;
        }

        // Send BIOSSMC_MSG_SetOverclockFreqAllCores
        if (P0Freq != 0xFFFFFFFF) {
          if (P0Freq > OcFrequencyMax) {
            IDS_HDT_CONSOLE (CPU_TRACE, "  P0Freq %d exceeds OcFrequencyMax limit to %d\n", P0Freq, OcFrequencyMax);
            P0Freq = OcFrequencyMax;
          }
          SmuArg[0] = P0Freq;
          NbioSmuServices->AmdSmuServiceRequest (NbioSmuServices, 0, OcMsgIdTable->SetOverclockFreqAllCores, SmuArg, SmuReturn);
          PstateMsr.Field.CpuFid = P0Freq / 25;
          PstateMsr.Field.CpuDfsId = PstateMsr.Field.CpuFid * 200 / P0Freq;
        }

        // By writing to MSR, we force ucode to re-evaluate the TSC
        CcxRunFunctionOnAps (ALL_THREADS, UpdatePstateOnAllCores, &PstateMsr.Value, NON_BLOCKING_MODE);
        UpdatePstateOnAllCores (&PstateMsr.Value);

        IDS_HDT_CONSOLE (CPU_TRACE, "  OC - Freq: %d;  VID: %X\n", P0Freq, P0Vid);
      } else {
        IDS_HDT_CONSOLE (CPU_TRACE, "  ERROR: SOC service unavailable!\n");
      }
    }

  } // End of CcxIsBsp

  return IDS_HOOK_SUCCESS;
}

VOID
UpdatePstateOnAllCores (
  IN       UINT64 *Pstate
  )
{
  AsmWriteMsr64 (MSR_PSTATE_0, *Pstate);
}

