/** @file
  Source file for LegacyBiosPlatformHookDxe driver.

;*******************************************************************************
;* Copyright (c) 2013 - 2018, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#include <Uefi.h>
#include <IndustryStandard/Pci.h>

#include <Guid/GlobalVariable.h>

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DevicePathLib.h>
#include <Library/ChipsetConfigLib.h>

#include <Protocol/LegacyBios.h>
#include <Protocol/LegacyBiosPlatform.h>
#include <Protocol/PciIo.h>
#include <Protocol/LegacyInterrupt.h>
#include <Protocol/OemServices.h>
#include <Protocol/DevicePath.h>

#include <ChipsetSetupConfig.h>

#include <Library/DxeChipsetSvcLib.h>
#include <IrqRoutingInformation.h>

#define MaxPIRQIndex                    8
#define PCI_UNUSED                      0x00
#define PCI_USED                        0xFF
#define LEGACY_USED                     0xFE
#define ROM_FOUND                       0x01
#define VALID_LEGACY_ROM                0x02

typedef enum {
  COMMON_DMI_SWITCH_TABLE,       //0
  COMMON_BOOTMANGER_KEY_TABLE,   //1
  COMMON_POST_KEY_TABLE,         //2
  COMMON_PCI_SKIP_TABLE_INDEX,   //3
  COMMON_LEGACY_BIOS_OEM_SLP,    //4
  COMMON_GET_MAX_CHECK_COUNT,    //5
  COMMON_ERROR_PASSWORD_CALL_BACK, //6
  COMMON_PASSWORD_ENTRY_CALL_BACK, //7
  COMMON_UPDATE_SCU_SYSTEM_INFO,   //8
  COMMON_SCU_CALLBACK_FUNCTION,    //9
  COMMON_CHECK_VGA_ORDER,          //10
  COMMON_DISPLAY_LOGO,             //11
  COMMON_CALCULATE_WRITE_CMOS_CHECKSUM, //12
  COMMON_CSM16_REFER_SWITCH,            //13
  COMMON_UPDATE_SLP20_PUBKEY_AND_SLP_MARKER, //14
  COMMON_UPDATE_MSDM_DATA,                   //15
  COMMON_FORM_LENGTH_UDPATE,                 //16
  COMMON_GET_MULTI_SIO_RESOURCE_TABLE,       //17
  COMMON_LOAD_DEFAULT_SETUP_MENU,            //18
  COMMON_LOGO_RESOLUTION,                    //19
  COMMON_IRQ_ROUTING_TABLE,                  //20
  COMMON_OPTION_ROM_TABLE,                   //21
  COMMON_UNSKIP_PCI_DEVICE,                  //22
  COMMON_GET_HOTPLUG_BRIDGE_INFO,            //23
  COMMON_UNLOAD_PCI_OPTION_ROM,              //24
  COMMON_BOOT_DISPLAY_DEVICE_REPLACE,        //25
  COMMON_PREPARE_INSTALL_MP_TABLE,           //26
  COMMON_GET_SIO_RESOURCE_TABLE,             //27
  COMMON_INSTALL_PCI_ROM_SWITCH_TEXT_MODE,   //28
  COMMON_DISPLAY_SPI_NOT_SUPPORT,
  COMMON_UPDATE_BBS_TABLE,
  COMMON_GET_SSID_SVID_INFO_CALLBACK,
  COMMON_PNP_DXE_UPDATE_STRING_TABLE,
  COMMON_PNP_DXE_GPNV_HANDLE_TABLE,
  COMMON_VARIABLE_RESERVED_TABLE,
  RUNTIME_MAX_NUM_COMMON_SUPPORT
} OEM_SERVICES_RUNTIME_COMMON_SUPPORT;


typedef struct {
  UINTN       BridgeBus;
  UINTN       BridgeDevice;
  UINTN       BridgeFunction;
  UINT8       InitialBus;
} EFI_LEGACY_MODIFY_PIR_TABLE;

typedef struct {
  UINT8 PirqNum;
  UINT8 CsReg;
} CHIPSET_DEV_IRQ_ROUTING_ENTRY;

#ifdef  RAID_OPTION_ROM

#define PMM_SIGNATURE  0x4D4D5024
#define MISC_SIGNATURE 0x4353494D
BOOLEAN                           gFindPMM = FALSE;

//
// stack of PMM function argment
//
typedef struct {
  UINT16  Function;
  UINT16  LengthL;
  UINT16  LengthH;
  UINT16  HandleL;
  UINT16  HandleH;
  UINT16  Flags;
} PMM_FUN_ARG;

#endif //#ifdef  RAID_OPTION_ROM

//
// The IRQ listed will be choosed to set PIRQA~PIRQH if they are not used by legacy
//
EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY IrqPriorityTable[] = {
    {03, PCI_UNUSED},  \
  {04, PCI_UNUSED},  \
  {05, PCI_UNUSED},  \
  {07, PCI_UNUSED},  \
  {10, PCI_UNUSED},  \
  {11, PCI_UNUSED},  \
  {00, PCI_UNUSED},  \
};

//
// The IRQ listed will be selected to set PIRQA~PIRQH if they are not used by legacy
//
EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY SetPIrqPriority[MaxPIRQIndex] = {
  {00, PCI_UNUSED},  /* PIRQ A */  \
  {00, PCI_UNUSED},  /* PIRQ B */  \
  {00, PCI_UNUSED},  /* PIRQ C */  \
  {00, PCI_UNUSED},  /* PIRQ D */  \
  {00, PCI_UNUSED},  /* PIRQ E */  \
  {00, PCI_UNUSED},  /* PIRQ F */  \
  {00, PCI_UNUSED},  /* PIRQ G */  \
  {00, PCI_UNUSED},  /* PIRQ H */  \
};

