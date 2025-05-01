/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/TimerLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IdsLib.h>

#include "AmdUsb4.h"
#include "Library/AmdUsb4Lib.h"
#include <AmdUsb4/Drom.h>
#include <AmdUsb4/ControlPacket.h>
#include <AmdUsb4/ConfigurationSpace.h>

#define FILECODE LIBRARY_AMDUSB4LIB_DROM_FILECODE

//#define ALIGN32(v, a) (UINT32)((((v) - 1) | ((a) - 1)) + 1)

#define AMD_USB4_DROM_UNUSED_ADAPTER_ENTRY_LENGTH                 2
#define AMD_USB4_DROM_DP_ADAPTER_ENTRY_LENGTH                     5
#define AMD_USB4_DROM_TBT3_LANE_ADAPTER_ENTRY_LENGTH              8
#define AMD_USB4_DROM_TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY_LENGTH     11
#define AMD_USB4_DROM_TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY_LENGTH   3

#define AMD_USB4_DROM_ASCII_VENDOR_NAME         1
#define AMD_USB4_DROM_ASCII_MODEL_NAME          2
#define AMD_USB4_DROM_TMU_MIN_REQUESTED_MODE    8
#define AMD_USB4_DROM_PRODUCT_DESCRIPTOR        9
#define AMD_USB4_DROM_SERIAL_NUMBER             0xA
#define AMD_USB4_DROM_USB_PORT_MAPPING          0xB
#define AMD_USB4_DROM_UTF16_VENDOR_NAME         0xC
#define AMD_USB4_DROM_UTF16_MODEL_NAME          0xD

char *DromGenericString[14] = {
  "Reserved",                   // 0
  "ASCII Vendor Name",          // 1
  "ASCII Model Name",           // 2
  "3", "4", "5", "6", "7",      // 3, 4, 5, 6, 7
  "TMU Minimum Requested Mode", // 8
  "Product Descriptor",         // 9
  "Serial Number",              // A
  "USB Port Mapping",           // B
  "UTF16 Vendor Name",          // C
  "UTF16 Model Name"            // D
};


UINT8
ThunderboltCRC8 (
  IN          UINT8 *dwp,
  IN          INT32 len
  )
{
  UINT8 crc8;
  INT32 i;
  INT32 b;

  crc8 = 0xFF;
  i = 0;
  b = 0;

  for ( i = 0; i < len; i++ ) {
    crc8 ^= dwp[i];
    for ( b = 0; b < 8; b++ ) {
      if ( crc8 & 0x80 ) {
        crc8 <<= 1;
        crc8 ^= 0x07;// x^8 + x^2 + x + 1
      } else {
        crc8 <<= 1;
      }
    }
  }

  return crc8;
}


