/** @file -- Tpm12And20Shim.c
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

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/Tpm12DeviceLib.h>
#include <Library/Tpm2DeviceLib.h>
#include <Library/Tpm12CommandLib.h>
#include <Library/Tpm2CommandLib.h>

#include <IndustryStandard/Tpm12.h>
#include <IndustryStandard/Tpm20.h>

#include "Tpm12And20Shim.h"
#include "InfineonSpecificTpmCommands.h"

// Obligatory definition for break-able do..while loops
// that are sometimes used for error handling.
#define       ERROR_HANDLING_RUN_ONCE     FALSE

BOOLEAN           mShimIsInitialized = FALSE;
TPM_ACTIVE_TYPE   mShimActiveType = TPM_ACTIVE_TYPE_UNKNOWN;

// TODO: Turn this into the 12And20CompatLib. Publish to TianoCore.
// TODO: Move some of these into Vendor-Specific init lib that already exists.
// TODO: Check implementation against TPM 2.0 and 1.2 spec for determining type.

//=======================================================================================
//
// TPM COMMAND STRUCTURES
// These structures make it easier to work with commands that must be manually sent
// to the TPM.
//
//=======================================================================================

#pragma pack(1)

typedef struct {
  TPM_RQU_COMMAND_HDR   Hdr;
  TPM_STARTUP_TYPE      TpmSt;
} TPM_CMD_START_UP;

typedef struct {
  TPM2_COMMAND_HEADER  Header;
  TPM_SU               StartupType;
} TPM2_STARTUP_COMMAND;

typedef struct {
  TPM2_RESPONSE_HEADER Header;
} TPM2_STARTUP_RESPONSE;

typedef union {
  TPM_CMD_START_UP      Tpm12Startup;
  TPM2_STARTUP_COMMAND  Tpm20Startup;
} TPM_12_20_COMMAND_BUFFER;

typedef union {
  TPM_RSP_COMMAND_HDR     Tpm12Startup;
  TPM2_STARTUP_RESPONSE   Tpm20Startup;
} TPM_12_20_RESPONSE_BUFFER;

#pragma pack()

//=======================================================================================
//=======================================================================================

/**
  This helper function will make calls to the TPM to determine
  what the active TPM type is.

  @retval     TPM_ACTIVE_TYPE   On error, return TPM_ACTIVE_TYPE_UNKNOWN.

**/
STATIC
TPM_ACTIVE_TYPE
DetermineActiveTpmType (
  VOID
  )
{
  TPM_ACTIVE_TYPE             Result = TPM_ACTIVE_TYPE_UNKNOWN;
  EFI_STATUS                  Status = EFI_SUCCESS;
  TPM_12_20_COMMAND_BUFFER    Command;
  TPM_12_20_RESPONSE_BUFFER   Response;
  UINT32                      ResponseSize, TpmResponseCode;

  do {
    // First thing's first, we need to try to start this thing up
    // as a TPM 2.0. If this works, we probably have a TPM 2.0.
    ZeroMem( &Command, sizeof( Command ) );
    ZeroMem( &Response, sizeof( Response ) );
    Command.Tpm20Startup.Header.tag         = SwapBytes16( TPM_ST_NO_SESSIONS );
    Command.Tpm20Startup.Header.paramSize   = SwapBytes32( sizeof( Command.Tpm20Startup ) );
    Command.Tpm20Startup.Header.commandCode = SwapBytes32( TPM_CC_Startup );
    Command.Tpm20Startup.StartupType        = SwapBytes16( TPM_SU_CLEAR );

    // Send the command, TPM 2.0 style.
    ResponseSize = sizeof( Response.Tpm20Startup );
    Status = Tpm2RequestUseTpm();
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Tpm2RequestUseTpm() = %r\n", Status ));
      break;
    }
    Status = Tpm2SubmitCommand( sizeof( Command.Tpm20Startup ), (UINT8*)&Command, &ResponseSize, (UINT8*)&Response );
    if (Status == EFI_SUCCESS) {
      // Check to see how things went.
      TpmResponseCode = SwapBytes32( Response.Tpm20Startup.Header.responseCode );
      // If we have any of these codes, we can safely conclude we have a TPM 2.0.
      if (TpmResponseCode == TPM_RC_SUCCESS ||
          TpmResponseCode == TPM_RC_INITIALIZE ||
          TpmResponseCode == TPM_RC_FAILURE) {
        Result = TPM_ACTIVE_TYPE_20;
        DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - TPM 2.0 detected in normal state.\n" ));
        break;
      }
      // If we have this code, we're still a TPM 2.0, but we need to reboot.
      // Might need to handle this case specially at some point.
      else if (TpmResponseCode == TPM_RC_REBOOT) {
        Result = TPM_ACTIVE_TYPE_20;
        DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - TPM 2.0 detected in reboot-required state.\n" ));
        break;
      }
    }
    else if (Status == EFI_UNSUPPORTED) {
      // This may indicate TPM 1.2, but seems to be vendor-specific.
    }
    else {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Tpm2SubmitCommand() = %r\n", Status ));
      break;
    }

    //
    // At this point, we cannot conclude we have a TPM 2.0,
    // but more investigation is needed to determine whether we have a TPM 1.2
    // or a FW-upgrade-failure state.
    //

    // Next thing, let's try to start it up as a TPM 1.2.
    ZeroMem( &Command, sizeof( Command ) );
    ZeroMem( &Response, sizeof( Response ) );
    Command.Tpm12Startup.Hdr.tag       = SwapBytes16( TPM_TAG_RQU_COMMAND );
    Command.Tpm12Startup.Hdr.paramSize = SwapBytes32( sizeof( Command.Tpm12Startup ) );
    Command.Tpm12Startup.Hdr.ordinal   = SwapBytes32( TPM_ORD_Startup );
    Command.Tpm12Startup.TpmSt         = SwapBytes16( TPM_ST_CLEAR );

    // Send the command, TPM 1.2 style.
    ResponseSize = sizeof( Response.Tpm12Startup );
    Status = Tpm12RequestUseTpm();
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Tpm12RequestUseTpm() = %r\n", Status ));
      break;
    }
    Status = Tpm12SubmitCommand( sizeof( Command.Tpm12Startup ), (UINT8*)&Command, &ResponseSize, (UINT8*)&Response);
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Tpm12SubmitCommand() = %r\n", Status ));
      break;
    }

    // Check to see how things went.
    TpmResponseCode = SwapBytes32( Response.Tpm12Startup.returnCode );
    // If we have any of these codes, we can safely conclude we have a TPM 1.2.
    if (TpmResponseCode == TPM_SUCCESS || TpmResponseCode == TPM_INVALID_POSTINIT) {
      Result = TPM_ACTIVE_TYPE_12;
      DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - TPM 1.2 detected in normal state.\n" ));
      break;
    }
    // If the TPM returns TPM_FAILEDSELFTEST, we may be in a failed FW update.
    // We'll need to probe (in a device-specific way) to determine if this is the case.
    else if (TpmResponseCode == TPM_FAILEDSELFTEST) {
      Result = GetInfineonTpm12Type();
    }
  } while (ERROR_HANDLING_RUN_ONCE);

  DEBUG_CODE(
    switch (Result) {
      case TPM_ACTIVE_TYPE_12:
        DEBUG(( DEBUG_INFO, __FUNCTION__" - TPM_ACTIVE_TYPE_12 type detected.\n" ));
        break;
      case TPM_ACTIVE_TYPE_12_LIMITED:
        DEBUG(( DEBUG_INFO, __FUNCTION__" - TPM_ACTIVE_TYPE_12_LIMITED type detected.\n" ));
        break;
      case TPM_ACTIVE_TYPE_20:
        DEBUG(( DEBUG_INFO, __FUNCTION__" - TPM_ACTIVE_TYPE_20 type detected.\n" ));
        break;
      default:
        DEBUG(( DEBUG_INFO, __FUNCTION__" - TPM_ACTIVE_TYPE_UNKNOWN type detected.\n" ));
        break;
    }
  );

  return Result;
} // DetermineActiveTpmType()


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
  )
{
  EFI_STATUS    Status = EFI_DEVICE_ERROR;

  // If the lib isn't initialized, we can't do anything yet.
  if (!mShimIsInitialized) {
    DEBUG(( DEBUG_INFO, __FUNCTION__" - Lib is not initialized.\n" ));
    return EFI_NOT_READY;
  }

  // If we're good, let's get access to the TPM.
  if (mShimActiveType == TPM_ACTIVE_TYPE_20) {
    Status = Tpm2RequestUseTpm();
  }
  else if (mShimActiveType == TPM_ACTIVE_TYPE_12 ||
           mShimActiveType == TPM_ACTIVE_TYPE_12_LIMITED) {
    Status = Tpm12RequestUseTpm();
  }
  else {
    Status = EFI_UNSUPPORTED;
  }

  DEBUG_CODE(
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Could not get access to TPM! %r\n", Status ));
    }
  );

  return Status;
} // RequestTpmAccess()


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
  @retval     EFI_INVALID_PARAMETER   One of the return pointers is NULL.
  @retval     EFI_NOT_READY           This library is not initialized.
  @retval     Others                  Some other error occurred.

