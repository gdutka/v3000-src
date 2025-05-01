/** @file
  This library provides Helper functions for HddPassword Table
;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_HDDPASSWORD_LIB_H_
#define _H2O_HDDPASSWORD_LIB_H_
#include <Guid/HddPasswordVariable.h>
#include <Library/H2OCpLib.h>

/**
  Returns the next entry of the HDD password table.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.

  @retval           The pointer to the next table.

**/
HDD_PASSWORD_TABLE *
GetNextTableEntry (
  IN  HDD_PASSWORD_TABLE                *HddPasswordTablePtr
  );


/**
  Returns the next entry of the HDD password table.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.

  @retval           The pointer to the next table.

**/
HDD_PASSWORD_TABLE *
GetTableEntryWithIndex (
  IN  HDD_PASSWORD_TABLE                *HddPasswordTablePtr,
  IN  UINTN                             Index
  );



/**
  Zero out HddPasswordTable by policy.

  @param[in]        HddPasswordTable         Point to HddPasswordTable.
  @param[in]        HddPasswordTableSize     Size of HddPasswordTable.

**/
VOID
ZeroMemByPolicy (
  IN  HDD_PASSWORD_TABLE                    *HddPasswordTable,
  IN  UINTN                                 HddPasswordTableSize
  );


/**
  Return ExtDataSize.

  @retval           The Size of ExtData is going to be used
**/
UINT16
GetExtDataSize (
  VOID
  );



/**
  extract device path from end of each Hddpassword table.

  @param[in]        HddPasswordTable         Point to HddPasswordTable.

  @retval           The pointer to devicepath.

**/
EFI_DEVICE_PATH_PROTOCOL*
ExtractDpFromHddPasswordTable (
  IN  HDD_PASSWORD_TABLE                    *HddPasswordTable
  );


/**
  Calculate the number of Hdd password tables.

  @param[in]        HddPasswordTable         Pointer to HddPasswordTable.
  @param[in]        HddPasswordTableSize     HddPasswordTable size.

  @retval The number of tables.

**/
UINT32
NumOfHddPasswordTable (
  IN  HDD_PASSWORD_TABLE                  *HddPasswordTable,
  IN  UINTN                               HddPasswordTableSize
  );


/**
  Internal function to  trigger checkpoint.
  @param[out]        HddPasswordTable               HddPassword table that will output to caller
  @param[out]        HddPasswordTableSize              Size of Hddpassword table which output to caller

  @retval EFI_SUCCESS             Trigger boot device select checkpoint successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
**/
EFI_STATUS
TriggerCpHddPasswordGetTable (
  IN OUT  HDD_PASSWORD_TABLE                    **HddPasswordTable,
  IN OUT  UINT32                                *HddPasswordCount
  );


/**
  Internal function to  trigger checkpoint.

  @retval EFI_SUCCESS             Trigger boot device select checkpoint successfully.
  @retval EFI_OUT_OF_RESOURCES    Allocate memory to initialize checkpoint data failed.
**/
EFI_STATUS
TriggerCpHddPasswordSetTable (
  IN HDD_PASSWORD_TABLE                     *HddPasswordTable,
  IN OUT  UINT32                            HddPasswordCount
  );


/**
  To install HDD password services for all supported devices.

  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBaseCpHddPasswordGetTableCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  );


/**
  To install HDD password services for all supported devices.

  @param[in]        Event               Pointer to this event
  @param[in]        Handle              The handle associated with a previously registered checkpoint handler.

**/
VOID
H2OBaseCpHddPasswordSetTableCallback (
  IN EFI_EVENT                              Event,
  IN H2O_CP_HANDLE                          Handle
  );

/**
  Get HDDPassword Table size

  @param[in]        NumOfHdd     The number of HDDs.
  @param[in]        TableSize    The size of table allocated.
  @param[in]        HddPasswordTableSize  The size of HddPassword table 
  @param[in]        HddInfoArray          Hdd password info pointer 

  @retval           Tablesize                This Hddpassword is valid.
**/
UINT32
GetHddPasswordTableSize (
  IN     UINTN                          HddPasswordCount,
  IN     HDD_PASSWORD_TABLE             *HddPasswordTable
  );

#endif

