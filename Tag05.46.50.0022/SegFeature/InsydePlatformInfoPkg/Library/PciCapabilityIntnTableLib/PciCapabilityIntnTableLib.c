/** @file
  PCI Capability Lookup table

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/BaseMemoryLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PciCapabilityIntnTableLib.h>

#include <IndustryStandard/Pci.h>
#include <IndustryStandard/PciExpress30.h>
#include <IndustryStandard/PciExpress40.h>
#include <IndustryStandard/PciCapability.h>

#include <Protocol/H2OPciDeviceInfo.h>

#define DEFAULT_STRING_BUFFER_SIZE    0x1000
#define DEFAULT_STRING_ID_BUFFER_SIZE 0x400

#define DEFAULT_INTPN_STRING        L"  No interpretation available."

STATIC EFI_GUID                         mPciCapabilityIntnTableStringPackGuid = PCI_CAPABILITY_INTN_TABLE_STRING_PACKAGE_GUID;
STATIC EFI_HII_HANDLE                   mCapabilityIntnStringPackHandle = NULL;
extern UINT8                            PciCapabilityIntnTableLibStrings[];

extern COMMON_CAP_INTERPRETATIONTABLE   PciCapabilityCommonInteLookUp[];
extern COMMON_CAP_INTERPRETATIONTABLE   PcieExtendedCapablityCommonInteLookUp[];
extern DYNAMIC_CAP_INTERPRETATIONTABLE  PciCapabilityDynamicInteLookUp[];
extern DYNAMIC_CAP_INTERPRETATIONTABLE  PcieExtCapabilityDynamicInteLookUp[];

STATIC EFI_HII_HANDLE           mVfrFormHiiHandle;
STATIC PCI_DEVICE_INFO          *mCurrentPciDeviceInfo;

STATIC EFI_PCI_CAPABILITY_HDR   *mPciCapHdr       = NULL;
STATIC PCI_EXP_EXT_HDR          *mPcieExtCapHdr   = NULL;
STATIC PCI_CAPABILITY_PCIEXP    *mPciExpCap       = NULL;
STATIC CHAR8                    *mLanguage        = NULL;
STATIC VOID                     *mStartOpCodeHandle = NULL;
STATIC CHAR16                   mStrBuffer[DEFAULT_STRING_BUFFER_SIZE] = {0};

STATIC EFI_STRING_ID            mPciCapabilityIntpnPromtStringIds[DEFAULT_STRING_ID_BUFFER_SIZE] = {0};
STATIC EFI_STRING_ID            mPciCapabilityIntpnHelpStringIds[DEFAULT_STRING_ID_BUFFER_SIZE]  = {0};

/**
  Add Hii String Package to Hii Handle

  @retval EFI_SUCCESS             - PCI Base String Init Success.
  @retval EFI_OUT_OF_RESOURCES    - Hii Add String Package failed
**/
EFI_STATUS
LibPciCapabilityIntnStringInit (
  VOID
) {
  mCapabilityIntnStringPackHandle = HiiAddPackages (
                                    &mPciCapabilityIntnTableStringPackGuid,
                                    NULL,
                                    PciCapabilityIntnTableLibStrings,
                                    NULL
                                    );
  if (mCapabilityIntnStringPackHandle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**
  Read Data with Specific data length

  @param  DataSize          - DataLength
  @param  DataAddress       - DataAddress

  @retval UINTN             - MemoryData.
**/
UINTN
ReadCapDataValue (
  UINTN    DataSize,
  VOID     *DataAddress
 ) {
  UINTN Value;
  Value = 0;
  switch (DataSize) {
    case sizeof(UINT8):
      Value = *((UINT8 *)DataAddress);
      break;
    case sizeof(UINT16):
      Value = *((UINT16 *)DataAddress);
      break;
    case sizeof(UINT32):
      Value = *((UINT32 *)DataAddress);
      break;
    case sizeof(UINT64):
      Value = *((UINT64 *)DataAddress);
      break;
    default:
    break;
  }
  return Value;
}

/**
  Interpretation MSI Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPciCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPciCapMSI (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                            Status;
  CHAR16                                *FormatString;
  CHAR16                                *HelpString;
  EFI_PCI_CAPABILITY_MSI_COMMON_HEADER  *MSI_COMMON;
  CAP_LOOKUP_TABLE                      *TableSel;
  UINTN                                 Index;
  UINT8                                 *OpCodeRet;
  UINTN                                 TableSize;
  UINTN                                 TableType;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPciCapHdr == NULL || mPciCapHdr->CapabilityID != EFI_PCI_CAPABILITY_ID_MSI) {
    return EFI_INVALID_PARAMETER;
  }

  MSI_COMMON = (EFI_PCI_CAPABILITY_MSI_COMMON_HEADER *) mPciCapHdr;
  if (MSI_COMMON->MsgCtrlReg.Bits.Addr64BitCap == 1) {
    TableType = PCI_CAP_MSI_TYPE64;
  } else {
    TableType = PCI_CAP_MSI_TYPE32;
  }

  //
  // Search Lookup Table
  //
  for (Index = 0; PciCapabilityDynamicInteLookUp[Index].CapabilityId != 0x00; Index++) {
    if (mPciCapHdr->CapabilityID == PciCapabilityDynamicInteLookUp[Index].CapabilityId) {
      if (TableType == PciCapabilityDynamicInteLookUp[Index].TableType) {
        TableSize = PciCapabilityDynamicInteLookUp[Index].TableSize;
        TableSel = PciCapabilityDynamicInteLookUp[Index].CapCommonTable;
        break;
      }
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  //
  // Dynamic Area Pending Bits & Mask Bits
  //
  if (!(MSI_COMMON->MsgCtrlReg.Bits.PVMCap == TRUE)) {
    TableSize -= 2;
    if (!(MSI_COMMON->MsgCtrlReg.Bits.ExtDataEn == TRUE)) {
      TableSize -= 1;
    }
  }

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPciCapHdr) + (TableSel[Index].ValueOffset)))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Enhanced Allocation Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPciCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPciCapEA (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                              Status;
  CHAR16                                  *FormatString;
  CHAR16                                  *HelpString;
  H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION  *EA_COMMON;
  CAP_LOOKUP_TABLE                        *TableSel;
  UINTN                                   Index;
  UINT8                                   *OpCodeRet;
  UINTN                                   TableSize;
  UINTN                                   TableType;
  UINTN                                   DynamicOffsetValue;
  UINTN                                   UpperBase;
  UINTN                                   UpperMaxOffset;
  UINTN                                   Base;
  UINTN                                   MaxOffset;
  BOOLEAN                                 Is64Base;
  BOOLEAN                                 Is64MaxOffset;

  Status = EFI_SUCCESS;
  TableType = 0;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPciCapHdr == NULL || mPciCapHdr->CapabilityID != H2O_PCI_CAPABILITY_ID_ENHANCED_ALLOCATION) {
    return EFI_INVALID_PARAMETER;
  }

  EA_COMMON = (H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION *)mPciCapHdr;

  //
  // Search Lookup Table
  //
  for (Index = 0; PciCapabilityDynamicInteLookUp[Index].CapabilityId != 0x00; Index++) {
    if (mPciCapHdr->CapabilityID == PciCapabilityDynamicInteLookUp[Index].CapabilityId) {
      if (TableType == PciCapabilityDynamicInteLookUp[Index].TableType) {
        TableSize = PciCapabilityDynamicInteLookUp[Index].TableSize;
        TableSel = PciCapabilityDynamicInteLookUp[Index].CapCommonTable;
        break;
      }
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  //
  // Capability EA Common
  //
  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPciCapHdr) + (TableSel[Index].ValueOffset)))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // SecDW only for PCI Header Type 1
  //
  if (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE) {
    DynamicOffsetValue = ((offsetof(H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION, DynamicArea)));
    if (mCurrentPciDeviceInfo->IsBridge) {
      FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_SECDW_STRING_FORMAT), mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        FormatString,
        ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPciCapHdr) + DynamicOffsetValue))
      );

      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                              );

      HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_SECDW_STRING_HELP), mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        HelpString,
        DynamicOffsetValue
        );
      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  mLanguage
                                                                );

      OpCodeRet = HiiCreateActionOpCode (
                    mStartOpCodeHandle,
                    (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                    0,
                    0
                    );
      if (OpCodeRet == NULL) {
        DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
      }
      (*InterpretationIndex)++;
      if (FormatString != NULL) {
        FreePool (FormatString);
      }
      if (HelpString != NULL) {
        FreePool (HelpString);
      }
      DynamicOffsetValue += sizeof(UINT32);
    }
  }

  //
  // Dynamic Area Enhaced Allocation Entries
  //

  //
  // TBD Entry Intn Table
  //
  for (Index = 0; (Index < EA_COMMON->NumEntries) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    Base = 0;
    UpperBase = 0;
    Is64Base = FALSE;
    MaxOffset = 0;
    UpperMaxOffset = 0;
    Is64MaxOffset = FALSE;
    //
    // Entry Header
    //
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_ENTRY_HEADER_FORMAT), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPciCapHdr) + DynamicOffsetValue))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                              );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_ENTRY_HEADER_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      DynamicOffsetValue
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                              );
    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;
    DynamicOffsetValue += sizeof(UINT32);
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
    if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
      break;
    }

    //
    // Base
    //
    Base = ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPciCapHdr) + DynamicOffsetValue));
    if (H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION_ENTRIES_64BITLENGTH(Base)) {
      Is64Base = TRUE;
      UpperBase = ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPciCapHdr) + DynamicOffsetValue + sizeof(UINT32)*2));
      Base = Base|(UpperBase<<32);
    }
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_ENTRY_BASE_FORMAT), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      (Is64Base?16:8),
      Base
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_ENTRY_BASE_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      DynamicOffsetValue
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );
    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;
    DynamicOffsetValue += sizeof(UINT32);
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
    if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
      break;
    }
    //
    // Max Offset
    //
    MaxOffset = ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPciCapHdr) + DynamicOffsetValue));
    if (H2O_PCI_CAPABILITY_ENHANCED_ALLOCATION_ENTRIES_64BITLENGTH(MaxOffset)) {
      Is64MaxOffset = TRUE;
      UpperMaxOffset = ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPciCapHdr) + DynamicOffsetValue + sizeof(UINT32)*2));
      MaxOffset = (MaxOffset|(UpperBase<<32));
    }
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_ENTRY_MAX_FORMAT), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      (Is64MaxOffset?16:8),
      MaxOffset
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_ENHANCED_ALLOCATION_ENTRY_MAX_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      DynamicOffsetValue
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );
    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;
    DynamicOffsetValue += sizeof(UINT32);
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
    if (Is64Base) {
      DynamicOffsetValue += sizeof(UINT32);
    }
    if (Is64MaxOffset) {
      DynamicOffsetValue += sizeof(UINT32);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Vendor Specific Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPciCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPciCapVS (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                     Status;
  CHAR16                         *FormatString;
  CHAR16                         *HelpString;
  H2O_PCI_CAPABILITY_VS          *VendorSpecific;
  CAP_LOOKUP_TABLE               *TableSel;
  UINTN                          Index;
  UINTN                          TableSize;
  UINTN                          VsLength;
  UINT8                          *OpCodeRet;
  UINTN                          BaseAddress;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPciCapHdr == NULL || mPciCapHdr->CapabilityID != EFI_PCI_CAPABILITY_ID_VENDOR) {
    return EFI_INVALID_PARAMETER;
  }

  VendorSpecific = (H2O_PCI_CAPABILITY_VS *) mPciCapHdr;

  //
  // Search Lookup Table
  //
  for (Index = 0; PciCapabilityDynamicInteLookUp[Index].CapabilityId != 0x00; Index++) {
    if (mPciCapHdr->CapabilityID == PciCapabilityDynamicInteLookUp[Index].CapabilityId) {
      TableSize = PciCapabilityDynamicInteLookUp[Index].TableSize;
      TableSel = PciCapabilityDynamicInteLookUp[Index].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPciCapHdr) + (TableSel[Index].ValueOffset)))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                              );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  VsLength = (VendorSpecific->Length) - 3; // Length field value include hdr and Length field
  BaseAddress = offsetof(H2O_PCI_CAPABILITY_VS, VsData[0]);
  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_VS_DATA_STRING_FORMAT), mLanguage);
  HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_CAP_VS_DATA_STRING_HELP), mLanguage);

  for (Index = 0; (Index < VsLength) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT8), (((UINT8*)mPciCapHdr) + (BaseAddress)))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      BaseAddress
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    BaseAddress += sizeof (UINT32);
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation PCI-X Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPciCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPciCapPCIX (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS        Status;
  CHAR16            *FormatString;
  CHAR16            *HelpString;
  CAP_LOOKUP_TABLE  *TableSel;
  UINTN             Index;
  UINT8             *OpCodeRet;
  UINTN             TableSize;
  UINTN             TableType;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPciCapHdr == NULL || mPciCapHdr->CapabilityID != EFI_PCI_CAPABILITY_ID_PCIX) {
    return EFI_INVALID_PARAMETER;
  }

  if (mCurrentPciDeviceInfo->IsBridge == TRUE) {
    TableType = PCI_CAP_PCIX_TYPE_BRIDGE;
  } else {
    TableType = PCI_CAP_PCIX_TYPE_DEVICE;
  }

  //
  // Search Lookup Table
  //
  for (Index = 0; PciCapabilityDynamicInteLookUp[Index].CapabilityId != 0x00; Index++) {
    if (mPciCapHdr->CapabilityID == PciCapabilityDynamicInteLookUp[Index].CapabilityId) {
      if (TableType == PciCapabilityDynamicInteLookUp[Index].TableType) {
        TableSize = PciCapabilityDynamicInteLookUp[Index].TableSize;
        TableSel = PciCapabilityDynamicInteLookUp[Index].CapCommonTable;
        break;
      }
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPciCapHdr) + (TableSel[Index].ValueOffset)))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Common Interpretation PCI Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPciCapCommon (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS        Status;
  CHAR16            *FormatString;
  CHAR16            *HelpString;
  UINTN             Index;
  UINT8             *OpCodeRet;
  CAP_LOOKUP_TABLE  *TableSel;
  UINTN             TableSize;

  //
  // Search Lookup Table
  //
  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  for (Index = 0; PciCapabilityCommonInteLookUp[Index].CapabilityId != 0x00; Index++) {
    if (mPciCapHdr->CapabilityID == PciCapabilityCommonInteLookUp[Index].CapabilityId) {
      TableSize = PciCapabilityCommonInteLookUp[Index].TableSize;
      TableSel = PciCapabilityCommonInteLookUp[Index].LookupTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  //
  // Capability Interpretation
  //
  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPciCapHdr)) + (TableSel[Index].ValueOffset))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Common Interpretation Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtCapCommon (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS              Status;
  CHAR16                  *FormatString;
  CHAR16                  *HelpString;
  UINTN                   Index;
  UINT8                   *OpCodeRet;
  CAP_LOOKUP_TABLE        *TableSel;
  UINTN                   TableSize;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (Index = 0; PcieExtendedCapablityCommonInteLookUp[Index].CapabilityId != 0x00; Index++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtendedCapablityCommonInteLookUp[Index].CapabilityId) {
      TableSize = PcieExtendedCapablityCommonInteLookUp[Index].TableSize;
      TableSel = PcieExtendedCapablityCommonInteLookUp[Index].LookupTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                              );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                              );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Virtual Channel Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtCapVC (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                                        Status;
  CHAR16                                                            *FormatString;
  CHAR16                                                            *HelpString;
  CHAR16                                                            *SubTitleFormatString;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_VIRTUAL_CHANNEL_CAPABILITY_EXT  *VcCap;
  CAP_LOOKUP_TABLE                                                  *TableSel;
  UINTN                                                             Index;
  UINTN                                                             ChannelIndex;
  UINT8                                                             *OpCodeRet;
  UINTN                                                             TableSize;
  UINTN                                                             ChannelNumber;
  UINTN                                                             ChannelBaseOffset;
  UINTN                                                             TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL ||
      (mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_ID &&
       mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_MFVC &&
       mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_MULTI_FUNCTION_VIRTUAL_CHANNEL_ID)) {
    return EFI_INVALID_PARAMETER;
  }

  VcCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_VIRTUAL_CHANNEL_CAPABILITY_EXT *) mPcieExtCapHdr;

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_ID == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  ChannelNumber = VcCap->PortVcCapability1.Bits.ExtendedVcCount;

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr) + (TableSel[Index].ValueOffset)))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  if (PcieExtCapabilityDynamicInteLookUp[TableIndex].CapDynamicTable == NULL) {
    return Status;
  }

  TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapDynamicTable;
  TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].DynamicTableSize;
  SubTitleFormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_VC_CHANNEL_SUBTITLE_STRING_FORMAT), mLanguage);

  for (ChannelIndex = 0; (ChannelIndex < ChannelNumber) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); ChannelIndex++) {
    ChannelBaseOffset = offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_VIRTUAL_CHANNEL_CAPABILITY_EXT, Capability[ChannelIndex]);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      SubTitleFormatString,
      ChannelIndex
      );
    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = mPciCapabilityIntpnPromtStringIds[*InterpretationIndex];
    HiiCreateSubTitleOpCode (mStartOpCodeHandle, mPciCapabilityIntpnPromtStringIds[*InterpretationIndex], mPciCapabilityIntpnHelpStringIds[*InterpretationIndex], 0, 0);
    (*InterpretationIndex)++;

    for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
      FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        FormatString,
        ReadCapDataValue (TableSel[Index].DataSize, (ChannelBaseOffset + ((UINT8*)mPcieExtCapHdr) + (TableSel[Index].ValueOffset)))
      );

      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  mLanguage
                                                                  );

      HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        HelpString,
        (ChannelBaseOffset + (TableSel[Index].ValueOffset))
        );
      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                 mVfrFormHiiHandle,
                                                                 mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                 mStrBuffer,
                                                                 mLanguage
                                                                 );

      OpCodeRet = HiiCreateActionOpCode (
                    mStartOpCodeHandle,
                    (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                    0,
                    0
                    );
      if (OpCodeRet == NULL) {
        DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
      }
      (*InterpretationIndex)++;
      if (FormatString != NULL) {
        FreePool (FormatString);
      }
      if (HelpString != NULL) {
        FreePool (HelpString);
      }
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Link Declaration Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtLinkDec (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                          Status;
  CHAR16                                              *FormatString;
  CHAR16                                              *HelpString;
  UINTN                                               Index;
  UINT8                                               *OpCodeRet;
  CAP_LOOKUP_TABLE                                    *TableSel;
  UINTN                                               TableSize;
  UINT8                                               LinkCount;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_LINK_DECLARATION  *LinkDec;
  UINTN                                               TableIndex;
  CHAR16                                              *TitleString;
  UINTN                                               EntryIndex;
  UINTN                                               BaseAddress;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  TitleString  = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  LinkDec   = (PCI_EXPRESS_EXTENDED_CAPABILITIES_LINK_DECLARATION *) mPcieExtCapHdr;
  LinkCount = PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_GET_LINK_COUNT(LinkDec);

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic LinkEntries
  //
  TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].DynamicTableSize;
  TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapDynamicTable;

  TitleString  = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_LINK_DECLAR_ENTRY_SUBTITLE_STRING_FORMAT), mLanguage);
  FormatString = NULL;
  HelpString   = NULL;

  BaseAddress = (UINTN)(offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_LINK_DECLARATION, LinkEntry[0]));
  for (EntryIndex = 0; (EntryIndex < LinkCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); EntryIndex++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      TitleString,
      EntryIndex
      );
    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );
    OpCodeRet = HiiCreateSubTitleOpCode (
                  mStartOpCodeHandle,
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;

    for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
      FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        FormatString,
        Index,
        ReadCapDataValue (TableSel[Index].DataSize, (VOID *)(((UINT8 *)mPcieExtCapHdr) + BaseAddress + (TableSel[Index].ValueOffset)))
        );

      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  mLanguage
                                                                  );

      HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        HelpString,
        (BaseAddress + (TableSel[Index].ValueOffset))
        );

      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                 mVfrFormHiiHandle,
                                                                 mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                 mStrBuffer,
                                                                 mLanguage
                                                                 );

      OpCodeRet = HiiCreateActionOpCode (
                    mStartOpCodeHandle,
                    (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                    0,
                    0
                    );
      if (OpCodeRet == NULL) {
        DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
      }
      (*InterpretationIndex)++;
      BaseAddress += sizeof (PCI_EXPRESS_EXTENDED_CAPABILITIES_LINK_DECLARATION_ENTRIES);
      if (FormatString != NULL) {
        FreePool (FormatString);
      }
      if (HelpString != NULL) {
        FreePool (HelpString);
      }
    }
  }

  if (TitleString != NULL) {
    FreePool (TitleString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Vendor Specific Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtVS (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                          Status;
  CHAR16                                              *FormatString;
  CHAR16                                              *HelpString;
  UINT8                                               *OpCodeRet;
  UINTN                                               Index;
  UINT16                                              VsSize;
  UINT16                                              VsCount;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_VENDOR_SPECIFIC   *VsCap;
  CAP_LOOKUP_TABLE                                    *TableSel;
  UINTN                                               TableSize;
  UINTN                                               TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  VsCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_VENDOR_SPECIFIC *) mPcieExtCapHdr;
  VsSize = PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_GET_SIZE(VsCap);
  //
  // VsSize include Cap header and VsSize field
  //
  VsCount = VsSize - (sizeof(PCI_EXPRESS_EXTENDED_CAPABILITIES_HEADER) + sizeof(UINT32));

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic Vendor specify
  //
  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_VEN_DATA_STRING_FORMAT), mLanguage);
  HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_VEN_DATA_STRING_HELP), mLanguage);

  for (Index = 0; (Index < VsCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT8), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_VENDOR_SPECIFIC, VendorSpecific[Index])))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_VENDOR_SPECIFIC, VendorSpecific[Index])
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Designated Vendor-Specific Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtDesignVS (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                 Status;
  CHAR16                                     *FormatString;
  CHAR16                                     *HelpString;
  UINTN                                      Index;
  UINT8                                      *OpCodeRet;
  CAP_LOOKUP_TABLE                           *TableSel;
  UINTN                                      TableSize;
  UINT16                                     VsSize;
  UINT16                                     VsCount;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC    *DesvsCap;
  UINTN                                      TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  DesvsCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC *) mPcieExtCapHdr;
  VsSize = PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_GET_LENGTH(DesvsCap);
  //
  // VsSize include Cap header and VsSize field
  //
  VsCount = VsSize - (sizeof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC) - sizeof(UINT8));

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic Vendor specify
  //
  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DVSEC_VSREG_STRING_FORMAT), mLanguage);
  HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DVSEC_VSREG_STRING_HELP), mLanguage);

  for (Index = 0; (Index < VsCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT8), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC, DvsecData[Index])))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DVSEC, DvsecData[Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation ACS Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtAcs (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                          Status;
  CHAR16                                              *FormatString;
  CHAR16                                              *HelpString;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_EXTENDED      *AcsCap;
  CAP_LOOKUP_TABLE                                    *TableSel;
  BOOLEAN                                             ExtendSupported;
  UINT8                                               *OpCodeRet;
  UINT16                                              TableSize;
  UINTN                                               Index;
  UINTN                                               EgressSize;
  UINTN                                               EgressCount;
  UINTN                                               TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_ACS_EXTENDED_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  AcsCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_EXTENDED *) mPcieExtCapHdr;
  ExtendSupported = (((PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_CAP *)(&(AcsCap->AcsCapability)))->Bits.EnhancedCap == 1);
  EgressSize = ((PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_CAP *)(&(AcsCap->AcsCapability)))->Bits.CtlVectorSize; // Bits
  //
  // align to Dword width
  //
  EgressSize += (32 - (EgressSize%32));
  EgressCount = EgressSize / 32;

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  //
  // Dynamic Egress Vector Array
  //
  if (ExtendSupported == FALSE) {
    return Status;
  }

  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_ACS2_STRING_FORMAT), mLanguage);
  HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_ACS2_STRING_HELP), mLanguage);

  for (Index = 0; (Index < EgressSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_EXTENDED, EgressControlVectorArray[4*Index])))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_ACS_EXTENDED, EgressControlVectorArray[4*Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }

  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Resizeable BAR Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtResizableBar (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                        Status;
  CHAR16                                            *FormatString;
  CHAR16                                            *HelpString;
  UINTN                                             Index;
  UINT8                                             *OpCodeRet;
  CAP_LOOKUP_TABLE                                  *TableSel;
  UINT16                                            TableSize;
  UINTN                                             BarIndex;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_RESIZABLE_BAR   *ResBarCap;
  UINTN                                             BarCount;
  UINTN                                             BaseAddress;
  CHAR16                                            *TitleString;
  UINTN                                             TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  TitleString  = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_RESIZABLE_BAR_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].DynamicTableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapDynamicTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  ResBarCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_RESIZABLE_BAR *) mPcieExtCapHdr;
  BarCount = GET_NUMBER_RESIZABLE_BARS(ResBarCap);

  //
  // Dynamic Resizable Bar
  //
  TitleString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_RESIZE_BAR_SUBTITLE_STRING_FORMAT), mLanguage);
  for (BarIndex = 0; (BarIndex < BarCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); BarIndex++) {
    BaseAddress = offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_RESIZABLE_BAR, Capability[BarIndex]);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      TitleString,
      BarIndex
    );
    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );
    OpCodeRet = HiiCreateSubTitleOpCode (
                  mStartOpCodeHandle,
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;

    for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
      FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        FormatString,
        ReadCapDataValue (TableSel[Index].DataSize, (BaseAddress + ((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
      );

      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  mLanguage
                                                                  );

      HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        HelpString,
        TableSel[Index].ValueOffset
        );
      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                 mVfrFormHiiHandle,
                                                                 mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                 mStrBuffer,
                                                                 mLanguage
                                                                 );

      OpCodeRet = HiiCreateActionOpCode (
                    mStartOpCodeHandle,
                    (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                    0,
                    0
                    );
      if (OpCodeRet == NULL) {
        DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
      }

      (*InterpretationIndex)++;

      if (FormatString != NULL) {
        FreePool (FormatString);
      }
      if (HelpString != NULL) {
        FreePool (HelpString);
      }
    }
  }

  if (TitleString != NULL) {
    FreePool (TitleString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Dynamic Power Allocation Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtDynamicPower (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                                     Status;
  CHAR16                                                         *FormatString;
  CHAR16                                                         *HelpString;
  UINTN                                                          Index;
  UINT8                                                          *OpCodeRet;
  CAP_LOOKUP_TABLE                                               *TableSel;
  UINTN                                                          TableSize;
  UINT16                                                         DpaArraySize;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_DYNAMIC_POWER_ALLOCATION     *DynamicPowerCap;
  UINTN                                                          TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_DYNAMIC_POWER_ALLOCATION_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  DynamicPowerCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_DYNAMIC_POWER_ALLOCATION *) mPcieExtCapHdr;
  DpaArraySize = PCI_EXPRESS_EXTENDED_CAPABILITY_DYNAMIC_POWER_ALLOCATION_GET_SUBSTATE_MAX(DynamicPowerCap);

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic DPA vector
  //
  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPA2_STRING_FORMAT), mLanguage);
  HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPA2_STRING_HELP), mLanguage);

  for (Index = 0; (Index < DpaArraySize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT8), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DYNAMIC_POWER_ALLOCATION, DpaPowerAllocationArray[Index])))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DYNAMIC_POWER_ALLOCATION, DpaPowerAllocationArray[Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation TPH Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtTph (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                 Status;
  CHAR16                                     *FormatString;
  CHAR16                                     *HelpString;
  UINTN                                      Index;
  UINT8                                      *OpCodeRet;
  CAP_LOOKUP_TABLE                           *TableSel;
  UINTN                                      TableSize;
  UINTN                                      TphStTableSize;
  UINTN                                      TphStTableCount;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_TPH      *TphCap;
  CHAR16                                     *TitleString;
  UINTN                                      TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  TitleString  = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_TPH_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  TphCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_TPH *) mPcieExtCapHdr;
  TphStTableSize = GET_TPH_TABLE_SIZE(TphCap);
  TphStTableCount = TphStTableSize / sizeof(UINT16);

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
    return Status;
  }

  //
  // Dynamic Tph vector
  //
  if (TphStTableCount == 0) {
    return Status;
  }

  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_ST_TABLE_STRING_FORMAT), mLanguage);
  HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_ST_TABLE_STRING_HELP), mLanguage);
  TitleString  = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_TPH_TABLE_STRING_FORMAT), mLanguage);
  ZeroMem (mStrBuffer, sizeof (mStrBuffer));
  UnicodeSPrint (
    mStrBuffer,
    sizeof (mStrBuffer),
    TitleString
  );
  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                              mVfrFormHiiHandle,
                                                              mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                              mStrBuffer,
                                                              mLanguage
                                                              );
  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                             mVfrFormHiiHandle,
                                                             mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                             mStrBuffer,
                                                             mLanguage
                                                             );
  OpCodeRet = HiiCreateSubTitleOpCode (
                mStartOpCodeHandle,
                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                0,
                0
                );
  if (OpCodeRet == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
  }

  (*InterpretationIndex)++;

  for (Index = 0; (Index < TphStTableCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT16), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_TPH, TphStTable[Index])))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_TPH, TphStTable[Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }
  if (TitleString != NULL) {
    FreePool (TitleString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Second PCIe Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtSecPcie (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                            Status;
  CHAR16                                                *FormatString;
  CHAR16                                                *HelpString;
  UINTN                                                 Index;
  UINT8                                                 *OpCodeRet;
  CAP_LOOKUP_TABLE                                      *TableSel;
  UINTN                                                 TableSize;
  UINTN                                                 EqualizationControlCount;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_SECONDARY_PCIE      *SecPcieCap;
  CHAR16                                                *TitleString;
  UINTN                                                 TableIndex;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  TitleString  = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_SECONDARY_PCIE_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  SecPcieCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_SECONDARY_PCIE *) mPcieExtCapHdr;
  EqualizationControlCount = mPciExpCap->LinkCapability.Bits.MaxLinkWidth;

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic Area
  //
  if (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE) {
    TitleString  = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_SECONDARY_EQCTL_STRING_FORMAT), mLanguage);
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_EQ_CTRL_LANE_STRING_FORMAT), mLanguage);
    HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_EQ_CTRL_LANE_STRING_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      TitleString
    );
    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );
    OpCodeRet = HiiCreateSubTitleOpCode (
                  mStartOpCodeHandle,
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;
  }

  for (Index = 0; (Index < EqualizationControlCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT16), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_SECONDARY_PCIE, EqualizationControl[Index])))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_SECONDARY_PCIE, EqualizationControl[Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }
  if (TitleString != NULL) {
    FreePool (TitleString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Lane Margining Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtLaneMargining (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                       Status;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_LANE_MARGINING *LaneMargin;
  CAP_LOOKUP_TABLE                                 *TableSel;
  CHAR16                                           *FormatString;
  CHAR16                                           *HelpString;
  UINT8                                            *OpCodeRet;
  UINTN                                            TableSize;
  UINTN                                            EntryCount;
  UINTN                                            TableIndex;
  UINTN                                            EntryOffset;
  UINTN                                            LaneIndex;
  UINTN                                            Index;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_LANE_MARGINING_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  LaneMargin = (PCI_EXPRESS_EXTENDED_CAPABILITIES_LANE_MARGINING *) mPcieExtCapHdr;

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }

    //
    // Dynamic Lane Entry
    //
    EntryCount  = mPciExpCap->LinkCapability.Bits.MaxLinkWidth;
    TableSel    = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapDynamicTable;
    TableSize   = PcieExtCapabilityDynamicInteLookUp[TableIndex].DynamicTableSize;
    EntryOffset = offsetof (PCI_EXPRESS_EXTENDED_CAPABILITIES_LANE_MARGINING, MarginingRegister[0]);
    for (LaneIndex = 0; (LaneIndex < EntryCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); LaneIndex++) {
      for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
        FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
        ZeroMem (mStrBuffer, sizeof (mStrBuffer));
        UnicodeSPrint (
          mStrBuffer,
          sizeof (mStrBuffer),
          FormatString,
          LaneIndex,
          ReadCapDataValue (TableSel[Index].DataSize, (EntryOffset +((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
          );

        mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                    mVfrFormHiiHandle,
                                                                    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                    mStrBuffer,
                                                                    mLanguage
                                                                    );

        HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
        ZeroMem (mStrBuffer, sizeof (mStrBuffer));
        UnicodeSPrint (
          mStrBuffer,
          sizeof (mStrBuffer),
          HelpString,
          (EntryOffset + (TableSel[Index].ValueOffset))
          );
        mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                   mVfrFormHiiHandle,
                                                                   mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                   mStrBuffer,
                                                                   mLanguage
                                                                   );

        OpCodeRet = HiiCreateActionOpCode (
                      mStartOpCodeHandle,
                      (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                      0,
                      0
                      );
        if (OpCodeRet == NULL) {
          DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
        }
        (*InterpretationIndex)++;
        EntryOffset += (LaneIndex * sizeof(MARGINING_REGISTER));
        if (FormatString != NULL) {
          FreePool (FormatString);
        }
        if (HelpString != NULL) {
          FreePool (HelpString);
        }
      }
    }
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Physical layer 32GT/s Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtPhyLayer32 (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                            Status;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_32_0 *SecPcieCap;
  CAP_LOOKUP_TABLE                                      *TableSel;
  CHAR16                                                *TitleString;
  CHAR16                                                *FormatString;
  CHAR16                                                *HelpString;
  UINT8                                                 *OpCodeRet;
  UINTN                                                 LaneEqualizationControlCount;
  UINTN                                                 TableIndex;
  UINTN                                                 TableSize;
  UINTN                                                 Index;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  TitleString  = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_32_0_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  SecPcieCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_32_0 *) mPcieExtCapHdr;
  LaneEqualizationControlCount = mPciExpCap->LinkCapability.Bits.MaxLinkWidth;

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic Area
  //
  if (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE) {
    TitleString  = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_PHY_LAYER16_EQCTL_TITLE_STRING_FORMAT), mLanguage);
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_PHY_LAYER16_EQCTL_STRING_FORMAT), mLanguage);
    HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_PHY_LAYER16_EQCTL_STRING_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      TitleString
    );
    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );
    OpCodeRet = HiiCreateSubTitleOpCode (
                  mStartOpCodeHandle,
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }

    (*InterpretationIndex)++;
  }

  for (Index = 0; (Index < LaneEqualizationControlCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT8), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_16_0, LaneEqualizationControl[Index])))
      );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_16_0, LaneEqualizationControl[Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }
  if (TitleString != NULL) {
    FreePool (TitleString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation Physical Layer 16GT/s Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtPhyLayer16 (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                                            Status;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_16_0 *SecPcieCap;
  CAP_LOOKUP_TABLE                                      *TableSel;
  CHAR16                                                *FormatString;
  CHAR16                                                *TitleString;
  CHAR16                                                *HelpString;
  UINTN                                                 LaneEqualizationControlCount;
  UINTN                                                 TableIndex;
  UINTN                                                 TableSize;
  UINTN                                                 Index;
  UINT8                                                 *OpCodeRet;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  TitleString  = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_16_0_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (TableIndex = 0; PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId != 0x0000; TableIndex++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[TableIndex].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[TableIndex].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[TableIndex].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  SecPcieCap = (PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_16_0 *) mPcieExtCapHdr;
  LaneEqualizationControlCount = mPciExpCap->LinkCapability.Bits.MaxLinkWidth;

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic Area
  //
  if (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE) {
    TitleString  = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_PHY_LAYER16_EQCTL_TITLE_STRING_FORMAT), mLanguage);
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_PHY_LAYER16_EQCTL_STRING_FORMAT), mLanguage);
    HelpString   = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_PHY_LAYER16_EQCTL_STRING_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      TitleString
    );
    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                              mVfrFormHiiHandle,
                                                              mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                              mStrBuffer,
                                                              mLanguage
                                                              );
    OpCodeRet = HiiCreateSubTitleOpCode (
                  mStartOpCodeHandle,
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }

  for (Index = 0; (Index < LaneEqualizationControlCount) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT8), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_16_0, LaneEqualizationControl[Index])))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );

    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_PHYSICAL_LAYER_16_0, LaneEqualizationControl[Index]))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }
  if (TitleString != NULL) {
    FreePool (TitleString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Interpretation DPC Extended Capability to SCU

  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  mPcieExtCapHdr is NULL or the cap id is not used this funciton.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
  @retval EFI_NOT_FOUND          The cap id can not found in lookup table
**/
EFI_STATUS
InterpretationPcieExtCapDpc (
  IN OUT UINTN                   *InterpretationIndex,
  IN     EFI_QUESTION_ID         BaseQuestionId
  )
{
  EFI_STATUS                            Status;
  PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC *Dpc;
  CAP_LOOKUP_TABLE                      *TableSel;
  CHAR16                                *FormatString;
  CHAR16                                *HelpString;
  UINT8                                 *OpCodeRet;
  UINTN                                 TableSize;
  UINTN                                 PioLogSize;
  UINTN                                 Index;

  Status = EFI_SUCCESS;
  TableSel = NULL;
  FormatString = NULL;
  HelpString   = NULL;
  OpCodeRet    = NULL;

  if (mPcieExtCapHdr == NULL || mPcieExtCapHdr->CapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_ID) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Search Lookup Table
  //
  for (Index = 0; PcieExtCapabilityDynamicInteLookUp[Index].CapabilityId != 0x0000; Index++) {
    if (mPcieExtCapHdr->CapabilityId == PcieExtCapabilityDynamicInteLookUp[Index].CapabilityId) {
      TableSize = PcieExtCapabilityDynamicInteLookUp[Index].TableSize;
      TableSel = PcieExtCapabilityDynamicInteLookUp[Index].CapCommonTable;
      break;
    }
  }
  if (TableSel == NULL) {
    DEBUG((EFI_D_ERROR, "%a() - This CapId Not Support In LookUp Table\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }
  Dpc = (PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC *)mPcieExtCapHdr;
  PioLogSize = PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_GET_PIOLOGSIZE(Dpc);

  for (Index = 0; (Index < TableSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].TextId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (TableSel[Index].DataSize, (((UINT8*)mPcieExtCapHdr)) + (TableSel[Index].ValueOffset))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, TableSel[Index].HelpId, mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      TableSel[Index].ValueOffset
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Optional PioHeaderLog
  //
  if (PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_RP_EXTENTION_SUPPORTED(Dpc) == 1) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPC_PIOHEADER_STRING_FORMAT), mLanguage);
    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPC_PIOHEADER_STRING_HELP), mLanguage);
    for (Index = 0; (Index < PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_HEADER_LOG_SIZE) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        FormatString,
        Index,
        ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPcieExtCapHdr)) + offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC, PioHeaderLog[Index]))
        );

      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  mLanguage
                                                                  );


      ZeroMem (mStrBuffer, sizeof (mStrBuffer));
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        HelpString,
        offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC, PioHeaderLog[Index])
        );
      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                 mVfrFormHiiHandle,
                                                                 mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                 mStrBuffer,
                                                                 mLanguage
                                                                 );

      OpCodeRet = HiiCreateActionOpCode (
                    mStartOpCodeHandle,
                    (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                    0,
                    0
                    );
      if (OpCodeRet == NULL) {
        DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
      }
      (*InterpretationIndex)++;
      PioLogSize--;
    }
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Optional PioImpspecLog
  //
  if ((PioLogSize > 0) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE)) {
    FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPC_PIOIMPSPEC_STRING_FORMAT), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPcieExtCapHdr)) + (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC, PioImpspecLog)))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPC_PIOIMPSPEC_STRING_HELP), mLanguage);
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      (offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC, PioImpspecLog))
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    PioLogSize--;
    if (FormatString != NULL) {
      FreePool (FormatString);
    }
    if (HelpString != NULL) {
      FreePool (HelpString);
    }
  }

  //
  // Dynamic Tlp Prefix Area
  //
  FormatString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPC_PIOTLPPREFIX_STRING_FORMAT), mLanguage);
  HelpString = HiiGetString (mCapabilityIntnStringPackHandle, STRING_TOKEN(STR_PCI_EXT_CAP_DPC_PIOTLPPREFIX_STRING_HELP), mLanguage);

  for (Index = 0; (Index < PioLogSize) && (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE); Index++) {
    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      FormatString,
      Index,
      ReadCapDataValue (sizeof(UINT32), (((UINT8*)mPcieExtCapHdr)) + offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC, PioTlpPrefixLog[Index]))
    );

    mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                mVfrFormHiiHandle,
                                                                mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                mStrBuffer,
                                                                mLanguage
                                                                );


    ZeroMem (mStrBuffer, sizeof (mStrBuffer));
    UnicodeSPrint (
      mStrBuffer,
      sizeof (mStrBuffer),
      HelpString,
      offsetof(PCI_EXPRESS_EXTENDED_CAPABILITIES_DPC, PioTlpPrefixLog[Index])
      );
    mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                               mVfrFormHiiHandle,
                                                               mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                               mStrBuffer,
                                                               mLanguage
                                                               );

    OpCodeRet = HiiCreateActionOpCode (
                  mStartOpCodeHandle,
                  (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                  mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                  0,
                  0
                  );
    if (OpCodeRet == NULL) {
      DEBUG((EFI_D_ERROR, "%a() - Opcode Out of Resourse\n", __FUNCTION__));
    }
    (*InterpretationIndex)++;
    PioLogSize--;
  }
  if (FormatString != NULL) {
    FreePool (FormatString);
  }
  if (HelpString != NULL) {
    FreePool (HelpString);
  }

  if (*InterpretationIndex >= DEFAULT_STRING_ID_BUFFER_SIZE) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG((
      EFI_D_ERROR,
      "%a() - The InterpretationIndex(0x%d) is out of string id buffer (0x%x) range\n",
      __FUNCTION__,
      *InterpretationIndex,
      DEFAULT_STRING_ID_BUFFER_SIZE
      ));
  }

  return Status;
}