**/
EFI_STATUS
GetTpmVersion (
  OUT TPM_ACTIVE_TYPE   *TpmType,
  OUT UINT32            *DataA,
  OUT UINT32            *DataB
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  TPM_VERSION   Tpm12Version;

  // If the lib isn't initialized, we can't do anything yet.
  if (!mShimIsInitialized) {
    DEBUG(( DEBUG_INFO, __FUNCTION__" - Lib is not initialized.\n" ));
    return EFI_NOT_READY;
  }

  //
  // First things, first. Let's set up some defaults
  // and validate the input.
  //
  if (TpmType == NULL || DataA == NULL || DataB == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  *TpmType = mShimActiveType;
  *DataA = 0;
  *DataB = 0;

  //
  // Make sure we can talk to the TPM.
  // If not, we can bail early.
  //
  Status = RequestTpmAccess();
  if (EFI_ERROR( Status )) {
    return Status;
  }

  //
  // We have to take a completely different path for TPM_TYPE_20 than TPM_TYPE_BOOTLOADER.
  // Let's make that choice here.
  //
  if (mShimActiveType == TPM_ACTIVE_TYPE_20) {
    Status = Tpm2GetCapabilityFirmwareVersion( DataA, DataB );
  }
  else if (mShimActiveType == TPM_ACTIVE_TYPE_12 ||
           mShimActiveType == TPM_ACTIVE_TYPE_12_LIMITED) {
    Status = Tpm12GetCapabilityFirmwareVersion( &Tpm12Version, NULL, NULL, NULL );
    // If successful, retrieve the full version from the vendor-specific data.
    if (!EFI_ERROR( Status )) {
      *DataA = (Tpm12Version.revMajor << 16) + Tpm12Version.revMinor;
    }
  }
  else {
    // How did you even get here?
    Status = EFI_DEVICE_ERROR;
  }

  DEBUG_CODE(
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Could not get TPM version! %r\n", Status ));
    }
    else {
      DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - %d, 0x%8X, 0x%8X\n", *TpmType, *DataA, *DataB ));
    }
  );

  return Status;
} // GetTpmVersion()


