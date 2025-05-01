/******************************************************************************
    Copyright 2008 - 2024 Advanced Micro Devices, Inc. All rights reserved.

    AMD is granting you permission to use this software and documentation (if
    any) (collectively, the "Materials") pursuant to the terms and conditions
    of the Software License Agreement included with the Materials.  If you do
    not have a copy of the Software License Agreement, contact your AMD
    representative for a copy.

    You agree that you will not reverse engineer or decompile the Materials, in
    whole or in part, except as allowed by applicable law.

    WARRANTY DISCLAIMER: THE MATERIALS ARE PROVIDED "AS IS" WITHOUT WARRANTY OF
    ANY KIND.  AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY,
    INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT, THAT THE
    MATERIALS WILL RUN UNINTERRUPTED OR ERROR-FREE OR WARRANTIES ARISING FROM
    CUSTOM OF TRADE OR COURSE OF USAGE.  THE ENTIRE RISK ASSOCIATED WITH THE
    USE OF THE MATERIAL IS ASSUMED BY YOU.  Some jurisdictions do not allow the
    exclusion of implied warranties, so the above exclusion may not apply to
    You.

    LIMITATION OF LIABILITY AND INDEMNIFICATION:  AMD AND ITS LICENSORS WILL
    NOT, UNDER ANY CIRCUMSTANCES BE LIABLE TO YOU FOR ANY PUNITIVE, DIRECT,
    INCIDENTAL, INDIRECT, SPECIAL OR CONSEQUENTIAL DAMAGES ARISING FROM USE OF
    THE MATERIALS OR THIS AGREEMENT EVEN IF AMD AND ITS LICENSORS HAVE BEEN
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  In no event shall AMD's total
    liability to You for all damages, losses, and causes of action (whether in
    contract, tort (including negligence) or otherwise) exceed the amount of
    $100 USD.  You agree to defend, indemnify and hold harmless AMD and its
    licensors, and any of their directors, officers, employees, affiliates or
    agents from and against any and all loss, damage, liability and other
    expenses (including reasonable attorneys' fees), resulting from Your use of
    the Materials or violation of the terms and conditions of this Agreement.

    U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are "commercial items", as
    that term is defined at 48 C.F.R. Section 2.101, consisting of "commercial
    computer software" and "commercial computer software documentation", as
    such terms are used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section
    227.7202, respectively. Consistent with 48 C.F.R. Section 12.212 or 48
    C.F.R. Section 227.7202-1 through 227.7202-4, as applicable, the commercial
    computer software and commercial computer software documentation are being
    licensed to U.S.  Government end users (a) only as commercial items and (b)
    with only those rights as are granted to all other end users pursuant to
    the terms and conditions set forth in this Agreement. Unpublished rights
    are reserved under the copyright laws of the United States.

    EXPORT RESTRICTIONS: You shall adhere to all applicable U.S. import/export
    laws and regulations, as well as the import/export control laws and
    regulations of other countries as applicable. You further agrees to not
    export, re-export, or transfer, directly or indirectly, any product,
    technical data, software or source code received from AMD under this
    license, or the direct product of such technical data or software to any
    country for which the United States or any other applicable government
    requires an export license or other governmental approval without first
    obtaining such licenses or approvals; or in violation of any applicable
    laws or regulations of the United States or the country where the technical
    data or software was obtained.  You acknowledge that the technical data and
    software received will not, in the absence of authorization from U.S. or
    local law and regulations as applicable, be used by or exported,
    re-exported or transferred to: (i) any sanctioned or embargoed country, or
    to nationals or residents of such countries; (ii) any restricted end-user
    as identified on any applicable government end-user list; or (iii) any
    party where the end-use involves nuclear, chemical/biological weapons,
    rocket systems, or unmanned air vehicles.   For the most current Country
    Group listings, or for additional information about the EAR or Licensee's
    obligations under those regulations, please refer to the U.S. Bureau of
    Industry and Security's website at http://www.bis.doc.gov/.
*****************************************************************************/

/*! \file */

#ifndef BL_ERRORCODES_H
#define BL_ERRORCODES_H

#include <stdint.h>

/** @enum BL_RETCODE
 * \brief BootLoader Error Codes
 *
 * Bootloader Return Codes, Error only (0x00 through 0x9F)
 */

