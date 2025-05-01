/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <AMD.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/IdsLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#include <Library/FabricResourceManagerLib.h>
#include <Library/TimerLib.h>

#include <IndustryStandard/Pci22.h>
#include <Protocol/PciIo.h>

#include <AmdUsb4.h>
#include <AmdUsb4/RingDescriptor.h>
#include <Library/AmdUsb4Lib.h>
#include <AmdUsb4/ControlPacket.h>

#define FILECODE USB4_AMDUSB4DXE_AMDUSB4DXE_FILECODE

#ifndef EXTERN
#define EXTERN extern
#endif

#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
typedef
EFI_STATUS
(EFIAPI * USB4_GET_USB4_HOST_COUNT) (
  IN     UINTN          *HostCount
);

typedef
EFI_STATUS
(EFIAPI * USB4_READ_CS) (
  IN     UINTN          HostIndex,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         ReadAddress,
  IN     UINT32         ReadSize,
     OUT UINT32         **DataOut,
     OUT UINT32         *DataSize
);

typedef
EFI_STATUS
(EFIAPI * USB4_WRITE_CS) (
  IN     UINTN          HostIndex,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         WriteAddress,
  IN     UINT32         *WriteDataSize,
  IN     UINT32         *WriteData
);

typedef
EFI_STATUS
(EFIAPI * USB4_RMW_CS) (
  IN     UINTN          HostIndex,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         Address,
  IN     UINT32         SizeInDW,
  IN     UINT32         *AndMask,
  IN     UINT32         *OrValue
);

typedef
EFI_STATUS
(EFIAPI * USB4_ROUTER_OPERATION) (
  IN     UINTN                                HostIndex,
  IN     UINT64                               TopologyID,
  IN     UINT16                               OpCode,
  IN     UINT32                               OperationMetadata,
  IN     UINT32                               OpSize,
  IN     UINT32                               *OpData,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               *CompletionStatus,
     OUT UINT32                               **Data
);

typedef
EFI_STATUS
(EFIAPI * USB4_PORT_OPERATION) (
  IN     UINTN                                HostIndex,
  IN     UINT64                               TopologyID,
  IN     UINT32                               AdapterNum,
  IN     UINT32                               Target,
  IN     UINT32                               RetimerIndex,
  IN     UINT32                               OpCode,
  IN     UINT32                               OpSize,
  IN     UINT32                               *OpData,
  IN     UINT32                               Metadata,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               **Data
);

typedef struct _AMD_USB4_PRE_OS_CM_PROTOCOL {
  USB4_GET_USB4_HOST_COUNT       GetUsb4HostCount;
  USB4_READ_CS                   ReadCs;
  USB4_WRITE_CS                  WriteCs;
  USB4_RMW_CS                    RMWCs;
  USB4_ROUTER_OPERATION          RouterOp;
  USB4_PORT_OPERATION            PortOp;
} AMD_USB4_PRE_OS_CM_PROTOCOL;

AMD_USB4_PRE_OS_CM_PROTOCOL AmdUsb4PreOsCmProtocol;
#endif

AMD_USB4_HOST_DATA    *gAmdUsb4HostData;

EXTERN EFI_STATUS
AmdUsb4GetUsb4Hosts (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN OUT UINT32                     *Usb4HostNum,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
  );


EXTERN EFI_STATUS
AmdUsb4InitHostMmio (
  IN     AMD_USB4_HOST_DATA         *pAmdUsbData,
  IN OUT UINT32                     *Usb4HostNum,
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *Usb4Host
  );


EXTERN EFI_STATUS
AmdUsb4AllocateRingMemory (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber
  );


EXTERN VOID
AmdUsb4DeAllocateRingMemory (
  IN     AMD_USB4_CM             *pCM,
  IN     UINT32                  RingNumber
  );


EXTERN EFI_STATUS
AmdUsb4HostConnectionInfo (
  IN     AMD_USB4_CM         *pCM,
  IN     UINT32              Usb4HostIndex
  );

#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
EXTERN VOID
WorkaroundForCVTest (
  IN  AMD_USB4_CM     *pCM,
  IN  UINT32          RingNumber
  );
#endif

EXTERN VOID
AmdDisableUsb4Ring (
  IN  AMD_USB4_CM     *pCM,
  IN  UINT32          RingNumber
  );

EXTERN UINTN
AmdUsb4Ring0Size (
  IN     UINT32                  RingNumber
  );

EXTERN VOID
AmdUsb4UpperBridgeMMIOSaveRestore (
  IN OUT AMD_USB4_HOST_PCI_ADDRESS  *pAmdUsb4HostPciAddress,
  IN     UINT32                     HostAmount,
  IN     BOOLEAN                    Action
  );

EFI_STATUS
AmdUsb4PreOsCmDxe ();


AMD_USB4_HOST_DATA    *gAmdUsb4HostData;
AMD_USB4_HOST_PCI_ADDRESS *gAmdUsb4HostPciAddress;

//---------------------------------------
//
//Platform Input -> need further update
//
//---------------------------------------
EXTERN UINT32  Usb4HostNumber;
EXTERN AMD_USB4_HOST_PCI_ADDRESS  Usb4HostList[];



EFI_STATUS
AmdUsb4AllocateBuffer (
  IN      UINTN                           Size,
      OUT EFI_PHYSICAL_ADDRESS            *Buffer
  )
{
  EFI_STATUS  Status;

  Status  = EFI_SUCCESS;

  Status = gBS->AllocatePages (
                  AllocateAnyPages,
                  EfiBootServicesData,
                  EFI_SIZE_TO_PAGES (Size),
                  Buffer
                  );
   return Status;
}


