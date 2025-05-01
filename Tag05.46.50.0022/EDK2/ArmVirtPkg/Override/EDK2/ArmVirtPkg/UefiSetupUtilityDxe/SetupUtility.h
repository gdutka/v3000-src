/** @file

;******************************************************************************
;* Copyright (c) 2022, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _UEFI_SETUP_UTILITY_H_
#define _UEFI_SETUP_UTILITY_H_

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SetupUtilityLib.h>
#include <Library/VariableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Protocol/FormBrowser2.h>
#include <Protocol/H2ODialog.h>
#include <Protocol/SetupUtilityApplication.h>
#include <Protocol/HiiConfigAccess.h>
#include <Guid/DataHubRecords.h>
#include <Guid/GlobalVariable.h>
#include <ChipsetSetupConfig.h>
#include <SetupFuncs.h>
#include <Advance/Advance.h>
#include <Power/Power.h>
#include <Guid/DebugMask.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <PostCode.h>
#include <Library/OemGraphicsLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <H2OStatusCode.h>
#include <SetupConfig.h>

//
// This is the generated header file which includes whatever needs to be exported (strings + IFR)
//
#include <SetupUtilityStrDefs.h>
#include <Protocol/NvmExpressPassthru.h>
#define bit(a)   (1 << (a))
#define HDD_SECURITY_SUPPORT bit(0)
#define HDD_SECURITY_ENABLE  bit(1)
#define HDD_SECURITY_LOCK    bit(2)
#define HDD_SECURITY_FROZEN  bit(3)

//
// Global externs
//
extern UINT8 SetupUtilityStrings[];
extern UINT32 SetupUtilityStringsTotalSize;
extern UINT32 SetupUtilityLibStringsTotalSize;
extern UINT8 AdvanceVfrBin[];
extern UINT8 PowerVfrBin[];
extern EFI_GUID  mFormSetGuid;
extern CHAR16    mVariableName[];
extern EFI_HII_HANDLE mDriverHiiHandle;

#define EFI_SETUP_UTILITY_SIGNATURE SIGNATURE_32('S','e','t','u')

#define STRING_PACK_GUID  { 0x8160a85f, 0x934d, 0x468b, {0xa2, 0x35, 0x72, 0x89, 0x59, 0x14, 0xf6, 0xfc} }
#define HAVE_CREATE_SYSTEM_HEALTH_EVENT   0xCF8F

extern UINT16   gSaveItemMapping[][2];

#define CUSTOM_BOOT_ORDER_SIZE              256
#define TIMEOUT_OF_EVENT                5000000
#define DEFAULT_LANGUAGE                "en-US"

#define RST_CNT                         0xCF9
#define V_RST_CNT_FULLRESET             0x0E

#ifndef _MRC_API_H
//
// define the graphics stolen memory
//
typedef enum {
  gms_0MB     = 0x0,  // 0MB
  gms_32MB    = 0x1,  // 32MB
  gms_64MB    = 0x2,  // 64MB
  gms_96MB    = 0x3,  // 96MB
  gms_128MB   = 0x4,  // 128MB
  gms_160MB   = 0x5,  // 160MB
  gms_192MB   = 0x6,  // 192MB
  gms_224MB   = 0x7,  // 224MB
  gms_256MB   = 0x8,  // 256MB
  gms_288MB   = 0x9,  // 288MB
  gms_320MB   = 0xA,  // 320MB
  gms_352MB   = 0xB,  // 352MB
  gms_384MB   = 0xC,  // 384MB
  gms_416MB   = 0xD,  // 416MB
  gms_448MB   = 0xE,  // 448MB
  gms_480MB   = 0xF,  // 480MB
  gms_512MB   = 0x10, // 512MB
} MRC_TGraphicsStolenSize;
#endif

typedef struct {
  UINTN                                 Signature;
  EFI_HANDLE                            Handle;
  EFI_SETUP_UTILITY_PROTOCOL            SetupUtility;
} SETUP_UTILITY_DATA;

typedef struct _NEW_PACKAGE_INFO {
  USER_INSTALL_CALLBACK_ROUTINE         CallbackRoutine;
  UINT8                                 *IfrPack;
  UINT8                                 *StringPack;
  UINT8                                 *ImagePack;
} NEW_PACKAGE_INFO;

#pragma pack(1)

typedef struct {
  UINT16 Mp;                /* Maximum Power */
  UINT8  Rsvd1;             /* Reserved as of Nvm Express 1.1 Spec */
  UINT8  Mps:1;             /* Max Power Scale */
  UINT8  Nops:1;            /* Non-Operational State */
  UINT8  Rsvd2:6;           /* Reserved as of Nvm Express 1.1 Spec */
  UINT32 Enlat;             /* Entry Latency */
  UINT32 Exlat;             /* Exit Latency */
  UINT8  Rrt:5;             /* Relative Read Throughput */
  UINT8  Rsvd3:3;           /* Reserved as of Nvm Express 1.1 Spec */
  UINT8  Rrl:5;             /* Relative Read Leatency */
  UINT8  Rsvd4:3;           /* Reserved as of Nvm Express 1.1 Spec */
  UINT8  Rwt:5;             /* Relative Write Throughput */
  UINT8  Rsvd5:3;           /* Reserved as of Nvm Express 1.1 Spec */
  UINT8  Rwl:5;             /* Relative Write Leatency */
  UINT8  Rsvd6:3;           /* Reserved as of Nvm Express 1.1 Spec */
  UINT8  Rsvd7[16];         /* Reserved as of Nvm Express 1.1 Spec */
} NVME_PSDESCRIPTOR;

