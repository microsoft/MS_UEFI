/**
 *	@brief		Implements the error handling methods.
 *	@details	This module stores information about the error occurred and maps
 *				errors to outgoing error codes and messages for logging and displaying.
 *	@file		Error.c
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

#include "Error.h"

/**
 *	@brief		Function to return the error stack
 *	@details	Not used
 *
 *	@returns	NULL
 */
_Check_return_
IfxErrorData*
Error_GetStack()
{
	return NULL;
}

/**
 *	@brief		Function to clear the error stack
 *	@details	Not used
 */
void
Error_ClearStack()
{
}

/**
 *	@brief		Function to clear the first item in the error stack
 *	@details	Not used
 */
void
Error_ClearFirstItem()
{
}

/**
 *	@brief		Function to store all parameters to an IfxErrorData structure
 *	@details	Not used
 *
 *	@param		PszOccurredInModule			Pointer to a char array holding the module name where the error occurred
 *	@param		PszOccurredInFunction		Pointer to a char array holding the function name where the error occurred
 *	@param		PnOccurredInLine			The line where the error occurred
 *	@param		PunInternalErrorCode		Internal error code
 *	@param		PwszInternalErrorMessage	Format string used to format the internal error message
 *	@param		PvaArgumentList				Parameters needed to format the error message
 */
_Check_return_
IfxErrorData*
Error_GetErrorData(
	_In_z_	const char*		PszOccurredInModule,
	_In_z_	const char*		PszOccurredInFunction,
	_In_	int				PnOccurredInLine,
	_In_	unsigned int	PunInternalErrorCode,
	_In_z_	const wchar_t*	PwszInternalErrorMessage,
	_In_	va_list			PvaArgumentList)
{
	UNREFERENCED_PARAMETER(PszOccurredInModule);
	UNREFERENCED_PARAMETER(PszOccurredInFunction);
	UNREFERENCED_PARAMETER(PnOccurredInLine);
	UNREFERENCED_PARAMETER(PunInternalErrorCode);
	UNREFERENCED_PARAMETER(PwszInternalErrorMessage);
	UNREFERENCED_PARAMETER(PvaArgumentList);
	return NULL;
}

/**
 *	@brief		Function to store an error
 *	@details	This function stores an error and its specific parameters for later use.
 *				The error element is stored as the first element of the error list (stack).
 *
 *	@param		PszOccurredInModule			Pointer to a char array holding the module name where the error occurred
 *	@param		PszOccurredInFunction		Pointer to a char array holding the function name where the error occurred
 *	@param		PnOccurredInLine			The line where the error occurred
 *	@param		PunInternalErrorCode		Internal error code
 *	@param		PwszInternalErrorMessage	Format string used to format the internal error message
 *	@param		...							Parameters needed to format the error message
 */
void
Error_Store(
	_In_z_	const char*		PszOccurredInModule,
	_In_z_	const char*		PszOccurredInFunction,
	_In_	int				PnOccurredInLine,
	_In_	unsigned int	PunInternalErrorCode,
	_In_z_	const wchar_t*	PwszInternalErrorMessage,
	...)
{
	unsigned int unReturnValue = RC_E_FAIL;
	UNREFERENCED_PARAMETER(PnOccurredInLine);

	do
	{
		wchar_t wszMessage[6144] = {0};
		unsigned int unMessageSize = RG_LEN(wszMessage);
		unsigned int unSize = unMessageSize;
		va_list argptr;

		// Format error code to message
		unReturnValue = Platform_StringFormat(wszMessage, &unSize, L"ErrorCode: 0x%.8x; ErrorMessage: " , PunInternalErrorCode);
		if (RC_SUCCESS != unReturnValue)
			break;

		unMessageSize -= unSize;

		// Format Message
		va_start(argptr, PwszInternalErrorMessage);
		unReturnValue = Platform_StringFormatV(&wszMessage[unSize], &unMessageSize, PwszInternalErrorMessage, argptr);
		va_end(argptr);

		if (RC_SUCCESS != unReturnValue)
			break;

		Logging_WriteLog(PszOccurredInModule, PszOccurredInFunction, LOGGING_LEVEL_1, wszMessage);
	}
	WHILE_FALSE_END;
}

/**
 *	@brief		Return the final error code
 *	@details	This function maps the given error code to the final one.
 *
 *	@retval		The final mapped error code
 */