VOID
AmdUsb4FreeBuffer (
  IN      EFI_PHYSICAL_ADDRESS    Buffer,
  IN      UINTN                   Size
  )
{
  gBS->FreePages (
         Buffer,
         EFI_SIZE_TO_PAGES (Size)
         );
}


VOID
EFIAPI
AmdUsb4ReadyToBootNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);

  gBS->CloseEvent (Event);

  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}


VOID
EFIAPI
AmdUsb4ExitBootServiceNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
  EFI_STATUS          Status;
  UINT32              i;
  AMD_USB4_CM         *pCM;
  EFI_PCI_IO_PROTOCOL *Usb4PciIo;
  VOID                *RingMapedAddress;
  VOID                *Ptr;

  Status            = EFI_SUCCESS;
  i                 = 0;
  pCM               = NULL;
  Usb4PciIo         = NULL;
  RingMapedAddress  = NULL;
  Ptr               = NULL;
#endif

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);

#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
  for (i = 0; i < gAmdUsb4HostData->Usb4HostNum; i++) {
    pCM               = gAmdUsb4HostData->pCM[i];
    Usb4PciIo         = (EFI_PCI_IO_PROTOCOL*)pCM->pUsb4PciIo;
    Ptr               = pCM->pHostAddress;
    RingMapedAddress  = pCM->pRingMapping;


    WorkaroundForCVTest (pCM, 0);


    IDS_HDT_CONSOLE_USB4_DEBUG ("   Disable USB4 [%d] Ring Valid.\n", i);
    AmdDisableUsb4Ring (pCM, 0);
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Unmap USB4 buffer.\n");
    Status = Usb4PciIo->Unmap (
                          Usb4PciIo,
                          RingMapedAddress
                          );
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Unmap Status %r.\n", Status);
    IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Free USB4 buffer.\n");
    Status = Usb4PciIo->FreeBuffer (
                          Usb4PciIo,
                          EFI_SIZE_TO_PAGES ((UINT64)AmdUsb4Ring0Size (0)),
                          Ptr
                          );
    IDS_HDT_CONSOLE_USB4_DEBUG ("   FreeBuffer Status %r.\n", Status);
    IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
  }
#endif

  gBS->CloseEvent (Event);

  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}


VOID
EFIAPI
AmdUsb4PciEnumerationCompleteEventNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  EFI_STATUS  Status;
  VOID        *Interface;

  Status    = EFI_SUCCESS;
  Interface = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);
  //
  // Try to locate it because EfiCreateProtocolNotifyEvent will trigger it once when registration.
  // Just return if it is not found.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    IDS_HDT_CONSOLE_PSP_TRACE ("\t   gEfiPciEnumerationCompleteProtocolGuid is not installed yet.\n");
    return;
  }

  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}


