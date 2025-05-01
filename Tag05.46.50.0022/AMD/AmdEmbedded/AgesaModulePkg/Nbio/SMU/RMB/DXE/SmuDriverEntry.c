/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/* $NoKeywords:$ */
/**
 * @file
 *
 * SmuV13Dxe Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  SmuV13Dxe
 * @e \$Revision: 312065 $   @e \$Date: 2015-01-30 04:23:05 -0600 (Fri, 30 Jan 2015) $
 *
 */

#include <PiDxe.h>
#include <Filecode.h>
#include "SmuV13Dxe.h"
#include <Protocol/AmdCcxProtocol.h>
#include <Protocol/AmdNbioCppcServicesProtocol.h>
#include <Protocol/PciEnumerationComplete.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Guid/GnbPcieInfoHob.h>
#include <GnbRegistersRMB.h>
#include <IdsHookId.h>
#include <Library/NbioHandleLib.h>
#include <Library/PcieConfigLib.h>
#include <Library/AmdIdsHookLib.h>
#include <Guid/HobList.h>
#include <Library/UefiLib.h>
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Library/SmnAccessLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/GnbLib.h>
#include <Library/GnbPciAccLib.h>
#include <Library/GnbPciLib.h>
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>
#include <Library/ApcbLibV3.h>
#include <Protocol/AmdApcbProtocol.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Addendum/Apcb/Inc/RMB/ApcbV3TokenUid.h>
#include <Protocol/SocLogicalIdProtocol.h>
#include <Library/CcxSmbiosLib.h>
#include <Library/PcdLib.h>

#define FILECODE        NBIO_SMU_RMB_DXE_SMUDRIVERENTRY_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
extern AMD_CORE_TOPOLOGY_SERVICES_V2_PROTOCOL mAmdCoreTopologyServicesProtocol;
extern DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL mDxeAmdNbioSmuServicesProtocol;
extern DXE_AMD_NBIO_CPPC_SERVICES_PROTOCOL     mNbioCppcServicesProtocol;
extern VOID EFIAPI CallbackAfterCcxDxeDone (
  IN      EFI_EVENT  Event,
  IN      VOID       *Context
  );

extern
VOID
PopulatePcieLinkStatusTable (
  GNB_HANDLE                           *GnbHandle,
  PcieLaneMappingTable_t               *SmuMappingData
  );

extern
VOID
PopulatePPTable (
  Smu13BiosIfTable_t                   *PPTable
  );

extern
VOID
EnableOrDisableIpDeepSleep (
  GNB_HANDLE      *GnbHandle,
  BOOLEAN         FeatureDsLclk,
  BOOLEAN         FeatureDsSocClk,
  BOOLEAN         FeatureDsShubClk,
  BOOLEAN         FeatureDsVcn,
  BOOLEAN         FeatureDsDcFClk
  );

extern
VOID
GetNameString (
  CHAR8 *String,
  AMD_CONFIG_PARAMS *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define GNB_MB_TO_PAGE(a) ((a) * (SIZE_1MB / EFI_PAGE_SIZE))
#define PCICFG_SPACE_Master_CLASS_CODE_OFFSET         0x0B
#define PCICFG_SPACE_SUB_CLASS_CODE_OFFSET            0x0A
#define APCB_TYPE_PURPOSE_ADMIN                       2

typedef union {
  struct {
    UINT16    VendorId;
    UINT16    DeviceId;
  } Field;
  UINT32 Value;
} VEN_DEV_ID;
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/**
 *---------------------------------------------------------------------------------------
 *  IsApprovedListDevice
 *
 *  Description:
 *     To check device is in the Approved list or not
 *  Parameters:
 *    @param[in]     Value32    Vendor and Device ID
 *    @retval        BOOLEAN    The result of Approved list
 *
 *---------------------------------------------------------------------------------------
 **/
BOOLEAN
IsApprovedListDevice (
  UINT32  Value32
  )
{
  VEN_DEV_ID          *ApprovedListDeviceList;
  UINTN               Index;

  ApprovedListDeviceList = (VEN_DEV_ID*)(UINTN)PcdGet32 (PcdS0i3UnSafeShutdownApprovedListVidDid);
  Index = 0;

  if (ApprovedListDeviceList == NULL) {
     return FALSE;
  }

  if (ApprovedListDeviceList[Index].Value != 0) {
    while (ApprovedListDeviceList[Index].Value != 0xFFFFFFFF) {
      if (ApprovedListDeviceList[Index].Value == Value32) {
        return TRUE;
      }
      Index++;
      // limit size of table
      if (Index > 0xFF) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "Index over 0xFF\n");
        break;
      }

    }
  }

  return FALSE;
}

