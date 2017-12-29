/**
 *	@brief		Declares the TPM2_Types method
 *	@file		TPM2_Types.h
 *	@details	This file was auto-generated based on TPM2.0 specification revision 116.
 *	@copyright
 *				Copyright Licenses:
 *
 *				* Trusted Computing Group (TCG) grants to the user of the source code
 *				in this specification (the "Source Code") a worldwide, irrevocable,
 *				nonexclusive, royalty free, copyright license to reproduce, create
 *				derivative works, distribute, display and perform the Source Code and
 *				derivative works thereof, and to grant others the rights granted
 *				herein.
 *
 *				* The TCG grants to the user of the other parts of the specification
 *				(other than the Source Code) the rights to reproduce, distribute,
 *				display, and perform the specification solely for the purpose of
 *				developing products based on such documents.
 *
 *				Source Code Distribution Conditions:
 *
 *				* Redistributions of Source Code must retain the above copyright
 *				licenses, this list of conditions and the following disclaimers.
 *
 *				* Redistributions in binary form must reproduce the above copyright
 *				licenses, this list of conditions and the following disclaimers in the
 *				documentation and/or other materials provided with the distribution.
 *
 *				Disclaimers:
 *
 *				* THE COPYRIGHT LICENSES SET FORTH ABOVE DO NOT REPRESENT ANY FORM OF
 *				LICENSE OR WAIVER, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, WITH
 *				RESPECT TO PATENT RIGHTS HELD BY TCG MEMBERS (OR OTHER THIRD PARTIES)
 *				THAT MAY BE NECESSARY TO IMPLEMENT THIS SPECIFICATION OR
 *				OTHERWISE. Contact TCG Administration
 *				(admin@trustedcomputinggroup.org) for information on specification
 *				licensing rights available through TCG membership agreements.
 *
 *				* THIS SPECIFICATION IS PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED
 *				WARRANTIES WHATSOEVER, INCLUDING ANY WARRANTY OF MERCHANTABILITY OR
 *				FITNESS FOR A PARTICULAR PURPOSE, ACCURACY, COMPLETENESS, OR
 *				NONINFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS, OR ANY WARRANTY
 *				OTHERWISE ARISING OUT OF ANY PROPOSAL, SPECIFICATION OR SAMPLE.
 *
 *				* Without limitation, TCG and its members and licensors disclaim all
 *				liability, including liability for infringement of any proprietary
 *				rights, relating to use of information in this specification and to
 *				the implementation of this specification, and TCG disclaims all
 *				liability for cost of procurement of substitute goods or services,
 *				lost profits, loss of use, loss of data or any incidental,
 *				consequential, direct, indirect, or special damages, whether under
 *				contract, tort, warranty or otherwise, arising in any way out of use
 *				or reliance upon this specification or any information herein.
 *
 *				Any marks and brands contained herein are the property of their
 *				respective owners.
 */
#pragma once

#include "../StdInclude.h"
#include "implementations.h"
/**
 * Table 3 - Definition of Base Types
 */
typedef uint8_t UINT8;
typedef uint8_t BYTE;
typedef int8_t INT8;
typedef int BOOL;
typedef uint16_t UINT16;
typedef int16_t INT16;
typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint64_t UINT64;
typedef int64_t INT64;
/**
 * Table 4 - Defines for Logic Values
 */
#ifndef TRUE
#define TRUE							1								/* - */
#endif
#ifndef FALSE
#define FALSE							0								/* - */
#endif
#define YES								1								/* - */
#define NO								0								/* - */
#define SET								1								/* - */
#define CLEAR							0								/* - */

/**
 * Table 4 - Defines for NIST_P192 ECC Values
 */
#define NIST_P192_curveID				TPM_ECC_NIST_P192				/* identifier for the curve */
#define NIST_P192_keySize				192								/* size in bits of the key */
#define NIST_P192_kdf					{TPM_ALG_KDF1_SP800_56A, TPM_ALG_SHA256}/* the default KDF and hash */
#define NIST_P192_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define NIST_P192_p						{24, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ,0xFF, 0xFF}}/* Fp (the modulus) */
#define NIST_P192_a						{24, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC}}/* coefficient of the linear term in the curve equation */
#define NIST_P192_b						{24, {0x64, 0x21, 0x05, 0x19, 0xE5, 0x9C, 0x80, 0xE7, 0x0F, 0xA7, 0xE9, 0xAB, 0x72, 0x24, 0x30, 0x49, 0xFE, 0xB8, 0xDE, 0xEC, 0xC1, 0x46, 0xB9, 0xB1}}/* constant term for curve equation */
#define NIST_P192_gX					{24, {0x18, 0x8D, 0xA8, 0x0E, 0xB0, 0x30, 0x90, 0xF6, 0x7C, 0xBF, 0x20, 0xEB, 0x43, 0xA1, 0x88, 0x00, 0xF4, 0xFF, 0x0A, 0xFD, 0x82, 0xFF, 0x10, 0x12}}/* x coordinate of base point G */
#define NIST_P192_gY					{24, {0x07, 0x19, 0x2B, 0x95, 0xFFC, 0x8D, 0xA7, 0x86, 0x31, 0x01, 0x1ED, 0x6B, 0x24, 0xCD, 0xD5, 0x73, 0xF9, 0x77, 0xA1, 0x1E, 0x79, 0x48, 0x11}}/* y coordinate of base point G */
#define NIST_P192_n						{24, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0xDE, 0xF8, 0x36, 0x14, 0x6B, 0xC9, 0xB1, 0xB4, 0xD2, 0x28, 0x31}}/* order of G */
#define NIST_P192_h						{1,{1}}							/* cofactor (a size of zero indicates a cofactor of 1) */

/**
 * Table 5 - Defines for NIST_P224 ECC Values
 */
#define NIST_P224_curveID				TPM_ECC_NIST_P224				/* identifier for the curve */
#define NIST_P224_keySize				224								/* Size in bits of the key */
#define NIST_P224_kdf					{TPM_ALG_KDF1_SP800_56A, TPM_ALG_SHA256}/* the default KDF and hash */
#define NIST_P224_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define NIST_P224_p						{28, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }}/* Fp (the modulus) */
#define NIST_P224_a						{28, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE }}/* coefficient of the linear term in the curve equation */
#define NIST_P224_b						{28, {0xB4, 0x05, 0x0A, 0x85, 0x0C, 0x04, 0xB3, 0xAB, 0xF5, 0x41, 0x32, 0x56, 0x50, 0x44, 0xB0, 0xB7, 0xD7, 0xBF, 0xD8, 0xBA, 0x27, 0x0B, 0x39, 0x43, 0x23, 0x55, 0xFF, 0xB4 }}/* constant term for curve equation */
#define NIST_P224_gX					{28, {0xB7, 0x0E, 0x0C, 0xBD, 0x6B, 0xB4, 0xBF, 0x7F, 0x32, 0x13, 0x90, 0xB9, 0x4A, 0x03, 0xC1, 0xD3, 0x56, 0xC2, 0x11, 0x22, 0x34, 0x32, 0x80, 0xD6, 0x11, 0x5C, 0x1D, 0x21 }}/* x coordinate of base point G */
#define NIST_P224_gY					{28, {0xBD, 0x37, 0x63, 0x88, 0xB5, 0xF7, 0x23, 0xFB, 0x4C, 0x22, 0xDF, 0xE6, 0xCD, 0x43, 0x75, 0xA0, 0x5A, 0x07, 0x47, 0x64, 0x44, 0xD5, 0x81, 0x99, 0x85, 0x00, 0x7E, 0x34 }}/* y coordinate of base point G */
#define NIST_P224_n						{28, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x16, 0xA2, 0xE0, 0xB8, 0xF0, 0x3E, 0x13, 0xDD, 0x29, 0x45, 0x5C, 0x5C, 0x2A, 0x3D }}/* order of G */
#define NIST_P224_h						{1,{1}}							/* cofactor */

/**
 * Table 6 - Defines for NIST_P256 ECC Values
 */
#define NIST_P256_curveID				TPM_ECC_NIST_P256				/* identifier for the curve */
#define NIST_P256_keySize				256								/* Size in bits of the key */
#define NIST_P256_kdf					{TPM_ALG_KDF1_SP800_56A, TPM_ALG_SHA256}/* the default KDF and hash */
#define NIST_P256_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define NIST_P256_p						{32, {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}/* Fp (the modulus) */
#define NIST_P256_a						{32, {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC }}/* coefficient of the linear term in the curve equation */
#define NIST_P256_b						{32, {0x5A, 0xC6, 0x35, 0xD8, 0xAA, 0x3A, 0x93, 0xE7, 0xB3, 0xEB, 0xBD, 0x55, 0x76, 0x98, 0x86, 0xBC, 0x65, 0x1D, 0x06, 0xB0, 0xCC, 0x53, 0xB0, 0xF6, 0x3B, 0xCE, 0x3C, 0x3E, 0x27, 0xD2, 0x60, 0x4B }}/* constant term for curve equation */
#define NIST_P256_gX					{32, {0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6, 0xE5, 0x63, 0xA4, 0x40, 0xF2, 0x77, 0x03, 0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0, 0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96 }}/* x coordinate of base point G */
#define NIST_P256_gY					{32, {0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB, 0x4A, 0x7C, 0x0F, 0x9E, 0x16, 0x2B, 0xCE, 0x33, 0x57, 0x6B, 0x31, 0x5E, 0xCE, 0xCB, 0xB6, 0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5 }}/* y coordinate of base point G */
#define NIST_P256_n						{32, {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBC, 0xE6, 0xFA, 0xAD, 0xA7, 0x17, 0x9E, 0x84, 0xF3, 0xB9, 0xCA, 0xC2, 0xFC, 0x63, 0x25, 0x51 }}/* order of G */
#define NIST_P256_h						{1,{1}}							/* cofactor */

/**
 * Table 7 - Defines for NIST_P384 ECC Values
 */
#define NIST_P384_curveID				TPM_ECC_NIST_P384				/* identifier for the curve */
#define NIST_P384_keySize				384								/* size in bits of the key */
#define NIST_P384_kdf					{TPM_ALG_KDF1_SP800_56A, TPM_ALG_SHA384}/* the default KDF and hash */
#define NIST_P384_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define NIST_P384_p						{48, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF }}/* Fp (the modulus) */
#define NIST_P384_a						{48, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFC }}/* coefficient of the linear term in the curve equation */
#define NIST_P384_b						{48, {0xB3, 0x31, 0x2F, 0xA7, 0xE2, 0x3E, 0xE7, 0xE4, 0x98, 0x8E, 0x05, 0x6B, 0xE3, 0xF8, 0x2D, 0x19, 0x18, 0x1D, 0x9C, 0x6E, 0xFE, 0x81, 0x41, 0x12, 0x03, 0x14, 0x08, 0x8F, 0x50, 0x13, 0x87, 0x5A, 0xC6, 0x56, 0x39, 0x8D, 0x8A, 0x2E, 0xD1, 0x9D, 0x2A, 0x85, 0xC8, 0xED, 0xD3, 0xEC, 0x2A, 0xEF }}/* constant term for curve equation */
#define NIST_P384_gX					{48, {0xAA, 0x87, 0xCA, 0x22, 0xBE, 0x8B, 0x05, 0x37, 0x8E, 0xB1, 0xC7, 0x1E, 0xF3, 0x20, 0xAD, 0x74, 0x6E, 0x1D, 0x3B, 0x62, 0x8B, 0xA7, 0x9B, 0x98, 0x59, 0xF7, 0x41, 0xE0, 0x82, 0x54, 0x2A, 0x38, 0x55, 0x02, 0xF2, 0x5D, 0xBF, 0x55, 0x29, 0x6C, 0x3A, 0x54, 0x5E, 0x38, 0x72, 0x76, 0x0A, 0xB7 }}/* x coordinate of base point G */
#define NIST_P384_gY					{48, {0x36, 0x17, 0xDE, 0x4A, 0x96, 0x26, 0x2C, 0x6F, 0x5D, 0x9E, 0x98, 0xBF, 0x92, 0x92, 0xDC, 0x29, 0xF8, 0xF4, 0x1D, 0xBD, 0x28, 0x9A, 0x14, 0x7C, 0xE9, 0xDA, 0x31, 0x13, 0xB5, 0xF0, 0xB8, 0xC0, 0x0A, 0x60, 0xB1, 0xCE, 0x1D, 0x7E, 0x81, 0x9D, 0x7A, 0x43, 0x1D, 0x7C, 0x90, 0xEA, 0x0E, 0x5F }}/* y coordinate of base point G */
#define NIST_P384_n						{48, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0x63, 0x4D, 0x81, 0xF4, 0x37, 0x2D, 0xDF, 0x58, 0x1A, 0x0D, 0xB2, 0x48, 0xB0, 0xA7, 0x7A, 0xEC, 0xEC, 0x19, 0x6A, 0xCC, 0xC5, 0x29, 0x73 }}/* order of G */
#define NIST_P384_h						{1,{1}}							/* cofactor */

/**
 * Table 7 - Defines for Implementation Values
 */
#define FIELD_UPGRADE_IMPLEMENTED		NO								/* temporary define */
#define BSIZE							UINT16							/* size used for internal storage of the size field of a TPM2B
																		This is the definition used for the reference design. Compilation with this value changed may cause warnings about conversions. */
#define BUFFER_ALIGNMENT				4								/* sets the size granularity for the buffers in a TPM2B structure
																		TPMxB buffers will be assigned a space that is a multiple of this value. This does not set the size limits for IO. Those are set by the canonical form of the TPMxB */
#define IMPLEMENTATION_PCR				24								/* the number of PCR in the TPM */
#define PLATFORM_PCR					24								/* the number of PCR required by the relevant platform specification */
#define DRTM_PCR						17								/* the D-RTM PCR
																		NOTE This value is not defined when the TPM does not implement D-RTM */
#define HCRTM_PCR						0								/* the PCR that will receive the H-CRTM value at TPM2_Startup. This value should not be changed. */
#define NUM_LOCALITIES					5								/* the number of localities supported by the TPM
																		This is expected to be either 5 for a PC, or 1 for just about everything else. */
#define MAX_HANDLE_NUM					3								/* the maximum number of handles in the handle area
																		This should be produced by the Part 3 parser but is here for now. */
#define MAX_ACTIVE_SESSIONS				64								/* the number of simultaneously active sessions that are supported by the TPM implementation */
#define CONTEXT_SLOT					UINT16							/* the type of an entry in the array of saved contexts */
#define CONTEXT_COUNTER					UINT64							/* the type of the saved session counter */
#define MAX_LOADED_SESSIONS				3								/* the number of sessions that the TPM may have in memory */
#define MAX_SESSION_NUM					3								/* this is the current maximum value */
#define MAX_LOADED_OBJECTS				3								/* the number of simultaneously loaded objects that are supported by the TPM; this number does not include the objects that may be placed in NV memory by TPM2_EvictControl(). */
#define MIN_EVICT_OBJECTS				2								/* the minimum number of evict objects supported by the TPM */
#define PCR_SELECT_MIN					((PLATFORM_PCR+7)/8)			/* - */
#define PCR_SELECT_MAX					((IMPLEMENTATION_PCR+7)/8)		/* - */
#define NUM_POLICY_PCR_GROUP			1								/* number of PCR groups that have individual policies */
#define NUM_AUTHVALUE_PCR_GROUP			1								/* number of PCR groups that have individual authorization values */
#define MAX_CONTEXT_SIZE				2048							/* This may be larger than necessary */
#define MAX_DIGEST_BUFFER				1024							/* - */
#define MAX_NV_INDEX_SIZE				2048							/* maximum data size allowed in an NV Index */
#define MAX_NV_BUFFER_SIZE				1024							/* maximum data size in one NV read or write command */
#define MAX_CAP_BUFFER					1024							/* maximum size of a capability buffer */
#define NV_MEMORY_SIZE					16384							/* size of NV memory in octets */
#define NUM_STATIC_PCR					16								/* - */
#define MAX_ALG_LIST_SIZE				64								/* number of algorithms that can be in a list */
#define TIMER_PRESCALE					100000							/* nominal value for the pre-scale value of Clock (the number of cycles of the TPM's oscillator for each increment of Clock) */
#define PRIMARY_SEED_SIZE				32								/* size of the Primary Seed in octets */
#define CONTEXT_ENCRYPT_ALG				TPM_ALG_AES						/* context encryption algorithm */
#define CONTEXT_ENCRYPT_KEY_BITS		MAX_SYM_KEY_BITS				/* context encryption key size in bits */
#define CONTEXT_ENCRYPT_KEY_BYTES		((CONTEXT_ENCRYPT_KEY_BITS+7)/8)/* - */
#define CONTEXT_INTEGRITY_HASH_ALG		TPM_ALG_SHA256					/* context integrity hash algorithm */
#define CONTEXT_INTEGRITY_HASH_SIZE		SHA256_DIGEST_SIZE				/* number of byes in the context integrity digest */
#define PROOF_SIZE						CONTEXT_INTEGRITY_HASH_SIZE		/* size of proof value in octets
																		This size of the proof should be consistent with the digest size used for context integrity. */
#define NV_CLOCK_UPDATE_INTERVAL		12								/* the update interval expressed as a power of 2 seconds
																		A value of 12 is 4,096 seconds (~68 minutes). */
#define NUM_POLICY_PCR					1								/* number of PCR that allow policy/auth */
#define MAX_COMMAND_SIZE				4096							/* maximum size of a command */
#define MAX_RESPONSE_SIZE				4096							/* maximum size of a response */
#define ORDERLY_BITS					8								/* number between 1 and 32 inclusive */
#define MAX_ORDERLY_COUNT				((1 << ORDERLY_BITS) - 1)		/* maximum count of orderly counter before NV is updated
																		This must be of the form 2^N - 1 where 1 <= N <= 32. */
#define ALG_ID_FIRST					TPM_ALG_FIRST					/* used by TPM2_GetCapability() processing to bound the algorithm search */
#define ALG_ID_LAST						TPM_ALG_LAST					/* used by TPM2_GetCapability() processing to bound the algorithm search */
#define MAX_SYM_DATA					128								/* the maximum number of octets that may be in a sealed blob; 128 is the minimum allowed value */
#define MAX_RNG_ENTROPY_SIZE			64								/* - */
#define RAM_INDEX_SPACE					512								/* - */
#define RSA_DEFAULT_PUBLIC_EXPONENT		0x00010001						/* 216 + 1 */
#define ENABLE_PCR_NO_INCREMENT			YES								/* indicates if the TPM_PT_PCR_NO_INCREMENT group is implemented */
#define CRT_FORMAT_RSA					YES								/* - */
#define PRIVATE_VENDOR_SPECIFIC_BYTES	((MAX_RSA_KEY_BYTES/2) * (3 + CRT_FORMAT_RSA * 2))/* MAX_RSA_KEY_BYTES is auto generated from the RSA key size selection in Table 4. If RSA is not implemented, this may need to be manually removed. */

/**
 * Table 8 - Defines for NIST_P521 ECC Values
 */
#define NIST_P521_curveID				TPM_ECC_NIST_P521				/* identifier for the curve */
#define NIST_P521_keySize				521								/* size in bits of the key */
#define NIST_P521_kdf					{TPM_ALG_KDF1_SP800_56A, TPM_ALG_SHA512}/* the default KDF and hash */
#define NIST_P521_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define NIST_P521_p						{66, {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}/* Fp (the modulus) */
#define NIST_P521_a						{66, {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC }}/* coefficient of the linear term in the curve equation */
#define NIST_P521_b						{66, {0x00, 0x51, 0x95, 0x3E, 0xB9, 0x61, 0x8E, 0x1C, 0x9A, 0x1F, 0x92, 0x9A, 0x21, 0xA0, 0xB6, 0x85, 0x40, 0xEE, 0xA2, 0xDA, 0x72, 0x5B, 0x99, 0xB3, 0x15, 0xF3, 0xB8, 0xB4, 0x89, 0x91, 0x8E, 0xF1, 0x09, 0xE1, 0x56, 0x19, 0x39, 0x51, 0xEC, 0x7E, 0x93, 0x7B, 0x16, 0x52, 0xC0, 0xBD, 0x3B, 0xB1, 0xBF, 0x07, 0x35, 0x73, 0xDF, 0x88, 0x3D, 0x2C, 0x34, 0xF1, 0xEF, 0x45, 0x1F, 0xD4, 0x6B, 0x50, 0x3F, 0x00 }}/* constant term for curve equation */
#define NIST_P521_gX					{66, {0x00, 0xC6, 0x85, 0x8E, 0x06, 0xB7, 0x04, 0x04, 0xE9, 0xCD, 0x9E, 0x3E, 0xCB, 0x66, 0x23, 0x95, 0xB4, 0x42, 0x9C, 0x64, 0x81, 0x39, 0x05, 0x3F, 0xB5, 0x21, 0xF8, 0x28, 0xAF, 0x60, 0x6B, 0x4D, 0x3D, 0xBA, 0xA1, 0x4B, 0x5E, 0x77, 0xEF, 0xE7, 0x59, 0x28, 0xFE, 0x1D, 0xC1, 0x27, 0xA2, 0xFF, 0xA8, 0xDE, 0x33, 0x48, 0xB3, 0xC1, 0x85, 0x6A, 0x42, 0x9B, 0xF9, 0x7E, 0x7E, 0x31, 0xC2, 0xE5, 0xBD, 0x66 }}/* x coordinate of base point G */
#define NIST_P521_gY					{66, {0x01, 0x18, 0x39, 0x29, 0x6A, 0x78, 0x9A, 0x3B, 0xC0, 0x04, 0x5C, 0x8A, 0x5F, 0xB4, 0x2C, 0x7D, 0x1B, 0xD9, 0x98, 0xF5, 0x44, 0x49, 0x57, 0x9B, 0x44, 0x68, 0x17, 0xAF, 0xBD, 0x17, 0x27, 0x3E, 0x66, 0x2C, 0x97, 0xEE, 0x72, 0x99, 0x5E, 0xF4, 0x26, 0x40, 0xC5, 0x50, 0xB9, 0x01, 0x3F, 0xAD, 0x07, 0x61, 0x35, 0x3C, 0x70, 0x86, 0xA2, 0x72, 0xC2, 0x40, 0x88, 0xBE, 0x94, 0x76, 0x9F, 0xD1, 0x66, 0x50 }}/* y coordinate of base point G */
#define NIST_P521_n						{66, {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA, 0x51, 0x86, 0x87, 0x83, 0xBF, 0x2F, 0x96, 0x6B, 0x7F, 0xCC, 0x01, 0x48, 0xF7, 0x09, 0xA5, 0xD0, 0x3B, 0xB5, 0xC9, 0xB8, 0x89, 0x9C, 0x47, 0xAE, 0xBB, 0x6F, 0xB7, 0x1E, 0x91, 0x38, 0x64, 0x09 }}/* order of G */
#define NIST_P521_h						{1,{1}}							/* cofactor */

