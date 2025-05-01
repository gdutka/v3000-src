/** @file
  This driver provides IHISI interface in SMM mode

;******************************************************************************
;* Copyright (c) 2017 - 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "Fbts.h"
#include <Library/VariableLib.h>

#include <Protocol/SmmVariable.h>
#include <Protocol/H2OIhisiFbtsToolVersionProtocol.h>

#include <Library/IoLib.h>
#include <Library/SmmOemSvcKernelLib.h>

#include <Guid/DebugMask.h>
#include <Guid/AmdMemoryContextSaveRestoreData.h>
#include <Guid/AmdMemoryContextSaveRestoreVariable.h>
#include "IhisiSmm.h"

#define EFI_SYSTEM_NV_DATA_HOB_GUID \
  {0xd6e5092d, 0xc7b2, 0x4872, {0xaf, 0x66, 0xfd, 0xc0, 0xe6, 0xf9, 0x5e, 0x78}}

#define FBTSAPHOOK_SUPPORT        204

STATIC
IHISI_REGISTER_TABLE
CHIPSET_FBTS_REGISTER_TABLE[] = {
  //
  // AH=17h
  //
  { FBTSGetRomFileAndPlatformTable, "S17Cs_GetPlatformTb", GetRomFileAndPlatformTable}, \

  //
  // AH=1Ch
  //
  { FBTSGetATpInformation, "S1CCs_GetATpInfo000", FbtsGetATpInformation}, \

  //
  // AH=1Fh
  //
  { FBTSApHookPoint, "S1FCs_ApHookForBios", FbtsApHookForBios}


};

/**
  Check the flash region whether is used.

  @param[in] FlashRegion  Flash Region x Register (x = 0 - 3).

  @retval TRUE            This  region is used
  @retval FALSE           This  region is not used
**/
STATIC
BOOLEAN
CheckFlashRegionIsValid (
  IN UINT32   FlashRegion
  )
{
  BOOLEAN         Flag = TRUE;

  //
  // the Region Base must be programmed to 0x1FFFh and the Region Limit
  // to 0x0000h to disable the region.
  //
  // Now, the tool programmed 0x0fff to base and 0x000 to limit to indicate
  // this is region is not used.
  //

  //
  //The limit block address must be greater than or equal to the base block address
  //
  if ((FlashRegion & 0x1fff) > (FlashRegion >> 16 & 0x1fff)) {
    Flag = FALSE;
  }

  return Flag;
}

/**
  Get Flash table from Rom file.
  if DescriptorMode is true, the FlashTable will be filled.
  if the descriptor is false,the FlashTable will be filled RegionTypeEos(0xff) directly.

  @param[in]  InputDataBuffer    the pointer to Rom file.
  @param[in]  DataBuffer         IN: the input buffer address.
                                 OUT:the flash region table from rom file.
  @param[in]  DescriptorMode     TRUE is indicate this Rom file is descriptor mode
                                 FALSE is indicate this Rom file is non-descriptor mode

  @retval EFI_SUCCESS            Successfully returns
**/
EFI_STATUS
GetRomFileFlashTable (
  IN       UINT8           *InputDataBuffer,
  IN OUT   FLASH_REGION    *DataBuffer,
  IN       BOOLEAN         DescriptorMode
  )
{
  UINT8           Index;
  UINT32          *FlashRegionPtr;
  UINT32          Frba;
  UINT32          *FlashMasterPtr;
  UINT32          Fmba;
  UINT32          FlashMap0Reg;
  UINT32          FlashMap1Reg;
  UINT32          ReadAccess;
  UINT32          WriteAccess;

  //
  //calulate Flash region base address
  //
  FlashMap0Reg = *((UINT32 *)(InputDataBuffer + FLASH_MAP_0_OFFSET));
  Frba = (FlashMap0Reg & FLASH_REGION_BASE_MASK) >> 12;
  FlashRegionPtr = (UINT32 *)(InputDataBuffer + Frba);
  //
  //calulate Flash master base address
  //

  FlashMap1Reg = *((UINT32 *)(InputDataBuffer + FLASH_MAP_1_OFFSET));

  Fmba = (FlashMap1Reg & FLASH_MASTER_BASE_MASK) << 4;
  FlashMasterPtr = (UINT32 *)(InputDataBuffer + Fmba);

  if (DescriptorMode) {
    for (Index = DESC_REGION; Index < MAX_FLASH_REGION; Index++,  FlashRegionPtr++) {

      if (DataBuffer->Type == FLASH_REGION_TYPE_OF_EOS){
        break;
      }

      if (CheckFlashRegionIsValid (*FlashRegionPtr)){

        DataBuffer->Type = Index;
        DataBuffer->Offset = (*FlashRegionPtr & 0x1fff) << 12;
        DataBuffer->Size = ((*FlashRegionPtr >> 16 & 0x1fff) - (*FlashRegionPtr & 0x1fff) + 1) << 12;
        //
        //Bios primary master always has access permissions to it's primary region
        //
        if (Index == BIOS_REGION) {
          ReadAccess = ACCESS_AVAILABLE;
          WriteAccess = ACCESS_AVAILABLE;
        } else if (Index < MAX_FLASH_REGION) {
          ReadAccess = (*FlashMasterPtr >> (FLASH_MASTER_1_READ_ACCESS_BIT + Index)) & ACCESS_AVAILABLE;
          WriteAccess = (*FlashMasterPtr >> (FLASH_MASTER_1_WRITE_ACCESS_BIT + Index)) & ACCESS_AVAILABLE;
        } else {
          ReadAccess = ACCESS_AVAILABLE;
          WriteAccess = ACCESS_AVAILABLE;
        }

        if (ReadAccess == ACCESS_AVAILABLE && WriteAccess == ACCESS_AVAILABLE) {
          DataBuffer->Access = ACCESS_AVAILABLE;
        } else {
          DataBuffer->Access = ACCESS_NOT_AVAILABLE;
        }
        DataBuffer++;
      }
    }
  }
  DataBuffer->Type = REGION_TYPE_OF_EOS;
  return EFI_SUCCESS;
}

