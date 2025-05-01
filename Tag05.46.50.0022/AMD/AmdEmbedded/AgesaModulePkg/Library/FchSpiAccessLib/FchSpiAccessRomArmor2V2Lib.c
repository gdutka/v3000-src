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
#include <Filecode.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/FchBaseLib.h>
#include <Library/FchSpiAccessLib.h>
#include <Library/AmdPspFlashAccLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdBaseLib.h>
#include <Library/AmdHeapLib.h>
#include "FchRegistersCommon.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHSPIACCESSLIB_FCHSPIACCESSROMARMOR2V2LIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
UINT32    mRom2Base, mRom2Size;
UINT32    mRom3Size;
UINT64    mRom3Base;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
EFIAPI
FchSpiAccessV2LibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT32           *HstiStateBuffer = NULL;
  LOCATE_HEAP_PTR  LocateHeapParams;

  LocateHeapParams.BufferHandle = AMD_PSP_HSTI_STATE_BUFFER_HANDLE;

  if (HeapLocateBuffer (&LocateHeapParams, NULL) == AGESA_SUCCESS) {
    HstiStateBuffer = (UINT32 *)LocateHeapParams.BufferPtr;
  }

  if ((HstiStateBuffer != NULL) && ((*HstiStateBuffer) & PSP_ROM_ARMOR_ENFORCED)){
    mRom2Base = FCH_ROM_INVALID_START_ADDRESS_2;
    mRom2Size = 0;
    mRom3Base = FCH_ROM_INVALID_START_ADDRESS_3;
    mRom3Size = 0;
  } else {
    if (GetRom2Info(&mRom2Base, &mRom2Size) && GetRom3Info(&mRom3Base, &mRom3Size)) {
      return EFI_SUCCESS;
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}


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
  UINT32    SpiRomPageXor;
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
  SpiRomPageXor = (UINT32)(Spix5C & FCH_SPI_SPIROM_PAGE_MASK);
  RomPage ^= SpiRomPageXor;

  return (RomPage << 24);
}

