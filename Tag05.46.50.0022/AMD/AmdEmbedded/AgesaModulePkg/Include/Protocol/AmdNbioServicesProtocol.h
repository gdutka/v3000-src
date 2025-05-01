/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * NBIO Services Protocol prototype definition
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
#ifndef _NBIO_SERVICES_PROTOCOL_H_
#define _NBIO_SERVICES_PROTOCOL_H_

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
/** @brief SMU init complete protocol
 *  @details 
 */
typedef struct _DXE_AMD_NBIO_SMU_INIT_COMPLETE_PROTOCOL {
  UINT32    Revision;               /***< Revision Number*/
} DXE_AMD_NBIO_SMU_INIT_COMPLETE_PROTOCOL;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define AMD_NBIO_SERVICES_PROTOCOL_REVISION   0x00  /***< Current Protocol revision*/

extern EFI_GUID gAmdNbioSmuInitCompleteProtocolGuid;
#endif


