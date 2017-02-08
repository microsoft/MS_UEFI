# Fastboot - Prehashed FVs

## Copyright

Copyright (c) 2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

The current UEFI implementation measures all FVs (including generating hashes) into TPM.  This results in slower boot because of hash generation taking significant time.

The *Prehashed FVs* feature relies on pre-generated hashes of FVs at build time and then passing these hashes in a newly defined PPI, `PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV_PPI`, to Tcg2Pei.c, which measures these FVs (using pre-generated hashes) into TPM.  This results in faster boot.

In Tcg2Pei.c, `MeasureMainBios()` will measure bootblock FV (FvInstance=0) only.  The rest of FVs are measured in callback `FirmwareVolmeInfoPpiNotifyCallback()`.  (The callback skips measuring child FVs.)  Both `MeasureMainBios()` and `FirmwareVolmeInfoPpiNotifyCallback()` call `MeasureFvImage()`, which checks all the instances of `PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV_PPI` for the given FV.  If a pre-generated FV hash is found in a PPI, then the code extends TPM PCR0 with that hash/digest and then finally logs a corresponding event in the TPM Event Log. 

