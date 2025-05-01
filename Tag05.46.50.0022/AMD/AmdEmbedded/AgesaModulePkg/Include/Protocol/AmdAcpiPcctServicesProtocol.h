/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD PCCT Services Protocol prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Protocol
 * @e \$Revision:  $   @e \$Date:  $
 */
 
#ifndef _AMD_ACPI_PCCT_SERVICES_PROTOCOL_H_
#define _AMD_ACPI_PCCT_SERVICES_PROTOCOL_H_

#pragma pack (push, 1)

/// Format for PCCT Header
typedef struct {
  UINT8   Sign[4];                   ///< PCCT, Signature for the Platform Communications Channel Table
  UINT32  Length;                    ///< Length, in bytes, of the entire PCCT
  UINT8   Revision;                  ///< 2, per ACPI v6.2 spec
  UINT8   Checksum;                  ///< Entire table must sum to zero.
  UINT8   OemId[6];                  ///< OEM ID
  UINT8   OemTableId[8];             ///< OEM Tabled ID
  UINT32  OemRev;                    ///< OEM Revision
  UINT8   CreatorId[4];              ///< Creator ID
  UINT32  CreatorRev;                ///< Creator Revision
  UINT32  Flags;                     ///< Flags
  UINT8   Reserved[8];               ///< Reserved
} PCCT_HEADER;

/// GAS Structure
typedef struct  _GAS_STRUCT {
  UINT8   AddrSpaceId;               ///< 0 - mem, 1 - io, 2 - PCI, 3 - EC, 4 - SMBus, 0xA - PCC, 0x7F FFH
  UINT8   RegisterBitWidth;          ///< Register width in bits
  UINT8   RegisterBitOffset;         ///< Offset to the start of the register in bits from the address
  UINT8   AccessSize;                ///< Access size: 0 - undefined, 1 - byte, 2 - word, 3 - dword, 4 - qword
  UINT64  RegisterAddress;           ///< Register Address
} GAS_STRUCT;

#define   GAS_SPACE_ID_MEM         0x0
#define   GAS_SPACE_ID_IO          0x1
#define   GAS_SPACE_ID_PCI         0x2
#define   GAS_SPACE_ID_EC          0x3
#define   GAS_SPACE_ID_SMBUS       0x4
#define   GAS_SPACE_ID_PCC         0xA
#define   GAS_SPACE_ID_FFH         0x7F
#define   GAS_ADDR_SIZE_UNDEFINED  0x0
#define   GAS_ADDR_SIZE_BYTE       0x1
#define   GAS_ADDR_SIZE_WORD       0x2
#define   GAS_ADDR_SIZE_DWORD      0x3
#define   GAS_ADDR_SIZE_QWORD      0x4

/// Format for Generic Communications Subspace Structure (type 0)
typedef struct {
  UINT8       Type;                      ///< 0 (Generic Communications Subspace)
  UINT8       Length;                    ///< 62
  UINT8       Reserved[6];               ///< Reserved
  UINT64      RangeBaseAddress;          ///< Base Address of the shared memory range
  UINT64      RangeLength;               ///< Length of the memory range. Must be > 8.
  GAS_STRUCT  DoorbellRegister;          ///< Generic Address Structure for the PCC doorbell
  UINT64      DoorbellPreserve;          ///< Contains a mask of bits to preserve when writing the doorbell register.
  UINT64      DoorbellWrite;             ///< Contains a mask of bits to set when writing the doorbell register.
  UINT32      NominalLatency;            ///< Expected latency to process a command, in microseconds.
  UINT32      MaxPeriodicAccessRate;     ///< The maximum number of periodic requests that the subspace
                                         ///< channel can support, reported in commands  per minute. 0 indicates no limitation.
  UINT16      MinReqTurnaroundTime;      ///< The minimum amount of time that OSPM must wait after the completion of a
                                         ///< command before issuing the next command, in microseconds.
} GEN_COMM_SUBSPACE_STRUCT;

/*----------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define  PCCT_FLAGS_DOORBELL_INT  0
#define  GEN_COMM_TYPE     0
#define  GEN_COMM_LENGTH   62

/// Forward declaration for AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL
typedef struct _AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *AMD_PCCT_SERVICES_ADD_GEN_COMM_SUBSPACE_STRUCT) (
  IN       AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL    *This,
  IN       PCCT_HEADER                            *PcctHeaderStructPtr,
  IN       AMD_CPPC_INFO                          *AmdCppcInfo,
  IN OUT   UINT8                                  **EndOfTable
  );

/// When installed, the PCCT Services Protocol produces a collection of
/// services related to PCCT
struct _AMD_CCX_ACPI_PCCT_SERVICES_PROTOCOL {
  UINTN                                                Revision;                  ///< Revision Number
  AMD_PCCT_SERVICES_ADD_GEN_COMM_SUBSPACE_STRUCT       AddGenCommSubspaceStruct;  ///< Creates a Generic Communications
                                                                                  ///< Subspace Structure (type 0)
};

extern EFI_GUID gAmdCcxAcpiPcctServicesProtocolGuid;

#pragma pack (pop)
#endif // _AMD_ACPI_PCCT_SERVICES_PROTOCOL_H_
