/** @file -- ResetSystemDxe.c
This driver is responsible for hooking and handling the ResetSystem call in DXE.
It enables:
- Dynamic registration for platform-specific handlers.
- Notification callbacks for all drivers to be informed when the system is going down.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ResetSystemCoreLib.h>


/**
  The driver's entry point.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The entry point executed successfully.
  @retval EFI_ALREADY_STARTED   An instance of the gEfiResetArchProtocolGuid has already been installed.
  @retval Other                 Some error occured when executing this entry point.

**/
EFI_STATUS
EFIAPI
ResetSystemDxeEntryPoint (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS          Status;
  EFI_RESET_SYSTEM    PreviousHandler = NULL;
  VOID                *ResetArchProtocol;
  EFI_HANDLE          DummyHandle;

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__"()\n" ));

  //
  // First, make sure that no one else has gotten here before us.
  // It's no good having contention over who gets to handle a call.
  Status = gBS->LocateProtocol( &gEfiResetArchProtocolGuid, NULL, &ResetArchProtocol );
  // Note, we care if the protocol *is* installed, so we're checking to see if it succeeds. Uncommon logic.
  if (!EFI_ERROR( Status ))
  {
    // We can safely bail here.
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - gEfiResetArchProtocolGuid is already installed!\n" ));
    ASSERT( FALSE );
    Status = EFI_ALREADY_STARTED;
    goto Exit;
  }

  //
  // If we're good to go, let's go ahead and hook gRT->ResetSystem.
  PreviousHandler = SystemTable->RuntimeServices->ResetSystem;
  SystemTable->RuntimeServices->ResetSystem = ResetSystemMain;
  DEBUG(( DEBUG_INFO, __FUNCTION__" - gRT->ResetSystem hooked. New function is ResetSystemDxe::ResetSystemMain().\n" ));

  //
  // If we're successful thus far, let's notify that the reset is being handled.
  DummyHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces( &DummyHandle,
                                                   &gEfiResetArchProtocolGuid,
                                                   NULL,
                                                   NULL );
  if (EFI_ERROR( Status ))
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to install the gEfiResetArchProtocolGuid signal protocol! %r\n", Status ));
  }

Exit:
  // If we're returning an error, we need to do everything possible to put things
  // back the way they were. Can't leave any loose ends.
  if (EFI_ERROR( Status ))
  {
    // If we have hooked the ResetSystem handler, replace it.
    if (PreviousHandler != NULL)
    {
      SystemTable->RuntimeServices->ResetSystem = PreviousHandler;
    }
  }

  return Status;
} // ResetSystemDxeEntryPoint()
