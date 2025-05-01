/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _MPM_ASF_PEI_H_
#define _MPM_ASF_PEI_H_

#include <Base.h>
#include <Ppi/ReportStatusCodeHandler.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IdsLib.h>
#include <Library/PcdLib.h>
#include <Pi/PiStatusCode.h>
#include <Pi/PiPeiCis.h>

#include "../MpmAsf.h"

#define PEI_MEMORY_NOT_INSTALLED  (EFI_SOFTWARE_PEI_CORE | EFI_SW_PS_PC_INSTALL_PEI_MEMORY)
#define PEI_MEMORY_NOT_DETECTED   (EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_NONE_DETECTED)

#endif
