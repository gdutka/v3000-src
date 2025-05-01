/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiDxe.h>
#include <Library/IvrsDeviceInfoLib.h>

#define FILECODE        NBIO_LIBRARY_IVRSDEVICEDFLTLIB_IVMDINFOTABLE_FILECODE
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

// This device list identifies exclusion ranges for PCIe devices

// Define a static IVMD entry for all devices
// IVMD_ENTRY(RangeBaseAddress, RangeLength)
// Arguments
// - StartIdB, StartIdD, StartIdF - Specifies the "start" bus, device, and function of the device(s) to which this range applies
// - EndIdB, EndIdD, EndIdF - Specifies the "end" bus, device, and function of the device(s) to which this range applies
// - RangBaseAddress - Specifies the start address of the memory exclusion range
// - RangeLength - Specifies the size of the memory exclusion range
//
// Define an exclusion range that applies to a single PCI device
// IVMD_DEVICE(DeviceIdB, DeviceIdD, DeviceIdF, RangeBaseAddress, RangeLength)
// Arguments
// - DeviceIdB, DeviceIdD, DeviceIdF - Specifies the "start" bus, device, and function of the device(s) to which this range applies
// - RangBaseAddress - Specifies the start address of the memory exclusion range
// - RangeLength - Specifies the size of the memory exclusion range
//
// Define an exclusion range that applies to multiple PCI devices
// IVMD_DEVICE_RANGE(StartIdB, StartIdD, StartIdF, EndIdB, EndIdD, EndIdF, RangeBaseAddress, RangeLength)
// Arguments
// - StartIdB, StartIdD, StartIdF - Specifies the "start" bus, device, and function of the device(s) to which this range applies
// - EndIdB, EndIdD, EndIdF - Specifies the "end" bus, device, and function of the device(s) to which this range applies
// - RangBaseAddress - Specifies the start address of the memory exclusion range
// - RangeLength - Specifies the size of the memory exclusion range
//
// Define a NULL entry that can be dynamically filled
// IVMD_EMPTY()


/*----------------------------------------------------------------------------------------
 *           D E V I C E   L I S T   I N S T A N T I A T I O N
 *----------------------------------------------------------------------------------------
 */


  START_OF_IVMD_LIST                                           // IVMD list starts with this macro
  IVMD_DEVICE (0, 0x14, 5, 0x0, 0x1000),                       // Unknown base address to be determined dynamicall
//  IVMD_ENTRY (0x10000000, 0x100),                            // Sample entry for all devices, Range at 0x10000000 with size of 0x100
//  IVMD_DEVICE_RANGE (1, 0, 0, 1, 0, 7, 0xC0000000, 0x10000), // Sample entry for Bus 1, Device 0, Function 0-7, Range at 0xC0000000 with size of 64KB
  IVMD_EMPTY (),                                               // NULL entry for dynamic determination
                                                               // Additional entries go here
  END_OF_IVMD_LIST                                             // This macro terminates the list
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
 AgesaIvmdListInfo

 @param[in][OUT]    IvmdInfo          Pointer to a pointer to hold the address of IOMMU_EXCLUSION_RANGE_DESCRIPTOR

 @retval            EFI_SUCCESS       Function returns successfully
 @retval            EFI_UNSUPPORTED   Function is not supported
*/
EFI_STATUS
AgesaIvmdListInfo (
  IN OUT  IOMMU_EXCLUSION_RANGE_DESCRIPTOR   **IvmdInfo
  )
{
  if ((OemIvmdList[0].Flags & TERMINATE_IVMD) == 0) {
    *IvmdInfo = &OemIvmdList[0];
    return EFI_SUCCESS;
  } else {
    *IvmdInfo = NULL;
    return EFI_UNSUPPORTED;
  }
}



