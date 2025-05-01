#;*****************************************************************************
#; Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
#;
#;*****************************************************************************


[LibraryClasses.common.SEC]
  CcxNonSmmResumeSecLib|AgesaModulePkg/Library/CcxNonSmmResumeSecLib/CcxNonSmmResumeSecLib.inf

[LibraryClasses.Common.PEIM]
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapPeiLib/AmdHeapPeiLib.inf
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/PeiFabricResourceSizeForEachRbLib/PeiFabricResourceSizeForEachRbLib.inf
  FabricResourceReportToGcdLib|AgesaModulePkg/Library/FabricResourceReportToGcdNullLib/FabricResourceReportToGcdNullLib.inf
  CcxSetMmioCfgBaseLib|AgesaModulePkg/Library/CcxSetMmioCfgBaseLib/CcxSetMmioCfgBaseLib.inf
  IdsLib|AgesaModulePkg/Library/IdsPeiLib/IdsPeiLib.inf
  FchPeiLibV9|AgesaModulePkg/Library/FchPeiLib/FchPeiLib.inf
  FchKeithPeiLib|AgesaModulePkg/Fch/Keith/FchKeithCore/FchKeithLibPei.inf
  FchInitHookLibPei|AgesaModulePkg/Library/FchInitHookLib/FchInitHookLibPei.inf
  GnbHeapLib|AgesaModulePkg/Library/GnbHeapPeiLib/GnbHeapPeiLib.inf
  AmdPspCommonLib|AgesaModulePkg/Library/AmdPspCommonLibPei/AmdPspCommonLibPei.inf
  ApobCommonServiceLib|AgesaModulePkg/Library/ApobCommonServiceLibPei/ApobCommonServiceLibPei.inf
  PeiSocLogicalIdServicesLib|AgesaModulePkg/Library/PeiSocLogicalIdServicesLib/PeiSocLogicalIdServicesLib.inf
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Pei/AmdTableHookPeiLibV2.inf
  AmdCapsuleLib|AgesaModulePkg/Library/AmdCapsuleLibPei/AmdCapsuleLibPei.inf
  PeiSocBistLogging2Lib|AgesaModulePkg/Library/PeiSocBistLogging2Lib/PeiSocBistLogging2Lib.inf
  AmdIOMMUDmarLib|AgesaModulePkg/Nbio/Library/IommuDmarLib/PEI/AmdIOMMUDmarLib.inf
  PcieComplexDefaultsLib|AgesaModulePkg/Nbio/Library/PcieComplexDefaultsLib/PcieComplexDefaultsLib.inf
  CcxPeiSmmAccessLib|AgesaModulePkg/Library/PeiCcxSmmAccessLib/PeiCcxSmmAccessLib.inf
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Null/AmdPspRegMuxLibV2.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibPei/AmdCfgPcdBufLibPei.inf

  ## MEM Libs
  AmdMemBaseLib|AgesaModulePkg/Library/Mem/BaseLib/AmdMemBaseLib.inf

  ## IDS LIB
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibPei/AmdIdsHookLib.inf
  CcxZen3RmbIdsHookLibPei|AgesaModulePkg/Library/CcxZen3RmbIdsHookLib/Pei/CcxZen3RmbIdsHookLibPei.inf
  FabricIdsHookRmbLibPei|AgesaModulePkg/Library/FabricIdsHookRmbLib/Pei/FabricIdsHookRmbLibPei.inf
  NbioIdsHookRmbLibPei|AgesaModulePkg/Library/NbioIdsHookRmbLib/Pei/NbioIdsHookRmbLibPei.inf
  FchIdsHookRmbLibPei|AgesaModulePkg/Library/FchIdsHookRmbLib/Pei/FchIdsHookRmbLibPei.inf