VOID
EFIAPI
AmdUsb4PciIoNotify (
  IN  EFI_EVENT        Event,
  IN  VOID             *Context
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  *HandleBuffer;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  EFI_PCI_IO_PROTOCOL         *Usb4PciIo[AMD_USB4_HOST_AMOUNT];
  EFI_PCI_IO_PROTOCOL         *Usb4ParentPciIo;
  UINTN                       HandleCount;
  UINTN                       Index;
  UINT32                      Temp32;
  UINT64                      Temp64;
  UINT32                      USB4BAR0;
  UINT32                      i;
  UINT32                      ClassCode;
  UINT32                      Usb4PciCount;
  UINT64                      BufferSize;
  UINTN                       Usb4Seg;
  UINTN                       Usb4Bus;
  UINTN                       Usb4Dev;
  UINTN                       Usb4Fn;
  VOID                        *Ptr[AMD_USB4_HOST_AMOUNT];

  EFI_PHYSICAL_ADDRESS        RingMappedBaseAddr;
  VOID                        *RingMapedAddress[AMD_USB4_HOST_AMOUNT];
  AMD_USB4_CM                 *pCM;
  UINT32                      *Ptr32;
  UINT32                      DataSize;

  Status                = EFI_SUCCESS;
  HandleBuffer          = NULL;
  Usb4ParentPciIo       = NULL;
  HandleCount           = 0;
  Index                 = 0;
  Temp32                = 0;
  Temp64                = 0;
  USB4BAR0              = 0;
  i                     = 0;
  Usb4Seg               = 0;
  Usb4Bus               = 0;
  Usb4Dev               = 0;
  Usb4Fn                = 0;
  ClassCode             = 0;
  Usb4PciCount          = 0;
  BufferSize            = 0;

  RingMappedBaseAddr    = 0;
  pCM                   = NULL;
  Ptr32                 = NULL;
  DataSize              = 0;

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);

  for (i=0; i<AMD_USB4_HOST_AMOUNT; i++) {
    Usb4PciIo[i]        = NULL;
    RingMapedAddress[i] = NULL;
    Ptr[i]              = NULL;
  }

  //
  // Get all PCI IO protocols
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  IDS_HDT_CONSOLE_USB4_INFO ("   Status = %r, HandleCount = %d\n", Status, HandleCount);

  //
  // Loop all PCI devices for Usb4 Host
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID**)&PciIo
                    );
    if ( EFI_ERROR (Status) ) {
      continue;
    }

    // Check Class code
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          PCI_REVISION_ID_OFFSET,
                          1,
                          &ClassCode
                          );
    IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

    if ( (ClassCode >> 8) == 0x0C0340 ) {
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Find Usb4 Device. %d\n", Usb4PciCount);
      Usb4PciIo[Usb4PciCount] = PciIo;
      Usb4PciCount++;
    }
  }

  IDS_HDT_CONSOLE_USB4_DEBUG (
    "   Usb4PciCount = 0x%x, gAmdUsb4HostData->Usb4HostNum = 0x%x\n",
    Usb4PciCount,
    gAmdUsb4HostData->Usb4HostNum
    );

  if ( Usb4PciCount == gAmdUsb4HostData->Usb4HostNum ) {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Find all Usb4 Devices.\n");
    for (i=0; i<gAmdUsb4HostData->Usb4HostNum; i++) {

      pCM = gAmdUsb4HostData->pCM[i];
      IDS_HDT_CONSOLE_USB4_DEBUG ("   #%d pCM = 0x%lx\n", i, pCM);

      // 1. Get Usb4 BDF.
      Status = Usb4PciIo[i]->GetLocation (
                               Usb4PciIo[i],
                               &Usb4Seg,
                               &Usb4Bus,
                               &Usb4Dev,
                               &Usb4Fn
                               );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Usb4Bus = 0x%lx\n", Usb4Bus);

      // 2. Find Upper PCIe bridge
      Usb4ParentPciIo = NULL;
      for (Index = 0; Index < HandleCount; Index++) {
        Status = gBS->HandleProtocol (
                        HandleBuffer[Index],
                        &gEfiPciIoProtocolGuid,
                        (VOID**)&PciIo
                        );
        if ( EFI_ERROR (Status) ) {
          continue;
        }

        // Get Class code
        Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              PCI_REVISION_ID_OFFSET,
                              1,
                              &ClassCode
                              );
        IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

        // Get Sec Bus number
        Status = PciIo->Pci.Read (
                              PciIo,
                              EfiPciIoWidthUint32,
                              PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET,
                              1,
                              &Temp32
                              );
        IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

        if ( ( ClassCode & 0xFFFFFF00 ) == ( (PCI_CLASS_BRIDGE<<24) | (PCI_CLASS_BRIDGE_P2P<<16) | (PCI_IF_BRIDGE_P2P<<8) )
          && ( ((Temp32 >> 8) & 0xFF) == Usb4Bus ) ) {
          IDS_HDT_CONSOLE_USB4_DEBUG ("   Find upper PCIe bridge.\n");
          Usb4ParentPciIo = PciIo;
          break;
        }
      }

      // Enable Memory
      Status = Usb4PciIo[i]->Attributes (
                               Usb4PciIo[i],
                               EfiPciIoAttributeOperationGet,
                               0,
                               &Temp64
                               );
      IDS_HDT_CONSOLE_USB4_INFO ("   Read Usb4 Attributes 0x%lx\n", Temp64);

      Status = Usb4PciIo[i]->Attributes (
                               Usb4PciIo[i],
                               EfiPciIoAttributeOperationSupported,
                               0,
                               &Temp64
                               );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

      Temp64 &= (UINT64)(EFI_PCI_IO_ATTRIBUTE_MEMORY | EFI_PCI_IO_ATTRIBUTE_BUS_MASTER);
      Status = Usb4PciIo[i]->Attributes (
                               Usb4PciIo[i],
                               EfiPciIoAttributeOperationEnable,
                               Temp64,
                               NULL
                               );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

      Status = Usb4PciIo[i]->Attributes (
                               Usb4PciIo[i],
                               EfiPciIoAttributeOperationGet,
                               0,
                               &Temp64
                               );
      IDS_HDT_CONSOLE_USB4_INFO ("   Read Usb4 Attributes 0x%lx again\n", Temp64);

      if ( NULL != Usb4ParentPciIo ) {
        Status = Usb4ParentPciIo->Attributes (
                                    Usb4ParentPciIo,
                                    EfiPciIoAttributeOperationGet,
                                    0,
                                    &Temp64
                                    );
        IDS_HDT_CONSOLE_USB4_INFO ("   Read Usb4 upper bridge Attributes 0x%lx\n", Temp64);

        Status = Usb4ParentPciIo->Attributes (
                                   Usb4ParentPciIo,
                                   EfiPciIoAttributeOperationSupported,
                                   0,
                                   &Temp64
                                   );
        IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
        Temp64 &= (UINT64)(EFI_PCI_IO_ATTRIBUTE_MEMORY | EFI_PCI_IO_ATTRIBUTE_BUS_MASTER);
        Status = Usb4ParentPciIo->Attributes (
                                    Usb4ParentPciIo,
                                    EfiPciIoAttributeOperationEnable,
                                    Temp64,
                                    NULL
                                    );
        IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

        Status = Usb4ParentPciIo->Attributes (
                                    Usb4ParentPciIo,
                                    EfiPciIoAttributeOperationGet,
                                    0,
                                    &Temp64
                                    );
        IDS_HDT_CONSOLE_USB4_INFO ("   Read Usb4 upper bridge Attributes 0x%lx again\n", Temp64);
      }

      // Get BAR0
      Status = Usb4PciIo[i]->Pci.Read (
                                   Usb4PciIo[i],
                                   EfiPciIoWidthUint32,
                                   PCI_BASE_ADDRESSREG_OFFSET,
                                   1,
                                   &USB4BAR0
                                   );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
      USB4BAR0 = USB4BAR0 & 0xFFFFFFF0;
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Get Usb4 BAR0 = 0x%x\n", USB4BAR0);
      IDS_HDT_CONSOLE_USB4_DEBUG ("   *(volatile UINT32*)((UINTN)USB4BAR0) = 0x%x\n", *(volatile UINT32*)((UINTN)USB4BAR0));
      IDS_HDT_CONSOLE_USB4_ASSERT ( *(volatile UINT32*)((UINTN)USB4BAR0) != 0xFFFFFFFF );

      // DMAR
      BufferSize = (UINT64) AmdUsb4Ring0Size (0);
      IDS_HDT_CONSOLE_USB4_DEBUG ("   BufferSize = 0x%lx\n", BufferSize);

      Status = Usb4PciIo[i]->AllocateBuffer (
                               Usb4PciIo[i],
                               AllocateAnyPages,
                               EfiBootServicesData,
                               EFI_SIZE_TO_PAGES (BufferSize),
                               &Ptr[i],
                               0
                               );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Ptr[i] = 0x%lx\n", Ptr[i]);

      ZeroMem (Ptr[i], (UINTN)BufferSize);

      Status = Usb4PciIo[i]->Map (
                               Usb4PciIo[i],
                               EfiPciIoOperationBusMasterCommonBuffer,
                               Ptr[i],
                               &BufferSize,
                               (EFI_PHYSICAL_ADDRESS *)&RingMappedBaseAddr,
                               &RingMapedAddress[i]
                               );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
      IDS_HDT_CONSOLE_USB4_DEBUG (
        "   RingMappedBaseAddr = 0x%lx, RingMapedAddress[%d] = 0x%lx\n",
        RingMappedBaseAddr,
        i,
        RingMapedAddress[i]
        );

      // Set Address to DeviceAddress
      pCM->HIMmioBase               = (UINT64)USB4BAR0;
      pCM->RingTxBase[0]            = (UINT64)Ptr[i];
      pCM->RingRxBase[0]            = pCM->RingTxBase[0] + TX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR);
      pCM->RingTxDataBufferBase[0]  = pCM->RingRxBase[0] + RX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR);
      pCM->RingRxDataBufferBase[0]  = pCM->RingTxDataBufferBase[0] + TX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE;

      pCM->RingDevTxBase[0]            = RingMappedBaseAddr;
      pCM->RingDevRxBase[0]            = pCM->RingDevTxBase[0] + TX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR);
      pCM->RingDevTxDataBufferBase[0]  = pCM->RingDevRxBase[0] + RX_RING0_SIZE * sizeof (AMD_USB4_DESCRIPTOR);
      pCM->RingDevRxDataBufferBase[0]  = pCM->RingDevTxDataBufferBase[0] + TX_RING0_SIZE * MAX_CONTROL_PACKET_SIZE;

      IDS_HDT_CONSOLE_USB4_DEBUG (
        "   pCM->HIMmioBase = 0x%lx\n"
        "   pCM->RingTxBase[0] = 0x%lx, pCM->RingRxBase[0] = 0x%lx, pCM->RingTxDataBufferBase[0] = 0x%lx, pCM->RingRxDataBufferBase[0] = 0x%lx\n"
        "   pCM->RingDevTxBase[0] = 0x%lx, pCM->RingDevRxBase[0] = 0x%lx, pCM->RingDevTxDataBufferBase[0] = 0x%lx, pCM->RingDevRxDataBufferBase[0] = 0x%lx\n",
        pCM->HIMmioBase,
        pCM->RingTxBase[0],
        pCM->RingRxBase[0],
        pCM->RingTxDataBufferBase[0],
        pCM->RingRxDataBufferBase[0],
        pCM->RingDevTxBase[0],
        pCM->RingDevRxBase[0],
        pCM->RingDevTxDataBufferBase[0],
        pCM->RingDevRxDataBufferBase[0]
        );

      // Set Ring base address
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Set Ring base address.\n");
      Status = AmdUsb4HostInterfaceRing0Init (
                 pCM,
                 TX_RING0_SIZE,
                 RX_RING0_SIZE,
                 MAX_CONTROL_PACKET_SIZE,
                 MAX_CONTROL_PACKET_SIZE
                 );
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

      pCM->pUsb4PciIo   = (VOID*)Usb4PciIo[i];
      pCM->pHostAddress = (VOID*)Ptr[i];
      pCM->pRingMapping = (VOID*)RingMapedAddress[i];

