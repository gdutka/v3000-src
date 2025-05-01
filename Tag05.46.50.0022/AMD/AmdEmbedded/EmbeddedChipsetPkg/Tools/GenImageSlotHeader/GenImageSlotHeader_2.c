/** @file
  Auto Generation of OemRomSig

;******************************************************************************
;* Copyright (c) 2013, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <GenImageSlotHeader.h>
#include <Library/BaseMemoryLib.h>

#define MAX_STRING_LEN               1000

#define _GET_FILE_NAME(f) #f
#define GET_FILE_NAME(f) _GET_FILE_NAME(f)

int
main (
  IN  INTN  Argc,
  IN  CHAR8 **Argv,
  IN  CHAR8 **Envp
  )
/*++

Routine Description:
  GenImageSlotHeader_1 utillity to generate the OemRomSig

Arguments:
  Argc - Number of command line arguments
  Argv - Array of command line argument strings
  Envp - Array of environmemt variable strings

Returns:
  0 - Normal exit
  1 - Abnormal exit

--*/
{
  CHAR8   OemIshAFileName[MAX_STRING_LEN];
  IMAGE_SLOT_HEADER IshAData;
  FILE    *File;

  if (Argc < 2) {
    printf ("GenImageSlotHeader_1 OemRomSig FilePath\n");
    return -1;
  }

  sprintf(OemIshAFileName, Argv[1]);

  File = fopen(OemIshAFileName, "w");
  if (File == NULL) {
    printf ("\nCreate file failure: Cannot create %s file!\n", OemIshAFileName);
    return -1;
  }

  SetMem((VOID *)&IshAData, sizeof(IMAGE_SLOT_HEADER), 0xFF);

  IshAData.Checksum = 0x00000000;
  IshAData.BootPriority = PcdGet32(PcdFlashAbImageSlot2Priority);
  IshAData.UpdateRetries = PcdGet32(PcdFlashAbImageSlotUpdateRetries);
  IshAData.GlitchRetries = PcdGet8(PcdFlashAbImageSlotGlitchRetry);
  IshAData.Location = PcdGet32(PcdFlashAbImageSlot2Base);
  IshAData.PspId = RMB_PSP_ID;
  IshAData.SlotMaxSize = 0xFFFFFFFF;

  //
  // refer AmdPspBaseLibV2.c ValidatePspDir() function
  //
  IshAData.Checksum = Fletcher32 (
                        (UINT16 *) &IshAData.BootPriority,
                        (sizeof(IMAGE_SLOT_HEADER) - OFFSET_OF (IMAGE_SLOT_HEADER, BootPriority)) / 2
                        );

  fwrite (&IshAData , sizeof(UINT8), sizeof(IMAGE_SLOT_HEADER), File);

  fclose(File);
  return 0;

}
