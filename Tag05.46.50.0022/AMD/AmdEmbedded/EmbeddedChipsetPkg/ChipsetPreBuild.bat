@REM
@REM Chipset PreBuild batch file
@REM
@REM ******************************************************************************
@REM * Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
@REM *
@REM * You may not reproduce, distribute, publish, display, perform, modify, adapt,
@REM * transmit, broadcast, present, recite, release, license or otherwise exploit
@REM * any part of this publication in any form, by any means, without the prior
@REM * written permission of Insyde Software Corporation.
@REM *
@REM ******************************************************************************

@REM     Add Chispet specific pre-build process here

@REM ------------Override GenOemRomSig.inf with GenOemRomSigMsvc2010.inf if using MSVC2010-------------#
@if "%SDK_DIR%" == "%TOOL_CHAIN_DRV%\%DEVTLS_DIR%\Microsoft SDKs\Windows\v7.0A" (
  @if not exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig_bak.inf (
      @echo copy GenOemRomSig.inf to GenOemRomSig_bak.inf
      copy %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig.inf %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig_bak.inf /y
  )
  @echo copy GenOemRomSigMsvc2010.inf to GenOemRomSig.inf
  copy %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSigMsvc2010.inf %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig.inf /y
)

@if not exist %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi_bak.h (
    copy %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi.h %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi_bak.h /y
)
copy %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Override\EDK2\MdePkg\Include\Register\Intel\StmApi.h %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi.h /y

@if not exist %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap_bak.h (
    copy %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap.h %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap_bak.h /y
)
copy %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Override\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap.h %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap.h /y
