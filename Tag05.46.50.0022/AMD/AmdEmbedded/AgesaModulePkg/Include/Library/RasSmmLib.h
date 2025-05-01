/*
*****************************************************************************
*
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

#ifndef _RAS_SMM_LIB_H_
#define _RAS_SMM_LIB_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "PiDxe.h"
#include "PiSmm.h"
#include <Library/BaseLib.h>
#include "Library/SmmServicesTableLib.h"
//#include "AmdRasRegistersRs.h"
#include "AmdRas.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
LibRasSmmMemRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
     OUT   VOID                        *Value
  );

VOID
LibRasSmmIoRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT16                      Address,
     OUT   VOID                        *Value
  );

VOID
LibRasSmmIoWrite (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
  IN       VOID                        *Value
  );

VOID
LibRasSmmPciRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT32                      PciAddress,
     OUT   VOID                        *Value
  );

VOID
LibRasSmmPmioRead8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

VOID
LibRasSmmPmioWrite8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  );

EFI_STATUS
LibRasSmmMsrWrite (
  IN       RAS_MSR_ACCESS *RasMsrAccess,
  IN       UINTN          ProcessorNumber
  );

EFI_STATUS
LibRasSmmMsrRead (
  IN OUT   RAS_MSR_ACCESS *RasMsrAccess,
  IN       UINTN          ProcessorNumber
  );

EFI_STATUS
LibRasSmmMsrTblWrite (
  IN OUT   RAS_BSP_AP_MSR_SYNC *ApMsrSync,
  IN       UINTN               ProcessorNumber
  );

EFI_STATUS
LibRasSmmSyncMsr (
  IN OUT   RAS_BSP_AP_MSR_SYNC *ApMsrSync
  );

EFI_STATUS
LibRasSmmRunFunc (
  IN       EFI_AP_PROCEDURE RasMpProcedure,
  IN       VOID             *Buffer,
  IN       UINTN            ProcessorNumber
  );

EFI_STATUS
LibRasSmmRunFuncOnAllAPs (
  IN        EFI_AP_PROCEDURE RasMpProcedure,
  IN        VOID             *Buffer
  );

EFI_STATUS
LibRasSmmRunFuncOnBspAndAllAPs (
  IN        EFI_AP_PROCEDURE RasMpProcedure,
  IN        VOID             *Buffer
  );

VOID
EFIAPI
GetSmmSaveStateBase (
  UINT64 *SmmSaveStateBase
  );

VOID
EFIAPI
GetLocalSmiStatus (
  LOCAL_SMI_STATUS* LocalSmiStatus
  );

VOID
EFIAPI
McaCloakControl (
  IN       RAS_MSR_ACCESS *RasMsrAccess
  );

#endif //_RAS_RS_SMM_LIB_H_