#if !AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
      /*
      Disable Ring valid right after enable Ring valid may cause HW
      unexpected behavior, like yellow bang in Win11 device manager.
      Add 1 millisecond to avoid such issue.
      */

      MicroSecondDelay (1000);
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Disable USB4 [%d] Ring Valid.\n", i);
      AmdDisableUsb4Ring (pCM, 0);
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Unmap USB4 buffer.\n");
      Status = Usb4PciIo[i]->Unmap (
                            Usb4PciIo[i],
                            RingMapedAddress[i]
                            );
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Unmap Status %r.\n", Status);
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
      IDS_HDT_CONSOLE_USB4_DEBUG ("   Free USB4 buffer.\n");
      Status = Usb4PciIo[i]->FreeBuffer (
                            Usb4PciIo[i],
                            EFI_SIZE_TO_PAGES ((UINT64)AmdUsb4Ring0Size (0)),
                            Ptr[i]
                            );
      IDS_HDT_CONSOLE_USB4_DEBUG ("   FreeBuffer Status %r.\n", Status);
      IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
#endif
    }

    gBS->CloseEvent (Event);

  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG ("   Only get %d Usb4 Device. Quit.\n", Usb4PciCount);
  }
}


VOID
EFIAPI
AmdUsb4FchDoneEventNotify (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);
  AGESA_TESTPOINT (TpUsb4DxeEntry, NULL);
  AmdUsb4PreOsCmDxe ();
  AGESA_TESTPOINT (TpUsb4DxeExit, NULL);
  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}


