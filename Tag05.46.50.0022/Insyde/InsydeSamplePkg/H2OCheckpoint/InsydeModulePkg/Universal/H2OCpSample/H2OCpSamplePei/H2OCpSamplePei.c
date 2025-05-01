/** @file
  Sample code to add a new PEI checkpoint

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

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
//[-start-H2OCpSamplePei-add]//
#include <Library/H2OCpLib.h>

#include <Guid/H2OCp.h>
//[-end-H2OCpSamplePei-add]//


#define TEST_COUNT_DEFAULT        0x02
#define TEST_COUNT_UPDATE         0x05

//[-start-H2OCpSamplePei-add]//
/**
 Sample PEI chekcpoint callback function

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

  H2O_PEI_CP_XXX_YYY_ZZZ_DATA             *PeiCpXxxYyyZzzData;
  EFI_STATUS                              Status;
  EFI_GUID                                PeiCpXxxYyyZzzGuid;

  //
  // Get checkpoint data by H2OCpLookup() if need use checkpoint data in callback function
  //
  Status = H2OCpLookup (Handle, (VOID **) &PeiCpXxxYyyZzzData, &PeiCpXxxYyyZzzGuid);
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
  PeiCpXxxYyyZzzData->Status    = H2O_CP_TASK_UPDATE;
  PeiCpXxxYyyZzzData->TestCount = TEST_COUNT_UPDATE;
  DEBUG_CP ((EFI_D_INFO, "Checkpoint Data Updated: %g\n", &PeiCpXxxYyyZzzGuid));
  DEBUG_CP ((EFI_D_INFO, "   %a\n", __FUNCTION__));

}
/**
  Entry point of sample code to register and trigger PEI checkpoint.

 @param[in] FfsHeader          Header for file of Firmware File System
 @param[in] PeiServices        The PEI core services table.

 @retval EFI_SUCCESS            Initialize board identifier successfully.
 @retval EFI_OUT_OF_RESOURCES   Fail to allocate memory.
**/
EFI_STATUS
EFIAPI
H2OCpSamplePeiEntryPoint (
  IN EFI_PEI_FILE_HANDLE          FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  UINTN                 TestCount;
  UINTN                 Status;
  H2O_CP_HANDLE         Handle;

  //
  // Below is example to register a H2O_CP_MEDIUM priority checkpoint
  //
  if (FeaturePcdGet (PcdH2OPeiCpXxxYyyZzzSupported)) {
    Status = H2OCpRegisterHandler (
               &gH2OPeiCpXxxYyyZzzGuid,
               XxxYyyZzzCallback,
               H2O_CP_MEDIUM,
               &Handle
               );
      if (EFI_ERROR (Status)) {
        DEBUG_CP ((DEBUG_ERROR, "Checkpoint Register Fail: %g (%r)\n", &gH2OPeiCpXxxYyyZzzGuid, Status));
        return Status;
      }
      DEBUG_CP ((DEBUG_INFO, "Checkpoint Registered: %g (%r)\n", &gH2OPeiCpXxxYyyZzzGuid, Status));
  }

  TestCount = TEST_COUNT_DEFAULT;

  //
  // Below is example to trigger checkpoint.
  //
  if (FeaturePcdGet (PcdH2OPeiCpXxxYyyZzzSupported)) {
    H2O_PEI_CP_XXX_YYY_ZZZ_DATA             PeiCpXxxYyyZzzData;

    PeiCpXxxYyyZzzData.Size                 = sizeof (H2O_PEI_CP_XXX_YYY_ZZZ_DATA);
    PeiCpXxxYyyZzzData.Status               = H2O_CP_TASK_NORMAL;
    //
    // Need initialize other member if have
    //
    PeiCpXxxYyyZzzData.TestCount            = TestCount;
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Trigger: %g\n", &gH2OPeiCpXxxYyyZzzGuid));
    H2OCpTrigger (&gH2OPeiCpXxxYyyZzzGuid, &PeiCpXxxYyyZzzData);
    DEBUG_CP ((DEBUG_INFO, "Checkpoint Result: %x\n", PeiCpXxxYyyZzzData.Status));

    //
    // Only need add below code if Status member will be changed in callback function.
    //
    if (PeiCpXxxYyyZzzData.Status == H2O_CP_TASK_SKIP) {
      goto XxxYyyZzzSkip;
    } else if (PeiCpXxxYyyZzzData.Status == H2O_CP_TASK_UPDATE) {
      //
      // Get updated member if return status is H2O_CP_TASK_SKIP
      //
      TestCount = PeiCpXxxYyyZzzData.TestCount ;
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
//[-end-H2OCpSamplePei-add]//