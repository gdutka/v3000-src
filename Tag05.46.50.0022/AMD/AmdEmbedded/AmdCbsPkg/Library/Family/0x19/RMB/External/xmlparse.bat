:: *****************************************************************************
:: *
:: * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
:: *
:: ******************************************************************************

@REM call CBS XML parse tool
@SET CBS_LIB_FAMILY_PATH=Library\Family\0x19\RMB\External
@SET CBS_AGESA_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Include
@SET CBS_BUILD_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Build
IF NOT EXIST %CBS_AGESA_PATH% @SET CBS_AGESA_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Include
@SET FIRMWARE_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaPkg\Addendum\Apcb\Inc\RMB
IF NOT EXIST %FIRMWARE_PATH% @SET FIRMWARE_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaPkg\Addendum\Apcb\Inc\RMB

IF "%PERL_PATH%" == "" (
  set PERL_PATH=C:\Strawberry\Perl\bin
)
set PERL_LIB_PATH=%PERL_PATH%\..\lib
SET PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\Perl\c\bin;%PATH%;

:: set Vender String
@if "%CBS_VENDER_STRING%"=="" (
  set CBS_VENDER_STRING=NULL
)

:: set CBS_PROMONTORY_21_SUPPORT=0 disable Promontory 21 support
:: set CBS_PROMONTORY_21_SUPPORT=1 enable Promontory 21 support
@if "%CBS_PROMONTORY_21_SUPPORT%"=="" (
  set CBS_PROMONTORY_21_SUPPORT=1
)

:: set CBS_TURNER_SUPPORT=0 disable Turner support
:: set CBS_TURNER_SUPPORT=1 enable Turner support
@if "%CBS_TURNER_SUPPORT%"=="" (
  set CBS_TURNER_SUPPORT=0
)

@if /I "%1" == "" (
  @if exist %CBS_BUILD_PATH%\ResourceRMB\AmdCbsVariable.h @rmdir /s /q %CBS_BUILD_PATH%\ResourceRMB
@echo %PERL_PATH%\perl.exe -I%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools -I%PERL_LIB_PATH% %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\CBSgenerate.pl
@echo   -i %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\%CBS_LIB_FAMILY_PATH%\Rembrandt\eSetupRembrandt.xml
@echo   -o %CBS_BUILD_PATH%\ResourceRMB --version external -b enable  -t 0 -x 0 -r 0 -y %CBS_VENDER_STRING% --cpt21Support %CBS_PROMONTORY_21_SUPPORT% --cTnrSupport %CBS_TURNER_SUPPORT%
  %PERL_PATH%\perl.exe -I%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools -I%PERL_LIB_PATH% %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\CBSgenerate.pl^
  -i %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\%CBS_LIB_FAMILY_PATH%\Rembrandt\eSetupRembrandt.xml^
  -o %CBS_BUILD_PATH%\ResourceRMB --version external -b enable  -t 0 -x 0 -r 0 -y %CBS_VENDER_STRING% --cpt21Support %CBS_PROMONTORY_21_SUPPORT% --cTnrSupport %CBS_TURNER_SUPPORT%
  @REM Only copy files if they differ.  Prevents needless rebuild of files.
  fc %CBS_BUILD_PATH%\ResourceRMB\IdsNvDefRMB.h %CBS_AGESA_PATH%\IdsNvDefRMB.h > nul 2>&1  || @copy /y %CBS_BUILD_PATH%\ResourceRMB\IdsNvDefRMB.h %CBS_AGESA_PATH%
)
@if /I "%1" == "EMBFP7" (
  @if exist %CBS_BUILD_PATH%\ResourceRMB\AmdCbsVariable.h @rmdir /s /q %CBS_BUILD_PATH%\ResourceRMB
@echo %PERL_PATH%\perl.exe -I%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools -I%PERL_LIB_PATH% %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\CBSgenerate.pl
@echo   -i %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\%CBS_LIB_FAMILY_PATH%\Rembrandt\eSetupFox.xml
@echo   -o %CBS_BUILD_PATH%\ResourceRMB --version external -b enable  -t 0 -x 0 -r 0 -y %CBS_VENDER_STRING% --cpt21Support %CBS_PROMONTORY_21_SUPPORT% --cTnrSupport %CBS_TURNER_SUPPORT%
  %PERL_PATH%\perl.exe -I%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools -I%PERL_LIB_PATH% %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\CBSgenerate.pl^
  -i %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\%CBS_LIB_FAMILY_PATH%\Rembrandt\eSetupFox.xml^
  -o %CBS_BUILD_PATH%\ResourceRMB --version external -b enable  -t 0 -x 0 -r 0 -y %CBS_VENDER_STRING% --cpt21Support %CBS_PROMONTORY_21_SUPPORT% --cTnrSupport %CBS_TURNER_SUPPORT%
  @REM Only copy files if they differ.  Prevents needless rebuild of files.
  fc %CBS_BUILD_PATH%\ResourceRMB\IdsNvDefRMB.h %CBS_AGESA_PATH%\IdsNvDefRMB.h > nul 2>&1  || @copy /y %CBS_BUILD_PATH%\ResourceRMB\IdsNvDefRMB.h %CBS_AGESA_PATH%
)