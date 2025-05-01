/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CPM_RAS_INFO_HOB_H_
#define _AMD_CPM_RAS_INFO_HOB_H_

extern EFI_GUID gAmdCpmRasHobGuid;
#define CPM_MAX_NUMBER_DPCSTATUS      128

#pragma pack (push, 1)

/// PCIE_DPC_STATUS
typedef struct {
  UINT8                           SocketId;                       ///< Socket ID
  UINT8                           DieID;                          ///< Die ID
  UINT8                           RBIndex;                        ///< Node to which GNB connected
  UINT8                           BusNumber;                      ///< PCI Bus Number
  UINT8                           PCIeCoreID;                     ///< PCIe core ID
  UINT8                           PCIePortID;                     ///< PCIe port ID
  UINT16                          DpcStatus;                      ///< PCIe DPC status
} CPM_PCIe_DPC_STATUS_RECORD;

/// PCIE_DPC_STATUS
typedef struct {
  UINT8                           size;                                 ///< number of PCIe DPC status record
  CPM_PCIe_DPC_STATUS_RECORD      DpcStatusArray[CPM_MAX_NUMBER_DPCSTATUS]; ///< PCIe DPC status Array
} CPM_PCIe_DPC_STATUS_DATA;

/// Platform RAS info HOB
typedef struct {
  EFI_HOB_GUID_TYPE         EfiHobGuidType;             ///< GUID Hob type structure
  UINT32                    Version;                    ///< Version of HOB structure
  CPM_PCIe_DPC_STATUS_DATA  CpmPcieDpcStatusData;       ///< PCIe DPC Status Data
} AMD_CPM_RAS_INFO_HOB;

#pragma pack (pop)

#define AMD_CPM_RAS_INFO_HOB_VERISION        0x00000100ul  // Ver: 00.00.01.00

#endif // _AMD_CPM_RAS_INFO_HOB_H_


