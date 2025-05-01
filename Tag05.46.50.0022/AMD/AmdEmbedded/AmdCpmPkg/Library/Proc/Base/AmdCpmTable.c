/*****************************************************************************
 *
 * Copyright (C) 2012-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmBase.h>
#include <AmdCpmTable.h>
#include CPM_PPI_DEFINITION (AmdCpmTablePpi)

/*----------------------------------------------------------------------------------------*/
/**
 * Get the pointer of AMD CPM Table with the required signature.
 *
 * @param[in]     This        The Pointer of CPM Table Ppi or Protocol
 * @param[in]     TableId     CPM Table Signature
 *
 * @retval        The pointer of CPM table
 */
VOID *
EFIAPI
CpmGetTablePtr (
  IN       VOID                        *This,
  IN       UINT32                      TableId
  )
{
  return CpmSearchAndUpdateTable (This, TableId, 0);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the pointer of AMD CPM Table with the required signature. The table can be rewritable.
 *
 * @param[in]     This        The Pointer of CPM Table Ppi or Protocol
 * @param[in]     TableId     CPM Table Signature
 *
 * @retval        The pointer of CPM table
 */
VOID *
EFIAPI
CpmGetTablePtr2 (
  IN       VOID                        *This,
  IN       UINT32                      TableId
  )
{
  return CpmSearchAndUpdateTable (This, TableId, 3);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the pointer of AMD CPM Table with the required signature and update the status
 * in the table list.
 *
 * @param[in]     This        The Pointer of CPM Table Ppi or Protocol
 * @param[in]     TableId     CPM Table Signature
 * @param[in]     Flag        Table Status to be updated
 *                              0: Keep original status
 *                              1: Enable the table
 *                              2: Disable the table
 *                              3: Make the table to re-writable
 *                              4: Enable and Make the table to re-writable
 *
 * @retval        The pointer of CPM table
 */
VOID *
EFIAPI
CpmSearchAndUpdateTable (
  IN       VOID                        *This,
  IN       UINT32                      TableId,
  IN       UINT8                       Flag
  )
{
  AMD_CPM_MAIN_TABLE  *MainTablePtr;
  AMD_CPM_TABLE_LIST  *TableListPtr;
  AMD_CPM_HOB_HEADER  *HobTablePtr;
  AMD_CPM_TABLE_COMMON_HEADER *TableHeaderPtr;
  AMD_CPM_COMMON_FUNCTION      *CommonFunctionPtr;
  AMD_CPM_PEIM_PUBLIC_FUNCTION *PeimPublicFunctionPtr;
  UINT32              PlatformMask;
  UINT32              i;

  MainTablePtr    = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  CommonFunctionPtr     = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;
  PeimPublicFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->PeimPublicFunction;
  PlatformMask    = 1 << MainTablePtr->CurrentPlatformId;
  TableListPtr    = MainTablePtr->TableInHobList.Pointer;
  HobTablePtr     = MainTablePtr->HobTablePtr.Pointer;
  DEBUG((DEBUG_INFO, " LIB-TBL-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, " This %p", This));
  DEBUG((DEBUG_INFO, " TableId %08X Flag %d\n", TableId, Flag));

  DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
  DEBUG((DEBUG_INFO, " MainTablePtr %p", MainTablePtr));
  DEBUG((DEBUG_INFO, " CommonFunctionPtr %p", CommonFunctionPtr));
  DEBUG((DEBUG_INFO, " PeimPublicFunctionPtr %p", PeimPublicFunctionPtr));
  DEBUG((DEBUG_INFO, " TableListPtr %p", TableListPtr));
  DEBUG((DEBUG_INFO, " HobTablePtr %p", HobTablePtr));
  DEBUG((DEBUG_INFO, " PlatformMask %08X Number %d\n", PlatformMask, TableListPtr->Number));

  if (TableListPtr) {
    for (i = 0; i < TableListPtr->Number; i++) {
      if (TableListPtr->Item[i].TableId == TableId) {
        TableHeaderPtr = (AMD_CPM_TABLE_COMMON_HEADER*)TableListPtr->Item[i].SubTable.Pointer;
        if (!TableHeaderPtr->PlatformMask || TableHeaderPtr->PlatformMask & PlatformMask) {
          if (Flag == 1) {
            TableListPtr->Item[i].TableId &= 0x7FFFFFFF;
          } else if (Flag == 2) {
            TableListPtr->Item[i].TableId |= 0x80000000;
          } else if (Flag == 3 || Flag == 4) {
            if (Flag == 4) {
              TableListPtr->Item[i].TableId &= 0x7FFFFFFF;
            }
            if (((UINT64)TableHeaderPtr & 0xFF000000) == 0xFF000000) {
              if (HobTablePtr->BufferItem == 0 || HobTablePtr->BufferSize < TableHeaderPtr->TableSize) {
                CpmPostCode (CpmTpErrorNoEnoughSizeInHobBuffer);
                CPM_DEADLOOP ();
              } else {
                TableListPtr->Item[i].SubTable.Raw = (UINTN) ((UINT8*)HobTablePtr + HobTablePtr->BufferOffset);
                TableListPtr->Item[i].Flag = 1;
                CpmCopyMem (TableListPtr->Item[i].SubTable.Pointer, TableHeaderPtr, TableHeaderPtr->TableSize);
                HobTablePtr->BufferOffset += TableHeaderPtr->TableSize;
                HobTablePtr->BufferSize -= TableHeaderPtr->TableSize;
                TableListPtr->Size += TableHeaderPtr->TableSize;
              }
            }
          }
          DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
          DEBUG((DEBUG_INFO, " TableListPtr->Item[%d]", i));
          DEBUG((DEBUG_INFO, " TableId %08X", TableListPtr->Item[i].TableId));
          DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableListPtr->Item[i].SubTable.Raw));
          return TableListPtr->Item[i].SubTable.Pointer;
        }
      }
    }
  }

  DEBUG((DEBUG_INFO, " LIB-TBL-%a-End-NULL\n", __FUNCTION__));
  return NULL;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the table size.
 *
 * @param[in]     TablePtr    The Pointer of CPM Table
 *
 * @retval        The size of CPM table
 */
UINT16
EFIAPI
CpmGetTableSize (
  IN       VOID    *TablePtr
  )
{
  UINT16 Size;
  Size = 0;
  if (TablePtr) {
    Size = ((AMD_CPM_TABLE_COMMON_HEADER*)TablePtr)->TableSize;
  }
  DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
  DEBUG((DEBUG_INFO, " TablePtr %p", TablePtr));
  DEBUG((DEBUG_INFO, " Size %04X\n", Size));
  return (Size);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Generate CPM Table lists according to which driver to use.
 *
 * @param[in]     TableListPtr        CPM table list pointer from CPM OEM driver
 * @param[in]     PlatformId          Current Platform Id
 * @param[in]     TableInHobListPtr   The Pointer of AMD CPM Table List for the table in HOB
 *
 */
VOID
EFIAPI
CpmGenerateTableList (
  IN       VOID                *TableListPtr,
  IN       UINT16              PlatformId,
  IN OUT   AMD_CPM_TABLE_LIST  *TableInHobListPtr
  )
{
  UINT32                      PlatformMask;
  AMD_CPM_TABLE_COMMON_HEADER **TablePtr;

  TablePtr = TableListPtr;

  if (PlatformId < 32) {
    PlatformMask = 1 << PlatformId;
  } else {
    PlatformMask = 0xFFFFFFFF;
  }

  DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
  DEBUG((DEBUG_INFO, " TableListPtr %p", TableListPtr));
  DEBUG((DEBUG_INFO, " PlatformId %04X", PlatformId));
  DEBUG((DEBUG_INFO, " TableInHobListPtr %p\n", TableInHobListPtr));
  TableInHobListPtr->Number = 0;
  TableInHobListPtr->Size = 0;

  while (*TablePtr != NULL) {
    if (((*TablePtr)->PlatformMask & PlatformMask) || ((*TablePtr)->PlatformMask == 0)) {
      TableInHobListPtr->Item[TableInHobListPtr->Number].SubTable.Raw = 0;
      TableInHobListPtr->Item[TableInHobListPtr->Number].SubTable.Pointer = *TablePtr;
      TableInHobListPtr->Item[TableInHobListPtr->Number].TableId = (*TablePtr)->TableSignature;
      DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
      DEBUG((DEBUG_INFO, " TableInHobListPtr->Item[%d]", TableInHobListPtr->Number));
      DEBUG((DEBUG_INFO, " TableId %08X", TableInHobListPtr->Item[TableInHobListPtr->Number].TableId));
      DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableInHobListPtr->Item[TableInHobListPtr->Number].SubTable.Raw));
      if (((*TablePtr)->Attribute & (CPM_OVERRIDE | CPM_PEI)) == (CPM_OVERRIDE | CPM_PEI)) {
        TableInHobListPtr->Item[TableInHobListPtr->Number].Flag = 1;
        TableInHobListPtr->Size += CpmGetTableSize (*TablePtr);
      } else {
        TableInHobListPtr->Item[TableInHobListPtr->Number].Flag = 0;
      }
      TableInHobListPtr->Number++;
    }
    TablePtr++;
  }
  if (TableInHobListPtr->Number > AMD_TABLE_LIST_ITEM_SIZE) {
    CpmPostCode (CpmTpErrorTableNumberOverflow);
    CPM_DEADLOOP ();
  }
  TableInHobListPtr->Header.TableSize = (UINT16) (sizeof (AMD_CPM_TABLE_LIST) - (AMD_TABLE_LIST_ITEM_SIZE - TableInHobListPtr->Number - AMD_TABLE_LIST_ADDITIONAL_ITEM_SIZE) * sizeof (AMD_CPM_TABLE_ITEM));
  TableInHobListPtr->Size += TableInHobListPtr->Header.TableSize + sizeof (AMD_CPM_HOB_HEADER);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Copy a memory block between two buffers.
 *
 * @param[in]     destination   The destination buffer address
 * @param[in]     source        The source buffer address
 * @param[in]     size          The memory size to copy
 *
 */
VOID
EFIAPI
CpmCopyMem (
  IN OUT   VOID    *destination,
  IN       VOID    *source,
  IN       UINTN    size
  )
{
  UINTN i;
  UINT8 *Src;
  UINT8 *Dst;
  if (source != NULL && destination != NULL && size != 0 && source != destination) {
    DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
    DEBUG((DEBUG_INFO, " destination %p", destination));
    DEBUG((DEBUG_INFO, " source %p", source));
    DEBUG((DEBUG_INFO, " size %X\n", size));
    Src = source;
    Dst = destination;
    for (i = 0; i < size; i++) {
      *Dst = *Src;
      Src ++;
      Dst ++;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Copy CPM table and table list to memory area and return the Main Table Pointer if found.
 *
 * @param[in]     TableInHobListPtr   The Pointer of AMD CPM Table List for the table in HOB
 * @param[in]     HobTablePtr         HOB buffer to  store CPM table
 *
 * @retval        CPM Main Table Pointer if non-zero
 */
VOID *
EFIAPI
CpmCopyTableListToMemory (
  IN       AMD_CPM_TABLE_LIST  *TableInHobListPtr,
  IN OUT   VOID                *HobTablePtr
  )
{
  UINT16  size;
  UINT16  i;
  VOID    *MainTablePtr;
  VOID    *HobPtr;

  DEBUG((DEBUG_INFO, " LIB-TBL-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, " TableInHobListPtr %p", TableInHobListPtr));
  DEBUG((DEBUG_INFO, " HobTablePtr %p\n", HobTablePtr));
  MainTablePtr = NULL;
  HobPtr = (VOID*) ((UINT8*)HobTablePtr + sizeof (AMD_CPM_HOB_HEADER));
  for (i = 0; i < TableInHobListPtr->Number; i++) {
    if (TableInHobListPtr->Item[i].Flag == 1) {
      DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
      DEBUG((DEBUG_INFO, " TableInHobListPtr->Item[%d]", i));
      DEBUG((DEBUG_INFO, " TableId %08X", TableInHobListPtr->Item[i].TableId));
      DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableInHobListPtr->Item[i].SubTable.Raw));
      if (TableInHobListPtr->Item[i].TableId == CPM_SIGNATURE_MAIN_TABLE) {
        MainTablePtr = HobPtr;
        DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
        DEBUG((DEBUG_INFO, " MainTablePtr %p\n", MainTablePtr));
      }
      size = CpmGetTableSize (TableInHobListPtr->Item[i].SubTable.Pointer);
      CpmCopyMem (HobPtr, TableInHobListPtr->Item[i].SubTable.Pointer, size);
      TableInHobListPtr->Item[i].SubTable.Raw = (UINTN)HobPtr;
      HobPtr = (VOID*) ((UINT8*)HobPtr + size);
      DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
      DEBUG((DEBUG_INFO, " TableInHobListPtr->Item[%d]", i));
      DEBUG((DEBUG_INFO, " TableId %08X", TableInHobListPtr->Item[i].TableId));
      DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableInHobListPtr->Item[i].SubTable.Raw));
    }
  }

  CpmCopyMem (HobPtr, TableInHobListPtr, TableInHobListPtr->Header.TableSize);

  ((AMD_CPM_HOB_HEADER*)HobTablePtr)->HeaderPtr.Raw = (UINTN)HobTablePtr;
  if (MainTablePtr != NULL) {
    ((AMD_CPM_HOB_HEADER*)HobTablePtr)->MainTablePtr.Raw = (UINTN)MainTablePtr;
  }
  ((AMD_CPM_HOB_HEADER*)HobTablePtr)->BufferOffset = TableInHobListPtr->Size;
  ((AMD_CPM_HOB_HEADER*)HobTablePtr)->BufferSize = AMD_TABLE_ADDITIONAL_BUFFER_SIZE;
  ((AMD_CPM_HOB_HEADER*)HobTablePtr)->BufferItem = AMD_TABLE_LIST_ADDITIONAL_ITEM_SIZE;

  if (MainTablePtr != NULL) {
    ((AMD_CPM_MAIN_TABLE*)MainTablePtr)->TableInRomList.Raw = 0;
    ((AMD_CPM_MAIN_TABLE*)MainTablePtr)->TableInRamList.Raw = 0;
    ((AMD_CPM_MAIN_TABLE*)MainTablePtr)->TableInHobList.Raw = (UINTN)HobPtr;
    ((AMD_CPM_MAIN_TABLE*)MainTablePtr)->HobTablePtr.Raw    = (UINTN)HobTablePtr;
  }

  DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
  DEBUG((DEBUG_INFO, " HobTablePtr->HeaderPtr.Raw       %LX\n", ((AMD_CPM_HOB_HEADER*)HobTablePtr)->HeaderPtr.Raw));
  if (MainTablePtr != NULL) {
    DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
    DEBUG((DEBUG_INFO, " HobTablePtr->MainTablePtr.Raw    %LX\n", ((AMD_CPM_HOB_HEADER*)HobTablePtr)->MainTablePtr.Raw));
    DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
    DEBUG((DEBUG_INFO, " MainTablePtr->TableInHobList.Raw %LX\n", ((AMD_CPM_MAIN_TABLE*)MainTablePtr)->TableInHobList.Raw));
    DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
    DEBUG((DEBUG_INFO, " MainTablePtr->HobTablePtr.Raw    %LX\n", ((AMD_CPM_MAIN_TABLE*)MainTablePtr)->HobTablePtr.Raw));
    DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
    DEBUG((DEBUG_INFO, " MainTablePtr %p\n", MainTablePtr));
  }
  return MainTablePtr;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Recalculate the size of CPM table.
 *
 * @param[in]     HobTablePtr         HOB buffer to store CPM table
 *
 * @retval        Table List Pointer if non-zero
 */
AMD_CPM_TABLE_LIST *
EFIAPI
CpmAdjustTableList (
  IN       VOID                *HobTablePtr
  )
{
  AMD_CPM_TABLE_LIST  *TableListPtr;
  AMD_CPM_MAIN_TABLE  *MainTablePtr;
  AMD_CPM_HOB_HEADER  *HobHeaderPtr;
  UINT16              Size;
  UINT16              TableNumber;
  UINT16              i;
  AMD_CPM_TABLE_COMMON_HEADER *TableHeaderPtr;

  Size = 0;
  TableNumber = 0;
  HobHeaderPtr = (AMD_CPM_HOB_HEADER*) HobTablePtr;
  MainTablePtr = (AMD_CPM_MAIN_TABLE*) (HobHeaderPtr->MainTablePtr.Pointer);
  TableListPtr = (AMD_CPM_TABLE_LIST*) (MainTablePtr->TableInHobList.Pointer);
  DEBUG((DEBUG_INFO, " LIB-TBL-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, " HobTablePtr %p", HobTablePtr));
  DEBUG((DEBUG_INFO, " HobHeaderPtr %p", HobHeaderPtr));
  DEBUG((DEBUG_INFO, " MainTablePtr %p\n", MainTablePtr));

  for (i = 0; i < TableListPtr->Number; i++) {
    TableHeaderPtr = (AMD_CPM_TABLE_COMMON_HEADER*)TableListPtr->Item[i].SubTable.Pointer;
    if ((TableHeaderPtr->PlatformMask & (1 << MainTablePtr->CurrentPlatformId)) || (TableHeaderPtr->PlatformMask == 0)) {
      TableListPtr->Item[TableNumber].SubTable.Raw = TableListPtr->Item[i].SubTable.Raw;
      TableListPtr->Item[TableNumber].TableId = TableListPtr->Item[i].TableId;
      TableListPtr->Item[TableNumber].Flag = 1;
      Size = Size + CpmGetTableSize (TableListPtr->Item[TableNumber].SubTable.Pointer);
      DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
      DEBUG((DEBUG_INFO, " TableListPtr->Item[%d]", TableNumber));
      DEBUG((DEBUG_INFO, " TableId %08X", TableListPtr->Item[TableNumber].TableId));
      DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableListPtr->Item[TableNumber].SubTable.Raw));
      TableNumber++;
    }
  }
  if (TableNumber > AMD_TABLE_LIST_ITEM_SIZE) {
    CpmPostCode (CpmTpErrorTableNumberOverflow);
    CPM_DEADLOOP ();
  }
  TableListPtr->Number = TableNumber;
  TableListPtr->Header.TableSize = (UINT16) (sizeof (AMD_CPM_TABLE_LIST) - (AMD_TABLE_LIST_ITEM_SIZE - TableListPtr->Number - AMD_TABLE_LIST_ADDITIONAL_ITEM_SIZE) * sizeof (AMD_CPM_TABLE_ITEM));
  TableListPtr->Size = TableListPtr->Header.TableSize + sizeof (AMD_CPM_HOB_HEADER) + Size;
  DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
  DEBUG((DEBUG_INFO, " TableListPtr %p", TableListPtr));
  DEBUG((DEBUG_INFO, " Number %d Header.TableSize %04X Size %04X\n", TableListPtr->Number, TableListPtr->Header.TableSize, TableListPtr->Size));
  return TableListPtr;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Adjust the pointer in CPM tables after it is copied to new area.
 *
 * @param[in]     TablePtr    The Pointer of buffer to store CPM table lists and CPM Table
 *
 */
VOID
EFIAPI
CpmRelocateTableList (
  IN OUT   VOID        *TablePtr
  )
{
  UINT64              Relocate;
  UINT8               i;
  AMD_CPM_MAIN_TABLE  *MainTablePtr;
  AMD_CPM_HOB_HEADER  *HobHeaderPtr;
  AMD_CPM_TABLE_LIST  *TableListPtr;
  BOOLEAN             Forward;

  HobHeaderPtr = (AMD_CPM_HOB_HEADER*)TablePtr;
  if ((UINTN)HobHeaderPtr >= (UINTN)HobHeaderPtr->HeaderPtr.Pointer) {
    Relocate = (UINTN)HobHeaderPtr - (UINTN)HobHeaderPtr->HeaderPtr.Pointer;
    Forward = TRUE;
  } else {
    Relocate = (UINTN)HobHeaderPtr->HeaderPtr.Pointer - (UINTN)HobHeaderPtr;
    Forward = FALSE;
  }
  DEBUG((DEBUG_INFO, " LIB-TBL-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, " TablePtr %p", TablePtr));
  DEBUG((DEBUG_INFO, " HobHeaderPtr %p", HobHeaderPtr));
  DEBUG((DEBUG_INFO, " HeaderPtr.Raw %LX", HobHeaderPtr->HeaderPtr.Raw));
  DEBUG((DEBUG_INFO, " Relocate %LX", Relocate));
  DEBUG((DEBUG_INFO, " Forward %s\n", Forward ? L"TRUE" : L"FALSE"));

  MainTablePtr = (AMD_CPM_MAIN_TABLE*) ((UINT8*)HobHeaderPtr->MainTablePtr.Pointer);
  TableListPtr = (AMD_CPM_TABLE_LIST*) ((UINT8*)MainTablePtr->TableInHobList.Pointer);

  if (Forward) {
    MainTablePtr = (AMD_CPM_MAIN_TABLE*) (HobHeaderPtr->MainTablePtr.Raw + Relocate);
    TableListPtr = (AMD_CPM_TABLE_LIST*) (MainTablePtr->TableInHobList.Raw + Relocate);
  } else {
    MainTablePtr = (AMD_CPM_MAIN_TABLE*) (HobHeaderPtr->MainTablePtr.Raw - Relocate);
    TableListPtr = (AMD_CPM_TABLE_LIST*) (MainTablePtr->TableInHobList.Raw - Relocate);
  }

  for (i = 0; i < TableListPtr->Number; i++) {
    if (TableListPtr->Item[i].Flag == 1) {
      if (Forward) {
        TableListPtr->Item[i].SubTable.Raw = TableListPtr->Item[i].SubTable.Raw + Relocate;
      } else {
        TableListPtr->Item[i].SubTable.Raw = TableListPtr->Item[i].SubTable.Raw - Relocate;
      }
      DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
      DEBUG((DEBUG_INFO, " TableListPtr->Item[%d]", i));
      DEBUG((DEBUG_INFO, " TableId %08X", TableListPtr->Item[i].TableId));
      DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableListPtr->Item[i].SubTable.Raw));
    }
  }

  HobHeaderPtr->HeaderPtr.Pointer = HobHeaderPtr;
  HobHeaderPtr->MainTablePtr.Pointer = MainTablePtr;
  MainTablePtr->TableInHobList.Pointer = TableListPtr;
  MainTablePtr->HobTablePtr.Pointer = TablePtr;
  DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
  DEBUG((DEBUG_INFO, " HobHeaderPtr %p", HobHeaderPtr));
  DEBUG((DEBUG_INFO, " HobHeaderPtr->HeaderPtr.Raw %LX", HobHeaderPtr->HeaderPtr.Raw));
  DEBUG((DEBUG_INFO, " HobHeaderPtr->MainTablePtr.Raw %LX\n", HobHeaderPtr->MainTablePtr.Raw));
  DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
  DEBUG((DEBUG_INFO, " MainTablePtr %p", MainTablePtr));
  DEBUG((DEBUG_INFO, " MainTablePtr->TableInHobList.Raw %LX", MainTablePtr->TableInHobList.Raw));
  DEBUG((DEBUG_INFO, " MainTablePtr->HobTablePtr.Raw %LX\n", MainTablePtr->HobTablePtr.Raw));
}

/*----------------------------------------------------------------------------------------*/
/**
 * If there is no CPM table with the same signature to exist, add a new table. Otherwise,
 * use the new table to replace the orignal one.
 *
 * @param[in]     This        The Pointer of CPM Table Ppi or Protocol
 * @param[in]     TablePtr    CPM Table to add
 *
 * @retval        Original CPM Table with same signature
 */
VOID*
EFIAPI
CpmAddTable (
  IN       VOID                *This,
  IN       VOID                *TablePtr
  )
{
  AMD_CPM_MAIN_TABLE          *MainTablePtr;
  AMD_CPM_HOB_HEADER          *HobTablePtr;
  AMD_CPM_TABLE_LIST          *TableInHobListPtr;
  AMD_CPM_TABLE_COMMON_HEADER *NewTable;
  AMD_CPM_TABLE_COMMON_HEADER *OldTable;

  MainTablePtr = ((AMD_CPM_TABLE_PPI*)This)->MainTablePtr;
  HobTablePtr = (AMD_CPM_HOB_HEADER*) MainTablePtr->HobTablePtr.Pointer;
  TableInHobListPtr = (AMD_CPM_TABLE_LIST*) MainTablePtr->TableInHobList.Pointer;
  NewTable = TablePtr;

  DEBUG((DEBUG_INFO, " LIB-TBL-%a-Start", __FUNCTION__));
  DEBUG((DEBUG_INFO, " This %p", This));
  DEBUG((DEBUG_INFO, " TablePtr %p\n", TablePtr));
  OldTable = CpmSearchAndUpdateTable (This, NewTable->TableSignature, 3);

  if (OldTable) {
    CpmCopyMem (OldTable, NewTable, OldTable->TableSize);
  } else {
    OldTable = CpmSearchAndUpdateTable (This, NewTable->TableSignature | 0x80000000, 4);
    if (OldTable) {
      CpmCopyMem (OldTable, NewTable, OldTable->TableSize);
    } else {
      if (HobTablePtr->BufferItem == 0 || HobTablePtr->BufferSize < NewTable->TableSize) {
        CpmPostCode (CpmTpErrorNoEnoughSizeInHobBuffer);
        CPM_DEADLOOP ();
      } else {
        HobTablePtr->BufferItem --;
        OldTable = (AMD_CPM_TABLE_COMMON_HEADER*) ((UINT8*)HobTablePtr + HobTablePtr->BufferOffset);
        CpmCopyMem (OldTable, NewTable, NewTable->TableSize);
        HobTablePtr->BufferOffset += NewTable->TableSize;
        HobTablePtr->BufferSize -= NewTable->TableSize;
        TableInHobListPtr->Item[TableInHobListPtr->Number].TableId = NewTable->TableSignature;
        TableInHobListPtr->Item[TableInHobListPtr->Number].Flag = 1;
        TableInHobListPtr->Item[TableInHobListPtr->Number].SubTable.Raw = (UINTN)OldTable;
        DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
        DEBUG((DEBUG_INFO, " TableInHobListPtr->Item[%d]", TableInHobListPtr->Number));
        DEBUG((DEBUG_INFO, " TableId %08X", TableInHobListPtr->Item[TableInHobListPtr->Number].TableId));
        DEBUG((DEBUG_INFO, " SubTable.Raw %LX\n", TableInHobListPtr->Item[TableInHobListPtr->Number].SubTable.Raw));
        TableInHobListPtr->Number ++;
        TableInHobListPtr->Size += NewTable->TableSize;
      }
    }
  }

  DEBUG((DEBUG_INFO, " LIB-TBL-%a-End", __FUNCTION__));
  DEBUG((DEBUG_INFO, " OldTable %p\n", OldTable));
  return OldTable;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Disable CPM table with the same signature to be found.
 *
 * @param[in]     This        The Pointer of CPM Table Ppi or Protocol
 * @param[in]     TablePtr    CPM Table to add
 *
 * @retval        Original CPM Table with same signature
 */
VOID*
EFIAPI
CpmRemoveTable (
  IN       VOID                *This,
  IN       VOID                *TablePtr
  )
{
  AMD_CPM_TABLE_COMMON_HEADER *OldTable;

  OldTable = TablePtr;

  DEBUG((DEBUG_INFO, " LIB-TBL-%a", __FUNCTION__));
  DEBUG((DEBUG_INFO, " This %p", This));
  DEBUG((DEBUG_INFO, " TablePtr %p", TablePtr));
  DEBUG((DEBUG_INFO, " TableId %08X\n", OldTable->TableSignature));
  return CpmSearchAndUpdateTable (This, OldTable->TableSignature, 2);
}
