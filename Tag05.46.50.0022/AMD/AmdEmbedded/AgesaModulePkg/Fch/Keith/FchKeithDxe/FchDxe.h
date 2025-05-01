/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_DXE_H_
#define _FCH_DXE_H_

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#include "FchPlatform.h"
#include <Library/FchInitHookLib.h>
#include <Library/FchDxeLib.h>

#include <IdsHookId.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/FabricResourceManagerLib.h>

#include <Protocol/FchInitProtocol.h>
#include <Protocol/FchInitDonePolicyProtocol.h>

//
// Module data structure
//
/// Private data and access defines
typedef struct _FCH_DXE_PRIVATE {
  UINTN                            Signature;           ///< Signature
  FCH_INIT_PROTOCOL                FchInit;              ///< Protocol data
  EFI_EVENT                        EventAfterPciInit;   ///< Event related data
  EFI_EVENT                        EventReadyToBoot;    ///< Event related data
} FCH_DXE_PRIVATE;

#define FCH_DXE_PRIVATE_DATA_SIGNATURE   SIGNATURE_32 ('S', 'B', 'i', 'D')

#define FCH_DXE_PRIVATE_INSTANCE_FROM_PPI_THIS(a) \
  CR (a, FCH_PEI_PRIVATE, FchInit, FCH_DXE_PRIVATE_DATA_SIGNATURE)


//
// Functions Prototypes
//
EFI_STATUS
EFIAPI
FchDxeInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );

EFI_STATUS
EFIAPI
FchUsbDisablePort (
  IN       CONST FCH_INIT_PROTOCOL   *This,
  IN       UINT8                     Socket,
  IN       UINT32                    USB3DisableMap,
  IN       UINT32                    USB2DisableMap
  );

#endif // _FCH_DXE_H_

