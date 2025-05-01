/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <Protocol/FchInitDonePolicyProtocol.h>
#include <Protocol/PciIo.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/AmdBaseLib.h>

extern EFI_GUID gAmdDefaultFbAbove4GVarGuid;

VOID                                    *mRegistrationForPciIo;
UINT8                                   OverrideIndex = 0;

typedef struct {
  UINTN                                 OverrideSegment;
  UINTN                                 OverrideBus;
  UINTN                                 OverrideDevice;
  UINTN                                 OverrideFunction;
  EFI_PCI_IO_PROTOCOL_ATTRIBUTES        mGenericAttributes;
} EFI_OVERRIDE_ATTRIBUTES;

EFI_OVERRIDE_ATTRIBUTES   OverrideAttributes [2] = {
  { 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0 }
};

#define DUAL_BOOT_TYPE     0x00
#define LEGACY_BOOT_TYPE   0x01
#define EFI_BOOT_TYPE      0x02
#define C2PMSG_97          0x3810A84
#define SBIOS_SCRATCH_0    0x10120120
#define RMB_IGPU_DEVICE_ID    0x164D          // VanGogh/Rembrandt_A0
#define RMB_IGPU2_DEVICE_ID   0x1681          // Rembrandt_B0
#define MP2_C2PMSG_22             (0x3E10558) // Addresss High
#define MP2_C2PMSG_23             (0x3E1055C) // Address Low
#define MP2_REQUEST_MEMORY_SIZE   (100*1024)

EFI_STATUS
GetFbAbove4gBackupValue (
  OUT UINT8 *DefaultAbove4G
  );

EFI_STATUS
BackupFbAbove4gValue (
  IN UINT8 DefaultAbove4G
  );

EFI_STATUS
GetApcbUmaAbove4G (
  OUT BOOLEAN          *EnableFlag
  );

EFI_STATUS
SetApcbUmaAbove4G (
  IN BOOLEAN           EnableFlag
  );

VOID
EFIAPI
AmdCpmOemInitMidNotifyEvent (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  );

VOID
EFIAPI
AmdCpmOemInitMid (
  IN      VOID             *Context
  );

VOID
EFIAPI
AmdCpmOemInitLate (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  );

BOOLEAN
EFIAPI
AmdCpmOemCheckCpuIdIsA0 (
  VOID
  );

BOOLEAN
EFIAPI
AmdCpmOemCheckSio1036 (
  VOID
  );

VOID
EFIAPI
AmdCpmHookGpuPciIo (
  IN      EFI_EVENT         Event,
  IN      VOID              *Context
  );

EFI_STATUS
GetFbAbove4gBackupValue (
  OUT UINT8   *DefaultAbove4G
)
{
  EFI_STATUS Status;
  UINTN      BufferSize;

  BufferSize = sizeof(UINT8);
  Status = gRT->GetVariable (
                  (CHAR16 *)PcdGetPtr(PcdDefaultFbAbove4GVarName),
                  &gAmdDefaultFbAbove4GVarGuid,
                  NULL,
                  &BufferSize,
                  DefaultAbove4G
                  );
  DEBUG((DEBUG_INFO, "OEM-DXE-GetFbAbove4gBackupValue Status:%r Value:%x\n", Status, *DefaultAbove4G));
  return Status;
}

EFI_STATUS
BackupFbAbove4gValue (
  IN UINT8 DefaultAbove4G
  )
{
  EFI_STATUS Status;
  UINTN      BufferSize;
  UINT8      Data;

  Status = GetFbAbove4gBackupValue(&Data);
  if (!EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-1-GetFbAbove4gBackupValue Status:%r Value:%x\n", Status, Data));
    return Status;
  }

  BufferSize = sizeof(UINT8);
  Status = gRT->SetVariable (
                  (CHAR16 *)PcdGetPtr(PcdDefaultFbAbove4GVarName),
                  &gAmdDefaultFbAbove4GVarGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  BufferSize,
                  &DefaultAbove4G
                  );
  DEBUG((DEBUG_INFO, "OEM-DXE-2-SetDefaultFbAbove4G Status:%r, Value:%x\n", Status, DefaultAbove4G));
  return Status;
}