#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
EFI_STATUS
IsExist (
  IN     AMD_USB4_CM    *pCM,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum
  )
{
  AMD_USB4_CM_CELL *PtrCell;

  PtrCell = NULL;

  if ( CsType == CS_TARGET_ROUTER ) {
    return AmdUsb4FindRouter (
             pCM,
             MASK_CM_BIT ((UINT32)(TopologyID>>32)),
             (UINT32)TopologyID,
             &PtrCell
             );
  } else if ( CsType == CS_TARGET_ADAPTER || CsType == CS_TARGET_PATH ) {
    return AmdUsb4FindAdapterByAdpNum (
             pCM,
             MASK_CM_BIT ((UINT32)(TopologyID>>32)),
             (UINT32)TopologyID,
             AdapterNum,
             &PtrCell
             );
  } else {
    return EFI_NOT_FOUND;
  }
}

EFI_STATUS
GetUsb4HostCount (
  IN     UINTN          *HostCount
  )
{
  *HostCount = (UINTN)gAmdUsb4HostData->Usb4HostNum;
  return EFI_SUCCESS;
}

EFI_STATUS
ReadCs (
  IN     UINTN          HostIndex,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         ReadAddress,
  IN     UINT32         ReadSize,
     OUT UINT32         **DataOut,
     OUT UINT32         *DataSize
  )
{
  AMD_USB4_CM    *pCM;

  pCM = gAmdUsb4HostData->pCM[HostIndex];

  if ( !EFI_ERROR(IsExist(pCM, TopologyID, CsType, AdapterNum)) ) {
    return AmdUsb4Ring0ReadCS (
             pCM,
             TopologyID,
             CsType,                  // CsType
             AdapterNum,              // AdapterNum
             ReadAddress,             // ReadAddress
             ReadSize,                // ReadSize 1 DWs
             DataOut,                 // DataOut
             DataSize                 // DataSize
             );
  } else {
    return EFI_NOT_FOUND;
  }
}


EFI_STATUS
WriteCs (
  IN     UINTN          HostIndex,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         WriteAddress,
  IN     UINT32         *WriteDataSize,
  IN     UINT32         *WriteData
  )
{
  AMD_USB4_CM    *pCM;

  pCM = gAmdUsb4HostData->pCM[HostIndex];

  if ( !EFI_ERROR(IsExist(pCM, TopologyID, CsType, AdapterNum)) ) {
    return AmdUsb4Ring0WriteCS (
             pCM,
             TopologyID,
             CsType,                  // CsType
             AdapterNum,              // AdapterNum
             WriteAddress,            // WriteAddress
             WriteDataSize,           // Write Data Size
             WriteData
             );
  } else {
    return EFI_NOT_FOUND;
  }
}


EFI_STATUS
RMWCs (
  IN     UINTN          HostIndex,
  IN     UINT64         TopologyID,
  IN     UINT32         CsType,
  IN     UINT32         AdapterNum,
  IN     UINT32         Address,
  IN     UINT32         SizeInDW,
  IN     UINT32         *AndMask,
  IN     UINT32         *OrValue
  )
{
  AMD_USB4_CM    *pCM;

  pCM = gAmdUsb4HostData->pCM[HostIndex];

  if ( !EFI_ERROR(IsExist(pCM, TopologyID, CsType, AdapterNum)) ) {
    return AmdUsb4Ring0RMWCS (
             pCM,
             TopologyID,
             CsType,             // CsType
             AdapterNum,         // AdapterNum
             Address,            // Address
             SizeInDW,           // Size
             AndMask,
             OrValue
             );
  } else {
    return EFI_NOT_FOUND;
  }
}

