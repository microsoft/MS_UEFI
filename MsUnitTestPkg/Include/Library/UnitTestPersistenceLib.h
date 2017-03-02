/** @file -- UnitTestPersistenceLib.h
This header file describes a library that contains functions to save and
restore unit test internal state, in case the test needs to pause and resume
(eg. a reboot-based test).

Copyright (c) 2016, Microsoft Corporation

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

#ifndef _UNIT_TEST_PERSISTENCE_LIB_H_
#define _UNIT_TEST_PERSISTENCE_LIB_H_

#define UNIT_TEST_PERSISTENCE_LIB_VERSION   1


/**
  Determines whether a persistence cache already exists for
  the given framework.

  @param[in]  FrameworkHandle   A pointer to the framework that is being persisted.

  @retval     TRUE
  @retval     FALSE   Cache doesn't exist or an error occurred.

**/
BOOLEAN
EFIAPI
DoesCacheExist (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  FrameworkHandle
  );


/**
  Will save the data associated with an internal Unit Test Framework
  state in a manner that can persist a Unit Test Application quit or
  even a system reboot.

  @param[in]  FrameworkHandle   A pointer to the framework that is being persisted.
  @param[in]  SaveData          A pointer to the buffer containing the serialized
                                framework internal state.

  @retval     EFI_SUCCESS   Data is persisted and the test can be safely quit.
  @retval     Others        Data is not persisted and test cannot be resumed upon exit.

**/
EFI_STATUS
EFIAPI
SaveUnitTestCache (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  FrameworkHandle,
  IN  UNIT_TEST_SAVE_HEADER       *SaveData
  );


/**
  Will retrieve any cached state associated with the given framework.
  Will allocate a buffer to hold the loaded data.

  @param[in]  FrameworkHandle   A pointer to the framework that is being persisted.
  @param[in]  SaveData          A pointer pointer that will be updated with the address
                                of the loaded data buffer.

  @retval     EFI_SUCCESS       Data has been loaded successfully and SaveData is updated
                                with a pointer to the buffer.
  @retval     Others            An error has occurred and no data has been loaded. SaveData
                                is set to NULL.

**/
EFI_STATUS
EFIAPI
LoadUnitTestCache (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  FrameworkHandle,
  OUT UNIT_TEST_SAVE_HEADER       **SaveData
  );

#endif // _UNIT_TEST_PERSISTENCE_LIB_H_
