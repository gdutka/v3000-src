//----------------------------------------------------------------------------
// SmmSupervisorPkg Release Note
//
// Copyright 2022, Advanced Micro Devices, Inc.
//----------------------------------------------------------------------------
===============================================================================

| Version 3.026X

Date:    Oct 11, 2023
===============================================================================

SmmSupervisorPkg changes:
Last commit c636d491992e76de2bc8ab21a4ff58ca6fe0cf2d
-   FWDEV-60574 SmmSupv: version 3.026X release
-   EDKII-3096: [EDKII][STX] SmmSupv GCC build support
-   EDKII-3069 [EDKII][STX] PiSmmCpuDxeSmm.lib(PiSmmCpuDxeSmm.obj): unresolved external symbol QuickSort

AmdSupverVisorPkg changes:
Last commit 8ea865b36aa555fb79aa5638ab70cd5749cf7d7d
-   EDKII-3096: [EDKII][STX] SmmSupv GCC build support
-   FWDEV-58878 SmmSupv: Update readme.md to align with latest spec (non functional change)
-   FWDEV-55180 [SmmSupv] Use Acpi Timer instead of TSC timer
===============================================================================

| Version 3.025X

Date:    Aug 17, 2023
===============================================================================

SmmSupervisorPkg changes:
Last commit dbd04ea88182eb77d697fc55e95703494c99d3ae
-  FWDEV-56763 SmmSupv: version 3.025X release
-  FWDEV-55193 Provide interface to show SmmSupervisor version in OS application
-  FWDEV-55036 Add debug post code to record SMI source from SCI
-  EDKII-2806 [EDKII][STX] edk2-stable202305: Invalid StandardSignatureIsAuthenticAMD/InitializeFloatingPointUnits (SmmSupv)

AmdSupverVisorPkg changes:
Last commit d1345972327fd2ee82ad9e72ad4ec3489d58ed20
-  EDKII-2806 [EDKII][STX] edk2-stable202305: Invalid StandardSignatureIsAuthenticAMD/InitializeFloatingPointUnits (AmdSupv)
===============================================================================

| Version 3.024X

Date:    Jun 29, 2023
===============================================================================
Important Notes:
In this release, UMIP(User Mode Instruction Prevention) security feature is enabled, execution of SGDT, SIDT, SLDT, SMSW and STR instructions become available only at CPL=0,
any attempt to execute them with CPL>0 in SMM mode results in a general protection exception.

SmmSupervisorPkg changes:
Last commit 0b508d3db456311b92b096f411f53b68f33adad7
-  FWDEV-53235 SmmSupv: version 3.024X release
-  FWDEV-49912 Mitigate PSIRT ticket CVE-2023-20596(part 2:fix stack alignment issue)
-  FWDEV-41432 SmmSupv refine: More error print, TestPoint on error(part 2)
-  FWDEV-49912 Mitigate PSIRT ticket CVE-2023-20596
-  FWDEV-42801 : [SmmSupv] Coverity Issue: SmmSupervisorPkg - Remove Unnecessary header file
-  FWDEV-41432 SmmSupv refine: More error print, TestPoint on error (keep postcode definition)
-  FWDEV-41432 SmmSupv refine: More error print, TestPoint on error (SmmSupervisorPkg)
-  FWDEV-42423 [SmmSupv] System may hang at dead loop when add EDK2 SerialIoLib in offical release/debug binary (SmmSupervisorPkg)

AmdSupverVisorPkg changes:
Last commit 6964403dff9b0d7e20a6cab3ca6495f31dd1db8d
-  FWDEV-42802 : [SmmSupv] Coverity Issue: AmdSupervisorPkg - Remove Unnecessary header file
-  FWDEV-41432 SmmSupv refine: More error print, TestPoint on error (AmdSupervisorPkg)
-  FWDEV-42423 [SmmSupv] System may hang at dead loop when add EDK2 SerialIoLib in offical release/debug binary (AmdSupervisorPkg)
-  FWDEV-42519 Modify Supervisor_ReleaseNotes.md to add CVE ID
===============================================================================

