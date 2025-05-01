/** @file
  SmmSupv Lib for PSP MMIO related function

  Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/SmmSupvSmnLib.h>
#include <Library/DebugLib.h>

#define IOHC_PSP_BASE_ADDR_LO_SMN_ADDR  0x13B102E0
#define IOHC_PSP_BASE_ADDR_HI_SMN_ADDR  0x13B102E4
#define MP0_C2PMSG_OFFSET_START         0x10500
#define MP0_C2PMSG_OFFSET_END           0x10B00
#define DRTM_OFFSET_START               0x10A20
#define DRTM_OFFSET_END                 0x10AAC
#define DRTM_SIZE                       (DRTM_OFFSET_END - DRTM_OFFSET_START)

/**
 * @brief   Get the PSP Mmio base and Drtm interface register Offset & size
 *
 * @param PspMmioBase
 * @param DrtmOffset
 * @param DrtmSize
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
GetDrtmMmio (
  OUT UINT64 *PspMmioBase,
  OUT UINT32 *DrtmOffset,
  OUT UINT32 *DrtmSize
  )
{
  if ((PspMmioBase == NULL) || (DrtmOffset == NULL) || (DrtmSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  *PspMmioBase = ((UINT64) SmmSupvSmnRegRead (IOHC_PSP_BASE_ADDR_HI_SMN_ADDR) << 32) | (SmmSupvSmnRegRead (IOHC_PSP_BASE_ADDR_LO_SMN_ADDR) & 0xFFF00000);
  *DrtmOffset = DRTM_OFFSET_START;
  *DrtmSize = DRTM_SIZE;

  return EFI_SUCCESS;
}

/**
 * @brief Check if PSP MMIO read is allowed, PSP MMIO is only allowed to access the MP::MP0CRU::MP0_C2PMSG which used as interface between
 *        X86 and PSP. C2P register used for DRTM interface is exception, as requirement of Level30, the write access to DRTM interface
 *        should be denied
 *
 * @param Offset  Offset to PSP MMIO base
 * @return EFI_STATUS EFI_SUCCESS: Allowed, EFI_WRITE_PROTECTED: DRTM region denied, EFI_ACCESS_DENIED: Access beyond MP0_C2PMSG region
 */
EFI_STATUS
EFIAPI
IsPspMmioWriteAllowed (
  IN UINT32 Offset
  )
{
  if ((Offset >= MP0_C2PMSG_OFFSET_START) && (Offset <= MP0_C2PMSG_OFFSET_END)) {
    if ((Offset >= DRTM_OFFSET_START) && (Offset <= DRTM_OFFSET_END)) {
      return EFI_WRITE_PROTECTED;
    }
    return EFI_SUCCESS;
  }
  return EFI_ACCESS_DENIED;
}

/**
 * @brief Check if PSP MMIO read is allowed, PSP MMIO is only allowed to access the MP::MP0CRU::MP0_C2PMSG which used as interface between
 *        X86 and PSP.
 *
 * @param Offset  Offset to PSP MMIO base
 * @return EFI_STATUS EFI_SUCCESS: Allowed, EFI_WRITE_PROTECTED: DRTM region denied, EFI_ACCESS_DENIED: Access beyond MP0_C2PMSG region
 */
EFI_STATUS
EFIAPI
IsPspMmioReadAllowed (
  IN UINT32 Offset
  )
{
  if ((Offset >= MP0_C2PMSG_OFFSET_START) && (Offset <= MP0_C2PMSG_OFFSET_END)) {
    return EFI_SUCCESS;
  }
  return EFI_ACCESS_DENIED;
}
/**
 * @brief Read from PSP MMIO offset
 *
 * @param Offset  Offset to PSP MMIO base
 * @return UINT32 Value read from offset
 */
UINT32
EFIAPI
SmmSupvPspMmioRead (
  IN UINT32 Offset
  )
{
  UINT64 PspMmioBase;
  UINT32 DrtmOffset;
  UINT32 DrtmSize;
  UINT32 Value;

  //Get PSP MMIO Base
  GetDrtmMmio (&PspMmioBase, &DrtmOffset, &DrtmSize);
  Value = *((volatile UINT32 *) (UINTN) (PspMmioBase + Offset));
  return Value;
}

/**
 * @brief Write PSP MMIO offset with given value
 *
 * @param Offset  Offset to PSP MMIO base
 * @param Value   Value to be written
 * @return VOID
 */
VOID
EFIAPI
SmmSupvPspMmioWrite (
  IN UINT32 Offset,
  IN UINT32 Value
  )
{
  UINT64 PspMmioBase;
  UINT32 DrtmOffset;
  UINT32 DrtmSize;

  //Get PSP MMIO Base
  GetDrtmMmio (&PspMmioBase, &DrtmOffset, &DrtmSize);
  *((volatile UINT32 *) (UINTN) (PspMmioBase + Offset)) = Value;
}