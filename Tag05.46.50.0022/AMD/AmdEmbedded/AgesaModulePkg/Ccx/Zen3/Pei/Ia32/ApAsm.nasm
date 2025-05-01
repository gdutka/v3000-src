;****************************************************************************
; Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
;
;****************************************************************************
;/**
; * @file
; *
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project:  Ccx
; * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
; */

SECTION .text

BspMsrLocationOffset                    EQU 0
ApSyncLocationOffset                    EQU 4
AllowToLaunchNextThreadLocationOffset   EQU 8
ApStackBasePtrOffset                    EQU 0Ch
ApGdtDescriptorOffset                   EQU 10h

AP_STACK_SIZE                           EQU 200h

extern ASM_PFX(RegSettingBeforeLaunchingNextThread)
extern ASM_PFX(ApEntryPointInC)

global ASM_PFX(ApAsmCode)
ASM_PFX(ApAsmCode):
  ; NOTE: EDI points to ApLaunchGlobalData
  mov ax, 18h
  mov ds, ax
  mov es, ax
  mov ss, ax

  ; Reset ESP
  mov eax, 1
  cpuid
  shr ebx, 24

  ; Use 1-based APIC ID to index into the top of this AP's stack
  xor eax, eax
  mov eax, ebx
  mov ecx, AP_STACK_SIZE
  mul ecx

  ; Make space for the first qword
  sub eax, 8

  mov esi, [edi + ApStackBasePtrOffset]
  add eax, esi
  mov esp, eax

  ; Enable Fixed MTRR modification
  mov ecx, 0C0010010h
  rdmsr
  or  eax, 00080000h
  wrmsr

  ; Setup MSRs to BSP values
  mov esi, [edi + BspMsrLocationOffset]
MsrStart:
  mov ecx, [esi]
  cmp ecx, 0FFFFFFFFh
  jz MsrDone
  add esi, 4
  mov eax, [esi]
  add esi, 4
  mov edx, [esi]
  wrmsr
  add esi, 4
  jmp MsrStart

MsrDone:
  ; Disable Fixed MTRR modification and enable MTRRs
  mov ecx, 0C0010010h
  rdmsr
  and eax, 0FFF7FFFFh
  or  eax, 00140000h
  bt  eax, 21
  jnc Tom2Disabled
  bts eax, 22
Tom2Disabled:
  wrmsr

  ; Enable caching
  mov eax, cr0
  btr eax, 30
  btr eax, 29
  mov cr0, eax

  ; Call into C code before next thread is launched
  push edi
  call ASM_PFX(RegSettingBeforeLaunchingNextThread)
  pop edi

  ; Call into C code
  push edi
  call ASM_PFX(ApEntryPointInC)
  pop edi

  ; Increment call count to allow to launch next thread
  mov esi, [edi + AllowToLaunchNextThreadLocationOffset]
  lock inc WORD [esi]

  ; Set up resident GDT
  mov esi, ApGdtDescriptorOffset
  add esi, edi
  lgdt [esi]
  ; Use stack base as a long jump pointer buffer
  mov esi, [edi + ApStackBasePtrOffset]
  ; Update selector
  mov WORD [esi + 4], 0010h
  mov ebx, NewGdtAddress
  mov [esi], ebx
  jmp far [esi]
NewGdtAddress:

  ; Increment call count to indicate core is done running
  mov esi, [edi + ApSyncLocationOffset]
  lock inc WORD [esi]

  ; Hlt
Hlt_loop:
  cli
  hlt
  jmp Hlt_loop

