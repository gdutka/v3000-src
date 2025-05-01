/*****************************************************************************
 * Copyright (C) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Initialize GFX V2 configuration data structure.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313706 $   @e \$Date: 2019-06-11 14:00:43 +0800 (Tus, 11 June 2019) $
 *
 */

#ifndef _NBIOGFXV2_H_
#define _NBIOGFXV2_H_

#include <AGESA.h>
#include <Gnb.h>

#define DEVICE_DFP                                0x1
#define DEVICE_CRT                                0x2
#define DEVICE_LCD                                0x3


#define CONNECTOR_DISPLAYPORT_ENUM                0x3013
#define CONNECTOR_HDMI_TYPE_A_ENUM                0x300c
#define CONNECTOR_SINGLE_LINK_DVI_D_ENUM          0x3003
#define CONNECTOR_DUAL_LINK_DVI_D_ENUM            0x3004
#define CONNECTOR_SINGLE_LINK_DVI_I_ENUM          0x3001
#define CONNECTOR_DUAL_LINK_DVI_I_ENUM            0x3002
#define CONNECTOR_VGA_ENUM                        0x3005
#define CONNECTOR_LVDS_ENUM                       0x300E
#define CONNECTOR_eDP_ENUM                        0x3014
#define CONNECTOR_LVDS_eDP_ENUM                   0x3016
//DP to VGA:
#define ENCODER_DP2VGA_ENUM_ID1                   0x2123
//DP to LVDS:
#define ENCODER_DP2LVDS_ENUM_ID2                  0x2223
//Hudson-2 NutMeg DP to VGA:
#define ENCODER_ALMOND_ENUM_ID1                   0x2122
#define ENCODER_NOT_PRESENT                       0x0000

// Third party translator which requires AMD SW init
#define eDP_TO_LVDS_SWINIT_ID                     0x02


#define ATOM_DEVICE_CRT1_SUPPORT                  0x0001
#define ATOM_DEVICE_DFP1_SUPPORT                  0x0008
#define ATOM_DEVICE_DFP6_SUPPORT                  0x0040
#define ATOM_DEVICE_DFP2_SUPPORT                  0x0080
#define ATOM_DEVICE_DFP3_SUPPORT                  0x0200
#define ATOM_DEVICE_DFP4_SUPPORT                  0x0400
#define ATOM_DEVICE_DFP5_SUPPORT                  0x0800
#define ATOM_DEVICE_LCD1_SUPPORT                  0x0002
//
// Assume standard IA-32 alignment.
// BugBug: Need to check portability of long long
//
typedef unsigned long long  uint64_t;
typedef long long           int64_t;
typedef unsigned int        uint32_t;
typedef int                 int32_t;
typedef unsigned short      uint16_t;
typedef short               int16_t;
typedef unsigned char       uint8_t;
typedef signed char         int8_t;

//IPCLEAN_START
enum  atom_bios_header_version_def{
  ATOM_MAJOR_VERSION        =0x0003,
  ATOM_MINOR_VERSION        =0x0003,
};

#ifdef _H2INC
  #ifndef uint32_t
    typedef unsigned long uint32_t;
  #endif

  #ifndef uint16_t
    typedef unsigned short uint16_t;
  #endif

  #ifndef uint8_t
    typedef unsigned char uint8_t;
  #endif

  #ifndef int32_t
    typedef signed long int32_t;
  #endif

  #ifndef int16_t
    typedef signed short int16_t;
  #endif

#endif

typedef enum {
  ATOM_CRTC1      =0,
  ATOM_CRTC2      =1,
  ATOM_CRTC3      =2,
  ATOM_CRTC4      =3,
  ATOM_CRTC5      =4,
  ATOM_CRTC6      =5,
  ATOM_CRTC_INVALID  =0xff,
} ATOM_CRTC_DEF;

typedef enum {
  ATOM_PPLL0          =2,
  ATOM_GCK_DFS        =8,
  ATOM_FCH_CLK        =9,
  ATOM_DP_DTO         =11,
  ATOM_COMBOPHY_PLL0  =20,
  ATOM_COMBOPHY_PLL1  =21,
  ATOM_COMBOPHY_PLL2  =22,
  ATOM_COMBOPHY_PLL3  =23,
  ATOM_COMBOPHY_PLL4  =24,
  ATOM_COMBOPHY_PLL5  =25,
  ATOM_PPLL_INVALID   =0xff,
} ATOM_PPLL_DEF;


// define ASIC internal encoder id ( bit vector ), used for CRTC_SourceSel
typedef enum {
  ASIC_INT_DIG1_ENCODER_ID  =0x03,
  ASIC_INT_DIG2_ENCODER_ID  =0x09,
  ASIC_INT_DIG3_ENCODER_ID  =0x0a,
  ASIC_INT_DIG4_ENCODER_ID  =0x0b,
  ASIC_INT_DIG5_ENCODER_ID  =0x0c,
  ASIC_INT_DIG6_ENCODER_ID  =0x0d,
  ASIC_INT_DIG7_ENCODER_ID  =0x0e,
} ATOM_DIG_DEF;

//ucEncoderMode
typedef enum {
  ATOM_ENCODER_MODE_DP          =0,
  ATOM_ENCODER_MODE_DP_SST      =0,
  ATOM_ENCODER_MODE_LVDS        =1,
  ATOM_ENCODER_MODE_DVI         =2,
  ATOM_ENCODER_MODE_HDMI        =3,
  ATOM_ENCODER_MODE_DP_AUDIO    =5,
  ATOM_ENCODER_MODE_DP_MST      =5,
  ATOM_ENCODER_MODE_CRT         =15,
  ATOM_ENCODER_MODE_DVO         =16,
} ATOM_ENCODE_MODE_DEF;

typedef enum {
  ENCODER_REFCLK_SRC_P1PLL      =0,
  ENCODER_REFCLK_SRC_P2PLL      =1,
  ENCODER_REFCLK_SRC_P3PLL      =2,
  ENCODER_REFCLK_SRC_EXTCLK     =3,
  ENCODER_REFCLK_SRC_INVALID    =0xff,
} ATOM_ENCODER_REFCLK_SRC_DEF;

typedef enum {
  ATOM_SCALER_DISABLE          =0,  /*scaler bypass mode, auto-center & no replication*/
  ATOM_SCALER_CENTER           =1,  //For Fudo, it's bypass and auto-center & auto replication
  ATOM_SCALER_EXPANSION        =2,  /*scaler expansion by 2 tap alpha blending mode*/
} ATOM_SCALER_DEF;

typedef enum {
  ATOM_DISABLE             = 0,
  ATOM_ENABLE              = 1,
  ATOM_INIT                = 7,
  ATOM_GET_STATUS          = 8,
} ATOM_OPERATION_DEF;

typedef enum {
  ATOM_LCD_BL_OFF                = 2,
  ATOM_LCD_BL_OM                 = 3,
  ATOM_LCD_BL_BRIGHTNESS_CONTROL = 4,
  ATOM_LCD_SELFTEST_START        = 5,
  ATOM_LCD_SELFTEST_STOP         = 6,
} ATOM_EMBEDDED_DISPLAY_OP_DEF;

typedef enum {
  ATOM_SS_CENTER_OR_DOWN_MODE_MASK  = 0x01,
  ATOM_SS_DOWN_SPREAD_MODE          = 0x00,
  ATOM_SS_CENTRE_SPREAD_MODE        = 0x01,
  ATOM_INT_OR_EXT_SS_MASK           = 0x02,
  ATOM_INTERNAL_SS_MASK             = 0x00,
  ATOM_EXTERNAL_SS_MASK             = 0x02,
} ATOM_SPREAD_SPECTRUM_MODE;

/* define panel bit per color  */
typedef enum {
  PANEL_BPC_UNDEFINE     =0x00,
  PANEL_6BIT_PER_COLOR   =0x01,
  PANEL_8BIT_PER_COLOR   =0x02,
  PANEL_10BIT_PER_COLOR  =0x03,
  PANEL_12BIT_PER_COLOR  =0x04,
  PANEL_16BIT_PER_COLOR  =0x05,
} ATOM_PANEL_BIT_PER_COLOR;

//ucVoltageType
typedef enum {
  VOLTAGE_TYPE_VDDC = 1,
  VOLTAGE_TYPE_VDDCR_SOC = 1,
  VOLTAGE_TYPE_MVDDC = 2,
  VOLTAGE_TYPE_VDDCR_HBM = 2,
  VOLTAGE_TYPE_MVDDQ = 3,
  VOLTAGE_TYPE_VDDIO_MEM = 3,
  VOLTAGE_TYPE_VDDCI = 4,
  VOLTAGE_TYPE_VDDCI_MEM = 4,
  VOLTAGE_TYPE_VDDGFX = 5,
  VOLTAGE_TYPE_VDDCR_GFX = 5,
  VOLTAGE_TYPE_PCC = 6,
  VOLTAGE_TYPE_MVPP = 7,
  VOLTAGE_TYPE_LEDDPM = 8,
  VOLTAGE_TYPE_PCC_MVDD = 9,
  VOLTAGE_TYPE_PCIE_VDDC = 10,
  VOLTAGE_TYPE_VDDIO_PCIE = 10,
  VOLTAGE_TYPE_PCIE_VDDR = 11,
  VOLTAGE_TYPE_GENERIC_I2C_1 = 0x11,
  VOLTAGE_TYPE_GENERIC_I2C_2 = 0x12,
  VOLTAGE_TYPE_GENERIC_I2C_3 = 0x13,
  VOLTAGE_TYPE_GENERIC_I2C_4 = 0x14,
  VOLTAGE_TYPE_GENERIC_I2C_5 = 0x15,
  VOLTAGE_TYPE_GENERIC_I2C_6 = 0x16,
  VOLTAGE_TYPE_GENERIC_I2C_7 = 0x17,
  VOLTAGE_TYPE_GENERIC_I2C_8 = 0x18,
  VOLTAGE_TYPE_GENERIC_I2C_9 = 0x19,
  VOLTAGE_TYPE_GENERIC_I2C_10 = 0x1A,
  VOLTAGE_TYPE_VDDAN_075 = 0x21,
  VOLTAGE_TYPE_VDDIO_18 = 0x22,
  VOLTAGE_TYPE_INP_CUR_WARN_FAULT = 0x31,
  VOLTAGE_TYPE_INP_CUR_WARN_FAULT_1 = 0x31,
  VOLTAGE_TYPE_INP_CUR_WARN_FAULT_2 = 0x32,
} ATOM_VOLTAGE_TYPE;

typedef enum {
  ATOM_DGPU_VRAM_TYPE_GDDR5 = 0x50,
  ATOM_DGPU_VRAM_TYPE_HBM2  = 0x60,
  ATOM_DGPU_VRAM_TYPE_GDDR6 = 0x70,
} ATOM_DGPU_VRAM_TYPE;

typedef enum {
  DP_VS_LEVEL0_PREEMPH_LEVEL0 = 0x00,
  DP_VS_LEVEL1_PREEMPH_LEVEL0 = 0x01,
  DP_VS_LEVEL2_PREEMPH_LEVEL0 = 0x02,
  DP_VS_LEVEL3_PREEMPH_LEVEL0 = 0x03,
  DP_VS_LEVEL0_PREEMPH_LEVEL1 = 0x08,
  DP_VS_LEVEL1_PREEMPH_LEVEL1 = 0x09,
  DP_VS_LEVEL2_PREEMPH_LEVEL1 = 0x0a,
  DP_VS_LEVEL0_PREEMPH_LEVEL2 = 0x10,
  DP_VS_LEVEL1_PREEMPH_LEVEL2 = 0x11,
  DP_VS_LEVEL0_PREEMPH_LEVEL3 = 0x18,
} ATOM_DP_VS_PREEMPH_DEF;


/*
enum atom_string_def{
asic_bus_type_pcie_string = "PCI_EXPRESS",
atom_fire_gl_string       = "FGL",
atom_bios_string          = "ATOM"
};
*/
//IPCLEAN_END
#pragma pack(1)                          /* BIOS data must use byte aligment*/
//IPCLEAN_START
enum atombios_image_offset{
OFFSET_TO_ATOM_ROM_HEADER_POINTER          =0x00000048,
OFFSET_TO_ATOM_ROM_IMAGE_SIZE              =0x00000002,
OFFSET_TO_ATOMBIOS_ASIC_BUS_MEM_TYPE       =0x94,
MAXSIZE_OF_ATOMBIOS_ASIC_BUS_MEM_TYPE      =20,  /*including the terminator 0x0!*/
OFFSET_TO_GET_ATOMBIOS_NUMBER_OF_STRINGS   =0x2f,
OFFSET_TO_GET_ATOMBIOS_STRING_START        =0x6e,
};

/****************************************************************************
* Common header for all tables (Data table, Command function).
* Every table pointed in _ATOM_MASTER_DATA_TABLE has this common header.
* And the pointer actually points to this header.
****************************************************************************/
//IPCLEAN_END
typedef struct _atom_common_table_header {
  uint16_t structuresize;
  uint8_t  format_revision;   //mainly used for a hw function, when the parser is not backward compatible
  uint8_t  content_revision;  //change it when a data table has a structure change, or a hw function has a input/output parameter change
} ATOM_COMMON_TABLE_HEADER;
//IPCLEAN_START
/****************************************************************************
* Structure stores the ROM header.
****************************************************************************/
typedef struct _atom_rom_header_v2_2 {
  ATOM_COMMON_TABLE_HEADER table_header;
  uint8_t  atom_bios_string[4];        //enum atom_string_def atom_bios_string;     //Signature to distinguish between Atombios and non-atombios,
  uint16_t bios_segment_address;
  uint16_t protectedmodeoffset;
  uint16_t configfilenameoffset;
  uint16_t crc_block_offset;
  uint16_t vbios_bootupmessageoffset;
  uint16_t int10_offset;
  uint16_t pcibusdevinitcode;
  uint16_t iobaseaddress;
  uint16_t subsystem_vender_id;
  uint16_t subsystem_id;
  uint16_t pci_info_offset;
  uint16_t masterhwfunction_offset;      //Offest for SW to get all command function offsets, Don't change the position
  uint16_t masterdatatable_offset;       //Offest for SW to get all data table offsets, Don't change the position
  uint16_t reserved;
  uint32_t pspdirtableoffset;
} ATOM_ROM_HEADER_V2_2;

/*==============================hw function portion======================================================================*/


/****************************************************************************
* Structures used in Command.mtb, each function name is not given here since those function could change from time to time
* The real functionality of each function is associated with the parameter structure version when defined
* For all internal cmd function definitions, please reference to atomstruct.h
****************************************************************************/
typedef struct _atom_master_list_of_command_functions_v2_1 {
  uint16_t asic_init;                   //Function
  uint16_t cmd_function1;               //used as an internal one
  uint16_t cmd_function2;               //used as an internal one
  uint16_t cmd_function3;               //used as an internal one
  uint16_t digxencodercontrol;          //Function
  uint16_t cmd_function5;               //used as an internal one
  uint16_t cmd_function6;               //used as an internal one
  uint16_t cmd_function7;               //used as an internal one
  uint16_t cmd_function8;               //used as an internal one
  uint16_t cmd_function9;               //used as an internal one
  uint16_t setengineclock;              //Function
  uint16_t setmemoryclock;              //Function
  uint16_t setpixelclock;               //Function
  uint16_t enabledisppowergating;       //Function
  uint16_t cmd_function14;              //used as an internal one
  uint16_t cmd_function15;              //used as an internal one
  uint16_t cmd_function16;              //used as an internal one
  uint16_t cmd_function17;              //used as an internal one
  uint16_t cmd_function18;              //used as an internal one
  uint16_t cmd_function19;              //used as an internal one
  uint16_t cmd_function20;              //used as an internal one
  uint16_t cmd_function21;              //used as an internal one
  uint16_t cmd_function22;              //used as an internal one
  uint16_t cmd_function23;              //used as an internal one
  uint16_t cmd_function24;              //used as an internal one
  uint16_t cmd_function25;              //used as an internal one
  uint16_t cmd_function26;              //used as an internal one
  uint16_t cmd_function27;              //used as an internal one
  uint16_t cmd_function28;              //used as an internal one
  uint16_t cmd_function29;              //used as an internal one
  uint16_t cmd_function30;              //used as an internal one
  uint16_t cmd_function31;              //used as an internal one
  uint16_t cmd_function32;              //used as an internal one
  uint16_t cmd_function33;              //used as an internal one
  uint16_t blankcrtc;                   //Function
  uint16_t enablecrtc;                  //Function
  uint16_t cmd_function36;              //used as an internal one
  uint16_t cmd_function37;              //used as an internal one
  uint16_t cmd_function38;              //used as an internal one
  uint16_t cmd_function39;              //used as an internal one
  uint16_t cmd_function40;              //used as an internal one
  uint16_t getsmuclockinfo;             //Function
  uint16_t selectcrtc_source;           //Function
  uint16_t cmd_function43;              //used as an internal one
  uint16_t cmd_function44;              //used as an internal one
  uint16_t cmd_function45;              //used as an internal one
  uint16_t setdceclock;                 //Function
  uint16_t getmemoryclock;              //Function
  uint16_t getengineclock;              //Function
  uint16_t setcrtc_usingdtdtiming;      //Function
  uint16_t externalencodercontrol;      //Function
  uint16_t cmd_function51;              //used as an internal one
  uint16_t cmd_function52;              //used as an internal one
  uint16_t cmd_function53;              //used as an internal one
  uint16_t processi2cchanneltransaction;//Function
  uint16_t cmd_function55;              //used as an internal one
  uint16_t cmd_function56;              //used as an internal one
  uint16_t cmd_function57;              //used as an internal one
  uint16_t cmd_function58;              //used as an internal one
  uint16_t cmd_function59;              //used as an internal one
  uint16_t computegpuclockparam;        //Function
  uint16_t gfx_init;                    //used as an internal one
  uint16_t cmd_function62;              //used as an internal one
  uint16_t dynamicmemorysettings;       //Function function
  uint16_t memorytraining;              //Function function
  uint16_t cmd_function65;              //used as an internal one
  uint16_t cmd_function66;              //used as an internal one
  uint16_t setvoltage;                  //Function
  uint16_t cmd_function68;              //used as an internal one
  uint16_t readefusevalue;              //Function
  uint16_t cmd_function70;              //used as an internal one
  uint16_t cmd_function71;              //used as an internal one
  uint16_t cmd_function72;              //used as an internal one
  uint16_t cmd_function73;              //used as an internal one
  uint16_t cmd_function74;              //used as an internal one
  uint16_t cmd_function75;              //used as an internal one
  uint16_t dig1transmittercontrol;      //Function
  uint16_t cmd_function77;              //used as an internal one
  uint16_t processauxchanneltransaction;//Function
  uint16_t cmd_function79;              //used as an internal one
  uint16_t getvoltageinfo;              //Function
} ATOM_MASTER_LIST_OF_COMMAND_FUNCTIONS_V2_1;



typedef struct _atom_master_command_function_v2_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  ATOM_MASTER_LIST_OF_COMMAND_FUNCTIONS_V2_1 listofcmdfunctions;
} ATOM_MASTER_COMMAND_FUNCTION_V2_1;

/****************************************************************************
* Structures used in every command function
****************************************************************************/
typedef struct _atom_function_attribute
{
  uint16_t  ws_in_bytes:8;            //[7:0]=Size of workspace in Bytes (in multiple of a dword),
  uint16_t  ps_in_bytes:7;            //[14:8]=Size of parameter space in Bytes (multiple of a dword),
  uint16_t  updated_by_util:1;        //[15]=flag to indicate the function is updated by util
} ATOM_FUNCTION_ATTRIBUTE;


/****************************************************************************
* Common header for all hw functions.
* Every function pointed by _master_list_of_hw_function has this common header.
* And the pointer actually points to this header.
****************************************************************************/
typedef struct _atom_rom_hw_function_header
{
  ATOM_COMMON_TABLE_HEADER func_header;
  ATOM_FUNCTION_ATTRIBUTE func_attrib;
} ATOM_ROM_HW_FUNCTION_HEADER;


/*==============================sw data table portion======================================================================*/
/****************************************************************************
* Structures used in data.mtb, each data table name is not given here since those data table could change from time to time
* The real name of each table is given when its data structure version is defined
****************************************************************************/
typedef struct _atom_master_list_of_data_tables_v2_1{
  uint16_t utilitypipeline;               /* Offest for the utility to get parser info,Don't change this position!*/
  uint16_t multimedia_info;
  uint16_t smc_dpm_info;
  uint16_t sw_datatable3;
  uint16_t firmwareinfo;                  /* Shared by various SW components */
  uint16_t sw_datatable5;
  uint16_t lcd_info;                      /* Shared by various SW components */
  uint16_t sw_datatable7;
  uint16_t smu_info;
  uint16_t sw_datatable9;
  uint16_t sw_datatable10;
  uint16_t vram_usagebyfirmware;          /* Shared by various SW components */
  uint16_t gpio_pin_lut;                  /* Shared by various SW components */
  uint16_t sw_datatable13;
  uint16_t gfx_info;
  uint16_t powerplayinfo;                 /* Shared by various SW components */
  uint16_t sw_datatable16;
  uint16_t sw_datatable17;
  uint16_t sw_datatable18;
  uint16_t sw_datatable19;
  uint16_t sw_datatable20;
  uint16_t sw_datatable21;
  uint16_t displayobjectinfo;             /* Shared by various SW components */
  uint16_t indirectioaccess;        /* used as an internal one */
  uint16_t umc_info;                      /* Shared by various SW components */
  uint16_t sw_datatable25;
  uint16_t sw_datatable26;
  uint16_t dce_info;                      /* Shared by various SW components */
  uint16_t vram_info;                     /* Shared by various SW components */
  uint16_t sw_datatable29;
  uint16_t integratedsysteminfo;          /* Shared by various SW components */
  uint16_t asic_profiling_info;           /* Shared by various SW components */
  uint16_t voltageobject_info;            /* shared by various SW components */
  uint16_t sw_datatable33;
  uint16_t sw_datatable34;
} ATOM_MASTER_LIST_OF_DATA_TABLES_V2_1;


typedef struct _atom_master_data_table_v2_1 {
  ATOM_COMMON_TABLE_HEADER table_header;
  ATOM_MASTER_LIST_OF_DATA_TABLES_V2_1 listOfdatatables;
} ATOM_MASTER_DATA_TABLE_V2_1;


typedef struct _atom_dtd_format {
  uint16_t  pixclk;
  uint16_t  h_active;
  uint16_t  h_blanking_time;
  uint16_t  v_active;
  uint16_t  v_blanking_time;
  uint16_t  h_sync_offset;
  uint16_t  h_sync_width;
  uint16_t  v_sync_offset;
  uint16_t  v_syncwidth;
  uint16_t  reserved;
  uint16_t  reserved0;
  uint8_t   h_border;
  uint8_t   v_border;
  uint16_t  miscinfo;
  uint8_t   atom_mode_id;
  uint8_t   refreshrate;
} ATOM_DTD_FORMAT;

/* atom_dtd_format.modemiscinfo defintion */
typedef enum {
  ATOM_HSYNC_POLARITY    = 0x0002,
  ATOM_VSYNC_POLARITY    = 0x0004,
  ATOM_H_REPLICATIONBY2  = 0x0010,
  ATOM_V_REPLICATIONBY2  = 0x0020,
  ATOM_INTERLACE         = 0x0080,
  ATOM_COMPOSITESYNC     = 0x0040,
} ATOM_DTD_FORMAT_MODEMISCINFO;


/* utilitypipeline
 * when format_revision==1 && content_revision==1, then this an info table for atomworks to use during debug session, no structure is associated with it.
 * the location of it can't change
*/


/*
  ***************************************************************************
    Data Table firmwareinfo  structure
  ***************************************************************************
*/

