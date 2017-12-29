/** @file -- MsInfineonTpmFmpDeviceLib.c
FMP driver library for Infineon dTPM device.
Largely implemented with sample code from Infineon.

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
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/MsBlobPersistenceLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/FmpWrapperDeviceLib/VariableSupport.h>
#include <Library/ReportStatusCodeLib.h>              // Status Code for Telemetry in Release Builds
#include <Library/FmpRecoveryProgressIndicatorLib.h>  // Visual indicator for recovery progress.

#include <Protocol/FirmwareManagement.h>

#include <Library/FmpDeviceLib.h>

#include <IndustryStandard/Tpm20.h>

#include <Guid/SystemResourceTable.h>
#include <Guid/ZeroGuid.h>

#include <MsStatusCodes.h>

#include "Tpm12And20Shim.h"
#include "InfineonSpecificTpmCommands.h"

#include "IfxDriverSource/Common/ErrorCodes.h"
#include "BorrowedInfineonFunctions.h"

// Obligatory definition for break-able do..while loops
// that are sometimes used for error handling.
#define     ERROR_HANDLING_RUN_ONCE     FALSE

#pragma pack(1)

// This header is defined so that a single capsule can contain payloads
// targeted at different "from" versions. This is because TPM versions
// are coded as "from" and "to"...
// So, if you're trying to update a collection of machines that may have
// versions 1.0, 2.0, or 3.0 to version 4.0, you would have to have:
//    A binary from 1.0 -> 4.0
//         "        2.0 -> 4.0
//         "        3.0 -> 4.0
// This header wraps multiple binaries so a single capsule can be built for
// all of these scenarios.
typedef struct _TPM_CAPSULE_PAYLOAD_HEADER {
  UINT32      Signature;
  UINT32      Version;
  UINT16      SubPayloadCount;
  // TPM_CAPSULE_SUBPAYLOAD_HEADER    SubPayloads[];
} TPM_CAPSULE_PAYLOAD_HEADER;

// This header describes a singly update payload.
// These payloads will be targeted at a single "from" version
// and update to the same "to" version.
//    eg. "from" 1.0, "to" 4.0
typedef struct _TPM_CAPSULE_SUBPAYLOAD_HEADER {
  UINT32      FromVersion;
  UINT32      SubPayloadSize;    // Size of the subpayload NOT including this header.
  // UINT8       SubPayload[];
} TPM_CAPSULE_SUBPAYLOAD_HEADER;

#pragma pack()

// We're apparently going to need one of these. We'll need to initialize it, too.
IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA  *g_pPrivateData = NULL;

#define     TPM_CAPSULE_PAYLOAD_HEADER_SIG    SIGNATURE_32( 'M', 'T', 'C', 'P' )
#define     TPM_CAPSULE_PAYLOAD_HEADER_VER    1

#define     TPM_RECOVERY_ATTEMPT_COUNT_VAR    L"RecAttemptCount"
#define     TPM_MAX_RECOVERY_ATTEMPTS         2
#define     TPM_MAX_UPDATES_ALLOWED           64      // This may be vendor-specific, since the internal implemention is v-s as well.

#define     TPM_LAST_KNOWN_FMP_UUID_VAR       L"LastKnownFmpUuid"

// Forward delcaration for functions in this file.
STATIC
EFI_STATUS
SetRecoveryAttemptCount (
  IN    UINT32    Count
  );

/**
  Since this isn't going to change on a single boot and we use the data
  multiple times, we can safely wrap the function with a caching version that
  will do the same thing.

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
STATIC
EFI_STATUS
GetCachedTpmVersion (
  OUT TPM_ACTIVE_TYPE   *TpmType,
  OUT UINT32            *DataA,
  OUT UINT32            *DataB
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;
  static BOOLEAN          CacheValid = FALSE;
  static TPM_ACTIVE_TYPE  CachedType = TPM_ACTIVE_TYPE_UNKNOWN;
  static UINT32           CachedDataA = 0;
  static UINT32           CachedDataB = 0;

  if (TpmType == NULL || DataA == NULL || DataB == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CacheValid) {
    Status = GetTpmVersion( &CachedType, &CachedDataA, &CachedDataB );
    if (!EFI_ERROR( Status )) {
      CacheValid = TRUE;
    }
  }

  *TpmType  = CachedType;
  *DataA    = CachedDataA;
  *DataB    = CachedDataB;

  return Status;
}


/**
  Used to pass the FMP install function to this lib.  
  This allows the library to have control of the handle
  that the FMP instance is installed on.  This allows the library 
  to use DriverBinding protocol model to locate its device(s) in the
  system. 

  @param[in]        Function pointer to FMP install function. 

  @retval EFI_SUCCESS       Library has saved function pointer and will call function pointer on each DriverBinding Start.
  @retval EFI_UNSUPPORTED   Library doesn't use driver binding and only supports a single instance. 
  @retval other error       Error occurred.  Don't install FMP
  
**/
EFI_STATUS
EFIAPI
RegisterFmpInstaller (
  IN FMP_DEVICE_LIB_REGISTER_FMP_INSTALLER    Func
  )
{
  // TODO: Add a check for TPM mfg into tpm-specific init lib.
  //       At *some* point we need to prove the MFG is correct for the lib.

  // We need to make sure that the TPM 1.2-2.0 abstraction lib has initialized.
  if (GetTpmType() == TPM_ACTIVE_TYPE_UNKNOWN) {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - TPM type cannot be determined!\n" ));
    return EFI_DEVICE_ERROR;
  }

  // We don't need to register for driver binding.
  // We only talk to the TPM.
  return EFI_UNSUPPORTED;
}

  
/**
Used to get the size of the image in bytes as returned by GetImage().
If the image size is indeterminate, return 0.

@retval UINTN that represents the size of the firmware.

**/
UINTN
EFIAPI
FmpDeviceGetSize(
  VOID
  )
{
  // We don't have a fixed size for this image.
  return 0;
}

/**
Retrieve the last known FMP GUID from NVRAM.
Used to determine the correct GUID when the TPM is in recovery mode.

  @retval The last known FMP GUID

  @retval     EFI_SUCCESS             GUID was successfully retrieved.
  @retval     EFI_DEVICE_ERROR        SetVariable returned an error.
 */