typedef enum BL_RETCODE
{
    BL_OK                               = 0x00,     /*!< General - Success */

    BL_ERR_GENERIC                      = 0x01,     /*!< Generic Error Code */
    BL_ERR_MEMORY                       = 0x02,     /*!< Generic Memory Error */
    BL_ERR_BUFFER_OVERFLOW              = 0x03,     /*!< Buffer Overflow */
    BL_ERR_INVALID_PARAMETER            = 0x04,     /*!< Invalid Parameter(s) */
    BL_ERR_DATA_LENGTH                  = 0x05,     /*!< Invalid Data Length */
    BL_ERR_DATA_ALIGNMENT               = 0x06,     /*!< Data Alignment Error */
    BL_ERR_NULL_PTR                     = 0x07,     /*!< Null Pointer Error */
    BL_ERR_FUNCTION_NOT_SUPPORTED       = 0x08,     /*!< Unsupported Function */
    BL_ERR_INVALID_SERVICE_ID           = 0x09,     /*!< Invalid Service ID */
    BL_ERR_INVALID_ADDRESS              = 0x0A,     /*!< Invalid Address */
    BL_ERR_OUT_OF_RESOURCES             = 0x0B,     /*!< Out of Resource Error*/
    BL_ERR_TIMEOUT                      = 0x0C,     /*!< Timeout */
    BL_ERR_DATA_ABORT                   = 0x0D,     /*!< data abort exception */
    BL_ERR_PREFETCH_ABORT               = 0x0E,     /*!< prefetch abort exception */
    BL_ERR_BOUNDARY_CHECK               = 0x0F,     /*!< Out of Boundary Condition Reached    */
    BL_ERR_DATA_CORRUPTION              = 0x10,     /*!< Data corruption */
    BL_ERR_UNKNOWN_ASIC                 = 0x11,     /*!< Unknown ASIC type */
    BL_ERR_FWLOAD_DEST_NULL_PTR         = 0x12,     /*!< Null destination pointer */
    BL_ERR_GET_FW_HEADER                = 0x13,     /*!< Failed to retrieve FW header during FW validation    */
    BL_ERR_KEY_SIZE                     = 0x14,     /*!< Key size not supported */
    BL_ERR_WRONG_TYPE                   = 0x15,     /*!< Generic wrong type with switch-case */
    BL_ERR_NO_SUBTYPE                   = 0x16,     /*!< Subtype in unified binary table doesn't match with FWtype in PSP header */
    BL_ERR_CCP_COMPLETE                 = 0x17,     /*!< Not used */
    BL_ERR_FWVALIDATION                 = 0x18,     /*!< Generic FW Validation error */
    BL_ERR_CCP_RSA                      = 0x19,     /*!< RSA operation fail - bootloader */
    BL_ERR_CCP_PASSTHR                  = 0x1A,     /*!< CCP Passthrough operation failed - internal status    */
    BL_ERR_CCP_AES                      = 0x1B,     /*!< AES operation fail */
    BL_ERR_FWSIZE_MISMATCH              = 0x1C,     /*!< Firmware size mismatch */
    BL_ERR_STATE_RESTORE                = 0x1D,     /*!< CCP state restore failed */
    BL_ERR_CCP_SHA                      = 0x1E,     /*!< SHA operation fail - internal status */
    BL_ERR_CCP_ZLIB                     = 0x1F,     /*!< ZLib Decompression operation fail */
    BL_ERR_HMAC_SHA256                  = 0x20,     /*!< HMAC-SHA256 operation fail - internal status*/
    BL_ERR_INVALID_BOOT_SOURCE          = 0x21,     /*!< Booted from boot source not recognized by PSP */
    BL_ERR_DIR_ENTRY_NOT_FOUND          = 0x22,     /*!< Entry not found at requested location */
    BL_ERR_DIR_ENTRY_NULL_PTR           = 0x23,     /*!< Null pointer provided */
    BL_ERR_SPIROM_WRITE_FAIL            = 0x24,     /*!< PSP failed to set the write enable latch */
    BL_ERR_SPIROM_BUSY_TIMEOUT          = 0x25,     /*!< PSP timed out because spirom took too long */
    BL_ERR_HMAC_VALIDATION              = 0x26,     /*!< General - Hmac validation failed */
    BL_ERR_OUT_OF_RANGE                 = 0x27,     /*!< Generic error which data is out of range */
    BL_ERR_MISMATCH_SEC_POL_BIN         = 0x28,     /*!< Security_Policy binary does not match to ASIC */
    BL_ERR_FW_ENC_COMPRESS_CONFLICT     = 0x29,     /*!< FW Encryption option is conflicted with compression */
    BL_ERR_CCP_INIT                     = 0x2A,     /*!< CCP is initializing */
    BL_ERR_CCP_UNSUPPORTED              = 0x2B,     /*!< Unsupported operation requested */
    BL_ERR_DETECT_BOOT_MODE             = 0x2C,     /*!< Valid boot mode wasn't detected */
    BL_ERR_CRYPTO_SHA_PASSTHROUGH       = 0x2D,     /*!< CCP combined SHA-DMA engine error */
    BL_ERR_BINARY_UNSIGNED              = 0x2E,     /*!< Binary is not signed.*/
    BL_ERR_CCP_GENERIC                  = 0x2F,     /*!< Generic CCP error*/
    BL_ERR_CRYPTO_OUT_BUFFER_ON_STACK   = 0x30,     /*!< Specified output buffer is on stack */
    BL_ERR_CRYPTO_INVALID_TRNG          = 0x31,     /*!< Unable to read a valid random number from CCP TRNG */
    BL_ERR_NO_PHY_CORES_PRESENT         = 0x32,     /*!< No physical x86 cores were found on die */
    BL_ERR_NO_PHY_CORE_ENABLED          = 0x33,     /*!< No physical x86 core enabled */
    BL_ERR_NO_PHY_CORES_ACTIVE          = 0x34,     /*!< No physical x86 core active */
    BL_ERR_UNSUP_SYSHUB_TARGET_TYPE     = 0x35,     /*!< SYSHUB mapping memory target type is not supported */
    BL_ERR_UNMAP_PSP_SECURE_REGION      = 0x36,     /*!< Attempt to unmap permanently mapped TLB to PSP secure region */
    BL_ERR_MAPSMN_NULL_POINTER          = 0x37,     /*!< Unable to map an SMN address to AXI space */
    BL_ERR_UNMAPSMN_NULL_POINTER        = 0x38,     /*!< Null pointer provided*/
    BL_ERR_UNMAPSMN_INVALID_ADDR        = 0x39,     /*!< Invalid SMN address */
    BL_ERR_SYSHUBMAP_FAILED             = 0x3A,     /*!< Unable to map a SYSHUB address to AXI space */
    BL_ERR_WLIST_VALIDATION             = 0x3B,     /*!< Whitelist feature - whiteList binary validation fail */
    BL_ERR_WLIST_UNLOCK                 = 0x3C,     /*!< Whitelist feature - whiteList unlocking fail */
    BL_ERR_WLIST_FORMAT_VER             = 0x3D,     /*!< Whitelist feature - whiteList format version incorrect */
    BL_ERR_FUSE_INFO                    = 0x3E,     /*!< Fuse info on all dies don't match */
    BL_ERR_PSP_SMU_MSG_FAIL             = 0x3F,     /*!< PSP sent message to SMU; SMU reported an error */
    BL_ERR_SMU_MSG_MISMATCH             = 0x40,     /*!< SMU message send attempted before response received */
    BL_ERR_RPMC_UNITITIALIZED           = 0x41,     /*!< RPMC Root Key needs to be programmed first */
    BL_ERR_RPMC_SIGNATURE_MISMATCH      = 0x42,     /*!< RPMC Signature mismatch */
    BL_ERR_COMPARE_MISMATCH             = 0x43,     /*!< HASH comparison failed */
    BL_ERR_UART_NOT_PRESENT             = 0x44,     /*!< UART initialization error */
    BL_ERR_RDRAND_SEED_FAIL             = 0x45,     /*!< RDRAND seeding failed */
    BL_ERR_WARM_SRAMHMAC_FAIL           = 0x46,     /*!< Validation of the PSP SRAM image failed on HMAC compare */
    BL_ERR_FW_DECOMP_SIZE_MISMATCH      = 0x47,     /*!< Decompressed FW size did not match UnCompImageSize in header */
    BL_ERR_DISABLE_PROTO_TKEK           = 0x48,     /*!< Proto tkek is disabled on proto mode */
    BL_ERR_DECRYPT                      = 0x49,     /*!< Asic mode is in fra or unsecure mode */
    BL_ERR_NO_DATA                      = 0x4A,     /*!< No data present in a given location */
    BL_ERR_CCP_AES_CMAC                 = 0x4B,     /*!< AES CMAC operation fail */
    BL_ERR_SECURE_UNLOCK_FAIL           = 0x4C,     /*!< Secure unlock error */
    BL_ERR_MISMATCH_BIN                 = 0x4D,     /*!< Binary versions do not match */
    BL_ERR_WLAN_DEVICE_NOT_FOUND        = 0x4E,     /*!< WLAN device cannot be found in any PCIe configuration */
//    BL_ERR_NOT_GLOBAL_MASTER            = 0x4F,     /*!< An operation was invoked that can only be performed by the GM */
    BL_ERR_BIXBY_DETECT_FAILED          = 0x50,     /*!< Bixby chipset not detected */
    BL_ERR_BIXBY_AUTH_FAILED            = 0x51,     /*!< Bixby chipset authentication failed */
    BL_ERR_BIXBY_WRAP_CHECKSUM          = 0x52,     /*!< Bixby chipset checksum failed */
    BL_ERR_BIXBY_PCIE_LINK_NOT_TRAINED  = 0x53,     /*!< Bixby chipset PCIe link is not trained by ABL/DXIO FW */
    BL_ERR_BIXBY_MP1_RESET              = 0x54,     /*!< Failed to reset Bixby MP1 */
    BL_ERR_BIXBY_BOOTLOADER             = 0x55,     /*!< Generic Bixby error code */
//    BL_ERR_SMB_TRANSACTION_FAILED       = 0x56,     /*!< Transaction failed to be started or processed by host, or not completed */
//    BL_ERR_SMB_UNSOLICITED_INTR_RX      = 0x57,     /*!< An unsolicited smbus interrupt was received */
//    BL_ERR_PSP_SMU_UNSUPPORTED_MSG      = 0x58,     /*!< An attempt to send an unsupported PSP-SMU message was made */
//    BL_ERR_PSP_SMU_CORRUPTED_TXFR       = 0x59,     /*!< An error/data corruption detected on response from SMU for sent msg */
//    BL_ERR_MCM_STEADY_UNIT_TEST_FAILED  = 0x5A,     /*!< MCM Steady-state unit test failed */
//    BL_ERR_S3_ENTER_FAILED              = 0x5B,     /*!< S3 Enter failed */
//    BL_ERR_PSP_SMU_RESERVED_NOT_SET     = 0x5C,     /*!< AGESA BL did not set PSP SMU reserved addresses via SVC call */
//    BL_ERR_PSP_SMU_RESERVED_INVALID     = 0x5D,     /*!< N/A */
//    BL_ERR_UNFUSED_PART                 = 0x5E,     /*!< CcxSecBisiEn not set in fuse RAM */
//    BL_ERR_UNIT_TEST_UNEXPECTED_RESULT  = 0x5F,     /*!< Received an unexpected result */
    BL_ERR_SECURITY_GASKET              = 0x60,     /*!< Error with applying security gasket */
//    BL_ERR_MP2_FW_RESPONSE              = 0x61,     /*!< MP2 FW provides error response in the message register */
    BL_ERR_FUSE_READ_ERROR              = 0x62,     /*!< Reading fuse failed */
    BL_ERR_FUSE_ALREADY_BLOWN           = 0x63,     /*!< The BIOS OEM public key of the BIOS was revoked for this platform */
    BL_ERR_FUSE_ERR_TIMEOUT_1           = 0x64,     /*!<Fuse sense operation timed out */
    BL_ERR_FUSE_ERR_TIMEOUT_2           = 0x65,     /*!< Fuse burn sequence/operation timed out waiting for burn done */
    BL_ERR_FUSE_ERR_TIMEOUT_3           = 0x66,     /*!< Fuse burn sequence/operation timed out waiting for burn done */
    BL_ERR_FUSE_ERR_INVALID             = 0x67,     /*!< Fuse invalid operation */
    BL_ERR_FUSE_ERR_BURN_FAILED         = 0x68,     /*!<Fuse burn sequence/operation failed due to internal SOC error */

    BL_ERR_AGESA_DRV_SIZE               = 0x69,     /*!< AGESA driver binary size is exceeded limit allocated in DRAM, need to increase limit */
    BL_ERR_DIAGBL_NOT_PRESENT           = 0X6A,     /*!< The Diag Bootloader is not present in SPI-ROM */
    BL_ERR_DIAG_BL_SEQUENCE             = 0x6B,     /*!< Failed to load PSP Diag BL */

    //OTP key verification error codes
    BL_ERR_CS_SEED_VERIFICATION         = 0x6C,     /*!< Failed to verify CS_SEED */
    BL_ERR_CS_SEED_BOOTROM_VERIFICATION = 0x6D,     /*!< Failed to verify CS_SEED, BootRom portion */
    BL_ERR_CS_SEED_LOWER_VERIFICATION   = 0x6E,     /*!< Failed to verify CS_SEED, Lower portion */
    BL_ERR_BIOS_DIR_MISMATCH            = 0x6F,     /*!< Failed to read from SPI the Bios Directory or Bios Combo Directory */
    BL_ERR_PSP_LV2_HEADER_NOT_MATCH     = 0x70,     /*!< PSP level 2 directory not match expected value. */
    BL_ERR_BIOS_LV2_HEADER_NOT_MATCH    = 0x71,     /*!< BIOS level 2 directory not match expected value. */

    BL_ERR_COMBO_BIOS_HEADER_MISMATCH   = 0x72,     /*!< Failed to read the combo bios header */
    BL_ERR_SPI_ROM_WRITABLE_REGIONS     = 0x73,     /*!< Failure in extracting writable regions information from spi-rom */
    BL_ERR_MAP_SYSHUB_WITH_INLINE_AES   = 0x74,     /*!< SysHub Mapping with Inline AES failed */
    BL_ERR_SVC_ACCESS                   = 0x75,     /*!< Failure due to SVC call */

    BL_ERR_DEBUG_UNLOCK_DISABLED        = 0x76,     /*!< Debug Unlock Disabled*/
    BL_ERR_MISSING_SEC_UNLOCK_PUB_KEY   = 0x77,     /*!< Debug Unlock Pulbic key is missing */
//    BL_ERR_RSMU_TIMEOUT_ENABLE          = 0x78,     /*!< RSMU Timeout enabled */
    BL_ERR_MISSING_SEC_UNLOCK_BIN       = 0x79,     /*!< Missing Security Unlock binary */
    BL_ERR_BOOTROM_CHECKSUM_MISMATCH    = 0x7A,     /*!< Mailbox structure mismatch between bootrom and bootloader */

    //FMR related
    BL_ERR_FMR_ITEM_NOT_FOUND           = 0x7C,
    BL_ERR_FMR_NO_SLOT_AVAILABLE        = 0x7D,

    BL_ERR_SPIROM_CONFIG_ERROR          = 0x7E,     /*!<  SpiRom configuration binary not built correctly*/
    BL_ERR_REQUEST_OR_RELEASE_ACCESS    = 0x7F,     /*!<  Requesting or releasing access has failed*/
    //fTPM driver related
    BL_ERR_FTPMSIZE_EXCEED_LIMIT        = 0x80,     /*!< fTPM binary size exceeds limit allocated in Private DRAM, need to increase the limit */
    //TPM lite (1SP) image
    BL_ERR_TPM_LITE_EXCEED_LIMIT        = 0x81,     /*!< TPM lite(1SP) binary size exceeds limit allocated in Private DRAM, need to increase the limit */
    BL_ERR_BIOS_RTM_SIG_MISSING         = 0x82,     /*!< Bootloader failed to find OEM signature */
    BL_ERR_BIOS_COPY                    = 0x83,     /*!< Error copying BIOS to DRAM */
    BL_ERR_BIOS_VALIDATION              = 0x84,     /*!< Error validating BIOS image signature */
    BL_ERR_OEM_KEY_INVALID_FLAG         = 0x85,     /*!< The validation of the OEM public key token failed */
    BL_ERR_OEM_KEY_INVALID_MODULUS      = 0x86,     /*!< Modulus of OEM Key is invalid */
    BL_ERR_OEM_KEY_INVALID              = 0x87,     /*!< OEM key is invalid */
    BL_ERR_PLATFORM_BINDING             = 0x88,     /*!< The BIOS binding to the fused vendor/model id failed */
    BL_ERR_BIOS_BOOT_FROM_SPI           = 0x89,     /*!< Bootloader detects BIOS request boot from SPI-ROM, which is unsupported for PSB. */
    BL_ERR_OEM_LEAF_KEY_INVALID         = 0x8A,     /*!< OEM BIOS signing Sub-CA leaf key failed signature verification */
    BL_ERR_OEM_LEAF_KEY_INVALID_USAGE   = 0x8B,     /*!< OEM BIOS signing Sub-CA leaf key usage flag violation */
    BL_ERR_OEM_LEAF_KEY_MISSING         = 0x8C,     /*!< OEM BIOS signing Sub-CA leaf key is missing */
    BL_ERR_OEM_KEY_BAD_REV_ID           = 0x8D,     /*!< The BIOS key revision ID is failing anti-rollback check */
    BL_ERR_BIOS_RESET_IMAGE_NOT_FOUND   = 0x90,     /*!< Reset image not found*/
    BL_ERR_FWTYPE_MISMATCH              = 0x91,     /*!< FW type mismatch between the requested FW type and the FW type embedded in the FW binary header*/
    BL_ERR_SECURE_LOCKING               = 0x92,     /*!< Secure Locking */
    BL_RECOVERY_FW_VALIDATION_FAIL,                 /*!< Recovery Firmware Validation Failed */
    BL_ERR_INPUT_NULL_POINTER,                      /*!< Null pointer provided */
    BL_ERR_IP_DISCOVERY                 = 0x95,     /*!< Error processing IP discovery */
    BL_ERR_SPL_ENFORCE                  = 0x96,     /*!< Secure Patch Level is being enforced */
    BL_ERR_SPL_LOOKUP                   = 0x97,     /*!< Secure Patch Level lookup failure */
    BL_ERR_M_TBL_FW_SPL_MISMATCH        = 0x98,     /*!< Main table Firmware Secure Patch Level mismatch */
    BL_ERR_SPL_TBL_HDR_MISMATCH         = 0x99,     /*!< Secure Patch Level Table Header Mismatch */

    //DRTM related
    BL_ERR_DRTMSIZE_EXCEED_LIMIT        = 0x9A,     /*!< DRTM binary size exceeds limit allocated in Private DRAM, need to increase the limit */
    BL_ERR_POSTCODE_MAX_VALUE           = 0x9B,     /*!< The maximum allowable error post code */

    //Address Mode related
    BL_ERR_DECODE_ADDR_MODE             = 0x9C,     /*!< Invalid address mode */

    //HSP related
    BL_ERR_HSP_FAILED                   = 0x9D,     /*!< HSP Failed to respond to psp request */
    BL_ERR_HSP_INVALID_EVENTID,                     /*!< HSP invalid event ID */
    BL_ERR_HSP_INVALID_MSGID            = 0x9E,     /*!< HSP invalid MSGID */
    BL_ERR_HSP_DISABLED                 = 0x9F,     /*!< HSP is Disabled */

    //Key Signing related
    BL_ERR_VALID_KEY_NOT_FOUND          = 0xA0,     /*!< HSP valid key not found */
    BL_ERR_CCP_CLEAR_LSB_SLOT           = 0xA1,     /*!< Error in clearing the CCP LSB slot */

    //Verstage related error
    BL_ERR_VERSTAGE_PSP_HEADER_MISMATCH     = 0xA2, /*!< PSP level directory from verstage not match expected value */
    BL_ERR_VERSTAGE_PSP_DIR_OFFSET_NOT_SET  = 0xA3, /*!< PSP Directory offset is not set by verstage */
    BL_ERR_VERSTAGE_BIOS_DIR_OFFSET_NOT_SET = 0xA4, /*!< BIOS Directory offset is not set by verstage */

    BL_ERR_STACK_CORRUPTION                 = 0xA5, /*!< Stack corruption buffer overflow */
    BL_ERR_SMU_MSG_TIMEOUT                  = 0xB0, /*!< SMU Timeout on MSG ID */
    BL_ERR_MAX                              = INT32_MAX /*!< Maximum possible Error code value */
} BL_RETCODE;

