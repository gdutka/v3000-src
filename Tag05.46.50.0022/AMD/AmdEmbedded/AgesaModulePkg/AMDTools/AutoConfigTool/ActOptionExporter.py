"""
*******************************************************************************
 Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.

*******************************************************************************
"""
import textwrap
import argparse
import xml.etree.ElementTree as ET
import ActStrings as pretty_act
import ActToolUuid as amd_uuid
import yaml
from ActOptionClasses import Option
from ActOptionClasses import SetupSection
import os
from os import path

# MAX_LINE_LEN defines the maximum string length for lines written
# to an output file
MAX_LINE_LEN=73

class ConfigFileParser:
    """!
        Manages all of the parsing and data management for extracting data from an
        XML file, using that data to generate UUIDs, and writing the extracted and
        generated data to header and python files.

        Properties:
            OutputHeaderFile    - Name of the Header File to be generated.
            HeaderFileGuard     - Header guard name string.
            CheckOptionNames[]  - List of ACT_CFG_UID_x names extracted from the existing
                                  ACT files listed in the configuration file.
            CheckOptionUids[]   - List of UIDs associated with the ACT_CFG_UID_x names that
                                  were extracted from the existing ACT files listed in the
                                  input configuration file.
            OptionList[]        - List of options gathered from InputXml file.
    """

    def __init__(self, ConfigFile):
        """!
            Opens up a configuration file and extracts configuration information
            about the input and outputs of this script.

            Parameters:
                ConfigFile      - Path to configuration YAML file where the git repositories
                                  are listed.
        """
        # Initialize Parser Data
        self.OutputHeaderFile       = ""
        self.HeaderFileGuard        = ""
        self.CheckOptionNames       = []
        self.CheckOptionUids        = []
        self.OptionList             = []

        # Read Config File
        check_path_exists(ConfigFile)
        YamlStream = open(ConfigFile, 'r')
        ConfigData = yaml.safe_load(YamlStream)
        YamlStream.close()

        # Save File Names from Config
        self.OutputHeaderFile = ConfigData['Settings']['Output Header File']
        self.HeaderFileGuard = ConfigData['Settings']['Header File Guard']
        self.ShellHeaderFile = ConfigData['Settings']['Shell Header File']

        # Parse BIOS Options XML File
        tree = ET.parse(ConfigData['Settings']['Input XML'])
        spec = tree.getroot()
        self.root = spec
        self.parse_setup_xml()

        # Populate UUID Values
        self.generate_uuids()

        # Write C Header File
        self.write_header()

        # Write Shell File
        self.write_shell_file()


    def generate_uuids(self):
        """!
            Generates uuid for setting, checks that it doesn't conflict with existing
            uuids and then adds the uuid to the set of known uuids
        """
        for OptionItem in self.OptionList:
            temp_uuid = amd_uuid.AmdUuid(OptionItem.VarName).UuidValue
            while temp_uuid in self.CheckOptionUids:
                temp_uuid = amd_uuid.AmdUuid(OptionItem.VarName).UuidValue
                print("regenerating Conflict UUID {}".format(temp_uuid))

            OptionItem.set_uuid(temp_uuid)
            self.CheckOptionNames.append(OptionItem.VarName)
            self.CheckOptionUids.append(temp_uuid)

    def parse_setup_xml(self):
        """!
            Parses options XML to create a set of Setup options, sections, and
            values.
        """
        for sectionLevel1 in self.root:
            TempSectionList = list()
            self.parse_section(sectionLevel1, TempSectionList)

    def parse_section(self, SectionItem, SectionList):
        """!
            Creates a new SetupSection for SectionItem, adds the SetupSection to
            the SectionList, then parses the ITEMS inside the SetupSection.
            If the ITEM found is an OPTION, it is saved with the SectionList
            data.
            If the ITEM found is a SECTION, we call this function with the
            SubSection as a parameter.

            Parameters:
                SectionItem       - Setup section currently being looked at.
                SectionList       - The "section path" to get to a setting
                                    or item. New sections are appended to the
                                    end of this list. When we finish adding options
                                    for a section it is popped off the end of the list.
        """
        # Create object for current section
        currentSection = SetupSection(SectionItem.attrib['Name'],
                                      SectionItem.attrib['Level'],
                                      SectionItem.find('Visibility').text)
        SectionList.append(currentSection)

        # Find Options in current Section
        for item in SectionItem.findall("ITEM"):

            # Add Option Item
            if item.attrib['Type'] == "OPTION":
                current_option = Option()
                current_option.fill_option_xml_data(item, SectionList)
                if current_option.Visible:
                    self.OptionList.append(current_option)

            # Keep Parsing Sections
            if item.attrib['Type'] == "SECTION":
                self.parse_section(item, SectionList)

        # Leaving Section, Remove from Section List
        SectionList.pop()

    def write_header(self):
        """
            This file outputs a C Header file with generated names and guids
            as definitions.
        """
        # Add Header
        contents = pretty_act.amd_header
        contents += pretty_act.define_header.format(guard = self.HeaderFileGuard)

        # Add Option Information (with comments)
        for OptionData in self.OptionList:
            # Start Option Comments
            contents += pretty_act.option_comment.format(
                pretty_name = OptionData.PrettyName,
                uid = OptionData.Uid)

            # Fill in Help Text
            for line in OptionData.HelpText.splitlines():
                # Limit Line Length to a Total of 79 Characters
                for sized_line in textwrap.wrap(line, MAX_LINE_LEN, break_long_words=False):
                    contents += pretty_act.option_help.format(
                        help_line = sized_line)

            # Fill In Data Type
            contents += pretty_act.option_data_type.format(
                data_type = OptionData.NvType
            )

            # Fill In Section Category
            category_string = ""
            for category in OptionData.Sections:
                category_string += pretty_act.category_string_part.format(format_section = category.SectionName)

            contents += pretty_act.option_category.format(
                format_section = category_string)

            # End Option Comments
            contents += pretty_act.option_end_comment

            # Add Option/Uid #define Pairs
            contents += pretty_act.define_option.format(
                var_name = OptionData.VarName,
                uid = OptionData.Uid)

        # Add Closing Include Guard
        contents += pretty_act.end_define_header.format(guard = self.HeaderFileGuard)

        # Write Everything to Header File
        HFile = open(self.OutputHeaderFile, 'w')
        HFile.writelines(contents)
        HFile.close()

    def write_shell_file(self):
        """
            This function outputs a header file for the UEFI Shell containing
            strings and other information about the setup options.

        """
        # Clear Value-Pair String Variable
        content_value_strings = ""

        # Add Copyright and Add Includes
        header_contents = pretty_act.amd_header
        header_contents += pretty_act.include_files

        # Start Set of Variables
        contents = pretty_act.start_variable_array

        # Add Option Data to File
        for OptionData in self.OptionList:
            nv_size_string = ""
            if OptionData.NvType == "Byte":
                nv_size_string = "ActSizeByte"
            elif OptionData.NvType == "Word":
                nv_size_string = "ActSizeWord"
            elif OptionData.NvType == "DWord":
                nv_size_string = "ActSizeDWord"
            else:
                nv_size_string = "ActSizeUndefined"

            uid_string = pretty_act.uid_name.format(
                var_name = OptionData.VarName.strip()
            )
            contents += pretty_act.start_variable_item.format(
                var_name = OptionData.VarName.strip(),
                display_name = OptionData.PrettyName,
                apcb_name = OptionData.ApcbName,
                pcd_name = OptionData.PcdName,
                uid = uid_string,
                value = OptionData.BiosDefault,
                nv_type = nv_size_string,
                var_count = OptionData.OptionValueCount
            )


            content_value_strings += pretty_act.start_value_set.format(
                var_name = OptionData.VarName.strip()
            )

            for ValueData in OptionData.OptionValues:
                content_value_strings += pretty_act.variable_value_pair.format(
                    value = ValueData.ItemData,
                    option_string = ValueData.ItemText
                )
            content_value_strings = content_value_strings.rstrip(',')
            content_value_strings += pretty_act.end_value_set
            contents += pretty_act.end_variable_item

        # Remove Trailing Space and Comma
        contents = contents.rstrip(',')
        contents += pretty_act.end_variable_array

        contents += pretty_act.end_value_file

        # Write Everything to Python File
        ShellFile = open(self.ShellHeaderFile, 'w')
        ShellFile.writelines(header_contents)
        ShellFile.writelines(content_value_strings)
        ShellFile.writelines(contents)
        ShellFile.close()


def check_path_exists(path):
    """!
        Check if path exists
        Raise FileNotFoundError if the path does not exist

        Parameters:
            path        - File path the function is checking

        Exception  FileNotFoundError
    """
    if not os.path.exists(path):
        error_text = '"{}" does not exist'.format(path)
        print(error_text)
        raise FileNotFoundError(error_text)

def parse_args():
    """
        Parses input arguments for values needed by function
    """
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        description="Parse XML file to extract BIOS Setup Information")

    parser.add_argument(
        "-c", "--cfg",
        help="YAML Configuration File for Project",
        default='AGESA/AgesaModulePkg/AMDTools/AutoConfigTool/StonesConfig.yaml')

    return parser.parse_args()


def main():
    """!
        Main function for the ActOptionExporter.py script. The script parses the
        Settings XML File input to generate UUIDs for each of the settings and then writes
        the data into files as specified by input parameters.
    """
    args = parse_args()

    configuration = ConfigFileParser(args.cfg)

if __name__ == "__main__":
    main()

