/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include "Uefi.h"
#include <AMD.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/AmdPspCommonLib.h>
#include <Library/AmdFtpmLib.h>
#include <IndustryStandard/Tpm20.h>
#include <Library/SmnAccessLib.h>
#include <Hsp.h>
#include <Filecode.h>

#define PATCH_BASESIZE   1

#define FILECODE LIBRARY_HSPLIB_HSPLIB_FILECODE
#define NBIF_STRAP_BIOS_CNTL_SMN    0x10133204
#define RCC_DEV2_EPF2_STRAP0_SMN    0x10136400


#if HSP_DEBUG_ENABLE
VOID
HspDumpData (
  IN  VOID    *p,
  IN  UINT32  iSize
  )
{
  UINT32  i;
  UINT32  j;
  UINT32  k;
  UINT8   arr[16];
  UINT8   *ptr;

  ptr = (UINT8*)p;

  IDS_HDT_CONSOLE_PSP_TRACE ("\t====================================================\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
  IDS_HDT_CONSOLE_PSP_TRACE ("\t----------------------------------------------------");

  for (i=0; i<iSize; i++) {
    if ( (i%16) == 0 ) {
      j = 0;
      if ( i != 0 ) {
        for (k=0; k<16; k++) {
          if ( arr[k] >= 0x21 && arr[k] <= 0x7e ) {
            IDS_HDT_CONSOLE_PSP_TRACE ("%c", arr[k]);
          } else {
            IDS_HDT_CONSOLE_PSP_TRACE (".");
          }
        }
      }
      IDS_HDT_CONSOLE_PSP_TRACE ("\n\t%03X0 ", i/16);
    }
    arr[j] = ptr[i];
    j++;
    IDS_HDT_CONSOLE_PSP_TRACE ("%02X ", ptr[i]);
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\n\t====================================================\n");
}
#endif


/**
 *  Get value by SMN
 *
 * @param[in]    AddressSMN        SMN address
 * @return       UINT32            Value
 *
 */
UINT32
FtpmGetValueSMN (
  IN UINT32 AddressSMN
  )
{
  UINT32 iReturn = 0xFFFFFFFF;
  SmnRegisterRead (
    0,
    AddressSMN,
    (VOID *)&iReturn
    );
  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.FtpmGetValue AddressSMN[0x%x]=0x%x\n", AddressSMN, iReturn);
  return iReturn;
}


/**
 *  Set value by SMN
 *
 * @param[in]    AddressSMN        SMN address
 * @param[in]    Value32           Value need to be set
 * @param[in]    Flags             Flags - BIT0 indicates S3 save/restore
 *
 */
VOID
FtpmSetValueSMN (
  IN UINT32 AddressSMN,
  IN UINT32 Value32,
  IN UINT32 Flags
  )
{
  SmnRegisterWrite (
    0,
    AddressSMN,
    (VOID *)&Value32,
    Flags
    );
}


/**
 *  Get value by MMIO
 *
 * @param[in]    iOffset        SMN address
 * @return       UINT32         The value to write
 *
 */

UINT32
FtpmGetValueMmio (
  IN UINT32 iOffset
  )
{
  UINT32            HspMmioBase;
  volatile UINT32   *PtrHspMmioBase;
  UINT32            Return32;

  // TODO: refine later. Use real offset.
  HspMmioBase = PcdGet32 (PcdAmdHspMmioBase);
  ASSERT (HspMmioBase != 0);
  HspMmioBase = iOffset - HSP_APERTURE_BASE + HspMmioBase;
  PtrHspMmioBase  = (volatile UINT32*)(UINTN)HspMmioBase;

  Return32 = *PtrHspMmioBase;

#if HSP_DEBUG_ENABLE
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP.FtpmGetValueMmio iOffset[0x%lx]=0x%x 64\n", PtrHspMmioBase, Return32);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP.FtpmGetValueMmio iOffset[0x%x]=0x%x 32\n", PtrHspMmioBase, Return32);
  }
#endif

  return Return32;
}