/**
 *---------------------------------------------------------------------------------------
 *  CallbackAfterPCIeEnumerateComplete
 *
 *  Description:
 *     notification event handler after gAmdPCIeEnumerateProtocolGuid ready
 *  Parameters:
 *    @param[in]     Event      Event whose notification function is being invoked.
 *    @param[in]     *Context   Pointer to the notification function's context.
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
EFIAPI
CallbackAfterPCIeEnumerateComplete (
  IN      EFI_EVENT  Event,
  IN      VOID       *Context
  )
{
  EFI_STATUS                              Status;
  PCIe_PLATFORM_CONFIG                    *PcieFromHob;
  PCIe_PLATFORM_CONFIG                    *Pcie;
  GNB_HANDLE                              *NbioHandle;
  UINT32                                  SmuArg[6];
  DXE_AMD_NBIO_SMU_SERVICES_PROTOCOL      *NbioSmuServiceProtocol;
  VOID                                    *Interface;
  PCIe_ENGINE_CONFIG                      *PcieEngine;
  PCIe_WRAPPER_CONFIG                     *PcieWrapper;
  UINT8      ClassCodeData;
  UINT8      SubClassCodeData;
  UINT8      SecondaryBusNumber;
  UINT8      NumberOfNvme;
  UINT32     Value32;
  BOOLEAN    IsNvme = FALSE;
  UINT8      PcieCapPtr;
  PCI_ADDR   Device;
  UINT8      PmcsrOffset;
  UINT8      MultiFunctionCounter;
  UINT32     VIDDID32;

  IDS_HDT_CONSOLE (MAIN_FLOW, "CallbackAfterPCIeEnumerateComplete First notify\n");

  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  (VOID **)&Interface
                  );
  if (EFI_ERROR (Status)) {
    return;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Callback AfterPCIeEnumerateComplete Entry\n");

  PcieFromHob = NULL;
  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;

  Status = gBS->LocateProtocol (
                  &gAmdNbioSmuServicesProtocolGuid,
                  NULL,
                  &NbioSmuServiceProtocol
                  );
  if (!EFI_ERROR (Status)) {
    NbioHandle = NbioGetHandle (Pcie);
    while (NbioHandle != NULL) {
      //
      // Message for SMU
      //
      if (PcdGet8 (PcdIsDAP) != 0xFF) {
        NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
        SmuArg[0] = (UINT32)PcdGet8 (PcdIsDAP);
        IDS_HDT_CONSOLE (NB_MISC, "  SmuArg 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", \
                                    SmuArg[0], SmuArg[1], SmuArg[2], SmuArg[3], SmuArg[4], SmuArg[5]);
        Status = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (NbioHandle),
                                          BIOSSMC_MSG_EnableWalle, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);  //0x44
      }

      if (PcdGet8 (PcdS0i3Enable) == 0x1) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "S0i3 Enabled\n");

        NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
        ClassCodeData = 0;
        SubClassCodeData = 0;
        SecondaryBusNumber = 0;
        NumberOfNvme = 0;
        MultiFunctionCounter = 0;

        PcieWrapper = PcieConfigGetChildWrapper (NbioHandle);
        while (PcieWrapper != NULL) {
          PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
          // Check NVME/ Non-NVME
          while (PcieEngine != NULL) {
            if (PcieLibIsEngineAllocated (PcieEngine) &&
              PcieConfigCheckPortStatus (PcieEngine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
              GnbLibPciRead(
                 PcieEngine->Type.Port.Address.AddressValue | PCICFG_SPACE_SECONDARY_BUS_OFFSET,
                 AccessWidth8,
                 &SecondaryBusNumber,
                 NULL
                 );
              IDS_HDT_CONSOLE (NB_MISC, "NVME/XHCI bus=%d dev=%d func=%d, SecondaryBusNumber 0x%x \n",
                               PcieEngine->Type.Port.Address.Address.Bus,
                               PcieEngine->Type.Port.Address.Address.Device,
                               PcieEngine->Type.Port.Address.Address.Function,
                               SecondaryBusNumber);

              MultiFunctionCounter = 0;

              do {
                GnbLibPciRead(
                   MAKE_SBDFO (0, SecondaryBusNumber, 0, MultiFunctionCounter, 0) | PCICFG_SPACE_REV_ID_OFFSET,
                   AccessWidth32,
                   &Value32,
                   NULL
                   );
                // Get VID DID
                GnbLibPciRead(
                   MAKE_SBDFO (0, SecondaryBusNumber, 0, MultiFunctionCounter, 0),
                   AccessWidth32,
                   &VIDDID32,
                   NULL
                   );

                IDS_HDT_CONSOLE (MAIN_FLOW, "EP's Device: bus=%d dev=0 func=%d Class Code: 0x%x VIDDID: 0x%x\n",SecondaryBusNumber, MultiFunctionCounter, Value32, VIDDID32);
                // EP device is available
                if (Value32 != 0xFFFFFFFF) {
                  ClassCodeData =  (UINT8) ((Value32 >> 24) & 0xFF);
                  SubClassCodeData = (UINT8) ((Value32 >> 16) & 0xFF);
                  IDS_HDT_CONSOLE (NB_MISC, "ClassCodeData=0x%x, SubClassCodeData=0x%x\n", ClassCodeData, SubClassCodeData);

                  IsNvme = FALSE;
                  // To check NVME device class code
                  if (ClassCodeData == 0x1) {
                    // To check device sub-class code
                    if (SubClassCodeData == 0x8) {
                      if (NumberOfNvme < 5) {
                        NumberOfNvme++;
                        Device.AddressValue = 0;
                        Device.Address.Bus = SecondaryBusNumber;
                        Device.Address.Function = MultiFunctionCounter;
                        PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, 0x1, (AMD_CONFIG_PARAMS* )NULL);
                        IDS_HDT_CONSOLE (NB_MISC, "PcieCapPtr=0x%x \n", PcieCapPtr);
                        PmcsrOffset = PcieCapPtr + 4;
                        //  [7:0] : ClassCodeData (0x1: NVME, 0x3:Display, 0xC: XHCI ...)
                        // [15:8] : PMCSR
                        //[23:16] : EP's function number
                        //[29:24] : WrapId
                        //[31:30] : PortId
                        SmuArg[NumberOfNvme] = ClassCodeData | (PmcsrOffset << 8);
                        SmuArg[NumberOfNvme] = SmuArg[NumberOfNvme] | (MultiFunctionCounter << 16);
                        SmuArg[NumberOfNvme] = SmuArg[NumberOfNvme] | (PcieEngine->Type.Port.PortId << 24);
                        SmuArg[NumberOfNvme] = SmuArg[NumberOfNvme] | (PcieWrapper->WrapId << 30);
                        IsNvme = TRUE;
                      }
                    }
                  }

                  // To check PCIe device (Non NVME)
                  if(IsNvme == FALSE) {
                    if (NumberOfNvme < 5) {
                      // Check Approved list
                      if (!IsApprovedListDevice (VIDDID32)) {
                        IDS_HDT_CONSOLE (MAIN_FLOW, "Not Approved list device : 0x%x\n", VIDDID32);
                        break;
                      }
                      NumberOfNvme++;
                      Device.AddressValue = 0;
                      Device.Address.Bus = SecondaryBusNumber;
                      Device.Address.Function = MultiFunctionCounter;
                      PcieCapPtr = GnbLibFindPciCapability (Device.AddressValue, 0x1, (AMD_CONFIG_PARAMS* )NULL);
                      IDS_HDT_CONSOLE (NB_MISC, "PcieCapPtr=0x%x \n", PcieCapPtr);
                      PmcsrOffset = PcieCapPtr + 4;
                      //  [7:0] : ClassCodeData (0x1: NVME, 0x3:Display, 0xC: XHCI ...)
                      // [15:8] : PMCSR
                      //[23:16] : EP's function number
                      //[29:24] : WrapId
                      //[31:30] : PortId
                      SmuArg[NumberOfNvme] = ClassCodeData | (PmcsrOffset << 8);
                      SmuArg[NumberOfNvme] = SmuArg[NumberOfNvme] | (MultiFunctionCounter << 16);
                      SmuArg[NumberOfNvme] = SmuArg[NumberOfNvme] | (PcieEngine->Type.Port.PortId << 24);
                      SmuArg[NumberOfNvme] = SmuArg[NumberOfNvme] | (PcieWrapper->WrapId << 30);
                    }
                  }
                  //
                  // Once over 5 devices, report to SMU first
                  //
                  if(NumberOfNvme == 5) {
                    // NVME, Non-NVME
                    SmuArg[0] = NumberOfNvme;
                    IDS_HDT_CONSOLE (NB_MISC, "  SmuArg 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", SmuArg[0], SmuArg[1], SmuArg[2], SmuArg[3], SmuArg[4], SmuArg[5]);
                    Status = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (NbioHandle), BIOSSMC_MSG_SetExternalPcieBuses, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);  //0x51
                    NumberOfNvme = 0; // Clear Counter;
                    NbioSmuServiceCommonInitArgumentsV13 (SmuArg); // clear SMU args
                  }
                }
                MultiFunctionCounter++;
              }while (Value32 != 0xFFFFFFFF);

            }
            PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
          }
          PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
        }
        if(NumberOfNvme != 0){
          // NVME, Non-NVME
          SmuArg[0] = NumberOfNvme;
          IDS_HDT_CONSOLE (NB_MISC, "  SmuArg 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",\
                                  SmuArg[0], SmuArg[1], SmuArg[2], SmuArg[3], SmuArg[4], SmuArg[5]);
          NbioSmuServiceRequestV13 (NbioGetHostPciAddress (NbioHandle),
                                 BIOSSMC_MSG_SetExternalPcieBuses, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);  //0x5A
        }

        if (PcdGetBool (PcdS0i3SetGenericPeRstEnable)) {
          NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
          SmuArg[0] = PcdGet32 (PcdS0i3SetGenericPeRstGpio0);
          SmuArg[1] = PcdGet32 (PcdS0i3SetGenericPeRstGpio1);
          IDS_HDT_CONSOLE (NB_MISC, "  SmuArg 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",\
                          SmuArg[0], SmuArg[1], SmuArg[2], SmuArg[3], SmuArg[4], SmuArg[5]);
          ///@todo
          // NbioSmuServiceRequestV13 (NbioGetHostPciAddress (NbioHandle), \
                        //BIOSSMC_MSG_SetGenericPeRstGpioAddrOffset, SmuArg, GNB_REG_ACC_FLAG_S3SAVE);  //0x56
        }

        // Send message of WLAN for RP - BIOSSMC_MSG_SetRootPortBridgeSbdfo
        if (PcdGet32 (PcdWLANS0i3SaveRestore) != 0) {
          NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
          SmuArg[0] = PcdGet32 (PcdWLANS0i3SaveRestore);
          NbioSmuServiceRequestV13 (NbioGetHostPciAddress (NbioHandle), BIOSSMC_MSG_SetRootPortBridgeSbdfo,
                                       SmuArg, GNB_REG_ACC_FLAG_S3SAVE);  //0x5C
        }
      }

      NbioHandle = GnbGetNextHandle (NbioHandle);
    }
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Callback AfterPCIeEnumerateComplete Exit\n");

  //
  // Close event, so it will not be invoked again.
  //
  gBS->CloseEvent (Event);
}

VOID
SyncSystemPowerSensorsRoutingConfig (
  IN AMD_APCB_SERVICE_PROTOCOL       *ApcbDxeServiceProtocol
  )
{
  EFI_STATUS                      Status;
  BOOLEAN                         IsWalleConfigChanged = FALSE;
  UINT8                           ApcbPurpose;
  UINT8                                Value;
  UINT8                                PcdValue;

  //step 1: sync POWER_SENSORS_ROUTING flag between SBIOS and APCB
  PcdValue = PcdGet8 (PcdPowerSensorsRoutingSelect);
  //read APCB config
  Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose,
                                                  APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT, &Value);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("1.Unable to get APCB token APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT, Status:%r\n",
                                                                                                           Status);
  } else {
    if (PcdValue != Value) {
      IDS_HDT_CONSOLE_PSP_TRACE ("PcdValue: %d, Value: %d, update in APCB\n", PcdValue, Value);
      Status = ApcbDxeServiceProtocol->ApcbSetToken8 (ApcbDxeServiceProtocol, APCB_TYPE_PURPOSE_ADMIN,
                                                        APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT, PcdValue);
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE_PSP_TRACE ("Unable to set APCB token APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT, Status:%r\n",
                                                                                                          Status);
      } else {
        Status = ApcbDxeServiceProtocol->ApcbGetToken8 (ApcbDxeServiceProtocol, &ApcbPurpose,
                                                         APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT, &Value);
        if (EFI_ERROR (Status)) {
          IDS_HDT_CONSOLE_PSP_TRACE ("2.Unable to get APCB token APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT, Status:%r\n",
                                                                                                          Status);
        } else {
          if (PcdValue == Value) { //update successfully
            IsWalleConfigChanged = TRUE;
          } else { //update failed
            IDS_HDT_CONSOLE_PSP_TRACE
                 ("Critial error: update to APCB token APCB_TOKEN_UID_NBIO_POWER_SENSORS_ROUTING_SELECT doesn't take effect\n");
          }
        }
      }
    }
  }

  if (IsWalleConfigChanged) {
    IDS_HDT_CONSOLE_PSP_TRACE ("IsWalleConfigChanged = TRUE\n");
    //Write back updated data to SPI
    ApcbDxeServiceProtocol->ApcbFlushData (ApcbDxeServiceProtocol);

    IDS_HDT_CONSOLE_PSP_TRACE ("Issue warm reset since APCB changed...\n");
    gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    //Should not come here
    CpuDeadLoop ();
  }

}
VOID
ApcbReadyCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                      Status;
  AMD_APCB_SERVICE_PROTOCOL       *mApcbDxeServiceProtocol;
  VOID                            *Interface;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a\n", __FUNCTION__);

  Status = gBS->LocateProtocol (
                &gPspFlashAccSmmCommReadyProtocolGuid,
                NULL,
                &Interface
                );
  if (EFI_ERROR (Status)) {
    return;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Locate gPspFlashAccSmmCommReadyProtocolGuid  success\n");

  gBS->CloseEvent(Event);

  Status = gBS->LocateProtocol (&gAmdApcbDxeServiceProtocolGuid, NULL, &mApcbDxeServiceProtocol);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("Unable to locate APCB Protocol");
    return ;
  }

  SyncSystemPowerSensorsRoutingConfig (mApcbDxeServiceProtocol);

  IDS_HDT_CONSOLE (MAIN_FLOW, "Callback ApcbReadyCallBack Exit\n");
}

//<Embedded_Override_Start>
/* -----------------------------------------------------------------------------*/
/**
 *
 *  CheckOPN
 *
 *
 *  Description:
 *    To Check OPN
 *
 *  @retval UINT8
 *
 */
