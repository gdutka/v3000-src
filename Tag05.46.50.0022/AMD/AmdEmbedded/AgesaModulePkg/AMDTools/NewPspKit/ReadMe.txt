Before use BuildPspImage.bat, please:
1. Have python 3.x installed.
2. Set system variable "USE_PYTHON_SCRIPT" to TRUE
3. Set  system variable "PYTHON_PATH" as the python install path (e.g. C:\Python27)

Notes:
1. This tool will extract POINT entries with command "dp", but those POINT entries will not be combined to BIOS binary with command "bb". Those entries are only for reference.

Version info:

#Version 4.0.50  05/10/2022
  # Avoid some FW cross 1MB boundary

#Version 4.0.4F  04/21/2022
  # Update DF Rib Version

#Version 4.0.4E  03/17/2022
  # PHX2 support
  # Add a work around for ComboAM4v2

#Version 4.0.4D  03/08/2022
  # Update parse PSP 0x5F entry method
  # Update entry info

#Version 4.0.4C  02/08/2022
  # Support SubProgram and Instance for PSP entry

#Version 4.0.4B  01/13/2022
  # Show directory size if size is 0

#Version 4.0.4A  12/20/2021
  # Support Strixpoint

#Version 4.0.49  12/10/2021
  # Fix Combo Directory bug

#Version 4.0.48  9/13/2021
  # Update PSP entry
  # Add MDN Support

#Version 4.0.47  7/16/2021
  # Fix non-ASCII character issue
  # Update DXIO FW(type 0x42) version

#Version 4.0.46  5/27/2021
  # Fix AddressMode bit for AddressMode 3
  # Change BIOS entry 0x68 to not writable
  # Fix BIOS directory header reserved size

#Version 4.0.45  4/30/2021
  # Update some POINT ENTRY to IMAGE ENTRY
  # Output more info in PspDirInfo
  # Add new entry type definition

#Version 4.0.44  2/23/2021
  # Update the format of Psp Version txt

#Version 4.0.43  1/11/2021
  # Phoenix Support

#Version 4.0.42  1/11/2021
  # ComboAM5 Support

#Version 4.0.41  12/25/2020
  # Ignore multiple 4A entry
  # Update PSP entry type

#Version 4.0.40  11/17/2020
  # Add Stones Support

#Version 4.0.3F  10/20/2020
  # Update GN PSP ID

#Version 4.0.3E  10/13/2020
  # Parse entry 2A version

#Version 4.0.3D  9/22/2020
  # Sign L2A and L2B respectively

#Version 4.0.3C  9/10/2020
  # Support ISH Header Generation in xml

#Version 4.0.3B  8/31/2020
  # RMB ISH Structure Support

#Version 4.0.3A  8/21/2020
  # Output Psp Version txt

#Version 4.0.39  8/18/2020
  # Update 0x6D entry
  # Set default log level to INFO
  # Update info message

#Version 4.0.38  8/11/2020
  # Include Modifiable bit in entry header
  # Add a new entry type

#Version 4.0.37  8/10/2020
  # Fix StrToNum float number issue

#Version 4.0.36  8/7/2020
  # Update Error Display
  # StrToNum supports basic calculation 

#Version 4.0.35  7/15/2020
  # Update Entry Description

#Version 4.0.34  6/23/2020
  # Update Entry Description

#Version 4.0.33  6/08/2020
  # Support GN EFS Update

#Version 4.0.32  6/04/2020
  # Fix PSB signing issue

#Version 4.0.31  6/02/2020
  # Add versions of firmware components to output when inserting
  #  Firmware in a BIOS image

#Version 4.0.30  6/02/2020
  # Fix a couple of small bugs
  #  String formatting value in GetOutEntryFileBaseName
  #  missing parameter to log.error(...)
  #  ";" at end of line

#Version 4.0.2F  5/28/2020
  # SMM Super Visor support

#Version 4.0.2E  5/14/2020
  # Fix Zlib header size bug

#Version 4.0.2D  5/12/2020
  # Error Exit if Image Entry file size exceeds its size limit

#Version 4.0.2C  4/29/2020
  # Fix extract compressed binary bug

#Version 4.0.2B  4/20/2020
  # ComboFP6 support

#Version 4.0.2A  4/10/2020
  # 64K SpiBlockSize support

#Version 4.0.29  4/02/2020
  # Update CZN PSP ID mask
  # Update entry descriptions

#Version 4.0.28  3/20/2020
  # Skip L2B if L2A and L2B shares the same offset

#Version 4.0.27  3/12/2020
  # Make BuildPspDirectory much less verbose in normal operation mode.  Print
  # only only one table at the end of the run.  Add verbosity parameter to arg
  # parsing.

#Version 4.0.26  3/5/2020
  # Update Entry description

#Version 4.0.25  2/28/2020
  # CZN support

#Version 4.0.24  2/26/2020
  # Change 'L' to 'I' in struct.unpack for RHEL7 support

