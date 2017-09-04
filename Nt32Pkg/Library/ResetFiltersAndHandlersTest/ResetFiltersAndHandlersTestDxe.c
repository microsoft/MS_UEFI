/** @file
  Provides a platform-specific method to enable Secure Boot Custom Mode setup.

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
#include <PiPei.h>
#include <Protocol/PlatformSpecificResetFilter.h>
#include <Protocol/PlatformSpecificResetHandler.h>

#include <Library/UefiBootServicesTableLib.h>
#include "ResetFiltersAndHandlersTestCommon.h"

EFI_STATUS
EFIAPI
TestConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS       Status;
  EDKII_PLATFORM_SPECIFIC_RESET_FILTER_PROTOCOL  *ResetFilter;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL *ResetHandler;

  Status = gBS->LocateProtocol (&gEdkiiPlatformSpecificResetFilterProtocolGuid, NULL, (VOID **)&ResetFilter);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEdkiiPlatformSpecificResetHandlerProtocolGuid, NULL, (VOID **)&ResetHandler);
  ASSERT_EFI_ERROR (Status);

  //
  // Uncomment Test1 or Test2.
  //
  //  Test1 (ResetFilter, ResetHandler);

  Test2 (ResetFilter, ResetHandler);
  return EFI_SUCCESS;
}