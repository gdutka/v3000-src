/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_AER_CPM_NBIO_COM_PROTOCOL_H_
#define _AMD_AER_CPM_NBIO_COM_PROTOCOL_H_

///
/// This PPI provide interface to access Board ID information.
///
typedef struct _DXE_AMD_AER_CPM_NBIO_COM_PROTOCOL       DXE_AMD_AER_CPM_NBIO_COM_PROTOCOL;

#define AER_BDF_MAX_NUM 0x20

struct _DXE_AMD_AER_CPM_NBIO_COM_PROTOCOL{
  UINTN                     Revision;                   ///< Revision Number
  UINT32                    IsDesiredEP;                ///< Designate whether having needed wr Ep
  UINT32                    NumberOfRP;
  UINT32                    NumberOfEP;
  UINT32                    NumberOfBridge;
  UINT32                    AerBDFRp[AER_BDF_MAX_NUM];
  UINT32                    AerBDFBridge[AER_BDF_MAX_NUM];
  UINT32                    AerBDFEp[AER_BDF_MAX_NUM];
  UINT32                    AerBDFBridgeDeviceControl[AER_BDF_MAX_NUM];
  UINT32                    AerBDFEpDeviceControl[AER_BDF_MAX_NUM];
};

// Current PPI revision
#define AMD_AER_CPM_NBIO_PROTOCOL_REVISION   0x00

extern EFI_GUID gAmdAerCpmNbioComProtocolGuid;

#endif // _AMD_AER_CPM_NBIO_COM_PROTOCOL_H_


