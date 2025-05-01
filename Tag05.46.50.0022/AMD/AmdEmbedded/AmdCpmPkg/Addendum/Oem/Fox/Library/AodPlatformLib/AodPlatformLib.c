/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */
/*****************************************************************************
 *
 * This software package can be used to enable the Overclocking of certain
 * AMD processors and its use is subject to the terms and conditions of the
 * AMD Overclocking Waiver. Enabling overclocking through use of the low-level
 * routines included in this package and operating an AMD processor outside of
 * the applicable AMD product specifications will void any AMD warranty and can
 * result in damage to the processor or the system into which the processor has
 * been integrated. The user of this software assumes, and AMD disclaims, all
 * risk, liability, costs and damages relating to or arising from the overclocking
 * of AMD processors.
 *
 ******************************************************************************
 */
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <AmdCpmDxe.h>
#include <Features/AODv2/Include/Library/AodPlatformLib.h>
#ifndef CFG_CPM_ECRAM_BASE_ADDR
#define CFG_CPM_ECRAM_BASE_ADDR   0x662
#endif

#pragma pack (push, 1)
typedef struct{
  UINT8   IdxEcRam;
  UINT8   AndEcRamData;
  UINT8   OrEcRamData;
  UINT8   Rev[1];
} struEcTbl;
#pragma pack (pop)


UINT8
InnerEcIdxRead (
  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr,
  UINT8               u8Ofst
  )
{
  EFI_STATUS    Status;
  UINT8         u8Val = 0xFF;

  Status = CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &u8Ofst, &u8Val);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "[%d]: Fail to Read CPM EC function - %r\n", __LINE__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return u8Val;
}

EFI_STATUS
ImmEcIdxWrite (
  AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr,
  UINT8               u8Ofst,
  UINT8               u8Val
  )
{
  EFI_STATUS    Status;

  Status = CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &u8Ofst, &u8Val);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "[%d]: Fail to Write CPM EC function - %r\n", __LINE__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;

}


EFI_STATUS  EcI2cDevByteAccess (
    AMD_CPM_TABLE_PROTOCOL   *CpmTableProtocolPtr,
    BOOLEAN             boolRead,
    UINT8               I2c7BitSlaveAddr,
    UINT8               SlaveIdx,
    UINT8               *SlaveData
)
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UINT8                 Offset;
  UINT8                 Value8;
  UINTN                 Index8;

  do
  {
    //
    // EcName.asl define the EC Access Rule
    //
//    Offset(0x90), // PMIC/VR
//    PMC0,8,       // register index
//    PMC1,8,       // value write to/read from PMIC/VR
//    PMC2,8,       // on write, this field indicates the 8-bit PMIC/VR slave device address.
                    //           LSB is 1 to trigger read operation, 0 to trigger write operation.
                    // on read,  returns the status of last operation.
                    //           0xAC - success, 0xE2 - error, 0xCC - ongoing
    // I2C Slave Index
    Status = ImmEcIdxWrite (CpmTableProtocolPtr, 0x90, SlaveIdx);
    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    if (FALSE == boolRead)
    {
      // I2C Slave Data to Write
      Status = ImmEcIdxWrite (CpmTableProtocolPtr, 0x91, *SlaveData);
      if (EFI_ERROR (Status))
      {
        ASSERT_EFI_ERROR (Status);
        break;
      }
    }

    // I2C Slave Addr and Execute write command.
    Value8 = (I2c7BitSlaveAddr << 1) | ( TRUE == boolRead ? 1 : 0);
    Status = ImmEcIdxWrite (CpmTableProtocolPtr, 0x92, Value8);
    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    // Wait device response
    do
    {
      for (Index8 = 0; Index8 < 50; Index8++)
      {
        CpmTableProtocolPtr->CommonFunction.IoRead8 (0x80);
      }
      Offset = 0x92;
      Status = CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
      ASSERT_EFI_ERROR (Status);
      if (0xAC == Value8)
      {
        Status = EFI_SUCCESS;
        break;
      }
      else if (0xE2 == Value8)
      {
        Status = EFI_DEVICE_ERROR;
        ASSERT_EFI_ERROR (Status);
        break;
      }
    } while (Value8 == 0xCC);

    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    if (TRUE == boolRead)
    {
      Offset = 0x91;
      Status = CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &Offset, &Value8);
      if (EFI_ERROR (Status))
      {
        ASSERT_EFI_ERROR (Status);
        break;
      }
      *SlaveData = Value8;
    }
    // Success Operation.
    DEBUG ((DEBUG_INFO, "  EC I2C %s - %r, Addr[0x%x],Offset[0x%x]:[0x%x]\n", (boolRead ? "Read":"Write"), Status, I2c7BitSlaveAddr, SlaveIdx, *SlaveData));
  } while (FALSE);

  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "  EC I2C %s - %r\n", (boolRead ? "Read":"Write"), Status));
  }

  return Status;
}

