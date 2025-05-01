/** @file
;******************************************************************************
;* Copyright (c) 2024, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/
#ifndef _X64_CPU_DEVICE_INFO_H_
#define _X64_CPU_DEVICE_INFO_H_

//
// Intel Platform
//
#define INTEL_MSR_RAPL_POWER_UNIT_ADDRESS   FixedPcdGet32(PcdH2OIntelMsrRaplPwrUnitAddress)
#define INTEL_MSR_POWER_UNIT_BITFIELD       0x0F   // Bit 3:0

#define INTEL_MSR_PKG_POWER_LIMIT_ADDRESS   FixedPcdGet32(PcdH2OIntelMsrPkgPwrLimitAddress)
#define INTEL_MSR_PKG_PWR_LIM_1_BITFIELD    0x7FFF // Bit 14:0

#define MAX_NUM_OF_CACHE_PARAMS_LEAF        6

//
// AMD Platform
//
#define IOHC_NB_SMN_INDEX2                  0xB8
#define IOHC_NB_SMN_DATA2                   0xBC
#define BIOSSMC_RESULT_OK                   0x01
#define BIOSSMC_RESULT_FATAL                0xFC
#define INVALID_SMU_MSG                     0xFF
#define SMC_MSG_TDP_TJMAX                   FixedPcdGet32(PcdH2OAmdBiosSmcMsgTdpRequestId)
#define MP1_C2PMSG_MESSAGE_ADDRESS          FixedPcdGet32(PcdH2OAmdMp1C2pmsgMessageAddress)
#define MP1_C2PMSG_RESPONSE_ADDRESS         FixedPcdGet32(PcdH2OAmdMp1C2pmsgResponseAddress)
#define MP1_C2PMSG_ARGUMENT_0_ADDRESS       FixedPcdGet32(PcdH2OAmdMp1C2pmsgArgument0Address)
#define MP1_C2PMSG_ARGUMENT_1_ADDRESS       FixedPcdGet32(PcdH2OAmdMp1C2pmsgArgument1Address)
#define MP1_C2PMSG_ARGUMENT_2_ADDRESS       FixedPcdGet32(PcdH2OAmdMp1C2pmsgArgument2Address)
#define MP1_C2PMSG_ARGUMENT_3_ADDRESS       FixedPcdGet32(PcdH2OAmdMp1C2pmsgArgument3Address)
#define MP1_C2PMSG_ARGUMENT_4_ADDRESS       FixedPcdGet32(PcdH2OAmdMp1C2pmsgArgument4Address)
#define MP1_C2PMSG_ARGUMENT_5_ADDRESS       FixedPcdGet32(PcdH2OAmdMp1C2pmsgArgument5Address)

#endif // _X64_CPU_DEVICE_INFO_H_