typedef struct _atom_firmware_info_v3_1 {
  ATOM_COMMON_TABLE_HEADER table_header;
  uint32_t firmware_revision;
  uint32_t bootup_sclk_in10khz;
  uint32_t bootup_mclk_in10khz;
  uint32_t firmware_capability;             // enum atombios_firmware_capability
  uint32_t main_call_parser_entry;          /* direct address of main parser call in VBIOS binary. */
  uint32_t bios_scratch_reg_startaddr;      // 1st bios scratch register dword address
  uint16_t bootup_vddc_mv;
  uint16_t bootup_vddci_mv;
  uint16_t bootup_mvddc_mv;
  uint16_t bootup_vddgfx_mv;
  uint8_t  mem_module_id;
  uint8_t  coolingsolution_id;              /*0: Air cooling; 1: Liquid cooling ... */
  uint8_t  reserved1[2];
  uint32_t mc_baseaddr_high;
  uint32_t mc_baseaddr_low;
  uint8_t  board_i2c_feature_id;            // enum of atom_board_i2c_feature_id_def
  uint8_t  board_i2c_feature_gpio_id;       // i2c id find in gpio_lut data table gpio_id
  uint8_t  board_i2c_feature_slave_addr;
  uint8_t  reserved3;
  uint32_t reserved2[5];
} ATOM_FIRMWARE_INFO_V3_1;
//IPCLEAN_END
/* Total 32bit cap indication */
typedef enum {
  ATOM_FIRMWARE_CAP_FIRMWARE_POSTED = 0x00000001,
  ATOM_FIRMWARE_CAP_GPU_VIRTUALIZATION  = 0x00000002,
  ATOM_FIRMWARE_CAP_SUGARLOAF_WORKAROUND_EN = 0x00000004,
  ATOM_FIRMWARE_CAP_WMI_SUPPORT  = 0x00000040,
  ATOM_FIRMWARE_CAP_HWEMU_ENABLE  = 0x00000080,
  ATOM_FIRMWARE_CAP_HWEMU_UMC_CFG = 0x00000100,
  ATOM_FIRMWARE_CAP_SRAM_ECC      = 0x00000200,
  ATOM_FIRMWARE_CAP_ENABLE_2STAGE_BIST_TRAINING  = 0x00000400,
} ATOMBIOS_FIRMWARE_CAPABILITY;

typedef enum {
  AIR_COOLING    = 0x00,
  LIQUID_COOLING = 0x01
} ATOM_COOLING_SOLUTION_ID;

typedef enum {
  NOT_PRESENT    = 0x0,
  REMOTE_MGMT    = 0x1,
  EXT_APP        = 0x2
} ATOM_BOARD_I2C_FEATURE_ID_DEF;


typedef struct _atom_firmware_info_v3_2
{
  ATOM_COMMON_TABLE_HEADER table_header;
  uint32_t firmware_revision;
  uint32_t bootup_sclk_in10khz;
  uint32_t bootup_mclk_in10khz;
  uint32_t firmware_capability;             // enum atombios_firmware_capability
  uint32_t main_call_parser_entry;          /* direct address of main parser call in VBIOS binary. */
  uint32_t bios_scratch_reg_startaddr;      // 1st bios scratch register dword address
  uint16_t bootup_vddc_mv;
  uint16_t bootup_vddci_mv;
  uint16_t bootup_mvddc_mv;
  uint16_t bootup_vddgfx_mv;
  uint8_t  mem_module_id;
  uint8_t  coolingsolution_id;              /*0: Air cooling; 1: Liquid cooling ... */
  uint8_t  reserved1[2];
  uint32_t mc_baseaddr_high;
  uint32_t mc_baseaddr_low;
  uint8_t  board_i2c_feature_id;            // enum of atom_board_i2c_feature_id_def
  uint8_t  board_i2c_feature_gpio_id;       // i2c id find in gpio_lut data table gpio_id
  uint8_t  board_i2c_feature_slave_addr;
  uint8_t  reserved3;
  uint16_t bootup_mvddq_mv;
  uint16_t bootup_mvpp_mv;
  uint32_t zfbstartaddrin16mb;
  uint32_t reserved2[3];
} ATOM_FIRMWARE_INFO_V3_2;

//IPCLEAN_START
typedef struct _atom_firmware_info_v3_3
{
  ATOM_COMMON_TABLE_HEADER table_header;
  uint32_t firmware_revision;
  uint32_t bootup_sclk_in10khz;
  uint32_t bootup_mclk_in10khz;
  uint32_t firmware_capability;             // enum atombios_firmware_capability
  uint32_t main_call_parser_entry;          /* direct address of main parser call in VBIOS binary. */
  uint32_t bios_scratch_reg_startaddr;      // 1st bios scratch register dword address
  uint16_t bootup_vddc_mv;
  uint16_t bootup_vddci_mv;
  uint16_t bootup_mvddc_mv;
  uint16_t bootup_vddgfx_mv;
  uint8_t  mem_module_id;
  uint8_t  coolingsolution_id;              /*0: Air cooling; 1: Liquid cooling ... */
  uint8_t  reserved1[2];
  uint32_t mc_baseaddr_high;
  uint32_t mc_baseaddr_low;
  uint8_t  board_i2c_feature_id;            // enum of atom_board_i2c_feature_id_def
  uint8_t  board_i2c_feature_gpio_id;       // i2c id find in gpio_lut data table gpio_id
  uint8_t  board_i2c_feature_slave_addr;
  uint8_t  reserved3;
  uint16_t bootup_mvddq_mv;
  uint16_t bootup_mvpp_mv;
  uint32_t zfbstartaddrin16mb;
  uint32_t pplib_pptable_id;                // if pplib_pptable_id!=0, pplib get powerplay table inside driver instead of from VBIOS
  uint32_t mvdd_ratio;                      // mvdd_raio = (real mvdd in power rail)*1000/(mvdd_output_from_svi2)
  uint16_t hw_bootup_vddgfx_mv;             // hw default vddgfx voltage level decide by board strap
  uint16_t hw_bootup_vddc_mv;               // hw default vddc voltage level decide by board strap
  uint16_t hw_bootup_mvddc_mv;              // hw default mvddc voltage level decide by board strap
  uint16_t hw_bootup_vddci_mv;              // hw default vddci voltage level decide by board strap
  uint32_t reserved[8];
} ATOM_FIRMWARE_INFO_V3_3;



/*
  ***************************************************************************
    Data Table lcd_info  structure
  ***************************************************************************
*/

typedef struct _lcd_info_v2_1 {
  ATOM_COMMON_TABLE_HEADER table_header;
  ATOM_DTD_FORMAT  lcd_timing;
  uint16_t backlight_pwm;
  uint16_t special_handle_cap;
  uint16_t panel_misc;
  uint16_t lvds_max_slink_pclk;
  uint16_t lvds_ss_percentage;
  uint16_t lvds_ss_rate_10hz;
  uint8_t  pwr_on_digon_to_de;          /*all pwr sequence numbers below are in uint of 4ms*/
  uint8_t  pwr_on_de_to_vary_bl;
  uint8_t  pwr_down_vary_bloff_to_de;
  uint8_t  pwr_down_de_to_digoff;
  uint8_t  pwr_off_delay;
  uint8_t  pwr_on_vary_bl_to_blon;
  uint8_t  pwr_down_bloff_to_vary_bloff;
  uint8_t  panel_bpc;
  uint8_t  dpcd_edp_config_cap;
  uint8_t  dpcd_max_link_rate;
  uint8_t  dpcd_max_lane_count;
  uint8_t  dpcd_max_downspread;
  uint8_t  min_allowed_bl_level;
  uint8_t  max_allowed_bl_level;
  uint8_t  bootup_bl_level;
  uint8_t  dplvdsrxid;
  uint32_t reserved1[8];
} LCD_INFO_V2_1;

/* lcd_info_v2_1.panel_misc defintion */
typedef enum {
  ATOM_PANEL_MISC_FPDI            =0x0002,
} ATOM_LCD_INFO_PANEL_MISC;

//uceDPToLVDSRxId
typedef enum {
  eDP_TO_LVDS_RX_DISABLE                 = 0x00,       // no eDP->LVDS translator chip
  eDP_TO_LVDS_COMMON_ID                  = 0x01,       // common eDP->LVDS translator chip without AMD SW init
  eDP_TO_LVDS_REALTEK_ID                 = 0x02,       // Realtek tansaltor which require AMD SW init
} ATOM_LCD_INFO_DPTOLVDS_RX_ID;


/*
  ***************************************************************************
    Data Table gpio_pin_lut  structure
  ***************************************************************************
*/

typedef struct _atom_gpio_pin_assignment {
  uint32_t data_a_reg_index;
  uint8_t  gpio_bitshift;
  uint8_t  gpio_mask_bitshift;
  uint8_t  gpio_id;
  uint8_t  reserved;
} ATOM_GPIO_PIN_ASSIGNMENT;

/* atom_gpio_pin_assignment.gpio_id definition */
typedef enum {
  I2C_HW_LANE_MUX        =0x0f, /* only valid when bit7=1 */
  I2C_HW_ENGINE_ID_MASK  =0x70, /* only valid when bit7=1 */
  I2C_HW_CAP             =0x80, /*only when the I2C_HW_CAP is set, the pin ID is assigned to an I2C pin pair, otherwise, it's an generic GPIO pin */

  /* gpio_id pre-define id for multiple usage */
  /* GPIO use to control PCIE_VDDC in certain SLT board */
  PCIE_VDDC_CONTROL_GPIO_PINID = 56,
  /* if PP_AC_DC_SWITCH_GPIO_PINID in Gpio_Pin_LutTable, AC/DC swithing feature is enable */
  PP_AC_DC_SWITCH_GPIO_PINID = 60,
  /* VDDC_REGULATOR_VRHOT_GPIO_PINID in Gpio_Pin_LutTable, VRHot feature is enable */
  VDDC_VRHOT_GPIO_PINID = 61,
  /*if VDDC_PCC_GPIO_PINID in GPIO_LUTable, Peak Current Control feature is enabled */
  VDDC_PCC_GPIO_PINID = 62,
  /* Only used on certain SLT/PA board to allow utility to cut Efuse. */
  EFUSE_CUT_ENABLE_GPIO_PINID = 63,
  /* ucGPIO=DRAM_SELF_REFRESH_GPIO_PIND uses  for memory self refresh (ucGPIO=0, DRAM self-refresh; ucGPIO= */
  DRAM_SELF_REFRESH_GPIO_PINID = 64,
  /* Thermal interrupt output->system thermal chip GPIO pin */
  THERMAL_INT_OUTPUT_GPIO_PINID =65,
} ATOM_GPIO_PIN_ASSIGNMENT_GPIO_ID;


typedef struct _atom_gpio_pin_lut_v2_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  /*the real number of this included in the structure is calcualted by using the (whole structure size - the header size)/size of atom_gpio_pin_lut  */
  ATOM_GPIO_PIN_ASSIGNMENT  gpio_pin[8];
} ATOM_GPIO_PIN_LUT_V2_1;


/*
  ***************************************************************************
    Data Table vram_usagebyfirmware  structure
  ***************************************************************************
*/

/*
Below definition is set by the GPUV in start_address_in_kb to resue used_by_firmware_in_kb
for exchanging info between PF and VFs.

*/
#ifndef PSP_VBIOS_H2INC
#ifndef _H2INC
typedef enum {
  ATOM_VRAM_BLOCK_SRIOV_MSG_SHARE_RESERVATION = 0x80000000,
} ATOMBIOS_VRAMUSAGE_ID;
#endif
#endif


typedef struct _vram_usagebyfirmware_v2_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t  start_address_in_kb;
  uint16_t  used_by_firmware_in_kb;
  uint16_t  used_by_driver_in_kb;
} VRAM_USAGEBYFIRMWARE_V2_1;


/*
  ***************************************************************************
    Data Table displayobjectinfo  structure
  ***************************************************************************
*/

typedef enum {
  ATOM_I2C_RECORD_TYPE =1,
  ATOM_HPD_INT_RECORD_TYPE =2,
  ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE =9,
  ATOM_CONNECTOR_HPDPIN_LUT_RECORD_TYPE =16,
  ATOM_CONNECTOR_AUXDDC_LUT_RECORD_TYPE =17,
  ATOM_ENCODER_CAP_RECORD_TYPE=20,
  ATOM_BRACKET_LAYOUT_RECORD_TYPE=21,
  ATOM_CONNECTOR_FORCED_TMDS_CAP_RECORD_TYPE=22,
  ATOM_RECORD_END_TYPE  =0xFF,
} ATOM_OBJECT_RECORD_TYPE_ID;

typedef struct _atom_common_record_header
{
  uint8_t record_type;                      //An emun to indicate the record type
  uint8_t record_size;                      //The size of the whole record in byte
} ATOM_COMMON_RECORD_HEADER;

typedef struct _atom_i2c_record
{
  ATOM_COMMON_RECORD_HEADER record_header;   //record_type = ATOM_I2C_RECORD_TYPE
  uint8_t i2c_id;
  uint8_t i2c_slave_addr;                   //The slave address, it's 0 when the record is attached to connector for DDC
} ATOM_I2C_RECORD;

typedef struct _atom_hpd_int_record
{
  ATOM_COMMON_RECORD_HEADER record_header;  //record_type = ATOM_HPD_INT_RECORD_TYPE
  uint8_t  pin_id;              //Corresponding block in GPIO_PIN_INFO table gives the pin info
  uint8_t  plugin_pin_state;
} ATOM_HPD_INT_RECORD;

// Bit maps for ATOM_ENCODER_CAP_RECORD.usEncoderCap
typedef enum _atom_encoder_caps_def {
  ATOM_ENCODER_CAP_RECORD_HBR2                  =0x01,         // DP1.2 HBR2 is supported by HW encoder, it is retired in NI. the real meaning from SI is MST_EN
  ATOM_ENCODER_CAP_RECORD_MST_EN                =0x01,         // from SI, this bit means DP MST is enable or not.
  ATOM_ENCODER_CAP_RECORD_HBR2_EN               =0x02,         // DP1.2 HBR2 setting is qualified and HBR2 can be enabled
  ATOM_ENCODER_CAP_RECORD_HDMI6Gbps_EN          =0x04,         // HDMI2.0 6Gbps enable or not.
  ATOM_ENCODER_CAP_RECORD_HBR3_EN               =0x08,         // DP1.3 HBR3 is supported by board.
  ATOM_ENCODER_CAP_RECORD_USB_C_TYPE            =0x100,        // the DP connector is a USB-C type.
} ATOM_ENCODER_CAPS_DEF;

typedef struct _atom_encoder_caps_record {
  ATOM_COMMON_RECORD_HEADER record_header;  //record_type = ATOM_ENCODER_CAP_RECORD_TYPE
  uint32_t  encodercaps;
} ATOM_ENCODER_CAPS_RECORD;

typedef enum {
  ATOM_CONNECTOR_CAP_INTERNAL_DISPLAY         = 0x01,        //a cap bit to indicate that this non-embedded display connector is an internal display
  ATOM_CONNECTOR_CAP_INTERNAL_DISPLAY_BL      = 0x02,        //a cap bit to indicate that this internal display requires BL control from GPU, refers to lcd_info for BL PWM freq
} ATOM_CONNECTOR_CAPS_DEF;

typedef struct _atom_disp_connector_caps_record {
  ATOM_COMMON_RECORD_HEADER record_header;
  uint32_t connectcaps;
} ATOM_DISP_CONNECTOR_CAPS_RECORD;

//The following generic object gpio pin control record type will replace JTAG_RECORD/FPGA_CONTROL_RECORD/DVI_EXT_INPUT_RECORD above gradually
typedef struct _atom_gpio_pin_control_pair {
  uint8_t gpio_id;               // GPIO_ID, find the corresponding ID in GPIO_LUT table
  uint8_t gpio_pinstate;         // Pin state showing how to set-up the pin
} ATOM_GPIO_PIN_CONTROL_PAIR;

typedef struct _atom_object_gpio_cntl_record {
  ATOM_COMMON_RECORD_HEADER record_header;
  uint8_t flag;                   // Future expnadibility
  uint8_t number_of_pins;         // Number of GPIO pins used to control the object
  ATOM_GPIO_PIN_CONTROL_PAIR gpio[1];              // the real gpio pin pair determined by number of pins ucNumberOfPins
} ATOM_OBJECT_GPIO_CNTL_RECORD;

//Definitions for GPIO pin state
typedef enum {
  GPIO_PIN_TYPE_INPUT             = 0x00,
  GPIO_PIN_TYPE_OUTPUT            = 0x10,
  GPIO_PIN_TYPE_HW_CONTROL        = 0x20,

//For GPIO_PIN_TYPE_OUTPUT the following is defined
  GPIO_PIN_OUTPUT_STATE_MASK      = 0x01,
  GPIO_PIN_OUTPUT_STATE_SHIFT     = 0,
  GPIO_PIN_STATE_ACTIVE_LOW       = 0x0,
  GPIO_PIN_STATE_ACTIVE_HIGH      = 0x1,
} ATOM_GPIO_PIN_CONTROL_PINSTATE_DEF;

// Indexes to GPIO array in GLSync record
// GLSync record is for Frame Lock/Gen Lock feature.
typedef enum {
  ATOM_GPIO_INDEX_GLSYNC_REFCLK    = 0,
  ATOM_GPIO_INDEX_GLSYNC_HSYNC     = 1,
  ATOM_GPIO_INDEX_GLSYNC_VSYNC     = 2,
  ATOM_GPIO_INDEX_GLSYNC_SWAP_REQ  = 3,
  ATOM_GPIO_INDEX_GLSYNC_SWAP_GNT  = 4,
  ATOM_GPIO_INDEX_GLSYNC_INTERRUPT = 5,
  ATOM_GPIO_INDEX_GLSYNC_V_RESET   = 6,
  ATOM_GPIO_INDEX_GLSYNC_SWAP_CNTL = 7,
  ATOM_GPIO_INDEX_GLSYNC_SWAP_SEL  = 8,
  ATOM_GPIO_INDEX_GLSYNC_MAX       = 9,
} ATOM_GLSYNC_RECORD_GPIO_INDEX_DEF;


typedef struct _atom_connector_hpdpin_lut_record     //record for ATOM_CONNECTOR_HPDPIN_LUT_RECORD_TYPE
{
  ATOM_COMMON_RECORD_HEADER record_header;
  uint8_t hpd_pin_map[8];
} ATOM_CONNECTOR_HPDPIN_LUT_RECORD;

typedef struct _atom_connector_auxddc_lut_record     //record for ATOM_CONNECTOR_AUXDDC_LUT_RECORD_TYPE
{
  ATOM_COMMON_RECORD_HEADER record_header;
  uint8_t aux_ddc_map[8];
} ATOM_CONNECTOR_AUXDDC_LUT_RECORD;

typedef struct _atom_connector_forced_tmds_cap_record {
  ATOM_COMMON_RECORD_HEADER record_header;
  // override TMDS capability on this connector when it operate in TMDS mode.  usMaxTmdsClkRate = max TMDS Clock in Mhz/2.5
  uint8_t  maxtmdsclkrate_in2_5mhz;
  uint8_t  reserved;
} ATOM_CONNECTOR_FORCED_TMDS_CAP_RECORD;

typedef struct _atom_connector_layout_info {
  uint16_t connectorobjid;
  uint8_t  connector_type;
  uint8_t  position;
} ATOM_CONNECTOR_LAYOUT_INFO;

// define ATOM_CONNECTOR_LAYOUT_INFO.ucConnectorType to describe the display connector size
typedef enum {
  CONNECTOR_TYPE_DVI_D                 = 1,

  CONNECTOR_TYPE_HDMI                  = 4,
  CONNECTOR_TYPE_DISPLAY_PORT          = 5,
  CONNECTOR_TYPE_MINI_DISPLAY_PORT     = 6,
} ATOM_CONNECTOR_LAYOUT_INFO_CONNECTOR_TYPE_DEF;

typedef struct _atom_bracket_layout_record {
  ATOM_COMMON_RECORD_HEADER record_header;
  uint8_t bracketlen;
  uint8_t bracketwidth;
  uint8_t conn_num;
  uint8_t reserved;
  ATOM_CONNECTOR_LAYOUT_INFO  conn_info[1];
} ATOM_BRACKET_LAYOUT_RECORD;

typedef enum {
  ATOM_DISPLAY_LCD1_SUPPORT            = 0x0002,  //an embedded display is either an LVDS or eDP signal type of display
  ATOM_DISPLAY_DFP1_SUPPORT            = 0x0008,
  ATOM_DISPLAY_DFP2_SUPPORT            = 0x0080,
  ATOM_DISPLAY_DFP3_SUPPORT            = 0x0200,
  ATOM_DISPLAY_DFP4_SUPPORT            = 0x0400,
  ATOM_DISPLAY_DFP5_SUPPORT            = 0x0800,
  ATOM_DISPLAY_DFP6_SUPPORT            = 0x0040,
  ATOM_DISPLAY_DFPx_SUPPORT            = 0x0ec8,
} ATOM_DISPLAY_DEVICE_TAG_DEF;

typedef struct _atom_display_object_path_v2 {
  uint16_t display_objid;                  //Connector Object ID or Misc Object ID
  uint16_t disp_recordoffset;
  uint16_t encoderobjid;                   //first encoder closer to the connector, could be either an external or intenal encoder
  uint16_t extencoderobjid;                //2nd encoder after the first encoder, from the connector point of view;
  uint16_t encoder_recordoffset;
  uint16_t extencoder_recordoffset;
  uint16_t device_tag;                     //a supported device vector, each display path starts with this.the paths are enumerated in the way of priority, a path appears first
  uint8_t  priority_id;
  uint8_t  reserved;
} ATOM_DISPLAY_OBJECT_PATH_V2;

typedef struct _display_object_info_table_v1_4
{
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint16_t  supporteddevices;
  uint8_t   number_of_path;
  uint8_t   reserved;
  ATOM_DISPLAY_OBJECT_PATH_V2 display_path[8];   //the real number of this included in the structure is calculated by using the (whole structure size - the header size- number_of_path)/size of atom_display_object_path
} DISPLAY_OBJECT_INFO_TABLE_V1_4;


/*
  ***************************************************************************
    Data Table dce_info  structure
  ***************************************************************************
*/
typedef struct _atom_display_controller_info_v4_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t display_caps;
  uint32_t bootup_dispclk_10khz;
  uint16_t dce_refclk_10khz;
  uint16_t i2c_engine_refclk_10khz;
  uint16_t dvi_ss_percentage;       // in unit of 0.001%
  uint16_t dvi_ss_rate_10hz;
  uint16_t hdmi_ss_percentage;      // in unit of 0.001%
  uint16_t hdmi_ss_rate_10hz;
  uint16_t dp_ss_percentage;        // in unit of 0.001%
  uint16_t dp_ss_rate_10hz;
  uint8_t  dvi_ss_mode;             // enum of atom_spread_spectrum_mode
  uint8_t  hdmi_ss_mode;            // enum of atom_spread_spectrum_mode
  uint8_t  dp_ss_mode;              // enum of atom_spread_spectrum_mode
  uint8_t  ss_reserved;
  uint8_t  hardcode_mode_num;       // a hardcode mode number defined in StandardVESA_TimingTable when a CRT or DFP EDID is not available
  uint8_t  reserved1[3];
  uint16_t dpphy_refclk_10khz;
  uint8_t  dpphy_refclk_srcsel;     // =0: XTALIN =1: PCIE =2: GENLK, =3: REFPAD
  uint8_t  reserved2;
  uint8_t  dceip_min_ver;
  uint8_t  dceip_max_ver;
  uint8_t  max_disp_pipe_num;
  uint8_t  max_vbios_active_disp_pipe_num;
  uint8_t  max_ppll_num;
  uint8_t  max_disp_phy_num;
  uint8_t  max_aux_pairs;
  uint8_t  remotedisplayconfig;
  uint16_t dc_golden_offset;
  uint16_t dcpipe_golden_offset;
  uint8_t  reserved3[4];
} ATOM_DISPLAY_CONTROLLER_INFO_V4_1;


