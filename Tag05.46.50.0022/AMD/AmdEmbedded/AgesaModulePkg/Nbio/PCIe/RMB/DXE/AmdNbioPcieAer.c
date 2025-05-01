/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <PiDxe.h>
#include <GnbRegistersRMB.h>
#include <GnbDxio.h>
#include <IdsHookId.h>
#include <Guid/HobList.h>
#include <Guid/GnbPcieInfoHob.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Library/UefiLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/GnbPciLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Protocol/AmdNbioPcieAerProtocol.h>
#include <Protocol/MpService.h>
#include "AmdNbioPcieAer.h"
#include "Filecode.h"
#include "AmdNbioPcieDxe.h"


#define FILECODE NBIO_PCIE_RMB_DXE_AMDNBIOPCIEAER_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
NbioSetPcieAerFeature (
  IN       AMD_NBIO_PCIE_AER_PROTOCOL  *This,
  IN       PCIe_PORT_AER_CONFIG        *PcieAerSetting
);

EFI_STATUS
EFIAPI
NbioSetPcieAerFeatureExtended (
  IN       PCIe_PORT_AER_CONFIG_EXTENDED        *PcieAerSetting
  );

BOOLEAN
SetPcieEdpcFeature (
  PCI_ADDR PciDevice
);

BOOLEAN
SetPcieEcrcFeature (
  PCI_ADDR PciDevice
);

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
AMD_NBIO_PCIe_AER_CONFIG NbioPcieAerConfig = {
  NUMBER_OF_PORTS,
  {
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      1,          ///< PciDev
      1,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      1,          ///< PciDev
      2,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      1,          ///< PciDev
      3,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      1,          ///< PciDev
      4,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      1,          ///< PciDev
      5,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      2,          ///< PciDev
      1,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      2,          ///< PciDev
      2,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      2,          ///< PciDev
      3,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      2,          ///< PciDev
      4,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      2,          ///< PciDev
      5,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    },
    { 0xFF,       ///< PortAerEnable, 0=Disable 1=Enable 0xFF=do nothing
      2,          ///< PciDev
      6,          ///< PciFunc
      {0,0,0,0,0,0,1,0},     ///< CorrectableMask
      {0},        ///< UncorrectableMask
      {0},        ///< UncorrectableSeverity
    }
  }
};

MCA_CTL_MASK_NBIO_STRUCT          mMcaCtlMask;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
AMD_NBIO_PCIE_AER_PROTOCOL mAmdNbioPcieAerProtocol = {
  NbioSetPcieAerFeature,
  NbioSetPcieAerFeatureExtended
};

BOOLEAN
IsBlackListDevice (
  PCI_ADDR  Device
);

VEN_DEV_ID DefaultEcrcBlacklistDeviceList[] = {
  {0x8086, 0x7560},
  {0xFFFF, 0xFFFF}
};

/**
 *---------------------------------------------------------------------------------------
 *
 *  NbioPcieAerFeatureEnable
 *
 *  Description:
 *     Enable/Disable AER.
 *
 *  Parameters:
 *    @param[in]     GnbHandle
 *    @param[in]     PortAerConfig
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
NbioPcieAerFeatureEnable (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       NBIO_PCIe_PORT_AER_CONFIG  *PortAerConfig
  )
{
  PCIe_ENGINE_CONFIG       *PcieEngine;
  PCIe_WRAPPER_CONFIG      *PcieWrapper;

  EP_PCIE_ERR_CNTL_STRUCT           ErrorControlSetup;
  PCIE_CORR_ERR_MASK_STRUCT         CorrectableErrorSetup;
  PCIE_UNCORR_ERR_MASK_STRUCT       UncorrectableErrorSetup;
  PCIE_UNCORR_ERR_SEVERITY_STRUCT   UncorrectableErrorSeveritySetup;

  // IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);
  while (PcieWrapper != NULL) {
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    if (PcdGetBool (PcdCfgAEREnable) == TRUE) {

      SmnRegisterRMW (GnbHandle->Address.Address.Bus,
                      WRAP_SPACE (GnbHandle, PcieWrapper, SMN_PCIE0_PCIE_STRAP_F0_ADDRESS),
                      ~(UINT32) (PCIE_STRAP_F0_STRAP_F0_AER_EN_MASK |
                        PCIE_STRAP_F0_STRAP_F0_CPL_ABORT_ERR_EN_MASK |
                        PCIE_STRAP_F0_STRAP_F0_ECRC_CHECK_EN_MASK),
                      (1 << PCIE_STRAP_F0_STRAP_F0_AER_EN_OFFSET |
                       1 << PCIE_STRAP_F0_STRAP_F0_CPL_ABORT_ERR_EN_OFFSET |
                       1 << PCIE_STRAP_F0_STRAP_F0_ECRC_CHECK_EN_OFFSET),
                      GNB_REG_ACC_FLAG_S3SAVE);

    }
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        if ((PcieEngine->Type.Port.PortData.DeviceNumber == PortAerConfig->PciDev) &&
            (PcieEngine->Type.Port.PortData.FunctionNumber == PortAerConfig->PciFunc)) {
          IDS_HDT_CONSOLE (PCIE_MISC, "Set AER feature: Dev%d Func%d %x\n",\
                           PortAerConfig->PciDev, PortAerConfig->PciFunc, PortAerConfig->PortAerEnable);

          SmnRegisterRead(GnbHandle->Address.Address.Bus,
                        PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8,
                                       SMN_FUNC0_PCIE0_PCIE_ERR_CNTL_ADDRESS), &(ErrorControlSetup.Value));
          if (PcdGetBool (PcdCfgAEREnable) == FALSE) {
            ErrorControlSetup.Field.ERR_REPORTING_DIS = 1;
          } else {
            ErrorControlSetup.Field.ERR_REPORTING_DIS = 0;
          }
          SmnRegisterWrite(GnbHandle->Address.Address.Bus,
                        PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8,
                        SMN_FUNC0_PCIE0_PCIE_ERR_CNTL_ADDRESS), &(ErrorControlSetup.Value), GNB_REG_ACC_FLAG_S3SAVE);
          //
          // PCIe Correctable Error Mask
          //
          SmnRegisterRead(GnbHandle->Address.Address.Bus, PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8, SMN_FUNC0_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS), &(CorrectableErrorSetup.Value));

          CorrectableErrorSetup.Field.BAD_TLP_MASK = PortAerConfig->CorrectableMask.Field.BadTLPMask;
          CorrectableErrorSetup.Field.BAD_DLLP_MASK = PortAerConfig->CorrectableMask.Field.BadDLLPMask;
          CorrectableErrorSetup.Field.REPLAY_NUM_ROLLOVER_MASK =
                               PortAerConfig->CorrectableMask.Field.ReplayNumberRolloverMask;
          CorrectableErrorSetup.Field.REPLAY_TIMER_TIMEOUT_MASK =
                               PortAerConfig->CorrectableMask.Field.ReplayTimerTimeoutMask;
          CorrectableErrorSetup.Field.ADVISORY_NONFATAL_ERR_MASK =
                               PortAerConfig->CorrectableMask.Field.AdvisoryNonFatalErrorMask;

          SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                               PORT_SPACE (GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8,
                               SMN_FUNC0_PCIE0_PCIE_CORR_ERR_MASK_ADDRESS), &(CorrectableErrorSetup.Value),
                               GNB_REG_ACC_FLAG_S3SAVE);

          //
          // PCIe Uncorrectable Error Mask
          //
          SmnRegisterRead(GnbHandle->Address.Address.Bus, PORT_SPACE(GnbHandle, PcieWrapper,
                             PcieEngine->Type.Port.PortId % 8, SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS),
                             &(UncorrectableErrorSetup.Value));

          UncorrectableErrorSetup.Field.DLP_ERR_MASK =
                             PortAerConfig->UncorrectableMask.Field.DataLinkProtocolErrorMask;
          UncorrectableErrorSetup.Field.PSN_ERR_MASK = PortAerConfig->UncorrectableMask.Field.PoisonedTLPMask;
          UncorrectableErrorSetup.Field.CPL_TIMEOUT_MASK =
                             PortAerConfig->UncorrectableMask.Field.CompletionTimeoutMask;
          UncorrectableErrorSetup.Field.CPL_ABORT_ERR_MASK = PortAerConfig->UncorrectableMask.Field.CompleterAbortMask;
          UncorrectableErrorSetup.Field.UNEXP_CPL_MASK =
                             PortAerConfig->UncorrectableMask.Field.UnexpectedCompletionMask;
          UncorrectableErrorSetup.Field.MAL_TLP_MASK = PortAerConfig->UncorrectableMask.Field.MalTlpMask;
          UncorrectableErrorSetup.Field.ECRC_ERR_MASK = PortAerConfig->UncorrectableMask.Field.ECRCErrorMask;
          UncorrectableErrorSetup.Field.UNSUPP_REQ_ERR_MASK =
                             PortAerConfig->UncorrectableMask.Field.UnsupportedRequestErrorMask;
          UncorrectableErrorSetup.Field.ACS_VIOLATION_MASK = PortAerConfig->UncorrectableMask.Field.AcsViolationMask;

          SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      PORT_SPACE (GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8,
                      SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_MASK_ADDRESS),
                      &(UncorrectableErrorSetup.Value), GNB_REG_ACC_FLAG_S3SAVE);

          //
          // PCIe Uncorrectable Error Severity
          //
          SmnRegisterRead(GnbHandle->Address.Address.Bus,
                      PORT_SPACE(GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8,
                      SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS), &(UncorrectableErrorSeveritySetup.Value));

          UncorrectableErrorSeveritySetup.Field.DLP_ERR_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.DataLinkProtocolErrorSeverity;
          UncorrectableErrorSeveritySetup.Field.PSN_ERR_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.PoisonedTLPSeverity;
          UncorrectableErrorSeveritySetup.Field.CPL_TIMEOUT_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.CompletionTimeoutSeverity;
          UncorrectableErrorSeveritySetup.Field.CPL_ABORT_ERR_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.CompleterAbortSeverity;
          UncorrectableErrorSeveritySetup.Field.UNEXP_CPL_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.UnexpectedCompletionSeverity;
          UncorrectableErrorSeveritySetup.Field.MAL_TLP_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.MalTlpSeverity;
          UncorrectableErrorSeveritySetup.Field.ECRC_ERR_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.ECRCErrorSeverity;
          UncorrectableErrorSeveritySetup.Field.UNSUPP_REQ_ERR_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.UnsupportedRequestErrorSeverity;
          UncorrectableErrorSeveritySetup.Field.ACS_VIOLATION_SEVERITY =
                           PortAerConfig->UncorrectableSeverity.Field.AcsViolationSeverity;

          SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                           PORT_SPACE (GnbHandle, PcieWrapper, PcieEngine->Type.Port.PortId % 8,
                           SMN_FUNC0_PCIE0_PCIE_UNCORR_ERR_SEVERITY_ADDRESS),
                           &(UncorrectableErrorSeveritySetup.Value), GNB_REG_ACC_FLAG_S3SAVE);

          if (PcdGetBool(PcdPcieEcrcEnablement)) {
            SetPcieEcrcFeature(PcieEngine->Type.Port.Address);
          }
          if (PcdGet8(PcdAmdEdpcEnable) > 0) { //(0) Disabled; (1) Enabled; (3) Auto
            SetPcieEdpcFeature(PcieEngine->Type.Port.Address);
          }
        }
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  // IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}


/*----------------------------------------------------------------------------------------*/
/*
 *  Performs a 64 bit read from SMN
 *
 *  NOTE: For now this is a local function. 64 bit wide reads/writes to SMN are only used in the
 *  RAS code currently. If we need more acccesses like this in other code, refactor this function
 *  up to SmnAccessLib
 *
 * @param[in]  BusNumber         Bus number of D0F0 of the target die
 * @param[in]  Address           Register offset, but PortDevice
 * @param[out] Value             Return value, must be 8 bytes of space here
 * @retval     VOID
 */
