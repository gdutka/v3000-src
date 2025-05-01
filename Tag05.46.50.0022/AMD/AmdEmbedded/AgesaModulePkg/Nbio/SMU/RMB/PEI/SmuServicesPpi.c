/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <AMD.h>

#include <PiPei.h>
#include <Filecode.h>
#include <GnbRegistersRMB.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/IdsLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/NbioHandleLib.h>
#include <Library/SmnAccessLib.h>
#include <Library/PcieConfigLib.h>
#include <SMU_RMB_MsgDef.h>
#include <Library/NbioSmuV13Lib.h>
#include <Ppi/NbioSmuServicesPpi.h>
#include <Ppi/NbioPcieServicesPpi.h>
#include <Guid/GnbPcieInfoHob.h>
#include "SmuV13Pei.h"
#include <AmdFusesRMB.h>
#include <Addendum/Apcb/Inc/EnvironmentFlags.h>

#define FILECODE        NBIO_SMU_RMB_PEI_SMUSERVICESPPI_FILECODE

/*----------------------------------------------------------------------------------------
 *                         E X T E R N   D E F I N I T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define MAX_CAC_WEIGHT_NUM  21

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
/**
  Reads a fuse value based on an enumerated list of fuse "names"

  Parameters:
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  MyHandle
    Pointer to GNB_HANDLE* to return GNB_HANDLE of this InstanceId
  FuseValue
    Pointer to the value of the fuse

  Status Codes Returned:
  EFI_SUCCESS           - The fuse was located and the FuseValue returned is valid
  EFI_INVALID_PARAMETER - InstanceId did not reference a valid NBIO instance
**/
EFI_STATUS
SmuGetGnbHandle (
  IN       UINT32                         InstanceId,
     OUT   GNB_HANDLE                     **MyHandle
  )
{
  EFI_PEI_HOB_POINTERS            Hob;
  GNB_PCIE_INFORMATION_DATA_HOB   *PciePlatformConfigHob;
  PCIe_PLATFORM_CONFIG            *Pcie;
  EFI_STATUS                      Status;
  GNB_HANDLE                      *GnbHandle;
  BOOLEAN                         InstanceFound;
  GNB_PCIE_INFORMATION_DATA_HOB   *GnbPcieInfoDataHob;

  GnbHandle = NULL;
  // Need topology structure
  GnbPcieInfoDataHob = NULL;
  PciePlatformConfigHob = NULL;

  Status = PeiServicesGetHobList (&Hob.Raw);

  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
        CompareGuid ( &Hob.Guid->Name, &gGnbPcieHobInfoGuid)) {
        PciePlatformConfigHob = (GNB_PCIE_INFORMATION_DATA_HOB *)Hob.Raw;
      Status = EFI_SUCCESS;
      break;
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  if (PciePlatformConfigHob == NULL) {
    return EFI_NOT_FOUND;
  }
  Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);
  GnbHandle = NbioGetHandle (Pcie);

  InstanceFound = FALSE;
  Status = EFI_INVALID_PARAMETER;
  while ((InstanceFound == FALSE) && (GnbHandle != NULL)) {
    if (GnbHandle->SocketId == InstanceId) {
      InstanceFound = TRUE;
      Status = EFI_SUCCESS;
    } else {
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }
  *MyHandle = GnbHandle;
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  Debug function used to read fuse values from the console, as well as to use for debugging

  Parameters:
  BufferSize
    Size of the buffer
  BufferIn
    Pointer to the buffer containing the fuse

  Returns:
    VOID
**/
VOID
PrintBuffer (
  UINTN   BufferSize,
  UINT8   *BufferIn
    )
{
  UINTN                           Counter;
  UINTN                           Counter2;
  UINT32                          *Buffer;
  UINT8                           *Bytes;

  Buffer = (UINT32 *)BufferIn;

  // Print in word chunks for better visibility
  for (Counter = 0; Counter < (BufferSize/4 +1); Counter++) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Buffer %d is %x \n", Counter, Buffer[Counter]);
    Bytes = (UINT8 *)Buffer;
    for (Counter2 = 0; Counter2 < 4; Counter2++) {
//      IDS_HDT_CONSOLE (MAIN_FLOW, "Byte at index %d is %x \n", Counter2, Bytes[Counter2]);
    }
  }
}

