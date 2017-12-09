/**
Implement UnitTestLib 

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
#include <PiDxe.h>
#include <UnitTestTypes.h>
#include <Library/UnitTestLogLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>


BOOLEAN
EFIAPI
UnitTestAssertTrue (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN BOOLEAN                    Expression,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  )
{
  if (!Expression)
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTTRUE, "%a::%d Expression (%a) is not TRUE!\n", FunctionName, LineNumber, Description);
    UnitTestLog(Framework, DEBUG_ERROR, "[ASSERT FAIL] %a::%d Expression (%a) is not TRUE!\n", FunctionName, LineNumber, Description );
  }
  return Expression;
}


BOOLEAN
EFIAPI
UnitTestAssertFalse (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN BOOLEAN                    Expression,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  )
{
  if (Expression)
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTFALSE, "%a::%d Expression(%a) is not FALSE!\n", FunctionName, LineNumber, Description );
    UnitTestLog(Framework, DEBUG_ERROR,"[ASSERT FAIL] %a::%d Expression (%a) is not FALSE!\n", FunctionName, LineNumber, Description );
  }
  return !Expression;
}


BOOLEAN
EFIAPI
UnitTestAssertNotEfiError (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN EFI_STATUS                 Status,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  )
{
  if (EFI_ERROR( Status ))
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTNOTEFIERROR, "%a::%d Status '%a' is EFI_ERROR (%r)!\n", FunctionName, LineNumber, Description, Status);
    UnitTestLog(Framework, DEBUG_ERROR,"[ASSERT FAIL] %a::%d Status '%a' is EFI_ERROR (%r)!\n", FunctionName, LineNumber, Description, Status );
  }
  return !EFI_ERROR( Status );
}


BOOLEAN
EFIAPI
UnitTestAssertEqual (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN UINT64                     ValueA,
  IN UINT64                     ValueB,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *DescriptionA,
  IN CONST CHAR8                *DescriptionB
  )
{
  if ((ValueA != ValueB))
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTEQUAL, "%a::%d Value %a != %a (%d != %d)!\n", FunctionName, LineNumber, DescriptionA, DescriptionB, ValueA, ValueB);
    UnitTestLog(Framework, DEBUG_ERROR,"[ASSERT FAIL] %a::%d Value %a != %a (%d != %d)!\n", FunctionName, LineNumber, DescriptionA, DescriptionB, ValueA, ValueB );
  }
  return (ValueA == ValueB);
}

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
)
{
  if (CompareMem((VOID*)ValueA, (VOID*)ValueB, Length) != 0)
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTEQUAL, __FUNCTION__, "%a::%d Memory at %a != %a for length %d bytes!\n", FunctionName, LineNumber, DescriptionA, DescriptionB, Length);
    UnitTestLog(Framework, DEBUG_ERROR, "[ASSERT FAIL] %a::%d Value %a != %a for length %d bytes!\n", FunctionName, LineNumber, DescriptionA, DescriptionB, Length);
    return FALSE;
  }
  return TRUE;
}


BOOLEAN
EFIAPI
UnitTestAssertNotEqual (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN UINT64                     ValueA,
  IN UINT64                     ValueB,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *DescriptionA,
  IN CONST CHAR8                *DescriptionB
  )
{
  if ((ValueA == ValueB))
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTNOTEQUAL,"%a::%d Value %a == %a (%d == %d)!\n", FunctionName, LineNumber, DescriptionA, DescriptionB, ValueA, ValueB);
    UnitTestLog(Framework, DEBUG_ERROR,"[ASSERT FAIL] %a::%d Value %a == %a (%d == %d)!\n", FunctionName, LineNumber,DescriptionA, DescriptionB, ValueA, ValueB );
  }
  return (ValueA != ValueB);
}


BOOLEAN
EFIAPI
UnitTestAssertStatusEqual (
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN EFI_STATUS                 Status,
  IN EFI_STATUS                 Expected,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *Description
  )
{
  if ((Status != Expected))
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTSTATUSEQUAL, "%a::%d Status '%a' is %r, should be %r!\n", FunctionName, LineNumber, Description, Status, Expected);
    UnitTestLog(Framework, DEBUG_ERROR,"[ASSERT FAIL] %a::%d Status '%a' is %r, should be %r!\n", FunctionName, LineNumber, Description, Status, Expected );
  }
  return (Status == Expected);
}

BOOLEAN
EFIAPI
UnitTestAssertNotNull(
  IN UNIT_TEST_FRAMEWORK_HANDLE Framework,
  IN VOID*                      Pointer,
  IN CONST CHAR8                *FunctionName,
  IN UINTN                      LineNumber,
  IN CONST CHAR8                *PointerName
)
{
  if (Pointer == NULL)
  {
    UnitTestLogFailure(Framework, FAILURETYPE_ASSERTNOTNULL, "%a::%d Pointer (%a) is NULL!\n", FunctionName, LineNumber, PointerName);
    UnitTestLog(Framework, DEBUG_ERROR, "[ASSERT FAIL] %a::%d Pointer (%a) is NULL!\n", FunctionName, LineNumber, PointerName);
  }
  return (Pointer != NULL);

}
