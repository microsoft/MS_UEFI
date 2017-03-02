# MsUnitTestPkg, XmlSupport, and UefiTestingPkg
## &#x1F539; Copyright
Copyright (c) 2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## &#x1F539; About
This branch adds three new packages. Each package has its own readme for details, dev notes, overview, andusage information
about the pacakge.   
1. The *MsUnitTestPkg* which contains the generic unit test framework to allowing
for simple, easy to write, UEFI shell based unit tests.  This package has a sample app to show the basics of a unit test
but otherwise only contains the core logic if the unit test framework.    
1. The *XmlSupportPkg*  contains a generic xml parser and libraries to support creation of xml trees and convertion
to string.  In this pacakge you will also find a QueryLib which makes working with and searching within XML nodes easier.  
Finally it also contains unit tests for each the libraries.  
1. The *UefiTestPkg* contains an ever increasing list of Audit and Functional Tests for UEFI based systems.  The goal of this
package is to create easy to use and easy to automate tools that can be run as part of a platform test pass.  

## &#x1F539; Quick Start/Successful Usage

### &#x1F538; Add the unit test libraries/ framework to your DSC (library classes section)
    UnitTestLib|MsUnitTestPkg/Library/UnitTestLib/UnitTestLib.inf
    UnitTestLogLib|MsUnitTestPkg/Library/UnitTestLogLib/UnitTestLogLib.inf
    UnitTestAssertLib|MsUnitTestPkg/Library/UnitTestAssertLib/UnitTestAssertLib.inf
    UnitTestPersistenceLib|MsUnitTestPkg/Library/UnitTestPersistenceFileSystemLib/UnitTestPersistenceFileSystemLib.inf
    UnitTestBootUsbLib|MsUnitTestPkg/Library/UnitTestBootUsbClassLib/UnitTestBootUsbClassLib.inf
    UnitTestResultReportLib|XmlSupportPkg/Library/UnitTestResultReportJUnitFormatLib/UnitTestResultReportLib.inf
    # ---- If you want plain text report output use this lib instance instead ----
    # UnitTestResultReportLib|MsUnitTestPkg/Library/UnitTestResultReportPlainTextOutputLib/UnitTestResultReportLib.inf

### &#x1F538; Add the tests/tools to your DSC
    ####################################################
    # Sample Test                                      #
    ####################################################
    MsUnitTestPkg/Sample/SampleUnitTestApp/SampleUnitTestApp.inf 

    #####################################################
    # Function System Tests                             #
    #####################################################

    # MemMap and MAT Test
    UefiTestingPkg/FunctionalSystemTests/MemmapAndMatTestApp/MemmapAndMatTestApp.inf 

    # MorLock v1 and v2 Test
    UefiTestingPkg/FunctionalSystemTests/MorLockTestApp/MorLockTestApp.inf

    # Capsule test
    UefiTestingPkg/FunctionalSystemTests/CapsuleTestApp/CapsuleTestApp.inf {
    <LibraryClasses>
      TestCapsuleHelperLib|UefiTestingPkg/Library/Private/TestCapsuleHelperLib/TestCapsuleHelperLib.inf
    }

    # MOR Bit test
    UefiTestingPkg/FunctionalSystemTests/MorBitTestApp/MorBitTestApp.inf {
    <LibraryClasses>
      TestCapsuleHelperLib|UefiTestingPkg/Library/Private/TestCapsuleHelperLib/TestCapsuleHelperLib.inf
    }

    ####################################################
    # Audit Test apps                                  #
    ####################################################

    UefiTestingPkg/AuditTests/UefiVarLockAudit/UEFI/UefiVarLockAudit.inf


    ####################################################
    # Unit Tests                                       #
    ####################################################
    XmlSupportPkg/UnitTests/XmlTreeLib/XmlTreeLibUnitTests.inf 

    XmlSupportPkg/UnitTests/XmlTreeQueryLib/XmlTreeQueryLibUnitTests.inf {
      <PcdsFixedAtBuild>
        #Turn off Halt on Assert and Print Assert so that libraries can 
        #be tested in more of a release mode environment
        gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0E 
    }

### &#x1F538; Use it
Boot the UEFI Shell and run the tool.  
Some tests will do reboots and continue more tests after the 
reboot so it is suggested to put your tool on a bootable USB key (with the shell) and 
create a startup.nsh file.  The test framework has support for setting boot next so the tests should
be able to complete without user interaction.  
