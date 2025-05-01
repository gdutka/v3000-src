    Name(_DSD, Package()
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
            Package (2)
            {
                "mipi-sdca-control-selector-list",
                Package()
                {
                    0x4, 0x5, 0x6, 0x7, 0x8
                }
            }, // Function level controls
			Package (0x02)
			{
				"msft-use-hardcoded-config", 
				One
			},
        },
        ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package ()
        {
            Package (2) {"mipi-sdca-controlselector-0x4-subproperties", "C004"},
            Package (2) {"mipi-sdca-controlselector-0x5-subproperties", "C005"},
            Package (2) {"mipi-sdca-controlselector-0x6-subproperties", "C006"},
            Package (2) {"mipi-sdca-controlselector-0x7-subproperties", "C007"},
            Package (2) {"mipi-sdca-controlselector-0x8-subproperties", "C008"},
        }
    }) // End _DSD

    Name(C004, Package() 
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () 
        {
            // Function_SDCA_Version = 0.6, Class, DC
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-access-mode", 5},
            Package (2) {"mipi-sdca-control-number-dc-value", 0x6},
        }
    }) // End C004

    Name(C005, Package() 
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package ()
        {
            // Function_Topology = 0x6 HID, Class, DC
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-access-mode", 5},
            Package (2) {"mipi-sdca-control-number-dc-value", 0x6},
        }
    }) // End C005

    Name(C006, Package() 
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () 
        {
            // Function_ManufacturerId = 0x025d, Class, DC
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-access-mode", 5},
            Package (2) {"mipi-sdca-control-number-dc-value", 0x025d},
        }
    }) // End C006

    Name(C007, Package() 
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () 
        {
            // Function_ID = 0x0711 (Device specific), Class, DC
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-access-mode", 5},
            Package (2) {"mipi-sdca-control-number-dc-value", 0x0711},
        }
    }) // End C007

    Name(C008, Package() 
    {
        ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () 
        {
            // Function_Version = 0x1, Class, DC
            Package (2) {"mipi-sdca-control-selector-access-layer", 4},
            Package (2) {"mipi-sdca-control-selector-access-mode", 5},
            Package (2) {"mipi-sdca-control-number-dc-value", 0x1},
        }
    }) // End C008