/** @enum BL_TRACECODE
 * \brief BootLoader Trace Codes
 *
 * Bootloader Trace Codes, Success only (0xA0 through 0xFF)
 */
enum bl_tracecode
{
    BL_TRACE_BOOTLOADER_ENTRY                   = 0xA0,     /*!< Updated only PSPFW Status when enetered in BL */
    BL_TRACE_GET_PACKGE_TYPE                    = 0xA1,     /*!< In GetPkgType() */
    BL_TRACE_RSMU_TIMEOUT_ENABLE_START          = 0xA2,     /*!< Starting RsmuTimeoutEnable() */
    BL_TRACE_MAP_SYSHUB_WITH_INLINE_AES_START   = 0xA3,     /*!< Updated only PSPFW Status when derived and programmed syshub inline AES */
    BL_TRACE_C_MAIN_START                       = 0xA4,     /*!< Bootloader successfully entered C Main */
    BL_TRACE_DERIVE_HMAC_KEY_START              = 0xA5,     /*!< HMAC key successfully derived */
    BL_TRACE_DETECT_BOOT_MODE_START             = 0xA6,     /*!< Master got Boot Mode and sent boot mode to all slaves */
    BL_TRACE_SPIROM_INIT_START                  = 0xA7,     /*!< SpiRom Init start */
    BL_TRACE_COPY_BIOS_DIRECTORY_START          = 0xA8,     /*!< BIOS Directory successfully read from SPI to SRAM */
    BL_TRACE_SECURE_CHIP_DETECT                 = 0xA9,     /*!< Soc is Secure */
    BL_TRACE_LOAD_BL_KDB_TO_SRAM                = 0xAA,     /*!< Loading Key Database to SRAM */
    BL_TRACE_SECURE_CHIP_UNLOCK                 = 0xAB,     /*!< ASIC has been securely Unlocked */
    BL_TRACE_APPLY_SECURITY_GASKET_START        = 0xAC,     /*!< Started applying Security Policy/Register Access Policy */
    BL_TRACE_APPLY_SECURITY_TO_UNSECURECHIP_START=0xAD,     /*!< Apply security for unsecure part */
    BL_TRACE_TURN_ON_DISPLAY_START              = 0xAE,     /*!< Starting Gfx */
    BL_TRACE_MBAT_CONFIGURE_START               = 0xAF,     /*!< Beginning mbat configuration */
    BL_TRACE_MBAT_CONFIGURE_COMPLETE            = 0xB0,     /*!< mbat configuration complete */
    BL_TRACE_S3_RESUME_DETECT                   = 0xB1,     /*!< Detected S3 resume */
    BL_TRACE_MP2_SRAM1_INIT                     = 0xB2,     /*!< Initializing Mp2 SRAM1 */
    BL_TRACE_COLD_GENERETE_INLINE_AES_KEY_START = 0xB3,     /*!< GenerateAndProgramInlineAESKey() started */
    BL_TRACE_WARM_RESTORE_INLINE_AES_KEY_START  = 0xB4,     /*!< RestoreInlineAesKey() started */
    BL_TRACE_CONFIG_TMZ_START                   = 0xB5,     /*!< ConfigureTmz() started */
    BL_TRACE_VALIDATE_OEM_PUB_KEY_START         = 0xB6,     /*!< Validation of OEM Publick Key started */
    BL_TRACE_VALIDATED_OEM_KEY                  = 0xB8,     /*!< Completed FW Validation step */
    BL_TRACE_RSMU_TIMEOUT_ENABLE_END            = 0xB9,     /*!< RsmuTimeoutEnable() Finished */
    BL_TRACE_SPL_TABLE_ENFORCE_START            = 0xBA,     /*!< SPL Table enforcing started */
    BL_TRACE_COPY_DMCU_START                    = 0xBB,     /*!< dmcu_copy_to_dram() started */
    BL_SUCCESS_DRTM                             = 0xBC,     /*!< Updated only PSPFW Status when passed DRTM loading */
    BL_TRACE_MCA_IPID_PROGRAM_START             = 0xBD,     /*!< program_mca_ipid() started */
    BL_TRACE_RSMU_SEC_VIOL_LOGGING_ENABLE       = 0xBE,     /*!< RSMU Security violation logging enabled */
    BL_TRACE_LOAD_MPCCX                         = 0xBF,     /*!< Load MPCCX firmware */
    BL_TRACE_LOAD_SMU_TO_SRAM_START             = 0xC0,     /*!< Bootloader start to load SMU FW */
    BL_TRACE_LOAD_SMU_TO_SRAM_DONE              = 0xC1,     /*!< Bootloader successfully loaded SMU FW */
    BL_TRACE_LOAD_MP2                           = 0xC2,     /*!< LoadMp2Fw() started */
    BL_TRACE_LOAD_MP2_SUCCESS                   = 0xC3,     /*!< LoadMp2Fw() Succeeded */
    BL_TRACE_LOAD_AND_RUN_AGESA_START           = 0xC4,     /*!< Load_Run_AgesaBL() started */
    BL_TRACE_AGESA_START_RUN                    = 0xC5,     /*!< Bootloader loaded Agesa0 from SpiRom */
    BL_TRACE_AGESA_EXECUTION_COMPLETE           = 0xC6,     /*!< AgesaBL Execution Completed */
    BL_TRACE_LOAD_SMU_TO_DRAM_START             = 0xC7,     /*!< LoadSMUFWToDram() started */
    BL_TRACE_LOAD_SMU_TO_DRAM_DONE              = 0XC8,     /*!< LoadSMUFWToDram() finished */
    BL_TRACE_LOAD_WLAN_BT_START                 = 0xC9,     /*!< Not used */
    BL_TRACE_LOAD_RUN_VBIOS_ASIC_INIT_START     = 0xCA,     /*!< Load_Run_vbiosAsicInit() started */
    BL_TRACE_LOAD_VBIOS_START                   = 0xCB,     /*!< bios_load2dram() started */
    BL_TRACE_LOAD_MP5_TO_SRAM_START             = 0xCC,     /*!< Bootloader start to load MP5 FW */
    BL_TRACE_LOAD_MP5_TO_SRAM_DONE              = 0xCD,     /*!< Bootloader successfully loaded MP5 FW */
    BL_TRACE_RECOVERY_MODE_RESET                = 0xCE,     /*!< A/B Recovery started */
    BL_TRACE_X86_RELEASE_START                  = 0xCF,     /*!< ReleaseX86AndGoToSteadyState(0 started) */
    BL_TRACE_LOAD_DIAG_FW_START                 = 0xD0,     /*!< Not used */
    BL_TRACE_VALID_CS_SEED_START                = 0xD1,     /*!< CsSeedValidation() started */
    BL_TRACE_ESPI_INIT_START                    = 0xD2,     /*!< eSPI_init() started */
    BL_TRACE_LPC_INIT_START                     = 0xD3,     /*!< Not used */
    BL_TRACE_BIXBY_DETECT_START                 = 0xD4,     /*!< detect_and_validate_bixby() started */
    BL_TRACE_MPIO_FW_LOAD_VALIDATE_START        = 0xD5,     /*!< Load and validate MPIO FW */
    BL_TRACE_APPLY_SECURITY_TO_UNSECURE         = 0xD6,     /*!< Apply RAP to unsecure when bit 4 of Entry 0xB is set */
    BL_TRACE_LOAD_LSDMA                         = 0xD7,     /*!< Load LSDMA firmware */
    BL_SPECIAL_IDLE_LOOP                        = 0xD8,     /*!< Entered idle loop defined for special bootloader versions */
    BL_TRACE_LOAD_L1_POLICY_START               = 0xD9,     /*!< Start of loading L1 security policy */
    BL_TRACE_APPLY_L1_SETTINGS_START            = 0xDA,     /*!< Start of Apply L1 security policy settings */
    BL_TRACE_MPCCX_LOAD_TO_DRAM                 = 0xDB,     /*!< Load MPCCX FW to secure DRAM region */
    BL_TRACE_LOAD_USB_C_PHY_START               = 0xDD,     /*!< Verify and load usb c phy from SPIROM to DRAM */
    BL_TRACE_MPIO_PHY_FW_LOADING                = 0xDE,     /*!< PSP BL is starting to load all MPIO PHY FWs */
//    BL_SUCCESS_S4_RESUME                        = 0xDF,     /*!< PSP BL identified resuming from S4 */
//    BL_TRACE_IP_DISCOVERY_COPY_START            = 0xE0,     /*!< Indicates start of IP Discovery Binary delivery process */
    BL_TRACE_SHADOW_ROM                         = 0xE1,     /*!< Start preparing Shadow ROM */
    BL_TRACE_PSPOS_RELEASE_START                = 0xE2,     /*!< PSP SysDriver and OS verification and loading */
    BL_TRACE_PSPOS_S3_S0I3_RESUME_START         = 0xE3,     /*!< ValidateAndJumpToAMDTEE_S3() started */
    BL_TRACE_GIVE_CONTROL_TO_PSPOS              = 0xE4,     /*!< Control handed over to PSPOS */
    BL_TRACE_MSMU_FW_LOAD_START                 = 0xE5,     /*!< msmu_iram_fw_load() started */
    BL_TRACE_MPM_INIT_START                     = 0xE6,     /*!< mpm_init() started */
    BL_TRACE_MSMU_ZSC_IRAM_SAVE                 = 0xE7,     /*!< ZCS has been called to save IRAM contents */
    BL_TRACE_M_EVENT_DETECT_START               = 0xE8,     /*!< Not used */
    BL_TRACE_LOAD_WRAPPED_IKEK_TA               = 0xE9,     /*!< Loading iKEK TA started */
    BL_TRACE_SPIROM_CONFIG_LOAD_VALIDATE        = 0xEA,     /*!< Load SPI ROM config binary */
    BL_TRACE_LX6_LOAD_TO_DRAM                   = 0xEB,     /*!< Save FW for S0i3/S3 restore */
    BL_TRACE_ZSC_STAGE1_IP_RESTORE_START        = 0xEC,     /*!< Stage1 of MSMU IP restore to signal ZSC of S0i3 exit */
    BL_TRACE_ZSC_STAGE2_IP_RESTORE_START        = 0xED,     /*!< Stage2 of MSMU IP restore to restore segment MSMUs */
    BL_TRACE_APPLY_MEMORY_FENCING               = 0xEE,     /*!< Apply memory fencing */
//    BL_SUCCESS_S5_COLD                          = 0xF0,     /*!< PSP BL identified previous reset S5 - Cold boot */
    BL_RECOVERY_MODE_FLAG                       = 0xF1,     /*!< To trace the recovery mode flag clearance; this has to be cleared before Releasex86 */
    BL_TRACE_IDLE_LOOP                          = 0xF2,     /*!< PSP BL is in idle loop which indicates the failure of recovery verification */
    BL_TRACE_ENABLE_TWIX_ERR_INTR               = 0xF3,     /*!< Setup TWIX error interrupt */
    BL_TRACE_SFFS_CONFIG_LOAD_VALIDATE          = 0xF4,     /*!< Load and validate SFFS Config */
    BL_TRACE_POST_RECOVERY_INFO                 = 0xF9,     /*!< Post recovery related information */
    BL_TRACE_UNIT_TESTS                         = 0xFA,     /*!< smn_unit_tests() started */
    BL_TRACE_PSP_MSG_TO_SMU                     = 0xFC,     /*!< Send SMU DRAM address start */
    BL_TRACE_SYS_CALL_START                     = 0xFD,     /*!< Syscall handler started */
    BL_STATUS_RECOVERED_MODE                    = 0xFE,     /*!< Going into Recovery Mode */
    BL_TRACE_TRIGGER_WARM_RESET                 = 0xFF,     /*!< Trigger warm reset */

