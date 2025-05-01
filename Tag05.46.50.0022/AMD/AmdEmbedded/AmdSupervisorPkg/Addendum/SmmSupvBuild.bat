set INPUT_FILE_NAME=SmmSupv_Empty
set OUTPUT_FILE_NAME=SmmSupv
:: Default Value

set SMM_ISOLATION_SUPPORT=%1

@if %SMM_ISOLATION_SUPPORT% == NO (
  goto END
)

REM Build version: Debug or Release
set BUILD_VERSION=%2
echo %BUILD_VERSION%

if "%PSPKIT_PATH%" == "" (
  set PSPKIT_PATH=%WORKSPACE%\AgesaModulePkg\AMDTools\NewPspKit
)
set SMMSUPV_PKG_PATH=%WORKSPACE%\AmdSupervisorPkg
set SMMSUPV_SIGNTOOL_PATH=%SMMSUPV_PKG_PATH%\AmdTools\SmmSupervisorSigningTool
set SMMSUPV_TEMP_PATH=%WORKSPACE%\Build\%PROJECT_PKG%\SmmSupv
set BIN_PATH=%SMMSUPV_TEMP_PATH%\Bin
REM "PSP_FW_PATH" is required for BuildPspImage.bat
set PSP_FW_PATH=%BIN_PATH%

if exist %WORKSPACE%\Conf\target.txt (
  @for /f "tokens=3" %%a in ('find "TARGET " %WORKSPACE%\Conf\target.txt') do @set TARGET=%%a
  @for /f "tokens=3" %%a in ('find "TOOL_CHAIN_TAG" %WORKSPACE%\Conf\target.txt') do @set TOOL_CHAIN_TAG=%%a
)
set SMMSUPV_BUILD_PATH=%WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\X64

REM deleted old build files
if exist %SMMSUPV_TEMP_PATH% (
  rd %SMMSUPV_TEMP_PATH% /S /Q
)
if exist %BIN_PATH% (
  rd %BIN_PATH% /S /Q
)
REM Copy PSP tool to build path
echo D | xcopy %PSPKIT_PATH%\*.*                    %SMMSUPV_TEMP_PATH%\. /S /Y
REM Copy SmmSupvDir required binaries
echo D | xcopy %SMMSUPV_PKG_PATH%\Binary\*.*      %BIN_PATH%\. /S /Y

:: Todo add code to build OemPolicy.bin

copy %BIN_PATH%\%INPUT_FILE_NAME%.bin                    %SMMSUPV_TEMP_PATH%\. /Y
set PSPKIT_PATH=%SMMSUPV_TEMP_PATH%


copy %SMMSUPV_PKG_PATH%\Binary\SmmSupervisor\%BUILD_VERSION%\SmmSupvSig.bin               %BIN_PATH%\. /Y
REM if /I "%TARGET%" == "DEBUG" (
REM   copy %SMMSUPV_PKG_PATH%\Binary\SmmSupervisor\Debug\SmmSupvSig.bin               %BIN_PATH%\. /Y
REM ) else (
REM   copy %SMMSUPV_PKG_PATH%\Binary\SmmSupervisor\Release\SmmSupvSig.bin               %BIN_PATH%\. /Y
REM )

set BIOS_IMAGE_CONFIG_FILE=%SMMSUPV_PKG_PATH%\Binary\SmmSupv.xml

set OUTPUT_BIN=%OUTPUT_FILE_NAME%.bin
call %SMMSUPV_TEMP_PATH%\BuildPspImage.bat %INPUT_FILE_NAME%.bin NotFound %BIOS_IMAGE_CONFIG_FILE% %OUTPUT_BIN% NOSIGN

if %ERRORLEVEL% NEQ 0 goto ERR_END
copy %SMMSUPV_TEMP_PATH%\TEMP\OUTPUT\%OUTPUT_BIN% %BIN_PATH% /Y /B
::Copy back the updated binary for further operation
@echo SMM SUPV directory @ %BIN_PATH%


goto END

:ERR_END
  @exit /B 2

:END