EFI_STATUS
RouterOp (
  IN     UINTN                                HostIndex,
  IN     UINT64                               TopologyID,
  IN     UINT16                               OpCode,
  IN     UINT32                               OperationMetadata,
  IN     UINT32                               OpSize,
  IN     UINT32                               *OpData,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               *CompletionStatus,
     OUT UINT32                               **Data
  )
{
  EFI_STATUS          Status;
  AMD_USB4_CM         *pCM;
  AMD_USB4_CM_ROUTER  *pRouter;

  Status  = EFI_SUCCESS;
  pCM     = gAmdUsb4HostData->pCM[HostIndex];
  pRouter = NULL;

  Status = AmdUsb4FindRouter (
             pCM,
             MASK_CM_BIT ((UINT32)(TopologyID>>32)),
             (UINT32)TopologyID,
             (AMD_USB4_CM_CELL**)&pRouter
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Find Router Status %r\n", __FUNCTION__, Status);
  if ( EFI_ERROR (Status) || pRouter == NULL ) {
    return EFI_NOT_FOUND;
  }

  return RouterOperation2 (
           pCM,
           pRouter,
           OpCode,
           OperationMetadata,
           OpSize,
           OpData,
           CompletionMetadata,
           CompletionStatus,
           Data
           );
}


EFI_STATUS
PortOp (
  IN     UINTN                                HostIndex,
  IN     UINT64                               TopologyID,
  IN     UINT32                               AdapterNum,
  IN     UINT32                               Target,
  IN     UINT32                               RetimerIndex,
  IN     UINT32                               OpCode,
  IN     UINT32                               OpSize,
  IN     UINT32                               *OpData,
  IN     UINT32                               Metadata,
     OUT UINT32                               *CompletionMetadata,
     OUT UINT32                               **Data
  )
{
  EFI_STATUS                Status;
  AMD_USB4_CM               *pCM;
  AMD_USB4_CM_LANE_ADAPTER  *pLaneAdapter;

  Status        = EFI_SUCCESS;
  pCM           = gAmdUsb4HostData->pCM[HostIndex];
  pLaneAdapter  = NULL;

  Status = AmdUsb4FindAdapterByAdpNum (
             pCM,
             MASK_CM_BIT ((UINT32)(TopologyID>>32)),
             (UINT32)TopologyID,
             AdapterNum,
             (AMD_USB4_CM_CELL**)&pLaneAdapter
             );
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a Find Lane Adapter Status %r\n", __FUNCTION__, Status);
  if ( EFI_ERROR (Status) || pLaneAdapter == NULL ) {
    return EFI_NOT_FOUND;
  }

  return AmdUsb4PortOperation (
           pCM,
           pLaneAdapter,
           Target,
           RetimerIndex,
           OpCode,
           OpSize,
           OpData,
           &Metadata,
           CompletionMetadata,
           Data
           );
}

AMD_USB4_PRE_OS_CM_PROTOCOL AmdUsb4PreOsCmProtocol = {
  GetUsb4HostCount,     ///< Get USB4 host amount in system.
  ReadCs,               ///< Read Configuration Space
  WriteCs,              ///< Write Configuration Space
  RMWCs,                ///< Read, Modify and Write Configuration Space
  RouterOp,             ///< Router Operation
  PortOp                ///< Port Operation
};
#endif

/**
 * @brief   Notify function for End of DXE.
 *
 * @param   Event            The registered event
 * @param   Context

 * @return
 */
VOID
EFIAPI
AmdUsb4EndOfDxe (
  IN EFI_EVENT                          Event,
  IN VOID                               *Context
  )
{
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - Start\n", __FUNCTION__);
  gBS->CloseEvent (Event);
  IDS_HDT_CONSOLE_USB4_DEBUG ("%a - End\n", __FUNCTION__);
}

/**
 * @brief   Usb4 Pre-OS Connection Manager for DXE.
 *
 * @return  EFI_STATUS      EFI_SUCCESS.
 *                          Others.
 */
EFI_STATUS
AmdUsb4PreOsCmDxe (
  )
{
  UINT32                 HostIndex;
  AMD_USB4_CM            *pCM;
  AMD_USB4_CM_ROUTER     *pRouter;
  EFI_STATUS             Status;

  HostIndex = 0;
  pCM       = NULL;
  pRouter   = NULL;
  Status    = EFI_SUCCESS;

  IDS_HDT_CONSOLE_USB4_INFO ("%a Enter\n", __FUNCTION__);

  //
  // Allocate memory for the private data
  //
  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (AMD_USB4_HOST_DATA),
                  &gAmdUsb4HostData
                  );
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for USB4 Host Data\n");
    return Status;
  }

  ZeroMem (gAmdUsb4HostData, sizeof (AMD_USB4_HOST_DATA));

  gAmdUsb4HostData->AmdUsb4Version = AMD_USB4_CM_VERSION;

  //
  // Get USB4 Host List
  //
  Status = AmdUsb4GetUsb4Hosts (
             gAmdUsb4HostData,
             &Usb4HostNumber,
             Usb4HostList
             );
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Cannot get USB4 Host!\n");
    return Status;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (AMD_USB4_HOST_PCI_ADDRESS) * Usb4HostNumber,
                  &gAmdUsb4HostPciAddress
                  );
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
  CopyMem (gAmdUsb4HostPciAddress, Usb4HostList, sizeof (AMD_USB4_HOST_PCI_ADDRESS) * Usb4HostNumber);

  AmdUsb4UpperBridgeMMIOSaveRestore (
    gAmdUsb4HostPciAddress,
    Usb4HostNumber,
    TRUE
    );

  //
  // Enable USB4 Host MMIO
  //
  Status = AmdUsb4InitHostMmio (
             gAmdUsb4HostData,
             &Usb4HostNumber,
             Usb4HostList
             );
  if (Status != EFI_SUCCESS) {
    IDS_HDT_CONSOLE_USB4_ERROR (" Cannot Enable USB4 Host MMIO!\n");
    return Status;
  }

#if AMD_USB4_DEBUG_BP_BEFORE_CM
  IDS_HDT_CONSOLE_USB4_DEBUG (" Check CMOS[0x88] Value.\n");
  IoWrite8 (0x72, 0x88);
  if ( IoRead8 (0x73) == 0x5A ) {
    IDS_HDT_CONSOLE_USB4_DEBUG (
      " CMOS[0x88] Value is value. go into infinite loop until the it changes to other value\n"
      );
    while (1) {
      if ( IoRead8 (0x73) == 0x5A ) {
        gBS->Stall (500000);  // Stall 500ms
      } else {
        break;
      }
    }
  } else {
    IDS_HDT_CONSOLE_USB4_DEBUG (" CMOS[0x88] Value is not 0x5A. Keep going.\n");
  }