/**
 * Table 9 - Defines for BN_P256 ECC Values
 */
#define BN_P256_curveID					TPM_ECC_BN_P256					/* identifier for the curve */
#define BN_P256_keySize					256								/* size in bits of the key */
#define BN_P256_kdf						{TPM_ALG_NULL, TPM_ALG_NULL}	/* the default KDF and hash */
#define BN_P256_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define BN_P256_p						{32, {0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC, 0XF0, 0XCD, 0X46, 0XE5, 0XF2, 0X5E, 0XEE, 0X71, 0XA4, 0X9F, 0X0C, 0XDC, 0X65, 0XFB, 0X12, 0X98, 0X0A, 0X82, 0XD3, 0X29, 0X2D, 0XDB, 0XAE, 0XD3, 0X30, 0X13 }}/* Fp (the modulus) */
#define BN_P256_a						{1,{0}}							/* coefficient of the linear term in the curve equation */
#define BN_P256_b						{1,{3}}							/* constant term for curve equation */
#define BN_P256_gX						{1,{1}}							/* x coordinate of base point G */
#define BN_P256_gY						{1,{2}};						/* y coordinate of base point G */
#define BN_P256_n						{32, {0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFC, 0XF0, 0XCD, 0X46, 0XE5, 0XF2, 0X5E, 0XEE, 0X71, 0XA4, 0X9E, 0X0C, 0XDC, 0X65, 0XFB, 0X12, 0X99, 0X92, 0X1A, 0XF6, 0X2D, 0X53, 0X6C, 0XD1, 0X0B, 0X50, 0X0D }}/* order of G */
#define BN_P256_h						{1,{1}}							/* cofactor */

/**
 * Table 10 - Defines for BN_P638 ECC Values
 */
#define BN_P638_curveID					TPM_ECC_BN_P638					/* identifier for the curve */
#define BN_P638_keySize					638								/* size in bits of the key */
#define BN_P638_kdf						{TPM_ALG_NULL, TPM_ALG_NULL}	/* the default KDF and hash */
#define BN_P638_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define BN_P638_p						{80, {0x23, 0xFF, 0xFF, 0xFD, 0xC0, 0x00, 0x00, 0x0D, 0x7F, 0xFF, 0xFF, 0xB8, 0x00, 0x00, 0x01, 0xD3, 0xFF, 0xFF, 0xF9, 0x42, 0xD0, 0x00, 0x16, 0x5E, 0x3F, 0xFF, 0x94, 0x87, 0x00, 0x00, 0xD5, 0x2F, 0xFF, 0xFD, 0xD0, 0xE0, 0x00, 0x08, 0xDE, 0x55, 0xC0, 0x00, 0x86, 0x52, 0x00, 0x21, 0xE5, 0x5B, 0xFF, 0xFF, 0xF5, 0x1F, 0xFF, 0xF4, 0xEB, 0x80, 0x00, 0x00, 0x00, 0x4C, 0x80, 0x01, 0x5A, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67 }}/* Fp (the modulus) */
#define BN_P638_a						{1,{0}}							/* coefficient of the linear term in the curve equation */
#define BN_P638_b						{2,{0x01, 0x01}}				/* constant term for curve equation */
#define BN_P638_gX						{80, {0x23, 0xFF, 0xFF, 0xFD, 0xC0, 0x00, 0x00, 0x0D, 0x7F, 0xFF, 0xFF, 0xB8, 0x00, 0x00, 0x01, 0xD3, 0xFF, 0xFF, 0xF9, 0x42, 0xD0, 0x00, 0x16, 0x5E, 0x3F, 0xFF, 0x94, 0x87, 0x00, 0x00, 0xD5, 0x2F, 0xFF, 0xFD, 0xD0, 0xE0, 0x00, 0x08, 0xDE, 0x55, 0xC0, 0x00, 0x86, 0x52, 0x00, 0x21, 0xE5, 0x5B, 0xFF, 0xFF, 0xF5, 0x1F, 0xFF, 0xF4, 0xEB, 0x80, 0x00, 0x00, 0x00, 0x4C, 0x80, 0x01, 0x5A, 0xCD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66 }}/* x coordinate of base point G */
#define BN_P638_gY						{1,{0x10}}						/* y coordinate of base point G */
#define BN_P638_n						{80, {0x23, 0xFF, 0xFF, 0xFD, 0xC0, 0x00, 0x00, 0x0D, 0x7F, 0xFF, 0xFF, 0xB8, 0x00, 0x00, 0x01, 0xD3, 0xFF, 0xFF, 0xF9, 0x42, 0xD0, 0x00, 0x16, 0x5E, 0x3F, 0xFF, 0x94, 0x87, 0x00, 0x00, 0xD5, 0x2F, 0xFF, 0xFD, 0xD0, 0xE0, 0x00, 0x08, 0xDE, 0x55, 0x60, 0x00, 0x86, 0x55, 0x00, 0x21, 0xE5, 0x55, 0xFF, 0xFF, 0xF5, 0x4F, 0xFF, 0xF4, 0xEA, 0xC0, 0x00, 0x00, 0x00, 0x49, 0x80, 0x01, 0x54, 0xD9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xED, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61 }}/* order of G */
#define BN_P638_h						{1,{1}}							/* cofactor */

/**
 * Table 11 - Defines for SM2_P256 ECC Values
 */
#define SM2_P256_curveID				TPM_ECC_SM2_P256				/* identifier for the curve */
#define SM2_P256_keySize				256								/* size in bits of the key */
#define SM2_P256_kdf					{TPM_ALG_KDF1_SP800_56A, TPM_ALG_SM3_256}/* the default KDF and hash */
#define SM2_P256_sign					{TPM_ALG_NULL, TPM_ALG_NULL}	/* mandatory signing scheme */
#define SM2_P256_p						{32, {0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}/* Fp (the modulus) */
#define SM2_P256_a						{32, {0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC }}/* coefficient of the linear term in the curve equation */
#define SM2_P256_b						{32, {0x28, 0xE9, 0xFA, 0x9E, 0x9D, 0x9F, 0x5E, 0x34, 0x4D, 0x5A, 0x9E, 0x4B, 0xCF, 0x65, 0x09, 0xA7, 0xF3, 0x97, 0x89, 0xF5, 0x15, 0xAB, 0x8F, 0x92, 0xDD, 0xBC, 0xBD, 0x41, 0x4D, 0x94, 0x0E, 0x93 }}/* constant term for curve equation */
#define SM2_P256_gX						{32, {0x32, 0xC4, 0xAE, 0x2C, 0x1F, 0x19, 0x81, 0x19, 0x5F, 0x99, 0x04, 0x46, 0x6A, 0x39, 0xC9, 0x94, 0x8F, 0xE3, 0x0B, 0xBF, 0xF2, 0x66, 0x0B, 0xE1, 0x71, 0x5A, 0x45, 0x89, 0x33, 0x4C, 0x74, 0xC7 }}/* x coordinate of base point G */
#define SM2_P256_gY						{32, {0xBC, 0x37, 0x36, 0xA2, 0xF4, 0xF6, 0x77, 0x9C, 0x59, 0xBD, 0xCE, 0xE3, 0x6B, 0x69, 0x21, 0x53, 0xD0, 0xA9, 0x87, 0x7C, 0xC6, 0x2A, 0x47, 0x40, 0x02, 0xDF, 0x32, 0xE5, 0x21, 0x39, 0xF0, 0xA0 }}/* y coordinate of base point G */
#define SM2_P256_n						{32, {0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x72, 0x03, 0xDF, 0x6B, 0x21, 0xC6, 0x05, 0x2B, 0x53, 0xBB, 0xF4, 0x09, 0x39, 0xD5, 0x41, 0x23 }}/* order of G */
#define SM2_P256_h						{1,{1}}							/* cofactor */

/**
 * Table 12 - Defines for SHA1 Hash Values
 */
#define SHA1_DIGEST_SIZE				20								/* size of digest in octets */
#define SHA1_BLOCK_SIZE					64								/* size of hash block in octets */
#define SHA1_DER_SIZE					15								/* size of the DER in octets */
#define SHA1_DER						0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2B, 0x0E, 0x03, 0x02, 0x1A, 0x05, 0x00, 0x04, 0x14/* the DER */

/**
 * Table 13 - Defines for SHA256 Hash Values
 */
#define SHA256_DIGEST_SIZE				32								/* size of digest */
#define SHA256_BLOCK_SIZE				64								/* size of hash block */
#define SHA256_DER_SIZE					19								/* size of the DER in octets */
#define SHA256_DER						0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20/* the DER */

/**
 * Table 14 - Defines for SHA384 Hash Values
 */
#define SHA384_DIGEST_SIZE				48								/* size of digest in octets */
#define SHA384_BLOCK_SIZE				128								/* size of hash block in octets */
#define SHA384_DER_SIZE					19								/* size of the DER in octets */
#define SHA384_DER						0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30/* the DER */

/**
 * Table 15 - Defines for SHA512 Hash Values
 */
#define SHA512_DIGEST_SIZE				64								/* size of digest in octets */
#define SHA512_BLOCK_SIZE				128								/* size of hash block in octets */
#define SHA512_DER_SIZE					19								/* size of the DER in octets */
#define SHA512_DER						0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40/* the DER */

/**
 * Table 16 - Defines for SM3_256 Hash Values
 */
#define SM3_256_DIGEST_SIZE				32								/* size of digest in octets */
#define SM3_256_BLOCK_SIZE				64								/* size of hash block in octets */
#define SM3_256_DER_SIZE				18								/* size of the DER in octets */
#define SM3_256_DER						0x30, 0x30, 0x30, 0x0c, 0x06, 0x08, 0x2A, 0x81, 0x1C, 0x81, 0x45, 0x01, 0x83, 0x11, 0x05, 0x00, 0x04, 0x20/* the DER */

/**
 * Table 17 - Defines for AES Symmetric Cipher Algorithm Constants
 */
#define AES_KEY_SIZES_BITS				{128, 192, 256}					/* - */
#define AES_BLOCK_SIZES_BITS			{128, 128, 128}					/* - */
#define AES_ROUNDS						{10, 12, 14}					/* - */

/**
 * Table 18 - Defines for SM4 Symmetric Cipher Algorithm Constants
 */
#define SM4_KEY_SIZES_BITS				{128}							/* - */
#define SM4_BLOCK_SIZES_BITS			{128}							/* - */
#define SM4_ROUNDS						{32}							/* - */

/**
 * Table 19 - Defines for CAMELLIA Symmetric Cipher Algorithm Constants
 */
#define CAMELLIA_KEY_SIZES_BITS			{128, 192, 256}					/* - */
#define CAMELLIA_BLOCK_SIZES_BITS		{128, 128, 128}					/* the block size is the same for all key sizes */
#define CAMELLIA_ROUNDS					{18, 24, 24}					/* - */

/**
 * Table 5 - Definition of Types for Documentation Clarity
 */
typedef UINT32 TPM_ALGORITHM_ID;
typedef UINT32 TPM_MODIFIER_INDICATOR;
typedef UINT32 TPM_AUTHORIZATION_SIZE;
typedef UINT32 TPM_PARAMETER_SIZE;
typedef UINT16 TPM_KEY_SIZE;
typedef UINT16 TPM_KEY_BITS;
#define MAX_SYM_KEY_BYTES 32
#define MAX_SYM_BLOCK_SIZE 16
#define MAX_RSA_KEY_BYTES 256
#define MAX_ECC_KEY_BYTES 32
typedef TPM_KEY_BITS TPMI_AES_KEY_BITS;
typedef TPM_KEY_BITS TPMI_SM4_KEY_BITS;
typedef TPM_KEY_BITS TPMI_CAMELLIA_KEY_BITS;
/**
 * Table 6 - Definition of (UINT32) TPM_SPEC Constants <>
 */
typedef UINT32							TPM_SPEC;
#define TPM_SPEC_FAMILY					(TPM_SPEC)(0x322E3000)
#define TPM_SPEC_LEVEL					(TPM_SPEC)(00)
#define TPM_SPEC_VERSION				(TPM_SPEC)(116)
#define TPM_SPEC_YEAR					(TPM_SPEC)(2014)
#define TPM_SPEC_DAY_OF_YEAR			(TPM_SPEC)(303)
/**
 * Table 7 - Definition of (UINT32) TPM_GENERATED Constants [O]
 */
typedef UINT32							TPM_GENERATED;
#define TPM_GENERATED_VALUE				(TPM_GENERATED)(0xff544347)
/**
 * Table 9 - Definition of (UINT16) TPM_ALG_ID Constants [IN/OUT, S]
 */
typedef UINT16							TPM_ALG_ID;
#define TPM_ALG_ERROR					(TPM_ALG_ID)(0x0000)
#define TPM_ALG_RSA						(TPM_ALG_ID)(0x0001)
#define TPM_ALG_SHA						(TPM_ALG_ID)(0x0004)
#define TPM_ALG_SHA1					(TPM_ALG_ID)(0x0004)
#define TPM_ALG_HMAC					(TPM_ALG_ID)(0x0005)
#define TPM_ALG_AES						(TPM_ALG_ID)(0x0006)
#define TPM_ALG_MGF1					(TPM_ALG_ID)(0x0007)
#define TPM_ALG_KEYEDHASH				(TPM_ALG_ID)(0x0008)
#define TPM_ALG_XOR						(TPM_ALG_ID)(0x000A)
#define TPM_ALG_SHA256					(TPM_ALG_ID)(0x000B)
#define TPM_ALG_SHA384					(TPM_ALG_ID)(0x000C)
#define TPM_ALG_SHA512					(TPM_ALG_ID)(0x000D)
#define TPM_ALG_NULL					(TPM_ALG_ID)(0x0010)
#define TPM_ALG_SM3_256					(TPM_ALG_ID)(0x0012)
#define TPM_ALG_SM4						(TPM_ALG_ID)(0x0013)
#define TPM_ALG_RSASSA					(TPM_ALG_ID)(0x0014)
#define TPM_ALG_RSAES					(TPM_ALG_ID)(0x0015)
#define TPM_ALG_RSAPSS					(TPM_ALG_ID)(0x0016)
#define TPM_ALG_OAEP					(TPM_ALG_ID)(0x0017)
#define TPM_ALG_ECDSA					(TPM_ALG_ID)(0x0018)
#define TPM_ALG_ECDH					(TPM_ALG_ID)(0x0019)
#define TPM_ALG_ECDAA					(TPM_ALG_ID)(0x001A)
#define TPM_ALG_SM2						(TPM_ALG_ID)(0x001B)
#define TPM_ALG_ECSCHNORR				(TPM_ALG_ID)(0x001C)
#define TPM_ALG_ECMQV					(TPM_ALG_ID)(0x001D)
#define TPM_ALG_KDF1_SP800_56A			(TPM_ALG_ID)(0x0020)
#define TPM_ALG_KDF2					(TPM_ALG_ID)(0x0021)
#define TPM_ALG_KDF1_SP800_108			(TPM_ALG_ID)(0x0022)
#define TPM_ALG_ECC						(TPM_ALG_ID)(0x0023)
#define TPM_ALG_SYMCIPHER				(TPM_ALG_ID)(0x0025)
#define TPM_ALG_CAMELLIA				(TPM_ALG_ID)(0x0026)
#define TPM_ALG_CTR						(TPM_ALG_ID)(0x0040)
#define TPM_ALG_OFB						(TPM_ALG_ID)(0x0041)
#define TPM_ALG_CBC						(TPM_ALG_ID)(0x0042)
#define TPM_ALG_CFB						(TPM_ALG_ID)(0x0043)
#define TPM_ALG_ECB						(TPM_ALG_ID)(0x0044)
/**
 * Table 10 - Definition of (UINT16) {ECC} TPM_ECC_CURVE Constants [IN/OUT, S]
 */
typedef UINT16							TPM_ECC_CURVE;
#define TPM_ECC_NONE					(TPM_ECC_CURVE)(0x0000)
#define TPM_ECC_NIST_P192				(TPM_ECC_CURVE)(0x0001)
#define TPM_ECC_NIST_P224				(TPM_ECC_CURVE)(0x0002)
#define TPM_ECC_NIST_P256				(TPM_ECC_CURVE)(0x0003)
#define TPM_ECC_NIST_P384				(TPM_ECC_CURVE)(0x0004)
#define TPM_ECC_NIST_P521				(TPM_ECC_CURVE)(0x0005)
#define TPM_ECC_BN_P256					(TPM_ECC_CURVE)(0x0010)
#define TPM_ECC_BN_P638					(TPM_ECC_CURVE)(0x0011)
#define TPM_ECC_SM2_P256				(TPM_ECC_CURVE)(0x0020)
/**
 * Table 13 - Definition of (UINT32) TPM_CC Constants (Numeric Order) [IN/OUT, S]
 */
typedef UINT32							TPM_CC;
#define TPM_CC_FIRST					(TPM_CC)(0x0000011F)
#define TPM_CC_PP_FIRST					(TPM_CC)(0x0000011F)
#define TPM_CC_NV_UndefineSpaceSpecial	(TPM_CC)(0x0000011F)
#define TPM_CC_EvictControl				(TPM_CC)(0x00000120)
#define TPM_CC_HierarchyControl			(TPM_CC)(0x00000121)
#define TPM_CC_NV_UndefineSpace			(TPM_CC)(0x00000122)
#define TPM_CC_ChangeEPS				(TPM_CC)(0x00000124)
#define TPM_CC_ChangePPS				(TPM_CC)(0x00000125)
#define TPM_CC_Clear					(TPM_CC)(0x00000126)
#define TPM_CC_ClearControl				(TPM_CC)(0x00000127)
#define TPM_CC_ClockSet					(TPM_CC)(0x00000128)
#define TPM_CC_HierarchyChangeAuth		(TPM_CC)(0x00000129)
#define TPM_CC_NV_DefineSpace			(TPM_CC)(0x0000012A)
#define TPM_CC_PCR_Allocate				(TPM_CC)(0x0000012B)
#define TPM_CC_PCR_SetAuthPolicy		(TPM_CC)(0x0000012C)
#define TPM_CC_PP_Commands				(TPM_CC)(0x0000012D)
#define TPM_CC_SetPrimaryPolicy			(TPM_CC)(0x0000012E)
#define TPM_CC_FieldUpgradeStart		(TPM_CC)(0x0000012F)
#define TPM_CC_ClockRateAdjust			(TPM_CC)(0x00000130)
#define TPM_CC_CreatePrimary			(TPM_CC)(0x00000131)
#define TPM_CC_NV_GlobalWriteLock		(TPM_CC)(0x00000132)
#define TPM_CC_PP_LAST					(TPM_CC)(0x00000132)
#define TPM_CC_GetCommandAuditDigest	(TPM_CC)(0x00000133)
#define TPM_CC_NV_Increment				(TPM_CC)(0x00000134)
#define TPM_CC_NV_SetBits				(TPM_CC)(0x00000135)
#define TPM_CC_NV_Extend				(TPM_CC)(0x00000136)
#define TPM_CC_NV_Write					(TPM_CC)(0x00000137)
#define TPM_CC_NV_WriteLock				(TPM_CC)(0x00000138)
#define TPM_CC_DictionaryAttackLockReset(TPM_CC)(0x00000139)
#define TPM_CC_DictionaryAttackParameters(TPM_CC)(0x0000013A)
#define TPM_CC_NV_ChangeAuth			(TPM_CC)(0x0000013B)
#define TPM_CC_PCR_Event				(TPM_CC)(0x0000013C)
#define TPM_CC_PCR_Reset				(TPM_CC)(0x0000013D)
#define TPM_CC_SequenceComplete			(TPM_CC)(0x0000013E)
#define TPM_CC_SetAlgorithmSet			(TPM_CC)(0x0000013F)
#define TPM_CC_SetCommandCodeAuditStatus(TPM_CC)(0x00000140)
#define TPM_CC_FieldUpgradeData			(TPM_CC)(0x00000141)
#define TPM_CC_IncrementalSelfTest		(TPM_CC)(0x00000142)
#define TPM_CC_SelfTest					(TPM_CC)(0x00000143)
#define TPM_CC_Startup					(TPM_CC)(0x00000144)
#define TPM_CC_Shutdown					(TPM_CC)(0x00000145)
#define TPM_CC_StirRandom				(TPM_CC)(0x00000146)
#define TPM_CC_ActivateCredential		(TPM_CC)(0x00000147)
#define TPM_CC_Certify					(TPM_CC)(0x00000148)
#define TPM_CC_PolicyNV					(TPM_CC)(0x00000149)
#define TPM_CC_CertifyCreation			(TPM_CC)(0x0000014A)
#define TPM_CC_Duplicate				(TPM_CC)(0x0000014B)
#define TPM_CC_GetTime					(TPM_CC)(0x0000014C)
#define TPM_CC_GetSessionAuditDigest	(TPM_CC)(0x0000014D)
#define TPM_CC_NV_Read					(TPM_CC)(0x0000014E)
#define TPM_CC_NV_ReadLock				(TPM_CC)(0x0000014F)
#define TPM_CC_ObjectChangeAuth			(TPM_CC)(0x00000150)
#define TPM_CC_PolicySecret				(TPM_CC)(0x00000151)
#define TPM_CC_Rewrap					(TPM_CC)(0x00000152)
#define TPM_CC_Create					(TPM_CC)(0x00000153)
#define TPM_CC_ECDH_ZGen				(TPM_CC)(0x00000154)
#define TPM_CC_HMAC						(TPM_CC)(0x00000155)
#define TPM_CC_Import					(TPM_CC)(0x00000156)
#define TPM_CC_Load						(TPM_CC)(0x00000157)
#define TPM_CC_Quote					(TPM_CC)(0x00000158)
#define TPM_CC_RSA_Decrypt				(TPM_CC)(0x00000159)
#define TPM_CC_HMAC_Start				(TPM_CC)(0x0000015B)
#define TPM_CC_SequenceUpdate			(TPM_CC)(0x0000015C)
#define TPM_CC_Sign						(TPM_CC)(0x0000015D)
#define TPM_CC_Unseal					(TPM_CC)(0x0000015E)
#define TPM_CC_PolicySigned				(TPM_CC)(0x00000160)
#define TPM_CC_ContextLoad				(TPM_CC)(0x00000161)
#define TPM_CC_ContextSave				(TPM_CC)(0x00000162)
#define TPM_CC_ECDH_KeyGen				(TPM_CC)(0x00000163)
#define TPM_CC_EncryptDecrypt			(TPM_CC)(0x00000164)
#define TPM_CC_FlushContext				(TPM_CC)(0x00000165)
#define TPM_CC_LoadExternal				(TPM_CC)(0x00000167)
#define TPM_CC_MakeCredential			(TPM_CC)(0x00000168)
#define TPM_CC_NV_ReadPublic			(TPM_CC)(0x00000169)
#define TPM_CC_PolicyAuthorize			(TPM_CC)(0x0000016A)
#define TPM_CC_PolicyAuthValue			(TPM_CC)(0x0000016B)
#define TPM_CC_PolicyCommandCode		(TPM_CC)(0x0000016C)
#define TPM_CC_PolicyCounterTimer		(TPM_CC)(0x0000016D)
#define TPM_CC_PolicyCpHash				(TPM_CC)(0x0000016E)
#define TPM_CC_PolicyLocality			(TPM_CC)(0x0000016F)
#define TPM_CC_PolicyNameHash			(TPM_CC)(0x00000170)
#define TPM_CC_PolicyOR					(TPM_CC)(0x00000171)
#define TPM_CC_PolicyTicket				(TPM_CC)(0x00000172)
#define TPM_CC_ReadPublic				(TPM_CC)(0x00000173)
#define TPM_CC_RSA_Encrypt				(TPM_CC)(0x00000174)
#define TPM_CC_StartAuthSession			(TPM_CC)(0x00000176)
#define TPM_CC_VerifySignature			(TPM_CC)(0x00000177)
#define TPM_CC_ECC_Parameters			(TPM_CC)(0x00000178)
#define TPM_CC_FirmwareRead				(TPM_CC)(0x00000179)
#define TPM_CC_GetCapability			(TPM_CC)(0x0000017A)
#define TPM_CC_GetRandom				(TPM_CC)(0x0000017B)
#define TPM_CC_GetTestResult			(TPM_CC)(0x0000017C)
#define TPM_CC_Hash						(TPM_CC)(0x0000017D)
#define TPM_CC_PCR_Read					(TPM_CC)(0x0000017E)
#define TPM_CC_PolicyPCR				(TPM_CC)(0x0000017F)
#define TPM_CC_PolicyRestart			(TPM_CC)(0x00000180)
#define TPM_CC_ReadClock				(TPM_CC)(0x00000181)
#define TPM_CC_PCR_Extend				(TPM_CC)(0x00000182)
#define TPM_CC_PCR_SetAuthValue			(TPM_CC)(0x00000183)
#define TPM_CC_NV_Certify				(TPM_CC)(0x00000184)
#define TPM_CC_EventSequenceComplete	(TPM_CC)(0x00000185)
#define TPM_CC_HashSequenceStart		(TPM_CC)(0x00000186)
#define TPM_CC_PolicyPhysicalPresence	(TPM_CC)(0x00000187)
#define TPM_CC_PolicyDuplicationSelect	(TPM_CC)(0x00000188)
#define TPM_CC_PolicyGetDigest			(TPM_CC)(0x00000189)
#define TPM_CC_TestParms				(TPM_CC)(0x0000018A)
#define TPM_CC_Commit					(TPM_CC)(0x0000018B)
#define TPM_CC_PolicyPassword			(TPM_CC)(0x0000018C)
#define TPM_CC_ZGen_2Phase				(TPM_CC)(0x0000018D)
#define TPM_CC_EC_Ephemeral				(TPM_CC)(0x0000018E)
#define TPM_CC_PolicyNvWritten			(TPM_CC)(0x0000018F)
#define TPM_CC_LAST						(TPM_CC)(0x0000018F)
/**
 * Table 17 - Definition of (UINT32) TPM_RC Constants (Actions) [OUT]
 */
