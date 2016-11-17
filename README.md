# EFI Special Pool

## Copyright

Copyright (c) 2016, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

EFI Special Pool is a debug environment that implements:


1. Stack Guard
2. Interrupt Stacks
3. Buffer Overrun or Buffer Underrun protection.
4. Page 0 protection

## Stack Guard

Stack Guard disables the bottom page of the stack.  Any recursive calling or excessive stack use will cause a page fault.

## Interrupt Stacks

UEFI runs at the highest privilege mode, so there is no automatic stack switching for exceptions.  Most exceptions are caught and UEFI displays the register information in the debug log.  However, without Interrupt Stacks, an exception that is caused when a processor stack operation references an invalid page will cause a double fault interrupt - which will then trigger a double fault shutdown event.  By having a different interrupt stack for likely processor exceptions accessing the stack, the UEFI code is able to display the register information showing the problem that occurred.

## Buffer Overrun or Buffer Underrun protection

AllocatePool operations will allocate pages of memory sufficient for the request, align the request so that the end of the buffer is close to the end of the page(s) for Overrun protection, or close to the beginning of the page(s) for Buffer Underrun protection.  

## Page 0 protection

The page at location 0x00000000 is marked not present causing a page fault when a NULL pointer is dereferenced.

## Enabling EFI Special Pool

Add the EfiSpecialPool anonymous library to DxeMain: 

      MdeModulePkg/Core/Dxe/DxeMain.inf {
        <LibraryClasses>
          NULL|MdeModulePkg/Library/EfiSpecialPoolLib/EfiSpecialPoolLib.inf

## Pcd's that control EFI Special Pool

      ## This fixed at build flag defines whether Special Pool traps for buffer overrun, or buffer underrun
      #  If this PCD is set to TRUE then the data is set a the end of a page, if FALSE, then the data is
      #  set to the beginning of a page.
      gEfiMdeModulePkgTokenSpaceGuid.PcdSpecialPoolOverrun|TRUE
    
      ## This fixed at build flag defines whether Special Pool traps references to page 0
      #  If this PCD is set to TRUE then any reference to memory from 0x0-0xfff will cause an exception.
      gEfiMdeModulePkgTokenSpaceGuid.PcdSpecialPoolTrapPage0|TRUE
    
      ## This fixed at build flag defines the lower address limit where Special Pool applies
      # The start range starts at the 2MB boundary at or below the specified start range
      gEfiMdeModulePkgTokenSpaceGuid.PcdSpecialPoolStartRange|0x00000000
    
      ## This fixed at build flag defines the upper address limit where Special Pool applies
      # The end range ends at the 2MB boundary at or above the specified end range
      gEfiMdeModulePkgTokenSpaceGuid.PcdSpecialPoolEndRange|0x90000000
    
      ## This fixed at build flag defines the upper address limit where Special Pool applies
      #
      # Any of the following may be used, however, some will not make sense as only allocations
      # by AllocatePool are monitored (ie AllocatePages is not monitored, at least not yet)
      #
      # EfiReservedMemoryType   1
      # EfiLoaderCode   2
      # EfiLoaderData   4
      # EfiBootServicesCode 8
      # EfiBootServicesData10
      # EfiRuntimeServicesCode 20
      # EfiRuntimeServicesData 40
      # EfiConventionalMemory  80
      # EfiUnusableMemory 100
      # EfiACPIReclaimMemory  200
      # EfiACPIMemoryNVS  400
      # EfiMemoryMappedIO 800
      # EfiMemoryMappedIOPortSpace   1000
      # EfiPalCode   2000
      # EfiPersistentMemory  4000
      #
      gEfiMdeModulePkgTokenSpaceGuid.PcdSpecialPoolTypes|0x00000050
    
      ## This fixed at build flag enables or disables StackFault handing
      gEfiMdeModulePkgTokenSpaceGuid.PcdSpecialPoolStackFaultHandling|TRUE