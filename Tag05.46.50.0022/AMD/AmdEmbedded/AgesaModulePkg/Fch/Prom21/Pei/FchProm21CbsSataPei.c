/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <Library/DebugLib.h>

#include <Filecode.h>

#include <Ppi/FchProm21InitPpi.h>
#include <Library/FchProm21Common.h>
#include <Library/FchProm21BaseLib.h>
#include "FchProm21CbsPei.h"

#define FILECODE FCH_PROM21_PEI_FCHPROM21CBSSATAPEI_FILECODE

VOID
FchSecProm21SetSataMode (
  IN        UINT32 XhciMmio
)
{
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));

  if(PcdGet8(PcdSecPT21SataMode) != 0xf) {
    if(PcdGet8(PcdSecPT21SataMode) == PTSataAhci) {
      FchProm21SetSataClassCode (XhciMmio, 0x6, PT_SATA_AHCI_DID);
    } else {
      FchProm21SetSataClassCode (XhciMmio, 0x4, PT_SATA_RAID_DID);
    }
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}

VOID
FchProm21SetSataMode (
  IN        UINT32 XhciMmio
)
{
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));

  if(PcdGet8(PcdPT21SataMode) != 0xf) {
    if(PcdGet8(PcdPT21SataMode) == PTSataAhci) {
      FchProm21SetSataClassCode (XhciMmio, 0x6, PT_SATA_AHCI_DID);
    } else {
      FchProm21SetSataClassCode (XhciMmio, 0x4, PT_SATA_RAID_DID);
    }
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
}

VOID
FchProm21SetSataClassCode (
  IN        UINT32 XhciMmio,
  IN        UINT8  SubClass,
  IN        UINT16 DeviceId
)
{
  UINT32 Address;
  UINT32 Value32;
  UINT8 Value8;

  // Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  //Set class code:
  // Bit[0:7]   SATA Programming Interface
  // Bit[8:15]  SATA Sub Class Code
  // Bit[16:23] SATA Base Class Code
  Address = PROM21_SATA_CLASS_CODE_REG;
  Value32 = FchProm21XhciReadDWord(XhciMmio, Address);
  Value32 &= (UINT32)(~0xFF00);
  Value32 |= (UINT32)(SubClass << 8);
  FchProm21XhciWriteDWord(XhciMmio, Address, Value32);

  //Set Device ID
  Address = PROM21_DEVICE_ID_ENABLE_REG;
  FchProm21XhciWriteByte(XhciMmio, Address, 0x2); //Bit1 = 1 to enable write
  Address = PROM21_SATA_DEVICE_ID_REG;
  FchProm21XhciWriteWord (XhciMmio, Address, DeviceId);

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);
}

// Promontory SATA Controller Enable
VOID
FchProm21SataControllerEnable (
  IN       UINT32 XhciMmio
  )
{
  UINT8    Data8;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  Data8 = FchProm21XhciReadByte (XhciMmio, 0x10151);

  if( (Data8 & 0x02) == 0 ) {
    Data8 |= 0x02;
    FchProm21XhciWriteByte (XhciMmio, 0x10151, Data8);
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return;
}

// Promontory SATA Controller Disable
VOID
FchProm21SataControllerDisable (
  IN       UINT32 XhciMmio
  )
{
  UINT8    Data8;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  Data8 = FchProm21XhciReadByte (XhciMmio, 0x10151);

  if ((Data8 & 0x02) != 0) {
    Data8 &= (~0x02);
    FchProm21XhciWriteByte (XhciMmio, 0x10151, Data8);
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return;
}

// Promontory SATA Port Control
VOID
FchProm21SataPortCtrl (
  IN       UINT32 XhciMmio,
  IN       BOOLEAN Enable,
  IN       UINT8  PortNum
  )
{
  UINT32 Address;
  UINT8  Value8;

  if(PortNum > PROM21_MAX_SATA_PORT_NUMBER){
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));

  // Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Enable/Disable SATA port
  Address = PROM21_SATA_PORT_ENABLE_REG;
  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  switch(Enable)
  {
    case 1:
      Value8 |= (1 << PortNum);
      break;
    case 0:
      Value8 &= (~ (1 << PortNum));
      break;
    default:
      break;
  }
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Set PI register corresponding bit
  Address = PROM21_AHCI_PI_REG;
  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  switch(Enable)
  {
    case 1:
      Value8 |= (1 << PortNum);
      break;
    case 0:
      Value8 &= (~ (1 << PortNum));
      break;
    default:
      Value8 |= (1 << PortNum);
      break;
  }
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return;
}

// Promontory SATA Port Enable
VOID
FchProm21SataPortEnable (
  IN       UINT32 XhciMmio,
  IN       UINT8  PortNum
  )
{
  UINT32 Address;
  UINT8  Value8;

  if(PortNum > PROM21_MAX_SATA_PORT_NUMBER){
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));

  // Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Enable SATA port
  Address = PROM21_SATA_PORT_ENABLE_REG;
  Value8 = FchProm21XhciReadByte(XhciMmio, Address);
  Value8 |= (1 << PortNum);
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Set PI register corresponding bit to 1
  Address = PROM21_AHCI_PI_REG;
  Value8 = FchProm21XhciReadByte(XhciMmio, Address);
  Value8 |= ( (1 << PortNum) );
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return;
}

