/**
 *	@brief		Declares defines and structures for IFXTPMUpdate.efi.
 *	@file		UEFI\IFXTPMUpdate.h
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

/*
 *	Driver specific flags and definitions for EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation function.
 */

/**
 *	@brief	Supported GUID for EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation function.
 *			Caller must pass an EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1 structure.
 */
#define EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1_GUID \
	{ 0xb7c7b35b, 0x1349, 0x46d2, {0x89, 0x5e, 0x2d, 0x48, 0x73, 0xbb, 0x33, 0xd1} }

/**
 *	@brief		Callback function for the Infineon TPM Firmware Update Driver to log details of the firmware update.
 *	@details	An OEM can implement this function to receive logging information from the Infineon TPM Firmware Update
 *				Driver. The callback function will be called before, during, and after the firmware update and provides
 *				information such as complete record of TPM communication.
 *
 *	@param	BufferSize		The size, in bytes, of the data in Buffer.
 *	@param	Buffer			A buffer that contains a Null-terminated Unicode string.
 *
 *	@retval	EFI_SUCCESS		The Buffer was logged successfully.
 **/
typedef
EFI_STATUS
(EFIAPI *EFI_IFXTPM_LOG_CALLBACK)(
	IN	UINTN			BufferSize,
	IN	CHAR16*			Buffer);

/**
 *	@brief		Infineon TPM Firmware Update Driver communication structure
 *	@details	This structure is used to communicate the logging parameters to the Infineon TPM Firmware Update Driver.
 */
typedef struct {
	/**
	 *	@brief	This pointer is used to configure a callback function for logging.\n
	 *			Set it to a valid function pointer to enable logging.\n
	 *			Set it to NULL to disable logging.
	 */
	EFI_IFXTPM_LOG_CALLBACK		LogCallback;
	/**
	 *	@brief	This flag enables or disables timestamps in the log output.
	 *			Set it to TRUE to prepend timestamps to logs, set it to FALSE otherwise.\n
	 *			Timestamps use the following format: YYYY-MM-DD hh:mm:ss.mmm\n
	 *			An example value is: [2014-12-31 23:59:59.999].
	 */
	BOOLEAN						AddTimeStamps;
} EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_LOGGING_1;

/**
 *	@brief	Supported GUID for EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation function.
 *			Caller must pass an EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1 structure.
 */
#define EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1_GUID \
	{ 0x2d8fcce2, 0xaf55, 0x42f9, {0x9d, 0xdb, 0x2a, 0xc5, 0x40, 0xdf, 0x12, 0x39} }

/**
 *	@brief		Infineon TPM Firmware Update Driver communication structure
 *	@details	This structure is used to communicate the parameters for the owned
 *				TPM1.2 firmware update to the Infineon TPM Firmware Update Driver.
 */
typedef struct {
	/**
	 *	@brief	SHA-1 hash of the TPM1.2 Owner password
	 */
	UINT8		OwnerPasswordSha1[20];
} EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM12_1;

/**
 *	@brief	Supported GUID for EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation function.
 *			Caller must pass an EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1 structure.
 */
#define EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1_GUID \
	{ 0x1b903ec7, 0xb325, 0x4f10, {0xa9, 0xdb, 0x7d, 0xf4, 0x64, 0x5c, 0x0f, 0xe1} }

/**
 *	@brief		Infineon TPM Firmware Update Driver communication structure
 *	@details	This structure is used to communicate additional parameters for
 *				firmware update of a TPM2.0 to the Infineon TPM Firmware Update Driver.
 */
typedef struct {
	/**
	 *	@brief	TPM2.0 authorized policy session handle (only required for TPM2.0 update).
	 */
	UINT32		SessionHandle;
} EFI_IFXTPM_FIRMWARE_UPDATE_DESCRIPTOR_TPM20_1;

/*
 *	Driver specific flags and definitions for EFI_FIRMWARE_MANAGEMENT_PROTOCOL.GetImageInfo function.
 */

