/** @file
  Initialize board identifier and PCD SKU identifier.
  After the board identifier has been set, then install board identifier set PPIs.

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>
#include <PostCode.h>
#include <Ppi/H2OBoardIdSet.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiOemSvcKernelLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <H2OBoardId.h>

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiH2OBoardIdSet = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gH2OBoardIdSetPpiGuid,
  NULL
};

/**
 Debug print the warning message for unsupported board id.
**/
VOID
DebugPrintBoardIdUnsupportedMsg (
  VOID
  )
{
  H2O_BOARD_ID                    *BoardIds;
  H2O_BOARD_ID                    *BoardIdList;
  UINT8                           *BoardIdSupportedList;
  UINTN                           BoardIdsSize;
  UINTN                           BoardIdListSize;
  UINTN                           BoardIdSupportedListSize;
  UINTN                           Index;

  DEBUG ((EFI_D_ERROR, "------------------------ PlatformBoardIdPei Print Begin -------------------\n"));
  DEBUG ((EFI_D_ERROR, "WARNING! This BIOS does not support specified BoardId. "));

  DEBUG ((EFI_D_ERROR, "This BIOS only supports Skuids"));
  BoardIds     = (H2O_BOARD_ID *) PcdGetPtr (PcdH2OBoardIds);
  BoardIdsSize = PcdGetSize (PcdH2OBoardIds);

  for (Index = 0; Index < (BoardIdsSize / sizeof(H2O_BOARD_ID)); Index++) {
    DEBUG ((EFI_D_ERROR | EFI_D_INFO, " 0x%lx", BoardIds[Index]));
  }

  BoardIdList = (UINT64 *)PcdGetPtr(PcdH2OBoardIdList);
  BoardIdListSize = PcdGetSize(PcdH2OBoardIdList);
  DEBUG((EFI_D_ERROR | EFI_D_INFO, "\n SkuIds List \n"));
  for (Index = 0; Index < (BoardIdListSize / sizeof(H2O_BOARD_ID)); Index++) {
    DEBUG((EFI_D_ERROR | EFI_D_INFO, " 0x%lx", BoardIdList[Index]));
  }

  DEBUG((EFI_D_ERROR | EFI_D_INFO, "\n SKU Supported List \n"));
  BoardIdSupportedList = (UINT8 *)PcdGetPtr(PcdH2OBoardIdSupportedList);
  BoardIdSupportedListSize = PcdGetSize(PcdH2OBoardIdSupportedList);
  for (Index = 0; Index < BoardIdSupportedListSize; Index++) {
    DEBUG((EFI_D_ERROR | EFI_D_INFO, "%4x", BoardIdSupportedList[Index]));
  }

  DEBUG ((EFI_D_ERROR, "\n"));

  DEBUG ((EFI_D_ERROR, "Verify that board %%d|%%s is listed in [SkuIds] in Project.dsc "));
  DEBUG ((EFI_D_ERROR, "and this board name is listed in TargetBoards in Project.cfg.\n"));
  DEBUG ((EFI_D_ERROR, "%%d = board number, %%s = board name\n"));
  DEBUG ((EFI_D_ERROR, "------------------------ PlatformBoardIdPei Print End ---------------------\n"));
}

