/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Memory API, and related functions.
 *
 * Contains code that initializes memory
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Mem
 * @e \$Revision:  $   @e \$Date:  $
 *
 */
#ifndef _AMD_MEM_PPR_PROTOCOL_H_
#define _AMD_MEM_PPR_PROTOCOL_H_

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/PcdLib.h>
#include <Guid/HobList.h>

#ifndef AMD_MEM_MAX_SOCKETS_SUPPORTED
 #define AMD_MEM_MAX_SOCKETS_SUPPORTED       FixedPcdGet8(PcdAmdMemMaxSocketSupportedV2)    ///< Max number of sockets in system
#endif

#ifndef AMD_MEM_MAX_DIES_PER_SOCKET
  #define AMD_MEM_MAX_DIES_PER_SOCKET        FixedPcdGet8(PcdAmdMemMaxDiePerSocketV2)    ///< Max dies per socket
#endif
#ifndef AMD_MEM_MAX_CHANNELS_PER_DIE
 #define AMD_MEM_MAX_CHANNELS_PER_DIE        FixedPcdGet8(PcdAmdMemMaxChannelPerDieV2)    ///< Max channels per die
#endif

#ifndef AMD_MEM_MAX_CHANNELS_PER_SOCKET
  #define AMD_MEM_MAX_CHANNELS_PER_SOCKET    AMD_MEM_MAX_CHANNELS_PER_DIE * AMD_MEM_MAX_DIES_PER_SOCKET    ///< Max Channels per sockets
#endif

#define AMD_MEM_MAX_DIMMS_PER_CHANNEL        FixedPcdGet8(PcdAmdMemMaxDimmPerChannelV2)   ///< Max dimms per channel



typedef struct _AMD_DIMM_INFO {
    UINT8  SocketId;
    UINT8  DieId;
    UINT8  ChannelId;
    UINT8  Chipselect;
} AMD_DIMM_INFO;

typedef struct _AMD_PPR_INFO {
    BOOLEAN IsValidRecord;
    BOOLEAN DpprSupported;
    BOOLEAN SpprSupported;
    UINT8   DeviceWidth;
    UINT32  SerialNumber;
} AMD_PPR_INFO;

//
// AMD_MEM_DIMM_SPD_DATA_STRUCT
//
typedef struct _AMD_MEM_PPR_DATA_TABLE {
  AMD_PPR_INFO    DimmSpdInfo[AMD_MEM_MAX_SOCKETS_SUPPORTED * AMD_MEM_MAX_CHANNELS_PER_SOCKET * AMD_MEM_MAX_DIMMS_PER_CHANNEL];
} AMD_MEM_PPR_DATA_TABLE;

//
// Forward declaration for the AMD_ACPI_CPU_SSDT_SERVICES_PROTOCOL.
//
typedef struct _AMD_POST_PACKAGE_REPAIR_INFO_PROTOCOL AMD_POST_PACKAGE_REPAIR_INFO_PROTOCOL;


typedef
EFI_STATUS
(EFIAPI *AMD_GET_PPR_INFO) (
  IN       AMD_POST_PACKAGE_REPAIR_INFO_PROTOCOL    *This,
  IN       AMD_DIMM_INFO                            *AmdDimmInfo,
     OUT   AMD_PPR_INFO                             *PprInfo
  );

//
// AMD Post Package Repair Protocol Info
//
struct _AMD_POST_PACKAGE_REPAIR_INFO_PROTOCOL {
  AMD_MEM_PPR_DATA_TABLE          AmdPprArray;
  AMD_GET_PPR_INFO                AmdGetPprInfo;
};

extern EFI_GUID  gAmdPostPackageRepairInfoProtocolGuid;

#endif