/**
 *	@brief	Supported EFI_FIRMWARE_IMAGE_DESCRIPTOR.ImageTypeId in
 *			EFI_FIRMWARE_MANAGEMENT_PROTOCOL.GetImageInfo function.
 */
#define EFI_IFXTPM_FIRMWARE_TYPE_GUID \
	{ 0xb6b0baa6, 0x42bc, 0x4710, {0x91, 0x02, 0x3a, 0x6a, 0xf0, 0xeb, 0x86, 0x54} }

/**
 *	@brief	Image is updatable only one more time.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE				0x0000100000000000

/**
 *	@brief	The TPM1.2 has an owner.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_HAS_OWNER				0x0000200000000000

/**
 *	@brief	The TPM is in invalid firmware mode because a previous update was interrupted.
 *			Firmware update must be retried with the same firmware image.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_INVALID_FIRMWARE_MODE	0x0000400000000000

/**
 *	@brief	The TPM is a TPM2.0.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_2_0						0x0000800000000000

/**
 *	@brief	The TPM is a TPM1.2.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_1_2						0x0001000000000000

/**
 *	@brief	Due to the current TPM state the attributes @ref IMAGE_ATTRIBUTE_IFXTPM_LAST_UPDATE and
 *			IMAGE_ATTRIBUTE_IMAGE_UPDATABLE cannot be determined. The system must be restarted to
 *			read these attributes.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_RESTART_REQUIRED			0x0002000000000000

/**
 *	@brief	The TPM1.2 has asserted Deferred Physical Presence.
 */
#define IMAGE_ATTRIBUTE_IFXTPM_DEFERREDPP				0x0004000000000000

/*
 *	Driver specific flags for EFI_FIRMWARE_MANAGEMENT_PROTOCOL.CheckImage function.
 */

/**
 *	@brief	SetImage() will change the TPM device type. For example:\n
 *			- SetImage() will update a TPM1.2 to a TPM2.0.
 *			- SetImage() will update a TPM2.0 to a TPM1.2.
 */
#define IMAGE_UPDATABLE_IFXTPM_DEVICETYPE_CHANGE		0x10000000

/**
 *	@brief	The firmware image is corrupt.
 */
#define IMAGE_UPDATABLE_IFXTPM_INVALID_IMAGE_CORRUPTED	0x20000000

/**
 *	@brief	The firmware image is not compatible with this version of the driver.
 *			A newer version of the driver is required to read the image.
 */
#define IMAGE_UPDATABLE_IFXTPM_NEWER_DRIVER_REQUIRED	0x40000000

/**
 *	@brief	SetImage() will update the TPM and reset the TPM to factory defaults.
 *			All data and state from the old TPM firmware will be lost. The appearance of this
 *			depends on the actual source and target firmware of the firmware update and can
 *			occur in both, cross version and normal update paths.
 */
#define IMAGE_UPDATABLE_IFXTPM_FACTORY_DEFAULTS		0x80000000

/*
 *	Driver specific error codes (0xE029000000000001 - 0xE029FFFFFFFFFFFF)
 */

/**
 *	@brief	The firmware image is corrupt.
 */
#define EFI_IFXTPM_CORRUPT_FIRMWARE_IMAGE				0xE029000000006001

/**
 *	@brief	The firmware image cannot be used to update the TPM firmware.
 *			The firmware image could be used to update a different TPM firmware.
 */
#define EFI_IFXTPM_WRONG_FIRMWARE_IMAGE					0xE029000000006002

/**
 *	@brief	Cannot update the TPM2.0 firmware on the TPM because the policy
 *			session handle is invalid or the policy authorization failed.
 */
#define EFI_IFXTPM_TPM20_INVALID_POLICYSESSION			0xE029000000006003

/**
 *	@brief	Cannot update the TPM1.2 firmware on the TPM because Deferred Physical Presence is not asserted.
 */
#define EFI_IFXTPM_TPM12_DEFERREDPP_REQUIRED			0xE029000000006004