[LibraryClasses.Common.DXE_DRIVER]
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/S3Save/AmdS3SaveLib.inf
  IdsLib|AgesaModulePkg/Library/IdsDxeLib/IdsDxeLib.inf
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/DxeFabricResourceSizeForEachRbLib/DxeFabricResourceSizeForEachRbLib.inf
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf
  GnbHeapLib|AgesaModulePkg/Library/GnbHeapDxeLib/GnbHeapDxeLib.inf
  AmdPspCommonLib|AgesaModulePkg/Library/AmdPspCommonLibDxe/AmdPspCommonLibDxe.inf
  ApobCommonServiceLib|AgesaModulePkg/Library/ApobCommonServiceLibDxe/ApobCommonServiceLibDxe.inf
  AmdPspFlashAccLib|AgesaModulePkg/Library/AmdPspFlashAccLibDxe/AmdPspFlashAccLibDxe.inf
  AmdPspFlashUpdateLib|AgesaModulePkg/Library/AmdPspFlashUpdateLib/AmdPspFlashUpdateLib.inf
  AmdTpm2CommandLib|AgesaModulePkg/Library/AmdTpm2CommandLib/AmdTpm2CommandLib.inf
#  ApcbSspLib|AgesaModulePkg/Library/ApcbSspLib/ApcbLib.inf                                                             <SSP|MTS>
  DxeSocLogicalIdServicesLib|AgesaModulePkg/Library/DxeSocLogicalIdServicesLib/DxeSocLogicalIdServicesLib.inf
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Dxe/AmdTableHookDxeLibV2.inf
  AmdCapsuleLib|AgesaModulePkg/Library/AmdCapsuleLibDxe/AmdCapsuleLibDxe.inf
  CcxMpServicesLib|AgesaModulePkg/Library/CcxMpServicesDxeLib/CcxMpServicesDxeLib.inf
  CcxSmmAccess2Lib|AgesaModulePkg/Library/DxeCcxSmmAccess2Lib/DxeCcxSmmAccess2Lib.inf
  AmdIOMMUDmarLib|AgesaModulePkg/Nbio/Library/IommuDmarLib/DXE/AmdIOMMUDmarLib.inf
  CcxCppcLib|AgesaModulePkg/Library/DxeCcxCppcLib/DxeCcxCppcLib.inf
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Dxe/AmdPspRegMuxLibV2.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibDxe/AmdCfgPcdBufLibDxe.inf

  ## MEM Lib
  ApcbLibV3|AgesaModulePkg/Library/ApcbLibV3/ApcbLibV3.inf
  ApcbVariableLibV3|AgesaModulePkg/Library/ApcbVariableLibV3/ApcbVariableLibV3.inf
  ApcbChecksumLibV3|AgesaModulePkg/Library/ApcbHmacChecksumLibV3/ApcbHmacChecksumLibV3.inf
  AmdPspDxeSmmBufLib|AgesaModulePkg/Library/AmdPspDxeSmmBufLib/AmdPspDxeSmmBufLib.inf

  ## IDS LIB
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibDxe/AmdIdsHookLib.inf
  CcxZen3RmbIdsHookLibDxe|AgesaModulePkg/Library/CcxZen3RmbIdsHookLib/Dxe/CcxZen3RmbIdsHookLibDxe.inf
  FabricIdsHookRmbLibDxe|AgesaModulePkg/Library/FabricIdsHookRmbLib/Dxe/FabricIdsHookRmbLibDxe.inf
  NbioIdsHookRmbLibDxe|AgesaModulePkg/Library/NbioIdsHookRmbLib/Dxe/NbioIdsHookRmbLibDxe.inf
  FchIdsHookRmbLibDxe|AgesaModulePkg/Library/FchIdsHookRmbLib/Dxe/FchIdsHookRmbLibDxe.inf

[LibraryClasses.Common.DXE_SMM_DRIVER]
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/S3Save/AmdS3SaveLib.inf
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/DxeFabricResourceSizeForEachRbLib/DxeFabricResourceSizeForEachRbLib.inf
#@todo add specifiic SMM Lib instance, DXE Lib is depend on gBS service exisitance
  IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibDxe/AmdIdsHookLib.inf
  AmdPspCommonLib|AgesaModulePkg/Library/AmdPspCommonLibDxe/AmdPspCommonLibDxe.inf
