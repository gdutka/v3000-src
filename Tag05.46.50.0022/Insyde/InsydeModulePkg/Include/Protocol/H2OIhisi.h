/** @file
  H2OIhisi Protocol

;******************************************************************************
;* Copyright (c) 2014 - 2021, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

#ifndef _H2O_IHISI_PROTOCOL_H_
#define _H2O_IHISI_PROTOCOL_H_

#include <Protocol/SmmCpu.h>

#define H2O_IHISI_PROTOCOL_GUID \
  { \
    0x6C23A1EF, 0x2CB7, 0x4A60, {0x8F, 0x8C, 0x08, 0xA3, 0xDE, 0x8D, 0x7A, 0xCF} \
  }


#define H2O_IHISI_PARAM_BUFFER_PROTOCOL_GUID \
  { \
    0xB5750984, 0xDD8A, 0x4E3F, {0x8D, 0x81, 0x50, 0x4C, 0x3C, 0xCC, 0xE4, 0x66} \
  }

typedef struct _H2O_IHISI_PROTOCOL                   H2O_IHISI_PROTOCOL;
typedef struct _H2O_IHISI_PARAM_BUFFER_PROTOCOL      H2O_IHISI_PARAM_BUFFER_PROTOCOL;

#define GetParam ReadCpuReg32
#define SetParam WriteCpuReg32

#pragma pack(1)
typedef struct _H2O_IHISI_PARAMS {
  UINT32 Size;        // Size of struct
  UINT32 Reserved;    // Must be 0

  UINT64 Param1;      // IhisiRax
  UINT64 Param2;      // IhisiRbx
  UINT64 Param3;      // IhisiRcx
  UINT64 Param4;      // IhisiRdx
  UINT64 Param5;      // IhisiRsi
  UINT64 Param6;      // IhisiRdi
  UINT64 Param7;      // IhisiFs
  UINT64 Param8;      // IhisiGs
} H2O_IHISI_PARAMS;
#pragma pack()

typedef
EFI_STATUS
(EFIAPI *IHISI_FUNCTION) (
  VOID
);

typedef struct {
  UINT32                        CmdNumber;
  IHISI_FUNCTION                IhisiFunction;
  UINT8                         Priority;
} IHISI_SMI_SUB_FUNCTION;

typedef enum {
  IhisiLowestPriority        = 0x00,
  IhisiLowPriority           = 0x20,
  IhisiBelowNormalPriority   = 0x40,
  IhisiNormalPriority        = 0x80,
  IhisiAboveNormalPriority   = 0xc0,
  IhisiHighPriority          = 0xe0,
  IhisiHighestPriority       = 0xff
} IHISI_PRIORIY;

/**
  IHISI Registration Protocol function for registering IHISI functions

  @param[in] FunctionNumber        Command code of the IHISI function
  @param[in] IhisiFunction         The IHISI function pointer
  @param[in] Priority              The priority level of this function.

  @retval EFI_SUCCESS              IHISI function successfully registered
  @retval EFI_ABORTED              faile to register IHISI function
  @retval EFI_ALREADY_STARTED      IHISI function already registered
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_REGISTER_COMMAND) (
  IN UINT32                     CmdCode,
  IN IHISI_FUNCTION             IhisiFunction,
  IN UINT8                      Priority
  );

/**
  IHISI Registration Protocol function for registering IHISI functions

  @param[in] CmdCode            Command code of the IHISI function
  @param[in] FromPriority       Functions ranges from Priority
  @param[in] ToPriority         Functions ranges to Priority

  @retval EFI_SUCCESS           IHISI function successfully unregistered
  @retval EFI_ABORTED           Failed to unregister IHISI function
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_REMOVE_FUNCTIONS) (
  IN UINT32                     CommandCode,
  IN UINT8                      FromPriority,
  IN UINT8                      ToPriority
  );


/**
  Execute IHISI Command

  @param[in]  CommandCode       Command code of IHISI function

  @retval EFI_SUCCESS           The given IHISI command is successfully executed.
  @return Other                 Error status returned from  executed IHISI command.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_EXECUTE_COMMAND) (
  IN UINT32                     CommandCode
  );

/**
  This function is a helper function for IHISI functions to get CPU
  registers of the active CPU in order to get data parameters or buffer
  passed from the application.

  @param[in]  RegisterNum       The corresponding IHIS status code to be handled.

  @return                       The content of specfiic CPU register.
**/
typedef
UINT32
(EFIAPI *IHISI_READ_CPU_REGISTER32) (
  IN  EFI_SMM_SAVE_STATE_REGISTER       RegisterNum
  );

