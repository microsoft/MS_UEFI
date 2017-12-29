/** @file -- InfineonSpecificTpmCommands.h
This file is just a dumping ground for commands and procedures that
may be specific to the Infineon dTPM parts.

Copyright (c) 2017, Microsoft Corporation

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

#ifndef _INFINEON_SPECIFIC_TPM_COMMANDS_H_
#define _INFINEON_SPECIFIC_TPM_COMMANDS_H_

/**
  Helper function to determine whether a TPM 1.2 part is
  in failure state because of a bad FW udpate or because
  something else went wrong.

  NOTE: Assumes that we're talking to an Infineon TPM and
        that we've already determined it's not communicating
        as a TPM 2.0.

  @retval     TPM_ACTIVE_TYPE

**/
TPM_ACTIVE_TYPE
GetInfineonTpm12Type (
  VOID
  );


/**
  This helper function determines how many updates are remaining
  on the current TPM.

  @param[in]  Count   The remaining count.

  @retval     EFI_SUCCESS       Count was retrieved.
  @retval     EFI_DEVICE_ERROR  Something went wrong.

**/
EFI_STATUS
GetRemainingUpdateCount (
  IN  UINT32    *Count
  );


/**
  This helper function determines what Infineon's vendor-specific "active version" is.

  @param[out] ActiveVersion   The active version.

  @retval     EFI_SUCCESS             Count was retrieved.
  @retval     EFI_INVALID_PARAMETER   One of the return pointers is NULL.

**/
EFI_STATUS
GetInfineonActiveVersion (
  OUT UINT32    *ActiveVersion
  );

#endif // _INFINEON_SPECIFIC_TPM_COMMANDS_H_
