/*
 ******************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include "Porting.h"
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Arch.h>
#include <Addendum/Apcb/Inc/CommonV3/ApcbV3Priority.h>
#include <Library/ApcbChecksumLibV3.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ApcbVariableLibV3.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ApcbLibV3.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/PspMboxSmmBufferAddressProtocol.h>
#include <Protocol/SmmReadyToLock.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_APCBHMACCHECKSUMLIBV3_APCBHMACCHECKSUMLIBV3_FILECODE

#define APCB_SIGNATURE  0x42435041ul

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

#define CHECKSUM_METHOD_STANDARD 0
#define CHECKSUM_METHOD_EXTENDED 1

typedef struct {
  UINT8                   BiosDirEntry;
  UINT8                   BinaryInstance;
  UINT8                   ChecksumMethod;
} APCB_BINARY_INSTANCE_CHECK_METHOD;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          G L O B A L        V A L U E S
 *----------------------------------------------------------------------------------------
 */


CONST APCB_BINARY_INSTANCE_CHECK_METHOD ApcbBinaryInstanceMethod[] = {
  { BIOS_APCB_INFO_BACKUP, APCB_BINARY_INSTANCE_DEFAULT,       CHECKSUM_METHOD_STANDARD },
  { BIOS_APCB_INFO,        APCB_BINARY_INSTANCE_BIOS_CONFIG,   CHECKSUM_METHOD_EXTENDED },
  { BIOS_APCB_INFO,        APCB_BINARY_INSTANCE_EVENT_LOGGING, CHECKSUM_METHOD_EXTENDED },
};

STATIC BOOLEAN                 mInSmm                         = FALSE;
STATIC EFI_SMM_SYSTEM_TABLE2   *mSmst                         = NULL;
STATIC UINT8                   *mPspMboxSmmBuffer             = NULL;
STATIC BOOLEAN                 *mPspMboxSmmFlagAddr           = NULL;
STATIC UINT8                   *mSmmApcbShadowBuffer          = NULL;