    BL_TRACE_MAX                                = INT32_MAX
};


/** @enum BL_MP1_STATUS
 * \brief BootLoader MP1 Status Codes
 *
 * Codes Returned from BootLoader during MP1 Operations
 */
enum bl_mp1_status
{
    BL_SMU_STATUS_RESET                 = 0x00,     /*!< SMU Status Reset */
    BL_SMU_VERIFICATION_START           = 0x01,     /*!< SMU Verification started */
    BL_SMU_POLL_SMU_INTR_READY          = 0x02,     /*!< SMU Interrupt ready */
    BL_SMU_DRAM_VERIFICATION_01_START   = 0x03,     /*!< SMU DRAM Verification 1 start */
    BL_SMU_DRAM_VERIFICATION_02_START   = 0x04,     /*!< SMU DRAM Verification 2 start */

    BL_MP1_STATUS_MAX                   = INT32_MAX
};

/** @enum BL_MP2_STATUS
 * \brief BootLoader MP2 Status Codes
 *
 * Codes Returned from BootLoader during MP2 Operations
 */
enum bl_mp2_status
{ //!<
    BL_MP2_STATUS_RESET                 = 0x00,     /*!< MP2 Status Reset */
    BL_MP2_RESERVED                     = 0x01,     /*!< MP2 Reserved */
    BL_MP2_CLEAR_CONFIG_FILE            = 0x02,     /*!< MP2 config file cleared */
    BL_MP2_LOAD_VERIFY_CONFIG_FILE      = 0x03,     /*!< Clearing MP2 Configuration file */
    BL_MP2_VERIFICATION_START           = 0x04,     /*!< MP2 Firmware Verification started */
    BL_MP2_REMOVE_HEADER_SIGNATURE      = 0x05,     /*!< MP2 Remove header signature */
    BL_MP2_RESET_TO_RUN                 = 0x06,     /*!< MP2 is being rest to start running */
    BL_MP2_FAILED_TO_RECOVERY           = 0x07,     /*!< MP2 failed, going to recovery */
    BL_MP2_DISABLE_BY_BOOTMODE          = 0x08,     /*!< MP2 Disabled by Bootmode */
    BL_MP2_DISABLE_BY_SFI               = 0x09,     /*!< MP2 Disabled by  SFI bit */
    BL_MP2_DISABLE_BY_S0I3_RAM          = 0x0A,     /*!< MP2 Disabled by S0I3 */
    BL_MP2_DISABLE_BY_SOFTFUSE          = 0x0B,     /*!< MP2 Disabled by Softfuse */
    BL_MP2_DISABLE_BY_RECOVERY          = 0x0C,     /*!< MP2 Disabled by recovery */

