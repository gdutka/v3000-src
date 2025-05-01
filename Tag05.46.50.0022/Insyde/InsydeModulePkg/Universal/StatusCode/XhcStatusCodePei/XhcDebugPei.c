/** @file
  XhcDebugPei.c

;******************************************************************************
;* Copyright (c) 2023, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Ppi/IoMmu.h>
#include <XhcStatusCode.h>
#include "Pci.h"

typedef struct {
  EFI_HOB_GUID_TYPE             EfiHobGuidType;
  UINT64                        HcToken;
  UINT64                        DebugContextAddr;
  UINT32                        DeviceAddrConverter;
  UINT32                        Flags;
  EDKII_IOMMU_PPI               *Iommu;
  EFI_PEI_NOTIFY_DESCRIPTOR     IommuNotifyList;
} XHC_DEBUG_PRIVATE;

STATIC UINT8                    mStr0[0x10]         = { 0x04, 0x03, 0x09, 0x04 };
STATIC UINT8                    mManufacturer[0x10] = { 0x0e, 0x03, 'I', 0x00, 'n', 0x00, 's', 0x00, 'y', 0x00, 'd', 0x00, 'e', 0x00 };
STATIC UINT8                    mProduct[0x30]      = { 0x2a, 0x03, 'I', 0x00, 'n', 0x00, 's', 0x00, 'y', 0x00, 'd', 0x00, 'e', 0x00, ' ', 0x00,
                                                        'U',  0x00, 'S', 0x00, 'B', 0x00, '3', 0x00, ' ', 0x00,
                                                        'D',  0x00, 'e', 0x00, 'b', 0x00, 'u', 0x00, 'g', 0x00, 'g', 0x00, 'e', 0x00, 'r', 0x00 };
STATIC UINT8                    mSerialNum[0x10]    = { 0x08, 0x03, '1', 0x00, '.', 0x00, '0', 0x00 };
STATIC EFI_GUID                 mXhcDebugHobGuid    = { 0x850fae6a, 0x16da, 0x4b42, { 0x8d, 0x84, 0x3c, 0x36, 0x56, 0xb1, 0xe0, 0x1f } };
STATIC UINT8                    mStockXhci[]        = { 0x14, 0x00, 0x15, 0x00, 0x10, 0x00, 0x1e, 0x00, 0x00, 0x00 };

#define HtoD(a)                 ((UINT32)(UINTN)(a) - Private->DeviceAddrConverter)
#define DtoH(a)                 ((UINT32)(UINTN)(a) + Private->DeviceAddrConverter)

struct
{
  CHAR8             Signature[4];
  DEBUGIO_CONFIG    Config;
  UINT32            XhciBar;
} XhcConfig = {
  { 'D', 'B', 'G', 'C' },
  { 0,        // 0:LPC, 1:PCI
    0,        // 0:Bypass connection off, 1:Bypass connection on
    0,        // 0:USB, 1:LPT, 2:COM
    FixedPcdGet8 (PcdUsbDebugXhciControl), // USB HC port
    0,        // USB debug port
    0x0080, // Debug port
    10       // Bypass timeout
  },
  FixedPcdGet32 (PcdUsbDebugXhciBaseAddress)
};

UINT8
EFIAPI
ByteRotate (
  IN  UINT8                     Data
  )
{
  return (Data << 4) | (Data >> 4);
}

/**
 Is it USB Device

 @param [in]   PciAddress

 @return 0x00 : No
 @return 0x01 : XHCI
 @return 0x0f : Pcie bridge

**/
STATIC
UINTN
IsUsbDevice (
  IN UINT32                     PciAddress
  )
{
  UINT32 ClassCode;

  PciRead (EfiWidthUint32, PciAddress + 0x08, &ClassCode);
  if (ClassCode == 0xffffffff) return EMPTY;
  ClassCode &= 0xFFFFFF00;
  switch (ClassCode) {
  case 0x0C033000: return XHCI;
  case 0x06040000: return BRIDGE;
  }
  return 0;
}

/**
 Memory copy common routine

 @param [in]   DestAddr         Destination Address
 @param [in]   SourAddr         Source Address
 @param [in]   Width            Data width

 @retval EFI_SUCCESS            Success
 @retval EFI_UNSUPPORTED        Upsupported

**/
STATIC
EFI_STATUS
MemoryCopy (
  IN UINT8                      *DestAddr,
  IN UINT8                      *SourAddr,
  IN EFI_WIDTH                  Width
  )
{
  switch (Width) {
  case EfiWidthUint32:
    *(volatile UINT32*)DestAddr = *(UINT32*)SourAddr;
    break;
  case EfiWidthUint64:
    //
    // break down into two 32 bits operation
    //
    *(volatile UINT32*)(DestAddr + 0) = *(UINT32*)(SourAddr + 0);
    *(volatile UINT32*)(DestAddr + 4) = *(UINT32*)(SourAddr + 4);
    break;
  default:
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
 Read Xhc debug register

 @param [in]   Bar              Debug capability base address
 @param [in]   Width            Data width
 @param [in]   RegAddr
 @param [in, out] Data          A pointer to data read from register

 @retval EFI_SUCCESS            Success
 @retval EFI_UNSUPPORTED        Upsupported

**/
EFI_STATUS
ReadXhcDebugReg (
  IN UINT64                     Bar,
  IN EFI_WIDTH                  Width,
  IN UINT32                     RegAddr,
  IN OUT VOID                   *Data
  )
{
  return MemoryCopy(Data, (UINT8*)(UINTN)(Bar + RegAddr), Width);
}

/**
 Write Xhc debug register

 @param [in]   Bar              Debug capability base address
 @param [in]   Width            Data width
 @param [in]   RegAddr
 @param [in]   Data             A pointer to data write to register

 @retval EFI_SUCCESS            Success
 @retval EFI_UNSUPPORTED        Upsupported

**/
EFI_STATUS
WriteXhcDebugReg (
  IN UINT64                     Bar,
  IN EFI_WIDTH                  Width,
  IN UINT32                     RegAddr,
  IN VOID                       *Data
  )
{
  return MemoryCopy((UINT8*)(UINTN)(Bar + RegAddr), Data, Width);
}

/**
 Update the event dequeue pointer

 @param [in]   DbgBar
 @param [in]   DebugContext     A pointer to debug context
 @param [in]   TargetAddr


**/
STATIC
BOOLEAN
UpdateEventDequeuePointer (
  IN XHC_DEBUG_PRIVATE          *Private,
  IN UINT64                     DbgBar,
  IN DBG_CONTEXT                *DebugContext,
  IN UINT64                     TargetAddr
  )
{
  UINT64              Data;
  UINT32              CycleBit;
  XHCI_TRANSFER_EVENT *Event;
  BOOLEAN             Result = FALSE;

  ReadXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCERDP, &Data);
  Event = (XHCI_TRANSFER_EVENT*)(UINTN)DtoH (Data);
  CycleBit = Event->CycleBit;
  while (CycleBit == Event->CycleBit) {
    if (TargetAddr && TargetAddr == DtoH (Event->TRBPointer)) Result = TRUE;
    Event ++;
    if ((UINT64)(UINTN)Event >= DtoH (DebugContext->EventRingSegmentTable.RingSegmentBaseAddress) + (MultU64x32 (DebugContext->EventRingSegmentTable.RingSegmentSize, sizeof (XHCI_TRANSFER_EVENT)))) {
      Event = (XHCI_TRANSFER_EVENT*)(UINTN)DtoH (DebugContext->EventRingSegmentTable.RingSegmentBaseAddress);
      CycleBit ^= 1;
    }
  }
  Data = (UINT64)(UINTN)HtoD (Event);
  WriteXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCERDP, &Data);
  do {
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCST, &Data);
  } while (Data & B_DCST_ER);
  return Result;
}


