/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Library
 *
 * Contains interface to the AMD AGESA library
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
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdSocBaseLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ApcbVariableLibV3.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdPspFlashUpdateLib.h>
// #include <Addendum/Apcb/Inc/SSP/APOB.h>
#include <Library/AmdPspApobLib.h>
#include <Library/ApcbLibV3.h>
#include <Library/ApcbChecksumLibV3.h>
// #include <Addendum/Apcb/Inc/SSP/APCB.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Arch.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Priority.h>
// #include <Addendum/Apcb/Inc/CommonV3/ApcbDataGroups.h>
// #include <Addendum/Apcb/Inc/CommonV3/ApcbV3Token.h>
#include <Library/ApobCommonServiceLib.h>
#include <Protocol/SmmBase2.h>
#include <Library/MemRestore.h>
#include <Library/ApobCommonServiceLib.h>
#include <Library/AmdPspFwImageHeaderLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_APCBLIBV3_APCBLIBV3_FILECODE

#define APCB_SIGNATURE       0x42435041ul

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef enum {
  APCB_SHADOW_BUF_POST_TIME = 0,
  APCB_SHADOW_BUF_RUN_TIME  = 1,
  APCB_SHADOW_BUF_ACTIVE    = 2,
} APCB_SHADOW_BUF_TYPE;

typedef UINT64 (*GET_ADDRESS_TYPE) (APCB_SHADOW_BUF_TYPE ApcbShadowBufferType);
typedef VOID (*SET_ADDRESS_TYPE) (APCB_SHADOW_BUF_TYPE ApcbShadowBufferType, UINT64 Address);

typedef struct {
  APCB_V3_HEADER          *Address;
  UINT32                  Size;
} APCB_ENTRY_INFO;

typedef struct {
  GET_ADDRESS_TYPE    GetAddress;
  SET_ADDRESS_TYPE    SetAddress;
} APCB_SHADOW;

typedef struct {
  UINT8                   BiosDirEntry;
  UINT8                   BinaryInstance;
  APCB_ENTRY_INFO         ApcbEntryInfo;
  APCB_SHADOW             ApcbShadow;
} APCB_BINARY_INSTANCE_STORAGE;

typedef struct {
  UINT8                   Purpose;
  UINT8                   PriorityLevel;
} APCB_PURPOSE_TO_PRIORITY_LEVEL;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
UINT64
GetApcb68ShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType
  );

UINT64
GetApcb60ConfigShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType
  );

UINT64
GetApcb60EvtLgShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType
  );

BOOLEAN
GetApcbMutexLocked (
  VOID
 );

VOID
SetApcb68ShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType,
  IN UINT64 Address
  );

VOID
SetApcb60ConfigShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType,
  IN UINT64 Address
  );

VOID
SetApcb60EvtLgShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType,
  IN UINT64 Address
  );

VOID
SetApcbMutexLocked (
  IN BOOLEAN Value
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern UINT8  CurrentPriorityLevel;
extern UINT8  CurrentBiosDirEntry;
extern UINT8  CurrentBinaryInstance;
extern UINT16 CurrentBoardMask;

/*----------------------------------------------------------------------------------------
 *                          G L O B A L        V A L U E S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN                 mInSmm                                = FALSE;
BOOLEAN                 mAtRuntime                            = FALSE;
EFI_SMM_SYSTEM_TABLE2   *mSmst                                = NULL;

APCB_PURPOSE_TO_PRIORITY_LEVEL PurposeToPriorityMapping[] = {
  {APCB_TYPE_PURPOSE_HARD_FORCE,      APCB_PRIORITY_LEVEL_HARD_FORCE},
  {APCB_TYPE_PURPOSE_ADMIN,           APCB_PRIORITY_LEVEL_HIGH},
  {APCB_TYPE_PURPOSE_DEBUG,           APCB_PRIORITY_LEVEL_MEDIUM},
  {APCB_TYPE_PURPOSE_EVENT_LOGGING,   APCB_PRIORITY_LEVEL_EVENT_LOGGING},
  {APCB_TYPE_PURPOSE_NORMAL,          APCB_PRIORITY_LEVEL_LOW},
  {APCB_TYPE_PURPOSE_DEFAULT,         APCB_PRIORITY_LEVEL_DEFAULT}
};

PRIORITY_INSTANCE_MAPPING PriorityInstanceMapping[] = {
  {APCB_PRIORITY_LEVEL_HARD_FORCE,    BIOS_APCB_INFO_BACKUP,  APCB_BINARY_INSTANCE_HARD_FORCE    },
  {APCB_PRIORITY_LEVEL_HIGH,          BIOS_APCB_INFO,         APCB_BINARY_INSTANCE_BIOS_CONFIG   },
  {APCB_PRIORITY_LEVEL_MEDIUM,        BIOS_APCB_INFO,         APCB_BINARY_INSTANCE_BIOS_CONFIG   },
  {APCB_PRIORITY_LEVEL_EVENT_LOGGING, BIOS_APCB_INFO,         APCB_BINARY_INSTANCE_EVENT_LOGGING },
  {APCB_PRIORITY_LEVEL_LOW,           BIOS_APCB_INFO,         APCB_BINARY_INSTANCE_BIOS_CONFIG   },
  {APCB_PRIORITY_LEVEL_DEFAULT,       BIOS_APCB_INFO_BACKUP,  APCB_BINARY_INSTANCE_DEFAULT       }
};

APCB_BINARY_INSTANCE_STORAGE ApcbBinaryInstanceStorage[] = {
  {
    BIOS_APCB_INFO_BACKUP,
    APCB_BINARY_INSTANCE_DEFAULT,
    {
      NULL,
      0
    },
    {
      GetApcb68ShadowAddress,
      SetApcb68ShadowAddress
    }
  },
  {
    BIOS_APCB_INFO,
    APCB_BINARY_INSTANCE_BIOS_CONFIG,
    {
      NULL,
      0
    },
    {
      GetApcb60ConfigShadowAddress,
      SetApcb60ConfigShadowAddress
    }
  },
  {
    BIOS_APCB_INFO,
    APCB_BINARY_INSTANCE_EVENT_LOGGING,
    {
      NULL,
      0
    },
    {
      GetApcb60EvtLgShadowAddress,
      SetApcb60EvtLgShadowAddress
    }
  }
};

/*
 * Function to get the shadow copy address of APCB entry 0x68
*/
UINT64
GetApcb68ShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType
  )
{
  EFI_STATUS            Status;
  UINT64                Address;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  Address             = 0;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return Address;
  }

  switch (ApcbShadowBufferType) {
    case APCB_SHADOW_BUF_POST_TIME:
      Address = pApcbVariableStruct->Apcb68PostTimeShadowAddress;
      break;

    case APCB_SHADOW_BUF_RUN_TIME:
      Address = pApcbVariableStruct->Apcb68RunTimeShadowAddress;
      break;

    case APCB_SHADOW_BUF_ACTIVE:
      if (!mAtRuntime) {
        Address = pApcbVariableStruct->Apcb68PostTimeShadowAddress;
      } else {
        Address = pApcbVariableStruct->Apcb68RunTimeShadowAddress;
      }
      break;

    default:
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: Apcb Shadow Buffer Type Error\n", __FUNCTION__);
      ASSERT (FALSE);
      break;
  }

  return Address;
}