/**
  This function is a helper function for IHISI functions to write CPU
  registers of the active CPU.

  @param[in]  RegisterNum       The corresponding IHIS status code to be
                                handled.
  @param[in]  RegisterValue     The value to be written to the index of the CPU
                                registered specified by RegisterNum.

  @return                       The content of specfiic CPU register.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_WRITE_CPU_REGISTER32) (
  IN  EFI_SMM_SAVE_STATE_REGISTER       RegisterNum,
  IN  UINT32                            RegisterValue
  );

/**
  This function check if the buffer overlaps with SMRAM.

  @param[in]  Buffer            Pointer to the first byte in a buffer.
  @param[in]  BufferSize        Unsigned integer that specifies the size of the Buffer, in bytes.

  @retval TRUE                  Buffer overlaps with SMRAM.
  @return FALSE                 Buffer does not overlap with SMRAM.
**/
typedef
BOOLEAN
(EFIAPI *IHISI_BUFFER_OVERLAP_SMRAM) (
  IN VOID                      *Buffer,
  IN UINTN                     BufferSize
  );

/**
  This function check if the whole buffer lies within the command buffer.

  @param[in]  Buffer            Pointer to the first byte in a buffer.
  @param[in]  BufferSize        Unsigned integer that specifies the size of the Buffer, in bytes.

  @retval TRUE                  Whole buffer lies within the command buffer.
  @return FALSE                 Any of byte in buffer lies outside the command buffer.
**/
typedef
BOOLEAN
(EFIAPI *IHISI_BUFFER_IN_CMD_BUFFER) (
  IN VOID                       *Buffer,
  IN UINTN                      BufferSize
  );


/**
  This function returns the current possible security levels and the current security level (if any).

  @param[out]  SecurityLevelsSupported    Pointer to a bitmask that indicates the security levels supported by
                                          IHISI on this platform.
  @param[out]  CurrentSecurityLevel       Pointer to a bitmask that indicates which security level was used when the
                                          IHISI session was opened.

  @retval EFI_SUCCESS                     This function completed successfully.
  @return EFI_INVALID_PARAMETER           SecurityLevelsSupported or CurrentSecurityLevel are NULL.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_GET_AUTH_STATUS) (
  OUT UINT32                    *SecurityLevelsSupported,
  OUT UINT32                    *CurrentSecurityLevel
  );

/**
  This function gets the command buffer and size in bytes of the command buffer.

  @param[out]  Buffer               The address to return a pointer to the command buffer.
  @param[out]  BufferSize           The size in bytes of the return command buffer.

  @retval EFI_SUCCESS               This function completed successfully.
  @return EFI_INVALID_PARAMETER     Buffer or BufferSize is NULL.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_GET_CMD_BUFFER) (
  OUT VOID                      **Buffer,
  OUT UINTN                     *BufferSize
  );

/**
  This function gets the image buffer and size in bytes of the image buffer.

  @param[out]  Buffer               The address to return a pointer to the image buffer.
  @param[out]  BufferSize           The size in bytes of the return image buffer.

  @retval EFI_SUCCESS               This function completed successfully.
  @return EFI_INVALID_PARAMETER     Buffer or BufferSize is NULL.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_GET_IMAGE_BUFFER) (
  OUT VOID                      **Buffer,
  OUT UINTN                     *BufferSize
  );

/**
  This function returns whether the bytes from Buffer through Buffer + BufferSize - 1
  lie entirely within the image buffer (TRUE) or whether one or more bytes lie outside the
  image buffer (FALSE).
  If PcdH2OIhisiImageBufferSupported is set to FALSE, then this function will always
  return TRUE.

  @param[in]  Buffer            Pointer to the first byte in a buffer.
  @param[in]  BufferSize        Unsigned integer that specifies the size of the Buffer, in bytes.

  @retval TRUE                  Whole buffer lies within the image buffer.
  @return FALSE                 Any of byte in buffer lies outside the image buffer.
**/
typedef
BOOLEAN
(EFIAPI *IHISI_BUFFER_IN_IMAGE_BUFFER) (
  IN VOID                       *Buffer,
  IN UINTN                      BufferSize
  );

