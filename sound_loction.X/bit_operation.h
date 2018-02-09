#ifndef __BIT_OPERATION_H
#define __BIT_OPERATION_H

#define bittest(var, bit) ((var) & (1 <<(bit)))
#define bitset(var, bit) ((var) |= (1 << (bit)))
#define bitclr(var, bit) ((var) &= ~(1 << (bit)))

#endif