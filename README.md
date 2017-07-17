# Int Safe Lib

## Copyright

Copyright (c) 2017, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

IntSafeLib provides helper functions to prevent integer overflow during
type conversion, addition, subtraction, and multiplication.

The files in this library are auto-generated.

**Conversion functions**

There are three reasons for having conversion functions:

1. We are converting from a signed type to an unsigned type of the same
   size, or vice-versa.

2. We are converting to a smaller type, and we could therefore possibly
   overflow.

3. We are converting to a bigger type, and we are signed and the type we are
   converting to is unsigned.

**Unsigned Addition, Subtraction, Multiplication**

Unsigned integer math functions protect from overflow and underflow (in case of subtraction).

**Signed Addition, Subtraction, Multiplication**

Strongly consider using unsigned numbers.

Signed numbers are often used where unsigned numbers should be used.
For example file sizes and array indices should always be unsigned.
Subtracting a larger positive signed number from a smaller positive
signed number with SafeInt32Sub will succeed, producing a negative number,
that then must not be used as an array index (but can occasionally be
used as a pointer index.) Similarly for adding a larger magnitude
negative number to a smaller magnitude positive number.

IntSafeLib does not protect you from such errors. It tells you if your
integer operations overflowed, not if you are doing the right thing
with your non-overflowed integers.

Likewise you can overflow a buffer with a non-overflowed unsigned index.

**Unit Tests**

Unit tests used for testing of all of the functions of this library are at
UnitTests/IntSafeLib, but they are not compiled due to dependencies on
UnitTestLib and UnitTestAssertLib.
