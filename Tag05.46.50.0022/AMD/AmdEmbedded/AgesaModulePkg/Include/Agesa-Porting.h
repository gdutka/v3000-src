/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#ifndef _AGESA_PORTING_H_
#define _AGESA_PORTING_H_

#pragma warning(push)
#if defined (_MSC_VER)
  #pragma warning(disable: 4103 4001 4324)
  // -----------------------------------------------------------------------
  //   Define a code_seg MACRO
  //
  #define MAKE_AS_A_STRING(arg) #arg

  //#include <intrin.h>  // MS has built-in functions

  #if _MSC_VER < 900
    // -----------------------------------------------------------------------
    //    Assume MSVC 1.52C (16-bit)
    //
    //    NOTE: When using MSVC 1.52C use the following command line:
    //
    //       CL.EXE /G3 /AL /O1i /Fa <FILENAME.C>
    //
    //    This will produce 32-bit code in USE16 segment that is optimized for code
    //    size.
    typedef void          VOID;

    // Create the universal 32, 16, and 8-bit data types
    typedef unsigned long   UINTN;
    typedef          long   INT32;
    typedef unsigned long   UINT32;
    typedef          int    INT16;
    typedef unsigned int    UINT16;
    typedef          char   INT8;
    typedef unsigned char   UINT8;
    typedef          char   CHAR8;
    typedef unsigned short  CHAR16;

    /// struct for 16-bit environment handling of 64-bit value
    typedef struct _UINT64 {
      IN OUT  UINT32 lo;      ///< lower 32-bits of 64-bit value
      IN OUT  UINT32 hi;      ///< highest 32-bits of 64-bit value
    } UINT64;

    // Create the Boolean type
    #define TRUE  1
    #define FALSE 0
    typedef unsigned char BOOLEAN;

    #define CONST const
    #define STATIC static
    #define VOLATILE volatile

    // Force tight packing of structures
    // Note: Entire AGESA (Project / Solution) will be using pragma pack 1
    #pragma warning( disable : 4103 ) // Disable '#pragma pack' in .h warning
//    #pragma pack(1)

    //   Disable WORD->BYTE automatic conversion warnings.  Example:
    //   BYTE LocalByte;
    //   void MyFunc(BYTE val);
    //
    //   MyFunc(LocalByte*2+1); // Warning, automatic conversion
    //
    //   The problem is any time math is performed on a BYTE, it is converted to a
    //   WORD by MSVC 1.52c, and then when it is converted back to a BYTE, a warning
    //   is generated.  Disable warning C4761
    #pragma warning( disable : 4761 )

  #else
    // -----------------------------------------------------------------------
    //   Assume a 32-bit MSVC++
    //
    // Disable the following warnings:
    // 4100 - 'identifier' : unreferenced formal parameter
    // 4276 - 'function' : no prototype provided; assumed no parameters
    // 4214 - non standard extension used : bit field types other than int
    // 4001 - nonstandard extension 'single line comment' was used
    // 4142 - benign redefinition of type for following declaration
    //      - typedef char    INT8
    #if defined (_M_IX86)
      #pragma warning (disable: 4100 4276 4214 4001 4142 4305 4306)

      #ifndef VOID
        typedef void VOID;
      #endif
    // Create the universal 32, 16, and 8-bit data types
      #ifndef UINTN
        typedef unsigned __w64 UINTN;
      #endif
      typedef          __int64 INT64;
      typedef unsigned __int64 UINT64;
      typedef          int   INT32;
      typedef unsigned int   UINT32;
      typedef          short INT16;
      typedef unsigned short UINT16;
      typedef          char  INT8;
      typedef unsigned char  UINT8;
      typedef          char  CHAR8;
      typedef unsigned short CHAR16;

    // Create the Boolean type
      //#ifndef TRUE
      #undef TRUE
      #define TRUE  1
      //#endif
      //#ifndef FALSE
      #undef FALSE
      #define FALSE 0
      //#endif
      typedef unsigned char BOOLEAN;

      // Force tight packing of structures
      // Note: Entire AGESA (Project / Solution) will be using pragma pack 1
//      #pragma pack(1)

      #define CONST const
      #define STATIC static
      #define VOLATILE volatile
      //Support for variadic macros was introduced in Visual C++ 2005.
      #if _MSC_VER >= 1400
        #define VA_ARGS_SUPPORTED
      #endif
      // 64 bit of compiler
    #else
      #pragma warning (disable: 4100 4276 4214 4001 4142 4305 4306 4366)

      #ifndef VOID
        typedef void VOID;
      #endif
      // Create the universal 32, 16, and 8-bit data types
      #ifndef UINTN
        typedef unsigned __int64 UINTN;
      #endif
      typedef          __int64 INT64;
      typedef unsigned __int64 UINT64;
      typedef          int   INT32;
      typedef unsigned int   UINT32;
      typedef          short INT16;
      typedef unsigned short UINT16;
      typedef          char  INT8;
      typedef unsigned char  UINT8;
      typedef          char  CHAR8;
      typedef unsigned short CHAR16;

      // Create the Boolean type
      //#ifndef TRUE
      #undef TRUE
      #define TRUE  1
      //#endif
      //#ifndef FALSE
      #undef FALSE
      #define FALSE 0
      //#endif
      typedef unsigned char BOOLEAN;
      // Force tight packing of structures
      // Note: Entire AGESA (Project / Solution) will be using pragma pack 1
//      #pragma pack(1)

      #define CONST const
      #define STATIC static
      #define VOLATILE volatile
      #define CALLCONV
    #endif
  #endif
  // -----------------------------------------------------------------------
  // End of MS compiler versions


#elif defined __GNUC__

  #define IN
  #define OUT
  #define STATIC static
  #define VOLATILE volatile
  #define TRUE 1
  #define FALSE 0
  #define CONST const
  #define CALLCONV

  typedef unsigned char  BOOLEAN;
  typedef   signed char  INT8;
  typedef   signed short INT16;
  typedef   signed long  INT32;
  typedef          char  CHAR8;
  typedef unsigned char  UINT8;
  typedef unsigned short UINT16;
  typedef unsigned long  UINT32;
  typedef unsigned long  UINTN;
  typedef unsigned long  long UINT64;
  typedef void VOID;
  //typedef unsigned long  size_t;
  typedef unsigned int   uintptr_t;

  //#include <intrin.h>                   // MingW-w64 library header
  #include <MyIntrinsics.h>               // AGESA temp file for GCC until support is added to MinGW

#else
  // -----------------------------------------------------------------------
  // Unknown or unsupported compiler - use for ARM compiler
  //
  #include <MyIntrinsics.h>               // AGESA temp file for ARM GCC
#endif



// -----------------------------------------------------------------------
// Common definitions for all compilers
//

//Support forward reference construct
#define AGESA_FORWARD_DECLARATION(x) typedef struct _##x x

#undef NULL
#define NULL              0

// The following are use in conformance to the UEFI style guide
#define IN
#define OUT

#endif /* _AGESA_PORTING_H_ */
