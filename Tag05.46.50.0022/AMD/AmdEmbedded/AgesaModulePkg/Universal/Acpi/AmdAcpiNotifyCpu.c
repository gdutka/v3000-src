/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Generate Notify CPU method and GPE LXX method.
 *
 * Contains implementation of the interfaces: Generate Notify CPU method and
 * GPE LXX method in AmdAcpiNotifyCpu.h. *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Universal
 * @e \$Revision$   @e \$Date$
 *
 */


#include "AGESA.h"
#include "Filecode.h"
#include "AmdAcpiDxe.h"
#include "AmdAcpiNotifyCpu.h"
#include <Library/AmdBaseLib.h>
#include "Library/AmdHeapLib.h"
#include <Library/BaseLib.h>
#include "AmdAcpiCpuSsdt.h"

#define FILECODE UNIVERSAL_ACPI_AMDACPINOTIFYCPU_FILECODE

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
AmdAcpiFillPkgLength (
  IN       UINT8                   *TableHead,
  IN OUT   UINTN                   *Offset,
  IN       UINT64                   Size,
  IN       UINT8                    Flag
  );

VOID
DigitsReverse (
  IN OUT   UINT64                  *Data,
  IN       UINT8                    Width
  );

VOID
HexToAscii (
  IN OUT   UINT64                  *Data,
  IN       UINT8                    Width
  );

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define ACPI_NAME_NFPR  0x5250464E
#define ACPI_NAME_NFPC  0x4350464E
#define ACPI_NAME_SHRM  0x4D524853
#define MIN_EXCURSION_NOTIFY_VALUE 0x84
#define HIGHEST_PERFORMANCE_CHANGED_NOTIFY_VALUE 0x85

#define BYTES_RSVD_FOR_METHOD_LENGTH 4
#define NFPR_METHOD_NAME_LENGTH      4
#define NFPC_METHOD_NAME_LENGTH      4
#define SHRM_NAME_LENGTH             4
#define CPPC_SHARED_MEMORY_SIZE_PER_PROC 0x100

#define WORD_LENGTH              2
#define DWORD_LENGTH             4
#define QWORD_LENGTH             8

#define AUTO_LENGTH              0
#define FIX_ONE_FOLLOW_BYTE      1
#define FIX_TWO_FOLLOW_BYTES     2
#define FIX_THREE_FOLLOW_BYTES   3

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdCreateNotifyCpuMethod
 *
 *  Description:
 *    This function will generate a method for notifying CPU device with 0x84.
 *    and the method will be called by either _GPE {_Lxx} or _EVT depends on if
 *    it's hardware-reduced or not
 *
 *  Parameters:
 *    @param[in,out]   SsdtPtr                    ACPI SSDT table pointer
 *    @param[in]       CoreCount                  Core count in system
 *    @param[in,out]   GpeScopeSize               Total size
 *
 *    @retval          AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *  AmdCreateNotifyCpuMethod creates below ASL code
 *
 *   Method(NFPR, 0, NotSerialized) {
 *     Store (0xXXXXXXXX, Local0)                             // 0xXXXXXXXX is BaseAddress of shared memory
 *     OperationRegion (SHRM, SystemMemory, Local0, 0xXXXX)   // SharedMemory, 0xXXXX is Length
 *       Field(SHRM, DWordAcc, NoLock, Preserve) {
 *            Offset (0x48),                                  // Offset 0x48, bit0 --- CPU 0
 *            V001, 1,
 *            Offset (0x148),                                 // Offset 0x148, bit0 --- CPU 1
 *            V002, 1,
 *            ...                                             // More fields for other CPUs
 *       }
 *
 *     if (Lequal(V001, 0x1)) {                               // If CPU 0's PerformanceLimitedRegister[0] is 1, notify CPU 0 with event 0x84
 *       Notify(\_PR.C000, 0x84)
 *     }
 *     if (Lequal(V002, 0x1)) {                               // If CPU 1's PerformanceLimitedRegister[0] is 1, notify CPU 1 with event 0x84
 *       Notify(\_PR.C001, 0x84)
 *     }
 *     ...                                                    // More notify for other CPUs
 *   }
 *
 *   Method(NFPC, 0, NotSerialized) {
 *     Notify(\_PR.C000, 0x85)                                // Notify CPU 0 with event 0x85
 *     Notify(\_PR.C001, 0x85)                                // Notify CPU 1 with event 0x85
 *     ...                                                    // More notify for other CPUs
 *   }
 *---------------------------------------------------------------------------------------
 */
