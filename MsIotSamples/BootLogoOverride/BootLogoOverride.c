/** @file

Copyright (c) 2004 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

// Copyright (c) 2017 Microsoft
//
// boot logo override sample package -- boot logo override sample
//
// adapted from tianocore mdemodulepkg\universal\DriverSampleDxe and BootGraphicsResourceTableDxe
//


#include "BootLogoOverride.h"
#include "contoso_bmp.h"

#if 0
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *gSimpleTextOut;
#define DEBUG_OUT(x) gSimpleTextOut->OutputString(gSimpleTextOut, x)

#define DEBUG_OUT_INIT						\
    Status = gBS->HandleProtocol(		\
        gST->ConsoleOutHandle,			\
        &gEfiSimpleTextOutProtocolGuid,	\
        (VOID**)&gSimpleTextOut			\
    );									\
    if (EFI_ERROR(Status)) {			\
        return Status;					\
    }									\

#else
#define DEBUG_OUT(x)

#define DEBUG_OUT_INIT

#endif



EFI_EVENT                        mBootGraphicsReadyToBootEvent;
UINTN                            mBootGraphicsResourceTableKey = 0;
BOOLEAN                          g_fInitGraphics = FALSE;
EFI_GRAPHICS_OUTPUT_PROTOCOL    *gGraphicsOutput = NULL;
EFI_UGA_DRAW_PROTOCOL           *gUgaDraw = NULL;

EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE mBootGraphicsResourceTableTemplate = {
    {
        EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE_SIGNATURE,
        sizeof(EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE),
        EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE_REVISION,     // Revision
        0x00,  // Checksum will be updated at runtime
               //
               // It is expected that these values will be updated at EntryPoint.
               //
        { 0x00 },     // OEM ID is a 6 bytes long field
        0x00,       // OEM Table ID(8 bytes long)
        0x00,       // OEM Revision
        0x00,       // Creator ID
        0x00,       // Creator Revision
    },
    EFI_ACPI_5_0_BGRT_VERSION,         // Version
    EFI_ACPI_5_0_BGRT_STATUS_VALID,    // Status
    EFI_ACPI_5_0_BGRT_IMAGE_TYPE_BMP,  // Image Type
    0,                                 // Image Address
    0,                                 // Image Offset X
    0                                  // Image Offset Y
};

EFI_GRAPHICS_OUTPUT_BLT_PIXEL gBlackPixel = { 0 };

/**
Allocate EfiBootServicesData below 4G memory address.

This function allocates EfiBootServicesData below 4G memory address.

@param[in]  Size   Size of memory to allocate.

@return Allocated address for output.

**/
VOID *
BgrtAllocateBsDataMemoryBelow4G(
    IN UINTN       Size
) {
    UINTN                 Pages;
    EFI_PHYSICAL_ADDRESS  Address;
    EFI_STATUS            Status;
    VOID                  *Buffer;

    Pages = EFI_SIZE_TO_PAGES(Size);
    Address = 0xffffffff;

    Status = gBS->AllocatePages(
        AllocateMaxAddress,
        EfiBootServicesData,
        Pages,
        &Address
    );
    ASSERT_EFI_ERROR(Status);

    Buffer = (VOID *)(UINTN)Address;
    ZeroMem(Buffer, Size);

    return Buffer;
}


/**
This function calculates and updates an UINT8 checksum.

@param[in]  Buffer          Pointer to buffer to checksum.
@param[in]  Size            Number of bytes to checksum.

**/
VOID
BgrtAcpiTableChecksum(
    IN UINT8      *Buffer,
    IN UINTN      Size
) {
    UINTN ChecksumOffset;

    ChecksumOffset = OFFSET_OF(EFI_ACPI_DESCRIPTION_HEADER, Checksum);

    //
    // Set checksum to 0 first.
    //
    Buffer[ChecksumOffset] = 0;

    //
    // Update checksum value.
    //
    Buffer[ChecksumOffset] = CalculateCheckSum8(Buffer, Size);
}

