/** @file -- FmpRecoveryProgressIndicatorLibNull.c
# This is a NULL implementation of a library that can be used to display
# update progress to a user.

Copyright (c) 2017, Microsoft Corporation

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>

/**
  Initializer for the Recovery Progress Indicator.
  Will make sure that graphics support is located and ready and that the
  underlying libraries are also initialized.

  @param[in]  FillColor         Color for the indicator progress bar as it fills.
  @param[in]  BackgroundColor   Color for the background of the circle. Ideally, should
                                match the rest of the screen.

  @retval     EFI_SUCCESS       Progress Indicator is ready for display and updating.
  @retval     EFI_UNSUPPORTED   Could not prepare one of the required resources.

**/
EFI_STATUS
EFIAPI
InitRecoveryProgressIndicator (
  IN UINT32    FillColor,
  IN UINT32    BackgroundColor
  )
{
  DEBUG(( DEBUG_INFO, __FUNCTION__" - Library not implemented!\n" ));
  return EFI_UNSUPPORTED;
}


/**
  Simple function that updates the progress in a Recovery Progress Indicator
  circle. The circle MUST have been initialized prior to running.

  This function is mostly used as the "Progress" function for FMP interfaces.

  The first call to this function will draw the Indicator for the first time.

  @param[in]  ProgressPercent     1-100. You know how this works.

  @retval     EFI_SUCCESS     Indicator was updated and drawn.
  @retval     EFI_NOT_READY   Library must be initialized prior to update.

**/
EFI_STATUS
EFIAPI
UpdateRecoveryProgressIndicator (
  IN UINTN  ProgressPercent
  )
{
  DEBUG(( DEBUG_INFO, __FUNCTION__" - Library not implemented!\n" ));
  DEBUG(( DEBUG_INFO, __FUNCTION__" - Update progress = %d%%\n", ProgressPercent ));
  return EFI_NOT_READY;
} // FmpRecoveryProgress()
