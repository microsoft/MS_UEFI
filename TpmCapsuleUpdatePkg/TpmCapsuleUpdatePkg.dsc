##
# Copyright (c) 2016-2017, Microsoft Corporation

# All rights reserved.
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##


[Defines]
  PLATFORM_NAME                  = TpmCapsuleUpdatePkg
  PLATFORM_GUID                  = B33145F2-E31E-4C4B-B366-CE06C2031A23
  PLATFORM_VERSION               = 0.96
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/TpmCapsuleUpdatePkg
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf

  TimerLib|PcAtChipsetPkg/Library/AcpiTimerLib/DxeAcpiTimerLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf

  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf 
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf 
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf

  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf

  CapsuleUpdatePolicyLib|MsCapsuleUpdatePkg/Library/CapsuleUpdatePolicyLibNull/CapsuleUpdatePolicyLibNull.inf
  MsFmpPayloadHeaderLib|MsCapsuleUpdatePkg/Library/MsFmpPayloadHeaderV1DxeLib/MsFmpPayloadHeaderV1DxeLib.inf
  BmpSupportLib|MsCapsuleUpdatePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
  IntSafeLib|MdePkg/Library/IntSafeLib/IntSafeLib.inf
  CapsuleLib|MsCapsuleUpdatePkg/Library/DxeCapsuleLib/DxeCapsuleLib.inf
  FmpHelperLib|MsCapsuleUpdatePkg/Library/FmpHelperLib/FmpHelperDxeLib.inf
  FmpPolicyLib|MsCapsuleUpdatePkg/Library/FmpPolicyLib/FmpPolicyDxeLib.inf
  # For update progress text display:
  #DisplayUpdateProgressLib|MsCapsuleUpdatePkg/Library/DisplayUpdateProgressTextLib/DisplayUpdateProgressTextLib.inf
  # For update progress bar (graphics) display:
  DisplayUpdateProgressLib|MsCapsuleUpdatePkg/Library/DisplayUpdateProgressGraphicsLib/DisplayUpdateProgressGraphicsLib.inf
  FmpWrapperDeviceLib|MsCapsuleUpdatePkg/Library/FmpWrapperDeviceLib/FmpWrapperDeviceLib.inf

  CapsuleKeyLib|TpmCapsuleUpdatePkg/Library/CapsuleKeyBaseLib/CapsuleKeyBaseLib.inf
  MsBlobPersistenceLib|TpmCapsuleUpdatePkg/Library/MsBlobPersistenceLib/MsBlobPersistenceLibNull.inf
  FmpRecoveryProgressIndicatorLib|TpmCapsuleUpdatePkg/Library/FmpHelperLibs/FmpRecoveryProgressIndicatorLibNull.inf


[LibraryClasses.X64]
  #
  # DXE phase common
  #
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf


[PcdsFeatureFlag]
#  <SET FEATURE PCDs FLAGS FOR THIS BUILD HERE>


[PcdsFixedAtBuild]
#  <SET FIXED AT BUILD PCDs FOR THIS BUILD HERE>


###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################

[Components]
  TpmCapsuleUpdatePkg/Library/MsInfineonTpmFmpDeviceLib/MsInfineonTpmFmpDeviceLib.inf

  MsSampleFmpDevicePkg/SampleDeviceLibWrapperFmpDxeDriver/SampleDeviceLibWrapperFMP.inf {
  <LibraryClasses>
    FmpDeviceLib|TpmCapsuleUpdatePkg/Library/MsInfineonTpmFmpDeviceLib/MsInfineonTpmFmpDeviceLib.inf
    Tpm12DeviceLib|SecurityPkg/Library/Tpm12DeviceLibDTpm/Tpm12DeviceLibDTpm.inf
    Tpm12CommandLib|SecurityPkg/Library/Tpm12CommandLib/Tpm12CommandLib.inf
    Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibDTpm/Tpm2DeviceLibDTpm.inf

  <PcdsFixedAtBuild>
    gMsCapsuleUpdatePkgTokenSpaceGuid.PcdDeviceLibWrapperDeviceImageName|L"Infineon dTPM"

    #Fallback FMP UUID (Only if it cannot be determined whether TPM version is Certified/Uncertified)
    ## {0279C247-739F-4BA7-9DED-415642BDCFAC}
    gMsCapsuleUpdatePkgTokenSpaceGuid.PcdDeviceLibWrapperDeviceGuid|{ 0x47, 0xc2, 0x79, 0x02, 0x9f, 0x73, 0xa7, 0x4b, 0x9d, 0xed, 0x41, 0x56, 0x42, 0xbd, 0xcf, 0xac }

    #Certified/Uncertified dynamic UUID
    ## AC012951-FDF5-461B-A9B1-74B80B5D6298
    gTpmCapsuleUpdatePkgTokenSpaceGuid.PcdDeviceTpmFmpGuidCertified|{ 0x51, 0x29, 0x01, 0xac, 0xf5, 0xfd, 0x1b, 0x46, 0xa9, 0xb1, 0x74, 0xb8, 0x0b, 0x5d, 0x62, 0x98 }
    ## B35A416D-63CB-4310-A5F3-8625ED8E1F22
    gTpmCapsuleUpdatePkgTokenSpaceGuid.PcdDeviceTpmFmpGuidUncertified|{ 0x6d, 0x41, 0x5a, 0xb3, 0xcb, 0x63, 0x10, 0x43, 0xa5, 0xf3, 0x86, 0x25, 0xed, 0x8e, 0x1f, 0x22 }

    #set to VIOLET (RGB) (128, 00, 64)
    gMsCapsuleUpdatePkgTokenSpaceGuid.PcdProgressColor|0xFF800040
    gMsCapsuleUpdatePkgTokenSpaceGuid.PcdBuildTimeLowestSupportedVersion|0x2807B302

  <PcdsFeatureFlag>
    gMsCapsuleUpdatePkgTokenSpaceGuid.PcdDeviceLibWrapperSystemResetRequired|TRUE
  }

[BuildOptions]
  #Turn off optimization to ease in debugging. 
  DEBUG_VS2015x86_*_CC_FLAGS     = /Od