/**
 *  Set value by MMIO
 *
 * @param[in]    iOffset           SMN address
 * @param[in]    iValue            Value need to be set
 *
 */

VOID
FtpmSetValueMmio (
  IN UINT32 iOffset,
  IN UINT32 iValue
  )
{
  UINT32            HspMmioBase;
  volatile UINT32   *PtrHspMmioBase;

  HspMmioBase = PcdGet32 (PcdAmdHspMmioBase);
  ASSERT (HspMmioBase != 0);
  HspMmioBase = iOffset - HSP_APERTURE_BASE + HspMmioBase;
  PtrHspMmioBase  = (volatile UINT32*)(UINTN)HspMmioBase;

  *PtrHspMmioBase = iValue;

#if HSP_DEBUG_ENABLE
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP.FtpmSetValueMmio iOffset[0x%lx]=0x%x 64\n", PtrHspMmioBase, *PtrHspMmioBase);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP.FtpmSetValueMmio iOffset[0x%x]=0x%x 32\n", PtrHspMmioBase, *PtrHspMmioBase);
  }
#endif
}


/**
 *  Check for HSP TPM CRB command's completeness
 *
 * @return       UINT32 TPM CRB status
 *
 */

UINT32
FtpmCheckStatus ()
{
  return FtpmGetValueMmio (HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_STATUS);
}


/**
 *  Check if specific channel is ready.
 *
 * @param[in] pChannelNumber    HSP channel number
 *
 * @return    BOOLEAN           Channel ready or not.
 *
 */

BOOLEAN
IsHspReady (
  IN       VOID    *pChannelNumber
  )
{
  UINT32              iH2C;
  HSP_MAILBOX_CHANNEL *ptr;

  UINT32              HspMmioBase;
  volatile UINT32     *PtrHspMmioBase;

  iH2C  = 0;
  ptr   = (HSP_MAILBOX_CHANNEL*)pChannelNumber;

  switch (*ptr) {
    case X86_VTL0_SPI:
      iH2C = HSP_H2C_HSP_L0_DOORBELL_SMN_BASE;
      break;
    case X86_VTL1_SPI:
      iH2C = HSP_H2C_HSP_L1_DOORBELL_SMN_BASE;
      break;
    case X86_VTL0_TPM:
    default:
      iH2C = HSP_H2C_TPM_L0_DOORBELL_SMN_BASE;
      break;
  }

  HspMmioBase = PcdGet32 (PcdAmdHspMmioBase);
  ASSERT (HspMmioBase != 0);
  HspMmioBase = iH2C - HSP_APERTURE_BASE + HspMmioBase;

  PtrHspMmioBase  = (volatile UINT32*)(UINTN)HspMmioBase;
  ASSERT ((*PtrHspMmioBase)!=0xFFFFFFFF);

  return ( *PtrHspMmioBase ) ? TRUE : FALSE;
}


/**
 *  Notify HSP firmware to do the command by HSP doorbell.
 *
 * @param[in] ChannelNumber     HSP channel number
 *
 */

VOID
NotifyHSP (
  IN       HSP_MAILBOX_CHANNEL    ChannelNumber
  )
{
  UINT32 iC2H;
  UINT32 iH2C;

  iC2H = 0;
  iH2C = 0;

  switch (ChannelNumber) {
    case X86_VTL0_SPI:
      iC2H = HSP_C2H_HSP_L0_DOORBELL_SMN_BASE;
      iH2C = HSP_H2C_HSP_L0_DOORBELL_SMN_BASE;
      break;
    case X86_VTL1_SPI:
      iC2H = HSP_C2H_HSP_L1_DOORBELL_SMN_BASE;
      iH2C = HSP_H2C_HSP_L1_DOORBELL_SMN_BASE;
      break;
    case X86_VTL0_TPM:
    default:
      iC2H = HSP_C2H_TPM_L0_DOORBELL_SMN_BASE;
      iH2C = HSP_H2C_TPM_L0_DOORBELL_SMN_BASE;
      break;
  }

  FtpmSetValueMmio (iH2C, 0);
  FtpmSetValueMmio (iC2H, 1);
}


