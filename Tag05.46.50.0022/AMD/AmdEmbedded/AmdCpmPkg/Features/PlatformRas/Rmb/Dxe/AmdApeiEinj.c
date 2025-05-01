/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AmdPlatformRasRmbDxe.h"
#include "AmdApeiEinj.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/AmdPspMboxLibV2.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                           G L O B A L   V A R I A B L E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define MAX_RETRY_COUNT            (0xFFFFFFFFL)
#define PSP_EINJ_SUPPORTED_ERROR_TYPE  \
  (EFI_ACPI_6_3_EINJ_ERROR_MEMORY_CORRECTABLE | EFI_ACPI_6_3_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL | EFI_ACPI_6_3_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL)

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern  EFI_BOOT_SERVICES       *gBS;


/*----------------------------------------------------------------------------------------
 *                          T A B L E    D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */
APEI_EINJ_ACPI_TABLE mEinjTable = {

  {
    //AcpiHeader
    {
      EFI_ACPI_6_3_ERROR_INJECTION_TABLE_SIGNATURE,
      sizeof (APEI_EINJ_ACPI_TABLE),
      ACPI_REV1,
      0,
      {'A','M','D',' ',' ',' '},
      AMD_EINJ_TABLE_ID,
      1,
      AMD_CREATOR_ID,
      1
    },
    //EINJ Serialization Header
    sizeof (EFI_ACPI_6_3_ERROR_INJECTION_TABLE_HEADER), // Injection Header Size
    EINJ_RESERVED,                                      // Injection Flags = RESERVED
    {
      EINJ_RESERVED,                                    // RESERVED
      EINJ_RESERVED,                                    // RESERVED
      EINJ_RESERVED                                     // RESERVED
    },
    EINJ_INSTR_ENTRIES                                  // EINJ_INSTR_ENTRIES

  },
  {
    //EinjBeginInject
    {
      EFI_ACPI_6_3_EINJ_BEGIN_INJECTION_OPERATION,      // Injection Action
      EFI_ACPI_6_3_EINJ_WRITE_REGISTER_VALUE,           // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_IO,                         // AddressSpaceID = System IO
        REG_BIT_WIDTH_BYTE,                             // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_BYTE,                              // Access Size
        0                                               // Address, SwSmiCmdPort address update in runtime
      },
      0,                                                // Value, SwSmiCmd data update in runtime
      EINJ_BYTE_MASK                                    // Mask
    },

    //EinjGetTriggerErrActionTable
    {
      EFI_ACPI_6_3_EINJ_GET_TRIGGER_ERROR_ACTION_TABLE, // Injection Action
      EFI_ACPI_6_3_EINJ_READ_REGISTER,                  // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // Access Size
        0                                               // Address
      },
      0,                                                // Value
      EINJ_QWORD_MASK                                   // Mask
    },

    //EinjSetErrorType
    {
      EFI_ACPI_6_3_EINJ_SET_ERROR_TYPE,                 // Injection Action
      EFI_ACPI_6_3_EINJ_WRITE_REGISTER,                 // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // Access Size
        0                                               // Address
      },
      0,                                                // Value
      EINJ_QWORD_MASK                                   // Mask
    },

    //EinjGetErrorType
    {
      EFI_ACPI_6_3_EINJ_GET_ERROR_TYPE,                 // Injection Action
      EFI_ACPI_6_3_EINJ_READ_REGISTER,                  // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // Access Size
        0                                               // Address
      },
      0,                                                // Value
      EINJ_QWORD_MASK                                   // Mask
    },

    //EinjEndOperation
    {
      EFI_ACPI_6_3_EINJ_END_OPERATION,                  // Injection Action
      EFI_ACPI_6_3_EINJ_WRITE_REGISTER_VALUE,           // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_IO,                         // AddressSpaceID = System IO
        REG_BIT_WIDTH_BYTE,                             // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_BYTE,                              // Access Size
        0                                               // Address, SwSmiCmdPort address update in runtime
      },
      0,                                                // Value, SwSmiCmd data update in runtime
      EINJ_BYTE_MASK                                    // Mask
    },

    //EinjExecOperation
    {
      EFI_ACPI_6_3_EINJ_EXECUTE_OPERATION,              // Injection Action
      EFI_ACPI_6_3_EINJ_WRITE_REGISTER_VALUE,           // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_IO,                         // AddressSpaceID = System IO
        REG_BIT_WIDTH_BYTE,                             // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_BYTE,                              // Access Size
        0                                               // Address, SwSmiCmdPort address update in runtime
      },
      0,                                                // Value, SwSmiCmd data update in runtime
      EINJ_BYTE_MASK                                    // Mask
    },

    //EinjCheckBusyStatus
    {
      EFI_ACPI_6_3_EINJ_CHECK_BUSY_STATUS,              // Injection Action
      EFI_ACPI_6_3_EINJ_READ_REGISTER_VALUE,            // Instruction
      EINJ_CLEAR_FLAG,                                  // Flags
      EINJ_RESERVED,                                    // Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // Access Size
        0                                               // Address
      },
      0x01,                                             // Value
      EINJ_CHK_BUSY_STS_MASK                            // Mask
    },

    //EinjGetCommandStatus
    {
      EFI_ACPI_6_3_EINJ_GET_COMMAND_STATUS,             // UINT8 SerializationAction - Injection Action
      EFI_ACPI_6_3_EINJ_READ_REGISTER,                  // UINT8 Instruction
      EINJ_CLEAR_FLAG,                                  // UINT8 Flags
      EINJ_RESERVED,                                    // UINT8 Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // Access Size
        0                                               // UINT64 Address
      },
      0,                                                // UINT64 Value
      EINJ_GET_CMD_STS_MASK                             // UINT64 Mask
    },

    //EinjSetErrorTypeWithAddress
    {
      EINJ_SET_ERROR_TYPE_WITH_ADDRESS,                 // UINT8 SerializationAction - Injection Action
      EFI_ACPI_6_3_EINJ_WRITE_REGISTER,                 // UINT8 Instruction
      EINJ_CLEAR_FLAG,                                  // UINT8 Flags
      EINJ_RESERVED,                                    // UINT8 Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // Register Bit Width
        REG_BIT_OFFSET0,                                // Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // Access Size
        0                                               // UINT64 Address
      },
      0,                                                // UINT64 Value
      EINJ_QWORD_MASK                                   // UINT64 Mask
    }
  }
};

