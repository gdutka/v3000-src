/** @file
  GUID for INTEL REMAP DEVICE Info Variables.

**/
#include <Protocol/HddPasswordService.h>
#include <Guid/HddPasswordVariable.h>

#ifndef __INTEL_REMAP_DEV_INFO_GUID_H__
#define __INTEL_REMAP_DEV_INFO_GUID_H__

//
// Variable Name for Remap Device Info
//
#define SAVE_HDD_PASSWORD_FOR_REMAP_DEVICE_NAME        L"SaveIntelRemapDevice"

#define OPAL_DEVICE_TYPE_UNKNOWN    0x0
#define OPAL_DEVICE_TYPE_ATA        0x1
#define OPAL_DEVICE_TYPE_NVME       0x2
#define OPAL_DEVICE_TYPE_REMAP_NVME 0x3  
#define OPAL_DEVICE_TYPE_TG         0x4  

#define INTEL_REMAP_DEV_INFO_VARIABLE \
  { \
    0xa8588563, 0xe0f9, 0x4dad, {0xab, 0xa0, 0xe2, 0x7a, 0x70, 0xf6, 0x41, 0xa3} \
  }
#pragma pack(1)
typedef struct {
  UINT16            Segment;
  UINT8             P2pBridgeSeg;
  UINT8             P2pBridgeBus;
  UINT8             P2pBridgeDevice;
  UINT8             P2pBridgeFunction;
  UINT8             Bus;
  UINT8             Device;
  UINT8             Function;
  UINT16            Port;
  UINT16            PortMultiplierPort;
  UINT16            ControllerNumberForHddPasswordTable;  
  UINT8             PasswordLength;
  UINT8             Password[HDD_PASSWORD_MAX_NUMBER + 1];
  UINT8             DeviceType;
  UINT32            BarAddr;
  UINT32            NvmeNamespaceId;
} OPAL_DEVICE_INTEL_REMAP;
#pragma pack()

extern EFI_GUID gSaveIntelRemapDevInfoGuid;

#endif