typedef UINT32							TPM_RC;
#define TPM_RC_SUCCESS					(TPM_RC)(0x000)
#define TPM_RC_BAD_TAG					(TPM_RC)(0x01E)
#define RC_VER1							(TPM_RC)(0x100)
#define TPM_RC_INITIALIZE				(TPM_RC)(RC_VER1 + 0x000)
#define TPM_RC_FAILURE					(TPM_RC)(RC_VER1 + 0x001)
#define TPM_RC_SEQUENCE					(TPM_RC)(RC_VER1 + 0x003)
#define TPM_RC_PRIVATE					(TPM_RC)(RC_VER1 + 0x00B)
#define TPM_RC_HMAC						(TPM_RC)(RC_VER1 + 0x019)
#define TPM_RC_DISABLED					(TPM_RC)(RC_VER1 + 0x020)
#define TPM_RC_EXCLUSIVE				(TPM_RC)(RC_VER1 + 0x021)
#define TPM_RC_AUTH_TYPE				(TPM_RC)(RC_VER1 + 0x024)
#define TPM_RC_AUTH_MISSING				(TPM_RC)(RC_VER1 + 0x025)
#define TPM_RC_POLICY					(TPM_RC)(RC_VER1 + 0x026)
#define TPM_RC_PCR						(TPM_RC)(RC_VER1 + 0x027)
#define TPM_RC_PCR_CHANGED				(TPM_RC)(RC_VER1 + 0x028)
#define TPM_RC_UPGRADE					(TPM_RC)(RC_VER1 + 0x02D)
#define TPM_RC_TOO_MANY_CONTEXTS		(TPM_RC)(RC_VER1 + 0x02E)
#define TPM_RC_AUTH_UNAVAILABLE			(TPM_RC)(RC_VER1 + 0x02F)
#define TPM_RC_REBOOT					(TPM_RC)(RC_VER1 + 0x030)
#define TPM_RC_UNBALANCED				(TPM_RC)(RC_VER1 + 0x031)
#define TPM_RC_COMMAND_SIZE				(TPM_RC)(RC_VER1 + 0x042)
#define TPM_RC_COMMAND_CODE				(TPM_RC)(RC_VER1 + 0x043)
#define TPM_RC_AUTHSIZE					(TPM_RC)(RC_VER1 + 0x044)
#define TPM_RC_AUTH_CONTEXT				(TPM_RC)(RC_VER1 + 0x045)
#define TPM_RC_NV_RANGE					(TPM_RC)(RC_VER1 + 0x046)
#define TPM_RC_NV_SIZE					(TPM_RC)(RC_VER1 + 0x047)
#define TPM_RC_NV_LOCKED				(TPM_RC)(RC_VER1 + 0x048)
#define TPM_RC_NV_AUTHORIZATION			(TPM_RC)(RC_VER1 + 0x049)
#define TPM_RC_NV_UNINITIALIZED			(TPM_RC)(RC_VER1 + 0x04A)
#define TPM_RC_NV_SPACE					(TPM_RC)(RC_VER1 + 0x04B)
#define TPM_RC_NV_DEFINED				(TPM_RC)(RC_VER1 + 0x04C)
#define TPM_RC_BAD_CONTEXT				(TPM_RC)(RC_VER1 + 0x050)
#define TPM_RC_CPHASH					(TPM_RC)(RC_VER1 + 0x051)
#define TPM_RC_PARENT					(TPM_RC)(RC_VER1 + 0x052)
#define TPM_RC_NEEDS_TEST				(TPM_RC)(RC_VER1 + 0x053)
#define TPM_RC_NO_RESULT				(TPM_RC)(RC_VER1 + 0x054)
#define TPM_RC_SENSITIVE				(TPM_RC)(RC_VER1 + 0x055)
#define RC_MAX_FM0						(TPM_RC)(RC_VER1 + 0x07F)
#define RC_FMT1							(TPM_RC)(0x080)
#define TPM_RC_ASYMMETRIC				(TPM_RC)(RC_FMT1 + 0x001)
#define TPM_RC_ATTRIBUTES				(TPM_RC)(RC_FMT1 + 0x002)
#define TPM_RC_HASH						(TPM_RC)(RC_FMT1 + 0x003)
#define TPM_RC_VALUE					(TPM_RC)(RC_FMT1 + 0x004)
#define TPM_RC_HIERARCHY				(TPM_RC)(RC_FMT1 + 0x005)
#define TPM_RC_KEY_SIZE					(TPM_RC)(RC_FMT1 + 0x007)
#define TPM_RC_MGF						(TPM_RC)(RC_FMT1 + 0x008)
#define TPM_RC_MODE						(TPM_RC)(RC_FMT1 + 0x009)
#define TPM_RC_TYPE						(TPM_RC)(RC_FMT1 + 0x00A)
#define TPM_RC_HANDLE					(TPM_RC)(RC_FMT1 + 0x00B)
#define TPM_RC_KDF						(TPM_RC)(RC_FMT1 + 0x00C)
#define TPM_RC_RANGE					(TPM_RC)(RC_FMT1 + 0x00D)
#define TPM_RC_AUTH_FAIL				(TPM_RC)(RC_FMT1 + 0x00E)
#define TPM_RC_NONCE					(TPM_RC)(RC_FMT1 + 0x00F)
#define TPM_RC_PP						(TPM_RC)(RC_FMT1 + 0x010)
#define TPM_RC_SCHEME					(TPM_RC)(RC_FMT1 + 0x012)
#define TPM_RC_SIZE						(TPM_RC)(RC_FMT1 + 0x015)
#define TPM_RC_SYMMETRIC				(TPM_RC)(RC_FMT1 + 0x016)
#define TPM_RC_TAG						(TPM_RC)(RC_FMT1 + 0x017)
#define TPM_RC_SELECTOR					(TPM_RC)(RC_FMT1 + 0x018)
#define TPM_RC_INSUFFICIENT				(TPM_RC)(RC_FMT1 + 0x01A)
#define TPM_RC_SIGNATURE				(TPM_RC)(RC_FMT1 + 0x01B)
#define TPM_RC_KEY						(TPM_RC)(RC_FMT1 + 0x01C)
#define TPM_RC_POLICY_FAIL				(TPM_RC)(RC_FMT1 + 0x01D)
#define TPM_RC_INTEGRITY				(TPM_RC)(RC_FMT1 + 0x01F)
#define TPM_RC_TICKET					(TPM_RC)(RC_FMT1 + 0x020)
#define TPM_RC_RESERVED_BITS			(TPM_RC)(RC_FMT1 + 0x021)
#define TPM_RC_BAD_AUTH					(TPM_RC)(RC_FMT1 + 0x022)
#define TPM_RC_EXPIRED					(TPM_RC)(RC_FMT1 + 0x023)
#define TPM_RC_POLICY_CC				(TPM_RC)(RC_FMT1 + 0x024)
#define TPM_RC_BINDING					(TPM_RC)(RC_FMT1 + 0x025)
#define TPM_RC_CURVE					(TPM_RC)(RC_FMT1 + 0x026)
#define TPM_RC_ECC_POINT				(TPM_RC)(RC_FMT1 + 0x027)
#define RC_WARN							(TPM_RC)(0x900)
#define TPM_RC_CONTEXT_GAP				(TPM_RC)(RC_WARN + 0x001)
#define TPM_RC_OBJECT_MEMORY			(TPM_RC)(RC_WARN + 0x002)
#define TPM_RC_SESSION_MEMORY			(TPM_RC)(RC_WARN + 0x003)
#define TPM_RC_MEMORY					(TPM_RC)(RC_WARN + 0x004)
#define TPM_RC_SESSION_HANDLES			(TPM_RC)(RC_WARN + 0x005)
#define TPM_RC_OBJECT_HANDLES			(TPM_RC)(RC_WARN + 0x006)
#define TPM_RC_LOCALITY					(TPM_RC)(RC_WARN + 0x007)
#define TPM_RC_YIELDED					(TPM_RC)(RC_WARN + 0x008)
#define TPM_RC_CANCELED					(TPM_RC)(RC_WARN + 0x009)
#define TPM_RC_TESTING					(TPM_RC)(RC_WARN + 0x00A)
#define TPM_RC_REFERENCE_H0				(TPM_RC)(RC_WARN + 0x010)
#define TPM_RC_REFERENCE_H1				(TPM_RC)(RC_WARN + 0x011)
#define TPM_RC_REFERENCE_H2				(TPM_RC)(RC_WARN + 0x012)
#define TPM_RC_REFERENCE_H3				(TPM_RC)(RC_WARN + 0x013)
#define TPM_RC_REFERENCE_H4				(TPM_RC)(RC_WARN + 0x014)
#define TPM_RC_REFERENCE_H5				(TPM_RC)(RC_WARN + 0x015)
#define TPM_RC_REFERENCE_H6				(TPM_RC)(RC_WARN + 0x016)
#define TPM_RC_REFERENCE_S0				(TPM_RC)(RC_WARN + 0x018)
#define TPM_RC_REFERENCE_S1				(TPM_RC)(RC_WARN + 0x019)
#define TPM_RC_REFERENCE_S2				(TPM_RC)(RC_WARN + 0x01A)
#define TPM_RC_REFERENCE_S3				(TPM_RC)(RC_WARN + 0x01B)
#define TPM_RC_REFERENCE_S4				(TPM_RC)(RC_WARN + 0x01C)
#define TPM_RC_REFERENCE_S5				(TPM_RC)(RC_WARN + 0x01D)
#define TPM_RC_REFERENCE_S6				(TPM_RC)(RC_WARN + 0x01E)
#define TPM_RC_NV_RATE					(TPM_RC)(RC_WARN + 0x020)
#define TPM_RC_LOCKOUT					(TPM_RC)(RC_WARN + 0x021)
#define TPM_RC_RETRY					(TPM_RC)(RC_WARN + 0x022)
#define TPM_RC_NV_UNAVAILABLE			(TPM_RC)(RC_WARN + 0x023)
#define TPM_RC_NOT_USED					(TPM_RC)(RC_WARN + 0x7F)
#define TPM_RC_H						(TPM_RC)(0x000)
#define TPM_RC_P						(TPM_RC)(0x040)
#define TPM_RC_S						(TPM_RC)(0x800)
#define TPM_RC_1						(TPM_RC)(0x100)
#define TPM_RC_2						(TPM_RC)(0x200)
#define TPM_RC_3						(TPM_RC)(0x300)
#define TPM_RC_4						(TPM_RC)(0x400)
#define TPM_RC_5						(TPM_RC)(0x500)
#define TPM_RC_6						(TPM_RC)(0x600)
#define TPM_RC_7						(TPM_RC)(0x700)
#define TPM_RC_8						(TPM_RC)(0x800)
#define TPM_RC_9						(TPM_RC)(0x900)
#define TPM_RC_A						(TPM_RC)(0xA00)
#define TPM_RC_B						(TPM_RC)(0xB00)
#define TPM_RC_C						(TPM_RC)(0xC00)
#define TPM_RC_D						(TPM_RC)(0xD00)
#define TPM_RC_E						(TPM_RC)(0xE00)
#define TPM_RC_F						(TPM_RC)(0xF00)
#define TPM_RC_N_MASK					(TPM_RC)(0xF00)
/**
 * Table 18 - Definition of (INT8) TPM_CLOCK_ADJUST Constants [IN]
 */
typedef INT8							TPM_CLOCK_ADJUST;
#define TPM_CLOCK_COARSE_SLOWER			(TPM_CLOCK_ADJUST)(-3)
#define TPM_CLOCK_MEDIUM_SLOWER			(TPM_CLOCK_ADJUST)(-2)
#define TPM_CLOCK_FINE_SLOWER			(TPM_CLOCK_ADJUST)(-1)
#define TPM_CLOCK_NO_CHANGE				(TPM_CLOCK_ADJUST)(0)
#define TPM_CLOCK_FINE_FASTER			(TPM_CLOCK_ADJUST)(1)
#define TPM_CLOCK_MEDIUM_FASTER			(TPM_CLOCK_ADJUST)(2)
#define TPM_CLOCK_COARSE_FASTER			(TPM_CLOCK_ADJUST)(3)
/**
 * Table 19 - Definition of (UINT16) TPM_EO Constants [IN/OUT]
 */
typedef UINT16							TPM_EO;
#define TPM_EO_EQ						(TPM_EO)(0x0000)
#define TPM_EO_NEQ						(TPM_EO)(0x0001)
#define TPM_EO_SIGNED_GT				(TPM_EO)(0x0002)
#define TPM_EO_UNSIGNED_GT				(TPM_EO)(0x0003)
#define TPM_EO_SIGNED_LT				(TPM_EO)(0x0004)
#define TPM_EO_UNSIGNED_LT				(TPM_EO)(0x0005)
#define TPM_EO_SIGNED_GE				(TPM_EO)(0x0006)
#define TPM_EO_UNSIGNED_GE				(TPM_EO)(0x0007)
#define TPM_EO_SIGNED_LE				(TPM_EO)(0x0008)
#define TPM_EO_UNSIGNED_LE				(TPM_EO)(0x0009)
#define TPM_EO_BITSET					(TPM_EO)(0x000A)
#define TPM_EO_BITCLEAR					(TPM_EO)(0x000B)
/**
 * Table 20 - Definition of (UINT16) TPM_ST Constants [IN/OUT, S]
 */
typedef UINT16							TPM_ST;
#define TPM_ST_RSP_COMMAND				(TPM_ST)(0x00C4)
#define TPM_ST_NULL						(TPM_ST)(0X8000)
#define TPM_ST_NO_SESSIONS				(TPM_ST)(0x8001)
#define TPM_ST_SESSIONS					(TPM_ST)(0x8002)
#define TPM_ST_ATTEST_NV				(TPM_ST)(0x8014)
#define TPM_ST_ATTEST_COMMAND_AUDIT		(TPM_ST)(0x8015)
#define TPM_ST_ATTEST_SESSION_AUDIT		(TPM_ST)(0x8016)
#define TPM_ST_ATTEST_CERTIFY			(TPM_ST)(0x8017)
#define TPM_ST_ATTEST_QUOTE				(TPM_ST)(0x8018)
#define TPM_ST_ATTEST_TIME				(TPM_ST)(0x8019)
#define TPM_ST_ATTEST_CREATION			(TPM_ST)(0x801A)
#define TPM_ST_CREATION					(TPM_ST)(0x8021)
#define TPM_ST_VERIFIED					(TPM_ST)(0x8022)
#define TPM_ST_AUTH_SECRET				(TPM_ST)(0x8023)
#define TPM_ST_HASHCHECK				(TPM_ST)(0x8024)
#define TPM_ST_AUTH_SIGNED				(TPM_ST)(0x8025)
#define TPM_ST_FU_MANIFEST				(TPM_ST)(0x8029)
/**
 * Table 21 - Definition of (UINT16) TPM_SU Constants [IN]
 */
typedef UINT16							TPM_SU;
#define TPM_SU_CLEAR					(TPM_SU)(0x0000)
#define TPM_SU_STATE					(TPM_SU)(0x0001)
/**
 * Table 22 - Definition of (UINT8) TPM_SE Constants [IN]
 */
typedef UINT8							TPM_SE;
#define TPM_SE_HMAC						(TPM_SE)(0x00)
#define TPM_SE_POLICY					(TPM_SE)(0x01)
#define TPM_SE_TRIAL					(TPM_SE)(0x03)
/**
 * Table 23 - Definition of (UINT32) TPM_CAP Constants
 */
typedef UINT32							TPM_CAP;
#define TPM_CAP_FIRST					(TPM_CAP)(0x00000000)
#define TPM_CAP_ALGS					(TPM_CAP)(0x00000000)
#define TPM_CAP_HANDLES					(TPM_CAP)(0x00000001)
#define TPM_CAP_COMMANDS				(TPM_CAP)(0x00000002)
#define TPM_CAP_PP_COMMANDS				(TPM_CAP)(0x00000003)
#define TPM_CAP_AUDIT_COMMANDS			(TPM_CAP)(0x00000004)
#define TPM_CAP_PCRS					(TPM_CAP)(0x00000005)
#define TPM_CAP_TPM_PROPERTIES			(TPM_CAP)(0x00000006)
#define TPM_CAP_PCR_PROPERTIES			(TPM_CAP)(0x00000007)
#define TPM_CAP_ECC_CURVES				(TPM_CAP)(0x00000008)
#define TPM_CAP_LAST					(TPM_CAP)(0x00000008)
#define TPM_CAP_VENDOR_PROPERTY			(TPM_CAP)(0x00000100)
/**
 * Table 24 - Definition of (UINT32) TPM_PT Constants [IN/OUT, S]
 */
