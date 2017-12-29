/** @file -- InfineonSpecificTpmCommands.c
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

#include <Library/DebugLib.h>
#include <Library/Tpm12DeviceLib.h>
#include <Library/Tpm2CommandLib.h>

#include <IndustryStandard/Tpm12.h>
#include <IndustryStandard/Tpm20.h>

#include "IfxDriverSource\Common\MicroTss\Tpm_2_0\TPM2_FieldUpgradeTypes.h"
#include "IfxDriverSource\Common\MicroTss\Tpm_2_0\TPM2_FieldUpgradeMarshal.h"

#include "Tpm12And20Shim.h"

// TODO: Much of this can probably be rolled into the init lib associated
//       with a given TPM.

//=======================================================================================
//
//  MODIFIED HELPER STRUCTURES
//
//=======================================================================================

#pragma pack(1)

typedef union {
  sSecurityModuleLogicInfo_d      Info;
  sSecurityModuleLogicInfo2_d     Info2;
} UNIFIED_SMLI_BUFFER;

typedef struct {
  TPM_RQU_COMMAND_HDR   Hdr;
  UINT8                 FuSubCommand;
  UINT16                InfoRequestSize;
} TPM_CMD_FU_INFO_REQUEST;

typedef struct {
  TPM_RSP_COMMAND_HDR   Hdr;
  UINT16                InfoResponseSize;
  UINT8                 InfoResponse[1024];
} TPM_RSP_FU_INFO_REQUEST;

typedef union {
  TPM_RSP_FU_INFO_REQUEST       Tpm12;
  TPMS_VENDOR_CAPABILITY_DATA   Tpm20;
} UNIFIED_FU_INFO_BUFFER;

#pragma pack()

//=======================================================================================
//=======================================================================================

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
  )
{
  // Since we should never have a dTPM 1.2 in our
  // systems, we can safely default to this.
  return TPM_ACTIVE_TYPE_12_LIMITED;
} // GetInfineonTpm12Type()


/**
  Retrieves the Security Module Logic Info structure from the TPM.
  This structure contains things like the remaining update count.

  @param[out] Buffer    Output buffer for the SMLI structure.

  @retval     EFI_SUCCESS       Buffer is successfully populated.
  @retval     EFI_NOT_READY     TPM access could not be obtained.
  @retval     EFI_UNSUPPORTED   Unknown/unsupported TPM type detected.
  @retval     EFI_DEVICE_ERROR  Something went wrong communicating with the TPM.
  @retval     Others            Tpm2GetCapability() or Tpm12SubmitCommand() returned an error.

**/
STATIC
EFI_STATUS
GetTpmSmli (
  OUT  UNIFIED_SMLI_BUFFER    *Buffer
  )
{
  EFI_STATUS              Status;
  TPM_ACTIVE_TYPE         TpmType;
  TPMI_YES_NO             HasMore;
  TPM_CMD_FU_INFO_REQUEST Tpm12Command;
  UNIFIED_FU_INFO_BUFFER  Response;
  UINT32                  Length;
  INT32                   IntLength;
  UINT8                   *MovingPointer;
  unsigned int            IfxResult;

  // Make sure we can use the TPM.
  Status = RequestTpmAccess();
  if (EFI_ERROR( Status )) {
    return EFI_NOT_READY;
  }

  // Initialize some things we're going to need.
  TpmType = GetTpmType();
  ZeroMem( Buffer, sizeof( *Buffer ) );
  ZeroMem( &Response, sizeof( Response ) );

  //
  // Based on the TpmType, we'll have to make different calls.
  // This is for TPM 2.0...
  //
  if (TpmType == TPM_ACTIVE_TYPE_20) {
    Status = Tpm2GetCapability( TPM_CAP_VENDOR_PROPERTY,
                                TPM_PT_VENDOR_FIX_SMLI2,
                                1,    // Property Count
                                &HasMore,
                                (TPMS_CAPABILITY_DATA*)&Response.Tpm20 );
    if (!EFI_ERROR( Status )) {
      // If we retrieved exactly one capability, we'll assume we're good to go.
      if (SwapBytes32(Response.Tpm20.data.vendorData.count) == 1) {
        //
        // NOTE: Because Infineon does some magic resizing while unpacking this structure,
        //       we must defer to THEIR version of unmarshalling.
        //
        MovingPointer = (UINT8*)&Response.Tpm20.data.vendorData.buffer[0].buffer[0];
        IntLength = SwapBytes16(Response.Tpm20.data.vendorData.buffer[0].size);
        IfxResult = TSS_sSecurityModuleLogicInfo2_d_Unmarshal( &Buffer->Info2, &MovingPointer, &IntLength );
        if (IfxResult != RC_SUCCESS) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to unmarshal Infineon-specifci data. 0x%8X\n", IfxResult ));
          Status = EFI_DEVICE_ERROR;
        }
      }
      else {
        DEBUG(( DEBUG_ERROR, __FUNCTION__" - GetCap returned too many fields.\n" ));
        Status = EFI_DEVICE_ERROR;
      }
    }
  } // if (TpmType == TPM_ACTIVE_TYPE_20)

  //
  // This is for TPM 1.2...
  //
  else if (TpmType == TPM_ACTIVE_TYPE_12 || TpmType == TPM_ACTIVE_TYPE_12_LIMITED) {
    // Prep the command for TPM 1.2.
    ZeroMem( &Tpm12Command, sizeof( Tpm12Command ) );
    Tpm12Command.Hdr.tag          = SwapBytes16( TPM_TAG_RQU_COMMAND );
    Tpm12Command.Hdr.paramSize    = SwapBytes32( sizeof( Tpm12Command ) );
    Tpm12Command.Hdr.ordinal      = SwapBytes32( TPM_ORD_FieldUpgrade );
    Tpm12Command.FuSubCommand     = TPM_FieldUpgradeInfoRequest2; // 1 Byte
    Tpm12Command.InfoRequestSize  = SwapBytes16( 0 );   // Not sure why this is, but Infineon says so.

    // Send the command.
    Length = sizeof( Response.Tpm12 );
    Status = Tpm12SubmitCommand( sizeof( Tpm12Command ),
                                 (UINT8*)&Tpm12Command,
                                 &Length,
                                 (UINT8*)&Response.Tpm12 );
    // Try twice on TPM_RESOURCES return code.
    if (!EFI_ERROR( Status ) && SwapBytes32( Response.Tpm12.Hdr.returnCode ) == TPM_RESOURCES) {
      Length = sizeof( Response.Tpm12 );
      Status = Tpm12SubmitCommand( sizeof( Tpm12Command ),
                                   (UINT8*)&Tpm12Command,
                                   &Length,
                                   (UINT8*)&Response.Tpm12 );
    }

    if (!EFI_ERROR( Status )) {
      if (SwapBytes32( Response.Tpm12.Hdr.returnCode ) == TPM_SUCCESS) {
        //
        // NOTE: Because Infineon does some magic resizing while unpacking this structure,
        //       we must defer to THEIR version of unmarshalling.
        //
        MovingPointer = &Response.Tpm12.InfoResponse[0];
        IntLength = SwapBytes16(Response.Tpm12.InfoResponseSize);
        IfxResult = TSS_sSecurityModuleLogicInfo_d_Unmarshal( &Buffer->Info, &MovingPointer, &IntLength );
        if (IfxResult != RC_SUCCESS) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to unmarshal Infineon-specifci data. 0x%8X\n", IfxResult ));
          Status = EFI_DEVICE_ERROR;
        }
      }
      else {
        DEBUG(( DEBUG_ERROR, __FUNCTION__" - TPM returned non-success for FieldUpgrade command. 0x%8X\n", SwapBytes32( Response.Tpm12.Hdr.returnCode ) ));
        Status = EFI_DEVICE_ERROR;
      }
    }
  } // else if (TpmType == TPM_ACTIVE_TYPE_12 || TpmType == TPM_ACTIVE_TYPE_12_LIMITED)

  //
  // This is for everything else...
  //
  else {
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Unknown TPM type!\n" ));
    Status = EFI_UNSUPPORTED;
  }

  return Status;
} // GetTpmSmli()


