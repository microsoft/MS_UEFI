/** @file
  x64 Long Mode Virtual Memory Management Definitions  

  References:
    1) IA-32 Intel(R) Architecture Software Developer's Manual Volume 1:Basic Architecture, Intel
    2) IA-32 Intel(R) Architecture Software Developer's Manual Volume 2:Instruction Set Reference, Intel
    3) IA-32 Intel(R) Architecture Software Developer's Manual Volume 3:System Programmer's Guide, Intel
    4) AMD64 Architecture Programmer's Manual Volume 2: System Programming

Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2016, Microsoft Corporation

This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/  
#ifndef _VIRTUAL_MEMORY_H_
#define _VIRTUAL_MEMORY_H_


#define SYS_CODE64_SEL 0x38

/**
  Enable Execute Disable Bit.

**/
VOID
EnableExecuteDisableBit (
  VOID
  );

/**
  Split 2M page to 4K.

  @param[in]      PhysicalAddress       Start physical address the 2M page covered.
  @param[in, out] PageEntry2M           Pointer to 2M page entry.
  @param[in]      StackBase             Stack base address.
  @param[in]      StackSize             Stack size.

**/
VOID
Split2MPageTo4K (
  IN EFI_PHYSICAL_ADDRESS               PhysicalAddress,
  IN OUT UINT64                         *PageEntry2M,
  IN EFI_PHYSICAL_ADDRESS               StackBase,
  IN UINTN                              StackSize
  );

/**
  Allocates and fills in the Page Directory and Page Table Entries to
  establish a 1:1 Virtual to Physical mapping.

  @param[in] StackBase  Stack base address.
  @param[in] StackSize  Stack size.

  @return The address of 4 level page map.

**/
UINTN
CreateIdentityMappingPageTables (
  IN EFI_PHYSICAL_ADDRESS   StackBase,
  IN UINTN                  StackSize
  );


/**
 
  Fix up the vector number in the vector code.
 
  @param VectorBase   Base address of the vector handler.
  @param VectorNum    Index of vector.

**/
VOID
EFIAPI
AsmVectorFixup (
  VOID    *VectorBase,
  UINT8   VectorNum
  );


/**
 
  Get the information of vector template.
  
  @param TemplateBase   Base address of the template code.
 
  @return               Size of the Template code.

**/
UINTN
EFIAPI
AsmGetVectorTemplatInfo (
  OUT   VOID  **TemplateBase
  );


#endif 
