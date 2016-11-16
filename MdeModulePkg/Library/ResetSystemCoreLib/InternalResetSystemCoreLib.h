/** @file -- InternalResetSystemCoreLib.h
This header declares functions that are shared between implementations
of ResetSystemCoreLib.

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

#ifndef _INTERNAL_RESET_SYSTEM_CORE_LIB_H_
#define _INTERNAL_RESET_SYSTEM_CORE_LIB_H_

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
  );


/**
  This function determines whether a protocol is defined for a
  EfiResetPlatformSpecific-type reset.
  If found, it will return the protocol, otherwise it will return NULL.

  IMPORTANT NOTE: The PPI and Protocol interfaces are identical. For simplicity,
                  this function and the business logic that consumes it will refer
                  to the Protocol structure.

  @param[in]  ResetGuid         Pointer to EFI_GUID representing the platform-specific
                                protocol being located.

  @retval     Pointer           Pointer to the protocol, if found.
  @retval     NULL              No protocol was found.

**/
PLATFORM_SPECIFIC_RESET_PROTOCOL*
GetPlatformSpecificProtocol (
  IN CONST EFI_GUID               *ResetGuid
  );


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
  );

#endif // _INTERNAL_RESET_SYSTEM_CORE_LIB_H_
