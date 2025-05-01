/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Library for Common Platform RAS Function
 *
 * Contains interface to the AMD CPM library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Library
 * @e \$Revision$   @e \$Date$
 *
 */
/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

#ifndef _AMD_CPM_RAS_RMB_LIB_H_
#define _AMD_CPM_RAS_RMB_LIB_H_

#include "Uefi.h"
#include  <AMD.h>
#include "AmdRasRegistersRmb.h"
#include "AmdRas.h"
#include "AmdCpmRas.h"

#include <Library/GnbPciLib.h>

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define SEC_PCIE_EXT_CAP_ID         (0x19)
#define PCIE_EXTENDED_CAP_REG       (0x100)

#define PCI_VENDORID_REG            (0x00)
#define PCI_DEVICEID_REG            (0x02)
#define PCI_COMMAND_REG             (0x04)
#define PCI_CLASS_CODE_0_REG        (0x09)
#define PCI_CLASS_CODE_1_REG        (0x0A)
#define PCI_CLASS_CODE_2_REG        (0x0B)
#define PCI_STATUS_REG              (0x06)
#define PCI_HEADER_TYPE_REG         (0x0E)
  #define MULTI_FUNC_DEVICE_MASK    (BIT7)
  #define PCI_TO_PCI_BRIDGE         (0x01)
#define PCI_PRIMARY_BUS_REG         (0x18)
#define PCI_SECONDARY_BUS_REG       (0x19)
#define PCI_SUBORDINATE_BUS_REG     (0x1A)
#define PCI_SEC_STATUS_REG          (0x1E)
#define PCI_CAPABILITY_REG          (0x34)

#define PCI_BRIDGE_CONTROL_REG      (0x3E)
  #define PCI_BRIDGECTRL_SERR_EN      (BIT1)

#define PCIE_UNCORR_STATUS_PTR      (0x04)
#define PCIE_UNCORR_MASK_PTR        (0x08)
#define PCIE_CORR_STATUS_PTR        (0x10)
#define PCIE_CORR_MASK_PTR          (0x14)
#define PCIE_ROOT_ERR_CMD_PTR       (0x2C)
#define PCIE_ROOT_STATUS_PTR        (0x30)
#define PCIE_ROOT_STATUS_FATAL_ERROR_MASK (BIT4 | BIT6)
#define PCIE_DEVICE_CONTROL_PTR     (0x08)
#define PCIE_DEVICE_STATUS_PTR      (0x0A)

#define PCIE_DEV_CORR_ERR           (0x1)
#define PCIE_DEV_NON_FATAL_ERR      (0x2)
#define PCIE_DEV_FATAL_ERR          (0x4)
#define PCIE_DEV_USR_DETECTED       (0x8)

#define PCIE_ROOT_CORR_ERR          (0x1)
#define PCIE_ROOT_NON_FATAL_ERR     (0x2)
#define PCIE_ROOT_FATAL_ERR         (0x4)

#define PCIE_LINK_CONTROL3_PTR      (0x04)
#define PCIE_LANE_ERR_STATUS_PTR    (0x08)

#define PCIE_CAP_REGISTER           (0x02)
  #define PCIE_DATA_LINK_LAYER_LINK_ACTIVE    (BIT13)
#define PCIE_SLOT_NUMBER_SHIFT      (19)

#define DPC_CAPABILITY_REG          (0x04)
#define DPC_CONTROL_REG             (0x06)
#define DPC_STATUS_REG              (0x08)
#define DPC_ERROR_SOURCE_ID_REG     (0x0A)
#define RP_PIO_STATUS_REG           (0x0C)
#define RP_PIO_MASK_REG             (0x10)
#define RP_PIO_SEVERITY_REG         (0x14)
#define RP_PIO_SYSERROR_REG         (0x18)
#define RP_PIO_EXECPTION_REG        (0x1C)
#define RP_PIO_HEADER_LOG_REG       (0x20)
#define RP_PIO_IMPSPEC_LOG_REG      (0x30)
#define RP_PIO_TLP_PREFIX_LOG_REG   (0x34)

#define INSTANCEID_NOT_FOUND        (0xFF)
#define INVALID_BYTE_RETURN         (0xFF)

#define PCIE_DATA_LINK_POLLING_COUNT  (100)

#define PCIE_ERROR_CHECK        (0x0)
#define CCIX_ERROR_CHECK        (0x1)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

//  Forward declaration needed for multi-structure mutual references
typedef struct _RAS_PCI_SCAN_DATA RAS_PCI_SCAN_DATA;

