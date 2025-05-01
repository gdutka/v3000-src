;*****************************************************************************
;
; Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
;
;******************************************************************************

DEFAULT REL
SECTION .text


global ASM_PFX(AsmSaveS3Context)
ASM_PFX(AsmSaveS3Context):
    push       rbx                          ; Must preserve rbx as according to calling convention. rax, rcx, rdx may be destroyed.
    push       rsi
    pushf
    cli

    mov        rbx, rcx                     ; Saves S3 resume entry point

    lea        rsi, [LongModeStart]         ; Avoid linker build error by reading this in 64-bit to use in 32-bit mode.

    ;
    ; Switch to 32-bit mode.
    ;

    push       10h                      ;32-bit code selector
    lea        rdx, [CompatibilityMode]
    push       rdx
    DB      0x48                         ; prefix to composite "retq" with next "retf"
    retf

CompatibilityMode:
   BITS 32

    mov        ax, 8h                   ;32-bit data selector
    mov        ds, ax
    mov        es, ax
    mov        fs, ax
    mov        gs, ax
    mov        ss, ax

    ; Disable paging - disables long mode
    mov        eax, cr0
    btr        eax, 31                  ; PG = 0
    mov        cr0, eax

    ; Disable long support
    mov        ecx, 0c0000080h          ; MSR EFER
    rdmsr
    btr        eax, 8                   ; LME = 0
    wrmsr

    ; Save S3 resume context
    mov     ecx, 0C00110E0h
    mov     eax, ebx                    ; rax = S3 resume address
    mov     edx, 0
    wrmsr
    wbinvd

    ; Switch to 64-bit mode
    mov     ecx, 0c0000080h             ; MSR EFER
    rdmsr
    bts     eax, 8                      ; LME = 1
    wrmsr

    ; Enable paging - entering Compatibility Mode
    mov     eax, cr0
    bts     eax, 31                     ; PG = 1
    mov     cr0, eax

    ; Far jump to 64-bit code
    push    38h                         ; 64-bit selector
    push    esi                         ; LongModeStart
    retf

LongModeStart:
BITS 64
    mov     ax, 30h
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

    popf
    pop     rsi
    pop     rbx
    ret