STATIC UINT32 CrcTable[256] = {
  0x00000000,0xF26B8303,0xE13B70F7,0x1350F3F4,
  0xC79A971F,0x35F1141C,0x26A1E7E8,0xD4CA64EB,
  0x8AD958CF,0x78B2DBCC,0x6BE22838,0x9989AB3B,
  0x4D43CFD0,0xBF284CD3,0xAC78BF27,0x5E133C24,
  0x105EC76F,0xE235446C,0xF165B798,0x030E349B,
  0xD7C45070,0x25AFD373,0x36FF2087,0xC494A384,
  0x9A879FA0,0x68EC1CA3,0x7BBCEF57,0x89D76C54,
  0x5D1D08BF,0xAF768BBC,0xBC267848,0x4E4DFB4B,
  0x20BD8EDE,0xD2D60DDD,0xC186FE29,0x33ED7D2A,
  0xE72719C1,0x154C9AC2,0x061C6936,0xF477EA35,
  0xAA64D611,0x580F5512,0x4B5FA6E6,0xB93425E5,
  0x6DFE410E,0x9F95C20D,0x8CC531F9,0x7EAEB2FA,
  0x30E349B1,0xC288CAB2,0xD1D83946,0x23B3BA45,
  0xF779DEAE,0x05125DAD,0x1642AE59,0xE4292D5A,
  0xBA3A117E,0x4851927D,0x5B016189,0xA96AE28A,
  0x7DA08661,0x8FCB0562,0x9C9BF696,0x6EF07595,
  0x417B1DBC,0xB3109EBF,0xA0406D4B,0x522BEE48,
  0x86E18AA3,0x748A09A0,0x67DAFA54,0x95B17957,
  0xCBA24573,0x39C9C670,0x2A993584,0xD8F2B687,
  0x0C38D26C,0xFE53516F,0xED03A29B,0x1F682198,
  0x5125DAD3,0xA34E59D0,0xB01EAA24,0x42752927,
  0x96BF4DCC,0x64D4CECF,0x77843D3B,0x85EFBE38,
  0xDBFC821C,0x2997011F,0x3AC7F2EB,0xC8AC71E8,
  0x1C661503,0xEE0D9600,0xFD5D65F4,0x0F36E6F7,
  0x61C69362,0x93AD1061,0x80FDE395,0x72966096,
  0xA65C047D,0x5437877E,0x4767748A,0xB50CF789,
  0xEB1FCBAD,0x197448AE,0x0A24BB5A,0xF84F3859,
  0x2C855CB2,0xDEEEDFB1,0xCDBE2C45,0x3FD5AF46,
  0x7198540D,0x83F3D70E,0x90A324FA,0x62C8A7F9,
  0xB602C312,0x44694011,0x5739B3E5,0xA55230E6,
  0xFB410CC2,0x092A8FC1,0x1A7A7C35,0xE811FF36,
  0x3CDB9BDD,0xCEB018DE,0xDDE0EB2A,0x2F8B6829,
  0x82F63B78,0x709DB87B,0x63CD4B8F,0x91A6C88C,
  0x456CAC67,0xB7072F64,0xA457DC90,0x563C5F93,
  0x082F63B7,0xFA44E0B4,0xE9141340,0x1B7F9043,
  0xCFB5F4A8,0x3DDE77AB,0x2E8E845F,0xDCE5075C,
  0x92A8FC17,0x60C37F14,0x73938CE0,0x81F80FE3,
  0x55326B08,0xA759E80B,0xB4091BFF,0x466298FC,
  0x1871A4D8,0xEA1A27DB,0xF94AD42F,0x0B21572C,
  0xDFEB33C7,0x2D80B0C4,0x3ED04330,0xCCBBC033,
  0xA24BB5A6,0x502036A5,0x4370C551,0xB11B4652,
  0x65D122B9,0x97BAA1BA,0x84EA524E,0x7681D14D,
  0x2892ED69,0xDAF96E6A,0xC9A99D9E,0x3BC21E9D,
  0xEF087A76,0x1D63F975,0x0E330A81,0xFC588982,
  0xB21572C9,0x407EF1CA,0x532E023E,0xA145813D,
  0x758FE5D6,0x87E466D5,0x94B49521,0x66DF1622,
  0x38CC2A06,0xCAA7A905,0xD9F75AF1,0x2B9CD9F2,
  0xFF56BD19,0x0D3D3E1A,0x1E6DCDEE,0xEC064EED,
  0xC38D26C4,0x31E6A5C7,0x22B65633,0xD0DDD530,
  0x0417B1DB,0xF67C32D8,0xE52CC12C,0x1747422F,
  0x49547E0B,0xBB3FFD08,0xA86F0EFC,0x5A048DFF,
  0x8ECEE914,0x7CA56A17,0x6FF599E3,0x9D9E1AE0,
  0xD3D3E1AB,0x21B862A8,0x32E8915C,0xC083125F,
  0x144976B4,0xE622F5B7,0xF5720643,0x07198540,
  0x590AB964,0xAB613A67,0xB831C993,0x4A5A4A90,
  0x9E902E7B,0x6CFBAD78,0x7FAB5E8C,0x8DC0DD8F,
  0xE330A81A,0x115B2B19,0x020BD8ED,0xF0605BEE,
  0x24AA3F05,0xD6C1BC06,0xC5914FF2,0x37FACCF1,
  0x69E9F0D5,0x9B8273D6,0x88D28022,0x7AB90321,
  0xAE7367CA,0x5C18E4C9,0x4F48173D,0xBD23943E,
  0xF36E6F75,0x0105EC76,0x12551F82,0xE03E9C81,
  0x34F4F86A,0xC69F7B69,0xD5CF889D,0x27A40B9E,
  0x79B737BA,0x8BDCB4B9,0x988C474D,0x6AE7C44E,
  0xBE2DA0A5,0x4C4623A6,0x5F16D052,0xAD7D5351
};

UINT32 DROMCrc32_8 (
  IN      UINT32  crc,
  IN      UINT32  b
  )
{
  INT32 ndx;
  ndx = (crc ^ b) & 0xFF;
  return (crc>>8) ^ CrcTable[ndx];
}


