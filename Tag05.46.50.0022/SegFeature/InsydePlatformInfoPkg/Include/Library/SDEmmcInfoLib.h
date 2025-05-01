/** @file
  SD/eMMC Info Library Definition.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __SD_EMMC_INFO_LIB_H__
#define __SD_EMMC_INFO_LIB_H__

#include <Uefi.h>
#include <Protocol/H2OSDEmmcInfo.h>

typedef CHAR16* (*INFO_PARSER_FUNCTION) (SD_EMMC_INFO*); 

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
  );

/**
  Helper function to transfer Device Name Ascii string to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Device Data.

  @retval Device Name Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceNameStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to transfer Serial Number Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Device Data.

  @retval Serial Number Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceSerialNumStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to transfer Revision to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Revision Number Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceRevisionStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to Transfer OEM/Application ID to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval OEM/Application ID Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceOemAppIdStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to Transfer Manufacture ID to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Manufacture ID Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceManufactureIdStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

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
  );

/**
  Helper function to transfer Manufacture Date to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Manufacture Date Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceManufactureDateStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to Transfer Card Type to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Card Type Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceCardTypeStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

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
  );

/**
  Helper function to transfer Device Capacity to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Capacity Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceCapacityStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to retrieve Block Number from Device Data.

  @param[in]  CardInfo          SD/EMMC Device Data.

  @retval Block Number.

**/
UINTN
ParseSDEmmcDeviceBlockNum (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to Transfer Device Block Count to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Block Count Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceBlockNumStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to retrieve Block Size from Device Data.

  @param[in]  CardInfo          SD/EMMC Device Data.

  @retval Block Size.

**/
UINTN
ParseSDEmmcDeviceBlockSize (
  IN  SD_EMMC_INFO    *CardInfo
  );

/**
  Helper function to transfer Device Block Size to Unicode String Buffer.

  @param[in]  CardInfo  SD/EMMC Data.

  @retval Block Size Unicode String Buffer.

**/
CHAR16 *
SDEmmcDeviceBlockSizeStr (
  IN  SD_EMMC_INFO    *CardInfo
  );

#endif