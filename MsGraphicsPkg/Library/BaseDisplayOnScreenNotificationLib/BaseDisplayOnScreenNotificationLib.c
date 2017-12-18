/** @file
Library used to display on screen notifications.

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

#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MsOnScreenNotificationLib.h>
#include <Library/MsDisplayOnScreenNotificationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/GraphicsOutput.h>  //structure defs
#include <Library/MemoryAllocationLib.h>
#include <UiPrimitiveSupport.h>
#include <Library/UiRectangleLib.h>

//Secure boot unlock icon
#include "Resources/UnlockBitmap.h"

//Height of single banner is percent of total screen height
#define HEIGHT_OF_SINGLE_BANNER (8)

//These widths are percent of banner height
#define STRIPE_WIDTH (20)
#define CHECKERBOARD_WIDTH (20)

//
//Colors from RainBow wiki
//
#define COLOR_RED       (0xFFfb0200)
#define COLOR_ORANGE    (0xFFfd6802)
#define COLOR_YELLOW    (0xFFffef00)
#define COLOR_GREEN     (0xFF00ff03)
#define COLOR_BLUE      (0xFF0094fb)
#define COLOR_INDIGO    (0xFF4500f7)
#define COLOR_VIOLET    (0xFF9c00ff)

#define COLOR_GREY      (0xFFC0C0C0)
#define COLOR_BLACK     (0xFF000000)
#define COLOR_WHITE     (0xFFFFFFFF)

//
// List of supported notifications.
// In order you want them displayed.
//
MS_ON_SCREEN_NOTIFICATION mSupportedNotifications[] = {
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_SECUREBOOT_OFF,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MODE_1,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MODE_2,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MODE_3,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MODE_4,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MODE_5,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MODE_6,
  (MS_ON_SCREEN_NOTIFICATION)MS_ON_SCREEN_NOTIFICATION_MAX  //this needs to be the last one
};


/**
Helper debug method to print out what notifications are set
**/
VOID
PrintNotificationValues(MS_ON_SCREEN_NOTIFICATION Notifications)
{
  DEBUG((DEBUG_INFO, "On Screen Notifications: \n"));
  if (Notifications & MS_ON_SCREEN_NOTIFICATION_SECUREBOOT_OFF)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_SECUREBOOT_OFF\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MODE_1)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MODE_1\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MODE_2)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MODE_2\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MODE_3)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MODE_3\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MODE_4)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MODE_4\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MODE_5)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MODE_5\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MODE_6)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MODE_6\n"));
  }

  if (Notifications & MS_ON_SCREEN_NOTIFICATION_MAX)
  {
    DEBUG((DEBUG_INFO, "\tMS_ON_SCREEN_NOTIFICATION_MAX\n"));
  }
}


/**
Function to Display all Active On Screen Notifications

@param FrameBufferBase   - Address of point 0,0 in the frame buffer
@param PixelsPerScanLine - Number of pixels per scan line.
@param WidthInPixels     - Number of Columns in FrameBuffer
@param HeightInPixels    - Number of Rows in FrameBuffer
**/
VOID
EFIAPI
DisplayOnScreenNotifications(
IN  UINT8* FrameBufferBase,
IN  INT32  PixelsPerScanLine,
IN  INT32  WidthInPixels,
IN  INT32  HeightInPixels
)
{
  MS_ON_SCREEN_NOTIFICATION Notifications = 0;
  MS_ON_SCREEN_NOTIFICATION* SupportedNotification = mSupportedNotifications;
  POINT ul;
  INT32 SingleBannerHeight = ((HeightInPixels * HEIGHT_OF_SINGLE_BANNER) / 100);


  Notifications = GetOnScreenNotifications();
  PrintNotificationValues(Notifications);

  ul.X = 0;
  ul.Y = 0;

  while ((*SupportedNotification != MS_ON_SCREEN_NOTIFICATION_MAX) && (Notifications > 0))
  {
    if (Notifications & *SupportedNotification)  //loop thru array of supported notifications
    {
      UI_STYLE_INFO si;
      si.Border.BorderWidth = 0;
      si.IconInfo.Width = 0;
      si.IconInfo.Height = 0;
      si.IconInfo.PixelData = NULL;

      if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_SECUREBOOT_OFF)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_RED;
        si.IconInfo.Width = SECURE_BOOT_UNLOCKED_BITMAP_WIDTH;
        si.IconInfo.Height = SECURE_BOOT_UNLOCKED_BITMAP_HEIGHT;
        si.IconInfo.Placement = MIDDLE_CENTER;
        si.IconInfo.PixelData = (UINT32*) &SecureBootUnlockedBitmap[0];

      }
      else if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_MODE_1)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_VIOLET;
      }
      else if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_MODE_2)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_GREEN;
      }
      else if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_MODE_3)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_BLUE;
      }
      else if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_MODE_4)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_INDIGO;
      }
      else if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_MODE_5)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_ORANGE;
      }
      else if (*SupportedNotification & MS_ON_SCREEN_NOTIFICATION_MODE_6)
      {
        si.FillType = FILL_SOLID;
        si.FillTypeInfo.SolidFill.FillColor = COLOR_YELLOW;
      }
      else
      {
        //
        // Catch any supported notification that doesn't have draw routine.
        // Generally this would mean developer forgot to update this part of the library to support
        //
        DEBUG((DEBUG_ERROR, "Notification 0x%X does not have code to support drawing.\n", *SupportedNotification));
        SupportedNotification++;
        continue;
      }

      UI_RECTANGLE* rect = new_UI_RECTANGLE(&ul, FrameBufferBase, PixelsPerScanLine, (UINT16)WidthInPixels, SingleBannerHeight, &si);
      DrawRect(rect);
      delete_UI_RECTANGLE(rect);
      ul.Y += SingleBannerHeight;
      Notifications -= *SupportedNotification;  //subtract notification so that we can break early.
    }  //close if notification is supported
    SupportedNotification++;
  }  //close while loop going thru each notification
}