typedef struct _atom_display_controller_info_v4_2
{
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t display_caps;
  uint32_t bootup_dispclk_10khz;
  uint16_t dce_refclk_10khz;
  uint16_t i2c_engine_refclk_10khz;
  uint16_t dvi_ss_percentage;       // in unit of 0.001%
  uint16_t dvi_ss_rate_10hz;
  uint16_t hdmi_ss_percentage;      // in unit of 0.001%
  uint16_t hdmi_ss_rate_10hz;
  uint16_t dp_ss_percentage;        // in unit of 0.001%
  uint16_t dp_ss_rate_10hz;
  uint8_t  dvi_ss_mode;             // enum of atom_spread_spectrum_mode
  uint8_t  hdmi_ss_mode;            // enum of atom_spread_spectrum_mode
  uint8_t  dp_ss_mode;              // enum of atom_spread_spectrum_mode
  uint8_t  ss_reserved;
  uint8_t  dfp_hardcode_mode_num;   // DFP hardcode mode number defined in StandardVESA_TimingTable when EDID is not available
  uint8_t  dfp_hardcode_refreshrate;// DFP hardcode mode refreshrate defined in StandardVESA_TimingTable when EDID is not available
  uint8_t  vga_hardcode_mode_num;   // VGA hardcode mode number defined in StandardVESA_TimingTable when EDID is not avablable
  uint8_t  vga_hardcode_refreshrate;// VGA hardcode mode number defined in StandardVESA_TimingTable when EDID is not avablable
  uint16_t dpphy_refclk_10khz;
  uint16_t reserved2;
  uint8_t  dcnip_min_ver;
  uint8_t  dcnip_max_ver;
  uint8_t  max_disp_pipe_num;
  uint8_t  max_vbios_active_disp_pipe_num;
  uint8_t  max_ppll_num;
  uint8_t  max_disp_phy_num;
  uint8_t  max_aux_pairs;
  uint8_t  remotedisplayconfig;
  uint8_t  reserved3[8];
} ATOM_DISPLAY_CONTROLLER_INFO_V4_2;

typedef struct _atom_display_controller_info_v4_3
{
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t display_caps;
  uint32_t bootup_dispclk_10khz;
  uint16_t dce_refclk_10khz;
  uint16_t i2c_engine_refclk_10khz;
  uint16_t dvi_ss_percentage;       // in unit of 0.001%
  uint16_t dvi_ss_rate_10hz;
  uint16_t hdmi_ss_percentage;      // in unit of 0.001%
  uint16_t hdmi_ss_rate_10hz;
  uint16_t dp_ss_percentage;        // in unit of 0.001%
  uint16_t dp_ss_rate_10hz;
  uint8_t  dvi_ss_mode;             // enum of atom_spread_spectrum_mode
  uint8_t  hdmi_ss_mode;            // enum of atom_spread_spectrum_mode
  uint8_t  dp_ss_mode;              // enum of atom_spread_spectrum_mode
  uint8_t  ss_reserved;
  uint8_t  dfp_hardcode_mode_num;   // DFP hardcode mode number defined in StandardVESA_TimingTable when EDID is not available
  uint8_t  dfp_hardcode_refreshrate;// DFP hardcode mode refreshrate defined in StandardVESA_TimingTable when EDID is not available
  uint8_t  vga_hardcode_mode_num;   // VGA hardcode mode number defined in StandardVESA_TimingTable when EDID is not avablable
  uint8_t  vga_hardcode_refreshrate;// VGA hardcode mode number defined in StandardVESA_TimingTable when EDID is not avablable
  uint16_t dpphy_refclk_10khz;
  uint16_t reserved2;
  uint8_t  dcnip_min_ver;
  uint8_t  dcnip_max_ver;
  uint8_t  max_disp_pipe_num;
  uint8_t  max_vbios_active_disp_pipe_num;
  uint8_t  max_ppll_num;
  uint8_t  max_disp_phy_num;
  uint8_t  max_aux_pairs;
  uint8_t  remotedisplayconfig;
  uint8_t  reserved3[8];
} ATOM_DISPLAY_CONTROLLER_INFO_V4_3;


typedef enum {
  // only for VBIOS
  DCE_INFO_CAPS_FORCE_DISPDEV_CONNECTED  =0x02,
  // only for VBIOS
  DCE_INFO_CAPS_DISABLE_DFP_DP_HBR2      =0x04,
  // only for VBIOS
  DCE_INFO_CAPS_ENABLE_INTERLAC_TIMING   =0x08,

} DCE_INFO_CAPS_DEF;

/*
  ***************************************************************************
    Data Table ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO  structure
  ***************************************************************************
*/
typedef struct _ext_display_path
{
  uint16_t  usDeviceTag;                      //A bit vector to show what devices are supported
  uint16_t  usDeviceACPIEnum;                //16bit device ACPI id.
  uint16_t  usDeviceConnector;                  //A physical connector for displays to plug in, using object connector definitions
  uint8_t   ucExtAUXDDCLutIndex;                 //An index into external AUX/DDC channel LUT
  uint8_t   ucExtHPDPINLutIndex;                    //An index into external HPD pin LUT
  uint16_t  usExtEncoderObjId;               //external encoder object id
  uint8_t   ucChannelMapping;                  // if ucChannelMapping=0, using default one to one mapping
  uint8_t   ucChPNInvert;                      // bit vector for up to 8 lanes, =0: P and N is not invert, =1 P and N is inverted
  uint16_t  usCaps;
  uint16_t  usReserved;
} EXT_DISPLAY_PATH;

//usCaps
typedef enum {
  EXT_DISPLAY_PATH_CAPS__HBR2_DISABLE               =0x0001,
  EXT_DISPLAY_PATH_CAPS__DP_FIXED_VS_EN             =0x0002,
  EXT_DISPLAY_PATH_CAPS__EXT_CHIP_MASK              =0x007C,
  EXT_DISPLAY_PATH_CAPS__HDMI20_PI3EQX1204          =(0x01 << 2 ),     //PI redriver chip
  EXT_DISPLAY_PATH_CAPS__HDMI20_TISN65DP159RSBT     =(0x02 << 2 ),     //TI retimer chip
  EXT_DISPLAY_PATH_CAPS__HDMI20_PARADE_PS175        =(0x03 << 2 ),     //Parade DP->HDMI recoverter chip
  EXT_DISPLAY_PATH_CAPS__HBR3_DISABLE               =0x0080,
  EXT_DISPLAY_PATH_CAPS__USB_C_TYPE                 =0x100,   // the DP connector is a USB-C type.
  EXT_DISPLAY_PATH_CAPS__HDMI20_DISABLE             =0x200,   // HDMI2.0 6GBPS disable
} EXT_DISPLAY_PATH_CAP_DEF;

typedef struct _atom_external_display_connection_info {
  ATOM_COMMON_TABLE_HEADER  sHeader;
  uint8_t                  ucGuid[16];                                  // a GUID is a 16 byte long string
  EXT_DISPLAY_PATH         sPath[7];                                   // total of fixed 7 entries.
  uint8_t                  ucChecksum;                                  // a simple Checksum of the sum of whole structure equal to 0x0.
  uint8_t                  stereopinid;                               // use for eDP panel
  uint8_t                  ucRemoteDisplayConfig;
  uint8_t                  uceDPToLVDSRxId;
  uint8_t                  ucFixDPVoltageSwing;                         // usCaps[1]=1, this indicate DP_LANE_SET value
  uint8_t                  reserved[3];                               // for potential expansion
} ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO;

/*
  ***************************************************************************
    Data Table integratedsysteminfo  structure
  ***************************************************************************
*/

typedef struct _atom_camera_dphy_timing_param {
  uint8_t  profile_id;       // SENSOR_PROFILES
  uint32_t param;
} ATOM_CAMERA_DPHY_TIMING_PARAM;

typedef struct _atom_camera_dphy_elec_param {
  uint16_t param[3];
} ATOM_CAMERA_DPHY_ELEC_PARAM;

typedef struct _atom_camera_module_info {
  uint8_t module_id;                    // 0: Rear, 1: Front right of user, 2: Front left of user
  uint8_t module_name[8];
  ATOM_CAMERA_DPHY_TIMING_PARAM timingparam[6]; // Exact number is under estimation and confirmation from sensor vender
} ATOM_CAMERA_MODULE_INFO;

typedef struct _atom_camera_flashlight_info {
  uint8_t flashlight_id;                // 0: Rear, 1: Front
  uint8_t flashlight_name[8];
} ATOM_CAMERA_FLASHLIGHT_INFO;

typedef struct _atom_camera_data {
  uint32_t versionCode;
  ATOM_CAMERA_MODULE_INFO cameraInfo[3];      // Assuming 3 camera sensors max
  ATOM_CAMERA_FLASHLIGHT_INFO flashInfo;      // Assuming 1 flashlight max
  ATOM_CAMERA_DPHY_ELEC_PARAM dphy_param;
  uint32_t crc_val;         // CRC
} ATOM_CAMERA_DATA;


typedef struct _atom_DCN_dpphy_dvihdmi_tuningset {
  uint32_t max_symclk_in10khz;
  uint8_t  encoder_mode;           //atom_encode_mode_def, =2: DVI, =3: HDMI mode
  uint8_t  phy_sel;                //bit vector of phy, bit0= phya, bit1=phyb, ....bit5 = phyf
  uint8_t  tx_eq_main;             // map to RDPCSTX_PHY_FUSE0/1/2/3[5:0](EQ_MAIN)
  uint8_t  tx_eq_pre;              // map to RDPCSTX_PHY_FUSE0/1/2/3[11:6](EQ_PRE)
  uint8_t  tx_eq_post;             // map to RDPCSTX_PHY_FUSE0/1/2/3[17:12](EQ_POST)
  uint8_t  reserved1;
  uint8_t  tx_vboost_lvl;          // tx_vboost_lvl, map to RDPCSTX_PHY_CNTL0.RDPCS_PHY_TX_VBOOST_LVL
  uint8_t  reserved2;
} ATOM_DCN_DPPHY_DVIHDMI_TUNINGSET;

typedef struct _atom_DCN_dpphy_dp_setting{
  uint8_t dp_vs_pemph_level;       //enum of atom_dp_vs_preemph_def
  uint8_t tx_eq_main;             // map to RDPCSTX_PHY_FUSE0/1/2/3[5:0](EQ_MAIN)
  uint8_t tx_eq_pre;              // map to RDPCSTX_PHY_FUSE0/1/2/3[11:6](EQ_PRE)
  uint8_t tx_eq_post;             // map to RDPCSTX_PHY_FUSE0/1/2/3[17:12](EQ_POST)
  uint8_t tx_vboost_lvl;          // tx_vboost_lvl, map to RDPCSTX_PHY_CNTL0.RDPCS_PHY_TX_VBOOST_LVL
} ATOM_DCN_DPPHY_DP_SETTING;

typedef struct _atom_DCN_dpphy_dp_tuningset {
  uint8_t phy_sel;                 // bit vector of phy, bit0= phya, bit1=phyb, ....bit5 = phyf
  uint8_t version;
  uint16_t table_size;             // size of atom_14nm_dpphy_dp_setting
  uint16_t reserved;
  ATOM_DCN_DPPHY_DP_SETTING dptunings[10];
} ATOM_DCN_DPPHY_DP_TUNINGSET;

typedef struct _atom_14nm_dig_transmitter_info_header_v4_0 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint16_t pcie_phy_tmds_hdmi_macro_settings_offset;     // offset of PCIEPhyTMDSHDMIMacroSettingsTbl
  uint16_t uniphy_vs_emph_lookup_table_offset;           // offset of UniphyVSEmphLookUpTbl
  uint16_t uniphy_xbar_settings_table_offset;            // offset of UniphyXbarSettingsTbl
} ATOM_14NM_DIG_TRANSMITTER_INFO_HEADER_V4_0;

typedef struct _atom_14nm_combphy_tmds_vs_set {
  uint32_t sym_clk;
  uint8_t dig_mode;
  uint8_t phy_sel;
  uint16_t common_mar_deemph_nom__margin_deemph_val;
  uint8_t common_seldeemph60__deemph_6db_4_val;
  uint8_t cmd_bus_global_for_tx_lane0__boostadj_val ;
  uint8_t common_zcalcode_ctrl__tx_driver_fifty_ohms_val;
  uint8_t margin_deemph_lane0__deemph_sel_val;
} ATOM_14NM_COMBPHY_TMDS_VS_SET;

typedef struct _atom_i2c_reg_info {
  uint8_t ucI2cRegIndex;
  uint8_t ucI2cRegVal;
} ATOM_I2C_REG_INFO;

typedef struct _atom_hdmi_retimer_redriver_set {
  uint8_t HdmiSlvAddr;
  uint8_t HdmiRegNum;
  uint8_t Hdmi6GRegNum;
  ATOM_I2C_REG_INFO HdmiRegSetting[9];        //For non 6G Hz use
  ATOM_I2C_REG_INFO Hdmi6GhzRegSetting[3];    //For 6G Hz use.
} ATOM_HDMI_RETIMER_REDRIVER_SET;

typedef struct _atom_DCN_dpphy_tuningset{
  uint8_t phy_sel;                 // bit vector of phy, bit0= phya, bit1=phyb, ....bit5 = phyf
  uint8_t TuningVal[5];
  uint16_t reserved;
} ATOM_DCN_DPPHY_TUNINGSET;

typedef struct _atom_integrated_system_info_v1_11 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t  vbios_misc;                       //enum of atom_system_vbiosmisc_def
  uint32_t  gpucapinfo;                       //enum of atom_system_gpucapinf_def
  uint32_t  system_config;
  uint32_t  cpucapinfo;
  uint16_t  gpuclk_ss_percentage;             //unit of 0.001%,   1000 mean 1%
  uint16_t  gpuclk_ss_type;
  uint16_t  lvds_ss_percentage;               //unit of 0.001%,   1000 mean 1%
  uint16_t  lvds_ss_rate_10hz;
  uint16_t  hdmi_ss_percentage;               //unit of 0.001%,   1000 mean 1%
  uint16_t  hdmi_ss_rate_10hz;
  uint16_t  dvi_ss_percentage;                //unit of 0.001%,   1000 mean 1%
  uint16_t  dvi_ss_rate_10hz;
  uint16_t  dpphy_override;                   // bit vector, enum of atom_sysinfo_dpphy_override_def
  uint16_t  lvds_misc;                        // enum of atom_sys_info_lvds_misc_def
  uint16_t  backlight_pwm_hz;                 // pwm frequency in hz
  uint8_t   memorytype;                       // enum of atom_dmi_t17_mem_type_def, APU memory type indication.
  uint8_t   umachannelnumber;                 // number of memory channels
  uint8_t   pwr_on_digon_to_de;               /* all pwr sequence numbers below are in uint of 4ms */
  uint8_t   pwr_on_de_to_vary_bl;
  uint8_t   pwr_down_vary_bloff_to_de;
  uint8_t   pwr_down_de_to_digoff;
  uint8_t   pwr_off_delay;
  uint8_t   pwr_on_vary_bl_to_blon;
  uint8_t   pwr_down_bloff_to_vary_bloff;
  uint8_t   min_allowed_bl_level;
  uint8_t   htc_hyst_limit;
  uint8_t   htc_tmp_limit;
  uint8_t   reserved1;
  uint8_t   reserved2;
  ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO extdispconninfo;
  ATOM_DCN_DPPHY_DVIHDMI_TUNINGSET  TMDS_tuningset;
  ATOM_DCN_DPPHY_DVIHDMI_TUNINGSET  hdmiCLK5_tuningset;
  ATOM_DCN_DPPHY_DVIHDMI_TUNINGSET  hdmiCLK8_tuningset;
  ATOM_DCN_DPPHY_DP_TUNINGSET dp_tuningset;        // rbr 1.62G dp tuning set
  ATOM_DCN_DPPHY_DP_TUNINGSET dp_hbr3_tuningset;   // HBR3 dp tuning set
  ATOM_CAMERA_DATA  camera_info;
  ATOM_HDMI_RETIMER_REDRIVER_SET dp0_retimer_set;   //for DP0
  ATOM_HDMI_RETIMER_REDRIVER_SET dp1_retimer_set;   //for DP1
  ATOM_HDMI_RETIMER_REDRIVER_SET dp2_retimer_set;   //for DP2
  ATOM_HDMI_RETIMER_REDRIVER_SET dp3_retimer_set;   //for DP3
  ATOM_DCN_DPPHY_DP_TUNINGSET dp_hbr_tuningset;    //hbr 2.7G dp tuning set
  ATOM_DCN_DPPHY_DP_TUNINGSET dp_hbr2_tuningset;   //hbr2 5.4G dp turnig set
  ATOM_DCN_DPPHY_DP_TUNINGSET edp_tuningset;       //edp tuning set
  ATOM_DCN_DPPHY_DVIHDMI_TUNINGSET  hdmiCLK6_tuningset;
  ATOM_DCN_DPPHY_TUNINGSET TX_LP_ATT_tuningset;
  ATOM_DCN_DPPHY_TUNINGSET TX_TERM_CTRL_tuningset;
  uint32_t  reserved[59];
} ATOM_INTEGRATED_SYSTEM_INFO_V1_12;


// system_config
typedef enum {
  INTEGRATED_SYSTEM_INFO__GET_EDID_CALLBACK_FUNC_SUPPORT = 0x01,
} ATOM_SYSTEM_VBIOSMISC_DEF;


// gpucapinfo
typedef enum {
  SYS_INFO_GPUCAPS__ENABEL_DFS_BYPASS  = 0x10,
  SYS_INFO_GPUCAPS__EXT_HDMI_INIT_PER_PORT = 0x20,
} ATOM_SYSTEM_GPUCAPINF_DEF;

//dpphy_override
typedef enum {
  ATOM_ENABLE_TMDS_TUNINGSET   = 0x01,
  ATOM_ENABLE_HDMI_CLK5_TUNINGSET  = 0x02,
  ATOM_ENABLE_HDMI_CLK8_TUNINGSET  = 0x04,
  ATOM_ENABLE_DP_TUNINGSET  = 0x08,
  ATOM_ENABLE_DP_HBR3_TUNINGSET  = 0x10,
  ATOM_ENABLE_DP_HBR_TUNINGSET  = 0x20,      ///<
  ATOM_ENABLE_DP_HBR2_TUNINGSET  = 0x40,     ///<
  ATOM_ENABLE_EDP_TUNINGSET  = 0x80,         ///<
} ATOM_SYSINFO_DPPHY_OVERRIDE_DEF;

//lvds_misc
typedef enum {
  SYS_INFO_LVDS_MISC_888_FPDI_MODE                 =0x01,
  SYS_INFO_LVDS_MISC_888_BPC_MODE                  =0x04,
  SYS_INFO_LVDS_MISC_OVERRIDE_EN                   =0x08,
} ATOM_SYS_INFO_LVDS_MISC_DEF;


//memorytype  DMI Type 17 offset 12h - Memory Type
typedef enum {
  OtherMemType = 0x01,                                  ///< Assign 01 to Other
  UnknownMemType,                                       ///< Assign 02 to Unknown
  DramMemType,                                          ///< Assign 03 to DRAM
  EdramMemType,                                         ///< Assign 04 to EDRAM
  VramMemType,                                          ///< Assign 05 to VRAM
  SramMemType,                                          ///< Assign 06 to SRAM
  RamMemType,                                           ///< Assign 07 to RAM
  RomMemType,                                           ///< Assign 08 to ROM
  FlashMemType,                                         ///< Assign 09 to Flash
  EepromMemType,                                        ///< Assign 10 to EEPROM
  FepromMemType,                                        ///< Assign 11 to FEPROM
  EpromMemType,                                         ///< Assign 12 to EPROM
  CdramMemType,                                         ///< Assign 13 to CDRAM
  ThreeDramMemType,                                     ///< Assign 14 to 3DRAM
  SdramMemType,                                         ///< Assign 15 to SDRAM
  SgramMemType,                                         ///< Assign 16 to SGRAM
  RdramMemType,                                         ///< Assign 17 to RDRAM
  DdrMemType,                                           ///< Assign 18 to DDR
  Ddr2MemType,                                          ///< Assign 19 to DDR2
  Ddr2FbdimmMemType,                                    ///< Assign 20 to DDR2 FB-DIMM
  Ddr3MemType = 0x18,                                   ///< Assign 24 to DDR3
  Fbd2MemType,                                          ///< Assign 25 to FBD2
  Ddr4MemType,                                          ///< Assign 26 to DDR4
  LpDdrMemType,                                         ///< Assign 27 to LPDDR
  LpDdr2MemType,                                        ///< Assign 28 to LPDDR2
  LpDdr3MemType,                                        ///< Assign 29 to LPDDR3
  LpDdr4MemType,                                        ///< Assign 30 to LPDDR4
  Ddr5MemType = 0x22,                                   ///< Assign 34 to DDR5
  LpDdr5MemType,                                        ///< Assign 35 to LPDDR5
} ATOM_DMI_T17_MEM_TYPE_DEF;


// this Table is used starting from NL/AM, used by SBIOS and pass the IntegratedSystemInfoTable/PowerPlayInfoTable/SystemCameraInfoTable
typedef struct {
  ATOM_INTEGRATED_SYSTEM_INFO_V1_12   sIntegratedSysInfo;       // refer to ATOM_INTEGRATED_SYSTEM_INFO_V1_8 definition
  uint32_t   powerplayinfo[256];                                // Reserve 1024 bytes space for PowerPlayInfoTable
} ATOM_FUSION_SYSTEM_INFO_V4;


/*
  ***************************************************************************
    Data Table gfx_info  structure
  ***************************************************************************
*/

typedef struct  _atom_gfx_info_v2_2
{
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t gfxip_min_ver;
  uint8_t gfxip_max_ver;
  uint8_t max_shader_engines;
  uint8_t max_tile_pipes;
  uint8_t max_cu_per_sh;
  uint8_t max_sh_per_se;
  uint8_t max_backends_per_se;
  uint8_t max_texture_channel_caches;
  uint32_t regaddr_cp_dma_src_addr;
  uint32_t regaddr_cp_dma_src_addr_hi;
  uint32_t regaddr_cp_dma_dst_addr;
  uint32_t regaddr_cp_dma_dst_addr_hi;
  uint32_t regaddr_cp_dma_command;
  uint32_t regaddr_cp_status;
  uint32_t regaddr_rlc_gpu_clock_32;
  uint32_t rlc_gpu_timer_refclk;
} ATOM_GFX_INFO_V2_2;

typedef struct _atom_gfx_info_v2_3 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t gfxip_min_ver;
  uint8_t gfxip_max_ver;
  uint8_t max_shader_engines;
  uint8_t max_tile_pipes;
  uint8_t max_cu_per_sh;
  uint8_t max_sh_per_se;
  uint8_t max_backends_per_se;
  uint8_t max_texture_channel_caches;
  uint32_t regaddr_cp_dma_src_addr;
  uint32_t regaddr_cp_dma_src_addr_hi;
  uint32_t regaddr_cp_dma_dst_addr;
  uint32_t regaddr_cp_dma_dst_addr_hi;
  uint32_t regaddr_cp_dma_command;
  uint32_t regaddr_cp_status;
  uint32_t regaddr_rlc_gpu_clock_32;
  uint32_t rlc_gpu_timer_refclk;
  uint8_t active_cu_per_sh;
  uint8_t active_rb_per_se;
  uint16_t gcgoldenoffset;
  uint32_t rm21_sram_vmin_value;
} ATOM_GFX_INFO_V2_3;

