/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Uefi.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/IdsLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDPSPMMIOLIB_AMDPSPMMIOLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
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


/**
 * @brief Read from PSP MMIO ofset
 * caller need to makesure PspMmioBase already programed before making the call.
 *
 * @param Offset  Offset to PSP MMIO base
 * @return UINT32 Value read from offset
 *           0XFF GetPspMmioBase error.
 */
UINT32
EFIAPI
PspMmioRead (
  IN UINT32 Offset
  )
{
  UINT32 MmioBase;
  UINT32 Value;
  BOOLEAN Status;
  MmioBase = 0;
  Value = 0xFFFFFFFF;// Retrun value in case of error.

  Status = GetPspMmioBase (&MmioBase);
  if (Status == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE (" GetPspMmioBase returned with Error \n");
    return Value;
  }
//todo change to debug verbose
//  IDS_HDT_CONSOLE_PSP_TRACE ("MmioBase %x\n", MmioBase);
  Value = *((volatile UINT32 *) (UINTN) (MmioBase + Offset));
//  IDS_HDT_CONSOLE_PSP_TRACE ("PspMmioRead %x <<\n",Value);
  return Value;
}

/**
 * @brief Read PSP MMIO with given offset,
 * caller need to makesure PspMmioBase already programed before making the call.
 *
 * @param Offset  Offset to PSP MMIO base
 * @param Value   Value to be write
 * @return VOID
 */
VOID
EFIAPI
PspMmioWrite (
  IN UINT32 Offset,
  IN UINT32 Value
  )
{
  UINT32 MmioBase;
  BOOLEAN Status;
//  IDS_HDT_CONSOLE_PSP_TRACE ("PspMmioWrite %x %x>>\n",Offset, Value);

  MmioBase = 0;
  Status = GetPspMmioBase (&MmioBase);
  if (Status == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE (" GetPspMmioBase returned with Error \n");
    return;
  }
//  IDS_HDT_CONSOLE_PSP_TRACE ("MmioBase %x\n", MmioBase);
  *((volatile UINT32 *) (UINTN) (MmioBase + Offset)) = Value;
//  IDS_HDT_CONSOLE_PSP_TRACE ("PspMmioWrite %x <<\n",Value);
}
