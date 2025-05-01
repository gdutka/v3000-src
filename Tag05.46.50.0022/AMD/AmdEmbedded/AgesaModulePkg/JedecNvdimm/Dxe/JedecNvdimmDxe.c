/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <Guid/EventGroup.h>
#include "AGESA.h"
#include "Addendum/Apcb/Inc/SSP/APOB.h"
#include <Addendum/Apcb/Inc/SSP/ApcbV3TokenUid.h>
#include <Addendum/Apcb/Inc/SSP/ApcbV3Priority.h>
#include <Library/DebugLib.h>
#include <Protocol/SmmCommunication.h>
#include "Library/IdsLib.h"
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmControl2.h>
#include "JedecNvdimmDxe.h"
#include <Include/JedecNvdimm.h>
#include <Protocol/AmdApcbProtocol.h>


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define FILECODE JEDECNVDIMM_DXE_JEDECNVDIMMDXE_FILECODE

#define JEDEC_NVDIMM_PRAITE_DATA_SIZE (OFFSET_OF (EFI_SMM_COMMUNICATE_HEADER, Data)  + sizeof (JEDEC_NVDIMM_PROTOCOL))

#define NVDIMM_CHECK_STATUS(DsmAcpiSmmData, reg, bitfield, count, v8) \
  {\
     UINT16 retrycount;\
     \
     retrycount = count;\
     v8 = CheckStatusWithRetryCount (DsmAcpiSmmData, reg, (bitfield), &retrycount);\
     DEBUG ((EFI_D_INFO, #reg##" status: 0x%02x, retrycount left = 0x%02x\n", v8, retrycount));\
   }
/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

extern  EFI_BOOT_SERVICES       *gBS;
EFI_SMM_CONTROL2_PROTOCOL *mSmmControl;
UINT8 mNvdimmPwrSrc = 0xFF;

//UINT8 mJedecNvdimmPrivateData[JEDEC_NVDIMM_PRAITE_DATA_SIZE];

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define N_MAX_DIMMS ABL_APOB_MAX_SOCKETS_SUPPORTED * ABL_APOB_MAX_CHANNELS_PER_SOCKET * ABL_APOB_MAX_DIMMS_PER_CHANNEL
#define N_MAX_DIMMS_PER_SOCKET ABL_APOB_MAX_CHANNELS_PER_SOCKET * ABL_APOB_MAX_DIMMS_PER_CHANNEL
#define N_MAX_DIMMS_PER_CHANNEL ABL_APOB_MAX_DIMMS_PER_CHANNEL
#define N_MAX_CHANNEL_PER_SOCKET ABL_APOB_MAX_CHANNELS_PER_SOCKET
#define BATTERY_CHARGING_TIMEOUT 300

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

VOID
EFIAPI
NvdimmEraseArm (
  IN EFI_EVENT        Event,
  IN VOID             *Context
);

UINT8
Dsm_I2c_Read_27 (
  DSM_ACPI_SMM_DATA *pDsmAcpiSmmData,
  IN OUT   UINT8    regOffset
  );

VOID
Dsm_I2c_Write_28 (
  DSM_ACPI_SMM_DATA *pDsmAcpiSmmData,
  IN OUT   UINT8    regOffset,
  IN OUT   UINT8    regData
  );

EFI_STATUS
SetNvdimmEsPolicy (
  IN DSM_ACPI_SMM_DATA *pDsmAcpiSmmData
 );

UINT8
CheckStatusWithRetryCount (
    IN DSM_ACPI_SMM_DATA *DsmAcpiSmmData,
    IN UINT8 Register,
    IN UINT8 BitField,
    IN OUT UINT16 *RetryCount
  );

/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */

EFI_STATUS
EFIAPI
JedecNvdimmDxe (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_EVENT Event;
  VOID *SmmCommData;

  DEBUG((EFI_D_INFO, "JedecNvdimmDxe Start\n"));

  // Allocate reserved data pool for SMM communication
  Status = gBS->AllocatePool (
            EfiReservedMemoryType, //EfiACPIMemoryNVS,
            JEDEC_NVDIMM_PRAITE_DATA_SIZE,
            &SmmCommData
           );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "JedecNvdimmDxe: Fail to allocate pool (%r)\n", Status));
    goto EndJedecNvdimmDxe;
  }

  // Create ReadToBoot event for NvdimmEraseArm
  Status = gBS->CreateEventEx (
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      NvdimmEraseArm,
      SmmCommData,
      &gEfiEventReadyToBootGuid,
      &Event
      );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "JedecNvdimmDxe: Fail to create NvdimmEraseArm ReadyToBoot event (%r)\n", Status));
  }