    BL_MP2_STATUS_MAX                   = INT32_MAX
};

/** @enum BL_MPM_STATUS
 * \brief BootLoader MPM Trace Codes
 *
 * Codes Returned from BootLoader during MPM Operations
 */
enum bl_mpm_status
{
    BL_MPM_AMF_FW1_LOAD_START                 = 0x01,   /*!< amf_fw1_to_sram_load() started */
    BL_MPM_AMF_FW1_LOAD_DONE                  = 0x02,   /*!< amf_fw1_to_sram_load() finished */
    BL_MPM_AMF_FW2_LOAD_START                 = 0x03,   /*!< amf_fw_to_dram_load() started */
    BL_MPM_AMF_FW2_LOAD_DONE                  = 0x04,   /*!< amf_fw_to_dram_load() finished */
    BL_MPM_MFD_LOAD_START                     = 0x05,   /*!< mfd_spirom_load() started */
    BL_MPM_WLAN_LOAD_START                    = 0x06,   /*!< wlan_driver_to_dram_load() started */
    BL_MPM_WLAN_LOAD_DONE                     = 0x07,   /*!< wlan_driver_to_dram_load() finished */
    BL_MPM_MFD_LOAD_DONE                      = 0x08,   /*!< mfd_spirom_load() finished */
    BL_MPM_DISABLE                            = 0x09,   /*!< Disable MPM when version error occurs */
    BL_MPM_WLAN_GET_UNITID_START              = 0x0A,   /*!< Get UnitID of WLAN started */
    BL_MPM_WLAN_GET_DEV_FUNC_NUM              = 0x0B,   /*!< Get device and function number */
    BL_MPM_WLAN_GET_BUS_NUM_CONFIG            = 0x0C,   /*!< Get config space of bus number */
    BL_MPM_WLAN_GET_FUNC_HEADER_TYPE          = 0x0D,   /*!< Get function header type */
    BL_MPM_WLAN_GET_CLASS_CODE                = 0x0E,   /*!< Get class code from PCIe config space */
    BL_MPM_WLAN_UNITID_FOUND                  = 0x0F,   /*!< Indicator that UnitID has been found */
    BL_MPM_WLAN_RESTRICT_DRAM                 = 0x10,   /*!< Restrict WLAN to only access WLAN DRAM region */


    BL_MPM_STATUS_MAX                         = INT32_MAX
};

/** @enum BL_M_EVENT_STATUS
 * \brief BootLoader MPM Event Codes
 *
 * Codes Returned from BootLoader when MPM status changes
 */
enum bl_m_event_status
{
    BL_MPM_TIMER_EXPIRY_WAKEUP_EVENT          = 0x01,   /*!< Not used */
    BL_MPM_AC_POWER_WAKEUP_EVENT              = 0x02,   /*!< Not used */
    BL_MPM_WLAN_WAKEUP_EVENT                  = 0x03,   /*!< Not used */

    BL_MPM_EVENT_MAX                         = INT32_MAX
};

/** @enum BL_AGESA_STATUS
 * \brief BootLoader AGESA Status Codes
 *
 * Codes Returned from BootLoader during AGES Initialization
 */
enum bl_agesa_status
{
    BL_AGESA_STATUS_RESET                      = 0x00,      /*!< Not used */
    BL_AGESA_TABLE_VALIDATION                  = 0x01,      /*!< Loading and Validating AGESA unified table */
    BL_AGESA_VALIDATION_FAILED                 = 0x02,      /*!< AGESA validation failed */
    BL_AGESA_TABLE_ADD_ENTRY_TO_MP2RAM_FAILED  = 0x03,      /*!< Failed to add AGESA unified table to MP2 RAM */
    BL_AGESA_ADD_ENTRY_TO_MP2RAM_FAILED        = 0x04,      /*!< Failed to add AGESA to MP2 RAM */
    BL_AGESA_READ_ENTRY_FROM_MP2RAM_S3         = 0x05,      /*!< Reading AGESA entry from MP2RAM during S3 */
    BL_AGESA_TABLE_READ_FROM_MP2RAM_FAILED     = 0x06,      /*!< Failed to read AGESA unified table from MP2 RAM */
    BL_AGESA_READ_FROM_MP2RAM_FAILED           = 0x07,      /*!< Failed to read AGESA from MP2 RAM */

    BL_AGESA_STATUS_MAX                        = INT32_MAX
};

/** @enum BL_BIXBY_STATUS
 * \brief BootLoader Bixby Status Codes
 *
 * Codes Returned from BootLoader during Bixby operations
 */
