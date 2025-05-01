/** @file

;******************************************************************************
;* Copyright (c) 2018, Insyde Software Corp. All Rights Reserved.
;*
;* You may not reproduce, distribute, publish, display, perform, modify, adapt,
;* transmit, broadcast, present, recite, release, license or otherwise exploit
;* any part of this publication in any form, by any means, without the prior
;* written permission of Insyde Software Corporation.
;*
;******************************************************************************
*/

Scope (IGPU_SCOPE)
{
    Method (_INI, 0)
    {
        Store (DID1, Index (TLPK, 0x00))
        Store (DID2, Index (TLPK, 0x02))
        Store (DID3, Index (TLPK, 0x04))
        Store (DID4, Index (TLPK, 0x06))
        Store (DID5, Index (TLPK, 0x08))
        Store (DID6, Index (TLPK, 0x0A))
        Store (DID7, Index (TLPK, 0x0C))
        Store (DID2, Index (TLPK, 0x0E))
        Store (DID1, Index (TLPK, 0x0F))
        Store (DID2, Index (TLPK, 0x11))
        Store (DID3, Index (TLPK, 0x12))
        Store (DID2, Index (TLPK, 0x14))
        Store (DID4, Index (TLPK, 0x15))
        Store (DID2, Index (TLPK, 0x17))
        Store (DID5, Index (TLPK, 0x18))
        Store (DID2, Index (TLPK, 0x1A))
        Store (DID6, Index (TLPK, 0x1B))
        Store (DID2, Index (TLPK, 0x1D))
        Store (DID7, Index (TLPK, 0x1E))
    }

    Method (MXMX, 1, Serialized)
    {
        If (LEqual (Arg0, Zero))
        {
            // Acquire DDC/AUX mux
            // No mutex implemented. No need to acquire mutex.
            // Set mux to iGPU
            P8XH (One, 0x77)
            P8XH (Zero, Zero)

            Return (One)
        }
        If (LEqual (Arg0, One))
        {
            // Release DDC/AUX mux
            // No mutex implemented. No need to release mutex.
            // 2-way mux. Hence no need to do anything
            P8XH (One, 0x77)
            P8XH (Zero, one)

            Return (One)
        }
        If (LEqual (Arg0, 0x02))
        {
            //Get ddc/aux mux status for iGPU
            P8XH (One, 0x77)
            P8XH (Zero, 0x02)
        }

        Return (Zero)
    }

    Method (MXDS, 1, Serialized)
    {
        If (LEqual (Arg0, Zero))
        {
            // Get display mux status for iGPU
        }
        If (LEqual (Arg0, One))
        {
            // Set display mux to iGPU
        }
        Return (Zero)
    }

    // Toggle List Package
    Name (TLPK, Package(0x20) {
        Ones, 0x2C,
        Ones, 0x2C,
        Ones, 0x2C,
        Ones, 0x2C,
        Ones, 0x2C,
        Ones, 0x2C,
        Ones, 0x2C,
        Ones, Ones, 0x2C,
        Ones, Ones, 0x2C,
        Ones, Ones, 0x2C,
        Ones, Ones, 0x2C,
        Ones, Ones, 0x2C,
        Ones, Ones, 0x2C
    })
}
