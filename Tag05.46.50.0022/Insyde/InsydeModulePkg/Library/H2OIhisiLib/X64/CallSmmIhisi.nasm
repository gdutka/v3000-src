;;******************************************************************************
;;* Copyright (c) 2017 - 2021, Insyde Software Corp. All Rights Reserved.
;;*
;;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;;* transmit, broadcast, present, recite, release, license or otherwise exploit
;;* any part of this publication in any form, by any means, without the prior
;;* written permission of Insyde Software Corporation.
;;*
;;******************************************************************************
;;
;; Module Name:
;;
;;   CallSmmIhisi.nasm
;;
;; Abstract:
;;
;;   64 bit Sent SMI to call IHISI flash ROM part
;;

SEGMENT .text

;------------------------------------------------------------------------------
;  VOID
;  IhisiLibGenericSmi (
;    IN OUT     H2O_IHISI_PARAMS	        *ParamsBuffer       // rcx
;    );
;------------------------------------------------------------------------------
global ASM_PFX(IhisiLibGenericSmi)
ASM_PFX(IhisiLibGenericSmi):

  push    rax
  push    rbx
  push    rsi
  push    rdi
  push    rcx
  push    rdx
  push    r8

  mov     r8,  rcx

  mov     rax, qword [r8 + 8]
  mov     rdx, qword [r8 + 32]

  out     dx, al

  pop     r8
  pop     rdx
  pop     rcx
  pop     rdi
  pop     rsi
  pop     rbx
  pop     rax
  ret