// FCH internal PCI devices IRQ logic mapping
CHIPSET_DEV_IRQ_ROUTING_ENTRY      mCsDevRoutingMap[] = {
  {0, 0x13},        // HD audio-> INTA#
  {2, 0x30},        // Dev18 (USB) IntA#-> INTC#
  {1, 0x31},        // Dev18 (USB) IntB# -> INTB#
  {2, 0x32},        // Dev19 (USB) IntA# -> INTC#
  {1, 0x33},        // Dev19 (USB) IntB#-> INTB#
  {2, 0x34},        // Dev22 (USB) IntA#-> INTC#
  {1, 0x35},        // Dev22 (USB) IntB#-> INTB#
  {2, 0x36},        // Dev20 (USB) IntC#-> INTC#
  {1, 0x40},        // IDE pci interrupt-> INTB#
  {3, 0x41},        // SATA pciPCI interrupt-> INTD#
  {0, 0x50},        // GPPInt0-> INTA#
  {1, 0x51},        // GPPInt1-> INTB#
  {2, 0x52},        // GPPInt2-> INTC#
  {3, 0x53},        // GPPInt3-> INTD#
  {0xff, 0xff}      // EOT
};

#define MAX_IRQ_PRIORITY_ENTRIES (sizeof (IrqPriorityTable) / sizeof (EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY))

#define AHCI_ENABLE                       0x8000
#define AHCI_PORT0                        0x0100
#define AHCI_PORT2                        0x0200
#define AHCI_PORT4                        0x0400
#define AHCI_PORT6                        0x0800

#pragma pack(1)
typedef struct {
  EFI_HANDLE  Handle;
  UINT16      Vid;
  UINT16      Did;
  UINT16      SvId;
  UINT16      SysId;
} DEVICE_STRUCTURE;
#pragma pack()

EFI_LEGACY_BIOS_PROTOCOL                     *mLegacyBios;

BOOLEAN                                      mGetVideoOriginalModeOK = FALSE;;
UINT16                                       mVideoOriginalMode = 0;
EFI_HANDLE                                   mDiskHandles[0x20];
EFI_HANDLE                                   mVideoHandles[8];

static
VOID
AddIrqUsedTime (
  IN UINT8      IrqNumber
  );

static
UINT8
GetSuitableIrq (
  IN UINT8      SmallLimitation,
  IN UINT8      BigLimitation
  );

static
EFI_STATUS
EFIAPI
TranslatePirqHook (
  IN     EFI_LEGACY_BIOS_PLATFORM_PROTOCOL        *This,
  IN     UINTN                                    PciBus,
  IN     UINTN                                    PciDevice,
  IN     UINTN                                    PciFunction,
  IN OUT UINT8                                    *Pirq,
     OUT UINT8                                    *PciIrq
  );