EndJedecNvdimmDxe:

  DEBUG((EFI_D_INFO, "JedecNvdimmDxe End\n"));

  return (Status);
}

VOID
EFIAPI
NvdimmEraseArm (
  IN EFI_EVENT        Event,
  IN VOID             *Context
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  DSM_ACPI_SMM_DATA *pDsmAcpiSmmData;
  UINT8 *JedecNvdimmSmmCommData;
  EFI_SMM_COMMUNICATE_HEADER *SmmCommBuff;
  JEDEC_NVDIMM_PROTOCOL *JedecNvdimmPrivateData;
  EFI_SMM_COMMUNICATION_PROTOCOL *SmmCommunication;
  UINTN SmmCommBufferSize;
  UINT32 gNvdimmBitMap;
  UINT8 SmiCmd;
  UINT16    socket;
  UINT16    channel;
  UINT16    dimm;
  UINT8     value8;
  UINT16    i;
  UINT32    Dsm_Status;
  UINT16    BatteryChargingTimeout;

  DEBUG((EFI_D_INFO, "NvdimmEraseArm Start\n"));

  // Close event
  gBS->CloseEvent (Event);

  JedecNvdimmSmmCommData = (UINT8 *) Context;

  //
  // Claim Dsm region
  //
  // Locate SmmCommunication protocol
  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &SmmCommunication);
  ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "NvdimmEraseArm: fail to locate SmmCommunicationProtocol (%r)\n", Status));
    goto EndNvDimmEraseArm;
  }

  // Prepare Smm communication data buffer
  ZeroMem ((VOID *)JedecNvdimmSmmCommData, JEDEC_NVDIMM_PRAITE_DATA_SIZE);
  SmmCommBuff = (EFI_SMM_COMMUNICATE_HEADER *)JedecNvdimmSmmCommData;
  CopyGuid (&SmmCommBuff->HeaderGuid, &gJedecNvdimmClaimDsmRegionGuid);
  SmmCommBuff->MessageLength = sizeof (JEDEC_NVDIMM_PROTOCOL);
  //Init JedecNvdimm SMM communicate private data
  JedecNvdimmPrivateData = (JEDEC_NVDIMM_PROTOCOL *)&SmmCommBuff->Data;
  SmmCommBufferSize = JEDEC_NVDIMM_PRAITE_DATA_SIZE;
  Status = SmmCommunication->Communicate (SmmCommunication, JedecNvdimmSmmCommData, &SmmCommBufferSize);
  //ASSERT_EFI_ERROR(Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "NvdimmEraseArm: SmmCommunication fail (%r)\n", Status));
    goto EndNvDimmEraseArm;
  }

  // Locate Smm Control protocol
  Status = gBS->LocateProtocol (&gEfiSmmControl2ProtocolGuid, NULL, (VOID **)&mSmmControl);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "NvdimmEraseArm: Fail to locate SmmCtrl Protocol (%r)\n", Status));
    goto EndNvDimmEraseArm;
  }

  // Init. data
  pDsmAcpiSmmData = JedecNvdimmPrivateData->DsmAcpiSmmData;
  gNvdimmBitMap = JedecNvdimmPrivateData->NvdimmBitMap;
  SmiCmd = JEDEC_NVDIMM_SMI_CMD;
  //DEBUG ((EFI_D_INFO, "pDsmAcpiSmmData = %x, gNvdimmBitMap = %08x\n", pDsmAcpiSmmData, gNvdimmBitMap));

  DEBUG ((EFI_D_INFO, "\nNvdimmEraseArm: NVDIMM-N Erase operation\n\n"));
  for (i = 0; i < N_MAX_DIMMS; i++) {
    if ((gNvdimmBitMap >> i) & 1) {   // NVDIMM at this DRAM slot ?
      socket = (i >= N_MAX_DIMMS_PER_SOCKET) ? 1 : 0;
      channel = i / N_MAX_DIMMS_PER_CHANNEL;
      channel = (channel >= N_MAX_CHANNEL_PER_SOCKET) ? (channel - N_MAX_CHANNEL_PER_SOCKET) : channel;
      dimm = i & 1;

      DEBUG ((EFI_D_INFO, "\tSocket %d Channel %d Dimm %d\n", socket, channel, dimm));
      pDsmAcpiSmmData->NfitDeviceHandle = (socket << 12) | (channel << 4) | dimm;
      value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, SAVE_STATUS0);
      DEBUG ((EFI_D_INFO, "\tSAVE_STATUS0 = 0x%02x\n", value8));
      if (value8 & SAVE_SUCCESS) {
        value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, CSAVE_INFO0);
        DEBUG ((EFI_D_INFO, "\tCSAVE_INFO0 = 0x%02x\n", value8));
        if (value8 & NVM_DATA_VALID) {
          value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, RESTORE_STATUS0);
          if (value8 & RESTORE_SUCCESS) {
            DEBUG ((EFI_D_INFO, "\tRESTORE_STATUS0 = 0x%02x\n", value8));
            DEBUG ((EFI_D_INFO, "\tRestore success, trigging Erase ...\n"));

            Dsm_I2c_Write_28 (pDsmAcpiSmmData, NVDIMM_MGT_CMD0, CL_ERASE_STAT);    // Clear erase status
            value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, CAPABILITIES1);
            //if (value8 & ATOMIC_ARM_AND_ERASE_SUPPORTED) {    // Atomic Arm and Erase Supported
            //  //Dsm_I2c_Write_28 (pDsmAcpiSmmData, ARM_CMD, ATOMIC_ARM_AND_ERASE);    // Trigger Atomic Arm and Erase
            //  DEBUG ((EFI_D_INFO, "\tAtomic Arm and Erase supported!\n"));
            //
            //} else {    // Does not support Atomic Arm and Erase
              // Issue DSM Erase NVM Image (Function Index 19)
              pDsmAcpiSmmData->FunctionIndex = DSM_ERASE_NVM_IMAGE;
              pDsmAcpiSmmData->InputDataSize = 0;   // Input: none
              //LibAmdIoWrite (AccessWidth8, 0xb2, &SmiCmd, NULL);
              mSmmControl->Trigger (mSmmControl, &SmiCmd, NULL, FALSE, 0);
              Dsm_Status = (pDsmAcpiSmmData->OutputData[3] << 24) | (pDsmAcpiSmmData->OutputData[2] << 16) | (pDsmAcpiSmmData->OutputData[1] << 8) | (pDsmAcpiSmmData->OutputData[0]);
              DEBUG ((EFI_D_INFO, "\tErase status = %x\n", Dsm_Status));

              if (0 == Dsm_Status) {    // Erase command success
                value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, ERASE_STATUS0);
                if (value8 & ERASE_SUCCESS) {
                  DEBUG ((EFI_D_INFO, "\tErase succes\n"));
                  value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, SAVE_STATUS0);
                  DEBUG ((EFI_D_INFO, "\tSAVE_STATUS0 = %x\n", value8));
                  value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, CSAVE_INFO0);
                  DEBUG ((EFI_D_INFO, "\tCSAVE_INFO0 = %x\n", value8));

                  // There is no way to clear NVM_Data_Valid, clear SAVE_STATUS0
                  DEBUG ((EFI_D_INFO, "NvdimmEraseArm: Clearing SAVE_STATUS0 ...\n"));
                  Dsm_I2c_Write_28 (pDsmAcpiSmmData, NVDIMM_MGT_CMD0, CL_SAVE_STAT);
                  value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, SAVE_STATUS0);
                  DEBUG ((EFI_D_INFO, "\tSAVE_STATUS0 = %x\n", value8));

                } else if (value8 & ERASE_ERROR) {
                  DEBUG ((EFI_D_ERROR, "\tErase not success & erase error\n"));

                } else {
                  DEBUG ((EFI_D_ERROR, "\tErase not success & no erase error\n"));
                }
              }
            //}   // Does not support Atomic Arm and Erase
          }
        }
      }
    }
  }

  // Arm
  DEBUG ((EFI_D_INFO, "\nNvdimmEraseArm: NVDIMM-N Arm operation\n\n"));
  for (i = 0; i < N_MAX_DIMMS; i++) {
    if ((gNvdimmBitMap >> i) & 1) {   // NVDIMM at this DRAM slot ?
      socket = (i >= N_MAX_DIMMS_PER_SOCKET) ? 1 : 0;
      channel = i / N_MAX_DIMMS_PER_CHANNEL;
      channel = (channel >= N_MAX_CHANNEL_PER_SOCKET) ? (channel - N_MAX_CHANNEL_PER_SOCKET) : channel;
      dimm = i & 1;

      DEBUG ((EFI_D_INFO, "\tSocket %d Channel %d Dimm %d\n", socket, channel, dimm));
      pDsmAcpiSmmData->NfitDeviceHandle = (socket << 12) | (channel << 4) | dimm;

      // Clear ARM status
      Dsm_I2c_Write_28 (pDsmAcpiSmmData, NVDIMM_MGT_CMD0, CL_ARM_STAT);

      DEBUG ((EFI_D_INFO, "NvdimmEraseArm: Set ES Policy ...\n"));
      Status = SetNvdimmEsPolicy (pDsmAcpiSmmData);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "NvdimmEraseArm: SetNvdimmEsPolicy fail (%r)\n", Status));
        continue;
      }

      // Check MODULE_HEALTH_STATUS1 [0] NOT_ENOUGH_ENERGY_FOR_CSAVE on DEVICE_MANAGED_POLICY_ENABLED
      value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, SET_ES_POLICY_STATUS);
      if (value8 & DEVICE_MANAGED_POLICY_ENABLED) {
        BatteryChargingTimeout = 0;
        value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, MODULE_HEALTH_STATUS1);
        if (value8 & NOT_ENOUGH_ENERGY_FOR_CSAVE) {
          gST->ConOut->OutputString (gST->ConOut, L"\n\rNvdimm-N battery charging ...\n\r");
          while (value8 & NOT_ENOUGH_ENERGY_FOR_CSAVE) {
            if (BatteryChargingTimeout >= BATTERY_CHARGING_TIMEOUT) {
              gST->ConOut->OutputString (gST->ConOut, L"\n\rNvdimm-N battery not charged in 300 seconds at Socket ");    // 180 == BATTERY_CHARGING_TIMEOUT
              switch (socket) {
              case 0:
                gST->ConOut->OutputString (gST->ConOut, L"0 DIMM Slot ");
                break;
              case 1:
                gST->ConOut->OutputString (gST->ConOut, L"1 DIMM Slot ");
                break;
              }

              switch (channel) {
              case 0:
                gST->ConOut->OutputString (gST->ConOut, L"0");
                break;
              case 1:
                gST->ConOut->OutputString (gST->ConOut, L"1");
                break;
              case 2:
                gST->ConOut->OutputString (gST->ConOut, L"2");
                break;
              case 3:
                gST->ConOut->OutputString (gST->ConOut, L"3");
                break;
              case 4:
                gST->ConOut->OutputString (gST->ConOut, L"4");
                break;
              case 5:
                gST->ConOut->OutputString (gST->ConOut, L"5");
                break;
              case 6:
                gST->ConOut->OutputString (gST->ConOut, L"6");
                break;
              case 7:
                gST->ConOut->OutputString (gST->ConOut, L"7");
                break;
              }

              switch (dimm) {
              case 0:
                gST->ConOut->OutputString (gST->ConOut, L" DIMM 0\n\r");
                break;
              case 1:
                gST->ConOut->OutputString (gST->ConOut, L" DIMM 1\n\r");
                break;
              }

              break;
            }
            gBS->Stall (1000 * 1000);    // Delay 1 sec
            BatteryChargingTimeout++;
            value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, MODULE_HEALTH_STATUS1);
          }
        }
      }

      DEBUG ((EFI_D_INFO, "NvdimmEraseArm: Trigger Arm command ...\n"));
      pDsmAcpiSmmData->FunctionIndex = DSM_ARM_NVDIMM_N;
      pDsmAcpiSmmData->InputDataSize = 0;   // Input: none
      //LibAmdIoWrite (AccessWidth8, 0xb2, &SmiCmd, NULL);
      mSmmControl->Trigger (mSmmControl, &SmiCmd, NULL, FALSE, 0);
      Dsm_Status = (pDsmAcpiSmmData->OutputData[3] << 24) | (pDsmAcpiSmmData->OutputData[2] << 16) | (pDsmAcpiSmmData->OutputData[1] << 8) | (pDsmAcpiSmmData->OutputData[0]);
      DEBUG ((EFI_D_INFO, "\tArm status = %x\n", Dsm_Status));

      if (0 == Dsm_Status) {    // Arm command success
        NVDIMM_CHECK_STATUS (pDsmAcpiSmmData, ARM_STATUS0, ARM_SUCCESS, 10, value8);
        if (value8 & ARM_SUCCESS) {
          DEBUG ((EFI_D_INFO, "\tArm success\n\n"));
        }
      }
    }
  }

