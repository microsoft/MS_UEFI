/** @file -- PeiResetSystemCoreLib.c
This file contains the PEI-specific portions of the the ResetSystemCore
library.

Copyright (c) 2016, Microsoft Corporation

All rights reserved.
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

#include <Uefi/UefiBaseType.h>              // EFI_GUID, etc.
#include <Uefi/UefiSpec.h>                  // EFI_RESET_SYSTEM, etc.

#include <Ppi/ResetSystemPpi.h>
#include <Ppi/ResetNotify.h>

#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/ResetSystemCoreLib.h>     // Must come after ResetSystemPpi.h

#include "InternalResetSystemCoreLib.h"

//================================================================================
// INTERNAL STATE - for Execute In Place
//================================================================================
#pragma pack(push, 1)
typedef struct  _RESET_PEI_INTERNAL_STATE
{
  BOOLEAN             AlreadyNotified;
} RESET_PEI_INTERNAL_STATE;
typedef struct  _RESET_PEI_INTERNAL_STATE_HOB
{
  EFI_HOB_GUID_TYPE         HobType;
  RESET_PEI_INTERNAL_STATE  State;
} RESET_PEI_INTERNAL_STATE_HOB;
#pragma pack(pop)
// {DE6C4D05-ED6D-468C-AB3D-60A9B2F89B8F}
CONST EFI_GUID        mResetSystemPeiInternalStateHobGuid = {
  0xde6c4d05, 0xed6d, 0x468c, { 0xab, 0x3d, 0x60, 0xa9, 0xb2, 0xf8, 0x9b, 0x8f }
};
//================================================================================


/**
  This function just makes it prettier to find the internal state.

  @retval     InternalState pointer or NULL if not found.

**/
STATIC
RESET_PEI_INTERNAL_STATE_HOB*
GetInternalState (
  VOID
  )
{
  return (RESET_PEI_INTERNAL_STATE_HOB*)GetFirstGuidHob( &mResetSystemPeiInternalStateHobGuid );
} // GetInternalState()


/**
  This function will attempt to create a default version of the RESET_PEI_INTERNAL_STATE
  HOB, if one does not already exist.

  @retval     EFI_SUCCESS   HOB successfully created with default values.
  @retval     EFI_ABORTED   HOB could not be created.

**/
STATIC
EFI_STATUS
InitializeInternalStateHob (
  VOID
  )
{
  EFI_STATUS                    Status;
  VOID                          *HobList;
  RESET_PEI_INTERNAL_STATE_HOB  *InternalState;

  //
  // First, make sure that HOB pointers have been set up.
  Status = PeiServicesGetHobList( &HobList );
  if (EFI_ERROR( Status ) || HobList == NULL)
  {
    DEBUG(( DEBUG_INFO, __FUNCTION__" - HOB list not ready!\n" ));
    Status = EFI_ABORTED;
    goto Exit;
  }

  //
  // Next, check to see whether the HOB has already been created.
  InternalState = GetInternalState();
  // If InternalState already exists, we're done.
  if (InternalState != NULL)
  {
    Status = EFI_SUCCESS;
    goto Exit;
  }

  //
  // Next, allocate and configure the internal state HOB.
  Status = PeiServicesCreateHob( EFI_HOB_TYPE_GUID_EXTENSION,
                                 sizeof( *InternalState ),
                                 (VOID**)&InternalState );
  // If we couldn't create it... that's no bueno.
  if (EFI_ERROR( Status ))
  {
    // We can safely bail here.
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Could not allocate HOB for internal state! %r\n", Status ));
    Status = EFI_ABORTED;
    goto Exit;
  }

  //
  // If we're here, we created the HOB successfully. Let's set that mother up.
  CopyMem( &InternalState->HobType.Name, &mResetSystemPeiInternalStateHobGuid, sizeof( EFI_GUID ) );
  InternalState->State.AlreadyNotified  = FALSE;
  Status = EFI_SUCCESS;

Exit:
  return Status;
} // InitializeInternalStateHob()


/**
  This is an internal wrapper for EfiAtRuntime() so that
  PEI and DXE can share business logic.
  The PEI version returns FALSE.
  The DXE version returns EfiAtRuntime().

  @retval     TRUE
  @retval     FALSE

**/
BOOLEAN
InternalAtRuntime (
  VOID
  )
{
  // For PEI, the answer is "No".
  return FALSE;
} // InternalAtRuntime()


