;*****************************************************************************
;
; Copyright 2020 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
;
; AMD is granting you permission to use this software and documentation (if
; any) (collectively, the "Materials") pursuant to the terms and conditions of
; the Software License Agreement included with the Materials.  If you do not
; have a copy of the Software License Agreement, contact your AMD
; representative for a copy.
;
; You agree that you will not reverse engineer or decompile the Materials, in
; whole or in part, except as allowed by applicable law.
;
; WARRANTY DISCLAIMER:  THE MATERIALS ARE PROVIDED "AS IS" WITHOUT WARRANTY OF
; ANY KIND.  AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY,
; INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT, THAT THE
; MATERIALS WILL RUN UNINTERRUPTED OR ERROR-FREE OR WARRANTIES ARISING FROM
; CUSTOM OF TRADE OR COURSE OF USAGE.  THE ENTIRE RISK ASSOCIATED WITH THE USE
; OF THE MATERIAL IS ASSUMED BY YOU.  Some jurisdictions do not allow the
; exclusion of implied warranties, so the above exclusion may not apply to
; You.
;
; LIMITATION OF LIABILITY AND INDEMNIFICATION:  AMD AND ITS LICENSORS WILL
; NOT, UNDER ANY CIRCUMSTANCES BE LIABLE TO YOU FOR ANY PUNITIVE, DIRECT,
; INCIDENTAL, INDIRECT, SPECIAL OR CONSEQUENTIAL DAMAGES ARISING FROM USE OF
; THE MATERIALS OR THIS AGREEMENT EVEN IF AMD AND ITS LICENSORS HAVE BEEN
; ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  In no event shall AMD's total
; liability to You for all damages, losses, and causes of action (whether in
; contract, tort (including negligence) or otherwise) exceed the amount of
; $100 USD. You agree to defend, indemnify and hold harmless AMD and its
; licensors, and any of their directors, officers, employees, affiliates or
; agents from and against any and all loss, damage, liability and other
; expenses (including reasonable attorneys' fees), resulting from Your use of
; the Materials or violation of the terms and conditions of this Agreement.
;
; U.S. GOVERNMENT RESTRICTED RIGHTS:  The Materials are provided with
; "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
; subject to the restrictions as set forth in FAR 52.227-14 and
; DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
; Government constitutes acknowledgment of AMD's proprietary rights in them.
;
; EXPORT RESTRICTIONS: The Materials may be subject to export restrictions as
; stated in the Software License Agreement.
;******************************************************************************

DEFAULT REL
SECTION .text

%define SMM_PROT_TEST_COMMON_SIG 12345678h

%define PROTECTED_INSTRURCTION_CHECK  0
%define DATA_READ_CHECK               1
%define EXECUTE_FUNCTION_CHECK        2

; This function is for testing instructions that may cause exceptions.
; After the exection the iret will return at AsmInterruptRet.
; This is required to maintain stack.

global ASM_PFX(AsmSmmProtectionsTestCommon)
ASM_PFX(AsmSmmProtectionsTestCommon):

  mov   r8, SMM_PROT_TEST_COMMON_SIG ;Test signature to check AsmInterruptRet is returned because of exception in this function.


  cmp   rcx, PROTECTED_INSTRURCTION_CHECK
  jne   .1

  mov   rax, cr3
  ret

.1:
  cmp   rcx, DATA_READ_CHECK
  jne   .2
  mov   rax, [rdx]
  ret

.2:
  cmp   rcx, EXECUTE_FUNCTION_CHECK
  jne   .3

  call  rdx
  ret

.3:
nop
; ...

; An exception will return to this function.
global ASM_PFX(AsmInterruptRet)
ASM_PFX(AsmInterruptRet):
  cmp r8, SMM_PROT_TEST_COMMON_SIG  ;Did this reach here from this function.
InfiniteLoop:
  jne InfiniteLoop  ; Exception not caused in AsmSmmProtectionsTestCommon function.
ret