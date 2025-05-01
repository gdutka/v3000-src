## @file
#  Describes Fixes for Build Errors when Upgrading
#
#******************************************************************************
#* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
#*
#* You may not reproduce, distribute, publish, display, perform, modify, adapt,
#* transmit, broadcast, present, recite, release, license or otherwise exploit
#* any part of this publication in any form, by any means, without the prior
#* written permission of Insyde Software Corporation.
#*
#******************************************************************************
Tag#: 05.41.28
Sympton: Build tool show "error B5000: MODULE_TYPE is not given"
RelatedFiles: INF and source files of EDK I style driver.
Solution: EDK I style drivers are not supported anymore. Please convert them to EDK II style.
          For more EDKII style related information. Please visit
          https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Specifications

Tag#: 05.41.16
Sympton: (1) Build tool show "IOError: [Errno 2] No such file or directory: '/Project.dsc'"
         (2) Build tool show "Cannot find PCD: gChipsetPkgTokenSpaceGuid.PcdFlashFvEcBase in the DEC files of the imported packages!"
         (3) Build tool show "BiosGuardSvn.exe error! F:Build\CoffeeLakeMultiBoardPkg\Project.dsc is inexistence!"
         (4) Linux build failed and show "cannot find CoffeeLakeMultiBoardPkg/PlatformConfig/PatchFitConfig.ini".
         (5) Build FSP failed by "BuildFsp.cmd /r" command and show "error B3000: CHIPSET_REL_PATH not defined".
RelatedFiles: (1) Board\Intel\CoffeeLakeMultiBoardPkg\ProjectSetup.bat
              (2) Board\Intel\CoffeeLakeMultiBoardPkg\ProjectSetup.bat
              (3) BaseTools\Bin\Win32\build.exe
                  Intel\CannonLake\CannonLakeChipsetPkg\Tools\Bin\Win32\BiosGuardSvn.exe
              (4) BaseTools\Conf\GNUmakefile
                  Board\Intel\CoffeeLakeMultiBoardPkg\GNUmakefile
              (5) Intel\CannonLake\CannonLakeFspPkg\BuildFv.cmd
Solution: (1) Add the file path of board package directory in PACKAGES_PATH environment variable in ProjectSetup.bat. For example: "set PACKAGES_PATH=%WORKSPACE%\Board\Intel".
          (2) Add the file path of platform package directory in PACKAGES_PATH environment variable in ProjectSetup.bat. For example: "set PACKAGES_PATH=%WORKSPACE%\Intel\CannonLake".
          (3) Add solution IB07240657 (Adopted in 05.41.20) to update build tool build.exe and BiosGuardSvn.exe to support multiple workspace.
          (4) Add solution IB07240660 (Adopted in 05.41.24) to update GNUmakefile file to correct the file path of PatchFitConfig.ini.
          (5) Add solution IB07240704 (Adopted in 05.41.44) to update BuildFv.cmd file to pass CHIPSET_REL_PATH environment variable to build.exe.