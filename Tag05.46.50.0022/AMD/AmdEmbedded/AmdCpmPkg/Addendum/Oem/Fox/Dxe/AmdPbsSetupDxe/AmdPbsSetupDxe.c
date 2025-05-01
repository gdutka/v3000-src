/*****************************************************************************
 *
 * Copyright (C) 2015-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 ******************************************************************************
 */

#include <AmdCpmDxe.h>
#include "AmdPbsConfig.h"
#include "AmdPbsSetupDxe.h"
#include <Library/AmdPbsConfigLib/AmdPbsConfigLib.h>
#include <Library/PcdLib.h>
#include <Library/AmdPspCommonLib.h>
#include <cpuRegisters.h>
#include <Protocol/ComponentName2.h>
#include <Library/DxeServicesLib.h>
#include <Library/FabricResourceManagerLib.h>
#include <Library/FchSpiAccessLib.h>
#include <Protocol/SocLogicalIdProtocol.h>
//#include "MpLib.h"
#include <Register/LocalApic.h>

UINT32
ReadLocalApicReg(
  IN UINTN  MmioOffset
  );

VOID
WriteLocalApicReg (
  IN UINTN  MmioOffset,
  IN UINT32 Value
  );

#pragma pack (push, 1)
typedef union {
  UINT64 FwVersion;
  struct {
    UINT16 Build;
    UINT8  Patch;
    UINT8  Minor : 4;
    UINT8  Major : 4;
    UINT16 AppName;
    UINT8  ExCricuit;
    UINT8  AppMinor : 4;
    UINT8  AppMajor : 4;
  }Field;
} F_HPI_FW_VERSION;

typedef union {
  struct {
    // Bit 0~7
    UINT8   iSata_Gpp11   : 1;      //  GPP11 reworked to iSATA
    UINT8   iSata_Gpp10   : 1;      //  GPP10 reworked to iSATA
    UINT8   reDp1         : 1;      //  DP1 reworked to DP CONN
    UINT8   reUsbC0_nova  : 1;      //  USBC0 reworked to NOVA CONN
    UINT8   reUsbC1_10G   : 1;      //  USBC1 reworked to NOVA CONN
    UINT8   DisUsb2Hs_Fp    : 1;    //  USB2 HS disconnected to FP
    UINT8   DisUsb3Hs_Wwan  : 1;    //  USB3 HS disconnected to WWAN
    UINT8   DisUsb6_Bt      : 1;    //  USB6 HS disconnected to BT
    // Bit 8~15
    UINT8   DisUsb7_Cam     : 1;    //  USB7 HS disconnected to camera
    UINT8   Ac25w_54w       : 1;    //  25W reworked to 54W
    UINT8   Ac54W_25W       : 1;    //  54W reworked to 25W
    UINT8   Adapter150WOnly : 1;    //  Adapter 150W only
    UINT8   Rev0            : 4;
    // Bit 16~63
    UINT8   Rev1;   //  16-23
    UINT8   Rev2;   //  24-31
    UINT8   Rev3;   //  32-39
    UINT8   Rev4;   //  40-47
    UINT8   Rev5;   //  48-55
    UINT8   Rev6;   //  56-63
    // Bit 64~71 (0x4E)
    UINT8   ApuSku          : 2;      // 00:Not initialized, 01:TV, 02:A0 APU silcon, 03: B0 APU silcon
    UINT8   PDInfo          : 2;      // 00:Not initialized, 01:PS8828A, 02:PS8830, 03:Not defined
    UINT8   Rev7            : 4;
    // 0x4F Memory type
    UINT8   Rev8            : 4;
    UINT8   MemType         : 4;      // (0x01 or 0x0F): Micron DRx32 16GB, 0x04: LPDDR5 Socket, 0x05: Micron SRx32 8GB, 0x06: Micron DRx32 32GB (x8 die)

  } Desc;
  UINT8      Raw[10];    // Up to 80 bit defined.
} T_BOARD_ID_REWORK_FIELD;
#pragma pack (pop)

#define  IS_ROM2_ADDRESS(Address) ((Address & 0xFF000000) == 0xFF000000)

CONST EFI_GUID  gZeroGuid = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

EFI_STATUS
EFIAPI
LoadAmdPbsDefault (
  OUT AMD_PBS_SETUP_OPTION      *AmdPbsConfiguration
  );

//
// This is the generated IFR binary data for each formset defined in VFR.
// This data array is ready to be used as input of PreparePackageList() to
// create a packagelist (which contains Form packages, String packages, etc).
//

extern UINT8 AmdPbsHiiBin[];

//
// This is the generated String package data for all Unicode files.
// This data array is ready to be used as input of PreparePackageList() to
// create a packagelist (which contains Form packages, String packages, etc).
//

extern UINT8 AmdPbsSetupDxeStrings[];

//
// Global Variables.
//

EFI_GUID mFormSetGuid = AMD_PBS_SETUP_FORMSET_GUID;
EFI_GUID mPbsSetupGuid = AMD_PBS_SETUP_GUID;

HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath0 = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    AMD_PBS_SETUP_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (END_DEVICE_PATH_LENGTH),
      (UINT8) ((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};


UINTN
AmdCpmGetVbiosImage (
  )
{
  EFI_STATUS                                  Status;
  UINTN                                       HandleCount;
  EFI_HANDLE                                  *HandleBuffer;
  UINTN                                       Index;
  EFI_PCI_IO_PROTOCOL                         *PciIo;
  BOOLEAN                                     Found;
  UINTN                                       Seg;
  UINTN                                       Bus;
  UINTN                                       Dev;
  UINTN                                       Fnc;
  EFI_HANDLE                                  VgaHandle;
  UINT16                                      dGpVid;
  AMD_CPM_DISPLAY_FEATURE_PROTOCOL            *CpmDisplayFeatureProtocolPtr;
  CPM_DISPLAY_FEATURE_PRIVATE                 *DisplayFeatureDataPtr;
  UINTN                                       VBiosImage;
  UINTN                                       VBiosImageSize;

  dGpVid = 0;
  Found = FALSE;
  PciIo = NULL;
  Seg = 0x00;
  Bus = 0x00;
  Dev = 0x00;
  Fnc = 0x00;
  VgaHandle = NULL;
  VBiosImage = 0;
  VBiosImageSize = 0;

  do
  {
    Status = gBS->LocateProtocol ( &gAmdCpmDisplayFeatureProtocolGuid,
                                   NULL,
                                   (VOID**)&CpmDisplayFeatureProtocolPtr );
    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    DisplayFeatureDataPtr   = &CpmDisplayFeatureProtocolPtr->DisplayFeatureData;

    //
    // A. Search Int or Ext GFX bridge and then get the SubBus information from this bridge
    //

    //  Step 01.
    //  Search handles that support the gEfiPciIoProtocolGuid protocol
    //  gBS->LocateHandleBuffer returns an array of handles.

    Status = gBS->LocateHandleBuffer ( ByProtocol,
                                       &gEfiPciIoProtocolGuid,
                                       NULL,
                                       &HandleCount,
                                       &HandleBuffer );
    if (EFI_ERROR (Status))
    {
      ASSERT_EFI_ERROR (Status);
      break;
    }

    //
    //B. Search iGPU / dGPU device under bridge
    //

    for (Index = 0; (Index < HandleCount) && (Found == FALSE); Index++)
    {
      Status = gBS->HandleProtocol ( HandleBuffer[Index],
                                     &gEfiPciIoProtocolGuid,
                                     (VOID**)&PciIo );
      if (EFI_ERROR (Status))
      {
        continue;
      }

      //
      // Step 01.
      // See if this is a PCI Graphics Controller by looking at Class Code Register
      //
      PciIo->GetLocation ( PciIo, &Seg, &Bus, &Dev, &Fnc );

      if (((UINTN)Bus == DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Bus) &&
          ((UINTN)Dev == DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Device) &&
          ((UINTN)Fnc == DisplayFeatureDataPtr->GfxDevicePfa[0].Pfa.Function))
      {

        Status = PciIo->Pci.Read ( PciIo,
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,        // Start offset: 0x00
                                   1,                           // Length of bytes
                                   &dGpVid );
        if (!EFI_ERROR (Status) && dGpVid != 0xFFFF)
        {
          VgaHandle      = HandleBuffer[Index];
          VBiosImage     = (UINTN) PciIo->RomImage;
          VBiosImageSize = (UINTN) PciIo->RomSize;
        }
        Found = TRUE;
      }
    }

  } while (FALSE);

  return VBiosImage;
}


CHAR16 *
AmdCpmGetGopDriverRev (
     OUT BOOLEAN                *Found
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  EFI_COMPONENT_NAME2_PROTOCOL  *ComponentName;
  CHAR16                        *DriverName;
  UINTN                         HandleCount;
  UINTN                         Index;

  ComponentName = NULL;
  DriverName = NULL;
  HandleCount = 0;
  *Found = FALSE;

  Status = gBS->LocateHandleBuffer ( ByProtocol,
                                     &gEfiDriverBindingProtocolGuid,
                                     NULL,
                                     &HandleCount,
                                     &HandleBuffer );
  if (!EFI_ERROR (Status)) {
    for (Index = 0; Index < HandleCount; Index++) {
      Status = gBS->OpenProtocol ( HandleBuffer[Index],
                                   &gEfiComponentName2ProtocolGuid,
                                   (VOID **) &ComponentName,
                                   gImageHandle,
                                   NULL,
                                   EFI_OPEN_PROTOCOL_GET_PROTOCOL );
      if (!EFI_ERROR (Status)) {
        Status = ComponentName->GetDriverName ( ComponentName,
                                                ComponentName->SupportedLanguages,
                                                &DriverName );
        gBS->CloseProtocol ( HandleBuffer[Index],
                             &gEfiComponentName2ProtocolGuid,
                             gImageHandle,
                             HandleBuffer[Index] );
        if (!EFI_ERROR (Status) && !StrnCmp (DriverName, L"AMD GOP ", 8)) {
          *Found = TRUE;
          break;
        }
      }
    }
  }

  if (HandleCount) {
    gBS->FreePool (HandleBuffer);
  }

  return DriverName;
}


CHAR16 *
AmdCpmGetGopDriverRevFromFv (
  IN     EFI_GUID               *GopDriverFileGuid,
  OUT   BOOLEAN                *Found
)
{
  EFI_STATUS                    Status;
  CHAR16                        *DriverName;
  CHAR16                        *CodeData;
  UINTN                         Index;
  EFI_IMAGE_DOS_HEADER          *ImageDosHdr;
  EFI_IMAGE_NT_HEADERS64        *ImageNt64Hdr;
  UINT32                        CodeSize;

  DriverName = NULL;
  *Found = FALSE;
  ImageDosHdr = NULL;
  ImageNt64Hdr = NULL;

  Status = GetSectionFromFv (GopDriverFileGuid, EFI_SECTION_PE32, 0, (VOID**)&ImageDosHdr, &Index);
  if (EFI_ERROR (Status)) {
    return DriverName;
  }

  if (NULL == ImageDosHdr)
  {
    return DriverName;
  }

  if (ImageDosHdr->e_magic != EFI_IMAGE_DOS_SIGNATURE) {
    return DriverName;
  }

  ImageNt64Hdr = (EFI_IMAGE_NT_HEADERS64 *)(((CHAR8 *)ImageDosHdr) + ImageDosHdr->e_lfanew);
  ASSERT (ImageNt64Hdr != NULL);
  if (NULL != ImageNt64Hdr)
  {
    CodeData = (CHAR16 *)(((CHAR8 *)ImageDosHdr) + ImageNt64Hdr->OptionalHeader.BaseOfCode);
    CodeSize = ImageNt64Hdr->OptionalHeader.AddressOfEntryPoint - ImageNt64Hdr->OptionalHeader.BaseOfCode;
    for (Index = 0; Index < (UINTN)CodeSize; Index += 0x10) {
      CodeData = (CHAR16 *)(((CHAR8 *)ImageDosHdr) + ImageNt64Hdr->OptionalHeader.BaseOfCode + Index);
      if (StrnCmp(L"AMD GOP ", CodeData, 8) == 0) {
        DriverName = CodeData;
        *Found = TRUE;
        break;
      }
    }
  }

  return DriverName;
}

CHAR16 *
GetDdiConnectorTypeStr (
  IN UINT8             ConnectorType
  )
{
#define STR(type, str) if (type == ConnectorType) return str

  STR(ConnectorTypeDP               , L"DP");
  STR(ConnectorTypeEDP              , L"EDP");
  STR(ConnectorTypeSingleLinkDVI    , L"SingleLinkDVI");
  STR(ConnectorTypeDualLinkDVI      , L"DualLinkDVI");
  STR(ConnectorTypeHDMI             , L"HDMI");
  STR(ConnectorTypeDpToVga          , L"DpToVga");
  STR(ConnectorTypeDpToLvds         , L"DpToLvds");
  STR(ConnectorTypeNutmegDpToVga    , L"NutmegDpToVga");
  STR(ConnectorTypeSingleLinkDviI   , L"SingleLinkDviI");
  STR(ConnectorTypeEDPToLvds        , L"EDPToLvds");
  STR(ConnectorTypeEDPToLvdsSwInit  , L"EDPToLvdsSwInit");
  STR(ConnectorTypeDpWithTypeC      , L"DP with TypeC");
  STR(ConnectorTypeDpWithoutTypeC   , L"DP without TypeC");
  STR(ConnectorTypeAutoDetect       , L"AutoDetect");
  STR(UnusedType                    , L"UnusedType");
  STR(MaxConnectorType              , L"Unknow");

  return L"Unknown";
}


VOID
GetDefaultDdiStr (
  CHAR16                              *Buffser,
  UINTN                               BufferSize,
  PCIE_CONNECTOR_TYPE                 ConnectorType
  )
{
  UnicodeSPrint (
    Buffser,
    BufferSize,
    L"Default (%s)",
    GetDdiConnectorTypeStr (ConnectorType)
    );
}



EFI_STATUS
AmdCpmUpdateDpDefaultString (
  AMD_PBS_DXE_PRIVATE_DATA         *Private
  )
{
  EFI_STATUS                          Status;
  AMD_CPM_DXIO_TOPOLOGY_TABLE         *DxioTopologyTablePtr;
  DDI_DESCRIPTOR                      *DdiPort;
  UINTN                               Index;
  CHAR16                              Buffer[100];
  STATIC AMD_CPM_TABLE_PROTOCOL       *CpmTableProtocolPtr = NULL;

  if (CpmTableProtocolPtr == NULL) {
    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    ASSERT_EFI_ERROR(Status);
  }

  if (CpmTableProtocolPtr == NULL) {
    return EFI_UNSUPPORTED;
  }

  DxioTopologyTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DXIO_TOPOLOGY);
  ASSERT (DxioTopologyTablePtr != NULL);
  if (DxioTopologyTablePtr == NULL) {
    return EFI_UNSUPPORTED;
  }

  DdiPort = &(DxioTopologyTablePtr->Ddi[0]);

  Index = 0;
  do {
    GetDefaultDdiStr (Buffer, sizeof (Buffer), DdiPort[Index].Ddi.ConnectorType);
    HiiSetString (Private->HiiHandle, STRING_TOKEN(STR_DXIO_DDI_DP0_DEFAULT_TEXT), Buffer, NULL);
    if ((DdiPort[Index++].Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) {
      break;
    }

    GetDefaultDdiStr (Buffer, sizeof (Buffer), DdiPort[Index].Ddi.ConnectorType);
    HiiSetString (Private->HiiHandle, STRING_TOKEN(STR_DXIO_DDI_DP1_DEFAULT_TEXT), Buffer, NULL);
    if ((DdiPort[Index++].Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) {
      break;
    }

    GetDefaultDdiStr (Buffer, sizeof (Buffer), DdiPort[Index].Ddi.ConnectorType);
    HiiSetString (Private->HiiHandle, STRING_TOKEN(STR_DXIO_DDI_DP2_DEFAULT_TEXT), Buffer, NULL);
    if ((DdiPort[Index++].Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) {
      break;
    }

    GetDefaultDdiStr (Buffer, sizeof (Buffer), DdiPort[Index].Ddi.ConnectorType);
    HiiSetString (Private->HiiHandle, STRING_TOKEN(STR_DXIO_DDI_DP3_DEFAULT_TEXT), Buffer, NULL);
    if ((DdiPort[Index++].Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) {
      break;
    }

    GetDefaultDdiStr (Buffer, sizeof (Buffer), DdiPort[Index].Ddi.ConnectorType);
    HiiSetString (Private->HiiHandle, STRING_TOKEN(STR_DXIO_DDI_DP4_DEFAULT_TEXT), Buffer, NULL);
    if ((DdiPort[Index++].Flags & DESCRIPTOR_TERMINATE_LIST) == DESCRIPTOR_TERMINATE_LIST) {
      break;
    }
  } while (FALSE);

  return EFI_SUCCESS;
}



//
// Reserved 1M MMIO space when iLaTraceMemoryEn
//
VOID
AllocateiLaTraceMemoryEnMmio (
  IN  EFI_HII_HANDLE                    HiiHandle,
  IN  AMD_PBS_SETUP_OPTION              *AmdPbsConfig
  )
{
  EFI_STRING_ID                         VfrTokenToUpdate;
  CHAR16                                *StringBuffer;
  EFI_STATUS                            Status;
  FABRIC_TARGET                         MmioTarget;
  FABRIC_MMIO_ATTRIBUTE                 Attributes;
  UINT64                                MmioBase, Length;

  DEBUG((DEBUG_INFO, "OEM-DXE-AllocateiLaTraceMemoryEnMmio- entry\n"));

  if (AmdPbsConfig->iLaTraceMemoryEn == 0) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AllocateiLaTraceMemoryEnMmio- iLaTraceMemoryEn disabled\n"));
    return;
  }

  StringBuffer = AllocateZeroPool (0x80);

  Length = 0x100000;
  MmioTarget.TgtType = TARGET_RB;
  MmioTarget.SocketNum = 0;
  MmioTarget.RbNum =0;
  Attributes.ReadEnable = 1;
  Attributes.WriteEnable = 1;
  Attributes.NonPosted = 0;
  Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
  MmioBase  = 0;
  Status = FabricAllocateMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);
  if (!EFI_ERROR(Status)) {
    DEBUG((DEBUG_INFO, "OEM-DXE-AllocateiLaTraceMemoryEnMmio- Success,  Reserved address: 0x%8x\n", MmioBase));
    UnicodeSPrint (StringBuffer, 0x80, L"0x%08x", MmioBase);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_ILA_TRACEMEM_EN_MMIO_ADDR_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, StringBuffer, NULL);
  }

  DEBUG((DEBUG_INFO, "OEM-DXE-AllocateiLaTraceMemoryEnMmio- exit\n"));
  gBS->FreePool (StringBuffer);
}

EFI_STATUS  CheckHwReworkMap (
    AMD_PBS_SETUP_OPTION        *AmdPbsConfig
)
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  AMD_CPM_TABLE_PROTOCOL      *CpmTableProtocolPtr;
  STATIC BOOLEAN              bHwRemapRead = FALSE;
  UINT8                       OfstU8;
  UINT8                       Value8;
  UINTN                       Index;
  STATIC T_BOARD_ID_REWORK_FIELD    HwReworkMap = {0};        // Initial for Zero.

  do
  {
    if (FALSE == bHwRemapRead)
    {
      DEBUG ((DEBUG_INFO, "[%d]: Initial HwReworkMap Table\n", __LINE__));
      Status = gBS->LocateProtocol ( &gAmdCpmTableProtocolGuid,
                                     NULL,
                                     (VOID**)&CpmTableProtocolPtr
                                   );
      if (EFI_ERROR (Status))
      {
        DEBUG (( DEBUG_INFO, "[%d]: Fail to locate gAmdCpmTableProtocolGuid protocol\n", (UINTN)__LINE__));
        ASSERT_EFI_ERROR (Status);
        break;
      }

      //
      //  Reading the Rework setting for Mayan/Lialc common field
      //
      // For the Special Rework, it will read from EC ram. it can get from T_BOARD_ID_EXPORTED_FIELD::specRework (Bit00~Bit48)
      for (Index = 0; Index < 6; ++Index)
      {
        OfstU8 = (UINT8)((0x93+3) + Index);   // "specRework" Start from ECRAMx96
        CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &HwReworkMap.Raw[Index]);
      }
      // 0x4E need to do special command access.
      // 1. ECRAMx31 = 0
      // 2. ECRAMx30 = 1
      // 3. then ECRAMx1E is board id rom offset 0x4E
      // 4. Close, ECRAMx31 = 0xFF;
      OfstU8 = 0x31, Value8 = 0;
      CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
      OfstU8 = 0x30, Value8 = 1;
      CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
      OfstU8 = 0x1E, Index = 8;
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &HwReworkMap.Raw[Index]);
      OfstU8 = 0x31, Value8 = 0xFF;
      CpmTableProtocolPtr->CommonFunction.EcRamWrite (CFG_CPM_ECRAM_BASE_ADDR, 0x81, &OfstU8, &Value8);
      // 0x4F can get from T_BOARD_ID_EXPORTED_FIELD::memType
      OfstU8 = (UINT8)(0x93+9);   // "memType" byte 9.
      Index = 9;
      CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &OfstU8, &HwReworkMap.Raw[Index]);

      bHwRemapRead = TRUE;
    }

    if ((1 == HwReworkMap.Desc.iSata_Gpp10) || (1 == HwReworkMap.Desc.iSata_Gpp11))
    {
      DEBUG ((DEBUG_INFO, "[%d]: Detected HwReworkMap iSata Enable\n", __LINE__));
      AmdPbsConfig->Ssd1PwrEn = 0;
    }

  } while (FALSE);

  return Status;
}

