/** @file -- ResetSystemProtocol.h
This file contains protocol definitions for registering platform-specific
reset handlers.

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

#ifndef _RESET_SYSTEM_PROTOCOL_H_
#define _RESET_SYSTEM_PROTOCOL_H_

//~~~~~~~~~~~~~~~~~ !!! ~~~~~~~~~~~~~~~
// A Note About Using This Interface
//~~~~~~~~~~~~~~~~~ !!! ~~~~~~~~~~~~~~~
/*
When registering a handler for a specific reset type, use the reset subtype itself as
the protocol GUID when installing the protocol/PPI. For instance, if you're trying to
handle an EfiResetCold that has a subtype of gCapsuleUpdateCompleteResetGuid, you should
install an instance of PLATFORM_SPECIFIC_RESET_PROTOCOL using the gCapsuleUpdateCompleteResetGuid
as the protocol GUID in InstallProtocol().
*/

///================================================================================================
///================================================================================================
///
/// PLATFORM-SPECIFIC HANDLER
/// Describes the protocol that should be registered from each driver that wishes to provide a
/// platform-specific way to reset the system.
///
///================================================================================================
///================================================================================================

/**
  This function is designed to filter a platform-specific reset. "Filter" in this case means to
  potentially transform one reset type into another. This function will accept pointers to all the
  values associated with a reset and be given an opportunity to udpate them.

  If the values are updated, return TRUE. Otherwise, FALSE.

  @param[in, out]  ResetType      All of these...
  @param[in, out]  ResetStatus    arguments are...
  @param[in, out]  DataSize       pointers to the arguments...
  @param[in, out]  ResetData      for EFI_RESET_SYSTEM.

  @retval     TRUE      The parameters for this reset have been udpated.
  @retval     FALSE     The parameters have been left as-is.

**/
typedef
BOOLEAN
(EFIAPI *EFI_RESET_FILTER) (
  IN OUT  EFI_RESET_TYPE      *ResetType,
  IN OUT  EFI_STATUS          *ResetStatus,
  IN OUT  UINTN               *DataSize,
  IN OUT  VOID                **ResetData OPTIONAL
  );

typedef struct _PLATFORM_SPECIFIC_RESET_PROTOCOL
{
  // If this is not NULL, will be used to determine whether the current Platform Specific
  // reset maps to a different type of reset. (eg. A capsule-type reset might actually just be
  // a warm reset with some extra icing).
  EFI_RESET_FILTER            FilterReset OPTIONAL;

  // If this is not NULL, will be used to reset the system if a Platform Specific reset is
  // requested and matches the GUID this protocol is registered with.
  EFI_RESET_SYSTEM            ResetSystem OPTIONAL;
} PLATFORM_SPECIFIC_RESET_PROTOCOL;

#endif // _RESET_SYSTEM_PROTOCOL_H_
