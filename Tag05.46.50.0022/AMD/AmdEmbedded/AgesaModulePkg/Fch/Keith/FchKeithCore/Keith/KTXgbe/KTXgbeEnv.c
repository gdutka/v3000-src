/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include "FchPlatform.h"
#include "Filecode.h"
#include "GnbRegistersRMB/GLOBALREGS.h"
#include <Library/PcdLib.h>
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTXGBE_KTXGBEENV_FILECODE


/**
 * FchInitEnvXGbe - Config xGbE controller before PCI emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvXGbe (
  IN  VOID     *FchDataPtr
  )
{
  FCH_DATA_BLOCK         *LocalCfgPtr;
  SYSHUB_DS_CTRL_SHUBCLK_STRUCT     SyshubDsCtlShubClk;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8                  Port;
  UINT32                 SystemPort;
  UINT32                 XgbeNbifStrap0;
  UINT32                 XgbeNbifStrap1;
  FCH_XGBE_PORT          *FchXgbePort;
  FCH_XGBE_MAC           *FchXgbePortMac;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;
  FchXgbePort = &LocalCfgPtr->Xgbe.Port[0];
  FchXgbePortMac = &LocalCfgPtr->Xgbe.Mac[0];

  AGESA_TESTPOINT (TpFchInitEnvXgbe, NULL);
  //PLAT-17946
  FchSmnRead (0, FCH_KT_RCC_DEV1_EPF2_STRAP0, &XgbeNbifStrap0, NULL);
  FchSmnRead (0, FCH_KT_RCC_DEV1_EPF2_STRAP0 + 0x200, &XgbeNbifStrap1, NULL);
  
  //Disable ds_allow (DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE) when XGBE IP is used 
  // SHUBCLK
  FchSmnRead (
    0,
    SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS,
    &(SyshubDsCtlShubClk.Value),
    NULL
    );

  if (LocalCfgPtr->Xgbe.XgbeDisable || (((XgbeNbifStrap0 & BIT28) == 0) && ((XgbeNbifStrap1 & BIT28) == 0))  || ((LocalCfgPtr->Xgbe.Port[0].XgbePortConfig == FALSE) && (LocalCfgPtr->Xgbe.Port[1].XgbePortConfig == FALSE))) 
    SyshubDsCtlShubClk.Field.DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE = 1;
  else
    SyshubDsCtlShubClk.Field.DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE = 0;

   IDS_HDT_CONSOLE (FCH_TRACE, "[FCH]SyshubDsCtlShubClk.Field.DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE: %d\n", SyshubDsCtlShubClk.Field.DMA_CL22_SHUBCLK_DEEPSLEEP_ALLOW_ENABLE);
  
  FchSmnWrite (
   0,
    SMN_NBIF0_SYSHUB_DS_CTRL_SHUBCLK_ADDRESS,
    &(SyshubDsCtlShubClk.Value),
    NULL
    );

  for (Port = 0; Port < 2; Port++) {
    //
    // Check if GMAC is enabled
    //
    //PLAT-16038
    if (FchXgbePort->XgbePortConfig == FALSE) {
      //Disable XGbe to pass Os install
      FchSmnRW (0, FCH_KT_RCC_DEV1_EPF2_STRAP0 + Port * 0x200, ~ (UINT32) (BIT28), 0, NULL);
    }

    FchSmnRead (0, FCH_KT_RCC_DEV1_EPF2_STRAP0 + Port * 0x200, &XgbeNbifStrap0, NULL);

    if (XgbeNbifStrap0 & BIT28) {
      SystemPort = Port;
      LocalCfgPtr->Xgbe.MaxPortNum++;
      if (FchXgbePort->XgbePortConfig) {
        FchKtXgbePortPlatformConfig (0, Port, SystemPort, FchXgbePort);
        FchKtXgbePortMacAddress (0, Port, FchXgbePortMac);
        FchKtXgbePortPtpInit (0, Port, FchXgbePort);
      }
    } else {
      FchKtXgbePortClockGate (0, Port);
    }
    FchXgbePort++;
    FchXgbePortMac++;
  }

}