EFI_STATUS
GetApcbUmaAbove4G (
  OUT BOOLEAN           *EnableFlag
  )
{
  EFI_STATUS                            Status;
  UINT32                                SizeInByte;
  UINT64                                Value;
  AMD_APCB_SERVICE_PROTOCOL             *mApcbDxeServiceProtocol;

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  SizeInByte = 1;
  Value = 0;
  Status = mApcbDxeServiceProtocol->ApcbGetConfigParameter (mApcbDxeServiceProtocol, APCB_ID_CONFIG_UMAABOVE4G, &SizeInByte, &Value);
  if (!EFI_ERROR (Status)) {
    *EnableFlag = (BOOLEAN)Value;
  }
  DEBUG((DEBUG_INFO, "OEM-DXE-GetApcbUmaAbove4G Status:%r\n", Status));
  return Status;
}

EFI_STATUS
SetApcbUmaAbove4G (
  IN BOOLEAN           EnableFlag
  )
{
  EFI_STATUS                            Status;
  UINT32                                SizeInByte;
  UINT64                                Value;
  AMD_APCB_SERVICE_PROTOCOL             *mApcbDxeServiceProtocol;

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  SizeInByte = 1;
  Value = 0;
  Status = mApcbDxeServiceProtocol->ApcbGetConfigParameter (mApcbDxeServiceProtocol, APCB_ID_CONFIG_UMAABOVE4G, &SizeInByte, &Value);
  DEBUG((DEBUG_INFO, "OEM-DXE-UmaAbove4GDisable CONFIG_UMAABOVE4G Get value=%x\n", Value));
  DEBUG((DEBUG_INFO, "OEM-DXE-UmaAbove4GDisable CONFIG_UMAABOVE4G Get (UINT64)EnableFlag=%x\n", (UINT64)EnableFlag));

  if (!EFI_ERROR (Status)) {
    if (Value != (UINT64)EnableFlag) {
      Value = (UINT64)EnableFlag;
      Status = mApcbDxeServiceProtocol->ApcbSetConfigParameter (mApcbDxeServiceProtocol, APCB_ID_CONFIG_UMAABOVE4G, &SizeInByte, &Value);
      DEBUG((DEBUG_INFO, "OEM-DXE-UmaAbove4GDisable CONFIG_UMAABOVE4G Set value=%x\n", Value));
      ASSERT_EFI_ERROR (Status);

      Status = mApcbDxeServiceProtocol->ApcbFlushData (mApcbDxeServiceProtocol);
      ASSERT_EFI_ERROR (Status);
      gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
  }
  return Status;
}
/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM OEM Init DXE driver
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmOemInitDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;
  AMD_CPM_TABLE_PROTOCOL                  *CpmTableProtocolPtr;
  EFI_EVENT                               AfterOemInitEnvEvent;
  VOID                                    *mAfterOemInitEnvEvent;
  UINT8                                   ApobDefaultFbAbove4G;
  EFI_EVENT                               PciIoEvent;
  UINT32                                  Data;
  UINT32                                  Data32;
  EFI_PHYSICAL_ADDRESS                    Mp2ReservedMemAddr;
  AMD_PBS_SETUP_OPTION                    AmdPbsConfiguration;
  VOID                                    *DummyProtocol = NULL;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-Start\n"));
  //
  // Initialize Global Variable
  //
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-CpmInitializeDriverLib\n"));
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-LocateProtocol-gAmdCpmTableProtocolGuid\n"));
  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status))
  {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-End-1-Status=%r\n", Status));
    return Status;
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpOemInitDxeDriverBegin);
  DEBUG((DEBUG_INFO, "OEM-DXE-%a-GetAmdPbsConfiguration\n", __FUNCTION__));
  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "OEM-DXE-%a-GetAmdPbsConfiguration-Status=%r\n", __FUNCTION__, Status));
  }

  if (AmdPbsConfiguration.Allocate100KBForMP2 == 1) {
  // Allocate memory for the MP2
    Mp2ReservedMemAddr = MAX_UINT32;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiRuntimeServicesData,
                    EFI_SIZE_TO_PAGES (MP2_REQUEST_MEMORY_SIZE),
                    &Mp2ReservedMemAddr
                    );
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "MP2 allocated 100KB success\n"));
      DEBUG ((DEBUG_INFO, "Mp2ReservedMemAddr = 0x%x\n", Mp2ReservedMemAddr));
      gBS->SetMem((VOID *)Mp2ReservedMemAddr, MP2_REQUEST_MEMORY_SIZE, 0);
      Data32 = (UINT32)((UINTN)Mp2ReservedMemAddr & 0xFFFFFFFF);
      CpmTableProtocolPtr->CommonFunction.SmnWrite32 (CpmTableProtocolPtr, 0, 0, 0, MP2_C2PMSG_23, 0, Data32);// Address Low
      Data32 = (UINT32)(RShiftU64(Mp2ReservedMemAddr, 32) & 0xFFFFFFFF);
      CpmTableProtocolPtr->CommonFunction.SmnWrite32 (CpmTableProtocolPtr, 0, 0, 0, MP2_C2PMSG_22, 0, Data32);// Address High
    }
  }

  //disable umaabove4g in legacy boot mode
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint PcdSystemBootType =%x\n", PcdGet8(PcdSystemBootType)));
  if (PcdGet8(PcdSystemBootType) != EFI_BOOT_TYPE)
  {
    if (EFI_ERROR (GetFbAbove4gBackupValue (&ApobDefaultFbAbove4G)))
    {
      Status = GetApcbUmaAbove4G (&ApobDefaultFbAbove4G);
      if (!EFI_ERROR(Status))
      {
        // CPM coverity check
        if (EFI_ERROR (BackupFbAbove4gValue (ApobDefaultFbAbove4G)))
        {
          DEBUG((DEBUG_INFO, "[%d]: Faild on BackupFbAbove4gValue (ApobDefaultFbAbove4G)\n", __LINE__ ));
          ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
        }
      }
    }
    // CPM coverity check
    if (EFI_ERROR (SetApcbUmaAbove4G (FALSE)))
    {
      DEBUG((DEBUG_INFO, "[%d]: Faild on SetApcbUmaAbove4G (FALSE)\n", __LINE__ ));
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    }
  }
  else
  {
    if (!EFI_ERROR (GetFbAbove4gBackupValue (&ApobDefaultFbAbove4G)))
    {
      // CPM coverity check
      if (EFI_ERROR (SetApcbUmaAbove4G (ApobDefaultFbAbove4G)))
      {
        DEBUG((DEBUG_INFO, "[%d]: Faild on SetApcbUmaAbove4G (ApobDefaultFbAbove4G)\n", __LINE__ ));
        ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      }
    }
  }
  //
  // First test gFchInitDonePolicyProtocolGuid is installed or not.
  //
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-LocateProtocol-gFchInitDonePolicyProtocolGuid\n"));
  Status = gBS->LocateProtocol (
                  &gFchInitDonePolicyProtocolGuid,
                  NULL,
                  &DummyProtocol
                  );
  if (EFI_ERROR (Status))
  {
    //
    // If gFchInitDonePolicyProtocolGuid still not be installed, RegisterProtocolNotify
    //
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-CreateEvent-AfterOemInitEnvEvent\n"));
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-For-AmdCpmOemInitMidNotifyEvent\n"));
    Status = gBS->CreateEvent (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmOemInitMidNotifyEvent,
                    (VOID*)CpmTableProtocolPtr,
                    &AfterOemInitEnvEvent
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-End-2-Status=%r\n", Status));
      return Status;
    }

    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-RegisterProtocolNotify-gFchInitDonePolicyProtocolGuid\n"));
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-For-AfterOemInitEnvEvent\n"));
    Status = gBS->RegisterProtocolNotify (
                    &gFchInitDonePolicyProtocolGuid,
                    AfterOemInitEnvEvent,
                    &mAfterOemInitEnvEvent
                    );
    if (EFI_ERROR (Status))
    {
      DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-End-3-Status=%r\n", Status));
      return Status;
    }
  }
  else
  {
    //
    // Already have gFchInitDonePolicyProtocolGuid, invoke AmdCpmOemInitMid()
    //
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-AmdCpmOemInitMid-2\n"));
    AmdCpmOemInitMid (CpmTableProtocolPtr);
    AmdCpmOemInitMid (CpmTableProtocolPtr);
  }

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-CreateEventEx-gEfiEventReadyToBootGuid\n"));
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-For-AmdCpmOemInitLate\n"));
  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_NOTIFY,
                  AmdCpmOemInitLate,
                  (VOID*)CpmTableProtocolPtr,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register the event handling function for PCIO install protocol
  //
  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_NOTIFY,
                  AmdCpmHookGpuPciIo,
                  NULL,
                  NULL,
                  &PciIoEvent
                  );
  if (EFI_ERROR (Status))
  {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-End-4-Status=%r\n", Status));
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
                  &gEfiPciIoProtocolGuid,
                  PciIoEvent,
                  &mRegistrationForPciIo
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-End-5-Status=%r\n", Status));
    return Status;
  }

