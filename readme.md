Microsoft IoT Logo Samples
==========================

Copyright (c) 2016-2017, Microsoft Corporation

## About

These are samples that show how to setup a UEFI build environment and create a loadable EFI driver that overrides the windows boot logo.
These samples are intended to work with the http://tianocore.org and it's EDK which is located at https://github.com/tianocore/tianocore.github.io/wiki/EDK-II

For IoT the expectation is that you will be using secure boot and signed drivers.  For more information about this along with scripts for generating your own keys see https://docs.microsoft.com/en-us/windows/iot-core/secure-your-device/securebootandbitlocker

Additional References:
* UEFI 2.6 or later specs that describe loadable drivers and the protocols used here are obtainable from http://www.uefi.org/
* ACPI 5.0 or later specs that define the Boot Graphics Resource Table ('BGRT') are also obtained from the same UEFI website.


There are 3 samples in this package.  
1. One is a completely empty driver named NullSampleDxe that may be helpful in verifying your build environment and gives you a blank driver template to start from since, at the time of this writing, there wasn't a blank skeletion in TianoCore.
2. The second is a simple driver named NullHello that uses the most primitive I/O protocol to print a hello message on the console.  This is useful for validating that your build is producing a loadable driver and it's actually getting loaded, run, and the binary signing is configured properly.
3. The last sample named BootLogoOverride shows a full blown driver that displays a Contoso logo during boot in place of the windows logo.

There are a set of prerequisites that are required in order boot logo override to work.  
* UEFI 2.6 or later
* The UEFI being used must support loadable EFI drivers.  Not all UEFI implementations support installing loadable DXE phase drivers.  If the UEFI you're trying to use doesn't allow this then you'll have to use a different approach such as changing boards or licensing the entire UEFI source for your board.
* Your board design must provide a non-volatile EFI partition that provides you a place to install the loadable driver that's accessible early enough in the boot process that the driver can be loaded when it's needed.

Microsoft IoT Logo Sample
=========================

The sample embeds the bitmap data as static data in the driver binary itself.  This is the simplest and most secure way to do this since the driver signing protects the bitmap data from tampering and designs away security attack vectors that target the bitmap data.  There are other approaches that are technically possible that load a .jpeg or .png from a separate file on the same volume as the driver or some other location the driver can reach.  However, if you do this you need to carefully analyze the security ramifications.  Since this data is now outside the driver signing enforcement provided by the loader any buffer overrun or other bug could allow the UEFI to be hijacked and compromise the security of the entire system.  So, at a minimum you should provide a cryptographically sound signing process to verify the external bitmap file has not been tampered with.  Unless there's a very compelling reason to do all this extra work it seems easier and safer just to delegate this to the core UEFI implementation by embedding the bitmap data in the driver binary itself.

***
IMPORTANT: In order to test secure boot, be sure that you have the ability to  reflash the base UEFI firmware on your device or have other directions to reset the firmware to factory state.  On many UEFI implementations turning on secure boot is a one way switch and once you have turned it on you cannot turn it off.  This makes the device more secure since malware can't simply disable secure boot.  But, it means that if you configure your device incorrectly and then turn on secure boot you may not be able to fix it.  Some UEFI's have system setup UI for clearing the keys and resetting secure boot to original state. Other UEFI's require you to reflash the base UEFI to turn off secure boot and clear the NV vars.  Yet other UEFI's require you to reflash to turn off secure boot first and then use the system setup UI to clear the key store nv variables.
***


Toplevel process for building and testing your driver
-----------------------------------------------------
1) Download and install the TianoCore EDK
2) Verify you can build, install, and run the driver without worrying about signing or turning on secure boot yet.
3) Save this driver for testing later.
4) Update the placeholder insecure codesigning.pfx with the real code signing key and rebuild the driver. directions for one approach to creating a real key are at: https://docs.microsoft.com/en-us/windows/iot-core/secure-your-device/securebootandbitlocker .  
Larger organizations likely already have a process in place for official and test codesigning and those process owners would need to be consulted for details about how to integrate an .efi driver build into that process as needed.
5) Install the keys(also called certs) for secure boot into the correct UEFI NV variables.
6) Install the new correctly signed driver.
7) turn on secure boot and verify that it works and the override logo is displayed.
8) Replace the correctly signed driver with the previously saved test driver that is not signed with a cert that matches a key installed in the UEFI key store.
9) Verify that the incorrect driver is ignored, the override logo doesn't appear, and the normal windows logo appears instead.

