/*;********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include "FchPlatform.h"
#include "GnbDxio.h"
#include <Library/NbioSmuV13Lib.h>

#define FILECODE FCH_KEITH_FCHKEITHCORE_COMMON_FCHLIB_FILECODE

VOID
FchPciReset (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8   PciRstValue;

  PciRstValue = 0x06;
  LibAmdIoWrite (AccessWidth8, FCH_PCIRST_BASE_IO, &PciRstValue, StdHeader);
}

/**< outPort80 - Reserved  */
VOID
OutPort80 (
  IN       UINT32              pcode,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdIoWrite (AccessWidth8, FCHOEM_OUTPUT_DEBUG_PORT, &pcode, StdHeader);
  return;
}

/**< outPort1080 - Reserved  */
VOID
OutPort1080 (
  IN       UINT32              pcode,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdIoWrite (AccessWidth32, 0x1080, &pcode, StdHeader);
  return;
}

/**< FchCopyMem - Reserved  */
VOID
FchCopyMem (
  IN      VOID*   pDest,
  IN      VOID*   pSource,
  IN      UINTN   Length
  )
{
  UINTN  i;
  UINT8  *Ptr;
  UINT8  *Source;
  Ptr = (UINT8*)pDest;
  Source = (UINT8*)pSource;
  for (i = 0; i < Length; i++) {
    *Ptr = *Source;
    Source++;
    Ptr++;
  }
}

/** GetRomSigPtr - Reserved **/
VOID*
GetRomSigPtr (
  IN      UINTN               *RomSigPtr,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32   RomPtr;
  UINT32   RomSig;
  UINT16   MswAddr;

  *RomSigPtr = 0;
  MswAddr = 0xFF00;
  do {
    RomPtr = (MswAddr << 16) + FCH_ROMSIG_BASE_IO;
    LibAmdMemRead (AccessWidth32, (UINT64) RomPtr, &RomSig, StdHeader);
    if (RomSig == FCH_ROMSIG_SIGNATURE) {
      *RomSigPtr = RomPtr;
      break;
    }
    MswAddr >>= 1;
    MswAddr |= BIT15;
  } while (MswAddr != 0xFFFC);//Change from 0xFFF8 ro 0xFFFC to cover ROM_SIG@0xFFFA0000
  if (*RomSigPtr == 0) {
    //Error Condition Embedded Firmware Structure not found
    IDS_HDT_CONSOLE (FCH_TRACE, "GetRomSigPtr failed to find Ptr HALT\n" );
    FCH_DEADLOOP();
  }
  return RomSigPtr;
}

/** AcLossControl - Reserved **/
VOID
AcLossControl (
  IN       UINT8 AcLossControlValue
  )
{
  AcLossControlValue &= 0x03;
  AcLossControlValue |= BIT2;
  RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG5B, AccessWidth8, 0xF0, AcLossControlValue);
}

/** RecordFchConfigPtr - Reserved **/
VOID
RecordFchConfigPtr (
  IN       UINT32 FchConfigPtr
  )
{
  RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x08, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 0) & 0xFF) );
  RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x09, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 8) & 0xFF) );
  RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0A, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 16) & 0xFF) );
  RwMem (ACPI_MMIO_BASE + CMOS_RAM_BASE + 0x0B, AccessWidth8, 0, (UINT8) ((FchConfigPtr >> 24) & 0xFF) );
}

/** ReadAlink - Reserved **/
UINT32
ReadAlink (
  IN       UINT32              Index,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 Data;
  LibAmdIoWrite (AccessWidth32, ALINK_ACCESS_INDEX, &Index, StdHeader);
  LibAmdIoRead (AccessWidth32, ALINK_ACCESS_DATA, &Data, StdHeader);
  //Clear Index
  Index = 0;
  LibAmdIoWrite (AccessWidth32, ALINK_ACCESS_INDEX, &Index, StdHeader);
  return Data;
}

/** WriteAlink - Reserved **/
VOID
WriteAlink (
  IN       UINT32              Index,
  IN       UINT32              Data,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  LibAmdIoWrite (AccessWidth32, ALINK_ACCESS_INDEX, &Index, StdHeader);
  LibAmdIoWrite (AccessWidth32, ALINK_ACCESS_DATA, &Data, StdHeader);
  //Clear Index
  Index = 0;
  LibAmdIoWrite (AccessWidth32, ALINK_ACCESS_INDEX, &Index, StdHeader);
}

/** RwAlink - Reserved **/
VOID
RwAlink (
  IN       UINT32              Index,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 AccessType;

  AccessType = Index & 0xE0000000;
  if (AccessType == (AXINDC << 29)) {
    WriteAlink ((FCH_AX_INDXC_REG30 | AccessType), Index & 0x1FFFFFFF, StdHeader);
    Index = FCH_AX_DATAC_REG34 | AccessType;
  } else if (AccessType == (AXINDP << 29)) {
    WriteAlink ((FCH_AX_INDXP_REG38 | AccessType), Index & 0x1FFFFFFF, StdHeader);
    Index = FCH_AX_DATAP_REG3C | AccessType;
  }
  WriteAlink (Index, (ReadAlink (Index, StdHeader) & AndMask) | OrMask, StdHeader);
}


