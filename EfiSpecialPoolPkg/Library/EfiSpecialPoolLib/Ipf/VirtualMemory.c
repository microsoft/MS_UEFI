/**

No support for this processor type

Copyright (c) 2016, Microsoft Corporation

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

/**
 * Disable Page
 *
 * Mark a page as disabled.  This will cause an exception if this
 * page is referenced or written to.
 *
 * @param Address
 *
 * @return EFI_STATUS
 */
EFI_STATUS
DisablePage (EFI_PHYSICAL_ADDRESS Address) {

    return EFI_UNSUPPORTED;
}

/**
 * Enable Page
 *
 * Enable the page at Address
 *
 * @param Address
 *
 * @return EFI_STATUS
 */
EFI_STATUS
EnablePage (EFI_PHYSICAL_ADDRESS Address) {

    return EFI_UNSUPPORTED;
}

/**
 * Initialize Special Pool.
 *
 *     1. Update the page table 2MB entries to 4KB pages.
 *     2. Flush TLB to force new mapping
 *
 * @param  NONE
 *
 * @return VOID
 */
VOID
InitializeSpecialPool (
    IN EFI_SYSTEM_TABLE  *SystemTable
) {

#error  Special pool not supported for this processor type

    return;
}

/**
 * InitializeStackFaultHandling
 *
 * @param SystemTable
 *
 * @return VOID
 */
VOID InitializeStackFaultHanding (
    VOID
) {

    return;
}

