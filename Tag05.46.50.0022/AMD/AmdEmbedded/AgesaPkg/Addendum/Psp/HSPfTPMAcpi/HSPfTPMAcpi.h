/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _HSP_FTPM_ACPI_H_
#define _HSP_FTPM_ACPI_H_

#include <Protocol/AcpiSystemDescriptionTable.h>

#define TPM2_ACPI_OEM_ID        {'A', 'M', 'D', ' ', ' ', ' '}
#define TPM2_ACPI_TABLE_OEM_ID  {'A', 'G', 'E', 'S', 'A', ' ',' ',' '}
#define TPM2_ACPI_OEM_REVISION  SIGNATURE_32 ('H', 'S', 'P', ' ')

#pragma  pack (push, 1)

/// Define TPM_2_ACPI_TABLE
typedef struct {
  EFI_ACPI_SDT_HEADER             Header;                  ///< Header
  UINT16                          PlatformClass;           ///< PlatformClass.
                                                           ///<   0 for client platforms
                                                           ///<   1 for server platforms.
  UINT16                          Reserved;                ///< ControlArea
  EFI_PHYSICAL_ADDRESS            AddressofControlArea;    ///< StartMethod
  UINT32                          StartMethod;             ///< HSP use 13
  EFI_PHYSICAL_ADDRESS            TPMStartAddress;         ///< C2PMSG[2]_CNTL address
  EFI_PHYSICAL_ADDRESS            TPMReplyAddress;         ///< P2CMSG[2]_CNTL address
} TPM2_ACPI_TABLE, *PTPM2_ACPI_TABLE;

/// Define MHSP_ACPI_TABLE
typedef struct {
  UINT8                 AddressSpaceID;       ///< 0x00 System Memory space
  UINT8                 RegisterBitWidth;     ///<
  UINT8                 RegisterBitOffset;    ///<
  UINT8                 AccessSize;           ///< 4 - QWord access
  EFI_PHYSICAL_ADDRESS  Address;              ///<
} GENERIC_ADDRESS;

typedef enum {
  MHSP_PROTOCOL_ID_INVALID    = 0,            ///< Indicates an uninitialized or invalid table
  MHSP_PROTOCOL_ID_ACPIFIXED  = 1             ///< Indicates that all resources will be provided in this table.
                                              ///< No items should be defined in _CRS.
} MHSP_PROTOCOL_ID;

typedef struct {
  UINT64  ChannelBaseAddress;
  UINT64  RequestDoorbellAddress;
  UINT64  ReplyDoorbellAddress;
  UINT32  ChannelSize;
  UINT32  IRQResource;
  UINT32  ChannelParameters[2];
} MHSP_CHANNEL, *PMHSP_CHANNEL;

typedef struct {
  EFI_ACPI_SDT_HEADER Header;       ///< Header
  UINT32              ProtocolId;   ///< Shall have a value of 1.
                                    ///< This indicates the Protocol Id for this hardware
  MHSP_CHANNEL        Channels[4];  ///< An array of 4 MHSP_CHANNEL structures tightly packed.
                                    ///< If the channel is not provided, the memory must be allocated and set to all zero.
                                    ///< Channels shall be provided in ascending order (by base address) and no gaps are allowed.
} MHSP_ACPI_TABLE, *PMHSP_ACPI_TABLE;

#pragma  pack (pop)

#endif //_HSP_FTPM_ACPI_H_

