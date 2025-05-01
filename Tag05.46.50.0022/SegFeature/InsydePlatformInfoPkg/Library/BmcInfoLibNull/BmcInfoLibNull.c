/** @file
  Implementation of BmcInfoLibNull.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include "Library/BmcInfoLib.h"

#include <Uefi.h>

/**
  Get Boot Option string.

  @param[out]   BootOptionStr   The pointer to Boot Option string.
                                Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetBootOptionStr (
  OUT CHAR16  **BootOptionStr
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get Current Boot Mode string.

  @param[out]   CurrentBootModeStr  The pointer to Current Boot Mode string.
                                    Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetCurrentBootModeStr (
  OUT CHAR16  **CurrentBootModeStr
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get IPv4 Address strings.

  @param[in]    ChannelNumber   The channel number.
  @param[out]   IPv4AddressStr  The pointer to IPv4 Address string.
                                Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetIPv4AddressStr (
  IN  UINT8   ChannelNumber,
  OUT CHAR16  **IPv4AddressStr
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get IPv6 Address strings.

  @param[in]    ChannelNumber           The channel number.
  @param[out]   NumberOfIPv6AddressStr  Number of IPv6 Address strings.
  @param[out]   IPv6AddressStrBuffer    The pointer to buffer of IPv6 Address strings.
                                        Caller has to free the memory after use it.

  @retval EFI_SUCCESS             Function completes successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_OUT_OF_RESOURCES    Out of resource.

**/
EFI_STATUS
GetIPv6AddressStrBuffer (
  IN  UINT8   ChannelNumber,
  OUT UINTN   *NumberOfIPv6AddressStr,
  OUT CHAR16  ***IPv6AddressStrBuffer
  )
{
  return EFI_UNSUPPORTED;
}