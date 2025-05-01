/** @file

   The definition of Memory Information HII.

;******************************************************************************
;* Copyright (c) 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _MEMORY_INFO_FORMSET_H_
#define _MEMORY_INFO_FORMSET_H_

#include <Guid/MemoryInfoSetupUtilityHii.h>

//
// Form ID definition
//
//  H2O_MEMORY_INFO_DIMM_LIST_FORM_ID                               0x0001  // Referenced MemoryInfoSetupUtilityHii.c
#define H2O_MEMORY_INFO_DIMM_INFO_FORM_ID                           0x0002

//
// Question ID definition
//
//  H2O_MEMORY_INFO_TOTAL_MEMORY_QUESTION_ID                        0x0001  // Referenced MemoryInfoSetupUtilityHii.c
//  H2O_MEMORY_INFO_INSTALLED_MEMORY_QUESTION_ID                    0x0002
//  H2O_MEMORY_INFO_UNINSTALL_MEMORY_QUESTION_ID                    0x0003
//  H2O_MEMORY_INFO_ECC_TYPE_QUESTION_ID                            0x0004
//  H2O_MEMORY_INFO_TOTAL_MEMORY_SIZE_QUESTION_ID                   0x0005
#define H2O_MEMORY_INFO_HIDDEN_UNINSTALL_MEMORY_QUESTION_ID         0x0006
#define H2O_MEMORY_INFO_DYNAMIC_DIMM_BASE_QUESTION_ID               0x1000
#define H2O_MEMORY_INFO_DYNAMIC_DIMM_DETAIL_BASE_INFO_QUESTION_ID   0x2000

//
// Label definition
//
#define H2O_MEMORY_INFO_LABEL_START                                 0x0001
#define H2O_MEMORY_INFO_LABEL_END                                   0x0002
#define H2O_MEMORY_DETAIL_LABEL_START                               0x0003
#define H2O_MEMORY_DETAIL_LABEL_END                                 0x0004

#endif