UINT8 
EFIAPI 
CheckOPN ()
{
  CHAR8                                    OPNNameString[10] = {0};
  CHAR8                                    NameString[49]={0};
  AMD_CONFIG_PARAMS                        *StdHeader=NULL;
  UINT8                                    Index,J=0;  

  GetNameString (NameString, StdHeader);
  
  IDS_HDT_CONSOLE (CPU_TRACE,"\nCPU Name String : %a\n",NameString);
  //OPN model name present in 19-23 and 24 is for iTemp
  for(Index=19;Index<24;Index++){
    OPNNameString[J] = NameString[Index];
    J++;
  }
  OPNNameString[J] = '\0';
  IDS_HDT_CONSOLE (CPU_TRACE,"\nOPN String : %a%c\n",OPNNameString,NameString[24]);
  //Set iTemp for SMU
  if((NameString[24] == 'I') || (NameString[24]== 'i')){
    PcdSet8S (Pcditemp,1);
  }
  //To Restrict Fmax
  if ( (AsciiStrCmp (OPNNameString, "V3G18") == 0) || (AsciiStrCmp (OPNNameString, "V3G48") == 0) || (AsciiStrCmp (OPNNameString, "V3H48") == 0)){
    PcdSet32S (PcdFMaxFrequency, 0);
    return 1;
  }

  return 0;
} 
//<Embedded_Override_End>

