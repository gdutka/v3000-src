/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>

#define XHCI_AZALIA_BRIDGE_DEVICE       8
#define XHCI_AZALIA_BRIDGE_FUNCTION     1
#define USB2_USB3_USB4_BRIDGE_FUNCTION  3
#define USB4_RT0_FUNCTION               5
#define USB4_RT1_FUNCTION               6
#define TEMP_BUS_NUMBER                 0x1F

#define PCIE_TUNNELING_BRIDGE_DEVICE_1  3
#define PCIE_TUNNELING_BRIDGE_DEVICE_2  4
#define PCIE_TUNNELING_BRIDGE_FUNCTION  1

/*----------------------------------------------------------------------------------------*/
/**
 *  Callback function to override GPIO SSDT Table.
 *
 * @param[in]     This            Pointer to Protocol
 * @param[in]     AmlObjPtr       The AML Object Buffer
 * @param[in]     Context         The Parameter Buffer
 *
 * @retval        TRUE            SSDT Table has been updated completely
 * @retval        FALSE           SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmOemGpioSsdtCallback (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*) Context;

  switch (*((UINT32 *) AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('E', 'C', '0', '_')):
    if (*BufferPtr) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) BufferPtr;
      DEBUG ((DEBUG_INFO, "\t%a(%d) EC0  is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '0', '4', '2')):     // CpmKbcMutex: M042
    if (*(BufferPtr + 1)) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 1);
      DEBUG ((DEBUG_INFO, "\t%a(%d) M042 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('L', 'P', 'C', '0')):
    if (*(BufferPtr + 2)) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 2);
      DEBUG ((DEBUG_INFO, "\t%a(%d) LPC0 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('O', 'K', 'E', 'C')):
    if (*(BufferPtr + 3)) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 3);
      DEBUG ((DEBUG_INFO, "\t%a(%d) OKEC is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('H', 'P', 'D', 'W')):
    if (*(BufferPtr + 4)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 4));
        DEBUG ((DEBUG_INFO, "\t%a(%d) HPDW is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override Internal GPP [0:8:1] SSDT Table
 *
 * @param[in]     This            Pointer to Protocol
 * @param[in]     AmlObjPtr       The AML Object Buffer
 * @param[in]     Context         The Parameter Buffer
 *
 * @retval        TRUE            SSDT Table has been updated completely
 * @retval        FALSE           SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmOemIntGpp01SsdtCallback (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32 *) Context;

  switch (*((UINT32 *) AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('G', 'P', '1', '7')):
    if ((*(BufferPtr + 0)) && (*(BufferPtr + 0) != CPM_SIGNATURE_32 ('G', 'P', '1', '7'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 0);
      DEBUG ((DEBUG_INFO, "\t%a(%d) GP17 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('V', 'G', 'A', '_')):
    if ((*(BufferPtr + 1)) && (*(BufferPtr + 1) != CPM_SIGNATURE_32 ('V', 'G', 'A', '_'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 1);
      DEBUG ((DEBUG_INFO, "\t%a(%d) VGA is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('H', 'D', 'A', 'U')):
    if ((*(BufferPtr + 2)) && (*(BufferPtr + 2) != CPM_SIGNATURE_32 ('H', 'D', 'A', 'U'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 2);
      DEBUG ((DEBUG_INFO, "\t%a(%d) HDAU is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'H', 'C', '0')):
    if ((*(BufferPtr + 3)) && (*(BufferPtr + 3) != CPM_SIGNATURE_32 ('X', 'H', 'C', '0'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 3);
      DEBUG ((DEBUG_INFO, "\t%a(%d) XHC0 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'H', 'C', '1')):
    if ((*(BufferPtr + 4)) && (*(BufferPtr + 4) != CPM_SIGNATURE_32 ('X', 'H', 'C', '1'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 4);
      DEBUG ((DEBUG_INFO, "\t%a(%d) XHC1 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('A', 'C', 'P', '_')):
    if ((*(BufferPtr + 5)) && (*(BufferPtr + 5) != CPM_SIGNATURE_32 ('A', 'C', 'P', '_'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 5);
      DEBUG ((DEBUG_INFO, "\t%a(%d) ACP is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('A', 'Z', 'A', 'L')):
    if ((*(BufferPtr + 6)) && (*(BufferPtr + 6) != CPM_SIGNATURE_32 ('A', 'Z', 'A', 'L'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 6);
      DEBUG ((DEBUG_INFO, "\t%a(%d) AZAL is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('P', '1', 'D', '3'):
    if (*(BufferPtr + 7)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 7));
        DEBUG ((DEBUG_INFO, "\t%a(%d) P1D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('Z', 'S', '0', 'W'):
    if (*(BufferPtr + 7)) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'S', '0', 'W');
      DEBUG ((DEBUG_INFO, "\t%a(%d) ZS0W is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('Z', 'P', 'R', 'W'):
    if (*(BufferPtr + 7)) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'P', 'R', 'W');
      DEBUG ((DEBUG_INFO, "\t%a(%d) ZPRW is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('F', '0', 'D', '3'):
    if (*(BufferPtr + 8)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 8));
        DEBUG ((DEBUG_INFO, "\t%a(%d) F0D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('F', '3', 'D', '3'):
    if (*(BufferPtr + 9)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 9));
        DEBUG ((DEBUG_INFO, "\t%a(%d) F3D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('F', '1', 'D', '3'):
    if (*(BufferPtr + 10)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8*) AmlObjPtr) + 5) = (UINT8)(*(BufferPtr + 10));
        DEBUG ((DEBUG_INFO, "\t%a(%d) F1D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('F', '5', 'D', '3'):
    if (*(BufferPtr + 11)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8*) AmlObjPtr) + 5) = (UINT8)(*(BufferPtr + 11));
        DEBUG ((DEBUG_INFO, "\t%a(%d) F5D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('F', '6', 'D', '3'):
    if (*(BufferPtr + 12)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8*) AmlObjPtr) + 5) = (UINT8)(*(BufferPtr + 12));
        DEBUG ((DEBUG_INFO, "\t%a(%d) F6D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('F', '5', 'W', 'V'):
    if (*(BufferPtr + 13)) {
      if (*(((UINT8*) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8*) AmlObjPtr) + 5) = (UINT8)(*(BufferPtr + 13));
        DEBUG ((DEBUG_INFO, "\t%a(%d) F5WV is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override Internal GPP [0:8:3] SSDT Table
 *
 * @param[in]     This            Pointer to Protocol
 * @param[in]     AmlObjPtr       The AML Object Buffer
 * @param[in]     Context         The Parameter Buffer
 *
 * @retval        TRUE            SSDT Table has been updated completely
 * @retval        FALSE           SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmOemIntGpp03SsdtCallback (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32 *) Context;

  switch (*((UINT32 *) AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('G', 'P', '1', '9')):
    if ((*(BufferPtr + 0)) && (*(BufferPtr + 0) != CPM_SIGNATURE_32 ('G', 'P', '1', '9'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 0);
      DEBUG ((DEBUG_INFO, "\t%a(%d) GP19 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'H', 'C', '2')):
    if ((*(BufferPtr + 1)) && (*(BufferPtr + 1) != CPM_SIGNATURE_32 ('X', 'H', 'C', '2'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 1);
      DEBUG ((DEBUG_INFO, "\t%a(%d) XHC2 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'H', 'C', '3')):
    if ((*(BufferPtr + 2)) && (*(BufferPtr + 2) != CPM_SIGNATURE_32 ('X', 'H', 'C', '3'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 2);
      DEBUG ((DEBUG_INFO, "\t%a(%d) XHC3 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'H', 'C', '4')):
    if ((*(BufferPtr + 3)) && (*(BufferPtr + 3) != CPM_SIGNATURE_32 ('X', 'H', 'C', '4'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 3);
      DEBUG ((DEBUG_INFO, "\t%a(%d) XHC4 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('P', '3', 'D', '3'):
    if (*(BufferPtr + 4)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 4));
        DEBUG ((DEBUG_INFO, "\t%a(%d) P3D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('Y', 'S', '0', 'W'):
    if (*(BufferPtr + 4)) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'S', '0', 'W');
      DEBUG ((DEBUG_INFO, "\t%a(%d) YS0W is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('Y', 'P', 'R', 'W'):
    if (*(BufferPtr + 4)) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'P', 'R', 'W');
      DEBUG ((DEBUG_INFO, "\t%a(%d) YPRW is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('G', '0', 'D', '3'):
    if (*(BufferPtr + 5)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 5));
        DEBUG ((DEBUG_INFO, "\t%a(%d) G0D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('G', '3', 'D', '3'):
    if (*(BufferPtr + 6)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 6));
        DEBUG ((DEBUG_INFO, "\t%a(%d) G3D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;

  case CPM_SIGNATURE_32 ('G', '5', 'D', '3'):
    if (*(BufferPtr + 7)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 7));
        DEBUG ((DEBUG_INFO, "\t%a(%d) G5D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('R', 'C', 'R', 'S'):
    if (*(BufferPtr + 8)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 8));
        DEBUG ((DEBUG_INFO, "\t%a(%d) RCRS is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('P', '4', 'D', '3'):
    if (*(BufferPtr + 9)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 9));
        DEBUG ((DEBUG_INFO, "\t%a(%d) P4D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('H', '0', 'D', '3'):
    if (*(BufferPtr + 10)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 10));
        DEBUG ((DEBUG_INFO, "\t%a(%d) H0D3 is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case (CPM_SIGNATURE_32 ('G', 'P', '1', '1')):
    if ((*(BufferPtr + 11)) && (*(BufferPtr + 11) != CPM_SIGNATURE_32 ('G', 'P', '1', '1'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 11);
      DEBUG ((DEBUG_INFO, "\t%a(%d) GP11 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('G', 'P', '1', '2')):
    if ((*(BufferPtr + 12)) && (*(BufferPtr + 12) != CPM_SIGNATURE_32 ('G', 'P', '1', '2'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 12);
      DEBUG ((DEBUG_INFO, "\t%a(%d) GP12 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('N', 'H', 'I', '0')):
    if ((*(BufferPtr + 13)) && (*(BufferPtr + 13) != CPM_SIGNATURE_32 ('N', 'H', 'I', '0'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 13);
      DEBUG ((DEBUG_INFO, "\t%a(%d) NHI0 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('N', 'H', 'I', '1')):
    if ((*(BufferPtr + 14)) && (*(BufferPtr + 14) != CPM_SIGNATURE_32 ('N', 'H', 'I', '1'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 14);
      DEBUG ((DEBUG_INFO, "\t%a(%d) NHI1 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('R', 'H', 'U', 'B')):
    if ((*(BufferPtr + 15)) && (*(BufferPtr + 15) != CPM_SIGNATURE_32 ('R', 'H', 'U', 'B'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 15);
      DEBUG ((DEBUG_INFO, "\t%a(%d) RHUB is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('P', 'R', 'T', '2')):
    if ((*(BufferPtr + 16)) && (*(BufferPtr + 16) != CPM_SIGNATURE_32 ('P', 'R', 'T', '2'))) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 16);
      DEBUG ((DEBUG_INFO, "\t%a(%d) PRT2 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('D', 'I', 'M', 'S'):
    if (*(BufferPtr + 17)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 17));
        DEBUG ((DEBUG_INFO, "\t%a(%d) DIMS is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('D', 'D', 'E', 'P'):
    if (*(BufferPtr + 18)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 18));
        DEBUG ((DEBUG_INFO, "\t%a(%d) DDEP is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('C', 'I', 'M', 'S'):
    if (*(BufferPtr + 19)) {
      if (*(((UINT8 *) AmlObjPtr) - 1) == AML_NAME_OP) {
        *(((UINT8 *) AmlObjPtr) + 5) = (UINT8) (*(BufferPtr + 19));
        DEBUG ((DEBUG_INFO, "\t%a(%d) CIMS is updated\n", __FUNCTION__, __LINE__));
      }
    }
    break;
  case CPM_SIGNATURE_32 ('X', 'D', 'S', 'D'):
    if (*(BufferPtr + 20) == 0x1022) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'D', 'S', 'D');
      DEBUG ((DEBUG_INFO, "\t%a(%d) XDSD is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case CPM_SIGNATURE_32 ('Y', 'D', 'S', 'D'):
    if (*(BufferPtr + 21) == 0x1022) {
      *((UINT32*)AmlObjPtr) = CPM_SIGNATURE_32 ('_', 'D', 'S', 'D');
      DEBUG ((DEBUG_INFO, "\t%a(%d) YDSD is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Callback function to override Thermal SSDT Table.
 *
 * @param[in]     This            Pointer to Protocol
 * @param[in]     AmlObjPtr       The AML Object Buffer
 * @param[in]     Context         The Parameter Buffer
 *
 * @retval        TRUE            SSDT Table has been updated completely
 * @retval        FALSE           SSDT Table has not been updated completely
 */
