/**
 *	@brief		Implements logging methods for the application
 *	@details	This module provides logging for the application.
 *	@file		Logging.c
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

#include "Logging.h"
#include "IFXTPMUpdateApp.h"
#include "Utility.h"

/// Flag indicating whether to write a header into the log file or not
BOOL g_fLogHeader = TRUE;

/// Flag indicating whether logging is already ongoing
BOOL g_fInLogging = FALSE;

/**
 *	@brief		Logging function
 *	@details	Logs all messages for logging levels 0 - 3 in case a logging callback function has been set by the caller of the driver.
 *
 *	@param		PszCurrentModule		Pointer to a char array holding the module name (optional, can be NULL)
 *	@param		PszCurrentFunction		Pointer to a char array holding the function name (optional, can be NULL)
 *	@param		PunLoggingLevel			Logging level
 *	@param		PwszLoggingMessage		Format string used to format the message
 *	@param		...						Parameters needed to format the message
 */
void
Logging_WriteLog(
	_In_z_	const char*		PszCurrentModule,
	_In_z_	const char*		PszCurrentFunction,
	_In_	unsigned int	PunLoggingLevel,
	_In_z_	const wchar_t*	PwszLoggingMessage,
	...)
{
	UNREFERENCED_PARAMETER(PszCurrentModule);
	UNREFERENCED_PARAMETER(PszCurrentFunction);
	do
	{
		if (g_pPrivateData->pfnLogCallback != NULL && PunLoggingLevel <= 3)
		{
			if (NULL != PwszLoggingMessage)
			{
				// Skip formating if an empty line should be logged
				if (PwszLoggingMessage[0] != L'\0')
				{
					wchar_t wszMessage[6144] = {0};
					unsigned int unMessageSize = RG_LEN(wszMessage);
					unsigned int unSize = unMessageSize;
					unsigned int unCount = 0;
					unsigned int unReturnValue = RC_E_FAIL;
					va_list argptr;

					if (g_pPrivateData->fLogTimeStamps)
					{
						// Get time-stamp as string
						unReturnValue = Utility_GetTimestamp(TRUE, &wszMessage[0], &unSize);
						if (RC_SUCCESS != unReturnValue)
							// Ignore that the time-stamp cant be returned
							unSize = 0;
						else
							wszMessage[unSize++] = L' ';
					}
					else
					{
						// Skip time-stamp
						unSize = 0;
					}

					unCount += unSize;
					unSize = unMessageSize - unCount;

					// Format Message
					va_start(argptr, PwszLoggingMessage);
					unReturnValue = Platform_StringFormatV(&wszMessage[unCount], &unSize, PwszLoggingMessage, argptr);
					va_end(argptr);
					if (RC_SUCCESS != unReturnValue)
						break;

					unCount += unSize;
					unSize = unMessageSize - unCount;
					unReturnValue = Platform_StringFormat(&wszMessage[unCount], &unSize, L"\n");
					if (RC_SUCCESS != unReturnValue)
						break;

					unCount += unSize + 1;

					// Log the message
					g_pPrivateData->pfnLogCallback(unCount * sizeof(CHAR16), (CHAR16*)wszMessage);
				}
			}
		}
	}
	WHILE_FALSE_END
}

/**
 *	@brief		Log hex dump function
 *	@details	Logs hex data for logging levels 0 - 3 in case a logging callback function has been set by the caller of the driver.
 *
 *	@param		PszCurrentModule		Pointer to a char array holding the module name (optional, can be NULL)
 *	@param		PszCurrentFunction		Pointer to a char array holding the function name (optional, can be NULL)
 *	@param		PunLoggingLevel			Logging level
 *	@param		PrgbHexData				Format string used to format the message
 *	@param		PunSize					Size of hex data buffer
 */
void
Logging_WriteHex(
	_In_z_					const char*		PszCurrentModule,
	_In_z_					const char*		PszCurrentFunction,
	_In_					unsigned int	PunLoggingLevel,
	_In_bytecount_(PunSize)	const BYTE*		PrgbHexData,
	_In_					unsigned int	PunSize)
{
	UNREFERENCED_PARAMETER(PszCurrentModule);
	UNREFERENCED_PARAMETER(PszCurrentFunction);
	do
	{
		// Check parameters
		if (NULL == PrgbHexData || 0 == PunSize)
			break;

		if (g_pPrivateData->pfnLogCallback != NULL && PunLoggingLevel <= 3)
		{
			wchar_t wszMessage[6144] = {0};
			unsigned int unMessageSize = RG_LEN(wszMessage);
			unsigned int unSize = unMessageSize;
			unsigned int unCount = 0;
			unsigned int unReturnValue = RC_E_FAIL;

			if (g_pPrivateData->fLogTimeStamps)
			{
				// Get time-stamp as string
				unReturnValue = Utility_GetTimestamp(TRUE, &wszMessage[0], &unSize);
				if (RC_SUCCESS != unReturnValue)
					// Ignore that the time-stamp cannot be returned
					unSize = 0;
				else
				{
					// Format Message
					unCount += unSize;
					unSize = unMessageSize - unCount;
					// Add a CR LF to the string (needs to be done with Platform_StringFormat because UEFI
					// does not interpret \n as \r\n in other string copy or concatenation functions)
					unReturnValue = Platform_StringFormat(&wszMessage[unCount], &unSize, L"\n");
					if (RC_SUCCESS != unReturnValue)
						break;
				}
			}
			else
			{
				// Skip time-stamp
				unSize = 0;
			}

			// Format Message
			unCount += unSize;
			unSize = unMessageSize - unCount;
			// Convert hex data to string
			unReturnValue = Utility_StringWriteHex(PrgbHexData, PunSize, &wszMessage[unCount], &unSize);
			if (RC_SUCCESS != unReturnValue)
				break;

			unCount += unSize;
			unSize = unMessageSize - unCount;
			unReturnValue = Platform_StringFormat(&wszMessage[unCount], &unSize, L"\n");
			if (RC_SUCCESS != unReturnValue)
				break;

			unCount += unSize + 1;
			// Log the message
			g_pPrivateData->pfnLogCallback(unCount * sizeof(CHAR16), (CHAR16*)wszMessage);
		}
	}
	WHILE_FALSE_END;
}
