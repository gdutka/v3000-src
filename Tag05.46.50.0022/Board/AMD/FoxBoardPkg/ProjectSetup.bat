@REM
@REM Project setup batch file
@REM
@REM ******************************************************************************
@REM * Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************

@echo off

REM ---------------------------------------------------------------------------------------------
REM Auto get chipset BIOS setting from Project.env
REM ---------------------------------------------------------------------------------------------
for /f "tokens=4" %%a in ('find "VN_16M_AB_CFG" Project.env') do set VN_16M_AB_CFG=%%a
for /f "tokens=4" %%a in ('find "VN_32M_AB_COMBO_CFG" Project.env') do set VN_32M_AB_COMBO_CFG=%%a
for /f "tokens=4" %%a in ('find "L2A_ONLY" Project.env') do set L2A_ONLY=%%a
for /f "tokens=4" %%a in ('find "LPDDR5" Project.env') do set LPDDR5=%%a
for /f "tokens=4" %%a in ('find "BIOSNAME" Project.env') do set BIOSNAME=%%a
for /f "tokens=4" %%a in ('find "DP_TI_SELECT" Project.env') do set DP_TI_SELECT=%%a
for /f "tokens=4" %%a in ('find "DP_CY_SELECT" Project.env') do set DP_CY_SELECT=%%a

REM ---------------------------------------------------------------------------------------------
REM Auto setting of WORKSPACE environment variable
REM ---------------------------------------------------------------------------------------------
pushd \ && set ROOT_DIR=%cd% && popd && pushd .
:SetWorkSpace
if "%cd%" == "%ROOT_DIR%" goto Next
cd ..
if not exist %cd%\BaseTools goto SetWorkSpace
set WORKSPACE=%cd%
:Next
set ROOT_DIR= && popd

REM ---------------------------------------------------------------------------------------------
REM Setting of using 32 or 64 bits build tools
REM ---------------------------------------------------------------------------------------------
REM set TOOL_BIN_PATH=Win64
if not defined TOOL_BIN_PATH (
  set TOOL_BIN_PATH=Win32
)
echo Selected TOOL_BIN_PATH = %TOOL_BIN_PATH%
echo.

%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv WORKSPACE > NUL && for /f %%i in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv WORKSPACE') do set %%i
REM ---------------------------------------------------------------------------------------------
REM Set PACKAGES_PATH here to specify multiple workspaces.
REM ---------------------------------------------------------------------------------------------
set PACKAGES_PATH=^
%WORKSPACE%\Board\AMD;^
%WORKSPACE%\AMD\AmdEmbedded;^
%WORKSPACE%\Insyde;^
%WORKSPACE%\EDK2;^
%WORKSPACE%\SegFeature;
@REM ---------------------------------------------------------------------------------------------
@REM Support Module build
@REM Build command sample:
@REM                      build -q -p ..\Build\SkylakeMultiBoardPkg\Project.dsc -m InsydeModulePkg/Universal/MemoryTest/GenericMemoryTestDxe/GenericMemoryTestDxe.inf
@REM ---------------------------------------------------------------------------------------------
%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_PKG > NUL && for /f %%i in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_PKG') do set %%i
%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv CHIPSET_REL_PATH > NUL && for /f %%a in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv CHIPSET_REL_PATH') do @set %%a
%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_REL_PATH > NUL && for /f %%a in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_REL_PATH') do @set %%a
%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv CHIPSET_PKG > NUL && for /f %%a in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv CHIPSET_PKG') do set %%a

REM ---------------------------------------------------------------------------------------------
REM Project dependent parameters
REM ---------------------------------------------------------------------------------------------
REM set TOOL_CHAIN=DEVTLS_VC14
set ASL_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\ASL\
set ARCH=IA32 X64
set EFI_SOURCE_DIR=InsydeModulePkg
if not defined PYTHON_PATH (
  for /f "tokens=*" %%a in ('where python') do @(
    setlocal enabledelayedexpansion
    "%%a" -V 2>&1 | findstr /i /c:"python 3" > nul
    if !errorlevel!==0 (
      endlocal
      set PYTHON_PATH=%%~dpa
    ) else (
      endlocal
    )
  )
)
set PROJECT_BUILD_OPTIONS=--DoNotCheckPcdValue
set FILE_EXTENSION=bat

REM ---------------------------------------------------------------------------------------------
REM Check tool chain
REM ---------------------------------------------------------------------------------------------
@if not defined TOOL_CHAIN (
  REM ---------------------------------------------------------------------------------------------
  REM @echo Check the DEVTLS environment.
  REM ---------------------------------------------------------------------------------------------
  set TOOL_DIR=DEVTLS\MSVC14
  call :CheckDevtlsPath C:
  call :CheckDevtlsPath D:
  call :CheckDevtlsPath E:
  call :CheckDevtlsPath F:
  call :CheckDevtlsPath G:
  call :CheckDevtlsPath H:
  call :CheckDevtlsPath %WORKSPACE:~0,2%
)

if defined TOOL_CHAIN_DRV (
  set TOOL_CHAIN=DEVTLS_VC14
  goto KernelEnvSetting
)

@if defined TOOL_CHAIN goto KernelEnvSetting

@if not defined TOOL_CHAIN (
  if defined VS140COMNTOOLS (
    REM ---------------------------------------------------------------------------------------------
    REM @echo Select the VS2015 environment.
    REM ---------------------------------------------------------------------------------------------
    set TOOL_CHAIN=VS2015x86
    goto KernelEnvSetting
  )
)

:ErrorHandle
REM If Visual Studio 2015 was not detected, return an error.
@if not defined TOOL_CHAIN (
  echo.
  echo !!! ERROR !!! Visual Studio 2015 not installed correctly!!!
  echo.
  pause
  goto :EOF
)

:CheckDevtlsPath
if not DEFINED TOOL_CHAIN_DRV (
  if exist "%1\%TOOL_DIR%" set TOOL_CHAIN_DRV=%1
)
goto :EOF

:KernelEnvSetting
call %WORKSPACE%\BaseTools\H2ORev50.bat
echo. >> %WORKSPACE%\Conf\tools_def.txt
echo ################## >> %WORKSPACE%\Conf\tools_def.txt
echo # AMD CompressBIOS tool definitions >> %WORKSPACE%\Conf\tools_def.txt
echo ################## >> %WORKSPACE%\Conf\tools_def.txt
echo *_*_*_AMDCOMPRESSBIOS_PATH = ENV(WORKSPACE)\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\CompressBios\CompressBios.bat >> %WORKSPACE%\Conf\tools_def.txt
echo *_*_*_AMDCOMPRESSBIOS_GUID = 991EFAC0-E260-416B-A4B8-3B153072B804 >> %WORKSPACE%\Conf\tools_def.txt

echo BiosName = %BIOSNAME%
echo.