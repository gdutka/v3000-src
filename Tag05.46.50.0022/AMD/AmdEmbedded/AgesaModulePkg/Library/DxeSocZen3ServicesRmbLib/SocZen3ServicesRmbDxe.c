/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SocZen3ServicesProtocol.h>
#include <Protocol/AmdNbioSmuServicesProtocol.h>
#include <GnbDxio.h>
#include "SocZen3ServicesRmbDxe.h"
#include <Filecode.h>
#include <AGESA.h>

#define FILECODE LIBRARY_DXESOCZEN3SERVICESRMBLIB_SOCZEN3SERVICESRMBDXE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
GetNbioSmuServices (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  **NbioSmuServices
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

STATIC AMD_SOC_ZEN3_SERVICES_PROTOCOL  mSocZen3ServicesRmbProtocol = {
  SOC_ZEN3_SERVICES_PROTOCOL_REVISION,
  SocZen3RmbGetOpnFmax,
  SocZen3RmbGetOpnCorePresence,
  SocZen3RmbGetOcSmuMsgId
};

/* @todo: sync with SMU team
STATIC SOC_ZEN3_OC_MSG_ID_TABLE SocOcMsgIdTableSocRmb = {
  SOC_OC_MSG_ID_MAP_REVISION,                 ///< Revision Number
  BIOSSMC_MSG_EnableOverclocking,             ///< SMU service ID for enable overclocking
  BIOSSMC_MSG_SetOverclockVID,                ///< SMU service ID for set overclock VID
  BIOSSMC_MSG_SetOverclockFreqAllCores        ///< SMU service ID for set overclock frequency for all cores
};
*/

EFI_STATUS
EFIAPI
SocZen3ServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  // Install SoC Zen3 Services
  Status = gBS->InstallProtocolInterface (
                &ImageHandle,
                &gAmdSocZen3ServicesProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mSocZen3ServicesRmbProtocol
                );

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves the value for the OPN's maximum frequency indicator.
 *
 * @param[in]  This                           A pointer to the AMD_SOC_ZEN3_SERVICES_PROTOCOL instance.
 * @param[out] FMax                           Pointer to the raw FMax value.
 *
 * @retval EFI_SUCCESS                        FMax is valid.
 * @retval EFI_INVALID_PARAMETER              FMax is NULL.
 * @retval EFI_DEVICE_ERROR                   FMax value not found.
 *
 **/
EFI_STATUS
SocZen3RmbGetOpnFmax (
 IN        AMD_SOC_ZEN3_SERVICES_PROTOCOL  *This,
     OUT   UINT32                          *FMax
  )
{
  EFI_STATUS                           Status;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *NbioSmuServices;
  UINT32                               FmaxFuse;

  if (FMax == NULL) {
    Status = EFI_INVALID_PARAMETER;
  } else if (GetNbioSmuServices (&NbioSmuServices)) {
    NbioSmuServices->AmdSmuRegisterRead (NbioSmuServices, 0, 0x5D744, &FmaxFuse);
    *FMax = (FmaxFuse & 0xFF0) >> 4;
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This service retrieves information on the OPN's core / thread presence.
 *
 * @param[in]  This                           A pointer to the AMD_SOC_ZEN3_SERVICES_PROTOCOL instance.
 * @param[out] CcdMap                         Pointer to the OPN CCD map.
 * @param[out] CoreDisMap                     Pointer to the OPN disabled core map.
 * @param[out] SmtEnable                      Pointer to whether or not the OPN supports SMT.
 *
 * @retval EFI_SUCCESS                        Presence info was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              All output parameter pointers are NULL.
 * @retval EFI_DEVICE_ERROR                   Presence info not found.
 *
 **/
EFI_STATUS
SocZen3RmbGetOpnCorePresence (
  IN       AMD_SOC_ZEN3_SERVICES_PROTOCOL  *This,
     OUT   UINT32                          *CcdMap,
     OUT   UINT32                          *CoreDisMap,
     OUT   BOOLEAN                         *SmtEnable
  )
{
  EFI_STATUS                           Status;
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  *NbioSmuServices;
  UINT32                               CoreFuse;

  if ((CcdMap == NULL) && (CoreDisMap == NULL) && (SmtEnable == NULL)) {
    Status = EFI_INVALID_PARAMETER;
  } else if (GetNbioSmuServices (&NbioSmuServices)) {
    if (CcdMap != NULL) {
      *CcdMap = 0x00000001;
    }
    NbioSmuServices->AmdSmuRegisterRead (NbioSmuServices, 0, 0x5D4DC, &CoreFuse);
    if (SmtEnable != NULL) {
      *SmtEnable = (BOOLEAN) ((CoreFuse & BIT6) != 0);
    }
    if (CoreDisMap != NULL) {
      *CoreDisMap = (CoreFuse >> 7) & 0xFF;
    }
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
 * This service retrieves information on the SOC overclocking SMU message ID map
 *
 * @param[in]  This                           A pointer to the AMD_SOC_ZEN3_SERVICES_PROTOCOL instance.
 * @param[out] OcMsgIdTable                   Pointer to the OC message ID table.
 *
 * @retval EFI_SUCCESS                        OcMsgIdTable info was successfully retrieved.
 * @retval EFI_INVALID_PARAMETER              Input protocol instance does not match.
 * @retval EFI_UNSUPPORTED                    Unsupported service for this SOC
 *
 **/
EFI_STATUS
SocZen3RmbGetOcSmuMsgId (
     IN    AMD_SOC_ZEN3_SERVICES_PROTOCOL  *This,
     OUT   SOC_ZEN3_OC_MSG_ID_TABLE       **OcMsgIdTable
  )
{
/* @todo: sync with SMU team
  if (This == &mSocZen3ServicesRmbProtocol) {
    *OcMsgIdTable = &SocOcMsgIdTableSocRmb;
    return EFI_SUCCESS;
  } else {
    return EFI_INVALID_PARAMETER;
  }
*/
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
GetNbioSmuServices (
  IN       DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL  **NbioSmuServices
  )
{
  return (BOOLEAN) (gBS->LocateProtocol (&gAmdNbioSmuServicesProtocolGuid, NULL, NbioSmuServices) == EFI_SUCCESS);
}