//
// Update all of AMD FW Version information in VFR.
//
VOID
UpdateAmdFwVersionInfo (
  IN  EFI_HII_HANDLE                    HiiHandle
)
{
  EFI_STRING_ID                         VfrTokenToUpdate;
  CHAR16                                *FwVersionValue;
  CONST UINTN                           FW_VERSION_STR_BYTE_SIZE = 0x40 * 2;
  CHAR16                                *TmpFwVersionValue;
  CHAR16                                *FwStringBuffer;
  CONST UINTN                           FW_STR_BUF_BYTE_SIZE = 0x20 * 2;
  UINT8                                 *FwData;
  UINTN                                 FwVersionStringOffset;
  UINT64                                mFwVersionRegionBase;
  UINT32                                mFwVersionRegionSize;
  UINT8                                 *FwVersionPtr;
  UINT32                                Data32;
  UINT8                                 Data8;
  UINT8                                 Index;
  BOOLEAN                               Found;
  EFI_STATUS                            Status;
  PSP_DIRECTORY_ENTRY_TYPE              EntryTypeValue;
  AMD_SOC_LOGICAL_ID_PROTOCOL           *SocLogicalId;
  SOC_LOGICAL_ID                        LogicalId;
//  UINT16                                VrmFw;

  FwVersionValue = AllocateZeroPool (FW_VERSION_STR_BYTE_SIZE);  // Due to FW Version string is wide string, avoid someone using count.
  FwStringBuffer = AllocateZeroPool (FW_STR_BUF_BYTE_SIZE);
  FwData = AllocateZeroPool (0x400);

  // AGESA Version
  FwVersionPtr = (UINT8 *)PcdGetPtr (PcdAmdPackageString);
  FwVersionStringOffset = AsciiStrSize (FwVersionPtr);
  for (Index = 0; Index < AsciiStrSize (FwVersionPtr + FwVersionStringOffset); Index++) {
    Data8 = *(FwVersionPtr + FwVersionStringOffset + Index);
    FwStringBuffer[0] = (CHAR16)Data8;
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
  }
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_AGESA_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

  // PSP BootLoader Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  Found = PSPEntryInfo (  PSP_FW_BOOT_LOADER,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x200);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x200);
    }
    FwVersionStringOffset = 0x60;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 3), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 2), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 1), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_PSP_BOOT_LOADER_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // PSP SecureOS Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  Found = PSPEntryInfo (  PSP_FW_TRUSTED_OS,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x100);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x100);
    }
    FwVersionStringOffset = 0x60;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 3), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 2), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 1), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *FwVersionPtr, 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_PSP_TRUSTED_OS_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // ABL Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  Found = PSPEntryInfo (  AGESA_BOOT_LOADER_0,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found)
  {
    // Locate the first ABL image
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x400);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x400);
    }
    FwVersionPtr = (UINT8 *) FwData + 0x100; // Point to Directory table
    if (*(UINT32*)FwVersionPtr == 0) {
      FwData += *(UINT32*)(FwVersionPtr + sizeof(UINT32));
    }
    FwVersionStringOffset = 0x60;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%02x%02x%02x%02x", \
        FwVersionPtr[3], FwVersionPtr[2], FwVersionPtr[1], FwVersionPtr[0] );
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_ABL_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

    // APCB Version
    ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
    ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
    FwVersionStringOffset = 0xA0;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 1), 2);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *FwVersionPtr, 2);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_APCB_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

    // APOB Version
    ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
    ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
    FwVersionStringOffset = 0xA4;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr+1), 2);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *FwVersionPtr, 2);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_APOB_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // UCODE Patch Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  mFwVersionRegionBase = AsmReadMsr64(MSR_PATCH_LEVEL);
  UnicodeValueToStringS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), mFwVersionRegionBase, 0);
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_UCODE_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

  // SMU FW Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  FwVersionStringOffset = 0x60;
  EntryTypeValue.Value = 0;
  EntryTypeValue.Field.Type = SMU_OFFCHIP_FW;
  Status = gBS->LocateProtocol (&gAmdSocLogicalIdProtocolGuid, NULL, (VOID **) &SocLogicalId);
  if (!EFI_ERROR (Status))
  {
    SocLogicalId->GetLogicalIdOnCurrentCore (SocLogicalId, &LogicalId);
    if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Ax) != 0)) {
      // A0
      EntryTypeValue.Field.SubProgram = 0;
    }
    if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
      // B0
      EntryTypeValue.Field.SubProgram = 1;
    }
  }
  Found = PSPEntryInfo (  EntryTypeValue.Value,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *) mFwVersionRegionBase, 0x100);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x100);
    }
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%02d.%02d.%02d.%02d", \
        FwVersionPtr[3], FwVersionPtr[2], FwVersionPtr[1], FwVersionPtr[0] );
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_SMU_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // ZSC FW Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  FwVersionStringOffset = 0x60;
  EntryTypeValue.Value = 0;
  EntryTypeValue.Field.Type = MSMU_BINARY_0;
  Status = gBS->LocateProtocol (&gAmdSocLogicalIdProtocolGuid, NULL, (VOID **) &SocLogicalId);
  if (!EFI_ERROR (Status))
  {
    SocLogicalId->GetLogicalIdOnCurrentCore (SocLogicalId, &LogicalId);
    if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Ax) != 0)) {
      // A0
      EntryTypeValue.Field.SubProgram = 0;
    }
    if ((LogicalId.Family == AMD_FAMILY_RMB) && ((LogicalId.Revision & AMD_REV_F19_RMB_Bx) != 0)) {
      // B0
      EntryTypeValue.Field.SubProgram = 1;
    }
  }
  Found = PSPEntryInfo (  EntryTypeValue.Value,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *) mFwVersionRegionBase, 0x100);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x100);
    }
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);

    UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%02d.%02d.%02d.%02d", \
        FwVersionPtr[3], FwVersionPtr[2], FwVersionPtr[1], FwVersionPtr[0] );
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_ZSC_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // DXIO FW Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  mFwVersionRegionSize = PcdGet32 (PcdDxioMajorRevision);
  UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), mFwVersionRegionSize, 4);
  StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
  StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
  mFwVersionRegionSize = PcdGet32 (PcdDxioMinorRevision);
  UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), mFwVersionRegionSize, 4);
  StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_DXIO_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

  // MP2 FW Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  FwVersionStringOffset = 0x60;
  Found = PSPEntryInfo (  0x25,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x100);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x100);
    }
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%02x.%02x.%02x.%02x", \
        FwVersionPtr[3], FwVersionPtr[2], FwVersionPtr[1], FwVersionPtr[0] );
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_MP2_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // KVM Engine Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  FwVersionStringOffset = 0x60;
  Found = PSPEntryInfo (  KVM_ENGINE_BINARY,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x100);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x100);
    }
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%d.%d.%d",
      (*(FwVersionPtr + 0)),
      (*(FwVersionPtr + 1)),
      (*(FwVersionPtr + 2)) + ((*(FwVersionPtr + 3))*0x100));
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_KVM_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // XHCI FW Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  FwVersionStringOffset = 0x60;

  if (LogicalId.Family == AMD_FAMILY_RMB) {
    if (((LogicalId.Revision & AMD_REV_F19_RMB_Ax) != 0) || ((LogicalId.Revision & AMD_REV_F19_RMB_B0) != 0)) {
      Found = PSPEntryInfo (  0x44,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
    }
    if ((LogicalId.Revision & AMD_REV_F19_RMB_B1) != 0) {
      Found = PSPEntryInfo (  0x144,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
    }
  }

  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x100);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x100);
    }
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%02x.%02x.%02x.%02x", \
        FwVersionPtr[3], FwVersionPtr[2], FwVersionPtr[1], FwVersionPtr[0] );
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_XHCI_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // VBIOS FW Version
  mFwVersionRegionBase = (UINT64)AmdCpmGetVbiosImage();
  if (mFwVersionRegionBase != 0) {
    if (*(UINT16 *)mFwVersionRegionBase == 0xAA55) {
      ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
      ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
      FwVersionStringOffset = 0x70;
      mFwVersionRegionSize = 0;
      for (Index = 0; *(UINT8 *)((mFwVersionRegionBase + FwVersionStringOffset) + Index) != 0x00; Index++) {
        Data8 = *(UINT8 *)((mFwVersionRegionBase + FwVersionStringOffset) + Index);
        if (((Data8 >= 0x30) && (Data8 <= 0x39)) || ((Data8 >= 0x41) && (Data8 <= 0x5A)) || (Data8 == 0x2D)) {
          FwStringBuffer[0] = (CHAR16)Data8;
          StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
        }
      }
      VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_VBIOS_VALUE);
      HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
    }
  }

  // GOP Driver Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  if ((EFI_GUID *)PcdGetPtr(PcdGopDriverFile) == NULL || CompareGuid ((EFI_GUID *)PcdGetPtr(PcdGopDriverFile), &gZeroGuid)) {
    TmpFwVersionValue = AmdCpmGetGopDriverRev(&Found);
  } else {
    TmpFwVersionValue = AmdCpmGetGopDriverRevFromFv((EFI_GUID *)PcdGetPtr(PcdGopDriverFile), &Found);
  }
  if (Found && (TmpFwVersionValue != NULL)) {
    StrCpyS(FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, TmpFwVersionValue);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_GOP_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  // EC FW Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  mFwVersionRegionSize = 0;
  Data32 = PcdGet32 (PcdECFwVersion);
  for (Index = 0; Index < 4; Index++) {
    Data8 = (UINT8)(Data32 >> ( 8 * ( 3 - Index )));
    if ( Data8 >= 0x30 ) {
      mFwVersionRegionSize |= (UINT32)( Data8 - 0x30 ) << ( 4 * (3 - Index));
    }
  }
  UnicodeValueToStringS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), mFwVersionRegionSize, 4);
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_EC_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

  // PD Info
  F_HPI_FW_VERSION PdFirmwareVer;
  PdFirmwareVer.FwVersion = PcdGet64 (PcdPdFwVersion);
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%04x.%02x.%x.%x", \
      PdFirmwareVer.Field.Build, PdFirmwareVer.Field.Patch, PdFirmwareVer.Field.Minor, PdFirmwareVer.Field.Major);
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_PD_VERSION_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

  // VRM Info