typedef UINT32							TPM_PT;
#define TPM_PT_NONE						(TPM_PT)(0x00000000)
#define PT_GROUP						(TPM_PT)(0x00000100)
#define PT_FIXED						(TPM_PT)(PT_GROUP * 1)
#define TPM_PT_FAMILY_INDICATOR			(TPM_PT)(PT_FIXED + 0)
#define TPM_PT_LEVEL					(TPM_PT)(PT_FIXED + 1)
#define TPM_PT_REVISION					(TPM_PT)(PT_FIXED + 2)
#define TPM_PT_DAY_OF_YEAR				(TPM_PT)(PT_FIXED + 3)
#define TPM_PT_YEAR						(TPM_PT)(PT_FIXED + 4)
#define TPM_PT_MANUFACTURER				(TPM_PT)(PT_FIXED + 5)
#define TPM_PT_VENDOR_STRING_1			(TPM_PT)(PT_FIXED + 6)
#define TPM_PT_VENDOR_STRING_2			(TPM_PT)(PT_FIXED + 7)
#define TPM_PT_VENDOR_STRING_3			(TPM_PT)(PT_FIXED + 8)
#define TPM_PT_VENDOR_STRING_4			(TPM_PT)(PT_FIXED + 9)
#define TPM_PT_VENDOR_TPM_TYPE			(TPM_PT)(PT_FIXED + 10)
#define TPM_PT_FIRMWARE_VERSION_1		(TPM_PT)(PT_FIXED + 11)
#define TPM_PT_FIRMWARE_VERSION_2		(TPM_PT)(PT_FIXED + 12)
#define TPM_PT_INPUT_BUFFER				(TPM_PT)(PT_FIXED + 13)
#define TPM_PT_HR_TRANSIENT_MIN			(TPM_PT)(PT_FIXED + 14)
#define TPM_PT_HR_PERSISTENT_MIN		(TPM_PT)(PT_FIXED + 15)
#define TPM_PT_HR_LOADED_MIN			(TPM_PT)(PT_FIXED + 16)
#define TPM_PT_ACTIVE_SESSIONS_MAX		(TPM_PT)(PT_FIXED + 17)
#define TPM_PT_PCR_COUNT				(TPM_PT)(PT_FIXED + 18)
#define TPM_PT_PCR_SELECT_MIN			(TPM_PT)(PT_FIXED + 19)
#define TPM_PT_CONTEXT_GAP_MAX			(TPM_PT)(PT_FIXED + 20)
#define TPM_PT_NV_COUNTERS_MAX			(TPM_PT)(PT_FIXED + 22)
#define TPM_PT_NV_INDEX_MAX				(TPM_PT)(PT_FIXED + 23)
#define TPM_PT_MEMORY					(TPM_PT)(PT_FIXED + 24)
#define TPM_PT_CLOCK_UPDATE				(TPM_PT)(PT_FIXED + 25)
#define TPM_PT_CONTEXT_HASH				(TPM_PT)(PT_FIXED + 26)
#define TPM_PT_CONTEXT_SYM				(TPM_PT)(PT_FIXED + 27)
#define TPM_PT_CONTEXT_SYM_SIZE			(TPM_PT)(PT_FIXED + 28)
#define TPM_PT_ORDERLY_COUNT			(TPM_PT)(PT_FIXED + 29)
#define TPM_PT_MAX_COMMAND_SIZE			(TPM_PT)(PT_FIXED + 30)
#define TPM_PT_MAX_RESPONSE_SIZE		(TPM_PT)(PT_FIXED + 31)
#define TPM_PT_MAX_DIGEST				(TPM_PT)(PT_FIXED + 32)
#define TPM_PT_MAX_OBJECT_CONTEXT		(TPM_PT)(PT_FIXED + 33)
#define TPM_PT_MAX_SESSION_CONTEXT		(TPM_PT)(PT_FIXED + 34)
#define TPM_PT_PS_FAMILY_INDICATOR		(TPM_PT)(PT_FIXED + 35)
#define TPM_PT_PS_LEVEL					(TPM_PT)(PT_FIXED + 36)
#define TPM_PT_PS_REVISION				(TPM_PT)(PT_FIXED + 37)
#define TPM_PT_PS_DAY_OF_YEAR			(TPM_PT)(PT_FIXED + 38)
#define TPM_PT_PS_YEAR					(TPM_PT)(PT_FIXED + 39)
#define TPM_PT_SPLIT_MAX				(TPM_PT)(PT_FIXED + 40)
#define TPM_PT_TOTAL_COMMANDS			(TPM_PT)(PT_FIXED + 41)
#define TPM_PT_LIBRARY_COMMANDS			(TPM_PT)(PT_FIXED + 42)
#define TPM_PT_VENDOR_COMMANDS			(TPM_PT)(PT_FIXED + 43)
#define TPM_PT_NV_BUFFER_MAX			(TPM_PT)(PT_FIXED + 44)
#define PT_VAR							(TPM_PT)(PT_GROUP * 2)
#define TPM_PT_PERMANENT				(TPM_PT)(PT_VAR + 0)
#define TPM_PT_STARTUP_CLEAR			(TPM_PT)(PT_VAR + 1)
#define TPM_PT_HR_NV_INDEX				(TPM_PT)(PT_VAR + 2)
#define TPM_PT_HR_LOADED				(TPM_PT)(PT_VAR + 3)
#define TPM_PT_HR_LOADED_AVAIL			(TPM_PT)(PT_VAR + 4)
#define TPM_PT_HR_ACTIVE				(TPM_PT)(PT_VAR + 5)
#define TPM_PT_HR_ACTIVE_AVAIL			(TPM_PT)(PT_VAR + 6)
#define TPM_PT_HR_TRANSIENT_AVAIL		(TPM_PT)(PT_VAR + 7)
#define TPM_PT_HR_PERSISTENT			(TPM_PT)(PT_VAR + 8)
#define TPM_PT_HR_PERSISTENT_AVAIL		(TPM_PT)(PT_VAR + 9)
#define TPM_PT_NV_COUNTERS				(TPM_PT)(PT_VAR + 10)
#define TPM_PT_NV_COUNTERS_AVAIL		(TPM_PT)(PT_VAR + 11)
#define TPM_PT_ALGORITHM_SET			(TPM_PT)(PT_VAR + 12)
#define TPM_PT_LOADED_CURVES			(TPM_PT)(PT_VAR + 13)
#define TPM_PT_LOCKOUT_COUNTER			(TPM_PT)(PT_VAR + 14)
#define TPM_PT_MAX_AUTH_FAIL			(TPM_PT)(PT_VAR + 15)
#define TPM_PT_LOCKOUT_INTERVAL			(TPM_PT)(PT_VAR + 16)
#define TPM_PT_LOCKOUT_RECOVERY			(TPM_PT)(PT_VAR + 17)
#define TPM_PT_NV_WRITE_RECOVERY		(TPM_PT)(PT_VAR + 18)
#define TPM_PT_AUDIT_COUNTER_0			(TPM_PT)(PT_VAR + 19)
#define TPM_PT_AUDIT_COUNTER_1			(TPM_PT)(PT_VAR + 20)
/**
 * Table 25 - Definition of (UINT32) TPM_PT_PCR Constants [IN/OUT, S]
 */
typedef UINT32							TPM_PT_PCR;
#define TPM_PT_PCR_FIRST				(TPM_PT_PCR)(0x00000000)
#define TPM_PT_PCR_SAVE					(TPM_PT_PCR)(0x00000000)
#define TPM_PT_PCR_EXTEND_L0			(TPM_PT_PCR)(0x00000001)
#define TPM_PT_PCR_RESET_L0				(TPM_PT_PCR)(0x00000002)
#define TPM_PT_PCR_EXTEND_L1			(TPM_PT_PCR)(0x00000003)
#define TPM_PT_PCR_RESET_L1				(TPM_PT_PCR)(0x00000004)
#define TPM_PT_PCR_EXTEND_L2			(TPM_PT_PCR)(0x00000005)
#define TPM_PT_PCR_RESET_L2				(TPM_PT_PCR)(0x00000006)
#define TPM_PT_PCR_EXTEND_L3			(TPM_PT_PCR)(0x00000007)
#define TPM_PT_PCR_RESET_L3				(TPM_PT_PCR)(0x00000008)
#define TPM_PT_PCR_EXTEND_L4			(TPM_PT_PCR)(0x00000009)
#define TPM_PT_PCR_RESET_L4				(TPM_PT_PCR)(0x0000000A)
#define TPM_PT_PCR_NO_INCREMENT			(TPM_PT_PCR)(0x00000011)
#define TPM_PT_PCR_DRTM_RESET			(TPM_PT_PCR)(0x00000012)
#define TPM_PT_PCR_POLICY				(TPM_PT_PCR)(0x00000013)
#define TPM_PT_PCR_AUTH					(TPM_PT_PCR)(0x00000014)
#define TPM_PT_PCR_LAST					(TPM_PT_PCR)(0x00000014)
/**
 * Table 26 - Definition of (UINT32) TPM_PS Constants [OUT]
 */
typedef UINT32							TPM_PS;
#define TPM_PS_MAIN						(TPM_PS)(0x00000000)
#define TPM_PS_PC						(TPM_PS)(0x00000001)
#define TPM_PS_PDA						(TPM_PS)(0x00000002)
#define TPM_PS_CELL_PHONE				(TPM_PS)(0x00000003)
#define TPM_PS_SERVER					(TPM_PS)(0x00000004)
#define TPM_PS_PERIPHERAL				(TPM_PS)(0x00000005)
#define TPM_PS_TSS						(TPM_PS)(0x00000006)
#define TPM_PS_STORAGE					(TPM_PS)(0x00000007)
#define TPM_PS_AUTHENTICATION			(TPM_PS)(0x00000008)
#define TPM_PS_EMBEDDED					(TPM_PS)(0x00000009)
#define TPM_PS_HARDCOPY					(TPM_PS)(0x0000000A)
#define TPM_PS_INFRASTRUCTURE			(TPM_PS)(0x0000000B)
#define TPM_PS_VIRTUALIZATION			(TPM_PS)(0x0000000C)
#define TPM_PS_TNC						(TPM_PS)(0x0000000D)
#define TPM_PS_MULTI_TENANT				(TPM_PS)(0x0000000E)
#define TPM_PS_TC						(TPM_PS)(0x0000000F)
/**
 * Table 27 - Definition of Types for Handles
 */
typedef UINT32 TPM_HANDLE;
/**
 * Table 28 - Definition of (UINT8) TPM_HT Constants [S]
 */
typedef UINT8							TPM_HT;
#define TPM_HT_PCR						(TPM_HT)(0x00)
#define TPM_HT_NV_INDEX					(TPM_HT)(0x01)
#define TPM_HT_HMAC_SESSION				(TPM_HT)(0x02)
#define TPM_HT_LOADED_SESSION			(TPM_HT)(0x02)
#define TPM_HT_POLICY_SESSION			(TPM_HT)(0x03)
#define TPM_HT_ACTIVE_SESSION			(TPM_HT)(0x03)
#define TPM_HT_PERMANENT				(TPM_HT)(0x40)
#define TPM_HT_TRANSIENT				(TPM_HT)(0x80)
#define TPM_HT_PERSISTENT				(TPM_HT)(0x81)
/**
 * Table 29 - Definition of (TPM_HANDLE) TPM_RH Constants [S]
 */
typedef TPM_HANDLE						TPM_RH;
#define TPM_RH_FIRST					(TPM_RH)(0x40000000)
#define TPM_RH_SRK						(TPM_RH)(0x40000000)
#define TPM_RH_OWNER					(TPM_RH)(0x40000001)
#define TPM_RH_REVOKE					(TPM_RH)(0x40000002)
#define TPM_RH_TRANSPORT				(TPM_RH)(0x40000003)
#define TPM_RH_OPERATOR					(TPM_RH)(0x40000004)
#define TPM_RH_ADMIN					(TPM_RH)(0x40000005)
#define TPM_RH_EK						(TPM_RH)(0x40000006)
#define TPM_RH_NULL						(TPM_RH)(0x40000007)
#define TPM_RH_UNASSIGNED				(TPM_RH)(0x40000008)
#define TPM_RS_PW						(TPM_RH)(0x40000009)
#define TPM_RH_LOCKOUT					(TPM_RH)(0x4000000A)
#define TPM_RH_ENDORSEMENT				(TPM_RH)(0x4000000B)
#define TPM_RH_PLATFORM					(TPM_RH)(0x4000000C)
#define TPM_RH_PLATFORM_NV				(TPM_RH)(0x4000000D)
#define TPM_RH_AUTH_00					(TPM_RH)(0x40000010)
#define TPM_RH_AUTH_FF					(TPM_RH)(0x4000010F)
#define TPM_RH_LAST						(TPM_RH)(0x4000010F)
/**
 * Table 30 - Definition of (TPM_HANDLE) TPM_HC Constants [S]
 */
typedef TPM_HANDLE						TPM_HC;
#define HR_HANDLE_MASK					(TPM_HC)(0x00FFFFFF)
#define HR_RANGE_MASK					(TPM_HC)(0xFF000000)
#define HR_SHIFT						(TPM_HC)(24)
#define HR_PCR							(TPM_HC)((TPM_HT_PCR << HR_SHIFT))
#define HR_HMAC_SESSION					(TPM_HC)((TPM_HT_HMAC_SESSION << HR_SHIFT))
#define HR_POLICY_SESSION				(TPM_HC)((TPM_HT_POLICY_SESSION << HR_SHIFT))
#define HR_TRANSIENT					(TPM_HC)((TPM_HT_TRANSIENT << HR_SHIFT))
#define HR_PERSISTENT					(TPM_HC)((TPM_HT_PERSISTENT << HR_SHIFT))
#define HR_NV_INDEX						(TPM_HC)((TPM_HT_NV_INDEX << HR_SHIFT))
#define HR_PERMANENT					(TPM_HC)((TPM_HT_PERMANENT << HR_SHIFT))
#define PCR_FIRST						(TPM_HC)((HR_PCR + 0))
#define PCR_LAST						(TPM_HC)((PCR_FIRST + IMPLEMENTATION_PCR-1))
#define HMAC_SESSION_FIRST				(TPM_HC)((HR_HMAC_SESSION + 0))
#define HMAC_SESSION_LAST				(TPM_HC)((HMAC_SESSION_FIRST+MAX_ACTIVE_SESSIONS-1))
#define LOADED_SESSION_FIRST			(TPM_HC)(HMAC_SESSION_FIRST)
#define LOADED_SESSION_LAST				(TPM_HC)(HMAC_SESSION_LAST)
#define POLICY_SESSION_FIRST			(TPM_HC)((HR_POLICY_SESSION + 0))
#define POLICY_SESSION_LAST				(TPM_HC)((POLICY_SESSION_FIRST + MAX_ACTIVE_SESSIONS-1))
#define TRANSIENT_FIRST					(TPM_HC)((HR_TRANSIENT + 0))
#define ACTIVE_SESSION_FIRST			(TPM_HC)(POLICY_SESSION_FIRST)
#define ACTIVE_SESSION_LAST				(TPM_HC)(POLICY_SESSION_LAST)
#define TRANSIENT_LAST					(TPM_HC)((TRANSIENT_FIRST+MAX_LOADED_OBJECTS-1))
#define PERSISTENT_FIRST				(TPM_HC)((HR_PERSISTENT + 0))
#define PERSISTENT_LAST					(TPM_HC)((PERSISTENT_FIRST + 0x00FFFFFF))
#define PLATFORM_PERSISTENT				(TPM_HC)((PERSISTENT_FIRST + 0x00800000))
#define NV_INDEX_FIRST					(TPM_HC)((HR_NV_INDEX + 0))
#define NV_INDEX_LAST					(TPM_HC)((NV_INDEX_FIRST + 0x00FFFFFF))
#define PERMANENT_FIRST					(TPM_HC)(TPM_RH_FIRST)
#define PERMANENT_LAST					(TPM_HC)(TPM_RH_LAST)
/**
 * Table 31 - Definition of (UINT32) TPMA_ALGORITHM Bits
 */
/**
 *	@brief		TPMA_ALGORITHM structure
 *	@details	TPMA_ALGORITHM structure definition
 */
typedef struct _TPMA_ALGORITHM {
	unsigned int asymmetric						: 1;	/**
														* SET (1): an asymmetric algorithm with public and private portions
														* CLEAR (0): not an asymmetric algorithm
														*/
	unsigned int symmetric						: 1;	/**
														* SET (1): a symmetric block cipher
														* CLEAR (0): not a symmetric block cipher
														*/
	unsigned int hash							: 1;	/**
														* SET (1): a hash algorithm
														* CLEAR (0): not a hash algorithm
														*/
	unsigned int object							: 1;	/**
														* SET (1): an algorithm that may be used as an object type
														* CLEAR (0): an algorithm that is not used as an object type
														*/
	unsigned int reserved5						: 4;
	unsigned int signing						: 1;	/**
														* SET (1): a signing algorithm. The setting of asymmetric, symmetric, and hash will indicate the type of signing algorithm.
														* CLEAR (0): not a signing algorithm
														*/
	unsigned int encrypting						: 1;	/**
														* SET (1): an encryption/decryption algorithm. The setting of asymmetric, symmetric, and hash will indicate the type of encryption/decryption algorithm.
														* CLEAR (0): not an encryption/decryption algorithm
														*/
	unsigned int method							: 1;	/**
														* SET (1): a method such as a key derivative function (KDF)
														* CLEAR (0): not a method
														*/
	unsigned int reserved9						: 21;
} TPMA_ALGORITHM;

/**
 * Table 32 - Definition of (UINT32) TPMA_OBJECT Bits
 */
/**
 *	@brief		TPMA_OBJECT structure
 *	@details	TPMA_OBJECT structure definition
 */
typedef struct _TPMA_OBJECT {
	unsigned int reserved1						: 1;	/**
														* shall be zero
														*/
	unsigned int fixedTPM						: 1;	/**
														* SET (1): The hierarchy of the object, as indicated by its Qualified Name, may not change.
														* CLEAR (0): The hierarchy of the object may change as a result of this object or an ancestor key being duplicated for use in another hierarchy.
														*/
	unsigned int stClear						: 1;	/**
														* SET (1): Previously saved contexts of this object may not be loaded after Startup(CLEAR).
														* CLEAR (0): Saved contexts of this object may be used after a Shutdown(STATE) and subsequent Startup().
														*/
	unsigned int reserved4						: 1;	/**
														* shall be zero
														*/
	unsigned int fixedParent					: 1;	/**
														* SET (1): The parent of the object may not change.
														* CLEAR (0): The parent of the object may change as the result of a TPM2_Duplicate() of the object.
														*/
	unsigned int sensitiveDataOrigin			: 1;	/**
														* SET (1): Indicates that, when the object was created with TPM2_Create() or TPM2_CreatePrimary(), the TPM generated all of the sensitive data other than the authValue.
														* CLEAR (0): A portion of the sensitive data, other than the authValue, was provided by the caller.
														*/
	unsigned int userWithAuth					: 1;	/**
														* SET (1): Approval of USER role actions with this object may be with an HMAC session or with a password using the authValue of the object or a policy session.
														* CLEAR (0): Approval of USER role actions with this object may only be done with a policy session.
														*/
	unsigned int adminWithPolicy				: 1;	/**
														* SET (1): Approval of ADMIN role actions with this object may only be done with a policy session.
														* CLEAR (0): Approval of ADMIN role actions with this object may be with an HMAC session or with a password using the authValue of the object or a policy session.
														*/
	unsigned int reserved9						: 2;	/**
														* shall be zero
														*/
	unsigned int noDA							: 1;	/**
														* SET (1): The object is not subject to dictionary attack protections.
														* CLEAR (0): The object is subject to dictionary attack protections.
														*/
	unsigned int encryptedDuplication			: 1;	/**
														* SET (1): If the object is duplicated, then symmetricAlg shall not be TPM_ALG_NULL and newParentHandle shall not be TPM_RH_NULL.
														* CLEAR (0): The object may be duplicated without an inner wrapper on the private portion of the object and the new parent may be TPM_RH_NULL.
														*/
	unsigned int reserved12						: 4;	/**
														* shall be zero
														*/
	unsigned int restricted						: 1;	/**
														* SET (1): Key usage is restricted to manipulate structures of known format; the parent of this key shall have restricted SET.
														* CLEAR (0): Key usage is not restricted to use on special formats.
														*/
	unsigned int decrypt						: 1;	/**
														* SET (1): The private portion of the key may be used to decrypt.
														* CLEAR (0): The private portion of the key may not be used to decrypt.
														*/
	unsigned int sign							: 1;	/**
														* SET (1): The private portion of the key may be used to sign.
														* CLEAR (0): The private portion of the key may not be used to sign.
														*/
	unsigned int reserved16						: 13;	/**
														* shall be zero
														*/
} TPMA_OBJECT;

/**
 * Table 33 - Definition of (UINT8) TPMA_SESSION Bits [IN/OUT]
 */
/**
 *	@brief		TPMA_SESSION structure
 *	@details	TPMA_SESSION structure definition
 */
typedef struct _TPMA_SESSION {
	unsigned int continueSession				: 1;	/**
														* SET (1): In a command, this setting indicates that the session is to remain active after successful completion of the command. In a response, it indicates that the session is still active. If SET in the command, this attribute shall be SET in the response.
														* CLEAR (0): In a command, this setting indicates that the TPM should close the session and flush any related context when the command completes successfully. In a response, it indicates that the session is closed and the context is no longer active.
														* This attribute has no meaning for a password authorization and the TPM will allow any setting of the attribute in the command and SET the attribute in the response.
														* This attribute will only be CLEAR in one response for a logical session. If the attribute is CLEAR, the context associated with the session is no longer in use and the space is available. A session created after another session is ended may have the same handle but logically is not the same session.
														* This attribute has no effect if the command does not complete successfully.
														*/
	unsigned int auditExclusive					: 1;	/**
														* SET (1): In a command, this setting indicates that the command should only be executed if the session is exclusive at the start of the command. In a response, it indicates that the session is exclusive. This setting is only allowed if the audit attribute is SET.
														* CLEAR (0): If audit is CLEAR, then this field is reserved but the error is TPM_RC_ATTRIBUTES rather than TPM_RC_RESERVED_BITS.
														* See "Exclusive Audit Session" clause in TPM 2.0 Part 1.
														*/
	unsigned int auditReset						: 1;	/**
														* SET (1): In a command, this setting indicates that the audit digest of the session should be initialized and the exclusive status of the session SET.
														* CLEAR (0): If audit is CLEAR, then this field is reserved but the error is TPM_RC_ATTRIBUTES rather than TPM_RC_RESERVED_BITS. This setting is always used for a response.
														*/
	unsigned int reserved4						: 2;	/**
														* shall be CLEAR
														*/
	unsigned int decrypt						: 1;	/**
														* SET (1): In a command, this setting indicates that the first parameter in the command is symmetrically encrypted using the parameter encryption scheme described in TPM 2.0 Part 1. The TPM will decrypt the parameter after performing any HMAC computations and before unmarshaling the parameter. In a response, the attribute is copied from the request but has no effect on the response.
														* CLEAR (0): Session not used for encryption.
														* For a password authorization, this attribute will be CLEAR in both the command and response.
														* This attribute may only be SET in one session per command.
														* This attribute may be SET in a session that is not associated with a command handle. Such a session is provided for purposes of encrypting a parameter and not for authorization.
														* This attribute may be SET in combination with any other session attributes.
														* This attribute may only be SET if the first parameter of the command is a sized buffer (TPM2B_).
														*/
	unsigned int encrypt						: 1;	/**
														* SET (1): In a command, this setting indicates that the TPM should use this session to encrypt the first parameter in the response. In a response, it indicates that the attribute was set in the command and that the TPM used the session to encrypt the first parameter in the response using the parameter encryption scheme described in TPM 2.0 Part 1.
														* CLEAR (0): Session not used for encryption.
														* For a password authorization, this attribute will be CLEAR in both the command and response.
														* This attribute may only be SET in one session per command.
														* This attribute may be SET in a session that is not associated with a command handle. Such a session is provided for purposes of encrypting a parameter and not for authorization.
														* This attribute may only be SET if the first parameter of a response is a sized buffer (TPM2B_).
														*/
	unsigned int audit							: 1;	/**
														* SET (1): In a command or response, this setting indicates that the session is for audit and that auditExclusive and auditReset have meaning. This session may also be used for authorization, encryption, or decryption. The encrypted and encrypt fields may be SET or CLEAR.
														* CLEAR (0): Session is not used for audit.
														* This attribute may only be SET in one session per command or response. If SET in the command, then this attribute will be SET in the response.
														*/
} TPMA_SESSION;

/**
 * Table 34 - Definition of (UINT8) TPMA_LOCALITY Bits [IN/OUT]
 */
/**
 *	@brief		TPMA_LOCALITY structure
 *	@details	TPMA_LOCALITY structure definition
 */
typedef struct _TPMA_LOCALITY {
	unsigned int TPM_LOC_ZERO					: 1;
	unsigned int TPM_LOC_ONE					: 1;
	unsigned int TPM_LOC_TWO					: 1;
	unsigned int TPM_LOC_THREE					: 1;
	unsigned int TPM_LOC_FOUR					: 1;
	unsigned int Extended						: 3;	/**
														* If any of these bits is set, an extended locality is indicated
														*/
} TPMA_LOCALITY;

/**
 * Table 35 - Definition of (UINT32) TPMA_PERMANENT Bits [OUT]
 */
