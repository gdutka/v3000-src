# @file
# CLI tool to work with a supervisor policy object.
#  Dump a policy binary
#  Create a new policy given an xml file
#  Append to a policy additional entries given an xml file
#
# Copyright (c) Microsoft Corporation
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

import logging
from argparse import ArgumentParser
import os
import datetime
import sys
from policy_entry import *
import xml.etree.ElementTree as ET


def ParseXmlAndAddToPolicy(filepath: str, policy: Supervisor_Policy):
    ''' Parse the XML input file and update the policy'''

    tree = ET.parse(filepath)
    root = tree.getroot()
    for parent in root.findall(".//SmmCategory"):
        pt = parent.get("name")
        access_attr = parent.find("PolicyAccessAttribute")
        if access_attr != None and "Allow" in access_attr.get("Value"):
            AccessAttr = AccessAttribute.ACCESS_ATTR_ALLOW
        else:
            AccessAttr = AccessAttribute.ACCESS_ATTR_DENY
        if pt == "IO":
            Type = POLICY_TYPE.IO
        elif pt == "MSR":
            Type = POLICY_TYPE.MSR
        elif pt == "INSTRUCTION":
            Type = POLICY_TYPE.INSTRUCTION
        elif pt == "SAVESTATE":
            Type = POLICY_TYPE.SAVESTATE
        else:
            raise NotImplementedError(
                    f"Can't encode policy root of type {pt}")

        policy_root = PolicyRoot(Type, AccessAttr)
        for element in parent.findall("PolicyEntry"):
            if pt == "IO":
                IoAddress = element.find("StartAddress").get("Value")
                Size = element.find("Size").get("Value")
                Attributes = element.find("SecurityAttributes").get("Value")
                AttributesValue = AccessType.INHERITED
                if "LimitedRead" in Attributes:
                    AttributesValue += AccessType.COND_READ_INHERITED
                elif "Read" in Attributes:
                    AttributesValue += AccessType.READ_INHERITED
                if "LimitedWrite" in Attributes:
                    AttributesValue += AccessType.COND_WRITE_INHERITED
                elif "Write" in Attributes:
                    AttributesValue += AccessType.WRITE_INHERITED
                if "Execute" in Attributes:
                    AttributesValue += AccessType.EXECUTE_INHERITED
                if "StrictWidth" in Attributes:
                    AttributesValue += AccessType.STRICT_WIDTH_INHERITED

                iop = IoPolicyEntry(IoAddress=int(IoAddress, base=0),
                                    Size=int(Size, base=0),
                                    Attributes=AttributesValue)
                policy_root.AddPolicy(iop)

            elif pt == "MSR":
                MsrAddress = element.find("StartAddress").get("Value")
                Size = element.find("Size").get("Value")
                Attributes = element.find("SecurityAttributes").get("Value")
                AttributesValue = AccessType.INHERITED
                if "LimitedRead" in Attributes:
                    AttributesValue += AccessType.COND_READ_INHERITED
                elif "Read" in Attributes:
                    AttributesValue += AccessType.READ_INHERITED
                if "LimitedWrite" in Attributes:
                    AttributesValue += AccessType.COND_WRITE_INHERITED
                elif "Write" in Attributes:
                    AttributesValue += AccessType.WRITE_INHERITED
                if "Execute" in Attributes:
                    AttributesValue += AccessType.EXECUTE_INHERITED

                msrp = MsrPolicyEntry(MsrAddress=int(MsrAddress, base=0),
                                      Size=int(Size, base=0),
                                      Attributes=AttributesValue)
                policy_root.AddPolicy(msrp)

            elif pt == "INSTRUCTION":
                Instruction = element.find("Instruction").get("Value")
                Attributes = element.find("SecurityAttributes").get("Value")
                AttributesValue = AccessType.INHERITED
                if "LimitedRead" in Attributes:
                    AttributesValue += AccessType.COND_READ_INHERITED
                elif "Read" in Attributes:
                    AttributesValue += AccessType.READ_INHERITED
                if "LimitedWrite" in Attributes:
                    AttributesValue += AccessType.COND_WRITE_INHERITED
                elif "Write" in Attributes:
                    AttributesValue += AccessType.WRITE_INHERITED
                if "Execute" in Attributes:
                    AttributesValue += AccessType.EXECUTE_INHERITED

                instrp = InstructionPolicyEntry(Instruction,
                                                Attributes=AttributesValue)
                policy_root.AddPolicy(instrp)

            elif pt == "SAVESTATE":
                SaveStateField = element.find("SaveStateField").get("Value")
                Attributes = element.find("SecurityAttributes").get("Value")
                ConditionElement = element.find("AccessCondition")
                if ConditionElement != None:
                    Condition = ConditionElement.get("Value")
                else:
                    Condition = ''
                AttributesValue = AccessType.INHERITED
                if "LimitedRead" in Attributes:
                    AttributesValue += AccessType.COND_READ_INHERITED
                elif "Read" in Attributes:
                    AttributesValue += AccessType.READ_INHERITED
                if "LimitedWrite" in Attributes:
                    AttributesValue += AccessType.COND_WRITE_INHERITED
                elif "Write" in Attributes:
                    AttributesValue += AccessType.WRITE_INHERITED

                svstrp = SaveStatePolicyEntry(SaveStateField,
                                              Attributes=AttributesValue,
                                              AccessCondition=Condition)
                policy_root.AddPolicy(svstrp)

            else:
                raise Exception("Unknown XML")

        policy.AddPolicyRoot (policy_root)


