/** @file
  x64 Virtual Memory Management Services in the form of an IA-32 driver.
  Used to establish a 1:1 Virtual to Physical Mapping that is required to
  enter Long Mode (x64 64-bit mode).

  While we make a 1:1 mapping (identity mapping) for all physical pages
  we still need to use the MTRR's to ensure that the cachability attributes
  for all memory regions is correct.

  The basic idea is to use 2MB page table entries where ever possible. If
  more granularity of cachability is required then 4K page tables are used.

  References:
    1) IA-32 Intel(R) Architecture Software Developer's Manual Volume 1:Basic Architecture, Intel
    2) IA-32 Intel(R) Architecture Software Developer's Manual Volume 2:Instruction Set Reference, Intel
    3) IA-32 Intel(R) Architecture Software Developer's Manual Volume 3:System Programmer's Guide, Intel

Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

Based on MdeModulePkg\Core\DxeIplPeim\X64\VirtualMemory.c

Copyright (c) 2016, Microsoft Corporation

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>

#include <Guid/MemoryAllocationHob.h>

#include "SpecialPoolLib.h"
#include <X64/ProcessorStructs.h>

#define PER_STACK_SIZE   SIZE_16KB               // Amount of stack space for each Interrupt Stack
#define TOTAL_STACK_SIZE (8 * PER_STACK_SIZE)    // There are 8 Interrupt Stacks available


EFI_SYSTEM_TABLE  *gSystemTable = NULL;


/**
 * Flush TLB
 *
 * Tell processor that the page tables have changed
 *
 * @param
 *
 * @return VOID
 */
VOID
FlushTLB (VOID) {
    UINT64  Cr3;

    Cr3 = AsmReadCr3 ();
    AsmWriteCr3 (Cr3);
}

/**
 * Get1GPteFromAddress
 *
 * Gets the pointer to the 1GPte for the address given
 *
 * @param[in] Address
 *
 * @return PAGE_TABLE_1G_ENTRY*
 */
PAGE_TABLE_1G_ENTRY *
Get1GPteFromAddress (EFI_PHYSICAL_ADDRESS Address) {

    UINT64                          Cr3;
    PAGE_MAP_AND_DIRECTORY_POINTER *Pml4;
    PAGE_TABLE_1G_ENTRY            *Pte1G;

    Cr3 = AsmReadCr3 () & ADDRESS_BITS;

    DEBUG((DEBUG_VERBOSE,"Checking Address %lx.  Cr3=%lx\n", Address, Cr3));

    Pml4 = (PAGE_MAP_AND_DIRECTORY_POINTER *) (Cr3 + Pml4Index(Address));
    DEBUG((DEBUG_VERBOSE,"Pml4 = %lx, *Pml4 = %lx\n",Pml4, Pml4->Uint64));

    if (!Pml4->Bits.Present) {
        return NULL;
    }

    Pte1G = (PAGE_TABLE_1G_ENTRY *) (Pml4->Uint64 & ADDRESS_BITS) + PdpteIndex(Address);
    DEBUG((DEBUG_VERBOSE,"Pte1G = %lx, *Pte1G = %lx, Pml4I=%lx, Index=%lx\n",Pte1G, Pte1G->Uint64,Pml4Index(Address),PdpteIndex(Address)));

    return Pte1G;
}

/**
 * Get 2MB Pte From Address
 *
 * Gets the pointer to the 2MB Pte for the address given
 *
 * @param[in] Address
 *
 * @return PAGE_TABLE_ENTRY*
 */
PAGE_TABLE_ENTRY *
Get2MPteFromAddress (EFI_PHYSICAL_ADDRESS Address) {

    PAGE_TABLE_1G_ENTRY         *Pte1G;
    PAGE_TABLE_ENTRY            *Pte2M;

    Pte1G = Get1GPteFromAddress (Address);
    if (Pte1G == NULL) {
        return NULL;
    }
    if (!Pte1G->Bits.Present) {
        return NULL;
    }
    if (Pte1G->Bits.MustBe1) {   // 1G PTE should have been converted
        return NULL;
    }

    Pte2M = (PAGE_TABLE_ENTRY *) (Pte1G->Uint64 & ADDRESS_BITS) + PdeIndex(Address);
    return Pte2M;
}

/**
 * Get 4KB Pte From Address
 *
 * Gets the pointer to the 4KB Pte for the address given
 *
 * @param[in] Address
 *
 * @return PAGE_TABLE_4K_ENTRY*
 */
