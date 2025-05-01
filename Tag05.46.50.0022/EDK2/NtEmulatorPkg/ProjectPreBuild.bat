@REM
@REM Project pre-build batch file
@REM
@REM ******************************************************************************
@REM * Copyright (c) 2012 - 2013, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************

@set CHIPSET_REL_PATH=Intel\CannonLake

REM Delete Old Sipkg.dec
REM --------------------
  if exist "%WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeSiliconPkg\SiPkg.dec" (
    del /f /q %WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeSiliconPkg\SiPkg.dec
  )

  if exist "%WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeFspPkg\PlatformSelector.dsc" (
    del /f /q %WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeFspPkg\PlatformSelector.dsc
  )

REM Select a platform of Sipkg(CNL/CFL).dec
REM ---------------------------------------
  copy %WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeSiliconPkg\SiPkgCfl.dec %WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeSiliconPkg\SiPkg.dec
  copy PlatformSelector.dsc %WORKSPACE%\%CHIPSET_REL_PATH%\CannonLakeFspPkg\PlatformSelector.dsc


@REM Run kernel pre-build process
@if exist %WORKSPACE%\BaseTools\KernelPreBuild.bat call %WORKSPACE%\BaseTools\KernelPreBuild.bat %1
@if not errorlevel 0 goto end

@REM Run Chipset specific pre-build process
@GetProjectEnv CHIPSET_PKG > NUL && for /f %%a in ('GetProjectEnv CHIPSET_PKG') do set %%a
if exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPreBuild.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPreBuild.bat %1
:end