/*----------------------------------------------------------------------------------------
/**
  Shifts bits through multiple bytes. Take something like byte0[3] through byte4[2] and make
  it an inteligible number.

  Parameters:
  Bytes
    The number of bytes in buffer
  BitsToShift
    Number of bits to shift left
  EndBit
    The number of the end bit on the last byte
  BufferSize
    Pointer to the buffer to work with

  Returns:
    VOID
**/
VOID
ShiftFuseBuffer (
  UINTN Bytes,
  UINTN BitsToShift,
  UINTN EndBit,
  UINT8 *Buffer
  )
{
  INTN  Counter;
  UINT8 Remainder, TempRemainder;

  Remainder = 0;

  IDS_HDT_CONSOLE (MAIN_FLOW, "ShiftFuseBuffer: Bits to shift %d \n", BitsToShift);
  // Make sure we need to do anything at all
  if (BitsToShift == 0) {
    return;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "ShiftFuseBuffer: Element 0 0 is %x \n", &Buffer[0]);

  // Go through byte by byte, calculate remainder, shift, apply remainder
  for (Counter = Bytes - 1; Counter >= 0 ; Counter--)
  {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Iteration %d \n", Counter);
    PrintBuffer(Bytes, Buffer);
    // Shift the bits that will be shifted out all the way to the left to where it will be in the next byte.
    // Lets call this the remainder
    TempRemainder = Buffer[Counter];
    TempRemainder = TempRemainder << (8-BitsToShift);
    IDS_HDT_CONSOLE (MAIN_FLOW, "TempRemainder Calculated %x \n", TempRemainder);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Remainder is %x \n", Remainder);
    IDS_HDT_CONSOLE (MAIN_FLOW, "Byte is %x \n", Buffer[Counter]);
    // Shift buffer
    Buffer[Counter] = Buffer[Counter] >> BitsToShift;
    // Apply previous remainder
    Buffer[Counter] = Buffer[Counter] | Remainder;
    // Set next remainder for next pass if there is one
    Remainder = TempRemainder;
  }

  PrintBuffer(Bytes, Buffer);
}

