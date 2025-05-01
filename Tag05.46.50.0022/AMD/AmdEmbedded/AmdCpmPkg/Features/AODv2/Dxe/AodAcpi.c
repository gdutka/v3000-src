/*****************************************************************************
 *
 * Copyright (C) 2017-2022 Advanced Micro Devices, Inc. All rights reserved.
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
#include "AodDxe.h"
AOD_MOUDLE_NAME ("AodAcpi")
UINTN             mNvsAddress = 0;
UINTN             mNvsSize = 0;
extern EFI_GUID gAodNvAreaUpdatedProtocolGuid;

///
///  To compatibe with ord ASL code define, we separate these commands
///  into two tables
///
UINT32 mOBIDList[0x40] = {
            0x00010001,     // Get APCB Config
            0x00010002,     // Get memory voltages
            0x00020001,     // Set memory clock frequency
            0x00020002,     // Set Tcl
            0x00020003,     // Set Trcdrd
            0x00020004,     // Set Trcdwr
            0x00020005,     // Set Tras
            0x00020006,     // Set Trp
            0x00020007,     // Set ProcODT
            0x00020008,     // Set TRCPAGE
            0x00020009,     // Set TRC
            0x0002000A,     // Set TRFC
            0x0002000B,     // Set TRFC2
            0x0002000C,     // Set TRFC4
            0x0002000D,     // Set TFAW
            0x0002000E,     // Set TRRDS
            0x0002000F,     // Set TRRDL
            0x00020010,     // Set TWR
            0x00020011,     // Set TWTRS
            0x00020012,     // Set TWTRL
            0x00020013,     // Set TCKE
            0x00020014,     // Set TCWL
            0x00020015,     // Set TRTP
            0x00020016,     // Set TRDRDSC
            0x00020017,     // Set TRDRDSCL
            0x00020018,     // Set TRDRDSD
            0x00020019,     // Set TRDRDDD
            0x0002001A,     // Set TWRWRSC
            0x0002001B,     // Set TWRWRSCL
            0x0002001C,     // Set TWRWRSD
            0x0002001D,     // Set TWRWRDD
            0x0002001E,     // Set TRDWR
            0x0002001F,     // Set TWRRD
            0x00020020,     // Set GEAR_DOWN_EN
            0x00020021,     // Set CMD2T
            0x00020022,     // Set RTTNOM
            0x00020023,     // Set RTTWR
            0x00020024,     // Set RTTPARK
            0x00020025,     // Set PowerDwonEn
            0x00020026,     // Set SMTEn
            0x00020027,     // Software Downcore Config
            0x00020028,     // Set EDC Throttler Control
            0x00020029,     // Set AddrCmdSetup
            0x0002002A,     // Set CsOdtSetup
            0x0002002B,     // Set CkeSetup
            0x0002002C,     // Set CadBusClkDrvStren
            0x0002002D,     // Set CadBusAddrCmdDrvStren
            0x0002002E,     // Set CadBusCsOdtCmdDrvStren
            0x0002002F,     // Set CadBusCkeDrvStren
            0x00020030,     // Set CLDO_VDDP
            0x00020031,     // Set MemInterleavingMode
            0x00020032,     // Set MemInterleavingSize
            0x00020033,     // Set FCLK OC Mode
            0x00020034,     // Set SOC VID
            0x00020035,     // Set FCLK Frequency
            0x00020036,     // Set CCLK Fmax
            0x00020037,     // Set GFXCLK Fmax
            0x00020038,     // Set CLDO_VDDG
            0x00020039,     // Get ECO Mode
            0x0002003A,     // Set ECO Mode
            0x00030001,     // Set VddIo
            0x00030002,     //
            0x00040001,     // Command Buffer Start
            0x00040002,     // Command Buffer End
    };
///
///  To compatibe with ord ASL code define, we separate these commands
///  into two tables
///

UINT32 mOBIEList[0x40] = {
          0x00050001,     // Set PPT Limit
          0x00050002,     // Set TDC Limit
          0x00050003,     // Set EDC Limit
          0x00050004,     // Set Scalar
          0x00050005,     // Set DRAM Map Inversion
          0x00050006,     // Set Gaming Mode
          0x00050007,     // Get Gaming Mode
          0x00050008,     // Set NPS Mode
          0x00050009,     // Get NPS Mode
          0x0005000A,     // Set Curve Optimizer
          0x0005000B,     // Set VDDG IOD
          0x0005000C,     // Set SET SOC TDC LIMMIT
          0x0005000D,     // Set SET SOC EDC LIMMIT
          0x0005000E,     // Set SET DRAM LATENCY ENHANCE
          0x0005000F,     // Get GET DRAM LATENCY ENHANCE
          0x0002003B,     // Set Trcd
          0x0002003C,     // Set Trfc1
          0x0002003D,     // Set TrfcSb
          0x0002003E,     // Set RttNomWr
          0x0002003F,     // Set RttNomRd
          0x00020040,     // Set RttParkDqs
          0x00020041,     // Set ProcDataDriveStrength
          0x00020042,     // Set DRAMDataDriveStrength
          0x00050010,     // Set GFX Curve Optimizer
          0x00010003,     // Get OC Disable
          0x00010004,     // Get OC Voltage Max
          0x00010005,     // Get OC Frequency Max
          0x00010006,     // Get GFX OC Voltage Max
          0x00010007,     // Get GFX OC Frequency Max
          0x00030004,     // Set APU VddIo
          0x00010008,     // Get Interlevaing Cap
          0x00030005,     // Set VDD_MEM
          0x00030003,     // Set VPP
          0x00020043,     // Set LCLK Freq
          0x00030006,     // Set VDD Misc
          0x00050011,     // Set CCD Freq Optimizer
          0x00020044,     // Set UCLK DIV1
          0x00020045,     // Set RMP Mode
          0x00020046,     // Set RMP Profile
          0x00020047,     // Set RMP PSTATE
          0x00030007,     // Set DIMM VDDQ
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
    };
/**
  This function allows a caller to check if current CmdId is in OBID or not

  @param[in]    CmdId            The Command Id of AOD WMI

  @retval TRUE if the CmdId is in OBID
  @retval FALSE the CmdId not in OBID
**/
BOOLEAN
EFIAPI
AodIsCmdInOBID (
  IN    UINT32         CmdId
  )
{
  UINTN   Index;
  for (Index = 0; Index < 0x40; Index ++) {
    if (CmdId == mOBIDList[Index]) return TRUE;
  }
  return FALSE;
}

