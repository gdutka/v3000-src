/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _RAS_OEM_DIMMMAP_PROTOCOL_H_
#define _RAS_OEM_DIMMMAP_PROTOCOL_H_

#include <AmdCpmRas.h>

///
/// Forward declaration for the DXE_RAS_OEM_DIMMMAP_PROTOCOL
///
typedef struct _DXE_RAS_OEM_DIMMMAP_PROTOCOL DXE_RAS_OEM_DIMMMAP_PROTOCOL;

///
/// This PPI provide interface to access Board ID information.
///
typedef struct _DXE_RAS_OEM_DIMMMAP_PROTOCOL {
  UINTN                     Revision;                   ///< Revision Number
  OEM_MEMORY_MAP_TABLE      *RasOemDimmMap;             ///< Platform memory DIMM locater map.
} DXE_RAS_OEM_DIMMMAP_PROTOCOL;

// Current PPI revision
#define RAS_OEM_DIMMMAP_PROTOCOL_REVISION   0x00

extern EFI_GUID gRasOemDimmMapProtocolGuid;

#endif // _AMDBOARDID_PROTOCOL_H_