typedef struct _atom_gfx_info_v2_4 //For VG 20 only, not applicable to NV, NV will get back to use 23
{
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t gfxip_min_ver;
  uint8_t gfxip_max_ver;
  uint8_t max_shader_engines;
  uint8_t reserved;
  uint8_t max_cu_per_sh;
  uint8_t max_sh_per_se;
  uint8_t max_backends_per_se;
  uint8_t max_texture_channel_caches;
  uint32_t regaddr_cp_dma_src_addr;
  uint32_t regaddr_cp_dma_src_addr_hi;
  uint32_t regaddr_cp_dma_dst_addr;
  uint32_t regaddr_cp_dma_dst_addr_hi;
  uint32_t regaddr_cp_dma_command;
  uint32_t regaddr_cp_status;
  uint32_t regaddr_rlc_gpu_clock_32;
  uint32_t rlc_gpu_timer_refclk;
  uint8_t active_cu_per_sh;
  uint8_t active_rb_per_se;
  uint16_t gcgoldenoffset;
  uint16_t gc_num_gprs;
  uint16_t gc_gsprim_buff_depth;
  uint16_t gc_parameter_cache_depth;
  uint16_t gc_wave_size;
  uint16_t gc_max_waves_per_simd;
  uint16_t gc_lds_size;
  uint8_t gc_num_max_gs_thds;
  uint8_t gc_gs_table_depth;
  uint8_t gc_double_offchip_lds_buffer;
  uint8_t gc_max_scratch_slots_per_cu;
  uint32_t sram_rm_fuses_val;
  uint32_t sram_custom_rm_fuses_val;
} ATOM_GFX_INFO_V2_4;

typedef struct _atom_gfx_info_v2_5 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t gfxip_min_ver;
  uint8_t gfxip_max_ver;
  uint8_t max_shader_engines;
  uint8_t max_tile_pipes;
  uint8_t max_cu_per_sh;
  uint8_t max_sh_per_se;
  uint8_t max_backends_per_se;
  uint8_t max_texture_channel_caches;
  uint32_t regaddr_cp_dma_src_addr;
  uint32_t regaddr_cp_dma_src_addr_hi;
  uint32_t regaddr_cp_dma_dst_addr;
  uint32_t regaddr_cp_dma_dst_addr_hi;
  uint32_t regaddr_cp_dma_command;
  uint32_t regaddr_cp_status;
  uint32_t regaddr_rlc_gpu_clock_32;
  uint32_t rlc_gpu_timer_refclk;
  uint8_t active_cu_per_sh;
  uint8_t active_rb_per_se;
  uint16_t reserved1;
  uint32_t sram_rm_fuses_val;
  uint32_t sram_custom_rm_fuses_val;
  uint32_t reserved[8];
} ATOM_GFX_INFO_V2_5;

/*
  ***************************************************************************
    Data Table smu_info  structure
  ***************************************************************************
*/
typedef struct _atom_smu_info_v3_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t smuip_min_ver;
  uint8_t smuip_max_ver;
  uint8_t smu_rsd1;
  uint8_t gpuclk_ss_mode;           // enum of atom_spread_spectrum_mode
  uint16_t sclk_ss_percentage;
  uint16_t sclk_ss_rate_10hz;
  uint16_t gpuclk_ss_percentage;    // in unit of 0.001%
  uint16_t gpuclk_ss_rate_10hz;
  uint32_t core_refclk_10khz;
  uint8_t  ac_dc_gpio_bit;          // GPIO bit shift in SMU_GPIOPAD_A  configured for AC/DC switching, =0xff means invalid
  uint8_t  ac_dc_polarity;          // GPIO polarity for AC/DC switching
  uint8_t  vr0hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A  configured for VR0 HOT event, =0xff means invalid
  uint8_t  vr0hot_polarity;         // GPIO polarity for VR0 HOT event
  uint8_t  vr1hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for VR1 HOT event , =0xff means invalid
  uint8_t  vr1hot_polarity;         // GPIO polarity for VR1 HOT event
  uint8_t  fw_ctf_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for CTF, =0xff means invalid
  uint8_t  fw_ctf_polarity;         // GPIO polarity for CTF
} ATOM_SMU_INFO_V3_1;

typedef struct _atom_smu_info_v3_2 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t  smuip_min_ver;
  uint8_t  smuip_max_ver;
  uint8_t  smu_rsd1;
  uint8_t  gpuclk_ss_mode;
  uint16_t sclk_ss_percentage;
  uint16_t sclk_ss_rate_10hz;
  uint16_t gpuclk_ss_percentage;    // in unit of 0.001%
  uint16_t gpuclk_ss_rate_10hz;
  uint32_t core_refclk_10khz;
  uint8_t  ac_dc_gpio_bit;          // GPIO bit shift in SMU_GPIOPAD_A  configured for AC/DC switching, =0xff means invalid
  uint8_t  ac_dc_polarity;          // GPIO polarity for AC/DC switching
  uint8_t  vr0hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A  configured for VR0 HOT event, =0xff means invalid
  uint8_t  vr0hot_polarity;         // GPIO polarity for VR0 HOT event
  uint8_t  vr1hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for VR1 HOT event , =0xff means invalid
  uint8_t  vr1hot_polarity;         // GPIO polarity for VR1 HOT event
  uint8_t  fw_ctf_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for CTF, =0xff means invalid
  uint8_t  fw_ctf_polarity;         // GPIO polarity for CTF
  uint8_t  pcc_gpio_bit;            // GPIO bit shift in SMU_GPIOPAD_A configured for PCC, =0xff means invalid
  uint8_t  pcc_gpio_polarity;       // GPIO polarity for CTF
  uint16_t smugoldenoffset;
  uint32_t gpupll_vco_freq_10khz;
  uint32_t bootup_smnclk_10khz;
  uint32_t bootup_socclk_10khz;
  uint32_t bootup_mp0clk_10khz;
  uint32_t bootup_mp1clk_10khz;
  uint32_t bootup_lclk_10khz;
  uint32_t bootup_dcefclk_10khz;
  uint32_t ctf_threshold_override_value;
  uint32_t reserved[5];
} ATOM_SMU_INFO_V3_2;
//IPCLEAN_END

typedef struct _atom_smu_info_v3_3 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t  smuip_min_ver;
  uint8_t  smuip_max_ver;
  uint8_t  waflclk_ss_mode;
  uint8_t  gpuclk_ss_mode;
  uint16_t sclk_ss_percentage;
  uint16_t sclk_ss_rate_10hz;
  uint16_t gpuclk_ss_percentage;    // in unit of 0.001%
  uint16_t gpuclk_ss_rate_10hz;
  uint32_t core_refclk_10khz;
  uint8_t  ac_dc_gpio_bit;          // GPIO bit shift in SMU_GPIOPAD_A  configured for AC/DC switching, =0xff means invalid
  uint8_t  ac_dc_polarity;          // GPIO polarity for AC/DC switching
  uint8_t  vr0hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A  configured for VR0 HOT event, =0xff means invalid
  uint8_t  vr0hot_polarity;         // GPIO polarity for VR0 HOT event
  uint8_t  vr1hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for VR1 HOT event , =0xff means invalid
  uint8_t  vr1hot_polarity;         // GPIO polarity for VR1 HOT event
  uint8_t  fw_ctf_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for CTF, =0xff means invalid
  uint8_t  fw_ctf_polarity;         // GPIO polarity for CTF
  uint8_t  pcc_gpio_bit;            // GPIO bit shift in SMU_GPIOPAD_A configured for PCC, =0xff means invalid
  uint8_t  pcc_gpio_polarity;       // GPIO polarity for CTF
  uint16_t smugoldenoffset;
  uint32_t gpupll_vco_freq_10khz;
  uint32_t bootup_smnclk_10khz;
  uint32_t bootup_socclk_10khz;
  uint32_t bootup_mp0clk_10khz;
  uint32_t bootup_mp1clk_10khz;
  uint32_t bootup_lclk_10khz;
  uint32_t bootup_dcefclk_10khz;
  uint32_t ctf_threshold_override_value;
  uint32_t syspll3_0_vco_freq_10khz;
  uint32_t syspll3_1_vco_freq_10khz;
  uint32_t bootup_fclk_10khz;
  uint32_t bootup_waflclk_10khz;
  uint32_t smu_info_caps;
  uint16_t waflclk_ss_percentage;    // in unit of 0.001%
  uint16_t smuinitoffset;
  uint32_t reserved;
} ATOM_SMU_INFO_V3_3;


typedef enum {
  // only for VBIOS
  SMU_INFO_V3_3_CAPS_GFXCLK_AFLL_MODE_EN  =0x01,
  SMU_INFO_V3_3_CAPS_HWCTF_HOTSPOT_EN     =0x02,
  SMU_INFO_V3_3_CAPS_PCC_SQR              =0x04,
} SMU_INFO_V3_3_CAPS_DEF;

//IPCLEAN_START
typedef struct _atom_smu_info_v3_4 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t  smuip_min_ver;
  uint8_t  smuip_max_ver;
  uint8_t  waflclk_ss_mode;
  uint8_t  gpuclk_ss_mode;
  uint16_t sclk_ss_percentage;
  uint16_t sclk_ss_rate_10hz;
  uint16_t gpuclk_ss_percentage;    // in unit of 0.001%
  uint16_t gpuclk_ss_rate_10hz;
  uint32_t core_refclk_10khz;
  uint8_t  ac_dc_gpio_bit;          // GPIO bit shift in SMU_GPIOPAD_A  configured for AC/DC switching, =0xff means invalid
  uint8_t  ac_dc_polarity;          // GPIO polarity for AC/DC switching
  uint8_t  vr0hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A  configured for VR0 HOT event, =0xff means invalid
  uint8_t  vr0hot_polarity;         // GPIO polarity for VR0 HOT event
  uint8_t  vr1hot_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for VR1 HOT event , =0xff means invalid
  uint8_t  vr1hot_polarity;         // GPIO polarity for VR1 HOT event
  uint8_t  fw_ctf_gpio_bit;         // GPIO bit shift in SMU_GPIOPAD_A configured for CTF, =0xff means invalid
  uint8_t  fw_ctf_polarity;         // GPIO polarity for CTF
  uint8_t  pcc_gpio_bit;            // GPIO bit shift in SMU_GPIOPAD_A configured for PCC, =0xff means invalid
  uint8_t  pcc_gpio_polarity;       // GPIO polarity for CTF
  uint16_t smugoldenoffset;
  uint32_t gpupll_vco_freq_10khz;
  uint32_t bootup_smnclk_10khz;
  uint32_t bootup_socclk_10khz;
  uint32_t bootup_mp0clk_10khz;
  uint32_t bootup_mp1clk_10khz;
  uint32_t bootup_lclk_10khz;
  uint32_t bootup_dcefclk_10khz;
  uint32_t ctf_threshold_override_value;
  uint32_t syspll3_0_vco_freq_10khz;
  uint32_t syspll3_1_vco_freq_10khz;
  uint32_t bootup_fclk_10khz;
  uint32_t bootup_waflclk_10khz;
  uint32_t smu_info_caps;
  uint16_t waflclk_ss_percentage;    // in unit of 0.001%
  uint16_t smuinitoffset;
  uint32_t bootup_dprefclk_10khz;
  uint32_t bootup_usbclk_10khz;
  uint32_t smb_slave_address;
  uint32_t cg_fdo_ctrl0_val;
  uint32_t cg_fdo_ctrl1_val;
  uint32_t cg_fdo_ctrl2_val;
  uint32_t gdfll_as_wait_ctrl_val;
  uint32_t gdfll_as_step_ctrl_val;
  uint32_t reserved[30];
} ATOM_SMU_INFO_V3_4;



/*
  ***************************************************************************
    Data Table smc_dpm_info  structure
  ***************************************************************************
*/
typedef struct _atom_smc_dpm_info_v4_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  // SECTION: BOARD PARAMETERS
  // i2c Control
  uint8_t  liquid1_i2c_address;
  uint8_t  liquid2_i2c_address;
  uint8_t  vr_i2c_address;
  uint8_t  plx_i2c_address;

  uint8_t  liquid_i2c_linescl;
  uint8_t  liquid_i2c_linesda;
  uint8_t  vr_i2c_linescl;
  uint8_t  vr_i2c_linesda;

  uint8_t  plx_i2c_linescl;
  uint8_t  plx_i2c_linesda;
  uint8_t  vrsensorpresent;
  uint8_t  liquidsensorpresent;

  uint16_t maxvoltagestepgfx; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.
  uint16_t maxvoltagestepsoc; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.

  uint8_t  vddgfxvrmapping;   // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddsocvrmapping;   // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddmem0vrmapping;  // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddmem1vrmapping;  // Use atom_smu_vr_mapping_bit_def

  uint8_t  gfxulvphasesheddingmask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t  soculvphasesheddingmask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t  padding8_v[2];

  // Telemetry Settings
  uint16_t gfxmaxcurrent;   // in Amps
  uint8_t  gfxoffset;       // in Amps
  uint8_t  padding_telemetrygfx;

  uint16_t socmaxcurrent;   // in Amps
  uint8_t  socoffset;       // in Amps
  uint8_t  padding_telemetrysoc;

  uint16_t mem0maxcurrent;   // in Amps
  uint8_t  mem0offset;       // in Amps
  uint8_t  padding_telemetrymem0;

  uint16_t mem1maxcurrent;   // in Amps
  uint8_t  mem1offset;       // in Amps
  uint8_t  padding_telemetrymem1;

  // GPIO Settings
  uint8_t  acdcgpio;         // GPIO pin configured for AC/DC switching
  uint8_t  acdcpolarity;     // GPIO polarity for AC/DC switching
  uint8_t  vr0hotgpio;       // GPIO pin configured for VR0 HOT event
  uint8_t  vr0hotpolarity;   // GPIO polarity for VR0 HOT event

  uint8_t  vr1hotgpio;       // GPIO pin configured for VR1 HOT event
  uint8_t  vr1hotpolarity;   // GPIO polarity for VR1 HOT event
  uint8_t  padding1;         // replace GPIO pin configured for CTF
  uint8_t  padding2;         // replace GPIO polarity for CTF

  // LED Display Settings
  uint8_t  ledpin0;          // GPIO number for LedPin[0]
  uint8_t  ledpin1;          // GPIO number for LedPin[1]
  uint8_t  ledpin2;          // GPIO number for LedPin[2]
  uint8_t  padding8_4;

  // GFXCLK Spread Spectrum
  uint8_t  pllgfxclkspreadenabled;   // on or off
  uint8_t  pllgfxclkspreadpercent;   // Q4.4
  uint16_t pllgfxclkspreadfreq;      // kHz

  // UCLK Spread Spectrum
  uint8_t  uclkspreadenabled;       // on or off
  uint8_t  uclkspreadpercent;      // Q4.4
  uint16_t uclkspreadfreq;           // kHz

  // SOCCLK Spread Spectrum
  uint8_t  socclkspreadenabled;     // on or off
  uint8_t  socclkspreadpercent;     // Q4.4
  uint16_t socclkspreadfreq;      // kHz

  // GFXCLK ACG Spread Spectrum
  uint8_t  acggfxclkspreadenabled;   // on or off
  uint8_t  acggfxclkspreadpercent;   // Q4.4
  uint16_t acggfxclkspreadfreq;      // kHz


  uint8_t  Vr2_I2C_address;         // VR I2C address for MVDD and VDDCI
  uint8_t  pading_vr2[3];

  // reserved
  uint32_t boardreserved[9];
} ATOM_SMC_DPM_INFO_V4_1;


// VR Mapping Bit Defines
typedef enum {
  ATOM_VR_MAPPING_VR_SELECT_MASK  = 0x01,
  ATOM_VR_MAPPING_PLANE_SELECT_MASK = 0x02,
} ATOM_SMU_VR_MAPPING_BIT_DEF;

// PSI Bit Defines
typedef enum {
  SVI_PSI_SEL_VR0_PLANE0_PSI0  = 0x01,
  SVI_PSI_SEL_VR0_PLANE0_PSI1  = 0x02,
  SVI_PSI_SEL_VR0_PLANE1_PSI0  = 0x04,
  SVI_PSI_SEL_VR0_PLANE1_PSI1  = 0x08,
  SVI_PSI_SEL_VR1_PLANE0_PSI0  = 0x10,
  SVI_PSI_SEL_VR1_PLANE0_PSI1  = 0x20,
  SVI_PSI_SEL_VR1_PLANE1_PSI0  = 0x40,
  SVI_PSI_SEL_VR1_PLANE1_PSI1  = 0x80,
} ATOM_SMU_SVI_PSI_BIT_DEF;


typedef struct _atom_smc_dpm_info_v4_3 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  // SECTION: BOARD PARAMETERS
  // i2c Control
  uint8_t  liquid1_i2c_address;
  uint8_t  liquid2_i2c_address;
  uint8_t  vr_i2c_address;
  uint8_t  plx_i2c_address;

  uint8_t  liquid_i2c_linescl;
  uint8_t  liquid_i2c_linesda;
  uint8_t  vr_i2c_linescl;
  uint8_t  vr_i2c_linesda;

  uint8_t  plx_i2c_linescl;
  uint8_t  plx_i2c_linesda;
  uint8_t  vrsensorpresent;
  uint8_t  liquidsensorpresent;

  uint16_t maxvoltagestepgfx; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.
  uint16_t maxvoltagestepsoc; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.

  uint8_t  vddgfxvrmapping;   // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddsocvrmapping;   // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddmem0vrmapping;  // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddmem1vrmapping;  // Use atom_smu_vr_mapping_bit_def

  uint8_t  gfxulvphasesheddingmask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t  soculvphasesheddingmask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t  externalsensorpresent;
  uint8_t  padding8_v;

  // Telemetry Settings
  uint16_t gfxmaxcurrent;   // in Amps
  uint8_t  gfxoffset;       // in Amps
  uint8_t  padding_telemetrygfx;

  uint16_t socmaxcurrent;   // in Amps
  uint8_t  socoffset;       // in Amps
  uint8_t  padding_telemetrysoc;

  uint16_t mem0maxcurrent;   // in Amps
  uint8_t  mem0offset;       // in Amps
  uint8_t  padding_telemetrymem0;

  uint16_t mem1maxcurrent;   // in Amps
  uint8_t  mem1offset;       // in Amps
  uint8_t  padding_telemetrymem1;

  // GPIO Settings
  uint8_t  acdcgpio;         // GPIO pin configured for AC/DC switching
  uint8_t  acdcpolarity;     // GPIO polarity for AC/DC switching
  uint8_t  vr0hotgpio;       // GPIO pin configured for VR0 HOT event
  uint8_t  vr0hotpolarity;   // GPIO polarity for VR0 HOT event

  uint8_t  vr1hotgpio;       // GPIO pin configured for VR1 HOT event
  uint8_t  vr1hotpolarity;   // GPIO polarity for VR1 HOT event
  uint8_t  padding1;         // replace GPIO pin configured for CTF
  uint8_t  padding2;         // replace GPIO polarity for CTF

  // LED Display Settings
  uint8_t  ledpin0;          // GPIO number for LedPin[0]
  uint8_t  ledpin1;          // GPIO number for LedPin[1]
  uint8_t  ledpin2;          // GPIO number for LedPin[2]
  uint8_t  padding8_4;

  // GFXCLK Spread Spectrum
  uint8_t  pllgfxclkspreadenabled;   // on or off
  uint8_t  pllgfxclkspreadpercent;   // Q4.4
  uint16_t pllgfxclkspreadfreq;      // kHz

  // UCLK Spread Spectrum
  uint8_t  uclkspreadenabled;       // on or off
  uint8_t  uclkspreadpercent;      // Q4.4
  uint16_t uclkspreadfreq;           // kHz

  // FCLK Spread Spectrum
  uint8_t  fclkspreadenabled;   // on or off
  uint8_t  fclkspreadpercent;   // Q4.4
  uint16_t fclkspreadfreq;      // kHz

  // GFXCLK Fll Spread Spectrum
  uint8_t  fllgfxclkspreadenabled;   // on or off
  uint8_t  fllgfxclkspreadpercent;   // Q4.4
  uint16_t fllgfxclkspreadfreq;      // kHz

  // reserved
  uint32_t boardreserved[10];
} ATOM_SMC_DPM_INFO_V4_3;

typedef enum {
  SMU_I2C_CONTROLLER_PORT_0 = 0, //CKSVII2C0
  SMU_I2C_CONTROLLER_PORT_1 = 1, //CKSVII2C1
} SMUDPM_I2CCONTROLLERPORT_E;

typedef enum {
  SMU_I2C_CONTROLLER_NAME_VR_GFX = 0,
  SMU_I2C_CONTROLLER_NAME_VR_SOC,
  SMU_I2C_CONTROLLER_NAME_VR_VDDCI,
  SMU_I2C_CONTROLLER_NAME_VR_HBM,
  SMU_I2C_CONTROLLER_NAME_LIQUID_0,
  SMU_I2C_CONTROLLER_NAME_LIQUID_1,
  SMU_I2C_CONTROLLER_NAME_PLX,
  SMU_I2C_CONTROLLER_NAME_COUNT,
} SMUDPM_I2CCONTROLLERNAME_E;

typedef enum {
  SMU_I2C_CONTROLLER_THROTTLER_TYPE_NONE = 0,
  SMU_I2C_CONTROLLER_THROTTLER_VR_GFX,
  SMU_I2C_CONTROLLER_THROTTLER_VR_SOC,
  SMU_I2C_CONTROLLER_THROTTLER_VR_VDDCI,
  SMU_I2C_CONTROLLER_THROTTLER_VR_HBM,
  SMU_I2C_CONTROLLER_THROTTLER_LIQUID_0,
  SMU_I2C_CONTROLLER_THROTTLER_LIQUID_1,
  SMU_I2C_CONTROLLER_THROTTLER_PLX,
} SMUDPM_I2CCONTROLLERTHROTTLER_E;

typedef enum {
  SMU_I2C_CONTROLLER_PROTOCOL_VR_XPDE132G5,
  SMU_I2C_CONTROLLER_PROTOCOL_VR_IR35217,
  SMU_I2C_CONTROLLER_PROTOCOL_TMP_TMP102A,
  SMU_I2C_CONTROLLER_PROTOCOL_SPARE_0,
  SMU_I2C_CONTROLLER_PROTOCOL_SPARE_1,
  SMU_I2C_CONTROLLER_PROTOCOL_SPARE_2,
} SMUDPM_I2CCONTROLLERPROTOCOL_E;

typedef enum {
  SMU_I2C_CONTROLLER_SPEED_SLOW = 0,
  SMU_I2C_CONTROLLER_SPEED_FAST = 1,
} SMUDMP_I2CCONTROLLERSPEED_E;

typedef struct _smudpm_i2ccontrollerconfig_t {
  uint32_t  enabled;
  uint32_t  slaveaddress;
  uint32_t  controllerport;     // enum of smudpm_i2ccontrollerPort_e
  uint32_t  controllername;     // enum of smudpm_i2ccontrollerName_e
  uint32_t  thermalthrottler;   // enum of smudpm_i2ccontrollerThrottler_e
  uint32_t  i2cprotocol;        // enum of smudpm_i2ccontrollerProtocol_e
  uint32_t  i2cspeed;           // enum of smudpm_i2ccontrollerSpeed_e
} SMUDPM_I2CCONTROLLERCONFIG_T;

