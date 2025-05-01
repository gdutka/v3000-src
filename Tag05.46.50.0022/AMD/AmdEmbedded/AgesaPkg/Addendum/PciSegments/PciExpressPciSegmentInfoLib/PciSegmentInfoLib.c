/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Base.h>
#include <Library/PciSegmentInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

STATIC PCI_SEGMENT_INFO PciSegmentInfo[] = {
  {0, 0, 0, 0xFF},
  {1, 0, 0, 0xFF},
  {2, 0, 0, 0xFF},
  {3, 0, 0, 0xFF},
  {4, 0, 0, 0xFF},
  {5, 0, 0, 0xFF},
  {6, 0, 0, 0xFF},
  {7, 0, 0, 0xFF}
};

/**
  Return an array of PCI_SEGMENT_INFO holding the segment information.

  Note: The returned array/buffer is owned by callee.

  @param  Count  Return the count of segments.

  @retval A callee owned array holding the segment information.
**/
PCI_SEGMENT_INFO *
EFIAPI
GetPciSegmentInfo (
  UINTN  *Count
  )
{
  UINTN i;

  if (Count == NULL) {
    return NULL;
  }

  *Count = sizeof (PciSegmentInfo) / sizeof (PciSegmentInfo[0]);
  for (i = 0; i < *Count; i++) {
    PciSegmentInfo[i].BaseAddress = PcdGet64 (PcdPciExpressBaseAddress) + 0x10000000 * i;
  }

  return PciSegmentInfo;
}