/**
Install Boot Graphics Resource Table to ACPI table.

@return Status code.

**/
EFI_STATUS
InstallBootGraphicsResourceTable(
    VOID
) {
    EFI_STATUS                    Status;
    EFI_ACPI_TABLE_PROTOCOL       *AcpiTableProtocol;
    UINT8                         *ImageBuffer;

    //
    // Get ACPI Table protocol.
    //
    Status = gBS->LocateProtocol(&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTableProtocol);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    ImageBuffer = BgrtAllocateBsDataMemoryBelow4G(sizeof(contoso_logo));
    if (ImageBuffer == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    CopyMem(ImageBuffer, contoso_logo, sizeof(contoso_logo));

    mBootGraphicsResourceTableTemplate.ImageAddress = (UINT64)(UINTN)ImageBuffer;

    // NOTE: this assumes the bmp will fit and top/left justifies the bitmap.
    // it might be more desireable to look up the bmp size and the monitor size and crop or center as needed
    mBootGraphicsResourceTableTemplate.ImageOffsetX = 0;
    mBootGraphicsResourceTableTemplate.ImageOffsetY = 0;

    //
    // Update Checksum.
    //
    BgrtAcpiTableChecksum((UINT8 *)&mBootGraphicsResourceTableTemplate, sizeof(EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE));

    //
    // Publish Boot Graphics Resource Table.
    //
    Status = AcpiTableProtocol->InstallAcpiTable(
        AcpiTableProtocol,
        &mBootGraphicsResourceTableTemplate,
        sizeof(EFI_ACPI_5_0_BOOT_GRAPHICS_RESOURCE_TABLE),
        &mBootGraphicsResourceTableKey
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    return Status;
}


EFI_STATUS
InitGraphics() {
    EFI_STATUS Status = EFI_SUCCESS;

    if (g_fInitGraphics) {
        return Status;
    }

    //
    // Try to open GOP first
    //
    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGraphicsOutput);
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"Can't find GOP protocol\r\n");
        Status = gBS->LocateProtocol(&gEfiUgaDrawProtocolGuid, NULL, (VOID **)&gUgaDraw);
        if (EFI_ERROR(Status)) {
            DEBUG_OUT(L"Can't find UgaDraw protocol\r\n");
            return EFI_UNSUPPORTED;
        } else {
            if (gUgaDraw) {
                DEBUG_OUT(L"Using UgaDraw\r\n");
            } else {
                DEBUG_OUT(L"UgaDraw located but protocol null\r\n");
                return EFI_UNSUPPORTED;
            }
        }
    } else {
        if (gGraphicsOutput) {
            DEBUG_OUT(L"Using GOP\r\n");
        } else {
            DEBUG_OUT(L"GOP located but protocol null\r\n");
            return EFI_UNSUPPORTED;
        }
    }

    return Status;
}

EFI_STATUS
RefreshModeInfo(UINT32* SizeX, UINT32* SizeY) {
    EFI_STATUS Status = EFI_SUCCESS;
    UINT32     ColorDepth;
    UINT32     RefreshRate;


    if (gGraphicsOutput != NULL) {
        *SizeX = gGraphicsOutput->Mode->Info->HorizontalResolution;
        *SizeY = gGraphicsOutput->Mode->Info->VerticalResolution;
    } else if (gUgaDraw != NULL) {
        Status = gUgaDraw->GetMode(gUgaDraw, SizeX, SizeY, &ColorDepth, &RefreshRate);
        if (EFI_ERROR(Status)) {
            DEBUG_OUT(L"RefreshModeInfo uga GetMode failed\r\n");
            return EFI_UNSUPPORTED;
        }
    } else {
        DEBUG_OUT(L"RefreshModeInfo failing, no gop or uga\r\n");
        return EFI_UNSUPPORTED;
    }


    return Status;
}

EFI_STATUS
ClearScreen(UINT32 SizeX, UINT32 SizeY) {
    EFI_STATUS Status = EFI_SUCCESS;

    if (g_fInitGraphics) {
        return Status;
    }
    if (gGraphicsOutput || gUgaDraw) {
        g_fInitGraphics = TRUE;
    }

    if (gGraphicsOutput != NULL) {
        // clear the screen
        Status = gGraphicsOutput->Blt(
            gGraphicsOutput,
            &gBlackPixel,
            EfiBltVideoFill,
            0,
            0,
            0,
            0,
            (UINTN)SizeX,
            (UINTN)SizeY,
            0
        );
    } else if (gUgaDraw != NULL) {
        // clear the screen
        Status = gUgaDraw->Blt(
            gUgaDraw,
            (EFI_UGA_PIXEL *)&gBlackPixel,
            EfiUgaVideoFill,
            0,
            0,
            0,
            0,
            (UINTN)SizeX,
            (UINTN)SizeY,
            0
        );
    } else {
        return EFI_UNSUPPORTED;
    }

    return Status;
}

