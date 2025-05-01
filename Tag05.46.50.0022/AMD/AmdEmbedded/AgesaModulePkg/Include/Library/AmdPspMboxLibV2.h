/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
/* $NoKeywords:$ */
/**
 * @file
 *
 * PSP Mailbox related functions Prototype definition
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  PSP
 * @e \$Revision: 312133 $   @e \$Date: 2015-02-03 02:47:45 +0800 (Tue, 03 Feb 2015) $
 */
#ifndef _PSP_MBOX_H_
#define _PSP_MBOX_H_
#pragma pack (push, 1)

#include <Protocol/GraphicsOutput.h>

typedef union _MBOX_BUFFER MBOX_BUFFER;

#define PSP_NOTIFY_SMM        0xdd    ///< @todo Move it out to other file

#define PSP_WAITSTS_MAX_COUNT 10000   ///< @todo Need check this value's correctness

#define SX_REQ_FLAG_FAKE_S3   1       ///< Fake S3 is requested

//======================================================================================
//
// Define PSP generic return code
//
//======================================================================================
//
// Return Error codes
typedef enum {
    BL_OK                               = 0x0,   // General - Success

    // Generic Error codes                       // (0x01 to 0x0F)
    BL_ERR_GENERIC                      = 0x1,   // Generic Error Code
    BL_ERR_MEMORY                       = 0x2,   // Generic Memory Error
    BL_ERR_BUFFER_OVERFLOW              = 0x3,   // Buffer Overflow
    BL_ERR_INVALID_PARAMETER            = 0x4,   // Invalid Parameter
    BL_ERR_DATA_LENGTH                  = 0x5,   // Invalid Data Length
    BL_ERR_DATA_ALIGNMENT               = 0x6,   // Data Alignment Error
    BL_ERR_NULL_PTR                     = 0x7,   // Null Pointer Error
    BL_ERR_FUNCTION_NOT_SUPPORTED       = 0x8,   // Unsupported Function
    BL_ERR_INVALID_SERVICE_ID           = 0x9,   // Invalid Service ID
    BL_ERR_INVALID_ADDRESS              = 0xA,   // Invalid Address
    BL_ERR_OUT_OF_RESOURCES             = 0xB,   // Out of Resource Error
    BL_ERR_TIMEOUT                      = 0xC,   // Timeout
    BL_ERR_DATA_ABORT                   = 0xD,   // data abort exception
    BL_ERR_PREFETCH_ABORT               = 0xE,   // prefetch abort exception
    BL_ERR_BOUNDARY_CHECK               = 0xF,   // Out of Boundary Condition Reached
    BL_ERR_DATA_CORRUPTION              = 0x10,  // Data corruption
    BL_ERR_INVALID_COMMAND              = 0x11,  // Invalid command
    BL_ERR_FUSE_ERROR_BLOWNFUSE         = 0x7F,  // Fuse Error Blown Fuse on the Master Socket
    BL_ERR_FUSE_ERROR_BLOWNFUSE_P1      = 0x83,  // Fuse Error Blown Fuse on the Slave Socket
} BL_RETCODE;

//======================================================================================
//
// Define Mailbox Command
//
//======================================================================================
//
/// Mbox command list. Only one command can be send till target process it;
/// the only exception is Abort command that BIOS may send in case of timeout etc.
///
typedef enum {
  MboxCmdRsvd                   = 0x00,   ///< Unused

  MboxBiosCmdDramInfo           = 0x01,   ///< Bios -> PSP: Memory DRAM information (ie. PspBuffer address etc)
  MboxBiosCmdSmmInfo            = 0x02,   ///< Bios -> PSP: Bios will provide SMM inf -
                                          /// SmmBase, PspSmmDataRegion,PspSmmDataRegionLength,
                                          /// SoftSmiValue, SoftSmiPort
  MboxBiosCmdSxInfo             = 0x03,   ///< Bios -> PSP: Sx transition info (S3, S5)
  MboxBiosCmdRsmInfo            = 0x04,   ///< Bios -> PSP: Resume transition info
                                          /// (BiosResume Vector, Size of resume code)
  MboxBiosCmdFtpmQuery          = 0x05,   ///< Bios -> PSP: Bios Find supported feature
  MboxBiosCmdBootDone           = 0x06,   ///< Bios -> PSP: Bios is done with BIOS POST
  MboxBiosCmdClearS3Sts         = 0x07,   ///< Bios -> PSP: Inform PSP clear S3ExitReset
  MboxBiosS3DataInfo            = 0x08,   ///< Bios -> PSP: Bios will send this command
                                          /// to inform PSP to save the data needed
                                          /// to restore memory during resume from S3
  MboxBiosCmdNop                = 0x09,   ///< Bios -> PSP: Bios will send this NOP command
                                          /// to indicate to PSP that is is done servicing PSP SMM request
  MboxBiosCmdHSTIQuery          = 0x14,   ///< Bios -> PSP: Command to get HSTI bit field
                                          /// representing the security state of the SoC from the PSP
  MboxBiosCmdGetVersion         = 0x19,   ///< Bios -> PSP: Bios will send this command to get PSP FW image version
  MboxBiosCmdLockDFReg          = 0x1B,   ///< Bios -> PSP: Bios will send this command to lock DF registers
  MboxBiosCmdClrSmmLock         = 0x1C,   ///< Bios -> PSP: Command to clear SMMLock Register
                                          /// in C6 private memory region
  MboxBiosCmdSetApCsBase        = 0x1D,   ///< Bios -> PSP: Bios will send the CS BASE value for AP threads
  MboxBiosCmdKvmInfo            = 0x1E,   ///< Bios -> PSP: KVM required information
  MboxBiosCmdLockSpi            = 0x1F,   ///< BIOS -> PSP: BIOS will send this command to lock SPI,
                                          /// X86 need in SMM mode when send this command
  MboxBiosCmdScreenOnGpio       = 0x20,   ///< BIOS -> PSP: Report the FCH GPIOs for early turn on eDP panel in S0i3
  MboxBiosCmdSpiOpWhiteList     = 0x21,   ///< BIOS -> PSP: BIOS send SPI operation whitelist to lock SPI,
                                          /// X86 need in SMM mode when send this command
  MboxBiosCmdRasEinj            = 0x22,   ///< BIOS -> PSP: BIOS send RAS Error Injection action
  MboxBiosCmdStartArs           = 0x24,   ///< Bios -> PSP: Command to Start ARS for RAS feature
  MboxBiosCmdStopArs            = 0x25,   ///< Bios -> PSP: Command to Stop ARS for RAS feature
  MboxBiosCmdSetBootPartitionId = 0x26,   ///< BIOS -> PSP: BIOS send this command to PSP
                                          /// to write the ACTIVE_BOOT_PARTITION_ID register
  MboxBiosCmdPspCapsQuery       = 0x27,   ///< Bios -> PSP: Bios check PSP NVRAM health
  MboxBiosCmdArmorEnterSmmOnlyMode    = 0x28,   ///< Bios -> PSP: Request secures the SPI Controller
  MboxBiosCmdArmorEnforceWhitelist    = 0x29,   ///< Bios -> PSP: Loads the whitelist into the PSP
  MboxBiosCmdArmorExecuteSpiCommand   = 0x2A,   ///< Bios -> PSP: Request execute SPI command
                                                /// provide in TSEG comm buffer
  MboxBiosCmdArmorSwitchCsMode        = 0x2B,   ///< Bios -> PSP: Request PSP firmware switch SPI controller chip select
  MboxBiosCmdDrtmInfoId         = 0x2C,   ///< BIOS -> PSP: BIOS send this command to PSP
                                          /// about the information DRTM feature required
  MboxBiosCmdLaterSplFuse       = 0x2D,   ///< Bios -> PSP: Bios send this command to PSP
                                          /// for SPL fuse for anti-rollback feature
  MboxBiosCmdDtpmInfo                 = 0x2E,   ///< Bios -> PSP: Command to get dTPM status and event log.
  MboxBiosCmdValidateManOsSignature   = 0x2F,   ///< BIOS -> PSP: Validate signature of manageability OS image
                                                /// based on header passed by BIOS
  MboxBiosCmdLockFCHReg         = 0x30,   ///< Bios -> PSP: Bios send this command to lock FCH PM and IOMux registers
  MboxBiosCmdPostDrtmInfoQuery  = 0x31,   ///< BIOS -> PSP: To query updated drtm information in post drtm phase.
  MboxBiosCmdSignValidateHmacDataPreSmm   = 0x34,   ////< BIOS -> PSP:Sign data with hmac256
                                                    /// or validate its signature at pre-SMM lock stage
  MboxBiosCmdSignValidateHmacDataSmm      = 0x35,   ////< BIOS -> PSP:Sign data with hmac256
                                                    /// or validate its signature in SMM
  MboxBiosCmdGetBootPartitionId = 0x36,   ///< BIOS -> PSP: BIOS send this command to PSP
                                          /// to read the ACTIVE_BOOT_PARTITION_ID register
  MboxBiosCmdLockeSPIReg        = 0x37,   ///< BIOS send this command to PSP to fence off eSPI 0x00:0F
  MboxBiosCmdValidateBinary     = 0x38,   ///< BIOS send this command to PSP
                                          /// to let PSP tOS to validate the integrity of Micro Code Patch
  MboxBiosCmdSetRpmcAddress     = 0x39,   ///< BIOS -> PSP: Bios send this command to PSP
                                          /// to decide which RPMC address to use. Only used in product line.
                                          /// A warm reset should be issued after receiving BIOS_MBOX_OK (0) from PSP tOS, otherwise BIOS do nothing.
  MboxBiosCmdSetGpioFencing     = 0x3A,   ///< BIOS -> PSP: Bios send this command to PSP to set GPIO Fencing. Only used in product line.
  MboxBiosCmdNotifyIntrusionEvent  = 0x3C,   ///< BIOS -> PSP: Bios send this command to notify PSP of an intrusion
  MboxBiosCmdGetIntrusionLog       = 0x3D,   ///< BIOS -> PSP: Bios send this command to PSP to retrieve intrusion detection log
  MboxBiosCmdCfgIntrusionDetection = 0x3E,   ///< BIOS -> PSP: Bios send this command to PSP to configure intrusion detection
  MboxBiosCmdSendIvrsAcpiTable  = 0x3F,   ///< BIOS -> PSP: Bios send IVRS buffer to PSP, PSP save it, then AMDSL will use another command to retrieve it back to the buffer.
  MboxBiosCmdTa                 = 0x40,   ///< Bios -> PSP: Send command to TA
  MboxBiosCmdAcpiRasEinj        = 0x41,   ///< Bios -> PSP: Command to Enable/Disable ACPI based RAS EINJ Feature
  MboxBiosCmdQueryTCGLog        = 0x42,   ///< Bios -> PSP: Get TCG Log
  MboxBiosCmdQuerySplFuse       = 0x47,   ///< Bios -> PSP: Get the current value of SPL_F (FW_ROLLBACK_CNT) fuse value
  MboxBiosCmdManageabilityCfg   = 0x49,   ///< Bios -> PSP: BIOS command to PSP to get the Manageability related Config
  MboxBiosCmdDisablePsb         = 0x4A,   ///< Bios -> PSP: Send command to disable Platform Secure Boot (PSB)
  MboxBiosCmdUsbConfig          = 0x4B,   ///< Bios -> PSP: USB Config
  MboxBiosCmdMpmPciAccess       = 0x4D,   ///< BIOS Command to PSP to enabled or disable the PCI access to MPM according to the input parameter
  MboxBiosCmdStbVerbosity       = 0x4F,   ///< BIOS Command to PSP to send Smart trace buffer Verbosity Control
  MboxBiosCmdArmorEnterSmmOnlyMode2 = 0x50,  ///< Bios -> PSP: Enter RomArmor mode2
  MboxBiosCmdArmorSpiTransaction    = 0x51,  ///< Bios -> PSP: BIOS submit SPI transaction to PSP
  MboxBiosCmdDeferredPsbFuse    = 0x52,   ///< Bios -> PSP: Send command to deferred Platform Secure Boot (PSB) fuse
  MboxBiosCmdLoadWlanFw         = 0x53,   ///< Bios -> PSP: PSP load decompress & authenticate the WLAN FW from temporary buffer to the allocated buffer
  MboxBiosCmdQuerySplValue      = 0x54,   ///< PSP -> BIOS: Get loaded SPL value
  MboxBiosCmdEncryptRangeConfigure  = 0x57,  ///< Bios -> PSP: Range Encryption - Configure Range
  MboxBiosCmdEncryptRangeQuery  = 0x58,   ///< Bios -> PSP: Range Encryption - Query Range
  MboxBiosCmdEncryptRangeLock   = 0x59,   ///< Bios -> PSP: Range Encryption - Range Lock
  MboxBiosCmdSetOcFuse          = 0x5B,   ///< Bios -> PSP: set OC fuse
  MboxBiosCmdSetCfg             = 0x5D,   ///< Bios -> PSP: Set BIOS generic configuration to PSP
  MboxBiosCmdLoadFwConfig       = 0x60,   ///< Bios -> PSP: load PROM21 and TURNER FW Configuration
  MboxBiosCmdGetSffsFuse        = 0x63,   ///< Bios -> PSP: Get SFFS fuse (SFFS = System Firmware Feature Sets)
  MboxBiosCmdSetPcdValues       = 0x6B,   ///< Bios -> PSP: BIOS send this command to pass X86 PCD setting to PSP Firmware by using the structure PCD_VALUES
  MboxBiosCmdSmmLock            = 0x6C,   ///< Bios -> PSP: SMM Lock

  // P2C command
  MboxPspCmdSpiGetAttrib        = 0x81,   ///< PSP -> BIOS: Get location of PSP NVRam region
  MboxPspCmdSpiSetAttrib        = 0x82,   ///< PSP -> BIOS: Get location of PSP NVRam region
  MboxPspCmdSpiGetBlockSize     = 0x83,   ///< PSP -> BIOS: Get Block size info
  MboxPspCmdSpiReadFV           = 0x84,   ///< PSP -> BIOS: Read PSP NVRAM firmware volume
  MboxPspCmdSpiWriteFV          = 0x85,   ///< PSP -> BIOS: Write PSP NVRAM firmware volume
  MboxPspCmdSpiEraseFV          = 0x86,   ///< PSP -> BIOS: Erase PSP NVRAM firmware volume
  MboxPspCmdRpmcIncMc           = 0x88,   ///< PSP -> BIOS: Increment Monotonic Counter
  MboxPspCmdRpmcReqMc           = 0x89,   ///< PSP -> BIOS: Request Monotonic Counter
  MboxPspCmdArsStatus           = 0x8A,   ///< PSP -> BIOS: ARS Status

  MboxCmdAbort                  = 0xfe,   ///< Abort the last command (BIOS to PSP in case of timeout etc)
} MBOX_COMMAND;
#define MboxBiosCmdPsbAutoFusing  0x21