/**
  Create Specific Capability Interpretatin Item to SCU

  @param HiiHandle             - Target EFI_HII_HANDLE instance.
  @param CurrentPciDeviceInfo  - Current Pci device.
  @param StartOpCodeHandle     - Hii start Opcode to add new Item.
  @param CapabilityHdr         - Specific Capability Struct.
  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.
  @param Language              - Hii Supported Language.

  @retval EFI_SUCCESS            The pci capability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  CapabilityHdr is NULL.
  @retval EFI_OUT_OF_RESOURCES   The pci capability interpretation string id buffer is full.
**/
EFI_STATUS
LibCreatePciCapabilityInterpretation (
  IN       EFI_HII_HANDLE          HiiHandle,
  IN       PCI_DEVICE_INFO         *CurrentPciDeviceInfo,
  IN       VOID                    *StartOpCodeHandle,
  IN       EFI_PCI_CAPABILITY_HDR  *CapabilityHdr,
  IN  OUT  UINTN                   *InterpretationIndex,
  IN       EFI_QUESTION_ID         BaseQuestionId,
  IN       CHAR8                   *Language
  )
{
  CHAR16      *DefaultStr;
  EFI_STATUS  Status;

  if (CapabilityHdr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((EFI_D_INFO, "PciCapability - Start Interpretation: 0x%02x\n", CapabilityHdr->CapabilityID));

  mVfrFormHiiHandle = HiiHandle;
  mCurrentPciDeviceInfo = CurrentPciDeviceInfo;
  mLanguage   = Language;
  mPciCapHdr  = CapabilityHdr;
  mStartOpCodeHandle = StartOpCodeHandle;
  DefaultStr = DEFAULT_INTPN_STRING;
  Status = EFI_SUCCESS;

  ZeroMem (mStrBuffer, sizeof (mStrBuffer));

  switch (mPciCapHdr->CapabilityID) {

  case EFI_PCI_CAPABILITY_ID_PMI:       // 01h
  case EFI_PCI_CAPABILITY_ID_AGP:       // 02h
  case EFI_PCI_CAPABILITY_ID_VPD:       // 03h
  case EFI_PCI_CAPABILITY_ID_SLOTID:    // 04h
  case H2O_PCI_CAPABILITY_ID_SSVID:     // 0Dh
  case EFI_PCI_CAPABILITY_ID_PCIEXP:    // 10h
  case H2O_PCI_CAPABILITY_ID_MSIX:      // 11h
  case EFI_PCI_CAPABILITY_ID_AF:        // 13h
  case H2O_PCI_CAPABILITY_ID_FPB:       // 15h
    Status = InterpretationPciCapCommon (InterpretationIndex, BaseQuestionId);
    break;

  case EFI_PCI_CAPABILITY_ID_MSI:       // 05h
    Status = InterpretationPciCapMSI (InterpretationIndex, BaseQuestionId);
    break;

  case EFI_PCI_CAPABILITY_ID_PCIX:      // 07h
    Status = InterpretationPciCapPCIX (InterpretationIndex, BaseQuestionId);
    break;

  case EFI_PCI_CAPABILITY_ID_VENDOR:    // 09h
    Status = InterpretationPciCapVS (InterpretationIndex, BaseQuestionId);
    break;

  case H2O_PCI_CAPABILITY_ID_ENHANCED_ALLOCATION: // 14h
    Status = InterpretationPciCapEA (InterpretationIndex, BaseQuestionId);
    break;

  case EFI_PCI_CAPABILITY_ID_HOTPLUG:   // 06h
  case EFI_PCI_CAPABILITY_ID_SHPC:      // 0Ch
  default:
    if (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE) {
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        DefaultStr
        );
      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  NULL
                                                                  );
      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                 mVfrFormHiiHandle,
                                                                 mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                 mStrBuffer,
                                                                 NULL
                                                                 );
      HiiCreateActionOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
        mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
        mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
        0,
        0
      );
      (*InterpretationIndex)++;
    } else {
      Status = EFI_OUT_OF_RESOURCES;
    }
    break;
  }

  DEBUG((EFI_D_INFO, "PciCapability - End Interpretation, Status: %r\n", Status));

  return Status;
}

