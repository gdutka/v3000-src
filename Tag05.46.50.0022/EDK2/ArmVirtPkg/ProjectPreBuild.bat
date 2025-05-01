@REM
@REM Project pre-build batch file
@REM
@REM ******************************************************************************
@REM * Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************

@set CHIPSET_REL_PATH=Intel\CannonLake

@REM Run kernel pre-build process
@if exist %WORKSPACE%\BaseTools\KernelPreBuild.bat call %WORKSPACE%\BaseTools\KernelPreBuild.bat %1
@if not errorlevel 0 goto end

@REM Run Chipset specific pre-build process
@GetProjectEnv CHIPSET_PKG > NUL && for /f %%a in ('GetProjectEnv CHIPSET_PKG') do set %%a
if exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPreBuild.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPreBuild.bat %1
:end



