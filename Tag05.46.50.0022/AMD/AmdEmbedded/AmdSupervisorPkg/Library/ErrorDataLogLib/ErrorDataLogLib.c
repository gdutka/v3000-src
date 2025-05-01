/*
*******************************************************************************

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/ErrorDataLogLib.h>
#include <Library/SmmSupvTestPointLib.h>

#define BIOS_RAM_MMIO_BASE                0xFED10000
#define SMMSUPV_ERROR_DATA_BIOSRAM_OFFSET 0x80
#define FCH_PM_MMIO_BASE                  0xFED80300  // FCH::PM::DECODEEN
#define ISACONTROL_OFFSET                 0x4

VOID
EnableBiosMmio (
  VOID
  )
{
  UINT32 Value;
  Value = MmioRead32 (FCH_PM_MMIO_BASE + ISACONTROL_OFFSET);
  // Enable FCH::PM::ISACONTROL.biosramen if not enabled yet
  if ((Value & BIT0) == 0) {
    Value |= BIT0;
    MmioWrite32 (FCH_PM_MMIO_BASE + ISACONTROL_OFFSET, Value);
  }
}

VOID
WriteBiosRam32 (
  UINT8 Index,
  UINT32 Data
  )
{
  UINT8  i;
  EnableBiosMmio ();
  for (i = 0; i < 4; i++) {
    MmioWrite8 (BIOS_RAM_MMIO_BASE + Index + i, *(((UINT8 *) &Data) + i));
  }
}

VOID
WriteErrorData (
  IN SMMSUPV_POST_CODE Data
  )
{
  UINT32 BiosRamData = SMMSUPV_POST_CODE_PREFIX;

  SmmSupvTestPoint (Data);

  BiosRamData |= (UINT32) Data;
  WriteBiosRam32 (SMMSUPV_ERROR_DATA_BIOSRAM_OFFSET, (UINT32) BiosRamData);

  if (Data != 0) {
    DEBUG ((DEBUG_ERROR, "%aSmmSupv Error: 0x%x\n", PREFIX_LOG_ANALYZER, (UINT32) BiosRamData));
  }
}

VOID
WriteErrorDataAndReset (
  IN SMMSUPV_POST_CODE Data
  )
{
  UINT32 BiosRamData = SMMSUPV_POST_CODE_PREFIX;

  SmmSupvTestPoint (Data);

  BiosRamData |= (UINT32) Data;
  WriteBiosRam32 (SMMSUPV_ERROR_DATA_BIOSRAM_OFFSET, (UINT32) BiosRamData);

  DEBUG ((DEBUG_ERROR, "%aSmmSupv Error and Reset: 0x%x\n", PREFIX_LOG_ANALYZER, (UINT32) BiosRamData));

  ResetCold ();
}

VOID
CleanErrorData (
  )
{
  UINT32 Data = 0;

  WriteErrorData (Data);
}
