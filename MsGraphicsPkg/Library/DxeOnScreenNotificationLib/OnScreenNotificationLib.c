/** @file
Functions used to support Getting and Setting Microsoft On Screen Notification alerts.

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

#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/MsOnScreenNotificationLib.h>
#include <Library/DebugLib.h>

// {B85596A7-FE3A-461A-B4C3-378CC99AB4EB}
extern GUID gOnScreenNotificationHobGuid;


/**
Function to Get Currently triggered OnScreenNotifications
@retval the currently triggered OnScreenNotifications
**/
MS_ON_SCREEN_NOTIFICATION
EFIAPI
GetOnScreenNotifications()
{
  UINT8* Hob;
  MS_ON_SCREEN_NOTIFICATION r = 0;
  Hob = GetFirstGuidHob(&gOnScreenNotificationHobGuid);
  if (Hob != NULL)
  {
    if (GET_GUID_HOB_DATA_SIZE(Hob) < sizeof(MS_ON_SCREEN_NOTIFICATION))
    {
      DEBUG((DEBUG_ERROR, "Found ON_SCREEN_NOTIFICATION Hob but invalid size. 0x%X\n", GET_GUID_HOB_DATA_SIZE(Hob)));
    }
    else
    {
      r = *((MS_ON_SCREEN_NOTIFICATION*) (GET_GUID_HOB_DATA(Hob)));
    }
  }
  return r;
}

/**
Function to Add additional triggered OnScreenNotifications

@param AdditionalNotifications - additional notifications to set active
@retval Status of operation.  EFI_SUCCESS on successful update.
**/
RETURN_STATUS
EFIAPI
AddOnScreenNotifications(MS_ON_SCREEN_NOTIFICATION AdditionalNotifications)
{
  DEBUG((DEBUG_ERROR, "Attempt to add OnScreenNotification after PEI phase"));
  ASSERT(FALSE);
  return EFI_UNSUPPORTED;
}

