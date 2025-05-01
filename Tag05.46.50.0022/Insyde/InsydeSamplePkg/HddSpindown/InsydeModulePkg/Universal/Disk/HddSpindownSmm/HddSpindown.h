/** @file
  Header file for HddSpindown Driver..

//;******************************************************************************
//;* Copyright (c) 2014, Insyde Software Corporation. All Rights Reserved.
//;*
//;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
//;* transmit, broadcast, present, recite, release, license or otherwise exploit
//;* any part of this publication in any form, by any means, without the prior
//;* written permission of Insyde Software Corporation.
//;*
//;******************************************************************************
*/

#ifndef _HDD_SPINDOWN_H
#define _HDD_SPINDOWN_H

#include <PortNumberMap.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/AtaPassThru.h>
#include <Protocol/HddSpindown.h>

#ifndef BIT0
#define  BIT0  1
#define  BIT1  (1 << 1)
#define  BIT2  (1 << 2)
#define  BIT3  (1 << 3)
#define  BIT4  (1 << 4)
#define  BIT5  (1 << 5)
#define  BIT6  (1 << 6)
#define  BIT7  (1 << 7)
#define  BIT8  (1 << 8)
#define  BIT9  (1 << 9)
#define  BIT10 (1 << 10)
#define  BIT11 (1 << 11)
#define  BIT12 (1 << 12)
#define  BIT13 (1 << 13)
#define  BIT14 (1 << 14)
#define  BIT15 (1 << 15)
#define  BIT16 (1 << 16)
#define  BIT17 (1 << 17)
#define  BIT18 (1 << 18)
#define  BIT19 (1 << 19)
#define  BIT20 (1 << 20)
#define  BIT21 (1 << 21)
#define  BIT22 (1 << 22)
#define  BIT23 (1 << 23)
#define  BIT24 (1 << 24)
#define  BIT25 (1 << 25)
#define  BIT26 (1 << 26)
#define  BIT27 (1 << 27)
#define  BIT28 (1 << 28)
#define  BIT29 (1 << 29)
#define  BIT30 (1 << 30)
#define  BIT31 (1 << 31)
#endif

///
/// PCH SATA controller default setting
///
#define PCI_BUS_NUMBER_PCH_SATA             0
#define PCI_DEVICE_NUMBER_PCH_SATA          0x1F
#define PCI_FUNCTION_NUMBER_PCH_SATA        0x02
#define PCI_SATA_COMMAND                    0x04
#define  PCI_SATA_COMMAND_BME               BIT2
#define  PCI_SATA_COMMAND_MSE               BIT1
#define  PCI_SATA_COMMAND_IOSE              BIT0
#define PCI_SATA_SUB_CLASS_CODE             0x0A
#define  PCI_SATA_SUB_CLASS_CODE_AHCI       0x06
#define PCI_SATA_BASE_CLASS_CODE            0x0B
#define  PCI_CLASS_MASS_STORAGE             0x01
#define PCI_SATA_AHCI_BAR                   0x24
#define  PCI_SATA_AHCI_BAR_LENGTH           0x800
#define  PCI_SATA_AHCI_BAR_ALIGNMENT        11
#define  PCI_AHCI_HBA_GHC                   0x04
#define   PCI_SATA_AHCI_GHC_AE              BIT31
#define PCI_SATA_MAP                        0x90
#define  PCI_SATA_MAP_SMS_AHCI              0x40
#define  PCI_SATA_PORT_TO_CONTROLLER_CFG    BIT5
#define PCI_SATA_PCS                        0x92
#define  PCI_SATA_PCS_PORT0_EN              BIT0

#define PCH_AHCI_MAX_PORTS                  0x06

#define STANDBY_IMMEDIATE_CMD               0xE0

//
// Timeout setting in micorsecond unit
//
#define HBA_NONDATA_DEVICE_ACK_TIMEOUT      10000
#define HBA_NONDATA_CMD_ISSUED_TIMEOUT      500
#define HBA_RFIS_START_TIME_OUT             500
#define HBA_RFIS_STOP_TIME_OUT              500
#define HBA_COMMAND_STOP_TIME_OUT           500

