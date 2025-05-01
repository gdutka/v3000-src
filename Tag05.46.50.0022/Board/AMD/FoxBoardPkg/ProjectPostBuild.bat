@REM
@REM Project post-build batch file
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
@REM   Run Chipset specific post-build process
@REM ============================================
@GetProjectEnv CHIPSET_PKG > NUL && for /f %%a in ('GetProjectEnv CHIPSET_PKG') do set %%a
@GetProjectEnv CHIPSET_REL_PATH > NUL && for /f %%a in ('GetProjectEnv CHIPSET_REL_PATH') do @set %%a
@if exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPostBuild.bat call %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\ChipsetPostBuild.bat %1

@REM ============================================
@REM   Run kernel post-build process
@REM ============================================
@if exist %WORKSPACE%\BaseTools\KernelPostBuild.bat call %WORKSPACE%\BaseTools\KernelPostBuild.bat %1

@REM ============================================
@REM   Run Project specific post-build process
@REM ============================================
@if exist %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\ProjectOverride.bat call %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\ProjectOverride.bat POSTBUILD

@for /f "tokens=3" %%a in ('find "TARGET =" %WORKSPACE%\Conf\target.txt') do @set TARGET=%%a
@for /f "tokens=3" %%a in ('find "TOOL_CHAIN_TAG" %WORKSPACE%\Conf\target.txt') do @set TOOL_CHAIN_TAG=%%a
@for /f "tokens=3" %%a in ('find "PLATFORM_NAME" %WORKSPACE%\Build\%PROJECT_PKG%\Project.dsc') do @set PLATFORM_NAME=%%a

@REM Update hash vale of FDM entries at last step of building
@for /f "tokens=3" %%a in ('findstr /R "\<TARGET\>" %WORKSPACE%\Conf\target.txt') do (
  @PatchFdmHash %WORKSPACE%/Build/%PROJECT_PKG%\%%a_%TOOL_CHAIN%/FV/%PLATFORM_NAME%.fd %WORKSPACE%/Build/%PROJECT_PKG%\%%a_%TOOL_CHAIN%/FV/%PLATFORM_NAME%.fdm
)

@REM FvAlignChecker.exe for 4K Align Check
FvAlignChecker.exe --PCD-report-file %WORKSPACE%/Build/%PROJECT_PKG%/BuildReport.txt

@set FV_OUTPUT_PATH=%WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\FV
@for /f "tokens=2" %%a in ('find "PcdFdmHash" %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt') do @set PcdFdmHash=%%a
@for /f "tokens=1-16 delims={~,}" %%a in ("%PcdFdmHash:0x=~%") do @set PcdFdmHash=%%d%%c%%b%%a-%%f%%e-%%h%%g-%%i%%j-%%k%%l%%m%%n%%o%%p
@for /f "tokens=2" %%a in ('find "PcdH2OFdmChainOfTrustSupported" %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt') do @set PcdH2OFdmChainOfTrustSupported=%%a
@if %PcdH2OFdmChainOfTrustSupported% == 1 (
  @%PYTHON_PATH%/python.exe %WORKSPACE%/%CHIPSET_REL_PATH%/%CHIPSET_PKG%/Tools/CalculateFileHash.py -i %FV_OUTPUT_PATH%/%PLATFORM_NAME%.fdm -o %FV_OUTPUT_PATH%/FdmHash.bin
  @%WORKSPACE%/BaseTools/Bin/%TOOL_BIN_PATH%/GenSec -s EFI_SECTION_RAW -o %FV_OUTPUT_PATH%/Ffs/%PcdFdmHash%RECOVERYFV/%PcdFdmHash%SEC1.raw %FV_OUTPUT_PATH%/FdmHash.bin
  @%WORKSPACE%/BaseTools/Bin/%TOOL_BIN_PATH%/GenFfs -t EFI_FV_FILETYPE_FREEFORM -g %PcdFdmHash% -o %FV_OUTPUT_PATH%/Ffs/%PcdFdmHash%RECOVERYFV/%PcdFdmHash%.ffs -i %FV_OUTPUT_PATH%/Ffs/%PcdFdmHash%RECOVERYFV/%PcdFdmHash%SEC1.raw
  @%WORKSPACE%/BaseTools/Bin/%TOOL_BIN_PATH%/GenFv -i %FV_OUTPUT_PATH%/RECOVERYFV.inf -o %FV_OUTPUT_PATH%/RECOVERYFV.fv
)

@set PEI_FD=%FV_OUTPUT_PATH%/RECOVERYFV.Fv
@set PEI_COMPRESS_FD=%FV_OUTPUT_PATH%/RECOVERYFV_COMPRESS.bin
@set COMPRESS_TOOL_PATH=%WORKSPACE%/%CHIPSET_REL_PATH%/AgesaModulePkg/AMDTools/CompressBios
@for /f "tokens=2" %%a in ('find "PcdFlashFvRecoveryBase" %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt') do @set PcdFlashFvRecoveryBase=%%a
@for /f "tokens=2" %%a in ('find "PcdFlashFvRecoverySize" %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt') do @set PcdFlashFvRecoverySize=%%a
@for /f "tokens=2" %%a in ('find "PcdFlashPeiFlashBBase" %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt') do @set PcdFlashPeiFlashBBase=%%a
@for /f "tokens=2" %%a in ('find "PcdFlashAreaBaseAddress" %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt') do @set PcdFlashAreaBaseAddress=%%a
@set /A FlashFvRecoveryBase=%PcdFlashFvRecoveryBase%-%PcdFlashAreaBaseAddress%
@set /A FlashPeiFlashBBase=%PcdFlashPeiFlashBBase%-%PcdFlashAreaBaseAddress%

@%COMPRESS_TOOL_PATH%/CompressBios.exe %PEI_FD% %PEI_COMPRESS_FD% %PcdFlashFvRecoverySize%
@for /f "usebackq" %%a in ('%PEI_COMPRESS_FD%') do @set PEI_COMPRESS_FD_SIZE=%%~za
@if %PEI_COMPRESS_FD_SIZE% == 0 (
  @exit /B
)
@%PYTHON_PATH%/python.exe %WORKSPACE%/%CHIPSET_REL_PATH%/%CHIPSET_PKG%/Tools/PatchBios.py -b %FV_OUTPUT_PATH%/%PLATFORM_NAME%.fd -p %PEI_COMPRESS_FD% -a %FlashFvRecoveryBase%
@%PYTHON_PATH%/python.exe %WORKSPACE%/%CHIPSET_REL_PATH%/%CHIPSET_PKG%/Tools/PatchBios.py -b %FV_OUTPUT_PATH%/%PLATFORM_NAME%.fd -p %PEI_COMPRESS_FD% -a %FlashPeiFlashBBase%

copy /b %FV_OUTPUT_PATH%\%BIOSNAME%.fd %FV_OUTPUT_PATH%\%BIOSNAME%_GLUED.fd

@echo VN_16M_AB_CFG = %VN_16M_AB_CFG%
@echo VN_32M_AB_COMBO_CFG = %VN_32M_AB_COMBO_CFG%

@call PspBuild.bat