~~Version 3.023X was dropped~~

===============================================================================

| Version 3.022X

Date:    Apr 07, 2023
===============================================================================
SmmSupervisorPkg changes:
Last commit d0ffe6028753156572b161855ce89d500d857a12
- FWDEV-41645 SmmSupv: version 3.022X release
- FWDEV-41382 [SmmSupv] When SmmSupervisorEntry got error status from it's sub function,
  set PcdSmmIsolationEnable to FALSE before SmmSupervisorEntry exits and return EFI_SUCCESS status
- FWDEV-40479 SmmSupv: Find more generic implementation of TimerLib (Part 2)
- FWDEV-41178 [SmmSupervisor] Change VmgExitLib to CcExitLib in SmmSupervisorPkg
- PLAT-129248 [STP] StormPeak 0091 RC3 non-ASCII characters

AmdSupverVisorPkg changes:
-  FWDEV-41645 SmmSupv: version 3.022X release
   commit 33f30342a1604a473ee5cfe97a36cc964c29c1fc
-  FWDEV-41225 [SmmSupervisor] Rename SIMNOW_SUPPORT to SMMSUPV_SIMNOW_SUPPORT (Part 1)
-  FWDEV-40479 SmmSupv: Find more generic implementation of TimerLib (Part 1)
-  FWDEV-41178 [SmmSupervisor] Change VmgExitLib to CcExitLib in SmmSupervisorPkg

===============================================================================

| Version 3.021X

Date:    Mar 28, 2023
===============================================================================
SmmSupervisorPkg changes:
Last commit 1e0b4ca29e298043e9e0d75b4546d189166805b2
- FWDEV-40255 SmmSupv: version 3.021X release
- FWDEV-39889 Mitigate PSIRT ticket CVE-2023-20571
- FWDEV-38337 Mitigate PSIRT ticket CVE-2023-20563
- FWDEV-38676 Mitigate PSIRT ticket CVE-2023-20565
- PLAT-125541 [STP][Coverity] [HFA] StormPeakPI-SP6_0090_RC1 Coverity test failed on Supervisor related files due to HFA
- PLAT-125445: Update the files to Copyright 2023 message

AmdSupverVisorPkg changes:
Last commit 1275ae2d577139aa2eec1e668daf661accaf930c
-  PLAT-125445: Update the files to Copyright 2023 message

===============================================================================

| Version 3.020X

Date:    Nov 21, 2022
===============================================================================
SmmSupervisorPkg changes:
Latest commit cd1091f6e22bcc8465a76cb1447309b86428a7c0
- FWDEV-28296 SmmSupv: version 3.020X release
- FWDEV-24576 SmmSupv: 5 Level Paging support
              SmiEntry binary update to version 1.3
- EDKII-2417: [EDKII][GCC] multiple definition of mSmmS3ResumeState
- EDKII-2373 [EDKII][GCC] IoLibMsc.c :: IoRead{8,16,32}, IoWrite{8,16,32} support
- FWDEV-13165 Add postcode at SmmSupvisor exception handler


AmdSupverVisorPkg changes:
- FWDEV-28296 SmmSupv: version 3.020X release
  commit 5f574e0343d0c7454da47d15a17a2b5d56305fa1
- FWDEV-24005 Add debug message build support for smmsupv source build
- FWDEV-14828 Add specific string in SmmSupervisor release notes
- FWDEV-15558 Add AmdSupervisorPkg.inc.fdf to include all necessary driver for SmmSupervisorPkg


===============================================================================

| Version 3.011X

