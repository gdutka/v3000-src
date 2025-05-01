/** @file
  TODO: Brief Description of Protocol DriverArpService
  
  TODO: Detailed Description of Protocol DriverArpService

  TODO: Copyright for Protocol DriverArpService
  
  TODO: License for Protocol DriverArpService
  
**/

#ifndef __DRIVER_ARP_SERVICE_H__
#define __DRIVER_ARP_SERVICE_H__

#pragma pack(1)

#define DRIVER_ARP_SERVICE_IMAGE_INFO_REVISION_1      1

typedef struct {
  UINT32                Revision;
  UINT8                 Reserve1[4];
  //
  UINTN                 Id;
  BOOLEAN               IsValid;
  UINT8                 Reserve2[7];
  //
  // Image identifiers
  //
  EFI_DEVICE_PATH_PROTOCOL  *FilePath;  ///< A pointer to the file path portion specific to DeviceHandle 
  GUID                  ImageFileGuid;
  CHAR8                 *ImageFileName;
  //
  // Location of where image was loaded
  //
  EFI_PHYSICAL_ADDRESS  ImageBase;      ///< The base address at which the image was loaded.
  EFI_PHYSICAL_ADDRESS  ImageSize;      ///< The size in bytes of the loaded image.
  EFI_MEMORY_TYPE       ImageCodeType;  ///< The memory type that the code sections were loaded as.
  EFI_MEMORY_TYPE       ImageDataType;  ///< The memory type that the data sections were loaded as.
} DRIVER_ARP_SERVICE_IMAGE_INFO;

//
//
//
#define EFI_DRIVER_ARP_SERVICE_PROTOCOL_REVISION_1    1

#define EFI_DRIVER_ARP_SERVICE_PROTOCOL_GUID \
  {0x3fd52e9e, 0x2e4e, 0x11e4, {0x86, 0x51, 0x94, 0x39, 0xe5, 0x61, 0x61, 0x3a}}

///
/// Forward declaration
///  
typedef struct _EFI_DRIVER_ARP_SERVICE_PROTOCOL EFI_DRIVER_ARP_SERVICE_PROTOCOL;

///
/// Function prototypes
///
typedef
EFI_STATUS
(EFIAPI *EFI_DRIVER_ARP_SERVICE_RETRIEVE_IMAGE_BY_ADDRESS)(
  IN  EFI_DRIVER_ARP_SERVICE_PROTOCOL   *This,
  IN  EFI_PHYSICAL_ADDRESS              Address,
  OUT DRIVER_ARP_SERVICE_IMAGE_INFO     *Info,
  IN OUT UINTN                          *NumberOfInfo
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DRIVER_ARP_SERVICE_FUNCTION_NAME_2)(
  IN EFI_DRIVER_ARP_SERVICE_PROTOCOL  *This
  //
  // Place additional function arguments here
  //
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DRIVER_ARP_SERVICE_FUNCTION_NAME_N)(
  IN EFI_DRIVER_ARP_SERVICE_PROTOCOL  *This
  //
  // Place additional function arguments here
  //
  );

///
/// Protocol structure
///
typedef struct _EFI_DRIVER_ARP_SERVICE_PROTOCOL {
  UINT32            Revision;       ///< Defines the revision of the EFI_DRIVER_ARP_SERVICE_PROTOCOL structure. 
                                    ///< All future revisions will be backward compatible to the current revision.
  EFI_DRIVER_ARP_SERVICE_RETRIEVE_IMAGE_BY_ADDRESS  RetrieveImageByAddress;
//  EFI_DRIVER_ARP_SERVICE_FUNCTION_NAME_2  Function2;
  // . . .
//  EFI_DRIVER_ARP_SERVICE_FUNCTION_NAME_N  FunctionN;
  //
  // Place protocol data fields here
  //
} EFI_DRIVER_ARP_SERVICE_PROTOCOL;

#pragma pack()

extern EFI_GUID gH2ODebugAssistDriverArpServiceProtocolGuid;

#endif
