#;*****************************************************************************
#;
#; Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
#;
#;******************************************************************************

[LibraryClasses.common.SEC]
  CcxNonSmmResumeSecLib|AgesaModulePkg/Library/CcxNonSmmResumeSecLib/CcxNonSmmResumeSecLib.inf                         <ALL>

[LibraryClasses.Common.PEIM]
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf                                         <ALL>
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapPeiLib/AmdHeapPeiLib.inf                                                    <ALL>
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/PeiFabricResourceSizeForEachRbLib/PeiFabricResourceSizeForEachRbLib.inf   <ALL>
  FabricResourceReportToGcdLib|AgesaModulePkg/Library/FabricResourceReportToGcdNullLib/FabricResourceReportToGcdNullLib.inf    <ALL>
  CcxSetMmioCfgBaseLib|AgesaModulePkg/Library/CcxSetMmioCfgBaseLib/CcxSetMmioCfgBaseLib.inf                            <ALL>
  IdsLib|AgesaModulePkg/Library/IdsPeiLib/IdsPeiLib.inf                                                                <ALL>
  FchPeiLibV9|AgesaModulePkg/Library/FchPeiLib/FchPeiLib.inf                                                           <ALL>
  FchTaishanPeiLib|AgesaModulePkg/Fch/Taishan/FchTaishanCore/FchTaishanLibPei.inf                                      <ZP>
  FchHuashanPeiLib|AgesaModulePkg/Fch/Huashan/FchHuashanCore/FchHuashanLibPei.inf                                      <SSP|MTS|GN|BA|VMR>
  FchSongshanPeiLib|AgesaModulePkg/Fch/Songshan/FchSongshanCore/FchSongshanLibPei.inf                                  <RS>
  FchQiantangPeiLib|AgesaModulePkg/Fch/Qiantang/FchQiantangCore/FchQiantangLibPei.inf                                  <MI3>
  FchSandstonePeiLib|AgesaModulePkg/Fch/Sandstone/FchSandstoneCore/FchSandstoneLibPei.inf                              <RV>
  FchShastaPeiLib|AgesaModulePkg/Fch/Shasta/FchShastaCore/FchShastaLibPei.inf                                          <RN|CZN>
  FchShangPeiLib|AgesaModulePkg/Fch/Shang/FchShangCore/FchShangLibPei.inf                                              <FF3>
  FchKeithPeiLib|AgesaModulePkg/Fch/Keith/FchKeithCore/FchKeithLibPei.inf                                              <RMB>
  FchYuntaiPeiLib|AgesaModulePkg/Fch/Yuntai/FchYuntaiCore/FchYuntaiLibPei.inf                                          <RPL>
  FchTacomaPeiLib|AgesaModulePkg/Fch/Tacoma/FchTacomaCore/FchTacomaLibPei.inf                                          <PHX>
  FchKeithMdnPeiLib|AgesaModulePkg/Fch/KeithMdn/FchKeithMdnCore/FchKeithMdnLibPei.inf                                  <MDN>
  FchKernPeiLib|AgesaModulePkg/Fch/Kern/FchKernCore/FchKernLibPei.inf                                                  <FAMILY15H>
  FchInitHookLibPei|AgesaModulePkg/Library/FchInitHookLib/FchInitHookLibPei.inf                                        <ALL>
  GnbHeapLib|AgesaModulePkg/Library/GnbHeapPeiLib/GnbHeapPeiLib.inf                                                    <ALL>
  AgesaGnbPeiLib|AgesaModulePkg/Library/GnbCZLib/GnbPeiLibCZ.inf                                                       <FAMILY15H>
  AmdPspCommonLib|AgesaModulePkg/Library/AmdPspCommonLibPei/AmdPspCommonLibPei.inf                                     <ALL>
  ApobCommonServiceLib|AgesaModulePkg/Library/ApobCommonServiceLibPei/ApobCommonServiceLibPei.inf                      <ALL>
  PeiSocLogicalIdServicesLib|AgesaModulePkg/Library/PeiSocLogicalIdServicesLib/PeiSocLogicalIdServicesLib.inf          <ALL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/Pei/AmdTableHookPeiLib.inf                                        <FAMILY15H|GN|BA|VMR|CZN|RV|ZP|SSP|MTS|RN|CGL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Pei/AmdTableHookPeiLibV2.inf                                    <FF3|RMB|RS|RPL|PHX|MDN|MI3|STP>
  AmdCapsuleLib|AgesaModulePkg/Library/AmdCapsuleLibPei/AmdCapsuleLibPei.inf                                           <ALL>
  PeiSocBistLoggingLib|AgesaModulePkg/Library/PeiSocBistLoggingLib/PeiSocBistLoggingLib.inf                            <ZP|RV>
  PeiSocBistLogging2Lib|AgesaModulePkg/Library/PeiSocBistLogging2Lib/PeiSocBistLogging2Lib.inf                         <SSP|MTS|RN|FAMILY19H|FF3|MDN>
  FchBixbyPeiLib|AgesaModulePkg/Fch/Bixby/FchBixbyCore/FchBixbyLibPei.inf                                              <BXB>
  BxbInitLibV1|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbInitLibV1.inf                                            <BXB>
  AmdIOMMUDmarLib|AgesaModulePkg/Nbio/Library/IommuDmarLib/PEI/AmdIOMMUDmarLib.inf                                     <ALL>
  PcieComplexDefaultsLib|AgesaModulePkg/Nbio/Library/PcieComplexDefaultsLib/PcieComplexDefaultsLib.inf                 <SSP|MTS|VMR|GN|BA|RS|RN|CZN|RMB|RPL|ZP|RV|VN|MR|AR|PHX|MDN|MI3>
  NbioCommonPeiLib|AgesaModulePkg/Nbio/Library/CommonPei/NbioCommonPeiLib.inf                                          <BA|RS|MI3>
  Mi200PeiLib|AgesaModulePkg/Nbio/MI200/PEI/Mi200PeiLib.inf                                                            <BA>
  CcxPeiSmmAccessLib|AgesaModulePkg/Library/PeiCcxSmmAccessLib/PeiCcxSmmAccessLib.inf                                  <RMB|PHX>
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Null/AmdPspRegMuxLibV2.inf                                 <ALL>
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibPei/AmdCfgPcdBufLibPei.inf                                     <ALL>

  ## MEM Libs
  AmdMemBaseLib|AgesaModulePkg/Library/Mem/BaseLib/AmdMemBaseLib.inf                                                   <ALL>
  AmdMemArdkLib|AgesaModulePkg/Library/Mem/ArdkLib/AmdMemArdkLib.inf                                                   <FAMILY15H>
  AmdMemMainCzLib|AgesaModulePkg/Library/Mem/MainCzLib/AmdMemMainCzLib.inf                                             <FAMILY15H>
  AmdMemTechLib|AgesaModulePkg/Library/Mem/TechLib/AmdMemTechLib.inf                                                   <FAMILY15H>
  AmdMemTechDdr3Lib|AgesaModulePkg/Library/Mem/TechDdr3Lib/AmdMemTechDdr3Lib.inf                                       <FAMILY15H>
  AmdMemTechDdr4Lib|AgesaModulePkg/Library/Mem/TechDdr4Lib/AmdMemTechDdr4Lib.inf                                       <FAMILY15H>
  AmdMemPsCzLib|AgesaModulePkg/Library/Mem/PsCzLib/AmdMemPsCzLib.inf                                                   <FAMILY15H>
  AmdMemPsLib|AgesaModulePkg/Library/Mem/PsLib/AmdMemPsLib.inf                                                         <FAMILY15H>
  AmdMemPsCzAm4Lib|AgesaModulePkg/Library/Mem/PsCzAm4Lib/AmdMemPsCzAm4Lib.inf                                          <FAMILY15H>
  AmdMemPsCzFp4Lib|AgesaModulePkg/Library/Mem/PsCzFp4Lib/AmdMemPsCzFp4Lib.inf                                          <FAMILY15H>
  AmdMemFeatIdentifyDimmLib|AgesaModulePkg/Library/Mem/FeatIdentifyDimmLib/AmdMemFeatIdentifyDimmLib.inf               <FAMILY15H>
  AmdMemFeatDmiCollectLib|AgesaModulePkg/Library/Mem/FeatDmiCollectLib/AmdMemFeatDmiCollectLib.inf                     <FAMILY15H>
  AmdMemFeatMemClrLib|AgesaModulePkg/Library/Mem/FeatMemClrLib/AmdMemFeatMemClrLib.inf                                 <FAMILY15H>
  AmdMemFeatCratLib|AgesaModulePkg/Library/Mem/FeatCratLib/AmdMemFeatCratLib.inf                                       <FAMILY15H>
  AmdMemFeatTableLib|AgesaModulePkg/Library/Mem/FeatTableLib/AmdMemFeatTableLib.inf                                    <FAMILY15H>
  AmdMemFeatEccLib|AgesaModulePkg/Library/Mem/FeatEccLib/AmdMemFeatEccLib.inf                                          <FAMILY15H>
  AmdMemServicesCzLib|AgesaModulePkg/Library/Mem/ServicesCzLib/AmdMemServicesCzLib.inf                                 <FAMILY15H>
  AmdMemFeatUmaLib|AgesaModulePkg/Library/Mem/FeatUmaLib/AmdMemFeatUmaLib.inf                                          <FAMILY15H>
  AmdMemFeatOLSpareLib|AgesaModulePkg/Library/Mem/Null/FeatOLSpareLibNull/AmdMemFeatOLSpareLibNull.inf                 <FAMILY15H>
  AmdMemFeatCsIntLvLib|AgesaModulePkg/Library/Mem/FeatCsIntLvLib/AmdMemFeatCsIntLvLib.inf                              <FAMILY15H>
  AmdMemFeatNodeIntLvLib|AgesaModulePkg/Library/Mem/Null/FeatNodeIntLvLibNull/AmdMemFeatNodeIntLvLibNull.inf           <FAMILY15H>
  AmdMemFeatChIntLvLib|AgesaModulePkg/Library/Mem/FeatChIntLvLib/AmdMemFeatChIntLvLib.inf                              <FAMILY15H>
  AmdMemFeatIntLvRgnLib|AgesaModulePkg/Library/Mem/Null/FeatIntLvRgnLibNull/AmdMemFeatIntLvRgnLibNull.inf              <FAMILY15H>
  AmdMemFeatLvDdr3Lib|AgesaModulePkg/Library/Mem/FeatLvDdr3Lib/AmdMemFeatLvDdr3Lib.inf                                 <FAMILY15H>
  AmdMemFeatOnDimmThermalLib|AgesaModulePkg/Library/Mem/FeatOnDimmThermalLib/AmdMemFeatOnDimmThermalLib.inf            <FAMILY15H>
  AmdMemFeatExcludeDimmLib|AgesaModulePkg/Library/Mem/FeatExcludeDimmLib/AmdMemFeatExcludeDimmLib.inf                  <FAMILY15H>
  AmdMemFeatAmpLib|AgesaModulePkg/Library/Mem/Null/FeatAmpLibNull/AmdMemFeatAmpLibNull.inf                             <FAMILY15H>

  AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RvLib/MemSmbiosV2Lib.inf                                         <RV>
  AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2ZpLib/MemSmbiosV2Lib.inf                                         <AM4SM|SP4SM>
  AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2ZpMcmLib/MemSmbiosV2Lib.inf                                      <SP3ZP|SP4ZP|SP3TR>
  AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2SspLib/MemSmbiosV2Lib.inf                                        <SSP|MTS|GN|BA|VMR>
  AmdCalloutLib|AgesaModulePkg/Library/AmdCalloutLib/AmdCalloutLib.inf                                                 <RV|AM4RN>


  ## IDS LIB
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibPei/AmdIdsHookLib.inf                                              <ALL>
  CcxXvIdsHookLibPei|AgesaModulePkg/Library/CcxXvIdsHookLib/Pei/CcxXvIdsHookLibPei.inf                                 <FAMILY15H>
  FabricIdsHookUnbLibPei|AgesaModulePkg/Library/FabricIdsHookUnbLib/Pei/FabricIdsHookUnbLibPei.inf                     <FAMILY15H>
  MemIdsHookLib|AgesaModulePkg/Library/Mem/MemIdsHookCzLib/MemIdsHookCzLib.inf                                         <FAMILY15H>
  CcxZenZpIdsHookLibPei|AgesaModulePkg/Library/CcxZenZpIdsHookLib/Pei/CcxZenZpIdsHookLibPei.inf                        <ZP>
  CcxZenRvIdsHookLibPei|AgesaModulePkg/Library/CcxZenRvIdsHookLib/Pei/CcxZenRvIdsHookLibPei.inf                        <RV>
  CcxVhIdsHookLibPei|AgesaModulePkg/Library/CcxVhIdsHookLib/Pei/CcxVhIdsHookLibPei.inf                                 <SSP|MTS>
  CcxVhRnIdsHookLibPei|AgesaModulePkg/Library/CcxVhRnIdsHookLib/Pei/CcxVhRnIdsHookLibPei.inf                           <RN>
  CcxVhFf3IdsHookLibPei|AgesaModulePkg/Library/CcxVhFf3IdsHookLib/Pei/CcxVhFf3IdsHookLibPei.inf                        <FF3>
  CcxVhMdnIdsHookLibPei|AgesaModulePkg/Library/CcxVhMdnIdsHookLib/Pei/CcxVhMdnIdsHookLibPei.inf                        <MDN>
  CcxZen3CznIdsHookLibPei|AgesaModulePkg/Library/CcxZen3CznIdsHookLib/Pei/CcxZen3CznIdsHookLibPei.inf                  <CZN>
  CcxZen3RmbIdsHookLibPei|AgesaModulePkg/Library/CcxZen3RmbIdsHookLib/Pei/CcxZen3RmbIdsHookLibPei.inf                  <RMB>
  CcxZen3IdsHookLibPei|AgesaModulePkg/Library/CcxZen3IdsHookLib/Pei/CcxZen3IdsHookLibPei.inf                           <VMR|CGL>
  CcxZen3IdsHookLibPei|AgesaModulePkg/Library/CcxZen3GnIdsHookLib/Pei/CcxZen3GnIdsHookLibPei.inf                       <GN>
  CcxZen3IdsHookLibPei|AgesaModulePkg/Library/CcxZen3BaIdsHookLib/Pei/CcxZen3BaIdsHookLibPei.inf                       <BA>
  CcxZen4IdsHookLibPei|AgesaModulePkg/Library/CcxZen4RsIdsHookLib/Pei/CcxZen4RsIdsHookLibPei.inf                       <RS|STP>
  CcxZen4IdsHookLibPei|AgesaModulePkg/Library/CcxZen4RplIdsHookLib/Pei/CcxZen4RplIdsHookLibPei.inf                     <RPL>
  CcxZen4IdsHookLibPei|AgesaModulePkg/Library/CcxZen4PhxIdsHookLib/Pei/CcxZen4PhxIdsHookLibPei.inf                     <PHX>
  CcxZen4IdsHookLibPei|AgesaModulePkg/Library/CcxZen4Mi3IdsHookLib/Pei/CcxZen4Mi3IdsHookLibPei.inf                     <MI3>
  FabricIdsHookZpLibPei|AgesaModulePkg/Library/FabricIdsHookZpLib/Pei/FabricIdsHookZpLibPei.inf                        <ZP>
  FabricIdsHookRvLibPei|AgesaModulePkg/Library/FabricIdsHookRvLib/Pei/FabricIdsHookRvLibPei.inf                        <RV>
  FabricIdsHookSspLibPei|AgesaModulePkg/Library/FabricIdsHookSspLib/Pei/FabricIdsHookSspLibPei.inf                     <SSP|MTS|VMR|CGL>
  FabricIdsHookSspLibPei|AgesaModulePkg/Library/FabricIdsHookGnLib/Pei/FabricIdsHookGnLibPei.inf                       <GN>
  FabricIdsHookRsLibPei|AgesaModulePkg/Library/FabricIdsHookRsLib/Pei/FabricIdsHookRsLibPei.inf                        <RS|STP>
  FabricIdsHookSspLibPei|AgesaModulePkg/Library/FabricIdsHookBaLib/Pei/FabricIdsHookBaLibPei.inf                       <BA>
  FabricIdsHookRnLibPei|AgesaModulePkg/Library/FabricIdsHookRnLib/Pei/FabricIdsHookRnLibPei.inf                        <RN|CZN>
  FabricIdsHookFf3LibPei|AgesaModulePkg/Library/FabricIdsHookFf3Lib/Pei/FabricIdsHookFf3LibPei.inf                     <FF3>
  FabricIdsHookRmbLibPei|AgesaModulePkg/Library/FabricIdsHookRmbLib/Pei/FabricIdsHookRmbLibPei.inf                     <RMB>
  FabricIdsHookMdnLibPei|AgesaModulePkg/Library/FabricIdsHookMdnLib/Pei/FabricIdsHookMdnLibPei.inf                     <MDN>
  FabricIdsHookPhxLibPei|AgesaModulePkg/Library/FabricIdsHookPhxLib/Pei/FabricIdsHookPhxLibPei.inf                     <PHX>
  FabricIdsHookRplLibPei|AgesaModulePkg/Library/FabricIdsHookRplLib/Pei/FabricIdsHookRplLibPei.inf                     <RPL>
  FabricIdsHookMi3LibPei|AgesaModulePkg/Library/FabricIdsHookMi3Lib/Pei/FabricIdsHookMi3LibPei.inf                     <MI3>
  NbioIdsHookSspLibPei|AgesaModulePkg/Library/NbioIdsHookSspLib/Pei/NbioIdsHookSspLibPei.inf                           <SSP|MTS|VMR>
  NbioIdsHookGnLibPei|AgesaModulePkg/Library/NbioIdsHookGnLib/Pei/NbioIdsHookGnLibPei.inf                              <GN>
  NbioIdsHookBaLibPei|AgesaModulePkg/Library/NbioIdsHookBaLib/Pei/NbioIdsHookBaLibPei.inf                              <BA>
  NbioIdsHookRsLibPei|AgesaModulePkg/Library/NbioIdsHookRsLib/Pei/NbioIdsHookRsLibPei.inf                              <RS>
  NbioIdsHookBxbLibPei|AgesaModulePkg/Library/NbioIdsHookBxbLib/Pei/NbioIdsHookBxbLibPei.inf                           <BXB>
  NbioIdsHookRnLibPei|AgesaModulePkg/Library/NbioIdsHookRnLib/Pei/NbioIdsHookRnLibPei.inf                              <RN|CZN>
  NbioIdsHookRmbLibPei|AgesaModulePkg/Library/NbioIdsHookRmbLib/Pei/NbioIdsHookRmbLibPei.inf                           <RMB>
  NbioIdsHookPhxLibPei|AgesaModulePkg/Library/NbioIdsHookPhxLib/Pei/NbioIdsHookPhxLibPei.inf                           <PHX>
  NbioIdsHookRplLibPei|AgesaModulePkg/Library/NbioIdsHookRplLib/Pei/NbioIdsHookRplLibPei.inf                           <RPL>
  NbioIdsHookFf3LibPei|AgesaModulePkg/Library/NbioIdsHookFf3Lib/Pei/NbioIdsHookLibPei.inf                              <FF3>
  NbioIdsHookMdnLibPei|AgesaModulePkg/Library/NbioIdsHookMdnLib/Pei/NbioIdsHookMdnLibPei.inf                           <MDN>
  FchIdsHookRmbLibPei|AgesaModulePkg/Library/FchIdsHookRmbLib/Pei/FchIdsHookRmbLibPei.inf                              <RMB>
  FchIdsHookPhxLibPei|AgesaModulePkg/Library/FchIdsHookPhxLib/Pei/FchIdsHookPhxLibPei.inf                              <PHX>
  FchIdsHookRplLibPei|AgesaModulePkg/Library/FchIdsHookRplLib/Pei/FchIdsHookRplLibPei.inf                              <RPL>
  FchIdsHookMdnLibPei|AgesaModulePkg/Library/FchIdsHookMdnLib/Pei/FchIdsHookMdnLibPei.inf                              <MDN>

  ## FCH I2C Lib
  Mp2I2cLibPei|AgesaModulePkg/Fch/Sandstone/FchSandstoneI2cPei/Mp2I2cLibPei.inf                                        <FP5RV|FT5RV>

  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessSmnLib.inf                                        <CGL|PHX|MDN>

[LibraryClasses.Common.DXE_DRIVER]
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/S3Save/AmdS3SaveLib.inf                                             <ALL>
  IdsLib|AgesaModulePkg/Library/IdsDxeLib/IdsDxeLib.inf                                                                <ALL>
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/DxeFabricResourceSizeForEachRbLib/DxeFabricResourceSizeForEachRbLib.inf   <ALL>
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf                                                    <ALL>
  GnbHeapLib|AgesaModulePkg/Library/GnbHeapDxeLib/GnbHeapDxeLib.inf                                                    <ALL>
  AgesaGnbDxeLib|AgesaModulePkg/Library/GnbCZLib/GnbDxeLibCZ.inf                                                       <FAMILY15H>
  AmdPspCommonLib|AgesaModulePkg/Library/AmdPspCommonLibDxe/AmdPspCommonLibDxe.inf                                     <ALL>
  ApobCommonServiceLib|AgesaModulePkg/Library/ApobCommonServiceLibDxe/ApobCommonServiceLibDxe.inf                      <ALL>
  AmdPspFlashAccLib|AgesaModulePkg/Library/AmdPspFlashAccLibDxe/AmdPspFlashAccLibDxe.inf                               <ALL>
  AmdPspFlashUpdateLib|AgesaModulePkg/Library/AmdPspFlashUpdateLib/AmdPspFlashUpdateLib.inf                            <ALL>
  AmdPspRomArmorLib|AgesaModulePkg/Library/AmdPspRomArmorLibNull/AmdPspRomArmorLibNull.inf                             <ZP|SSP|GN|BA|RS|MI3>
  AmdTpm2CommandLib|AgesaModulePkg/Library/AmdTpm2CommandLib/AmdTpm2CommandLib.inf                                     <RMB>
  ApcbZpLib|AgesaModulePkg/Library/ApcbZpLib/ApcbLib.inf                                                               <ZP>
  ApcbRvLib|AgesaModulePkg/Library/ApcbRvLib/ApcbLib.inf                                                               <RV>