/*
 * Function to get the shadow copy address of APCB entry 0x68
*/
UINT64
GetApcb60ConfigShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType
  )
{
  EFI_STATUS            Status;
  UINT64                Address;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  Address             = 0;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return Address;
  }

  switch (ApcbShadowBufferType) {
    case APCB_SHADOW_BUF_POST_TIME:
      Address = pApcbVariableStruct->Apcb60ConfigPostTimeShadowAddress;
      break;

    case APCB_SHADOW_BUF_RUN_TIME:
      Address = pApcbVariableStruct->Apcb60ConfigRunTimeShadowAddress;
      break;

    case APCB_SHADOW_BUF_ACTIVE:
      if (!mAtRuntime) {
        Address = pApcbVariableStruct->Apcb60ConfigPostTimeShadowAddress;
      } else {
        Address = pApcbVariableStruct->Apcb60ConfigRunTimeShadowAddress;
      }
      break;

    default:
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: Apcb Shadow Buffer Type Error\n", __FUNCTION__);
      ASSERT (FALSE);
      break;
  }

  return Address;
}

/*
 * Function to get the shadow copy address of APCB entry 0x68
*/
UINT64
GetApcb60EvtLgShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType
  )
{
  EFI_STATUS            Status;
  UINT64                Address;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  Address             = 0;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return Address;
  }

  switch (ApcbShadowBufferType) {
    case APCB_SHADOW_BUF_POST_TIME:
      Address = pApcbVariableStruct->Apcb60EvtLgPostTimeShadowAddress;
      break;

    case APCB_SHADOW_BUF_RUN_TIME:
      Address = pApcbVariableStruct->Apcb60EvtLgRunTimeShadowAddress;
      break;

    case APCB_SHADOW_BUF_ACTIVE:
      if (!mAtRuntime) {
        Address = pApcbVariableStruct->Apcb60EvtLgPostTimeShadowAddress;
      } else {
        Address = pApcbVariableStruct->Apcb60EvtLgRunTimeShadowAddress;
      }
      break;

    default:
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: Apcb Shadow Buffer Type Error\n", __FUNCTION__);
      ASSERT (FALSE);
      break;
  }

  return Address;
}

/**
 * @brief Get Apcb Mutex Locked Value
 *
 * @param   VOID
 *
 * @return  BOOLEAN  Apcb Mutex Locked Value
 */
BOOLEAN
GetApcbMutexLocked (
  VOID
 )
{
  EFI_STATUS            Status;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return 0;
  }

  return pApcbVariableStruct->ApcbMutexLocked;
}

/*
 * Function to set the shadow copy address of APCB entry 0x68
*/
VOID
SetApcb68ShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType,
  IN UINT64 Address
  )
{
  EFI_STATUS            Status;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return;
  }

  switch (ApcbShadowBufferType) {
    case APCB_SHADOW_BUF_POST_TIME:
      pApcbVariableStruct->Apcb68PostTimeShadowAddress = Address;
      break;

    case APCB_SHADOW_BUF_RUN_TIME:
      pApcbVariableStruct->Apcb68RunTimeShadowAddress = Address;
      break;

    case APCB_SHADOW_BUF_ACTIVE:
      if (!mAtRuntime) {
        pApcbVariableStruct->Apcb68PostTimeShadowAddress = Address;
      } else {
        pApcbVariableStruct->Apcb68RunTimeShadowAddress = Address;
      }
      break;

    default:
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: Apcb Shadow Buffer Type Error\n", __FUNCTION__);
      ASSERT (FALSE);
      break;
  }

}