/*  VrmFw = PcdGet16 (PcdVrmFwVersion);
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%04x", VrmFw);
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_VRM_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
 */ 
  // Application Frimware Version
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  UnicodeSPrint (FwVersionValue, FW_VERSION_STR_BYTE_SIZE, L"%02x.%02x.%x.%x", \
      PdFirmwareVer.Field.AppName, PdFirmwareVer.Field.ExCricuit, PdFirmwareVer.Field.AppMinor, PdFirmwareVer.Field.AppMajor);
  VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_PDA_VERSION_VALUE);
  HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);

  //AIM-T FW Version (0x85)
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  Found = PSPEntryInfo (  MPM_FW_1,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x200);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x200);
    }
    FwVersionStringOffset = 0x60;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 3), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 2), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 1), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_AIMT_FW_PART1_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  //AIM-T FW Version (0x86)
  ZeroMem (FwVersionValue, FW_VERSION_STR_BYTE_SIZE);
  ZeroMem (FwStringBuffer, FW_STR_BUF_BYTE_SIZE);
  Found = PSPEntryInfo (  MPM_FW_2,
                          &mFwVersionRegionBase,
                          &mFwVersionRegionSize );
  if (Found) {
    if(IS_ROM2_ADDRESS(mFwVersionRegionBase)) {
      CopyMem (FwData, (UINT8 *)mFwVersionRegionBase, 0x200);
    } else {
      FchSpiRomRead((UINT32)mFwVersionRegionBase, FwData, 0x200);
    }
    FwVersionStringOffset = 0x60;
    FwVersionPtr = (UINT8 *)(FwData + FwVersionStringOffset);
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 3), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 2), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr + 1), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, L".");
    UnicodeValueToStringS (FwStringBuffer, FW_STR_BUF_BYTE_SIZE, (PREFIX_ZERO + RADIX_HEX), *(FwVersionPtr), 0);
    StrCatS (FwVersionValue, FW_VERSION_STR_BYTE_SIZE / 2, FwStringBuffer);
    VfrTokenToUpdate = STRING_TOKEN (STR_AMD_PBS_FWVER_AIMT_FW_PART2_VALUE);
    HiiSetString (HiiHandle, VfrTokenToUpdate, FwVersionValue, NULL);
  }

  gBS->FreePool (FwVersionValue);
  gBS->FreePool (FwStringBuffer);
  return;
}
EFI_STATUS
AmdPbsBrowserActionChangingByQuestionId (
  EFI_QUESTION_ID       QuestionId,
  AMD_PBS_SETUP_OPTION  *AmdPbsSetupConfig
)
{
  EFI_STATUS           Status = EFI_SUCCESS;

  switch (QuestionId){
    case KEY_XGBE_AIC2_TYPE_SEL:
      if(AmdPbsSetupConfig->Aic2TypeSel == 0){
        AmdPbsSetupConfig->Aic3TypeSel = 1;
      }
      else{
        AmdPbsSetupConfig->Aic3TypeSel = 0;
      }
    break;
    case KEY_XGBE_AIC3_TYPE_SEL:
      if(AmdPbsSetupConfig->Aic3TypeSel == 0){
        AmdPbsSetupConfig->Aic2TypeSel = 1;
      }
      else{
        AmdPbsSetupConfig->Aic2TypeSel = 0;
      }
    break;

    default:
     break;
  }


  return Status;

}


