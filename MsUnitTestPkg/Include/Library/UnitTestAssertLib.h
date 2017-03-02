/** @file
Provides a unit test assert helpers.  This allows tests to focus on testing logic
and the library to handle common assertions. 

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

#ifndef __UNIT_TEST_ASSERT_LIB_H__
#define __UNIT_TEST_ASSERT_LIB_H__


#define UT_ASSERT_TRUE(Expression)                \
  if(!UnitTestAssertTrue( Framework, (Expression), __FUNCTION__, __LINE__, #Expression )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_FALSE(Expression)               \
  if(!UnitTestAssertFalse( Framework, (Expression), __FUNCTION__, __LINE__, #Expression )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_EQUAL(ValueA, ValueB)           \
  if(!UnitTestAssertEqual( Framework, (UINTN)ValueA, (UINTN)ValueB, __FUNCTION__, __LINE__, #ValueA, #ValueB )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_MEM_EQUAL(ValueA, ValueB, Length) \
  if(!UnitTestAssertMemEqual( Framework, (UINTN)ValueA, (UINTN)ValueB, (UINTN)Length, __FUNCTION__, __LINE__, #ValueA, #ValueB )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_NOT_EQUAL(ValueA, ValueB)       \
  if(!UnitTestAssertNotEqual( Framework, ValueA, ValueB, __FUNCTION__, __LINE__, #ValueA, #ValueB )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_NOT_EFI_ERROR(Status)           \
  if(!UnitTestAssertNotEfiError( Framework, Status, __FUNCTION__, __LINE__, #Status )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_STATUS_EQUAL(Status, Expected)  \
  if(! UnitTestAssertStatusEqual( Framework, Status, Expected, __FUNCTION__, __LINE__, #Status )) { return UNIT_TEST_ERROR_TEST_FAILED;}

#define UT_ASSERT_NOT_NULL(Pointer)     \
  if(!UnitTestAssertNotNull(Framework, Pointer, __FUNCTION__, __LINE__, #Pointer)) { return UNIT_TEST_ERROR_TEST_FAILED; }


BOOLEAN
EFIAPI
UnitTestAssertTrue (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN BOOLEAN                    Expression,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  );

BOOLEAN
EFIAPI
UnitTestAssertFalse (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN BOOLEAN                    Expression,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  );

BOOLEAN
EFIAPI
UnitTestAssertNotEfiError (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN EFI_STATUS                 Status,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  );

BOOLEAN
EFIAPI
UnitTestAssertEqual (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN UINTN                      ValueA,
  IN UINTN                      ValueB,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *DescriptionA,
  IN CONST CHAR8                *DescriptionB
  );

BOOLEAN
EFIAPI
UnitTestAssertMemEqual(
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN UINTN                      ValueA,
  IN UINTN                      ValueB,
  IN UINTN                      Length,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *DescriptionA,
  IN CONST CHAR8                *DescriptionB
);

BOOLEAN
EFIAPI
UnitTestAssertNotEqual (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN UINTN                      ValueA,
  IN UINTN                      ValueB,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *DescriptionA,
  IN CONST CHAR8                *DescriptionB
  );

BOOLEAN
EFIAPI
UnitTestAssertStatusEqual (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN EFI_STATUS                 Status,
  IN EFI_STATUS                 Expected,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  );

BOOLEAN
EFIAPI
UnitTestAssertNotNull(
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN VOID*                      Pointer,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *PointerName
);

#endif