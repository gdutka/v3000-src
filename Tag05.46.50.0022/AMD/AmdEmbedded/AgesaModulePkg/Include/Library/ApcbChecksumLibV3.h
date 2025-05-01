/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * ApcbChecksumLibV3.h
 *
 * AGESA PSP Customization Block
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem)
 * @e \$Revision: 192403 $ @e \$Date: 2012-12-12 15:46:25 -0600 (Wed, 12 Dec 2012) $
 *
 **/

#ifndef _APCB_CHECKSUM_LIB_V3_H_
#define _APCB_CHECKSUM_LIB_V3_H_

#include "Porting.h"

#define HMAC_CHECKSUM_LENGTH     (32)

BOOLEAN
ApcbValidateCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
);

BOOLEAN
IsApcbCheckSumAvailable (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
);

BOOLEAN
ApcbUpdateCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
);


#endif