QuickStart Tips for building with TianoCore 
-------------------------------------------
* Clone the TianoCore repo
* Clone the samples from the repo containing this readme file to %local_repo_root%\MsIotSamples
* Run edksetup.bat
* The first time this is run it will copy a set of files into %local_repo_root%\Conf
* Update prefix.bat for your environment pointing to correct tool drive letters, etc.
* Edit target.txt to specify desired architecture, toolchain, etc. See tianocore docs and comments in the target.txt file for details
* Change platform to samples by editing target.txt and making:

        ACTIVE_PLATFORM=MsIotSamples\MsIotSamples.dsc
* Note: An insecure placeholder codesigning.pfx is placed here so that developers that don't care about secure boot can proceed with their normal routine in a transparent manner.  When you get to the steps that involve proper signing you will need to replace this with a real codesigning.pfx that matches the keys you install in your uefi key store and update your environment with the password for this file.

Installing and testing your initial insecure driver (step #2 above)
-------------------------------------------------------------------
1) Copy the nullhello.efi file and bootlogooverride.efi files to an SD card, USB stick, or other storage accessible to your IoT board. NOTE: This must be formatted with some UEFI supported FAT variant(typically FAT32 or exFAT).
2) Plug the USB stick into your iot board
3) Boot your board to EFI shell.  The method for this, usually keyboard Fkeys, vary across UEFI implementations.
4) Find the EFI partition for your device.  This is the partition that contains bootmgr.efi and the boot configuration store BCD.
Often this is fs0: But this varies depending on the storage available on the device.
5) Find the partition with your test driver
Often this is fs1: or fs2:
Since the windows volume will be formatted with NTFS it won't show up from the efi shell as an accessible volume and won't get assigned an fsX: designation.  NOTE: For the remainder of this documentation we will assume your efi partition is fs0: and your portable storage with your driver is fs1:. do:

        load fs1:\nullhello.efi

6) Verify that you see the hello message from the driver on your console. On some UEFIs you can then do load fs1:\bootlogooverride and see your bitmap. On other UEFI implementations the shell disables graphics and the bitmap won't display from the shell.
7) Then do:

        copy fs1:\nullhello.efi fs0:\nullhello.efi
        bcfg driver add <driver # -- usually 0> fs0:\<yourdriver>.efi
        reset
8) If you test first with nullhello.efi after the reset on some UEFIs if you watch closely you will see the hello message flash by other UEFIs seem to disable the console output and the message doesn't show up.  And, since nullhello doesn't install a BGRT the windows logo will overwrite it once that gets painted so the text message only shows briefly.  However, when you install bootlogooverride.efi as the driver you will see the Contoso logo and the normal windows logo will not appear.  But, you will see the spinning dots unless you've turned them off with bcdedit(see further explanation about bcdedit later).
***
IMPORTANT: Be sure you understand the note about clearing secure boot at the top of this readme before proceeding further or you may brick your device.
***
9) Build your driver with the correct .pfx from the powershell script pointed to from the ms iot secure boot directions that generates all the certs and key files.
10) Copy the .efi drivers and the .bin files for db,kek,pk onto your portable storage device.
11) Install the correctly signed driver per the above directions
12) Boot into IoT Core and make sure you see the correct override logo.
11) While booted into IoT Core obtain a command prompt.  This can be done several ways including ssh, remote powershell, or the device administration site Windows Device Portal (WDP) that runs in the developer image by default.
12) At the command prompt run fwvar to install the key .bin files that you copied to portable storage into the appropriate uefi non-volatile variables:

        FWVar.exe put imagesecurity:db SetVariable_db.bin NV BS RT TA
        FWVar.exe put efiglobal:KEK SetVariable_kek.bin NV BS RT TA
        FWVar.exe put efiglobal:PK SetVariable_pk.bin NV BS RT TA
NOTE: The command order is important.  Putting 'PK' enables secureboot mode so it needs to be the last one you set.  Also, the variable names are case-sensitive. 
13) Reboot and see that your logo still displays.
14) Replace the correctly signed driver with the previous insecurely signed driver that doesn't match the installed .bin keyfiles.  
15) Reboot and observe that the bad driver file is ignored and the normal windows logo is displayed.
16) Restore the correctly signed one and see that override logo works again.

## Additional Considerations
### BCDEdit
Besides the boot logo itself some developers may wish to eliminate the spinning dots or other parts of the standard Windows boot experience UI.
Running bcdedit against the boot store can configure a number of options.  
For details about currently supported options run bcdedit /? and examine the usage options.  In particular for the dots set the bootapp option 'bootuxdisabled' to true.




## Copyright

Copyright (c) 2016-2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


+ 



