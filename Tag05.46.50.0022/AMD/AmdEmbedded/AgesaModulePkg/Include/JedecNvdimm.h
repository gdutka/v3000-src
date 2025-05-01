/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * JedecNvdimm.h
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  JedecNvdimm
 * @e \$Revision$   @e \$Date$
 *
 */

/** @file JedecNvdimm.h
    Header file for JedecNvdimm Driver

**/

#ifndef _JEDEC_NVDIMM_H_
#define _JEDEC_NVDIMM_H_

//---------------------------------------------------------------------------

#include <Library/DebugLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IoLib.h>

#define EFI_ACPI_TABLE_VERSION_X        (EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0 | EFI_ACPI_TABLE_VERSION_4_0)
#define JEDEC_NVDIMM_SMI_CMD 0xEB

#define INPUT_DATA_SIZE     0x100   // TODO: largest size of input?
#define OUTPUT_DATA_SIZE    0x100   // TODO: largest size of output?

// Todo - remove hardcode
// These variables are set by AMD but can be changed by a particular OEM's definition of this
// PcdAmdCpmArsOutputBufferEntries -
//     Defines the number of error records that can be stored in the ARS output buffer. Defaults to 8.
//#define OUTPUT_BUFFER_SIZE PcdAmdCpmArsOutputBufferEntries * 24 bytes/error record + 44 bytes/output buffer
//#define ARS_OUTPUT_BUF_SIZE ((8 * 24) + 44)
#define ARS_OUTPUT_BUF_SIZE OUTPUT_DATA_SIZE

#pragma pack (1)

typedef struct _DSM_ACPI_SMM_DATA {
    UINT32  FunctionIndex;                  // Function Index 0 - 31
    UINT32  InputDataSize;                  // Size of input data
    UINT8   InputData[INPUT_DATA_SIZE];     // Input data (bytes)
    UINT32  OutputDataSize;                 // Size of output data
    UINT8   OutputData[OUTPUT_DATA_SIZE];   // Output data (bytes)
    UINT16  NfitDeviceHandle;               // NFIT Device Handle
    UINT32  NvdimmBitMap;                   // NVDIMM Bitmap
} DSM_ACPI_SMM_DATA;

typedef struct _ARS_ACPI_SMM_DATA {
    UINT32  FunctionIndex;                  // Function Index 0 - 31
    UINT32  InputDataSize;                  // Size of input data
    UINT8   InputData[INPUT_DATA_SIZE];     // Input data (bytes)
    UINT32  OutputDataSize;                 // Size of output data
    UINT8   OutputData[ARS_OUTPUT_BUF_SIZE];// Output data (bytes)
    UINT16  NfitDeviceHandle;               // NFIT Device Handle
    UINT32  NvdimmBitMap;                   // NVDIMM Bitmap
    UINT8   ArsSwSmiCmd;                    // ARS_SW_SMI_CMD
    VOID    *ErrorRecord;
} ARS_ACPI_SMM_DATA;

//
// NVDIMM Root Device _DSMs
//
/**
    Function Index 1 - Query ARS Capabilities
 */
// Input Buffer
typedef struct _ARS_FUNCTION_1_INPUT {
    UINT64  ArsStartSpaAddress;             // Starting of System Physical Address of ARS
    UINT64  ArsLength;                      // In bytes
} ARS_FUNCTION_1_INPUT;

// Output Buffer
typedef struct _ARS_FUNCTION_1_OUTPUT {
    UINT16  Status;
    UINT16  ExtStatus;                      // Extended Status
    UINT32  MaxQryArsStsOptBufSize;         // Max Query ARS Status Output Buffer Size
    UINT32  ClrUncorErrRngLenUntSize;       // Clear Uncorrectable Error Range Length Unit Size
    UINT16  Flags;
    UINT16  Reserved;
} ARS_FUNCTION_1_OUTPUT;


/**
    Function Index 2 - Start ARS
 */
// Input Buffer
typedef struct _ARS_FUNCTION_2_INPUT {
    UINT64  ArsStartSpaAddress;             // Starting of System Physical Address of ARS, In bytes
    UINT64  ArsLength;                      // In bytes
    UINT16  Type;
    UINT8   Flags;
    UINT8   Reserved[5];
} ARS_FUNCTION_2_INPUT;

// Output Buffer
typedef struct _ARS_FUNCTION_2_OUTPUT {
    UINT16  Status;
    UINT16  ExtStatus;
    UINT32  EstTimeForScrub;                // Estimated Time for Scrub
} ARS_FUNCTION_2_OUTPUT;


/**
    Function Index 3 - Query ARS Status
 */
