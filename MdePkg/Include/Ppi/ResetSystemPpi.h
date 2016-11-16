/** @file -- ResetSystemPpi.h
This file contains PPI definitions for registering platform-specific
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

#ifndef _RESET_SYSTEM_PPI_H_
#define _RESET_SYSTEM_PPI_H_

// Typically, this header within a header should be avoided, but some of
// these structures alias each other.
#include <Protocol/ResetSystemProtocol.h>

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
/// Describes the PPI that should be registered from each driver that wishes to provide a
/// platform-specific way to reset the system.
///
///================================================================================================
///================================================================================================

typedef PLATFORM_SPECIFIC_RESET_PROTOCOL    PLATFORM_SPECIFIC_RESET_PPI;

#endif // _RESET_SYSTEM_PPI_H_