BOOLEAN
EFIAPI
AmdCpmOemThermalSsdtCallback (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;
  BufferPtr = (UINT32*) Context;

  switch (*((UINT32 *) AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('E', 'C', '0', '_')):
    if (*BufferPtr) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) BufferPtr;
      DEBUG ((DEBUG_INFO, "\t%a(%d) EC0  is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '0', '4', '2')):     // CpmKbcMutex: M042
    if (*(BufferPtr + 1)) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 1);
      DEBUG ((DEBUG_INFO, "\t%a(%d) M042 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('L', 'P', 'C', '0')):
    if (*(BufferPtr + 2)) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 2);
      DEBUG ((DEBUG_INFO, "\t%a(%d) LPC0 is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  case (CPM_SIGNATURE_32 ('O', 'K', 'E', 'C')):
    if (*(BufferPtr + 3)) {
      *(UINT32 *) AmlObjPtr = *(UINT32 *) (BufferPtr + 3);
      DEBUG ((DEBUG_INFO, "\t%a(%d) OKEC is updated\n", __FUNCTION__, __LINE__));
    }
    break;
  }

  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM GPIO SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event           EFI_EVENT
 * @param[in]     Context         The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemGpioSsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT64                          OemTableIdSig;
  UINT32                          Buffer[5];

  if (!InitlateInvoked) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    OemTableIdSig = SIGNATURE_64 ('C','P','M','G','P','I','O','0');
    Buffer[0] = CFG_AMD_CPM_ASL_EC_NAME;                      // EC0  PcdGet32 (EcAslName)
    Buffer[1] = CFG_AMD_CPM_ASL_EC_MUTEX;                     // M042 PcdGet32 (EcMutexAslName)
    Buffer[2] = CFG_AMD_CPM_ASL_LPC_NAME;                     // LPC0 PcdGet32 (LpcAslName)
    Buffer[3] = CFG_AMD_CPM_ASL_OKEC_NAME;                    // OKEC PcdGet32 (OkEcAslName)
    Buffer[4] = (UINT32) AmdPbsConfiguration.WakeOnHpdWa;     // HPDW WakeOnHumanPresenceDetection
    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                        &gEfiCallerIdGuid,
                                                        &OemTableIdSig,
                                                        AmdCpmOemGpioSsdtCallback,
                                                        &Buffer[0]);

    InitlateInvoked = TRUE;
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));
  } // End of if (!InitlateInvoked)

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM Internal GPP [0:8:1] SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event           EFI_EVENT
 * @param[in]     Context         The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemIntGpp01SsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT64                          OemTableIdSig;
  UINT32                          Buffer[14];

  if (!InitlateInvoked) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    OemTableIdSig = SIGNATURE_64 ('I','N','T','G','P','P','0','1');
    ZeroMem (Buffer, sizeof (Buffer));
    Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                      XHCI_AZALIA_BRIDGE_DEVICE,
                                                                      XHCI_AZALIA_BRIDGE_FUNCTION); // GP17
    Buffer[1] = PcdGet32 (IGpuAslName);                       // VGA
    Buffer[2] = PcdGet32 (HdauAslName);                       // HDAU
    Buffer[3] = PcdGet32 (Xhci0AslName);                      // XHC0
    Buffer[4] = PcdGet32 (Xhci1AslName);                      // XHC1
    Buffer[5] = PcdGet32 (AcpAslName);                        // ACP
    Buffer[6] = PcdGet32 (AzaliaAslName);                     // AZAL
    Buffer[7] = (UINT32) AmdPbsConfiguration.IntPcieGpp0D3;   // P1D3, ZS0W and ZPRW
    Buffer[8] = (UINT32) AmdPbsConfiguration.SocGpuD3;        // F0D3
    Buffer[9] = (UINT32) AmdPbsConfiguration.SocUsb31D3;      // F3D3
    Buffer[10] = (UINT32) AmdPbsConfiguration.SocHdAudioD3;   // F1D3
    Buffer[11] = (UINT32) AmdPbsConfiguration.SocAcpD3;       // F5D3
    Buffer[12] = (UINT32) AmdPbsConfiguration.SocAzaliaD3;    // F6D3
    Buffer[13] = (UINT32) AmdPbsConfiguration.WoV;            // F5WV
    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                        &gEfiCallerIdGuid,
                                                        &OemTableIdSig,
                                                        AmdCpmOemIntGpp01SsdtCallback,
                                                        &Buffer[0]);

    InitlateInvoked = TRUE;
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));
  } // End of if (!InitlateInvoked)

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM Internal GPP [0:8:3] SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event           EFI_EVENT
 * @param[in]     Context         The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemIntGpp03SsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  UINT64                          OemTableIdSig;
  UINT32                          Buffer[22];
  UINT32                          OriginalBusNum;
  UINT8                           SecBusNum;

  if (!InitlateInvoked) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    OemTableIdSig = SIGNATURE_64 ('I','N','T','G','P','P','0','3');
    ZeroMem (Buffer, sizeof (Buffer));
    Buffer[0] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (  CpmTableProtocolPtr,
                                                                      XHCI_AZALIA_BRIDGE_DEVICE,
                                                                      USB2_USB3_USB4_BRIDGE_FUNCTION);  // GP19
    Buffer[1] = PcdGet32 (Xhci2AslName);                                  // XHC2
    Buffer[2] = PcdGet32 (Xhci3AslName);                                  // XHC3
    Buffer[3] = PcdGet32 (Xhci4AslName);                                  // XHC4
    Buffer[4] = (UINT32) AmdPbsConfiguration.IntPcieGpp2D3;               // P3D3, YS0W and YPRW
    Buffer[5] = (UINT32) AmdPbsConfiguration.SocUsb20D3;                  // G0D3
    Buffer[6] = (UINT32) AmdPbsConfiguration.SocUsb31ForUsb4D3;           // G3D3
    Buffer[7] = (UINT32) AmdPbsConfiguration.SocUsb4D3;                   // G5D3
    Buffer[8] = (UINT32) AmdPbsConfiguration.ReconfigRebalanceResources;  // RCRS
    Buffer[9] = (UINT32) AmdPbsConfiguration.IntUsb4PcieTunnelingD3;      // P4D3, XS0W and XPRW
    Buffer[10] = (UINT32) AmdPbsConfiguration.SocUsb4PcieEndpointD3;      // H0D3
    Buffer[11] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName ( CpmTableProtocolPtr,
                                                                      PCIE_TUNNELING_BRIDGE_DEVICE_1,
                                                                      PCIE_TUNNELING_BRIDGE_FUNCTION);  // GP11
    Buffer[12] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName ( CpmTableProtocolPtr,
                                                                      PCIE_TUNNELING_BRIDGE_DEVICE_2,
                                                                      PCIE_TUNNELING_BRIDGE_FUNCTION);  // GP12
    Buffer[13] = PcdGet32 (Usb4Rt0AslName);                               // NHI0
    Buffer[14] = PcdGet32 (Usb4Rt1AslName);                               // NHI1
    Buffer[15] = PcdGet32 (Xhci3Xhci4RootHubAslName);                     // RHUB
    Buffer[16] = PcdGet32 (Xhci3Xhci4Usb3PortAslName);                    // PRT2
    if (PcdGet8 (SwSmiCmdUsb4DepSupport) != 0xCF) {
      Buffer[17] = (UINT32) PcdGet8 (SwSmiCmdUsb4DepSupport);             // DIMS
    }
    Buffer[18] = (UINT32) AmdPbsConfiguration.Usb4DepSupport;             // DDEP
    if (PcdGet8 (SwSmiCmdUsb4PSxSupport) != 0xCE) {
      Buffer[19] = (UINT32) PcdGet8 (SwSmiCmdUsb4PSxSupport);             // CIMS
    }

    OriginalBusNum = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, 0, XHCI_AZALIA_BRIDGE_DEVICE, USB2_USB3_USB4_BRIDGE_FUNCTION, 0x18);
    if ((OriginalBusNum & 0x00FFFF00) == 0) {
      CpmTableProtocolPtr->CommonFunction.PciWrite8 (CpmTableProtocolPtr, 0, XHCI_AZALIA_BRIDGE_DEVICE, USB2_USB3_USB4_BRIDGE_FUNCTION, 0x19, TEMP_BUS_NUMBER);
      CpmTableProtocolPtr->CommonFunction.PciWrite8 (CpmTableProtocolPtr, 0, XHCI_AZALIA_BRIDGE_DEVICE, USB2_USB3_USB4_BRIDGE_FUNCTION, 0x1A, TEMP_BUS_NUMBER);
      SecBusNum = TEMP_BUS_NUMBER;
    } else {
      SecBusNum = (UINT8) (OriginalBusNum >> 8);
    }

    Buffer[20] = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, SecBusNum, 0, USB4_RT0_FUNCTION, 0x00) & 0xFFFF;
    Buffer[21] = CpmTableProtocolPtr->CommonFunction.PciRead32 (CpmTableProtocolPtr, SecBusNum, 0, USB4_RT1_FUNCTION, 0x00) & 0xFFFF;

    if ((Buffer[20] != 0x1022) && (Buffer[21] != 0x1022)) {
      if (Buffer[18] != 0) {
        // USB4 Router 0 and USB4 Router 1 do not exist
        DEBUG ((DEBUG_INFO, "\tUSB4 RT0 & RT1 do not exist, override USB4 ACPI _DEP Support to Disabled\n"));
        Buffer[18] = 0x00;                                                // DDEP
      }
    }

    if ((OriginalBusNum & 0x00FFFF00) == 0) {
      CpmTableProtocolPtr->CommonFunction.PciWrite32 (CpmTableProtocolPtr, 0, XHCI_AZALIA_BRIDGE_DEVICE, USB2_USB3_USB4_BRIDGE_FUNCTION, 0x18, OriginalBusNum);
    }

    DEBUG ((DEBUG_INFO, "\tUsb4 D3 enable: %d\n", Buffer[7]));
    DEBUG ((DEBUG_INFO, "\tRCRS: %d\n", Buffer[8]));
    DEBUG ((DEBUG_INFO, "\tUSB4 PCIe Tunneling D3 = %d\n", Buffer[9]));
    DEBUG ((DEBUG_INFO, "\tUSB4 USB4 Endpoints D3 = %d\n", Buffer[10]));
    DEBUG ((DEBUG_INFO, "\tUSB4 ACPI _DEP Support = %d\n", Buffer[18]));
    DEBUG ((DEBUG_INFO, "\tSW SMI 0x%02X for USB4 _DEP\n", PcdGet8 (SwSmiCmdUsb4DepSupport)));

    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                        &gEfiCallerIdGuid,
                                                        &OemTableIdSig,
                                                        AmdCpmOemIntGpp03SsdtCallback,
                                                        &Buffer[0]);

    InitlateInvoked = TRUE;
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));
  } // End of if (!InitlateInvoked)

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM OEM Thermal SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event           EFI_EVENT
 * @param[in]     Context         The Parameter Buffer
 *
 */