PAGE_TABLE_4K_ENTRY *
Get4KPteFromAddress (EFI_PHYSICAL_ADDRESS Address) {
    PAGE_TABLE_ENTRY         *Pte2M;
    PAGE_TABLE_4K_ENTRY      *Pte4K;

    Pte2M = Get2MPteFromAddress (Address);
    if (Pte2M == NULL) {
        return NULL;
    }

    Pte4K = (PAGE_TABLE_4K_ENTRY *) (Pte2M->Uint64 & ADDRESS_BITS) + PteIndex(Address);
    return Pte4K;
}

/**
  Split 2M page to 4K.

  @param[in]      PhysicalAddress       Start physical address the 2M page covered.
  @param[in, out] PageEntry2M           Pointer to 2M page entry.

**/
VOID
Split2MPageTo4K (
  IN EFI_PHYSICAL_ADDRESS               PhysicalAddress,
  IN OUT UINT64                        *PageEntry2M
  )
{
    UINTN                                 IndexOfPageTableEntries;
    UINT64                                NewPageDirectoryEntry;
    PAGE_TABLE_4K_ENTRY                  *PageTableEntry;
    EFI_PHYSICAL_ADDRESS                  PhysicalAddress4K;
    EFI_STATUS                            Status;


    Status = gSystemTable->BootServices->AllocatePages(
                    AllocateAnyPages,
                    EfiBootServicesData,
                    1,
                    (EFI_PHYSICAL_ADDRESS *) &PageTableEntry);

    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
        return;
    }

    //
    // Compute 2M page entry.
    //
    NewPageDirectoryEntry = (UINT64) (UINTN) PageTableEntry | IA32_PG_P | IA32_PG_RW;

    PhysicalAddress4K = PhysicalAddress;
    for (IndexOfPageTableEntries = 0; IndexOfPageTableEntries < 512; IndexOfPageTableEntries++, PageTableEntry++, PhysicalAddress4K += SIZE_4KB) {
        //
        // Fill in the Page Table entries
        //
        PageTableEntry->Uint64 = (UINT64) PhysicalAddress4K;
        PageTableEntry->Bits.ReadWrite = 1;
        PageTableEntry->Bits.Present = 1;
    }

    //
    // Fill in 2M page entry after building new page table to
    // insure consistent mapping
    //
    *PageEntry2M = NewPageDirectoryEntry;
    FlushTLB ();
}

