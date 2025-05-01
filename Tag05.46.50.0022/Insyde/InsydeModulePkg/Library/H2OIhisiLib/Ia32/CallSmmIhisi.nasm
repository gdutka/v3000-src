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
;;   32 bit Sent SMI to call IHISI flash ROM part
;;

  SECTION .text

;------------------------------------------------------------------------------
;  VOID
;  IhisiLibGenericSmi (
;    IN OUT     H2O_IHISI_PARAMS	        *ParamsBuffer  // ebp + 08h
;    );
;------------------------------------------------------------------------------
global ASM_PFX(IhisiLibGenericSmi)
ASM_PFX(IhisiLibGenericSmi):
  push    ebp
  mov     ebp, esp
  push    eax
  push    ebx
  push    esi
  push    edi
  push    ecx
  push    edx

  mov     eax, [ebp + 0x10]
  mov     edx, [eax + 24]
  mov     eax, [eax]

  out     dx, al


  pop     edx
  pop     ecx
  pop     edi
  pop     esi
  pop     ebx
  pop     eax
  leave
  ret