/**
  Dump binary buffer

  @param[in]      Buffer          Point to data to dump
  @param[in]      Size            Size of buffer in byte

**/
STATIC
VOID
DumpBuffer (
  IN       VOID    *Buffer,
  IN       UINT32   Size
  )
{
  UINT32   i;
  UINT8   *BufPtr;

  IDS_HDT_CONSOLE_PSP_TRACE ("Buffer: @ 0x%x, Size: 0x%x\n", Buffer, Size);
  BufPtr = (UINT8*) Buffer;

  IDS_HDT_CONSOLE_PSP_TRACE ("00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15\n");

  for (i = 0; i < Size; i++, BufPtr++) {
    IDS_HDT_CONSOLE_PSP_TRACE ("%02x ", *(BufPtr));
    if ((i != 0) && ((i + 1) % 16 == 0)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\n");
    }
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\n");
}

/**
  Calculate APCB checksum with standard algorithm

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      CheckSum        Point to checksum buffer

**/
STATIC
VOID
ApcbCalcCheckSumStandard (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN OUT   UINT8            *CheckSum
  )
{
  UINT8  CheckSumValue;
  UINT8  *DataPtr;

  ASSERT (NULL != ApcbPtr);

  CheckSumValue = 0;
  DataPtr = ApcbPtr;
  while (Length --) {
    CheckSumValue += *(DataPtr ++);
  }
  *CheckSum = CheckSumValue;
}


/**
  Calculate APCB checksum with HMAC algorithm

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index
  @param[in]      CheckSum        Point to checksum buffer
  @param[out]     CheckSumLength  Point to checksum size buffer

**/
STATIC
VOID
ApcbCalcCheckSumExtended (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance,
  IN OUT   UINT8            *CheckSum,
  OUT      UINT32           *CheckSumLength
  )
{
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL      *PspMboxSmmBufferAddressProtocol;
  EFI_STATUS                               Status;
  HMAC_PROTECTED_DATA                      HmacProtectedData;
  BOOLEAN                                  ExchangeShadowBuffer;
  BOOLEAN                                  UseSmmFunction;
  VOID                                     *DummyProtocol;
  APCB_VARIABLE_STRUCT                     *pApcbVariableStruct;

  IDS_HDT_CONSOLE_PSP_TRACE ("ApcbCalcCheckSumExtended:\n");
  ASSERT ((UINTN)CheckSum == ((UINTN)(ApcbPtr) + Length));

  *CheckSumLength = HMAC_CHECKSUM_LENGTH;
  HmacProtectedData.BufferddrLo = (UINT32)(((UINT64)(UINTN)ApcbPtr) & 0xFFFFFFFF) ;
  HmacProtectedData.BufferAddrHi = (UINT32)(((UINT64)(UINTN)ApcbPtr) >> 32);
  HmacProtectedData.BufferSize = Length + HMAC_CHECKSUM_LENGTH;
  HmacProtectedData.DataOffset = 0;
  HmacProtectedData.DataSize = Length;
  HmacProtectedData.SignatureOffset = Length;
  HmacProtectedData.SignatureSize = HMAC_CHECKSUM_LENGTH;
  HmacProtectedData.Validate = 0;

  UseSmmFunction = FALSE;
  if (mInSmm) {
    Status = mSmst->SmmLocateProtocol (&gEfiSmmReadyToLockProtocolGuid, NULL, (VOID **)&DummyProtocol);
    if (!EFI_ERROR(Status)) {
      UseSmmFunction = TRUE;
    }
  }

  if (UseSmmFunction) {
    mSmmApcbShadowBuffer = NULL;
    if ((BiosDirEntry == BIOS_APCB_INFO) && (BinaryInstance == APCB_BINARY_INSTANCE_BIOS_CONFIG)) {
      pApcbVariableStruct = GetApcbVariableStruct ();
      if (pApcbVariableStruct == NULL) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[%a] pApcbVariableStruct is NULL\n", __FUNCTION__);
        ASSERT (FALSE);
      }
      mSmmApcbShadowBuffer = (UINT8 *)(UINTN) pApcbVariableStruct->Apcb60ConfigRunTimeShadowAddress;
      IDS_HDT_CONSOLE_PSP_TRACE ("Config mSmmApcbShadowBuffer: 0x%08X\n", mSmmApcbShadowBuffer);
      ASSERT (mSmmApcbShadowBuffer != NULL);
    } else if ((BiosDirEntry == BIOS_APCB_INFO) && (BinaryInstance == APCB_BINARY_INSTANCE_EVENT_LOGGING)) {
      pApcbVariableStruct = GetApcbVariableStruct ();
      if (pApcbVariableStruct == NULL) {
        IDS_HDT_CONSOLE_PSP_TRACE ("[%a] pApcbVariableStruct is NULL\n", __FUNCTION__);
        ASSERT (FALSE);
      }
      mSmmApcbShadowBuffer = (UINT8 *)(UINTN) pApcbVariableStruct->Apcb60EvtLgRunTimeShadowAddress;
      IDS_HDT_CONSOLE_PSP_TRACE ("Event log mSmmApcbShadowBuffer: 0x%08X\n", mSmmApcbShadowBuffer);
      ASSERT (mSmmApcbShadowBuffer != NULL);
    }
    ASSERT (mSmmApcbShadowBuffer != NULL);

    if ((mPspMboxSmmBuffer == NULL) || (mPspMboxSmmFlagAddr == NULL)) {
      Status = mSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
      ASSERT (!EFI_ERROR(Status));

      if (!EFI_ERROR(Status)) {
        mPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
        mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
        ASSERT (mPspMboxSmmBuffer != NULL);
        ASSERT (mPspMboxSmmFlagAddr != NULL);
      }
    }

    if ((mPspMboxSmmBuffer == NULL) || (mPspMboxSmmFlagAddr == NULL) || (mSmmApcbShadowBuffer == NULL)) {
      //
      // Not possible to run calculate function
      //
      SetMem ((UINT8 *)ApcbPtr + Length, HMAC_CHECKSUM_LENGTH, 0xFF);
      return;
    }

    if (ApcbPtr == mSmmApcbShadowBuffer) {
      ExchangeShadowBuffer = FALSE;
    } else {
      ExchangeShadowBuffer = TRUE;
      CopyMem (mSmmApcbShadowBuffer, ApcbPtr, Length);
      HmacProtectedData.BufferddrLo = (UINT32)(((UINT64)(UINTN)mSmmApcbShadowBuffer) & 0xFFFFFFFF);
      HmacProtectedData.BufferAddrHi = (UINT32)(((UINT64)(UINTN)mSmmApcbShadowBuffer) >> 32);
    }

    Status = PspMboxBiosCmdSignValidateHmacDataSmm (&HmacProtectedData, mPspMboxSmmBuffer, mPspMboxSmmFlagAddr);
    ASSERT (!EFI_ERROR(Status));

    if (EFI_ERROR(Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PspMboxBiosCmdSignValidateHmacDataSmm failed: %r\n", Status);
      return;
    }

    IDS_HDT_CONSOLE_PSP_TRACE ("HMAC checksum(SMM):\n");
    if (ExchangeShadowBuffer) {
      CopyMem (CheckSum, mSmmApcbShadowBuffer + Length, HMAC_CHECKSUM_LENGTH);
      ZeroMem (mSmmApcbShadowBuffer, HmacProtectedData.BufferSize);
    }
    DumpBuffer (CheckSum, HMAC_CHECKSUM_LENGTH);
    return;
  }

  Status = PspMboxBiosCmdSignValidateHmacDataPreSmm (&HmacProtectedData);
  ASSERT (!EFI_ERROR(Status));

  if (EFI_ERROR(Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("PspMboxBiosCmdSignValidateHmacDataPreSmm failed: %r\n", Status);
    return;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("HMAC checksum:\n");
  DumpBuffer (CheckSum, HMAC_CHECKSUM_LENGTH);
}

/**
  Get APCB checksum algorithm

  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index

  @retval CHECKSUM_METHOD_STANDARD   Use byte sum algorithm
  @retval CHECKSUM_METHOD_EXTENDED   Use HMAC algorithm

**/
STATIC
UINT8
GetCheckSumMethod (
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  UINTN   Index;

  if (!FeaturePcdGet(PcdApcbUseHmacChecksum)) {
    return CHECKSUM_METHOD_STANDARD;
  }
  for (Index = 0; Index < sizeof(ApcbBinaryInstanceMethod) / sizeof(APCB_BINARY_INSTANCE_CHECK_METHOD); Index ++) {
    if ((ApcbBinaryInstanceMethod[Index].BiosDirEntry == BiosDirEntry) &&
        (ApcbBinaryInstanceMethod[Index].BinaryInstance == BinaryInstance)) {
      return ApcbBinaryInstanceMethod[Index].ChecksumMethod;
    }
  }
  return CHECKSUM_METHOD_EXTENDED;
}

/**
  Check APCB checksum

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index

  @retval TRUE                   APCB checksum is valid
  @retval FALSE                  APCB checksum is not valid

**/
BOOLEAN
ApcbValidateCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  UINT8                                    CheckSumMethod;
  APCB_V3_HEADER                           *Header;
  UINT8                                    StandardCheckSumData;
  PSP_MBOX_SMMBUFFER_ADDRESS_PROTOCOL      *PspMboxSmmBufferAddressProtocol;
  EFI_STATUS                               Status;
  HMAC_PROTECTED_DATA                      HmacProtectedData;
  BOOLEAN                                  UseSmmFunction;
  BOOLEAN                                  ExchangeShadowBuffer;
  BOOLEAN                                  HmacCheckSumAvailable;
  UINT8                                    *HmacCheckSumPtr;
  UINTN                                    Count;
  VOID                                     *DummyProtocol;
  APCB_VARIABLE_STRUCT                     *pApcbVariableStruct;

  Header = (APCB_V3_HEADER *)ApcbPtr;
  if ((Header == NULL) || (Header->Signature != APCB_SIGNATURE)) {
    return FALSE;
  }

  CheckSumMethod = GetCheckSumMethod(BiosDirEntry, BinaryInstance);
  if (CheckSumMethod == CHECKSUM_METHOD_EXTENDED) {
    IDS_HDT_CONSOLE_PSP_TRACE ("ApcbValidateCheckSum extended\n");

    UseSmmFunction = FALSE;
    ExchangeShadowBuffer = FALSE;
    HmacProtectedData.BufferddrLo = (UINT32)(((UINT64)(UINTN)Header) & 0xFFFFFFFF);
    HmacProtectedData.BufferAddrHi = (UINT32)(((UINT64)(UINTN)Header) >> 32);
    HmacProtectedData.BufferSize = Length + HMAC_CHECKSUM_LENGTH;
    HmacProtectedData.DataOffset = 0;
    HmacProtectedData.DataSize = Length;
    HmacProtectedData.SignatureOffset = Length;
    HmacProtectedData.SignatureSize = HMAC_CHECKSUM_LENGTH;
    HmacProtectedData.Validate = 1;

    IDS_HDT_CONSOLE_PSP_TRACE ("ApcbValidateCheckSum extended Dump HMAC\n");
    HmacCheckSumPtr = ((UINT8 *)Header) + Length;
    DumpBuffer(HmacCheckSumPtr, HMAC_CHECKSUM_LENGTH);

    //
    // Check if extended check is available
    //
    HmacCheckSumAvailable = FALSE;
    for (Count = 1; Count < HMAC_CHECKSUM_LENGTH; Count ++) {
      if (HmacCheckSumPtr[0] != HmacCheckSumPtr[Count]) {
        HmacCheckSumAvailable = TRUE;
        break;
      }
    }
    IDS_HDT_CONSOLE_PSP_TRACE ("HMAC checksum is %a.\n", HmacCheckSumAvailable ? "available" : "not available");

    if (HmacCheckSumAvailable) {
      if (mInSmm) {
        Status = mSmst->SmmLocateProtocol (&gEfiSmmReadyToLockProtocolGuid, NULL, (VOID **)&DummyProtocol);
        if (!EFI_ERROR(Status)) {
          UseSmmFunction = TRUE;
        }
      }

      if (UseSmmFunction) {
        if ((mPspMboxSmmBuffer == NULL) || (mPspMboxSmmFlagAddr == NULL)) {
          Status = mSmst->SmmLocateProtocol (&gPspMboxSmmBufferAddressProtocolGuid, NULL, &PspMboxSmmBufferAddressProtocol);
          ASSERT (!EFI_ERROR(Status));

          mPspMboxSmmBuffer = PspMboxSmmBufferAddressProtocol->PspMboxSmmBuffer;
          mPspMboxSmmFlagAddr = PspMboxSmmBufferAddressProtocol->PspMboxSmmFlagAddr;
        }

        if ((BiosDirEntry == BIOS_APCB_INFO) && (BinaryInstance == APCB_BINARY_INSTANCE_BIOS_CONFIG)) {
          pApcbVariableStruct = GetApcbVariableStruct ();
          if (pApcbVariableStruct == NULL) {
            IDS_HDT_CONSOLE_PSP_TRACE ("[%a] pApcbVariableStruct is NULL\n", __FUNCTION__);
            ASSERT (FALSE);
          }
          mSmmApcbShadowBuffer = (UINT8 *)(UINTN) pApcbVariableStruct->Apcb60ConfigRunTimeShadowAddress;
          IDS_HDT_CONSOLE_PSP_TRACE ("Config mSmmApcbShadowBuffer: 0x%08X\n", mSmmApcbShadowBuffer);
          ASSERT (mSmmApcbShadowBuffer != NULL);
        } else if ((BiosDirEntry == BIOS_APCB_INFO) && (BinaryInstance == APCB_BINARY_INSTANCE_EVENT_LOGGING)) {
          pApcbVariableStruct = GetApcbVariableStruct ();
          if (pApcbVariableStruct == NULL) {
            IDS_HDT_CONSOLE_PSP_TRACE ("[%a] pApcbVariableStruct is NULL\n", __FUNCTION__);
            ASSERT (FALSE);
          }
          mSmmApcbShadowBuffer = (UINT8 *)(UINTN) pApcbVariableStruct->Apcb60EvtLgRunTimeShadowAddress;
          IDS_HDT_CONSOLE_PSP_TRACE ("Event log mSmmApcbShadowBuffer: 0x%08X\n", mSmmApcbShadowBuffer);
          ASSERT (mSmmApcbShadowBuffer != NULL);
        }
        if (EFI_ERROR(Status)) {
          mSmmApcbShadowBuffer = NULL;
        }

        if ((mSmmApcbShadowBuffer == NULL) || (mPspMboxSmmBuffer == NULL) || (mPspMboxSmmFlagAddr == NULL)) {
          return FALSE;
        }

        ASSERT (mSmmApcbShadowBuffer != NULL);
        if (ApcbPtr != mSmmApcbShadowBuffer) {
          //
          // Before SmmLock(To be change, It is ReadyToBoot), ApcbLibV3 SMM still uses post time driver which is not in TBase. It is not allowed by the PSP. So, borrow shadow for buffering.
          //
          ExchangeShadowBuffer = TRUE;
          CopyMem (mSmmApcbShadowBuffer, ApcbPtr, Length + HMAC_CHECKSUM_LENGTH);
          HmacProtectedData.BufferddrLo = (UINT32)(((UINT64)(UINTN)mSmmApcbShadowBuffer) & 0xFFFFFFFF);
          HmacProtectedData.BufferAddrHi = (UINT32)(((UINT64)(UINTN)mSmmApcbShadowBuffer) >> 32);
        }

        Status = PspMboxBiosCmdSignValidateHmacDataSmm (&HmacProtectedData, mPspMboxSmmBuffer, mPspMboxSmmFlagAddr);
        IDS_HDT_CONSOLE_PSP_TRACE ("Validate HMAC source Smm: %r\n", Status);

        if (ExchangeShadowBuffer) {
          ZeroMem (mSmmApcbShadowBuffer, HmacProtectedData.BufferSize);
        }
        if (EFI_ERROR(Status)) {
          return FALSE;
        }
        return TRUE;
      }

      Status = PspMboxBiosCmdSignValidateHmacDataPreSmm (&HmacProtectedData);
      IDS_HDT_CONSOLE_PSP_TRACE ("Validate HMAC source PreSmm: %r\n", Status);
      if (EFI_ERROR(Status)) {
        return FALSE;
      }
    }
  }

  ApcbCalcCheckSumStandard (
            Header,
            Length,
            &StandardCheckSumData
            );
  if (StandardCheckSumData != 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Check APCB checksum availability

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index

  @retval TRUE                   There is APCB checksum
  @retval FALSE                  There is no APCB checksum

**/
BOOLEAN
IsApcbCheckSumAvailable (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  UINTN Index;
  UINT8 CheckSumMethod;
  UINT8 CheckSumData;
  UINT8 *CheckSumPointer;

  CheckSumMethod = GetCheckSumMethod(BiosDirEntry, BinaryInstance);
  if (CheckSumMethod == CHECKSUM_METHOD_EXTENDED) {
    CheckSumPointer = ((UINT8 *)ApcbPtr) + Length;
    CheckSumData = *CheckSumPointer;
    for (Index = 1; Index < HMAC_CHECKSUM_LENGTH; Index ++) {
      if (CheckSumPointer[Index] != CheckSumData) {
        return TRUE;
      }
    }
    return FALSE;
  }

  return TRUE;
}

/**
  Update APCB checksum

  @param[in,out]  ApcbPtr         Point to APCB data
  @param[in       Length          Length of APCB data
  @param[in]      BiosDirEntry    Bios Dir entry index
  @param[in]      BinaryInstance  Binary instance index

  @retval TRUE                   Updated successfully
  @retval FALSE                  APCB header is not invalid

**/
BOOLEAN
ApcbUpdateCheckSum (
  IN       VOID             *ApcbPtr,
  IN       UINT32           Length,
  IN       UINT8            BiosDirEntry,
  IN       UINT8            BinaryInstance
)
{
  UINT8              CheckSumMethod;
  UINT32             CheckSumLength;
  APCB_V3_HEADER     *Header;
  UINT8              StandardCheckSumData;

  Header = (APCB_V3_HEADER *)ApcbPtr;
  if ((Header == NULL) || (Header->Signature != APCB_SIGNATURE)) {
    return FALSE;
  }

  Header->CheckSumByte = 0;
  ApcbCalcCheckSumStandard (
            Header,
            Length,
            &StandardCheckSumData
            );
  Header->CheckSumByte = (~StandardCheckSumData) + 1;

  CheckSumMethod = GetCheckSumMethod(BiosDirEntry, BinaryInstance);
  if (CheckSumMethod == CHECKSUM_METHOD_EXTENDED) {
    ApcbCalcCheckSumExtended (
              Header,
              Length,
              BiosDirEntry,
              BinaryInstance,
              ((UINT8 *)Header) + Length,
              &CheckSumLength
              );
  }

  return TRUE;
}


/**

  ApcbHmacChecksumLibConstructor, check if we are in SMM

**/
EFI_STATUS
EFIAPI
ApcbHmacChecksumLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_SMM_BASE2_PROTOCOL    *SmmBase2;
  EFI_STATUS                Status;

  if (FeaturePcdGet(PcdApcbUseHmacChecksum)) {
    Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**) &SmmBase2);
    if (!EFI_ERROR (Status)) {
      SmmBase2->InSmm (SmmBase2, &mInSmm);
    }

    if (mInSmm) {
      //
      // We are now in SMM
      // get SMM table base
      //
      Status = SmmBase2->GetSmstLocation (SmmBase2, &mSmst);
      ASSERT (!EFI_ERROR(Status));
    }
  }

  return EFI_SUCCESS;
}
