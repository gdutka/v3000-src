use strict;
use warnings;
use Cwd;

my $year = `date /t`;
$year =~ /(\d{4})/i;
$year = $1;

my $copyright = "/******************************************************************************
 *    Copyright 2015 - $year Advanced Micro Devices, Inc. All rights reserved.
 *
 *   AMD is granting you permission to use this software and documentation (if
 *   any) (collectively, the \"Materials\") pursuant to the terms and conditions
 *   of the Software License Agreement included with the Materials.  If you do
 *   not have a copy of the Software License Agreement, contact your AMD
 *   representative for a copy.
 *
 *   You agree that you will not reverse engineer or decompile the Materials, in
 *   whole or in part, except as allowed by applicable law.
 *
 *   WARRANTY DISCLAIMER: THE MATERIALS ARE PROVIDED \"AS IS\" WITHOUT WARRANTY OF
 *   ANY KIND.  AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY,
 *   INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT, THAT THE
 *   MATERIALS WILL RUN UNINTERRUPTED OR ERROR-FREE OR WARRANTIES ARISING FROM
 *   CUSTOM OF TRADE OR COURSE OF USAGE.  THE ENTIRE RISK ASSOCIATED WITH THE
 *   USE OF THE MATERIAL IS ASSUMED BY YOU.  Some jurisdictions do not allow the
 *   exclusion of implied warranties, so the above exclusion may not apply to
 *   You.
 *
 *   LIMITATION OF LIABILITY AND INDEMNIFICATION:  AMD AND ITS LICENSORS WILL
 *   NOT, UNDER ANY CIRCUMSTANCES BE LIABLE TO YOU FOR ANY PUNITIVE, DIRECT,
 *   INCIDENTAL, INDIRECT, SPECIAL OR CONSEQUENTIAL DAMAGES ARISING FROM USE OF
 *   THE MATERIALS OR THIS AGREEMENT EVEN IF AMD AND ITS LICENSORS HAVE BEEN
 *   ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  In no event shall AMD's total
 *   liability to You for all damages, losses, and causes of action (whether in
 *   contract, tort (including negligence) or otherwise) exceed the amount of
 *   \$100 USD.  You agree to defend, indemnify and hold harmless AMD and its
 *   licensors, and any of their directors, officers, employees, affiliates or
 *   agents from and against any and all loss, damage, liability and other
 *   expenses (including reasonable attorneys' fees), resulting from Your use of
 *   the Materials or violation of the terms and conditions of this Agreement.
 * 
 *   U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are \"commercial items\", as
 *   that term is defined at 48 C.F.R. Section 2.101, consisting of \"commercial
 *   computer software\" and \"commercial computer software documentation\", as
 *   such terms are used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section
 *   227.7202, respectively. Consistent with 48 C.F.R. Section 12.212 or 48
 *   C.F.R. Section 227.7202-1 through 227.7202-4, as applicable, the commercial
 *   computer software and commercial computer software documentation are being
 *   licensed to U.S.  Government end users (a) only as commercial items and (b)
 *   with only those rights as are granted to all other end users pursuant to
 *   the terms and conditions set forth in this Agreement. Unpublished rights
 *   are reserved under the copyright laws of the United States.
 *
 *   EXPORT RESTRICTIONS: You shall adhere to all applicable U.S. import/export
 *   laws and regulations, as well as the import/export control laws and
 *   regulations of other countries as applicable. You further agrees to not
 *   export, re-export, or transfer, directly or indirectly, any product,
 *   technical data, software or source code received from AMD under this
 *   license, or the direct product of such technical data or software to any
 *   country for which the United States or any other applicable government
 *   requires an export license or other governmental approval without first
 *   obtaining such licenses or approvals; or in violation of any applicable
 *   laws or regulations of the United States or the country where the technical
 *   data or software was obtained.  You acknowledge that the technical data and
 *   software received will not, in the absence of authorization from U.S. or
 *   local law and regulations as applicable, be used by or exported,
 *   re-exported or transferred to: (i) any sanctioned or embargoed country, or
 *   to nationals or residents of such countries; (ii) any restricted end-user
 *   as identified on any applicable government end-user list; or (iii) any
 *   party where the end-use involves nuclear, chemical/biological weapons,
 *   rocket systems, or unmanned air vehicles.   For the most current Country
 *   Group listings, or for additional information about the EAR or Licensee's
 *   obligations under those regulations, please refer to the U.S. Bureau of
 *   Industry and Security's website at http://www.bis.doc.gov/.
 ****************************************************************************
*/


#include <PiPei.h>
#include <Uefi.h>

#include <Dxe/AmdPbsSetupDxe/AmdPbsConfig.h>
#include <Library/PcdLib.h>

EFI_STATUS
EFIAPI
LoadAmdPbsDefault (
  OUT AMD_PBS_SETUP_OPTION        *AmdPbsConfiguration
  )
{
    UINT16   CurrentBoardId;
    UINT32   PreSilCtrlValue;

    CurrentBoardId                                                     = PcdGet16 (CurrentPlatformId);
    AmdPbsConfiguration->PlatformId                                    = PcdGet16 (OriginalPlatformId);
    AmdPbsConfiguration->CurrentId                                     = CurrentBoardId;
    AmdPbsConfiguration->CpuType                                       = 0;
    AmdPbsConfiguration->AmdCpmUsedPbsSize                             = AMD_CPM_USED_PBS_SIZE;
    AmdPbsConfiguration->PBSPlatformSel                                = 0;

";

my $varadjust = "
    PreSilCtrlValue = PcdGet32 (PcdAmdEnvironmentFlag);
    if (PreSilCtrlValue != 0){
      AmdPbsConfiguration->ModernStandbyEn = 0;
      AmdPbsConfiguration->PowerSensorsRoutingSelect = 0;
      AmdPbsConfiguration->Mp2FwSelection = 0;
    }
";

    my $FoundKeyString;
    my $Source_path = $ARGV[0];
    my $ResultFile =$ARGV[1];
    my $TempStr;
    my $savetofile;
    my $LastString;
    my @PreDefinedKeywordsArray = ("#if","#ifdef","#else","#elseif","#endif");
    my $keyword;

    $Source_path =~ s/\\/\//g;
    $savetofile = "";
    $LastString = "";
    $FoundKeyString =0;

    $savetofile .= $copyright;
    open(OUTPUT, "<$Source_path");
    foreach(<OUTPUT>)
    {
        if ($_ =~ "^//") {
           next;
        }

        foreach $keyword (@PreDefinedKeywordsArray) {
          if ($_ =~ $keyword) {
            $savetofile .= $_;
            last;
          }
        }

        if ($_ =~ /(\s+)oneof/) {
           $FoundKeyString = 1;                 # 1: oneof key world 
           next;
        }
        if ($_ =~ /(\s+)numeric/) {
           $FoundKeyString = 2;                 # 2: numeric key world
           next;
        }
        if ($FoundKeyString == 1){
            if ($_ =~ /AMD_PBS_SETUP.([\[\w\]]+),/i) {
                $LastString = "    AmdPbsConfiguration->".$1;
            }
            if ($_ =~ /flags/i && $' =~ /DEFAULT/i){
                $TempStr = $_;
                $TempStr =~ s/\s*//g;                                 # integrate several blank space to one space
                if ($TempStr =~ /default=(0[xX][0-9a-f]+)/i) {
                    $savetofile .= sprintf ("%-70s = %s;\n",$LastString, $1);
                } elsif ($TempStr =~ /value=(\d+)/i) {
                    $savetofile .= sprintf ("%-70s = %d;\n",$LastString, $1);
                } else {
                    print $TempStr;
                    print "====>fail\n"
                }
                $FoundKeyString = 0;

            }
            next;
        }
        if ($FoundKeyString == 2){
            if ($_ =~ /AMD_PBS_SETUP.([\[\w\]]+),/i){
                $LastString = "    AmdPbsConfiguration->".$1;
            }
            if ($_ =~ /DEFAULT/i){
                $TempStr = $_;
                $TempStr =~ s/\s*//g;                                 # integrate several blank space to one space
                if ($TempStr =~ /default=(0[xX][0-9a-f]+)/i) {
                    $savetofile .= sprintf ("%-70s = %s;\n",$LastString, $1);
                } elsif ($TempStr =~ /default=(\d+)/i) {
                    $savetofile .= sprintf ("%-70s = %d;\n",$LastString, $1);
                } else {
                    print $TempStr;
                    print "====>fail\n"
                }

                $FoundKeyString = 0;
            }
            next;
        }
        
    }
    $savetofile .= $varadjust;    
    $savetofile .= "    return EFI_SUCCESS;\n";
    $savetofile .= "}";
    close(OUTPUT);

    open(OUTPUT, ">${ResultFile}");
    print OUTPUT $savetofile;
    close(OUTPUT);



