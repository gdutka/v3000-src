/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

  /* Macros to aid debugging */
  /* These definitions expand to zero (0) bytes of code when disabled */

#ifndef _IDS_H_
#define _IDS_H_

#include <AMD.h>
#include <AgesaPostCode.h>

#undef FALSE
#undef TRUE
#define FALSE 0
#define TRUE  1
#define IDS_STRINGIZE(a) #a ///< for define stringize macro
#ifndef IDS_DEADLOOP
  #define IDS_DEADLOOP()    { volatile UINTN __i; __i = 1; while (__i); }
#endif

#define   LOW_LEVEL_VERBOSITY    0x1    ///< Low level verbosity
#define   PROD_LEVEL_VERBOSITY   0x2    ///< Production level verbosity
#define   HIGH_LEVEL_VERBOSITY   0x3    ///< High level verbosity
/**
 *  Make a Progress Report to the User, verbosity is set to PROD_LEVEL_VERBOSITY
 *
 *  This Macro is always enabled. The default action is to write the TestPoint value
 *  to an I/O port. The I/O port is 8 bits in size and the default address is 0x80.
 *  IBVs can change AGESA's default port by defining IDS_DEBUG_PORT to desired port
 *  in OptionsIds.h in their build tip.
 *
 *  @param[in]      TestPoint       The value for display indicating progress
 *  @param[in,out]  StdHeader       Pointer of AMD_CONFIG_PARAMS
 *
 **/

#define AGESA_TESTPOINT(TestPoint, StdHeader) IdsAgesaTestPoint ((TestPoint), (StdHeader))

/**
 *  Make a Progress Report to the User with LOW_LEVEL_VERBOSITY
 *
 *
 *  @param[in]      TestPoint       The value for display indicating progress
 *
 **/

#define AGESA_TESTPOINT_LOW(TestPoint) IdsAgesaTestPointVerbosity ((TestPoint), LOW_LEVEL_VERBOSITY)


/**
 *  Make a Progress Report to the User with PROD_LEVEL_VERBOSITY
 *
 *
 *  @param[in]      TestPoint       The value for display indicating progress
 *
 **/

#define AGESA_TESTPOINT_PROD(TestPoint) IdsAgesaTestPointVerbosity ((TestPoint), PROD_LEVEL_VERBOSITY)


/**
 *  Make a Progress Report to the User with HIGH_LEVEL_VERBOSITY
 *
 *
 *  @param[in]      TestPoint       The value for display indicating progress
 *
 **/

#define AGESA_TESTPOINT_HIGH(TestPoint) IdsAgesaTestPointVerbosity ((TestPoint), HIGH_LEVEL_VERBOSITY)

#ifdef STOP_CODE
  #undef STOP_CODE
#endif
#define STOP_CODE (((UINT32)FILECODE)*0x10000ul + \
                    ((__LINE__) % 10) + (((__LINE__ / 10) % 10)*0x10) + \
                    (((__LINE__ / 100) % 10)*0x100) + (((__LINE__ / 1000) % 10)*0x1000))
/**
 * @def STOP_HERE
 *  (macro) - Causes program to halt. This is @b only for use during active debugging .
 *
 *  Causes the program to halt and display the file number of the source of the
 *  halt (displayed in decimal).
 *
 **/
#ifndef IDSOPT_NDEBUG
  #define STOP_HERE IdsAssert (STOP_CODE);
#else
  #define STOP_HERE
#endif

/**
 * @def ASSERT
 *  Test an assertion that the given statement is True.
 *
 *  The statement is evaluated to a boolean value. If the statement is True,
 *  then no action is taken (no error). If the statement is False, a error stop
 *  is generated to halt the program. Used for testing for fatal errors that
 *  must be resolved before production. This is used to do parameter checks,
 *  bounds checking, range checks and 'sanity' checks.
 *
 * @param[in]   conditional    Assert that evaluating this conditional results in TRUE.
 *
 **/
#ifndef IDSOPT_NDEBUG
  #ifndef ASSERT
    #define ASSERT(conditional)   \
      do {                        \
        if (!(conditional)) {     \
          IdsAssert (STOP_CODE);  \
        }                         \
      } while (FALSE)
  #endif
#else
  #ifndef ASSERT
    #define ASSERT(conditional)
  #endif
#endif

/**
 * @def DEBUG_CODE
 * Make the code active when IDSOPT_DEBUG_CODE_ENABLED enable
 *
 */
