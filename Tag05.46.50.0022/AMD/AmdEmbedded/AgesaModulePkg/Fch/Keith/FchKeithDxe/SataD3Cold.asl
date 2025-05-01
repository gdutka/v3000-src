//SATA D3 Cold asl
DefinitionBlock (
    "SataD3Cold.aml",        // Output file
    "SSDT",                  // Signature
    0x01,                    // SSDT Revision
    "AMD",                   // OEM ID
    "STD3",                  // OEM Table ID
    0x1                      // OEM Revision
    )
{
  External(\_SB.PCI0.RTXX.DEV0, DeviceObj)

  OperationRegion(PM00, SystemMemory, 0xFED80300, 0x100)
  Field(PM00, AnyAcc, NoLock, Preserve)
  {
    Offset (0x6A),
    SWSP, 8,
  }

  OperationRegion(SWRG, SystemIo, SWSP, 0x02)
  Field(SWRG, AnyAcc, NoLock, Preserve) {
    SWCP, 8,  //Command Port
    SWDP, 8,  //Data Port
  }

  Scope(\_SB)
  {
    Name(STDS, 0) // SATA D3Cold Support Status
    Name(STDD, 0) // SATA Dummy D3Cold Support Status
    Name(SCPD, 0xFF) // SATA D3Cold Entry SMI number
    Name(SCPU, 0xFF) // SATA D3Cold Exit SMI number
  }

  Scope(\_SB.PCI0.RTXX.DEV0) {
    Name(D0S0, 1)
    Name(D3S0, 1)

    PowerResource(P0S0, 0, 0) {
      Method(_STA) {
        Return(D0S0)
      }
      Method(_ON) {
        Store(0x01, D0S0)
      }
      Method(_OFF) {
        Store(0x00, D0S0)
      }
    } // End of PowerResource(P0S0)

    PowerResource(P3S0, 0, 0) {
      Method(_STA) {
        Return(D3S0)
      }
      Method(_ON) {
        Store(0x01, D3S0)
      }
      Method(_OFF) {
        Store(0x00, D3S0)
      }
    } // End of PowerResource(P3S0)

    // Indicate support for D0.
    Name(_PR0, Package() {P0S0})          // Power resources required for D0

    // Indicate support for D3Cold.
    Name(_PR3, Package() {P3S0})          // Power resource for D3; These will be turned off ONLY if drivers

    Name (_DSD, Package () {
      ToUUID("5025030F-842F-4AB4-A561-99A5189762D0"),
      Package () {
        Package (2) {"StorageD3Enable", 1}, // 1 - Enable; 0 - Disable
      }
    }) //_DSD end

    Method(_S0W, 0) {
      if (LEqual(STDS, one)) {
        Return(4)
      } else {
        Return(0)
      }
    }

    Method(_PS0, 0) {
      if (LEqual(STDS, one)) {
        if (LEqual(STDD, zero)) {
          Store(0x00, SWDP)
          Store(SCPU, SWCP)
        }
      }
    }

    Method(_PS3, 0) {
      if (LEqual(STDS, one)) {
        if (LEqual(STDD, zero)) {
          Store(0x00, SWDP)
          Store(SCPD, SWCP)
        }
      }
    }
  } // End of Scope(\_SB.PCI0.RTXX.DEV0)
}
