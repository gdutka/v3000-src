/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _HG_DATA_INFO_
#define _HG_DATA_INFO_

///
/// Forward reference for pure ANSI compatability
///
typedef struct _EFI_HG_NVS_AREA_PROTOCOL EFI_HG_NVS_AREA_PROTOCOL;
///
/// Global NVS Area Protocol GUID
///
#define HG_DATA_INFORMATION_PROTOCOL_GUID \
  { \
    0xbb210948, 0x3101, 0x4cef, 0x8d, 0x1d, 0x3f, 0xe5, 0x5e, 0x61, 0x53, 0xb4 \
  }
///
/// Revision id - Added TPM related fields
///
///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gEfiHgNvsAreaProtocolGuid;

//
// This structure must match with the structure in AmdSsdt.asl and NvidiaSsdt.asl HG OpRegion "HGOP"
//
#pragma pack (push, 1)
typedef struct _EFI_HG_NVS_AREA_PROTOCOL {
  //
  // PEG Endpoint Info
  //
  UINT32      EndpointBaseAddress;          // PEG Endpoint PCIe Base Address
  UINT32      DgpuPcieCfgBaseAddress;       // dGPU Device's PCIe Config Space Base Address
  //
  // HG Board Info
  //
  UINT16      DgpuVendorID;
  UINT8       OptimusFlag;
  //
  // Nvidia dGPU Generation
  //
  UINT8       NvDgpuGen;
  //
  // dGPU SSID SVID
  //
  UINT32      DgpuSsidSvid;
  //
  // Nvidia DDS feature
  //
  UINT8       DisplayMode;     // DIS_MODE_FLAG: ACPI flag reflect to SBIOS option.
} EFI_HG_NVS_AREA_PROTOCOL;
#pragma pack (pop)

#endif //#ifndef HG_DATA_INFO
