/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains code associated with the CC6 feature for Zen3 processors
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Ccx
 * @e \$Revision$   @e \$Date$
 *
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/BaseLib.h>
#include <Porting.h>
#include <AMD.h>
#include <Filecode.h>
#include <CcxRegistersZen3.h>
#include <Library/AmdBaseLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/IdsLib.h>
#include <Protocol/MpService.h>

#define FILECODE CCX_ZEN3_DXE_CCXZEN3C6_FILECODE

extern  EFI_BOOT_SERVICES *gBS;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

typedef struct {
  UINT16          CStateBaseAddr;
  UINT8           Cc6Ctrl;
} CSTATE_CTRL_INFO;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
CcxZen3InitializeC6Aps (
  IN       CSTATE_CTRL_INFO  *CstateCtrlInfo
  );

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Enable CC6
 *
 * Set up the BIOS owned registers associated with CC6 according to the PPR
 *
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
CcxZen3InitializeC6 (
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8                     CStateMode;
  EFI_STATUS                Status;
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  CSTATE_CTRL_INFO          CstateCtrlInfo;

  CStateMode = PcdGet8 (PcdAmdCStateMode);

  ASSERT (CStateMode <= 1);
  CstateCtrlInfo.Cc6Ctrl = PcdGet8 (PcdAmdCc6Ctrl);

  if (CStateMode == 1) {
    IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitializeC6 - Enable C6\n");

    CstateCtrlInfo.CStateBaseAddr = PcdGet16 (PcdAmdCStateIoBaseAddress);

    Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
    ASSERT (!EFI_ERROR (Status));
    if (EFI_ERROR (Status)) {
      return AGESA_ERROR;
    }
    MpServices->StartupAllAPs (
        MpServices,
        CcxZen3InitializeC6Aps,
        FALSE,
        NULL,
        0,
        (VOID *) &CstateCtrlInfo,
        NULL
    );
    CcxZen3InitializeC6Aps (&CstateCtrlInfo);
  }
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3InitializeC6Aps
 *
 *  @param[in]  CStateBaseAddr   Pointer to CState base address
 *
 *  Description:
 *    This funtion enables C6 on all threads.
 *
 */
VOID
CcxZen3InitializeC6Aps (
  IN       CSTATE_CTRL_INFO  *CstateCtrlInfo
  )
{
  CSTATE_CFG_MSR      CstateCfg;
  AMD_CONFIG_PARAMS   StdHeader;
  CSTATE_ADDRESS_MSR  CstateAddr;
  UINT8               Cc6Enable;

  if (CcxIsComputeUnitPrimary (&StdHeader)) {
    Cc6Enable = CstateCtrlInfo->Cc6Ctrl & 1;
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
  CstateAddr.Field.CstateAddr = (UINT32) CstateCtrlInfo->CStateBaseAddr;
  AsmWriteMsr64 (MSR_CSTATE_ADDRESS, CstateAddr.Value);
}