// Input Buffer
//   None

// Function Output
//   ARS_FUNCTION_3_OUTPUT -> ARS_DATA -> ARS_ERROR_RECORD
typedef struct _ARS_ERROR_RECORD {
    UINT32  NfitHandle;
    UINT32  Reserved;
    UINT64  StartSpaOfErr;                            // Start SPA of Error Location, Start of System Physical Address of the error.
    UINT64  Length;
} ARS_ERROR_RECORD;

typedef struct _ARS_DATA {
    UINT32            OutputSize;
    UINT64            StartSpa;
    UINT64            Length;
    UINT64            RestartArsStartSpaAddr;         // Restart ARS Start SPA Address
    UINT64            RestartArsLen;                  // Restart ARS Length
    UINT16            Type;
    UINT16            Flags;
    UINT32            NumOfErrRec;                    // Number of Error Records
    ARS_ERROR_RECORD  ArsErrorRec[];
} ARS_DATA;

// Output Buffer
typedef struct _ARS_FUNCTION_3_OUTPUT {
    UINT16    Status;
    UINT16    ExtStatus;
    ARS_DATA  ArsData;
} ARS_FUNCTION_3_OUTPUT;


/**
    Function Index 4 - Clear Uncorrectable Error
 */
// Input Buffer
typedef struct _ARS_FUNCTION_4_INPUT {
    UINT64  ClrUncorErrSpaRngBase;          // Clear Uncorrectable Error SPA Range Base
    UINT64  ClrUncorErrRngLen;              // Clear Uncorrectable Error Range Length
} ARS_FUNCTION_4_INPUT;

// Output Buffer
typedef struct _ARS_FUNCTION_4_OUTPUT {
    UINT16  Status;
    UINT16  ExtStatus;
    UINT32  Reserved;
    UINT64  ClredUncorErrRngLen;            // Cleared Uncorrectable Error Range Length
} ARS_FUNCTION_4_OUTPUT;


/**
    Function Index 5 - Translate SPA
 */
// Input Buffer
typedef struct _ARS_FUNCTION_5_INPUT {
    UINT64  Spa;
} ARS_FUNCTION_5_INPUT;


// Function Output
//   ARS_FUNCTION_5_OUTPUT -> TRANSLATED_NVDIMM_DEVICE
typedef struct _TRANSLATED_NVDIMM_DEVICE {
    UINT32  NfitDevHandle;                            // NFIT Device Handle
    UINT32  Reserved;
    UINT64  Dpa;                                      // DIMM Physical Address that the SPA translates to.
} TRANSLATED_NVDIMM_DEVICE;

// Output Buffer
typedef struct _ARS_FUNCTION_5_OUTPUT {
    UINT16                    Status;
    UINT16                    ExtStatus;
    UINT8                     Flags;
    UINT8                     Reserved[3];
    UINT64                    TranslatedLen;          // Translated Length
    UINT32                    NumOfNvdimms;           // Number of NVDIMMs
    TRANSLATED_NVDIMM_DEVICE  TranslatedNvdimmDev[0];
} ARS_FUNCTION_5_OUTPUT;


/**
    Function Index 6 - Reserved
 */


/**
    Function Index 7 - ARS Error Inject
 */
// Input Buffer
typedef struct _ARS_FUNCTION_7_INPUT {
    UINT64  ArsErrInjSpaRngBase;            // ARS Error Inject SPA Range Base
    UINT64  ArsErrInjSpaRngLen;             // ARS Error Inject SPA Range Length
    UINT8   ArsErrInjOpt;                   // ARS Error Inject Options
} ARS_FUNCTION_7_INPUT;

// Output Buffer
typedef struct _ARS_FUNCTION_7_OUTPUT {
    UINT16  Status;
    UINT16  ExtStatus;
} ARS_FUNCTION_7_OUTPUT;


/**
    Function Index 8 - ARS Error Inject Clear
 */
// Input Buffer
typedef struct _ARS_FUNCTION_8_INPUT {
    UINT64  ArsErrInjClrSpaRngBase;         // ARS Error Inject Clear SPA Range Base
    UINT64  ArsErrInjClrSpaRngLen;          // ARS Error Inject Clear SPA Range Length
} ARS_FUNCTION_8_INPUT;

// Output Buffer
typedef struct _ARS_FUNCTION_8_OUTPUT {
    UINT16  Status;
    UINT16  ExtStatus;
} ARS_FUNCTION_8_OUTPUT;


/**
    Function Index 9 - ARS Error Inject Status Query
 */
// Input Buffer
//   None