/**
  AH = 1Ch
  Passing information to flash program on whether
  if current BIOS (AT-p capable) can be flashed with
  other BIOS ROM file
  AT-p: (Anti-Theft PC Protection).

  @retval EFI_SUCCESS   Success returns.
**/
EFI_STATUS
FbtsGetATpInformation (
  VOID
  )
{
  UINT32                      Ecx_data = 0;

  //
  // Return ECX for tool to determin proper error message.
  // Please refer to IHISI spec.
  //
  mH2OIhisi->WriteCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RCX, Ecx_data);

  return EFI_SUCCESS;
}

/**
  AH = 17h , Get platform and Rom file flash descriptor region.

  @retval EFI_SUCCESS   Success returns.
**/
EFI_STATUS
GetRomFileAndPlatformTable (
  VOID
  )
{
  EFI_STATUS      Status;
  UINTN           DataSize;
  FLASH_REGION   *FlashRegionPtr;
  UINT8          *InputDataBuffer;
  UINT8          *OutputDataBuffer;
//[-start-220513-IB14740241-add]//
  UINT8          *MaxOutputDataBuffer;
  UINT8           Index;
//[-end-220513-IB14740241-add]//

  Status = EFI_UNSUPPORTED;
//[-start-220513-IB14740241-add]//
  Index = 0;
//[-end-220513-IB14740241-add]//
  DataSize = (UINTN) mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RCX);
  if (DataSize == 0) {
    return Status;
  }
  OutputDataBuffer = (UINT8 *)(UINTN) mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RDI);

  InputDataBuffer  = (UINT8 *)(UINTN) mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RSI);
  if (!mH2OIhisi->BufferInCmdBuffer ((VOID *) OutputDataBuffer, sizeof(FBTS_REGION_INFORMATION_STRUCTURE)) ||
      !mH2OIhisi->BufferInCmdBuffer ((VOID *) InputDataBuffer, DataSize)) {
    return IHISI_INVALID_PARAMETER;
  }

//[-start-220513-IB14740241-modify]//
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    (sizeof (FLASH_REGION) * (TOTAL_FLASH_REGION)),
                    (VOID **)&MaxOutputDataBuffer
                    );

  if (!EFI_ERROR (Status)) {
    //
    //we must get platform region table through read chipset register, because
    //1. we can override access permissions in the Flash Descriptor through BMWAG and BMRAG
    //2. Flash regions may be haven't read/write access.
    //
    Status = mSmmFwBlockService->GetFlashTable (
                                   mSmmFwBlockService,
                                   MaxOutputDataBuffer
                                   );

    //
    // Calculate actually Flash table numbers
    //
    FlashRegionPtr = (FLASH_REGION *) MaxOutputDataBuffer;
    while (FlashRegionPtr->Type != REGION_TYPE_OF_EOS) {
      Index ++;
      FlashRegionPtr ++;
    }

    if (ROM_FILE_REGION_OFFSET < sizeof (FLASH_REGION) * (Index + 1)) {
      return IHISI_OB_LEN_TOO_SMALL;
    }

    CopyMem ((VOID *)OutputDataBuffer, (VOID *)MaxOutputDataBuffer, sizeof (FLASH_REGION) * (Index + 1));
    gSmst->SmmFreePool (MaxOutputDataBuffer);

    OutputDataBuffer += ROM_FILE_REGION_OFFSET;
    FlashRegionPtr = (FLASH_REGION *) OutputDataBuffer;

    //
    //check Rom file is whether descriptor mode
    //
    if (*((UINT32 *)InputDataBuffer) == FLASH_VALID_SIGNATURE) {
      Status = GetRomFileFlashTable (InputDataBuffer, FlashRegionPtr, TRUE);
    } else {
      Status = GetRomFileFlashTable (InputDataBuffer, FlashRegionPtr, FALSE);
    }
  }