EFI_STATUS
AmdPbsBrowserActionChangedByQuestionId (
  EFI_QUESTION_ID       QuestionId,
  AMD_PBS_SETUP_OPTION  *AmdPbsSetupConfig
)
{
  EFI_STATUS           Status = EFI_SUCCESS;

  switch (QuestionId) {
  #ifdef MODERN_STANDBY_SUPPORT
    case KEY_REDUCE_HARDWARE_SETTING:
      if (AmdPbsSetupConfig->ReduceHardwareEn == 1)
      {
        if (AmdPbsSetupConfig->WakeOnPME != 0)
        {
          AmdPbsSetupConfig->WakeOnPME = 0;
        }
      }
      else if (AmdPbsSetupConfig->ReduceHardwareEn == 0)
      {
        if (AmdPbsSetupConfig->WakeOnPME != 1)
        {
          AmdPbsSetupConfig->WakeOnPME = 1;
        }
      }
      break;
  #endif

    case KEY_SPECIAL_VGA_FEATURE_SETTING:
      if (AmdPbsSetupConfig->SpecialVgaFeature != 0)
      {
        AmdPbsSetupConfig->D3ColdSupport = 1;
        AmdPbsSetupConfig->DgpuDsmFunASupport = 1;
        AmdPbsSetupConfig->DgpuDsmFunBSupport = 1;
      }
      break;

    case KEY_D3_COLD_SUPPORT_SETTING:
      if (AmdPbsSetupConfig->D3ColdSupport == 1)
      {
        AmdPbsSetupConfig->DgpuDsmFunASupport = 1;
        AmdPbsSetupConfig->DgpuDsmFunBSupport = 1;
      } else if (AmdPbsSetupConfig->D3ColdSupport == 0) {
        AmdPbsSetupConfig->DgpuDsmFunASupport = 0;
        AmdPbsSetupConfig->DgpuDsmFunBSupport = 0;
      }
      break;

    case KEY_DGPU_SSID_SETTING:
      if (AmdPbsSetupConfig->DgpuSsidSvid == 0) {
        AmdPbsSetupConfig->AmdPcdCpmDgpuSsid = 0;
        AmdPbsSetupConfig->AmdPcdCpmDgpuAudioSsid = 0;
      }
      break;


    case KEY_DGPU_MACO_SETTING:
    if (AmdPbsSetupConfig->DgpuMaco == 1)
    {
      AmdPbsSetupConfig->AmdBlinkLed = 2;
    }
    break;

    case KEY_WOV_SETTING:
//    if (AmdPbsSetupConfig->WoV == 1)
//    {
//      AmdPbsSetupConfig->AcpPowerGating = 0;
//    } else if (AmdPbsSetupConfig->WoV == 0)
//    {
//      AmdPbsSetupConfig->AcpPowerGating = 1;
//    }
      break;

    case KEY_TOUCH_PANEL_SETTING:
    case KEY_TOUCH_PAD_SETTING:
      if (((AmdPbsSetupConfig->TouchPanel & 0xF) == 2) || (AmdPbsSetupConfig->TouchPad == 2))
      {
//        AmdPbsSetupConfig.EvalCardTdiodeRoutingSelect = 0;
      }
      break;

    case KEY_NFC_SENSOR_SETTING:
      if (AmdPbsSetupConfig->NfcSensor == 2 )
      {
//        AmdPbsSetupConfig.EvalCardTdiodeRoutingSelect = 0;
      }
      break;

    case KEY_AMD_LIGHT_SENSOR_SETTING:
      if (AmdPbsSetupConfig->amdLightSensor == 2)
      {
//        AmdPbsSetupConfig.EvalCardTdiodeRoutingSelect = 0;
      }
      break;

    case KEY_SATA_SSD_CONFIG0_SETTING:
      break;

    case KEY_DT_SLOT_SETTING:
      if (AmdPbsSetupConfig->DtSlotPowerControl == 1) {
        AmdPbsSetupConfig->Ssd1PwrEn = 0;
      }
      break;
    case KEY_SATA_SSD_CONFIG1_SETTING:
      if (AmdPbsSetupConfig->Ssd1PwrEn == 1) {
        AmdPbsSetupConfig->DtSlotPowerControl = 0;
#ifndef LPDDR5
        AmdPbsSetupConfig->OddPwrEn = 0;
        AmdPbsSetupConfig->HDDPwrEn = 0;
        AmdPbsSetupConfig->ZeroPowerOddEn = 0;
#endif
      }
      break;
    case KEY_WWAN_SELECT_SETTING:
      if (AmdPbsSetupConfig->WwanPowerEn == 1) {
        AmdPbsSetupConfig->Pciex1PowerEn = 0;
      }
      break;
    case KEY_PCIEX1_SELECT_SETTING:
      if (AmdPbsSetupConfig->Pciex1PowerEn == 1) {
        AmdPbsSetupConfig->WwanPowerEn = 0;
      }
      break;
    case KEY_ODD_SELECT_SETTING:
    case KEY_HDD_SELECT_SETTING:
      if ((AmdPbsSetupConfig->OddPwrEn == 1) || (AmdPbsSetupConfig->HDDPwrEn == 1)){     // SSD1 must be disabled if Odd or HDD enabled
        AmdPbsSetupConfig->Ssd1PwrEn = 0;
      }
      if (AmdPbsSetupConfig->OddPwrEn == 0) {
        AmdPbsSetupConfig->ZeroPowerOddEn = 0;
      }
      break;

    case KEY_PWR_SENSORS_ROUTING_SELECT:
      if (0 == AmdPbsSetupConfig->PowerSensorsRoutingSelect)
        AmdPbsSetupConfig->Mp2FwSelection = 0;
      else
        AmdPbsSetupConfig->Mp2FwSelection = 1;
    break;

    case KEY_MP2_FW_SELECT:
      if (0 == AmdPbsSetupConfig->Mp2FwSelection)
      {
        AmdPbsSetupConfig->PowerSensorsRoutingSelect = 0;
      }
    break;

    case KEY_DP1_CAPS_ADJUST_SETTING:
    case KEY_DP2_CAPS_ADJUST_SETTING:
      if (AmdPbsSetupConfig->AdjustDdi1 == 1 )
      {
        AmdPbsSetupConfig->DP1DisplayCaps = PcdGet32 (PcdDisplayCapDdi1);
      }
      else
      {
        AmdPbsSetupConfig->DP1DisplayCaps = 0xFFFFFFFF;
      }

      if (AmdPbsSetupConfig->AdjustDdi2 == 1 )
      {
        AmdPbsSetupConfig->DP2DisplayCaps = PcdGet32 (PcdDisplayCapDdi2);
      }
      else
      {
        AmdPbsSetupConfig->DP2DisplayCaps = 0xFFFFFFFF;
      }
      break;
    case KEY_DP1_CAPS_NUM_SETTING:
    case KEY_DP2_CAPS_NUM_SETTING:
      if (AmdPbsSetupConfig->AdjustDdi1 == 1 && AmdPbsSetupConfig->DP1DisplayCaps == 0xFFFFFFFF)
      {
        AmdPbsSetupConfig->AdjustDdi1 = 0;
      }
      if (AmdPbsSetupConfig->AdjustDdi2 == 1 && AmdPbsSetupConfig->DP2DisplayCaps == 0xFFFFFFFF)
      {
        AmdPbsSetupConfig->AdjustDdi2 = 0;
      }

    case KEY_XGBE_PATH_SELECT:
      if(0 == AmdPbsSetupConfig->XgbePathSelect)
        AmdPbsSetupConfig->PxeBoot = 0;
    break;

    case KEY_XGBE_PORT0_TYPE_SWITCH:
      if(0  == AmdPbsSetupConfig->Port0TypeSwitch){
         AmdPbsSetupConfig->Port0ModeSel = 6;
      }
      else{
        AmdPbsSetupConfig->Port0ModeSel = 8;
         AmdPbsSetupConfig->Port0SpeedSel = 20;
      }
    break;

    case KEY_XGBE_PORT1_TYPE_SWITCH:
      if(0 == AmdPbsSetupConfig->Port1TypeSwitch){
         AmdPbsSetupConfig->Port1ModeSel = 6;
      }
      else{
        AmdPbsSetupConfig->Port1ModeSel = 8;
         AmdPbsSetupConfig->Port1SpeedSel = 20;
      }
    break;
    case KEY_XGBE_AIC_TYPE_SWITCH:
      if(0 == AmdPbsSetupConfig->AicTypeSwitch){
         AmdPbsSetupConfig->AICModeSel = 8;
         AmdPbsSetupConfig->Aic2TypeSel = 0;
         AmdPbsSetupConfig->Aic3TypeSel = 0;
          AmdPbsSetupConfig->AICSpeedSel = 20;
      }
      else{
        if( (AmdPbsSetupConfig->Aic2TypeSel == 0) || (AmdPbsSetupConfig->Aic3TypeSel == 0)){
            AmdPbsSetupConfig->AICModeSel = 6;
        }
        else
           AmdPbsSetupConfig->AICModeSel =3;
        }

    break;

    case KEY_XGBE_AIC2_TYPE_SEL:
        if(AmdPbsSetupConfig->Aic2TypeSel == 0){
          AmdPbsSetupConfig->AICModeSel = 6;
          AmdPbsSetupConfig->Aic3TypeSel = 0;
          AmdPbsSetupConfig->AICSpeedSel = 20;
        }
        else{
          AmdPbsSetupConfig->AICModeSel = 3;
          AmdPbsSetupConfig->Aic3TypeSel = 1;
          AmdPbsSetupConfig->AICSpeedSel = 4;
         }
    break;
    case KEY_XGBE_AIC3_TYPE_SEL:
       if(AmdPbsSetupConfig->Aic3TypeSel == 0){
          AmdPbsSetupConfig->AICModeSel = 6;
          AmdPbsSetupConfig->Aic2TypeSel = 0;
          AmdPbsSetupConfig->AICSpeedSel = 20;

       }
       else{
          AmdPbsSetupConfig->AICModeSel = 3;
          AmdPbsSetupConfig->Aic2TypeSel = 1;
         AmdPbsSetupConfig->AICSpeedSel = 4;
       }
    break;

    case KEY_XGBE_PORT0_MODE_SEL:
      if( AmdPbsSetupConfig->Port0ModeSel == 1 || AmdPbsSetupConfig->Port0ModeSel == 8)
        AmdPbsSetupConfig->Port0SpeedSel = 20;
      else if( AmdPbsSetupConfig->Port0ModeSel == 2)
        AmdPbsSetupConfig->Port0SpeedSel = 8;
      else if( AmdPbsSetupConfig->Port0ModeSel == 9)
         AmdPbsSetupConfig->Port0SpeedSel = 16;
    break;

    case KEY_XGBE_PORT1_MODE_SEL:
         if( AmdPbsSetupConfig->Port1ModeSel == 1 || AmdPbsSetupConfig->Port1ModeSel == 8)
           AmdPbsSetupConfig->Port1SpeedSel = 20;
         else if( AmdPbsSetupConfig->Port1ModeSel == 2)
           AmdPbsSetupConfig->Port1SpeedSel = 8;
         else if( AmdPbsSetupConfig->Port1ModeSel == 9)
           AmdPbsSetupConfig->Port1SpeedSel = 16;
    break;

    case KEY_XGBE_AIC_MODE_SEL:
      if( AmdPbsSetupConfig->AICModeSel == 3)
        AmdPbsSetupConfig->AICSpeedSel = 4;
    break;

    default:
      break;
  }

  return Status;
}


