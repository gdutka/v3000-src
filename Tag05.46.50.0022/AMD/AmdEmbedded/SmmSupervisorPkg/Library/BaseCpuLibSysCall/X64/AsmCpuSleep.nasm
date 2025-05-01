;------------------------------------------------------------------------------ ;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; Copyright (C), Microsoft Corporation.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   CpuSleep.Asm
;
; Abstract:
;
;   CpuSleep function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; AsmCpuSleep (
;   VOID
;   );
;------------------------------------------------------------------------------
global ASM_PFX(AsmCpuSleep)
ASM_PFX(AsmCpuSleep):
    hlt
    ret

