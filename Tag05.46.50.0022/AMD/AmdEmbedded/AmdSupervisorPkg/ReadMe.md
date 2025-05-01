# SmmSupervisorPkg

## About
Secured-Core PC (aka Level 3):

Secured-core PCs (also referred to as Security Level 3), is a term defined by Microsoft, combine identity, virtualization, operating system, hardware and firmware protection to add another layer of security underneath the operating system. This includes the System Guard feature.

DRTM:

Dynamic Root of Trust for Measurement (DRTM) allows to establish a new Trusted Computing Base (TCB) at runtime that excludes any software executed previously and thus mitigates the impact of any exploited/malicious platform/device firmware in OS boot path.

AMD Processors supports CPU instruction referred to as SKINIT: The CPU cores are placed in a known state and measures the code that it will run. Before being measured, this code is protected so that it cannot be tampered. This provides assurance that the code that is measured is the code that is executed.

In AMD Platform, necessary AMD software components required for DRTM Secure Launch is deployed through a Windows DLL called AmdDrtm.dll which is deployable through Microsoft Windows Update (WU) mechanism as well can be pre-installed on the Platform.


SMM Isolation:

SMM isolation (aka SMM supervisor) is mechanism to protect SMM drivers executing non-authorized high privilege instruction, to meet the requirement listed in EEAP document "provide mechanism to protect the SMM page tables from unauthorized modification". In AMD Platform, SMM isolation is achieved with combination of SmmSupervisor UEFI package and DRTM protection.


## SMM Isolation Design goal

We propose the development of a common, open source, and security audited module that executes immediately after control is transferred to the SMI handler after an SMI has occurred. The module is called the SMM Supervisor, and its purpose is to:

  -   Execute the SMI handler logic
  -   Protect system resources from SMI handler access
  -   Protect itself from tampering


The SMM isolation feature will be built based on UEFI PiSmmCpuDxeSmm driver with several modifications to meet our design requirements.

It will create an isolated SMM environment to protect key system security resource (e.g. hypervisor memory) from unauthorized SMM handlers. The isolation will leverage the processor privilege mode and PSP, details as follows:

  -  Add CPL0 CPL3 mode switching in SMM entry code. The entry itself will be running at CPL0 mode, and it will setup the CPL3 environment, including page table, exception handlers, call gate and jump to the entry in PiSmmCpuDxeSmm

  -  Separate SMM entry code as standalone binary

  -  PSP DRTM TA binary will authenticate the SMM entry during DRTM boot.

  -  Create a new SmmSupervisor UEFI driver which used to:
    Install SMM entry code
    report DRTM related information to DRTM TA
    Update page table with CPL0, CPL3 restriction
    Implement GP# exception handler for secure policy controlled CPL3 privilege instruction
    Implement SysCall interface

  -  SMM entry code released in binary form, with AMD Key signed signature used for authentication

  -  SmmSupervisor UEFI driver released in binary form, with AMD Key signed signature used for authentication

  -  Embed DRTM boot required image as separate FFS

There are also several other factors that need be considered in the design: ensure that SMM supervisor is trustworthy, minimize OEM enablement/porting effort, ensure compatibility with SMM Paging feature, ability to turn off in the build process, the impact of performance, the impact of stability etc.

## SMM Supervisor package folder layout
In this section, we are going to describe the real implementation of SMM Isolation. SMM Isolation is implemented as UEFI Package, with multiple major components: SmiEntry, SmmSupervisor UEFI driver, Library, supported tool and binaries.

<Addendum> folder
This folder contains the batch file used to build SmmSupervisor Directory. For details about how to build SmmSupervisor Directory, and embedded this directory as UEFI FFS.

<Binary> folder
Contain the binary used to build SmmSupervisor directory including: Signature for SmmSupervisor UEFI driver, SmmSupvEntry Binary, Signature for SmmSupvEntry Binary, Public portion of the key used to sing SmmSupervisor UEFI driver, and SmmSupvEntry binary, OEM secure policy binary.
In this folder, there is a sub folder named "SmmSupervisor". This folder contain the SmmSupervisor UEFI driver released in binary form.

<Include> folder
This folder includes the header file for library and protocol provided by SmmSupvervisor package
Detail interface please refer to Appendix C SmmSupvervisor package interface.