EFI_STATUS
EFIAPI
GetLastKnownFmpGuid(
  OUT EFI_GUID** Guid
  )
{
  EFI_STATUS    Status;
  UINTN         DataSize;
  EFI_GUID      *FmpGuid;

  FmpGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
  if (FmpGuid == NULL || CompareGuid(FmpGuid, &gZeroGuid)) {
    // GUID has not been defined correctly in the platform.dsc
    DEBUG(( DEBUG_ERROR, __FUNCTION__ " - FMP GUID not defined\n"));
    Status = EFI_DEVICE_ERROR;
  }
  else {
    // Read FMP GUID from NVRAM
    DataSize = sizeof( EFI_GUID );
    Status = gRT->GetVariable( TPM_LAST_KNOWN_FMP_UUID_VAR,
                               FmpGuid,
                               NULL,
                               &DataSize,
                               *Guid );
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - GetVariable() = %r\n", Status ));
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
Set the last known FMP GUID into NVRAM
Used to keep track of the FMP GUID in case the TPM enters recovery mode.

  @param Guid:  The GUID to update the variable to

  @retval     EFI_SUCCESS             GUID was successfully updated.
  @retval     EFI_DEVICE_ERROR        SetVariable returned an error.

 */
EFI_STATUS
EFIAPI
SetLastKnownFmpGuid(
    EFI_GUID* Guid
    )
{
  EFI_STATUS    Status;
  UINTN         DataSize;
  UINT32        Attributes;
  EFI_GUID      *FmpGuid;


  FmpGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
  if (FmpGuid == NULL || CompareGuid(FmpGuid, &gZeroGuid)) {
    // GUID has not been defined correctly in the platform.dsc
    DEBUG(( DEBUG_ERROR, __FUNCTION__ " - FMP GUID not defined\n"));
    Status = EFI_DEVICE_ERROR;
  }
  else {
    Attributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS);
    DataSize = sizeof( EFI_GUID );
    Status = gRT->SetVariable( TPM_LAST_KNOWN_FMP_UUID_VAR,
                               FmpGuid,
                               Attributes,
                               DataSize,
                               Guid);
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - SetVariable() = %r\n", Status ));
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
Used to return a library supplied guid that will be the ImageTypeId guid of the FMP descriptor.  
This is optional but can be used if at runtime the guid needs to be determined.
If not specified, guid from PcdDeviceLibWrapperDeviceGuid will be used.

@param  Guid:  Double Guid Ptr that will be updated to point to guid.  This should be from static memory
               and will not be freed.  
@return EFI_UNSUPPORTED: if you library instance doesn't need dynamic guid return this.
@return Error: Any error will cause the wrapper to use the GUID defined by PCD
@return EFI_SUCCESS:  Guid ptr should be updated to point to static memeory which contains a valid guid
**/
EFI_STATUS
EFIAPI
FmpDeviceGetImageTypeIdGuidPtr(
  OUT EFI_GUID** Guid)
{
  EFI_STATUS        Status;
  TPM_ACTIVE_TYPE   TpmType;
  UINT32            Version = 0;
  EFI_GUID*         FmpDeviceGuid = NULL;

  TpmType = GetTpmType();
  if (TpmType == TPM_ACTIVE_TYPE_12 || TpmType == TPM_ACTIVE_TYPE_12_LIMITED) {
    // In bootloader mode, the minor number is not available,
    // which unfortunately is what tells us whether the TPM is Certified or Uncertified.
    DEBUG((DEBUG_WARN, "TPM is in fallback mode, retrieving TPM FMP GUID from NVRAM\n"));

    // In this case, we must retrieve the last used GUID from NVRAM
    Status = GetLastKnownFmpGuid(&FmpDeviceGuid);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "TPM could not retrieve the last known FMP GUID!!\n"));
      FmpDeviceGuid = NULL;
    }
  }
  else {
    Status = FmpDeviceGetVersion(&Version);
    if (!EFI_ERROR(Status)) {
      UINT32 Revision = Version & 0xFF;

      if (Revision == 0) {
        // Certified TPM Version (x.x.x.0)
        FmpDeviceGuid = (EFI_GUID*)PcdGetPtr(PcdDeviceTpmFmpGuidCertified);
        if (FmpDeviceGuid == NULL || CompareGuid(FmpDeviceGuid, &gZeroGuid)) {
          // GUID has not been defined correctly in the platform.dsc
          DEBUG(( DEBUG_ERROR, __FUNCTION__ " - Certified FMP GUID not defined\n"));
          Status = EFI_DEVICE_ERROR;
        }
        else {
          DEBUG((DEBUG_INFO, "TPM is Certified (x.x.x.0)\n"));
        }
      }
      else if (Revision == 2) {
        // Uncertified TPM Version (x.x.x.2)
        FmpDeviceGuid = (EFI_GUID*)PcdGetPtr(PcdDeviceTpmFmpGuidUncertified);
        if (FmpDeviceGuid == NULL || CompareGuid(FmpDeviceGuid, &gZeroGuid)) {
          // GUID has not been defined correctly in the platform.dsc
          DEBUG(( DEBUG_ERROR, __FUNCTION__ " - Uncertified FMP GUID not defined\n"));
          Status = EFI_DEVICE_ERROR;
        }
        else {
          DEBUG((DEBUG_INFO, "TPM is Uncertified (x.x.x.2)\n"));
        }
      }
      else {
        // We do not support this minor version - fallback
        DEBUG((DEBUG_WARN, "TPM revision is unsupported (x.x.x.%d)\n", Revision));
      }
    }
  }

  if (!EFI_ERROR(Status) && (FmpDeviceGuid != NULL) && (!CompareGuid(FmpDeviceGuid, &gZeroGuid))) {
    // We have a valid FMP GUID to report
    DEBUG((DEBUG_INFO, __FUNCTION__ " - TPM FMP GUID: %g\n", FmpDeviceGuid));

    Status = SetLastKnownFmpGuid(FmpDeviceGuid);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, __FUNCTION__ " - Error storing the last known TPM FMP GUID\n"));
      // Don't fail, we can still proceed...
    }

    *Guid = FmpDeviceGuid;
    return EFI_SUCCESS;
  }
  else {
    // Could not determine whether the TPM should provide certified or uncertified GUID.
    // Fail, and let the capsule lib fall back to PcdDeviceLibWrapperDeviceGuid
    DEBUG((DEBUG_INFO, __FUNCTION__ " - Using fallback TPM FMP GUID\n"));
    return EFI_UNSUPPORTED;
  }
}