#define PSP_MBOX_SUCCESS                  0x00        ///< Mail box success
#define PSP_MBOX_INVALID_PARAMETER        0x01        ///< Invalid Parameter
#define PSP_MBOX_CRC_ERROR                0x02        ///< CRC error
#define PSP_MBOX_COMMAND_PROCESS_ERROR    0x04        ///< Command process error
#define PSP_MBOX_UNSUPPORTED              0x08        ///< Unsupported
#define PSP_MBOX_SPI_BUSY_ASYNC           0x0A        ///< SPI Bus is busy due Async SPI implementation
#define PSP_MBOX_SPI_BUSY_FPR_OWNED       0x0B        ///< SPI Bus is busy due SPI finger print take the ownership

typedef UINT32  P2C_MBOX_STATUS;
///
/// standard header structure for additional parameter
///
typedef struct {
  UINT32    TotalSize;                      ///< Total Size of MBOX_BUFFER (including this field)
  UINT32    Status;                         ///< Status value if any:e
  //UINT8   ReqBuffer[x];                   ///< X byte long Request buffer for additional parameter.
} MBOX_BUFFER_HEADER;


//======================================================================================
//
// Below define Request buffer for various commands. This structure is based on Command
//
//======================================================================================
#define SMM_TRIGGER_IO                      0
#define SMM_TRIGGER_MEM                     1

#define SMM_TRIGGER_BYTE                    0
#define SMM_TRIGGER_WORD                    1
#define SMM_TRIGGER_DWORD                   2

/// Define structure of SMM_TRIGGER_INFO
typedef struct {
  UINT64  Address;                          ///< Memory or IO address (Memory will be qword, IO will be word)
  UINT32  AddressType;                      ///< SMM trigger typr - Perform write to IO/Memory
  UINT32  ValueWidth;                       ///< Width of value to write (byte write, word write,..)
  UINT32  ValueAndMask;                     ///< AND mask of value after reading from the address
  UINT32  ValueOrMask;                      ///< OR Mask of value to write to this address.
} SMM_TRIGGER_INFO;
/// SMM register information
typedef struct {
  UINT64  Address;                          ///< Memory or IO address (Memory will be qword, IO will be word)
  UINT32  AddressType;                      ///< SMM trigger typr - Perform write to IO/Memory
  UINT32  ValueWidth;                       ///< Width of value to write (byte write, word write,..)
  UINT32  RegBitMask;                       ///< AND mask of value after reading from the address
  UINT32  ExpectValue;                      ///< OR Mask of value to write to this address.
} SMM_REGISTER;
/// All SMM register report via SMMINFO command
typedef struct {
  SMM_REGISTER SmiEnb;                      ///< Register information for SmiEnb
  SMM_REGISTER Eos;                         ///< Register information for EOS
  SMM_REGISTER FakeSmiEn;                   ///< Register information for FakeSmiEn
  SMM_REGISTER Reserved[5];                 ///< Reserved
} SMM_REGISTER_INFO;
///
/// structure of ReqBuffer for MboxBiosCmdSmmInfo mailbox command
///
typedef struct {
  UINT64            SMMBase;                  ///< SMM TSeg Base
  UINT64            SMMMask;                  ///< Mask of SMM SMM TSeg
  UINT64            PSPSmmDataRegion;         ///< PSP region base in Smm space
  UINT64            PspSmmDataLength;         ///< Psp region length in smm space
  SMM_TRIGGER_INFO  SmmTrigInfo;              ///< Information to generate SMM
  SMM_REGISTER_INFO SmmRegInfo;               ///< Information describe the SMM register information
  UINT64            PspMboxSmmBufferAddress;  ///< Address of MBOX_BUFFER structure used for BIOS-to-PSP commands sent from SMM mode
  UINT64            PspMboxSmmFlagAddress;    ///< Address of 32-bit flag indicating the CPU is in SMM mode
} SMM_REQ_BUFFER;

/// MBOX buffer for SMM info
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  SMM_REQ_BUFFER      Req;                  ///< Reques buffer
} MBOX_SMM_BUFFER;


///
/// structure of ReqBuffer for MboxBiosCmdSxInfo mailbox command
///
typedef struct {
  UINT8   SleepType;                         ///< Inform which sleep state the system is going to
  UINT8   Flag;                              ///< Additional flag to describe Sleep Type status
                                             /// BIT1 for S3 means Fake S3 for Capsule Update
  UINT16  Reserved;                          /// Reserved, set to 0
} SX_REQ_BUFFER;

/// MBOX buffer for Sx info
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  SX_REQ_BUFFER       Req;                  ///< Request buffer
} MBOX_SX_BUFFER;


///
/// structure of ReqBuffer for MboxBiosCmdRsmInfo mailbox command
///
typedef struct {
  UINT64  ResumeVecorAddress;               ///< Address of BIOS resume vector
  UINT64  ResumeVecorLength;                ///< Length of BIOS resume vector
} RSM_REQ_BUFFER;

/// MBOX buffer for RSM info
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  RSM_REQ_BUFFER      Req;                  ///< Req
} MBOX_RSM_BUFFER;

/// CAPS_REQ_BUFFER structure
typedef struct {
  UINT32 Capabilities;                      ///< PSP Writes capabilities into this field when it returns.
} CAPS_REQ_BUFFER;

/// SFFS Fuse Bitmap
#ifndef SFFS_CPU_OC_FUSED
  #define SFFS_CPU_OC_FUSED                   BIT14  ///< if CPU has been OC fused
#endif