#  ApcbSspLib|AgesaModulePkg/Library/ApcbSspLib/ApcbLib.inf                                                             <SSP|MTS>
  DxeSocLogicalIdServicesLib|AgesaModulePkg/Library/DxeSocLogicalIdServicesLib/DxeSocLogicalIdServicesLib.inf          <ALL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/Dxe/AmdTableHookDxeLib.inf                                        <FAMILY15H|GN|BA|VMR|CZN|RS|RV|ZP|SSP|MTS|RN|CGL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Dxe/AmdTableHookDxeLibV2.inf                                    <FF3|RMB|RS|RPL|PHX|MI3|STP>
  AmdCapsuleLib|AgesaModulePkg/Library/AmdCapsuleLibDxe/AmdCapsuleLibDxe.inf                                           <ALL>
  CcxMpServicesLib|AgesaModulePkg/Library/CcxMpServicesDxeLib/CcxMpServicesDxeLib.inf                                  <ALL>
  CcxSmmAccess2Lib|AgesaModulePkg/Library/DxeCcxSmmAccess2Lib/DxeCcxSmmAccess2Lib.inf                                  <ALL>
  AmdIOMMUDmarLib|AgesaModulePkg/Nbio/Library/IommuDmarLib/DXE/AmdIOMMUDmarLib.inf                                     <ALL>
  CcxCppcLib|AgesaModulePkg/Library/DxeCcxCppcLib/DxeCcxCppcLib.inf                                                    <FAMILY17H|FAMILY19H>
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Dxe/AmdPspRegMuxLibV2.inf                                  <ALL>
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibDxe/AmdCfgPcdBufLibDxe.inf                                     <ALL>

  ## MEM Lib
  AmdMemS3InitLib|AgesaModulePkg/Library/Mem/S3InitLib/S3InitLib.inf                                                   <FAMILY15H>
  AmdMemArdkLib|AgesaModulePkg/Library/Mem/Null/ArdkLibNull/AmdMemArdkLibNull.inf                                      <FAMILY15H>
  AmdMemMainCzLib|AgesaModulePkg/Library/Mem/Null/MainCzLibNull/AmdMemMainCzLibNull.inf                                <FAMILY15H>
  AmdMemPsLib|AgesaModulePkg/Library/Mem/Null/PsLibNull/AmdMemPsLibNull.inf                                            <FAMILY15H>
  AmdMemPsCzLib|AgesaModulePkg/Library/Mem/Null/PsCzLibNull/AmdMemPsCzLibNull.inf                                      <FAMILY15H>
  AmdMemPsCzAm4Lib|AgesaModulePkg/Library/Mem/Null/PsCzAm4LibNull/AmdMemPsCzAm4LibNull.inf                             <FAMILY15H>
  AmdMemPsCzFp4Lib|AgesaModulePkg/Library/Mem/Null/PsCzFp4LibNull/AmdMemPsCzFp4LibNull.inf                             <FAMILY15H>
  AmdMemFeatDmiCollectLib|AgesaModulePkg/Library/Mem/Null/FeatDmiCollectLibNull/AmdMemFeatDmiCollectLibNull.inf        <FAMILY15H>
  AmdMemFeatCratLib|AgesaModulePkg/Library/Mem/Null/FeatCratLibNull/AmdMemFeatCratLibNull.inf                          <FAMILY15H>
  AmdMemTechLib|AgesaModulePkg/Library/Mem/Null/TechLibNull/AmdMemTechLibNull.inf                                      <FAMILY15H>
  AmdMemTechDdr3Lib|AgesaModulePkg/Library/Mem/Null/TechDdr3LibNull/AmdMemTechDdr3LibNull.inf                          <FAMILY15H>
  AmdMemTechDdr4Lib|AgesaModulePkg/Library/Mem/Null/TechDdr4LibNull/AmdMemTechDdr4LibNull.inf                          <FAMILY15H>
  AmdMemFeatTableLib|AgesaModulePkg/Library/Mem/Null/FeatTableLibNull/AmdMemFeatTableLibNull.inf                       <FAMILY15H>
  AmdMemFeatIdentifyDimmLib|AgesaModulePkg/Library/Mem/Null/FeatIdentifyDimmLibNull/AmdMemFeatIdentifyDimmLibNull.inf  <FAMILY15H>
  AmdMemFeatMemClrLib|AgesaModulePkg/Library/Mem/Null/FeatMemClrLibNull/AmdMemFeatMemClrLibNull.inf                    <FAMILY15H>
  AmdMemFeatDmiConstructLib|AgesaModulePkg/Library/Mem/FeatDmiConstructLib/MemFeatDmiConstructLib.inf                  <FAMILY15H>
  ApcbLibV3|AgesaModulePkg/Library/ApcbLibV3/ApcbLibV3.inf                                                             <SSP|MTS|RN|GN|BA|VMR|CZN|RMB|FF3|RS|RPL|PHX|MDN|MI3>
  ApcbVariableLibV3|AgesaModulePkg/Library/ApcbVariableLibV3/ApcbVariableLibV3.inf                                     <SSP|MTS|RN|GN|BA|VMR|CZN|RMB|FF3|RS|RPL|PHX|MDN|MI3>
  ApcbChecksumLibV3|AgesaModulePkg/Library/ApcbChecksumLibV3/ApcbChecksumLibV3.inf                                     <SSP|MTS|RN|GN|BA|VMR|CZN|FF3|MI3>
  ApcbChecksumLibV3|AgesaModulePkg/Library/ApcbHmacChecksumLibV3/ApcbHmacChecksumLibV3.inf                             <RMB|PHX|MDN|RPL|RS>
  AmdPspDxeSmmBufLib|AgesaModulePkg/Library/AmdPspDxeSmmBufLib/AmdPspDxeSmmBufLib.inf                                  <ALL>

  ## IDS LIB
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibDxe/AmdIdsHookLib.inf                                              <ALL>
  CcxXvIdsHookLibDxe|AgesaModulePkg/Library/CcxXvIdsHookLib/Dxe/CcxXvIdsHookLibDxe.inf                                 <FAMILY15H>
  FabricIdsHookUnbLibDxe|AgesaModulePkg/Library/FabricIdsHookUnbLib/Dxe/FabricIdsHookUnbLibDxe.inf                     <FAMILY15H>
  CcxZenZpIdsHookLibDxe|AgesaModulePkg/Library/CcxZenZpIdsHookLib/Dxe/CcxZenZpIdsHookLibDxe.inf                        <ZP>
  FabricIdsHookZpLibDxe|AgesaModulePkg/Library/FabricIdsHookZpLib/Dxe/FabricIdsHookZpLibDxe.inf                        <ZP>
  CcxZenRvIdsHookLibDxe|AgesaModulePkg/Library/CcxZenRvIdsHookLib/Dxe/CcxZenRvIdsHookLibDxe.inf                        <RV>
  FabricIdsHookRvLibDxe|AgesaModulePkg/Library/FabricIdsHookRvLib/Dxe/FabricIdsHookRvLibDxe.inf                        <RV>
  CcxVhIdsHookLibDxe|AgesaModulePkg/Library/CcxVhIdsHookLib/Dxe/CcxVhIdsHookLibDxe.inf                                 <SSP|MTS>
  CcxVhRnIdsHookLibDxe|AgesaModulePkg/Library/CcxVhRnIdsHookLib/Dxe/CcxVhRnIdsHookLibDxe.inf                           <RN>
  CcxVhFf3IdsHookLibDxe|AgesaModulePkg/Library/CcxVhFf3IdsHookLib/Dxe/CcxVhFf3IdsHookLibDxe.inf                        <FF3>
  CcxVhMdnIdsHookLibDxe|AgesaModulePkg/Library/CcxVhMdnIdsHookLib/Dxe/CcxVhMdnIdsHookLibDxe.inf                        <MDN>
  CcxZen3CznIdsHookLibDxe|AgesaModulePkg/Library/CcxZen3CznIdsHookLib/Dxe/CcxZen3CznIdsHookLibDxe.inf                  <CZN>
  CcxZen3RmbIdsHookLibDxe|AgesaModulePkg/Library/CcxZen3RmbIdsHookLib/Dxe/CcxZen3RmbIdsHookLibDxe.inf                  <RMB>
  CcxZen3IdsHookLibDxe|AgesaModulePkg/Library/CcxZen3IdsHookLib/Dxe/CcxZen3IdsHookLibDxe.inf                           <VMR|CGL>
  CcxZen3IdsHookLibDxe|AgesaModulePkg/Library/CcxZen3GnIdsHookLib/Dxe/CcxZen3GnIdsHookLibDxe.inf                       <GN>
  CcxZen3IdsHookLibDxe|AgesaModulePkg/Library/CcxZen3BaIdsHookLib/Dxe/CcxZen3BaIdsHookLibDxe.inf                       <BA>
  CcxZen4IdsHookLibDxe|AgesaModulePkg/Library/CcxZen4RsIdsHookLib/Dxe/CcxZen4RsIdsHookLibDxe.inf                       <RS|STP>
  CcxZen4IdsHookLibDxe|AgesaModulePkg/Library/CcxZen4RplIdsHookLib/Dxe/CcxZen4RplIdsHookLibDxe.inf                     <RPL>
  CcxZen4IdsHookLibDxe|AgesaModulePkg/Library/CcxZen4PhxIdsHookLib/Dxe/CcxZen4PhxIdsHookLibDxe.inf                     <PHX>
  CcxZen4IdsHookLibDxe|AgesaModulePkg/Library/CcxZen4Mi3IdsHookLib/Dxe/CcxZen4Mi3IdsHookLibDxe.inf                     <MI3>
  FabricIdsHookSspLibDxe|AgesaModulePkg/Library/FabricIdsHookSspLib/Dxe/FabricIdsHookSspLibDxe.inf                     <SSP|MTS|BA|VMR|CGL>
  FabricIdsHookSspLibDxe|AgesaModulePkg/Library/FabricIdsHookGnLib/Dxe/FabricIdsHookGnLibDxe.inf                       <GN>
  FabricIdsHookSspLibDxe|AgesaModulePkg/Library/FabricIdsHookBaLib/Dxe/FabricIdsHookBaLibDxe.inf                       <BA>
  FabricIdsHookRsLibDxe|AgesaModulePkg/Library/FabricIdsHookRsLib/Dxe/FabricIdsHookRsLibDxe.inf                        <RS|STP>
  FabricIdsHookRnLibDxe|AgesaModulePkg/Library/FabricIdsHookRnLib/Dxe/FabricIdsHookRnLibDxe.inf                        <RN|CZN>
  FabricIdsHookFf3LibDxe|AgesaModulePkg/Library/FabricIdsHookFf3Lib/Dxe/FabricIdsHookFf3LibDxe.inf                     <FF3>
  FabricIdsHookRmbLibDxe|AgesaModulePkg/Library/FabricIdsHookRmbLib/Dxe/FabricIdsHookRmbLibDxe.inf                     <RMB>
  FabricIdsHookMdnLibDxe|AgesaModulePkg/Library/FabricIdsHookMdnLib/Dxe/FabricIdsHookMdnLibDxe.inf                     <MDN>
  FabricIdsHookPhxLibDxe|AgesaModulePkg/Library/FabricIdsHookPhxLib/Dxe/FabricIdsHookPhxLibDxe.inf                     <PHX>
  FabricIdsHookRplLibDxe|AgesaModulePkg/Library/FabricIdsHookRplLib/Dxe/FabricIdsHookRplLibDxe.inf                     <RPL>
  FabricIdsHookMi3LibDxe|AgesaModulePkg/Library/FabricIdsHookMi3Lib/Dxe/FabricIdsHookMi3LibDxe.inf                     <MI3>
  NbioIdsHookSspLibDxe|AgesaModulePkg/Library/NbioIdsHookSspLib/Dxe/NbioIdsHookSspLibDxe.inf                           <SSP|MTS|VMR>
  NbioIdsHookGnLibDxe|AgesaModulePkg/Library/NbioIdsHookGnLib/Dxe/NbioIdsHookGnLibDxe.inf                              <GN>
  NbioIdsHookBaLibDxe|AgesaModulePkg/Library/NbioIdsHookBaLib/Dxe/NbioIdsHookBaLibDxe.inf                              <BA>
  NbioIdsHookRsLibDxe|AgesaModulePkg/Library/NbioIdsHookRsLib/Dxe/NbioIdsHookRsLibDxe.inf                              <RS>
  NbioIdsHookBxbLibDxe|AgesaModulePkg/Library/NbioIdsHookBxbLib/Dxe/NbioIdsHookBxbLibDxe.inf                           <BXB>
  NbioIdsHookRnLibDxe|AgesaModulePkg/Library/NbioIdsHookRnLib/Dxe/NbioIdsHookRnLibDxe.inf                              <RN|CZN>
  NbioIdsHookRmbLibDxe|AgesaModulePkg/Library/NbioIdsHookRmbLib/Dxe/NbioIdsHookRmbLibDxe.inf                           <RMB>
  NbioIdsHookPhxLibDxe|AgesaModulePkg/Library/NbioIdsHookPhxLib/Dxe/NbioIdsHookPhxLibDxe.inf                           <PHX>
  NbioIdsHookRplLibDxe|AgesaModulePkg/Library/NbioIdsHookRplLib/Dxe/NbioIdsHookRplLibDxe.inf                           <RPL>
  NbioIdsHookFf3LibDxe|AgesaModulePkg/Library/NbioIdsHookFf3Lib/Dxe/NbioIdsHookLibDxe.inf                              <FF3>
  NbioIdsHookMdnLibDxe|AgesaModulePkg/Library/NbioIdsHookMdnLib/Dxe/NbioIdsHookMdnLibDxe.inf                           <MDN>
  FchIdsHookRmbLibDxe|AgesaModulePkg/Library/FchIdsHookRmbLib/Dxe/FchIdsHookRmbLibDxe.inf                              <RMB>
  FchIdsHookPhxLibDxe|AgesaModulePkg/Library/FchIdsHookPhxLib/Dxe/FchIdsHookPhxLibDxe.inf                              <PHX>
  FchIdsHookRplLibDxe|AgesaModulePkg/Library/FchIdsHookRplLib/Dxe/FchIdsHookRplLibDxe.inf                              <RPL>
  FchIdsHookMdnLibDxe|AgesaModulePkg/Library/FchIdsHookMdnLib/Dxe/FchIdsHookMdnLibDxe.inf                              <MDN>

  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf                                       <CGL|PHX|MDN>
#  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3V2Lib.inf                                    <ALL>

[LibraryClasses.Common.DXE_SMM_DRIVER]
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf                                                    <ALL>
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/S3Save/AmdS3SaveLib.inf                                             <ALL>
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/DxeFabricResourceSizeForEachRbLib/DxeFabricResourceSizeForEachRbLib.inf   <ALL>
#@todo add specifiic SMM Lib instance, DXE Lib is depend on gBS service exisitance
  IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf                                                        <ALL>
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibDxe/AmdIdsHookLib.inf                                              <ALL>
  AmdPspCommonLib|AgesaModulePkg/Library/AmdPspCommonLibDxe/AmdPspCommonLibDxe.inf                                     <ALL>
!if $(SMM_ISOLATION_LEVEL30_SUPPORT) == TRUE
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLibSmmIso/AmdPspMmioLib.inf
!else
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
!endif                                                                                                                 <ALL>
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Dxe/AmdPspRegMuxLibV2.inf                                  <ALL>
  ApobCommonServiceLib|AgesaModulePkg/Library/ApobCommonServiceLibDxe/ApobCommonServiceLibDxe.inf                      <ALL>
  AmdPspFlashUpdateLib|AgesaModulePkg/Library/AmdPspFlashUpdateLib/AmdPspFlashUpdateLib.inf                            <ALL>
  AmdPspRomArmorLib|AgesaModulePkg/Library/AmdPspRomArmorLib/AmdPspRomArmorLib.inf                                     <ZP|SSP|GN|BA|RS|MI3>
  CcxZenZpIdsHookLibSmm|AgesaModulePkg/Library/CcxZenZpIdsHookLib/Smm/CcxZenZpIdsHookLibSmm.inf                        <ZP>
  CcxZenRvIdsHookLibSmm|AgesaModulePkg/Library/CcxZenRvIdsHookLib/Smm/CcxZenRvIdsHookLibSmm.inf                        <RV>
  CcxVhIdsHookLibSmm|AgesaModulePkg/Library/CcxVhIdsHookLib/Smm/CcxVhIdsHookLibSmm.inf                                 <SSP|MTS>
  CcxVhRnIdsHookLibSmm|AgesaModulePkg/Library/CcxVhRnIdsHookLib/Smm/CcxVhRnIdsHookLibSmm.inf                           <RN>
  CcxVhRnIdsHookLibSmm|AgesaModulePkg/Library/CcxVhRnIdsHookLib/Smm/CcxVhRnIdsHookLibSmm.inf                           <RN>
  CcxVhFf3IdsHookLibSmm|AgesaModulePkg/Library/CcxVhFf3IdsHookLib/Smm/CcxVhFf3IdsHookLibSmm.inf                        <FF3>
  CcxVhMdnIdsHookLibSmm|AgesaModulePkg/Library/CcxVhMdnIdsHookLib/Smm/CcxVhMdnIdsHookLibSmm.inf                        <MDN>
  CcxZen3IdsHookLibSmm|AgesaModulePkg/Library/CcxZen3CznIdsHookLib/Smm/CcxZen3CznIdsHookLibSmm.inf                     <CZN>
  CcxZen3IdsHookLibSmm|AgesaModulePkg/Library/CcxZen3RmbIdsHookLib/Smm/CcxZen3RmbIdsHookLibSmm.inf                     <RMB>
  CcxZen3IdsHookLibSmm|AgesaModulePkg/Library/CcxZen3IdsHookLib/Smm/CcxZen3IdsHookLibSmm.inf                           <VMR|CGL>
  CcxZen3IdsHookLibSmm|AgesaModulePkg/Library/CcxZen3GnIdsHookLib/Smm/CcxZen3GnIdsHookLibSmm.inf                       <GN>
  CcxZen3IdsHookLibSmm|AgesaModulePkg/Library/CcxZen3BaIdsHookLib/Smm/CcxZen3BaIdsHookLibSmm.inf                       <BA>
  CcxZen4IdsHookLibSmm|AgesaModulePkg/Library/CcxZen4RsIdsHookLib/Smm/CcxZen4RsIdsHookLibSmm.inf                       <RS|STP>
  CcxZen4IdsHookLibSmm|AgesaModulePkg/Library/CcxZen4RplIdsHookLib/Smm/CcxZen4RplIdsHookLibSmm.inf                     <RPL>
  CcxZen4IdsHookLibSmm|AgesaModulePkg/Library/CcxZen4PhxIdsHookLib/Smm/CcxZen4PhxIdsHookLibSmm.inf                     <PHX>
  CcxZen4IdsHookLibSmm|AgesaModulePkg/Library/CcxZen4Mi3IdsHookLib/Smm/CcxZen4Mi3IdsHookLibSmm.inf                     <MI3>
  ApcbZpLib|AgesaModulePkg/Library/ApcbZpLib/ApcbLib.inf                                                               <ZP>
  ApcbRvLib|AgesaModulePkg/Library/ApcbRvLib/ApcbLib.inf                                                               <RV>
#  ApcbSspLib|AgesaModulePkg/Library/ApcbSspLib/ApcbLib.inf                                                             <SSP|MTS>
  AmdCapsuleLib|AgesaModulePkg/Library/AmdCapsuleLibDxe/AmdCapsuleLibDxe.inf                                           <ALL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/Dxe/AmdTableHookDxeLib.inf                                        <FAMILY15H|GN|BA|VMR|CZN|RMB|RS|RV|ZP|SSP|MTS|RN|CGL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Dxe/AmdTableHookDxeLibV2.inf                                    <FF3|RMB|RS|RPL|PHX|MI3|STP>
  CcxMpServicesLib|AgesaModulePkg/Library/CcxMpServicesSmmLib/CcxMpServicesSmmLib.inf                                  <ALL>
  RasZpSmmLib|AgesaModulePkg/Library/RasZpSmmLib/RasZpSmmLib.inf                                                       <ZP>
  RasRvSmmLib|AgesaModulePkg/Library/RasRvSmmLib/RasRvSmmLib.inf                                                       <RV>
  RasSspSmmLib|AgesaModulePkg/Library/RasSspSmmLib/RasSspSmmLib.inf                                                    <SSP|MTS|VMR>
  ApcbLibV3|AgesaModulePkg/Library/ApcbLibV3/ApcbLibV3.inf                                                             <SSP|MTS|RN|GN|BA|VMR|CZN|RMB|FF3|RS|RPL|PHX|MDN|MI3>
  ApcbVariableLibV3|AgesaModulePkg/Library/ApcbVariableLibV3/ApcbVariableLibV3.inf                                     <SSP|MTS|RN|GN|BA|VMR|CZN|RMB|FF3|RS|RPL|PHX|MDN|MI3>
  ApcbChecksumLibV3|AgesaModulePkg/Library/ApcbChecksumLibV3/ApcbChecksumLibV3.inf                                     <SSP|MTS|RN|GN|BA|VMR|CZN|FF3|MI3>
  ApcbChecksumLibV3|AgesaModulePkg/Library/ApcbHmacChecksumLibV3/ApcbHmacChecksumLibV3.inf                             <RMB|PHX|MDN|RPL|RS>
  RasGnSmmLib|AgesaModulePkg/Library/RasGnSmmLib/RasGnSmmLib.inf                                                       <GN>
  RasBaSmmLib|AgesaModulePkg/Library/RasBaSmmLib/RasBaSmmLib.inf                                                       <BA>
  RasSmmLib|AgesaModulePkg/Library/Ras/RS/RasRsSmmLib/RasRsSmmLib.inf                                                  <RS>
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibDxe/AmdCfgPcdBufLibDxe.inf                                     <ALL>
  AmdPspDxeSmmBufLib|AgesaModulePkg/Library/AmdPspDxeSmmBufLib/AmdPspDxeSmmBufLib.inf                                  <ALL>

  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf                                       <CGL|PHX|MDN>
