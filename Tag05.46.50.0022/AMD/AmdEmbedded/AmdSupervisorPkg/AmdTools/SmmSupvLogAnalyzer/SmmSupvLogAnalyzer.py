
# *******************************************************************************
#
# Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# *******************************************************************************

import os
import sys
#Module Data Packing
import struct
#Module for arguments parsing
import argparse
#Module for logging,
import logging
import traceback
import re


Version                      = "0.0.1"
#Version 0.0.1  8/28/2020
  # Initial draft

#Set log level to ERROR
log = logging.getLogger()
FORMAT = '%(message)s'  #message ONLY
logging.basicConfig(format= FORMAT,stream=sys.stderr,level=logging.INFO)

def CheckFileExistance (file):
  return os.path.exists(file)

def FatalErrorExit ():
  log.error (traceback.extract_stack())
  log.error ("ERORR Exit\n")
  sys.exit (2)

#Functional routines
def ParseArg ():
  global Version
  """Parse Input arguments, and return the parsed result"""
  Parser = argparse.ArgumentParser (description='Tool used to parse SmmSupervisor related BIOS serial logs')
  Parser.add_argument('-v', '--verbosity', type=str, choices=['CRITICAL', 'ERROR', 'WARNING', 'INFO', 'DEBUG'], default='INFO')
  Parser.add_argument ('-b','--bootmode', default="")
  Parser.add_argument ('-i','--inputpath', default="")
  Parser.add_argument ('-o','--outpath', default="")

  args = Parser.parse_args ()
  log.setLevel(args.verbosity)
  return args

def printUsage ():
    print("SmmSupvLogAnalyzer Version:" + Version)
    print ("\nUsage:")
    print ("SmmSupvLogAnalyzer -i <input log file path> -o <output file path> -b <boot times, s or m>")
    print ("\n-i <input log file path>: provide input log file path to parse, this is a required parameter;")
    print ("-o <output file path>: provide a file path to output the parsing result, this is an optional parameter;")
    print ("-b <boot mode, s or m>: specify the boot mode which the log covers, s=the log covers single boot, m=the log covers multiple boots. this is a required parameter.")

def logInfoWithoutNewLine (message):
    logging.StreamHandler.terminator = ""
    log.info(message)
    logging.StreamHandler.terminator = "\n"

def truncate(n, decimals=0):
    multiplier = 10 ** decimals
    return int(n * multiplier) / multiplier

