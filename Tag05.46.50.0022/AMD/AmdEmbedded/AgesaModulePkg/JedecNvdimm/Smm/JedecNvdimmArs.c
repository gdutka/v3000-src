/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/** @file NvdimmARS.c
    Address Range Scrubber (ARS) Driver Implementation

**/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Filecode.h"
#include <Library/DebugLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <MemDmi.h>
#include <Addendum/Apcb/Inc/SSP/APOB.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/JedecNvdimmArs.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/PspMboxSmmBufferAddressProtocol.h>
#include "JedecNvdimmAcpi.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE JEDECNVDIMM_SMM_JEDECNVDIMMARS_FILECODE
#define MAX_NUM_NFIT_STRUCTS  (ABL_APOB_MAX_SOCKETS_SUPPORTED * ABL_APOB_MAX_CHANNELS_PER_SOCKET * ABL_APOB_MAX_DIMMS_PER_CHANNEL - 1)

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
UINT8                   *mPspMboxSmmBuffer = NULL;
BOOLEAN                 *mPspMboxSmmFlagAddr = NULL;
VOID                    *mArsSpaRangeTables = NULL;
UINT8                   mCacheLineLength = 0;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
EFIAPI
AsmClzero (
    IN  UINT64  Addrs
    );

UINT8
ChecksumAcpiTable (
    IN    NFIT_HEADER   *Table
    );

EFI_STATUS
InitSmmNvdimmSpaRangeTable ();

EFI_STATUS
EFIAPI
NvdimmArsQueryArsUnitLength (
    OUT   UINT64  *UnitLength
    );

EFI_STATUS
EFIAPI
NvdimmArsClearUncorrectableError (
    IN    UINT64   SpaRangeBase,
    IN    UINT64   SpaRangeLength
    );

EFI_STATUS
EFIAPI
NvdimmArsStartArs (
    IN    UINT64    SpaStartAddress,
    IN    UINT64    SpaEndAddress,
    IN    UINT64    ScrubGranularity,
    IN    BOOLEAN   HighPriority
    );

EFI_STATUS
EFIAPI
NvdimmArsStopArs ();

EFI_STATUS
EFIAPI
NvdimmArsSpaIsPersistent (
    IN    UINT64                                                      Spa,
    OUT   BOOLEAN                                                     *Presistent,
    OUT   UINT32                                                      *NvdimmNfitHandle,
    OUT   EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE   *NfitSpaRangeStruc
    );

EFI_STATUS
EFIAPI
NvdimmArsSpaIsMirroredInterleaveSet (
    IN    UINT64      Spa
    );

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
AMD_NVDIMM_ARS_SMM_PROTOCOL AmdNvdimmArsSmmProtocol = {
    NvdimmArsQueryArsUnitLength,
    NvdimmArsClearUncorrectableError,
    NvdimmArsStartArs,
    NvdimmArsStopArs,
    NvdimmArsSpaIsPersistent,
    NvdimmArsSpaIsMirroredInterleaveSet
};

#pragma pack (push, 1)

typedef struct {
  UINT32            NvdimmNfitHandle;
  SPA_RANGE_STRUCT  SpaRangeStruc;
} ARS_SPA_RANGE_STRUCT;

#pragma pack (pop)

/*---------------------------------------------------------------------------------------*/
/**
 * Calculate an ACPI style checksum
 *
 * Computes the checksum of the passed in ACPI table.
 *
 * @param[in]  Table             ACPI table to checksum
 * @param[out] Checksum          Calculated checksum
 *
 */
