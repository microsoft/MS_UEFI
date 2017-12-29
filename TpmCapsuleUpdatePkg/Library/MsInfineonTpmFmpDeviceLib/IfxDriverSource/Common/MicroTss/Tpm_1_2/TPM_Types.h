/**
 *	@brief		Declares definitions for TPM types and structures
 *	@details
 *	@file		TPM_Types.h
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
#include "TPM2_Types.h"

// Maximum sizes for dynamic command response structure elements
// (Values are arbitrary and may be changed if necessary.)
#define ST_EL_BIG_SZ	100
#define ST_EL_SML_SZ	50

/// Typedef and defines for TPM_TAG
typedef UINT16						TPM_TAG;
#define TPM_TAG_RQU_COMMAND			(TPM_TAG) 0x00C1
#define TPM_TAG_RQU_AUTH1_COMMAND	(TPM_TAG) 0x00C2	// An authenticated command with one authentication handle
#define TPM_TAG_RSP_COMMAND			(TPM_TAG) 0x00C4

/// Typedef and defines for TPM_COMMAND_CODE
typedef UINT32							TPM_COMMAND_CODE;
#define TPM_ORD_OIAP					(TPM_COMMAND_CODE) 0x0000000A
#define TPM_ORD_OSAP					(TPM_COMMAND_CODE) 0x0000000B
#define TPM_ORD_ChangeAuthOwner			(TPM_COMMAND_CODE) 0x00000010
#define TPM_ORD_TakeOwnership			(TPM_COMMAND_CODE) 0x0000000D
#define TPM_ORD_SetCapability			(TPM_COMMAND_CODE) 0x0000003F
#define TPM_ORD_GetTestResult			(TPM_COMMAND_CODE) 0x00000054
#define TPM_ORD_GetCapability			(TPM_COMMAND_CODE) 0x00000065
#define TPM_ORD_ReadPubEK				(TPM_COMMAND_CODE) 0x0000007C
#define TPM_ORD_OwnerReadInternalPub	(TPM_COMMAND_CODE) 0x00000081
#define TPM_ORD_Startup					(TPM_COMMAND_CODE) 0x00000099
#define TPM_ORD_FlushSpecific			(TPM_COMMAND_CODE) 0x000000BA
#define TPM_ORD_OwnerClear				(TPM_COMMAND_CODE) 0x0000005B
#define TSC_ORD_PhysicalPresence		(TPM_COMMAND_CODE) 0x4000000A

/// Defines for TPM Protocol types
#define TPM_PID_OWNER		0x0005

/// Defines for TPM_RESOURCE_TYPE
#define TPM_RT_AUTH				0x00000002 ///< The handle is an authorization handle. Authorization handles come from TPM_OIAP, TPM_OSAP and TPM_DSAP

/// Typedef for TPM_RESULT
typedef UINT32				TPM_RESULT;
#define TPM_BASE			0x00000000
#define TPM_NON_FATAL		0x800
/// Self-test has failed and the TPM has shutdown
#define TPM_AUTHFAIL			((TPM_RESULT)(TPM_BASE + 0x01))
#define TPM_BAD_PARAMETER		((TPM_RESULT)(TPM_BASE + 0x03))
#define TPM_DEACTIVATED			((TPM_RESULT)(TPM_BASE + 0x06))
#define TPM_DISABLED			((TPM_RESULT)(TPM_BASE + 0x07))
#define TPM_FAIL				((TPM_RESULT)(TPM_BASE + 0x09))
#define TPM_BAD_ORDINAL			((TPM_RESULT)(TPM_BASE + 0x0A))
#define TPM_BAD_PARAM_SIZE		((TPM_RESULT)(TPM_BASE + 0x19))
#define TPM_NOSRK				((TPM_RESULT)(TPM_BASE + 0x12))
#define TPM_RESOURCES			((TPM_RESULT)(TPM_BASE + 0x15))
#define TPM_FAILEDSELFTEST		((TPM_RESULT)(TPM_BASE + 0x1C))
#define TPM_BADTAG				((TPM_RESULT)(TPM_BASE + 0x1E))
#define TPM_INVALID_POSTINIT	((TPM_RESULT)(TPM_BASE + 0x26))
#define TPM_BAD_PRESENCE		((TPM_RESULT)(TPM_BASE + 0x2D))
#define TPM_DEFEND_LOCK_RUNNING	((TPM_RESULT)(TPM_BASE + TPM_NON_FATAL + 0x003))

/// Typedef and defines for TPM_STARTUP_TYPE
typedef UINT16					TPM_STARTUP_TYPE;
#define TPM_ST_CLEAR			(TPM_STARTUP_TYPE) 0x0001
#define TPM_ST_STATE			(TPM_STARTUP_TYPE) 0x0002
#define TPM_ST_DEACTIVATED		(TPM_STARTUP_TYPE) 0x0003

/// Typedef and defines for TPM_CAPABILITY_AREA
typedef UINT32					TPM_CAPABILITY_AREA;
#define TPM_CAP_FLAG			0x00000004
#define TPM_CAP_PROPERTY		0x00000005
#define TPM_CAP_DA_LOGIC		0x00000019
#define TPM_CAP_VERSION_VAL		0x0000001A
#define TPM_CAP_FLAG_PERMANENT	0x00000108
#define TPM_CAP_FLAG_VOLATILE	0x00000109
#define TPM_CAP_PROP_OWNER		0x00000111
#define TPM_SET_PERM_FLAGS		0x00000001

/// Defines for Permanent Flags
#define TPM_PF_READPUBEK		0x00000004
#define TPM_TAG_PERMANENT_FLAGS	0x001f
#define TPM_TAG_STCLEAR_FLAGS	0x0020

/// Defines for TPM_SetCapability
#define TPM_SET_STCLEAR_DATA				0x00000004 ///< The ability to set a value is field specific and a review of the structure will disclose the ability and requirements to set a value
#define TPM_SD_DEFERREDPHYSICALPRESENCE		0x00000006

/// Defines for TPM_PHYSICAL_PRESENCE
#define TPM_PHYSICAL_PRESENCE_CMD_ENABLE	0x0020 ///< Sets the physicalPresenceCMDEnable to TRUE
#define TPM_PHYSICAL_PRESENCE_PRESENT		0x0008 ///< Sets PhysicalPresence = TRUE
#define TPM_PHYSICAL_PRESENCE_NOTPRESENT	0x0010 ///< Sets PhysicalPresence = FALSE
#define TPM_PHYSICAL_PRESENCE_LOCK			0x0004 ///< Sets PhysicalPresenceLock = TRUE

/// Defines for FieldUpgradeInfoRequest
#define OWNER_SET_FLAG		0x0001

/// Defines for TPM_KEY_USAGE
#define TPM_KEY_STORAGE		0x0011

/// Defines for TPM_AUTH_DATA_USAGE
#define TPM_AUTH_ALWAYS		0x01

/// Defines for TPM_SIG_SCHEME
#define TPM_SS_NONE			0x0001

/// Defines for dictionary attack
#define TPM_ET_XOR				0x00
#define TPM_ET_OWNER			0x02
#define TPM_TAG_DA_INFO			0x37
#define TPM_TAG_DA_INFO_LIMITED	0x38
#define TPM_DA_STATE_INACTIVE	0x00 ///< The dictionary attack mitigation logic is currently inactive
#define TPM_DA_STATE_ACTIVE		0x01 ///< The dictionary attack mitigation logic is active. TPM_DA_ACTION_TYPE (21.10) is in progress.

#define TPM_KH_OWNER			0x40000001 ///< The handle points to the TPM Owner
#define TPM_KH_EK				0x40000006 ///< The handle points to the PUBEK, only usable with TPM_OwnerReadInternalPub

#define TPM_PID_ADCP			0X0004 ///< The ADCP protocol.

/// Typedef for TPM_STRUCTURE_TAG;
typedef UINT16 TPM_STRUCTURE_TAG;

/// Typedef for TPM_VERSION_BYTE;
typedef BYTE TPM_VERSION_BYTE;

/// Typedef for TPM_AUTHHANDLE.
/// Handle to an authorization session
typedef UINT32 TPM_AUTHHANDLE;

/// Typedef for TPM_RESOURCE_TYPE.
/// The types of resources that a TPM may have using internal resources
typedef UINT32 TPM_RESOURCE_TYPE;

/// Typedef for TPM_ENC_SCHEME.
/// The definition of the encryption scheme
typedef UINT16 TPM_ENC_SCHEME;

/// Typedef for TPM_SIG_SCHEME
/// The definition of the signature scheme
typedef UINT16 TPM_SIG_SCHEME;

/// Typedef for TPM_PROTOCOL_ID
/// The protocol in use
typedef UINT16 TPM_PROTOCOL_ID;

/// Typedef for TPM_AUTH_DATA_USAGE
/// When authorization is required for an entity
typedef BYTE TPM_AUTH_DATA_USAGE;

/// Typedef for TPM_KEY_FLAGS
typedef UINT32 TPM_KEY_FLAGS;

/// Typedef for TPM_KEY_USAGE
/// Indicates the permitted usage of the key. See 4.16.6
typedef UINT16 TPM_KEY_USAGE;

/// Typedef for TPM_ENTITY_TYPE
typedef UINT16 TPM_ENTITY_TYPE;

/// Typedef for TPM_DA_STATE
typedef BYTE TPM_DA_STATE;

// Typedef for TPM_KEY_HANDLE
typedef UINT32 TPM_KEY_HANDLE;
/**
 *	@brief		TPM_VERSION structure
 *	@details	TPM_VERSION structure definition
 *
 **/