/**
 *	@brief		TPMA_PERMANENT structure
 *	@details	TPMA_PERMANENT structure definition
 */
typedef struct _TPMA_PERMANENT {
	unsigned int ownerAuthSet					: 1;	/**
														* SET (1): TPM2_HierarchyChangeAuth() with ownerAuth has been executed since the last TPM2_Clear().
														* CLEAR (0): ownerAuth has not been changed since TPM2_Clear().
														*/
	unsigned int endorsementAuthSet				: 1;	/**
														* SET (1): TPM2_HierarchyChangeAuth() with endorsementAuth has been executed since the last TPM2_Clear().
														* CLEAR (0): endorsementAuth has not been changed since TPM2_Clear().
														*/
	unsigned int lockoutAuthSet					: 1;	/**
														* SET (1): TPM2_HierarchyChangeAuth() with lockoutAuth has been executed since the last TPM2_Clear().
														* CLEAR (0): lockoutAuth has not been changed since TPM2_Clear().
														*/
	unsigned int reserved4						: 5;
	unsigned int disableClear					: 1;	/**
														* SET (1): TPM2_Clear() is disabled.
														* CLEAR (0): TPM2_Clear() is enabled.
														* NOTE	See "TPM2_ClearControl" in TPM 2.0 Part 3 for details on changing this attribute.
														*/
	unsigned int inLockout						: 1;	/**
														* SET (1): The TPM is in lockout and commands that require authorization with other than Platform Authorization or Lockout Authorization will not succeed.
														*/
	unsigned int tpmGeneratedEPS				: 1;	/**
														* SET (1): The EPS was created by the TPM.
														* CLEAR (0): The EPS was created outside of the TPM using a manufacturer-specific process.
														*/
	unsigned int reserved8						: 21;
} TPMA_PERMANENT;

/**
 * Table 36 - Definition of (UINT32) TPMA_STARTUP_CLEAR Bits [OUT]
 */
/**
 *	@brief		TPMA_STARTUP_CLEAR structure
 *	@details	TPMA_STARTUP_CLEAR structure definition
 */
typedef struct _TPMA_STARTUP_CLEAR {
	unsigned int phEnable						: 1;	/**
														* SET (1): The platform hierarchy is enabled and platformAuth or platformPolicy may be used for authorization.
														* CLEAR (0): platformAuth and platformPolicy may not be used for authorizations, and objects in the platform hierarchy, including persistent objects, cannot be used.
														* NOTE	See "TPM2_HierarchyControl" in TPM 2.0 Part 3 for details on changing this attribute.
														*/
	unsigned int shEnable						: 1;	/**
														* SET (1): The Storage hierarchy is enabled and ownerAuth or ownerPolicy may be used for authorization. NV indices defined using owner authorization are accessible.
														* CLEAR (0): ownerAuth and ownerPolicy may not be used for authorizations, and objects in the Storage hierarchy, persistent objects, and NV indices defined using owner authorization cannot be used.
														* NOTE	See "TPM2_HierarchyControl" in TPM 2.0 Part 3 for details on changing this attribute.
														*/
	unsigned int ehEnable						: 1;	/**
														* SET (1): The EPS hierarchy is enabled and Endorsement Authorization may be used to authorize commands.
														* CLEAR (0): Endorsement Authorization may not be used for authorizations, and objects in the endorsement hierarchy, including persistent objects, cannot be used.
														* NOTE	See "TPM2_HierarchyControl" in TPM 2.0 Part 3 for details on changing this attribute.
														*/
	unsigned int phEnableNV						: 1;	/**
														* SET (1): NV indices that have TPMA_PLATFORM_CREATE SET may be read or written. The platform can create define and undefine indices.
														* CLEAR (0): NV indices that have TPMA_PLATFORM_CREATE SET may not be read or written (TPM_RC_HANDLE). The platform cannot define (TPM_RC_HIERARCHY) or undefined (TPM_RC_HANDLE) indices.
														* NOTE	See "TPM2_HierarchyControl" in TPM 2.0 Part 3 for details on changing this attribute.
														* NOTE
														* read refers to these commands: TPM2_NV_Read, TPM2_NV_ReadPublic, TPM_NV_Certify, TPM2_PolicyNV
														* write refers to these commands: TPM2_NV_Write, TPM2_NV_Increment, TPM2_NV_Extend, TPM2_NV_SetBits
														* NOTE The TPM must query the index TPMA_PLATFORM_CREATE attribute to determine whether phEnableNV is applicable. Since the TPM will return TPM_RC_HANDLE if the index does not exist, it also returns this error code if the index is disabled. Otherwise, the TPM would leak the existence of an index even when disabled.
														*/
	unsigned int reserved5						: 27;	/**
														* shall be zero
														*/
	unsigned int orderly						: 1;	/**
														* SET (1): The TPM received a TPM2_Shutdown() and a matching TPM2_Startup().
														* CLEAR (0): TPM2_Startup(TPM_SU_CLEAR) was not preceded by a TPM2_Shutdown() of any type.
														* NOTE A shutdown is orderly if the TPM receives a TPM2_Shutdown() of any type followed by a TPM2_Startup() of any type. However, the TPM will return an error if TPM2_Startup(TPM_SU_STATE) was not preceded by TPM2_State_Save(TPM_SU_STATE).
														*/
} TPMA_STARTUP_CLEAR;

/**
 * Table 37 - Definition of (UINT32) TPMA_MEMORY Bits [Out]
 */
/**
 *	@brief		TPMA_MEMORY structure
 *	@details	TPMA_MEMORY structure definition
 */
typedef struct _TPMA_MEMORY {
	unsigned int sharedRAM						: 1;	/**
														* SET (1): indicates that the RAM memory used for authorization session contexts is shared with the memory used for transient objects
														* CLEAR (0): indicates that the memory used for authorization sessions is not shared with memory used for transient objects
														*/
	unsigned int sharedNV						: 1;	/**
														* SET (1): indicates that the NV memory used for persistent objects is shared with the NV memory used for NV Index values
														* CLEAR (0): indicates that the persistent objects and NV Index values are allocated from separate sections of NV
														*/
	unsigned int objectCopiedToRam				: 1;	/**
														* SET (1): indicates that the TPM copies persistent objects to a transient-object slot in RAM when the persistent object is referenced in a command. The TRM is required to make sure that an object slot is available.
														* CLEAR (0): indicates that the TPM does not use transient-object slots when persistent objects are referenced
														*/
	unsigned int reserved4						: 29;	/**
														* shall be zero
														*/
} TPMA_MEMORY;

/**
 * Table 38 - Definition of (TPM_CC) TPMA_CC Bits [OUT]
 */
/**
 *	@brief		TPMA_CC structure
 *	@details	TPMA_CC structure definition
 */
typedef struct _TPMA_CC {
	unsigned int commandIndex					: 16;	/**
														* indicates the command being selected
														*/
	unsigned int reserved2						: 6;	/**
														* shall be zero
														*/
	unsigned int nv								: 1;	/**
														* SET (1): indicates that the command may write to NV
														* CLEAR (0): indicates that the command does not write to NV
														*/
	unsigned int extensive						: 1;	/**
														* SET (1): This command could flush any number of loaded contexts.
														* CLEAR (0): no additional changes other than indicated by the flushed attribute
														*/
	unsigned int flushed						: 1;	/**
														* SET (1): The context associated with any transient handle in the command will be flushed when this command completes.
														* CLEAR (0): No context is flushed as a side effect of this command.
														*/
	unsigned int cHandles						: 3;	/**
														* indicates the number of the handles in the handle area for this command
														*/
	unsigned int rHandle						: 1;	/**
														* SET (1): indicates the presence of the handle area in the response
														*/
	unsigned int V								: 1;	/**
														* SET (1): indicates that the command is vendor-specific
														* CLEAR (0): indicates that the command is defined in a version of this specification
														*/
	unsigned int Res							: 2;	/**
														* allocated for software; shall be zero
														*/
} TPMA_CC;

/**
 * Table 39 - Definition of (BYTE) TPMI_YES_NO Type
 */
typedef BYTE TPMI_YES_NO;

/**
 * Table 40 - Definition of (TPM_HANDLE) TPMI_DH_OBJECT Type
 */
typedef TPM_HANDLE TPMI_DH_OBJECT;

/**
 * Table 41 - Definition of (TPM_HANDLE) TPMI_DH_PERSISTENT Type
 */
typedef TPM_HANDLE TPMI_DH_PERSISTENT;

/**
 * Table 42 - Definition of (TPM_HANDLE) TPMI_DH_ENTITY Type [IN]
 */
typedef TPM_HANDLE TPMI_DH_ENTITY;

/**
 * Table 43 - Definition of (TPM_HANDLE) TPMI_DH_PCR Type [IN]
 */
typedef TPM_HANDLE TPMI_DH_PCR;

/**
 * Table 44 - Definition of (TPM_HANDLE) TPMI_SH_AUTH_SESSION Type [IN/OUT]
 */
typedef TPM_HANDLE TPMI_SH_AUTH_SESSION;

/**
 * Table 45 - Definition of (TPM_HANDLE) TPMI_SH_HMAC Type [IN/OUT]
 */
typedef TPM_HANDLE TPMI_SH_HMAC;

/**
 * Table 46 - Definition of (TPM_HANDLE) TPMI_SH_POLICY Type [IN/OUT]
 */
typedef TPM_HANDLE TPMI_SH_POLICY;

/**
 * Table 47 - Definition of (TPM_HANDLE) TPMI_DH_CONTEXT Type
 */
typedef TPM_HANDLE TPMI_DH_CONTEXT;

/**
 * Table 48 - Definition of (TPM_HANDLE) TPMI_RH_HIERARCHY Type
 */
typedef TPM_HANDLE TPMI_RH_HIERARCHY;

/**
 * Table 49 - Definition of (TPM_HANDLE) TPMI_RH_ENABLES Type
 */
typedef TPM_HANDLE TPMI_RH_ENABLES;

/**
 * Table 50 - Definition of (TPM_HANDLE) TPMI_RH_HIERARCHY_AUTH Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_HIERARCHY_AUTH;

/**
 * Table 51 - Definition of (TPM_HANDLE) TPMI_RH_PLATFORM Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_PLATFORM;

/**
 * Table 52 - Definition of (TPM_HANDLE) TPMI_RH_OWNER Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_OWNER;

/**
 * Table 53 - Definition of (TPM_HANDLE) TPMI_RH_ENDORSEMENT Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_ENDORSEMENT;

/**
 * Table 54 - Definition of (TPM_HANDLE) TPMI_RH_PROVISION Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_PROVISION;

/**
 * Table 55 - Definition of (TPM_HANDLE) TPMI_RH_CLEAR Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_CLEAR;

/**
 * Table 56 - Definition of (TPM_HANDLE) TPMI_RH_NV_AUTH Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_NV_AUTH;

/**
 * Table 57 - Definition of (TPM_HANDLE) TPMI_RH_LOCKOUT Type [IN]
 */
typedef TPM_HANDLE TPMI_RH_LOCKOUT;

/**
 * Table 58 - Definition of (TPM_HANDLE) TPMI_RH_NV_INDEX Type [IN/OUT]
 */
typedef TPM_HANDLE TPMI_RH_NV_INDEX;

/**
 * Table 59 - Definition of (TPM_ALG_ID) TPMI_ALG_HASH Type
 */
typedef TPM_ALG_ID TPMI_ALG_HASH;

/**
 * Table 60 - Definition of (TPM_ALG_ID) TPMI_ALG_ASYM Type
 */
typedef TPM_ALG_ID TPMI_ALG_ASYM;

/**
 * Table 61 - Definition of (TPM_ALG_ID) TPMI_ALG_SYM Type
 */
typedef TPM_ALG_ID TPMI_ALG_SYM;

/**
 * Table 62 - Definition of (TPM_ALG_ID) TPMI_ALG_SYM_OBJECT Type
 */
typedef TPM_ALG_ID TPMI_ALG_SYM_OBJECT;

/**
 * Table 63 - Definition of (TPM_ALG_ID) TPMI_ALG_SYM_MODE Type
 */
typedef TPM_ALG_ID TPMI_ALG_SYM_MODE;

/**
 * Table 64 - Definition of (TPM_ALG_ID) TPMI_ALG_KDF Type
 */
typedef TPM_ALG_ID TPMI_ALG_KDF;

/**
 * Table 65 - Definition of (TPM_ALG_ID) TPMI_ALG_SIG_SCHEME Type
 */
typedef TPM_ALG_ID TPMI_ALG_SIG_SCHEME;

/**
 * Table 67 - Definition of (TPM_ST) TPMI_ST_COMMAND_TAG Type
 */
typedef TPM_ST TPMI_ST_COMMAND_TAG;

/**
 * Table 68 - Definition of TPMS_EMPTY Structure [IN/OUT]
 */
typedef BYTE TPMS_EMPTY;
/**
 * Table 69 - Definition of TPMS_ALGORITHM_DESCRIPTION Structure [OUT]
 */
/**
 *	@brief		TPMS_ALGORITHM_DESCRIPTION structure
 *	@details	TPMS_ALGORITHM_DESCRIPTION structure definition
 */
typedef struct _TPMS_ALGORITHM_DESCRIPTION {
	/// An algorithm
	TPM_ALG_ID alg;
	/// The attributes of the algorithm
	TPMA_ALGORITHM attributes;
} TPMS_ALGORITHM_DESCRIPTION;

/**
 * Table 70 - Definition of TPMU_HA Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_HA union
 *	@details	TPMU_HA union definition
 */
typedef union _TPMU_HA {
	BYTE sha1[SHA1_DIGEST_SIZE];
	BYTE sha256[SHA256_DIGEST_SIZE];
	BYTE sha384[SHA384_DIGEST_SIZE];
	BYTE sha512[SHA512_DIGEST_SIZE];
	BYTE sm3_256[SM3_256_DIGEST_SIZE];
} TPMU_HA;

/**
 * Table 71 - Definition of TPMT_HA Structure [IN/OUT]
 */
/**
 *	@brief		TPMT_HA structure
 *	@details	TPMT_HA structure definition
 */
typedef struct _TPMT_HA {
	/// Selector of the hash contained in the digest that implies the size of the digest
	/// NOTE	The leading "+" on the type indicates that this structure should pass an indication to the unmarshaling function for TPMI_ALG_HASH so that TPM_ALG_NULL will be allowed if a use of a TPMT_HA allows TPM_ALG_NULL.
	TPMI_ALG_HASH hashAlg;
	/// The digest data
	TPMU_HA digest;
} TPMT_HA;

/**
 * Table 72 - Definition of TPM2B_DIGEST Structure
 */
/**
 *	@brief		TPM2B_DIGEST structure
 *	@details	TPM2B_DIGEST structure definition
 */
typedef struct _TPM2B_DIGEST {
	/// Size in octets of the buffer field; may be 0
	UINT16 size;
	/// The buffer area that can be no larger than a digest
	BYTE buffer[sizeof(TPMU_HA)];
} TPM2B_DIGEST;

/**
 * Table 73 - Definition of TPM2B_DATA Structure
 */
/**
 *	@brief		TPM2B_DATA structure
 *	@details	TPM2B_DATA structure definition
 */
typedef struct _TPM2B_DATA {
	/// Size in octets of the buffer field; may be 0
	UINT16 size;
	/// The buffer area that contains the algorithm ID and the digest
	BYTE buffer[sizeof(TPMT_HA)];
} TPM2B_DATA;

/**
 * Table 74 - Definition of Types for TPM2B_NONCE
 */
typedef TPM2B_DIGEST TPM2B_NONCE;
/**
 * Table 75 - Definition of Types for TPM2B_AUTH
 */
typedef TPM2B_DIGEST TPM2B_AUTH;
/**
 * Table 76 - Definition of Types for TPM2B_OPERAND
 */
typedef TPM2B_DIGEST TPM2B_OPERAND;
/**
 * Table 77 - Definition of TPM2B_EVENT Structure
 */
/**
 *	@brief		TPM2B_EVENT structure
 *	@details	TPM2B_EVENT structure definition
 */
typedef struct _TPM2B_EVENT {
	/// Size of the operand buffer
	UINT16 size;
	/// The operand
	BYTE buffer[1024];
} TPM2B_EVENT;

/**
 * Table 78 - Definition of TPM2B_MAX_BUFFER Structure
 */
/**
 *	@brief		TPM2B_MAX_BUFFER structure
 *	@details	TPM2B_MAX_BUFFER structure definition
 */
typedef struct _TPM2B_MAX_BUFFER {
	/// Size of the buffer
	UINT16 size;
	/// The operand
	/// NOTE	MAX_DIGEST_BUFFER is TPM-dependent but is required to be at least 1,024.
	BYTE buffer[MAX_DIGEST_BUFFER];
} TPM2B_MAX_BUFFER;

/**
 * Table 79 - Definition of TPM2B_MAX_NV_BUFFER Structure
 */
/**
 *	@brief		TPM2B_MAX_NV_BUFFER structure
 *	@details	TPM2B_MAX_NV_BUFFER structure definition
 */
typedef struct _TPM2B_MAX_NV_BUFFER {
	/// Size of the buffer
	UINT16 size;
	/// The operand
	/// NOTE	MAX_NV_BUFFER_SIZE is TPM-dependent
	BYTE buffer[MAX_NV_BUFFER_SIZE];
} TPM2B_MAX_NV_BUFFER;

/**
 * Table 80 - Definition of TPM2B_TIMEOUT Structure [IN/OUT]
 */
/**
 *	@brief		TPM2B_TIMEOUT structure
 *	@details	TPM2B_TIMEOUT structure definition
 */
typedef struct _TPM2B_TIMEOUT {
	/// Size of the timeout value
	/// This value is fixed for a TPM implementation.
	UINT16 size;
	/// The timeout value
	BYTE buffer[sizeof(UINT64)];
} TPM2B_TIMEOUT;

/**
 * Table 81 - Definition of TPM2B_IV Structure [IN/OUT]
 */
/**
 *	@brief		TPM2B_IV structure
 *	@details	TPM2B_IV structure definition
 */
typedef struct _TPM2B_IV {
	/// Size of the timeout value
	/// This value is fixed for a TPM implementation.
	UINT16 size;
	/// The timeout value
	BYTE buffer[MAX_SYM_BLOCK_SIZE];
} TPM2B_IV;

/**
 * Table 82 - Definition of TPMU_NAME Union <>
 */
/**
 *	@brief		TPMU_NAME union
 *	@details	TPMU_NAME union definition
 */
typedef union _TPMU_NAME {
	TPMT_HA digest;
	TPM_HANDLE handle;
} TPMU_NAME;

/**
 * Table 83 - Definition of TPM2B_NAME Structure
 */
/**
 *	@brief		TPM2B_NAME structure
 *	@details	TPM2B_NAME structure definition
 */
typedef struct _TPM2B_NAME {
	/// Size of the Name structure
	UINT16 size;
	/// The Name structure
	BYTE name[sizeof(TPMU_NAME)];
} TPM2B_NAME;

/**
 * Table 84 - Definition of TPMS_PCR_SELECT Structure
 */
/**
 *	@brief		TPMS_PCR_SELECT structure
 *	@details	TPMS_PCR_SELECT structure definition
 */
typedef struct _TPMS_PCR_SELECT {
	/// The size in octets of the pcrSelect array
	UINT8 sizeofSelect;
	/// The bit map of selected PCR
	BYTE pcrSelect[PCR_SELECT_MAX];
} TPMS_PCR_SELECT;

/**
 * Table 85 - Definition of TPMS_PCR_SELECTION Structure
 */
/**
 *	@brief		TPMS_PCR_SELECTION structure
 *	@details	TPMS_PCR_SELECTION structure definition
 */
typedef struct _TPMS_PCR_SELECTION {
	/// The hash algorithm associated with the selection
	TPMI_ALG_HASH hash;
	/// The size in octets of the pcrSelect array
	UINT8 sizeofSelect;
	/// The bit map of selected PCR
	BYTE pcrSelect[PCR_SELECT_MAX];
} TPMS_PCR_SELECTION;

/**
 * Table 88 - Definition of TPMT_TK_CREATION Structure
 */
/**
 *	@brief		TPMT_TK_CREATION structure
 *	@details	TPMT_TK_CREATION structure definition
 */
typedef struct _TPMT_TK_CREATION {
	/// Ticket structure tag
	TPM_ST tag;
	/// The hierarchy containing name
	TPMI_RH_HIERARCHY hierarchy;
	/// This shall be the HMAC produced using a proof value of hierarchy.
	TPM2B_DIGEST digest;
} TPMT_TK_CREATION;

/**
 * Table 89 - Definition of TPMT_TK_VERIFIED Structure
 */
/**
 *	@brief		TPMT_TK_VERIFIED structure
 *	@details	TPMT_TK_VERIFIED structure definition
 */
typedef struct _TPMT_TK_VERIFIED {
	/// Ticket structure tag
	TPM_ST tag;
	/// The hierarchy containing keyName
	TPMI_RH_HIERARCHY hierarchy;
	/// This shall be the HMAC produced using a proof value of hierarchy.
	TPM2B_DIGEST digest;
} TPMT_TK_VERIFIED;

/**
 * Table 90 - Definition of TPMT_TK_AUTH Structure
 */
/**
 *	@brief		TPMT_TK_AUTH structure
 *	@details	TPMT_TK_AUTH structure definition
 */
typedef struct _TPMT_TK_AUTH {
	/// Ticket structure tag
	TPM_ST tag;
	/// The hierarchy of the object used to produce the ticket
	TPMI_RH_HIERARCHY hierarchy;
	/// This shall be the HMAC produced using a proof value of hierarchy.
	TPM2B_DIGEST digest;
} TPMT_TK_AUTH;

/**
 * Table 91 - Definition of TPMT_TK_HASHCHECK Structure
 */
/**
 *	@brief		TPMT_TK_HASHCHECK structure
 *	@details	TPMT_TK_HASHCHECK structure definition
 */
typedef struct _TPMT_TK_HASHCHECK {
	/// Ticket structure tag
	TPM_ST tag;
	/// The hierarchy
	TPMI_RH_HIERARCHY hierarchy;
	/// This shall be the HMAC produced using a proof value of hierarchy.
	TPM2B_DIGEST digest;
} TPMT_TK_HASHCHECK;

/**
 * Table 92 - Definition of TPMS_ALG_PROPERTY Structure [OUT]
 */
/**
 *	@brief		TPMS_ALG_PROPERTY structure
 *	@details	TPMS_ALG_PROPERTY structure definition
 */
typedef struct _TPMS_ALG_PROPERTY {
	/// An algorithm identifier
	TPM_ALG_ID alg;
	/// The attributes of the algorithm
	TPMA_ALGORITHM algProperties;
} TPMS_ALG_PROPERTY;

/**
 * Table 93 - Definition of TPMS_TAGGED_PROPERTY Structure [OUT]
 */
