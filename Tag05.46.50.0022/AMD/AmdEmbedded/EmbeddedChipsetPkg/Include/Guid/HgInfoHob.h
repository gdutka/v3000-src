/** @file
  This file contains definitions required for creation of HG Information data HOB.

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _HG_INFORMATION_DATA_HOB_H_
#define _HG_INFORMATION_DATA_HOB_H_

//
// {648CE07B-AE5D-4973-BD3C-8C9153C05DC5}
//
#define HG_INFOFORMATION_DATA_HOB_GUID \
  { \
    0x648ce07b, 0xae5d, 0x4973, 0xbd, 0x3c, 0x8c, 0x91, 0x53, 0xc0, 0x5d, 0xc5 \
  }

extern EFI_GUID                               gH2OHgInformationDataHobGuid;

#pragma pack (push, 1)
typedef struct _HG_INFORMATION_DATA_HOB {
  EFI_HOB_GUID_TYPE                           EfiHobGuidType;

  UINT8                                       HgMode;
  UINT8                                       BootType;

  UINT32                                      DelayAfterPwrEn;
  UINT32                                      DelayAfterReset;

  UINT16                                      DgpuPresentGpioNo;
  UINT16                                      DgpuResetGpioNo;
  UINT16                                      DgpuPwrEnableGpioNo;
  UINT16                                      DgpuPwrOnGpioNo;
  UINT16                                      DgpuPwrGoodGpioNo;                                  

  BOOLEAN                                     DgpuResetActive;
  BOOLEAN                                     DgpuPwrEnableActive;
  BOOLEAN                                     DgpuPwrOnActive;
  BOOLEAN                                     DgpuPwrGoodActive;

  BOOLEAN                                     DgpuGpioSupport;

} HG_INFORMATION_DATA_HOB;
#pragma pack (pop)

#endif