//C2PMSG_97  (SMN address 0x3810A84)
//typedef union {
//  struct {
//    UINT32 Environment:4;                  ///< Environment: 0 - HW, 1 - SoC emulation, 2 - Cobra, 3 - GIO+USB, 4 - GC Lite.  Replaces UserDataPattern0 force
//    UINT32 CoreClNum:3;                    ///< Core CL selector: Varies from release to release.  Replaces DF scratch 30[23:0] force
//    UINT32 DisablePmuDevinitUseSwmra:1;    ///< Disable DEVINIT and use SWMRA instead. Replaces UmcCfgEco[19] force
//    UINT32 OsBackdoorLoad:1;               ///< OS backdoor load to DRAM.  New feature bit.
//    UINT32 :23;                            ///< Reserved
//  } Field;
//  UINT32  Value;
//} PRESIL_CTRL0;
//Vbios checks bit0 @  SBIOS_SCRATCH_0 ,
//if read as 0: Real HW environment (default value), Vbios will performs the full registers access.
//if read as 1: Emulation environment, Vbios will bypass the PHY registers access.
//
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-Pass-environment-to-VBIOS\n"));
  Data = (PcdGet32(PcdAmdEnvironmentFlag) & 0x1F) == 0 ? 0 : 1;
  SmnRegisterWrite(0, SBIOS_SCRATCH_0, &Data, AccessWidth32);

  CpmTableProtocolPtr->CommonFunction.PostCode (CpmTpOemInitDxeDriverEnd);

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitDxeEntryPoint-End-6-Status=%r\n", Status));
  return EFI_SUCCESS;
}

