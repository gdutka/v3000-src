/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Platform Pcie Reset Control Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  AmdNbioSmuV9Pei
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */
#include <PiDxe.h>
#include <Library/IvrsDeviceInfoLib.h>

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define   EMMC_DEVICE_ID    (UINT16) ((0x13 << 3) + 1)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           D E V I C E   D E F I N I T I O N   D O C U M E N T A T I O N
 *----------------------------------------------------------------------------------------
 */

// This device list identifies ACPI non-PCIe devices that support DMAr
// The device information should match exactly with the instantiation of the devices in ACPI

// IVRS_DEVICE_NOUID(mDeviceId, mmDataSetting, mHardwareId, mCompatibleId)
// Use this macro to identify a device with no _UID

//IVRS_DEVICE_UIDSTR(mDeviceId, mmDataSetting, mHardwareId, mCompatibleId, mUidLength, mUid)
// Use this macro to identify a device with a "string" _UID
// Example:
//     IVRS_DEVICE_UIDSTR (0x1111, 0xF6, "FAKE0000", "COMP0000", 4, "UID0"),

//IVRS_DEVICE_UIDINT(mDeviceId, mmDataSetting, mHardwareId, mCompatibleId, mUid)
// Use this macro to identify a device with an integer _UID
// Example
//    IVRS_DEVICE_UIDINT (0x4444, 0xC3, "FAKE0003", "COMP0303", 3),

// Arguments
//  - mDeviceId       : DeviceID used as source by peripheral.
//  - mmDataSetting
//    -  Bit 7 Lint1Pass Identifies a device able to assert LINT1 interrupts
//    -  Bit 6 Lint0Pass Identifies a device able to assert LINT0 interrupts
//    -  Bit 5:4 SysMgt[1:0] Identifies a device able to assert System Management messages (e.g. VID/FID)
//    -  Bit 3 Reserved Reserved; must be zero.
//    -  Bit 2 NMIPass Identifies a device able to assert NMI interrupts
//    -  Bit 1 EIntPass Identifies a device able to assert ExtInt interrupts
//    -  Bit 0 INITPass Identifies a device able to assert INIT interrupts
//
//  - mHardwareId    : Defines the name of the Hardware ID (_HID) as an ASCII string (8 characters or less)
//  - mCompatibleId  : Defines the name of the Compatible ID (_CID)as an ASCII string (8 characters or less)
//                      - Use "0" if no Compatible ID is defined.
//  - mUid           : Defines the _UID value for devices that have multiple instances
//                      - For IVRS_DEVICE_UIDSTR this should be an ASCII string (8 characters or less)
//                      - For IVRS_UIDINT this should be an integer value
//  - mUidLength     : Defines the length of the ASCII string mUid (not including string terminator)

/*----------------------------------------------------------------------------------------
 *           D E V I C E   L I S T   I N S T A N T I A T I O N
 *----------------------------------------------------------------------------------------
 */

  START_OF_DEVICE_LIST                                         // Device list starts with this macro
    IVRS_DEVICE_UIDSTR (0x00A5, 0x40, "AMDI0020", 0, 9, "\\_SB.FUR0"),                      ///< Use this macro to identify a device with a "string" _UID
    IVRS_DEVICE_UIDSTR (0x00A5, 0x40, "AMDI0020", 0, 9, "\\_SB.FUR1"),                      ///< Use this macro to identify a device with a "string" _UID
    IVRS_DEVICE_UIDSTR (0x00A5, 0x40, "AMDI0020", 0, 9, "\\_SB.FUR2"),                      ///< Use this macro to identify a device with a "string" _UID
    IVRS_DEVICE_UIDSTR (0x00A5, 0x40, "AMDI0020", 0, 9, "\\_SB.FUR3"),                      ///< Use this macro to identify a device with a "string" _UID
                                                               // Additional entries go here
  END_OF_DEVICE_LIST                                           // This macro terminates the list
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 AgesaIvrsDeviceInfo

 @param[in][OUT]    DeviceInfo        Pointer to a pointer to hold the address of IVRS_DEVICE_INFO
 @param[in]         EmmcDriverType    FCH selection of eMMC driver type

 @retval            EFI_SUCCESS       Function returns successfully
 @retval            EFI_UNSUPPORTED   Function is not supported
*/
EFI_STATUS
AgesaIvrsDeviceInfo (
  IN OUT  IVRS_DEVICE_LIST   **DeviceInfo,          ///< device information ptr
  IN      UINT8              EmmcDriverType         ///< Emmc driver type
  )
{

  if (OemIvrsDeviceList[0].DeviceId != 0xFFFF) {
    *DeviceInfo = &OemIvrsDeviceList[0];
    return EFI_SUCCESS;
  } else {
    *DeviceInfo = NULL;
    return EFI_UNSUPPORTED;
  }
}



