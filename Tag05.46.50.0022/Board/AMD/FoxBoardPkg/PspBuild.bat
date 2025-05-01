
@echo "inside RMB pspbuild Emu"


set FD_NAME=%BIOSNAME%_GLUED

:: Default Value
if "%KEY_MODE%"=="" (
  set KEY_MODE=NOSIGN
)

@echo KEY_MODE %KEY_MODE%
set PSP_PLATFORM_PATH=%WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\PlatformConfig
set KVM_PLATFORM_PATH=%WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\Binary\KVM

if "%KEY_MODE%"=="PK" (
  set PSP_FW_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Firmwares\RMB
  set PSP_CONFIG_FILE_PATH=%PSP_PLATFORM_PATH%
)

if "%KEY_MODE%"=="NOSIGN" (
  set PSP_FW_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Firmwares\RMB
  set PSP_CONFIG_FILE_PATH=%PSP_PLATFORM_PATH%
)

@REM #[-start-220301-IB14740220-modify]#
@REM copy /y %WORKSPACE%\%PROJECT_PKG%\Override\AgesaModulePkg\Firmwares\RMB\TypeId0x28_PspSystemDriver_RMB.sbin %PSP_FW_PATH%
@REM #[-end-220301-IB14740220-modify]#

@echo PSP_FW_PATH %PSP_FW_PATH%

set PSPKIT_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\AMDTools\NewPspKit
set PSP_TEMP_PATH=%WORKSPACE%\Build\%PROJECT_PKG%\NewPspKit

set APCB_BOARD_PKG_TEMP_PATH=%WORKSPACE%\Build\%PROJECT_PKG%\ApcbToolV3\External
set CUSTOM_APCB_PATH=%APCB_BOARD_PKG_TEMP_PATH%\Release
@IF %LPDDR5% == YES (
  IF NOT EXIST %CUSTOM_APCB_PATH%\APCB_FP7_DefaultRecovery.bin set CUSTOM_APCB_PATH=
  IF NOT EXIST %CUSTOM_APCB_PATH%\APCB_FP7_Updatable.bin set CUSTOM_APCB_PATH=
) else if %LPDDR5% == NO (
  IF NOT EXIST %CUSTOM_APCB_PATH%\APCB_FP7r2_DefaultRecovery.bin set CUSTOM_APCB_PATH=
  IF NOT EXIST %CUSTOM_APCB_PATH%\APCB_FP7r2_Updatable.bin set CUSTOM_APCB_PATH=
)

set OPENSSL_PATH=%WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\Tools\OpenSSL-Win32


@for /f "tokens=3" %%a in ('find "TARGET =" %WORKSPACE%\Conf\target.txt') do @set TARGET=%%a
@for /f "tokens=3" %%a in ('find "TOOL_CHAIN_TAG" %WORKSPACE%\Conf\target.txt') do @set TOOL_CHAIN_TAG=%%a
set BIOS_FV_PATH=%WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\FV

@for /f "tokens=4" %%a in ('find "KVM_SUPPORT" Project.env') do set KVM_SUPPORT=%%a
@echo KVM_SUPPORT %KVM_SUPPORT%

if exist %PSP_TEMP_PATH% (
  rd %PSP_TEMP_PATH% /S /Q
)
echo D | xcopy %PSPKIT_PATH%\*.*                    %PSP_TEMP_PATH%\. /S /Y
copy %BIOS_FV_PATH%\%FD_NAME%.fd                    %PSP_TEMP_PATH%\. /Y
set PSPKIT_PATH=%PSP_TEMP_PATH%

set RTM_FILE=RECOVERYFV.Fv
copy %BIOS_FV_PATH%\%RTM_FILE%                      %PSP_TEMP_PATH%\. /Y

::
:: Select BIOSImageDirectory
::
@if %VN_16M_AB_CFG% == YES (
  @echo PSP 16M RMB build
  set BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory16M.xml
) else (
  @echo PSP 32M RMB build
  @IF  %LPDDR5% == YES (
    @IF %L2A_ONLY% == YES (
      set BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_LPDDR5_L2A.xml
    ) else (
      set BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_LPDDR5.xml
    )
  ) else if %LPDDR5% == NO (
    @IF %L2A_ONLY% == YES (
      set BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_DDR5_L2A.xml
    ) else (
      set BIOS_IMAGE_CONFIG_FILE_NAME=BIOSImageDirectory32M_DDR5.xml
    )
  )
)

