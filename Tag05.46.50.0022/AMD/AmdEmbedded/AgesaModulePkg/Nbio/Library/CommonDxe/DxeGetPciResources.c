/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#include <DxeGetPciResources.h>

#define FILECODE        NBIO_LIBRARY_COMMONDXE_DXEGETPCIRESOURCES_FILECODE

LIST_ENTRY              *RootBridgeListHead = NULL;
UINTN                   TotalRootBridges = 0;

AMD_PCI_RESOURCES_PROTOCOL  mAmdPciResourcesProtocol = {
    AmdPciResourcesGetNumberOfRootBridges,
    AmdPciResourcesGetRootBridgeInfo,
    AmdPciResourcesGetNumberOfRootPorts,
    AmdPciResourcesGetRootPortInfo,
    AmdPciResourcesGetNumberOfFixedResources,
    AmdPciResourcesGetFixedResourceInfo
};

/**
 * Init function to collect pci resources. This gets called from CallbackAfterPciIo in the dxe phase after pcie enumeration is done
 * Includes root bridges, root ports and fixed resources(IOMMU and IOAPIC base) for each root bridge
 * Does not include root bridge MMIO/PMMIO/IO resources since they can be found via the EDKII PciRootBridge Protocols
 *
 * @param[in]      PCIe_PLATFORM_CONFIG   Pointer to PCIe_PLATFORM_CONFIG
 *
 * @retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
EFIAPI
AmdCollectPciResourcesInit (
     IN       PCIe_PLATFORM_CONFIG      *Pcie
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  PCI_ROOT_BRIDGE_OBJECT_INSTANCE       *RootBridge;
  GNB_HANDLE                            *GnbHandle;
  LIST_ENTRY                            ListHeadReal;
  UINTN                                 Index = 0;

  RootBridgeListHead = &ListHeadReal;
  InitializeListHead (RootBridgeListHead);

  GnbHandle = NbioGetHandle (Pcie);

  while (GnbHandle != NULL) {
    RootBridge = AllocateZeroPool(sizeof (PCI_ROOT_BRIDGE_OBJECT_INSTANCE));

    if (RootBridge == NULL) {
      IDS_HDT_CONSOLE (GNB_TRACE, "%a ERROR: Root Bridge instance allocation failed\n", __FUNCTION__);
      Status = EFI_OUT_OF_RESOURCES;
      return Status;
    }
    ++Index;
    RootBridge->Signature = PCI_ROOT_BRIDGE_OBJECT_INSTANCE_SIGNATURE;
    RootBridge->RootBridgeObject.BaseBusNumber = GnbHandle->Address.Address.Bus;
    RootBridge->RootBridgeObject.Segment = GnbHandle->Address.Address.Segment;
    RootBridge->RootBridgeObject.SocketId = GnbHandle->SocketId;
    RootBridge->RootBridgeObject.Index = Index;

    RootBridge->RootPortList = AllocatePool (sizeof (LIST_ENTRY));

    if(RootBridge->RootPortList == NULL)
    {
      IDS_HDT_CONSOLE (GNB_TRACE, "%a ERROR: Root Bridge instance allocation failed\n", __FUNCTION__);
      Status = EFI_OUT_OF_RESOURCES;
      return Status;
    }

    InitializeListHead (RootBridge->RootPortList);
    CollectRootPortInfo (GnbHandle, RootBridge);

    RootBridge->FixedResourcesList = AllocatePool (sizeof (LIST_ENTRY));
    if(RootBridge->FixedResourcesList == NULL)
    {
      IDS_HDT_CONSOLE (GNB_TRACE, "%a ERROR: Root Bridge instance allocation failed\n", __FUNCTION__);
      Status = EFI_OUT_OF_RESOURCES;
      return Status;
    }

    InitializeListHead (RootBridge->FixedResourcesList);
    CollectFixedResourcesInfo (GnbHandle, RootBridge);

    //Insert root bridge to the tail
    InsertTailList (RootBridgeListHead, &RootBridge->Link);

    ++TotalRootBridges;
    //Move to the next root bridge
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }
  return Status;
}

/**
  *Internal function to collect Root Port instances under each Root Bridge instance
  *Device, Function numbers, bridge and endpoint interrupts relative to IOPIC.

  *@param[in]      GnbHandle    Instance of GNB handle
  *@param[in,out]  RootBridge - Single Root Bridge Instance

  *@retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
STATIC
CollectRootPortInfo (
  IN      GNB_HANDLE                      *GnbHandle,
  IN OUT  PCI_ROOT_BRIDGE_OBJECT_INSTANCE *RootBridge
)
{
    PCIe_ENGINE_CONFIG                  *PcieEngine;
    PCIe_WRAPPER_CONFIG                 *PcieWrapper;
    EFI_STATUS                           Status = EFI_SUCCESS;
    PCI_ROOT_PORT_OBJECT_INSTANCE       *RootPort;
    UINTN                               Index = 0;

    PcieWrapper = PcieConfigGetChildWrapper (GnbHandle);

    while (PcieWrapper != NULL) {
        PcieEngine = PcieConfigGetChildEngine (PcieWrapper);
        while (PcieEngine != NULL) {

            RootPort = AllocatePool (sizeof (PCI_ROOT_PORT_OBJECT_INSTANCE));

            if(RootPort == NULL)
            {
                IDS_HDT_CONSOLE (GNB_TRACE, "%a ERROR: Root Port Instance Allocation Failed\n", __FUNCTION__);
                Status = EFI_OUT_OF_RESOURCES;
                return Status;
            }
            RootPort->Signature = PCI_ROOT_PORT_OBJECT_INSTANCE_SIGNATURE;
            ++Index;
            RootPort->RootPortObject.Index = Index;
            RootPort->RootPortObject.PortPresent = PcieEngine->Type.Port.PortData.PortPresent;
            RootPort->RootPortObject.Device = PcieEngine->Type.Port.PortData.DeviceNumber;
            RootPort->RootPortObject.Function = PcieEngine->Type.Port.PortData.FunctionNumber;
            RootPort->RootPortObject.SlotNum = PcieEngine->Type.Port.PortData.SlotNum;

            CollectInterruptInfo(GnbHandle, RootPort, PcieEngine);

            //Insert root port to the tail
            InsertTailList (RootBridge->RootPortList, &RootPort->Link);

            RootBridge->NumberOfRootPorts++;
            //Move to the next root port
            PcieEngine = PcieLibGetNextDescriptor (PcieEngine);
        }
        PcieWrapper = PcieLibGetNextDescriptor (PcieWrapper);
    }
    return Status;
}

/**
  *Internal function which calculates Redirection table entry for mapped bridge interrupt and
  *redirection table entries for mapped INT A/B/C/D for endpoint interrupts
  *
  *@param[in]      GnbHandle    Instance of GNB handle
  *@param[in,out]  RootBridge - Single Root Port Instance
  *@param[in]      PcieEngine   Single PcieEngine Instance
  *
**/
VOID
STATIC
CollectInterruptInfo (
  IN      GNB_HANDLE                      *GnbHandle,
  IN OUT  PCI_ROOT_PORT_OBJECT_INSTANCE   *RootPort,
  IN      PCIe_ENGINE_CONFIG              *PcieEngine
)
{
    UINTN                                 RelativeInterrupt;
    UINT32                                InterruptSwizzle;
    UINT8                                 *InterruptSwizzleArray;
    IOAPIC_BR_INTERRUPT_ROUTING_STRUCT    IoapicBrInterruptRouting;
    UINTN                                 Index;
    UINT32                                SmnReg;

    InterruptSwizzleArray = (UINT8 *)&InterruptSwizzle;

    SmnReg = SMN_IOAPIC_BR_INTERRUPT_ROUTING_ADDRESS + (PcieEngine->Type.Port.PortId * 4);

    SmnRegisterReadS ( GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
                          NBIO_SPACE(GnbHandle, SmnReg), &(IoapicBrInterruptRouting.Value));
    RootPort->RootPortObject.BridgeInterrupt = IoapicBrInterruptRouting.Field.Br_ext_Intr_map;

    // Swizzle Interrupts 0->3 (INTA->INTD) according to the swizzle bits.
    //rotates the interrupts in to the proper swizzle position (4 interrupts 0->3 and then rotates 8 bits times swz)
    InterruptSwizzle = LRotU32 (0x03020100, IoapicBrInterruptRouting.Field.Br_ext_Intr_swz * 8);

    //4 interrupts per group
    RelativeInterrupt = IoapicBrInterruptRouting.Field.Br_ext_Intr_grp * 4;
    for (Index = 0; Index < 4; Index++) {
         RootPort->RootPortObject.EndpointInterruptArray[Index] = RelativeInterrupt + InterruptSwizzleArray[Index];
    }
}