/**
Used to get the FMP attributes for this device.
Do not use Authentication.  

@param[out] Supported     Attributes Supported on this platform (See FirmwareManagementProtocol).
@param[out] Setting       Attributes Set for the current running image (See FirmwareManagementProtocol).
**/
VOID
EFIAPI
FmpDeviceGetAttributes (
  IN OUT  UINT64* Supported,
  IN OUT UINT64* Setting
  )
{
  UINT32    TpmUpdatesRemaining, TpmUpdatesAdvertised;

  DEBUG(( DEBUG_VERBOSE, "InfineonFmpGetAttributes()\n" ));

  *Supported = (IMAGE_ATTRIBUTE_IN_USE | IMAGE_ATTRIBUTE_RESET_REQUIRED | IMAGE_ATTRIBUTE_IMAGE_UPDATABLE);
  *Setting = (IMAGE_ATTRIBUTE_IN_USE | IMAGE_ATTRIBUTE_RESET_REQUIRED);

  // Determine whether we should advertise any more updates.
  TpmUpdatesAdvertised = PcdGet32( TpmMaxAdvertisedUpdates );
  ASSERT( TpmUpdatesAdvertised <= TPM_MAX_UPDATES_ALLOWED );
  if (!EFI_ERROR( GetRemainingUpdateCount( &TpmUpdatesRemaining ) )) {
    if (TpmUpdatesRemaining > (TPM_MAX_UPDATES_ALLOWED - TpmUpdatesAdvertised)) {
      *Setting |= IMAGE_ATTRIBUTE_IMAGE_UPDATABLE;
    }
  }

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Supported: 0x%016lX, Setting: 0x%016lX\n", *Supported, *Setting ));

  return;
}


/**
Gets the current Lowest Supported Version.
This is a protection mechanism so that a previous version with known issue is not
applied.

ONLY implement this if your running firmware has a method to return this at runtime.

@param[out] Version           On return this value represents the 
current Lowest Supported Version (in same format as GetVersion).  

@retval EFI_SUCCESS           The Lowest Supported Version was correctly retrieved
@retval EFI_UNSUPPORTED       Device firmware doesn't support reporting LSV
@retval EFI_DEVICE_ERROR      Error occurred when trying to get the LSV
**/
EFI_STATUS
EFIAPI
FmpDeviceGetLowestSupportedVersion(
  OUT UINT32* LowestSupportedVersion
  )
{
  // TPM devices do not support FW rollback for security reasons.
  return FmpDeviceGetVersion(LowestSupportedVersion);
}

/**
Gets the current running version in Unicode string format.  
ONLY implement this if your running firmware has a method to return this at boot time.
Memory must be allocated in BS memory.  String must be NULL terminated. 
Return NULL if error or not implemented.  

@retval Valid Pointer         A function allocated memory buffer in BS memory containing a NULL terminated unicode string representing the version. 
@retval NULL                  An error or unsupported.  
**/
CHAR16*
EFIAPI
FmpDeviceGetVersionString(
  VOID
  )
{
  EFI_STATUS        Status;
  TPM_ACTIVE_TYPE   TpmType;
  UINT32            DataA, DataB;
  CHAR16            *VersionString;
  UINTN             StringSize;

  DEBUG(( DEBUG_VERBOSE, "InfineonFmpGetVersionString()\n" ));

  // First, get the version from the TPM.
  Status = GetCachedTpmVersion( &TpmType, &DataA, &DataB );
  if (EFI_ERROR( Status )) {
    return NULL;
  }

  // If we've gotten a version, let's format it.
  // We'll allocate a buffer large enough to hold a string that is "XXXXX.YYYYY.ZZZ.QQQ".
  StringSize = sizeof( L"XXXXX.YYYYY.ZZZ.QQQ" );
  VersionString = AllocatePool( StringSize );

  // If we've that didn't return NULL, we should be good to format the string.
  if (VersionString != NULL) {
    // IFX-Specific:
    // Maybe move this to a lib at some point.
    UnicodeSPrint( VersionString, StringSize, L"%d.%d.%d.%d", DataA >> 16, DataA & 0xFFFF, DataB >> 8, DataB & 0xFF );
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - VersionString: '%s'\n", VersionString ));
  }

  return VersionString;
}


/**
Gets the current running version.  
ONLY implement this if your running firmware has a method to return this at runtime.

@param[out] Version           On return this value represents the current running version

@retval EFI_SUCCESS           The version was correctly retrieved
@retval EFI_UNSUPPORTED       Device firmware doesn't support reporting current version
@retval EFI_DEVICE_ERROR      Error occurred when trying to get the version
**/
EFI_STATUS
EFIAPI
FmpDeviceGetVersion(
  OUT UINT32* Version
  )
{
  EFI_STATUS        Status;
  TPM_ACTIVE_TYPE   TpmType;
  UINT32            DataA, DataB;

  DEBUG(( DEBUG_VERBOSE, "InfineonFmpGetVersion()\n" ));

  // First, get the version from the TPM.
  Status = GetCachedTpmVersion( &TpmType, &DataA, &DataB );
  *Version = 0;

  // If we're still good, we should be able to format this version number.
  if (!EFI_ERROR( Status )) {
    // For bootloader mode, the version is just DataA.
    if (TpmType == TPM_ACTIVE_TYPE_12 || TpmType == TPM_ACTIVE_TYPE_12_LIMITED) {
      *Version = DataA;
    }
    // For TPM 2.0 mode, the version is a permutation of DataA and DataB.
    else if (TpmType == TPM_ACTIVE_TYPE_20) {
      // In this encoding, we take the bottom 8-bits of DataA (the minor number)
      // and combine it with the two sub-version numbers in DataB.
      // IFX-Specific:
      // Maybe move this to a lib at some point.
      *Version = ((DataA & 0xFF) << 24) + (DataB & 0xFFFFFF);
    }
  }

  DEBUG_CODE(
    if (!EFI_ERROR( Status )) {
      DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Version: 0x%08X\n", *Version ));
    }
  );

  return Status;
}


