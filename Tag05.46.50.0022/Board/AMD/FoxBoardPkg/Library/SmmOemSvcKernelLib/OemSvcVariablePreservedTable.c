/** @file
  To return the preserved list.

;******************************************************************************
;* Copyright (c) 2014 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/SmmOemSvcKernelLib.h>
#include <Guid/H2OSeamlessRecovery.h>

#define PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_GUID      {0xffffffff, 0xffff, 0xffff, { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,0xff, 0xff}}
#define PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_NAME      L"PreservedTable2OtherAllName"

//
// Old funcion OemSvcVariablePreservedTable
// The followings are the sample codes for old function OemSvcVariablePreservedTable. Please customize here.
//
//#define SampleGuid { 0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 }
//PRESERVED_VARIABLE_TABLE mPreservedVariableTable[] = {
//  { SampleGuid, L"SampleVariable" },
//  { {0},   NULL}
//};

//
// New funcion OemSvcVariablePreservedTable2
// The followings are the sample codes for new function OemSvcVariablePreservedTable2 . Please customize here.
//
// PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_GUID : Other all variable GUID
// PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_NAME : Other all variable name
// If DeleteVariable is FALSE : Variable reserve.
// If DeleteVariable is TRUE  : Variable remove.
// The order of priority within the table is from top to bottom.
// The special item "PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_GUID, FALSE or TRUE, PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_NAME" needs to be placed at the last
// and before the end of table line.
// #define SampleGuid { 0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 }
//
//
//PRESERVED_VARIABLE_TABLE_2 mPreservedVariableTable2[] = {
//  VariableGuid    DeleteVariable    VariableName
//  { SampleGuid,            FALSE,   L"SampleVariable" },
//  { PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_GUID, FALSE, PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_NAME},   // put at here, other all variable GUID and Other all variable name
//  { {0}, FALSE, NULL}   // End of table
//};

PRESERVED_VARIABLE_TABLE_2 mPreservedVariableTable2[] = {
//  VariableGuid    DeleteVariable    VariableName
  { {0x1DA748E5, 0x3C39, 0x43E6, {0xB7, 0xAB, 0x55, 0xBA, 0xC7, 0x1A, 0xC7, 0xD2}}, FALSE, L"SetupChange"}, // gH2OSetupChangeVariableGuid
  { {0x7f9102df, 0xe999, 0x4740, {0x80, 0xa6, 0xb2, 0x03, 0x85, 0x12, 0x21, 0x7b}}, FALSE, L"SystemSupervisorPw"}, // gEfiSupervisorPwGuid, DeleteVariable = FALSE to keep kernel behavior that flash BIOS doesn't clean password when password in stored in variable.
  { {0x8cf3cfd3, 0xd8e2, 0x4e30, {0x83, 0xff, 0xb8, 0x6f, 0x0c, 0x52, 0x2a, 0x5e}}, FALSE, L"SystemUserPw"}, // gEfiUserPwGuid, DeleteVariable = FALSE to keep kernel behavior that flash BIOS doesn't clean password when password in stored in variable.
  { {0xE3539E26, 0x105D, 0x4068, {0xA9, 0x24, 0x9B, 0x11, 0xF3, 0xA2, 0xF1, 0xE6}}, FALSE, L"AMD_MODS_RES"},
  { {0xe83f7603, 0xa8ed, 0x4b9a, {0x8c, 0x16, 0xb0, 0x15, 0x29, 0xf6, 0x4b, 0x4f}}, FALSE, UPDATE_PROGRESS_NAME}, // gH2OSeamlessRecoveryGuid, DeleteVariable = FALSE to keep kernel behavior that flash BIOS doesn't clean the update progress stored in variable.
  { {0xfa07a1ea, 0x486d, 0x4b5e, {0xb9, 0x4d, 0x8e, 0x06, 0xe6, 0xfa, 0xd9, 0xdb}}, FALSE, BACKUP_SBB_FILE_DIGEST_SHA256_NAME}, // gH2OSeamlessRecoveryDigestGuid, DeleteVariable = FALSE to keep kernel behavior that flash BIOS doesn't clean the digest stored in variable.
  { {0x92E59835, 0x5F42, 0x4E0B, {0x9A, 0x84, 0x47, 0xc7, 0x81, 0x0E, 0xA8, 0x06}}, FALSE, L"IhisiParamBuffer"}, 
//[-start-240919-IB20840095-modify]//
#if defined (DEFAULT_SUPERVISOR_PW_SUPPORTED)
  { {0xA04A27f4, 0xDF00, 0x4D42, {0xB5, 0x52, 0x39, 0x51, 0x13, 0x02, 0x11, 0x3D}}, FALSE, L"DefSetupPswd"},
#endif
//[-end-240919-IB20840095-modify]//
  { PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_GUID, TRUE, PRESERVED_VARIABLE_TABLE_2_OTHER_ALL_NAME},   // Put at here, Other all variable GUID and Other all variable name
  { {0}, FALSE, NULL}   // End of table
};
/**
  New function OemSvcVariablePreservedTable2 to return new structure (PRESERVED_VARIABLE_TABLE_2) preserved list.
  The process will launch new function OemSvcVariablePreservedTable2 first,
  and then launch old function OemSvcVariablePreservedTable only when new function OemSvcVariablePreservedTable2
  return EFI_UNSUPPORTED.
  According to logo requirement, should preserve all UEFI variables with VendorGuid
  {77fa9abd-0359-4d32-bd60-28f4e78f784b}, so the those UEFI variables with this VendorGuidVendorGuid
  will not be limited in this table when if table is filled with those special variables to remove in table, 
  this table parse process will skip those.

  @param[in, out]  VariablePreservedTable2    Pointer to the table of preserved variables.
                                              (new structure PRESERVED_VARIABLE_TABLE_2)
  @retval          EFI_UNSUPPORTED           Returns unsupported by default.
  @retval          EFI_SUCCESS               The work to delete the variables is completed.
  @retval          EFI_MEDIA_CHANGED         The table of preserved variables is updated.
**/
EFI_STATUS
OemSvcVariablePreservedTable2 (
  IN OUT PRESERVED_VARIABLE_TABLE_2            **VariablePreservedTable2
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/

  //
  // The followings are the sample codes. Please customize here.
  //
 
  *VariablePreservedTable2 = mPreservedVariableTable2;
  return EFI_MEDIA_CHANGED;

  //return EFI_UNSUPPORTED;
}