enum bl_bixby_status
{
    BL_BIXBY_PCIE_LINK_TRAINED      = 0x00,     /*!< PCIe link not trained by Bixby MP0 Boot Rom */
    BL_BIXBY_DETECTED               = 0x01,     /*!< Bixby chipset detected on board */
    BL_BIXBY_BOOTROM_READY          = 0x02,     /*!< Bixby Boot Rom ready */
    BL_BIXBY_COPIED_PSPBL           = 0x03,     /*!< Bixby PSPBL image copied successfully by SoC MP0 to Bixby MP1 SRAM */
    BL_BIXBY_INTERRUPT_BIXBY        = 0x04,     /*!< Send notificaiton to Bixby MP0 */
    BL_BIXBY_PSPBL_READY            = 0x05,     /*!< Received status from Bixby MP0 PSPBL about its ready state */
    BL_BIXBY_AUTHENTICATION         = 0x06,     /*!< Bixby chipset authenticated */
    BL_BIXBY_COPIED_SECPOLICY       = 0x07,     /*!< Bixby Security Policy validated and copied to Bixby MP1 SRAM */
    BL_BIXBY_APPLIED_SECPOLICY      = 0x08,     /*!< Received status from Bixby about Security Policy applied */
    BL_BIXBY_COPIED_SDU_BIN         = 0x09,     /*!< Bixby SDU binary validated and copied to Bixby MP1 SRAM */
    BL_BIXBY_LOADED_SDU_BIN         = 0x0A,     /*!< Received status from Bixby MP0 about its ready state after loading SDU */
    BL_BIXBY_COPIED_SMU_FW          = 0x0B,     /*!< Bixby SMU firmware validated and copied to Bixby Mp1 SRAM */
    BL_BIXBY_LOADED_SMU_FW          = 0x0C,     /*!< Received status from Bixby that MP1 is out of reset and functional */

    BL_BIXBY_STATUS_MAX             = INT32_MAX
};

/** @enum BL_WLAN_STATUS
 * \brief BootLoader wlan Status Codes
 *
 * Codes Returned from BootLoader during wlan operations
 */
enum bl_wlan_status
{
    BL_WLAN_STATUS_RESET                       = 0x00,      /*!< Not used */
    BL_WLAN_DISABLE_PHY                        = 0x01,      /*!< WLAN Physical interface Disabled */
    BL_WLAN_POLL_WAKE_UP                       = 0x02,      /*!< WLAN Polled to wake up */
    BL_WLAN_DETECTION_FAILED                   = 0x03,      /*!< WLAN ws not detected */
    BL_WLAN_POLL_FOR_POWER_ENABLE              = 0x04,      /*!< Poll for WLAN Power up enable */
    BL_WLAN_POLL_FOR_EVENT_DONE                = 0x05,      /*!< Poll event detected */
    BL_WLAN_POLL_FOR_EVENT_FAILED_WITH_TIMEOUT = 0x06,      /*!< Poll event timed out */
    BL_WLAN_BT_TABLE_VERIFICATION_START        = 0x07,      /*!< WLAN and BT Table validation started*/
    BL_WLAN_REGISTER01_VERIFICATION_START      = 0x08,      /*!< WLAN Register Verification started */
    /* 0x09 is skipped? */
    BL_WLAN_LOAD_CORE_MCP_START                = 0x0A,      /*!< Verifying and loading core MCP started */
    /* 0xB ~ 0x10 are skipped? */
    BL_WLAN_BT_VERIFICATION_START              = 0x11,      /*!< WLAN and BT firmware verification started*/
    /* 0x2 ~ 0x16 are skipped? */
    BL_WLAN_POST_SET_START                     = 0x17,      /*!< WLAN post set start */
    BL_WLAN_INIT                               = 0x18,      /*!< WLAN Initializing */
    BL_WLAN_POST_DISABLE                       = 0x19,      /*!< Disabling WLAN Physical Layer */
    BL_WLAN_POLL_SRAM_INIT_START               = 0x1A,      /*!< WLAN SRAM Initialization started */
    BL_WLAN_RESET                              = 0x1B,      /*!< WLAN Rest */
    BL_WLAN_BT_MAX_LOG                         = 0x1C,      /*!< Not used */

    BL_WLAN_STATUS_MAX                         = INT32_MAX  /*!< Maximum Status value */
};

/** @enum BL_BIOS_STATUS
 * \brief BootLoader BIOS Status Codes
 *
 * Codes Returned from BootLoader during BIOS operations
 */
enum bl_bios_status
{
    BL_BIOS_STATUS_RESET               = 0x00,      /*!< Not used */
    BL_BIOS_COLD_START_MODE            = 0x01,      /*!< Cold starting BIOS */
    BL_BIOS_LOAD_OEM_PUB_KEY           = 0x02,      /*!< Loading OEM Public Key */
    BL_BIOS_VERIFY_OEM_PUB_KEY         = 0x03,      /*!< Verifying OEM Public Key */
    BL_BIOS_SELF_VERIFY_OEM_PUB_KEY    = 0x04,      /*!< Not used */
    BL_BIOS_LOAD_RTM_SIGNATURE         = 0x05,      /*!< Loading RTM Signature */
    BL_BIOS_SEARCH_RESET_IMAGE         = 0x06,      /*!< Search for BIOS reset image */
    BL_BIOS_LOAD_RESET_IMAGE           = 0x07,      /*!< Loading BIOS reset image */
    BL_BIOS_COPY_VALID_BIOS_ENTRIES    = 0x08,      /*!< Copy valid BIOS entries */
    BL_BIOS_VERIFY_SIGNATURE           = 0x09,      /*!< Verify BIOS Signature */
    BL_BIOS_READ_ENTRY_FROM_MP2RAM_S3  = 0x0A,      /*!< Read BIOS entires from MP2SRAM Durin S3/S0i3 boot */
    BL_BIOS_COMPARE_HASH_S3            = 0x0B,      /*!< Compare BIOS Hash during S3/S0i3 boot */
    BL_BIOS_ADD_ENTRY_TO_MP2RAM_FAILED = 0x0C,      /*!< Adding BIOS entry to MP2RAM failed */
    BL_BIOS_S3_S0i3_RESUME_FAILED      = 0x0D,      /*!< BIOS resume from S3/S0i3 failed */
    BL_BIOS_VALIDATE_AND_LOAD_FAILED   = 0x0E,      /*!< BIOS validate and load failed */

    BL_BIOS_STATUS_MAX                 = INT32_MAX  /*!< Maimum BIOS status code value */
};

/** @enum BL_X86_STATUS
 * \brief BootLoader x86 Status Codes
 *
 * Codes Returned from BootLoader during x86 operations
 */
enum bl_x86_status
{
    BL_X86_STATUS_RESET          = 0x00,        /*!< Not used */
    BL_X86_RELEASE_CORE          = 0x01,        /*!< x86 Cores released */
    BL_X86_DIAG_FW_UNSUPPORT     = 0x02,        /*!< x86 Diagnostic Firmware Unsupported */
    BL_X86_LOAD_DIAG_FW          = 0x03,        /*!< x86 Diagnostic Firmware Loading */
    BL_X86_JUMP_TO_DIAG_FW       = 0x04,        /*!< Jumping to x86 Diagnostic firmware */
    BL_X86_LOAD_IP_DISCOVERY     = 0x05,        /*!< Loading x86 IP discovery table */
    BL_X86_LOAD_SYS_DRV          = 0x06,        /*!< Loading and validating sys_drv binary */
    BL_X86_LOAD_PSP_OS           = 0x07,        /*!< Loading and validating secure OS binary */
    BL_X86_LOAD_KEY_DATABASE     = 0x08,        /*!< Loading and validating key database */
    BL_X86_COPY_AGESA_DRIVER     = 0x09,        /*!< Copying AGESA Driver from SPI-ROM to secre DRAM */
    BL_X86_COPY_NVRAM            = 0x0A,        /*!< Load AMD-TEE write-once presistent data from SPI-ROM to memory buffer */
    BL_X86_COPY_PSP_STORAGE      = 0x0B,        /*!< Copy non-volitile PSP Storage data from SPI-ROM to DRAM memory buffer */
    BL_X86_COPY_OEM_TRUSTLET     = 0x0C,        /*!< Copy trusted OEM dirver from SPI-ROM to private DRAM */
    BL_X86_COPY_FTPM             = 0x0D,        /*!< Copy fTPM driver from SPI-ROM to private DRAM */
    BL_X86_CSTATE_DISABLE        = 0x0E,        /*!< CSTATE disable */
    BL_X86_CSTATE_RESTORE        = 0x0F,        /*!< CSTATE restore */

    BL_X86_COPY_STATUS_MAX       = INT32_MAX    /*!< Maximum value for bl_x86_status */
};

/** @enum BL_PSPOS_STATUS
 * \brief BootLoader PSPOS Status Codes
 *
 * Codes Returned from BootLoader during PSPOS operations
 */
enum bl_pspos_status
{
    BL_PSPOS_STATUS_RESET               = 0x00,     /*!< Not used */
    BL_PSPOS_S3_S0I3_HMAC_VERIFICATION  = 0x09,     /*!< Calculate HMAC of SRAM content saved in DRAM by PSP SecureOS during S3 suspend  */

    BL_PSPOS_STATUS_MAX                 = INT32_MAX /*!< Maximum value for bl_pspos_status */
};

