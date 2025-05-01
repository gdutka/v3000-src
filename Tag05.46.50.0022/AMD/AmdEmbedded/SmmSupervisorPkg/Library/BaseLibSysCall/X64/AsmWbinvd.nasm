;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; Copyright (C), Microsoft Corporation.
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   AsmWbinvd.Asm
;
; Abstract:
;
;   AsmWbinvdCPL0 function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; AsmWbinvdCPL0 (
;   VOID
;   );
;------------------------------------------------------------------------------
global ASM_PFX(AsmWbinvdCPL0)
ASM_PFX(AsmWbinvdCPL0):
    wbinvd
    ret

