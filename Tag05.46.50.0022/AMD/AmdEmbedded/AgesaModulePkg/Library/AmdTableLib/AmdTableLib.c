/*****************************************************************************
 * Copyright (C) 2008-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/
/* $NoKeywords:$ */
/**
 * @file
 *
 *
 * AMD Register Table Related Functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Lib
 * @e \$Revision$   @e \$Date$
 *
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include <Filecode.h>
#include <Library/BaseLib.h>
#include <Library/AmdBaseLib.h>
#include <Library/GnbLib.h>
#include <Library/AmdTableLib.h>
#include <Library/CcxRolesLib.h>
#include <Library/CcxApicLib.h>
#include <Library/PciLib.h>

#define FILECODE LIBRARY_AMDTABLELIB_AMDTABLELIB_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
F_DO_TABLE_ENTRY *DoTableEntry[TableEntryTypeMax] = {
  SetPciEntry,
  SetPciProfileFixupEntry,
  SetPciCpuRevEntry,
  SetMsrEntry,
  SetMsrCpuRevEntry,
  SetMsrCoreRevEntry,
  SetMsrPlatformFeatEntry,
  SetMsrCpuRevPlatformFeatEntry,
  SetSmnEntry,
  SetSmnMultiEntry,
  SetSmuIndexEntry,
  SetSmuIndexProfileFixupEntry,
  CopyBitFieldEntry
};


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * An iterator for all the Family and Model Register Tables.
 *
 * RegisterTableHandle should be set to NULL to begin iteration, the first time the method is
 * invoked.  Register tables can be processed, until this method returns NULL.  RegisterTableHandle
 * should simply be passed back to the method without modification or use by the caller.
 * The table selector allows the relevant tables for different cores to be iterated, if the family separates
 * tables.  For example, MSRs can be in a table processed by all cores and PCI registers in a table processed by
 * primary cores.
 *
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 * @param[in]     Selector                Select whether to iterate over tables for either all cores, primary cores, bsp, ....
 * @param[in]     RegisterTableList       Register table list
 * @param[in,out] RegisterTableHandle     IN: The handle of the current register table, or NULL if Begin.
 *                                        OUT: The handle of the next register table, if not End.
 *
 * @return        The pointer to the next Register Table, or NULL if End.
 */
