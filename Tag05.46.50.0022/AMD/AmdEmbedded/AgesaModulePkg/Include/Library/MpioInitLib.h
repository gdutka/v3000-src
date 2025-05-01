/**
 *  @file MpioInitLib.h
 *  @brief Structures and defines for MPIO firmware initailization
 *//*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************

*
*/

#ifndef _MpioInitLib_H_
#define _MpioInitLib_H_

#define DXIO_MBOX_RETURN_NONPOSTED              0
#define DXIO_MBOX_RETURN_POSTED                 1
#define DXIO_MBOX_RETURN_POSTED_DEFRD           3

#define DXIO_MBOX_RETVAL_NULL                   0
#define DXIO_MBOX_RETVAL_TIMEOUT                1
#define DXIO_MBOX_RETVAL_ERROR                  2
#define DXIO_MBOX_RETVAL_OK                     3
#define DXIO_MBOX_RETVAL_UNKNOWN_LOCK           4
#define DXIO_MBOX_RETVAL_TRYAGAIN               5
#define DXIO_MBOX_RETVAL_MBOX_IDLE              6
#define DXIO_MBOX_RETVAL_MBOX_BUSY              7
#define DXIO_MBOX_RETVAL_MBOX_DONE              8

#define BIOSSMC_Result_OK                    0x1
#define BIOSSMC_Result_Failed                0xFF

#define UBM_INVALID_SWITCH_ADDR              0x0
#define UBM_GPIO_NOT_USED                    0xFF

#define POSTED_MSG(message) message + (DXIO_MBOX_RETURN_POSTED_DEFRD << 8)

/*! The new device connected is an SATA/SAS device (bios_dfc_info_t.state.type). */
#define BIOS_DFC_INFO_TYPE_SATA_SAS         (0x4)
/*! The new device connected is an PCIe device (bios_dfc_info_t.state.type). */
#define BIOS_DFC_INFO_TYPE_QUAD_PCI         (0x5)
/*! The old device is no longer present (bios_dfc_info_t.state.type). */
#define BIOS_DFC_INFO_TYPE_EMPTY            (0x7)

#pragma pack (push, 1)

/**
 *  @brief MPIO Message List
 *
 *  This enumerator defins the messages supported by MPIO FW3
 */
