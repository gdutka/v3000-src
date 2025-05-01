/*****************************************************************************
  *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
  *******************************************************************************
  *
 */
/* $NoKeywords:$ */
/**
 * @file
 *
 * MemSmbiosV2Lib.c
 *
 * Memory DMI table support.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main)
 * @e \$Revision: 329870 $ @e \$Date: 2015-10-29 13:13:29 +0800 (Thu, 29 Oct 2015) $
 *
 **/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */

#include <Library/BaseLib.h>
#include "AGESA.h"
#include "Library/AmdBaseLib.h"
#include "Library/IdsLib.h"
#include "Library/AmdHeapLib.h"
#include "mm.h"
#include "mn.h"
#include "mtspd5.h"
#include "MemDmi.h"
#include <Addendum/Apcb/Inc/RMB/APOB.h>
#include <Library/AmdPspApobLib.h>
#include "Library/AmdCalloutLib.h"
#include "Library/MemChanXLat.h"
#include <Library/ApobCommonServiceLib.h>
#include <Guid/AmdMemoryInfoHob.h>
#include "Filecode.h"

#define FILECODE LIBRARY_MEMSMBIOSV2RMBD5LIB_MEMSMBIOSV2LIB_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

#define SPD_BUFFER_SIZE 1024

VOID
InitSmbios32Type17 (
  IN      BOOLEAN                       DimmPresent,
  IN      UINT8                         *SpdData,
  IN OUT  TYPE17_DMI_INFO               *T17
  );

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

typedef struct _MEM_TCK_ENTRY {
  UINT16  Memclk;
  UINT16  TckPs;
} MEM_TCK_ENTRY;

CONST MEM_TCK_ENTRY MemFreqToTckTable[] = {
  {1000,1000},
  {1050, 952},
  {1067, 938},
  {1200, 833},
  {1333, 750},
  {1367, 732},
  {1400, 714},
  {1433, 698},
  {1467, 682},
  {1500, 667},
  {1533, 652},
  {1567, 638},
  {1600, 625},
  {1633, 612},
  {1667, 599},
  {1700, 588},
  {1733, 577},
  {1767, 565},
  {1800, 555},
  {1833, 545},
  {1867, 535},
  {1900, 526},
  {1933, 517},
  {1967, 508},
  {2000, 500},
  {2033, 491},
  {2067, 483},
  {2100, 476},
  {2133, 468},
  {2167, 461},
  {2200, 454},
  {2233, 447},
  {2267, 441},
  {2300, 434},
  {2333, 428},
  {2367, 422},
  {2400, 416},
  {2433, 411},
  {2467, 405},
  {2500, 400},
  {2550, 392},
  {2600, 384},
  {2650, 377},
  {2700, 370},
  {2750, 363},
  {2800, 357},
  {2850, 350},
  {2900, 344},
  {2950, 338},
  {3000, 333},
  {3050, 327},
  {3100, 322},
  {3150, 317},
  {3200, 312},
  {3250, 307},
  {3300, 303},
  {3350, 298},
  {3400, 294},
  {3450, 289},
  {3500, 285},
  {3550, 281},
  {3600, 277},
  {3650, 273},
  {3700, 270},
  {3750, 266},
  {3800, 263},
  {3850, 259},
  {3900, 256},
  {3950, 253},
  {4000, 250},
  {4050, 246},
  {4100, 243},
  {4150, 240},
  {4200, 238},
  {4250, 235},
  {4300, 232},
  {4350, 229},
  {4400, 227}
};

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
VOID
STATIC
IntToString (
  IN OUT   CHAR8 *String,
  IN       UINT8 *Integer,
  IN       UINT8 SizeInByte
);

/* -----------------------------------------------------------------------------*/
/**
 *  TranslateChannelInfo
 *
 *  Description:
 *     Translate the channel Id depending upon the channel translation table.
 *
 *  Parameters:
 *    @param[in]        RequestedChannelId - The requested channel Id
 *    @param[out]       *TranslatedChannelId - Pointer to the translated Id
 *    @param[in]        *XlatTable - Pointer to the memory channel translation table
 *
 */