/**
 *	@brief		TPMS_TAGGED_PROPERTY structure
 *	@details	TPMS_TAGGED_PROPERTY structure definition
 */
typedef struct _TPMS_TAGGED_PROPERTY {
	/// A property identifier
	TPM_PT property;
	/// The value of the property
	UINT32 value;
} TPMS_TAGGED_PROPERTY;

/**
 * Table 94 - Definition of TPMS_TAGGED_PCR_SELECT Structure [OUT]
 */
/**
 *	@brief		TPMS_TAGGED_PCR_SELECT structure
 *	@details	TPMS_TAGGED_PCR_SELECT structure definition
 */
typedef struct _TPMS_TAGGED_PCR_SELECT {
	/// The property identifier
	TPM_PT tag;
	/// The size in octets of the pcrSelect array
	UINT8 sizeofSelect;
	/// The bit map of PCR with the identified property
	BYTE pcrSelect[PCR_SELECT_MAX];
} TPMS_TAGGED_PCR_SELECT;

/**
 * Table 95 - Definition of TPML_CC Structure
 */
/**
 *	@brief		TPML_CC structure
 *	@details	TPML_CC structure definition
 */
typedef struct _TPML_CC {
	/// Number of commands in the commandCode list; may be 0
	UINT32 count;
	/// A list of command codes
	/// The maximum only applies to a command code list in a command. The response size is limited only by the size of the parameter buffer.
	TPM_CC commandCodes[MAX_CAP_CC];
} TPML_CC;

/**
 * Table 96 - Definition of TPML_CCA Structure [OUT]
 */
/**
 *	@brief		TPML_CCA structure
 *	@details	TPML_CCA structure definition
 */
typedef struct _TPML_CCA {
	/// Number of values in the commandAttributes list; may be 0
	UINT32 count;
	/// A list of command codes attributes
	TPMA_CC commandAttributes[MAX_CAP_CC];
} TPML_CCA;

/**
 * Table 97 - Definition of TPML_ALG Structure
 */
/**
 *	@brief		TPML_ALG structure
 *	@details	TPML_ALG structure definition
 */
typedef struct _TPML_ALG {
	/// Number of algorithms in the algorithms list; may be 0
	UINT32 count;
	/// A list of algorithm IDs
	/// The maximum only applies to an algorithm list in a command. The response size is limited only by the size of the parameter buffer.
	TPM_ALG_ID algorithms[MAX_ALG_LIST_SIZE];
} TPML_ALG;

/**
 * Table 98 - Definition of TPML_HANDLE Structure [OUT]
 */
/**
 *	@brief		TPML_HANDLE structure
 *	@details	TPML_HANDLE structure definition
 */
typedef struct _TPML_HANDLE {
	/// The number of handles in the list
	/// may have a value of 0
	UINT32 count;
	/// An array of handles
	TPM_HANDLE handle[MAX_CAP_HANDLES];
} TPML_HANDLE;

/**
 * Table 99 - Definition of TPML_DIGEST Structure
 */
/**
 *	@brief		TPML_DIGEST structure
 *	@details	TPML_DIGEST structure definition
 */
typedef struct _TPML_DIGEST {
	/// Number of digests in the list, minimum is two for TPM2_PolicyOR().
	UINT32 count;
	/// A list of digests
	/// For TPM2_PolicyOR(), all digests will have been computed using the digest of the policy session. For TPM2_PCR_Read(), each digest will be the size of the digest for the bank containing the PCR.
	TPM2B_DIGEST digests[8];
} TPML_DIGEST;

/**
 * Table 100 - Definition of TPML_DIGEST_VALUES Structure
 */
/**
 *	@brief		TPML_DIGEST_VALUES structure
 *	@details	TPML_DIGEST_VALUES structure definition
 */
typedef struct _TPML_DIGEST_VALUES {
	/// Number of digests in the list
	UINT32 count;
	/// A list of tagged digests
	TPMT_HA digests[HASH_COUNT];
} TPML_DIGEST_VALUES;

/**
 * Table 101 - Definition of TPM2B_DIGEST_VALUES Structure
 */
/**
 *	@brief		TPM2B_DIGEST_VALUES structure
 *	@details	TPM2B_DIGEST_VALUES structure definition
 */
typedef struct _TPM2B_DIGEST_VALUES {
	/// Size of the operand buffer
	UINT16 size;
	/// The operand
	BYTE buffer[sizeof(TPML_DIGEST_VALUES)];
} TPM2B_DIGEST_VALUES;

/**
 * Table 102 - Definition of TPML_PCR_SELECTION Structure
 */
/**
 *	@brief		TPML_PCR_SELECTION structure
 *	@details	TPML_PCR_SELECTION structure definition
 */
typedef struct _TPML_PCR_SELECTION {
	/// Number of selection structures
	/// A value of zero is allowed.
	UINT32 count;
	/// List of selections
	TPMS_PCR_SELECTION pcrSelections[HASH_COUNT];
} TPML_PCR_SELECTION;

/**
 * Table 103 - Definition of TPML_ALG_PROPERTY Structure [OUT]
 */
/**
 *	@brief		TPML_ALG_PROPERTY structure
 *	@details	TPML_ALG_PROPERTY structure definition
 */
typedef struct _TPML_ALG_PROPERTY {
	/// Number of algorithm properties structures
	/// A value of zero is allowed.
	UINT32 count;
	/// List of properties
	TPMS_ALG_PROPERTY algProperties[MAX_CAP_ALGS];
} TPML_ALG_PROPERTY;

/**
 * Table 104 - Definition of TPML_TAGGED_TPM_PROPERTY Structure [OUT]
 */
/**
 *	@brief		TPML_TAGGED_TPM_PROPERTY structure
 *	@details	TPML_TAGGED_TPM_PROPERTY structure definition
 */
typedef struct _TPML_TAGGED_TPM_PROPERTY {
	/// Number of properties
	/// A value of zero is allowed.
	UINT32 count;
	/// An array of tagged properties
	TPMS_TAGGED_PROPERTY tpmProperty[MAX_TPM_PROPERTIES];
} TPML_TAGGED_TPM_PROPERTY;

/**
 * Table 105 - Definition of TPML_TAGGED_PCR_PROPERTY Structure [OUT]
 */
/**
 *	@brief		TPML_TAGGED_PCR_PROPERTY structure
 *	@details	TPML_TAGGED_PCR_PROPERTY structure definition
 */
typedef struct _TPML_TAGGED_PCR_PROPERTY {
	/// Number of properties
	/// A value of zero is allowed.
	UINT32 count;
	/// A tagged PCR selection
	TPMS_TAGGED_PCR_SELECT pcrProperty[MAX_PCR_PROPERTIES];
} TPML_TAGGED_PCR_PROPERTY;

/**
 * Table 106 - Definition of {ECC} TPML_ECC_CURVE Structure [OUT]
 */
/**
 *	@brief		TPML_ECC_CURVE structure
 *	@details	TPML_ECC_CURVE structure definition
 */
typedef struct _TPML_ECC_CURVE {
	/// Number of curves
	/// A value of zero is allowed.
	UINT32 count;
	/// Array of ECC curve identifiers
	TPM_ECC_CURVE eccCurves[MAX_ECC_CURVES];
} TPML_ECC_CURVE;

/**
 * Table 107 - Definition of TPMU_CAPABILITIES Union [OUT]
 */
/**
 *	@brief		TPMU_CAPABILITIES union
 *	@details	TPMU_CAPABILITIES union definition
 */
typedef union _TPMU_CAPABILITIES {
	TPML_ALG_PROPERTY algorithms;
	TPML_HANDLE handles;
	TPML_CCA command;
	TPML_CC ppCommands;
	TPML_CC auditCommands;
	TPML_PCR_SELECTION assignedPCR;
	TPML_TAGGED_TPM_PROPERTY tpmProperties;
	TPML_TAGGED_PCR_PROPERTY pcrProperties;
	TPML_ECC_CURVE eccCurves;
} TPMU_CAPABILITIES;

/**
 * Table 108 - Definition of TPMS_CAPABILITY_DATA Structure [OUT]
 */
/**
 *	@brief		TPMS_CAPABILITY_DATA structure
 *	@details	TPMS_CAPABILITY_DATA structure definition
 */
typedef struct _TPMS_CAPABILITY_DATA {
	/// The capability
	TPM_CAP capability;
	/// The capability data
	TPMU_CAPABILITIES data;
} TPMS_CAPABILITY_DATA;

/**
 * Table 109 - Definition of TPMS_CLOCK_INFO Structure
 */
/**
 *	@brief		TPMS_CLOCK_INFO structure
 *	@details	TPMS_CLOCK_INFO structure definition
 */
typedef struct _TPMS_CLOCK_INFO {
	/// Time in milliseconds during which the TPM has been powered
	/// This structure element is used to report on the TPM's Clock value.
	/// The value of Clock shall be recorded in non-volatile memory no less often than once per 222 milliseconds (~69.9 minutes) of TPM operation. The reference for the millisecond timer is the TPM oscillator.
	/// This value is reset to zero when the Storage Primary Seed is changed (TPM2_Clear()).
	/// This value may be advanced by TPM2_AdvanceClock().
	UINT64 clock;
	/// Number of occurrences of TPM Reset since the last TPM2_Clear()
	UINT32 resetCount;
	/// Number of times that TPM2_Shutdown() or _TPM_Hash_Start have occurred since the last TPM Reset or TPM2_Clear().
	UINT32 restartCount;
	/// No value of Clock greater than the current value of Clock has been previously reported by the TPM. Set to YES on TPM2_Clear().
	TPMI_YES_NO safe;
} TPMS_CLOCK_INFO;

/**
 * Table 110 - Definition of TPMS_TIME_INFO Structure
 */
/**
 *	@brief		TPMS_TIME_INFO structure
 *	@details	TPMS_TIME_INFO structure definition
 */
typedef struct _TPMS_TIME_INFO {
	/// Time in milliseconds since the last _TPM_Init() or TPM2_Startup()
	/// This structure element is used to report on the TPM's Time value.
	UINT64 time;
	/// A structure containing the clock information
	TPMS_CLOCK_INFO clockInfo;
} TPMS_TIME_INFO;

/**
 * Table 111 - Definition of TPMS_TIME_ATTEST_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_TIME_ATTEST_INFO structure
 *	@details	TPMS_TIME_ATTEST_INFO structure definition
 */
typedef struct _TPMS_TIME_ATTEST_INFO {
	/// The Time, Clock, resetCount, restartCount, and Safe indicator
	TPMS_TIME_INFO time;
	/// A TPM vendor-specific value indicating the version number of the firmware
	UINT64 firmwareVersion;
} TPMS_TIME_ATTEST_INFO;

/**
 * Table 112 - Definition of TPMS_CERTIFY_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_CERTIFY_INFO structure
 *	@details	TPMS_CERTIFY_INFO structure definition
 */
typedef struct _TPMS_CERTIFY_INFO {
	/// Name of the certified object
	TPM2B_NAME name;
	/// Qualified Name of the certified object
	TPM2B_NAME qualifiedName;
} TPMS_CERTIFY_INFO;

/**
 * Table 113 - Definition of TPMS_QUOTE_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_QUOTE_INFO structure
 *	@details	TPMS_QUOTE_INFO structure definition
 */
typedef struct _TPMS_QUOTE_INFO {
	/// Information on algID, PCR selected and digest
	TPML_PCR_SELECTION pcrSelect;
	/// Digest of the selected PCR using the hash of the signing key
	TPM2B_DIGEST pcrDigest;
} TPMS_QUOTE_INFO;

/**
 * Table 114 - Definition of TPMS_COMMAND_AUDIT_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_COMMAND_AUDIT_INFO structure
 *	@details	TPMS_COMMAND_AUDIT_INFO structure definition
 */
typedef struct _TPMS_COMMAND_AUDIT_INFO {
	/// The monotonic audit counter
	UINT64 auditCounter;
	/// Hash algorithm used for the command audit
	TPM_ALG_ID digestAlg;
	/// The current value of the audit digest
	TPM2B_DIGEST auditDigest;
	/// Digest of the command codes being audited using digestAlg
	TPM2B_DIGEST commandDigest;
} TPMS_COMMAND_AUDIT_INFO;

/**
 * Table 115 - Definition of TPMS_SESSION_AUDIT_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_SESSION_AUDIT_INFO structure
 *	@details	TPMS_SESSION_AUDIT_INFO structure definition
 */
typedef struct _TPMS_SESSION_AUDIT_INFO {
	/// Current exclusive status of the session
	/// TRUE if all of the commands recorded in the sessionDigest were executed without any intervening TPM command that did not use this audit session
	TPMI_YES_NO exclusiveSession;
	/// The current value of the session audit digest
	TPM2B_DIGEST sessionDigest;
} TPMS_SESSION_AUDIT_INFO;

/**
 * Table 116 - Definition of TPMS_CREATION_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_CREATION_INFO structure
 *	@details	TPMS_CREATION_INFO structure definition
 */
typedef struct _TPMS_CREATION_INFO {
	/// Name of the object
	TPM2B_NAME objectName;
	/// CreationHash
	TPM2B_DIGEST creationHash;
} TPMS_CREATION_INFO;

/**
 * Table 117 - Definition of TPMS_NV_CERTIFY_INFO Structure [OUT]
 */
/**
 *	@brief		TPMS_NV_CERTIFY_INFO structure
 *	@details	TPMS_NV_CERTIFY_INFO structure definition
 */
typedef struct _TPMS_NV_CERTIFY_INFO {
	/// Name of the NV Index
	TPM2B_NAME indexName;
	/// The offset parameter of TPM2_NV_Certify()
	UINT16 offset;
	/// Contents of the NV Index
	TPM2B_MAX_NV_BUFFER nvContents;
} TPMS_NV_CERTIFY_INFO;

/**
 * Table 118 - Definition of (TPM_ST) TPMI_ST_ATTEST Type [OUT]
 */
typedef TPM_ST TPMI_ST_ATTEST;

/**
 * Table 119 - Definition of TPMU_ATTEST Union [OUT]
 */
/**
 *	@brief		TPMU_ATTEST union
 *	@details	TPMU_ATTEST union definition
 */
typedef union _TPMU_ATTEST {
	TPMS_CERTIFY_INFO certify;
	TPMS_CREATION_INFO creation;
	TPMS_QUOTE_INFO quote;
	TPMS_COMMAND_AUDIT_INFO commandAudit;
	TPMS_SESSION_AUDIT_INFO sessionAudit;
	TPMS_TIME_ATTEST_INFO time;
	TPMS_NV_CERTIFY_INFO nv;
} TPMU_ATTEST;

/**
 * Table 120 - Definition of TPMS_ATTEST Structure [OUT]
 */
/**
 *	@brief		TPMS_ATTEST structure
 *	@details	TPMS_ATTEST structure definition
 */
typedef struct _TPMS_ATTEST {
	/// The indication that this structure was created by a TPM (always TPM_GENERATED_VALUE)
	TPM_GENERATED magic;
	/// Type of the attestation structure
	TPMI_ST_ATTEST type;
	/// Qualified Name of the signing key
	TPM2B_NAME qualifiedSigner;
	/// External information supplied by caller
	/// NOTE	A TPM2B_DATA structure provides room for a digest and a method indicator to indicate the components of the digest. The definition of this method indicator is outside the scope of this specification.
	TPM2B_DATA extraData;
	/// Clock, resetCount, restartCount, and Safe
	TPMS_CLOCK_INFO clockInfo;
	/// TPM-vendor-specific value identifying the version number of the firmware
	UINT64 firmwareVersion;
	/// The type-specific attestation information
	TPMU_ATTEST attested;
} TPMS_ATTEST;

/**
 * Table 121 - Definition of TPM2B_ATTEST Structure [OUT]
 */
/**
 *	@brief		TPM2B_ATTEST structure
 *	@details	TPM2B_ATTEST structure definition
 */
typedef struct _TPM2B_ATTEST {
	/// Size of the attestationData structure
	UINT16 size;
	/// The signed structure
	BYTE attestationData[sizeof(TPMS_ATTEST)];
} TPM2B_ATTEST;

/**
 * Table 122 - Definition of TPMS_AUTH_COMMAND Structure [IN]
 */
/**
 *	@brief		TPMS_AUTH_COMMAND structure
 *	@details	TPMS_AUTH_COMMAND structure definition
 */
typedef struct _TPMS_AUTH_COMMAND {
	/// The session handle
	TPMI_SH_AUTH_SESSION sessionHandle;
	/// The session nonce, may be the Empty Buffer
	TPM2B_NONCE nonce;
	/// The session attributes
	TPMA_SESSION sessionAttributes;
	/// Either an HMAC, a password, or an EmptyAuth
	TPM2B_AUTH hmac;
} TPMS_AUTH_COMMAND;

/**
 * Table 123 - Definition of TPMS_AUTH_RESPONSE Structure [OUT]
 */
/**
 *	@brief		TPMS_AUTH_RESPONSE structure
 *	@details	TPMS_AUTH_RESPONSE structure definition
 */
typedef struct _TPMS_AUTH_RESPONSE {
	/// The session nonce, may be the Empty Buffer
	TPM2B_NONCE nonce;
	/// The session attributes
	TPMA_SESSION sessionAttributes;
	/// Either an HMAC, a password, or an EmptyAuth
	TPM2B_AUTH hmac;
} TPMS_AUTH_RESPONSE;

/**
 * Table 125 - Definition of TPMU_SYM_KEY_BITS Union
 */
/**
 *	@brief		TPMU_SYM_KEY_BITS union
 *	@details	TPMU_SYM_KEY_BITS union definition
 */
typedef union _TPMU_SYM_KEY_BITS {
	TPMI_AES_KEY_BITS aes;
	TPMI_SM4_KEY_BITS sm4;
	TPMI_CAMELLIA_KEY_BITS camellia;
	TPM_KEY_BITS sym;
	TPMI_ALG_HASH xor;
} TPMU_SYM_KEY_BITS;

/**
 * Table 126 - Definition of TPMU_SYM_MODE Union
 */
/**
 *	@brief		TPMU_SYM_MODE union
 *	@details	TPMU_SYM_MODE union definition
 */
typedef union _TPMU_SYM_MODE {
	TPMI_ALG_SYM_MODE aes;
	TPMI_ALG_SYM_MODE sm4;
	TPMI_ALG_SYM_MODE camellia;
	TPMI_ALG_SYM_MODE sym;
} TPMU_SYM_MODE;

/**
 * Table 128 - Definition of TPMT_SYM_DEF Structure
 */
/**
 *	@brief		TPMT_SYM_DEF structure
 *	@details	TPMT_SYM_DEF structure definition
 */
typedef struct _TPMT_SYM_DEF {
	/// Indicates a symmetric algorithm
	TPMI_ALG_SYM algorithm;
	/// A supported key size
	TPMU_SYM_KEY_BITS keyBits;
	/// The mode for the key
	TPMU_SYM_MODE mode;
} TPMT_SYM_DEF;

/**
 * Table 129 - Definition of TPMT_SYM_DEF_OBJECT Structure
 */
/**
 *	@brief		TPMT_SYM_DEF_OBJECT structure
 *	@details	TPMT_SYM_DEF_OBJECT structure definition
 */
typedef struct _TPMT_SYM_DEF_OBJECT {
	/// Selects a symmetric block cipher
	TPMI_ALG_SYM_OBJECT algorithm;
	/// The key size
	TPMU_SYM_KEY_BITS keyBits;
	/// Default mode
	TPMU_SYM_MODE mode;
} TPMT_SYM_DEF_OBJECT;

/**
 * Table 130 - Definition of TPM2B_SYM_KEY Structure
 */
/**
 *	@brief		TPM2B_SYM_KEY structure
 *	@details	TPM2B_SYM_KEY structure definition
 */
typedef struct _TPM2B_SYM_KEY {
	/// Size, in octets, of the buffer containing the key; may be zero
	UINT16 size;
	/// The key
	BYTE buffer[MAX_SYM_KEY_BYTES];
} TPM2B_SYM_KEY;

/**
 * Table 131 - Definition of TPMS_SYMCIPHER_PARMS Structure
 */
/**
 *	@brief		TPMS_SYMCIPHER_PARMS structure
 *	@details	TPMS_SYMCIPHER_PARMS structure definition
 */
typedef struct _TPMS_SYMCIPHER_PARMS {
	/// A symmetric block cipher
	TPMT_SYM_DEF_OBJECT sym;
} TPMS_SYMCIPHER_PARMS;

/**
 * Table 132 - Definition of TPM2B_SENSITIVE_DATA Structure
 */
/**
 *	@brief		TPM2B_SENSITIVE_DATA structure
 *	@details	TPM2B_SENSITIVE_DATA structure definition
 */
typedef struct _TPM2B_SENSITIVE_DATA {
	///
	UINT16 size;
	/// The keyed hash private data structure
	BYTE buffer[MAX_SYM_DATA];
} TPM2B_SENSITIVE_DATA;

/**
 * Table 133 - Definition of TPMS_SENSITIVE_CREATE Structure [IN]
 */
/**
 *	@brief		TPMS_SENSITIVE_CREATE structure
 *	@details	TPMS_SENSITIVE_CREATE structure definition
 */
typedef struct _TPMS_SENSITIVE_CREATE {
	/// The USER auth secret value
	TPM2B_AUTH userAuth;
	/// Data to be sealed
	TPM2B_SENSITIVE_DATA data;
} TPMS_SENSITIVE_CREATE;

/**
 * Table 134 - Definition of TPM2B_SENSITIVE_CREATE Structure [IN, S]
 */
/**
 *	@brief		TPM2B_SENSITIVE_CREATE structure
 *	@details	TPM2B_SENSITIVE_CREATE structure definition
 */
typedef struct _TPM2B_SENSITIVE_CREATE {
	/// Size of sensitive in octets (may not be zero)
	/// NOTE	The userAuth and data parameters in this buffer may both be zero length but the minimum size of this parameter will be the sum of the size fields of the two parameters of the TPMS_SENSITIVE_CREATE.
	UINT16 size;
	/// Data to be sealed or a symmetric key value.
	TPMS_SENSITIVE_CREATE sensitive;
} TPM2B_SENSITIVE_CREATE;

/**
 * Table 135 - Definition of TPMS_SCHEME_HASH Structure
 */
/**
 *	@brief		TPMS_SCHEME_HASH structure
 *	@details	TPMS_SCHEME_HASH structure definition
 */
typedef struct _TPMS_SCHEME_HASH {
	/// The hash algorithm used to digest the message
	TPMI_ALG_HASH hashAlg;
} TPMS_SCHEME_HASH;

/**
 * Table 136 - Definition of {ECC} TPMS_SCHEME_ECDAA Structure
 */