/**
  Small helper function to get the TPM type that was
  determined by this library during init.

  @retval     TPM_ACTIVE_TYPE   On error, return TPM_ACTIVE_TYPE_UNKNOWN.

**/
TPM_ACTIVE_TYPE
GetTpmType (
  VOID
  )
{
  TPM_ACTIVE_TYPE   Result = TPM_ACTIVE_TYPE_UNKNOWN;
  if (mShimIsInitialized) {
    Result = mShimActiveType;
  }
  return Result;
} // GetTpmType()


/**
  Initializes this abstraction lib so that future
  calls can be processed correctly.

  @retval     EFI_SUCCESS       Lib fully initialized.
  @retval     EFI_DEVICE_ERROR  Something went wrong.

**/
EFI_STATUS
InitTpm12And20AbstractionLib (
  VOID
  )
{
  mShimActiveType = DetermineActiveTpmType();
  // TPM_ACTIVE_TYPE is set with an enum starting at 0,
  // so this can be a numerical comparison.
  if (mShimActiveType < TPM_ACTIVE_TYPE_UNKNOWN) {
    mShimIsInitialized = TRUE;
  }

  return (mShimIsInitialized ? EFI_SUCCESS : EFI_DEVICE_ERROR);
} // InitTpm12And20AbstractionLib()
