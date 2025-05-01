;;******************************************************************************
;;* Copyright (c) 1983-2017, Insyde Software Corporation. All Rights Reserved.
;;*
;;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;;* transmit, broadcast, present, recite, release, license or otherwise exploit
;;* any part of this publication in any form, by any means, without the prior
;;* written permission of Insyde Software Corporation.
;;*
;;******************************************************************************

        INCLUDE AmdUefiStack.inc
        INCLUDE SecCore.inc
        INCLUDE SecPlatformInit.inc

        .686p
        .mmx
        .model small, c

        PUBLIC  TopOfCar
        EXTRN   SecStartup:NEAR
        EXTRN   OemSvcHookBeforePeiCoreEntryPoint:NEAR
        EXTRN   PcdGet64 (PcdPciExpressBaseAddress):QWORD
;[-start-220126-IB14740206-modify];
        EXTRN   PcdGet32 (PcdPciExpressSizeMsrValue):DWORD
;[-end-220126-IB14740206-modify];
        EXTRN   PcdGet32 (PcdFlashFvRecoverySize):DWORD
        EXTRN   PcdGet32 (PcdFlashFvRecoveryBase):DWORD
        EXTRN   PcdGet32(PcdH2OBootFirmwareVolumeBase):DWORD
        EXTRN   PcdGet32 (PcdFlashAreaBaseAddress):DWORD
        EXTRN   NonSmmS3ResumeInit:NEAR
        EXTRN   PcdGetBool (PcdAmdPspS3WakeFromSmm):BYTE

        SMM_RESUME_SIGNATURE  = 055AABB66h ;When SMM rsm here on resume take alternate path
        CAPSULE_SIGNATURE     = 0CA9501EFh
        FLASH_REGION_START    = 1000000h
        BSP_STACK_BASE        = 100000h

_TEXT_REALMODE      SEGMENT PARA PUBLIC USE16 'CODE'
                    ASSUME  CS:_TEXT_REALMODE, DS:_TEXT_REALMODE

;------------------------------------------------------------------------------
;
;  SEC "Security" Code module.
;
;  Transition to non-paged flat-model protected mode from a
;  hard-coded GDT that provides exactly two descriptors.
;  This is a bare bones transition to protected mode only
;  used for while in PEI and possibly DXE.
;
;  IA32 specific cache as RAM modules
;
;  After enabling protected mode, a far jump is executed to
;  TransferToPEI using the newly loaded GDT.
;  This code also enables the Cache-as-RAM
;
;  RETURNS:    none
;
;------------------------------------------------------------------------------

align 4

