#ifndef __UTILS_H__
#define __UTILS_H__

// Macro to get the bit value
#define BIT(x) (1 << (x))

// Extact
#define HALFWRD(variable, offset) (((variable) >> (offset)) & 0xFFFF)
#define BYTE(variable, offset) (((variable) >> (offset)) & 0xFF)
#define NIBBLE(variable, offset) (((variable) >> (offset)) & 0x0F)

// Macros that get the lesser/greater of two values
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

// Absolute value of
#define ABS(a)      ((a) < 0 ? (-(a)) : (a))

// Swap two variables
#define SWAP(a, b)  do{ typeof(a) SWAP = a; a = b; b = SWAP; }while(0)

#endif  // __UTILS_H__