def main() -> int:
    # Arg Parse
    parser = ArgumentParser(
        description='Tool to make or dump a Supervisor Policy binary')
    parser.add_argument("-i", "--InputBinary", "--inputbinary", dest="input_bin", default=None,
                        help="Path to input binary to decode.  If -x is given new entries will be appended.",
                        type=str)
    parser.add_argument('-x', "--XmlFile", "--xmlfile", "--Xmlfile", dest="xml_file_path",
                        help="Path to Xml File to encode as policy", type=str)
    parser.add_argument("-o", "--OutputBinary", "--outputbinary", dest="output_binary_path",
                        default=None, help="Path to output policy binary")
    parser.add_argument("-v", "--OutputVersion", "--outputversion", dest="output_version",
                        default=Supervisor_Policy.FLEXBILE_STRUCTURE_VERSION, help="Output binary version in UINT32 format, default will output v1.0",
                        type=int)
    args = parser.parse_args()

    logging.info("Log Started: " + datetime.datetime.strftime(
        datetime.datetime.now(), "%A, %B %d, %Y %I:%M%p"))

    if args.input_bin is not None and not os.path.isfile(args.input_bin):
        logging.critical("Invalid Input Binary file")
        return -1

    if args.xml_file_path is not None and not os.path.isfile(args.xml_file_path):
        logging.critical("Invalid Xml file path")
        return -2

    if args.output_version is None:
        logging.critical("Invalid output version specified")
        return -3

    Policy = Supervisor_Policy(args.output_version)  # create a new one

    if args.input_bin is not None:
        # if input populate with contents
        with open(args.input_bin, "rb") as f:
            Policy.Decode(f.read())

    # if xml file append new entries
    if args.xml_file_path is not None:
        ParseXmlAndAddToPolicy(args.xml_file_path, Policy)

        # print out our policy
    print("=================================================")
    print("========    Start Dumping Policy    =============")
    print("=================================================")
    Policy.DumpInfo(prefix="  ")
    print("=================================================")
    print("========    End Dumping Policy    ===============")
    print("=================================================")
    # write out policy as binary file
    if args.output_binary_path is not None:
        with open(args.output_binary_path, "wb") as f:
            f.write(Policy.Encode())
        logging.critical(
            f"Policy written to: {os.path.abspath(args.output_binary_path)}")

    return 0


if __name__ == "__main__":
    # setup main console as logger
    logger = logging.getLogger('')
    logger.setLevel(logging.NOTSET)
    console = logging.StreamHandler()
    logger.addHandler(console)
    console.setLevel(logging.WARNING)

    # call main worker function
    retcode = main()

    if retcode != 0:
        logging.critical("Failed.  Return Code: %d" % retcode)
    else:
        logging.debug("Success!")
    # end logging
    logging.shutdown()
    sys.exit(retcode)
