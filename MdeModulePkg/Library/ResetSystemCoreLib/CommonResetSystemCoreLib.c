/** @file -- PeiDxeResetSystemCoreLib.c
This library contains core business logic that is shared between
PEI main and DXE main.

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiResetSystemLib.h>
#include <Library/ResetHelperLib.h>

#include <Ppi/ResetSystemPpi.h>
#include <Protocol/ResetSystemProtocol.h>

#include <Library/ResetSystemCoreLib.h>

#include "InternalResetSystemCoreLib.h"


/**
  This is the primary function for all resets.

  This function will check for potential handlers, notify registered callbacks,
  and handle any errors.

  @param[in]  ResetType         EFI_RESET_TYPE for the reset being requested.
  @param[in]  ResetStatus       Honestly... just check the UEFI spec...
  @param[in]  DataSize          This function is an implementation of the EFI_RESET_SYSTEM prototype.
  @param[in]  ResetData

**/
VOID
EFIAPI
ResetSystemMain (
  IN EFI_RESET_TYPE               ResetType,
  IN EFI_STATUS                   ResetStatus,
  IN UINTN                        DataSize,
  IN VOID                         *ResetData OPTIONAL
  )
{
  EFI_RESET_TYPE                    FinalResetType = ResetType;
  EFI_STATUS                        FinalResetStatus = ResetStatus;
  UINTN                             FinalDataSize = DataSize;
  VOID                              *FinalResetData = ResetData;
  BOOLEAN                           ParametersUpdated;
  EFI_GUID                          *ResetSubtypeGuid = NULL;
  PLATFORM_SPECIFIC_RESET_PROTOCOL  *ResetProtocol = NULL;

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__"( 0x%X, %r, %d, <Pointer> )\n", FinalResetType, FinalResetStatus, FinalDataSize ));
  // TODO: Telemetry.
  // REPORT_STATUS_CODE (EFI_PROGRESS_CODE, (EFI_SOFTWARE_EFI_RUNTIME_SERVICE | EFI_SW_RS_PC_RESET_SYSTEM));
  // TODO: EFI_SW_PS_PC_RESET_SYSTEM is for PEI. EFI_SW_RS_PC_RESET_SYSTEM is for Runtime.

  //
  // Bounce on invalid calls.
  if (FinalDataSize > 0 && FinalResetData == NULL)
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - ResetSystem called with DataSize > 0 and NULL ResetData!!\n" ));
    // TODO: Telemetry.
    ASSERT( FALSE );
    // If we're not in debug mode, fall through so that a system reset can be attempted.
    // UEFI spec indicates that every attempt should be made to reset the system on request.
    FinalDataSize = 0;
  }

  //
  // Quickly, let's check to see whether there's a subtype supplied.
  ResetSubtypeGuid = GetResetSubtypeGuid( FinalDataSize, FinalResetData );

  //
  // Assuming we found a subtype, let's try to find an associated reset protocol.
  // This protocol will know more about how to handle this specific reset, inlcuding
  // potentially transforming it into a different reset.
  if (ResetSubtypeGuid != NULL && !InternalAtRuntime())
  {
    // Continue looping until the parameters don't get updated.
    ParametersUpdated = TRUE;
    while (ParametersUpdated)
    {
      // We start by assuming the parameters will not be updated.
      ParametersUpdated = FALSE;

      // Attempt to find a matching protocol.
      ResetProtocol = GetPlatformSpecificProtocol( ResetSubtypeGuid );

      // If found, filter the values.
      if (ResetProtocol != NULL && ResetProtocol->FilterReset != NULL)
      {
        ParametersUpdated = ResetProtocol->FilterReset( &FinalResetType, &FinalResetStatus, &FinalDataSize, &FinalResetData );
      }

      // If the values changed, make sure to grab the latest GUID.
      if (ParametersUpdated)
      {
        ResetSubtypeGuid = GetResetSubtypeGuid( FinalDataSize, FinalResetData );
      }
    }

    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Filtered parameters = 0x%X, %r, %d, <Pointer>\n", FinalResetType, FinalResetStatus, FinalDataSize ));
  }

  //
  // If we've made it this far, we have a handler and will attempt to reset the system.
  // We should notify any interested parties that this is really happening.
  if (!InternalAtRuntime())
  {
    // IMPORTANT NOTE: This function will keep track of whether notification has
    //                 already occurred, so it only notifies once per boot.
    NotifyAllResetCallbacks( FinalResetType, FinalResetStatus, FinalDataSize, FinalResetData );
  }

  //
  // At this point, if we found a protocol, let's attempt to use it to reset the system.
  if (ResetProtocol != NULL && ResetProtocol->ResetSystem != NULL)
  {
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Attempting platform-specific reset...\n" ));
    ResetProtocol->ResetSystem( FinalResetType, FinalResetStatus, FinalDataSize, FinalResetData );
    DEBUG(( DEBUG_WARN, __FUNCTION__" - Platform-specific reset failed to reset system!!\n" ));
  }
  // Otherwise, we shouldn't be attempting to handle platform-specific resets.
  else if (FinalResetType == EfiResetPlatformSpecific)
  {
    // TODO: Telemetry.
    if (ResetSubtypeGuid != NULL)
    {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - EfiResetPlatformSpecific requested, but no handler found for subtype %g!!\n", ResetSubtypeGuid ));
    }
    else
    {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - EfiResetPlatformSpecific requested, but no subtype specified!!\n" ));
    }
    ASSERT( FALSE );
    // If we're not in debug mode, fall through so that a system reset can be attempted.
    // UEFI spec indicates that every attempt should be made to reset the system on request.
  }

  //
  // If we're still here, we need to try the default handler with the current parameters.
  DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Resetting with default handler...\n" ));
  LibResetSystem( FinalResetType, FinalResetStatus, FinalDataSize, FinalResetData );
  DEBUG(( DEBUG_WARN, __FUNCTION__" - Default reset failed to reset system!!\n" ));

  //
  // If we're still here it's because we thought we had a good handler, but it all fell through.
  // We're going to display a notice and fall back to an EfiResetCold.
  //
  // IMPORTANT NOTE: This type may not match the type that was indicated in the reset notification.
  DEBUG(( DEBUG_INFO, __FUNCTION__" - ResetSystem handler failed to reset. Falling back to EfiResetCold.\n" ));
  LibResetSystem( EfiResetCold, FinalResetStatus, FinalDataSize, FinalResetData );

  //
  // If we're still here, something has gone wrong.
  DEBUG(( DEBUG_ERROR, __FUNCTION__" - WE SHOULD NOT BE HERE!! Reset requested but fell through!\n" ));
  // TODO: Telemetry.
  ASSERT( FALSE );
  CpuDeadLoop();      // JBB TODO: Log telemetry. Failed reset.

  return;
} // ResetSystemMain()