/*
*****************************************************************************
*
 * Copyright (C) 2018-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*/
#ifndef _RAS_RMB_BASE_LIB_H_
#define _RAS_RMB_BASE_LIB_H_

#include "AmdRasRegistersRmb.h"
#include "AmdRas.h"

UINTN
convert_to_addr_trans_index (
  UINTN  pkg_no,
  UINTN  mpu_no,
  UINTN  umc_inst_num,
  UINTN  umc_chan_num
  );

UINT32
smnRegRead (
  UINTN   socket,
  UINTN   die,
  UINTN   umc,
  UINTN   ch,
  UINTN   offset,
  UINTN   BusNumberBase
  );

VOID
retrieve_regs (
  UINTN   pkgno,
  UINTN   mpuno,
  UINTN   umcno,
  UINTN   umcchno,
  UINTN   BusNumberBase
  );

VOID
NormalizedToBankAddrMap (
  UINT64 ChannelAddr,
  UINT8 *Bank,
  UINT32 *Row,
  UINT16 *Col,
  UINT8 *Rankmul,
  UINT8 *Subchan,
  UINT8 numbankbits,
  UINT8 bank4,
  UINT8 bank3,
  UINT8 bank2,
  UINT8 bank1,
  UINT8 bank0,
  UINT8 numrowlobits,
  UINT8 numcolbits,
  UINT8 row_lo0,
  UINT32 COL0REG,
  UINT32 COL1REG,
  UINT8 numcsbits,
  UINT8 rm0,
  UINT8 rm1,
  UINT8 rm2,
  UINT8 chan,
  UINT8 invertmsbse,
  UINT8 invertmsbso,
  UINT8 SEC,
  UINT8 cs,
  UINT32 addrhashbank0,
  UINT32 addrhashbank1,
  UINT32 addrhashbank2,
  UINT32 addrhashbank3,
  UINT32 addrhashbank4,
  UINT32 addrhashpc,
  UINT32 addrhashpc2,
  UINT8 vcmen
  );

VOID
translate_norm_to_dram_addr (
  UINT64  ChannelAddr,
  UINT8   pkg_no,
  UINT8   mpu_no,
  UINT8   umc_inst_num,
  UINT8   umc_chan_num,
  UINT8   *cs_num,
  UINT8   *bank,
  UINT32  *row,
  UINT16  *col,
  UINT8   *rankmul,
  UINT8   *subchan
  );

UINT64 translate_dram_to_norm_addr (
  UINT8 pkg_no,
  UINT8 mpu_no,
  UINT8 umc_inst_num,
  UINT8 umc_chan_num,
  UINT8 cs_num,
  UINT8 bank,
  UINT32 row,
  UINT16 col,
  UINT8 rankmul,
  UINT8 subchan);

UINT64
calcSysAddr (
  UINT64  normAddr,
  UINT32  mySocketNum,
  UINT32  myDieNum,
  UINT32  myChannelNum
  );

NORMALIZED_ADDRESS
calcNormAddr (
    UINT64 sysAddr
    );

VOID
translate_norm_to_dpa (
  UINT64  ChannelAddr,
  UINT8   pkg_no,
  UINT8   mpu_no,
  UINT8   umc_inst_num,
  UINT8   umc_chan_num,
  UINT8   cs_num,
  UINT8   bank,
  UINT32  row,
  UINT16  col,
  UINT8   rankmul,
  UINT64  *Dpa
  );

VOID
RasSmnRead (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

VOID
RasSmnWrite (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

VOID
RasSmnRW (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  );

VOID
RasSmnRead8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8
  );

VOID
RasSmnWrite8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               *Value8
  );

VOID
RasSmnRW8 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT8               AndMask,
  IN       UINT8               OrMask
  );

EFI_STATUS
AcquireNbioSecBusNum (
  IN       UINT8 DfNode,
  IN       UINT8 NbioNumber,
  OUT      UINT8 *NbioSecBusNum
  );

extern  ADDR_DATA               *gAddrData;

#endif //_RAS_RMB_BASE_LIB_H_