typedef SCAN_STATUS (*RAS_SCAN_CALLBACK) (
  IN       PCI_ADDR             Device,
  IN OUT   RAS_PCI_SCAN_DATA    *ScanData
  );

///Scan supporting data
struct _RAS_PCI_SCAN_DATA {
  RAS_SCAN_CALLBACK     RasScanCallback;  ///< Callback for each found device
  VOID                  *Buffer;          ///< Input Buffer
};

typedef struct {
    UINT32     FunctionId;
    UINT32     EAX_Reg;
    UINT32     EBX_Reg;
    UINT32     ECX_Reg;
    UINT32     EDX_Reg;
} RAS_CPUID_FN;

/// BSP/AP MSR Access
typedef struct {
  UINT32 RegisterAddress;   ///< MSR Address
  UINT64 RegisterValue;     ///< BSC's MSR Value
  BOOLEAN IsWrite;          ///< Access Type
} PLAT_RAS_MSR_ACCESS;

//PCIe_PORT_AER_CONFIG
typedef struct {
  UINT8     AerEnable;                          ///< General per-port enable, 0=disable 1=enable
  UINT8     PciBus;                             ///< PCI Bus Number
  UINT8     PciDev;                             ///< PCI Device Number
  UINT8     PciFunc;                            ///< PCI Function Number
  UINT32    CorrectableMask;                    ///< Mask for correctable errors
  UINT32    UncorrectableMask;                  ///< Mask for uncorrectable errors
  UINT32    UncorrectableSeverity;              ///< Severity configuration for uncorrectable errors
} PCIe_AER_CONFIG_TEMP;

typedef struct _RAS_ERR_LOG_DATA RAS_ERR_LOG_DATA;

typedef EFI_STATUS (*RAS_LOG_CALLBACK) (
  IN       PCI_ADDR             Device,
  IN OUT   RAS_ERR_LOG_DATA    *ErrLogData
  );

///Error Log supporting data
struct _RAS_ERR_LOG_DATA {
  RAS_LOG_CALLBACK      RasLogCallback;   ///< Callback for each found device
  VOID                  *Buffer;          ///< Input Buffer
};

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
RasSmnRead (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

VOID
RasSmnWrite (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              *Value
  );

VOID
RasSmnRW (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT32              AndMask,
  IN       UINT32              OrMask
  );

VOID
RasSmnRead16 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT16              *Value16
  );

VOID
RasSmnWrite16 (
  IN       UINT32              IohcBus,
  IN       UINT32              SmnAddress,
  IN       UINT16               *Value16
  );

BOOLEAN
RasPciIsDevicePresent (
  IN      UINT32              Address
 );

BOOLEAN
RasPciIsBridgeDevice (
  IN      UINT32              Address
 );

BOOLEAN
RasPciIsMultiFunctionDevice (
  IN      UINT32              Address
 );

BOOLEAN
RasPciIsPcieDevice (
  IN      UINT32              Address
 );

UINT8
RasFindPciCapability (
  IN      UINT32              Address,
  IN      UINT8               CapabilityId
  );

UINT16
RasFindPcieExtendedCapability (
  IN      UINT32              Address,
  IN      UINT16              ExtendedCapabilityId,
  IN      UINT16              CurrentCapabilityPtr
  );

VOID
RasPciScan (
  IN      PCI_ADDR            Start,
  IN      PCI_ADDR            End,
  IN      RAS_PCI_SCAN_DATA   *ScanData
 );

VOID
RasPciScanSecondaryBus (
  IN       PCI_ADDR             Bridge,
  IN OUT   RAS_PCI_SCAN_DATA    *ScanData
  );

PCIE_DEVICE_TYPE
RasGetPcieDeviceType (
  IN      PCI_ADDR            Device
  );

EFI_STATUS
RasPcieStsClr (
  IN       PCI_ADDR PciCfgAddr,
  IN       UINT32   RetryCnt
  );

EFI_STATUS
RasDpcErrHandle (
  IN       PCI_ADDR PciCfgAddr,
  IN       UINT32   RetryCnt
  );

BOOLEAN
RasPciErrCheck(
  IN       PCI_ADDR PciCfgAddr,
  OUT      BOOLEAN  *RootStatusSet,
  OUT      UINT32   *RootErrStatus
  );

EFI_STATUS
RasCcixErrCheck(
  IN       PCI_ADDR           PciCfgAddr,
  IN       RAS_ERR_LOG_DATA   *ErrLogData
  );

