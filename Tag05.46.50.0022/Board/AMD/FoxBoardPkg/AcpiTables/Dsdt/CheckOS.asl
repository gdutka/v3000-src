/** @file
  Check OS version for MicroSoft OS
;*******************************************************************************
;* Copyright (c) 2013 - 2015, Insyde Software Corporation. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;*******************************************************************************
*/

#ifndef _CHECKOS_ASI_
#define _CHECKOS_ASI_

Method (SCMP, 2)
{
  Name (STG1, Buffer(80){})
  Name (STG2, Buffer(80){})

  Store (Arg0, STG1)
  Store (Arg1, STG2)

  If (LNotEqual(Sizeof(Arg0), Sizeof(Arg1)))
  {
    Return(Zero)
  }

  Store (Zero, Local0)
  While (LLess(Local0, Sizeof(Arg0)))
  {
    If(LNotEqual(Derefof(Index(STG1, Local0)), Derefof(Index(STG2, Local0))))
    {
      Return(Zero)
    }
    Increment(Local0)
  }
  Return(One)
}

Name (WNOS, 0)
Name (MYOS, Zero)
Name (HTTS, Zero)
Name (OSTB,Ones)
Name (TPOS, Zero)
Name (LINX, 0x00)
Name (OSSP, 0x00)

Method (SEQL,2,Serialized)
{
  Store (SizeOf(Arg0),Local0)
  Store (SizeOf(Arg1),Local1)
  If (LNot(LEqual(Local0,Local1)))
  {
    Return (Zero)
  }
  Name (BUF0,Buffer(Local0){})
  Store (Arg0,BUF0)
  Name (BUF1,Buffer(Local0){})
  Store (Arg1,BUF1)
  Store (Zero,Local2)
  While (LLess(Local2,Local0))
  {
    Store (DeRefOf(Index(BUF0,Local2)),Local3)
      Store (DeRefOf(Index(BUF1,Local2)),Local4)
      If (LNot(LEqual(Local3,Local4)))
      {
        Return (Zero)
      }
    Increment (Local2)
  }
  Return (One)
}

Method (OSTP,0,NotSerialized)
{
  If(LEqual(OSTB,Ones))
  {
    If(CondRefOf(\_OSI,Local0))
    {
      CpmDebugPrint ("PLA-ASL-\\OSTP \\_OSI exist\n", 0, 0, 0, 0, 0, 0)
      Store(0x00,OSTB)
      Store(0x00,TPOS)
      If(\_OSI("Windows 2001"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2001", 0, 0, 0, 0, 0)
        Store(0x08,OSTB)
        Store(0x08,TPOS)
      }
      If(\_OSI("Windows 2001.1"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2001.1", 0, 0, 0, 0, 0)
        Store(0x20,OSTB)
        Store(0x20,TPOS)
      }
      If(\_OSI("Windows 2001 SP1"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2001 SP1", 0, 0, 0, 0, 0)
        Store(0x10,OSTB)
        Store(0x10,TPOS)
      }
      If(\_OSI("Windows 2001 SP2"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2001 SP2", 0, 0, 0, 0, 0)
        Store(0x11,OSTB)
        Store(0x11,TPOS)
      }
      If(\_OSI("Windows 2001 SP3"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2001 SP3", 0, 0, 0, 0, 0)
        Store(0x12,OSTB)
        Store(0x12,TPOS)
      }
      If(\_OSI("Windows 2006"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2006", 0, 0, 0, 0, 0)
        Store(0x40,OSTB)
        Store(0x40,TPOS)
      }
      If(\_OSI("Windows 2006 SP1"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2006 SP1", 0, 0, 0, 0, 0)
        Store(0x41,OSTB)
        Store(0x41,TPOS)
        Store(0x01, OSSP)
      }
      If(\_OSI("Windows 2009"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2009", 0, 0, 0, 0, 0)
        Store(1, OSSP)
        Store(0x50, OSTB)
        Store(0x50, TPOS)
      }
      If(\_OSI("Windows 2012"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2012", 0, 0, 0, 0, 0)
        Store(1, OSSP)
        Store(0x60, OSTB)       // OSTYPE_WINDOWS8
        Store(0x60, TPOS)
      }
      If(\_OSI("Windows 2013"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2013", 0, 0, 0, 0, 0)
        Store(1, OSSP)
        Store(0x61, OSTB)       // OSTYPE_WINDOWS8.1
        Store(0x61, TPOS)
      }
      If(\_OSI("Windows 2015"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Windows 2015", 0, 0, 0, 0, 0)
        Store(1, OSSP)
        Store(0x70, OSTB)       // OSTYPE_WINDOWS 10
        Store(0x70, TPOS)
      }
      If(\_OSI("Linux"))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP Call _OSI(%S) = TRUE\n", "Linux", 0, 0, 0, 0, 0)
        Store(0x01,LINX)
        Store(0x80,OSTB)
        Store(0x80,TPOS)
      }
    }
    Else
    {
      CpmDebugPrint ("PLA-ASL-\\OSTP \\_OSI does NOT exist\n", 0, 0, 0, 0, 0, 0)
      If(CondRefOf(\_OS,Local0))
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP \\_OS exist\n", 0, 0, 0, 0, 0, 0)
        If(SEQL(\_OS,"Microsoft Windows"))
        {
          CpmDebugPrint ("PLA-ASL-\\OSTP Call SEQL \\_OS == (%S)\n", "Microsoft Windows", 0, 0, 0, 0, 0)
          Store(0x01,OSTB)
          Store(0x01,TPOS)
        }
        Else
        {
          If(SEQL(\_OS,"Microsoft WindowsME: Millennium Edition"))
          {
            CpmDebugPrint ("PLA-ASL-\\OSTP Call SEQL \\_OS == (%S)\n", "Microsoft WindowsME: Millennium Edition", 0, 0, 0, 0, 0)
            Store(0x02,OSTB)
            Store(0x02,TPOS)
          }
          Else
          {
            If(SEQL(\_OS,"Microsoft Windows NT"))
            {
              CpmDebugPrint ("PLA-ASL-\\OSTP Call SEQL \\_OS == (%S)\n", "Microsoft Windows NT", 0, 0, 0, 0, 0)
              Store(0x04,OSTB)
              Store(0x04,TPOS)
            }
            Else
            {
              CpmDebugPrint ("PLA-ASL-\\OSTP Call SEQL \\_OS != (%S)\n", "Microsoft Windows", 0, 0, 0, 0, 0)
              Store(0x00,OSTB)
              Store(0x00,TPOS)
            }
          }
        }
      }
      Else
      {
        CpmDebugPrint ("PLA-ASL-\\OSTP \\_OS does NOT exist\n", 0, 0, 0, 0, 0, 0)
        Store(0x00,OSTB)
        Store(0x00,TPOS)
      }
    }
  }
  CpmDebugPrint ("PLA-ASL-\\OSTP TPOS = 0x%X, Return OSTB = 0x%X\n", TPOS, OSTB, 0, 0, 0, 0)
  Return(OSTB)
}

#endif
