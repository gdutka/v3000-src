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
chipset_prebuild:
	$(shell chmod a+x $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/ChipsetPreBuild.sh)
	$(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/ChipsetPreBuild.sh

chipset_postbuild:
	$(shell chmod a+x $(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/ChipsetPostBuild.sh)
	$(WORKSPACE)/$(CHIPSET_REL_PATH)/$(CHIPSET_PKG)/ChipsetPostBuild.sh

