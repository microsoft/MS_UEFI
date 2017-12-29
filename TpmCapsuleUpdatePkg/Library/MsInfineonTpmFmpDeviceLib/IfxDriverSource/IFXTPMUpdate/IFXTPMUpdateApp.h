/**
 *	@brief		Declares general definitions for Infineon TPM Firmware Update Driver
 *	@details
 *	@file		IFXTPMUpdateApp.h
 *	@copyright	Copyright 2014 - 2017 Infineon Technologies AG ( www.infineon.com )
 *
 *	@copyright	All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once

#include "StdInclude.h"
#include "IFXTPMUpdate.h"
#include "FirmwareUpdate.h"

/// Definition of Locality 0 for accessing the TPM
#define LOCALITY_0			0

/// Maximum size of a TPM response
#define TPM_RESPONSE_BUF_MAXSIZE 1280

/// Global Variables
/// External global variable for Driver Binding Protocol
extern EFI_DRIVER_BINDING_PROTOCOL	g_IFXTPMUpdateDriverBinding;
/// External global variable for Component Name protocol
extern EFI_COMPONENT_NAME_PROTOCOL	g_IFXTPMUpdateComponentName;
/// External global variable for Component Name2 protocol
extern EFI_COMPONENT_NAME2_PROTOCOL	g_IFXTPMUpdateComponentName2;
/// External global variable for Adapter Information protocol
extern EFI_ADAPTER_INFORMATION_PROTOCOL g_IFXTPMUpdateAdapterInformation;
/// External global variable for Firmware Management protocol
extern EFI_FIRMWARE_MANAGEMENT_PROTOCOL g_IFXTPMUpdateFirmwareManagement;

/**
 *	@brief		TPM Firmware Update Private Data structure.
 *	@details	This structure is used to hold all private data for the running instance of IFXTPMUpdate.efi.
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

/// External global variable that points to private data of IFXTPMUpdate.efi
extern IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA* g_pPrivateData;

/**
 *	@brief		The function is not supported.
 *	@details
 *
 *	@param		PpThis					-
 *	@param		PhController			-
 *	@param		PpRemainingDevicePath	-
 *
 *	@retval		EFI_UNSUPPORTED			The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdateDriverBindingSupported(
	IN	EFI_DRIVER_BINDING_PROTOCOL*	PpThis,
	IN	EFI_HANDLE						PhController,
	IN	EFI_DEVICE_PATH_PROTOCOL*		PpRemainingDevicePath);

/**
 *	@brief		The function is not supported.
 *	@details
 *
 *	@param		PpThis					-
 *	@param		PhController			-
 *	@param		PpRemainingDevicePath	-
 *
 *	@retval		EFI_UNSUPPORTED			The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdateDriverBindingStart(
	IN	EFI_DRIVER_BINDING_PROTOCOL*	PpThis,
	IN	EFI_HANDLE						PhController,
	IN	EFI_DEVICE_PATH_PROTOCOL*		PpRemainingDevicePath);

/**
 *	@brief		The function is not supported.
 *	@details
 *
 *	@param		PpThis					-
 *	@param		PhController			-
 *	@param		PullNumberOfChildren	-
 *	@param		PpChildHandleBuffer		-
 *
 *	@retval		EFI_UNSUPPORTED			The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdateDriverBindingStop(
	IN	EFI_DRIVER_BINDING_PROTOCOL*	PpThis,
	IN	EFI_HANDLE						PhController,
	IN	UINTN							PullNumberOfChildren,
	IN	EFI_HANDLE*						PpChildHandleBuffer);
