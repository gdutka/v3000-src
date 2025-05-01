DefinitionBlock (
    "SecureBioCam.aml",                 // Output file
    "SSDT",                     // Signature
    0x01,                       // SSDT Revision
    "AMD",                      // OEM ID
    "SECBIO",                   // OEM Table ID
    0x1                         // OEM Revision
    )
{
    External (\_SB.PCI0.GPP1.DEV0, DeviceObj)

    Scope(\_SB.PCI0.GPP1.DEV0)
    {
      Device(RHUB)
      {
    
        Name(_ADR, 0x00000000)
      
        // SS port 0
        Device(PRT1) {
          Name(_ADR, 0x00000001)
        
          Name(UPC1, Package(4) { 0xFF, 0x03, 0x00000000, 0x00000000 } ) 
          Method (_UPC,0,Serialized)
          {
            Return (UPC1)          
          }
        
          Name(PLD1, Package() {
            Buffer(0x14) {      
            0x82,                   // Revision 2, Ignore color;
            0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
            0x00, 0x00, 0x00, 0x00, // Width & Height;
            0x11, 0x0C, 0x80, 0x00, //User visible, Group Token =0;
                                    // Group Position 1st;
            0x01, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            } 
          })
         
          Method (_PLD,0,Serialized) 
          {
            Return (PLD1)        
          }                               
        }
      
        // SS port 1
        Device(PRT2) {
          Name(_ADR, 0x00000002)
        
          Name(UPC1, Package(4) { 0xFF, 0x03, 0x00000000, 0x00000000 } ) 
          Method (_UPC,0,Serialized)
          {
            Return (UPC1)          
          }
        
          Name(PLD1, Package(1) {// RV1
            Buffer(0x14) {       
            0x82,                   // Revision 2, Ignore color;
            0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
            0x00, 0x00, 0x00, 0x00, // Width & Height;
            0x91, 0x0C, 0x00, 0x01, // User visible, Group Token = 0;
                                    // Group Position 2nd;
            0x01, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            }
          })
          
          Method (_PLD,0,Serialized) 
          {
            Return (PLD1)        
          }                               
        }
      
        //port 3: HS
        Device(PRT3) {
          Name(_ADR, 0x00000003)
        
          Name(UPC1, Package(4) { 0xFF, 0x03, 0x00000000, 0x00000000 } ) 
          Method (_UPC,0,Serialized)
          {
            Return (UPC1)          
          }
        
          Name(PLD1, Package() {
            Buffer(0x14) {      
            0x82,                   // Revision 2, Ignore color;
            0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
            0x00, 0x00, 0x00, 0x00, // Width & Height;
            0x11, 0x0C, 0x80, 0x00, //User visible, Group Token =0;
                                  // Group Position 1st;
            0x01, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            } 
          })
          
          Method (_PLD,0,Serialized) 
          {
            Return (PLD1)        
          }

          Device(CAM0) {
            Name(_ADR, 0x5)      
          }    
        
          Device(CAM1) {
            Name(_ADR, 0x3)      
          }                                                
        }
      
        //port 4: HS
        Device(PRT4) {
          Name(_ADR, 0x00000004)
        
          Name(UPC1, Package(4) { 0xFF, 0x03, 0x00000000, 0x00000000 } ) 
          Method (_UPC,0,Serialized)
          {
            Return (UPC1)          
          }
        
          Name(PLD1, Package(1) {// RV1
            Buffer(0x14) {       
            0x82,                   // Revision 2, Ignore color;
            0x00, 0x00, 0x00,       // 24-bit RGB value for the color of the device;
            0x00, 0x00, 0x00, 0x00, // Width & Height;
            0x91, 0x0C, 0x00, 0x01, // User visible, Group Token = 0;
                                    // Group Position 2nd;
            0x01, 0x00, 0x00, 0x00,
            0xFF, 0xFF, 0xFF, 0xFF  // Reserve
            }
          })
          
          Method (_PLD,0,Serialized) 
          {
            Return (PLD1)        
          } 

          Device(CAM0) {
            Name(_ADR, 0x5)      
          }                                           
        }                           
      }
    }
}

