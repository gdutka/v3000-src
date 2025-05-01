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
#include <Library/GnbPcieConfigLib.h>
#include <Library/NbioTableBlastLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/PcdLib.h>
#include <Library/PcieConfigLib.h>
#include <Filecode.h>

#define FILECODE LIBRARY_NBIOTABLEBLASTLIB_NBIOTABLEBLASTLIB_FILECODE
#define PCD_TYPE_UNKNOWN 0xFF

extern EFI_GUID gEfiAmdAgesaModulePkgTokenSpaceGuid;
extern EFI_GUID gEfiAmdAgesaPkgTokenSpaceGuid;

static EFI_GUID* mPcdTokenSpaceGuidList[]= {
  &gEfiAmdAgesaModulePkgTokenSpaceGuid,
  &gEfiAmdAgesaPkgTokenSpaceGuid
  };
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


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
 * Supporting function for register read modify write in NBIO space
 *
 * @param[in] GnbHandle                 Gnb handle
 * @param[in] Data                      Data pointer
 * @param[in] Flags                     Flags
 */

VOID
STATIC
GnbProcessTableRegisterRmwSimpleSpace (
  IN      GNB_HANDLE                     *GnbHandle,
  IN      GNB_RMW_BLOCK                  *Data,
  IN      UINT32                         Flags
  )
{
  UINT64  Value;
  Value = 0;
  SmnRegisterReadS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    Data->Address,
    &Value
    );
  Value = (Value & (~ (UINT64) Data->AndMask)) | Data->OrMask;
  SmnRegisterWriteS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    Data->Address,
    &Value,
    Flags
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Supporting function for register read modify write in NBIO space
 *
 * @param[in] GnbHandle                 Gnb handle
 * @param[in] Data                      Data pointer
 * @param[in] Flags                     Flags
 */

VOID
STATIC
GnbProcessTableRegisterRmwNbioSpace (
  IN      GNB_HANDLE                     *GnbHandle,
  IN      GNB_RMW_BLOCK                  *Data,
  IN      UINT32                         Flags
  )
{
  UINT64  Value;
  Value = 0;
  SmnRegisterReadS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, Data->Address),
    &Value
    );
  Value = (Value & (~ (UINT64) Data->AndMask)) | Data->OrMask;
  SmnRegisterWriteS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, Data->Address),
    &Value,
    Flags
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Supporting function for register read modify write in wrapper space
 *
 * @param[in] GnbHandle                 Gnb handle
 * @param[in] Wrapper                   Pcie wrapper
 * @param[in] Data                      Data pointer
 * @param[in] Flags                     Flags
 */

VOID
STATIC
GnbProcessTableRegisterRmwWrapperSpace (
  IN      GNB_HANDLE                     *GnbHandle,
  IN      PCIe_WRAPPER_CONFIG            *Wrapper,
  IN      GNB_RMW_BLOCK                  *Data,
  IN      UINT32                         Flags
  )
{
  UINT64  Value;
  Value = 0;
  SmnRegisterReadS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE(GnbHandle, Wrapper, Data->Address),
    &Value
    );
  Value = (Value & (~ (UINT64) Data->AndMask)) | Data->OrMask;
  SmnRegisterWriteS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    WRAP_SPACE(GnbHandle, Wrapper, Data->Address),
    &Value,
    Flags
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Supporting function for register read modify write in pcie port space
 *
 * @param[in] GnbHandle                 Gnb handle
 * @param[in] Wrapper                   Pcie wrapper
 * @param[in] PcieEngine                Pcie port
 * @param[in] Data                      Data pointer
 * @param[in] Flags                     Flags
 */

VOID
STATIC
GnbProcessTableRegisterRmwPortSpace (
  IN      GNB_HANDLE                     *GnbHandle,
  IN      PCIe_WRAPPER_CONFIG            *Wrapper,
  IN      PCIe_ENGINE_CONFIG             *PcieEngine,
  IN      GNB_RMW_BLOCK                  *Data,
  IN      UINT32                         Flags
  )
{
  UINT64  Value;
  Value = 0;
  SmnRegisterReadS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE(GnbHandle, Wrapper, PcieEngine->Type.Port.PortId % 8, Data->Address),
    &Value
    );
  Value = (Value & (~ (UINT64) Data->AndMask)) | Data->OrMask;
  SmnRegisterWriteS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE(GnbHandle, Wrapper, PcieEngine->Type.Port.PortId % 8, Data->Address),
    &Value,
    Flags
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Supporting function for register read modify write in NBIO space
 *
 * @param[in] GnbHandle                 Gnb handle
 * @param[in] Data                      Data pointer
 * @param[in] Flags                     Flags
 */

