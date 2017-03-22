# Bds changes for platform control

## Additional callbacks
### PlatformBootManagerOnDemandConInConnect()
PlatformBootManageOnDemandConInConnect is called propr to connecting the Console device paths.  This is used by Surface to connect the XHCI controller for USB devices, the integrated touch driver, and non USB keyboards and pointer devices.
### PlatformBootManagerEntry() 
PlatformBootManageEntry is called at the beginning of BDS in order to handle a few things before END_OF_DXE.  Surface uses this callback to signal a platform event for DXE drivers to comple accesses to lockable variables. In addition, Surface uses this to latch secure boot violations from the previous boot. 
### PlatformBootManagerProcessBootCompletion()
PlatformBootManageProcessBootCompletion is called if a started image returns to Bds. Surface captures any Secure Boot violations, and restore the console to native mode for the next boot option.
### PlatformBootManagerPriorityBoot()
PlatformBootManagePriority boot is called for to allow the platform checks to boot a particular boot option should be booted.  This is similar to HotKey boot but can be some other input mechanism (GPIO, etc). 
### PlatformBootManagerDeadloop()
PlatformBootManagerDeadloop is called before BdsEntry calls EfiDeadLoop().  This routine implements the platform desired method to handle boot failure.  Surface restarts to the setup page.

## New events signalled from UefiBootManagerLib
+ PreReadyToBoot
The Surface PreReadyToBoot locks boot variables and enables our On Screen Keyboard.
+ PostReadyToBoot
The Surface PostReadyToBoot publishes our FACS HardwareSignature update to accout for the firmware version and the heap information.
## Two PCDs to control default behavior for BdsDxe
+ PlatformRecoverySupported
Allow removing support for PlatformRecovery####
+ BootManagerInBootOrder
Allow for BootManager to NOT reside in the Boot Order.