Date:    Jul 14, 2022
===============================================================================
SmmSupervisorPkg changes:
Latest commit fa8f57baea017f5a5ae2f6ff78da5772c823ec31
-  PLAT-110803 [RPL AM5] Flash BIOS failed and hang B0005002 after flashing BIOS from external to internal by windows
-  EDKII-2183: Move SmmCpuFeaturesLib as part of AGESA release

AmdSupverVisorPkg changes:
Latest commit a38c59832771112ed29901b2f47d5a24770f9c50
-  PLAT-110803 [RPL AM5] Flash BIOS failed and hang B0005002 after flashing BIOS from external to internal by windows
===============================================================================

| Version 3.010X

Date:    Jun 23, 2022
===============================================================================
SmmSupervisorPkg changes:
Latest commit 12b45d2a594ace428391582f36ba249006058065
 -  FWDEV-16413 SmmSupv: Skip CR register access when SmmPgCfgLock enabled
 -  PLAT-109583 Fix coding style issue in SmmSupervisor
 -  EDKII-2118 [EDKII] edk2-stable202205 integration - IntelTdxNull.c
 -  FWDEV-16230 SmmSupv: Fix compile warning of mSmmSupervisorHandle (Fix exception bring by 7014948, add more PC and assert)
 -  FWDEV-16329 SMMSUPV: SmmSupervisorExceptionHandlerLib sync with EDKII edk2-stable202205 release
 -  FWDEV-16261 [SmmSupv] Fix potential BSP AP sync issue
 -  FWDEV-16230 SmmSupv: Fix compile warning of mSmmSupervisorHandle
 -  FWDEV-15722 Fix potential BSP AP sync issue
 -  PLAT-103327 [Rembrandt FP7r2] System keeps restart continuously after adding the SmmSupvErrorReport.inf to the fdf file
 -  PLAT-102089: Update the files to Copyright 2022 message

AmdSupverVisorPkg changes:
Latest commit 08668d12380b98cb804409aa004903cf862dc88c
 -  PLAT-109566 Use FchForceResetSystemLib instead of FchBaseResetSystemLib in SmmSupervisor
 -  FWDEV-16227 SMMSUPV: Infrastructure update for release (dsc file update)
 -  FWDEV-16227 SMMSUPV: Infrastructure update for release
 -  PLAT-103327 [Rembrandt FP7r2] System keeps restart continuously after adding the SmmSupvErrorReport.inf to the fdf file
 -  FWDEV-7080 [PHX] Remove SmmSupervisorSigningTool

===============================================================================

| Version 3.003X (Patch release over 3.002X)

Date:    Jun 22, 2022
===============================================================================
SmmSupervisorPkg changes:
 -  FWDEV-16261 [SmmSupv] Fix potential BSP AP sync issue

AmdSupverVisorPkg changes:
 -  PLAT-109566 Use FchForceResetSystemLib instead of FchBaseResetSystemLib in SmmSupervisor

===============================================================================

| Version 3.002X

Date:    Aug 24, 2021
===============================================================================
SmmSupervisorPkg changes:
Latest commit 51e3aad5703f7283cb9aa6a19498084a2b15944d
  1.  Added protocol version check for backward compatibility in error reporting driver
  2.  PostDrtm - syncronize BSP and APs before setting GDT page RO
  3.  Clean up paremeters for FindsmmSupvEntry and InitializePolicyData. Free Buffer on ReportDrtmInfo.
  4.  Implement a service to return if SmmIsolation is enabled
  5.  Call Write Msr from assembly function to resolve AMD S3 resume SMM stack alignment issue.
  6.  Reboot the system if BSP/AP sync timeout in the DRTM Protection SMI
  7.  Remove unused header file reference


AmdSupverVisorPkg changes:
Latest commit 5dfe0c8bd009694438cd6d27271e1cbf51438a58
  1. Renamed PspMmio* to SmmSupvPspMmio* to Avoid Conflict with Functions in AmdPspMmioLib
  2.  Memory alignment fix required for PI 0071
  3.  Match updated SmmsupvEntryLocalLib FindSmmSupvEntry arguments
  4.  Add Trackpad IO protocol guid
  5.  SmmSupervisor debug version should stop the system instead of reset when fatal error occurs
  6.  Fix non-ASCII in ReadMe.md