/** @enum BL_SECPOL_STATE
 * \brief BootLoader Security Policy state Codes
 *
 * Codes Returned from BootLoader during Security Policy operations
 */
enum bl_secpol_state
{
    BL_SECURITYPOLICY_NONE            = 0x00,       /*!< Not used */
    BL_SECURITYPOLICY_RSMU_FINE_GRAIN = 0x01,       /*!< Applying RSMU Security Policy */
    BL_SECURITYPOLICY_PSP             = 0x02,       /*!< Applying PSP Security Policy */
    BL_SECURITYPOLICY_GC_VDDGFX       = 0x03,       /*!< Apply gfx Security Policy */
    BL_SECURITYPOLICY_GC_INTERNAL     = 0x04,       /*!< Apply Internal Security Policy  */
    BL_SECURITYPOLICY_GC_GRBM_CAM     = 0x05,       /*!< Apply GRBM CAM Securiyt Policy */
    BL_SECURITYPOLICY_VDDGFX_UNLOCK   = 0x06,       /*!< Apply VDDGFX Unlock Security Policy */
    BL_SECURITYPOLICY_GFX_DLDO        = 0x07,       /*!< Apply gfx DLDO Security Policy */
    BL_SECURITYPOLICY_UNLOCK          = 0x08,       /*!< Apply Unlock Security Policy */
    BL_SECURITYPOLICY_PSP_UNLOCK      = 0x09,       /*!< Apply PSP Unlock Security Policy */
    BL_SECURITYPOLICY_DMU             = 0x0A,       /*!< Apply DMU Security Policy */

    BL_SECURITYPOLICY_STATE_MAX       = INT32_MAX   /*!<  */
};

/** @enum BL_DMCU_STATUS
 * \brief BootLoader Display Micro Controller Unit Status Codes
 *
 * Codes Returned from BootLoader during DMCU operations
 */
enum bl_dmcu_status
{
    BL_ERAM_COPY       = 0x0,       /*!<  */
    BL_ISR_COPY        = 0x1,       /*!<  */

    BL_DMCU_STATUS_MAX = INT32_MAX  /*!< Maximum bl_dmcu_satus code value */
};

/** @enum BL_MSMU_STATUS
 * \brief BootLoader Display Mid-System Management Unit Status Codes
 *
 * Codes Returned from BootLoader during MSMU operations
 */
enum bl_msmu_status
{
    BL_MSMU_FW_DESCRIPTOR_READ = 0x0,       /*!< Reading MSMU firmware descriptior */
    BL_MSMU_FW_LOAD_START      = 0x1,       /*!< Loading MSMU firmware */
    BL_MSMU_FW_LOAD_FINISH     = 0x2,       /*!< Finished loading MSMU firmware */

    BL_MSMU_STATUS_MAX         = INT32_MAX  /*!< Maximum bl_msmu_status code value */
};

/** @enum BL_WARMRESET_STATUS
 * \brief BootLoader warm reset Status Codes
 *
 * Codes Returned from BootLoader during warm reset operations
 */
enum bl_warmreset_status
{
    BL_WRMRST_WAIT_RESET_DEASSERT  = 0x0,       /*!< Wait for reset deassert */
    BL_WRMRST_WAIT_RESET_ASSERT    = 0x1,       /*!< Wait for reset interrupt signal */
    BL_WRMRST_WAIT_RESET_PROPAGATE = 0x2,       /*!< Wait for warm reset */
    BL_WRMRST_WAIT_SMU             = 0x3,       /*!< Wait for SMU to handle warm reset */

    BL_WRMRST_MAX                  = INT32_MAX  /*!< Maximum bl_warmreset_status code value */
};

/** @enum BL_STAGE1_TRACE
 * \brief BootLoader Stage1 trace Codes
 *
 * Codes Returned from BootLoader during Interactions with Stage1 BL
 */
enum bl_stage1_trace
{
    BL_STG1_BL_LOAD_SUCCESS            = 0x01,      /*!< Stage 1 BootLoader started executing */
    BL_STG1_PSPL2_DIR_READ_FROM_FLASH  = 0x02,      /*!< Reading PSP L2 Directory from SPI-ROM */
    BL_STG1_M_SPL_INIT                 = 0x03,      /*!< Stage 1 BootLoader M_SPL Initializaion  */
    BL_STG1_M_SPL_TABLE_READ_FROM_SPI  = 0x04,      /*!< Read the main SPL table to obtain M_SPL */
    BL_STG1_M_SPL_TABLE_AUTH           = 0x05,      /*!< Authenticating SPL table */
    BL_STG1_KDB_READ_FROM_FLASH        = 0x06,      /*!< Read Key database from SPI-ROM */
    BL_STG1_KDB_AUTH                   = 0x07,      /*!< Authenticating Key Database binary */
    BL_STG1_STG2_BL_READ_FROM_FLASH    = 0x08,      /*!< Read and load Stage 2 BootLoader from SPI-ROM */
    BL_STG1_PSP_STG2_BL_AUTH           = 0x09,      /*!< Authenticating Stag 2 PSP BootLoader binary */
    BL_STG1_SWITCHING_PARTITION        = 0x0A,      /*!< Switching partition */
    BL_STG1_TRIGGER_WARM_RESET         = 0x0B,      /*!< Not used */
    BL_STG1_M_SPL_TABLE_SAVE           = 0x0C,      /*!< Saving M_SPL table */
    BL_STG1_M_SPL_TABLE_RESTORE        = 0x0D,      /*!< Restoring M_SPL table */
    BL_STG1_KDB_SAVE                   = 0x0E,      /*!< Saving Key Database */
    BL_STG1_KDB_RESTORE                = 0x0F,      /*!< Restorign Key Database */
    BL_STG1_STG2_BL_SAVE               = 0x10,      /*!< Saveing Stage 2 BootLoader image */
    BL_STG1_STG2_BL_RESTORE            = 0x11,      /*!< Restoring Stage 2 BootLoader image */
    BL_STG1_S5_SRAM_INIT               = 0x12,      /*!< Stage1 S5 SRAM Initializtion */
    BL_STG1_READ_CCD_PRESENT_FUSE      = 0x13,      /*!< Stage1 Read CCD Present fuse */
    BL_STG1_BL_READ_VERSTAGE           = 0x14,      /*!< Stage1 Read verstage binary */
    BL_STG1_BL_READ_VERSTAGE_KEY       = 0x15,      /*!< Stage1 Read verstage key */
    BL_STG1_BL_ENTER_DEVELOPER_MODE    = 0x16,      /*!< Stage1 Enter Developer mode */
    BL_STG1_PSP_BIOS_DIRECTORY_UPDATE  = 0x17,      /*!< Stage1 PSP Directory update from verstage */
    BL_STG1_BL_ISH_CHK                 = 0x18,      /*!< Stage1 executing ISH boot priority check */
    BL_STG1_BL_FIPS_SELF_TEST_DONE     = 0x19,      /*!< Stage1 FIPS self test success */
    BL_STG1_BL_NON_FIPS_ALGO_START1    = 0x1A,      /*!< Stage1 FIPS unapproved algorithm 1 started */
    BL_STG1_BL_NON_FIPS_ALGO_EXIT1     = 0x1B,      /*!< Stage1 FIPS unapproved algorithm 1 exited */
    BL_STG1_BL_FIPS_SERVICE_ENTER      = 0x1C,      /*!< Stage1 FIPS authentication service started */
    BL_STG1_BL_FIPS_SERVICE_EXIT       = 0x1D,      /*!< Stage1 FIPS authentication service exited */
    BL_STG1_BL_NON_FIPS_ALGO_START2    = 0x1E,      /*!< Stage1 FIPS unapproved algorithm 2 started */
    BL_STG1_BL_NON_FIPS_ALGO_EXIT2     = 0x1F,      /*!< Stage1 FIPS unapproved algorithm 2 exited */
    BL_STG1_BL_NON_FIPS_ALGO_START3    = 0x20,      /*!< Stage1 FIPS unapproved algorithm 3 started */
    BL_STG1_BL_NON_FIPS_ALGO_EXIT3     = 0x21,      /*!< Stage1 FIPS unapproved algorithm 3 exited */
    BL_STG1_JUMP_TO_STAGE2_BL          = 0xAA,      /*!< Jumping to Stage 2 BootLoader */

    BL_STG1_TRACE_MAX                  = INT32_MAX  /*!< Maximum bl_stage1_trace code value */
};

/** @enum BL_STAGE1_ERROR
 * \brief BootLoader Stage1 Error Codes
 *
 * Stage1 BootLoader Error Codes
 */
