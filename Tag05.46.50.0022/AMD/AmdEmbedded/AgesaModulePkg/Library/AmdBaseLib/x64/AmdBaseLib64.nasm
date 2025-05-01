;****************************************************************************
; Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
;
;****************************************************************************
;/**
; * @file
; *
; * Agesa library 64bit
; *
; * Contains AMD AGESA Library
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project:  Lib
; * @e \$Revision: 312770 $   @e \$Date: 2015-02-11 13:24:42 +0800 (Wed, 11 Feb 2015) $
; */

    DEFAULT REL
    SECTION .text

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Enter debugger on SimNow
; *
; *
; * @retval  Flag to indicate execution of Port 80 loop code.
; */
global ASM_PFX(LibAmdSimNowEnterDebugger)
ASM_PFX(LibAmdSimNowEnterDebugger):
    pushfq
    push    rbx
    push    rcx
    push    rdx
    push    rsi
    mov     si,  0                  ; Clear SI to indicate display message on port 80
    mov     rax, 0BACCD00Bh         ; Backdoor in SimNow
    mov     rbx, 2                  ; Select breakpoint feature
    cpuid

    mov     ax, si                  ; Return SI
    pop     rsi
    pop     rdx
    pop     rcx
    pop     rbx
    popfq
    ret


;/*---------------------------------------------------------------------------------------*/
;/**
; *  Force breakpoint on HDT
; *
; *
; */
global ASM_PFX(LibAmdHDTBreakPoint)
ASM_PFX(LibAmdHDTBreakPoint):
    push    rcx
    push    rdi
    push    rax
    push    rdx
    mov     rcx, 0C001100Ah         ;bit 0 = HDT redirect
    mov     rdi, 09C5A203Ah         ;Password
    rdmsr
    and     rax, 0ffffffffh
    or      rax, 1

    wrmsr

    mov     rax, 0B2h               ;Marker = B2
    db      0F1h                    ;ICEBP

    pop     rdx
    pop     rax
    pop     rdi
    pop     rcx
    ret

;/*---------------------------------------------------------------------------------------*/
;/**
; *  FPU init
; *
; *
; */
global ASM_PFX(LibAmdFinit)
ASM_PFX(LibAmdFinit):
    finit
    ret
