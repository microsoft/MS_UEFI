# MsUnitTestPkg, XmlSupport, and UefiTestingPkg
Copyright (c) 2018, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## &#x1F539; About
This branch adds three new packages. Each package has its own readme for details, dev notes, overview, and usage information
about the package.   
1. The *MsUnitTestPkg* which contains the generic unit test framework to allow
for simple, easy to write, UEFI shell based unit tests.  This package has a sample app to show the basics of a unit test
but otherwise only contains the core logic if the unit test framework.    
1. The *XmlSupportPkg*  contains a generic xml parser and libraries to support creation of xml trees and conversion
to string.  In this package you will also find a QueryLib which makes working with and searching within XML nodes easier.  
Finally it also contains unit tests for each the libraries.  
1. The *UefiTestingPkg* contains an ever increasing list of Audit and Functional Tests for UEFI based systems.  The goal of this
package is to create easy to use and easy to automate tools that can be run as part of a platform test pass.   Look thru the different tests in this package as they are not enumerated in this readme file.  

## &#x1F539; Quick Start/Successful Usage
There are readme.md files within many of the test folders and packages.  These readme files contain more specific directions for a given test and should be considered the guide for that test.  The guide below is a general sample for starting to build your own unit tests.  

### &#x1F538; Add the unit test libraries/ framework to your DSC (library classes section)
```
UnitTestLib|MsUnitTestPkg/Library/UnitTestLib/UnitTestLib.inf
UnitTestLogLib|MsUnitTestPkg/Library/UnitTestLogLib/UnitTestLogLib.inf
UnitTestAssertLib|MsUnitTestPkg/Library/UnitTestAssertLib/UnitTestAssertLib.inf
UnitTestPersistenceLib|MsUnitTestPkg/Library/UnitTestPersistenceFileSystemLib/UnitTestPersistenceFileSystemLib.inf
UnitTestBootUsbLib|MsUnitTestPkg/Library/UnitTestBootUsbClassLib/UnitTestBootUsbClassLib.inf
UnitTestResultReportLib|XmlSupportPkg/Library/UnitTestResultReportJUnitFormatLib/UnitTestResultReportLib.inf
# ---- If you want plain text report output use this lib instance instead ----
# UnitTestResultReportLib|MsUnitTestPkg/Library/UnitTestResultReportPlainTextOutputLib/UnitTestResultReportLib.inf
```

### &#x1F538; Add your test to your DSC
```
####################################################
# Sample Test                                      #
####################################################
MsUnitTestPkg/Sample/SampleUnitTestApp/SampleUnitTestApp.inf 
```

### &#x1F538; Use it
Boot the UEFI Shell and run the tool.  
Some tests will do reboots and continue more tests after the 
reboot so it is suggested to put your tool on a bootable USB key (with the shell) and 
create a startup.nsh file.  The test framework has support for setting boot next so the tests should
be able to complete without user interaction.  