typedef struct _atom_smc_dpm_info_v4_4 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  // SECTION: BOARD PARAMETERS
  uint32_t  i2c_padding[3];   // old i2c control are moved to new area

  uint16_t maxvoltagestepgfx; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.
  uint16_t maxvoltagestepsoc; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.

  uint8_t  vddgfxvrmapping;   // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddsocvrmapping;   // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddmem0vrmapping;  // Use atom_smu_vr_mapping_bit_def
  uint8_t  vddmem1vrmapping;  // Use atom_smu_vr_mapping_bit_def

  uint8_t  gfxulvphasesheddingmask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t  soculvphasesheddingmask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t  externalsensorpresent;
  uint8_t  padding8_v;

  // Telemetry Settings
  uint16_t gfxmaxcurrent;   // in Amps
  uint8_t  gfxoffset;       // in Amps
  uint8_t  padding_telemetrygfx;

  uint16_t socmaxcurrent;   // in Amps
  uint8_t  socoffset;       // in Amps
  uint8_t  padding_telemetrysoc;

  uint16_t mem0maxcurrent;   // in Amps
  uint8_t  mem0offset;       // in Amps
  uint8_t  padding_telemetrymem0;

  uint16_t mem1maxcurrent;   // in Amps
  uint8_t  mem1offset;       // in Amps
  uint8_t  padding_telemetrymem1;

  // GPIO Settings
  uint8_t  acdcgpio;         // GPIO pin configured for AC/DC switching
  uint8_t  acdcpolarity;     // GPIO polarity for AC/DC switching
  uint8_t  vr0hotgpio;       // GPIO pin configured for VR0 HOT event
  uint8_t  vr0hotpolarity;   // GPIO polarity for VR0 HOT event

  uint8_t  vr1hotgpio;       // GPIO pin configured for VR1 HOT event
  uint8_t  vr1hotpolarity;   // GPIO polarity for VR1 HOT event
  uint8_t  padding1;         // replace GPIO pin configured for CTF
  uint8_t  padding2;         // replace GPIO polarity for CTF

  // LED Display Settings
  uint8_t  ledpin0;          // GPIO number for LedPin[0]
  uint8_t  ledpin1;          // GPIO number for LedPin[1]
  uint8_t  ledpin2;          // GPIO number for LedPin[2]
  uint8_t  padding8_4;

  // GFXCLK Spread Spectrum
  uint8_t  pllgfxclkspreadenabled;   // on or off
  uint8_t  pllgfxclkspreadpercent;   // Q4.4
  uint16_t pllgfxclkspreadfreq;      // kHz

  // UCLK Spread Spectrum
  uint8_t  uclkspreadenabled;       // on or off
  uint8_t  uclkspreadpercent;      // Q4.4
  uint16_t uclkspreadfreq;           // kHz

  // FCLK Spread Spectrum
  uint8_t  fclkspreadenabled;   // on or off
  uint8_t  fclkspreadpercent;   // Q4.4
  uint16_t fclkspreadfreq;      // kHz

  // GFXCLK Fll Spread Spectrum
  uint8_t  fllgfxclkspreadenabled;   // on or off
  uint8_t  fllgfxclkspreadpercent;   // Q4.4
  uint16_t fllgfxclkspreadfreq;      // kHz

  // I2C Controller Structure
  SMUDPM_I2CCONTROLLERCONFIG_T  i2ccontrollers[7];

  // reserved
  uint32_t boardreserved[10];
} ATOM_SMC_DPM_INFO_V4_4;


typedef enum {
    SMC_V4_5_I2C_CONTROLLER_NAME_VR_GFX = 0,
    SMC_V4_5_I2C_CONTROLLER_NAME_VR_SOC,
    SMC_V4_5_I2C_CONTROLLER_NAME_VR_VDDCI,
    SMC_V4_5_I2C_CONTROLLER_NAME_VR_MVDD,
    SMC_V4_5_I2C_CONTROLLER_NAME_LIQUID0,
    SMC_V4_5_I2C_CONTROLLER_NAME_LIQUID1,
    SMC_V4_5_I2C_CONTROLLER_NAME_PLX,
    SMC_V4_5_I2C_CONTROLLER_NAME_SPARE,
    SMC_V4_5_I2C_CONTROLLER_NAME_COUNT,
} SMUDPM_V4_5_I2CCONTROLLERNAME_E;

typedef enum {
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_TYPE_NONE = 0,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_VR_GFX,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_VR_SOC,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_VR_VDDCI,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_VR_MVDD,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_LIQUID0,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_LIQUID1,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_PLX,
    SMC_V4_5_I2C_CONTROLLER_THROTTLER_COUNT,
} SMUDPM_V4_5_I2CCONTROLLERTHROTTLER_E;

typedef enum {
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_VR_0,     // XPDE132G5
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_VR_1,     // IR35217
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_TMP_0,
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_TMP_1,
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_SPARE_0,
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_SPARE_1,
    SMC_V4_5_I2C_CONTROLLER_PROTOCOL_COUNT,
} SMUDPM_V4_5_I2CCONTROLLERPROTOCOL_E;

typedef struct _smudpm_i2c_controller_config_v2 {
    uint8_t   Enabled;
    uint8_t   Speed;
    uint8_t   Padding[2];
    uint32_t  SlaveAddress;
    uint8_t   ControllerPort;
    uint8_t   ControllerName;
    uint8_t   ThermalThrotter;
    uint8_t   I2cProtocol;
} SMUDPM_I2C_CONTROLLER_CONFIG_V2;

typedef struct _atom_smc_dpm_info_v4_5 {
  ATOM_COMMON_TABLE_HEADER  table_header;
    // SECTION: BOARD PARAMETERS
    // I2C Control
  SMUDPM_I2C_CONTROLLER_CONFIG_V2  I2cControllers[8];

  // SVI2 Board Parameters
  uint16_t     MaxVoltageStepGfx; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.
  uint16_t     MaxVoltageStepSoc; // In mV(Q2) Max voltage step that SMU will request. Multiple steps are taken if voltage change exceeds this value.

  uint8_t      VddGfxVrMapping;   // Use VR_MAPPING* bitfields
  uint8_t      VddSocVrMapping;   // Use VR_MAPPING* bitfields
  uint8_t      VddMem0VrMapping;  // Use VR_MAPPING* bitfields
  uint8_t      VddMem1VrMapping;  // Use VR_MAPPING* bitfields

  uint8_t      GfxUlvPhaseSheddingMask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t      SocUlvPhaseSheddingMask; // set this to 1 to set PSI0/1 to 1 in ULV mode
  uint8_t      ExternalSensorPresent; // External RDI connected to TMON (aka TEMP IN)
  uint8_t      Padding8_V;

  // Telemetry Settings
  uint16_t     GfxMaxCurrent;   // in Amps
  uint8_t      GfxOffset;       // in Amps
  uint8_t      Padding_TelemetryGfx;
  uint16_t     SocMaxCurrent;   // in Amps
  uint8_t      SocOffset;       // in Amps
  uint8_t      Padding_TelemetrySoc;

  uint16_t     Mem0MaxCurrent;   // in Amps
  uint8_t      Mem0Offset;       // in Amps
  uint8_t      Padding_TelemetryMem0;

  uint16_t     Mem1MaxCurrent;   // in Amps
  uint8_t      Mem1Offset;       // in Amps
  uint8_t      Padding_TelemetryMem1;

  // GPIO Settings
  uint8_t      AcDcGpio;        // GPIO pin configured for AC/DC switching
  uint8_t      AcDcPolarity;    // GPIO polarity for AC/DC switching
  uint8_t      VR0HotGpio;      // GPIO pin configured for VR0 HOT event
  uint8_t      VR0HotPolarity;  // GPIO polarity for VR0 HOT event

  uint8_t      VR1HotGpio;      // GPIO pin configured for VR1 HOT event
  uint8_t      VR1HotPolarity;  // GPIO polarity for VR1 HOT event
  uint8_t      GthrGpio;        // GPIO pin configured for GTHR Event
  uint8_t      GthrPolarity;    // replace GPIO polarity for GTHR

  // LED Display Settings
  uint8_t      LedPin0;         // GPIO number for LedPin[0]
  uint8_t      LedPin1;         // GPIO number for LedPin[1]
  uint8_t      LedPin2;         // GPIO number for LedPin[2]
  uint8_t      padding8_4;

  // GFXCLK PLL Spread Spectrum
  uint8_t      PllGfxclkSpreadEnabled;   // on or off
  uint8_t      PllGfxclkSpreadPercent;   // Q4.4
  uint16_t     PllGfxclkSpreadFreq;      // kHz

  // GFXCLK DFLL Spread Spectrum
  uint8_t      DfllGfxclkSpreadEnabled;   // on or off
  uint8_t      DfllGfxclkSpreadPercent;   // Q4.4
  uint16_t     DfllGfxclkSpreadFreq;      // kHz

  // UCLK Spread Spectrum
  uint8_t      UclkSpreadEnabled;   // on or off
  uint8_t      UclkSpreadPercent;   // Q4.4
  uint16_t     UclkSpreadFreq;      // kHz

  // SOCCLK Spread Spectrum
  uint8_t      SoclkSpreadEnabled;   // on or off
  uint8_t      SocclkSpreadPercent;   // Q4.4
  uint16_t     SocclkSpreadFreq;      // kHz

  // Total board power
  uint16_t     TotalBoardPower;     //Only needed for TCP Estimated case, where TCP = TGP+Total Board Power
  uint16_t     BoardPadding;

  // Mvdd Svi2 Div Ratio Setting
  uint32_t     MvddRatio; // This is used for MVDD Vid workaround. It has 16 fractional bits (Q16.16)

  // Reneses VR workaround
  uint8_t      RenesesLoadLineEnabled;
  uint8_t      GfxLoadlineResistance;
  uint8_t      SocLoadlineResistance;
  uint8_t      Padding8_Loadline;

  uint32_t     BoardReserved[8];
} ATOM_SMC_DPM_INFO_V4_5;

/*
  ***************************************************************************
    Data Table asic_profiling_info  structure
  ***************************************************************************
*/
typedef struct _atom_asic_profiling_info_v4_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t  maxvddc;                              //Maximum voltage for all parts, in unit of 0.001mv;
  uint32_t  minvddc;                              //Minimum voltage for all parts, in unit of 0.001mv;
  uint32_t  avfs_meannsigma_acontant0;
  uint32_t  avfs_meannsigma_acontant1;
  uint32_t  avfs_meannsigma_acontant2;
  uint16_t  avfs_meannsigma_dc_tol_sigma;
  uint16_t  avfs_meannsigma_platform_mean;
  uint16_t  avfs_meannsigma_platform_sigma;
  uint32_t  gb_vdroop_table_cksoff_a0;
  uint32_t  gb_vdroop_table_cksoff_a1;
  uint32_t  gb_vdroop_table_cksoff_a2;
  uint32_t  gb_vdroop_table_ckson_a0;
  uint32_t  gb_vdroop_table_ckson_a1;
  uint32_t  gb_vdroop_table_ckson_a2;
  uint32_t  avfsgb_fuse_table_cksoff_m1;
  uint32_t  avfsgb_fuse_table_cksoff_m2;
  uint32_t  avfsgb_fuse_table_cksoff_b;
  uint32_t  avfsgb_fuse_table_ckson_m1;
  uint32_t  avfsgb_fuse_table_ckson_m2;
  uint32_t  avfsgb_fuse_table_ckson_b;
  uint16_t  max_voltage_0_25mv;
  uint8_t   enable_gb_vdroop_table_cksoff;
  uint8_t   enable_gb_vdroop_table_ckson;
  uint8_t   enable_gb_fuse_table_cksoff;
  uint8_t   enable_gb_fuse_table_ckson;
  uint16_t  psm_age_comfactor;
  uint8_t   enable_apply_avfs_cksoff_voltage;
  uint8_t   reserved;
  uint32_t  dispclk2gfxclk_a;
  uint32_t  dispclk2gfxclk_b;
  uint32_t  dispclk2gfxclk_c;
  uint32_t  pixclk2gfxclk_a;
  uint32_t  pixclk2gfxclk_b;
  uint32_t  pixclk2gfxclk_c;
  uint32_t  dcefclk2gfxclk_a;
  uint32_t  dcefclk2gfxclk_b;
  uint32_t  dcefclk2gfxclk_c;
  uint32_t  phyclk2gfxclk_a;
  uint32_t  phyclk2gfxclk_b;
  uint32_t  phyclk2gfxclk_c;
} ATOM_ASIC_PROFILING_INFO_V4_1;

typedef struct _atom_asic_profiling_info_v4_2 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t  maxvddc;                              //Maximum voltage for all parts, in unit of 0.001mv;
  uint32_t  minvddc;                              //Minimum voltage for all parts, in unit of 0.001mv;
  uint32_t  avfs_meannsigma_acontant0;
  uint32_t  avfs_meannsigma_acontant1;
  uint32_t  avfs_meannsigma_acontant2;
  uint16_t  avfs_meannsigma_dc_tol_sigma;
  uint16_t  avfs_meannsigma_platform_mean;
  uint16_t  avfs_meannsigma_platform_sigma;
  uint32_t  gb_vdroop_table_cksoff_a0;
  uint32_t  gb_vdroop_table_cksoff_a1;
  uint32_t  gb_vdroop_table_cksoff_a2;
  uint32_t  gb_vdroop_table_ckson_a0;
  uint32_t  gb_vdroop_table_ckson_a1;
  uint32_t  gb_vdroop_table_ckson_a2;
  uint32_t  avfsgb_fuse_table_cksoff_m1;
  uint32_t  avfsgb_fuse_table_cksoff_m2;
  uint32_t  avfsgb_fuse_table_cksoff_b;
  uint32_t  avfsgb_fuse_table_ckson_m1;
  uint32_t  avfsgb_fuse_table_ckson_m2;
  uint32_t  avfsgb_fuse_table_ckson_b;
  uint16_t  max_voltage_0_25mv;
  uint8_t   enable_gb_vdroop_table_cksoff;
  uint8_t   enable_gb_vdroop_table_ckson;
  uint8_t   enable_gb_fuse_table_cksoff;
  uint8_t   enable_gb_fuse_table_ckson;
  uint16_t  psm_age_comfactor;
  uint8_t   enable_apply_avfs_cksoff_voltage;
  uint8_t   reserved;
  uint32_t  dispclk2gfxclk_a;
  uint32_t  dispclk2gfxclk_b;
  uint32_t  dispclk2gfxclk_c;
  uint32_t  pixclk2gfxclk_a;
  uint32_t  pixclk2gfxclk_b;
  uint32_t  pixclk2gfxclk_c;
  uint32_t  dcefclk2gfxclk_a;
  uint32_t  dcefclk2gfxclk_b;
  uint32_t  dcefclk2gfxclk_c;
  uint32_t  phyclk2gfxclk_a;
  uint32_t  phyclk2gfxclk_b;
  uint32_t  phyclk2gfxclk_c;
  uint32_t  acg_gb_vdroop_table_a0;
  uint32_t  acg_gb_vdroop_table_a1;
  uint32_t  acg_gb_vdroop_table_a2;
  uint32_t  acg_avfsgb_fuse_table_m1;
  uint32_t  acg_avfsgb_fuse_table_m2;
  uint32_t  acg_avfsgb_fuse_table_b;
  uint8_t   enable_acg_gb_vdroop_table;
  uint8_t   enable_acg_gb_fuse_table;
  uint32_t  acg_dispclk2gfxclk_a;
  uint32_t  acg_dispclk2gfxclk_b;
  uint32_t  acg_dispclk2gfxclk_c;
  uint32_t  acg_pixclk2gfxclk_a;
  uint32_t  acg_pixclk2gfxclk_b;
  uint32_t  acg_pixclk2gfxclk_c;
  uint32_t  acg_dcefclk2gfxclk_a;
  uint32_t  acg_dcefclk2gfxclk_b;
  uint32_t  acg_dcefclk2gfxclk_c;
  uint32_t  acg_phyclk2gfxclk_a;
  uint32_t  acg_phyclk2gfxclk_b;
  uint32_t  acg_phyclk2gfxclk_c;
} ATOM_ASIC_PROFILING_INFO_V4_2;


/*
  ***************************************************************************
    Data Table multimedia_info  structure
  ***************************************************************************
*/
typedef struct _atom_multimedia_info_v2_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint8_t uvdip_min_ver;
  uint8_t uvdip_max_ver;
  uint8_t vceip_min_ver;
  uint8_t vceip_max_ver;
  uint16_t uvd_enc_max_input_width_pixels;
  uint16_t uvd_enc_max_input_height_pixels;
  uint16_t vce_enc_max_input_width_pixels;
  uint16_t vce_enc_max_input_height_pixels;
  uint32_t uvd_enc_max_bandwidth;           // 16x16 pixels/sec, codec independent
  uint32_t vce_enc_max_bandwidth;           // 16x16 pixels/sec, codec independent
} ATOM_MULTIMEDIA_INFO_V2_1;


/*
  ***************************************************************************
    Data Table umc_info  structure
  ***************************************************************************
*/
typedef struct _atom_umc_info_v3_1 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t ucode_version;
  uint32_t ucode_rom_startaddr;
  uint32_t ucode_length;
  uint16_t umc_reg_init_offset;
  uint16_t customer_ucode_name_offset;
  uint16_t mclk_ss_percentage;
  uint16_t mclk_ss_rate_10hz;
  uint8_t umcip_min_ver;
  uint8_t umcip_max_ver;
  uint8_t vram_type;              //atom_dgpu_vram_type, for APU products, memory type is defined in memorytype.integrate info
  uint8_t umc_config;
  uint32_t mem_refclk_10khz;
} ATOM_UMC_INFO_V3_1;
//IPCLEAN_END
// umc_info.umc_config
typedef enum {
  UMC_CONFIG__ENABLE_1KB_INTERLEAVE_MODE  =   0x00000001,
  UMC_CONFIG__DEFAULT_MEM_ECC_ENABLE      =   0x00000002,
  UMC_CONFIG__ENABLE_HBM_LANE_REPAIR      =   0x00000004,
  UMC_CONFIG__ENABLE_BANK_HARVESTING      =   0x00000008,
  UMC_CONFIG__ENABLE_PHY_REINIT           =   0x00000010,
  UMC_CONFIG__DISABLE_UCODE_CHKSTATUS     =   0x00000020,
  UMC_CONFIG__HOLD_UCODE_INIT             =   0x00000040,
  UMC_CONFIG__HOLD_UCODE_TRAIN            =   0x00000080,
} ATOM_UMC_CONFIG_DEF;
//IPCLEAN_START
typedef struct _atom_umc_info_v3_2 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t ucode_version;
  uint32_t ucode_rom_startaddr;
  uint32_t ucode_length;
  uint16_t umc_reg_init_offset;
  uint16_t customer_ucode_name_offset;
  uint16_t mclk_ss_percentage;
  uint16_t mclk_ss_rate_10hz;
  uint8_t umcip_min_ver;
  uint8_t umcip_max_ver;
  uint8_t vram_type;              //enum of atom_dgpu_vram_type
  uint8_t umc_config;
  uint32_t mem_refclk_10khz;
  uint32_t pstate_uclk_10khz[4];
  uint16_t umcgoldenoffset;
  uint16_t densitygoldenoffset;
} ATOM_UMC_INFO_V3_2;
//IPCLEAN_END
typedef struct _atom_umc_info_v3_3 {
  ATOM_COMMON_TABLE_HEADER  table_header;
  uint32_t ucode_reserved;
  uint32_t ucode_rom_startaddr;
  uint32_t ucode_length;
  uint16_t umc_reg_init_offset;
  uint16_t customer_ucode_name_offset;
  uint16_t mclk_ss_percentage;
  uint16_t mclk_ss_rate_10hz;
  uint8_t umcip_min_ver;
  uint8_t umcip_max_ver;
  uint8_t vram_type;              //enum of atom_dgpu_vram_type
  uint8_t umc_config;
  uint32_t mem_refclk_10khz;
  uint32_t pstate_uclk_10khz[4];
  uint16_t umcgoldenoffset;
  uint16_t densitygoldenoffset;
  uint32_t umc_config1;
  uint32_t bist_data_startaddr;
  uint32_t reserved[2];
} ATOM_UMC_INFO_V3_3;

typedef enum {
  UMC_CONFIG1__ENABLE_PSTATE_PHASE_STORE_TRAIN = 0x00000001,
  UMC_CONFIG1__ENABLE_AUTO_FRAMING = 0x00000002,
  UMC_CONFIG1__ENABLE_RESTORE_BIST_DATA = 0x00000004,
  UMC_CONFIG1__DISABLE_STROBE_MODE = 0x00000008,
} ATOM_UMC_CONFIG1_DEF;


/*
  ***************************************************************************
    Data Table vram_info  structure
  ***************************************************************************
*/
typedef struct _atom_vram_module_v9 {
  // Design Specific Values
  uint32_t  memory_size;                   // Total memory size in unit of MB for CONFIG_MEMSIZE zeros
  uint32_t  channel_enable;                // bit vector, each bit indicate specific channel enable or not
  uint32_t  max_mem_clk;                   // max memory clock of this memory in unit of 10kHz, =0 means it is not defined
  uint16_t  reserved[3];
  uint16_t  mem_voltage;                   // mem_voltage
  uint16_t  vram_module_size;              // Size of atom_vram_module_v9
  uint8_t   ext_memory_id;                 // Current memory module ID
  uint8_t   memory_type;                   // enum of atom_dgpu_vram_type
  uint8_t   channel_num;                   // Number of mem. channels supported in this module
  uint8_t   channel_width;                 // CHANNEL_16BIT/CHANNEL_32BIT/CHANNEL_64BIT
  uint8_t   density;                       // _8Mx32, _16Mx32, _16Mx16, _32Mx16
  uint8_t   tunningset_id;                 // MC phy registers set per.
  uint8_t   vender_rev_id;                 // [7:4] Revision, [3:0] Vendor code
  uint8_t   refreshrate;                   // [1:0]=RefreshFactor (00=8ms, 01=16ms, 10=32ms,11=64ms)
  uint8_t   hbm_ven_rev_id;                 // hbm_ven_rev_id
  uint8_t   vram_rsd2;                     // reserved
  char    dram_pnstring[20];               // part number end with '0'.
} ATOM_VRAM_MODULE_V9;

//IPCLEAN_START
typedef struct _atom_vram_info_header_v2_3 {
  ATOM_COMMON_TABLE_HEADER table_header;
  uint16_t mem_adjust_tbloffset;                         // offset of atom_umc_init_reg_block structure for memory vendor specific UMC adjust setting
  uint16_t mem_clk_patch_tbloffset;                      // offset of atom_umc_init_reg_block structure for memory clock specific UMC setting
  uint16_t mc_adjust_pertile_tbloffset;                  // offset of atom_umc_init_reg_block structure for Per Byte Offset Preset Settings
  uint16_t mc_phyinit_tbloffset;                         // offset of atom_umc_init_reg_block structure for MC phy init set
  uint16_t dram_data_remap_tbloffset;                    // reserved for now
  uint16_t tmrs_seq_offset;                              // offset of HBM tmrs
  uint16_t post_ucode_init_offset;                       // offset of atom_umc_init_reg_block structure for MC phy init after MC uCode complete umc init
  uint16_t vram_rsd2;
  uint8_t  vram_module_num;                              // indicate number of VRAM module
  uint8_t  umcip_min_ver;
  uint8_t  umcip_max_ver;
  uint8_t  mc_phy_tile_num;                              // indicate the MCD tile number which use in DramDataRemapTbl and usMcAdjustPerTileTblOffset
  ATOM_VRAM_MODULE_V9  vram_module[16];         // just for allocation, real number of blocks is in ucNumOfVRAMModule;
} ATOM_VRAM_INFO_HEADER_V2_3;
//IPCLEAN_END
typedef struct _atom_umc_register_addr_info {
  uint32_t  umc_register_addr:24;
  uint32_t  umc_reg_type_ind:1;
  uint32_t  umc_ind_reg_type:2;
  uint32_t  umc_reg_rsvd:5;
} ATOM_UMC_REGISTER_ADDR_INFO;

//atom_umc_register_addr_info.
typedef enum {
  b3ATOM_UMC_REG_ADD_INFO_INDIRECT_ACCESS  =0x01,
  b3ATOM_UMC_REG_ADD_INFO_IND_SRAM_ACCESS  =0x01,
  b3ATOM_UMC_REG_ADD_INFO_IND_MPHY_ACCESS  =0x03,
  b3ATOM_UMC_REG_ADD_INFO_IND_DPHY_ACCESS  =0x05,
} ATOM_UMC_REGISTER_ADDR_INFO_FLAG;
//IPCLEAN_START
//atom_umc_ind_register_type.
typedef enum {
  ATOM_UMC_IND_REG_MP_SRAM_TYPE = 0x00,
  ATOM_UMC_IND_REG_MPHY_TYPE = 0x01,
  ATOM_UMC_IND_REG_DPHY_TYPE = 0x02,
} ATOM_UMC_IND_REGISTER_TYPE;
//IPCLEAN_END

