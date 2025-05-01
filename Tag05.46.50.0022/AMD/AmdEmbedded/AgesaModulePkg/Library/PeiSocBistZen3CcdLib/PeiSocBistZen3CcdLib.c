/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD SOC BIST Library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 *
 */
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <AMD.h>
#include <Filecode.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#define FILECODE LIBRARY_PEISOCBISTZEN3CCDLIB_PEISOCBISTZEN3CCDLIB_FILECODE

 /*---------------------------------------------------------------------------------------*/
 /**
  *
  * Reads the status of CCX BIST
  *
  * @param[in]      SocketNum              Socket number to read BIST data from
  * @param[in]      DieNum                 Die number to read BIST data from
  * @param[in]      CcdNum                 CCD number to read BIST data from
  * @param[in]      CcxNumber              CCX number to read BIST data from
  * @param[in]      CurrSysDieCount        Current system based die number
  * @param[in]      NumberOfComplexes      Number of Complexes per die
  * @param[out]     BistData               Value of PerSrcBistPF
  * @param[in]      PeiServices            Pointer to PEI services
  *
  *
  * @retval         AGESA_ALERT            A BIST error has occurred
  * @retval         AGESA_SUCCESS          No BIST errors have occured
  */
AGESA_STATUS
ReadCcxBistData (
  IN       UINTN            SocketNum,
  IN       UINTN            DieNum,
  IN       UINTN            CcdNum,
  IN       UINTN            CcxNum,
  IN       UINTN            CurrSysDieCount,
  IN       UINTN            NumberOfComplexes,
       OUT UINTN            *BistData,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
/* @todo: sync with SMU team on the procedure
  UINT32                        PhysCcxNum;
  UINT32                        BistEn;
  UINT32                        BistEnFuse;
  UINT32                        ApobInstanceId;
  UINT8                         PhysCcdNum;
  EFI_STATUS                    CalledStatus;
  PEI_AMD_NBIO_SMU_SERVICES_PPI *NbioSmuServices;
  APOB_COMMON_SERVICE_PPI       *ApobCmnServices;


  CalledStatus = (**PeiServices).LocatePpi (
                                 PeiServices,
                                 &gAmdNbioSmuServicesPpiGuid,
                                 0,
                                 NULL,
                                 &NbioSmuServices
                                 );

  CalledStatus = (**PeiServices).LocatePpi (
                                 PeiServices,
                                 &gApobCommonServicePpiGuid,
                                 0,
                                 NULL,
                                 &ApobCmnServices
                                 );

  ASSERT (CalledStatus == EFI_SUCCESS);

  ApobInstanceId = MAKE_APOB_INSTANCE_ID (SocketNum, DieNum);
  ApobCmnServices->ApobGetPhysCcdNumber (ApobInstanceId, (UINT32) CcdNum, &PhysCcdNum);

  NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x30081854 | (PhysCcdNum << 25), &BistEnFuse);
  BistEn = (BistEnFuse >> 4) & 0x1;

  if (BistEn) {
    PhysCcxNum = (CcdNum * NumberOfComplexes) + CcxNum;
    NbioSmuServices->SmuReadBistInfo ((UINT8) SocketNum, (UINT8) PhysCcxNum, BistData);
    // Bist Pass/Fail per source. Sources are {L3, Core3, Core2, Core1, Core0}.
    // Bit will be set if BIST passed for the corresponding source or the core has been disabled.
    if (*BistData == 0) {
      return AGESA_ALERT;
    }
  }
  return AGESA_SUCCESS;
*/
  *BistData = 0;
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
 /**
  *
  * Reads the status of non-CCX BIST
  *
  * @param[in]      SocketNum              Socket number to read data from
  * @param[in]      DieNum                 Die number to read data from
  * @param[in]      CcdNum                 Ccd number to read data from
  * @param[in]      CurrSysDieCount        Current system based die number
  * @param[out]     BistData               Value of BIST status register
  * @param[in]      PeiServices            Pointer to PEI services
  *
  * @retval         AGESA_ALERT            A BIST error has occurred
  * @retval         AGESA_SUCCESS          No BIST errors have occured
  */
AGESA_STATUS
ReadNonCcxBistData (
  IN       UINTN            SocketNum,
  IN       UINTN            DieNum,
  IN       UINTN            CcdNum,
  IN       UINTN            CurrSysDieCount,
       OUT UINTN            *BistData,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
/* @todo: sync with SMU team on the procedure
  UINT8                         RunMBist;
  UINT8                         PhysCcdNum;
  UINT32                        RunMBistFuse;
  UINT32                        ApobInstanceId;
  UINTN                         TempBistData;
  UINTN                         IodBistData;
  UINTN                         CcdBistData;
  EFI_STATUS                    CalledStatus;
  PEI_AMD_NBIO_SMU_SERVICES_PPI *NbioSmuServices;
  APOB_COMMON_SERVICE_PPI       *ApobCmnServices;

  CalledStatus = (**PeiServices).LocatePpi (
                                 PeiServices,
                                 &gAmdNbioSmuServicesPpiGuid,
                                 0,
                                 NULL,
                                 &NbioSmuServices
                                 );

  ASSERT (CalledStatus == EFI_SUCCESS);

  CalledStatus = (**PeiServices).LocatePpi (
                                 PeiServices,
                                 &gApobCommonServicePpiGuid,
                                 0,
                                 NULL,
                                 &ApobCmnServices
                                 );

  ASSERT (CalledStatus == EFI_SUCCESS);

  // IOD Bist Data
  // Retrieve MP0_RUN_MBIST fuse
  NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x5D138, &RunMBistFuse);
  RunMBist = (RunMBistFuse >> 6) & 0x1;

  IodBistData = 0;
  if (RunMBist == 0x1) {
    // Retrieve IOD BIST data from MP0_C2PMSG_103 - MP0_C2PMST_98
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x03810A88, &TempBistData);
    IodBistData |= TempBistData;
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x03810A8C, &TempBistData);
    IodBistData |= TempBistData;
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x03810A90, &TempBistData);
    IodBistData |= TempBistData;
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x03810A94, &TempBistData);
    IodBistData |= TempBistData;
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x03810A98, &TempBistData);
    IodBistData |= TempBistData;
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x03810A9C, &TempBistData);
    IodBistData |= TempBistData;
  }

  // non-CCX CCD Bist Data
  // Retrieve MP5_RUN_MBIST fuse
  ApobInstanceId = MAKE_APOB_INSTANCE_ID (SocketNum, DieNum);
  ApobCmnServices->ApobGetPhysCcdNumber (ApobInstanceId, (UINT32) CcdNum, &PhysCcdNum);

  NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x30081A34 | (PhysCcdNum << 25), &RunMBistFuse);
  RunMBist = ((RunMBistFuse >> 6) & 0x1);

  CcdBistData = 0;
  if (RunMBist == 0x1) {
    // Retrieve non-CCX CCD BIST data from MP5_C2PMSG_98
    NbioSmuServices->SmuRegisterRead (NbioSmuServices, CurrSysDieCount, 0x30410A88 | (PhysCcdNum << 25), &CcdBistData);
  }

  *BistData = (IodBistData | CcdBistData);
  if (*BistData != 0) {
      return AGESA_ALERT;
  } else {
    return AGESA_SUCCESS;
  }
*/
  *BistData = 0;
  return AGESA_SUCCESS;
}