!if $(SMM_ISOLATION_LEVEL30_SUPPORT) == TRUE
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLibSmmIso/AmdPspMmioLib.inf
!else
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
!endif
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Dxe/AmdPspRegMuxLibV2.inf
  ApobCommonServiceLib|AgesaModulePkg/Library/ApobCommonServiceLibDxe/ApobCommonServiceLibDxe.inf
  AmdPspFlashUpdateLib|AgesaModulePkg/Library/AmdPspFlashUpdateLib/AmdPspFlashUpdateLib.inf
  CcxZen3IdsHookLibSmm|AgesaModulePkg/Library/CcxZen3RmbIdsHookLib/Smm/CcxZen3RmbIdsHookLibSmm.inf
#  ApcbSspLib|AgesaModulePkg/Library/ApcbSspLib/ApcbLib.inf                                                             <SSP|MTS>
  AmdCapsuleLib|AgesaModulePkg/Library/AmdCapsuleLibDxe/AmdCapsuleLibDxe.inf
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/Dxe/AmdTableHookDxeLib.inf
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Dxe/AmdTableHookDxeLibV2.inf
  CcxMpServicesLib|AgesaModulePkg/Library/CcxMpServicesSmmLib/CcxMpServicesSmmLib.inf
  ApcbLibV3|AgesaModulePkg/Library/ApcbLibV3/ApcbLibV3.inf
  ApcbVariableLibV3|AgesaModulePkg/Library/ApcbVariableLibV3/ApcbVariableLibV3.inf
  ApcbChecksumLibV3|AgesaModulePkg/Library/ApcbHmacChecksumLibV3/ApcbHmacChecksumLibV3.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibDxe/AmdCfgPcdBufLibDxe.inf
  AmdPspDxeSmmBufLib|AgesaModulePkg/Library/AmdPspDxeSmmBufLib/AmdPspDxeSmmBufLib.inf

[LibraryClasses.COMMON.DXE_RUNTIME_DRIVER]
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2DxeRt/AmdPspRegMuxLibV2.inf
  AmdStbLib|AgesaModulePkg/Library/AmdStbLibNull/AmdStbLibNull.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibDxe/AmdCfgPcdBufLibDxe.inf

[LibraryClasses.common.PEI_CORE]
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapPeiLib/AmdHeapPeiLib.inf
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Pei/AmdTableHookPeiLibV2.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibPei/AmdCfgPcdBufLibPei.inf