/**
  *Internal function which gets the IOMMU base
  *
  *@param[in]      GnbHandle       Instance of GNB handle
  *@param[in,out]  FixedResource - Single Fixed Resource Instance
  *
  *@retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
STATIC
GetIommuBase (
  IN       GNB_HANDLE                      *GnbHandle,
  IN OUT   FIXED_RESOURCES_OBJECT_INSTANCE *FixedResource
  )
{
    UINT16                  CapabilityOffset;
    UINT64                  BaseAddress = 0x0;
    UINT32                  Value;
    PCI_ADDR                GnbIommuPciAddress;

    GnbIommuPciAddress = NbioGetHostPciAddress (GnbHandle);
    GnbIommuPciAddress.Address.Function = 0x2;

    if (GnbLibPciIsDevicePresent (GnbIommuPciAddress.AddressValue, NULL)) {

      CapabilityOffset = GnbLibFindPciCapability (GnbIommuPciAddress.AddressValue, 0xF, NULL);
      GnbLibPciRead (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x8), AccessWidth32, &Value, NULL);
      BaseAddress = (UINT64) LShiftU64(Value, 32);
      GnbLibPciRead (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x4), AccessWidth32, &Value, NULL);
      BaseAddress |= Value;
    }
    FixedResource->FixedResourceObject.Address = BaseAddress;

    return EFI_SUCCESS;
}

/**
  *Internal function which gets the IOAPIC base
  *
  *@param[in]      GnbHandle       Instance of GNB handle
  *@param[in,out]  FixedResource - Single Fixed Resource Instance
  *
  *@retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
STATIC
GetIoApicBase (
  IN       GNB_HANDLE                      *GnbHandle,
  IN OUT   FIXED_RESOURCES_OBJECT_INSTANCE *FixedResource
  )
{
    UINT64        BaseAddress = 0x0;
    UINT32        Value;

    SmnRegisterReadS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
                                   NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_HI_ADDRESS),
                                   &(Value));

    BaseAddress = (UINT64) LShiftU64(Value, 32);

    SmnRegisterReadS (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
                                   NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS),
                                   &(Value));

    BaseAddress |= Value;

    FixedResource->FixedResourceObject.Address = BaseAddress;

    return EFI_SUCCESS;
}

/**
  *Internal function to Collect fixed resource instances under each Root Bridge instance
  *Includes IOAPIC and IOMMU base addresses.

  *@param[in]      GnbHandle    Instance of GNB handle
  *@param[in,out]  RootBridge - Single Root Bridge Instance

  *@retval         EFI_SUCCESS, various EFI FAILUREs.
**/
EFI_STATUS
STATIC
CollectFixedResourcesInfo (
  IN      GNB_HANDLE                      *GnbHandle,
  IN OUT  PCI_ROOT_BRIDGE_OBJECT_INSTANCE *RootBridge
)
{
    EFI_STATUS                           Status;
    FIXED_RESOURCES_OBJECT_INSTANCE      *FixedResource;
    UINTN                                Index;

    RootBridge->NumberOfFixedResources = NBIO_MAX_FIXED_RESOURCES;

    EFI_STATUS (*fun_ptr_arr[])(GNB_HANDLE *, FIXED_RESOURCES_OBJECT_INSTANCE *) = {GetIommuBase, GetIoApicBase};

    for (Index = 0; Index < NBIO_MAX_FIXED_RESOURCES; Index ++) {
       FixedResource = AllocatePool (sizeof (FIXED_RESOURCES_OBJECT_INSTANCE));

       if(FixedResource == NULL)
       {
         IDS_HDT_CONSOLE (GNB_TRACE, "%a ERROR: Fixed Resource Instance Allocation Failed\n", __FUNCTION__);
         Status = EFI_OUT_OF_RESOURCES;
         return Status;
       }
       FixedResource->Signature = PCI_FIXED_RESOURCES_OBJECT_INSTANCE_SIGNATURE;
       FixedResource->FixedResourceObject.Index = Index + 1;
       FixedResource->FixedResourceObject.ResourceType = Index;
       Status = (*fun_ptr_arr[Index])(GnbHandle, FixedResource);

       //Insert to the tail
       InsertTailList (RootBridge->FixedResourcesList, &FixedResource->Link);
    }
    return Status;
}

