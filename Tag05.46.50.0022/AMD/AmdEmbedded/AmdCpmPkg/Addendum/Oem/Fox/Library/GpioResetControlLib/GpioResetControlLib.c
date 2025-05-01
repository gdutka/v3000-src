/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <AMD.h>
#include <Library/OemGpioResetControlLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <AmdPcieComplex.h>
#include "../../Pei/AmdCpmOemInitPei/AmdCpmOemInitPeim.h"

#include <Ppi/AmdCpmTablePpi/AmdCpmTablePpi.h>
#include <Ppi/NbioPcieComplexPpi.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------*/
/**
 * Local delay function
 *
 *
 *
 * @param[in]  PeiServices    Pointer to EFI_PEI_SERVICES pointer
 * @param[in]  xus            Microseconds to delay
 *
 */
VOID
STATIC
delay_xus (
  IN  CONST     EFI_PEI_SERVICES   **PeiServices,
  IN  UINT32    xus
  )
{
  EFI_PEI_CPU_IO_PPI  *CpuIo;
  UINTN               uSec;
  UINT8               Data;

  // Issue the reset
  CpuIo = (*PeiServices)->CpuIo;

  //Simplified implementation
  uSec = xus >> 1;
  while (uSec != 0) {
    Data = CpuIo->IoRead8 (
                    PeiServices,
                    CpuIo,
                    (UINT64) 0x80
                    );
    uSec--;
  }
}

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

#define __CFG_CPM_ECRAM_BASE_ADDR   0x662
#define ClearBit(ArgBit)           ((UINT8)~(ArgBit))
#define DELAY_SET                   0
#define FCH_GPIO                    1
#define EC_GPIO                     2

typedef struct {
  UINT8     Type;
  UINT8     IdxEcRam;
  UINT8     AndEcRamData;
  UINT8     OrEcRamData;
} PowerSequenceTbl;

PowerSequenceTbl FibocomL860RPlus_WwanPowerSequence[] =
{
    // Generate a reset sequence
    {DELAY_SET, 30,          0,                                 0}, //DELAY_SET doesn't care AndEcRamData
    {EC_GPIO,   ECRAMxA3,    ClearBit (WWAN_RST_AUX),           0},
    {DELAY_SET, 70,          0,                                 0}, //DELAY_SET doesn't care AndEcRamData
    {EC_GPIO,   ECRAMxA7,    ClearBit (WWAN_MODULE_RST),        0},
    {DELAY_SET, 30,          0,                                 0}, //DELAY_SET doesn't care AndEcRamData
    {EC_GPIO,   ECRAMxAB,    ClearBit (WWAN_CARD_POWER_EN),     0},
    {DELAY_SET, 500,         0,                                 0}, //DELAY_SET doesn't care AndEcRamData
    {EC_GPIO,   ECRAMxAB,    ClearBit (0),                      WWAN_CARD_POWER_EN},
    {DELAY_SET, 30,          0,                                 0}, //DELAY_SET doesn't care AndEcRamData
    {EC_GPIO,   ECRAMxA7,    ClearBit (0),                      WWAN_MODULE_RST},
    {DELAY_SET, 70,          0,                                 0}, //DELAY_SET doesn't care AndEcRamData
    {EC_GPIO,   ECRAMxA3,    ClearBit (0),                      WWAN_RST_AUX},
};

typedef struct {
  UINT8     IdxEcRam;
  UINT8     AndEcRamData;
  UINT8     OrEcRamData;
} StEcConfigTbl;;

StEcConfigTbl DeAssertEcReset[] =
{
    {ECRAMxA0,    ClearBit (0),                       SD_RST_AUX},
    {ECRAMxA2,    ClearBit (0),                       X1_SLOT_RST_AUX},
    {ECRAMxA3,    ClearBit (0),                       EVAL_RST_AUX | LOM_RESET_AUX | DT_RESET_AUX | WWAN_RST_AUX | WLAN_RESET_AUX},
//    {ECRAMxA7,    ClearBit (TPM_SOI3_RST),            WWAN_MODULE_RST},
    {ECRAMxA7,    ClearBit (0),                       WWAN_MODULE_RST},
};

