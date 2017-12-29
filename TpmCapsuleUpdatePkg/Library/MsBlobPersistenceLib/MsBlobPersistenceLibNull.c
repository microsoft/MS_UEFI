/** @file -- MsBlobPersistenceLibNull.c
A NULL implementation of the MsBlobPersistenceLib.

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>


/**
  This function behaves very similar to GetVariable, but is used
  to retrieve persisted blobs from the MsBlobPersistenceLib.

  Similar to GetVariable, pass a zero size to check for only existence and size.

  @param[in]        BlobId            EFI_GUID representing the unique ID for the blob.
  @param[in, out]   BlobSize          On input, the size of the buffer. On output, the size of the blob.
  @param[out]       BlobBuffer        A caller-allocated buffer to hold the contents of the retrieved blob.

  @retval     EFI_SUCCESS             Blob was retrieved and placed in buffer. Size has been updated.
  @retval     EFI_NOT_FOUND           A blob matching the ID was not found.
  @retval     EFI_BUFFER_TOO_SMALL    Blob found, but BlobSize is not large enough to contain it. BlobSize
                                      updated with the correct size.
  @retval     EFI_INVALID_PARAMETER   BlobId, BlobSize, or BlobBuffer is NULL.
  @retval     EFI_DEVICE_ERROR        Something went wrong while retrieving the blob.
  @retval     EFI_UNSUPPORTED         There is no persistence lib implemented.

**/
EFI_STATUS
EFIAPI
GetPersistedBlob (
  IN      EFI_GUID    *BlobId,
  IN OUT  UINTN       *BlobSize,
  OUT     VOID        *BlobBuffer
  )
{
  ASSERT( FALSE );
  return EFI_UNSUPPORTED;
} // GetPersistedBlob()


/**
  This function is the complement to GetPersistedBlob().
  It takes in a blob of data and attempts to persist it in an internal, non-volatile
  store. Implementations may allow one or multiple stores, however only one store is
  allowed per BlobId.

  If a blob already exists, it must be deleted before another may be created.

  To delete a blob, call SetPersistedBlob() with BlobSize == 0 and BlobBuffer == NULL.

  @param[in]        BlobId            EFI_GUID representing the unique ID for the blob.
  @param[in, out]   BlobSize          On input, the size of the blob. On error, output the max size of the store.
  @param[in]        BlobBuffer        A caller-allocated buffer to hold the contents of the blob.

  @retval     EFI_SUCCESS             Blob was saved (or deleted) successfully.
  @retval     EFI_ALREADY_STARTED     There is already an instance of this BlobId; cannot set (can delete).
  @retval     EFI_OUT_OF_RESOURCES    There are no remaining stores for blobs.
  @retval     EFI_BAD_BUFFER_SIZE     BlobSize exceeds max store size. BlobSize is updated with max store size.
  @retval     EFI_INVALID_PARAMETER   BlobId is NULL, BlobSize is NULL, or BlobSize and BlobBuffer
                                      disagree (eg. 0 <> NULL).
  @retval     EFI_DEVICE_ERROR        Something went wrong while saving the blob.
  @retval     EFI_UNSUPPORTED         There is no persistence lib implemented.

**/
EFI_STATUS
EFIAPI
SetPersistedBlob (
  IN      EFI_GUID    *BlobId,
  IN OUT  UINTN       *BlobSize,
  IN      VOID        *BlobBuffer
  )
{
  ASSERT( FALSE );
  return EFI_UNSUPPORTED;
} // SetPersistedBlob()