// Promontory SATA Port Disable
VOID
FchProm21SataPortDisable (
  IN      UINT32    XhciMmio,
  IN      UINT8     PortNum
  )
{
  UINT32  Address;
  UINT8   Value8;

  if (PortNum > PROM21_MAX_SATA_PORT_NUMBER) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start\n", __FUNCTION__));

  // Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Disable SATA port
  Address = PROM21_SATA_PORT_ENABLE_REG;
  Value8 = FchProm21XhciReadByte(XhciMmio, Address);
  Value8 &= (~ (1 << PortNum));
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  // Set PI register corresponding bit to 0
  Address = PROM21_AHCI_PI_REG;
  Value8 = FchProm21XhciReadByte(XhciMmio, Address);
  Value8 &= (~ (1 << PortNum));
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End\n", __FUNCTION__));
  return;
}

VOID AmdPt21GppClockOutput  (
IN      UINT32    XhciMmio,
IN      UINT8     ClkReqNum,
IN      UINT8     ReqMode
)
{
  UINT32 Address;
  UINT16 Data16;
  UINT8  Value8;

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  if((ClkReqNum > PROM21_MAX_CLKREQ_NUMBER) || (ReqMode > PROM21_MAX_CLKREQ_MODE)) {
    return;
  }

  //Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  //
  Address = PROM21_CLKREQ_MODE_REG;
  Data16 = FchProm21XhciReadWord (XhciMmio, Address);

  DEBUG ((EFI_D_INFO, "Read Data16 = 0x%x @ Address 0x%x\n", Data16, Address));

  switch(ClkReqNum)
  {
    case 0:
      Data16 &= ~(BIT0 + BIT1);
      Data16 |= ReqMode;
      break;
    case 1:
      Data16 &= ~(BIT2 + BIT3);
      Data16 |= (ReqMode << 2);
      break;
    case 2:
      Data16 &= ~(BIT4 + BIT5);
      Data16 |= (ReqMode << 4);
      break;
    case 3:
      Data16 &= ~(BIT6 + BIT7);
      Data16 |= (ReqMode << 6);
      break;
    case 4:
      Data16 &= ~(BIT8 + BIT9);
      Data16 |= (ReqMode << 8);
      break;
    case 5:
      Data16 &= ~(BIT10 + BIT11);
      Data16 |= (ReqMode << 10);
      break;
    default:
      break;
  }

  FchProm21XhciWriteWord(XhciMmio, Address, Data16);
  DEBUG ((EFI_D_INFO, "Write Data16 = 0x%x @ Address 0x%x\n", Data16, Address));

  Data16 = FchProm21XhciReadWord (XhciMmio, Address);

  DEBUG ((EFI_D_INFO, "Readback Data16 = 0x%x @ Address 0x%x\n", Data16, Address));

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}