typedef union {
  ATOM_UMC_REGISTER_ADDR_INFO umc_reg_addr;
  uint32_t u32umc_reg_addr;
} ATOM_UMC_REGISTER_ADDR_INFO_ACCESS;

typedef struct _atom_umc_reg_setting_id_config {
  uint32_t memclockrange:24;
  uint32_t mem_blk_id:8;
} ATOM_UMC_REG_SETTING_ID_CONFIG;

typedef union {
  ATOM_UMC_REG_SETTING_ID_CONFIG umc_id_access;
  uint32_t  u32umc_id_access;
} ATOM_UMC_REG_SETTING_ID_CONFIG_ACCESS;

typedef struct _atom_umc_reg_setting_data_block{
  ATOM_UMC_REG_SETTING_ID_CONFIG_ACCESS  block_id;
  uint32_t u32umc_reg_data[1];
} ATOM_UMC_REG_SETTING_DATA_BLOCK;

typedef struct _atom_umc_init_reg_block{
  uint16_t umc_reg_num;
  uint16_t reserved;
  ATOM_UMC_REGISTER_ADDR_INFO_ACCESS umc_reg_list[1];     //for allocation purpose, the real number come from umc_reg_num;
  ATOM_UMC_REG_SETTING_DATA_BLOCK umc_reg_setting_list[1];
} ATOM_UMC_INIT_REG_BLOCK;

//IPCLEAN_START
typedef struct _atom_vram_module_v10 {
  // Design Specific Values
  uint32_t  memory_size;                   // Total memory size in unit of MB for CONFIG_MEMSIZE zeros
  uint32_t  channel_enable;                // bit vector, each bit indicate specific channel enable or not
  uint32_t  max_mem_clk;                   // max memory clock of this memory in unit of 10kHz, =0 means it is not defined
  uint16_t  reserved[3];
  uint16_t  mem_voltage;                   // mem_voltage
  uint16_t  vram_module_size;              // Size of atom_vram_module_v9
  uint8_t   ext_memory_id;                 // Current memory module ID
  uint8_t   memory_type;                   // enum of atom_dgpu_vram_type
  uint8_t   channel_num;                   // Number of mem. channels supported in this module
  uint8_t   channel_width;                 // CHANNEL_16BIT/CHANNEL_32BIT/CHANNEL_64BIT
  uint8_t   density;                       // _8Mx32, _16Mx32, _16Mx16, _32Mx16
  uint8_t   tunningset_id;                 // MC phy registers set per.
  uint8_t   vender_rev_id;                 // [7:4] Revision, [3:0] Vendor code
  uint8_t   refreshrate;                   // [1:0]=RefreshFactor (00=8ms, 01=16ms, 10=32ms,11=64ms)
  uint8_t   vram_flags;                     // bit0= bankgroup enable
  uint8_t   vram_rsd2;                     // reserved
  uint16_t  gddr6_mr10;                    // gddr6 mode register10 value
  uint16_t  gddr6_mr1;                     // gddr6 mode register1 value
  uint16_t  gddr6_mr2;                     // gddr6 mode register2 value
  uint16_t  gddr6_mr7;                     // gddr6 mode register7 value
  char    dram_pnstring[20];               // part number end with '0'.
} ATOM_VRAM_MODULE_V10;

typedef struct _atom_vram_info_header_v2_4 {
  ATOM_COMMON_TABLE_HEADER table_header;
  uint16_t mem_adjust_tbloffset;                         // offset of atom_umc_init_reg_block structure for memory vendor specific UMC adjust settings
  uint16_t mem_clk_patch_tbloffset;                      // offset of atom_umc_init_reg_block structure for memory clock specific UMC settings
  uint16_t mc_adjust_pertile_tbloffset;                  // offset of atom_umc_init_reg_block structure for Per Byte Offset Preset Settings
  uint16_t mc_phyinit_tbloffset;                         // offset of atom_umc_init_reg_block structure for MC phy init set
  uint16_t dram_data_remap_tbloffset;                    // offset of ATOM_DRAM_DATA_REMAP array to indicate DRAM data lane to GPU mapping
  uint16_t reserved;                                     // offset of reserved
  uint16_t post_ucode_init_offset;                       // offset of atom_umc_init_reg_block structure for MC phy init after MC uCode complete umc init
  uint16_t strobe_mode_patch_tbloffset;                  // offset of atom_umc_init_reg_block structure for Strobe Mode memory clock specific UMC settings
  uint8_t  vram_module_num;                              // indicate number of VRAM module
  uint8_t  umcip_min_ver;
  uint8_t  umcip_max_ver;
  uint8_t  mc_phy_tile_num;                              // indicate the MCD tile number which use in DramDataRemapTbl and usMcAdjustPerTileTblOffset
  ATOM_VRAM_MODULE_V10  vram_module[16];        // just for allocation, real number of blocks is in ucNumOfVRAMModule;
} ATOM_VRAM_INFO_HEADER_V2_4;



/*
  ***************************************************************************
    Data Table voltageobject_info  structure
  ***************************************************************************
*/
typedef struct  _atom_i2c_data_entry {
  uint16_t  i2c_reg_index;               // i2c register address, can be up to 16bit
  uint16_t  i2c_reg_data;                // i2c register data, can be up to 16bit
} ATOM_I2C_DATA_ENTRY;

typedef struct _atom_voltage_object_header_v4 {
  uint8_t    voltage_type;                           //enum atom_voltage_type
  uint8_t    voltage_mode;                           //enum atom_voltage_object_mode
  uint16_t   object_size;                            //Size of Object
} ATOM_VOLTAGE_OBJECT_HEADER_V4;

// atom_voltage_object_header_v4.voltage_mode
typedef enum {
   VOLTAGE_OBJ_GPIO_LUT              =  0,        //VOLTAGE and GPIO Lookup table ->atom_gpio_voltage_object_v4
   VOLTAGE_OBJ_VR_I2C_INIT_SEQ       =  3,        //VOLTAGE REGULATOR INIT sequece through I2C -> atom_i2c_voltage_object_v4
   VOLTAGE_OBJ_PHASE_LUT             =  4,        //Set Vregulator Phase lookup table ->atom_gpio_voltage_object_v4
   VOLTAGE_OBJ_SVID2                 =  7,        //Indicate voltage control by SVID2 ->atom_svid2_voltage_object_v4
   VOLTAGE_OBJ_EVV                   =  8,
   VOLTAGE_OBJ_MERGED_POWER          =  9,
} ATOM_VOLTAGE_OBJECT_MODE;

typedef struct _atom_i2c_voltage_object_v4 {
   ATOM_VOLTAGE_OBJECT_HEADER_V4 header;  // voltage mode = VOLTAGE_OBJ_VR_I2C_INIT_SEQ
   uint8_t  regulator_id;                        //Indicate Voltage Regulator Id
   uint8_t  i2c_id;
   uint8_t  i2c_slave_addr;
   uint8_t  i2c_control_offset;
   uint8_t  i2c_flag;                            // Bit0: 0 - One byte data; 1 - Two byte data
   uint8_t  i2c_speed;                           // =0, use default i2c speed, otherwise use it in unit of kHz.
   uint8_t  reserved[2];
   ATOM_I2C_DATA_ENTRY i2cdatalut[1];     // end with 0xff
} ATOM_I2C_VOLTAGE_OBJECT_V4;

// ATOM_I2C_VOLTAGE_OBJECT_V3.ucVoltageControlFlag
typedef enum {
   VOLTAGE_DATA_ONE_BYTE = 0,
   VOLTAGE_DATA_TWO_BYTE = 1,
} ATOM_I2C_VOLTAGE_CONTROL_FLAG;


typedef struct atom_voltage_gpio_map_lut
{
  uint32_t  voltage_gpio_reg_val;              // The Voltage ID which is used to program GPIO register
  uint16_t  voltage_level_mv;                  // The corresponding Voltage Value, in mV
} ATOM_VOLTAGE_GPIO_MAP_LUT;

typedef struct _atom_gpio_voltage_object_v4 {
   ATOM_VOLTAGE_OBJECT_HEADER_V4 header;  // voltage mode = VOLTAGE_OBJ_GPIO_LUT or VOLTAGE_OBJ_PHASE_LUT
   uint8_t  gpio_control_id;                     // default is 0 which indicate control through CG VID mode
   uint8_t  gpio_entry_num;                      // indiate the entry numbers of Votlage/Gpio value Look up table
   uint8_t  phase_delay_us;                      // phase delay in unit of micro second
   uint8_t  reserved;
   uint32_t gpio_mask_val;                         // GPIO Mask value
   ATOM_VOLTAGE_GPIO_MAP_LUT voltage_gpio_lut[1];
} ATOM_GPIO_VOLTAGE_OBJECT_V4;

typedef struct _atom_svid2_voltage_object_v4 {
   ATOM_VOLTAGE_OBJECT_HEADER_V4 header;  // voltage mode = VOLTAGE_OBJ_SVID2
   uint8_t loadline_psi1;                        // bit4:0= loadline setting ( Core Loadline trim and offset trim ), bit5=0:PSI1_L disable =1: PSI1_L enable
   uint8_t psi0_l_vid_thresd;                    // VR PSI0_L VID threshold
   uint8_t psi0_enable;                          //
   uint8_t maxvstep;
   uint8_t telemetry_offset;
   uint8_t telemetry_gain;
   uint16_t reserved1;
} ATOM_SVID2_VOLTAGE_OBJECT_V4;

typedef struct _atom_svid2_voltage_object_v4_2 {
   ATOM_VOLTAGE_OBJECT_HEADER_V4 header;  // voltage mode = VOLTAGE_OBJ_SVID2
   uint8_t  loadline;                             // bit4:0 loadlie setting ( Core Loadline trim and offset trim )
   uint8_t  psi0_l_vid_thresd;                    // VR PSI0_L VID threshold
   uint8_t  psi_enable;                           // enum of atom_psi_enable_def_v4_2
   uint8_t  svi_plane_id;                         // enum of atom_svi_plane_id_def
   uint32_t reserved;                             // reserved, maxstep move to smc_dpm_info table
} ATOM_SVID2_VOLTAGE_OBJECT_V4_2;

// svi_plane_id
typedef enum {
   SMUSVI0_PLANE0_ID = 0,
   SMUSVI0_PLANE1_ID = 2,
   SMUSVI1_PLANE0_ID = 1,
   SMUSVI1_PLANE1_ID = 3,
} ATOM_SVI_PLANE_ID_DEF;
//IPCLEAN_END
typedef enum {
  SMUSVI_BOOTUP_PSI0_ENABLE_MASK  = 0x01,         // bit0: ASIC bootup ( VBIOS asic_init ), PSI0 enable bit
  SMUSVI_BOOTUP_PSI1_ENABLE_MASK  = 0x02,         // bit1: ASIC bootup ( VBIOS asic_init ), PSI1 enable bit
} ATOM_PSI_ENABLE_DEF_V4_2;
//IPCLEAN_START
typedef struct _atom_merged_voltage_object_v4 {
  ATOM_VOLTAGE_OBJECT_HEADER_V4 header;  // voltage mode = VOLTAGE_OBJ_MERGED_POWER
  uint8_t  merged_powerrail_type;               //enum atom_voltage_type
  uint8_t  reserved[3];
} ATOM_MERGED_VOLTAGE_OBJECT_V4;

typedef union {
  ATOM_GPIO_VOLTAGE_OBJECT_V4 gpio_voltage_obj;
  ATOM_I2C_VOLTAGE_OBJECT_V4 i2c_voltage_obj;
  ATOM_SVID2_VOLTAGE_OBJECT_V4 svid2_voltage_obj;
  ATOM_MERGED_VOLTAGE_OBJECT_V4 merged_voltage_obj;
} ATOM_VOLTAGE_OBJECT_V4;

typedef union {
  ATOM_GPIO_VOLTAGE_OBJECT_V4 gpio_voltage_obj;
  ATOM_I2C_VOLTAGE_OBJECT_V4 i2c_voltage_obj;
  ATOM_SVID2_VOLTAGE_OBJECT_V4_2 svid2_voltage_obj;
  ATOM_MERGED_VOLTAGE_OBJECT_V4 merged_voltage_obj;
} ATOM_VOLTAGE_OBJECT_V4_2;


typedef struct  _atom_voltage_objects_info_v4_1 {
  ATOM_COMMON_TABLE_HEADER table_header;
  ATOM_VOLTAGE_OBJECT_V4 voltage_object[1];   //Info for Voltage control
} ATOM_VOLTAGE_OBJECTS_INFO_V4_1;

typedef struct _atom_voltage_objects_info_v4_2 {
  ATOM_COMMON_TABLE_HEADER table_header;
  ATOM_VOLTAGE_OBJECT_V4_2 voltage_object[1];   //Info for Voltage control
} ATOM_VOLTAGE_OBJECTS_INFO_V4_2;


/*
  ***************************************************************************
              All Command Function structure definition
  ***************************************************************************
*/

/*
  ***************************************************************************
              Structures used by asic_init
  ***************************************************************************
*/

typedef struct _asic_init_engine_parameters {
  uint32_t sclkfreqin10khz:24;
  uint32_t engineflag:8;              /* enum atom_asic_init_engine_flag  */
} ASIC_INIT_ENGINE_PARAMETERS;

typedef struct _asic_init_mem_parameters {
  uint32_t mclkfreqin10khz:24;
  uint32_t memflag:8;                 /* enum atom_asic_init_mem_flag  */
} ASIC_INIT_MEM_PARAMETERS;

typedef struct _asic_init_parameters_v2_1 {
  ASIC_INIT_ENGINE_PARAMETERS engineparam;
  ASIC_INIT_MEM_PARAMETERS memparam;
} ASIC_INIT_PARAMETERS_V2_1;

typedef struct _asic_init_ps_allocation_v2_1 {
  ASIC_INIT_PARAMETERS_V2_1 param;
  uint32_t reserved[16];
} ASIC_INIT_PS_ALLOCATION_V2_1;


typedef enum {
  b3NORMAL_ENGINE_INIT = 0,
  b3SRIOV_SKIP_ASIC_INIT = 0x02,
  b3RECOVER_TDR_MODE2 = 0x04,
  b3SRIOV_LOAD_UCODE = 0x40,
} ATOM_ASIC_INIT_ENGINE_FLAG;

typedef enum {
  b3NORMAL_MEM_INIT = 0,
  b3DRAM_SELF_REFRESH_EXIT =0x20,
  b3MEM_SETTING_OVERRIDE= 0x01,         // caller need to set this bit if he want to override VBIOS default mem settings, such as ECC enable/disable, caller need to update bit1
  b3MEM_ECC_CTRL_ENABLE = 0x02,         // caller update ECC enable/disable to change VBIOS default ECC settings, caller need to set bit0=1
} ATOM_ASIC_INIT_MEM_FLAG;

/*
  ***************************************************************************
              Structures used by setengineclock
  ***************************************************************************
*/

typedef struct _set_engine_clock_parameters_v2_1 {
  uint32_t sclkfreqin10khz:24;
  uint32_t sclkflag:8;              /* enum atom_set_engine_mem_clock_flag,  */
  uint32_t reserved[10];
} SET_ENGINE_CLOCK_PARAMETERS_V2_1;

typedef struct _set_engine_clock_ps_allocation_v2_1 {
  SET_ENGINE_CLOCK_PARAMETERS_V2_1 clockinfo;
  uint32_t reserved[10];
} SET_ENGINE_CLOCK_PS_ALLOCATION_V2_1;


typedef enum {
  b3NORMAL_CHANGE_CLOCK = 0,
  b3FIRST_TIME_CHANGE_CLOCK = 0x08,
  b3STORE_DPM_TRAINGING = 0x40,         //Applicable to memory clock change,when set, it store specific DPM mode training result
} ATOM_SET_ENGINE_MEM_CLOCK_FLAG;

/*
  ***************************************************************************
              Structures used by getengineclock
  ***************************************************************************
*/
typedef struct _get_engine_clock_parameter {
  uint32_t sclk_10khz;          // current engine speed in 10KHz unit
  uint32_t reserved;
} GET_ENGINE_CLOCK_PARAMETER;

/*
  ***************************************************************************
              Structures used by setmemoryclock
  ***************************************************************************
*/
typedef struct _set_memory_clock_parameters_v2_1 {
  uint32_t mclkfreqin10khz:24;
  uint32_t mclkflag:8;              /* enum atom_set_engine_mem_clock_flag,  */
  uint32_t reserved[10];
} SET_MEMORY_CLOCK_PARAMETERS_V2_1;

typedef struct _set_memory_clock_ps_allocation_v2_1 {
  SET_MEMORY_CLOCK_PARAMETERS_V2_1 clockinfo;
  uint32_t reserved[10];
} SET_MEMORY_CLOCK_PS_ALLOCATION_V2_1;


/*
  ***************************************************************************
              Structures used by getmemoryclock
  ***************************************************************************
*/
typedef struct _get_memory_clock_parameter {
  uint32_t mclk_10khz;          // current engine speed in 10KHz unit
  uint32_t reserved;
} GET_MEMORY_CLOCK_PARAMETER;



/*
  ***************************************************************************
              Structures used by setvoltage
  ***************************************************************************
*/

typedef struct _set_voltage_parameters_v1_4 {
  uint8_t  voltagetype;                /* enum atom_voltage_type */
  uint8_t  command;                    /* Indicate action: Set voltage level, enum atom_set_voltage_command */
  uint16_t vlevel_mv;                  /* real voltage level in unit of mv or Voltage Phase (0, 1, 2, .. ) */
} SET_VOLTAGE_PARAMETERS_V1_4;

//set_voltage_parameters_v2_1.voltagemode
typedef enum {
  ATOM_SET_VOLTAGE  = 0,
  ATOM_INIT_VOLTAGE_REGULATOR = 3,
  ATOM_SET_VOLTAGE_PHASE = 4,
  ATOM_GET_LEAKAGE_ID    = 8,
} ATOM_SET_VOLTAGE_COMMAND;

typedef struct _set_voltage_ps_allocation_v1_4 {
  SET_VOLTAGE_PARAMETERS_V1_4 setvoltageparam;
  uint32_t reserved[10];
} SET_VOLTAGE_PS_ALLOCATION_V1_4;


/*
  ***************************************************************************
              Structures used by computegpuclockparam
  ***************************************************************************
*/

//ATOM_COMPUTE_CLOCK_FREQ.ulComputeClockFlag
typedef enum {
  COMPUTE_GPUCLK_INPUT_FLAG_DEFAULT_GPUCLK =0x00,
  COMPUTE_GPUCLK_INPUT_FLAG_GFXCLK =0x01,
  COMPUTE_GPUCLK_INPUT_FLAG_UCLK =0x02,
  COMPUTE_GPUCLK_INPUT_FLAG_DISPCLK = 0x03,
} ATOM_GPU_CLOCK_TYPE;

typedef struct _compute_gpu_clock_input_parameter_v1_8 {
  uint32_t  gpuclock_10khz:24;         //Input= target clock, output = actual clock
  uint32_t  gpu_clock_type:8;          //Input indicate clock type: enum atom_gpu_clock_type
  uint32_t  reserved[5];
} COMPUTE_GPU_CLOCK_INPUT_PARAMETER_V1_8;


typedef struct _compute_gpu_clock_output_parameter_v1_8 {
  uint32_t  gpuclock_10khz:24;              //Input= target clock, output = actual clock
  uint32_t  dfs_did:8;                      //return parameter: DFS divider which is used to program to register directly
  uint32_t  pll_fb_mult;                    //Feedback Multiplier, bit 8:0 int, bit 15:12 post_div, bit 31:16 frac
  uint32_t  pll_ss_fbsmult;                 // Spread FB Mult: bit 8:0 int, bit 31:16 frac
  uint16_t  pll_ss_slew_frac;
  uint8_t   pll_ss_enable;
  uint8_t   reserved;
  uint32_t  reserved1[2];
} COMPUTE_GPU_CLOCK_OUTPUT_PARAMETER_V1_8;



/*
  ***************************************************************************
              Structures used by ReadEfuseValue
  ***************************************************************************
*/

typedef struct _read_efuse_input_parameters_v3_1 {
  uint16_t efuse_start_index;
  uint8_t  reserved;
  uint8_t  bitslen;
} READ_EFUSE_INPUT_PARAMETERS_V3_1;

// ReadEfuseValue input/output parameter
typedef union {
  READ_EFUSE_INPUT_PARAMETERS_V3_1 efuse_info;
  uint32_t efusevalue;
} READ_EFUSE_VALUE_PARAMETERS_V3_1;


/*
  ***************************************************************************
              Structures used by getsmuclockinfo
  ***************************************************************************
*/
typedef struct _atom_get_smu_clock_info_parameters_v3_1 {
  uint8_t syspll_id;          // 0= syspll0, 1=syspll1, 2=syspll2
  uint8_t clk_id;             // atom_smu9_syspll0_clock_id  (only valid when command == GET_SMU_CLOCK_INFO_V3_1_GET_CLOCK_FREQ )
  uint8_t command;            // enum of atom_get_smu_clock_info_command
  uint8_t dfsdid;             // =0: get DFS DID from register, >0, give DFS divider, (only valid when command == GET_SMU_CLOCK_INFO_V3_1_GET_CLOCK_FREQ )
} ATOM_GET_SMU_CLOCK_INFO_PARAMETERS_V3_1;

typedef enum {
  GET_SMU_CLOCK_INFO_V3_1_GET_CLOCK_FREQ       = 0,
  GET_SMU_CLOCK_INFO_V3_1_GET_PLLVCO_FREQ      = 1,
  GET_SMU_CLOCK_INFO_V3_1_GET_PLLREFCLK_FREQ   = 2,
} ATOM_GET_SMU_CLOCK_INFO_COMMAND;

typedef enum {
  SMU9_SYSPLL0_SMNCLK_ID   = 0,       //  SMNCLK
  SMU9_SYSPLL0_SOCCLK_ID   = 1,       //  SOCCLK (FCLK)
  SMU9_SYSPLL0_MP0CLK_ID   = 2,       //  MP0CLK
  SMU9_SYSPLL0_MP1CLK_ID   = 3,       //  MP1CLK
  SMU9_SYSPLL0_LCLK_ID     = 4,       //  LCLK
  SMU9_SYSPLL0_DCLK_ID     = 5,       //  DCLK
  SMU9_SYSPLL0_VCLK_ID     = 6,       //  VCLK
  SMU9_SYSPLL0_ECLK_ID     = 7,       //  ECLK
  SMU9_SYSPLL0_DCEFCLK_ID  = 8,       //  DCEFCLK
  SMU9_SYSPLL0_DPREFCLK_ID = 10,      //  DPREFCLK
  SMU9_SYSPLL0_DISPCLK_ID  = 11,      //  DISPCLK
} ATOM_SMU9_SYSPLL0_CLOCK_ID;
//IPCLEAN_END
typedef enum {
  SMU11_SYSPLL0_ID            = 0,
  SMU11_SYSPLL1_0_ID          = 1,
  SMU11_SYSPLL1_1_ID          = 2,
  SMU11_SYSPLL1_2_ID          = 3,
  SMU11_SYSPLL2_ID            = 4,
  SMU11_SYSPLL3_0_ID          = 5,
  SMU11_SYSPLL3_1_ID          = 6,
} ATOM_SMU11_SYSPLL_ID;


typedef enum {
  SMU11_SYSPLL0_ECLK_ID     = 0,       //  ECLK
  SMU11_SYSPLL0_SOCCLK_ID   = 1,       //  SOCCLK
  SMU11_SYSPLL0_MP0CLK_ID   = 2,       //  MP0CLK
  SMU11_SYSPLL0_DCLK_ID     = 3,       //  DCLK
  SMU11_SYSPLL0_VCLK_ID     = 4,       //  VCLK
  SMU11_SYSPLL0_DCEFCLK_ID  = 5,       //  DCEFCLK
} ATOM_SMU11_SYSPLL0_CLOCK_ID;


typedef enum {
  SMU11_SYSPLL1_0_UCLKA_ID   = 0,       // UCLK_a
} ATOM_SMU11_SYSPLL1_0_CLOCK_ID;