def main ():
  try:
      RE_PREFIX_LOG_ANALYZER       = "\^\$\s"
      RE_LOG_SMMSUPV               = "\^\$\s+SmmSupv\s+"
      RE_LOG_DRTM_DLL              = "\^\$\s+DrtmDll\s+"
      RE_LOG_AMD_SL                = "\^\$\s+AmdSl\s+"

      RE_TestPoint                 = "\^\$\s+\w+\s+TestPoint\s+(?P<tp>\w+)"

      RE_LOG_SMM_ISOLATION_ENABLED = "\^\$\s+SmmSupv\s+PostDrtmEntry Success"
      RE_LOG_DRTM_ENABLED          = "\^\$\s+AmdSl\s+Prepare jump to OsSL Entry"

      RE_SMM_SUPV_VERSION          = "SmmSupervisor\s+driver\s+version:\s*(?P<ver>[\w\.]+)"
      RE_SMI_ENTRY_VERSION         = "SmiEntry\s+binary\s+version:\s*(?P<ver>[\w\.]+)"
      RE_AMD_SL_VERSION            = "AmdSlMain\s+Version:\s*(?P<ver>[\w\.]+)"

      RE_DRTM_DLL_SECURE_CALL      = ">>>\s+amddrtm\s+Secure\s+call\s+Operation\s+"

      #error regex
      RE_ERROR_SMMSUPV             = "\^\$\sSmmSupv\s+Fatal:"
      RE_ERROR_AMDSL               = "\^\$\sAmdSl\s+Fatal:"
      RE_ERROR_DRTM_DLL            = "\^\$\sDrtmDll\s+>>>\s+amddrtm\s+Secure\s+call\s+Operation\s+"

      TestPointMap = {
        #SmmSupv TestPoint
       'B0005001': 'Entry point of main',
       'B0005002': 'Exit SmmSupv Entry',
       'B0005003': 'SmmSupv install smi handler',
       'B0005004': 'Exit SmmSupv install smi handler',
       'B0005005': 'SmmSupv SmiEntry',
       'B0005006': 'SmmSupv post drtm entry',
       'B0005007': 'SmmSupv post drtm entry end',
       'B0005008': 'SmmSupv smi exit',
       'B0005009': 'SmmSupv smi exit',
       'B000500A': 'SmmSupv smi exit',
       'B0005E01': 'SmmSupv allocate page fail',
       'B0005E02': 'SmmSupv allocate for GlobalData',
       'B0005E03': 'SmmSupv Initialize SmmIdt fail',
       'B0005E04': 'SmmSupv install protocol interface fail',
       'B0005E05': 'SmmSupv locate smi entry bin fail',
       'B0005E06': 'SmmSupv register psp mbox callback function fail',
       'B0005E07': 'SmmSupv register psp mbox callback function fail',
       'B0005E08': 'SmmSupv allocate GDT buffer fail',
       'B0005E09': 'Unsupport Smi Entry version detected',
       'B0005E0A': 'DrtmInfo is not identical as we report during boottime',
       'B0005E0B': 'DrtmCmdLaunch execute fail',
       'B0005E0C': 'Stack used in SmmEntry is not identical as we reserved',
       'B0005E0D': 'CPL0 stack and CPL3 stack overlap',
       'B0005E0E': 'SmmSupv find SmmSupv directory fail',
       'B0005E0F': 'SmmSupv stack check fail',
       'B0005E10': 'SmmSupv re-init GDT error',
       'B0005E11': 'SmmSupv verify page table error',
       'B0005E12': 'SmmSupv allocate protected memory pool fail',
        #Amd Sl TestPoint
       'B0001001': 'Entry point of main',
       'B0001002': 'Before AmdSlCollectApData (&MpInit)',
       'B0001003': 'Before SetEnFeatures',
       'B0001004': 'Before Fill MemoryDescriptors',
       'B0001005': 'Before GenTmrDescriptors',
       'B0001006': 'Before GenTcgLog',
       'B0001007': 'Before GenAcpiTables',
       'B0001008': 'Before GenSmmConfig',
       'B0001009': 'Before CleanUp',
       'B000100A': 'Before Handle over to OS',
       'B000100B': 'After Handle over to OS, invalid place should not come here',
       'B000100C': 'EXTEND_OSSL_DIGEST',
       'B0001800': '18[DrtmCmdId]',
       'B0001900': '19[DrtmCmdId]',
       'B0001A00': '1A[DrtmCmdId]',
       'B0001E01': 'DrtmCmdLaunch execute fail',
       'B0001E02': 'DrtmCmdGetTmrDescriptors execute fail',
       'B0001E03': 'DrtmCmdExtendOsslDigest execute fail',
       'B0001E04': 'DrtmCmdGetTcgLogs execute fail',
       'B0001E05': 'DrtmCmdAllocateSharedMemory fail',
       'B0001E06': 'DrtmCmdProcessSmm fail',
       'B0001E07': 'DrtmCmdGetSmmPolicy fail',
       'B0001E08': 'time out in waiting DRTM command ready',
       'B0001E09': 'time out in processing DRTM command',
       'B0001FFF': 'Last defined AMDSL PCs'
      }

      Args = ParseArg()

      #validate args
      if (Args.inputpath == "") or (Args.bootmode != "s" and Args.bootmode != "m"):
          printUsage ()
          return

      if (Args.outpath != ""):
         if (CheckFileExistance(Args.outpath) == True):
            os.remove(Args.outpath)

         #file logging
         fileLoggerHandler = logging.FileHandler(Args.outpath)
         log.addHandler(fileLoggerHandler)

      if (CheckFileExistance(Args.inputpath) == False):
        log.error("[Error] Can't Open Input file '%s'", Args.inputpath)
        FatalErrorExit ()

      log.debug ("Args: %s",Args)

      IsLogFromSmmSupervisorFound         = False
      IsLogFromDrtmDllFound              = False
      IsSmmSupervisorTurnedOnSuccessfully = False
      IsAmdSlExecutedSuccessfully        = False

      IsSmmSupvVersionFound              = False
      IsSmiEntryVersionFound             = False
      IsAmdSlVersionFound                = False
      IsDrtmSecureCallFound              = False

      RawData                            = []
      VersionLogs                        = []
      ErrorLogs                          = []

      AmdSlSuccessCount                  = 0
      SmmSupervisorTurnedOnCount          = 0

      log.info ("SmmSupvLogAnalyzer Version:" + Version)
      #Parse file
      logFileSizeInBytes = os.path.getsize(Args.inputpath)
      fileSizeStr = ""
      if (logFileSizeInBytes < 1024):
          fileSizeStr = str(truncate(logFileSizeInBytes, 2)) + " Byte(s)"
      else:
          logFileSizeInKB = logFileSizeInBytes / 1024;
          if (logFileSizeInKB < 1024):
            fileSizeStr = str(truncate(logFileSizeInKB, 2)) + " KB"
          else:
              logFileSizeInMB = logFileSizeInKB / 1024;
              if (logFileSizeInMB < 1024):
                fileSizeStr = str(truncate(logFileSizeInMB,2)) + " MB"
              else:
                log.error("[Error] The Input file '%s' is too large(" + str(truncate(logFileSizeInMB, 2)) + " MB) to read.", Args.inputpath)
                FatalErrorExit ()
                return

      logInfoWithoutNewLine ("Reading logs in '" + Args.inputpath + "' (" + fileSizeStr + ")")



      #code page 437 has best compatibility for all kinds of encoding
      with open(Args.inputpath, mode="r", encoding="cp437") as fp:
            if Args.bootmode == "m": #multile boots log
                lineNumber = 0
                line = fp.readline()
                while line:
                    lineNumber += 1
                    if (lineNumber % 1000) == 0:
                        logInfoWithoutNewLine(".")

                    # at each line check for a match with a regex
                    if (IsSmmSupvVersionFound == False):
                        versionMatch = re.search(RE_SMM_SUPV_VERSION, line)
                        if versionMatch:
                            IsSmmSupvVersionFound = True
                            VersionLogs.append ("SmmSupervisor Version:" + str(versionMatch.groups('ver')[0]))
                            versionMatch = None
                            if (IsLogFromSmmSupervisorFound == False):
                                IsLogFromSmmSupervisorFound = True

                    if (IsSmiEntryVersionFound == False):
                        versionMatch = re.search(RE_SMI_ENTRY_VERSION, line)
                        if versionMatch:
                            IsSmiEntryVersionFound = True
                            VersionLogs.append ("SmiEntry Version:" + str(versionMatch.groups('ver')[0]))
                            versionMatch = None
                            if (IsLogFromSmmSupervisorFound == False):
                                IsLogFromSmmSupervisorFound = True

                    if (IsAmdSlVersionFound == False):
                        versionMatch = re.search(RE_AMD_SL_VERSION, line)
                        if versionMatch:
                            IsAmdSlVersionFound = True
                            VersionLogs.append ("AMD SL Version:" + str(versionMatch.groups('ver')[0]))
                            versionMatch = None
                            if (IsLogFromDrtmDllFound == False):
                                IsLogFromDrtmDllFound = True

                    if (IsDrtmSecureCallFound == False) and (IsLogFromDrtmDllFound == False):
                        match = re.search(RE_DRTM_DLL_SECURE_CALL, line)
                        if match:
                            IsDrtmSecureCallFound = True
                            IsLogFromDrtmDllFound = True
                            match = None

                    #error match
                    errorMatch = re.search(RE_ERROR_SMMSUPV, line)
                    if errorMatch:
                            ErrorLogs.append(errorMatch.string)
                            errorMatch = None
                    errorMatch = re.search(RE_ERROR_AMDSL, line)
                    if errorMatch:
                            ErrorLogs.append(errorMatch.string)
                            errorMatch = None
                    errorMatch = re.search(RE_ERROR_DRTM_DLL, line)
                    if errorMatch:
                            ErrorLogs.append(errorMatch.string)
                            errorMatch = None

                    match = re.search(RE_PREFIX_LOG_ANALYZER, line)
                    if match:
                        matchSS = re.search(RE_LOG_SMMSUPV, line)
                        matchDD = re.search(RE_LOG_DRTM_DLL, line)
                        matchAS = re.search(RE_LOG_AMD_SL, line)
                        matchTP = re.search(RE_TestPoint, line)
                        if (matchSS):
                            if (IsLogFromSmmSupervisorFound == False):
                                IsLogFromSmmSupervisorFound = True
                            matchSSEnabled = re.search(RE_LOG_SMM_ISOLATION_ENABLED, line)
                            if (matchSSEnabled):
                                SmmSupervisorTurnedOnCount += 1
                                if (IsSmmSupervisorTurnedOnSuccessfully == False):
                                    IsSmmSupervisorTurnedOnSuccessfully = True

                        if (matchDD or matchAS):
                            if (IsLogFromDrtmDllFound == False):
                                IsLogFromDrtmDllFound = True
                            matchDrtmEnabled = re.search(RE_LOG_DRTM_ENABLED, line)
                            if (matchDrtmEnabled):
                                AmdSlSuccessCount += 1
                                if (IsAmdSlExecutedSuccessfully == False):
                                    IsAmdSlExecutedSuccessfully = True
                        if (matchTP):
                            tp = str(matchTP.groups('tp')[0])
                            if tp in TestPointMap.keys():
                                RawData.append(match.string.rstrip() + TestPointMap[tp])
                            else:
                                RawData.append(match.string)
                        else:
                            RawData.append(match.string)
                    line = fp.readline()
            else: #single boot log
               #logs = fp.read()
               log.error("\nsingle boot mode is not supported yet, we will support it in next version\n.")
               FatalErrorExit ()
               return


      log.info("\n\nVersions:")
      if (len(VersionLogs) != 0):
          for i in VersionLogs:
            log.info(i)
      else:
         log.info ("no versions found.")

      log.info("\nStatus:")
      if (IsLogFromSmmSupervisorFound == False) and (IsLogFromDrtmDllFound == False):
        log.info("No logs from SmmSupervisor or DRTM DLL found!\nMake sure you have SmmSupervisor package 1005 and beyond.")
      else:
          if (IsLogFromDrtmDllFound == False):
            log.info("No logs from amddrtm.dll found, are you using release version of amddrtm.dll?")
          else:
              if (IsAmdSlExecutedSuccessfully):
                  log.info("DRTM.AMD SL execution: " + str(AmdSlSuccessCount) + " success.")
              else:
                  log.info("DRTM.AMD SL is NEVER executed successfully!")

          if (IsLogFromSmmSupervisorFound == False):
            log.info("No logs from SmmSupervisor package found!")
          else:
              if (IsSmmSupervisorTurnedOnSuccessfully):
                  log.info("SmmSupervisor execution: " + str(SmmSupervisorTurnedOnCount) + " success")
              else:
                  log.info("SmmSupervisor is NEVER turned on successfully!")

      log.info("\nErrors:")
      if (len(ErrorLogs) != 0):
          for i in ErrorLogs:
            log.info(i)
      else:
          log.info("no errors found.")

      log.info("\nRaw Data:")
      if (len(RawData) != 0):
          for i in RawData:
            log.info(i.rstrip())
      else:
          log.info("nothing found.")
  except BaseException as e:
    log.info('An exception occurred: {}'.format(e))


if __name__ == "__main__":
  main ()