typedef enum {
  MPIO_GET_FIRMWARE_VERSION  = 0x00,   ///< 0x00 - Get firmware version and status information
  MPIO_GET_STATUS,                     ///< 0x01 - Get the current status of MPIO
  MPIO_SET_GLOBAL_CONFIG,              ///< 0x02 - Set BIOS global configuration structure values.
  MPIO_GET_ASK_RESULT,                 ///< 0x03 - Update status for links defined in shared memory
  MPIO_SETUP_LINK,                     ///< 0x04 - MPIO will process links defined in the shared memory
  MPIO_RECONCILE_LINK,                 ///< 0x05 - Processs links in shared memory, reconcile mapping to capabilities
  MPIO_RECOVER_ASK,                    ///< 0x06 - Recover the BMC link ASK that was requested by ABL
  MPIO_TRANSFER_ASK,                   ///< 0x07 - DMA an array of ASKs using the form MPIO_ASK_STRUCT
  MPIO_TRANSFER_EXTENDED_ATTRIBUTES,   ///< 0x08 - DMA a table of extended attributes formerly ancillary data

  MPIO_MSG_PCIE_SPEED_CHANGE,          ///< 0x09 - PCIE Speed change
  MPIO_MSG_PCIE_INITIALIZE_ESM,        ///< 0x0A - ESM Support
  MPIO_MSG_PCIE_RESET_CONTROLLER,      ///< 0x0B - Reset PCIE Controller

  /*! This interface allows the MPIO to proxy the accesses to the PCIe straps for the BIOS.
  // strap number - 0xfffe0000 | index, pcie core upper 16, in aperture order
  // 32 data
  // each controller has index/data port0-8

  @arguments
  - Argument 0 - pcie core [31:16] strap_idx [15:0]
  - Argument 1 - The value to write to the strap
  @response
  - Response Register 0 = @ref SMC_Result_OK on success and @ref SMC_Result_Failed on error.
  - Error Register = @ref MSG_ACCEPTED */
  MPIO_MSG_PCIE_WRITE_STRAP                  = 0x0C,

  /*! DMA the table of CXL devices using the form of cxl_device_info_t
  @arguments
   - rguments	Description
   - Argument 0 - DMA source address (high)
   - Argument 1 - DMA source address (low)
   - Argument 2 - Number of Devices
  @response
  - Response Register 0 = @ref SMC_Result_OK on success and @ref SMC_Result_Failed on error.
  - Error Register = @ref MSG_ACCEPTED */

  MPIO_MSG_CXL_INITIALIZE                   = 0x0D,
  /*! Provides the MPIO with the I2C topography information necessary to enumerate the UBM, U.3, and OCP controllers. This interface uses
  the structure defined in @ref ubm_i2c_info_t. The information is provided via a pointer to an array of these entries and
  is saved by the MPIO before responding to this call. This call also starts the MPIO process of enumerating the devices which will complete
  before this command is acknowledged. This can take some time allowing the BIOS to continue on and check back with the MPIO for completion.
  @note The I2C topography provided must contain at least one I2C switch and expander in order to be considered valid and the MPIO responds
  with an error if this is not the case.
  @arguments
  - Argument 0 - The MPIO accessible location of the I2C data stucture array containing the I2C topography for the system.
  - Argument 1 - The number of entries in the I2C data array provided in the first argument.
  @response
  - Response Register 0 = @ref SMC_Result_OK on success and @ref SMC_Result_Failed on error.
  - Error Register = @ref MSG_ACCEPTED */

  BIOS_MPIO_MSG_I2C_ENUMERATE              = 0x10,
  /*! Request from the BIOS to return the nth DFC connection information for a UBM connected device connector. This request provides
  the connection information for the I2C connector index and device connector index provided. If the either is out of range this command
  returns and error.
  @arguments
  - Argument 0 - The I2C index from the original array provided by the BIOS_MPIO_MSG_I2C_ENUMERATE interface.
  - Argument 1 - The zero based device connector index for this I2C interface.
  @response
  - Response Register 0 = Returns the total number of device connectors associated with the I2C connection from this request.
  - Response Register 1 = @ref bios_dfc_info_t word 0.
  - Response Register 2 = @ref bios_dfc_info_t word 1.
  - Response Register 3 = @ref bios_dfc_info_t word 2.
  - Error Register = @ref MSG_ACCEPTED */
  BIOS_MPIO_MSG_I2C_DEVICE_GET             = 0x11,
  /*! The MPIO signals an event via interrupt to the BIOS indicating that it has detected a change has been detected to a port
  that was added via the BIOS_MPIO_MSG_I2C_ENUMERATE command. This command allows the MPIO to return each change event to the
  BIOS.
  @arguments
  There are no arguments to this commands.
  @response
  The response provided is based on the type of node that triggered the event. The BIOS is expected to know this based on the
  HFC index that is returned via this command.
  - UBM
      - Response Register 0 is the HFC index for the device that is notifying an event has occurred.
      - Response Register 1 Word 0 of @ref bios_dfc_info_t
      - Response Register 2 Word 1 of @ref bios_dfc_info_t
      - Response Register 3 Word 2 of @ref bios_dfc_info_t
  - OCP
      - Response Register 0 is the HFC index for the device that is notifying an event has occurred.
      - Response Register 1 is @ref bios_ocp_info_t
  - U.3/U.2:
      - Response Register 0 is the HFC index for the device that is notifying an event has occurred.
      - Response Register 1 is @ref bios_u3_info_t
  */
  BIOS_MPIO_MSG_I2C_DEVICE_CHANGE_GET      = 0x12,
  /*! This command is used to send the legacy hotplug I2C configuration information to the MPIO.
  @note this was originally a two step process with a call to  fSMC_MSG_SetBiosDramAddr which set this address. It
  was 64-bit address as well which we may still need? Seems like this is part of our shared memory and should be like
  the interface for UBM I2C.
  @arguments
  - Argument 0 - The Address to DMA this data from and into to the MPIO local storage.
  - Argument 1 - TODO can this be number of entries in the table??
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET   = 0x13,
  /*! This is the initial call to enable the legacy hotplug interface for the MPIO. The
  BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET command must have been sent before this or this call fails.
  @arguments
  - Argument Register 0 - The values in this argument are encoded in the @ref bios_msg_enable_hotplug_t.args structure.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_HOTPLUG_ENABLE             = 0x14,
  /*! This is the call is used to disable the legacy hotplug interface for the MPIO. This will shutdown all polling
  and notifications related to any legacy hotplug events.
  @arguments
  None.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK.
  */
  BIOS_MPIO_MSG_HOTPLUG_DISABLE            = 0x15,
  /*! This is the call is used to set a given bus switch's address with a single argument passed in the
  first argument.
  @arguments
  - Argument Register 0 - The format of this register is in @ref bios_msg_i2c_switch_addr_t which holds the
  new I2C address in the least significant byte and the bus index in the second byte.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the new address was stored.
  */
  BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET        = 0x16,
  /*! This is the call sets the Express Module blinking period for LEDs.
  @arguments
  - Argument Register 0 - The blink period in ms?
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_BLINK_INTERVAL_SET         = 0x17,
  /*! This is the call sets the polling interval of I2C connected devices. This can be used to slow the polling of
  I2C devices for hotplug related events. This value is ignored if it is less than the total number of I2C devices
  present in the system. The interval given here is the gap between polling the full list of I2C devices.
  @arguments
  - Argument Register 0 - The polling interval between accesses to the I2C devices.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_POLL_INTERVAL_SET  = 0x18,
  /*! This is the call sets some of the debugging features in the hotplug interface. This call is set only and once these are set
  they cannot be cleared.
  @arguments
  - Argument Register 0 - The debug flags to set for the hotplug interface.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_FLAGS_SET          = 0x19,
  /*!
  This command is used to repspond to interrupts by setting the proper values in the argument register formated using
  the GpioIntrCommand_t structure.
  @arguments
  - Argument Register 0 - This is the stucture defined in GpioIntrCommand_t.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_GPIO_INT_CMD_SET   = 0x1a,
  /*! This command is used to return the current GPIO interrupt status in the firmware.
  @arguments
  - None.
  @response
  - Response Register 0 - The response is always @ref SMC_Result_OK as there is no invalid value.
  */
  BIOS_MPIO_MSG_HOTPLUG_GPIO_INT_STATUS_GET        = 0x1b,
  /*! This command is used to issue one of two GPIO related commands. These are based on a field in the
  argument passed to this command in the BIOS_HOTPLUG_COMMAND_FORMAT_t structure.
  These are:
  - Write the hotplug reset pin.
  - Read the hotplug reset pin.
  @arguments
  - None.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_GPIO_SERVICE       = 0x1c,
  /*! Unblock MSI interrupt for firmware first hotplug on a given NBIO.
  @arguments
  - Argument Register 0 - The zero based NBIO instance number to unblock.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_PORT_UNBLOCK       = 0x1d,
  /*! Cancel hot add for firmware first hotplug on a given NBIO.
  @arguments
  - Argument Register 0
      - byte 0 is the port
      - byte 1 is the pcie instance
      - byte 2 is the zero based NBIO instance
      - byte 3 bit 0 is clear to set bridge disable or 1 to clear bridge disable.
  @response
  - Response Register 0 - Is either @ref SMC_Result_Failed or @ref SMC_Result_OK if the value was accepted.
  */
  BIOS_MPIO_MSG_HOTPLUG_ADD_CANCEL         = 0x1e,
  BIOS_MPIO_MSG_MESSAGE_COUNT              = 0x20
} MPIO_MESSAGES;