EndNvDimmEraseArm:
  DEBUG((EFI_D_INFO, "NvdimmEraseArm End\n"));

}

UINT8
Dsm_I2c_Read_27 (
  DSM_ACPI_SMM_DATA *pDsmAcpiSmmData,
  IN OUT   UINT8    regOffset
  )
{
  UINT8    SmiCmd;
  UINT32    Dsm_Status;
  UINT8     retValue;

  SmiCmd = JEDEC_NVDIMM_SMI_CMD;

  pDsmAcpiSmmData->FunctionIndex = DSM_I2C_READ_27;
  pDsmAcpiSmmData->InputDataSize = 2;   // page # and register offset
  pDsmAcpiSmmData->InputData[0] = 0;    // Always set page # = 0
  pDsmAcpiSmmData->InputData[1] = regOffset;
  //LibAmdIoWrite (AccessWidth8, 0xb2, &SmiCmd, NULL);
  mSmmControl->Trigger (mSmmControl, &SmiCmd, NULL, FALSE, 0);

  Dsm_Status = (pDsmAcpiSmmData->OutputData[3] << 24) | (pDsmAcpiSmmData->OutputData[2] << 16) | (pDsmAcpiSmmData->OutputData[1] << 8) | (pDsmAcpiSmmData->OutputData[0]);
  if (0 == Dsm_Status) {
    DEBUG ((EFI_D_ERROR, "\tRead NVDIMM register %x = %02x\n", regOffset, pDsmAcpiSmmData->OutputData[4]));
    retValue = pDsmAcpiSmmData->OutputData[4];
  } else {
    DEBUG ((EFI_D_ERROR, "\tFailure on read NVDIMM register %x, Status = %08x\n", regOffset, Dsm_Status));
    retValue = 0xff;
  }

  return retValue;
}