/**
Retrieves a copy of the current firmware image of the device.

This function allows a copy of the current firmware image to be created and saved.
The saved copy could later been used, for example, in firmware image recovery or rollback.

@param[out] Image              Points to the buffer where the current image is copied to.
@param[out] ImageSize          On entry, points to the size of the buffer pointed to by Image, in bytes.
On return, points to the length of the image, in bytes.

@retval EFI_SUCCESS            The device was successfully updated with the new image.
@retval EFI_BUFFER_TOO_SMALL   The buffer specified by ImageSize is too small to hold the
image. The current buffer size needed to hold the image is returned
in ImageSize.
@retval EFI_INVALID_PARAMETER  The Image was NULL.
@retval EFI_NOT_FOUND          The current image is not copied to the buffer.
@retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceGetImage(
  IN OUT VOID*  Image,
  IN OUT UINTN* ImageSize
  )
{
  // We don't support reading the image from the TPM.
  return EFI_UNSUPPORTED;
}


/**
  This function will:
  - validate a TPM capsule payload header
  - determine the current state of the TPM and the current version that we're coming from
  - walk through the subpayload list to determine whether there's a valid subpayload that
    matches the version we're coming from

  @param[in]  Image           Pointer to the full capsule image.
  @param[in]  ImageSize       Size of the full capsule image.
  @param[out] SubPayload      Pointer to the subpayload targeted at our current version, if found.
  @param[out] SubPayloadSize  Size of the subpayload, if found.

  @retval     EFI_SUCCESS             A correct and matching subpayload was found.
  @retval     EFI_NOT_FOUND           This payload looks valid, but a matching subpayload could not be found.
  @retval     EFI_INVALID_PARAMETER   Image, SubPayload, or SubPayloadSize are NULL.
  @retval     EFI_INVALID_PARAMETER   The payload signature or version do not match.
  @retval     EFI_INVALID_PARAMETER   The size of the payload and the size of the subpaylaods
                                      are not in agreement.
  @retval     EFI_DEVICE_ERROR        Failed to determine the current FW version from FMP.

**/
STATIC
EFI_STATUS
GetCorrectSubPayload (
  IN  CONST VOID          *Image,
  IN  UINTN               ImageSize,
  OUT VOID                **SubPayload,
  OUT UINTN               *SubPayloadSize
  )
{
  EFI_STATUS        Status = EFI_NOT_FOUND;
  TPM_ACTIVE_TYPE   TpmType;
  UINT32            TargetVersion, VersionCompareMask;
  UINTN             RemainingImageSize;
  UINTN             Index;
  CONST TPM_CAPSULE_PAYLOAD_HEADER    *PayloadHeader;
  CONST TPM_CAPSULE_SUBPAYLOAD_HEADER *SubPayloadHeader;

  if (Image == NULL || SubPayload == NULL || SubPayloadSize == NULL) {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Required parameter is null!\n" ));
    return EFI_INVALID_PARAMETER;
  }

  // First, let's initialize the return pointers.
  *SubPayload = NULL;
  *SubPayloadSize = 0;

  // Determine what mode the TPM is in.
  TpmType = GetTpmType();

  // ERROR_HANDLING_RUN_ONCE
  do {
    // Make sure that we understand our current version so
    // we can find the corresponding subpayload.
    if (TpmType == TPM_ACTIVE_TYPE_UNKNOWN) {
      Status = EFI_DEVICE_ERROR;
      break;
    }
    else if (TpmType == TPM_ACTIVE_TYPE_12_LIMITED) {
      Status = GetInfineonActiveVersion( &TargetVersion );
      // Compare limited bits in TPM_ACTIVE_TYPE_12_LIMITED state.
      VersionCompareMask = 0x00FFFF00;
    }
    else {
      Status = FmpDeviceGetVersion( &TargetVersion );
      if (EFI_ERROR( Status )) {
        Status = EFI_DEVICE_ERROR;
        break;
      }
      // Compare all bits in normal operation.
      VersionCompareMask = 0xFFFFFFFF;
    }

    // NOTE: It's important that we verify the sizes as we process.
    //       Don't want to do bad pointer math or go past bounds.
    if (ImageSize < sizeof( *PayloadHeader )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Bad image size!\n" ));
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    // Check the payload header signature and version.
    PayloadHeader = Image;
    if (PayloadHeader->Signature != TPM_CAPSULE_PAYLOAD_HEADER_SIG &&
        PayloadHeader->Version != TPM_CAPSULE_PAYLOAD_HEADER_VER) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Header appears invalid!\n" ));
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    // Update the remaining size and sub-payload pointer.
    RemainingImageSize = ImageSize - sizeof( *PayloadHeader );
    SubPayloadHeader = (TPM_CAPSULE_SUBPAYLOAD_HEADER*)((CONST UINT8*)Image + sizeof( *PayloadHeader ));
    Index = 0;
    Status = EFI_NOT_FOUND;

    // Iterate through the sub-payloads, looking for a match.
    while (Index < PayloadHeader->SubPayloadCount) {
      // Check to see whether the sub-payload size fits in the remaining size.
      // Otherwise, something fishy is going on.
      if (RemainingImageSize < sizeof( *SubPayloadHeader ) ||
          RemainingImageSize < SubPayloadHeader->SubPayloadSize ||
          RemainingImageSize < (sizeof( *SubPayloadHeader ) + SubPayloadHeader->SubPayloadSize)) {
        DEBUG(( DEBUG_ERROR, __FUNCTION__" - Subpayload index %d has bad size!\n" ));
        Status = EFI_INVALID_PARAMETER;
        break;
      }

      // If the size seems sufficient, check to see whether this payload matches what
      // we're looking for.
      if ((SubPayloadHeader->FromVersion & VersionCompareMask) == TargetVersion) {
        *SubPayload = (UINT8*)SubPayloadHeader + sizeof( *SubPayloadHeader );
        *SubPayloadSize = SubPayloadHeader->SubPayloadSize;
        Status = EFI_SUCCESS;
        break;
      }

      // If it doesn't match, let's keep looking.
      RemainingImageSize -= sizeof( *SubPayloadHeader ) + SubPayloadHeader->SubPayloadSize;
      SubPayloadHeader = (TPM_CAPSULE_SUBPAYLOAD_HEADER*)((UINT8*)SubPayloadHeader +
                                                          sizeof( *SubPayloadHeader ) +
                                                          SubPayloadHeader->SubPayloadSize);
      Index++;
    }

    // Status should be accurate for return coming out of the prior while loop.

  } while (ERROR_HANDLING_RUN_ONCE);

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Status = %r, SubPayload = 0x%016lX, SubPayloadSize = 0x%X\n",
          Status, *SubPayload, *SubPayloadSize ));
  
  return Status;
} // GetCorrectSubPayload()