/**
  This helper function determines how many updates are remaining
  on the current TPM.

  @param[in]  Count   The remaining count.

  @retval     EFI_SUCCESS             Count was retrieved.
  @retval     EFI_INVALID_PARAMETER   One of the return pointers is NULL.

**/
EFI_STATUS
GetRemainingUpdateCount (
  IN  UINT32    *Count
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UNIFIED_SMLI_BUFFER   SmliBuffer;

  // Set a default value.
  *Count = 0;

  // The remaining update count is an attribute of the SMLI structure in Infineon
  // parts. The first part of the structure is identical, so we should be safe to
  // query it without part-specific logic.
  Status = GetTpmSmli( &SmliBuffer );
  if (!EFI_ERROR( Status )) {
    *Count = SmliBuffer.Info.wFieldUpgradeCounter;
  }

  DEBUG_CODE(
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Could not determine remaining updates! %r\n", Status ));
    }
    else {
      DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Remaining updates = %d.\n", *Count ));
    }
  );

  return Status;
} // GetRemainingUpdateCount()


/**
  This helper function determines what Infineon's vendor-specific "active version" is.

  @param[out] ActiveVersion   The active version.

  @retval     EFI_SUCCESS             Count was retrieved.
  @retval     EFI_INVALID_PARAMETER   One of the return pointers is NULL.

**/
EFI_STATUS
GetInfineonActiveVersion (
  OUT UINT32    *ActiveVersion
  )
{
  EFI_STATUS            Status = EFI_SUCCESS;
  UNIFIED_SMLI_BUFFER   SmliBuffer;

  // Set a default value.
  *ActiveVersion = 0;

  // The remaining update count is an attribute of the SMLI structure in Infineon
  // parts. The first part of the structure is identical, so we should be safe to
  // query it without mode-specific logic.
  Status = GetTpmSmli( &SmliBuffer );
  if (!EFI_ERROR( Status )) {
    SmliBuffer.Info.sSecurityModuleLogic.sFirmwareConfiguration.FirmwarePackage[0].StaleVersion != INACTIVE_STALE_VERSION ?
    (*ActiveVersion = SmliBuffer.Info.sSecurityModuleLogic.sFirmwareConfiguration.FirmwarePackage[0].Version) :
    (*ActiveVersion = SmliBuffer.Info.sSecurityModuleLogic.sFirmwareConfiguration.FirmwarePackage[1].Version);

    // Adjust ActiveVersion to align with regular version mask.
    *ActiveVersion = *ActiveVersion << 8;
  }

  DEBUG_CODE(
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Could not determine active version! %r\n", Status ));
    }
    else {
      DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Active version = 0x%8X.\n", *ActiveVersion ));
    }
  );

  return Status;
} // GetInfineonActiveVersion()
    