/**
  Split 1G page to 2M.

  @param[in]      PhysicalAddress       Start physical address the 1G page covered.
  @param[in, out] PageEntry1G           Pointer to 1G page entry.

**/
VOID
Split1GPageTo2M (
  IN EFI_PHYSICAL_ADDRESS               PhysicalAddress,
  IN OUT UINT64                        *PageEntry1G
  )
{
    UINTN                                 IndexOfPageDirectoryEntries;
    UINT64                                NewPageDirectoryEntry;
    PAGE_TABLE_ENTRY                     *PageDirectoryEntry;
    EFI_PHYSICAL_ADDRESS                  PhysicalAddress2M;
    EFI_STATUS                            Status;


    Status = gSystemTable->BootServices->AllocatePages(
                     AllocateAnyPages,
                     EfiBootServicesData,
                     1,
                     (EFI_PHYSICAL_ADDRESS *) &PageDirectoryEntry);

    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
        return;
    }

    //
    // Capture new directory entry.
    //
    NewPageDirectoryEntry = (UINT64) (UINTN) PageDirectoryEntry | IA32_PG_P | IA32_PG_RW;

    PhysicalAddress2M = PhysicalAddress;
    for (IndexOfPageDirectoryEntries = 0; IndexOfPageDirectoryEntries < 512; IndexOfPageDirectoryEntries++, PageDirectoryEntry++, PhysicalAddress2M += SIZE_2MB) {
        //
        // Fill in the Page Directory entries
        //
        PageDirectoryEntry->Uint64 = (UINT64) PhysicalAddress2M;
        PageDirectoryEntry->Bits.ReadWrite = 1;
        PageDirectoryEntry->Bits.Present = 1;
        PageDirectoryEntry->Bits.MustBe1 = 1;
    }

    //
    // Fill in 1G page entry after building directory to insure consisten mapping.
    //
    *PageEntry1G = NewPageDirectoryEntry;
    FlushTLB ();

}

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
    PAGE_TABLE_4K_ENTRY *Pte4K;
    EFI_STATUS           Status;


    Pte4K = Get4KPteFromAddress (Address);
    if (NULL == Pte4K) {
        Status = EFI_UNSUPPORTED;
    } else {
#if (0)   // Debug code to fill disabled pages
        if (Address != 0) {    // Avoid stupid ASSERT in SetMem32
            Pte4K->Bits.Present  = 1;   // Insure page present
            FlushTLB ();
            SetMem32((VOID *) (UINTN) Address,EFI_PAGE_SIZE,0x47617264);
        }
#endif
        Pte4K->Bits.Present  = 0;
        Pte4K->Bits.Accessed = 0;
        Pte4K->Bits.Dirty    = 0;
        FlushTLB ();
        Status = EFI_SUCCESS;
    }
    return Status;
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
    PAGE_TABLE_4K_ENTRY *Pte4K;
    EFI_STATUS           Status;


    Pte4K = Get4KPteFromAddress (Address);
    if (NULL == Pte4K) {
        Status = EFI_UNSUPPORTED;
    } else {
        Pte4K->Bits.Present = 1;
        Status = EFI_SUCCESS;
        FlushTLB ();
#if (0)  // debug code to fill enabled page
        SetMem32((VOID *) (UINTN) Address,EFI_PAGE_SIZE,0xAEAEAEAE);
#endif
    }
    return Status;
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
    UINT64                 Address;
    PAGE_TABLE_1G_ENTRY   *Pte1G;
    PAGE_TABLE_ENTRY      *Pte2M;


    gSystemTable = SystemTable;

    if (PcdGetBool(PcdUse1GPageTable)) {
        //
        // For all of UEFI's address range, convert 1GB page table entries
        // to 2MB page table entries for more granulatrity.
        //
        for (Address = PcdGet64(PcdSpecialPoolStartRange); Address < PcdGet64(PcdSpecialPoolEndRange); Address += SIZE_1GB) {
            Pte1G = Get1GPteFromAddress (Address);
            if ((Pte1G != NULL) &&
                 Pte1G->Bits.Present &&
                 Pte1G->Bits.MustBe1 ) {
                Split1GPageTo2M (Address, (UINT64 *) Pte1G);
            }
        }
    }

    //
    // For all of UEFI's address range, convert 2MB page table entries
    // to 4KB page table entries for more granulatrity.
    //
    for (Address = PcdGet64(PcdSpecialPoolStartRange); Address < PcdGet64(PcdSpecialPoolEndRange); Address += SIZE_2MB) {
        Pte2M = Get2MPteFromAddress(Address);
        if ((Pte2M != NULL) &&
             Pte2M->Bits.Present &&
             Pte2M->Bits.MustBe1 ) {
            Split2MPageTo4K (Address, (UINT64 *) Pte2M);
        }
    }

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
    IA32_SEGMENT_DESCRIPTOR   *Gdt;
    IA32_DESCRIPTOR            Gdtr_New;
    IA32_DESCRIPTOR            Gdtr_Old;
    EFI_PEI_HOB_POINTERS       Hob;
    UINTN                      i;
    IA32_IDT_GATE_DESCRIPTOR  *Idt;
    IA32_DESCRIPTOR            Idtr;
    UINT64                     StackStart;
    EFI_STATUS                 Status;
    EFI_PHYSICAL_ADDRESS       TaskSpace;
    UINT16                     Tr;
    TSS_X64                   *TSS;
    UINT64                     TSSBase;

    if (NULL == gSystemTable) {
        DEBUG((DEBUG_ERROR,__FUNCTION__ " gSystemTable is NULL\n"));
        return;
    }

    // Allocate new space for:
    //
    // 1. Task descriptor structure
    // 2. New GDT so we can add a TSS Descriptor to load into the TR
    // 3. Stack space for #DF (DoubleFault) and #SS (StackFault) exceptions

    TaskSpace = SIZE_4GB;
    Status = gSystemTable->BootServices->AllocatePages(
                      AllocateMaxAddress,
                      EfiBootServicesData,
                      TOTAL_STACK_SIZE / EFI_PAGE_SIZE,
                      &TaskSpace);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR,"Error allocating TSS space for Stack Fault handling\n"));
        return;
    }

    SetMem64 ((VOID *) TaskSpace, TOTAL_STACK_SIZE, 0x7479426b63617453);  // Fill region with 'StackByt'

    AsmReadGdtr(&Gdtr_Old);

    // Carve TSS and copy of GDT from the top stack space

    TSS = (TSS_X64 *) ((UINT64) TaskSpace + TOTAL_STACK_SIZE - sizeof (TSS_X64));
    ZeroMem (TSS, sizeof(TSS_X64));
    TSSBase = (UINT64) TSS;

    Gdtr_New.Base = TSSBase - (Gdtr_Old.Limit + 1 + sizeof(IA32_SEGMENT_DESCRIPTOR) * 2);
    Gdtr_New.Limit = Gdtr_Old.Limit + sizeof(IA32_SEGMENT_DESCRIPTOR) * 2;

    CopyMem ((VOID *) Gdtr_New.Base, (VOID *) Gdtr_Old.Base, Gdtr_Old.Limit + 1);

    Gdt = (IA32_SEGMENT_DESCRIPTOR *) Gdtr_New.Base;
    Tr = Gdtr_Old.Limit + 1;
    i = Tr / sizeof(IA32_SEGMENT_DESCRIPTOR);

    StackStart = (UINT64) Gdt;
    StackStart -= 0x10;

    Gdt[i].Bits.BaseLow   =  TSSBase & 0x0000FFFF;  // Low 16 bits;
    Gdt[i].Bits.BaseMid   = (TSSBase & 0x00FF0000 ) >> 16;
    Gdt[i].Bits.BaseHigh  = (TSSBase & 0xFF000000 ) >> 24;
    Gdt[i].Bits.LimitLow  = 0xFFFF;
    Gdt[i].Bits.LimitHigh = 0xFF;
    Gdt[i].Bits.Type      = 9;                 // 64 bit available TSS
    Gdt[i].Bits.P         = 1;
    Gdt[i].Bits.G         = 1;

    Gdt[i+1].Uint64       = 0;             // Two consecutive IA32_SEGMENT_DESCRIPTORS for X64 TSS

    TSS->IST1 = StackStart;                     // 16KB stack for #UD Stack Fault
    TSS->IST2 = TSS->IST1 - PER_STACK_SIZE;     // 16KB stack for #DF Double Fault
    TSS->IST3 = TSS->IST2 - PER_STACK_SIZE;     // 16KB stack for #SS Double Fault
    TSS->IST4 = TSS->IST3 - PER_STACK_SIZE;     // 16KB stack for #GP Double Fault
    TSS->IST5 = TSS->IST4 - PER_STACK_SIZE;     // 16KB stack for #PF Double Fault
    TSS->IST6 = TSS->IST5 - PER_STACK_SIZE;     // 16KB stack for #AC Double Fault
    TSS->IST7 = TSS->IST6 - PER_STACK_SIZE;     // 16KB stack for #MC Double Fault

    DEBUG((DEBUG_INFO,"TSS @ %p, Gdt @ %p, Stack @ %p, Tr=%x\n", TSS, Gdt, TSS->IST1, Tr));

    AsmWriteGdtr (&Gdtr_New);

    AsmWriteTr(Tr);

    AsmReadIdtr(&Idtr);

    Idt = (IA32_IDT_GATE_DESCRIPTOR *) Idtr.Base;

    // Pick 8 of the most likely exceptions that may need its own stack
    // as the TSS has 8 Interrupt Stacks

    Idt[6].Bits.Reserved_0 = 1;            // Tell #UD to use IST1
    Idt[8].Bits.Reserved_0 = 2;            // Tell #DF to use IST2
    Idt[12].Bits.Reserved_0 = 3;           // Tell #SS to use IST3
    Idt[13].Bits.Reserved_0 = 4;           // Tell #GP to use IST4
    Idt[14].Bits.Reserved_0 = 5;           // Tell #PF to use IST5
    Idt[17].Bits.Reserved_0 = 6;           // Tell #AC to use IST6
    Idt[18].Bits.Reserved_0 = 7;           // Tell #MC to use IST7

#if 0   // set to 1 to validate a fault from the stack.
    // Disabling the stack page should generate a page fault on the stack pointer when
    // the stack is used to return from DisablePage
    DisablePage ((EFI_PHYSICAL_ADDRESS) &SystemTable);
#endif

    Hob.Raw = GetHobList ();
    while ((Hob.Raw = GetNextHob (EFI_HOB_TYPE_MEMORY_ALLOCATION, Hob.Raw)) != NULL) {
      if (CompareGuid (&gEfiHobMemoryAllocStackGuid, &(Hob.MemoryAllocationStack->AllocDescriptor.Name))) {
          // Disable the lowest page in the stack to prevent overflowing into other allocations
          DisablePage (Hob.MemoryAllocationStack->AllocDescriptor.MemoryBaseAddress);
          DEBUG((DEBUG_INFO,"Stack page @ %p disabled\n",Hob.MemoryAllocationStack->AllocDescriptor.MemoryBaseAddress));
        break;
      }
      Hob.Raw = GET_NEXT_HOB (Hob);
    }

    return;
}

