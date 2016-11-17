/** @file
  x64 Long Mode Virtual Memory Management Definitions

  References:
    1) IA-32 Intel(R) Architecture Software Developer's Manual Volume 1:Basic Architecture, Intel
    2) IA-32 Intel(R) Architecture Software Developer's Manual Volume 2:Instruction Set Reference, Intel
    3) IA-32 Intel(R) Architecture Software Developer's Manual Volume 3:System Programmer's Guide, Intel
    4) AMD64 Architecture Programmer's Manual Volume 2: System Programming

Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

 Copyright (c) 2016, Microsoft Corporation

**/
#ifndef _PROCESSOR_STRUCTS_H_
#define _PROCESSOR_STRUCTS_H_


// This should be determined dynamically, but UEFI doesn't use full system memory. The high address
// bits should be zero anyway.   Mask is for 39 bits of physical address, enough for 512GB.
#define ADDRESS_BITS 0x0000007FFFFFF000ull

#define Pml4Index(a)  ((UINT64) ((a & 0x0000FF8000000000ull)) >> 39) //12+9+9+9
#define PdpteIndex(a) ((UINT64) ((a & 0x0000007FC0000000ull)) >> 30) //12+9+9
#define PdeIndex(a)   ((UINT64) ((a & 0x000000003FE00000ull)) >> 21) //12+9
#define PteIndex(a)   ((UINT64) ((a & 0x00000000001FF000ull)) >> 12) //12

#pragma pack(1)

typedef union {
  struct {
    UINT32  LimitLow    : 16;
    UINT32  BaseLow     : 16;
    UINT32  BaseMid     : 8;
    UINT32  Type        : 4;
    UINT32  System      : 1;
    UINT32  Dpl         : 2;
    UINT32  Present     : 1;
    UINT32  LimitHigh   : 4;
    UINT32  Software    : 1;
    UINT32  Reserved    : 1;
    UINT32  DefaultSize : 1;
    UINT32  Granularity : 1;
    UINT32  BaseHigh    : 8;
  } Bits;
  UINT64  Uint64;
} IA32_GDT;

typedef struct {
  IA32_IDT_GATE_DESCRIPTOR  Ia32IdtEntry;
  UINT32                    Offset32To63;
  UINT32                    Reserved;
} X64_IDT_GATE_DESCRIPTOR;
//
// Page-Map Level-4 Offset (PML4) and
// Page-Directory-Pointer Offset (PDPE) entries 4K & 2MB
//

typedef union {
  struct {
    UINT64  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;         // 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;           // 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Reserved:1;               // Reserved
    UINT64  MustBeZero:2;             // Must Be Zero
    UINT64  Available:3;              // Available for use by system software
    UINT64  PageTableBaseAddress:40;  // Page Table Base Address
    UINT64  AvabilableHigh:11;        // Available for use by system software
    UINT64  Nx:1;                     // No Execute bit
  } Bits;
  UINT64    Uint64;
} PAGE_MAP_AND_DIRECTORY_POINTER;

//
// Page Table Entry 4KB
//
typedef union {
  struct {
    UINT64  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;         // 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;           // 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty:1;                  // 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  PAT:1;                    //
    UINT64  Global:1;                 // 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available:3;              // Available for use by system software
    UINT64  PageTableBaseAddress:40;  // Page Table Base Address
    UINT64  AvabilableHigh:11;        // Available for use by system software
    UINT64  Nx:1;                     // 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64    Uint64;
} PAGE_TABLE_4K_ENTRY;

//
// Page Table Entry 2MB
//
typedef union {
  struct {
    UINT64  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;         // 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;           // 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty:1;                  // 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  MustBe1:1;                // Must be 1
    UINT64  Global:1;                 // 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available:3;              // Available for use by system software
    UINT64  PAT:1;                    //
    UINT64  MustBeZero:8;             // Must be zero;
    UINT64  PageTableBaseAddress:31;  // Page Table Base Address
    UINT64  AvabilableHigh:11;        // Available for use by system software
    UINT64  Nx:1;                     // 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64    Uint64;
} PAGE_TABLE_ENTRY;

//
// Page Table Entry 1GB
//
typedef union {
  struct {
    UINT64  Present:1;                // 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite:1;              // 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor:1;         // 0 = Supervisor, 1=User
    UINT64  WriteThrough:1;           // 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled:1;          // 0 = Cached, 1=Non-Cached
    UINT64  Accessed:1;               // 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty:1;                  // 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  MustBe1:1;                // Must be 1
    UINT64  Global:1;                 // 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available:3;              // Available for use by system software
    UINT64  PAT:1;                    //
    UINT64  MustBeZero:17;            // Must be zero;
    UINT64  PageTableBaseAddress:22;  // Page Table Base Address
    UINT64  AvabilableHigh:11;        // Available for use by system software
    UINT64  Nx:1;                     // 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64    Uint64;
} PAGE_TABLE_1G_ENTRY;

typedef struct {
    UINT32      Reserved1;
    UINT64      RSP0;                 // Ring 0 Stack
    UINT64      RSP1;                 // Ring 1 Stack
    UINT64      RSP2;                 // Ring 2 Stack
    UINT32      Reserved2;
    UINT32      Reserved3;
    UINT64      IST1;                 // Unique interrupt stacks
    UINT64      IST2;
    UINT64      IST3;
    UINT64      IST4;
    UINT64      IST5;
    UINT64      IST6;
    UINT64      IST7;
    UINT32      Reserved4;
    UINT32      Reserved5;
    UINT16      Reserved6;
    UINT16      IoMapBase;
    // Space for I/O Map if used
}  TSS_X64;

#pragma pack()

#define IA32_PG_P                   BIT0
#define IA32_PG_RW                  BIT1


#endif