_Check_return_
unsigned int
Error_GetFinalCodeFromError(
	_In_ unsigned int PunErrorCode)
{
	return PunErrorCode;
}

/**
 *	@brief		Return the final error code
 *	@details	Not used
 *
 *	@retval		The final mapped error code
 */
_Check_return_
unsigned int
Error_GetFinalCode()
{
	return RC_E_FAIL;;
}

/**
 *	@brief		Return the final error code
 *	@details	Not used
 *
 *	@param		PunErrorCode		Error Code to map to the final message
 *	@param		PwszErrorMessage	Pointer to a char buffer to copy the error message to
 *	@param		PpunBufferSize		Size of the error message buffer
 *	@retval		RC_SUCCESS			The operation completed successfully.
 *	@retval		RC_E_BAD_PARAMETER	An invalid parameter was passed to the function. E.g. PwszErrorMessage == NULL
 *	@retval		...					Error codes from called functions.
 */
_Check_return_
unsigned int
Error_GetFinalMessageFromErrorCode(
	_In_							unsigned int	PunErrorCode,
	_Out_z_cap_(*PpunBufferSize)	wchar_t*		PwszErrorMessage,
	_Inout_							unsigned int*	PpunBufferSize)
{
	UNREFERENCED_PARAMETER(PunErrorCode);
	UNREFERENCED_PARAMETER(PwszErrorMessage);
	UNREFERENCED_PARAMETER(PpunBufferSize);
	return RC_E_FAIL;
}

/**
 *	@brief		Return the final error code
 *	@details	Not used
 *
 *	@param		PwszErrorMessage	Pointer to a char buffer to copy the error message to
 *	@param		PpunBufferSize		Size of the error message buffer
 *	@retval		RC_SUCCESS			The operation completed successfully.
 *	@retval		RC_E_BAD_PARAMETER	An invalid parameter was passed to the function. E.g. PwszErrorMessage == NULL
 *	@retval		...					Error codes from called functions.
 */
_Check_return_
unsigned int
Error_GetFinalMessage(
	_Out_z_cap_(*PpunBufferSize)	wchar_t*		PwszErrorMessage,
	_Inout_							unsigned int*	PpunBufferSize)
{
	UNREFERENCED_PARAMETER(PwszErrorMessage);
	UNREFERENCED_PARAMETER(PpunBufferSize);
	return RC_E_FAIL;
}

/**
 *	@brief		Return the internal error code
 *	@details	Not used
 *
 *	@retval		The internal error code
 */
_Check_return_
unsigned int
Error_GetInternalCode()
{
	return RC_E_FAIL;
}

/**
 *	@brief		Log the error stack
 *	@details	Not used
 */
void
Error_LogStack()
{
}

/**
 *	@brief		Log error code and message
 *	@details	Not used
 *
 *	@param		PszOccurredInModule			Pointer to a char array holding the module name where the error occurred
 *	@param		PszOccurredInFunction		Pointer to a char array holding the function name where the error occurred
 *	@param		PnOccurredInLine			The line where the error occurred
 *	@param		PunInternalErrorCode		Internal error code
 *	@param		PwszInternalErrorMessage	Format string used to format the internal error message
 *	@param		...							Parameters needed to format the error message
 */
void
Error_LogCodeAndMessage(
	_In_z_	const char*		PszOccurredInModule,
	_In_z_	const char*		PszOccurredInFunction,
	_In_	int				PnOccurredInLine,
	_In_	unsigned int	PunInternalErrorCode,
	_In_z_	const wchar_t*	PwszInternalErrorMessage,
	...)
{
	UNREFERENCED_PARAMETER(PszOccurredInModule);
	UNREFERENCED_PARAMETER(PszOccurredInFunction);
	UNREFERENCED_PARAMETER(PnOccurredInLine);
	UNREFERENCED_PARAMETER(PunInternalErrorCode);
	UNREFERENCED_PARAMETER(PwszInternalErrorMessage);
}

/**
 *	@brief		Log IfxErrorData and linked IfxErrorData
 *	@details	Not used
 */
void
Error_LogErrorData(
	_In_	const IfxErrorData*	PpErrorData)
{
	UNREFERENCED_PARAMETER(PpErrorData);
}