/**
  This function used to retrieve onboard Voltage information

  @param[in]  VoltageId             The Voltage Id
  @param[out] Defalut               The Default voltage setting
  @param[out] Min                   The Min voltage setting
  @param[out] Max                   The Max voltage setting
  @param[out] Step                  The Step voltage setting

  @retval EFI_SUCCESS if retrieve voltage information successfully
          EFI_UNSUPPORTED if voltage is unsupported.
          others by callee

**/
EFI_STATUS
AodGetOnBoardVoltageInfo (
  IN   UINT8        VoltageId,
  OUT  UINT16       *Defalut,
  OUT  UINT16       *Min,
  OUT  UINT16       *Max,
  OUT  UINT16       *Step
  )
{
  switch (VoltageId) {
    case AOD_VOLTAGE_APU_VDDIO:
      *Defalut = 1100;
      *Min = 800;
      *Max = 1500;
      *Step = 10;
      return EFI_SUCCESS;
    case AOD_VOLTAGE_VDD_MEM:
      *Defalut = 780;
      *Min = 530;
      *Max = 1030;
      *Step = 2;
      return EFI_SUCCESS;
    default:
      break;
  }
  return EFI_UNSUPPORTED;
}
/**
  This function used to get current VoltageId Voltage in mV

  @param[in]  VoltageId             The Voltage Id
  @param[out] Voltage               The voltage out pointer in mV

  @retval EFI_SUCCESS if get voltage successfully
          EFI_UNSUPPORTED if voltage is unsupported.
          others by callee

**/
EFI_STATUS
AodGetOnBoardVoltage (
  IN   UINT8        VoltageId,
  OUT  UINT16       *Voltage
  )
{
  EFI_STATUS                 Status;
  AMD_CPM_TABLE_PROTOCOL    *CpmTableProtocolPtr;
  UINT8                      Data;
  if (Voltage == NULL) return EFI_INVALID_PARAMETER;

  Status = gBS->LocateProtocol (
                  &gAmdCpmTableProtocolGuid,
                  NULL,
                  (VOID**)&CpmTableProtocolPtr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Can Get %g Status = %r\n", &gAmdCpmTableProtocolGuid, Status));
    return Status;
  }

  switch (VoltageId) {
    case AOD_VOLTAGE_APU_VDDIO:
      Status = EcI2cDevByteAccess (CpmTableProtocolPtr, TRUE, 0x30, 0xF8, &Data);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Get Voltage status = %r\n", Status));
        return Status;
      }
      if (Data & 0x80) {
        *Voltage = (UINT16)(1100 - (Data&0x7f) * 5);
      } else {
        *Voltage = (UINT16)(1100 + (Data&0x7f) * 5);
      }
      return EFI_SUCCESS;
    case AOD_VOLTAGE_VDD_MEM:
      Status = EcI2cDevByteAccess (CpmTableProtocolPtr, TRUE, 0x30, 0xFB, &Data);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Get Voltage status = %r\n", Status));
        return Status;
      }
      if (Data & 0x80) {
        *Voltage = (UINT16)(780 - (Data&0x7f) * 2);
      } else {
        *Voltage = (UINT16)(780 + (Data&0x7f) * 2);
      }
      return EFI_SUCCESS;
    default:
      break;
  }
  return EFI_UNSUPPORTED;
}

/**
  This function used to set current VoltageId Voltage in mV into VR

  @param[in]  VoltageId             The Voltage Id
  @param[out] Voltage               The voltage in mV

  @retval EFI_SUCCESS if set voltage successfully
          EFI_UNSUPPORTED if voltage is unsupported.
          others by callee

**/
EFI_STATUS
AodSetOnBoardVoltage (
  IN  UINT8        VoltageId,
  IN  UINT16       Voltage
  )
{
  UINT8 Data;

  switch (VoltageId) {
    case AOD_VOLTAGE_APU_VDDIO:
      if (Voltage >= 1100) {
        Data = (UINT8)((Voltage-1100)/5);
      } else {
        Data = (UINT8)((1100-Voltage)/5 | 0x80);
      }
      IoWrite8 (0x72, 0xA0);
      IoWrite8 (0x73, Data);
      return EFI_SUCCESS;
    case AOD_VOLTAGE_VDD_MEM:
      if (Voltage >= 780) {
        Data = (UINT8)((Voltage-780)/2);
      } else {
        Data = (UINT8)((780-Voltage)/2 | 0x80);
      }
      IoWrite8 (0x72, 0xA2);
      IoWrite8 (0x73, Data);
      return EFI_SUCCESS;
    default:
      break;
  }
  return EFI_UNSUPPORTED;
}
