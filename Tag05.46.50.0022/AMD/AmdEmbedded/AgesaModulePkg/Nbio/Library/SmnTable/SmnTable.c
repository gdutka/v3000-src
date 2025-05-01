/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/BaseLib.h>
#include <AGESA.h>
#include <Library/IdsLib.h>
#include <Library/AmdBaseLib.h>
#include <Gnb.h>
#include <Library/SmnTableLib.h>
#include <Library/SmnAccessLib.h>
#include <Filecode.h>

#define FILECODE NBIO_LIBRARY_SMNTABLE_SMNTABLE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define MODIFIED(address)  (address + Modifier)


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/**
 * Process table
 *
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 * @param[in] Flags           Flags
 * @param[in] Modifier        Table modifier to include NBIO/Wrapper/Port specific SMN info
 */

AGESA_STATUS
NbioSmnTable (
  IN      GNB_HANDLE            *GnbHandle,
  IN      SMN_TABLE             *Table,
  IN      UINT32                Modifier,
  IN      UINT32                Property,
  IN      UINT32                Flags
  )
{
  UINT8     *EntryPointer;
  UINT32    WriteAccFlags;
  UINTN     EntrySize;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  IDS_HDT_CONSOLE (GNB_TRACE, " Property - 0x%08x\n", Property);
  IDS_HDT_CONSOLE (GNB_TRACE, " Modifier - 0x%08x\n", Modifier);

  EntryPointer = (UINT8 *) Table;
  WriteAccFlags = 0;
  EntrySize = 0;

  // if ((Flags & NBIO_TABLE_FLAGS_FORCE_S3_SAVE) != 0) {
    // WriteAccFlags |= SMN_REG_ACC_FLAG_S3SAVE;
  // }

  while (*EntryPointer != SmnEntryTerminate) {
    switch (*EntryPointer) {
    case SmnEntryWr:
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        MODIFIED (((SMN_TABLE_ENTRY_WR*) EntryPointer)->Address),
        &((SMN_TABLE_ENTRY_WR*) EntryPointer)->Value,
        WriteAccFlags
        );
      EntrySize = sizeof (SMN_TABLE_ENTRY_WR);
      break;
    case SmnEntryRmw:
      SmnRegisterRMWS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        MODIFIED (((SMN_TABLE_ENTRY_WR*) EntryPointer)->Address),
        ~((SMN_TABLE_ENTRY_RMW*) EntryPointer)->AndMask,
        ((SMN_TABLE_ENTRY_RMW*) EntryPointer)->OrMask,
        WriteAccFlags
        );
      EntrySize = sizeof (SMN_TABLE_ENTRY_RMW);
      break;
    case SmnEntryPropertyWr:
      if ((Property & ((SMN_TABLE_ENTRY_PROPERTY_WR *) EntryPointer)->Property) == (((SMN_TABLE_ENTRY_PROPERTY_WR *) EntryPointer)->Property)) {
        SmnRegisterWriteS (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          MODIFIED (((SMN_TABLE_ENTRY_PROPERTY_WR*) EntryPointer)->Address),
          &((SMN_TABLE_ENTRY_PROPERTY_WR*) EntryPointer)->Value,
          WriteAccFlags
          );
      }
      EntrySize = sizeof (SMN_TABLE_ENTRY_PROPERTY_WR);
      break;
    case SmnEntryPropertyRmw:
      if ((Property & ((SMN_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property) == (((SMN_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property)) {
        SmnRegisterRMWS (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          MODIFIED (((SMN_TABLE_ENTRY_PROPERTY_WR*) EntryPointer)->Address),
          ~((SMN_TABLE_ENTRY_PROPERTY_RMW*) EntryPointer)->AndMask,
          ((SMN_TABLE_ENTRY_PROPERTY_RMW*) EntryPointer)->OrMask,
          WriteAccFlags
          );
      }
      EntrySize = sizeof (SMN_TABLE_ENTRY_PROPERTY_RMW);
      break;
    case SmnTableEntry:
      NbioSmnTable (GnbHandle, ((SMN_TABLE_ENTRY *) EntryPointer)->Address, Modifier, Property, Flags);
      EntrySize = sizeof (SMN_TABLE_ENTRY);
      break;
    case SmnTableEntryProperty:
      if ((Property & ((SMN_TABLE_ENTRY_PROPERTY *) EntryPointer)->Property) == (((SMN_TABLE_ENTRY_PROPERTY *) EntryPointer)->Property)) {
          NbioSmnTable (GnbHandle, ((SMN_TABLE_ENTRY_PROPERTY *) EntryPointer)->Address, Modifier, Property, Flags);
      }
      EntrySize = sizeof (SMN_TABLE_ENTRY_PROPERTY);
      break;
    default:
      IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
      ASSERT (FALSE);
    }
    ///
    /// Make sure table entry was processed. An incorrect type ID
    /// means the table is malformed
    ///
    // if (EntryPointer[1] >= MAX_ADDR_TYPE) {
      // IDS_HDT_CONSOLE (GNB_TRACE, "%a : Received unknown type %d\n", __FUNCTION__, EntryPointer[1]);
      // ASSERT(FALSE);
    // }
    EntryPointer = EntryPointer + EntrySize;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return AGESA_SUCCESS;
}