#  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3V2Lib.inf                                    <ALL>

[LibraryClasses.Common.COMBINED_SMM_DXE]
  ## FCH SPI ACCESS lib
#  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf                                      <CGL|PHX|MDN>
#  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3V2Lib.inf                                    <ALL>

[LibraryClasses.COMMON.DXE_RUNTIME_DRIVER]
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapDxeLib/AmdHeapDxeLib.inf                                                    <ALL>
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2DxeRt/AmdPspRegMuxLibV2.inf                                <ALL>
  AmdStbLib|AgesaModulePkg/Library/AmdStbLibNull/AmdStbLibNull.inf                                                     <ALL>
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibDxe/AmdCfgPcdBufLibDxe.inf                                     <ALL>
  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom3Lib.inf                                       <CGL|PHX|MDN>

[LibraryClasses.common.PEI_CORE]
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapPeiLib/AmdHeapPeiLib.inf                                                    <ALL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/Pei/AmdTableHookPeiLib.inf                                        <FAMILY15H|GN|BA|VMR|CZN|RV|ZP|SSP|MTS|RN|CGL>
  AmdTableHookLib|AgesaModulePkg/Library/AmdTableLibV2/Pei/AmdTableHookPeiLibV2.inf                                    <FF3|RMB|RS|RPL|PHX|MI3|STP>
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibPei/AmdCfgPcdBufLibPei.inf                                     <ALL>

  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessSmnLib.inf                                        <CGL|PHX|MDN>

[LibraryClasses]
  #
  # Agesa specific common libraries
  #
  AmdBaseLib|AgesaModulePkg/Library/AmdBaseLib/AmdBaseLib.inf                                                          <ALL>
  FabricResourceSizeForEachRbLib|AgesaModulePkg/Library/DxeFabricResourceSizeForEachRbLib/DxeFabricResourceSizeForEachRbLib.inf   <ALL>
  FabricResourceReportToGcdLib|AgesaModulePkg/Library/FabricResourceReportToGcdLib/FabricResourceReportToGcdLib.inf    <ALL>
  AmdIdsDebugPrintLib|AgesaModulePkg/Library/AmdIdsDebugPrintLib/AmdIdsDebugPrintLib.inf                               <ALL>
  AmdIdsHookLib|AgesaModulePkg/Library/AmdIdsHookLibNull/AmdIdsHookLibNull.inf                                         <ALL>
  AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf                                    <ALL>
  IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf                                                        <ALL>
  IdsMiscLib|AgesaModulePkg/Library/IdsMiscLib/IdsMiscLib.inf                                                          <ALL>
  AmdHeapLib|AgesaModulePkg/Library/AmdHeapLibNull/AmdHeapLibNull.inf                                                  <ALL>
  AmdCfgPcdBufLib|AgesaModulePkg/Library/AmdCfgPcdBufLibNull/AmdCfgPcdBufLibNull.inf                                   <ALL>

  AmdSocBaseLib|AgesaModulePkg/Library/AmdSocBaseLib/AmdSocBaseLib.inf                                                 <ALL>
  AmdErrorLogLib|AgesaModulePkg/Library/AmdErrorLogLib/AmdErrorLogLib.inf                                              <ALL>
  AmdTableLib|AgesaModulePkg/Library/AmdTableLib/AmdTableLib.inf                                                       <FAMILY15H|GN|BA|VMR|CZN|RV|ZP|SSP|MTS|RN|CGL>
  AmdTableLib|AgesaModulePkg/Library/AmdTableLibV2/AmdTableLibV2.inf                                                   <FF3|RMB|RS|RPL|PHX|MDN|MI3|STP>
  SocCmnIdsHookZpLibPei|AgesaModulePkg/Library/SocCmnIdsHookZpLib/Pei/SocCmnIdsHookZpLibPei.inf                        <ZP|FAMILY15H>
  SocCmnIdsHookZpLibDxe|AgesaModulePkg/Library/SocCmnIdsHookZpLib/Dxe/SocCmnIdsHookZpLibDxe.inf                        <ZP|FAMILY15H>
  SocCmnIdsHookRvLibPei|AgesaModulePkg/Library/SocCmnIdsHookRvLib/Pei/SocCmnIdsHookRvLibPei.inf                        <RV>
  SocCmnIdsHookRvLibDxe|AgesaModulePkg/Library/SocCmnIdsHookRvLib/Dxe/SocCmnIdsHookRvLibDxe.inf                        <RV>
  SocCmnIdsHookSspLibPei|AgesaModulePkg/Library/SocCmnIdsHookSspLib/Pei/SocCmnIdsHookSspLibPei.inf                     <SSP|MTS|VMR|CGL>
  SocCmnIdsHookSspLibDxe|AgesaModulePkg/Library/SocCmnIdsHookSspLib/Dxe/SocCmnIdsHookSspLibDxe.inf                     <SSP|MTS|VMR|CGL>
  SocCmnIdsHookRnLibPei|AgesaModulePkg/Library/SocCmnIdsHookRnLib/Pei/SocCmnIdsHookRnLibPei.inf                        <RN>
  SocCmnIdsHookRnLibDxe|AgesaModulePkg/Library/SocCmnIdsHookRnLib/Dxe/SocCmnIdsHookRnLibDxe.inf                        <RN>
  SocCmnIdsHookFf3LibPei|AgesaModulePkg/Library/SocCmnIdsHookFf3Lib/Pei/SocCmnIdsHookFf3LibPei.inf                     <FF3>
  SocCmnIdsHookFf3LibDxe|AgesaModulePkg/Library/SocCmnIdsHookFf3Lib/Dxe/SocCmnIdsHookFf3LibDxe.inf                     <FF3>
  SocCmnIdsHookCznLibPei|AgesaModulePkg/Library/SocCmnIdsHookCznLib/Pei/SocCmnIdsHookCznLibPei.inf                     <CZN>
  SocCmnIdsHookCznLibDxe|AgesaModulePkg/Library/SocCmnIdsHookCznLib/Dxe/SocCmnIdsHookCznLibDxe.inf                     <CZN>
  SocCmnIdsHookRmbLibPei|AgesaModulePkg/Library/SocCmnIdsHookRmbLib/Pei/SocCmnIdsHookRmbLibPei.inf                     <RMB>
  SocCmnIdsHookRmbLibDxe|AgesaModulePkg/Library/SocCmnIdsHookRmbLib/Dxe/SocCmnIdsHookRmbLibDxe.inf                     <RMB>
  SocCmnIdsHookRplLibPei|AgesaModulePkg/Library/SocCmnIdsHookRplLib/Pei/SocCmnIdsHookRplLibPei.inf                     <RPL>
  SocCmnIdsHookRplLibDxe|AgesaModulePkg/Library/SocCmnIdsHookRplLib/Dxe/SocCmnIdsHookRplLibDxe.inf                     <RPL>
  SocCmnIdsHookGnLibPei|AgesaModulePkg/Library/SocCmnIdsHookGnLib/Pei/SocCmnIdsHookGnLibPei.inf                        <GN>
  SocCmnIdsHookGnLibDxe|AgesaModulePkg/Library/SocCmnIdsHookGnLib/Dxe/SocCmnIdsHookGnLibDxe.inf                        <GN>
  SocCmnIdsHookBaLibPei|AgesaModulePkg/Library/SocCmnIdsHookBaLib/Pei/SocCmnIdsHookBaLibPei.inf                        <BA>
  SocCmnIdsHookBaLibDxe|AgesaModulePkg/Library/SocCmnIdsHookBaLib/Dxe/SocCmnIdsHookBaLibDxe.inf                        <BA>
  SocCmnIdsHookRsLibPei|AgesaModulePkg/Library/SocCmnIdsHookRsLib/Pei/SocCmnIdsHookRsLibPei.inf                        <RS|STP>
  SocCmnIdsHookRsLibDxe|AgesaModulePkg/Library/SocCmnIdsHookRsLib/Dxe/SocCmnIdsHookRsLibDxe.inf                        <RS|STP>
  SocCmnIdsHookPhxLibPei|AgesaModulePkg/Library/SocCmnIdsHookPhxLib/Pei/SocCmnIdsHookPhxLibPei.inf                     <PHX>
  SocCmnIdsHookPhxLibDxe|AgesaModulePkg/Library/SocCmnIdsHookPhxLib/Dxe/SocCmnIdsHookPhxLibDxe.inf                     <PHX>
  SocCmnIdsHookMdnLibPei|AgesaModulePkg/Library/SocCmnIdsHookMdnLib/Pei/SocCmnIdsHookMdnLibPei.inf                     <MDN>
  SocCmnIdsHookMdnLibDxe|AgesaModulePkg/Library/SocCmnIdsHookMdnLib/Dxe/SocCmnIdsHookMdnLibDxe.inf                     <MDN>
  BaseCoreLogicalIdLib|AgesaModulePkg/Library/BaseCoreLogicalIdX86Lib/BaseCoreLogicalIdX86Lib.inf                      <ALL>
  AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf                                         <ALL>
  AmdStbLib|AgesaModulePkg/Library/AmdStbLib/AmdStbLib.inf                                                             <ALL>
  AmdEmulationFlagLib|AgesaModulePkg/Library/AmdEmulationFlagLib/AmdEmulationFlagLib.inf                               <ALL>
  PresiliconControlLib|AgesaModulePkg/Library/PresiliconControlRmbLib/PresiliconControlRmbLib.inf                      <RMB>
  PresiliconControlLib|AgesaModulePkg/Library/PresiliconControlPhxLib/PresiliconControlPhxLib.inf                      <PHX>
  PresiliconControlLib|AgesaModulePkg/Library/PresiliconControlRplLib/PresiliconControlRplLib.inf                      <RPL>
  PresiliconControlLib|AgesaModulePkg/Library/PresiliconControlMdnLib/PresiliconControlMdnLib.inf                      <MDN>
  PresiliconControlLib|AgesaModulePkg/Library/PresiliconControlRsLib/PresiliconControlRsLib.inf                        <RS|STP>
  AgesaConfigLib|AgesaModulePkg/Library/AgesaConfigLib/AgesaConfigLib.inf                                              <ALL>

  ## PSP Libs
  AmdPspMboxLibV1|AgesaModulePkg/Library/AmdPspMboxLibV1/AmdPspMboxLibV1.inf                                           <ALL>
  AmdPspBaseLibV1|AgesaModulePkg/Library/AmdPspBaseLibV1/AmdPspBaseLibV1.inf                                           <ALL>
  AmdPspBaseLibV2|AgesaModulePkg/Library/AmdPspBaseLibV2/AmdPspBaseLibV2.inf                                           <ALL>
  AmdPspMboxLibV2|AgesaModulePkg/Library/AmdPspMboxLibV2/AmdPspMboxLibV2.inf                                           <ALL>
  AmdPspRegMuxLibV2|AgesaModulePkg/Library/AmdPspRegMuxLibV2Null/AmdPspRegMuxLibV2.inf                                 <ALL>
  AmdPspApobLib|AgesaModulePkg/Library/AmdPspApobLib/AmdPspApobLib.inf                                                 <ALL>
  AmdPspFtpmLib|AgesaModulePkg/Library/AmdPspFtpmLib/AmdPspFtpmLib.inf                                                 <AM4|RV|SP3TR|SP3CP|SP4SM|RN|VMR|CZN|VN|RMB|SP3CGL|RPL|PHX|MDN>
  AmdHspFtpmLib|AgesaModulePkg/Library/AmdHspFtpmLib/AmdHspFtpmLib.inf                                                 <VN|RMB|RPL|PHX|MDN>
  HspLib|AgesaModulePkg/Library/HspLib/HspLib.inf                                                                      <VN|RMB|RPL|PHX|MDN>
  AmdPspFlashAccLib|AgesaModulePkg/Library/AmdPspFlashAccLibNull/AmdPspFlashAccLibNull.inf                             <ALL>
  PspIdsHookLibDxe|AgesaModulePkg/Library/PspIdsHookLib/Dxe/PspIdsHookLibDxe.inf                                       <ALL>
  ApobZpRvLib|AgesaModulePkg/Library/ApobZpRvLib/ApobZpRvLib.inf                                                       <AM4BR|RV|ZP|MTS|VMR>
  ApobZpMcmLib|AgesaModulePkg/Library/ApobZpMcmLib/ApobZpMcmLib.inf                                                    <SP3ZP|SP4ZP|SP3TR>
  ApobSspLib|AgesaModulePkg/Library/ApobSspLib/ApobSspLib.inf                                                          <SSP|MTS|GN|BA|VMR>
  ApobRnLib|AgesaModulePkg/Library/ApobRnLib/ApobRnLib.inf                                                             <RN|CZN>
  ApobRmbLib|AgesaModulePkg/Library/ApobRmbLib/ApobRmbLib.inf                                                          <RMB>
  ApobFf3Lib|AgesaModulePkg/Library/ApobFf3Lib/ApobFf3Lib.inf                                                          <VN|MR|AR>
  ApobRsLib|AgesaModulePkg/Library/ApobRsLib/ApobRsLib.inf                                                             <RS>
  ApobRplLib|AgesaModulePkg/Library/ApobRplLib/ApobRplLib.inf                                                          <RPL>
  ApobPhxLib|AgesaModulePkg/Library/ApobPhxLib/ApobPhxLib.inf                                                          <PHX>
  ApobMdnLib|AgesaModulePkg/Library/ApobMdnLib/ApobMdnLib.inf                                                          <MDN>
  ApobDummyLib|AgesaModulePkg/Library/ApobDummyLib/ApobDummyLib.inf                                                    <ALL>
  AmdPspPsbFusingLib|AgesaModulePkg/Library/AmdPspPsbFusingLib/AmdPspPsbFusingLib.inf                                  <FAMILY17H|FAMILY19H>
  AmdPspHstiStateLib|AgesaModulePkg/Library/AmdPspHstiStateLib/AmdPspHstiStateLib.inf                                  <FAMILY17H|FAMILY19H>
  MpmMboxLib|AgesaModulePkg/Library/MpmMboxLib/MpmMboxLib.inf                                                          <RMB|RPL|PHX>
  MpmLib|AgesaModulePkg/Library/MpmLib/MpmLib.inf                                                                      <RMB|RPL|PHX>
  MpmFuncLib|AgesaModulePkg/Library/MpmFuncLib/MpmFuncLib.inf                                                          <RMB|RPL|PHX>
  MpmDevLib|AgesaModulePkg/Library/MpmDevLib/MpmDevLib.inf                                                             <RMB|RPL|PHX>
  MpmBiosCmdLib|AgesaModulePkg/Library/MpmBiosCmdLib/MpmBiosCmdLib.inf                                                 <RMB|RPL|PHX>
  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf                                                 <ALL>
  AmdDirectoryBaseLib|AgesaModulePkg/Library/AmdDirectoryBaseLib/AmdDirectoryBaseLib.inf                               <ALL>

  ## CCX Lib
  CcxBaseX86Lib|AgesaModulePkg/Library/CcxBaseX86Lib/CcxBaseX86Lib.inf                                                 <ALL>
  CcxBistLib|AgesaModulePkg/Library/CcxBistLib/CcxBistLib.inf                                                          <ALL>
  CcxPspLib|AgesaModulePkg/Library/CcxPspLib/CcxPspLib.inf                                                             <ALL>
  CcxHaltLib|AgesaModulePkg/Library/CcxHaltLib/CcxHaltLib.inf                                                          <ALL>
  CcxMicrocodePatchLib|AgesaModulePkg/Library/CcxMicrocodePatchLib/CcxMicrocodePatchLib.inf                            <ALL>
  CcxRolesLib|AgesaModulePkg/Library/CcxRolesX86Lib/CcxRolesX86Lib.inf                                                 <ALL>
  CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf                                       <ZP|RV>
  CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf                                             <SSP|MTS|RN|FF3>
  CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf                                         <GN|BA|VMR|CZN|RMB|CGL>
  CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen4Lib/CcxPstatesZen4Lib.inf                                         <RS|RPL|PHX>
  CcxStallLib|AgesaModulePkg/Library/CcxStallZenLib/CcxStallZenLib.inf                                                 <FAMILY17H|FAMILY19H>
  DxeCcxBaseX86ServicesLib|AgesaModulePkg/Library/DxeCcxBaseX86ServicesLib/DxeCcxBaseX86ServicesLib.inf                <ALL>
  CcxApicLib|AgesaModulePkg/Library/CcxApicVhLib/CcxApicVhLib.inf                                                      <SSP|MTS|RN|FF3|MDN>
  CcxApicZen3Lib|AgesaModulePkg/Library/CcxApicZen3Lib/CcxApicZen3Lib.inf                                              <GN|BA|VMR|CZN|RMB|CGL>
  CcxApicZen4Lib|AgesaModulePkg/Library/CcxApicZen4Lib/CcxApicZen4Lib.inf                                              <RS|RPL|PHX|MI3|STP>

  ## DF Lib
  FabricPstatesLib|AgesaModulePkg/Library/FabricPstatesZpLib/FabricPstatesZpLib.inf                                    <FAMILY17H|FAMILY19H>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf                     <FAMILY15H|ZP|RV>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf                   <SSP|MTS|GN|VMR|CGL>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf                     <BA>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf                     <RS|STP>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMi3Lib/BaseFabricTopologyMi3Lib.inf                   <MI3>
  MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf                                              <BA>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf                     <RN|CZN>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf                   <FF3>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf                   <MDN>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf                   <RMB>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyPhxLib/BaseFabricTopologyPhxLib.inf                   <PHX>
  BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf                   <RPL>
  PeiFabricTopologyServices2Lib|AgesaModulePkg/Library/PeiFabricTopologyServices2Lib/PeiFabricTopologyServices2Lib.inf <ALL>
  DxeFabricTopologyServices2Lib|AgesaModulePkg/Library/DxeFabricTopologyServices2Lib/DxeFabricTopologyServices2Lib.inf <ALL>
  SmmFabricTopologyServices2Lib|AgesaModulePkg/Library/SmmFabricTopologyServices2Lib/SmmFabricTopologyServices2Lib.inf <ALL>
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf                  <ZP|RV|FAMILY15H>
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf                      <SSP|MTS|RN|GN|VMR|CZN|RMB|FF3|CGL|MDN>
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf                        <BA>
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf                        <RS|MI3|STP>
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccPhxLib/FabricRegisterAccPhxLib.inf                      <PHX>
  FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf                      <RPL>

  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf            <FAMILY15H|ZP|RV>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf            <SSP|MTS|RN|GN|VMR|CZN|CGL>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Ff3Lib/FabricResourceManager3Lib.inf         <FF3>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf         <RMB>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf         <MDN>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerPhxLib/FabricResourceManager3Lib.inf            <PHX>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf             <BA>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf             <RS|STP>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerMi3Lib/FabricResourceManager3Lib.inf            <MI3>
  FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf            <RPL>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInitBasedOnNv3Lib.inf    <SSP|MTS|RN|GN|VMR|CZN|CGL>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerDf3Ff3Lib/FabricResourceInitBasedOnNv3Lib.inf <FF3>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceInitBasedOnNv3Lib.inf <RMB>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceInitBasedOnNv3Lib.inf <MDN>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerPhxLib/FabricResourceInitBasedOnNv3Lib.inf    <PHX>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceInitBasedOnNv3Lib.inf     <BA>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceInitBasedOnNv3Lib.inf     <RS|MI3|STP>
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceInitBasedOnNv3Lib.inf    <RPL>
  PeiFabricResourceManagerServicesLib|AgesaModulePkg/Library/PeiFabricResourceManagerServicesLib/PeiFabricResourceManagerServicesLib.inf    <ALL>
  DxeFabricResourceManagerServicesLib|AgesaModulePkg/Library/DxeFabricResourceManagerServicesLib/DxeFabricResourceManagerServicesLib.inf    <ALL>
  DxeFabricSocSpecificServicesLib|AgesaModulePkg/Library/DxeFabricSocSpecificServicesSspLib/DxeFabricSocSpecificServicesSspLib.inf    <SSP>

  ## MEM Lib
  AmdMemBaseLib|AgesaModulePkg/Library/Mem/BaseLib/AmdMemBaseLib.inf                                                   <ALL>
  AmdMemMainLib|AgesaModulePkg/Library/Mem/MainLib/AmdMemMainLib.inf                                                   <FAMILY15H>
  AmdMemNbLib|AgesaModulePkg/Library/Mem/NbLib/AmdMemNbLib.inf                                                         <FAMILY15H>
  AmdMemNbCzLib|AgesaModulePkg/Library/Mem/NbCzLib/AmdMemNbCzLib.inf                                                   <FAMILY15H>
  AmdMemFeatS3Lib|AgesaModulePkg/Library/Mem/FeatS3Lib/AmdMemFeatS3Lib.inf                                             <FAMILY15H>
  AmdMemErrorLogLib|AgesaModulePkg/Library/Mem/ErrorLogLib/AmdMemErrorLogLib.inf                                       <FAMILY15H>
  HardCodedMemCommonServiceLib|AgesaModulePkg/Library/HardCodedMemCommonServiceLib/HardCodedMemCommonServiceLib.inf    <FAMILY15H>
  OptionMemoryInstallCzLib|AgesaModulePkg/Library/OptionMemoryInstallCzLib/OptionMemoryInstallCzLib.inf                <FAMILY15H>
  MemRestoreLib|AgesaModulePkg/Library/MemRestoreLib/MemRestoreLib.inf                                                 <FAMILY17H|FAMILY19H>

  ## Gnb Lib
  GnbCommonLib|AgesaModulePkg/Library/GnbCommonLib/GnbCommonLib.inf                                                    <ALL>
  GnbMemAccLib|AgesaModulePkg/Library/GnbMemAccLib/GnbMemAccLib.inf                                                    <ALL>
  GnbIoAccLib|AgesaModulePkg/Library/GnbIoAccLib/GnbIoAccLib.inf                                                       <ALL>
  GnbPciAccLib|AgesaModulePkg/Library/GnbPciAccLib/GnbPciAccLib.inf                                                    <FAMILY15H|FAMILY17H|GN|BA|VMR|CZN|RMB|CGL|RPL|PHX>
  GnbPciAccLib|AgesaModulePkg/Library/GnbPciSegmentAccLib/GnbPciSegmentAccLib.inf                                      <RS|MI3|STP>
  GnbCpuAccLib|AgesaModulePkg/Library/GnbCpuAccLib/GnbCpuAccLib.inf                                                    <ALL>
  GnbPciLib|AgesaModulePkg/Library/GnbPciLib/GnbPciLib.inf                                                             <ALL>
  GnbLib|AgesaModulePkg/Library/GnbLib/GnbLib.inf                                                                      <ALL>
  NbioHandleLib|AgesaModulePkg/Library/NbioHandleLib/NbioHandleLib.inf                                                 <ALL>
  NbioIommuIvrsLib|AgesaModulePkg/Library/NbioIommuIvrsLib/NbioIommuIvrsLib.inf                                        <RV|ZP>
  NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf                                                 <RN|FAMILY19H|FF3|SSP|MTS|VMR>
  IvrsDeviceInfoLib|AgesaModulePkg/Nbio/Library/IvrsDeviceDfltLib/IvrsDeviceDfltLib.inf                                <RN|FAMILY19H|FF3|SSP|MTS|VMR|MDN>
  PcieConfigLib|AgesaModulePkg/Library/PcieConfigLib/PcieConfigLib.inf                                                 <ALL>
  GnbSSocketLib|AgesaModulePkg/Library/GnbSSocketLib/GnbSSocketLib.inf                                                 <ALL>
  GnbRegisterAccZpLib|AgesaModulePkg/Library/GnbRegisterAccZpLib/GnbRegisterAccZpLib.inf                               <ALL>
  NbioRegisterAccLib|AgesaModulePkg/Library/NbioRegisterAccLib/NbioRegisterAcc.inf                                     <ALL>
  NbioSmuV9Lib|AgesaModulePkg/Library/NbioSmuV9Lib/NbioSmuV9Lib.inf                                                    <ZP|RV|FAMILY15H>
  NbioSmuV10Lib|AgesaModulePkg/Library/NbioSmuV10Lib/NbioSmuV10Lib.inf                                                 <RV>
  NbioSmuV11Lib|AgesaModulePkg/Library/NbioSmuV11Lib/NbioSmuV11Lib.inf                                                 <SSP|MTS|GN|BA|VMR>
  NbioSmuV12Lib|AgesaModulePkg/Library/NbioSmuV12Lib/NbioSmuV12Lib.inf                                                 <RN|CZN>
  NbioSmuV13Lib|AgesaModulePkg/Library/NbioSmuV13Lib/NbioSmuV13Lib.inf                                                 <RMB|RPL|FF3|PHX|MDN>
  NbioSmuRsLib|AgesaModulePkg/Library/NbioSmuRsLib/NbioSmuRsLib.inf                                                    <RS|MI3>
  NbioSmuMI300Lib|AgesaModulePkg/Library/NbioSmuMI300Lib/NbioSmuMi300Lib.inf                                           <MI3>
  NbioSmuRplLib|AgesaModulePkg/Library/NbioSmuRplLib/NbioSmuRplLib.inf                                                 <RPL>
  NbioTableBlastLib|AgesaModulePkg/Library/NbioTableBlastLib/NbioTableBlastLib.inf                                     <SSP|MTS|RN|BXB|GN|BA|RS|VMR|CZN|RMB|RPL|FF3>
  SmnTableLib|AgesaModulePkg/Nbio/Library/SmnTable/SmnTableLib.inf                                                     <RS|MI3>
  NbioIdsHookCZLib|AgesaModulePkg/Library/NbioIdsHookCZLib/NbioIdsHookCZLib.inf                                        <FAMILY15H>
  NbioIdsHookZPLib|AgesaModulePkg/Library/NbioIdsHookZPLib/NbioIdsHookZPLib.inf                                        <ZP>
  NbioIdsHookRVLib|AgesaModulePkg/Library/NbioIdsHookRVLib/NbioIdsHookRVLib.inf                                        <RV>
  DxioLibV1|AgesaModulePkg/Library/DxioLibV1/DxioLibV1.inf                                                             <ZP|RV>

  DxioLibV2|AgesaModulePkg/Library/DxioLibV2/DxioLibV2.inf                                                             <SSP|MTS|RN|BXB|GN|BA|VMR|CZN|RMB|FF3|PHX|MDN>
  MpioInitLib|AgesaModulePkg/Nbio/Library/MpioInitLib/MpioInitLib.inf                                                  <RS|RPL|PHX|MI3>
  PcieMiscCommLib|AgesaModulePkg/Library/PcieMiscCommLib/PcieMiscCommLib.inf                                           <FAMILY17H|FAMILY19H|MI3>
  NbioAzaliaLib|AgesaModulePkg/Library/NbioAzaliaLib/NbioAzaliaLib.inf                                                 <FAMILY17H|FAMILY19H>
  SmnAccessLib|AgesaModulePkg/Library/SmnAccessLib/SmnAccessLib.inf                                                    <FAMILY17H|FAMILY19H|MI3>
  NbioUtilLib|AgesaModulePkg/Library/NbioUtilLib/NbioUtilLib.inf                                                       <SSP|MTS|RN|GN|BA|RS|VMR|CZN|RMB|RPL|FF3|PHX|MI3>
  BxbSmuV11Lib|AgesaModulePkg/Library/BxbNbio/BxbSmuV11Lib/BxbSmuV11Lib.inf                                            <BXB>
  BxbAccessLib|AgesaModulePkg/Library/BxbNbio/BxbAccessLib/BxbAccessLib.inf                                            <BXB>
  BxbInitLibV1|AgesaModulePkg/Library/BxbNbio/BxbNullLib/BxbNullLib.inf                                                <SSP|MTS|RV|RN|GN|BA|RS|VMR|CZN|RMB|RPL|PHX|MDN>
  BxbSocLibSSP|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibSSP/BxbSocLibSSP.inf                               <SSP|MTS|GN|BA|VMR|RS>
  BxbSocLibRN|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibRN/BxbSocLibRN.inf                                  <RN|CZN>
  BxbSocLibRMB|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibRMB/BxbSocLibRMB.inf                               <RMB|PHX|MDN>
  BxbSocLibRPL|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibRPL/BxbSocLibRPL.inf                               <RPL>
  BxbSocLibZP|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibZP/BxbSocLibZP.inf                                  <ZP>
  BxbSocLibRV|AgesaModulePkg/Library/BxbNbio/BxbInitLibV1/BxbSocLibRV/BxbSocLibRV.inf                                  <RV>
  OemClkReqControlLib|AgesaModulePkg/Library/NbioClkReqControlLibNull/NbioClkReqControlLibNull.inf                     <ZP|RV|SSP|MTS|RN|GN|BA|RS|VMR|CZN|RMB|RPL|VN|AR|PHX>
  NbioServicesLibDxe|AgesaModulePkg/Library/NbioServicesLib/Dxe/NbioServicesLibDxe.inf                                 <GN|BA>
  NbioCommonDxeLib|AgesaModulePkg/Nbio/Library/CommonDxe/NbioCommonDxeLib.inf                                                 <RS|RPL|PHX|MI3>
  Mi200DxeLib|AgesaModulePkg/Nbio/MI200/DXE/Mi200DxeLib.inf                                                            <BA>
  MpioLib|AgesaModulePkg/Nbio/Library/MpioLib/MpioLib.inf                                                              <BA|RS|RPL|PHX|MI3>

  ## Fch Lib
  FchBaseLib|AgesaModulePkg/Library/FchBaseLib/FchBaseLib.inf                                                          <ALL>
  ResetSystemLib|AgesaModulePkg/Library/FchBaseResetSystemLib/FchBaseResetSystemLib.inf                                <ALL>
  FchDxeLibV9|AgesaModulePkg/Library/FchDxeLib/FchDxeLib.inf                                                           <ALL>
  FchSmmLibV9|AgesaModulePkg/Library/FchSmmLib/FchSmmLib.inf                                                           <ALL>
  FchTaishanDxeLib|AgesaModulePkg/Fch/Taishan/FchTaishanCore/FchTaishanLibDxe.inf                                      <ZP|FAMILY15H>
  FchHuashanDxeLib|AgesaModulePkg/Fch/Huashan/FchHuashanCore/FchHuashanLibDxe.inf                                      <SSP|MTS|GN|BA|VMR>
  FchSongshanDxeLib|AgesaModulePkg/Fch/Songshan/FchSongshanCore/FchSongshanLibDxe.inf                                  <RS>
  FchQiantangDxeLib|AgesaModulePkg/Fch/Qiantang/FchQiantangCore/FchQiantangLibDxe.inf                                  <MI3>
  FchSandstoneDxeLib|AgesaModulePkg/Fch/Sandstone/FchSandstoneCore/FchSandstoneLibDxe.inf                              <RV>
  FchShastaDxeLib|AgesaModulePkg/Fch/Shasta/FchShastaCore/FchShastaLibDxe.inf                                          <RN|CZN>
  FchShangDxeLib|AgesaModulePkg/Fch/Shang/FchShangCore/FchShangLibDxe.inf                                              <FF3>
  FchKeithDxeLib|AgesaModulePkg/Fch/Keith/FchKeithCore/FchKeithLibDxe.inf                                              <RMB>
  FchYuntaiDxeLib|AgesaModulePkg/Fch/Yuntai/FchYuntaiCore/FchYuntaiLibDxe.inf                                          <RPL>
  FchTacomaDxeLib|AgesaModulePkg/Fch/Tacoma/FchTacomaCore/FchTacomaLibDxe.inf                                          <PHX>
  FchKeithMdnDxeLib|AgesaModulePkg/Fch/KeithMdn/FchKeithMdnCore/FchKeithMdnLibDxe.inf                                  <MDN>
  FchKernDxeLib|AgesaModulePkg/Fch/Kern/FchKernCore/FchKernLibDxe.inf                                                  <FAMILY15H>
  FchInitHookLibDxe|AgesaModulePkg/Library/FchInitHookLib/FchInitHookLibDxe.inf                                        <ALL>
  FchIdsHookLib|AgesaModulePkg/Library/FchIdsHookLib/FchIdsHookLib.inf                                                 <ALL>
  Mp2I2cLibDxe|AgesaModulePkg/Fch/Sandstone/FchSandstoneI2cDxe/Mp2I2cLibDxe.inf                                        <FP5RV|FT5RV>
  RtcLib|AgesaModulePkg/Library/AmdRtclib/AmdRtcLib.inf                                                                <RMB|RPL|PHX>
  FchEspiCmdLib|AgesaModulePkg/Library/FchEspiCmdLib/FchEspiCmdLib.inf                                                 <RMB>
  LegacyInterruptLib|AgesaModulePkg/Library/FchDxeLegacyInterruptLib/Songshan/FchDxeLegacyInterruptLib.inf             <RS>
  LegacyInterruptLib|AgesaModulePkg/Library/FchDxeLegacyInterruptLib/Songshan/FchDxeLegacyInterruptLib.inf             <MI3>

  ## FCH SPI ACCESS lib
  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom2Lib.inf                                       <ALL>
