/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiPei.h>
#include <Library/OemClkReqControlLib.h>

#define FILECODE LIBRARY_NBIOCLKREQCONTROLLIBNULL_NBIOCLKREQCONTROLLIBNULL_FILECODE
/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

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
  return EFI_UNSUPPORTED;
}