EFI_STATUS
RasPcieErrStsCheck(
  IN       PCIE_PORT_PROFILE      *PciePortProfileInstance,
  IN OUT   PCIE_ERR_ENTRY         *PcieErrorEntry,
  OUT      UINT16                 *PcieErrorEntryCnt
  );

EFI_STATUS
RasSlinkErrStsCheck(
  IN       PCIE_PORT_PROFILE  *PciePortProfileInstance,
  IN       RAS_ERR_LOG_DATA   *ErrLogData
  );

EFI_STATUS
SlinkErrorScanDxe (
  UINT8 NbioBusNum
  );

VOID
GetCpuId (
 IN OUT    RAS_CPUID_FN* RasCpuIdFn
 );

VOID
GetPciMmioBase (
  OUT   UINT64 *MmioBase
  );

VOID
GetLocalApicBase (
  OUT   UINT64 *ApicBase
  );

VOID
GetLocalApicId(
  OUT      UINT32 *LocalApicId
  );

VOID
GetSysEnterEip (
  OUT      UINT64* SysEnterEip
  );

VOID
GetMcaBankCount(
  OUT      UINT8* BankCount
  );

UINT8
McaInstanceIdSearch (
  MCA_IPID_MSR McaIpidMsr
  );

VOID
MsrRegisterAccess (
  IN OUT   PLAT_RAS_MSR_ACCESS  *RasMsrAccess
  );

EFI_STATUS
AcquireNbioSecBusNum (
  IN       UINT8 DfNode,
  IN       UINT8 NbioNumber,
  OUT      UINT8 *NbioSecBusNum
  );

UINT32
RasGetDfReg (
  UINT32 instanceId,
  UINT32 nodeId,
  UINT32 regAddr
  );

UINT32
RasSetDfReg (
  UINT32 instanceId,
  UINT32 nodeId,
  UINT32 regAddr,
  UINT32 regData
  );

UINT64
ConvertToMilliseconds (
  IN       UINT64 TscTicks
  );

BOOLEAN
NbioSearchErr (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  );

EFI_STATUS
NbioClrStatus (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  );

EFI_STATUS
NbioResetCounter (
  RAS_NBIO_ERROR_INFO *RasNbioErrorInfo
  );

VOID
RasGetMcaSeverityType(
  IN       RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo,
  IN       UINT8                 BankIndex,
  OUT      UINT8                 *McaSeverityType
  );

BOOLEAN
RasRetry (
  IN       UINT32    *RetryCnt
  );

EFI_STATUS
UpdateMemErrFruText (
     OUT   CHAR8*                FruTextDest,
  IN       UINT16                Node,
  IN       UINT16                Card,
  IN       UINT16                Module,
  IN       OEM_MEMORY_MAP_TABLE  *RasOemDimmMap
);

VOID
STATIC
PcieDevCntlEnableOnFunction (
  IN       PCI_ADDR             Function,
  IN       PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  );

VOID
STATIC
PcieDevCntlEnableOnDevice (
  IN       PCI_ADDR             Device,
  IN       PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  );

SCAN_STATUS
STATIC
PcieDevCntlCallback (
  IN       PCI_ADDR             Device,
  IN OUT   RAS_PCI_SCAN_DATA    *ScanData
  );

VOID
STATIC
PcieDevCntlConfigure (
  IN       PCI_ADDR  DownstreamPort,
  IN       PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  );

EFI_STATUS
EnDevErrReport(
  PCI_ADDR  PciPortAddr,
  PCIe_AER_CONFIG_TEMP  *PcieAerSetting
  );

EFI_STATUS
AerErrorReportControl(
  PCI_ADDR  PciPortAddr,
  BOOLEAN   EnableReport
  );

EFI_STATUS
RasSetPcieAerFeature (
  IN VOID                          *PciePortAerConfigStrucPtr
  );

BOOLEAN
IsHotPlugRp (
  PCI_ADDR  PciPortAddr
  );

UINT8
RasBitPositionToInt (
  UINT32       BitMap
  );

UINT64
RasAlignValue (
  IN UINT64   Value,
  IN UINTN    Alignment,
  IN BOOLEAN  RoundUp
  );
EFI_STATUS
RasPcieErrStsCheckRmb(
  IN       PCIE_PORT_PROFILE      *PciePortProfileInstance,
  IN OUT   PCIE_ERR_ENTRY         *PcieErrorEntry,
  OUT      UINT16                 *PcieErrorEntryCnt,
  IN       BOOLEAN                OsEdrEnabledFlag
  );
#endif // _AMD_CPM_RAS_GN_LIB_H_

