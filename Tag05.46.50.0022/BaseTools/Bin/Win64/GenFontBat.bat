@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

if "%MAKEDIR%"=="" (
  for /f "tokens=3" %%a in ('findstr /R "\<TARGET\>" %WORKSPACE%\Conf\target.txt') do (
    set BUILDTARGET=%%a
  )
  set MAKEDIR=%WORKSPACE%\Build\%PROJECT_PKG%\!BUILDTARGET!_%TOOL_CHAIN%
)

set BASEDIR=!MAKEDIR!
set LANGCODES=
set PCDLANGCODES=
set PCDLIST=

REM
REM find langcodes by PcdList.txt if --langcodes isn't exist in commanbd line parameter
REM
for %%i in (..\IA32 ..\X64 ..\ARM ..\AARCH64 IA32 X64 ARM AARCH64) do (

  set PCD_LIST_PATH=!MAKEDIR!\%%i\PcdList.txt

  if "!PCDLANGCODES!" == "" if exist !PCD_LIST_PATH! (
    for /f "tokens=2* delims=: " %%j  in ('findstr PcdUefiVariableDefaultPlatformLangCodes !PCD_LIST_PATH!') do (
      set "PCDLANGCODES='%%j'"
      set "PCDLIST=--pcdlist !PCD_LIST_PATH!"
    )
  )

)

((echo "%*"|find "--langcodes")>nul) || (
  if "!PCDLANGCODES!" == "" (
    echo.
    echo ---- Can not found langcodes in PcdList.txt, it will not generate font.hpk ----
    echo.
    @exit 0
  )

  set "LANGCODES=--langcodes !PCDLANGCODES!"
)

set Param=!LANGCODES! !PCDLIST! --hpkdir !MAKEDIR! %*

if exist %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\Project.fif (
  set Param=!Param! --fif %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\Project.fif
)

GenFont.exe !Param!

@exit %ERRORLEVEL%