/**
  This is a wrapper for the common parts of CheckImage that are called
  by both FmpDeviceCheckImage() and the recovery path.

  @param[in]  Image             Points to the new image.
  @param[in]  ImageSize         Size of the new image in bytes.
  @param[out] ImageUpdatable    Indicates if the new image is valid for update. It also provides,
                                if available, additional information if the image is invalid.

  @retval     EFI_SUCCESS         ImageUpdatable reflects the true state of the TPM.
  @retval     EFI_DEVICE_ERROR    Something went wrong while talking to the TPM.

**/
STATIC
EFI_STATUS
InternalCheckImage (
  IN  CONST VOID                        *Image,
  IN  UINTN                             ImageSize,
  OUT UINT32                            *ImageUpdatable
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT32                      TpmUpdatesRemaining = REMAINING_UPDATES_UNAVAILABLE;
  UINT32                      MaxCapsuleUpdates;
  UINTN                       IfxCallResult = RC_SUCCESS;
  int                         ImageValid = FALSE;     // Reassigned from BOOL (instead of BOOLEAN)
  UINT32                      DetailedError = 0;
  BITFIELD_NEW_TPM_FIRMWARE_INFO    FwImageCheckInfo = {0};

  // Set a default return value.
  *ImageUpdatable = 0;

  //
  // ERROR_HANDLING_RUN_ONCE
  //
  do {
    // Make sure we can talk to the TPM.
    // If not, we can bail early.
    Status = RequestTpmAccess();
    if (EFI_ERROR( Status )) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    // Determine whether we should allow any more capsule updates.
    // If we're out of updates, we can bail early.
    MaxCapsuleUpdates = PcdGet32( TpmMaxAllowedCapsuleUpdates );
    ASSERT( MaxCapsuleUpdates <= TPM_MAX_UPDATES_ALLOWED );
    if (!EFI_ERROR( GetRemainingUpdateCount( &TpmUpdatesRemaining ) )) {
      if (TpmUpdatesRemaining <= (TPM_MAX_UPDATES_ALLOWED - MaxCapsuleUpdates)) {
        *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
        DEBUG(( DEBUG_INFO, __FUNCTION__" - No more capsule updates allowed!\n" ));
        Status = EFI_SUCCESS;

        // Telemetry for this unexpected event.
        ReportStatusCode( (EFI_ERROR_MAJOR | EFI_ERROR_CODE), MS_RSC_CAPSULE_TPM_NO_MORE_UPDATES_ALLOWED );

        break;
      }
    }
    else {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to GetRemainingUpdates()!\n" ));
      Status = EFI_DEVICE_ERROR;
      break;
    }

    //
    // Initialize the Infineon TPM stack (as opposed to our internal stack)...
    //
    // IFX-Specific:
    // Maybe move this to a lib at some point.
    Status = InitializeTpmAccess();
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to InitializeTpmAccess()! %r\n", Status ));
      Status = EFI_DEVICE_ERROR;
      break;
    }

    // Otherwise, we'll just pass this request along to the IFX code.
    IfxCallResult = FirmwareUpdate_CheckImage( (VOID*)Image,
                                               ImageSize,
                                               &ImageValid,
                                               &FwImageCheckInfo,
                                               &DetailedError );
    // Now we must interpret how that all went down.
    // Infineon returns more detailed errors than this FMP driver can support.
    // We'll just stick to the basics.
    // Are the return values valid?
    if (IfxCallResult == RC_SUCCESS) {
      Status = EFI_SUCCESS;

      // Is the image valid?
      if (ImageValid) {
        *ImageUpdatable |= IMAGE_UPDATABLE_VALID;
      }
      // Otherwise, what type of error occurred?
      else {
        DEBUG(( DEBUG_INFO, __FUNCTION__" - Infineon reports image is not valid. 0x%X\n", DetailedError ));
        switch (DetailedError) {
          case RC_E_FW_UPDATE_BLOCKED:
            *ImageUpdatable = IMAGE_UPDATABLE_INVALID;
            break;
          case RC_E_WRONG_DECRYPT_KEYS:
          case RC_E_WRONG_FW_IMAGE:
            *ImageUpdatable = IMAGE_UPDATABLE_INVALID_TYPE;
            break;
          default:
            break;
        }
      }
    }
    // Return values are invalid.
    else {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to CheckImage! 0x%X\n", IfxCallResult ));
      Status = EFI_DEVICE_ERROR;
    }
  } while (ERROR_HANDLING_RUN_ONCE);

  return Status;
} // InternalCheckImage()


/**
Checks if the firmware image is valid for the device.

This function allows firmware update application to validate the firmware image without
invoking the SetImage() first.

@param[in]  Image             Points to the new image.
@param[in]  ImageSize         Size of the new image in bytes.
@param[out] ImageUpdatable    Indicates if the new image is valid for update. It also provides,
                              if available, additional information if the image is invalid.

@retval EFI_SUCCESS            The image was successfully checked.
@retval EFI_INVALID_PARAMETER  The Image was NULL or the header didn't check out.

**/
EFI_STATUS
EFIAPI
FmpDeviceCheckImage (
  IN  CONST VOID                        *Image,
  IN  UINTN                             ImageSize,
  OUT UINT32                            *ImageUpdatable
  )
{
  EFI_STATUS                  Status;
  VOID                        *InternalPayload;
  UINTN                       InternalPayloadSize;

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__"()\n" ));

  //
  // There are a number of places that assume a UINT32 size.
  // Let's check that now.
  //
  if (ImageSize > MAX_UINT32) {
    return EFI_INVALID_PARAMETER;
  }

  // Now, we're going to need a valid payload targeted at this machine.
  Status = GetCorrectSubPayload( Image, ImageSize, &InternalPayload, &InternalPayloadSize );
  if (EFI_ERROR( Status )) {
    return EFI_INVALID_PARAMETER;
  }

  return InternalCheckImage( InternalPayload, InternalPayloadSize, ImageUpdatable );
}


/**
  Simple debug function for verifying the progress of a "recovery"
  update. Will display a debug progress message as well as updating
  the visual indicator provided by FmpRecoveryProgressIndicatorLib.

  @param[in]  Progress      A UINTN from 0-100 indicating the current progress.

  @retval     Result of UpdateRecoveryProgressIndicator()

**/
STATIC
EFI_STATUS
RecoveryProgressUpdate (
  IN  UINTN    Progress
  )
{
  DEBUG(( DEBUG_INFO, "TpmFmpDeviceLib - Recovery progress -- %d%\n", Progress ));
  return UpdateRecoveryProgressIndicator( Progress );
} // RecoveryProgressUpdate()


