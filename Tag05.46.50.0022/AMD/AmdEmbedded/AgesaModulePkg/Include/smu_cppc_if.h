/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 * Contains the CPPC definitios. This file is periodically synced with the external SMU source repository
 *
 *
 */

#ifndef SMU_CPPC_IF_H_
#define SMU_CPPC_IF_H_

#define CPPC_SHM_SIGNATURE 0x50434300

/* From ACPI spec: */
/* The doorbell is used by OSPM to notify the platform that the shared */
/* memory region contains a valid command that is ready to be */
/* processed. A doorbell consists of a hardware register that is accessed */
/* via I/O or memory mapped I/O, abstracted in the doorbell field of the */
/* PCC subspace structure. OSPM rings the doorbell by performing a */
/* read/modify/write cycle on the specified register, preserving and */
/* setting the bits specified in the preserve and write mask of the PCC */
/* subspace structure. */

/*    To ensure consistency of the shared memory region, the shared */
/* memory region is exclusively owned by OSPM or the platform at any */
/* point in time. After being initialized by the platform, the region is */
/* owned by OSPM. Writing the doorbell register transfers ownership of */
/* the memory region to the platform. The platform transfers ownership */
/* back by setting the Command Complete bit in the Status field. */
#define mmMP1_DOORBELL_REG  mmMP1_C2PMSG_12

// We use 256 bytes per logical core.
#define CPPC_CPC_REGISTER_SPACE_SIZE 256
typedef struct {
  uint32_t HighestPerformance;
  uint32_t NominalPerformance;
  uint32_t LowestNonlinearPerformance;
  uint32_t LowestPerformance;
  uint32_t GuaranteedPerformance;
  uint32_t DesiredPerformance;
  uint32_t MinimumPerformance;
  uint32_t MaximumPerformance;
  uint32_t PerformanceReductionTolerance;
  uint32_t TimeWindow;
  uint64_t CounterWraparoundTime;
  uint64_t ReferencePerformanceCounter;
  uint64_t DeliveredPerformanceCounter;
  uint32_t PerformanceLimited;
  uint32_t CPPCEnable;
  uint32_t AutonomousSelectionEnable;
  uint32_t AutonomousActivityWindow;
  uint32_t EnergyPerformancePreference;
  uint32_t ReferencePerformance;
/* 0x58-0XFF  -  -  Reserved (other than last CPU)  - */
/* 0x58-0XF7  -  -  Reserved (last CPU)  - */
} CPPC_CPC_Register_Space_t;

// This is used to communicate to BIOS the registers that our CPPC
// implementation is using. BIOS will generate the DSDT according to
// this.
typedef struct {
  uint32_t Version                       :  2;
  uint32_t HighestPerformance            :  1;
  uint32_t NominalPerformance            :  1;
  uint32_t LowestNonlinearPerformance    :  1;
  uint32_t LowestPerformance             :  1;
  uint32_t GuaranteedPerformance         :  1;
  uint32_t DesiredPerformance            :  1;
  uint32_t MinimumPerformance            :  1;
  uint32_t MaximumPerformance            :  1;
  uint32_t PerformanceReductionTolerance :  1;
  uint32_t TimeWindow                    :  1;
  uint32_t CounterWraparoundTime         :  1;
  uint32_t ReferencePerformanceCounter   :  1;
  uint32_t DeliveredPerformanceCounter   :  1;
  uint32_t PerformanceLimited            :  1;
  uint32_t CPPCEnable                    :  1;
  uint32_t AutonomousSelectionEnable     :  1;
  uint32_t AutonomousActivityWindow      :  1;
  uint32_t EnergyPerformancePreference   :  1;
  uint32_t ReferencePerformance          :  1;
  uint32_t LowestFrequency               :  1;
  uint32_t NominalFrequency              :  1;
  uint32_t Reserved                      :  9;
} CPPC_Supported_Registers_t;

typedef union {
  uint32_t val;
  CPPC_Supported_Registers_t f;
} CPPC_Supported_Registers_u;

typedef struct {
  uint8_t Command;
  uint8_t Reserved;
} PCC_Command_t;

typedef union {
  uint16_t val;
  PCC_Command_t f;
} PCC_Command_u;

typedef struct {
  uint32_t Command_Complete      :  1;
  uint32_t Doorbell_Interrupt    :  1;
  uint32_t Error                 :  1;
  uint32_t Platform_Notification :  1;
  uint32_t Reserved              : 12;
} PCC_Status_t;

typedef union {
  uint16_t val;
  PCC_Status_t f;
} PCC_Status_u;

typedef struct {
  uint32_t Signature;
  PCC_Command_u Command;
  PCC_Status_u Status;
} PCC_Shared_Memory_Header_t;

// AGESA Addition:
// Defines from CPPC architect. Pushing to get these officially included in this file
#define CPPC_NOMINAL_LATENCY            4000
#define CPPC_MAX_PERIODIC_ACCESS_RATE   (60 * 1000)   // Max supported commands per min (60 * 1000ms)
#define CPPC_MIN_REQ_TURNAROUND_TIME    0

#endif /* SMU_CPPC_IF_H_ */