// Trigger Error Action Table
TRIG_ERROR_ACTION  mTrigErrActTable = \
{
  {
    sizeof (EFI_ACPI_6_3_EINJ_TRIGGER_ACTION_TABLE),    // UINT32 HeaderSize
    0,                                                  // UINT32 Revsion
    sizeof (TRIG_ERROR_ACTION),                         // UINT32 TableSize
    EINJ_TRIG_INSTR_ENTRIES                             // UINT32 Instruction EntryCount
  },
  {
    { //APEI_SERIALIZATION_INST_ENTRY[0]
      EFI_ACPI_6_3_EINJ_TRIGGER_ERROR,                  // UINT8 SerializationAction - Injection Action
      EFI_ACPI_6_3_EINJ_WRITE_REGISTER_VALUE,           // UINT8 Instruction
      EINJ_SET_FLAG,                                    // UINT8 Flags, 1 = PRESERVE_REGISTER
      EINJ_RESERVED,                                    // UINT8 Reserved
      { // Generic Address Region
        EFI_ACPI_6_3_SYSTEM_MEMORY,                     // UINT8 AddressSpaceID = System Memory
        REG_BIT_WIDTH_QWORD,                            // UINT8 Register Bit Width
        REG_BIT_OFFSET0,                                // UINT8 Register Bit Offset
        EFI_ACPI_6_3_QWORD,                             // UINT8 Access Size
        0x100000000                                     // UINT64 Address
      },
      0,                                                // UINT64 Value
      0                                                 // UINT64 Mask
    }
  }
};