VOID
SmnRegisterRead64 (
  IN       UINT32              BusNumber,
  IN       UINT32              Address,
     OUT   VOID                *Value
  )
{
  UINT32   *Destination;

  Destination = Value;

  SmnRegisterRead(BusNumber, Address, Destination);
  Destination++; // Increment pointer by 4 bytes and  read again
  SmnRegisterRead(BusNumber, Address + sizeof(UINT32), Destination);
}

/*----------------------------------------------------------------------------------------*/
/*
 *  Performs a 64 bit write to SMN
 *
 *  NOTE: For now this is a local function. 64 bit wide reads/writes to SMN are only used in the
 *  RAS code currently. If we need more acccesses like this in other code, refactor this function
 *  up to SmnAccessLib
 *
 * @param[in]  BusNumber         Bus number of D0F0 of the target die
 * @param[in]  Address           Register offset, but PortDevice
 * @param[in]  Value             The value to write. Must be 8 bytes long
 * @param[in]  Flags             Flags - BIT0 indicates S3 save/restore
 * @retval     VOID
 */
VOID
SmnRegisterWrite64 (
  IN       UINT32              BusNumber,
  IN       UINT32              Address,
  IN       VOID                *Value,
  IN       UINT32              Flags
  )
{
  UINT32   *Source;

  Source = Value;

  SmnRegisterWrite(BusNumber, Address, Source, Flags);
  Source++; // Increment pointer by 4 bytes and write again
  SmnRegisterWrite(BusNumber, Address + sizeof(UINT32), Source, Flags);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  NbioPcieAerReportingPathExt
 *
 *  Description:
 *     Sets the AER reporting path, FW first, OS or MCA to all
 *     extrernal PCIE ports
 *
 *  Parameters:
 *    @param[in]     GnbHandle
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
NbioPcieAerReportingPathExt (
  IN       GNB_HANDLE                 *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG       *PcieEngine;
  PCIe_WRAPPER_CONFIG      *PcieWrapper;

  PCIE0PortASerr_ACTION_CONTROL_STRUCT          SerrAc;
  PCIE0PortAExtFatal_ACTION_CONTROL_STRUCT      ExtFatalAc;
  PCIE0PortAExtNonFatal_ACTION_CONTROL_STRUCT   ExtNonFatalAc;
  PCIE0PortAExtCorr_ACTION_CONTROL_STRUCT       ExtCorrAc;
  UINT8                                         AerReportPcd;

  // may be set via CBS. (0) MCA reporting; (1) OS First; (2) FW First
  AerReportPcd = PcdGet8(PcdAmdPcieAerReportMechanism);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);

  //
  // Perform this action to all enabled pcie ports (PCIE[1:0]Port[H:A] for BA)
  //
  while (PcieWrapper != NULL) {
    IDS_HDT_CONSOLE(GNB_TRACE, "%a Wrapper %d\n", __FUNCTION__, PcieWrapper->WrapId);
    PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
    while (PcieEngine != NULL) {
      if (PcieLibIsEngineAllocated (PcieEngine)) {
        //
        // Serr_ACTION_CONTROL
        //
        SmnRegisterRead(GnbHandle->Address.Address.Bus,
        IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8, SMN_PCIE0PortASerr_ACTION_CONTROL_ADDRESS),
        &(SerrAc.Value));
        SerrAc.Field.SyncFlood_En = 1;
        SerrAc.Field.LinkDis_En = 1;
        SerrAc.Field.APML_ERR_En = 1;
        if (AerReportPcd == AER_PCD_OS_FIRST) {
          // Do nothing
        }
        else {
          SerrAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
        }
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                  IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                  SMN_PCIE0PortASerr_ACTION_CONTROL_ADDRESS), &(SerrAc.Value), GNB_REG_ACC_FLAG_S3SAVE);
        //
        // ExtFatal_ACTION_CONTROL
        //
        SmnRegisterRead(GnbHandle->Address.Address.Bus,
                 IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                 SMN_PCIE0PortAExtFatal_ACTION_CONTROL_ADDRESS), &(ExtFatalAc.Value));
        if (PcdGetBool(PcdAmdPcieSyncFloodOnFatal)) {
          ExtFatalAc.Field.SyncFlood_En = 1;
          ExtFatalAc.Field.LinkDis_En = 1;
          ExtFatalAc.Field.APML_ERR_En = 1;
        }
        else {
          ExtFatalAc.Field.SyncFlood_En = 0;
          ExtFatalAc.Field.LinkDis_En = 0;
          ExtFatalAc.Field.APML_ERR_En = 0;
        }
        if (AerReportPcd == AER_PCD_OS_FIRST) {
          // Do nothing
        }
        else {
          ExtFatalAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
        }
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                         IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                         SMN_PCIE0PortAExtFatal_ACTION_CONTROL_ADDRESS), &(ExtFatalAc.Value), GNB_REG_ACC_FLAG_S3SAVE);

        //
        // ExtNonFatal_ACTION_CONTROL
        //
        SmnRegisterRead(GnbHandle->Address.Address.Bus,
                         IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                         SMN_PCIE0PortAExtNonFatal_ACTION_CONTROL_ADDRESS), &(ExtNonFatalAc.Value));
        if (AerReportPcd == AER_PCD_OS_FIRST) {
          // Do nothing
        }
        else {
          ExtNonFatalAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
        }
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                       IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                       SMN_PCIE0PortAExtNonFatal_ACTION_CONTROL_ADDRESS),
                       &(ExtNonFatalAc.Value), GNB_REG_ACC_FLAG_S3SAVE);

        //
        // ExtCorr_ACTION_CONTROL
        //
        SmnRegisterRead(GnbHandle->Address.Address.Bus,
                       IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                       SMN_PCIE0PortAExtCorr_ACTION_CONTROL_ADDRESS), &(ExtCorrAc.Value));
        if (AerReportPcd == AER_PCD_OS_FIRST) {
          // Do nothing
        }
        else {
          ExtCorrAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
        }
        SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                       IOHC_SPACE (0, PcieWrapper->WrapId, PcieEngine->Type.Port.PortId % 8,
                       SMN_PCIE0PortAExtCorr_ACTION_CONTROL_ADDRESS),
                       &(ExtCorrAc.Value), GNB_REG_ACC_FLAG_S3SAVE);
      }
      PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
    }
    PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  NbioNbifAerReportingPathExt
 *
 *  Description:
 *     Sets the AER reporting path, FW first, OS or MCA to all
 *     extrernal PCIE ports
 *
 *  Parameters:
 *    @param[in]     GnbHandle
 *
 *    @retval         VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
