/**
 *	@brief		Implements the TPM I/O interface
 *	@details	Implements the TPM I/O interface.
 *	@file		UEFI\TpmIO.c
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
#include "TpmIO.h"
#include "Logging.h"
#include "DeviceAccess.h"
#include "TPM_TIS.h"
#include "PropertyStorage.h"

/// Global flag to signalize if module is connected or disconnected
BOOL g_fConnected = 0;
/// Define for locality configuration setting property
#define PROPERTY_LOCALITY				L"Locality"

/**
 *	@brief		TPM connect function
 *	@details	This function handles the connect to the underlying TPM.
 *
 *	@retval		RC_SUCCESS					The operation completed successfully.
 *	@retval		RC_E_ALREADY_CONNECTED		If TPM I/O is already connected
 *	@retval		RC_E_COMPONENT_NOT_FOUND	No IFX TPM found
 *	@retval		...							Error codes from DeviceAccess_Initialize and TIS
 */
_Check_return_
unsigned int
TPMIO_Connect()
{
	unsigned int unReturnValue = RC_E_FAIL;
	BOOL bFlag = FALSE;
	unsigned int unLocality = 0;

	LOGGING_WRITE_LEVEL4(LOGGING_METHOD_ENTRY_STRING);

	do
	{
		// Check if already connected
		if (FALSE != g_fConnected)
		{
			unReturnValue = RC_E_ALREADY_CONNECTED;
			break;
		}

		// Try to connect to the TPM and check return code
		LOGGING_WRITE_LEVEL4(L"Connecting to TPM...");

		// Get the selected locality for TPM access
		if (FALSE == PropertyStorage_GetUIntegerValueByKey(PROPERTY_LOCALITY, &unLocality))
		{
			unReturnValue = RC_E_FAIL;
			break;
		}

		unReturnValue = DeviceAccess_Initialize((BYTE)unLocality);
		if (RC_SUCCESS != unReturnValue)
		{
			LOGGING_WRITE_LEVEL1_FMT(L"Error initializing LowLevelIO: 0x%.8X", unReturnValue);
			break;
		}

		LOGGING_WRITE_LEVEL4(L"Using memory access routines");
		LOGGING_WRITE_LEVEL4_FMT(L"Using Locality: %d", unLocality);

		// Check the presence of a TPM first
		// Check whether TPM.ACCESS.VALID
		unReturnValue = TIS_IsAccessValid((BYTE)unLocality, &bFlag);
		if (RC_SUCCESS != unReturnValue)
		{
			LOGGING_WRITE_LEVEL1_FMT(L"Error TIS access is not valid: 0x%.8X", unReturnValue);
			break;
		}

		if (!bFlag)
		{
			LOGGING_WRITE_LEVEL1_FMT(L"Error TIS is not ready: 0x%.8X", unReturnValue);
			unReturnValue = RC_E_NOT_READY;
			break;
		}

		LOGGING_WRITE_LEVEL4(L"Connected to TPM");

		g_fConnected = TRUE;
	}
	WHILE_FALSE_END;

	LOGGING_WRITE_LEVEL4_FMT(LOGGING_METHOD_EXIT_STRING_RET_VAL, unReturnValue);

	return unReturnValue;
}

/**
 *	@brief		TPM disconnect function
 *	@details	This function handles the disconnect to the underlying TPM.
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_NOT_CONNECTED		If TPM I/O is not connected to the TPM
 *	@retval		...						Error codes from DeviceAccess_Uninitialize function
 */
_Check_return_
unsigned int
TPMIO_Disconnect()
{
	unsigned int unReturnValue = RC_E_FAIL;
	unsigned int unLocality = 0;

	LOGGING_WRITE_LEVEL4(LOGGING_METHOD_ENTRY_STRING);

	do
	{
		// Check if connected to the TPM
		if (FALSE == g_fConnected)
		{
			unReturnValue = RC_E_NOT_CONNECTED;
			break;
		}

		// Try to disconnect the TPM and check return code
		LOGGING_WRITE_LEVEL4(L"Disconnecting from TPM...");

		// Get the selected locality for TPM access
		if (FALSE == PropertyStorage_GetUIntegerValueByKey(PROPERTY_LOCALITY, &unLocality))
		{
			unReturnValue = RC_E_FAIL;
			break;
		}
		unReturnValue = DeviceAccess_Uninitialize((BYTE)unLocality);
		if (RC_SUCCESS != unReturnValue)
			break;

		LOGGING_WRITE_LEVEL4(L"Disconnected from TPM");
		g_fConnected = FALSE;
	}
	WHILE_FALSE_END;

	LOGGING_WRITE_LEVEL4_FMT(LOGGING_METHOD_EXIT_STRING_RET_VAL, unReturnValue);

	return unReturnValue;
}