typedef enum {
  SMU11_SYSPLL1_0_UCLKB_ID   = 0,       // UCLK_b
} atom_smu11_syspll1_1_clock_id;

typedef enum {
  SMU11_SYSPLL1_0_FCLK_ID   = 0,        // FCLK
} ATOM_SMU11_SYSPLL1_2_CLOCK_ID;

typedef enum  {
  SMU11_SYSPLL2_GFXCLK_ID   = 0,        // GFXCLK
} ATOM_SMU11_SYSPLL2_CLOCK_ID;

typedef enum {
  SMU11_SYSPLL3_0_DISPCLK_ID = 0,      //  DISPCLK
  SMU11_SYSPLL3_0_DPREFCLK_ID = 1,     //  DPREFCLK
  SMU11_SYSPLL3_0_WAFCLK_ID = 2,       //  WAFCLK
} ATOM_SMU11_SYSPLL3_0_CLOCK_ID;

typedef enum {
  SMU11_SYSPLL3_1_SMNCLK_ID = 0,       //  SMNCLK
  SMU11_SYSPLL3_1_MP1CLK_ID = 1,       //  MP1CLK
  SMU11_SYSPLL3_1_LCLK_ID = 2,         //  LCLK
} ATOM_SMU11_SYSPLL3_1_CLOCK_ID;

//IPCLEAN_START
// SMU CLK v11.1 ( NV11 ) clock mapping for each SYSPLL
typedef enum {
  SMU11_1_SYSPLL0_SMNCLK_ID   = 0,       //  SOCCLK/FCLK
  SMU11_1_SYSPLL0_SOCCLK_ID   = 1,       //  SOCCLK/FCLK
  SMU11_1_SYSPLL0_MP0CLK_ID   = 2,       //  MP0CLK
  SMU11_1_SYSPLL0_DCLK_ID     = 3,       //  DCLK
  SMU11_1_SYSPLL0_VCLK_ID     = 4,       //  VCLK
  SMU11_1_SYSPLL0_DCEFCLK_ID  = 5,       //  DCEFCLK
} ATOM_SMU11_1_SYSPLL0_CLOCK_ID;

typedef enum {
  SMU11_1_SYSPLL1_0_UCLKA_ID   = 0,       // UCLK_a
  SMU11_1_SYSPLL1_0_UCLKDIVA_ID= 1,       // UCLK_DIV_A
} ATOM_SMU11_1_SYSPLL1_0_CLOCK_ID;

typedef enum {
  SMU11_1_SYSPLL1_0_UCLKB_ID   = 0,       // UCLK_b
  SMU11_1_SYSPLL1_0_UCLKDIVB_ID= 1,       // UCLK_DIV_B
} ATOM_SMU11_1_SYSPLL1_1_CLOCK_ID;

typedef enum {
  SMU11_1_SYSPLL2_GFXCLK_ID   = 0,        // GFXCLK
} ATOM_SMU11_1_SYSPLL2_CLOCK_ID;

typedef enum {
  SMU11_1_SYSPLL3_0_DISPCLK_ID = 0,      //  DISPCLK
  SMU11_1_SYSPLL3_0_DPPCLK_ID = 1,       //  DPPCLK
  SMU11_1_SYSPLL3_0_DPREFCLK_ID = 2,     //  DPREFCLK
} ATOM_SMU11_1_SYSPLL3_0_CLOCK_ID;

typedef enum {
  SMU11_1_SYSPLL3_1_USBDFSCLK_ID = 0,    //  SMNCLK
  SMU11_1_SYSPLL3_1_MP1CLK_ID = 1,       //  MP1CLK
  SMU11_1_SYSPLL3_1_LCLK_ID = 2,         //  LCLK
} ATOM_SMU11_1_SYSPLL3_1_CLOCK_ID;

//IPCLEAN_END
typedef enum {
  SMU12_SYSPLL0_ID          = 0,
  SMU12_SYSPLL1_ID          = 1,
  SMU12_SYSPLL2_ID          = 2,
  SMU12_SYSPLL3_0_ID        = 3,
  SMU12_SYSPLL3_1_ID        = 4,
} ATOM_SMU12_SYSPLL_ID;

//IPCLEAN_START
// SMU CLK v12 ( RN ) clock mapping for each SYSPLL
typedef enum {
  SMU12_SYSPLL0_SMNCLK_ID   = 0,      //  SOCCLK
  SMU12_SYSPLL0_SOCCLK_ID   = 1,      //  SOCCLK
  SMU12_SYSPLL0_MP0CLK_ID   = 2,      //  MP0CLK
  SMU12_SYSPLL0_MP1CLK_ID   = 3,      //  MP1CLK
  SMU12_SYSPLL0_MP2CLK_ID   = 4,      //  MP2CLK
  SMU12_SYSPLL0_VCLK_ID     = 5,      //  VCLK
  SMU12_SYSPLL0_LCLK_ID     = 6,      //  LCLK
  SMU12_SYSPLL0_DCLK_ID     = 7,      //  DCLK
  SMU12_SYSPLL0_ACLK_ID     = 8,      //  ACLK
  SMU12_SYSPLL0_ISPCLK_ID   = 9,      //  ISPCLK
  SMU12_SYSPLL0_SHUBCLK_ID  = 10,      //  SHUBCLK
  SMU12_SYSPLL0_GFX_DLDO_ROCLK_ID  = 11,    //  GFX_DLDO_ROCLK
  SMU12_SYSPLL0_CCX_DLDO_ROCLK_ID  = 12,    //  CCX_DLDO_ROCLK
} ATOM_SMU12_SYSPLL0_CLOCK_ID;

typedef enum {
  SMU12_SYSPLL1_DISPCLK_ID  = 0,      //  DISPCLK
  SMU12_SYSPLL1_DPPCLK_ID   = 1,      //  DPPCLK
  SMU12_SYSPLL1_DPREFCLK_ID = 2,      //  DPREFCLK
  SMU12_SYSPLL1_DCFCLK_ID   = 3,      //  DCFCLK
} ATOM_SMU12_SYSPLL1_CLOCK_ID;

typedef enum {
  SMU12_SYSPLL2_Pre_GFXCLK_ID = 0,   // Pre_GFXCLK
} ATOM_SMU12_SYSPLL2_CLOCK_ID;

typedef enum {
  SMU12_SYSPLL3_0_FCLK_ID = 0,      //  FCLK
} ATOM_SMU12_SYSPLL3_0_CLOCK_ID;

typedef enum {
  SMU12_SYSPLL3_1_UMCCLK_ID = 0,    //  UMCCLK
} ATOM_SMU12_SYSPLL3_1_CLOCK_ID;

typedef struct _atom_get_smu_clock_info_output_parameters_v3_1
{
  union {
    uint32_t smu_clock_freq_hz;
    uint32_t syspllvcofreq_10khz;
    uint32_t sysspllrefclk_10khz;
  }atom_smu_outputclkfreq;
} ATOM_GET_SMU_CLOCK_INFO_OUTPUT_PARAMETERS_V3_1;



/*
  ***************************************************************************
              Structures used by dynamicmemorysettings
  ***************************************************************************
*/

typedef enum {
  COMPUTE_MEMORY_PLL_PARAM = 1,
  COMPUTE_ENGINE_PLL_PARAM = 2,
  ADJUST_MC_SETTING_PARAM = 3,
  PATCH_AC_TIMING_PARAM = 3,
} ATOM_DYNAMIC_MEMORY_SETTING_COMMAND;

/* when command = COMPUTE_MEMORY_PLL_PARAM or ADJUST_MC_SETTING_PARAM */
typedef struct dynamic_mclk_settings_parameters_v2_1{
  uint32_t  mclk_10khz:24;         //Input= target mclk
  uint32_t  command:8;             //command enum of atom_dynamic_memory_setting_command, must be COMPUTE_MEMORY_PLL_PARAM
  uint32_t  reserved;
} DYNAMIC_MCLK_SETTINGS_PARAMETERS_V2_1;

/* when command = COMPUTE_ENGINE_PLL_PARAM */
typedef struct _dynamic_sclk_settings_parameters_v2_1 {
  uint32_t  sclk_10khz:24;         //Input= target mclk
  uint32_t  command:8;             //command enum of atom_dynamic_memory_setting_command, must be COMPUTE_ENGINE_PLL_PARAM
  uint32_t  mclk_10khz;
  uint32_t  reserved;
} DYNAMIC_SCLK_SETTINGS_PARAMETERS_V2_1;

typedef struct _dynamic_patch_actiming_parameters_v2_1
{
  uint32_t  mclk_10khz:24;         //Input= target mclk
  uint32_t  command:8;             //command enum of atom_dynamic_memory_setting_command, must be PATCH_AC_TIMING_PARAM
  uint8_t   pstate;                //=0: Pstate0 =1: Pstate1 =2: Pstate2, =3: Pstate3
  uint8_t   reserved[3];
} DYNAMIC_PATCH_ACTIMING_PARAMETERS_V2_1;


typedef union {
  DYNAMIC_MCLK_SETTINGS_PARAMETERS_V2_1 mclk_setting;
  DYNAMIC_SCLK_SETTINGS_PARAMETERS_V2_1 sclk_setting;
  DYNAMIC_PATCH_ACTIMING_PARAMETERS_V2_1 actiming_setting;
} DYNAMIC_MEMORY_SETTINGS_PARAMETERS_V2_1;



/*
  ***************************************************************************
              Structures used by memorytraining
  ***************************************************************************
*/

typedef enum {
  UMC60_UCODE_FUNC_ID_REINIT                 = 0,
  UMC60_UCODE_FUNC_ID_ENTER_SELFREFRESH      = 1,
  UMC60_UCODE_FUNC_ID_EXIT_SELFREFRESH       = 2,
} ATOM_UMC6_0_UCODE_FUNCTION_CALL_ENUM_ID;


typedef struct _memory_training_parameters_v2_1 {
  uint8_t ucode_func_id;
  uint8_t ucode_reserved[3];
  uint32_t reserved[5];
} MEMORY_TRAINING_PARAMETERS_V2_1;


/*
  ***************************************************************************
              Structures used by setpixelclock
  ***************************************************************************
*/

typedef struct _set_pixel_clock_parameter_v1_7 {
    uint32_t pixclk_100hz;               // target the pixel clock to drive the CRTC timing in unit of 100Hz.

    uint8_t  pll_id;                     // ATOM_PHY_PLL0/ATOM_PHY_PLL1/ATOM_PPLL0
    uint8_t  encoderobjid;               // ASIC encoder id defined in objectId.h,
                                         // indicate which graphic encoder will be used.
    uint8_t  encoder_mode;               // Encoder mode:
    uint8_t  miscinfo;                   // enum atom_set_pixel_clock_v1_7_misc_info
    uint8_t  crtc_id;                    // enum of atom_crtc_def
    uint8_t  deep_color_ratio;           // HDMI panel bit depth: enum atom_set_pixel_clock_v1_7_deepcolor_ratio
    uint8_t  reserved1[2];
    uint32_t reserved2;
} SET_PIXEL_CLOCK_PARAMETER_V1_7;

//ucMiscInfo
typedef enum {
  PIXEL_CLOCK_V7_MISC_FORCE_PROG_PPLL         = 0x01,
  PIXEL_CLOCK_V7_MISC_PROG_PHYPLL             = 0x02,
  PIXEL_CLOCK_V7_MISC_YUV420_MODE             = 0x04,
  PIXEL_CLOCK_V7_MISC_DVI_DUALLINK_EN         = 0x08,
  PIXEL_CLOCK_V7_MISC_REF_DIV_SRC             = 0x30,
  PIXEL_CLOCK_V7_MISC_REF_DIV_SRC_XTALIN      = 0x00,
  PIXEL_CLOCK_V7_MISC_REF_DIV_SRC_PCIE        = 0x10,
  PIXEL_CLOCK_V7_MISC_REF_DIV_SRC_GENLK       = 0x20,
  PIXEL_CLOCK_V7_MISC_REF_DIV_SRC_REFPAD      = 0x30,
  PIXEL_CLOCK_V7_MISC_ATOMIC_UPDATE           = 0x40,
  PIXEL_CLOCK_V7_MISC_FORCE_SS_DIS            = 0x80,
} ATOM_SET_PIXEL_CLOCK_V1_7_MISC_INFO;

/* deep_color_ratio */
typedef enum {
  PIXEL_CLOCK_V7_DEEPCOLOR_RATIO_DIS          = 0x00,      //00 - DCCG_DEEP_COLOR_DTO_DISABLE: Disable Deep Color DTO
  PIXEL_CLOCK_V7_DEEPCOLOR_RATIO_5_4          = 0x01,      //01 - DCCG_DEEP_COLOR_DTO_5_4_RATIO: Set Deep Color DTO to 5:4
  PIXEL_CLOCK_V7_DEEPCOLOR_RATIO_3_2          = 0x02,      //02 - DCCG_DEEP_COLOR_DTO_3_2_RATIO: Set Deep Color DTO to 3:2
  PIXEL_CLOCK_V7_DEEPCOLOR_RATIO_2_1          = 0x03,      //03 - DCCG_DEEP_COLOR_DTO_2_1_RATIO: Set Deep Color DTO to 2:1
} ATOM_SET_PIXEL_CLOCK_V1_7_DEEPCOLOR_RATIO;

/*
  ***************************************************************************
              Structures used by setdceclock
  ***************************************************************************
*/

// SetDCEClock input parameter for DCE11.2( ELM and BF ) and above
typedef struct _set_dce_clock_parameters_v2_1 {
  uint32_t dceclk_10khz;                               // target DCE frequency in unit of 10KHZ, return real DISPCLK/DPREFCLK frequency.
  uint8_t  dceclktype;                                 // =0: DISPCLK  =1: DPREFCLK  =2: PIXCLK
  uint8_t  dceclksrc;                                  // ATOM_PLL0 or ATOM_GCK_DFS or ATOM_FCH_CLK or ATOM_COMBOPHY_PLLx
  uint8_t  dceclkflag;                                 // Bit [1:0] = PPLL ref clock source ( when ucDCEClkSrc= ATOM_PPLL0 )
  uint8_t  crtc_id;                                    // ucDisp Pipe Id, ATOM_CRTC0/1/2/..., use only when ucDCEClkType = PIXCLK
} SET_DCE_CLOCK_PARAMETERS_V2_1;

//ucDCEClkType
typedef enum {
  DCE_CLOCK_TYPE_DISPCLK                      = 0,
  DCE_CLOCK_TYPE_DPREFCLK                     = 1,
  DCE_CLOCK_TYPE_PIXELCLK                     = 2,        // used by VBIOS internally, called by SetPixelClock
  DCE_CLOCK_TYPE_DPPCLK                       = 3,
} ATOM_SET_DCE_CLOCK_CLOCK_TYPE;

//ucDCEClkFlag when ucDCEClkType == DPREFCLK
typedef enum {
  DCE_CLOCK_FLAG_PLL_REFCLK_SRC_MASK          = 0x03,
  DCE_CLOCK_FLAG_PLL_REFCLK_SRC_GENERICA      = 0x00,
  DCE_CLOCK_FLAG_PLL_REFCLK_SRC_GENLK         = 0x01,
  DCE_CLOCK_FLAG_PLL_REFCLK_SRC_PCIE          = 0x02,
  DCE_CLOCK_FLAG_PLL_REFCLK_SRC_XTALIN        = 0x03,
} ATOM_SET_DCE_CLOCK_DPREFCLK_FLAG;

//ucDCEClkFlag when ucDCEClkType == PIXCLK
typedef enum {
  DCE_CLOCK_FLAG_PCLK_DEEPCOLOR_RATIO_MASK    = 0x03,
  DCE_CLOCK_FLAG_PCLK_DEEPCOLOR_RATIO_DIS     = 0x00,      //00 - DCCG_DEEP_COLOR_DTO_DISABLE: Disable Deep Color DTO
  DCE_CLOCK_FLAG_PCLK_DEEPCOLOR_RATIO_5_4     = 0x01,      //01 - DCCG_DEEP_COLOR_DTO_5_4_RATIO: Set Deep Color DTO to 5:4
  DCE_CLOCK_FLAG_PCLK_DEEPCOLOR_RATIO_3_2     = 0x02,      //02 - DCCG_DEEP_COLOR_DTO_3_2_RATIO: Set Deep Color DTO to 3:2
  DCE_CLOCK_FLAG_PCLK_DEEPCOLOR_RATIO_2_1     = 0x03,      //03 - DCCG_DEEP_COLOR_DTO_2_1_RATIO: Set Deep Color DTO to 2:1
  DCE_CLOCK_FLAG_PIXCLK_YUV420_MODE           = 0x04,
} ATOM_SET_DCE_CLOCK_PIXCLK_FLAG;

typedef struct _set_dce_clock_ps_allocation_v2_1 {
  SET_DCE_CLOCK_PARAMETERS_V2_1 param;
  uint32_t ulReserved[2];
} SET_DCE_CLOCK_PS_ALLOCATION_V2_1;

typedef struct _set_dce_clock_parameters_v2_2 {
  uint32_t dceclk_10khz;                               // target DCE frequency in unit of 10KHZ, return real DISPCLK/DPREFCLK frequency.
  uint8_t  dceclktype;                                 // =0: DISPCLK  =2: PIXCLK =3 DPPCLK
  uint8_t  dceclksrc;                                  // ATOM_PLL0 or ATOM_GCK_DFS or ATOM_FCH_CLK or ATOM_COMBOPHY_PLLx
  uint8_t  dceclkflag;                                 // Bit [1:0] = PPLL ref clock source ( when ucDCEClkSrc= ATOM_PPLL0 )
  uint8_t  crtc_id;                                    // ucDisp Pipe Id, ATOM_CRTC0/1/2/..., use only when ucDCEClkType = PIXCLK
} SET_DCE_CLOCK_PARAMETERS_V2_2;

typedef struct _set_dce_clock_ps_allocation_v2_2 {
  SET_DCE_CLOCK_PARAMETERS_V2_2 param;
  uint32_t ulReserved[2];
} SET_DCE_CLOCK_PS_ALLOCATION_V2_2;


/****************************************************************************/
// Structures used by BlankCRTC
/****************************************************************************/
typedef struct _blank_crtc_parameters {
  uint8_t  crtc_id;                   // enum atom_crtc_def
  uint8_t  blanking;                  // enum atom_blank_crtc_command
  uint16_t reserved;
  uint32_t reserved1;
} BLANK_CRTC_PARAMETERS;

typedef enum {
  ATOM_BLANKING         = 1,
  ATOM_BLANKING_OFF     = 0,
} ATOM_BLANK_CRTC_COMMAND;

/****************************************************************************/
// Structures used by enablecrtc
/****************************************************************************/
typedef struct _enable_crtc_parameters {
  uint8_t crtc_id;                    // enum atom_crtc_def
  uint8_t enable;                     // ATOM_ENABLE or ATOM_DISABLE
  uint8_t padding[2];
} ENABLE_CRTC_PARAMETERS;


/****************************************************************************/
// Structure used by EnableDispPowerGating
/****************************************************************************/
typedef struct _enable_disp_power_gating_parameters_v2_1 {
  uint8_t disp_pipe_id;                // ATOM_CRTC1, ATOM_CRTC2, ...
  uint8_t enable;                     // ATOM_ENABLE or ATOM_DISABLE
  uint8_t padding[2];
} ENABLE_DISP_POWER_GATING_PARAMETERS_V2_1;

typedef struct _enable_disp_power_gating_ps_allocation {
  ENABLE_DISP_POWER_GATING_PARAMETERS_V2_1 param;
  uint32_t ulReserved[4];
} ENABLE_DISP_POWER_GATING_PS_ALLOCATION;


/****************************************************************************/
// Structure used by dispcontroller_init
/****************************************************************************/

typedef struct _disp_controller_init_parameters_v2_1 {
  uint8_t reserved;
  uint8_t action;                      // = atom_disp_controller_init_action_enum
  uint8_t padding[2];
} DISP_CONTROLLER_INIT_PARAMETERS_V2_1;

typedef struct _disp_controller_init_ps_allocation {
  DISP_CONTROLLER_INIT_PARAMETERS_V2_1 param;
  uint32_t ulReserved[6];
} DISP_CONTROLLER_INIT_PS_ALLOCATION;

//action
typedef enum {
  ATOM_INIT_DCHUB = 2,
  ATOM_INIT_DC = 7,
  ATOM_INIT_DC_PREOS = 8,
} ATOM_DISP_CONTROLLER_INIT_ACTION_ENUM;


/****************************************************************************/
// Structure used in setcrtc_usingdtdtiming
/****************************************************************************/
typedef struct _set_crtc_using_dtd_timing_parameters {
  uint16_t  h_size;
  uint16_t  h_blanking_time;
  uint16_t  v_size;
  uint16_t  v_blanking_time;
  uint16_t  h_syncoffset;
  uint16_t  h_syncwidth;
  uint16_t  v_syncoffset;
  uint16_t  v_syncwidth;
  uint16_t  modemiscinfo;
  uint8_t   h_border;
  uint8_t   v_border;
  uint8_t   crtc_id;                   // enum atom_crtc_def
  uint8_t   encoder_mode;         // atom_encode_mode_def
  uint8_t   padding[2];
} SET_CRTC_USING_DTD_TIMING_PARAMETERS;


/****************************************************************************/
// Structures used by processi2cchanneltransaction
/****************************************************************************/
typedef struct _process_i2c_channel_transaction_parameters {
  uint8_t i2cspeed_khz;
  union {
    uint8_t regindex;
    uint8_t status;                  /* enum atom_process_i2c_flag */
  } regind_status;
  uint16_t  i2c_data_out;
  uint8_t   flag;                    /* enum atom_process_i2c_status */
  uint8_t   trans_bytes;
  uint8_t   slave_addr;
  uint8_t   i2c_id;
} PROCESS_I2C_CHANNEL_TRANSACTION_PARAMETERS;

//ucFlag
typedef enum {
  HW_I2C_WRITE          = 1,
  HW_I2C_READ           = 0,
  I2C_2BYTE_ADDR        = 0x02,
  HW_I2C_SMBUS_BYTE_WR  = 0x04,
} ATOM_PROCESS_I2C_FLAG;
//IPCLEAN_END
//status
typedef enum {
  HW_ASSISTED_I2C_STATUS_FAILURE     =2,
  HW_ASSISTED_I2C_STATUS_SUCCESS     =1,
} ATOM_PROCESS_I2C_STATUS;
//IPCLEAN_START

/****************************************************************************/
// Structures used by processauxchanneltransaction
/****************************************************************************/

typedef struct _process_aux_channel_transaction_parameters_v1_2 {
  uint16_t aux_request;
  uint16_t dataout;
  uint8_t  channelid;
  union {
    uint8_t   reply_status;
    uint8_t   aux_delay;
  } aux_status_delay;
  uint8_t   dataout_len;
  uint8_t   hpd_id;                                       //=0: HPD1, =1: HPD2, =2: HPD3, =3: HPD4, =4: HPD5, =5: HPD6
} PROCESS_AUX_CHANNEL_TRANSACTION_PARAMETERS_V1_2;


/****************************************************************************/
// Structures used by selectcrtc_source
/****************************************************************************/

typedef struct _select_crtc_source_parameters_v2_3 {
  uint8_t crtc_id;                        // enum atom_crtc_def
  uint8_t encoder_id;                     // enum atom_dig_def
  uint8_t encode_mode;                    // enum atom_encode_mode_def
  uint8_t dst_bpc;                        // enum atom_panel_bit_per_color
} SELECT_CRTC_SOURCE_PARAMETERS_V2_3;


/****************************************************************************/
// Structures used by digxencodercontrol
/****************************************************************************/