/*----------------------------------------------------------------------------------------
/**
  Reads a fuse value based on an enumerated list of fuse "names"

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  FuseName
    The "name" of the fuse from an enumerated list of fuses supported by this function. This translates
    to an number value used in a LUT at compile time
  BufferSize
    Pointer to value indicating the size of the buffer in bytes passed in. On return this value will
    be the actual size of the retrieved fuse.
  FuseValue
    Pointer to memory that will contain the value of the retrieved fuse. This memory should
    be managed by the caller.

  Status Codes Returned:
  EFI_SUCCESS           - The fuse was located and the FuseValue returned is valid
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
                        - FuseName was not found in the list of supported fuse identifier values
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
  EFI_NOT_FOUND         - Fuse was not found in the table. Out variables are untouched this way.
**/
EFI_STATUS
SmuReadFuseByName (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINTN                          FuseName,
  IN OUT   UINTN                          *BufferSize,
     OUT   UINT8                          *FuseValue
  )
{
  AMD_FUSE    *FuseFromTable;
  UINT32       WordReads, AlignedAddress, SizeIndex, BytesToRead, ReadWord, ReadWordIndex, ReturnFuseValIndex;
  UINT8       *BytePointer, EndBit;
  INT8        EndBitTemp;
  UINTN       Index;

  FuseFromTable = NULL;

  if (FuseValue == NULL || BufferSize == NULL || This == NULL) {
    ASSERT(FALSE);
    return EFI_INVALID_PARAMETER;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry Name: %d , Buffersize %d\n", __FUNCTION__, FuseName, *BufferSize);

  // Search for the fuse we want to read, if it is not in the table return not found
  for (Index = 0; Index < sizeof(AllFuses)/sizeof(AMD_FUSE); Index++) {
    if (AllFuses[Index].EnumIndex == FuseName) {
      FuseFromTable = &AllFuses[Index];
    }
  }
  if (FuseFromTable == NULL) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "ERROR: Did not find fuse %d in table\n", FuseName);
    return EFI_NOT_FOUND;
  }

  IDS_HDT_CONSOLE(MAIN_FLOW, "%a StartBit: %d, Bits %d\n", __FUNCTION__, FuseFromTable->StartBit, FuseFromTable->Size);

  // All SMN reads need to be aligned to nearest 32 bit word
  AlignedAddress = (~0x3ull) & FuseFromTable->SmnStartAddress;
  IDS_HDT_CONSOLE (MAIN_FLOW, "AlignedAddress %x RealAddress %x\n", AlignedAddress, FuseFromTable->SmnStartAddress);

  // Calculate how many aligned 32 bit reads we need. This should be the start bit number + size in bits divided by 32
  // and one added
  WordReads = (FuseFromTable->Size + FuseFromTable->StartBit)/32;
  if ((FuseFromTable->StartBit == 0 ) && ((FuseFromTable->Size % 32) == 0)) {
    // If it starts at bit 0 and is only 32 bits do not add extra word
  }
  else {
    // Need to add extra word for any remainder
    WordReads++;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Need %d word reads\n", WordReads);

  // Calulate the end bit
  EndBitTemp = (FuseFromTable->Size%32 + FuseFromTable->StartBit) - 1;
  if (EndBitTemp < 0) {
    EndBitTemp += 32;
  }
  EndBit = (UINT8) EndBitTemp;
  IDS_HDT_CONSOLE(MAIN_FLOW, "End bit %d\n", EndBit);

  // Calculate bytes needed to store the value of this fuse, regardlesss of alignment and check this vs the supplied
  // output buffer.
  BytesToRead = FuseFromTable->Size/8;
  if ((FuseFromTable->Size%8) > 0) {
    BytesToRead++;
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "Bytes to read %d\n", BytesToRead);

  // Make sure buffer passed in is of correct size
  if (*BufferSize < BytesToRead) {
    // Put the size of the buffer we need in *BufferSize
    *BufferSize = BytesToRead;
    return EFI_BUFFER_TOO_SMALL;
  }

  ReturnFuseValIndex = 0;
  // Now read the value in 32 bit chunks
  for (SizeIndex = 0; SizeIndex < WordReads; SizeIndex++ ) {
    SmuRegisterRead(
      This,
      InstanceId,
      AlignedAddress + SizeIndex*sizeof(UINT32),
      &ReadWord
      );
      IDS_HDT_CONSOLE (MAIN_FLOW, "Read SMN address %x \n", AlignedAddress + SizeIndex*sizeof(UINT32));
      IDS_HDT_CONSOLE (MAIN_FLOW, "ReadWord Value %x\n", ReadWord);

    // Copy only total number of bytes (calculated earlier)
    for (ReadWordIndex = 0; (ReadWordIndex < sizeof(UINT32)) && (ReturnFuseValIndex < BytesToRead); ReadWordIndex++, ReturnFuseValIndex++) {
      // Advance counter to pass over unnecessary bytes ONLY for the pass through the first UINT32
      if (SizeIndex == 0 && ReadWordIndex == 0) {
        ReadWordIndex += FuseFromTable->StartBit / 8;
      }
      IDS_HDT_CONSOLE (MAIN_FLOW, "WordIndex %d FuseValIndex %d\n", ReadWordIndex, ReturnFuseValIndex);
      BytePointer = (UINT8 *)&ReadWord;
      FuseValue[ReturnFuseValIndex] = BytePointer[ReadWordIndex];
      IDS_HDT_CONSOLE (MAIN_FLOW, "Read byte %x\n", BytePointer[ReadWordIndex]);
    }

    IDS_HDT_CONSOLE (MAIN_FLOW, "Testing last for last read?\n");
  }

  *BufferSize = BytesToRead;

  ShiftFuseBuffer (
    BytesToRead,
    FuseFromTable->StartBit%8,
    EndBit%8,
    FuseValue
    );

  // Shift out the preceding bits that were read in but out of bounds of the field we want
  FuseValue[BytesToRead-1] = FuseValue[BytesToRead-1]<< (BytesToRead*8 - FuseFromTable->Size);
  FuseValue[BytesToRead-1] = FuseValue[BytesToRead-1]>> (BytesToRead*8 - FuseFromTable->Size);

  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------
/**
  Reads a fuse value based on chain, start bit, and size

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  FuseChain
    The chain number of the fuse to read
  FuseStartBit
    The starting bit within the specified fuse chain of the fuse to read
  FuseSize
    The bitfield width of the fuse to read
  FuseValue
    Pointer to the value of the fuse

  Status Codes Returned:
  EFI_SUCCESS           - The fuse was located and the FuseValue returned is valid
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
                        - FuseChain was not a valid fuse chain value
                        - FuseStartBit was not valid within the specified fuse chain
                        - FuseSize exceeded the size available in the return field
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuReadFuseByLocation (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         FuseChain,
  IN       UINT32                         FuseStartBit,
  IN       UINT32                         FuseSize,
     OUT   UINT32                         *FuseValue
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------
/**
  Executes a specific SMU service and returns the results (Generally intended for NBIO internal requests)

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the NBIO instance associated with this socket/die
  ServiceRequest
    The service request identifier
  InputParameters
    Pointer to an array of service request arguments (for CZ, UINT32[6]).  Input parameters are not validated by the driver prior to issuing the service request.
  ReturnValues
    Pointer to an array of service request response values (for CZ, UINT32[6])

  Status Codes Returned:
  EFI_SUCCESS           - The SMU service request was completed and the argument registers are returned in "ReturnValues"
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
                        - ServiceRequest was not supported by this version of SMU
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuServiceRequest (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         ServiceRequest,
  IN       UINT32                         *InputParameters,
     OUT   UINT32                         *ReturnValues
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          SmuArg[6];
  UINT32                          SmuResult;
  PRESIL_CTRL1                    PreSilCtrl1;

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "SmuServiceRequest Entry\n");
  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    LibAmdMemCopy (SmuArg, InputParameters, 24, (AMD_CONFIG_PARAMS *) NULL);
    SmuResult = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), ServiceRequest, SmuArg, 0);
    //if (SmuResult == 1) {
      LibAmdMemCopy (ReturnValues, SmuArg, 24, (AMD_CONFIG_PARAMS *) NULL);
    //} else {
    //  Status = EFI_INVALID_PARAMETER;
    //  IDS_HDT_CONSOLE (MAIN_FLOW, "InstanceId not found!!\n");
    //  ASSERT (FALSE);       // Unsupported SMU service request
    //}
  }
  IDS_HDT_CONSOLE (MAIN_FLOW, "SmuServiceRequest Exit Status = %d\n", Status);
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  Executes a specific DXIO service and returns the results (Generally intended for NBIO internal requests)

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  ServiceRequest
    The service request identifier
  InputParameters
    Pointer to an array of service request arguments (for CZ, UINT32[6]).  Input parameters are not validated by the driver prior to issuing the service request.
  ReturnValues
    Pointer to an array of service request response values (for CZ, UINT32[6])

  Status Codes Returned:
  EFI_SUCCESS           - The SMU service request was completed and the argument registers are returned in "ReturnValues"
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
                        - ServiceRequest was not supported by this version of SMU
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuDxioServiceRequest (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         ServiceRequest,
  IN       UINT32                         *InputParameters,
     OUT   UINT32                         *ReturnValues
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          SmuArg[6];
  UINT32                          SmuResult;

  IDS_HDT_CONSOLE (MAIN_FLOW, "SmuDxioServiceRequest Entry\n");
  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "GnbHandle Found\n");
    LibAmdMemCopy (SmuArg, InputParameters, 24, (AMD_CONFIG_PARAMS *) NULL);
    SmuResult = NbioDxioServiceRequestV13 (GnbHandle, ServiceRequest, SmuArg, 0);
    if (SmuResult == 1) {
      LibAmdMemCopy (ReturnValues, SmuArg, 24, (AMD_CONFIG_PARAMS *) NULL);
    } else {
      Status = EFI_INVALID_PARAMETER;
      IDS_HDT_CONSOLE (MAIN_FLOW, "InstanceId not found!!\n");
      ASSERT (FALSE);       // Unsupported SMU service request
    }
  }
    IDS_HDT_CONSOLE (MAIN_FLOW, "SmuDxioServiceRequest Exit Status = %d\n", Status);
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  Reads a 32-bit register from SMU register space (Generally intended for NBIO internal requests)

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  RegisterIndex
    The index of the register to be read
  RegisterValue
    Pointer to a UINT32 to store the value read from the register

  Status Codes Returned:
  EFI_SUCCESS           - The register value is returned in RegisterValue
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuRegisterRead (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         RegisterIndex,
     OUT   UINT32                         *RegisterValue
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;

  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    SmnRegisterRead (GnbHandle->Address.Address.Bus, RegisterIndex, RegisterValue);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  Writes a 32-bit register in SMU register space (Generally intended for NBIO internal requests)

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  RegisterIndex
    The index of the register to be read
  RegisterValue
    Pointer to a UINT32 holding the value to write to the register

  Status Codes Returned:
  EFI_SUCCESS           - The specified register has been updated with the requested value
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuRegisterWrite (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         RegisterIndex,
  IN       UINT32                         *RegisterValue
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;

  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, RegisterIndex, RegisterValue, 0);
  }
  return Status;
}

/**
  Read/Modify/Write a 32-bit register from SMU register space (Generally intended for NBIO internal requests)

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  RegisterIndex
    The index of the register to be read
  RegisterANDValue
    UINT32 holding the value to be "ANDed" with the data read from the register (i.e. mask of data that will not be modified)
  RegisterORValue
    UINT32 holding the value to be "ORed" with the data read from the register (i.e. data bits to be "set" by this function)

  Status Codes Returned:
  EFI_SUCCESS           - The specified register has been updated with the requested value
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuRegisterRMW (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         RegisterIndex,
  IN       UINT32                         RegisterANDValue,
  IN       UINT32                         RegisterORValue
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          RegisterValue;

  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    SmnRegisterRead (GnbHandle->Address.Address.Bus, RegisterIndex, &RegisterValue);
    RegisterValue = (RegisterValue & RegisterANDValue) | RegisterORValue;
    SmnRegisterWrite (GnbHandle->Address.Address.Bus, RegisterIndex, &RegisterValue, 0);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  Enable/Disable HTC

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  HtcStatus
    Pointer to a BOOLEAN that returns the state of HTC.  If HtcReg.HtcTmpLmt != 0 then HTC will be enabled and return TRUE.
      TRUE   - HTC is enabled
      FALSE  - HTC is disabled

  Status Codes Returned:
  EFI_SUCCESS           - HTC register has been updated based on the value of HtcTmpLmt (i.e enable/disable)
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuHtcControl (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
     OUT   BOOLEAN                        *HtcStatus
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------
/**
  Returns the CAC weights read from SMU

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  MaxNumWeights
    The number of values to read from the SMU (defines the maximum size of ApmWeights return data buffer)
  ApmWeights
    Pointer to return data buffer
  NumWeightsRead
    The number of values read from SMU
  EnergyCountNormalize
    Pointer to return value for energy count normalize

  Status Codes Returned:
  EFI_SUCCESS           - ApmWeights contains the CaC weights read from SMU
                        - ApmWeights contains the number of values available from SMU
                        - EnergyCountNormalize contains the value returned by SMU
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_BUFFER_TOO_SMALL  - The number of weights available exceeds MaxNumWeights
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuReadCacWeights (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         MaxNumWeights,
     OUT   UINT64                         *ApmWeights
  )
{
  EFI_STATUS                           Status;
  GNB_HANDLE                           *GnbHandle;
  UINT8                                WeightIndex;
  UINT32                               SmuArg[6];
  UINT32                               SmuResult;
  PRESIL_CTRL1                         PreSilCtrl1;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  IDS_HDT_CONSOLE (MAIN_FLOW, "MaxNumWeights %x", MaxNumWeights);
  if (MaxNumWeights > MAX_CAC_WEIGHT_NUM) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "Input over Max of Weights number");
    return EFI_UNSUPPORTED;
  }

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }

  Status = SmuGetGnbHandle (0, &GnbHandle);

  for (WeightIndex = 0; WeightIndex < MaxNumWeights; WeightIndex++) {

    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
    SmuArg[0] = WeightIndex;

    SmuResult = NbioSmuServiceRequestV13 (
                  NbioGetHostPciAddress (GnbHandle),
                  BIOSSMC_MSG_ReadCoreCacWeightRegister,
                  SmuArg,
                  0
                  );

    if (SmuResult == 1) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "SmuArg[0] = %x\n", SmuArg[0]);
      IDS_HDT_CONSOLE (MAIN_FLOW, "SmuArg[1] = %x\n", SmuArg[1]);
      ApmWeights[WeightIndex] = (LShiftU64 ((UINT64) SmuArg[1], 32)) | SmuArg[0];
    } else {
      Status = EFI_INVALID_PARAMETER;
      IDS_HDT_CONSOLE (MAIN_FLOW, "InstanceId not found!!\n");
      ASSERT (FALSE);       // Unsupported SMU service request
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit Status = %d\n", __FUNCTION__, Status);
  return Status;
}


/*----------------------------------------------------------------------------------------
/**
  Program PsiVid and PsiVidEn

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  TargetVid
    Target Vid value (0 to skip enablement)
  TargetNbVid
    Target NbVid value (0 to skip enablement)
**/
EFI_STATUS
SmuPsiControl (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         TargetVid,
  IN       UINT32                         TargetNbVid
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------
/**
  Returns the TDP value from cTDP or SMU-defined value from SystemConfig in milliwatts

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  TdpValue
    Pointer to a UINT32 to store the TDP value in milliwatts

  Status Codes Returned:
  EFI_SUCCESS           - TdpValue is updated with the cTDP or SmuTDP value as appropriate
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver OR TdpValue is not available
**/
EFI_STATUS
SmuGetTdpValue (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
     OUT   UINT32                         *TdpValue
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------
/**
  Returns the brand string read from SMN space.

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  BrandStringLength
    Number of characters to be read.  This value does NOT have to be a multiple of 4.  NBIO driver code will be responsible for alignment, endianness, and string length.
  BrandString
    Pointer to a CHAR array to store the brand string.

  Status Codes Returned:
  EFI_SUCCESS           - BrandString has been updated with the value read from SMU
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuReadBrandString (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
  IN       UINT32                         BrandStringLength,
     OUT   UINT8                          *BrandString
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          SmuArg[6];
  UINT32                          SmuResult;
  UINT8                           *StringPtr;
  UINT32                          StringCount;
  UINT32                          ByteCount;
  UINT32                          Iteration;
  PRESIL_CTRL1                    PreSilCtrl1;

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    IDS_HDT_CONSOLE (MAIN_FLOW, "GnbHandle Found\n");
    StringCount = BrandStringLength;
    StringPtr = BrandString;
    Iteration = 0;
    while (StringCount != 0) {
      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
      SmuArg[0] = Iteration++;
      SmuResult = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_GetNameString, SmuArg, 0);
      if (SmuResult == 1) {
        ByteCount = (StringCount < 4) ? StringCount : 4;
        LibAmdMemCopy (StringPtr, SmuArg, StringCount, (AMD_CONFIG_PARAMS *) NULL);
        StringPtr += ByteCount;
        StringCount -= ByteCount;
      } else {
        Status = EFI_INVALID_PARAMETER;
        IDS_HDT_CONSOLE (MAIN_FLOW, "SmuRequestFail!!\n");
        break;
      }
    }
  }
    IDS_HDT_CONSOLE (MAIN_FLOW, "%a Exit Status = %d\n", __FUNCTION__, Status);
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  Reads the Core PllLock Timer and returns the programmed value

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  InstanceId
    The unique identifier of the SMU instance associated with this socket (for RMB, the socket number)
  LockTimerValue
    Pointer to a UINT32 to store the value read from the Core PLL Lock Timer.  This field alters the timing from PLL lock enable to Pll frequency lock. Please consult the PLL specification for more detail.

  Status Codes Returned:
  EFI_SUCCESS           - LockTimerValue has been updated with the value read from the PLL Lock Timer register field
  EFI_INVALID_PARAMETER - One of the input parameters was invalid
                        - InstanceId did not reference a valid NBIO instance
  EFI_UNSUPPORTED       - This function is not supported by this version of the driver
**/
EFI_STATUS
SmuReadCorePllLockTimer (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         InstanceId,
     OUT   UINT32                         *LockTimerValue
  )
{
  return EFI_UNSUPPORTED;
}

/*----------------------------------------------------------------------------------------
/**
  set the downcore register to the specified value on all dies across the system.

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.
  CoreDisMask
    The value to set the downcore register to.

  Status Codes Returned:
  EFI_SUCCESS           - The downcore register was successfully updated.
  EFI_NOT_FOUND         - the PPI is not in the database.

**/
EFI_STATUS
SmuSetDownCoreRegister (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This,
  IN       UINT32                         CoreDisMask
  )
{
  UINT32                          SmuArg[6];
  CONST EFI_PEI_SERVICES          **PeiServices;
  PEI_AMD_NBIO_PCIE_SERVICES_PPI  *PcieServicesPpi;
  PCIe_PLATFORM_CONFIG            *Pcie;
  EFI_STATUS                      Status;
  GNB_HANDLE                      *GnbHandle;
  GNB_PCIE_INFORMATION_DATA_HOB   *PciePlatformConfigHob;


  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  GnbHandle = NULL;

  PeiServices = GetPeiServicesTablePointer();
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieServicesPpiGuid,
                             0,
                             NULL,
                             (VOID **)&PcieServicesPpi
                             );
  if (Status == EFI_SUCCESS) {
    PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
    Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);

    GnbHandle = NbioGetHandle (Pcie);
    while (GnbHandle != NULL) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "  down core\n");
      SmnRegisterWrite (GnbHandle->Address.Address.Bus, 0x5A860, &CoreDisMask, 0);
      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
      SmuArg[0] = 0xAA55AA55;
      Status = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_TestMessage, SmuArg, 0);
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a(%x) Exit\n", __FUNCTION__, Status);
  return Status;
}