/**
 *  Set Channel 0 TPM to idle mode.
 *
 * @param[in] ChannelNumber     HSP channel number
 *
 */
VOID
FtpmGotoIdle (
  IN       HSP_MAILBOX_CHANNEL    ChannelNumber
  )
{
  IDS_HDT_CONSOLE_PSP_TRACE ("\t%a - Start\n", __FUNCTION__);
  FtpmSetValueMmio (HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_REQUEST, 2);
  NotifyHSP (ChannelNumber);
  PspLibTimeOut (PcdGet64 (PcdAmdFtpmTimeout2), &IsHspReady, (VOID*)&ChannelNumber);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t%a - End\n", __FUNCTION__);
}


/**
 *  Copy command to HSP mailbox buffer.
 *
 * @param[in] *CommandBuffer    Command buffer base address
 * @param[in] CommandSize       Command size
 *
 */
VOID
CopyToHSPBuffer (
  IN     VOID                 *CommandBuffer,
  IN     UINT32                CommandSize
  )
{
  UINT32  i;
  UINT32  iBase;
  UINT32  iSize;
  UINT32  iRemainder;
  UINT32  iRemainderData;
  UINT32  *pCmdBuffer32;

  i               = 0;
  iBase           = 0;
  iSize           = 0;
  iRemainder      = 0;
  iRemainderData  = 0;
  pCmdBuffer32    = NULL;

  IDS_HDT_CONSOLE_PSP_TRACE ("\t%a - Start\n", __FUNCTION__);

  // Set TPM_BUFFERS.ControlArea.CommandSize
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSet TPM_BUFFERS.ControlArea.CommandSize: 0x%x \n", CommandSize);
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_COMMANDSIZE,
    CommandSize
    );

  // Set TPM_BUFFERS.ControlArea.ResponseSize
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSet TPM_BUFFERS.ControlArea.ResponseSize: 0x%x \n", HSP_COMMANDRESPONSE_BUFFER_SIZE);
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_RESPONSESIZE,
    HSP_COMMANDRESPONSE_BUFFER_SIZE
    );

  // Set TPM_BUFFERS.ControlArea.InterruptControl to 0
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSet TPM_BUFFERS.ControlArea.InterruptControl: 0x%x \n", 0);
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_INTERRUPTCONTROL,
    0
    );

  // Set TPM_BUFFERS.CommandResponseBuffer
  iBase = HSP_C2H_TPM_L0_SMN_BASE + HSP_MAILBOX_SIZE - HSP_COMMANDRESPONSE_BUFFER_SIZE;
  iSize = ALIGN (CommandSize, 4);
  iRemainder = iSize - CommandSize; // Everytime copy 4 bytes, Remainder keep rest of bytes.
  // TODO: MMIO block copy, to remove Remainder

  if ( iRemainder ) {
    iSize -= 4;
    iRemainder = CommandSize & 3;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommandSize=%d, iSize=%d, iRemainder=%d\n", CommandSize, iSize, iRemainder);

  pCmdBuffer32 = (UINT32*)CommandBuffer;
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tpCmdBuffer32 (0x%lx)=0x%x 64\n", pCmdBuffer32, *pCmdBuffer32);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tpCmdBuffer32 (0x%x)=0x%x 32\n", pCmdBuffer32, *pCmdBuffer32);
  }

  for (i=0; i<iSize; i+=4, pCmdBuffer32++) {
    FtpmSetValueMmio (iBase+i, *pCmdBuffer32);
  }

  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBefore process remainder, pCmdBuffer32 (0x%lx)=0x%x, i=%d 64\n", pCmdBuffer32, *pCmdBuffer32, i);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBefore process remainder, pCmdBuffer32 (0x%x)=0x%x, i=%d 32\n", pCmdBuffer32, *pCmdBuffer32, i);
  }

  if ( iRemainder ) {
    iRemainderData = 0;
    CopyMem (&iRemainderData, pCmdBuffer32, iRemainder);
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tiRemainderData=0x%x\n", iRemainderData);
    FtpmSetValueMmio (iBase+i, iRemainderData);
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\t%a - End\n", __FUNCTION__);
}


