/**
 *	@brief		Declares global definitions for the UEFI platform
 *	@details
 *	@file		Globals_UEFI.h
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

// Common include files
#include "Globals.h"

/// @cond SHOW_SAL_DEFINITIONS
// Defines for SAL annotations
#define _Check_return_

#define _In_					IN
#define _In_opt_				IN OPTIONAL
#define _In_bytecount_(x)		IN
#define _In_opt_bytecount_(x)	IN OPTIONAL
#define _In_z_					IN
#define _In_z_count_(x)			IN
#define _In_opt_z_count_(x)		IN OPTIONAL
#define _In_reads_z_(x)			IN
#define _In_reads_or_z_(x)		IN
#define _In_reads_bytes_(x)		IN
#define _In_reads_bytes_opt_(x) IN OPTIONAL

#define _Inout_					IN OUT
#define _Inout_bytecap_(x)		IN OUT
#define _Inout_opt_				IN OUT OPTIONAL
#define _Inout_updates_z_(x)	IN OUT
#define _Inout_z_				IN OUT
#define _Inout_z_cap_(x)		IN OUT
#define _Inout_opt_z_cap_(x)	IN OUT OPTIONAL

#define _Out_							OUT
#define _Out_bytecap_(x)				OUT
#define _Out_opt_bytecap_(x)			OUT OPTIONAL
#define _Out_opt_bytecapcount_(x)		OUT OPTIONAL
#define _Out_writes_bytes_all_(x)		OUT
#define _Out_writes_bytes_to_opt_(x, y)	OUT OPTIONAL
#define _Outptr_result_maybenull_		OUT
#define _Outptr_result_maybenull_z_		OUT
#define _Out_z_cap_(x)					OUT
#define _Out_z_bytecap_(x)				OUT
#define _Out_writes_z_(x)				OUT
/// @endcond

/// UEFI-compatible redefinition of Windows' va_list macro
#define va_list				VA_LIST
/// UEFI-compatible redefinition of Windows' va_start macro
#define va_start			VA_START
/// UEFI-compatible redefinition of Windows' va_end macro
#define va_end				VA_END
/// UEFI-compatible redefinition of Windows' va_arg macro
#define va_arg				VA_ARG

/// Scan code of ESC character on Windows
#define CHAR_ESC 27
/// End of file
#define EOF (-1)
/// Maximum numerical value of an unsigned 32-bit integer
#define UINT_MAX 0xFFFFFFFF
/// Maximum numerical value of an unsigned 64-bit integer
#define UINT64_MAX 0xFFFFFFFFFFFFFFFF
/// Maximum numerical value of a signed 32-bit integer
#define INT_MAX 2147483647
/// Maximum numerical value of a byte
#define MAXBYTE 0xFF

// Typedefs for MicroTss
/// Definition of type uint8_t
typedef unsigned char uint8_t;
/// Definition of type int8_t
typedef signed char int8_t;
/// Definition of type wchar_t
typedef unsigned short wchar_t;
/// Definition of type uint16_t
typedef unsigned short uint16_t;
/// Definition of type int16_t
typedef short int16_t;
/// Definition of type uint32_t
typedef unsigned int uint32_t;
/// Definition of type int32_t
typedef int int32_t;
/// Definition of type uint64_t
typedef unsigned long long uint64_t;
/// Definition of type int64_t
typedef long long int64_t;
