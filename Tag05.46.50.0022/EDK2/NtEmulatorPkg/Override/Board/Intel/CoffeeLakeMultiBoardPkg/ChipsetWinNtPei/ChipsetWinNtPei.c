/** @file
  WinNt driver

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#include <PiPei.h>
#include <Library/PeimEntryPoint.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#define __HOB__H__
//#include <Guid/XTUInfoHob.h>
#include <MeBiosPayloadHob.h>
#include <Library/PcdLib.h>
#include <Ppi/H2OBoardIdSet.h>
#include <Library/PeiServicesLib.h>

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiH2OBoardIdSet = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gH2OBoardIdSetPpiGuid,
  NULL
};

/**
  Initialize the state information for ChipsetWinNtDriver

  @param[in] ImageHandle          Image handle of the loaded driver
  @param[in] SystemTable          Pointer to the System Table

  @retval EFI_SUCCESS             Thread can be successfully created
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate protocol data structure
  @retval EFI_DEVICE_ERROR        Cannot create the timer service
**/
EFI_STATUS
EFIAPI
ChipsetWinNtPeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
//  XTU_INFO_DATA                 *XtuInfo;
  ME_BIOS_PAYLOAD               *MeBiosPayLoad;

//  XtuInfo = (XTU_INFO_DATA *) BuildGuidHob (&gXTUInfoHobGuid, sizeof (XTU_INFO_DATA));
//  ZeroMem (XtuInfo, sizeof (XTU_INFO_DATA));

  //typedef struct _ME_BIOS_PAYLOAD_HOB {
  //EFI_HOB_GUID_TYPE Header;
  //UINT8             Revision;
  //ME_BIOS_PAYLOAD   MeBiosPayload;
  //} ME_BIOS_PAYLOAD_HOB;
  MeBiosPayLoad = (ME_BIOS_PAYLOAD *) BuildGuidHob (&gMeBiosPayloadHobGuid, sizeof(UINT8) + sizeof (ME_BIOS_PAYLOAD));
  ZeroMem (MeBiosPayLoad, sizeof(UINT8) + sizeof (ME_BIOS_PAYLOAD));


  PcdSet64S (PcdH2OBoardId, 0);

  PeiServicesInstallPpi (&mPpiH2OBoardIdSet);

  return EFI_SUCCESS;
}