#  FchSpiAccessLib|AgesaModulePkg/Library/FchSpiAccessLib/FchSpiAccessRom2V2Lib.inf                                     <ALL>

  ## Bixby Lib
  FchBixbyDxeLib|AgesaModulePkg/Fch/Bixby/FchBixbyCore/FchBixbyLibDxe.inf                                              <BXB>

  ## Ras Lib
  RasZpBaseLib|AgesaModulePkg/Library/RasZpBaseLib/RasZpBaseLib.inf                                                    <ZP>
  RasRvBaseLib|AgesaModulePkg/Library/RasRvBaseLib/RasRvBaseLib.inf                                                    <RV>
  RasSspBaseLib|AgesaModulePkg/Library/RasSspBaseLib/RasSspBaseLib.inf                                                 <SSP|MTS|VMR|RN|CZN>
  RasBrBaseLib|AgesaModulePkg/Library/RasBrBaseLib/RasBrBaseLib.inf                                                    <FAMILY15H>
  RasGnBaseLib|AgesaModulePkg/Library/RasGnBaseLib/RasGnBaseLib.inf                                                    <GN>
  RasBaBaseLib|AgesaModulePkg/Library/RasBaBaseLib/RasBaBaseLib.inf                                                    <BA>

  ## Usb4 Lib
  AmdUsb4BaseLib|AgesaModulePkg/Library/AmdUsb4Lib/AmdUsb4Lib.inf                                                      <RMB|PHX>

[Components.IA32]
  AgesaModulePkg/Debug/AmdIdsDebugPrintPei/AmdIdsDebugPrintPei.inf                                                     <ALL>

  ##PSP Drivers
  AgesaModulePkg/Psp/AmdPspPeiV1/AmdPspPeiV1.inf                                                                       <FAMILY15H>
  AgesaModulePkg/Psp/AmdPspPeiV2/AmdPspPeiV2.inf                                                                       <RV|ZP|RN|CZN|VN|MR|AR>
  AgesaModulePkg/Psp/AmdPspPeiV2Ssp/AmdPspPeiV2.inf                                                                    <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Psp/AmdPspPeiV2Rmb/AmdPspPeiV2.inf                                                                    <RMB>
  AgesaModulePkg/Psp/AmdPspPeiV2Rpl/AmdPspPeiV2.inf                                                                    <RPL>
  AgesaModulePkg/Psp/AmdPspPeiV2Phx/AmdPspPeiV2.inf                                                                    <PHX>
  AgesaModulePkg/Psp/AmdPspPeiV2Mdn/AmdPspPeiV2.inf                                                                    <MDN>
  AgesaModulePkg/Psp/AmdPspPeiV2Rs/AmdPspPeiV2.inf                                                                     <RS>
  AgesaModulePkg/Psp/AmdPspPeiV2Mi3/AmdPspPeiV2.inf                                                                    <MI3>
  AgesaModulePkg/Psp/AmdPspFtpmPei/AmdPspFtpmPei.inf                                                                   <AM4|RV|SP3TR|SP3CP|SP4SM|RN|VMR|CZN|VN|RMB|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdHspFtpmPei/AmdHspFtpmPei.inf                                                                   <VN|RMB|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdPspDtpmPei/AmdPspDtpmPei.inf                                                                   <SP3SSP|SP3GN>
  AgesaModulePkg/Psp/AmdPspPsbDisablePei/AmdPspPsbDisablePei.inf                                                       <SP3SSP|SP3GN>
  AgesaModulePkg/Psp/ApobDrv/ApobZpRvPei/ApobZpRvPei.inf                                                               <AM4RV|FP5RV|AM4SM|SP4SM|FT5RV>
  AgesaModulePkg/Psp/ApobDrv/ApobZpMcmPei/ApobZpMcmPei.inf                                                             <SP3ZP|SP4ZP|SP3TR>
  AgesaModulePkg/Psp/ApobDrv/ApobSspPei/ApobSspPei.inf                                                                 <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Psp/ApobDrv/ApobRnPei/ApobRnPei.inf                                                                   <RN|CZN>
  AgesaModulePkg/Psp/ApobDrv/ApobRmbPei/ApobRmbPei.inf                                                                 <RMB>
  AgesaModulePkg/Psp/ApobDrv/ApobFf3Pei/ApobFf3Pei.inf                                                                 <VN|MR|AR>
  AgesaModulePkg/Psp/ApobDrv/ApobRsPei/ApobRsPei.inf                                                                   <RS>
  AgesaModulePkg/Psp/ApobDrv/ApobRplPei/ApobRplPei.inf                                                                 <RPL>
  AgesaModulePkg/Psp/ApobDrv/ApobPhxPei/ApobPhxPei.inf                                                                 <PHX>
  AgesaModulePkg/Psp/ApobDrv/ApobMdnPei/ApobMdnPei.inf                                                                 <MDN>
  AgesaModulePkg/Psp/ApobDrv/ApobDummyPei/ApobDummyPei.inf                                                             <FAMILY15H>
  AgesaModulePkg/Psp/AmdPspIntrusionDetection/AmdPspIntrusionDetectionPei.inf                                          <RMB>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPei/MpmPei.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmAsf/MpmAsfPei/MpmAsfPei.inf