/**
  This function allows a caller to check if current CmdId is in OBIE or not

  @param[in]    CmdId            The Command Id of AOD WMI

  @retval TRUE if the CmdId is in OBIE
  @retval FALSE the CmdId not in OBIE
**/
BOOLEAN
EFIAPI
AodIsCmdInOBIE (
  IN    UINT32         CmdId
  )
{
  UINTN   Index;
  for (Index = 0; Index < 0x40; Index ++) {
    if (CmdId == mOBIEList[Index]) return TRUE;
  }
  return FALSE;
}

/**
  This function used to calc the bytes occupied by package length
  Aml opcode length:
  LeadByte PkgLength data... LeadByte ...
            |                 |
            |____PkgLength____|
  @param[in]    DataBufferSize            The Buffer Size of all package data,euqal to
                                          (PkgLength-PkgLengthBytes)

  @retval the bytes occupied by PkgLength
**/
UINT8
AodCalcAmlPkgLengthBytes (
  IN  UINTN          DataBufferSize
  )
{
  if (DataBufferSize < 0x3F) return 1;
  if (DataBufferSize < 0xFFF) return 2;
  if (DataBufferSize < 0xFFFFF) return 3;
  if (DataBufferSize < 0xFFFFFFF) return 4;
  ASSERT (0);
  return 0;
}

/**
  This function used to calc the bytes occupied by BufferSize
  DefBuffer := BufferOp PkgLength BufferSize ByteList
  BufferOp := 0x11
  BufferSize := TermArg => Integer
  @param[in]    DataBufferSize            The Data Size of all bytelist

  @retval the bytes occupied by BufferSize
**/
UINT8
AodCalcAmlBufferSizeBytes (
  IN  UINTN          DataBufferSize
  )
{
  if (DataBufferSize < 0xff) return 2; // Byte
  if (DataBufferSize < 0xFFFF) return 3; // Word
  if (DataBufferSize < 0xFFFFFFFF) return 5; //DWord
  ASSERT (0);
  return 0;
}

/**
  This function used to append AML stream

  @param[in]    pStream            The AML stream pointer
  @param[in]    pData              The AML Data pointer
  @param[in]    Type               The AML stream append type
  @param[in]    DataSize           The AML Data Size, used when the data type if buffer

  @retval EFI_INVALID_PARAMETER when the input Type is unknown.
  @retval EFI_SUCCESS means append the AML stream successfully
**/
EFI_STATUS
EFIAPI
AodStreamAppend (
  IN OUT       ASTREAM         *pStream,
  IN           VOID            *pData,
  IN           APPEND_TYPE      Type,
  IN OPTIONAL  UINTN            DataSize
  )
{
  UINTN              AppendSize;
  UINT8              *pPtr;
  UINT8              LeadByte;
  UINT8              Type1;
  UINT32             PkgLen;
  switch (Type) {
    case APPEND_AMLONE:
    case APPEND_AMLZERO:
    case APPEND_BYTE:
      AppendSize = 1;
      break;
    case APPEND_WORD:
      AppendSize = 2;
      break;
    case APPEND_AMLNAMEDATA:
    case APPEND_DWORD:
      AppendSize = 4;
      break;
    case APPEND_BUFFER:
      AppendSize = DataSize;
      break;
    case APPEND_AMLBYTE:
      AppendSize = 2;
      break;
    case APPEND_AMLWORD:
      AppendSize = 3;
      break;
    case APPEND_AMLDWORD:
      AppendSize = 5;
      break;
    case APPEND_AMLSTRING:
      if (pData == NULL) {
        AppendSize = 2 ;
      } else {
        AppendSize = 1 + AsciiStrSize ((CHAR8 *)pData);
      }
      break;
    case APPEND_AMLPKGLEN:
      AppendSize = AodCalcAmlPkgLengthBytes (*(UINT32 *)pData);
      break;
    case APPEND_AMLBUFSIZE:
      AppendSize = AodCalcAmlBufferSizeBytes (*(UINT32 *)pData);
      break;
    default:
      AOD_DEBUG ((EFI_D_ERROR, "Unkown AML Stream type %x\n", Type));
      return EFI_INVALID_PARAMETER;
  }
  if ((pStream->Length + AppendSize) > pStream->MaxLength) {
    return EFI_OUT_OF_RESOURCES;
  }
  pPtr = pStream->pPtr + pStream->MaxLength - pStream->Length - AppendSize;

  Type1 = Type;
  if (Type == APPEND_AMLBUFSIZE) {
    switch (AppendSize) {
      case 2:
        Type1 = APPEND_AMLBYTE;
        break;
      case 3:
        Type1 = APPEND_AMLWORD;
        break;
      case 5:
        Type1 = APPEND_AMLDWORD;
        break;
      default:
        ASSERT(0);
        break;
    }
  }
  switch (Type1) {
    case APPEND_AMLONE:
      *pPtr = 1;
      break;
    case APPEND_AMLZERO:
      *pPtr = 0;
      break;
    case APPEND_BYTE:
      *pPtr = *(UINT8 *)pData;
      break;
    case APPEND_WORD:
      *(UINT16 *)pPtr = *(UINT16 *)pData;
      break;
    case APPEND_AMLNAMEDATA:
    case APPEND_DWORD:
      *(UINT32 *)pPtr = *(UINT32 *)pData;
      break;
    case APPEND_BUFFER:
      CopyMem (pPtr, pData, DataSize);
      break;
    case APPEND_AMLBYTE:
      //ByteConst := BytePrefix ByteData
      //BytePrefix := 0x0A
      *pPtr = 0x0A;
      *(pPtr + 1) = *(UINT8 *)pData;
      break;
    case APPEND_AMLWORD:
      //WordConst := WordPrefix WordData
      //WordPrefix := 0x0B
      *pPtr = 0x0B;
      *(UINT16 *)(pPtr + 1) = *(UINT16 *)pData;
      break;
    case APPEND_AMLDWORD:
      //DWordConst := DWordPrefix DWordData
      //DWordPrefix := 0x0C
      *pPtr = 0x0C;
      *(UINT32 *)(pPtr + 1) = *(UINT32 *)pData;
      break;
    case APPEND_AMLSTRING:
      //String := StringPrefix AsciiCharList NullChar
      //StringPrefix := 0x0D
      *pPtr = 0x0D;
      if (AppendSize == 2) {
        *(pPtr+1) = 0;
      } else {
        CopyMem (pPtr+1, pData, AppendSize-1);
      }
      break;
    case APPEND_AMLPKGLEN:
      *(UINT32 *)pData = *(UINT32 *)pData + (UINT32)AppendSize;
      if (AppendSize == 1) {
        *pPtr = *(UINT8 *)pData;
      } else {
        LeadByte = (UINT8)((AppendSize - 1) << 6);
        LeadByte = (UINT8)(LeadByte | (*(UINT8 *)pData & 0xF));
        *pPtr = LeadByte;
        PkgLen = *(UINT32 *)pData;
        PkgLen = (PkgLen >> 4);
        CopyMem (pPtr+1, (UINT8 *)&PkgLen, AppendSize-1);
      }
      break;
    default:
      // should not reached here
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
      return EFI_INVALID_PARAMETER;
  }
  pStream->Length = pStream->Length + AppendSize;

  return EFI_SUCCESS;
}