/**
  Old function OemSvcVariablePreservedTable to return the old sturcture (PRESERVED_VARIABLE_TABLE) preserved list.
  The process will launch new function OemSvcVariablePreservedTable2 first,
  and then launch old function OemSvcVariablePreservedTable only when new function OemSvcVariablePreservedTable2
  return EFI_UNSUPPORTED.
  According to logo requirement, should preserve all UEFI variables with VendorGuid
  {77fa9abd-0359-4d32-bd60-28f4e78f784b}, so the those UEFI variables with this VendorGuidVendorGuid
  will not be limited in this table when if table is filled with those special variables to remove in table, 
  this table parse process will skip those.

  @param[in, out]  VariablePreservedTable    Pointer to the table of preserved variables.
                                             (old structure PRESERVED_VARIABLE_TABLE)
  @param[in, out]  IsKeepVariableInList      True: The variables in the table should be preserved.
                                             FALSE: The variables in the table should be deleted.

  @retval          EFI_UNSUPPORTED           Returns unsupported by default.
  @retval          EFI_SUCCESS               The work to delete the variables is completed.
  @retval          EFI_MEDIA_CHANGED         The table of preserved variables is updated.
**/
EFI_STATUS
OemSvcVariablePreservedTable (
  IN OUT PRESERVED_VARIABLE_TABLE              **VariablePreservedTable,
  IN OUT BOOLEAN                               *IsKeepVariableInList
  )
{
  /*++
    Todo:
      Add project specific code in here.
  --*/

  //
  // The followings are the sample codes. Please customize here.
  //
  //*IsKeepVariableInList = TRUE;
  //*VariablePreservedTable = mPreservedVariableTable;
  //return EFI_MEDIA_CHANGED;

  return EFI_UNSUPPORTED;
}
