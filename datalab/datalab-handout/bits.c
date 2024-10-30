/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
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
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


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

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

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
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
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
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  int xNoty = x & (~y);
  int yNotx = (~x) & y;
  return ~(~(xNoty) & ~(yNotx));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1<<31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  return !(!(x+1)) & !((~x) ^ (x+1));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  x = x & (x>>16); // 1,3,5,..,15 bits with 17,19...31 bits 
  x = x & (x>>8);  // 1,3,5,7 bits with 9,11,13,15 bits
  x = x & (x>>4);  
  x = x & (x>>2);
  return (x>>1) & 1;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  // check 4-th bit to 31st bit is 3
  int isThree = !((x>>4)^3);
  
  // check NOT (3-rd bit is 1 and 1st bit/2nd bit is 1)
  int leNine = !(x & 8) | !(x & 6);
  return isThree & leNine;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  // !x -> 0(x!=0), 1(x==0)
  // ~(!x) + 1 -> 0(x!=0), -1(x==0)
  int notX = !x;
  notX = ~notX + 1;

  // if x then y else z
  // equals to (not x and z) or (x and y)
  return (notX & z) | (~notX & y);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int ySubx = y + (~x+1);

  // x <= y ->  y-x >= 0 (if no overflow occurs)
  int ySubxGTZero = !(ySubx >> 31);

  // check if x y is negative
  int xNegative = x>>31;
  int yNegative = y>>31;

  // avoid y-x overflow, if x is Negative, y-x >= 0 if y is negative
  //                     if x is non-negative, y-x>=0 if y is non-negative
  return (xNegative & ((!yNegative) | ySubxGTZero)) | ((!xNegative) & ((!yNegative) & ySubxGTZero));
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int nx = ~x;
  nx = nx & (nx >> 16); // nx0 ... nx15 with  nx16 ... nx31
  nx = nx & (nx >> 8);  // nx0 ... nx7  with nx8 ... nx15  (get nx0 & nx16 & nx8 & nx 24) ... (nx7 & nx15 & nx23 & nx31)
  nx = nx & (nx >> 4);
  nx = nx & (nx >> 2);
  nx = nx & (nx >> 1);
  return nx & 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int inLowerHalf;
  int iBase16, iBase8, iBase4, iBase2, iBase1;
  // if x < 0, then x = ~x (change x to non-negative number)
  int nonNegative = !(x>>31);
  nonNegative = ~nonNegative + 1;
  x = (nonNegative & x) | (~nonNegative & (~x));

  // determine the index of most significant 1
  // 1. determine the 16--31 of x has 1
  // 2. if has 1, dicard 0--15 bits; else keep 0--15 bits. -> xNew
  // 3. determine the 8--15 bits of xNew has 1
  // 4. if has 1, discard 0--7 bits; else keep 0--7 bits. -> xNew
  // 5. determine the 4--7 bits of xNew has 1
  // 6. if has 1, discard 0--3 bits; else keep 0--3 bits. -> xNew
  // 7. determine the 2--3 bits of xNew has 1
  // 8. if has 1, discard 0--1 bits, else keep 0--1 bits. -> xNew
  // 9. determine 1 bit of xNew has 1
  // 10. add all the index
 
  inLowerHalf = !(x>>16);
  iBase16 = (!inLowerHalf)<<4;
  x = x>>iBase16;

  inLowerHalf = !(x>>8);
  iBase8 = (!inLowerHalf)<<3;
  x = x>>iBase8;

  inLowerHalf = !(x>>4);
  iBase4 = (!inLowerHalf)<<2;
  x = x>>iBase4;

  inLowerHalf = !(x>>2);
  iBase2 = (!inLowerHalf)<<1;
  x = x>>iBase2;

  inLowerHalf = !(x>>1);
  iBase1 = !inLowerHalf;
  x = x>>iBase1;

  return iBase16 + iBase8 + iBase4 + iBase2  + iBase1 + x + 1;

}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned result;
  int expBits, newExpBits;
  int signBit;
  
  result = uf;
  expBits = (uf >> 23) & (0xFF);
  signBit = uf >> 31;

  if (!(expBits ^ 0xFF)){
    // NaN / ininity
    return result;
  }

  if (!expBits){
    // Denorm form: exp part all 0
    return (result + result) | (signBit << 31);
  }

  // norm form
  newExpBits = expBits + 1;
  result = result ^ (expBits << 23);
  result = result ^ (newExpBits << 23);

  return result;
}

/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int outOfRange;
  int expBits;
  int signBit;
  int exp;
  int result;

  outOfRange = 1<<31;
  expBits = (uf >> 23) & (0xFF);
  signBit = uf >> 31;
  exp = expBits - 127;
  result = uf;

  if (!(expBits ^ 0xFF)){
    // NaN / ininity
    return outOfRange;
  }
  
  if (exp < 0){
     // absolute value of float < 1
     return 0;
  }

  // 清空sign bit
  result = result ^ (signBit<<31);
  
  // 清空exp bits，同时给frac bits最高位的高一位设置为1
  result = result ^ (expBits << 23);
  result = result | (1 << 23);
  if (exp <= 23){
      result = result >> (23-exp);
  } else if (exp < 31){
     // sign needs 1 bit, only 31 bits left for int
     result = result << (exp-23);
  } else{
     return outOfRange;
  }

  if (signBit){
    result = -result;
  }

  return result;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
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
unsigned floatPower2(int x) {
    int bias, fracBitsLen;
    unsigned inf;

    bias = (1<<7) - 1;
    fracBitsLen = 23;
    inf = 0xff << fracBitsLen;

    if (x < 1-bias-fracBitsLen){
       return 0;
    }
    if (x < 1-bias){
      // denorm, set fraction bits
      return 1<<(x+bias-1+fracBitsLen);
    }
    if (x <= bias){
      // norm, 2**x, set exponent bits (x = e - bias -> e = x+bias)
      return (x+bias)<<fracBitsLen;
    }
    // infinity
    return inf;
}