VOID
Dsm_I2c_Write_28 (
  DSM_ACPI_SMM_DATA *pDsmAcpiSmmData,
  IN OUT   UINT8    regOffset,
  IN OUT   UINT8    regData
  )
{
  UINT8    SmiCmd;
  UINT32    Dsm_Status;

  SmiCmd = JEDEC_NVDIMM_SMI_CMD;

  pDsmAcpiSmmData->FunctionIndex = DSM_I2C_WRITE_28;
  pDsmAcpiSmmData->InputDataSize = 3;   // page #, register offset & data
  pDsmAcpiSmmData->InputData[0] = 0;    // Always set page # = 0
  pDsmAcpiSmmData->InputData[1] = regOffset;
  pDsmAcpiSmmData->InputData[2] = regData;
  //LibAmdIoWrite (AccessWidth8, 0xb2, &SmiCmd, NULL);
  mSmmControl->Trigger (mSmmControl, &SmiCmd, NULL, FALSE, 0);

  Dsm_Status = (pDsmAcpiSmmData->OutputData[3] << 24) | (pDsmAcpiSmmData->OutputData[2] << 16) | (pDsmAcpiSmmData->OutputData[1] << 8) | (pDsmAcpiSmmData->OutputData[0]);
  if (0 == Dsm_Status) {
    DEBUG ((EFI_D_INFO, "\tWrite NVDIMM register %x = %02x success\n", regOffset, regData));
  } else {
    DEBUG ((EFI_D_ERROR, "\tFailure on write NVDIMM register %x, Status = %08x\n", regOffset, Dsm_Status));
  }
}