EFI_STATUS
EFIAPI
AmdPbsExtractConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN CONST EFI_STRING Request,
  OUT EFI_STRING *Progress,
  OUT EFI_STRING *Results
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  AMD_PBS_DXE_PRIVATE_DATA         *Private;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;
  EFI_STRING                       ConfigRequestHdr;
  EFI_STRING                       ConfigRequest;
  BOOLEAN                          AllocatedRequest;
  UINTN                            Size;
  CHAR16                           *StrPointer;

  if (This == NULL || Progress == NULL || Results == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize the local variables.
  //
  ConfigRequestHdr = NULL;
  ConfigRequest    = NULL;
  Size             = 0;
  *Progress = Request;
  AllocatedRequest = FALSE;

  Private = AMD_PBS_DXE_PRIVATE_FROM_THIS (This);
  HiiConfigRouting = Private->HiiConfigRouting;

  //
  // Get Buffer Storage data from EFI variable.
  // Try to get the current setting from variable.
  //
  BufferSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = gRT->GetVariable (
            AMD_PBS_SETUP_VARIABLE_NAME,
            &gAmdPbsSystemConfigurationGuid,
            NULL,
            &BufferSize,
            Private->IfrData
            );
  if (EFI_ERROR (Status)) {
    // set all default value to &Private->IfrData
    DEBUG (( DEBUG_INFO, "LoadAmdPbsDefault in AmdPbsExtractConfig\n"));
    LoadAmdPbsDefault ((AMD_PBS_SETUP_OPTION*) &Private->IfrData);
  }

  if (Request == NULL) {
    //
    // Request is set to NULL, construct full request string.
    // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
    // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
    //
    ConfigRequestHdr = HiiConstructConfigHdr (&gAmdPbsSystemConfigurationGuid, AMD_PBS_SETUP_VARIABLE_NAME, Private->DriverHandle);
    Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
    ConfigRequest = AllocateZeroPool (Size);
    if (ConfigRequest == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    AllocatedRequest = TRUE;
    UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", ConfigRequestHdr, (UINT64)BufferSize);
    FreePool (ConfigRequestHdr);
    ConfigRequestHdr = NULL;
  } else {
    //
    // Check routing data in <ConfigHdr>.
    // Note: if only one Storage is used, then this checking could be skipped.
    //
    if (!HiiIsConfigHdrMatch (Request, &mPbsSetupGuid, AMD_PBS_SETUP_VARIABLE_NAME)) {
      return EFI_NOT_FOUND;
    }
    //
    // Set Request to the unified request string.
    //
    ConfigRequest = Request;
    //
    // Check whether Request includes Request Element.
    //
    if (StrStr (Request, L"OFFSET") == NULL) {
      //
      // Check Request Element does exist in Reques String
      //
      StrPointer = StrStr (Request, L"PATH");
      if (StrPointer == NULL) {
        return EFI_INVALID_PARAMETER;
      }
      if (StrStr (StrPointer, L"&") == NULL) {
        Size = (StrLen (Request) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest = AllocateZeroPool (Size);
        ASSERT (ConfigRequest != NULL);
        AllocatedRequest = TRUE;
        UnicodeSPrint (ConfigRequest, Size, L"%s&OFFSET=0&WIDTH=%016LX", Request, (UINT64) BufferSize);
      }
    }
  }

  Status = HiiConfigRouting->BlockToConfig (
                                HiiConfigRouting,
                                ConfigRequest,
                                Private->IfrData,
                                BufferSize,
                                Results,
                                Progress
                                );

  //
  // Free the allocated config request string.
  //
  if (AllocatedRequest) {
    FreePool (ConfigRequest);
    ConfigRequest = NULL;
  }

  //
  // Set Progress string to the original request string.
  //
  if (Request == NULL) {
    *Progress = NULL;
  } else if (StrStr (Request, L"OFFSET") == NULL) {
    *Progress = Request + StrLen (Request);
  }

  return Status;
} // AmdPbsExtractConfig


EFI_STATUS
EFIAPI
AmdPbsRouteConfig (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN CONST EFI_STRING Configuration,
  OUT EFI_STRING *Progress
  )
{
  EFI_STATUS                       Status;
  UINTN                            BufferSize;
  AMD_PBS_DXE_PRIVATE_DATA         *Private;
  EFI_HII_CONFIG_ROUTING_PROTOCOL  *HiiConfigRouting;

  if (Configuration == NULL || Progress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Private = AMD_PBS_DXE_PRIVATE_FROM_THIS (This);
  HiiConfigRouting = Private->HiiConfigRouting;
  *Progress = Configuration;

  //
  // Check routing data in <ConfigHdr>.
  // Note: if only one Storage is used, then this checking could be skipped.
  //
  if (!HiiIsConfigHdrMatch (Configuration, &gAmdPbsSystemConfigurationGuid, AMD_PBS_SETUP_VARIABLE_NAME)) {
    return EFI_NOT_FOUND;
  }

  //
  // Convert <ConfigResp> to buffer data by helper function ConfigToBlock()
  //
  BufferSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = HiiConfigRouting->ConfigToBlock (
                               HiiConfigRouting,
                               Configuration,
                               Private->IfrData,
                               &BufferSize,
                               Progress
                               );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Store Buffer Storage back to EFI variable
  //
  BufferSize = sizeof (AMD_PBS_SETUP_OPTION);
  Status = gRT->SetVariable(
                  AMD_PBS_SETUP_VARIABLE_NAME,
                  &gAmdPbsSystemConfigurationGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  BufferSize,
                  Private->IfrData
                  );
  return Status;
}



EFI_STATUS
EFIAPI
AmdPbsCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN  EFI_BROWSER_ACTION                    Action,
  IN  EFI_QUESTION_ID                       QuestionId,
  IN  UINT8                                 Type,
  IN  EFI_IFR_TYPE_VALUE                    *Value,
  OUT EFI_BROWSER_ACTION_REQUEST            *ActionRequest
  )
{
  EFI_STATUS                          Status;
  AMD_PBS_DXE_PRIVATE_DATA            *Private;
  UINTN                               BufferSize;
  STATIC BOOLEAN                      AmdPbsCallbackInvoked = FALSE;

  Status = EFI_UNSUPPORTED;

  do
  {
    Private = AMD_PBS_DXE_PRIVATE_FROM_THIS (This);

    //
    // Retrive data from Browser
    //
    BufferSize = sizeof (AMD_PBS_SETUP_OPTION);
    if (!HiiGetBrowserData(&gAmdPbsSystemConfigurationGuid,
                           (CHAR16 *) AMD_PBS_SETUP_VARIABLE_NAME,
                           BufferSize,
                           (UINT8*)Private->IfrData))
    {
      Status = EFI_NOT_FOUND;
      break;
    }

    if ((Action == EFI_BROWSER_ACTION_FORM_OPEN) || (Action == EFI_BROWSER_ACTION_FORM_CLOSE))
    {
      if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
        AmdCpmUpdateDpDefaultString (Private);
      }
      Status = EFI_SUCCESS;
      break;
    }

    if ((Value == NULL) || (ActionRequest == NULL))
    {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    if ((Type == EFI_IFR_TYPE_STRING) && (Value->string == 0))
    {
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    switch (Action)
    {
      case EFI_BROWSER_ACTION_RETRIEVE:
        if (!AmdPbsCallbackInvoked)
        {
          UpdateAmdFwVersionInfo (Private->HiiHandle);
          AllocateiLaTraceMemoryEnMmio (Private->HiiHandle, (AMD_PBS_SETUP_OPTION *)Private->IfrData);
          Status = EFI_SUCCESS;
        }
        AmdPbsCallbackInvoked = TRUE;
        break;

      case EFI_BROWSER_ACTION_DEFAULT_STANDARD:
        switch (QuestionId) {
          default:
            Status = EFI_UNSUPPORTED;
            break;
        }
        break;

      case EFI_BROWSER_ACTION_CHANGING:
        AmdPbsBrowserActionChangingByQuestionId(QuestionId, (AMD_PBS_SETUP_OPTION *)Private->IfrData);
        Status = EFI_SUCCESS;
        break;

      case EFI_BROWSER_ACTION_CHANGED:
        Status = AmdPbsBrowserActionChangedByQuestionId (QuestionId, (AMD_PBS_SETUP_OPTION *)Private->IfrData);
        break;

      default:
        break;
    }

  } while (FALSE);

  if (!EFI_ERROR (Status))
  {
    //
    // Sync the platform call back changed.
    //
    Status = HiiSetBrowserData(&gAmdPbsSystemConfigurationGuid,
                      (CHAR16 *) AMD_PBS_SETUP_VARIABLE_NAME,
                      BufferSize,
                      (UINT8 *)Private->IfrData,
                      NULL);
  }

  return Status;
}

#define ACPIMMIO32(x) (*(volatile UINT32*)(UINTN)(x))
#define ACPIMMIO16(x) (*(volatile UINT16*)(UINTN)(x))
#define ACPIMMIO8(x)  (*(volatile UINT8*)(UINTN)(x))

EFI_STATUS EFIAPI ConfigRts5261 (
    EFI_EVENT               Event,
    VOID                    *Context
)
{
  EFI_STATUS      Status = EFI_SUCCESS;
  UINT32          u32PcieMmioAddr;
  UINT16          u16VID = 0, u16DID = 0;
  UINT32          u32TempVal;
  UINTN           unIdx;
  UINT8           *u8ptr = NULL;

  do
  {
    DEBUG (( DEBUG_INFO, "[%d]: Enter Rts5261 Config\n", (UINTN)__LINE__));

    // Bus/Dev/Func, The Root
    u32PcieMmioAddr = (UINT32)PcdGet64 (PcdPciExpressBaseAddress) + (0x0 << 20) + (0x2 << 15) + (0x3 << 12) + 0x00;
    DEBUG ((DEBUG_INFO, "[%d]: u32PcieMmioAddr - 0x%x\n", __LINE__, u32PcieMmioAddr));
    // Check AMD VID/DID
    u16VID = ACPIMMIO16 (u32PcieMmioAddr);
    u16DID = ACPIMMIO16 (u32PcieMmioAddr + 0x02);
    DEBUG (( DEBUG_INFO, "  VID [%x], DID[%x]\n", u16VID, u16DID));
    // GPP 0/2/5 + 0x19. Get the Sub Bus number
    u32TempVal = (UINT32)ACPIMMIO8 (u32PcieMmioAddr + 0x19);
    DEBUG ((DEBUG_INFO, "  Sub Bus Num [%x]\n", u32TempVal));
    // RTS Dev/Func (Bus/0/0)
    u32PcieMmioAddr = (UINT32)PcdGet64 (PcdPciExpressBaseAddress) + (u32TempVal << 20) + (0x00 << 15) + (0x00 << 12) + 0x00;
    u16VID = ACPIMMIO16 (u32PcieMmioAddr);
    u16DID = ACPIMMIO16 (u32PcieMmioAddr + 0x02);
    DEBUG (( DEBUG_INFO, "  VID [%x], DID[%x]\n", u16VID, u16DID));

    if ((0x10EC != u16VID) || (0x5261!=u16DID) )
    {
      Status = EFI_SUCCESS;
      DEBUG (( DEBUG_INFO, "  Not RTS5261 Device, Skipped\n"));
      break;
    }

    // Unlock, Offset[0x817] = 0x80
    unIdx = 0;
    u32TempVal = 0x80000000;
    do
    {
      if (5 == unIdx)
      {
        Status = EFI_DEVICE_ERROR;
        DEBUG (( DEBUG_INFO, "  [%d]: Error, broken set offset[0x817]\n", __LINE__));
        break;
      }
      ACPIMMIO32 (u32PcieMmioAddr + 0x814) = u32TempVal;
      ++unIdx;
      gBS->Stall (100); // Delay 100 us, follow spec
    } while ( (ACPIMMIO32 (u32PcieMmioAddr + 0x814) & u32TempVal) != u32TempVal );
    if (EFI_ERROR (Status))
    {
      break;
    }

    // Config 0x724~0x727
    u32TempVal = 0;
    u8ptr = (UINT8*)&u32TempVal;
    u8ptr [0] = 0x00;     // 0x724
    u8ptr [1] = 0x00;     // 0x725
    u8ptr [2] = 0x00;     // 0x726
    u8ptr [3] = 0x00;     // 0x727

    unIdx = 0;
    do
    {
      if (5 == unIdx)
      {
        Status = EFI_DEVICE_ERROR;
        DEBUG (( DEBUG_INFO, "  [%d]: Error, broken set offset[0x724~727]\n", __LINE__));
        break;
      }
      ACPIMMIO32 (u32PcieMmioAddr + 0x724) = u32TempVal;
      ++unIdx;
      gBS->Stall (100); // Delay 100 us, follow spec
    } while ( (ACPIMMIO32 (u32PcieMmioAddr + 0x724) & u32TempVal) != u32TempVal );
    if (EFI_ERROR (Status))
    {
      break;
    }

    // Valid Offset[0x816],bit[0] = 1
    u32TempVal = 0;
    u8ptr = (UINT8*)&u32TempVal;
    u8ptr [0] = 0x28;     // 0x814, Bit3: RT D3, Bit5: LED blinking
    u8ptr [1] = 0x00;     // 0x815
    u8ptr [2] = 0x01;     // 0x816
    u8ptr [3] = 0x00;     // 0x817
    unIdx = 0;
    do
    {
      if (5 == unIdx)
      {
        Status = EFI_DEVICE_ERROR;
        DEBUG (( DEBUG_INFO, "  [%d]: Error, broken set offset[0x815,0x816]\n", __LINE__));
        break;
      }
      ACPIMMIO32 (u32PcieMmioAddr + 0x814) = u32TempVal;
      ++unIdx;
      gBS->Stall (100); // Delay 100 us, follow spec
    } while ( (ACPIMMIO32 (u32PcieMmioAddr + 0x814) & u32TempVal) != u32TempVal );
    if (EFI_ERROR (Status))
    {
      break;
    }

  } while (FALSE);

  if (!EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "  [%d]: Finished Config Realtek 5261 ...\n", __LINE__));
  }

  return Status;

}

EFI_STATUS  UpdateAmdPbsSetupData ()
{
  EFI_STATUS                      Status;
  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  UINT8                           data8;
  UINT8                           index;
  LOCAL_APIC_SVR                  Svr;
  BOOLEAN                         NeedUpdatePBS;
  UINT8                           PlatformSel;

  NeedUpdatePBS = FALSE;
  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status))
  {
    NeedUpdatePBS = TRUE;
    DEBUG((DEBUG_INFO, "UpdateAmdPbsSetupData -  Error:  Failed to get AMD PBS variables\n"));
    LoadAmdPbsDefault (&AmdPbsConfiguration);
    Status = EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "UpdateAmdPbsSetupData [%d]: AmdPbsConfiguration.Usb4NonPrefetchMemoryReserved =%d\n", __LINE__, AmdPbsConfiguration.Usb4NonPrefetchMemoryReserved));
  DEBUG ((DEBUG_INFO, "UpdateAmdPbsSetupData [%d]: AmdPbsConfiguration.Usb4PrefetchMemoryReserved =%d\n", __LINE__, AmdPbsConfiguration.Usb4PrefetchMemoryReserved));

  CpmTableProtocolPtr = NULL;
  if (CpmTableProtocolPtr == NULL) {
    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, (VOID**)&CpmTableProtocolPtr);
    ASSERT_EFI_ERROR(Status);
  }

  PlatformSel = 0;
  if (!EFI_ERROR (Status)) {
    //  If ((0x93 == 0x0X) && (0x94 == 0x02))   this is Lilac SLT
    //  If ((0x93 == 0x1X) && (0x94 == 0x03))   this is Mayan SLT
    //  If ((0x93 == 0x5X) && (0x94 == 0x04))   this is Lilac KD SLT
    //  If ((0x93 == 0x6X) && (0x94 == 0x04))   this is Mayan KD SLT

    // PBSPlatformSel
    // HighByte[7:4]  used for SLT
      // 2 - D595 Lilac SLT
      // 3 - D597 Mayan SLT
    // LowByte [3:0]  used for CRB
      // 0 - D595 Lilac
      // 1 - D597 Mayan
      // 2 - D596 Lilac DAP
      // 3 - D598 Mayan DAP
      // 5 - D884 Lilac_KD
      // 6 - D883 Mayan_KD
    index = 0x93;
    CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &index, &data8);
    PlatformSel |= (data8 & 0xF0) >> 4;

    index = 0x94;
    CpmTableProtocolPtr->CommonFunction.KbcRead (CFG_CPM_ECRAM_BASE_ADDR, 0x80, &index, &data8);
    if ((PlatformSel == 0) && ((data8 & 0x0F) == 2)) {   // Lilac SLT
      PlatformSel = 0x20;
    }
    if ((PlatformSel == 1) && ((data8 & 0x0F) == 3)) {   // Mayan SLT
      PlatformSel = 0x31;
    }
    if ((PlatformSel == 5) && ((data8 & 0x0F) == 4)) {   // Lilac KD SLT
      PlatformSel = 0x45;
    }
    if ((PlatformSel == 6) && ((data8 & 0x0F) == 4)) {   // Mayan KD SLT
      PlatformSel = 0x46;
    }
    if (PlatformSel != AmdPbsConfiguration.PBSPlatformSel) {
      NeedUpdatePBS = TRUE;
      AmdPbsConfiguration.PBSPlatformSel = PlatformSel;
    }
    DEBUG (( DEBUG_INFO, "AmdPbsConfiguration.PBSPlatformSel = [%x]\n", AmdPbsConfiguration.PBSPlatformSel));
  }

  // Check Platform ID changed.

  if (AmdPbsConfiguration.CurrentId != PcdGet16 (CurrentPlatformId)) {
    NeedUpdatePBS = TRUE;
    AmdPbsConfiguration.CurrentId = PcdGet16 (CurrentPlatformId);
  }
  if (AmdPbsConfiguration.AmdCpmUsedPbsSize != AMD_CPM_USED_PBS_SIZE) {
    NeedUpdatePBS = TRUE;
    AmdPbsConfiguration.AmdCpmUsedPbsSize = AMD_CPM_USED_PBS_SIZE;
  }
  if (AmdPbsConfiguration.PlatformId != AmdPbsConfiguration.CurrentId ) {
    NeedUpdatePBS = TRUE;
    AmdPbsConfiguration.PlatformId = AmdPbsConfiguration.CurrentId;
  }
  DEBUG ((DEBUG_INFO, "OEM-DXE-UpdateAmdPbsSetupData-AmdPbsConfiguration.PlatformId         = 0x%x\n", AmdPbsConfiguration.PlatformId));
  DEBUG ((DEBUG_INFO, "OEM-DXE-UpdateAmdPbsSetupData-AmdPbsConfiguration.CurrentId          = 0x%x\n", AmdPbsConfiguration.CurrentId));
  DEBUG ((DEBUG_INFO, "OEM-DXE-UpdateAmdPbsSetupData-AmdPbsConfiguration.AmdCpmUsedPbsSize  = 0x%x\n", AMD_CPM_USED_PBS_SIZE));
  DEBUG ((DEBUG_INFO, "OEM-DXE-UpdateAmdPbsSetupData-AmdPbsConfiguration.AmdCpmUsedPbsSize  = 0x%x\n", AmdPbsConfiguration.AmdCpmUsedPbsSize));

  // Sync with Security Level 3 PCD
  if (AmdPbsConfiguration.SecurityLvl3SetupDxe != (FeaturePcdGet (PcdModernStandbyEnableForLevel3) ? 1 : 0)) {
    NeedUpdatePBS = TRUE;
    AmdPbsConfiguration.SecurityLvl3SetupDxe = FeaturePcdGet (PcdModernStandbyEnableForLevel3) ? 1 : 0;
  }

  if ((1 == AmdPbsConfiguration.SecurityLvl3SetupDxe) && (AmdPbsConfiguration.ModernStandbyEn != 1))
  {
    // Force Enable
    NeedUpdatePBS = TRUE;
    AmdPbsConfiguration.ModernStandbyEn = 1;
  }
  if (0 != PcdGet32 (PcdAmdEnvironmentFlag) && (AmdPbsConfiguration.ModernStandbyEn != 0))
  {
    NeedUpdatePBS = TRUE;
    AmdPbsConfiguration.ModernStandbyEn = 0;
  }

  if (0 == AmdPbsConfiguration.APICSwEnable)
  {
    Svr.Uint32 = ReadLocalApicReg (XAPIC_SPURIOUS_VECTOR_OFFSET);
    Svr.Bits.SoftwareEnable = 0;
    WriteLocalApicReg (XAPIC_SPURIOUS_VECTOR_OFFSET, Svr.Uint32);
  }


  if (NeedUpdatePBS)
  {
    Status = SetAmdPbsConfiguration (&AmdPbsConfiguration);
    DEBUG ((DEBUG_INFO, "UpdateAmdPbsSetupData [%d]: Update AMD PBS SetupVariable - %r\n", __LINE__, Status));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}