/**
 Initializes board identifier and PCD SKU identifier.

 @param[in] FfsHeader          Header for file of Firmware File System
 @param[in] PeiServices        The PEI core services table.

 @retval EFI_SUCCESS            Initialize board identifier successfully.
 @retval EFI_OUT_OF_RESOURCES   Fail to allocate memory.
**/
EFI_STATUS
EFIAPI
PlatformBoardIdPeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE          FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  H2O_BOARD_ID                    BoardId;
  EFI_STATUS                      Status;
  EFI_PEI_PPI_DESCRIPTOR          *H2OBoardIdsSetPpiDesc;

  H2O_BOARD_ID                    PcdSkuId;

  //
  // Initialize H2O board ID and PCD SKU ID.
  //
  DEBUG ((EFI_D_INFO, "------------------------ PlatformBoardId get SKU START -------------------\n"));
  BoardId = PcdGet64 (PcdH2OBoardId);
  DEBUG ((EFI_D_INFO, "  PCD get BoardId = 0x%lx\n", BoardId));
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices Call: OemSvcGetBoardId \n"));
  Status  = OemSvcGetBoardId (&BoardId);
  DEBUG_OEM_SVC ((DEBUG_INFO, "OemKernelServices OemSvcGetBoardId Status: %r\n", Status));
  DEBUG ((EFI_D_INFO, "  OemSvcGetBoardId get BoardId = 0x%lx,  ReturnStatus = %r\n", BoardId, Status));
  if (BoardId & H2O_64_HIGH_BIT_MASK) {
    DEBUG ((EFI_D_INFO, "BoardId is 0x%lx, bits 32-63 are non-zero\n", BoardId));
  }
  DEBUG ((EFI_D_INFO, "------------------------ PlatformBoardId get SKU END ---------------------\n"));
  if (Status == EFI_SUCCESS) {
    return Status;
  }
  DEBUG ((EFI_D_INFO, "------------------------ PlatformBoardId set SKU START -------------------\n"));

  DEBUG_CODE (
    if (!H2O_BOARD_SUPPORTED(BoardId)) {
      DebugPrintBoardIdUnsupportedMsg ();
    }
  );
  LibPcdSetSku ((UINTN) BoardId);
  PcdSkuId = (H2O_BOARD_ID) LibPcdGetSku ();
  DEBUG ((EFI_D_INFO, "  LibPcdGetSku = 0x%lx\n", PcdSkuId));
  if (PcdSkuId != BoardId) {
    DEBUG ((EFI_D_ERROR, " Use BoardId 0x%lx to call LibPcdSetSku failed,\n", BoardId));
    DEBUG ((EFI_D_ERROR, " it should check the BoardId value is whether declared in both [SkuIds] and SKUID_IDENTIFIER of Project.dsc.\n"));
    DEBUG ((EFI_D_ERROR, " Report post code 0x8E PEI_BOARD_ID_SETUP_FAILED and then call CpuDeadLoop()\n"));
    POST_CODE (PEI_BOARD_ID_SETUP_FAILED);
    ASSERT (PcdSkuId != BoardId);
    CpuDeadLoop ();
  }
  DEBUG ((EFI_D_INFO, "------------------------ PlatformBoardId set SKU END ---------------------\n"));

  Status = PcdSet64S (PcdH2OBoardId, BoardId);
  ASSERT_EFI_ERROR (Status);

  //
  // Install H2OBoardIdsSet and H2OBoardIdsSet PPIs.
  //
  Status = PeiServicesInstallPpi (&mPpiH2OBoardIdSet);
  ASSERT_EFI_ERROR (Status);

  H2OBoardIdsSetPpiDesc = AllocatePool (sizeof (EFI_PEI_PPI_DESCRIPTOR) + sizeof (EFI_GUID));
  if (H2OBoardIdsSetPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  H2OBoardIdsSetPpiDesc->Flags = (UINTN) (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  H2OBoardIdsSetPpiDesc->Guid  = (EFI_GUID *) ((UINT8 *) H2OBoardIdsSetPpiDesc + sizeof (EFI_PEI_PPI_DESCRIPTOR));
  H2OBoardIdsSetPpiDesc->Ppi   = NULL;

  CopyGuid (H2OBoardIdsSetPpiDesc->Guid, &gH2OBoardIdDepexPpiGuid);
  H2OBoardIdsSetPpiDesc->Guid->Data1 &= H2O_64_HIGH_BIT_MASK;
  H2OBoardIdsSetPpiDesc->Guid->Data1 += (UINT32) BoardId;
  Status = PeiServicesInstallPpi (H2OBoardIdsSetPpiDesc);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