#ifndef IDSOPT_NDEBUG
  #ifndef DEBUG_CODE
    #define  DEBUG_CODE(Code) Code
  #endif
#else
  #ifndef DEBUG_CODE
    #define  DEBUG_CODE(Code)
  #endif
#endif

/**
 * @def IDS_ERROR_TRAP
 * Trap AGESA Error events with stop code display.
 *
 * Works similarly to use of "ASSERT (FALSE);"
 *
 */
#ifndef IDSOPT_NDEBUG
  #define IDS_ERROR_TRAP  IdsAssert (STOP_CODE)
#else
  #define IDS_ERROR_TRAP
#endif


 //Note a is from 0 to 63
#define DEBUG_PRINT_SHIFT(a)   ((UINT64)1 << a)
//If you change the Bitmap definition below, please change the Hash in ParseFilter of hdtout2008.pl accordingly
//Memory Masks
#define MEM_SETREG                                  DEBUG_PRINT_SHIFT (0)
#define MEM_GETREG                                  DEBUG_PRINT_SHIFT (1)
#define MEM_FLOW                                    DEBUG_PRINT_SHIFT (2)
#define MEM_STATUS                                  DEBUG_PRINT_SHIFT (3)
#define MEM_UNDEF_BF                                DEBUG_PRINT_SHIFT (4)
#define MEM_PMU                                     DEBUG_PRINT_SHIFT (5)
#define MEMORY_TRACE_RSV3                           DEBUG_PRINT_SHIFT (6)
#define MEMORY_TRACE_RSV4                           DEBUG_PRINT_SHIFT (7)
#define MEMORY_TRACE_RSV5                           DEBUG_PRINT_SHIFT (8)
#define MEMORY_TRACE_RSV6                           DEBUG_PRINT_SHIFT (9)

//CPU Masks
#define CPU_TRACE                                   DEBUG_PRINT_SHIFT (10)
#define CPU_TRACE_RSV1                              DEBUG_PRINT_SHIFT (11)
#define CPU_TRACE_RSV2                              DEBUG_PRINT_SHIFT (12)
#define CPU_TRACE_RSV3                              DEBUG_PRINT_SHIFT (13)
#define CPU_TRACE_RSV4                              DEBUG_PRINT_SHIFT (14)
#define CPU_TRACE_RSV5                              DEBUG_PRINT_SHIFT (15)
#define CPU_TRACE_RSV6                              DEBUG_PRINT_SHIFT (16)
#define CPU_TRACE_RSV7                              DEBUG_PRINT_SHIFT (17)
#define CPU_TRACE_RSV8                              DEBUG_PRINT_SHIFT (18)
#define CPU_TRACE_RSV9                              DEBUG_PRINT_SHIFT (19)

//GNB Masks
#define GNB_TRACE                                   DEBUG_PRINT_SHIFT (20)
#define PCIE_MISC                                   DEBUG_PRINT_SHIFT (21)
#define PCIE_PORTREG_TRACE                          DEBUG_PRINT_SHIFT (22)
#define PCIE_HOSTREG_TRACE                          DEBUG_PRINT_SHIFT (23)
#define GNB_TRACE_RSV2                              DEBUG_PRINT_SHIFT (24)
#define NB_MISC                                     DEBUG_PRINT_SHIFT (25)
#define GNB_TRACE_RSV3                              DEBUG_PRINT_SHIFT (26)
#define GFX_MISC                                    DEBUG_PRINT_SHIFT (27)
#define NB_SMUREG_TRACE                             DEBUG_PRINT_SHIFT (28)
#define GNB_TRACE_RSV1                              DEBUG_PRINT_SHIFT (29)

//Topology Masks
#define TOPO_TRACE                                  DEBUG_PRINT_SHIFT (30)
// #define TOPO_TRACE_RSV1                             DEBUG_PRINT_SHIFT (31)
// #define TOPO_TRACE_RSV2                             DEBUG_PRINT_SHIFT (32)
// #define TOPO_TRACE_RSV3                             DEBUG_PRINT_SHIFT (33)
// #define TOPO_TRACE_RSV4                             DEBUG_PRINT_SHIFT (34)
// #define TOPO_TRACE_RSV5                             DEBUG_PRINT_SHIFT (35)
// #define TOPO_TRACE_RSV6                             DEBUG_PRINT_SHIFT (36)
// #define TOPO_TRACE_RSV7                             DEBUG_PRINT_SHIFT (37)
// #define TOPO_TRACE_RSV8                             DEBUG_PRINT_SHIFT (38)
// #define TOPO_TRACE_RSV9                             DEBUG_PRINT_SHIFT (39)
#define PSP_TRACE                                   DEBUG_PRINT_SHIFT (31)

