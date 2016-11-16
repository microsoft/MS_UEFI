/** @file
  EFI Reset Notify PPI

@copyright
 Copyright (c) 2011 - 2014 Intel Corporation. All rights reserved
 Copyright (c) 2016, Microsoft Corporation

 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subject
 to additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef __RESET_NOTIFY_PPI_H__
#define __RESET_NOTIFY_PPI_H__

//
// Extern the GUID for PPI users.
//
extern EFI_GUID gEfiResetNotifyCallbackPpiGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_RESET_NOTIFY_CALLBACK_PPI EFI_RESET_NOTIFY_CALLBACK_PPI;

//
// Related Definitions
//
//

//
// Member functions
//
/**
  Execute call back function for System Resets.

  IMPORTANT NOTE: DO NOT perform a system reset in this callback.
  It is for information only so that drivers that manage resources that need
  to be informed of system resets can do so.

  @param[in]  ResetType         EFI_RESET_TYPE for the reset being requested.
  @param[in]  ResetStatus       Honestly... just check the UEFI spec...
  @param[in]  DataSize          This function mirrors the EFI_RESET_SYSTEM function prototype.
  @param[in]  ResetData

**/
typedef
VOID
(EFIAPI *EFI_RESET_NOTIFY_CALLBACK) (
  IN EFI_RESET_TYPE               ResetType,
  IN EFI_STATUS                   ResetStatus,
  IN UINTN                        DataSize,
  IN CONST VOID                   *ResetData OPTIONAL
  );

struct _EFI_RESET_NOTIFY_CALLBACK_PPI {
  EFI_RESET_NOTIFY_CALLBACK  ResetNotifyCallback;
};

#endif
