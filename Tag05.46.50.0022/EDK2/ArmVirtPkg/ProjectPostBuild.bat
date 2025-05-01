@REM ******************************************************************************
@REM * Copyright (c) 2022, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************

@REM Run kernel post-build process
@if exist %WORKSPACE%\BaseTools\KernelPostBuild.bat call %WORKSPACE%\BaseTools\KernelPostBuild.bat %1

@REM Run Chipset specific post-build process
@REM GetProjectEnv CHIPSET_PKG > NUL && for /f %%a in ('GetProjectEnv CHIPSET_PKG') do set %%a
@REM GetProjectEnv CHIPSET_REL_PATH > NUL && for /f %%a in ('GetProjectEnv CHIPSET_REL_PATH') do set %%a
@REM if exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPostBuild.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPostBuild.bat %1


@set FV_DIR=%WORKSPACE%\Build\%PROJECT_PKG%\DEBUG_%TOOL_CHAIN%\FV
