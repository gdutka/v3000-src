/** @file
  IntelTgHddPwd

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

#ifndef _INTEL_TG_HDD_PWD_H_
#define _INTEL_TG_HDD_PWD_H_

#include <Uefi.h>
#include <Protocol/UefiRaidOpromReady.h>
#include <Protocol/LoadedImage.h>

#include <IndustryStandard/Pci30.h>

#include <Guid/IntelRemapDevInfoVariable.h>
#include <Guid/H2OBdsCheckPoint.h>

#include <Library/H2OCpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/VariableLib.h>
#include <Library/SataLib.h>

#include <BdsPlatform.h>

#define DYNAMIC_CONFIG    0x1

#define PCI_CLASS_MASS_STORAGE_NVM                0x08  // mass storage sub-class non-volatile memory.
#define PCI_IF_NVMHCI                             0x02  // mass storage programming interface NVMHCI.

#define IS_PCI_NVM(_p)               IS_CLASS2 (_p, PCI_CLASS_MASS_STORAGE, PCI_CLASS_MASS_STORAGE_NVM)

BOOLEAN
IsTgDevice (
  UINT16                    DeviceVendorId,
  UINT16                    DeviceDeviceId
  );

VOID
BuildTgDeviceInfoForIntelRst (
  );

VOID
DisconnectTgController (
  );

#endif
