/** @file
  This library abstract how to access TCM hardware device.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
**/

#ifndef _TCM_DEVICE_LIB_H_
#define _TCM_DEVICE_LIB_H_

#include <Tcm.h>

/**
  This service enables the sending of commands to the TCM.

  @param[in]      InputParameterBlockSize  Size of the TCM input parameter block.
  @param[in]      InputParameterBlock      Pointer to the TCM input parameter block.
  @param[in,out]  OutputParameterBlockSize Size of the TCM output parameter block.
  @param[in]      OutputParameterBlock     Pointer to the TCM output parameter block.

  @retval EFI_SUCCESS            The command byte stream was successfully sent to the device and a response was successfully received.
  @retval EFI_DEVICE_ERROR       The command was not successfully sent to the device or a response was not successfully received from the device.
  @retval EFI_BUFFER_TOO_SMALL   The output parameter block is too small.
**/
EFI_STATUS
EFIAPI
TcmSubmitCommand (
  IN UINT32            InputParameterBlockSize,
  IN UINT8             *InputParameterBlock,
  IN OUT UINT32        *OutputParameterBlockSize,
  IN UINT8             *OutputParameterBlock
  );

/**
  This service requests use TCM.

  @retval EFI_SUCCESS      Get the control of TCM chip.
  @retval EFI_NOT_FOUND    TCM not found.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TisPcRequestUseTcm (
  VOID
  );

#endif