BOOLEAN
GetPspRasMboxRegRegLocation (
  IN       UINT8            DfNode,
  IN OUT   PSP_RAS_MBOX_REG **PspRasMboxReg
  )
{
  EFI_STATUS        Status;
  UINT8             NbioBusNum;
  UINT32            PspMmioBase;

  Status = AcquireNbioSecBusNum(DfNode, 0, &NbioBusNum);  //Param1: 0 = Socket0, 1= Socket1; Param2: 0 = NBIO0;
  if (EFI_ERROR(Status)) {
    return FALSE;
  }

  PspMmioBase = 0;
  RasSmnRead (NbioBusNum, IOHC_MSIC0_SMN_BASE + IOHC_MSICX_PSP_BASE_ADDR_LO_OFFSET, &PspMmioBase);  //PspMmioBase is currently under 4G
  //Mask out the lower bits
  PspMmioBase &= 0xFFF00000;
  if (PspMmioBase == 0) {
    return FALSE;
  }
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] PSP_BASE_ADDR_LO: 0x%08x\n", (UINT32)PspMmioBase));

  *PspRasMboxReg = (PSP_RAS_MBOX_REG *)(UINTN)(PspMmioBase + C2PMSG_0_BASE + RAS_PSP_MAILBOX_OFFSET);

  return TRUE;
}

EFI_STATUS
ExecutePspRasEinjCmd (
  IN      PSP_RAS_MBOX_REG  *PspRasMboxReg,
  IN OUT  PSP_RAS_EINJ_CMD  *PspRasEinjCmd
  )
{
  UINT32  RetryCnt;
  PSP_RAS_MBOX_REG  PspRasMboxRegOp;

  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] %a-Entry PspRasMboxReg->Value: 0x%08x\n", __FUNCTION__, PspRasMboxReg->Value));

  PspRasMboxRegOp.Value = PspRasMboxReg->Value;

  PspRasMboxRegOp.Field.StatOrDta = 0;
  if (PspRasEinjCmd->CmdId == CMD_ID_EXECUTE_COMMAND_DIRECT) {
    PspRasMboxRegOp.Field.StatOrDta = PspRasEinjCmd->StatOrDta;
  }
  PspRasMboxRegOp.Field.CommandId = PspRasEinjCmd->CmdId;
  PspRasMboxRegOp.Field.AltStat = 0;
  PspRasMboxRegOp.Field.CmdOrRspns = 0;

  PspRasMboxReg->Value = PspRasMboxRegOp.Value;

  RetryCnt = 0;
  while ((PspRasMboxReg->Field.CmdOrRspns != 1) && (RetryCnt++ < MAX_RETRY_COUNT)) {
    NanoSecondDelay(10);     //10 ns
  }
  if ((PspRasMboxReg->Field.CmdOrRspns == 0) && (RetryCnt > MAX_RETRY_COUNT)) {
    DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] Timeout when waiting for PSP done\n"));
    return EFI_TIMEOUT;
  }

  PspRasEinjCmd->StatOrDta = (UINT16)PspRasMboxReg->Field.StatOrDta;
  PspRasEinjCmd->AltStat = (UINT16)PspRasMboxReg->Field.AltStat;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] %a-Exit PspRasMboxReg->Value: 0x%08x, RetryCnt = 0x%08x.\n",
    __FUNCTION__, PspRasMboxReg->Value, RetryCnt));

  return EFI_SUCCESS;
}

