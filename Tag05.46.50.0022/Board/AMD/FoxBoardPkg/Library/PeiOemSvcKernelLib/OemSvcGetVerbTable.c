/** @file
  This OemService is part of setting Verb Table. The function is created for setting verb table
  to support Multi-Sku and return the table to common code to program.

;******************************************************************************
;* Copyright (c) 2012 - 2021, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Library/PeiOemSvcKernelLib.h>
#include <Ppi/VerbTable.h>
#include <MultiSkuInfoCommonDef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sample :                                                                                                      //
//                                                                                                               //
//   #define OEM_VERB_TABLE_ID_(n)         n                                                                     //
//                                                                                                               //
//   #define OEM_VERB_TABLE_(n)_HEADER(a)  0x10EC0287, // Vendor ID / Device ID                                  //
//                                         0x00000000, // SubSystem ID                                           //
//                                         0xFF,       // RevisionId                                             //
//                                         0x02,       // Front panel support ( 1 = Yes, 2 = No )                //
//                                         0x0022,     // Number of Rear Jacks = 136                             //
//                                         0x0000      // Number of Front Jacks =  0                             //
//                                                                                                               //
//   #define OEM_VERB_TABLE_(n)_DATA(a)    0x01171CF0,0x01171D11,0x01171E11,0x01171F41, // NID(0x11): 0x411111F0 //
//                                         0x01271C40,0x01271D09,0x01271EA3,0x01271F99, // NID(0x12): 0x99A30940 //
//                                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// To define Verb Table ID.
//
#if 0
#define OEM_VERB_TABLE_ID_1          1

#define OEM_VERB_TABLE_1_HEADER1     0x10EC0287, \
                                     0x00000000, \
                                     0xFF,       \
                                     0x02,       \
                                     0x0022,     \
                                     0x0000

//
//ALC3306_17AA3809_20200409_7909_S750_14_TGL_HPF_120Hz
//
#define OEM_VERB_TABLE_1_DATA1        \
   0x00172009, 0x00172138, 0x001722AA, 0x00172317, \
   0x0017FF00, 0x0017FF00, 0x0017FF00, 0x0017FF00, \
   0x01271CF0, 0x01271D11, 0x01271E11, 0x01271F41, \
   0x01371CF0, 0x01371D11, 0x01371E11, 0x01371F41, \
   0x01471C10, 0x01471D01, 0x01471E17, 0x01471F90, \
   0x01671CF0, 0x01671D11, 0x01671E11, 0x01671F41, \
   0x01771CF0, 0x01771D11, 0x01771E11, 0x01771F41, \
   0x01871CF0, 0x01871D11, 0x01871E11, 0x01871F41, \
   0x01971C30, 0x01971D10, 0x01971EA1, 0x01971F03, \
   0x01A71CF0, 0x01A71D11, 0x01A71E11, 0x01A71F41, \
   0x01B71CF0, 0x01B71D11, 0x01B71E11, 0x01B71F41, \
   0x01D71C6D, 0x01D71D1A, 0x01D71E47, 0x01D71F40, \
   0x01E71CF0, 0x01E71D11, 0x01E71E11, 0x01E71F41, \
   0x02171C20, 0x02171D10, 0x02171E21, 0x02171F03, \
   0x0205000A, 0x02047770, 0x02050010, 0x02048006, \
   0x02050045, 0x02045289, 0x0205004A, 0x02042010, \
   0x02050009, 0x02048000, 0x02050038, 0x02047909, \
   0x02050040, 0x02048800, 0x02050010, 0x02048303, \
   0x05C5000F, 0x05C40000, 0x05A50004, 0x05A42113, \
   0x0205005F, 0x0204E200, 0x0205005F, 0x0204E200, \
   0x05350000, 0x0534001A, 0x05350000, 0x0534201A, \
   0x0535001d, 0x05340800, 0x0535001e, 0x05340800, \
   0x05350003, 0x05341F80, 0x05350004, 0x05340000, \
   0x05350003, 0x05341F75, 0x05350004, 0x05340000, \
   0x05350003, 0x05341FC0, 0x05350004, 0x05340000, \
   0x05350003, 0x05341FE0, 0x05350004, 0x05340000, \
   0x05350000, 0x05442000, 0x0545001d, 0x05440800, \
   0x0545001e, 0x05440800, 0x0545001e, 0x05440800, \
   0x05450003, 0x05441F80, 0x05450004, 0x05440000, \
   0x05450003, 0x05441F75, 0x05450004, 0x05440000, \
   0x05450003, 0x05441FC0, 0x05450004, 0x05440000, \
   0x05450003, 0x05441FE0, 0x05450004, 0x05440000, \
   0x05350000, 0x0534E01A, 0x02050038, 0x02047909, \
   0x05350002, 0x05348000, 0x05D50006, 0x05D44C50   

#ifdef OEM_VERB_TABLE_ID_1
DEFINE_VERB_TABLE_LOCAL_HEADER_DATA_1(OEM_VERB_TABLE_ID_1);
COLLECT_DEFINE_VERB_TABLE_LOCAL_HEADER_DATA_1(OEM_VERB_TABLE_ID_1);
#endif
#endif

/**
  This OemService is part of setting Verb Table. The function is created for setting verb table 
  to support Multi-Sku and return the table to common code to program.

  @param  *VerbTableHeaderDataAddress    A pointer to VerbTable data/header

  @retval EFI_MEDIA_CHANGED              Get verb table data/header success.
  @retval Others                         Base on OEM design.
**/
EFI_STATUS
OemSvcGetVerbTable (
  OUT COMMON_CHIPSET_AZALIA_VERB_TABLE      **VerbTableHeaderDataAddress
  )
{
#if 0
  *VerbTableHeaderDataAddress = VERB_TABLE_HEADER_DATA_BUFFER_ADDRESS(OEM_VERB_TABLE_ID_1);

  return EFI_MEDIA_CHANGED;
#else
  return EFI_UNSUPPORTED;
#endif
}
