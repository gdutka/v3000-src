/*
 ******************************************************************************
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
#include <Library/BaseLib.h>
#include "AMD.h"
#include "Filecode.h"
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/CcxBaseX86Lib.h>

#define FILECODE LIBRARY_CCXBASEX86LIB_CCXBASEX86LIB_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the MMIO address used for PCI extended configuration space
 *
 * @param[in]     MmioAddress   Mmio Address
 * @param[in]     MmioSize      Mmio Size
 * @param[in]     StdHeader     Standard configuration header (Optional)
 */
BOOLEAN
CcxGetPciMmioAddress (
     OUT   UINT64            *MmioAddress,
     OUT   UINT32            *MmioSize,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN MmioIsEnabled;
  UINT32  EncodedSize;
  UINT64  LocalMsrRegister;

  MmioIsEnabled = FALSE;
  LocalMsrRegister = AsmReadMsr64 (0xC0010058);
  if ((LocalMsrRegister & BIT0) != 0) {
    *MmioAddress = LocalMsrRegister & 0xFFFFFFFFFFF00000;
    EncodedSize = (UINT32) RShiftU64 ((LocalMsrRegister & 0x3C), 2);
    *MmioSize = ((1 << EncodedSize) * 0x100000);
    MmioIsEnabled = TRUE;
  }
  return MmioIsEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the package type mask for the processor
 *
 *
 * @param[in]     StdHeader     Standard configuration header (Optional)
 */
UINT32
CcxGetProcessorPackageType (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32      ProcessorPackageType;
  CPUID_DATA  CpuId;

  AsmCpuid (
      0x80000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  ProcessorPackageType = (UINT32) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28
  return (UINT32) (1 << ProcessorPackageType);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the core's APIC ID at reset
 *
 *
 * @param[in]     StdHeader     Standard configuration header (Optional)
 */
UINT32
CcxGetInitialApicID (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  CPUID_DATA  CpuId;

  AsmCpuid (
      0x00000001,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
      );
  return ((CpuId.EBX_Reg >> 24) & 0xFF);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the value of ThreadsPerCore
 *
 *
 * @retval Value of ThreadsPerCore + 1
 */
UINT8
CcxGetThreadsPerCore (
  )
{
  CPUID_DATA  CpuId;

  AsmCpuid (
      0x8000001E,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
    );

  return (UINT8) (((CpuId.EBX_Reg >> 8) & 0xFF) + 1);

}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the value of PhysAddrSize
 *
 *
 * @retval Value of CPUID_Fn8000_00008_EAX[7:0, PhysAddrSize]
 */
UINT8
CcxGetPhysAddrSize (
  )
{
  CPUID_DATA  CpuId;

  AsmCpuid (
      0x80000008,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
    );

  return (UINT8) (CpuId.EAX_Reg & 0xFF);

}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the value of ApicIdCoreIdSize
 *
 *
 * @retval Value of CPUID_Fn8000_00008_ECX[15:12, ApicIdCoreIdSize]
 */
UINT8
CcxGetApicIdCoreIdSize (
  )
{
  CPUID_DATA  CpuId;

  AsmCpuid (
      0x80000008,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
    );

  return (UINT8) ((CpuId.ECX_Reg >> 12) & 0xF);

}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the value of LFuncExt
 *
 *
 * @retval Value of CPUID_Fn8000_00000_EAX[31:0, LFuncExt]
 */
UINT32
CcxGetLFuncExt (
  )
{
  CPUID_DATA  CpuId;

  AsmCpuid (
      0x80000000,
      &(CpuId.EAX_Reg),
      &(CpuId.EBX_Reg),
      &(CpuId.ECX_Reg),
      &(CpuId.EDX_Reg)
    );

  return (CpuId.EAX_Reg);

}