// Function Output
//   ARS_FUNCTION_9_OUTPUT -> ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORDS
typedef struct _ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORDS {
    UINT64  SpaRngBase;                               //ARS Error Inject Status Query Error Record SPA Range Base
    UINT64  SpaRngeLen;                               //ARS Error Inject Status Query Error Record SPA Range Length
} ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORDS;

// Output Buffer
typedef struct _ARS_FUNCTION_9_OUTPUT {
    UINT16                                       Status;
    UINT16                                       ExtStatus;
    UINT32                                       InjedErrRecCnt;  //Injected Error Record Count
    ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORDS  ArsErrInjStsQryErrRec[];
} ARS_FUNCTION_9_OUTPUT;


/**
    Structure to keep SMM timeout data.
 */
typedef struct {
    UINTN  OverFlow;
    UINTN  EndValue;
    UINTN  OldTimerValue;
} SMM_TIMEOUT;

/**
    Defines ACPI timer width in bits
 */
#define NUM_BITS_IN_ACPI_TIMER      32

///< Buffer for each NVDIMM, keep SPD data for ACPI NFIT table
typedef struct {
    UINT16    DataValid;                ///< 0x55aa --> data valid in this entry
    UINT8     Socket;                   ///< Socket
    UINT8     Channel;                  ///< Channel
    UINT8     Dimm;                     ///< Dimm
    UINT16    NvdimmIndex;              ///< Dimm index
    UINT16    Handle;                   ///< TYPE17->Handle
    UINT16    Spd192;                   ///<
    UINT16    Spd194;                   ///<
    UINT16    Spd196;                   ///<
    UINT16    Spd198;                   ///< High byte reserved, set to 0
    UINT16    Spd320;                   ///<
    UINT16    Spd322;                   ///< High byte reserved, set to 0
    UINT16    Spd323;                   ///<
    UINT8     SerialNumber[4];          ///< SPD 325 ~ 328
    UINT16    Spd349;                   ///< High byte reserved, set to 0
    UINT16    Reserved;                 ///< Reserved
} NVDIMM_SPD_INFO;

typedef struct _JEDEC_NVDIMM_SMM_PROTOCOL  JEDEC_NVDIMM_SMM_PROTOCOL;

typedef struct {
  DSM_ACPI_SMM_DATA                     *DsmAcpiSmmData;
  ARS_ACPI_SMM_DATA                     *ArsAcpiSmmData;
  UINT32                                NvdimmBitMap;
} JEDEC_NVDIMM_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *READ_MEM_ERROR_COUNTERS)(
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    OUT UINT8                           *DramEccErrorCount,
    OUT UINT8                           *DramThresholdEccCount
);

typedef
EFI_STATUS
(EFIAPI *SET_MEM_ERROR_COUNTERS)(
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    IN UINT8                            DramEccErrorCount,
    IN UINT8                            DramThresholdEccCount
);

typedef
EFI_STATUS
(EFIAPI *INCREMENT_MEM_ERROR_COUNTERS)(
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    IN BOOLEAN                          IncDramEccErrorCount,
    IN BOOLEAN                          IncDramThresholdEccCount
);

typedef
EFI_STATUS
(EFIAPI *PERFORM_NVDIMM_PURGE)(
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm
);

struct _JEDEC_NVDIMM_SMM_PROTOCOL {
    READ_MEM_ERROR_COUNTERS         ReadMemoryErrorCounters;
    SET_MEM_ERROR_COUNTERS          SetMemoryErrorCounters;
    INCREMENT_MEM_ERROR_COUNTERS    IncrementMemErrorCounters;
    PERFORM_NVDIMM_PURGE            PerformNvdimmPurge;
};

#pragma pack ()

EFI_STATUS
EFIAPI
ReadMemoryErrorCounters (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    OUT UINT8                           *DramEccErrorCount,
    OUT UINT8                           *DramThresholdEccCount
);

EFI_STATUS
EFIAPI
SetMemoryErrorCounters (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    IN UINT8                            DramEccErrorCount,
    IN UINT8                            DramThresholdEccCount
);

EFI_STATUS
EFIAPI
IncrementMemErrorCounters (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm,
    IN BOOLEAN                          IncDramEccErrorCount,
    IN BOOLEAN                          IncDramThresholdEccCount
);

EFI_STATUS
EFIAPI
PerformNvdimmPurge (
    IN CONST JEDEC_NVDIMM_SMM_PROTOCOL  *This,
    IN UINT8                            Socket,
    IN UINT8                            Channel,
    IN UINT8                            Dimm
);

#endif  // JEDEC_NVDIMM_H



