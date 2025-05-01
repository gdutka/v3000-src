/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains code associated with core performance boost for Zen3 processors
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
#include <cpuRegisters.h>
#include <CcxRegistersZen3.h>
#include <Filecode.h>
#include <Library/AmdS3SaveLib.h>
#include <Protocol/MpService.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>

#define FILECODE CCX_ZEN3_DXE_CCXZEN3CPB_FILECODE

extern  EFI_BOOT_SERVICES *gBS;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
EFI_EVENT            CcxZen3DisableCpbMpServicesEvent;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
CcxZen3DisableCpbAps (
  IN       VOID  *Void
  );

VOID
EFIAPI
CcxZen3DisableCpbApsCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
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
 * Enabling Core Performance Boost.
 *
 * Set up D18F4x15C[BoostSrc] and start the PDMs according to the BKDG.
 *
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
CcxZen3InitializeCpb (
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  UINT8                     CpbMode;
  EFI_STATUS                Status;
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  EFI_STATUS                CalledStatus;
  VOID                      *Registration;

  Status = AGESA_SUCCESS;

  CpbMode = PcdGet8 (PcdAmdCpbMode);

  if (CpbMode == 0) {
    CalledStatus = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
    if (EFI_ERROR (CalledStatus)) {
      //
      // Set up callback after MP services are available.
      //
      CalledStatus = gBS->CreateEventEx (
                               EVT_NOTIFY_SIGNAL,
                               TPL_NOTIFY,
                               CcxZen3DisableCpbApsCallback,
                               NULL,
                               NULL,
                               &CcxZen3DisableCpbMpServicesEvent
                               );
      Status = (CalledStatus > Status) ? CalledStatus : Status;

      CalledStatus = gBS->RegisterProtocolNotify (
                              &gEfiMpServiceProtocolGuid,
                              CcxZen3DisableCpbMpServicesEvent,
                              &Registration
                              );
      Status = (CalledStatus > Status) ? CalledStatus : Status;
    } else {
      IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3InitializeCpb - Disable CPB\n");

      MpServices->StartupAllAPs (
          MpServices,
          CcxZen3DisableCpbAps,
          FALSE,
          NULL,
          0,
          NULL,
          NULL
      );
      CcxZen3DisableCpbAps (NULL);
    }
  }
  return Status;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3DisableCpbAps
 *
 *  Description:
 *    This funtion disables CPB on all threads.
 *
 */
VOID
CcxZen3DisableCpbAps (
  IN       VOID  *Void
  )
{
  AsmMsrOr64 (MSR_HWCR, BIT25);
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *  CcxZen3DisableCpbApsCallback
 *
 *  Description:
 *     notification event handler after MpService Protocol ready
 *  Parameters:
 *    @param[in]     Event      Event whose notification function is being invoked.
 *    @param[in]     *Context   Pointer to the notification function's context.
 *
 */
VOID
EFIAPI
CcxZen3DisableCpbApsCallback (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS                Status;
  EFI_MP_SERVICES_PROTOCOL  *MpServices;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CcxZen3DisableCpbApsCallback - Disable CPB\n");

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  ASSERT (!EFI_ERROR (Status));
  MpServices->StartupAllAPs (
      MpServices,
      CcxZen3DisableCpbAps,
      FALSE,
      NULL,
      0,
      NULL,
      NULL
  );
  CcxZen3DisableCpbAps (NULL);

  gBS->CloseEvent(Event);
}