/**
  Handle all registered handlers for the specified IHISI command with priorities greater than
  or equal to FromPriority and less than or equal to ToPriority, or until one of the handlers
  returns IHISI_END_FUNCTION_CHAIN.

  Functions ranges from FromPriority to ToPriority (inclusive) in the IHISI command function
  chain will be executed.

  @param[in]  CmdCode            Pointer to the first byte in a buffer.
  @param[in]  FromPriority       Vlaue of From Priority
  @param[in]  ToPriority         Vlaue of To Priority

  @retval TRUE                  Whole buffer lies within the image buffer.
  @return FALSE                 Any of byte in buffer lies outside the image buffer.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_EXECUTE_COMMAND_BY_PRIORITY) (
  IN UINT32         CmdCode,
  IN UINT8          FromPriority,
  IN UINT8          ToPriority
  );

/**
  This function gets the parameter buffer and size in bytes of the parameter buffer.

  @param[out] ParamBuffer        points to the address of the first byte of the parameter buffer.
  @param[out] ParamBufferSize    points to the unsigned integer that indicates the size of the
                                 parameter buffer.

  @retval EFI_SUCCESS            Function completed successfully.
  @return EFI_NOT_FOUND          Parameter Buffer not found.
  @return EFI_INVALID_PARAMETER  ParamBuffer is NULL or ParamBufferSize is NULL.
**/
typedef
EFI_STATUS
(EFIAPI *IHISI_GET_PARAM_BUFFER) (
  OUT H2O_IHISI_PARAMS    **ParamBuffer,
  OUT UINT32              *ParamBufferSize
  );

struct _H2O_IHISI_PROTOCOL {
  UINT32                               Size;
  IHISI_REGISTER_COMMAND               RegisterCommand;
  IHISI_REMOVE_FUNCTIONS               RemoveFunctions;
  IHISI_EXECUTE_COMMAND                ExecuteCommand;
  IHISI_READ_CPU_REGISTER32            ReadCpuReg32;
  IHISI_WRITE_CPU_REGISTER32           WriteCpuReg32;
  IHISI_BUFFER_OVERLAP_SMRAM           BufferOverlapSmram;
  IHISI_BUFFER_IN_CMD_BUFFER           BufferInCmdBuffer;
  IHISI_GET_AUTH_STATUS                GetAuthStatus;
  IHISI_GET_CMD_BUFFER                 GetCmdBuffer;
  IHISI_GET_IMAGE_BUFFER               GetImageBuffer;
  IHISI_BUFFER_IN_IMAGE_BUFFER         BufferInImageBuffer;
  IHISI_EXECUTE_COMMAND_BY_PRIORITY    ExecuteCommandByPriority;
  IHISI_GET_PARAM_BUFFER               GetParamBuffer;
};

struct _H2O_IHISI_PARAM_BUFFER_PROTOCOL {
  EFI_PHYSICAL_ADDRESS       ParamBuffer;
};


extern EFI_GUID gH2OIhisiProtocolGuid;
extern EFI_GUID gH2OIhisiParamBufferProtocolGuid;
#endif
