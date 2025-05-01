## @file
#  Project Makefile
#
#******************************************************************************
#* Copyright (c) 2012 - 2017, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************

#
# Build mode customization
# -q: Quiet mode   - disable all messages except fatal errors, module based messages.
#                    this is default build mode if BUILD_MODE is not specified
# -s: Silent mode  - file based messages
# -v: Verbose mode - Turn on verbose output with informational messages printed
#
#BUILD_MODE     = -s

#
# Build report customization
# Below is the default build report setting if BUILD_REPORT is not specified
#BUILD_REPORT = -Y PCD -Y DEPEX -Y LIBRARY -y $(WORKSPACE)/Build/$(PROJECT_PKG)/BuildReport.txt

#
# Add additional project build options here
#

PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D VN_16M_AB_CFG=$(VN_16M_AB_CFG)
PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D VN_32M_AB_COMBO_CFG=$(VN_32M_AB_COMBO_CFG)
!if "$(SMM_ISOLATION_SUPPORT)" == "YES"
PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D SMM_ISOLATION_SUPPORT=$(SMM_ISOLATION_SUPPORT)
!endif
PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D BIOSNAME=$(BIOSNAME)
PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D LPDDR5=$(LPDDR5)

PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D DP_TI_SELECT=$(DP_TI_SELECT)
PROJECT_BUILD_OPTIONS = $(PROJECT_BUILD_OPTIONS) -D DP_CY_SELECT=$(DP_CY_SELECT)

PEI_CRISIS_RECOVERY_BUILD = -D USE_FAST_CRISIS_RECOVERY=NO
TEXT_BUILD                = -D TEXT_SUPPORT=YES
DDT_DEBUG_COM             = -D INSYDE_DEBUGGER=YES -D H2O_DDT_DEBUG_IO=Com
DDT_DEBUG_XHC             = -D INSYDE_DEBUGGER=YES -D H2O_DDT_DEBUG_IO=Xhc

!include $(WORKSPACE)\BaseTools\Conf\Makefile

Uefi64PeiCrisisRecovery:
  @$(MAKE) release BUILD_TARGET=$@ BUILD_TARGET_OPTIONS="$(PEI_CRISIS_RECOVERY_BUILD)"
Uefi64Text:
  @$(MAKE) release BUILD_TARGET=$@ BUILD_TARGET_OPTIONS="$(TEXT_BUILD)"
Uefi64ddtCom:
  @$(MAKE) debug BUILD_TARGET=$@ BUILD_TARGET_OPTIONS="$(DDT_DEBUG_COM)"
Uefi64ddtXhc:
  @$(MAKE) debug BUILD_TARGET=$@ BUILD_TARGET_OPTIONS="$(DDT_DEBUG_XHC)"
Uefi64RvsStandard:
  @$(MAKE) release BUILD_TARGET=$@ BUILD_MODE="$(BUILD_MODE)"
