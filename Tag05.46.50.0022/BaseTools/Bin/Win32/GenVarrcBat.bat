@REM
@REM  Batch file to generat Varrc file by calling VarCompile
@REM
@REM ******************************************************************************
@REM * Copyright (c) 2019 - 2022, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************
@echo off
pushd %1%
@REM #
@REM # Copy project.var to VarFile folder and set default priority to 3
@REM #

if exist %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\Project.var (

  if not exist VarFile mkdir VarFile
  if exist VarFile\Project.var (
    del /f /q VarFile\Project.var
  )

  FOR /F "tokens=* delims=" %%i IN (%WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\Project.var) DO (
    @REM
    @REM redirect string to file before EnableDelayedExpansion to prevent from leading ! is removed
    @REM ex: !include will become include.
    @REM
    @echo( %%i>> VarFile\Project.var
    @REM
    @REM set %i to b and remove all of the spaces in local b variable.
    @REM
    setlocal EnableDelayedExpansion
    set b=%%i & set b=!b: =!
    FOR /F "delims=. tokens=1*" %%j IN ("!b!") DO (
      if "%%j" == "[Var" (
        @echo(   Priority = 3 >>VarFile\Project.var
      )
    )
    endlocal
  )
)

@REM #
@REM # Create VarList.txt which contains all of *.var file from different priorities
@REM #
if exist VarList.txt (
  del /f /q VarList.txt
)
if exist VarFile (
  dir /b VarFile > VarList.txt
  move /y VarList.txt VarFile\VarList.txt
)

@REM #
@REM # Create a merged var file which used to call VarCompile tool.
@REM #
if exist TotalVarFile.var (
  del /f /q TotalVarFile.var
)

if exist VarFile (
REM
REM  Don't use batch file to put !include to the start of file.
REM  The performance of batch file is too bad. So enhance VarCompile
REM  tool to support !include can put in anywhere in var file.
REM

REM   cd VarFile
REM   for /r %%i in (*.var) do type %%i >> ..\TotalVarFile_temp.var
REM   FOR /F "tokens=* delims=" %%i IN (..\TotalVarFile_temp.var) DO (
REM     FOR /F "tokens=1*" %%j IN ("%%i") DO (
REM       if "%%j" == "!include" (
REM         @echo %%i>> ..\TotalVarFile.var
REM       )
REM     )
REM   )
REM   FOR /F "tokens=* delims=" %%i IN (..\TotalVarFile_temp.var) DO (
REM     FOR /F "tokens=1*" %%j IN ("%%i") DO (
REM       if not "%%j" == "!include" (
REM         @echo %%i>> ..\TotalVarFile.var
REM       )
REM     )
REM   )
REM   del /f /q ..\TotalVarFile_temp.var
REM   cd ..
  copy VarFile\*.var TotalVarFile.var > NUL
  if exist VarDefault.varrc (
    del /f /q VarDefault.varrc
  )

  if exist TotalVarFile.var (
    VarCompile.exe -var TotalVarFile.var -of VarDefault.varrc
    if errorlevel 1 (
      popd
      exit /B 1
    )
@REM #
@REM # Create dummy .varrc file if not have any .var file
@REM #
  ) else (
    echo. 2>VarDefault.varrc
  )
@REM #
@REM # Create dummy .varrc file if not have any .var file
@REM #
) else (
  echo. 2>VarDefault.varrc
)
popd