/**
 *  @brief Transfer Direction
 *
 *  This enumeration defines values for direction of TRANSFER_ASK
 */
typedef enum {
    MPIO_TO_DRAM = 0,     ///< Transfer from MPIO into DRAM
    DRAM_TO_MPIO          ///< Transfer from DRAM into MPIO
} TRANSFER_DIRECTION;

/**
 *  @brief Link Selector
 *
 *  This enumeration defines values for selection of link transfers
 */
typedef enum {
    ALL_LINKS = 0,        ///< Transfer ALL links
    SELECTED_LINKS        ///< Transfer SELECTED links
} LINK_SELECTOR;

/**
 *  @brief MPIO Link States
 *
 *  This enumerator defines the link status values for MPIO FW3.
 */
typedef enum {
    LINK_NOT_ALLOCATED = 0,
    LINK_ALLOCATED,
    LINK_PROVISIONED,
    LINK_BIFURCATION_FAILURE,
    LINK_RESET,
    LINK_NOT_TRAINED,
    LINK_TRAINED,
    LINK_FAILURE,
    LINK_TRAINING_FAILURE,
    LINK_TIMEOUT
} MPIO_LINK_STATE;

/**
 *  @brief MPIO Control Type
 *
 *  This enumerator defines the link types for the MPIO FW3 ASK structure.
 */