//[-end-220513-IB14740241-modify]//

  return Status;
}
/**
  This call back function will be invoked several times during flash process.
  BIOS can know which step is running now.
  BIOS can base on it to do specific hook such as EC idle and weak up.

       CL - denote the start of AP process.
       CL = 0x00, AP terminate. (Before IHISI 0x16)
       CL = 0x01, AP start. (After IHISI 0x10)
       CL = 0x02, Start to read ROM. (Before IHISI 0x14)
       CL = 0x03, Start to write ROM. (Before IHISI 0x15)
       CL = 0x04, Start to write EC. (Before IHISI 0x20)
       CL = 0x05, Before dialog popup.
       CL = 0x06, After dialog close and continue running.

  @param

  @retval EFI_SUCCESS
**/
EFI_STATUS
FbtsApHookForBios (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT8                                 ApState;
  UINT32                                Eax;
  H2O_IHISI_FBTS_TOOL_VERSION_PROTOCOL  *ToolVersionProtocol = NULL;
  UINT16                                IhisiVersion;

  Status = gSmst->SmmLocateProtocol (
                    &gH2OIhisiFbtsToolVersionProtocolGuid,
                    NULL,
                    (VOID**)&ToolVersionProtocol
                    );

  if (EFI_ERROR (Status)) {
    IhisiVersion = 0;
  } else {
    IhisiVersion = ToolVersionProtocol->IhisiVersion;
  }

  if (IhisiVersion < FBTSAPHOOK_SUPPORT) {
    //
    // Return AL for funtion success
    //
    Eax = mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RAX);
    Eax = (UINT32) ((Eax & 0xffffff00) | (UINT32) IhisiSuccess);
    mH2OIhisi->WriteCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RAX, Eax);
    return EFI_SUCCESS;
  }
  ApState = (UINT8) (mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RCX));  // CL

  switch (ApState) {
    case 0x00:
      //
      // Here provided a hook before AP terminate.
      //
      if (PcdGetBool (PcdH2OEcSharedFlashSupported) && !PcdGetBool (PcdH2OEcIdlePerWriteBlockSupported)) {
        OemSvcEcIdle (FALSE);
      }
      break;

    case 0x01:
      //
      // Here provided a hook before AP start.
      //

      break;

    case 0x02:
      //
      // Here provided a hook before FbtsRead.
      //
      if (PcdGetBool (PcdH2OEcSharedFlashSupported) && !PcdGetBool (PcdH2OEcIdlePerWriteBlockSupported)) {
        OemSvcEcIdle (TRUE);
      }
      break;

    case 0x03:
      //
      // Here provided a hook before FbtsWrite.
      //
      if (PcdGetBool (PcdH2OEcSharedFlashSupported) && !PcdGetBool (PcdH2OEcIdlePerWriteBlockSupported)) {
        OemSvcEcIdle (TRUE);
      }
      break;

    case 0x04:
      //
      // Here provided a hook before FetsWrite.
      //
      if (PcdGetBool (PcdH2OEcSharedFlashSupported) && !PcdGetBool (PcdH2OEcIdlePerWriteBlockSupported)) {
        OemSvcEcIdle (TRUE);
      }
      break;

    case 0x05:
      //
      // Here provided a hook before dialog popup. (Winflash)
      //
      if (PcdGetBool (PcdH2OEcSharedFlashSupported) && !PcdGetBool (PcdH2OEcIdlePerWriteBlockSupported)) {
        OemSvcEcIdle (FALSE);
      }
      break;

    case 0x06:
      //
      // Here provided a hook after dialog close and continue running. (Winflash)
      //

      break;
  }

  //
  // Return AL for funtion success
  //

  Eax = mH2OIhisi->ReadCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RAX);
  Eax = (UINT32) ((Eax & 0xffffff00) | (UINT32) IhisiSuccess);
  mH2OIhisi->WriteCpuReg32 (EFI_SMM_SAVE_STATE_REGISTER_RAX, Eax);

  Status = OemSvcIhisiS1FHookFbtsApHookForBios (ApState);
  DEBUG_OEM_SVC ((EFI_D_ERROR | EFI_D_INFO, "SmmOemSvcChipsetLib OemSvcIhisiS1FHookFbtsApHookForBios, Status : %r\n", Status));

  return EFI_SUCCESS;

}

/**
  Initialize Fbts relative services

  @retval EFI_SUCCESS        Initialize Fbts services successful.
  @return Others             Any error occurred.
**/
EFI_STATUS
FbtsInit (
  VOID
  )
{
  EFI_STATUS   Status;
  IHISI_REGISTER_TABLE   *SubFuncTable;
  UINT16                  TableCount;

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmFwBlockServiceProtocolGuid,
                    NULL,
                    (VOID **) &mSmmFwBlockService
                    );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  SubFuncTable = CHIPSET_FBTS_REGISTER_TABLE;
  TableCount = sizeof(CHIPSET_FBTS_REGISTER_TABLE)/sizeof(CHIPSET_FBTS_REGISTER_TABLE[0]);
  Status = RegisterIhisiSubFunction (SubFuncTable, TableCount);
  if (EFI_ERROR(Status)) {
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

