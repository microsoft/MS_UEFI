/** @file -- Tpm12And20Shim.h
This file contains code that wraps the TPM command so that
single calls may be made for both 1.2 and 2.0 TPMs.

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

#ifndef _TPM_12_AND_20_SHIM_H_
#define _TPM_12_AND_20_SHIM_H_

typedef UINT8   TPM_ACTIVE_TYPE;
enum {
  TPM_ACTIVE_TYPE_12 = 0,
  TPM_ACTIVE_TYPE_12_LIMITED,
  TPM_ACTIVE_TYPE_20,
  // Other types.
  TPM_ACTIVE_TYPE_UNKNOWN
};

/**
  Small helper function to get the TPM type that was
  determined by this library during init.

  @retval     TPM_ACTIVE_TYPE   On error, return TPM_ACTIVE_TYPE_UNKNOWN.

**/
TPM_ACTIVE_TYPE
GetTpmType (
  VOID
  );


/**
  This helper function abstracts the TPM12 and TPM2 device
  libs so that a single request may be made to get TPM device
  access.

  @retval     EFI_SUCCESS   Access is available.
  @retval     EFI_NOT_READY This library is not initialized.
  @retval     Others        No attempt should be made to access the TPM.

**/
EFI_STATUS
RequestTpmAccess (
  VOID
  );


/**
  This helper function will retrieve the version number
  for a dTPM (using the Infineon encoding) in either TPM 2.0
  or "bootloader" mode.

  NOTE: This current method is device-specific.

  @param[out] TpmType The type of the current TPM, since the encoding of the version is
                      specific to this.
  @param[out] DataA   This is the first 32 bits of the version info for TPM_TYPE_20
                      or all of the version info for TPM_TYPE_BOOTLOADER.
  @param[out] DataB   This is the second 32 bits of the version info for TPM_TYPE_20.

  @retval     EFI_SUCCESS             The version was retrieved successfully.
  @retval     EFI_INVALID_PARAMETER   One of the return pointers is NULL or the type
                                      is something other than TPM_TYPE_20 or TPM_TYPE_BOOTLOADER.
  @retval     EFI_NOT_READY           This library is not initialized.
  @retval     Others                  Some other error occurred.

**/
EFI_STATUS
GetTpmVersion (
  OUT TPM_ACTIVE_TYPE   *TpmType,
  OUT UINT32            *DataA,
  OUT UINT32            *DataB
  );


/**
  Initializes this abstraction lib so that future
  calls can be processed correctly.

  @retval     EFI_SUCCESS       Lib fully initialized.
  @retval     EFI_DEVICE_ERROR  Something went wrong.

**/
EFI_STATUS
InitTpm12And20AbstractionLib (
  VOID
  );

#endif // _TPM_12_AND_20_SHIM_H_
