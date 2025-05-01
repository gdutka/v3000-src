/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/
/**
 * @file
 *
 * Contains global definition for HSP
 *
 *
 */
#ifndef __HSP_H__
#define __HSP_H__

#define HSP_DEBUG_ENABLE        1
// Use For Debug. 1: Hide HSP Pcie Device Under OS;  0: Display HSP Pcie Device Under OS
#define HSP_HIDE_PCIE_DEVICE    1

#define HSP_VIDDID                                  0x163E1022
#define HSP_TEMP_BAR0_SIZE                          0x10000       // 64KB is enough
#define AMD_VID                                     0x1022
#define HSP_BRIDGE_DID                              0x14B9
#define HSP_DEVICE_DID                              0x163E
#define HSP_PCICFG_BRIDGE_UEFI_VARIABLE_CHECK_FLAG  0x55AA0000
#define HSP_PCICFG_DEVICE_UEFI_VARIABLE_CHECK_FLAG  0x000066BB
#define HSP_PCICFG_UEFI_VARIABLE_CHECK_FLAG         0x55AA66BB
#define HSP_PCICFG_S3_SAVE_UEFI_VARIABLE_NAME       L"HspPciCfgS3SaveUefiVariable"

#ifndef ALIGN
  #define ALIGN(v, a) (UINTN)((((v) - 1) | ((a) - 1)) + 1)
#endif

#ifndef offsetof
  #define offsetof(s, m) (UINTN)&(((s *)0)->m)
#endif

#define GET_TPM_CONTROL_AREA                  (offsetof (TPM_BUFFERS, ControlArea))
#define GET_TPM_COMMANDRESPONSE_BUFFER        (offsetof (TPM_BUFFERS, CommandResponseBuffer))

#define GET_TPM_CONTROL_AREA_REQUEST          (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, Request)))
#define GET_TPM_CONTROL_AREA_COMMANDSIZE      (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, CommandSize)))
#define GET_TPM_CONTROL_AREA_COMMANDADDRESS   (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, CommandAddress)))
#define GET_TPM_CONTROL_AREA_RESPONSESIZE     (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, ResponseSize)))
#define GET_TPM_CONTROL_AREA_RESPONSEADDRESS  (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, ResponseAddress)))
#define GET_TPM_CONTROL_AREA_INTERRUPTCONTROL (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, InterruptControl)))
#define GET_TPM_CONTROL_AREA_STATUS           (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, Status)))
#define GET_TPM_CONTROL_AREA_START            (GET_TPM_CONTROL_AREA + (offsetof (TPM_CONTROL_AREA, Start)))

#define GET_SPI_REQUEST_STATUS                (offsetof (SPI_REQUEST, Status))

#define HSP_VIDDID                          0x163E1022

#define HSP_APERTURE_BASE                   0x07000000
#define HSP_MAILBOX_SIZE                    0x1000
#define HSP_DOORBELL_SIZE                   4

#define HSP_COMMANDRESPONSE_BUFFER_SIZE     0xF80

//
// C2H_TPM_L0
//
#define HSP_C2H_TPM_L0_OFFSET               0x0000
#define HSP_C2H_TPM_L0_DOORBELL_OFFSET      0x8000
#define HSP_H2C_TPM_L0_DOORBELL_OFFSET      0x8028

#define HSP_C2H_TPM_L0_SMN_BASE             HSP_APERTURE_BASE + HSP_C2H_TPM_L0_OFFSET
#define HSP_C2H_TPM_L0_SMN_SIZE             HSP_MAILBOX_SIZE
#define HSP_C2H_TPM_L0_DOORBELL_SMN_BASE    HSP_APERTURE_BASE + HSP_C2H_TPM_L0_DOORBELL_OFFSET
#define HSP_H2C_TPM_L0_DOORBELL_SMN_BASE    HSP_APERTURE_BASE + HSP_H2C_TPM_L0_DOORBELL_OFFSET

//
// C2H_HSP_L0
//
#define HSP_C2H_HSP_L0_OFFSET               0x1000
#define HSP_C2H_HSP_L0_DOORBELL_OFFSET      0x800C
#define HSP_H2C_HSP_L0_DOORBELL_OFFSET      0x8034