typedef enum {
  ASK_TYPE_PCIe=0,
  ASK_TYPE_SATA=1,
  ASK_TYPE_xGMI=2,
  ASK_TYPE_GMI=3,
  ASK_TYPE_ETH=4,
  ASK_TYPE_USB=5,
  //  dxio_ctrl_type_WAFL=5, will be type xGMI, sub-type WAFL
} MPIO_ASK_TYPE;

typedef struct {
 // DWORD 0 - General
    UINT32    skipVetting                 :1;
    UINT32    ntbhotpluginterval          :1;
    UINT32    saverestoremode             :2;
    UINT32    matchPortSizeExactly        :1;
    UINT32    skipLinkDisableOnFailure    :1;
    UINT32    usePhySram                  :1; // If optional
    UINT32    ValidPhyFWFlag              :1; // If optional
    UINT32    padDword0                   :24;

 // DWORD 1 - Power Management
    UINT32    PWRMNGMT_PRFRM_CLK_GATING            :1;
    UINT32    PWRMNGMT_PRFRM_STATIC_PWR_GATING     :1;
    UINT32    PWRMNGMT_PRFRM_REFCLK_SHUTDOWN       :1;
    UINT32    CBSOPTIONS_ENABLEPOWERMANAGEMENT     :1;
    UINT32    PWRMNGMT_PRFRM_PMA_POWER_GATING      :1;
    UINT32    PWRMNGMT_PRFRM_PMA_CLOCK_GATING      :1;
    UINT32    padDword1                            :26;

 // DWORD 2 - PCIE Link Timeouts (in msec)
    UINT32    LinkReceiverDetectionPolling          :8;
    UINT32    LinkResetToTrainingTime               :8;
    UINT32    LinkL0Polling                         :16;

 // DWORD 3 - Protocol
    UINT32     PCIE_ALLOW_COMPLETION_PASS           :1;
    UINT32     CBSOPTIONS_ALLOWPOINTERSLIPINTERVAL  :1;
    UINT32     padDword3                            :30;

 // DWORD 4 - PCIE PERST
    UINT32     GPIO26_GENERIC_RST                   :1;
    UINT32     GPIO40_NVME_RST                      :1;
    UINT32     GLOBAL_RST                           :30;      // From Rembrandt

 // DWORD 5 - Future
    UINT32     padDword5;
} MPIO_GLOBAL_CONFIG;

