/*****************************************************************************
 * Copyright (C) 2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Uefi/UefiSpec.h>
#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DashAsfSmbusLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "Mctp.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/**
  DashSmbusIoRead - Smbus io read function.

  @param  Offset the address to read from
  @retval Data - return value

**/

UINT8
DashSmbusIoRead (
  IN UINT8 Offset
  )
{
  UINT8 Data8;
  //DEBUG ((DEBUG_INFO, "Enter DashSmbusIoRead.\n"));
  Data8 =(UINT8) _inp(DASH_SMBUS_ASF_IOBASE + Offset);
  return Data8;
}

/**
  DashSmbusIoWrite - Smbus io write function.

  @param  Offset the address to read from
  @param  Data the write value
  @retval Data - return Status

**/

VOID
DashSmbusIoWrite (
  IN UINT8 Offset,
  IN UINT8 Data
  )
{
  //DEBUG ((DEBUG_INFO, "Enter DashSmbusIoWrite.\n"));
  _outp(DASH_SMBUS_ASF_IOBASE + Offset, Data);
}

/**
  DashWaitForSmbusBuf - Wait ASF slave smbus buffer full.

  @retval Data - return Status

**/

EFI_STATUS
DashWaitForSmbusBuf(VOID)
{
  UINTN Count;
  UINT8 DataBankSelect;
  EFI_STATUS Status;

  Status = EFI_NOT_READY;

  for (Count = 0; Count < DASH_WAIT_SMBUS_SLAVE_BUFFER_STALL; Count++) {
    gBS->Stall(1000);
    DataBankSelect = DashSmbusIoRead(ASF_DATABANKSEL);
    //DEBUG ((EFI_D_INFO, "Wait For Smbus Buffer %x\n", DataBankSelect));
    if((DataBankSelect & BIT2) == BIT2) {
      Status = EFI_SUCCESS;
      break;
    } else if ((DataBankSelect & BIT3) == BIT3) {
      Status = EFI_SUCCESS;
      break;
    }
  }
  return Status;
}

/**
  DashResetAsfSlave - Reset Dash ASF Slave bus.
  @retval None

**/

VOID
DashResetAsfSlave(VOID)
{
  DashSmbusIoWrite(ASF_SLAVE_EN, 0x10);
  gBS->Stall(1);;
  DashSmbusIoWrite(ASF_SLAVE_EN, 0x00);
}

/**
  DashEnableSmbusListening - Reset Dash ASF Slave bus.
  @retval None

**/

VOID
DashEnableSmbusListening(VOID)
{
  DashSmbusIoWrite(ASF_LISTEN_ADR, MCTP_SOURCE_SLAVE_ADDR);
  DashSmbusIoWrite(ASF_REMOTECTRL_ADR, (MCTP_SOURCE_SLAVE_ADDR & 0xFE));
  DashSmbusIoWrite(ASF_SLAVE_EN, 0x02);
  gBS->Stall(2);
}

/**
  DashReadDataBank - Read data bank
  @param  *BufferSize - a pointer to Buffer Size.
  @param  *MctpBuffer - a pointer to Buffer.

  @retval None

**/

VOID
DashReadDataBank(
  UINT8 *BufferSize,
  UINT8 *MctpBuffer
)
{
  UINT8 Buffer[MCTP_SMBUS_BLOCK_LENGTH];
  UINT8 Addr;
  UINT8 Cmd;
  UINT8 Length;
  UINT8 i;

  //
  // Read data to Buffer
  //
  DashSmbusIoWrite(ASF_SETDATAREAD_POINTER, 0x00);

  Addr = DashSmbusIoRead(ASF_DATA_INDEX);
  Cmd = DashSmbusIoRead(ASF_DATA_INDEX);
  Length = DashSmbusIoRead(ASF_DATA_INDEX);

  for (i = 0; i <= Length-1; i++) {
    Buffer[i] = DashSmbusIoRead(ASF_DATA_INDEX);
  }

  DashSmbusIoWrite(ASF_STATUS, BIT6);
  *BufferSize = Length;
  CopyMem(MctpBuffer, &Buffer, Length);
}

/**
  DashListenModeRead - Read Data from Slave device.
  @param  *BufferSize - a pointer to Buffer Size.
  @param  *MctpBuffer - a pointer to Buffer.

  @retval Return - EFI status code.

**/

EFI_STATUS
EFIAPI
DashListenModeRead (
  UINT8 *BufferSize,
  UINT8 *MctpBuffer
)
{
  EFI_STATUS Status;
  UINT8 SlaveStatus;
  UINT8 DataBankSel;

  Status = EFI_SUCCESS;

  do{
    SlaveStatus = DashSmbusIoRead(ASF_SLAVE_STATUS);
    if(SlaveStatus){
      //DEBUG ((EFI_D_INFO, "DashListenModeRead Slave status %x\n", SlaveStatus));
      DashSmbusIoWrite(ASF_SLAVE_STATUS, SlaveStatus);
    }
  }while(SlaveStatus!=0);

  DataBankSel = DashSmbusIoRead(ASF_DATABANKSEL);

  if((DataBankSel & BIT2) && (DataBankSel & BIT3))
  {
     //DEBUG ((EFI_D_INFO, "DashListenModeRead both bank are full.\n"));
     DashReadDataBank((UINT8 *)BufferSize, (UINT8 *)MctpBuffer);
     DashSmbusIoWrite(ASF_DATABANKSEL, BIT2);
     DashReadDataBank((UINT8 *)BufferSize, (UINT8 *)MctpBuffer);
     DashSmbusIoWrite(ASF_DATABANKSEL, BIT3);
  }
  else if(DataBankSel & BIT2) { // Data bank 0 is full
    //DEBUG ((EFI_D_INFO, "DashListenModeRead bank 0 is full.\n"));
    DashSmbusIoWrite(ASF_DATABANKSEL, 0x00);              // Read data from data bank 0
    DashReadDataBank((UINT8 *)BufferSize, (UINT8 *)MctpBuffer);
    DashSmbusIoWrite(ASF_DATABANKSEL, BIT2);
  }
  else if(DataBankSel & BIT3) { // Data bank 1 is full
    //DEBUG ((EFI_D_INFO, "DashListenModeRead bank 1 is full.\n"));
    DashSmbusIoWrite(ASF_DATABANKSEL, BIT4);              // Read data from data bank 1
    DashReadDataBank((UINT8 *)BufferSize, (UINT8 *)MctpBuffer);
    DashSmbusIoWrite(ASF_DATABANKSEL, BIT3);
  }

  DashSmbusIoWrite(ASF_DATABANKSEL, (BIT0 | BIT1 | BIT2 | BIT3));

  return EFI_SUCCESS;
}
