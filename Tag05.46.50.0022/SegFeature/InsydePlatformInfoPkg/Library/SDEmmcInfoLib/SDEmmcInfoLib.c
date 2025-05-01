/** @file
  Implementation of SDEmmcInfoLib.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/SDEmmcInfoLib.h>

#include <Protocol/H2OSDEmmcInfo.h>


#define CAPACITY_1TB  1000000000000UL
#define CAPACITY_1GB  1000000000UL
#define CAPACITY_1MB  1000000UL

#define STR_SIZE      20

/**
  Helper function to retrieve Device Name from Device Data.

  @param[in]  CardInfo      SD/EMMC Device Data.
  @param[OUT] AsciiStrSize  Ascii String size of return buffer.

  @retval Ascii Device Name Buffer.

**/
UINT8 *
ParseSDEmmcDeviceName (
  IN  SD_EMMC_INFO    *CardInfo,
  OUT UINTN           *AsciiStrSize
  )
{
  CARD_TYPE   CardType;
  SD_CID      *SdCidData;
  EMMC_CID    *MmcCidData;
  UINT8       *DeviceName;

  DeviceName = NULL;

  CardType    = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    MmcCidData = &CardInfo->CardInfo->MmcCidData;
    (*AsciiStrSize) = sizeof (MmcCidData->ProductName);
    DeviceName  = (UINT8 *)AllocateCopyPool ((*AsciiStrSize), MmcCidData->ProductName);

    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    SdCidData = &CardInfo->CardInfo->SdCidData;
    (*AsciiStrSize) = sizeof (SdCidData->ProductName);
    DeviceName  = (UINT8 *)AllocateCopyPool ((*AsciiStrSize), SdCidData->ProductName);

    break;

  default:
    break;
  }

  return DeviceName;
}