VOID
EFIAPI
AmdCpmOemInitMidNotifyEvent (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  )
{
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMidNotifyEvent-Start\n"));
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMidNotifyEvent-AmdCpmOemInitMid\n"));
  AmdCpmOemInitMid (Context);
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMidNotifyEvent-End\n"));
}

VOID
EFIAPI
AmdCpmOemInitMid (
  IN      VOID             *Context
  )
{
  STATIC BOOLEAN  AmdCpmOemInitMidInvoked = FALSE;

  if (!AmdCpmOemInitMidInvoked) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMid-Start\n"));
    if (AmdCpmOemCheckCpuIdIsA0 ()) {
      DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMid-CPU is rev:A0\n"));
      if (AmdCpmOemCheckSio1036 ()) {
        DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMid-LPC SIO SMSC card exist\n"));
        DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMid-Disable SMU FEATURE_GFX_CKS bit3 of SMU_FEATURES_ENABLE_EXT_DEFAULT\n"));
        PcdSet32S (PcdSmuFeatureControlDefinesExt, (PcdGet32 (PcdSmuFeatureControlDefinesExt) & 0xFFFFFFF7));
      }
    }
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitMid-End\n"));
  }

  AmdCpmOemInitMidInvoked = TRUE;
}

VOID
EFIAPI
AmdCpmOemInitLate (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  )
{
  STATIC BOOLEAN                   AmdCpmOemInitLateInvoked = FALSE;
  EFI_STATUS                       Status;
  AMD_CPM_TABLE_PROTOCOL           *CpmTableProtocolPtr;
  UINT8                            TempOffset;
  UINT8                            P3TLimit = 0;
  UINT8                            EcValue = 0;
  UINT8                            MpmTempOffset = 0xB6;

  if (!AmdCpmOemInitLateInvoked) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitLate-Start\n"));
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitLate-From-"));
    DEBUG((DEBUG_INFO, "AmdCpmOemInitDxeEntryPoint-CreateEventEx-gEfiEventReadyToBootGuid\n"));

    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemInitLate-End\n"));
  }
  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID**)&CpmTableProtocolPtr
                  );
  if (!EFI_ERROR (Status)) {
    P3TLimit = (UINT8)PcdGet32 (PcdP3TLimit);
    if (P3TLimit) {
      TempOffset = 0xB2;
      CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &TempOffset, &P3TLimit);
    }
  }

  // Wireless manageability EC sync
  if (PcdGetBool (PcdMpmEnable) && FeaturePcdGet (PcdMpmWirelessManageability) && PcdGetBool (PcdMpmProvisioned)) {
    CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &MpmTempOffset, &EcValue);
    //set bit4
    EcValue |= BIT4;
    CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &MpmTempOffset, &EcValue);
  } else {
    CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &MpmTempOffset, &EcValue);
    //clear bit4
    EcValue &= ~BIT4;
    CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &MpmTempOffset, &EcValue);
  }

  AmdCpmOemInitLateInvoked = TRUE;
}

