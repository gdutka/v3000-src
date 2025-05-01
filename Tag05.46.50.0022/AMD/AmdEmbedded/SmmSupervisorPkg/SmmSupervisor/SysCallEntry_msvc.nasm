;------------------------------------------------------------------------------ ;
; Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>
; Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
; Copyright (C) Microsoft Corporation.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   WriteTr.nasm
;
; Abstract:
;
;   Write TR register
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

extern ASM_PFX(SyscallDispatcher)
extern ASM_PFX(mExternalErrorLogger)


; This should be OFFSET_OF (MM_SUPV_SYSCALL_CACHE, MmSupvRsp)
%define MM_SUPV_RSP                     0x00
; This should be OFFSET_OF (MM_SUPV_SYSCALL_CACHE, SavedUserRsp)
%define SAVED_USER_RSP                  0x08

;------------------------------------------------------------------------------
; Caller Interface:
; UINT64
; EFIAPI <SysV calling convention>
; SysCall (
;   UINTN CallIndex,
;   UINTN Arg1,
;   UINTN Arg2,
;   UINTN Arg3
;   );
;
; Backend Interface:
; UINT64
; EFIAPI
; SyscallDispatcher (
;   UINTN         CallIndex,
;   UINTN         Arg1,
;   UINTN         Arg2,
;   UINTN         Arg3,
;   UINTN         CallerAddr
;   );
;------------------------------------------------------------------------------
global ASM_PFX(SyscallCenter)
ASM_PFX(SyscallCenter):
; Calling convention: CallIndex in RAX, Arg1 in RDX, Arg2 in R8, Arg3 in R9 from SysCallLib
; Architectural definiton: CallerAddr in RCX, rFLAGs in R11 from x64 syscall instruction
; push CallIndex stored at top of stack

; First instruction used will be swapgs to exchange GS in order to load previously populated
; ring0 stack pointer into RSP (and save current RSP). Then all register/memory operations are performed on trusted space.
; Need to ensure use kernel stack all through SyscallCenter function
    swapgs  ; get kernel pointer, save user GSbase
    mov gs:[SAVED_USER_RSP], rsp ; save user's stack pointer
    mov rsp, gs:[MM_SUPV_RSP] ; set up kernel stack

    ;Preserve all registers in CPL3
    push    rax
    push    rcx
    push    rbp
    push    rdx
    push    r8
    push    r9
    push    rsi
    push    r12
    push    rdi
    push    rbx
    push    r11




    push    0                            ; *JumpToErrorHandler = 0. Reserve space on stack and initialize to 0.

    ;
    ;  Setup calling parameters.
    ;
    ;  SyscallDispatcher (
    ;    UINTN         CallIndex,
    ;    UINTN         Arg1,
    ;    UINTN         Arg2,
    ;    UINTN         Arg3,
    ;    UINTN         CallerAddr,
    ;    BOOLEAN       *JumpToErrorHandler
    ;    );

; Note: Stack must maintain current alignment. If stack is modified, please verify alignment by using using
; Library Instance BaseMemoryLibSse2 function CopyMem in C code to very XMM 16-byte alignment.

    push    rsp                          ; Push pointer JumpToErrorHandler. Address of rsp for "push 0" above.
    push    rcx                          ; Push CallerAddr. On entry, rcx = caller address.
                                         ; R9  = Arg3. On SyscallCenter entry, R9 = Arg3.
                                         ; R8  = Arg2. On SyscallCenter entry, R8 = Arg2.
                                         ; RDX = Arg1. On SyscallCenter entry, RDX = Arg1.
    mov     rcx, rax                     ; RCX = CallerIndex = RAX. On SyscallCenter entry, RAX = CallerIndex.
    sub     rsp, 0x20                    ; Reserve 0x20 bytes as part of calling convention.

    call    ASM_PFX(SyscallDispatcher)
    mov     r10B, [rsp + 0x20 + 8 + 8]   ; R10B = *JumpToErrorHandler

    ; Balance the Stack
    ;   push    0
    ;   push    rsp
    ;   push    rcx
    ;   sub     rsp, 0x20
    add     rsp, 0x20 + 8 + 8 + 8


    ;restore registers from stack except RAX
    pop     r11
    pop     rbx
    pop     rdi
    pop     r12
    pop     rsi
    pop     r9
    pop     r8
    pop     rdx
    pop     rbp
    pop     rcx                          ; RCX = return address of caller

    ;
    ;   Check if normal return or execute ErrorLogger.
    ;
    or      r10B, r10B
    jz      NormalReturn  ; if syscall dispatcher changed return address, need to go through error report

    pop     rax           ; RAX = CallIndex

    mov     r10, rcx      ; keep the Return address in R10, caller to SyscallCenter.
                          ; When AsmErrorLogger executes "jmp r10", it will jump to this address.
    ; The next will be call index pushed from rax
    mov     rcx, [ASM_PFX(mExternalErrorLogger)]    ;rcx = AsmSmmErrorLogger in SysCallErrorX64.nasm
    jmp     Sysret
NormalReturn:
    add     rsp, 8        ; pop rax without changing rax.
Sysret:
    mov     rsp, gs:[SAVED_USER_RSP]  ; restore user RSP
    swapgs  ; restore user GS, save kernel pointer
    ;rax = return value for normal return.
    ;rax = CallIndex for Error Logger.
    ;rcx = sysret jump to address.
    db      48h           ; return to the CPL3
    sysret
