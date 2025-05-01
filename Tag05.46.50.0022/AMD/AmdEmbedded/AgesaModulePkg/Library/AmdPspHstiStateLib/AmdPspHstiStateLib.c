/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Library/AmdBaseLib.h>
#include <Library/AmdPspMboxLibV2.h>
#include <Library/AmdPspHstiStateLib.h>
#include <Library/AmdHeapLib.h>
#include <Filecode.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_AMDPSPHSTISTATELIB_AMDPSPHSTISTATELIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                            L O C A L   F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                                 F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 * @brief initialize HSTI state to save it to Heap for later use
 * this function must be called in non-SMM mode before we call GetHstiState
 *
 */
EFI_STATUS
EFIAPI
InitializeHstiState (
  )
{
  UINT32                      *HstiStateBuffer;
  UINT32                      MailboxHSTIState;

  if (GetHeapBufferByHandle (AMD_PSP_HSTI_STATE_BUFFER_HANDLE, sizeof (UINT32), (UINT8 **)&HstiStateBuffer) != TRUE) {
     return EFI_DEVICE_ERROR;
  }

  MailboxHSTIState = 0;
  if (PspMboxBiosQueryHSTIState (&MailboxHSTIState) == EFI_SUCCESS) {
    *HstiStateBuffer = MailboxHSTIState;
  }
  return EFI_SUCCESS;
}

/**
 * @brief Get the HSTI State
 * This function returns HSTI state, can be used in both non-SMM and SMM mode,
 * but InitializeHstiState should be called in non-SMM first so that we can get HSTI state from heap
 */
EFI_STATUS
EFIAPI
GetHstiState (
  UINT32   *HstiState
  )
{
  UINT32                      *HstiStateBuffer;
  if (GetHeapBufferByHandle (AMD_PSP_HSTI_STATE_BUFFER_HANDLE, sizeof (UINT32), (UINT8 **)&HstiStateBuffer) != TRUE) {
     return EFI_DEVICE_ERROR;
  }
  *HstiState = *HstiStateBuffer;
  return EFI_SUCCESS;
}