<Library> folder
This folder contains the Lib class instance of UEFI existing interface, BaseCpuLib, BaseIoLib, BaseLib using Syscall, also contain the implementation of SmmSupervisor package defined Lib class, SmmPolicyGateLib, SysCallLib.

  -  BaseCpuLib, BaseIoLib, BaseLib
  As described in previous section, once SMM isolation feature has been turn on, after "DRTM boot flag" been enabled, all SMM driver will be executed under CPL3, executing privilege instruction directly will causing GP exception occurs due CPL violation. Hence, all privilege instruction request need go through Sys-call interface provided by SmmSupvisor.
  The BaseCpuLib, BaseIoLib, BaseLib under "SmmSupervisorPkg\Library" is the implementation of calling Sys-call interface. All SMM drivers need to declare using these Lib class instances explicitly.

  -  SmmSupervisorExceptionHandlerLib
  SmmSupervisorExceptionHandlerLib is only used for SmmSupv Uefi driver, doesn't require to link with other UEFI driver. It is implemented based on UDK open source version with 1) self-modified code removed for Smm Supervisor driver authentication 2) Enhance the debuggability

  -  MemLogLib, MemLogLibNull
  Currently this Library has only been used by SmmSupervisor driver debug version.
  This is a Library which can output the debug string to memory buffer with RT attribute. The memory buffer is organized with ring buffer, when buffer is full, the old content will be replaced by new content.
  The address of buffer is stored in non-volatile BIOS RAM index  0x80 -0x83 which can be access through IO port CD4/CD5.

  -  SmmErrorReportOemLibNull
  NULL instance of SmmErrorReportOemLib. SmmErrorReportOemLib will be call when SMM Security policy violation occurs. It can be replaced with OEM implemented version.

  -  SmmPolicyGateLib
  A library providing the service if specified IO or MSR is permitted according to OEM secure policy.

  -  SysCallLib
  A library providing the method to call service required CPL0 privilege for driver executed in CPL3, this service is implemented by SmmSupvisor through syscall interfacde. Refer to Appendix A for details of syscall interface.

  -  TscTimerLib
  Currently this Library has only been used by SmmSupervisor driver. It is implemented base on EDKII TscTimerLib with PCD removed and AMD special implementation.

<SmiEntry> folder
This folder contains the source codes for the binary file "Binary\SmmSupvEntry.bin". SmmSupvEntry is the code 1st been executed when SMI triggered, it is responsible for:
  1.Setup the executing environment including processor mode, GDT, Page Table, IDT, stack.
  2.Call the Hook point of SmmSupervisor UEFI driver before and after calling the hook point of PiSmmCpuDxeSmm driver.
  3.Call the hook point of PiSmmCpuDxeSmm driver.
  4.Switch to CPL3 when DRTM boot flag has been set.

<SmmSupervisor> folder

This folder contains the source code for binary "Binary\SmmSupervisor\<Debug |Release>\SmmSupervisor.efi".
SmmSupervisor is UEFI DXE SMM driver, which will implement below task to support SMM isolation requirement:

  1.Install SMM_SUPERVISOR_PROTOCOL, responsible for installing SmmSupvEntry to the address SMMBASE + 0x8000.
  2.Report DRTM Information to PSP FW required to support DRTM boot.
  3.Implement the syscall interface to provide service to execute authorized high privilege instruction for CPL3.
  4.Implement a mechanism to check if specified IO or MSR is permitted according to OEM secure policy.
  5.If DRTM boot flag is turn on
    a.Patch the page table to set security critical region to CPL0 access only.
    b.Perform stack security check.
    c.Reinit GDT Table.
  6.Implement isolated exception handler, with updated GP handler to handler CPL violation.

< SupervisorPolicyTools> folder
Contain the python script used to generate "SmmSupvOemSecurePolicy.bin", for more details please refer to 5.2Build Secure policy binary.

## SMM driver requirement

No high privilege instruction

Once SMM isolation feature has been turn on, after "DRTM boot flag" been enabled, all SMM driver will be executed under CPL3, executing privilege instruction directly will causing GP exception occurs due CPL violation.
Privilege instruction includes access MSR, IO, CR register and etc. All SMM driver need update to use BaseCpuLib, BaseIoLib, BaseLib provided by "SmmSupervisorPkg\Library".