/// MBOX buffer for Capabilities Query
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  CAPS_REQ_BUFFER     Req;                  ///< Req
} MBOX_CAPS_BUFFER;

// Bitmap defining Psp Capabilities, C2P command 0x27
#define PSP_NVRAM_HEALTH                  BIT0    ///0 - not healthy, resert needed; 1 - healthy,don't need reset
#define PSP_NVRAM_USING_RPMC_PROTECTION   BIT1    ///0: PSP storage is not using RPMC protection, 1: PSP storage is using RPMC protection

/// MBOX buffer for PSP Capabilities
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  UINT32              PspCapabilities;       ///< PspNvramStatus
} MBOX_PSP_CAPS_BUFFER;

#define HSTI_STATE_MASK                           0xF ///only 4 bits are true HSTI state
// Bitmap defining HSTI bitfield
#define HSTISTATE_PSP_SECURE_EN                   BIT0 ///< Fuse SecureEnable to ensure silicon level secure
#define HSTISTATE_PSP_PLATFORM_SECURE_EN          BIT1 ///< Boot Integrity Support
#define HSTISTATE_PSP_DEBUG_LOCK_ON               BIT2 ///< Protection against external hardware debugger
#define HSTISTATE_PSP_CUSTOMER_KEY_LOCK_ON        BIT3 ///< This bit field is programmed to meet OS current implementation on automatic Device Encryption
//This bit is originally used to report if KVM capabilities are supported, and then repurposed for Pro SKU check
//because KVM capabilities are provided based on Pro SKU or non-Pro SKU in PSP
#define PSP_RYZEN_PRO_SKU_FLAG                    BIT4
#define PSP_TSME_STATUS                           BIT5  //Tsme status reported by PSP FW
#define PSP_ANTI_ROLLBACK_STATUS                  BIT7  //Report if firmware anti-rollback enforcement is enabled
#define PSP_RPMC_PRODUCTION_ENABLED               BIT8  // fuse bit was set for RPMC Root Key Provision done at Manufacturing
#define PSP_RPMC_SPIROM_AVAILABLE                 BIT9  // Reports the availability of RPMC supported SPIROM
#define PSP_ROM_ARMOR_ENFORCED                    BIT11  // Reports ROM-Armor (v1 or v2) is enforced
#define CPU_OC_CAPABLE                            BIT15  // Report if the part is OC capable, starting from CGL

/// MBOX buffer for Get Tcg Logs
typedef struct {
  MBOX_BUFFER_HEADER    Header;                    ///< Header
  UINT32                TcgsBufferPhyAddressLo;    ///< Address provided by PSP, default zero
  UINT32                TcgsBufferPhyAddressHi;    ///< Address provided by PSP, default zero
  UINT32                TcgsBufferSize;            ///< Size
} MBOX_GET_TCG_LOGS;

/// MBOX buffer for HSTI
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  UINT32              HSTIState;            ///< HSTIState
} MBOX_HSTI_STATE;

/// MBOX buffer for Exit BIOS info
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
} MBOX_DEFAULT_BUFFER;

/// GENERIC_UIN32_DATA structure
typedef struct {
  UINT32 Value;                      ///< generic UINT32 value
} GENERIC_UIN32_DATA;

/// MBOX buffer to get uint32 data from PSP
typedef struct {
  MBOX_BUFFER_HEADER      Header;             ///< Header
  GENERIC_UIN32_DATA      Data;               ///< UINT32 data
} MBOX_GET_UINT32_BUFFER;

/// MBOX buffer for ActiveBootPartitionId
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  UINT32              BootPartitionId;      ///< Buffer composed of multiple FW version
} MBOX_ACTIVE_BOOT_PARTITION_ID_BUFFER;

/// MBOX buffer for FAR enablement and fusing SPL value
typedef struct {
  MBOX_BUFFER_HEADER  Header;              ///< Header
  UINT32              SplValue;            ///< Initial SPL Value
} MBOX_FIRMWARE_ANTI_ROLLBACK_BUFFER;


/// Max MBOX buffer for SPL fuse
#define MAX_SOCKET  8
typedef struct {
  //MAX_SOCKET is valid for server program with multiple socket support
  //In client program only 1st element of SplFuse [] [4] is valid
  UINT32              SplFuse [MAX_SOCKET] [4];         ///< SPL fuse value, upto 128 bits
                                                // SplFuse[0], bits[31:0]
                                                // SplFuse[1], bits[63:32]
                                                // SplFuse[2], bits[95:64]
                                                // SplFuse[3], bits[127:96]
  UINT32              NumOfSockets;             // Only valid  for server program with multiple socket support
} GET_SPL_BUFFER;
/// MBOX buffer for SPL fuse
typedef struct {
  MBOX_BUFFER_HEADER  Header;             ///< Header
  GET_SPL_BUFFER      SplBuffer;          ///< Spl Buffer
} MBOX_GET_SPL_BUFFER;


/// MBOX buffer to read loaded SPL
typedef struct {
  MBOX_BUFFER_HEADER  Header;             ///< Header
  UINT32              SplValue;           ///< Loaded Spl value
} MBOX_GET_SPL_VALUE_BUFFER;


/// CS Base Buffer
typedef struct {
  UINT32  Value;
} AP_CS_BASE_BUFFER;

/// MBOX buffer for ApCsBase
typedef struct {
  MBOX_BUFFER_HEADER      Header;                 ///< Header
  AP_CS_BASE_BUFFER       Req;                    ///< Req
} MBOX_AP_CS_BASE_BUFFER;


/// Version Buffer
typedef struct {
  UINT32              PspFwVersion;       // PSP FW Version
  UINT32              AgesaFwVersion;     // AGESA FW Version
  UINT32              APPBVersion;        // AGESA APPB FW Version
  UINT32              APCBVersion;        // AGESA APCB FW Version
  UINT32              APOBVersion;        // AGESA APOB FW Version
  UINT32              SmuVersion;         // SMU FW Version
} VERSION_BUFFER;

/// Generic MBOX buffer
typedef struct {
  MBOX_BUFFER_HEADER    Header;               ///< Header
  UINT32                BufferAddrLo;         ///< Lower 32 bit address of the buffer
  UINT32                BufferAddrHi;         ///< Higher 32 bit address of the buffer
  UINT32                BufferSize;           ///< Size of the buffer
} MBOX_GENERIC_BUFFER;

/// MBOX buffer for MboxBiosCmdSetRpmcAddress
typedef struct {
  MBOX_BUFFER_HEADER    Header;                 ///< Header
  UINT32                RpmcAddressToUse;       ///< tell PSP which RPMC address to program, value range 0 to 3
} MBOX_SET_RPMC_ADDRESS_BUFFER;

/// MBOX buffer for GetVersion
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  VERSION_BUFFER      VersionBuffer;        ///< Buffer composed of multiple FW version
} MBOX_GET_VERSION_BUFFER;

typedef UINT32 TEE_STATUS;                  ///< Status of TEE command
#define TEE_OK                                  0x000     ///< TEE command success
#define TEE_ERR_SOC_ADDRESS_MAPPING             0x10C     ///< TEE command error of SOC address mapping
#define TEE_ERR_NWD_INVALID_SESSION             0x10E     ///< TEE command error of invalid session
#define TEE_ERR_NWD_TOO_MANY_MAPPED_BUFFERS     0x10F     ///< TEE command error of too many mapped buffers
#define TEE_ERR_NWD_INVALID_WSM_LEN             0x110     ///< TEE command error of invalid length

/// MBOX buffer for MboxBiosCmdTa
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  UINT64              TaCmdBufferAddress;      ///<Address of TA command buffer
  UINT64              TaCmdBufferSize;         ///< Size of TA command
} MBOX_TA;

#define KVM_DMA_SIZE    (16 * 1024* 1024)
#define KVM_NIC_COHERENCY_SIZE  (256 * 1024)

/// BIOS to KVM mailbox
typedef struct {
  MBOX_BUFFER_HEADER                        Header;               ///< Header
  UINT32                                    KvmDmaAddrLo;               ///< KVM DMA Addr
  UINT32                                    KvmDmaAddrHi;               ///< KVM DMA Addr
  UINT32                                    KvmDmaSize;                 ///< KVM DMA size
} MBOX_KVM;

/// BIOS to PSP mailbox to validate the signature of manageability OS image
typedef struct {
  MBOX_BUFFER_HEADER                        Header;               ///< Header
  UINT32                                    ManOsAddrLo;               ///< memory buffer base of the manOS
  UINT32                                    ManOsAddrHi;               ///< memory buffer base of the manOS
  UINT32                                    ManOsBufferSize;                 ///< memory buffer size
} MBOX_MANOS;

#define MAX_NUM_WAKEUP_GPIO                 30
typedef struct {
  UINT32                                    NumOfGpio;                  ///< numbre of GPIO
  UINT32                                    Gpio[MAX_NUM_WAKEUP_GPIO];  ///< Gpio List, filled with GPIO pin number,
                                      /// e.g. Gpio[0]= 3, Gpio[1]=5, denote GPIO Pin #3 (AGPIO3) & #5 (AGPIO5_DEVSLP0)
} SCREENON_GPIO_LIST;

typedef struct {
  UINT16                                    AcpiPm1StatusMask;   ///the address is from PMx00000060
                                                                 /// (FCH::PM::ACPIPM1EVTBLK)
                                                                ///< example 0x0010 means PSP need
                                                                /// to check ACPI [Pm1Status] bit 8.
                                                                /// if set early screen needed
  UINT16                                    Reserved;
  UINT32                                    AcpiGpe0StatusMask;  ///the address is from PMx00000068
                                                                /// (FCH::PM::ACPIGPE0BLK)
                                                                ///< example 0x0012 means PSP need to
                                                                /// check ACPI [EventStatus] bit 8 and bit 2.
                                                                /// if set early screen needed
} SCREENON_GPE_LIST;

typedef struct {
  UINT32                                     Version;             ///< current version is 1, used for future update
  UINT32                                     Flag;                // 1 = SCREENON_GPIO_LIST; 2 = SCREENON_GPE_LIST
  union {
    SCREENON_GPIO_LIST  Gpio;
    SCREENON_GPE_LIST   Gpe;
  } ControlList;
} SCREENON_CONTROL_LIST;

