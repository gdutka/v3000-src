/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#ifndef _AOD_BASE_LIB_H_
#define _AOD_BASE_LIB_H_
#include <Uefi/UefiInternalFormRepresentation.h>
#include <Features/AODv2/Include/AodSetup.h>
#include <Features/AODv2/Include/AodCoreInfo.h>
///
/// RamDebug buffer layout for this instance:
/// ----------------------------------------------------------------------
/// |Signature|Length  |HiiDataAddress(rsved)  |    Offset    |DebugMessage|
/// ----------------------------------------------------------------------
/// |'AODD'   |0x10000 | Front/String Pkg(TBD) |start|current |  Ascii     |
/// ----------------------------------------------------------------------
/// Length is UINT32, it is total debug message received plug UINT32 Length field.
/// The first UINT16 is regarded as offset, because total buffer length is 0x10000.
///
#define RAM_DEBUG_LENGTH               0x10000
#define RAM_DEBUG_SIGN_LENGTH          0x04   //SIGNATURE_32 ('A', 'O', 'D', 'D')
#define RAM_DEBUG_LENGTH_LENGTH        0x04   //UINT32
#define RAM_DEBUG_HII_ADDRESS_LENGTH   0x04   //UINT32
#define RAM_DEBUG_MSG_OFFSET_LENGTH    0x04   //UINT32

#define RAM_DEBUG_MSG_OFFSET_OFFSET (RAM_DEBUG_LENGTH_LENGTH + RAM_DEBUG_HII_ADDRESS_LENGTH + RAM_DEBUG_SIGN_LENGTH)
#define RAM_DEBUG_MSG_START    (RAM_DEBUG_MSG_OFFSET_OFFSET + 0x4)
#define RAM_DEBUG_MSG_OFFSET_MAX (RAM_DEBUG_LENGTH - RAM_DEBUG_SIGN_LENGTH - RAM_DEBUG_LENGTH_LENGTH - RAM_DEBUG_HII_ADDRESS_LENGTH - RAM_DEBUG_MSG_OFFSET_LENGTH)
/**
  Write data from buffer to RAM debug.

  Writes NumberOfBytes data bytes from Buffer to the serial device.
  The number of bytes actually written to the serial device is returned.
  If the return value is less than NumberOfBytes, then the write operation failed.
  If Buffer is NULL, then ASSERT().
  If NumberOfBytes is zero, then return 0.

  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to the serial device.

  @retval 0                NumberOfBytes is 0 or the first byte in Buffer is a '\0'
  @retval >0               The number of bytes written to the serial device.
                           If this value is less than NumberOfBytes, then the read operation failed.
**/
UINTN
EFIAPI
AodRamDebugWrite (
  IN  VOID          *RamPointer,
  IN  UINT8         *Buffer,
  IN  UINTN         NumberOfBytes
  );

/**
  this function is used to print debug info.
  @param[in]  DebugLevel          Con Out message level
  @param[in]  pFileName           Current file name.
  @param[in]  pFunctionName       Current function name.
  @param[in]  CmdId               WMI cmdid
  @param[in]  Format               WMI cmdid
  @param  ...             - A variable argument list whose contents are accessed based on the format string specified by Format.
  @retval none
**/
VOID
EFIAPI
AodDebugMsg (
  IN  UINTN               DebugLevel,
  IN  CONST CHAR8         *pFileName,
  IN  CONST CHAR8         *pFunctionName,
  IN  UINTN               LineNumber,
  IN  CONST CHAR8         *pSocName,
  IN  CONST CHAR8         *Format,
  ...
  );