#Version 4.0.23  2/14/2020
  # Support multiple slot

#Version 4.0.22  1/20/2020
  # Fix a bug of last submit

#Version 4.0.21  1/15/2020
  # Update copyright
  # Report error if entry 0x0 is not the first entry in PSP directory level 1

#Version 4.0.20  1/8/2020
  # Report error if directory size exceeds 4MB
  # Report error for address mode 0 in 32MB BIOS

#Version 4.0.1F  1/2/2020
  # Add a new entry

#Version 4.0.1E  12/24/2019
  # Seperate PSP/BIOS entry type
  # Update entry description
  # Update program name

#Version 4.0.1D  12/20/2019
  # Support VN 2 level PSP structure

#Version 4.0.1C  12/05/2019
  # Support Python 2 and Python 3 in one script

#Version 4.0.1B  11/26/2019
  #Support AddressMode 3
  #Add several new types

#Version 4.0.1A  10/17/2019
  #Change SAA PSP Id for combo support
  #Add SAB PSP Id

#Version 4.0.19  09/27/2019
  #Support AddressMode for point entry in Dirs with AddressMode = 2
  #Change VM to VMR
  #Change VG to VN

#Version 4.0.18  09/17/2019
  #Support VM program
  #Change MR to VG

#Version 4.0.17  09/05/2019
  #Support MR program
  #Currently only parse the first L2A directory for MR program

#Version 4.0.16  08/20/2019
  #Add several new entry types
  #Add Vermeer subprogram info

#Version 4.0.15  07/26/2019
  #Check A/B recovery before trying to find BIOS Dir in Combo Header

#Version 4.0.14  06/26/2019
  #Show used size in error log when used size exceeds directory limit

#Version 4.0.13  06/04/2019
  #Update ABL version string parse method

#Version 4.0.12  05/06/2019
  #Fix a potential bug for extracted filename

#Version 4.0.11  04/30/2019
  #Output Point Entry Binary

#Version 4.0.10  04/24/2019
  #Check Type conflict

#Version 4.0.0F  04/19/2019
  #Update Entry Type Definitions
  #Get version at offset 0x60 by default

#Version 4.0.0E  04/12/2019
  #Add Copy Attribute for A/B recovery

#Version 4.0.0D  04/09/2019
  #Add PCO string to subprogram type description

#Version 4.0.0C  04/08/2019
  #Change AddressMode not match from Error to Warning

#Version 4.0.0B  04/01/2019
  #A/B recovery support
  #AddressMode for each directory
  #AddressMode may differ from each entry

#Version 4.0.0A  02/25/2019
  #Update BIOS Combo Dir Signature to 2BHD

#Version 4.0.09  02/20/2019
  #Update 0x6 entry definition

#Version 4.0.08  02/14/2019
  #Support new BIOS combo directory header

#Version 4.0.07  02/14/2019
  #Update SMU version

#Version 4.0.06  02/12/2019
  #Initial support for Renoir

#Version 4.0.05  02/02/2019
  #Add new Psp/BIOS Entries

#Version 4.0.04  01/28/2019
  #Update Bixby entry types

#Version 4.0.03  01/16/2019
  #Add DxioFw version info

#Version 4.0.02  01/16/2019
  #Merge 3.1.1C~3.1.1F python2 changes. Will no longer update python2 script.
  #Add Bixby entry types

#Version 4.0.01  10/10/2018
  #Update to support python 3.0 only, verified with python 3.7

#Version 3.1.1F  01/15/2019
  #Remove unnecessary files
  #Support CMN_DIR

#Version 3.1.1E  01/10/2019
  #Fix read PSP recovery issue in some BIOS
  #Add two new PSP entry types: 0x42, 0x43

#Version 3.1.1D  09/27/2018
  #Update to offset in EFS and PSP Directory

#Version 3.1.1C  09/26/2018
  #Fix instance disappeared when tool generate PspDirectory.xml for multiple instance Point entry

#Version 3.1.1B  09/05/2018
  #Add support EFS address 0x120000

#Version 3.1.1A  08/17/2018
  #Fix "dp" command doesn't support BIOS rom with subprogram in PSP DIR type, which bring in by version 3.1.19

#Version 3.1.19  07/25/2018
  #Add support for RomId

#Version 3.1.18  06/26/2018
  #Add dump function for MTS and RV2

#Version 3.1.17  05/22/2018
  #Fix SMU version display issue on SSP

#Version 3.1.16  05/21/2018
  #Fix output nonexistent HeaderBase in SSP 2Lv PSP Dir configuration.

#Version 3.1.15  04/08/2018
  # Support Devel SMU version display

#Version 3.1.14  04/02/2018
  # Fix dp -P SSP fail in parameter check
  # Add new attribute "HeaderBase" in PSP_DIR, BIOS_DIR.
  # When "HeaderBase" specified, the tool will put the directory header to address "HeaderBase" point to.
  # All IMAGE_ENTRY will be put to the region where "BASE" and "Size" speicified.
  #
  # If only "Base" specified, the tool will put the directory header at "Base", and IMAGE_ENTRYs follow by