/**
  Helper function to transfer Device Name Ascii string to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Device Data.

  @retval Device Name Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceNameStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINTN       AsciiSize;
  UINTN       Index;
  UINTN       Index1;
  UINT8       *AsciiStr;
  CHAR16      *DeviceName;

  DeviceName = NULL;

  AsciiStr    = ParseSDEmmcDeviceName (CardInfo, &AsciiSize);
  DeviceName  = AllocateZeroPool (sizeof (CHAR16) * (AsciiSize + 1));
  if ((DeviceName == NULL) || (AsciiStr == NULL)) {
    return L"(Out of Resource)";
  }

  Index1 = AsciiSize-1;
  for (Index = 0; Index < AsciiSize; Index++) {
    if (AsciiStr[Index] == ' ') {
      continue;
    }
    if (DeviceName != NULL) {
      DeviceName[(Index1-Index)] = AsciiStr[Index];
    }
  }
  return DeviceName;
}

/**
  Helper function to transfer Serial Number Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Device Data.

  @retval Serial Number Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceSerialNumStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINT8       *SerialNum;
  CHAR16      *SerialNumStr;
  CARD_TYPE   CardType;
  SD_CID      *SdCidData;
  EMMC_CID    *MmcCidData;

  SerialNum     = NULL;
  SerialNumStr  = NULL;

  SerialNumStr  = (CHAR16 *)AllocateZeroPool (sizeof (CHAR16) * (STR_SIZE));
  if (SerialNumStr == NULL) {
    return L"(Out of Resource)";
  }
  CardType      = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    MmcCidData    = &CardInfo->CardInfo->MmcCidData;
    SerialNum     = MmcCidData->ProductSerialNumber;
    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    SdCidData     = &CardInfo->CardInfo->SdCidData;
    SerialNum     = SdCidData->ProductSerialNumber;
    break;

  default:
    break;
  }

  if (SerialNum == NULL) {
    return L"Unsupport";
  }

  UnicodeSPrint (
    SerialNumStr,
    (sizeof (CHAR16) * (STR_SIZE)),
    L"%02x%02x%02x%02x",
    (UINTN)SerialNum[3],
    (UINTN)SerialNum[2],
    (UINTN)SerialNum[1],
    (UINTN)SerialNum[0]
    );

  return SerialNumStr;
}

/**
  Helper function to transfer Revision to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Revision Number Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceRevisionStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINT8       Revision;
  CHAR16      *RevisionStr;
  CARD_TYPE   CardType;
  SD_CID      *SdCidData;
  EMMC_CID    *MmcCidData;

  Revision = 0;

  RevisionStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (RevisionStr == NULL) {
    return L"(Out of Resource)";
  }
  CardType = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    MmcCidData = &CardInfo->CardInfo->MmcCidData;
    Revision   = MmcCidData->ProductRevision;
    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    SdCidData = &CardInfo->CardInfo->SdCidData;
    Revision = SdCidData->ProductRevision;
    break;

  default:
    break;
  }

  UnicodeSPrint (
    RevisionStr,
    STR_SIZE,
    L"%d.%d",
    (UINTN)((Revision & 0xF0) >> 4),
    (UINTN)(Revision & 0x0F)
    );

  return RevisionStr;
}

/**
  Helper function to Transfer OEM/Application ID to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval OEM/Application ID Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceOemAppIdStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINT16      OemIdBuffer;
  CHAR16      *OemIdStr;
  CARD_TYPE   CardType;
  SD_CID      *SdCidData;
  EMMC_CID    *MmcCidData;

  OemIdBuffer = 0;

  OemIdStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (OemIdStr == NULL) {
    return L"(Out of Resource)";
  }
  CardType = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    MmcCidData  = &CardInfo->CardInfo->MmcCidData;
    OemIdBuffer = MmcCidData->OemId;
    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    SdCidData = &CardInfo->CardInfo->SdCidData;
    OemIdBuffer = ((SdCidData->OemId[1] << 8) | SdCidData->OemId[0]);
    break;

  default:
    break;
  }

  UnicodeSPrint (
    OemIdStr,
    (sizeof (CHAR16) * STR_SIZE),
    L"%04x",
    OemIdBuffer
    );

  return OemIdStr;
}

/**
  Helper function to Transfer Manufacture ID to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Manufacture ID Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceManufactureIdStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINT8       ManufactureId;
  CHAR16      *ManufactureIdStr;
  CARD_TYPE   CardType;
  SD_CID      *SdCidData;
  EMMC_CID    *MmcCidData;

  ManufactureId = 0;

  ManufactureIdStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (ManufactureIdStr == NULL) {
    return L"(Out of Resource)";
  }
  CardType = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    MmcCidData = &CardInfo->CardInfo->MmcCidData;
    ManufactureId = MmcCidData->ManufacturerId;

    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    SdCidData = &CardInfo->CardInfo->SdCidData;
    ManufactureId = SdCidData->ManufacturerId;

    break;

  default:
    break;
  }

  UnicodeSPrint (
    ManufactureIdStr,
    STR_SIZE,
    L"%02x",
    (UINTN)ManufactureId
    );

  return ManufactureIdStr;
}

/**
  Helper function to retrieve Manufacture ID from Device Data.

  @param[in]  CardInfo          SD/EMMC Device Data.
  @param[OUT] ManufactureMonth  Manufacture Month.
  @param[OUT] ManufactureYear   Manufacture Year.

**/
VOID
ParseSDEmmcDeviceManufactureDate (
  IN  SD_EMMC_INFO    *CardInfo,
  OUT UINTN           *ManufactureMonth,
  OUT UINTN           *ManufactureYear
  )
{
  UINTN         BaseYear;
  CARD_TYPE     CardType;
  SD_CID        *SdCidData;
  EMMC_CID      *MmcCidData;
  EMMC_EXT_CSD  *MmcCsdExData;

  CardType = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    MmcCidData = &CardInfo->CardInfo->MmcCidData;
    MmcCsdExData = &CardInfo->CardInfo->MmcCsdExData;
    (*ManufactureMonth) = (MmcCidData->ManufacturingDate & 0xF0) >> 4;

    BaseYear = 1997;
    if (MmcCsdExData->ExtCsdRev > 4 && (MmcCidData->ManufacturingDate & 0x0F) < 0x0D) {
      BaseYear = 2003;
    }

    (*ManufactureYear) = BaseYear + (MmcCidData->ManufacturingDate & 0x0F);

    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    SdCidData = &CardInfo->CardInfo->SdCidData;
    (*ManufactureMonth) = SdCidData->ManufacturingDate & 0xF;
    (*ManufactureYear) = ((SdCidData->ManufacturingDate & 0xFF0) >> 4) + 2000;

    break;

  default:
    break;
  }

}

