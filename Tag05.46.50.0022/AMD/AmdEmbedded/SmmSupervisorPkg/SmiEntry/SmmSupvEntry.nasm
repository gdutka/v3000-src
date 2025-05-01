;------------------------------------------------------------------------------ ;
; Copyright (c) 2016 - 2019, Intel Corporation. All rights reserved.<BR>
; Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
; Copyright (C) Microsoft Corporation.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   SmiEntry.nasm
;
; Abstract:
;
;   Code template of the SMI handler for a particular processor
;
;-------------------------------------------------------------------------------

%include "StuffRsbNasm.inc"
%include "Nasm.inc"

;
; Variables referenced by C code
;
%define MSR_SMM_BASE  0xC0010111
; %define MSR_IA32_MISC_ENABLE 0x1A0
%define MSR_EFER      0xc0000080
%define MSR_EFER_XD   0x800

; MSR_CCD0LTHREE0CORE0THREAD0_STAR_ADDRESS
%define MSR_STAR_ADDRESS    0xc0000081
; MSR_CCD0LTHREE0CORE0THREAD0_STAR64_ADDRESS
%define MSR_STAR64_ADDRESS  0xc0000082

;
; Constants relating to PROCESSOR_SMM_DESCRIPTOR
;

;
; Constants relating to CPU State Save Area
;
%define SSM_DR6 0xffd0
%define SSM_DR7 0xffc8

%define PROTECT_MODE_CS 0x8
%define PROTECT_MODE_DS 0x20
%define LONG_MODE_CS 0x38
%define COMP_MODE_CS_R3 0x4B
%define LONG_MODE_DS_R3 0x53
%define LONG_MODE_CS_R3 0x5B
%define CALL_GATE 0x63
%define TSS_SEGMENT 0x80
%define GDT_SIZE 0x88
%define CALL_GATE_DESCRIPTOR_OFFSET 0x60
%define TSS_DESCRIPTOR_OFFSET 0x80
%define FX_SAVE_SIZE  0x200

%define SMM_SUPV_SMI_ENTRY_PARAS_SIZE                   0x80
%define SMM_SUPV_SMI_ENTRY_PARAS_CPUINDEX_OFFSET        0;
%define SMM_SUPV_SMI_ENTRY_PARAS_CPL0STACK_OFFSET       8;
%define SMM_SUPV_SMI_ENTRY_PARAS_CPL3STACK_OFFSET       16;
%define SMM_SUPV_SMI_ENTRY_PARAS_CPL3STACK_SIZE_OFFSET   24;
%define SMM_SUPV_SMI_ENTRY_PARAS_SMIENTRY_STRUCT_OFFSET    32;
%define SMM_SUPV_SMI_ENTRY_PARAS_CPL0_CALLGATE_OFFSET    40;
%define SMM_SUPV_SMI_ENTRY_PARAS_CPL0_STACK_POINT_OFFSET    48;

%define CPU_SMM_DEBUG_ENTRY_OFF 0
%define SMI_RENDEZVOUS_OFF 8
%define CPU_SMM_DEBUG_EXIT_OFF 16

    DEFAULT REL
    SECTION .text

BITS 16
_SmiEntryPoint:
    mov     edi, [cs:FixUp32_1 - _SmiEntryPoint + 0x8000]   ;EDI point to GDTR
o32 lgdt    [ds:edi]

    mov     bx, @ProtectedMode - _SmiEntryPoint + 0x8000
    mov     ax, PROTECT_MODE_CS
    mov     [cs:bx-0x2],ax
    mov     ecx, MSR_SMM_BASE
    rdmsr
    mov     edi, eax    ; EAX = SmmBase
    lea     eax, [edi + (@ProtectedMode - _SmiEntryPoint) + 0x8000]
    mov     [cs:bx-0x6],eax
    mov     ebx, cr0
    and     ebx, 0x9ffafff3
    or      ebx, 0x23
    mov     cr0, ebx
_PmJumpPointFixup:
    jmp     dword 0x0:0x0

BITS 32
@ProtectedMode:
; at this point SmBase is in edi
    mov     ax, PROTECT_MODE_DS
o16 mov     ds, ax
o16 mov     es, ax
o16 mov     fs, ax
o16 mov     gs, ax
o16 mov     ss, ax
    mov     esp, dword [FixUp32_3 - _SmiEntryPoint + 0x8000 + edi]               ; source operand will be populated at loading time
    mov     edx, [esp]             ; dx <- CpuIndex
    mov     esp, dword [FixUp32_5 - _SmiEntryPoint + 0x8000 + edi]               ; source operand will be populated at loading time
SpSetDone:
    jmp     ProtFlatMode