BOOLEAN
CheckRom2Bank(
  UINT32   Offset,
  UINT32   *Bank
  )
{
  UINT32    Rom2Bank[FCH_ROM_ROM2_MAX_BANK];
  UINT32    Rom2Page[FCH_ROM_ROM2_MAX_BANK];
  UINT32    BankCount = 0;

  if (Bank == NULL) {
    return FALSE;
  }

  BankCount = mRom2Size / FCH_ROM_SIZE_16M;
  if (mRom2Size % FCH_ROM_SIZE_16M) {
    BankCount += 1;
  }
  while ((BankCount > 0) && (BankCount <= FCH_ROM_ROM2_MAX_BANK)) {
    BankCount--;
    Rom2Bank[BankCount] = mRom2Base + FCH_ROM_SIZE_16M * BankCount;
    Rom2Page[BankCount] = SearchRom2Page(Rom2Bank[BankCount]);
    if ((Rom2Page[BankCount] & BIT24) == (Offset & BIT24)){
      *Bank = Rom2Bank[BankCount];
      return TRUE;
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
  if ((Location >= mRom2Base) && (Location <= (mRom2Base + mRom2Size - 1)) && (Length <= mRom2Size) && \
      ((Length + Location) <= (mRom2Base + mRom2Size - 1))) {
    return TRUE;
  } else {
    return FALSE;
  }
}

UINT64
SearchRom3Page(
  UINT64    RomBank
  )
{
  UINT32    Spix5C;
  UINT64    SpiRomPageXor;
  UINT64    RomPage;

  RomPage = RomBank >> 24;
  Spix5C = MmioRead32 ((UINTN)(FCH_SPI_BASE_ADDRESS + FCH_SPI_MMIO_REG5C_Addr32_Ctrl3));
  SpiRomPageXor = (UINT64)(Spix5C & FCH_SPI_SPIROM_PAGE_MASK);
  RomPage ^= SpiRomPageXor;

  return (RomPage << 24);
}

BOOLEAN
CheckRom3Bank(
  UINT32   Offset,
  UINT64   *Bank
  )
{
  UINT64    Rom3Bank[FCH_ROM_ROM2_MAX_BANK];
  UINT64    Rom3Page[FCH_ROM_ROM2_MAX_BANK];
  UINT32    BankCount = 0;

  if (Bank == NULL) {
    return FALSE;
  }

  BankCount = mRom3Size / FCH_ROM_SIZE_16M;
  if (mRom3Size % FCH_ROM_SIZE_16M) {
    BankCount += 1;
  }
  while ((BankCount > 0) && (BankCount <= FCH_ROM_ROM3_MAX_BANK)) {
    BankCount--;
    Rom3Bank[BankCount] = mRom3Base + FCH_ROM_SIZE_16M * BankCount;
    Rom3Page[BankCount] = SearchRom3Page(Rom3Bank[BankCount]);
    if ((Rom3Page[BankCount] & (BIT24 | BIT25)) == (Offset & (BIT24 | BIT25))){
      *Bank = Rom3Bank[BankCount];
      return TRUE;
    }
  }
  return FALSE;
}

BOOLEAN
CheckRom3Offset (
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
CheckRom3Window (
  UINT64   Location,
  UINT32   Length
  )
{
  if ((Location >= mRom3Base) && (Location <= (mRom3Base + mRom3Size - 1)) && (Length <= mRom3Size) && \
      ((Length + Location) <= (mRom3Base + mRom3Size - 1))) {
    return TRUE;
  } else {
    return FALSE;
  }
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
  return FALSE;
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
  return FchSpiRomReadEx(Offset, Data, Length);
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
  UINT32            Rom2Bank;
  UINT64            Rom3Bank;
  UINT32            Rom2Base, Rom2Size;
  BOOLEAN           AddressValid = FALSE;
  UINTN             HostAddress;

  EFI_STATUS        Status = EFI_UNSUPPORTED;
  UINTN             NumByte;

  if ((Data == NULL) || (Length == 0)){
   return FALSE;
  }

  if (sizeof(UINTN) != sizeof(UINT64)){ //not in 64 bit mode
    return FALSE;
  }

  if (Rom32MbSupported()) {
    if ((mRom2Base != FCH_ROM_INVALID_START_ADDRESS_2) && (mRom3Base != FCH_ROM_INVALID_START_ADDRESS_3)) {
      if (CheckRom3Window((UINT64)Location, Length) || CheckRom2Window((UINT32)Location, Length)) {
        HostAddress = Location;
        AddressValid = TRUE;
      } else if ((Location < FCH_ROM_SIZE_64M) && (CheckRom3Bank((UINT32)Location, &Rom3Bank))) {
        if (CheckRom3Offset ((UINT32)(Location & FCH_ROM_RANGE_MASK), Length)) {
          HostAddress = Rom3Bank + (UINT32)(Location & FCH_ROM_RANGE_MASK);
          AddressValid = TRUE;
        }
      } else if ((Location < FCH_ROM_SIZE_64M) && (CheckRom2Bank((UINT32)Location, &Rom2Bank))) {
        if (CheckRom2Offset ((UINT32)(Location & FCH_ROM_RANGE_MASK), Length)) {
          HostAddress = Rom2Bank + (UINT32)(Location & FCH_ROM_RANGE_MASK);
          AddressValid = TRUE;
        }
      } else {
        AddressValid = FALSE;
      }
    }

    NumByte = Length;
    Status = PspReadFlash (Location, &NumByte, Data);
    if ((Status == EFI_NOT_FOUND) && (AddressValid == TRUE)) {
      CopyMem (Data, (VOID *)HostAddress, Length);
      Status = EFI_SUCCESS;
    }
  } else {
    if (mRom2Base != FCH_ROM_INVALID_START_ADDRESS_2) {
      if (CheckRom2Window((UINT32)Location, Length)) {
        HostAddress = Location;
        AddressValid = TRUE;
      } else if ((Location < FCH_ROM_SIZE_16M) && CheckRom2Offset ((UINT32)(Location & FCH_ROM_RANGE_MASK), Length)) {
        if (GetRom2Info(&Rom2Base, &Rom2Size)) {
          HostAddress = Rom2Base + (UINT32)(Location & FCH_ROM_RANGE_MASK);
          AddressValid = TRUE;
        }
      } else {
        AddressValid = FALSE;
      }
    }

    NumByte = Length;
    Status = PspReadFlash (Location, &NumByte, Data);
    if ((Status == EFI_NOT_FOUND) && (AddressValid == TRUE)) {
      CopyMem (Data, (VOID *)HostAddress, Length);
      Status = EFI_SUCCESS;
    }
  }


  return ((Status == EFI_SUCCESS) ? TRUE : FALSE);
}
