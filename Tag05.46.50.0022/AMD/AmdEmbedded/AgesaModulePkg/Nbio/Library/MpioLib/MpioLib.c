/**
 *  @file MpioLib.c
 *  @brief MPIO firmware messaging interface library
 */
/*
*****************************************************************************
*
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*******************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  <AGESA.h>
#include  <Gnb.h>
#include  <Filecode.h>
#include  <Library/GnbLib.h>
#include  <Library/IdsLib.h>
#include  <Library/AmdBaseLib.h>
#include  <Library/NbioHandleLib.h>
#include  <Library/AmdS3SaveLib.h>
#include  <MpioLib.h>
#include  <Library/SmnAccessLib.h>


#define FILECODE NBIO_LIBRARY_MPIOLIB_MPIOLIB_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#ifndef INVALID_MPIO_MSG
  #define INVALID_MPIO_MSG  0xFF
#endif
#ifndef BIOSMPIO_Result_FATAL
#define BIOSMPIO_Result_FATAL      ((UINT8)0xFC)     // i.e. -4
#endif
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#ifndef D0F0xB8_ADDRESS
  // **** D0F0xB8 Register Definition ****
  // Address
  #define D0F0xB8_ADDRESS          0xB8
#endif

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------*/
/*
 *  Routine to Init Arguments
 *
 *
 *
 * @param[in, out]  SmuArg Request Argument
 */
VOID
NbioMpioServiceCommonInitArguments (
  IN OUT   UINT32                   *MpioArg
  )
{
  LibAmdMemFill (MpioArg, 0x00, 24, NULL);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Mpio service request
 *
 *
 * @param[in]      NbioPciAddress  PCI_ADDR of this NBIO
 * @param[in]      RequestId       Request ID
 * @param[in]      RequestArgument Request Argument
 * @param[in]      AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 */

UINT32
MpioServiceRequest (
  IN       PCI_ADDR                 NbioPciAddress,
  IN       UINT32                   RequestId,
  IN OUT   UINT32                   *RequestArgument,
  IN       UINT32                   AccessFlags
  )
{
 UINT32                      RxMpioMessageResponse;
 UINT32                      DataMask;
 UINT32                      Address;
 UINT32                      DoorBellValue;

 IDS_HDT_CONSOLE (GNB_TRACE, "MpioServiceRequest Enter\n");

 DataMask = 0xFFFFFFFF;
 DoorBellValue = 0xFFFFFFFF;

 if (RequestId == INVALID_MPIO_MSG) {
   IDS_HDT_CONSOLE (GNB_TRACE, "MPIOServiceRequest INVALID!!\n");
   return BIOSMPIO_Result_OK;
 }

 IDS_HDT_CONSOLE (GNB_TRACE, "  MPIO Service Request 0x%x\n", RequestId);
 IDS_HDT_CONSOLE (GNB_TRACE, "  MPIO Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n", RequestArgument[0], RequestArgument[1], RequestArgument[2], RequestArgument[3], RequestArgument[4], RequestArgument[5]);


 // 1 Waiting MPIO until ready (mmMPIO_C2PMSG_30 bit31)
 do {
   SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_RESPONSE_ADDRESS, &RxMpioMessageResponse);
   RxMpioMessageResponse = (RxMpioMessageResponse & BIT31);
 } while (RxMpioMessageResponse == 0x0);

 // 2 Write message arguments (Write cmd params to mmMPIO_C2PMSG_31-36)
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_0_ADDRESS, &RequestArgument[0], AccessFlags);
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_1_ADDRESS, &RequestArgument[1], AccessFlags);
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_2_ADDRESS, &RequestArgument[2], AccessFlags);
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_3_ADDRESS, &RequestArgument[3], AccessFlags);
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_4_ADDRESS, &RequestArgument[4], AccessFlags);
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_5_ADDRESS, &RequestArgument[5], AccessFlags);

  // 3 Write message ID (Write cmd ID to mmMPIO_C2PMSG_30 with Ready_flag cleare)
  RequestId = (RequestId & (0xFF)) << 8;
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_RESPONSE_ADDRESS, &RequestId, AccessFlags);

  // 4 Write value to mmMPIO_C2PMSG_21 doorbell for MPIO to begin work
  SmnRegisterWriteS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_DOORBELL_ADDRESS, &DoorBellValue, AccessFlags);

  // 5 Waiting MPIO until ready (mmMPIO_C2PMSG_30 bit31)
  do {
    SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_RESPONSE_ADDRESS, &RxMpioMessageResponse);
  } while ((RxMpioMessageResponse & BIT31) == 0x0);

///@todo - implement S3 save for resume
  if (AccessFlags) {
    Address = MPIO_C2PMSG_RESPONSE_ADDRESS;
    AmdS3SaveScriptPciWrite (AccessWidth32, NbioPciAddress.AddressValue | 0xB8, &Address);
    DataMask = 0xFFFFFFFF;
    AmdS3SaveScriptPciPoll (AccessWidth32, NbioPciAddress.AddressValue | 0xBC, &RxMpioMessageResponse, &DataMask, 0xffffffff);
  }
  // 6 Read updated MPIO message arguments
  SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_0_ADDRESS, &RequestArgument[0]);
  SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_1_ADDRESS, &RequestArgument[1]);
  SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_2_ADDRESS, &RequestArgument[2]);
  SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_3_ADDRESS, &RequestArgument[3]);
  SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_4_ADDRESS, &RequestArgument[4]);
  SmnRegisterReadS (NbioPciAddress.Address.Segment, NbioPciAddress.Address.Bus, MPIO_C2PMSG_ARGUMENT_5_ADDRESS, &RequestArgument[5]);

  // If the status from MPIO is not okay
  if ( BIOSMPIO_Result_OK != (RxMpioMessageResponse & MPIO_C2PMSG_RESPONSE_STATUS_MASK))
  {
    IDS_HDT_CONSOLE (GNB_TRACE, "  MPIO ERROR! Error ID: 0x%x\n", RequestArgument[0]);
  }

  // 7. Read the response mmMPIO_C2PMSG_30 [15:0] and return function
  IDS_HDT_CONSOLE (GNB_TRACE, "MpioServiceRequest Exit\n");
  return (RxMpioMessageResponse);

}




