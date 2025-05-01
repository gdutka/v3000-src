"""
*******************************************************************************
 Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.

*******************************************************************************
"""

from dataclasses import dataclass
import ActStrings as pretty_act
from ast import literal_eval

# VISIBLE_SELECTION can be either "Internal" or "External"
# Determines which sections are visible to the ACT Tool
VISIBLE_SECTION=["External"]

# VISIBLE_TYPE can contain "Fixed", "Customed", or "Text"
# Determines which input types are visible to the ACT Tool
VISIBLE_TYPE=["Fixed"]

# PREFIX_REMOVE contains a list of variable prefixes that should be removed
# from the beginning of Variable Names.
PREFIX_REMOVE=["Cbs", "Pcd"]

@dataclass
class SetupSection:
    """!
        Defines a Section object that represents a section in BIOS Setup.

        Properties:
            Name        - Name of the section
            Level       - Section Level
            Visibility  - Indicates the visibility of the Section as a whole
    """
    SectionName: str
    Level: int
    Visibility: str

@dataclass
class OptionValueData:
    """!
        Defines an OptionValueData object that represents a potential setting for
        a setup option.

        Properties:
            ItemText    - Output text for the Option Value
            ItemData    - Data value corresponding to ItemText
            ItemImpact  - Indicates register impact of Option Value
            ImpactText  - String output to reflect impacted registers
    """
    ItemText: str
    ItemData: str
    ItemImpact: str
    ImpactText: str


def read_xml_string(OptionItem, Field):
    """!
        Takes in an Options Object in XML format and returns
        string value for the "Field" in OptionItem.

        Returns a string representation of the "Field" data value
        in OptionItem or an empty string if no "Field" is populated.

        Parameters:
            OptionItem       - Option represented in XML format
            Field            - String field property to be read from OptionItem
    """
    # Input Validation
    if not Field:
        return ""

    if OptionItem.find(Field).text != None:
        return OptionItem.find(Field).text
    else:
        return ""

def format_pcd(PcdName):
    """!
        Takes in a full PCD name and strips off the beginning
        token space and the type at the end to return a string
        consisting of only the PCD name.

        For Example, the PCD
        gEfiAmdAgesaModulePkgTokenSpaceGuid.PcdAmdEnableRMSS(BOOL)
        shortens to PcdAmdEnableRMSS

        Parameters:
            PcdName       - PCD string to format
    """
    # Input Validation
    if not PcdName:
        return ""

    # Remove Token Space Prefix
    PcdName = PcdName.split('.')[1]

    # Remove End Type if Present
    PcdName = PcdName.split('(')[0]

    return PcdName