/**
 *	@brief	Cannot update the TPM1.2 firmware on the TPM because the TPM has an owner and TPM Owner
 *			authorization value was not set with EFI_ADAPTER_INFORMATION_PROTOCOL.SetInformation.
 */
#define EFI_IFXTPM_TPM12_MISSING_OWNERAUTH				0xE029000000006005

/**
 *	@brief	The firmware update operation was started but failed.
 */
#define EFI_IFXTPM_FIRMWARE_UPDATE_FAILED				0xE029000000006006

/**
 *	@brief	The TPM is not manufactured by Infineon. It is not supported by the driver.
 */
#define EFI_IFXTPM_UNSUPPORTED_VENDOR					0xE029000000006007

/**
 *	@brief	The TPM firmware can no longer be updated.
 */
#define EFI_IFXTPM_NO_MORE_UPDATES						0xE029000000006008

/**
 *	@brief	Cannot update the TPM1.2 firmware on the TPM because TPM Owner authorization failed.
 */
#define EFI_IFXTPM_TPM12_INVALID_OWNERAUTH				0xE029000000006009

/**
 *	@brief	Cannot update the TPM1.2 firmware on the TPM with TPM Owner
 *			authorization because the TPM does not have an owner.
 */
#define EFI_IFXTPM_TPM12_NO_OWNER						0xE02900000000600A

/**
 *	@brief	The system must be restarted before the TPM can be used.
 */
#define EFI_IFXTPM_RESTART_REQUIRED						0xE02900000000600B

/**
 *	@brief	Cannot update the TPM1.2 firmware on the TPM with TPM Owner authorization because
 *			TPM Owner is locked out due to dictionary attack.
 */
#define EFI_IFXTPM_TPM12_DA_ACTIVE						0xE02900000000600C

/**
 *	@brief	The firmware image is not compatible with this version of the driver.
 *			A newer version of the driver is required to read the firmware image.
 */
#define EFI_IFXTPM_NEWER_DRIVER_REQUIRED				0xE02900000000600D

/**
 *	@brief	The Infineon TPM chip detected is not supported by the driver.
 */
#define EFI_IFXTPM_UNSUPPORTED_CHIP						0xE02900000000600E

/**
 *	@brief	The policy session is not loaded to the TPM.
 */
#define EFI_IFXTPM_TPM20_POLICYSESSION_NOT_LOADED		0xE02900000000600F

/**
 *	@brief	The policy session handle value is out of range.
 */
#define EFI_IFXTPM_TPM20_POLICY_HANDLE_OUT_OF_RANGE		0xE029000000006010

/**
 *	@brief	The TPM1.2 is deactivated. It needs to be activated for firmware update.
 */
#define EFI_IFXTPM_TPM12_DEACTIVATED					0xE029000000006011

/**
 *	@brief	The TPM1.2 is disabled. It needs to be enabled for firmware update.
 */
#define EFI_IFXTPM_TPM12_DISABLED						0xE029000000006012

/**
 *	@brief	The TPM2.0 is in failure mode.
 */
#define EFI_IFXTPM_TPM20_FAILURE_MODE					0xE029000000006013

/**
 *	@brief	The TPM1.2 failed the self-test.
 */
#define EFI_IFXTPM_TPM12_FAILED_SELFTEST				0xE029000000006014

 /**
 *	@brief	The TPM2.0 platformAuth is not the Empty Buffer.
 */
#define EFI_IFXTPM_TPM20_PLATFORMAUTH_NOT_EMPTYBUFFER	0xE029000000006015

/**
 *	@brief	The TPM2.0 platform hierarchy is disabled.
 */
#define EFI_IFXTPM_TPM20_PLATFORMHIERARCHY_DISABLED		0xE029000000006016

 /**
 *	@brief	Command code for vendor specific command TPM2_FieldUpgradeStartVendor.
 */
#define TPM_CC_FieldUpgradeStartVendor				0x2000012F
