/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AMD_PSP_ARS_SERVICE_PROTOCOL_H_
#define _AMD_PSP_ARS_SERVICE_PROTOCOL_H_

#include "AmdRas.h"

extern EFI_GUID gAmdPspArsServiceProtocolGuid;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

#define PSP_ARS_COMPLETE              0x00        ///< ARS Complete
#define PSP_ARS_ERROR_FOUND           0x01        ///< ARS Error Found


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
typedef struct _AMD_PSP_ARS_SERVICE_PROTOCOL AMD_PSP_ARS_SERVICE_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *PSP_ARS_STATUS_HANDLER) (
  IN       UINT64             ArsStatus
);

typedef
EFI_STATUS
(EFIAPI *MCA_ERROR_ARS_HANDLER) (
  IN       RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  IN       UINT8                 BankIndex
);

/// Protocol Structure
struct _AMD_PSP_ARS_SERVICE_PROTOCOL {
  PSP_ARS_STATUS_HANDLER           PspArsStatusHandler;      /// PSP NVDIMM ARS status handler
  MCA_ERROR_ARS_HANDLER            McaErrorArsHandler;       /// MCA Error NVDIMM ARS handler
};

#endif //_AMD_PSP_ARS_SERVICE_PROTOCOL_H_