EFI_STATUS
AdjustInjectionInstEntries ( VOID )
{
  EFI_STATUS                    Status;
  PSP_RAS_MBOX_REG              *PspRasMboxReg;
  PSP_RAS_MBOX_REG              PspRasMboxRegData;
  PSP_RAS_EINJ_CMD              PspRasEinjCmd;
  UINT8                         DfNode;
  UINT8                         Index;
  UINT64                        PspCommandBuffer;
  PSP_COMMAND_BUFFER_STRUCTURE  *PspCmdBuff;

  DfNode = 0; //0: Socket0
  PspRasMboxReg = NULL;
  if (!GetPspRasMboxRegRegLocation (DfNode, &PspRasMboxReg)) {
    return EFI_NOT_FOUND;
  }

  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] MP0_C2PMSG_31 Register Address: 0x%08x\n", (UINTN)PspRasMboxReg));

  PspCommandBuffer = 0;
  for (Index = 0; Index <= CMD_ID_GET_CMD_BUF_ADDR_BITS_48_63; Index++) {
    PspRasEinjCmd.CmdId = Index;
    PspRasEinjCmd.StatOrDta = 0;
    PspRasEinjCmd.AltStat = 0;
    Status = ExecutePspRasEinjCmd (PspRasMboxReg, &PspRasEinjCmd);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] ExecutePspRasEinjCmd: %r\n", Status));
      return Status;
    }
    PspCommandBuffer |= ((UINT64)PspRasEinjCmd.StatOrDta << (16 * Index));
  }

  PspCmdBuff = (PSP_COMMAND_BUFFER_STRUCTURE *)(UINTN)PspCommandBuffer;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] PSP Command Buffer@ 0x%lx\n", (UINT64)(UINTN)PspCmdBuff));
  if ((PspCommandBuffer == 0) || (PspCommandBuffer == -1)) {
    return EFI_NOT_FOUND;
  }

  //Entry for Action BEGIN_INJECTION_OPERATION   (0x00): EinjBeginInject.
  mEinjTable.InjectionInstEntry[0].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[0].RegisterRegion.RegisterBitWidth = REG_BIT_WIDTH_DWORD;
  mEinjTable.InjectionInstEntry[0].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[0].RegisterRegion.Address          = (UINT64)((UINTN)&PspCmdBuff->PspCmdBuffOffset16);
  mEinjTable.InjectionInstEntry[0].Value                           = 1; //(sets BUSY flag read via CHECK_BUSY_STATUS)
  mEinjTable.InjectionInstEntry[0].Mask                            = EINJ_DWORD_MASK;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] ChkBsyStatRgn: 0x%lx\n", mEinjTable.InjectionInstEntry[0].RegisterRegion.Address));

  //Entry for Action GET_TRIGGER_ACTION_TABLE    (0x01): EinjGetTriggerErrActionTable.
  mEinjTable.InjectionInstEntry[1].RegisterRegion.Address = (UINT64)((UINTN)&PspCmdBuff->GetTrigActTblRgn);
  //Copy local Trigger Error Action table to [PSP Command Buffer + 128: Trigger Action Table]
  gBS->CopyMem (&PspCmdBuff->TrigActTbl, &mTrigErrActTable, sizeof (TRIG_ERROR_ACTION));
  //PspCmdBuff->TrigActTbl.TrigErrorInstruction[0].Instruction = EFI_ACPI_6_3_EINJ_READ_REGISTER_VALUE;
  //PspCmdBuff->TrigActTbl.TrigErrorInstruction[0].Flags = EINJ_CLEAR_FLAG;
  PspCmdBuff->TrigActTbl.TrigErrorInstruction[0].RegisterRegion.Address = (UINT64)((UINTN)&PspCmdBuff->TrigErrRgn);
  PspCmdBuff->TrigActTbl.TrigErrorInstruction[0].Value = 1; //(sets TRIGGER_START Flag, consumed by PSP)
  PspCmdBuff->TrigActTbl.TrigErrorInstruction[0].Mask= EINJ_DWORD_MASK;
  PspCmdBuff->GetTrigActTblRgn = (UINT64)((UINTN)&PspCmdBuff->TrigActTbl);
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] GetTrigActTblRgn: 0x%lx, TblAddress@ 0x%lx\n", mEinjTable.InjectionInstEntry[1].RegisterRegion.Address, PspCmdBuff->GetTrigActTblRgn));
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] +++=== TrigActTbl ===+++\n"));
  for (Index = 0; Index < sizeof (TRIG_ERROR_ACTION); Index++) {
    DEBUG ((EFI_D_ERROR, "%02x ", *(((UINT8 *)&PspCmdBuff->TrigActTbl) + Index)));
  if ((Index+1) % 16 == 0) {
      DEBUG ((EFI_D_ERROR, "\n"));
  }
  }
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] ---=== TrigActTbl ===---\n"));

  //Entry for Action SET_ERROR_TYPE              (0x02): EinjSetErrorType
  mEinjTable.InjectionInstEntry[2].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[2].RegisterRegion.RegisterBitWidth = REG_BIT_WIDTH_DWORD;
  mEinjTable.InjectionInstEntry[2].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[2].RegisterRegion.Address = (UINT64)((UINTN)&PspCmdBuff->SetErrTypRgn);
  mEinjTable.InjectionInstEntry[2].Mask                            = EINJ_DWORD_MASK;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] SetErrTypRgn: 0x%lx\n", mEinjTable.InjectionInstEntry[2].RegisterRegion.Address));

  //Entry for Action GET_ERROR_TYPE              (0x03): EinjGetErrorType
  mEinjTable.InjectionInstEntry[3].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[3].RegisterRegion.RegisterBitWidth = REG_BIT_WIDTH_DWORD;
  mEinjTable.InjectionInstEntry[3].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[3].RegisterRegion.Address          = (UINT64)((UINTN)&PspCmdBuff->GetErrTypRgn);
  mEinjTable.InjectionInstEntry[3].Mask                            = EINJ_DWORD_MASK;
  PspCmdBuff->GetErrTypRgn = (UINT32)PSP_EINJ_SUPPORTED_ERROR_TYPE;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] GetErrTypRgn: 0x%lx, *GetErrTypRgn: 0x%08x\n", mEinjTable.InjectionInstEntry[3].RegisterRegion.Address, PspCmdBuff->GetErrTypRgn));

  //Entry for Action END_OPERATION               (0x04): EinjEndOperation
  mEinjTable.InjectionInstEntry[4].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[4].RegisterRegion.RegisterBitWidth = REG_BIT_WIDTH_DWORD;
  mEinjTable.InjectionInstEntry[4].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[4].RegisterRegion.Address          = (UINT64)((UINTN)&PspCmdBuff->EndOpnRgn);
  mEinjTable.InjectionInstEntry[4].Value                           = 1; //sets TRIGGER_END flag read by PSP
  mEinjTable.InjectionInstEntry[4].Mask                            = EINJ_DWORD_MASK;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] EndOpnRgn: 0x%lx\n", mEinjTable.InjectionInstEntry[4].RegisterRegion.Address));

  //Entry for Action EXECUTE_OPERATION           (0x05): EinjExecOperation
  PspRasMboxRegData.Value = 0;
  PspRasMboxRegData.Field.CommandId = CMD_ID_EXECUTE_COMMAND_DIRECT;
  PspRasMboxRegData.Field.StatOrDta = 0x83; //EINJ_EXECUTE_CODE
  mEinjTable.InjectionInstEntry[5].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[5].RegisterRegion.RegisterBitWidth = REG_BIT_WIDTH_DWORD;
  mEinjTable.InjectionInstEntry[5].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[5].RegisterRegion.Address          = (UINT64)(UINTN)PspRasMboxReg;
  mEinjTable.InjectionInstEntry[5].Value                           = PspRasMboxRegData.Value;
  mEinjTable.InjectionInstEntry[5].Mask                            = EINJ_DWORD_MASK;

  //Entry for Action CHECK_BUSY_STATUS           (0x06): EinjCheckBusyStatus
  mEinjTable.InjectionInstEntry[6].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[6].RegisterRegion.RegisterBitWidth = REG_BIT_WIDTH_DWORD;
  mEinjTable.InjectionInstEntry[6].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[6].RegisterRegion.Address          = (UINT64)((UINTN)&PspCmdBuff->PspCmdBuffOffset16);

  //Entry for Action GET_COMMAND_STATUS          (0x07): EinjGetCommandStatus
  mEinjTable.InjectionInstEntry[7].RegisterRegion.AddressSpaceId   = EFI_ACPI_6_3_SYSTEM_MEMORY;
  mEinjTable.InjectionInstEntry[7].RegisterRegion.RegisterBitWidth = (REG_BIT_WIDTH_DWORD - 1);
  mEinjTable.InjectionInstEntry[7].RegisterRegion.RegisterBitOffset = REG_BIT_OFFSET1;  //1
  mEinjTable.InjectionInstEntry[7].RegisterRegion.AccessSize       = EFI_ACPI_6_3_DWORD;
  mEinjTable.InjectionInstEntry[7].RegisterRegion.Address          = (UINT64)((UINTN)&PspCmdBuff->PspCmdBuffOffset16);
  mEinjTable.InjectionInstEntry[7].Mask                            = EINJ_DWORD_MASK;
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] GetCmdStatRgn: 0x%lx\n", mEinjTable.InjectionInstEntry[7].RegisterRegion.Address));

  //Entry for Action SET_ERROR_TYPE_WITH_ADDRESS (0x08): EinjSetErrorTypeWithAddress
  mEinjTable.InjectionInstEntry[8].RegisterRegion.Address = (UINT64)((UINTN)&PspCmdBuff->SetErrTypWtAddrRgn);
  ZeroMem ((UINT8 *)((UINTN)&PspCmdBuff->SetErrTypWtAddrRgn), sizeof (SET_ERR_TYPE_WITH_ADDR));
  DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] SetErrTypWtAddrRgn: 0x%lx\n", mEinjTable.InjectionInstEntry[8].RegisterRegion.Address));

  return EFI_SUCCESS;
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  ApeiEinjInit
 *
 *  Description:
 *     Initialize ACPI APEI EINJ table.
 *
 *  Parameters:
 *    @retval        EFI_SUCCESS
 *
 *---------------------------------------------------------------------------------------
 **