/**
  *Public function to get the number of root bridges
  *
  *@param[in]      This                  Pointer to AMD_PCI_RESOURCES_PROTOCOL
  *@param[in,out]  NumberOfRootBridges   Number of root bridges returned
  *
**/
EFI_STATUS
EFIAPI
AmdPciResourcesGetNumberOfRootBridges (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    OUT      UINTN                                 *NumberOfRootBridges
)
{
   *NumberOfRootBridges = TotalRootBridges;
   return EFI_SUCCESS;
}

/**
  *Public function to get the root bridge info
  *
  *@param[in]      This                  Pointer to AMD_PCI_RESOURCES_PROTOCOL
  *@param[in]     RootBridgeIndex        Root bridge index for which the root bridge info will be returned
  *@param[out]    RootBridgeInfo         Returned Root bridge info
  *
**/
EFI_STATUS
EFIAPI
AmdPciResourcesGetRootBridgeInfo (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    OUT      PCI_ROOT_BRIDGE_OBJECT                *RootBridgeInfo
)
{
    LIST_ENTRY                            *RbNode;
    PCI_ROOT_BRIDGE_OBJECT_INSTANCE       *RootBridge;
    EFI_STATUS                            Status = EFI_NOT_FOUND;

    RbNode = GetFirstNode (RootBridgeListHead);
    while(RbNode != RootBridgeListHead){
        RootBridge = PCI_ROOT_BRIDGE_INSTANCE_FROM_LINK(RbNode);

        if(RootBridge->RootBridgeObject.Index == RootBridgeIndex) {
            RootBridgeInfo = &(RootBridge->RootBridgeObject);
            return EFI_SUCCESS;
        }
        RbNode = GetNextNode (RootBridgeListHead, RbNode);
    }
    return Status;
}

