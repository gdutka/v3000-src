/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CCX CPPC Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 */

#ifndef _CCX_CPPC_LIB_H_
#define _CCX_CPPC_LIB_H_

#include <Porting.h>
#include <Protocol/AmdNbioCppcServicesProtocol.h>

#pragma pack (push, 1)
#define   CPC_INTEGER_DWORD                0
#define   CPC_BUFFER                       10
#define   CPC_END_OF_TABLE                 0xFF


#define   SUPPORTED_BY_V2                  2
#define   SUPPORTED_BY_V3                  3

#define   GENERIC_REGISTER_DESC_BODY_SIZE sizeof (GENERIC_REGISTER_DESC_BODY)
#define   GENERIC_REG_SPACE_ID_MEM         0x0
#define   GENERIC_REG_SPACE_ID_IO          0x1
#define   GENERIC_REG_SPACE_ID_PCI         0x2
#define   GENERIC_REG_SPACE_ID_EC          0x3
#define   GENERIC_REG_SPACE_ID_SMBUS       0x4
#define   GENERIC_REG_SPACE_ID_PCC         0xA
#define   GENERIC_REG_SPACE_ID_FFH         0x7F
#define   GENERIC_REG_ADDR_SIZE_UNDEFINED  0x0
#define   GENERIC_REG_ADDR_SIZE_BYTE       0x1
#define   GENERIC_REG_ADDR_SIZE_WORD       0x2
#define   GENERIC_REG_ADDR_SIZE_DWORD      0x3
#define   GENERIC_REG_ADDR_SIZE_QWORD      0x4
#define   PCC_SUBSPACE_0                   0x0


//--------------------------------------------
//              AML code definition
//             (CPC Header and Body)
//---------------------------------------------

#define   GENERIC_REGISTER_DESC_HEADER_SIZE sizeof (GENERIC_REGISTER_DESC_HEADER)
#define   GENERIC_REG_DESCRIPTOR_OPCODE    0x82
#define   GENERIC_REG_LENGTH               0xC


typedef struct  _GENERIC_REGISTER_DESC_HEADER {
  UINT8   GenericRegisterDescriptor;    ///< 0x82 fixed
  UINT16  Length;                       ///< 0x000C fixed
} GENERIC_REGISTER_DESC_HEADER;

typedef struct  _GENERIC_REGISTER_DESC_BODY {
  UINT8   AddrSpaceId;                  ///< 0 - mem, 1 - io, 2 - PCI, 3 - EC, 4 - SMBus, 0xA - PCC, 0x7F FFH
  UINT8   RegisterBitWidth;             ///< Register width in bits
  UINT8   RegisterBitOffset;            ///< Offset to the start of the register in bits from the address
  UINT8   AddressSize;                  ///< Access size: 0 - undefined, 1 - byte, 2 - word, 3 - dword, 4 - qword
  UINT64  RegisterAddress;              ///< Register Address
} GENERIC_REGISTER_DESC_BODY;

typedef struct  _GENERIC_REGISTER_DESC {
  GENERIC_REGISTER_DESC_HEADER  Header;
  GENERIC_REGISTER_DESC_BODY    Body;
} GENERIC_REGISTER_DESC;

#define   GENERIC_REGISTER_DESC_SIZE sizeof (GENERIC_REGISTER_DESC)
typedef struct  _RES_TEMP_GEN_REG {
  UINT8   BufferOp;                     ///< 0x11 fixed
  UINT8   PkgLength;                    ///< 0x14 fixed for gen reg
  UINT8   BufferSizePrefix;             ///< 0x0A fixed for gen reg
  UINT8   BufferSize;                   ///< 0x11 fixed for gen reg
  GENERIC_REGISTER_DESC  Register;      ///< Register descriptor
  UINT16  EndTag;                       ///< 0x0079 fixed
} RES_TEMP_GEN_REG;
#define   RES_TEMP_GEN_REG_SIZE sizeof (RES_TEMP_GEN_REG)
#define   BUFFER_OP                   0x11
#define   RES_TEMP_GEN_REG_PKG_LEN    0x14
#define   BYTE_OP                     0xA
#define   RES_TEMP_GEN_REG_BUFF_SIZE  0x11
#define   RES_TEMP_END_TAG            0x0079

typedef struct  _RES_TEMP_GEN_INTEGER_DWORD {
  UINT8   DWordPrefix;                  ///< 0x0C fixed
  UINT32  Data;
} RES_TEMP_GEN_INTEGER_DWORD;


typedef UINT32 (*CPPC_GET_INT_VALUE) (
  IN VOID *Buffer
);

typedef struct _CPPC_ENTRIES_COMMON_TABLE {
  UINT8   Type;                         ///< Integer or buffer
  GENERIC_REGISTER_DESC_BODY GenericRegister; ///< Used for buffer type
  CPPC_GET_INT_VALUE         GetIntValue;     ///< Used for Integer type
  UINT8   SupportedRevision;            ///< Supported by this revision and later
} CPPC_ENTRIES_COMMON_TABLE;

UINT8
GetCppcReportingVersion (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  );

CPPC_ENTRIES_COMMON_TABLE   *
GetCppcRegisterDescriptor (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  );

UINT32
AmdCppcAutonomousSelectionEnable (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  );

UINT32
AmdCppcGetLowestSpeed (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  );

UINT32
AmdCppcGetNominalSpeed (
  IN       DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL *NbioCppcServices
  );

#pragma pack (pop)

#endif

