# TPM Capsule Update Sample - Infineon dTPM

## &#x1F539; Copyright

Copyright (c) 2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


## &#x1F539; About

This branch contains the TpmCapsuleUpdatePkg which, among other things, contains the MsInfineonTpmFmpDeviceLib. The purpose of this lib is to work with the libraries and drivers in MsCapsuleUpdatePkg to produce a FIRMWARE_MANAGEMENT_PROTOCOL and entry in the ESRT that allows the firmware on a dTPM to be targeted with an update capsule from the OS.

This package also contains tools to package the TPM update capsule and helper libraries to address update robustness/resiliency, user experience, and capsule signing.

&#x1F538; Contains code contributed by Infineon, released under BSD-3 license. This code is entirely contained within the IfxDriverSource directory, with the only exceptions being `BorrowedInfineonFunctions.h` and `InfineonSpecificTpmCommands.c`, as noted in 'Notes' section. A few modifications were made to the Infineon source, denoted by `IFX_CHANGE` code comments.


## &#x1F539; How to Use

#### &#x1F538; Required Platform Steps

It is required that the platform implement an instance of `MsBlobPersistenceLib` and `CapsuleKeyLib`. The details of these libs and the requirements for implementation are described in 'Customizations' section.

#### &#x1F538; Building the Driver

`TpmCapsuleUpdatePkg.dsc` contains an example of how to build the driver. It is assumed that the build environment contains the `MsCapsuleUpdatePkg` and that the builder has familiarity with its use. Documentation for this package can be found in the GitHub 'share' branch associated with it.

The `SampleDeviceLibWrapperFmpDxeDriver` from `MsSampleFmpDevicePkg` is being used as the driver wrapper for the update lib. Inspection will show that this wrapper is mostly a shim for the entry point. A custom wrapper can be used if desired.

Customizations in this .dsc include:
* ESRT/Capsule Targeting GUIDs.
    * `PcdDeviceLibWrapperDeviceGuid`, `PcdDeviceTpmFmpGuidCertified`, and `PcdDeviceTpmFmpGuidUncertified` are defined for this purpose. If FIPS certification can be deteremined (for the existing FW), the 'Certified' or 'Uncertified' GUID will be reported in the FMP `ImageTypeId` and ESRT. If not, the `PcdDeviceLibWrapperDeviceGuid` will be reported. These can all be the same value if desired.
* Progress indicator color. This is the color that will be used by `FmpRecoveryProgressIndicatorLib` to visually display the progress (if implemented).
    * Change `PcdProgressColor` to a 32-Bit RGB value. Top nibble must be 0xFF.

#### &#x1F538; Preparing the Capsule (Complications with Versioning)

While the capsule itself should be built with the tools provided in `MsCapsuleUpdatePkg`, there is an additional step for this driver due to the specificity of TPM FW updates.

When updating the firmware for many other devices, it is possible to bring several 'current' versions up to a common 'new' version with a single update. For example, suppose there is a device that was shipped with firmware v1.0 and there has been a previous update to v2.0. It's possible to issue a single capsule v3.0 that updates both a v1.0 and a v2.0 firmware. However, the FW updates for TPM are always version-to-version. This means that a payload for v1.0->v3.0 will not work on a v2.0 part.

To address this, and make it possible to simulate the same behavior as other firmware capsules, the TPM update capsule is multi-homed (ie. can contain multiple payloads that target each bring one 'source' version up to a common 'destination' version). *NOTE:* See 'Notes' section for more details on how version numbers are handled.

```
MsInfineonTpmFmpDeviceLib expects a structure like the one below...

+------------------------------------+
|     TPM_CAPSULE_PAYLOAD_HEADER     |   <--- Contains SubPayload Count
+------------------------------------+
|  TPM_CAPSULE_SUBPAYLOAD_HEADER 1   |   <--- 'Source' Version and Length
+------------------------------------+
|                                    |
|           <SUBPAYLOAD 1>           |   <--- FW Payload Sent to dTPM
|                                    |
+------------------------------------+
|                ...                 |
+------------------------------------+
|  TPM_CAPSULE_SUBPAYLOAD_HEADER N   |
+------------------------------------+
|                                    |
|           <SUBPAYLOAD N>           |
|                                    |
+------------------------------------+
```

To help create this structure, the `GenerateMultiHomedTpmCapsulePayload.py` tool is provided, located in the `Tools` directory of the `TpmCapsuleUpdatePkg`. It takes in one or more Infineon FW payload (with '-i' flag) and outputs a multi-homed update file (with '-o' flag). The binary produced by this script should then be processed like a normal capsule payload with the tools in `MsCapsuleUpdatePkg`.