#define HBA_IS                              0x8
#define FIS_TYPE_MASK                       0xFF
#define MAX_PORT_NUMBER                     0x20
#define MAX_COMMAND_SLOT_NUMBER             0x20
#define MAX_SUPPORT_PRDT_NUMBER             0x400
#define RECEIVE_FIS_SIZE                    0x100
#define COMMAND_LIST_SIZE                   0x20
#define MAX_RECEIVE_FIS_SIZE                (MAX_PORT_NUMBER * RECEIVE_FIS_SIZE)
#define MAX_COMMAND_LIST_SIZE               (MAX_COMMAND_SLOT_NUMBER * COMMAND_LIST_SIZE)
#define COMMAND_TABLE_SIZE                  0x80
#define PRDT_SIZE                           0x10
#define SUPPORT_PRDT_NUMBER                 0x20
#define CFIS_TABLE_SIZE                     0x40
#define SUPPORT_PRDT_NUMBER                 0x20
#define SUPPORT_COMMAND_TABLE_NUMBER        2
#define MAX_COMMAND_TABLE_SIZE              ((SUPPORT_COMMAND_TABLE_NUMBER) * (COMMAND_TABLE_SIZE + PRDT_SIZE * MAX_SUPPORT_PRDT_NUMBER))
//
// Port register
//
#define HBA_PORTS_START                     0x0100
#define HBA_PORTS_REG_WIDTH                 0x0080
#define HBA_PORTS_CLB                       0x0
#define HBA_PORTS_CLBU                      0x4
#define HBA_PORTS_FB                        0x8
#define HBA_PORTS_FBU                       0xC
#define HBA_PORTS_IS                        0x10
#define HBA_PORTS_CMD                       0x18
#define HBA_PORTS_CMD_ST                    BIT0
#define HBA_PORTS_CMD_FRE                   BIT4
#define HBA_PORTS_CMD_FR                    BIT14
#define HBA_PORTS_CMD_CR                    BIT15
#define HBA_PORTS_CMD_ATAPI                 BIT24
#define HBA_PORTS_CMD_DLAE                  BIT25
#define HBA_PORTS_SERR                      0x30
#define HBA_PORTS_SACT                      0x34
#define HBA_PORTS_CI                        0x38
#define D2H_FIS_OFFSET                      0x40
#define FIS_REGISTER_D2H                    0x34

#define AHCI_FIS_RUNNING(a) \
 ((a) & HBA_PORTS_CMD_FR) == HBA_PORTS_CMD_FR

#define AHCI_COMMAND_RUNNING(a) \
 ((a) & (HBA_PORTS_CMD_ST | HBA_PORTS_CMD_CR)) != 0
//
// HBA Generic Register
//
#define HBA_PORT_REG_BASE(Port) \
  (UINTN) (Port * HBA_PORTS_REG_WIDTH + HBA_PORTS_START)

#define HBA_PORT_MEM_ADDR(BaseAddr, Port, MemAddr) \
  (UINT32*)((UINTN) (BaseAddr) + (UINTN) (HBA_PORT_REG_BASE ((Port))) + (UINTN) (MemAddr))

#define MmAddress( BaseAddr, Register ) \
  ((UINTN) (BaseAddr) + \
   (UINTN) (Register) \
  )

#define Mm32Ptr( BaseAddr, Register ) \
  ((volatile UINT32*)MmAddress (BaseAddr, Register ))

#define HBA_REG32( BaseAddr, Register ) \
  (*Mm32Ptr ((BaseAddr), (Register)))

#define HBA_CLEAR_STS(BaseAddr, Sts) \
  HBA_REG32 ((BaseAddr), (Sts)) = HBA_REG32 ((BaseAddr), (Sts))

#define HBA_PORT_REG32(BaseAddr, Port, Register) \
  (HBA_REG32 ((BaseAddr), ((Register) + HBA_PORT_REG_BASE (Port))))

#define HBA_PORT_CLEAR_STS(BaseAddr, Port, Sts) \
  HBA_PORT_REG32 ((BaseAddr), (Port), (Sts)) = HBA_PORT_REG32 ((BaseAddr), (Port), (Sts))

#define HBA_REG32_AND( BaseAddr, Register, AndData) \
  (HBA_REG32 ((BaseAddr), (Register))) = (HBA_REG32 ((BaseAddr), (Register))) & ((UINT32) (AndData))