EFI_STATUS
SetNvdimmEsPolicy (
   IN DSM_ACPI_SMM_DATA *pDsmAcpiSmmData
 )
{
  EFI_STATUS Status = EFI_SUCCESS;
  AMD_APCB_SERVICE_PROTOCOL *ApcbV3Service;
  UINT8 Purpose;
  UINT8 Value8;

  DEBUG ((EFI_D_INFO, "NvdimmEraseArm: SetNvdimmEsPolicy start\n"));

  if (mNvdimmPwrSrc == 0xFF) {
    Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, (VOID **)&ApcbV3Service);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "SetNvdimmEsPolicy: Fail to locate AmdApcbDxeServiceProtocol (%r)\n", Status));
      return Status;
    }

    Purpose = APCB_TYPE_PURPOSE_NORMAL;
    Status = ApcbV3Service->ApcbGetToken8 (ApcbV3Service, &Purpose, APCB_TOKEN_UID_MEM_NVDIMM_POWER_SOURCE, &mNvdimmPwrSrc);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "SetNvdimmEsPolicy: Apcb get token failed (%r)\n", Status));
      return Status;
    }

    DEBUG ((EFI_D_INFO, "SetNvdimmEsPolicy: NvdimmPwrSrc = 0x%02x\n", mNvdimmPwrSrc));
    if ((mNvdimmPwrSrc != 1) && (mNvdimmPwrSrc != 2)) {
      ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
      return EFI_UNSUPPORTED;
    }
  }

  // Check ES policy support capability
  Value8 = Dsm_I2c_Read_27 (pDsmAcpiSmmData, ENERGY_SOURCE_POLICY);
  DEBUG ((EFI_D_INFO, "SetNvdimmEsPolicy: Nvdimm ES Policy supported = 0x%02x\n", Value8));
  if ((Value8 & mNvdimmPwrSrc) == 0) {
    DEBUG ((EFI_D_ERROR, "SetNvdimmEsPolicy: Mismatched Apcb ES Policy selected (=0x%02x)\n", mNvdimmPwrSrc));
    return EFI_UNSUPPORTED;
  }

  gBS->Stall (10000);
  Dsm_I2c_Write_28 (pDsmAcpiSmmData, SET_ES_POLICY_CMD, mNvdimmPwrSrc);
  NVDIMM_CHECK_STATUS (pDsmAcpiSmmData, SET_ES_POLICY_STATUS,
                      (SET_ES_POLICY_SUCCESS | SET_ES_POLICY_ERROR), 10, Value8);

  DEBUG ((EFI_D_INFO, "NvdimmEraseArm: SetNvdimmEsPolicy end\n"));

  return Status;
}

UINT8
CheckStatusWithRetryCount (
    IN DSM_ACPI_SMM_DATA *DsmAcpiSmmData,
    IN UINT8 Register,
    IN UINT8 BitField,
    IN OUT UINT16 *RetryCount
  )
{
  //UINT8 RetryCount = 100;
  UINT8 Value8;

  do {
     // Delay 10 ms each time
     gBS->Stall (10000);
     Value8 = Dsm_I2c_Read_27 (DsmAcpiSmmData, Register);
     *RetryCount -= 1;
  } while (((Value8 & BitField) == 0) && (*RetryCount != 0));

  // return final reported status value
  return Value8;
}