/**
  This is a wrapper for most of the SetImage logic implemented in this
  library. It's used here so that there's a common entry point from both
  FmpDeviceSetImage() and the recovery path.

  Will delete any persisted image using the MsBlobPersistenceLib if an update
  is completed successfully.

  @param[in]    Image       A buffer containing the image. Must be wrapped with a
                            valid payload header.
  @param[in]    ImageSize   Size of the image buffer.
  @param[in]    Progress    A FMP progress update callback function.

  @retval       EFI_SUCCESS           Update was completed successfully.
  @retval       EFI_INVALID_PARAMETER Payload wrapper was not found or is invalid.
  @retval       EFI_DEVICE_ERROR      MsBlobPersistenceLib returned an error.
  @retval       EFI_DEVICE_ERROR      Could not initialize TPM access.
  @retval       EFI_DEVICE_ERROR      Could not determine TPM type.
  @retval       EFI_DEVICE_ERROR      Could not start TPM policy session.
  @retval       EFI_ABORTED           Infineon update function returned error.

**/
STATIC
EFI_STATUS
InternalSetImage  (
  IN  CONST VOID                                       *Image,
  IN  UINTN                                            ImageSize,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS    Progress
  )
{
  EFI_STATUS                  Status;
  EFI_GUID                    *BlobIdGuid;
  UINTN                       BlobSize;
  TPM_ACTIVE_TYPE             TpmType;
  // Required for Infineon stack.
  UINT32                      PolicySession;
  UINTN                       IfxCallResult;
  IfxFirmwareUpdateData       UpdateData;

  // ERROR_HANDLING_RUN_ONCE
  do {
    //
    // Initialize the Infineon TPM stack (as opposed to our internal stack)...
    //
    // IFX-Specific:
    // Maybe move this to a lib at some point.
    Status = InitializeTpmAccess();
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to InitializeTpmAccess()! %r\n", Status ));
      Status = EFI_DEVICE_ERROR;
      break;
    }

    //
    // If the TpmType == TPM_ACTIVE_TYPE_20,
    // we must create a policy session to authenticate with.
    //
    TpmType = GetTpmType();
    if (TpmType >= TPM_ACTIVE_TYPE_UNKNOWN) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Could not determine the TPM type!\n" ));
      Status = EFI_DEVICE_ERROR;
      break;
    }
    if (TpmType == TPM_ACTIVE_TYPE_20) {
      IfxCallResult = FirmwareUpdate_PrepareTPM20Policy( &PolicySession );
      if (IfxCallResult != RC_SUCCESS) {
        DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to start TPM 2.0 policy session! 0x%X\n", IfxCallResult ));
        Status = EFI_DEVICE_ERROR;
        break;
      }
      // IFX-Specific:
      // Maybe move this to a lib at some point.
      g_pPrivateData->unSessionHandle = PolicySession;
    }

    //
    // The Infineon call expects all its parameters in this command structure.
    //
    // IFX-Specific:
    // Maybe move this to a lib at some point.
    ZeroMem( &UpdateData, sizeof( UpdateData ) );
    UpdateData.rgbFirmwareImage     = (UINT8*)Image;
    UpdateData.unFirmwareImageSize  = (UINT32)ImageSize;
    UpdateData.fnProgressCallback   = Progress;
    UpdateData.unSessionHandle      = g_pPrivateData->unSessionHandle;
    // Arguably, we should also support a 1.2 OwnerPassword, but we don't support 1.2.

    //
    // Finally, we're ready to send the command down to the Infineon stack.
    //
    // IFX-Specific:
    // Maybe move this to a lib at some point.
    IfxCallResult = FirmwareUpdate_UpdateImage( &UpdateData );
    if (IfxCallResult != RC_SUCCESS) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to SetImage! 0x%X\n", IfxCallResult ));
      Status = EFI_ABORTED;

      // Telemetry for this unexpected event.
      ReportStatusCode( (EFI_ERROR_MAJOR | EFI_ERROR_CODE), MS_RSC_CAPSULE_TPM_INTERNAL_SETIMAGE_FAILED );

      break;
    }

    //
    // If the update was successful, we can clean up the persistence blob.
    //
    // In order to do this, we need to get a GUID to identify the blob.
    BlobIdGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
    BlobSize = 0;
    Status = SetPersistedBlob( BlobIdGuid, &BlobSize, NULL );
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to clean up the persistence blob! %r\n", Status ));
      ASSERT( FALSE );
    }

    //
    // If we've made it this far, we're hopefully successful.
    //
    Status = EFI_SUCCESS;
  } while (ERROR_HANDLING_RUN_ONCE);

  return Status;
} // InternalSetImage()


