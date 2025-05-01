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

REM ---------------------------------------------------------------------------------------------
REM Set PACKAGES_PATH here to specify multiple workspaces.
REM ---------------------------------------------------------------------------------------------
set PACKAGES_PATH=^
%WORKSPACE%\Board\Intel;^
%WORKSPACE%\Intel\CannonLake;^
%WORKSPACE%\Insyde;^
%WORKSPACE%\EDK2

%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_PKG > NUL && for /f %%i in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_PKG') do set %%i
%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_REL_PATH > NUL && for /f %%a in ('%WORKSPACE%\BaseTools\Bin\%TOOL_BIN_PATH%\GetProjectEnv PROJECT_REL_PATH') do set %%a

REM ---------------------------------------------------------------------------------------------
REM Project dependent parameters
REM ---------------------------------------------------------------------------------------------
set TOOL_CHAIN=DEVTLS_VC14
REM Modify ASL_PATH if you want to use customized ASL compiler
REM set ASL_PATH=c:/DEVTLS/Asl/Acpi5.0
set ARCH=IA32 X64
set EFI_SOURCE_DIR=InsydeModulePkg

call %WORKSPACE%\BaseTools\H2ORev50.bat
