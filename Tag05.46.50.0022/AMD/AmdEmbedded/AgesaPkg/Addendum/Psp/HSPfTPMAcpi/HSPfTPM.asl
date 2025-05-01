/*****************************************************************************
 *
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */

DefinitionBlock (
  "Tpm.aml",   // Output Filename
  "SSDT",      // Signature
  0x01,        // DSDT Compliance Revision
  "AMD",       // OEMID
  "TPMACPI",   // TABLE ID
  0x1000       // OEM Revision
  )
{
  //The TPM 2.0 Device Object MUST be located under the system bus at "root\_SB".
  Scope (\_SB)
  {
    //Debug purpose
    OperationRegion (DBG1, SystemIO, 0x80, 0x2)
    Field (DBG1, WordAcc, NoLock, Preserve)
    {
        P80H, 16
    }
    Device (TPM2)
    {
      //The actual plug and play hardware identifier (e.g. _HID) for the TPM 2.0
      // device object MUST be MSFT0101 or the device MUST have a compatible ID of
      //MSFT0101 and the _CID could be vendor specific.
      Name (_HID, "MSFT0101")
      Name (_CID, "MSFT0101")
      Name (_STR, Unicode ("TPM 2.0 Device"))

      Method (_STA, 0)
      {
        Return (0x0F)
      }

      Name (_CRS, ResourceTemplate () {
        //TPM2.0 Buffer allocated by BIOS (TPM_BUFFERS), should be updated during POST
        Memory32Fixed (ReadWrite, 0xBBBBBBBB, 0x100000)
      })
    }
  }
}