ProtFlatMode:
    mov     esi, FixUp32_2 - _SmiEntryPoint + 0x8000
    add     esi, edi
    mov     eax, [esi]               ; source operand will be populated at loading time
    mov     cr3, eax
    mov     eax, 0x668                   ; as cr4.PGE is not set here, refresh cr3

    mov     esi, FixUp32_4 - _SmiEntryPoint + 0x8000
    add     esi, edi
    bt      dword [esi], 0              ; Check if FIXUP32_CONFIG_5LEVEL_PAGE_TABLE bit has been set, caller set this bit by checking 5 Level Page support flag

    jnc     SET_CR4
    ;
    ; Enable 5-Level Paging bit
    ;

    bts     eax, 12                     ; Set LA57 bit (bit #12)
SET_CR4:
    mov     cr4, eax                    ; in PreModifyMtrrs() to flush TLB.
    push    edx
    mov     eax, 0x00000007
    xor     ecx, ecx
    cpuid
    pop     edx
    bt      ecx, 2                      ; Test bit 2 of ECX to check if UMIP is supported
    jnc     Load_TSS
    mov     ecx, cr4
    bts     ecx, 11
    mov     cr4, ecx                    ; Enable UMIP (bit 11 of CR4)
Load_TSS:
    sub     esp, 8                      ; reserve room in stack
    sgdt    [esp]
    mov     eax, [esp + 2]              ; eax = GDT base
    add     esp, 8
    mov     cl, 0x89
    cmp     byte [eax + TSS_SEGMENT + 5], cl
    jz      SkipClrBusyFlag;
    mov     [eax + TSS_SEGMENT + 5], cl ; clear busy flag
SkipClrBusyFlag:
    mov     eax, TSS_SEGMENT
    ltr     ax
; Always enable NXE
    mov     ecx, MSR_EFER
    push    edx
    rdmsr
    or      ax, MSR_EFER_XD            ; enable NXE
    wrmsr
    pop     edx

; Switch into @LongMode
    push    LONG_MODE_CS                ; push cs hardcore here
    call    Base                       ; push return address for retf later
Base:
    add     dword [esp], @LongMode - Base; offset for far retf, seg is the 1st arg

    push    edx
    mov     ecx, MSR_EFER
    rdmsr
    or      ah, 1                      ; enable LME
    or      al, 1                      ; enable SCE
    wrmsr
    pop     edx
    mov     ebx, cr0
    or      ebx, 0x80010023            ; enable paging + WP + NE + MP + PE
    mov     cr0, ebx
    retf
@LongMode:                              ; long mode (64-bit code) starts here
BITS 64
; at this point SmBase is in rdi
    mov     rax, [FixUp64_5 - _SmiEntryPoint + 0x8000 + rdi]     ;  mov     rax, ASM_PFX(gSmiHandlerIdtr)
    lidt    [rax]
    ; Use one GDT entry 0x20 for ds, es, fs, gs, ss in long mode
    mov     ax, PROTECT_MODE_DS
    mov     ds, eax
    mov     es, eax
    mov     fs, eax
    mov     gs, eax
    mov     ss, eax

    ;
    ; Save FP registers
    ;
    ;RSP need 0x10 aligned or else GP#
    sub     rsp, 8
    sub     rsp, FX_SAVE_SIZE
    fxsave64 [rsp]

    sub     rsp, 8  ; RSP need 0x10 aligned or else GP# in "movaps [rsp+60h],xmm6"
    push    rdx     ; save CpuIndex to CPL0 stack, it will be restored to rbx in SmiExit for both direct ring 0 and returned from ring 3
    ; the rsp here will be saved as Cpl0StackPoint for CPL0 call gate

    ; Call SmmSupvSmiEntry to perform the security check for GDT, page table and other security concerned setting
    ; Prepare below input parameter
    ;typedef struct {
    ;UINT64                     CpuIndex;
    ;UINT64                     Cpl0Stack;
    ;UINT64                     Cpl3Stack;
    ;UINT64                     Cpl3StackSize;
    ;UINT64                     PerCoreSmiEntryStructAddr;
    ;UINT64                     Cpl0CallGateOffset;
    ;UINT64                     Cpl0StackPoint;
    ;} SMM_SUPV_SMI_ENTRY_PARAS;
    ; Reserve the parameter in stack, to hold input parameters SMM_SUPV_SMI_ENTRY_PARAS
    ; NOTE sizeof (SMM_SUPV_SMI_ENTRY_PARAS) should not exceed SMM_SUPV_SMI_ENTRY_PARAS_SIZE
    push    rdx  ; save CpuIndex to CPL0 stack, it will be transferred to SmiRendezvousEntry
    sub     rsp, SMM_SUPV_SMI_ENTRY_PARAS_SIZE

    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_CPUINDEX_OFFSET], rdx    ;rdx = CpuIndex

    mov     rax, 0
    mov     eax, dword [FixUp32_5 - _SmiEntryPoint + 0x8000 + rdi]   ;Cpl0Stack
    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_CPL0STACK_OFFSET], rax    ;rax = Cpl0Stack

    mov     eax,  dword [FixUp32_3 - _SmiEntryPoint + 0x8000 + rdi] ;Cpl3Stack
    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_CPL3STACK_OFFSET], rax    ;rax = Cpl3Stack

    mov     eax,  dword [FixUp32_6 - _SmiEntryPoint + 0x8000 + rdi] ;Cpl3StackSize
    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_CPL3STACK_SIZE_OFFSET], rax    ;rax = Cpl3StackSize

    lea     rax, [PerCoreSmiEntryStruct  - _SmiEntryPoint + 0x8000 + rdi] ; PerCoreSmiEntryStructAddr
    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_SMIENTRY_STRUCT_OFFSET], rax    ;rax = PerCoreSmiEntryStructAddr

    lea     rax, [LongModeR0  - _SmiEntryPoint + 0x8000 + rdi] ; Cpl0CallGateOffset
    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_CPL0_CALLGATE_OFFSET], rax    ;rax = Cpl0CallGateOffset

    mov     rax, rsp ; Cpl0StackPoint
    add     rax, SMM_SUPV_SMI_ENTRY_PARAS_SIZE + 0x8    ;    add     rsp, SMM_SUPV_SMI_ENTRY_PARAS_SIZE ;pop     rdx
    mov     [rsp + SMM_SUPV_SMI_ENTRY_PARAS_CPL0_STACK_POINT_OFFSET], rax    ;rax = Cpl0StackPoint

    mov     rax, [FixUp64_0 - _SmiEntryPoint + 0x8000 + rdi]    ; point to SmmSupvSmiEntry
    ; x64 calling convention Integer  First 4 parameters - RCX, RDX, R8, R9. Others passed on stack.
    mov     rcx, rsp
    ; The caller reserves space on the stack for arguments passed in registers.
    ; The called function can use this space to spill the contents of registers to the stack.
    sub     rsp, 0x48  ; need to be 16bytes aligned if Turns on optimizations or will have #GP in "movdqa [rsp+40h],xmm0"
    call    rax        ; call SmmSupvSmiEntry
    add     rsp, 0x48
    add     rsp, SMM_SUPV_SMI_ENTRY_PARAS_SIZE
    pop     rdx

    ; Determine which path we need to take from here (Setup call gate, jump to ring 0 or ring 3, etc.)
    and     rax, 0xFFFFFF               ; Check ready to lock and DRTM flag returned
    cmp     rax, 0xFFFFFF               ; First check if post DRTM
    je      SwitchRing3                 ; If post ready to lock, need ring transition
    cmp     al, 0xFF                    ; Then check if in ready to lock (regardless pre or post this event)
    je      DirectRing0                 ; If even before lock, need to setup GDT, otherwise, need to go to ring 0 code directly
    ;
    ;   Codes before SMM ready to lock
    ;
