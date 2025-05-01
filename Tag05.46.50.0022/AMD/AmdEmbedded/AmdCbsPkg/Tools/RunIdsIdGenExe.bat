:: *****************************************************************************
:: *
:: * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
:: *
:: ******************************************************************************

@REM call Ids Id headerfile script tool

@SET WORKSPACE=..\..


%WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\IdsIdGen.exe -i %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Library -o %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Tools\IdsIdGenExeBuild

