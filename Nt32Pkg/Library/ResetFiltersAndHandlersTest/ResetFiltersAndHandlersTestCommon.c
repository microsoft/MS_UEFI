/** @file
  Common test code.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "ResetFiltersAndHandlersTestCommon.h"

VOID
EFIAPI
DumpResetType (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN VOID             *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_ERROR, "ResetHandler: ResetType = %d, ResetStatus = %r, DataSize = 0x%x\n", ResetType, ResetStatus, DataSize));
}




/**
  ResetFilter only to change ResetType in the following loop:
  Cold -> Warm -> Shutdown -> PlatformSpecific
   |                              |
   +-------------<----------------+

  So that the ResetSystem will be called infinitely.
  This to test the max depth check.
**/
VOID
EFIAPI
ChangeResetType (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN VOID             *ResetData OPTIONAL
  )
{
  switch (ResetType) {
  case EfiResetCold:
    ResetWarm ();
    return;

  case EfiResetWarm:
    ResetShutdown ();
    return;

  case EfiResetShutdown:
    ResetPlatformSpecific (0, NULL);
    return;

  case EfiResetPlatformSpecific:
    ResetCold ();
    return;

  default:
    ASSERT (FALSE);
    return;
  }
}

EFI_GUID mResetSubtypeGuids[] = {
  { 0x4faea88, 0xa678, 0x4b6a,{ 0xae, 0xdc, 0xba, 0xf4, 0x83, 0xa, 0x81, 0xbf } },
  { 0xed342987, 0xba22, 0x4408,{ 0x87, 0xab, 0xed, 0x86, 0xdb, 0x40, 0xc3, 0x5e } },
  { 0x4774f863, 0xb43a, 0x4d2c,{ 0x8b, 0x82, 0x84, 0x75, 0x7, 0x81, 0xc7, 0x9c } },
  { 0xc5cfb78b, 0x13cb, 0x4d5d,{ 0x84, 0x2d, 0x69, 0xed, 0x5a, 0x9c, 0x80, 0xac } },
  { 0x73ccb3ef, 0xb8a0, 0x48ee,{ 0x91, 0x9e, 0x8f, 0x80, 0x7b, 0x75, 0xe, 0xfe } },
  { 0x8c7f1e6, 0xd6fe, 0x4f27,{ 0x9b, 0xe7, 0xb2, 0x33, 0xdd, 0xb9, 0xa0, 0x82 } },
  { 0xe24292dd, 0x7a50, 0x4849,{ 0xaf, 0xf9, 0x6f, 0xa9, 0x1, 0x7b, 0xf4, 0xf9 } },
  { 0xd16bc1ec, 0xf65, 0x4071,{ 0x82, 0xee, 0xbd, 0x6b, 0x64, 0x58, 0x9f, 0x5a } },
  { 0x726a7b6c, 0xfa26, 0x49f2,{ 0x8a, 0xee, 0x78, 0xeb, 0xc1, 0x21, 0x9, 0xe1 } },
  { 0x722029c8, 0x6c21, 0x4cbd,{ 0x9f, 0x24, 0x2e, 0x78, 0x8a, 0xd7, 0x2c, 0x74 } }
};