/**
 Get USB Bar

 @param [in]   HcToken

 @return USB BAR

**/
STATIC
UINT64
GetUsbBar (
  IN UINT64                     HcToken
  )
{
  UINT32  PciAddress;
  UINT64  Bar;
  
  PciAddress = EFI_PCI_ADDRESS(((UINT32)(HcToken & B_TOKEN_XHCI_PCI_BUS) >> N_TOKEN_XHCI_PCI_BUS), ((UINT32)(HcToken & B_TOKEN_XHCI_PCI_DEV) >> N_TOKEN_XHCI_PCI_DEV), ((UINT32)(HcToken & B_TOKEN_XHCI_PCI_FUN) >> N_TOKEN_XHCI_PCI_FUN), 0);
  PciRead (EfiWidthUint64, PciAddress + 0x10, &Bar);
  Bar &= ~0x0f;
  if (Bar == 0) {
    Bar = XhcConfig.XhciBar;
    PciWrite (EfiWidthUint64, PciAddress + 0x10, &Bar);
    MicroSecondDelay (100000);
  }
  return Bar;
}


/**
 Turn power state on

 @param [in]   PciAddress


**/
STATIC
VOID
PowerStateOn (
  IN UINT32                     PciAddress
  )
{
  UINT32 Data;
  UINT8  CapPtr;
  //
  // Searching for Power Capabilities
  //
  PciRead (EfiWidthUint8, PciAddress + 0x34, &CapPtr);
  while (CapPtr != 0) {
    PciRead (EfiWidthUint32, PciAddress + CapPtr, &Data);
    if ((Data & 0xff) == 0x01) {
      PciRead (EfiWidthUint8, PciAddress + CapPtr + 4, &Data);
      if ((Data & 0x03) != 0) {
        Data = 0;
        PciWrite (EfiWidthUint8, PciAddress + CapPtr + 4, &Data);
        MicroSecondDelay (100000);
      }
      break;
    }
    CapPtr = (UINT8)((Data >> 8) & 0xff);
  }
}

/**
 Route the debug port from ordinary USB3 port

 @param [in]   Bar
 @param [in]   DbgBar

**/
STATIC
VOID
RouteDebugPort (
  IN  UINT64                    Bar,
  IN  UINT64                    DbgBar
  )
{
  UINTN  Index;
  UINT32 Cap;
  UINT32 Data;
  UINT32 *PortSc;
  UINTN  Port;
  UINTN  Count;
  UINTN  Countdown;
  UINTN  Timeout;
  UINTN  Retry;

  Countdown = 1000;
  do {
    Index = ((*(UINT32*)(UINTN)(Bar + 0x10) & 0xffff0000) >> 14);
    do {
      Cap = *(UINT32*)(UINTN)(Bar + Index);
      if ((Cap & 0xff0000ff) == 0x3000002) {
        //
        // USB3 supported protocol
        //
        Data = *(UINT32*)(UINTN)(Bar + Index + 8);
        PortSc = (UINT32*)(UINTN)(Bar + 0x400 + (*(UINT32*)(UINTN)Bar & 0xff) + (((Data & 0xff) - 1) * 0x10));
        Count = (UINTN)((Data >> 8) & 0xff);
        for (Timeout = 0, Retry = 0; Retry < 1000; Retry ++) {
          for (Port = 0; Port < Count; Port ++) {
            Data = PortSc[Port * 4];
            if ((Data & 0x1e0) == 0xe0) {
              //
              // The port in polling state, given more retry to make it become inactive state
              //
              Timeout ++;
              if (Timeout < 1000) {
                Retry --;
                MicroSecondDelay (1000);
              } 
            } else if ((Data & 0x1e0) == 0xc0) {
              //
              // The port in inactive state and debug port found, toggle the port power to make the signal route to debug port
              //
              Data &= ~0x200;
              PortSc[Port * 4] = Data;
              MicroSecondDelay (500000);
              Data = 0x200;
              PortSc[Port * 4] = Data;
              MicroSecondDelay (500000);
              return;
            }
          }
        }
      }
      Index += (UINTN)((Cap & 0xff00) >> (8 - 2));
    } while ((Cap & 0xff00) != 0);
    Countdown --;
    MicroSecondDelay (1000);
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
    if (Data & B_DCCTRL_DCR) break;
  } while (Countdown != 0);
}

/**
 Reset the debug port

 @param [in]   Bar

--*/
STATIC
VOID
ResetDebugPort (
  IN  UINT64                    Bar
  )
{
  UINTN  Index;
  UINTN  Offset;
  UINT32 Data;
  UINT32 Cap;
  UINT32 *Port;
  UINTN  Count;
  BOOLEAN IsReset;

  Offset = ((*(UINT32*)(UINTN)(Bar + 0x10) & 0xffff0000) >> 14);
  do {
    Cap = *(UINT32*)(UINTN)(Bar + Offset);
    if ((Cap & 0xff0000ff) == 0x3000002) {
      //
      // USB3 supported protocol
      //
      Data = *(UINT32*)(UINTN)(Bar + Offset + 8);
      Port = (UINT32*)(UINTN)(Bar + 0x400 + (*(UINT32*)(UINTN)Bar & 0xff) + (((Data & 0xff) - 1) * 0x10));
      Count = (UINTN)((Data >> 8) & 0xff);
      for (Index = 0, IsReset = FALSE; Index < Count; Index ++, Port += 4) {
        Data = *Port;
        if ((Data & 0x1e0) == 0xc0) {
          //
          // The debug port found, issue warm reset to the debug port
          //
          Data = 0x80000210;
          *Port = Data;
          IsReset = TRUE;
        } else if ((Data & 0x1e0) == 0x80) {
          //
          // The port in disabled state and debug port found, toggle the port power to make the signal route to debug port
          //
          Data &= ~0x200;
          *Port = Data;
          MicroSecondDelay (500000);
          Data = 0x200;
          *Port = Data;
          MicroSecondDelay (500000);
          IsReset = TRUE;
        }
      }
      if (!IsReset) {
        //
        // The debug port not found, reset whole non-connected port
        //
        for (Index = 0; Index < Count; Index ++, Port += 4) {
          Data = *Port;
          if ((Data & 0x03) == 0) {
            //
            // Issue warm reset to non-connected port
            //
            Data = 0x80000210;
            *Port = Data;
          }
        }
      }
    }
    Offset += (UINTN)((Cap & 0xff00) >> (8 - 2));
  } while ((Cap & 0xff00) != 0);
}

