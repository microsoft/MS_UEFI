
/** @file
 *PlatformBootManager  - Misc functions.

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

#include <Uefi.h>

#include <Library/PlatformBootManagerLib.h>

/**
  OnDemandConInConnect

  Platform callback to connect devices needed by console support
  before ConsoleVariable devices are started.

 */
VOID
EFIAPI
PlatformBootManagerOnDemandConInConnect (
  VOID
  )
{
    return;
}

/**
 * Constructor
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EFIAPI
PlatformBootManagerEntry (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
) {

    return EFI_SUCCESS;
}

/**
BDS Entry  - DXE phase complete, BDS Entered.
*/
VOID
EFIAPI
PlatformBootManagerBdsEntry (
  VOID
) {

    return;
}

/**
ProcessBootCompletion

Process boot loader errors

*/
VOID
EFIAPI
PlatformBootManagerProcessBootCompletion (
  IN EFI_BOOT_MANAGER_LOAD_OPTION *BootOption
) {

    return;
}


/**
 Priority Boot.  This is a platform defined mechanism to
 affect the boot order via GPIO or some other mechanism
*/
VOID
EFIAPI
PlatformBootManagerPriorityBoot (
    UINT16 **BootNext
    )
{

    return;
}



/**
 PlatformBootManagerDeadloop

 BDS has given up, and would deadloop.  Perform platform
 action instead of just locking up.
*/
VOID
EFIAPI
PlatformBootManagerDeadloop (
    VOID
    )
{

    return;
}

