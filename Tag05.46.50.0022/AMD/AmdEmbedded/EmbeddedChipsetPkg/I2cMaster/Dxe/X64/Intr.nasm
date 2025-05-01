
SECTION .text

;--------------------------------------------------------------
; VOID 
; LocalIrqSave(
;   OUT UINT32 * eflags // rcx
;   );
;
;
; PROC: the compiler will automatically insert prolog and epilog
;--------------------------------------------------------------
global ASM_PFX(LocalIrqSave)
ASM_PFX(LocalIrqSave):
;  mov      rcx, eflagsPtr
  pushfq
  pop      rax
  mov      QWORD [rcx], rax
  cli
  ret
 
;--------------------------------------------------------------
; VOID 
; LocalIrqRestore(
;   IN UINT32  eflags // rcx
;   );
;
;--------------------------------------------------------------

global ASM_PFX(LocalIrqRestore)
ASM_PFX(LocalIrqRestore):
  mov      rax, rcx
  push     rax
  popfq
  ret