/*
 * Function to set the shadow copy address of APCB entry 0x68
*/
VOID
SetApcb60ConfigShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType,
  IN UINT64 Address
  )
{
  EFI_STATUS            Status;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return;
  }

  switch (ApcbShadowBufferType) {
    case APCB_SHADOW_BUF_POST_TIME:
      pApcbVariableStruct->Apcb60ConfigPostTimeShadowAddress = Address;
      break;

    case APCB_SHADOW_BUF_RUN_TIME:
      pApcbVariableStruct->Apcb60ConfigRunTimeShadowAddress = Address;
      break;

    case APCB_SHADOW_BUF_ACTIVE:
      if (!mAtRuntime) {
        pApcbVariableStruct->Apcb60ConfigPostTimeShadowAddress = Address;
      } else {
        pApcbVariableStruct->Apcb60ConfigRunTimeShadowAddress = Address;
      }
      break;

    default:
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: Apcb Shadow Buffer Type Error\n", __FUNCTION__);
      ASSERT (FALSE);
      break;
  }

}

/*
 * Function to set the shadow copy address of APCB entry 0x68
*/
VOID
SetApcb60EvtLgShadowAddress (
  IN APCB_SHADOW_BUF_TYPE ApcbShadowBufferType,
  IN UINT64 Address
  )
{
  EFI_STATUS            Status;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return;
  }

  switch (ApcbShadowBufferType) {
    case APCB_SHADOW_BUF_POST_TIME:
      pApcbVariableStruct->Apcb60EvtLgPostTimeShadowAddress = Address;
      break;

    case APCB_SHADOW_BUF_RUN_TIME:
      pApcbVariableStruct->Apcb60EvtLgRunTimeShadowAddress = Address;
      break;

    case APCB_SHADOW_BUF_ACTIVE:
      if (!mAtRuntime) {
        pApcbVariableStruct->Apcb60EvtLgPostTimeShadowAddress = Address;
      } else {
        pApcbVariableStruct->Apcb60EvtLgRunTimeShadowAddress = Address;
      }
      break;

    default:
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: Apcb Shadow Buffer Type Error\n", __FUNCTION__);
      ASSERT (FALSE);
      break;
  }

}

/**
 * @brief Set Apcb Mutex Locked Value
 *
 * @param[in]  BOOLEAN  Apcb Mutex Locked Value
 */
VOID
SetApcbMutexLocked (
  IN BOOLEAN Value
  )
{
  EFI_STATUS            Status;
  APCB_VARIABLE_STRUCT  *pApcbVariableStruct;

  Status              = EFI_SUCCESS;
  pApcbVariableStruct = NULL;

  pApcbVariableStruct = GetApcbVariableStruct ();
  if (pApcbVariableStruct == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] %a: pApcbVariableStruct is NULL\n", __FUNCTION__, Status);
    ASSERT (FALSE);
    return;
  }

  pApcbVariableStruct->ApcbMutexLocked = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function acquires mutex for the subsequent APCB operations
 *
 */
