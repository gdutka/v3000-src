/** @file
  A PEIM for installing gPchHdaVerbTablePpiGuid PPI.

;******************************************************************************
;* Copyright (c) 2014, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiOemSvcKernelLib.h>
#include <Library/DebugLib.h>
#include <Ppi/PchHdaVerbTable.h>
#include <Ppi/VerbTable.h>
#include <Library/MemoryAllocationLib.h>

///
/// Azalia codec verb table prototope
///
typedef struct  {
  PCH_HDA_VERB_TABLE_HEADER  Header;  ///< Azalia codec verb table header.
  UINT32  Data[1];                      ///< Verb table data       
} PCH_HDA_VERB_TABLE_PPI;

HDAUDIO_VERB_TABLE HdaVerbTableDisplayAudio = {
  //
  //  VerbTable:
  //  Revision ID = 0xFF
  //  Codec Vendor: 0x80862809
  //
  {
    0x8086,     // Vendor ID
    0x2809,     // Device ID
    0xFF,       // Revision ID (any)
    0xFF,       // SDI number (any)
    3 * 4 + 2   // Data size in DWORDs
  },
  {
    //
    // Display Audio Verb Table
    //   
    // Enable the third converter and Pin first (NID 08h)
    0x00878101,
    // Pin Widget 5 - PORT B - Configuration Default: 0x18560010
    0x00571C10,
    0x00571D00,
    0x00571E56,
    0x00571F18,
    // Pin Widget 6 - PORT C - Configuration Default: 0x18560020
    0x00671C20,
    0x00671D00,
    0x00671E56,
    0x00671F18,
    // Pin Widget 7 - PORT D - Configuration Default: 0x18560030
    0x00771C30,
    0x00771D00,
    0x00771E56,
    0x00771F18,
    // Disable the third converter and third Pin (NID 08h)
    0x00878100
  }
};

STATIC
EFI_STATUS
InternalInstallVerbTable (
  IN  HDAUDIO_VERB_TABLE         *VerbTable
  )
{
  EFI_PEI_PPI_DESCRIPTOR  *VerbTablePpiDesc;
  EFI_STATUS               Status;

  VerbTablePpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (VerbTablePpiDesc != NULL);
  if (VerbTablePpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  VerbTablePpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  VerbTablePpiDesc->Guid  = &gPchHdaVerbTablePpiGuid;
  VerbTablePpiDesc->Ppi   = VerbTable;

  DEBUG ((DEBUG_INFO,
      "PCH HD-Audio installing verb table for vendor = 0x%04X devId = 0x%04X (size = %d DWords)\n",
      VerbTable->Header.VendorId,
      VerbTable->Header.DeviceId,
      VerbTable->Header.DataDwords));

  Status = PeiServicesInstallPpi (VerbTablePpiDesc);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  A function for transfering the VerbTable header from common code format to RC format.

  @param [in]       CommonVerbTableHeader   The VerbTable header gotten by OemSvcGetVerbTable() is in coommon code format.
  @param [out]      PchHdaVerbTableHeader   The VerbTable header in RC format.

  @retval     EFI_SUCCESS 

**/
EFI_STATUS
TransferVerbTableHeader (
  IN  COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER  **CommonVerbTableHeader,
  OUT PCH_HDA_VERB_TABLE_HEADER                *PchHdaVerbTableHeader
  )
{
  COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER      *CommonTransferHeader;
  UINT16                                       TotalJackNum;

  CommonTransferHeader = (COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER*)*CommonVerbTableHeader;
  TotalJackNum = CommonTransferHeader->NumberOfRearJacks + CommonTransferHeader->NumberOfFrontJacks;

  (*PchHdaVerbTableHeader).VendorId = (UINT16)(CommonTransferHeader->VendorDeviceId >> 16);
  (*PchHdaVerbTableHeader).DeviceId = (UINT16)(CommonTransferHeader->VendorDeviceId & 0xFFFF);
  (*PchHdaVerbTableHeader).RevisionId = CommonTransferHeader->RevisionId;
  (*PchHdaVerbTableHeader).SdiNo = 0xFF;
  (*PchHdaVerbTableHeader).DataDwords = TotalJackNum * 4;
  (*PchHdaVerbTableHeader).Reserved = 0;

  return EFI_SUCCESS;
}


/**
  A PEIM for installing gPchHdaVerbTablePpiGuid PPI.

  @param      FileHandle              Handle of the file being invoked.
  @param      PeiServices             Pointer to PEI Services table.

  @retval     EFI_SUCCESS 

**/
EFI_STATUS
PeimInstallPchHdaVerbTableEntry (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                               Status;
  UINT8                                    Index;
  UINT16                                   DataDwords;
  UINT32                                   DataIndex;
  COMMON_CHIPSET_AZALIA_VERB_TABLE         *OemVerbTableHeaderDataAddress;
  COMMON_CHIPSET_AZALIA_VERB_TABLE_HEADER  *OemVerbTableHeader;
  UINT32                                   *OemVerbTableDataBuffer;
  PCH_HDA_VERB_TABLE_HEADER                PchHdaVerbTableHeader;
  PCH_HDA_VERB_TABLE_PPI                   *OemVerbTablePpi;
  EFI_PEI_PPI_DESCRIPTOR                   *OemVerbTablePpiDesc;

  OemVerbTableHeaderDataAddress = NULL;
  InternalInstallVerbTable (&HdaVerbTableDisplayAudio);
  Status = OemSvcGetVerbTable (&OemVerbTableHeaderDataAddress);
  if (Status == EFI_UNSUPPORTED || OemVerbTableHeaderDataAddress == NULL) {
    return Status;
  }

  for (Index = 0; OemVerbTableHeaderDataAddress[Index].VerbTableHeader != NULL; Index++) {
    OemVerbTableHeader  = NULL;
    OemVerbTablePpi     = NULL;
    OemVerbTablePpiDesc = NULL;
    DataDwords = (UINT16)(OemVerbTableHeaderDataAddress[Index].VerbTableHeader->NumberOfFrontJacks + \
                 OemVerbTableHeaderDataAddress[Index].VerbTableHeader->NumberOfRearJacks) * 4;

    Status = PeiServicesAllocatePool ((sizeof (PCH_HDA_VERB_TABLE_HEADER) + (DataDwords * sizeof(UINT32))), (VOID **)&OemVerbTablePpi);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    Status = PeiServicesAllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR), (VOID **)&OemVerbTablePpiDesc);
    if (EFI_ERROR(Status)) {
      return Status;
    }
    OemVerbTablePpiDesc->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    OemVerbTablePpiDesc->Guid = &gPchHdaVerbTablePpiGuid;
    OemVerbTablePpiDesc->Ppi = OemVerbTablePpi;

    OemVerbTableHeader = OemVerbTableHeaderDataAddress[Index].VerbTableHeader;
    OemVerbTableDataBuffer = OemVerbTableHeaderDataAddress[Index].VerbTableData;
    TransferVerbTableHeader (&OemVerbTableHeader, &PchHdaVerbTableHeader);
    OemVerbTablePpi->Header = PchHdaVerbTableHeader;
    for (DataIndex = 0; DataIndex < OemVerbTablePpi->Header.DataDwords; DataIndex++) {
      OemVerbTablePpi->Data[DataIndex] = *((UINT32 *)OemVerbTableDataBuffer + DataIndex);
    }
    
    Status = PeiServicesInstallPpi(OemVerbTablePpiDesc);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_INFO, "\n\nInstall VerbTable PPI : %r \n", Status));
    }
  }

  return Status;
}
