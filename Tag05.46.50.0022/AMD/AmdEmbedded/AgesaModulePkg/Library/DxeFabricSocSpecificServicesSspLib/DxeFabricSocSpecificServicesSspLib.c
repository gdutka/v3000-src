/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Fabric SOC Specific Services
 *
 * Contains code that provides fabric SOC specific services
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Fabric
 * @e \$Revision$   @e \$Date$
 *
 */
#include <Library/SmnAccessLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseFabricTopologyLib.h>
#include <Library/IdsLib.h>
#include <Protocol/FabricSocSpecificServicesProtocol.h>
#include <Filecode.h>
#include <AGESA.h>

#define FILECODE LIBRARY_DXEFABRICSOCSPECIFICSERVICESSSPLIB_DXEFABRICSOCSPECIFICSERVICESSSPLIB_FILECODE

#define RX_INPUT_OFFSET                         0x08B4
#define FCH_MISC2_RMT_PLLCNTL_0_REG             0x02D02330

EFI_STATUS
EFIAPI
FabricGetXgmiFreq (
  IN       FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL    *This,
  IN OUT   FABRIC_XGMI_FREQ_FOR_EACH_LINK           *XgmiFreqForEachLink
  );

STATIC FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL  mAmdFabricSocSpecificServices= {
  AMD_FABRIC_SOC_SPECIFIC_PROTOCOL_REV,
  FabricGetXgmiFreq
};


EFI_STATUS
EFIAPI
FabricSocSpecificServicesProtocolInstall (
  IN       EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdFabricSocSpecificServicesProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mAmdFabricSocSpecificServices
                  );
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * FabricGetXgmiFreq
 *
 * Get xGMI frequency for each link
 *
 * @param[in]         This                  Function pointer to FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL.
 * @param[in, out]    XgmiFreqForEachLink   xGMI frequency for each link
 *
 * @retval            EFI_SUCCESS           Frequencies successfully obtained
 *                    EFI_ABORTED           Unable to obtain information
 */
EFI_STATUS
EFIAPI
FabricGetXgmiFreq (
  IN       FABRIC_SOC_SPECIFIC_SERVICES_PROTOCOL    *This,
  IN       FABRIC_XGMI_FREQ_FOR_EACH_LINK           *XgmiFreqForEachLink
  )
{
  EFI_STATUS        Status;
  UINT8             NumOfSockets;
  UINT8             Socket;
  UINT8             Link;
  UINT32            XgmiPcsBaseAddrs[] = {0x12E00000, 0x12F00000, 0x13000000, 0x13100000};
  UINT32            XgmiPcsRxInputAddrs;
  UINT32            FreqCnt;
  UINT32            RefClk;
  UINT8             LinkCnt;

  ASSERT((sizeof(XgmiPcsBaseAddrs)/sizeof(UINT32)) >= MAX_XGMI_LINKS_PER_SOCKET);

  NumOfSockets = (UINT8)FabricTopologyGetNumberOfProcessorsPresent ();
  if (NumOfSockets < 2) {
    return EFI_ABORTED;
  }

  // read ref clock
  SmnRegisterRead (0, FCH_MISC2_RMT_PLLCNTL_0_REG, &RefClk);
  RefClk = (((RefClk & 0xFF) == 0) | ((RefClk & 0xFF) == 0xFF)) ? 100 : 133;

  LinkCnt = 0;
  for (Socket=0; Socket < NumOfSockets; Socket++) {
    for (Link=0; Link < MAX_XGMI_LINKS_PER_SOCKET; Link++) {
      ASSERT(LinkCnt < MAX_XGMI_LINKS_SUPPORTED);

      // get raw frequency count
      XgmiPcsRxInputAddrs = XgmiPcsBaseAddrs[Link] + RX_INPUT_OFFSET;
      SmnRegisterRead ((UINT32) (FabricTopologyGetHostBridgeBusBase (Socket, 0, 0)), XgmiPcsRxInputAddrs, &FreqCnt);
      FreqCnt = FreqCnt >> 24;

      // calculate xGMI frequency
      XgmiFreqForEachLink->XgmiFreq[LinkCnt++] = FreqCnt*2*RefClk/1000;
    }
  }

  Status = EFI_SUCCESS;

  return Status;
}


