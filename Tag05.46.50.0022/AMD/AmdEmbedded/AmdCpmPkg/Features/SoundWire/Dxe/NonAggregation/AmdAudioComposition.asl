Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-interface-revision", 0x00000001},    // 0.1 Spec Version Bits 31-16: (Upper word) Major version number. Bits 15-0: (Lower word) Minor version number
       Package (2) {"acpi-acd-endpoint-count", 8},    // 8 Endpoints: Speaker, MicrophoneArray, Headphones, LineOut, HeadsetOutput, Microphone, LineIn, HeadsetMic
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},     // Configuration for Speaker
       Package (2) {"acpi-acd-endpoint-1-properties", "EP01"},     // Configuration for MicrophoneArray 
       Package (2) {"acpi-acd-endpoint-2-properties", "EP02"},     // Configuration for Headphones
       Package (2) {"acpi-acd-endpoint-3-properties", "EP03"},     // Configuration for LineOut 
       Package (2) {"acpi-acd-endpoint-4-properties", "EP04"},     // Configuration for HeadsetOutput
       Package (2) {"acpi-acd-endpoint-5-properties", "EP05"},     // Configuration for Microphone
       Package (2) {"acpi-acd-endpoint-6-properties", "EP06"},     // Configuration for LineIn 
       Package (2) {"acpi-acd-endpoint-7-properties", "EP07"},     // Configuration for HeadsetMic
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
           Package (2) {"acpi-acd-endpoint-config-0-properties", "EC00"},
        }
   }) //End EP00

   Name(EC00, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
        Package () {
           Package (2) {"acpi-acd-config-priority", 1},
           Package (2) {"acpi-acd-config-friendly-name", "Speaker_With_DSP"},
           Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
           Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
           Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
        Package () {
           Package (2) {"acpi-acd-collection-0-properties", "CC00"},    // DSP Configuration
           Package (2) {"acpi-acd-collection-1-properties", "CC01"},    // Sdca Audio Function Configuration
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
	}
}) //End VN00

   Name(CC01, Package() {
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
   }) //End CC01

   Name(AC01, Package() {    // This package is shared by all Codec devices in this composition table
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
        Package () {
           Package (2) {"msft-acx-factory-circuit", Zero},    // Not an ACX Factory
           Package (2) {"msft-acx-core-circuit", One},    // ACX Core circuit
        }
   }) //End AC01

   Name(EP01, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
        Package () {
           Package (2) {"acpi-acd-endpoint-friendly-name", "MicrophoneArray"},
           Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
           Package (2) {"acpi-acd-endpoint-id", 2},    // Endpoint id
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
        Package () {
           Package (2) {"acpi-acd-endpoint-config-0-properties", "EC10"},
        }
   }) //End EP01

   Name(EC10, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
        Package () {
           Package (2) {"acpi-acd-config-priority", 1},
           Package (2) {"acpi-acd-config-friendly-name", "MicrophoneArray_With_DSP"},
           Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
           Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
           Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
        Package () {
           Package (2) {"acpi-acd-collection-0-properties", "CC10"},    // DSP Configuration
           Package (2) {"acpi-acd-collection-1-properties", "CC11"},    // Sdca Audio Function Configuration
        }
   }) //End EC10

   Name(CC10, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
        Package () {
           Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
           Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
        Package () {
           Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
           Package (2) {"acpi-acd-vendor-collection-properties", "VN01"},
        }
   }) //End CC10

Name(VN01, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_MicrophoneArray"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x6},    // Raw Stream Data port number on Mic device
	   Package (2) {"amd-sdw-vad-stream-dp-number", 0x4},	 	 // VAD Stream Data port number on Mic device
	}
}) //End VN01

   Name(CC11, Package() {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
        Package () {
           Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV3.AF02"},
           Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
           Package (2) {"acpi-acd-sdca-terminal-id", 0x0f},    // Entity id of the Analog terminal used for this endpoint
           Package (2) {"acpi-acd-sdca-terminal-type", 0x0201},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
        Package () {
           Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
        }
   }) //End CC11
Name(EP02, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Headphones"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 3},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC20"},
    }
}) //End EP02

