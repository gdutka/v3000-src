/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _CBS_H_
#define _CBS_H_

#include  <IdsHookId.h>

#define CBS_FUNC_DECLARE(Name, Family)	     CBS##Name##Family
#define CBS_FUNCTABLE_DECLARE(Name, Family)	 Cbs##Name##Family

#define AMD_RESERVE_ALL 0x00000000ul

#endif