/**
 *  Copy data in HSP mailbox buffer to caller response buffer.
 *
 * @param[in] *ResponseBuffer    Response buffer base address
 * @param[in] ResponseSize       Response size
 *
 */
VOID
CopyFromHSPBuffer (
  IN     VOID                 *ResponseBuffer,
  IN     UINT32               ResponseSize
  )
{
  UINT32  i;
  UINT32  iBase;
  UINT32  iSize;
  UINT32  iRemainder;
  UINT32  iRemainderData;
  UINT32  *pCmdBuffer32;

  i               = 0;
  iBase           = HSP_C2H_TPM_L0_SMN_BASE + HSP_MAILBOX_SIZE - HSP_COMMANDRESPONSE_BUFFER_SIZE;
  iSize           = ALIGN (ResponseSize, 4);
  iRemainder      = iSize - ResponseSize;
  iRemainderData  = 0;
  pCmdBuffer32    = NULL;

  if ( iRemainder ) {
    iSize -= 4;
    iRemainder = ResponseSize & 3;
  }

  // TODO: MMIO block copy, to remove Remainder

  pCmdBuffer32 = (UINT32*)ResponseBuffer;
  for (i=0; i<iSize; i+=4, pCmdBuffer32++) {
    *pCmdBuffer32 = FtpmGetValueMmio (iBase+i);
  }

  if ( iRemainder ) {
    iRemainderData = FtpmGetValueMmio (iBase+i);
    CopyMem (pCmdBuffer32, &iRemainderData, iRemainder);
  }
}


/**
 *  SPI_REQUEST send command
 *
 * @param[in]    CommandBuffer              Point to the TPM command buffer
 * @param[in]    CommandSize                Size of the TPM command buffer
 *
 * @return       EFI_SUCCESS                Command executed successfully
 * @return       EFI_UNSUPPORTED            HSP Device unsupported
 * @return       EFI_NOT_READY              HSP Device Channel 1 is not ready
 * @return       EFI_TIMEOUT                Command fail due the time out
 * @return       EFI_DEVICE_ERROR           Command fail due the error status set
 *
 */