/**
  Install callback when EFI_LEGACY_BIOS_PLATFORM_PROTOCOL installed.  

  @param[in] Standard EFI Image entry - EFI_IMAGE_ENTRY_POINT
             
  @retval    EFI_SUCEESS  Driver Dispatch success.

**/
EFI_STATUS
EFIAPI
LegacyBiosPlatformHookDxeEntry (
  IN EFI_HANDLE                       ImageHandle,
  IN EFI_SYSTEM_TABLE                 *SystemTable
  )
{
  EFI_STATUS                          Status;
  EFI_LEGACY_BIOS_PLATFORM_PROTOCOL   *LegacyBiosPlatform;

  Status = gBS->LocateProtocol (&gEfiLegacyBiosPlatformProtocolGuid, NULL, (VOID**)&LegacyBiosPlatform);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  LegacyBiosPlatform->TranslatePirq = TranslatePirqHook;

  Status = gBS->LocateProtocol (
                  &gEfiLegacyBiosProtocolGuid,
                  NULL,
                  (VOID**)&mLegacyBios
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;

}


/**
  Add the used time of designate IRQ

  @param[in]  IrqNumber - The IRQ which been assigned.

  @retval     None

**/
VOID
AddIrqUsedTime (
  IN UINT8        IrqNumber
  )
{
  UINTN   Index;

  Index = 0;

  for (Index = 0; Index < MAX_IRQ_PRIORITY_ENTRIES; Index++) {
    if (IrqPriorityTable[Index].Irq == IrqNumber) {
      IrqPriorityTable[Index].Used++;
      break;
    }
  }

  return ;
}

/**
  Get the suitable IRQ by request.

  @param[in]  SmallLimitation - The IRQ which must bigger than SmallLimitation.
              BigLimitation   - The IRQ which must smaller than BigLimitation.

  @retval     The suitable IRQ

**/
UINT8
GetSuitableIrq (
  IN UINT8        SmallLimitation,
  IN UINT8        BigLimitation
  )
{
  UINTN   Index;
  UINT8   SuitableIrq;
  UINT8   UsedTime;

  Index = 0;
  SuitableIrq = 0;
  UsedTime = 0xff;

  if (SmallLimitation < 1) {
    SmallLimitation = 1;
  }
  if (BigLimitation >= 0x10 ||
      BigLimitation == 0) {
    BigLimitation = 0x10;
  }

  for (Index = 0; Index < MAX_IRQ_PRIORITY_ENTRIES; Index++) {
    if (IrqPriorityTable[Index].Irq > SmallLimitation &&
        IrqPriorityTable[Index].Irq < BigLimitation &&
        IrqPriorityTable[Index].Irq != 0 &&
        IrqPriorityTable[Index].Used != LEGACY_USED &&
        IrqPriorityTable[Index].Used != PCI_USED &&
        IrqPriorityTable[Index].Used < UsedTime
      ) {
      SuitableIrq = IrqPriorityTable[Index].Irq;
      UsedTime = IrqPriorityTable[Index].Used;
    }
  }

  return SuitableIrq;
}

EFI_STATUS
SvcInstallIrqRoutingTableThunk (
  IN IRQ_ROUTING_TABLE **IrqRoutingTable
  )
{
  EFI_STATUS                            Status;
  LEGACY_MODIFY_PIR_TABLE               *VirtualBusTablePtr;
  UINT8                                 VirtualBusTableEntryNumber;
  UINT8                                 *PirqPriorityTablePtr;
  UINT8                                 PirqPriorityTableEntryNumber;
  EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY   *IrqPoolTablePtr;
  UINT8                                 IrqPoolTableNumber;

  VirtualBusTablePtr             = NULL;
  VirtualBusTableEntryNumber     = 0;
  IrqPoolTablePtr                = NULL;
  IrqPoolTableNumber             = 0;
  PirqPriorityTablePtr           = NULL;
  PirqPriorityTableEntryNumber   = 0;

  //
  // make a call to DxeCsSvc internally
  //
  Status = DxeCsSvcIrqRoutingInformation( &VirtualBusTablePtr,    &VirtualBusTableEntryNumber, 
                                          &IrqPoolTablePtr,       &IrqPoolTableNumber, 
                                          &PirqPriorityTablePtr,  &PirqPriorityTableEntryNumber,
                                          IrqRoutingTable
                                         );   

  return Status;
}

/**
  A hook for LegacyBiosPlatform->TranslatePirq().

  @param  This                  The protocol instance pointer.
  @param  PciBus                The PCI bus number for this device.
  @param  PciDevice             The PCI device number for this device.
  @param  PciFunction           The PCI function number for this device.
  @param  Pirq                  Input is PIRQ reported by device, and output is true PIRQ.
  @param  PciIrq                The IRQ already assigned to the PIRQ, or the IRQ to be
                                assigned to the PIRQ.

  @retval EFI_SUCCESS           The PIRQ was translated.

**/
static
EFI_STATUS
EFIAPI
TranslatePirqHook (
  IN  EFI_LEGACY_BIOS_PLATFORM_PROTOCOL           *This,
  IN UINTN                                        PciBus,
  IN UINTN                                        PciDevice,
  IN UINTN                                        PciFunction,
  IN  OUT UINT8                                   *Pirq,
  OUT UINT8                                       *PciIrq
  )
{
  EFI_LEGACY_IRQ_ROUTING_ENTRY        *IrqRoutingEntryTablePtr;
  IRQ_ROUTING_TABLE                   *IrqTablePtr;
  EFI_LEGACY_INTERRUPT_PROTOCOL       *LegacyInterrupt;
  EFI_STATUS                          Status;
  UINTN                               Index;
  UINT8                               LocalPirq;
  UINT8                               PirqData;
  UINTN                               MaxIrqEntriesCount;

  Status = gBS->LocateProtocol (
                            &gEfiLegacyInterruptProtocolGuid,
                            NULL,
                            (VOID**)&LegacyInterrupt
                            );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  IrqTablePtr = NULL;
  Status = SvcInstallIrqRoutingTableThunk(&IrqTablePtr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MaxIrqEntriesCount = IrqTablePtr->MaxRoutingTableCount;
  IrqRoutingEntryTablePtr = IrqTablePtr->IrqRoutingTablePtr;

  LocalPirq = (UINT8) (*Pirq);

  for ( Index = 0; Index < MaxIrqEntriesCount; Index ++) {
    if ((IrqRoutingEntryTablePtr[Index].Bus == PciBus) &&
        (IrqRoutingEntryTablePtr[Index].Device == PciDevice)
        ) {

      LocalPirq = (UINT8) (IrqRoutingEntryTablePtr[Index].PirqEntry[LocalPirq].Pirq & 0x0f);
      //
      // Read PIRQ data from Chipset
      // (Note for SB800, 0x1f is the default value)
      //
      LegacyInterrupt->ReadPirq (
                         LegacyInterrupt,
                         LocalPirq,
                         &PirqData
                         );
      if (PirqData == 0x1f) {
        //
        // There is no IRQ assigned, get the suitable IRQ from SetPirqPriority table or IrqPriorityTable
        //
        if (LocalPirq < MaxPIRQIndex) {
          PirqData = SetPIrqPriority[LocalPirq].Irq;
        } else {
          ASSERT (LocalPirq < MaxPIRQIndex);
        }
        if (PirqData != 0) {
          //
          // There is a IRQ already been assigned for this PIRQ before, so use it directly.
          //
          LegacyInterrupt->WritePirq (
                             LegacyInterrupt,
                             LocalPirq,
                             PirqData
                             );
        } else {
          //
          // There is no IRQ assigned, get the suitable IRQ from IrqPriorityTable
          //
          PirqData = GetSuitableIrq (0x0, 0x10);
          if (PirqData != 0) {
            //
            // Get the suitable IRQ from IrqPriority table, set it to PIRQ register in chipset.
            //
            LegacyInterrupt->WritePirq (
                               LegacyInterrupt,
                               LocalPirq,
                               PirqData
                               );
            if (LocalPirq < MaxPIRQIndex) {
              SetPIrqPriority[LocalPirq].Irq = PirqData;
            } else {
              ASSERT (LocalPirq < MaxPIRQIndex);
            }
            AddIrqUsedTime (PirqData);
          } else {
            return EFI_NOT_FOUND;
          }
        }
      }

      //
      // There is already a IRQ been assigned in chipset, so use it directly.
      //
      *PciIrq = PirqData;
      *Pirq = LocalPirq;

      Index = 0;
      while (mCsDevRoutingMap[Index].PirqNum != 0xff) {
        if (mCsDevRoutingMap[Index].PirqNum == LocalPirq) {
          // Set IRQ number to PIC
          LegacyInterrupt->WritePirq (
                             LegacyInterrupt,
                             mCsDevRoutingMap[Index].CsReg,
                             PirqData
                             );
          // Set IRQ number to APIC
          LegacyInterrupt->WritePirq (
                             LegacyInterrupt,
                             mCsDevRoutingMap[Index].CsReg | BIT7,
                             LocalPirq | BIT4
                             );
        }
        Index++;
      }
      return EFI_SUCCESS;
    }
  }

  return  EFI_NOT_FOUND;
}