EFI_STATUS
ApeiEinjInstall ( VOID )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  APEI_EINJ_ACPI_TABLE  *ApeiEinjAcpiTable;
  UINTN                 TableKey;
  APEI_EINJ_REGISTERS   *EinjActMemRegAddr;
  UINT8                 *TrigErrorAct;
  SET_ERR_TYPE_WITH_ADDR *SetErrTypeWithAddr;

  if (PcdGet8 (PcdCpmRasEinjMode)) {      //PSP
    Status = AdjustInjectionInstEntries ();
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[RAS PSP EINJ] Unable to install EINJ table: %r \n", Status));
      return Status;
    }
  } else {                                 //BIOS
    //
    //Reserve EFI memory buffer for Einj serial instruction
    //
    Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (APEI_EINJ_REGISTERS), &EinjActMemRegAddr);
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    //
    //Reserve EFI memory buffer for Einj SET_ERROR_TYPE_WITH_ADDRESS action command structure
    //
    Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (SET_ERR_TYPE_WITH_ADDR), &SetErrTypeWithAddr);
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem (SetErrTypeWithAddr, sizeof (SET_ERR_TYPE_WITH_ADDR));
    EinjActMemRegAddr->EinjSetErrTypeWithAddrReg = (UINT64)(UINTN)SetErrTypeWithAddr;

    DEBUG ((EFI_D_ERROR, "[RAS]Set Error Type With Address structure locate: 0x%08x", RShiftU64(EinjActMemRegAddr->EinjSetErrTypeWithAddrReg, 32)));
    DEBUG ((EFI_D_ERROR, "%08x\n", EinjActMemRegAddr->EinjSetErrTypeWithAddrReg & 0xFFFFFFFF));

    mEinjTable.InjectionInstEntry[1].RegisterRegion.Address  = (UINT64) ((UINTN) &EinjActMemRegAddr->EinjGetTriActTblReg);
    mEinjTable.InjectionInstEntry[2].RegisterRegion.Address  = (UINT64) ((UINTN) &EinjActMemRegAddr->EinjSetErrTypeReg);
    mEinjTable.InjectionInstEntry[3].RegisterRegion.Address  = (UINT64) ((UINTN) &EinjActMemRegAddr->EinjGetErrTypeReg);
    mEinjTable.InjectionInstEntry[6].RegisterRegion.Address  = (UINT64) ((UINTN) &EinjActMemRegAddr->EinjChkBusyStsReg);
    mEinjTable.InjectionInstEntry[7].RegisterRegion.Address  = (UINT64) ((UINTN) &EinjActMemRegAddr->EinjCmdStsReg);
    mEinjTable.InjectionInstEntry[8].RegisterRegion.Address  = (UINT64) ((UINTN) EinjActMemRegAddr->EinjSetErrTypeWithAddrReg);

    //Update Einj Begin Injection Operation Software SMIcommand port address and command data
    mEinjTable.InjectionInstEntry[0].RegisterRegion.Address = mPlatformApeiPrivate->PlatRasPolicy.SwSmiCmdPortAddr;
    mEinjTable.InjectionInstEntry[0].Value = (UINT64)mPlatformApeiPrivate->PlatRasPolicy.EinjBeginInjOptSwSmiData;

    //Update Einj End Operation Software SMIcommand port address and command data
    mEinjTable.InjectionInstEntry[4].RegisterRegion.Address = mPlatformApeiPrivate->PlatRasPolicy.SwSmiCmdPortAddr;
    mEinjTable.InjectionInstEntry[4].Value = (UINT64)mPlatformApeiPrivate->PlatRasPolicy.EinjEndOptSwSmiData;

    //Update EinjExecOperation Software SMIcommand port address and command data
    mEinjTable.InjectionInstEntry[5].RegisterRegion.Address = mPlatformApeiPrivate->PlatRasPolicy.SwSmiCmdPortAddr;
    mEinjTable.InjectionInstEntry[5].Value = (UINT64)mPlatformApeiPrivate->PlatRasPolicy.EinjSwSmiData;

    // Set error injection capabilities bitmask for this platform
    EinjActMemRegAddr->EinjGetErrTypeReg = EFI_ACPI_6_3_EINJ_ERROR_MEMORY_CORRECTABLE | EFI_ACPI_6_3_EINJ_ERROR_MEMORY_UNCORRECTABLE_NONFATAL| EFI_ACPI_6_3_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL;
    // EinjActMemRegAddr->EinjGetErrTypeReg = EFI_ACPI_6_3_EINJ_ERROR_MEMORY_CORRECTABLE | EFI_ACPI_6_3_EINJ_ERROR_MEMORY_UNCORRECTABLE_FATAL | EFI_ACPI_6_3_EINJ_ERROR_PLATFORM_CORRECTABLE | EFI_ACPI_6_3_EINJ_ERROR_PLATFORM_UNCORRECTABLE_FATAL;

    // Allocate ACPI NVS memory for Trigger Error Action Table
    Status = gBS->AllocatePool (EfiReservedMemoryType, sizeof (TRIG_ERROR_ACTION), (VOID **) &TrigErrorAct);
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }

    // Copy local Trigger Error Action table to ACPI NVS memory space
    gBS->CopyMem (TrigErrorAct, &mTrigErrActTable, sizeof (TRIG_ERROR_ACTION));

    //Update EINJ action interustion register buffer pointer,
    //Trigger Error Injection Action table pointer to APEI private buffer.
    mPlatformApeiPrivate->EinjActMemRegAddr = EinjActMemRegAddr;
    mPlatformApeiPrivate->TrigErrorAct = TrigErrorAct;
  }

  //Allocate EFI Boot Services memory space for EINJ table
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (APEI_EINJ_ACPI_TABLE), &ApeiEinjAcpiTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->CopyMem (&mEinjTable.InjectionHeader.Header.OemTableId, (UINT64 *)PcdGetPtr (PcdAmdAcpiEinjTableHeaderOemTableId), sizeof (UINT64));
  // Copy local EINJ table to EFI Boot Services memory buffer
  gBS->CopyMem (ApeiEinjAcpiTable, &mEinjTable, sizeof (APEI_EINJ_ACPI_TABLE));

  // Update EINJ header Checksum
  ChecksumAcpiTable ((EFI_ACPI_DESCRIPTION_HEADER *) ApeiEinjAcpiTable);

  TableKey = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                           AcpiTableProtocol,
                           ApeiEinjAcpiTable,
                           sizeof (APEI_EINJ_ACPI_TABLE),
                           &TableKey
                           );
  DEBUG ((EFI_D_ERROR, "[RAS EINJ] Install EINJ table: %r \n", Status));

  gBS->FreePool (ApeiEinjAcpiTable);

  return Status;
}*/