EFI_STATUS
SpiRequestSendCommand (
  IN      SPI_REQUEST           *CommandBuffer,
  IN      UINT32                CommandSize
  )
{
  BOOLEAN     ExeStatus;
  SPI_STATUS  SpiStatus;
  UINT32      i, iBase, iSize, iRemainder, iRemainderData, *pCmdBuffer32;
  HSP_MAILBOX_CHANNEL HspMailboxChannel = X86_VTL0_SPI;

  IDS_HDT_CONSOLE_PSP_TRACE ("\tHSP.Ftpm HspSpiProtocol\n");

  if ( !IsHspReady ((VOID*)&HspMailboxChannel) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tHSP VTL0 is not ready, exit\n");
    return EFI_NOT_READY;
  }

  // Validate Input parameters
  if ( ( CommandBuffer == NULL ) || ( CommandSize > HSP_C2H_HSP_L0_SMN_SIZE ) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tInvalid parameters (HSP.Ftpm.HspSpiProtocol)\n");
    return EFI_INVALID_PARAMETER;
  }

  if ( CommandBuffer->RequestType >= SPI_MAX_FUNCTION ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSPI_REQUEST function do not support (HSP.Ftpm.HspSpiProtocol)\n");
    return EFI_UNSUPPORTED;
  }

  //
  // Copy Command buffer data to HSP buffer
  //
  iBase = HSP_C2H_HSP_L0_SMN_BASE;
  iSize = ALIGN (CommandSize, 4);
  iRemainder = iSize - CommandSize;

  if ( iRemainder ) {
    iSize -= 4;
    iRemainder = CommandSize & 3;
  }

  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommandSize=%d, iSize=%d, iRemainder=%d\n", CommandSize, iSize, iRemainder);

  pCmdBuffer32 = (UINT32*)CommandBuffer;
  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tpCmdBuffer32 (0x%lx)=0x%x 64\n", pCmdBuffer32, *pCmdBuffer32);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tpCmdBuffer32 (0x%x)=0x%x 32\n", pCmdBuffer32, *pCmdBuffer32);
  }

  for (i=0; i<iSize; i+=4, pCmdBuffer32++) {
    FtpmSetValueMmio (iBase+i, *pCmdBuffer32);
  }

  if ( sizeof (UINTN) == sizeof (UINT64) ) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBefore process remainder, pCmdBuffer32 (0x%lx)=0x%x, i=%d 64\n", pCmdBuffer32, *pCmdBuffer32, i);
  } else {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBefore process remainder, pCmdBuffer32 (0x%x)=0x%x, i=%d 32\n", pCmdBuffer32, *pCmdBuffer32, i);
  }

  if ( iRemainder ) {
    iRemainderData = 0;
    CopyMem (&iRemainderData, pCmdBuffer32, iRemainder);
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tiRemainderData=0x%x\n", iRemainderData);
    FtpmSetValueMmio (iBase+i, iRemainderData);
  }

  // Notify HSP
  NotifyHSP (X86_VTL0_SPI);

  // Wait for HSP to finish
  ExeStatus = PspLibTimeOut (PcdGet64 (PcdAmdFtpmTimeout2), &IsHspReady, (VOID*)&HspMailboxChannel);

  if (ExeStatus == FALSE) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tCommand time out (HSP.fTPM.SpiRequestSendCommand)\n");
    return (EFI_TIMEOUT);
  }

  SpiStatus = (SPI_STATUS)FtpmGetValueMmio (HSP_C2H_HSP_L0_SMN_BASE + GET_SPI_REQUEST_STATUS);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tSPI_REQUEST Command Return Status=0x%x\n", SpiStatus);
  if ( SPI_RESPONSE_SUCCESS != SpiStatus ) {
    return (EFI_DEVICE_ERROR);
  }

  return (EFI_SUCCESS);

}


EFI_STATUS
SendIrq2Hsp (
  IN  UINT32  IrqRegisterSMNAddress,
  IN  UINT8   *Irq,
  IN  UINT32  IrqAmount
  )
{
  UINT32          i;
  SET_OUTPUT_IRQ  HspSpi;

  i = 0;
  ZeroMem (&HspSpi, sizeof (SET_OUTPUT_IRQ));

  HspSpi.Request.RequestType    = SPI_SET_OUTPUT_IRQ;
  HspSpi.Request.Status         = SPI_RESPONSE_SUCCESS;
  HspSpi.IrqRegisterSMNAddress  = IrqRegisterSMNAddress;
  for (i=0; i<IrqAmount; i++) {
    HspSpi.Irq[i] = Irq[i];
  }

  return SpiRequestSendCommand ((SPI_REQUEST*)&HspSpi, sizeof (SET_OUTPUT_IRQ));
}