NbioNbifAerReportingPathExt (
  IN       GNB_HANDLE                 *GnbHandle
  )
{
  NBIF1PortASerr_ACTION_CONTROL_STRUCT          SerrAc;
  NBIF1PortAExtFatal_ACTION_CONTROL_STRUCT      ExtFatalAc;
  NBIF1PortAExtNonFatal_ACTION_CONTROL_STRUCT   ExtNonFatalAc;
  NBIF1PortAExtCorr_ACTION_CONTROL_STRUCT       ExtCorrAc;
  UINT8                                         AerReportPcd;

  // may be set via CBS. (0) MCA reporting; (1) OS First; (2) FW First
  AerReportPcd = PcdGet8(PcdAmdPcieAerReportMechanism);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  //
  // Perform this action to all enabled nbif ports (NBIF[1:0]Port[C:A] for BA)
  //
  //
  // Serr_ACTION_CONTROL
  //
  SmnRegisterRead(GnbHandle->Address.Address.Bus, SMN_NBIF1PortASerr_ACTION_CONTROL_ADDRESS, &(SerrAc.Value));
  SerrAc.Field.SyncFlood_En = 1;
  SerrAc.Field.LinkDis_En = 1;
  SerrAc.Field.APML_ERR_En = 1;
  if (AerReportPcd == AER_PCD_OS_FIRST) {
    // Do nothing
  }
  else {
    SerrAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
  }
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                      SMN_NBIF1PortASerr_ACTION_CONTROL_ADDRESS, &(SerrAc.Value), GNB_REG_ACC_FLAG_S3SAVE);

  //
  // ExtFatal_ACTION_CONTROL
  //
  SmnRegisterRead(GnbHandle->Address.Address.Bus, SMN_NBIF1PortAExtFatal_ACTION_CONTROL_ADDRESS, &(ExtFatalAc.Value));
  if (PcdGetBool(PcdAmdPcieSyncFloodOnFatal)) {
    ExtFatalAc.Field.SyncFlood_En = 1;
    ExtFatalAc.Field.LinkDis_En = 1;
    ExtFatalAc.Field.APML_ERR_En = 1;
  }
  else {
    ExtFatalAc.Field.SyncFlood_En = 0;
    ExtFatalAc.Field.LinkDis_En = 0;
    ExtFatalAc.Field.APML_ERR_En = 0;
  }
  if (AerReportPcd == AER_PCD_OS_FIRST) {
    // Do nothing
  }
  else {
    ExtFatalAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
  }
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                   SMN_NBIF1PortAExtFatal_ACTION_CONTROL_ADDRESS, &(ExtFatalAc.Value), GNB_REG_ACC_FLAG_S3SAVE);

  //
  // ExtNonFatal_ACTION_CONTROL
  //
  SmnRegisterRead(GnbHandle->Address.Address.Bus,
                   SMN_NBIF1PortAExtNonFatal_ACTION_CONTROL_ADDRESS, &(ExtNonFatalAc.Value));
  if (AerReportPcd == AER_PCD_OS_FIRST) {
    // Do nothing
  }
  else {
    ExtNonFatalAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
  }
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                   SMN_NBIF1PortAExtNonFatal_ACTION_CONTROL_ADDRESS, &(ExtNonFatalAc.Value), GNB_REG_ACC_FLAG_S3SAVE);

  //
  // ExtCorr_ACTION_CONTROL
  //
  SmnRegisterRead(GnbHandle->Address.Address.Bus, SMN_NBIF1PortAExtCorr_ACTION_CONTROL_ADDRESS, &(ExtCorrAc.Value));
  if (AerReportPcd == AER_PCD_OS_FIRST) {
    // Do nothing
  }
  else {
    ExtCorrAc.Field.IntrGenSel = AER_REPORT_SMI; // AER Legacy FW First
  }
  SmnRegisterWrite (GnbHandle->Address.Address.Bus,
                     SMN_NBIF1PortAExtCorr_ACTION_CONTROL_ADDRESS, &(ExtCorrAc.Value), GNB_REG_ACC_FLAG_S3SAVE);

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  DumpPcieAerSetting
 *
 *  Description:
 *     dump Nbio PCIe each port AER feature.
 *
 *  Parameters:
 *    @param[in]     PortAerConfig
 *
 *    @retval        VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID DumpPcieAerSetting (
  IN       NBIO_PCIe_PORT_AER_CONFIG  *PortAerConfig
  )
{
  IDS_HDT_CONSOLE (PCIE_MISC, "Dev%d Func%d AER - %a\n",\
             PortAerConfig->PciDev, PortAerConfig->PciFunc, PortAerConfig->PortAerEnable == 0? "Disable":"Enable");
  IDS_HDT_CONSOLE (PCIE_MISC, "  BadTLPMask - %d\n", \
             PortAerConfig->CorrectableMask.Field.BadTLPMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  BadDLLPMask - %d\n", \
             PortAerConfig->CorrectableMask.Field.BadDLLPMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  ReplayNumberRolloverMask - %d\n", \
             PortAerConfig->CorrectableMask.Field.ReplayNumberRolloverMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  ReplayTimerTimeoutMask - %d\n", \
             PortAerConfig->CorrectableMask.Field.ReplayTimerTimeoutMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  AdvisoryNonFatalErrorMask - %d\n", \
             PortAerConfig->CorrectableMask.Field.AdvisoryNonFatalErrorMask);

  IDS_HDT_CONSOLE (PCIE_MISC, "  DataLinkProtocolErrorMask - %d\n", \
             PortAerConfig->UncorrectableMask.Field.DataLinkProtocolErrorMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  PoisonedTLPMask - %d\n", \
             PortAerConfig->UncorrectableMask.Field.PoisonedTLPMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  CompletionTimeoutMask - %d\n",\
             PortAerConfig->UncorrectableMask.Field.CompletionTimeoutMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  CompleterAbortMask - %d\n", \
             PortAerConfig->UncorrectableMask.Field.CompleterAbortMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  UnexpectedCompletionMask - %d\n",\
             PortAerConfig->UncorrectableMask.Field.UnexpectedCompletionMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  MalTlpMask - %d\n",\
             PortAerConfig->UncorrectableMask.Field.MalTlpMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  ECRCErrorMask - %d\n", PortAerConfig->UncorrectableMask.Field.ECRCErrorMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  UnsupportedRequestErrorMask - %d\n",\
             PortAerConfig->UncorrectableMask.Field.UnsupportedRequestErrorMask);
  IDS_HDT_CONSOLE (PCIE_MISC, "  AcsViolationMask - %d\n", \
             PortAerConfig->UncorrectableMask.Field.AcsViolationMask);

  IDS_HDT_CONSOLE (PCIE_MISC, "  DataLinkProtocolErrorSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.DataLinkProtocolErrorSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  PoisonedTLPSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.PoisonedTLPSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  CompletionTimeoutSeverity - %d\n",\
             PortAerConfig->UncorrectableSeverity.Field.CompletionTimeoutSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  CompleterAbortSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.CompleterAbortSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  UnexpectedCompletionSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.UnexpectedCompletionSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  MalTlpSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.MalTlpSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  ECRCErrorSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.ECRCErrorSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  UnsupportedRequestErrorSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.UnsupportedRequestErrorSeverity);
  IDS_HDT_CONSOLE (PCIE_MISC, "  AcsViolationSeverity - %d\n", \
             PortAerConfig->UncorrectableSeverity.Field.AcsViolationSeverity);
}

/**
  AerSettingsOnAllPcieDevices

    When PcdAmdPcieAerReportMechanism is set to "legacy" or "MCA",
    firmware must program the following standard PCIe registers:
    BRIDGE_CNTL[Serr_En]=1'b1 within each root port or switch
    DEVICE_CNTL[CORR_ERR_EN, NONFATAL_ERR_EN, FATAL_ERR_EN]=3'b111 within each root port, switch and endpoint device

  @return VOID   Returns nothing
**/
VOID
AerSettingsOnAllPcieDevices (
  VOID
  )
{
  UINT16    Bus, Dev, Fun;
  PCI_ADDR  Device;
  UINT16    DeviceCapabilitiesPointer;
  PCICFG_SPACE_BRIDGE_CONTROL_STRUCT    BridgeControlReg;
  PCIE_DEVICE_CTRL_REGISTER_STRUCT      DeviceControlReg;
  UINT32    DeviceId;
  UINT32    LegacyPciAccess;

  Device.AddressValue = 0;
  DeviceCapabilitiesPointer = 0;

  for (Bus = 0; Bus <= 255; Bus++) {
    for (Dev = 0; Dev < 32; Dev++) {
      for (Fun = 0; Fun <= 7; Fun++) {
        Device.Address.Bus = Bus;
        Device.Address.Device = Dev;
        Device.Address.Function = Fun;

        // read from PCI register
        LegacyPciAccess = (((UINT32)1 << 31) + (Device.Address.Register & 0xFC) +
                      (Device.Address.Function << 8) + (Device.Address.Device << 11) +  (Device.Address.Bus << 16));
        LibAmdIoWrite (AccessWidth32, IOCF8, &LegacyPciAccess, NULL);
        LibAmdIoRead (AccessWidth32, IOCFC, &DeviceId, NULL);

        // Make sure device is present
        if (DeviceId != 0xFFFFFFFF) {
          if (GnbLibPciIsBridgeDevice (Device.AddressValue, NULL)) {
            // This is a bridge
            //
            // Set Serr_En in BRIDGE_CONTROL
            // IDS_HDT_CONSOLE(MAIN_FLOW, "%a Bridge BDF = %d %d %d\n", __FUNCTION__, Bus, Dev, Fun);
            GnbLibPciRead(
              Device.AddressValue | PCICFG_SPACE_BRIDGE_CONTROL_OFFSET,
              AccessWidth16,
              &(BridgeControlReg.Value),
              NULL
              );
            BridgeControlReg.Field.SerrEnable = 1;
            GnbLibPciWrite(
              Device.AddressValue | PCICFG_SPACE_BRIDGE_CONTROL_OFFSET,
              AccessWidth16,
              &(BridgeControlReg.Value),
              NULL
              );
          } else {
            if (GnbLibPciIsPcieDevice(Device.AddressValue, NULL)) {
              // This an Endpoint Device
              // Set CORR_ERR_EN, NONFATAL_ERR_EN, FATAL_ERR_EN in DEVICE_CNTRL
              // IDS_HDT_CONSOLE(MAIN_FLOW, "  %a Device BDF = %d %d %d\n", __FUNCTION__, Bus, Dev, Fun);
              DeviceCapabilitiesPointer = GnbLibFindPciCapability (Device.AddressValue, PCIE_CAP_ID, NULL);
              if (DeviceCapabilitiesPointer != 0) {
                GnbLibPciRead (
                  Device.AddressValue | (DeviceCapabilitiesPointer + PCIE_DEVICE_CTRL_REGISTER),
                  AccessWidth16,
                  &(DeviceControlReg.Value),
                  NULL
                );
                DeviceControlReg.Field.CorrectableErrorReportingEn = 1;
                DeviceControlReg.Field.NonFatalErrorReportingEn = 1;
                DeviceControlReg.Field.FatalErrorReportingEn = 1;
                GnbLibPciWrite (
                  Device.AddressValue | (DeviceCapabilitiesPointer + PCIE_DEVICE_CTRL_REGISTER),
                  AccessWidth16,
                  &(DeviceControlReg.Value),
                  NULL
                );
              }
            }
          }
        }
      }  //Fun
    }  //Dev
  }  //Bus
}

/*----------------------------------------------------------------------------------------*/
/**
  Write MSR entry

  If this function is ever needed for other MSRs in the future make it into a library call.

**/
VOID
NbioMsrHandler (
  IN       VOID  *Void
  )
{
  MCG_CAP_STRUCT                    McgCap;
  MCA_IPID_LS_STRUCT                McaIpId;
  UINT32                            McaCtlMaskAddr;
  UINT32                            BankNumber;

  // MSR read to MCG_CAP (MSR0000_0179) to get the value of the Count field
  McgCap.Value = AsmReadMsr64 (MSR_CCD0LTHREE0CORE0THREAD0_MCG_CAP_ADDRESS);

  // For all banks
  for (BankNumber = 0; BankNumber < McgCap.Field.Count; BankNumber++)
   {
     // MSR read to MCA_IPID (MSRC000_2005 + (0x10 (bank size) * bank number))
     McaIpId.Value = AsmReadMsr64(MSR_CCD0LTHREE0CORE00MSR_MCA_IPID_LS_ADDRESS + 0x10 * BankNumber);
     // This is an NBIO bank
     if ((McaIpId.Field.HardwareID == MCA_IPID_NBIO_HardwareId) && (McaIpId.Field.McaType == MCA_IPID_NBIO_McaType))
     {
       // Program the MCA_CTL_MASK_NBIO register
       McaCtlMaskAddr = MSR_CCD0LTHREE0CORE00MSR_MCA_CTL_MASK_LS_ADDRESS + BankNumber;
       AsmWriteMsr64 (McaCtlMaskAddr, mMcaCtlMask.Value);
      }
   }
}

/*----------------------------------------------------------------------------------------*/
/**
  This function sets the MCA_CTL_MASK to the proper values based on platform configuration and the PPR defaults


**/
VOID
SetMcaCtlMask (
  )
{
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  EFI_STATUS                Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);

  // Determine MCA_CTL_MASK value and program all instances. First initialize it to zero
  mMcaCtlMask.Value = 0;
  // If syncflood enabled set the ErrEvent bit
  if (PcdGetBool (PcdAmdMaskNbioSyncFlood) == TRUE) {
    mMcaCtlMask.Field.ErrEvent = 1;
  }
  // Mask PCIe sideband if not MCA reporting
  if (PcdGet8 (PcdAmdPcieAerReportMechanism) != PCD_AERREPORTING_MCA) {
    mMcaCtlMask.Field.PCIE_Sideband = 1;
  }
  // If RasControl is MCA, leave EccParityError 0. If its legacy or disabled set it to 1.
  if (PcdGet8 (PcdAmdNbioRASControlV2) != PCD_RASCONTROL_MCA) {
    mMcaCtlMask.Field.EccParityError = 1;
  }
  // Set Egress_Poison bit if reporting through EGRESS_POISON_STATUS registers instead of MCA
  if (PcdGetBool (PcdAmdNbioPoisonConsumption)) {
    mMcaCtlMask.Field.Egress_Poison = 1;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "mMcaCtlMask = 0x%lx\n", mMcaCtlMask.Value);

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, &MpServices);
  ASSERT (!EFI_ERROR (Status));

  MpServices->StartupAllAPs (
      MpServices,
      NbioMsrHandler,
      FALSE,
      NULL,
      0,
      NULL,
      NULL
  );
  NbioMsrHandler (NULL);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  NbioPcieAerInit
 *
 *  Description:
 *     Initializer Nbio PCIe each port AER feature.
 *
 *  Parameters:
 *    @param[in]     GnbHandle
 *
 *    @retval        VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