!endif                                                                                                                 <RMB|RPL|PHX>

  AgesaModulePkg/Ccx/Zen/CcxZenZpPei/AmdCcxZenZpPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZpLib/CcxResetTablesZpLib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZpLib/CcxResetTablesZpLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Ccx/Zen/CcxZenRvPei/AmdCcxZenRvPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesRvLib/CcxResetTablesRvLib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesRvLib/CcxResetTablesRvLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Ccx/Vh/Pei/AmdCcxVhPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhLib/CcxResetTablesVhLib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
  }                                                                                                                    <SSP|MTS>
  AgesaModulePkg/Ccx/Vh/Pei/Rn/AmdCcxVhRnPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhLib/CcxResetTablesVhLib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
  }                                                                                                                    <RN>
  AgesaModulePkg/Ccx/Vh/Pei/Ff3/AmdCcxVhFf3Pei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhV2Lib/CcxResetTablesVhV2Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Ccx/Vh/Pei/Mdn/AmdCcxVhMdnPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhV2Lib/CcxResetTablesVhV2Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Ccx/Zen3/Pei/AmdCcxZen3Pei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
  }                                                                                                                    <VMR|CGL>
  AgesaModulePkg/Ccx/Zen3/Pei/Czn/AmdCcxZen3CznPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
  }                                                                                                                    <CZN>
  AgesaModulePkg/Ccx/Zen3/Pei/Rmb/AmdCcxZen3RmbPei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3V2Lib/CcxResetTablesZen3V2Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Ccx/Zen3/Pei/AmdCcxZen3Pei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
  }                                                                                                                    <GN|BA>
  AgesaModulePkg/Ccx/Zen4/Pei/AmdCcxZen4Pei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen4Lib/CcxResetTablesZen4Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen4Lib/CcxRolesZen4Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen4Lib/CcxPstatesZen4Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen4Lib/CcxSetMcaZen4Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf4Lib/FabricWdtDf4Lib.inf
  }                                                                                                                    <RS|RPL|PHX|STP>
  AgesaModulePkg/Ccx/Zen4/Pei/Mi3/AmdCcxZen4Mi3Pei.inf {
    <LibraryClasses>
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen4Lib/CcxResetTablesZen4Lib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen4Lib/CcxRolesZen4Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen4Lib/CcxPstatesZen4Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen4Lib/CcxSetMcaZen4Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf4Lib/FabricWdtDf4Lib.inf
  }                                                                                                                    <MI3>
  AgesaModulePkg/Ccx/Xv/CcxXvPei/AmdCcxXvPei.inf {
    <LibraryClasses>
    ApCommunicationLib|AgesaModulePkg/Library/ApCommunicationXvLib/ApCommunicationXvLib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxDownCoreLib|AgesaModulePkg/Library/CcxDownCoreXvLib/CcxDownCoreXvLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesXvLib/CcxPstatesXvLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/PeiXv/AmdTableHookPeiXvLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Fabric/ZP/FabricZpPei/AmdFabricZpPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Fabric/SSP/FabricSspPei/AmdFabricSspPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <SSP|MTS|VMR|CGL>
  AgesaModulePkg/Fabric/SSP/FabricSspPei/AmdFabricSspPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                      <GN>
  AgesaModulePkg/Fabric/BA/FabricBaPei/AmdFabricBaPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
  }                                                                                                                      <BA>
  AgesaModulePkg/Fabric/RS/FabricRsPei/AmdFabricRsPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
  }                                                                                                                      <RS|STP>
  AgesaModulePkg/Fabric/MI3/FabricMi3Pei/AmdFabricMi3Pei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
  }                                                                                                                      <MI3>
  AgesaModulePkg/Fabric/RV/FabricRvPei/AmdFabricRvPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Fabric/RN/FabricRnPei/AmdFabricRnPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Fabric/FF3/FabricFf3Pei/AmdFabricFf3Pei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Fabric/RMB/FabricRmbPei/AmdFabricRmbPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RMB>
 AgesaModulePkg/Fabric/MDN/FabricMdnPei/AmdFabricMdnPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <MDN>
    AgesaModulePkg/Fabric/PHX/FabricPhxPei/AmdFabricPhxPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyPhxLib/BaseFabricTopologyPhxLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccPhxLib/FabricRegisterAccPhxLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerPhxLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <PHX>
  AgesaModulePkg/Fabric/RPL/FabricRplPei/AmdFabricRplPei.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Fabric/XV/UnbXvPei/AmdUnbXvPei.inf {
    <LibraryClasses>
    FabricPstatesLib|AgesaModulePkg/Library/FabricPstatesUnbLib/FabricPstatesUnbLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/PeiXv/AmdTableHookPeiXvLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Nbio/SMU/AmdNbioSmuV8Pei/AmdNbioSmuV8Pei.inf                                                          <FAMILY15H>
  AgesaModulePkg/Nbio/SMU/AmdNbioSmuV9Pei/AmdNbioSmuV9Pei.inf                                                          <ZP>
  AgesaModulePkg/Nbio/SMU/AmdNbioSmuV10Pei/AmdNbioSmuV10Pei.inf                                                        <RV>
  AgesaModulePkg/Nbio/PCIe/AmdNbioPcieZPPei/AmdNbioPcieZPPei.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Nbio/PCIe/AmdNbioPcieRVPei/AmdNbioPcieRVPei.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Nbio/NbioBase/AmdNbioBaseZPPei/AmdNbioBaseZPPei.inf                                                   <ZP>
  AgesaModulePkg/Nbio/NbioBase/AmdNbioBaseCZPei/AmdNbioBaseCZPei.inf                                                   <FAMILY15H>
  AgesaModulePkg/Nbio/NbioBase/AmdNbioBaseRVPei/AmdNbioBaseRVPei.inf                                                   <RV>
  AgesaModulePkg/Nbio/GFX/AmdNbioGfxRVPei/AmdNbioGfxRVPei.inf                                                          <RV>
  AgesaModulePkg/Nbio/GFX/RN/PEI/AmdNbioGfxRNPei.inf                                                                   <RN|CZN>
  AgesaModulePkg/Nbio/GFX/RMB/PEI/AmdNbioGfxRMBPei.inf                                                                 <RMB>
  AgesaModulePkg/Nbio/GFX/RPL/PEI/AmdNbioGfxRPLPei.inf                                                                 <RPL>
  AgesaModulePkg/Nbio/GFX/MDN/PEI/AmdNbioGfxMDNPei.inf                                                                 <MDN>
  AgesaModulePkg/Nbio/IOMMU/AmdNbioIOMMUZPPei/AmdNbioIOMMUZPPei.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
  }                                                                                                                    <ZP|RV>
  AgesaModulePkg/Nbio/SMU/SSP/PEI/SmuV11PeiSsp.inf                                                                     <SSP|MTS>
  AgesaModulePkg/Nbio/SMU/VMR/PEI/SmuV11PeiVmr.inf                                                                     <VMR|CGL>
  AgesaModulePkg/Nbio/SMU/GN/PEI/SmuV11PeiGn.inf                                                                       <GN>
  AgesaModulePkg/Nbio/SMU/BA/PEI/SmuV11PeiBa.inf                                                                       <BA>
  AgesaModulePkg/Nbio/PCIe/SSP/PEI/AmdNbioPcieSspPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <SSP|MTS|VMR>
  AgesaModulePkg/Nbio/PCIe/GN/PEI/AmdNbioPcieGnPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Nbio/PCIe/BA/PEI/AmdNbioPcieBaPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Nbio/IOAPIC/SSP/PEI/AmdNbioIoApicSspPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <SSP|MTS|VMR|GN>
  AgesaModulePkg/Nbio/NbioBase/SSP/PEI/AmdNbioBaseSspPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <SSP|MTS|VMR>
  AgesaModulePkg/Nbio/NbioBase/GN/PEI/AmdNbioBaseGnPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Nbio/NbioBase/BA/PEI/AmdNbioBaseBaPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Nbio/RS/PEI/NbioPeiRs.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RS>
  AgesaModulePkg/Nbio/MI300/PEI/NbioPeiMI300.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMi3Lib/BaseFabricTopologyMi3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerMi3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <MI3>
  AgesaModulePkg/Nbio/PHX/PEI/NbioPeiPhx.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
  }                                                                                                                    <PHX>
  AgesaModulePkg/Nbio/IOMMU/SSP/PEI/AmdNbioIOMMUSspPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <SSP|MTS|VMR>
  AgesaModulePkg/Nbio/IOMMU/GN/PEI/AmdNbioIOMMUGnPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Nbio/IOMMU/BA/PEI/AmdNbioIOMMUBaPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Nbio/BIXBY/PEI/BixbyPei.inf                                                                           <BXB>
  AgesaModulePkg/Nbio/BIXBY/BixbyEarlyLink/BixbyEarlyLink.inf                                                          <BXB>
  AgesaModulePkg/Nbio/SMU/RN/PEI/SmuV12PeiRN.inf                                                                       <RN>
  AgesaModulePkg/Nbio/SMU/CZN/PEI/SmuV12PeiCZN.inf                                                                     <CZN>
  AgesaModulePkg/Nbio/SMU/RMB/PEI/SmuV13PeiRMB.inf                                                                     <RMB>
  AgesaModulePkg/Nbio/SMU/RPL/PEI/SmuV13PeiRPL.inf                                                                     <RPL>
  AgesaModulePkg/Nbio/SMU/MDN/PEI/SmuV13PeiMDN.inf                                                                     <MDN>
  AgesaModulePkg/Nbio/PCIe/RN/PEI/AmdNbioPcieRNPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Nbio/NbioBase/RN/PEI/AmdNbioBaseRNPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Nbio/IOMMU/RN/PEI/AmdNbioIOMMURNPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Nbio/PCIe/RMB/PEI/AmdNbioPcieRMBPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Nbio/NbioBase/RMB/PEI/AmdNbioBaseRMBPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Nbio/IOMMU/RMB/PEI/AmdNbioIOMMURMBPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Nbio/PCIe/RPL/PEI/AmdNbioPcieRPLPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Nbio/NbioBase/RPL/PEI/AmdNbioBaseRPLPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Nbio/IOMMU/RPL/PEI/AmdNbioIOMMURPLPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Nbio/PCIe/MDN/PEI/AmdNbioPcieMDNPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Nbio/NbioBase/MDN/PEI/AmdNbioBaseMDNPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Nbio/IOMMU/MDN/PEI/AmdNbioIOMMUMDNPei.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Nbio/GFX/FF3/PEI/AmdNbioGfxFF3Pei.inf                                                                 <FF3>
  AgesaModulePkg/Nbio/SMU/FF3/PEI/SmuV13PeiFF3.inf                                                                     <FF3>
  AgesaModulePkg/Nbio/PCIe/FF3/PEI/AmdNbioPcieFF3Pei.inf                                                               <VN|AR>
  AgesaModulePkg/Nbio/PCIe/MR/PEI/AmdNbioPcieMRPei.inf                                                                 <MR>
  AgesaModulePkg/Nbio/NbioBase/FF3/PEI/AmdNbioBaseFF3Pei.inf                                                           <FF3>
  AgesaModulePkg/Nbio/IOMMU/FF3/PEI/AmdNbioIOMMUFF3Pei.inf                                                             <FF3>

  AgesaModulePkg/Fch/Taishan/FchTaishanPei/FchPei.inf                                                                  <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanSmbusPei/Smbus.inf                                                              <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanMultiFchPei/FchMultiFchPei.inf  {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Fch/Huashan/FchHuashanPei/FchPei.inf                                                                  <SSP|MTS|GN|BA|VMR|CGL>
  AgesaModulePkg/Fch/Huashan/FchHuashanSmbusPei/Smbus.inf                                                              <SSP|MTS|GN|BA|VMR|CGL>
  AgesaModulePkg/Fch/Huashan/FchHuashanMultiFchPei/FchMultiFchPei.inf  {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
  }                                                                                                                    <SSP|GN>
  AgesaModulePkg/Fch/Songshan/FchSongshanPei/FchPei.inf                                                                <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanSmbusPei/Smbus.inf                                                            <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanI3cPei/I3c.inf                                                                <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanMultiFchPei/FchMultiFchPei.inf  {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
  }                                                                                                                    <RS>

  AgesaModulePkg/Fch/Qiantang/FchQiantangPei/FchPei.inf                                                                <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangSmbusPei/Smbus.inf                                                            <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangMultiFchPei/FchMultiFchPei.inf  {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerMi3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
  }                                                                                                                    <MI3>

  AgesaModulePkg/Fch/Sandstone/FchSandstonePei/FchPei.inf                                                              <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneSmbusPei/Smbus.inf                                                          <RV>
  AgesaModulePkg/Fch/Shasta/FchShastaPei/FchPei.inf                                                                    <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaSmbusPei/Smbus.inf                                                                <RN|CZN>
  AgesaModulePkg/Fch/Shang/FchShangPei/FchPei.inf                                                                      <FF3>
  AgesaModulePkg/Fch/Shang/FchShangSmbusPei/Smbus.inf                                                                  <FF3>
  AgesaModulePkg/Fch/Keith/FchKeithPei/FchPei.inf                                                                      <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithSmbusPei/Smbus.inf                                                                  <RMB>
  AgesaModulePkg/Fch/Common/FchEspiCmdPei/FchEspiCmdPei.inf                                                            <RMB>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiPei/FchPei.inf                                                                    <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiSmbusPei/Smbus.inf                                                                <RPL>
  AgesaModulePkg/Fch/Tacoma/FchTacomaPei/FchPei.inf                                                                    <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaSmbusPei/Smbus.inf                                                                <PHX>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnPei/FchPei.inf                                                                <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnSmbusPei/Smbus.inf                                                            <MDN>
  AgesaModulePkg/Fch/Kern/FchKernPei/FchPei.inf                                                                        <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernSmbusPei/Smbus.inf                                                                    <FAMILY15H>
  AgesaModulePkg/Mem/AmdMemFp8Pei/AmdMemFp8Pei.inf                                                                     <PHX>
  AgesaModulePkg/Mem/AmdMemFt6Pei/AmdMemFt6Pei.inf                                                                     <MDN>
  AgesaModulePkg/Mem/AmdMemAm4RnPei/AmdMemAm4RnPei.inf                                                                 <RN|CZN>
  AgesaModulePkg/Mem/AmdMemAm4ZpPei/AmdMemAm4ZpPei.inf                                                                 <AM4BR|AM4RV|FP5RV|AM4SM|SP4SM|FT5RV>
  AgesaModulePkg/Mem/AmdMemAm4SspPei/AmdMemAm4SspPei.inf                                                               <AM4MTS|AM4VMR>
  AgesaModulePkg/Mem/AmdMemSspSp3Pei/AmdMemSspSp3Pei.inf                                                               <SP3SSP|SP3CP|SP3GN>
  AgesaModulePkg/Mem/AmdMemRsSp5Pei/AmdMemRsSp5Pei.inf                                                                 <SP5RS>
  AgesaModulePkg/Mem/AmdMemBaSp3Pei/AmdMemBaSp3Pei.inf                                                                 <SP3BA>
  AgesaModulePkg/Mem/AmdMemZpSp3Pei/AmdMemZpSp3Pei.inf                                                                 <SP3ZP|SP4ZP>
  AgesaModulePkg/Mem/AmdMemZpSp3r2Pei/AmdMemZpSp3r2Pei.inf                                                             <SP3TR>
  AgesaModulePkg/Mem/AmdMemFf3Pei/AmdMemFf3Pei.inf                                                                     <FF3>
  AgesaModulePkg/Mem/AmdMemFp7Pei/AmdMemFp7Pei.inf                                                                     <FP7|FP7R2>
  AgesaModulePkg/Mem/AmdMemAm5Pei/AmdMemAm5Pei.inf                                                                     <AM5>
  AgesaModulePkg/Soc/AmdSocAm4VmrPei/AmdSocAm4VmrPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesMtsLib/PeiFabricSocSpecificServicesMtsLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesVmrLib/PeiSocZen3ServicesVmrLib.inf
  }                                                                                                                    <AM4VMR|AM4CGL>
  AgesaModulePkg/Soc/AmdSocAm4MtsPei/AmdSocAm4MtsPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistVhCcdLib/PeiSocBistVhCcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesMtsLib/PeiFabricSocSpecificServicesMtsLib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesMtsLib/PeiSocValhallaServicesMtsLib.inf
  }                                                                                                                    <AM4MTS>
  AgesaModulePkg/Soc/AmdSocAm4RvPei/AmdSocAm4RvPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRvDieLib/BaseSocketLogicalIdRvDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistRvLib/PeiSocBistRvLib.inf
    PeiSocZenServicesLib|AgesaModulePkg/Library/PeiSocZenServicesLib/PeiSocZenServicesLib.inf
  }                                                                                                                    <AM4RV>
  AgesaModulePkg/Soc/AmdSocAm4SmPei/AmdSocAm4SmPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZpLib/PeiSocBistZpLib.inf
  }                                                                                                                    <AM4SM>
  AgesaModulePkg/Soc/AmdSocSp4SmPei/AmdSocSp4SmPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZpLib/PeiSocBistZpLib.inf
  }                                                                                                                    <SP4SM>
  AgesaModulePkg/Soc/AmdSocAm4BrPei/AmdSocAm4BrPei.inf {
    <LibraryClasses>
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdUnbLib/BaseSocketLogicalIdUnbLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyUnbLib/BaseFabricTopologyUnbLib.inf
  }                                                                                                                    <AM4BR>
  AgesaModulePkg/Soc/AmdSocFp5RvPei/AmdSocFp5RvPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRvDieLib/BaseSocketLogicalIdRvDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistRvLib/PeiSocBistRvLib.inf
  }                                                                                                                    <FP5RV>
  AgesaModulePkg/Soc/AmdSocFp6RnPei/AmdSocFp6RnPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRnDieLib/BaseSocketLogicalIdRnDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRnDieLib/BaseSocLogicalIdXlatRnDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistRnLib/PeiSocBistRnLib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesRnLib/PeiSocValhallaServicesRnLib.inf
  }                                                                                                                    <FP6RN>
  AgesaModulePkg/Soc/AmdSocFf3VnPei/AmdSocFf3VnPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdFf3DieLib/BaseSocketLogicalIdFf3DieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatFf3DieLib/BaseSocLogicalIdXlatFf3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistFf3Lib/PeiSocBistFf3Lib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesFf3Lib/PeiSocValhallaServicesFf3Lib.inf
  }                                                                                                                    <FF3VN|FF3AR>
  AgesaModulePkg/Soc/AmdSocFf3MrPei/AmdSocFf3MrPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdFf3DieLib/BaseSocketLogicalIdFf3DieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatFf3DieLib/BaseSocLogicalIdXlatFf3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistFf3Lib/PeiSocBistFf3Lib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesFf3Lib/PeiSocValhallaServicesFf3Lib.inf
  }                                                                                                                    <FF3MR>
  AgesaModulePkg/Soc/AmdSocAm4RnPei/AmdSocAm4RnPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRnDieLib/BaseSocketLogicalIdRnDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRnDieLib/BaseSocLogicalIdXlatRnDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistRnLib/PeiSocBistRnLib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesRnLib/PeiSocValhallaServicesRnLib.inf
  }                                                                                                                    <AM4RN>
  AgesaModulePkg/Soc/AmdSocSp3GnPei/AmdSocSp3GnPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesGnLib/PeiSocZen3ServicesGnLib.inf
  }                                                                                                                    <SP3GN>
  AgesaModulePkg/Soc/AmdSocSp3r4BaPei/AmdSocSp3r4BaPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesGnLib/PeiSocZen3ServicesGnLib.inf
  }                                                                                                                    <SP3BA>
  AgesaModulePkg/Soc/AmdSocSp5RsPei/AmdSocSp5RsPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen4CcdLib/PeiSocBistZen4CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen4ServicesLib|AgesaModulePkg/Library/PeiSocZen4ServicesRsLib/PeiSocZen4ServicesRsLib.inf
  }                                                                                                                    <SP5RS>
  AgesaModulePkg/Soc/AmdSocSh5Mi3Pei/AmdSocSh5Mi3Pei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerMi3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen4CcdLib/PeiSocBistZen4CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen4ServicesLib|AgesaModulePkg/Library/PeiSocZen4ServicesRsLib/PeiSocZen4ServicesRsLib.inf
  }                                                                                                                    <SH5Mi3>
  AgesaModulePkg/Soc/AmdSocAm4CznPei/AmdSocAm4CznPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesMtsLib/PeiFabricSocSpecificServicesMtsLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesCznLib/PeiSocZen3ServicesCznLib.inf
  }                                                                                                                    <AM4CZN>
  AgesaModulePkg/Soc/AmdSocFp6CznPei/AmdSocFp6CznPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesMtsLib/PeiFabricSocSpecificServicesMtsLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesCznLib/PeiSocZen3ServicesCznLib.inf
  }                                                                                                                    <FP6CZN>
  AgesaModulePkg/Soc/AmdSocAm5RmbPei/AmdSocAm5RmbPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceInitBasedOnNv3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesRmbLib/PeiSocZen3ServicesRmbLib.inf
  }                                                                                                                    <AM5RMB>
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
  }                                                                                                                    <FP7RMB>
  AgesaModulePkg/Soc/AmdSocFp7r2RmbPei/AmdSocFp7r2RmbPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesRmbLib/PeiSocZen3ServicesRmbLib.inf
  }                                                                                                                    <FP7R2RMB>
  AgesaModulePkg/Soc/AmdSocFp8PhxPei/AmdSocFp8PhxPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerPhxLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerPhxLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiSocZen4ServicesLib|AgesaModulePkg/Library/PeiSocZen4ServicesPhxLib/PeiSocZen4ServicesPhxLib.inf
  }                                                                                                                    <FP8PHX>
    AgesaModulePkg/Soc/AmdSocFt6MdnPei/AmdSocFt6MdnPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesMdnLib/PeiSocValhallaServicesMdnLib.inf
  }                                                                                                                    <FT6MDN>
  AgesaModulePkg/Soc/AmdSocSp3RmPei/AmdSocSp3RmPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistVhCcdLib/PeiSocBistVhCcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesSspLib/PeiSocValhallaServicesSspLib.inf
  }                                                                                                                    <SP3SSP>
  AgesaModulePkg/Soc/AmdSocSp3ZpPei/AmdSocSp3ZpPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZpLib/PeiSocBistZpLib.inf
  }                                                                                                                    <SP3ZP>
  AgesaModulePkg/Soc/AmdSocSp4ZpPei/AmdSocSp4ZpPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZpLib/PeiSocBistZpLib.inf
  }                                                                                                                    <SP4ZP>
  AgesaModulePkg/Soc/AmdSocSp3r2TrPei/AmdSocSp3r2TrPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZpLib/PeiSocBistZpLib.inf
  }                                                                                                                    <SP3TR>
  AgesaModulePkg/Soc/AmdSocSp3r3CpPei/AmdSocSp3r3CpPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistVhCcdLib/PeiSocBistVhCcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocValhallaServicesLib|AgesaModulePkg/Library/PeiSocValhallaServicesSspLib/PeiSocValhallaServicesSspLib.inf
  }                                                                                                                    <SP3CP>
  AgesaModulePkg/Soc/AmdSocSp3r3CglPei/AmdSocSp3r3CglPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen3CcdLib/PeiSocBistZen3CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen3ServicesLib|AgesaModulePkg/Library/PeiSocZen3ServicesGnLib/PeiSocZen3ServicesGnLib.inf
  }                                                                                                                    <SP3CGL>
  AgesaModulePkg/Soc/AmdSocFt5RvPei/AmdSocFt5RvPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceInit1Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRvDieLib/BaseSocketLogicalIdRvDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistRvLib/PeiSocBistRvLib.inf
  }                                                                                                                    <FT5RV>
  AgesaModulePkg/Soc/AmdSocAm5RplPei/AmdSocAm5RplPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
    FabricResourceInitBasedOnNvLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceInitBasedOnNv3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen4CcdLib/PeiSocBistZen4CcdLib.inf
    #@Todo PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesMtsLib/PeiFabricSocSpecificServicesMtsLib.inf
    PeiSocZen4ServicesLib|AgesaModulePkg/Library/PeiSocZen4ServicesRplLib/PeiSocZen4ServicesRplLib.inf
  }                                                                                                                    <AM5RPL>
  AgesaModulePkg/Soc/AmdSocSp6StpPei/AmdSocSp6StpPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen4CcdLib/PeiSocBistZen4CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen4ServicesLib|AgesaModulePkg/Library/PeiSocZen4ServicesRsLib/PeiSocZen4ServicesRsLib.inf
  }                                                                                                                    <SP6STP>
  AgesaModulePkg/Soc/AmdSocTr5StpPei/AmdSocTr5StpPei.inf {
    <LibraryClasses>
    FabricResourceInitLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceInit3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    PeiSocBistLib|AgesaModulePkg/Library/PeiSocBistZen4CcdLib/PeiSocBistZen4CcdLib.inf
    PeiFabricSocSpecificServicesLib|AgesaModulePkg/Library/PeiFabricSocSpecificServicesSspLib/PeiFabricSocSpecificServicesSspLib.inf
    PeiSocZen4ServicesLib|AgesaModulePkg/Library/PeiSocZen4ServicesRsLib/PeiSocZen4ServicesRsLib.inf
  }                                                                                                                    <TR5STP>

  AgesaModulePkg/Mem/AmdMemFeatPei/AmdMemFeatPei.inf                                                                   <FAMILY15H>
  AgesaModulePkg/Mem/AmdMemCzPei/AmdMemCzPei.inf {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesXvLib/CcxPstatesXvLib.inf
    FabricPstatesLib|AgesaModulePkg/Library/FabricPstatesUnbLib/FabricPstatesUnbLib.inf
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Mem/AmdMemChanXLatDummyPei/MemChanXLatDummyPei.inf                                                    <RN|CZN|FF3|FP7|FP7R2|AM5|PHX|Ft6>
  AgesaModulePkg/Mem/AmdMemChanXLatZpPei/MemChanXLatZpPei.inf                                                          <ZP|RV>
  AgesaModulePkg/Mem/AmdMemChanXLatSspPei/MemChanXLatSspPei.inf                                                        <SSP|MTS|GN|BA|VMR|RS>
  AgesaModulePkg/Mem/AmdMemSmbiosV2RnPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RnLib/MemSmbiosV2Lib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Mem/AmdMemSmbiosV2RvPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RvLib/MemSmbiosV2Lib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Mem/AmdMemSmbiosV2ZpPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2ZpLib/MemSmbiosV2Lib.inf
  }                                                                                                                    <AM4SM|SP4SM>
  AgesaModulePkg/Mem/AmdMemSmbiosV2ZpMcmPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2ZpMcmLib/MemSmbiosV2Lib.inf
  }                                                                                                                    <SP3ZP|SP4ZP|SP3TR>
  AgesaModulePkg/Mem/AmdMemSmbiosV2SspPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2SspLib/MemSmbiosV2Lib.inf
  }                                                                                                                    <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Mem/AmdMemSmbiosV2Ff3Pei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2Ff3Lib/MemSmbiosV2Lib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Mem/AmdMemSmbiosV2RmbPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RmbLpd5Lib/MemSmbiosV2Lib.inf
  }                                                                                                                    <FP7>
  AgesaModulePkg/Mem/AmdMemSmbiosV2RmbPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RmbD5Lib/MemSmbiosV2Lib.inf
  }                                                                                                                    <FP7R2|AM5RMB>
  AgesaModulePkg/Mem/AmdMemSmbiosV2RplPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RplD5Lib/MemSmbiosV2Lib.inf
  }                                                                                                                    <AM5RPL>
  AgesaModulePkg/Mem/AmdMemSmbiosV2PhxPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2PhxLpd5Lib/MemSmbiosV2Lib.inf
  }                                                                                                                    <PHX>
  AgesaModulePkg/Mem/AmdMemSmbiosV2RsPei/MemSmbiosV2Pei.inf {
    <LibraryClasses>
    AmdMemSmbiosV2Lib|AgesaModulePkg/Library/MemSmbiosV2RsD5Lib/MemSmbiosV2Lib.inf
  }                                                                                                                    <SP5RS>
  AgesaModulePkg/Mem/AmdMemRestorePei/MemRestorePei.inf                                                                <FAMILY17H|FAMILY19H>
  AgesaModulePkg/Mem/AmdMbistZpSp3Pei/AmdMbistPei.inf                                                                  <SP3ZP|SP4ZP>
  AgesaModulePkg/Mem/AmdMbistSspPei/AmdMbistPei.inf                                                                    <SP3SSP|SP3GN|SP3BA|SP5RS>
  AgesaModulePkg/Mem/AmdMemTestExPei/AmdMemTestExPei.inf                                                               <SP3GN>
  AgesaModulePkg/ErrorLog/AmdErrorLogPei/AmdErrorLogPei.inf                                                            <ALL>
  AgesaModulePkg/Nbio/EarlyPhase/AmdNbioEarlyPhaseRVPei/AmdNbioEarlyPhaseRVPei.inf                                     <RV>
  AgesaModulePkg/Nbio/EarlyPhase/AmdNbioEarlyPhaseRNPei/AmdNbioEarlyPhaseRNPei.inf                                     <RN|CZN>
  AgesaModulePkg/Nbio/EarlyPhase/AmdNbioEarlyPhaseRMBPei/AmdNbioEarlyPhaseRMBPei.inf                                   <RMB>
  AgesaModulePkg/Nbio/EarlyPhase/AmdNbioEarlyPhaseRPLPei/AmdNbioEarlyPhaseRPLPei.inf                                   <RPL>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimRn/AmdMemoryHobInfoPeim.inf                                                   <RN|CZN>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimRv/AmdMemoryHobInfoPeim.inf                                                   <RV>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimZp/AmdMemoryHobInfoPeim.inf                                                   <ZP|AM4BR>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimV2/AmdMemoryHobInfoPeimV2.inf                                                 <SP3SSP|SP3CP|AM4MTS|SP3GN|AM4VMR|AM4CGL>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimV3/AmdMemoryHobInfoPeimV3.inf                                                 <SP3BA>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimRs/AmdMemoryHobInfoPeimRs.inf                                                 <SP5RS>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimFf3/AmdMemoryHobInfoPeim.inf                                                  <FF3>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimRmb/AmdMemoryHobInfoPeim.inf                                                  <RMB>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimRpl/AmdMemoryHobInfoPeim.inf                                                  <RPL>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimPhx/AmdMemoryHobInfoPeim.inf                                                  <PHX>
  AgesaModulePkg/Mem/AmdMemoryHobInfoPeimFt6/AmdMemoryHobInfoPeim.inf                                                  <FT6>
  AgesaModulePkg/Universal/Version/AmdVersionPei/AmdVersionPei.inf                                                     <ALL>

  AgesaModulePkg/Fch/Bixby/FchBixbyPei/FchPei.inf                                                                      <BXB>
  AgesaModulePkg/Fch/Bixby/FchBixbyPei/FchBixbyGpioPei.inf                                                             <BXB>
  AgesaModulePkg/Fch/Bixby/FchBixbyPei/FchBixbyEarlyLink.inf                                                           <BXB>

  AgesaModulePkg/WorkloadProfile/WorkloadProfileGnPei/WorkloadProfileGnPei.inf                                         <GN>
  AgesaModulePkg/WorkloadProfile/WorkloadProfileBaPei/WorkloadProfileBaPei.inf                                         <BA>

  AgesaModulePkg/Usb4/AmdUsb4Pei/AmdUsb4Pei.inf                                                                        <RMB|PHX>

