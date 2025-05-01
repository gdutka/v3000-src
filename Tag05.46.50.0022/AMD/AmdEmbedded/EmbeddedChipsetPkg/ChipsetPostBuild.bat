@REM
@REM Chipset PostBuild batch file
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

@REM     Add Chispet specific post-build process here

@REM ------------Restore GenOemRomSig.inf file -------------#
@if "%SDK_DIR%" == "%TOOL_CHAIN_DRV%\%DEVTLS_DIR%\Microsoft SDKs\Windows\v7.0A" (
  @if exist %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig_bak.inf (
    copy %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig_bak.inf %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig.inf /y
    del %WORKSPACE%\%CHIPSET_REL_PATH%\%CHIPSET_PKG%\Tools\GenOemRomSig\GenOemRomSig_bak.inf
  ) else (
    @echo ERROR: Restore Fail: GenOemRomSig.inf.
  )
)

@if exist %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi_bak.h (
  copy %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi_bak.h %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi.h /y
  del %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\StmApi_bak.h
) else (
  @echo ERROR: Restore Fail: StmApi.h.
)

@if exist %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap_bak.h (
  copy %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap_bak.h %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap.h /y
  del %WORKSPACE%\EDK2\MdePkg\Include\Register\Intel\SmramSaveStateMap_bak.h
) else (
  @echo ERROR: Restore Fail: SmramSaveStateMap.h.
)
