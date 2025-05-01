/** @file
  SmmSupv SMN related function

  Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>


#define NB_SMN_INDEX_2_PCI_ADDR             (PCI_LIB_ADDRESS (0, 0, 0, 0xB8))  ///< PCI Addr of NB_SMN_INDEX_2
#define NB_SMN_DATA_2_PCI_ADDR              (PCI_LIB_ADDRESS (0, 0, 0, 0xBC))  ///< PCI Addr of NB_SMN_DATA_2

/**
 * @brief Light version version SMN Read with less depx
 *
 * @param Address   SMN address
 * @return UINT32   Smn Register value
 */
UINT32
SmmSupvSmnRegRead (
  UINT32 Address
  )
{
  UINTN                  PciAddress;

  PciAddress = NB_SMN_INDEX_2_PCI_ADDR;
  PciWrite32 (PciAddress, Address);
  PciAddress = NB_SMN_DATA_2_PCI_ADDR;
  return PciRead32 (PciAddress);
}


/**
 * @brief Light version version SMN write with less depx
 *
 * @param Address
 * @param Value
 * @return VOID
 */
VOID
SmmSupvSmnRegWrite (
  UINT32 Address,
  UINT32 Value
  )
{
  UINTN                  PciAddress;

  PciAddress = NB_SMN_INDEX_2_PCI_ADDR;
  PciWrite32 (PciAddress, Address);
  PciAddress = NB_SMN_DATA_2_PCI_ADDR;
  PciWrite32 (PciAddress, Value);
}
