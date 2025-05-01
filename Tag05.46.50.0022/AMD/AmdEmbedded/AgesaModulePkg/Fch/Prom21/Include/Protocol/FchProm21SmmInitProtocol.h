/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _FCH_PROM21_SMM_INIT_PROTOCOL_H_
#define _FCH_PROM21_SMM_INIT_PROTOCOL_H_

extern EFI_GUID gFchProm21SmmInitProtocolGuid;

typedef struct _FCH_PROM21_SMM_INIT_PROTOCOL FCH_PROM21_SMM_INIT_PROTOCOL;

//
// Protocol prototypes
//

/// PT PLUS SMM INIT Protocol
typedef struct _FCH_PROM21_SMM_INIT_PROTOCOL {
  UINTN                     Revision;                 ///< Protocol Revision
} FCH_PROM21_SMM_INIT_PROTOCOL;

#endif // #ifndef _FCH_PROM21_SMM_INIT_PROTOCOL_H_

