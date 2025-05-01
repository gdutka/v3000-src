/** @file

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <PiPei.h>
#include <AmdCpmPei.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/GpioCfgLib.h>
#include <Library/PcdLib.h>


EFI_STATUS
EFIAPI
GpioTableOverrideNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mGpioTableOverrideNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmTablePpiGuid,
  GpioTableOverrideNotifyCallback
};

/**
 * Callback Function to Override Gpio Settings By Chipset Definition Tables
 *
 * @param[in]     PeiServices       The PEI core services table.
 * @param[in]     NotifyDescriptor  The descriptor for the notification event.
 * @param[in]     Ppi               Pointer to the PPI in question
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
GpioTableOverrideNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  EFI_STATUS                    Status;
  AMD_CPM_TABLE_PPI             *AmdCpmTablePpi;
  AMD_CPM_GPIO_INIT_TABLE       *GpioInitTablePtr;
  H2O_BOARD_ID                  Board;
  H2O_GPIO_INIT_CONFIG          *GpioInitStruct;
  UINT32                        GpioInitStructCount;
  UINT32                        Index;
  //UINT32                        PlatformMask;                       

  DEBUG ((DEBUG_INFO, "GpioCfg Notified\n"));
  
  
  Status = (*PeiServices)->LocatePpi (
                           (CONST EFI_PEI_SERVICES**)PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           (VOID**)&AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "LocatePpi-gAmdCpmTablePpiGuid-Status=%r\n", Status));
    return Status;
  }
  // when PcdH2OGpioCfgSupported is true , *.gpio overwrite gCpmGpioInitTable , you need to modify *.gpio.
  // PcdH2OGpioCfgSupported default is false 

  if (PcdGetBool (PcdH2OGpioCfgSupported)) {

    DEBUG((DEBUG_INFO, "PcdH2OGpioCfgSupported : TURE\n"));
     
    GpioInitTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_INIT);
    //DEBUG((DEBUG_INFO, "PlatformMask:0x%x\n",PlatformMask));
    Board = 1;
    //
    // Use GetGpioCfg in GpioCfgLib to get build-time GPIO configuration
    // Input Board value corresponds to Skuids in Project.dsc
    //
    Status = GetGpioCfg (Board, &GpioInitStruct, &GpioInitStructCount);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "ERROR GetGpioCfg\n"));
      return EFI_NOT_FOUND;
    }
    Index = 0;
    while (Index != GpioInitStructCount) {
      DEBUG((DEBUG_INFO, "AmdGpioInit[%d]:0x%x\n", Index, GpioInitTablePtr->GpioList[Index]));
      DEBUG((DEBUG_INFO, "InsydeGpioInit[%d]:0x%x\n", Index, GpioInitStruct[Index].GpioList));
      GpioInitTablePtr->GpioList[Index] = GpioInitStruct[Index].GpioList;
      DEBUG((DEBUG_INFO, "After AmdGpioInit[%d]:0x%x\n", Index, GpioInitTablePtr->GpioList[Index]));
      Index++;
    }
    // DEBUG ((DEBUG_INFO, "OemKernelServices Call: OemSvcUpdateGpioCfg \n"));.
    // Status = OemSvcUpdateGpioCfg (&SingleGpioInitStruct);
    // DEBUG ((DEBUG_INFO, "OemKernelServices OemSvcUpdateGpioCfg Status: %r\n", Status));
  } else {
    DEBUG((DEBUG_INFO, "PcdH2OGpioCfgSupported : FALSE\n"));
  }
  return Status;
}


EFI_STATUS
GpioCfgPeiEntry (
  IN EFI_PEI_FILE_HANDLE             FileHandle,
  IN CONST EFI_PEI_SERVICES        **PeiServices
  )
{
  EFI_STATUS                    Status;
  DEBUG ((DEBUG_INFO, "GpioTableOverrideNotifyCallback-NotifyPpi-gAmdCpmTablePpiGuid\n"));
  Status = (**PeiServices).NotifyPpi (PeiServices, &mGpioTableOverrideNotify);


  return EFI_SUCCESS;
}

