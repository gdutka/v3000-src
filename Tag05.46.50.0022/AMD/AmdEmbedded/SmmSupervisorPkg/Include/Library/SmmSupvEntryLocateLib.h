/*
*******************************************************************************

Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.
Copyright (c) Microsoft Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************
*/

#ifndef _SMM_SUPV_ENTRY_LOCATE_LIB_H_
#define _SMM_SUPV_ENTRY_LOCATE_LIB_H_

// {83e1f409-21a3-491d-a415-b163a153776d}
#define SMM_SUPV_DIR_FILE_GUID \
  {0x83e1f409, 0x21a3, 0x491d, {0xa4, 0x15, 0xb1, 0x63, 0xa1, 0x53, 0x77, 0x6d}}

enum SMM_SUPV_ENTRY_TYPE {
  SMM_SUPV_SIGNATURE                                  = 0xC0,           ///< SMM Supervisor signature
  SMM_SUPV_CORE_ENTRY_BINARY                          = 0xC1,           ///< Core SMM entry binary, which will be load to SMM_BASE+0x8000
  SMM_SUPV_CORE_ENTRY_BINARY_SIGNATURE                = 0xC2,           ///< Signature of Core SMM entry binary
  SMM_SUPV_PUB_KEY                                    = 0xC3,           ///< Public Key of signature
  SMM_SUPV_OEM_SECURE_POLICY                          = 0xC4,           ///< OEM Secure policy, report the white list of IO and MSR
};

/**
  Find the SmmEntry binary in SMMSUPV directory structure

  @param[in]  EntryType  Smm supervisor entry type.
  @param[out] Location   Location of SmmEntry binary Entry.
  @param[out] EntrySize  Size of SmmEntry binary.

  @retval  EFI_SUCCESS            Find smm supervisor successfully.
  @retval  EFI_OUT_OF_RESOURCES   Find smm supervisor successfully.
  @retval  EFI_NOT_FOUND          The smm supervisor could not be found.
  @retval  EFI_DEVICE_ERROR       A hardware error occurs during reading from the
                                  Firmware Volume.
  @retval  EFI_ACCESS_DENIED      The firmware volume containing the searched
                                  Firmware File is configured to disallow reads.
**/
EFI_STATUS
EFIAPI
FindSmmSupvEntry (
  IN  UINT32  EntryType,
  OUT VOID    **Location,
  OUT UINT32  *EntrySize
  );

#endif