BOOLEAN
ApcbAcquireMutex (
  )
{
  if (!GetApcbMutexLocked ()) {
    SetApcbMutexLocked (TRUE);
    if (!mInSmm) {
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB DXE Mutex acquired successfully\n");
      return TRUE;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB SMM Mutex acquired successfully\n");
      return TRUE;
    }
  } else {
    if (!mInSmm) {
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Failed to acquire APCB DXE Mutex\n");
      ASSERT (FALSE);
      return FALSE;
    } else {
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Failed to acquire APCB SMM Mutex\n");
      ASSERT (FALSE);
      return FALSE;
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function releases mutex for the subsequent APCB operations
 *
 */
VOID
ApcbReleaseMutex (
  )
{
  SetApcbMutexLocked(FALSE);
  if (!mInSmm) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB DXE Mutex released successfully\n");
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB SMM Mutex released successfully\n");
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function set the target priority level for the subsequent APCB operations
 *
 * @param[in]         PriorityLevel        - APCB Priority Level
 *
 * @retval
 *
 */
VOID
ApcbSetPriorityLevel (
  IN       UINT8         PriorityLevel
  )
{
  UINT8   i;

  for (i = 0; i < sizeof (PriorityInstanceMapping) / sizeof (PRIORITY_INSTANCE_MAPPING); i ++) {
    if (PriorityLevel == PriorityInstanceMapping[i].PriorityLevel) {
      CurrentPriorityLevel  = PriorityLevel;
      CurrentBiosDirEntry   = PriorityInstanceMapping[i].BiosDirEntry;
      CurrentBinaryInstance = PriorityInstanceMapping[i].BinaryInstance;
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB Priority Level set to %d\n", CurrentPriorityLevel);
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] \t\tCurrentBiosDirEntry = 0x%x\n", CurrentBiosDirEntry);
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] \t\tCurrentBinaryInstance = 0x%x\n", CurrentBinaryInstance);
      return;
    }
  }
  ASSERT (FALSE);
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function set the purpose for the subsequent APCB operations
 *
 * @param[in]         Purpose        - Purpose of APCB operations
 *
 * @retval
 *
 */
VOID
ApcbSetPurpose (
  IN       UINT8         Purpose
  )
{
  UINT8   i;

  ASSERT (Purpose > APCB_TYPE_PURPOSE_NONE && Purpose < APCB_TYPE_PURPOSE_LIMIT);

  for (i = 0; i < sizeof (PurposeToPriorityMapping) / sizeof (APCB_PURPOSE_TO_PRIORITY_LEVEL); i ++) {
    if (Purpose == PurposeToPriorityMapping[i].Purpose) {
      ApcbSetPriorityLevel (PurposeToPriorityMapping[i].PriorityLevel);
      return;
    }
  }
  ASSERT (FALSE);
}

/*---------------------------------------------------------------------------------------*/
/**
 *      This function set the purpose for the subsequent APCB operations
 *
 *
 * @retval  - Purpose
 *
 */
UINT8
ApcbGetPurpose (
  VOID
  )
{
  UINT8   i;

  for (i = 0; i < sizeof (PurposeToPriorityMapping) / sizeof (APCB_PURPOSE_TO_PRIORITY_LEVEL); i ++) {
    if (CurrentPriorityLevel == PurposeToPriorityMapping[i].PriorityLevel) {
      return PurposeToPriorityMapping[i].Purpose;
    }
  }
  ASSERT (FALSE);
  return APCB_TYPE_PURPOSE_NONE;
}

/*
 * Function to get ApcbShadowCopy Address, return NULL if fail
*/
VOID *
GetApcbShadowCopy (
  IN OUT  UINT32  *Size
  )
{
  VOID*          ApcbData;
  UINT8          i;
  UINT8          j;
  APCB_V3_HEADER ApcbHeader;

  ASSERT (Size != NULL);

  ApcbData = NULL;
  for (i = 0; i < sizeof (PriorityInstanceMapping) / sizeof (PRIORITY_INSTANCE_MAPPING); i ++) {
    if (CurrentPriorityLevel == PriorityInstanceMapping[i].PriorityLevel) {
      for (j = 0; j < sizeof (ApcbBinaryInstanceStorage) / sizeof (APCB_BINARY_INSTANCE_STORAGE); j ++) {
        if (PriorityInstanceMapping[i].BiosDirEntry == ApcbBinaryInstanceStorage[j].BiosDirEntry &&
            PriorityInstanceMapping[i].BinaryInstance == ApcbBinaryInstanceStorage[j].BinaryInstance) {

          ApcbData = (VOID *) (UINTN) ApcbBinaryInstanceStorage[j].ApcbShadow.GetAddress (APCB_SHADOW_BUF_ACTIVE);
          *Size = ApcbBinaryInstanceStorage[j].ApcbEntryInfo.Size;

          if (0 != *Size && NULL != ApcbData && APCB_SIGNATURE == (*(UINT32 *)ApcbData)) {
            IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] GetApcbShadowCopy for Entry 0x%x, Binary Instance %d, 0x%x:0x%x\n",
                                        ApcbBinaryInstanceStorage[j].BiosDirEntry,
                                        ApcbBinaryInstanceStorage[j].BinaryInstance,
                                        ApcbData,
                                        *Size);
            MapSpiDataToBuffer ((UINT32)(UINTN) ApcbBinaryInstanceStorage[j].ApcbEntryInfo.Address,
                    (VOID *) &ApcbHeader,
                    sizeof (APCB_V3_HEADER));
            if (ApcbHeader.UniqueApcbInstance != ((APCB_V3_HEADER*)ApcbData)->UniqueApcbInstance) {
              MapSpiDataToBuffer ((UINT32)(UINTN) ApcbBinaryInstanceStorage[j].ApcbEntryInfo.Address,
                      (VOID *) ApcbData,
                      ApcbHeader.SizeOfApcb);
            }
            return ApcbData;
          } else {
            IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB Shadow copy not found\n");
            return NULL;
          }
        }
      }
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB Shadow copy not found\n");
  return NULL;
}

/*
 * Function to set ApcbShadowCopy Address
*/
VOID
SetApcbShadowCopy (
  IN      VOID * ApcbShadowAddress
  )
{
  UINT8 i;
  UINT8 j;

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] SetApcbShadowCopy %x\n", ApcbShadowAddress);

  ASSERT (NULL != ApcbShadowAddress);

  for (i = 0; i < sizeof (PriorityInstanceMapping) / sizeof (PRIORITY_INSTANCE_MAPPING); i ++) {
    if (CurrentPriorityLevel == PriorityInstanceMapping[i].PriorityLevel) {
      for (j = 0; j < sizeof (ApcbBinaryInstanceStorage) / sizeof (APCB_BINARY_INSTANCE_STORAGE); j ++) {
        if (PriorityInstanceMapping[i].BiosDirEntry == ApcbBinaryInstanceStorage[j].BiosDirEntry &&
            PriorityInstanceMapping[i].BinaryInstance == ApcbBinaryInstanceStorage[j].BinaryInstance) {

          ApcbBinaryInstanceStorage[j].ApcbShadow.SetAddress (APCB_SHADOW_BUF_ACTIVE, (UINT64) (UINTN) ApcbShadowAddress);
          return;
        }
      }
    }
  }
  ASSERT (FALSE);
}

/*
 * Function to calculate APCB checksum
 * @todo: Remove and replace it with ApcbUpdateCheckSum()
*/
UINT8
ApcbCalcCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length
  )
{
  UINT8  CheckSum;
  UINT8  *DataPtr;

  ASSERT (NULL != ApcbPtr);

  CheckSum = 0;
  DataPtr = ApcbPtr;
  while (Length --) {
    CheckSum += *(DataPtr ++);
  }
  return CheckSum;
}

/**
 *  Write APCB data to BIOS Directory APCB Entry 0x60
 *
 *
 *  @retval EFI_SUCCESS       The Data save to FLASH successfully
 *
 **/
EFI_STATUS
AmdPspWriteBackApcbShadowCopy (
  IN    BOOLEAN             bDiscardMemContext
  )
{
  APCB_V3_HEADER                  *ApcbHeader;
  APCB_V3_HEADER                  *ApcbBaseDataHeader;
  UINT8                           *ApcbData;
  BOOLEAN                         ApcbRecoveryFlag;
  BOOLEAN                         ApcbChanged;
  UINT32                          SizeOfApcb;
  UINT8                           i;

  ApcbChanged = FALSE;

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] AmdPspWriteBackApcbShadowCopy Enter\n");

  //
  // Exit service, if recovery flag set
  //
  if (mAtRuntime == FALSE && mInSmm == FALSE) {
    ApobGetApcbRecoveryFlag (&ApcbRecoveryFlag);
    if (ApcbRecoveryFlag) {
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB.RecoveryFlag Set, exit service\n");
      return EFI_UNSUPPORTED;
    }
  }
  //
  // Exit service, if recovery flag set
  //
  if (TRUE == (mInSmm ? CheckPspRecoveryFlagSmn () : CheckPspRecoveryFlagV2 ())) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Recovery flag set, exit service\n");
    // ASSERT (FALSE); // Assertion in the debug build
    return EFI_UNSUPPORTED;
  }

  for (i = 0; i < sizeof (ApcbBinaryInstanceStorage) / sizeof (APCB_BINARY_INSTANCE_STORAGE); i ++) {
    if (BIOS_APCB_INFO_BACKUP == ApcbBinaryInstanceStorage[i].BiosDirEntry) {
      //
      // Do NOT write to the read-only entry (Entry 0x68)
      //
      continue;
    }

    ApcbData = (UINT8 *)(UINTN) ApcbBinaryInstanceStorage[i].ApcbShadow.GetAddress (APCB_SHADOW_BUF_ACTIVE);
    ApcbBaseDataHeader = (APCB_V3_HEADER *)(((UINT8 *)ApcbData) + ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size);

    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Ready to write APCB shadow copy @ 0x%x back to : Entry 0x%x, Binary Instance %d\n",
                                ApcbData,
                                ApcbBinaryInstanceStorage[i].BiosDirEntry,
                                ApcbBinaryInstanceStorage[i].BinaryInstance
                              );
    ApcbHeader = (APCB_V3_HEADER *) ApcbData;
    if (NULL != ApcbHeader && 0 != ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size) {
      if (ApcbHeader->SizeOfApcb > ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB SPI Entry too small: 0x%x < 0x%x\n",
                                    ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size,
                                    ApcbHeader->SizeOfApcb
                                  );
      }

      //
      // Compare if any changes
      //
      MapSpiDataToBuffer ((UINT32)(UINTN) ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
                          (VOID *) ApcbBaseDataHeader,
                          sizeof (APCB_V3_HEADER)
                          );

      SizeOfApcb = ApcbBaseDataHeader->SizeOfApcb;
      MapSpiDataToBuffer ((UINT32)(UINTN) ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
                          (VOID *) ApcbBaseDataHeader,
                          SizeOfApcb
                          );

      ApcbChanged = FALSE;
      if ((ApcbBaseDataHeader->CheckSumByte != ApcbHeader->CheckSumByte) ||
          (ApcbBaseDataHeader->SizeOfApcb != ApcbHeader->SizeOfApcb)     ||
          (CompareMem (ApcbData, (VOID *) ApcbBaseDataHeader, ApcbHeader->SizeOfApcb))) {
        ApcbChanged = TRUE;
      }

      if (ApcbChanged) {
        //Update UniqueApcbInstance when APCB content has been updated
        ApcbHeader->UniqueApcbInstance += 1;
        ApcbHeader->CheckSumByte -= 1;
        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Data different between APCB shadow & SPI copy at Entry %x, Binary Instance %d\n",
                                   ApcbBinaryInstanceStorage[i].BiosDirEntry,
                                   ApcbBinaryInstanceStorage[i].BinaryInstance
                                   );
        ApcbUpdateCheckSum (ApcbHeader, ApcbHeader->SizeOfApcb, ApcbBinaryInstanceStorage[i].BiosDirEntry, ApcbBinaryInstanceStorage[i].BinaryInstance);
        if (CompareMem (ApcbData + ApcbHeader->SizeOfApcb, (VOID *) (((UINTN) ApcbBaseDataHeader) + ApcbHeader->SizeOfApcb), HMAC_CHECKSUM_LENGTH) != 0) {
          PspUpdateFlash ((UINT32)(UINTN)ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address, ApcbHeader->SizeOfApcb + HMAC_CHECKSUM_LENGTH, ApcbData);
        } else {
          PspUpdateFlash ((UINT32)(UINTN)ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address, ApcbHeader->SizeOfApcb, ApcbData);
        }

        if (bDiscardMemContext) {
          //
          // APCB Update should trigger memory re-training in the next boot
          //
          IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Trigger memory re-training in the next boot\n");

          if (SocFamilyIdentificationCheck (F19_RPL_RAW_ID) || \
              SocFamilyIdentificationCheck(F19_RMB_B0_RAW_ID) || SocFamilyIdentificationCheck(F19_RMB_A0_RAW_ID)) {
            // new flag supported since RPL
            AmdMemRestoreApcbChanged ();
          } else {
            AmdMemRestoreDiscardCurrentMemContext ();
          }

        }
      } else {
        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] No APCB data update for Entry %x, Binary Instance %d. Bypassing SPI write.\n",
                                   ApcbBinaryInstanceStorage[i].BiosDirEntry,
                                   ApcbBinaryInstanceStorage[i].BinaryInstance
                                  );
      }
    }
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] AmdPspWriteBackApcbShadowCopy Exit\n");

  return EFI_SUCCESS;
}