//FCH Masks
#define FCH_TRACE                                    DEBUG_PRINT_SHIFT (40)
#define FCH_TRACE_RSV1                               DEBUG_PRINT_SHIFT (41)
#define FCH_TRACE_RSV2                               DEBUG_PRINT_SHIFT (42)
#define FCH_TRACE_RSV3                               DEBUG_PRINT_SHIFT (43)
#define FCH_TRACE_RSV4                               DEBUG_PRINT_SHIFT (44)
#define FCH_TRACE_RSV5                               DEBUG_PRINT_SHIFT (45)
#define FCH_TRACE_RSV6                               DEBUG_PRINT_SHIFT (46)
#define FCH_TRACE_RSV7                               DEBUG_PRINT_SHIFT (47)
#define FCH_TRACE_RSV8                               DEBUG_PRINT_SHIFT (48)
#define FCH_TRACE_RSV9                               DEBUG_PRINT_SHIFT (49)

//Other Masks
#define MAIN_FLOW                                    DEBUG_PRINT_SHIFT (50)
#define EVENT_LOG                                    DEBUG_PRINT_SHIFT (51)
#define PERFORMANCE_ANALYSE                          DEBUG_PRINT_SHIFT (52)

//Ids Masks
#define IDS_TRACE                                    DEBUG_PRINT_SHIFT (53)
#define BVM_TRACE                                    DEBUG_PRINT_SHIFT (54)
#define IDS_TRACE_RSV2                               DEBUG_PRINT_SHIFT (55)
#define IDS_TRACE_RSV3                               DEBUG_PRINT_SHIFT (56)

//S3
#define S3_TRACE                                     DEBUG_PRINT_SHIFT (57)

//Library function to read/write PCI/MSR registers
#define LIB_PCI_RD                                   DEBUG_PRINT_SHIFT (58)
#define LIB_PCI_WR                                   DEBUG_PRINT_SHIFT (59)

//AGESA test points
#define TEST_POINT                                   DEBUG_PRINT_SHIFT (60)

//Reserved for platform log
#define PLAT_RSV1                                    DEBUG_PRINT_SHIFT (61)
#define PLAT_RSV2                                    DEBUG_PRINT_SHIFT (62)
#define PLAT_RSV3                                    DEBUG_PRINT_SHIFT (63)

#ifndef IDSOPT_NDEBUG
  #define IDS_HDT_CONSOLE    IdsLibDebugPrint
#else
  #define IDS_HDT_CONSOLE
#endif

#define IDS_HDT_CONSOLE_PSP_TRACE(...) IDS_HDT_CONSOLE(PSP_TRACE, __VA_ARGS__)

#undef ASSERT_EFI_ERROR
#define ASSERT_EFI_ERROR(x) ASSERT (!EFI_ERROR (x))

VOID
IdsAgesaTestPointVerbosity (
  IN       AGESA_POST_CODE      TestPoint,
  IN       UINT8  Verbosity
  );

