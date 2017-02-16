# Delayed Dispatch

## Copyright

Copyright (c) 2016, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## High Level Goals

Define a time based event mechanism for PEI (Pei Delayed Dispatch):

* Eliminate spin loops for timing. Instead of blocking for 50 or 100 ms, the driver   would request callbacks after 50ms (or 100ms) and allow other PEIM’s to dispatch. Each loop through the PEI dispatcher will attempt to dispatch each of the queued delayed dispatch requests.
  
* Method to schedule a callback after a minimum delay, not a real time callback. Maintains a 64 bit "State" for use by the callback.

* PPI has only one function - Register, to register a function to be called after the required dealy with the current value of "State".

```c
Status = DelayedDispatchPpi->Register (DelayedDispatchPpi, 
                                       TestDispatchFunction, 
                                       0,                    // Initial state is 0 
                                       100000);              // 100ms 

```

* Callback can request to be called back again after a new delay.  If *NewDelay if not set to a new value, the Delayed Dispatch element is terminated.

```c
VOID
EFIAPI
TestDispatchFunction (
    IN OUT UINT64 *State, 
    OUT UINT32 *NewDelay) { 
  switch (*State) { 
  case 0: 
      *State = 1; 
      *NewDelay = 200000; // 200 ms 
      break; 
  case 1: 
      *State = 2; 
      *NewDelay = 300000; //300 ms 
      break; 
  case 2: 
      *State = 3; 
      *NewDelay = 400000; //400 ms 
      break; 
  case 3: 
      break; 
  default: 
      DEBUG((DEBUG_ERROR,__FUNCTION__ " Invalid function entry\n")); 
      break; 
  }
}
```
