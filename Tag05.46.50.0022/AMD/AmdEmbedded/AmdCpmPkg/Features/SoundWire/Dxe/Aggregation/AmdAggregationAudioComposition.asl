Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-interface-revision", 0x00000001},    // 0.1 Spec Version Bits 31-16: (Upper word) Major version number. Bits 15-0: (Lower word) Minor version number
       Package (2) {"acpi-acd-endpoint-count", 1},    // 1 Endpoint: Speaker
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},     // Configuration for Aggregated Speaker
    }
}) //End _DSD

Name(EP00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Speaker"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 1},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC00"}, // Configuration for DSP and Aggregated devices
    }
}) //End EP00

Name(EC00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Aggregated_Speaker_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Functions (aggregated) makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC00"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "AG00"},    // Aggregated Speakers
    }
}) //End EC00

Name(CC00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN00"},
    }
}) //End CC00

Name(AC00, Package() {    // This package is shared by all DSP devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", One},    // Is an ACX Factory
       Package (2) {"msft-acx-core-circuit", Zero},    // Not an ACX Core circuit
    }
}) //End AC00

Name(VN00, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_Speaker"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x1},    // Raw Stream Data port number on SPK device
	   Package (2) {"amd-sdw-feedback-stream-dp-number", 0x2},	 // Feedback Stream Data port number on SPK device
	   Package (2) {"amd-sdw-feedback-stream-type", 0x1},	 	 // 0x1:IVSensing 0x2:PCMReference
	   Package (2) {"amd-sdw-feedback-stream-format", 0x1},	
	   Package (2) {"amd-sdw-render-apo-type", 0x19},	 	 // Waves+SEP483
	}
}) //End VN00

Name(AG00, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Speaker_Aggregation"},
       Package (2) {"acpi-acd-collection-type", 0},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 1},    // Parallel Connection
       Package (2) {"acpi-acd-collection-count", 2},    // Two SDCA Audio Functions
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC11"},    // First SDCA AMP Function
       Package (2) {"acpi-acd-collection-1-properties", "CC21"},    // Second SDCA AMP Function
    }
}) //End AG00

Name(AC01, Package() {    // This package is shared by all Codec devices in this composition table
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"msft-acx-factory-circuit", Zero},    // Not an ACX Factory
       Package (2) {"msft-acx-core-circuit", One},    // ACX Core circuit
       Package (2) {"msft-acx-aggregated-circuit", One},
    }
}) //End AC01

Name(CC11, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV0.AF04"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x56},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC01

Name(CC21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV1.AF04"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x56},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0380},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC21