#if !defined(MDEPKG_NDEBUG)
  ///
  /// The module name is used primarily for error and debug messages.
  /// The __FILE__ macro is not very useful for this, because it
  /// usually includes the entire pathname to the module making the
  /// debug output difficult to read.
  ///
  #define AOD_MOUDLE_NAME(Name)          static CONST CHAR8 *_AodModuleName = Name;
  #define AOD_SOC_NAME(Name)             static CONST CHAR8 *_AodSocName = Name;
  #define _AOD_DEBUG(PrintLevel, ...)                                                     \
    do {                                                                                  \
      AodDebugMsg (PrintLevel, _AodModuleName, __FUNCTION__,  __LINE__ , NULL, ##__VA_ARGS__);       \
    } while (FALSE)
  #define _AOD_DEBUG_SOC(PrintLevel, ...)                                                     \
    do {                                                                                  \
      AodDebugMsg (PrintLevel, _AodModuleName, __FUNCTION__,  __LINE__ , _AodSocName, ##__VA_ARGS__);       \
    } while (FALSE)
  #define AOD_DEBUG(Expression)  _AOD_DEBUG Expression
  #define AOD_DEBUG_SOC(Expression)  _AOD_DEBUG_SOC Expression
#else
  #define AOD_MOUDLE_NAME(Name)
  #define AOD_SOC_NAME(Name)
  #define _AodModuleName " "
  #define _AodSocName NULL
  #define AOD_DEBUG DEBUG
  #define AOD_DEBUG_SOC DEBUG
#endif


#pragma pack(1)
#define AOD_MAX_INIT_NUM                 0x10
typedef enum {
  AOD_UNKNOWN_INIT                  = 0x00,
  ///
  /// AodDispatchFlag Init, this will been called every init function dispatch
  /// AOD_INIT_FUNCTION (out u16 *AodDispatchFlag)
  ///
  AOD_DISPATCH_INIT                = 0x01,
  ///
  /// Update AOD nvs Init, this will been called at AodDxe/SMM
  /// used to update initial NVS value at AodDxe:
  ///   AOD_INIT_FUNCTION (in,out void *nvs)
  /// used to register AOD_SMM_CMD_TO_NVS phase callback at SMM:
  ///   AOD_INIT_FUNCTION (in AOD_CMD_SMM_DISPATCH_PROTOCOL *protocol)
  ///
  AOD_UPDATE_NVS                    = 0x02,
  ///
  /// Aod AOD_UPDATE_VARIABLE Init, this will been called at SMM
  /// used to register AOD_SMM_CMD_TO_VARIABLE phase callback:
  ///   AOD_INIT_FUNCTION (in AOD_CMD_SMM_DISPATCH_PROTOCOL *protocol)
  ///
  AOD_UPDATE_VARIABLE               = 0x03,
  ///
  /// Aod AOD_UPDATE_CORE_INFO Init, this will been called at PEI
  /// used to register update Policy value at endofpei:
  ///   AOD_INIT_FUNCTION (in,out AOD_CORE_INFO *CoreInfo)
  ///
  AOD_UPDATE_CORE_INFO               = 0x04,
  ///
  /// Aod AOD_SYNC_VARIABLE_PCD Init, this will been called at PEI
  /// used to update AGESA Pcd with "AOD_SETUP" at PEI:
  ///   AOD_INIT_FUNCTION (in AOD_CONFIG *AodConfig)
  ///
  AOD_SYNC_VARIABLE_PCD             = 0x05,
  ///
  /// Aod AOD_CONFIG_LOAD_DEFAULT Init, this will been called at AodDxe
  /// used to load "Aodsetup" default at AodDxe:
  ///   AOD_INIT_FUNCTION (in AOD_CONFIG *AodConfig)
  ///
  AOD_CONFIG_LOAD_DEFAULT           = 0x06,
  ///
  /// Update AOD_CONFIG_SYNC_APCB Init, this will been called at AodDxe/SMM
  /// used to sync aod variable into APCB value at AodDxe SETUP rountie:
  ///   AOD_INIT_FUNCTION (in AOD_CONFIG *AodConfig)
  /// used to register AOD_SMM_VARIABLE_TO_APCB phase callback at SMM:
  ///   AOD_INIT_FUNCTION (in AOD_CMD_SMM_DISPATCH_PROTOCOL *protocol)
  ///
  AOD_CONFIG_SYNC_APCB              = 0x07,
  ///
  /// Aod AOD_UPDATE_HIIRESOURCE Init, this will been called at AodDxe
  /// used to add AOD hii front/string database into hii database:
  ///   AOD_INIT_FUNCTION (in EFI_HANDLE DriverHandler)
  ///
  AOD_UPDATE_HIIRESOURCE             = 0x08,
  ///
  /// Aod AOD_BUILD_WMI_CMD Init, this will been called at AodDxe
  /// used to register defalut WMI cmd list at AodDxe:
  ///   AOD_INIT_FUNCTION (in AOD_CMD_PROTOCOL *protocol)
  ///
  AOD_BUILD_WMI_CMD                 = 0x09,
  ///
  /// Aod AOD_UPDATE_HII_CALLBACK Init, this will been called at AodDxe
  /// used to register AodHiiCallback at AodDxe:
  ///   AOD_INIT_FUNCTION (in callbacklist)
  ///
  AOD_UPDATE_HII_CALLBACK            = 0x0A,

  ///
  /// Aod AOD_CONFIG_DEBUG, this will been called at AodDxe
  /// used to show debug message for:
  ///   AOD_INIT_FUNCTION (in AOD_CONFIG *AodConfig)
  ///
  AOD_CONFIG_DEBUG                   = 0x0B,
} AOD_INIT_PHASE;

typedef
EFI_STATUS
(EFIAPI *AOD_INIT_FUNCTION) (
  IN VOID     *Content
  );

typedef struct {
  UINT16            AodDispatchFlag;
  UINT16            Phase;
  AOD_INIT_FUNCTION Init;
} AOD_INIT_TABLE;
#pragma pack()

/**
  AOD setup callback routine define
  @param[in]  HiiHandle          the hii handle
  @param[in]  Action             Specifies the type of action taken by the browser.
  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  Type               The type of value for the question.
  @param[in]  Value              A pointer to the data being sent to the original
                                 exporting driver.
  @param[in]  HiiHandle          the ifr varstore data
  @param[out] Req                On return, points to the action requested by the
                                 callback function.
**/
typedef
EFI_STATUS
(EFIAPI *CALLBACK_ITEM_EX) (
  IN  EFI_HII_HANDLE                       Handle,
  IN  UINTN                                Action,
  IN  UINT16                               KeyValue,
  IN  UINT8                                Type,
  IN  EFI_IFR_TYPE_VALUE                   *Value,
  IN  VOID                                 *IfrData,
  OUT UINTN                                *Req
  );

#define AOD_DXE_SETUP_CALLBACK_SIGNATURE SIGNATURE_32 ('A', 'O', 'S', 'C')
typedef struct {
  UINT32                Signature;
  LIST_ENTRY            Link;
  BOOLEAN               UpdateForm;
  CALLBACK_ITEM_EX      Callback;
} AOD_SETUP_CALLBACK;
#define AOD_SETUP_CALLBACK_NODE_FROM_LINK(a)  CR (a, AOD_SETUP_CALLBACK, Link, AOD_DXE_SETUP_CALLBACK_SIGNATURE)


/**
  Retrieve "AodSetup" config

 @param[out] AodConfiguration        The Pointer to the structure of AOD_CONFIG

 @retval EFI_SUCCESS            The AOD configuration is successfully retrieved

**/
EFI_STATUS
EFIAPI
AodGetConfiguration (
  OUT VOID        **ppAodConfiguration
  );

/**
  Set "AodSetup" config

  @param[out] pAodConfiguration     The Pointer to the structure of AOD_CONFIG
  @param[out] VarSize               VarSize


  @retval EFI_SUCCESS            The AOD CONFIG is successfully set

**/
EFI_STATUS
EFIAPI
AodSetConfiguration (
  IN  VOID          *pAodConfiguration,
  IN  UINTN         VarSize
  );

/**
  Retrieve AOD_CORE_INFO data

  @param[out]       AodCoreInfo   The Pointer to the structure of AOD_CORE_INFO,
                                  this pointer must be allocated with sizeof(AOD_CORE_INFO)
                                  before being called

 @retval EFI_SUCCESS            The AOD configuration is successfully retrieved
 @retval EFI_INVALID_PARAMETER  NULL pointer for input AodCoreInfo paramater
 @return EFI_NOT_FOUND          can't found the guid hob
**/
EFI_STATUS
EFIAPI
AodGetCoreInfo (
  OUT AOD_CORE_INFO     *AodCoreInfo
  );

/**
  Set AOD_CORE_INFO

  @param[in]       AodCoreInfo     the Pointer to the structure of AOD_CORE_INFO,


  @retval EFI_SUCCESS            The AOD configuration is successfully retrieved
  @retval EFI_INVALID_PARAMETER  NULL pointer for input AodCoreInfo paramater
  @return EFI_OUT_OF_RESOURCES   Failed to AllocateBuffer
**/
EFI_STATUS
EFIAPI
AodSetCoreInfo (
  IN AOD_CORE_INFO        *AodCoreInfo
  );

/**
  Aod Init Dispatch function

  @param[in]  Phase            Aod Init Phase identify
  @param[in]  Context          Aod Init Context Pointer

  @retval     EFI_SUCCESS          The function completes successfully.
  @retval     EFI_UNSUPPORTED      The AodInitTableList is not init properly.
  @retval     Others By callee
**/
EFI_STATUS
AodDispatchInitTable (
  IN  UINT8           Phase,
  IN  VOID            *Context
  );

/**
  Add Aod init table into list

  @param[in]  AodInitTable        Pointer to the array of the Aod Init Table.

  @retval  EFI_SUCCESS             Table add successfully.
  @retval  EFI_INVALID_PARAMETER   Input is Null
**/
EFI_STATUS
AodAddInitTable (
  IN  AOD_INIT_TABLE  *AodInitTable
  );

/**
  Read SMN register in DWord

  @param[in] DieBusNum       Die bus number
  @param[in] SmnAddress      Register SMN address
  @param[in] Value           Pointer to register value

**/
VOID
AodSmnRead (
  IN       UINT8               DieBusNum,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

/**
  Write SMN register in DWord

  @param[in] DieBusNum       Die bus number
  @param[in] SmnAddress      Register SMN address
  @param[in] Value           Pointer to register value

**/
VOID
AodSmnWrite (
  IN       UINT8               DieBusNum,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

/**
  This function is used to get active bit counts

  @param[in] Value   the input value

  @retval the count number of active bits
**/
UINT32
GetSetBitCount (
  IN       UINT32 Value
  );

/**
  Add Aod Setup Callback
  @param[in]       List           --  Aod Callback link list
  @param[in]       UpdateForm     --  Aod Callback update front package
  @param[in]       Callback       --  Aod Callback Routine

 @retval EFI_OUT_OF_RESOURCES  Allocate Pool failed
**/
EFI_STATUS
EFIAPI
AodAddSetupCallback (
  IN  LIST_ENTRY                        *List,
  IN  BOOLEAN                           UpdateForm,
  IN  CALLBACK_ITEM_EX                  Callback
  );
///
/// AOD SMM communication Fucntion Id
///
typedef enum {
  AOD_SMM_COMM_VARIABLE_HOOK           = 0x1,
  AOD_SMM_COMM_ID_MAX                  = 0x2
}  AOD_SMM_COMM_FUNCTION_ID;

#pragma pack(1)
typedef struct _AOD_SMM_COMMUNICATE_HEADER {
  UINT8                       Id;              ///< ID of smm communication buffer, used for dispatch
  EFI_STATUS                  RetStatus;       ///< Return Status of Communicate handler
  UINT8                       Data[1];         ///< data
} AOD_SMM_COMMUNICATE_HEADER;
#pragma pack()

/**
  Send the Data in communicate Buffer to SMM.
  @param[in]   Id                     AOD_SMM_COMM_FUNCTION_ID.
  @param[in]   Buffer                 Points to the Data in the communicate Buffer.
  @param[in]   DataSize               This Size of the function Header and the Data.

  @retval      EFI_SUCCESS            Success is returned from the functin in SMM.
  @retval      Others                 Failure

**/
EFI_STATUS
EFIAPI
AodSendCommunicateBuffer (
  IN      UINT8                             Id,
  IN      VOID                              *Data,
  IN      UINTN                             DataSize
  );
/**
  VID3 to voltage mv, acc to SVI3 spec, type 1 slave vid table
  5mv is a step, SIVD[8:0] max means 0x1ff 2.8v
**/
#define VID3_TO_MV(vid)     (((vid&0x1FF) >= 1) ? (250 + (5 * ((vid&0x1FF) - 1))) : 0)
#define mV_TO_VID3(voltage) (((voltage) >= 250) ? ((UINT32)((((voltage) - 250) / 5) + 1)) : 0)
#define VID3_MAX_VOLTAGE 2800

///< [31:28] --> CCD, [27:20] --> CORE, 0xFFFFFFFF --> all Core, 0xFFFFFFFE --> Array Terminate flag
#define AOD_PER_CORE_CURVE_INDICATER(ccd,core)  ((UINT32)(((ccd&0x7)<<28) + (UINT32)((core&0xff)<<20)))
#define AOD_ALL_CORE_CURVE_INDICATER(ccd,core)  0xFFFFFFFF
#define AOD_TERMINATE_CURVE_INDICATER(ccd,core) 0xFFFFFFFE
#endif