UINT64
GetHspChannelOffset (
  IN  UINT32 Types
  )
{
  UINT64 iOffset = 0;;

  switch (Types) {
    case 0x00:
      iOffset = HSP_C2H_TPM_L0_OFFSET + GET_TPM_CONTROL_AREA;
      break;
    case 0x01:
      iOffset = HSP_C2H_TPM_L0_DOORBELL_OFFSET;
      break;
    case 0x02:
      iOffset = HSP_H2C_TPM_L0_DOORBELL_OFFSET;
      break;
    case 0x10:
      iOffset = HSP_C2H_HSP_L0_OFFSET;
      break;
    case 0x11:
      iOffset = HSP_C2H_HSP_L0_DOORBELL_OFFSET;
      break;
    case 0x12:
      iOffset = HSP_H2C_HSP_L0_DOORBELL_OFFSET;
      break;
    case 0x20:
      iOffset = HSP_C2H_HSP_L1_OFFSET;
      break;
    case 0x21:
      iOffset = HSP_C2H_HSP_L1_DOORBELL_OFFSET;
      break;
    case 0x22:
      iOffset = HSP_H2C_HSP_L1_DOORBELL_OFFSET;
      break;
    case 0x30:
      iOffset = HSP_C2H_HSP_L2_OFFSET;
      break;
    case 0x31:
      iOffset = HSP_C2H_HSP_L2_DOORBELL_OFFSET;
      break;
    case 0x32:
      iOffset = HSP_H2C_HSP_L2_DOORBELL_OFFSET;
      break;
    case 0x40:
      iOffset = HSP_C2H_HSP_L3_OFFSET;
      break;
    case 0x41:
      iOffset = HSP_C2H_HSP_L3_DOORBELL_OFFSET;
      break;
    case 0x42:
      iOffset = HSP_H2C_HSP_L3_DOORBELL_OFFSET;
      break;
    default:
      break;
  }

  return iOffset;
}


VOID
SetVTL0TpmBaseSize ()
{
  UINT32  HspMmioBase;

  HspMmioBase = PcdGet32 (PcdAmdHspMmioBase);
  ASSERT (HspMmioBase != 0);
  IDS_HDT_CONSOLE_PSP_TRACE ("\t%a - Start\n", __FUNCTION__);

  // Set TPM_BUFFERS.ControlArea.CommandSize
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tSet TPM_BUFFERS.ControlArea.CommandSize: 0x%x\n",
    HSP_COMMANDRESPONSE_BUFFER_SIZE
    );
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_COMMANDSIZE,
    HSP_COMMANDRESPONSE_BUFFER_SIZE
    );

  // Set TPM_BUFFERS.ControlArea.ResponseSize
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tSet TPM_BUFFERS.ControlArea.ResponseSize: 0x%x \n",
    HSP_COMMANDRESPONSE_BUFFER_SIZE
    );
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_RESPONSESIZE,
    HSP_COMMANDRESPONSE_BUFFER_SIZE
    );

  // Set TPM_BUFFERS.ControlArea.CommandAddress
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tSet TPM_BUFFERS.ControlArea.CommandAddress: 0x%x\n",
    HspMmioBase + GET_TPM_COMMANDRESPONSE_BUFFER
    );
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_COMMANDADDRESS,
    HspMmioBase+ GET_TPM_COMMANDRESPONSE_BUFFER
    );
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_COMMANDADDRESS + 4,
    0
    );

  // Set TPM_BUFFERS.ControlArea.ResponseAddress
  IDS_HDT_CONSOLE_PSP_TRACE (
    "\t\tSet TPM_BUFFERS.ControlArea.ResponseAddress: 0x%x\n",
    HspMmioBase+ GET_TPM_COMMANDRESPONSE_BUFFER
    );
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_RESPONSEADDRESS,
    HspMmioBase+ GET_TPM_COMMANDRESPONSE_BUFFER
    );
  FtpmSetValueMmio (
    HSP_C2H_TPM_L0_SMN_BASE + GET_TPM_CONTROL_AREA_RESPONSEADDRESS + 4,
    0
    );

  IDS_HDT_CONSOLE_PSP_TRACE ("\t%a - End\n", __FUNCTION__);
}

