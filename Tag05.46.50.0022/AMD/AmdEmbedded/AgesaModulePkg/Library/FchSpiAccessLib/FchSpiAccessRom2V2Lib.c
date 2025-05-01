/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Uefi.h"
#include <Filecode.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FchBaseLib.h>
#include <Library/FchSpiAccessLib.h>
#include "FchRegistersCommon.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHSPIACCESSLIB_FCHSPIACCESSROM2V2LIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

BOOLEAN
Rom32MbSupported (
  VOID
  )
{
  BOOLEAN                   Status = TRUE;
  UINT32                    Family, ExtModel;

  Family = FchGetFamilyId();
  ExtModel = FchGetModelId() & 0xF0;

  //SSP/RN/RMB/...
  if ((Family == 0x17 && ExtModel == 0x30) || (Family == 0x17 && ExtModel >= 0x60) || (Family == 0x19)){
    Status = TRUE;
  }else{
    Status = FALSE;
  }
  return Status;
}


UINT32
SearchRom2Page(
  UINT32    RomBank
  )
{
  UINT32    Spix30, Spix5C;
  UINT8     SpiRomPageXor;
  UINT32    RomPage;

  Spix30 = MmioRead32 ((UINTN)(FCH_SPI_BASE_ADDRESS + FCH_SPI_MMIO_REG30));
  RomPage = RomBank >> 24;
  if (Spix30 & FCH_SPI_R2MSK24){
    RomPage &= ~FCH_SPI_R2VAL24;
    RomPage |= Spix30 & FCH_SPI_R2VAL24;
  }
  if (Spix30 & FCH_SPI_R2MSK25){
    RomPage &= ~FCH_SPI_R2VAL25;
    RomPage |= Spix30 & FCH_SPI_R2VAL25;
  }

  Spix5C = MmioRead32 ((UINTN)(FCH_SPI_BASE_ADDRESS + FCH_SPI_MMIO_REG5C_Addr32_Ctrl3));
  SpiRomPageXor = (UINT8)(Spix5C & FCH_SPI_SPIROM_PAGE_MASK);
  RomPage ^= SpiRomPageXor;

  return (RomPage << 24);
}

BOOLEAN
CheckRom2Bank(
  UINT32   Offset,
  UINT32   *Bank
  )
{
  BOOLEAN   Status;
  UINT32    Rom2Base, Rom2Size;
  UINT32    Rom2Bank[FCH_ROM_ROM2_MAX_BANK];
  UINT32    Rom2Page[FCH_ROM_ROM2_MAX_BANK];
  UINT32    BankCount = 0;

  if (Bank == NULL) {
    return FALSE;
  }

  Status = GetRom2Info(&Rom2Base, &Rom2Size);
  if (Status) {
    BankCount = Rom2Size / FCH_ROM_SIZE_16M;
    if (Rom2Size % FCH_ROM_SIZE_16M) {
      BankCount += 1;
    }
    while ((BankCount > 0) && (BankCount <= FCH_ROM_ROM2_MAX_BANK)) {
      BankCount--;
      Rom2Bank[BankCount] = Rom2Base + FCH_ROM_SIZE_16M * BankCount;
      Rom2Page[BankCount] = SearchRom2Page(Rom2Bank[BankCount]);
      if ((Rom2Page[BankCount] & BIT24) == (Offset & BIT24)){
        *Bank = Rom2Bank[BankCount];
        return TRUE;
      }
    }
  }

  return FALSE;
}

BOOLEAN
CheckRom2Offset (
  UINT32   Offset,
  UINT32   Length
  )
{
  if((Offset < FCH_ROM_SIZE_16M) && (Length <= FCH_ROM_SIZE_16M) && ((Offset + Length) < FCH_ROM_SIZE_16M)){
    return TRUE;
  } else {
    return FALSE;
  }
}

