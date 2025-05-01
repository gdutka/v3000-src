/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "Porting.h"
#include <Library/AmdBaseLib.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Arch.h>
#include <Library/ApcbChecksumLibV3.h>
#include <Library/ApcbLibV3.h>
#include <Filecode.h>


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_APCBCHECKSUMLIBV3_APCBCHECKSUMLIBV3_FILECODE

#define APCB_SIGNATURE  0x42435041ul


/**
  Calculate APCB checksum with standard algorithm

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      CheckSum        Point to checksum buffer

**/
STATIC
VOID
ApcbCalcCheckSumStandard (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN OUT   UINT8            *CheckSum
  )
{
  UINT8  CheckSumValue;
  UINT8  *DataPtr;

  ASSERT (NULL != ApcbPtr);

  CheckSumValue = 0;
  DataPtr = ApcbPtr;
  while (Length --) {
    CheckSumValue += *(DataPtr ++);
  }
  *CheckSum = CheckSumValue;
}

/**
  Check APCB checksum

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index

  @retval TRUE                   APCB checksum is valid
  @retval FALSE                  APCB checksum is not valid

**/
BOOLEAN
ApcbValidateCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  APCB_V3_HEADER    *Header;
  UINT8             StandardCheckSumData;

  Header = (APCB_V3_HEADER *)ApcbPtr;
  if ((Header == NULL) || (Header->Signature != APCB_SIGNATURE)) {
    return FALSE;
  }

  ApcbCalcCheckSumStandard (
            Header,
            Length,
            &StandardCheckSumData
            );
  if (StandardCheckSumData != 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}


BOOLEAN
IsApcbCheckSumAvailable (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  return TRUE;
}

/**
  Check APCB checksum availability

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index

  @retval TRUE                   There is APCB checksum
  @retval FALSE                  There is no APCB checksum

**/
BOOLEAN
ApcbUpdateCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  APCB_V3_HEADER     *Header;
  UINT8              StandardCheckSumData;

  Header = (APCB_V3_HEADER *)ApcbPtr;
  if ((Header == NULL) || (Header->Signature != APCB_SIGNATURE)) {
    return FALSE;
  }

  Header->CheckSumByte = 0;
  ApcbCalcCheckSumStandard (
            Header,
            Length,
            &StandardCheckSumData
            );
  Header->CheckSumByte = (~StandardCheckSumData) + 1;
  return TRUE;
}