typedef struct tdTPM_VERSION
{
	TPM_VERSION_BYTE major;
	TPM_VERSION_BYTE minor;
	BYTE revMajor;
	BYTE revMinor;
} TPM_VERSION;

typedef TPM_VERSION TCPA_VERSION;

/**
 *	@brief		TPM_CAP_VERSION_INFO structure
 *	@details	TPM_CAP_VERSION_INFO structure definition
 *
 **/
typedef struct tdTPM_CAP_VERSION_INFO
{
	TPM_STRUCTURE_TAG tag;
	TPM_VERSION version;
	UINT16 specLevel;
	BYTE specLetter;
	BYTE tpmVendorID[4];
	UINT16 vendorSpecificSize;
	BYTE vendorSpecific[ST_EL_SML_SZ];	// Size_is(vendorSpecificSize)
} TPM_CAP_VERSION_INFO;

/**
 *	@brief		TPM_DA_ACTION_TYPE structure
 *	@details	TPM_DA_ACTION_TYPE structure definition
 *
 **/
typedef struct tdTPM_DA_ACTION_TYPE
{
	TPM_STRUCTURE_TAG tag;
	UINT32 actions;
} TPM_DA_ACTION_TYPE;

/**
 *	@brief		TPM_DA_INFO structure
 *	@details	TPM_DA_INFO structure definition
 *
 **/
