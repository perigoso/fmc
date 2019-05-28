#ifndef __UTILS_H__
#define __UTILS_H__

// Macro to get the bit value
#define BIT(x) (1 << (x))

// Macros that get the lesser/greater of two values
#define MAX(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define MIN(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

#endif  // __UTILS_H__