VOID
STATIC
TranslateChannelInfo (
  IN       UINT8 RequestedChannelId,
  OUT      UINT8 *TranslatedChannelId,
  IN       HOST_TO_APCB_CHANNEL_XLAT *XlatTable
  )
{
  UINT8 Index;

  *TranslatedChannelId = RequestedChannelId;

  if (XlatTable != NULL) {
    Index = 0;
    while (XlatTable->RequestedChannelId != 0xFF) {
      if (RequestedChannelId == XlatTable->RequestedChannelId) {
        *TranslatedChannelId = XlatTable->TranslatedChannelId;
        return;
      }
      XlatTable++;
    }
  }
}

UINT8
MemSpdDecodeDensity (
   UINT8  SpdDensity
  )
{
  UINT8 SpdDensityTable[] = SPD_DENSITY_TABLE;
  UINT8 Density;

  if (SpdDensity >= sizeof(SpdDensityTable)) {
    Density = 0;
  } else {
    Density = SpdDensityTable[SpdDensity];
  }
  return Density;
}

UINT8
MemSpdDecodeDiesPerPackage (
   UINT8  SpdDies
  )
{
  UINT8 SpdDiePerPkgTable[] = SPD_DIE_PER_PKG_TABLE;
  UINT8 Dies;

  if (SpdDies >= sizeof(SpdDiePerPkgTable)) {
    Dies = 0;
  } else {
    Dies = SpdDiePerPkgTable[SpdDies];
  }
  return Dies;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetPhysicalDimmInfoD4
 *
 *  Description:
 *     Obtain the physical DIMM information from SPD data.
 *
 *  Parameters:
 *    @param[in, out]  *T17                 - Pointer to TYPE17_DMI_INFO
 *    @param[in]       *PhysicalDimm        - Pointer to the physical DIMM info
 *    @param[in]       TranslatedChannel    - Translated Channel Number
 *    @param[in]       *StdHeader           - Pointer to AMD_CONFIG_PARAMS
 *
 *    @retval          BOOLEAN
 *
 */
BOOLEAN
STATIC
GetPhysicalDimmInfoD4 (
  IN OUT   TYPE17_DMI_INFO *T17,
  IN       APOB_MEM_DMI_PHYSICAL_DIMM  *PhysicalDimm,
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       UINT8 TranslatedChannel
  )
{
  UINT16                i;
  UINT8                 Socket;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 DimmSpd[SPD_BUFFER_SIZE];
  AGESA_READ_SPD_PARAMS ReadSpd;
  AGESA_STATUS          AgesaStatus;
  UINT8                 IoWidth;
  UINT16                BusWidth;
  UINT16                TckPs;
  UINT32                MemorySize;
  UINT32                FreqTableIndex;
  APOB_TYPE_HEADER      *ApobEntry;
  EFI_STATUS            ApobStatus;
  UINT16                Instance;
  UINT16                DieLoop;
  UINT8                 NumRanks;
  UINT8                 Rank;
  UINT8                 SpdCapacity;
  BOOLEAN               Asymetric;
  APOB_MEM_DIMM_SPD_DATA_STRUCT ApobSpdHeader;
  SPD_BASE_CONFIG_0_S  *BaseConfig0;
  SPD_ANNEX_COMMON_S   *ModuleParms;
  SPD_MANUFACTURING_INFO_S *MfgInfo;
  UINT8 ChannelsPerDimm;
  UINT8 DiePerPkg;

//  UINT32 ManufacturerIdCode;

  Socket  = PhysicalDimm->Socket;
  Channel = PhysicalDimm->Channel;
  Dimm    = PhysicalDimm->Dimm;
  T17->Handle = PhysicalDimm->Handle;
  T17->DeviceSet = 0;

  T17->DeviceLocator[0] = 'D';
  T17->DeviceLocator[1] = 'I';
  T17->DeviceLocator[2] = 'M';
  T17->DeviceLocator[3] = 'M';
  T17->DeviceLocator[4] = ' ';
  T17->DeviceLocator[5] = Dimm + 0x30;
  T17->DeviceLocator[6] = '\0';
  T17->DeviceLocator[7] = '\0';

  T17->BankLocator[0] = 'P';
  T17->BankLocator[1] = Socket + 0x30;
  T17->BankLocator[2] = ' ';
  T17->BankLocator[3] = 'C';
  T17->BankLocator[4] = 'H';
  T17->BankLocator[5] = 'A';
  T17->BankLocator[6] = 'N';
  T17->BankLocator[7] = 'N';
  T17->BankLocator[8] = 'E';
  T17->BankLocator[9] = 'L';
  T17->BankLocator[10] = ' ';
  T17->BankLocator[11] = TranslatedChannel + 0x41;
  T17->BankLocator[12] = '\0';

  if (PhysicalDimm->DimmPresent) {
    ReadSpd.SocketId = Socket;
    ReadSpd.MemChannelId = TranslatedChannel;
    ReadSpd.DimmId = Dimm;

    ReadSpd.Buffer = &DimmSpd[0];
    LibAmdMemCopy (&ReadSpd.StdHeader, StdHeader, sizeof (AMD_CONFIG_PARAMS), StdHeader);

    IDS_HDT_CONSOLE (MAIN_FLOW, "SPD Socket %d Channel %d Dimm %d: %08x\n", Socket, Channel, Dimm, DimmSpd);
    AGESA_TESTPOINT (TpProcMemBeforeAgesaReadSpd, StdHeader);
    // Get SPD Data from APOB
    ApobEntry = NULL;
    DieLoop = 0;
    AgesaStatus = AGESA_UNSUPPORTED;
    for (DieLoop = 0; DieLoop < ABL_APOB_MAX_DIES_PER_SOCKET; DieLoop++) {
    //Instance = (TranslatedChannel >> 1);
      Instance = DieLoop;
      Instance |= ((Socket & 0x000000FF) << 8);
      IDS_HDT_CONSOLE (MAIN_FLOW, "Get Spd Data from APOB for Socket %d, Die %d , Channel %d Instance %d\n", Socket, DieLoop, TranslatedChannel, Instance);
      ApobStatus = ApobGetDimmSpdData (Instance, Socket, Channel, Dimm, SPD_BUFFER_SIZE, DimmSpd);
      if (ApobStatus == EFI_SUCCESS) {
        LibAmdMemCopy (&ApobSpdHeader, ApobEntry, sizeof (APOB_MEM_DIMM_SPD_DATA_STRUCT), StdHeader);
        AgesaStatus = AGESA_SUCCESS;
        break;
      }
      if (AgesaStatus == AGESA_SUCCESS){
        break;
      }
    }
    if (AgesaStatus != AGESA_SUCCESS) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "Get Spd Data from SMBUS\n");
      AgesaStatus = AgesaReadSpd (0, &ReadSpd);
    }
    AGESA_TESTPOINT (TpProcMemAfterAgesaReadSpd, StdHeader);

    ASSERT (AGESA_SUCCESS == AgesaStatus);

    for (i = 0; i < 1024; i ++) {
      if (0 == i % 16) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "\n\t");
      }
      IDS_HDT_CONSOLE (MAIN_FLOW, "%02x ", DimmSpd[i]);
    }
    IDS_HDT_CONSOLE (MAIN_FLOW, "\n");

    BaseConfig0 = (SPD_BASE_CONFIG_0_S*) &(DimmSpd[SpdBlock_BaseConfig_0 * SPD_BLOCK_LEN]);
    ModuleParms = (SPD_ANNEX_COMMON_S*) &(DimmSpd[SpdBlock_ModuleParms_0 * SPD_BLOCK_LEN]);
    MfgInfo     = (SPD_MANUFACTURING_INFO_S*) &(DimmSpd[SpdBlock_MfgInfo0 * SPD_BLOCK_LEN]);
    MemorySize = 0;
    Asymetric = (ModuleParms->ModuleOrg.Field.RankMix == RankMixAsymmetrical) ? TRUE : FALSE;
    NumRanks = SPD_PACKAGE_RANKS_DECODE(ModuleParms->ModuleOrg.Field.RanksPerChannel);
    for (Rank = 0; Rank < MIN(NumRanks, 2); Rank ++ ) {
      if (Asymetric && (Rank & 0x01) ) {
        IoWidth = SPD_DECODE_IO_WIDTH(BaseConfig0->SecondIoWidth.Field.IoWidth);
        DiePerPkg = MemSpdDecodeDiesPerPackage(BaseConfig0->SecondDensity.Field.DiePerPkg);
      } else {
        IoWidth = SPD_DECODE_IO_WIDTH(BaseConfig0->FirstIoWidth.Field.IoWidth);
        DiePerPkg = MemSpdDecodeDiesPerPackage(BaseConfig0->FirstDensity.Field.DiePerPkg);
      }
      BusWidth = SPD_CHANNEL_BUS_WIDTH_DECODE(ModuleParms->ChBusWidth.Field.Width);
      ChannelsPerDimm = SPD_CHANNELS_PER_DIMM_DECODE(ModuleParms->ChBusWidth.Field.NumChannels);
      T17->DataWidth = BusWidth * ChannelsPerDimm;
      T17->TotalWidth = (BusWidth + SPD_CHANNEL_BUS_WIDTH_EXT_DECODE (ModuleParms->ChBusWidth.Field.WidthExt)) * ChannelsPerDimm;
      SpdCapacity = MemSpdDecodeDensity(BaseConfig0->FirstDensity.Field.Density);
      MemorySize += ChannelsPerDimm * BusWidth / IoWidth * DiePerPkg * SpdCapacity / 8; // MemorySize in GB
    }

    MemorySize = MemorySize << 10; // MemorySize in MB
    if (MemorySize < 0x7FFF) {
      T17->MemorySize = (UINT16)MemorySize;
      T17->ExtSize = 0;
    } else {
      T17->MemorySize = 0x7FFF;
      T17->ExtSize = MemorySize;
    }

    T17->TypeDetail.Synchronous = 1;

    switch (BaseConfig0->KeyByte2.Field.BaseModuleType) {
      case SPD_BASEMODULE_RDIMM:
        T17->TypeDetail.Registered = 1;
        T17->FormFactor = DimmFormFactorFormFactor;
        break;
      case SPD_BASEMODULE_LRDIMM:
        T17->TypeDetail.Registered = 1;
        T17->FormFactor = DimmFormFactorFormFactor;
        break;
      case SPD_BASEMODULE_UDIMM:
        T17->TypeDetail.Unbuffered = 1;
        T17->FormFactor = DimmFormFactorFormFactor;
        break;
      case SPD_BASEMODULE_SODIMM:
      case SPD_BASEMODULE_SORDIMM:
        T17->TypeDetail.Unbuffered = 1;
        T17->FormFactor = SodimmFormFactor;
        break;
      case SPD_BASEMODULE_SOLDEREDDOWN:
        T17->TypeDetail.Unbuffered = 1;
        T17->FormFactor = OtherFormFactor;
        break;
      default:
        T17->TypeDetail.Unknown = 1;
        T17->FormFactor = UnknowFormFactor;
    }

    switch (BaseConfig0->KeyByte1.Field.ModuleType) {
      case ModTypeLpddr4Sram:
      case ModTypeLpddr4xSdram:
        T17->MemoryType = LpDdr4MemType;
        T17->MinimumVoltage = 600;
        T17->MaximumVoltage = 600;
        break;
      case ModTypeLpddr5Sdram:
        T17->MemoryType = LpDdr5MemType;
        T17->MinimumVoltage = 1100;
        T17->MaximumVoltage = 1100;
        break;
      case ModTypeDdr4Sdram:
      case ModTypeDdr4ESdram:
        T17->MemoryType = Ddr4MemType;
        T17->MinimumVoltage = 1200;
        T17->MaximumVoltage = 1200;
        break;
      default:
        T17->MemoryType = Ddr5MemType;
        T17->MinimumVoltage = 1100;
        T17->MaximumVoltage = 1100;
        break;
    }
    TckPs = BaseConfig0->TckAvgMin.Value;
    for (FreqTableIndex = 0; FreqTableIndex < sizeof(MemFreqToTckTable) / sizeof(MemFreqToTckTable[0]); FreqTableIndex ++) {
      if (MemFreqToTckTable[FreqTableIndex].TckPs <= TckPs) {
        T17->Speed = MemFreqToTckTable[FreqTableIndex].Memclk;
        break;
      }
    }

    T17->ManufacturerIdCode = MfgInfo->ModuleMfgId.Value;
    IntToString (T17->SerialNumber, MfgInfo->ModuleSerialNumber.String, SPD_MODULE_SERIAL_NUMBER_LEN);
    for (i = 0; i < sizeof (T17->PartNumber) - 1; i ++) {
      T17->PartNumber[i] = MfgInfo->ModulePartNumber.String[i];
    }
    T17->PartNumber[i] = 0;

    T17->Attributes = NumRanks;

    T17->ConfigSpeed = PhysicalDimm->ConfigSpeed;
    T17->ConfiguredVoltage = PhysicalDimm->ConfigVoltage;

  } else {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Dummy Type 17 Created for Socket %d, Channel %d, Dimm %d\n", Socket, Channel, Dimm);
    T17->DataWidth = 0xFFFF;
    T17->TotalWidth = 0xFFFF;
    T17->MemorySize = 0;
    T17->ExtSize = 0;
    T17->TypeDetail.Unknown = 1;
    T17->FormFactor = UnknowFormFactor;
    T17->MemoryType = UnknownMemType;
    T17->Speed = 0;
    T17->ManufacturerIdCode = 0;

    for (i = 0; i < sizeof (T17->SerialNumber); i++) {
      T17->SerialNumber[i] = 0x0;
    }

    for (i = 0; i < sizeof (T17->PartNumber); i++) {
      T17->PartNumber[i] = 0x0;
    }

    T17->Attributes = 0;
    T17->MinimumVoltage = 0;
    T17->MaximumVoltage = 0;
    T17->ConfigSpeed = 0;
    T17->ConfiguredVoltage = 0;
  }

  InitSmbios32Type17 (PhysicalDimm->DimmPresent, DimmSpd, T17);

  if (PhysicalDimm->DimmPresent) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetLogicalDimmInfo
 *
 *  Description:
 *     Obtain the logical DIMM information for Type 20.
 *
 *  Parameters:
 *    @param[in]        T20 - Pointer to TYPE20_DMI_INFO
 *    @param[out]       *LogicalDimm - Pointer to the logical DIMM info
 *
 *    @retval         BOOLEAN
 *
 */
