/*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _FCH_DEF_H_
#define _FCH_DEF_H_

AGESA_STATUS  FchInitReset (IN FCH_RESET_DATA_BLOCK *FchParams);
AGESA_STATUS  FchInitEnv (IN FCH_DATA_BLOCK *FchDataBlock);
AGESA_STATUS  FchInitMid (IN FCH_DATA_BLOCK *FchDataBlock);
AGESA_STATUS  FchInitLate (IN FCH_DATA_BLOCK *FchDataBlock);

UINT32        ReadAlink (IN UINT32 Index, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          WriteAlink (IN UINT32 Index, IN UINT32 Data, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          RwAlink (IN UINT32 Index, IN UINT32 AndMask, IN UINT32 OrMask, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          ReadMem (IN UINT32 Address, IN UINT8 OpFlag, IN VOID *ValuePtr);
VOID          WriteMem (IN UINT32 Address, IN UINT8 OpFlag, IN VOID *ValuePtr);
VOID          RwMem (IN UINT32 Address, IN UINT8 OpFlag, IN UINT32 Mask, IN UINT32 Data);
VOID          ReadPci (IN UINT32 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          WritePci (IN UINT32 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          RwPci (
                IN UINT32 Address,
                IN UINT8 OpFlag,
                IN UINT32 Mask,
                IN UINT32  Data,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          ProgramPciByteTable (
                IN REG8_MASK* pPciByteTable,
                IN UINT16 dwTableSize,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          ProgramFchAcpiMmioTbl (IN ACPI_REG_WRITE  *pAcpiTbl, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          ProgramFchSciMapTbl (IN SCI_MAP_CONTROL  *pSciMapTbl, IN FCH_RESET_DATA_BLOCK *FchResetDataBlock);
VOID          ProgramFchGpioTbl (IN GPIO_CONTROL  *pGpioTbl, IN FCH_RESET_DATA_BLOCK *FchResetDataBlock);
VOID          ProgramFchSataPhyTbl (IN SATA_PHY_CONTROL  *pSataPhyTbl, IN FCH_RESET_DATA_BLOCK *FchResetDataBlock);
VOID          GetChipSysMode (IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
BOOLEAN       IsImcEnabled (IN AMD_CONFIG_PARAMS *StdHeader);
VOID          ReadPmio (IN UINT8 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          WritePmio (IN UINT8 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          RwPmio (
                IN UINT8 Address,
                IN UINT8 OpFlag,
                IN UINT32 AndMask,
                IN UINT32 OrMask,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          ReadPmio2 (IN UINT8 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          WritePmio2 (IN UINT8 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          RwPmio2 (
                IN UINT8 Address,
                IN UINT8  OpFlag,
                IN UINT32 AndMask,
                IN UINT32 OrMask,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          ReadBiosram (IN UINT8 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          WriteBiosram (IN UINT8 Address, IN UINT8 OpFlag, IN VOID *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID          GetFchAcpiPmBase (OUT  UINT16 *AcpiPmBase, IN AMD_CONFIG_PARAMS *StdHeader);
UINT8         ReadFchSleepType (IN AMD_CONFIG_PARAMS *StdHeader);
UINT8         ReadFchChipsetRevision (IN AMD_CONFIG_PARAMS *StdHeader);
UINT32        ReadSocType (VOID);
UINT32        ReadSocDieBusNum (UINT8 Die);
BOOLEAN       FchCheckRmbA0 (VOID);
BOOLEAN       FchCheckRmbB0 (VOID);

VOID          FchSmnRead (
                IN UINT32 IohcBus,
                IN UINT32 SmnAddress,
                IN UINT32 *Value,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          FchSmnWrite (
                IN UINT32 IohcBus,
                IN UINT32 SmnAddress,
                IN UINT32 *Value,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          FchSmnRW (
                IN UINT32 IohcBus,
                IN UINT32 SmnAddress,
                IN UINT32 AndMask,
                IN UINT32 OrMask,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          FchSmnRead8 (
                IN UINT32 IohcBus,
                IN UINT32 SmnAddress,
                IN UINT8 *Value8,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          FchSmnWrite8 (
                IN UINT32 IohcBus,
                IN UINT32 SmnAddress,
                IN UINT8 *Value8,
                IN AMD_CONFIG_PARAMS *StdHeader
                );
VOID          FchSmnRW8 (
                IN UINT32 IohcBus,
                IN UINT32 SmnAddress,
                IN UINT8 AndMask,
                IN UINT8 OrMask,
                IN AMD_CONFIG_PARAMS *StdHeader
                );

///
/// Fch Ab Routines
///
///  Pei Phase
///
VOID  FchInitResetAb (IN VOID* FchDataPtr);
VOID  FchProgramAbPowerOnReset (IN VOID* FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitEnvAb (IN VOID* FchDataPtr);
VOID  FchInitEnvAbSpecial (IN VOID* FchDataPtr);
VOID  FchInitMidAb (IN VOID* FchDataPtr);
VOID  FchInitLateAb (IN VOID* FchDataPtr);
///
///  Other Public Routines
///
VOID  FchInitEnvAbLinkInit (IN VOID* FchDataPtr);
VOID  FchAbLateProgram (IN VOID* FchDataPtr);

///
/// Fch Pcie Routines
///
///
///  Dxe Phase
///
VOID  ProgramPcieNativeMode (IN VOID* FchDataPtr);


///
/// Fch HwAcpi Routines
///
///  Pei Phase
///
VOID  FchInitResetHwAcpiP    (IN VOID  *FchDataPtr);
VOID  FchInitResetHwAcpi     (IN VOID  *FchDataPtr);
VOID  ProgramFchHwAcpiResetP (IN VOID  *FchDataPtr);
VOID  FchInitEnableWdt       (IN VOID  *FchDataPtr);
VOID  ProgramResetRtcExt     (IN VOID  *FchDataPtr);
VOID  ProgramCpuRstBTmr      (IN VOID  *FchDataPtr);
VOID  FchInitEnableI2cI3c    (IN VOID  *FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitEnvHwAcpiP                      (IN VOID *FchDataPtr);
VOID  FchInitEnvHwAcpi                       (IN VOID *FchDataPtr);
VOID  ProgramEnvPFchAcpiMmio                 (IN VOID *FchDataPtr);
VOID  ProgramEnvPFchiLa1MTraceMemoryEn       (IN VOID *FchDataPtr);
VOID  ProgramFchEnvHwAcpiPciReg              (IN VOID *FchDataPtr);
VOID  ProgramSpecificFchInitEnvAcpiMmio      (IN VOID *FchDataPtr);
VOID  ProgramFchEnvSpreadSpectrum            (IN VOID *FchDataPtr);
VOID  ProgramFchEnvAoacInit                  (IN VOID *FchDataPtr);
VOID  ProgramFchEnvHfpInit                   (IN VOID *FchDataPtr);
VOID  ProgramFchEnvHidInit                   (IN VOID *FchDataPtr);
VOID  ProgramFchEnvZstateTiming              (IN VOID *FchDataPtr);
VOID  PciIntVwInit                           (IN VOID *FchDataPtr);
VOID  FchInternalDeviceIrqInit               (IN VOID *FchDataPtr);
VOID  FchInitMidHwAcpi                       (IN VOID *FchDataPtr);
VOID  FchInitLateHwAcpi                      (IN VOID *FchDataPtr);
VOID  FchInitLateTriggerSmi                  (IN VOID *FchDataPtr);

///
///  Other Public Routines
///
VOID HpetInit               (IN VOID  *FchDataPtr);
VOID MtC1eEnable            (IN VOID  *FchDataPtr);
VOID GcpuRelatedSetting     (IN VOID  *FchDataPtr);
VOID StressResetModeLate    (IN VOID  *FchDataPtr);
VOID FchEventInitUsbGpe     (IN VOID  *FchDataPtr);
VOID FchEventInitCioGpe     (IN VOID  *FchDataPtr);
VOID FchAl2ahbInit          (IN VOID  *FchDataPtr);
VOID FchI2cUartInit         (IN VOID  *FchDataPtr);
VOID FchI2cUartInitLate     (IN VOID  *FchDataPtr);
VOID FchAlinkRasEnable      (IN VOID  *FchDataPtr);
VOID FchInitLateShortenResetTiming      (IN VOID  *FchDataPtr);

///
/// Fch SATA Routines
///
///  Pei Phase
///
VOID  FchInitResetSata          (IN VOID  *FchDataPtr);
VOID  FchInitResetSataProgram   (IN UINT32 DieBusNum, IN VOID  *FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitMidSata                   (IN VOID  *FchDataPtr);
VOID  FchInitEnvSata                   (IN VOID  *FchDataPtr);
VOID  FchInitLateSata                  (IN VOID  *FchDataPtr);
VOID  FchKTInitEnvProgramSata          (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTInitMidProgramSataRegs      (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTInitLateProgramSataRegs     (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);

VOID  FchInitEnvSataAhciKT             (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchInitLateSataAhciKT            (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchInitEnvSataRaidKT             (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchInitLateSataRaidKT            (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);

VOID  SataAhciSetDeviceNumMsi          (IN UINT32 DieBusNum, IN VOID  *FchDataPtr);
VOID  SataRaidSetDeviceNumMsi          (IN UINT32 DieBusNum, IN VOID  *FchDataPtr);
VOID  SataSetIrqIntResource            (IN VOID  *FchDataPtr, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  FchKTSataInitHideSataPci         (IN UINT32 DieBusNum, IN VOID  *FchDataPtr);
VOID  SataEnableWriteAccessKT          (IN UINT32 DieBusNum, IN UINT32 Controller);
VOID  SataDisableWriteAccessKT         (IN UINT32 DieBusNum, IN UINT32 Controller);
VOID  FchKTSataInitBootUpSata          (IN UINT32 DieBusNum, IN BOOLEAN Enable, IN VOID  *FchDataPtr);
VOID  FchKTSataInitEnableSata          (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitDisableSata         (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitHideNbioSataPci     (IN UINT32 DieBusNum, IN UINT32 NbioSata, IN VOID  *FchDataPtr);
VOID  FchKTSataInitEnableNbioSataPci     (IN UINT32 DieBusNum, IN UINT32 NbioSata, IN VOID  *FchDataPtr);
VOID  FchKTSataInitHideUnconnectedSataPci     (IN UINT32 DieBusNum, IN VOID  *FchDataPtr);
VOID  FchKTSataInitStaggeredSpinStep2  (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitPortOffline (
        IN UINT32 DieBusNum,
        IN UINT32 Controller,
        IN UINT32 PortNum,
        IN VOID *FchDataPtr
        );
VOID  FchKTSataInitPortActive (
        IN UINT32 DieBusNum,
        IN UINT32 Controller,
        IN UINT32 PortNum,
        IN VOID *FchDataPtr
        );
VOID  FchKTSataInitEnableErr           (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitEsata               (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitRsmuCtrl            (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitCtrlReg             (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitMMC                 (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataSetPortGenMode          (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataControllerSetPortGenMode (
        IN UINT32 DieBusNum,
        IN UINT32 Controller,
        IN UINT16 PortMode,
        IN VOID *FchDataPtr
        );
VOID  FchKTSataShutdownUnconnectedSataPortClock (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataAutoShutdownController  (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataGpioInitial             (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataGpioSetPad              (IN UINT32 DieBusNum, IN UINT32 Controller, IN VOID  *FchDataPtr);
VOID  FchKTSataInitDevSlp              (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID  FchKTSataInitMpssMap             (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID  FchKTSataRecovery                (IN UINT32 DieBusNum);
BOOLEAN FchSataD3ColdClockOn           (IN UINT32 Controller);

///
/// FCH USB Controller Public Function
///
///  Pei Phase
///
VOID  FchInitResetUsb            (IN VOID  *FchDataPtr);
VOID  FchInitResetXhci           (IN VOID  *FchDataPtr);
VOID  FchInitResetXhciProgram    (IN VOID  *FchDataPtr);
VOID  FchInitResetUsb4           (IN VOID  *FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitEnvUsb              (IN VOID  *FchDataPtr);
VOID  FchInitMidUsb              (IN VOID  *FchDataPtr);
VOID  FchInitLateUsb             (IN VOID  *FchDataPtr);
VOID  FchInitEnvUsbXhci          (IN VOID  *FchDataPtr);
VOID  FchInitMidUsbXhci          (IN VOID  *FchDataPtr);
VOID  FchInitLateUsbXhci         (IN VOID  *FchDataPtr);
VOID  FchInitEnvUsb4             (IN VOID  *FchDataPtr);
VOID  FchInitMidUsb4             (IN VOID  *FchDataPtr);
VOID  FchInitLateUsb4            (IN VOID  *FchDataPtr);
///
///  Other Public Routines
///
VOID FchKTXhciInitBootProgram    (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID FchKTXhciInitS3ExitProgram  (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID FchKTXhciInitS3EntryProgram (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID FchKTXhciInitSsid (IN UINT32 DieBusNum, IN UINT32 Ssid);
VOID FchKTDisableXhci2 (VOID);
BOOLEAN FchKTXhciSmuUsbConfigUpdate (
          IN UINT32 DieBusNum,
          IN UINT32 smn_register,
          IN UINT32 smn_mask,
          IN UINT32 smn_data,
          IN UINT32 smn_group
          );
BOOLEAN FchKTXhciCheckUsbPhySkip    (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
BOOLEAN FchKTXhciCheckUsbSkipFlag   (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID FchKTUsb4InitBootProgram    (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
VOID FchKTUsb4InitS3ExitProgram  (IN UINT32 DieBusNum, IN VOID *FchDataPtr);

///
/// Fch Sd Routines
///
VOID  FchInitEnvSd  (IN VOID  *FchDataPtr);
VOID  FchInitMidSd  (IN VOID  *FchDataPtr);
VOID  FchInitLateSd (IN VOID  *FchDataPtr);

///
///  Other Public Routines
///

VOID FchInitEnvSdProgram (IN VOID  *FchDataPtr);

///
/// Fch Spi Routines
///
///  Pei Phase
///
VOID  FchInitResetSpi        (IN VOID  *FchDataPtr);
VOID  FchInitResetLpc        (IN VOID  *FchDataPtr);
VOID  FchInitResetLpcProgram (IN VOID  *FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitEnvSpi          (IN VOID  *FchDataPtr);
VOID  FchInitMidSpi          (IN VOID  *FchDataPtr);
VOID  FchInitLateSpi         (IN VOID  *FchDataPtr);
VOID  FchInitEnvLpc          (IN VOID  *FchDataPtr);
VOID  FchInitMidLpc          (IN VOID  *FchDataPtr);
VOID  FchInitLateLpc         (IN VOID  *FchDataPtr);
VOID  FchInitEnvLpcProgram   (IN VOID  *FchDataPtr);
///
///  Other Public Routines
///
VOID  FchSpiUnlock       (IN VOID  *FchDataPtr);
VOID  FchSpiLock         (IN VOID  *FchDataPtr);

///
/// Fch ESPI Routines
///
///
VOID  FchInitResetEspi   (IN VOID  *FchDataPtr);
VOID  FchInitKTEspiEnableIo80 (IN  BOOLEAN  Enable);
VOID  FchInitKTEspiEnableKbc6064 (IN BOOLEAN  Enable);
VOID  FchInitKTEspiTimer (IN UINT32 EspiBase);
VOID  FchInitKTEspiIoMux (IN VOID  *FchDataPtr);
VOID  FchInitKTEspiDevice0 (IN VOID  *FchDataPtr);
VOID  FchInitKTEspiIoDecode (IN  VOID  *FchDataPtr);
VOID  FchInitKTEspiMmioDecode (IN  VOID  *FchDataPtr);
UINT32 GetEspiBase ();


///
/// Fch eMMC Routines
///
///  Pei Phase
///
VOID  FchInitResetEmmc (IN VOID* FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitEnvEmmc (IN VOID* FchDataPtr);
VOID  FchInitMidEmmc (IN VOID* FchDataPtr);
VOID  FchInitLateEmmc (IN VOID* FchDataPtr);

///
/// Fch Wifi Routines
///
///  Pei Phase
///
VOID  FchInitResetWifi (IN VOID* FchDataPtr);
///
///  Dxe Phase
///
VOID  FchInitEnvWifi (IN VOID* FchDataPtr);
VOID  FchInitMidWifi (IN VOID* FchDataPtr);
VOID  FchInitLateWifi (IN VOID* FchDataPtr);

VOID  FchInitWifiKTPowerSaving (IN VOID* FchDataPtr);

///
/// Fch XgbE Routines
///
///  Pei Phase
///

///
///  Dxe Phase
///
VOID  FchInitEnvXGbe (IN VOID* FchDataPtr);

VOID  FchKtXgbePortClockGate (IN UINT32 DieBusNum, IN UINT8 Port);
VOID  FchKtXgbePortPlatformConfig (IN UINT32 DieBusNum, IN UINT8 PortNum, IN UINT32 SystemPort, IN VOID *PortData);
VOID  FchKtXgbePortMacAddress (IN UINT32 DieBusNum, IN UINT8 PortNum, IN VOID *MacData);
VOID  FchKtXgbePortPtpInit (IN UINT32 DieBusNum, IN UINT8 PortNum, IN VOID *PortData);
VOID  FchKtXgbePadEnable (IN UINT32 DieBusNum, IN VOID *FchDataPtr);
/*--------------------------- Documentation Pages ---------------------------*/
VOID  FchStall (IN UINT32 uSec, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  FchPciReset (IN AMD_CONFIG_PARAMS *StdHeader);
VOID  OutPort80 (IN UINT32 pcode, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  OutPort1080 (IN UINT32 pcode, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  GetEfuseStatus (IN VOID* Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  TurnOffCG2 (OUT VOID);
VOID  BackUpCG2 (OUT VOID);
VOID  FchCopyMem (IN VOID* pDest, IN VOID* pSource, IN UINTN Length);
VOID* GetRomSigPtr (IN UINTN* RomSigPtr, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  RwXhciIndReg (IN UINT32 Index, IN UINT32 AndMask, IN UINT32 OrMask, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  RwXhci0IndReg (IN UINT32 Index, IN UINT32 AndMask, IN UINT32 OrMask, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  RwXhci1IndReg (IN UINT32 Index, IN UINT32 AndMask, IN UINT32 OrMask, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  ReadXhci0Phy (IN UINT32 Port, IN UINT32 Address, IN UINT32 *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  ReadXhci1Phy (IN UINT32 Port, IN UINT32 Address, IN UINT32 *Value, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  RwSsicIndReg (IN UINT32 Index, IN UINT32 AndMask, IN UINT32 OrMask, IN AMD_CONFIG_PARAMS *StdHeader);
VOID  AcLossControl (IN UINT8 AcLossControlValue);
VOID  FchVgaInit (OUT VOID);
VOID  RecordFchConfigPtr (IN UINT32 FchConfigPtr);
VOID  ValidateFchVariant (IN VOID  *FchDataPtr);
VOID  RecordSmiStatus (IN AMD_CONFIG_PARAMS *StdHeader);
VOID  ClearAllSmiStatus (IN AMD_CONFIG_PARAMS *StdHeader);
BOOLEAN  IsExternalClockMode (IN VOID  *FchDataPtr);
VOID  SbSleepTrapControl (IN BOOLEAN SleepTrap);

AGESA_STATUS
FchSpiTransfer (
  IN       UINT8    PrefixCode,
  IN       UINT8    Opcode,
  IN  OUT  UINT8    *DataPtr,
  IN       UINT8    *AddressPtr,
  IN       UINT8    Length,
  IN       BOOLEAN  WriteFlag,
  IN       BOOLEAN  AddressFlag,
  IN       BOOLEAN  DataFlag,
  IN       BOOLEAN  FinishedFlag
  );

VOID
FchDisEspiMasCtlRegWr (
  IN  VOID     *FchDataPtr
  );

BOOLEAN
FchConfigureSpiDeviceDummyCycle (
  IN       UINT32     DeviceID,
  IN       UINT8      SpiMode
  );

UINT32
FchReadSpiId (
  IN       BOOLEAN    Flag
  );

BOOLEAN
FchPlatformSpiQe (
  IN       VOID     *FchDataPtr
  );

VOID
ProgramPMEDis (
  IN       UINT8     dbBusNo,
  IN       BOOLEAN   PMEDis
  );

VOID
FchSmiGenerateControl (
  IN       BOOLEAN   SmiControl
  );

BOOLEAN
FchKTXhciSmuService (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId
  );

BOOLEAN
FchKTXhciSmuServiceWithPara (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  UINT32    *Parameter
  );

BOOLEAN
FchKTUsbSmuServiceReqOnly (
    IN  UINT32    DieBusNum,
    IN  UINT32    RequestId,
    IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  );

BOOLEAN
FchKTUsbSmuServiceNew (
    IN  UINT32    DieBusNum,
    IN  UINT32    RequestId,
    IN  FCH_RESET_DATA_BLOCK     *FchDataPtr
  );

BOOLEAN
FchKTSmuService (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  UINT32    Controller
  );

BOOLEAN
FchKTSmuServiceOnly (
  IN  UINT32    DieBusNum,
  IN  UINT32    RequestId,
  IN  UINT32    Controller
  );

VOID
FchKTXhciDisablePortLate (
  IN  UINT32   DieBusNum,
  IN  FCH_DATA_BLOCK     *FchDataPtr
  );

VOID
KTeSPI_NoticeSlaveBeforeUserReset (
  );
#endif