#define HSP_C2H_HSP_L0_SMN_BASE             HSP_APERTURE_BASE + HSP_C2H_HSP_L0_OFFSET
#define HSP_C2H_HSP_L0_SMN_SIZE             HSP_MAILBOX_SIZE
#define HSP_C2H_HSP_L0_DOORBELL_SMN_BASE    HSP_APERTURE_BASE + HSP_C2H_HSP_L0_DOORBELL_OFFSET
#define HSP_H2C_HSP_L0_DOORBELL_SMN_BASE    HSP_APERTURE_BASE + HSP_H2C_HSP_L0_DOORBELL_OFFSET

//
// C2H_HSP_L1
//
#define HSP_C2H_HSP_L1_OFFSET               0x2000
#define HSP_C2H_HSP_L1_DOORBELL_OFFSET      0x8010
#define HSP_H2C_HSP_L1_DOORBELL_OFFSET      0x8038

#define HSP_C2H_HSP_L1_SMN_BASE             HSP_APERTURE_BASE + HSP_C2H_HSP_L1_OFFSET
#define HSP_C2H_HSP_L1_SMN_SIZE             HSP_MAILBOX_SIZE
#define HSP_C2H_HSP_L1_DOORBELL_SMN_BASE    HSP_APERTURE_BASE + HSP_C2H_HSP_L1_DOORBELL_OFFSET
#define HSP_H2C_HSP_L1_DOORBELL_SMN_BASE    HSP_APERTURE_BASE + HSP_H2C_HSP_L1_DOORBELL_OFFSET

//
// C2H_HSP_L2
//
#define HSP_C2H_HSP_L2_OFFSET               0x3000
#define HSP_C2H_HSP_L2_DOORBELL_OFFSET      0x8014
#define HSP_H2C_HSP_L2_DOORBELL_OFFSET      0x803C

//
// C2H_HSP_L3
//
#define HSP_C2H_HSP_L3_OFFSET               0x4000
#define HSP_C2H_HSP_L3_DOORBELL_OFFSET      0x8018
#define HSP_H2C_HSP_L3_DOORBELL_OFFSET      0x8040

typedef enum {
  X86_VTL0_TPM        = 0,
  X86_VTL0_SPI        = 1,
  X86_VTL1_SPI        = 2,
} HSP_MAILBOX_CHANNEL;


#define MAKE_SPI_ERROR(x) (0xF0000000 | (x))

typedef enum _SPI_REQUEST_FUNCTION {
  SPI_REQ_TEST_MESSAGE        = 1,    // ID set by AMD ROM
  SPI_REQ_SECURE_UNLOCK       = 2,    // ID set by AMD ROM
  SPI_REQ_PCR_EXTEND          = 3,    // ID set by AMD ROM
  SPI_REQ_LOAD_RUNTIME        = 4,    // ID set by AMD ROM
  SPI_REQ_HASH_DATA           = 5,    // AMD refers to this as SPI_REQ_HASH_DATA_PCRExtend
  SPI_REQ_LOCALITY_CONTROL    = 6,
  SPI_REQ_TCG_INIT            = 7,
  SPI_SET_OUTPUT_IRQ          = 0x40,
  SPI_ENABLE_OUTPUT_IRQ       = 9,    // not implemented yet
  SPI_MAX_FUNCTION            = 0xFF,
} SPI_REQUEST_FUNCTION;

