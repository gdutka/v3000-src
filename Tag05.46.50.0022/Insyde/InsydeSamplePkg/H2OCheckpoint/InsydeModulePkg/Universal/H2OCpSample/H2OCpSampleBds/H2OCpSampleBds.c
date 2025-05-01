/** @file
  Sample code to add a new BDS checkpoint

;******************************************************************************
;* Copyright (c) 2017 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiDxe.h>
//
// The Library classes this module consumes
//
#include <Library/UefiLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
//[-start-H2OCpSampleBds-add]//
#include <Library/H2OCpLib.h>

#include <Guid/H2OCp.h>
//[-end-H2OCpSampleBds-add]//


#define TEST_COUNT_DEFAULT        0x02
#define TEST_COUNT_UPDATE         0x05

//[-start-H2OCpSampleBds-add]//
/**
 Sample BDS chekcpoint callback function

 @param[in] Event          A pointer to the Event that triggered the callback.
 @param[in] Handle         Checkpoint handle.
**/
VOID
EFIAPI
XxxYyyZzzCallback (
  IN EFI_EVENT                        Event,
  IN H2O_CP_HANDLE                    Handle
  )
{

  H2O_BDS_CP_XXX_YYY_ZZZ_DATA             *BdsCpXxxYyyZzzData;
  EFI_STATUS                              Status;
  EFI_GUID                                BdsCpXxxYyyZzzGuid;

  //
  // Get checkpoint data by H2OCpLookup() if need use checkpoint data in callback function
  //
  Status = H2OCpLookup (Handle, (VOID **) &BdsCpXxxYyyZzzData, &BdsCpXxxYyyZzzGuid);
  if (EFI_ERROR (Status)) {
    DEBUG_CP ((DEBUG_ERROR, "Checkpoint Data Not Found: %x (%r)\n", Handle, Status));
    DEBUG_CP ((DEBUG_ERROR, "   %a\n", __FUNCTION__));
    return;
  }
  //
  // Unregister checkpoint if need.
  //
  H2OCpUnregisterHandler (Handle);

  //
  // some code to process checkpoint task
  //


  //
  // Updating checkpoint data if need
  //
  BdsCpXxxYyyZzzData->Status    = H2O_CP_TASK_UPDATE;
  BdsCpXxxYyyZzzData->TestCount = TEST_COUNT_UPDATE;
  DEBUG_CP ((EFI_D_INFO, "Checkpoint Data Updated: %g\n", &BdsCpXxxYyyZzzGuid));
  DEBUG_CP ((EFI_D_INFO, "   %a\n", __FUNCTION__));

}

/**
  Entry point of sample code to register and trigger BDS checkpoint.

  @param ImageHandle            A handle for the image that is initializing this driver
  @param SystemTable            A pointer to the EFI system table

  @retval EFI_SUCCESS:          Module initialized successfully
  @retval Others                Module initialized unsuccessfully
**/
EFI_STATUS
EFIAPI
H2OCpSampleBdsEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  UINTN                 TestCount;
  UINTN                 Status;
  H2O_CP_HANDLE         Handle;

  //
  // Below is example to register a H2O_CP_MEDIUM priority checkpoint
  //
  if (FeaturePcdGet (PcdH2OBdsCpXxxYyyZzzSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OBdsCpXxxYyyZzzGuid,
               XxxYyyZzzCallback,
               H2O_CP_MEDIUM,
               &Handle
               );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OBdsCpXxxYyyZzzGuid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OBdsCpXxxYyyZzzGuid, Status));
  }

  TestCount = TEST_COUNT_DEFAULT;

  //
  // Below is example to trigger checkpoint.
  //
  if (FeaturePcdGet (PcdH2OBdsCpXxxYyyZzzSupported)) {
    H2O_BDS_CP_XXX_YYY_ZZZ_DATA             BdsCpXxxYyyZzzData;

    BdsCpXxxYyyZzzData.Size                 = sizeof (H2O_BDS_CP_XXX_YYY_ZZZ_DATA);
    BdsCpXxxYyyZzzData.Status               = H2O_CP_TASK_NORMAL;
    //
    // Need initialize other member if have
    //
    BdsCpXxxYyyZzzData.TestCount            = TestCount;
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OBdsCpXxxYyyZzzGuid));
    H2OCpTrigger (&gH2OBdsCpXxxYyyZzzGuid, &BdsCpXxxYyyZzzData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", BdsCpXxxYyyZzzData.Status));

    //
    // Only need add below code if Status member will be changed in callback function.
    //
    if (BdsCpXxxYyyZzzData.Status == H2O_CP_TASK_SKIP) {
      goto XxxYyyZzzSkip;
    } else if (BdsCpXxxYyyZzzData.Status == H2O_CP_TASK_UPDATE) {
      //
      // Get updated member if return status is H2O_CP_TASK_SKIP
      //
      TestCount = BdsCpXxxYyyZzzData.TestCount ;
    }
  }

  //
  // Here is some code need skip if return status is H2O_CP_TASK_SKIP
  //


XxxYyyZzzSkip:
  //
  // code need execute no matter return status is H2O_CP_TASK_SKIP or not.
  //

  return EFI_SUCCESS;

}
//[-end-H2OCpSampleBds-add]//