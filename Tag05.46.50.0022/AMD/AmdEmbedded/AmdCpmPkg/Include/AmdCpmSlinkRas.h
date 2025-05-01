/*****************************************************************************
 *
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#ifndef _AMD_CPM_SLINK_RAS_H_
#define _AMD_CPM_SLINK_RAS_H_
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <Register/AmdSlinkPcieReg.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
/// PCI Component Device GUID
#define CCIX_GUID \
  { 0xC3CB993B, 0x436F, 0x02C4, 0x9B, 0x68, 0xD2, 0x71, 0xF2, 0xE8, 0xCA, 0x31 }
  //C3CB993B-436F-02C4-71D2689B-31CAE8F2

#define CCIX_PL_LOG_HEADER_SIZE         0x18

// Relative to CCIX PER Log Header
#define PER_LOG_HEADER_DW0_OFFSET       0x00
#define PER_LOG_HEADER_DW1_OFFSET       0x04


#define ERROR_COMP_TYPE_RA              0x00
#define ERROR_COMP_TYPE_HA              0x01
#define ERROR_COMP_TYPE_SA              0x02
#define ERROR_COMP_TYPE_PORT            0x03
#define ERROR_COMP_TYPE_LINK            0x04


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

///
/// CCIX Base Specification 1.0 Table 7-3
///
typedef union {
  struct {
     UINT32     PerRev:8;               ///< 
     UINT32     LogLen:7;               ///< 
     UINT32     MultiErr:1;             ///< 
     UINT32     :16;                    ///< 
  } Field;
  UINT32  Value;
} PER_LOG_HEADER_DW0;

typedef union {
  struct {
     UINT32     :12;                    ///< 
     UINT32     ErrCompType:4;          ///< Error Component Type
     UINT32     SevUe:1;                ///< Uncorrected Error
     UINT32     SevNocomm:1;            ///< CCIX Protocol Communication Broken
     UINT32     SevDegraded:1;          ///< Degraded / Threshold
     UINT32     SevDeferred:1;          ///< Deferrable
     UINT32     :4;                     ///< Reserved
     UINT32     PerType:4;              ///< PER Type
     UINT32     :2;                     ///< Reserved
     UINT32     AddrValid:1;            ///< Address Valid Flag
     UINT32     VenErrTypeFlag:1;       ///< Vendor-Specific Error Type Flag
  } Field;
  UINT32  Value;
} PER_LOG_HEADER_DW1;

///
/// CCIX PER Log Header
///
typedef struct {
  PER_LOG_HEADER_DW0    PerLogHeaderDw0;        ///< 
  PER_LOG_HEADER_DW1    PerLogHeaderDw1;        ///< 
  UINT32                FaultAddrHi;            ///< 
  UINT32                FaultAddrLo;            ///< 
  UINT32                FaultAddrMaskLen;       ///< 
  UINT32                Reserved1;              ///< 
  UINT32                Reserved2;              ///< 
  UINT32                Reserved3;              ///< 
} CCIX_PER_LOG_HEADER;

///
/// CCIX Base Specification 1.0 Table 7-6
///
typedef union {
  struct {                                      ///< CCIX_MEM_ERR_VALID_BIT
    UINT32       GenericMemoryType:1;           ///< [0] Generic Memory Type
    UINT32       Operation:1;                   ///< [1] Operation
    UINT32       MemoryErrorType:1;             ///< [2] Memory Error Type
    UINT32       Card:1;                        ///< [3] Card (Channel)
    UINT32       Bank:1;                        ///< [4] Bank
    UINT32       Device:1;                      ///< [5] Device
    UINT32       Row:1;                         ///< [6] Row
    UINT32       Column:1;                      ///< [7] Column
    UINT32       Rank:1;                        ///< [8] Rank
    UINT32       BitPosition:1;                 ///< [9] Bit Position
    UINT32       ChipIdentification:1;          ///< [10] Chip Identification
    UINT32       VenderSpecificLogInfo:1;       ///< [11] Vender-Specific Log Info
    UINT32       Module:1;                      ///< [12] Module
    UINT32       SpecificMemoryType:1;          ///< [13] Specific Memory Type
    UINT32       :18;                           ///< [31:14] Reserved
  } Field;
  UINT32  Value;
} CCIX_MEM_ERR_VALID_BIT;

///
/// Memory Error Type Structure
///
typedef struct {
  CCIX_MEM_ERR_VALID_BIT     ValidationBits;            ///< Validation Bits
  UINT8                      FruId;                     ///< FRU ID
  UINT8                      Reserved;                  ///< Reserved
  UINT16                     Length;                    ///< Length
  UINT8                      GenericMemTypeCap;         ///< Memory Pool Generic Memory Type Capability
  UINT8                      Operation;                 ///< Operation Type
  UINT8                      MemoryErrorType;           ///< Memory Error Type
  UINT8                      ChannelNum;                ///< Card or Channel Number
  UINT16                     Module;                    ///< Module
  UINT16                     Bank;                      ///< Bank
  UINT32                     Device;                    ///< Device
  UINT32                     Row;                       ///< Row
  UINT32                     Column;                    ///< Column
  UINT32                     Rank;                      ///< Rank
  UINT8                      BitPosition;               ///< Bit Position
  UINT8                      ChipIdentification;        ///< Chip Identification
  UINT8                      SpecificMemTypeCap;        ///< Memory Pool Specific Memory Type Capability
  UINT8                      VendorSpecificLogInfo[];   ///< Vendor-Specific Log Info
} CCIX_MEM_ERR_STURC;

///
/// CCIX Base Specification 1.0 Table 7-7
///
typedef union {
  struct {                                      ///< CCIX_CACHE_ERR_VALID_BIT
    UINT32       CacheType:1;                   ///< [0] Cache Type
    UINT32       Operation:1;                   ///< [1] Operation
    UINT32       CacheErrorType:1;              ///< [2] Cache Error Type
    UINT32       CacheLevel:1;                  ///< [3] Cache Level
    UINT32       Set:1;                         ///< [4] Set
    UINT32       Way:1;                         ///< [5] Way
    UINT32       CacheInstanceId:1;             ///< [6] Cache Instance ID
    UINT32       VendorSpecificLogInfo:1;       ///< [7] Vendor-Specific Log Info
    UINT32       :24;                           ///< [31:8] Reserved
  } Field;
  UINT32  Value;
} CCIX_CACHE_ERR_VALID_BIT;

///
/// Cache Error Type Structure
///
typedef struct {
  CCIX_CACHE_ERR_VALID_BIT   ValidationBits;            ///< Validation Bits
  UINT16                     Length;                    ///< Length
  UINT8                      CacheType;                 ///< Cache Type
  UINT8                      OperationType;             ///< Operation Type
  UINT8                      CacheErrorType;            ///< Cache Error Type
  UINT8                      CacheLevel;                ///< Cache Level
  UINT32                     Set;                       ///< Set
  UINT32                     Way;                       ///< Way
  UINT8                      CacheInstanceId;           ///< Cache Instance ID
  UINT8                      Reserved;                  ///< Reserved
  UINT8                      VendorSpecificLogInfo[];   ///< Vendor-Specific Log Info
} CCIX_CACHE_ERR_STURC;

///
/// CCIX Base Specification 1.0 Table 7-8
///
typedef union {
  struct {                                      ///< CCIX_CACHE_ERR_VALID_BIT
    UINT32       Operation:1;                   ///< [0] Operation
    UINT32       CacheInstanceId:1;             ///< [1] Cache Instance ID
    UINT32       VendorSpecificLogInfo:1;       ///< [2] Vendor-Specific Log Info
    UINT32       :29;                           ///< [31:3] Reserved
  } Field;
  UINT32  Value;
} CCIX_ATC_ERR_VALID_BIT;

///
/// ATC Error Type Structure
///
typedef struct {
  CCIX_ATC_ERR_VALID_BIT     ValidationBits;            ///< Validation Bits
  UINT16                     Length;                    ///< Length
  UINT8                      OperationType;             ///< Operation Type
  UINT8                      AtcInstanceId;             ///< ATC Instance ID
  UINT32                     Reserved;                  ///< Reserved
  UINT8                      VendorSpecificLogInfo[];   ///< Vendor-Specific Log Info
} CCIX_ATC_ERR_STURC;

///
/// CCIX Base Specification 1.0 Table 7-9
///
typedef union {
  struct {                                      ///< CCIX_CACHE_ERR_VALID_BIT
    UINT32       Operation:1;                   ///< [0] Operation
    UINT32       PortErrorType:1;               ///< [1] Port Error Type
    UINT32       CcixMessage:1;                 ///< [2] CCIX Message
    UINT32       VendorSpecificLogInfo:1;       ///< [3] Vendor-Specific Log Info
    UINT32       :28;                           ///< [31:4] Reserved
  } Field;
  UINT32  Value;
} CCIX_PORT_ERR_VALID_BIT;

///
/// Port Error Type Structure
///
typedef struct {
  CCIX_PORT_ERR_VALID_BIT    ValidationBits;            ///< Validation Bits
  UINT16                     Length;                    ///< Length
  UINT8                      OperationType;             ///< Operation Type
  UINT32                     PortErrorType;             ///< Port Error Type
  UINT8                      CcixMessage[32];           ///< Ccix Message
  UINT8                      VendorSpecificLogInfo[];   ///< Vendor-Specific Log Info
} CCIX_PORT_ERR_STURC;

///
/// CCIX Base Specification 1.0 Table 7-10
///
typedef union {
  struct {                                      ///< CCIX_CACHE_ERR_VALID_BIT
    UINT32       Operation:1;                   ///< [0] Operation
    UINT32       LinkErrorType:1;               ///< [1] Port Error Type
    UINT32       LinkId:1;                      ///< [2] CCIX Message
    UINT32       CreditType:1;                  ///< [3] Port Error Type
    UINT32       CcixMessage;                   ///< [4] CCIX Message
    UINT32       VendorSpecificLogInfo:1;       ///< [5] Vendor-Specific Log Info
    UINT32       :26;                           ///< [31:6] Reserved
  } Field;
  UINT32  Value;
} CCIX_LINK_ERR_VALID_BIT;

///
/// CCIX Link Error Type Structure
///
typedef struct {
  CCIX_LINK_ERR_VALID_BIT    ValidationBits;            ///< Validation Bits
  UINT16                     Length;                    ///< Length
  UINT8                      OperationType;             ///< Operation Type
  UINT8                      LinkErrorType;             ///< Link Error Type
  UINT8                      LinkId;                    ///< Link ID
  UINT8                      LinkErrCreditType;         ///< Link Error Credit Type
  UINT16                     Reserved;                  ///< Reserved
  UINT8                      CcixMessage[32];           ///< Ccix Message
  UINT8                      VendorSpecificLogInfo[];   ///< Vendor-Specific Log Info
} CCIX_LINK_ERR_STURC;

///
/// CCIX Base Specification 1.0 Table 7-11
///
typedef union {
  struct {                                      ///< CCIX_CACHE_ERR_VALID_BIT
    UINT32       VendorSpecificLogInfo:1;       ///< [0] Vendor-Specific Log Info
    UINT32       :31;                           ///< [31:1] Reserved
  } Field;
  UINT32  Value;
} CCIX_AGENT_INT_ERR_VALID_BIT;

///
/// CCIX PER Agent Internal Error Type Structure
///
typedef struct {
  CCIX_AGENT_INT_ERR_VALID_BIT  ValidationBits;            ///< Validation Bits
  UINT16                        Length;                    ///< Length
  UINT16                        Reserved;                  ///< Reserved
  UINT8                         VendorSpecificLogInfo[];   ///< Vendor-Specific Log Info
} CCIX_AGENT_INT_ERR_STURC;

#endif //_AMD_CPM_SLINK_H_