/**
  Helper function to transfer Manufacture Date to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Manufacture Date Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceManufactureDateStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINTN         ManufactureMonth;
  UINTN         ManufactureYear;
  CHAR16        *ManufactureDateStr;

  ManufactureDateStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (ManufactureDateStr == NULL) {
    return L"(Out of Resource)";
  }
  ParseSDEmmcDeviceManufactureDate (CardInfo, &ManufactureMonth, &ManufactureYear);

  UnicodeSPrint (
    ManufactureDateStr,
    STR_SIZE,
    L"%02d, %d",
    (UINTN)ManufactureMonth,
    (UINTN)ManufactureYear
    );

  return ManufactureDateStr;
}


/**
  Helper function to Transfer Card Type to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Card Type Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceCardTypeStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  CARD_TYPE     CardType;
  CHAR16        *CardTypeStr;


  CardTypeStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (CardTypeStr == NULL) {
    return L"(Out of Resource)";
  }
  CardType = CardInfo->CardInfo->CardType;

  switch (CardType) {
  case MMC_CARD:
    StrCpyS (CardTypeStr, STR_SIZE, L"eMMC");
    break;

  case SD_CARD:
  case SD_CARD_2:
  case SD_CARD_2_HIGH:
    StrCpyS (CardTypeStr, STR_SIZE, L"SD Card");

    break;

  default:
    StrCpyS (CardTypeStr, STR_SIZE, L"Unknown");
    break;
  }

  return CardTypeStr;
}

/**
  Helper function to retrieve Capacity from Device Data.

  @param[in]  CardInfo          SD/EMMC Device Data.
  @param[OUT] Integer           Capacity Integer.
  @param[OUT] Decimal           Capacity Decimal.

  @retval Unit Unicode String.

**/
CHAR16 *
ParseSDEmmcDeviceCapacity (
  IN  SD_EMMC_INFO    *CardInfo,
  OUT UINT64          *Integer,
  OUT UINT32          *Decimal
  )
{
  EFI_BLOCK_IO_PROTOCOL  *BlockIo;
  UINT32                  Divisor;
  UINT64                  Capacity;
  CHAR16                  *UnitStr;

  Divisor = 1;
  UnitStr = NULL;

  BlockIo = CardInfo->BlockIo;

  Capacity = MultU64x32 ((BlockIo->Media->LastBlock + 1), BlockIo->Media->BlockSize);

  if (Capacity >= CAPACITY_1GB) {
    Divisor = CAPACITY_1GB;
    UnitStr = L"GB";
  } else if ((Capacity >= CAPACITY_1MB) && (Capacity < CAPACITY_1GB)) {
    Divisor = CAPACITY_1MB;
    UnitStr = L"MB";
  }

  (*Integer) = DivU64x32 (Capacity, Divisor);
  (*Decimal) = ModU64x32 (Capacity, Divisor);
  Divisor /= 100;
  (*Decimal) /= Divisor;

  return UnitStr;
}

/**
  Helper function to transfer Device Capacity to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Capacity Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceCapacityStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  UINT64                  Integer;
  UINT32                  Decimal;
  CHAR16                  *UnitStr;
  CHAR16                  *CapacityStr;


  CapacityStr = AllocateZeroPool ((sizeof (CHAR16) * STR_SIZE) + sizeof (CHAR16));
  if (CapacityStr == NULL) {
    return L"(Out of Resource)";
  }
  UnitStr     = ParseSDEmmcDeviceCapacity (CardInfo, &Integer, &Decimal);

  UnicodeSPrint (
    CapacityStr,
    STR_SIZE,
    L"%d.%02d %s",
    (UINTN)Integer,
    (UINTN)Decimal,
    UnitStr
    );

  return CapacityStr;
}

/**
  Helper function to retrieve Block Number from Device Data.

  @param[in]  CardInfo          SD/EMMC Device Data.

  @retval Block Number.

**/
UINTN
ParseSDEmmcDeviceBlockNum (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  EFI_BLOCK_IO_PROTOCOL   *BlockIo;

  BlockIo = CardInfo->BlockIo;

  return (UINTN)(BlockIo->Media->LastBlock + 1);
}

/**
  Helper function to Transfer Device Block Count to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Block Count Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceBlockNumStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  CHAR16                  *BlockCountStr;

  BlockCountStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (BlockCountStr == NULL) {
    return L"(Out of Resource)";
  }

  UnicodeSPrint (
    BlockCountStr,
    STR_SIZE,
    L"%ld",
    ParseSDEmmcDeviceBlockNum (CardInfo)
    );

  return BlockCountStr;
}

/**
  Helper function to retrieve Block Size from Device Data.

  @param[in]  CardInfo          SD/EMMC Device Data.

  @retval Block Size.

**/
UINTN
ParseSDEmmcDeviceBlockSize (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  EFI_BLOCK_IO_PROTOCOL   *BlockIo;

  BlockIo = CardInfo->BlockIo;

  return BlockIo->Media->BlockSize;
}

/**
  Helper function to transfer Device Block Size to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Block Size Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceBlockSizeStr (
  IN  SD_EMMC_INFO    *CardInfo
  )
{
  CHAR16                  *BlockSizeStr;

  BlockSizeStr = AllocateZeroPool (sizeof (CHAR16) * STR_SIZE);
  if (BlockSizeStr == NULL) {
    return L"(Out of Resource)";
  }

  UnicodeSPrint (
    BlockSizeStr,
    STR_SIZE,
    L"%d Bytes",
    ParseSDEmmcDeviceBlockSize (CardInfo)
    );

  return BlockSizeStr;
}