NbioPcieAerInit (
  IN       GNB_HANDLE  *GnbHandle
  )
{
  UINT32                   Index;
  UINT8 AerReportPcd;

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Enter\n", __FUNCTION__);
  NbioPcieAerConfig.NumberOfPorts = NUMBER_OF_PORTS;
  IDS_HDT_CONSOLE (GNB_TRACE, "Socket:%x, Die:%x, Instance:%x, Number of ports in NBIO: %d\n", \
                  GnbHandle->SocketId, GnbHandle->DieNumber, GnbHandle->InstanceId, NbioPcieAerConfig.NumberOfPorts);
  IDS_HOOK (IDS_HOOK_NBIO_PCIE_AER_INIT, NULL, (VOID *)&NbioPcieAerConfig);

  //Programming PCIe Core register

  for (Index = 0; Index < NbioPcieAerConfig.NumberOfPorts; Index++) {
    // scottd RMB emulation: reduce logging
    //DumpPcieAerSetting ((NBIO_PCIe_PORT_AER_CONFIG *) &NbioPcieAerConfig.PortAerConfig[Index]);
    NbioPcieAerFeatureEnable (GnbHandle, (NBIO_PCIe_PORT_AER_CONFIG *) &NbioPcieAerConfig.PortAerConfig[Index]);
  }

  ///
  /// Setup AER report method for PCIE and NBIF
  ///
  // may be set via CBS. (0) MCA reporting; (1) OS First; (2) FW First
  AerReportPcd = PcdGet8(PcdAmdPcieAerReportMechanism);
  if (PcdGetBool(PcdCfgAEREnable) && (AerReportPcd > AER_PCD_MCA))
  {
    NbioPcieAerReportingPathExt(GnbHandle);
    NbioNbifAerReportingPathExt(GnbHandle);

  }

  SetMcaCtlMask ();

  IDS_HDT_CONSOLE (GNB_TRACE, "%a Exit\n", __FUNCTION__);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  SetPcieEcrcFeature
 *
 *  Description:
 *     Set Pcie AER ECRC feature enable.
 *
 *  Parameters:
 *    @param[in]     PciDevice
 *
 *    @retval        BOOLEAN
 *
 *---------------------------------------------------------------------------------------
 **/
BOOLEAN
SetPcieEcrcFeature (
  PCI_ADDR PciDevice
)
{
  UINT8     Bus, Dev, Fun;
  UINT16    ParentCapPtr;
  UINT16    ChildCapPtr;
  PCI_ADDR  Device;
  BOOLEAN   EcrcEnabled;
  PCIE_ADV_ERR_CAP_CNTL_STRUCT      ParentCapFlag;
  PCIE_ADV_ERR_CAP_CNTL_STRUCT      ChildCapFlag;
  PCIE_UNCORR_ERR_SEVERITY_STRUCT   UceSeverity;

  ParentCapPtr = 0;
  ChildCapPtr = 0;
  Device.AddressValue = 0;
  EcrcEnabled = FALSE;

  // Check Bridge AER Capabilities
  ParentCapFlag.Value = 0;
  ParentCapPtr = GnbLibFindPcieExtendedCapability (PciDevice.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
  if (ParentCapPtr != 0) {
    GnbLibPciRead (PciDevice.AddressValue | (ParentCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
                                 AccessWidth32, &(ParentCapFlag.Value), NULL);
  }

  // Check Ecrc Capability support
  if (ParentCapFlag.Field.ECRC_GEN_CAP && ParentCapFlag.Field.ECRC_CHECK_CAP) {
    // Get Secondary Bus
    GnbLibPciRead (PciDevice.AddressValue | PCICFG_SPACE_SECONDARY_BUS_OFFSET, AccessWidth8, &Bus, NULL);
    Device.Address.Bus = Bus;

    // Scan support ECRC endpoint device
    for (Dev = 0; Dev < 32; Dev++) {
      Device.Address.Device = Dev;

      for (Fun = 0; Fun <= 7; Fun++) {
        Device.Address.Function = Fun;

        if (GnbLibPciIsDevicePresent (Device.AddressValue, NULL)) {
          // Is Bridge ?
          if (GnbLibPciIsBridgeDevice (Device.AddressValue, NULL)) {
            EcrcEnabled = SetPcieEcrcFeature (Device);
          } else {
            // Is Endpoint Device
            ChildCapPtr = GnbLibFindPcieExtendedCapability (Device.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
            if (ChildCapPtr != 0 && !IsBlackListDevice (Device)) {
              GnbLibPciRead (Device.AddressValue |
                     (ChildCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
                                  AccessWidth32, &(ChildCapFlag.Value), NULL);

              // If Bridge & Endpoint device support than enable ECRC
              if (ChildCapFlag.Field.ECRC_GEN_CAP && ChildCapFlag.Field.ECRC_CHECK_CAP) {
                // Set Endpoint ECRC Generation & Check Enable
                ChildCapFlag.Field.ECRC_GEN_EN = 1;
                ChildCapFlag.Field.ECRC_CHECK_EN = 1;
                GnbLibPciWrite (Device.AddressValue |
                       (ChildCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
                        AccessS3SaveWidth32, &(ChildCapFlag.Value), NULL);
                IDS_HDT_CONSOLE (PCIE_MISC, "Set EndPoint ECRC Enable Bus=%02X, Dev=%02X, Fun=%02X, Reg=%03X\n", \
                    Device.Address.Bus, Device.Address.Device, Device.Address.Function,
                    (Device.AddressValue | (ChildCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET)));
                EcrcEnabled = TRUE;
                if (PcdGetBool (PcdPcieEcrcSeverityFatal)) {
                  // Set ECRC Severity as Fatal
                  GnbLibPciRead (Device.AddressValue |
                              (ChildCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
                               AccessWidth32, &(UceSeverity.Value), NULL);
                  UceSeverity.Field.ECRC_ERR_SEVERITY = 1;
                  GnbLibPciWrite (Device.AddressValue |
                        (ChildCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
                        AccessS3SaveWidth32, &(UceSeverity.Value), NULL);
                }
              }
            }
          }

          // If not multi function skip to next Device
          if (!GnbLibPciIsMultiFunctionDevice (Device.AddressValue, NULL)) {
            break;
          }
        }
      }  //Fun
    }  //Dev

    // Check if Endpoint device enable ECRC
    if (EcrcEnabled) {
      // Set Bridge ECRC Generation & Check Enable
      ParentCapFlag.Field.ECRC_GEN_EN = 1;
      ParentCapFlag.Field.ECRC_CHECK_EN = 1;
      GnbLibPciWrite (PciDevice.AddressValue |
               (ParentCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET),
                AccessS3SaveWidth32, &(ParentCapFlag.Value), NULL);
      IDS_HDT_CONSOLE (PCIE_MISC, "Set Bridget ECRC Enable Bus=%02X, Dev=%02X, Fun=%02X, Reg=%03X\n", \
          PciDevice.Address.Bus, PciDevice.Address.Device, PciDevice.Address.Function,
          (PciDevice.AddressValue | (ParentCapPtr + PCIE_EXTCAP_AER_ADVANCED_ERROR_CAPABILITIES_CNTL_OFFSET)));
      if (PcdGetBool (PcdPcieEcrcSeverityFatal)) {
        // Set ECRC Severity as Fatal
        GnbLibPciRead (PciDevice.AddressValue | (ParentCapPtr + 0x0C), AccessWidth32, &(UceSeverity.Value), NULL);
        UceSeverity.Field.ECRC_ERR_SEVERITY = 1;
        GnbLibPciWrite (PciDevice.AddressValue | (ParentCapPtr + 0x0C), AccessS3SaveWidth32, &(UceSeverity.Value), NULL);
      }
    }
  }
  return EcrcEnabled;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  SetPcieEdpcFeature
 *
 *  Description:
 *     Set Pcie EDPC feature enable on the specified PCIe
 *     bridge/switch and all downstream of it
 *
 *  Parameters:
 *    @param[in]     PciDevice  Address of PCIe device
 *
 *    @retval        BOOLEAN
 *
 *---------------------------------------------------------------------------------------
 **/
BOOLEAN
SetPcieEdpcFeature (
  PCI_ADDR PciDevice
)
{
  UINT8     Bus, Dev, Fun;
  UINT16    ParentCapPtr;
  PCI_ADDR  Device;
  BOOLEAN   EdpcEnabled;
  PCIE_DPC_ENH_CAP_LIST_STRUCT      DeviceCapFlag;
  PCIE_DPC_CNTL_STRUCT              DeviceCapCntl;

  ParentCapPtr = 0;
  Device.AddressValue = 0;
  EdpcEnabled = FALSE;

  // Check for bogus PCIe device
  if (!GnbLibPciIsDevicePresent(PciDevice.AddressValue, NULL)) {
    return FALSE;
  }
  // Check to see if device is bridge. If it is not return false
  if (!GnbLibPciIsBridgeDevice (PciDevice.AddressValue, NULL)) {
    return FALSE;
  }
  // See if device has eDPC capability
  GnbLibPciRead (PciDevice.AddressValue | PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_ENH_CAP_LIST_OFFSET,
                    AccessWidth32, &(DeviceCapFlag.Value), NULL);
  if (DeviceCapFlag.Field.CAP_ID != DPC_EXT_CAP_ID) {
    return FALSE;
  }
  // Read current eDPC control structure
  GnbLibPciRead (PciDevice.AddressValue | PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_CNTL_OFFSET,
                    AccessWidth16, &(DeviceCapCntl.Value), NULL);

  // Get Secondary Bus
  GnbLibPciRead (PciDevice.AddressValue | PCICFG_SPACE_SECONDARY_BUS_OFFSET, AccessWidth8, &Bus, NULL);
  Device.Address.Bus = Bus;

  // Scan for downstream bridges and switches to also process
  for (Dev = 0; Dev < 32; Dev++) {
    Device.Address.Device = Dev;

    for (Fun = 0; Fun <= 7; Fun++) {
      Device.Address.Function = Fun;

      if (GnbLibPciIsDevicePresent (Device.AddressValue, NULL)) {
        // If device is a bridge, enable eDPC on downstream bridge first (recursive call)
        if (GnbLibPciIsBridgeDevice (Device.AddressValue, NULL)) {
          EdpcEnabled = SetPcieEdpcFeature (Device);
        } else {
          // eDPC spec says enable eDPC on bridges/switches only.
        }

        // If not multi function skip to next Device
        if (!GnbLibPciIsMultiFunctionDevice (Device.AddressValue, NULL)) {
          break;
        }
      }
    }  //Fun
  }  //Dev

  EdpcEnabled = TRUE;
  DeviceCapCntl.Field.DPC_TRIGGER_ENABLE = 1;
  DeviceCapCntl.Field.DPC_COMPLETION_CONTROL = 1;
  DeviceCapCntl.Field.DPC_INTERRUPT_ENABLE = 0;
  DeviceCapCntl.Field.POISONED_TLP_EGRESS_BLOCKING_ENABLE = 0;
  DeviceCapCntl.Field.DPC_SOFTWARE_TRIGGER = 0;
  DeviceCapCntl.Field.DL_ACTIVE_ERR_COR_ENABLE = 0;

  // Set DPC_ERR_COR_ENABLE based on PcdAmdPcieAerReportMechanism ( (0) MCA reporting; (1) OS First; (2) FW First )
  if ((PcdGet8 (PcdAmdPcieAerReportMechanism) == 0) || (PcdGet8 (PcdAmdPcieAerReportMechanism) == 2)) {
    DeviceCapCntl.Field.DPC_ERR_COR_ENABLE = 1;
  }
  else {
    DeviceCapCntl.Field.DPC_ERR_COR_ENABLE = 0;
  }

  // Read current eDPC control structure
  GnbLibPciWrite (PciDevice.AddressValue | PCICFG_NBIO_FUNC0_PCIE0_PCIE_DPC_CNTL_OFFSET,
                       AccessS3SaveWidth16, &(DeviceCapCntl.Value), NULL);

  return EdpcEnabled;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  NbioSetPcieAerFeature
 *
 *  Description:
 *     Enable/Disable AER feature. This function only can set a limited number of AER bits and should not
 *     be used for future implementations. Please use NbioSetPcieAerFeatureExtended instead. This interface is
 *     kept for code that does not know any better.
 *
 *  Parameters:
 *    @param[in]     GnbHandle
 *    @param[in]     PortAerConfig
 *
 *    @retval        EFI_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/
EFI_STATUS
EFIAPI
NbioSetPcieAerFeature (
  IN       AMD_NBIO_PCIE_AER_PROTOCOL  *This,
  IN       PCIe_PORT_AER_CONFIG        *PcieAerSetting
  )
{
  EFI_STATUS                        Status;
  UINT16                            PcieCapPtr;
  PCI_ADDR                          Device;
  PCIE_CORR_ERR_MASK_STRUCT         CorrectableErrorSetup;
  PCIE_UNCORR_ERR_MASK_STRUCT       UncorrectableErrorSetup;
  PCIE_UNCORR_ERR_SEVERITY_STRUCT   UncorrectableErrorSeveritySetup;

  // IDS_HDT_CONSOLE (MAIN_FLOW, "NbioSetPcieAerFeature Entry\n");
  Status = EFI_UNSUPPORTED;
  Device.AddressValue = 0;

  if (PcieAerSetting->AerEnable == 1) {
    Device.Address.Bus = PcieAerSetting->PciBus;
    Device.Address.Device = PcieAerSetting->PciDev;
    Device.Address.Function = PcieAerSetting->PciFunc;

    PcieCapPtr = GnbLibFindPcieExtendedCapability (Device.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
    if (PcieCapPtr != 0) {
      // Uncorrectable Error Mask Register (Offset 08h)
      GnbLibPciRead (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET),
                                  AccessWidth32, &(UncorrectableErrorSetup.Value), NULL);

      UncorrectableErrorSetup.Field.DLP_ERR_MASK = PcieAerSetting->UncorrectableMask.Field.DataLinkProtocolErrorMask;
      UncorrectableErrorSetup.Field.PSN_ERR_MASK = PcieAerSetting->UncorrectableMask.Field.PoisonedTLPMask;
      UncorrectableErrorSetup.Field.CPL_TIMEOUT_MASK = PcieAerSetting->UncorrectableMask.Field.CompletionTimeoutMask;
      UncorrectableErrorSetup.Field.CPL_ABORT_ERR_MASK = PcieAerSetting->UncorrectableMask.Field.CompleterAbortMask;
      UncorrectableErrorSetup.Field.UNEXP_CPL_MASK = PcieAerSetting->UncorrectableMask.Field.UnexpectedCompletionMask;
      UncorrectableErrorSetup.Field.MAL_TLP_MASK = PcieAerSetting->UncorrectableMask.Field.MalTlpMask;
      UncorrectableErrorSetup.Field.ECRC_ERR_MASK = PcieAerSetting->UncorrectableMask.Field.ECRCErrorMask;
      UncorrectableErrorSetup.Field.UNSUPP_REQ_ERR_MASK =
                              PcieAerSetting->UncorrectableMask.Field.UnsupportedRequestErrorMask;
      UncorrectableErrorSetup.Field.ACS_VIOLATION_MASK = PcieAerSetting->UncorrectableMask.Field.AcsViolationMask;

      GnbLibPciWrite (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET),
                               AccessS3SaveWidth32, &(UncorrectableErrorSetup.Value), NULL);

      // Uncorrectable Error Severity Register (Offset 0Ch)
      GnbLibPciRead (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
                               AccessWidth32, &(UncorrectableErrorSeveritySetup.Value), NULL);

      UncorrectableErrorSeveritySetup.Field.DLP_ERR_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.DataLinkProtocolErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.PSN_ERR_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.PoisonedTLPSeverity;
      UncorrectableErrorSeveritySetup.Field.CPL_TIMEOUT_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.CompletionTimeoutSeverity;
      UncorrectableErrorSeveritySetup.Field.CPL_ABORT_ERR_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.CompleterAbortSeverity;
      UncorrectableErrorSeveritySetup.Field.UNEXP_CPL_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.UnexpectedCompletionSeverity;
      UncorrectableErrorSeveritySetup.Field.MAL_TLP_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.MalTlpSeverity;
      UncorrectableErrorSeveritySetup.Field.ECRC_ERR_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.ECRCErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.UNSUPP_REQ_ERR_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.UnsupportedRequestErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.ACS_VIOLATION_SEVERITY =
                              PcieAerSetting->UncorrectableSeverity.Field.AcsViolationSeverity;

      GnbLibPciWrite (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
                             AccessS3SaveWidth32, &(UncorrectableErrorSeveritySetup.Value), NULL);

      // Correctable Error Mask Register (Offset 14h)
      GnbLibPciRead (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET),
                             AccessWidth32, &(CorrectableErrorSetup.Value), NULL);

      CorrectableErrorSetup.Field.BAD_TLP_MASK = PcieAerSetting->CorrectableMask.Field.BadTLPMask;
      CorrectableErrorSetup.Field.BAD_DLLP_MASK = PcieAerSetting->CorrectableMask.Field.BadDLLPMask;
      CorrectableErrorSetup.Field.REPLAY_NUM_ROLLOVER_MASK =
                            PcieAerSetting->CorrectableMask.Field.ReplayNumberRolloverMask;
      CorrectableErrorSetup.Field.REPLAY_TIMER_TIMEOUT_MASK =
                            PcieAerSetting->CorrectableMask.Field.ReplayTimerTimeoutMask;
      CorrectableErrorSetup.Field.ADVISORY_NONFATAL_ERR_MASK =
                            PcieAerSetting->CorrectableMask.Field.AdvisoryNonFatalErrorMask;

      GnbLibPciWrite (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET),
                             AccessS3SaveWidth32, &(CorrectableErrorSetup.Value), NULL);

      //*NBIO_TODO : Revisit these how we dispatch these two calls per PCIe port
      //  after BA CBS PCIe configuration is finalized
      if (PcdGetBool(PcdPcieEcrcEnablement)) {
        SetPcieEcrcFeature(Device);
      }
      if (PcdGet8(PcdAmdEdpcEnable) > 0) { //(0) Disabled; (1) Enabled; (3) Auto
        SetPcieEdpcFeature(Device);
      }

      Status = EFI_SUCCESS;
    }
  }

  // IDS_HDT_CONSOLE (MAIN_FLOW, "NbioSetPcieAerFeature Exit(%x)\n", Status);
  return Status;
}


/**
 * Set passed in AER config
 *
 *
 * @param PcieAerSetting Desired AER configuration
 *
 * @return EFI_STATUS    Always returns EFI_NOT_AVAILABLE_YET because this function is only implemented for BA and up
 */
EFI_STATUS
EFIAPI
NbioSetPcieAerFeatureExtended (
  IN       PCIe_PORT_AER_CONFIG_EXTENDED        *PcieAerSetting
  )
{
  EFI_STATUS                        Status;
  UINT16                            PcieCapPtr;
  PCI_ADDR                          Device;
  PCIE_CORR_ERR_MASK_STRUCT         CorrectableErrorSetup;
  PCIE_UNCORR_ERR_MASK_STRUCT       UncorrectableErrorSetup;
  PCIE_UNCORR_ERR_SEVERITY_STRUCT   UncorrectableErrorSeveritySetup;

  // IDS_HDT_CONSOLE (MAIN_FLOW, "NbioSetPcieAerFeature Entry\n");
  Status = EFI_UNSUPPORTED;
  Device.AddressValue = 0;

  if (PcieAerSetting->AerEnable == 1) {
    Device.Address.Bus = PcieAerSetting->PciBus;
    Device.Address.Device = PcieAerSetting->PciDev;
    Device.Address.Function = PcieAerSetting->PciFunc;

    PcieCapPtr = GnbLibFindPcieExtendedCapability (Device.AddressValue, PCIE_EXT_AER_CAP_ID, NULL);
    if (PcieCapPtr != 0) {
      // Uncorrectable Error Mask Register (Offset 08h)
      GnbLibPciRead (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET),
                                            AccessWidth32, &(UncorrectableErrorSetup.Value), NULL);

      UncorrectableErrorSetup.Field.DLP_ERR_MASK                 =
                                        PcieAerSetting->UncorrectableMask.Field.DataLinkProtocolErrorMask;
      UncorrectableErrorSetup.Field.SURPDN_ERR_MASK              =
                                        PcieAerSetting->UncorrectableMask.Field.SurpriseDownErrorMask;
      UncorrectableErrorSetup.Field.PSN_ERR_MASK                 =
                                        PcieAerSetting->UncorrectableMask.Field.PoisonedTLPMask;
      UncorrectableErrorSetup.Field.FC_ERR_MASK                  =
                                        PcieAerSetting->UncorrectableMask.Field.FlowControlProtocolErrorMask;
      UncorrectableErrorSetup.Field.CPL_TIMEOUT_MASK             =
                                        PcieAerSetting->UncorrectableMask.Field.CompletionTimeoutMask;
      UncorrectableErrorSetup.Field.CPL_ABORT_ERR_MASK           =
                                        PcieAerSetting->UncorrectableMask.Field.CompleterAbortMask;
      UncorrectableErrorSetup.Field.UNEXP_CPL_MASK               =
                                        PcieAerSetting->UncorrectableMask.Field.UnexpectedCompletionMask;
      UncorrectableErrorSetup.Field.RCV_OVFL_MASK                =
                                        PcieAerSetting->UncorrectableMask.Field.ReceiverOverflowMask;
      UncorrectableErrorSetup.Field.MAL_TLP_MASK                 =
                                        PcieAerSetting->UncorrectableMask.Field.MalTlpMask;
      UncorrectableErrorSetup.Field.ECRC_ERR_MASK                =
                                        PcieAerSetting->UncorrectableMask.Field.ECRCErrorMask;
      UncorrectableErrorSetup.Field.UNSUPP_REQ_ERR_MASK          =
                                        PcieAerSetting->UncorrectableMask.Field.UnsupportedRequestErrorStatuser;
      UncorrectableErrorSetup.Field.ACS_VIOLATION_MASK           =
                                        PcieAerSetting->UncorrectableMask.Field.AcsViolationMask;
      UncorrectableErrorSetup.Field.UNCORR_INT_ERR_MASK          =
                                        PcieAerSetting->UncorrectableMask.Field.UncorrectableInternalErrorMask;
      UncorrectableErrorSetup.Field.MC_BLOCKED_TLP_MASK          =
                                        PcieAerSetting->UncorrectableMask.Field.McBlockedTlpMask;
      UncorrectableErrorSetup.Field.ATOMICOP_EGRESS_BLOCKED_MASK =
                                        PcieAerSetting->UncorrectableMask.Field.AtomicopEgressBlockedMask;
      UncorrectableErrorSetup.Field.TLP_PREFIX_BLOCKED_ERR_MASK  =
                                        PcieAerSetting->UncorrectableMask.Field.TlpPrefixBlockedErrorMask;

      GnbLibPciWrite (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_MASK_OFFSET),
                                        AccessS3SaveWidth32, &(UncorrectableErrorSetup.Value), NULL);

      // Uncorrectable Error Severity Register (Offset 0Ch)
      GnbLibPciRead (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
                                        AccessWidth32, &(UncorrectableErrorSeveritySetup.Value), NULL);

      UncorrectableErrorSeveritySetup.Field.DLP_ERR_SEVERITY                 =
                                        PcieAerSetting->UncorrectableSeverity.Field.DataLinkProtocolErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.SURPDN_ERR_SEVERITY              =
                                        PcieAerSetting->UncorrectableSeverity.Field.SurpriseDownErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.PSN_ERR_SEVERITY                 =
                                        PcieAerSetting->UncorrectableSeverity.Field.PoisonedTLPSeverity;
      UncorrectableErrorSeveritySetup.Field.FC_ERR_SEVERITY                  =
                                        PcieAerSetting->UncorrectableSeverity.Field.FlowControlProtocolErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.CPL_TIMEOUT_SEVERITY             =
                                        PcieAerSetting->UncorrectableSeverity.Field.CompletionTimeoutSeverity;
      UncorrectableErrorSeveritySetup.Field.CPL_ABORT_ERR_SEVERITY           =
                                        PcieAerSetting->UncorrectableSeverity.Field.CompleterAbortSeverity;
      UncorrectableErrorSeveritySetup.Field.UNEXP_CPL_SEVERITY               =
                                        PcieAerSetting->UncorrectableSeverity.Field.UnexpectedCompletionSeverity;
      UncorrectableErrorSeveritySetup.Field.RCV_OVFL_SEVERITY                =
                                        PcieAerSetting->UncorrectableSeverity.Field.ReceiverOverflowErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.MAL_TLP_SEVERITY                 =
                                        PcieAerSetting->UncorrectableSeverity.Field.MalTlpSeverity;
      UncorrectableErrorSeveritySetup.Field.ECRC_ERR_SEVERITY                =
                                        PcieAerSetting->UncorrectableSeverity.Field.ECRCErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.UNSUPP_REQ_ERR_SEVERITY          =
                                        PcieAerSetting->UncorrectableSeverity.Field.UnsupportedRequestErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.ACS_VIOLATION_SEVERITY           =
                                        PcieAerSetting->UncorrectableSeverity.Field.AcsViolationSeverity;
      UncorrectableErrorSeveritySetup.Field.UNCORR_INT_ERR_SEVERITY          =
                                        PcieAerSetting->UncorrectableSeverity.Field.UncorrectableInternalErrorSeverity;
      UncorrectableErrorSeveritySetup.Field.MC_BLOCKED_TLP_SEVERITY          =
                                        PcieAerSetting->UncorrectableSeverity.Field.McBlockedTlpSeverity;
      UncorrectableErrorSeveritySetup.Field.ATOMICOP_EGRESS_BLOCKED_SEVERITY =
                                        PcieAerSetting->UncorrectableSeverity.Field.AtomicOpEgressBlockedSeverity;
      UncorrectableErrorSeveritySetup.Field.TLP_PREFIX_BLOCKED_ERR_SEVERITY  =
                                        PcieAerSetting->UncorrectableSeverity.Field.TlpPrefixBlockedErrorSeverity;

      GnbLibPciWrite (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_UNCORRECTABLE_ERROR_SEVERITY_OFFSET),
                                        AccessS3SaveWidth32, &(UncorrectableErrorSeveritySetup.Value), NULL);

      // Correctable Error Mask Register (Offset 14h)
      GnbLibPciRead (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET),
                                        AccessWidth32, &(CorrectableErrorSetup.Value), NULL);

      CorrectableErrorSetup.Field.RCV_ERR_MASK                   =
                                        PcieAerSetting->CorrectableMask.Field.ReceiverErrorMask;
      CorrectableErrorSetup.Field.BAD_TLP_MASK                   =
                                        PcieAerSetting->CorrectableMask.Field.BadTLPMask;
      CorrectableErrorSetup.Field.BAD_DLLP_MASK                  =
                                        PcieAerSetting->CorrectableMask.Field.BadDLLPMask;
      CorrectableErrorSetup.Field.REPLAY_NUM_ROLLOVER_MASK       =
                                        PcieAerSetting->CorrectableMask.Field.ReplayNumberRolloverMask;
      CorrectableErrorSetup.Field.REPLAY_TIMER_TIMEOUT_MASK      =
                                        PcieAerSetting->CorrectableMask.Field.ReplayTimerTimeoutMask;
      CorrectableErrorSetup.Field.ADVISORY_NONFATAL_ERR_MASK     =
                                        PcieAerSetting->CorrectableMask.Field.AdvisoryNonFatalErrorMask;
      CorrectableErrorSetup.Field.CORR_INT_ERR_MASK              =
                                        PcieAerSetting->CorrectableMask.Field.CorrectedInternalErrorMask;
      CorrectableErrorSetup.Field.HDR_LOG_OVFL_MASK              =
                                        PcieAerSetting->CorrectableMask.Field.HeaderLogOverflowMask;

      GnbLibPciWrite (Device.AddressValue | (PcieCapPtr + PCIE_EXTCAP_AER_CORRECTABLE_ERROR_MASK_OFFSET),
                                         AccessS3SaveWidth32, &(CorrectableErrorSetup.Value), NULL);

      //*NBIO_TODO : Revisit these how we dispatch these two calls per PCIe port
      //after RMB CBS PCIe configuration is finalized
      if (PcdGetBool(PcdPcieEcrcEnablement)) {
        SetPcieEcrcFeature(Device);
      }
      if (PcdGet8(PcdAmdEdpcEnable) > 0) { //(0) Disabled; (1) Enabled; (3) Auto
        SetPcieEdpcFeature(Device);
      }

      Status = EFI_SUCCESS;
    }
  }

  // IDS_HDT_CONSOLE (MAIN_FLOW, "NbioSetPcieAerFeature Exit(%x)\n", Status);
  return Status;
}

/**
 *---------------------------------------------------------------------------------------
 *  IsBlackListDevice
 *
 *  Description:
 *     To check device is in the Black list or not
 *  Parameters:
 *    @param[in]     Value32    Vendor and Device ID
 *    @retval        BOOLEAN    The result of Black list
 *
 *---------------------------------------------------------------------------------------
 **/
BOOLEAN
IsBlackListDevice (
  PCI_ADDR  Device
  )
{
  UINTN               Index;
  UINT32              Value32;

  Index = 0;
  GnbLibPciRead (Device.AddressValue, AccessWidth32, &Value32, NULL);

  if (DefaultEcrcBlacklistDeviceList[Index].Value != 0) {
    while (DefaultEcrcBlacklistDeviceList[Index].Value != 0xFFFFFFFF) {
      if (DefaultEcrcBlacklistDeviceList[Index].Value == Value32) {
        return TRUE;
      }
      Index++;
      // limit size of table
      if (Index >= (sizeof(DefaultEcrcBlacklistDeviceList) / sizeof(VEN_DEV_ID))) {
        break;
      }

    }
  }

  return FALSE;
}