VOID
STATIC
GnbProcessTableRegisterRmwSdpMuxSpace (
  IN      GNB_HANDLE                     *GnbHandle,
  IN      GNB_RMW_BLOCK                  *Data,
  IN      UINT32                         Flags
  )
{
  UINT64  Value;
  Value = 0;
  SmnRegisterReadS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    SDPMUX_SPACE(GnbHandle, Data->Address),
    &Value
    );
  Value = (Value & (~ (UINT64) Data->AndMask)) | Data->OrMask;
  SmnRegisterWriteS (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    SDPMUX_SPACE(GnbHandle, Data->Address),
    &Value,
    Flags
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Determine what type of PCD is associated with a token, and then retrieve that value
 *
 * @param[in] Token    PCD Token Value
 *
 * @retval             The PCD value
 *
 */
UINT32
DeterminePcdValue (
  UINT32      Token
  )
{
  PCD_INFO                         PcdInfoData = {PCD_TYPE_UNKNOWN, 0, NULL};

#ifdef DynamicToDynamicEx
  EFI_GUID                         *Guid;
  UINT8                            Index;
//[-start-220425-IB14740237-add]//
  UINT8                            DebugPropertyMask;
//[-end-220425-IB14740237-add]//

  Guid = NULL;
  for (Index = 0; Index < sizeof(mPcdTokenSpaceGuidList) / sizeof(EFI_GUID*); Index++) {
    Guid = mPcdTokenSpaceGuidList[Index];

    IDS_HDT_CONSOLE(GNB_TRACE, "Token %x\n", Token);
//[-start-220425-IB14740237-modify]//
    DebugPropertyMask = PcdGet8 (PcdDebugPropertyMask);
    PcdSet8S (PcdDebugPropertyMask, 0x00);
    LibPcdGetInfoEx(Guid, Token, &PcdInfoData);
    PcdSet8S (PcdDebugPropertyMask, DebugPropertyMask);
//[-end-220425-IB14740237-modify]//
    if (PcdInfoData.PcdType != PCD_TYPE_UNKNOWN) {
      break;
    }
  }
  ASSERT(PcdInfoData.PcdType != PCD_TYPE_UNKNOWN);

  IDS_HDT_CONSOLE(GNB_TRACE, "%a : Getting PCD value of %a\n", __FUNCTION__, PcdInfoData.PcdName);

  switch (PcdInfoData.PcdType) {
  case PCD_TYPE_8:
    return LibPcdGetEx8(Guid, Token);
  case PCD_TYPE_16:
    return LibPcdGetEx16(Guid, Token);
  case PCD_TYPE_32:
    return LibPcdGetEx32(Guid, Token);
  case PCD_TYPE_BOOL:
    return LibPcdGetExBool(Guid, Token);
  default:
    IDS_HDT_CONSOLE (GNB_TRACE, "%a : PCD Type not supported\n", __FUNCTION__);
    ASSERT(FALSE);
    return 0;
  }
#else

  LibPcdGetInfo(Token, &PcdInfoData);

  IDS_HDT_CONSOLE(GNB_TRACE, "%a : Getting PCD value of %a\n", __FUNCTION__, PcdInfoData.PcdName);
  IDS_HDT_CONSOLE(GNB_TRACE, "Token %x\n", Token);

  switch (PcdInfoData.PcdType) {
  case PCD_TYPE_8:
    return LibPcdGet8(Token);
  case PCD_TYPE_16:
    return LibPcdGet16(Token);
  case PCD_TYPE_32:
    return LibPcdGet32(Token);
  case PCD_TYPE_BOOL:
    return LibPcdGetBool(Token);
  default:
    IDS_HDT_CONSOLE (GNB_TRACE, "%a : PCD Type not supported\n", __FUNCTION__);
    ASSERT(FALSE);
    return 0;
  }
#endif
}

/*----------------------------------------------------------------------------------------*/
/**
 * Take care of all NBIO_TYPE entries.
 *
 * @param[in] EntryPointer    Pointer to beginning byte of entry
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 */
UINTN
SimpleTypeHandler(
 IN  UINT8       *EntryPointer,
 IN  GNB_HANDLE  *GnbHandle,
 IN  UINT32      WriteAccFlags,
 IN  UINT32      Property
  )
{
  UINTN                            EntrySize;
  GNB_TABLE_ENTRY_PCD_RMW          *PcdRmwEntry;
  GNB_TABLE_ENTRY_PCD_WR           *PcdWrEntry;
  BOOLEAN                          BlastedEntry;

  EntrySize = 0;
  BlastedEntry = FALSE;

  switch (*EntryPointer) {
  case GnbEntryWr:
    SmnRegisterWriteS (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      ((GNB_TABLE_ENTRY_WR*) EntryPointer)->Address,
      &((GNB_TABLE_ENTRY_WR*) EntryPointer)->Value,
      WriteAccFlags
      );
    EntrySize = sizeof (GNB_TABLE_ENTRY_WR);
    break;
  case GnbEntryRmw:
    GnbProcessTableRegisterRmwNbioSpace (
      GnbHandle,
      &((GNB_TABLE_ENTRY_RMW *) EntryPointer)->Data,
      WriteAccFlags
      );
    EntrySize = sizeof (GNB_TABLE_ENTRY_RMW);
    break;
  case GnbEntryPcdWr:
    PcdWrEntry = (GNB_TABLE_ENTRY_PCD_WR *)EntryPointer;
    IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_WR. Token value %x , PCD Operation Type %d\n", PcdWrEntry->Token, PcdWrEntry->PcdType);
    if ((PcdWrEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdWrEntry->Token) == PcdWrEntry->CompareValue)) {
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        (PcdWrEntry->Address),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if ((PcdWrEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdWrEntry->Token) != PcdWrEntry->CompareValue)) {
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        (PcdWrEntry->Address),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if ((PcdWrEntry->PcdType == TableBlastWriteValue)) {
      PcdWrEntry->Value = DeterminePcdValue(PcdWrEntry->Token);
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        (PcdWrEntry->Address),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if (BlastedEntry) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD WR entry.\n");
    }
    EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_WR);
    break;
  case GnbEntryPcdRmw:
    PcdRmwEntry = (GNB_TABLE_ENTRY_PCD_RMW *)EntryPointer;
    // IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_RMW. Token value %x , PCD Operation Type %d\n", PcdRmwEntry->Token, PcdRmwEntry->PcdType);
    if ((PcdRmwEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdRmwEntry->Token) == PcdRmwEntry->CompareValue)) {
      GnbProcessTableRegisterRmwSimpleSpace (
        GnbHandle,
        &(PcdRmwEntry->Data),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if ((PcdRmwEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdRmwEntry->Token) != PcdRmwEntry->CompareValue)) {
      GnbProcessTableRegisterRmwSimpleSpace (
        GnbHandle,
        &(PcdRmwEntry->Data),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if (BlastedEntry) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD RMW entry.\n");
    }
    /// Check for an invalid PCD RMW Instruction(s) to prevent
    /// developer mistakes
    ASSERT(PcdRmwEntry->PcdType != TableBlastWriteValue);
    EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_RMW);
    break;
  case GnbEntryPropertyRmw:
    if ((Property & ((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property) != 0) {
      GnbProcessTableRegisterRmwSimpleSpace (
        GnbHandle,
        &((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Data,
        WriteAccFlags
        );
    }
    EntrySize = sizeof (GNB_TABLE_ENTRY_PROPERTY_RMW);
    break;
  default:
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
    ASSERT (FALSE);
    return 0;
  }

  return EntrySize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Take care of all NBIO_TYPE entries.
 *
 * @param[in] EntryPointer    Pointer to beginning byte of entry
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 */
UINTN
NbioTypeHandler(
 IN  UINT8       *EntryPointer,
 IN  GNB_HANDLE  *GnbHandle,
 IN  UINT32      WriteAccFlags,
 IN  UINT32      Property
  )
{
  UINTN                            EntrySize;
  GNB_TABLE_ENTRY_PCD_RMW          *PcdRmwEntry;
  GNB_TABLE_ENTRY_PCD_WR           *PcdWrEntry;
  BOOLEAN                          BlastedEntry;

  EntrySize = 0;
  BlastedEntry = FALSE;

  switch (*EntryPointer) {
  case GnbEntryWr:
    SmnRegisterWriteS (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, ((GNB_TABLE_ENTRY_WR*) EntryPointer)->Address),
      &((GNB_TABLE_ENTRY_WR*) EntryPointer)->Value,
      WriteAccFlags
      );
    EntrySize = sizeof (GNB_TABLE_ENTRY_WR);
    break;
  case GnbEntryRmw:
    GnbProcessTableRegisterRmwNbioSpace (
      GnbHandle,
      &((GNB_TABLE_ENTRY_RMW *) EntryPointer)->Data,
      WriteAccFlags
      );
    EntrySize = sizeof (GNB_TABLE_ENTRY_RMW);
    break;
  case GnbEntryPcdWr:
    PcdWrEntry = (GNB_TABLE_ENTRY_PCD_WR *)EntryPointer;
    IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_WR. Token value %x , PCD Operation Type %d\n", PcdWrEntry->Token, PcdWrEntry->PcdType);
    if ((PcdWrEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdWrEntry->Token) == PcdWrEntry->CompareValue)) {
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, (PcdWrEntry->Address)),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if ((PcdWrEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdWrEntry->Token) != PcdWrEntry->CompareValue)) {
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, (PcdWrEntry->Address)),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if ((PcdWrEntry->PcdType == TableBlastWriteValue)) {
      PcdWrEntry->Value = DeterminePcdValue(PcdWrEntry->Token);
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, (PcdWrEntry->Address)),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if (BlastedEntry) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD WR entry.\n");
    }
    EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_WR);
    break;
  case GnbEntryPcdRmw:
    PcdRmwEntry = (GNB_TABLE_ENTRY_PCD_RMW *)EntryPointer;
    // IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_RMW. Token value %x , PCD Operation Type %d\n", PcdRmwEntry->Token, PcdRmwEntry->PcdType);
    if ((PcdRmwEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdRmwEntry->Token) == PcdRmwEntry->CompareValue)) {
      GnbProcessTableRegisterRmwNbioSpace (
        GnbHandle,
        &(PcdRmwEntry->Data),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if ((PcdRmwEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdRmwEntry->Token) != PcdRmwEntry->CompareValue)) {
      GnbProcessTableRegisterRmwNbioSpace (
        GnbHandle,
        &(PcdRmwEntry->Data),
        WriteAccFlags
        );
      BlastedEntry = TRUE;
    }
    if (BlastedEntry) {
      IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD RMW entry.\n");
    }
    /// Check for an invalid PCD RMW Instruction(s) to prevent
    /// developer mistakes
    ASSERT(PcdRmwEntry->PcdType != TableBlastWriteValue);
    EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_RMW);
    break;
  case GnbEntryPropertyRmw:
    if ((Property & ((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property) != 0) {
      GnbProcessTableRegisterRmwNbioSpace (
        GnbHandle,
        &((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Data,
        WriteAccFlags
        );
    }
    EntrySize = sizeof (GNB_TABLE_ENTRY_PROPERTY_RMW);
    break;
  default:
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
    ASSERT (FALSE);
    return 0;
  }

  return EntrySize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Take care of all ALL_WRAP_TYPE entries.
 *
 * @param[in] EntryPointer    Pointer to beginning byte of entry
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 */
UINTN
WrapperTypeHandler(
 IN  UINT8       *EntryPointer,
 IN  GNB_HANDLE  *GnbHandle,
 IN  UINT32      WriteAccFlags,
 IN  UINT32      Property
  )
{
  UINTN                            EntrySize;
  PCIe_WRAPPER_CONFIG              *PcieWrapper;
  GNB_TABLE_ENTRY_PCD_RMW          *PcdRmwEntry;
  GNB_TABLE_ENTRY_PCD_WR           *PcdWrEntry;
  BOOLEAN                          BlastedEntry;

  EntrySize = 0;
  BlastedEntry = FALSE;
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  //
  // Perform this action to all enabled pcie wrappers
  //
  while ((PcieWrapper != NULL) && (PcieWrapper->WrapId < 2)) {
    switch (*EntryPointer) {
    case GnbEntryWr:
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        WRAP_SPACE(GnbHandle, PcieWrapper, ((GNB_TABLE_ENTRY_WR*) EntryPointer)->Address),
        &((GNB_TABLE_ENTRY_WR*) EntryPointer)->Value,
        WriteAccFlags
        );
      EntrySize = sizeof (GNB_TABLE_ENTRY_WR);
      break;
    case GnbEntryRmw:
      GnbProcessTableRegisterRmwWrapperSpace (
        GnbHandle,
        PcieWrapper,
        &((GNB_TABLE_ENTRY_RMW *) EntryPointer)->Data,
        WriteAccFlags
        );
      EntrySize = sizeof (GNB_TABLE_ENTRY_RMW);
      break;
    case GnbEntryPcdWr:
      PcdWrEntry = (GNB_TABLE_ENTRY_PCD_WR *)EntryPointer;
      IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_WR. Token value %x , PCD Operation Type %d\n", PcdWrEntry->Token, PcdWrEntry->PcdType);
      if ((PcdWrEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdWrEntry->Token) == PcdWrEntry->CompareValue)) {
        SmnRegisterWriteS (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          WRAP_SPACE(GnbHandle, PcieWrapper, (PcdWrEntry->Address)),
          &(PcdWrEntry->Value),
          WriteAccFlags
          );
        BlastedEntry = TRUE;
      }
      if ((PcdWrEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdWrEntry->Token) != PcdWrEntry->CompareValue)) {
        SmnRegisterWriteS (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          WRAP_SPACE(GnbHandle, PcieWrapper, (PcdWrEntry->Address)),
          &(PcdWrEntry->Value),
          WriteAccFlags
          );
        BlastedEntry = TRUE;
      }
      if ((PcdWrEntry->PcdType == TableBlastWriteValue)) {
        PcdWrEntry->Value = DeterminePcdValue(PcdWrEntry->Token);
        SmnRegisterWriteS (
          GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          WRAP_SPACE(GnbHandle, PcieWrapper, (PcdWrEntry->Address)),
          &(PcdWrEntry->Value),
          WriteAccFlags
          );
        BlastedEntry = TRUE;
      }
      if (BlastedEntry) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD WR entry.\n");
      }
      EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_WR);
      break;
    case GnbEntryPcdRmw:
      PcdRmwEntry = (GNB_TABLE_ENTRY_PCD_RMW *)EntryPointer;
      // IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_RMW. Token value %x , PCD Operation Type %d\n", PcdRmwEntry->Token, PcdRmwEntry->PcdType);
      if ((PcdRmwEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdRmwEntry->Token) == PcdRmwEntry->CompareValue)) {
        GnbProcessTableRegisterRmwWrapperSpace (
          GnbHandle,
          PcieWrapper,
          &(PcdRmwEntry->Data),
          WriteAccFlags
          );
        BlastedEntry = TRUE;
      }
      if ((PcdRmwEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdRmwEntry->Token) != PcdRmwEntry->CompareValue)) {
        GnbProcessTableRegisterRmwWrapperSpace (
          GnbHandle,
          PcieWrapper,
          &(PcdRmwEntry->Data),
          WriteAccFlags
          );
        BlastedEntry = TRUE;
      }
      if (BlastedEntry) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD RMW entry.\n");
      }
      /// Check for an invalid PCD RMW Instruction(s) to prevent
      /// developer mistakes.
      ASSERT(PcdRmwEntry->PcdType != TableBlastWriteValue);
      EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_RMW);
      break;
    case GnbEntryPropertyRmw:
      if ((Property & ((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property) != 0) {
        GnbProcessTableRegisterRmwWrapperSpace (
          GnbHandle,
          PcieWrapper,
          &((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Data,
          WriteAccFlags
          );
      }
      EntrySize = sizeof (GNB_TABLE_ENTRY_PROPERTY_RMW);
      break;
    default:
      IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
      ASSERT (FALSE);
      return 0;
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  return EntrySize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Take care of all ALL_PORT_TYPE entries.
 *
 * @param[in] EntryPointer    Pointer to beginning byte of entry
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 */
UINTN
PortTypeHandler(
 IN  UINT8       *EntryPointer,
 IN  GNB_HANDLE  *GnbHandle,
 IN  UINT32      WriteAccFlags,
 IN  UINT32      Property
  )
{
  UINTN                            EntrySize;
  PCIe_WRAPPER_CONFIG              *PcieWrapper;
  PCIe_ENGINE_CONFIG               *PcieEngine;
  GNB_TABLE_ENTRY_PCD_RMW          *PcdRmwEntry;
  GNB_TABLE_ENTRY_PCD_WR           *PcdWrEntry;
  BOOLEAN                          BlastedEntry;

  EntrySize = 0;
  BlastedEntry = FALSE;
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  //
  // Perform this action to all enabled pcie wrappers
  //
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        switch (*EntryPointer) {
        case GnbEntryWr:
          SmnRegisterWriteS (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8, ((GNB_TABLE_ENTRY_WR*) EntryPointer)->Address),
            &((GNB_TABLE_ENTRY_WR*) EntryPointer)->Value,
            WriteAccFlags
            );
          EntrySize = sizeof (GNB_TABLE_ENTRY_WR);
          break;
        case GnbEntryRmw:
          GnbProcessTableRegisterRmwPortSpace (
            GnbHandle,
            PcieWrapper,
            PcieEngine,
            &((GNB_TABLE_ENTRY_RMW *) EntryPointer)->Data,
            WriteAccFlags
            );
          EntrySize = sizeof (GNB_TABLE_ENTRY_RMW);
          break;
        case GnbEntryPcdWr:
          PcdWrEntry = (GNB_TABLE_ENTRY_PCD_WR *)EntryPointer;
          IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_WR. Token value %x , PCD Operation Type %d\n", PcdWrEntry->Token, PcdWrEntry->PcdType);
          if ((PcdWrEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdWrEntry->Token) == PcdWrEntry->CompareValue)) {
            SmnRegisterWriteS (
              GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8, (PcdWrEntry->Address)),
              &(PcdWrEntry->Value),
              WriteAccFlags
              );
            BlastedEntry = TRUE;
          }
          if ((PcdWrEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdWrEntry->Token) != PcdWrEntry->CompareValue)) {
            SmnRegisterWriteS (
              GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8, (PcdWrEntry->Address)),
              &(PcdWrEntry->Value),
              WriteAccFlags
              );
            BlastedEntry = TRUE;
          }
          if ((PcdWrEntry->PcdType == TableBlastWriteValue)) {
            PcdWrEntry->Value = DeterminePcdValue(PcdWrEntry->Token);
            SmnRegisterWriteS (
              GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8, (PcdWrEntry->Address)),
              &(PcdWrEntry->Value),
              WriteAccFlags
              );
            BlastedEntry = TRUE;
          }
          if (BlastedEntry) {
            IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD WR entry.\n");
          }
          EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_WR);
          break;
        case GnbEntryPcdRmw:
          PcdRmwEntry = (GNB_TABLE_ENTRY_PCD_RMW *)EntryPointer;
          // IDS_HDT_CONSOLE(GNB_TRACE, "  Read GNB_ENTRY_PCD_RMW. Token value %x , PCD Operation Type %d\n", PcdRmwEntry->Token, PcdRmwEntry->PcdType);
          if ((PcdRmwEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdRmwEntry->Token) == PcdRmwEntry->CompareValue)) {
            GnbProcessTableRegisterRmwPortSpace (
              GnbHandle,
              PcieWrapper,
              PcieEngine,
              &(PcdRmwEntry->Data),
              WriteAccFlags
              );
            BlastedEntry = TRUE;
          }
          if ((PcdRmwEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdRmwEntry->Token) != PcdRmwEntry->CompareValue)) {
            GnbProcessTableRegisterRmwPortSpace (
              GnbHandle,
              PcieWrapper,
              PcieEngine,
              &(PcdRmwEntry->Data),
              WriteAccFlags
              );
            BlastedEntry = TRUE;
          }
          if (BlastedEntry) {
            IDS_HDT_CONSOLE (GNB_TRACE, "Blasted PCD RMW entry.\n");
          }
          /// Check for an invalid PCD RMW Instruction(s) to prevent
          /// developer mistakes.
          ASSERT(PcdRmwEntry->PcdType != TableBlastWriteValue);
          EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_RMW);
          break;
        case GnbEntryPropertyRmw:
          if ((Property & ((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property) != 0) {
            GnbProcessTableRegisterRmwPortSpace (
              GnbHandle,
              PcieWrapper,
              PcieEngine,
              &((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Data,
              WriteAccFlags
              );
          }
          EntrySize = sizeof (GNB_TABLE_ENTRY_PROPERTY_RMW);
          break;
        default:
          IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
          ASSERT (FALSE);
          return 0;
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  return EntrySize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Take care of all OTHER_TYPE entries.
 *
 * @param[in] EntryPointer    Pointer to beginning byte of entry
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 */
UINTN
OtherTypeHandler (
 IN  UINT8       *EntryPointer,
 IN  GNB_HANDLE  *GnbHandle,
 IN  UINT32      WriteAccFlags,
 IN  UINT32      Property
  )
{
  UINTN                            EntrySize;
  GNB_TABLE_ENTRY_CPU_DEAD_LOOP    *CpuDeadLoopEntry;

  EntrySize = 0;
  switch (*EntryPointer) {
  case GnbEntryCpuDeadLoop:
    CpuDeadLoopEntry = (GNB_TABLE_ENTRY_CPU_DEAD_LOOP *)EntryPointer;
    IDS_HDT_CONSOLE(GNB_TRACE, "%a : Hit GnbEntryCpuDeadLoop. Halting Execution @ DebugId %d.\n", __FUNCTION__, CpuDeadLoopEntry->DebugId);
    CpuDeadLoop();
    EntrySize = sizeof (GNB_TABLE_ENTRY_CPU_DEAD_LOOP);
    break;
  case GnbEntryStall:
    IDS_HDT_CONSOLE (GNB_TRACE, "%a : STALL is not needed nor implemented as of now.\n", __FUNCTION__);
    ASSERT(FALSE);
    EntrySize = sizeof (GNB_TABLE_ENTRY_STALL);
    break;
  default:
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
    ASSERT (FALSE);
    return 0;
  }

  return EntrySize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Take care of all SDPMUX_TYPE entries.
 *
 * @param[in] EntryPointer    Pointer to beginning byte of entry
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 */
UINTN
SdpMuxTypeHandler(
 IN  UINT8       *EntryPointer,
 IN  GNB_HANDLE  *GnbHandle,
 IN  UINT32      WriteAccFlags,
 IN  UINT32      Property
  )
{
  UINTN                            EntrySize;
  GNB_TABLE_ENTRY_PCD_RMW          *PcdRmwEntry;
  GNB_TABLE_ENTRY_PCD_WR           *PcdWrEntry;

  EntrySize = 0;

  switch (*EntryPointer) {
  case GnbEntryWr:
    SmnRegisterWriteS (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      SDPMUX_SPACE(GnbHandle, ((GNB_TABLE_ENTRY_WR*) EntryPointer)->Address),
      &((GNB_TABLE_ENTRY_WR*) EntryPointer)->Value,
      WriteAccFlags
      );
    EntrySize = sizeof (GNB_TABLE_ENTRY_WR);
    break;
  case GnbEntryRmw:
    GnbProcessTableRegisterRmwSdpMuxSpace (
      GnbHandle,
      &((GNB_TABLE_ENTRY_RMW *) EntryPointer)->Data,
      WriteAccFlags
      );
    EntrySize = sizeof (GNB_TABLE_ENTRY_RMW);
    break;
  case GnbEntryPcdWr:
    PcdWrEntry = (GNB_TABLE_ENTRY_PCD_WR *)EntryPointer;
    if ((PcdWrEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdWrEntry->Token) == PcdWrEntry->CompareValue)) {
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        SDPMUX_SPACE(GnbHandle, (PcdWrEntry->Address)),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
    }
    if ((PcdWrEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdWrEntry->Token) != PcdWrEntry->CompareValue)) {
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        SDPMUX_SPACE(GnbHandle, (PcdWrEntry->Address)),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
    }
    if ((PcdWrEntry->PcdType == TableBlastWriteValue)) {
      PcdWrEntry->Value = DeterminePcdValue(PcdWrEntry->Token);
      SmnRegisterWriteS (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        SDPMUX_SPACE(GnbHandle, (PcdWrEntry->Address)),
        &(PcdWrEntry->Value),
        WriteAccFlags
        );
    }
    EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_WR);
    break;
  case GnbEntryPcdRmw:
    PcdRmwEntry = (GNB_TABLE_ENTRY_PCD_RMW *)EntryPointer;
    if ((PcdRmwEntry->PcdType == TableBlastWriteIfValEqual) && (DeterminePcdValue(PcdRmwEntry->Token) == PcdRmwEntry->CompareValue)) {
      GnbProcessTableRegisterRmwSdpMuxSpace (
        GnbHandle,
        &(PcdRmwEntry->Data),
        WriteAccFlags
        );
    }
    if ((PcdRmwEntry->PcdType == TableBlastWriteIfValNotEqual) && (DeterminePcdValue(PcdRmwEntry->Token) != PcdRmwEntry->CompareValue)) {
      GnbProcessTableRegisterRmwSdpMuxSpace (
        GnbHandle,
        &(PcdRmwEntry->Data),
        WriteAccFlags
        );
    }
    /// Check for an invalid PCD RMW Instruction(s) to prevent
    /// developer mistakes
    ASSERT(PcdRmwEntry->PcdType != TableBlastWriteValue);
    EntrySize = sizeof(GNB_TABLE_ENTRY_PCD_RMW);
    break;
  case GnbEntryPropertyRmw:
    if ((Property & ((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Property) != 0) {
      GnbProcessTableRegisterRmwSdpMuxSpace (
        GnbHandle,
        &((GNB_TABLE_ENTRY_PROPERTY_RMW *) EntryPointer)->Data,
        WriteAccFlags
        );
    }
    EntrySize = sizeof (GNB_TABLE_ENTRY_PROPERTY_RMW);
    break;
  default:
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Register table parse\n");
    ASSERT (FALSE);
    return 0;
  }

  return EntrySize;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Process table
 *
 * @param[in] GnbHandle       Gnb handle
 * @param[in] Table           Table pointer
 * @param[in] Property        Property
 * @param[in] Flags           Flags
 * @param[in] StdHeader       Standard configuration header
 */

AGESA_STATUS
GnbBlastTable (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GNB_TABLE             *Table,
  IN      UINT32                Property,
  IN      UINT32                Flags,
  IN      AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT8     *EntryPointer;
  UINT32    WriteAccFlags;
  UINTN     EntrySize;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  IDS_HDT_CONSOLE (GNB_TRACE, " Property - 0x%08x\n", Property);

  EntryPointer = (UINT8 *) Table;
  WriteAccFlags = 0;
  EntrySize = 0;

  if ((Flags & NBIO_TABLE_FLAGS_FORCE_S3_SAVE) != 0) {
    WriteAccFlags |= GNB_REG_ACC_FLAG_S3SAVE;
  }

  while (*EntryPointer != GnbEntryTerminate) {
    ///
    /// Actions to execute per NBIO
    ///
    if (EntryPointer[1] == ALL_NBIO_TYPE || EntryPointer[1] == ONE_ADDR_TYPE) {
      EntrySize = NbioTypeHandler (
                    EntryPointer,
                    GnbHandle,
                    WriteAccFlags,
                    Property
                    );
    }
    if (EntryPointer[1] == ALL_WRAP_TYPE) {
      EntrySize = WrapperTypeHandler (
                    EntryPointer,
                    GnbHandle,
                    WriteAccFlags,
                    Property
                    );
    }
    if (EntryPointer[1] == SIMPLE_ADDR_TYPE) {
      EntrySize = SimpleTypeHandler (
                    EntryPointer,
                    GnbHandle,
                    WriteAccFlags,
                    Property
                    );
    }
    if (EntryPointer[1] == ALL_PORT_TYPE) {
      EntrySize = PortTypeHandler (
                    EntryPointer,
                    GnbHandle,
                    WriteAccFlags,
                    Property
                    );
    }
    if (EntryPointer[1] == OTHER_TYPE) {
      EntrySize = OtherTypeHandler (
                    EntryPointer,
                    GnbHandle,
                    WriteAccFlags,
                    Property
                    );
    }
    if (EntryPointer[1] == ALL_SDPMUX_TYPE) {
      EntrySize = SdpMuxTypeHandler (
                    EntryPointer,
                    GnbHandle,
                    WriteAccFlags,
                    Property
                    );
    }

    ///
    /// Make sure table entry was processed. An incorrect type ID
    /// means the table is malformed
    ///
    if (EntryPointer[1] >= MAX_ADDR_TYPE) {
      IDS_HDT_CONSOLE (GNB_TRACE, "%a : Received unknown type %d\n", __FUNCTION__, EntryPointer[1]);
      ASSERT(FALSE);
    }

    EntryPointer = EntryPointer + EntrySize;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
  return AGESA_SUCCESS;
}

