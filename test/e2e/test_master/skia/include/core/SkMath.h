/*
 * Copyright 2006 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkMath_DEFINED
#  define SkMath_DEFINED
#  include "include/core/SkTypes.h"
// 64bit -> 32bit utilities

// Handy util that can be passed two ints, and will automatically promote to
// 64bits before the multiply, so the caller doesn't have to remember to cast
// e.g. (int64_t)a * b;
static int64_t sk_64_mul(int64_t a, int64_t b)
{
  return a * b;
}
///////////////////////////////////////////////////////////////////////////////

/** Given an integer and a positive (max) integer, return the value
 *  pinned against 0 and max, inclusive.
 *  @param value    The value we want returned pinned between [0...max]
 *  @param max      The positive max value
 *  @return 0 if value < 0, max if value > max, else value
 */
static int SkClampMax(int value, int max)
{
    // ensure that max is positive
  SkASSERT(max >= 0);
  if (value < 0)
  {
    value = 0;
  }
  if (value > max)
  {
    value = max;
  }
  return value;
}
/**
 *  Returns true if value is a power of 2. Does not explicitly check for
 *  value <= 0.
 */
template <typename T>
inline constexpr bool SkIsPow2(T value)
{
  return (value & (value - 1)) == 0;
}
///////////////////////////////////////////////////////////////////////////////

/**
 *  Return a*b/((1 << shift) - 1), rounding any fractional bits.
 *  Only valid if a and b are unsigned and <= 32767 and shift is > 0 and <= 8
 */
static unsigned SkMul16ShiftRound(U16CPU a, U16CPU b, int shift)
{
  SkASSERT(a <= 32767);
  SkASSERT(b <= 32767);
  SkASSERT(shift > 0 && shift <= 8);
  unsigned prod = a * b + (1 << (shift - 1));
  return (prod + (prod >> shift)) >> shift;
}
/**
 *  Return a*b/255, rounding any fractional bits.
 *  Only valid if a and b are unsigned and <= 32767.
 */
static U8CPU SkMulDiv255Round(U16CPU a, U16CPU b)
{
  SkASSERT(a <= 32767);
  SkASSERT(b <= 32767);
  unsigned prod = a * b + 128;
  return (prod + (prod >> 8)) >> 8;
}
#endif
