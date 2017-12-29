/**
 *	@brief		Implements the Cryptography interface for UEFI
 *	@details	This module provides cryptographic functions.
 *	@file		UEFI\Crypt.c
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

#include "Crypt.h"

#include <Uefi.h>
// IFX_CHANGE
#include <Library/BaseMemoryLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/MemoryAllocationLib.h>
/*
IFX_CHANGE
#undef assert // Use assert definition from OpenSSL framework instead of the defaults set in MicroTss headers.
#include <openssl/rsa.h>
#include <openssl/evp.h>
*/

/// OAEP Pad
static const BYTE g_rgbOAEPPad[] = { 'T', 'C', 'P', 'A' };

/**
 *	@brief		Calculate HMAC-SHA-1 on the given message
 *	@details	This function calculates a HMAC-SHA-1 on the input message.
 *
 *	@param		PrgbInputMessage		Input message
 *	@param		PusInputMessageSize		Input message size in bytes
 *	@param		PrgbKey					Message authentication key
 *	@param		PrgbHMAC				Receives the HMAC
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. PrgbInputMessage is NULL or PusInputMessageSize is 0
 */
_Check_return_
unsigned int
Crypt_HMAC(
	_In_bytecount_(PusInputMessageSize)		const BYTE*		PrgbInputMessage,
	_In_									UINT16			PusInputMessageSize,
	_In_opt_bytecount_(SHA1_DIGEST_SIZE)	const BYTE		PrgbKey[SHA1_DIGEST_SIZE],
	_Out_bytecap_(SHA1_DIGEST_SIZE)			BYTE			PrgbHMAC[SHA1_DIGEST_SIZE])
{
	unsigned int unReturnValue = RC_E_FAIL;
	VOID* pHmacCtx = NULL;

	do
	{
		UINTN ullCtxSize = 0;
		SetMem(PrgbHMAC, SHA1_DIGEST_SIZE, 0);

		// Check parameters
		if (NULL == PrgbInputMessage || 0 == PusInputMessageSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		// Calculate HMAC
		ullCtxSize = HmacSha1GetContextSize();
		pHmacCtx = AllocatePool(ullCtxSize);
		if (NULL == pHmacCtx)
			break;
		if (!HmacSha1Init(pHmacCtx, PrgbKey, SHA1_DIGEST_SIZE))
			break;
		if (!HmacSha1Update(pHmacCtx, PrgbInputMessage, PusInputMessageSize))
			break;
		if (!HmacSha1Final(pHmacCtx, PrgbHMAC))
			break;
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	if (NULL != pHmacCtx)
		FreePool(pHmacCtx);

	return unReturnValue;
}

/**
 *	@brief		Calculate SHA-1 on the given data
 *	@details	This function calculates a SHA-1 hash on the given data stream.
 *
 *	@param		PrgbInputMessage		Input message
 *	@param		PusInputMessageSize		Input message size in bytes
 *	@param		PrgbSHA1				Receives the SHA-1
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. PrgbInputMessage is NULL or PusInputMessageSize is 0
 */
_Check_return_
unsigned int
Crypt_SHA1(
	_In_bytecount_(PusInputMessageSize)	const BYTE*		PrgbInputMessage,
	_In_								const UINT16	PusInputMessageSize,
	_Out_bytecap_(SHA1_DIGEST_SIZE)		BYTE			PrgbSHA1[SHA1_DIGEST_SIZE])
{
	unsigned int unReturnValue = RC_E_FAIL;
	VOID* pSha1Ctx = NULL;

	do
	{
		UINTN ullCtxSize = 0;
		SetMem(PrgbSHA1, SHA1_DIGEST_SIZE, 0);

		// Check parameters
		if (NULL == PrgbInputMessage || 0 == PusInputMessageSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		// Calculate SHA-1
		ullCtxSize = Sha1GetContextSize();
		pSha1Ctx = AllocatePool(ullCtxSize);
		if (NULL == pSha1Ctx)
			break;
		if (!Sha1Init(pSha1Ctx))
			break;
		if (!Sha1Update(pSha1Ctx, PrgbInputMessage, PusInputMessageSize))
			break;
		if (!Sha1Final(pSha1Ctx, PrgbSHA1))
			break;
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	if (NULL != pSha1Ctx)
		FreePool(pSha1Ctx);

	return unReturnValue;
}

/**
 *	@brief		Calculate SHA-256 on the given data
 *	@details	This function calculates a SHA-256 hash on the given data stream.
 *
 *	@param		PrgbInputMessage		Input message
 *	@param		PunInputMessageSize		Input message size in bytes
 *	@param		PrgbSHA256				Receives the SHA-256
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. PrgbInputMessage is NULL or PunInputMessageSize is 0
 */
_Check_return_
unsigned int
Crypt_SHA256(
	_In_bytecount_(PunInputMessageSize)	const BYTE*		PrgbInputMessage,
	_In_								const UINT32	PunInputMessageSize,
	_Out_bytecap_(SHA256_DIGEST_SIZE)	BYTE			PrgbSHA256[SHA256_DIGEST_SIZE])
{
	unsigned int unReturnValue = RC_E_FAIL;
	VOID* pSha256Ctx = NULL;

	do
	{
		UINTN ullCtxSize = 0;
		SetMem(PrgbSHA256, SHA1_DIGEST_SIZE, 0);

		// Check parameters
		if (NULL == PrgbInputMessage || 0 == PunInputMessageSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		// Calculate SHA-256
		ullCtxSize = Sha256GetContextSize();
		pSha256Ctx = AllocatePool(ullCtxSize);
		if (NULL == pSha256Ctx)
			break;
		if (!Sha256Init(pSha256Ctx))
			break;
		if (!Sha256Update(pSha256Ctx, PrgbInputMessage, PunInputMessageSize))
			break;
		if (!Sha256Final(pSha256Ctx, PrgbSHA256))
			break;
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	if (NULL != pSha256Ctx)
		FreePool(pSha256Ctx);

	return unReturnValue;
}

/**
 *	@brief		Seed the pseudo random number generator
 *	@details	This function seeds the pseudo random number generator.
 *
 *	@param		PrgbSeed				Seed
 *	@param		PusSeedSize				Seed size in bytes
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. PrgbSeed is NULL and PusSeedSize is not 0.
 */
_Check_return_
unsigned int
Crypt_SeedRandom(
	_In_bytecount_(PusSeedSize)	const BYTE*		PrgbSeed,
	_In_						const UINT16	PusSeedSize)
{
	unsigned int unReturnValue = RC_E_FAIL;
	do
	{
		if (NULL == PrgbSeed && 0 != PusSeedSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		if (!RandomSeed(PrgbSeed, PusSeedSize))
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;
	return unReturnValue;
}

/**
 *	@brief		Get random bytes from the pseudo random number generator
 *	@details	This function gets random bytes from the pseudo random number generator.
 *
 *	@param		PusRandomSize			Number of bytes requested.
 *	@param		PrgbRandom				Receives pseudo random bytes.
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. PrgbRandom is NULL or PusRandomSize is 0
 */
_Check_return_
unsigned int
Crypt_GetRandom(
	_In_							const UINT16	PusRandomSize,
	_Out_bytecap_(PusRandomSize)	BYTE*			PrgbRandom)
{
	unsigned int unReturnValue = RC_E_FAIL;
	do
	{
		// Check input parameters
		if (NULL == PrgbRandom || 0 == PusRandomSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		if (!RandomBytes(PrgbRandom, PusRandomSize))
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;
	return unReturnValue;
}

// IFX_CHANGE [BEGIN]
#if 0
/**
 *	@brief		Encrypt a byte array with a RSA 2048-bit public key
 *	@details	This function encrypts the given data stream with RSA 2048-bit.
 *
 *	@param		PusEncryptionScheme				Encryption scheme. Only CRYPT_ES_RSAESOAEP_SHA1_MGF1 is supported.
 *	@param		PunInputDataSize				Size of input data in bytes
 *	@param		PrgbInputData					Input data buffer
 *	@param		PunPublicModulusSize			Size of public modulus in bytes
 *	@param		PrgbPublicModulus				Public modulus buffer
 *	@param		PunPublicExponentSize			Size of public exponent in bytes
 *	@param		PrgbPublicExponent				Public exponent buffer
 *	@param		PpunEncryptedDataSize			In: Size of buffer for encrypted data in bytes
 *												Out: Size of encrypted data in bytes
 *	@param		PrgbEncryptedData				Encrypted data buffer
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred during RSA functionality.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. It was NULL or empty.
 *	@retval		RC_E_BUFFER_TOO_SMALL	In case of PrgbEncryptedData is too small
 *	@retval		RC_E_INTERNAL			In case of a not supported padding schema
 */
_Check_return_
unsigned int
Crypt_EncryptRSA(
	_In_										CRYPT_ENC_SCHEME	PusEncryptionScheme,
	_In_										unsigned int		PunInputDataSize,
	_In_bytecount_(PunInputDataSize)			const BYTE*			PrgbInputData,
	_In_										unsigned int		PunPublicModulusSize,
	_In_bytecount_(PunPublicModulusSize)		const BYTE*			PrgbPublicModulus,
	_In_										unsigned int		PunPublicExponentSize,
	_In_bytecount_(PunPublicExponentSize)		const BYTE*			PrgbPublicExponent,
	_Inout_										unsigned int*		PpunEncryptedDataSize,
	_Inout_bytecap_(*PpunEncryptedDataSize)		BYTE*				PrgbEncryptedData)
{
	unsigned int unReturnValue = RC_E_FAIL;
	RSA *pRSAPubKey = NULL;
	BIGNUM* pbnPublicModulus = NULL;
	BIGNUM* pbnExponent = NULL;

	do
	{
		BYTE rgbPaddedBuffer[MAX_RSA_KEY_BYTES] = {0};
		unsigned int unPaddedBufferSize = RG_LEN(rgbPaddedBuffer);

		// Check input parameters
		if (NULL == PrgbInputData || 0 == PunInputDataSize ||
				NULL == PrgbPublicModulus || 0 == PunPublicModulusSize ||
				NULL == PrgbPublicExponent || 0 == PunPublicExponentSize ||
				NULL == PrgbEncryptedData || NULL == PpunEncryptedDataSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		if (*PpunEncryptedDataSize < unPaddedBufferSize)
		{
			unReturnValue = RC_E_BUFFER_TOO_SMALL;
			break;
		}

		// Initialize RSA Public Key object
		pRSAPubKey = RSA_new();
		if (NULL == pRSAPubKey)
		{
			unReturnValue = RC_E_FAIL;
			break;
		}

		pbnPublicModulus = BN_bin2bn((const BYTE*)PrgbPublicModulus, PunPublicModulusSize, pbnPublicModulus);
		if (NULL == pbnPublicModulus)
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		pRSAPubKey->n = pbnPublicModulus;

		pbnExponent = BN_bin2bn((const BYTE*)PrgbPublicExponent, PunPublicExponentSize, pbnExponent);
		if (NULL == pbnExponent)
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		pRSAPubKey->e = pbnExponent;
		pRSAPubKey->d = NULL;

		// Add padding to the decrypted data
		if (CRYPT_ES_RSAESOAEP_SHA1_MGF1 == PusEncryptionScheme)
		{
			int nReturnValue = 0;

			nReturnValue = RSA_padding_add_PKCS1_OAEP(
							   (BYTE*)rgbPaddedBuffer,
							   unPaddedBufferSize,
							   (const BYTE*)PrgbInputData,
							   PunInputDataSize,
							   g_rgbOAEPPad,
							   sizeof(g_rgbOAEPPad));

			if (0 == nReturnValue)
			{
				unReturnValue = RC_E_FAIL;
				break;
			}
		}
		else
		{
			unReturnValue = RC_E_INTERNAL;
			break;
		}

		// Encrypt data with public key.
		{
			int nReturnValue = RSA_public_encrypt(
								   unPaddedBufferSize,
								   rgbPaddedBuffer,
								   PrgbEncryptedData,
								   pRSAPubKey,
								   RSA_NO_PADDING);
			if (0 > nReturnValue)
			{
				unReturnValue = RC_E_FAIL;
				break;
			}
			*PpunEncryptedDataSize = nReturnValue;
		}
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	// Free RSA object and its components (BIGNUM)
	if (NULL != pRSAPubKey)
		RSA_free(pRSAPubKey);

	return unReturnValue;
}

/**
 *	@brief		Verify the given RSA PKCS#1 RSASSA-PSS signature
 *	@details	This function verifies the given RSA PKCS#1 RSASSA-PSS signature with a RSA 2048-bit public key.
 *
 *	@param		PrgbMessageHash			Message hash buffer
 *	@param		PunMessageHashSize		Size of message hash buffer
 *	@param		PrgbSignature			Signature buffer
 *	@param		PunSignatureSize		Size of the signature buffer
 *	@param		PrgbModulus				Public modulus buffer
 *	@param		PunModulusSize			Size of public modulus buffer
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_FAIL				An unexpected error occurred during RSA functionality.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function. An input parameter is NULL or empty
 *	@retval		RC_E_VERIFY_SIGNATURE	In case the signature is invalid
 */
_Check_return_
unsigned int
Crypt_VerifySignature(
	_In_bytecount_(PunMessageHashSize)	const BYTE*		PrgbMessageHash,
	_In_								const UINT32	PunMessageHashSize,
	_In_bytecount_(PunSignatureSize)	const BYTE*		PrgbSignature,
	_In_								const UINT32	PunSignatureSize,
	_In_bytecount_(PunModulusSize)		const BYTE*		PrgbModulus,
	_In_								const UINT32	PunModulusSize)
{
	unsigned int unReturnValue = RC_E_FAIL;
	RSA* pRSAPubKey = NULL;
	BIGNUM* pbnModulus = NULL;
	BIGNUM* pbnExponent = NULL;

	do
	{
		// Check input parameters
		if (NULL == PrgbMessageHash || 0 == PunMessageHashSize ||
				NULL == PrgbSignature || 0 == PunSignatureSize ||
				NULL == PrgbModulus || 0 == PunModulusSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		// Initialize RSA Public Key object
		pRSAPubKey = RSA_new();
		if (NULL == pRSAPubKey)
		{
			unReturnValue = RC_E_FAIL;
			break;
		}

		pbnModulus = BN_bin2bn(PrgbModulus, PunModulusSize, pbnModulus);
		if (NULL == pbnModulus)
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		pRSAPubKey->n = pbnModulus;

		pbnExponent = BN_bin2bn(RSA_PUB_EXPONENT_KEY_ID_0, sizeof(RSA_PUB_EXPONENT_KEY_ID_0), pbnExponent);
		if (NULL == pbnExponent)
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		pRSAPubKey->e = pbnExponent;
		pRSAPubKey->d = NULL;

		{
			BYTE prgbDecryptedDigest[sizeof(RSA_PUB_MODULUS_KEY_ID_0)] = {0};
			int nReturnValue = -1;
			nReturnValue = RSA_public_decrypt(PunSignatureSize, PrgbSignature, prgbDecryptedDigest, pRSAPubKey, RSA_NO_PADDING);
			if (-1 == nReturnValue)
			{
				unReturnValue = RC_E_FAIL;
				break;
			}

			// Verify the signature
			if (1 != RSA_verify_PKCS1_PSS(pRSAPubKey, PrgbMessageHash, EVP_sha256(), prgbDecryptedDigest, CRYPT_PSS_PADDING_SALT_SIZE))
			{
				unReturnValue = RC_E_VERIFY_SIGNATURE;
				break;
			}
		}

		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	// Free RSA object and its components (BIGNUM)
	if (NULL != pRSAPubKey)
		RSA_free(pRSAPubKey);

	return unReturnValue;
}
#endif
// IFX_CHANGE [END]

/**
 *	@brief		Calculate the CRC value of the given data stream
 *	@details	The function utilizes the UDK method for calculating a CRC32 value.
 *
 *	@param		PpInputData			Data stream for CRC calculation
 *	@param		PnInputDataSize		Size if data to calculate the CRC
 *	@param		PpunCRC				Calculated CRC value
 *
 *	@retval		RC_SUCCESS			The operation completed successfully.
 *	@retval		RC_E_FAIL			An unexpected error occurred during CRC calculation.
 *	@retval		RC_E_BAD_PARAMETER	An invalid parameter was passed to the function. It was NULL or empty.
 */
_Check_return_
unsigned int
Crypt_CRC(
	_In_bytecount_(PnInputDataSize)	const void*		PpInputData,
	_In_							int				PnInputDataSize,
	_Out_							unsigned int*	PpunCRC)
{
	unsigned int unReturnValue = RC_E_FAIL;

	do
	{
		EFI_STATUS efiStatus = EFI_UNSUPPORTED;

		// Check parameter
		if (NULL == PpInputData ||
				0 == PnInputDataSize ||
				NULL == PpunCRC)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		// Calculate CRC value
		efiStatus = gBS->CalculateCrc32((void*)PpInputData, PnInputDataSize, PpunCRC);
		if (EFI_SUCCESS == efiStatus)
			unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	return unReturnValue;
}