#define HBA_REG32_OR( BaseAddr, Register, OrData) \
  (HBA_REG32 ((BaseAddr), (Register))) = (HBA_REG32 ((BaseAddr), (Register))) | ((UINT32) (OrData))

#define HBA_PORT_REG32_OR(BaseAddr, Port, Register, OrData) \
  (HBA_REG32_OR ((BaseAddr), ((Register) + HBA_PORT_REG_BASE (Port)), (OrData)))

#define HBA_PORT_REG32_AND(BaseAddr, Port, Register, AndData) \
  (HBA_REG32_AND ((BaseAddr), ((Register) + HBA_PORT_REG_BASE (Port)), (AndData)))

#define HBA_REG32_AND_OR( BaseAddr, Register,AndData,  OrData) \
   (HBA_REG32 ((BaseAddr), (Register)) = \
     (((HBA_REG32 ((BaseAddr), (Register))) & ((UINT32) (AndData))) | ((UINT32) (OrData))))

#define HBA_PORT_REG32_AND_OR(BaseAddr, Port, Register, AndData, OrData) \
  (HBA_REG32_AND_OR ((BaseAddr), ((Register) + HBA_PORT_REG_BASE (Port)), (AndData), (OrData)))

typedef struct {
  UINT8                         Ahci_Dsfis[0x1C];
  //
  // Dma Setup Fis
  //
  UINT8                         Ahci_Dsfis_Rsvd[0x04];
  UINT8                         Ahci_Psfis[0x14];
  //
  // Pio Setip Fis
  //
  UINT8                         Ahci_Psfis_Rsvd[0x0C];
  UINT8                         Ahci_Rfis[0x14];
  //
  // D2H Register Fis
  //
  UINT8                         Ahci_Rfis_Rsvd[0x04];
  //
  // Set Device Bits Fix
  //
  UINT64                        Ahci_Sdbfis;
  UINT8                         Ahci_Ufis[0x40];
  //
  // Unkonw FIS
  //
  UINT8                         Ahci_Ufis_Rsvd[0x60];
} AHCI_RECEIVED_FIS;


typedef struct {
  UINT8                         Ahci_CFis_Type;
  UINT8                         AHci_CFis_PmNum:4;
  UINT8                         Ahci_CFis_Rsvd1:1;
  UINT8                         Ahci_CFis_Rsvd2:1;
  UINT8                         Ahci_CFis_Rsvd3:1;
  UINT8                         Ahci_CFis_CmdInd:1;
  UINT8                         Ahci_CFis_Cmd;
  UINT8                         Ahci_CFis_Feature;
  UINT8                         Ahci_CFis_SecNum;
  UINT8                         Ahci_CFis_ClyLow;
  UINT8                         Ahci_CFis_ClyHigh;
  UINT8                         Ahci_CFis_DevHead;
  UINT8                         Ahci_CFis_SecNumExp;
  UINT8                         Ahci_CFis_ClyLowExp;
  UINT8                         Ahci_CFis_ClyHighExp;
  UINT8                         Ahci_CFis_FeatureExp;
  UINT8                         Ahci_CFis_SecCount;
  UINT8                         Ahci_CFis_SecCountExp;
  UINT8                         Ahci_CFis_Rsvd4;
  UINT8                         Ahci_CFis_Control;
  UINT8                         Ahci_CFis_Rsvd5[4];
  UINT8                         Ahci_CFis_Rsvd6[44];
} AHCI_COMMAND_FIS;

typedef struct {
  UINT32                        Ahci_Cmd_Cfl:5;
  UINT32                        Ahci_Cmd_A:1;
  UINT32                        Ahci_Cmd_W:1;
  UINT32                        Ahci_Cmd_P:1;
  UINT32                        Ahci_Cmd_R:1;
  UINT32                        Ahci_Cmd_B:1;
  UINT32                        Ahci_Cmd_C:1;
  UINT32                        Ahci_Cmd_Rsvd1:1;
  UINT32                        Ahci_Cmd_Pmp:4;
  UINT32                        Ahci_Cmd_Prdtl:16;
  UINT32                        Ahci_Cmd_Prdbc;
  UINT32                        Ahci_Cmd_Ctba;
  UINT32                        Ahci_Cmd_Ctbau;
  UINT32                        Ahci_Cmd_Rsvd2[4];
} AHCI_COMMAND_LIST;

