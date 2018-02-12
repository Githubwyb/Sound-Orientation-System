#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define u8  unsigned char  
#define u16 unsigned short
#define u32 unsigned int

#define F_SYS_CLK 44236800
#define F_PB_CLK  11059200

#define bittest(var, bit) ((var) & (1 <<(bit)))
#define bitset(var, bit) ((var) |= (1 << (bit)))
#define bitclr(var, bit) ((var) &= ~(1 << (bit)))

#endif 