/**
  This function used to Generate AOD OBIT table from CmdList
  WMI:
class GetMinMaxSteppingPackage
{
    [WmiDataId(1),
        read, write,
        Description("ID")
        ] uint32 Value;
        [WmiDataId(2),
         read, write,
         Description("Current")
        ] uint32 Current;
        [WmiDataId(3),
         read, write,
         Description("Min")
        ] uint32 Min;
        [WmiDataId(4),
         read, write,
         Description("Max")
        ] uint32 Max;
        [WmiDataId(5),
         read, write,
         Description("Stepping")
        ] uint32 Stepping;
};
class GetdValuesPackage
{
 [WmiDataId(1),
     read, write,
     Description("ID")
    ] uint32 ID;
 [WmiDataId(2),
    read, write,
    Description("Current")
    ] uint32 Current;
 [WmiDataId(3),
     read, write,
     Description("Number of ID")
    ] uint32 Length;
 [WmiDataId(4),
     read, write,
     Description("100*4 bytes of data")
    ] uint32 DValuesBuffer[100];
};
  AML Ex:
  Name(OBIT, Package(38)
  {
    0x25,
    Package(4)
    {
      0x00020007, //WMI command ID reference
      0x00000000, //WMI Command Property
      0x00000000, //WMI Runtime NVS offset
      Buffer(0x019c) //GetdValuesPackage
      {
        0x07, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00,
        0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x3F, 0x00, 0x00, 0x00
      }
    },
    Package(4)
    {
      0x00020024, //WMI command ID reference
      0x00000001, //WMI Command Property
      0x00000000, //WMI Runtime NVS offset
      Buffer(0x14) //GetMinMaxSteppingPackage
      {
        0x24, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x00,
        0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
      }
    },
    ....
  }
  @param[in]    pInstance            The private data of AOD Dxe instance
  @param[in]    pStream              The AML Stream


  @retval EFI_INVALID_PARAMETER when the Cmd List is empty
  @retval EFI_SUCCESS means generate OBIT AML successfully and OBIT AML will
                      store at AML Stream
**/
EFI_STATUS
EFIAPI
AodGenOBIT (
  IN     AOD_DXE_PRIVATE_DATA       *pInstance,
  IN     ASTREAM                    *pStream
  )
{
  EFI_STATUS                                   Status;
  UINT8                                        Count = 0;
  UINT32                                       BufferSize = 0;
  UINT32                                       PkgLength = 0;
  UINT32                                       PkgLength1 = 0;
  UINT32                                       InputLength = 0;
  UINT8                                        Data8 = 0;
  UINT32                                       Data32 = 0;
  LIST_ENTRY                                   *Link;
  AOD_CMD                                      *pCmd;

  InputLength = (UINT32)pStream->Length;
  //
  // 0 Build CmdPkg
  //
  if (IsListEmpty (&pInstance->CmdList)) {
    AOD_DEBUG ((EFI_D_ERROR, "There is no cmd registered\n"));
    return EFI_INVALID_PARAMETER;
  }
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (pCmd->DataCount != 0) {

      //DefBuffer := BufferOp PkgLength BufferSize ByteList
      //BufferOp := 0x11
      PkgLength = (UINT32)pStream->Length;
      PkgLength1 = (UINT32)pStream->Length;
      //1.0 ByteList.Data u32 MAX, U32 MIN, U32 SETP
      Status = AodStreamAppend (pStream, (VOID *)pCmd->pValue, APPEND_BUFFER, pCmd->DataCount*sizeof(UINT32));
      if (EFI_ERROR (Status)) return Status;

      //1.1 ByteList.Length U32 length
      if (pCmd->Property == 0) {
        Status = AodStreamAppend (pStream, (VOID *)&pCmd->DataCount, APPEND_DWORD, 0);
        if (EFI_ERROR (Status)) return Status;
      }
      //1.2 Update Current
      Status = AodStreamAppend (pStream, (VOID *)&pCmd->Current, APPEND_DWORD, 0);
      if (EFI_ERROR (Status)) return Status;
      //1.3 ByteList.CmdId
      Status = AodStreamAppend (pStream, (VOID *)&pCmd->CmdId, APPEND_DWORD, 0);
      if (EFI_ERROR (Status)) return Status;

      //2.BufferSize
      BufferSize = (pCmd->Property)?20:(103*4);
      Status = AodStreamAppend (pStream, (VOID *)&BufferSize, APPEND_AMLBUFSIZE, 0);
      if (EFI_ERROR (Status)) return Status;

      //3. Package Length
      PkgLength = (UINT32)pStream->Length - PkgLength;
      Status = AodStreamAppend (pStream, (VOID *)&PkgLength, APPEND_AMLPKGLEN, 0);
      if (EFI_ERROR (Status)) return Status;

      //4. Buffer Opcode
      Data8 = 0x11;
      Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
      if (EFI_ERROR (Status)) return Status;

      //5. cmdid runtime NVS Index
      Status = AodStreamAppend (pStream, (VOID *)&pCmd->CurrentNvsIndex, APPEND_AMLBYTE, 0);
      if (EFI_ERROR (Status)) return Status;

      //6. cmdid Property
      if (pCmd->Property == 1) {
        Status = AodStreamAppend (pStream, (VOID *)&pCmd->CmdId, APPEND_AMLONE, 0);
      } else {
        Status = AodStreamAppend (pStream, (VOID *)&pCmd->CmdId, APPEND_AMLZERO, 0);
      }
      if (EFI_ERROR (Status)) return Status;

      //7. cmdid
      Status = AodStreamAppend (pStream, (VOID *)&pCmd->CmdId, APPEND_AMLDWORD, 0);
      if (EFI_ERROR (Status)) return Status;

      //8. NumElements
      Data8 = 0x4;
      Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
      if (EFI_ERROR (Status)) return Status;

      //9. Package Length
      PkgLength1 = (UINT32)pStream->Length - PkgLength1;
      Status = AodStreamAppend (pStream, (VOID *)&PkgLength1, APPEND_AMLPKGLEN, 0);
      if (EFI_ERROR (Status)) return Status;

      //10. Package Opcode
      Data8 = 0x12;
      Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
      if (EFI_ERROR (Status)) return Status;
      Count ++;
    }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }

  //1. Count
  Data8 = Count;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_AMLBYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //2. NumElements
  Data8 = Count + 1;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //3. Package Length
  PkgLength = (UINT32)pStream->Length - InputLength;
  Status = AodStreamAppend (pStream, (VOID *)&PkgLength, APPEND_AMLPKGLEN, 0);
  if (EFI_ERROR (Status)) return Status;

  //4. Package Opcode
  Data8 = 0x12;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //5. Name OBIT
  Data32 = SIGNATURE_32('O', 'B', 'I', 'T');
  Status = AodStreamAppend (pStream, (VOID *)&Data32, APPEND_DWORD, 0);
  if (EFI_ERROR (Status)) return Status;

  //6. Package Opcode
  Data8 = 0x8;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  return EFI_SUCCESS;
}
/**
  This function used to Generate AOD OBID table from CmdList
  class OIDPackage
  {
  [WmiDataId(1),
  read, write,
  Description("Number of ID")
  ] uint8 Length;
  [WmiDataId(2),
  read, write,
  Description("64*4 bytes of data")
  ] uint32 ID[64];
  [WmiDataId(3),
  read, write,
  Description("64*4 bytes of data")
  ] uint32 property[64];
  [WmiDataId(4),
  read, write,
  Description("64 strings")
  ] string IDstring[64];
  };
  Name(OBID, Package(193) {
    0x40,
    "ID"[64],
    "Property"[64],
    "String"[64],
  }
  @param[in]    pInstance            The private data of AOD Dxe instance
  @param[in]    pStream              The AML Stream


  @retval EFI_INVALID_PARAMETER when the Cmd List is empty
  @retval EFI_SUCCESS means generate OBIT AML successfully and OBIT AML will
                      store at AML Stream
**/
EFI_STATUS
EFIAPI
AodGenOBID (
  IN     AOD_DXE_PRIVATE_DATA       *pInstance,
  IN     ASTREAM                    *pStream
  )
{
  EFI_STATUS                                   Status;
  UINT32                                       PkgLength = 0;
  UINT32                                       InputLength = 0;
  UINT8                                        Data8 = 0;
  UINT32                                       Data32 = 0;
  LIST_ENTRY                                   *Link;
  AOD_CMD                                      *pCmd;
  UINTN                                        Index;
  struct {
    UINT32 CmdId;
    UINT8  Property;
    CHAR8  *pName;
  }CmdIdList[0x40];
  if (IsListEmpty (&pInstance->CmdList)) {
    AOD_DEBUG ((EFI_D_ERROR, "There is no cmd registered\n"));
    return EFI_INVALID_PARAMETER;
  }
  //
  // 1. Sort Cmd Id
  //
  Index = 0;
  for (Index = 0; Index < 0x40; Index ++) {
    CmdIdList[Index].CmdId = 0;
    CmdIdList[Index].Property = 0;
    CmdIdList[Index].pName = NULL;
  }
  Index = 0;
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (AodIsCmdInOBID (pCmd->CmdId)) {
      CmdIdList[Index].CmdId = pCmd->CmdId;
      CmdIdList[Index].Property = pCmd->Property?1:0;
      CmdIdList[Index].pName = pCmd->pName;
      Index ++;
     }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }


  PkgLength = (UINT32)pStream->Length;
  InputLength = (UINT32)pStream->Length;

  //
  // 2: Build String
  //
  for (Index = 0x40; Index > 0; Index --) {
    Status = AodStreamAppend (pStream, (VOID *)CmdIdList[Index-1].pName, APPEND_AMLSTRING, 0);
    if (EFI_ERROR (Status)) return Status;
  }
  //
  // 3: Build Property
  //

  for (Index = 0x40; Index > 0; Index --) {
    Status = AodStreamAppend (pStream, NULL, CmdIdList[Index-1].Property?APPEND_AMLONE:APPEND_AMLZERO, 0);
    if (EFI_ERROR (Status)) return Status;
  }
  //
  // 4: Build CmdId
  //
  for (Index = 0x40; Index > 0; Index --) {
    Status = AodStreamAppend (pStream, (VOID *)&CmdIdList[Index-1].CmdId, CmdIdList[Index-1].CmdId?APPEND_AMLDWORD:APPEND_AMLZERO, 0);
    if (EFI_ERROR (Status)) return Status;
  }
  //
  // 5: 0x40, COUNT
  //
  Data8 = 0x40;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_AMLBYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //6. NumElements
  Data8 = 0xC1;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //8. PackageLen
  PkgLength = (UINT32)pStream->Length - PkgLength;
  Status = AodStreamAppend (pStream, (VOID *)&PkgLength, APPEND_AMLPKGLEN, 0);
  if (EFI_ERROR (Status)) return Status;

  //8. Package Opcode
  Data8 = 0x12;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;
  //9. Name OBIT
  Data32 = SIGNATURE_32('O', 'B', 'I', 'D');
  Status = AodStreamAppend (pStream, (VOID *)&Data32, APPEND_DWORD, 0);
  if (EFI_ERROR (Status)) return Status;

  //10. Package Opcode
  Data8 = 0x8;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  return EFI_SUCCESS;
}