/**
 * @brief Set the Register Bit by Smn
 *
 * @param AddressSMN  SMN address of the register
 * @param SetBit      True for Set, False for Clear
 * @param Bit         Bit to set or clear
 * @param Flags       Flags to set register
 * @return EFI_STATUS EFI_SUCCESS  The function completed successfully; others, fail
 */
EFI_STATUS
EFIAPI
FtpmSetRegisterBitSmn (
  IN UINT32 AddressSMN,
  IN BOOLEAN SetBit,
  IN UINT32  Bit,
  IN UINT32  Flags
)
{
  UINT32      RegValue;
  RegValue = FtpmGetValueSMN (AddressSMN);
  if (RegValue == 0xFFFFFFFF) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Can't access 0x%x, Value is 0xFFFFFFFF.\n", AddressSMN);
    return EFI_INVALID_PARAMETER;
  }

  if (SetBit) {  // Set Bit
    if ((RegValue & Bit) == Bit)  {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBIT Mask %x of register 0x%x has been set already, ignoring.\n", Bit, AddressSMN);
      return EFI_SUCCESS;
    } else {
      RegValue |= Bit;
    }
  } else {  // Clear Bit
    if ((RegValue & Bit) != Bit) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tBIT Mask %x of register 0x%x has been cleared already, ignoring.\n", Bit, AddressSMN);
      return EFI_SUCCESS;
    } else {
      RegValue &= (~Bit);
    }
  }

  FtpmSetValueSMN (AddressSMN, RegValue, Flags);

  RegValue = FtpmGetValueSMN (AddressSMN);
  if (RegValue == 0xFFFFFFFF) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Can't access 0x%x, Value is 0xFFFFFFFF.\n", AddressSMN);
    return EFI_INVALID_PARAMETER;
  }
  IDS_HDT_CONSOLE_PSP_TRACE ("\t\tRead back 0x%x, Value is 0x%x.\n", AddressSMN, RegValue);
  if (SetBit) {  // Set BIT
    if ((RegValue & Bit) != Bit) {  // Set BIT failed
      return EFI_ACCESS_DENIED;
    }
  } else {  // Clear BIT
    if ((RegValue & Bit) == Bit) {  // Clear BIT failed
      return EFI_ACCESS_DENIED;
    }
  }
  return EFI_SUCCESS;
}

/**
 * @brief Show or Hide HSP Pcie Device
 * @details Show or Hide HSP Pcie Device
 *
 * @param[in] ShowHspPci       Show/hide HSP Pcie Device
 * @param[in] Flags       Flags - BIT0 indicates S3 save/restore
 *
 * @return    EFI_STATUS
 *              - EFI_SUCCESS           The Function Completed Successfully.
 *              - EFI_INVALID_PARAMETER Can't access NBIFMM::RCC_DEV2_EPF2_STRAP0
 */
EFI_STATUS
EFIAPI
ShowHspPciDevice (
  IN BOOLEAN ShowHspPci,
  IN UINT32  Flags
)
{
  EFI_STATUS  Status;
  Status      = EFI_SUCCESS;

  // Enable SW write to NBIF BIOS Strap
  Status = FtpmSetRegisterBitSmn (NBIF_STRAP_BIOS_CNTL_SMN, TRUE, BIT0, Flags);
  ASSERT (Status == EFI_SUCCESS);
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Can't enable SW write to NBIF BIOS Strap:%r.\n", Status);
  } else {
    Status = FtpmSetRegisterBitSmn (RCC_DEV2_EPF2_STRAP0_SMN, ShowHspPci, BIT28, Flags);
    ASSERT (Status == EFI_SUCCESS);
    if (EFI_ERROR (Status)) {
      IDS_HDT_CONSOLE_PSP_TRACE ("\t\tERROR: Can't set/clear BIT28 in register 0x%x:%r.\n", RCC_DEV2_EPF2_STRAP0_SMN, Status);
    }
  }
  return Status;
}