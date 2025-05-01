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
#ifndef __AOD_CMD_PROTOCOL_H__
#define __AOD_CMD_PROTOCOL_H__


///
/// Global ID for the Aod Cmd Protocol
///
#define EFI_AOD_CMD_PROTOCOL_GUID \
  { 0x7942d0a0, 0xfd6b, 0x4e9a, { 0x8b, 0x49, 0xc5, 0x47, 0x1f, 0xf8, 0x38, 0x6 } };

typedef struct  _AOD_CMD_PROTOCOL         AOD_CMD_PROTOCOL;

/**
  Unregister a AOD cmd out of cmd list instance

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId

  @retval EFI_INVALID_PARAMETER input protocol is NULL
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful unregister the cmd
**/
typedef
EFI_STATUS
(EFIAPI *AOD_CMD_UPDATE_VALUE) (
  IN    AOD_CMD_PROTOCOL                    *This,
  IN    UINT32                              CmdId,
  IN    UINT8                               Property,
  IN    UINT32                              DataCount,
  IN    UINT32                              *pValue
  );

/**
  Update Command Id Current support value

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[in]     Current            Current value


  @retval EFI_INVALID_PARAMETER input protocol/data pointer is NULL or DataCount is Zero
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful update current value
**/

typedef
EFI_STATUS
(EFIAPI *AOD_CMD_UPDATE_CURRENT) (
  IN    AOD_CMD_PROTOCOL                    *This,
  IN    UINT32                              CmdId,
  IN    UINT32                              Current
  );
/**
  This function used to Register a AOD cmd into cmd list instance

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[in]     pCmdName           Pointer to AOD Cmd name

  @retval EFI_INVALID_PARAMETER input protocol is NULL
  @retval EFI_ALREADY_STARTED   this cmdid already been registerd
  @retval EFI_OUT_OF_RESOURCES allocate source failed
  @retval EFI_SUCCESS successful register the cmd
**/
typedef
EFI_STATUS
(EFIAPI *AOD_CMD_REGISTER) (
  IN    AOD_CMD_PROTOCOL                    *pThis,
  IN    UINT32                               CmdId,
  IN    CHAR8                                *pName
  );
/**
  Unregister a AOD cmd out of cmd list instance

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId

  @retval EFI_INVALID_PARAMETER input protocol is NULL
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful unregister the cmd
**/
typedef
EFI_STATUS
(EFIAPI *AOD_CMD_UNREGISTER) (
  IN    AOD_CMD_PROTOCOL                     *This,
  IN    UINT32                               CmdId
  );

/**
  Query Command Id information

  @param[in]     pThis              Pointer to the gAmdAODCmdProtocol protocol
  @param[in]     CmdId              AOD CmdId
  @param[out]    Current            Current value
  @param[out]    Current            Current value
  @param[out]    Index              Command Index
  @param[out]    Property           Property: 0 for discrete, 1 for linear
  @param[out]    DataCount          Supported Data Count
  @param[out]    Data               Pointer to supported Data

  @retval EFI_INVALID_PARAMETER input protocol/data pointer is NULL or DataCount is Zero
  @retval EFI_NOT_FOUND  this cmdid is not been registered
  @retval EFI_SUCCESS successful
**/
typedef
EFI_STATUS
(EFIAPI *AOD_CMD_QUERY_INFO) (
  IN    AOD_CMD_PROTOCOL                    *This,
  IN    UINT32                              CmdId,
  OUT   UINT32                              *Current,
  OUT   UINT32                              *Index,
  OUT   UINT8                               *Property,
  OUT   UINT32                              *DataCount,
  OUT   UINT32                              **Data
  );

struct _AOD_CMD_PROTOCOL {
  AOD_CMD_REGISTER                        Register;
  AOD_CMD_UNREGISTER                      UnRegister;
  AOD_CMD_UPDATE_VALUE                    UpdateValue;
  AOD_CMD_UPDATE_CURRENT                  UpdateCurrent;
  AOD_CMD_QUERY_INFO                      QueryInfo;
};

extern EFI_GUID gAmdAodCmdProtocolGuid;

#endif