/// MboxBiosCmdScreenOnGpio
typedef struct {
  MBOX_BUFFER_HEADER                        Header;                  ///< Header
  SCREENON_CONTROL_LIST                     List;                    ///< Gpio List
} MBOX_SCREENON_GPIO;

//MboxBiosCmdSpiOpWhiteList
// OpCode[7:0]   - this is simply the "whitelist" Opcode
// Options[1:0]  -
//               0              0              This OpCode does not require additional check
//               0              1              This is WRITE command, firmware should make sure the address does not cross cacheline.
//                                               We will rely on hw to enforce the address checking
//               1              0              This is READ command, firmware should make sure the address does not cross cacheline.
//                                               We will rely on hw to enforce the address checking
//               1              1              This Opcode is a ERASE command with address.  This Option will require a
//                                               SIZE field.  Firmware will check the address in index FIFO:
//                                             a)  Does not cross write protection range
//                                             b)  Aligns with SIZE definition accordingly.  For example, if the SIZE is 256B, lower
//                                                 8 bit address must be all 0s.
// Size[2:0]     - This field defines the size of ERASE:
//                  000b == 256B,
//                  001b == 512B,
//                  010b == 1KB,
//                  011b == 2KB,
//                  100b == 4KB,
//                  101b == 8KB,
//                  110b == 16KB,
//                  111b == 32KB
typedef struct {
  UINT8                                    OpCode;      ///< SPI OpCodes
  UINT8                                    Options;     ///< Option of SPI OpCodes
  UINT8                                    Size;        ///< Defines the size of ERASE
  UINT8                                    Frequency;   ///< Reserved
  UINT8                                    IoModeDummy; ///< Bits 0-2 are used for io_mode (0-3) and bit 3 is used for dummy_mode support flag (0 or 1)
  UINT8                                    AddrMode;    ///< 0, 1, 2 representing no-addressing, 24bit, or 32 bit respectively
  UINT16                                   Dummy;
} SPI_OP;

#define MAX_NUM_SPI_OP                      32
typedef struct {
  UINT32                                    NumOfOp;                    ///< Number of SPI Operation
  SPI_OP                                    Ops[MAX_NUM_SPI_OP];        ///< SPI opreations
} SPI_OP_LIST;

typedef struct {
  MBOX_BUFFER_HEADER                        Header;                     ///< Header
  SPI_OP_LIST                               List;                       ///< SPI OP List
  UINT32                                    ROMAddrRng2;                ///< ROMAddrRng2 of LPC PCI 0x6C
  UINT32                                    RomProtect[4];              ///< RomProtect of LPC PCI 0x50-0x5C
  UINT8                                     SpiStatusReg;               ///< the SPI status register address
  UINT8                                     BusyBitMask;                ///< the busy bit mask within the SpiStatusReg
  UINT16                                    Reserved;                   ///< reserved for future use
} MBOX_SPI_OP_WHITELIST;

//MboxBiosCmdRasEinj: 0x22
#define RAS_EINJ_ENABLE_INJECTION               (UINT32)((MboxBiosCmdRasEinj << 16) | 0x1)
#define RAS_EINJ_BEGIN_INJECTION_OPERATION      (UINT32)((MboxBiosCmdRasEinj << 16) | 0x2)
#define RAS_EINJ_END_OPERATION                  (UINT32)((MboxBiosCmdRasEinj << 16) | 0x3)
#define RAS_EINJ_EXECUTE_OPERATION              (UINT32)((MboxBiosCmdRasEinj << 16) | 0x4)

//MboxBiosCmdAcpiRasEinj: 0x41
#define RAS_ACPI_EINJ_ENABLE_INJECTION          (UINT32)((MboxBiosCmdAcpiRasEinj << 16) | 0x01)
#define RAS_ACPI_EINJ_DISABLE_INJECTION         (UINT32)((MboxBiosCmdAcpiRasEinj << 16) | 0x02)


typedef struct {
  MBOX_BUFFER_HEADER                        Header;                     ///< Header
  UINT32                                    Action;                   ///< Action of EINJ interface, will have structure following by depend on the action ID.
} MBOX_RAS_EINJ;

typedef struct {
  UINT64 SysMemAddr;
  UINT32 UmcRegBase;
  UINT16 EccInjVector;
  UINT8  ChipSelect;
  UINT8  SocketId;
} RAS_EINJ_EXECUTE_OPERATION_STRUCT;

//PSP ARS command
typedef enum {
  ArsActionStart = 0,                     ///< Start Address Range Scrubber
  ArsActionStop,                          ///< Stop Address Range Scrubber
} ARS_ACTIONS;

typedef struct {
  UINT64  SPAStartAddress;                ///< SPA (System Physical Address) start address
  UINT64  SPALength;
  UINT64  ScrubGranularity;
  BOOLEAN Priority;
} ARS_BUFFER;

typedef struct {
  MBOX_BUFFER_HEADER    Header;           ///< Header
  ARS_BUFFER            OperationBuf;
} MBOX_ARS_CMD_BUF;

//PSP Rom Armor
typedef enum {
  ArmorEnterSmmOnlyMode   = 0,            ///Send request Enter SMM Only Mode
  ArmorEnforceWhitelist,                  ///Load the whitelist of allowed SPI commands
  ArmorExecuteSpiCommand,                 ///Send execute SPI command request
  ArmorSwitchCsMode                       ///Switch Chip Select mode
} ROM_ARMOR_ACTIONS;

typedef struct {
  UINT64  TsegAddress;                    ///< TSEG Command Buffer address
  UINT32  ChipSelect;                     ///< SPI controller Chip Select. (Reserved)
} ROM_ARMOR_CMD_BUFFER;

typedef struct {
  MBOX_BUFFER_HEADER    Header;           ///< Header
  ROM_ARMOR_CMD_BUFFER  RomArmorCmdBuffer;
} MBOX_ROM_ARMOR_CMD_BUF;

//PSP dTPM
typedef struct {
  UINT32              DesiredConfig;      // Out - dTPM configuration requested by ABL
  UINT32              ConfigStatus;       // Out - 0 - success. non-zero failure.
  UINT32              LogDataSize;        // In/Out - size of LogData buffer
  UINT64              LogData;            // In - LogData buffer address
} MBOX_DTPM_INFO;

typedef struct {
  MBOX_BUFFER_HEADER    Header;           ///< Header
  MBOX_DTPM_INFO        MboxDtpmInfo;
} MBOX_DTPM_INFO_BUF;

typedef struct {
  UINT32                SmmSupvAddrLo;            ///< Address of SmmSupv driver PE image
  UINT32                SmmSupvAddrHi;            ///< Address of SmmSupv driver PE image
  UINT32                SmmSupvSize;            ///< Size of SmmSupv driver
  UINT32                SmmSupvDirAddrLo;   ///< Address of SMM Supervisor directory Header
  UINT32                SmmSupvDirAddrHi;   ///< Address of SMM Supervisor directory Header
  UINT32                SmmSupvDirSize;   ///< Size of SmmSupv directory
  UINT32                SmmPolicyDataAddrLo;      ///< Address of SmmPolicy data
  UINT32                SmmPolicyDataAddrHi;      ///< Address of SmmPolicy data
  UINT32                SmmPolicyDataSize;      ///< Size of SmmPolicy data
  UINT32                SmmSupvSmmEntryAddrLo;       ///< Address of SmmSupvSmmEntry
  UINT32                SmmSupvSmmEntryAddrHi;       ///< Address of SmmSupvSmmEntry
  UINT32                SmmSupvSmmExitAddrLo;       ///< Address of SmmSupvSmmExit
  UINT32                SmmSupvSmmExitAddrHi;       ///< Address of SmmSupvSmmExit
} DRTM_INFO;

//MboxBiosCmdDrtmInfoId
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  DRTM_INFO           DrtmInfo;
} MBOX_DRTM_INFO_BUFFER;

typedef struct {
  UINT32                IvrsBufferAddrLo;            ///< Low 32 bit Address of Ivrs Buffer
  UINT32                IvrsBufferAddrHi;            ///< High 32 bit Address of Ivrs Buffer
  UINT32                IvrsAcpiTableSize;            ///< Size of Ivrs Buffer
} IVRS_ACPI_TABLE_INFO;


//MboxBiosCmdSendIvrsAcpiTable
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  IVRS_ACPI_TABLE_INFO           IvrsAcpiTableInfo;
} MBOX_Ivrs_Acpi_Table_INFO_BUFFER;

// Bit fields of "ManageabilityConfig" word from MBOX_MANAGEABILITY strucure
#define BIOS2PSP_MPM_WM_FUSE_ENABLE_SHIFT  0  // "Manageability Enabled by Fuse" bit
#define BIOS2PSP_MPM_WM_FUSE_ENABLE_SIZE  1
#define BIOS2PSP_MPM_WM_FUSE_ENABLE_MASK  (BIOS2PSP_MPM_WM_FUSE_ENABLE_SIZE << BIOS2PSP_MPM_WM_FUSE_ENABLE_SHIFT)
#define BIOS2PSP_MPM_AIM_T_SHIFT     1  // AIM-T bit
#define BIOS2PSP_MPM_AIM_T_SIZE      1
#define BIOS2PSP_MPM_AIM_T_MASK      (BIOS2PSP_MPM_AIM_T_SIZE << BIOS2PSP_MPM_AIM_T_SHIFT)
#define BIOS2PSP_MPM_WM_BIOS_ENABLE_SHIFT  2  // "Manageability Enabled by BIOS Option" bit
#define BIOS2PSP_MPM_WM_BIOS_ENABLE_SIZE  1
#define BIOS2PSP_MPM_WM_BIOS_ENABLE_MASK  (BIOS2PSP_MPM_WM_BIOS_ENABLE_SIZE << BIOS2PSP_MPM_WM_BIOS_ENABLE_SHIFT)
#define BIOS2PSP_MPM_WIRED_KVM_SHIFT    3  // "Wired KVM Enable" bit
#define BIOS2PSP_MPM_WIRED_KVM_SIZE      1
#define BIOS2PSP_MPM_WIRED_KVM_MASK      (BIOS2PSP_MPM_WIRED_KVM_SIZE << BIOS2PSP_MPM_WIRED_KVM_SHIFT)
#define BIOS2PSP_MPM_WIRELESS_KVM_SHIFT    4  // "Wireless KVM Enable" bit
#define BIOS2PSP_MPM_WIRELESS_KVM_SIZE    1
#define BIOS2PSP_MPM_WIRELESS_KVM_MASK    (BIOS2PSP_MPM_WIRELESS_KVM_SIZE << BIOS2PSP_MPM_WIRELESS_KVM_SHIFT)
#define BIOS2PSP_MPM_TIMER_ACPOWER_SHIFT  5  // "Events: MPM Timer Expiry OR AC-Power Plugin events" bit
#define BIOS2PSP_MPM_TIMER_ACPOWER_SIZE    1
#define BIOS2PSP_MPM_TIMER_ACPOWER_MASK    (BIOS2PSP_MPM_TIMER_ACPOWER_SIZE << BIOS2PSP_MPM_TIMER_ACPOWER_SHIFT)
#define BIOS2PSP_MPM_WLAN_DASH_PACKET_SHIFT  6  // "Event: WLAN-DASH Packet Arrival" bit
#define BIOS2PSP_MPM_WLAN_DASH_PACKET_SIZE  1
#define BIOS2PSP_MPM_WLAN_DASH_PACKET_MASK  (BIOS2PSP_MPM_WLAN_DASH_PACKET_SIZE << BIOS2PSP_MPM_WLAN_DASH_PACKET_SHIFT)

