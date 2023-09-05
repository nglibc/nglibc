/* Tests for *cvt function, long double version.
   Copyright (C) 1998-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#define ECVT qecvt
#define FCVT qfcvt
#define ECVT_R qecvt_r
#define FCVT_R qfcvt_r
#define FLOAT long double
#define PRINTF_CONVERSION "%Lf"

#define EXTRA_ECVT_TESTS

#include "tst-efgcvt-template.c"
