/** @file
  LPC/eSPI I/O decoding control library.

;******************************************************************************
;* Copyright 2022 Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef IO_DECODE_EX_LIB_H_
#define IO_DECODE_EX_LIB_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>

/**
  Enables decoding of I/O addresses to LPC or eSPI bus.

  @param[in]  IoAddress  I/O base address to enable.
  @param[in]  IoLength   Length of the I/O space to enable.
  @param[in]  SlaveId    For I/O decoding to LPC bus, SlaveId is not applicable
                         and must specify zero. For decoding to eSPI bus, this
                         specifies the Chip Select# pin which the I/O addresses
                         route to. Possible values vary among PCH design.

  @retval EFI_SUCCESS           I/O decode is enabled successfully.
  @retval EFI_OUT_OF_RESOURCES  There is no resource to enable I/O decode.

**/
EFI_STATUS
IoDecodeControlEx (
  IN UINT16  IoAddress,
  IN UINT16  IoLength,
  IN UINTN   SlaveId
  );

#endif // IO_DECODE_EX_LIB_H_