/**
Updates the firmware image of the device.

This function updates the hardware with the new firmware image.
This function returns EFI_UNSUPPORTED if the firmware image is not updatable.
If the firmware image is updatable, the function should perform the following minimal validations
before proceeding to do the firmware image update.
- Validate the image is a supported image for this device.  The function returns EFI_ABORTED if
the image is unsupported.  The function can optionally provide more detailed information on
why the image is not a supported image.
- Validate the data from VendorCode if not null.  Image validation must be performed before
VendorCode data validation.  VendorCode data is ignored or considered invalid if image
validation failed.  The function returns EFI_ABORTED if the data is invalid.

VendorCode enables vendor to implement vendor-specific firmware image update policy.  Null if
the caller did not specify the policy or use the default policy.  As an example, vendor can implement
a policy to allow an option to force a firmware image update when the abort reason is due to the new
firmware image version is older than the current firmware image version or bad image checksum.
Sensitive operations such as those wiping the entire firmware image and render the device to be
non-functional should be encoded in the image itself rather than passed with the VendorCode.
AbortReason enables vendor to have the option to provide a more detailed description of the abort
reason to the caller.

@param[in]  Image              Points to the new image.
@param[in]  ImageSize          Size of the new image in bytes.
@param[in]  VendorCode         This enables vendor to implement vendor-specific firmware image update policy.
Null indicates the caller did not specify the policy or use the default policy.
@param[in]  Progress           A function used by the driver to report the progress of the firmware update.
@param[in]  CapsuleFwVersion   MSFMPCapsule version of the image
@param[out] AbortReason        A pointer to a pointer to a null-terminated string providing more
details for the aborted operation. The buffer is allocated by this function
with AllocatePool(), and it is the caller's responsibility to free it with a
call to FreePool().

@retval EFI_SUCCESS            The device was successfully updated with the new image.
@retval EFI_ABORTED            The operation is aborted.
@retval EFI_INVALID_PARAMETER  The Image was NULL.
@retval EFI_UNSUPPORTED        The operation is not supported.

**/
EFI_STATUS
EFIAPI
FmpDeviceSetImage (
  IN  CONST VOID                                       *Image,
  IN  UINTN                                            ImageSize,
  IN  CONST VOID                                       *VendorCode,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS    Progress,
  IN  UINT32                                           CapsuleFwVersion,
  OUT CHAR16                                           **AbortReason
  )
{
  EFI_STATUS                  Status;
  EFI_GUID                    *BlobIdGuid;
  UINTN                       BlobSize;
  VOID                        *InternalPayload;
  UINTN                       InternalPayloadSize;

  DEBUG(( DEBUG_VERBOSE, __FUNCTION__"()\n" ));

  // Now, we're going to need a valid payload targeted at this machine.
  Status = GetCorrectSubPayload( Image, ImageSize, &InternalPayload, &InternalPayloadSize );
  if (EFI_ERROR( Status )) {
    return EFI_INVALID_PARAMETER;
  }

  // If we're coming in from a fresh capsule, clear the retry count.
  // We haven't attempting any recoveries with this image yet.
  SetRecoveryAttemptCount( 0 );

  //
  // Before we will start a flash, we will first attempt to save the
  // FW image so that we can re-attempt the update if something goes wrong.
  // NOTE: We will persist the entire payload, header and all, because
  //       this will still be run through CheckImage, and CheckImage
  //       will inspect parts of the header.
  //
  // In order to do this, we need to get a GUID to identify the blob.
  BlobIdGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
  // Check to see whether the blob already exists for this device.
  BlobSize = 0;
  Status = GetPersistedBlob( BlobIdGuid, &BlobSize, NULL );
  // If the return is EFI_BUFFER_TOO_SMALL, this blob already exists.
  // For any other status we will try to create the blob.
  if (Status != EFI_BUFFER_TOO_SMALL) {
    // Now that we've got the BlobId, let's attempt to set the blob.
    // If ANYTHING goes wrong with this, we should stop right now.
    BlobSize = InternalPayloadSize;
    Status = SetPersistedBlob( BlobIdGuid, &BlobSize, InternalPayload );
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - SetPersistedBlob() = %r\n", Status ));
      return EFI_DEVICE_ERROR;
    }
  }

  return InternalSetImage( InternalPayload, InternalPayloadSize, Progress );
}


/**
Device frimware should trigger lock mechanism so that device fw can not be updated or tampered with.
This lock mechanism is generally only cleared by a full system reset (not just sleep state/low power mode)

@retval EFI_SUCCESS           The device was successfully locked.
@retval EFI_UNSUPPORTED       The hardware device/firmware doesn't support locking

**/
EFI_STATUS
EFIAPI
FmpDeviceLock(
  VOID
  )
{
  // The locking that would be done here is handled by the
  // platform TPM driver.
  return EFI_UNSUPPORTED;
}


