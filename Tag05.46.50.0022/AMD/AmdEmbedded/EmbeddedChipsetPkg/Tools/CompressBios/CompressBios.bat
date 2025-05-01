@REM @file
@REM This script will exec AMD CompressBios tool with -e options.
@REM
@REM GenFfs will call CompressBios.bat [-z] -e -o OutFile InFile  [--debug n] [-v]
@REM 

@echo off
@setlocal

set QLT=-e
set ARGS=

:Begin
if "%1"=="" goto End

@REM ignore -z param 
if "%1"=="-z" (
  shift
  goto Begin
)

@REM ignore -v param 
if "%1"=="-v" (
  shift
  goto Begin
)

if "%1"=="-e" (
  shift
  goto Begin
)

if "%1"=="-o" (
  set ARGS=%ARGS% %1 %2 %3
  set OutFile=%2
  set OutFolder=%~dp2
  set InFile=%3
  goto End
)

shift
goto Begin

:End
@REM Split Usage (EDK2 tool on BaseTools\Bin\Win32):
@REM   Split.exe 
@REM     -f, --filename inputFile to split
@REM     -s, --split VALUE the number of bytes in the first file
@REM     [-p, --prefix OutputDir]
@REM     [-o, --firstfile Filename1]
@REM     [-t, --secondfile Filename2]
@REM     [-v, --verbose]
@REM     [--version]
@REM     [-q, --quiet disable all messages except fatal errors]
@REM     [-d, --debug[#]
@REM     [-h, --help]

@REM Project.fdf use SECTION ALIGN=128 FV_IMAGE = RECOVERYFV 
@REM 0x80 is align bytes need be striped 
%WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\CompressBios\split.exe -f %InFile% -s 0x80 -p %OutFolder% -o %InFile%.1 -t %InFile%.2

@REM CompressBios Usage: CompressBios.exe -[e/d] -o FileName <inputFile> (size)
%WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\CompressBios\CompressBios.exe -e -o %OutFile% %InFile%.2
@echo on 
