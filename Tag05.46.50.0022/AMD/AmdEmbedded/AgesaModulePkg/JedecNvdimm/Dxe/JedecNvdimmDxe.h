/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************/

#ifndef _JEDEC_NVDIMM_DXE_H_
#define _JEDEC_NVDIMM_DXE_H_

#define DSM_ERASE_NVM_IMAGE (19)
#define DSM_ARM_NVDIMM_N (20)
#define DSM_I2C_READ_27   (27)
#define DSM_I2C_WRITE_28   (28)

// -----------------------------------------------------------------------------------------------------
//
// DDR4 NVDIMM controller register
//
// -----------------------------------------------------------------------------------------------------
#define OPEN_PAGE                           0
#define ENERGY_SOURCE_POLICY                0x14
  #define DEVICE_MANAGED_POLICY_SUPPORTED   0x1
  #define HOST_MANAGED_POLICY_SUPPORTED     0x2

#define HWREV                               0x4
#define SPECREV                             0x6
#define SLOT0_FWREV0                        0x7
#define SLOT0_FWREV1                        0x8
#define SLOT1_FWREV0                        0x9
#define SLOT1_FWREV1                        0xa

#define CAPABILITIES1                       0x11
  #define ATOMIC_ARM_AND_ERASE_SUPPORTED    0x1

#define CSAVE_TRIGGER_SUPPORT               0x16
  #define SAVE_N                            0x1

#define CSAVE_TIMEOUT0                      0x18
#define CSAVE_TIMEOUT1                      0x19

#define PAGE_SWITCH_LATENCY0                0x1a

#define PAGE_SWITCH_LATENCY1                0x1b
  #define UNIT_IN_SECOND                    0x80
  #define LATENCY1_MASK                     0x7f

#define RESTORE_TIMEOUT0                    0x1c
#define RESTORE_TIMEOUT1                    0x1d

#define ERASE_TIMEOUT0                      0x1e
#define ERASE_TIMEOUT1                      0x1f

#define ARM_TIMEOUT0                        0x20
#define ARM_TIMEOUT1                        0x21
#define ABORT_CMD_TIMEOUT                   0x24

#define NVDIMM_MGT_CMD0                     0x40
  #define RST_CTRL                          0x1
  #define CL_ALL_CMD_STAT                   0x2
  #define CL_SAVE_STAT                      0x4
  #define CL_RESTORE_STAT                   0x8
  #define CL_ERASE_STAT                     0x10
  #define CL_ARM_STAT                       0x20
  #define CL_SET_EVENT_NOTIFICATION_STAT    0x40
  #define CL_SET_ES_STAT                    0x80
  #define CL_ALL                            0xfe

#define NVDIMM_FUNC_CMD0                    0x43
  #define START_FACTORY_DEFAULT             0x1
  #define START_RESTORE                     0x4
  #define START_ERASE                       0x8
  #define ABORT_CURRENT_OP                  0x10

#define ARM_CMD                             0x45
  #define ARM_SAVE_N                        0x1
  #define ATOMIC_ARM_AND_ERASE              0x80

#define SET_ES_POLICY_CMD                   0x49
  #define DEVICE_MANAGED_POLICY             0x1
  #define HOST_MANAGED_POLICY               0x2

#define NVDIMM_READY                        0x60

#define NVDIMM_CMD_STATUS0                  0x61
  #define CONTROLLER_BUSY                   0x1
  #define FACTORY_DEFAULT_IN_PROGRESS       0x2
  #define SAVE_IN_PROGRESS                  0x4
  #define RESTORE_IN_PROGRESS               0x8
  #define ERASE_IN_PROGRESS                 0x10
  #define ABORT_IN_PROGRESS                 0x20
  #define ARM_IN_PROGRESS                   0x40
  #define FIRMWARE_OPS_IN_PROGRESS          0x80

#define SAVE_STATUS0                        0x64
  #define SAVE_SUCCESS                      0x1
  #define SAVE_ERROR                        0x2

#define RESTORE_STATUS0                     0x66
  #define RESTORE_SUCCESS                   0x1
  #define RESTORE_ERROR                     0x2
  #define ABORT_SUCCESS                     0x10
  #define ABORT_ERROR                       0x20

#define ERASE_STATUS0                       0x68
  #define ERASE_SUCCESS                     0x1
  #define ERASE_ERROR                       0x2

#define ARM_STATUS0                         0x6a
  #define ARM_SUCCESS                       0x1
  #define ARM_ERROR                         0x2
  #define SAVE_N_ARMED                      0x4

#define FACTORY_DEFAULT_STATUS0             0x6C
  #define FACTORY_DEFAULT_SUCCESS           0x1
  #define FACTORY_DEFAULT_ERROR             0x2

#define SET_ES_POLICY_STATUS                0x70
  #define SET_ES_POLICY_SUCCESS             0x1
  #define SET_ES_POLICY_ERROR               0x2
  #define DEVICE_MANAGED_POLICY_ENABLED     0x4

#define CSAVE_INFO0                         0x80
  #define NVM_DATA_VALID                    0x1
  #define SAVE_N_INFO0                      0x4

#define CSAVE_FAIL_INFO0                    0x84
#define CSAVE_FAIL_INFO1                    0x85
#define MODULE_HEALTH                       0xa0
#define MODULE_HEALTH_STATUS0               0xa1
#define MODULE_HEALTH_STATUS1               0xa2
  #define NOT_ENOUGH_ENERGY_FOR_CSAVE       1

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

//
// Functions Prototypes
//

#endif // _JEDEC_NVDIMM_DXE_H_