BOOLEAN
EFIAPI
AmdCpmOemCheckCpuIdIsA0 (
  VOID
  )
{
  EFI_STATUS                Status;
  AMD_CPM_TABLE_PROTOCOL    *CpmTableProtocolPtr;
  UINT32                    CpuidFamilyModelStepping = 0;
  UINT32                    PkgType = 0;
  BOOLEAN                   ReturnValue = FALSE;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckCpuIdIsA0-Start\n"));
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckCpuIdIsA0-LocateProtocol-gAmdCpmTableProtocolGuid\n"));
  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckCpuIdIsA0-End-1-Status=%r Return FALSE\n", Status));
    return ReturnValue;
  }

  CpmTableProtocolPtr->CommonFunction.CpuidRead (CpmTableProtocolPtr, &CpuidFamilyModelStepping, &PkgType);

  // Check CPUID_Fn80000001_EAX Bit[7:4]: BaseModel
  if ((CpuidFamilyModelStepping & 0xF0) == 0x00) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckCpuIdIsA0-End-2-Return TRUE\n"));
    ReturnValue = TRUE;
  } else {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckCpuIdIsA0-End-2-Return FALSE\n"));
    ReturnValue = FALSE;
  }

  return ReturnValue;
}

#define LPC_SIO_SMSC_1036_INDEX     0x164E
#define LPC_SIO_SMSC_1036_DATA      0x164F
#define LPC_SIO_SMSC_1036_ID_REG    0x0D
#define LPC_SIO_SMSC_1036_ID        0x82

