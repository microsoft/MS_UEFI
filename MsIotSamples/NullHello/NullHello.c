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
// boot logo override sample package -- 'hello world' skeleton driver to validate load/unload from efi shell is working
//
// adapted from tianocore mdemodulepkg\universal\DriverSampleDxe
//


#include "NullHello.h"


/**
  Main entry for this driver.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.

**/
EFI_STATUS
EFIAPI
NullHelloInit (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *SimpleTextOut;
    Status = gBS->HandleProtocol(
        gST->ConsoleOutHandle,
        &gEfiSimpleTextOutProtocolGuid,
        (VOID**)&SimpleTextOut
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // NOTE: since this isn't a real driver it doesn't verify the mode is correct for console output to succeed.
    // you may need to manually set it with the 'mode' command from efi shell before running this code in order to see the output

    Status = SimpleTextOut->OutputString(SimpleTextOut, L"Hello from a driver\n");

    return Status;
}

/**
  Unloads the application and its installed protocol.

  @param[in]  ImageHandle       Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
**/
EFI_STATUS
EFIAPI
NullHelloUnload (
  IN EFI_HANDLE  ImageHandle
  )
{

    return EFI_SUCCESS;
}
