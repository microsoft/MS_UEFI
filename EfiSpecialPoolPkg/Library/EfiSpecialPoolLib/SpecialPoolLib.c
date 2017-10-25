/** @file
*  Implementation of Special Pool for UEFI
*
*  This library is linked anonamously to DXE_MAIN as an override in the build.dsc file.
*
*  Special Pool enables paging at 4K granularity, and then provides mechanisms to mark pages
*  invalid or valid.
*
*  In addition, Special Pool hooks CoreAllocatePool and CoreFreePool.  Requests that are not
*  monitored (ie "Not Special") are passed to the original routines. Memory types that are special
*  are allocated their own page(s) of memory, and two additional guard pages that are marked invalid.
*  The request is aligned to the beginning of the first, or the end of the last page.  Buffer overruns
*  will cause an exception.
*
**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>

#include "SpecialPoolLib.h"

EFI_ALLOCATE_POOL   gEfiAllocatePool;
EFI_FREE_POOL       gEfiFreePool;
UINTN               gGlobalAllocateCount = 0;
UINTN               gGlobalFreeCount = 0;
EFI_BOOT_SERVICES  *gBootServices;


// Special Pool versions of AllocatePool and FreePool

/**
  Allocate pool of a particular type.

  @param  PoolType               Type of pool to allocate
  @param  Size                   The amount of pool to allocate
  @param  Buffer                 The address to return a pointer to the allocated
                                 pool

  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
                                 PoolType is in the range EfiMaxMemoryType..0x6FFFFFFF.
                                 PoolType is EfiPersistentMemory.
  @retval EFI_OUT_OF_RESOURCES   Size exceeds max pool size or allocation failed.
  @retval EFI_SUCCESS            Pool successfully allocated.

**/
EFI_STATUS
EFIAPI
SpecialPool_AllocatePool (
  IN EFI_MEMORY_TYPE  PoolType,
  IN UINTN            Size,
  OUT VOID            **Buffer
  )
{
    EFI_STATUS            Status;
    UINTN                 NoPages;
    SPECIAL_POOL_ADDRESS  Guard1;                     //
    SPECIAL_POOL_ADDRESS  Guard2;                     //
    POOL_HEAD            *Head;     // User data within the block.
    POOL_TAIL            *Tail;     // User data within the block.


    if (NULL == Buffer) {
        return EFI_INVALID_PARAMETER;
    }

    if (((0x01 << PoolType) & PcdGet32(PcdSpecialPoolTypes)) &&  // Supported Pool Type   ?
        UsingSpecialPoolPageTables()) {                          // Proper page tables being used ?
        gGlobalAllocateCount++;
        Size = ALIGN_VARIABLE (Size);
        Size += POOL_OVERHEAD;
        NoPages = EFI_SIZE_TO_PAGES (Size) + 2;  // Add two guard pages

        Status = gBootServices->AllocatePages(AllocateAnyPages, PoolType, NoPages, &Guard1.Phys);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) {
            return EFI_OUT_OF_RESOURCES;
        }

        Guard2.u8 = Guard1.u8 + ((NoPages-1) * EFI_PAGE_SIZE);

        if (PcdGetBool (PcdSpecialPoolOverrun)) {
            Head = (POOL_HEAD *) (Guard2.u8 - Size);
        } else {
            Head = (POOL_HEAD *) (Guard1.u8 + EFI_PAGE_SIZE);
        }

        DisablePage (Guard1.Phys);
        DisablePage (Guard2.Phys);

        Head->Signature = POOL_HEAD_SIGNATURE;
        Head->Reserved  = 0;
        Head->Size      = Size;
        Head->Type      = (EFI_MEMORY_TYPE) PoolType;
        Tail            = HEAD_TO_TAIL (Head);
        Tail->Signature = POOL_TAIL_SIGNATURE;
        Tail->Reserved  = 0;
        Tail->Size      = Size;

        *Buffer = Head->Data;
        return EFI_SUCCESS;

    } else {
        return (*gEfiAllocatePool)(PoolType, Size, Buffer);
    }
}