DirectRing0:
    ; Pre ready to lock, no need to switch to ring 3
    lea     r15, [SmiExit]                                       ; R15 = return point to SmiEntry, RDX = CpuIndex
    mov     rsi, [FixUp64_2 - _SmiEntryPoint + 0x8000 + rdi]     ; rsi point to SmiRendezvousEntry in PiSmmCpuDxeSmm driver
    jmp     rsi                                                  ; jmp SmiRendezvousEntry
; Switch to 64 bit ring 3
; Initial DS, ES, FS, GS Segment Selectors, RPL set to 3
SwitchRing3:
    mov     ax, LONG_MODE_DS_R3
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    mov     rax, 0
    mov     eax, [FixUp32_3 - _SmiEntryPoint + 0x8000 + edi]     ;   Cpl3Stack
    mov     rsi, [FixUp64_2 - _SmiEntryPoint + 0x8000 + rdi]     ;   SmiRendezvousEntry
    mov     r15, CALL_GATE                                       ;   R15 filled with call gate used for CPL3 return back to CPL0
    shl     r15, 32

; Prepare context for ring 3
    push    LONG_MODE_DS_R3     ; SS
    push    rax                 ; RSP
    push    LONG_MODE_CS_R3     ; CS
    push    rsi                 ; SmiRendezvousEntry