/* -----------------------------------------------------------------------------*/
/**
 *
 *  SmuV13DxeEntry
 *
 *  @param[in]  ImageHandle     EFI Image Handle for the DXE driver
 *  @param[in]  SystemTable     Pointer to the EFI system table
 *
 *  Description:
 *    SMUV13 Driver Entry.
 *
 *  @retval EFI_STATUS
 *
 */
EFI_STATUS
EFIAPI
SmuV13DxeEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                           Status;
  EFI_HANDLE                           Handle;
  Smu13BiosIfTable_t                   *PPTable;
  EFI_PHYSICAL_ADDRESS                 SetAgmLogDramAddress;
  EFI_PHYSICAL_ADDRESS                 SmuDramBufferAddress;
  UINT32                               DramBufferSize;
  GNB_HANDLE                           *GnbHandle;
  PCIe_PLATFORM_CONFIG                 *Pcie;
  PCIe_PLATFORM_CONFIG                 *PcieFromHob;
  VOID                                 *Registration;
  UINT32                               PackageType;
  PcieLaneMappingTable_t               *LaneMappingTable;
  UINT64                               SmuMemoryDestination= 0;
  UINT64                               ToolDestination64 = 0;
  UINT64                               STBlDestination64= 0;
  EFI_PHYSICAL_ADDRESS                 STBAddress;
  PRESIL_CTRL1                         PreSilCtrl1;
  UINT32                               GPIOTableAddress = 0;
  AMD_SOC_LOGICAL_ID_PROTOCOL          *SocLogicalId;
  SOC_LOGICAL_ID                       LogicalId;

  AGESA_TESTPOINT (TpNbioSmuDxeEntry, NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a\n", __FUNCTION__);

  //<Embedded_Override_Start>
  CheckOPN();
  //<Embedded_Override_End>

  Handle = NULL;
  SetAgmLogDramAddress = 0xffffffff;

  SocLogicalId = NULL;
  LogicalId.Family = 0x0;
  LogicalId.Revision = 0x0;
  Status = gBS->LocateProtocol (&gAmdSocLogicalIdProtocolGuid, NULL, (VOID **) &SocLogicalId);
  if (!EFI_ERROR (Status)) {
    SocLogicalId->GetLogicalIdOnCurrentCore (SocLogicalId, &LogicalId);
  }

  // Enable default SMU Features based on package type
  PackageType = LibAmdGetPackageType ((AMD_CONFIG_PARAMS *) NULL);
  IDS_HDT_CONSOLE (MAIN_FLOW, "PackageType = %d\n", PackageType);

  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdNbioSmuServicesProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mDxeAmdNbioSmuServicesProtocol
                  );

  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdCoreTopologyServicesV2ProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAmdCoreTopologyServicesProtocol
                  );
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdNbioCppcServicesProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mNbioCppcServicesProtocol
                  );

  // Collecting Pcie information from Hob
  SmuDxeInitialize (&PcieFromHob);
  Pcie = PcieFromHob;
  GnbHandle = NbioGetHandle (Pcie);

  //
  // Allocate memory for various tables. Use type runtime because tables are not regenerated on S3 Resume.
  //
  Status = gBS->AllocatePool (
                  EfiRuntimeServicesData,
                  sizeof (Smu13BiosIfTable_t),
                  &PPTable
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Allocation FAILED for PPTable\n", __FUNCTION__);
    return Status;
  }

  // Fill PPTable with values that are stored as PCDs, then use IDS hook to populate further values (if any)
  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_DFLT_TBL == 0) {
    PopulatePPTable(PPTable);
  }
  SmuFixupPlatformConfig (PackageType, PPTable);
  if (PreSilCtrl1.Field.FASTSIM_DFLT_TBL == 0) {
    IDS_HOOK (IDS_HOOK_NBIO_SMU_INIT, (VOID*)NbioGetHandle (Pcie), (VOID*)PPTable);
  }

  DumpSmuFeatureControl (PcdGet32 (PcdSmuFeatureControlDefines), PcdGet32 (PcdSmuFeatureControlDefinesExt),
                                                                          PcdGet32 (PcdSmuFeatureControlDefines64));
  DumpPPTable (PPTable);

  GnbHandle = NbioGetHandle (Pcie);

  while (GnbHandle != NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "SMU Version End\n");

    // Get external GPIO table for Z9/S0i3
    GPIOTableAddress = PcdGet32 (PcdAmdSmuGpioConfigTable);
    if (GPIOTableAddress !=0) {
      //
      //  Pass DRAM space for the GPIO table Structure.
      //
      SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetBiosDramAddr,
                   (UINT32) ((EFI_PHYSICAL_ADDRESS) GPIOTableAddress & (0xFFFFFFFF)),
                   0, 0, 0, 0, 0);
      //
      //  Call BIOSSMC_MSG_TransferBiosIfTableToSmu to ask SMU to read in the GPIO Table,
      // SMU reply when the DRAM read is complete.
      //
      SmuServiceRequest (GnbHandle, BIOSSMC_MSG_TransferTableDram2Smu, TABLE_BIOS_GPIO_CONFIG, 0, 0, 0, 0, 0);
    }
    //
    //  Pass DRAM space for the PPTable Structure.
    //
    if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 0) {
      SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetBiosDramAddr,
                   (UINT32) ((EFI_PHYSICAL_ADDRESS) PPTable & (0xFFFFFFFF)),
                  (UINT32) (UINT32) RShiftU64 ((UINT64)PPTable, 32), 0, 0, 0, 0);
      //
      //  Call BIOSSMC_MSG_TransferBiosIfTableToSmu to ask SMU to read in the PP Table,
      // SMU reply when the DRAM read is complete.
      //
      GnbLibDebugDumpBuffer ((VOID*) PPTable, sizeof (Smu13BiosIfTable_t), 1, 16);
      SmuServiceRequest (GnbHandle, BIOSSMC_MSG_TransferTableDram2Smu, TABLE_BIOS_IF, 0, 0, 0, 0, 0);

      //
      //  Pass pointer to LaneMappingTable Structure.
      //
      Status = gBS->AllocatePool (
                      EfiRuntimeServicesData,
                      sizeof (PcieLaneMappingTable_t),
                      &LaneMappingTable
                      );
      if (EFI_ERROR (Status)) {
        IDS_HDT_CONSOLE (MAIN_FLOW, "%a : Allocation FAILED for LaneMappingTable\n", __FUNCTION__);
        return Status;
      }

      PopulatePcieLinkStatusTable (GnbHandle, LaneMappingTable);
      SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetBiosDramAddr,
                      (UINT32) ((EFI_PHYSICAL_ADDRESS) LaneMappingTable & (0xFFFFFFFF)),
                      (UINT32) (UINT32) RShiftU64 ((UINT64)LaneMappingTable, 32), 0, 0, 0, 0);
      GnbLibDebugDumpBuffer ((VOID*) LaneMappingTable, sizeof (PcieLaneMappingTable_t), 1, 16);
      //
      //  Tell SMU to read the table
      //
      // NBIO_RMB_TODO SmuServiceRequest (GnbHandle, BIOSSMC_MSG_TransferPcieLaneMappingTableToSmu, 0, 0, 0, 0, 0, 0);

      // Enable deep sleep on all IP block. This must be done before SMU feature enable bits are sent
      EnableOrDisableIpDeepSleep (
        GnbHandle,
        (PcdGet32 (PcdSmuFeatureControlDefines) & (FEATURE_DS_LCLK_MASK)) > 0 ? TRUE : FALSE,
        (PcdGet32 (PcdSmuFeatureControlDefines) & (FEATURE_DS_SOCCLK_MASK)) > 0 ? TRUE : FALSE,
        (PcdGet32 (PcdSmuFeatureControlDefines) & (FEATURE_DS_SHUBCLK_MASK)) > 0 ? TRUE : FALSE,
        (PcdGet32 (PcdSmuFeatureControlDefinesExt) & (FEATURE_DS_VCN_MASK)) > 0 ? TRUE : FALSE,
        (PcdGet32 (PcdSmuFeatureControlDefines) & (FEATURE_DS_DCFCLK_MASK)) > 0 ? TRUE : FALSE
        );

      // Get Smu Memory address
      SmuMemoryDestination = (UINT64)(PcdGet32 (PcdReservedSmuMemoryLo) |
                                    LShiftU64 (PcdGet32 (PcdReservedSmuMemoryHi),32));
      IDS_HDT_CONSOLE (MAIN_FLOW, "SmuMemoryDestination = 0x%lx\n", SmuMemoryDestination);

      if (PcdGet32 (PcdSmuFeatureControlDefines) & FEATURE_DATA_CALCULATION_MASK) {
        //
        // Allocate 16MB for STB Dram size
        //
        if (PcdGetBool (PcdMsgSetStbDramAddr) != 0) {
          if (SmuMemoryDestination != 0) {
            STBlDestination64 = SmuMemoryDestination;
            IDS_HDT_CONSOLE (MAIN_FLOW, "STBlDestination64 = 0x%lx\n", (UINT64)STBlDestination64);
            SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetStbDramAddr,
                              (UINT32) STBlDestination64,(UINT32) RShiftU64 ((UINT64)STBlDestination64, 32),
                              16 * SIZE_1MB, 0, 0, 0);
            SmuMemoryDestination += SIZE_16MB;

          } else {
            Status = gBS->AllocatePages (
                            AllocateAnyPages,
                            EfiACPIMemoryNVS,
                            GNB_MB_TO_PAGE (16),
                            &STBAddress
                            );
            IDS_HDT_CONSOLE (MAIN_FLOW, "Status = 0x%x\n", Status);
            if (!EFI_ERROR (Status)) {
              IDS_HDT_CONSOLE (MAIN_FLOW, "STB Address = 0x%x\n", STBAddress);
              ZeroMem ((VOID *) STBAddress, 16 * SIZE_1MB);
              SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetStbDramAddr, (UINT32)STBAddress,
                                (UINT32) RShiftU64 ((UINT64)STBAddress, 32), 16 * SIZE_1MB, 0, 0, 0);
            } else {
              IDS_HDT_CONSOLE (MAIN_FLOW, "Allocation FAILED!!\n");
            }
          }
        }
      }
      //
      //  Allocate 4KB DRAM, Call Message BIOSSMC_MSG_SetAgmLogDramAddress with 64 bit address
      //
      if (SmuMemoryDestination != 0) {
        // for 256K alignment
        ToolDestination64 = SmuMemoryDestination;
        IDS_HDT_CONSOLE (MAIN_FLOW, "ToolDestination64 = 0x%lx\n", (UINT64)ToolDestination64);
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetToolsDramAddr, (UINT32) ToolDestination64,
                                          (UINT32) RShiftU64 ((UINT64)ToolDestination64, 32), 0, 0, 0, 0);
        SmuMemoryDestination += SIZE_256KB;
      } else {
        SetAgmLogDramAddress = 0xFFFFFFFF;
        DramBufferSize = PcdGet16 (PcdCfgAgmLogDramSize);
        if (DramBufferSize == 0) {
          DramBufferSize = 4;
        }
        Status = gBS->AllocatePages (
                        AllocateMaxAddress,
                        EfiReservedMemoryType,
                        DramBufferSize,
                        &SetAgmLogDramAddress
                        );
        IDS_HDT_CONSOLE (MAIN_FLOW, "AgmLogDram Address = 0x%x, size = 0x%x\n", \
                                                           SetAgmLogDramAddress, DramBufferSize * 4096);
        IDS_HDT_CONSOLE (MAIN_FLOW, "Status = 0x%x\n", Status);

        ZeroMem ((VOID *) (UINTN) SetAgmLogDramAddress, (DramBufferSize * EFI_PAGE_SIZE));
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetToolsDramAddr, (UINT32)SetAgmLogDramAddress,
                                            (UINT32) RShiftU64 ((UINT64)SetAgmLogDramAddress, 32), 0, 0, 0, 0);
      }

      if (GnbHandle->DieNumber == 0) {
        //  Step22 not support anymore when DRTM enabled
        //
        //  Step22. if CBS options SmuAllocateDramBuffer is non-zero, allocate unsecure
                    //DRAM based on the size in Byte defined in the CBS options.
        //          Call message BIOSSMC_MSG_SetDramBufferAddress to pass in the 64 bit address
                    //(use 2 message argument)
        //  NOTE:  This memory is allocated for Die 0 of each socket
        DramBufferSize = PcdGet32 (PcdSmuAllocateDramBufferSize);

        if (DramBufferSize != 0) {
          IDS_HDT_CONSOLE (MAIN_FLOW, "Allocating DramBufferSize = 0x%x\n", DramBufferSize);
          Status = gBS->AllocatePages (
                          AllocateAnyPages,
                          EfiACPIMemoryNVS,
                          GNB_MB_TO_PAGE (DramBufferSize),
                          &SmuDramBufferAddress
                          );
          IDS_HDT_CONSOLE (MAIN_FLOW, "Status = 0x%x\n", Status);
          if (!EFI_ERROR (Status)) {
            IDS_HDT_CONSOLE (MAIN_FLOW, "SmuDramBufferAddress = 0x%x\n", SmuDramBufferAddress);

            ZeroMem ((VOID *) SmuDramBufferAddress, DramBufferSize * SIZE_1MB);
            SmuServiceRequest (GnbHandle, BIOSSMC_MSG_DramLogSetDramAddr, (UINT32)SmuDramBufferAddress,
                          (UINT32) RShiftU64 ((UINT64)SmuDramBufferAddress, 32), DramBufferSize * SIZE_1MB, 0, 0, 0);
          } else {
            IDS_HDT_CONSOLE (MAIN_FLOW, "Allocation FAILED!!\n");
          }
        }
      }


      if (PcdGetBool (PcdEnablePowerGateMmHub) != 0) {
        SmuServiceRequest (GnbHandle, BIOSSMC_MSG_PowerGateMmHub, 0, 0, 0, 0, 0, 0);
      }
    }

    if ((LogicalId.Family == AMD_FAMILY_RMB) &&
        ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
      // BIOSSMC_MSG_SetupCfg0NbifTrap
      SmuServiceRequest (GnbHandle, BIOSSMC_MSG_SetupCfg0NbifTrap, 0, 0, 0, 0, 0, 0);
    }

    SmuPackageTypeWorkarounds (PackageType, GnbHandle);
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  EfiCreateProtocolNotifyEvent (&gAmdCcxDxeInitCompleteProtocolGuid, TPL_CALLBACK,
                                               CallbackAfterCcxDxeDone, NULL, &Registration);

  EfiCreateProtocolNotifyEvent (&gEfiPciEnumerationCompleteProtocolGuid, TPL_NOTIFY,
                                               CallbackAfterPCIeEnumerateComplete, NULL, &Registration);

  IDS_HDT_CONSOLE (MAIN_FLOW, "ApcbReadyCallBack\n");
  EfiCreateProtocolNotifyEvent (
     &gPspFlashAccSmmCommReadyProtocolGuid,
     TPL_NOTIFY,
     ApcbReadyCallBack,
     NULL,
     &Registration
     );

  AGESA_TESTPOINT (TpNbioSmuDxeExit, NULL);

  return EFI_SUCCESS;
}