===============================================================================

| Version 3.001X

Date:    April 25, 2021
===============================================================================
SmmSupervisorPkg changes:
  1. RMB SMM Isolation - SMM Page Lock (HW) based SMM Isolation optimizations
  2. Add MSR policy for Level30
  3. Removed duplicated policy entries for example policy
  4. Library clean up to remove redundant dependencies on silicon packages
  5. Write access must be denied to AMD Secure Processor MMIO registers allocated for DRTM Interface
  6. Initialize Ret for SyscallDispatcher to avoid compilation errors and stack information leakage.
  7. Added size check to assure supervisor not to report unrecognized bytes
  8. Update module GUIDs to avoid duplicates and declare libraries
  9. Use MMIO instead of IO to access BIOS RAM
  10. SmmSupervisorPkg spelling fix
  11. Abstracted AMD specific IP into library to remove Agesa dependency
  12. Abstract IHV specific supervised regions into a library
  14. Support returning to function that caused policy violation/exception
  15. Move SmmSupervisorPkg to separate repository
  16. Added AccessCondition field to Save State policy entry
  17. Update for SMM30.SMMSAVESTATE request, seperate the supervision logic to IHV Lib

AmdSupverVisorPkg changes:
  1. AMD implementation of IhvSmmSaveStateSupervisionLib

===============================================================================

| Version 2.005X

Date:   February 2, 2021
===============================================================================
Changes:
  1. Update secure policy definition for level 20+ supervisor

===============================================================================

| Version 2.004X

Date:   January 13, 2020
===============================================================================
Changes:
  1. Update SmmLevel 20 Policies
  2. Protect IOMMU range for level 20

===============================================================================

| Version 1.011X

Date:   December 17, 2020
===============================================================================
Changes:
  1. Fix stack parameters passed to the Error Handler
  2. Add DynamicEx Pcd to control SmmIso feature on/off

===============================================================================

~~Version 1.010X was dropped~~

===============================================================================

| Version 1.009X

Date:   November 11, 2020
===============================================================================
Changes:
  1. Separate Memory, Page, Policy, Post Code into separate libraries.
  2. Support registered exception handler reporting if interrupt was handled
  3. Code for Register SMM Supervisor interrupt handler (Smi Binary version 1.2)

===============================================================================

| Version 1.008X

Date:   November 3, 2020
===============================================================================
Changes:
  1. Use S3 Boot Script NULL library override
  2. Increase CPL0 region count from 0x30 to 0x400
  3. Resolve GPF when SmmErrorReportOem uses XMM instructions
  4. SMM Supervisor allow longer sync time for DRTM SMI

===============================================================================

| Version 1.007X

Date:   September 30, 2020
===============================================================================
Changes:
  1. Make stack 16 bytes aligned to avoid GPF on XMM instructions
  2. increase the smm policy data size from 32KB to 128KB

===============================================================================

| Version 1.006X

Date:   September 15, 2020
===============================================================================
Changes:
  1.Change the thread count to decrement once it is synced

===============================================================================

| Version 1.005X

Date:   August 14, 2020
===============================================================================
Changes:
  1.Clearing page table ready flags at SMI exit to prevent page fault if PostDrtmEntry routine being called multiple times

===============================================================================

| Version 1.004X

Date:   August 12, 2020
===============================================================================
Changes:
  1.fix copyright and non-ASCII issues
  2.GDT protection related enhancement
  3.Set some region pagetable attribute back to readonly and make memory pool 4k aligned
  4.enhance the logging to make it easier to debug SmmSupervisor

===============================================================================

| Version 1.003X