enum bl_stage1_error
{
    BL_STG1_ERR_SPIROM_READ_FAIL           = 0x03,      /*!< Failed to read from SPI-ROM */
    BL_STG1_ERR_ISH_CORRUPT_FAIL           = 0x04,      /*!< Detect possibly PspID corruption */
    BL_STG1_ERR_FIPS_AUTH_SIG_FAIL         = 0xFA,      /*!< Binary verification failure in fips_auth() */
    BL_STG1_ERR_FIPS_SHA_KAT_FAIL          = 0xFB,      /*!< FIPS module SHA known answer test failed */
    BL_STG1_ERR_FIPS_RSA_KAT_FAIL          = 0xFC,      /*!< FIPS module RSA signature known answer test failed */
    BL_STG1_ERR_FIPS_INTEGRITY_FAIL        = 0xFD,      /*!< FIPS module code integrity hash incorrect */
    BL_STG1_ERR_UNIDENTIFIED               = 0xFE,      /*!< Unidentified error */
    BL_STG1_ERR_UNKNOWN                    = 0xFF,      /*!< Unknown error */

    BL_STG1_ERR_MAX                        = INT32_MAX  /*!< Maximum bl_stage1_error code value */
};

/** @enum BL_STAGE2_RECOVER_STATUS
 * \brief BootLoader Stage2 reovery status Codes
 *
 * Codes encountered during Stage2 recovery operations
 */
enum bl_stage2_recovery_status
{
    BL_STG2_RECOVERY_LOGGING                = 0x00,     /*!< Logging recovery */
    BL_STG2_RECOVERY_UNBOOTABLE_PARTITION   = 0x01,     /*!< Recovery partition is unbootable */

    BL_STG2_RECOVERY_MAX                    = INT32_MAX /*!< Maximum bl_stage2_recovery_status code value */
};

/** @enum RECOVERY_INFO_TO_POST
 * \brief Status of recovery log posting
 *
 * Recovery log posting status
 */
enum recovery_info_post
{
    END_OF_RECOVERY_LOG_POST                = 0xF0      /*!< End of recovery log info  */
};

/** @enum BL_UNITTESTS_STATUS
 * \brief BootLoader Unit Test Status Codes
 *
 * Codes Returned from BootLoader embedded unit tests
 */
enum bl_unittests_status
{
    BL_UNITTESTS_SMNIF_DIRECT       = 0x00,     /*!< Starting SMNIF Direct unit test */
    BL_UNITTESTS_SMNIF_MAPPING      = 0x01,     /*!< Starting SMNIF mapped unit test */
    BL_UNITTESTS_DRAM_DATA_BUS      = 0x02,
    BL_UNITTESTS_DRAM_RD_WR         = 0x03,
    BL_UNITTESTS_DRAM_FULL_RANGE    = 0x04,
    BL_UNITTESTS_DRAM_OVERLAP       = 0x05,

    BL_UNITTESTS_SMNIF_MAX        = INT32_MAX   /*!< Maximum bl_unittests_status code value */
};

/** @enum BL_HSP_STATUS
 * \brief BootLoader Hardware Security Processor Status Codes
 *
 * Codes Returned from BootLoader during HSP operations
 */
enum bl_hsp_status
{
    BL_HSP_BUILD_SEND_PCR_MEASUREMENT      = 0x20,
    BL_HSP_BUILD_SEND_GENERIC              = 0x21,
    BL_HSP_BUILD_SEND_DBG_UNLOCK           = 0x22,
    BL_HSP_CMD_DISPATCHER                  = 0x23,

    BL_HSP_BUILD_SEND_MAX                  = INT32_MAX  /*!< Maximum bl_hsp_status code value */
};

/** @enum BL_UBU_TRACE
 * \brief BootLoader USB BIOS Updater Codes
 *
 * Codes Returned from BootLoader during UBU operations
 */
enum bl_ubu_trace {
    BL_UBU_BOOTLOADER_ENTRY                = 0x01,      /*!< UBU Bootloader entry point reached */
    BL_UBU_MAIN_START                      = 0x02,      /*!< UBU Bootloader main function executing */
    BL_UBU_COLD_RESET                      = 0x03,      /*!< UBU Bootloader triggering cold reset */
    BL_UBU_INIT_SPI                        = 0x04,      /*!< UBU SPI Initialized */
    BL_UBU_PAL_MEMPOOL                     = 0x05,      /*!< UBU PAL Mempool */
    BL_UBU_FSDL_USB_INIT                   = 0x20,      /*!< UBU FSDL USB Initialization */
    BL_UBU_USB_UNINIT                      = 0x22,      /*!< UBU USB UnInitialized */
    BL_UBU_USB_INIT                        = 0x26,      /*!< UBU USB Initializaion */
    BL_UBU_MSC_INIT                        = 0x2A,      /*!< UBU MSC Initialization */
    BL_UBU_FF_FILE_MOUNT                   = 0x2B,      /*!< UBU FF File mount */
    BL_UBU_FF_FILE_OPEN                    = 0x2C,      /*!< UBU FF File opene */
    BL_UBU_FF_FILE_READ                    = 0x2D,      /*!< UBU FF File read */
    BL_UBU_FF_FILE_CLOSE                   = 0x2E,      /*!< UBU FF File close */

    /* UBU Bootloader Verify BIOS Compatibility (VBC) */
    BL_UBU_VBC_GET_SPIROM_EFS_START        = 0x40,     /*!< Entry point reached */

    BL_UBU_RD_USB_FIRMWARE_RETRY           = 0x6F,      /*!< UBU Read usb firmware retry */
    BL_UBU_SPIWRITE_CONST_VAL_TEST         = 0xA0,      /*!< Starting constant value write test */
    BL_UBU_SPIWRITE_COUNTER_TEST           = 0xA1,      /*!< Starting incremental counter write test */
    BL_UBU_SPIWRITE_TESTS_PASS             = 0xA2,      /*!< Write tests have passed */
    BL_UBU_CRC_CHECKSUM_FAIL               = 0xA3,      /*!< UBU CRC Checksum fail */
    BL_UBU_SET_HDT_CLAIMANT                = 0xB1,      /*!< UBU Set HDR claimant */
    BL_UBU_TRACE_IDLE_LOOP                 = 0xF2,      /*!< UBU BL is in idle loop */
};

/** @enum BL_UBU_STATUS
 * \brief BootLoader USB BIOS Updater Codes
 *
 * Codes Returned from BootLoader during UBU operations
 */
enum bl_ubu_status {
    BL_UBU_VBC_SPIROM_EFS_START_NOT_FOUND      = 0x01,     /*!< EFS cookie not found */
    BL_UBU_VBC_SPIROM_EFS_START_FOUND          = 0x02,     /*!< EFS cookie found - next byte contains \
                                                                first byte of EFS address */
    BL_UBU_VBC_SPIROM_VENDOR_ID                = 0x03,     /*!< EFS Vendor ID */
    BL_UBU_VBC_FORCE_FILE_FOUND                = 0x04,     /*!< FORCE.TXT file found on flash drive */
    BL_UBU_VBC_FORCE_FILE_NOT_FOUND            = 0x05,     /*!< FORCE.TXT file NOT found on flash drive */
    BL_UBU_VBC_SPIROM_VENDOR_BOARD_ID          = 0x06,     /*!< */
    BL_UBU_VBC_BIOS_VENDOR_ID                  = 0x07,     /*!< */
    BL_UBU_VBC_BIOS_VENDOR_BOARD_ID            = 0x08,     /*!< */
    BL_UBU_VBC_VENDORID_VENDORBOARDID_MATCH    = 0x09,     /*!< Vendor ID / Vendor Board ID Match */
    BL_UBU_VBC_NO_VID_VBID_MATCH_SO_HALT       = 0x0A,     /*!< Vendor ID / Vendor Board ID no match */
};

/** @enum BL_LX6_TRACE
 * \brief BootLoader LX6 FW Updater Codes
 *
 * Codes Returned from BootLoader during LX6 operations
 */
enum bl_lx6_status {
    BL_LX6_ROM_READY                   = 0x01,
    BL_LX6_ASSERT_RESET                = 0x02,
    BL_LX6_SRAM_LOAD                   = 0x03,
    BL_LX6_RESET_VECTOR_SEL            = 0x04,
    BL_LX6_DEASSERT_RESET              = 0x05,
    BL_LX6_FW_READY                    = 0x06,
};

/** @enum BL_RSMU_STATUS
 * \brief BootLoader RSMU IP Configuration Updater Codes
 *
 * Codes Returned from BootLoader during RSMU operations
 */
enum bl_rsmu_status {
    BL_RSMU_IOD                        = 0x01,
    BL_RSMU_CCD                        = 0x02,
};

/** @enum BL_MCA_STATUS
 * \brief BootLoader MCA Configure Status Codes
 *
 * Codes Returned from BootLoader during MCA Operations
 */
enum bl_mca_status
{
    BL_MCA_STATUS_RESET                   = 0x00,     /*!< MCA Status Reset */
    BL_MCA_INTRCFG_FAILED                 = 0x01,     /*!< MCA Interrupt configuration failed */
    BL_MCA_RAS_INTR_ADDRESS_CONFIG_FAILED = 0x02,     /*!< MCA RAS Interrupt address configuration failed */
};

#endif