typedef struct {
  UINT32              ManageabilityConfig;// Manaeability Configuration and event word
  UINT32              MpmDramAddrLo;   // bits [31:0] of 64-bit address for reserved MPM DRAM region
  UINT32              MpmDramAddrHi;  // bits [63:32] of 64-bit address for reserved MPM DRAM region

} MANAGEABILITY_CFG;

//MboxBiosCmdManageabilityCfg
typedef struct {
  MBOX_BUFFER_HEADER  Header;               ///< Header
  MANAGEABILITY_CFG   Cfg;                  ///< Cfg structure
} MBOX_MANAGEABILITY_CFG_BUFFER;

/// GENERIC_CONFIG structure
typedef struct {
  UINT32  ConfigId; ///< type of CMD_CONFIG_ID
  UINT32  Args0;    ///< first argument, set to 0 if no need
  UINT32  Args1;    ///< second argument, set to 0 if no need
  UINT32  Args2;    ///< third argument, set to 0 if no need
  UINT32  Args3;    ///< fourth argument, set to 0 if no need
} GENERIC_CONFIG;   /// MBOX buffer to get/set generic config from/to PSP

typedef struct {
  MBOX_BUFFER_HEADER  Header; ///< Header
  GENERIC_CONFIG      Cfg;    ///< generic config
} MBOX_GENERIC_CONFIG_BUFFER;

typedef enum {
  CMD_CONFIG_ID_ZSTATE_ENABLE = 0x2,
  CMD_CONFIG_ID_OPEN_CS_NO_SPI_LOCK = 0x3,
  CMD_CONFIG_ID_ENABLE_SLVERR_TIMEOUT_HANDLE = 0x4,
} CMD_CONFIG_ID;

//PSP Disable PSB
#define PSB_DISABLEMENT_SUCCESS                 (0x0)
#define PSB_DISABLEMENT_FAILED                  (0x1)
#define PSB_ALREADY_DISABLED                    (0x2)
#define PSB_ALREADY_ENABLED                     (0x3)

#define PSB_STATE_PSB_NOT_ENABLED               (0)
#define PSB_STATE_PSB_DISABLED                  (1)
#define PSB_STATE_PSB_ENABLED                   (3)
#define PSB_DISABLE_VERIFY_ID                   (0x50534244)

typedef struct {
  MBOX_BUFFER_HEADER    Header;           ///< Header
  UINT32                Result;
  UINT32                PsbDisableVerifyId;
} MBOX_DIS_PSB_CMD_BUFFER;

///MboxBiosCmdLoadWlanFw
typedef struct {
  UINT32                TempDramAddrLo;         // bits [31:0] of 64-bit address for temporary DRAM region, used to hold WLAN FW to be loaded
  UINT32                TempDramAddrHi;         // bits [63:32] of 64-bit address for temporary DRAM region, used to hold WLAN FW to be loaded
  UINT32                TempDramSize;         // Size of temporary DRAM region
  UINT32                AllocatedDramAddrLo;    // bits [31:0] of 64-bit address for allocated DRAM region, which decompressed and authenticated WLAN FW will copy to
  UINT32                AllocatedDramAddrHi;    // bits [63:32] of 64-bit address for allocated DRAM region, which decompressed and authenticated WLAN FW will copy to
  UINT32                AllocatedDramSize;         // Size of allocated DRAM region
} WLAN_FW_BUF;

typedef struct {
  MBOX_BUFFER_HEADER    Header;               ///< Header
  WLAN_FW_BUF WlanFwBuf;                  ///< Buffer
} MBOX_LOAD_WLAN_FW;

//
// Define Malbox buffer comming from PSP->BIOS
//

///
/// structure of ReqBuffer for MboxPspCmdSpiGetAddress/MboxPspCmdSpiGetAddress  mailbox command
///
typedef struct {
  UINT64    Attribute;                      ///< Inform attribute of SPI part
} SPI_ATTRIB_REQ;

/// MBOX buffer for Spi Get/Set attribute info
typedef struct {
  MBOX_BUFFER_HEADER      Header;           ///< Header
  SPI_ATTRIB_REQ          Req;              ///< Req
} MBOX_SPI_ATTRIB_BUFFER;


///
/// structure of ReqBuffer for MboxPspCmdSpiGetBlockSize mailbox command
///
typedef struct {
  UINT64  TargetNvId;                       ///< Target NV ID
  UINT64  Lba;                              ///< starting LBA
  UINT64  BlockSize;                        ///< Block size of each Lba
  UINT64  NumberOfBlocks;                   ///< Total number of blocks
} SPI_INFO_REQ;

/// MBOX buffer for Spi read block attribute
typedef struct {
  MBOX_BUFFER_HEADER    Header;             ///< Header
  SPI_INFO_REQ          Req;                ///< Req
} MBOX_SPI_INFO_BUFFER;


///
/// structure of ReqBuffer for MboxPspCmdSpiRead/Write mailbox command
///
typedef struct {
  UINT64  TargetNvId;                       ///< Target NV ID
  UINT64  Lba;                              ///< starting LBA
  UINT64  Offset;                           ///< Offset in LBA
  UINT64  NumByte;                          ///< Total byte to read
  UINT8   Buffer[1];                        ///< Buffer to read the data
} SPI_RW_REQ;

/// MBOX buffer for Spi read block attribute
typedef struct {
  MBOX_BUFFER_HEADER    Header;             ///< Header
  SPI_RW_REQ            Req;                ///< Req
} MBOX_SPI_RW_BUFFER;


///
/// structure of ReqBuffer for MboxPspCmdSpiErase mailbox command
///
typedef struct {
  UINT64  TargetNvId;                       ///< Target NV ID
  UINT64  Lba;                              ///< starting LBA
  UINT64  NumberOfBlocks;                   ///< Total number of blocks
} SPI_ERASE_REQ;

/// MBOX buffer for Spi read block attribute
typedef struct {
  MBOX_BUFFER_HEADER    Header;             ///< Header
  SPI_ERASE_REQ         Req;                ///< Req
} MBOX_SPI_ERASE_BUFFER;


///
/// structure of MboxPspCmdRpmcIncMc
///
typedef struct {
  UINT32   CounterAddr;                      ///< Input, CounterAddr for the "Increment Monotonic Counter" SPI command
  UINT32  CounterData;                      ///< Input, CounterData for the "Increment Monotonic Counter" SPI command
  UINT8   Signature[32];                    ///< Input, Signature for the "Increment Monotonic Counter" SPI command
} SPI_RPMC_INC_MC_REQ;

/// MBOX buffer for MboxPspCmdRpmcIncMc
typedef struct {
  MBOX_BUFFER_HEADER    Header;             ///< Header
  SPI_RPMC_INC_MC_REQ   Req;                ///< Req
} MBOX_SPI_RPMC_INC_MC_BUFFER;

///
/// structure of MboxPspCmdRpmcReqMc
///
typedef struct {
  UINT32   CounterAddr;                      ///< Input, CounterAddr for the "Request Monotonic Counter" SPI command
  UINT8   Tag[12];                          ///< Input, Tag for the "Request Monotonic Counter" SPI command
  UINT8   Signature[32];                    ///< Input, Signature for the "Request Monotonic Counter" SPI command
  UINT32  CounterData;                      ///< Output, CounterData for the "Request Monotonic Counter" SPI command
  UINT8   OutputSignature[32];              ///< Output, Returned Signature for the "Request Monotonic Counter" SPI command
} SPI_RPMC_REQ_MC;

/// MBOX buffer for MboxPspCmdRpmcReqMc
typedef struct {
  MBOX_BUFFER_HEADER    Header;             ///< Header
  SPI_RPMC_REQ_MC       Req;                ///< Req
} MBOX_SPI_RPMC_REQ_MC_REQ_BUFFER;

///
/// structure of MboxPspCmdArsStatus
///
typedef struct {
  UINT64    ArsStatus;                            ///< ARS return status
} ARS_STATUS_REQ;

/// MBOX buffer for MboxPspCmdArsStatus
typedef struct {
  MBOX_BUFFER_HEADER      Header;                 ///< Header
  ARS_STATUS_REQ          Req;                    ///< Req
} MBOX_ARS_STATUS_BUFFER;

/// MBOX buffer for USB Configuration
//  UsbConfigureOp        Operation Code for USB configuration
//                          00: Read
//                          01: Write
//  UsbRegisterID         USB Controller and Register
//                          Bit[31:24] USB Controller Number
//                          Bit[23:0] USB Register ID
//  UsbConfigureData0     32-bit Data DW
//                        If Read: Register Value return by PSP
//                        If Write: 32-bit bit-level enables for write data.
//                          0: no modification to the corresponding register bit position.
//                          1: Corresponding Write Data bit to be written to the same register bit position
//  UsbConfigureData1     32-bit Write Data
//                        filtered by the bit-level enables (DW2), to be updated to the register indexed by the register address (DW1).
#define MBOX_USB_CONFIG_OP_RD                      0x00
#define MBOX_USB_CONFIG_OP_WR                      0x01