/**
 Gpio reset control.

 @param[in]         FcnData             Function data
 @param[in]         ResetInfo           Reset information

 @retval            EFI_SUCCESS       Function returns successfully
 @retval            EFI_UNSUPPORTED   Function is not supported
*/
EFI_STATUS
AgesaGpioSlotResetControl (
  IN      UINTN                         FcnData,
  IN      GPIO_RESET_INFO               *ResetInfo
  )
{
  EFI_PEI_SERVICES                      **PeiServices;
  AMD_CPM_TABLE_PPI                     *AmdCpmTablePpi;
  EFI_STATUS                            Status;

  DXIO_PORT_DESCRIPTOR                  *UserCfgPortList;
  PEI_AMD_NBIO_PCIE_COMPLEX_PPI         *NbioPcieComplexPpi;
  DXIO_COMPLEX_DESCRIPTOR               *UserConfig;
//  UINTN                                 Counter = 0;
//  GPIO_RESET_INFO                       GpioResetInfo;
  AMD_CPM_GPIO_DEVICE_POWER_TABLE       *PowerTablePtr;
  UINT8                                 *TmpBufPtr = NULL;
  AMD_CPM_DISPLAY_FEATURE_TABLE         *DisplayFeatureTablePtr;

  UINT8                                 OfstU8;
  UINT8                                 Value8;
  UINTN                                 Index;
  PowerSequenceTbl                      *WwanPowerSequencePtr = NULL;
  UINT32                                WwanPowerSequenceSize = 0;
  AMD_PBS_SETUP_OPTION                  AmdPbsConfiguration;


  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-Start\n"));

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);

  if ( AmdPbsConfiguration.DelayBeforePcieSlotReset!= 0) {
    MicroSecondDelay(AmdPbsConfiguration.DelayBeforePcieSlotReset * 1000); // PCIe card with retimer require delay
  }

  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdCpmTablePpiGuid,
                             0,
                             NULL,
                             &AmdCpmTablePpi
                             );
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-ResetId=%X   ResetControl=%X\n", ResetInfo->ResetId, ResetInfo->ResetControl));
    Status = (*PeiServices)->LocatePpi (PeiServices, &gAmdNbioPcieComplexPpiGuid, 0, NULL, &NbioPcieComplexPpi);

    NbioPcieComplexPpi->PcieGetComplex (NbioPcieComplexPpi, &UserConfig);

    UserCfgPortList = UserConfig->PciePortList;

//    DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-EGPIO26 Output high\n"));
//    AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, 26, 1);                       // EGPIO26 Output high

    // Get PLATFORM_SEL, 0: LilacTv_, 1:MayanTv/LilacTv_RevB
