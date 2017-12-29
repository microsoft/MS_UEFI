/** @file -- MsStatusCodes.h
This header contains definitions, flags, and masks to work with the
Surface-specific status codes for ReportStatusCode.

Copyright (c) 2016-2017, Microsoft Corporation

All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

#ifndef _MS_STATUS_CODES_H_
#define _MS_STATUS_CODES_H_

//
// This is the parent class for all MS status codes.
//
#define MS_RSC_PARENT_CLASS                 0xA0000000

//
// Status Code Subclasses
//
#define MS_RSC_SUBCLASS_CAPSULE             0xB0000
#define MS_RSC_SUBCLASS_MISC                0xC0000

//
// Miscellaneous Codes
//
#define MS_RSC_SPI_PERSISTENCE_LIB_RW_FAILURE           (MS_RSC_PARENT_CLASS | MS_RSC_SUBCLASS_MISC     | 0x00000001)
#define MS_RSC_VARIABLE_ANALYSIS_DUPLICATE_VARIABLE     (MS_RSC_PARENT_CLASS | MS_RSC_SUBCLASS_MISC     | 0x00000002)

//
// Other Capsule Codes
#define MS_RSC_CAPSULE_TPM_NO_MORE_RECOVERY_ATTEMPTS    (MS_RSC_PARENT_CLASS | MS_RSC_SUBCLASS_CAPSULE  | 0x00000001)
#define MS_RSC_CAPSULE_TPM_INTERNAL_SETIMAGE_FAILED     (MS_RSC_PARENT_CLASS | MS_RSC_SUBCLASS_CAPSULE  | 0x00000002)
#define MS_RSC_CAPSULE_TPM_RECOVERY_STARTED             (MS_RSC_PARENT_CLASS | MS_RSC_SUBCLASS_CAPSULE  | 0x00000003)
#define MS_RSC_CAPSULE_TPM_NO_MORE_UPDATES_ALLOWED      (MS_RSC_PARENT_CLASS | MS_RSC_SUBCLASS_CAPSULE  | 0x00000004)

#endif // _MS_STATUS_CODES_H_