/**
 Search Xhci

 @param [in]   Private          Pointer to instance
 @param [in]   BufferSize       Buffer size
 @param [in]   Buffer           Pointer to buffer

 @retval EFI_SUCCESS

**/
STATIC
EFI_STATUS
SearchXhci (
  IN XHC_DEBUG_PRIVATE          *Private,
  IN UINT64                     HcToken,
  IN UINT64                     *XhciController,
  IN UINT32                     *LastBus,
  IN BOOLEAN                    Scan
  )
{
  UINT32  PciAddress;
  UINT32  Bus;
  UINT32  Dev;
  UINT32  DevStart;
  UINT32  Fun;
  UINT32  FunStart;
  UINT32  Data;
  UINT32  Bridge;
  UINT64  Bar;
  UINT32  Index;
  UINT32  LmcIndex;
  UINT64  Dcerstba;
  UINTN   Type;
  BOOLEAN Found;
  BOOLEAN RestoreSetting;
  UINTN   Phase;
  UINT8*  Ptr;

  Found    = FALSE;
  Bridge   = 0;
  Phase    = 0;
SEARCH_BRIDGES:
  Bus      = ((UINT32)HcToken >> 24) & 0xff;
  DevStart = ((UINT32)HcToken >> 19) & 0x1f;
  FunStart = ((UINT32)HcToken >> 16) & 0x07;
  for (Dev = DevStart; Dev < 0x20 && !Found; Dev ++) {
    for (Fun = FunStart; Fun < 8 && !Found; Fun ++) {
      PciAddress = EFI_PCI_ADDRESS(Bus, Dev, Fun, 0);
      Type = IsUsbDevice (PciAddress);
      switch (Type) {
      case XHCI:
        if (Bus == 0 && Phase == 1) break;
        Ptr = (UINT8*)&HcToken;
        *(UINT32*)(Ptr + 0) = 0;
        HcToken |= ((UINT32)PciAddress << 8) & 0xffff0000;
        Bar = GetUsbBar (HcToken);
        Data = 0;
        PciRead (EfiWidthUint16, PciAddress + 4, &Data);
        if ((Data & 0x06) != 0x06) {
          Data |= 0x06;
          PciWrite (EfiWidthUint16, PciAddress + 4, &Data);
          MicroSecondDelay (100000);
        }
        PowerStateOn (PciAddress);
        if (*(UINT32*)(UINTN)(Bar) != 0xffffffff) {
          Index = ((*(UINT32*)(UINTN)(Bar + 0x10) & 0xffff0000) >> 14);
          LmcIndex = 0;
          do {
            Data = *(UINT32*)(UINTN)(Bar + Index);
            if ((Data & 0xff) == 0x0a) {
              //
              // Debug Capability
              //
              HcToken |= Index;
            } else if ((Data & 0xff) == 0x06) {
              //
              // Local Memory Capability
              //
              HcToken |= 0x0001;
              LmcIndex = Index;
            }
            Index += ((Data & 0xff00) >> (8 - 2));
          } while ((Data & 0xff00) != 0);
          if ((HcToken & 0xfffe) != 0) {
            //
            // Debug Capability available
            //
            if (HcToken & 0x0001) {
              //
              // Setup LMC if available
              //
              Dcerstba = (UINT64)(UINTN)HtoD ((Bar + LmcIndex + (UINTN)&((DBG_CONTEXT*)0)->EventRingSegmentTable));
              WriteXhcDebugReg (Bar + (HcToken & 0xfffe), EfiWidthUint64, XHCI_DCERSTBA, &Dcerstba);
            }
            for (Index = 0; Index < MAX_XHCI_CONTROLLER; Index ++) {
              if (XhciController[Index] == 0) break;
            }
            XhciController[Index] = HcToken;
            if (Index == XhcConfig.Config.UsbHcPort) {
              Found = TRUE;
              continue;
            }
          }
        }
        PciRead (EfiWidthUint16, PciAddress + 4, &Data);
        Data &= ~0x06;
        PciWrite (EfiWidthUint16, PciAddress + 4, &Data);
        if (Bar == XhcConfig.XhciBar) {
          Bar = 0;
          PciWrite (EfiWidthUint64, PciAddress + 0x10, &Bar);
        }
        break;
      case BRIDGE:
        if (Bus == 0 && Phase == 0) break;
        PciRead (EfiWidthUint32, PciAddress + 0x18, &Data);
        RestoreSetting = FALSE;
        if (Data == 0) {
          Data = 0x00ff0000 + (*LastBus << 8) + Bus;
          PciWrite (EfiWidthUint32, PciAddress + 0x18, &Data);
          MicroSecondDelay (1000);
          Bridge = *LastBus;
          RestoreSetting = TRUE;
        } else {
          Bridge = (Data >> 8) & 0xff;
        }
        PciRead (EfiWidthUint32, PciAddress + 0x20, &Data);
        if ((Data == 0) || (((Data & 0xffff0000) >> 16) < (Data & 0x0000ffff))) {
          Data = (XhcConfig.XhciBar & 0xfff00000) + ((XhcConfig.XhciBar & 0xfff00000) >> 16);
          PciWrite (EfiWidthUint32, PciAddress + 0x20, &Data);
          MicroSecondDelay (1000);
          RestoreSetting = TRUE;
        }
        PowerStateOn (PciAddress);
        Data = 0;
        PciRead (EfiWidthUint16, PciAddress + 4, &Data);
        if ((Data & 0x06) != 0x06) {
          Data |= 0x06;
          PciWrite (EfiWidthUint16, PciAddress + 4, &Data);
          MicroSecondDelay (100000);
          RestoreSetting = TRUE;
        }
        (*LastBus) ++;
        Ptr = (UINT8*)&HcToken;
        Data = *(UINT32*)(Ptr + 4);
        Data <<= 8;
        Data |= ((Dev << 3) + Fun);
        *(UINT32*)(Ptr + 0) = (Bridge << 24);
        *(UINT32*)(Ptr + 4) = Data;
        if (SearchXhci (Private, HcToken, XhciController, LastBus, Scan) == EFI_SUCCESS) {
          //
          // if it is add-on PCIe XHCI card on Intel BayTrail chipset, set upstream non-posted request size
          // in PCIe root bridge Channel Configuration(0xd0) bit 15 to 1
          //
          if (RestoreSetting) {
            Data = 0;
            PciRead (EfiWidthUint16, PciAddress, &Data);
            if (Data == 0x8086) {
              PciRead (EfiWidthUint32, PciAddress + 0xd0, &Data);
              if ((Data & 0x00008000) == 0) {
                Data |= 0x00008000;
                PciWrite (EfiWidthUint32, PciAddress + 0xd0, &Data);
              }
            }
            PciRead (EfiWidthUint32, PciAddress + 0x18, &Data);
            Data &= 0x0000ffff;
            Data |= ((*LastBus - 1) << 16);
            PciWrite (EfiWidthUint32, PciAddress + 0x18, &Data);
          }
          Found = TRUE;
          continue;
        } else if (RestoreSetting) {
          Ptr = (UINT8*)&HcToken;
          Data = *(UINT32*)(Ptr + 4);
          Data >>= 8;
          *(UINT32*)(Ptr + 4) = Data;
          PciRead (EfiWidthUint16, PciAddress + 4, &Data);
          Data &= ~0x06;
          PciWrite (EfiWidthUint16, PciAddress + 4, &Data);
          Data = 0;
          PciWrite (EfiWidthUint32, PciAddress + 0x20, &Data);
          Data = 0;
          PciWrite (EfiWidthUint32, PciAddress + 0x18, &Data);
          RestoreSetting = FALSE;
        }
        break;
      case 0:
        if (!Scan) {
          Dev = 0x20;
          Fun = 0x08;
        }
        break;
      case EMPTY:
        if (!Scan || (Fun == 0 && Bus != 0)) {
          Dev = 0x20;
          Fun = 0x08;
        }
        break;
      }
    }
    FunStart = 0;
  }
  if (!Found && Scan && Bus == 0 && Phase == 0) {
    Phase ++;
    goto SEARCH_BRIDGES;
  }
  return (Found) ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
 Validate bridges

 @param [in]   Private          Pointer to instance

 @retval EFI_SUCCESS
        
**/
STATIC
EFI_STATUS
ValidateBridges (
  IN  XHC_DEBUG_PRIVATE         *Private
  )
{
  UINT32  PciAddress;
  UINT64  HcToken;
  UINT8   *Ptr;
  UINT32  Data;
  UINTN   Index;
  UINT32  Bridges;
  UINT32  Bus;
  UINT32  Dev;
  UINT32  Fun;
  UINT32  LastBus;
  BOOLEAN BridgeFound;
  UINT64  XhciController[MAX_XHCI_CONTROLLER];

  HcToken    = Private->HcToken;
  Ptr        = (UINT8*)&HcToken;
  Bus        = 0;
  PciAddress = 0;
  Data       = 0;
  if ((HcToken & B_TOKEN_XHCI_PCI_BUS) != 0) {
    Bridges = *(UINT32*)(Ptr + 4);
    BridgeFound = FALSE;
    for (Index = 0; Index < 4; Index ++) {
      Dev = (Bridges >> (((3 - Index) * 8) + 3)) & 0x1f;
      Fun = (Bridges >> (((3 - Index) * 8) + 0)) & 0x07;
      if (!BridgeFound && Dev == 0 && Fun == 0) continue;
      BridgeFound = TRUE;
      PciAddress = EFI_PCI_ADDRESS(Bus, Dev, Fun, 0);
      PciRead (EfiWidthUint16, PciAddress, &Data);
      if (Bus == 0 && Data == 0xffff) {
        ZeroMem (XhciController, sizeof (UINT64) * MAX_XHCI_CONTROLLER);
        LastBus = XHCI_BRIDGE;
        XhcConfig.XhciBar += 0x100000;
        if (SearchXhci (Private, 0, XhciController, &LastBus, TRUE) == EFI_SUCCESS) {
          HcToken = XhciController[XhcConfig.Config.UsbHcPort];
          Private->HcToken = HcToken;
          Bridges = *(UINT32*)(Ptr + 4);
          Dev = (Bridges >> (((3 - Index) * 8) + 3)) & 0x1f;
          Fun = (Bridges >> (((3 - Index) * 8) + 0)) & 0x07;
          PciAddress = EFI_PCI_ADDRESS(Bus, Dev, Fun, 0);
        }
        XhcConfig.XhciBar -= 0x10000;
      }
      PciRead (EfiWidthUint16, PciAddress + 4, &Data);
      if ((Data & 0x06) != 0x06) {
        Data |= 0x06;
        PciWrite (EfiWidthUint16, PciAddress + 4, &Data);
        MicroSecondDelay (100000);
      }
      PciRead (EfiWidthUint16, PciAddress + 0x18, &Data);
      Bus = (Data >> 8) & 0xff;
    }
  }
  Data = *(UINT32*)(Ptr + 0);
  Dev = (Data >> 19) & 0x1f;
  Fun = (Data >> 16) & 0x07;
  PciAddress = EFI_PCI_ADDRESS(Bus, Dev, Fun, 0);
  if (Bus && Bus != ((Data & B_TOKEN_XHCI_PCI_BUS) >> N_TOKEN_XHCI_PCI_BUS)) {
    Ptr[3] = (UINT8)Bus;
    Private->HcToken = HcToken;
  }
  return (IsUsbDevice (PciAddress) == XHCI) ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
 Get Xhci PCI location

 @param [in]   Private			Pointer to instance

 @return PCI address

**/
STATIC
UINT32
GetXhciLocation (
  IN  XHC_DEBUG_PRIVATE         *Private
  )
{
  UINT32  PciAddress;
  UINT64  HcToken;
  UINT64  Bar;
  UINT32  Data;

  HcToken = Private->HcToken;
  PciAddress = EFI_PCI_ADDRESS(((UINT32)(HcToken & B_TOKEN_XHCI_PCI_BUS) >> N_TOKEN_XHCI_PCI_BUS), ((UINT32)(HcToken & B_TOKEN_XHCI_PCI_DEV) >> N_TOKEN_XHCI_PCI_DEV), ((UINT32)(HcToken & B_TOKEN_XHCI_PCI_FUN) >> N_TOKEN_XHCI_PCI_FUN), 0);
  if (IsUsbDevice (PciAddress) != XHCI) {
    //
    // XHCI disappeared, starting XHCI recovery procedure
    //
    if (ValidateBridges (Private) != EFI_SUCCESS) {
      return 0;
    }
    HcToken = Private->HcToken;
    PciAddress = EFI_PCI_ADDRESS(((UINT32)(HcToken & B_TOKEN_XHCI_PCI_BUS) >> N_TOKEN_XHCI_PCI_BUS), ((UINT32)(HcToken & B_TOKEN_XHCI_PCI_DEV) >> N_TOKEN_XHCI_PCI_DEV), ((UINT32)(HcToken & B_TOKEN_XHCI_PCI_FUN) >> N_TOKEN_XHCI_PCI_FUN), 0);
  }
  Bar = GetUsbBar (HcToken);
  Data = 0;
  PciRead (EfiWidthUint16, PciAddress + 4, &Data);
  if ((Data & 0x06) != 0x06) {
    Data |= 0x06;
    PciWrite (EfiWidthUint16, PciAddress + 4, &Data);
    MicroSecondDelay (100000);
  }
  PowerStateOn (PciAddress);
  return (*(UINT32*)(UINTN)(Bar) == 0xffffffff) ? 0 : PciAddress;
}

/**
 Debug Software Startup with Non-Initialized XHCI

 @param [in]   HcToken
 @param [out]  DebugBar
 @param [in, out] DebugContextAddr
 @param [in]   RuntimeState

 @retval EFI_SUCCESS

**/
STATIC
EFI_STATUS
InitXhciDebugPort (
  IN  XHC_DEBUG_PRIVATE         *Private,
  IN  BOOLEAN                   Run,
  OUT UINT64                    *DebugBar
  )
{
  EFI_STATUS            Status;
  UINT32                PciAddress;
  UINT64                HcToken;
  UINT64                Bar;
  UINT64                DbgBar;
  UINT32                Data;
  UINT64                Pointer;
  DBG_CONTEXT           *DebugContext;
  UINT8                 PostCode;
  UINTN                 Index;
  UINTN                 Timeout;
  BOOLEAN               UpdateContext;
  UINT32                RootPort;
  UINT8                 *Ptr;
  UINT32                Dev;
  UINT32                Fun;
  UINT16                VidDid[2];

  UpdateContext = TRUE;
  DebugContext  = NULL;
  if ((PciAddress = GetXhciLocation (Private)) == 0) return EFI_DEVICE_ERROR;
  PciRead (EfiWidthUint32, PciAddress, VidDid);
  HcToken = Private->HcToken;
  Bar = GetUsbBar (HcToken);
  DbgBar = Bar + (HcToken & B_TOKEN_DEBUG_CAP_INDEX);
  if (DebugBar) *DebugBar = DbgBar;
  if (!Run) {
    //
    // Turn off XHCI DC upon Run flag is FALSE
    //
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
    if (Data & B_DCCTRL_DCE) {
      Data = 0;
      WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
    }
    if (VidDid[0] == 0x1022) {
      //
      // Stop the XHCI for AMD chipset
      //
      *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff)) = 0x00;
      do {
        Data = *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff) + 4);
        MicroSecondDelay (1000);
      } while ((Data & 0x01) == 0);
    }
    return EFI_SUCCESS;
  }
  //
  // First pass for XHCI debug capability initial
  //
  while (TRUE) {
    ReadXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCERSTBA, &Pointer);
    if (Pointer == 0) {
      //
      // Initial XHCI DC due to DC doesn't been initial 
      //
      break;
    }
    DebugContext = (DBG_CONTEXT*)(UINTN)DtoH ((Pointer - (UINTN)&((DBG_CONTEXT*)0)->EventRingSegmentTable));
    if (DebugContext != (DBG_CONTEXT*)(UINTN)Private->DebugContextAddr) {
      if (((DBG_CONTEXT*)(UINTN)Private->DebugContextAddr)->Signature == USB_XHCID_SIGNATURE &&
          DebugContext->Signature == USB_XHCID_SIGNATURE) {
        //
        // Debug context is valid, update debug context address to sync with XHCI_DCERSTBA register
        //
        Private->DebugContextAddr = (UINT64)DebugContext;
        return EFI_SUCCESS;
      } else {
        //
        // Reinitial XHCI DC due to debug context address changed
        //
        break;
      }
    }
    Data = *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff));
    if (Data & 0x02) {
      do {
        Data = *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff));
        MicroSecondDelay (1000);
      } while ((Data & 0x02) != 0);
      //
      // Reinitial XHCI DC due to HCRST in progress 
      //
      break;
    }
    if ((Data & 0x01) == 0) {
      if (VidDid[0] == 0x1022) {
        //
        // Reinitial XHCI DC due to Xhc in stop state for AMD chipset
        //
        break;
      }
    }
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
    if (!(Data & B_DCCTRL_DCE)) {
      //
      // Reinitial XHCI DC due to DC on stop state 
      //
      break;
    }
    if (HcToken & B_TOKEN_PORT_ROUTING) {
      PciRead (EfiWidthUint32, PciAddress + 0xd0, &Data);
      if (Data == 0) {
        //
        // Reinitial XHCI DC due to USB3 port routing changed, Intel only
        //
        break;
      }
      PciRead (EfiWidthUint32, PciAddress + 0xd8, &Data);
      if (Data == 0) {
        //
        // Reinitial XHCI DC due to USB3 port routing changed, Intel only
        //
        break;
      }
    }
    if ((HcToken & B_TOKEN_LMC) && DebugContext->Signature != USB_XHCID_SIGNATURE) {
      //
      // Reinitial XHCI DC due to the XHC BAR changes during runtime and need to update XHC local memory
      //
      break;
    }
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCPORTSC, &Data);
    if (Data & (B_DCPORTSC_CSC | B_DCPORTSC_PRC | B_DCPORTSC_PLC | B_DCPORTSC_CEC)) {
      //
      // Port status changed, we need to recheck the port connection status without context update
      //
      UpdateContext = FALSE;
      //
      // Enable the port if it under disable state
      //
      if ((Data & B_DCPORTSC_CCS) && ((Data & B_DCPORTSC_PLS) >> N_DCPORTSC_PLS) == 0x04) {
        Data |= B_DCPORTSC_PED;
        WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCPORTSC, &Data);
        MicroSecondDelay (100000);
      }
      //
      // Reinitial XHCI DC due to USB3 port state changed
      //
      break;
    }
    return EFI_SUCCESS;
  }
  //
  // Set upstream non-posted request size to 1 if it is add-on PCIe XHCI card on Intel BayTrail chipset
  //
  if ((HcToken & B_TOKEN_XHCI_PCI_BUS) != 0) {
    //
    // Set upstream non-posted request size in PCIe root bridge Channel Configuration(0xd0) bit 15 to 1
    //
    Ptr  = (UINT8*)&HcToken;
    Data = *(UINT32*)(Ptr + 4);
    for (Index = 0, RootPort = 0; Index < 4; Index ++) {
      Dev = (Data >> (((3 - Index) * 8) + 3)) & 0x1f;
      Fun = (Data >> (((3 - Index) * 8) + 0)) & 0x07;
      if (Dev != 0 || Fun != 0) {
        RootPort = EFI_PCI_ADDRESS(0, Dev, Fun, 0);
        break;
      }
    }
    RootPort &= 0xfffff800;
    Data = 0;
    PciRead (EfiWidthUint16, RootPort, &Data);
    if (Data == 0x8086) {
      PciRead (EfiWidthUint32, RootPort + 0xd0, &Data);
      if ((Data & 0x00008000) == 0) {
        Data |= 0x00008000;
        PciWrite (EfiWidthUint32, RootPort + 0xd0, &Data);
      }
    }
  }
  //
  // Set XHC/EHC routing for Intel XHCI to make sure the signal routed to XHC
  //
  if (HcToken & B_TOKEN_PORT_ROUTING) {
    Data = 0xffffffff;
    PciWrite (EfiWidthUint32, PciAddress + 0xd0, &Data);
    PciWrite (EfiWidthUint32, PciAddress + 0xd8, &Data);
    MicroSecondDelay (100000);
  }
  if (VidDid[0] == 0x8086) {
    if ((*(UINT32*)(UINTN)(Bar + 0x8070) & 0xffff) == 0xffc0) {
      *(UINT32*)(UINTN)(Bar + 0x80b8) |= 0x0000000f;
    }
    if ((*(UINT32*)(UINTN)(Bar + 0x8800) & 0xffff) == 0x40c4) {
      *(UINT32*)(UINTN)(Bar + 0x880c) |= 0xC0000000;
      *(UINT32*)(UINTN)(Bar + 0x883c) |= 0xC0000000;
    }
    //
    // Set 64 byte DMA boundary(bit 22~25) and Read Request Size control(bit 0~3) on Intel BayTrail chipset
    //
    Data = 0x83cfc68f;
    PciWrite (EfiWidthUint32, PciAddress + 0x44, &Data);
  }
  //
  // Make DC disable before issuing HCRST
  //
  ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
  if (Data & B_DCCTRL_DCE) {
    //
    // Debug Capability Disable
    //
    Data = 0;
    WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
    MicroSecondDelay (10000);
  }
  Data = *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff));
  if (Data & 0x01) {
    //
    // Issue debug port warm reset instead of HCRESET if XHC is under run state
    // to avoid the other port from reset by HCRESET 
    //
    ResetDebugPort (Bar);
  } else {
    //
    // Issue a HCRESET before initialization
    //
    *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff)) = 0x02;
    do {
      Data = *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff));
      MicroSecondDelay (1000);
    } while ((Data & 0x02) != 0);
    if (VidDid[0] == 0x1022) {
      //
      // Turn on Run state for AMD chipset
      //
      *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff)) = 0x01;
      do {
        Data = *(UINT32*)(UINTN)(Bar + (*(UINT32*)(UINTN)Bar & 0xff) + 4);
        MicroSecondDelay (1000);
      } while ((Data & 0x01) != 0);
    }
  }
  //
  // Getting the DebugContext according to hardware supporting level
  //
  if (!(HcToken & B_TOKEN_LMC)) {
    //
    // Allocate memory for the debug context
    //
    if (Private->DebugContextAddr != 0) {
      DebugContext = (DBG_CONTEXT*)(UINTN)Private->DebugContextAddr;
    } else if (DebugContext == NULL) {
      Status = PeiServicesAllocatePages (EfiRuntimeServicesData, 1, &Pointer);
      if (Status != EFI_SUCCESS) {
        return EFI_OUT_OF_RESOURCES;
      }
      DebugContext = (DBG_CONTEXT*)(UINTN)Pointer;
    }
  }
  if (UpdateContext) {
    //
    // Set LMC enable after HCRESET
    //
    ZeroMem (DebugContext, sizeof (DBG_CONTEXT));
    DebugContext->LmcCapID |= B_LMC_LME;
    //
    // Setup Debug Context
    //
    DebugContext->Signature                             = USB_XHCID_SIGNATURE;
    DebugContext->ContextSize                           = sizeof (DBG_CONTEXT);
    DebugContext->EventRingSegmentTable.RingSegmentSize = EVENT_RING_SIZE;
    DebugContext->DebugInfoContext.Str0Len              = mStr0[0];
    DebugContext->DebugInfoContext.ManufacturerLen      = mManufacturer[0];
    DebugContext->DebugInfoContext.ProductLen           = mProduct[0];
    DebugContext->DebugInfoContext.SerialNumLen         = mSerialNum[0];
    DebugContext->EndpointOut.MaxPacketSize             = 0x400;
    DebugContext->EndpointOut.ErrorCount                = 3;
    DebugContext->EndpointOut.EPType                    = EP_TYPE_BULK_OUT;
    DebugContext->EndpointOut.AvgTRBLength              = 0x400;
    DebugContext->BulkOutCycleBit                       = 1;
    DebugContext->BulkOutLinkTRB.ToggleCycle            = 1;
    DebugContext->BulkOutLinkTRB.TRBType                = TRB_TYPE_LINK;
    DebugContext->EndpointIn.MaxPacketSize              = 0x400;
    DebugContext->EndpointIn.ErrorCount                 = 3;
    DebugContext->EndpointIn.EPType                     = EP_TYPE_BULK_IN;
    DebugContext->EndpointIn.AvgTRBLength               = 0x400;
    DebugContext->BulkInCycleBit                        = 1;
    DebugContext->BulkInLinkTRB.ToggleCycle             = 1;
    DebugContext->BulkInLinkTRB.TRBType                 = TRB_TYPE_LINK;
    CopyMem (DebugContext->Str0, mStr0, 0x10);
    CopyMem (DebugContext->ManufacturerStr, mManufacturer, 0x10);
    CopyMem (DebugContext->ProductStr, mProduct, 0x30);
    CopyMem (DebugContext->SerialNum, mSerialNum, 0x10);
    //
    // Setup Debug Capability Registers
    //
    Data = 1;
    WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCERSTSZ, &Data);
    Data = INSYDE_USB3D_VID << N_DCDDI1_VID;
    WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCDDI1, &Data);
    Data = (INSYDE_USB3D_REV << N_DCDDI2_REV) + INSYDE_USB3D_PID;
    WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCDDI2, &Data);
    //
    // Setup internal data pointers
    //
    DebugContext->EventRingSegmentTable.RingSegmentBaseAddress = (UINT64)(UINTN)HtoD (DebugContext->Event);
    DebugContext->DebugInfoContext.Str0DescAddr                = (UINT64)(UINTN)HtoD (DebugContext->Str0);
    DebugContext->DebugInfoContext.ManufacturerDescAddr        = (UINT64)(UINTN)HtoD (DebugContext->ManufacturerStr);
    DebugContext->DebugInfoContext.ProductDescAddr             = (UINT64)(UINTN)HtoD (DebugContext->ProductStr);
    DebugContext->DebugInfoContext.SerialNumDescAddr           = (UINT64)(UINTN)HtoD (DebugContext->SerialNum);
    DebugContext->EndpointOut.TRDequeuePointer                 = (UINT64)(UINTN)HtoD (DebugContext->BulkOutTRB) | 0x01;
    DebugContext->EndpointIn.TRDequeuePointer                  = (UINT64)(UINTN)HtoD (DebugContext->BulkInTRB) | 0x01;
    DebugContext->BulkOutLinkTRB.RingSegmentPointer            = (UINT64)(UINTN)HtoD (DebugContext->BulkOutTRB);
    DebugContext->BulkInLinkTRB.RingSegmentPointer             = (UINT64)(UINTN)HtoD (DebugContext->BulkInTRB);
    //
    // Setup Debug Capability pointer registers
    //
    Pointer = (UINT64)(UINTN)HtoD (&DebugContext->EventRingSegmentTable);
    WriteXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCERSTBA, &Pointer);
    Pointer = (UINT64)(UINTN)HtoD (DebugContext->Event);
    WriteXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCERDP, &Pointer);
    Pointer = (UINT64)(UINTN)HtoD (&DebugContext->DebugInfoContext);
    WriteXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCCP, &Pointer);
  }
  //
  // Debug Capability Enable
  //
  Data = B_DCCTRL_DCE | B_DCCTRL_DRC;
  WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
  //
  // Route the port signal from ordinary USB3 port to debug port
  //
  RouteDebugPort (Bar, DbgBar);
  //
  // Waiting for USB3 debug port linked
  //
  PostCode = 0xd0;
  Index = 0;
  Timeout = XhcConfig.Config.BypassTimeout * 2;
  while (TRUE) {
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCCTRL, &Data);
    if (Data & B_DCCTRL_DCR) break;
    MicroSecondDelay (1000);
    if (!Index) {
      IoWrite32 ((UINT16)XhcConfig.Config.DebugPort, PostCode);
      PostCode = ByteRotate(PostCode);
      Index = 500;
      if (XhcConfig.Config.Bypass) {
        if (!Timeout) return EFI_TIMEOUT;
        Timeout --;
      }
    }
    Index --;
  }
  //
  // Sets debug port for 0xd1 to indicate Xhci debug port ready
  //
  IoWrite32 ((UINT16)XhcConfig.Config.DebugPort, 0xd1);
  //
  // Stall 3 seconds to waiting for host/device communication completed
  //
  MicroSecondDelay (1000000);

  //
  // Clear status registers
  //
  ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCPORTSC, &Data);
  WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCPORTSC, &Data);
  UpdateEventDequeuePointer (Private, DbgBar, DebugContext, 0);
  Private->DebugContextAddr = (UINT64)(UINTN)DebugContext;
  return EFI_SUCCESS;
}

