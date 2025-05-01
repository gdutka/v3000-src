/** @file
  Definitions for H2O check point

;******************************************************************************
;* Copyright (c) 2017, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef __H2O_CHECK_POINT_H__
#define __H2O_CHECK_POINT_H__

#include <Protocol/GraphicsOutput.h>
#include <Protocol/HiiImage.h>
#include <Protocol/OEMBadgingSupport.h>

//
// BDS CP GUID definitions
//
#define H2O_BDS_CP_BOOT_AFTER_GUID \
  { \
    0x4173b6cd, 0x5711, 0x462c, 0x9c, 0x6e, 0x1f, 0x3c, 0x95, 0x49, 0xcc, 0x9f \
  }

#define H2O_BDS_CP_BOOT_BEFORE_GUID \
  { \
    0xe7e316d5, 0xffb9, 0x4cec, 0xba, 0x82, 0x3b, 0x0, 0xe5, 0x3e, 0x12, 0xa8 \
  }

#define H2O_BDS_CP_BOOT_DEVICE_SELECT_GUID \
  { \
    0x633370f0, 0x4b8c, 0x4100, 0x84, 0xb7, 0x3b, 0x41, 0x8a, 0x5d, 0xc5, 0xba \
  }

#define H2O_BDS_CP_BOOT_FAILED_GUID \
  { \
    0xd4e5b97d, 0xb7de, 0x469b, 0xb7, 0xe3, 0x2a, 0x9c, 0x27, 0x6a, 0x11, 0xe8 \
  }

#define H2O_BDS_CP_BOOT_SUCCESS_GUID \
  { \
    0x75f1ff71, 0xd7bb, 0x4225, 0xa8, 0xc8, 0xab, 0x4c, 0xeb, 0xe6, 0x1b, 0x52 \
  }

#define H2O_BDS_CP_CONNECT_ALL_AFTER_GUID \
  { \
    0xadd30d8b, 0xda29, 0x40fe, 0x9b, 0x5e, 0x54, 0x3c, 0xda, 0xa3, 0x25, 0xe7 \
  }

#define H2O_BDS_CP_CONNECT_ALL_BEFORE_GUID \
  { \
    0x10b1ae5f, 0xf4d1, 0x4c15, 0x89, 0xd0, 0xa8, 0xc2, 0x67, 0x28, 0xf5, 0x80  \
  }

#define H2O_BDS_CP_CON_IN_CONNECT_AFTER_GUID \
  { \
    0xf32791c1, 0x6551, 0x449d, 0xbe, 0x5a, 0x99, 0xbe, 0x0, 0xe, 0xe6, 0x6b \
  }

#define H2O_BDS_CP_CON_IN_CONNECT_BEFORE_GUID \
  { \
    0x8a1a9798, 0xadfb, 0x4483, 0xa1, 0xc3, 0x29, 0x9d, 0xab, 0x99, 0x46, 0xdb \
  }

#define H2O_BDS_CP_CON_OUT_CONNECT_AFTER_GUID \
  { \
    0xd2c4a682, 0xa6ef, 0x4410, 0xb1, 0x2d, 0x58, 0xd6, 0xdd, 0xc1, 0xf0, 0xb2 \
  }

#define H2O_BDS_CP_CON_OUT_CONNECT_BEFORE_GUID \
  { \
    0x68655f09, 0x464a, 0x4642, 0xa7, 0x22, 0xd1, 0x18, 0x93, 0xbd, 0x3f, 0x7b \
  }

#define H2O_BDS_CP_DISPLAY_BEFORE_GUID \
  { \
    0xc5d30c2c, 0xd1f6, 0x401b, 0xbb, 0xd3, 0x74, 0xe6, 0xde, 0x1b, 0x4f, 0xf \
  }

#define H2O_BDS_CP_DISPLAY_STRING_BEFORE_GUID \
  { \
    0x9ba004e5, 0xf300, 0x453a, 0xb3, 0xbe, 0x3b, 0x7c, 0xeb, 0x21, 0xb7, 0x74 \
  }

#define H2O_BDS_CP_DRIVER_CONNECT_AFTER_GUID \
  { \
    0x64b0026a, 0x6886, 0x4ca3, 0xa4, 0xa8, 0x95, 0xed, 0x36, 0x68, 0x1f, 0x54 \
  }

#define H2O_BDS_CP_DRIVER_CONNECT_BEFORE_GUID \
  { \
    0x3faf7df3, 0x9b55, 0x46f8, 0xad, 0xfa, 0x12, 0x8e, 0x9d, 0x90, 0x7a, 0x1c \
  }

#define H2O_BDS_CP_END_OF_DXE_AFTER_GUID \
  { \
    0x5b02c9e2, 0x7ce3, 0x403a, 0x99, 0x7c, 0xca, 0x10, 0xd8, 0x66, 0x50, 0x7d \
  }

#define H2O_BDS_CP_END_OF_DXE_BEFORE_GUID \
  { \
    0x6adc6c22, 0x72e1, 0x410b, 0xa9, 0xf2, 0x9a, 0xc6, 0xed, 0xe9, 0xfe, 0x5e \
  }

#define H2O_BDS_CP_EXIT_BOOT_SERVICES_BEFORE_GUID \
  { \
    0x1dd55b9d, 0x3e0c, 0x41b1, 0xb0, 0xcf, 0x5b, 0xfd, 0x70, 0xb5, 0xc2, 0x12 \
  }

#define H2O_BDS_CP_INIT_GUID \
  { \
    0xd74ef78e, 0xc75b, 0x4786, 0xba, 0x79, 0xd2, 0xc2, 0xb6, 0x72, 0x52, 0x50 \
  }

#define H2O_BDS_CP_LEGACY_BOOT_AFTER_GUID \
  { \
    0xb96a8be, 0xb79b, 0x419e, 0x8b, 0x13, 0x70, 0x45, 0x37, 0xac, 0x3c, 0xdb \
  }

#define H2O_BDS_CP_LEGACY_BOOT_BEFORE_GUID \
  { \
    0xf9afcfac, 0x68bb, 0x4c3c, 0x9e, 0xe1, 0x33, 0x5a, 0x91, 0x2b, 0x30, 0xb5 \
  }

#define H2O_BDS_CP_NO_BOOT_DEVICE_GUID \
  { \
    0xde22bc1a, 0x3499, 0x4207, 0xa0, 0xde, 0x3d, 0x70, 0x6a, 0xbe, 0x5f, 0xf1 \
  }

#define H2O_BDS_CP_OS_RECOVERY_GUID \
  { \
    0x4e66c7e8, 0x08f4, 0x49b1, 0x82, 0x11, 0xb7, 0x22, 0xd7, 0x24, 0x7e, 0xac \
  }

#define H2O_BDS_CP_PLATFORM_RECOVERY_GUID \
  { \
    0x0814d9f4, 0x4ef9, 0x4c8e, 0xb1, 0x90, 0x28, 0xe5, 0x63, 0x34, 0x90, 0xf2 \
  }

#define H2O_BDS_CP_READY_TO_BOOT_AFTER_GUID \
  { \
    0xef208648, 0xb128, 0x4257, 0x9f, 0x21, 0xba, 0xaa, 0x51, 0x17, 0xef, 0xa8 \
  }

#define H2O_BDS_CP_READY_TO_BOOT_BEFORE_GUID \
  { \
    0x463ee173, 0x1485, 0x4749, 0xb7, 0x82, 0x91, 0x30, 0xdd, 0xf2, 0x2e, 0xaa \
  }

#define H2O_BDS_CP_SYSPREP_GUID \
  { \
    0x1536c670, 0x7cb6, 0x437e, 0xa8, 0xca, 0xce, 0x06, 0xcf, 0xff, 0xe6, 0x3a \
  }

//[-start-H2OCpSampleBds-add]//
#define H2O_BDS_CP_XXX_YYY_ZZZ_GUID \
  { \
    0xc9286f1d, 0x9755, 0x4eee, 0x80, 0xa9, 0x65, 0xc5, 0x01, 0x86, 0xfc, 0x15 \
  }
//[-end-H2OCpSampleBds-add]//

//[-start-H2OCpSampleDxe-add]//
#define H2O_DXE_CP_XXX_YYY_ZZZ_GUID \
  { \
    0xdbaa4d9d, 0x376c, 0x4f63, 0xa1, 0xf6, 0xbf, 0x48, 0x0f, 0x4d, 0xb9, 0xbb \
  }
//[-end-H2OCpSampleDxe-add]//

//[-start-H2OCpSamplePei-add]//
#define H2O_PEI_CP_XXX_YYY_ZZZ_GUID \
  { \
    0xf4d4610a, 0xd27f, 0x4b30, 0xb3, 0x8c, 0xe9, 0x1e, 0xb1, 0x4a, 0x95, 0xaf \
  }
//[-end-H2OCpSamplePei-add]//

//[-start-H2OCpSampleSmm-add]//
#define H2O_SMM_CP_XXX_YYY_ZZZ_GUID \
  { \
    0x79742950, 0x9422, 0x4ad6, 0x89, 0x62, 0x7c, 0xfb, 0xd4, 0x4c, 0x81, 0xe4 \
  }
//[-end-H2OCpSampleSmm-add]//

//
// BDS CP GUID definitions.
// NOTE: Below GUID defines are deprecated. Suggest using H2O_BDS_CP_XXX_YYY_GUID not Below
// H2O_BDS_CP_XXX_YYY_PROTOCOLGUID
//
#define H2O_BDS_CP_BOOT_AFTER_PROTOCOL_GUID                  H2O_BDS_CP_BOOT_AFTER_GUID
#define H2O_BDS_CP_BOOT_BEFORE_PROTOCOL_GUID                 H2O_BDS_CP_BOOT_BEFORE_GUID
#define H2O_BDS_CP_BOOT_DEVICE_SELECT_PROTOCOL_GUID          H2O_BDS_CP_BOOT_DEVICE_SELECT_GUID
#define H2O_BDS_CP_BOOT_FAILED_PROTOCOL_GUID                 H2O_BDS_CP_BOOT_FAILED_GUID
#define H2O_BDS_CP_BOOT_SUCCESS_PROTOCOL_GUID                H2O_BDS_CP_BOOT_SUCCESS_GUID
#define H2O_BDS_CP_CONNECT_ALL_AFTER_PROTOCOL_GUID           H2O_BDS_CP_CONNECT_ALL_AFTER_GUID
#define H2O_BDS_CP_CONNECT_ALL_BEFORE_PROTOCOL_GUID          H2O_BDS_CP_CONNECT_ALL_BEFORE_GUID
#define H2O_BDS_CP_CON_IN_CONNECT_AFTER_PROTOCOL_GUID        H2O_BDS_CP_CON_IN_CONNECT_AFTER_GUID
#define H2O_BDS_CP_CON_IN_CONNECT_BEFORE_PROTOCOL_GUID       H2O_BDS_CP_CON_IN_CONNECT_BEFORE_GUID
#define H2O_BDS_CP_CON_OUT_CONNECT_AFTER_PROTOCOL_GUID       H2O_BDS_CP_CON_OUT_CONNECT_AFTER_GUID
#define H2O_BDS_CP_CON_OUT_CONNECT_BEFORE_PROTOCOL_GUID      H2O_BDS_CP_CON_OUT_CONNECT_BEFORE_GUID
#define H2O_BDS_CP_DISPLAY_BEFORE_PROTOCOL_GUID              H2O_BDS_CP_DISPLAY_BEFORE_GUID
#define H2O_BDS_CP_DISPLAY_STRING_BEFORE_PROTOCOL_GUID       H2O_BDS_CP_DISPLAY_STRING_BEFORE_GUID
#define H2O_BDS_CP_DRIVER_CONNECT_AFTER_PROTOCOL_GUID        H2O_BDS_CP_DRIVER_CONNECT_AFTER_GUID
#define H2O_BDS_CP_DRIVER_CONNECT_BEFORE_PROTOCOL_GUID       H2O_BDS_CP_DRIVER_CONNECT_BEFORE_GUID
#define H2O_BDS_CP_END_OF_DXE_AFTER_PROTOCOL_GUID            H2O_BDS_CP_END_OF_DXE_AFTER_GUID
#define H2O_BDS_CP_END_OF_DXE_BEFORE_PROTOCOL_GUID           H2O_BDS_CP_END_OF_DXE_BEFORE_GUID
#define H2O_BDS_CP_EXIT_BOOT_SERVICES_BEFORE_PROTOCOL_GUID   H2O_BDS_CP_EXIT_BOOT_SERVICES_BEFORE_GUID
#define H2O_BDS_CP_INIT_PROTOCOL_GUID                        H2O_BDS_CP_INIT_GUID
#define H2O_BDS_CP_LEGACY_BOOT_AFTER_PROTOCOL_GUID           H2O_BDS_CP_LEGACY_BOOT_AFTER_GUID
#define H2O_BDS_CP_LEGACY_BOOT_BEFORE_PROTOCOL_GUID          H2O_BDS_CP_LEGACY_BOOT_BEFORE_GUID
#define H2O_BDS_CP_NO_BOOT_DEVICE_PROTOCOL_GUID              H2O_BDS_CP_NO_BOOT_DEVICE_GUID
#define H2O_BDS_CP_OS_RECOVERY_PROTOCOL_GUID                 H2O_BDS_CP_OS_RECOVERY_GUID
#define H2O_BDS_CP_PLATFORM_RECOVERY_PROTOCOL_GUID           H2O_BDS_CP_PLATFORM_RECOVERY_GUID
#define H2O_BDS_CP_READY_TO_BOOT_AFTER_PROTOCOL_GUID         H2O_BDS_CP_READY_TO_BOOT_AFTER_GUID
#define H2O_BDS_CP_READY_TO_BOOT_BEFORE_PROTOCOL_GUID        H2O_BDS_CP_READY_TO_BOOT_BEFORE_GUID
#define H2O_BDS_CP_SYSPREP_PROTOCOL_GUID                     H2O_BDS_CP_SYSPREP_GUID

//
// Task behavior for H2O_CP_XXXX_DATA structure Status member
//
#define H2O_CP_TASK_NORMAL         0x00
#define H2O_CP_TASK_SKIP           0x01
#define H2O_CP_TASK_UPDATE         0x02
#define H2O_CP_TASK_BOOT_FAIL      0x03

//
// Task behavior for H2O_BDS_CP_XXXX_DATA structure Status member
//
#define H2O_BDS_TASK_NORMAL         H2O_CP_TASK_NORMAL
#define H2O_BDS_TASK_SKIP           H2O_CP_TASK_SKIP
#define H2O_BDS_TASK_UPDATE         H2O_CP_TASK_UPDATE
#define H2O_BDS_TASK_BOOT_FAIL      H2O_CP_TASK_BOOT_FAIL

// @Prompt Structure definition for gH2OBdsCpBootAfterGuid checkpoint
typedef struct _H2O_BDS_CP_BOOT_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_BOOT_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpBootBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_BOOT_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_BOOT_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpBootDeviceSelectGuid checkpoint
typedef struct _H2O_BDS_CP_BOOT_DEVICE_SELECT_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
  LIST_ENTRY      **BootList;   /// Pointer to a pointer to the first load option in the Boot load option list.
} H2O_BDS_CP_BOOT_DEVICE_SELECT_DATA;

// @Prompt Structure definition for gH2OBdsCpBootFailedGuid checkpoint
typedef struct _H2O_BDS_CP_BOOT_FAILED_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
  EFI_STATUS      ReturnStatus; /// Status returned from failed boot.
  CHAR16          *ExitData;    /// Pointer to exit data returned from the failed boot.
  UINTN           ExitDataSize; /// Pointer to size of exit data returned from failed boot.
} H2O_BDS_CP_BOOT_FAILED_DATA;

// @Prompt Structure definition for gH2OBdsCpBootSuccessGuid checkpoint
typedef struct _H2O_BDS_CP_BOOT_SUCCESS_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_BOOT_SUCCESS_DATA;

// @Prompt Structure definition for gH2OBdsCpConnectAllAfterGuid checkpoint
typedef struct _H2O_BDS_CP_CONNECT_ALL_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_CONNECT_ALL_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpConnectAllBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_CONNECT_ALL_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_CONNECT_ALL_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpConInConnectAfterGuid checkpoint
typedef struct _H2O_BDS_CP_CON_IN_CONNECT_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_CON_IN_CONNECT_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpConInConnectBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_CON_IN_CONNECT_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_CON_IN_CONNECT_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpConOutConnectAfterGuid checkpoint
typedef struct _H2O_BDS_CP_CON_OUT_CONNECT_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_CON_OUT_CONNECT_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpConOutConnectBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_CON_OUT_CONNECT_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_CON_OUT_CONNECT_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpDisplayBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_DISPLAY_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
  UINT32          Features;     /// Bitmask that specifies the kernel's current policy concerning the primary display contents.
  EFI_IMAGE_INPUT *Image;       /// On input, this points to the read-only graphics image that the kernel intends to display on
                                /// the screen (if Features.H2O_BDS_CP_DISPLAY_LOGO is set) and place into the BGRT table
                                /// (if Features.H2O_BDS_CP_DISPLAY_BGRT is set). On output, this points to the graphics image
                                /// that the checkpoint handler will display as its final image.
} H2O_BDS_CP_DISPLAY_BEFORE_DATA;


//
// Bitmask that specifies the kernel's current policy concerning the primary display contents
//
#define H2O_BDS_CP_DISPLAY_LOGO          0x0000000000000001
#define H2O_BDS_CP_DISPLAY_INFO          0x0000000000000002
#define H2O_BDS_CP_DISPLAY_BGRT          0x0000000000000004

// @Prompt Structure definition for gH2OBdsCpDisplayStringBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_DISPLAY_STRING_BEFORE_DATA {
  UINT32                             Size;               /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32                             Status;             /// Enumerated value that specifies the kernel behavior.
  EFI_OEM_BADGING_SUPPORT_PROTOCOL   *Badging;           /// Optional pointer to an instance of the EFI_OEM_BADGING_SUPPORT_PROTOCOL.
  BOOLEAN                            AfterSelect;        /// Boolean that specifies after user presses hotkey or not.
  UINT8                              SelectedStringNum;  /// Unsigned integer that specifies the string in rows that will be displayed.
} H2O_BDS_CP_DISPLAY_STRING_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpDriverConnectAfterGuid checkpoint
typedef struct _H2O_BDS_CP_DRIVER_CONNECT_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_DRIVER_CONNECT_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpDriverConnectBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_DRIVER_CONNECT_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_DRIVER_CONNECT_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpEndOfDxeAfterGuid checkpoint
typedef struct _H2O_BDS_CP_END_OF_DXE_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_END_OF_DXE_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpEndOfDxeBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_END_OF_DXE_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_END_OF_DXE_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpExitBootServicesBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_EXIT_BOOT_SERVICES_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_EXIT_BOOT_SERVICES_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpInitGuid checkpoint
typedef struct _H2O_BDS_CP_INIT_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_INIT_DATA;

// @Prompt Structure definition for gH2OBdsCpLegacyBootAfterGuid checkpoint
typedef struct _H2O_BDS_CP_LEGACY_BOOT_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_LEGACY_BOOT_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpLegacyBootBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_LEGACY_BOOT_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_LEGACY_BOOT_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpNoBootDeviceGuid checkpoint
typedef struct _H2O_BDS_CP_NO_BOOT_DEVICE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_NO_BOOT_DEVICE_DATA;

// @Prompt Structure definition for gH2OBdsCpOsRecoveryGuid checkpoint
typedef struct _H2O_BDS_CP_OS_RECOVERY_DATA {
  UINT32          Size;              /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;            /// Enumerated value that specifies the kernel behavior.
  LIST_ENTRY      *OsRecoveryList;   /// Pointer to a pointer to the first load option in the OS Recovery load option list.
} H2O_BDS_CP_OS_RECOVERY_DATA;

// @Prompt Structure definition for gH2OBdsCpPlatformRecoveryGuid checkpoint
typedef struct _H2O_BDS_CP_PLATFORM_RECOVERY_DATA {
  UINT32          Size;                   /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;                 /// Enumerated value that specifies the kernel behavior.
  LIST_ENTRY      *PlatformRecoveryList;  /// Pointer to a pointer to the first load option in the Platform Recovery load option list.
} H2O_BDS_CP_PLATFORM_RECOVERY_DATA;

// @Prompt Structure definition for gH2OBdsCpReadyToBootAfterGuid checkpoint
typedef struct _H2O_BDS_CP_READY_TO_BOOT_AFTER_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_READY_TO_BOOT_AFTER_DATA;

// @Prompt Structure definition for gH2OBdsCpReadyToBootBeforeGuid checkpoint
typedef struct _H2O_BDS_CP_READY_TO_BOOT_BEFORE_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
} H2O_BDS_CP_READY_TO_BOOT_BEFORE_DATA;

// @Prompt Structure definition for gH2OBdsCpSysPrepGuid checkpoint
typedef struct _H2O_BDS_CP_SYSPREP_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
  LIST_ENTRY      *SysPrepList; /// Pointer to a pointer to the first load option in the System Preparation load option list.
} H2O_BDS_CP_SYSPREP_DATA;

//[-start-H2OCpSampleBds-add]//
// @Prompt Structure definition for gH2OBdsCpXxxYyyZzzGuid checkpoint
typedef struct _H2O_BDS_CP_XXX_YYY_ZZZ_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
  //
  // May have extra member or not. Below example is one UINTN member.
  //
  UINTN           TestCount;    /// The TestCount member of this example.
} H2O_BDS_CP_XXX_YYY_ZZZ_DATA;
//[-end-H2OCpSampleBds-add]//

//[-start-H2OCpSampleDxe-add]//
// @Prompt Structure definition for gH2ODxeCpXxxYyyZzzGuid checkpoint
typedef struct _H2O_DXE_CP_XXX_YYY_ZZZ_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// Enumerated value that specifies the kernel behavior.
  //
  // May have extra member or not. Below example is one UINTN member.
  //
  UINTN           TestCount;    /// The TestCount member of this example.
} H2O_DXE_CP_XXX_YYY_ZZZ_DATA;
//[-end-H2OCpSampleDxe-add]//

//[-start-H2OCpSamplePei-add]//
// @Prompt Structure definition for gH2OPeiCpXxxYyyZzzGuid checkpoint
typedef struct _H2O_PEI_CP_XXX_YYY_ZZZ_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// The TestCount member of this example.
  //
  // May have extra member or not. Below example is one UINTN member.
  //
  UINTN           TestCount;    /// The TestCount member of this example.
} H2O_PEI_CP_XXX_YYY_ZZZ_DATA;
//[-end-H2OCpSamplePei-add]//

//[-start-H2OCpSampleSmm-add]//
// @Prompt Structure definition for gH2OSmmCpXxxYyyZzzGuid checkpoint
typedef struct _H2O_SMM_CP_XXX_YYY_ZZZ_DATA {
  UINT32          Size;         /// Unsigned integer that specifies the size of this structure, in bytes.
  UINT32          Status;       /// The TestCount member of this example.
  //
  // May have extra member or not. Below example is one UINTN member.
  //
  UINTN           TestCount;    /// The TestCount member of this example.
} H2O_SMM_CP_XXX_YYY_ZZZ_DATA;
//[-end-H2OCpSampleSmm-add]//


extern EFI_GUID gH2OBdsCpBootAfterGuid;
extern EFI_GUID gH2OBdsCpBootBeforeGuid;
extern EFI_GUID gH2OBdsCpBootDeviceSelectGuid;
extern EFI_GUID gH2OBdsCpBootFailedGuid;
extern EFI_GUID gH2OBdsCpBootSuccessGuid;
extern EFI_GUID gH2OBdsCpConnectAllAfterGuid;
extern EFI_GUID gH2OBdsCpConnectAllBeforeGuid;
extern EFI_GUID gH2OBdsCpConInConnectAfterGuid;
extern EFI_GUID gH2OBdsCpConInConnectBeforeGuid;
extern EFI_GUID gH2OBdsCpConOutConnectAfterGuid;
extern EFI_GUID gH2OBdsCpConOutConnectBeforeGuid;
extern EFI_GUID gH2OBdsCpDisplayBeforeGuid;
extern EFI_GUID gH2OBdsCpDisplayStringBeforeGuid;
extern EFI_GUID gH2OBdsCpDriverConnectAfterGuid;
extern EFI_GUID gH2OBdsCpDriverConnectBeforeGuid;
extern EFI_GUID gH2OBdsCpEndOfDxeAfterGuid;
extern EFI_GUID gH2OBdsCpEndOfDxeBeforeGuid;
extern EFI_GUID gH2OBdsCpExitBootServicesBeforeGuid;
extern EFI_GUID gH2OBdsCpInitGuid;
extern EFI_GUID gH2OBdsCpLegacyBootAfterGuid;
extern EFI_GUID gH2OBdsCpLegacyBootBeforeGuid;
extern EFI_GUID gH2OBdsCpNoBootDeviceGuid;
extern EFI_GUID gH2OBdsCpOsRecoveryGuid;
extern EFI_GUID gH2OBdsCpPlatformRecoveryGuid;
extern EFI_GUID gH2OBdsCpReadyToBootAfterGuid;
extern EFI_GUID gH2OBdsCpReadyToBootBeforeGuid;
extern EFI_GUID gH2OBdsCpSysPrepGuid;
//[-start-H2OCpSampleBds-add]//
extern EFI_GUID gH2OBdsCpXxxYyyZzzGuid;
//[-end-H2OCpSampleBds-add]//
//[-start-H2OCpSampleDxe-add]//
extern EFI_GUID gH2ODxeCpXxxYyyZzzGuid;
//[-end-H2OCpSampleDxe-add]//
//[-start-H2OCpSamplePei-add]//
extern EFI_GUID gH2OPeiCpXxxYyyZzzGuid;
//[-end-H2OCpSamplePei-add]//
//[-start-H2OCpSampleSmm-add]//
extern EFI_GUID gH2OSmmCpXxxYyyZzzGuid;
//[-end-H2OCpSampleSmm-add]//

#endif