/**
 *
 *  Init mApcbSmmRtShadowCopy
 *
 **/
EFI_STATUS
EFIAPI
ApcbRTBCallBack (
  IN CONST EFI_GUID                       *Protocol,
  IN VOID                                 *Interface,
  IN EFI_HANDLE                           Handle
  )
{
  APCB_V3_HEADER  *ApcbHeader;
  UINT8           i;

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] ApcbRTBCallBack\n");
  for (i = 0; i < sizeof (ApcbBinaryInstanceStorage) / sizeof (APCB_BINARY_INSTANCE_STORAGE); i ++) {
    //
    //Check if mApcbSmmRtShadowCopy already initialized
    //Use APCB signature to do the simple check below
    //
    ApcbHeader = (APCB_V3_HEADER *)(UINTN) ApcbBinaryInstanceStorage[i].ApcbShadow.GetAddress (APCB_SHADOW_BUF_RUN_TIME);

    if ((NULL != ApcbHeader) &&
        (0 != ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size) &&
        (APCB_SIGNATURE != ApcbHeader->Signature)
        ) {
      IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Copy the Apcb from SPI %x to APCB RT copy %x, size = %x\n",
                                  ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
                                  (UINTN) ApcbHeader,
                                  ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size
                                );
      //Copy the Apcb from SPI to Shadow Copy
      MapSpiDataToBuffer( (UINT32)(UINTN) ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
                (VOID *) ApcbHeader,
                ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size
              );
    }
  }

  mAtRuntime = TRUE;

  return EFI_SUCCESS;
}
/**
 *
 *  ApcbLibConstructor, initial the shadow copy of APCB data, and save the address to PCD
 *
 **/
