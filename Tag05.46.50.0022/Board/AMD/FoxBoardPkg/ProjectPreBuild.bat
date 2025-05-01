@REM
@REM Project pre-build batch file
@REM
@REM ******************************************************************************
@REM * Copyright (c) 2012 - 2020, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************


@REM ============================================
@REM   Run Project specific pre-build process
@REM ============================================

@REM ============================================
@REM   Run Project File Override
@REM    * Here, consider that it might temporary terminal the build process, check there have the some previous .bak file, it need to be restore.
@REM ============================================
@if exist %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\ProjectOverride.bat call %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\ProjectOverride.bat PREBUILD
  @set GBS_FILE_NAME=iFoxGbs.xml

@findstr /i /c:"SETUP_CPU_DEBUG_OPTIONS_LABLE" %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Build\ResourceRMB\AmdCbsFormID.h
@IF /I %ERRORLEVEL% EQU 0 (
  @if /I [%BUILD_TYPE%] EQU [EXTERNAL] rmdir %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Build\ResourceRMB /s /q
)
@REM ============================================
@REM   Copy Package files for CBS
@REM ============================================
@REM CBS XML parse tool
@if exist %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\Perl\bin\perl.exe (
  @set PERL_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\Perl\bin
  @if exist %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\IdsIdGen.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\IdsIdGen.bat
  @if exist %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Library\Family\0x19\RMB\External\xmlparse.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Library\Family\0x19\RMB\External\xmlparse.bat
)

@REM ============================================
@REM   Gen APCB
@REM ============================================


@call GenApcbV3BinaryRMB_DDR5.bat

@REM ============================================
@REM   Run Kernel pre-build process
@REM ============================================
@if exist %WORKSPACE%\BaseTools\KernelPreBuild.bat call %WORKSPACE%\BaseTools\KernelPreBuild.bat %1
@if not errorlevel 0 goto end

@REM ============================================
@REM   Run Chipset specific pre-build process
@REM ============================================
@GetProjectEnv CHIPSET_PKG > NUL && for /f %%a in ('GetProjectEnv CHIPSET_PKG') do set %%a
@if exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPreBuild.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPreBuild.bat %1


@REM ============================================
@REM Auto Generate PBS default value
@REM ============================================
@if exist %PERL_PATH%\bin\perl.exe (
  %PERL_PATH%\perl.exe -I%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools -I%PERL_PATH%\..\lib^
  %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\AutoPbsDefault.pl %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCpmPkg\Addendum\Oem\Fox\Dxe\AmdPbsSetupDxe\AmdPbsHii.vfr^
  %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCpmPkg\Addendum\Oem\Fox\Library\AmdPbsConfigLib\AmdPbsDefault.c
)



:end