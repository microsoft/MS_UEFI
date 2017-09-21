/** @file
  Common test code.

  Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/ResetUtilityLib.h>
#include <Library/PrintLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Protocol/ResetNotification.h>
VOID
Test1 (
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetFilter,
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetHandler
);
VOID
Test2 (
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetFilter,
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetHandler
);