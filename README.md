# ----- UEFI Confidential -----

# High Level Goals

Define a time based event mechanism for PEI (Pei Delayed Dispatch):

* Eliminate spin loops for timing. Instead of blocking for 50 or 100 ms, the driver   would request callbacks after 50ms (or 100ms) and allow other PEIM’s to dispatch. Each loop through the PEI dispatcher will attempt to dispatch each of the queued delayed dispatch requests.
  
* Method to schedule a callback after a minimum delay, not a real time callback. Maintains a 64 bit "State" for use by the callback.

* PPI has only one function - Register, to register a function to be called after the required dealy with the current value of "State".

```
Status = DelayedDispatchPpi->Register (DelayedDispatchPpi, 
                                       TestDispatchFunction, 
                                       0,                    // Initial state is 0 
                                       100000);              // 100ms 

```
* Callback can request to be called back again after a new delay.  If *NewDelay if not set to a new value, the Delayed Dispatch element is terminated.
```
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
# ----- UEFI Confidential -----