Critical memory region including:
  1.Page table memory region.
  2.CPL0 Stack.
  3.CPL0 Heaps.
  4.SMI Entry region.
  5.SmmSupervisor driver memory region.
  6.Memory used to hold GDT.
  7.Memory used to hold exception handlers.
  8.SMM save state region.
Access any of above region will cause PF exeception.

## Steps of enabling SMM Isolation & DRTM

1. Add SmmSupervisorPkg
  - Get SmmSupervisorPkg from released PI package
  - Include SmmSupervisorPkg.inc.dsc in platform dsc file
  - Include SmmSupervisorPkg.inc.fdf to platform fdf file
  - Include SmmSupervisorPkg/SmmSupervisorPkg.dec to the inf file which may refer the protocol or library SmmSupervisorPkg provided
  - Add build macros to project.dsc file (if not specified, release build will be used by default)
    - SMMSUPV_SOURCE_BUILD
      - TRUE - enable source level build (Only for internal debug purpose), SMMSUPV_BIN_DEBUG_VERSION will be ignored
      - FALSE - use binary release build (Default)
    - SMMSUPV_BIN_DEBUG_VERSION
        - TRUE - Debug version binary build
        - FALSE - Release version binary build (Default)


2. Build SmmSupv Directory
Refer to "SmmSupervisorPkg\Addendum\SmmSupvBuild.bat" for how to use "BuildPspDirectory" tool to SmmSupv Directory, section "Build SmmSupervisor directory" describe the details steps of building this directory.

3. Embedded SmmSupv Directory FFS to FV

Add SmmSupv Directory binary to any FV volume, suggest put it to the same FV as SmmSupervisor UEFI driver. Note, the GUID need be fixed, or else it will not be recognized.
e.g.

  "FILE FREEFORM = 83e1f409-21a3-491d-a415-b163a153776d {
      SECTION RAW = $(WORKSPACE)/Build/$(PROJECT_PKG)/SmmSupv/Bin/SmmSupv.bin
  }"

4. Update Libs for SMM drivers
All SMM driver need update to use BaseCpuLib, BaseIoLib, BaseLib provided by "SmmSupervisorPkg\Library".
It has already been done in SmmSupervisorPkg.inc.dsc as below:
However, there are still some driver execute privilege instruction through its own implemented Library, this portion codes need be update, or else GP# exception will be triggered when CPL violation occurs.

5. Update PiSmmCpuDxeSmm driver
   a. Add SmmSupervisorPkg/Addendum/SmiRendezvous.nasm to PiSmmCpuDxeSmm\X64
   b. SmmSupvEntry.bin has some requirement of GDT table ordering and attribute, refer to SmmSupervisorPkg/Addendum/SmiException.nasm update the one in PiSmmCpuDxeSmm/X64
   c. Update inf file
   d. Locate SmmSupv protocol in driver entry like below, the "SMM_ISOLATION_SUPPORT" build switch is used to demonstrate how to create a build token to switch on/off this feature.
      "PcdSmmIsolationSupport" is used to demonstrate how to use a PCD to switch on/off this feature.
   e. PiSmmCpuDxeSmm\SmramSaveState.c
      ----Update routine GetSmiHandlerSize
      "if (Size != 0) {
        return Size;
      }
      if (SmmSupervisor != NULL) {
        return SmmSupervisor->SmiHandlerSize;
      }
      else {
      return gcSmiHandlerSize;
      }"

      ----Update routine InstallSmiHandler
      "if (SmmSupervisor != NULL) {
        SmmSupervisor->InstallSmiHandler(
          CpuIndex,
          SmBase,
          SmiStack,
          StackSize,
          Cr3,
          GdtBase,
          GdtSize,
          (UINT64)&gSmiHandlerIdtr,
          (UINT64)&SmiRendezvousEntry,
          (UINT8 **)&SmmEntryAddr,
          &SmmEntrySize
        );
      }
      if (SmmEntryAddr != NULL) {
        FreePool(SmmEntryAddr);
      }"

6. SMI entry binary use fixed value to access segment descriptor, please make sure segment definition initialed by PiSmmCpuDxeSmm is aligned with "Addendum\SmiException.nasm"

