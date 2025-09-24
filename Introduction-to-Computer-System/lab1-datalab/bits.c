/* 
 * CS:APP Data Lab 
 * 
 * <xiyouluo>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2024 Free Software Foundation, Inc.
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
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */

/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  /* 
    x | y == ~(~x & ~y);
    The truth table shows the left-hand side is 0 only with the input (0, 0). 
    We can achieve this by taking the & operator's property of yielding 1 only for (1, 1) 
    and simply applying the right negations.
  */
  return ~(~x & ~y);
}
/* 
 * upperBits - pads n upper bits with 1's
 *  You may assume 0 <= n <= 32
 *  Example: upperBits(4) = 0xF0000000
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 1
 */
int upperBits(int n) {
/*
 * Approach 1: Inversion Method
 *
 * Instead of building the mask of upper 1's directly, it's easier to
 * build the inverse mask (lower 1's) and then negate the result.
 *
 * A mask with (32-n) lower bits set to 1 can be expressed as
 * (1 << (32 - n)) - 1. This works for 0 < n <= 32.
 * However, when n = 0, the shift amount becomes 32, which is
 * Undefined Behavior (UB) in C.
 *
 * To handle the n=0 case, we can replace the literal '1' with '!!n'.
 * Since logical operators treat values as either 0 or non-zero, '!!n'
 * evaluates to 0 if n is 0, and 1 otherwise.
 * This prevents the illegal shift when n=0. The final expression is
 * ~((!!n << (32 - n)) - 1).
 */
  // return ~((1 << (32 - n)) - 1);
  // return ~((!!n << (32 + (~n + 1))) + ~0);
/*
 * Approach 2: Arithmetic Right Shift Method
 *
 * This approach leverages the property of arithmetic right shifts, which
 * fill the upper bits with the sign bit.
 *
 * First, we create a value with only the sign bit set: (1 << 31).
 * Then, we arithmetically right-shift it by (n-1) positions to create
 * n 1's in the upper bits. Since the '-' operator is disallowed, (n-1)
 * is calculated as (n + ~0).
 *
 * The naive expression (1 << 31) >> (n + ~0) causes UB when n=0 because
 * the shift amount becomes -1.
 *
 * Similar to the first approach, we fix this by replacing the initial '1'
 * with '!!n'. If n=0, the value to be shifted is 0, resulting in 0,
 * which is the correct answer. If n > 0, the value is (1 << 31) as intended.
 * The final expression is (!!n << 31) >> (n + ~0).
 */
  return (!!n << 31) >> (n + ~0);
}
/*
 * fullAdd - 4-bits add using bit-wise operations only.
 *   (0 <= x, y < 16) 
 *   Example: fullAdd(12, 7) = 3,
 *            fullAdd(7, 8) = 15,
 *   Legal ops: ~ | ^ & << >>
 *   Max ops: 30
 *   Rating: 2
 */
int fullAdd(int x, int y) {
/*
 * 1 + 1 -> sum = 0, carry = 1
 * ...
 * sum -> ^, carry -> &
 */
  int sum0 = x ^ y, carry0 = (x & y) << 1;
  int sum1 = sum0 ^ carry0, carry1 = (sum0 & carry0) << 1;
  int sum2 = sum1 ^ carry1, carry2 = (sum1 & carry1) << 1;
  
  return (sum2 ^ carry2) & 0xf;
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
/*
*/
  // int lower_mask = (!!n << (32 - n)) - 1, upper_mask = ~lower_mask;
  int lower_mask = (!!n << (33 + ~n)) + ~0, upper_mask = ~lower_mask;
  int lower_bits = x & lower_mask, upper_bits = x & upper_mask;
  return lower_bits << n | (upper_bits >> 33 + ~n) & ((1 << n) + ~0);  // Arithmetic shift!!
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
/*
*/
  int xor16 = (x >> 16) ^ x;
  int xor8 = (xor16 >> 8) ^ xor16;
  int xor4 = (xor8 >> 4) ^ xor8;
  int xor2 = (xor4 >> 2) ^ xor4;
  int xor1 = (xor2 >> 1) ^ xor2;
  return xor1 & 1;
}
/* 
 * palindrome - return 1 if x is palindrome in binary form,
 *   return 0 otherwise
 *   A number is palindrome if it is the same when reversed
 *   YOU MAY USE BIG CONST IN THIS PROBLEM, LIKE 0xFFFF0000
 *   YOU MAY USE BIG CONST IN THIS PROBLEM, LIKE 0xFFFF0000
 *   YOU MAY USE BIG CONST IN THIS PROBLEM, LIKE 0xFFFF0000
 *   Example: palindrome(0xff0000ff) = 1,
 *            palindrome(0xff00ff00) = 0
 *   Legal ops: ~ ! | ^ & << >> +
 *   Max ops: 40
 *   Rating: 4

 */