REGISTER_TABLE
**GetNextRegisterTable (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       UINT32                  Selector,
  IN       REGISTER_TABLE        **RegisterTableList,
  IN OUT   REGISTER_TABLE       ***RegisterTableHandle
  )
{
  REGISTER_TABLE **NextTable;

  ASSERT (StdHeader != NULL);
  ASSERT (Selector < TableCoreSelectorMax);

  NextTable = *RegisterTableHandle;
  if (NextTable == NULL) {
    // Begin
    NextTable = RegisterTableList;
  } else {
    NextTable++;
  }
  // skip if not selected
  while ((*NextTable != NULL) && (((*NextTable)->Selector != Selector))) {
    NextTable++;
  }
  if (*NextTable == NULL) {
    // End
    *RegisterTableHandle = NULL;
  } else {
    // Iterate next table
    *RegisterTableHandle = NextTable;
  }
  return NextTable;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the PCI Register Entry.
 *
 * @TableEntryTypeMethod{::PciRegister}.
 *
 * Make the current core's PCI address with the function and register for the entry.
 * Read - Modify - Write the PCI register, clearing masked bits, and setting the data bits.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The PCI register entry to perform
 *
 */
VOID
SetPciEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  UINT32               PciRegData;
  PCI_TYPE_ENTRY_DATA *PciEntry;

  PciEntry = (PCI_TYPE_ENTRY_DATA *) (*Entry);
  // "Mask nothing" is a bug, even if just by policy.
  ASSERT (PciEntry->Mask != 0);

  PciRegData = PciRead32 (PciEntry->Address.AddressValue);
  PciRegData = PciRegData & (~(PciEntry->Mask));
  PciRegData = PciRegData | PciEntry->Data;
  PciWrite32 (PciEntry->Address.AddressValue, PciRegData);
  // Entry MUST point to next register entry
  (*((PCI_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Performance Profile PCI Register Entry.
 *
 * @TableEntryTypeMethod{::PciProfileFixup}.
 *
 * Check the entry's performance profile features to the platform's and do the
 * PCI register entry if they match.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The Performance Profile register entry to perform
 *
 */
VOID
SetPciProfileFixupEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA *ProfileFixupEntry;
  PCI_TYPE_ENTRY_DATA               *PciEntry;

  ProfileFixupEntry = (PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  ASSERT ((ProfileFixupEntry->TypeFeats.ProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch (Criteria->ProfileFeats.ProfileValue,
                                      ProfileFixupEntry->TypeFeats.ProfileValue)) {
    PciEntry = &ProfileFixupEntry->PciEntry;
    SetPciEntry (StdHeader, Criteria, (UINT8 **) &PciEntry);
  }
  // Entry MUST point to next register entry
  (*((PCI_PROFILE_FIXUP_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Processor Revision Dependent PCI Register Entry.
 *
 * @TableEntryTypeMethod{::PciCpuRev}.
 *
 * Read - Modify - Write the PCI register, clearing masked bits, and setting the data bits,
 * if the current core's revision matches the revision specified in the entry.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The PCI register entry to perform
 *
 */
VOID
SetPciCpuRevEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  PCI_CPU_REV_TYPE_ENTRY_DATA *CpuRevPciEntry;
  UINT32                       PciRegData;

  CpuRevPciEntry = (PCI_CPU_REV_TYPE_ENTRY_DATA *) (*Entry);
  ASSERT (CpuRevPciEntry->Mask != 0);

  if (((Criteria->SocLogicalId.Family & CpuRevPciEntry->CpuRevision.Family) != 0) &&
      ((Criteria->SocLogicalId.Revision & CpuRevPciEntry->CpuRevision.Revision) != 0)) {
    PciRegData = PciRead32 (CpuRevPciEntry->Address.AddressValue);
    PciRegData = PciRegData & (~(CpuRevPciEntry->Mask));
    PciRegData = PciRegData | CpuRevPciEntry->Data;
    PciWrite32 (CpuRevPciEntry->Address.AddressValue, PciRegData);
  }
  // Entry MUST point to next register entry
  (*((PCI_CPU_REV_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the MSR Register Entry.
 *
 * @TableEntryTypeMethod{::MsrRegister}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The MSR register entry to perform
 *
 */
VOID
SetMsrEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  MSR_TYPE_ENTRY_DATA *MsrEntry;

  MsrEntry = (MSR_TYPE_ENTRY_DATA *) (*Entry);
  // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
  ASSERT (MsrEntry->Mask != 0);

  AsmMsrAndThenOr64 (MsrEntry->Address, ~(MsrEntry->Mask), MsrEntry->Data);
  // Entry MUST point to next register entry
  (*((MSR_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Processor Revision Dependent MSR Entry.
 *
 * @TableEntryTypeMethod{::MsrCpuRev}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits,
 * if the current core's revision matches the revision specified in the entry.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The MSR entry to perform
 *
 */
VOID
SetMsrCpuRevEntry (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       UINT8                  **Entry
  )
{
  MSR_CPU_REV_TYPE_ENTRY_DATA *CpuRevMsrEntry;
  MSR_TYPE_ENTRY_DATA         *MsrEntry;

  CpuRevMsrEntry = (MSR_CPU_REV_TYPE_ENTRY_DATA *) (*Entry);
  MsrEntry = &CpuRevMsrEntry->MsrEntry;
  ASSERT (MsrEntry->Mask != 0);

  if (((Criteria->SocLogicalId.Family & CpuRevMsrEntry->CpuRevision.Family) != 0) &&
      ((Criteria->SocLogicalId.Revision & CpuRevMsrEntry->CpuRevision.Revision) != 0)) {
    SetMsrEntry (StdHeader, Criteria, (UINT8 **) &MsrEntry);
  }
  // Entry MUST point to next register entry
  (*((MSR_CPU_REV_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the CPU silicon Revision Dependent MSR Entry.
 *
 * @TableEntryTypeMethod{::MsrCoreRev}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits,
 * if the current core's revision matches the revision specified in the entry.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The MSR entry to perform
 *
 */
VOID
SetMsrCoreRevEntry (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       UINT8                  **Entry
  )
{
  MSR_CORE_REV_TYPE_ENTRY_DATA *CoreRevMsrEntry;
  MSR_TYPE_ENTRY_DATA          *MsrEntry;

  CoreRevMsrEntry = (MSR_CORE_REV_TYPE_ENTRY_DATA *) (*Entry);
  MsrEntry = &CoreRevMsrEntry->MsrEntry;
  ASSERT (MsrEntry->Mask != 0);

  if (((Criteria->CoreLogicalId.CoreFamily & CoreRevMsrEntry->CoreRevision.CoreFamily) != 0) &&
      ((Criteria->CoreLogicalId.CoreRevision & CoreRevMsrEntry->CoreRevision.CoreRevision) != 0)) {
    SetMsrEntry (StdHeader, Criteria, (UINT8 **) &MsrEntry);
  }
  // Entry MUST point to next register entry
  (*((MSR_CORE_REV_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Platform Feature Dependent MSR Entry.
 *
 * @TableEntryTypeMethod{::MsrPlatformFeat}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits,
 * if the current core's platform features matches the features specified in the entry.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The MSR entry to perform
 *
 */
VOID
SetMsrPlatformFeatEntry (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       UINT8                  **Entry
  )
{
  MSR_PLATFORM_FEAT_ENTRY_DATA *PlatformFeatEntry;
  MSR_TYPE_ENTRY_DATA          *MsrEntry;

  PlatformFeatEntry = (MSR_PLATFORM_FEAT_ENTRY_DATA *) (*Entry);

  ASSERT ((PlatformFeatEntry->TypeFeats.PlatformValue & ~((AMD_PF_ALL) | (AMD_PF_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch (Criteria->PlatformFeats.PlatformValue,
                                      PlatformFeatEntry->TypeFeats.PlatformValue)) {
    MsrEntry = &PlatformFeatEntry->MsrEntry;
    SetMsrEntry (StdHeader, Criteria, (UINT8 **) &MsrEntry);
  }
  // Entry MUST point to next register entry
  (*((MSR_PLATFORM_FEAT_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Cpu Rev & Platform Feature Dependent MSR Entry.
 *
 * @TableEntryTypeMethod{::MsrCpuRevPlatformFeat}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits,
 * if the current core's revision matches the revision specified & current core's
 * platform features matches the features specified in the entry.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The MSR entry to perform
 *
 */
VOID
SetMsrCpuRevPlatformFeatEntry (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       UINT8                  **Entry
  )
{
  MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA *CpuRevPlatformFeatEntry;
  MSR_TYPE_ENTRY_DATA                  *MsrEntry;

  CpuRevPlatformFeatEntry = (MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA *) (*Entry);

  ASSERT ((CpuRevPlatformFeatEntry->TypeFeats.PlatformValue & ~((AMD_PF_ALL) | (AMD_PF_AND))) == 0);

  // Check CPU Rev match
  if (((Criteria->SocLogicalId.Family & CpuRevPlatformFeatEntry->CpuRevision.Family) != 0) &&
      ((Criteria->SocLogicalId.Revision & CpuRevPlatformFeatEntry->CpuRevision.Revision) != 0)) {
    // Check platform features match
    if (DoesEntryTypeSpecificInfoMatch (Criteria->PlatformFeats.PlatformValue,
                                        CpuRevPlatformFeatEntry->TypeFeats.PlatformValue)) {
      MsrEntry = &CpuRevPlatformFeatEntry->MsrEntry;
      SetMsrEntry (StdHeader, Criteria, (UINT8 **) &MsrEntry);
    }
  }

  // Entry MUST point to next register entry
  (*((MSR_CPU_REV_PLATFORM_FEAT_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the SMN Register Entry.
 *
 * @TableEntryTypeMethod{::SmnRegisters}.
 *
 * Read - Modify - Write the SMN register, clearing masked bits, and setting the data bits.
 *
 * See if you can use the other entries or make an entry that covers the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model code specific to one case.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The SMN Data entry to perform
 *
 */
VOID
SetSmnEntry (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       UINT8                  **Entry
  )
{
  SMN_TYPE_ENTRY_DATA *SmnEntry;

  if (CcxIsBsp (StdHeader)) {
    SmnEntry = (SMN_TYPE_ENTRY_DATA *) (*Entry);
    // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
    ASSERT (SmnEntry->Mask != 0);

    SetSmnEntryLib (SmnEntry->InstanceId, SmnEntry->RegisterIndex, ~(SmnEntry->Mask), SmnEntry->Data);
  }

  // Entry MUST point to next register entry
  (*((SMN_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the SMN Multi Registers Entry.
 *
 * @TableEntryTypeMethod{::SmnMultiRegisters}.
 *
 * Read - Modify - Write the SMN register, clearing masked bits, and setting the data bits.
 *
 * See if you can use the other entries or make an entry that covers the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model code specific to one case.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The SMN Data entry to perform
 *
 */
VOID
SetSmnMultiEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  UINT32                     InstanceId;
  UINT32                     RegisterIndex;
  UINT32                     AndValue;
  UINT32                     OrValue;
  SMN_MULTI_TYPE_ENTRY_DATA *SmnMultiEntry;

  if (CcxIsBsp (StdHeader)) {
    SmnMultiEntry = (SMN_MULTI_TYPE_ENTRY_DATA *) (*Entry);
    // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
    ASSERT (SmnMultiEntry->Mask != 0);

    InstanceId = SmnMultiEntry->InstanceId;
    AndValue = ~(SmnMultiEntry->Mask);
    OrValue = SmnMultiEntry->Data;
    for (RegisterIndex = SmnMultiEntry->RegisterIndexStart; RegisterIndex <= SmnMultiEntry->RegisterIndexEnd; RegisterIndex += SmnMultiEntry->Steps) {
      SetSmnEntryLib (InstanceId, RegisterIndex, AndValue, OrValue);
    }
  }

  // Entry MUST point to next register entry
  (*((SMN_MULTI_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the SMU Index/Data Register Entry.
 *
 * @TableEntryTypeMethod{::SmuIndexRegister}.
 *
 * Read - Modify - Write the SMU Index/Data register, clearing masked bits, and setting the data bits.
 *
 * See if you can use the other entries or make an entry that covers the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model code specific to one case.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The SMU Index Data entry to perform
 *
 */
VOID
SetSmuIndexEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  UINT32               SmuIndexRegister;
  SMU_INDEX_ENTRY_DATA *SmuIndexEntry;

  SmuIndexEntry = (SMU_INDEX_ENTRY_DATA *) (*Entry);
  // "Mask nothing" is a bug, even if just by policy.
  ASSERT (SmuIndexEntry->Mask != 0);

  // Software must wait for 0xD8230000[31][SVIBUSY] to clear to 0 before writing any of the following fields
  if ((SmuIndexEntry->Index == 0xD8230000) || (SmuIndexEntry->Index == 0xD8230004) || (SmuIndexEntry->Index == 0xD823000C)) {
    do {
      GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, 0xD8230000, AccessWidth32, &SmuIndexRegister, StdHeader);
    } while ((SmuIndexRegister & BIT31) != 0);
  }

  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndexEntry->Index, AccessWidth32, &SmuIndexRegister, StdHeader);
  SmuIndexRegister = SmuIndexRegister & (~(SmuIndexEntry->Mask));
  SmuIndexRegister = SmuIndexRegister | SmuIndexEntry->Data;
  GnbLibPciIndirectWrite (SMU_INDEX_ADDRESS, SmuIndexEntry->Index, AccessWidth32, &SmuIndexRegister, StdHeader);
  // Entry MUST point to next register entry
  (*((SMU_INDEX_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Performance Profile SMU Index/Data Register Entry.
 *
 * @TableEntryTypeMethod{::SmuIndexProfileFixup}.
 *
 * Check the entry's performance profile features to the platform's and do the
 * SMU Index/Data register entry if they match.
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The Performance Profile register entry to perform
 *
 */
VOID
SetSmuIndexProfileFixupEntry (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       UINT8                  **Entry
  )
{
  SMU_INDEX_ENTRY_DATA               *SmuIndexEntry;
  SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA *ProfileFixupSmuIndexEntry;

  ProfileFixupSmuIndexEntry = (SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  ASSERT ((ProfileFixupSmuIndexEntry->TypeFeats.ProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  if (DoesEntryTypeSpecificInfoMatch (Criteria->ProfileFeats.ProfileValue,
                                      ProfileFixupSmuIndexEntry->TypeFeats.ProfileValue)) {
    SmuIndexEntry = &(ProfileFixupSmuIndexEntry->SmuIndexEntry);
    SetSmuIndexEntry (StdHeader, Criteria, (UINT8 **) &SmuIndexEntry);
  }
  // Entry MUST point to next register entry
  (*((SMU_INDEX_PROFILE_FIXUP_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Copy Bitfield Entry.
 *
 * @TableEntryTypeMethod{::CopyBitField}.
 *
 * Copy bitfield from register A to register B
 *
 * @param[in]     StdHeader         Config handle for library and services.
 * @param[in]     Criteria          Info on the installed config for entry matching.
 * @param[in]     Entry             The Performance Profile register entry to perform
 *
 */
VOID
CopyBitFieldEntry (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                 **Entry
  )
{
  UINT8                    *DestEntry;
  UINT64                    SourceData;
  UINT64                    SourceMask;
  UINT64                    DestData;
  UINT64                    DestMask;
  UINTN                     PciAddress;
  TABLE_ENTRY_DATA          DestEntryStruct;
  COPY_BIT_FIELD_DESCRIPTION *Source;
  COPY_BIT_FIELD_DESCRIPTION *Destination;

  Source = &(((COPY_BIT_FIELD_ENTRY_DATA *) (*Entry))->Source);
  Destination = &(((COPY_BIT_FIELD_ENTRY_DATA *) (*Entry))->Destination);
  // Get source data
  switch (Source->RegType) {
  case MsrRegister:
    SourceData = AsmReadMsr64 (Source->Address);
    break;
  case PciRegister:
    PciAddress = Source->Address;
    SourceData = PciRead32 (PciAddress);
    break;
  case SmuIndexRegister:
    GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, Source->Address, AccessWidth32, &SourceData, StdHeader);
    break;
  default:
    ASSERT (FALSE);
    return;
  }

  // Check MSB, LSB, and get SourceMask, SourceData
  if (Source->MSB >= Source->LSB) {
    SourceMask = LShiftU64 (1, (Source->MSB - Source->LSB + 1)) - 1;
    SourceData  = RShiftU64 (SourceData, Source->LSB) & SourceMask;
  } else {
    // The input MSB LSB are wrong. exchange them
    ASSERT (FALSE);
    SourceMask = LShiftU64 (1, (Source->LSB - Source->MSB + 1)) - 1;
    SourceData  = RShiftU64 (SourceData, Source->MSB) & SourceMask;
  }

  // Check MSB, LSB, and get DestMask, DestData
  if (Destination->MSB >= Destination->LSB) {
    DestMask = LShiftU64 (1, (Destination->MSB - Destination->LSB + 1)) - 1;
    DestMask = LShiftU64 (DestMask, Destination->LSB);
    DestData = LShiftU64 (SourceData, Destination->LSB);
  } else {
    // The input MSB LSB are wrong. exchange them
    ASSERT (FALSE);
    DestMask = LShiftU64 (1, (Destination->LSB - Destination->MSB + 1)) - 1;
    DestMask = LShiftU64 (DestMask, Destination->MSB);
    DestData = LShiftU64 (SourceData, Destination->MSB);
  }
  if (SourceMask > DestMask) {
    ASSERT (FALSE);
    return;
  }

  // Set destination data
  switch (Destination->RegType) {
  case MsrRegister:
    DestEntryStruct.MsrEntry.Address = Destination->Address;
    DestEntryStruct.MsrEntry.Data = DestData;
    DestEntryStruct.MsrEntry.Mask = DestMask;
    break;
  case PciRegister:
    DestEntryStruct.PciEntry.Address.AddressValue = Destination->Address;
    DestEntryStruct.PciEntry.Data = (UINT32) DestData;
    DestEntryStruct.PciEntry.Mask = (UINT32) DestMask;
    break;
  case SmuIndexRegister:
    DestEntryStruct.SmuIndexEntry.Index = Destination->Address;
    DestEntryStruct.SmuIndexEntry.Data = (UINT32) DestData;
    DestEntryStruct.SmuIndexEntry.Mask = (UINT32) DestMask;
    break;
  default:
    ASSERT (FALSE);
    return;
  }
  DestEntry = (UINT8 *) &DestEntryStruct;

  DoTableEntry[Destination->RegType] (StdHeader, Criteria, (UINT8 **) &DestEntry);
  // Entry MUST point to next register entry
  (*((COPY_BIT_FIELD_ENTRY_DATA **)Entry))++;
}

BOOLEAN
SpecialFuseCheck (
  IN       UINT32 Addr,
  IN       UINT32 BitLsb,
  IN       UINT32 BitMsb,
  IN       UINT32 ExpectedValue
  )
{
  UINT32   Value;
  UINTN    PciAddr;

  PciAddr = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
  PciWrite32 (PciAddr, Addr);

  PciAddr = MAKE_SBDFO (0, 0, 0, 0, 0xBC);
  Value = PciRead32 (PciAddr);

  Value >>= BitLsb;
  Value &= (1 << (BitMsb - BitLsb + 1)) - 1;

  return (Value == ExpectedValue);
}

/* -----------------------------------------------------------------------------*/
/**
 * Returns the platform features list of the currently running processor core.
 *
 * @param[in]       StdHeader         Config handle for library and services.
 * @param[out]      Features          The Features supported by this platform
 *
 */
VOID
GetBasePlatformFeatures (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       SOC_LOGICAL_ID         *SocLogicalId,
     OUT   PLATFORM_FEATS         *Features
  )
{
  UINT32          PackageType;
  PLATFORM_FEATS  PlatformFeats;
  UINT8           ApicMode;

  PlatformFeats.PlatformValue = 0;

  // bit 0 - PlatformAm4:   AM4 package
  // bit 1 - PlatformSP3r3: SP3r3 package
  // bit 2 - PlatformSP3Ws: SP3 workstation package
  // bit 3 - PlatformSP3:   SP3 package
  PackageType = LibAmdGetPackageType (StdHeader);
  switch (PackageType) {
  case PACKAGE_TYPE_AM4:
    PlatformFeats.PlatformFeatures.PlatformAm4 = 1;
    break;
  case PACKAGE_TYPE_SP3R3:
    PlatformFeats.PlatformFeatures.PlatformSp3r3 = 1;
    break;
  case PACKAGE_TYPE_SP3:
    if (SocLogicalId->Family == AMD_FAMILY_17_SSP) {
      if (SpecialFuseCheck (0x5D484, 24, 31, 0x40)) {
        PlatformFeats.PlatformFeatures.PlatformSp3Ws = 1;
      } else {
        PlatformFeats.PlatformFeatures.PlatformSp3 = 1;
      }
    } else {
      PlatformFeats.PlatformFeatures.PlatformSp3 = 1;
    }
  }

  // bit 4 - PlatformX2Apic: x2APIC mode desired
  // bit 5 - PlatformApic: APIC mode desired
  ApicMode = PcdGet8 (PcdAmdApicMode);
  if (ApicMode == xApicMode) {
    PlatformFeats.PlatformFeatures.PlatformApic = 1;
  } else if (ApicMode == x2ApicMode) {
    PlatformFeats.PlatformFeatures.PlatformX2Apic = 1;
  }

  // bit 6 - PlatformDisIbrs: IBRS (Indirect Branch Prediction Speculation) should be disabled
  PlatformFeats.PlatformFeatures.PlatformDisIbrs = PcdGetBool (PcdAmdIbrsEn) ? 0 : 1;

  // bit 7 - PlatformEmulation: In emulation environment
  PlatformFeats.PlatformFeatures.PlatformEmulation = (PcdGet32(PcdAmdEnvironmentFlag) & 0x0F) == 0 ? 0 : 1;

  // bit 8 - PlatformBranchSampling: Branch Sampling desired
  PlatformFeats.PlatformFeatures.PlatformBranchSampling = (PcdGetBool (PcdAmdBranchSampling)) ? 1 : 0;

  Features->PlatformValue = PlatformFeats.PlatformValue;
}

/*-------------------------------------------------------------------------------------*/
/**
 * Returns the performance profile features list of the currently running processor core.
 *
 * @param[in]       StdHeader         Header for library and services
 * @param[out]      Features          The performance profile features supported by this platform
 *
 */
VOID
GetBasePerformanceFeatures (
  IN       AMD_CONFIG_PARAMS  *StdHeader,
     OUT   PROFILE_FEATS      *Features
  )
{
  // Start with none.
  Features->ProfileValue = 0;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Checks register table entry type specific criteria to the platform.
 *
 * Entry Data Type implementer methods can use this generically to check their own
 * specific criteria.  The method collects the actual platform characteristics and
 * provides them along with the table entry's criteria to this service.
 *
 * There are a couple considerations for any implementer method using this service.
 * The criteria value has to be representable as a UINT32.  The MSB, Bit 31, has to
 * be used as a AND test request if set in the entry.  (The platform value should never
 * have that bit set.)
 *
 * @param[in]     PlatformTypeSpecificFeatures        The platform features
 * @param[in]     EntryTypeFeatures                   The entry's desired platform features
 *
 * @retval        TRUE                                This entry should be applied
 * @retval        FALSE                               This entry does not apply
 *
 */
BOOLEAN
DoesEntryTypeSpecificInfoMatch (
  IN       UINT32   PlatformTypeSpecificFeatures,
  IN       UINT32   EntryTypeFeatures
  )
{
  BOOLEAN Result;

  Result = FALSE;

  if ((EntryTypeFeatures & BIT31) == 0) {
    // Match if ANY entry feats match a platform feat (an OR test)
    if ((EntryTypeFeatures & PlatformTypeSpecificFeatures) != 0) {
      Result = TRUE;
    }
  } else {
    // Match if ALL entry feats match a platform feat (an AND test)
    if ((EntryTypeFeatures & ~(BIT31)) == (EntryTypeFeatures & PlatformTypeSpecificFeatures)) {
      Result = TRUE;
    }
  }
  return Result;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Determine this core's Selector matches.
 *
 * @param[in]  StdHeader   Config handle for library and services.
 * @param[in]  Selector    The selector type for the table.
 *
 * @retval  TRUE           The current core matches the selector type
 * @retval  FALSE          The current core does not match the selector type
 */
BOOLEAN
IsCoreSelector (
  IN       AMD_CONFIG_PARAMS        *StdHeader,
  IN       UINT32                    Selector
  )
{
  BOOLEAN Result;

  Result = TRUE;
  ASSERT (Selector < TableCoreSelectorMax);

  if ((Selector == PrimaryCores) && !CcxIsSocketPrimary (StdHeader)) {
    Result = FALSE;
  }
  if ((Selector == DiePrimary) && !CcxIsDiePrimary (StdHeader)) {
    Result = FALSE;
  }
  if ((Selector == ComplexPrimary) && !CcxIsComplexPrimary (StdHeader)) {
    Result = FALSE;
  }
  if ((Selector == ComputeUnitPrimary) && !CcxIsComputeUnitPrimary (StdHeader)) {
    Result = FALSE;
  }
  if ((Selector == BscCore) && (!CcxIsBsp (StdHeader))) {
    Result = FALSE;
  }
  return Result;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the registers for this core based on entries in a list of Register Tables.
 *
 * Determine the platform features and this core's logical id.  Get the specific table
 * entry type implementations for the logical model, which may be either generic (the ones
 * in this file) or specific.
 *
 * Scan the tables starting the with ones for all cores and progressively narrowing the selection
 * based on this core's role (ex. primary core).  For a selected table, check for each entry
 * matching the current core and platform, and call the implementer method to perform the
 * register set operation if it matches.
 *
 * @param[in]  StdHeader                   Config handle for library and services.
 * @param[in]  Criteria                    Info on the installed config for entry matching.
 * @param[in]  RegisterEntry               RegisterEntry
 *
 */
VOID
SetRegistersFromTable (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       ENTRY_CRITERIA         *Criteria,
  IN       UINT8                  *RegisterEntry
  )
{
  TABLE_ENTRY_FIELDS    *Entries;
  TABLE_ENTRY_DATA      *EntryData;
  UINT16                 EntryType;

  // Entries Format:
  //
  // 0: UINT16          EntryType
  //    VariableLength  EntryData
  // 1: UINT16          EntryType
  //    VariableLength  EntryData
  // ...
  // ...
  Entries = (TABLE_ENTRY_FIELDS *) RegisterEntry;                    // Get the first entry
  EntryType = Entries->EntryType;                                    // Get EntryType
  EntryData = &(Entries->EntryData);                                 // Get EntryData block
  while (EntryType != TableTerminator) {
    if (EntryType < TableEntryTypeMax) {
      // EntryData will be added with correct size by DoTableEntry ()
      // After that, it points to the next entry
      DoTableEntry[EntryType] (StdHeader, Criteria, (UINT8 **) &EntryData);
      Entries = (TABLE_ENTRY_FIELDS *) EntryData;                    // Get the next entry
      EntryType = Entries->EntryType;                                // Get EntryType
      EntryData = &(Entries->EntryData);                             // Get EntryData
    } else {
      ASSERT (FALSE);
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the registers for this core based on entries in a list of Register Tables.
 *
 * Determine the platform features and this core's logical id.  Get the specific table
 * entry type implementations for the logical model, which may be either generic (the ones
 * in this file) or specific.
 *
 * Scan the tables starting the with ones for all cores and progressively narrowing the selection
 * based on this core's role (ex. primary core).  For a selected table, check for each entry
 * matching the current core and platform, and call the implementer method to perform the
 * register set operation if it matches.
 *
 * @param[in]  StdHeader         Config handle for library and services.
 * @param[in]  Criteria          Info on the installed config for entry matching.
 * @param[in]  RegisterTableList RegisterTableList
 *
 */
VOID
SetRegistersFromTableList (
  IN       AMD_CONFIG_PARAMS       *StdHeader,
  IN       ENTRY_CRITERIA          *Criteria,
  IN       REGISTER_TABLE         **RegisterTableList
  )
{
  TABLE_CORE_SELECTOR    Selector;
  REGISTER_TABLE       **TableHandle;
  REGISTER_TABLE       **RegisterTable;

  for (Selector = AllCores; Selector < TableCoreSelectorMax; Selector++) {
    if (IsCoreSelector (StdHeader, Selector)) {
      // If the current core is the selected type of core, work the table list for tables for that type of core.
      TableHandle = NULL;
      RegisterTable = GetNextRegisterTable (StdHeader, Selector, RegisterTableList, &TableHandle);
      while (*RegisterTable != NULL) {
        SetRegistersFromTable (StdHeader, Criteria, (UINT8 *) ((*RegisterTable)->Table));
        RegisterTable = GetNextRegisterTable (StdHeader, Selector, RegisterTableList, &TableHandle);
      }
    } else {
      // Once a selector does not match the current core, quit looking.
      break;
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the registers for this core based on entries in a list of Register Tables.
 *
 * This function acts as a wrapper for calling the SetRegistersFromTable.
 *
 *  @param[in]   StdHeader             Config handle for library and services.
 *  @param[in]   RegTableListAtGivenTP Points to the register table list at the given time point.
 *  @param[in]   TimePoint             Point in time of execution.
 *
 *  @retval      AGESA_SUCCESS         Register table successfully loaded
 *  @retval      AGESA_UNSUPPORTED     Register table is not supported
 */
AGESA_STATUS
SetRegistersFromTablesAtGivenTimePoint (
  IN       AMD_CONFIG_PARAMS          *StdHeader,
  IN       REGISTER_TABLE_AT_GIVEN_TP *RegTableListAtGivenTP,
  IN       REGISTER_TABLE_TIME_POINT   TimePoint
  )
{
  REGISTER_TABLE            **RegisterTableList;
  AGESA_STATUS                Status;
  ENTRY_CRITERIA              Criteria;

  if (TimePoint >= MaxAmdRegisterTableTps) {
    ASSERT (FALSE);
    return AGESA_BOUNDS_CHK;
  }

  RegisterTableList = NULL;
  while (RegTableListAtGivenTP->TimePoint != MaxAmdRegisterTableTps) {
    if (RegTableListAtGivenTP->TimePoint == TimePoint) {
      RegisterTableList = (REGISTER_TABLE **) RegTableListAtGivenTP->TableList;
      break;
    }
    RegTableListAtGivenTP++;
  }
  if (RegisterTableList != NULL) {
    // Prepare criteria for entry matching
    GetPlatformFeatures (StdHeader, &Criteria.PlatformFeats);
    GetPerformanceFeatures (StdHeader, &Criteria.ProfileFeats);
    GetSocLogicalIdOnCurrentCore (StdHeader, &Criteria.SocLogicalId);
    GetCoreLogicalIdOnCurrentCore (StdHeader, &Criteria.CoreLogicalId);

    // Process table list
    SetRegistersFromTableList (StdHeader, &Criteria, RegisterTableList);
    Status = AGESA_SUCCESS;
  } else {
    Status = AGESA_UNSUPPORTED;
  }
  return Status;
}