#define ID_PORT_DISABLE_WRITE_ONCE_CNTR            0x00
#define ID_INTERRUPT_CONTROL_CNTR0                 0x01
#define ID_CURRENT_POWER_STATE_CNTR0               0x02

typedef struct {
  UINT32                UsbConfigureOp;            ///< Operation code
  UINT32                UsbRegisterID;             ///< USB Controller and Register ID
  UINT32                UsbConfigureData0;         ///< Data0
  UINT32                UsbConfigureData1;         ///< Data1
} USB_CONFIG_DATA;

typedef struct {
  MBOX_BUFFER_HEADER    Header;                    ///< Header
  USB_CONFIG_DATA       MboxUsbConfig;             ///< USB Configure Data
} MBOX_USB_CONFIG_BUFFER;

/// MBOX buffer for MboxBiosCmdMpmPciAccess
typedef struct {
  MBOX_BUFFER_HEADER    Header;                    ///< Header
  UINT32                AccessFlag;                ///< Enable (1) or Disable (0) PCI Access to MPM
  UINT32                WirelessFlag;              ///<1: Wireless Lan; 0: Wired Lan
} MBOX_MPM_PCI_ACCESS;


/// MBOX buffer for MboxBiosCmdStbVerbosity
//  Verbosity  #0x0:  STB disabled  #0x1:  Low level verbosity
//          #0x2:  Production level verbosity #0x3:  High level verbosity
typedef struct {
  MBOX_BUFFER_HEADER    Header;                    ///< Header
  UINT32                Verbosity;                 ///< Verbosity
} MBOX_STB_VERBOSITY;

//
// Structure definitions for configure range
//
typedef struct {
  UINT8                              RangeNumber;           ///< Number of the range to be loaded
  UINT8                              Reserved[7];           ///< Reserved - must be zero
  UINT64                             BaseAddress;           ///< Bits [63:21] - set the base address of the range, Bits [20:0] must be zero
  UINT64                             LimitAddress;          ///< Bits [63:21] - set the limit address of the range, Bits [20:0] must be zero
  UINT8                              ReservedKeys[40];      ///< Reserved - used to align keys, must be zero
  UINT8                              DataKey[32];           ///< Key used for encryption of data
  UINT8                              Reserved1[32];          ///< Reserved - should be zero
  UINT8                              IVKey[32];             ///< It can be used for algorithm IV or in the case of AES XTS mode
  UINT8                              Reserved2[32];          ///< Reserved - should be zero
} CONFIG_RANGE_DATA;

typedef struct {
  MBOX_BUFFER_HEADER                 Header;                ///< Header
  CONFIG_RANGE_DATA                  Data;                  ///< Configure range data
} MBOX_CONFIG_RANGE_BUFFER;

//
// Structure definitions for query range
//

typedef struct {
  UINT8                              Version;               ///< Version number supported
  UINT8                              Ranges;                ///< Total number of ranges that can be loaded by BIOS
  UINT8                              Available;             ///< Number of ranges that can still be set up
  UINT8                              Locked;                ///< Indicates if range configuration is locked
} QUERY_RANGE_STATUS;

typedef struct {
  UINT8                              Version;               ///< Version number, of the command, that is understood by the BIOS
  UINT8                              Reserved[7];           ///< Reserved - must be zero
  UINT64                             BufferAddress;         ///< Address for where the PSP should place the return data
  QUERY_RANGE_STATUS                 QueryStatus;           ///< query range status
} QUERY_RANGE_DATA;

typedef struct {
  MBOX_BUFFER_HEADER                 Header;                ///< Header
  QUERY_RANGE_DATA                   Data;                  ///< query range data
} MBOX_QUERY_RANGE_BUFFER;

//======================================================================================
// intrusion detection begin
//======================================================================================
// define the intrusion event
typedef enum {
  CMOS_BATTERY_DRAINED  = 0x1,    ///< Battery exhaustion considered an intrusion event
  CASE_PANEL_OPENED_0   = 0x10,   ///< Specify which of possibly multiple panels/lids was opened
/* Future expansion
  CASE_PANEL_OPENED_1   = 0x20,
  CASE_PANEL_OPENED_2   = 0x40,
  CASE_PANEL_OPENED_3   = 0x80,
*/
} INTRUSION_EVENT;

// when intrusion happened, what actions to take
typedef enum {
  DETECT_INTRUSION         = 0x1,         ///< Whether intrusion detection is enabled or not
  LOG_EVENT                = 0x2,         ///< Whether to log intrusion events
  CLEAR_TPM                = 0x4,         ///< Clear TPM NVStorage upon intrusion event
  INIT_INTRUSION_DETECTION = 0x8,         ///< Initialize this feature for the first time.
                                          ///< If no valid log, create it, do not treat as error.
  POWER_OFF_SYSTEM         = 0x10000000,  ///< Forcibly power off system upon intrusion
/* Future expansion
  SELF_BRICK_SOFT          = 0x20000000,  ///< Delete contents of SPI ROM, force recovery, etc.
  SELF_BRICK_HARD          = 0x40000000,  ///< Burn fuses and render SOC unusable
*/
} INTRUSION_COUNTERMEASURE;

/* Arguments to accompany MboxBiosCmdNotifyIntrusionEvent */
typedef struct {
  MBOX_BUFFER_HEADER    Header;               ///< Header
  UINT32                Version;              ///< Set to 0 for this design
  UINT32                IntrusionEvent;       ///< See INTRUSION_EVENT above
} MBOX_NOTIFY_INTRUSION_EVENT;

/* Structures pertaining to MboxBiosCmdGetIntrusionLog */
// Details about a single intrusion event that has been logged.
typedef struct {
  UINT32              TimeStamp_Lo;    ///< Time stamp, least significant 32 bits
  UINT32              TimeStamp_Hi;    ///< Time stamp, most significant 32 bits
  UINT32              IntrusionEvent;  ///< See INTRUSION_EVENT above
} INTRUSION_EVENT_ENTRY;

// This is the entire logs returned by PSP in response to MboxBiosCmdGetIntrusionLog
typedef struct
{
  UINT32                 IntrusionCountermeasure;  ///< enum INTRUSION_COUNTERMEASURE
  UINT32                 IntrusionCount;           ///< Number of intrusions detected,
                                                   ///< including old events rolled out of EventDetails
  UINT32                 MaximumEventEntries;      ///< Maximum number of entries that can fit in EventDetails array:
                                                   ///< 5 for this spec.
  INTRUSION_EVENT_ENTRY  EventEntries[5];          ///< Always has space for given number of entries
} INTRUSION_EVENT_LOGS;

typedef struct
{
  MBOX_BUFFER_HEADER    Header;               ///< Header
  INTRUSION_EVENT_LOGS  IntrusionLogs;        ///< Intrusion Event Logs
} MBOX_INTRUSION_EVENT_LOGS;

/* Arguments to accompany MboxBiosCmdCfgIntrusionDetection */
typedef struct {
  MBOX_BUFFER_HEADER    Header;                   ///< Header
  UINT32                Version;                  ///< Set to 0 for this design
  UINT32                IntrusionCountermeasure;  ///< See INTRUSION_COUNTERMEASURE above.
                        ///< For now, PSP will only examine the bit at (1 << INIT_INTRUSION_DETECTION).
} MBOX_CFG_INTRUSION_DETECTION;
//======================================================================================
// intrusion detection end
//======================================================================================

/// Union of various structure
typedef union _MBOX_BUFFER {
  MBOX_DEFAULT_BUFFER       Dflt;           ///< Default

  MBOX_SMM_BUFFER           Smm;            ///< Smm
  MBOX_SX_BUFFER            Sx;             ///< Sx
  MBOX_RSM_BUFFER           Rsm;            ///< Rsm
  MBOX_CAPS_BUFFER          Cap;            ///< Cap
  MBOX_GET_VERSION_BUFFER   Ver;            ///< Version
  MBOX_AP_CS_BASE_BUFFER    CsBase;         ///< CS base
  MBOX_SCREENON_GPIO        Gpio;           ///< GPIO
  MBOX_SPI_OP_WHITELIST     WhiteList;      ///< WhiteList
  MBOX_SPI_ATTRIB_BUFFER    SpiAttrib;      ///< SpiAttrib
  MBOX_SPI_INFO_BUFFER      SpiInfo;        ///< SpiInfo
  MBOX_SPI_RW_BUFFER        SpiRw;          ///< SpiRw
  MBOX_SPI_ERASE_BUFFER     SpiErase;       ///< SpiErase
  MBOX_DRTM_INFO_BUFFER     DrtmInfo;       ///< DrtmInfo
  MBOX_USB_CONFIG_BUFFER    UsbConfig;      ///< UsbConfig
  MBOX_GET_SPL_BUFFER       SplBuff;        ///< SPL buffer
  MBOX_CONFIG_RANGE_BUFFER  Config;         ///< Configuration data for encryption
  MBOX_QUERY_RANGE_BUFFER   Query;          ///< Query Range data for encryption
  MBOX_INTRUSION_EVENT_LOGS IntrusionLogs;  ///< Get intrusion event logs
  UINT8                     Rsvd[32];       ///< To Keep the minimal size 32 bytes
} MBOX_BUFFER;

/// 2x Sized Mailbox Buffer for alignment
typedef struct {
  MBOX_BUFFER   Chunk[2];                   ///< Add additional one for alignment
} UNALIGNED_MBOX_BUFFER;

///
/// Buffer is 2X size of aligned structure of size at least 32 bytes long
/// To align in go to next offset past the buffer size & from there back
/// to 32 byte aligned address
#define BALIGN32(p)  ((VOID *) (((UINTN)(VOID*)(p) + 32) & ~0x1F))


///
/// PSP to X86 Buffer exist in SMMRAM
///
typedef struct {
  UINT32 CheckSumValue:8;   ///< 8bits Checksum Value of MBOX_BUFFER
  UINT32 CheckSumEn:1;      ///< Switch for Enable/Disable Checksum
  UINT32 Reserved:22;          ///< Reserved
  UINT32 CommandReady:1; ///< Flag used specify P2C SMI command is ready to handle a new command
} SMI_MBOX_STATUS;