VOID
IdsAgesaTestPoint (
  IN      AGESA_POST_CODE TestPoint,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/**
 *  IDS Backend Function for ASSERT
 *
 * Halt execution with stop code display.  Stop Code is displayed on port 80, with rotation so that
 * it is visible on 8, 16, or 32 bit display.  The stop code is alternated with 0xDEAD on the display,
 * to help distinguish the stop code from a post code loop.
 * Additional features may be available if using simulation.
 *
 * @param[in]     FileCode    File code(define in FILECODE.h) mix with assert Line num.
**/
VOID
IdsAssert (
  IN      UINT32 FileCode
  );

/**
 * The engine code for ASSERT MACRO
 *
 * Halt execution with stop code display.  Stop Code is displayed on port 80, with rotation so that
 * it is visible on 8, 16, or 32 bit display.  The stop code is alternated with 0xDEAD on the display,
 * to help distinguish the stop code from a post code loop.
 * Additional features may be available if using simulation.
 *
 * @param[in]     FileCode    File code(define in FILECODE.h) mix with assert Line num.
 *
 */
BOOLEAN
IdsErrorStop (
  IN      UINT32 FileCode
  );

VOID
IdsLibDebugPrint (
  IN      UINT64      Flag,
  IN      CONST CHAR8 *Format,
  IN      ...
  );

/**
 * Dump buffer to HDTOUT
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 * @param[in]     DataWidth       DataWidth 1 - Byte; 2 - Word; 3 - DWORD; 4 - QWORD
 * @param[in]     LineWidth       Number of data item per line
 */
VOID
IdsDumpBuffer (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count,
  IN       UINT8            DataWidth,
  IN       UINT8            LineWidth
  );

/**
 * Dump buffer to HDTOUT, with 16 bytes per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferByte (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  );

/**
 * Dump buffer to HDTOUT, with 16 words per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferWord (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  );

/**
 * Dump buffer to HDTOUT, with 16 Qword per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferQword (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  );

/**
 * Dump buffer to HDTOUT, with 16 Dword per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferDword (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  );


/**
 * Dump buffer to HDTOUT, with 16 bytes per line
 *
 *
 * @param[in]     Flag            HdtOutFilter
 * @param[in]     Buffer          Buffer pointer
 * @param[in]     Count           Count of data elements
 */
VOID
IdsDumpBufferByte (
  IN       UINT64           HdtOutFilter,
  IN       VOID             *Buffer,
  IN       UINT32           Count
  );


#if 0
VOID
AmdIdsDpHdtOutInit (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
AmdIdsDpHdtOutExit (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
AmdIdsDpHdtOutS3Exit (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
AmdIdsDpHdtOutS3ApExit (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
AmdIdsDebugPrint (
  IN       UINT64      Flag,
  IN       CONST CHAR8 *Format,
  IN       ...
  );

VOID
AmdIdsDebugPrintTopology (
  IN      CONST CHAR8 *Format,
  IN      ...
  );

VOID
AmdIdsDebugPrintCpu (
  IN      CONST CHAR8 *Format,
  IN      ...
  );

VOID
AmdIdsDebugPrintMem (
  IN      CONST CHAR8 *Format,
  IN      ...
  );

VOID
AmdIdsDebugPrintGnb (
  IN      CONST CHAR8 *Format,
  IN      ...
  );

VOID
AmdIdsDebugPrintAll (
  IN      CONST CHAR8 *Format,
  IN      ...
  );

VOID
AmdIdsDpHdtOutBufferFlush (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsMemTimeOut (
  IN OUT   VOID *DataPtr
  );

VOID
IdsAgesaTestPoint (
  IN      AGESA_POST_CODE TestPoint,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

/**
 *  IDS Backend Function for ASSERT
 *
 * Halt execution with stop code display.  Stop Code is displayed on port 80, with rotation so that
 * it is visible on 8, 16, or 32 bit display.  The stop code is alternated with 0xDEAD on the display,
 * to help distinguish the stop code from a post code loop.
 * Additional features may be available if using simulation.
 *
 * @param[in]     FileCode    File code(define in FILECODE.h) mix with assert Line num.
**/
VOID
IdsAssert (
  IN      UINT32 FileCode
  );

/**
 * The engine code for ASSERT MACRO
 *
 * Halt execution with stop code display.  Stop Code is displayed on port 80, with rotation so that
 * it is visible on 8, 16, or 32 bit display.  The stop code is alternated with 0xDEAD on the display,
 * to help distinguish the stop code from a post code loop.
 * Additional features may be available if using simulation.
 *
 * @param[in]     FileCode    File code(define in FILECODE.h) mix with assert Line num.
 *
 */
BOOLEAN
IdsErrorStop (
  IN      UINT32 FileCode
  );

VOID
IdsDelay (
  VOID
);

AGESA_STATUS
IdsPerfTimestamp (
  IN       UINT32 LineInFile,
  IN       UINT32 Description,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
IdsPerfAnalyseTimestamp (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

#if IDSOPT_IDS_ENABLED == FALSE
  #undef IEM_SKIP_CODE
  #undef IEM_INSERT_CODE
#endif
#ifndef IEM_SKIP_CODE
  #define IEM_SKIP_CODE(L)
#endif
#ifndef IEM_INSERT_CODE
  #define IEM_INSERT_CODE(L, Fn, Parm)
#endif
#endif
#endif // _IDS_H_