int palindrome(int x) {
/*
*/
  // Logical shift!
  // unsigned y = x;  dlc:bits.c:306:palindrome: Illegal data type: unsigned
  int y = x;
  y = ((y & 0x55555555) << 1) | ((y & 0xaaaaaaaa) >> 1) & 0x7fffffff;  // manual logical shift by & 0x7fffffff
  y = ((y & 0x33333333) << 2) | ((y & 0xcccccccc) >> 2) & 0x3fffffff;
  y = ((y & 0x0f0f0f0f) << 4) | ((y & 0xf0f0f0f0) >> 4) & 0x0fffffff;
  y = ((y & 0x00ff00ff) << 8) | ((y & 0xff00ff00) >> 8) & 0x00ffffff;
  // y = ((y & 0x0000ffff) << 16) | ((y & 0xffff0000) >> 16) & 0x0000ffff;
  y = y << 16 | y >> 16 & 0x0000ffff;
  return !(x ^ y);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
/*
*/
  return ~x + 1;
}
/* 
 * oneMoreThan - return 1 if y is one more than x, and 0 otherwise
 *   Examples oneMoreThan(0, 1) = 1, oneMoreThan(-1, 1) = 0
 *   Legal ops: ~ & ! ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int oneMoreThan(int x, int y) {
/* (x+1) ^ y == 0
 * x = 2147483647 -> overflow
 */ 
  int diff_sign = (x ^ y) >> 31;
  // return !diff_sign & !((x + 1) ^ y) | diff_sign & !~x & !y;
  return !diff_sign & !(y + ~x) | diff_sign & !~x & !y;
}
/*
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
/* (x + 2 * x) / 4
 * negtive: x + (x << 1) + bias >> 2; mask = ((1 << 2) - 1) = 3;
 */
  // return x + (x << 1) + (x >> 31 & 3) >> 2;
  int three_x = x + (x << 1), bias = three_x >> 31 & 3;
  return (three_x + bias) >> 2;
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
/* x + (~y + 1) >> 31 & 1;
 * overflow!! 
 * diff_sign_mask & result_when_diff | same_sign_mask & result_when_same;
 */
return  (((x ^ y) >> 31) & (x >> 31) | ~((x ^ y) >> 31) & (x + (~y + 1) >> 31)) & 1;
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x90000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
/*
 * x + x;
 * overflow.
 * overflow_mask_x<0 & TMin | overflow_mask_x>=0 & TMax | not_overflow_mask & 2x
 */
  int y = x + x;
  return (((x ^ y) & x) >> 31) & (1 << 31) | (((x ^ y) & ~x) >> 31) & ~(1 << 31) | (~(x ^ y) >> 31) & y;
}
/*
 * modThree - calculate x mod 3 without using %.
 *   Example: modThree(12) = 0,
 *            modThree(2147483647) = 1,
 *            modThree(-8) = -2,
 *   Legal ops: ~ ! | ^ & << >> +
 *   Max ops: 60
 *   Rating: 4
 */
