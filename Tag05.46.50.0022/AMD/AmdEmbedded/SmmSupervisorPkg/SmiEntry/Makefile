# Copyright(C) 2020 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: BSD-2-Clause-Patent

#SMI_ENTRY_INCLUDE = will be populated by build environment
CL = cl
NASM = nasm
TRIM = Trim

FNAME = SmmSupvEntry

NASM_FLAGS = -Ox -f win64 -o $(FNAME).obj $(FNAME).trim $(SMI_ENTRY_INCLUDE)
NASM_FLAGS2 = -Ox -f bin -o $(FNAME).bin $(FNAME).trim $(SMI_ENTRY_INCLUDE)
TRIM_FLAGS = --trim-long --source-code -o $(FNAME).trim $(FNAME).cl
CL_FLAGS = /nologo /E /TC /D INTERNAL_IDS $(FNAME).nasm > $(FNAME).cl

all:
	$(CL) $(CL_FLAGS)
	$(TRIM) $(TRIM_FLAGS)
	$(NASM) $(NASM_FLAGS)
	$(NASM) $(NASM_FLAGS2)

clean:
	del /F $(FNAME).trim $(FNAME).cl $(FNAME).bin $(FNAME).obj