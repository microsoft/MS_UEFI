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
#include <Uefi.h>
#include <PiPei.h>
#include <Ppi/PlatformSpecificResetFilter.h>
#include <Ppi/PlatformSpecificResetHandler.h>
#include <Library/PeiServicesLib.h>
#include "ResetFiltersAndHandlersTestCommon.h"

EFI_STATUS
EFIAPI
TestConstructor (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
)
{
  EFI_STATUS       Status;
  EDKII_PLATFORM_SPECIFIC_RESET_FILTER_PPI  *ResetFilter;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PPI *ResetHandler;

  Status = PeiServicesLocatePpi (&gEdkiiPlatformSpecificResetFilterPpiGuid, 0, NULL, (VOID **)&ResetFilter);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (&gEdkiiPlatformSpecificResetHandlerPpiGuid, 0, NULL, (VOID **)&ResetHandler);
  ASSERT_EFI_ERROR (Status);
  //
  // Uncomment Test1 or Test2.
  //
//  Test1 (ResetFilter, ResetHandler);

  Test2 (ResetFilter, ResetHandler);
  return EFI_SUCCESS;
}