//
//  Identify Controller Data
//
typedef struct {
  //
  // Controller Capabilities and Features 0-255
  //
  UINT16 Vid;                 /* PCI Vendor ID */
  UINT16 Ssvid;               /* PCI sub-system vendor ID */
  UINT8  Sn[20];              /* Product serial number */

  UINT8  Mn[40];              /* Proeduct model number */
  UINT8  Fr[8];               /* Firmware Revision */
  UINT8  Rab;                 /* Recommended Arbitration Burst */
  UINT8  Ieee_oui[3];         /* Organization Unique Identifier */
  UINT8  Cmic;                /* Multi-interface Capabilities */
  UINT8  Mdts;                /* Maximum Data Transfer Size */
  UINT8  Cntlid[2];           /* Controller ID */
  UINT8  Rsvd1[176];          /* Reserved as of Nvm Express 1.1 Spec */
  //
  // Admin Command Set Attributes
  //
  UINT16 Oacs;                /* Optional Admin Command Support */
  UINT8  Acl;                 /* Abort Command Limit */
  UINT8  Aerl;                /* Async Event Request Limit */
  UINT8  Frmw;                /* Firmware updates */
  UINT8  Lpa;                 /* Log Page Attributes */
  UINT8  Elpe;                /* Error Log Page Entries */
  UINT8  Npss;                /* Number of Power States Support */
  UINT8  Avscc;               /* Admin Vendor Specific Command Configuration */
  UINT8  Apsta;               /* Autonomous Power State Transition Attributes */
  UINT8  Rsvd2[246];          /* Reserved as of Nvm Express 1.1 Spec */
  //
  // NVM Command Set Attributes
  //
  UINT8  Sqes;                /* Submission Queue Entry Size */
  UINT8  Cqes;                /* Completion Queue Entry Size */
  UINT16 Rsvd3;               /* Reserved as of Nvm Express 1.1 Spec */
  UINT32 Nn;                  /* Number of Namespaces */
  UINT16 Oncs;                /* Optional NVM Command Support */
  UINT16 Fuses;               /* Fused Operation Support */
  UINT8  Fna;                 /* Format NVM Attributes */
  UINT8  Vwc;                 /* Volatile Write Cache */
  UINT16 Awun;                /* Atomic Write Unit Normal */
  UINT16 Awupf;               /* Atomic Write Unit Power Fail */
  UINT8  Nvscc;               /* NVM Vendor Specific Command Configuration */
  UINT8  Rsvd4;               /* Reserved as of Nvm Express 1.1 Spec */
  UINT16 Acwu;                /* Atomic Compare & Write Unit */
  UINT16 Rsvd5;               /* Reserved as of Nvm Express 1.1 Spec */
  UINT32 Sgls;                /* SGL Support  */
  UINT8  Rsvd6[164];          /* Reserved as of Nvm Express 1.1 Spec */
  //
  // I/O Command set Attributes
  //
  UINT8 Rsvd7[1344];          /* Reserved as of Nvm Express 1.1 Spec */
  //
  // Power State Descriptors
  //
  NVME_PSDESCRIPTOR PsDescriptor[32];

  UINT8  VendorData[1024];    /* Vendor specific data */
} NVME_ADMIN_CONTROLLER_DATA;

#pragma pack()

#define EFI_SETUP_UTILITY_FROM_THIS(a) CR (a, SETUP_UTILITY_DATA, SetupUtility, EFI_SETUP_UTILITY_SIGNATURE)

extern SETUP_UTILITY_BROWSER_DATA       *gSUBrowser;
extern BOOLEAN                          mFullResetFlag;


EFI_STATUS
PowerOnSecurity (
  IN  EFI_SETUP_UTILITY_PROTOCOL        *SetupUtility
  );

EFI_STATUS
InstallHiiData (
  VOID
  );

EFI_STATUS
RemoveHiiData (
  IN VOID     *StringPack,
  ...
  );

EFI_STATUS
GetSystemConfigurationVar (
  IN OUT SETUP_UTILITY_DATA                 *SetupData
  );

EFI_STATUS
InstallSetupUtilityBrowserProtocol (
  IN  EFI_SETUP_UTILITY_PROTOCOL            *This
  );

EFI_STATUS
UninstallSetupUtilityBrowserProtocol (
  VOID
  );

EFI_STATUS
HotKeyCallBack (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  EFI_BROWSER_ACTION                    Action,
  IN  EFI_QUESTION_ID                       QuestionId,
  IN  UINT8                                 Type,
  IN  EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  );

EFI_STATUS
DisplayPlatformInfo (
  IN  SETUP_UTILITY_BROWSER_DATA            *SUBrowser
  );

EFI_STATUS
UpdateMemoryInfo (
  IN  SETUP_UTILITY_BROWSER_DATA            *SUBrowser
  );

EFI_STATUS
EFIAPI
GenericExtractConfigHook (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  );

EFI_STATUS
EFIAPI
GenericExtractConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Request,
  OUT EFI_STRING                             *Progress,
  OUT EFI_STRING                             *Results
  );

EFI_STATUS
EFIAPI
GenericRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
  );

VOID
EFIAPI
SetupUtilityNotifyFn (
  IN EFI_EVENT                             Event,
  IN VOID                                  *Context
  );

EFI_STATUS
InitSetupUtilityBrowser (
  IN  EFI_SETUP_UTILITY_PROTOCOL            *This
  );

#endif
