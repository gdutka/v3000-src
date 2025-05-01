/** @file

;******************************************************************************
;* Copyright (c) 2018 - 2019, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

Scope (DGPU_SCOPE)
{
//    OperationRegion (PCAP, PCI_Config, EECP, 0x14)
//    Field (PCAP, DWordAcc, NoLock, Preserve)
//    {
//        Offset (0xC), // Offset (12),
//            ,   4,
//        EMLW,   6,
//        Offset (0x10), // Offset (16),
//        LCTL,   16
//    }

    //
    // dGPU just output to HDMI in Optimus HW artchetecture
    //
    Method (_DOD, 0, NotSerialized)
    {
        Return (Package(0x01) {ACPI_ID_HDMI})
    }

    Method (MXMX, 1, Serialized)
    {
        If (LEqual (Arg0, Zero))
        {
            // Acquire DDC/AUX mux
            // No mutex implemented. No need to acquire mutex.
            // Set mux to dGPU
            P8XH (One, 0x99)
            P8XH (Zero, Zero)

            Return (One)
        }
        If (LEqual (Arg0, One))
        {
            // Release DDC/AUX mux
            // No mutex implemented. No need to release mutex.
            // 2-way mux. Hence no need to do anything
            P8XH (One, 0x99)
            P8XH (Zero, one)

            Return (One)
        }
        If (LEqual (Arg0, 0x02))
        {
            P8XH (One, 0x99)
            P8XH (Zero, 0x02)

            // Get ddc/aux mux status for dGPU
        }

        Return (Zero)
    }

    Method (MXDS, 1, Serialized)
    {
        If (LEqual (Arg0, Zero))
        {
            // Get display mux status for dGPU
        }
        If (LEqual (Arg0, One))
        {
            // Set display mux to dGPU
        }
        Return (Zero)
    }

    Method (MXM, 4, Serialized)
    {
        Switch (ToInteger (Arg2))
        {
            //
            // Function 0: MXM_FUNC_MXSS
            //
            case (MXM_FUNC_MXSS)
            {
                // Sub-Functions 0,16,24 are supported
                Return (ToBuffer (0x01010001))
            }

            //
            // Function 16: MXM_FUNC_MXMS
            //
            case (MXM_FUNC_MXMS)
            {
                If (LEqual (Arg1, 0x300))
                {
                    If (LNotEqual (MXBS, 0))
                    {
                        Name (MXM3, Buffer(MXBS) {0x00})
                        Store (MXMB, MXM3)
                        Return (MXM3)
                    }
                }
            }

            //
            // Function 24: MXM_FUNC_MXMI
            //
            case (MXM_FUNC_MXMI)
            {
                Return (ToBuffer (0x30))
            }
        }
    }
}