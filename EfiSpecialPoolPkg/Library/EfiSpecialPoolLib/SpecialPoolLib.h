/** @file
  Special pool interfaces.


  Copyright (c) 2016, Microsoft Corporation. All rights reserved.<BR>

**/

#ifndef __EFI_SPECIAL_POOL_H__
#define __EFI_SPECIAL_POOL_H__

typedef union {
    EFI_PHYSICAL_ADDRESS Phys;
    UINT8               *u8;
} SPECIAL_POOL_ADDRESS;

#define POOL_HEAD_SIGNATURE   SIGNATURE_32('s','p','c','l')
typedef struct {
  UINT32          Signature;
  UINT32          Reserved;
  EFI_MEMORY_TYPE Type;
  UINTN           Size;
  CHAR8           Data[1];
} POOL_HEAD;

#define SIZE_OF_POOL_HEAD OFFSET_OF(POOL_HEAD,Data)

#define POOL_TAIL_SIGNATURE   SIGNATURE_32('p','o','o','l')
typedef struct {
  UINT32      Signature;
  UINT32      Reserved;
  UINTN       Size;
} POOL_TAIL;

#define POOL_OVERHEAD (SIZE_OF_POOL_HEAD + sizeof(POOL_TAIL))

#define HEAD_TO_TAIL(a)   \
  ((POOL_TAIL *) (((CHAR8 *) (a)) + (a)->Size - sizeof(POOL_TAIL)));

//
//  Interfaces to the Virtual Memory processor unique code
//

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
EnablePage (EFI_PHYSICAL_ADDRESS Address);

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
DisablePage (EFI_PHYSICAL_ADDRESS Address);

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
InitializeSpecialPool (IN EFI_SYSTEM_TABLE  *SystemTable);

/**
 * InitializeStackFaultHandling
 *
 * Requires Special Pool to be initialized.
 *
 * @param SystemTable
 *
 * @return VOID
 */
VOID InitializeStackFaultHanding (VOID);

/**
 * UsingMasterCr3
 *
 * Is the proper set of page tables being used?
 *
 *
 * @return BOOLEAN  TRUE= System is using SpecialPool page tables
 */
BOOLEAN UsingSpecialPoolPageTables (VOID);

#endif
