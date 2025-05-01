/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Porting.h"
#include <Library/PcdLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdPspDxeSmmBufLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SmmBase2.h>

#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE        LIBRARY_AMDPSPDXESMMBUFLIB_AMDPSPDXESMMBUFLIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

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
//[-start-240520-IB20840057-modify]//
STATIC BOOLEAN                 mInSmm                          = FALSE;
STATIC EFI_SMM_SYSTEM_TABLE2   *mSmst                          = NULL;
//[-end-240520-IB20840057-modify]//
STATIC UINT8                   *mAmdPspSmmRunTimeBufferAddress = NULL;

/**
 * @brief Get Amd Psp Smm/Run Time Buffer Address
 *
 * @param   VOID
 *
 * @return  UINT8* Pointer
 */
UINT8 *
GetAmdPspSmmRunTimeBufferAddress (
  VOID
  )
{
  if (mAmdPspSmmRunTimeBufferAddress == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] mAmdPspSmmRunTimeBufferAddress is NULL\n", \
      __FUNCTION__);
  }

  return mAmdPspSmmRunTimeBufferAddress;
}

/**
 * @brief ApcbVariableLibV3Constructor
 *
 * @param[in] EFI_HANDLE        ImageHandle
 * @param[in] EFI_SYSTEM_TABLE  *SystemTable

 * @return  EFI_STATUS     EFI_SUCCESS       Success
 *                         Other Status      Fail
 */
EFI_STATUS
EFIAPI
AmdPspDxeSmmBufLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS              Status;
  UINT8                   *pAmdPspSmmRunTimeBufferAddress;
  EFI_SMM_BASE2_PROTOCOL  *SmmBase2;

  Status                         = EFI_SUCCESS;
  pAmdPspSmmRunTimeBufferAddress = NULL;
  mInSmm                         = FALSE;

  IDS_HDT_CONSOLE_PSP_TRACE ("[%a] Start ...\n", __FUNCTION__);

  Status = gBS->LocateProtocol (
                &gEfiSmmBase2ProtocolGuid,
                NULL,
                (VOID**) &SmmBase2
                );
  if (!EFI_ERROR (Status)) {
    SmmBase2->InSmm (SmmBase2, &mInSmm);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] gEfiSmmBase2ProtocolGuid Locate Fail\n", \
      __FUNCTION__);
  }

  if (!mInSmm) {
    return EFI_SUCCESS;
  }

  //
  // We are now in SMM, and get SMM Table Base
  //
  Status = SmmBase2->GetSmstLocation (SmmBase2, &mSmst);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] GetSmstLocation Fail\n", __FUNCTION__);
    return EFI_SUCCESS;
  }

  if (PcdGet64 (PcdAmdPspSmmRunTimeBufferAddress) == 0) {
    Status = mSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      AMD_PSP_SMM_TMP_BUFFER_SIZE,
                      (VOID **)&pAmdPspSmmRunTimeBufferAddress
                      );
    if (EFI_ERROR(Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("[%a] SmmAllocatePool Fail, Status: %r\n", \
        __FUNCTION__, Status);
      ASSERT (FALSE);
      return EFI_SUCCESS;
    }

    if (pAmdPspSmmRunTimeBufferAddress == NULL) {
      IDS_HDT_CONSOLE_PSP_TRACE ("[%a] SmmAllocatePool Address is NULL\n", \
        __FUNCTION__);
      ASSERT (FALSE);
      return EFI_SUCCESS;
    }

    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] SmmAllocatePool Address: 0x%x\n", \
      __FUNCTION__, pAmdPspSmmRunTimeBufferAddress);

    ZeroMem (pAmdPspSmmRunTimeBufferAddress, AMD_PSP_SMM_TMP_BUFFER_SIZE);
    PcdSet64S (PcdAmdPspSmmRunTimeBufferAddress, (UINT64) (UINTN) pAmdPspSmmRunTimeBufferAddress);
  }

  mAmdPspSmmRunTimeBufferAddress = (UINT8 *) (UINTN) PcdGet64 (PcdAmdPspSmmRunTimeBufferAddress);

  if (mAmdPspSmmRunTimeBufferAddress == NULL) {
    IDS_HDT_CONSOLE_PSP_TRACE ("[%a] mAmdPspSmmRunTimeBufferAddress is NULL\n", \
      __FUNCTION__);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("[%a] Exit ...\n", __FUNCTION__);

  return EFI_SUCCESS;
}