typedef struct {
 // Byte 0
    UINT32    devfuncnumber;
 // Byte 4
    UINT32    portPresent        :1;
    UINT32    earlyTrainLink     :1;
    UINT32    linkComplianceMode :1;
    UINT32    pad                :1;
    UINT32    linkHotplugType    :4; /// Move NTB Hotplug to be part of this enum

 // Byte5
    UINT32    maxLinkSpeedCap    :4;
    UINT32    targetLinkSpeed    :4;

 // Byte 6
    UINT32    PSPPMode           :3;
    UINT32    partnerDeviceType  :2;
    UINT32    PSPP_pad           :3;

 // Byte 7
    UINT32    localPerst         :1;
    UINT32    bifMode            :1;
    UINT32    isMasterPLL        :1;
    UINT32    invertPolarityRx   :1;
    UINT32    invertPolarityTx   :1;
    UINT32    ctrl_pad           :3;

// Byte 8
    UINT32    gen3EqSearchMode   :2;
    UINT32    gen3EqSearchModeEn :2;
    UINT32    gen4EqSearchMode   :2;
    UINT32    gen4EqSearchModeEn :2;

// Byte 9
    UINT32    gen5EqSearchMode   :2;
    UINT32    gen5EqSearchModeEn :2;
    UINT32    gen6EqSearchMode   :2;
    UINT32    gen6EqSearchModeEn :2;

// Byte 10
    UINT32    txDeEmphasis       :2;
    UINT32    txDeEmphasisEn     :1;
    UINT32    txVetting          :1;
    UINT32    rxVetting          :1;
    UINT32    txrx_padding       :3;

// Byte 11
    UINT32    esmSpeed           :6;
    UINT32    esmMode            :2;

// Byte 12-19
    UINT8     hfc_index;
    UINT8     dfc_index;
    UINT16    dwPad1;
    UINT32    dwPad2;
    UINT32    dwPad3;
} FW3_LINK_ATTR;

typedef struct {
    UINT32   startLaneIndex     :16;  ///< The lowest number DXIO lane assigned to this link
    UINT32   numberOfLanes      :6;   ///< The number of lanes assigned to this link
    UINT32   reversed           :1;   ///< Indicates that the link is logically reversed
    UINT32   linkStatus         :5;   ///< Reserved for ICTFW use only
    UINT32   ctrlType           :4;   ///< Link type as defined in #MPIO_ASK_TYPE
    UINT32   gpioHandle         :8;   ///< Platform identifier for PERST# for this link
    UINT32   channelType        :8;   ///< Channel Type
    UINT32   ancillaryDataIdx   :16;  ///< Offset of extended attributes for this link in ancillary data

    FW3_LINK_ATTR link_attributes;    ///< #FW3_LINK_ATTR structure
} FW3_LINK_STRUCT;

typedef struct {
    UINT32   state              :4;  ///< State of link represented as ICTFW_LINK_STATES
    UINT32   speed              :7;  ///< Speed of link represented as ICTFW_LINK_SPEEDS
    UINT32   width              :5;  ///< Number of lanes in-use by link

    UINT32   port               :8;  ///< Port associated with this link (0 except for PCIE
    UINT32   pad                :8;  ///< Reserved
} ICTFW_LINK_STATUS;

/**
 *  @brief MPIO ASK STRUCTURE
 *
 *  @detail ASK is comprised of link description and link status
 *   To prevent runtime translation, same FW3_LINK_STRUCT is used for link description
 *   To keep struct binary compatible after simple changes, some reserved fields are added
 */
typedef struct {
    FW3_LINK_STRUCT   desc;
    ICTFW_LINK_STATUS status;

    UINT32            reserved[4];
} FW_ASK_STRUCT;