*NOTE ABOUT THE TOOL:* The tool determines 'source' and 'destination versions from the Infineon FW payload naming convention. This naming convention looks like...

```
TPM20_AA.BB.CC.DD_to_TPM20_WW.XX.YY.ZZ.bin
      (Source Ver)         (Dest Ver)
```

The script will check to see that a similar name was provided and -- if multiple input files were provided -- that the 'sources' are unique and the 'destinations' are common.


## &#x1F539; Important Notes About Usage/Implementation

#### &#x1F538; Files and the File Layout

The files in the `IfxDriverSource` directory are almost an exact copy of the source in the `TPMToolsUEFIPkg` provided by Infineon. The only modifications are wrapped in `IFX_CHANGE` comments, and should be easy to find and/or port to future versions.

The primary implementation of the `FmpDeviceLib` is in the `MsInfineonTpmFmpDeviceLib.c` file. This file calls out to a few functions provided by the `IfxDriverSource` files and helper functions provided by the following files:

* `Tpm12And20Shim.c` - This file provides an abstraction between common functions (such as GetCapabilities()) in the `Tpm12CommandLib` and `Tpm20CommandLib`. This is so that the logic for determining TPM version and marshalling the data doesn't complicate the business logic for the actual update.
* `InfineonSpecificTpmCommands.c` - This file contains native implementations (built against the `Tpm2DeviceLib`) of commands that are vendor-specific to the Infineon dTPM part.
* `BorrorowedInfineonFunctions.h` - Perhaps the most confusing file, the purpose of this file is to contain function and structure declarations for calling into the `IfxDriverSource` files. This file is tightly coupled to the declarations in the `IfxDriverSource` header files and must be kept in sync. This file only exists to eliminate the need to include `IfxDriverSource` header files directly, and thus prevent conflict in the #include chains.

#### &#x1F538; Robustness and Fault Tolerance

The Infineon dTPM does not have enough storage space to perform a "ping-pong" firmware update. This means that if a power loss or other system failure occurs during FW update, the TPM will be left in a reduced functionality mode. This mode is recoverable, but requires the firmware payload that initiated the FW update. However, since it is undesireable to boot to an OS without the TPM functional, it is necessary to attempt to persist this payload in UEFI to enable an attempt to recover.

This mechanism is highly platform-specific. As such, the `MsBlobPersistenceLib` is defined to allow the platform to persist the TPM FW payload. It is important to note that this library interface does not specify a mechanism to validate the FW payload, and any such mechanism must be defined by the implementer. However, the dTPM will validate the payload internally, so it is up to the implementer to decide whether this assurance is satisfactory.

The FW update will *NOT* proceed if the `MsBlobPersistenceLib` is unable to save the payload. If saved, the driver will attempt an initial update and up to three recovery updates before declaring a critical failure and booting to the OS.

NOTE: Since the capsule is multi-homed, only the matching payload will be persisted. This minimizes the required space for persistence.

#### &#x1F538; 32-Bit Incompatibility

IA32 incompatibilities currently exist, primarily in layer between MS code and Infineon code. Build errors are known to occur when targeting 32-bit binaries. This code is only tested on X64 architectures.

#### &#x1F538; TPM FW Version Packing

TPM 2.0 version numbers are defined as 64-bit values, the schema of which is defined by the TPM vendor. The version numbers supported by ESRT and the `MsCapsuleUpdatePkg` framework are 32-bit values. As such, it was necessary to define a packing scheme to consistently pack the 64-bit version number into 32 bits.

This is implemented specifically for the Infineon dTPM in the `FmpDeviceGetVersion()` function. In summary, the values for the physical interface (LPC vs SPI) and the certifications (FIPS vs non-FIPS) are encoded into the GUID for the FMP instance. The remaining bits are used to describe the software version.

#### &#x1F538; Update Count Limitations

For security (to prevent part wear-out from hammering), most dTPM vendors limit the number of updates that a given part can take. This includes update attempts. For this implementation, the max number is defined as 64 (as defined in `MsInfineonTpmFmpDeviceLib.c` by `TPM_MAX_UPDATES_ALLOWED`). After this number of attempts, it is assumed that the TPM part will block all future attempts to initiate an update.