/**
  This function used to Generate AOD OBIE table from CmdList
  class OIDPackage
  {
  [WmiDataId(1),
  read, write,
  Description("Number of ID")
  ] uint8 Length;
  [WmiDataId(2),
  read, write,
  Description("64*4 bytes of data")
  ] uint32 ID[64];
  [WmiDataId(3),
  read, write,
  Description("64*4 bytes of data")
  ] uint32 property[64];
  [WmiDataId(4),
  read, write,
  Description("64 strings")
  ] string IDstring[64];
  };
  Name(OBID, Package(193) {
    0x40,
    "ID"[64],
    "Property"[64],
    "String"[64],
  }
  @param[in]    pInstance            The private data of AOD Dxe instance
  @param[in]    pStream              The AML Stream


  @retval EFI_INVALID_PARAMETER when the Cmd List is empty
  @retval EFI_SUCCESS means generate OBIE AML successfully and OBIE AML will
                      store at AML Stream
**/
EFI_STATUS
EFIAPI
AodGenOBIE (
  IN     AOD_DXE_PRIVATE_DATA       *pInstance,
  IN     ASTREAM                    *pStream
  )
{
  EFI_STATUS                                   Status;
  UINT32                                       PkgLength = 0;
  UINT32                                       InputLength = 0;
  UINT8                                        Data8 = 0;
  UINT32                                       Data32 = 0;
  LIST_ENTRY                                   *Link;
  AOD_CMD                                      *pCmd;
  UINTN                                        Index;
  struct {
    UINT32 CmdId;
    UINT8  Property;
    CHAR8  *pName;
  }CmdIdList[0x40];

  //
  // 1. Sort Cmd Id
  //
  Index = 0;
  for (Index = 0; Index < 0x40; Index ++) {
    CmdIdList[Index].CmdId = 0;
    CmdIdList[Index].Property = 0;
    CmdIdList[Index].pName = NULL;
  }
  Index = 0;
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    if (AodIsCmdInOBIE (pCmd->CmdId)) {
      CmdIdList[Index].CmdId = pCmd->CmdId;
      CmdIdList[Index].Property = pCmd->Property?1:0;
      CmdIdList[Index].pName = pCmd->pName;
      Index ++;
     }
    Link = GetNextNode (&pInstance->CmdList, Link);
  }


  PkgLength = (UINT32)pStream->Length;
  InputLength = (UINT32)pStream->Length;
  //
  // 2: Build String
  //
  for (Index = 0x40; Index > 0; Index --) {
    Status = AodStreamAppend (pStream, (VOID *)CmdIdList[Index-1].pName, APPEND_AMLSTRING, 0);
    if (EFI_ERROR (Status)) return Status;
  }
  //
  // 3: Build Property
  //
  for (Index = 0x40; Index > 0; Index --) {
    Status = AodStreamAppend (pStream, NULL, CmdIdList[Index-1].Property?APPEND_AMLONE:APPEND_AMLZERO, 0);
    if (EFI_ERROR (Status)) return Status;
  }
  //
  // 4: Build CmdId
  //
  for (Index = 0x40; Index > 0; Index --) {
    Status = AodStreamAppend (pStream, (VOID *)&CmdIdList[Index-1].CmdId, CmdIdList[Index-1].CmdId?APPEND_AMLDWORD:APPEND_AMLZERO, 0);
    if (EFI_ERROR (Status)) return Status;
  }
  //
  // 5: 0x40, COUNT
  //
  Data8 = 0x40;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_AMLBYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //6. NumElements
  Data8 = 0xC1;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //8. PackageLen
  PkgLength = (UINT32)pStream->Length - PkgLength;
  Status = AodStreamAppend (pStream, (VOID *)&PkgLength, APPEND_AMLPKGLEN, 0);
  if (EFI_ERROR (Status)) return Status;

  //8. Package Opcode
  Data8 = 0x12;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  //9. Name OBIT
  Data32 = SIGNATURE_32('O', 'B', 'I', 'E');
  Status = AodStreamAppend (pStream, (VOID *)&Data32, APPEND_DWORD, 0);
  if (EFI_ERROR (Status)) return Status;

  //10. Package Opcode
  Data8 = 0x8;
  Status = AodStreamAppend (pStream, (VOID *)&Data8, APPEND_BYTE, 0);
  if (EFI_ERROR (Status)) return Status;

  return EFI_SUCCESS;
}