BOOLEAN
CheckRom2Window (
  UINT32   Location,
  UINT32   Length
  )
{
  BOOLEAN   Status;
  UINT32    Rom2Base, Rom2Size;

  Status = GetRom2Info(&Rom2Base, &Rom2Size);
  if (Status) {
    if ((Location >= Rom2Base) && (Location <= (Rom2Base + Rom2Size - 1)) && (Length <= Rom2Size) && \
        ((Length + Location) <= (Rom2Base + Rom2Size - 1))) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

BOOLEAN
CheckRom3Window (
  UINT64   Location,
  UINT32   Length
  )
{
  BOOLEAN   Status;
  UINT64    Rom3Base;
  UINT32    Rom3Size;

  Status = GetRom3Info(&Rom3Base, &Rom3Size);
  if (Status){
    if ((Location >= Rom3Base) && (Location <= (Rom3Base + Rom3Size - 1)) && (Length <= Rom3Size) && \
        ((Length + Location) <= (Rom3Base + Rom3Size - 1))) {
      return TRUE;
    }
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * IsRom2Decoded - check Offset
 *
 *
 * @param[in] Offset - The offset of ROM
 *
 * @retval BOOLEAN Is in SPI ROM2 decode window or not
 */
BOOLEAN
IsRom2Decoded (
  IN       UINT64        Offset
  )
{
  BOOLEAN    Status = FALSE;
  UINT32     Bank;

  if (Rom32MbSupported()) {
    Status = CheckRom2Bank((UINT32)Offset, &Bank);
  } else {
    Status = FALSE;
  }

  return Status;
}

/**
 * FchSpiRomRead - Read the SPI ROM
 *
 *
 * @param[in] Offset - Offset of physical SPI ROM
 * @param[out]*Data - Buffer address to save the data
 * @param[in] Length - The number of byte to read
 *
 * @retval BOOLEAN success read or not
 */
BOOLEAN
FchSpiRomRead (
    IN      UINT32            Offset,
    OUT     UINT8             *Data,
    IN      UINT32            Length
  )
{
  BOOLEAN                   Status = FALSE;
  UINT32                    Rom2Base, Rom2Size;
  UINT32                    Rom2Bank;

  if ((Data == NULL) || (Length == 0)) {
    return FALSE;
  }

  if (Rom32MbSupported()) {
    if (CheckRom2Bank((UINT32)Offset, &Rom2Bank)) {
      if (CheckRom2Offset ((UINT32)(Offset & FCH_ROM_RANGE_MASK), Length)) {
        Rom2Bank += (UINT32)(Offset & FCH_ROM_RANGE_MASK);
        CopyMem(Data, (VOID *)(UINTN) Rom2Bank, Length);
        Status = TRUE;
      } else {
        Status = FALSE;
      }
    }
  }else {
    if ((Offset < FCH_ROM_SIZE_16M) && CheckRom2Offset(Offset , Length)) {
      Status = GetRom2Info(&Rom2Base, &Rom2Size);
      if (Status) {
        Rom2Bank = Rom2Base + Offset;
        CopyMem(Data, (VOID *)(UINTN) Rom2Bank, Length);
        Status = TRUE;
      }
    } else {
      Status = FALSE;
    }
  }

  return Status;
}

/**
 * FchSpiRomReadEx - Extended function to read the SPI ROM
 *
 *
 * @param[in] Location - Offset of physical SPI ROM
 *                     - Host address in ROM2/ROM3 window to SPI ROM
 * @param[out]*Data - Buffer address to save the data
 * @param[in] Length - The number of byte to read
 *
 * @retval BOOLEAN success read or not
 */
BOOLEAN
FchSpiRomReadEx (
    IN      UINTN             Location,
    OUT     UINT8             *Data,
    IN      UINT32            Length
  )
{
  BOOLEAN    Status = FALSE;
  UINT32     Rom2Base, Rom2Size;
  UINT32     Rom2Bank;

  if ((Data == NULL) || (Length == 0)) {
    return FALSE;
  }

  if (Rom32MbSupported()) {
    // ROM3 can only access in 64 bit mode
    if ((sizeof(UINTN) == sizeof(UINT64)) && (CheckRom3Window((UINT64)Location, Length))) {
      CopyMem(Data, (VOID *)Location, Length);
      Status = TRUE;
    }else if (CheckRom2Window((UINT32)Location, Length)) {
      CopyMem(Data, (VOID *)Location, Length);
      Status = TRUE;
    } else if ((Location < FCH_ROM_SIZE_64M) && (CheckRom2Bank((UINT32)Location, &Rom2Bank))) {
      if (CheckRom2Offset((UINT32)(Location & FCH_ROM_RANGE_MASK), Length)) {
        Rom2Bank += (UINT32)(Location & FCH_ROM_RANGE_MASK);
        CopyMem(Data, (VOID *)(UINTN) Rom2Bank, Length);
        Status = TRUE;
      }
    } else {
      Status = FALSE;
    }
  } else {
    if (CheckRom2Window((UINT32)Location, Length)) {
      CopyMem(Data, (VOID *)Location, Length);
      Status = TRUE;
    } else if ((Location < FCH_ROM_SIZE_16M) && CheckRom2Offset((UINT32)(Location & FCH_ROM_RANGE_MASK), Length)) {
      Status = GetRom2Info(&Rom2Base, &Rom2Size);
      if (Status) {
        Rom2Bank = Rom2Base + (UINT32)(Location & FCH_ROM_RANGE_MASK);
        CopyMem(Data, (VOID *)(UINTN) Rom2Bank, Length);
        Status = TRUE;
      }
    } else {
      Status = FALSE;
    }
  }

  return Status;
}