/**
 Detect Xhci

 @param [in]   Token
 @param [out]  DbgBar
 @param [in, out] DebugContextAddr

 @retval EFI_SUCCESS
 @retval EFI_NOT_FOUND

**/
STATIC
EFI_STATUS
DetectXhci (
  IN  XHC_DEBUG_PRIVATE         *Private,
  IN  BOOLEAN                   Run,
  OUT UINT64                    *DbgBar
  )
{
  EFI_STATUS  Status;
  UINT64      XhciController[MAX_XHCI_CONTROLLER];
  UINT32      PciAddress;
  UINTN       Index;
  UINTN       Index1;
  BOOLEAN     Found;
  UINT32      LastBus;

  Status       = EFI_SUCCESS;
  if (Private->HcToken == 0) {
    //
    // XHCI detection upon initial phase
    //
    ZeroMem (XhciController, sizeof (UINT64) * MAX_XHCI_CONTROLLER);
    Found = FALSE;
    LastBus = XHCI_BRIDGE;
    if (XhcConfig.Config.UsbHcPort == 0) {
      for (Index = 0, Index1 = 0; mStockXhci[Index] != 0; Index += 2) {
        PciAddress = EFI_PCI_ADDRESS(0, mStockXhci[Index], mStockXhci[Index + 1], 0);
        Private->HcToken = ((UINT32)PciAddress << 8) & 0xffff0000;
        if (SearchXhci (Private, Private->HcToken, XhciController, &LastBus, FALSE) == EFI_SUCCESS) {
          Found  = TRUE;
          break;
        }
      }
    }
    if (!Found) {
      SearchXhci (Private, 0, XhciController, &LastBus, TRUE);
    }
    Status = EFI_NOT_FOUND;
    if (XhciController[XhcConfig.Config.UsbHcPort] != 0) {
      Private->HcToken = XhciController[XhcConfig.Config.UsbHcPort];
      Status = EFI_SUCCESS;
    }
  }
  if (Status == EFI_SUCCESS) {
    //
    // Initial XHCI once proper XHC detected
    //
    Status = InitXhciDebugPort (Private, Run, DbgBar);
  }
  return Status;
}