[LibraryClasses]
  #
  # Agesa specific common libraries
  #
  AmdBaseLib|AgesaModulePkg/Library/AmdBaseLib/AmdBaseLib.inf
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/DxeFabricResourceSizeForEachRbLib/DxeFabricResourceSizeForEachRbLib.inf
  FabricResourceReportToGcdLib|AgesaModulePkg/Library/FabricResourceReportToGcdLib/FabricResourceReportToGcdLib.inf
  AmdIdsDebugPrintLib|AgesaModulePkg/Library/AmdIdsDebugPrintLib/AmdIdsDebugPrintLib.inf
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibNull/AmdIdsHookLibNull.inf
  AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
  IdsMiscLib|AgesaModulePkg/Library/IdsMiscLib/IdsMiscLib.inf
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapLibNull/AmdHeapLibNull.inf
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibNull/AmdCfgPcdBufLibNull.inf

  AmdSocBaseLib|AgesaModulePkg/Library/AmdSocBaseLib/AmdSocBaseLib.inf
  AmdErrorLogLib|AgesaModulePkg/Library/AmdErrorLogLib/AmdErrorLogLib.inf
  AmdTableLib|AgesaModulePkg/Library/AmdTableLibV2/AmdTableLibV2.inf
  SocCmnIdsHookRmbLibPei|AgesaModulePkg/Library/SocCmnIdsHookRmbLib/Pei/SocCmnIdsHookRmbLibPei.inf
  SocCmnIdsHookRmbLibDxe|AgesaModulePkg/Library/SocCmnIdsHookRmbLib/Dxe/SocCmnIdsHookRmbLibDxe.inf
  BaseCoreLogicalIdLib|AgesaModulePkg/Library/BaseCoreLogicalIdX86Lib/BaseCoreLogicalIdX86Lib.inf
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  AmdStbLib|AgesaModulePkg/Library/AmdStbLib/AmdStbLib.inf
  AmdEmulationFlagLib|AgesaModulePkg/Library/AmdEmulationFlagLib/AmdEmulationFlagLib.inf
  PresiliconControlLib|AgesaModulePkg/Library/PresiliconControlRmbLib/PresiliconControlRmbLib.inf
  AgesaConfigLib|AgesaModulePkg/Library/AgesaConfigLib/AgesaConfigLib.inf

  ## PSP Libs
  AmdPspMboxLibV1|AgesaModulePkg/Library/AmdPspMboxLibV1/AmdPspMboxLibV1.inf
  AmdPspBaseLibV1|AgesaModulePkg/Library/AmdPspBaseLibV1/AmdPspBaseLibV1.inf
  AmdPspBaseLibV2|AgesaModulePkg/Library/AmdPspBaseLibV2/AmdPspBaseLibV2.inf
  AmdPspMboxLibV2|AgesaModulePkg/Library/AmdPspMboxLibV2/AmdPspMboxLibV2.inf
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Null/AmdPspRegMuxLibV2.inf
  AmdPspApobLib|AgesaModulePkg/Library/AmdPspApobLib/AmdPspApobLib.inf
  AmdPspFtpmLib|AgesaModulePkg/Library/AmdPspFtpmLib/AmdPspFtpmLib.inf
  AmdHspFtpmLib|AgesaModulePkg/Library/AmdHspFtpmLib/AmdHspFtpmLib.inf
  HspLib|AgesaModulePkg/Library/HspLib/HspLib.inf
  AmdPspFlashAccLib|AgesaModulePkg/Library/AmdPspFlashAccLibNull/AmdPspFlashAccLibNull.inf
  PspIdsHookLibDxe|AgesaModulePkg/Library/PspIdsHookLib/Dxe/PspIdsHookLibDxe.inf
  ApobRmbLib|AgesaModulePkg/Library/ApobRmbLib/ApobRmbLib.inf
  ApobDummyLib|AgesaModulePkg/Library/ApobDummyLib/ApobDummyLib.inf
  AmdPspPsbFusingLib|AgesaModulePkg/Library/AmdPspPsbFusingLib/AmdPspPsbFusingLib.inf
  AmdPspHstiStateLib|AgesaModulePkg/Library/AmdPspHstiStateLib/AmdPspHstiStateLib.inf
  MpmMboxLib|AgesaModulePkg/Library/MpmMboxLib/MpmMboxLib.inf
  MpmLib|AgesaModulePkg/Library/MpmLib/MpmLib.inf
  MpmFuncLib|AgesaModulePkg/Library/MpmFuncLib/MpmFuncLib.inf
  MpmDevLib|AgesaModulePkg/Library/MpmDevLib/MpmDevLib.inf
  MpmBiosCmdLib|AgesaModulePkg/Library/MpmBiosCmdLib/MpmBiosCmdLib.inf
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
  AmdDirectoryBaseLib|AgesaModulePkg/Library/AmdDirectoryBaseLib/AmdDirectoryBaseLib.inf

  ## CCX Lib
  CcxBaseX86Lib|AgesaModulePkg/Library/CcxBaseX86Lib/CcxBaseX86Lib.inf
  CcxBistLib|AgesaModulePkg/Library/CcxBistLib/CcxBistLib.inf
  CcxPspLib|AgesaModulePkg/Library/CcxPspLib/CcxPspLib.inf
  CcxHaltLib|AgesaModulePkg/Library/CcxHaltLib/CcxHaltLib.inf
  CcxMicrocodePatchLib|AgesaModulePkg/Library/CcxMicrocodePatchLib/CcxMicrocodePatchLib.inf
  CcxRolesLib|AgesaModulePkg/Library/CcxRolesX86Lib/CcxRolesX86Lib.inf
  CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  CcxStallLib|AgesaModulePkg/Library/CcxStallZenLib/CcxStallZenLib.inf
  DxeCcxBaseX86ServicesLib|AgesaModulePkg/Library/DxeCcxBaseX86ServicesLib/DxeCcxBaseX86ServicesLib.inf
  CcxApicZen3Lib|AgesaModulePkg/Library/CcxApicZen3Lib/CcxApicZen3Lib.inf

  ## DF Lib
  FabricPstatesLib|AgesaModulePkg/Library/FabricPstatesZpLib/FabricPstatesZpLib.inf
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
  PeiFabricTopologyServices2Lib|AgesaModulePkg/Library/PeiFabricTopologyServices2Lib/PeiFabricTopologyServices2Lib.inf
  DxeFabricTopologyServices2Lib|AgesaModulePkg/Library/DxeFabricTopologyServices2Lib/DxeFabricTopologyServices2Lib.inf
  SmmFabricTopologyServices2Lib|AgesaModulePkg/Library/SmmFabricTopologyServices2Lib/SmmFabricTopologyServices2Lib.inf
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf

  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceInitBasedOnNv3Lib.inf
  PeiFabricResourceManagerServicesLib|AgesaModulePkg/Library/PeiFabricResourceManagerServicesLib/PeiFabricResourceManagerServicesLib.inf
  DxeFabricResourceManagerServicesLib|AgesaModulePkg/Library/DxeFabricResourceManagerServicesLib/DxeFabricResourceManagerServicesLib.inf

  ## MEM Lib
  AmdMemBaseLib|AgesaModulePkg/Library/Mem/BaseLib/AmdMemBaseLib.inf
  MemRestoreLib|AgesaModulePkg/Library/MemRestoreLib/MemRestoreLib.inf

  ## Gnb Lib
  GnbCommonLib|AgesaModulePkg/Library/GnbCommonLib/GnbCommonLib.inf
  GnbMemAccLib|AgesaModulePkg/Library/GnbMemAccLib/GnbMemAccLib.inf
  GnbIoAccLib|AgesaModulePkg/Library/GnbIoAccLib/GnbIoAccLib.inf
  GnbPciAccLib|AgesaModulePkg/Library/GnbPciAccLib/GnbPciAccLib.inf
  GnbCpuAccLib|AgesaModulePkg/Library/GnbCpuAccLib/GnbCpuAccLib.inf
  GnbPciLib|AgesaModulePkg/Library/GnbPciLib/GnbPciLib.inf
  GnbLib|AgesaModulePkg/Library/GnbLib/GnbLib.inf
  NbioHandleLib|AgesaModulePkg/Library/NbioHandleLib/NbioHandleLib.inf
  NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  IvrsDeviceInfoLib|AgesaModulePkg/Nbio/Library/IvrsDeviceDfltLib/IvrsDeviceDfltLib.inf
  PcieConfigLib|AgesaModulePkg/Library/PcieConfigLib/PcieConfigLib.inf
  GnbSSocketLib|AgesaModulePkg/Library/GnbSSocketLib/GnbSSocketLib.inf
  GnbRegisterAccZpLib|AgesaModulePkg/Library/GnbRegisterAccZpLib/GnbRegisterAccZpLib.inf
  NbioRegisterAccLib|AgesaModulePkg/Library/NbioRegisterAccLib/NbioRegisterAcc.inf
  NbioSmuV13Lib|AgesaModulePkg/Library/NbioSmuV13Lib/NbioSmuV13Lib.inf
  NbioTableBlastLib|AgesaModulePkg/Library/NbioTableBlastLib/NbioTableBlastLib.inf

  DxioLibV2|AgesaModulePkg/Library/DxioLibV2/DxioLibV2.inf
  PcieMiscCommLib|AgesaModulePkg/Library/PcieMiscCommLib/PcieMiscCommLib.inf
  NbioAzaliaLib|AgesaModulePkg/Library/NbioAzaliaLib/NbioAzaliaLib.inf
  SmnAccessLib|AgesaModulePkg/Library/SmnAccessLib/SmnAccessLib.inf
  NbioUtilLib|AgesaModulePkg/Library/NbioUtilLib/NbioUtilLib.inf
  BxbInitLibV1|AgesaModulePkg/Library/BxbNbio/BxbNullLib/BxbNullLib.inf
  BxbSocLibRMB|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibRMB/BxbSocLibRMB.inf
  OemClkReqControlLib|AgesaModulePkg/Library/NbioClkReqControlLibNull/NbioClkReqControlLibNull.inf

  ## Fch Lib
  FchBaseLib|AgesaModulePkg/Library/FchBaseLib/FchBaseLib.inf
  ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchBaseResetSystemLib.inf
  FchDxeLibV9|AgesaModulePkg/Library/FchDxeLib/FchDxeLib.inf
  FchSmmLibV9|AgesaModulePkg/Library/FchSmmLib/FchSmmLib.inf
  FchKeithDxeLib|AgesaModulePkg/Fch/Keith/FchKeithCore/FchKeithLibDxe.inf
  FchInitHookLibDxe|AgesaModulePkg/Library/FchInitHookLib/FchInitHookLibDxe.inf
  FchIdsHookLib|AgesaModulePkg/Library/FchIdsHookLib/FchIdsHookLib.inf
  RtcLib|AgesaModulePkg/Library/AmdRtclib/AmdRtcLib.inf

  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom2Lib.inf

  ## Usb4 Lib
  AmdUsb4BaseLib|AgesaModulePkg/Library/AmdUsb4Lib/AmdUsb4Lib.inf

  ## DashAsfBase
  DashAsfBaseLib|AgesaModulePkg/Library/DashAsfBaseLib/DashAsfBaseLib.inf

  ## DashAsfSmbusLib
  DashAsfSmbusLib|AgesaModulePkg/Library/DashAsfSmbusLib/DashAsfSmbusLib.inf

