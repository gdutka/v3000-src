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

/*
    == Readme ==
    For NVOP support, SBIOS has to adjust below code to meet platform:
    - Modify the method path of \_SB.PCI0.PGON and \_SB.PCI0.PGOF to Intel RC dGPU ON/OFF method
*/

Scope (DGPU_SCOPE)
{
    Method (NVOP, 4, Serialized)
    {
        If (LEqual (Arg2, Zero))
        {
            Return (Buffer (0x04)
            {
                0x01, 0x00, 0x00, 0x04
            })
        }
        Else
        {
            If (LEqual (Arg2, 0x1A))
            {
                CreateField (Arg3, 0x18, 0x02, OMPR)
                CreateField (Arg3, Zero, One, FLCH)
                CreateField (Arg3, One, One, DVSR)
                CreateField (Arg3, 0x02, One, DVSC)
                If (ToInteger (FLCH))
                {
                    Store (OMPR, \_SB.PCI0.GPP0.PEGP.OPCE)
                }
                Store (Buffer (0x04)
                {
                    0x00, 0x00, 0x00, 0x00
                }, Local0)
                CreateField (Local0, Zero, One, OPEN)
                CreateField (Local0, 0x03, 0x02, CGCS)
                CreateField (Local0, 0x06, One, SHPC)
                CreateField (Local0, 0x08, One, SNSR)
                CreateField (Local0, 0x18, 0x03, DGPC)
                CreateField (Local0, 0x1B, 0x02, HDAC)
                Store (One, OPEN)
                Store (One, SHPC)
                Store (0x03, HDAC)
                Store (One, DGPC)
                If (ToInteger (DVSC))
                {
                    If (ToInteger (DVSR))
                    {
                        Store (One, \_SB.PCI0.GPP0.PEGP.GPRF)
                    }
                    Else
                    {
                        Store (Zero, \_SB.PCI0.GPP0.PEGP.GPRF)
                    }
                }
                Store (\_SB.PCI0.GPP0.PEGP.GPRF, SNSR)
                If (LNotEqual (\_SB.PCI0.GPP0.PEGP.SGST(), Zero))
                {
                    Store (0x03, CGCS)
                }
                Return (Local0)
            }
        }
        Return (0x80000002)
    }
}