/**
 Calculate the TD size based on xHCI interface version

 @param [in]   RemainingSize    Remaining Byte Size

 @return TD size

**/
STATIC
UINT32
CalcTDSize (
  IN  UINTN                     RemainingSize
  )
{
  UINT32 TDSize = (UINT32)(RemainingSize + 0x400 - 1) / 0x400;
  return (TDSize < 0x1f) ? TDSize : 0x1f;
}

/**

  Signal handlers for IOMMU Ppi event

  @param  Event                 Event type                
  @param  Context               Context for the event

**/
STATIC
EFI_STATUS
EFIAPI
IommuPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  XHC_DEBUG_PRIVATE             *Private;
  EFI_STATUS                    Status;
  VOID                          *Ptr;
  VOID                          *Map;
  UINTN                         Size;
  EFI_PHYSICAL_ADDRESS          Addr;
  
  Private = GetFirstGuidHob (&mXhcDebugHobGuid);;
  Private->Iommu = (EDKII_IOMMU_PPI*)Ppi;
  Status = Private->Iommu->AllocateBuffer (
                             Private->Iommu,
                             EfiRuntimeServicesData,
                             1,
                             &Ptr,
                             EDKII_IOMMU_ATTRIBUTE_MEMORY_WRITE_COMBINE | EDKII_IOMMU_ATTRIBUTE_MEMORY_CACHED
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Size = 0x1000;
  Status = Private->Iommu->Map (
                             Private->Iommu,
                             EdkiiIoMmuOperationBusMasterCommonBuffer,
                             Ptr,
                             &Size,
                             &Addr,
                             &Map
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (Ptr, Size);
  Private->DebugContextAddr    = (UINT64)(UINTN)Ptr;
  Private->DeviceAddrConverter = (UINT32)((UINTN)Ptr - (UINTN)Addr);
  Status = Private->Iommu->SetAttribute (
                             Private->Iommu,
                             Map,
                             EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
                             );
  if (!EFI_ERROR (Status)) {
    //
    // AMD chipset installs IOMMU PPI/protocol after DMAr enabled, issue wbinvd 
    // CPU command after DMAr turned on in order to workaround XHCI DC unable to
    // be enabled issue on AMD chipset 
    //
    AsmWbinvd ();
    //
    // Intel chipset installs IOMMU PPI/rotocol before DMAr enabled, Stop XHCI DC
    // before DMAr turned on in order to workaround XHCI HC_RESET bit freeze 
    // (can't cleared by XHCI) issue after DMAr enabled on Intel chipset
    //
    DetectXhci (Private, FALSE, NULL);
  }
  return EFI_SUCCESS;
}

/**
 DebugUsbWrite

 @param [in]   Private          Pointer to instance
 @param [in]   BufferSize       Buffer size
 @param [in]   Buffer           Pointer to buffer

 @retval EFI_SUCCESS

**/
STATIC
EFI_STATUS
DebugXhcWrite (
  IN XHC_DEBUG_PRIVATE          *Private,
  IN UINTN                      BufferSize,
  IN VOID                       *Buffer
  )
{
  EFI_STATUS                Status;
  UINT64                    DbgBar;
  UINT32                    Data;
  UINT64                    Pointer;
  DBG_CONTEXT               *DebugContext;
  XHCI_NORMAL_TRB           InternalTRB;

  if (DetectXhci (Private, TRUE, &DbgBar) != EFI_SUCCESS) return EFI_DEVICE_ERROR;
  //
  // Get the debug context back from DCERSTBA
  //
  ReadXhcDebugReg (DbgBar, EfiWidthUint64, XHCI_DCERSTBA, &Pointer);
  DebugContext = (DBG_CONTEXT*)(UINTN)DtoH ((Pointer - (UINTN)&((DBG_CONTEXT*)0)->EventRingSegmentTable));
  ZeroMem (&InternalTRB, sizeof (XHCI_NORMAL_TRB));
  InternalTRB.TRBType        = TRB_TYPE_NORMAL;
  InternalTRB.IOC            = 1;
  InternalTRB.DataBufferPointer = (UINT64)(UINTN)HtoD (DebugContext->StringBuffer);
  InternalTRB.TransferLength = (UINT32)(BufferSize);
  InternalTRB.CycleBit       = DebugContext->BulkOutCycleBit;
  InternalTRB.TDSize         = CalcTDSize(BufferSize);
  CopyMem (&DebugContext->BulkOutTRB[DebugContext->BulkOutIndex], &InternalTRB, sizeof (XHCI_NORMAL_TRB));
  if (DebugContext->BulkOutIndex == BULK_RING_SIZE - 2) {
    DebugContext->BulkOutLinkTRB.CycleBit = DebugContext->BulkOutCycleBit;
  }
  CopyMem (DebugContext->StringBuffer, Buffer, BufferSize);
  //
  // Ring the door bell
  //
  Data = 0x00000000;
  WriteXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCDB, &Data);
  Status = EFI_SUCCESS;
  while (TRUE) {
    ReadXhcDebugReg (DbgBar, EfiWidthUint32, XHCI_DCST, &Data);
    if (Data & B_DCST_ER) {
      if (UpdateEventDequeuePointer (Private, DbgBar, DebugContext, (UINT64)(UINTN)&DebugContext->BulkOutTRB[DebugContext->BulkOutIndex])) break;
    }
  }
  DebugContext->BulkOutIndex ++;
  if (DebugContext->BulkOutIndex == BULK_RING_SIZE - 1) {
    DebugContext->BulkOutIndex     = 0;
    DebugContext->BulkOutCycleBit ^= 1;
  }
  return Status;
}