EFI_STATUS
EFIAPI
ApcbLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  APCB_V3_HEADER              *ApcbShadowCopy;
  BOOLEAN                     ApcbEntryExist;
  TYPE_ATTRIB                 TypeAttrib;
  UINT64                      EntryDest;
  EFI_SMM_BASE2_PROTOCOL      *SmmBase2;
  EFI_STATUS                  Status;
  VOID                        *Registration;
  BOOLEAN                     ApcbRecoveryFlag;
  UINT8                       i;
  UINT32                      SubProgram;
  EFI_MEMORY_TYPE             MemoryPoolType;
  UINTN                       MemorySize;
  UINTN                       *pApcbEntryInfoAddress;
  UINT32                      PspFwImageHeaderLen;

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] ApcbLibConstructor\n");
  ApcbShadowCopy        = NULL;
  mInSmm                = FALSE;
  SubProgram            = 0;

  //
  // Exit if BR program
  //
  if (SocFamilyIdentificationCheck (F15_BR_RAW_ID)) {
    return EFI_SUCCESS;
  }

  //
  // Exit service, if recovery flag set
  //
  ApcbRecoveryFlag = FALSE;
  Status = ApobGetApcbRecoveryFlag (&ApcbRecoveryFlag);
  if (ApcbRecoveryFlag) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB.RecoveryFlag Set, exit service\n");
    return EFI_SUCCESS;
  }

  //
  // Exit service, if recovery flag set
  //
  if (CheckPspRecoveryFlagV2 () == TRUE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Recovery flag set, exit service\n");
    // ASSERT (FALSE); // Assertion in the debug build
    return EFI_SUCCESS;
  }

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**) &SmmBase2);
  if (!EFI_ERROR (Status)) {
    SmmBase2->InSmm (SmmBase2, &mInSmm);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] gEfiSmmBase2ProtocolGuid Locate Fail\n");
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] mInSmm: %a\n", mInSmm ? "InSmm" : "OutSmm");

  ApobGetBoardMask (0, &CurrentBoardMask);
  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Current Board Mask = %x\n", CurrentBoardMask);
  ApobGetSubProgram (&SubProgram);
  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Current SubProgram = %x\n", SubProgram);

  //
  // Initialize the APCB purpose to priority mapping
  //
  for (i = 0; i < sizeof (PurposeToPriorityMapping) / sizeof (APCB_PURPOSE_TO_PRIORITY_LEVEL); i ++) {
    switch (PurposeToPriorityMapping[i].Purpose) {
    case APCB_TYPE_PURPOSE_ADMIN:
      PurposeToPriorityMapping[i].PriorityLevel = PcdGet8 (PcdAmdApcbPriorityLevelAdmin);
      break;
    case APCB_TYPE_PURPOSE_DEBUG:
      PurposeToPriorityMapping[i].PriorityLevel = PcdGet8 (PcdAmdApcbPriorityLevelDebug);
      break;
    case APCB_TYPE_PURPOSE_NORMAL:
      PurposeToPriorityMapping[i].PriorityLevel = PcdGet8 (PcdAmdApcbPriorityLevelNormal);
      break;
    default:
      break;
    }
  }

  TypeAttrib.SubProgram = SubProgram & 0x7;
  for (i = 0; i < sizeof (ApcbBinaryInstanceStorage) / sizeof (APCB_BINARY_INSTANCE_STORAGE); i ++) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Locating APCB binary Instance 0x%x in Entry 0x%x ...\n",
                                ApcbBinaryInstanceStorage[i].BinaryInstance,
                                ApcbBinaryInstanceStorage[i].BiosDirEntry
                              );
    ApcbEntryExist = BIOSEntryInfoByAttributes (ApcbBinaryInstanceStorage[i].BiosDirEntry,
                                    ApcbBinaryInstanceStorage[i].BinaryInstance,
                                    (UINT8) SubProgram & 0x7,
                                    &TypeAttrib,
                                    (VOID *)&ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
                                    &ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size,
                                    &EntryDest
                                    );

    // Check Apcb Data has PSP Fw Image Header or not
    if (ApcbFwImageHeaderCheck (
          (UINT64)(UINTN)ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
          &PspFwImageHeaderLen)) {
      pApcbEntryInfoAddress   = (UINTN *) &ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address;
      *pApcbEntryInfoAddress += PspFwImageHeaderLen;
      if ((ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size != 0) &&
          (ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size > PspFwImageHeaderLen) &&
          (PspFwImageHeaderLen != 0)) {
        ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size -= PspFwImageHeaderLen;
      }
    }
    //
    // Check if Posttime APCB shadow already initialized
    //
    // If PcdApcbUseHmacChecksum == TRUE
    //   |- Total Allocate Buffer Size =
    //      ApcbEntrySize * 2 [PostTime][EfiBootServicesData] +
    //      ApcbEntrySize * 2 [RunTime][EfiRuntimeServicesData (Allocate in SMM)]
    //
    // If PcdApcbUseHmacChecksum == FALSE
    //   |- Total Allocate Buffer Size =
    //      ApcbEntrySize * 2 [PostTime|RunTime][EfiRuntimeServicesData]
    //   [Note] PostTime and RunTime use the Same Buffer.
    ApcbShadowCopy = (APCB_V3_HEADER *) (UINTN) ApcbBinaryInstanceStorage[i].ApcbShadow.GetAddress (APCB_SHADOW_BUF_POST_TIME);
    if (ApcbShadowCopy == NULL) {
      if (!ApcbEntryExist) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB Instance 0x%x does not exist in Entry 0x%x\n",
                                    ApcbBinaryInstanceStorage[i].BinaryInstance,
                                    ApcbBinaryInstanceStorage[i].BiosDirEntry
                                  );
      } else {
        //
        // Shadow Copy haven't init yet
        //
        if (FeaturePcdGet(PcdApcbUseHmacChecksum)) {
          MemoryPoolType = EfiBootServicesData;
        } else{
          MemoryPoolType = EfiRuntimeServicesData;
        }
        MemorySize = ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size * 2;

        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Allocate PostTime APCB Shadow Copy for Binary Entry 0x%x Instance 0x%x with 0x%x bytes\n",
                                    ApcbBinaryInstanceStorage[i].BiosDirEntry,
                                    ApcbBinaryInstanceStorage[i].BinaryInstance,
                                    MemorySize
                                  );

        Status = gBS->AllocatePool (MemoryPoolType,
                                    MemorySize,
                                    (VOID **) &ApcbShadowCopy
                                   );
        if (EFI_ERROR(Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE("[APCB Lib V3] AllocatePool fail\n");
        }
        ASSERT (ApcbShadowCopy != NULL);
        if (ApcbShadowCopy == NULL) {
          IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Fail to Allocate APCB Shadow Copy\n");
          return EFI_OUT_OF_RESOURCES;
        }
        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] PostTime ALLOCATED [%x:%x]\n",
                                    ApcbShadowCopy,
                                    MemorySize
                                  );

        ZeroMem (ApcbShadowCopy, MemorySize);
        //
        // Copy the Apcb from SPI to Shadow Copy
        //
        MapSpiDataToBuffer( (UINT32)(UINTN) ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Address,
                ApcbShadowCopy,
                ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size
              );
        //
        // Calc the checksum, and report the warning
        //
        if (!ApcbValidateCheckSum (ApcbShadowCopy, ApcbShadowCopy->SizeOfApcb, ApcbBinaryInstanceStorage[i].BiosDirEntry, ApcbBinaryInstanceStorage[i].BinaryInstance)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Warning: APCB Checksum failure\n");
        }

        ApcbBinaryInstanceStorage[i].ApcbShadow.SetAddress (APCB_SHADOW_BUF_POST_TIME, (UINT64) (UINTN) ApcbShadowCopy);
      }
    }
  }

  if (!mInSmm) {
    return EFI_SUCCESS;
  }

  //
  // We are now in SMM
  // get SMM table base
  //
  Status = SmmBase2->GetSmstLocation (SmmBase2, &mSmst);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] GetSmstLocation Fail\n");
    return Status;
  }

  for (i = 0; i < sizeof (ApcbBinaryInstanceStorage) / sizeof (APCB_BINARY_INSTANCE_STORAGE); i ++) {

    ApcbShadowCopy = (APCB_V3_HEADER *) (UINTN) ApcbBinaryInstanceStorage[i].ApcbShadow.GetAddress (APCB_SHADOW_BUF_RUN_TIME);
    if ((ApcbShadowCopy == NULL) && (0 != ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size)) {

      if (FeaturePcdGet(PcdApcbUseHmacChecksum)) {
        //
        // Allocate APCB shadow for SMM RT
        //   front part is for shadow copy
        //   back part is for working copy
        //
        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Allocate RunTime APCB Shadow Copy for Binary Entry 0x%x Instance 0x%x with 0x%x bytes\n",
                                    ApcbBinaryInstanceStorage[i].BiosDirEntry,
                                    ApcbBinaryInstanceStorage[i].BinaryInstance,
                                    (ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size * 2)
                                  );

        Status = mSmst->SmmAllocatePool (
                          EfiRuntimeServicesData,
                          ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size * 2,
                          (VOID **)&ApcbShadowCopy
                          );

        if (ApcbShadowCopy == NULL) {
          IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] APCB Shadow Copy allocate fail\n");
          ASSERT (ApcbShadowCopy != NULL);
          return Status;
        }

        IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] RunTime ALLOCATED [%x:%x]\n",
                                    ApcbShadowCopy,
                                    ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size * 2
                                  );

        ZeroMem (ApcbShadowCopy, ApcbBinaryInstanceStorage[i].ApcbEntryInfo.Size * 2);

        ApcbBinaryInstanceStorage[i].ApcbShadow.SetAddress (APCB_SHADOW_BUF_RUN_TIME, (UINT64) (UINTN) ApcbShadowCopy);

      } else {
        // PostTime Buffer Address is as same as RunTime Buffer Address.
        ApcbShadowCopy = (APCB_V3_HEADER *) (UINTN) ApcbBinaryInstanceStorage[i].ApcbShadow.GetAddress (APCB_SHADOW_BUF_POST_TIME);
        ApcbBinaryInstanceStorage[i].ApcbShadow.SetAddress (APCB_SHADOW_BUF_RUN_TIME, (UINT64) (UINTN) ApcbShadowCopy);
      }
    }
  }