// taken from Van Gogh Crypto ROM document
typedef enum SPI_STATUS_CODE
{
    // Response Not Set
    SPI_RESPONSE_NULL                       = 0xFFFFFFFF, // 0 already taken for SUCCESS

    // Success
    SPI_RESPONSE_SUCCESS                    = 0x00000000,

    // Generic Error
    SPI_CALL_FAILED                         = MAKE_SPI_ERROR (1),

    // Planned function/channel combination that is not currently implemented.

    // This is a placeholder for code to be implemented and should

    // not happen in released firmware
    SPI_NOT_IMPLEMENTED                     = MAKE_SPI_ERROR (2),

    // bad argument to function
    SPI_INVALID_ARGUMENT                    = MAKE_SPI_ERROR (3),

    // Hardware state is inconsistent with request
    SPI_HARDWARE_ILLEGAL_STATE              = MAKE_SPI_ERROR (4),

    // illegal request code, or a valid request sent via wrong channel.

    // For example, X86 making a PSP-only request is considered Insecure.

    // This is an assertion the indicates a code bug (either in the caller or HSP)

    // or a security attack.
    SPI_INSECURE_REQUEST                    = MAKE_SPI_ERROR (7),

    // Think assertion failure.  Some sort of random internal error that shouldn't occur.
    SPI_INTERNAL_ERROR                      = MAKE_SPI_ERROR (8),

    // 20-49 reserved for SPI_REQ_TCG_INIT

    // subranged as:

    // * 20-39 reserved for log parsing errors:
    SPI_TCG_INIT_E_HEADER_ELEMENT_MALFORMED     = MAKE_SPI_ERROR (20),
    SPI_TCG_INIT_E_NOT_CRYPTO_AGILE             = MAKE_SPI_ERROR (21),
    SPI_TCG_INIT_E_HEADER_VERSION               = MAKE_SPI_ERROR (22),
    SPI_TCG_INIT_E_MISSING_HASH_COUNT           = MAKE_SPI_ERROR (23),
    SPI_TCG_INIT_E_BAD_HASH_COUNT               = MAKE_SPI_ERROR (24),
    SPI_TCG_INIT_E_BAD_HASH_TABLE               = MAKE_SPI_ERROR (25),
    SPI_TCG_INIT_E_UNSUPPORTED_HASH             = MAKE_SPI_ERROR (26),
    SPI_TCG_INIT_E_WRONG_HASH_SIZE              = MAKE_SPI_ERROR (27),
    SPI_TCG_INIT_E_TRUNCATED_EVENT              = MAKE_SPI_ERROR (28),

   // hash found in an event that was not declared in the header
    SPI_TCG_INIT_E_UNREGISTERED_HASH            = MAKE_SPI_ERROR (29),
    SPI_TCG_INIT_E_DUPLICATE_HASH               = MAKE_SPI_ERROR (30),
    SPI_TCG_INIT_E_MISSING_HASH                 = MAKE_SPI_ERROR (31),
    SPI_TCG_INIT_E_NOT_SUFFICIENT_BUFFER        = MAKE_SPI_ERROR (33),
    SPI_TCG_INIT_E_INVALIDARG                   = MAKE_SPI_ERROR (34),

    // * 40-49 reserved for tpm_crb use

    // log was parsed okay, but didn't match the expected HW PCR3
    SPI_TCG_LOG_VERIFICATION_FAILED         = MAKE_SPI_ERROR (40),

    // the log buffer must not cross a 128MB physical boundary

    // returned by the memory mapper.
    SPI_TCG_LOG_UNMAPPABLE                  = MAKE_SPI_ERROR (41),

    //
    // 50-59 reserved for SPI_REQ_HASH_DATA
    //

    // Attempt to call SPI_HASH_DATA while Locality 4 is locked via SPI_REQ_LOCALITY_CONTROL
    SPI_HASH_DATA_LOCKED                    = MAKE_SPI_ERROR (50),

    // Attempt to call SPI_HASH_DATA prior to TPM Startup
    SPI_HASH_DATA_NOT_STARTED               = MAKE_SPI_ERROR (51),

    //
    // 60-69 reserved for SPI_REQ_LOCALITY_CONTROL
    //
} SPI_STATUS;


//
// SMN aperture registers
//
#define SMN_NBIO_IOHUB_FASTREG_APERTURE_ADDRESS         0x13b07000UL
#define SMN_NBIO_IOHUB_FASTREG_BASE_ADDR_HI_ADDRESS     0x13b10304UL
#define SMN_NBIO_IOHUB_FASTREG_BASE_ADDR_LO_ADDRESS     0x13b10300UL

#pragma  pack (push, 1)

// HSP Tcg Logs
typedef struct {
  EFI_STATUS  Status;
  UINT32      TcgLogsSize;
} TCGLOGS, *PTCGLOGS;

//
// TPM Buffer Protocol
//
typedef struct {
  volatile UINT32                 Request;          ///< Request                          // 0  (0x00) 0x40
  volatile UINT32                 Status;           ///< Status                           // 4  (0x04) 0x44
  volatile UINT32                 Cancel;           ///< Cancel                           // 8  (0x08) 0x48
  volatile UINT32                 Start;            ///< Start                            // 12 (0x0C) 0x4C
  UINT64                          InterruptControl; ///< InterruptControl                 // 16 (0x10) 0x50
  UINT32                          CommandSize;      ///< CommandSize                      // 24 (0x18) 0x58
  EFI_PHYSICAL_ADDRESS            CommandAddress;   ///< CommandAddress                   // 28 (0x1C) 0x5C
  UINT32                          ResponseSize;     ///< ResponseSize                     // 36 (0x24) 0x64
  EFI_PHYSICAL_ADDRESS            ResponseAddress;  ///< ResponseAddress                  // 40 (0x28) 0x68
} TPM_CONTROL_AREA;

