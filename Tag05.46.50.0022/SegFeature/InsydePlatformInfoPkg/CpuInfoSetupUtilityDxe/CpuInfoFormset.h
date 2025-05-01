/** @file

   The definition of CPU Information HII.

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

#ifndef _CPU_INFO_FORMSET_H_
#define _CPU_INFO_FORMSET_H_

#include <Guid/CpuInfoSetupUtilityHii.h>

//
// Form ID definition
//
// H2O_CPU_INFO_FORM_ID                               0x0001  // Referenced from CpuInfoSetupUtilityHii.c

//
// Question ID definition
//
// H2O_CPU_INFO_TOTAL_CPU_QUESTION_ID                 0x0001  // Referenced from CpuInfoSetupUtilityHii.c
#define H2O_CPU_INFO_DYNAMIC_DETAIL_BASE_QUESTION_ID  0x1000

//
// Label definition
//
#define H2O_CPU_INFO_LABEL_START                      0x0001
#define H2O_CPU_INFO_LABEL_END                        0x0002

#endif