;Use retf instead iretq to fix NMI issue
;An SMM handler can unmask NMI interrupts by simply executing an IRET.
;Upon completion of the IRET instruction, the processor recognizes the pending NMI,
;and transfers control to the NMI handler. Once an NMI is recognized
;within SMM using this technique, subsequent NMIs are recognized until SMM is exited.

    DB      0x48              ; prefix to composite "retq" with next "retf"
    retf                      ; far return

LongModeR0:                   ; long mode (64-bit code) ring 0 starts here
    add     rsp, 0x20
    mov     ax, PROTECT_MODE_DS
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax

SmiExit:
    pop     rbx         ; restore CpuIndex from CPL0 stack
    add     rsp, 8
     ; get SMM base again as rdi might be updated in ring 3
    mov     ecx, MSR_SMM_BASE
    rdmsr
    mov     edi, eax    ; EAX = SmmBase
    and     rdi, 0xFFFFFFFF

    ; Call SmmSupvSmiExit to perform register restore
    mov     rcx, rbx    ;RCX = CpuIndex, Arg 0 for SmmSupvSmiEntry
    mov     rax, [FixUp64_1 - _SmiEntryPoint + 0x8000 + rdi]
    call    rax         ; Call SmmSupvSmiExit
    ;RAX contain return status, check if required
    fxrstor64 [rsp]
    add     rsp, FX_SAVE_SIZE
    StuffRsb64
    rsm

gcSmiHandlerSize    DW      $ - _SmiEntryPoint

;Below structure could be used during DRTM event, make sure it is aligned
ALIGN   8
PerCoreSmiEntryStruct:
;--------------    Header version
HeaderVersion         DD    4

FixUpStructOffset     DB    FixUpOffset0Size - PerCoreSmiEntryStruct
FixUpStructNum        DB    4

FixUp64Offset         DB    FixUp64_0 - PerCoreSmiEntryStruct
FixUp64Num            DB    7

FixUp32Offset         DB    FixUp32_0 - PerCoreSmiEntryStruct
FixUp32Num            DB    7

;--------------    SmiEntry binary version
; Version = Major.Minor
; Major: higher 8 bits of SmiEntryVersion; Minor: lower 8 bits of SmiEntryVersion;
; For example, 0x010F will be split into 0x01 (Major version) and 0x0F (Minor version), the final version will be showed as 1.F (hex to upper char)
SmiEntryVersion       DW    0x0105
SplValue              DD    1
Reserved              DD    0
;--------------     #define MAXNUM_FIXUP_OFFSET 4
;--------------     FIXUP_OFFSET FixupOffset[MAXNUM_FIXUP_OFFSET];---------
FixUpOffset0Size      DW    6
FixUpOffset0          DW    _PmJumpPointFixup + 2

FixUpOffset1Size      DW    0
FixUpOffset1          DW    0

FixUpOffset2Size      DW    0
FixUpOffset2          DW    0

FixUpOffset3Size      DW    0
FixUpOffset3          DW    0

;--------------     #define MAXNUM_NUM_FIXUP64 7
;--------------     UINT64   FixUp64 [MAXNUM_FIXUP_64]; ---------
FixUp64_0            DQ    0  ; FIXUP64_SMMSUPV_SMIENTRY point to SmmSupvSmiEntry
FixUp64_1            DQ    0  ; FIXUP64_SMMSUPV_SMIEXIT point to SmmSupvSmiExit
FixUp64_2            DQ    0  ; FIXUP64_SMI_RDZ_ENTRY point to SmiRendezvousEntry
FixUp64_3            DQ    0  ; FIXUP64_OFFSET3_RSVD
FixUp64_4            DQ    0  ; FIXUP64_OFFSET4_RSVD
FixUp64_5            DQ    0  ; FIXUP64_SMI_HANDLER_IDTR SmiHandlerIdtr
FixUp64_6            DQ    0  ; FIXUP64_OFFSET6_RSVD

;--------------     #define MAXNUM_NUM_FIXUP32 7
;--------------     UINT32   FixUp32[MAXNUM_FIXUP_VARIABLE]; ---------
FixUp32_0        DD    0  ; FIXUP32_DRTM_BOOT_FLAG
FixUp32_1        DD    0  ; FIXUP32_GDTR
FixUp32_2        DD    0  ; FIXUP32_CR3_OFFSET
FixUp32_3        DD    0  ; FIXUP32_STACK_OFFSET_CPL3
FixUp32_4        DD    0  ; FIXUP32_CONFIG_OFFSET
FixUp32_5        DD    0  ; FIXUP32_STACK_OFFSET_CPL0
FixUp32_6        DD    0  ; FIXUP32_STACK_SIZE_CPL3

Size             DD    $ - PerCoreSmiEntryStruct
