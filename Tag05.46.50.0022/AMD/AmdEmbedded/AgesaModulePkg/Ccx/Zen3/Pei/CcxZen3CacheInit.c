/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/*++
Module Name:

  CcxZen3CacheInit.c
  Initialize Cache

Abstract:
--*/

#include <Library/BaseLib.h>
#include <AGESA.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>

#define FILECODE CCX_ZEN3_PEI_CCXZEN3CACHEINIT_FILECODE

/* -----------------------------------------------------------------------------*/
/**
 *
 * Initialize vMTRRs and Program 0~TOM as WB.
 *
 */
VOID
InitializeVariableMTRRs (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64 LocalMsrRegister;
  UINT32 CurBase;
  UINT32 CurLimit;
  UINT32 CurSize;
  UINT32 CurAddr;
  UINT32 Value32;
  UINT32 Cache32bTOP;

  LocalMsrRegister = AsmReadMsr64 (TOP_MEM);
  Cache32bTOP = (UINT32) (RShiftU64 (LocalMsrRegister, 16) & 0xFFFFFFFF);

  CurBase = 0;
  CurLimit = Cache32bTOP;
  CurAddr = 0x200;

  while ((CurAddr >= 0x200) && (CurAddr < 0x20E) && (CurBase < Cache32bTOP)) {
    CurSize = CurLimit = (UINT32) 1 << (LowBitSet32 (CurBase) == -1 ? 31 : LowBitSet32 (CurBase)) ;
    CurLimit += CurBase;
    if ((CurBase == 0) || (Cache32bTOP < CurLimit)) {
      CurLimit = Cache32bTOP - CurBase;
      CurSize = CurLimit = (UINT32) 1 << (HighBitSet32 (CurLimit) == -1 ? 31 : HighBitSet32 (CurLimit));
      CurLimit += CurBase;
    }

    // prog. MTRR with current region Base
    LocalMsrRegister = (CurBase << 16) | 6;
    AsmWriteMsr64 (CurAddr, LocalMsrRegister);

    // prog. MTRR with current region Mask
    CurAddr++;                              // other half of MSR pair
    Value32 = CurSize - (UINT32) 1;
    Value32 = ~Value32;
    LocalMsrRegister = 0xFFFF00000000ull | (Value32 << 16) | ((UINT32) 1 << 11);
    AsmWriteMsr64 (CurAddr, LocalMsrRegister);

    CurBase = CurLimit;
    CurAddr++;                              // next MSR pair
  }

  if (CurLimit < Cache32bTOP) {
    // Announce failure
    IDS_ERROR_TRAP;
  }

  // Clear the vMTRR
  while ((CurAddr >= 0x200) && (CurAddr <= 0x20F)) {
    if ((CurAddr % 2) == 0) {
      AsmWriteMsr64 (CurAddr + 1, 0);
      AsmWriteMsr64 (CurAddr, 0);
      CurAddr += 2;
      continue;
    }
    CurAddr++;
  }
}

/*++

Routine Description:

  Zen3 cache initialization

Arguments:
  AMD_CONFIG_PARAMS *

Returns:

--*/
VOID
CcxZen3CacheInit (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  volatile UINT64        LocalMsrRegister;

  //
  //======================================================================
  // Set default values for CPU registers
  //======================================================================
  //
  AsmMsrOr64 (0xC0010010, BIT19);

  LocalMsrRegister = 0x1E1E1E1E1E1E1E1Eull;
  AsmWriteMsr64 (0x250, LocalMsrRegister);      // 0 - 512K = WB Mem
  LocalMsrRegister = 0x1E1E1E1E1E1E1E1Eull;
  AsmWriteMsr64 (0x258, LocalMsrRegister);      // 512K - 640K = WB Mem

  AsmMsrOr64 (0xC0010010, (BIT18 | BIT20));

  LocalMsrRegister = AsmReadMsr64 (0xC0010010);
  if ((LocalMsrRegister & BIT21) != 0) {
    LocalMsrRegister |= BIT22;
  }

  LocalMsrRegister &= ~BIT19;
  AsmWriteMsr64 (0xC0010010, LocalMsrRegister);

  //
  //======================================================================
  // Set IORR Registers
  //======================================================================
  //
  LocalMsrRegister = AsmReadMsr64 (0xC001001D);
  if (LocalMsrRegister >= 0xFD00000000) {
    // Decode FD_0000_0000 ~ FD_03FF_FFFF(64MB) as MMIO for ROM3
    AsmWriteMsr64 (0xC0010016, 0x00FD00000000);
    AsmWriteMsr64 (0xC0010017, 0x00FFFC000800);
  }
}


