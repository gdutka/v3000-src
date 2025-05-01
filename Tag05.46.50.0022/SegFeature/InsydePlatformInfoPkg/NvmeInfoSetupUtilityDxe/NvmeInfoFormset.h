/** @file

   The definition of Nvme Information HII.

;******************************************************************************
;* Copyright (c) 2021 - 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corp.
;*
;******************************************************************************
*/

#ifndef NVME_INFO_FORMSET_H_
#define NVME_INFO_FORMSET_H_

#include <Guid/NvmeInfoSetupUtilityHii.h>

//
// Form ID definition
//
//  H2O_NVME_INFO_CONTROLLER_LIST_FORM_ID            0x0001   // Reference defined from NvmeInfoSetupUtilityHii.h
#define H2O_NVME_INFO_CONTROLLER_INFO_FORM_ID        0x0002

#define H2O_NVME_INFO_LABEL_START                    0x0001
#define H2O_NVME_INFO_LABEL_END                      0x0002

#define NVME_CONTROLLER_INFORMATION_QUESTION_ID_BASE 0x0001

#define NVME_NAMESPACE_INFORMATION_QUESTION_ID_BASE  0x1000

#define NVME_SMART_INFORMATION_QUESTION_ID_BASE      0x2000

#define NVME_DEVICE_KEY                              0x3000

#endif // NVME_INFO_FORMSET_H_
