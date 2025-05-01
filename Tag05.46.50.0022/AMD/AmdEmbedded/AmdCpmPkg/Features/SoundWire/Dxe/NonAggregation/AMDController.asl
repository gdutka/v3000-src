	Name(_DSD, Package() 
	{
		ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
		Package() 
		{
			package() {"mipi-sdw-sw-interface-revision", 0x00010000},  //v1.0
		},
		ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
        	Package () 
		{
			Package () {"mipi-sdw-sw0-master-list", 0x01},
			Package () {"amd-sdw-sw0-enabled", 0x01},
			Package () {"amd-sdw-sw0-power-optimization", 0x02},		//1-Normal, 2-Deep Sleep
			Package () {"mipi-sdw-sw1-master-list", 0x01},
			Package () {"amd-sdw-sw1-enabled", 0x00},
			Package () {"amd-sdw-sw1-power-optimization", 0x02},		//1-Normal, 2-Deep Sleep
        	}
	}) // End _DSD