#endif

  //
  // Loop for all USB4 Host
  //
  for (HostIndex = 0; HostIndex < Usb4HostNumber; HostIndex++) {
    IDS_HDT_CONSOLE_USB4_INFO (" Start Host Index: %d\n", HostIndex);
    //
    // Init CM data structure for Host
    //
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (AMD_USB4_CM),
                    &(gAmdUsb4HostData->pCM[HostIndex])
                    );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for USB4 CM!\n");
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem (gAmdUsb4HostData->pCM[HostIndex], sizeof (AMD_USB4_CM));
    pCM = gAmdUsb4HostData->pCM[HostIndex];
    pCM->HIMmioBase = gAmdUsb4HostData->Usb4HostMmioBase[HostIndex];
    IDS_HDT_CONSOLE_USB4_DEBUG (" pCM address=0x%lx, pCM->HIMmioBase=0x%lx\n", pCM, pCM->HIMmioBase);
    CopyMem (&(pCM->HostRouterPciAddress), &(Usb4HostList[HostIndex]), sizeof (AMD_USB4_HOST_PCI_ADDRESS));

    pCM->TBT3WaitCL0 = PcdGet32 (PcdAmdUsb4WaitCL0TBT3);
    pCM->USB4WaitCL0 = PcdGet32 (PcdAmdUsb4WaitCL0USB4);


    //
    //   Allocate Memory for Ring0 Interface
    //
    Status = AmdUsb4AllocateRingMemory (
               pCM,
               0
               );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for Ring0!\n");
      return EFI_OUT_OF_RESOURCES;
    }

    //
    // Allocated 64 double words buffer for Tx and Rx buffer
    //
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (UINT32) * 64,
                    &(pCM->PtrControlPacketTxBuffer)
                    );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for PtrControlPacketTxBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrControlPacketTxBuffer, sizeof (UINT32) * 64);

    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (UINT32) * 64,
                    &(pCM->PtrControlPacketRxBuffer)
                    );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for PtrControlPacketRxBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrControlPacketRxBuffer, sizeof (UINT32) * 64);

    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (UINT32) * 64,
                    &(pCM->PtrControlPacketClearBuffer)
                    );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for PtrControlPacketClearBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrControlPacketClearBuffer, sizeof (UINT32) * 64);

    //
    // Allocate Memory for DROM buffer
    //
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (UINT8) * MAX_CM_DROM_BUFFER_SIZE,
                    &(pCM->PtrDROMBuffer)
                    );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for PtrDROMBuffer!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrDROMBuffer, sizeof (UINT8) * MAX_CM_DROM_BUFFER_SIZE);

    //
    // Allocate memory for CM data pCMData
    //
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (AMD_USB4_CM_CELL) * MAX_CM_DATA_ARRAY_AMOUNT,
                    &(pCM->PtrCmData)
                    );
    if (Status != EFI_SUCCESS) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Cannot allocate memory for CM Data!\n");
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (pCM->PtrCmData, sizeof (AMD_USB4_CM_CELL) * MAX_CM_DATA_ARRAY_AMOUNT);
    pCM->CmDataIndex = 0;

    //
    //    Init H/I for Ring0
    //
    Status = AmdUsb4HostInterfaceRing0Init (
               pCM,
               TX_RING0_SIZE,
               RX_RING0_SIZE,
               MAX_CONTROL_PACKET_SIZE,
               MAX_CONTROL_PACKET_SIZE
               );

    //
    // Update USB3/PCIe/DP info that connect to USB4 Host Router
    //
    Status = AmdUsb4HostConnectionInfo (pCM, HostIndex);

    //
    // Initialize DisplayPort Plugged Adapter list
    //
    InitializeListHead (&pCM->DpInPluggedList);
    InitializeListHead (&pCM->DpOutPluggedList);

    //
    // Set TMU mode
    //
    pCM->CmTmuMode = TMU_MODE_HIFI_BI; // set to HIFI_BI for Pre-OS CM

    //
    // Initialize RS-FEC Gen3 and Gen2
    //
    pCM->RequestRsFecGen3 = ((PcdGet8 (PcdUsb4Gen3RsFec) >> HostIndex) & ((UINT8) BIT0)) ? 1 : 0;
    pCM->RequestRsFecGen2 = ((PcdGet8 (PcdUsb4Gen2RsFec) >> HostIndex) & ((UINT8) BIT0)) ? 1 : 0;
    pCM->LinkSpeed  = ((PcdGet8 (PcdUsb4Gen3Support) >> HostIndex) & ((UINT8) BIT0)) ? 1 : 0;

    pCM->HPDWaitTimeout.OldTimerValue = 0;
    pCM->HPDWaitTimeout.EndValue      = 0;

    //
    // Init Host Router
    //
    //    Enumerate Host Router
    //
    pRouter = (AMD_USB4_CM_ROUTER*)&(pCM->PtrCmData[pCM->CmDataIndex]);
    pCM->CmDataIndex++;

    AGESA_TESTPOINT (TP_HOST_ROUTER_INIT, NULL);

    Status = HostRouterInitialization (pCM, pRouter);
    if ( Status != EFI_SUCCESS ) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Host Router Init fail!\n");
      return EFI_DEVICE_ERROR;
    }

    AGESA_TESTPOINT (TP_DEVICE_ROUTER_INIT, NULL);

    //
    // Detect and Init Device Router
    //
    Status = DeviceRouterInitialization (pCM, pRouter);
    if ( Status == EFI_DEVICE_ERROR ) {
      IDS_HDT_CONSOLE_USB4_ERROR (" Device Router Init fail!\n");
      return EFI_DEVICE_ERROR;
    }

    AGESA_TESTPOINT (TP_WAIT_DPOUT, NULL);

    //
    // Check Timer requirement for DP OUT
    //
    do {
      AmdUsb4ClearReceiveRing (pCM);
    }  while ( ( HasItTimedOut (&(pCM->HPDWaitTimeout)) != EFI_TIMEOUT ) );

    AGESA_TESTPOINT (TP_WAIT_DPOUT | 1, NULL);
    PreEnableDpTunneling (pCM);

    //
    // Setup Path
    //
    AGESA_TESTPOINT (TP_DP_TUNNELING, NULL);
    EnableDpTunneling (pCM);
    AGESA_TESTPOINT (TP_USB_TUNNELING, NULL);
    EnableUsb3Tunneling (pCM);
    AGESA_TESTPOINT (TP_PCI_TUNNELING, NULL);
    EnablePcieTunneling (pCM);

    //
    // Set Timer event
    //

    //
    // Check if there is Child device router connected.
    // Set topology id valid to 0 if no device router.
    //
    AnyDeviceRouterConnected (pCM, pRouter);

    //
    // Disable USB4 Ring
    //
    AmdDisableUsb4Ring (pCM, 0);
    // Free Ring and Ring buffer
    AmdUsb4DeAllocateRingMemory (pCM, 0);
  }

  AmdUsb4UpperBridgeMMIOSaveRestore (
    gAmdUsb4HostPciAddress,
    Usb4HostNumber,
    FALSE
    );

  return EFI_SUCCESS;
}