/**
  This function determines whether a PPI is defined for a
  EfiResetPlatformSpecific-type reset.
  If found, it will return the PPI, otherwise it will return NULL.

  IMPORTANT NOTE: The PPI and Protocol interfaces are identical. For simplicity,
                  this function and the business logic that consumes it will refer
                  to the Protocol structure.

  ASSERT if more than one PPI is installed for a given type. In release
  builds, just return the first PPI found.

  @param[in]  ResetGuid         Pointer to EFI_GUID representing the platform-specific
                                PPI being located.

  @retval     Pointer           Pointer to the PPI, if found.
  @retval     NULL              No PPI was found.

**/
PLATFORM_SPECIFIC_RESET_PROTOCOL*
GetPlatformSpecificProtocol (
  IN CONST EFI_GUID               *ResetGuid
  )
{
  EFI_STATUS                          Status;
  PLATFORM_SPECIFIC_RESET_PROTOCOL    *PlatformSpecificReset = NULL;
  VOID                                *DummyPointer = NULL;

  // If no GUID is provided, let's get out of here.
  if (ResetGuid == NULL)
  {
    goto Exit;
  }

  //
  // First, we'll attempt to locate ANY instance of the platform-specific reset PPI.
  Status = PeiServicesLocatePpi( ResetGuid,
                                 0,
                                 NULL,
                                 &PlatformSpecificReset );
  // If there was an error, we can't proceed.
  if (EFI_ERROR( Status ))
  {
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - PeiServicesLocatePpi = %r\n", Status ));
    goto Exit;
  }

  //
  // If that worked, now is as good a time as any to make sure that no one else
  // has doubled up on the instances.
  // Attempt to locate a second instance.
  Status = PeiServicesLocatePpi( ResetGuid,
                                 1,
                                 NULL,
                                 &DummyPointer );
  // If we found too many instances, let's take a look at that...
  // Probably the platform code needs to be revised.
  // NOTE: This step only means anything in debug mode.
  if (!EFI_ERROR( Status ))
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Too many PPIs for platform-specific reset %g!\n", ResetGuid ));
    ASSERT( FALSE );
    // If we're not in debug mode, fall through so that a system reset can be attempted.
    // UEFI spec indicates that every attempt should be made to reset the system on request.
  }

Exit:
  return PlatformSpecificReset;
} // GetPlatformSpecificProtocol()


/**
  This function will locate any instances of EFI_RESET_NOTIFY_CALLBACK_PPI
  and notify them of the impending reset.

  IMPORTANT NOTE: This function will keep track of whether notification has
                  already occurred, so it only notifies once per boot.

  @param[in]  ResetType         EFI_RESET_TYPE for the reset being requested.
  @param[in]  ResetStatus       Honestly... just check the UEFI spec...
  @param[in]  DataSize          This function is an implementation of the EFI_RESET_SYSTEM prototype.
  @param[in]  ResetData

**/
VOID
NotifyAllResetCallbacks (
  IN EFI_RESET_TYPE               ResetType,
  IN EFI_STATUS                   ResetStatus,
  IN UINTN                        DataSize,
  IN CONST VOID                   *ResetData OPTIONAL
  )
{
  EFI_STATUS                          Status;
  UINTN                               Instance;
  RESET_PEI_INTERNAL_STATE_HOB        *InternalState;
  EFI_RESET_NOTIFY_CALLBACK_PPI       *ResetNotify;

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Notifying all interested parties of system reset.\n" ));

  //
  // If the callbacks have already been notified, we can get out of here.
  //
  InternalState = GetInternalState();
  // If the HOB doesn't exist yet, try to create it.
  if (InternalState == NULL)
  {
    Status = InitializeInternalStateHob();
    if (EFI_ERROR( Status ))
    {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to initialize persistence HOB!\n" ));
      ASSERT( FALSE );
      return;
    }

    InternalState = GetInternalState();
  }
  // If the HOB indicates that we've already notified, we can bail.
  if (InternalState != NULL && InternalState->State.AlreadyNotified)
  {
    // NOTE: If InternalState is NULL, we'll notify again,
    //       but that's a bit of an error anyway.
    return;
  }

  //
  // Notify all PPIs.
  //
  // Start at the beginning.
  Instance = 0;

  // Iterate until you don't find any more instances.
  do
  {
    // Check for the current instance.
    Status = PeiServicesLocatePpi( &gEfiResetNotifyCallbackPpiGuid,
                                   Instance,
                                   NULL,
                                   &ResetNotify );

    // If it exists, notify.
    if (Status == EFI_SUCCESS)
    {
      ResetNotify->ResetNotifyCallback( ResetType, ResetStatus, DataSize, ResetData );
    }
    // If it was a random error, let's raise that alarm.
    else if (Status != EFI_NOT_FOUND)
    {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Unexpected error while locating notification PPIs! %r\n", Status ));
      ASSERT( FALSE );
    }

    // Increment the instance.
    Instance += 1;
  } while (Status == EFI_SUCCESS);

  //
  // Remember that these have been notified once.
  //
  // This shouldn't matter, since the system IS GOING DOWN, right?
  if (InternalState != NULL)
  {
    InternalState->State.AlreadyNotified = TRUE;
  }

  return;
} // NotifyAllResetCallbacks()
