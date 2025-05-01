/*****************************************************************************
  *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
  *******************************************************************************
  *
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * AgesaS3Save.c
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project:
 * @e \$Revision: 309090 $ @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 **/

#include <AMD.h>
#include <PiDxe.h>
#include <Filecode.h>

#define FILECODE LIBRARY_AMDCALLOUTLIB_AGESAS3SAVE_FILECODE

/*---------------------------------------------------------------------------------------*/
/**
 * OemHook to save the nonvolatile context
 *
 * @param[in]         DataSize
 * @param[in]         *Data
 *
 * @retval      EFI_SUCCESS
 *
 */
AGESA_STATUS
AgesaSaveNonVolatileS3Info (
  IN       UINTN                        DataSize,
  IN       VOID                         *Data
  )
{
  return EFI_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * OemHook to save the nonvolatile context
 *
 * @param[in]         DataSize
 * @param[in]         *Data
 *
 * @retval      BOOLEAN
 *
 */
AGESA_STATUS
AgesaSaveVolatileS3Info (
  IN       UINTN                        DataSize,
  IN       VOID                         *Data
  )
{
  return EFI_SUCCESS;
}