/*! The current state of this DFC. (Word 2 Byte 0) */
typedef union {
  struct {
    /*! Current device type installed.
        This contains the PRSNT, IFDET, and IFDET2 pin information for UBM.
    */
    UINT8     type:3;
    /*! Reserved for type expansion. */
    UINT8     rsvd:3;
    //! If set port is currently bifurcated.
    UINT8     bifurcate_port:1;
    /*! If set then information is for a secondary port. */
    UINT8     secondary_port:1;
  } Field;
  /*! Used to directly access the byte. */
  UINT8 Value;
} dfcState;

/*! The static features for this DFC. (Word 2 Byte 1) */
typedef union {
  struct {
    //! If set reference clock routing is present.
    UINT8     ref_clk:1;
    //! If set power disable feature is present.
    UINT8     power_disable:1;
    //! If set PCIe reset feature is present.
    UINT8     perst:1;
    //! If set dual port feature is present.
    UINT8     dual_port:1;
    /*! Reserved. */
    UINT8     rsvd:4;
  } Field;
  /*! Used to directly access the byte. */
  UINT8 Value;
} dfcFeats;


/*! This is the event structure used to respond to initial connection information during enumeration and
during hotplug events for UBM interfaces.  For BIOS_EVENT_DEVICE_CONNECTED and
BIOS_EVENT_DEVICE_NOT_PRESENT events all values are filled with valid data. For
BIOS_EVENT_DEVICE_DISCONNECTED only the hfc_idx and event values are valid, no other values can be
used in this case. */
typedef struct
{
    /*! HFC index to match to BIOS database. */
    UINT8 hfc_idx;
    /*! The event that triggered this message and is one of the BIOS_EVENT_DEVICE_ values. */
    UINT8 event;
    /*! The total number of DFCs for this HFC, this is always returned. (Word 0) */
    UINT16 num_dfcs;
    /*! The logical lane assignment for this DFC (0-15). (Word 1 Byte 0) */
    UINT8 lane_start;
    /*! The lane width for this DFC (1-16). (Word 1 Byte 1) */
    UINT8 lane_width;
    /*! The type specific rate for this DFC. (Word 1 Byte 2) */
    /*! The PCIe gen speed. (0-7) A value of 7 is No limit. */
    /*! The SATA Link Rate. (0-3) Value of 3 is No Limit. */
    UINT8 genspeed;
    /*! The current state of this DFC. (Word 2 Byte 0) */
    dfcState  state;
    /*! The static features for this DFC. (Word 2 Byte 1) */
    dfcFeats  features;
    /*! Align to a 32-bit word. (Word 2 Byte 2-3) */
    UINT8 slot;
    /*! Align to a 32-bit word. (Word 2 Byte 2-3) */
    UINT8 : 8;
    UINT8 : 8;
} DFC_DESCRIPTOR;

/*! This is the device info structure to describe CXL devices.
    This information is used for persistent memory messaging support  */
typedef struct {
    UINT32 function  : 3;
    UINT32 device    : 5;
    UINT32 bus       : 8;
    UINT32 iohc_id   : 4; // IOHC0-3
    UINT32 pcie_port : 4; // Port0-3 (up to 4 per PCIe Controller/IOHC)
    UINT32 cxl_type  : 4; // Type1-3
    UINT32 unused    : 4;
} CXL_DEVICE_INFO;

/**
 *  Message argument structures
 */

typedef struct {
  UINT32      Version;
  UINT32      FwPostCode;
  UINT32      FwStatus;
  UINT32      SharedMemVersion;
  UINT32      Reserved[2];
} GET_FIRMWARE_INFO_RESULTS;

typedef struct {
  UINT32      CmdStatus;
  UINT32      CycleStatus;  ///< Phases already executed
  UINT32      FwPostCode;
  UINT32      FwStatus;
  UINT32      Reserved[2];
} GET_STATUS_RESULTS;