/**
  This function used to Generate AOD AML for WMI command static struct information

  @param[in]    pThis                The CMD Protocol instance
  @param[out]   AmlLength            The AOD AML data length
  @param[out]   ppAml                The output pointer of AOD AML data, it's caller's
                                     responsibility to Free the resource

  @retval EFI_OUT_OF_RESOURCES when allocate memory failed
  @retval EFI_SUCCESS means generate AOD AML successfully
**/
EFI_STATUS
EFIAPI
AodGenAml (
  IN     AOD_CMD_PROTOCOL                    *pThis,
  OUT    UINTN                               *AmlLength,
  OUT    UINT8                               **ppAml
  )
{
  AOD_DXE_PRIVATE_DATA                         *pInstance;
  EFI_STATUS                                   Status;
  ASTREAM                                      *pStream;
  UINT8                                        *pPtr;
  UINTN                                         Size;

  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);

  pStream = AllocateZeroPool (sizeof (ASTREAM));
  if (pStream == NULL) return EFI_OUT_OF_RESOURCES;
  pStream->MaxLength = 0x100000;
  pStream->pPtr = AllocatePages (EFI_SIZE_TO_PAGES (pStream->MaxLength));
  if (pStream->pPtr == NULL) return EFI_OUT_OF_RESOURCES;
  //Generate OBIT
  Status = AodGenOBIT (pInstance, pStream);
  if (EFI_ERROR (Status)) return Status;
  //Generate OBIE
  Status = AodGenOBIE (pInstance, pStream);
  if (EFI_ERROR (Status)) return Status;
  //Generate OBID
  Status = AodGenOBID (pInstance, pStream);
  if (EFI_ERROR (Status)) return Status;

  Size = pStream->Length;
  pPtr = AllocateZeroPool (Size);
  if (pPtr == NULL) return EFI_OUT_OF_RESOURCES;

  CopyMem (pPtr, pStream->pPtr+pStream->MaxLength-pStream->Length, Size);
  FreePages (pStream->pPtr, EFI_SIZE_TO_PAGES (pStream->MaxLength));
  FreePool (pStream);

  *ppAml = pPtr;
  *AmlLength = Size;

  return EFI_SUCCESS;
}

