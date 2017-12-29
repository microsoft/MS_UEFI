/**
 *	@brief		Implements the driver binding for IFXTPMUpdate
 *	@details	Implementation file for the Infineon TPM Firmware Update Driver.
 *				The IFXTPMUpdate driver implements the TPM Firmware Update functionality.
 *	@file		IFXTPMUpdate.c
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

#include "StdInclude.h"
#include "IFXTPMUpdateApp.h"
#include "Crypt.h"

IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA* g_pPrivateData = NULL;

/**
 *	@brief		Initialization of the external driver binding structure
 *	@details	Initializes the driver binding structure with the function pointer and values of the current driver implementation.
 */
EFI_DRIVER_BINDING_PROTOCOL g_IFXTPMUpdateDriverBinding = {
	IFXTPMUpdateDriverBindingSupported,
	IFXTPMUpdateDriverBindingStart,
	IFXTPMUpdateDriverBindingStop,
	0x010109E4, // Lowest IHV value
	NULL,
	NULL
};

/**
 *	@brief		The user Entry Point for module IFXTPMUpdate.
 *	@details	The user code starts with this function.
 *
 *	@param		PhImageHandle			The firmware allocated handle for the EFI image.
 *	@param		PpSystemTable			A pointer to the EFI System Table.
 *
 *	@retval		EFI_SUCCESS				The entry point is executed successfully.
 *	@retval		EFI_OUT_OF_RESOURCES	In case of an allocate memory failed.
 *	@retval		EFI_DEVICE_ERROR		In case of a TPM communication error.
 *	@retval		other					An error occurred when executing this function.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdateEntryPoint(
	IN	EFI_HANDLE				PhImageHandle,
	IN	EFI_SYSTEM_TABLE*		PpSystemTable)
{
	EFI_STATUS efiStatus = EFI_UNSUPPORTED;

	do
	{
		// Install driver model protocol(s).
		efiStatus = EfiLibInstallDriverBindingComponentName2(
						PhImageHandle,
						PpSystemTable,
						&g_IFXTPMUpdateDriverBinding,
						PhImageHandle,
						&g_IFXTPMUpdateComponentName,
						&g_IFXTPMUpdateComponentName2);
		if (EFI_SUCCESS != efiStatus)
		{
			break;
		}

		// Install the TPM Firmware Management Protocol (and others) onto an existing handle.
		efiStatus = gBS->InstallMultipleProtocolInterfaces(
						&PhImageHandle,
						&gEfiFirmwareManagementProtocolGuid,
						&g_IFXTPMUpdateFirmwareManagement,
						&gEfiAdapterInformationProtocolGuid,
						&g_IFXTPMUpdateAdapterInformation,
						NULL);
		if (EFI_SUCCESS != efiStatus)
			break;

		// Allocate and zero the private context data structure
		g_pPrivateData = (IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA*)AllocateZeroPool(sizeof(IFX_TPM_FIRMWARE_UPDATE_PRIVATE_DATA));
		if (NULL == g_pPrivateData)
		{
			efiStatus = EFI_OUT_OF_RESOURCES;
			break;
		}

		// Seed the random number generator
		if (RC_SUCCESS != Crypt_SeedRandom(NULL, 0))
		{
			efiStatus = EFI_DEVICE_ERROR;
			break;
		}
	}
	WHILE_FALSE_END;

	return efiStatus;
}

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
	IN	EFI_DEVICE_PATH_PROTOCOL*		PpRemainingDevicePath)
{
	return EFI_UNSUPPORTED;
}

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
	IN	EFI_DEVICE_PATH_PROTOCOL*		PpRemainingDevicePath)
{
	return EFI_UNSUPPORTED;
}

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
	IN	EFI_HANDLE*						PpChildHandleBuffer)
{
	return EFI_UNSUPPORTED;
}

/**
 *	@brief		Unloads the driver.
 *	@details	Disconnect the driver specified by PhImageHandle from all the devices in the handle database.
 *				Uninstall all the protocols installed in the driver entry point.
 *
 *	@param		PhImageHandle	The drivers' driver image.
 *
 *	@retval		EFI_SUCCESS		The image is unloaded.
 *	@retval		others			Failed to unload the image.
 */
EFI_STATUS
EFIAPI
DefaultUnload(
	IN EFI_HANDLE PhImageHandle)
{
	EFI_STATUS	efiStatus;

	do
	{
		// Uninstall multiple protocols. Ignore return value
		efiStatus = gBS->UninstallMultipleProtocolInterfaces(
						PhImageHandle,
						&gEfiFirmwareManagementProtocolGuid,	&g_IFXTPMUpdateFirmwareManagement,
						&gEfiAdapterInformationProtocolGuid,	&g_IFXTPMUpdateAdapterInformation,
						&gEfiComponentNameProtocolGuid,			&g_IFXTPMUpdateComponentName,
						&gEfiComponentName2ProtocolGuid,		&g_IFXTPMUpdateComponentName2,
						&gEfiDriverBindingProtocolGuid,			&g_IFXTPMUpdateDriverBinding,
						NULL);

		efiStatus = EFI_SUCCESS;
	}
	WHILE_FALSE_END;

	// Free the private context data structure.
	if (NULL != g_pPrivateData)
	{
		FreePool(g_pPrivateData);
		g_pPrivateData = NULL;
	}

	return efiStatus;
}
