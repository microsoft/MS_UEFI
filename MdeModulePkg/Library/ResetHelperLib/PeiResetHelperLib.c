/** @file -- PeiResetHelperLib.c
This file contains the Reset Helper functions that require
special implementations for PEI.

Copyright (c) 2016, Microsoft Corporation
Copyright (c) 2017, Intel Corporation

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

#include <PiPei.h>
#include "ResetHelperLibCommon.h"
#include <Library/PeiServicesLib.h>

/**
  This is a shorthand helper function to reset with a subtype so that
  the caller doesn't have to bother with a function that has half a dozen
  parameters.

  This will generate a reset with status EFI_SUCCESS, a NULL string, and
  no custom data. The subtype will be formatted in such a way that it can be
  picked up by notification registrations and custom handlers.

  NOTE: This call will fail if the architectural ResetSystem underpinnings
        are not initialized. For DXE, you can add gEfiResetArchProtocolGuid
        to your DEPEX.

  @param[in]  ResetType     Base reset type as defined in UEFI spec.
  @param[in]  ResetSubtype  GUID pointer for the reset subtype to be used.

  @retval     Any           Whatever ResetSystemWithAllParameters() returns.

**/
EFI_STATUS
EFIAPI
ResetSystemWithSubtype (
  IN        EFI_RESET_TYPE  ResetType,
  IN CONST  EFI_GUID        *ResetSubtype
  )
{
  GUID_SPECIFIC_RESET_DATA  ResetData;

  ResetData.NullTerminator = 0;
  CopyGuid (&ResetData.ResetSubtype, ResetSubtype);
  PeiServicesResetSystem2 (ResetType, EFI_SUCCESS, sizeof(ResetData), &ResetData);
}