VOID
AmdUsb4HookProtocol (
  )
{
  EFI_STATUS                  Status;
  EFI_EVENT                   Evt;
  VOID                        *Registration;
  EFI_HANDLE                  Handle;

  Status        = EFI_SUCCESS;
  Evt           = NULL;
  Registration  = NULL;
  Handle        = NULL;

  IDS_HDT_CONSOLE_USB4_INFO ("%a - Start\n", __FUNCTION__);

#if AMD_USB4_DEBUG_EXPOSE_PREOS_CM_PROTOCOL
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install USB4 protocol\n");
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gAmdUsb4ProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &AmdUsb4PreOsCmProtocol
                  );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install USB4 protocol Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));
#endif

  //
  // Register notify function on ReadyToBoot Event.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install ReadyToBoot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmdUsb4ReadyToBootNotify,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install ReadyToBoot Event Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install Legacy Boot Event\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmdUsb4ReadyToBootNotify,
                  NULL,
                  &gEfiEventLegacyBootGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install Legacy Boot Event Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmdUsb4ExitBootServiceNotify,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install Boot Service Event Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));


  //
  // Register notify function on gEfiPciIoProtocolGuid.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install gEfiPciIoProtocolGuid Event.\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmdUsb4PciIoNotify,
                  NULL,
                  &gEfiPciIoProtocolGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install gEfiPciIoProtocolGuid Event Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

  IDS_HDT_CONSOLE_USB4_DEBUG ("   Register gEfiPciIoProtocolGuid Event.\n");
  Status = gBS->RegisterProtocolNotify (
                  &gEfiPciIoProtocolGuid,
                  Evt,
                  &Registration
                  );
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Register gEfiPciIoProtocolGuid Event Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));

  //
  // Register notify function to get HSP base address.
  //
  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install gEfiPciEnumerationCompleteProtocolGuid callback\n");
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_NOTIFY,
    AmdUsb4PciEnumerationCompleteEventNotify,
    NULL,
    &Registration
    );

  IDS_HDT_CONSOLE_USB4_DEBUG ("   Install gFchInitDonePolicyProtocolGuid callback\n");
  EfiCreateProtocolNotifyEvent (
    &gFchInitDonePolicyProtocolGuid,
    TPL_NOTIFY,
    AmdUsb4FchDoneEventNotify,
    NULL,
    &Registration
    );

  IDS_HDT_CONSOLE_PSP_TRACE ("    Register notify function on End of Dxe.\n");
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AmdUsb4EndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Evt
                  );
  IDS_HDT_CONSOLE_PSP_TRACE ("    Register notify function on End of Dxe Status %r\n", Status);
  IDS_HDT_CONSOLE_USB4_ASSERT (!EFI_ERROR (Status));


  IDS_HDT_CONSOLE_USB4_INFO ("%a - End\n", __FUNCTION__);
}


/*++

Routine Description:

Arguments:

Returns:

  EFI_STATUS

--*/
EFI_STATUS
AmdUsb4DxeEntry (
  IN     EFI_HANDLE        ImageHandle,
  IN     EFI_SYSTEM_TABLE  *SystemTable
  )
{
  AGESA_TESTPOINT (TpUsb4DxeEntry, NULL);
  IDS_HDT_CONSOLE_USB4_INFO ("%a Entry\n", __FUNCTION__);

  if ( PcdGetBool (PcdAmdUsb4CmEnable) == 0 ) {
    IDS_HDT_CONSOLE_USB4_INFO ("\nUsb4 do not support\n");
    return (EFI_SUCCESS);
  }

#if AMD_USB4_DEBUG_PRE_OS_CM_DISABLE
  IDS_HDT_CONSOLE_USB4_DEBUG ("\nSkip USB4 Pre-OS CM.\n");
  return (EFI_SUCCESS);
#endif

  if ( AMD_USB4_PRE_OS_CM_PEI_SUPPORT == 1 ) {
    IDS_HDT_CONSOLE_USB4_INFO (" Usb4 Pre-OS CM DXE do not support\n");
    return (EFI_SUCCESS);
  }

  /*
  Pre-OS CM uses FCHUSB4PDSLVI2C::PD_LL_port0_data to know if there is device router connected.
  If there is device router connected, Pre-OS CM will wait extra time in DetectRouter for DFP
  Adapter_State flippint to CL0.
  Some slow device need more time to establish link and register FCHUSB4PDSLVI2C::PD_LL_port0_data
  may not be set with correct value when Pre-OS read it. It will cause Pre-OS CM do not wait
  extra time for Adapter_State flipping to CL0.
  So to add a delay before Pre-OS CM is need to avoid this situation happened.
  */
  MicroSecondDelay (PcdGet32(PcdAmdUsb4DelayBeforePreOSCM));

  //AmdUsb4PreOsCmDxe ();
  AmdUsb4HookProtocol ();

  IDS_HDT_CONSOLE_USB4_INFO ("%a Exit\n", __FUNCTION__);
  AGESA_TESTPOINT (TpUsb4DxeExit, NULL);

  return (EFI_SUCCESS);
}

