/** @file
  DxeSmmH2OIoLibSmm.h.

;******************************************************************************
;* Copyright (c) 2020, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __DXE_SMM_H2O_IOLIB_H_
#define __DXE_SMM_H2O_IOLIB_H_



#include <Base.h>

#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/DxeSmmIoLibProtocol.h>
#include <Protocol/SmmCpuIo2.h>

extern H2O_DXE_SMM_IO_LIB_PROTOCOL *mDxeSmmIoLibProtocol;
#endif
