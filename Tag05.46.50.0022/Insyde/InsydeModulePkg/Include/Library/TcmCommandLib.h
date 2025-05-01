/** @file
  This library is used by other modules to send TCM command.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
**/

#ifndef _TCM_COMMAND_LIB_H_
#define _TCM_COMMAND_LIB_H_

#include <Tcm.h>

/**
  Send Startup command to TCM.

  @param TcmSt           Startup Type.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmStartup (
  IN TCM_STARTUP_TYPE          TcmSt
  );

/**
  Send SaveState command to TCM.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmSaveState (
  VOID
  );

/**
  Send ForceClear command to TCM.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmForceClear (
  VOID
  );

#pragma pack(1)

typedef struct {
  UINT16                          sizeOfSelect;
  UINT8                           pcrSelect[3];
} TCM12_PCR_SELECTION;

typedef struct {
  TCM12_PCR_SELECTION             pcrSelection;
  TCM_LOCALITY_SELECTION          localityAtRelease;
  TCM_COMPOSITE_HASH              digestAtRelease;
} TCM12_PCR_INFO_SHORT;

typedef struct {
  TCM_STRUCTURE_TAG               tag;
  TCM_NV_INDEX                    nvIndex;
  TCM12_PCR_INFO_SHORT            pcrInfoRead;
  TCM12_PCR_INFO_SHORT            pcrInfoWrite;
  TCM_NV_ATTRIBUTES               permission;
  BOOLEAN                         bReadSTClear;
  BOOLEAN                         bWriteSTClear;
  BOOLEAN                         bWriteDefine;
  UINT32                          dataSize;
} TCM12_NV_DATA_PUBLIC;

#pragma pack()

/**
  Send NV DefineSpace command to TCM.

  @param PubInfo           The public parameters of the NV area.
  @param EncAuth           The encrypted AuthData, only valid if the attributes require subsequent authorization.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmNvDefineSpace (
  IN TCM12_NV_DATA_PUBLIC  *PubInfo,
  IN TCM_ENCAUTH           *EncAuth
  );

/**
  Send NV ReadValue command to TCM.

  @param NvIndex           The index of the area to set.
  @param Offset            The offset into the area.
  @param DataSize          The size of the data area.
  @param Data              The data to set the area to.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmNvReadValue (
  IN TCM_NV_INDEX   NvIndex,
  IN UINT32         Offset,
  IN OUT UINT32     *DataSize,
  OUT UINT8         *Data
  );

/**
  Send NV WriteValue command to TCM.

  @param NvIndex           The index of the area to set.
  @param Offset            The offset into the NV Area.
  @param DataSize          The size of the data parameter.
  @param Data              The data to set the area to.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR Unexpected device behavior.
**/
EFI_STATUS
EFIAPI
TcmNvWriteValue (
  IN TCM_NV_INDEX   NvIndex,
  IN UINT32         Offset,
  IN UINT32         DataSize,
  IN UINT8          *Data
  );

/**
  Extend a TCM PCR.

  @param[in]  DigestToExtend    The 160 bit value representing the event to be recorded.
  @param[in]  PcrIndex          The PCR to be updated.
  @param[out] NewPcrValue       New PCR value after extend.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmExtend (
  IN  TCM_DIGEST    *DigestToExtend,
  IN  TCM_PCRINDEX  PcrIndex,
  OUT TCM_DIGEST    *NewPcrValue
  );

/**
  Send TCM_ORD_PhysicalEnable command to TCM.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmPhysicalPresenceEnable (
  VOID
  );

/**
  Send TCM_ORD_PhysicalDisable command to TCM.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmPhysicalPresenceDisble (
  VOID
  );

/**
  Send TCM_ORD_PhysicalSetDeactivated command to TCM.

  @param[in] Deactivated        The state to set the TCM deactivated flag.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmPhysicalPresenceSetDeactivated (
  IN BOOLEAN  Deactivated
  );

/**
  Send TCM_ContinueSelfTest command to TCM.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmContinueSelfTest (
  VOID
  );

/**
  Get TCM capability permanent flags.

  @param[out] TcmPermanentFlags   Pointer to the buffer for returned flag structure.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityFlagPermanent (
  OUT TCM_PERMANENT_FLAGS  *TcmPermanentFlags
  );

/**
  Get TCM capability volatile flags.

  @param[out] VolatileFlags   Pointer to the buffer for returned flag structure.

  @retval EFI_SUCCESS      Operation completed successfully.
  @retval EFI_DEVICE_ERROR The command was unsuccessful.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityFlagVolatile (
  OUT TCM_STCLEAR_FLAGS                 *VolatileFlags
  );

/**
  Get TCM capability property owner flags.

  @param[out] TcmOwner   Returned owner flag.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityPropertyOwner (
  OUT BOOLEAN                 *TcmOwner
  );

/**
  Get TCM capability version value.

  @param[out] VersionInfo   Returned version info.

  @retval EFI_SUCCESS           Operation completed successfully.
  @retval EFI_TIMEOUT           The register can't run into the expected status in time.
  @retval EFI_BUFFER_TOO_SMALL  Response data buffer is too small.
  @retval EFI_DEVICE_ERROR      Unexpected device behavior.

**/
EFI_STATUS
EFIAPI
TcmGetCapabilityVersionValue (
  OUT TCM_CAP_VERSION_INFO                *VersionInfo
  );

#endif
