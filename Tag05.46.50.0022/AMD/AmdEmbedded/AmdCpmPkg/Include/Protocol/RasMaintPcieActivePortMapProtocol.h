/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _RAS_MAINT_PCIEACTIVEPORTMAP_PROTOCOL_H_
#define _RAS_MAINT_PCIEACTIVEPORTMAP_PROTOCOL_H_

#include <AmdCpmRas.h>

extern EFI_GUID gRasMaintPcieActivePortMapProtocolGuid;

typedef
EFI_STATUS
(EFIAPI *REBUILD_BY_ROOTPORT)(
  IN  UINT32    RpPciAddr
  );

typedef struct _RAS_MAINT_PCIEACTIVEPORTMAP_PROTOCOL {
  REBUILD_BY_ROOTPORT    RebuildByRootPort;
} RAS_MAINT_PCIEACTIVEPORTMAP_PROTOCOL;

#endif // _RAS_MAINT_PCIEACTIVEPORTMAP_PROTOCOL_H_