/**
  Frees pool.

  @param  Buffer                 The allocated pool entry to free

  @retval EFI_INVALID_PARAMETER  Buffer is not a valid value.
  @retval EFI_SUCCESS            Pool successfully freed.

**/
EFI_STATUS
EFIAPI
SpecialPool_FreePool (
  IN VOID  *Buffer
  )
{
    POOL_HEAD            *Head;
    POOL_TAIL            *Tail;
    UINTN                 NoPages;
    SPECIAL_POOL_ADDRESS  Guard1;
    SPECIAL_POOL_ADDRESS  Guard2;

    if (NULL == Buffer) {
        return EFI_INVALID_PARAMETER;
    }

    // Cannot use CR for this as the Head->Signature will be different for special pool
    Head = (POOL_HEAD *) ((UINTN) Buffer - OFFSET_OF(POOL_HEAD,Data));
    ASSERT(Head != NULL);

    if (Head == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    if (Head->Signature == POOL_HEAD_SIGNATURE) {
        gGlobalFreeCount++;

        //
        // Debug
        //
        Tail = HEAD_TO_TAIL (Head);
        ASSERT(Tail != NULL);
        ASSERT (Tail->Signature == POOL_TAIL_SIGNATURE);
        ASSERT (Head->Size == Tail->Size);

        if ((Tail == NULL) ||
            (Tail->Signature != POOL_TAIL_SIGNATURE) ||
            (Head->Size != Tail->Size)) {
            return EFI_INVALID_PARAMETER;  // Don't free garbage
        }

        Head->Signature = 0;
        NoPages = EFI_SIZE_TO_PAGES (Head->Size) + 2;  // Add two guard pages

        Guard1.u8 = (UINT8 *) (((UINTN) Head) & ~(SIZE_4KB - 1)) - SIZE_4KB;
        Guard2.u8 = Guard1.u8 + (NoPages - 1) * SIZE_4KB;

        EnablePage (Guard1.Phys);
        EnablePage (Guard2.Phys);
        gBootServices->FreePages (Guard1.Phys, NoPages);

        return EFI_SUCCESS;
    } else {
        return (*gEfiFreePool)(Buffer);
    }
}

/**
  Constructor:

  Initialize the 4K environment for memory in the standard range (0x00000000 - 0x90000000).

  @param[in] ImageHandle  DxeMain image handle.
  @param[in] *SystemTable Pointer to system table.

  @return EFI_SUCCESS  (Returing FALSE will crater DXE_MAIN)

**/

EFI_STATUS
EFIAPI
EfiSpecialPoolConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
) {

    DEBUG ((DEBUG_LOAD,__FUNCTION__ " Efi Special Pool enabled\n"));
    gBootServices = SystemTable->BootServices;

    //
    //  Display Special Pool settings
    //
    DEBUG((DEBUG_INFO,"PcdSpecialPoolOverrun            = %d\n",PcdGetBool(PcdSpecialPoolOverrun)));
    DEBUG((DEBUG_INFO,"PcdSpecialPoolStackFaultHandling = %d\n",PcdGetBool(PcdSpecialPoolTrapPage0)));
    DEBUG((DEBUG_INFO,"PcdSpecialPoolTrapPage0          = %d\n",PcdGetBool(PcdSpecialPoolTrapPage0)));
    DEBUG((DEBUG_INFO,"PcdSpecialPoolTypes              = %8.8x\n",PcdGet32(PcdSpecialPoolTypes)));
    DEBUG((DEBUG_INFO,"PcdSpecialPoolStartRange         = %p\n",PcdGet64(PcdSpecialPoolStartRange)));
    DEBUG((DEBUG_INFO,"PcdSpecialPoolEndRange           = %p\n",PcdGet64(PcdSpecialPoolEndRange)));

    InitializeSpecialPool (SystemTable);  // Processor dependent code

    if (PcdGet64(PcdSpecialPoolStartRange) == 0L) {
        if (PcdGetBool(PcdSpecialPoolTrapPage0)) {
            DisablePage(0);
        }
    } else {
        if (PcdGetBool(PcdSpecialPoolTrapPage0)) {
            DEBUG((DEBUG_ERROR,"Invalid parameters. Trap Page0 ignored when StartRange > 0\n"));
        }
    }

    gEfiAllocatePool = gBootServices->AllocatePool;
    gEfiFreePool     = gBootServices->FreePool;

    gBootServices->AllocatePool = SpecialPool_AllocatePool;
    gBootServices->FreePool = SpecialPool_FreePool;
    gBootServices->Hdr.CRC32 = 0;
    gBootServices->CalculateCrc32(gBootServices,
                                  sizeof (EFI_BOOT_SERVICES),
                                 &gBootServices->Hdr.CRC32);

    if (PcdGetBool(PcdSpecialPoolStackFaultHandling)) {
        InitializeStackFaultHanding();
    }

    return EFI_SUCCESS;
}

