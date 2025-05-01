::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Copyright (C) 2018 - 2021 ADVANCED MICRO DEVICES, INC. All rights reserved.
::
:: AMD is granting You permission to use this software and documentation (if
:: any) (collectively, the "Software") pursuant to the terms and conditions of
:: the Software License Agreement included with the Software. If You do not have
:: a copy of the Software License Agreement, contact Your AMD representative for
:: a copy.
::
:: You agree that You will not reverse engineer or decompile the Software, in
:: whole or in part, except as allowed by applicable law.
::
:: WARRANTY DISCLAIMER: THE SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
:: KIND. AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
:: BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, QUALITY,
:: FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT AND WARRANTIES
:: ARISING FROM CUSTOM OF TRADE OR COURSE OF USAGE WITH RESPECT TO THE SOFTWARE,
:: INCLUDING WITHOUT LIMITATION, THAT THE SOFTWARE WILL RUN UNINTERRUPTED OR
:: ERROR-FREE. THE ENTIRE RISK ASSOCIATED WITH THE USE OF THE SOFTWARE IS
:: ASSUMED BY YOU. Some jurisdictions do not allow the exclusion of implied
:: warranties, so the above exclusion may not apply to You, but only to the
:: extent required by law.
::
:: LIMITATION OF LIABILITY AND INDEMNIFICATION: TO THE EXTENT NOT PROHIBITED BY
:: APPLICABLE LAW, AMD AND ITS LICENSORS WILL NOT, UNDER ANY CIRCUMSTANCES BE
:: LIABLE TO YOU FOR ANY PUNITIVE, DIRECT, INCIDENTAL, INDIRECT, SPECIAL OR
:: CONSEQUENTIAL DAMAGES ARISING FROM POSSESSION OR USE OF THE SOFTWARE OR
:: OTHERWISE IN CONNECTION WITH ANY PROVISION OF THIS AGREEMENT EVEN IF AMD AND
:: ITS LICENSORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. THIS
:: INCLUDES, WITHOUT LIMITATION, DAMAGES DUE TO LOST OR MISAPPROPRIATED DATA,
:: LOST PROFITS OR CONFIDENTIAL OR OTHER INFORMATION, FOR BUSINESS INTERRUPTION,
:: FOR PERSONAL INJURY, FOR LOSS OF PRIVACY, FOR FAILURE TO MEET ANY DUTY
:: INCLUDING OF GOOD FAITH OR REASONABLE CARE, FOR NEGLIGENCE AND FOR ANY OTHER
:: PECUNIARY OR OTHER LOSS WHTSOEVER. In no event shall AMD's total liability to
:: You for all damages, losses, and causes of action (whether in contract, tort
:: (including negligence) or otherwise) exceed the amount of $50 USD. You agree
:: to defend, indemnify and hold harmless AMD, its subsidiaries and affiliates
:: and their respective licensors, directors, officers, employees, affiliates or
:: agents from and against any and all loss, damage, liability and other
:: expenses (including reasonable attorneys' fees), resulting from Your
:: possession or use of the Software or violation of the terms and conditions of
:: this Agreement.
::
:: U.S. GOVERNMENT RESTRICTED RIGHTS: Notice to U.S. Government End Users. The
:: Software and related documentation are "commercial items", as that term is
:: defined at 48 C.F.R. Section 2.101, consisting of "commercial computer
:: software" and "commercial computer software documentation", as such terms are
:: used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section 227.7202,
:: respectively. Consistent with 48 C.F.R. Section 12.212 or 48 C.F.R. Sections
:: 227.7202-1 through 227.7202-4, as applicable, the commercial computer
:: software and commercial computer software documentation are being licensed to
:: U.S. Government end users: (a) only as commercial items, and (b) with only
:: those rights as are granted to all other end users pursuant to the terms and
:: conditions set forth in this Agreement. Unpublished rights are reserved under
:: the copyright laws of the United States.
::
:: EXPORT RESTRICTIONS:  You shall adhere to all applicable U.S. import/export
:: laws and regulations, as well as the import/export control laws and
:: regulations of other countries as applicable. You further agree You will not
:: export, re-export, or transfer, directly or indirectly, any product,
:: technical data, software or source code received from AMD under this license,
:: or the direct product of such technical data or software to any country for
:: which the United States or any other applicable government requires an export
:: license or other governmental approval without first obtaining such licenses
:: or approvals, or in violation of any applicable laws or regulations of the
:: United States or the country where the technical data or software was
:: obtained. You acknowledges the technical data and software received will not,
:: in the absence of authorization from U.S. or local law and regulations as
:: applicable, be used by or exported, re-exported or transferred to: (i) any
:: sanctioned or embargoed country, or to nationals or residents of such
:: countries; (ii) any restricted end-user as identified on any applicable
:: government end-user list; or (iii) any party where the end-use involves
:: nuclear, chemical/biological weapons, rocket systems, or unmanned air
:: vehicles.  For the most current Country Group listings, or for additional
:: information about the EAR or Your obligations under those regulations, please
:: refer to the website of the U.S. Bureau of Industry and Security at
:: http://www.bis.doc.gov/.
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo off
setlocal EnableDelayedExpansion
pushd %cd%

set origin=%cd%
set drive=%~d0
set root=%~dp0
set BIOS_DIR_L2=BiosDirL2
set BIOS_DIR_L1=BiosDirL1
set DirLevel=%BIOS_DIR_L2%
IF "%PspKitPath%" == "" (
  SET PspKitPath=%root%..\NewPspKit  
  call :GetFullPath !PspKitPath!
  set PspKitPath=!paramFullPathName!


  IF NOT EXIST !PspKitPath!\BuildPspImage.bat (
     SET PspKitPath=%root%..\..\NewPspKit
     call :GetFullPath !PspKitPath!
     set PspKitPath=!paramFullPathName!
  )
  

  ::CHECK_PATH
  IF NOT EXIST !PspKitPath!\BuildPspImage.bat (
    @echo !!!PspKitPath system variable is NOT set correctly!!
    goto ERROR
  )
)


SET PspToolPath=%PspKitPath%\PspDirectoryTool\

set APCB_BIN=NULL
set LOOP_END=FALSE
if %1% == -r goto READ
if %1% == -w goto WRITE
goto END

:GetFullPath
set paramFullPathName=%~f1
EXIT /B 0

:READ
set SOC_NAME=%2
set INPUT_BIN=%3
set HEADER_FILE=%4
set XML_NAME=%5
set SubProgramID=%6
del %PspKitPath%\input.rom
copy %INPUT_BIN% %PspKitPath%\input.rom
rd /S /Q %PspToolPath%output
%PspToolPath%BuildPspDirectory.exe -o %PspToolPath%output dp -p %SOC_NAME% -b -d -x %PspKitPath%\input.rom
%drive%
cd %PspToolPath%output

:APCB_BIN_READ_PARSE
for /F "tokens=2 delims=<>" %%a in ('findstr "Type=\"0x68\"" PspDirInfo.xml') do (
  set str=%%a
  set instanceID="0x8"
  set fileflag=false
  set instanceflag=false
  set subprogramflag=false
  call :XML_LINE_PARSE
)

for /F "tokens=2 delims=<>" %%a in ('findstr "Type=\"0x60\"" PspDirInfo.xml') do (
  set str=%%a
  set instanceID="0x1"
  set fileflag=false
  set instanceflag=false
  set subprogramflag=false
  call :XML_LINE_PARSE
)

for /F "tokens=2 delims=<>" %%a in ('findstr "Type=\"0x60\"" PspDirInfo.xml') do (
  set str=%%a
  set instanceID="0x0"
  set fileflag=false
  set instanceflag=false
  set subprogramflag=false
  call :XML_LINE_PARSE
)

for /F "tokens=2 delims=<>" %%a in ('findstr "Type=\"0x68\"" PspDirInfo.xml') do (
  set str=%%a
  set instanceID="0x0"
  set fileflag=false
  set instanceflag=false
  set subprogramflag=false
  call :XML_LINE_PARSE
)

if !APCB_BIN! == NULL (
  if "!DirLevel!" == "%BIOS_DIR_L2%" (
    set DirLevel=%BIOS_DIR_L1%
    goto APCB_BIN_READ_PARSE
  )
)

del %PspKitPath%\input.rom
%drive%
cd %root%
echo ApcbV3Editor.exe -r "!APCB_BIN!" %HEADER_FILE% %XML_NAME%
ApcbV3Editor.exe -r "!APCB_BIN!" %HEADER_FILE% %XML_NAME%
goto END

:WRITE
set SRC_BIN=%2
set OUT_BIN=%3
set HEADER_FILE=%4
set XML_NAME=%5
set SubProgramID=%6
%drive%
cd %PspToolPath%output

:APCB_BIN_WRITE_PARSE
for /F "tokens=2 delims=<>" %%a in ('findstr "Type=\"0x60\"" PspDirInfo.xml') do (
  set str=%%a
  set instanceID=""
  set fileflag=false
  set instanceflag=false
  set subprogramflag=false
  call :XML_LINE_PARSE
)

for /F "tokens=2 delims=<>" %%a in ('findstr "Type=\"0x68\"" PspDirInfo.xml') do (
  set str=%%a
  set instanceID=""
  set fileflag=false
  set instanceflag=false
  set subprogramflag=false
  call :XML_LINE_PARSE
)

if !APCB_BIN! == NULL (
  if "!DirLevel!" == "%BIOS_DIR_L2%" (
    set DirLevel=%BIOS_DIR_L1%
    goto APCB_BIN_WRITE_PARSE
  )
)

cd %root%

echo ApcbV3Editor.exe -w "!APCB_BIN!" %HEADER_FILE% %XML_NAME%
ApcbV3Editor.exe -w "!APCB_BIN!" %HEADER_FILE% %XML_NAME%
copy %SRC_BIN% %PspKitPath%\input.rom
%drive%
cd %PspKitPath%
call %PspKitPath%\BuildPspImage.bat "input.rom" "NotFound" "%PspToolPath%output\PspDirectory.xml" output.rom NOSIGN

popd
copy %PspKitPath%\output.rom %OUT_BIN%
del %PspKitPath%\input.rom
del %PspKitPath%\output.rom
goto END

:END
popd
@echo on
@goto EOF

:XML_LINE_PARSE
for /F "tokens=1* delims==" %%b in ("!str!") do (
  set name=%%b
  set str=%%c
  if !instanceID! == "" (
    set instanceflag=true
  ) else (
    if "!name:~-8!" == "Instance" (
      for /F "tokens=1" %%x in ("%%c") do (
        set instance=%%x
        set instance=!instance:~1,-1!
        if "!instance!" == !instanceID! (
          set instanceflag=true
        )
      )
    )
  )

  if "!SubProgramID!" == "NULL" (
    set subprogramflag=true
  ) else (
    if "!name:~-10!" == "SubProgram" (
      for /F "tokens=1" %%x in ("%%c") do (
        set SubProgram=%%x
        set SubProgram=!SubProgram:~1,-1!
        if !SubProgram! == !SubProgramID! (
          set subprogramflag=true
        ) else (
          set subprogramflag=false
        )
      )
    )
  )

  if "!name:~-4!" == "File" (
    for /F "tokens=1" %%x in ("%%c") do (
      set file=%%x
      set file=!file:~1,-1!
      if "!file:~0,9!" == "!DirLevel!" (
        set fileflag=true
      )
    )
  )
  if !fileflag!==true if !instanceflag!==true if !subprogramflag!==true (
    echo Input binary !file! found.
    if "!APCB_BIN!" == "NULL" (
      set APCB_BIN=%PspToolPath%output\!file!
      set LOOP_END=TRUE
    ) else (
      set APCB_BIN=!APCB_BIN!;%PspToolPath%output\!file!
      set LOOP_END=TRUE
    )
  )
)

if "!str!" == "" (
  goto EOF
) else (
  if "!LOOP_END!" == "FALSE" (
    CALL :XML_LINE_PARSE
    goto EOF
  ) else (
    set LOOP_END=FALSE
    goto EOF
  )
)

:ERROR
@echo **** Fail ****
@exit /B 2
GOTO EOF

:EOF


