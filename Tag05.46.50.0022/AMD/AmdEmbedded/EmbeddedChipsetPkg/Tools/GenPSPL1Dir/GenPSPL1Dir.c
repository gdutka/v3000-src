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
#include <GenPSPL1Dir.h>
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
  GenPSPL1Dir utillity to generate the OemRomSig

Arguments:
  Argc - Number of command line arguments
  Argv - Array of command line argument strings
  Envp - Array of environmemt variable strings

Returns:
  0 - Normal exit
  1 - Abnormal exit

--*/
{
  CHAR8   PSPL1DirFileName[MAX_STRING_LEN];
  PSP_DIRECTORY PSPL1DirData;
  FILE    *File;

  if (Argc < 2) {
    printf ("GenPSPL1Dir OemRomSig FilePath\n");
    return -1;
  }

  sprintf(PSPL1DirFileName, Argv[1]);

  File = fopen(PSPL1DirFileName, "w");
  if (File == NULL) {
    printf ("\nCreate file failure: Cannot create %s file!\n", PSPL1DirFileName);
    return -1;
  }

  SetMem((VOID *)&PSPL1DirData, sizeof(PSP_DIRECTORY), 0xFF);

  // Header
  PSPL1DirData.Header.Cookie = PSP_DIRECTORY_HEADER_SIGNATURE;
  PSPL1DirData.Header.Checksum = 0x00000000;
  PSPL1DirData.Header.TotalEntries = PcdGet32(PcdFlashAbImageSlotNumbers);
  PSPL1DirData.Header.Reserved = 0x00000000;

  // Entry 1
  PSPL1DirData.PspEntry[0].Type.Value = PSP_REGION_A_DIR;
  PSPL1DirData.PspEntry[0].Size = 0x00000000;
  PSPL1DirData.PspEntry[0].Location = PcdGet32(PcdFlashImageSlot1HdrBase);

  // Entry 2
  PSPL1DirData.PspEntry[1].Type.Value = PSP_REGION_B_DIR;
  PSPL1DirData.PspEntry[1].Size = 0x00000000;
  PSPL1DirData.PspEntry[1].Location = PcdGet32(PcdFlashImageSlot2HdrBase);


#ifdef RMB_COMBO_32M
  // Entry 3
  PSPL1DirData.PspEntry[2].Type.Value = PSP_REGION_A_DIR;
  PSPL1DirData.PspEntry[2].Size = 0x00000000;
  PSPL1DirData.PspEntry[2].Location = PcdGet32(PcdFlashImageSlot3HdrBase);

  // Entry 4
  PSPL1DirData.PspEntry[3].Type.Value = PSP_REGION_B_DIR;
  PSPL1DirData.PspEntry[3].Size = 0x00000000;
  PSPL1DirData.PspEntry[3].Location = PcdGet32(PcdFlashImageSlot4HdrBase);
#endif

  //
  // refer AmdPspBaseLibV2.c ValidatePspDir() function
  //
  PSPL1DirData.Header.Checksum = Fletcher32 (
                                   (UINT16 *) &PSPL1DirData.Header.TotalEntries, 
                                   (sizeof (UINT32) * (PcdGet32(PcdFlashAbImageSlotNumbers) * 4 + 2)) / 2
                                   );


  fwrite (&PSPL1DirData , sizeof(UINT32), (PcdGet32(PcdFlashAbImageSlotNumbers) * 4 + 4), File);

  fclose(File);
  return 0;

}