typedef struct tdTPM_DA_INFO
{
	TPM_STRUCTURE_TAG tag;
	TPM_DA_STATE state;
	UINT16 currentCount;
	UINT16 thresholdCount;
	TPM_DA_ACTION_TYPE actionAtThreshold;
	UINT32 actionDependValue;
	UINT32 vendorDataSize;
	BYTE vendorData[ST_EL_BIG_SZ];
} TPM_DA_INFO;

/// Size of a TPM_NONCE
#define TPM_NONCE_SIZE	20

/**
 *	@brief		TPM_NONCE structure
 *	@details	TPM_NONCE structure definition
 *
 **/
typedef struct tdTPM_NONCE
{
	BYTE nonce[TPM_NONCE_SIZE];
} TPM_NONCE;

/**
 *	@brief		TPM_PERMANENT_FLAGS structure
 *	@details	TPM_PERMANENT_FLAGS structure definition (according to spec revision >=103)
 *
 **/
typedef struct tdTPM_PERMANENT_FLAGS
{
	TPM_STRUCTURE_TAG tag;
	BYTE disable;						// Actually: BOOL value using 1 Byte of memory
	BYTE ownership;						// Actually: BOOL value using 1 Byte of memory
	BYTE deactivated;					// Actually: BOOL value using 1 Byte of memory
	BYTE readPubek;						// Actually: BOOL value using 1 Byte of memory
	BYTE disableOwnerClear;				// Actually: BOOL value using 1 Byte of memory
	BYTE allowMaintenance;				// Actually: BOOL value using 1 Byte of memory
	BYTE physicalPresenceLifetimeLock;	// Actually: BOOL value using 1 Byte of memory
	BYTE physicalPresenceHWEnable;		// Actually: BOOL value using 1 Byte of memory
	BYTE physicalPresenceCMDEnable;		// Actually: BOOL value using 1 Byte of memory
	BYTE CEKPUsed;						// Actually: BOOL value using 1 Byte of memory
	BYTE TPMpost;						// Actually: BOOL value using 1 Byte of memory
	BYTE TPMpostLock;					// Actually: BOOL value using 1 Byte of memory
	BYTE FIPS;							// Actually: BOOL value using 1 Byte of memory
	BYTE bOperator;						// Actually: BOOL value using 1 Byte of memory
	BYTE enableRevokeEK;				// Actually: BOOL value using 1 Byte of memory
	BYTE nvLocked;						// Actually: BOOL value using 1 Byte of memory
	BYTE readSRKPub;					// Actually: BOOL value using 1 Byte of memory
	BYTE tpmEstablished;				// Actually: BOOL value using 1 Byte of memory
	BYTE maintenanceDone;				// Actually: BOOL value using 1 Byte of memory
	BYTE disableFullDALogicInfo;		// Actually: BOOL value using 1 Byte of memory
} TPM_PERMANENT_FLAGS;

/**
 *	@brief		TPM_STCLEAR_FLAGS structure
 *	@details	TPM_STCLEAR_FLAGS structure definition (according to spec revision 116)
 **/
