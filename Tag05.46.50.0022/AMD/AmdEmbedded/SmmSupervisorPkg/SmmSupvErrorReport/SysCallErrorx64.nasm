;------------------------------------------------------------------------------ ;
; Copyright (C) Microsoft Corporation.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   SysCallErrorx64.nasm
;
; Abstract:
;
;   Manipulate registers and jump to C code function for Smm error report
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

extern ASM_PFX(SmmErrorLogger)

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; AsmSmmErrorLogger(
;  IN UINTN          Arg0,
;  IN UINTN          Arg1,
;  IN UINTN          Arg2,
;  IN UINTN          Arg3,
;  IN UINTN          CallerAddr
;  );
;------------------------------------------------------------------------------
global ASM_PFX(AsmSmmErrorLogger)
ASM_PFX(AsmSmmErrorLogger):
; sysret from SysCallEntry_msvc.nasm jumps to this function.

; CallIndex in RAX, Arg1 in RDX, Arg2 in R8, Arg3 in R9
; CallerAddr is passed via R10
; NOTE it is now in CPL3

  mov     rcx, rax            ; Put CallerIndex to first input argument
  push    r10                 ; R10 contained CallerAddr, works as parameter of SmmErrorLogger, as well as return address of this routine
  sub     rsp, 0x20           ; Leave 4 stack segments for input args and align stack per calling convention

  ;EFIAPI
  ;SmmErrorLogger (
  ;  IN UINTN          CallIndex,
  ;  IN UINTN          Arg1,
  ;  IN UINTN          Arg2,
  ;  IN UINTN          Arg3,
  ;  IN UINTN          CallerAddr
  ; System may hang or reset in SmmErrorLogger depending on policy.
  call    ASM_PFX(SmmErrorLogger)

  ;Balance the stack
  add     rsp, 0x20

  xor     rax, rax            ; If returns from SmmErrorLogger after policy violation, always return 0.
  pop     r10
  jmp     r10                 ; Returns to original caller using syscall.