// GPP Clock Configuration
VOID AmdSecPt21GppClockConfig (
  IN      UINT32              XhciMmio
)
{
  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  if(PcdGet8 (PcdSecPT21PcieClkreq0Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 0, PcdGet8(PcdSecPT21PcieClkreq0Mode));
  }

  if(PcdGet8 (PcdSecPT21PcieClkreq1Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 1, PcdGet8(PcdSecPT21PcieClkreq1Mode));
  }

  if(PcdGet8 (PcdSecPT21PcieClkreq2Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 2, PcdGet8(PcdSecPT21PcieClkreq2Mode));
  }

  if(PcdGet8 (PcdSecPT21PcieClkreq3Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 3, PcdGet8(PcdSecPT21PcieClkreq3Mode));
  }

  if(PcdGet8 (PcdSecPT21PcieClkreq4Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 4, PcdGet8(PcdSecPT21PcieClkreq4Mode));
  }

  if(PcdGet8 (PcdSecPT21PcieClkreq5Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 5, PcdGet8(PcdSecPT21PcieClkreq5Mode));
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}

VOID AmdPt21GppClockConfig (
  IN      UINT32              XhciMmio
)
{
  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  if(PcdGet8 (PcdPT21PcieClkreq0Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 0, PcdGet8(PcdPT21PcieClkreq0Mode));
  }

  if(PcdGet8 (PcdPT21PcieClkreq1Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 1, PcdGet8(PcdPT21PcieClkreq1Mode));
  }

  if(PcdGet8 (PcdPT21PcieClkreq2Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 2, PcdGet8(PcdPT21PcieClkreq2Mode));
  }

  if(PcdGet8 (PcdPT21PcieClkreq3Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 3, PcdGet8(PcdPT21PcieClkreq3Mode));
  }

  if(PcdGet8 (PcdPT21PcieClkreq4Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 4, PcdGet8(PcdPT21PcieClkreq4Mode));
  }

  if(PcdGet8 (PcdPT21PcieClkreq5Mode) != 0xf) {
    AmdPt21GppClockOutput(XhciMmio, 5, PcdGet8(PcdPT21PcieClkreq5Mode));
  }

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}

// Downstream Port CLKREQ Pin select
VOID
AmdPt21ClkReqPinSelect (
  IN      UINT32    XhciMmio,
  IN      UINT8     ClkReqNum,
  IN      UINT8     PortNumSel
  )
{
  UINT32       Address;
  UINT8        Value8;
 // CLKREQ_PORT  ClkReqPort;
  UINT32       Data32;

  Data32 = 0;

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-Start\n", __FUNCTION__));

  if((ClkReqNum > PROM21_MAX_CLKREQ_NUMBER) || (PortNumSel > PROM21_MAX_PORT_NUMBER)) {
    return;
  }

  //Enable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x86;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  //
  Address = PROM21_CLKREQ_PORT_REG;
  Data32 = FchProm21XhciReadDWord (XhciMmio, Address);

  DEBUG ((EFI_D_INFO, "Read Data32 = 0x%x @ Address 0x%x\n", Data32, Address));

  //Set CLKREQ port
  switch(ClkReqNum)
  {
    case 0:
      Data32 &= 0xFFFFFFF0;
      Data32 |= PortNumSel;
      break;
    case 1:
      Data32 &= 0xFFFFFF0F;
      Data32 |= (PortNumSel << 4);
      break;
    case 2:
      Data32 &= 0xFFFFF0FF;
      Data32 |= (PortNumSel << 8);
      break;
    case 3:
      Data32 &= 0xFFFF0FFF;
      Data32 |= (PortNumSel << 12);
      break;
    case 4:
      Data32 &= 0xFFF0FFFF;
      Data32 |= (PortNumSel << 16);
      break;
    case 5:
      Data32 &= 0xFF0FFFFF;
      Data32 |= (PortNumSel << 20);
      break;
    default:
      break;
  }

  //
  FchProm21XhciWriteDWord (XhciMmio, Address, Data32);

  DEBUG ((EFI_D_INFO, "Write Data32 = 0x%x @ Address 0x%x\n", Data32, Address));

  Data32 = FchProm21XhciReadDWord (XhciMmio, Address);

  DEBUG ((EFI_D_INFO, "Readback Data32 = 0x%x @ Address 0x%x\n", Data32, Address));

  // Disable Attribute
  Address = PROM21_CTRL_WR_ENABLE_REG;
  Value8 = 0x0;
  FchProm21XhciWriteByte (XhciMmio, Address, Value8);

  DEBUG ((DEBUG_INFO, "PT-PEI-%a-End\n", __FUNCTION__));
}

VOID
AmdSecPt21ClkConfig (
  IN     UINT32    XhciMmio
)
{
  if(PcdGet8(PcdSecPT21PcieClkreq0PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 0, PcdGet8(PcdSecPT21PcieClkreq0PinSelect));
  }

  if(PcdGet8(PcdSecPT21PcieClkreq1PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 1, PcdGet8(PcdSecPT21PcieClkreq1PinSelect));
  }

  if(PcdGet8(PcdSecPT21PcieClkreq2PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 2, PcdGet8(PcdSecPT21PcieClkreq2PinSelect));
  }

  if(PcdGet8(PcdSecPT21PcieClkreq3PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 3, PcdGet8(PcdSecPT21PcieClkreq3PinSelect));
  }

  if(PcdGet8(PcdSecPT21PcieClkreq4PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 4, PcdGet8(PcdSecPT21PcieClkreq4PinSelect));
  }

  if(PcdGet8(PcdSecPT21PcieClkreq5PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 5, PcdGet8(PcdSecPT21PcieClkreq5PinSelect));
  }

}

VOID
AmdPt21ClkConfig (
  IN     UINT32    XhciMmio
)
{
  if(PcdGet8(PcdPT21PcieClkreq0PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 0, PcdGet8(PcdPT21PcieClkreq0PinSelect));
  }

  if(PcdGet8(PcdPT21PcieClkreq1PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 1, PcdGet8(PcdPT21PcieClkreq1PinSelect));
  }

  if(PcdGet8(PcdPT21PcieClkreq2PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 2, PcdGet8(PcdPT21PcieClkreq2PinSelect));
  }

  if(PcdGet8(PcdPT21PcieClkreq3PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 3, PcdGet8(PcdPT21PcieClkreq3PinSelect));
  }

  if(PcdGet8(PcdPT21PcieClkreq4PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 4, PcdGet8(PcdPT21PcieClkreq4PinSelect));
  }

  if(PcdGet8(PcdPT21PcieClkreq5PinSelect) != 0xf) {
    AmdPt21ClkReqPinSelect(XhciMmio, 5, PcdGet8(PcdPT21PcieClkreq5PinSelect));
  }

}

VOID
FchProm21SetAhciDevslp (
  IN      UINT32    XhciMmio,
  IN      BOOLEAN   Enable,
  IN      UINT8     PortNum
  )
{
  UINT32  Address;
  UINT8   Value8;

  if (PortNum > PROM21_MAX_SATA_PORT_NUMBER) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- PortNum- %d -Enable(%d)\n", __FUNCTION__, PortNum, Enable));

  Address = PROM21_AHCI_DEVSLP_REG;

  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  DEBUG ((EFI_D_INFO, "Read Value8 = 0x%x, Address = 0x%x\n", Value8, Address));

  switch (Enable) {
    case 1:
      Value8 |= (1 << PortNum);
      break;
    case 0:
      Value8 &= (~ (1 << PortNum));
      break;
    default:
      Value8 &= (~ (1 << PortNum));
      break;
  }

  DEBUG ((EFI_D_INFO, "Write Value8 = 0x%x, Address = 0x%x\n", Value8, Address));

  FchProm21XhciWriteByte (XhciMmio, Address, Value8);
}

VOID
FchProm21SetAhciCapReg (
  IN       UINT32 XhciMmio,
  IN       UINT32 Capreg,
  IN       BOOLEAN Enable
  )
{
  UINT32 Address;
  UINT32 Value32;

  Address = PROM21_AHCI_CAP_REG;

  Value32 = FchProm21XhciReadDWord (XhciMmio, Address);
  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- Reg- 0x%x -Enable(%d)\n", __FUNCTION__, Capreg, Enable));

  DEBUG ((EFI_D_INFO, "Read Value32 = 0x%x, Address = 0x%x\n", Value32, Address));

  switch(Enable)
  {
    case 1:
      Value32 |= Capreg;
      break;
    case 0:
      Value32 &= (~Capreg);
      break;
    default:
      Value32 |= Capreg;
      break;
  }

  DEBUG ((EFI_D_INFO, "Write Value32 = 0x%x, Address = 0x%x\n", Value32, Address));

  FchProm21XhciWriteDWord(XhciMmio, Address, Value32);
}

VOID
FchProm21AhciPortSataSpeed (
  IN       UINT32 XhciMmio,
  IN       UINT8  PortNum,
  IN       UINT8  Speed
  )
{
  UINT32 Address;
  UINT8  Value8;

  if((PortNum > PROM21_MAX_SATA_PORT_NUMBER) ||
     (Speed < PROM21_MIN_SATA_SPEED) ||
     (Speed > PROM21_MAX_SATA_SPEED)){
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- 0x%x (%d)\n", __FUNCTION__, Speed, PortNum));

  switch(PortNum)
  {
    case 0:
      Address = PROM21_SATA_P0_GEN_REG;
      break;
    case 1:
      Address = PROM21_SATA_P1_GEN_REG;
      break;
    case 2:
      Address = PROM21_SATA_P2_GEN_REG;
      break;
    case 3:
      Address = PROM21_SATA_P3_GEN_REG;
      break;
    default:
      Address = PROM21_SATA_P0_GEN_REG;
      break;
  }

  switch(Speed)
  {
    case 1:
      Value8 = 0x90;
      break;
    case 2:
      Value8 = 0xA0;
      break;
    case 3:
      Value8 = 0xC0;
      break;
  }

  DEBUG ((EFI_D_INFO, "Write Value8 = 0x%x, Address = 0x%x\n", Value8, Address));

  FchProm21XhciWriteByte(XhciMmio, Address, Value8);
}

VOID
FchProm21AhciPortHotplug (
  IN       UINT32 XhciMmio,
  IN       UINT8  PortNum,
  IN       BOOLEAN Enable
  )
{
  UINT32 Address;
  UINT8  Value8;

  if(PortNum > PROM21_MAX_SATA_PORT_NUMBER){
    return;
  }

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start- %d (%d)\n", __FUNCTION__, PortNum, Enable));

  Address = PROM21_AHCI_PCMD_CAP_REG + PortNum;

  Value8 = FchProm21XhciReadByte(XhciMmio, Address);

  switch(Enable)
  {
    case 1:
      Value8 |= BIT3;
      break;
    case 0:
      Value8 &= (~BIT3);
      break;
    default:
      Value8 |= BIT3;
      break;
  }

  DEBUG ((EFI_D_INFO, "Write Value8 = 0x%x, Address = 0x%x\n", Value8, Address));

  FchProm21XhciWriteByte(XhciMmio, Address, Value8);
}

// Promontory SATA CBS setting
VOID
FchProm21SataCbsPei (
  IN      UINT32    XhciMmio
  )
{
  UINT8      i;
  UINT8      SataPort = 0xFF;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  //++++++++++++++++++++++++++++++++++ SATA CBS start
  FchProm21SetSataMode (XhciMmio);

  if (PcdGet8 (PcdPT21SataPort0Enable) != 0xf) {
    if (PcdGet8 (PcdPT21SataPort0Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x0);
      DEBUG ((EFI_D_ERROR, "PcdPT21SataPort0Enable = 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x0);
      SataPort &= (UINT8)( ~(1 << 0) );
    }
  }

  if (PcdGet8 (PcdPT21SataPort1Enable) != 0xf) {
    if (PcdGet8 (PcdPT21SataPort1Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x1);
      DEBUG ((EFI_D_ERROR, "PcdPT21SataPort1Enable = 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x1);
      SataPort &= (UINT8)( ~(1 << 1) );
    }
  }

  if (PcdGet8 (PcdPT21SataPort2Enable) != 0xf) {
    if (PcdGet8 (PcdPT21SataPort2Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x2);
      DEBUG ((EFI_D_ERROR, "PcdPT21SataPort2Enable = 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x2);
      SataPort &= (UINT8)( ~(1 << 2) );
    }
  }

  if (PcdGet8 (PcdPT21SataPort3Enable) != 0xf) {
    if (PcdGet8 (PcdPT21SataPort3Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x3);
      DEBUG ((EFI_D_ERROR, "PcdPT21SataPort3Enable = 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x3);
      SataPort &= (UINT8)( ~(1 << 3) );
    }
  }

  if (PcdGet8 (PcdPT21SataAggressiveDevSlpP0) != 0xf) {
    if (PcdGet8 (PcdPT21SataAggressiveDevSlpP0) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x0);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x0);
    }
  }

  if (PcdGet8 (PcdPT21SataAggressiveDevSlpP1) != 0xf) {
    if (PcdGet8 (PcdPT21SataAggressiveDevSlpP1) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x1);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x1);
    }
  }

  if (PcdGet8 (PcdPT21SataAggressiveDevSlpP2) != 0xf) {
    if (PcdGet8 (PcdPT21SataAggressiveDevSlpP2) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x2);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x2);
    }
  }

  if (PcdGet8 (PcdPT21SataAggressiveDevSlpP3) != 0xf) {
    if (PcdGet8 (PcdPT21SataAggressiveDevSlpP3) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x3);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x3);
    }
  }

  if (PcdGet8 (PcdPT21SataAggrLinkPmCap) != 0xf) {
    if (PcdGet8 (PcdPT21SataAggrLinkPmCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SALP_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SALP_REG, 0);
    }
  }

  if (PcdGet8 (PcdPT21SataPscCap) != 0xf) {
    if (PcdGet8 (PcdPT21SataPscCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_PSC_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_PSC_REG, 0);
    }
  }

  if (PcdGet8 (PcdPT21SataPTSataCCCSCap) != 0xf) {
    if (PcdGet8 (PcdPT21SataPTSataCCCSCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_CCCS_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_CCCS_REG, 0);
    }
  }

  if (PcdGet8 (PcdPT21SataSscCap) != 0xf) {
    if (PcdGet8 (PcdPT21SataSscCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SSC_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SSC_REG, 0);
    }
  }

  if (PcdGet8 (PcdPT21SataPortMdPort0) != 0xf) {
    if (PcdGet8 (PcdPT21SataPortMdPort0) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 1);
    } else if (PcdGet8 (PcdPT21SataPortMdPort0) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 2);
    } else if (PcdGet8 (PcdPT21SataPortMdPort0) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 1);
    }
  }

  if (PcdGet8 (PcdPT21SataPortMdPort1) != 0xf) {
    if (PcdGet8 (PcdPT21SataPortMdPort1) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 1);
    } else if (PcdGet8 (PcdPT21SataPortMdPort1) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 2);
    } else if (PcdGet8 (PcdPT21SataPortMdPort1) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 1);
    }
  }

  if (PcdGet8 (PcdPT21SataPortMdPort2) != 0xf) {
    if (PcdGet8 (PcdPT21SataPortMdPort2) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 1);
    } else if (PcdGet8 (PcdPT21SataPortMdPort2) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 2);
    } else if (PcdGet8 (PcdPT21SataPortMdPort2) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 1);
    }
  }

  if (PcdGet8 (PcdPT21SataPortMdPort3) != 0xf) {
    if (PcdGet8 (PcdPT21SataPortMdPort3) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 1);
    } else if (PcdGet8 (PcdPT21SataPortMdPort3) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 2);
    } else if (PcdGet8 (PcdPT21SataPortMdPort3) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 1);
    }
  }

  if (PcdGet8 (PcdPT21SataHotPlug) != 0xf) {
    if (PcdGet8 (PcdPT21SataHotPlug) == 1) {
      for (i = 0; i <= PROM21_MAX_SATA_PORT_NUMBER; i++) {
        FchProm21AhciPortHotplug(XhciMmio, i, 1);
      }
    } else {
      for (i = 0; i <= PROM21_MAX_SATA_PORT_NUMBER; i++) {
        FchProm21AhciPortHotplug(XhciMmio, i, 0);
       }
    }
  }

   // Force GPP Clock Setting
  DEBUG ((DEBUG_INFO, "  Force GPP Clock Output\n"));
  AmdPt21GppClockConfig (XhciMmio);

  // CLKREQ Mode Select
  DEBUG ((DEBUG_INFO, "  Select CLKREQ!\n"));
  AmdPt21ClkConfig (XhciMmio);
  //-------------------------------------------- SATA CBS end

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End-return \n", __FUNCTION__));
}
VOID
FchSecProm21SataCbsPei (
  IN      UINT32    XhciMmio
  )
{
  UINT8      i;
  UINT8      SataPort = 0xFF;

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-Start-0x%08X\n", __FUNCTION__, XhciMmio));

  //++++++++++++++++++++++++++++++++++ SATA CBS start
  FchSecProm21SetSataMode (XhciMmio);

  if (PcdGet8 (PcdSecPT21SataPort0Enable) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPort0Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x0);
      DEBUG ((EFI_D_ERROR, "PcdSecPT21SataPort0Enable= 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x0);
      SataPort &= (UINT8)( ~(1 << 0) );
    }
  }

  if (PcdGet8 (PcdSecPT21SataPort1Enable) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPort1Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x1);
      DEBUG ((EFI_D_ERROR, "PcdSecPT21SataPort1Enable= 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x1);
      SataPort &= (UINT8)( ~(1 << 1) );
    }
  }

  if (PcdGet8 (PcdSecPT21SataPort2Enable) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPort2Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x2);
      DEBUG ((EFI_D_ERROR, "PcdSecPT21SataPort2Enable= 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x2);
      SataPort &= (UINT8)( ~(1 << 2) );
    }
  }

  if (PcdGet8 (PcdSecPT21SataPort3Enable) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPort3Enable) == 1) {
      FchProm21SataPortEnable (XhciMmio, 0x3);
      DEBUG ((EFI_D_ERROR, "PcdSecPT21SataPort3Enable= 1\n"));
    } else {
      FchProm21SataPortDisable (XhciMmio, 0x3);
      SataPort &= (UINT8)( ~(1 << 3) );
    }
  }

  if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP0) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP0) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x0);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x0);
    }
  }

  if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP1) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP1) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x1);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x1);
    }
  }

  if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP2) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP2) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x2);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x2);
    }
  }

  if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP3) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataAggressiveDevSlpP3) == 1) {
      FchProm21SetAhciDevslp(XhciMmio, 1, 0x3);
    } else {
      FchProm21SetAhciDevslp(XhciMmio, 0, 0x3);
    }
  }

  if (PcdGet8 (PcdSecPT21SataAggrLinkPmCap) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataAggrLinkPmCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SALP_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SALP_REG, 0);
    }
  }

  if (PcdGet8 (PcdSecPT21SataPscCap) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPscCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_PSC_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_PSC_REG, 0);
    }
  }

  if (PcdGet8 (PcdSecPT21SataPTSataCCCSCap) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPTSataCCCSCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_CCCS_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_CCCS_REG, 0);
    }
  }

  if (PcdGet8 (PcdSecPT21SataSscCap) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataSscCap) == 1) {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SSC_REG, 1);
    } else {
      FchProm21SetAhciCapReg(XhciMmio, PROM21_AHCI_CAP_SSC_REG, 0);
    }
  }

  if (PcdGet8 (PcdSecPT21SataPortMdPort0) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPortMdPort0) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 1);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort0) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 2);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort0) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 0, 1);
    }
  }

  if (PcdGet8 (PcdSecPT21SataPortMdPort1) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPortMdPort1) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 1);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort1) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 2);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort1) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 1, 1);
    }
  }

  if (PcdGet8 (PcdSecPT21SataPortMdPort2) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPortMdPort2) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 1);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort2) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 2);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort2) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 2, 1);
    }
  }

  if (PcdGet8 (PcdSecPT21SataPortMdPort3) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataPortMdPort3) == 1) {
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 1);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort3) == 2){
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 2);
    } else if (PcdGet8 (PcdSecPT21SataPortMdPort3) == 3){
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 3);
    } else {
      FchProm21AhciPortSataSpeed (XhciMmio, 3, 1);
    }
  }

  if (PcdGet8 (PcdSecPT21SataHotPlug) != 0xf) {
    if (PcdGet8 (PcdSecPT21SataHotPlug) == 1) {
      for (i = 0; i <= PROM21_MAX_SATA_PORT_NUMBER; i++) {
        FchProm21AhciPortHotplug(XhciMmio, i, 1);
      }
    } else {
      for (i = 0; i <= PROM21_MAX_SATA_PORT_NUMBER; i++) {
        FchProm21AhciPortHotplug(XhciMmio, i, 0);
       }
    }
  }

  // Force GPP Clock Setting
  DEBUG ((DEBUG_INFO, "  Force GPP Clock Output\n"));
  AmdSecPt21GppClockConfig (XhciMmio);

  // CLKREQ Mode Select
  DEBUG ((DEBUG_INFO, "  Select CLKREQ!\n"));
  AmdSecPt21ClkConfig (XhciMmio);

  //-------------------------------------------- SATA CBS end

  DEBUG ((DEBUG_INFO, "PT-CBSPEI-%a-End-return\n", __FUNCTION__));
}