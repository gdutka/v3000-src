/*****************************************************************************
 *
 * Copyright (C) 2016-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#ifndef _EFI_AMD_UNIFY_MESSAGE_H
#define _EFI_AMD_UNIFY_MESSAGE_H

#include <Library/SerialPortLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/AmdIdsDebugPrintLib.h>

#define AUM_FCH_UART_INIT         0xB0010111
#define AUM_ESPI_EC_UART_INIT     0xB0010112
#define AUM_ESPI_CAP_DETECT_FAIL  0xB0010113

#define AUM_DEADLOOP() { volatile UINTN __iii; __iii = 1; while (__iii); }

// # Enable debug message (BIT[x] = 0 means disable serial out)
// # Bit 0: 1 - enable ESPI UART serail out, including UDC(unversial debug card) or EC ESIP UART
// # Bit 1: 1 - enable LPC UART serail out
// # Bit 2: 1 - enable port80 redirect serail out
// # Bit 3: 1 - enable FCH UART0 serail out
// # Bit 4: 1 - enable FCH UART1 serail out
// # Bit 5: 1 - enable FCH UART2 serail out
// # Bit 6: 1 - enable FCH UART3 serail out
// # Bit 7: 1 - enable FCH UART4 serail out
#define PCD_AUM_ESPI_UART         BIT0
#define PCD_AUM_LPC_UART          BIT1
#define PCD_AUM_PORT80            BIT2
#define PCD_AUM_FCH_UART0         BIT3
#define PCD_AUM_FCH_UART1         BIT4
#define PCD_AUM_FCH_UART2         BIT5
#define PCD_AUM_FCH_UART3         BIT6
#define PCD_AUM_FCH_UART4         BIT7

#define R_UART_TXBUF              0
#define R_UART_FCR                2
#define   B_UART_FCR_FIFOE        BIT0
#define   B_UART_FCR_FIFO64       BIT5
#define R_UART_LSR                5
#define   B_UART_LSR_TXRDY        BIT5
#define   B_UART_LSR_TEMT         BIT6
#define R_UART_MSR                6
#define   B_UART_MSR_CTS          BIT4

#define ACPI_MMIO_BASE            0xFED80000ul
#define IOMUX_BASE                0xD00
#define FCH_PMIOA_REG74           0x74
#define PMIO_BASE                 0x300
#define GPIO70                    0x1618

typedef BOOLEAN (*PF_AMD_UNIFY_MESSAGE_SUPPORT) (VOID);
typedef RETURN_STATUS (*PF_AMD_UNIFY_MESSAGE_INITIALIZE) (VOID);
typedef UINTN (*PF_AMD_UNIFY_MESSAGE_WRITE) (UINT8 *Buffer, UINTN NumberOfBytes);
typedef UINTN (*PF_AMD_UNIFY_MESSAGE_READ) (UINT8 *Buffer, UINTN NumberOfBytes);
typedef BOOLEAN (*PF_AMD_UNIFY_MESSAGE_POLL) (VOID);
typedef RETURN_STATUS (*PF_AMD_UNIFY_MESSAGE_SETCONTROL) (UINT32 Control);
typedef RETURN_STATUS (*PF_AMD_UNIFY_MESSAGE_GETCONTROL) (UINT32 *Control);
typedef RETURN_STATUS (*PF_AMD_UNIFY_MESSAGE_SETATTRIBUTES) (UINT64 *BaudRate, UINT32 *ReceiveFifoDepth, UINT32 *Timeout, EFI_PARITY_TYPE *Parity, UINT8 *DataBits, EFI_STOP_BITS_TYPE *StopBits);

typedef struct _AMD_UNIFY_MESSAGE_INSTANCE {
    PF_AMD_UNIFY_MESSAGE_SUPPORT          Support;
    // Standard SerialPortLib supported functions
    PF_AMD_UNIFY_MESSAGE_INITIALIZE       Initialize;
    PF_AMD_UNIFY_MESSAGE_WRITE            Write;
    PF_AMD_UNIFY_MESSAGE_READ             Read;
    PF_AMD_UNIFY_MESSAGE_POLL             Poll;
    PF_AMD_UNIFY_MESSAGE_SETCONTROL       SetControl;
    PF_AMD_UNIFY_MESSAGE_GETCONTROL       GetControl;
    PF_AMD_UNIFY_MESSAGE_SETATTRIBUTES    SetAttributes;
} AMD_UNIFY_MESSAGE_INSTANCE;

typedef enum {
    AMD_UNIFY_MESSAGE_SUPPORT = 0,
    // Standard SerialPortLib supported functions
    AMD_UNIFY_MESSAGE_INITIALIZE,
    AMD_UNIFY_MESSAGE_WRITE,
    AMD_UNIFY_MESSAGE_READ,
    AMD_UNIFY_MESSAGE_POLL,
    AMD_UNIFY_MESSAGE_SETCONTROL,
    AMD_UNIFY_MESSAGE_GETCONTROL,
    AMD_UNIFY_MESSAGE_SETATTRIBUTES
} MESSAGEID;

#endif