/*----------------------------------------------------------------------------------------*/
/**
 * Read PMIO
 *
 *
 *
 * @param[in] Address  - PMIO Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Read Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
ReadPmio (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  ReadMem (
    (UINT32)(ACPI_MMIO_BASE + PMIO_BASE + (UINT32)Address),
    OpFlag,
    Value
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Write PMIO
 *
 *
 *
 * @param[in] Address  - PMIO Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Write Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
WritePmio (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  WriteMem (
    (UINT32)(ACPI_MMIO_BASE + PMIO_BASE + (UINT32)Address),
    OpFlag,
    Value
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * RwPmio - Read/Write PMIO
 *
 *
 *
 * @param[in] Address    - PMIO Offset value
 * @param[in] OpFlag     - Access sizes
 * @param[in] AndMask    - Data And Mask 32 bits
 * @param[in] OrMask     - Data OR Mask 32 bits
 * @param[in] StdHeader
 *
 */
VOID
RwPmio (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 Result;

  ReadPmio (Address, OpFlag, &Result, StdHeader);
  Result = (Result & AndMask) | OrMask;
  WritePmio (Address, OpFlag, &Result, StdHeader);
}


/*----------------------------------------------------------------------------------------*/
/**
 * Read PMIO2
 *
 *
 *
 * @param[in] Address  - PMIO2 Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Read Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
ReadPmio2 (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  ReadMem (
    (UINT32)(ACPI_MMIO_BASE + PMIO2_BASE + (UINT32)Address),
    OpFlag,
    Value
    );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Write PMIO 2
 *
 *
 *
 * @param[in] Address  - PMIO2 Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Write Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
WritePmio2 (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  WriteMem (
    (UINT32)(ACPI_MMIO_BASE + PMIO2_BASE + (UINT32)Address),
    OpFlag,
    Value
    );
}


/*----------------------------------------------------------------------------------------*/
/**
 * RwPmio2 - Read/Write PMIO2
 *
 *
 *
 * @param[in] Address    - PMIO2 Offset value
 * @param[in] OpFlag     - Access sizes
 * @param[in] AndMask    - Data And Mask 32 bits
 * @param[in] OrMask     - Data OR Mask 32 bits
 * @param[in] StdHeader
 *
 */
VOID
RwPmio2 (
  IN       UINT8 Address,
  IN       UINT8  OpFlag,
  IN       UINT32 AndMask,
  IN       UINT32 OrMask,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 Result;

  ReadPmio2 (Address, OpFlag, &Result, StdHeader);
  Result = (Result & AndMask) | OrMask;
  WritePmio2 (Address, OpFlag, &Result, StdHeader);
}


/*----------------------------------------------------------------------------------------*/
/**
 * Read BIOSRAM
 *
 *
 *
 * @param[in] Address  - BIOSRAM Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Read Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
ReadBiosram (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  ReadMem (
    (UINT32)(ACPI_MMIO_BASE + BIOS_RAM_BASE + (UINT32)Address),
    OpFlag,
    Value
    );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Write BIOSRAM
 *
 *
 *
 * @param[in] Address  - BIOSRAM Offset value
 * @param[in] OpFlag   - Access sizes
 * @param[in] Value    - Write Data Buffer
 * @param[in] StdHeader
 *
 */
VOID
WriteBiosram (
  IN       UINT8               Address,
  IN       UINT8               OpFlag,
  IN       VOID                *Value,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  WriteMem (
    (UINT32)(ACPI_MMIO_BASE + BIOS_RAM_BASE + (UINT32)Address),
    OpFlag,
    Value
    );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Record SMI Status
 *
 *
 * @param[in] StdHeader
 *
 */
VOID
RecordSmiStatus (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINTN   Index;
  UINT8   SwSmiValue;

  ACPIMMIO8 (0xfed80304) |= 0x01;
  for ( Index = 0; Index < 20; Index++ ) {
    ACPIMMIO8 (0xfed10020 + Index) = ACPIMMIO8 (0xfed80280 + Index);
  }
  LibAmdIoRead (AccessWidth8, 0xB0, &SwSmiValue, StdHeader);
  ACPIMMIO8 (0xfed10040) = SwSmiValue;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Clear All SMI Status
 *
 *
 * @param[in] StdHeader
 *
 */
VOID
ClearAllSmiStatus (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINTN   Index;

  for ( Index = 0; Index < 20; Index++ ) {
    ACPIMMIO8 (0xfed80280 + Index) |= 0;
  }
}

/**
 * FchKTSmuService Request -  FCH Keith SMU Service Request
 *
 *
 * @param[in] RequestId           Request ID.
 *
 */
BOOLEAN
FchKTSmuService (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  UINT32    Controller
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];

  status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  SmuArg[0] = Controller;
  if (NbioSmuServiceRequestV13 (NbioPciAddress, RequestId, SmuArg,0)) {
    status = TRUE;
  }

  return status;
}


/**
 * FchKTSmuService Request -  FCH Keith SMU Service Request
 *
 *
 * @param[in] RequestId           Request ID.
 *
 */
BOOLEAN
FchKTSmuServiceOnly (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  UINT32    Controller
  )
{
  BOOLEAN status;
  PCI_ADDR        NbioPciAddress;
  UINT32          SmuArg[6];

  status = FALSE;
  NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
  NbioPciAddress.AddressValue = MAKE_SBDFO (0, DieBusNum, 0, 0, 0);
  SmuArg[0] = Controller;
  if (NbioSmuServiceRequestOnlyV13 (NbioPciAddress, RequestId, SmuArg,0)) {
    status = TRUE;
  }

  return status;
}

