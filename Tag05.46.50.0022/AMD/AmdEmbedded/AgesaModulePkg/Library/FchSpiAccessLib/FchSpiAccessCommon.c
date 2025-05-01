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
#include <AMD.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/FchSpiAccessLib.h>
#include "FchRegistersCommon.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_FCHSPIACCESSLIB_FCHSPIACCESSCOMMON_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * GetRom2Info - Get ROM2 window Base address and Size
 *
 *
 * @param[out] Base - pointer to the buffer to store ROM2 base address
 * @param[out] Size - pointer to the buffer to store ROM2 window size
 *
 * @retval BOOLEAN Get successfully or not
 */

BOOLEAN
GetRom2Info (
  OUT    UINT32  *Base,
  OUT    UINT32  *Size
  )
{
  UINT16    Rom2StartAddress;
  UINT16    Rom2EndAddress;
  UINT32    StartAddress = 0, EndAddress = 0;
  UINTN     PciAddress;

  PciAddress = MAKE_SBDFO (0, 0, 0x14, 0x03, 0x6C);
  Rom2StartAddress = PciRead16 (PciAddress);
  StartAddress = ((UINT32)Rom2StartAddress << 16);
  StartAddress &= ~(UINT32)0xFFFF;

  PciAddress = MAKE_SBDFO (0, 0, 0x14, 0x03, 0x6E);
  Rom2EndAddress = PciRead16 (PciAddress);
  EndAddress = ((UINT32)Rom2EndAddress << 16);
  EndAddress |= (UINT32)0xFFFF;

  *Base = StartAddress;

  if(EndAddress > StartAddress) {
    *Size = EndAddress - StartAddress + 1;
  } else {
    return FALSE;
  }
  return TRUE;
}

/**
 * GetRom3Info - Get ROM3 window Base address and Size
 *
 *
 * @param[out] Base - pointer to the buffer to store ROM3 base address
 * @param[out] Size - pointer to the buffer to store ROM3 window size
 *
 * @retval BOOLEAN Get successfully or not
 */

BOOLEAN
GetRom3Info (
  OUT    UINT64  *Base,
  OUT    UINT32  *Size
  )
{
  UINT64    Rom3StartAddressLow, Rom3StartAddressHigh;
  UINT64    StartAddress = 0;
  UINTN     PciAddress;

  PciAddress = MAKE_SBDFO (0, 0, 0x14, 0x03, 0xA8);
  Rom3StartAddressLow = PciRead32 (PciAddress);

  PciAddress = MAKE_SBDFO (0, 0, 0x14, 0x03, 0xAC);
  Rom3StartAddressHigh = PciRead32 (PciAddress);

  StartAddress = ((UINT64)Rom3StartAddressHigh << 32);
  StartAddress |= (UINT64)(Rom3StartAddressLow & 0xFC000000);

  *Base = StartAddress;
  *Size = 0x3FFFFFF;  //64MB BIOS ROM space

  return TRUE;
}

/**
 * TransRom2Addr - Transfer Host ROM2 address to Offset of physical SPI ROM
 *
 *
 * @param[in] HostAddr - host ROM2 address
 *
 * @retval UINT32 Offset of physical SPI ROM
 */

UINT32
TransRom2Addr (
  IN     UINT32  HostAddr
  )
{
  UINT32    Spix30, Spix5C;
  UINT32    RomPage;

  RomPage = (HostAddr >> 24) & BIT0;
  Spix30 = MmioRead32 ((UINTN)(FCH_SPI_BASE_ADDRESS + FCH_SPI_MMIO_REG30));
  if (Spix30 & FCH_SPI_R2MSK24){
    RomPage &= ~FCH_SPI_R2VAL24;
    RomPage |= Spix30 & FCH_SPI_R2VAL24;
  }
  if (Spix30 & FCH_SPI_R2MSK25){
    RomPage &= ~FCH_SPI_R2VAL25;
    RomPage |= Spix30 & FCH_SPI_R2VAL25;
  }

  Spix5C = MmioRead32 ((UINTN)(FCH_SPI_BASE_ADDRESS + FCH_SPI_MMIO_REG5C_Addr32_Ctrl3));
  RomPage ^= (Spix5C & FCH_SPI_SPIROM_PAGE_MASK);

  return (UINT32)((RomPage << 24) | (HostAddr & FCH_ROM_RANGE_MASK));
}

/**
 * TransRom3Addr - Transfer Host ROM3 address to Offset of physical SPI ROM
 *
 *
 * @param[in] HostAddr - host ROM3 address
 *
 * @retval UINT32 Offset of physical SPI ROM
 */

UINT32
TransRom3Addr (
  IN     UINT64  HostAddr
  )
{
  UINT32    Spix5C;
  UINT64    RomPage;

  RomPage = (HostAddr >> 24) & BIT0;

  Spix5C = MmioRead32 ((UINTN)(FCH_SPI_BASE_ADDRESS + FCH_SPI_MMIO_REG5C_Addr32_Ctrl3));
  RomPage ^= (Spix5C & FCH_SPI_SPIROM_PAGE_MASK);

  return (UINT32)((RomPage << 24) | (HostAddr & FCH_ROM_RANGE_MASK));
}