7. Update the code accessing SMM save state region
e.g.
if (NeedSysCall () == TRUE) {
    *(UINT32*)Buffer = (UINT32) SysCall (SMM_SC_SVST_READ, CpuIndex, (UINTN) ((UINT8 *)CpuSaveState + mSmmCpuWidthOffset[RegisterIndex].Offset32), Width);
} else {
    CopyMem(Buffer, (UINT8 *)CpuSaveState + mSmmCpuWidthOffset[RegisterIndex].Offset32, Width);
}

8. Ensure PSP DRTM TA FW is included in the PSP directory
   "<!-- DRTM-TA -->
   <IMAGE_ENTRY Type="0x47" File="dr_drtm_out_prod.sbin"/>"


## Additional Steps of enabling SMM 3.X
In order to open source *SmmSupervisorPkg*, we separate AMD specific implementation from SmmSupervisorPkg with introducing the new *AmdSupervisorPkg*. All AMD specific implementation will be moved to this new package.
Below are steps of enabling SMM LEVEL 20/30 use the SmmSupervisor version 3 series in addition to previous section **Steps of enabling SMM Isolation & DRTM**.

1.	Copy AmdSupervisorPkg from PI package to project path
2.  Check AGESA .inc.dsc file, make sure it has **SMM_ISOLATION_LEVEL30_SUPPORT** included.**SMM_ISOLATION_LEVEL30_SUPPORT**
3.	Define **SMM_ISOLATION_LEVEL30_SUPPORT** to "TRUE" in platform build, this macro will be consumed by AGESA to select Lib instance as below. As start from SMM30, we will block the MMIO access to DRTM interface.
      ```
        !if $(SMM_ISOLATION_LEVEL30_SUPPORT) == TRUE
          AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLibSmmIso/AmdPspMmioLib.inf
        !else
          AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
        !endif
      ```
4.	For platform which include non-UEFI standard driver type "COMBINED_SMM_DXE", below lines need be added manually project dsc file
      ```
        + [LibraryClasses.common.COMBINED_SMM_DXE]
        +!if $(SMM_ISOLATION_LEVEL30_SUPPORT) == TRUE
        +  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLibSmmIso/AmdPspMmioLib.inf
        +!else
        +  AmdPspMmioLib|AgesaModulePkg/Library/AmdPspMmioLib/AmdPspMmioLib.inf
        +!endif
      ```
5. Some files have been move to AmdSupervisorPkg, Rename the **SmmSupervisorPkg** package to **AmdSupervisorPkg**, as below
   * `!include SmmSupervisorPkg\SmmSupervisorPkg.inc.dsc` => `!include AmdSupervisorPkg\AmdSupervisorPkg.inc.dsc`
   * `SmmSupervisorPkg\Addendum\SmmSupvBuild.bat` => `AmdSupervisorPkg\Addendum\SmmSupvBuild.bat`
   * `SmmSupervisorPkg\Binary` => `AmdSupervisorPkg\Binary`
6. Use *Binary\SmmSupvOemSecurePolicy.bin*, which generated from *SmmIsolationPoliciesExample.xml*, below are the required policy item for SMM30
  ```xml
    <PolicyEntry>
      <!-- MSRC001_1010 (Core::X86::Msr::BT_CTL) -->
      <StartAddress Value="0xC0011010"/> <Size Value="0x1"/><SecurityAttributes Value="Read | Write | Execute" />
    </PolicyEntry>
    <PolicyEntry>
      <!-- MSRC001_1024 (Core::X86::Msr::DBG_CTL_MSR2) -->
      <StartAddress Value="0xC0011024"/> <Size Value="0x1"/><SecurityAttributes Value="Read | Write | Execute" />
    </PolicyEntry>
    <PolicyEntry>
      <!-- MSRC001_1018 (Core::X86::Msr::EXCP_BP_CTL) -->
      <StartAddress Value="0xC0011018"/> <Size Value="0x1"/><SecurityAttributes Value="Read | Write | Execute" />
    </PolicyEntry>
  ```
  ```xml
  <SmmCategory name="SAVESTATE">
    <!-- All save state policy entries listed here are allowed -->
    <PolicyAccessAttribute Value="Allow"/>
    <!-- Save state policies required for level 30s start -->
    <PolicyEntry>
        <!-- Allow Read on trapped IO port Write for RAX -->
        <SaveStateField Value="RAX" /> <SecurityAttributes Value="LimitedRead" /> <AccessCondition Value="IoWrite" />
    </PolicyEntry>

    <PolicyEntry>
        <!-- Allow Read for IO_TRAP -->
        <SaveStateField Value="IO_TRAP" /> <SecurityAttributes Value="Read" />
    </PolicyEntry>
    <!-- Save state policies required for level 30s end -->
  </SmmCategory>
  ```
