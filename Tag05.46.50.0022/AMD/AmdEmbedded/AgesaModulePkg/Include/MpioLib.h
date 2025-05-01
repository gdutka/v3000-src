/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * MPIO services
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2020-12-11 16:53:43 -0600 (Fri, 11 Dec 2020) $
 *
 */
#ifndef _NBIOMPIOLIB_H_
#define _NBIOMPIOLIB_H_

#include <NbioRegisterTypes.h>


#define INVALID_MPIO_MESSAGE                  0xFF

// MPIO Response Codes: // @Todo Need to confirm with DXIO team
#define BIOSMPIO_Result_OK                    0x1
#define BIOSMPIO_Result_Failed                0xFF
#define BIOSMPIO_Result_UnknownCmd            0xFE
#define BIOSMPIO_Result_CmdRejectedPrereq     0xFD
#define BIOSMPIO_Result_CmdRejectedBusy       0xFC


// Address C2PMSG 21
#define MPIO_C2PMSG_DOORBELL_ADDRESS                        0xc910554ul

// Address C2PMSG 50
#define MPIO_C2PMSG_RESPONSE_ADDRESS                        0xC9109C8ul

// Address C2PMSG 51 to 56
#define MPIO_C2PMSG_ARGUMENT_0_ADDRESS                      0xC9109CCul
#define MPIO_C2PMSG_ARGUMENT_1_ADDRESS                      0xC9109D0ul
#define MPIO_C2PMSG_ARGUMENT_2_ADDRESS                      0xC9109D4ul
#define MPIO_C2PMSG_ARGUMENT_3_ADDRESS                      0xC9109D8ul
#define MPIO_C2PMSG_ARGUMENT_4_ADDRESS                      0xC9109DCul
#define MPIO_C2PMSG_ARGUMENT_5_ADDRESS                      0xC9109E0ul

#define MPIO_C2PMSG_RESPONSE_STATUS_MASK                    0xFFul

/// MPIO basic lib

VOID
NbioMpioServiceCommonInitArguments (
  IN OUT   UINT32                   *MpioArg
  );

UINT32
MpioServiceRequest (
  IN       PCI_ADDR                 NbioPciAddress,
  IN       UINT32                   RequestId,
  IN       UINT32                   *RequestArgument,
  IN       UINT32                   AccessFlags
  );

#endif