/**
  *Public function to get the number of root ports
  *
  *@param[in]      This                Pointer to AMD_PCI_RESOURCES_PROTOCOL
  *@param[in]      RootBridgeIndex     Root bridge index for which the no of root ports are returned
  *@param[in,out]  NumberOfRootPorts   Number of root ports returned
  *
**/
EFI_STATUS
EFIAPI
AmdPciResourcesGetNumberOfRootPorts (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    OUT      UINTN                                 *NumberOfRootPorts
)
{
    LIST_ENTRY                            *RbNode;
    PCI_ROOT_BRIDGE_OBJECT_INSTANCE       *RootBridge;
    EFI_STATUS                            Status = EFI_NOT_FOUND;

    *NumberOfRootPorts = 0;
    RbNode = GetFirstNode (RootBridgeListHead);

    while(RbNode != RootBridgeListHead){
        RootBridge = PCI_ROOT_BRIDGE_INSTANCE_FROM_LINK(RbNode);

        if(RootBridge->RootBridgeObject.Index == RootBridgeIndex) {
            *NumberOfRootPorts = RootBridge->NumberOfRootPorts;
            return EFI_SUCCESS;
        }
        RbNode = GetNextNode (RootBridgeListHead, RbNode);
    }
    return Status;
}

/**
  *Public function to get the root port info
  *
  *@param[in]     This                   Pointer to AMD_PCI_RESOURCES_PROTOCOL
  *@param[in]     RootBridgeIndex        Root bridge index for which the root port info will be returned
  *@param[in]     RootPortIndex          Root port index for which the root port info will be returned
  *@param[out]    RootPortInfo           Returned Root port info
  *
**/
EFI_STATUS
EFIAPI
AmdPciResourcesGetRootPortInfo (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    IN       UINTN                                 RootPortIndex,
    OUT      PCI_ROOT_PORT_OBJECT                  *RootPortInfo
)
{
    LIST_ENTRY                            *RbNode;
    LIST_ENTRY                            *RpNode;
    PCI_ROOT_BRIDGE_OBJECT_INSTANCE       *RootBridge;
    PCI_ROOT_PORT_OBJECT_INSTANCE         *RootPort;
    EFI_STATUS                            Status = EFI_NOT_FOUND;

    RbNode = GetFirstNode (RootBridgeListHead);

    while(RbNode != RootBridgeListHead){
        RootBridge = PCI_ROOT_BRIDGE_INSTANCE_FROM_LINK(RbNode);
        if(RootBridge->RootBridgeObject.Index == RootBridgeIndex) {
            RpNode = GetFirstNode(RootBridge->RootPortList);
            while(RpNode != RootBridge->RootPortList)
            {
                RootPort = PCI_ROOT_PORT_INSTANCE_FROM_LINK(RpNode);
                if(RootPort->RootPortObject.Index == RootPortIndex) {
                    RootPortInfo = &(RootPort->RootPortObject);
                    return EFI_SUCCESS;
                }
                RpNode = GetNextNode (RootBridge->RootPortList, RpNode);
            }
        }
        RbNode = GetNextNode (RootBridgeListHead, RbNode);
    }
    return Status;
}

