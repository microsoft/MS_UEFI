/**
PPI to describe a hash digest for a given FV

Copyright (c) 2017, Microsoft Corporation

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

#ifndef __PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV_H__
#define __PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV_H__

#define PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV_PPI_GUID \
 { 0x3ce1e631, 0x7008, 0x477c, { 0xad, 0xa7, 0x5d, 0xcf, 0xc7, 0xc1, 0x49, 0x4b } }

#pragma warning(push)
#pragma warning(disable: 4200) // zero-sized array
#pragma pack(push)
#pragma pack(1)

//
// This structure is currently matching PlatformData revision 2.0 structure.
// If PlatformData structure changes in future, this structure must be 
// populated accordingly.
//
typedef struct _PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV {
  UINT32                                     FvBase;
  UINT32                                     FvLength;
  UINT16                                     HashAlgoId;
  UINT16                                     HashSize;
  UINT8                                      FvHash[];
} PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV;

//
// This PPI means a FV is already hashed and only the hash needs to be extended  
// to PCR and logged to TCG event log by TCG modules
// The Count field in PPI is followed by Count number of Prehashed FV entries
//
typedef struct {
  UINT32                                     Count;
  //PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV      Fv[];
} PEI_FIRMWARE_VOLUME_INFO_PREHASHED_FV_PPI;

#pragma pack(pop)
#pragma warning(pop)

extern EFI_GUID gPeiFirmwareVolumeInfoPrehashedFvPpiGuid;

#endif

