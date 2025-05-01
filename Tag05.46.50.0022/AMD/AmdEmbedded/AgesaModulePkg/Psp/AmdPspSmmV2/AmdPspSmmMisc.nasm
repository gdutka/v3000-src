;****************************************************************************
; Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
;
;****************************************************************************
;/**
; * @file
; *
; * PSP SMM ASSEMBLY library
; *
; * @xrefitem bom "File Content Label" "Release Content"
; * @e project:      AGESA
; * @e sub-project:  PSP
; * @e \$Revision$   @e \$Date$
; */

SECTION .text
;------------------------------------------------------------------------------
; VOID
; S3ExitToBiosV2 (
;   VOID
;   );
;------------------------------------------------------------------------------
; The below code will resume outside SMM
global ASM_PFX(S3ExitToBiosV2)
ASM_PFX(S3ExitToBiosV2):
  rsm