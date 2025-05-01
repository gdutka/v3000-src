/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <AMD.h>
#include <AmdCpmBase.h>
#include <Library/OemClkReqControlLib.h>
#include <Library/IdsLib.h>
#include <Library/IoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>


/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

// This value is the same for both Matisse and Premium Chipset
#define   MMIO_GPP_CLK_CNTROL_ADDRESS     0xFED80E00

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef union _GPP_CLK_CNTROL_STRUC {
    UINT32  Value;
    struct {
      UINT32        GppClk0: 2;
      UINT32        GppClk1: 2;
      UINT32        GppClk4: 2;
      UINT32        GppClk2: 2;
      UINT32        GppClk3: 2;
      UINT32        GppClk5: 2;
      UINT32        GppClk6: 2;
      UINT32        Reservd: 18;
    } Field;
} GPP_CLK_CNTROL_STRUC;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 Gpio Clk Req Control.

 @param[in]         NbioRootBus           Root Bus number of target NBIO (or chipset)
 @param[in]         ClkReqNumber          ClkReq number request
 @param[in]         ChipsetFlag           Premium Chipset flag (1 == Use Premium Chipset registers)

 @retval            EFI_SUCCESS       Function returns successfully
 @retval            EFI_UNSUPPORTED   Function is not supported
*/
EFI_STATUS
OemGpioClkReqControl (
  IN      UINT32                     NbioRootBus,
  IN      UINT8                      ClkReqNumber,
  IN      UINT8                      ChipsetFlag
  )
{
  GPP_CLK_CNTROL_STRUC             GppClkCntrol;
  AMD_PBS_SETUP_OPTION              AmdPbsConfiguration;
  EFI_STATUS                        PbsStatus;
  EFI_STATUS                        Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Bus %d Clk %d Chipset %d\n", __FUNCTION__, NbioRootBus, ClkReqNumber, ChipsetFlag);

  if (1 == ChipsetFlag) {
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;
  PbsStatus = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (ClkReqNumber != 0) {
    GppClkCntrol.Value = MmioRead32 (MMIO_GPP_CLK_CNTROL_ADDRESS + 0x00);

    switch (ClkReqNumber) {
      case DxioClkReq0:
        if ((0 == AmdPbsConfiguration.ClockRequest0) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq0 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk0 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq0\n");
        }
        break;
      case DxioClkReq1:
        if ((0 == AmdPbsConfiguration.ClockRequest1) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq1 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk1 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq1\n");
        }
        break;
      case DxioClkReq2:
        if ((0 == AmdPbsConfiguration.ClockRequest2) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq2 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk2 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq2\n");
        }
        break;
      case DxioClkReq3:
        if ((0 == AmdPbsConfiguration.ClockRequest3) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq3 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk3 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq3\n");
        }
        break;
      case DxioClkReq4:
        if ((0 == AmdPbsConfiguration.ClockRequest4) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq4 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk4 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq4\n");
        }
        break;
      case DxioClkReq5:
        if ((0 == AmdPbsConfiguration.ClockRequest5) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq5 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk5 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq5\n");
        }
        break;
      case DxioClkReq6:
        if ((0 == AmdPbsConfiguration.ClockRequest6) && (PbsStatus == EFI_SUCCESS)) {
          Status = EFI_UNSUPPORTED;
          IDS_HDT_CONSOLE (GNB_TRACE, " - ClkReq6 is disabled\n");
        } else {
          GppClkCntrol.Field.GppClk6 = 1;
          IDS_HDT_CONSOLE (GNB_TRACE, " - Enable ClkReq6\n");
        }
        break;
      default:
        break;
    }

    MmioWrite32 (MMIO_GPP_CLK_CNTROL_ADDRESS + 0x00, GppClkCntrol.Value);
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, MMIO_GPP_CLK_CNTROL_ADDRESS + 0x00, 1, &(GppClkCntrol.Value));
  }
  return Status;
}