/**
 Prints a string to the USB port

 @param [in]   OutputString     Ascii string to print to serial port.

 @retval None

**/
VOID
EFIAPI
XhcDebugPrint (
  IN CHAR8    *OutputString
  )
{
  UINTN                         StringLength;
  CHAR8                         *StringPtr;
  UINT32                        PciIndexSave;
  DBG_CONTEXT                   *DbgContext;
  UINTN                         BufferLen;
  XHC_DEBUG_PRIVATE             *Private;

  Private = GetFirstGuidHob (&mXhcDebugHobGuid);
  if (Private == NULL) {
    return ;
  }
  DbgContext = (DBG_CONTEXT*)(UINTN)Private->DebugContextAddr;
  //
  // Save PCI index
  //
  PciIndexSave = IoRead32 (0xcf8);
  StringLength = AsciiStrLen (OutputString);
  for (StringPtr = OutputString; StringLength > 0; StringLength -= BufferLen, StringPtr += BufferLen) {
    BufferLen = (StringLength > sizeof (DbgContext->StringBuffer)) ? sizeof (DbgContext->StringBuffer) : StringLength;
    DebugXhcWrite (Private, BufferLen, StringPtr);
  }
  //
  // Restore PCI index
  //
  IoWrite32 (0xcf8, PciIndexSave);
}

EFI_STATUS
EFIAPI
XhcInitializeStatusCode (
  VOID
  )
{
  XHC_DEBUG_PRIVATE             *Private;
  EFI_STATUS                    Status;

  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (XHC_DEBUG_PRIVATE),
             (VOID **) &Private
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  CopyGuid (&Private->EfiHobGuidType.Name, &mXhcDebugHobGuid);
  Private->HcToken                = 0;
  Private->DebugContextAddr       = 0;
  Private->DeviceAddrConverter    = 0;
  Private->Flags                  = 0;
  Private->Iommu                  = NULL;
  Private->IommuNotifyList.Flags  = EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  Private->IommuNotifyList.Guid   = &gEdkiiIoMmuPpiGuid;
  Private->IommuNotifyList.Notify = IommuPpiNotifyCallback;
  PeiServicesNotifyPpi (&Private->IommuNotifyList);
  Status = DetectXhci (Private, TRUE, NULL);
  return Status;
}