BOOLEAN
GetLogicalDimmInfo (
  IN OUT   TYPE20_DMI_INFO *T20,
  IN       APOB_MEM_DMI_LOGICAL_DIMM *LogicalDimm
  )
{
  if (1 == LogicalDimm->DimmPresent) {
    T20->MemoryDeviceHandle = LogicalDimm->MemoryDeviceHandle;
    T20->PartitionRowPosition = 0xFF;
    T20->InterleavePosition   = 0;
    T20->InterleavedDataDepth = 0;
    if (1 == LogicalDimm->Interleaved) {
      T20->InterleavePosition = 0xFF;
      T20->InterleavedDataDepth = 0xFF;
    }
    T20->StartingAddr       = LogicalDimm->StartingAddr;
    T20->EndingAddr         = LogicalDimm->EndingAddr;
    T20->ExtStartingAddr    = LogicalDimm->UnifiedExtStartingAddr.ExtStartingAddr;
    T20->ExtEndingAddr      = LogicalDimm->UnifiedExtEndingAddr.ExtEndingAddr;
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *  GetMemSmbiosV2
 *
 *  Description:
 *     This is the common routine for getting DMI Type 16, Type 17, Type 19 and Type 20 related information.
 *
 *  Parameters:
 *    @param[in]        DramType - DRAM Type
 *    @param[in]        *ApobSmbiosInfo - Pointer to the APOB SMBIOS data block
 *    @param[in]        *MemChanXLatTab - Pointer to the memory channel translation table
 *    @param[in]        *StdHeader - Pointer to AMD_CONFIG_PARAMS
 *
 *    @retval         AGESA_STATUS
 *
 */
AGESA_STATUS
GetMemSmbiosV2 (
  IN       UINT8                       DramType,
  IN       UINT8                       NumSockets,
  IN       VOID                        *ApobSmbiosInfo,
  IN       HOST_TO_APCB_CHANNEL_XLAT   *MemChanXLatTab,
  IN       AMD_CONFIG_PARAMS           *StdHeader,
  IN       AMD_MEMORY_INFO_HOB         *AmdMemoryInfoHob
  )
{
  UINT8                         Socket;
  UINT8                         Channel;
  UINT8                         Dimm;
  UINT16                        DimmIndex;
  UINT8                         MaxPhysicalDimms;
  UINT8                         MaxLogicalDimms;
  UINT64                        Value64;
  DMI_INFO                      *DmiBuffer;
  ALLOCATE_HEAP_PARAMS          AllocateHeapParams;
  APOB_MEM_DMI_HEADER           *ApobMemDmiHeader;
  UINT32                        TotalMemSize;
  APOB_MEM_DMI_PHYSICAL_DIMM    *PhysicalDimm;
  APOB_MEM_DMI_LOGICAL_DIMM     *LogicalDimm;
  UINT8                         NumActiveDimms;
  UINT8                         TranslatedChannel;
  //
  // Allocate a buffer by heap function
  //
  AllocateHeapParams.BufferHandle = AMD_DMI_INFO_BUFFER_HANDLE;
  AllocateHeapParams.RequestedBufferSize = sizeof (DMI_INFO);
  AllocateHeapParams.Persist = HEAP_SYSTEM_MEM;

  if (HeapAllocateBuffer (&AllocateHeapParams, StdHeader) != AGESA_SUCCESS) {
    ASSERT (FALSE);
    return AGESA_ERROR;
  }

  DmiBuffer = (DMI_INFO *) AllocateHeapParams.BufferPtr;

  IDS_HDT_CONSOLE (MAIN_FLOW, "\tDMI enabled\n");
  // Fill with 0x00
  LibAmdMemFill (DmiBuffer, 0x00, sizeof (DMI_INFO), StdHeader);

  ASSERT (ApobSmbiosInfo != NULL);
  ApobMemDmiHeader = (APOB_MEM_DMI_HEADER *)ApobSmbiosInfo;
  MaxPhysicalDimms = ApobMemDmiHeader->MaxPhysicalDimms;
  MaxLogicalDimms = ApobMemDmiHeader->MaxLogicalDimms;
  TotalMemSize = 0;
  PhysicalDimm = (APOB_MEM_DMI_PHYSICAL_DIMM *)&ApobMemDmiHeader[1];

  // Type 16 construction
  DmiBuffer->T16.Location = 0x03;
  DmiBuffer->T16.Use = 0x03;
  DmiBuffer->T16.NumberOfMemoryDevices = MaxPhysicalDimms;
  DmiBuffer->T16.MemoryErrorCorrection = (ApobMemDmiHeader->EccCapable != 0) ? Dmi16MultiBitEcc : Dmi16NoneErrCorrection;

  NumActiveDimms = 0;
  // TYPE 17 entries are organized by physical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxPhysicalDimms; DimmIndex ++, PhysicalDimm ++) {
    Socket  = PhysicalDimm->Socket;
    TranslatedChannel = PhysicalDimm->Channel;
    TranslateChannelInfo (PhysicalDimm->Channel, &TranslatedChannel, MemChanXLatTab);
    Dimm    = PhysicalDimm->Dimm;
    if (GetPhysicalDimmInfoD4 (&DmiBuffer->T17[Socket][TranslatedChannel][Dimm], PhysicalDimm, StdHeader, TranslatedChannel)) {
      NumActiveDimms ++;
    }
    TotalMemSize += (DmiBuffer->T17[Socket][TranslatedChannel][Dimm].MemorySize != 0x7FFF) ?
                      DmiBuffer->T17[Socket][TranslatedChannel][Dimm].MemorySize : DmiBuffer->T17[Socket][TranslatedChannel][Dimm].ExtSize;
  }

  // Pointer to DMI info of Logical DIMMs
  LogicalDimm = (APOB_MEM_DMI_LOGICAL_DIMM *) PhysicalDimm;

  // TYPE 20 entries are organized by logical DIMMs
  for (DimmIndex = 0; DimmIndex < MaxLogicalDimms; DimmIndex ++, LogicalDimm ++) {
    Socket  = LogicalDimm->Socket;
    TranslateChannelInfo (LogicalDimm->Channel, &Channel, MemChanXLatTab);
    if (Channel != 0xFF) {
      Dimm    = LogicalDimm->Dimm;
      GetLogicalDimmInfo (&DmiBuffer->T20[Socket][Channel][Dimm], LogicalDimm);
    }
  }

  // TYPE 19
  DmiBuffer->T19[0].StartingAddr    = 0;
  DmiBuffer->T19[0].ExtStartingAddr = 0;
  DmiBuffer->T19[0].ExtEndingAddr   = 0;

  // If Ending Address >= 0xFFFFFFFF, update Starting Address (offset 04h) & Ending Address (offset 08h) to 0xFFFFFFFF,
  // and use the Extended Starting Address (offset 0Fh) & Extended Ending Address (offset 17h) instead.
  Value64 = ((UINT64) TotalMemSize << 10) - 1;
  if (Value64 >= ((UINT64) 0xFFFFFFFF)) {
    DmiBuffer->T19[0].StartingAddr = 0xFFFFFFFFUL;
    DmiBuffer->T19[0].EndingAddr = 0xFFFFFFFFUL;
    // In Byte
    DmiBuffer->T19[0].ExtEndingAddr = LShiftU64 (Value64, 10);
  } else {
    // In KByte
    DmiBuffer->T19[0].EndingAddr = (UINT32) Value64;
  }

  DmiBuffer->T19[0].PartitionWidth = NumActiveDimms;

  return AGESA_SUCCESS;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  IntToString
 *
 *  Description:
 *    Translate UINT array to CHAR array.
 *
 *  Parameters:
 *    @param[in, out]    *String       Pointer to CHAR array
 *    @param[in]         *Integer      Pointer to UINT array
 *    @param[in]         SizeInByte    The size of UINT array
 *
 *  Processing:
 *
 */
VOID
STATIC
IntToString (
  IN OUT   CHAR8 *String,
  IN       UINT8 *Integer,
  IN       UINT8 SizeInByte
  )
{
  UINT8 Index;

  for (Index = 0; Index < SizeInByte; Index++) {
    *(String + Index * 2) = (*(Integer + Index) >> 4) & 0x0F;
    *(String + Index * 2 + 1) = *(Integer + Index) & 0x0F;
  }
  for (Index = 0; Index < (SizeInByte * 2); Index++) {
    if (*(String + Index) >= 0x0A) {
      *(String + Index) += 0x37;
    } else {
      *(String + Index) += 0x30;
    }
  }
  *(String + SizeInByte * 2) = 0x0;
}
