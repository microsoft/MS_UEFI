/** @file -- ResetHelperLib.h
This header describes various helper functions for resetting the system.

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

#ifndef _RESET_HELPER_LIB_H_
#define _RESET_HELPER_LIB_H_

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

  @retval     EFI_ERROR     An error was returned by ConstructResetData().
  @retval     -NONE-        Ideally, this call will not return. System will reset.

**/
EFI_STATUS
EFIAPI
ResetSystemWithSubtype (
  IN        EFI_RESET_TYPE    ResetType,
  IN CONST  EFI_GUID          *ResetSubtype
  );


/**
  This function will examine the DataSize and ResetData of a
  call to ResetSystem() to determine whether there is a valid
  reset subtype GUID at the end.

  Assumes a properly formatted ResetData buffer. ConstructResetData() helper
  is provided if the caller doesn't want to format manually.

  @param[in]  DataSize
  @param[in]  ResetData

  @retval     Pointer     Pointer to the EFI_GUID for the subtype being requested.
  @retval     NULL        No subtype could be found.

**/
EFI_GUID*
EFIAPI
GetResetSubtypeGuid (
  IN UINTN                        DataSize,
  IN CONST VOID                   *ResetData
  );


/**
  This is a helper function that will create the reset data
  structure for a ResetSystem() call.

  NOTE: Strings are internally limited by MAX_UINT16.

  @param[in, out] ResetDataSize On input, the size of the ResetData buffer. On output, either
                                the total number of bytes copied, or the required buffer size.
  @param[in, out] ResetData     A pointer to the buffer in which to place the final structure.
  @param[in]      ResetSubtype  [Optional] EFI_GUID pointer to the subtype that should be used.
  @param[in]      ResetString   [Optional] CHAR16 pointer to a string describing the reset.
  @param[in]      ExtraDataSize [Optional] If >0, the size of the ExtraData buffer.
  @param[in]      ExtraData     [Optional] If not NULL, a pointer to any extra data provided.

  @retval     EFI_SUCCESS             ResetDataSize and ResetData are now updated.
  @retval     EFI_INVALID_PARAMETER   ResetData is NULL.
  @retval     EFI_INVALID_PARAMETER   ExtraData and ExtraData size don't jive.
  @retval     EFI_INVALID_PARAMETER   ExtraData was provided without a ResetSubtype. This is
                                      not a state supported by the UEFI spec.
  @retval     EFI_BUFFER_TOO_SMALL    An insufficient buffer was provided.
                                      ResetDataSize is updated with minimum value.
  @retval     EFI_OUT_OF_RESOURCES    Could not allocate a buffer for the data.

**/
EFI_STATUS
EFIAPI
ConstructResetData (
  IN OUT   UINTN         *ResetDataSize,
  IN OUT   VOID          *ResetData,
  IN CONST EFI_GUID      *ResetSubtype OPTIONAL,
  IN CONST CHAR16        *ResetString OPTIONAL,
  IN       UINTN         ExtraDataSize OPTIONAL,
  IN CONST UINT8         *ExtraData OPTIONAL
  );

#endif // _RESET_HELPER_LIB_H_