/**
  Retrieves the current attempt counter.
  NOTE: Returns 0 if not found.

  @param[out]   Count   The current count, if found. 0 if not found.

  @retval     EFI_SUCCESS             Count was successfully retrieved.
  @retval     EFI_INVALID_PARAMETER   Count is NULL.
  @retval     EFI_DEVICE_ERROR        GetVariable returned an error.

**/
STATIC
EFI_STATUS
GetRecoveryAttemptCount (
  OUT   UINT32    *Count
  )
{
  EFI_STATUS    Status;
  UINTN         CountSize;
  EFI_GUID      *FmpGuid;

  // Make sure we're not doing something dumb.
  if (Count == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Get the variable, why not.
  CountSize = sizeof( *Count );
  FmpGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
  Status = gRT->GetVariable( TPM_RECOVERY_ATTEMPT_COUNT_VAR,
                             FmpGuid,
                             NULL,
                             &CountSize,
                             Count );
  if (Status == EFI_NOT_FOUND) {
    *Count = 0;
    Status = EFI_SUCCESS;
  }
  if (EFI_ERROR( Status )) {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - GetVariable() = %r\n", Status ));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} // GetRecoveryAttemptCount()


/**
  Will create or update the current attempt counter.
  NOTE: Setting the Count to 0 will delete the count.

  @param[in]  Count       The updated count.

  @retval     EFI_SUCCESS             Count was successfully updated.
  @retval     EFI_DEVICE_ERROR        SetVariable returned an error.

**/
STATIC
EFI_STATUS
SetRecoveryAttemptCount (
  IN    UINT32    Count
  )
{
  EFI_STATUS    Status;
  UINT32        *UpCount;
  UINTN         UpCountSize;
  UINT32        Attributes;
  EFI_GUID      *FmpGuid;

  // Are we deleting or updating?
  if (Count == 0) {
    UpCount = NULL;
    UpCountSize = 0;
    Attributes = 0;
  }
  else {
    UpCount = &Count;
    UpCountSize = sizeof( Count );
    Attributes = (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS);
  }

  // Send the update.
  FmpGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
  Status = gRT->SetVariable( TPM_RECOVERY_ATTEMPT_COUNT_VAR,
                             FmpGuid,
                             Attributes,
                             UpCountSize,
                             UpCount );
  if (EFI_ERROR( Status )) {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - SetVariable() = %r\n", Status ));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} // SetRecoveryAttemptCount()


/**
  The driver's entry point.

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point executed successfully.
  @retval other           Some error occured when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiDeviceLibraryConstructor (
  IN    EFI_HANDLE                  ImageHandle,
  IN    EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  EFI_GUID      *BlobIdGuid;
  UINTN         BlobSize;
  UINT8         *BlobBuffer = NULL;
  UINT32        UpdatableStatus, RecoveryAttemptCount;

  DEBUG(( DEBUG_INFO, __FUNCTION__" - Initializing Infineon FMP...\n" ));

  // ERROR_HANDLING_RUN_ONCE
  do {
    // Initialize the support lib.
    Status = InitTpm12And20AbstractionLib();
    if (EFI_ERROR( Status )) {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to initialize the support lib!\n" ));
      break;
    }

    // Infineon Stack Step!
    // Allocate and zero the private context data structure
    g_pPrivateData = AllocateZeroPool( sizeof( IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA ) );
    if (g_pPrivateData == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }

    // Infineon Stack Step!
    // Seed the random number generator
    if (RC_SUCCESS != Crypt_SeedRandom(NULL, 0)) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    //===============================================================
    // RECOVERY BEGIN
    //  In this section of the entry point, we will check to see whether
    //  we have a failed update that we should recover from.
    //  If detected, we will attempt to restart the update.
    //  If successful, we will update status as though nothing had gone wrong.
    //  If failed, we will retry a certain number of times before failing
    //  permanently.
    //===============================================================

    //
    // Now we will check to see whether we have an update in progress.
    // If we do, we must re-attempt the update.
    //
    // In order to do this, we need to get a GUID to identify the blob.
    BlobIdGuid = (EFI_GUID*)PcdGetPtr( PcdDeviceLibWrapperDeviceGuid );
    BlobSize = 0;
    Status = GetPersistedBlob( BlobIdGuid, &BlobSize, NULL );
    if (Status == EFI_BUFFER_TOO_SMALL) {
      BlobBuffer = AllocatePool( BlobSize );
      if (BlobBuffer != NULL) {
        Status = GetPersistedBlob( BlobIdGuid, &BlobSize, BlobBuffer );
      }
    }

    //
    // If we found it, we need to check it and set it.
    //
    if (!EFI_ERROR( Status )) {
      DEBUG(( DEBUG_INFO, __FUNCTION__" - Recovery capsule data detected. Evaluating...\n" ));

      //
      // Verify the image.
      // We just got it out of flash, we don't know where it came from.
      // NOTE: This is already past the Capsule signature check, so hopefully
      //       the payload itself has some sort of signature checking on the device.
      //
      Status = InternalCheckImage( BlobBuffer, BlobSize, &UpdatableStatus );
      // Only if this passes should we try to set the image.
      if (!EFI_ERROR( Status ) && UpdatableStatus == IMAGE_UPDATABLE_VALID) {
        DEBUG(( DEBUG_INFO, __FUNCTION__" - Recovery capsule looks valid. Will attempt recovery.\n" ));

        ReportStatusCode( (EFI_ERROR_MAJOR | EFI_ERROR_CODE), MS_RSC_CAPSULE_TPM_RECOVERY_STARTED );

        //
        // Fun Fact:
        // Now that we feel confident that we have a valid recovery image,
        // we should do everything in our power to make sure the recovery
        // is completed. Handle errors accordingly.
        //

        // First, let's check and increment the attempt count (and create it if it doesn't exist).
        Status = GetRecoveryAttemptCount( &RecoveryAttemptCount );
        if (EFI_ERROR( Status )) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - GetRecoveryAttemptCount(%d) = %r\n", RecoveryAttemptCount, Status ));
          // We would still like to attempt to recover, so... soldier on.
          RecoveryAttemptCount = 0;
          Status = EFI_SUCCESS;
        }
        RecoveryAttemptCount++;

        // If we have reached our max... well.. damn...
        // Clean everything up and bail out of this path.
        if (RecoveryAttemptCount > TPM_MAX_RECOVERY_ATTEMPTS) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - Maximum number of recovery attempts exceeded!\n" ));
          // We need to purge the recovery buffer.
          BlobSize = 0;
          SetPersistedBlob( BlobIdGuid, &BlobSize, NULL );
          // We should clear the counter.
          SetRecoveryAttemptCount( 0 );
          // We should update the status.
          SetLastAttemptStatusInVariable( LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL );

          // Telemetry for this unexpected event.
          ReportStatusCode( (EFI_ERROR_MAJOR | EFI_ERROR_CODE), MS_RSC_CAPSULE_TPM_NO_MORE_RECOVERY_ATTEMPTS );

          break;
        }

        // If we're still here, update the recovery attempt count.
        Status = SetRecoveryAttemptCount( RecoveryAttemptCount );
        if (EFI_ERROR( Status )) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - SetRecoveryAttemptCount(%d) = %r\n", RecoveryAttemptCount, Status ));
          // We would still like to attempt to recover, so... soldier on.
          Status = EFI_SUCCESS;
        }

        // Let's make sure that the users can see something useful, if at all possible.
        Status = InitRecoveryProgressIndicator( PcdGet32( PcdProgressColor ), RECOVERY_BG_COLOR );
        if (EFI_ERROR( Status )) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - InitRecoveryProgressIndicator() = %r\n", Status ));
          // We would still like to attempt to recover, so... soldier on.
          Status = EFI_SUCCESS;
        }
        UpdateRecoveryProgressIndicator( 0 );     // Pop up the indicator with a complete background.

        //
        // MOST IMPORTANT STEP...
        // Actually attempt the recovery.
        //
        Status = InternalSetImage( BlobBuffer, BlobSize, RecoveryProgressUpdate );
        UpdateRecoveryProgressIndicator( 100 );   // Make sure the indicator is complete, no matter what.

        // If recovery was successful, we should probably update the status.
        if (!EFI_ERROR( Status )) {
          SetLastAttemptStatusInVariable( LAST_ATTEMPT_STATUS_SUCCESS );
          // We should clear the counter.
          SetRecoveryAttemptCount( 0 );
        }

        gRT->ResetSystem( EfiResetCold, EFI_SUCCESS, 0, NULL );
        break;    // This should never happen, actually.
      }
      else {
        DEBUG(( DEBUG_INFO, __FUNCTION__" - Recovery capsule looks invalid. Removing.\n" ));
        BlobSize = 0;
        Status = SetPersistedBlob( BlobIdGuid, &BlobSize, NULL );
        if (EFI_ERROR( Status )) {
          DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to clean up the persistence blob! %r\n", Status ));
          ASSERT( FALSE );
        }
      }
    }
    // If not found, that's cool.
    else if (Status == EFI_NOT_FOUND) {
      Status = EFI_SUCCESS;
      break;
    }

    //===============================================================
    // RECOVERY END
    //===============================================================

  } while (ERROR_HANDLING_RUN_ONCE);

  return Status;
} // UefiDeviceLibraryConstructor()