7. Use AmdPspMmioLib to access PSP MMIO
8. If SMMSaveState policy has been declared as above, Platforms must configure access to SMM Save State as follows:
   * Read/write access from/to State Save registers should be denied, except:
      * Read access to Core::X86::Smm::TrapOffset - always allowed.
      * Read access to AL/AX/EAX on trapped IO port write (size derived from Core::X86::Smm::TrapOffset)
9. SMM Supervisor 3.x still provide flexibility to support **"SMM Level20"** plus some mandatory requirements bounding with SMM 3.x package:
    1) Only allow read access to limited offset (0x10500 to 0x10B00) of PSP MMIO through **SMM_SC_IHV_SUPV_READ**.
    2) Only allow write access to limited offset (0x10500 to 0x10B00, exclude 0x10A20 to 0x10AAC which used for DRTM interface) of PSP MMIO through **SMM_SC_IHV_SUPV_WRITE**.

    In this case, only need to switch policy file from 3.0 to 2.0, please NOTE **SMM_ISOLATION_LEVEL30_SUPPORT** still needs to be defined as "TRUE" in platform build.


### Disable SMMSaveState policy restriction
SMM version 3 also provide the flexibility to disable the SMM save state read restriction through update policy binary. Remove **entire** node below in *SmmIsolationPoliciesExample.xml*, use *SupervisorPolicyTools\SupervisorPolicyMaker.py* to generate a new policy binary, and replace the existing one in the binary folder.
Using the policy with entire *"SAVESTATE"* node removed, all read access to SMM save state are allowed, which is same as version 1, and version 2.

**NOTE**: Only remove all `<PolicyEntry>` doesn't means remove the restriction, instead denote allow nothing.

**NOTE**: Once you remove entire `SAVESTATE` node or remove the required SAVESTATE policy, you can't get the score 30.

  ```xml
  <SmmCategory name="SAVESTATE">
    <!-- All save state policy entries listed here are allowed -->
    <PolicyAccessAttribute Value="Allow"/>
    <!-- Save state policies required for level 30s start -->
    <PolicyEntry>
        <!-- Allow Read on trapped IO port Write for RAX -->
        <SaveStateField Value="RAX" /> <SecurityAttributes Value="LimitedRead" /> <AccessCondition Value="IoWrite" />
    </PolicyEntry>

    <PolicyEntry>
        <!-- Allow Read for IO_TRAP -->
        <SaveStateField Value="IO_TRAP" /> <SecurityAttributes Value="Read" />
    </PolicyEntry>
    <!-- Save state policies required for level 30s end -->
  </SmmCategory>
  ```

## How to check if Smm Isolation is enabled

1. Check if SMM Isolation is enabled by querying information in SmmSupervisorPublicInfoTable

Code example:

```
SMM_SUPERVISOR_PUBLIC_INFO_TABLE *PublicInfoTable;

SmmGetSystemConfigurationTable (&gSmmSupervisorPublicInfoTableGuid, (VOID
**)&PublicInfoTable);

if (PublicInfoTable == NULL) {

IDS_HDT_CONSOLE_PSP_TRACE ("PublicInfoTable - NULL, cannot get
SupervisorPublicInfoTable\n");

} else {

// if PublicInfoTable->SmmIsolationEnabled is TRUE, it means SmmIsolation is
enabled

IDS_HDT_CONSOLE_PSP_TRACE ("%a SmmIsolationEnabled: %x\n", __FUNCTION__,
PublicInfoTable->SmmIsolationEnabled);

}
```


2. Check if SmmIsolation has been taken effect

Code example:

```
if (NeedSysCall () == TRUE) { // check if SmmIsolation has been taken effect

// do something

}
```


## Copyright

Copyright (C) 2021-2023 Advanced Micro Devices, Inc. All rights reserved.

