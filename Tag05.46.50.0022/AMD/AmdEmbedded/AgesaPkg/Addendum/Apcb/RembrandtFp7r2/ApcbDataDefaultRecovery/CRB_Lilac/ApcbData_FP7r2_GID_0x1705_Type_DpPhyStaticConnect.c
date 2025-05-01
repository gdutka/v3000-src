/*******************************************************************************
*
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************/

#include <MyPorting.h>
#include <APCB.h>
#include "ApcbCustomizedDefinitions.h"
#include "ApcbCustomizedBoardDefinitions.h"
#include <ApcbDefaults.h>


APCB_TYPE_DATA_START_SIGNATURE();
APCB_V3_TYPE_HEADER       ApcbTypeHeader = {
  APCB_GROUP_GNB,                             // GroupId
  APCB_GNB_TYPE_DP_PHY_STATIC_CONNECT,        // TypeId
  sizeof(ApcbTypeHeader),        // SizeOfType, will be fixed up by tool
  0,                             // InstanceId
  {
    APCB_TYPE_ATTR_CONTEXT_TYPE_STRUCT,
    APCB_TYPE_ATTR_CONTEXT_FORMAT_NATIVE_RAW,
    0,
    APCB_PRIORITY_TYPE_MASK_DEFAULT,    // Priority mask
    0,
    0,
    BLDCFG_APCB_DATA_BOARD_MASK         // Board specific APCB instance mask
  }
};  // SizeOfType will be fixed up by tool


