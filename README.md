# ResetSystem Feature

## Copyright

Copyright (c) 2016, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

This feature branch contains changes to the way `(*PeiServices)->ResetSystem()`, `(*PeiServices)->ResetSystem2()`, and `gRT->ResetSystem()` work. It also alters the principal library consumed by the core from `ResetSystemLib` to `UefiResetSystemLib`.

The functional changes are:
- Allow platform drivers to register filters to interpret certain reset types.
- Allow platform drivers to register handlers for any type of reset.
- Allow platform drivers to receive notifications of the exact reset type prior to reset.

In general, there is also a philisophical change from `EfiPlatformSpecificReset` indicating a single, specific reset type to a design where the `ResetType` is a "type class" and the GUID provided in `ResetData` is a "type subclass" or "subtype".

*NOTE:* In its current state, all of these interfaces will cease to function at `ExitBootServices`, as many of the functions rely on locating PPIs/Protocols, which isn't possible without Boot Services.

## Reasoning and Philosophy

The following subsections detail some of the thoughts behind the changes.

### Notifications

Notifications are designed specifically to manage processes and hardware for off-SoC or out-of-band parts. Some examples of this include:
- Modern TPM devices can assert physical tampering/hammering protections if the TPM is not informed of a power loss. Prior to shutdown/reset, a TPM shutdown command with the correct parameters must be sent. A notification gives the TPM driver the opportunity to send this command.
- NVME parts can suffer data corruption if caches are not flushed prior to a power loss. A notification gives the block IO driver an opportunity to send this signal.

In general, a notification should *only* be used to inform another process or component of an impending shutdown. A notification callback *must not* take any action to shutdown or reset the SoC itself, since this might -- among other things -- prevent further notifications from being dispatched for other components.

### Handlers

Handlers do the actual heavy lifting. After the reset parameters are correct and after all components have been notified of the impending reset, the modular handler system allows the system core to broadcast common reset commands, complete with semantic information and context, and have them handled differently on different architectures.

For example, on some Intel systems it may be desireable to have the ME handle certain `EfiResetCold` events based off of where they occur in the boot order and/or where they originated. This additional context can be imparted in the reset subtype and dispatched to specific handlers. These handlers might handle the same resets in a more generic way on an AMD or ARM system.

### Filters

Filters are provided as a method for a platform to tailor behavior or account for missing handlers in very specific reset situations. In general, the should *only* be provided by the platform code.

In a filter, the hardware state should not be modified *at all*. The expectation is that a filter will *only* change the parameters of the incoming reset -- _prior to_ the notificaitons being dispatched -- without addressing how those parameters are handled in any way.

## Basic Usage

Almost all of the added functionality references the following function prototype:

```c
typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM)(
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  );
```

This prototype is used for all reset notifications and reset handlers. For reset filters, a slightly different prototype is used, as described later.

For filtering and handling, the following structure is used:

```c
typedef struct _PLATFORM_SPECIFIC_RESET_PROTOCOL
{
  // If this is not NULL, will be used to determine whether the current Platform Specific
  // reset maps to a different type of reset. (eg. A capsule-type reset might actually just be
  // a warm reset with some extra icing).
  EFI_RESET_FILTER            FilterReset OPTIONAL;

  // If this is not NULL, will be used to reset the system if a Platform Specific reset is
  // requested and matches the GUID this protocol is registered with.
  EFI_RESET_SYSTEM            ResetSystem OPTIONAL;
} PLATFORM_SPECIFIC_RESET_PROTOCOL;
```

This structure is registered as a protocol with a GUID corresponding to the `ResetSubtype` that is intended to be handled.

For notifications, the following structure is used:

```c
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
```

This structure is registered as a protocol with the `gEfiResetNotifyCallbackPpiGuid` or `gEfiResetNotifyCallbackProtocolGuid` GUIDs.

## Advanced Usage

### Filters

To filter a given reset, the following function prototype is used in conjunction with the `PLATFORM_SPECIFIC_RESET_PROTOCOL` structure:

```c
typedef
BOOLEAN
(EFIAPI *EFI_RESET_FILTER) (
  IN OUT  EFI_RESET_TYPE      *ResetType,
  IN OUT  EFI_STATUS          *ResetStatus,
  IN OUT  UINTN               *DataSize,
  IN OUT  VOID                **ResetData OPTIONAL
  );
```

This function prototype is similar to the `EFI_RESET_SYSTEM` prototype, but uses indirect pointers to each parameter so that they may be updated. Care would need to be taken to make sure that any updated data is within the scope of the reset infrastructure (eg. allocated memory for `ResetData`).

For example, if a given platform core signals a `gEfiCapsuleUpdateCompleteResetGuid` (not currently a real GUID) subtype when finishing a capsule update, and the platform wants to treat this subtype as a normal `EfiResetCold`, an instance of `PLATFORM_SPECIFIC_RESET_PROTOCOL` can be registered for the `gEfiCapsuleUpdateCompleteResetGuid`. This instance would have a NULL `ResetSystem` field and an instance of `EFI_RESET_FILTER` registered that removes the subtype from the `ResetData` parameter and changes the `ResetType` to `EfiResetCold`.