int modThree(int x) {
/*
 * The solution is based on the mathematical property that for any integer x,
 * its value modulo 3 is congruent to the sum of its parts, split at an
 * **even** power of 2. For a 16-bit split (k=16):
 *
 * x = H * 2^{16} + L
 * Since 2^{16} = (2^2)^8 = 4^8, and 4 is congruent to 1 (mod 3),
 * it follows that 2^16 is congruent to 1^8 = 1 (mod 3).
 * Therefore, x is congruent to (H*1 + L) = (H + L) (mod 3).
 * This property allows us to "fold" the number while preserving its mod 3 value.
 * 
 */
  int original_sign, mask, result_is_positive, needs_correction;
  original_sign = x >> 31;

  x = (x >> 16) + (x & (1 << 16) + ~0);  // notice carry! max: 2^{15} - 1 + 2^{16} - 1 -> 17 bits; cannot use 0xffff
  x = (x >> 8) + (x & 0xff);  // up to 636, 10 bits
  x = (x >> 4) + (x & 0xf);  // up to 51, 6 bits
  x = (x >> 2) + (x & 0x3);  // up to 15, 4 bits
  x = (x >> 2) + (x & 0x3);  // up to 6, 3 bits
  x = (x >> 2) + (x & 0x3);  // up to 3, 3 bits

  // if (x >= 3) x -= 3;  
  mask = ~((x + ~2) >> 31);  // x - 3; mask == -1 if x >= 3 else 0;
  x += ~2 & mask;  // x -= 3 & mask

  // if (x <= -3) x += 3;
  mask = (x + 2) >> 31;  // find f(x) which >= 0 when x > -3 (x >= -2) and < 0 when x <= -3
  x += 3 & mask;
  
  result_is_positive = !((x >> 31) | !x); // 1 if x > 0 else 0
  needs_correction = original_sign & result_is_positive; // 1 if correction needed
  x += ~2 & (~needs_correction + 1);  // 1 -> -1, 0 -> 0; bool -> mask

  return x;
}
/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
/* 1. NaN, inf: return uf;
 * 2. Denormalized: M >>= 1, rounding;
 * 3. Normalized -> Denormalized!
 * 4. Normalized: E -= 1;
 */
  unsigned sign = 1 << 31 & uf, exp = uf & 0x7f800000, frac = uf & 0x7fffff;

  if (exp == 0x7f800000) return uf;

  else if (exp == 0) {
    unsigned round_bit = (frac & 3) == 3;  // 3 is 0b11 not 0x11
    frac = (frac >> 1) + round_bit;
    // return sign | exp | frac;
    return sign | frac;
  }

  else if (exp == 0x00800000) {
    unsigned m = 1 << 23 | frac, round_bit = (m & 3) == 3;
    m = (m >> 1) + round_bit;
    return sign | m;
  }

  else {
    exp -= (1 << 23);
    return sign | exp | frac;
  }
}
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
/* (-1)^s * M * 2^E
 * 0. Normalized!
 * 1. 0, pos, neg
 * 2. Rounding
 */
  unsigned sign, exp, frac, MSB, shift, half, LSB;
  // unsigned temp_x, discarded;

  if (x == 0) return 0;
  if (x == 0x80000000) return 0xcf000000;

  // unsigned sign = x & 0x80000000, exp, frac;
  sign = x & 0x80000000;
  if (sign) x = -x;  // can use "-" here, less ops

  // unsigned MSB = 0, shift = 16;
  /* MSB = 0, shift = 16, temp_x = x;
  while (shift > 0) {
    if (temp_x >> shift) {
      MSB += shift;
      temp_x >>= shift;
    }
    shift >>= 1;
  }
   */
  MSB = 30;  // 31 is sign bit
  while (!(x >> MSB)) MSB -- ;  // dlc counts ops only once even if it's inside a loop

  exp = MSB + 127;
  frac = x & ~(1 << MSB); // Hidden leading one!

  /* too many ops for rounding!
  if (MSB > 23) {
    // unsigned shift = MSB - 23, discarded = x & ((1 << shift) - 1), half = 1 << (shift - 1);
    shift = MSB - 23, discarded = frac & ((1 << shift) - 1), half = 1 << (shift - 1);
    frac >>= shift;
    if (discarded > half || discarded == half && (frac & 1)) frac ++ ;
  }
   */
  if (MSB > 23) {
    shift = MSB - 23, half = 1 << (shift - 1), LSB = (frac >> shift) & 1;
    frac += half - 1 + LSB;  
    /*
     * When the low 'shift' bits of frac are >= half + 1 (> half), we must round up (carry).
     * When they are <= half - 1 (< half), we must round down (do not carry).
     *
     * Thus adding (half - 1) to frac cleverly handles this carrying/not-carrying behavior
     * before the final right shift, after which the low 'shift' bits are discarded. 
     * (>= half + 1) + half -> round up, (<= half - 1) + half -> round down.
     *
     * The LSB is the new least significant bit after frac is right-shifted. It is
     * either 0 or 1, and is used to handle the tie-breaking case where the low
     * 'shift' bits of frac are exactly equal to half.
     */
    frac >>= shift;
  }
  else frac <<= (23 - MSB);

  if (frac == 0x800000) { frac = 0; exp ++ ; }  // frac overflows!

  return sign | exp << 23 | frac;
}
/* 
 * float64_f2i - Return bit-level equivalent of expression (int) f
 *   for 64 bit floating point argument f.
 *   Argument is passed as two unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   double-precision floating point value.
 *   Notice: uf1 contains the lower part of the f64 f
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 20
 *   Rating: 4
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
/* 1. NaN; inf - > overflow
 * 2. Denormalized -> 0;
 * 3. Normalized: Hidden leading one!
 */
  unsigned sign = uf2 >> 31 & 1, M = 0x80000000 | (uf2 & 0xfffff) << 11 | uf1 >> 21, ret;
  int E = (uf2 >> 20 & 0x7ff) - 1023;  // E must be signed!
  // M = [Hidden leading one] [20 uf2 lower bits] [11 uf1 upper bits]
  if (E < 0) return 0;  // The (int)f cast rounds toward zero (truncates)!
  /* else if (E > 31) return 0x80000000u;
  else if (E == 31) {
      if (M == 1 && sign) return 0x80000000;
      else return 0x80000000u;
    }
   */
  else if (E >= 31) return 0x80000000;
  else ret = M >> (31 - E);  // M / 2^{31} * 2^E
  if (sign) ret = ~ret + 1;
  return ret;
}
/* 
 * float_pwr2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */


unsigned float_pwr2(int x) {
/* No casting, "build" it directly!
 */
  if (x > 127) return 0x7f800000;  // exp = x + 127 > 254
  else if (x > -127) return x + 127 << 23;  // exp > 0
  // else if (exp == 0)
    /* no "exp = x + 127" anymore; exp = 0, E = 1 - 127 = -126; x < -126;
     * 2.0^x = 0.frac * 2^{-126}; -> 2.0^{x + 126} = 0.frac; -> 
     * such as -(x + 126) = 1, 0.frac should be 0.1000... (22 0's, 23 bits)
     * we can see that frac = 1 << 23 - (-(x + 126)) = 1 << x + 149;
     */
  else if (x >= -149) return 1 << x + 149;
  else return 0;  // underflow, x < -149
}

