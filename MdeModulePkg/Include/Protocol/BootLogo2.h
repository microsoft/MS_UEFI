/**

Copyright (c) 2016, Microsoft Corporation

All rights reserved.
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

// MS_CHANGE: New file for BootLogo2 protocol

#ifndef _BOOT_LOGO2_H_
#define _BOOT_LOGO2_H_

#include <Protocol/GraphicsOutput.h>

//
// Forward reference for pure ANSI compatability
//
typedef struct _EFI_BOOT_LOGO_PROTOCOL2 EFI_BOOT_LOGO_PROTOCOL2;

/**
  Update information of logo image drawn on screen.

  @param  This           The pointer to the Boot Logo protocol 2 instance.
  @param  BltBuffer      The BLT buffer for logo drawn on screen. If BltBuffer
                         is set to NULL, it indicates that logo image is no
                         longer on the screen.
  @param  DestinationX   X coordinate of destination for the BltBuffer.
  @param  DestinationY   Y coordinate of destination for the BltBuffer.
  @param  Width          Width of rectangle in BltBuffer in pixels.
  @param  Height         Hight of rectangle in BltBuffer in pixels.

  @retval EFI_SUCCESS             The boot logo information was updated.
  @retval EFI_INVALID_PARAMETER   One of the parameters has an invalid value.
  @retval EFI_OUT_OF_RESOURCES    The logo information was not updated due to
                                  insufficient memory resources.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_BOOT_LOGO2)(
  IN EFI_BOOT_LOGO_PROTOCOL2           *This,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL     *BltBuffer       OPTIONAL,
  IN UINTN                             DestinationX,
  IN UINTN                             DestinationY,
  IN UINTN                             Width,
  IN UINTN                             Height
  );


/**
  Get the location of the boot logo on the screen. 

  @param This             The pointer to the Boot Logo Protocol 2 instance
  @param DestinationX     Pointer to a UINTN that will be updated with X start position
  @param DestinationY     Pointer to a UINTN that will be updated with Y start position
  @param Width            Pointer to a UINTN that will be updated with width
  @param Height           Pointer to a UINTN that will be updated with Height

  @retval EFI_SUCCESS     The pointers updated with valid screen coordinates
  @retval EFI_NOT_READY   The pointers not updated.  no boot logo has been set yet
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_BOOT_LOGO2)(
IN EFI_BOOT_LOGO_PROTOCOL2           *This,
IN UINTN                             *DestinationX,
IN UINTN                             *DestinationY,
IN UINTN                             *Width,
IN UINTN                             *Height
);



struct _EFI_BOOT_LOGO_PROTOCOL2 {
  EFI_SET_BOOT_LOGO2        SetBootLogo;
  EFI_GET_BOOT_LOGO2        GetBootLogo;
};

extern EFI_GUID gEfiBootLogoProtocol2Guid;

#endif
