@echo off

if not exist %WORKSPACE%\Build mkdir %WORKSPACE%\Build
if exist %WORKSPACE%\Build\BuildPcdValue.txt del %WORKSPACE%\Build\BuildPcdValue.txt

for /f "tokens=1-3 delims=.ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" %%a in ('GetProjectEnv PROJECT_UNI_STR_VALUE STR_CCB_VERSION'                  ) do echo|set /p="--pcd gInsydeTokenSpaceGuid.PcdH2OCcbVersion=0x%%a%%b%%c00 "                        > %WORKSPACE%\Build\BuildPcdValue.txt
for /f                                                                           %%a in ('GetProjectEnv PROJECT_UNI_STR_VALUE STR_ESRT_VERSION'                 ) do echo|set /p="--pcd gInsydeTokenSpaceGuid.PcdH2OEsrtSystemFirmwareVersion=0x%%a "                >> %WORKSPACE%\Build\BuildPcdValue.txt
for /f                                                                           %%a in ('GetProjectEnv PROJECT_UNI_STR_VALUE STR_ESRT_LOWEST_SUPPORTED_VERSION') do echo|set /p="--pcd gInsydeTokenSpaceGuid.PcdH2OEsrtSystemFirmwareLowestSupportedVersion=0x%%a " >> %WORKSPACE%\Build\BuildPcdValue.txt
for /f                                                                           %%a in ('GetProjectEnv PROJECT_UNI_STR_VALUE STR_ESRT_FIRMWARE_GUID'           ) do set g=%%a
echo|set /p="--pcd gInsydeTokenSpaceGuid.PcdH2OEsrtSystemFirmwareGuid=H{0x%g:~6,2%,0x%g:~4,2%,0x%g:~2,2%,0x%g:~0,2%,0x%g:~11,2%,0x%g:~9,2%,0x%g:~16,2%,0x%g:~14,2%,0x%g:~19,2%,0x%g:~21,2%,0x%g:~24,2%,0x%g:~26,2%,0x%g:~28,2%,0x%g:~30,2%,0x%g:~32,2%,0x%g:~34,2%} "                >> %WORKSPACE%\Build\BuildPcdValue.txt
echo|set /p="--pcd gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid=H{0x%g:~6,2%,0x%g:~4,2%,0x%g:~2,2%,0x%g:~0,2%,0x%g:~11,2%,0x%g:~9,2%,0x%g:~16,2%,0x%g:~14,2%,0x%g:~19,2%,0x%g:~21,2%,0x%g:~24,2%,0x%g:~26,2%,0x%g:~28,2%,0x%g:~30,2%,0x%g:~32,2%,0x%g:~34,2%} " >> %WORKSPACE%\Build\BuildPcdValue.txt

if not exist %WORKSPACE%\Build\BuildPcdValue.txt @echo --pcd gInsydeTokenSpaceGuid.PcdH2OCcbVersion=0 > %WORKSPACE%\Build\BuildPcdValue5.txt