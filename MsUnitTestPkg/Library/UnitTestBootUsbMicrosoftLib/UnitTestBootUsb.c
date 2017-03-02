/**
Implement UnitTestBootUsbLib using internal microsoft uefi boot usb boot option 

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

#include <PiDxe.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>

EFI_STATUS
EFIAPI
SetUsbBootNext (
  VOID
  )
{
  EFI_STATUS  Status;
  INT16        BootOptionIndex;
  CHAR16      BootOptionName[30];
  CHAR16      *BootOptionIndexChar = NULL;
  UINT8       *OptionBuffer = NULL;
  UINTN       OptionBufferSize = 0, VariableSize = 0;
  BOOLEAN     IsUsbOptionFound = FALSE;

  StrCpy(BootOptionName, L"Boot000");
  BootOptionIndexChar = BootOptionName + StrLen(BootOptionName);

  //
  // Walk through each of the first 10 boot options looking for the
  // generic USB boot option.
  for (BootOptionIndex = 0; BootOptionIndex < 10; BootOptionIndex++)
  {
    // Construct the BootOption name for this boot option.
    // Do this by altering the last character of the name.
    UnicodeValueToString(BootOptionIndexChar, 0, (INT64)BootOptionIndex, 1);

    // Attempt to retrieve the option.
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Checking for %s...\n", BootOptionName ));
    VariableSize = OptionBufferSize;
    Status = gRT->GetVariable( BootOptionName,
                               &gEfiGlobalVariableGuid,
                               NULL,
                               &VariableSize,
                               OptionBuffer );
    // If we need a larger buffer, let's do that now.
    if (Status == EFI_BUFFER_TOO_SMALL)
    {
      // Free the existing buffer.
      if (OptionBuffer != NULL)
      {
        FreePool( OptionBuffer );
      }
      // Allocate a larger buffer.
      OptionBuffer = AllocatePool( VariableSize );
      // If you fail to... we've gotta get out of here.
      if (OptionBuffer == NULL)
      {
        DEBUG(( DEBUG_ERROR, __FUNCTION__" - Failed to allocate memory for Boot option variable %s...\n", BootOptionName ));
        return EFI_OUT_OF_RESOURCES;
      }
      OptionBufferSize = VariableSize;

      // Now that we've got a larger buffer, try that again.
      Status = gRT->GetVariable( BootOptionName,
                                 &gEfiGlobalVariableGuid,
                                 NULL,
                                 &VariableSize,
                                 OptionBuffer );
    }

    // If we failed to retrieve this option... move on with your life.
    if (EFI_ERROR( Status ))
    {
      DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Failed to locate option (%r). Moving on.\n", Status ));
      continue;
    }

    // If we found the option, check to see whether this is the generic USB option.
    // For now... we're lazy... just check the last four bytes (which should be the
    // optional data) and see whether it's "USB".
    if (VariableSize > 4 &&
        OptionBuffer[VariableSize - 4] == 'U' && OptionBuffer[VariableSize - 3] == 'S' &&
        OptionBuffer[VariableSize - 2] == 'B' && OptionBuffer[VariableSize - 1] == 0x00 )
    {
      IsUsbOptionFound = TRUE;
      break;
    }
  }

  //
  // If the correct boot option was found,
  // set it to the BootNext variable.
  if (IsUsbOptionFound)
  {
    Status = gRT->SetVariable( L"BootNext",
                               &gEfiGlobalVariableGuid,
                               (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE),
                               sizeof( BootOptionIndex ),
                               &BootOptionIndex );
    DEBUG(( DEBUG_VERBOSE, __FUNCTION__" - Set BootNext Status (%r)\n", Status ));
  }
  else
  {
    DEBUG(( DEBUG_WARN, __FUNCTION__" - Could not find generic USB boot option.\n" ));
    Status = EFI_NOT_FOUND;
  }

  // Always put away your toys.
  if (OptionBuffer != NULL)
  {
    FreePool( OptionBuffer );
  }

  return Status;  
} // SetUsbBootNext()