/**
 *	@brief		TPM transmit function
 *	@details	This function submits the TPM command to the underlying TPM.
 *
 *	@param		PrgbRequestBuffer		Pointer to a byte array containing the TPM command request bytes
 *	@param		PunRequestBufferSize	Size of command request in bytes
 *	@param		PrgbResponseBuffer		Pointer to a byte array receiving the TPM command response bytes
 *	@param		PpunResponseBufferSize	Input size of response buffer, output size of TPM command response in bytes
 *	@param		PunMaxDuration			The maximum duration of the command in microseconds (relevant for memory based access / TIS protocol only)
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function.
 *	@retval		RC_E_NOT_CONNECTED		If the TPM I/O is not connected to the TPM
 *	@retval		...						Error codes from TDDL_TransmitData function
 */
_Check_return_
unsigned int
TPMIO_Transmit(
	_In_bytecount_(PunRequestBufferSize)		const BYTE*		PrgbRequestBuffer,
	_In_										unsigned int	PunRequestBufferSize,
	_Out_bytecap_(*PpunResponseBufferSize)		BYTE*			PrgbResponseBuffer,
	_Inout_										unsigned int*	PpunResponseBufferSize,
	_In_										unsigned int	PunMaxDuration)
{
	unsigned int unReturnValue = RC_E_FAIL;
	unsigned int unLocality = 0;

	LOGGING_WRITE_LEVEL4(LOGGING_METHOD_ENTRY_STRING);

	do
	{
		// Check parameters
		if (NULL == PrgbRequestBuffer || NULL == PrgbResponseBuffer || NULL == PpunResponseBufferSize)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}
		// Check if connected to the TPM
		if (FALSE == g_fConnected)
		{
			unReturnValue = RC_E_NOT_CONNECTED;
			break;
		}

		// Get the selected locality for TPM access
		if (FALSE == PropertyStorage_GetUIntegerValueByKey(PROPERTY_LOCALITY, &unLocality))
		{
			unReturnValue = RC_E_FAIL;
			break;
		}

		unReturnValue = TIS_TransceiveLPC(
							(BYTE)unLocality,
							PrgbRequestBuffer,
							(UINT16)PunRequestBufferSize,
							PrgbResponseBuffer,
							(UINT16*)PpunResponseBufferSize,
							PunMaxDuration);

		if (RC_SUCCESS != unReturnValue)
		{
			LOGGING_WRITE_LEVEL1(L"Transmission of data via TIS failed !!!");
			break;
		}
	}
	WHILE_FALSE_END;

	LOGGING_WRITE_LEVEL4_FMT(LOGGING_METHOD_EXIT_STRING_RET_VAL, unReturnValue);

	return unReturnValue;
}

/**
 *	@brief		Read a byte from a specific address (register)
 *	@details	This function reads a byte from the specified address
 *
 *	@param		PunRegisterAddress		Register address
 *	@param		PpbRegisterValue		Pointer to a byte to store the register value
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		RC_E_BAD_PARAMETER		An invalid parameter was passed to the function.
 *	@retval		...						Error codes from called functions
 */
_Check_return_
unsigned int
TPMIO_ReadRegister(
	_In_		unsigned int		PunRegisterAddress,
	_Inout_		BYTE*				PpbRegisterValue)
{
	unsigned int unReturnValue = RC_E_FAIL;

	LOGGING_WRITE_LEVEL4(LOGGING_METHOD_ENTRY_STRING);

	do
	{
		// Check parameters
		if (NULL == PpbRegisterValue)
		{
			unReturnValue = RC_E_BAD_PARAMETER;
			break;
		}

		// Read byte from register address
		*PpbRegisterValue = DeviceAccess_ReadByte(PunRegisterAddress);
		unReturnValue = RC_SUCCESS;
	}
	WHILE_FALSE_END;

	LOGGING_WRITE_LEVEL4_FMT(LOGGING_METHOD_EXIT_STRING_RET_VAL, unReturnValue);

	return unReturnValue;
}

/**
 *	@brief		Write a byte to a specific address (register)
 *	@details	This function writes a byte to the specified address
 *
 *	@param		PunRegisterAddress		Register address
 *	@param		PbRegisterValue			Byte to write to the register address
 *
 *	@retval		RC_SUCCESS				The operation completed successfully.
 *	@retval		...						Error codes from called functions
 */
_Check_return_
unsigned int
TPMIO_WriteRegister(
	_In_		unsigned int		PunRegisterAddress,
	_In_		BYTE				PbRegisterValue)
{
	unsigned int unReturnValue = RC_E_FAIL;

	LOGGING_WRITE_LEVEL4(LOGGING_METHOD_ENTRY_STRING);

	// Write byte to register address
	DeviceAccess_WriteByte(PunRegisterAddress, PbRegisterValue);
	unReturnValue = RC_SUCCESS;

	LOGGING_WRITE_LEVEL4_FMT(LOGGING_METHOD_EXIT_STRING_RET_VAL, unReturnValue);

	return unReturnValue;
}