BOOLEAN
EFIAPI
AmdCpmOemCheckSio1036 (
  VOID
  )
{
  EFI_STATUS                Status;
  AMD_CPM_TABLE_PROTOCOL    *CpmTableProtocolPtr;
  UINT8                     Data8 = 0;
  BOOLEAN                   ReturnValue = FALSE;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckSio1036-Start\n"));
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckSio1036-LocateProtocol-gAmdCpmTableProtocolGuid\n"));
  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckSio1036-End-1-Status=%r Return FALSE\n", Status));
    return ReturnValue;
  }

  //
  // Enter Conf-mode
  //
  CpmTableProtocolPtr->CommonFunction.IoWrite8 (LPC_SIO_SMSC_1036_INDEX, 0x55);

  //
  // Check ID
  //
  CpmTableProtocolPtr->CommonFunction.IoWrite8 (LPC_SIO_SMSC_1036_INDEX, LPC_SIO_SMSC_1036_ID_REG);
  Data8 = CpmTableProtocolPtr->CommonFunction.IoRead8 (LPC_SIO_SMSC_1036_DATA);

  //
  // Exit Conf-mode
  //
  CpmTableProtocolPtr->CommonFunction.IoWrite8 (LPC_SIO_SMSC_1036_INDEX, 0xAA);

  if (Data8 == LPC_SIO_SMSC_1036_ID) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckSio1036-End-2-Return TRUE\n"));
    ReturnValue = TRUE;
  } else {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOemCheckSio1036-End-2-Return FALSE\n"));
    ReturnValue = FALSE;
  }

  return ReturnValue;
}


EFI_STATUS
AmdCpmOverrideDgpuAttributes (
  IN       EFI_PCI_IO_PROTOCOL                       *This,
  IN       EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION  Operation,
  IN       UINT64                                   Attributes,
     OUT   UINT64                                   *Result OPTIONAL
  )
{
  EFI_STATUS                Status;
  AMD_CPM_TABLE_PROTOCOL    *CpmTableProtocolPtr;
  UINT8                     GpuCommandData;
  UINTN                     OverrideSegment;
  UINTN                     OverrideBus;
  UINTN                     OverrideDevice;
  UINTN                     OverrideFunction;
  UINT8                     Index;

  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOverrideDgpuAttributes-End-1-Status=%r\n", Status));
    return Status;
  }

  Status = This->GetLocation (  This,
                                &OverrideSegment,
                                &OverrideBus,
                                &OverrideDevice,
                                &OverrideFunction );

  for (Index = 0; Index <= OverrideIndex; Index++) {
    if ((OverrideAttributes[Index].OverrideSegment   == OverrideSegment) &&
        (OverrideAttributes[Index].OverrideBus       == OverrideBus) &&
        (OverrideAttributes[Index].OverrideDevice    == OverrideDevice) &&
        (OverrideAttributes[Index].OverrideFunction  == OverrideFunction)) {
      Status = OverrideAttributes[Index].mGenericAttributes (This, Operation, Attributes, Result);

      GpuCommandData = CpmTableProtocolPtr->CommonFunction.PciRead8 (CpmTableProtocolPtr, (UINT8)OverrideBus, (UINT8)OverrideDevice, (UINT8)OverrideFunction, 0x04);
      if ((GpuCommandData != 0xFF) && ((GpuCommandData & 1) == 1)) {
        CpmTableProtocolPtr->CommonFunction.PciWrite8 (CpmTableProtocolPtr, (UINT8)OverrideBus, (UINT8)OverrideDevice, (UINT8)OverrideFunction, 0x04, (GpuCommandData & 0xFE));
        DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmOverrideDgpuAttributes-End-2-[%X:%X:%X]-DgpuCmd = %X\n", (UINT8)OverrideBus, (UINT8)OverrideDevice, (UINT8)OverrideFunction, (GpuCommandData & 0xFE)));
      }
    }
  }

  return Status;
}


