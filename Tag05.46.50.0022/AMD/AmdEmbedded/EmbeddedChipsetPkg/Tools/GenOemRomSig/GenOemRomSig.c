/** @file
  Auto Generation of OemRomSig

;******************************************************************************
;* Copyright (c) 2013 - 2020, Insyde Software Corp. All Rights Reserved.
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
//#include <direct.h>
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>

#define MAX_STRING_LEN                  1000
#define NUM_OF_MULLINS_ROMSIG_FILED  20
#define ROMSIG_HEADER_FILED_NUM         0
#define ROMSIG_IEC_FILED_NUM            1
#define ROMSIG_XHCI_FILED_NUM           3
#define ROMSIG_PSP_DIRECTORY_FIELD_NUM_LEGACY  4        //For prior Program, ML, BP, CZ
#define ROMSIG_PSP_DIRECTORY_FIELD_NUM         5        //For New Program, ST, ZP
#define ROMSIG_BACKUP_PSP_DIR_FIELD_NUM        11       //EFS.ComboB

#define ROMSIG_SPI_2ND_GEN_EFS_FIELD_NUM       9        //Flag to indicate ROMSIG for MTS onward, offset 0x24[0]. 1 - 1st gen, 0 - 2nd gen
#define ROMSIG_BIOS_COMBO_HDR_FIELD_NUM        10       //Bios Combo Header

#define ROM_SIG_ADDR                    0x20000

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
  GenOemRomSig utillity to generate the OemRomSig

Arguments:
  Argc - Number of command line arguments
  Argv - Array of command line argument strings
  Envp - Array of environmemt variable strings

Returns:
  0 - Normal exit
  1 - Abnormal exit

--*/
{
  CHAR8              OemRomSigFileName[MAX_STRING_LEN];
  UINT32  RomSigData[NUM_OF_MULLINS_ROMSIG_FILED] = {0};
  UINT8   *RomSigData8;
  UINT32             FlashFvSigBase;
  UINT32             FlashAreaBase;
  FILE               *File;

  if (Argc < 2) {
    printf ("GenOemRomSig OemRomSig FilePath\n");
    return -1;
  }

  snprintf(OemRomSigFileName, MAX_STRING_LEN, "%s", Argv[1]);

  File = fopen(OemRomSigFileName, "w");
  if (File == NULL) {
    printf ("\nCreate file failure: Cannot create %s file!\n", OemRomSigFileName);
    return -1;
  }

  FlashFvSigBase = PcdGet32 (PcdH2OFlashFvSigBase);
  FlashAreaBase = PcdGet32 (PcdFlashAreaBaseAddress);

  RomSigData[ROMSIG_HEADER_FILED_NUM] = 0x55AA55AA;
  RomSigData[ROMSIG_IEC_FILED_NUM] = 0;
  
  RomSigData[ROMSIG_PSP_DIRECTORY_FIELD_NUM] = PcdGet32(PcdFlashPSPL1HdrBase) - PcdGet32 (PcdFlashAreaBaseAddress);
  RomSigData[ROMSIG_BACKUP_PSP_DIR_FIELD_NUM] = PcdGet32(PcdFlashBackupPSPL1HdrBase) - PcdGet32 (PcdFlashAreaBaseAddress);
  
  //Enable 33Mhz 
  RomSigData8 = (UINT8 *)&RomSigData[0];
  RomSigData8[0x47] = 0x0; // Normal read 33MHz
  RomSigData8[0x48] = 1;
  RomSigData8[0x49] = 0x55;    
 
  if (FlashFvSigBase - FlashAreaBase != ROM_SIG_ADDR) {
    printf ("\nRomsig address error..%x, %x!!\n", FlashFvSigBase, FlashAreaBase);
    return -1;
  }

  fwrite (RomSigData , sizeof(UINT32), NUM_OF_MULLINS_ROMSIG_FILED, File);

  fclose(File);
  return 0;
}
