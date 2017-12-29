/**
 *	@file		swap.h
 *	@brief		Declares byte array swap methods.
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

#define BYTE_ARRAY_TO_UINT8(b)	(UINT8)((b)[0])

#define BYTE_ARRAY_TO_UINT16(b)	(UINT16)(((b)[0] << 8) + (b)[1])

#define BYTE_ARRAY_TO_UINT32(b)	(UINT32)(((b)[0] << 24) \
										+ ((b)[1] << 16) \
										+ ((b)[2] << 8) \
										+ (b)[3])

#define BYTE_ARRAY_TO_UINT64(b)	(UINT64)(((UINT64)(b)[0] << 56) \
										+ ((UINT64)(b)[1] << 48) \
										+ ((UINT64)(b)[2] << 40) \
										+ ((UINT64)(b)[3] << 32) \
										+ ((UINT64)(b)[4] << 24) \
										+ ((UINT64)(b)[5] << 16) \
										+ ((UINT64)(b)[6] << 8) \
										+ (UINT64)(b)[7])

#define UINT8_TO_BYTE_ARRAY(i, b)		((b)[0] = (BYTE)(i))

#define UINT16_TO_BYTE_ARRAY(i, b)		((b)[0] = (BYTE)((i) >> 8), \
										(b)[1] = (BYTE) (i))

#define UINT32_TO_BYTE_ARRAY(i, b)		((b)[0] = (BYTE)((i) >> 24), \
										(b)[1] = (BYTE)((i) >> 16), \
										(b)[2] = (BYTE)((i) >> 8), \
										(b)[3] = (BYTE) (i))

#define UINT64_TO_BYTE_ARRAY(i, b)		((b)[0] = (BYTE)((i) >> 56), \
										(b)[1] = (BYTE)((i) >> 48), \
										(b)[2] = (BYTE)((i) >> 40), \
										(b)[3] = (BYTE)((i) >> 32), \
										(b)[4] = (BYTE)((i) >> 24), \
										(b)[5] = (BYTE)((i) >> 16), \
										(b)[6] = (BYTE)((i) >> 8), \
										(b)[7] = (BYTE) (i))
