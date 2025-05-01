;****************************************************************************
; Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
;
;****************************************************************************
;/**
; * @file
; *
; * Agesa library 32bit
; *
; * Contains AMD AGESA Library
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project:  Lib
; * @e \$Revision: 312770 $   @e \$Date: 2015-02-11 13:24:42 +0800 (Wed, 11 Feb 2015) $
; */

    SECTION .text

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Enter debugger on SimNow
; *
; * @retval  Flag to indicate execution of Port 80 loop code.
; *
; */
global ASM_PFX(LibAmdSimNowEnterDebugger)
ASM_PFX(LibAmdSimNowEnterDebugger):

    push    ebx
    push    ecx
    push    edx
    push    esi

    mov     si,  0                  ; Clear SI to indicate display message on port 80
    mov     eax, 0BACCD00Bh         ; Backdoor in SimNow
    mov     ebx, 2                  ; Select breakpoint feature
    cpuid

    mov     ax,  si                 ; Return value in SI

    pop     esi
    pop     edx
    pop     ecx
    pop     ebx

    ret

;/*---------------------------------------------------------------------------------------*/
;/**
; *  Force breakpoint on HDT
; *
; *
; */
global ASM_PFX(LibAmdHDTBreakPoint)
ASM_PFX(LibAmdHDTBreakPoint):

    pushad

    mov     ecx, 0C001100Ah         ;bit 0 = HDT redirect
    mov     edi, 09C5A203Ah         ;Password
    rdmsr                           ;
    or      al,  1                  ;
    wrmsr                           ;
    mov     al,  0B2h               ;Marker = B2
    db      0F1h                    ;ICEBP

    popad
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
