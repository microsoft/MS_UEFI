/**
 *	@brief		Declares the Error Codes for all projects
 *	@details	This file contains definitions for all error and return codes.
 *	@file		ErrorCodes.h
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

/// Return code for a successful execution
#define RC_SUCCESS								0x00000000U	// General success return code

/// Error code mask for TPM errors
#define RC_TPM_MASK								0xE0280000U

/// Error code mask for tool errors
#define RC_APP_MASK								0xE0295000U

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/// General error code (0xE0295001)
#define RC_E_FAIL								RC_APP_MASK + 0x001
/// General internal error (0xE0295100)
#define RC_E_INTERNAL							RC_APP_MASK + 0x100
// Following error codes will be mapped to RC_E_INTERNAL
/// Error code for not initialized modules (0xE0295101)
#define RC_E_NOT_INITIALIZED					RC_E_INTERNAL + 0x01
/// Error code for not connected module e.g. TpmIO (0xE0295102)
#define RC_E_NOT_CONNECTED						RC_E_INTERNAL + 0x02
/// Error code for already connected module e.g. TpmIO (0xE0295103)
#define RC_E_ALREADY_CONNECTED					RC_E_INTERNAL + 0x03
/// Error code for Bad Parameters (0xE0295104)
#define RC_E_BAD_PARAMETER						RC_E_INTERNAL + 0x04
/// Buffer too small (0xE0295105)
#define RC_E_BUFFER_TOO_SMALL					RC_E_INTERNAL + 0x05
/// End of String (0xE029510A)
#define RC_E_END_OF_STRING						RC_E_INTERNAL + 0x0A
/// String not found (0xE029510B)
#define RC_E_NOT_FOUND							RC_E_INTERNAL + 0x0B
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/// General TPM device/connection error code (0xE0295200)
#define RC_E_NO_TPM								RC_APP_MASK + 0x200
// Following error codes will be mapped to RC_E_NO_TPM
/// Component not found. Used by TIS. (0xE0295201)
#define RC_E_COMPONENT_NOT_FOUND				RC_E_NO_TPM + 0x01
/// Not an active locality. Used by TIS. (0xE0295202)
#define RC_E_LOCALITY_NOT_ACTIVE				RC_E_NO_TPM + 0x02
/// Not a supported locality. Used by TIS. (0xE0295203)
#define RC_E_LOCALITY_NOT_SUPPORTED				RC_E_NO_TPM + 0x03
/// No data available. Used by TIS. (0xE0295204)
#define RC_E_TPM_NO_DATA_AVAILABLE				RC_E_NO_TPM + 0x04
/// Buffer insufficient. Used by TIS. (0xE0295205)
#define RC_E_INSUFFICIENT_BUFFER				RC_E_NO_TPM + 0x05
/// Error receiving data. Used by TIS. (0xE0295206)
#define RC_E_TPM_RECEIVE_DATA					RC_E_NO_TPM + 0x06
/// Error transmit data. Used by TIS. (0xE0295207)
#define RC_E_TPM_TRANSMIT_DATA					RC_E_NO_TPM + 0x07
/// TPM not ready. Used by TIS. (0xE0295208)
#define RC_E_NOT_READY							RC_E_NO_TPM + 0x08
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// TPM errors
/// General TPM error code (0xE0295300)
#define RC_E_TPM_GENERAL						RC_APP_MASK + 0x300
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// TPM Firmware Update failures
/// Error code mask for TPM Firmware Update codes (0xE0295500)
#define RC_E_TPM_FIRMWARE_UPDATE				RC_APP_MASK + 0x500
/// Error code for a wrong firmware image (0xE0295504)
#define RC_E_WRONG_FW_IMAGE						RC_E_TPM_FIRMWARE_UPDATE + 0x04
/// Error code for no Infineon TPM (0xE0295506)
#define RC_E_NO_IFX_TPM							RC_E_TPM_FIRMWARE_UPDATE + 0x06
/// Error code for not empty platform authentication (0xE0295507)
#define RC_E_PLATFORM_AUTH_NOT_EMPTY			RC_E_TPM_FIRMWARE_UPDATE + 0x07
/// Error code for disabled platform hierarchy (0xE0295508)
#define RC_E_PLATFORM_HIERARCHY_DISABLED		RC_E_TPM_FIRMWARE_UPDATE + 0x08
/// Error code for blocked firmware update (0xE0295509)
#define RC_E_FW_UPDATE_BLOCKED					RC_E_TPM_FIRMWARE_UPDATE + 0x09
/// Error code for failed update (0xE029550A)
#define RC_E_FIRMWARE_UPDATE_FAILED				RC_E_TPM_FIRMWARE_UPDATE + 0x0A
/// Error code for owned TPM1.2 (0xE029550B)
#define RC_E_TPM12_OWNED						RC_E_TPM_FIRMWARE_UPDATE + 0x0B
/// Error code for locked PP TPM1.2 (0xE029550C)
#define RC_E_TPM12_PP_LOCKED					RC_E_TPM_FIRMWARE_UPDATE + 0x0C
/// Error code for restart required (0xE029550F)
#define RC_E_RESTART_REQUIRED					RC_E_TPM_FIRMWARE_UPDATE + 0x0F
/// Error code for missing Deferred PP TPM1.2 (0xE0295510)
#define RC_E_TPM12_DEFERREDPP_REQUIRED			RC_E_TPM_FIRMWARE_UPDATE + 0x10
/// Error code for disabled / deactivated TPM1.2 (0xE0295511)
#define RC_E_TPM12_DISABLED_DEACTIVATED			RC_E_TPM_FIRMWARE_UPDATE + 0x11
/// Error code for TPM1.2 dictionary attack mode (0xE0295512)
#define RC_E_TPM12_DA_ACTIVE					RC_E_TPM_FIRMWARE_UPDATE + 0x12
/// Error code for newer firmware image (0xE0295513)
#define RC_E_NEWER_TOOL_REQUIRED				RC_E_TPM_FIRMWARE_UPDATE + 0x13
/// Error code for unsupported Infineon TPMs (0xE0295514)
#define RC_E_UNSUPPORTED_CHIP					RC_E_TPM_FIRMWARE_UPDATE + 0x14
/// Error code for a corrupt firmware image (0xE0295515)
#define RC_E_CORRUPT_FW_IMAGE					RC_E_TPM_FIRMWARE_UPDATE + 0x15
/// Error code for not matching decrypt keys between TPM and firmware image (0xE0295516)
#define RC_E_WRONG_DECRYPT_KEYS					RC_E_TPM_FIRMWARE_UPDATE + 0x16
/// Error code for an invalid config option (0xE0295517)
#define RC_E_INVALID_CONFIG_OPTION				RC_E_TPM_FIRMWARE_UPDATE + 0x17
/// Error code if an update firmware file could not be found (0xE0295518)
#define RC_E_FIRMWARE_UPDATE_NOT_FOUND			RC_E_TPM_FIRMWARE_UPDATE + 0x18

// Error code Ox19 is for tool internal use

/// Error code if the rundata file to resume interrupted firmware update is not available (0xE029551A)
#define RC_E_RESUME_RUNDATA_NOT_FOUND			RC_E_TPM_FIRMWARE_UPDATE + 0x1A

// Range from 0x1B to 0x1F can be used for new error codes.

// Error codes 0x20 and 0x21 is for tool internal use

/// Error code for an invalid owner authorization (0xE0295522)
#define RC_E_TPM12_INVALID_OWNERAUTH			RC_E_TPM_FIRMWARE_UPDATE + 0x22
/// Error code for an unowned TPM1.2 (0xE0295523)
#define RC_E_TPM12_NO_OWNER						RC_E_TPM_FIRMWARE_UPDATE + 0x23

// Error codes 0x24 to 0x27 are for tool internal use

/// Error code for an invalid access-mode configuration (0xE0295528)
#define RC_E_INVALID_ACCESS_MODE				RC_E_TPM_FIRMWARE_UPDATE + 0x28
/// Error code for TPM2.0 in Failure Mode (0xE0295529)
#define RC_E_TPM20_FAILURE_MODE					RC_E_TPM_FIRMWARE_UPDATE + 0x29
/// Error code for TPM in self-test failed mode (0xE029552A)
#define RC_E_TPM12_FAILED_SELFTEST				RC_E_TPM_FIRMWARE_UPDATE + 0x2A

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// TPM Firmware Update error codes for internal mapping
/// Internal error code for a config option triggered update that detects the
/// state "already up to date" (0xE0295528)
#define RC_E_ALREADY_UP_TO_DATE					RC_E_TPM_FIRMWARE_UPDATE + 0x19
/// Internal error code for invalid policy session (0xE0295520)
#define RC_E_TPM20_INVALID_POLICY_SESSION		RC_E_TPM_FIRMWARE_UPDATE + 0x20
/// Internal error code for missing owner authorization (0xE0295521)
#define RC_E_TPM12_MISSING_OWNERAUTH			RC_E_TPM_FIRMWARE_UPDATE + 0x21
/// Internal error code for TPM not in Boot Loader Mode (0xE0295524)
#define RC_E_TPM_NO_BOOT_LOADER_MODE			RC_E_TPM_FIRMWARE_UPDATE + 0x24
// 0x22 and 0x23 were moved from internal to external mapping
/// Internal error code for not loaded policy session (0xE0295525)
#define RC_E_TPM20_POLICY_SESSION_NOT_LOADED	RC_E_TPM_FIRMWARE_UPDATE + 0x25
/// Internal error code for policy handle out of range (0xE0295526)
#define RC_E_TPM20_POLICY_HANDLE_OUT_OF_RANGE	RC_E_TPM_FIRMWARE_UPDATE + 0x26
/// Internal error code for failed signature verification (0xE0295527)
#define RC_E_VERIFY_SIGNATURE					RC_E_TPM_FIRMWARE_UPDATE + 0x27
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
