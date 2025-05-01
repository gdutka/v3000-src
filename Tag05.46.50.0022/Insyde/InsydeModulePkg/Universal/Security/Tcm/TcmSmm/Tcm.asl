/** @file
  The TCM definition block in ACPI table for MemoryClear.

;******************************************************************************
;* Copyright (c) 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

DefinitionBlock (
  "Tcm.aml",
  "SSDT",
  2,
  "Insyde",
  "TcmTable",
  0x1000
  )
{
  Scope (\_SB)
  {
    Device (TPM) // Naming TCM to TPM in order to adopt Method (PTS) for MOR feature
    {
      Name (_HID, EISAID ("ZIC0201"))

      //
      // Readable name of this device, don't know if this way is correct yet
      //
      Name (_STR, Unicode ("TCM Device"))

      //
      // Return the resource consumed by TCM device
      //
      Name (_CRS, ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0xfed40000, 0x5000)
      })

      //
      // Operational region for Smi port access
      //
      OperationRegion (SMIP, SystemIO, FixedPcdGet16 (PcdSoftwareSmiPort), 1)
      Field (SMIP, ByteAcc, NoLock, Preserve)
      {
          IOB2, 8
      }

      //
      // Operational region for TCM access
      //
      OperationRegion (TPMR, SystemMemory, 0xfed40000, 0x5000)
      Field (TPMR, AnyAcc, NoLock, Preserve)
      {
        ACC0, 8,
      }

      //
      // Operational region for TCM support
      // Region Offset 0xFFFF0000 and Length 0xF0 will be fixed in C code.
      //
      OperationRegion (TNVS, SystemMemory, 0xFFFF0000, 0xF0)
      Field (TNVS, AnyAcc, NoLock, Preserve)
      {
        PPIN,   8,  //   Software SMI for Physical Presence Interface
        PPIP,   32, //   Used for save physical presence paramter
        PPRP,   32, //   Physical Presence request operation response
        PPRQ,   32, //   Physical Presence request operation
        LPPR,   32, //   Last Physical Presence request operation
        FRET,   32, //   Physical Presence function return code
        MCIN,   8,  //   Software SMI for Memory Clear Interface
        MCIP,   32, //   Used for save the Mor paramter
        MORD,   32, //   Memory Overwrite Request Data
        MRET,   32, //   Memory Overwrite function return code
        UCRQ,   32  //   Phyical Presence request operation to Get User Confirmation Status
      }

      Method (PTS, 1, Serialized)
      {
        //
        // Detect Sx state for MOR, only S4, S5 need to handle
        //
        If (LAnd (LLess (Arg0, 6), LGreater (Arg0, 3)))
        {
          //
          // Bit4 -- DisableAutoDetect. 0 -- Firmware MAY autodetect.
          //
          If (LNot (And (MORD, 0x10)))
          {
            //
            // Trigger the SMI through ACPI _PTS method.
            //
            Store (0x02, MCIP)

            //
            // Trigger the SMI interrupt
            //
            Store (MCIN, IOB2)
          }
        }
        Return (0)
      }

      Method (_STA, 0)
      {
        if (LEqual (ACC0, 0xff))
        {
            Return (0)
        }
        Return (0x0f)
      }
    }
  }
}
