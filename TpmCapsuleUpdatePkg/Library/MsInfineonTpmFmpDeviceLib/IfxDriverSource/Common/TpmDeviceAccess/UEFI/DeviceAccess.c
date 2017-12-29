/**
 *	@brief		Implements the Device memory access routines
 *	@details
 *	@file		UEFI\DeviceAccess.c
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

#include "DeviceAccess.h"
#include "Logging.h"
#include <Library/IoLib.h>						// Because of TPM-LPC

/**
 *	@brief		Initialize the device access
 *	@details
 *
 *	@param		PbLocality		Locality value
 *	@retval		RC_SUCCESS		The operation completed successfully.
 */
_Check_return_
unsigned int
DeviceAccess_Initialize(
	_In_	BYTE	PbLocality)
{
	return RC_SUCCESS;
}

/**
 *	@brief		UnInitialize the device access
 *	@details
 *
 *	@param		PbLocality		Locality value
 *	@retval		RC_SUCCESS		The operation completed successfully.
 */
_Check_return_
unsigned int
DeviceAccess_Uninitialize(
	_In_	BYTE	PbLocality)
{
	return RC_SUCCESS;
}

/**
 *	@brief		Read a Byte from the specified memory address
 *	@details
 *
 *	@param		PunMemoryAddress	Memory address
 *	@returns	Data value read from specified memory
 */
_Check_return_
BYTE
DeviceAccess_ReadByte(
	_In_	unsigned int PunMemoryAddress)
{
	BYTE bData = 0;

	bData = MmioRead8(PunMemoryAddress);
	LOGGING_WRITE_LEVEL4_FMT(L"DeviceAccess_ReadByte:   Address: %0.8X :         %0.2X", PunMemoryAddress, bData);

	return bData;
}

/**
 *	@brief		Write a Byte to the specified memory address
 *	@details
 *
 *	@param		PunMemoryAddress	Memory address
 *	@param		PbData				Data to be written
 */
void
DeviceAccess_WriteByte(
	_In_	unsigned int	PunMemoryAddress,
	_In_	BYTE			PbData)
{
	UINT8 bDataWritten = 0;

	LOGGING_WRITE_LEVEL4_FMT(L"DeviceAccess_WriteByte:  Address: %0.8X = %0.2X", PunMemoryAddress, PbData);
	bDataWritten = MmioWrite8(PunMemoryAddress, PbData);

	if (bDataWritten != PbData)
		LOGGING_WRITE_LEVEL1_FMT(L"\n\tError: DeviceAccess_WriteByte: MemAddress %x is invalid !!!", PunMemoryAddress);
}

/**
 *	@brief		Read a Word from the specified memory address
 *	@details
 *
 *	@param		PunMemoryAddress	Memory address
 *	@returns	Data value read from specified memory
 */
_Check_return_
unsigned short
DeviceAccess_ReadWord(
	_In_	unsigned int	PunMemoryAddress)
{
	unsigned short usData = 0;

	// 16 bit read access must be aligned on a 16-bit boundary
	// because of that the access is split up into two 8 bit read's if necessary
	// This restriction is also true for a 16 bit write, and a 32 bit read/write access,
	// (32-bit aligned) but for the LPC-TPM there is so far no use case for this access
	if ((PunMemoryAddress & 1) == 1)
	{
		unsigned short usTemp = 0;
		LOGGING_WRITE_LEVEL4_FMT(L"DeviceAccess_ReadWord:   Address: %0.8X is not word aligned so read it in two byte operations", PunMemoryAddress);
		usData = MmioRead8(PunMemoryAddress);
		usTemp = MmioRead8(PunMemoryAddress + 1);
		usData |= (usTemp << 8);
	}
	else
		usData = MmioRead16(PunMemoryAddress);

	LOGGING_WRITE_LEVEL4_FMT(L"DeviceAccess_ReadWord:   Address: %0.8X :         %0.4X", PunMemoryAddress, usData);

	return usData;
}

/**
 *	@brief		Write a Word to the specified memory address
 *	@details
 *
 *	@param		PunMemoryAddress	Memory address
 *	@param		PusData				Data to be written
 */
void
DeviceAccess_WriteWord(
	_In_	unsigned int	PunMemoryAddress,
	_In_	unsigned short	PusData)
{
	UINT16 usDataWritten = 0;

	LOGGING_WRITE_LEVEL4_FMT(L"DeviceAccess_WriteWord:  Address: %0.8X = %0.4X", PunMemoryAddress, PusData);
	usDataWritten = MmioWrite16(PunMemoryAddress, PusData);

	if (usDataWritten != PusData)
		LOGGING_WRITE_LEVEL1_FMT(L"\n\tError: DeviceAccess_WriteWord: MemAddress %x is invalid !!!", PunMemoryAddress);
}