Date:   July 7, 2020
===============================================================================
Changes:
  1.Introduce Instruction Execution Policy Type: Currently only supports CLI, HLT, WBINVD
  2.Check blank for jump buffer before long jump
  3.Provide capability to turn on SMM isolation at SMM ready to lock
  4.Instruction policy change to use mSmmPolicyData, or else will cause Policy gate doesn't works well when SMM ISO at RTL enabled
  5.Add SupervisorSmmCpuFeaturesLib
  6.Added CLI unblock into example policy for PiSmmCpuDxeSmm driver
  7.Update SMM_SUPERVISOR_PROTOCOL to make enabling SMM Isolation more extensible
  8.Introduced PCD to control which event to start SMM isolation
  9.add SPL to SmiEntry binary, add SPL and version info to SmmSupvisor efi file
  10.enable production key enforcement

===============================================================================

| Version 1.002X

Date:   May 29, 2020
===============================================================================
Changes:
  1.Checkout if policy size is greater than maximum SMM_POLICY_DATA_SIZE.
  2.Only search BIOS for Smm Entry Binary once instead of once for each logical processor.
  3.Modify smi timerTicker from 1000000 to 5000.
  4.Switch over to ResetSystemLib and link to Fch base version.
  5.Reset when exception occur in release version.
  6.Added global counter to indicate violated access count. Upon exiting SMM, conduct a BIOS ram recording and reset if counter is larger than 0.
  7.Revert remove CLI in syscall because some IBV/OEM still using it.
  8.Allocate new buffer for each core smmentry.
  9.Added policy entry overlap check and fixed existed MSR overlap in supervisor example.Also fixed an issue where the policy gate does not honor size field from entry header.
  10.Add BaseIoLibIntrinsic.inf for LibraryClasses.X64.SMM_CORE in SmmSupervisorPkg.inc.dsc for some IBV


===============================================================================

| Version 1.001X

Date:   May 18, 2020
===============================================================================
Changes:
  1.update version flag definition add version info for SmmSupervisor driver and SmiEntry binary(update version flag definition)
    #Version string definition
    #Version = Major.Minor
    #Major: major version is one or two digits from 0 to 99
    #Minor: minor version is four digits, the first 3 digits are the minor number, the last digit is the flag
    #flag=9 represents RELEASE version, flag=8 represents DEBUG version, flag=0~7 represents test version


  2.Added DRTM enablement driver for DRTM capable platforms Added DRTM enablement driver for DRTM capable platforms. This driver so far will only do one thing: NV index creation for System Guard. But any common routine needed can go here.

  3.Remove Unused Syscall Remove Unused Syscall

  4.Change mSmmSupervisorPostReadyToLock to be volatile Change mSmmSupervisorPostReadyToLock to be volatile since this will be a multi threaded accessible variable. This change is also expected to change fix the assertion of OUT_OF_SOURCES from allocate pages caused by double invocation of ReadyToLokc callback.

  5.Allocate buffer to save STAR MSRs for each thread Allocate buffer to save STAR MSRs for each thread.

  6.Synchronize BSP with APs at SmmSupvSmiEntry Synchronize BSP with APs at SmmSupvSmiEntry to avoid potential MP related issues.

  7.Write error code to bios ram and reset system to replace CpuDeadLoop and asse... Write error code to bios ram and reset system to replace CpuDeadLoop and assert in Release version.

  8.Enhance the logic to make sure PspMboxSmmBuffer is always updated.

===============================================================================

| Version 1.000X

Date:   May 7, 2020
===============================================================================
  1.Add Binary version
    #Version string definition
    #Version = Major.Minor
    #Major: major version is one or two digits from 0 to 99
    #Minor: minor version is four digits, the first 3 digits are the minor number, the last digit is the flag
    #flag=0 represents RELEASE version, flag=1 represents DEBUG version, flag=2~9 represents test version
    VERSION_STRING = 1.000x

  2.Show sw smi source on postcode
  3.Add ReadMe.md file