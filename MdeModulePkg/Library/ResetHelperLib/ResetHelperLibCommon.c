/** @file -- ResetHelperLibCommon.c
This contains the business logic for the module-specific Reset Helper functions.

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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ResetHelperLib.h>

#include "ResetHelperLibCommon.h"


/**
  This is a helper function that serves as a wrapper for ConstructResetData
  and the standard ResetSystem interface.

  The "help" that it provides is buffer management.

  NOTE: This call will fail if the architectural ResetSystem underpinnings
        are not initialized. For DXE, you can add gEfiResetArchProtocolGuid
        to your DEPEX.

  @param[in]  ResetType     This is the reset type as defined by EFI_RESET_SYSTEM.
  @param[in]  ResetStatus   This is the status as defined by EFI_RESET_SYSTEM.
  @param[in]  ResetSubtype  [Optional] EFI_GUID pointer to the subtype that should be used.
  @param[in]  ResetString   [Optional] CHAR16 pointer to a string describing the reset.
  @param[in]  ExtraDataSize [Optional] If >0, the size of the ExtraData buffer.
  @param[in]  ExtraData     [Optional] If not NULL, a pointer to any extra data provided.

  @retval     EFI_OUT_OF_RESOURCES  Memory could not be allocated for the ResetData or we are
                                    after ExitBootServices (and memory cannot be allocated).
  @retval     EFI_DEVICE_ERROR      For some reason the system did not reset.
  @retval     -NONE-                Ideally, this call will not return. System will reset.

**/
STATIC
EFI_STATUS
ResetSystemWithAllParameters (
  IN       EFI_RESET_TYPE ResetType,
  IN       EFI_STATUS     ResetStatus,
  IN CONST EFI_GUID       *ResetSubtype OPTIONAL,
  IN CONST CHAR16         *ResetString OPTIONAL,
  IN       UINTN          ExtraDataSize OPTIONAL,
  IN CONST UINT8          *ExtraData OPTIONAL
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINTN       ResetDataSize;
  VOID        *ResetData = NULL;

  //
  // Step 1: Determine how large a buffer we need.
  ResetDataSize = 0;
  Status = ConstructResetData( &ResetDataSize,
                               NULL,
                               ResetSubtype,
                               ResetString,
                               ExtraDataSize,
                               ExtraData );

  //
  // Step 2: Attempt to allocate a buffer for the ResetData.
  if (Status == EFI_BUFFER_TOO_SMALL)
  {
    ResetData = AllocatePool( ResetDataSize );
    if (ResetData == NULL)
    {
      Status = EFI_OUT_OF_RESOURCES;
    }
    else
    {
      Status = EFI_SUCCESS;
    }
  }

  //
  // Step 3: ACTUALLY build the data structure.
  if (!EFI_ERROR( Status ))
  {
    Status = ConstructResetData( &ResetDataSize,
                                 ResetData,
                                 ResetSubtype,
                                 ResetString,
                                 ExtraDataSize,
                                 ExtraData );
  }

  //
  // Step 2: If buffer was constructed, COOL! Reset.
  if (!EFI_ERROR( Status ))
  {
    ResetSystemWrapper( ResetType, EFI_SUCCESS, ResetDataSize, ResetData );
    // If we're still here, something went wrong.
    Status = EFI_DEVICE_ERROR;
  }

  // If we're still here, make sure to free the pointer.
  // We failed to reset, after all...
  if (ResetData != NULL)
  {
    FreePool( ResetData );
  }

  return Status;
} // ResetSystemWithAllParameters()


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
  IN        EFI_RESET_TYPE    ResetType,
  IN CONST  EFI_GUID          *ResetSubtype
  )
{
  return ResetSystemWithAllParameters( ResetType,
                                       EFI_SUCCESS,
                                       ResetSubtype,
                                       NULL,
                                       0,
                                       NULL );
} // ResetSystemWithSubtype()


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
  )
{
  UINTN           ResetDataStringLength, ResetDataStringSize;
  EFI_GUID        *ResetSubtypeGuid = NULL;

  //
  // If the parameters aren't useful, GTFO.
  if (ResetData == NULL || DataSize < sizeof( EFI_GUID ))
  {
    return NULL;
  }

  //
  // Next, determine how long the string is at the beginning of the ResetData.
  ResetDataStringLength = StrnLenS( ResetData, (DataSize / sizeof( CHAR16 )) );
  // Add the NULL terminator.
  ResetDataStringLength += 1;
  ResetDataStringSize = ResetDataStringLength * sizeof( CHAR16 );

  //
  // Now, assuming that we have enough data for a GUID after the string, the GUID
  // should be immediately after the string itself.
  if (ResetDataStringSize < DataSize &&
      (DataSize - ResetDataStringSize) >= sizeof( EFI_GUID ))
  {
    ResetSubtypeGuid = (EFI_GUID*)((UINT8*)ResetData + ResetDataStringSize);
  }

  //
  // Display a useful message, why not?
  if (ResetSubtypeGuid != NULL)
  {
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Detected reset subtype %g...\n", ResetSubtypeGuid ));
  }

  return ResetSubtypeGuid;
} // GetResetSubtypeGuid()


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
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN         ResetStringLength, ResetStringSize;
  UINTN         ResetDataBufferSize;
  CONST CHAR16  *FinalResetString = ResetString;
  UINT8         *CopyHere;

  //
  // First thing's first, let's check the parameters.
  if (ResetDataSize == NULL ||                            // If the size return pointer is NULL.
      (ExtraDataSize > 0 && ExtraData == NULL) ||         // If extra data is indicated, but pointer is NULL.
      (ResetSubtype == NULL && ExtraDataSize > 0))        // If extra data is indicated, but no subtype GUID is supplied.
  {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Determine the final string.
  if (FinalResetString == NULL)
  {
    FinalResetString = L"";     // Use an empty string.
  }
  // We'll pick an arbitrary limit, becaus the real limit will be how much
  // buffer the caller has provided us.
  ResetStringLength = StrnLenS( FinalResetString, MAX_UINT16 );
  ResetStringLength += 1;     // Add a space for the NULL terminator.
  ResetStringSize = ResetStringLength * sizeof( CHAR16 );

  //
  // Calculate the total buffer required for ResetData.
  ResetDataBufferSize = ResetStringSize;
  ResetDataBufferSize += ExtraDataSize;
  if (ResetSubtype != NULL)
  {
    ResetDataBufferSize += sizeof( EFI_GUID );
  }

  //
  // At this point, if the buffer isn't large enough (or if
  // the buffer is NULL) we cannot proceed.
  if (*ResetDataSize < ResetDataBufferSize)
  {
    Status = EFI_BUFFER_TOO_SMALL;
  }
  else if (ResetData == NULL)
  {
    Status = EFI_INVALID_PARAMETER;
  }

  //
  // If we're still here, we're copying.
  if (!EFI_ERROR( Status ))
  {
    CopyHere = ResetData;
    StrnCpyS( (CHAR16*)CopyHere, ResetStringLength, FinalResetString, ResetStringLength - 1 );
    CopyHere += ResetStringSize;
    if (ResetSubtype != NULL)
    {
      CopyMem( CopyHere, ResetSubtype, sizeof( EFI_GUID ) );
      CopyHere += sizeof( EFI_GUID );
    }
    if (ExtraDataSize > 0)
    {
      CopyMem( CopyHere, ExtraData, ExtraDataSize );
    }
  }

  //
  // Return the results and let's get out of here.
  *ResetDataSize  = ResetDataBufferSize;
  return Status;
} // ConstructResetData()
