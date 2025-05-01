## @file
#  Common Makefile for project build
#
#******************************************************************************
#* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

#
# Build options for build targets
#
$(shell chmod a+x GetWorkspace.sh)
export WORKSPACE = $(shell ./GetWorkspace.sh)
export PACKAGES_PATH=$(WORKSPACE)/Board/AMD:$(WORKSPACE)/AMD/AmdEmbedded:$(WORKSPACE)/Insyde:$(WORKSPACE)/EDK2:$(WORKSPACE)/SegFeature

getenv2 = $(shell  export PACKAGES_PATH=$(PACKAGES_PATH) ; $(WORKSPACE)/BaseTools/Bin/Linux/GetProjectEnv $(1))
getenv = $(word 2, $(subst =, ,$(call getenv2, $1)))
export CHIPSET_PKG=$(call getenv, CHIPSET_PKG)
export CHIPSET_REL_PATH=$(call getenv, CHIPSET_REL_PATH)
export PLATFORM_NAME = $(shell echo $(word 3, $(shell grep 'PLATFORM_NAME'  $(WORKSPACE)/Build/$(PROJECT_PKG)/Project.dsc)) | tr a-z A-Z)
export IASL_PREFIX=$(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/Tools/Bin/Linux/
$(shell chmod -R a+x $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/Tools/Bin/Linux)


include $(WORKSPACE)/BaseTools/Conf/GNUmakefile
include $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/GNUmakefile

export PATH=$(EDK_TOOLS_PATH)/Bin/Linux:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

export TARGET=$(shell grep -e "^TARGET[\t ]*=" $(WORKSPACE)/Conf/target.txt | awk '{print $$3}')
export TOOL_CHAIN_TAG=$(TOOL_CHAIN)
export FILE_EXTENSION=sh
FV_OUTPUT_PATH=$(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/FV
PcdFdmHash=$(shell grep 'PcdFdmHash' $(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/IA32/PcdList.txt | awk '{print $$2}' | sed 's/0x/~/g' | awk -F '[{~,}]' '{print $$9$$7$$5$$3"-"$$13$$11"-"$$17$$15"-"$$19$$21"-"$$23$$25$$27$$29$$31$$33}' | tr A-Z a-z)
PcdH2OFdmChainOfTrustSupported=$(shell grep 'PcdH2OFdmChainOfTrustSupported' $(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/IA32/PcdList.txt | awk '{print $$2}')
PEI_FD=$(FV_OUTPUT_PATH)/RECOVERYFV.Fv
PEI_COMPRESS_FD=$(FV_OUTPUT_PATH)/RECOVERYFV_COMPRESS.bin
COMPRESS_TOOL_PATH=$(WORKSPACE)/$(CHIPSET_REL_PATH)/AgesaModulePkg/AMDTools/CompressBios
PcdFlashFvRecoveryBase=$(shell grep 'PcdFlashFvRecoveryBase' $(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/IA32/PcdList.txt | awk '{print $$2}')
PcdFlashFvRecoverySize=$(shell grep 'PcdFlashFvRecoverySize' $(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/IA32/PcdList.txt | awk '{print $$2}')
PcdFlashPeiFlashBBase=$(shell grep 'PcdFlashPeiFlashBBase' $(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/IA32/PcdList.txt | awk '{print $$2}')
PcdFlashAreaBaseAddress=$(shell grep 'PcdFlashAreaBaseAddress' $(WORKSPACE)/Build/$(PROJECT_PKG)/$(TARGET)_$(TOOL_CHAIN_TAG)/IA32/PcdList.txt | awk '{print $$2}')
FlashFvRecoveryBase=$(shell echo $$(($(PcdFlashFvRecoveryBase)-$(PcdFlashAreaBaseAddress))))
FlashPeiFlashBBase=$(shell echo $$(($(PcdFlashPeiFlashBBase)-$(PcdFlashAreaBaseAddress))))

buildcbs:
	$(shell chmod a+x $(WORKSPACE)/$(CHIPSET_REL_PATH)/AmdCbsPkg/Tools/IdsIdGen.sh)
	$(shell chmod a+x $(WORKSPACE)/$(CHIPSET_REL_PATH)/AmdCbsPkg/Library/Family/0x19/RMB/External/xmlparse.sh)
	@$(WORKSPACE)/$(CHIPSET_REL_PATH)/AmdCbsPkg/Tools/IdsIdGen.sh
	@$(WORKSPACE)/$(CHIPSET_REL_PATH)/AmdCbsPkg/Library/Family/0x19/RMB/External/xmlparse.sh EMBFP7

project_prebuild:
	$(shell chmod a+x $(WORKSPACE)/$(PROJECT_REL_PATH)/$(PROJECT_PKG)/GenApcbV3BinaryRMB_DDR5.sh)
	$(WORKSPACE)/$(PROJECT_REL_PATH)/$(PROJECT_PKG)/GenApcbV3BinaryRMB_DDR5.sh
	make kernel_prebuild
	make chipset_prebuild
	$(shell chmod a+x $(WORKSPACE)/$(PROJECT_REL_PATH)/$(PROJECT_PKG)/SmmSupvBuild.sh)
#kernel_prebuild

project_postbuild:
	make kernel_postbuild
	make chipset_postbuild
	$(WORKSPACE)/BaseTools/Bin/Linux/PatchFdmHash $(WORKSPACE)/Build/$(PROJECT_PKG)/$(word 3, $(shell grep 'TARGET '  $(WORKSPACE)/Conf/target.txt))_$(TOOL_CHAIN)/FV/$(PLATFORM_NAME).fd  $(WORKSPACE)/Build/$(PROJECT_PKG)/$(word 3, $(shell grep 'TARGET '  $(WORKSPACE)/Conf/target.txt))_$(TOOL_CHAIN)/FV/$(PLATFORM_NAME).fdm
	@if [ $(PcdH2OFdmChainOfTrustSupported) -eq 1 ]; then \
	python3 $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/Tools/CalculateFileHash.py -i $(FV_OUTPUT_PATH)/$(PLATFORM_NAME).fdm -o $(FV_OUTPUT_PATH)/FdmHash.bin; \
	$(WORKSPACE)/BaseTools/Bin/Linux/GenSec -s EFI_SECTION_RAW -o $(FV_OUTPUT_PATH)/Ffs/$(PcdFdmHash)RECOVERYFV/$(PcdFdmHash)SEC1.raw $(FV_OUTPUT_PATH)/FdmHash.bin; \
	$(WORKSPACE)/BaseTools/Bin/Linux/GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $(PcdFdmHash) -o $(FV_OUTPUT_PATH)/Ffs/$(PcdFdmHash)RECOVERYFV/$(PcdFdmHash).ffs -i $(FV_OUTPUT_PATH)/Ffs/$(PcdFdmHash)RECOVERYFV/$(PcdFdmHash)SEC1.raw; \
	$(WORKSPACE)/BaseTools/Bin/Linux/GenFv -i $(FV_OUTPUT_PATH)/RECOVERYFV.inf -o $(FV_OUTPUT_PATH)/RECOVERYFV.fv; \
	fi;
	perl $(COMPRESS_TOOL_PATH)/CompressBios.pl $(PEI_FD) $(PEI_COMPRESS_FD) $(PcdFlashFvRecoverySize)
	python3 $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/Tools/PatchBios.py -b $(FV_OUTPUT_PATH)/$(PLATFORM_NAME).fd -p $(PEI_COMPRESS_FD) -a $(FlashFvRecoveryBase)
	python3 $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/Tools/PatchBios.py -b $(FV_OUTPUT_PATH)/$(PLATFORM_NAME).fd -p $(PEI_COMPRESS_FD) -a $(FlashPeiFlashBBase)
	cp $(FV_OUTPUT_PATH)/$(PLATFORM_NAME).fd $(FV_OUTPUT_PATH)/$(PLATFORM_NAME)_GLUED.fd
	$(shell chmod a+x $(WORKSPACE)/$(PROJECT_REL_PATH)/$(PROJECT_PKG)/PspBuild.sh)
	$(WORKSPACE)/$(PROJECT_REL_PATH)/$(PROJECT_PKG)/PspBuild.sh