//Dump by AGESA log CompareBetweenApcbAndUMA, should be align with CPM DDI Config.
APCB_STATIC_INFO_V1_0_CONNECT_ENTRY  DisplayDpPhyStaticInfoId =
{
// ATOM_COMMON_TABLE_HEADER
{
  0x0200,  //  uint16_t structuresize;
  0x01,  //  uint8_t  format_revision
  0x00  //  uint8_t  content_revision
},
// ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO
 // ATOM_COMMON_TABLE_HEADER
{
 {
   0x008C,  //  uint16_t structuresize;
   0x01,  //  uint8_t  format_revision
   0x01  //  uint8_t  content_revision
 },
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},     //  uint8_t                   guid[16];                                  // a GUID is a 16 byte long string
   // atom_ext_display_path [0]
 {
   {
    0x2,     //  uint16_t  device_tag;                      // A bit vector to show what devices are supported
    0x110,   //  uint16_t  device_acpi_enum;                // 16bit device ACPI id.
    0x3114,  //  uint16_t  connectorobjid;                  // A physical connector for displays to plug in, using object connector definitions
    0x0,     //  uint8_t   auxddclut_index;                 // An index into external AUX/DDC channel LUT
    0x0,     //  uint8_t   hpdlut_index;                    // An index into external HPD pin LUT
    0,       //  uint16_t  ext_encoder_objid;               // external encoder object id
    0xE4,    //  uint8_t   channelmapping;                  // if ucChannelMapping=0, using default one to one mapping
    0,       //  uint8_t   chpninvert;                      // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0,       //  uint32_t  caps;
   },
   // atom_ext_display_path [1]
   {
    0x8,    //  uint16_t  device_tag;                       // A bit vector to show what devices are supported
    0x210,  //  uint16_t  device_acpi_enum;                 // 16bit device ACPI id.
    0x3113, //  uint16_t  connectorobjid;                   // A physical connector for displays to plug in, using object connector definitions
    0x1,    //  uint8_t   auxddclut_index;                  // An index into external AUX/DDC channel LUT
    0x1,    //  uint8_t   hpdlut_index;                     // An index into external HPD pin LUT
    0,      //  uint16_t  ext_encoder_objid;                // external encoder object id
    0xE4,   //  uint8_t   channelmapping;                   // if ucChannelMapping=0, using default one to one mapping
    0,      //  uint8_t   chpninvert;                       // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0x300000,      //  uint32_t  caps;
   },
   // atom_ext_display_path [2]
   {0x80,   //  uint16_t  device_tag;                       // A bit vector to show what devices are supported
    0x220,  //  uint16_t  device_acpi_enum;                 // 16bit device ACPI id.
    0x3213, //  uint16_t  connectorobjid;                   // A physical connector for displays to plug in, using object connector definitions
    0x2,    //  uint8_t   auxddclut_index;                  // An index into external AUX/DDC channel LUT
    0x2,    //  uint8_t   hpdlut_index;                     // An index into external HPD pin LUT
    0,      //  uint16_t  ext_encoder_objid;                // external encoder object id
    0,      //  uint8_t   channelmapping;                   // if ucChannelMapping=0, using default one to one mapping
    0xe4,   //  uint8_t   chpninvert;                       // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0x20300102,  //  uint32_t  caps;
   },
   // atom_ext_display_path [3]
   {0x200,  //  uint16_t  device_tag;                       // A bit vector to show what devices are supported
    0x230,  //  uint16_t  device_acpi_enum;                 // 16bit device ACPI id.
    0x3313, //  uint16_t  connectorobjid;                   // A physical connector for displays to plug in, using object connector definitions
    0x3,    //  uint8_t   auxddclut_index;                  // An index into external AUX/DDC channel LUT
    0x3,    //  uint8_t   hpdlut_index;                     // An index into external HPD pin LUT
    0,      //  uint16_t  ext_encoder_objid;                // external encoder object id
    0xE4,   //  uint8_t   channelmapping;                   // if ucChannelMapping=0, using default one to one mapping
    0,      //  uint8_t   chpninvert;                       // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0x20300102,  //  uint32_t  caps;
   },
   // atom_ext_display_path [4]
   {0x400,  //  uint16_t  device_tag;                       // A bit vector to show what devices are supported
    0x240,  //  uint16_t  device_acpi_enum;                 // 16bit device ACPI id.
    0x3413, //  uint16_t  connectorobjid;                   // A physical connector for displays to plug in, using object connector definitions
    0x4,    //  uint8_t   auxddclut_index;                  // An index into external AUX/DDC channel LUT
    0x4,    //  uint8_t   hpdlut_index;                     // An index into external HPD pin LUT
    0,      //  uint16_t  ext_encoder_objid;                // external encoder object id
    0xE4,   //  uint8_t   channelmapping;                   // if ucChannelMapping=0, using default one to one mapping
    0,      //  uint8_t   chpninvert;                       // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0x300100,//  uint32_t  caps;
   },
   // atom_ext_display_path [5]
   {0, //  uint16_t  device_tag;                      // A bit vector to show what devices are supported
    0, //  uint16_t  device_acpi_enum;                // 16bit device ACPI id.
    0, //  uint16_t  connectorobjid;                  // A physical connector for displays to plug in, using object connector definitions
    0, //  uint8_t   auxddclut_index;                 // An index into external AUX/DDC channel LUT
    0, //  uint8_t   hpdlut_index;                    // An index into external HPD pin LUT
    0, //  uint16_t  ext_encoder_objid;               // external encoder object id
    0, //  uint8_t   channelmapping;                  // if ucChannelMapping=0, using default one to one mapping
    0, //  uint8_t   chpninvert;                      // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0, //  uint32_t  caps;
   },
   // atom_ext_display_path [6]
   {0, //  uint16_t  device_tag;                      // A bit vector to show what devices are supported
    0, //  uint16_t  device_acpi_enum;                // 16bit device ACPI id.
    0, //  uint16_t  connectorobjid;                  // A physical connector for displays to plug in, using object connector definitions
    0, //  uint8_t   auxddclut_index;                 // An index into external AUX/DDC channel LUT
    0, //  uint8_t   hpdlut_index;                    // An index into external HPD pin LUT
    0, //  uint16_t  ext_encoder_objid;               // external encoder object id
    0, //  uint8_t   channelmapping;                  // if ucChannelMapping=0, using default one to one mapping
    0, //  uint8_t   chpninvert;                      // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
    0, //  uint32_t  caps;
   }
 },
   0,      //  uint8_t                   checksum;                                  // a simple Checksum of the sum of whole structure equal to 0x0.
   0,      //  uint8_t                   stereopinid;                               // use for eDP panel
   0,      //  uint8_t                   remotedisplayconfig;
   0,      //  uint8_t                   edptolvdsrxid;
   0,      //  uint8_t                   fixdpvoltageswing;                         // usCaps[1]=1, this indicate DP_LANE_SET value
   {0,0,0}//  uint8_t                   reserved[3];
},
  // for potential expansion
  0,             //uint8_t                               modern_standby_resume;          /*0: Normal boot up; 1: Resume from modern standby/S3 */
  {0,0,0,0,0},   //   uint8_t                      DP_dphy_tx_vboost_lvl[5];
  {0,0,0,0,0},   //   uint8_t                      DP_dphy_tx_term_ctrl[5];
  {0},           //   uint8_t                               reserved[1];
  {0}            //  uint32_t                              reserved[89];
};


APCB_TYPE_DATA_END_SIGNATURE();
APCB_DUMMY_MAIN_FUNC();