AGESA_STATUS
AmdCreateNotifyCpuMethod (
  IN OUT   VOID                   **SsdtPtr,
  IN       UINT32                   CoreCount,
  IN OUT   UINT32                  *GpeScopeSize
  )
{
  UINT8    *NtfyMthd;
  UINTN     i;
  UINT32    CoreLoop;
  UINT64    Temp;
  UINT64    SharedRegion;
  UINT64    OperationRegionLength;
  UINT64    FieldLength;
  LOCATE_HEAP_PTR  LocateHeapParams;
  UINTN     MethodNFPROffset;
  UINTN     MethodNFPCOffset;
  BOOLEAN   ScopeWithContainer;
  UINT8     ProcessorContainerName[4+1] = {0};
  UINTN     NameIndex;
  BOOLEAN   FillTrailingUnderscore;

  // Get PCCT shared memory base address from heap
  LocateHeapParams.BufferHandle = AMD_PCCT_SHARED_REGION_ADDRESS_HANDLE;
  if (HeapLocateBuffer (&LocateHeapParams, NULL) != AGESA_SUCCESS) {
    return AGESA_ERROR;
  }
  SharedRegion = *((UINT64 *) LocateHeapParams.BufferPtr);
  ScopeWithContainer = PcdGetBool (PcdAmdAcpiCpuSsdtProcessorContainerInSb);
  if (ScopeWithContainer) {
    ASSERT (AsciiStrnLenS (PcdGetPtr (PcdAmdAcpiCpuSsdtProcessorContainerName), 5) <= 4);
    LibAmdMemCopy ((VOID *) &ProcessorContainerName[0],
                  (VOID *) PcdGetPtr (PcdAmdAcpiCpuSsdtProcessorContainerName),
                  AsciiStrnLenS (PcdGetPtr (PcdAmdAcpiCpuSsdtProcessorContainerName), 4),
                  NULL);
    if (ProcessorContainerName[0] == 0) {
      ProcessorContainerName[0] = SCOPE_NAME_P;
      ProcessorContainerName[1] = SCOPE_NAME_L;
      ProcessorContainerName[2] = SCOPE_NAME_T;
      ProcessorContainerName[3] = SCOPE_NAME_F;
    } else {
      // Fill trailing underscores
      FillTrailingUnderscore = FALSE;
      for (NameIndex = 0; NameIndex < 4; NameIndex++) {
        if ((FillTrailingUnderscore == TRUE) || (ProcessorContainerName[NameIndex] == 0)) {
          ProcessorContainerName[NameIndex] = (UINT8) '_';
          ASSERT (NameIndex != 0);
          FillTrailingUnderscore = TRUE;
        }
      }
    }
  }

  NtfyMthd = (UINT8 *) *SsdtPtr;

  // Method(NFPR, 0, NotSerialized)                          // MethodOp            PkgLength           NFPR                MethodFlags
                                                             // 14                  XX XX XX XX         4E 46 50 52         00
  i = 0;
  MethodNFPROffset = 1;
  NtfyMthd[i++] = METHOD_OPCODE;
  // Reserve offset 1 ~ 4 for PkgLength
  i += BYTES_RSVD_FOR_METHOD_LENGTH;
  *((UINT32 *) (NtfyMthd + i)) = ACPI_NAME_NFPR;
  i += NFPR_METHOD_NAME_LENGTH;
  NtfyMthd[i++] = 0;  // MethodFlags

  // Store (0xXXXXXXXX, Local0)                              // StoreOp             DWordPrefix         Data                Local0
                                                             // 70                  0C                  XX XX XX XX         60
  NtfyMthd[i++] = STORE_OPCODE;
  if (SharedRegion < 0x100000000) {
    NtfyMthd[i++] = DWORD_PREFIX_OPCODE;
    *((UINT32 *) (NtfyMthd + i)) = (UINT32) SharedRegion;
    i += DWORD_LENGTH;
  } else {
    NtfyMthd[i++] = QWORD_PREFIX_OPCODE;
    *((UINT64 *) (NtfyMthd + i)) = SharedRegion;
    i += QWORD_LENGTH;
  }
  NtfyMthd[i++] = LOCAL0_OPCODE;

  // OperationRegion(SHRM, SystemMemory, Local0, 0xXXXX)     // OpRegionOp          SHRM                SystemMemory        Local0              Word/DwordPrefix   Length
                                                             // 5B 80               53 48 52 4D         00                  60                  0B/0C              XX XX
  NtfyMthd[i++] = OP_REGION_OPCODE_1;
  NtfyMthd[i++] = OP_REGION_OPCODE_2;
  *((UINT32 *) (NtfyMthd + i)) = ACPI_NAME_SHRM;
  i += SHRM_NAME_LENGTH;
  NtfyMthd[i++] = 0;
  // SystemMemory
  NtfyMthd[i++] = LOCAL0_OPCODE;

  OperationRegionLength = (CPPC_SHARED_MEMORY_SIZE_PER_PROC * CoreCount) + 8;
  if (OperationRegionLength < 0x10000) {
    NtfyMthd[i++] = WORD_PREFIX_OPCODE;
    *((UINT16 *) (NtfyMthd + i)) = (UINT16) OperationRegionLength;
    i += WORD_LENGTH;
  } else if (OperationRegionLength < 0x100000000) {
    NtfyMthd[i++] = DWORD_PREFIX_OPCODE;
    *((UINT32 *) (NtfyMthd + i)) = (UINT32) OperationRegionLength;
    i += DWORD_LENGTH;
  } else {
    NtfyMthd[i++] = QWORD_PREFIX_OPCODE;
    *((UINT64 *) (NtfyMthd + i)) = OperationRegionLength;
    i += QWORD_LENGTH;
  }

  // Field(SHRM, DWordAcc, NoLock, Preserve)                 // FieldOp             PkgLength           SHRM                FieldFlags
                                                             // 5B 81               XX XX               53 48 52 4D         03 (DWordAcc/NoLock/Preserve)
  NtfyMthd[i++] = FIELD_OPCODE_1;
  NtfyMthd[i++] = FIELD_OPCODE_2;
  FieldLength = 6 + (8 * CoreCount); // size of 'Offset(0x??48)...' is 8, and if PkgLength is only 1 byte, then PkgLength = 6 + (8 * CoreCount)
  // Check if 1 byte is enough for PkgLength
  if (FieldLength > 0xFFFFF) {
    FieldLength += 3;
  } else if (FieldLength > 0xFFF) {
    FieldLength += 2;
  } else if (FieldLength > 0x3F) {
    FieldLength += 1;
  }
  AmdAcpiFillPkgLength (NtfyMthd, &i, FieldLength, AUTO_LENGTH);
  *((UINT32 *) (NtfyMthd + i)) = ACPI_NAME_SHRM;
  i += SHRM_NAME_LENGTH;
  NtfyMthd[i++] = 0x3;  // FieldFlags (DWordAcc/NoLock/Preserve)
  // Offset(0x48), V000, 1,                                  // ReservedField       BitsOffset          V000                BitsWidth
                                                             // 00                  40 24               56 30 30 30         01
  // Offset(0x148), V001, 1,                                 // ReservedField       BitsOffset          V001                BitsWidth
                                                             // 00                  4F 7F               56 30 30 31         01
                                                             // NOTE: BitsOffset for V001 = (0x148 - 0x48) * 8 - BitsWidth of V000 = 0x100 * 8 - 1 = 0x7FF
  for (CoreLoop = 0; CoreLoop < CoreCount; CoreLoop++) {
    if (CoreLoop == 0) {
      *((UINT64 *) (NtfyMthd + i)) = 0x0130303056244000;     // Offset(0x48), V000, 1,
    } else {
      *((UINT64 *) (NtfyMthd + i)) = 0x01000000567F4F00;     // Offset(0x??48), Vxxx, 1,
      Temp = CoreLoop;  // Vxxx, xxx should be replaced by core number, 001, 002...00n
      DigitsReverse (&Temp, 3);
      HexToAscii (&Temp, 3);
      *((UINT64 *) (NtfyMthd + i)) |= Temp << 32;
    }
    i += 8;
  }

  for (CoreLoop = 0; CoreLoop < CoreCount; CoreLoop++) {
    // If(LEqual(V000, One))                                   // IfOp                PkgLength           LEqualOp            V000                Data
                                                               // A0                  14/19               93                  56 30 30 30         01
    NtfyMthd[i++] = IF_OPCODE;
    if (ScopeWithContainer) {
      NtfyMthd[i++] = 0x19;  // PkgLength, fixed value of 0x19
    } else {
      NtfyMthd[i++] = 0x14;  // PkgLength, fixed value of 0x14
    }
    NtfyMthd[i++] = LEQUAL_OPCODE;

    *((UINT32 *) (NtfyMthd + i)) = 'V';  // Vxxx, xxx should be replaced by core number, 001, 002...00n
    Temp = CoreLoop;
    DigitsReverse (&Temp, 3);
    HexToAscii (&Temp, 3);
    *((UINT32 *) (NtfyMthd + i)) |= Temp << 8;
    i += 4;

    NtfyMthd[i++] = 1;

    // Notify(\_PR.C000, 0x84) or Notify(\_SB.C000, 0x84)      // NotifyOp  RootChar DualNamePrefix  _PR_ or _SB_              C000          BytePrefix  Data
                                                               // 86        5C       2E              5F 50 52 5F               43 30 30 30   0A          84
    // Notify(\_SB.PLTF.C000, 0x84)                            // NotifyOp  RootChar MultiNamePrefix _SB_         PLTF         C000          BytePrefix  Data
                                                               // 86        5C       2F 03           5F 53 42 5F  50 4C 54 46  43 30 30 30   0A          84
    NtfyMthd[i++] = NOTIFY_OPCODE;
    NtfyMthd[i++] = ROOT_CHAR_OPCODE;

    if (ScopeWithContainer) {
      NtfyMthd[i++] = MULTI_NAME_PREFIX_OPCODE;
      NtfyMthd[i++] = 0x03;
      NtfyMthd[i++] = '_';
      NtfyMthd[i++] = 'S';
      NtfyMthd[i++] = 'B';
      NtfyMthd[i++] = '_';

      // Container name
      NtfyMthd[i++] = ProcessorContainerName[0];
      NtfyMthd[i++] = ProcessorContainerName[1];
      NtfyMthd[i++] = ProcessorContainerName[2];
      NtfyMthd[i++] = ProcessorContainerName[3];
    } else {
      NtfyMthd[i++] = DUAL_NAME_PREFIX_OPCODE;
      NtfyMthd[i++] = '_';
      if (PcdGetBool (PcdAmdAcpiCpuSsdtProcessorScopeInSb)) {
        NtfyMthd[i++] = 'S';
        NtfyMthd[i++] = 'B';
      } else {
        NtfyMthd[i++] = 'P';
        NtfyMthd[i++] = 'R';
      }
      NtfyMthd[i++] = '_';
   }

    *((UINT32 *) (NtfyMthd + i)) = PcdGet8 (PcdAmdAcpiCpuSsdtProcessorScopeName0); //'C'
    Temp = CoreLoop;
    DigitsReverse (&Temp, 3);
    HexToAscii (&Temp, 3);
    *((UINT32 *) (NtfyMthd + i)) |= Temp << 8;
    i += 4;

    NtfyMthd[i++] = BYTE_PREFIX_OPCODE;
    NtfyMthd[i++] = MIN_EXCURSION_NOTIFY_VALUE;
  }

  // Set PkgLength of Method (NFPR)
  AmdAcpiFillPkgLength (NtfyMthd, &MethodNFPROffset, (i - MethodNFPROffset), FIX_THREE_FOLLOW_BYTES);

  // Method(NFPC, 0, NotSerialized)                            // MethodOp            PkgLength           NFPC                MethodFlags
                                                               // 14                  XX XX XX XX         4E 46 50 43         00
  MethodNFPCOffset = i + 1;
  NtfyMthd[i++] = METHOD_OPCODE;
  // Reserve offset 1 ~ 4 for PkgLength
  i += BYTES_RSVD_FOR_METHOD_LENGTH;
  *((UINT32 *) (NtfyMthd + i)) = ACPI_NAME_NFPC;
  i += NFPC_METHOD_NAME_LENGTH;
  NtfyMthd[i++] = 0;  // MethodFlags

  for (CoreLoop = 0; CoreLoop < CoreCount; CoreLoop++) {
    // Notify(\_PR.C000, 0x85) or Notify(\_SB.C000, 0x85)      // NotifyOp  RootChar DualNamePrefix  _PR_ or _SB_              C000          BytePrefix  Data
                                                               // 86        5C       2E              5F 50 52 5F               43 30 30 30   0A          85
    // Notify(\_SB.PLTF.C000, 0x85)                            // NotifyOp  RootChar MultiNamePrefix _SB_         PLTF         C000          BytePrefix  Data
                                                               // 86        5C       2F 03           5F 53 42 5F  50 4C 54 46  43 30 30 30   0A          85
    NtfyMthd[i++] = NOTIFY_OPCODE;
    NtfyMthd[i++] = ROOT_CHAR_OPCODE;

    if (ScopeWithContainer) {
      NtfyMthd[i++] = MULTI_NAME_PREFIX_OPCODE;
      NtfyMthd[i++] = 0x03;
      NtfyMthd[i++] = '_';
      NtfyMthd[i++] = 'S';
      NtfyMthd[i++] = 'B';
      NtfyMthd[i++] = '_';

      // Container name
      NtfyMthd[i++] = ProcessorContainerName[0];
      NtfyMthd[i++] = ProcessorContainerName[1];
      NtfyMthd[i++] = ProcessorContainerName[2];
      NtfyMthd[i++] = ProcessorContainerName[3];
    } else {
      NtfyMthd[i++] = DUAL_NAME_PREFIX_OPCODE;
      NtfyMthd[i++] = '_';
      if (PcdGetBool (PcdAmdAcpiCpuSsdtProcessorScopeInSb)) {
        NtfyMthd[i++] = 'S';
        NtfyMthd[i++] = 'B';
      } else {
        NtfyMthd[i++] = 'P';
        NtfyMthd[i++] = 'R';
      }
      NtfyMthd[i++] = '_';
   }

    *((UINT32 *) (NtfyMthd + i)) = PcdGet8 (PcdAmdAcpiCpuSsdtProcessorScopeName0); //'C'
    Temp = CoreLoop;
    DigitsReverse (&Temp, 3);
    HexToAscii (&Temp, 3);
    *((UINT32 *) (NtfyMthd + i)) |= Temp << 8;
    i += 4;

    NtfyMthd[i++] = BYTE_PREFIX_OPCODE;
    NtfyMthd[i++] = HIGHEST_PERFORMANCE_CHANGED_NOTIFY_VALUE;
  }

  // Set PkgLength of Method (NFPC)
  AmdAcpiFillPkgLength (NtfyMthd, &MethodNFPCOffset, (i - MethodNFPCOffset), FIX_THREE_FOLLOW_BYTES);

  // Update *SsdtPtr to point to the end of this table
  *SsdtPtr = ((UINT8 *) (*SsdtPtr)) + i;

  // Update GpeSocpeSize
  *GpeScopeSize += (UINT32) i;

  return AGESA_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdCreateGpe
 *
 *  Description:
 *    This function will generate _GPE {_Lxx} method
 *
 *  Parameters:
 *    @param[in,out]   SsdtPtr                    ACPI SSDT table pointer
 *    @param[in,out]   GpeScopeSize               Total size
 *
 *    @retval          AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *  AmdCreateGpe creates below ASL code
 *
 *  Scope(_GPE)
 *  {
 *    Method(_L18, 0, NotSerialized)                         // Per spec, we use GPE 24 for these event
 *    {
 *      NFPR()                                               // Call NFPR method
 *    }
 *  }
 *---------------------------------------------------------------------------------------
 */
AGESA_STATUS
AmdCreateGpe (
  IN OUT   VOID                   **SsdtPtr,
  IN OUT   UINT32                  *GpeScopeSize
  )
{
  // Scope(_GPE)                                             // ScopeOp             PkgLength           _GPE
                                                             // 10                  10                  5F 47 50 45
  // Method(_L18, 0, NotSerialized)                          // MethodOp            PkgLength           _L18                MethodFlags
                                                             // 14                  0A                  5F 4C 31 38         00
  // NFPR()                                                  // NFPR
                                                             // 4E 46 50 52

  //               [0]   [1]   [2]   [3]   [4]   [5]   [6]   [7]  [8]_  [9]L  [10]1 [11]8  offset10,11 should be replaced by PCD
  UINT8 GpeLxx[] = {0x10, 0x10, 0x5F, 0x47, 0x50, 0x45, 0x14, 0xA, 0x5F, 0x4C, 0x31, 0x38, 0x00, 0x4E, 0x46, 0x50, 0x52};
  UINT8 SciPin;
  UINT32 PcdSciPin;
  CONST UINT8 DigitToAscii[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  PcdSciPin = PcdGet32 (PcdCppcSciBitMap);
  if (PcdSciPin != 0) {
    for (SciPin = 0; PcdSciPin > 1; SciPin++) {
      PcdSciPin = PcdSciPin >> 1;
    }

    GpeLxx[10] = DigitToAscii[SciPin >> 4];
    GpeLxx[11] = DigitToAscii[SciPin & 0xF];

    LibAmdMemCopy (*SsdtPtr, GpeLxx, sizeof (GpeLxx), NULL);

    *SsdtPtr = ((UINT8 *) (*SsdtPtr)) + sizeof (GpeLxx);
    *GpeScopeSize += sizeof (GpeLxx);
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdAcpiFillPkgLength
 *
 *  Description:
 *    Generate PkgLength at *(TableHead + Offset) based on Size
 *
 *  Parameters:
 *    @param[in]       TableHead                  Table head
 *    @param[in,out]   Offset                     Offset of PkgLength
 *    @param[in]       Size                       Data size
 *    @param[in]       Flag                       How many follow bytes are in the PkgLength
 *
 *---------------------------------------------------------------------------------------
 */
VOID
AmdAcpiFillPkgLength (
  IN       UINT8                   *TableHead,
  IN OUT   UINTN                   *Offset,
  IN       UINT64                   Size,
  IN       UINT8                    Flag
  )
{
  // PkgLength LeadByte := bit 7-6: ByteData count that follows (0-3)
  //                       bit 5-4: Only used if PkgLength < 63
  //                       bit 3-0: Least significant package length nybble
  // The high 2 bits of the first byte reveal how many follow bytes are in the PkgLength. If the
  // PkgLength has only one byte, bit 0 through 5 are used to encode the package length (in other
  // words, values 0-63). If the package length value is more than 63, more than one byte must be
  // used for the encoding in which case bit 4 and 5 of the PkgLeadByte are reserved and must be
  // zero. If the multiple bytes encoding is used, bits 0-3 of the PkgLeadByte become the least
  // significant 4 bits of the resulting package length value. The next ByteData will become the next
  // least significant 8 bits of the resulting value and so on, up to 3 ByteData bytes. Thus, the maximum
  // package length is 2**28.

  UINT8  NumFollowByte;
  ASSERT (Size < 0x10000000);

  NumFollowByte = 0;
  switch (Flag) {
    case FIX_ONE_FOLLOW_BYTE:
      if (Size < 0x1000) {
        NumFollowByte = 1;
      }
      break;
    case FIX_TWO_FOLLOW_BYTES:
      if (Size < 0x100000) {
        NumFollowByte = 2;
      }
      break;
    case FIX_THREE_FOLLOW_BYTES:
      if (Size < 0x10000000) {
        NumFollowByte = 3;
      }
      break;
    default:
      break;
  }

  // Flag = AUTO_LENGTH Or Size doesn't match Flag
  if (NumFollowByte == 0) {
    if (Size < 0x3F) {
      NumFollowByte = 0;
    } else if (Size < 0x1000) {
      NumFollowByte = 1;
    } else if (Size < 0x100000) {
      NumFollowByte = 2;
    } else {
      NumFollowByte = 3;
    }
  }

  if (NumFollowByte == 0) {
    TableHead[(*Offset)++] = (UINT8) Size;
  } else if (NumFollowByte == 1) {
    TableHead[(*Offset)++] = (UINT8) (0x40 | (Size & 0xF));
    TableHead[(*Offset)++] = (UINT8) ((Size >> 4) & 0xFF);
  } else if (NumFollowByte == 2) {
    TableHead[(*Offset)++] = (UINT8) (0x80 | (Size & 0xF));
    TableHead[(*Offset)++] = (UINT8) ((Size >> 4) & 0xFF);
    TableHead[(*Offset)++] = (UINT8) ((Size >> 12) & 0xFF);
  } else if (NumFollowByte == 3) {
    TableHead[(*Offset)++] = (UINT8) (0xC0 | (Size & 0xF));
    TableHead[(*Offset)++] = (UINT8) ((Size >> 4) & 0xFF);
    TableHead[(*Offset)++] = (UINT8) ((Size >> 12) & 0xFF);
    TableHead[(*Offset)++] = (UINT8) ((Size >> 20) & 0xFF);
  }

  return;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  DigitsReverse
 *
 *  Description:
 *    0x12345 => 0x54321
 *
 *  Parameters:
 *    @param[in,out]   Data                       Data
 *    @param[in]       Width                      Number of digits you want to reverse
 *
 *---------------------------------------------------------------------------------------
 */
VOID
DigitsReverse (
  IN OUT   UINT64                  *Data,
  IN       UINT8                    Width
  )
{
  UINT8   i;
  UINT64  Low;
  UINT64  High;
  UINT64  Mask;

  for (i = 1; (i * 2) <= Width; i++) {
    Low  = ((*Data) >> (4 * (i - 1))) & 0xF;
    High = ((*Data) >> (4 * (Width - i))) & 0xF;


    Mask = (UINT64)0xF << (4 * (i - 1));
    Mask = ~Mask;
    *Data &= Mask;
    *Data |= High << (4 * (i - 1));

    Mask = (UINT64)0xF << (4 * (Width - i));
    Mask = ~Mask;
    *Data &= Mask;
    *Data |= Low << (4 * (Width - i));
  }

  return;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  HexToAscii
 *
 *  Description:
 *    0x1234 => 0x31323334
 *
 *  Parameters:
 *    @param[in,out]   Data                       Data
 *    @param[in]       Width                      Number of digits you want to convert to ASCii
 *
 *---------------------------------------------------------------------------------------
 */
VOID
HexToAscii (
  IN OUT   UINT64                  *Data,
  IN       UINT8                    Width
  )
{
  UINT8   i;
  UINT64  Temp;
  UINT64  Ascii;

  ASSERT (Width <= 8);

  Ascii = 0;
  for (i = 0; i < Width; i++) {
    Temp = ((*Data) >> (4 * i)) & 0xF;
    if (Temp < 0xA) {
      Temp = '0' + Temp;
    } else {
      Temp = 'A' + Temp - 0xA;
    }
    Ascii |= Temp << (8 * i);
  }
  *Data &= ~((1ull << (8 * Width)) - 1);
  *Data |= Ascii;

  return;
}

