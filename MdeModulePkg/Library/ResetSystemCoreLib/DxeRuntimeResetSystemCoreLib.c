/** @file -- DxeRuntimeResetSystemCoreLib.c
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

#include <Library/DebugLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Protocol/ResetSystemProtocol.h>
#include <Protocol/ResetNotify.h>

#include <Library/ResetSystemCoreLib.h>

#include "InternalResetSystemCoreLib.h"


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
  return EfiAtRuntime();
}


/**
  This function determines whether a protocol is defined for a
  EfiResetPlatformSpecific-type reset.
  If found, it will return the protocol, otherwise it will return NULL.

  IMPORTANT NOTE: The PPI and Protocol interfaces are identical. For simplicity,
                  this function and the business logic that consumes it will refer
                  to the Protocol structure.

  ASSERT if more than one protocol is installed for a given type. In release
  builds, just return the first protocol found.

  @param[in]  ResetGuid         Pointer to EFI_GUID representing the platform-specific
                                protocol being located.

  @retval     Pointer           Pointer to the protocol, if found.
  @retval     NULL              No protocol was found.

**/
PLATFORM_SPECIFIC_RESET_PROTOCOL*
GetPlatformSpecificProtocol (
  IN CONST EFI_GUID               *ResetGuid
  )
{
  EFI_STATUS                          Status;
  UINTN                               HandleCount;
  EFI_HANDLE                          *HandleBuffer = NULL;
  PLATFORM_SPECIFIC_RESET_PROTOCOL    *PlatformSpecificReset = NULL;

  // If no GUID is provided, let's get out of here.
  if (ResetGuid == NULL)
  {
    goto Exit;
  }

  // Attempt to locate a protocol associated with this reset type.
  Status = gBS->LocateHandleBuffer( ByProtocol,
                                    (EFI_GUID*)ResetGuid,     // Must cast because UEFI doesn't agree with the CONST.
                                    NULL,
                                    &HandleCount,
                                    &HandleBuffer );
  // If there was an error or we didn't find any protocols, we can't proceed.
  if (EFI_ERROR( Status ) || HandleCount < 1)
  {
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - LocateHandleBuffer = %r. Count = %d.\n", Status, HandleCount ));
    goto Exit;
  }

  // If we found too many instances, let's take a look at that...
  // Probably the platform code needs to be revised.
  // NOTE: This step only means anything in debug mode.
  if (HandleCount > 1)
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Too many protocols for platform-specific reset %g! Count = %d.\n", ResetGuid, HandleCount ));
    ASSERT( FALSE );
    // If we're not in debug mode, fall through so that a system reset can be attempted.
    // UEFI spec indicates that every attempt should be made to reset the system on request.
  }

  // If it was found, awesome! Let's get to it.
  Status = gBS->HandleProtocol( HandleBuffer[0],
                                (EFI_GUID*)ResetGuid,         // Must cast because UEFI doesn't agree with the CONST.
                                (VOID**)&PlatformSpecificReset );
  // If this fails... not much we can do here, either.
  if (EFI_ERROR( Status ))
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Platform-specific reset registered, but could not be found!\n" ));
    PlatformSpecificReset = NULL;
    goto Exit;
  }

Exit:
  // If a buffer was created, let's free it.
  if (HandleBuffer != NULL)
  {
    FreePool( HandleBuffer );
    HandleBuffer = NULL;
  }

  return PlatformSpecificReset;
} // GetPlatformSpecificProtocol()


/**
  This function will locate any instances of EFI_RESET_NOTIFY_CALLBACK_PROTOCOL
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
  UINTN                               Index, HandleCount;
  EFI_HANDLE                          *HandleBuffer = NULL;
  EFI_RESET_NOTIFY_CALLBACK_PROTOCOL  *ResetNotify;
  static BOOLEAN                      AlreadyNotified = FALSE;

  // If the callbacks have already been notified, we can get out of here.
  if (AlreadyNotified)
  {
    return;
  }

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Notifying all interested parties of system reset.\n" ));

  // Find all instances of the notify protocol.
  Status = gBS->LocateHandleBuffer( ByProtocol,
                                    &gEfiResetNotifyCallbackProtocolGuid,
                                    NULL,
                                    &HandleCount,
                                    &HandleBuffer );

  // If we're good, let's walk each and dispatch the notification function.
  if (!EFI_ERROR( Status ))
  {
    // Walk each and do the do.
    for (Index = 0; Index < HandleCount; Index++)
    {
      Status = gBS->HandleProtocol( HandleBuffer[Index],
                                    &gEfiResetNotifyCallbackProtocolGuid,
                                    (VOID**)&ResetNotify );
      if (!EFI_ERROR( Status ))
      {
        ResetNotify->ResetNotifyCallback( ResetType, ResetStatus, DataSize, ResetData );
      }
      else
      {
        DEBUG(( DEBUG_ERROR, __FUNCTION__" - ResetSystem Notify registered, but could not be found!\n" ));
      }
    }

    // Make sure to free the buffer. Not that the intention isn't to IMMEDIATELY bail.
    FreePool( HandleBuffer );
    HandleBuffer = NULL;
  }

  // Remember that these have been notified once.
  // This shouldn't matter, since the system IS GOING DOWN, right?
  AlreadyNotified = TRUE;

  return;
} // NotifyAllResetCallbacks()