#Version 3.1.13  03/01/2018
  # Add SSP combo suppport

#Version 3.1.12  02/26/2018
  # Update help description for entry 0xB

#Version 3.1.11  02/12/2018
  # Add 'SubProgam' support to BIOS DIR

#Version 3.1.10  01/18/2018
  # *Fix dump function abnormal exit when over 10 instances

#Version 3.1.09  11/14/2017
  # *Add Version rendering for KVM engine

#Version 3.1.08  10/24/2017
  # *Support subprogram filed in PSP Directory

#Version 3.1.07  07/26/2017
  # *Add Multiple ROMSIG Address support

#Version 3.1.06  07/26/2017
  # *Fix TR SMU version display issue

#Version 3.1.05  06/15/2017
  # *Add new FW entry type

#Version 3.1.05  06/15/2017
  # *Add new FW entry type

#Version 3.1.04  04/01/2017
  # *Add version display for SMU2

#Version 3.1.03  03/31/2017
  # *Update PSP tool to accommodate with SMU version field change

#Version 3.1.02  11/14/2016
  # * HVB related changes for PSP init recovery (two BIOS signature)

#Version 3.1.01  10/24/2016
  # * Add combo DIR support to DP command (-P new required parameter added)

#Version 3.1.00  09/09/2016
  # * Add Recovery two level DIR support

#Version 3.0.09  08/23/2016
  # * Add entry 0x25 to 0x28

#Version 3.0.08  07/12/2016
  # * Add definition for CoreMCEData, correct modfiable bit for some entries

#Version 3.0.07  06/21/2016
  # * Add Support for PSP Combo header generation

#Version 3.0.06  05/06/2016
  # * Correct the SMU, PSP version
  # * Add version display for ABL & Ucode patch
  # * Add Entry ID for Ucode patch 0x66

#Version 3.0.05  3/31/2016
  # * Add support to specify the instance id of BIOS directory image
  # * Change default alignment from 0x1000 to 0x100
  # * Add new tag SpiBlockSize to specify the SPI block size used for modifiable entry
  # * Add New entry 0x38, 0x64, 0x65

#Version 3.0.04  12/17/2015
  # * Update binary identification sequence, check ROMSIG 1st

#Version 3.0.03  11/09/2015
  # * Support dump PSP SPI image

#Version 3.0.02  09/16/2015
  # * Update PSP entry definition according to PSP BIOS Guide ver 1.03

#Version 3.0.01  08/24/2015
  # * Add Secure gasket binary (0x24)

#Version 3.0.00  05/05/2015
  # * ZP initial support

#Version 2.1.12  03/16/2015
  # * Update PSP directory definition

#Version 2.1.11  03/06/2015
  # * Add Compress Flag support for BIOS directory

#Version 2.1.10  01/19/2015
  # * Change the order & method of parsing input binary for "dp" command to
  #   Fix Can't support eMMC image which "$BHD" BIOS header not locate at offset 0.

#Version 2.1.9  11/24/2014
  # * Output "Resetimage.bin" for type 62 with reset flag set

#Version 2.1.8  9/09/2014
  # * Update 'dp' command for only output image entry
  # * Fix 'dp' command display minus APOB entry address
  # * Fix 'bb', 'bd' command shown incorrect end address for a PSP entry
  # * Add check for PSP, BIOS DIR region address overlap in 'bb', 'bd' command

#Version 2.1.7  8/28/2014
 # * Mask high 4 BITS of PSP FW version
 # * Update DP -X output file format
  #   . Separate Entry Node to IMAGE_ENTRY, VALUE_ENTRY, POINT_ENTRY for both BIOS & PSP Directory
  #   . Remove Sub node of PSP BL/OS, SMU, Add new general Node Detail for BVM to present the detail column

#Version 2.1.6  8/19/2014
 # * Add USB FW type

#Version 2.1.5  6/6/2014
 # * Update APOB, SMU2 Entry type
 # * Always fill APOB Source as zero

#Version 2.1.4
 # * Fix "DP" unexpected error exit when there is no Entry 0x11 in PSP DIR

#Version 2.1.3
 # * Add New Type 0x12, 0x61

#Version 2.1.2
 # * EMMC/UFS support
 #         with AddressMode attribute in the Root tag added
 #         support exact eMMC type image
 # * Update BIOS Image Signature to "$BHD"
 # * Display tool's version at startup

#Version 2.1.1:
 # * Update Entry id: PspAgesaRsm 0x11->0x10; BiosDirInfo 0x12->0x11
 # * Fix RTMSignature.bin has been missed for dp -b
 # * Add output binary file name to "dp -x"

#Version 2.1.0:
 # * Add TypeAttrib tag support
 # * Update Entry id
 # * Add "dp" support to BIOS directory

#Version 2.0.0: XML version configure file