/**
  This function used to Update WMI commnad current into global NVS

  @param[in]    pThis                The CMD Protocol instance
  @param[out]   WMINvsCurrentBuf     Pointer of AOD Current NVS buffer

  @retval EFI_INVALID_PARAMETER This is null or WMINvsCurrentBuf is NULL
  @retval EFI_SUCCESS means successfully
**/
EFI_STATUS
EFIAPI
AodSsdtUpdateNvsCurrent (
  IN     AOD_CMD_PROTOCOL                    *pThis,
  IN     UINT32                              *WMINvsCurrentBuf
  )
{
  AOD_DXE_PRIVATE_DATA                         *pInstance;
  LIST_ENTRY                                   *Link;
  AOD_CMD                                      *pCmd;

  if ((pThis == NULL) || (WMINvsCurrentBuf == NULL)) return EFI_INVALID_PARAMETER;
  pInstance = AOD_DXE_PRIVATE_FROM_CMDPROTCOL (pThis);
  if (IsListEmpty (&pInstance->CmdList)) {
    AOD_DEBUG ((EFI_D_ERROR, "There is no cmd registered\n"));
    return EFI_INVALID_PARAMETER;
  }
  Link  = GetFirstNode (&pInstance->CmdList);
  while (!IsNull (&pInstance->CmdList, Link)) {
    pCmd = AOD_CMD_FROM_LINK (Link);
    WMINvsCurrentBuf[pCmd->CurrentNvsIndex] = pCmd->Current;
    Link = GetNextNode (&pInstance->CmdList, Link);
  }
  return EFI_SUCCESS;
}
/**
  This function used to Calculate checksum8 of AML data

  @param[in]    Data                 The AML data pointer
  @param[out]   Length               The AML data length

  @retval the checksum8 will been returned
**/
UINT8
EFIAPI
CalculateChecksum8 (
  IN  UINT8                        *Data,
  IN  UINTN                        Length
  )
{
  UINTN                                 Index;
  UINT8                                 Sum = 0;

  for (Index = 0; Index < Length; Index ++) {
    Sum += Data[Index];
  }

  return((UINT8)(0 - Sum));
}
/**
  This function used to get acpi aml from section raw

  @param[in]    OemTableId   the acpi table oemtableid

  @retval the acpi table will returned
**/
VOID *
GetAcpiTableFromSectionRaw (
  UINT64          OemTableId
  )
{
  EFI_ACPI_DESCRIPTION_HEADER           *pTable;
  EFI_STATUS                            Status;
  UINTN                                 TableSize;
  UINTN                                 SectionInstance = 0;
  do {
    pTable = NULL;
    Status = GetSectionFromFv (&gEfiCallerIdGuid, EFI_SECTION_RAW, SectionInstance, (VOID **) &pTable, &TableSize);
    if (!EFI_ERROR (Status)) {
      if (pTable->OemTableId == OemTableId) {
       return pTable;
      }
    }
    SectionInstance ++;
  } while (!EFI_ERROR (Status));
  return NULL;
}
/**
  This function used to update "AODE" memory region address and filed length

  @param[in]    SsdtPointer           the AML data pointer of AOD SSDT
  @param[in]    RegionStartAddress    the "AODE" Address
  @param[in]    SsdtPointer           the "AODE" size

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
UpdateOpRegion (
  IN UINT8               *SsdtPointer,
  IN UINT32              RegionStartAddress,
  IN UINT32              RegionSize
  )
{
  UINT8                         AddressPrefix;
  UINT32                        *Address;
  UINT32                        *DwordSize;
  UINT8                         SizePrefix;
  UINT16                        *WordSize;
  UINT8                         *ByteSize;

  AddressPrefix = *(SsdtPointer + 5);
  ASSERT (AddressPrefix == 0x0C);
  Address = (UINT32*) (SsdtPointer + 6);
  *Address = (UINT32)RegionStartAddress;
  SizePrefix = *(SsdtPointer + 10);
  if (SizePrefix == 0x0C) {
    DwordSize = (UINT32*) (SsdtPointer + 11);
    *DwordSize = RegionSize;
  } else if (SizePrefix == 0x0B) {
    ASSERT (RegionSize < 0x10000);
    RegionSize &= 0xFFFF;
    WordSize = (UINT16*) (SsdtPointer + 11);
    *WordSize = (UINT16) RegionSize;
  } else if (SizePrefix == 0x0A) {
    ASSERT (RegionSize < 0x100);
    RegionSize &= 0xFF;
    ByteSize = (UINT8*) (SsdtPointer + 11);
    *ByteSize = (UINT8) RegionSize;
  } else {
    ASSERT (FALSE);
  }

  return EFI_SUCCESS;
}

/**
  This function used to install AOD SSDT table and gets called each time the
  EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.

  @param[in]     Event          EFI_EVENT
  @param[in]     Context        The Parameter Buffer

  @retval EFI_SUCCESS
**/
VOID
EFIAPI
AodSsdtInstallReadyToBootEvt (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  EFI_STATUS                            Status;
  EFI_ACPI_DESCRIPTION_HEADER           *pTable;
  UINT8                                 *pNewTable;
  EFI_ACPI_TABLE_PROTOCOL               *pAcpiTableProtocol;
  UINTN                                 Key;
  AOD_CMD_PROTOCOL                      *CmdProtocol;
  UINTN                                 AmlSize;
  UINT8                                 *AmlData;
  AodNvAreaStruct                       *AodNvs;
  gBS->CloseEvent (Event); // Only run once
  AOD_DEBUG ((EFI_D_INFO,"start\n"));
  Status = gBS->LocateProtocol (&gAmdAodCmdProtocolGuid, NULL, (VOID **) &CmdProtocol);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR,"  Locate %g Failed, Status = [%r]\n", &gAmdAodCmdProtocolGuid, Status));
    return;
  }
  //
  // Get acpi table with oem tabled id from section raw
  //
  pTable = (EFI_ACPI_DESCRIPTION_HEADER *)GetAcpiTableFromSectionRaw (SIGNATURE_64('A','O','D',' ',' ',' ',' ',' '));
  if (pTable == NULL) {
    AOD_DEBUG ((EFI_D_ERROR,"  Get Aod ssdt failed\n"));
    return;
  }
  for (pNewTable = (UINT8 *)((UINTN)pTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER)); \
       pNewTable <= (UINT8 *)((UINTN)pTable + (UINTN)(pTable->Length)); \
       pNewTable++) {

    if ((*(UINT32*) pNewTable) == SIGNATURE_32 ('A', 'O', 'D', 'E')) {
      if (*(UINT16*)(pNewTable - 2) == 0x805B) { //if openregion
        AOD_DEBUG ((EFI_D_INFO, "  Update AODE Base = 0x%x, Size = 0x%x\n", (UINT32) (UINTN) mNvsAddress, mNvsSize));
        UpdateOpRegion (pNewTable, (UINT32)mNvsAddress, (UINT32)mNvsSize);
      }
      break;
    }
  }
  for (pNewTable = (UINT8 *)((UINTN)pTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER)); \
       pNewTable <= (UINT8 *)((UINTN)pTable + (UINTN)(pTable->Length)); \
       pNewTable++) {

    if ((*(UINT32*) pNewTable) == SIGNATURE_32 ('A', 'S', 'M', 'I')) {
      if ((*(UINT8*)(pNewTable + 4) == 0xB) && (*(UINT8*)(pNewTable - 1) == 0x8)) { //if word prefix and NameOp
        *(UINT16*)(pNewTable + 5) = (UINT16)MmioRead16 (0xFED8036A);
      }
      break;
    }
  }
  for (pNewTable = (UINT8 *)((UINTN)pTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER)); \
       pNewTable <= (UINT8 *)((UINTN)pTable + (UINTN)(pTable->Length)); \
       pNewTable++) {

    if ((*(UINT32*) pNewTable) == SIGNATURE_32 ('D', 'S', 'M', 'I')) {
      if ((*(UINT8*)(pNewTable + 4) == 0xA) && (*(UINT8*)(pNewTable - 1) == 0x8)) { //if byte prefix and NameOp
        *(UINT8*)(pNewTable + 5) = PcdGet8 (SwSmiCmdAodUpdateMemClkTimings);
      }
      break;
    }
  }
  //
  // Locate ACPI Table protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&pAcpiTableProtocol);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR,"  Locate %g Failed, Status = [%r]\n", &gEfiAcpiTableProtocolGuid, Status));
    return;
  }
  //
  // Gen Aod Aml
  //
  Status = AodGenAml (CmdProtocol, &AmlSize, &AmlData);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR,"  Aod AML Gen Status = [%r]\n", Status));
    return;
  }
  //
  // Update NVS Current
  //
  AodNvs = (AodNvAreaStruct *)(UINTN)mNvsAddress;
  Status = AodSsdtUpdateNvsCurrent (CmdProtocol, AodNvs->AodWMIRuntimeCurrent);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR,"  Aod Update NVS Current Status = [%r]\n", Status));
    return;
  }
  //
  // Copy Aod Aml into Aod ssdt
  //
  pNewTable = AllocateZeroPool (pTable->Length + AmlSize);
  if (pNewTable == NULL) {
    AOD_DEBUG ((EFI_D_ERROR,"  Aollocate Size Status = %r\n", Status));
    return;
  }
  CopyMem (pNewTable, (UINT8 *)pTable, sizeof(EFI_ACPI_DESCRIPTION_HEADER)); //Header
  CopyMem (pNewTable + sizeof(EFI_ACPI_DESCRIPTION_HEADER), AmlData, AmlSize); // AML GEN by Aod
  CopyMem (pNewTable + sizeof(EFI_ACPI_DESCRIPTION_HEADER)+AmlSize, \
    (UINT8 *)pTable + sizeof(EFI_ACPI_DESCRIPTION_HEADER), \
    pTable->Length-sizeof(EFI_ACPI_DESCRIPTION_HEADER\
    )); //Aml gen by iasl

  ((EFI_ACPI_DESCRIPTION_HEADER *)pNewTable)->Length = (UINT32)(AmlSize + pTable->Length);
  ((EFI_ACPI_DESCRIPTION_HEADER *)pNewTable)->Checksum = CalculateChecksum8 ((UINT8*)pNewTable, AmlSize + pTable->Length);

  pTable =(EFI_ACPI_DESCRIPTION_HEADER *)pNewTable;
  //
  // Install new DSDT
  //
  Status = pAcpiTableProtocol->InstallAcpiTable (pAcpiTableProtocol, (VOID *)pTable, pTable->Length, &Key);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR,"  Install New Aod SSDT Table Failed, Status = [%r]\n", Status));
    return;
  }

  AOD_DEBUG ((EFI_D_INFO, "End\n"));
  return;
}