#ifdef APCB_SHADOW_ADDRESS_DEBUG
  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Apcb68ShadowAddress:\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("  |- PostTimeShadowAddress:  0x%08x\n", \
    ApcbBinaryInstanceStorage[0].ApcbShadow.GetAddress (APCB_SHADOW_BUF_POST_TIME));
  IDS_HDT_CONSOLE_PSP_TRACE ("  |- RunTimeShadowAddress:   0x%08x\n", \
    ApcbBinaryInstanceStorage[0].ApcbShadow.GetAddress (APCB_SHADOW_BUF_RUN_TIME));

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Apcb60ConfigShadowAddress:\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("  |- PostTimeShadowAddress:  0x%08x\n", \
    ApcbBinaryInstanceStorage[1].ApcbShadow.GetAddress (APCB_SHADOW_BUF_POST_TIME));
  IDS_HDT_CONSOLE_PSP_TRACE ("  |- RunTimeShadowAddress:   0x%08x\n", \
    ApcbBinaryInstanceStorage[1].ApcbShadow.GetAddress (APCB_SHADOW_BUF_RUN_TIME));

  IDS_HDT_CONSOLE_PSP_TRACE ("[APCB Lib V3] Apcb60EvtLgShadowAddress:\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("  |- PostTimeShadowAddress:  0x%08x\n", \
    ApcbBinaryInstanceStorage[2].ApcbShadow.GetAddress (APCB_SHADOW_BUF_POST_TIME));
  IDS_HDT_CONSOLE_PSP_TRACE ("  |- RunTimeShadowAddress:   0x%08x\n", \
    ApcbBinaryInstanceStorage[2].ApcbShadow.GetAddress (APCB_SHADOW_BUF_RUN_TIME));
#endif

  //
  // Register SMM Exit boot service callback used init mApcbSmmRtShadowCopy
  //
  mSmst->SmmRegisterProtocolNotify (
           &gEfiSmmReadyToLockProtocolGuid,
           ApcbRTBCallBack,
           &Registration
           );

  return EFI_SUCCESS;
}