class Option():
    """!
        Defines a Option object that represents an option in CBS Setup

        Properties:
            VarName         - Option Variable Name
            Level           - Level of option
            Uid             - UID Generated for option
            PrettyName      - Option Display Name
            Sections[]      - Sections the option appears under
            InputType       - Fixed versus customized
            NvType          - Size of option variable
            HelpText        - Help text to be displayed with the option
            ApcbName        - Name of APCB designator
            PcdName         - Name of PCD identifier
            BiosDefault     - Default value for option
            BiosCurrent     - Boot value for this option (initialized to default)
            ByIds           - Boolean indicating IDS inclusion
            InPei           - Boolean indicating PEI availability
            InDxe           - Boolean indicating DXE availability
            IsApcb          - Boolean indicating if variable is APCB variable
            OptionValueCount - Number of possible values for this option
            OptionItems[]   - List of possible values for this option
    """

    def __init__(self):
        """!
            Creates a Option object that has empty strings and invalid data.
            Object can be populated with fill_option_xml_data

        """
        self.Level = ""
        self.PrettyName = ""

        # Save Empty Section Heirarchy
        self.Sections = list()

        # Initialize Empty Option Value List
        self.OptionValues   = list()
        self.OptionValueCount = 0

        self.VarName = ""
        self.FullVarName = ""
        self.InputType = ""
        self.NvType = ""
        self.HelpText = ""
        self.ApcbName = ""
        self.PcdName = ""
        self.Uid = 0
        self.BiosDefault = 0
        self.BiosCurrent = 0
        self.ByIds = False
        self.InPei = False
        self.InDxe = False
        self.IsApcb = False
        self.Visible = False


    def __str__(self):
        """!
            Returns a formatted, pretty output string with all of the data for Option
        """
        return pretty_act.console_option_display.format(
            pretty_name     = self.PrettyName,
            var_name        = self.VarName,
            uid             = self.Uid,
            in_type         = self.InputType,
            data_size       = self.NvType,
            default_value   = str(self.BiosDefault),
            current_value   = str(self.BiosCurrent),
            pcd_name        = self.PcdName,
            apcb_name       = self.ApcbName,
            help_text       = self.HelpText)

    def fill_option_xml_data(self, OptionItem, SectionList):
        """!
            Fills a Option object based on the option item parameter passed in

            Parameters:
                OptionItem  - Root node for the Option object passed in
                SectionList - Heirarchy of setup pages to get to option
        """
        # Read and Populate Option Attributes
        self.Level      = OptionItem.attrib["Level"]
        self.PrettyName = OptionItem.attrib["Name"]

        # Save Current Section Heirarchy
        self.Sections = list(SectionList)

        # Initialize Empty Option Value List
        self.OptionValues   = list()

        # Read and Populate String Option Elements (if defined)
        self.VarName        = self.remove_prefix(read_xml_string(OptionItem, "Variable_name"))
        self.FullVarName    = read_xml_string(OptionItem, "Variable_name")
        self.InputType      = read_xml_string(OptionItem, "Input_type")
        self.NvType         = read_xml_string(OptionItem, "Nv_type")
        self.HelpText       = read_xml_string(OptionItem, "Help_text")
        self.ApcbName       = read_xml_string(OptionItem, "Apcb_structure")
        self.PcdName        = format_pcd(read_xml_string(OptionItem, "PCD"))
        self.Uid            = 0

        # Read and Populate Processed Option Elements (if defined)
        self.BiosDefault    = literal_eval(read_xml_string(OptionItem, "BIOS_Default"))
        self.BiosCurrent    = literal_eval(read_xml_string(OptionItem, "BIOS_Default"))
        self.ByIds          = bool(int(read_xml_string(OptionItem, "By_IDS")))
        self.InPei          = bool(int(read_xml_string(OptionItem, "PEI")))
        self.InDxe          = bool(int(read_xml_string(OptionItem, "DXE")))
        self.IsApcb         = bool(int(read_xml_string(OptionItem, "APCB")))

        # Determine and Save Option Visibility
        self.Visible        = self.section_is_visible() and self.in_type_is_visible()

        # Get All Possible Values for Option
        self.add_option_values(OptionItem)


    def add_option_values(self, OptionItem):
        """!
            Collects all the possible Option Data Values for a particular option
            and saves them with this Option Object.

            Parameters:
                OptionItem      - Option represented as an XML tree
        """
        option_value_count = 0
        option_data_list = OptionItem.findall("OPTION_ITEM")
        for option_value in option_data_list:
            option_value_count+=1
            currentItem = OptionValueData(option_value.attrib['Name'],
                                          read_xml_string(option_value, "Nv_storage"),
                                          "", "")

            self.OptionValues.append(currentItem)
        self.OptionValueCount = option_value_count

    def remove_prefix(self, FullVarName):
        """!
            Removes the string "Cbs" or "Pcd" from the start of the
            variable name.
            Additional strings can be added to PREFIX_REMOVE.

            Parameters:
                FullVarName - String containing the full variable name
                              extracted from the config XML file.
        """
        for prefix in PREFIX_REMOVE:
            if FullVarName.startswith(prefix):
                return FullVarName[len(prefix):]
            return FullVarName


    def section_is_visible(self):
        """!
            Determines if the Setup Section is visible or not based on
            the visibility of the preceding Sections.

            Returns:
                True  - If the Config Section shows up in BIOS Setup
                False - If the Config Section is hidden in BIOS Setup
        """

        for section in self.Sections:
            if section.Visibility in VISIBLE_SECTION:
                return True

        return False

    def in_type_is_visible(self):
        """!
            Check that the input type of the Config Option is a type that
            is included in this list.

            Returns:
                True  - If the Config Option shows up in BIOS Setup
                False - If the Config Option is hidden in BIOS Setup
        """
        if self.InputType in VISIBLE_TYPE:
            return True

        return False

    def set_uuid(self, UuidValue):
        """!
            This function sets the Uuid for this particular option object.

            Parameters:
                UuidValue   - Numeric UUID to set Uid to
        """
        self.Uid = UuidValue