typedef struct {
  UINT32    DestAddressHi;
  UINT32    DestAddressLo;
  UINT32    LinksOnly:1;      ///< Links Only (0 = entire struct, 1 = only selected links)
  UINT32    Reserved:31;
  UINT32    StartLink;
  UINT32    LinkCount;
  UINT32    Reserved1;
} GET_ASK_RESULT_ARGS;

typedef struct {
  UINT32    Result;
  UINT32    BytesTransferred;
  UINT32    Reserved[4];
} GET_ASK_RESULT_RESULTS;

typedef struct {
  UINT32    Map:1;            ///< Reconcile (Map)
  UINT32    Configure:1;      ///< PCS/PHY Programing  (Configure)
  UINT32    Reconfigure:1;    ///< Controller programming (Reconfigure)
  UINT32    PerstReq:1;       ///< Perst Callout
  UINT32    Training:1;       ///< Training
  UINT32    Enumerate:1;      ///< Enumerate
  UINT32    Reserved:26;
  UINT32    Reserved1[5];
} SETUP_LINK_ARGS;

typedef struct {
  UINT32    Result;
  UINT32    Map:1;            ///< Reconcile (Map)
  UINT32    Configure:1;      ///< PCS/PHY Programing  (Configure)
  UINT32    Reconfigure:1;    ///< Controller programming (Reconfigure)
  UINT32    PerstReq:1;       ///< Perst Callout
  UINT32    Training:1;       ///< Training
  UINT32    Enumerate:1;      ///< Enumerate
  UINT32    Reserved:26;
  UINT32    Reserved1[4];
} SETUP_LINK_RESULTS;

typedef struct {
  UINT32    DramAddressHi;    ///<
  UINT32    DramAddressLo;    ///<
  UINT32    Links:1;          ///<
  UINT32    Direction:1;      ///<
  UINT32    Reserved:30;
  UINT32    StartLink;
  UINT32    LinkCount;
  UINT32    Reserved1;
} TRANSFER_ASK_ARGS;

typedef struct {
  UINT32    Result;
  UINT32    BytesTransferred;
  UINT32    Reserved[4];
} TRANSFER_ASK_RESULTS;

//=============================================
// UBM Structures
//=============================================

#define MAX_I2C_TOPOGRAPHY_DEPTH  2

typedef enum {
    /*! This is a UBM connected node (ubm_i2c_info_t.node_type). */
    MPIO_I2C_NODE_TYPE_UBM  = 0,
    /*! This is a OCP connected node (ubm_i2c_info_t.node_type). */
    MPIO_I2C_NODE_TYPE_OCP  = 1,
    /*! This is a U.2 connected node (ubm_i2c_info_t.node_type). */
    MPIO_I2C_NODE_TYPE_U2   = 2,
    /*! This is a U.3 connected node (ubm_i2c_info_t.node_type). */
    MPIO_I2C_NODE_TYPE_U3   = 3,
} i2c_node_type;

typedef struct {
  UINT8 Addr;       ///< The address of the gpio Expander for this HFC.
  UINT8 Type;       ///< The type of this GPIO expander and is one of the following values:
                    ///<  - UBM_GPIO_EXPANDER_TYPE_9539
                    ///<  - UBM_GPIO_EXPANDER_TYPE_9535
                    ///<  - UBM_GPIO_EXPANDER_TYPE_9506
  UINT8 Start_Lane; ///< Reserved
  UINT8 Bp_Type;    ///< This is the bit offset for the bp_type pin in a GPIO expander.
  UINT8 I2c_Reset;  ///< This is the bit offset for the i2c_reset pin in a GPIO expander.
  UINT8 Reserved0;  ///< This is the bit offset for the perst pin in a GPIO expander.
  UINT8 SlotNum;    ///< Reserved
} I2C_EXPANDER;