typedef struct _TPM_BUFFERS {
  UINT32            TPM_LOC_STATE;      // READ-ONLY                                      // 0  (0x00)
  UINT8             Reserved0[4];                                                         // 4  (0x04)
  UINT32            TPM_LOC_CTRL;       // UNUSED                                         // 8  (0x08)
  UINT32            TPM_LOC_STS;        // UNUSED                                         // 12 (0x0C)
  UINT8             Reserved1[0x20];                                                      // 16 (0x10)
  UINT64            TPM_CRB_INTF_ID;    // UNUSED                                         // 48 (0x30)
  UINT64            TPM_CRB_CTRL_EXT;   // UNUSED                                         // 56 (0x38)
  TPM_CONTROL_AREA  ControlArea;                                                          // 64 (0x40)
  UINT8             Reserved2[0x10];                                                      // 112 (0x70)
  UINT8             CommandResponseBuffer[HSP_COMMANDRESPONSE_BUFFER_SIZE]; // 4096-128   // 128 (0x80)
} TPM_BUFFERS, *PTPM_BUFFERS;

//
// SPI_REQUEST Protocol
//
typedef UINT8 SHA256_HASH[32];

typedef struct _SPI_REQUEST {
  SPI_REQUEST_FUNCTION  RequestType;  //enum that selects the called function
  SPI_STATUS            Status;       // Output return status
} SPI_REQUEST, *PSPI_REQUEST;

typedef struct _PCR_EXTEND_REQUEST {
  SPI_REQUEST   Request;
  UINT16        PCR;              // PCR number to extend
  SHA256_HASH   Digest;           // Digest value to be extended
} PCR_EXTEND_REQUEST, *PPCR_EXTEND_REQUEST;

typedef struct _LOAD_RUNTIME_REQUEST {
  SPI_REQUEST          Request;
  EFI_PHYSICAL_ADDRESS pRuntime;  // Location in DRAM holding HSP runtime
  UINT32               Size;      // Size in bytes of HSP runtime
} LOAD_RUNTIME_REQUEST, *PLOAD_RUNTIME_REQUEST;

typedef struct _SET_OUTPUT_IRQ {
  SPI_REQUEST          Request;
  UINT32               IrqRegisterSMNAddress; // For VanGogh & RMB, it is always 0x28000020 IOAPIC0MMIOx00000020 (IOAPIC::IRQ_PIN_ASSERTION_REGISTER)
  UINT8                Irq[5];                // Irq[0] Irq number for Mailbox Channel 0 / TPM
                                              // Irq[1] Irq number for Mailbox Channel 1 / ACPI -> (Channels[0])
                                              // Irq[2] Irq number for Mailbox Channel 2 / ACPI -> (Channels[1])
                                              // Irq[3] Irq number for Mailbox Channel 3 / ACPI -> (Channels[2])
                                              // Irq[4] Irq number for Mailbox Channel 4 / ACPI -> (Channels[3])
  UINT8                Reserved[7];           // Reserved for padding (64 bit aligned)
} SET_OUTPUT_IRQ, *PSET_OUTPUT_IRQ;

/**
 * @brief HSP Save PCIe Bridge/Device Configuration Space Struct
 * @detail HSP Save PCIe Bridge/Device Configuration Space Struct
 * for S3 Resume use.
 */
typedef struct _HSP_PCIE_BRIDGE_REG {
  UINT16  MemoryBase;                   /// Offset: 0x20 ~ 0x21, Bridge Memory Base Register
  UINT16  MemoryLimit;                  /// Offset: 0x22 ~ 0x23, Bridge Memory Limit Register
} HSP_PCIE_BRIDGE_REG;

typedef struct _HSP_PCIE_DEVICE_REG {
  UINT32  Bar0;                          /// Offset: 0x10 ~ 0x13, Device Bar0 Register
} HSP_PCIE_DEVICE_REG;

typedef struct _HSP_SAVE_PCIE_CONFIG_SPACE {
  UINT32 CheckFlag;                     /// Check Flag
  HSP_PCIE_BRIDGE_REG HspPcieBridgeReg; /// HSP Bridge Struct
  HSP_PCIE_DEVICE_REG HspPcieDeviceReg; /// HSP Device Struct
} HSP_SAVE_PCIE_CONFIG_SPACE;

#pragma  pack (pop)

extern EFI_GUID gAmdHspPciCfgS3SaveUefiVariableGuid;

#endif /* __HSP_H__ */