Name(EC20, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Headphones_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC20"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC21"},    // Sdca Audio Function Configuration
    }
}) //End EC20

Name(CC20, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN02"},
    }
}) //End CC20

Name(VN02, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-config-type", "Streaming_Headphones"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x3},    // Raw Stream Data port number on Headphone device
	}
}) //End VN02

Name(CC21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV2.AF01"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x6},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06c0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC21

Name(EP03, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "LineOut"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 3},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC30"},
    }
}) //End EP03

Name(EC30, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "LineOut_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC30"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC31"},    // Sdca Audio Function Configuration
    }
}) //End EC30

Name(CC30, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN03"},
    }
}) //End CC30

Name(VN03, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_LineOut"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x3},    // Raw Stream Data port number on LineOut device
	}
}) //End VN03

Name(CC31, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV2.AF01"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x7},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0690},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC31

Name(EP04, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "HeadsetOutput"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 3},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC40"},
    }
}) //End EP04

Name(EC40, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "HeadsetOutput_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC40"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC41"},    // Sdca Audio Function Configuration
    }
}) //End EC40

Name(CC40, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN04"},
    }
}) //End CC40

Name(VN04, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_HeadsetOutput"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x3},    // Raw Stream Data port number on Headset device
	}
}) //End VN04

Name(CC41, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV2.AF01"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x48},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC41

Name(EP05, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "Microphone"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 4},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC50"},
    }
}) //End EP05

Name(EC50, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "Microphone_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC50"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC51"},    // Sdca Audio Function Configuration
    }
}) //End EC50

Name(CC50, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN05"},
    }
}) //End CC50

Name(VN05, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_Microphone"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x2},    // Raw Stream Data port number on Microphone device
	}
}) //End VN05

Name(CC51, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV2.AF01"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x8},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06a0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC51

Name(EP06, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "LineIn"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 4},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC60"},
    }
}) //End EP06

Name(EC60, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "LineIn_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC60"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC61"},    // Sdca Audio Function Configuration
    }
}) //End EC60

Name(CC60, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN06"},
    }
}) //End CC60

Name(VN06, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_LineIn"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x2},    // Raw Stream Data port number on LineIn device
	}
}) //End VN06

Name(CC61, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV2.AF01"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x9},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0680},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC61

Name(EP07, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-friendly-name", "HeadsetMic"},
       Package (2) {"acpi-acd-endpoint-config-count", 1},    // Only one config, No alternate config
       Package (2) {"acpi-acd-endpoint-id", 4},    // Endpoint id
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-config-0-properties", "EC70"},
    }
}) //End EP07

Name(EC70, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-config-priority", 1},
       Package (2) {"acpi-acd-config-friendly-name", "HeadsetMic_With_DSP"},
       Package (2) {"acpi-acd-collection-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-collection-ordering", 0},    // Serial Connection
       Package (2) {"acpi-acd-collection-count", 2},    // DSP + Sdca Audio Function makes the endpoint
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-collection-0-properties", "CC70"},    // DSP Configuration
       Package (2) {"acpi-acd-collection-1-properties", "CC71"},    // Sdca Audio Function Configuration
    }
}) //End EC70

Name(CC70, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PC00.HDAS"},
       Package (2) {"acpi-acd-device-type", 0},    // 0: Generic, 1: SoundWire
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC00"},    // Acx specific properties
       Package (2) {"acpi-acd-vendor-collection-properties", "VN07"},
    }
}) //End CC70

Name(VN07, Package() {    // Passed in as an AcxObjectBag during circuit creation. Contents of this package are proprietary.
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-vendor-id", 0x1},
       Package (2) {"acpi-vendor-config-type", "Streaming_HeadsetMic"},
    },
	ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
	Package () {
	   Package (2) {"amd-sdw-default-stream-dp-number", 0x2},    // Raw Stream Data port number on HeadsetMic device
	}
}) //End VN07

Name(CC71, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", "\\_SB.PCI0.GP17.ACP.SDWC.SLV2.AF01"},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x43},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC01"},    // Acx specific properties
    }
}) //End CC71