if "%KVM_SUPPORT%"=="YES" (
  ::Copy file KvmEngine if exist
  IF EXIST %KVM_PLATFORM_PATH%\KvmEngine.csbin (
    @echo copy %KVM_PLATFORM_PATH%\*.*                  %PSP_FW_PATH% /Y
    copy %KVM_PLATFORM_PATH%\*.*                        %PSP_FW_PATH% /Y 
  )
)

@echo PathPspDir %PSP_PLATFORM_PATH%\%BIOS_IMAGE_CONFIG_FILE_NAME%
@IF %L2A_ONLY% == YES (
  %PSP_PLATFORM_PATH%\Tools\PatchPspDir.exe -p %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt ^
                                            -t %PSP_PLATFORM_PATH%\PspPcdMapTableConfig_L2A.py ^
                                            -i %PSP_PLATFORM_PATH%\%BIOS_IMAGE_CONFIG_FILE_NAME%
) else (
  %PSP_PLATFORM_PATH%\Tools\PatchPspDir.exe -p %WORKSPACE%\Build\%PROJECT_PKG%\%TARGET%_%TOOL_CHAIN_TAG%\Ia32\PcdList.txt ^
                                            -t %PSP_PLATFORM_PATH%\PspPcdMapTableConfig.py ^
                                            -i %PSP_PLATFORM_PATH%\%BIOS_IMAGE_CONFIG_FILE_NAME%
)
if %ERRORLEVEL% NEQ 0 goto ERR_END

copy %PSP_PLATFORM_PATH%\%BIOS_IMAGE_CONFIG_FILE_NAME% %BIOS_FV_PATH% /y
set BIOS_IMAGE_CONFIG_FILE=%BIOS_FV_PATH%\%BIOS_IMAGE_CONFIG_FILE_NAME%

if "%KEY_MODE%"=="NOSIGN" (
  REM remove the line that conatins RTMSignature string for NOSIGN mode
  @findstr /v "RTMSignature" %BIOS_IMAGE_CONFIG_FILE% > %BIOS_FV_PATH%\BIOSImageDirectory-NOSIGN.xml
  set BIOS_IMAGE_CONFIG_FILE=%BIOS_FV_PATH%\BIOSImageDirectory-NOSIGN.xml
)

@echo BIOS_IMAGE_CONFIG_FILE %BIOS_IMAGE_CONFIG_FILE%
set OUTPUT_BIOS=%BIOSNAME%.FD
call %PSP_TEMP_PATH%\BuildPspImage.bat %FD_NAME%.fd %RTM_FILE% %BIOS_IMAGE_CONFIG_FILE% %OUTPUT_BIOS% %KEY_MODE%

if %ERRORLEVEL% NEQ 0 goto ERR_END
::copy %PSP_TEMP_PATH%\TEMP\OUTPUT\%OUTPUT_BIOS% %WORKSPACE%\%OUTPUT_BIOS% /Y /B
::Copy back the updated binary for further operation
::copy %PSP_TEMP_PATH%\%OUTPUT_BIOS% %BIOS_FV_PATH%\%FD_NAME%.fd /Y /B
::@echo Final BIOS @ %WORKSPACE%\%OUTPUT_BIOS%
copy %PSP_TEMP_PATH%\%OUTPUT_BIOS% %WORKSPACE%\%PROJECT_REL_PATH%\%PROJECT_PKG%\BIOS /Y /B

if "%KVM_SUPPORT%"=="YES" (
  ::Remove the copy file KvmEngine if exist
  if exist %KVM_PLATFORM_PATH%\KvmEngine.csbin (
    @echo del %PSP_FW_PATH%\KvmEngine.csbin /S /Q
    del %PSP_FW_PATH%\KvmEngine.csbin /S /Q
  )
)

goto END

:ERR_END
  @exit /B 2

:END
