/** @file
  SVM feature enable/disable function

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include "SVM.h"

/**
  The internal funciton to enable AMD SVM.

  @param  SVMLock      Lock SvmeDisable of MSR VM_CR  

**/
VOID
EnableSVM (
  IN  BOOLEAN   SVMLock
)
{
  UINT64  MsrData;

  if (SVMLock){
    AsmWriteMsr64 (AMD_MSR_SVM_LOCK_KEY, 0);
  }

  MsrData = AsmReadMsr64 (AMD_MSR_VM_CR);
  MsrData &= ~(0x10);
  if (SVMLock){
    MsrData |= 0x8;
  }
  AsmWriteMsr64 (AMD_MSR_VM_CR, MsrData);
}

/**
  The internal funciton to disable AMD SVM.

  @param  SVMLock      Lock SvmeDisable of MSR VM_CR  

**/
VOID
DisableSVM (
  IN  BOOLEAN   SVMLock
)
{
  UINT64  MsrData;

  if (SVMLock){
    AsmWriteMsr64 (AMD_MSR_SVM_LOCK_KEY, 0);
  }
  MsrData = AsmReadMsr64 (AMD_MSR_VM_CR);
  MsrData |= 0x10;
  if (SVMLock){
    MsrData |= 0x8;
  }
  AsmWriteMsr64 (AMD_MSR_VM_CR, MsrData);
}

/**
  The funciton to disable/enable AMD SVM feature.

  @param  SVMEnable    Enable or Disable AMD SVM feature
  @param  SVMLock      Lock SvmeDisable of MSR VM_CR  

**/
VOID
EnableDisableSVM (
  IN  BOOLEAN   SVMEnable,
  IN  BOOLEAN   SVMLock
)
{  
  if (SVMEnable) {
      EnableSVM (SVMLock);
  } else {
      DisableSVM (SVMLock);
  }
}