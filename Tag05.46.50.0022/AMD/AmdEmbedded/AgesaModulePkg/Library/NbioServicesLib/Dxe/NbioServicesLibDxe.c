/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Filecode.h>
#include <AGESA.h>
#include <Library/IdsLib.h>
#include <Protocol/AmdNbioServices.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseFabricTopologyLib.h>

#define FILECODE LIBRARY_NBIOSERVICESLIB_DXE_NBIOSERVICESLIBDXE_FILECODE

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

EFI_STATUS
NbioServiceGetSystemInfo (
  IN    AMD_NBIO_SERVICES_PROTOCOL            *This,
  OUT   UINTN                                 *NumberOfInstalledProcessors,
  OUT   UINTN                                 *TotalNumberOfDie,
  OUT   UINTN                                 *TotalNumberOfRootBridges
)
{
  if ((NumberOfInstalledProcessors == NULL) && (TotalNumberOfDie == NULL) && (TotalNumberOfRootBridges == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfInstalledProcessors != NULL) {
    *NumberOfInstalledProcessors = FabricTopologyGetNumberOfProcessorsPresent ();
  }

  if (TotalNumberOfDie != NULL) {
    *TotalNumberOfDie = FabricTopologyGetNumberOfSystemDies ();
  }

  if (TotalNumberOfRootBridges != NULL) {
    *TotalNumberOfRootBridges = FabricTopologyGetNumberOfSystemRootBridges ();
  }

  return EFI_SUCCESS;
}

EFI_STATUS
NbioServiceGetProcessorInfo (
  IN    AMD_NBIO_SERVICES_PROTOCOL            *This,
  IN    UINTN                                 Socket,
  OUT   UINTN                                 *NumberOfDie,
  OUT   UINTN                                 *NumberOfRootBridges
)
{
  if ((NumberOfDie == NULL) && (NumberOfRootBridges == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Socket >= FabricTopologyGetNumberOfProcessorsPresent ()) {
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfDie != NULL) {
    *NumberOfDie = FabricTopologyGetNumberOfDiesOnSocket (Socket);
  }

  if (NumberOfRootBridges != NULL) {
    *NumberOfRootBridges = FabricTopologyGetNumberOfRootBridgesOnSocket (Socket);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
NbioServiceGetRootBridgeInfo (
  IN    AMD_NBIO_SERVICES_PROTOCOL            *This,
  IN    UINTN                                 Socket,
  IN    UINTN                                 Die,
  IN    UINTN                                 Index,
  OUT   UINTN                                 *NbioPhysicalID,
  OUT   UINTN                                 *NbioLogicalID,
  OUT   UINTN                                 *BusNumberBase,
  OUT   UINTN                                 *BusNumberLimit,
  OUT   UINTN                                 *PhysicalRootBridgeNumber
)
{
  if ((NbioPhysicalID == NULL) && (NbioLogicalID == NULL) && (BusNumberBase == NULL) && (BusNumberLimit == NULL) &&
      (PhysicalRootBridgeNumber == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Socket >= FabricTopologyGetNumberOfProcessorsPresent ()) {
    return EFI_INVALID_PARAMETER;
  }

  if (Die >= FabricTopologyGetNumberOfDiesOnSocket (Socket)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Index >= FabricTopologyGetNumberOfRootBridgesOnDie (Socket, Die)) {
    return EFI_INVALID_PARAMETER;
  }

  if (NbioPhysicalID != NULL) {
    *NbioPhysicalID = FabricTopologyGetPhysRootBridgeNumber (Socket, Die, Index);
  }

  if (NbioLogicalID != NULL) {
    *NbioLogicalID = Index;
  }

  if (BusNumberBase != NULL) {
    *BusNumberBase = FabricTopologyGetHostBridgeBusBase (Socket, Die, Index);
  }

  if (BusNumberLimit != NULL) {
    *BusNumberLimit = FabricTopologyGetHostBridgeBusLimit (Socket, Die, Index);
  }

  if (PhysicalRootBridgeNumber != NULL) {
    *PhysicalRootBridgeNumber = FabricTopologyGetPhysRootBridgeNumber (Socket, Die, Index);
  }

  return EFI_SUCCESS;
}

STATIC AMD_NBIO_SERVICES_PROTOCOL  mAmdNbioServicesProtocol = {
  NbioServiceGetSystemInfo,
  NbioServiceGetProcessorInfo,
  NbioServiceGetRootBridgeInfo
};

EFI_STATUS
EFIAPI
NbioServiceProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;
  Handle = NULL;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdNbioServicesProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAmdNbioServicesProtocol
                  );

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);

  return Status;
}
