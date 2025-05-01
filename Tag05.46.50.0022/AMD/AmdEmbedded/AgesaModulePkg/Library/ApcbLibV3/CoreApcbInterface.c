/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * core APCB interface
 *
 * Contains core interface to manipulate APCB V3
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Porting.h"
// #include "Addendum/Apcb/Inc/SSP/APCB.h"
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Arch.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbDataGroups.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Token.h>
#include "CoreApcbInterface.h"
#include "CalloutLib.h"
#include <Filecode.h>

#define FILECODE LIBRARY_APCBLIBV3_COREAPCBINTERFACE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          G L O B A L        V A L U E S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves the APCB structure/data versions
 *
 * @param[in]         apcbBuf              - Pointer to the APCB binary
 * @param[out]        StructVersion        - APCB Struct version
 * @param[out]        DataVersion          - APCB Data version
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_INVALID
 *
 */
CORE_APCB_RET_CODE
coreApcbGetVersion (
  IN         UINT8        *apcbBuf,
      OUT    UINT16       *StructVersion,
      OUT    UINT16       *DataVersion
  )
{
  APCB_V3_HEADER        *apcbHeader;

  customPrint ("\t[CORE APCB INFO] coreApcbGetVersion Entry\n");

  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  *StructVersion        = apcbHeader->StructVersion;
  *DataVersion          = apcbHeader->DataVersion;

  customPrint ("\t[CORE APCB INFO] APCB Struct Version = %04X\n", *StructVersion);
  customPrint ("\t[CORE APCB INFO] APCB Data Version = %04X\n", *DataVersion);

  return CORE_APCB_OK;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function calculates the checksum of the APCB binary instance
 *
 * @param[in,out]     apcbBuf         - Pointer to the APCB binary
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_INVALID
 *
 */
CORE_APCB_RET_CODE
coreChecksumApcb (
  IN  OUT    UINT8        *apcbBuf
  )
{
  UINT8                 checksum;
  UINT8                 *dataPtr;
  APCB_V3_HEADER        *apcbHeader;
  UINT32                length;

  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;

  if (NULL == apcbBuf || APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  apcbHeader->CheckSumByte = 0;
  checksum  = 0;
  dataPtr   = apcbBuf;
  length    = apcbHeader->SizeOfApcb;
  while (length --) {
    checksum += *(dataPtr ++);
  }
  apcbHeader->CheckSumByte = (~checksum) + 1;

  return CORE_APCB_OK;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a boolean APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         apcbToken       - APCB token ID
 * @param[out]        bValue          - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_TOKEN_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbGetBool (
  IN         UINT8        *apcbBuf,
  IN         UINT8        priorityMask,
  IN         UINT16       boardMask,
  IN         UINT8        typeInstance,
  IN         UINT32       apcbToken,
      OUT    BOOLEAN      *bValue
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;

  customPrint ("\t[CORE APCB INFO] coreApcbGetBool Entry\n");

  if (CORE_APCB_OK == coreApcbGetType (apcbBuf, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_BOOLEAN, priorityMask, boardMask, typeInstance, &dataBuf, &dataSize)) {
    for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
      if (tempPair->token == apcbToken) {
        *bValue = tempPair->value.bValue;
        customPrint ("\t[CORE APCB INFO] APCB token found: [%08X] = %08X\n", apcbToken, *bValue);
        return CORE_APCB_OK;
      }
    }
  }

  customPrint ("\t[CORE APCB ERR] APCB token not found\n");
  return CORE_APCB_ERR_TOKEN_NOT_FOUND;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a boolean APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         sizeApcbBuf     - Size of the input APCB buffer. The buffer has to be
 *                                      big enough for a new token/value pair to be added.
 * @param[in]         apcbToken       - APCB token ID
 * @param[in]         bValue          - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_SPACE_NOT_ENOUGH
 *                    CORE_APCB_ERR_TYPE_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbSetBool (
  IN  OUT     UINT8       *apcbBuf,
  IN          UINT32      sizeApcbBuf,
  IN          UINT8       priorityMask,
  IN          UINT16      boardMask,
  IN          UINT8       typeInstance,
  IN          UINT32      apcbToken,
  IN          BOOLEAN     bValue
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  APCB_TOKEN_PAIR       *apcbInsertPair;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;
  UINT32                *ApcbTail;
  UINT32                ApcbTailSizeInDword;
  UINT32                i;
  BOOLEAN               typeFound;
  CORE_APCB_RET_CODE    retCode;

  customPrint ("\t[CORE APCB INFO] coreApcbSetBool Entry\n");

  if (NULL == apcbBuf) {
    customPrint ("\t[CORE APCB ERR]NULL  APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  typeFound         = FALSE;
  apcbTempBuf       = apcbBuf;
  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding        = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader   = (APCB_GROUP_HEADER *)&apcbHeader[1];

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB Signature\n");
    return CORE_APCB_ERR_INVALID;
  }

  while ((UINT8 *)apcbGroupHeader < apcbEnding) {
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
      if (APCB_GROUP_TOKEN == apcbTypeHeader->GroupId &&
          APCB_TYPE_TOKEN_BOOLEAN == apcbTypeHeader->TypeId &&
          (priorityMask & apcbTypeHeader->sApcbTypeExtV3.PriorityMask) != 0 &&
          ((boardMask & (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask)) != 0 || (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask + 1) == 0)) {
        typeFound        = TRUE;

        dataBuf          = (UINT8 *)&apcbTypeHeader[1];
        apcbInsertPair   = (APCB_TOKEN_PAIR *)dataBuf;
        dataSize         = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
        for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
          if (tempPair->token == apcbToken) {
            tempPair->value.bValue = bValue;
            customPrint ("\t[CORE APCB INFO] APCB token set: [%08X] = %08X\n", apcbToken, bValue);
            return coreChecksumApcb (apcbBuf);
          } else if (tempPair->token < apcbToken) {
            apcbInsertPair = &tempPair[1];
          }
        }

        if (sizeApcbBuf < apcbHeader->SizeOfApcb + sizeof (APCB_TOKEN_PAIR)) {
          customPrint ("\t[CORE APCB ERR] Not enough space to add a token\n");
          return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
        }

        ApcbTailSizeInDword     = ((UINT32) (UINTN) (apcbEnding - (UINT8 *)apcbInsertPair)) / sizeof (UINT32);
        ApcbTail                = (UINT32 *)apcbInsertPair;
        for (i = 0; i < ApcbTailSizeInDword; i ++) {
          ApcbTail[ApcbTailSizeInDword - i + 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
        }
        apcbInsertPair->token           = apcbToken;
        apcbInsertPair->value.value32   = (UINT32)bValue;
        apcbTypeHeader->SizeOfType      += sizeof (APCB_TOKEN_PAIR);
        apcbGroupHeader->SizeOfGroup    += sizeof (APCB_TOKEN_PAIR);
        apcbHeader->SizeOfApcb += sizeof (APCB_TOKEN_PAIR);

        customPrint ("\t[CORE APCB INFO] APCB token appended: [%08X] = %08X\n", apcbToken, bValue);
        return coreChecksumApcb (apcbBuf);
      } else {
        apcbTempBuf       = (UINT8 *)apcbTypeHeader;
        apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbTempBuf[apcbTypeHeader->SizeOfType];
      }
    }
    apcbGroupHeader = (APCB_GROUP_HEADER *)apcbTypeHeader;
  }

  if (!typeFound) {
    customPrint ("\t[CORE APCB INFO] Attempting to create a new type instance\n");
    retCode = coreApcbSetType ( apcbBuf,
                                sizeApcbBuf,
                                APCB_GROUP_TOKEN,
                                APCB_TYPE_TOKEN_BOOLEAN,
                                priorityMask,
                                boardMask,
                                0,
                                NULL,
                                0
                              );
    if (CORE_APCB_OK != retCode) {
      return retCode;
    }
    return coreApcbSetBool ( apcbBuf,
                             sizeApcbBuf,
                             priorityMask,
                             boardMask,
                             typeInstance,
                             apcbToken,
                             bValue
                        );
  } else {
    customPrint ("\t[CORE APCB ERR] Should never reach here\n");
    return CORE_APCB_ERR_TYPE_NOT_FOUND;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a UINT8 APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         apcbToken       - APCB token ID
 * @param[out]        Value8          - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_TOKEN_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbGet8 (
  IN         UINT8        *apcbBuf,
  IN         UINT8        priorityMask,
  IN         UINT16       boardMask,
  IN         UINT8        typeInstance,
  IN         UINT32       apcbToken,
      OUT    UINT8        *value8
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;

  customPrint ("\t[CORE APCB INFO] coreApcbGet8 Entry\n");

  if (CORE_APCB_OK == coreApcbGetType (apcbBuf, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_1BYTE, priorityMask, boardMask, typeInstance, &dataBuf, &dataSize)) {
    for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
      if (tempPair->token == apcbToken) {
        *value8 = tempPair->value.value8;
        customPrint ("\t[CORE APCB INFO] APCB token found: [%08X] = %08X\n", apcbToken, *value8);
        return CORE_APCB_OK;
      }
    }
  }

  customPrint ("\t[CORE APCB ERR] APCB token not found\n");
  return CORE_APCB_ERR_TOKEN_NOT_FOUND;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a UINT8 APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         sizeApcbBuf     - Size of the input APCB buffer. The buffer has to be
 *                                      big enough for a new token/value pair to be added.
 * @param[in]         apcbToken       - APCB token ID
 * @param[in]         value8          - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_SPACE_NOT_ENOUGH
 *                    CORE_APCB_ERR_TYPE_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbSet8 (
  IN  OUT    UINT8        *apcbBuf,
  IN  OUT    UINT32       sizeApcbBuf,
  IN         UINT8        priorityMask,
  IN         UINT16       boardMask,
  IN         UINT8        typeInstance,
  IN         UINT32       apcbToken,
  IN         UINT8        value8
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  APCB_TOKEN_PAIR       *apcbInsertPair;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;
  UINT32                *ApcbTail;
  UINT32                ApcbTailSizeInDword;
  UINT32                i;
  BOOLEAN               typeFound;
  CORE_APCB_RET_CODE    retCode;

  customPrint ("\t[CORE APCB INFO] coreApcbSet8 Entry\n");

  if (NULL == apcbBuf) {
    customPrint ("\t[CORE APCB ERR] NULL APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  typeFound         = FALSE;
  apcbTempBuf       = apcbBuf;
  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding        = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader   = (APCB_GROUP_HEADER *)&apcbHeader[1];

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB Signature\n");
    return CORE_APCB_ERR_INVALID;
  }

  while ((UINT8 *)apcbGroupHeader < apcbEnding) {
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
      if (APCB_GROUP_TOKEN == apcbTypeHeader->GroupId &&
          APCB_TYPE_TOKEN_1BYTE == apcbTypeHeader->TypeId &&
          (priorityMask & apcbTypeHeader->sApcbTypeExtV3.PriorityMask) != 0 &&
          ((boardMask & (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask)) != 0 || (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask + 1) == 0)) {
        typeFound        = TRUE;

        dataBuf          = (UINT8 *)&apcbTypeHeader[1];
        apcbInsertPair   = (APCB_TOKEN_PAIR *)dataBuf;
        dataSize         = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
        for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
          if (tempPair->token == apcbToken) {
            tempPair->value.value8 = value8;
            customPrint ("\t[CORE APCB INFO] APCB token set: [%08X] = %08X\n", apcbToken, value8);
            return coreChecksumApcb (apcbBuf);
          } else if (tempPair->token < apcbToken) {
            apcbInsertPair = &tempPair[1];
          }
        }

        if (sizeApcbBuf < apcbHeader->SizeOfApcb + sizeof (APCB_TOKEN_PAIR)) {
          customPrint ("\t[CORE APCB ERR] Not enough space to add a token\n");
          return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
        }

        ApcbTailSizeInDword     = ((UINT32) (UINTN) (apcbEnding - (UINT8 *)apcbInsertPair)) / sizeof (UINT32);
        ApcbTail                = (UINT32 *)apcbInsertPair;
        for (i = 0; i < ApcbTailSizeInDword; i ++) {
          ApcbTail[ApcbTailSizeInDword - i + 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
        }
        apcbInsertPair->token           = apcbToken;
        apcbInsertPair->value.value32   = (UINT32)value8;
        apcbTypeHeader->SizeOfType      += sizeof (APCB_TOKEN_PAIR);
        apcbGroupHeader->SizeOfGroup    += sizeof (APCB_TOKEN_PAIR);
        apcbHeader->SizeOfApcb += sizeof (APCB_TOKEN_PAIR);

        customPrint ("\t[CORE APCB INFO] APCB token appended: [%08X] = %08X\n", apcbToken, value8);
        return coreChecksumApcb (apcbBuf);
      } else {
        apcbTempBuf       = (UINT8 *)apcbTypeHeader;
        apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbTempBuf[apcbTypeHeader->SizeOfType];
      }
    }
    apcbGroupHeader = (APCB_GROUP_HEADER *)apcbTypeHeader;
  }

  if (!typeFound) {
    customPrint ("\t[CORE APCB INFO] Attempting to create a new type instance\n");
    retCode = coreApcbSetType ( apcbBuf,
                                sizeApcbBuf,
                                APCB_GROUP_TOKEN,
                                APCB_TYPE_TOKEN_1BYTE,
                                priorityMask,
                                boardMask,
                                0,
                                NULL,
                                0
                              );
    if (CORE_APCB_OK != retCode) {
      return retCode;
    }
    return coreApcbSet8 ( apcbBuf,
                          sizeApcbBuf,
                          priorityMask,
                          boardMask,
                          typeInstance,
                          apcbToken,
                          value8
                        );
  } else {
    customPrint ("\t[CORE APCB ERR] Should never reach here\n");
    return CORE_APCB_ERR_TYPE_NOT_FOUND;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a UINT16 APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         apcbToken       - APCB token ID
 * @param[out]        value16         - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_TOKEN_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbGet16 (
  IN         UINT8        *apcbBuf,
  IN         UINT8        priorityMask,
  IN         UINT16       boardMask,
  IN         UINT8        typeInstance,
  IN         UINT32       apcbToken,
      OUT      UINT16     *value16
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;

  customPrint ("\t[CORE APCB INFO] coreApcbGet16 Entry\n");

  if (CORE_APCB_OK == coreApcbGetType (apcbBuf, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_2BYTES, priorityMask, boardMask, typeInstance, &dataBuf, &dataSize)) {
    for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
      if (tempPair->token == apcbToken) {
        *value16 = tempPair->value.value16;
        customPrint ("\t[CORE APCB INFO] APCB token found: [%08X] = %08X\n", apcbToken, *value16);
        return CORE_APCB_OK;
      }
    }
  }

  customPrint ("\t[CORE APCB ERR] APCB token not found\n");
  return CORE_APCB_ERR_TOKEN_NOT_FOUND;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a UINT16 APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         sizeApcbBuf     - Size of the input APCB buffer. The buffer has to be
 *                                      big enough for a new token/value pair to be added.
 * @param[in]         apcbToken       - APCB token ID
 * @param[in]         value16         - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_SPACE_NOT_ENOUGH
 *                    CORE_APCB_ERR_TYPE_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbSet16 (
  IN  OUT    UINT8        *apcbBuf,
  IN  OUT    UINT32       sizeApcbBuf,
  IN         UINT8        priorityMask,
  IN         UINT16       boardMask,
  IN         UINT8        typeInstance,
  IN         UINT32       apcbToken,
  IN         UINT16       value16
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  APCB_TOKEN_PAIR       *apcbInsertPair;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;
  UINT32                *ApcbTail;
  UINT32                ApcbTailSizeInDword;
  UINT32                i;
  BOOLEAN               typeFound;
  CORE_APCB_RET_CODE    retCode;

  customPrint ("\t[CORE APCB INFO] coreApcbSet16 Entry\n");
  if (NULL == apcbBuf) {
    customPrint ("\t[CORE APCB ERR] NULL APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  typeFound         = FALSE;
  apcbTempBuf       = apcbBuf;
  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding        = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader   = (APCB_GROUP_HEADER *)&apcbHeader[1];

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB Signature\n");
    return CORE_APCB_ERR_INVALID;
  }

  while ((UINT8 *)apcbGroupHeader < apcbEnding) {
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
      if (APCB_GROUP_TOKEN == apcbTypeHeader->GroupId &&
          APCB_TYPE_TOKEN_2BYTES == apcbTypeHeader->TypeId &&
          (priorityMask & apcbTypeHeader->sApcbTypeExtV3.PriorityMask) != 0 &&
          ((boardMask & (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask)) != 0 || (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask + 1) == 0)) {
        typeFound        = TRUE;

        dataBuf          = (UINT8 *)&apcbTypeHeader[1];
        apcbInsertPair   = (APCB_TOKEN_PAIR *)dataBuf;
        dataSize         = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
        for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
          if (tempPair->token == apcbToken) {
            tempPair->value.value16 = value16;
            customPrint ("\t[CORE APCB INFO] APCB token set: [%08X] = %08X\n", apcbToken, value16);
            return coreChecksumApcb (apcbBuf);
          } else if (tempPair->token < apcbToken) {
            apcbInsertPair = &tempPair[1];
          }
        }

        if (sizeApcbBuf < apcbHeader->SizeOfApcb + sizeof (APCB_TOKEN_PAIR)) {
          customPrint ("\t[CORE APCB ERR] Not enough space to add a token\n");
          return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
        }

        ApcbTailSizeInDword     = ((UINT32) (UINTN) (apcbEnding - (UINT8 *)apcbInsertPair)) / sizeof (UINT32);
        ApcbTail                = (UINT32 *)apcbInsertPair;
        for (i = 0; i < ApcbTailSizeInDword; i ++) {
          ApcbTail[ApcbTailSizeInDword - i + 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
        }
        apcbInsertPair->token           = apcbToken;
        apcbInsertPair->value.value32   = (UINT32)value16;
        apcbTypeHeader->SizeOfType      += sizeof (APCB_TOKEN_PAIR);
        apcbGroupHeader->SizeOfGroup    += sizeof (APCB_TOKEN_PAIR);
        apcbHeader->SizeOfApcb += sizeof (APCB_TOKEN_PAIR);

        customPrint ("\t[CORE APCB INFO] APCB token appended: [%08X] = %08X\n", apcbToken, value16);
        return coreChecksumApcb (apcbBuf);
      } else {
        apcbTempBuf       = (UINT8 *)apcbTypeHeader;
        apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbTempBuf[apcbTypeHeader->SizeOfType];
      }
    }
    apcbGroupHeader = (APCB_GROUP_HEADER *)apcbTypeHeader;
  }

  if (!typeFound) {
    customPrint ("\t[CORE APCB INFO] Attempting to create a new type instance\n");
    retCode = coreApcbSetType ( apcbBuf,
                                sizeApcbBuf,
                                APCB_GROUP_TOKEN,
                                APCB_TYPE_TOKEN_2BYTES,
                                priorityMask,
                                boardMask,
                                0,
                                NULL,
                                0
                              );
    if (CORE_APCB_OK != retCode) {
      return retCode;
    }
    return coreApcbSet16 ( apcbBuf,
                           sizeApcbBuf,
                           priorityMask,
                           boardMask,
                           typeInstance,
                           apcbToken,
                           value16
                        );
  } else {
    customPrint ("\t[CORE APCB ERR] Should never reach here\n");
    return CORE_APCB_ERR_TYPE_NOT_FOUND;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a UINT32 APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         apcbToken       - APCB token ID
 * @param[out]        value32         - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_TOKEN_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbGet32 (
  IN          UINT8       *apcbBuf,
  IN          UINT8       priorityMask,
  IN          UINT16      boardMask,
  IN          UINT8       typeInstance,
  IN          UINT32      apcbToken,
      OUT     UINT32      *value32
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;

  customPrint ("\t[CORE APCB INFO] coreApcbGet32 Entry\n");

  if (CORE_APCB_OK == coreApcbGetType (apcbBuf, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_4BYTES, priorityMask, boardMask, typeInstance, &dataBuf, &dataSize)) {
    for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
      if (tempPair->token == apcbToken) {
        *value32 = tempPair->value.value32;
        customPrint ("\t[CORE APCB INFO] APCB token found: [%08X] = %08X\n", apcbToken, *value32);
        return CORE_APCB_OK;
      }
    }
  }

  customPrint ("\t[CORE APCB ERR] APCB token not found\n");
  return CORE_APCB_ERR_TOKEN_NOT_FOUND;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a UINT32 APCB token
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         typeInstance    - Type Instance
 * @param[in]         sizeApcbBuf     - Size of the input APCB buffer. The buffer has to be
 *                                      big enough for a new token/value pair to be added.
 * @param[in]         apcbToken       - APCB token ID
 * @param[in]         value16         - APCB token value
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_SPACE_NOT_ENOUGH
 *                    CORE_APCB_ERR_TYPE_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbSet32 (
  IN  OUT     UINT8       *apcbBuf,
  IN  OUT     UINT32      sizeApcbBuf,
  IN          UINT8       priorityMask,
  IN          UINT16      boardMask,
  IN          UINT8       typeInstance,
  IN          UINT32      apcbToken,
  IN          UINT32      value32
    )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  APCB_TOKEN_PAIR       *tempPair;
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  APCB_TOKEN_PAIR       *apcbInsertPair;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;
  UINT32                *ApcbTail;
  UINT32                ApcbTailSizeInDword;
  UINT32                i;
  BOOLEAN               typeFound;
  CORE_APCB_RET_CODE    retCode;

  customPrint ("\t[CORE APCB INFO] coreApcbSet32 Entry\n");

  if (NULL == apcbBuf) {
    customPrint ("\t[CORE APCB ERR] NULL APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  typeFound         = FALSE;
  apcbTempBuf       = apcbBuf;
  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding        = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader   = (APCB_GROUP_HEADER *)&apcbHeader[1];

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB Signature\n");
    return CORE_APCB_ERR_INVALID;
  }

  while ((UINT8 *)apcbGroupHeader < apcbEnding) {
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
      if (APCB_GROUP_TOKEN == apcbTypeHeader->GroupId &&
          APCB_TYPE_TOKEN_4BYTES == apcbTypeHeader->TypeId &&
          (priorityMask & apcbTypeHeader->sApcbTypeExtV3.PriorityMask) != 0 &&
          ((boardMask & (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask)) != 0 || (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask + 1) == 0)) {
        typeFound        = TRUE;

        dataBuf          = (UINT8 *)&apcbTypeHeader[1];
        apcbInsertPair   = (APCB_TOKEN_PAIR *)dataBuf;
        dataSize         = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
        for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
          if (tempPair->token == apcbToken) {
            tempPair->value.value32 = value32;
            customPrint ("\t[CORE APCB INFO] APCB token set: [%08X] = %08X\n", apcbToken, value32);
            return coreChecksumApcb (apcbBuf);
          } else if (tempPair->token < apcbToken) {
            apcbInsertPair = &tempPair[1];
          }
        }

        if (sizeApcbBuf < apcbHeader->SizeOfApcb + sizeof (APCB_TOKEN_PAIR)) {
          customPrint ("\t[CORE APCB ERR] Not enough space to add a token\n");
          return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
        }

        ApcbTailSizeInDword     = ((UINT32) (UINTN) (apcbEnding - (UINT8 *)apcbInsertPair)) / sizeof (UINT32);
        ApcbTail                = (UINT32 *)apcbInsertPair;
        for (i = 0; i < ApcbTailSizeInDword; i ++) {
          ApcbTail[ApcbTailSizeInDword - i + 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
        }
        apcbInsertPair->token           = apcbToken;
        apcbInsertPair->value.value32   = value32;
        apcbTypeHeader->SizeOfType      += sizeof (APCB_TOKEN_PAIR);
        apcbGroupHeader->SizeOfGroup    += sizeof (APCB_TOKEN_PAIR);
        apcbHeader->SizeOfApcb += sizeof (APCB_TOKEN_PAIR);

        customPrint ("\t[CORE APCB INFO] APCB token appended: [%08X] = %08X\n", apcbToken, value32);
        return coreChecksumApcb (apcbBuf);
      } else {
        apcbTempBuf       = (UINT8 *)apcbTypeHeader;
        apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbTempBuf[apcbTypeHeader->SizeOfType];
      }
    }
    apcbGroupHeader = (APCB_GROUP_HEADER *)apcbTypeHeader;
  }

  if (!typeFound) {
    customPrint ("\t[CORE APCB INFO] Attempting to create a new type instance\n");
    retCode = coreApcbSetType ( apcbBuf,
                                sizeApcbBuf,
                                APCB_GROUP_TOKEN,
                                APCB_TYPE_TOKEN_4BYTES,
                                priorityMask,
                                boardMask,
                                0,
                                NULL,
                                0
                              );
    if (CORE_APCB_OK != retCode) {
      return retCode;
    }
    return coreApcbSet32 ( apcbBuf,
                           sizeApcbBuf,
                           priorityMask,
                           boardMask,
                           typeInstance,
                           apcbToken,
                           value32
                        );
  } else {
    customPrint ("\t[CORE APCB ERR] Should never reach here\n");
    return CORE_APCB_ERR_TYPE_NOT_FOUND;
  }
}

VOID
coreApcbFreeTypeChain (
  IN          CORE_APCB_TYPE_ENTRY  *coreApcbType
  )
{
  if (NULL != coreApcbType) {
    if (NULL != coreApcbType->next) {
      coreApcbFreeTypeChain (coreApcbType->next);
    } else {
      customFree (coreApcbType);
    }
  }
}

VOID
coreApcbFreeTokenChain (
  IN          CORE_APCB_TOKEN_ENTRY  *coreApcbToken
  )
{
  if (NULL != coreApcbToken) {
    if (NULL != coreApcbToken->next) {
      coreApcbFreeTokenChain (coreApcbToken->next);
    }
    customFree (coreApcbToken);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves the first type of APCB
 *
 * @param[in]         apcbBuf    - Pointer to the APCB binary
 * @param[out]        groupId    - Group Id of the first type
 * @param[out]        typeId     - Type Id of the first type
 * @param[out]        priorityMask - Priority Mask
 * @param[out]        boardMask    - Board Mask
 * @param[out]        instance   - Instance Id of the first type
 * @param[out]        dataBuf    - Pointer to the data of the first type
 * @param[out]        dataSize   - size of data of the first type
 * @param[out]        typeHandle - Handle of the first type, used to retrieve subsequent types
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_INVALID
 *                    CORE_APCB_ERR_GROUP_INVALID
 *                    CORE_APCB_ERR_TYPE_INVALID
 *                    CORE_APCB_ERR_SPACE_NOT_ENOUGH
 *
 */
CORE_APCB_RET_CODE
coreApcbGetFirstType (
  IN        UINT8       *apcbBuf,
      OUT   UINT16      *groupId,
      OUT   UINT16      *typeId,
      OUT   UINT8       *priorityMask,
      OUT   UINT16      *boardMask,
      OUT   UINT16      *instance,
      OUT   UINT8       **dataBuf,
      OUT   UINT32      *dataSize,
      OUT   UINT32      *typeHandle
  )
{
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;
  UINT8                 *apcbTypeEnding;
  CORE_APCB_TYPE_ENTRY  *apcbTypeOrigin;

  customPrint ("\t[CORE APCB INFO] coreApcbGetFirstType Entry\n");

  customGetTypeCoreDb (&apcbTypeOrigin);
  coreApcbFreeTypeChain (apcbTypeOrigin);

  apcbTempBuf       = apcbBuf;
  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding        = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader   = (APCB_GROUP_HEADER *)&apcbHeader[1];
  apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  apcbTempBuf       = (UINT8 *)apcbGroupHeader;
  apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
  if (apcbGroupEnding > apcbEnding) {
    customPrint ("\t[CORE APCB ERR] Invalid first APCB group\n");
    return CORE_APCB_ERR_GROUP_INVALID;
  }

  apcbTempBuf       = (UINT8 *)apcbTypeHeader;
  apcbTypeEnding    = &apcbTempBuf[apcbTypeHeader->SizeOfType];
  if (apcbTypeEnding > apcbGroupEnding) {
    customPrint ("\t[CORE APCB ERR] Invalid first APCB type\n");
    return CORE_APCB_ERR_TYPE_INVALID;
  }

  apcbTypeOrigin    = (CORE_APCB_TYPE_ENTRY *)customAlloc (sizeof (CORE_APCB_TYPE_ENTRY));
  if (NULL == apcbTypeOrigin) {
    customPrint ("\t[CORE APCB ERR] Not enough space for core type data\n");
    return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
  }

  apcbTypeOrigin->next            = NULL;
  apcbTypeOrigin->typeHandle      = 1;
  apcbTypeOrigin->groupId         = apcbTypeHeader->GroupId;
  apcbTypeOrigin->typeId          = apcbTypeHeader->TypeId;
  apcbTypeOrigin->instance        = apcbTypeHeader->InstanceId;
  apcbTypeOrigin->priorityMask    = apcbTypeHeader->sApcbTypeExtV3.PriorityMask;
  apcbTypeOrigin->boardMask       = apcbTypeHeader->sApcbTypeExtV3.BoardMask;
  apcbTypeOrigin->dataBuf         = (UINT8 *)&apcbTypeHeader[1];
  apcbTypeOrigin->dataSize        = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
  apcbTypeOrigin->apcbGroupEnding = apcbGroupEnding;
  apcbTypeOrigin->apcbEnding      = apcbEnding;

  *groupId                        = apcbTypeHeader->GroupId;
  *typeId                         = apcbTypeHeader->TypeId;
  *instance                       = apcbTypeHeader->InstanceId;
  *priorityMask                   = apcbTypeOrigin->priorityMask;
  *boardMask                      = apcbTypeOrigin->boardMask;
  *dataBuf                        = (UINT8 *)&apcbTypeHeader[1];
  *dataSize                       = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
  *typeHandle                     = 1;

  customPrint ("\t[CORE APCB INFO] The first type: GroupId = 0x%x, TypeId = 0x%x, InstanceId = %d, PriorityMask = 0x%x, BoardMask = 0x%x, dataBuf = %08X, dataSize = %08X\n",
                  *groupId, *typeId, *instance, *priorityMask, *boardMask, *dataBuf, *dataSize);

  customSetTypeCoreDb (apcbTypeOrigin);

  return CORE_APCB_OK;
}


/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves the APCB type next to that of the current handle
 *
 * @param[in, out]    typeHandle      - Handle of the current type and passed out as the token to retrieve the subsequent type
 * @param[out]        groupId         - Group Id of the next type
 * @param[out]        typeId          - Type Id of the next type
 * @param[out]        priorityMask    - Priority Mask
 * @param[out]        boardMask       - Board Mask
 * @param[out]        instance        - Instance Id of the next type
 * @param[out]        dataBuf         - Pointer to the data of the first type
 * @param[out]        dataSize        - size of data of the first type
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_LAST_TYPE
 *                    CORE_APCB_ERR_NO_ORIGIN_TYPE
 *                    CORE_APCB_ERR_INVALID_TYPE_HANDLE
 *                    CORE_APCB_ERR_GROUP_INVALID
 *                    CORE_APCB_ERR_TYPE_INVALID
 *                    CORE_APCB_ERR_SPACE_NOT_ENOUGH
 *
 */
CORE_APCB_RET_CODE
coreApcbGetNextType (
  IN  OUT     UINT32      *typeHandle,
      OUT     UINT16      *groupId,
      OUT     UINT16      *typeId,
      OUT     UINT8       *priorityMask,
      OUT     UINT16      *boardMask,
      OUT     UINT16      *instance,
      OUT     UINT8       **dataBuf,
      OUT     UINT32      *dataSize
  )
{
  UINT8                 *apcbTempBuf;
  UINT8                 *apcbTypeEnding;
  UINT8                 *apcbGroupEnding;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  CORE_APCB_TYPE_ENTRY  *apcbTypeOrigin;
  CORE_APCB_TYPE_ENTRY  *currentApcbType;
  CORE_APCB_TYPE_ENTRY  *newApcbType;

  customPrint ("\t[CORE APCB INFO] coreApcbGetNextType Entry\n");

  apcbTypeOrigin      = NULL;
  customGetTypeCoreDb (&apcbTypeOrigin);

  if (NULL == apcbTypeOrigin) {
    customPrint ("\t[CORE APCB ERR] Call coreApcbGetFirstType to retrieve the first type first\n");
    return CORE_APCB_ERR_NO_ORIGIN_TYPE;
  }

  currentApcbType = apcbTypeOrigin;
  while (currentApcbType->typeHandle != *typeHandle) {
    if (currentApcbType->next == NULL) {
      customPrint ("\t[CORE APCB ERR] The type handle is invalid\n");
      return CORE_APCB_ERR_INVALID_TYPE_HANDLE;
    }
    currentApcbType = currentApcbType->next;
  }

  if (NULL != currentApcbType->next) {
    currentApcbType   = currentApcbType->next;
    *groupId          = currentApcbType->groupId;
    *typeId           = currentApcbType->typeId;
    *instance         = currentApcbType->instance;
    *priorityMask     = currentApcbType->priorityMask;
    *boardMask        = currentApcbType->boardMask;
    *dataBuf          = currentApcbType->dataBuf;
    *dataSize         = currentApcbType->dataSize;
    *typeHandle       = currentApcbType->typeHandle + 1;

    customPrint ("\t[CORE APCB INFO] The next type: GroupId = 0x%x, TypeId = 0x%x, InstanceId = %d, PriorityMask = 0x%x, BoardMask = 0x%x, dataBuf = %08X, dataSize = %08X\n",
                    *groupId, *typeId, *instance, *priorityMask, *boardMask, *dataBuf, *dataSize);

    return CORE_APCB_OK;
  }

  apcbTempBuf       = &currentApcbType->dataBuf[currentApcbType->dataSize];
  apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)apcbTempBuf;
  apcbGroupEnding   = currentApcbType->apcbGroupEnding;

  if (apcbTempBuf >= currentApcbType->apcbEnding) {
    customPrint ("\t[CORE APCB ERR] This is already the last APCB type\n");
    return CORE_APCB_ERR_LAST_TYPE;
  }

  if (apcbTempBuf >= apcbGroupEnding) {
    //
    // This is the beginning of a new group
    //
    apcbGroupHeader   = (APCB_GROUP_HEADER *)apcbTempBuf;
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    if (apcbGroupEnding > currentApcbType->apcbEnding) {
      customPrint ("\t[CORE APCB ERR] Invalid APCB group\n");
      return CORE_APCB_ERR_GROUP_INVALID;
    }
    apcbTempBuf       = (UINT8 *)apcbTypeHeader;
  }

  apcbTypeEnding    = &apcbTempBuf[apcbTypeHeader->SizeOfType];
  if (apcbTypeEnding > apcbGroupEnding) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB type\n");
    return CORE_APCB_ERR_TYPE_INVALID;
  }

  newApcbType       = (CORE_APCB_TYPE_ENTRY *)customAlloc (sizeof (CORE_APCB_TYPE_ENTRY));
  if (NULL == newApcbType) {
    customPrint ("\t[CORE APCB ERR] Not enough space for core type data\n");
    return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
  }

  currentApcbType->next           = newApcbType;
  newApcbType->next               = NULL;
  newApcbType->typeHandle         = currentApcbType->typeHandle + 1;
  newApcbType->groupId            = apcbTypeHeader->GroupId;
  newApcbType->typeId             = apcbTypeHeader->TypeId;
  newApcbType->instance           = apcbTypeHeader->InstanceId;
  newApcbType->priorityMask       = apcbTypeHeader->sApcbTypeExtV3.PriorityMask;
  newApcbType->boardMask          = apcbTypeHeader->sApcbTypeExtV3.BoardMask;
  newApcbType->dataBuf            = (UINT8 *)&apcbTypeHeader[1];
  newApcbType->dataSize           = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
  newApcbType->apcbGroupEnding    = apcbGroupEnding;
  newApcbType->apcbEnding         = currentApcbType->apcbEnding;

  *groupId                        = apcbTypeHeader->GroupId;
  *typeId                         = apcbTypeHeader->TypeId;
  *instance                       = apcbTypeHeader->InstanceId;
  *priorityMask                   = apcbTypeHeader->sApcbTypeExtV3.PriorityMask;
  *boardMask                      = apcbTypeHeader->sApcbTypeExtV3.BoardMask;
  *dataBuf                        = (UINT8 *)&apcbTypeHeader[1];
  *dataSize                       = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
  *typeHandle                     = currentApcbType->typeHandle + 1;

  customPrint ("\t[CORE APCB INFO] The next type: GroupId = 0x%x, TypeId = 0x%x, InstanceId = %d, PriorityMask = 0x%x, BoardMask = 0x%x, dataBuf = %08X, dataSize = %08X\n",
                  *groupId, *typeId, *instance, *priorityMask, *boardMask, *dataBuf, *dataSize);

  return CORE_APCB_OK;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves the APCB type by the specified attributes
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         groupId         - Group Id of the specified type
 * @param[in]         typeId          - Type Id of the specified type
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         instance        - Instance Id of the specified type
 * @param[out]        dataBuf         - Pointer to the data of the specified type
 * @param[out]        dataSize        - size of data of the specified type
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_INVALID
 *                    CORE_APCB_ERR_TYPE_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbGetType (
  IN           UINT8       *apcbBuf,
  IN           UINT16      groupId,
  IN           UINT16      typeId,
  IN           UINT8       priorityMask,
  IN           UINT16      boardMask,
  IN           UINT16      instance,
      OUT      UINT8       **dataBuf,
      OUT      UINT32      *dataSize
  )
{
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;

  customPrint ("\t[CORE APCB INFO] coreApcbGetType Entry\n");

  if (NULL == apcbBuf) {
    customPrint ("\t[CORE APCB ERR] NULL APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  apcbTempBuf       = apcbBuf;
  apcbHeader        = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding        = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader   = (APCB_GROUP_HEADER *)&apcbHeader[1];

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB Signature\n");
    return CORE_APCB_ERR_INVALID;
  }

  while ((UINT8 *)apcbGroupHeader < apcbEnding) {
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
      if (groupId == apcbTypeHeader->GroupId && typeId == apcbTypeHeader->TypeId &&
        (priorityMask & apcbTypeHeader->sApcbTypeExtV3.PriorityMask) != 0 &&
        ((boardMask & (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask)) != 0 || (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask + 1) == 0)) {
        *dataBuf          = (UINT8 *)&apcbTypeHeader[1];
        *dataSize         = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
        customPrint ("\t[CORE APCB INFO] APCB type found: GroupId = 0x%x, TypeId = 0x%x, InstanceId = %d, PriorityMask = 0x%x, BoardMask = 0x%x, dataBuf = %08X, dataSize = %08X\n",
                          groupId, typeId, instance, apcbTypeHeader->sApcbTypeExtV3.PriorityMask, apcbTypeHeader->sApcbTypeExtV3.BoardMask, *dataBuf, *dataSize);
        return CORE_APCB_OK;
      } else {
        apcbTempBuf       = (UINT8 *)apcbTypeHeader;
        apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbTempBuf[apcbTypeHeader->SizeOfType];
      }
    }
    apcbGroupHeader = (APCB_GROUP_HEADER *)apcbTypeHeader;
  }

  customPrint ("\t[CORE APCB ERR] APCB type not found\n");
  return CORE_APCB_ERR_TYPE_NOT_FOUND;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets the APCB type of the specified attributes
 *
 * @param[in]         apcbBuf         - Pointer to the APCB binary
 * @param[in]         sizeApcbBuf     - Size of the input APCB buffer. The buffer has to be
 *                                      big enough for the new type data to be substituted.
 * @param[in]         groupId         - Group Id of the specified type
 * @param[in]         typeId          - Type Id of the specified type
 * @param[in]         priorityMask    - Priority Mask
 * @param[in]         boardMask       - Board Mask
 * @param[in]         instance        - Instance Id of the specified type
 * @param[in]         dataBuf         - Pointer to the data of the specified type to be set
 * @param[in]         dataSize        - size of data of the specified type to be set
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_INVALID
 *                    CORE_APCB_ERR_TYPE_NOT_FOUND
 *
 */
CORE_APCB_RET_CODE
coreApcbSetType (
  IN           UINT8       *apcbBuf,
  IN  OUT      UINT32      sizeApcbBuf,
  IN           UINT16      groupId,
  IN           UINT16      typeId,
  IN           UINT8       priorityMask,
  IN           UINT16      boardMask,
  IN           UINT16      instance,
  IN           UINT8       *dataBuf,
  IN           UINT32      dataSize
  )
{
  APCB_V3_HEADER        *apcbHeader;
  APCB_GROUP_HEADER     *apcbGroupHeader;
  APCB_GROUP_HEADER     *existingApcbGroupHeader;
  APCB_V3_TYPE_HEADER   *apcbTypeHeader;
  UINT8                 *apcbTempBuf;
  UINT8                 *apcbEnding;
  UINT8                 *apcbGroupEnding;
  UINT8                 *apcbTypeEnding;
  UINT8                 *dataBuf0;
  UINT32                dataSize0;
  UINT32                *ApcbTail;
  UINT32                ApcbTailSizeInDword;
  UINT32                delta32;
  UINT32                i;
  UINT32                groupSignature;

  customPrint ("\t[CORE APCB INFO] coreApcbSetType Entry\n");

  if (NULL == apcbBuf) {
    customPrint ("\t[CORE APCB ERR] NULL APCB data\n");
    return CORE_APCB_ERR_INVALID;
  }

  apcbTempBuf               = apcbBuf;
  apcbHeader                = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding                = &apcbTempBuf[apcbHeader->SizeOfApcb];
  apcbGroupHeader           = (APCB_GROUP_HEADER *)&apcbHeader[1];
  existingApcbGroupHeader   = NULL;

  if (APCB_SIGNATURE != apcbHeader->Signature) {
    customPrint ("\t[CORE APCB ERR] Invalid APCB Signature\n");
    return CORE_APCB_ERR_INVALID;
  }

  while ((UINT8 *)apcbGroupHeader < apcbEnding) {
    apcbTempBuf       = (UINT8 *)apcbGroupHeader;
    apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
    apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
    if (apcbGroupHeader->GroupId == groupId) {
      //
      // The target group is already present in the APCB instance
      //
      existingApcbGroupHeader = apcbGroupHeader;
    }
    while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
      if (groupId == apcbTypeHeader->GroupId && typeId == apcbTypeHeader->TypeId &&
        (priorityMask & apcbTypeHeader->sApcbTypeExtV3.PriorityMask) != 0 &&
        ((boardMask & (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask)) != 0 || (UINT16) (apcbTypeHeader->sApcbTypeExtV3.BoardMask + 1) == 0)) {
        dataBuf0          = (UINT8 *)&apcbTypeHeader[1];
        dataSize0         = apcbTypeHeader->SizeOfType - sizeof (APCB_V3_TYPE_HEADER);
        customPrint ("\t[CORE APCB INFO] APCB type found: GroupId = 0x%x, TypeId = 0x%x, InstanceId = %d, PriorityMask = 0x%x, BoardMask = 0x%x, dataBuf = %08X, dataSize = %08X\n",
                          groupId, typeId, instance, apcbTypeHeader->sApcbTypeExtV3.PriorityMask, apcbTypeHeader->sApcbTypeExtV3.BoardMask, dataBuf0, dataSize0);

        if ((dataSize & 0x3) != 0) {
          //
          // Align dataSize to be multiple of 4 bytes
          //
          dataSize = (dataSize & 0xFFFFFFFC) + 4;
        }
        if (sizeApcbBuf < apcbHeader->SizeOfApcb - dataSize0 + dataSize) {
          customPrint ("\t[CORE APCB ERR] Not enough space to set the specified type\n");
          return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
        }

        ApcbTailSizeInDword     = ((UINT32) (UINTN) (apcbEnding - &dataBuf0[dataSize0])) / sizeof (UINT32);
        delta32 = (dataSize0 - dataSize) / sizeof (UINT32);
        if (dataSize > dataSize0) {
          ApcbTail = (UINT32 *)&dataBuf0[dataSize0];
          for (i = 0; i < ApcbTailSizeInDword; i ++) {
            ApcbTail[ApcbTailSizeInDword - i + delta32 - 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
          }
        } else {
          ApcbTail = (UINT32 *)&dataBuf0[dataSize];
          for (i = 0; i < ApcbTailSizeInDword; i ++) {
            ApcbTail[i] = ApcbTail[i + delta32];
          }
        }
        for (i = 0; i < dataSize; i ++) {
          dataBuf0[i] = dataBuf[i];
        }
        apcbTypeHeader->SizeOfType      = apcbTypeHeader->SizeOfType - (UINT16) dataSize0 + (UINT16) dataSize;
        apcbGroupHeader->SizeOfGroup    = apcbGroupHeader->SizeOfGroup - dataSize0 + dataSize;
        apcbHeader->SizeOfApcb          = apcbHeader->SizeOfApcb - dataSize0 + dataSize;

        return coreChecksumApcb (apcbBuf);
      } else {
        apcbTempBuf       = (UINT8 *)apcbTypeHeader;
        apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)&apcbTempBuf[apcbTypeHeader->SizeOfType];
      }
    }
    apcbGroupHeader = (APCB_GROUP_HEADER *)apcbTypeHeader;
  }

  if (existingApcbGroupHeader == NULL) {
    //
    // If the target group is not found, try creating a new one
    //
    apcbGroupHeader           = (APCB_GROUP_HEADER *)&apcbHeader[1];
    while ((UINT8 *)apcbGroupHeader < apcbEnding) {
      if (groupId > apcbGroupHeader->GroupId) {
        apcbTempBuf       = (UINT8 *)apcbGroupHeader;
        apcbGroupEnding   = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
        apcbGroupHeader   = (APCB_GROUP_HEADER *)apcbGroupEnding;
      } else {
        break;
      }
    }
    //
    // Make sure the APCB buffer is big enough to accommodate the increased APCB instance
    //
    if (sizeApcbBuf < apcbHeader->SizeOfApcb + sizeof (APCB_GROUP_HEADER)) {
      customPrint ("\t[CORE APCB ERR] Not enough space to create the new group header\n");
      return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
    }
    //
    // Find out the group signature
    //
    switch (groupId) {
      case APCB_GROUP_PSP:
        groupSignature          = SIGNATURE_32 ('P', 'S', 'P', 'G');
        break;
      case APCB_GROUP_CCX:
        groupSignature          = SIGNATURE_32 ('C', 'C', 'X', 'G');
        break;
      case APCB_GROUP_DF:
        groupSignature          = SIGNATURE_32 ('D', 'F', 'G', ' ');
        break;
      case APCB_GROUP_MEMORY:
        groupSignature          = SIGNATURE_32 ('M', 'E', 'M', 'G');
        break;
      case APCB_GROUP_GNB:
        groupSignature          = SIGNATURE_32 ('G', 'N', 'B', 'G');
        break;
      case APCB_GROUP_FCH:
        groupSignature          = SIGNATURE_32 ('F', 'C', 'H', 'G');
        break;
      case APCB_GROUP_CBS:
        groupSignature          = SIGNATURE_32 ('C', 'B', 'S', 'G');
        break;
      case APCB_GROUP_OEM:
        groupSignature          = SIGNATURE_32 ('O', 'E', 'M', 'G');
        break;
      case APCB_GROUP_TOKEN:
        groupSignature          = SIGNATURE_32 ('T', 'O', 'K', 'N');
        break;
      default:
        customPrint ("\t[CORE APCB ERR] Invalid Group ID: 0x%x\n", groupId);
        return CORE_APCB_ERR_GROUP_INVALID;
    }
    //
    // Found the location to insert the new group
    // Now move the tail data to the new location to free the space for the new group header
    //
    apcbHeader->SizeOfApcb += sizeof (APCB_GROUP_HEADER);
    ApcbTailSizeInDword     = ((UINT32) (UINTN) (apcbEnding - (UINT8 *)apcbGroupHeader)) / sizeof (UINT32);
    delta32                 = sizeof (APCB_GROUP_HEADER) / sizeof (UINT32);
    ApcbTail                = (UINT32 *)apcbGroupHeader;
    for (i = 0; i < ApcbTailSizeInDword; i ++) {
      ApcbTail[ApcbTailSizeInDword - i + delta32 - 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
    }
    //
    // Initialize the new group header
    //
    apcbGroupHeader->Signature          = groupSignature;
    apcbGroupHeader->GroupId            = groupId;
    apcbGroupHeader->SizeOfHeader       = sizeof (APCB_GROUP_HEADER);
    apcbGroupHeader->Version            = 1;
    apcbGroupHeader->SizeOfGroup        = sizeof (APCB_GROUP_HEADER);

    customPrint ("\t[CORE APCB INFO] New group header created successfully. GroupId = 0x%x\n", groupId);
  } else {
    apcbGroupHeader = existingApcbGroupHeader;
  }

  //
  // Create new type instance
  //
  apcbTempBuf               = apcbBuf;
  apcbHeader                = (APCB_V3_HEADER *)apcbBuf;
  apcbEnding                = &apcbTempBuf[apcbHeader->SizeOfApcb];

  apcbTempBuf               = (UINT8 *)apcbGroupHeader;
  apcbGroupEnding           = &apcbTempBuf[apcbGroupHeader->SizeOfGroup];
  apcbTypeHeader            = (APCB_V3_TYPE_HEADER *)&apcbGroupHeader[1];
  //
  // Find the location to insert the new type instance
  //
  while ((UINT8 *)apcbTypeHeader < apcbGroupEnding) {
    if (typeId > apcbTypeHeader->TypeId) {
      apcbTempBuf       = (UINT8 *)apcbTypeHeader;
      apcbTypeEnding    = &apcbTempBuf[apcbTypeHeader->SizeOfType];
      apcbTypeHeader    = (APCB_V3_TYPE_HEADER *)apcbTypeEnding;
    } else {
      break;
    }
  }

  if ((dataSize & 0x3) != 0) {
    //
    // Align dataSize to be multiple of 4 bytes
    //
    dataSize = (dataSize & 0xFFFFFFFC) + 4;
  }
  //
  // Make sure the APCB buffer is big enough to accommodate the increased APCB instance
  //
  if (sizeApcbBuf < apcbHeader->SizeOfApcb + sizeof (APCB_V3_TYPE_HEADER) + dataSize) {
    customPrint ("\t[CORE APCB ERR] Not enough space to create the new type instance\n");
    return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
  }

  //
  // Found the location to insert the new type instance
  // Now move the tail data to the new location to free the space for the new type instance
  //
  apcbHeader->SizeOfApcb        += sizeof (APCB_V3_TYPE_HEADER) + dataSize;
  apcbGroupHeader->SizeOfGroup  += sizeof (APCB_V3_TYPE_HEADER) + dataSize;
  ApcbTailSizeInDword            = ((UINT32) (UINTN) (apcbEnding - (UINT8 *)apcbTypeHeader)) / sizeof (UINT32);
  delta32                        = (sizeof (APCB_V3_TYPE_HEADER) + dataSize) / sizeof (UINT32);
  ApcbTail                       = (UINT32 *)apcbTypeHeader;
  for (i = 0; i < ApcbTailSizeInDword; i ++) {
    ApcbTail[ApcbTailSizeInDword - i + delta32 - 1] = ApcbTail[ApcbTailSizeInDword - i - 1];
  }

  //
  // Initialize the new type instance
  //
  apcbTypeHeader->GroupId                               = groupId;
  apcbTypeHeader->TypeId                                = typeId;
  apcbTypeHeader->SizeOfType                            = sizeof (APCB_V3_TYPE_HEADER) + (UINT16) dataSize;
  apcbTypeHeader->InstanceId                            = instance;
  if (APCB_GROUP_TOKEN == groupId) {
    apcbTypeHeader->sApcbTypeExtV3.ContextType          = 2;
    apcbTypeHeader->sApcbTypeExtV3.ContextFormat        = 1;
    apcbTypeHeader->sApcbTypeExtV3.UnitSize             = 8;
    apcbTypeHeader->sApcbTypeExtV3.KeySize              = 4;
    apcbTypeHeader->sApcbTypeExtV3.KeyPos               = 0;
  } else {
    apcbTypeHeader->sApcbTypeExtV3.ContextType          = 0;
    apcbTypeHeader->sApcbTypeExtV3.ContextFormat        = 0;
    apcbTypeHeader->sApcbTypeExtV3.UnitSize             = 0;
    apcbTypeHeader->sApcbTypeExtV3.KeySize              = 0;
    apcbTypeHeader->sApcbTypeExtV3.KeyPos               = 0;
  }
  apcbTypeHeader->sApcbTypeExtV3.PriorityMask           = priorityMask;
  apcbTypeHeader->sApcbTypeExtV3.BoardMask              = boardMask;

  //
  // Fill in the type data
  //
  dataBuf0          = (UINT8 *)&apcbTypeHeader[1];
  for (i = 0; i < dataSize; i ++) {
    dataBuf0[i] = dataBuf[i];
  }

  customPrint ("\t[CORE APCB INFO] New type instance created successfully. GroupId = 0x%x, TypeId = 0x%x, InstanceId = %d, PriorityMask = 0x%x, BoardMask = 0x%x\n",
                groupId, typeId, instance, priorityMask, boardMask);
  return coreChecksumApcb (apcbBuf);
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves the first APCB token
 *
 * @param[in]         typeHandle    - The type handle returned by coreApcbGetFirstType/coreApcbGetNextType
 * @param[out]        token         - token number
 * @param[out]        value         - token value
 * @param[out]        attribute     - attribute of the token: 0 - BOOL, 1- UINT8, 2 - UINT16, 3 - UINT32
 * @param[out]        tokenHandle   - Handle of the first token, used to retrieve subsequent token
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_NO_ORIGIN_TYPE
 *                    CORE_APCB_ERR_INVALID_TYPE_HANDLE
 *                    CORE_APCB_ERR_NON_APCB_TOKEN_CONTAINER
 *                    CORE_APCB_ERR_NO_MORE_TOKENS
 *
 */
CORE_APCB_RET_CODE
coreApcbGetFirstToken (
  IN          UINT32      typeHandle,
      OUT     UINT32      *token,
      OUT     VOID        *value,
      OUT     UINT8       *attribute,
      OUT     UINT32      *tokenHandle
  )
{
  UINT8                 *dataBuf;
  UINT32                dataSize;
  UINT32                handle;
  APCB_TOKEN_PAIR       *tempPair;
  CORE_APCB_TYPE_ENTRY  *apcbTypeOrigin;
  CORE_APCB_TYPE_ENTRY  *currentApcbType;
  CORE_APCB_TOKEN_ENTRY *apcbTokenOrigin;
  CORE_APCB_TOKEN_ENTRY *newApcbToken;
  CORE_APCB_TOKEN_ENTRY *previousApcbToken;
  BOOLEAN               firstApcbTokenHandled;
  CORE_APCB_RET_CODE    retSts;


  customPrint ("\t[CORE APCB INFO] coreApcbGetFirstToken Entry\n");

  retSts                  = CORE_APCB_ERR_NO_MORE_TOKENS;
  handle                  = 0;
  firstApcbTokenHandled   = FALSE;
  apcbTypeOrigin          = NULL;
  customGetTypeCoreDb (&apcbTypeOrigin);

  if (NULL == apcbTypeOrigin) {
    customPrint ("\t[CORE APCB ERR] Call coreApcbGetFirstType to retrieve the first type first\n");
    return CORE_APCB_ERR_NO_ORIGIN_TYPE;
  }

  customGetTokenCoreDb (&apcbTokenOrigin);
  coreApcbFreeTokenChain (apcbTokenOrigin);
  customSetTokenCoreDb (NULL);
  previousApcbToken   = NULL;

  currentApcbType     = apcbTypeOrigin;
  while (currentApcbType->typeHandle != typeHandle) {
    if (currentApcbType->next == NULL) {
      customPrint ("\t[CORE APCB ERR] The type handle is invalid\n");
      return CORE_APCB_ERR_INVALID_TYPE_HANDLE;
    }
    currentApcbType = currentApcbType->next;
  }

  if (currentApcbType->groupId != APCB_GROUP_TOKEN) {
    customPrint ("\t[CORE APCB ERR] The type does not contain APCB tokens\n");
    return CORE_APCB_ERR_NON_APCB_TOKEN_CONTAINER;
  }

  dataBuf          = currentApcbType->dataBuf;
  dataSize         = currentApcbType->dataSize;

  for (tempPair = (APCB_TOKEN_PAIR *)dataBuf; (UINT8 *)tempPair < &dataBuf[dataSize]; tempPair ++) {
    if (!firstApcbTokenHandled) {
      *token = tempPair->token;
      switch (currentApcbType->typeId) {
        case APCB_TYPE_TOKEN_BOOLEAN:
          *(BOOLEAN *)value  = tempPair->value.bValue;
          *attribute        = 0;
          break;
        case APCB_TYPE_TOKEN_1BYTE:
          *(UINT8 *)value   = tempPair->value.value8;
          *attribute        = 1;
          break;
        case APCB_TYPE_TOKEN_2BYTES:
          *(UINT16 *)value  = tempPair->value.value16;
          *attribute        = 2;
          break;
        case APCB_TYPE_TOKEN_4BYTES:
          *(UINT32 *)value  = tempPair->value.value32;
          *attribute        = 3;
          break;
        default:
          customPrint ("\t[CORE APCB ERR] The type does not contain APCB tokens\n");
          return CORE_APCB_ERR_NON_APCB_TOKEN_CONTAINER;
      }
      firstApcbTokenHandled = TRUE;
      retSts                = CORE_APCB_OK;
    }

    newApcbToken = (CORE_APCB_TOKEN_ENTRY *)customAlloc (sizeof (CORE_APCB_TOKEN_ENTRY));
    if (NULL == newApcbToken) {
      customPrint ("\t[CORE APCB ERR] Not enough space for core token data\n");
      return CORE_APCB_ERR_SPACE_NOT_ENOUGH;
    }

    if (NULL == previousApcbToken) {
      customSetTokenCoreDb (newApcbToken);
    } else {
      previousApcbToken->next = newApcbToken;
    }
    newApcbToken->next                  = NULL;
    newApcbToken->tokenHandle           = handle ++;
    newApcbToken->pair.token            = tempPair->token;
    newApcbToken->pair.value.value32    = tempPair->value.value32;
    newApcbToken->attribute             = *attribute;
    previousApcbToken                   = newApcbToken;
  }

  if (firstApcbTokenHandled) {
    *tokenHandle                        = 1;
    customPrint ("\t[CORE APCB INFO] First APCB token found: [%08X] = %08X\n", *token, *(UINT32 *)value);
  }
  return retSts;
}


/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves the next APCB token
 *
 * @param[in, out]    tokenHandle       - handle of the current token and passed out to retrieve the next token
 * @param[out]        token             - token number
 * @param[out]        value             - token value
 * @param[out]        attribute         - attribute of the token: 0 - BOOL, 1- UINT8, 2 - UINT16, 3 - UINT32
 *
 * @retval            CORE_APCB_OK
 *                    CORE_APCB_ERR_NO_MORE_TOKENS
 *
 */
CORE_APCB_RET_CODE
coreApcbGetNextToken (
  IN  OUT     UINT32      *tokenHandle,
      OUT     UINT32      *token,
      OUT     VOID        *value,
      OUT     UINT8       *attribute
  )
{
  CORE_APCB_TOKEN_ENTRY   *apcbTokenOrigin;
  CORE_APCB_TOKEN_ENTRY   *currentApcbToken;

  customPrint ("\t[CORE APCB INFO] coreApcbGetNextToken Entry\n");

  apcbTokenOrigin     = NULL;
  customGetTokenCoreDb (&apcbTokenOrigin);

  currentApcbToken    = apcbTokenOrigin;
  while (currentApcbToken->tokenHandle != *tokenHandle) {
    if (currentApcbToken->next == NULL) {
      customPrint ("\t[CORE APCB ERR] No more APCB tokens\n");
      return CORE_APCB_ERR_NO_MORE_TOKENS;
    }
    currentApcbToken = currentApcbToken->next;
  }

  *token              = currentApcbToken->pair.token;
  *(UINT32 *)value    = currentApcbToken->pair.value.value32;
  *attribute          = currentApcbToken->attribute;
  *tokenHandle        = currentApcbToken->tokenHandle + 1;
  customPrint ("\t[CORE APCB INFO] APCB token found: [%08X] = %08X\n", *token, *(UINT32 *)value);

  return CORE_APCB_OK;
}



