/** @file -- BorrowedInfineonFunctions.h
Header for functions from the Infineon driver that are referenced
directly. Included here to keep them separate from their respective dependencies
and header file hierarchies.

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

#ifndef _BORROWED_INFINEON_FUNCTIONS_H_
#define _BORROWED_INFINEON_FUNCTIONS_H_

#define   INACTIVE_STALE_VERSION      0xFFFFEEEE

/**
 *  @brief    Callback function for the Infineon TPM Firmware Update Driver to log details of the firmware update.
 *  @details  An OEM can implement this function to receive logging information from the Infineon TPM Firmware Update
 *        Driver. The callback function will be called before, during, and after the firmware update and provides
 *        information such as complete record of TPM communication.
 *
 *  @param  BufferSize    The size, in bytes, of the data in Buffer.
 *  @param  Buffer      A buffer that contains a Null-terminated Unicode string.
 *
 *  @retval EFI_SUCCESS   The Buffer was logged successfully.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_IFXTPM_LOG_CALLBACK)(
  IN  UINTN     BufferSize,
  IN  CHAR16*     Buffer);

/// Function pointer type definition for Response_ProgressCallback
typedef unsigned long long (*PFN_FIRMWAREUPDATE_PROGRESSCALLBACK)(
  unsigned long long PullCompletion);

/// Function pointer type definition for UpdateStarted_Callback
typedef void (*PFN_FIRMWAREUPDATE_UPDATESTARTEDCALLBACK)(
  );

/**
 *  @brief    TPM Firmware Update Private Data structure.
 *  @details  This structure is used to hold all private data for the running instance of IFXTPMUpdate.efi.
 */
typedef struct tdIFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA
{
  /// Stores the SessionHandle which is set in EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation()
  UINT32 unSessionHandle;
  /// Stores whether TPM access has been initialized
  BOOLEAN fTpmAccessInitialized;
  /// Stores the pointer to the log call back function
  EFI_IFXTPM_LOG_CALLBACK pfnLogCallback;
  /// Stores whether logging should include time stamps
  BOOLEAN fLogTimeStamps;
  /// Stores whether an owned firmware update shall be executed
  BOOLEAN fOwnedUpdate;
  /// Stores the TPM1.2 Owner password hash
  UINT8 rgbOwnerPasswordSha1[SHA1_DIGEST_SIZE];
} IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA;

/**
 *  @brief    TPM state after firmware update used in FirmwareUpdate interface methods.
 *  @details  TPM state after firmware update used in FirmwareUpdate interface methods.
 */
typedef struct tdBITFIELD_NEW_TPM_FIRMWARE_INFO
{
  /// Firmware update will change the TPM device type. For example:\n
  /// - Firmware update will update a TPM1.2 to a TPM2.0.
  /// - Firmware update will update a TPM2.0 to a TPM1.2.
  unsigned int deviceTypeChange : 1;
  /// Firmware update will update the TPM and reset the TPM to factory defaults. All data and state from the
  /// old TPM firmware will be lost. The appearance of this depends on the actual source and target
  /// firmware of the firmware update and can occur in both, cross version and normal update paths.
  unsigned int factoryDefaults : 1;
} BITFIELD_NEW_TPM_FIRMWARE_INFO;

/**
 *  @brief    Firmware Update Data structure
 *  @details  This structure is used to hand over the firmware update related data.
 */
typedef struct tdIfxFirmwareUpdateData
{
  /// Firmware image size
  UINT32 unFirmwareImageSize;
  /// Pointer to a Firmware image
  UINT8* rgbFirmwareImage;
  /// Progress call back function pointer
  PFN_FIRMWAREUPDATE_PROGRESSCALLBACK fnProgressCallback;
  /// Update started call back function pointer
  PFN_FIRMWAREUPDATE_UPDATESTARTEDCALLBACK fnUpdateStartedCallback;
  /// Session handle used for updating a TPM2.0
  unsigned int unSessionHandle;
  /// TPM Owner authentication hash value used for updating a TPM1.2 with TPM Owner authorization
  UINT8 rgbOwnerAuthHash[SHA1_DIGEST_SIZE];
} IfxFirmwareUpdateData;

/// This value indicates that the number of remaining firmware updates is unknown
#define REMAINING_UPDATES_UNAVAILABLE (unsigned int)(-1)

