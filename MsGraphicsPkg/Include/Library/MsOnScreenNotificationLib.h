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

#ifndef __MS_ON_SCREEN_NOTIFICATION_LIB_H__
#define __MS_ON_SCREEN_NOTIFICATION_LIB_H__

//
//DEFINE the possible on screen notifications here.
// Current API is defined as a 31bit bitmask  (bit 32 is reserved for MAX)
//

#define MS_ON_SCREEN_NOTIFICATION_SECUREBOOT_OFF            (1 << (0))
#define MS_ON_SCREEN_NOTIFICATION_MODE_1                    (1 << (1))
#define MS_ON_SCREEN_NOTIFICATION_MODE_2                    (1 << (2))
#define MS_ON_SCREEN_NOTIFICATION_MODE_3                    (1 << (3))
#define MS_ON_SCREEN_NOTIFICATION_MODE_4                    (1 << (4))
#define MS_ON_SCREEN_NOTIFICATION_MODE_5                    (1 << (5))
#define MS_ON_SCREEN_NOTIFICATION_MODE_6                    (1 << (6))

#define MS_ON_SCREEN_NOTIFICATION_MAX                       (1 << (31))



typedef UINT32  MS_ON_SCREEN_NOTIFICATION;


/**
Function to Get Currently triggered OnScreenNotifications
@retval the currently triggered OnScreenNotifications
**/
MS_ON_SCREEN_NOTIFICATION
EFIAPI
GetOnScreenNotifications();

/**
Function to Add additional triggered OnScreenNotifications

@param AdditionalNotifications - additional notifications to set active
@retval Status of operation.  EFI_SUCCESS on successful update.
**/
RETURN_STATUS
EFIAPI
AddOnScreenNotifications(
MS_ON_SCREEN_NOTIFICATION AdditionalNotifications
);


#endif