BOOLEAN
AmdCpmPrimaryVideoAdaptorExist (
  IN       UINT8                                    PrimaryVideoAdaptor
  )
{
  EFI_STATUS                Status;
  AMD_CPM_TABLE_PROTOCOL    *CpmTableProtocolPtr;
  AMD_CPM_DEVICE_PATH_TABLE *DevicePathTablePtr;
  UINT8                     Index;
  BOOLEAN                   ReturnValue = FALSE;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-Start-PrimaryVideoAdaptor = %X\n", PrimaryVideoAdaptor));
  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-End-1-Status=%r\n", ReturnValue));
    return ReturnValue;
  }

  DevicePathTablePtr      = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DEVICE_PATH);

  if (DevicePathTablePtr) {
    for (Index = 0; Index < AMD_DISPLAY_DEVICE_PATH_SIZE; Index++) {
      if (DevicePathTablePtr->Path[Index].FeatureMask.Mask.Valid == 0) {
        break;
      } else if ((PrimaryVideoAdaptor == 1) && (DevicePathTablePtr->Path[Index].IsDgpu == 0)) {
        DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-Check IGPU exist or not\n"));
        if (DevicePathTablePtr->Path[Index].Bridge.Device == 0) {
          DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-Check BR IGPU\n"));
          if (CpmTableProtocolPtr->CommonFunction.PciRead8 (  CpmTableProtocolPtr,
                                                              0,
                                                              DevicePathTablePtr->Path[Index].Device.Device,
                                                              DevicePathTablePtr->Path[Index].Device.Function,
                                                              PCI_CLASSCODE_OFFSET + 2 ) == 0x03) {
            ReturnValue = TRUE;
            break;
          }
        } else {
          DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-Check RV IGPU\n"));
          if (CpmTableProtocolPtr->CommonFunction.CheckPcieVga (  CpmTableProtocolPtr,
                                                                  0,
                                                                  0,
                                                                  0,
                                                                  DevicePathTablePtr->Path[Index].Bridge.Device,
                                                                  DevicePathTablePtr->Path[Index].Bridge.Function )) {
            ReturnValue = TRUE;
            break;
          }
        }
      } else if ((PrimaryVideoAdaptor == 2) && (DevicePathTablePtr->Path[Index].IsDgpu == 1)) {
        DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-Check DGPU exist or not\n"));
        if (CpmTableProtocolPtr->CommonFunction.CheckPcieVga (  CpmTableProtocolPtr,
                                                                0,
                                                                0,
                                                                0,
                                                                DevicePathTablePtr->Path[Index].Bridge.Device,
                                                                DevicePathTablePtr->Path[Index].Bridge.Function )) {
          ReturnValue = TRUE;
          break;
        }
      }
    }
  }

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmPrimaryVideoAdaptorExist-End-2-Return = %s\n", ReturnValue ? L"TRUE" : L"FALSE"));
  return ReturnValue;
}


