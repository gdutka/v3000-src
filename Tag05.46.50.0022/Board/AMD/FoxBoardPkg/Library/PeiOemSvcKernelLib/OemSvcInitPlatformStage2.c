/** @file
  CPU/Chipset/Platform Initial depends on project characteristic.

;******************************************************************************
;* Copyright (c) 2012, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/PeiOemSvcKernelLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/AmdCpmTablePpi/AmdCpmTablePpi.h>
#include "SetupConfig.h"
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#define ECRAMxA8                    0xA8
#define SMBUS1_DEV_MUX_SW           BIT1       // ECRAMxA8[1]
/**
  CPU/Chipset/Platform Initial depends on project characteristic.

  @param[in]  *Buffer               A pointer to SYSTEM_CONFIGURATION.
  @param[in]  SetupVariableExist    Setup variable be found in variable storage or not.

  @retval     EFI_UNSUPPORTED       Returns unsupported by default.
  @retval     EFI_SUCCESS           The service is customized in the project.
  @retval     EFI_MEDIA_CHANGED     The value of IN OUT parameter is changed. 
  @retval     Others                Depends on customization.
**/
EFI_STATUS
OemSvcInitPlatformStage2 (
  IN  VOID                                 *Buffer,
  IN  BOOLEAN                              SetupVariableExist
  )
{
  SYSTEM_CONFIGURATION                  *SystemConfiguration = NULL;
  EFI_STATUS                            Status;
  AMD_CPM_TABLE_PPI                     *AmdCpmTablePpi;
  UINT8                                 Value8;
  UINT8                                 Offset;  

  DEBUG ((EFI_D_INFO, "OemSvcInitPlatformStage2, SetupVariableExist:%x\n"));
  
  if (SetupVariableExist) {
    DEBUG ((EFI_D_INFO, "SetupVariableExist = TRUE.\n"));
    SystemConfiguration = (SYSTEM_CONFIGURATION *)Buffer;
  }
    
  Status = PeiServicesLocatePpi (
             &gAmdCpmTablePpiGuid,
             0,
             NULL,
             (VOID**)&AmdCpmTablePpi
             );

//[-start-230206-IB14740276-remove]//
//  Status = PcdSet8S (PcdH2OMmioMem64Enable, 0x00);
//  ASSERT_EFI_ERROR (Status);
//[-end-230206-IB14740276-remove]//

  if (!EFI_ERROR(Status)) {
     if (SetupVariableExist) {
       if(SystemConfiguration->DashSupport == 1) {
         DEBUG ((EFI_D_INFO, "Dash Supported, change SMBUS1_DEV#_MUX_SW to high.\n"));
          Offset = ECRAMxA8;
          AmdCpmTablePpi->CommonFunction.KbcRead (0x662, 0x80, &Offset, &Value8);
          DEBUG((DEBUG_INFO, "OemSvcInitPlatformStage1-ECRAMxA8 = 0x%x\n", Value8));
          if ((Value8 & SMBUS1_DEV_MUX_SW) != SMBUS1_DEV_MUX_SW) {
            // Set EC RAM 0xA8[1] to 1'b for the SMBUS1(ASF) connection between APU/NIC
            Value8 |= SMBUS1_DEV_MUX_SW;  
            AmdCpmTablePpi->CommonFunction.EcRamWrite (0x662, 0x81, &Offset, &Value8);
          }
       }
       if (SystemConfiguration->EnableTPM == 1) {
         Status = PcdSet8S (PcdLpcRstS0i3, 0x01);
         ASSERT_EFI_ERROR (Status);
       }
//[-start-230206-IB14740276-remove]//
//>>EnableAbove4GBMmio++
//       if (FeaturePcdGet (Pcd64BitAmlSupported)) {
//         if (SystemConfiguration->EnableAbove4GBMmio == 1) {
//           DEBUG ((EFI_D_INFO, "EnableAbove4GBMmio: TRUE\n"));
//           if (SystemConfiguration->BootType == EFI_BOOT_TYPE) {
//             DEBUG ((EFI_D_INFO, "PcdH2OMmioMem64Enable: TRUE\n"));
//             Status = PcdSet8S (PcdH2OMmioMem64Enable, 0x01);
//             ASSERT_EFI_ERROR (Status);
//           }
//         }
//       }
//<<EnableAbove4GBMmio++
//[-end-230206-IB14740276-remove]//
     } 
  }
  return EFI_SUCCESS;
}

