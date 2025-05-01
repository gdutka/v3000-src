/** @file
 * SmmSupv Lib for PSP MMIO related function

Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _SMMSUPV_PSP_MMIO_H_
#define _SMMSUPV_PSP_MMIO_H_
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
  );

/**
 * @brief Check if PSP MMIO read is allowed, PSP MMIO is only allowed to access the MP::MP0CRU::MP0_C2PMSG which used as interface between
 *        X86 and PSP.
 *
 * @param Offset  Offset to PSP MMIO base
 * @return EFI_STATUS EFI_SUCCESS: Allowed, EFI_WRITE_PROTECTED: DRTM region denied, EFI_NO_MAPPING: Access beyond MP0_C2PMSG region
 */
EFI_STATUS
EFIAPI
IsPspMmioReadAllowed (
  IN UINT32 Offset
  );

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
  );

/**
 * @brief Check if PSP MMIO read is allowed, PSP MMIO is only allowed to access the MP::MP0CRU::MP0_C2PMSG which used as interface between
 *        X86 and PSP. C2P register used for DRTM interface is exception, as requirement of Level30, the write access to DRTM interface
 *        should be denied
 *
 * @param Offset  Offset to PSP MMIO base
 * @return EFI_STATUS EFI_SUCCESS: Allowed, EFI_WRITE_PROTECTED: DRTM region denied, EFI_NO_MAPPING: Access beyond MP0_C2PMSG region
 */
EFI_STATUS
EFIAPI
IsPspMmioWriteAllowed (
  IN UINT32 Offset
  );


/**
 * @brief Read PSP MMIO with given offset
 *
 * @param Offset  Offset to PSP MMIO base
 * @param Value   Value to be write
 * @return VOID
 */
VOID
EFIAPI
SmmSupvPspMmioWrite (
  IN UINT32 Offset,
  IN UINT32 Value
  );

#endif // _SMMSUPV_PSP_MMIO_H_