/**
  This function used to install AOD SSDT table and initial all AOD ACPI modules

  @param[in]     Private          Private data pointer of AodDxe

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
AodInstallAcpi (
  IN  AOD_DXE_PRIVATE_DATA                    *Private
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;
  UINTN                                   BufferSize;
  AodNvAreaStruct                         *AodNvs;
  AOD_NV_AREA_UPDATED_PROTOCOL            *AodNvsProtocol;
  EFI_HANDLE                              DriverHandle = NULL;
  AOD_DEBUG ((EFI_D_INFO,"start\n"));
  //
  // Init and allocate AodMemBuffer
  //
  BufferSize = sizeof (AodNvAreaStruct);
  Status = gBS->AllocatePool (
                  EfiACPIMemoryNVS,
                  BufferSize,
                  &AodNvs
                  );
  if (EFI_ERROR(Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Allocate Nvs = %r\n",  Status));
    return Status;
  }
  gBS->SetMem (AodNvs, BufferSize, 0);
  mNvsAddress = (UINTN) AodNvs;
  mNvsSize   = BufferSize;
  BufferSize = sizeof (AOD_NV_AREA_UPDATED_PROTOCOL);
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  BufferSize,
                  &AodNvsProtocol
                  );
  if (EFI_ERROR(Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Allocate Protocol = %r\n",  Status));
    return Status;
  }
  AodNvsProtocol->Version = 1;
  AodNvsProtocol->AodNvAreaPtr = AodNvs;
  //
  // init aod nvs by program and platform, this is request when RyMaster
  // get the default config of AOD WMI
  //
  Status = AodDispatchInitTable (AOD_UPDATE_NVS, (VOID *)AodNvs);
  if (EFI_ERROR (Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Table[AOD_UPDATE_NVS] = %r\n", Status));
  }
  //
  // install aod nvs area update protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &DriverHandle,
                  &gAodNvAreaUpdatedProtocolGuid,
                  AodNvsProtocol,
                  NULL
                  );
  if (EFI_ERROR(Status)) {
    AOD_DEBUG ((EFI_D_ERROR, "  Install %g Protocol =%r\n", &gAodNvAreaUpdatedProtocolGuid, Status));
    return Status;
  }
  //
  // Cread ready to boot event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AodSsdtInstallReadyToBootEvt,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  AOD_DEBUG ((EFI_D_INFO, "End\n"));
  return Status;
}