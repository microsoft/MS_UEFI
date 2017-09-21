/** @file -- ResetUtilityLib.h
This header describes various helper functions for resetting the system.

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

#ifndef _RESET_UTILITY_LIB_H_
#define _RESET_UTILITY_LIB_H_

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

**/
VOID
EFIAPI
ResetPlatformSpecificGuid (
  IN CONST  GUID        *ResetSubtype
  );

/**
  This function examines the DataSize and ResetData parameters passed to
  to ResetSystem() and detemrines if the ResetData contains a Null-terminated
  Unicode string followed by a GUID specific subtype.  If the GUID specific 
  subtype is present, then a pointer to the GUID value in ResetData is returned.

  @param[in]  DataSize    The size, in bytes, of ResetData.
  @param[in]  ResetData   Pointer to the data buffer passed into ResetSystem().

  @retval     Pointer     Pointer to the GUID value in ResetData.
  @retval     NULL        ResetData is NULL.
  @retval     NULL        ResetData does not start with a Null-terminated
                          Unicode string.
  @retval     NULL        A Null-terminated Unicode string is present, but there
                          are less than sizeof (GUID) bytes after the string.
  @retval     NULL        No subtype is found.

**/
GUID *
EFIAPI
GetResetPlatformSpecificGuid (
  IN UINTN       DataSize,
  IN CONST VOID  *ResetData
  );

/**
  This is a helper function that creates the reset data buffer that can be 
  passed into ResetSystem().

  The reset data buffer is returned in ResetData and contains ResetString
  followed by the ResetSubtype GUID followed by the ExtraData.

  NOTE: Strings are internally limited by MAX_UINT16.

  @param[in, out] ResetDataSize  On input, the size of the ResetData buffer. On
                                 output, either the total number of bytes
                                 copied, or the required buffer size.
  @param[in, out] ResetData      A pointer to the buffer in which to place the
                                 final structure.
  @param[in]      ResetSubtype   Pointer to the GUID specific subtype.  This
                                 parameter is optional and may be NULL.
  @param[in]      ResetString    Pointer to a Null-terminated Unicode string
                                 that describes the reset.  This parameter is
                                 optional and may be NULL.
  @param[in]      ExtraDataSize  The size, in bytes, of ExtraData buffer.
  @param[in]      ExtraData      Pointer to a buffer of extra data.  This
                                 parameter is optional and may be NULL.

  @retval     RETURN_SUCCESS             ResetDataSize and ResetData are updated.
  @retval     RETURN_INVALID_PARAMETER   ResetDataSize is NULL.
  @retval     RETURN_INVALID_PARAMETER   ResetData is NULL.
  @retval     RETURN_INVALID_PARAMETER   ExtraData was provided without a
                                         ResetSubtype. This is not supported by the
                                         UEFI spec.
  @retval     RETURN_BUFFER_TOO_SMALL    An insufficient buffer was provided.
                                         ResetDataSize is updated with minimum size
                                         required.
**/
RETURN_STATUS
EFIAPI
BuildResetData (
  IN OUT   UINTN     *ResetDataSize,
  IN OUT   VOID      *ResetData,
  IN CONST GUID      *ResetSubtype  OPTIONAL,
  IN CONST CHAR16    *ResetString   OPTIONAL,
  IN       UINTN     ExtraDataSize  OPTIONAL,
  IN CONST VOID      *ExtraData     OPTIONAL
  );

#endif // _RESET_UTILITY_LIB_H_
