/**
Implement UnitTestBootUsbLib using USB Class Boot option.  This should be industry standard and should
work on all platforms

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
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/DevicePath.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS
EFIAPI
SetUsbBootNext(
  VOID
)
{
  EFI_STATUS                      Status;
  EFI_BOOT_MANAGER_LOAD_OPTION    NewOption;
  UINT32                          Attributes;
  UINT8                          *OptionalData = NULL;
  UINT32                          OptionalDataSize = 0;
  UINT16                         BootNextValue = 0xABCD;  // this should be a safe number...
  USB_CLASS_DEVICE_PATH           UsbDp;
  EFI_DEVICE_PATH_PROTOCOL       *DpEnd = NULL;
  EFI_DEVICE_PATH_PROTOCOL       *Dp = NULL;
  BOOLEAN                        NewOptionValid = FALSE;

  UsbDp.Header.Length[0] = (UINT8)(sizeof(USB_CLASS_DEVICE_PATH) & 0xff);
  UsbDp.Header.Length[1] = (UINT8)(sizeof(USB_CLASS_DEVICE_PATH) >> 8);
  UsbDp.Header.Type = MESSAGING_DEVICE_PATH;
  UsbDp.Header.SubType = MSG_USB_CLASS_DP;
  UsbDp.VendorId = 0xFFFF;
  UsbDp.ProductId = 0xFFFF;
  UsbDp.DeviceClass = 0xFF;
  UsbDp.DeviceSubClass = 0xFF;
  UsbDp.DeviceProtocol = 0xFF;

  Attributes = LOAD_OPTION_ACTIVE;

  DpEnd = AppendDevicePathNode(NULL, NULL);
  if (DpEnd == NULL)
  {
    DEBUG((DEBUG_ERROR, __FUNCTION__ ": Unable to create device path.  DpEnd is NULL.\n"));
    goto CLEANUP;
  }

  Dp = AppendDevicePathNode(DpEnd, (EFI_DEVICE_PATH_PROTOCOL *)&UsbDp);  //@MRT --- Is this memory leak becasue we lose the old Dp memory
  if (Dp == NULL)
  {
    DEBUG((DEBUG_ERROR, __FUNCTION__ ": Unable to create device path.  Dp is NULL.\n"));
    goto CLEANUP;
  }

  Status = EfiBootManagerInitializeLoadOption(
    &NewOption,
    (UINTN) BootNextValue, 
    LoadOptionTypeBoot,
    Attributes,
    L"Generic USB Class Device",
    Dp,
    OptionalData,
    OptionalDataSize
  );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, __FUNCTION__ ": Error creating load option.  Status = %r\n", Status));
    goto CLEANUP;
  }
  
  NewOptionValid = TRUE;  
  DEBUG((DEBUG_VERBOSE, __FUNCTION__ ": Generic USB Class Device boot option created.\n"));
  Status = EfiBootManagerLoadOptionToVariable(&NewOption);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, __FUNCTION__ ": Error Saving boot option NV variable. Status = %r\n", Status));
    goto CLEANUP;
  }

  //Set Boot Next
  Status = gRT->SetVariable(L"BootNext",
    &gEfiGlobalVariableGuid,
    (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE),
    sizeof(BootNextValue),
    &(BootNextValue));

  DEBUG((DEBUG_VERBOSE, __FUNCTION__" - Set BootNext Status (%r)\n", Status));

CLEANUP:
  if (Dp != NULL)
  {
    FreePool(Dp);
  }

  if (DpEnd != NULL)
  {
    FreePool(DpEnd);
  }

  if (NewOptionValid)
  {
    EfiBootManagerFreeLoadOption(&NewOption);
  }

  return Status;
}