/**
 *	@brief		TPMS_SCHEME_ECDAA structure
 *	@details	TPMS_SCHEME_ECDAA structure definition
 */
typedef struct _TPMS_SCHEME_ECDAA {
	/// The hash algorithm used to digest the message
	TPMI_ALG_HASH hashAlg;
	/// The counter value that is used between TPM2_Commit() and the sign operation
	UINT16 count;
} TPMS_SCHEME_ECDAA;

/**
 * Table 137 - Definition of (TPM_ALG_ID) TPMI_ALG_KEYEDHASH_SCHEME Type
 */
typedef TPM_ALG_ID TPMI_ALG_KEYEDHASH_SCHEME;

/**
 * Table 138 - Definition of Types for HMAC_SIG_SCHEME
 */
typedef TPMS_SCHEME_HASH TPMS_SCHEME_HMAC;
/**
 * Table 139 - Definition of TPMS_SCHEME_XOR Structure
 */
/**
 *	@brief		TPMS_SCHEME_XOR structure
 *	@details	TPMS_SCHEME_XOR structure definition
 */
typedef struct _TPMS_SCHEME_XOR {
	/// The hash algorithm used to digest the message
	TPMI_ALG_HASH hashAlg;
	/// The key derivation function
	TPMI_ALG_KDF kdf;
} TPMS_SCHEME_XOR;

/**
 * Table 140 - Definition of TPMU_SCHEME_KEYEDHASH Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_SCHEME_KEYEDHASH union
 *	@details	TPMU_SCHEME_KEYEDHASH union definition
 */
typedef union _TPMU_SCHEME_KEYEDHASH {
	TPMS_SCHEME_HMAC hmac;
	TPMS_SCHEME_XOR xor;
} TPMU_SCHEME_KEYEDHASH;

/**
 * Table 141 - Definition of TPMT_KEYEDHASH_SCHEME Structure
 */
/**
 *	@brief		TPMT_KEYEDHASH_SCHEME structure
 *	@details	TPMT_KEYEDHASH_SCHEME structure definition
 */
typedef struct _TPMT_KEYEDHASH_SCHEME {
	/// Selects the scheme
	TPMI_ALG_KEYEDHASH_SCHEME scheme;
	/// The scheme parameters
	TPMU_SCHEME_KEYEDHASH details;
} TPMT_KEYEDHASH_SCHEME;

/**
 * Table 142 - Definition of {RSA} Types for RSA Signature Schemes
 */
typedef TPMS_SCHEME_HASH TPMS_SIG_SCHEME_RSASSA;
typedef TPMS_SCHEME_HASH TPMS_SIG_SCHEME_RSAPSS;
/**
 * Table 143 - Definition of {ECC} Types for ECC Signature Schemes
 */
typedef TPMS_SCHEME_HASH TPMS_SIG_SCHEME_ECDSA;
typedef TPMS_SCHEME_HASH TPMS_SIG_SCHEME_SM2;
typedef TPMS_SCHEME_HASH TPMS_SIG_SCHEME_ECSCHNORR;
typedef TPMS_SCHEME_ECDAA TPMS_SIG_SCHEME_ECDAA;
/**
 * Table 144 - Definition of TPMU_SIG_SCHEME Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_SIG_SCHEME union
 *	@details	TPMU_SIG_SCHEME union definition
 */
typedef union _TPMU_SIG_SCHEME {
	TPMS_SIG_SCHEME_RSASSA rsassa;
	TPMS_SIG_SCHEME_RSAPSS rsapss;
	TPMS_SIG_SCHEME_ECDSA ecdsa;
	TPMS_SIG_SCHEME_SM2 sm2;
	TPMS_SIG_SCHEME_ECSCHNORR ecschnorr;
	TPMS_SCHEME_HMAC hmac;
	TPMS_SCHEME_HASH any;
} TPMU_SIG_SCHEME;

/**
 * Table 145 - Definition of TPMT_SIG_SCHEME Structure
 */
/**
 *	@brief		TPMT_SIG_SCHEME structure
 *	@details	TPMT_SIG_SCHEME structure definition
 */
typedef struct _TPMT_SIG_SCHEME {
	/// Scheme selector
	TPMI_ALG_SIG_SCHEME scheme;
	/// Scheme parameters
	TPMU_SIG_SCHEME details;
} TPMT_SIG_SCHEME;

/**
 * Table 146 - Definition of Types for {RSA} Encryption Schemes
 */
typedef TPMS_SCHEME_HASH TPMS_ENC_SCHEME_OAEP;
typedef TPMS_EMPTY TPMS_ENC_SCHEME_RSAES;
/**
 * Table 147 - Definition of Types for {ECC} ECC Key Exchange
 */
typedef TPMS_SCHEME_HASH TPMS_KEY_SCHEME_ECDH;
typedef TPMS_SCHEME_HASH TPMS_KEY_SCHEME_ECMQV;
/**
 * Table 148 - Definition of Types for KDF Schemes
 */
typedef TPMS_SCHEME_HASH TPMS_SCHEME_MGF1;
typedef TPMS_SCHEME_HASH TPMS_SCHEME_KDF1_SP800_56A;
typedef TPMS_SCHEME_HASH TPMS_SCHEME_KDF2;
typedef TPMS_SCHEME_HASH TPMS_SCHEME_KDF1_SP800_108;
/**
 * Table 149 - Definition of TPMU_KDF_SCHEME Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_KDF_SCHEME union
 *	@details	TPMU_KDF_SCHEME union definition
 */
typedef union _TPMU_KDF_SCHEME {
	TPMS_SCHEME_MGF1 mgf1;
	TPMS_SCHEME_KDF1_SP800_56A kdf1_sp800_56a;
	TPMS_SCHEME_KDF2 kdf2;
	TPMS_SCHEME_KDF1_SP800_108 kdf1_sp800_108;
} TPMU_KDF_SCHEME;

/**
 * Table 150 - Definition of TPMT_KDF_SCHEME Structure
 */
/**
 *	@brief		TPMT_KDF_SCHEME structure
 *	@details	TPMT_KDF_SCHEME structure definition
 */
typedef struct _TPMT_KDF_SCHEME {
	/// Scheme selector
	TPMI_ALG_KDF scheme;
	/// Scheme parameters
	TPMU_KDF_SCHEME details;
} TPMT_KDF_SCHEME;

/**
 * Table 151 - Definition of (TPM_ALG_ID) TPMI_ALG_ASYM_SCHEME Type <>
 */
typedef TPM_ALG_ID TPMI_ALG_ASYM_SCHEME;

/**
 * Table 152 - Definition of TPMU_ASYM_SCHEME Union
 */
/**
 *	@brief		TPMU_ASYM_SCHEME union
 *	@details	TPMU_ASYM_SCHEME union definition
 */
typedef union _TPMU_ASYM_SCHEME {
	TPMS_KEY_SCHEME_ECDH ecdh;
	TPMS_KEY_SCHEME_ECMQV ecmqv;
	TPMS_SIG_SCHEME_RSASSA rsassa;
	TPMS_SIG_SCHEME_RSAPSS rsapss;
	TPMS_SIG_SCHEME_ECDSA ecdsa;
	TPMS_SIG_SCHEME_SM2 sm2;
	TPMS_SIG_SCHEME_ECSCHNORR ecschnorr;
	TPMS_ENC_SCHEME_RSAES rsaes;
	TPMS_SCHEME_HASH anySig;
} TPMU_ASYM_SCHEME;

/**
 * Table 153 - Definition of TPMT_ASYM_SCHEME Structure <>
 */
/**
 *	@brief		TPMT_ASYM_SCHEME structure
 *	@details	TPMT_ASYM_SCHEME structure definition
 */
typedef struct _TPMT_ASYM_SCHEME {
	/// Scheme selector
	TPMI_ALG_ASYM_SCHEME scheme;
	/// Scheme parameters
	TPMU_ASYM_SCHEME details;
} TPMT_ASYM_SCHEME;

/**
 * Table 154 - Definition of (TPM_ALG_ID) {RSA} TPMI_ALG_RSA_SCHEME Type
 */
typedef TPM_ALG_ID TPMI_ALG_RSA_SCHEME;

/**
 * Table 155 - Definition of {RSA} TPMT_RSA_SCHEME Structure
 */
/**
 *	@brief		TPMT_RSA_SCHEME structure
 *	@details	TPMT_RSA_SCHEME structure definition
 */
typedef struct _TPMT_RSA_SCHEME {
	/// Scheme selector
	TPMI_ALG_RSA_SCHEME scheme;
	/// Scheme parameters
	TPMU_ASYM_SCHEME details;
} TPMT_RSA_SCHEME;

/**
 * Table 156 - Definition of (TPM_ALG_ID) {RSA} TPMI_ALG_RSA_DECRYPT Type
 */
typedef TPM_ALG_ID TPMI_ALG_RSA_DECRYPT;

/**
 * Table 157 - Definition of {RSA} TPMT_RSA_DECRYPT Structure
 */
/**
 *	@brief		TPMT_RSA_DECRYPT structure
 *	@details	TPMT_RSA_DECRYPT structure definition
 */
typedef struct _TPMT_RSA_DECRYPT {
	/// Scheme selector
	TPMI_ALG_RSA_DECRYPT scheme;
	/// Scheme parameters
	TPMU_ASYM_SCHEME details;
} TPMT_RSA_DECRYPT;

/**
 * Table 158 - Definition of {RSA} TPM2B_PUBLIC_KEY_RSA Structure
 */
/**
 *	@brief		TPM2B_PUBLIC_KEY_RSA structure
 *	@details	TPM2B_PUBLIC_KEY_RSA structure definition
 */
typedef struct _TPM2B_PUBLIC_KEY_RSA {
	/// Size of the buffer
	/// The value of zero is only valid for create.
	UINT16 size;
	/// Value
	BYTE buffer[MAX_RSA_KEY_BYTES];
} TPM2B_PUBLIC_KEY_RSA;

/**
 * Table 159 - Definition of {RSA} (TPM_KEY_BITS) TPMI_RSA_KEY_BITS Type
 */
typedef TPM_KEY_BITS TPMI_RSA_KEY_BITS;

/**
 * Table 160 - Definition of {RSA} TPM2B_PRIVATE_KEY_RSA Structure
 */
/**
 *	@brief		TPM2B_PRIVATE_KEY_RSA structure
 *	@details	TPM2B_PRIVATE_KEY_RSA structure definition
 */
typedef struct _TPM2B_PRIVATE_KEY_RSA {
	///
	UINT16 size;
	///
	BYTE buffer[MAX_RSA_KEY_BYTES / 2];
} TPM2B_PRIVATE_KEY_RSA;

/**
 * Table 161 - Definition of {ECC} TPM2B_ECC_PARAMETER Structure
 */
/**
 *	@brief		TPM2B_ECC_PARAMETER structure
 *	@details	TPM2B_ECC_PARAMETER structure definition
 */
typedef struct _TPM2B_ECC_PARAMETER {
	/// Size of buffer
	UINT16 size;
	/// The parameter data
	BYTE buffer[MAX_ECC_KEY_BYTES];
} TPM2B_ECC_PARAMETER;

/**
 * Table 162 - Definition of {ECC} TPMS_ECC_POINT Structure
 */
/**
 *	@brief		TPMS_ECC_POINT structure
 *	@details	TPMS_ECC_POINT structure definition
 */
typedef struct _TPMS_ECC_POINT {
	/// X coordinate
	TPM2B_ECC_PARAMETER x;
	/// Y coordinate
	TPM2B_ECC_PARAMETER y;
} TPMS_ECC_POINT;

/**
 * Table 163 - Definition of {ECC} TPM2B_ECC_POINT Structure
 */
/**
 *	@brief		TPM2B_ECC_POINT structure
 *	@details	TPM2B_ECC_POINT structure definition
 */
typedef struct _TPM2B_ECC_POINT {
	/// Size of the remainder of this structure
	UINT16 size;
	/// Coordinates
	TPMS_ECC_POINT point;
} TPM2B_ECC_POINT;

/**
 * Table 164 - Definition of (TPM_ALG_ID) {ECC} TPMI_ALG_ECC_SCHEME Type
 */
typedef TPM_ALG_ID TPMI_ALG_ECC_SCHEME;

/**
 * Table 165 - Definition of {ECC} (TPM_ECC_CURVE) TPMI_ECC_CURVE Type
 */
typedef TPM_ECC_CURVE TPMI_ECC_CURVE;

/**
 * Table 166 - Definition of (TPMT_SIG_SCHEME) {ECC} TPMT_ECC_SCHEME Structure
 */
/**
 *	@brief		TPMT_ECC_SCHEME structure
 *	@details	TPMT_ECC_SCHEME structure definition
 */
typedef struct _TPMT_ECC_SCHEME {
	/// Scheme selector
	TPMI_ALG_ECC_SCHEME scheme;
	/// Scheme parameters
	TPMU_ASYM_SCHEME details;
} TPMT_ECC_SCHEME;

/**
 * Table 167 - Definition of {ECC} TPMS_ALGORITHM_DETAIL_ECC Structure [OUT]
 */
/**
 *	@brief		TPMS_ALGORITHM_DETAIL_ECC structure
 *	@details	TPMS_ALGORITHM_DETAIL_ECC structure definition
 */
typedef struct _TPMS_ALGORITHM_DETAIL_ECC {
	/// Identifier for the curve
	TPM_ECC_CURVE curveID;
	/// Size in bits of the key
	UINT16 keySize;
	/// The default KDF and hash algorithm used in secret sharing operations
	TPMT_KDF_SCHEME kdf;
	/// If not TPM_ALG_NULL, this is the mandatory signature scheme that is required to be used with this curve.
	TPMT_ECC_SCHEME sign;
	/// Fp (the modulus)
	TPM2B_ECC_PARAMETER p;
	/// Coefficient of the linear term in the curve equation
	TPM2B_ECC_PARAMETER a;
	/// Constant term for curve equation
	TPM2B_ECC_PARAMETER b;
	/// X coordinate of base point G
	TPM2B_ECC_PARAMETER gX;
	/// Y coordinate of base point G
	TPM2B_ECC_PARAMETER gY;
	/// Order of G
	TPM2B_ECC_PARAMETER n;
	/// Cofactor (a size of zero indicates a cofactor of 1)
	TPM2B_ECC_PARAMETER h;
} TPMS_ALGORITHM_DETAIL_ECC;

/**
 * Table 168 - Definition of {RSA} TPMS_SIGNATURE_RSA Structure
 */
/**
 *	@brief		TPMS_SIGNATURE_RSA structure
 *	@details	TPMS_SIGNATURE_RSA structure definition
 */
typedef struct _TPMS_SIGNATURE_RSA {
	/// The hash algorithm used to digest the message
	/// TPM_ALG_NULL is not allowed.
	TPMI_ALG_HASH hash;
	/// The signature is the size of a public key.
	TPM2B_PUBLIC_KEY_RSA sig;
} TPMS_SIGNATURE_RSA;

/**
 * Table 169 - Definition of Types for {RSA} Signature
 */
typedef TPMS_SIGNATURE_RSA TPMS_SIGNATURE_RSASSA;
typedef TPMS_SIGNATURE_RSA TPMS_SIGNATURE_RSAPSS;
/**
 * Table 170 - Definition of {ECC} TPMS_SIGNATURE_ECC Structure
 */
/**
 *	@brief		TPMS_SIGNATURE_ECC structure
 *	@details	TPMS_SIGNATURE_ECC structure definition
 */
typedef struct _TPMS_SIGNATURE_ECC {
	/// The hash algorithm used in the signature process
	/// TPM_ALG_NULL is not allowed.
	TPMI_ALG_HASH hash;
	///
	TPM2B_ECC_PARAMETER signatureR;
	///
	TPM2B_ECC_PARAMETER signatureS;
} TPMS_SIGNATURE_ECC;

/**
 * Table 171 - Definition of Types for {ECC} TPMS_SIGNATUE_ECC
 */
typedef TPMS_SIGNATURE_ECC TPMS_SIGNATURE_ECDSA;
typedef TPMS_SIGNATURE_ECC TPMS_SIGNATURE_SM2;
typedef TPMS_SIGNATURE_ECC TPMS_SIGNATURE_ECSCHNORR;
/**
 * Table 172 - Definition of TPMU_SIGNATURE Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_SIGNATURE union
 *	@details	TPMU_SIGNATURE union definition
 */
typedef union _TPMU_SIGNATURE {
	TPMS_SIGNATURE_RSASSA rsassa;
	TPMS_SIGNATURE_RSAPSS rsapss;
	TPMS_SIGNATURE_ECDSA ecdsa;
	TPMS_SIGNATURE_SM2 sm2;
	TPMS_SIGNATURE_ECSCHNORR ecschnorr;
	TPMT_HA hmac;
	TPMS_SCHEME_HASH any;
} TPMU_SIGNATURE;

/**
 * Table 173 - Definition of TPMT_SIGNATURE Structure
 */
/**
 *	@brief		TPMT_SIGNATURE structure
 *	@details	TPMT_SIGNATURE structure definition
 */
typedef struct _TPMT_SIGNATURE {
	/// Selector of the algorithm used to construct the signature
	TPMI_ALG_SIG_SCHEME sigAlg;
	/// This shall be the actual signature information.
	TPMU_SIGNATURE signature;
} TPMT_SIGNATURE;

/**
 * Table 174 - Definition of TPMU_ENCRYPTED_SECRET Union [S]
 */
/**
 *	@brief		TPMU_ENCRYPTED_SECRET union
 *	@details	TPMU_ENCRYPTED_SECRET union definition
 */
typedef union _TPMU_ENCRYPTED_SECRET {
	BYTE ecc[sizeof(TPMS_ECC_POINT)];
	BYTE rsa[MAX_RSA_KEY_BYTES];
	BYTE symmetric[sizeof(TPM2B_DIGEST)];
	BYTE keyedHash[sizeof(TPM2B_DIGEST)];
} TPMU_ENCRYPTED_SECRET;

/**
 * Table 175 - Definition of TPM2B_ENCRYPTED_SECRET Structure
 */
/**
 *	@brief		TPM2B_ENCRYPTED_SECRET structure
 *	@details	TPM2B_ENCRYPTED_SECRET structure definition
 */
typedef struct _TPM2B_ENCRYPTED_SECRET {
	/// Size of the secret value
	UINT16 size;
	/// Secret
	BYTE secret[sizeof(TPMU_ENCRYPTED_SECRET)];
} TPM2B_ENCRYPTED_SECRET;

/**
 * Table 176 - Definition of (TPM_ALG_ID) TPMI_ALG_PUBLIC Type
 */
typedef TPM_ALG_ID TPMI_ALG_PUBLIC;

/**
 * Table 177 - Definition of TPMU_PUBLIC_ID Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_PUBLIC_ID union
 *	@details	TPMU_PUBLIC_ID union definition
 */
typedef union _TPMU_PUBLIC_ID {
	TPM2B_DIGEST keyedHash;
	TPM2B_DIGEST sym;
	TPM2B_PUBLIC_KEY_RSA rsa;
	TPMS_ECC_POINT ecc;
} TPMU_PUBLIC_ID;

/**
 * Table 178 - Definition of TPMS_KEYEDHASH_PARMS Structure
 */
/**
 *	@brief		TPMS_KEYEDHASH_PARMS structure
 *	@details	TPMS_KEYEDHASH_PARMS structure definition
 */
typedef struct _TPMS_KEYEDHASH_PARMS {
	/// Indicates the signing method used for a keyedHash signing object. This field also determines the size of the data field for a data object created with TPM2_Create(). This field shall not be set to TPM_ALG_NULL in a template if either sign or encrypt is SET.
	TPMT_KEYEDHASH_SCHEME scheme;
} TPMS_KEYEDHASH_PARMS;

/**
 * Table 179 - Definition of TPMS_ASYM_PARMS Structure <>
 */
/**
 *	@brief		TPMS_ASYM_PARMS structure
 *	@details	TPMS_ASYM_PARMS structure definition
 */
typedef struct _TPMS_ASYM_PARMS {
	/// The companion symmetric algorithm for a restricted decryption key and shall be set to a supported symmetric algorithm
	/// This field is optional for keys that are not decryption keys and shall be set to TPM_ALG_NULL if not used.
	TPMT_SYM_DEF_OBJECT symmetric;
	/// For a key with the sign attribute SET, a valid signing scheme for the key type
	/// for a key with the decrypt attribute SET, a valid key exchange protocol
	/// for a key with sign and decrypt attributes, shall be TPM_ALG_NULL
	TPMT_ASYM_SCHEME scheme;
} TPMS_ASYM_PARMS;

/**
 * Table 180 - Definition of {RSA} TPMS_RSA_PARMS Structure
 */
/**
 *	@brief		TPMS_RSA_PARMS structure
 *	@details	TPMS_RSA_PARMS structure definition
 */
typedef struct _TPMS_RSA_PARMS {
	/// For a restricted decryption key, shall be set to a supported symmetric algorithm, key size, and mode.
	/// if the key is not a restricted decryption key, this field shall be set to TPM_ALG_NULL.
	TPMT_SYM_DEF_OBJECT symmetric;
	/// For an unrestricted signing key, shall be either TPM_ALG_RSAPSS TPM_ALG_RSASSA or TPM_ALG_NULL
	/// for a restricted signing key, shall be either TPM_ALG_RSAPSS or TPM_ALG_RSASSA
	/// for an unrestricted decryption key, shall be TPM_ALG_RSAES, TPM_ALG_OAEP, or TPM_ALG_NULL unless the object also has the sign attribute
	/// for a restricted decryption key, this field shall be TPM_ALG_NULL
	/// NOTE	When both sign and decrypt are SET, restricted shall be CLEAR and scheme shall be TPM_ALG_NULL.
	TPMT_RSA_SCHEME scheme;
	/// Number of bits in the public modulus
	TPMI_RSA_KEY_BITS keyBits;
	/// The public exponent
	/// A prime number greater than 2.
	/// When zero, indicates that the exponent is the default of 216 + 1
	UINT32 exponent;
} TPMS_RSA_PARMS;

/**
 * Table 181 - Definition of {ECC} TPMS_ECC_PARMS Structure
 */
/**
 *	@brief		TPMS_ECC_PARMS structure
 *	@details	TPMS_ECC_PARMS structure definition
 */
typedef struct _TPMS_ECC_PARMS {
	/// For a restricted decryption key, shall be set to a supported symmetric algorithm, key size. and mode.
	/// if the key is not a restricted decryption key, this field shall be set to TPM_ALG_NULL.
	TPMT_SYM_DEF_OBJECT symmetric;
	/// If the sign attribute of the key is SET, then this shall be a valid signing scheme.
	/// NOTE	If the sign parameter in curveID indicates a mandatory scheme, then this field shall have the same value.
	/// If the decrypt attribute of the key is SET, then this shall be a valid key exchange scheme or TPM_ALG_NULL.
	/// If the key is a Storage Key, then this field shall be TPM_ALG_NULL.
	TPMT_ECC_SCHEME scheme;
	/// ECC curve ID
	TPMI_ECC_CURVE curveID;
	/// An optional key derivation scheme for generating a symmetric key from a Z value
	/// If the kdf parameter associated with curveID is not TPM_ALG_NULL then this is required to be NULL.
	/// NOTE	There are currently no commands where this parameter has effect and, in the reference code, this field needs to be set to TPM_ALG_NULL.
	TPMT_KDF_SCHEME kdf;
} TPMS_ECC_PARMS;