/*---------------------------------------------------------------------------------------*/
UINT8
ChecksumAcpiTable (
    IN    NFIT_HEADER        *Table
   )
{
    UINT8   *BuffTempPtr;
    UINT8   Checksum;
    UINT32  BufferOffset;

    Checksum = Table->Checksum;
    BuffTempPtr = (UINT8 *) Table;

    for (BufferOffset = 0; BufferOffset < Table->Length; BufferOffset++) {
      Checksum = Checksum - *(BuffTempPtr + BufferOffset);
    }

    return Checksum;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: InitSmmNvdimmSpaRangeTable
 *
 * Description
 *   Find ACPI NFIT table and copy SPA_RANGE_STRUCTs and corresponding
 *   NFIT device handles to SMM accessible table (mArsSpaRangeTables).
 *
 *  @retval EFI_SUCCESS
 *
 */
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
InitSmmNvdimmSpaRangeTable ()
{
  EFI_ACPI_SDT_PROTOCOL       *AcpiSdtProtocol;
  EFI_ACPI_SDT_HEADER         *AcpiTbl;
  EFI_ACPI_TABLE_VERSION      Version;
  UINTN                       AcpiTblIndex;
  UINTN                       AcpiHandle;
  NFIT_HEADER                 *NfitHeaderPtr;
  SPA_RANGE_STRUCT            *NfitSpaRangeStrucPtr;
  REGION_MAPPING_STRUCT       *NfitRegMapStrucPtr;
  UINT16                      NfitNumOfStrucs;
  ARS_SPA_RANGE_STRUCT        *ArsSpaRangeBufPtr;
  VOID                        *ArsSpaRangeTblPtr;
  UINT16                      NumOfSpaStrucs;
  UINT16                      NumOfHandlesCopied;
  UINT16                      i;
  UINT16                      j;
  EFI_STATUS                  Status;

  // get NFIT ACPI table
  Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, &AcpiSdtProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (AcpiTblIndex=0; ; AcpiTblIndex++) {
    AcpiTbl = NULL;
    Status = AcpiSdtProtocol->GetAcpiTable (AcpiTblIndex, &AcpiTbl, &Version, &AcpiHandle);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      // no more tables, NFIT table not found
      return Status;
    }

    if (AcpiTbl->Signature == NFIT_SIGNATURE) {    // ('N', 'F', 'I', 'T')
      // found NFIT table
      NfitHeaderPtr = (NFIT_HEADER*)AcpiTbl;
      break;
    }
    // GetAcpiTable allocates pool, need to free it here
    if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
      gBS->FreePool((VOID *) AcpiTbl);
    }
  }

  // verify creator ID, rev, & checksum of NFIT table
  ASSERT(*((UINT32*)NfitHeaderPtr->CreatorId) == SIGNATURE_32('A', 'M', 'D', ' '));
  ASSERT(NfitHeaderPtr->Revision == 1);
  ASSERT(NfitHeaderPtr->OemRev == 1);
  ASSERT(NfitHeaderPtr->CreatorRev == 1);
  ASSERT(NfitHeaderPtr->Checksum == ChecksumAcpiTable(NfitHeaderPtr));

  // Allocate temp memory buffer for ARS SPA range
  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  (MAX_NUM_NFIT_STRUCTS * sizeof (ARS_SPA_RANGE_STRUCT)),
                  (VOID**)&ArsSpaRangeBufPtr
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (ArsSpaRangeBufPtr, (MAX_NUM_NFIT_STRUCTS * sizeof (ARS_SPA_RANGE_STRUCT)));

  // get start of SPA_RANGE_STRUCT region in NFIT table
  NfitSpaRangeStrucPtr = (SPA_RANGE_STRUCT*)((UINT8*)NfitHeaderPtr + sizeof(NFIT_HEADER));

  // copy SPA range structures from NFIT table to ARS SPA range buffer
  NumOfSpaStrucs = 0;
  NfitNumOfStrucs = 0;
  ArsSpaRangeTblPtr = (VOID*)ArsSpaRangeBufPtr;
  while ((NfitSpaRangeStrucPtr->Type == NFIT_SPA_RANGE_STRUC_TYPE) && (NfitNumOfStrucs < MAX_NUM_NFIT_STRUCTS)) {
    // SPA range index of 0: region doesn't map to a SPA range, so exclude
    if (NfitSpaRangeStrucPtr->SpaRangeStructIndex != 0) {
      gBS->CopyMem (
            (VOID*)(&ArsSpaRangeBufPtr->SpaRangeStruc),
            (VOID*)NfitSpaRangeStrucPtr,
            sizeof (ARS_SPA_RANGE_STRUCT)
            );
      ArsSpaRangeBufPtr++;
      NumOfSpaStrucs++;
    }
    NfitSpaRangeStrucPtr++;
    NfitNumOfStrucs++;
  }
  ASSERT(NfitNumOfStrucs <= MAX_NUM_NFIT_STRUCTS);

  // get start of REGION_MAPPING_STRUCT region in NFIT table
  NfitRegMapStrucPtr = (REGION_MAPPING_STRUCT*)((UINT8*)NfitSpaRangeStrucPtr + (NfitNumOfStrucs * sizeof(CONTROL_REGION_STRUCT)));

  // find REGION_MAPPING_STRUCT in NFIT that corresponds
  // to SPA_RANGE_STRUCT in ARS SPA range buffer &
  // copy NVDIMM NFIT Handles from NFIT table to ARS SPA range buffer
  NumOfHandlesCopied = 0;
  for (i=0; i < NfitNumOfStrucs; i++, NfitRegMapStrucPtr++) {
    ASSERT(NfitRegMapStrucPtr->Type == NFIT_NVDIMM_REG_MAP_STRUC_TYPE);
    ArsSpaRangeBufPtr = (ARS_SPA_RANGE_STRUCT*)ArsSpaRangeTblPtr;
    for (j=0; j < NumOfSpaStrucs; j++, ArsSpaRangeBufPtr++) {
      if (NfitRegMapStrucPtr->SpaRangeStructIndex == ArsSpaRangeBufPtr->SpaRangeStruc.SpaRangeStructIndex) {
        ArsSpaRangeBufPtr->NvdimmNfitHandle = NfitRegMapStrucPtr->NfitDeviceHandle;
        NumOfHandlesCopied++;
        break;
      }
    }
    if (NumOfHandlesCopied == NumOfSpaStrucs) {
      // all handles found & copied, so stop looking
      break;
    }
  }
  ASSERT(NumOfHandlesCopied == NumOfSpaStrucs);

  // GetAcpiTable allocates pool, need to free it here
  if (!PcdGetBool (PcdCompliantEdkIIAcpiSdtProtocol)) {
    gBS->FreePool((VOID *) NfitHeaderPtr);
  }

  // Allocate Memory for SPA range tables in SMM
  Status = gBS->AllocatePool (
                EfiReservedMemoryType,
                (sizeof(NumOfSpaStrucs)+(NumOfSpaStrucs * sizeof (ARS_SPA_RANGE_STRUCT))),
                &mArsSpaRangeTables);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    // free ARS SPA range temp buffer
    gBS->FreePool (ArsSpaRangeTblPtr);
    return Status;
  }

  // place total number of ARS SPA structs at top of ARS SPA range table
  *((UINT16*)mArsSpaRangeTables) = NumOfSpaStrucs;

  // copy SPA range structs to ARS SPA range table
  gBS->CopyMem (
        (VOID*)((UINT8*)mArsSpaRangeTables + (sizeof(NumOfSpaStrucs))),
        ArsSpaRangeTblPtr,
        (NumOfSpaStrucs * sizeof (ARS_SPA_RANGE_STRUCT))
        );

  // free ARS SPA range temp buffer
  gBS->FreePool (ArsSpaRangeTblPtr);
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: NvdimmArsQueryArsUnitLength
 *
 * Description
 *   Query the Clear Unit Length.
 *
 *  @param[out]  UnitLength
 *
 *  @retval EFI_SUCCESS
 *
 */
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
NvdimmArsQueryArsUnitLength (
    OUT   UINT64  *UnitLength
    )
{
  EFI_STATUS  Status = EFI_SUCCESS;

  *UnitLength = mCacheLineLength;     // always clear one cache line at a time

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: NvdimmArsClearUncorrectableError
 *
 * Description
 *   Clear an uncorrectable error from a persistent memory location.
 *
 *  @param[in]    SpaRangeBase
 *  @param[in]    SpaRangeLength
 *
 *  @retval EFI_SUCCESS
 *
 */
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
NvdimmArsClearUncorrectableError (
    IN    UINT64   SpaRangeBase,
    IN    UINT64   SpaRangeLength
    )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64      Addrs;

  for (Addrs = SpaRangeBase; Addrs < (SpaRangeBase + SpaRangeLength); Addrs += mCacheLineLength) {
    AsmClzero (Addrs);
  }

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: NvdimmArsStartArs
 *
 * Description
 *   Start an ARS Scrub.
 *
 *  @param[in]    SpaStartAddress
 *  @param[in]    SpaEndAddress
 *  @param[in]    ScrubGranularity
 *  @param[in]    HighPriority
 *
 *  @retval EFI_SUCCESS or EFI_INVALID_PARAMETER
 *
 */
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
NvdimmArsStartArs (
    IN    UINT64    SpaStartAddress,
    IN    UINT64    SpaEndAddress,
    IN    UINT64    ScrubGranularity,
    IN    BOOLEAN   HighPriority
    )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  ARS_BUFFER    OperationBuf;

  ASSERT(SpaEndAddress > SpaStartAddress);
  OperationBuf.SPAStartAddress  = SpaStartAddress;
  OperationBuf.SPALength        = (SpaEndAddress-SpaStartAddress);
  OperationBuf.ScrubGranularity = ScrubGranularity;
  OperationBuf.Priority         = HighPriority;

  Status = PspMboxBiosArs (ArsActionStart, &OperationBuf, mPspMboxSmmBuffer, mPspMboxSmmFlagAddr);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: NvdimmArsStopArs
 *
 * Description
 *   Stop an in-progress ARS Scrub.
 *
 *  @retval EFI_SUCCESS or EFI_DEVICE_ERROR
 *
 */
/*---------------------------------------------------------------------------------------*/
EFI_STATUS
EFIAPI
NvdimmArsStopArs ()
{
  EFI_STATUS    Status = EFI_SUCCESS;

  Status = PspMboxBiosArs (ArsActionStop, NULL, mPspMboxSmmBuffer, mPspMboxSmmFlagAddr);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: NvdimmArsSpaIsPersistent
 *
 * Description
 *   Query whether SPA lies in a persistent memory region,
 *   and if so return TRUE, NVDIMM NFIT Handle, &
 *   EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE for SPA.
 *
 *
 *  @param[in]    Spa
 *  @param[out]   *Presistent
 *  @param[out]   *NvdimmNfitHandle
 *  @param[out]   *NfitSpaRangeStruc
 *
 *  @retval EFI_SUCCESS or EFI_NOT_FOUND
 *
 */
/*---------------------------------------------------------------------------------------*/

EFI_STATUS
EFIAPI
NvdimmArsSpaIsPersistent (
    IN    UINT64                                                      Spa,
    OUT   BOOLEAN                                                     *Presistent,
    OUT   UINT32                                                      *NvdimmNfitHandle,
    OUT   EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE   *NfitSpaRangeStruc
    )
{
  UINT16                    NumOfArsSpaStrucs;
  ARS_SPA_RANGE_STRUCT      *ArsSpaRangeSturcPtr;
  SPA_RANGE_STRUCT          *SpaRangeSturcPtr;
  UINT16                    i;
  EFI_STATUS                Status;

  // init variables
  *Presistent = FALSE;
  Status = EFI_SUCCESS;

  // ARS SPA Range Table not initialized
  if (mArsSpaRangeTables == NULL) {
    return EFI_NOT_FOUND;
  }

  NumOfArsSpaStrucs = *((UINT16*)mArsSpaRangeTables);
  ArsSpaRangeSturcPtr = (ARS_SPA_RANGE_STRUCT*)((UINT8*)mArsSpaRangeTables + sizeof(NumOfArsSpaStrucs));

  // search ARS SPA Range Table for SPA
  for (i=0; i < NumOfArsSpaStrucs; i++, ArsSpaRangeSturcPtr++) {
    SpaRangeSturcPtr = &ArsSpaRangeSturcPtr->SpaRangeStruc;
    if ((SpaRangeSturcPtr->SpaRangeBase <= Spa) &&
        (Spa <= (SpaRangeSturcPtr->SpaRangeBase + SpaRangeSturcPtr->SpaRangeLength)))
    {
      *Presistent = TRUE;
      CopyMem (NfitSpaRangeStruc, SpaRangeSturcPtr, sizeof (SPA_RANGE_STRUCT));
      *NvdimmNfitHandle = ArsSpaRangeSturcPtr->NvdimmNfitHandle;
      break;
    }
  }

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Name: NvdimmArsSpaIsMirroredInterleaveSet
 *
 * Description
 *   Query whether SPA lies in a mirrored interleave set in a persistent memory region.
 *
 *  @param[in]    Spa
 *
 *  @retval EFI_SUCCESS or EFI_UNSUPPORTED
 *
 */
/*---------------------------------------------------------------------------------------*/

EFI_STATUS
EFIAPI
NvdimmArsSpaIsMirroredInterleaveSet (
    IN    UINT64      Spa
    )
{
  EFI_STATUS  Status = EFI_UNSUPPORTED;
  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * AMD ARS SMM driver main entry point
 *
 *
 * @param[in]       ImageHandle           Image Handle
 * @param[in]       SystemTable           Pointer to system globals
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
ArsNvdimmSmmInit ()
{
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL *PspMboxSmmBufferAddressProtocol;
  EFI_SMM_BASE2_PROTOCOL              *SmmBase2;
  EFI_HANDLE                          Handle;
  EFI_STATUS                          Status;
  CPUID_DATA                          CpuId;

  DEBUG ((EFI_D_ERROR, "ArsSmmEntryPoint Entry\n"));

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**)&SmmBase2);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
      return Status;
  }

  Status = SmmBase2->GetSmstLocation (SmmBase2, &gSmst);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
  }

  // verify that CLZERO is supported
  // CPUID_Fn80000008_EBX [Extended Feature Extensions ID EBX] CLZERO:[0]
  AsmCpuid (0x80000008, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
  if ((CpuId.EBX_Reg & 0x1)) {
    // save cache line length
    // Fn0000_0001_EBX[CLFlush]
    AsmCpuid (0x00000001, &(CpuId.EAX_Reg), &(CpuId.EBX_Reg), &(CpuId.ECX_Reg), &(CpuId.EDX_Reg));
    // CLFlush:[15:8] - size in quadwords
    mCacheLineLength = 8 * ((CpuId.EBX_Reg >> 8) & 0xFF);
  }
  ASSERT(mCacheLineLength);
  if (mCacheLineLength == 0) {
    return EFI_UNSUPPORTED;
  }

  // Save PspMboxSmmBuffer to globol variable
  Status = gSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
  if (Status == EFI_SUCCESS) {
    mPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
    mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
    DEBUG ((EFI_D_ERROR, "\tmPspMboxSmmBuffer 0x%x\n", mPspMboxSmmBuffer));
    DEBUG ((EFI_D_ERROR, "\tmPspMboxSmmFlagAddr 0x%x\n", mPspMboxSmmFlagAddr));
  } else {
    DEBUG ((EFI_D_ERROR, "\tLocate gPspMboxSmmBufferAddressProtocolGuid: %r - return to caller\n", Status));
    return Status;
  }

  // init ARS SPA range Table
  Status = InitSmmNvdimmSpaRangeTable ();
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // NFIT table not found
  if (mArsSpaRangeTables == NULL) {
    DEBUG ((EFI_D_ERROR, "\tInitSmmNvdimmSpaRangeTable() didn't create ARS SPA range Table: %r - return to caller\n", Status));
    return Status;
  }

  //
  // install SMM protocol
  //
  Handle =  NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gAmdJedecNvdimmArsSmmProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &AmdNvdimmArsSmmProtocol
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((EFI_D_ERROR, "ArsSmmEntryPoint Exit\n"));
  return Status;
}