[Components.IA32]
  AgesaModulePkg/Debug/AmdIdsDebugPrintPei/AmdIdsDebugPrintPei.inf

  ##PSP Drivers
  AgesaModulePkg/Psp/AmdPspPeiV2Rmb/AmdPspPeiV2.inf
  AgesaModulePkg/Psp/AmdPspFtpmPei/AmdPspFtpmPei.inf
  AgesaModulePkg/Psp/AmdHspFtpmPei/AmdHspFtpmPei.inf
  AgesaModulePkg/Psp/ApobDrv/ApobRmbPei/ApobRmbPei.inf
  AgesaModulePkg/Psp/AmdPspIntrusionDetection/AmdPspIntrusionDetectionPei.inf
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPei/MpmPei.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmAsf/MpmAsfPei/MpmAsfPei.inf
!endif

  AgesaModulePkg/Ccx/Zen3/Pei/Rmb/AmdCcxZen3RmbPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3V2Lib/CcxResetTablesZen3V2Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
  }
  AgesaModulePkg/Fabric/RMB/FabricRmbPei/AmdFabricRmbPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }
  AgesaModulePkg/Nbio/GFX/RMB/PEI/AmdNbioGfxRMBPei.inf
  AgesaModulePkg/Nbio/SMU/RMB/PEI/SmuV13PeiRMB.inf
  AgesaModulePkg/Nbio/PCIe/RMB/PEI/AmdNbioPcieRMBPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }
  AgesaModulePkg/Nbio/NbioBase/RMB/PEI/AmdNbioBaseRMBPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }
  AgesaModulePkg/Nbio/IOMMU/RMB/PEI/AmdNbioIOMMURMBPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
  }

  AgesaModulePkg/Fch/Keith/FchKeithPei/FchPei.inf
  AgesaModulePkg/Fch/Keith/FchKeithSmbusPei/Smbus.inf
  AgesaModulePkg/Mem/AmdMemFp7Pei/AmdMemFp7Pei.inf
  AgesaModulePkg/Soc/AmdSocFp7RmbPei/AmdSocFp7RmbPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesRmbLib/PeiSocZen3ServicesRmbLib.inf
  }

  AgesaModulePkg/Mem/AmdMemChanXLatDummyPei/MemChanXLatDummyPei.inf
  AgesaModulePkg/Mem/AmdMemSmbiosV2RmbPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RmbLpd5Lib/MemSmbiosV2Lib.inf
  }
  AgesaModulePkg/Mem/AmdMemRestorePei/MemRestorePei.inf
  AgesaModulePkg/ErrorLog/AmdErrorLogPei/AmdErrorLogPei.inf
  AgesaModulePkg/Nbio/EarlyPhase/AmdNbioEarlyPhaseRMBPei/AmdNbioEarlyPhaseRMBPei.inf
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimRmb/AmdMemoryHobInfoPeim.inf
  AgesaModulePkg/Universal/Version/AmdVersionPei/AmdVersionPei.inf

  AgesaModulePkg/Usb4/AmdUsb4Pei/AmdUsb4Pei.inf