/*----------------------------------------------------------------------------------------
/**
  disable SMT on all dies across the system. This routine should only be called from the BSP.

  Parameters:
  This
    A pointer to the PEI_AMD_NBIO_SMU_SERVICES_PPI instance.

  Status Codes Returned:
  EFI_SUCCESS           - SMT was successfully disabled.
  EFI_NOT_FOUND         - the PPI is not in the database.

**/
EFI_STATUS
SmuDisableSmt (
  IN       PEI_AMD_NBIO_SMU_SERVICES_PPI  *This
  )
{
  UINT32                                SmuArg[6];
  CONST EFI_PEI_SERVICES                **PeiServices;
  PEI_AMD_NBIO_PCIE_SERVICES_PPI        *PcieServicesPpi;
  PCIe_PLATFORM_CONFIG                  *Pcie;
  EFI_STATUS                            Status;
  GNB_HANDLE                            *GnbHandle;
  GNB_PCIE_INFORMATION_DATA_HOB         *PciePlatformConfigHob;
  THREAD_CONFIGURATION_STRUCT           ThreadConfiguration;

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  GnbHandle = NULL;

  PeiServices = GetPeiServicesTablePointer();
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gAmdNbioPcieServicesPpiGuid,
                             0,
                             NULL,
                             (VOID **)&PcieServicesPpi
                             );
  if (Status == EFI_SUCCESS) {
    PcieServicesPpi->PcieGetTopology (PcieServicesPpi, &PciePlatformConfigHob);
    Pcie = &(PciePlatformConfigHob->PciePlatformConfigHob);

    GnbHandle = NbioGetHandle (Pcie);
    while (GnbHandle != NULL) {
      IDS_HDT_CONSOLE (MAIN_FLOW, "  Disabling SMT\n");
      SmnRegisterWrite (GnbHandle->Address.Address.Bus, SMN_THREAD_CONFIGURATION_ADDRESS, &ThreadConfiguration, 0);
      ThreadConfiguration.Field.SMTDisable = 1;
      SmnRegisterWrite (
        GnbHandle->Address.Address.Bus,
        SMN_THREAD_CONFIGURATION_ADDRESS,
        &ThreadConfiguration.Value,
        0
        );
      NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
      SmuArg[0] = 0xAA55AA55;
      Status = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_TestMessage, SmuArg, 0);
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%A(%x) Exit\n", __FUNCTION__, Status);
  return Status;
}