// ucAction:
typedef enum {
  ATOM_ENCODER_CMD_DISABLE_DIG                  = 0,
  ATOM_ENCODER_CMD_ENABLE_DIG                   = 1,
  ATOM_ENCODER_CMD_DP_LINK_TRAINING_START       = 0x08,
  ATOM_ENCODER_CMD_DP_LINK_TRAINING_PATTERN1    = 0x09,
  ATOM_ENCODER_CMD_DP_LINK_TRAINING_PATTERN2    = 0x0a,
  ATOM_ENCODER_CMD_DP_LINK_TRAINING_PATTERN3    = 0x13,
  ATOM_ENCODER_CMD_DP_LINK_TRAINING_COMPLETE    = 0x0b,
  ATOM_ENCODER_CMD_DP_VIDEO_OFF                 = 0x0c,
  ATOM_ENCODER_CMD_DP_VIDEO_ON                  = 0x0d,
  ATOM_ENCODER_CMD_SETUP_OBFUSCATION            = 0x10,
  ATOM_ENCODER_CMD_DP_LINK_TRAINING_PATTERN4    = 0x14,
  ATOM_ENCODER_CMD_STREAM_SETUP                 = 0x0F,
  ATOM_ENCODER_CMD_LINK_SETUP                   = 0x11,
  ATOM_ENCODER_CMD_ENCODER_BLANK                = 0x12,
} ATOM_DIG_ENCODER_CONTROL_ACTION;

//define ucObfuscateMode
typedef enum {
  DP_OBFUSCATION_DISABLE                        = 0x00,
  DP_OBFUSCATION_ENABLE_eDP_MODE                = 0x01,
  DP_OBFUSCATION_ENABLE_LVLINK_MODE             = 0x11,
} ATOM_DIG_ENCODER_CONTROL_OBFMODE;

//ucDigId
typedef enum {
  ATOM_ENCODER_CONFIG_V5_DIG0_ENCODER           = 0x00,
  ATOM_ENCODER_CONFIG_V5_DIG1_ENCODER           = 0x01,
  ATOM_ENCODER_CONFIG_V5_DIG2_ENCODER           = 0x02,
  ATOM_ENCODER_CONFIG_V5_DIG3_ENCODER           = 0x03,
  ATOM_ENCODER_CONFIG_V5_DIG4_ENCODER           = 0x04,
  ATOM_ENCODER_CONFIG_V5_DIG5_ENCODER           = 0x05,
  ATOM_ENCODER_CONFIG_V5_DIG6_ENCODER           = 0x06,
  ATOM_ENCODER_CONFIG_V5_DIG7_ENCODER           = 0x07,
} ATOM_DIG_ENCODER_CONTROL_V5_DIGID;

typedef struct _dig_encoder_stream_setup_parameters_v1_5 {
  uint8_t digid;            // 0~6 map to DIG0~DIG6 enum atom_dig_encoder_control_v5_digid
  uint8_t action;           // =  ATOM_ENOCODER_CMD_STREAM_SETUP
  uint8_t digmode;          // ATOM_ENCODER_MODE_DP/ATOM_ENCODER_MODE_DVI/ATOM_ENCODER_MODE_HDMI
  uint8_t lanenum;          // Lane number
  uint32_t pclk_10khz;      // Pixel Clock in 10Khz
  uint8_t bitpercolor;
  uint8_t dplinkrate_270mhz;//= DP link rate/270Mhz, =6: 1.62G  = 10: 2.7G, =20: 5.4Ghz, =30: 8.1Ghz etc
  uint8_t reserved[2];
} DIG_ENCODER_STREAM_SETUP_PARAMETERS_V1_5;

typedef struct _dig_encoder_link_setup_parameters_v1_5 {
  uint8_t digid;           // 0~6 map to DIG0~DIG6 enum atom_dig_encoder_control_v5_digid
  uint8_t action;          // =  ATOM_ENOCODER_CMD_LINK_SETUP
  uint8_t digmode;         // ATOM_ENCODER_MODE_DP/ATOM_ENCODER_MODE_DVI/ATOM_ENCODER_MODE_HDMI
  uint8_t lanenum;         // Lane number
  uint8_t symclk_10khz;    // Symbol Clock in 10Khz
  uint8_t hpd_sel;
  uint8_t digfe_sel;       // DIG stream( front-end ) selection, bit0 means DIG0 FE is enable,
  uint8_t reserved[2];
} DIG_ENCODER_LINK_SETUP_PARAMETERS_V1_5;

typedef struct _dp_obfuscation_set_parameters_v1_5
{
  uint8_t digid;              // 0~6 map to DIG0~DIG6 enum atom_dig_encoder_control_v5_digid
  uint8_t action;             // = ATOM_ENCODER_CMD_DPLINK_SETUP
  uint8_t obfuscatemode;      // enum atom_dig_encoder_control_obfmode
  uint8_t reserved1;
  uint32_t reserved2[2];
} DP_OBFUSCATION_SET_PARAMETERS_V1_5;

typedef struct _dig_encoder_generic_cmd_parameters_v1_5 {
  uint8_t digid;           // 0~6 map to DIG0~DIG6 enum atom_dig_encoder_control_v5_digid
  uint8_t action;          // = rest of generic encoder command which does not carry any parameters
  uint8_t reserved1[2];
  uint32_t reserved2[2];
} DIG_ENCODER_GENERIC_CMD_PARAMETERS_V1_5;

typedef union {
  DIG_ENCODER_GENERIC_CMD_PARAMETERS_V1_5  cmd_param;
  DIG_ENCODER_STREAM_SETUP_PARAMETERS_V1_5 stream_param;
  DIG_ENCODER_LINK_SETUP_PARAMETERS_V1_5   link_param;
  DP_OBFUSCATION_SET_PARAMETERS_V1_5 dpObf_param;
} DIG_ENCODER_CONTROL_PARAMETERS_V1_5;

/*
  ***************************************************************************
              Structures used by dig1transmittercontrol
  ***************************************************************************
*/
typedef struct _dig_transmitter_control_parameters_v1_6 {
  uint8_t phyid;           // 0=UNIPHYA, 1=UNIPHYB, 2=UNIPHYC, 3=UNIPHYD, 4= UNIPHYE 5=UNIPHYF
  uint8_t action;          // define as ATOM_TRANSMITER_ACTION_xxx
  union {
    uint8_t digmode;        // enum atom_encode_mode_def
    uint8_t dplaneset;      // DP voltage swing and pre-emphasis value defined in DPCD DP_LANE_SET, "DP_LANE_SET__xDB_y_zV"
  } mode_laneset;
  uint8_t  lanenum;        // Lane number 1, 2, 4, 8
  uint32_t symclk_10khz;   // Symbol Clock in 10Khz
  uint8_t  hpdsel;         // =1: HPD1, =2: HPD2, .... =6: HPD6, =0: HPD is not assigned
  uint8_t  digfe_sel;      // DIG stream( front-end ) selection, bit0 means DIG0 FE is enable,
  uint8_t  connobj_id;     // Connector Object Id defined in ObjectId.h
  uint8_t  reserved;
  uint32_t reserved1;
} DIG_TRANSMITTER_CONTROL_PARAMETERS_V1_6;

typedef struct DIG_TRANSMITTER_CONTROL_PS_ALLOCATION_V1_6
{
  DIG_TRANSMITTER_CONTROL_PARAMETERS_V1_6 param;
  uint32_t reserved[4];
} TYPE_DIG_TRANSMITTER_CONTROL_PS_ALLOCATION_V1_6;

//ucAction
typedef enum {
  ATOM_TRANSMITTER_ACTION_DISABLE                 = 0,
  ATOM_TRANSMITTER_ACTION_ENABLE                  = 1,
  ATOM_TRANSMITTER_ACTION_LCD_BLOFF               = 2,
  ATOM_TRANSMITTER_ACTION_LCD_BLON                = 3,
  ATOM_TRANSMITTER_ACTION_BL_BRIGHTNESS_CONTROL   = 4,
  ATOM_TRANSMITTER_ACTION_LCD_SELFTEST_START      = 5,
  ATOM_TRANSMITTER_ACTION_LCD_SELFTEST_STOP       = 6,
  ATOM_TRANSMITTER_ACTION_INIT                    = 7,
  ATOM_TRANSMITTER_ACTION_DISABLE_OUTPUT          = 8,
  ATOM_TRANSMITTER_ACTION_ENABLE_OUTPUT           = 9,
  ATOM_TRANSMITTER_ACTION_SETUP                   = 10,
  ATOM_TRANSMITTER_ACTION_SETUP_VSEMPH            = 11,
  ATOM_TRANSMITTER_ACTION_POWER_ON                = 12,
  ATOM_TRANSMITTER_ACTION_POWER_OFF               = 13,
} ATOM_DIG_TRANSMITTER_CONTROL_ACTION;

// digfe_sel
typedef enum {
  ATOM_TRANMSITTER_V6__DIGA_SEL                   = 0x01,
  ATOM_TRANMSITTER_V6__DIGB_SEL                   = 0x02,
  ATOM_TRANMSITTER_V6__DIGC_SEL                   = 0x04,
  ATOM_TRANMSITTER_V6__DIGD_SEL                   = 0x08,
  ATOM_TRANMSITTER_V6__DIGE_SEL                   = 0x10,
  ATOM_TRANMSITTER_V6__DIGF_SEL                   = 0x20,
  ATOM_TRANMSITTER_V6__DIGG_SEL                   = 0x40,
} ATOM_DIG_TRANSMITTER_CONTROL_DIGFE_SEL;


//ucHPDSel
typedef enum {
  ATOM_TRANSMITTER_V6_NO_HPD_SEL                  = 0x00,
  ATOM_TRANSMITTER_V6_HPD1_SEL                    = 0x01,
  ATOM_TRANSMITTER_V6_HPD2_SEL                    = 0x02,
  ATOM_TRANSMITTER_V6_HPD3_SEL                    = 0x03,
  ATOM_TRANSMITTER_V6_HPD4_SEL                    = 0x04,
  ATOM_TRANSMITTER_V6_HPD5_SEL                    = 0x05,
  ATOM_TRANSMITTER_V6_HPD6_SEL                    = 0x06,
} ATOM_DIG_TRANSMITTER_CONTROL_HPD_SEL;

// ucDPLaneSet
typedef enum {
  DP_LANE_SET__0DB_0_4V                           = 0x00,
  DP_LANE_SET__0DB_0_6V                           = 0x01,
  DP_LANE_SET__0DB_0_8V                           = 0x02,
  DP_LANE_SET__0DB_1_2V                           = 0x03,
  DP_LANE_SET__3_5DB_0_4V                         = 0x08,
  DP_LANE_SET__3_5DB_0_6V                         = 0x09,
  DP_LANE_SET__3_5DB_0_8V                         = 0x0a,
  DP_LANE_SET__6DB_0_4V                           = 0x10,
  DP_LANE_SET__6DB_0_6V                           = 0x11,
  DP_LANE_SET__9_5DB_0_4V                         = 0x18,

  DP_LANE_SET__PE0_SW0                            = 0x00,
  DP_LANE_SET__PE0_SW1                            = 0x01,
  DP_LANE_SET__PE0_SW2                            = 0x02,
  DP_LANE_SET__PE0_SW3                            = 0x03,
  DP_LANE_SET__PE1_SW0                            = 0x08,
  DP_LANE_SET__PE1_SW1                            = 0x09,
  DP_LANE_SET__PE1_SW2                            = 0x0a,
  DP_LANE_SET__PE2_SW0                            = 0x10,
  DP_LANE_SET__PE2_SW1                            = 0x11,
  DP_LANE_SET__PE3_SW0                            = 0x18,
} ATOM_DIG_TRANSMITTER_CONTROL_DPLANESET;



/****************************************************************************/
// Structures used by ExternalEncoderControl V2.4
/****************************************************************************/

typedef struct _external_encoder_control_parameters_v2_4 {
  uint16_t pixelclock_10khz;  // pixel clock in 10Khz, valid when ucAction=SETUP/ENABLE_OUTPUT
  uint8_t  config;            // indicate which encoder, and DP link rate when ucAction = SETUP/ENABLE_OUTPUT
  uint8_t  action;            //
  uint8_t  encodermode;       // encoder mode, only used when ucAction = SETUP/ENABLE_OUTPUT
  uint8_t  lanenum;           // lane number, only used when ucAction = SETUP/ENABLE_OUTPUT
  uint8_t  bitpercolor;       // output bit per color, only valid when ucAction = SETUP/ENABLE_OUTPUT and ucEncodeMode= DP
  uint8_t  hpd_id;
} EXTERNAL_ENCODER_CONTROL_PARAMETERS_V2_4;


// ucAction
typedef enum {
  EXTERNAL_ENCODER_ACTION_V3_DISABLE_OUTPUT           = 0x00,
  EXTERNAL_ENCODER_ACTION_V3_ENABLE_OUTPUT            = 0x01,
  EXTERNAL_ENCODER_ACTION_V3_ENCODER_INIT             = 0x07,
  EXTERNAL_ENCODER_ACTION_V3_ENCODER_SETUP            = 0x0f,
  EXTERNAL_ENCODER_ACTION_V3_ENCODER_BLANKING_OFF     = 0x10,
  EXTERNAL_ENCODER_ACTION_V3_ENCODER_BLANKING         = 0x11,
  EXTERNAL_ENCODER_ACTION_V3_DACLOAD_DETECTION        = 0x12,
  EXTERNAL_ENCODER_ACTION_V3_DDC_SETUP                = 0x14,
} EXTERNAL_ENCODER_CONTROL_ACTION_DEF;

// ucConfig
typedef enum {
  EXTERNAL_ENCODER_CONFIG_V3_DPLINKRATE_MASK          = 0x03,
  EXTERNAL_ENCODER_CONFIG_V3_DPLINKRATE_1_62GHZ       = 0x00,
  EXTERNAL_ENCODER_CONFIG_V3_DPLINKRATE_2_70GHZ       = 0x01,
  EXTERNAL_ENCODER_CONFIG_V3_DPLINKRATE_5_40GHZ       = 0x02,
  EXTERNAL_ENCODER_CONFIG_V3_DPLINKRATE_3_24GHZ       = 0x03,
  EXTERNAL_ENCODER_CONFIG_V3_ENCODER_SEL_MAKS         = 0x70,
  EXTERNAL_ENCODER_CONFIG_V3_ENCODER1                 = 0x00,
  EXTERNAL_ENCODER_CONFIG_V3_ENCODER2                 = 0x10,
  EXTERNAL_ENCODER_CONFIG_V3_ENCODER3                 = 0x20,
} EXTERNAL_ENCODER_CONTROL_V2_4_CONFIG_DEF;

typedef struct _external_encoder_control_ps_allocation_v2_4 {
  EXTERNAL_ENCODER_CONTROL_PARAMETERS_V2_4 sExtEncoder;
  uint32_t reserved[2];
} EXTERNAL_ENCODER_CONTROL_PS_ALLOCATION_V2_4;


/****************************************************************************/
// Structures used by gfx_init
/****************************************************************************/

typedef struct _gfx_init_parameters_v2_1 {
  uint32_t reserved[5];
} GFX_INIT_PARAMETERS_V2_1;

/*
  ***************************************************************************
                           AMD ACPI Table

  ***************************************************************************
*/

typedef struct _amd_acpi_description_header {
  uint32_t signature;
  uint32_t tableLength;      //Length
  uint8_t  revision;
  uint8_t  checksum;
  uint8_t  oemId[6];
  uint8_t  oemTableId[8];    //UINT64  OemTableId;
  uint32_t oemRevision;
  uint32_t creatorId;
  uint32_t creatorRevision;
} AMD_ACPI_DESCRIPTION_HEADER;

typedef struct _uefi_acpi_vfct {
  AMD_ACPI_DESCRIPTION_HEADER sheader;
  uint8_t  tableUUID[16];    //0x24
  uint32_t vbiosimageoffset; //0x34. Offset to the first GOP_VBIOS_CONTENT block from the beginning of the stucture.
  uint32_t lib1Imageoffset;  //0x38. Offset to the first GOP_LIB1_CONTENT block from the beginning of the stucture.
  uint32_t reserved[4];      //0x3C
} UEFI_ACPI_VFCT;

typedef struct _vfct_image_header {
  uint32_t  pcibus;          //0x4C
  uint32_t  pcidevice;       //0x50
  uint32_t  pcifunction;     //0x54
  uint16_t  vendorid;        //0x58
  uint16_t  deviceid;        //0x5A
  uint16_t  ssvid;           //0x5C
  uint16_t  ssid;            //0x5E
  uint32_t  revision;        //0x60
  uint32_t  imagelength;     //0x64
} VFCT_IMAGE_HEADER;


typedef struct _gop_vbios_content {
  VFCT_IMAGE_HEADER vbiosheader;
  uint8_t                  vbioscontent[1];
} GOP_VBIOS_CONTENT;

typedef struct _gop_lib1_content {
  VFCT_IMAGE_HEADER lib1header;
  uint8_t                  lib1content[1];
} GOP_LIB1_CONTENT;



/*
  ***************************************************************************
                   Scratch Register definitions
  Each number below indicates which scratch regiser request, Active and
  Connect all share the same definitions as display_device_tag defines
  ***************************************************************************
*/

typedef enum {
  ATOM_DEVICE_CONNECT_INFO_DEF      = 0,
  ATOM_BL_BRI_LEVEL_INFO_DEF        = 2,
  ATOM_ACTIVE_INFO_DEF              = 3,
  ATOM_LCD_INFO_DEF                 = 4,
  ATOM_MEM_INFO_DEF                 = 4,
  ATOM_DEVICE_REQ_INFO_DEF          = 5,
  ATOM_ACC_CHANGE_INFO_DEF          = 6,
  ATOM_PRE_OS_MODE_INFO_DEF         = 7,
  ATOM_PRE_OS_ASSERTION_DEF         = 8,    //For GOP to record a 32bit assertion code, this is enabled by default in prodution GOP drivers.
  ATOM_INTERNAL_TIMER_INFO_DEF      = 10,
} SCRATCH_REGISTER_DEF;

typedef enum {
  ATOM_DISPLAY_LCD1_CONNECT           =0x0002,
  ATOM_DISPLAY_DFP1_CONNECT           =0x0008,
  ATOM_DISPLAY_DFP2_CONNECT           =0x0080,
  ATOM_DISPLAY_DFP3_CONNECT           =0x0200,
  ATOM_DISPLAY_DFP4_CONNECT           =0x0400,
  ATOM_DISPLAY_DFP5_CONNECT           =0x0800,
  ATOM_DISPLAY_DFP6_CONNECT           =0x0040,
  ATOM_DISPLAY_DFPx_CONNECT           =0x0ec8,
  ATOM_CONNECT_INFO_DEVICE_MASK       =0x0fff,
} SCRATCH_DEVICE_CONNECT_INFO_BIT_DEF;

typedef enum {
  ATOM_CURRENT_BL_LEVEL_SHIFT         =0x8,
#ifndef _H2INC
  ATOM_CURRENT_BL_LEVEL_MASK          =0x0000ff00,
  ATOM_DEVICE_DPMS_STATE              =0x00010000,
#endif
} SCRATCH_BL_BRI_LEVEL_INFO_BIT_DEF;

typedef enum {
  ATOM_DISPLAY_LCD1_ACTIVE            =0x0002,
  ATOM_DISPLAY_DFP1_ACTIVE            =0x0008,
  ATOM_DISPLAY_DFP2_ACTIVE            =0x0080,
  ATOM_DISPLAY_DFP3_ACTIVE            =0x0200,
  ATOM_DISPLAY_DFP4_ACTIVE            =0x0400,
  ATOM_DISPLAY_DFP5_ACTIVE            =0x0800,
  ATOM_DISPLAY_DFP6_ACTIVE            =0x0040,
  ATOM_ACTIVE_INFO_DEVICE_MASK        =0x0fff,
} SCRATCH_ACTIVE_INFO_BITS_DEF;

typedef enum {

#ifndef _H2INC
  ATOM_MEM_VENDER_ID_MASK             =0xffff,
#endif

  b2ATOM_MEM_MODULE_ID_MASK           =0xff,
} SCRATCH_MEM_INFO_DEF;

typedef enum {
  ATOM_DISPLAY_LCD1_REQ               =0x0002,
  ATOM_DISPLAY_DFP1_REQ               =0x0008,
  ATOM_DISPLAY_DFP2_REQ               =0x0080,
  ATOM_DISPLAY_DFP3_REQ               =0x0200,
  ATOM_DISPLAY_DFP4_REQ               =0x0400,
  ATOM_DISPLAY_DFP5_REQ               =0x0800,
  ATOM_DISPLAY_DFP6_REQ               =0x0040,
  ATOM_REQ_INFO_DEVICE_MASK           =0x0fff,
} SCRATCH_DEVICE_REQ_INFO_BITS_DEF;

typedef enum {
  ATOM_ACC_CHANGE_ACC_MODE_SHIFT    =4,
  ATOM_ACC_CHANGE_LID_STATUS_SHIFT  =6,
} SCRATCH_ACC_CHANGE_INFO_BITSHIFT_DEF;

typedef enum {
  ATOM_ACC_CHANGE_ACC_MODE          =0x00000010,
  ATOM_ACC_CHANGE_LID_STATUS        =0x00000040,
} SCRATCH_ACC_CHANGE_INFO_BITS_DEF;

typedef enum {
  ATOM_PRE_OS_MODE_MASK             =0x00000003,
  ATOM_PRE_OS_MODE_VGA              =0x00000000,
  ATOM_PRE_OS_MODE_VESA             =0x00000001,
  ATOM_PRE_OS_MODE_GOP              =0x00000002,
  ATOM_PRE_OS_MODE_PIXEL_DEPTH      =0x0000000C,
  ATOM_PRE_OS_MODE_PIXEL_FORMAT_MASK=0x000000F0,
  ATOM_PRE_OS_MODE_8BIT_PAL_EN      =0x00000100,
  ATOM_ASIC_INIT_COMPLETE           =0x00000200,
#ifndef PSP_VBIOS_H2INC
#ifndef _H2INC
  ATOM_PRE_OS_MODE_NUMBER_MASK      =0xFFFF0000,
#endif
#endif
} SCRATCH_PRE_OS_MODE_INFO_BITS_DEF;



/*
  ***************************************************************************
                       ATOM firmware ID header file
              !! Please keep it at end of the atomfirmware.h !!
  ***************************************************************************
*/
#include "atomfirmwareid.h"
//IPCLEAN_END
#pragma pack()

#define GNB_SBDFO MAKE_SBDFO(0, 0, 0, 0, 0)

/// Define configuration values for ulGPUCapInfo
// BIT[0] - TMDS/HDMI Coherent Mode 0: use cascade PLL mode, 1: use single PLL mode.
#define GPUCAPINFO_TMDS_HDMI_USE_CASCADE_PLL_MODE      0x00ul
#define GPUCAPINFO_TMDS_HDMI_USE_SINGLE_PLL_MODE       0x01ul

// BIT[1] - DP mode 0: use cascade PLL mode, 1: use single PLL mode
#define GPUCAPINFO_DP_MODE_USE_CASCADE_PLL_MODE        0x00ul
#define GPUCAPINFO_DP_USE_SINGLE_PLL_MODE              0x02ul

// BIT[3] - AUX HW mode detection logic 0: Enable, 1: Disable
#define GPUCAPINFO_AUX_HW_MODE_DETECTION_ENABLE        0x00ul
#define GPUCAPINFO_AUX_HW_MODE_DETECTION_DISABLE       0x08ul

// BIT[4] - DFS bypass 0: Disable, 1: Enable
#define GPUCAPINFO_DFS_BYPASS_DISABLE       0x00ul
#define GPUCAPINFO_DFS_BYPASS_ENABLE        0x10ul
#define GPUCAPSINFO_EXT_HDMI_INIT_PER_PORT  0x20ul

// BIT[16]=1 indicate SMC firmware is able to support GNB fast resume function
#define GPUCAPS__GNB_FAST_RESUME_CAPABLE    0x10000ul

// BIT[17] - Enable battery boost feature 0: Disable, 1: Enable
#define GPUCAPINFO_BATTERY_BOOST_ENABLE       0x20000ul
#endif

