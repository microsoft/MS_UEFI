/**
 *	@brief		Declares the EFI_ADAPTER_INFORMATION_PROTOCOL
 *	@details	Declares the functions of EFI_ADAPTER_INFORMATION_PROTOCOL in the Infineon TPM Firmware Update Driver.
 *				@cond !SHOW_INTERNAL @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification" contains more details on EFI_ADAPTER_INFORMATION_PROTOCOL.
 *				Functions are prefixed with @a IFXTPMUpdate_AdapterInformation_ in this document but not in @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification".
 *				@endcond
 *	@file		AdapterInformation.h
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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/AdapterInformation.h>

/**
 *	@brief		Returns the current state information for the adapter.
 *	@details	The function is not supported.
 *
 *	@param		PpThis						A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
 *	@param		PpInformationType			A pointer to an EFI_GUID that defines the contents of PppInformationBlock.
 *	@param		PppInformationBlock			The service returns a pointer to the buffer with the PppInformationBlock structure which contains details about the data
 *											specific to PpInformationType.
 *	@param		PpullInformationBlockSize	The driver returns the size of the PppInformationBlock in bytes.
 *
 *	@retval		EFI_UNSUPPORTED				The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetInformation(
	IN	EFI_ADAPTER_INFORMATION_PROTOCOL*	PpThis,
	IN	EFI_GUID*							PpInformationType,
	OUT VOID**								PppInformationBlock,
	OUT UINTN*								PpullInformationBlockSize);

/**
 *	@brief		Sets information that will be stored by the Infineon TPM Firmware Update Driver for further use by Firmware Management Protocols methods.
 *	@details	This function sends information of type PpInformationType for an adapter. The adapter supports the following information types:
 *				<table>
 *				<tr><th>Information Type</th><th>Description</th></tr>
 *				<tr>
 *				<td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID</td>
 *				<td>Use the information type to turn on logging functionality. The caller must pass a @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1 structure to configure
 *				a logging callback function.</td>
 *				</tr>
 *				<tr>
 *				<td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID</td>
 *				<td>Use the information type to set the TPM1.2 Owner Authorization for TPM Firmware Update if the TPM has an owner. Do not use the information type if the TPM is a TPM2.0 or if the TPM is a TPM1.2 and does not have an owner.
 *				The caller must pass a @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1 structure to set the owner authorization hash in the Infineon TPM Firmware Update Driver.</td>
 *				</tr>
 *				<tr>
 *				<td>@ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID</td>
 *				<td>Use the information type to set the authorized policy session handle for TPM2.0. Do not use the information type if the TPM is a TPM1.2.
 *				The caller must pass a @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1 structure and either set the policy session handle for an authorized policy session in TPM2.0
 *				or set policy session handle to 0 to make IFXTPMUpdate.efi create a default policy session.</td>
 *				</tr>
 *				</table>
 *				Otherwise EFI_UNSUPPORTED is returned.
 *
 *	@param		PpThis						A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
 *	@param		PpInformationType			A pointer to an EFI_GUID that defines the contents of PpInformationBlock.
 *	@param		PpInformationBlock			A pointer to the PpInformationBlock structure which contains details about the data specific to PpInformationType.
 *	@param		PullInformationBlockSize	The size of the PpInformationBlock in bytes.
 *
 *	@retval		EFI_SUCCESS										The information was received and interpreted successfully.
 *	@retval		EFI_DEVICE_ERROR								An unexpected error occurred.
 *	@retval		EFI_INVALID_PARAMETER							In case of an invalid input parameter.
 *	@retval		EFI_UNSUPPORTED									The PpInformationType is not known.
 *	@retval		EFI_IFXTPM_TPM12_DA_ACTIVE						The TPM Owner is locked out due to dictionary attack\n (The error code only applies to TPM1.2 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID).
 *	@retval		EFI_IFXTPM_TPM12_INVALID_OWNERAUTH				TPM Owner authentication is incorrect\n (The error code only applies to TPM1.2 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID).
 *	@retval		EFI_IFXTPM_TPM12_NO_OWNER						The TPM does not have an owner but TPM Owner authorization was provided\n (The error code only applies to TPM1.2 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID).
 *	@retval		EFI_IFXTPM_TPM20_FAILURE_MODE					The TPM2.0 is in failure mode.
 *	@retval		EFI_IFXTPM_TPM20_PLATFORMAUTH_NOT_EMPTYBUFFER	The platformAuth is not the Empty Buffer. In this case you cannot use the default policy.
 *	@retval		EFI_IFXTPM_TPM20_POLICYSESSION_NOT_LOADED		The policy session handle is not loaded in the TPM\n (The error code only applies to TPM2.0 and information type EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID).
 *	@retval		EFI_IFXTPM_UNSUPPORTED_CHIP						The Infineon TPM chip detected is not supported by the driver.
 *	@retval		EFI_IFXTPM_UNSUPPORTED_VENDOR					The TPM is not manufactured by Infineon. It is not supported by the driver.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_SetInformation(
	IN	EFI_ADAPTER_INFORMATION_PROTOCOL*	PpThis,
	IN	EFI_GUID*							PpInformationType,
	IN	VOID*								PpInformationBlock,
	IN	UINTN								PullInformationBlockSize);

/**
 *	@brief		Get a list of supported information types for the Infineon TPM Firmware Update Driver.
 *	@details	This function returns a list of InformationType GUIDs that are supported on Infineon TPM Firmware Update Driver with this
 *				instance of EFI_ADAPTER_INFORMATION_PROTOCOL. The list is returned in PppInfoTypesBuffer, and the number
 *				of GUID pointers in PppInfoTypesBuffer is returned in PpullInfoTypesBufferCount. The method returns
 *				the following information types:
 *				* @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID
 *				* @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID
 *				* @ref EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID
 *
 *	@param		PpThis						A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
 *	@param		PppInfoTypesBuffer			A pointer to the array of InformationType GUIDs that are supported by PpThis.
 *	@param		PpullInfoTypesBufferCount	A pointer to the number of GUIDs present in InfoTypesBuffer. *PpullInfoTypesBufferCount will be set to 3.
 *
 *	@retval		EFI_SUCCESS					The list of information type GUIDs that are supported on this adapter was returned in PppInfoTypesBuffer.
 *											The number of information type GUIDs was returned in PpullInfoTypesBufferCount.
 *	@retval		EFI_INVALID_PARAMETER		In case of an invalid or null parameter
 *	@retval		EFI_OUT_OF_RESOURCES		There is not enough pool memory to store the results.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_AdapterInformation_GetSupportedTypes(
	IN	EFI_ADAPTER_INFORMATION_PROTOCOL*	PpThis,
	OUT EFI_GUID**							PppInfoTypesBuffer,
	OUT UINTN*								PpullInfoTypesBufferCount);