UINT32 DROMCrc32 (
  IN      UINT8   *dwp,
  IN      INT32   len
  )
{
  UINT32  crc32c;
  UINT8   *bp;
  int     i;

  crc32c  = 0xFFFFFFFFL;
  bp      = dwp;
  i       = 0;

  for (i=0; i<len; i++) {
    crc32c = DROMCrc32_8 (crc32c, bp[i]);
  }

  return crc32c ^ 0xFFFFFFFFL;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Read DROM
 *
 * @param[in] *pCM            Point to global structure AMD_USB4_CM
 * @param[in] *pRouter        Point to Router
 *
 * @retval EFI_SUCCESS              Success
 * @retval EFI_INVALID_PARAMETER    Input parameters is invalid
 * @retval EFI_DEVICE_ERROR         Device error
 * @retval EFI_UNSUPPORTED          Router do not support DROM or it is not a USB4 router
 * @retval EFI_OUT_OF_RESOURCES     Not enough memory hold DROM image
**/
EFI_STATUS
ReadDRom (
  IN  AMD_USB4_CM         *pCM,
  IN  AMD_USB4_CM_ROUTER  *pRouter
  )
{
  EFI_STATUS        Status;
  UINT32            CurrentDromOffset;
  AMD_DROM_METADATA Metadata;
  UINT32            CompletionMetadata;
  UINT32            PendingData;
  UINT32            DromReadSize;
  UINT32            DromLength;
  UINT8             UnusedAdapterCount;
  UINT8             DpAdapterCount;
  UINT8             TBT3LaneAdaptercount;
  UINT8             TBT3PcieUpstreamAdapterCount;
  UINT8             TBT3PcieDownstreamAdapterCount;
  UINT8             EntryLength;
  UINT32            *PtrTempData;
  UINT8             GenericType;
  UINT8             AdapterType;
  UINT8             UsbportMapLength;
  UINT8             Index;
  DROM_HEADER       *PtrDromHeader;
  TBT3_IDENTIFICATION_SECTION *PtrTbt3IdSection;
  UINT32            Temp32;
  UINT8             Temp8;
  UINT32            CompletionStatus;
  UINT8             Data8[64];
  UINT32            Offset;
  UINT8             *PtrDRom;

  AMD_USB4_CM_HI_ADAPTER       HostInterfaceAdp;

  CompletionMetadata              = 0;
  Status                          = EFI_SUCCESS;
  DromReadSize                    = 0;
  UnusedAdapterCount              = 0;
  DpAdapterCount                  = 0;
  TBT3LaneAdaptercount            = 0;
  TBT3PcieUpstreamAdapterCount    = 0;
  TBT3PcieDownstreamAdapterCount  = 0;
  PendingData                     = 0;
  CurrentDromOffset               = 0;
  UsbportMapLength                = 0;
  GenericType                     = 0;
  AdapterType                     = 0;
  PtrDromHeader                   = NULL;
  PtrTbt3IdSection                = NULL;
  Temp32                          = 0;
  Temp8                           = 0;
  CompletionStatus                = 0;
  PtrTempData                     = NULL;
  Offset                          = 0;
  PtrDRom                         = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Entry\n", __FUNCTION__);

  if ((pCM == NULL) || (pRouter == NULL)) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   Invalid input parameters\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( !IsUSB4Router (pRouter) && ( 0 == pRouter->Tbt3DROMBase ) ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   TBT3 Router do not have DROM.\n");
    return EFI_UNSUPPORTED;
  }

  PtrDRom = pCM->PtrDROMBuffer;

  if ( NULL == PtrDRom ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   No Buffer for DROM data.\n");
    return EFI_UNSUPPORTED;
  }

  ZeroMem (Data8, sizeof (Data8));

  if ( !IsUSB4Router (pRouter) ) {
    Status = Tbt3FindHostInterfaceAdapter (
               pCM,
               pRouter,
               &HostInterfaceAdp
               );
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("   TBT3 Router do not have Host Interface adapter.\n");
      return EFI_UNSUPPORTED;
    }
  }

  //
  // 1. Read drom header - First 4 DW's
  //
  if ( IsUSB4Router (pRouter) ) {
    Metadata.Value = 0;
    Metadata.Field.Address  = 0;
    Metadata.Field.ReadSize = sizeof (DROM_HEADER) >> 2;

    /*
    Status = RouterOperation (
               pCM,
               pRouter,
               DROM_READ,
               Metadata.Value,
               &CompletionMetadata,
               &PtrTempData,
               &CompletionStatus
               );
    */
    Status = RouterOperation2 (
               pCM,
               pRouter,
               DROM_READ,
               Metadata.Value,
               0,
               NULL,
               &CompletionMetadata,
               &CompletionStatus,
               &PtrTempData
               );

    IDS_HDT_CONSOLE_USB4_DEBUG ("   Read DROM Header Status: %r, CompletionStatus: 0x%x\n", Status, CompletionStatus);
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to Read DROM Header. Status: %r\n", Status);
      return Status;
    }

    if ( CompletionStatus != 0 ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Completion Status is not 0. Seems this Router do not support DROM.\n");
      return EFI_UNSUPPORTED;
    }
  } else {
    // We need to consider 4 bytes alignment because TBT3 DROM base address is in range 1 to 0xFFFF.
    Offset            = pRouter->Tbt3DROMBase + 0;
    CurrentDromOffset = Offset & (~3);                            // 4 bytes alignment
    DromReadSize      = sizeof (DROM_HEADER) + (Offset & 3);
    Status = TBT3ReadDrom (
               pCM,
               &HostInterfaceAdp,
               CurrentDromOffset,
               DromReadSize,
               (UINT32*)Data8
               );
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Read TBT3 DROM Header Status: %r\n", Status);
    if ( EFI_ERROR (Status) ) {
      IDS_HDT_CONSOLE_USB4_ERROR ("   Failed to Read TBT3 DROM Header. Status: %r\n", Status);
      return EFI_DEVICE_ERROR;
    } else {
      PtrTempData = (UINT32*)(Data8 + (Offset & 3));
    }
  }

  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("...Dump DROM Header >>>\n");
    Usb4DumpData (PtrTempData, sizeof (DROM_HEADER));
    IDS_HDT_CONSOLE_USB4_DEBUG ("...Dump DROM Header <<<\n");
  }

  PtrDromHeader = (DROM_HEADER*)PtrTempData;
  IDS_HDT_CONSOLE_USB4_DEBUG ("   DROM version: %d\n", PtrDromHeader->Version);

  //
  // 2. Check DROM version
  //
  if ( IsUSB4Router (pRouter) && PtrDromHeader->Version == 2 ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Get USB4 DROM\n");
  } else {
    PtrTbt3IdSection = (TBT3_IDENTIFICATION_SECTION*)PtrTempData;
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Get USB4 DROM with TBT3-Compatibility, UUID: 0x%lx\n", PtrTbt3IdSection->UUID);
    Temp8 = ThunderboltCRC8 (
              (UINT8*)&(PtrTbt3IdSection->UUID),
              sizeof (PtrTbt3IdSection->UUID)
              );
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Expect Crc8 value 0x%x, get value 0x%x\n", PtrTbt3IdSection->Crc8, Temp8);
    if ( Temp8 == PtrTbt3IdSection->Crc8 ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("   TBT3-Identification Section CRC8 is correct\n");
    } else {
      IDS_HDT_CONSOLE_USB4_ERROR ("   TBT3-Identification Section CRC8 is NOT correct. Quit\n");
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // 3. Check The Length of Drom Structure in 4th Dword Bit [11:0] masking other bits
  //    Adding 13 bytes to include crc and Reserved as Drom length starts only from Version.
  //
  DromLength  = (UINT32)PtrDromHeader->Length + 13; // USB4 and TBT3 DROM has same header size 13.
                                                    // DromLength will have total length in bytes.
  if ( DromLength > MAX_CM_DROM_BUFFER_SIZE ) {
    IDS_HDT_CONSOLE_USB4_ERROR (
      "   The Router (0x%lx) DROM size is %d, it is big than %d. CM cannot handle it\n",
      pRouter->Header.TopologyID,
      DromLength,
      MAX_CM_DROM_BUFFER_SIZE
      );
    return EFI_OUT_OF_RESOURCES;
  }

  //AGESA_TESTPOINT (0xb0b4, NULL);

  IDS_HDT_CONSOLE_USB4_DEBUG ("   DROM has total %d bytes\n", DromLength);

  //
  // 4. Copy all DROM data to pCM->PtrDROMBuffer
  //
  ZeroMem (PtrDRom, MAX_CM_DROM_BUFFER_SIZE);
  CurrentDromOffset = 0;
  PendingData       = DromLength;

  // Tbt3 Router
  if ( !IsUSB4Router (pRouter) ) {
    CurrentDromOffset +=  pRouter->Tbt3DROMBase;
    CurrentDromOffset &=  ~3;
    PendingData       +=  pRouter->Tbt3DROMBase & 3;

    if ( PendingData > MAX_CM_DROM_BUFFER_SIZE ) {
      IDS_HDT_CONSOLE_USB4_ERROR (
        "   The Router (0x%lx) DROM size is %d, it is big than %d. CM cannot handle it\n",
        pRouter->Header.TopologyID,
        PendingData,
        MAX_CM_DROM_BUFFER_SIZE
        );
      return EFI_OUT_OF_RESOURCES;
    }
  }
  Offset = CurrentDromOffset;

  do {
    //
    // If the Length is greater than or equal to 64 read 16 dw's  else read required dw's
    //
    DromReadSize = ( PendingData >= 64 ) ? 64 : PendingData;

    if ( IsUSB4Router (pRouter) ) {
      Metadata.Value = 0;
      Metadata.Field.Address  = CurrentDromOffset >> 2;
      Metadata.Field.ReadSize = ALIGN_VALUE (DromReadSize, 4) >> 2;
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "   Read DROM. Address in DW: %d, ReadSize in DW: %d\n",
        Metadata.Field.Address,
        Metadata.Field.ReadSize
        );

      /*
      Status = RouterOperation (
                 pCM,
                 pRouter,
                 DROM_READ,
                 Metadata.Value,
                 &CompletionMetadata,
                 &PtrTempData,
                 &CompletionStatus
                 );
      */
      Status = RouterOperation2 (
                 pCM,
                 pRouter,
                 DROM_READ,
                 Metadata.Value,
                 0,
                 NULL,
                 &CompletionMetadata,
                 &CompletionStatus,
                 &PtrTempData
                 );

      if (EFI_ERROR (Status) || ( CompletionStatus != 0 ) ) {
        IDS_HDT_CONSOLE_USB4_ERROR (
          "   Failed to Read DROM data. Status: %r, ReturnStatus: 0x%x\n",
          Status,
          CompletionStatus
          );
        return EFI_DEVICE_ERROR;
      }
    } else {
      ZeroMem (Data8, sizeof (Data8));
      Status = TBT3ReadDrom (
                 pCM,
                 &HostInterfaceAdp,
                 CurrentDromOffset,             // Offset in byte
                 DromReadSize,                  // Size in byte
                 (UINT32*)Data8
                 );
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Read TBT3 DROM Data Status: %r\n", Status);
      if ( EFI_ERROR (Status) ) {
        IDS_HDT_CONSOLE_USB4_DEBUG ("   Failed to Read Tbt3 DROM data Status: %r\n", Status);
        return EFI_DEVICE_ERROR;
      } else {
        PtrTempData = (UINT32*)Data8;
      }
    }

    CopyMem ((VOID*)(PtrDRom + CurrentDromOffset - Offset), (VOID*)PtrTempData, DromReadSize);

    CurrentDromOffset += DromReadSize;
    PendingData       -= DromReadSize;
  } while (PendingData);

  if ( !IsUSB4Router (pRouter) ) {
    PtrDRom = (UINT8*)(PtrDRom + (pRouter->Tbt3DROMBase & 3));
  }

  PtrDromHeader = (DROM_HEADER*)PtrDRom;

  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("...Dump DROM data >>>\n");
    Usb4DumpData (PtrDRom, DromLength);
    IDS_HDT_CONSOLE_USB4_DEBUG ("...Dump DROM data <<<\n");
  }

  //
  // 5. Calculate CRC
  //
  Temp32 = DROMCrc32 ((UINT8*)(PtrDRom + 13), (INT32)(PtrDromHeader->Length));
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Expect Crc32 value 0x%x, get value 0x%x\n", PtrDromHeader->Crc32, Temp32);

  if ( Temp32 != PtrDromHeader->Crc32 ) {
    IDS_HDT_CONSOLE_USB4_ERROR ("   CRC32 is NOT correct. Quit\n");
    return EFI_DEVICE_ERROR;
  }

  //
  // 6. Analyze DROM
  //
  pRouter->DromVersion = PtrDromHeader->Version;

  // WA for some AR TBT3 router DROM version is 2.
  if ( !IsUSB4Router (pRouter) ) {
    pRouter->DromVersion = 1;
  }

  if (pRouter->DromVersion == 2) {
    CurrentDromOffset = sizeof (DROM_HEADER);
  } else {
    CurrentDromOffset = sizeof (TBT3_HEADER_SECTION) + sizeof (TBT3_IDENTIFICATION_SECTION);
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   DRom Ver: %d, CurrentDromOffset = %d (0x%x), DromLength = %d\n",
    pRouter->DromVersion,
    CurrentDromOffset,
    CurrentDromOffset,
    DromLength
    );

  while (CurrentDromOffset < DromLength) {
    EntryLength = *(UINT8*)(PtrDRom + CurrentDromOffset);
    AdapterType = (*(UINT8*)(PtrDRom + CurrentDromOffset + 1)) >> 7;
    IDS_HDT_CONSOLE_USB4_DEBUG ("   EntryLength = %d, AdapterType = %d\n", EntryLength, AdapterType);
    if ( AdapterType ) {
      switch (EntryLength) {
        case AMD_USB4_DROM_UNUSED_ADAPTER_ENTRY_LENGTH:  // unused adapter entry size 2 bytes

          if ( UnusedAdapterCount >= MAX_UNUSED_ADAPTERS ) {
            IDS_HDT_CONSOLE_USB4_ERROR ("   Unused Adapter Count reach max count (%d)\n", MAX_UNUSED_ADAPTERS);
            CurrentDromOffset += EntryLength;
            continue;
          }

          // Host Interface adapter is always in TBT3 DROM Unused Adapters list
          // But we need it for DP Path setup.
          // So just skip TBT3 Host Interface adapter.
          if ( !IsUSB4Router (pRouter) &&
            HostInterfaceAdp.Header.AdapterNum == ((UNUSED_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber )
          {
            IDS_HDT_CONSOLE_USB4_DEBUG (
              "TBT3 Router %lx Adapter %d is Host Interface adapter, not unused adapter.\n",
              pRouter->Header.TopologyID,
              ((UNUSED_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber
              );
            CurrentDromOffset += EntryLength;
            continue;
          }

          if ( ((UNUSED_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterDisabled ) {
            pRouter->UnusedAdapEntry[UnusedAdapterCount].Field.AdapterNumber  =
              ((UNUSED_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber;
            UnusedAdapterCount++;
          }
          break;
        case AMD_USB4_DROM_DP_ADAPTER_ENTRY_LENGTH:  // dp adapter entry size 5 bytes

          if ( DpAdapterCount >= MAX_DP_ADAPTERS ) {
            IDS_HDT_CONSOLE_USB4_ERROR ("   DP Adapter Count reach max count (%d)\n", MAX_DP_ADAPTERS);
            CurrentDromOffset += EntryLength;
            continue;
          }

          if ( !((DP_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterDisabled
            && ((DP_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->PreferedValid ) {
            pRouter->DpAdapEntry[DpAdapterCount].Field.AdapterNumber =
              ((DP_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber;
            pRouter->DpAdapEntry[DpAdapterCount].Field.PA            =
              ((DP_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->PreferedLaneAdapter;
            pRouter->DpAdapEntry[DpAdapterCount].Field.PV            =
              ((DP_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->PreferedValid;
            DpAdapterCount++;
          }
          break;
        case AMD_USB4_DROM_TBT3_LANE_ADAPTER_ENTRY_LENGTH:  // Tbt3 Lane adapter entry size 8 bytes

          if ( TBT3LaneAdaptercount >= MAX_TBT3_ADAPTERS ) {
            IDS_HDT_CONSOLE_USB4_ERROR ("   TBT3 Lane Adapter Count reach max count (%d)\n", MAX_TBT3_ADAPTERS);
            CurrentDromOffset += EntryLength;
            continue;
          }

          if ( !((TBT3_LANE_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterDisabled ) {
            pRouter->Tbt3LaneAdapEntry[TBT3LaneAdaptercount].Field.AdapterNumber  =
              ((TBT3_LANE_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber;
            pRouter->Tbt3LaneAdapEntry[TBT3LaneAdaptercount].Field.L1A            =
            ((TBT3_LANE_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->Lane1Adapter;
            pRouter->Tbt3LaneAdapEntry[TBT3LaneAdaptercount].Field.DLC            =
              ((TBT3_LANE_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->DualLaneLinkCapable;
            pRouter->Tbt3LaneAdapEntry[TBT3LaneAdaptercount].Field.SecAdapterNum  =
              ((TBT3_LANE_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->SecAdapNum;
            TBT3LaneAdaptercount++;
          }
          break;
        case AMD_USB4_DROM_TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY_LENGTH:  // Tbt3 Up Stream adapter entry size 11 bytes

          if ( TBT3PcieUpstreamAdapterCount >= MAX_TBT3_ADAPTERS ) {
            IDS_HDT_CONSOLE_USB4_ERROR ("   TBT3 Upstream Adapter Count reach max count (%d)\n", MAX_TBT3_ADAPTERS);
            CurrentDromOffset += EntryLength;
            continue;
          }

          if ( !((TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterDisabled ) {
            pRouter->Tbt3UpStreamAdapEntry[TBT3PcieUpstreamAdapterCount].Field.AdapterNumber  =
              ((TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber;
            pRouter->Tbt3UpStreamAdapEntry[TBT3PcieUpstreamAdapterCount].Field.Fn             =
              ((TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->FunctionNumber;
            pRouter->Tbt3UpStreamAdapEntry[TBT3PcieUpstreamAdapterCount].Field.Dev            =
              (((TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->DevHi)<<3 |
              ((TBT3_PCIE_UPSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->DevLo;
            TBT3PcieUpstreamAdapterCount++;
          }
          break;
        case AMD_USB4_DROM_TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY_LENGTH:  // Tbt3 Down Stream adapter entry size 3 bytes

          if ( TBT3PcieDownstreamAdapterCount >= MAX_TBT3_ADAPTERS ) {
            IDS_HDT_CONSOLE_USB4_ERROR ("   TBT3 Downstream Adapter Count reach max count (%d)\n", MAX_TBT3_ADAPTERS);
            CurrentDromOffset += EntryLength;
            continue;
          }

          if ( !((TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterDisabled ) {
            pRouter->Tbt3DownStreamAdapEntry[TBT3PcieDownstreamAdapterCount].Field.AdapterNumber  =
              ((TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->AdapterNumber;
            pRouter->Tbt3DownStreamAdapEntry[TBT3PcieDownstreamAdapterCount].Field.Fn             =
              ((TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->FunctionNumber;
            pRouter->Tbt3DownStreamAdapEntry[TBT3PcieDownstreamAdapterCount].Field.Dev            =
              (((TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->DevHi)<<3 |
              ((TBT3_PCIE_DOWNSTREAM_ADAPTER_ENTRY*)(PtrDRom + CurrentDromOffset))->DevLo;
            TBT3PcieDownstreamAdapterCount++;
          }
          break;
        default:
          IDS_HDT_CONSOLE_USB4_ERROR ("   Entry Length is %d. Cannot handle it.\n", EntryLength);
          break;
      }
    } else {
      GenericType = (*(UINT8*)(PtrDRom + CurrentDromOffset + 1)) & 0x3f;
      IDS_HDT_CONSOLE_USB4_DEBUG ("   GenericType = %d\n", GenericType);
      switch (GenericType) {
        case AMD_USB4_DROM_TMU_MIN_REQUESTED_MODE:
          pRouter->DromTMU.Field.TMUMode        =
            ((TMU_MINIMUM_REQUESTMODE_ENTRY*)(PtrDRom + CurrentDromOffset))->TMUMode;
          pRouter->DromTMU.Field.TMURefreshRate =
            ((TMU_MINIMUM_REQUESTMODE_ENTRY*)(PtrDRom + CurrentDromOffset))->TMURefreshRate;
          IDS_HDT_CONSOLE_USB4_DEBUG (
            "   TMU Mode: %d, TMU Rate: %d\n",
            pRouter->DromTMU.Field.TMUMode,
            pRouter->DromTMU.Field.TMURefreshRate
            );
          break;
        case AMD_USB4_DROM_USB_PORT_MAPPING:
          UsbportMapLength = ( EntryLength - 2 ) / sizeof (AMD_USB4_PORT_MAP_ENTRY);
          IDS_HDT_CONSOLE_USB4_DEBUG ("   Usb Port Mapping Entry (Total: %d):\n", UsbportMapLength);
          for (Index = 0; Index < UsbportMapLength; Index++) {
            pRouter->Usb3PortMap[Index].Field.UsbPortNumber           =
              (UINT32)(((AMD_USB4_PORT_MAP_ENTRY*)(PtrDRom +
              CurrentDromOffset +
              (Index * sizeof (AMD_USB4_PORT_MAP_ENTRY))))->Usb3PortNumber.Field.Usb3PortNumber);
            pRouter->Usb3PortMap[Index].Field.PdPortNumber            =
              (UINT32)(((AMD_USB4_PORT_MAP_ENTRY*)(PtrDRom +
              CurrentDromOffset +
              (Index * sizeof (AMD_USB4_PORT_MAP_ENTRY))))->PdPortNumber.Field.PDPortNumber);
            pRouter->Usb3PortMap[Index].Field.TC                      =
              (UINT32)(((AMD_USB4_PORT_MAP_ENTRY*)(PtrDRom +
              CurrentDromOffset +
              (Index * sizeof (AMD_USB4_PORT_MAP_ENTRY))))->PdPortNumber.Field.USbTypeC);
            pRouter->Usb3PortMap[Index].Field.Usb3DownStreamAdpNumber =
              (UINT32)(((AMD_USB4_PORT_MAP_ENTRY*)(PtrDRom +
              CurrentDromOffset +
              (Index * sizeof (AMD_USB4_PORT_MAP_ENTRY))))->Usb3AdapterNumber.Field.USB3AdapterNumber);
            pRouter->Usb3PortMap[Index].Field.TS                      =
              (UINT32)(((AMD_USB4_PORT_MAP_ENTRY*)(PtrDRom +
              CurrentDromOffset +
              (Index * sizeof (AMD_USB4_PORT_MAP_ENTRY))))->Usb3AdapterNumber.Field.TunnelingSupport);
            IDS_HDT_CONSOLE_USB4_DEBUG (
              "      #%d Usb Port Number: 0x%x, Pd Port Number: 0x%x, TC: %d, Usb3 Adapter Number: 0x%x, TS: %d\n",
              Index,
              pRouter->Usb3PortMap[Index].Field.UsbPortNumber,
              pRouter->Usb3PortMap[Index].Field.PdPortNumber,
              pRouter->Usb3PortMap[Index].Field.TC,
              pRouter->Usb3PortMap[Index].Field.Usb3DownStreamAdpNumber,
              pRouter->Usb3PortMap[Index].Field.TS
              );
          }
          break;
        case AMD_USB4_DROM_ASCII_VENDOR_NAME:
        case AMD_USB4_DROM_ASCII_MODEL_NAME:
        case AMD_USB4_DROM_PRODUCT_DESCRIPTOR:
        case AMD_USB4_DROM_SERIAL_NUMBER:
        case AMD_USB4_DROM_UTF16_VENDOR_NAME:
        case AMD_USB4_DROM_UTF16_MODEL_NAME:
          IDS_HDT_CONSOLE_USB4_DEBUG ("   Get Generic Entry: %a\n", DromGenericString[GenericType]);
          break;
        default:
          IDS_HDT_CONSOLE_USB4_ERROR ("   Reserved Generic Entry %d\n", GenericType);
          break;
      }
    }
    CurrentDromOffset += EntryLength;
  }

  pRouter->NumOfUnusedAdapEntries         = UnusedAdapterCount;
  pRouter->NumOfDpAdapEntries             = DpAdapterCount;
  pRouter->NumOfTbt3LaneAdapEntries       = TBT3LaneAdaptercount;
  pRouter->NumOfTbt3UpStreamAdapEntries   = TBT3PcieUpstreamAdapterCount;
  pRouter->NumOfTbt3DownStreamAdapEntries = TBT3PcieDownstreamAdapterCount;

  pRouter->NumOfUsb3PortMapEntries        = UsbportMapLength;

  if (FeaturePcdGet (PcdAmdIdsDebugPrintEnable) != 0) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "Summarize DROM:\n  Number of Unused Adapters      = 0x%x\n",
      pRouter->NumOfUnusedAdapEntries
      );
    for (Temp32=0; Temp32<pRouter->NumOfUnusedAdapEntries; Temp32++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "      #%d: Adapter Number %d\n",
        Temp32,
        pRouter->UnusedAdapEntry[Temp32].Field.AdapterNumber
        );
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Number of DP Adapters          = 0x%x\n",
      pRouter->NumOfDpAdapEntries
      );
    for (Temp32=0; Temp32<pRouter->NumOfDpAdapEntries; Temp32++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "      #%d: Adapter Number %d, PA %d, PV %d\n",
        Temp32,
        pRouter->DpAdapEntry[Temp32].Field.AdapterNumber,
        pRouter->DpAdapEntry[Temp32].Field.PA,
        pRouter->DpAdapEntry[Temp32].Field.PV
        );
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Number of USB3 Port Mappings   = 0x%x\n", pRouter->NumOfUsb3PortMapEntries
      );
    for (Temp32=0; Temp32<pRouter->NumOfUsb3PortMapEntries; Temp32++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "      #%d: Adapter Number %d, PA %d, PV %d\n",
        Temp32,
        pRouter->DpAdapEntry[Temp32].Field.AdapterNumber,
        pRouter->DpAdapEntry[Temp32].Field.PA,
        pRouter->DpAdapEntry[Temp32].Field.PV
        );
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Number of Tbt3 Lane Adapters   = 0x%x\n",
      pRouter->NumOfTbt3LaneAdapEntries
      );
    for (Temp32=0; Temp32<pRouter->NumOfTbt3LaneAdapEntries; Temp32++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "      #%d: Adapter Number %d, Lane 1 Adapter %d, Dual-Lane Link Capable %d, second Lane Adapter %d\n",
        Temp32,
        pRouter->Tbt3LaneAdapEntry[Temp32].Field.AdapterNumber,
        pRouter->Tbt3LaneAdapEntry[Temp32].Field.L1A,
        pRouter->Tbt3LaneAdapEntry[Temp32].Field.DLC,
        pRouter->Tbt3LaneAdapEntry[Temp32].Field.SecAdapterNum
        );
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Number of Tbt3 Pcie Up Stream Adapters = 0x%x\n",
      pRouter->NumOfTbt3UpStreamAdapEntries
      );
    for (Temp32=0; Temp32<pRouter->NumOfTbt3UpStreamAdapEntries; Temp32++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "      #%d: Adapter Number %d, Fn %d, Dev %d\n",
        Temp32,
        pRouter->Tbt3UpStreamAdapEntry[Temp32].Field.AdapterNumber,
        pRouter->Tbt3UpStreamAdapEntry[Temp32].Field.Fn,
        pRouter->Tbt3UpStreamAdapEntry[Temp32].Field.Dev
        );
    }
    IDS_HDT_CONSOLE_USB4_DEBUG (
      "   Number of Tbt3 Pcie Down Stream Adapters = 0x%x\n",
      pRouter->NumOfTbt3DownStreamAdapEntries
      );
    for (Temp32=0; Temp32<pRouter->NumOfTbt3DownStreamAdapEntries; Temp32++) {
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "      #%d: Adapter Number %d, Fn %d, Dev %d\n",
        Temp32,
        pRouter->Tbt3DownStreamAdapEntry[Temp32].Field.AdapterNumber,
        pRouter->Tbt3DownStreamAdapEntry[Temp32].Field.Fn,
        pRouter->Tbt3DownStreamAdapEntry[Temp32].Field.Dev
        );
    }
  }

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Exit\n", __FUNCTION__);

  return EFI_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Look up DROM unused adapter entry with specific adapter number
 *
 * @param[in] *pRouter        Point to Router
 * @param[in] AdapterNumber   Adapter Number
 *
 * @retval TRUE               The adapter is unused
 * @retval FALSE              The adapter is not in DROM unused adapter entries
**/
BOOLEAN
IsDROMUnusedAdapter (
  IN  AMD_USB4_CM_ROUTER  *pRouter,
  IN  UINT32              AdapterNumber
  )
{
  UINT32  i;

  i = 0;

  if ( pRouter != NULL
    && pRouter->DromVersion != 0xFFFFFFFF
    && pRouter->NumOfUnusedAdapEntries != 0 ) {
    for (i=0; i<pRouter->NumOfUnusedAdapEntries; i++) {
      if ( pRouter->UnusedAdapEntry[i].Field.AdapterNumber == AdapterNumber )
        return TRUE;
    }
  }

  return FALSE;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Look up DROM DP Adapter entry with specific adapter number
 *
 * @param[in] *pRouter          Point to Router
 * @param[in] DpAdapterNumber   DP Adapter Number
 *
 * @retval UINT32               Perferred Lane Adapter number;
                                0     - No preferred lane adapter
                                other - Preferred lane adapter number
**/
UINT32
GetDROMDPPreferLaneAdapter (
  IN  AMD_USB4_CM_ROUTER  *pRouter,
  IN  UINT32              DpAdapterNumber
  )
{
  UINT32  i;

  i = 0;

  if ( pRouter != NULL
    && pRouter->DromVersion != 0xFFFFFFFF
    && pRouter->NumOfDpAdapEntries != 0 ) {
    for (i=0; i<pRouter->NumOfDpAdapEntries; i++) {
      if ( pRouter->DpAdapEntry[i].Field.PV && pRouter->DpAdapEntry[i].Field.AdapterNumber == DpAdapterNumber )
        return (UINT32)(pRouter->DpAdapEntry[i].Field.PA);
    }
  }

  return 0;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get DROM TMU information
 *
 * @param[in] *pRouter          Point to Router
 *
 * @retval Pointer              Pointer of AMD_USB4_DROM_TMU_ENTRY
                                NULL - There is no DROM TMU entry.
                                Other - Return AMD_USB4_CM_ROUTER->DromTMU
**/
AMD_USB4_DROM_TMU_ENTRY*
GetDROMTMU (
  IN  AMD_USB4_CM_ROUTER  *pRouter
  )
{
  if ( pRouter != NULL
    && pRouter->DromVersion != 0xFFFFFFFF
    && pRouter->DromTMU.Value != 0xFF ) {
    return &(pRouter->DromTMU);
  } else {
    return NULL;
  }
}

