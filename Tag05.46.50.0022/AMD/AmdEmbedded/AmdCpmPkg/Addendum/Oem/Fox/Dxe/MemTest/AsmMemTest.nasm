;*****************************************************************************
;*
;* Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
;*
;******************************************************************************
;*/
SECTION .text

;rcx = Start
;rdx = Mid
;r8 = Length



global ASM_PFX(AsmCopyMem)
ASM_PFX(AsmCopyMem):
    push  rbx
    push  rsi
    push  rdi

    mov   rax, rcx
    mov   rbx, rdx
    mov   rdx, r8

    ;
    ; At the end of all this
    ; - the second half equals the inital value of the first half
    ; - the first half is right shifted 32-bytes (with wrapping)
    ;

    ; Move first half to second half
    mov   rsi,  rax           ; Source, 'buf' (start point)
    mov   rdi,  rbx           ; Destination 'mid' (mid point)
    mov   rcx,  rdx           ; Length, 'half_len_dw' (size of a half in DWORDS)
    rep   movsd

    ; Move the second half, less the last 32-bytes.
    mov   rsi, rbx            ; Source, 'mid'
    mov   rdi, rax            ; Destination 'buf'
    mov   rcx, rdx            ; Length, 'half_len_dw' (size of a half in DWORDS)
    rep   movsd

    pop   rdi
    pop   rsi
    pop   rbx

    ret


global ASM_PFX(AsmDeadLoop)
ASM_PFX(AsmDeadLoop):
    jmp $
    ret