/**
  Retrieve BIST info from SMU

  @param InstanceId  The unique identifier of the SMU instance associated with this socket (for SSP, the socket number)
  @param PhysicalCcx Number of the physical CCX
  @param BistInfo    Pointer to where BIST info should be copied

  @return EFI_STATUS Returns EFI_NOT_AVAILABLE_YET if unable to make SMU call. Returns EFI_INVALID_PARAMETER if null pointer passed in
                     and returns EFI_SUCCESS if call succeded and return argument is populated. Returns EFI_UNSUPPORTED for
                     SMU that do not support this call
**/
EFI_STATUS
SmuReadBistInfo (
  IN  UINT8       InstanceId,
  IN  UINT8       PhysicalCcx,
  OUT UINT32      *BistInfo
  )
{
  GNB_HANDLE                      *GnbHandle;
  EFI_STATUS                      Status;
  UINT32                          SmuArg[6];
  UINT32                          SmuResult;
  PRESIL_CTRL1                    PreSilCtrl1;

  PreSilCtrl1.Value = PcdGet32 (PcdAmdPreSilCtrl1);
  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 1) {
    return EFI_SUCCESS;
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%a Entry\n", __FUNCTION__);
  Status = SmuGetGnbHandle (InstanceId, &GnbHandle);
  if (Status == EFI_SUCCESS) {
    NbioSmuServiceCommonInitArgumentsV13 (SmuArg);
    SmuArg[0] = PhysicalCcx;
    SmuResult = NbioSmuServiceRequestV13 (NbioGetHostPciAddress (GnbHandle), BIOSSMC_MSG_GetPerSrcBistPF, SmuArg, 0);
    if (SmuResult == 1) {
      *BistInfo = SmuArg[0];
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_INVALID_PARAMETER;
      IDS_HDT_CONSOLE (MAIN_FLOW, "SmuRequestFail!!\n");
    }
  }

  IDS_HDT_CONSOLE (MAIN_FLOW, "%A(%x) Exit\n", __FUNCTION__, Status);
  return Status;
}

/*----------------------------------------------------------------------------------------
 *                          P P I   D E S C R I P T O R
 *----------------------------------------------------------------------------------------
 */

PEI_AMD_NBIO_SMU_SERVICES_PPI mNbioSmuServicePpi = {
  AMD_NBIO_SMU_SERVICES_REVISION,  ///< revision
  SmuReadFuseByName,
  SmuReadFuseByLocation,
  SmuServiceRequest,
  SmuDxioServiceRequest,
  SmuRegisterRead,
  SmuRegisterWrite,
  SmuRegisterRMW,
  SmuHtcControl,
  SmuReadCacWeights,
  SmuPsiControl,
  SmuGetTdpValue,
  SmuReadBrandString,
  SmuReadCorePllLockTimer,
  SmuSetDownCoreRegister,
  SmuDisableSmt,
  SmuReadBistInfo
};


