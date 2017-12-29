/**
 *	@brief		Declares the EFI_COMPONENT_NAME_PROTOCOL and EFI_COMPONENT_NAME2_PROTOCOL
 *	@details	Declares the functions of EFI_COMPONENT_NAME_PROTOCOL and EFI_COMPONENT_NAME2_PROTOCOL in the Infineon TPM Firmware Update Driver.
 *				@cond !SHOW_INTERNAL @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification" contains more details on EFI_COMPONENT_NAME_PROTOCOL and EFI_COMPONENT_NAME2_PROTOCOL.
 *				Functions are prefixed with @a IFXTPMUpdate_ComponentName_ in this document but not in @ref secRefUEFISpec "Unified Extensible Firmware Interface Specification".
 *				@endcond
 *	@file		ComponentName.h
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
#include <Protocol/ComponentName.h>
#include <Protocol/ComponentName2.h>
#include "IFXTPMUpdateApp.h"

/// External global variable for Component Name protocol
extern EFI_COMPONENT_NAME_PROTOCOL	g_IFXTPMUpdateComponentName;
/// External global variable for Component Name 2 protocol
extern EFI_COMPONENT_NAME2_PROTOCOL	g_IFXTPMUpdateComponentName2;

/**
 *	@brief		Retrieves a Unicode string that is the user readable name of the driver.
 *	@details	This function retrieves the user readable name of IFXTPMUpdate driver in the form of a Unicode string. If language specified by PszLanguage
 *				is "eng;en" then a pointer to the driver name is returned in PpwszDriverName, and EFI_SUCCESS is returned. Otherwise EFI_UNSUPPORTED is returned.\n\n
 *				The driver name is: "Infineon TPM Firmware Update Driver"
 *
 *	@param		PpThis			A pointer to the EFI_COMPONENT_NAME2_PROTOCOL or EFI_COMPONENT_NAME_PROTOCOL instance.
 *	@param		PszLanguage		A pointer to a Null-terminated ASCII string array indicating the language. This is the
 *								language of the driver name that the caller is requesting. IFXTPMUpdate only supports "eng;en".
 *	@param		PpwszDriverName	A pointer to the Unicode string to return. This Unicode string is the name of the
 *								driver specified by PpThis in the language specified by PszLanguage.
 *
 *	@retval EFI_SUCCESS				The Unicode string for the Driver specified by PpThis and the language specified by PszLanguage was returned in PpwszDriverName.
 *	@retval EFI_INVALID_PARAMETER	PpThis, PszLanguage is NULL, or PpwszDriverName is NULL.
 *	@retval EFI_UNSUPPORTED			The driver specified by PpThis does not support the language specified by PszLanguage.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_ComponentName_GetDriverName(
	IN	EFI_COMPONENT_NAME_PROTOCOL*	PpThis,
	IN	CHAR8*							PszLanguage,
	OUT	CHAR16**						PpwszDriverName);

/**
 *	@brief		The function is not supported. IFXTPMUpdate does not manage controllers.
 *	@details
 *
 *	@param		PpThis				-
 *	@param		PhControllerHandle	-
 *	@param		PhChildHandle		-
 *	@param		PszLanguage			-
 *	@param		PpwszControllerName	-
 *
 *	@retval		EFI_UNSUPPORTED			The function is not supported.
 */
EFI_STATUS
EFIAPI
IFXTPMUpdate_ComponentName_GetControllerName(
	IN	EFI_COMPONENT_NAME_PROTOCOL*	PpThis,
	IN	EFI_HANDLE						PhControllerHandle,
	IN	EFI_HANDLE						PhChildHandle,
	IN	CHAR8*							PszLanguage,
	OUT	CHAR16**						PpwszControllerName);
