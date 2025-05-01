/*********************************************************************************
;
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
;******************************************************************************
;*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE FCH_KEITH_FCHKEITHCORE_KEITH_KTUSB_KTXHCILATE_FILECODE

/**
 * FchInitLateUsbXhci - Config USB3 controller before OS Boot
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitLateUsbXhci (
  IN  VOID     *FchDataPtr
  )
{
    FCH_DATA_BLOCK         *LocalCfgPtr;
    AMD_CONFIG_PARAMS      *StdHeader;
    LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
    StdHeader = LocalCfgPtr->StdHeader;
    AGESA_TESTPOINT (TpFchInitLateUsb, NULL);
    FchSmnRW (0, 0x10134608, ~(UINT32) (7 << 24), 0 , NULL);
    FchSmnRW (0, 0x10134808, ~(UINT32) (7 << 24), 0 , NULL);
    FchSmnRW (0, 0x10136008, ~(UINT32) (7 << 24), 0 , NULL);
    FchSmnRW (0, 0x10136608, ~(UINT32) (7 << 24), 0 , NULL);
    FchSmnRW (0, 0x10136808, ~(UINT32) (7 << 24), 0 , NULL);

    FchSmnRW (0, 0x1013461C, ~(UINT32) (0x7FF << 5), 0 , NULL);
    FchSmnRW (0, 0x1013481C, ~(UINT32) (0x7FF << 5), 0 , NULL);
    FchSmnRW (0, 0x1013601C, ~(UINT32) (0x7FF << 5), 0 , NULL);
    FchSmnRW (0, 0x1013661C, ~(UINT32) (0x7FF << 5), 0 , NULL);
    FchSmnRW (0, 0x1013681C, ~(UINT32) (0x7FF << 5), 0 , NULL);
    //FchKTXhciDisablePortLate (0, FchDataPtr);
}