/**
  Reset filter that changes the reset subtype GUID, reset string, and the optional data.
**/
VOID
EFIAPI
ChangeResetSubtypeGuid (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN VOID             *ResetData OPTIONAL
  )
{
  EFI_GUID            *Guid;
  UINTN               Index;
  UINT8               MyResetData[512];
  UINTN               MyResetDataSize;
  CHAR16              MyResetString[20];
  CHAR16              *EndPointer;

  if (ResetType != EfiResetPlatformSpecific) {
    return;
  }
  Guid = GetResetPlatformSpecificGuid (DataSize, ResetData);
  if (Guid == NULL) {
    return;
  }

  //
  // ResetData := <Text representation of Index> + <mResetSubtypeGuids[Index]> + <mResetSubtypeGuids[Index]>
  // The second mResetSubtypeGuids[Index] is for the optinal data.
  //
  StrDecimalToUintnS ((CHAR16 *)ResetData, &EndPointer, &Index);
  ASSERT (Guid == (EFI_GUID *) (EndPointer + 1)); // Make sure End of text is start of GUID.
  ASSERT (Index < ARRAY_SIZE (mResetSubtypeGuids) && CompareGuid (Guid, &mResetSubtypeGuids[Index])); // Make sure Index is valid.
  ASSERT ((UINTN)Guid - (UINTN)ResetData + sizeof (EFI_GUID) + sizeof (EFI_GUID) == DataSize); // Make sure DataSize is valid.
  ASSERT (CompareGuid (Guid + 1, &mResetSubtypeGuids[Index])); // Make sure optional data is valid.

  //
  // Continue to call ResetSystem using next GUID.
  //
  if (Index < ARRAY_SIZE (mResetSubtypeGuids) - 1) {
    DEBUG ((DEBUG_INFO, "Recursive Call ResetPlatformSpecific: Subtype %g -> %g\n", &mResetSubtypeGuids[Index], &mResetSubtypeGuids[Index + 1]));
    Index++;
    UnicodeValueToStringS (MyResetString, sizeof (MyResetString), 0, Index, 0);
    MyResetDataSize = sizeof (MyResetData);
    BuildResetData (&MyResetDataSize, MyResetData, &mResetSubtypeGuids[Index], MyResetString, sizeof (EFI_GUID), &mResetSubtypeGuids[Index]);
    ResetPlatformSpecific (MyResetDataSize, MyResetData);
  } else {
    DEBUG ((DEBUG_INFO, "END GUID is met\n"));
  }
}

VOID
RegisterConformanceTest (
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetFilter
  )
{
  EFI_STATUS        Status;
  UINTN             Index;

  //
  // Conformance test
  //
  Status = ResetFilter->RegisterResetNotify (ResetFilter, NULL);
  ASSERT (Status == EFI_INVALID_PARAMETER);
  Status = ResetFilter->UnregisterResetNotify (ResetFilter, NULL);
  ASSERT (Status == EFI_INVALID_PARAMETER);

  for (Index = 1; Index <= PcdGet32 (PcdMaximumPeiResetNotifies); Index++) {
    Status = ResetFilter->RegisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM) Index);
    ASSERT_EFI_ERROR (Status);
  }
  Status = ResetFilter->UnregisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM)7);
  ASSERT_EFI_ERROR (Status);
  Status = ResetFilter->RegisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM)7);
  ASSERT_EFI_ERROR (Status);
  Status = ResetFilter->RegisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM)7);
  ASSERT (Status == EFI_ALREADY_STARTED);

  for (Index = 1; Index <= PcdGet32 (PcdMaximumPeiResetNotifies); Index++) {
    Status = ResetFilter->UnregisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM)Index);
    ASSERT_EFI_ERROR (Status);
  }
  Status = ResetFilter->UnregisterResetNotify (ResetFilter, (EFI_RESET_SYSTEM)7);
  ASSERT (Status == EFI_INVALID_PARAMETER);
}


VOID
Test1 (
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetFilter,
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetHandler
)
{
  RegisterConformanceTest (ResetFilter);
  RegisterConformanceTest (ResetHandler);

  ResetFilter->RegisterResetNotify (ResetFilter, ChangeResetType);
  ResetHandler->RegisterResetNotify (ResetHandler, DumpResetType);

  //
  // Initiate Cold reset
  //
  ResetCold ();
}

VOID
Test2 (
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetFilter,
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetHandler
  )
{
  UINT8               MyResetData[512];
  UINTN               MyResetDataSize;

  RegisterConformanceTest (ResetFilter);
  RegisterConformanceTest (ResetHandler);

  ResetFilter->RegisterResetNotify (ResetFilter, ChangeResetSubtypeGuid);
  ResetHandler->RegisterResetNotify (ResetHandler, DumpResetType);

  //
  // Initiate ResetSystem using GUID #0.
  //
  MyResetDataSize = sizeof (MyResetData);
  BuildResetData (&MyResetDataSize, MyResetData, &mResetSubtypeGuids[0], L"0", sizeof (EFI_GUID), &mResetSubtypeGuids[0]);
  ResetPlatformSpecific (MyResetDataSize, MyResetData);
}
