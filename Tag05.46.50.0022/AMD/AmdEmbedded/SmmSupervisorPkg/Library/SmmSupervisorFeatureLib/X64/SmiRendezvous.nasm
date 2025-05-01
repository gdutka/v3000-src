;------------------------------------------------------------------------------ ;
; Copyright (c) 2016 - 2019, Intel Corporation. All rights reserved.<BR>
; Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   SmiRendezvous.nasm
;
; Abstract:
;
;   Code template of the SMI handler for a particular processor
;
;-------------------------------------------------------------------------------




extern ASM_PFX(SmiRendezvous)
extern ASM_PFX(CpuSmmDebugEntry)
extern ASM_PFX(CpuSmmDebugExit)

    DEFAULT REL
    SECTION .text

global ASM_PFX(SmiRendezvousEntry)
BITS 64
;Below function will be invoke in both CPL0 mode and CPL3 mode
;Input for this func, rdx CpuIndex, r15 return address
;If it executed in CPL0 mode, it can return back to SmiEntry by jmp r15
;If it executed in CPL3 mode, it can return back to SmiEntry through callgate provide in r15
;This routine need to ensure stack balance
SmiRendezvousEntry:
    mov     rbx, rdx
    add     rsp, -0x20


    mov     rcx, rbx
    call    ASM_PFX(CpuSmmDebugEntry)

    mov     rcx, rbx
    call    ASM_PFX(SmiRendezvous)

    mov     rcx, rbx
    call    ASM_PFX(CpuSmmDebugExit)

    add     rsp, 0x20

    mov     ax, cs
    and     ax, 0x03
    cmp     al, 0x03                                              ;   query to see if CPL3
    jz      Callgate
    jmp     r15                                                   ;   r15 contain the return address to SmiEntry
Callgate:
;   invoke CALL GATE
;   In a call-gate transfer, the CS selector points to a call-gate descriptor rather than a code-segment descriptor, and the rIP is ignored
;   (but required by the instruction).
    push    r15
    call    far qword [rsp]             ; return to ring 0 via call gate
    jmp     $                           ; Code should not reach here