//#if PLATFORM_SEL == 0
//    if (1 == PcdGet8 (PcdRmbBoardSelect))
//#endif
    {
      DEBUG ((DEBUG_INFO, "[%d]: MayanTv/LilacTv(RevB) need to de-assert the SSD0/SSD1 Aux#_RST Pin\n", __LINE__));
      // For MayanTv, SSD0/SSD1 AUX#_RST need to be de-assert
      AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, 78, 1);                         // EGPIO78 Output high
      AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, 79, 1);                         // EGPIO79 Output high
    }


    // //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
    if (PcdGet32 (PcdAmdEnvironmentFlag) == 0)
    {
      for (Index=0; Index < sizeof(DeAssertEcReset)/sizeof(DeAssertEcReset[0]); ++Index)
      {
        OfstU8 = DeAssertEcReset[Index].IdxEcRam;
        AmdCpmTablePpi->CommonFunction.KbcRead (__CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
        DEBUG ((DEBUG_INFO, "  Read   ECRAM0x%2x[%x]\n", OfstU8, Value8));
        Value8 &= (UINT8)DeAssertEcReset[Index].AndEcRamData;
        Value8 |= (UINT8)DeAssertEcReset[Index].OrEcRamData;
        // WWAN override this table.
        switch (AmdPbsConfiguration.PbsWWANDeviceSupport) {
        case 0:
          break;
        case 1: // Fibocom L860 R+
          // Control WWAN sequence later.
          if (OfstU8 == ECRAMxA3) {
            Value8 &= ClearBit (WWAN_RST_AUX);
          }
          if (OfstU8 == ECRAMxA7) {
            Value8 &= ClearBit (WWAN_MODULE_RST);
          }
          break;
        default:
          break;
        }
        DEBUG ((DEBUG_INFO, "  Set    ECRAM0x%2x[%x]\n", OfstU8, Value8));
        AmdCpmTablePpi->CommonFunction.EcRamWrite (__CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
        if ((OfstU8 == ECRAMxA3) && ((Value8 & EVAL_RST_AUX) == EVAL_RST_AUX)) {
          DisplayFeatureTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DISPLAY_FEATURE);
          if (DisplayFeatureTablePtr) {
            if (DisplayFeatureTablePtr->TimePeRstToWakeL23 != 0) {
              // EVAL_RST to LC_WAKE_FROM_L23 Time 1 ~ 30(ms)
              AmdCpmTablePpi->CommonFunction.Stall (AmdCpmTablePpi, DisplayFeatureTablePtr->TimePeRstToWakeL23 * 1000);
            } // End of if (DisplayFeatureTablePtr->TimePeRstToWakeL23 != 0)
          } // End of if (DisplayFeatureTablePtr)
        } // End of if ((OfstU8 == ECRAMxA3) && ((Value8 & EVAL_RST_AUX) == EVAL_RST_AUX))
      } // End of for (Index=0; Index < sizeof(DeAssertEcReset)/sizeof(DeAssertEcReset[0]); ++Index)
    } // End of if (PcdGet32 (PcdAmdEnvironmentFlag) == 0)

    DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-EGPIO26 Output high\n"));
    AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, 26, 1);                       // EGPIO26 Output high
    DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-EGPIO27 Output high\n"));
    AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, 27, 1);                       // EGPIO27 Output high

    switch (AmdPbsConfiguration.PbsWWANDeviceSupport) {
    case 0:
      WwanPowerSequencePtr = NULL;
      WwanPowerSequenceSize = 0;
      break;
    case 1: // Fibocom L860 R+
      DEBUG ((DEBUG_INFO, "Generate power sequence for WWAN Fibocom L860 R+.\n"));
      WwanPowerSequencePtr = FibocomL860RPlus_WwanPowerSequence;
      WwanPowerSequenceSize = sizeof(FibocomL860RPlus_WwanPowerSequence)/sizeof(FibocomL860RPlus_WwanPowerSequence[0]);
      break;
    default:
      break;
    }

    if (WwanPowerSequencePtr != NULL && WwanPowerSequenceSize != 0) {
      for (Index = 0; Index < WwanPowerSequenceSize; ++Index)
      {
        switch (WwanPowerSequencePtr[Index].Type) {
        case DELAY_SET:
          AmdCpmTablePpi->CommonFunction.Stall (AmdCpmTablePpi, WwanPowerSequencePtr[Index].IdxEcRam * 1000);
          break;
        case FCH_GPIO:
          AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, WwanPowerSequencePtr[Index].IdxEcRam, WwanPowerSequencePtr[Index].OrEcRamData);
          break;
        case EC_GPIO:
          OfstU8 = WwanPowerSequencePtr[Index].IdxEcRam;
          AmdCpmTablePpi->CommonFunction.KbcRead (__CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
          DEBUG ((DEBUG_INFO, "  Read   ECRAM0x%2x[%x]\n", OfstU8, Value8));
          Value8 &= (UINT8)WwanPowerSequencePtr[Index].AndEcRamData;
          Value8 |= (UINT8)WwanPowerSequencePtr[Index].OrEcRamData;
          DEBUG ((DEBUG_INFO, "  Set    ECRAM0x%2x[%x]\n", OfstU8, Value8));
          AmdCpmTablePpi->CommonFunction.EcRamWrite (__CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
          break;
        }
      } // End of for (Index = 0; Index < WwanPowerSequenceSize; ++Index)
    }

    switch (AmdPbsConfiguration.PbsWWANDeviceSupport) {
    case 0:
      break;
    case 1: // Fibocom L860 R+
      // PLAT-94051
      AmdCpmTablePpi->CommonFunction.Stall (AmdCpmTablePpi, 300000);  // Delay 300ms
      break;
    default:
      break;
    }


    // Latest doing the S0i3 Reset PIN fuse, Required by HW teams
    if (1 == PcdGetBool (PcdFchMiscModernStanbyEnable))
    {
      DEBUG ((DEBUG_INFO, "[%d]: Reset ECRamxA7[Bit0] TPM_S0I3# to Low\n", __LINE__));
      OfstU8 = ECRAMxA7;
      AmdCpmTablePpi->CommonFunction.KbcRead (__CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
      Value8 &= ClearBit (TPM_SOI3_RST);
      AmdCpmTablePpi->CommonFunction.EcRamWrite (__CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
    }

    // Restore the AMD_CPM_GPIO_DEVICE_POWER_TABLE
    TmpBufPtr = (UINT8*)PcdGetPtr (PcdRmbDevPwrTblStageRestore);
    PowerTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_POWER);
    if (PowerTablePtr)
    {
      for (Index= 0; 0xFF != PowerTablePtr->DevicePowerList[Index].DeviceId; ++Index)
      {
        // Don't skip DEVICE_ID_VGAMUXVCC and DEVICE_ID_VGAMUXSEL
        if ((PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXVCC) && \
            (PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXSEL)) {
          if (0x5F != TmpBufPtr[Index])
          {
            PowerTablePtr->DevicePowerList[Index].InitFlag = TmpBufPtr[Index];
//            DEBUG ((DEBUG_INFO, "  Restore DevicePowerList[%d].InitFlag[%x]\n", (UINTN)Index, (UINTN)PowerTablePtr->DevicePowerList[Index].InitFlag));
            TmpBufPtr[Index] = 0x5F;
          }
        } // End of if ((PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXVCC) && (PowerTablePtr->DevicePowerList[Index].DeviceId != DEVICE_ID_VGAMUXSEL))
      }
      Index = AMD_GPIO_DEVICE_POWER_SIZE;
      PcdSetPtrS (PcdRmbDevPwrTblStageRestore, &Index, TmpBufPtr);
    }
    else
    {
      DEBUG ((DEBUG_ERROR, "[%d] Fail to get DevicePowerList - %r\n", __LINE__, EFI_NOT_FOUND));
    }

//    while (1);

#if 0
    DEBUG ((DEBUG_INFO, "[%d]: Reset ECRamxA7[Bit0] TPM_S0I3# to Low\n", __LINE__));
    {
#define __CFG_CPM_ECRAM_BASE_ADDR   0x662
#define ClearBit(ArgBit)           ((UINT8)~(ArgBit))

      struct {
        UINT8     IdxEcRam;
        UINT8     AndEcRamData;
        UINT8     OrEcRamData;
      } DeAssertEcReset[] =
      {
          {ECRAMxA2,    ClearBit (0),                       X1_SLOT_RST_AUX},
          {ECRAMxA3,    ClearBit (0),                       EVAL_RST_AUX | LOM_RESET_AUX | DT_RESET_AUX | WWAN_RST_AUX | WLAN_RESET_AUX},
          {ECRAMxA7,    ClearBit (TPM_SOI3_RST),            WWAN_MODULE_RST},
      };

      UINT8   OfstU8;
      UINT8   Value8;
      UINTN   Index;

      // //Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO, 4 - GC Lite
      if (PcdGet32 (PcdAmdEnvironmentFlag) == 0)
      {
        for (Index=0; Index < sizeof(DeAssertEcReset)/sizeof(DeAssertEcReset[0]); ++Index)
        {
          OfstU8 = DeAssertEcReset[Index].IdxEcRam;
          AmdCpmTablePpi->CommonFunction.KbcRead (__CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &Value8);
          DEBUG ((DEBUG_INFO, "  Read   ECRAM0x%2x[%x]\n", OfstU8, Value8));
          Value8 &= (UINT8)DeAssertEcReset[Index].AndEcRamData;
          Value8 |= (UINT8)DeAssertEcReset[Index].OrEcRamData;
          DEBUG ((DEBUG_INFO, "  Set    ECRAM0x%2x[%x]\n", OfstU8, Value8));
          AmdCpmTablePpi->CommonFunction.EcRamWrite (__CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
        }
      }
    }

    while ((UserCfgPortList->Flags & DESCRIPTOR_TERMINATE_LIST) == 0) {
      do {
        UserCfgPortList = UserConfig->PciePortList;
        UserCfgPortList = UserCfgPortList + Counter;
        Counter++;
        DEBUG((DEBUG_INFO, "Counter 0x%x\n", Counter));
      } while (UserCfgPortList->EngineData.EngineType == DxioUnusedEngine);

      GpioResetInfo.ResetId = UserCfgPortList->EngineData.GpioGroupId;
      GpioResetInfo.ResetControl = DeassertSlotReset;

      if (GpioResetInfo.ResetId != 1) {
        AmdCpmTablePpi->PeimPublicFunction.PcieReset ( AmdCpmTablePpi,
                                           (UINT8) GpioResetInfo.ResetId,
                                           (UINT8) GpioResetInfo.ResetControl );
        if ((GpioResetInfo.ResetId == DEVICE_ID_MXM) && (GpioResetInfo.ResetControl == CPM_RESET_DEASSERT)) {
          DisplayFeatureTablePtr = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DISPLAY_FEATURE);
          if (DisplayFeatureTablePtr) {
            if (DisplayFeatureTablePtr->TimePeRstToWakeL23 != 0) {
              // EVAL_RST to LC_WAKE_FROM_L23 Time 1 ~ 30(ms)
              AmdCpmTablePpi->CommonFunction.Stall (AmdCpmTablePpi, DisplayFeatureTablePtr->TimePeRstToWakeL23 * 1000);
            } // End of if (DisplayFeatureTablePtr->TimePeRstToWakeL23 != 0)
          } // End of if (DisplayFeatureTablePtr)
        } // End of if ((GpioResetInfo.ResetId == DEVICE_ID_MXM) && (GpioResetInfo.ResetControl == CPM_RESET_DEASSERT))
      } // End of if (GpioResetInfo.ResetId != 1)
    }
#endif
    DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-End-1-EFI_SUCCESS\n"));
    return EFI_SUCCESS;
  }

  DEBUG((DEBUG_INFO, "OEM-PEI-AgesaGpioSlotResetControl-End-2-EFI_UNSUPPORTED\n"));
  return EFI_UNSUPPORTED;
}

