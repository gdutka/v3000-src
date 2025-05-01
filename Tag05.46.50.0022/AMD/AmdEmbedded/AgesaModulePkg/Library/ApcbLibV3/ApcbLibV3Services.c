/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * APCB Lib V3.0
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
#include <Porting.h>
// #include <Addendum/Apcb/Inc/SSP/APCB.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Arch.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Priority.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbDataGroups.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Token.h>
// #include "ApcbV3Priority.h"
#include "CoreApcbInterface.h"
#include <Library/ApcbLibV3.h>
#include <Library/IdsLib.h>
#include <Library/BaseMemoryLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_APCBLIBV3_APCBLIBV3SERVICES_FILECODE
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
UINT8                   CurrentPriorityLevel          = INVALID_PRIORITY_LEVEL;
UINT8                   CurrentBiosDirEntry           = INVALID_BIOS_DIR_ENTRY;
UINT8                   CurrentBinaryInstance         = INVALID_BINARY_INSTANCE;
UINT16                  CurrentBoardMask              = 0;

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a boolean APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully retrived
 *                    FALSE           - The token cannot be found
 *
 */
BOOLEAN
ApcbGetBool (
  IN       UINT32        ApcbToken,
      OUT  BOOLEAN       *Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
  if (NULL != ApcbBuf) {
    if (CORE_APCB_OK == coreApcbGetBool (ApcbBuf, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, 0, ApcbToken, Value)) {
      return TRUE;
    }
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a boolean APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully set
 *                    FALSE           - The token cannot be set
 *
 */
BOOLEAN
ApcbSetBool (
  IN       UINT32        ApcbToken,
  IN       BOOLEAN       Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      if (CORE_APCB_OK == coreApcbSetBool (ApcbBuf, ApcbBufSize, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, ApcbToken, Value)) {
        return TRUE;
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a UINT8 APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully retrived
 *                    FALSE           - The token cannot be found
 *
 */
BOOLEAN
ApcbGet8 (
  IN       UINT32        ApcbToken,
      OUT  UINT8         *Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
  if (NULL != ApcbBuf) {
    if (CORE_APCB_OK == coreApcbGet8 (ApcbBuf, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, 0, ApcbToken, Value)) {
      return TRUE;
    }
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a UINT8 APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully set
 *                    FALSE           - The token cannot be set
 *
 */
BOOLEAN
ApcbSet8 (
  IN       UINT32        ApcbToken,
  IN       UINT8         Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      if (CORE_APCB_OK == coreApcbSet8 (ApcbBuf, ApcbBufSize, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, ApcbToken, Value)) {
        return TRUE;
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a UINT16 APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully retrived
 *                    FALSE           - The token cannot be found
 *
 */
BOOLEAN
ApcbGet16 (
  IN       UINT32        ApcbToken,
      OUT  UINT16        *Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
  if (NULL != ApcbBuf) {
    if (CORE_APCB_OK == coreApcbGet16 (ApcbBuf, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, 0, ApcbToken, Value)) {
      return TRUE;
    }
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a UINT16 APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully set
 *                    FALSE           - The token cannot be set
 *
 */
BOOLEAN
ApcbSet16 (
  IN       UINT32        ApcbToken,
  IN       UINT16        Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      if (CORE_APCB_OK == coreApcbSet16 (ApcbBuf, ApcbBufSize, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, ApcbToken, Value)) {
        return TRUE;
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function retrieves a UINT32 APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully retrived
 *                    FALSE           - The token cannot be found
 *
 */
BOOLEAN
ApcbGet32 (
  IN       UINT32        ApcbToken,
      OUT  UINT32        *Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
  if (NULL != ApcbBuf) {
    if (CORE_APCB_OK == coreApcbGet32 (ApcbBuf, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, 0, ApcbToken, Value)) {
      return TRUE;
    }
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets a UINT32 APCB token
 *
 * @param[in]         ApcbToken       - APCB token ID
 * @param[out]        Value           - APCB token value
 *
 * @retval            TRUE            - The token value is successfully set
 *                    FALSE           - The token cannot be set
 *
 */
BOOLEAN
ApcbSet32 (
  IN       UINT32        ApcbToken,
  IN       UINT32        Value
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      if (CORE_APCB_OK == coreApcbSet32 (ApcbBuf, ApcbBufSize, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, ApcbToken, Value)) {
        return TRUE;
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function gets the data of a specified type
 *
 * @param[in]         GroupId         - Group ID
 * @param[in]         TypeId          - Type ID
 * @param[in]         InstanceId      - Instance ID
 * @param[out]        DataBuf         - Pointer to the type data
 * @param[out]        DataSize        - Pointer to the size of the type data
 *
 * @retval            TRUE            - The type data is retrieved successfully
 *                    FALSE           - The type data cannot be retrieved
 *
 */
BOOLEAN
ApcbGetType (
  IN       UINT16        GroupId,
  IN       UINT16        TypeId,
  IN       UINT16        InstanceId,
      OUT  UINT8         **DataBuf,
      OUT  UINT32        *DataSize
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
  if (NULL != ApcbBuf) {
    if (CORE_APCB_OK == coreApcbGetType (ApcbBuf, GroupId, TypeId, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, InstanceId, DataBuf, DataSize)) {
      return TRUE;
    }
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function sets the data of a specified type
 *
 * @param[in]         GroupId         - Group ID
 * @param[in]         TypeId          - Type ID
 * @param[in]         InstanceId      - Instance ID
 * @param[out]        DataBuf         - Pointer to the type data
 * @param[out]        DataSize        - Pointer to the size of the type data
 *
 * @retval            TRUE            - The type data is set successfully
 *                    FALSE           - The type data cannot be set
 *
 */
BOOLEAN
ApcbSetType (
  IN       UINT16        GroupId,
  IN       UINT16        TypeId,
  IN       UINT16        InstanceId,
  IN       UINT8         *DataBuf,
  IN       UINT32        DataSize
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      if (CORE_APCB_OK == coreApcbSetType (ApcbBuf, ApcbBufSize, GroupId, TypeId, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, InstanceId, DataBuf, DataSize)) {
        return TRUE;
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function purges tokens
 *
 *
 * @retval            TRUE            - The tokens are successfully purged
 *                    FALSE           - The tokens cannot be found
 *
 */
BOOLEAN
ApcbPurgeTokens (
  UINT32        *ApcbTokenList,
  UINT32        TokenCount
  )
{
  UINT8             i;
  UINT8             j;
  UINT32            Temp;
  UINT8             *ApcbBuf;
  UINT32            ApcbBufSize;
  UINT8             *DataBuf;
  UINT32            DataSize;
  UINT32            Size;
  UINT32            CopySize;
  BOOLEAN           found;
  APCB_TOKEN_PAIR   *SourcePair;
  APCB_TOKEN_PAIR   *TargetPair;
  UINT16            ApcbTypeIdList[] = {APCB_TYPE_TOKEN_BOOLEAN, APCB_TYPE_TOKEN_1BYTE, APCB_TYPE_TOKEN_2BYTES, APCB_TYPE_TOKEN_4BYTES};

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      // Sort ApcbTokenList
      for (i = 0; i < TokenCount - 1; i ++) {
        for (j = 0; j < TokenCount - i - 1; j ++) {
          if (ApcbTokenList[j] > ApcbTokenList[j + 1]) {
            Temp = ApcbTokenList[j];
            ApcbTokenList[j] = ApcbTokenList[j + 1];
            ApcbTokenList[j + 1] = Temp;
          }
        }
      }
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB purge tokens:\n");
      for (i = 0; i < TokenCount; i ++) {
        IDS_HDT_CONSOLE_PSP_TRACE ("\t\t\t0x%x\n", ApcbTokenList[i]);
      }
      for (i = 0; i < sizeof (ApcbTypeIdList) / sizeof (ApcbTypeIdList[0]); i ++) {
        if (CORE_APCB_OK == coreApcbGetType (ApcbBuf, APCB_GROUP_TOKEN, ApcbTypeIdList[i], APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, 0, &DataBuf, &DataSize)) {
          SourcePair = (APCB_TOKEN_PAIR *)DataBuf;
          TargetPair = SourcePair;
          Size = 0;
          CopySize = 0;
          while (Size < DataSize) {
            found = FALSE;
            for (j = 0; j < TokenCount; j ++) {
              if (SourcePair->token == ApcbTokenList[j]) {
                found = TRUE;
              } else if (SourcePair->token < ApcbTokenList[j]){
                break;
              }
            }
            if (!found) {
              CopyMem ((VOID*)TargetPair, (VOID*)SourcePair, sizeof (APCB_TOKEN_PAIR));
              CopySize += sizeof (APCB_TOKEN_PAIR);
              TargetPair ++;
              SourcePair = TargetPair;
            } else {
              SourcePair ++;
            }
            Size += sizeof (APCB_TOKEN_PAIR);
          }
          coreApcbSetType (ApcbBuf, ApcbBufSize, APCB_GROUP_TOKEN, ApcbTypeIdList[i], APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), CurrentBoardMask, 0, DataBuf, CopySize);
        }
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function purges all tokens
 *
 *
 * @retval            TRUE            - The tokens are successfully purged
 *                    FALSE           - The tokens cannot be found
 *
 */
BOOLEAN
ApcbPurgeAllTokens (
  VOID
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      if (CORE_APCB_OK != coreApcbSetType (ApcbBuf, ApcbBufSize, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_BOOLEAN, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, NULL, 0)) {
        return FALSE;
      }
      if (CORE_APCB_OK != coreApcbSetType (ApcbBuf, ApcbBufSize, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_1BYTE, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, NULL, 0)) {
        return FALSE;
      }
      if (CORE_APCB_OK != coreApcbSetType (ApcbBuf, ApcbBufSize, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_2BYTES, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, NULL, 0)) {
        return FALSE;
      }
      if (CORE_APCB_OK != coreApcbSetType (ApcbBuf, ApcbBufSize, APCB_GROUP_TOKEN, APCB_TYPE_TOKEN_4BYTES, APCB_PRIORITY_LEVEL_TO_MASK (CurrentPriorityLevel), APCB_BOARD_INSTANCE_ALL_KNOWN, 0, NULL, 0)) {
        return FALSE;
      }
      return TRUE;
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function purges all types
 *
 *
 * @retval            TRUE            - The types are successfully purged
 *                    FALSE           - The types cannot be purged
 *
 */
BOOLEAN
ApcbPurgeAllTypes (
  VOID
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;
  UINT32        TypeHandle;
  UINT16        GroupId;
  UINT16        TypeId;
  UINT8         *DataBuf;
  UINT32        DataSize;
  UINT8         PriorityMask;
  UINT16        BoardMask;
  UINT16        InstanceId;

  if (APCB_PRIORITY_LEVEL_HIGH == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_MEDIUM == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_EVENT_LOGGING == CurrentPriorityLevel ||
      APCB_PRIORITY_LEVEL_LOW == CurrentPriorityLevel) {
    ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
    if (NULL != ApcbBuf) {
      while (TRUE) {
        if (CORE_APCB_OK == coreApcbGetFirstType (ApcbBuf, &GroupId, &TypeId, &PriorityMask, &BoardMask, &InstanceId, &DataBuf, &DataSize, &TypeHandle)) {
          while (TRUE) {
            if (0 != DataSize) {
              if (ApcbSetType (GroupId, TypeId, InstanceId, NULL, 0)) {
                IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Type data purged from GroupId = 0x%0x, TypeId = 0x%x, PriorityMask = 0x%x, BoardMask = 0x%x, TypeInstanceId = 0x%x, 0x%x:0x%x\n",
                                            GroupId, TypeId, InstanceId, PriorityMask, BoardMask, DataBuf, DataSize);
                //
                // Restart type searching loop
                //
                break;
              } else {
                IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Failed to purge type data from GroupId = 0x%0x, TypeId = 0x%x, PriorityMask = 0x%x, BoardMask = 0x%x, TypeInstanceId = 0x%x, 0x%x:0x%x\n",
                                            GroupId, TypeId, InstanceId, PriorityMask, BoardMask, DataBuf, DataSize);
                return FALSE;
              }
            }
            if (CORE_APCB_OK != coreApcbGetNextType (&TypeHandle, &GroupId, &TypeId, &PriorityMask, &BoardMask, &InstanceId, &DataBuf, &DataSize)) {
              //
              // No more APCB types found
              //
              return TRUE;
            }
          }
        } else {
          //
          // No more APCB types found
          //
          return TRUE;        
        }
      }
    }
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB writes not allowed at the current priority level: %d\n", CurrentPriorityLevel);
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function demonstrates the usage of the following core APCB interfaces:
 *        - coreApcbGetFirstType
 *        - coreApcbGetNextType
 *        - coreApcbGetFirstToken
 *        - coreApcbGetNextToken
 *
 *
 * @retval            TRUE            - The tokens are successfully purged
 *                    FALSE           - The tokens cannot be found
 *
 */
BOOLEAN
ApcbEnumerateTokens (
  VOID
  )
{
  UINT8         *ApcbBuf;
  UINT32        ApcbBufSize;
  UINT32        TypeHandle;
  UINT32        TokenHandle;
  UINT16        GroupId;
  UINT16        TypeId;
  UINT8         PriorityMask;
  UINT16        BoardMask;
  UINT16        TypeInstanceId;
  UINT8         *DataBuf;
  UINT32        DataSize;
  UINT32        Token;
  UINT32        Value32;
  UINT8         Attribute;

  ApcbBuf = (UINT8 *)GetApcbShadowCopy (&ApcbBufSize);
  if (NULL != ApcbBuf) {
    if (CORE_APCB_OK == coreApcbGetFirstType (ApcbBuf, &GroupId, &TypeId, &PriorityMask, &BoardMask, &TypeInstanceId, &DataBuf, &DataSize, &TypeHandle)) {
      while (TRUE) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[Demo] GroupId = 0x%0x, TypeId = 0x%x, TypeInstanceId = 0x%x, 0x%x:0x%x\n",
                                      GroupId, TypeId, TypeInstanceId, DataBuf, DataSize);
        if (CORE_APCB_OK == coreApcbGetFirstToken (TypeHandle, &Token, &Value32, &Attribute, &TokenHandle)) {
          while (TRUE) {
            IDS_HDT_CONSOLE_PSP_TRACE ("[Demo]\t\t\t %a [0x%0x] = 0x%x\n",
                                      (0 == Attribute) ? "BOOL" : ((1 == Attribute) ? "UINT8" : ((2 == Attribute) ? "UINT16" : "UINT32")),
                                      Token, Value32);
            if (CORE_APCB_OK != coreApcbGetNextToken (&TokenHandle, &Token, &Value32, &Attribute)) {
              break;
            }
          }
        }
        if (CORE_APCB_OK != coreApcbGetNextType (&TypeHandle, &GroupId, &TypeId, &PriorityMask, &BoardMask, &TypeInstanceId, &DataBuf, &DataSize)) {
          break;
        }
      }
    }
  }
  return TRUE;
}