/**
  Create Specific Capability Interpretatin Item to SCU

  @param HiiHandle             - Target EFI_HII_HANDLE instance.
  @param CurrentPciDeviceInfo  - Current Pci device.
  @param StartOpCodeHandle     - Hii start Opcode to add new Item.
  @param CapabilityHdr         - Specific Capability Struct.
  @param InterpretationIndex   - Index for all of Intn Item.
  @param BaseQuestionId        - Base Qid for Intn Item.
  @param PciExpCap             - Pointer to PCIe Extended Capability.
  @param Language              - Hii Supported Language.

  @retval EFI_SUCCESS            The pcie extcapability interpretation is create successfully.
  @retval EFI_INVALID_PARAMETER  CapabilityHdr is NULL.
  @retval EFI_OUT_OF_RESOURCES   The pcie extcapability interpretation string id buffer is full.
**/
EFI_STATUS
LibCreatePcieExtCapabilityInterpretation (
  IN       EFI_HII_HANDLE          HiiHandle,
  IN       PCI_DEVICE_INFO         *CurrentPciDeviceInfo,
  IN       VOID                    *StartOpCodeHandle,
  IN       PCI_EXP_EXT_HDR         *CapabilityHdr,
  IN  OUT  UINTN                   *InterpretationIndex,
  IN       EFI_QUESTION_ID         BaseQuestionId,
  IN       PCI_CAPABILITY_PCIEXP   *PciExpCap,     OPTIONAL
  IN       CHAR8                   *Language       OPTIONAL
  )
{
  CHAR16      *DefaultStr;
  EFI_STATUS  Status;

  if (CapabilityHdr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((EFI_D_INFO, "PcieExtCapability - Start Interpretation: 0x%04x\n", CapabilityHdr->CapabilityId));

  Status = EFI_SUCCESS;
  mVfrFormHiiHandle     = HiiHandle;
  mCurrentPciDeviceInfo = CurrentPciDeviceInfo;
  mLanguage             = Language;
  mPcieExtCapHdr        = CapabilityHdr;
  mStartOpCodeHandle    = StartOpCodeHandle;
  DefaultStr = DEFAULT_INTPN_STRING;
  mPciExpCap = PciExpCap;
  ZeroMem (mStrBuffer, sizeof (mStrBuffer));

  switch (CapabilityHdr->CapabilityId) {

  //
  // Common Intn
  //
  case PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID:               // 0001h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_SERIAL_NUMBER_ID:                          // 0003h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_POWER_BUDGETING_ID:                        // 0004h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_CONTROL_ID:                           // 0006h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_EVENT_COLLECTOR_ENDPOINT_ASSOCIATION_ID:   // 0007h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_RCRB_HEADER_ID:                            // 000Ah
  case PCI_EXPRESS_EXTENDED_CAPABILITY_ARI_CAPABILITY_ID:                         // 000Eh
  case EFI_PCIE_CAPABILITY_ID_SRIOV:                                              // 0010h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_MULTICAST_ID:                              // 0012h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_LATENCE_TOLERANCE_REPORTING_ID:            // 0018h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_PASID_ID:                                  // 001Bh
  case PCI_EXPRESS_EXTENDED_CAPABILITY_LNR_ID:                                    // 001Ch
  case PCI_EXPRESS_EXTENDED_CAPABILITY_L1_PM_SUBSTATES_ID:                        // 001Eh
  case PCI_EXPRESS_EXTENDED_CAPABILITY_PTM_ID:                                    // 001Fh
  case PCI_EXPRESS_EXTENDED_CAPABILITY_FRS_QUEUE_ID:                              // 0021h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_RTR_ID:                                    // 0022h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_SFI_ID:                                    // 002Ch
  case PCI_EXPRESS_EXTENDED_CAPABILITY_DATA_LINK_ID:                              // 0025h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_HIERARCHY_ID_ID:                           // 0028h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_NPEM_ID:                                   // 0029h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_ALTERNATE_PROTOCOL_ID:                     // 002Bh
    Status = InterpretationPcieExtCapCommon (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_VIRTUAL_CHANNEL_ID:                        // 0002h
  case PCI_EXPRESS_EXTENDED_CAPABILITY_MULTI_FUNCTION_VIRTUAL_CHANNEL_ID:         // 0008h
    Status = InterpretationPcieExtCapVC (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_LINK_DECLARATION_ID:                       // 0005h
    Status = InterpretationPcieExtLinkDec (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID:                        // 000Bh
    Status = InterpretationPcieExtVS(InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_ACS_EXTENDED_ID:                           // 000Dh
    Status = InterpretationPcieExtAcs(InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_RESIZABLE_BAR_ID:                          // 0015h
    Status = InterpretationPcieExtResizableBar (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_DYNAMIC_POWER_ALLOCATION_ID:               // 0016h
    Status = InterpretationPcieExtDynamicPower (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_TPH_ID:                                    // 0017h
    Status = InterpretationPcieExtTph (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_SECONDARY_PCIE_ID:                         // 0019h
    Status = InterpretationPcieExtSecPcie (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_16_0_ID:                    // 0026h
    Status = InterpretationPcieExtPhyLayer16 (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_PHYSICAL_LAYER_32_0_ID:                    // 002Ah
    Status = InterpretationPcieExtPhyLayer32 (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_LANE_MARGINING_ID:                         // 0027h
    Status = InterpretationPcieExtLaneMargining (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_DPC_ID:                                    // 001Dh
    Status = InterpretationPcieExtCapDpc (InterpretationIndex, BaseQuestionId);
    break;

  case PCI_EXPRESS_EXTENDED_CAPABILITY_DVSEC_ID:                                  // 0023h
    Status = InterpretationPcieExtDesignVS (InterpretationIndex, BaseQuestionId);
    break;

  default:
    if (*InterpretationIndex < DEFAULT_STRING_ID_BUFFER_SIZE) {
      UnicodeSPrint (
        mStrBuffer,
        sizeof (mStrBuffer),
        DefaultStr
        );
      mPciCapabilityIntpnPromtStringIds[*InterpretationIndex] = HiiSetString (
                                                                  mVfrFormHiiHandle,
                                                                  mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
                                                                  mStrBuffer,
                                                                  NULL
                                                                  );
      mPciCapabilityIntpnHelpStringIds[*InterpretationIndex] = HiiSetString (
                                                                 mVfrFormHiiHandle,
                                                                 mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
                                                                 mStrBuffer,
                                                                 NULL
                                                                 );
      HiiCreateActionOpCode (
        StartOpCodeHandle,
        (EFI_QUESTION_ID)(BaseQuestionId + (*InterpretationIndex)),
        mPciCapabilityIntpnPromtStringIds[*InterpretationIndex],
        mPciCapabilityIntpnHelpStringIds[*InterpretationIndex],
        0,
        0
        );
      (*InterpretationIndex)++;
    } else {
      Status = EFI_OUT_OF_RESOURCES;
    }
    break;
  }

  DEBUG((EFI_D_INFO, "PcieExtCapability - End Interpretation, Status: %r\n", Status));

  return Status;
}