/// PSP to BIOS SMI mailbox structure
typedef struct {
  volatile MBOX_COMMAND     MboxCmd;        ///< Mbox Command 32 bit wide
  volatile SMI_MBOX_STATUS  MboxSts;        ///< Mbox status  32 bit wide
  MBOX_BUFFER               Buffer;         ///< Mailbox buffer right after
} BIOS_MBOX;

typedef struct {
  UINT32                BufferddrLo;      ///< Address of the buffer containing both
  UINT32                BufferAddrHi;     ///< data and signature
  UINT32                BufferSize;       ///< Size of the buffer
  UINT32                DataOffset;       ///< Offset of the data to be signed/validated
  UINT32                DataSize;         ///< Size of the data
  UINT32                SignatureOffset;  ///< Signature offset
  UINT32                SignatureSize;    ///< Signature size
  UINT32                Validate;         ///< Flag to indicate validation request
} HMAC_PROTECTED_DATA;

typedef struct {
  MBOX_BUFFER_HEADER    Header;           ///< Header
  HMAC_PROTECTED_DATA   Data;             ///< HMAC protected data
} MBOX_HMAC_DATA_BUFFER;

//
// Structure definitions for RomArmor2
//
typedef struct _MBOX_ROM_ARMOR_ENFORCE {
  MBOX_BUFFER_HEADER            Header;           /* [in] Command Header */
  UINT32                        FlashSize;        /* [OUT] Flash Size */
  UINT32                        CapsuleUpdate;    /* [in] 0: Normal Boot, 1: Capsule Update */
} MBOX_ROM_ARMOR_ENFORCE;

typedef enum _SPI_TRANSACTION {
  READ_ACCESS     = 1,
  WRITE_ACCESS    = 2,
  ERASE           = 3,
} SPI_TRANSACTION;

typedef struct _MBOX_ROM_ARMOR_FLASH_COMMAND_BUFFER {
  MBOX_BUFFER_HEADER           Header;      /* [in] Command Header */
  SPI_TRANSACTION              Transaction; /* [in] SPI Transaction command to PSP: read, write, erase, etc. */
  UINT8                        *Buffer;     /* [in] pointer of Buffer for Data to read/write or NULL to erase */
  UINT32                       Offset;      /* [in] Offset in the region to read/write or SectorIndex to erase */
  UINT32                       Size;        /* [in] size of buffer to read/write or Number of sectors to erase */
  UINT32                       ReadBack;    /* [in] 1: Read back from SPI-ROM to validate. 0: Ignore */
} MBOX_ROM_ARMOR_FLASH_COMMAND_BUFFER;

/// MBOX buffer for loading PROM21 and TURNER FW Configuration
typedef struct {
  UINT32                OperationCode;              ///< Operation Code 0x01: Decrypt and copy PROM21 FW without PSP header to available memory
                                                    ///<                0x10: Decrypt and copy TURNER FW without PSP header to available memory

  UINT32                AvailableMemoryAddress;     ///< Available memory address
  UINT32                AvailableMemorySize;        ///< Available memory size
} LOAD_FW_CONFIG_DATA;

typedef struct {
  MBOX_BUFFER_HEADER    Header;                    ///< Header
  LOAD_FW_CONFIG_DATA   LoadFwConfigData;          ///< Load PROM21 and TURNER FW Configure Data
} MBOX_LOAD_FW_CONFIG_BUFFER;

/// MboxBiosCmdSetPcdValues - MBOX buffer for passing PCD values to PSP FW
typedef struct {
  UINT8                 PcdSyncFloodToApml;        ///< CBS Option: NBIO Sync-Flood Reporting
  UINT8                 PcdResetCpuOnSyncFlood;    ///< CBS Option: FCH -> Reset After Sync-Flood
  UINT16                Reserved;                  ///< for 4 byte alignment
} PCD_VALUES;

typedef struct {
  MBOX_BUFFER_HEADER   Header;                     ///< Header
  PCD_VALUES           Req;                        ///< Container for passing PCD values
} MBOX_PCD_VALUES;

#pragma pack (pop)
//======================================================================================
//
// Define Mailbox function prototype
//
//======================================================================================
//
//
EFI_STATUS
CheckPspCapsV2 (
  IN OUT   UINT32 *PspCaps
  );

EFI_STATUS
EFIAPI
CheckFtpmCapsV2 (
  IN OUT   UINT32 *Caps
  );

EFI_STATUS
PspMboxBiosQueryUint32 (
  IN       MBOX_COMMAND  Cmd,
  IN OUT   UINT32        *Data
  );

EFI_STATUS
PspMboxBiosGeneralCmd (
  IN       MBOX_COMMAND  Cmd
  );

EFI_STATUS
PspMboxBiosSetOcFuse (
  );

EFI_STATUS
PspMboxBiosCmdS3Info (
  IN UINT64 S3RestoreBufferBase,
  IN UINT64 S3RestoreBufferSize,
  IN OUT   UINT8 *Hmac
   );

EFI_STATUS
PspMboxBiosCmdSmmInfo (
  IN SMM_REQ_BUFFER      *SmmInfoReq
  );

EFI_STATUS
PspMboxBiosCmdSxInfo (
  IN SX_REQ_BUFFER  *SleepRequest,
  IN UINT8          *SmmBuffer,
  IN BOOLEAN        *SmmFlag
  );

EFI_STATUS
PspMboxBiosCmdResumeInfo (
  IN UINT64 S3ResumeAddress,
  IN UINT64 S3ResumeCodeSize
  );

EFI_STATUS
PspMboxBiosCmdExitBootServices ();


EFI_STATUS
PspMboxBiosQueryHSTIState (
  IN OUT   UINT32 *HSTIState
  );

EFI_STATUS
PspMboxBiosQuerySffsFuse (
  IN OUT   UINT32 *SffsFuse
  );

EFI_STATUS
PspMboxBiosSendApCsBase (
  IN       UINT32 ApCsBase
  );

EFI_STATUS
PspMboxBiosGetFwVersions (
  IN OUT   VERSION_BUFFER *FwVersions
  );

EFI_STATUS
PspMboxBiosCmdKvmInfo (
  MBOX_KVM *KvmMboxMemBuffer
  );

EFI_STATUS
PspMboxBiosCmdValidateManOsSignature (
  MBOX_MANOS *ManOsMboxMemBuffer
  );

EFI_STATUS
PspMboxBiosClearSmmLock (
  IN UINT8     *SmmBuffer,
  IN BOOLEAN   *SmmFlag
  );

EFI_STATUS
PspMboxBiosLockDFReg (
  );

/**
 * @brief BIOS send this command to PSP to fence off eSPI 0x00:0F
 *
 * @return EFI_STATUS   0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosLockeSPIReg (
  );

EFI_STATUS
PspMboxBiosLockFCHReg (
  IN  VOID     *FchDataPtr
  );

EFI_STATUS
PspMboxBiosLockGpio (
  IN  VOID     *FchDataPtr
  );


EFI_STATUS
PspMboxBiosLockSpi (
  );

EFI_STATUS
PspMboxBiosScreenOnGpio (
  SCREENON_CONTROL_LIST *GpioList
  );

EFI_STATUS
PspMboxBiosSpiOpWhiteList (
  SPI_OP_LIST *WhiteList,
  IN UINT8     *SmmBuffer,
  IN BOOLEAN   *SmmFlag
  );

EFI_STATUS
PspMboxBiosRasEinj (
  IN UINT32    Action,
  IN VOID      *ActionStruct,
  IN UINT8     *SmmBuffer,
  IN BOOLEAN   *SmmFlag
  );

EFI_STATUS
PspMboxBiosPsbAutoFusing (
  );

EFI_STATUS
PspMboxBiosArs (
  IN UINT8      Action,
  IN ARS_BUFFER *OperationBuf,
  IN UINT8      *SmmBuffer,
  IN BOOLEAN    *SmmFlag
  );


EFI_STATUS
PspMboxBiosGetActiveBootPartitionId (
  IN OUT UINT32            *BootPartitionId
  );

/**
 *
 *
 * Bios send this command to Set ActiveBootPartitionId
 *
 * @param[in]  BootPartitionId     Partition Index of PSP L1 Directory
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosSetActiveBootPartitionId (
  IN UINT32            BootPartitionId
  );

EFI_STATUS
PspMboxPspCapsQuery (
  IN OUT   UINT32 *PspCapabilities
  );

EFI_STATUS
PspMboxBiosDrtmInfo (
  IN DRTM_INFO        *DrtmInfo
  );

EFI_STATUS
PspMboxBiosArmor (
  IN UINT8      Action,
  IN UINT8      ChipSelect,
  IN VOID       *OperationBuf,
  IN BOOLEAN    *SmmFlag
  );

EFI_STATUS
PspMboxGetDTPMData (
     OUT UINT32 *DesiredConfig,
     OUT UINT32 *ConfigStatus,
  IN OUT UINT32 *LogDataSize,
     OUT VOID   *LogData
  );

EFI_STATUS
PspMboxLateSplFusing (
  IN UINT32 SplValue
  );

/**
 * @brief BIOS send the command the PSP to let PSP tOS to validate the integrity of uCode patch
 *
 * @param Buffer        The buffer to store uCode patch
 * @return EFI_STATUS   0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdValidateBinary (
  IN MBOX_GENERIC_BUFFER      *Buffer
  );

  /**
  * Bios send this command to PSP to decide which RPMC address to use.
  * A warm reset should be issued after receiving BIOS_MBOX_OK (0) from PSP tOS, otherwise BIOS do nothing.
  * It's caller's responsbility to issue the warm reset.
  *
  * @param[in]  RpmcAddressToUse           which RPMC address to program. Value 0 to 3
  *
  @retval EFI_STATUS              0: Success, NonZero Error
  * for example: 0x14 means the PSP report RPMC not available
**/
EFI_STATUS
PspMboxBiosCmdSetRpmcAddress (
  UINT32                RpmcAddressToUse
  );

 /**
 *
 *
 * Bios send IVRS buffer to PSP, PSP save it, then AMDSL will use another command to retrieve it back to the buffer.
 *
 * @param[in]  IvrsAcpiTableInfo         Address of IVRS_ACPI_TABLE_INFO Structure
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosSendIvrsAcpiTable (
  IN IVRS_ACPI_TABLE_INFO        *IvrsAcpiTableInfo
  );

EFI_STATUS
PspMboxBiosCmdPostDrtmInfoQuery (
  IN DRTM_INFO *DrtmInfo,
  IN UINT8     *SmmBuffer,
  IN BOOLEAN   *SmmFlag
  );

 /**
 *
 *
 * Bios send command to PSP, PSP return TCG Logs.
 *
 * @param[in]  TcgsBufferAddress         Address of buffer for containing TCG logs
 * @param[in]  TcgsBufferSize            Size of buffer
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxGetTcgLogs (
  IN OUT UINT32               *TcgsBufferPhyAddressLo,
  IN OUT UINT32               *TcgsBufferPhyAddressHi,
  IN OUT UINT32               *TcgsBufferSize
  );

/**
  BIOS will send this command to PSP for getting the current value of SPL_F (FW_ROLLBACK_CNT) fuse value

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxQuerySplFuse (
  IN OUT UINT32               *SplFuse0,
  IN OUT UINT32               *SplFuse1,
  IN OUT UINT32               *SplFuse2,
  IN OUT UINT32               *SplFuse3
  );

/**
 * @brief  BIOS will send this command to PSP for getting the current value of SPL_F (FW_ROLLBACK_CNT) fuse value
 *
 * @param SplBuffer  Buffer to get the spl values
 * @return EFI_STATUS             0: Success, NonZero Error
 */