/**
 *---------------------------------------------------------------------------------------
 *
 *  AmdPbsDxeEntry
 *
 *  Description:
 *     Entry point for the cpu driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         PMODULE_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
AmdPbsSetupDxeEntry (
  IN       EFI_HANDLE             ImageHandle,
  IN       EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HII_DATABASE_PROTOCOL       *HiiDatabase;
  EFI_HII_STRING_PROTOCOL         *HiiString;
  EFI_HII_CONFIG_ROUTING_PROTOCOL *HiiConfigRouting;
  AMD_PBS_DXE_PRIVATE_DATA        *Private;
//  AMD_PBS_SETUP_OPTION            AmdPbsConfiguration;
//  UINT8                           ReflashFlag;
//  AMD_PBS_SETUP_OPTION            AmdPbsTempConfiguration;
//  UINT8                           Index;
//  UINT8                           DefaultUsedPbsSize;
  UINT32                          TempData32;
  UINT16                          CurrentBoardId;

  TempData32 = 0;
  CurrentBoardId = 0;
#if 0
  /*
  ReflashFlag = 0;
  Index = 0;
  DefaultUsedPbsSize = 46;  // It's RV PI 1001 DefaultUsedPbsSize.

  Status = GetAmdPbsConfiguration (&AmdPbsConfiguration);
  if (EFI_ERROR (Status)) {
    ReflashFlag |= BIT4 + BIT0;
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-  Error:  Failed to get AMD PBS variables\n"));
  }

  DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-AmdPbsConfiguration.AmdCpmUsedPbsSize = 0x%X\n", AmdPbsConfiguration.AmdCpmUsedPbsSize));
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-AMD_CPM_USED_PBS_SIZE = 0x%X\n", AMD_CPM_USED_PBS_SIZE));
  if (AmdPbsConfiguration.AmdCpmUsedPbsSize > 0) {
    DefaultUsedPbsSize = AmdPbsConfiguration.AmdCpmUsedPbsSize;
  }

  if (DefaultUsedPbsSize < AMD_CPM_USED_PBS_SIZE) {
    ReflashFlag |= BIT4 + BIT1;
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-  Notice: Add new created AMD PBS variables in NVRAM\n"));
  }

  //Update CPU type

  TempData32 =  (PcdGet32 (PcdGetRVFamilyOPN) >> 30) & 0x3;
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry- PcdGetRVFamilyOPN = 0x%X\n", PcdGet32 (PcdGetRVFamilyOPN)));
  DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry- Cpu die Type = 0x%X\n", TempData32));

  if (AmdPbsConfiguration.CpuType != (UINT8)TempData32) {
    ReflashFlag |= BIT4;
  if (TempData32 == 0) {
      AmdPbsConfiguration.CpuType = 0;
    } else if (TempData32 == 1 || TempData32 == 2 || TempData32 == 3) {
      AmdPbsConfiguration.CpuType = 1;
    }
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-  Notice: Update AMD PBS CpuType = 0x%X in NVRAM\n", AmdPbsConfiguration.CpuType));
  }

  if (AmdPbsConfiguration.PlatformId != ((PcdGet16 (OriginalPlatformId) & BIT2) >> 2)) {
    ReflashFlag |= BIT4;
    AmdPbsConfiguration.PlatformId = ((PcdGet16 (OriginalPlatformId) & BIT2) >> 2);
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-  Notice: Update AMD PBS PlatformId = 0x%X in NVRAM\n", AmdPbsConfiguration.PlatformId));
  }

  CurrentBoardId = PcdGet16 (CurrentPlatformId);
  if (AmdPbsConfiguration.CurrentId != (CurrentBoardId)) {
    ReflashFlag |= BIT4;
    AmdPbsConfiguration.CurrentId = CurrentBoardId;
    DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-  Notice: Update AMD PBS CurrentId = 0x%X in NVRAM\n", AmdPbsConfiguration.CurrentId));
  }

  if (ReflashFlag & BIT0) {
    LoadAmdPbsDefault (&AmdPbsConfiguration);
  } else if (ReflashFlag & BIT1) {
    LoadAmdPbsDefault (&AmdPbsTempConfiguration);
    CopyMem (((UINT8 *) &AmdPbsConfiguration) + DefaultUsedPbsSize, ((UINT8 *) &AmdPbsTempConfiguration) + DefaultUsedPbsSize, AMD_CPM_USED_PBS_SIZE - DefaultUsedPbsSize);
    for (Index = DefaultUsedPbsSize; Index < AMD_CPM_USED_PBS_SIZE; Index ++) {
      DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-Update PBS Index[0x%X] = 0x%X to 0x%X\n", Index, ((UINT8 *) &AmdPbsConfiguration)[Index], ((UINT8 *) &AmdPbsConfiguration)[Index]));
    }
    if (AmdPbsConfiguration.AmdCpmUsedPbsSize != AMD_CPM_USED_PBS_SIZE) {
      AmdPbsConfiguration.AmdCpmUsedPbsSize = AMD_CPM_USED_PBS_SIZE;
      DEBUG((DEBUG_INFO, "OEM-DXE-AmdPbsSetupDxeEntry-Update AmdCpmUsedPbsSize to 0x%X\n", AmdPbsConfiguration.AmdCpmUsedPbsSize));
    }
  }
  */
