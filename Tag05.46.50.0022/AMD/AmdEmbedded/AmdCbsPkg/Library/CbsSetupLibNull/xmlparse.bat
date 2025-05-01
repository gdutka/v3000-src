:: *****************************************************************************
:: *
:: * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
:: *
:: ******************************************************************************

@SET CBS_LIB_FAMILY_PATH=\Library\CbsSetupLibNull

@if /I "%1" == "" (
  @copy /y %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\AmdCbsPkg.dsc %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Package.dsc
  @copy /y %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\%CBS_LIB_FAMILY_PATH%\AmdCbsVariable.h %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Include\Library
  @copy /y %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\%CBS_LIB_FAMILY_PATH%\AmdCbsFormID.h %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Include\Library
) else if /I "%1" == "POSTBUILD" (
  @if exist %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Include\Library\AmdCbsVariable.h del %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Include\Library\AmdCbsVariable.h
  @if exist %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Include\Library\AmdCbsFormID.h del %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Include\Library\AmdCbsFormID.h
  @if exist %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Package.dsc del %WORKSPACE%\%CHIPSET_REL_PATH%\AmdCbsPkg\Package.dsc
)
