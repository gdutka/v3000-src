/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AmdNbioPcieZPDxe Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioPcieZPDxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmdNbioBaseServicesProtocol.h>
#include <Protocol/AmdNbioPcieServicesProtocol.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Protocol/PciIo.h>
#include <Library/PcieConfigLib.h>
#include <Library/NbioHandleLib.h>
#include <GnbRegistersRMB.h>
#include <Library/SmnAccessLib.h>

#define FILECODE NBIO_NBIOBASE_RMB_DXE_AMDNBIOBASE_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * set HWINIT_WR_LOCK
 *
 *
 *
 * @param[in]  GnbHandle      Pointer to the Silicon Descriptor for this node
 */

VOID
NbioBaseSetHwInitLock (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE(GnbHandle, SMN_DEV0_NBIF0PORT0DEC_DN_PCIE_CNTL_ADDRESS),
                   (UINT32) ~(DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK),
                   1 << DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET,
                   GNB_REG_ACC_FLAG_S3SAVE
                   );
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE(GnbHandle, SMN_DEV1_NBIF0PORT1DEC_DN_PCIE_CNTL_ADDRESS),
                   (UINT32) ~(DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK),
                   1 << DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET,
                   GNB_REG_ACC_FLAG_S3SAVE
                   );
  SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                   NBIO_SPACE(GnbHandle, SMN_DEV2_NBIF0PORT2DEC_DN_PCIE_CNTL_ADDRESS),
                   (UINT32) ~(DN_PCIE_CNTL_HWINIT_WR_LOCK_MASK),
                   1 << DN_PCIE_CNTL_HWINIT_WR_LOCK_OFFSET,
                   GNB_REG_ACC_FLAG_S3SAVE
                   );

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/**
 *---------------------------------------------------------------------------------------
 *
 * create event for set HWINIT_WR_LOCK.
 *
 *
 *
 * @param[in]     Event
 * @param[in]     *Context
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
NbioBaseHookPciIO (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{

  EFI_STATUS                          Status;
  PCIe_PLATFORM_CONFIG                *Pcie;
  DXE_AMD_NBIO_PCIE_SERVICES_PROTOCOL *PcieServicesProtocol;
  GNB_PCIE_INFORMATION_DATA_HOB       *PciePlatformConfigHobData;
  GNB_HANDLE                          *GnbHandle;

  AGESA_TESTPOINT (TpNbioBaseHookPciIOEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  Status = gBS->LocateProtocol (
                  &gAmdNbioPcieServicesProtocolGuid,
                  NULL,
                  &PcieServicesProtocol
                  );

  if (Status == EFI_SUCCESS) {
    PcieServicesProtocol->PcieGetTopology (PcieServicesProtocol, (UINT32 **) &PciePlatformConfigHobData);
    Pcie = &(PciePlatformConfigHobData->PciePlatformConfigHob);
    GnbHandle = NbioGetHandle (Pcie);
    while (GnbHandle != NULL) {
      NbioBaseSetHwInitLock (GnbHandle);
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }

  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpNbioBaseHookPciIOExit, NULL);

  return;
}

/**
 *---------------------------------------------------------------------------------------
 *
 * NBIO Base driver entry.
 *
 *
 *
 * @param[in]     ImageHandle
 * @param[in]     *SystemTable
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
AmdNbioBaseDxeEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_HANDLE          Handle;
  EFI_EVENT           PciIoEvent;
  VOID                *Registration;

  AGESA_TESTPOINT (TpNbioBaseDxeEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a\n", __FUNCTION__);
  Handle = NULL;

  //
  // Register the event handling function
  //
  Status = gBS->CreateEventEx (
             EVT_NOTIFY_SIGNAL,
             TPL_NOTIFY,
             NbioBaseHookPciIO,
             NULL,
             NULL,
             &PciIoEvent
             );

  Status = gBS->RegisterProtocolNotify (
              &gEfiPciIoProtocolGuid,
              PciIoEvent,
              &Registration
              );

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a\n", __FUNCTION__);
  AGESA_TESTPOINT (TpNbioBaseDxeExit, NULL);
  return EFI_SUCCESS;
}