VOID
EFIAPI
AmdCpmHookGpuPciIo (
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                Handle;
  UINTN                     Size;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                Pci;
  AMD_PBS_SETUP_OPTION      AmdPbsConfiguration;
  UINTN                     OverrideSegment;
  UINTN                     OverrideBus;
  UINTN                     OverrideDevice;
  UINTN                     OverrideFunction;
  AMD_CPM_TABLE_PROTOCOL    *CpmTableProtocolPtr;
  UINT16                    IGPU_DID;
  UINT16                    IGPU_DID2;

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-Start\n"));

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-GetAmdPbsConfiguration\n"));
  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-End-1-Status=%r\n", Status));
    return;
  }

  Size = sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (  ByRegisterNotify,
                                NULL,
                                mRegistrationForPciIo,
                                &Size,
                                &Handle );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-End-2-Status=%r\n", Status));
    return;
  }

  //
  // Locate PCIIO protocol associated with registration
  //
  Status = gBS->HandleProtocol (  Handle,
                                  &gEfiPciIoProtocolGuid,
                                  &PciIo );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-End-3-Status=%r\n", Status));
    return;
  }

  Status = PciIo->Pci.Read (  PciIo,
                              EfiPciIoWidthUint32,
                              0,
                              sizeof (Pci) / sizeof (UINT32),
                              &Pci );

  DEBUG ((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-VID = %X DID = %X CMD = %X\n", Pci.Hdr.VendorId, Pci.Hdr.DeviceId, Pci.Hdr.Command));
  if ((!IS_PCI_VGA (&Pci)) && (!IS_PCI_OLD_VGA(&Pci)) ) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-End-4-Not-VGA-Controller\n"));
    return;
  }

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-LocateProtocol-gAmdCpmTableProtocolGuid\n"));
  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  (VOID**)&CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-End-5-Status=%r\n", Status));
    return;
  }

  IGPU_DID = (UINT16)RMB_IGPU_DEVICE_ID;
  IGPU_DID2 = (UINT16)RMB_IGPU2_DEVICE_ID;

  if ((Pci.Hdr.VendorId == 0x1002) && (Pci.Hdr.DeviceId == IGPU_DID || Pci.Hdr.DeviceId == IGPU_DID2) && (AmdPbsConfiguration.PrimaryVideoAdaptor == 2)) {
    if (AmdCpmPrimaryVideoAdaptorExist (AmdPbsConfiguration.PrimaryVideoAdaptor)) {
      Status = PciIo->GetLocation ( PciIo,
                                    &OverrideSegment,
                                    &OverrideBus,
                                    &OverrideDevice,
                                    &OverrideFunction );
      DEBUG ((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-It is IGPU [%X:%X:%X] and PrimaryVideoAdaptor is PCIE DGPU\n", (UINT8)OverrideBus, (UINT8)OverrideDevice, (UINT8)OverrideFunction));
      OverrideAttributes[OverrideIndex].OverrideSegment     = OverrideSegment;
      OverrideAttributes[OverrideIndex].OverrideBus         = OverrideBus;
      OverrideAttributes[OverrideIndex].OverrideDevice      = OverrideDevice;
      OverrideAttributes[OverrideIndex].OverrideFunction    = OverrideFunction;
      OverrideAttributes[OverrideIndex].mGenericAttributes  = PciIo->Attributes;
      OverrideIndex++;
      PciIo->Attributes = AmdCpmOverrideDgpuAttributes;
      if (OverrideIndex == 1) {     // Support up to 1 IGPU
        gBS->CloseEvent (Event);
      }
    }
  } else if ( !((Pci.Hdr.VendorId == 0x1002) && (Pci.Hdr.DeviceId == IGPU_DID || Pci.Hdr.DeviceId == IGPU_DID2)) && (AmdPbsConfiguration.PrimaryVideoAdaptor == 1)) {
    if (AmdCpmPrimaryVideoAdaptorExist (AmdPbsConfiguration.PrimaryVideoAdaptor)) {
      Status = PciIo->GetLocation ( PciIo,
                                    &OverrideSegment,
                                    &OverrideBus,
                                    &OverrideDevice,
                                    &OverrideFunction );
      DEBUG ((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-It is PCIE DGPU [%X:%X:%X] and PrimaryVideoAdaptor is IGPU\n", (UINT8)OverrideBus, (UINT8)OverrideDevice, (UINT8)OverrideFunction));
      OverrideAttributes[OverrideIndex].OverrideSegment     = OverrideSegment;
      OverrideAttributes[OverrideIndex].OverrideBus         = OverrideBus;
      OverrideAttributes[OverrideIndex].OverrideDevice      = OverrideDevice;
      OverrideAttributes[OverrideIndex].OverrideFunction    = OverrideFunction;
      OverrideAttributes[OverrideIndex].mGenericAttributes  = PciIo->Attributes;
      OverrideIndex++;
      PciIo->Attributes = AmdCpmOverrideDgpuAttributes;
      if (OverrideIndex == 2) {     // Support up to 2 DGPUs
        gBS->CloseEvent (Event);
      }
    }
  }
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdCpmHookGpuPciIo-End-6-CloseEvent\n"));
}

