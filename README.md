# TPM 2 Debugging Lib Feature

## Copyright

Copyright (c) 2016, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

This feature branch adds three implementations of `Tpm2DebugLib`, including a verbose implementation that performs data analysis to simplify the debugging process. This library is linked against the `Tpm2DeviceLibDTpm` (both TIS and PTP interfaces) so that debug strings are only produced when data is sent/received to/from the TPM itself.

## Tpm2DebugLibNull

This is a NULL implementation that produces no debugging messages at all.

## Tpm2DebugLibSimple

This is a simple implementation that does no data analysis and only dumps portions of the _command_ and _response_ buffers.

Note that this lib will only produce strings if `DEBUG_INFO` level strings are enabled.

## Tpm2DebugLibVerbose

This is a verbose implemention that will analyze the _command_ and _response_ headers, dump _command code_ and _response code_ information from within the headers, and attempt to interpret certain commands. At this time, the commands that can be interpreted are `TPM_CC_PCR_Extend` and `TPM_CC_PCR_Event`.

Note that this lib will only produce strings if `DEBUG_INFO` level strings are enabled. It will also produce more elaborate strings if `DEBUG_VERBOSE` level strings are enabled.