[Components.X64]
  AgesaModulePkg/Debug/AmdIdsDebugPrintDxe/AmdIdsDebugPrintDxe.inf
  AgesaModulePkg/Mem/AmdMemSmbiosV2Dxe/AmdMemSmbiosV2Dxe.inf
  AgesaModulePkg/Mem/AmdMemFp7Dxe/AmdMemFp7Dxe.inf
  AgesaModulePkg/Mem/AmdMemRestoreDxe/MemRestoreDxe.inf
  AgesaModulePkg/Mem/AmdMemRestoreDxeV2/MemRestoreDxe.inf
  AgesaModulePkg/Psp/ApcbDrv/ApcbV3Dxe/ApcbV3Dxe.inf
  AgesaModulePkg/Psp/ApcbDrv/ApcbV3Smm/ApcbV3Smm.inf

  AgesaModulePkg/Psp/AmdPspDxeV2Rmb/AmdPspDxeV2.inf
  AgesaModulePkg/Psp/AmdPspDxeV2Rmb/AmdDrtmAsl.inf
  AgesaModulePkg/Psp/AmdPspP2CmboxV2/AmdPspP2CmboxV2.inf
  AgesaModulePkg/Psp/AmdPspP2CmboxV2/AmdPspP2CmboxV2SmmBuffer.inf

  AgesaModulePkg/Psp/AmdPspSmmV2/AmdPspSmmV2.inf

  AgesaModulePkg/Psp/AmdPspFtpmDxe/AmdPspFtpmDxe.inf
  AgesaModulePkg/Psp/AmdHspFtpmDxe/AmdHspFtpmDxe.inf
  AgesaModulePkg/Psp/AmdHstiV2/AmdHstiV2.inf

  AgesaModulePkg/Psp/ApobDrv/ApobRmbDxe/ApobRmbDxe.inf
  AgesaModulePkg/Psp/AmdPspIntrusionDetection/AmdPspIntrusionDetectionDxe.inf
  AgesaModulePkg/Psp/AmdPspIntrusionDetection/AmdPspIntrusionDetectionSmm.inf

