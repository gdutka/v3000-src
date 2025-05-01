/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Nbio IOMMU Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Nbio
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#ifndef _NBIO_IOMMU_PROTOCOL_H_
#define _NBIO_IOMMU_PROTOCOL_H_

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
/// Forward declaration for the AMD_NBIO_IOMMU_PROTOCOL
typedef struct _AMD_NBIO_IOMMU_PROTOCOL AMD_NBIO_IOMMU_PROTOCOL;

/// @todo Style_Analyzer: Add Doxygen comment to enum construct
typedef enum {
  SkipIommuWorkaround = 0x0000000000000001ul  ///< BIT0 - Skip Iommu Workaround
} NBIO_IOMMU_STATE;

//
// Protocol Definitions
//
struct _AMD_NBIO_IOMMU_PROTOCOL {
  UINT64 IommuState; ///< flag
};

extern EFI_GUID gAmdNbioIommuProtocolGuid;          ///< protocol call

#endif