/**
 *  @brief    Checks if the firmware image is valid for the TPM
 *  @details  Performs integrity, consistency and content checks to determine if the given firmware image can be applied to the installed TPM.
 *
 *  @param    PrgbImage         Firmware image byte stream
 *  @param    PullImageSize       Size of firmware image byte stream
 *  @param    PpfValid          TRUE in case the image is valid, FALSE otherwise.
 *  @param    PpbfNewTpmFirmwareInfo    Pointer to a bit field to return info data for the new firmware image.
 *  @param    PpunErrorDetails      Pointer to an unsigned int to return error details. Possible values are:\n
 *                        RC_E_FW_UPDATE_BLOCKED in case the field upgrade counter value has been exceeded.\n
 *                        RC_E_WRONG_FW_IMAGE in case the TPM is not updatable with the given image.\n
 *                        RC_E_CORRUPT_FW_IMAGE in case the firmware image is corrupt.\n
 *                        RC_E_NEWER_TOOL_REQUIRED in case a newer version of the tool is required to parse the firmware image.\n
 *                        RC_E_WRONG_DECRYPT_KEYS in case the TPM2.0 does not have decrypt keys matching to the firmware image.
 *
 *  @retval   RC_SUCCESS          The operation completed successfully
 *  @retval   RC_E_BAD_PARAMETER      An invalid parameter was passed to the function.
 *  @retval   RC_E_TPM20_FAILURE_MODE   The TPM2.0 is in failure mode.
 *  @retval   RC_E_RESTART_REQUIRED   In case a restart is required to get back to a functional TPM
 *  @retval   RC_E_NO_IFX_TPM       In case TPM vendor is not IFX
 *  @retval   RC_E_FAIL         An unexpected error occurred.
 *  @retval   ...             Error codes from called functions.
 */
unsigned int
FirmwareUpdate_CheckImage(
  IN      UINT8*             PrgbImage,
  IN      UINT64              PullImageSize,
  OUT     int*             PpfValid,      // Reassigned from BOOL
  OUT     BITFIELD_NEW_TPM_FIRMWARE_INFO* PpbfNewTpmFirmwareInfo,
  OUT     UINT32*             PpunErrorDetails);

/**
 *  @brief    Function to update the firmware with the given firmware image
 *  @details  This function updates the TPM firmware with the image given in the parameters.
 *        A check if the firmware can be updated with the image is done before.
 *
 *  @param    PpsFirmwareUpdateData Pointer to structure containing all relevant data for a firmware update
 *
 *  @retval   RC_SUCCESS            The operation completed successfully.
 *  @retval   RC_E_CORRUPT_FW_IMAGE     Policy parameter block is corrupt and/or cannot be unmarshalled.
 *  @retval   RC_E_NO_IFX_TPM         The TPM is not manufactured by Infineon.
 *  @retval   RC_E_TPM12_MISSING_OWNERAUTH  The TPM has an owner but TPM Owner authorization was not provided (TPM1.2 only).
 *  @retval   RC_E_TPM12_NO_OWNER       The TPM does not have an owner but TPM Owner authorization was provided (TPM1.2 only).
 *  @retval   RC_E_FAIL           TPM connection or command error.
 *  @retval   ...               Error codes from called functions.
 *  @retval   RC_E_FIRMWARE_UPDATE_FAILED   The update operation was started but failed.
 */
unsigned int
FirmwareUpdate_UpdateImage(
  IN  const IfxFirmwareUpdateData* const  PpsFirmwareUpdateData);

/**
 *  @brief    Prepares a policy session for TPM firmware.
 *  @details  The function prepares a policy session for TPM Firmware Update.
 *
 *  @param    PphPolicySession          Pointer to session handle that will be filled in by this method.
 *
 *  @retval   RC_SUCCESS              The operation completed successfully.
 *  @retval   RC_E_BAD_PARAMETER          An invalid parameter was passed to the function. It is invalid or NULL.
 *  @retval   RC_E_FAIL             An unexpected error occurred.
 *  @retval   RC_E_PLATFORM_AUTH_NOT_EMPTY    In case PlatformAuth is not the Empty Buffer.
 *  @retval   RC_E_PLATFORM_HIERARCHY_DISABLED  In case platform hierarchy has been disabled.
 *  @retval   ...                 Error codes from called functions. like TPM error codes
 */
unsigned int
FirmwareUpdate_PrepareTPM20Policy(
  OUT TPMI_SH_AUTH_SESSION* PphPolicySession);

/**
 *  @brief    Seed the pseudo random number generator
 *  @details  This function seeds the pseudo random number generator.
 *
 *  @param    PrgbSeed        Seed
 *  @param    PusSeedSize       Seed size in bytes
 *
 *  @retval   RC_SUCCESS        The operation completed successfully.
 *  @retval   RC_E_FAIL       An unexpected error occurred.
 *  @retval   RC_E_BAD_PARAMETER    An invalid parameter was passed to the function. PrgbSeed is NULL and PusSeedSize is not 0.
 */
unsigned int
Crypt_SeedRandom(
  IN CONST UINT8  *PrgbSeed,
  IN CONST UINT16 PusSeedSize);

/**
 *  @brief    Initializes access to the TPM.
 *  @details
 *
 *  @retval   EFI_SUCCESS             TPM access initialized successfully or already initialized.
 *  @retval   EFI_DEVICE_ERROR          Failed to initialize TPM access.
 *  @retval   EFI_OUT_OF_RESOURCES        Not enough memory.
 *  @retval   EFI_IFXTPM_UNSUPPORTED_VENDOR   The TPM is not manufactured by Infineon. It is not supported by the driver.
 */
EFI_STATUS
InitializeTpmAccess();

#endif // _BORROWED_INFINEON_FUNCTIONS_H_