!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmDxe/MpmDxe.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmDxe/AmdMpmAsl.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmKvm/MpmKvm.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmAsf/MpmAsfDxe/MpmAsfDxe.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmBatteryInfo/MpmBatteryInfo.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmBase/MpmPldmBase.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmSmbios/MpmPldmSmbios.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmBcc/MpmPldmBcc.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmMonitor/MpmPldmSensor.inf
!endif
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmSerialIoDxe/MpmSerialIoDxe.inf
!endif

  AgesaModulePkg/Ccx/Zen3/Dxe/Rmb/AmdCcxZen3RmbDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen3Lib/CcxSmbiosZen3Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }
  AgesaModulePkg/Ccx/Zen3/Smm/AmdCcxZen3Smm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
  }
  AgesaModulePkg/Fabric/RMB/FabricRmbDxe/AmdFabricRmbDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
  }
  AgesaModulePkg/Fabric/RMB/FabricRmbSmm/AmdFabricRmbSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }
  AgesaModulePkg/Soc/AmdSocFp7RmbDxe/AmdSocFp7RmbDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesRmbLib/DxeSocZen3ServicesRmbLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }

  AgesaModulePkg/ErrorLog/AmdErrorLogDxe/AmdErrorLogDxe.inf
  AgesaModulePkg/ErrorLog/AmdErrorLogDisplayDxe/AmdErrorLogDisplayDxe.inf

  ## Gnb Dxe Drivers
  AgesaModulePkg/Nbio/NbioBase/RMB/DXE/AmdNbioBaseRMBDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }
  AgesaModulePkg/Nbio/SMU/RMB/DXE/SmuV13DxeRMB.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }
  AgesaModulePkg/Nbio/PCIe/RMB/DXE/AmdNbioPcieRMBDxe.inf
  AgesaModulePkg/Nbio/IOMMU/RMB/DXE/AmdNbioIOMMURMBDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }
  AgesaModulePkg/Nbio/GFX/RMB/DXE/AmdNbioGfxRMBDxe.inf
  AgesaModulePkg/Nbio/NbioALib/RMB/DXE/AmdNbioAlibRMBDxe.inf

  ## Fch Dxe Drivers
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchDxe.inf
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchKeithSsdt.inf
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchKeithDsdt.inf
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchKeithAml.inf
  AgesaModulePkg/Fch/Keith/FchKeithSmm/FchSmm.inf
  AgesaModulePkg/Fch/Keith/FchKeithSmbusDxe/SmbusLight.inf
  AgesaModulePkg/Fch/Keith/FchKeithCf9ResetDxe/Cf9Reset.inf
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Keith/FchKeithLegacyInterruptDxe/LegacyInterrupt.inf
!endif
  AgesaModulePkg/Fch/Keith/FchKeithSmmControlDxe/SmmControl.inf
  AgesaModulePkg/Fch/Keith/FchKeithSmmDispatcher/FchSmmDiagDispatcher.inf
  AgesaModulePkg/Fch/Keith/FchKeithSmmDispatcher/FchSmmDispatcher.inf
  AgesaModulePkg/Fch/Keith/FchKeithComplement/FchKTSataD3Cold/FchKTSataD3ColdSmm.inf

  # Universal
  AgesaModulePkg/Universal/Smbios/AmdSmbiosDxe.inf
  AgesaModulePkg/Universal/Acpi/AmdAcpiDxe.inf
  AgesaModulePkg/Universal/AmdSmmCommunication/AmdSmmCommunication.inf
  AgesaModulePkg/Universal/Version/AmdVersionDxe/AmdVersionDxe.inf

  ## Usb4 Dxe Driver
  AgesaModulePkg/Usb4/AmdUsb4Dxe/AmdUsb4Dxe.inf