typedef struct tdTPM_STCLEAR_FLAGS
{
	TPM_STRUCTURE_TAG tag;
	BYTE deactivated;					// Actually: BOOL value using 1 Byte of memory
	BYTE disableForceClear;				// Actually: BOOL value using 1 Byte of memory
	BYTE physicalPresence;				// Actually: BOOL value using 1 Byte of memory
	BYTE physicalPresenceLock;			// Actually: BOOL value using 1 Byte of memory
	BYTE bGlobalLock;					// Actually: BOOL value using 1 Byte of memory
} TPM_STCLEAR_FLAGS;

/// Size of a TPM_AUTHDATA
#define TPM_AUTHDATA_SIZE	20

/**
 *	@brief		TPM_AUTHDATA structure
 *	@details	TPM_AUTHDATA structure definition
 *
 **/
typedef struct tdTPM_AUTHDATA
{
	BYTE authdata[TPM_AUTHDATA_SIZE];
} TPM_AUTHDATA;

/// Typedef for TPM_ENCAUTH
typedef TPM_AUTHDATA TPM_ENCAUTH;

/**
 *	@brief		TPM_AUTH_IN structure
 *	@details	TPM_AUTH_IN structure definition
 *
 **/
typedef struct tdTPM_AUTH_IN
{
	TPM_AUTHHANDLE dwAuthHandle;
	TPM_NONCE sNonceOdd;
	BYTE bContinueAuthSession; // Actually: BOOL value using 1 Byte of memory
	TPM_AUTHDATA sOwnerAuth;
} TPM_AUTH_IN;

/**
 *	@brief		TPM_RSA_KEY_PARMS structure
 *	@details	TPM_RSA_KEY_PARMS structure definition
 *
 **/
typedef struct tdTPM_RSA_KEY_PARMS
{
	UINT32 keyLength;
	UINT32 numPrimes;
	UINT32 exponentSize;
} TPM_RSA_KEY_PARMS;

/**
 *	@brief		TPM_KEY_PARMS structure
 *	@details	TPM_KEY_PARMS structure definition
 *
 **/
typedef struct tdTPM_KEY_PARMS
{
	TPM_ALGORITHM_ID algorithmID;
	TPM_ENC_SCHEME encScheme;
	TPM_SIG_SCHEME sigScheme;
	UINT32 parmSize;
	// Only for RSA-2048! actually: parms; size_is(parmSize)
	TPM_RSA_KEY_PARMS parms;
} TPM_KEY_PARMS;

/// Public Endorsement Key length in Bytes
// IMPORTANT: KEY_LEN is NOT a max value, but the RSA-2048 key length!
#define KEY_LEN	256

/**
 *	@brief		TPM_STORE_PUBKEY structure
 *	@details	TPM_STORE_PUBKEY structure definition
 *
 **/
typedef struct tdTPM_STORE_PUBKEY
{
	UINT32 keyLength;
	BYTE key[KEY_LEN];	// Actually: size_is(keyLength)
} TPM_STORE_PUBKEY;

/**
 *	@brief		TPM_PUBKEY structure
 *	@details	TPM_PUBKEY structure definition
 *
 **/
typedef struct tdTPM_PUBKEY
{
	TPM_KEY_PARMS algorithmParms;
	TPM_STORE_PUBKEY pubKey;
} TPM_PUBKEY;

/**
 *	@brief		TPM_STRUCT_VER structure
 *	@details	TPM_STRUCT_VER structure definition
 *
 **/
typedef struct tdTPM_STRUCT_VER
{
	BYTE major;
	BYTE minor;
	BYTE revMajor;
	BYTE revMinor;
} TPM_STRUCT_VER;

/**
 *	@brief		TPM_KEY structure
 *	@details	TPM_KEY structure definition
 *
 **/
typedef struct tdTPM_KEY
{
	TPM_STRUCT_VER ver;
	TPM_KEY_USAGE keyUsage;
	TPM_KEY_FLAGS keyFlags;
	TPM_AUTH_DATA_USAGE authDataUsage;
	TPM_KEY_PARMS algorithmParms;
	UINT32 PCRInfoSize;
//	PCRInfo; empty for unbound keys // Actually: size_is(PCRInfoSize)
	TPM_STORE_PUBKEY pubKey;
	UINT32 encSize;
	BYTE encData[ST_EL_BIG_SZ];
} TPM_KEY;

/**
 *	@brief		IFX_FIELDUPGRADEINFO structure definition
 **/
typedef struct tdIFX_FIELDUPGRADEINFO
{
	uint8_t internal1;
	uint8_t internal2[4];
	TCPA_VERSION internal3;
	uint8_t internal4[3];
	uint16_t wMaxDataSize;
	uint16_t internal6;
	uint8_t internal7;
	uint8_t internal8;
	uint16_t wFlagsFieldUpgrade;
} IFX_FIELDUPGRADEINFO;