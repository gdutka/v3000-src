:: *****************************************************************************
:: *
:: * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
:: *
:: ******************************************************************************

@REM call Ids Id headerfile script tool
@IF EXIST %WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Include (
  @SET AGESA_INC_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Include
) ELSE (
  @SET AGESA_INC_PATH=%WORKSPACE%\%CHIPSET_REL_PATH%\AgesaModulePkg\Include
)

IF "%PERL_PATH%" == "" (
  set PERL_PATH=C:\Strawberry\Perl\bin
)

set PERL_LIB_PATH=%PERL_PATH%\..\lib
set PATH=%PERL_PATH%\..\c\bin;%PATH%;

%PERL_PATH%\perl.exe -I%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools -I%PERL_LIB_PATH% %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\IdsIdGen.pl -i %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Library -o %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Build
copy %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Build\*.h      %AGESA_INC_PATH% /Y