#SMM Protection Test Driver
!if $(SMM_PROTECTION_TEST_SUPPORT) == TRUE
  AgesaModulePkg/Universal/AmdTestPkg/SmmProtectionsTest/Smm/SmmProtectionsTestSmm.inf
!endif

  ## DASH
  ## Mctp Dxe Driver
  AgesaModulePkg/Dash/MctpSmbus/DashMctpSmbusDxe.inf
  ## ASF Msg Dxe Drivers
  AgesaModulePkg/Dash/AsfMsg/AsfMsg.inf
  AgesaModulePkg/Dash/AsfTable/AsfTableAsl.inf
  ## Pldm Data
  AgesaModulePkg/Dash/DashPldm/DashPldmBase/DashPldmBase.inf
  AgesaModulePkg/Dash/DashPldm/DashPldmBcc/DashPldmBcc.inf
  AgesaModulePkg/Dash/DashPldm/DashPldmSmbios/DashPldmSmbios.inf
  ## Add Smbios Data
  AgesaModulePkg/Dash/SmbiosTypeAdd/SmbiosTypeAdd.inf

[PcdsFixedAtBuild]
  # AMD AGESA PI Name & Version string
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RembrandtPI-FP7 1.0.0.A"

  # For BIOS with A/B layout, send command to PSP no matter Recovery flag is set or not
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSendPspCommandIgnoreRecoveryFlag|TRUE

[PcdsDynamicDefault]
  # Gnb Gbs Override
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgIOMMUDynamicPgEnable|FALSE
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPcieEcrcEnablement|FALSE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdVddOffVid|300

  # CCX and Fabric Default
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableRMSS|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableFSRM|TRUE
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableERMS|TRUE

  #ACPI C3 Control = LPI
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiC3Ctrl|2
