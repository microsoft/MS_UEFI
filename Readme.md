# On Screen Notifications
## Copyright

Copyright (c) 2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

**OnScreenNotifications**

The MsGraphicsPkg provides the necessary functions to display on screen notifications in the format of colored bars and bitmap icons. This can be used to give developers 
a quick and easy visual indication of what modes are enabled/disabled on a particular device.

This is implemented by using a UINT bitmask stored in a HOB to signify what modes are active.  You can add active modes to the HOB using the OnScreenNotificationLib.

Once you are ready to display the notifications you will need to pass in the Frame Buffer, Pixels per scan line, Width, and Height of the frame buffer to the DisplayOnScreenNotifications function of the BaseDisplayOnScreenNotificationLib


Addtional Notes:

**MsGraphicsPkg\Include\Library\MsOnScreenNotificationLib.h**
___
In this header you should rename the modes to reflect what you are trying to display. We have kept SecureBoot as an example of how to use the OnScreenNotification library and how to incorporate an icon into the colored bar


**MsGraphicsPkg\Include\Library\BaseDisplayOnScreenNotificationLib.c**
___
If you wish to change the colors used, fill options, or the height of the banners you will need to modify the #defines in BaseDisplayOnScreenNotificationLib.c

To add new modes or remove unsupported modes you can modify the mSupportedNotifications array in BaseDisplayOnScreenNotificationLib.c