/**
 * Table 182 - Definition of TPMU_PUBLIC_PARMS Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_PUBLIC_PARMS union
 *	@details	TPMU_PUBLIC_PARMS union definition
 */
typedef union _TPMU_PUBLIC_PARMS {
	TPMS_KEYEDHASH_PARMS keyedHashDetail;
	TPMS_SYMCIPHER_PARMS symDetail;
	TPMS_RSA_PARMS rsaDetail;
	TPMS_ECC_PARMS eccDetail;
	TPMS_ASYM_PARMS asymDetail;
} TPMU_PUBLIC_PARMS;

/**
 * Table 183 - Definition of TPMT_PUBLIC_PARMS Structure
 */
/**
 *	@brief		TPMT_PUBLIC_PARMS structure
 *	@details	TPMT_PUBLIC_PARMS structure definition
 */
typedef struct _TPMT_PUBLIC_PARMS {
	/// The algorithm to be tested
	TPMI_ALG_PUBLIC type;
	/// The algorithm details
	TPMU_PUBLIC_PARMS parameters;
} TPMT_PUBLIC_PARMS;

/**
 * Table 184 - Definition of TPMT_PUBLIC Structure
 */
/**
 *	@brief		TPMT_PUBLIC structure
 *	@details	TPMT_PUBLIC structure definition
 */
typedef struct _TPMT_PUBLIC {
	/// "algorithm" associated with this object
	TPMI_ALG_PUBLIC type;
	/// Algorithm used for computing the Name of the object
	/// NOTE	The "+" indicates that the instance of a TPMT_PUBLIC may have a "+" to indicate that the nameAlg may be TPM_ALG_NULL.
	TPMI_ALG_HASH nameAlg;
	/// Attributes that, along with type, determine the manipulations of this object
	TPMA_OBJECT objectAttributes;
	/// Optional policy for using this key
	/// The policy is computed using the nameAlg of the object.
	/// NOTE Shall be the Empty Buffer if no authorization policy is present.
	TPM2B_DIGEST authPolicy;
	/// The algorithm or structure details
	TPMU_PUBLIC_PARMS parameters;
	/// The unique identifier of the structure
	/// For an asymmetric key, this would be the public key.
	TPMU_PUBLIC_ID unique;
} TPMT_PUBLIC;

/**
 * Table 185 - Definition of TPM2B_PUBLIC Structure
 */
/**
 *	@brief		TPM2B_PUBLIC structure
 *	@details	TPM2B_PUBLIC structure definition
 */
typedef struct _TPM2B_PUBLIC {
	/// Size of publicArea
	/// NOTE	The "=" will force the TPM to try to unmarshal a TPMT_PUBLIC and check that the unmarshaled size matches the value of size. If all the required fields of a TPMT_PUBLIC are not present, the TPM will return an error (generally TPM_RC_SIZE) when attempting to unmarshal the TPMT_PUBLIC.
	UINT16 size;
	/// The public area
	/// NOTE	The "+" indicates that the caller may specify that use of TPM_ALG_NULL is allowed for nameAlg.
	TPMT_PUBLIC publicArea;
} TPM2B_PUBLIC;

/**
 * Table 186 - Definition of TPM2B_PRIVATE_VENDOR_SPECIFIC Structure<>
 */
/**
 *	@brief		TPM2B_PRIVATE_VENDOR_SPECIFIC structure
 *	@details	TPM2B_PRIVATE_VENDOR_SPECIFIC structure definition
 */
typedef struct _TPM2B_PRIVATE_VENDOR_SPECIFIC {
	///
	UINT16 size;
	///
	BYTE buffer[PRIVATE_VENDOR_SPECIFIC_BYTES];
} TPM2B_PRIVATE_VENDOR_SPECIFIC;

/**
 * Table 187 - Definition of TPMU_SENSITIVE_COMPOSITE Union [IN/OUT, S]
 */
/**
 *	@brief		TPMU_SENSITIVE_COMPOSITE union
 *	@details	TPMU_SENSITIVE_COMPOSITE union definition
 */
typedef union _TPMU_SENSITIVE_COMPOSITE {
	TPM2B_PRIVATE_KEY_RSA rsa;
	TPM2B_ECC_PARAMETER ecc;
	TPM2B_SENSITIVE_DATA bits;
	TPM2B_SYM_KEY sym;
	TPM2B_PRIVATE_VENDOR_SPECIFIC any;
} TPMU_SENSITIVE_COMPOSITE;

/**
 * Table 188 - Definition of TPMT_SENSITIVE Structure
 */
/**
 *	@brief		TPMT_SENSITIVE structure
 *	@details	TPMT_SENSITIVE structure definition
 */
typedef struct _TPMT_SENSITIVE {
	/// Identifier for the sensitive area
	/// This shall be the same as the type parameter of the associated public area.
	TPMI_ALG_PUBLIC sensitiveType;
	/// User authorization data
	/// The authValue may be a zero-length string.
	/// This value shall not be larger than the size of the digest produced by the nameAlg of the object.
	TPM2B_AUTH authValue;
	/// For asymmetric key object, the optional protection seed; for other objects, the obfuscation value
	/// This value shall not be larger than the size of the digest produced by nameAlg of the object.
	TPM2B_DIGEST seedValue;
	/// The type-specific private data
	TPMU_SENSITIVE_COMPOSITE sensitive;
} TPMT_SENSITIVE;

/**
 * Table 189 - Definition of TPM2B_SENSITIVE Structure [IN/OUT]
 */
/**
 *	@brief		TPM2B_SENSITIVE structure
 *	@details	TPM2B_SENSITIVE structure definition
 */
typedef struct _TPM2B_SENSITIVE {
	/// Size of the private structure
	UINT16 size;
	/// An unencrypted sensitive area
	TPMT_SENSITIVE sensitiveArea;
} TPM2B_SENSITIVE;

/**
 * Table 190 - Definition of _PRIVATE Structure <>
 */
/**
 *	@brief		_PRIVATE structure
 *	@details	_PRIVATE structure definition
 */
typedef struct __PRIVATE {
	///
	TPM2B_DIGEST integrityOuter;
	/// Could also be a TPM2B_IV
	TPM2B_DIGEST integrityInner;
	/// The sensitive area
	TPMT_SENSITIVE sensitive;
} _PRIVATE;

/**
 * Table 191 - Definition of TPM2B_PRIVATE Structure [IN/OUT, S]
 */
/**
 *	@brief		TPM2B_PRIVATE structure
 *	@details	TPM2B_PRIVATE structure definition
 */
typedef struct _TPM2B_PRIVATE {
	/// Size of the private structure
	UINT16 size;
	/// An encrypted private area
	BYTE buffer[sizeof(_PRIVATE)];
} TPM2B_PRIVATE;

/**
 * Table 192 - Definition of _ID_OBJECT Structure <>
 */
/**
 *	@brief		_ID_OBJECT structure
 *	@details	_ID_OBJECT structure definition
 */
typedef struct __ID_OBJECT {
	/// HMAC using the nameAlg of the storage key on the target TPM
	TPM2B_DIGEST integrityHMAC;
	/// Credential protector information returned if name matches the referenced object
	/// All of the encIdentity is encrypted, including the size field.
	/// NOTE	The TPM is not required to check that the size is not larger than the digest of the nameAlg. However, if the size is larger, the ID object may not be usable on a TPM that has no digest larger than produced by nameAlg.
	TPM2B_DIGEST encIdentity;
} _ID_OBJECT;

/**
 * Table 193 - Definition of TPM2B_ID_OBJECT Structure [IN/OUT]
 */
/**
 *	@brief		TPM2B_ID_OBJECT structure
 *	@details	TPM2B_ID_OBJECT structure definition
 */
typedef struct _TPM2B_ID_OBJECT {
	/// Size of the credential structure
	UINT16 size;
	/// An encrypted credential area
	BYTE credential[sizeof(_ID_OBJECT)];
} TPM2B_ID_OBJECT;

/**
 * Table 194 - Definition of (UINT32) TPM_NV_INDEX Bits <>
 */
/**
 *	@brief		TPM_NV_INDEX structure
 *	@details	TPM_NV_INDEX structure definition
 */
typedef struct _TPM_NV_INDEX {
	unsigned int index							: 24;	/**
														* The Index of the NV location
														*/
	unsigned int RH_NV							: 8;	/**
														* constant value of TPM_HT_NV_INDEX indicating the NV Index range
														*/
} TPM_NV_INDEX;

/**
 * Table 195 - Definition of (UINT32) TPMA_NV Bits
 */
/**
 *	@brief		TPMA_NV structure
 *	@details	TPMA_NV structure definition
 */
typedef struct _TPMA_NV {
	unsigned int TPMA_NV_PPWRITE				: 1;	/**
														* SET (1): The Index data can be written if Platform Authorization is provided.
														* CLEAR (0): Writing of the Index data cannot be authorized with Platform Authorization.
														*/
	unsigned int TPMA_NV_OWNERWRITE				: 1;	/**
														* SET (1): The Index data can be written if Owner Authorization is provided.
														* CLEAR (0): Writing of the Index data cannot be authorized with Owner Authorization.
														*/
	unsigned int TPMA_NV_AUTHWRITE				: 1;	/**
														* SET (1): Authorizations to change the Index contents that require USER role may be provided with an HMAC session or password.
														* CLEAR (0): Authorizations to change the Index contents that require USER role may not be provided with an HMAC session or password.
														*/
	unsigned int TPMA_NV_POLICYWRITE			: 1;	/**
														* SET (1): Authorizations to change the Index contents that require USER role may be provided with a policy session.
														* CLEAR (0): Authorizations to change the Index contents that require USER role may not be provided with a policy session.
														* NOTE	TPM2_NV_ChangeAuth() always requires that authorization be provided in a policy session.
														*/
	unsigned int TPMA_NV_COUNTER				: 1;	/**
														* SET (1): Index contains an 8-octet value that is to be used as a counter and can only be modified with TPM2_NV_Increment().
														* CLEAR (0): The Index is not a counter.
														*/
	unsigned int TPMA_NV_BITS					: 1;	/**
														* SET (1): Index contains an 8-octet value to be used as a bit field and can only be modified with TPM2_NV_SetBits().
														* CLEAR (0): The Index is not a bit field.
														*/
	unsigned int TPMA_NV_EXTEND					: 1;	/**
														* SET (1): Index contains a digest-sized value used like a PCR. The Index may only be modified using TPM2_NV_Extend. The extend will use the nameAlg of the Index.
														* CLEAR (0): Index is not a PCR.
														*/
	unsigned int reserved8						: 3;	/**
														* shall be zero
														* reserved for use in defining additional write controls
														*/
	unsigned int TPMA_NV_POLICY_DELETE			: 1;	/**
														* SET (1): Index may not be deleted unless the authPolicy is satisfied using TPM2_NV_UndefineSpaceSpecial().
														* CLEAR (0): Index may be deleted with proper platform or owner authorization using TPM2_NV_UndefineSpace().
														*/
	unsigned int TPMA_NV_WRITELOCKED			: 1;	/**
														* SET (1): Index cannot be written.
														* CLEAR (0): Index can be written.
														*/
	unsigned int TPMA_NV_WRITEALL				: 1;	/**
														* SET (1): A partial write of the Index data is not allowed. The write size shall match the defined space size.
														* CLEAR (0): Partial writes are allowed. This setting is required if TPMA_NV_BITS is SET.
														*/
	unsigned int TPMA_NV_WRITEDEFINE			: 1;	/**
														* SET (1): TPM2_NV_WriteLock() may be used to prevent further writes to this location.
														* CLEAR (0): TPM2_NV_WriteLock() does not block subsequent writes if TPMA_NV_WRITE_STCLEAR is also CLEAR.
														*/
	unsigned int TPMA_NV_WRITE_STCLEAR			: 1;	/**
														* SET (1): TPM2_NV_WriteLock() may be used to prevent further writes to this location until the next TPM Reset or TPM Restart.
														* CLEAR (0): TPM2_NV_WriteLock() does not block subsequent writes if TPMA_NV_WRITEDEFINE is also CLEAR.
														*/
	unsigned int TPMA_NV_GLOBALLOCK				: 1;	/**
														* SET (1): If TPM2_NV_GlobalWriteLock() is successful, then further writes to this location are not permitted until the next TPM Reset or TPM Restart.
														* CLEAR (0): TPM2_NV_GlobalWriteLock() has no effect on the writing of the data at this Index.
														*/
	unsigned int TPMA_NV_PPREAD					: 1;	/**
														* SET (1): The Index data can be read if Platform Authorization is provided.
														* CLEAR (0): Reading of the Index data cannot be authorized with Platform Authorization.
														*/
	unsigned int TPMA_NV_OWNERREAD				: 1;	/**
														* SET (1): The Index data can be read if Owner Authorization is provided.
														* CLEAR (0): Reading of the Index data cannot be authorized with Owner Authorization.
														*/
	unsigned int TPMA_NV_AUTHREAD				: 1;	/**
														* SET (1): The Index data may be read if the authValue is provided.
														* CLEAR (0): Reading of the Index data cannot be authorized with the Index authValue.
														*/
	unsigned int TPMA_NV_POLICYREAD				: 1;	/**
														* SET (1): The Index data may be read if the authPolicy is satisfied.
														* CLEAR (0): Reading of the Index data cannot be authorized with the Index authPolicy.
														*/
	unsigned int reserved19						: 5;	/**
														* shall be zero
														* reserved for use in defining additional read controls
														*/
	unsigned int TPMA_NV_NO_DA					: 1;	/**
														* SET (1): Authorization failures of the Index do not affect the DA logic and authorization of the Index is not blocked when the TPM is in Lockout mode.
														* CLEAR (0): Authorization failures of the Index will increment the authorization failure counter and authorizations of this Index are not allowed when the TPM is in Lockout mode.
														*/
	unsigned int TPMA_NV_ORDERLY				: 1;	/**
														* SET (1): NV Index state is only required to be saved when the TPM performs an orderly shutdown (TPM2_Shutdown()).
														* CLEAR (0): NV Index state is required to be persistent after the command to update the Index completes successfully (that is, the NV update is synchronous with the update command).
														*/
	unsigned int TPMA_NV_CLEAR_STCLEAR			: 1;	/**
														* SET (1): TPMA_NV_WRITTEN for the Index is CLEAR by TPM Reset or TPM Restart.
														* CLEAR (0): TPMA_NV_WRITTEN is not changed by TPM Restart.
														* NOTE	This attribute may only be SET if TPMA_NV_COUNTER is not SET.
														* NOTE	If the TPMA_NV_ORDERLY is SET, TPMA_NV_WRITTEN will be CLEAR by TPM Reset.
														*/
	unsigned int TPMA_NV_READLOCKED				: 1;	/**
														* SET (1): Reads of the Index are blocked until the next TPM Reset or TPM Restart.
														* CLEAR (0): Reads of the Index are allowed if proper authorization is provided.
														*/
	unsigned int TPMA_NV_WRITTEN				: 1;	/**
														* SET (1): Index has been written.
														* CLEAR (0): Index has not been written.
														*/
	unsigned int TPMA_NV_PLATFORMCREATE			: 1;	/**
														* SET (1): This Index may be undefined with Platform Authorization but not with Owner Authorization.
														* CLEAR (0): This Index may be undefined using Owner Authorization but not with Platform Authorization.
														* The TPM will validate that this attribute is SET when the Index is defined using Platform Authorization and will validate that this attribute is CLEAR when the Index is defined using Owner Authorization.
														*/
	unsigned int TPMA_NV_READ_STCLEAR			: 1;	/**
														* SET (1): TPM2_NV_ReadLock() may be used to SET TPMA_NV_READLOCKED for this Index.
														* CLEAR (0): TPM2_NV_ReadLock() has no effect on this Index.
														*/
} TPMA_NV;

/**
 * Table 196 - Definition of TPMS_NV_PUBLIC Structure
 */
/**
 *	@brief		TPMS_NV_PUBLIC structure
 *	@details	TPMS_NV_PUBLIC structure definition
 */
typedef struct _TPMS_NV_PUBLIC {
	/// The handle of the data area
	TPMI_RH_NV_INDEX nvIndex;
	/// Hash algorithm used to compute the name of the Index and used for the authPolicy
	TPMI_ALG_HASH nameAlg;
	/// The Index attributes
	TPMA_NV attributes;
	/// The access policy for the Index
	TPM2B_DIGEST authPolicy;
	/// The size of the data area
	/// The maximum size is implementation-dependent. The minimum maximum size is platform-specific.
	UINT16 dataSize;
} TPMS_NV_PUBLIC;

/**
 * Table 197 - Definition of TPM2B_NV_PUBLIC Structure
 */
/**
 *	@brief		TPM2B_NV_PUBLIC structure
 *	@details	TPM2B_NV_PUBLIC structure definition
 */
typedef struct _TPM2B_NV_PUBLIC {
	/// Size of nvPublic
	UINT16 size;
	/// The public area
	TPMS_NV_PUBLIC nvPublic;
} TPM2B_NV_PUBLIC;

/**
 * Table 198 - Definition of TPM2B_CONTEXT_SENSITIVE Structure [IN/OUT]
 */
/**
 *	@brief		TPM2B_CONTEXT_SENSITIVE structure
 *	@details	TPM2B_CONTEXT_SENSITIVE structure definition
 */
typedef struct _TPM2B_CONTEXT_SENSITIVE {
	///
	UINT16 size;
	/// The sensitive data
	BYTE buffer[MAX_CONTEXT_SIZE];
} TPM2B_CONTEXT_SENSITIVE;

/**
 * Table 199 - Definition of TPMS_CONTEXT_DATA Structure [IN/OUT, S]
 */
/**
 *	@brief		TPMS_CONTEXT_DATA structure
 *	@details	TPMS_CONTEXT_DATA structure definition
 */
typedef struct _TPMS_CONTEXT_DATA {
	/// The integrity value
	TPM2B_DIGEST integrity;
	/// The sensitive area
	TPM2B_CONTEXT_SENSITIVE encrypted;
} TPMS_CONTEXT_DATA;

/**
 * Table 200 - Definition of TPM2B_CONTEXT_DATA Structure [IN/OUT]
 */
/**
 *	@brief		TPM2B_CONTEXT_DATA structure
 *	@details	TPM2B_CONTEXT_DATA structure definition
 */
typedef struct _TPM2B_CONTEXT_DATA {
	///
	UINT16 size;
	///
	BYTE buffer[sizeof(TPMS_CONTEXT_DATA)];
} TPM2B_CONTEXT_DATA;

/**
 * Table 201 - Definition of TPMS_CONTEXT Structure
 */
/**
 *	@brief		TPMS_CONTEXT structure
 *	@details	TPMS_CONTEXT structure definition
 */
typedef struct _TPMS_CONTEXT {
	/// The sequence number of the context
	/// NOTE	Transient object contexts and session contexts used different counters.
	UINT64 sequence;
	/// The handle of the session, object or sequence
	TPMI_DH_CONTEXT savedHandle;
	/// The hierarchy of the context
	TPMI_RH_HIERARCHY hierarchy;
	/// The context data and integrity HMAC
	TPM2B_CONTEXT_DATA contextBlob;
} TPMS_CONTEXT;

/**
 * Table 203 - Definition of TPMS_CREATION_DATA Structure [OUT]
 */
/**
 *	@brief		TPMS_CREATION_DATA structure
 *	@details	TPMS_CREATION_DATA structure definition
 */
typedef struct _TPMS_CREATION_DATA {
	/// List indicating the PCR included in pcrDigest
	TPML_PCR_SELECTION pcrSelect;
	/// Digest of the selected PCR using nameAlg of the object for which this structure is being created
	/// pcrDigest.size shall be zero if the pcrSelect list is empty.
	TPM2B_DIGEST pcrDigest;
	/// The locality at which the object was created
	TPMA_LOCALITY locality;
	/// NameAlg of the parent
	TPM_ALG_ID parentNameAlg;
	/// Name of the parent at time of creation
	/// The size will match digest size associated with parentNameAlg unless it is TPM_ALG_NULL, in which case the size will be 4 and parentName will be the hierarchy handle.
	TPM2B_NAME parentName;
	/// Qualified Name of the parent at the time of creation
	/// Size is the same as parentName.
	TPM2B_NAME parentQualifiedName;
	/// Association with additional information added by the key creator
	/// This will be the contents of the outsideInfo parameter in TPM2_Create() or TPM2_CreatePrimary().
	TPM2B_DATA outsideInfo;
} TPMS_CREATION_DATA;

/**
 * Table 204 - Definition of TPM2B_CREATION_DATA Structure [OUT]
 */
/**
 *	@brief		TPM2B_CREATION_DATA structure
 *	@details	TPM2B_CREATION_DATA structure definition
 */
typedef struct _TPM2B_CREATION_DATA {
	/// Size of the creation data
	UINT16 size;
	///
	TPMS_CREATION_DATA creationData;
} TPM2B_CREATION_DATA;

/**
 */
typedef TPMI_SH_AUTH_SESSION TPM_AUTH_HANDLE;

/**
 * Table 14 - Session-Based Authorization of Command
 */
/**
 *	@brief		AuthorizationCommandData structure
 *	@details	AuthorizationCommandData structure definition
 */
typedef struct _AuthorizationCommandData {
	/// The handle for the authorization session
	TPMI_SH_AUTH_SESSION authHandle;
	/// The caller-provided session nonce; size may be zero
	TPM2B_NONCE nonceCaller;
	/// The flags associated with the session
	TPMA_SESSION sessionAttributes;
	/// The session HMAC digest value
	TPM2B_AUTH hmac;
} AuthorizationCommandData;

/**
 * Table 15 - Session-Based Acknowledgment in Response
 */
/**
 *	@brief		AcknowledgmentResponseData structure
 *	@details	AcknowledgmentResponseData structure definition
 */
typedef struct _AcknowledgmentResponseData {
	/// The TPM-provided session nonce. Size is as specified when the session was started.
	TPM2B_NONCE nonceTPM;
	/// The flags associated with the session. This attribute should be the same as the values in the command except continueSession may be CLEAR.
	TPMA_SESSION sessionAttributes;
	/// The session HMAC digest value
	TPM2B_AUTH hmac;
} AcknowledgmentResponseData;