VOID
EFIAPI
AmdCpmOemThermalSsdtInstall (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  UINT64                          OemTableIdSig;
  UINT32                          Buffer[4];

  if (!InitlateInvoked) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
      return;
    }

    OemTableIdSig = SIGNATURE_64 ('T','H','E','R','M','A','L','0');
    Buffer[0] = CFG_AMD_CPM_ASL_EC_NAME;                      // EC0  PcdGet32 (EcAslName)
    Buffer[1] = CFG_AMD_CPM_ASL_EC_MUTEX;                     // M042 PcdGet32 (EcMutexAslName)
    Buffer[2] = CFG_AMD_CPM_ASL_LPC_NAME;                     // LPC0 PcdGet32 (LpcAslName)
    Buffer[3] = CFG_AMD_CPM_ASL_OKEC_NAME;                    // OKEC PcdGet32 (OkEcAslName)
    CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  (VOID*)CpmTableProtocolPtr,
                                                        &gEfiCallerIdGuid,
                                                        &OemTableIdSig,
                                                        AmdCpmOemThermalSsdtCallback,
                                                        &Buffer[0]);

    InitlateInvoked = TRUE;
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a-End\n", __FUNCTION__));
  } // End of if (!InitlateInvoked)

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM OEM ACPI driver
 *
 * @param[in]     ImageHandle     Pointer to the firmware file system header
 * @param[in]     SystemTable     Pointer to System table
 *
 * @retval        EFI_SUCCESS     Module initialized successfully
 * @retval        EFI_ERROR       Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmOemAcpiEntryPoint (
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       ReadyToBootEvent;

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a-Start\n", __FUNCTION__));
  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  // Install GPIO SSDT
  Status = gBS->CreateEventEx ( CPM_EVENT_NOTIFY_SIGNAL,
                                CPM_TPL_CALLBACK,
                                AmdCpmOemGpioSsdtInstall,
                                NULL,
                                &gEfiEventReadyToBootGuid,
                                &ReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  // Install Internal GPP 01 SSDT
  Status = gBS->CreateEventEx ( CPM_EVENT_NOTIFY_SIGNAL,
                                CPM_TPL_CALLBACK,
                                AmdCpmOemIntGpp01SsdtInstall,
                                NULL,
                                &gEfiEventReadyToBootGuid,
                                &ReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  // Install Internal GPP 03 SSDT
  Status = gBS->CreateEventEx ( CPM_EVENT_NOTIFY_SIGNAL,
                                CPM_TPL_CALLBACK,
                                AmdCpmOemIntGpp03SsdtInstall,
                                NULL,
                                &gEfiEventReadyToBootGuid,
                                &ReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-ERROR-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  // Install Thermal SSDT
  Status = gBS->CreateEventEx ( CPM_EVENT_NOTIFY_SIGNAL,
                                CPM_TPL_CALLBACK,
                                AmdCpmOemThermalSsdtInstall,
                                NULL,
                                &gEfiEventReadyToBootGuid,
                                &ReadyToBootEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "OEM-DXE-%a(%d)-End-Status = EFI_SUCCESS\n", __FUNCTION__, __LINE__));
  return EFI_SUCCESS;
}