;; Offset 0xFFFFFFF0 (reset address has code below
;; nop; nop; jmp Flat32Start
;; So we can read offset 0xFFFFFFF3 to find the location of Flat32Start
;; Subtract that with 12 we will know this SMMResumeInfo location
SMMResumeInfo: ;; This offset can bie found as 0xFFFFFFF5 + word [ 0xFFFFFFF3] - 12
      DD OFFSET BootGDTtable            ; GDT base address
      DW LINEAR_CODE_SEL                ; code segment
      DW SYS_DATA_SEL                   ; data segment
      DD OFFSET S3ResumeEntryPoint      ; Offset of our 32 bit code
      DD SMM_RESUME_SIGNATURE

Flat32Start         PROC NEAR C PUBLIC

        ;
        ; Save BIST state in EAX
        ;
        mov     ebp,  eax

        ;
        ; Enable LBR
        ;
       ;;; mov     ecx, 1D9h       ; DBG_CTL_MSR
       ;;; rdmsr
       ;;; or      eax, 01h        ; LBR
       ;;; wrmsr

        ;
        ; BspApCheck
        ;
        mov     ecx, APIC_BASE_ADDRESS  ; MSR:0000_001B
        rdmsr
        bt      eax, APIC_BSC           ; Is this the BSC?
        .if (!carry?)
           ; This is AP
           jmp     Ap_skip_1M_ROM
        .endif

        SEC_PLATFORM_ENABLE_DEBUG_PORT_MACRO

        POSTCODE (0201h);SEC_SYSTEM_POWER_ON

        SEC_PLATFORM_INIT_STAGE1_MACRO

Ap_skip_1M_ROM:
        ;
        ; Switch to Protected mode.
        ;
        mov     esi,  OFFSET GdtDesc
        DB      66h
        lgdt    fword ptr cs:[si]
        mov     eax, cr0                           ; Get control register 0
        or      eax, 00000003h                     ; Set PE bit (bit #0) & MP bit (bit #1)
        mov     cr0, eax                           ; Activate protected mode
        mov     eax, cr4                           ; Get control register 4
        or      eax, 00000600h                     ; Set OSFXSR bit (bit #9) & OSXMMEXCPT bit (bit #10)
        mov     cr4, eax

        ;
        ; Now we're in Protected16
        ; Set up the selectors for protected mode entry
        ;
        mov     ax, SYS_DATA_SEL
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax

        ;
        ; Go to Protected32
        ;
        mov esi, offset NemInitLinearAddress
        jmp     fword ptr cs:[si]
Flat32Start         ENDP
_TEXT_REALMODE      ENDS

_TEXT_PROTECTED_MODE      SEGMENT PARA PUBLIC USE32 'CODE'
                          ASSUME  CS:_TEXT_PROTECTED_MODE, DS:_TEXT_PROTECTED_MODE

ProtectedModeEntryPoint PROC NEAR PUBLIC

        jmp     InitializeNem

ProtectedModeEntryPoint ENDP

InitializeNem   PROC    NEAR    PRIVATE

        ;
        ; Determine processor family
        ;
        mov     eax, 1                           ; Get version information
        cpuid                                    ;
        mov     ebx, eax                         ; Save version information in eax

;        mov     ecx, NB_CFG
;        rdmsr
;        or      edx, bEnableCF8ExtCfg
;        wrmsr

        ; using Pcd instead
        mov     eax, DWORD PTR PcdGet64 (PcdPciExpressBaseAddress)
;[-start-220126-IB14740206-modify];
        mov     ecx, PcdGet32 (PcdPciExpressSizeMsrValue)
        or      eax, ecx
;[-end-220126-IB14740206-modify];

SetPcieMmioMsr:
        xor     edx, edx
        mov     ecx, 0C0010058h
        wrmsr

        ;
        ; Passing parameters into AMD_ENABLE_UEFI_STACK2
        ;
        xor     eax, eax
        mov     eax, ebp
        xor     ecx, ecx
        mov     ecx, FLASH_REGION_START
        xor     edx, edx
        mov     edx, PcdGet32 (PcdFlashAreaBaseAddress)
        or      edx, 0FF000000h ; cache 16M maximum
        AMD_ENABLE_UEFI_STACK2 STACK_AT_TOP, _PCD_VALUE_PcdBspStackSize, BSP_STACK_BASE

CAR_TEST_KEY    EQU     012345678h
        xor     eax, eax
        push    CAR_TEST_KEY
        pop     eax
        cmp     eax, CAR_TEST_KEY
        je      CarTestOk
        mov     al, 0C2h
        out     80h, al
        jmp $
CarTestOk:
        ;
        ; BspApCheck
        ;
        mov     ecx, APIC_BASE_ADDRESS  ; MSR:0000_001B
        rdmsr
        bt      eax, APIC_BSC           ; Is this the BSC?
        .if (!carry?)
           ; This is AP
           jmp     StartUpAp
        .endif

        POSTCODE (0207h);SEC_SETUP_CAR_OK

        jmp     CallPeiCoreEntryPoint

InitializeNem   ENDP

S3ResumeEntryPoint PROC NEAR  PRIVATE

;[-start-220221-IB14740216-add];
  ;
  ; Clear dr0 and dr7 when import InsydePostCodePkg
  ;
  push    eax
  xor     eax, eax
  mov     dr7, eax
  mov     dr0, eax
  pop     eax
;[-end-220221-IB14740216-add];

  ;
  ; S3 resume path
  ;
  cmp     ds:BYTE PTR PcdGetBool (PcdAmdPspS3WakeFromSmm), 1h
  jne     NonSmmS3Resume

  push    eax            ; Address of mPspSmmRsmMemInfo, initialized by PSP SMM Resume code

  ; Clear Long Mode Enable when returned from S3 resume.
  mov     ecx, 0c0000080h; EFER MSR number.
  rdmsr                  ; Read EFER.
  btr     eax, 8         ; Set LME=0
  wrmsr                  ; Write EFER.
  mov     ecx, 0c0010113h ; SMMMask
  rdmsr
  btr     eax, 0          ; Set AValid=0
  btr     eax, 1          ; Set TValid=0
  wrmsr
  pop     eax
  jmp     S3EntryCommon

NonSmmS3Resume:
  ;Open TSEG Region
  mov     ecx, MSR_SMMMASK_ADDRESS
  rdmsr
  btr     eax, B_TVALID
  wrmsr

  ; Get TSEG base and PSP_SMM_HDR_DATA
  mov     ecx, MSR_SMMADDR_ADDRESS
  rdmsr   ; eax = base = PSP_SMM_HDR_DATA

  mov     esi, eax

  mov     esp, dword ptr [esi].PSP_SMM_HDR_DATA.ApStackTop ;Default AP. Will re-init for BSP.
  mov     ecx, 1Bh
  rdmsr
  bt      eax, 8
  jnc     @f

  mov     esp, dword ptr [esi].PSP_SMM_HDR_DATA.PspSmmRsmMemInfo.StackPtr
  add     esp, [esi].PSP_SMM_HDR_DATA.PspSmmRsmMemInfo.BspStackSize

@@:
  ; Do CCX Init for non S3.
  push    esi  ; Pass in PSP_SMM_HDR_DATA to function.
  call    NonSmmS3ResumeInit
  pop     edx  ; adjust stack

  ; Setup eax, ebx entry for S3EntryCommon
  lea     eax, [esi].PSP_SMM_HDR_DATA.PspSmmRsmMemInfo
  mov     ebx, [esi].PSP_SMM_HDR_DATA.CapuseUpdateSignature

    ; Check for BSP
  push    eax
  mov     ecx, 1Bh
  rdmsr
  bt      eax, 8
  pop     eax
  jc      S3EntryCommon ; If yes, jump to Common
  lock inc dword ptr [esi].PSP_SMM_HDR_DATA.ApSyncFlag
  jmp  S3EntryCommon
S3ResumeEntryPoint ENDP

S3EntryCommon Proc
  ; registers pushed for call at end of proc.
  ;push    ebx            ; Push the CAPSULE_SIGNATURE
  push    eax            ; Address of mPspSmmRsmMemInfo, initialized by PSP SMM Resume code

  cmp     ebx, CAPSULE_SIGNATURE
  jnz     S3_resume

  ; On capsule path, set SMM Base to default
  mov     ecx, 0C0010111h
  rdmsr
  xor     edx, edx
  mov     eax, 30000h
  wrmsr

  ;Update SMM_MASK
  mov     ecx, 0C0010113h
  rdmsr
  xor     edx, edx
  and     eax, 0fffffffch ; Clear AVaild and TValid
  wrmsr

S3_resume:
  ; Check for BSP
  mov     ecx, 1Bh
  rdmsr
  bt      ax, 8
  jnc     AP_StarupCode

  SEC_PLATFORM_ENABLE_DEBUG_PORT_MACRO
  POSTCODE (0201h);SEC_SYSTEM_POWER_ON
  SEC_PLATFORM_INIT_STAGE1_MACRO

  pop eax  ; Address of mPspSmmRsmMemInfo, initialized by PSP SMM Resume code

  push eax ; Address of mPspSmmRsmMemInfo
  push ebx ; Capsule update signature

  mov edi, PcdGet32(PcdH2OBootFirmwareVolumeBase)
  push edi
  ;
  ; Pass stack base into the PEI Core
  ;
  mov  edi, eax
  push DWORD PTR ds:[edi+08h]

  ;
  ; Pass stack size into the PEI Core
  ;
  push    _PCD_VALUE_PcdBspStackSize
  ;
  ; Pass Control into the PEI Core
  ;
  POSTCODE (0209h);SEC_GO_TO_SECSTARTUP
  call SecStartup
  ;
  ; PEI Core should never return to here, this is just to capture an invalid return.
  ;
  jmp     $

AP_StarupCode:
  cmp     ebx, CAPSULE_SIGNATURE
  jz      Skip_SmmLock

;  mov     ecx, HWCR ; Lock AP's SMM in S3 resume
;  rdmsr
;  or      eax, SMMLOCK
;  wrmsr

Skip_SmmLock:
@@:
  cli
  hlt
  jmp @B
S3EntryCommon ENDP

CallPeiCoreEntryPoint   PROC    NEAR    PRIVATE
        ;
        ; Call SEC OEM Function
        ;
        CALL    OemSvcHookBeforePeiCoreEntryPoint
        cmp     al, 0                         ; Return status = Sucess
        je      TransferToSecStartup
        ;
        ; Push CPU count to stack first, then AP's (if there is one)
        ; BIST status, and then BSP's
        ;
        mov     eax, 1
        cpuid
        shr     ebx, 16
        and     ebx, 0000000FFh
        push    ebx
        cmp     bl, 1
        jbe     PushBspBist

;         mov     ecx, MTRR_PHYS_MASK_1
;         rdmsr
;         shr     eax, 12
;         push    ax
;         mov     ecx, MTRR_PHYS_BASE_1
;         rdmsr
;         shr     eax, 12
;         push    ax

PushBspBist:
        push    ebp

        ;
        ; Pass BFV into the PEI Core
        ;
;        mov  edi, 0FFFFFFFCh
;        push DWORD PTR ds:[edi]
        mov edi, PcdGet32(PcdH2OBootFirmwareVolumeBase)
        push edi

        ;
        ; Pass stack base into the PEI Core
        ;
        push    BSP_STACK_BASE

        ;
        ; Pass stack size into the PEI Core
        ;
        push    _PCD_VALUE_PcdBspStackSize
        ;
        ; Pass Control into the PEI Core
        ;
        POSTCODE (0209h);SEC_GO_TO_SECSTARTUP
TransferToSecStartup:
        call SecStartup
CallPeiCoreEntryPoint   ENDP

StartUpAp       PROC    NEAR    PUBLIC




        ;
        ; Get ApInit Address from CMOS
        ;
        mov     ah, 0A0h ; AGESA_CPU_INIT
        mov     ecx, 04h
GetApInitAddressLoop:
        mov     al, ah
        out     072h, al
        inc     ah
        out     0edh, al
        in      al, 073h
        mov     bl, al
        ror     ebx, 08h
        loop    GetApInitAddressLoop

        ; Go AP Init
        push    dword ptr 0     ; IN EFI_PEI_SERVICES           **PeiServices
        call    ebx             ; AmdCpuInitialize

StartUpAp       ENDP

align 10h
                    PUBLIC  BootGDTtable

;
; GDT[0]: 0x00: Null entry, never used.
;
NULL_SEL            equ     $ - GDT_BASE               ; Selector [0]
GDT_BASE:
BootGDTtable        DD      0
                    DD      0
;
; Linear data segment descriptor
;
LINEAR_SEL          equ     $ - GDT_BASE               ; Selector [0x8]
                    DW      0FFFFh                     ; limit 0xFFFFF
                    DW      0                          ; base 0
                    DB      0
                    DB      092h                       ; present, ring 0, data, expand-up, writable
                    DB      0CFh                       ; page-granular, 32-bit
                    DB      0
;
; Linear code segment descriptor
;
LINEAR_CODE_SEL     equ     $ - GDT_BASE               ; Selector [0x10]
                    DW      0FFFFh                     ; limit 0xFFFFF
                    DW      0                          ; base 0
                    DB      0
                    DB      09Bh                       ; present, ring 0, data, expand-up, not-writable
                    DB      0CFh                       ; page-granular, 32-bit
                    DB      0
;
; System data segment descriptor
;
SYS_DATA_SEL        equ     $ - GDT_BASE               ; Selector [0x18]
                    DW      0FFFFh                     ; limit 0xFFFFF
                    DW      0                          ; base 0
                    DB      0
                    DB      093h                       ; present, ring 0, data, expand-up, not-writable
                    DB      0CFh                       ; page-granular, 32-bit
                    DB      0

;
; System code segment descriptor
;
SYS_CODE_SEL        equ     $ - GDT_BASE               ; Selector [0x20]
                    DW      0FFFFh                     ; limit 0xFFFFF
                    DW      0                          ; base 0
                    DB      0
                    DB      09Ah                       ; present, ring 0, data, expand-up, writable
                    DB      0CFh                       ; page-granular, 32-bit
                    DB      0
;
; Spare segment descriptor
;
SYS16_CODE_SEL      equ     $ - GDT_BASE               ; Selector [0x28]
                    DW      0FFFFh                     ; limit 0xFFFF
                    DW      0                          ; base 0xF000
                    DB      0Fh
                    DB      09Bh                       ; present, ring 0, code, expand-up, writable
                    DB      00h                       ; byte-granular, 16-bit
                    DB      0
;
; Spare segment descriptor
;
SYS16_DATA_SEL      equ     $ - GDT_BASE               ; Selector [0x30]
                    DW      0FFFFh                     ; limit 0xFFFF
                    DW      0                          ; base 0
                    DB      0
                    DB      093h                       ; present, ring 0, data, expand-up, not-writable
                    DB      00h                        ; byte-granular, 16-bit
                    DB      0

;
; Spare segment descriptor
;
SPARE5_SEL          equ     $ - GDT_BASE               ; Selector [0x38]
                    DW      0                          ; limit 0
                    DW      0                          ; base 0
                    DB      0
                    DB      0                          ; present, ring 0, data, expand-up, writable
                    DB      0                          ; page-granular, 32-bit
                    DB      0
GDT_SIZE            EQU     $ - BootGDTtable            ; Size, in bytes

GdtDesc:                                                ; GDT descriptor
OffsetGDTDesc       EQU     $ - Flat32Start
                    DW      GDT_SIZE - 1                ; GDT limit
                    DD      OFFSET BootGDTtable ; GDT base address

NemInitLinearAddress   LABEL   FWORD
NemInitLinearOffset    LABEL   DWORD
                    DD      OFFSET ProtectedModeEntryPoint              ; Offset of our 32 bit code
                    DW      LINEAR_CODE_SEL

TopOfCar            DD      BSP_STACK_BASE + _PCD_VALUE_PcdBspStackSize -4
_TEXT_PROTECTED_MODE    ENDS
        END

