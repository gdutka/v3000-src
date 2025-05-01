/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Fabric SOC specific services Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 */

#ifndef _FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL_H_
#define _FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL_H_
#include <AGESA.h>

#pragma pack (push, 1)

/*---------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *---------------------------------------------------------------------------------------
 */
#define AMD_FABRIC_SOC_SPECIFIC_PROTOCOL_REV  0x01
#define MAX_XGMI_LINKS_PER_SOCKET             4
#define MAX_XGMI_LINKS_SUPPORTED              MAX_SOCKETS_SUPPORTED*MAX_XGMI_LINKS_PER_SOCKET

/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
typedef struct _FABRIC_XGMI_FREQ_FOR_EACH_LINK {
  UINT32     XgmiFreq[MAX_XGMI_LINKS_SUPPORTED];
} FABRIC_XGMI_FREQ_FOR_EACH_LINK;

/*---------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *---------------------------------------------------------------------------------------
 */
typedef struct _FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL;

/// Function prototype
typedef EFI_STATUS (EFIAPI *FABRIC_DXE_GET_XGMI_FREQ) (
  IN       FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL    *This,                  ///< Protocol Instance
  IN OUT   FABRIC_XGMI_FREQ_FOR_EACH_LINK           *XgmiFreqForEachLink    ///< Get xGMI frequency for each link
);

/// Fabric Soc Specific Protocol Structure
struct _FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL {                             //See the Forward Declaration above
  UINTN                              Revision;                              ///< Revision
  FABRIC_DXE_GET_XGMI_FREQ           FabricGetXgmiFreq;                     ///< Pointer to function
};

extern EFI_GUID gAmdFabricSocSpecificServicesProtocolGuid;

#pragma pack (pop)
#endif // _FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL_H_


