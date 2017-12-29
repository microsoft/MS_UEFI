/**
 *	@brief		Implements the TPM2_HierarchyChangeAuth method
 *	@file		TPM2_HierarchyChangeAuth.c
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
#include "TPM2_HierarchyChangeAuth.h"
#include "TPM2_Marshal.h"
#include "DeviceManagement.h"
#include "Platform.h"
#include "StdInclude.h"

/**
 *	@brief	Implementation of TPM2_HierarchyChangeAuth command.
 *
 *	@retval TPM_RC_SIZE						newAuth size is greater than that of integrity hash digest
 */
_Check_return_
unsigned int
TSS_TPM2_HierarchyChangeAuth(
	_In_	TPMI_RH_HIERARCHY_AUTH			authHandle,
	_In_	AuthorizationCommandData		authHandleSessionRequestData,
	_In_	TPM2B_AUTH						newAuth,
	_Out_	AcknowledgmentResponseData*		pAuthHandleSessionResponseData
)
{
	unsigned int unReturnValue = RC_SUCCESS;
	do
	{
		BYTE rgbRequest[MAX_COMMAND_SIZE] = {0};
		BYTE rgbResponse[MAX_RESPONSE_SIZE] = {0};
		BYTE* pbBuffer = NULL;
		INT32 nSizeRemaining = sizeof(rgbRequest);
		INT32 nSizeResponse = sizeof(rgbResponse);
		UINT32 unParameterSize = 0;
		// Request parameters
		TPM_ST tag = TPM_ST_SESSIONS;
		UINT32 unCommandSize = 0;
		TPM_CC commandCode = TPM_CC_HierarchyChangeAuth;
		// Response parameters
		UINT32 unResponseSize = 0;
		TPM_RC responseCode = TPM_RC_SUCCESS;

		// Initialize _Out_ parameters
		unReturnValue |= Platform_MemorySet(pAuthHandleSessionResponseData, 0x00, sizeof(AcknowledgmentResponseData));
		if (RC_SUCCESS != unReturnValue)
			break;
		// Marshal the request
		pbBuffer = rgbRequest;
		unReturnValue = TSS_TPMI_ST_COMMAND_TAG_Marshal(&tag, &pbBuffer, &nSizeRemaining);
		if (RC_SUCCESS != unReturnValue)
			break;
		unReturnValue = TSS_UINT32_Marshal(&unCommandSize, &pbBuffer, &nSizeRemaining);
		if (RC_SUCCESS != unReturnValue)
			break;
		unReturnValue = TSS_TPM_CC_Marshal(&commandCode, &pbBuffer, &nSizeRemaining);
		if (RC_SUCCESS != unReturnValue)
			break;
		unReturnValue = TSS_TPMI_RH_HIERARCHY_AUTH_Marshal(&authHandle, &pbBuffer, &nSizeRemaining);
		if (RC_SUCCESS != unReturnValue)
			break;
		// Marshal Session Context
		{
			UINT32 unSizeSessionData = 0;
			INT32 nSizeRemain = sizeof(UINT32);
			BYTE* prgbPositionOfSize = pbBuffer;
			unReturnValue = TSS_UINT32_Marshal(&unSizeSessionData, &pbBuffer, &nSizeRemaining);
			if (RC_SUCCESS != unReturnValue)
				break;
			unReturnValue = TSS_AuthorizationCommandData_Marshal(&authHandleSessionRequestData, &pbBuffer, &nSizeRemaining);
			if (RC_SUCCESS != unReturnValue)
				break;
			// Overwrite authorization data size
			unSizeSessionData = (UINT32) (pbBuffer - (prgbPositionOfSize + nSizeRemain));
			unReturnValue = TSS_UINT32_Marshal(&unSizeSessionData, &prgbPositionOfSize, &nSizeRemain);
			if (RC_SUCCESS != unReturnValue)
				break;
		}
		unReturnValue = TSS_TPM2B_AUTH_Marshal(&newAuth, &pbBuffer, &nSizeRemaining);
		if (RC_SUCCESS != unReturnValue)
			break;

		// Overwrite unCommandSize
		unCommandSize = sizeof(rgbRequest) - nSizeRemaining;
		pbBuffer = rgbRequest + 2;
		nSizeRemaining = 4;
		unReturnValue = TSS_UINT32_Marshal(&unCommandSize, &pbBuffer, &nSizeRemaining);
		if (RC_SUCCESS != unReturnValue)
			break;

		// Transmit the command over TDDL
		unReturnValue = DeviceManagement_Transmit(rgbRequest, unCommandSize, rgbResponse, (unsigned int*)&nSizeResponse);
		if (TPM_RC_SUCCESS != unReturnValue)
			break;

		// Unmarshal the response
		pbBuffer = rgbResponse;
		nSizeRemaining = nSizeResponse;
		unReturnValue = TSS_TPM_ST_Unmarshal(&tag, &pbBuffer, &nSizeRemaining);
		if (TPM_RC_SUCCESS != unReturnValue)
			break;
		unReturnValue = TSS_UINT32_Unmarshal(&unResponseSize, &pbBuffer, &nSizeRemaining);
		if (TPM_RC_SUCCESS != unReturnValue)
			break;
		unReturnValue = TSS_TPM_RC_Unmarshal(&responseCode, &pbBuffer, &nSizeRemaining);
		if (TPM_RC_SUCCESS != unReturnValue)
			break;
		if (responseCode != TPM_RC_SUCCESS)
		{
			unReturnValue = RC_TPM_MASK | responseCode;
			break;
		}
		// Unmarshal the parameter size
		unReturnValue = TSS_UINT32_Unmarshal(&unParameterSize, &pbBuffer, &nSizeRemaining);
		if (TPM_RC_SUCCESS != unReturnValue)
			break;
		// Unmarshal Session Context
		{
			unReturnValue = TSS_AcknowledgmentResponseData_Unmarshal(pAuthHandleSessionResponseData, &pbBuffer, &nSizeRemaining);
			if (RC_SUCCESS != unReturnValue)
				break;
		}
	}
	WHILE_FALSE_END;
	return unReturnValue;
}