/**
  *Public function to get the number of fixed resources
  *
  *@param[in]      This                     Pointer to AMD_PCI_RESOURCES_PROTOCOL
  *@param[in]      RootBridgeIndex          Root bridge index for which the no of fixed resources are returned
  *@param[in,out]  NumberOfFixedResources   Number of fixed resources returned
  *
**/
EFI_STATUS
EFIAPI
AmdPciResourcesGetNumberOfFixedResources (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    OUT      UINTN                                 *NumberOfFixedResources
)
{
    LIST_ENTRY                            *RbNode;
    PCI_ROOT_BRIDGE_OBJECT_INSTANCE       *RootBridge;
    EFI_STATUS                            Status = EFI_NOT_FOUND;

    *NumberOfFixedResources = 0;
    RbNode = GetFirstNode (RootBridgeListHead);

    while(RbNode != RootBridgeListHead){
        RootBridge = PCI_ROOT_BRIDGE_INSTANCE_FROM_LINK(RbNode);

        if(RootBridge->RootBridgeObject.Index == RootBridgeIndex) {
            *NumberOfFixedResources = RootBridge->NumberOfFixedResources;
            return EFI_SUCCESS;
        }
        RbNode = GetNextNode (RootBridgeListHead, RbNode);
    }
    return Status;
}

/**
  *Public function to get the fixed resource info
  *
  *@param[in]     This                   Pointer to AMD_PCI_RESOURCES_PROTOCOL
  *@param[in]     RootBridgeIndex        Root bridge index for which the fixed resource info will be returned
  *@param[in]     FixedResourceIndex     Fixed resource index for which the fixed resource info will be returned
  *@param[out]    FixedResourceInfo      Returned fixed resource info
  *
**/
EFI_STATUS
EFIAPI
AmdPciResourcesGetFixedResourceInfo (
    IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
    IN       UINTN                                 RootBridgeIndex,
    IN       UINTN                                 FixedResourceIndex,
    OUT      FIXED_RESOURCES_OBJECT                *FixedResourceInfo
)
{
    LIST_ENTRY                            *RbNode;
    LIST_ENTRY                            *FrNode;
    PCI_ROOT_BRIDGE_OBJECT_INSTANCE       *RootBridge;
    FIXED_RESOURCES_OBJECT_INSTANCE       *FixedResource;
    EFI_STATUS                            Status = EFI_NOT_FOUND;

    RbNode = GetFirstNode (RootBridgeListHead);

    while(RbNode != RootBridgeListHead){
        RootBridge = PCI_ROOT_BRIDGE_INSTANCE_FROM_LINK(RbNode);
        if(RootBridge->RootBridgeObject.Index == RootBridgeIndex) {
            FrNode = GetFirstNode(RootBridge->FixedResourcesList);
            while(FrNode != RootBridge->FixedResourcesList)
            {
                FixedResource = PCI_FIXED_RESOURCE_INSTANCE_FROM_LINK(FrNode);
                if(FixedResource->FixedResourceObject.Index == FixedResourceIndex) {
                    FixedResourceInfo = &(FixedResource->FixedResourceObject);
                    return EFI_SUCCESS;
                }
                FrNode = GetNextNode (RootBridge->FixedResourcesList, FrNode);
            }
        }
        RbNode = GetNextNode (RootBridgeListHead, RbNode);
    }
    return Status;
}