In order to prevent this limit being reached unexpectedly, there are two limiters placed in this implementation that serve as barriers to allow a platform vendor to respond if a capsule publication or servicing error should result in unexpected update attempts. The `TpmMaxAllowedCapsuleUpdates` and `TpmMaxAdvertisedUpdates` PCDs are defined in the `gTpmCapsuleUpdatePkgTokenSpaceGuid` namespace to allow build time customization of these limits. After `TpmMaxAdvertisedUpdates` is exceeded, the FW will not be reported as updateable in `FmpDeviceGetAttributes()`. After `TpmMaxAllowedCapsuleUpdates` is exceeded, the driver will refuse any further capsules. A UEFI update will be required to raise the limit. This provides the opportunity for the platform vendor to address whatever problem may be causing the unexpected attempts and raise the limit to allow further attempts.

#### &#x1F538; ESRT GUID Definitions

Since the TPM FW Version Packing resulted in the truncation of information, this information must be available elsewhere. In this implementaiton, it is denoted by the GUID chosen for the ESRT node. 

There is more information in the 'Customizations' section about these GUIDs.


## &#x1F539; Customizations

#### &#x1F538; CapsuleKeyLib (Required)

The `CapsuleKeyLib` requires an implementation in order to verify the signing of the capsule wrapper. This implementation may be shared with other FMP drivers based on the `MsCapsuleUpdatePkg` framework, or it may be specific to this driver. See the scripts provided in the `Tools` directory of the `MsSampleFmpDevicePkg` to help generate this library.

#### &#x1F538; MsBlobPersistenceLib (Required)

As mentioned in the 'Notes' section, this library must be implemented to provide fault-tolerance for the update. There are two functions described by the interface: `GetPersistedBlob()` and `SetPersistedBlob()`. These functions must behave as described in the header file (`MsBlobPersistenceLib.h`).

The blob will be deleted if the TPM update completes successfully, or recovery fails three times in succession.

*IMPORTANT NOTE:* The presence of a blob on any boot will be considered sufficient to indicate that an update was in progress on the previous boot and the recovery process will be initiated. It is considered the responsibility of the implementer to take steps to ensure that either this blob can only be written by a trusted process or that the blob is validated prior to reporting its presence.

The NULL implementation included in this package is sufficient to build, but will ASSERT whenever called (which should happen at least once during boot). If no persistence is desired or available, it is possible to alter this implementation to return EFI_SUCCESS on `SetPersistedBlob()` and EFI_NOT_FOUND on `GetPersistedBlob()`, but this is *NOT* recommended.

#### &#x1F538; Change the GUIDs for ESRT targeting. (Recommended)

There are two pieces of information that need to be denoted: physical interface and certification.

As for physical interface, no mechanism has been provided to dynamically choose GUIDs at runtime. Given that most platform vendors are likely to pick unique GUIDs for FW targets for each of their platforms, anyway, and that a single platform is unlikely to contain both LPC and SPI variants, it is felt that it's safe to assume that the uniqueness of these targets is built-in.

As seen in the .dsc file, there are three different GUIDs defined for the driver. `PcdDeviceLibWrapperDeviceGuid` is the base GUID and the default. It is used as the UEFI variable namespace for internal data, and as the fallback ESRT GUID if the certification state cannot be determined. `PcdDeviceTpmFmpGuidCertified` and `PcdDeviceTpmFmpGuidUncertified` will be used as the ESRT GUID if the state can be positively determined.

There are several permutations that can be employed with these GUIDs, including setting two of them to be the same (indicating that one of the certification modes is the default) or setting all three to be the same (indicating that all capsules will include payloads for both certified and uncertified).

#### &#x1F538; Status Codes/Telemetry (Optional)

Multiple pieces of this package are built to report error conditions through the `ReportStatusCode()` interface. If support is available in the platform UEFI, these codes can be reported through any platform-specific interface. The codes used can be found in `MsStatusCodes.h`.

#### &#x1F538; TpmMaxAdvertisedUpdates and TpmMaxAllowedCapsuleUpdates PCDs (Optional)

These PCDs have default values, but can be adjusted by the platform UEFI as desired. See the 'Notes' section for information about usage.

#### &#x1F538; User Progress Updates/Feedback (Optional)

This package uses the `MsCapsuleUpdatePkg` framework to report status to the user during normal updates, but calls out to `FmpRecoveryProgressIndicatorLib` to report update progress to the user during recovery updates. This is because the recovery update may occur with fewer system resources available, and may require platform-specific knowledge to implement.

If desired, this library can be implemented to provide this feedback to the user. Only two functions must be implemented. `InitRecoveryProgressIndicator()` will be called once when preparing to run the update. A non-EFI_SUCCESS return value will NOT stop the update from proceeding. `UpdateRecoveryProgressIndicator()` will be called with a 1% granularity for the duration of the update.

This package has a NULL implementation of this library for building purposes.
