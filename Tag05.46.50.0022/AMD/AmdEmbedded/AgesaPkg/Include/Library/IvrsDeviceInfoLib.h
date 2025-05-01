/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Platform PCIe Complex Definitions
 *
 * Contains AMD AGESA core interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Include
 * @e \$Revision: 313706 $   @e \$Date: 2015-02-25 21:00:43 -0600 (Wed, 25 Feb 2015) $
 */


#ifndef _IVRS_DEVICE_INFO_H_
#define _IVRS_DEVICE_INFO_H_

// AGESA Types and Definitions

// --------------------------------------------------------------------------------------------------
/// IVHD Device List Structure
typedef struct {
  UINT16   DeviceId;           ///< DeviceID
  UINT8    DataSetting;        ///< Data settings
  UINT8    HardwareId[9];      ///< ACPI Hardware ID
  UINT8    CompatibleId[9];    ///< ACPI Compatible ID
  UINT8    UidFormat;          ///< Unique ID Format
  UINT8    UidLength;          ///< Unique ID Length
  UINT8    Uid[20];            ///< _UID Value
} IVRS_DEVICE_LIST;

#define START_OF_DEVICE_LIST IVRS_DEVICE_LIST  OemIvrsDeviceList[] = {   ///< Instantiate a structure
#define END_OF_DEVICE_LIST  {0xFFFF}};                                   ///< End of structure


// Macro for statically initializing various structures
// Simplified versions

#define  IVRS_DEVICE_NOUID(mDeviceId, mmDataSetting, mHardwareId, mCompatibleId) \
                    {mDeviceId, mmDataSetting, {mHardwareId}, {mCompatibleId}, 0, 0}                                    ///< UID string- Uses string to define uid

#define  IVRS_DEVICE_UIDSTR(mDeviceId, mmDataSetting, mHardwareId, mCompatibleId, mUidLength, mUid) \
                    {mDeviceId, mmDataSetting, {mHardwareId}, {mCompatibleId}, 2, mUidLength, {mUid}}                   ///< UID string- Uses string to define uid

#define  IVRS_DEVICE_UIDINT(mDeviceId, mmDataSetting, mHardwareId, mCompatibleId, mUid) \
                    {mDeviceId, mmDataSetting, {mHardwareId}, {mCompatibleId}, 1, 2, {mUid & 0xff, mUid >> 8, 0}}       ///< UID int=uses integer-supported by windows


// --------------------------------------------------------------------------------------------------
///IOMMU requestor ID
#ifndef _AGESA_H_
  typedef struct {
    IN       UINT16     Bus       :8;                                ///< Bus
    IN       UINT16     Device    :5;                                ///< Device
    IN       UINT16     Function  :3;                                ///< Function
  } IOMMU_REQUESTOR_ID;

/// IVMD exclusion range descriptor
  typedef struct {
  IN       UINT32               Flags;                    /**< Descriptor flags
                                                           * @li @b Flags[31] - Terminate descriptor array.
                                                           * @li @b Flags[30] - Ignore descriptor.
                                                           */
  IN       IOMMU_REQUESTOR_ID   RequestorIdStart;         ///< Requestor ID start
  IN       IOMMU_REQUESTOR_ID   RequestorIdEnd;           ///< Requestor ID end (use same as start for single ID)
  IN       UINT64               RangeBaseAddress;         ///< Phisical base address of exclusion range
  IN       UINT64               RangeLength;              ///< Length of exclusion range in bytes
} IOMMU_EXCLUSION_RANGE_DESCRIPTOR;
#endif

#define TERMINATE_IVMD          (1ul << 31)     ///< terminated ivmd
#define IGNORE_IVMD             (1ul << 30)     ///< ignore ivmd

#define START_OF_IVMD_LIST IOMMU_EXCLUSION_RANGE_DESCRIPTOR  OemIvmdList[] = {      ///< start of ivmd list
#define END_OF_IVMD_LIST  {(UINT32)TERMINATE_IVMD}};                                ///< end of ivmd list

// Macro for statically initializing various structures
// Simplified versions
#define  IVMD_ENTRY(RangeBaseAddress, RangeLength) \
                   {0, {0, 0, 0}, {0xFF, 0x1F, 7}, RangeBaseAddress, RangeLength}             ///< IVMD entry range

#define  IVMD_DEVICE(DeviceIdB, DeviceIdD, DeviceIdF, RangeBaseAddress, RangeLength) \
                    {0, {DeviceIdB, DeviceIdD, DeviceIdF}, {DeviceIdB, DeviceIdD, DeviceIdF}, RangeBaseAddress, RangeLength}        ///< ivmd device info

#define  IVMD_DEVICE_RANGE(StartIdB, StartIdD, StartIdF, EndIdB, EndIdD, EndIdF, RangeBaseAddress, RangeLength) \
                          {0, {StartIdB, StartIdD, StartIdF}, {EndIdB, EndIdD, EndIdF}, RangeBaseAddress, RangeLength}            ///< device range information

#define  IVMD_EMPTY() {IGNORE_IVMD, {0, 0, 0}, {0, 0, 0}, 0xFFFFFFFFFFFFFFFF, 0}            ///< IVMD empty


// --------------------------------------------------------------------------------------------------
// Function Prototypes
EFI_STATUS
AgesaIvrsDeviceInfo (
  IN OUT  IVRS_DEVICE_LIST   **DeviceInfo,        ///< ptr to device info
  IN      UINT8              EmmcDriverType       ///< emmc device type
  );

EFI_STATUS
AgesaIvmdListInfo (
  IN OUT  IOMMU_EXCLUSION_RANGE_DESCRIPTOR   **IvmdInfo     ///< ivmd list information
  );

#endif // _IVRS_DEVICE_INFO_H_


