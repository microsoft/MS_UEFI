 # Welcome to the MS_UEFI repository.  

This repo is used as a public place to share common UEFI features and prototypes as we work to get fully integrated into the tianocore edk2 project.  

## Copyright & License
All Microsoft contributed code will contain a Microsoft Copyright and be licensed under a standard BSD 2 clause license (same as edk2)

Copyright (c) 2016, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## Branch Model

The repo contains various branches to isolate different activities. 

### upstream/tiano

This branch contains the latest tianocore code integrated into the repo.  This is the basis for all feature or share branches. 

### share/**_feature name_**

These branches contain an isolated feature we want to promote and share with the UEFI community.  Our ultimate goal is to get the feature integrated into the tianocore edk2 repository and then remove this branch.  Until that happens we are happy to get feedback and suggestions.   