/// The information describing the path to this HFC. */
typedef struct {
  UINT8 Addr;     ///< The I2C address of this I2C Switch. A value of UBM_INVALID_SWITCH_ADDR indicates the entry must not be used.
  UINT8 Select:4; ///< The switch selection port for this HFC which has valid values from 0-3.
  UINT8 Type:4;   ///< Reserved for future use, must be zero.
} I2C_SWITCH;

typedef struct {
  UINT8           NodeType;
  I2C_EXPANDER    Expander;
  I2C_SWITCH      Switch[MAX_I2C_TOPOGRAPHY_DEPTH];
} MPIO_HFC_PORT;

/// UBM HFC Descriptor
typedef struct {
  IN       UINT32          Size;                 ///< Socket Id
  IN       MPIO_HFC_PORT   HfcPortList[32];      ///< Pointer to array of PCIe port descriptors or NULL (Last element of array must be terminated with DESCRIPTOR_TERMINATE_LIST).
} MPIO_UBM_HFC_DESCRIPTOR;

#pragma pack (pop)

VOID
ParseTopologyForUbm (
  IN      DXIO_COMPLEX_DESCRIPTOR     **ComplexDescriptor,
  IN      PCIe_PLATFORM_CONFIG        *Pcie
  );

AGESA_STATUS
MpioEarlyInitV1 (
  IN      PCIe_PLATFORM_CONFIG            *Pcie,
  IN      GNB_HANDLE                      *StartHandle,
  IN      DXIO_COMPLEX_DESCRIPTOR         *PlatformTopology
  );

//Hot plug structures

#define MAX_HOTPLUG_PCIE_PORTS 160
#define BIOSSMC_MSG_Param_HotplugForceA0Mode            0x04
#define BIOSSMC_MSG_Param_HotplugPolling                0x10
#define BIOSSMC_MSG_Param_HotplugPlatformFirst          0x20
#define BIOSSMC_MSG_Param_HotplugSFI                    0x40

typedef struct {
  UINT32 HotPlugFormat          : 3;
  UINT32 ResetDescriptorValid   : 1;
  UINT32 PortActive             : 1;
  UINT32 MasterSlaveAPU         : 1;
  UINT32 DieID                  : 1;
  UINT32 PciePortInstance       : 4;
  UINT32 PcieTileInstance       : 4;
  UINT32 BridgeOffset           : 5;
  UINT32                        : 4;
  UINT32 AltSlotNum             : 6;
  UINT32 SecondaryLink          : 1;
  UINT32                        : 1;
} PcieHotPlugMappingDescriptorV3_t;

typedef struct {
  UINT32 i2c_bit_or_nibble_select   :   3;  // [2] Nibble Select for Enterprise SSD; [2:0] Bit Select for Simple Presence Detect
  UINT32 i2c_byte_select            :   3;
  UINT32 i2c_device_address         :   5;
  UINT32 i2c_device_type            :   2;
  UINT32 i2c_bus_segment            :   5;
  UINT32 function_mask              :   8;
  UINT32 i2c_bus_segment2           :   6;
} PcieHotPlugFunctionDescriptor_t;

typedef struct {
  UINT32                            :   3;
  UINT32 i2c_byte_select            :   3;
  UINT32 i2c_device_address         :   5;
  UINT32 i2c_device_type            :   2;
  UINT32 i2c_bus_segment            :   5;
  UINT32 reset_select               :   8;
  UINT32                            :   6;
} PcieHotPlugResetDescriptor_t;

typedef struct {
  PcieHotPlugMappingDescriptorV3_t  MappingDescriptor   [MAX_HOTPLUG_PCIE_PORTS];
  PcieHotPlugFunctionDescriptor_t FunctionDescriptor  [MAX_HOTPLUG_PCIE_PORTS];
  PcieHotPlugResetDescriptor_t    ResetDescriptor     [MAX_HOTPLUG_PCIE_PORTS];
} PcieHotPlugConfigTable_t;

#endif