#else

  do
  {
    VOID          *Interface = NULL;
    EFI_EVENT     Event;
    VOID          *Registration;

    Status = gBS->LocateProtocol (
                   &gEfiPciEnumerationCompleteProtocolGuid,
                   NULL,
                   &Interface
                   );
    if (!EFI_ERROR (Status))
    {
      ConfigRts5261 (NULL, NULL);
      break;
    }

    Event = EfiCreateProtocolNotifyEvent (
                       &gEfiPciEnumerationCompleteProtocolGuid,
                       TPL_NOTIFY,
                       ConfigRts5261,
                       NULL,
                       &Registration
                       );
    if (EFI_ERROR (Status))
    {
      break;
    }

  } while (FALSE);


  Status = UpdateAmdPbsSetupData ();
  if (EFI_ERROR (Status))
  {
    // Fail to Sync AmdPbsSetup Data.
  }

#endif
  Private = AllocateZeroPool (sizeof (AMD_PBS_DXE_PRIVATE_DATA));
  if (Private == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Private->Signature   = AMD_PBS_DXE_PRIVATE_SIGNATURE;

  Private->ConfigAccess.ExtractConfig = AmdPbsExtractConfig;
  Private->ConfigAccess.RouteConfig   = AmdPbsRouteConfig;
  Private->ConfigAccess.Callback      = AmdPbsCallback;

  Private->IfrData = AllocateZeroPool (sizeof (AMD_PBS_SETUP_OPTION));
  if (NULL == Private->IfrData)
  {
    DEBUG ((DEBUG_INFO, "[%d]: Fatal Error on allocate memory to Setup PBS - %r\n", __LINE__, Status));
    ASSERT (Private->IfrData != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **) &HiiDatabase);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Private->HiiDatabase = HiiDatabase;

  //
  // Locate HiiString protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Private->HiiString = HiiString;

  //
  // Locate ConfigRouting protocol
  //
  Status = gBS->LocateProtocol (&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID **) &HiiConfigRouting);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Private->HiiConfigRouting = HiiConfigRouting;

  Private->DriverHandle = NULL;              // if NULL, handle will be assigned.

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Private->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &Private->ConfigAccess,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Publish the HII package list
  //
  Private->HiiHandle = HiiAddPackages (
                 &mFormSetGuid,
                 Private->DriverHandle,
                 AmdPbsSetupDxeStrings,
                 AmdPbsHiiBin,
                 NULL);
  if (Private->HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
                  &Private->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mHiiVendorDevicePath0,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &Private->ConfigAccess,
                  NULL
                  );

    return EFI_OUT_OF_RESOURCES;
  }

  return Status;
}
