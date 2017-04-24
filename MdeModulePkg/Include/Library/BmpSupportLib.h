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

// MS_CHANGE: BMP support for capsule processing

#ifndef __BMP_SUPPORT_LIB_H__
#define __BMP_SUPPORT_LIB_H__

/**
Translate a *.BMP graphics image to a GOP blt buffer. If a NULL Blt buffer
is passed in a GopBlt buffer will be allocated by this routine. If a GopBlt
buffer is passed in it will be used if it is big enough.

@param  BmpImage      Pointer to BMP file
@param  BmpImageSize  Number of bytes in BmpImage
@param  GopBlt        Buffer containing GOP version of BmpImage.
@param  GopBltSize    Size of GopBlt in bytes.
@param  PixelHeight   Height of GopBlt/BmpImage in pixels
@param  PixelWidth    Width of GopBlt/BmpImage in pixels

@retval EFI_SUCCESS           GopBlt and GopBltSize are returned.
@retval EFI_UNSUPPORTED       BmpImage is not a valid *.BMP image
@retval EFI_BUFFER_TOO_SMALL  The passed in GopBlt buffer is not big enough.
GopBltSize will contain the required size.
@retval EFI_OUT_OF_RESOURCES  No enough buffer to allocate.

**/
RETURN_STATUS
EFIAPI
TranslateBmpToGopBlt(
IN      VOID      *BmpImage,
IN      UINTN     BmpImageSize,
IN OUT  VOID      **GopBlt,
IN OUT  UINTN     *GopBltSize,
OUT     UINTN     *PixelHeight,
OUT     UINTN     *PixelWidth
);


#endif
