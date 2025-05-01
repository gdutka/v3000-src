;*****************************************************************************
; AMD Generic Encapsulated Software Architecture
;
; Module Name:
;
;   SmmSupvGdtTbl.nasm
;
; Abstract:
;
;   GDT structure
;
;-------------------------------------------------------------------------------
;*****************************************************************************
;
; Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
;
; SPDX-License-Identifier: BSD-2-Clause-Patent
;******************************************************************************
;============================================================================

global  ASM_PFX(mSmiGdtr)

DEFAULT REL
SECTION .text

NullSeg: DQ 0                   ; reserved by architecture
;8
CodeSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x10
ProtModeCodeSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x18
ProtModeSsSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x20
DataSeg32:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x28
CodeSeg16:
            DW      -1
            DW      0
            DB      0
            DB      0x9b
            DB      0x8f
            DB      0
;0x30
DataSeg16:
            DW      -1
            DW      0
            DB      0
            DB      0x93
            DB      0x8f
            DB      0
;0x38
CodeSeg64:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xaf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x40
DataSeg64:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93                ; P = 1, DPL = 0, S = 1
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x48
CodeSegComR3:                           ; Placeholder, not used
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xfb                ; P = 1, DPL = 3
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x50
DataSeg64R3:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xf3                ; P = 1, DPL = 3
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x58
CodeSeg64R3:
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xfb                ; P = 1, DPL = 3
            DB      0xaf                ; LimitHigh
            DB      0                   ; BaseHigh
;0x60
CallGate64:
            DW      0                   ; TargetOffsetLow
            DW      0x38                ; TargetSelector
            DB      0                   ; Reserved
            DB      0xec                ; P = 1, DPL = 3, Type = 64 bit call gate
            DW      0                   ; TargetOffsetMid
            DD      0                   ; TargetOffsetHigh
            DD      0                   ; Reserved
;0x70
CallGate64Iret:
            DW      0                   ; TargetOffsetLow
            DW      0x38                ; TargetSelector
            DB      0                   ; Reserved
            DB      0xec                ; P = 1, DPL = 3, Type = 64 bit call gate
            DW      0                   ; TargetOffsetMid
            DD      0                   ; TargetOffsetHigh
            DD      0                   ; Reserved
;0x80
; TSS Segment for X64 specially
TssSeg:
            DW      TSS_DESC_SIZE       ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x89
            DB      0x80                ; LimitHigh
            DB      0                   ; BaseHigh
            DD      0                   ; BaseUpper
            DD      0                   ; Reserved
GDT_SIZE equ $ -   NullSeg

; Create TSS Descriptor just after GDT
TssDescriptor:
            DD      0                   ; Reserved
            DQ      0                   ; RSP0
            DQ      0                   ; RSP1
            DQ      0                   ; RSP2
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DQ      0                   ; IST1
            DQ      0                   ; IST2
            DQ      0                   ; IST3
            DQ      0                   ; IST4
            DQ      0                   ; IST5
            DQ      0                   ; IST6
            DQ      0                   ; IST7
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DW      0                   ; Reserved
            DW      0                   ; I/O Map Base Address
TSS_DESC_SIZE equ $ -   TssDescriptor


global ASM_PFX(InternalX86Retf)
ASM_PFX(InternalX86Retf):
    mov     RAX, 0
    mov     eax, cs
    push    RAX
    call    Base1                       ; push return address for retf later
Base1:
    add     dword [rsp], InternalX86RetfExit - Base1; offset for far retf, seg is the 1st arg
    DB      0x48              ; prefix to composite "retq" with next "retf"
    retf                      ; far return
InternalX86RetfExit:
    ret

;
; CODE & DATA segments for SMM runtime
;
CODE_SEL        equ   CodeSeg64 -   NullSeg
DATA_SEL        equ   DataSeg32 -   NullSeg
CODE32_SEL      equ   CodeSeg32 -   NullSeg
IRET_RET_SEL    equ   CallGate64Iret - NullSeg

;Note: All calls are relative to rip, and this function is copied to CPL3 memory, thus one can't refer to externals after copied.
;This will be copied to memory user page.
global ASM_PFX(Cpl3HandlerStub)
ASM_PFX(Cpl3HandlerStub):
    db 48h, 0b8h            ; mov     rax, 0
ASM_PFX(mOemExceptionHandlerFixUp):
    dq      0
    cmp     rax, 0
    je      .0

    sub     rsp, 4 * 8 + 8  ; Allocate maximum parameter stack space and align stack
    call    rax
    add     rsp, 4 * 8 + 8

.0:
    mov     eax, cs
    and     eax, 3h         ; Check CPL level.
    jnz     .1              ; Jmp if not CPL0
    ret
.1:
    ; Call gate
    mov     rax, IRET_RET_SEL
    shl     rax, 32
    push    rax
    call    far qword [rsp]
    jmp $   ; Will not return
global ASM_PFX(Cpl3HandlerEndStub)
ASM_PFX(Cpl3HandlerEndStub):

SECTION .data

ASM_PFX(mSmiGdtr):
    DW      GDT_SIZE - 1
    DQ        NullSeg


global ASM_PFX(mOemExceptionHandlerFixUpOffset)
ASM_PFX(mOemExceptionHandlerFixUpOffset):
    dw mOemExceptionHandlerFixUp  - Cpl3HandlerStub