EFI_STATUS
PspMboxQuerySplFuseMp (
  IN OUT GET_SPL_BUFFER *SplBuffer
  );
/**
  BIOS will send this command to PSP for getting the current value of loaded SPL Value.

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxQuerySplValue (
  IN OUT UINT32               *SplValue
  );

/**
  Sign data with Hmac256 or validate its signature before SMM
 * @param[in]  Data                Point to the data to be protected
  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdSignValidateHmacDataPreSmm (
  IN OUT HMAC_PROTECTED_DATA             *Data
  );

/**
  Sign data with Hmac256 or validate its signature in SMM
 * @param[in]  Data                Point to the data to be protected
 * @param[in]  SmmBuffer           Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr

  @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdSignValidateHmacDataSmm (
  IN OUT HMAC_PROTECTED_DATA  *Data,
  IN UINT8                    *SmmBuffer,
  IN BOOLEAN                  *SmmFlag
  );

 /**
 *
 *
 * Bios send command to PSP, PSP return TCG Logs.
 *
 * @param[in]  TcgsBufferAddress         Address of buffer for containing TCG logs
 * @param[in]  TcgsBufferSize            Size of buffer
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxGetTcgLogs (
  IN OUT UINT32               *TcgsBufferPhyAddressLo,
  IN OUT UINT32               *TcgsBufferPhyAddressHi,
  IN OUT UINT32               *TcgsBufferSize
  );

/**
  BIOS will send this command to PSP for getting Manageability related Config
  * @param[in,out]  Cfg point to the address to hold MANAGEABILITY_CFG structure
    If return status is success, the structure will be filled with information from PSP FW.

   @retval EFI_STATUS              0: Success, NonZero Error
**/
EFI_STATUS
PspMboxManageabilityCfg (
  MANAGEABILITY_CFG   *Cfg
);

/**
   BIOS send this command to PSP for setting SPI controller related Config

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxSetCfg (
  VOID
  );

/**
  BIOS send this command to PSP to update USB configuration

  @param[in out]  UsbConfigureData       Pointer to Usb Configure Data

  @retval EFI_STATUS                     0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosCmdUsbConfig (
  IN OUT USB_CONFIG_DATA    *UsbConfigureData,
  IN     UINT8              *SmmBuffer,
  IN     BOOLEAN            *SmmFlag
  );


#define DISABLE_MPM_PCI_ACC   0
#define ENABLE_MPM_PCI_ACC    1
/**
  BIOS Command to PSP to enabled or disable the PCI access to MPM according to the input parameter

  @param[in out]  AccessFlag       Enable (1) or Disable (0) PCI Access to MPM
  @param[in]      WirelessKvmFlag  1 Wireless KVM Flag, 0 Wired KVM Flag
  @retval EFI_STATUS  0: Success, NonZero Error

**/
EFI_STATUS
PspMboxBiosMpmPciAccess (
  IN UINT32            AccessFlag,
  IN UINT32            WirelessKvmFlag
  );

/**
  Bios send these commands to PSP to disable Platform Secure Boot

  @param[out]      Result              0 - success. non-zero.

  @retval EFI_STATUS                   0: Success, NonZero Error

**/
EFI_STATUS
PspMboxBiosDisablePsb (
     OUT UINT32 *Result
  );

/**
  Bios send these commands to PSP to deferred Platform Secure Boot fuse
  @retval EFI_STATUS                   0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosDeferredPsbFuse (
  VOID
  );

/**
  BIOS Command to PSP to send Smart trace buffer Verbosity Control

  @param[in out]  Verbosity       Valid value: LOW_LEVEL_VERBOSITY,PROD_LEVEL_VERBOSITY,HIGH_LEVEL_VERBOSITY

  @retval EFI_STATUS  0: Success, NonZero Error

**/
EFI_STATUS
PspMboxBiosStbVerbosity (
  IN UINT8            Verbosity
  );

/**
 * Enter RomArmor 2
 *
 * @param[in]  RomArmor2EnforceBuffer RomArmor2 Enforce buffer
 * @param[in]  SmmFlag                Point to allocated SmmFlag
 * @retval EFI_SUCCESS                Initial success
 * @retval Others                     Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdArmorEnterSmmOnlyMode2 (
  IN OUT  MBOX_ROM_ARMOR_ENFORCE *RomArmor2EnforceBuffer,
  IN      BOOLEAN   *SmmFlag
  );

/**
 * Request execute SPI Transction provide in TSEG Transaction buffer.
 *
 * @param[in]  SpiTransction      SPi Transaction buffer
 * @param[in]  SmmFlag            Point to allocated SmmFlag
 * @retval EFI_SUCCESS            Initial success
 * @retval Others                 Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdArmorSpiTransction (
  IN MBOX_ROM_ARMOR_FLASH_COMMAND_BUFFER *SpiTransction,
  IN BOOLEAN   *SmmFlag
  );

/**
 * X86 Allocate temporary memory, copy the WLAN FW from SPI to temp memory, allocate additional 64M DMA memory.
 * Send MboxBiosCmdLoadWlanFw to PSP FW, PSP will do the decompress & authentication, then copy the decompressed image to 64M DMA memory
 *
 * @param[in]  WlanFwBuf        Structure contain WLAN Buffer information
 * @retval EFI_SUCCESS            Initial success
 * @retval Others                 Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdLoadWlanFw (
  IN WLAN_FW_BUF *WlanFwBuf
  );

/**
 * Bios send this command to inform PSP to set up a range for encryption
 *
 * @param[in]  ConfigRange         Point to Config Range Data
 *
 * @retval EFI_STATUS              0: Success, NonZero Error
 *
 */
EFI_STATUS
PspMboxBiosConfigureRange (
  IN CONFIG_RANGE_DATA  *ConfigRange
  );

/**
 * BIOS send this command to PSP to Query Range for encryption

 * @param[in out]  QueryData     Structure contain Query Range Data
 *                               - Version : Version number supported
 *                               - Ranges : Total number of ranges that can be loaded by BIOS
 *                               - Available : Number of ranges that can still be set up
 *                               - Locked : Indicates if range configuration is locked
 * @retval EFI_STATUS            0: Success, NonZero Error
**/
EFI_STATUS
PspMboxBiosQueryRange(
  IN OUT QUERY_RANGE_DATA *QueryData
  );

/**
   BIOS send this command to PSP to check and set a Range Lock

  * @retval EFI_SUCCESS            Indicates range configuration is Unlocked
  * @retval Others                 Indicates range configuration is locked
**/
EFI_STATUS
PspGetBiosRangeLockFlag(
 );

/**
 * @brief BIOS send this command to PSP to load PROM21 and TURNER FW Configuration
 *
 * @param FwConfigBuf  FW configuration buffer
 * @return EFI_STATUS  0: Success, NonZero Error
 */
EFI_STATUS
EFIAPI
PspMboxBiosCmdLoadFwConfig (
  IN LOAD_FW_CONFIG_DATA *FwConfigBuf
  );

/**
 * @brief BIOS sends the specified PCD value to PSP FW
 *
 * @param[in]  PcdValues         Point to PCD_VALUES structure
 *
 * @retval EFI_SUCCESS           The specified PCD values have been sent to PSP FW successfully
 * @retval Others                Error happens during sending PCD_VALUES structure to PSP FW
 **/
EFI_STATUS
PspMboxBiosSetPcdValues (
  IN PCD_VALUES  *PcdValues
  );

/**
 * @brief notify PSP an intrusion event happened
 *
 * @param IntrusionEvent  intrusion event with masks from INTRUSION_EVENT
 * @param[in]  SmmBuffer           Point to allocated Smmbuffer, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmBuffer
 * @param[in]  SmmFlag             Point to allocated SmmFlag, call gPspMboxSmmBufferAddressProtocolGuid->PspMboxSmmFlagAddr
 * @return EFI_STATUS 0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdNotifyIntrusionEvent (
  IN UINT32                IntrusionEvent,
  IN UINT8                 *SmmBuffer,
  IN BOOLEAN               *SmmFlag
  );

/**
 * @brief get intrusion event logs from PSP
 *
 * @param IntrusionEventLogBuffer   intrusion event logs
 * @return EFI_STATUS               0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdGetIntrusionLog (
  IN OUT INTRUSION_EVENT_LOGS      *IntrusionEventLogBuffer
  );

/**
 * @brief configure intrusion detection
 *
 * @param IntrusionCountermeasure   intrusion countermeasure with masks from INTRUSION_COUNTERMEASURE
 * @return EFI_STATUS               0: Success, NonZero Error
 */
EFI_STATUS
PspMboxBiosCmdCfgIntrusionDetection (
  IN UINT32      IntrusionCountermeasure
  );

/**
   BIOS sends the message to PSP for SMM lock

   @retval EFI_STATUS              0: Success, NonZero Error
 **/
EFI_STATUS
PspMboxBiosSmmLock (
  );

#endif //_PSP_MBOX_H_