[Components.X64]
  AgesaModulePkg/Debug/AmdIdsDebugPrintDxe/AmdIdsDebugPrintDxe.inf                                                     <ALL>
  AgesaModulePkg/Mem/AmdMemAm4Dxe/AmdMemAm4Dxe.inf                                                                     <AM4|FP5RV|FP6RN|SP4SM|FP6CZN|FT5RV>
  AgesaModulePkg/Mem/AmdMemSspSp3Dxe/AmdMemSspSp3Dxe.inf                                                               <SP3SSP|SP3CP|SP3GN|SP3BA|SP5RS>
  AgesaModulePkg/Mem/AmdMemZpSp3Dxe/AmdMemZpSp3Dxe.inf                                                                 <SP3ZP|SP4ZP>
  AgesaModulePkg/Mem/AmdMemZpSp3r2Dxe/AmdMemZpSp3r2Dxe.inf                                                             <SP3TR>
  AgesaModulePkg/Mem/AmdMemSmbiosDxe/AmdMemSmbiosDxe.inf                                                               <FAMILY15H>
  AgesaModulePkg/Mem/AmdMemSmbiosV2Dxe/AmdMemSmbiosV2Dxe.inf                                                           <FAMILY17H|FAMILY19H>
  AgesaModulePkg/Mem/AmdMemFf3Dxe/AmdMemFf3Dxe.inf                                                                     <FF3>
  AgesaModulePkg/Mem/AmdMemFp7Dxe/AmdMemFp7Dxe.inf                                                                     <FP7|FP7R2>
  AgesaModulePkg/Mem/AmdMemFp8Dxe/AmdMemFp8Dxe.inf                                                                     <FP8>
  AgesaModulePkg/Mem/AmdMemAm5Dxe/AmdMemAm5Dxe.inf                                                                     <AM5>
  AgesaModulePkg/Mem/AmdMemS3CzDxe/AmdMemS3CzDxe.inf {
    <LibraryClasses>
    FabricPstatesLib|AgesaModulePkg/Library/FabricPstatesUnbLib/FabricPstatesUnbLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesXvLib/CcxPstatesXvLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Mem/AmdMemRestoreDxe/MemRestoreDxe.inf                                                                <FAMILY17H|FAMILY19H>
  AgesaModulePkg/Mem/AmdMemRestoreDxeV2/MemRestoreDxe.inf                                                              <RMB>
  AgesaModulePkg/Mem/AmdMemPprSmmDriverSsp/AmdMemPprSmmDriverSsp.inf                                                   <SP3SSP|SP3GN|SP3CP|SP3BA>
  AgesaModulePkg/Psp/ApcbDrv/ApcbZpDxe/ApcbZpDxe.inf{
    <LibraryClasses>
    ApcbZpLib|AgesaModulePkg/Library/ApcbZpLib/ApcbLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Psp/ApcbDrv/ApcbZpSmm/ApcbZpSmm.inf{
    <LibraryClasses>
    ApcbZpLib|AgesaModulePkg/Library/ApcbZpLib/ApcbLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Psp/ApcbDrv/ApcbRvDxe/ApcbRvDxe.inf{
    <LibraryClasses>
    ApcbRvLib|AgesaModulePkg/Library/ApcbRvLib/ApcbLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Psp/ApcbDrv/ApcbRvSmm/ApcbRvSmm.inf{
    <LibraryClasses>
    ApcbRvLib|AgesaModulePkg/Library/ApcbRvLib/ApcbLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Psp/ApcbDrv/ApcbV3Dxe/ApcbV3Dxe.inf                                                                   <SSP|MTS|RN|GN|BA|VMR|CZN|RMB|VN|MR|AR|RS|RPL|PHX|MDN>
  AgesaModulePkg/Psp/ApcbDrv/ApcbV3Smm/ApcbV3Smm.inf                                                                   <SSP|MTS|RN|GN|BA|VMR|CZN|RMB|VN|MR|AR|RS|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdPspDxeV1/AmdPspDxeV1.inf                                                                       <FAMILY15H>
  AgesaModulePkg/Psp/AmdPspSmmV1/AmdPspSmmV1.inf                                                                       <FAMILY15H>
  AgesaModulePkg/Psp/AmdPspP2CmboxV1/AmdPspP2CmboxV1.inf                                                               <FAMILY15H>

  AgesaModulePkg/Psp/AmdPspDxeV2Zp/AmdPspDxeV2.inf                                                                     <AM4SM|SP4SM>
  AgesaModulePkg/Psp/AmdPspDxeV2ZpMcm/AmdPspDxeV2.inf                                                                  <SP3ZP|SP4ZP|SP3TR>
  AgesaModulePkg/Psp/AmdPspDxeV2Rv/AmdPspDxeV2.inf                                                                     <RV>
  AgesaModulePkg/Psp/AmdPspDxeV2Rn/AmdPspDxeV2.inf                                                                     <RN|CZN>
  AgesaModulePkg/Psp/AmdPspDxeV2Rn/AmdDrtmAsl.inf                                                                      <RN>
  AgesaModulePkg/Psp/AmdPspDxeV2Ssp/AmdPspDxeV2.inf                                                                    <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Psp/AmdPspDxeV2Ff3/AmdPspDxeV2.inf                                                                    <VN|MR|AR>
  AgesaModulePkg/Psp/AmdPspDxeV2Ff3/AmdDrtmAsl.inf                                                                     <VN|AR>

  AgesaModulePkg/Psp/AmdPspDxeV2Rmb/AmdPspDxeV2.inf                                                                    <RMB>
  AgesaModulePkg/Psp/AmdPspDxeV2Rmb/AmdDrtmAsl.inf                                                                     <RMB>
  AgesaModulePkg/Psp/AmdPspDxeV2Rpl/AmdPspDxeV2.inf                                                                    <RPL>
  AgesaModulePkg/Psp/AmdPspDxeV2Rpl/AmdDrtmAsl.inf                                                                     <RPL>
  AgesaModulePkg/Psp/AmdPspDxeV2Phx/AmdPspDxeV2.inf                                                                    <PHX>
  AgesaModulePkg/Psp/AmdPspDxeV2Phx/AmdDrtmAsl.inf                                                                     <PHX>
  AgesaModulePkg/Psp/AmdPspDxeV2Mdn/AmdPspDxeV2.inf                                                                    <MDN>
  AgesaModulePkg/Psp/AmdPspDxeV2Mdn/AmdDrtmAsl.inf                                                                     <MDN>
  AgesaModulePkg/Psp/AmdPspDxeV2Rs/AmdPspDxeV2.inf                                                                     <RS>
  AgesaModulePkg/Psp/AmdPspDxeV2Rs/AmdDrtmAsl.inf                                                                      <RS>
  AgesaModulePkg/Psp/AmdPspDxeV2Mi3/AmdPspDxeV2.inf                                                                    <MI3>
  AgesaModulePkg/Psp/AmdPspP2CmboxV2/AmdPspP2CmboxV2.inf                                                               <ZP|RV|RN|CZN|VN|MR|RMB|AR|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdPspP2CmboxV2/AmdPspP2CmboxV2SmmBuffer.inf                                                      <ZP|RV|RN|CZN|VN|MR|RMB|AR|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdPspRomArmorSmm/AmdPspRomArmorSmm.inf                                                           <RN|SP3CP>
  AgesaModulePkg/Psp/AmdPspRomArmor2Smm/AmdPspRomArmor2Smm.inf                                                         <RN|CZN>
  AgesaModulePkg/Psp/AmdPspRomArmor3Smm/AmdPspRomArmor3Smm.inf                                                         <CGL|PHX|MDN>
  AgesaModulePkg/Psp/AmdPspP2CmboxV2Ssp/AmdPspP2CmboxV2.inf                                                            <SSP|MTS|GN|BA|VMR|RS>
  AgesaModulePkg/Psp/AmdPspP2CmboxV2Ssp/AmdPspP2CmboxV2SmmBuffer.inf                                                   <SSP|MTS|GN|BA|VMR|RS>

  AgesaModulePkg/Psp/AmdPspSmmV2/AmdPspSmmV2.inf                                                                       <ZP|RV|RN|CZN|VN|MR|RMB|AR|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdPspSmmV2Ssp/AmdPspSmmV2.inf                                                                    <SSP|MTS|GN|BA|VMR|RS>

  AgesaModulePkg/Psp/AmdPspFtpmDxe/AmdPspFtpmDxe.inf                                                                   <AM4|RV|SP3TR|SP3CP|SP4SM|RN|VMR|CZN|VN|RMB|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdHspFtpmDxe/AmdHspFtpmDxe.inf                                                                   <VN|RMB|RPL|PHX|MDN>
  AgesaModulePkg/Psp/AmdHstiV1/AmdHstiV1.inf                                                                           <FAMILY15H>
  AgesaModulePkg/Psp/AmdHstiV2/AmdHstiV2.inf                                                                           <FAMILY17H|FAMILY19H>

  AgesaModulePkg/Psp/ApobDrv/ApobZpRvDxe/ApobZpRvDxe.inf                                                               <AM4RV|FP5RV|AM4SM|SP4SM|FT5RV>
  AgesaModulePkg/Psp/ApobDrv/ApobZpMcmDxe/ApobZpMcmDxe.inf                                                             <SP3ZP|SP4ZP|SP3TR>
  AgesaModulePkg/Psp/ApobDrv/ApobSspDxe/ApobSspDxe.inf                                                                 <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Psp/ApobDrv/ApobRnDxe/ApobRnDxe.inf                                                                   <RN|CZN>
  AgesaModulePkg/Psp/ApobDrv/ApobFf3Dxe/ApobFf3Dxe.inf                                                                 <VN|MR|AR>
  AgesaModulePkg/Psp/ApobDrv/ApobRmbDxe/ApobRmbDxe.inf                                                                 <RMB>
  AgesaModulePkg/Psp/ApobDrv/ApobRsDxe/ApobRsDxe.inf                                                                   <RS>
  AgesaModulePkg/Psp/ApobDrv/ApobRplDxe/ApobRplDxe.inf                                                                 <RPL>
  AgesaModulePkg/Psp/ApobDrv/ApobPhxDxe/ApobPhxDxe.inf                                                                 <PHX>
  AgesaModulePkg/Psp/ApobDrv/ApobMdnDxe/ApobMdnDxe.inf                                                                 <MDN>
  AgesaModulePkg/Psp/ApobDrv/ApobDummyDxe/ApobDummyDxe.inf                                                             <FAMILY15H>
  AgesaModulePkg/Psp/AmdPspIntrusionDetection/AmdPspIntrusionDetectionDxe.inf                                          <RMB>
  AgesaModulePkg/Psp/AmdPspIntrusionDetection/AmdPspIntrusionDetectionSmm.inf                                          <RMB>

!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmDxe/MpmDxe.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmDxe/AmdMpmAsl.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmKvm/MpmKvm.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmAsf/MpmAsfDxe/MpmAsfDxe.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmBatteryInfo/MpmBatteryInfo.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmBase/MpmPldmBase.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmSmbios/MpmPldmSmbios.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmBcc/MpmPldmBcc.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmPldm/MpmPldmMonitor/MpmPldmSensor.inf
!endif                                                                                                                 <RMB|RPL|PHX>
!ifndef $(AMD_MPM_SUPPORT_DISABLED)
  AgesaModulePkg/Mpm/MpmSerialIoDxe/MpmSerialIoDxe.inf
!endif                                                                                                                 <RMB|RPL|PHX>

  AgesaModulePkg/Fch/Bixby/FchBixbyDxe/FchDxe.inf                                                                      <BXB>
  AgesaModulePkg/Fch/Bixby/FchBixbySmm/FchSmm.inf                                                                      <BXB>
  AgesaModulePkg/Fch/Bixby/FchBixbyDxe/FchBixbyAml.inf                                                                 <BXB>
  AgesaModulePkg/Fch/Bixby/FchBixbyDxe/FchBixbySsdt.inf                                                                <BXB>

  AgesaModulePkg/Ccx/Zen/CcxZenZpDxe/AmdCcxZenZpDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZpLib/CcxResetTablesZpLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZpLib/CcxSetMcaZpLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtZpLib/FabricWdtZpLib.inf
    CcxIdsCustomPstatesLib|AgesaModulePkg/Library/CcxIdsCustomPstateZenLib/CcxIdsCustomPstatesZenLib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZenZpLib/CcxSmbiosZenZpLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <AM4SM|SP4SM|SP3TR>
  AgesaModulePkg/Ccx/Zen/CcxZenZpDxe/AmdCcxZenZpDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZpLib/CcxResetTablesZpLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZpLib/CcxSetMcaZpLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtZpLib/FabricWdtZpLib.inf
    CcxIdsCustomPstatesLib|AgesaModulePkg/Library/CcxIdsCustomPstateZenLib/CcxIdsCustomPstatesZenLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZpLib/CcxResetTablesZpLib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZenZpLib/CcxSmbiosZenZpLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <SP3ZP|SP4ZP>
  AgesaModulePkg/Ccx/Zen/CcxZenRvDxe/AmdCcxZenRvDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesRvLib/CcxResetTablesRvLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    CcxIdsCustomPstatesLib|AgesaModulePkg/Library/CcxIdsCustomPstateZenLib/CcxIdsCustomPstatesZenLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaRvLib/CcxSetMcaRvLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesRvLib/CcxResetTablesRvLib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZenRvLib/CcxSmbiosZenRvLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Ccx/Vh/Dxe/AmdCcxVhDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhLib/CcxResetTablesVhLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosVhLib/CcxSmbiosVhLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <SSP|MTS>
  AgesaModulePkg/Ccx/Vh/Dxe/Rn/AmdCcxVhRnDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhLib/CcxResetTablesVhLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosVhLib/CcxSmbiosVhLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <RN>
  AgesaModulePkg/Ccx/Vh/Dxe/Ff3/AmdCcxVhFf3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhV2Lib/CcxResetTablesVhV2Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosVhLib/CcxSmbiosVhLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Ccx/Vh/Dxe/Mdn/AmdCcxVhMdnDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesVhV2Lib/CcxResetTablesVhV2Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosVhLib/CcxSmbiosVhLib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesVhLib/CcxRolesVhLib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Ccx/Zen3/Dxe/AmdCcxZen3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen3Lib/CcxSmbiosZen3Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <VMR|CGL>
  AgesaModulePkg/Ccx/Zen3/Dxe/Czn/AmdCcxZen3CznDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen3Lib/CcxSmbiosZen3Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <CZN>
  AgesaModulePkg/Ccx/Zen3/Dxe/Rmb/AmdCcxZen3RmbDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen3Lib/CcxSmbiosZen3Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Ccx/Zen3/Dxe/AmdCcxZen3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf3Lib/FabricWdtDf3Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen3Lib/CcxSmbiosZen3Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Ccx/Zen3/Dxe/AmdCcxZen3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen3Lib/CcxResetTablesZen3Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtBaLib/FabricWdtBaLib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen3Lib/CcxSmbiosZen3Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen3Lib/CcxRolesZen3Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Ccx/Zen4/Dxe/AmdCcxZen4Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen4Lib/CcxResetTablesZen4Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen4Lib/CcxSetMcaZen4Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf4Lib/FabricWdtDf4Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen4Lib/CcxSmbiosZen4Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen4Lib/CcxRolesZen4Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen4Lib/CcxPstatesZen4Lib.inf
  }                                                                                                                    <RS|RPL|PHX|STP>
  AgesaModulePkg/Ccx/Zen4/Dxe/Mi3/AmdCcxZen4Mi3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    CcxResetTablesLib|AgesaModulePkg/Library/CcxResetTablesZen4Lib/CcxResetTablesZen4Lib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen4Lib/CcxSetMcaZen4Lib.inf
    FabricWdtLib|AgesaModulePkg/Library/FabricWdtDf4Lib/FabricWdtDf4Lib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosZen4Lib/CcxSmbiosZen4Lib.inf
    CcxRolesLib|AgesaModulePkg/Library/CcxRolesZen4Lib/CcxRolesZen4Lib.inf
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen4Lib/CcxPstatesZen4Lib.inf
  }                                                                                                                    <MI3>
  AgesaModulePkg/Ccx/Xv/CcxXvDxe/AmdCcxXvDxe.inf {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesXvLib/CcxPstatesXvLib.inf
    CcxSmbiosLib|AgesaModulePkg/Library/CcxSmbiosXvLib/CcxSmbiosXvLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/DxeXv/AmdTableHookDxeXvLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Ccx/Xv/CcxXvSmm/AmdCcxXvSmm.inf  {
    <LibraryClasses>
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/DxeXv/AmdTableHookDxeXvLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Ccx/Zen/CcxZenZpSmm/AmdCcxZenZpSmm.inf {
    <LibraryClasses>
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZpLib/CcxSetMcaZpLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Ccx/Zen/CcxZenRvSmm/AmdCcxZenRvSmm.inf {
    <LibraryClasses>
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaRvLib/CcxSetMcaRvLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Ccx/Vh/Smm/AmdCcxVhSmm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
  }                                                                                                                    <SSP|MTS>
  AgesaModulePkg/Ccx/Vh/Smm/Rn/AmdCcxVhRnSmm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
  }                                                                                                                    <RN>
  AgesaModulePkg/Ccx/Vh/Smm/Ff3/AmdCcxVhFf3Smm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Ccx/Vh/Smm/Mdn/AmdCcxVhMdnSmm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaVhLib/CcxSetMcaVhLib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Ccx/Zen3/Smm/AmdCcxZen3Smm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
  }                                                                                                                    <VMR|CZN|RMB|CGL>
  AgesaModulePkg/Ccx/Zen3/Smm/AmdCcxZen3Smm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen3Lib/CcxSetMcaZen3Lib.inf
  }                                                                                                                    <GN|BA>
  AgesaModulePkg/Ccx/Zen4/Smm/AmdCcxZen4Smm.inf {
    <LibraryClasses>
    CcxSetMcaLib|AgesaModulePkg/Library/CcxSetMcaZen4Lib/CcxSetMcaZen4Lib.inf
  }                                                                                                                    <RS|RPL|PHX|MI3|STP>
  AgesaModulePkg/Fabric/ZP/FabricZpDxe/AmdFabricZpDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Fabric/ZP/FabricZpSmm/AmdFabricZpSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Fabric/SSP/FabricSspDxe/AmdFabricSspDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
  }                                                                                                                    <SSP|MTS|VMR|CGL>
  AgesaModulePkg/Fabric/SSP/FabricSspDxe/AmdFabricSspDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Fabric/BA/FabricBaDxe/AmdFabricBaDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
    FabricIdsHookSspLibDxe|AgesaModulePkg/Library/FabricIdsHookBaLib/Dxe/FabricIdsHookBaLibDxe.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Fabric/RS/FabricRsDxe/AmdFabricRsDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRsLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
  }                                                                                                                    <RS|STP>
  AgesaModulePkg/Fabric/MI3/FabricMi3Dxe/AmdFabricMi3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
  }                                                                                                                    <MI3>
  AgesaModulePkg/Fabric/SSP/FabricSspSmm/AmdFabricSspSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <SSP|MTS|GN|VMR|CGL>
  AgesaModulePkg/Fabric/BA/FabricBaSmm/AmdFabricBaSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Fabric/RS/FabricRsSmm/AmdFabricRsSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <RS|STP>
  AgesaModulePkg/Fabric/MI3/FabricMi3Smm/AmdFabricMi3Smm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <MI3>
  AgesaModulePkg/Fabric/RN/FabricRnDxe/AmdFabricRnDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Fabric/FF3/FabricFf3Dxe/AmdFabricFf3Dxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Fabric/RMB/FabricRmbDxe/AmdFabricRmbDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Fabric/MDN/FabricMdnDxe/AmdFabricMdnDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3MdnLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Fabric/PHX/FabricPhxDxe/AmdFabricPhxDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerPhxLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccPhxLib/FabricRegisterAccPhxLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyPhxLib/BaseFabricTopologyPhxLib.inf
  }                                                                                                                    <PHX>
  AgesaModulePkg/Fabric/RPL/FabricRplDxe/AmdFabricRplDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Fabric/RN/FabricRnSmm/AmdFabricRnSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Fabric/FF3/FabricFf3Smm/AmdFabricFf3Smm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <FF3>
  AgesaModulePkg/Fabric/RMB/FabricRmbSmm/AmdFabricRmbSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                     <RMB>
  AgesaModulePkg/Fabric/MDN/FabricMdnSmm/AmdFabricMdnSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                     <MDN>
  AgesaModulePkg/Fabric/PHX/FabricPhxSmm/AmdFabricPhxSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                     <PHX>
  AgesaModulePkg/Fabric/RPL/FabricRplSmm/AmdFabricRplSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                     <RPL>
  AgesaModulePkg/Fabric/RV/FabricRvDxe/AmdFabricRvDxe.inf {
    <LibraryClasses>
    IdsLib|AgesaModulePkg/Library/IdsNonUefiLib/IdsNonUefiLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Fabric/RV/FabricRvSmm/AmdFabricRvSmm.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Fabric/XV/UnbXvDxe/AmdUnbXvDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerUnbLib/FabricResourceManagerUnbLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    AmdTableHookLib|AgesaModulePkg/Library/AmdTableLib/DxeXv/AmdTableHookDxeXvLib.inf
  }                                                                                                                    <FAMILY15H>
  AgesaModulePkg/Soc/AmdSocAm4VmrDxe/AmdSocAm4VmrDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesVmrLib/DxeSocZen3ServicesVmrLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4VMR>
  AgesaModulePkg/Soc/AmdSocAm4MtsDxe/AmdSocAm4MtsDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesMtsLib/DxeSocValhallaServicesMtsLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4MTS>
  AgesaModulePkg/Soc/AmdSocAm4SmDxe/AmdSocAm4SmDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4SM>
  AgesaModulePkg/Soc/AmdSocSp4SmDxe/AmdSocSp4SmDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP4SM>
  AgesaModulePkg/Soc/AmdSocAm4BrDxe/AmdSocAm4BrDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdUnbLib/BaseSocketLogicalIdUnbLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatBrLib/BaseSocLogicalIdXlatBrLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyUnbLib/BaseFabricTopologyUnbLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4BR>
  AgesaModulePkg/Soc/AmdSocFp5RvDxe/AmdSocFp5RvDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRvDieLib/BaseSocketLogicalIdRvDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
  }                                                                                                                    <FP5RV>
  AgesaModulePkg/Soc/AmdSocFp6RnDxe/AmdSocFp6RnDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRnDieLib/BaseSocketLogicalIdRnDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRnDieLib/BaseSocLogicalIdXlatRnDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesRnLib/DxeSocValhallaServicesRnLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <FP6RN>
  AgesaModulePkg/Soc/AmdSocFf3VnDxe/AmdSocFf3VnDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdFf3DieLib/BaseSocketLogicalIdFf3DieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatFf3DieLib/BaseSocLogicalIdXlatFf3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesFf3Lib/DxeSocValhallaServicesFf3Lib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <FF3VN|FF3AR>
  AgesaModulePkg/Soc/AmdSocFf3MrDxe/AmdSocFf3MrDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdFf3DieLib/BaseSocketLogicalIdFf3DieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatFf3DieLib/BaseSocLogicalIdXlatFf3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyFf3Lib/BaseFabricTopologyFf3Lib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesFf3Lib/DxeSocValhallaServicesFf3Lib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <FF3MR>
  AgesaModulePkg/Soc/AmdSocAm4RnDxe/AmdSocAm4RnDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRnDieLib/BaseSocketLogicalIdRnDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRnDieLib/BaseSocLogicalIdXlatRnDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesRnLib/DxeSocValhallaServicesRnLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4RN>
  AgesaModulePkg/Soc/AmdSocAm4CznDxe/AmdSocAm4CznDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesCznLib/DxeSocZen3ServicesCznLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4CZN>
  AgesaModulePkg/Soc/AmdSocFp6CznDxe/AmdSocFp6CznDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesCznLib/DxeSocZen3ServicesCznLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <FP6CZN>
  AgesaModulePkg/Soc/AmdSocAm5RmbDxe/AmdSocAm5RmbDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesRmbLib/DxeSocZen3ServicesRmbLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM5RMB>
  AgesaModulePkg/Soc/AmdSocFp7RmbDxe/AmdSocFp7RmbDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesRmbLib/DxeSocZen3ServicesRmbLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <FP7RMB>
  AgesaModulePkg/Soc/AmdSocFp8PhxDxe/AmdSocFp8PhxDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccPhxLib/FabricRegisterAccPhxLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
    DxeSocZen4ServicesLib|AgesaModulePkg/Library/DxeSocZen4ServicesPhxLib/DxeSocZen4ServicesPhxLib.inf
  }                                                                                                                    <FP8PHX>
    AgesaModulePkg/Soc/AmdSocFt6MdnDxe/AmdSocFt6MdnDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesMdnLib/DxeSocValhallaServicesMdnLib.inf
  }                                                                                                                    <FT6MDN>
  AgesaModulePkg/Soc/AmdSocFp7r2RmbDxe/AmdSocFp7r2RmbDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRmbDieLib/BaseSocketLogicalIdRmbDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesRmbLib/DxeSocZen3ServicesRmbLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <FP7R2RMB>
  AgesaModulePkg/Soc/AmdSocSp3GnDxe/AmdSocSp3GnDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesGnLib/DxeSocZen3ServicesGnLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3GN>
  AgesaModulePkg/Soc/AmdSocSp3r4BaDxe/AmdSocSp3r4BaDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesGnLib/DxeSocZen3ServicesGnLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3BA>
  AgesaModulePkg/Soc/AmdSocSp5RsDxe/AmdSocSp5RsDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    DxeSocZen4ServicesLib|AgesaModulePkg/Library/DxeSocZen4ServicesRsLib/DxeSocZen4ServicesRsLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP5RS>
  AgesaModulePkg/Soc/AmdSocSh5Mi3Dxe/AmdSocSh5Mi3Dxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMi3Lib/BaseFabricTopologyMi3Lib.inf
    DxeSocZen4ServicesLib|AgesaModulePkg/Library/DxeSocZen4ServicesRsLib/DxeSocZen4ServicesRsLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SH5Mi3>
  AgesaModulePkg/Soc/AmdSocSp3RmDxe/AmdSocSp3RmDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesSspLib/DxeSocValhallaServicesSspLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3SSP>
  AgesaModulePkg/Soc/AmdSocSp3ZpDxe/AmdSocSp3ZpDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3ZP>
  AgesaModulePkg/Soc/AmdSocSp4ZpDxe/AmdSocSp4ZpDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP4ZP>
  AgesaModulePkg/Soc/AmdSocAm4RvDxe/AmdSocAm4RvDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRvDieLib/BaseSocketLogicalIdRvDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    DxeSocZenServicesLib|AgesaModulePkg/Library/DxeSocZenServicesLib/DxeSocZenServicesLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM4RV>
  AgesaModulePkg/Soc/AmdSocSp3r2TrDxe/AmdSocSp3r2TrDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdZpDieLib/BaseSocketLogicalIdZpDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZpDieLib/BaseSocLogicalIdXlatZpDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3TR>
  AgesaModulePkg/Soc/AmdSocSp3r3CpDxe/AmdSocSp3r3CpDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocValhallaServicesLib|AgesaModulePkg/Library/DxeSocValhallaServicesSspLib/DxeSocValhallaServicesSspLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3CP>
  AgesaModulePkg/Soc/AmdSocFt5RvDxe/AmdSocFt5RvDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdRvDieLib/BaseSocketLogicalIdRvDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatRvDieLib/BaseSocLogicalIdXlatRvDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
  }                                                                                                                    <FT5RV>
  AgesaModulePkg/Soc/AmdSocSp3r3CglDxe/AmdSocSp3r3CglDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen3DieLib/BaseSocLogicalIdXlatZen3DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    DxeSocZen3ServicesLib|AgesaModulePkg/Library/DxeSocZen3ServicesGnLib/DxeSocZen3ServicesGnLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP3CGL>
  AgesaModulePkg/Soc/AmdSocAm5RplDxe/AmdSocAm5RplDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatSspDieLib/BaseSocLogicalIdXlatSspDieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    DxeSocZen4ServicesLib|AgesaModulePkg/Library/DxeSocZen4ServicesRplLib/DxeSocZen4ServicesRplLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <AM5RPL>
  AgesaModulePkg/Soc/AmdSocSp6StpDxe/AmdSocSp6StpDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    DxeSocZen4ServicesLib|AgesaModulePkg/Library/DxeSocZen4ServicesRsLib/DxeSocZen4ServicesRsLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <SP6STP>
  AgesaModulePkg/Soc/AmdSocTr5StpDxe/AmdSocTr5StpDxe.inf {
    <LibraryClasses>
    AmdS3SaveLib|AgesaModulePkg/Library/AmdS3SaveLib/WOS3Save/AmdWOS3SaveLib.inf
    BaseSocketLogicalIdLib|AgesaModulePkg/Library/BaseSocketLogicalIdSspDieLib/BaseSocketLogicalIdSspDieLib.inf
    BaseSocLogicalIdXlatLib|AgesaModulePkg/Library/BaseSocLogicalIdXlatZen4DieLib/BaseSocLogicalIdXlatZen4DieLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRsLib/BaseFabricTopologyRsLib.inf
    DxeSocZen4ServicesLib|AgesaModulePkg/Library/DxeSocZen4ServicesRsLib/DxeSocZen4ServicesRsLib.inf
    AmdIdsHookExtLib|AgesaModulePkg/Library/AmdIdsExtLibNull/AmdIdsHookExtLibNull.inf
  }                                                                                                                    <TR5STP>

  AgesaModulePkg/ErrorLog/AmdErrorLogDxe/AmdErrorLogDxe.inf                                                            <ALL>
  AgesaModulePkg/ErrorLog/AmdErrorLogDisplayDxe/AmdErrorLogDisplayDxe.inf                                              <VMR|CZN|RMB|CGL>
  AgesaModulePkg/ErrorLog/AmdErrorLogDisplayGnDxe/AmdErrorLogDisplayGnDxe.inf                                          <GN|BA>
  AgesaModulePkg/ErrorLog/AmdErrorLogDisplayRsDxe/AmdErrorLogDisplayRsDxe.inf                                          <RS>

  ## Gnb Dxe Drivers
  AgesaModulePkg/Nbio/Common/CxlManagerDxe/CxlManagerDxe.inf                                                           <RS|MI3>
  AgesaModulePkg/Nbio/RS/DXE/NbioDxeRs.inf                                                                             <RS>
  AgesaModulePkg/Nbio/MI300/DXE/NbioDxeMI300.inf                                                                       <MI3>
  AgesaModulePkg/Nbio/NbioBase/AmdNbioBaseCZDxe/AmdNbioBaseCZDxe.inf {
      <LibraryClasses>
    CcxBtcLib|AgesaModulePkg/Library/CcxBtcLib/CcxXvBtcLib.inf
  }                                                                                                                    <FAMILY15H>

  AgesaModulePkg/Nbio/PHX/DXE/NbioDxePhx.inf                                                                            <PHX>

  AgesaModulePkg/Nbio/NbioBase/RN/DXE/AmdNbioBaseRNDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Nbio/NbioBase/RMB/DXE/AmdNbioBaseRMBDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Nbio/NbioBase/RPL/DXE/AmdNbioBaseRPLDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Nbio/NbioBase/MDN/DXE/AmdNbioBaseMDNDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyMdnLib/BaseFabricTopologyMdnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Nbio/NbioBase/AmdNbioBaseRVDxe/AmdNbioBaseRVDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Nbio/NbioBase/AmdNbioBaseZPDxe/AmdNbioBaseZPDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccSoc15Lib/FabricRegisterAccSoc15Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyZpLib/BaseFabricTopologyZpLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Nbio/SMU/AmdNbioSmuV9Dxe/AmdNbioSmuV9Dxe.inf {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Nbio/SMU/AmdNbioSmuV10Dxe/AmdNbioSmuV10Dxe.inf {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZenZpLib/CcxPstatesZenZpLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Nbio/SMU/RN/DXE/SmuV12DxeRN.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <RN>
  AgesaModulePkg/Nbio/SMU/RMB/DXE/SmuV13DxeRMB.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Nbio/SMU/RPL/DXE/SmuV13DxeRPL.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Nbio/SMU/MDN/DXE/SmuV13DxeMDN.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Nbio/SMU/CZN/DXE/SmuV12DxeCZN.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <CZN>
  AgesaModulePkg/Nbio/PCIe/AmdNbioPcieZPDxe/AmdNbioPcieZPDxe.inf                                                       <ZP>
  AgesaModulePkg/Nbio/PCIe/AmdNbioPcieRVDxe/AmdNbioPcieRVDxe.inf                                                       <RV>
  AgesaModulePkg/Nbio/PCIe/RN/DXE/AmdNbioPcieRNDxe.inf                                                                 <RN|CZN>
  AgesaModulePkg/Nbio/PCIe/RMB/DXE/AmdNbioPcieRMBDxe.inf                                                               <RMB>
  AgesaModulePkg/Nbio/PCIe/RPL/DXE/AmdNbioPcieRPLDxe.inf                                                               <RPL>
  AgesaModulePkg/Nbio/PCIe/MDN/DXE/AmdNbioPcieMDNDxe.inf                                                               <MDN>
  AgesaModulePkg/Nbio/IOMMU/AmdNbioIOMMUZPDxe/AmdNbioIOMMUZPDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Library/NbioIommuIvrsLib/NbioIommuIvrsLib.inf
  }                                                                                                                    <ZP>
  AgesaModulePkg/Nbio/IOMMU/RN/DXE/AmdNbioIOMMURNDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRnLib/BaseFabricTopologyRnLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <RN|CZN>
  AgesaModulePkg/Nbio/IOMMU/MDN/DXE/AmdNbioIOMMUMDNDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <MDN>
  AgesaModulePkg/Nbio/IOMMU/RMB/DXE/AmdNbioIOMMURMBDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRmbLib/BaseFabricTopologyRmbLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3RmbLib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <RMB>
  AgesaModulePkg/Nbio/IOMMU/RPL/DXE/AmdNbioIOMMURPLDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyRplLib/BaseFabricTopologyRplLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf4Lib/FabricRegisterAccDf4Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerRplLib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <RPL>
  AgesaModulePkg/Nbio/IOMMU/AmdNbioIOMMURVDxe/AmdNbioIOMMURVDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf1Lib/FabricResourceManager1Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Library/NbioIommuIvrsLib/NbioIommuIvrsLib.inf
  }                                                                                                                    <RV>
  AgesaModulePkg/Nbio/GFX/AmdNbioGfxRVDxe/AmdNbioGfxRVDxe.inf                                                          <RV>
  AgesaModulePkg/Nbio/GFX/RN/DXE/AmdNbioGfxRNDxe.inf                                                                   <RN|CZN>
  AgesaModulePkg/Nbio/GFX/RMB/DXE/AmdNbioGfxRMBDxe.inf                                                                 <RMB>
  AgesaModulePkg/Nbio/GFX/RPL/DXE/AmdNbioGfxRPLDxe.inf                                                                 <RPL>
  AgesaModulePkg/Nbio/GFX/MDN/DXE/AmdNbioGfxMDNDxe.inf                                                                 <MDN>
  AgesaModulePkg/Nbio/NbioALib/SSP/DXE/AmdNbioAlibDxe.inf                                                              <SSP|MTS|GN|BA>
  AgesaModulePkg/Nbio/NbioALib/VMR/DXE/AmdNbioAlibDxe.inf                                                              <VMR|CGL>
  AgesaModulePkg/Nbio/NbioALib/AmdNbioAlibZPDxe/AmdNbioAlibZPDxe.inf                                                   <ZP>
  AgesaModulePkg/Nbio/NbioALib/AmdNbioAlibRVDxe/AmdNbioAlibRVDxe.inf                                                   <RV>
  AgesaModulePkg/Nbio/NbioALib/RN/DXE/AmdNbioAlibRNDxe.inf                                                             <RN|CZN>
  AgesaModulePkg/Nbio/NbioALib/RMB/DXE/AmdNbioAlibRMBDxe.inf                                                           <RMB>
  AgesaModulePkg/Nbio/NbioALib/RPL/DXE/AmdNbioAlibRPLDxe.inf                                                           <RPL>
  AgesaModulePkg/Nbio/NbioALib/MDN/DXE/AmdNbioAlibMDNDxe.inf                                                           <MDN>
  AgesaModulePkg/Nbio/NbioBase/SSP/DXE/AmdNbioBaseSspDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
  }                                                                                                                    <SSP|MTS|VMR>
  AgesaModulePkg/Nbio/NbioBase/GN/DXE/AmdNbioBaseGnDxe.inf {
    <LibraryClasses>
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Nbio/NbioBase/BA/DXE/AmdNbioBaseBaDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Nbio/SMU/SSP/DXE/SmuV11DxeSsp.inf   {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <SSP|MTS>
  AgesaModulePkg/Nbio/SMU/GN/DXE/SmuV11DxeGn.inf   {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Nbio/SMU/BA/DXE/SmuV11DxeBa.inf   {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Nbio/SMU/VMR/DXE/SmuV11DxeVmr.inf   {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesZen3Lib/CcxPstatesZen3Lib.inf
  }                                                                                                                    <VMR|CGL>
  AgesaModulePkg/Nbio/PCIe/SSP/DXE/AmdNbioPcieSspDxe.inf                                                               <SSP|MTS|VMR>
  AgesaModulePkg/Nbio/PCIe/GN/DXE/AmdNbioPcieGnDxe.inf                                                                 <GN>
  AgesaModulePkg/Nbio/PCIe/BA/DXE/AmdNbioPcieBaDxe.inf                                                                 <BA>
  AgesaModulePkg/Nbio/IOMMU/SSP/DXE/AmdNbioIOMMUSspDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <SSP|MTS|VMR>
  AgesaModulePkg/Nbio/IOMMU/GN/DXE/AmdNbioIOMMUGnDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologySspLib/BaseFabricTopologySspLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerDf3Lib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <GN>
  AgesaModulePkg/Nbio/IOMMU/BA/DXE/AmdNbioIOMMUBaDxe.inf {
    <LibraryClasses>
    BaseFabricTopologyLib|AgesaModulePkg/Library/BaseFabricTopologyBaLib/BaseFabricTopologyBaLib.inf
    MI200FabricLib|AgesaModulePkg/Library/MI200FabricLib/MI200FabricLib.inf
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccBaLib/FabricRegisterAccBaLib.inf
    FabricResourceManagerLib|AgesaModulePkg/Library/FabricResourceManagerBaLib/FabricResourceManager3Lib.inf
    NbioIommuIvrsLib|AgesaModulePkg/Nbio/Library/IvrsLibV2/IvrsLibV2.inf
  }                                                                                                                    <BA>
  AgesaModulePkg/Nbio/NbioBase/FF3/DXE/AmdNbioBaseFF3Dxe.inf                                                           <FF3>
  AgesaModulePkg/Nbio/SMU/FF3/DXE/SmuV13DxeFF3.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <MR|VN>
  AgesaModulePkg/Nbio/SMU/AR/DXE/SmuV13DxeFF3.inf  {
    <LibraryClasses>
    CcxPstatesLib|AgesaModulePkg/Library/CcxPstatesVhLib/CcxPstatesVhLib.inf
  }                                                                                                                    <AR>
  AgesaModulePkg/Nbio/PCIe/FF3/DXE/AmdNbioPcieFF3Dxe.inf                                                               <FF3>
  AgesaModulePkg/Nbio/IOMMU/FF3/DXE/AmdNbioIOMMUFF3Dxe.inf                                                             <FF3>
  AgesaModulePkg/Nbio/GFX/FF3/DXE/AmdNbioGfxFF3Dxe.inf                                                                 <FF3>
  AgesaModulePkg/Nbio/NbioALib/VN/DXE/AmdNbioAlibVNDxe.inf                                                             <VN|AR>

  ## Fch Dxe Drivers
  AgesaModulePkg/Fch/Taishan/FchTaishanDxe/FchDxe.inf                                                                  <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanSmm/FchSmm.inf                                                                  <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanSmbusDxe/SmbusLight.inf                                                         <ZP>
  AgesaModulePkg/Fch/Common/FchEspiCmdDxe/FchEspiCmdDxe.inf                                                            <BRH>
  AgesaModulePkg/Fch/Common/FchEspiCmdSmm/FchEspiCmdSmm.inf   
  AgesaModulePkg/Fch/Taishan/FchTaishanCf9ResetDxe/Cf9Reset.inf                                                        <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanCf9ResetSmm/Cf9ResetSmm.inf                                                     <ZP>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Taishan/FchTaishanLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanSmmControlDxe/SmmControl.inf                                                    <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanSmmDispatcher/FchSmmDispatcher.inf                                              <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanMultiFchDxe/FchMultiFchDxe.inf                                                  <ZP>
  AgesaModulePkg/Fch/Taishan/FchTaishanMultiFchSmm/FchMultiFchSmm.inf                                                  <SP3ZP|SP4ZP|SP3TR>
  AgesaModulePkg/Fch/Taishan/FchTaishanDxe/FchTaishanSsdt.inf                                                          <AM4SM|SP4SM>
  AgesaModulePkg/Fch/Taishan/FchTaishanDxe/FchTaishanAml.inf                                                           <AM4SM|SP4SM>
  AgesaModulePkg/Fch/Huashan/FchHuashanDxe/FchDxe.inf                                                                  <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanSmm/FchSmm.inf                                                                  <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanDxe/FchHuashanDsdt.inf                                                          <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanDxe/FchHuashanSsdt.inf                                                          <VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanDxe/FchHuashanAml.inf                                                           <VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanSmbusDxe/SmbusLight.inf                                                         <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanCf9ResetDxe/Cf9Reset.inf                                                        <SSP|MTS|GN|BA|VMR>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Huashan/FchHuashanLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanSmmControlDxe/SmmControl.inf                                                    <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanSmmDispatcher/FchSmmDiagDispatcher.inf                                          <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanSmmDispatcher/FchSmmDispatcher.inf                                              <SSP|MTS|GN|BA|VMR>
  AgesaModulePkg/Fch/Huashan/FchHuashanMultiFchDxe/FchMultiFchDxe.inf {
    <LibraryClasses>
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
  }                                                                                                                    <SSP|GN>
  AgesaModulePkg/Fch/Huashan/FchHuashanMultiFchSmm/FchMultiFchSmm.inf {
    <LibraryClasses>
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccDf3Lib/FabricRegisterAccDf3Lib.inf
  }                                                                                                                    <SSP|GN>

  AgesaModulePkg/Fch/Songshan/FchSongshanDxe/FchDxe.inf                                                                <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanSmm/FchSmm.inf                                                                <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanDxe/FchSongshanDsdt.inf                                                       <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanDxe/FchSongshanSsdt.inf                                                       <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanDxe/FchSongshanAml.inf                                                        <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanSmbusDxe/SmbusLight.inf                                                       <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanI3cDxe/I3cDxe.inf                                                             <RS>
!ifndef $(AMD_RESET_DXE_DRIVER_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Songshan/FchSongshanCf9ResetDxe/Cf9Reset.inf
!endif                                                                                                                 <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanSmmControlDxe/SmmControl.inf                                                  <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanSmmDispatcher/FchSmmDiagDispatcher.inf                                        <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanSmmDispatcher/FchSmmDispatcher.inf                                            <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanMultiFchDxe/FchMultiFchDxe.inf {
    <LibraryClasses>
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
  }                                                                                                                    <RS>
  AgesaModulePkg/Fch/Songshan/FchSongshanMultiFchSmm/FchMultiFchSmm.inf {
    <LibraryClasses>
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
  }                                                                                                                    <RS>

  AgesaModulePkg/Fch/Qiantang/FchQiantangDxe/FchDxe.inf                                                                <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangSmm/FchSmm.inf                                                                <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangDxe/FchQiantangDsdt.inf                                                       <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangDxe/FchQiantangSsdt.inf                                                       <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangDxe/FchQiantangAml.inf                                                        <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangSmbusDxe/SmbusLight.inf                                                       <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangCf9ResetDxe/Cf9Reset.inf                                                      <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangSmmControlDxe/SmmControl.inf                                                  <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangSmmDispatcher/FchSmmDiagDispatcher.inf                                        <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangSmmDispatcher/FchSmmDispatcher.inf                                            <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangMultiFchDxe/FchMultiFchDxe.inf {
    <LibraryClasses>
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
  }                                                                                                                    <MI3>
  AgesaModulePkg/Fch/Qiantang/FchQiantangMultiFchSmm/FchMultiFchSmm.inf {
    <LibraryClasses>
    FabricRegisterAccLib|AgesaModulePkg/Library/FabricRegisterAccRsLib/FabricRegisterAccRsLib.inf
  }                                                                                                                    <MI3>

  AgesaModulePkg/Nbio/BIXBY/DXE/BixbyDxe.inf                                                                           <BXB>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneDxe/FchDxe.inf                                                              <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneSmm/FchSmm.inf                                                              <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneSmbusDxe/SmbusLight.inf                                                     <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneCf9ResetDxe/Cf9Reset.inf                                                    <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneCf9ResetSmm/Cf9ResetSmm.inf                                                 <RV>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Sandstone/FchSandstoneLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneSmmControlDxe/SmmControl.inf                                                <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneSmmDispatcher/FchSmmDispatcher.inf                                          <RV>
  AgesaModulePkg/Fch/Sandstone/FchSandstoneComplement/FchSSSataD3Cold/FchSSSataD3ColdSmm.inf                           <RV>
  AgesaModulePkg/Fch/Shasta/FchShastaDxe/FchDxe.inf                                                                    <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaDxe/FchShastaSsdt.inf                                                             <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaDxe/FchShastaAml.inf                                                              <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaSmm/FchSmm.inf                                                                    <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaSmbusDxe/SmbusLight.inf                                                           <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaCf9ResetDxe/Cf9Reset.inf                                                          <RN|CZN>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Shasta/FchShastaLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaSmmControlDxe/SmmControl.inf                                                      <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaSmmDispatcher/FchSmmDiagDispatcher.inf                                            <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaSmmDispatcher/FchSmmDispatcher.inf                                                <RN|CZN>
  AgesaModulePkg/Fch/Shasta/FchShastaComplement/FchSHSataD3Cold/FchSHSataD3ColdSmm.inf                                 <RN|CZN>
  AgesaModulePkg/Fch/Shang/FchShangDxe/FchDxe.inf                                                                      <FF3>
  AgesaModulePkg/Fch/Shang/FchShangDxe/FchShangSsdt.inf                                                                <FF3>
  AgesaModulePkg/Fch/Shang/FchShangDxe/FchShangAml.inf                                                                 <FF3>
  AgesaModulePkg/Fch/Shang/FchShangSmm/FchSmm.inf                                                                      <FF3>
  AgesaModulePkg/Fch/Shang/FchShangSmbusDxe/SmbusLight.inf                                                             <FF3>
  AgesaModulePkg/Fch/Shang/FchShangCf9ResetDxe/Cf9Reset.inf                                                            <FF3>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Shang/FchShangLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <FF3>
  AgesaModulePkg/Fch/Shang/FchShangSmmControlDxe/SmmControl.inf                                                        <FF3>
  AgesaModulePkg/Fch/Shang/FchShangSmmDispatcher/FchSmmDiagDispatcher.inf                                              <FF3>
  AgesaModulePkg/Fch/Shang/FchShangSmmDispatcher/FchSmmDispatcher.inf                                                  <FF3>
  AgesaModulePkg/Fch/Shang/FchShangComplement/FchSGSataD3Cold/FchSGSataD3ColdSmm.inf                                   <FF3>
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchDxe.inf                                                                      <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchKeithSsdt.inf                                                                <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchKeithDsdt.inf                                                                <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithDxe/FchKeithAml.inf                                                                 <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithSmm/FchSmm.inf                                                                      <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithSmbusDxe/SmbusLight.inf                                                             <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithCf9ResetDxe/Cf9Reset.inf                                                            <RMB>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Keith/FchKeithLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithSmmControlDxe/SmmControl.inf                                                        <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithSmmDispatcher/FchSmmDiagDispatcher.inf                                              <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithSmmDispatcher/FchSmmDispatcher.inf                                                  <RMB>
  AgesaModulePkg/Fch/Keith/FchKeithComplement/FchKTSataD3Cold/FchKTSataD3ColdSmm.inf                                   <RMB>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiDxe/FchDxe.inf                                                                    <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiDxe/FchYuntaiSsdt.inf                                                             <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiDxe/FchYuntaiDsdt.inf                                                             <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiDxe/FchYuntaiAml.inf                                                              <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiSmm/FchSmm.inf                                                                    <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiSmbusDxe/SmbusLight.inf                                                           <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiCf9ResetDxe/Cf9Reset.inf                                                          <RPL>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Yuntai/FchYuntaiLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiSmmControlDxe/SmmControl.inf                                                      <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiSmmDispatcher/FchSmmDiagDispatcher.inf                                            <RPL>
  AgesaModulePkg/Fch/Yuntai/FchYuntaiSmmDispatcher/FchSmmDispatcher.inf                                                <RPL>
  AgesaModulePkg/Fch/Tacoma/FchTacomaDxe/FchDxe.inf                                                                    <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaDxe/FchTacomaSsdt.inf                                                             <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaDxe/FchTacomaDsdt.inf                                                             <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaDxe/FchTacomaAml.inf                                                              <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaSmm/FchSmm.inf                                                                    <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaSmbusDxe/SmbusLight.inf                                                           <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaCf9ResetDxe/Cf9Reset.inf                                                          <PHX>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Tacoma/FchTacomaLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaSmmControlDxe/SmmControl.inf                                                      <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaSmmDispatcher/FchSmmDiagDispatcher.inf                                            <PHX>
  AgesaModulePkg/Fch/Tacoma/FchTacomaSmmDispatcher/FchSmmDispatcher.inf                                                <PHX>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnDxe/FchDxe.inf                                                                <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnDxe/FchKeithMdnSsdt.inf                                                       <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnDxe/FchKeithMdnDsdt.inf                                                       <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnDxe/FchKeithMdnAml.inf                                                        <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnSmm/FchSmm.inf                                                                <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnSmbusDxe/SmbusLight.inf                                                       <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnCf9ResetDxe/Cf9Reset.inf                                                      <MDN>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnSmmControlDxe/SmmControl.inf                                                  <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnSmmDispatcher/FchSmmDiagDispatcher.inf                                        <MDN>
  AgesaModulePkg/Fch/KeithMdn/FchKeithMdnSmmDispatcher/FchSmmDispatcher.inf                                            <MDN>
  AgesaModulePkg/Fch/Kern/FchKernDxe/FchDxe.inf                                                                        <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernSmm/FchSmm.inf                                                                        <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernSmbusDxe/SmbusLight.inf                                                               <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernCf9ResetDxe/Cf9Reset.inf                                                              <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernCf9ResetSmm/Cf9ResetSmm.inf                                                           <FAMILY15H>
!ifndef $(AMD_CSM_SUPPORT_DISABLED)
  AgesaModulePkg/Fch/Kern/FchKernLegacyInterruptDxe/LegacyInterrupt.inf
!endif                                                                                                                 <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernSmmControlDxe/SmmControl.inf                                                          <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernSmmDispatcher/FchSmmDispatcher.inf                                                    <FAMILY15H>
  AgesaModulePkg/Fch/Kern/FchKernComplement/ImcControl/ImcControl.inf                                                  <FAMILY15H>

  # Universal
  AgesaModulePkg/Universal/Smbios/AmdSmbiosDxe.inf                                                                     <ALL>
  AgesaModulePkg/Universal/Acpi/AmdAcpiDxe.inf                                                                         <ALL>
  AgesaModulePkg/Universal/AmdSmmCommunication/AmdSmmCommunication.inf                                                 <ALL>
  AgesaModulePkg/Universal/Version/AmdVersionDxe/AmdVersionDxe.inf                                                     <ALL>
  AgesaModulePkg/Universal/AmdiGpuVgaControlDxe/AmdiGpuVgaControlDxe.inf                                               <FAMILY15H>

  ## Ras Dxe Drivers
  AgesaModulePkg/Ras/AmdRasZpSp3ApeiDxe/AmdRasZpSp3ApeiDxe.inf                                                         <SP3ZP>
  AgesaModulePkg/Ras/AmdRasZpSp3Dxe/AmdRasZpSp3Dxe.inf                                                                 <SP3ZP>
  AgesaModulePkg/Ras/AmdRasZpSp3Smm/AmdRasZpSp3Smm.inf                                                                 <SP3ZP>
  AgesaModulePkg/Ras/AmdRasRvApeiDxe/AmdRasRvApeiDxe.inf                                                               <RV>
  AgesaModulePkg/Ras/AmdRasRvDxe/AmdRasRvDxe.inf                                                                       <RV>
  AgesaModulePkg/Ras/AmdRasRvSmm/AmdRasRvSmm.inf                                                                       <RV>
  AgesaModulePkg/Ras/AmdRasRnApeiDxe/AmdRasRnApeiDxe.inf                                                               <RN|CZN>
  AgesaModulePkg/Ras/AmdRasRnDxe/AmdRasRnDxe.inf                                                                       <RN|CZN>
  AgesaModulePkg/Ras/AmdRasSspApeiDxe/AmdRasSspApeiDxe.inf                                                             <SSP|MTS|VMR>
  AgesaModulePkg/Ras/AmdRasSspDxe/AmdRasSspDxe.inf                                                                     <SSP|MTS|VMR>
  AgesaModulePkg/Ras/AmdRasSspSmm/AmdRasSspSmm.inf                                                                     <SSP|MTS|VMR>
  AgesaModulePkg/Ras/AmdRasBrApeiDxe/AmdRasBrApeiDxe.inf                                                               <FAMILY15H>
  AgesaModulePkg/Ras/AmdRasGnApeiDxe/AmdRasGnApeiDxe.inf                                                               <GN>
  AgesaModulePkg/Ras/AmdRasGnDxe/AmdRasGnDxe.inf                                                                       <GN>
  AgesaModulePkg/Ras/AmdRasGnSmm/AmdRasGnSmm.inf                                                                       <GN>
  AgesaModulePkg/Ras/AmdRasBaApeiDxe/AmdRasBaApeiDxe.inf                                                               <BA>
  AgesaModulePkg/Ras/AmdRasBaDxe/AmdRasBaDxe.inf                                                                       <BA>
  AgesaModulePkg/Ras/AmdRasBaSmm/AmdRasBaSmm.inf                                                                       <BA>
  AgesaModulePkg/Ras/AmdRasRsServiceDxe/AmdRasRsServiceDxe.inf {
    <LibraryClasses>
    DfAddressTranslateLib|AgesaModulePkg/Library/Ras/RS/DfAddressTranslateRsLib/DfAddressTranslateRsLib.inf
    RasSocLib|AgesaModulePkg/Library/Ras/RS/RasRsSocLib/RasRsSocLib.inf
    RasAcpiLib|AgesaModulePkg/Library/Ras/RasAcpi63Lib/RasAcpi63Lib.inf
  }                                                                                                                    <RS|RPL>
  AgesaModulePkg/Ras/AmdRasRsDxe/AmdRasRsDxe.inf {
    <LibraryClasses>
    DfAddressTranslateLib|AgesaModulePkg/Library/Ras/RS/DfAddressTranslateRsLib/DfAddressTranslateRsLib.inf
    RasSocLib|AgesaModulePkg/Library/Ras/RS/RasRsSocLib/RasRsSocLib.inf
  }                                                                                                                    <RS|RPL>
  AgesaModulePkg/Ras/AmdRasRsServiceSmm/AmdRasRsServiceSmm.inf {
    <LibraryClasses>
    DfAddressTranslateLib|AgesaModulePkg/Library/Ras/RS/DfAddressTranslateRsLib/DfAddressTranslateRsLib.inf
    RasSocLib|AgesaModulePkg/Library/Ras/RS/RasRsSocLib/RasRsSocLib.inf
    RasSmmLib|AgesaModulePkg/Library/Ras/RS/RasRsSmmLib/RasRsSmmLib.inf
  }                                                                                                                    <RS|RPL>

  ## NVDIMM Dxe Driver
  AgesaModulePkg/JedecNvdimm/Smm/JedecNvdimm.inf                                                                       <SP3SSP|SP3ZP|SP3GN|SP3BA>
  AgesaModulePkg/JedecNvdimm/Dxe/JedecNvdimmDxe.inf                                                                    <SP3SSP|SP3GN|SP3BA>
  AgesaModulePkg/JedecNvdimm/Asl/JedecNvdimmAsl.inf                                                                    <SP3GN|SP3BA>

  ## Usb4 Dxe Driver
  AgesaModulePkg/Usb4/AmdUsb4Dxe/AmdUsb4Dxe.inf                                                                        <RMB|PHX>

  #ACT Dxe Driver
  AgesaModulePkg/Universal/ActDxe/ActDxe.inf                                                                          <SP5RS>

  # MPDMA
  AgesaModulePkg/Nbio/MPDMA/RS/Dxe/MPDMADxe.inf                                                                       <SP5RS>
  AgesaModulePkg/Nbio/MPDMA/RS/Asl/MpDmaAsl.inf                                                                       <SP5RS>

  #ISP Dxe Driver
  AgesaModulePkg/ISP/IspSecureBioDxe/IspSecureBioDxe.inf                                                              <PHX>
  AgesaModulePkg/ISP/IspSecureBioDxe/IspSecureBioACPIAml.inf                                                          <PHX>

#SMM Protection Test Driver
!if $(SMM_PROTECTION_TEST_SUPPORT) == TRUE
  AgesaModulePkg/Universal/AmdTestPkg/SmmProtectionsTest/Smm/SmmProtectionsTestSmm.inf
!endif                                                                                                                 <PHX|RMB|STX|MDN|RPL>

[PcdsFixedAtBuild]
  # AMD AGESA PI Name & Version string
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9/0BristolPI-AM4 1.1.0.0"                            <*Am4Br>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RavenPI-FP5-AM4 1.1.0.2"                          <*Am4Rv>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0SummitPI-AM4 0.0.6.0"                             <*Am4SmBr>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RavenPI-FP5-AM4 1.1.0.2"                          <*Am4RvSmBr>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RavenPI-FP5-AM4 1.1.0.2"                          <*Am4RvSm>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0SummitPI-AM4 0.0.6.0"                             <*Am4Sm>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0PinnaclePI-SP4r2 0.0.7.0"                         <*Sp4Sm>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0PicassoPI-FP5 0.0.8.0"                            <*Fp5Rv>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RenoirPI-FP6 0.0.8.0"                             <*Fp6Rn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0CezannePI-FP6 0.0.8.0"                            <*Fp6Czn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0CezannePI-FP6 0.0.8.0"                            <*Fp6CznRn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0NaplesPI-SP3 0.0.6.0"                             <*Sp3Zp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0SnowOwlPI-SP4 0.0.6.0"                            <*Sp4Zp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0ThreadRipperPI-SP3r2-0.0.5.0"                     <*Sp3TR>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0ComboAM4PI 1.0.0.4"                               <Am4Mts>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0CastlePeakWSPI-sWRX8-1.0.0.2"                     <*Sp3CP>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RomePI-SP3 0.0.3.0"                               <*Sp3Ssp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0MilanPI-SP3 0.0.3.0"                              <*Sp3GnSspZp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0MilanPI-SP3 0.0.6.0"                              <*Sp3Gn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0TrentoPI-SP3 E.0.1.0"                             <*Sp3Ba>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0ComboAM4v2PI 1.2.0.3"                             <*Am4CznRn|*Am4Rn|Mts|*Am4RvSm|Vmr|*Am4Rv|*Am4Sm|*Am4Czn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0PollockPI-FT5 0.0.8.1"                            <*Ft5Rv>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0VangoghPI-FF3 0.0.5.0"                            <*Ff3Vn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0MeroBDK-FF3 0.0.6.4"                              <*Ff3Mr>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RembrandtPI-FP7 1.0.0.A"                         <*Fp7Rmb>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0RembrandtPI-FP7r2 1.0.0.A"                       <*Fp7r2Rmb>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0ChagallWSPI-sWRX4 0.0.8.0"                        <*Sp3CglCp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0ComboAM5PI 0.0.6.4"                               <*Am5Rmb|*Am5Rpl|*Am5RplRmb>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0GenoaPI-SP5 0.0.3.0"                              <*Sp5Rs>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdPackageString|"AGESA!V9\0PhoenixPI-FP8 0.0.1.0"                            <PHX>

  # For APCB HMAC
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdApcbUseHmacChecksum|TRUE                                                      <*Sp5Rs>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdLockApcbDxeAfterSmmLock|TRUE                                                  <*Sp5Rs>
  # For BIOS with A/B layout, send command to PSP no matter Recovery flag is set or not
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdSendPspCommandIgnoreRecoveryFlag|TRUE                                            <RMB|MDN|PHX|RPL|STX>

[PcdsDynamicDefault]
  # Gnb Gbs Override
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdIommuL1ClockGatingEnable|TRUE                                                 <RV>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdIommuL2ClockGatingEnable|TRUE                                                 <RV>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdIOHCClkGatingSupport|TRUE                                                     <RV>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgSstunlClkGating|TRUE                                                       <RV>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdPCIeExactMatchEnable|TRUE                                                     <SSP|GN|BA>
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdDeterminismControl|TRUE                                                          <*Sp3GnSspZp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgNbioCTOtoSC|TRUE                                                           <*Sp3SspZp>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdFchDisableAsfSlave|TRUE                                                       <*Sp3Gn>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdXhciForceGen1|0xFF                                                            <*Sp3Gn|*Sp5Rs>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdPTDMAEnable|TRUE                                                              <GN>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgNTBClockGatingEnable|TRUE                                                  <BA>
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdCfgIoapicSbFeature|0                                                                <CGL>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdCfgIOMMUDynamicPgEnable|FALSE                                                 <*Fp7Rmb|*Fp7r2Rmb>
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdPcieEcrcEnablement|FALSE                                                            <RMB>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdVddOffVid|300                                                                 <*Fp7Rmb|*Fp7r2Rmb>

  # CCX and Fabric Default
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCStateMode|1                                                                     <FP5RV|FT5RV|SP3BA>
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiC3Ctrl|0x00                                                                  <FP5RV|FT5RV>
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdCpuWdtEn|FALSE                                                                 <*Sp5Rs>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableRMSS|TRUE                                                            <SP5RS|FP7Rmb|Fp7r2Rmb>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableFSRM|TRUE                                                            <FP7Rmb|Fp7r2Rmb>
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableERMS|TRUE                                                            <FP7Rmb|Fp7r2Rmb>

  #ACPI C3 Control = LPI
  gEfiAmdAgesaPkgTokenSpaceGuid.PcdAmdAcpiC3Ctrl|2                                                                      <FP6RN|RMB>
  #
  # To be applied in AM4RN
  #

  # For FCH Fencing
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdFchOemSecureEnable|FALSE                                                      <*Sp5Rs|*Sp3Ba|*Sp3Gn>
  # For SSC disablement
  gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdSpreadSpectrum|FALSE                                                           <*Sp5Rs>


