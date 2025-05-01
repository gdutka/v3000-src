;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; Copyright (C) Microsoft Corporation.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   DisableInterrupts.Asm
;
; Abstract:
;
;   DisableInterrupts function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; AsmDisableInterrupts (
;   VOID
;   );
;------------------------------------------------------------------------------
global ASM_PFX(AsmDisableInterrupts)
ASM_PFX(AsmDisableInterrupts):
    cli
    ret