typedef struct {
  UINT32                        Ahci_Prdt_Dba;
  UINT32                        Ahci_Prdt_Dbau;
  UINT32                        Ahci_Prdt_Rsvd;
  UINT32                        Ahci_Prdt_Dbc:22;
  UINT32                        Ahci_Prdt_Rsvd1:9;
  UINT32                        Ahci_Prdt_Ioc:1;
} AHCI_COMMAND_PRDT;

typedef struct {
  UINT8                         Ahci_Atapi_Command[0x10];
} AHCI_ATAPI_COMMAND;

typedef struct {
  AHCI_COMMAND_FIS              CFis;
  AHCI_ATAPI_COMMAND            AtapiCmd;
  UINT8                         Rsvd[0x30];
  AHCI_COMMAND_PRDT             PrdtTable[MAX_SUPPORT_PRDT_NUMBER];
} AHCI_COMMAND_TABLE;

#define EFI_HDD_SPINDOWN_SIGNATURE     SIGNATURE_32 ('H', 'D', 'D', 'S')

#define HDD_SPINDOWN_FROM_HDDSPINDOWN(a) CR (a, HDD_SPINDOWN_PRIVATE_DATA, HddSpindown, EFI_HDD_SPINDOWN_SIGNATURE)

typedef enum {
  IdeType = 0,
  AhciType,
  Unknown
} HDD_SPINDOWN_CONTROLLER_TYPE;

typedef struct _HDD_SPINDOWN_PRIVATE_DATA {
  UINT32                     Signature;
  UINT8                      ControllerMode;
  HDD_SPINDOWN_PROTOCOL      HddSpindown;
} HDD_SPINDOWN_PRIVATE_DATA;

#ifndef PLATFORM_PCIEX_BASE_ADDRESS
#define PCI_EXPRESS_BASE_ADDRESS    0xE0000000
#else
#ifndef PLATFORM_PCIEX_BASE_ADDRESS_64MB
#define PCI_EXPRESS_BASE_ADDRESS    0xF0000000
#else
#define PCI_EXPRESS_BASE_ADDRESS    0xF0000000
#endif
#endif

#ifndef MmPciAddress
#define MmPciAddress(Segment, Bus, Device, Function, Register) \
    ( \
      (UINTN) PCI_EXPRESS_BASE_ADDRESS + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) (Function << 12) + \
        (UINTN) (Register) \
    )
#endif

//
// UINT8
//
#define MmPci8Ptr(Segment, Bus, Device, Function, Register) \
    ((volatile UINT8 *) MmPciAddress (Segment, Bus, Device, Function, Register))

#define MmPci8(Segment, Bus, Device, Function, Register) *MmPci8Ptr (Segment, Bus, Device, Function, Register)

//
// UINT16
//
#define MmPci16Ptr(Segment, Bus, Device, Function, Register) \
    ((volatile UINT16 *) MmPciAddress (Segment, Bus, Device, Function, Register))

#define MmPci16(Segment, Bus, Device, Function, Register) *MmPci16Ptr (Segment, Bus, Device, Function, Register)
#define MmPci16Or(Segment, Bus, Device, Function, Register, OrData) \
  MmPci16 (Segment, Bus, Device, Function, Register) = \
    (UINT16) (MmPci16 (Segment, Bus, Device, Function, Register) | (UINT16) (OrData))

//
// UINT32
//
#define MmPci32Ptr(Segment, Bus, Device, Function, Register) \
    ((volatile UINT32 *) MmPciAddress (Segment, Bus, Device, Function, Register))

#define MmPci32(Segment, Bus, Device, Function, Register) *MmPci32Ptr (Segment, Bus, Device, Function, Register)

#define MmPci32Or(Segment, Bus, Device, Function, Register, OrData) \
  MmPci32 (Segment, Bus, Device, Function, Register) = \
    (UINT32) (MmPci32 (Segment, Bus, Device, Function, Register) | (UINT32) (OrData))

#endif
