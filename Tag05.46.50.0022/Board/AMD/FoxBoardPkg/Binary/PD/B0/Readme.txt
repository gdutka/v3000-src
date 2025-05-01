Legacy Mayan/MayanDAP/Lilac/LilacDAP format
EC SIG             Offset 0x00000000    Size 0x00001000
PD TI 0            Offset 0x00001000    Size 0x00005000  SKU3: A0 silicon, USB3.2/DP-Alt. Supp both PS8828A and PS8830.
PD TI 1            Offset 0x00006000    Size 0x00005000  SKU5: B0 silicon, USB4. Supp PS8830 only.
PD TI 2            Offset 0x0000B000    Size 0x00005000  SKU7: A0/B0 silicon, USB3.2/DP-Alt. GPIOs AUX PN switch.
PD TI 3            Offset 0x00010000    Size 0x00005000  
PD TI 4            Offset 0x00015000    Size 0x00005000 	
PD TI 5            Offset 0x0001A000    Size 0x00006000
PSP SIG            Offset 0x00020000    Size 0x00001000
PD CY 0            Offset 0x00021000    Size 0x00010000  SKU3: A0 silicon, USB3.2/DP-Alt. Supp both PS8828A and PS8830.
PD CY 1            Offset 0x00031000    Size 0x00010000  SKU5: B0 silicon, USB4. Supp PS8830 only.
PD CY 2            Offset 0x00041000    Size 0x00010000  SKU7: A0/B0 silicon, USB3.2/DP-Alt. GPIOs AUX PN switch.
PD CY 3            Offset 0x00051000    Size 0x00010000
PD CY 4            Offset 0x00061000    Size 0x00010000
PD CY 5            Offset 0x00071000    Size 0x00010000
EC2 ROM            Offset 0x00081000    Size 0x00040000
BIOS others        Offset 0x000C1000    ....


Mayan/MayanDAP new format
EC SIG             Offset 0x00000000    Size 0x00001000
PD TI 0            Offset 0x00001000    Size 0x00008000  SKU3: A0 silicon,    TPS6599xAD, USB3.2/DP-Alt, Supp PS8828A/PS8830, Polling mode.
PD TI 1            Offset 0x00009000    Size 0x00008000  SKU1: A0 silicon,    TPS6599xAD, USB3.2/DP-Alt, Supp KB8002,         Polling mode.
PD TI 2            Offset 0x00011000    Size 0x00008000  SKU7: A0/B0 silicon, TPS6599xAD, USB3.2/DP-Alt, GPIOs AUX PN switch, Polling/Interrupt mode.
hole               Offset 0x00019000    Size 0x00007000  Hole
PSP SIG            Offset 0x00020000    Size 0x00001000  PSP
PD TI 3            Offset 0x00021000    Size 0x00008000  SKU2: B0 silicon,    TPS6599xBF, USB4,          Supp KB8002,         Polling/Interrupt mode.
PD TI 4            Offset 0x00029000    Size 0x00008000  SKU4: B0 silicon,    TPS6599xBF, USB4,          Supp PS8828A/PS8830, Polling/Interrupt mode. 	
PD TI 5            Offset 0x00031000    Size 0x00008000  SKU5: A0/B0 silicon, TPS6599xBF, USB3.2/DP-Alt, GPIOs AUX PN switch, Polling/Interrupt mode.
PD TI 6            Offset 0x00039000    Size 0x00008000  SKU6: A0 silicon,    TPS6599xBF, USB3.2/DP-Alt, Supp KB8002,         Polling mode.
PD TI 7            Offset 0x00041000    Size 0x00008000  Empty
PD TI 8            Offset 0x00049000    Size 0x00008000  Empty	
PD TI 9            Offset 0x00051000    Size 0x00008000  Empty	
PD TI 10           Offset 0x00059000    Size 0x00008000  Empty	
PD TI 11           Offset 0x00061000    Size 0x00008000  Empty	
PD TI 12           Offset 0x00069000    Size 0x00008000  NA	
PD TI 13           Offset 0x00071000    Size 0x00008000	 NA
PD TI 14           Offset 0x00079000    Size 0x00008000	 NA	
EC2 ROM            Offset 0x00081000    Size 0x00040000
BIOS others        Offset 0x000C1000    ....

	
Lilac/LilacDAP new format
EC SIG             Offset 0x00000000    Size 0x00001000
PD CY 1            Offset 0x00001000    Size 0x00010000  SKU3: A0 silicon,    CCGDF, USB3.2/DP-Alt. Supp PS8828A/PS8830, Polling mode.
PD CY 2            Offset 0x00011000    Size 0x0000F000  SKU1: A0 silicon,    CCGDF, USB3.2/DP-Alt, Supp KB8002,         Polling mode.
PSP SIG            Offset 0x00020000    Size 0x00001000  PSP
PD CY 3            Offset 0x00021000    Size 0x00010000  SKU7: A0/B0 silicon, CCGSF, USB3.2/DP-Alt. GPIOs AUX PN switch, Polling/Interrupt mode.
PD CY 4            Offset 0x00031000    Size 0x00010000  SKU2: B0 silicon,    CCGDF, USB4,          Supp KB8002,         Polling/Interrupt mode. 	
PD CY 5            Offset 0x00041000    Size 0x00010000  SKU4: B0 silicon,    CCGDF, USB4,          Supp PS8828A/PS8830, Polling/Interrupt mode.
PD CY 6            Offset 0x00051000    Size 0x00010000  Empty
PD CY 7            Offset 0x00061000    Size 0x00010000  Empty
PD CY 8            Offset 0x00071000    Size 0x00010000  Empty	
EC2 ROM            Offset 0x00081000    Size 0x00040000
BIOS others        Offset 0x000C1000    ....