EFI_STATUS
DrawLogo(EFI_GRAPHICS_OUTPUT_BLT_PIXEL *logo_pixel,
        UINTN logo_size,
        UINTN logo_width,
        UINTN logo_height
) {
    EFI_STATUS Status = EFI_SUCCESS;
    UINT32     SizeOfX;
    UINT32     SizeOfY;
    INTN       DestX;
    INTN       DestY;

    Status = InitGraphics();
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"InitGraphics failed\r\n");
    }

    Status = RefreshModeInfo(&SizeOfX, &SizeOfY);
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"RefreshModeInfo failed\r\n");
    }

    //
    // Erase Cursor from screen
    //
    gST->ConOut->EnableCursor(gST->ConOut, FALSE);

    Status = ClearScreen(SizeOfX, SizeOfY);
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"ClearScreen failed\r\n");
    }

    DestX = (SizeOfX - logo_width) / 2;
    DestY = (SizeOfY - logo_height) / 2;

    if ((DestX >= 0) && (DestY >= 0)) {
        if (gGraphicsOutput != NULL) {
            // paint the logo
            Status = gGraphicsOutput->Blt(
                gGraphicsOutput,
                logo_pixel,
                EfiBltBufferToVideo,
                0,
                0,
                (UINTN)DestX,
                (UINTN)DestY,
                logo_width,
                logo_height,
                logo_width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
            );
        } else if (gUgaDraw != NULL) {
            // paint the logo
            Status = gUgaDraw->Blt(
                gUgaDraw,
                (EFI_UGA_PIXEL *)logo_pixel,
                EfiUgaBltBufferToVideo,
                0,
                0,
                (UINTN)DestX,
                (UINTN)DestY,
                logo_width,
                logo_height,
                logo_width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
            );
        } else {
            return EFI_UNSUPPORTED;
        }
    }

    return Status;
}

EFI_STATUS
EFIAPI
ShowLogo(
) {
    EFI_STATUS Status = EFI_SUCCESS;

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *logo_pixel = NULL;
    UINTN logo_size = 0;
    UINTN logo_width = 0;
    UINTN logo_height = 0;
    Status = DecodeImage(
        ImageFormatBmp,
        contoso_logo,
        sizeof(contoso_logo),
        &logo_pixel,
        &logo_size,
        &logo_width,
        &logo_height
    );
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"decode image failed\r\n");
        return Status;
    }


    Status = DrawLogo(logo_pixel, logo_size, logo_width, logo_height);
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"draw failed\r\n");
    }

    FreePool(logo_pixel);
    return Status;
}

VOID
EFIAPI
ProcessLogo(
) {
    EFI_STATUS Status = ShowLogo();
    if (EFI_ERROR(Status)) {
        DEBUG_OUT(L"showlogo failed\r\n");
        //return Status;
    }
    InstallBootGraphicsResourceTable();
}

VOID
EFIAPI
BgrtReadyToBootEventNotify(
    IN EFI_EVENT        Event,
    IN VOID             *Context
) {
	DEBUG_OUT(L"Boot Logo Override BgrtReadyToBootEventNotify\r\n");

    ProcessLogo();
}


/**
  Main entry for this driver.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.

**/
EFI_STATUS
EFIAPI
BootLogoOverrideInit (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
    EFI_STATUS Status = EFI_SUCCESS;

	DEBUG_OUT_INIT;
    DEBUG_OUT(L"Boot Logo Override driver\r\n");

    //
    // Register notify functions to install BGRT and paint logo  -- see PI spec v 1.3 Vol 2 DXE CIS 
    // we do this multiple times because some UEFI implementations overwrite our custom logo with their own built-in logo
    //

    Status = gBS->CreateEventEx(
        EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        BgrtReadyToBootEventNotify,
        NULL,
        &gEfiEventDxeDispatchGuid, // install BGRT and paint logo at first dxe load pass
        &mBootGraphicsReadyToBootEvent
    );

    Status = gBS->CreateEventEx(
        EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        BgrtReadyToBootEventNotify,
        NULL,
        &gEfiEndOfDxeEventGroupGuid, // install BGRT and paint logo when dxe phase complete
        &mBootGraphicsReadyToBootEvent
    );

    Status = gBS->CreateEventEx(
        EVT_NOTIFY_SIGNAL,
        TPL_CALLBACK,
        BgrtReadyToBootEventNotify,
        NULL,
        &gEfiEventReadyToBootGuid,  // install BGRT and paint logo when boot phase starts
        &mBootGraphicsReadyToBootEvent
    );
    DEBUG_OUT(L"Boot Logo Override Events Created\r\n");

    return Status;
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
BootLogoOverrideUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
    DEBUG_OUT(L"Boot Logo Override Unloading\r\n");

    return EFI_SUCCESS;
}
