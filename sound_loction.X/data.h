#ifndef __DATA_H__
#define	__DATA_H__

#include <stdint.h>
typedef enum 
{
    MK1,
    MK2,
    MK3,
    MK_NONE,
}ENUM_MK;

typedef enum 
{
    STATE_IDLE,
    STATE_WAIT_FIRST_PULSE,
    STATE_WAIT_SECOND_PULSE, 
    STATE_WAIT_THIRD_PULSE,    
    STATE_TIMEOUT,
    STATE_ERROR,
}ENUM_PROCESS_STATE;

typedef struct
{
    ENUM_MK mk;
    uint16_t deltaT;
}ST_PULSE_RECORD;
typedef struct
{
    ENUM_PROCESS_STATE processState;
    ST_PULSE_RECORD record1;
    ST_PULSE_RECORD record2;
    ST_PULSE_RECORD record3;
    uint16_t degree;
    uint16_t valueTemp;
    uint16_t temp2[15];
    uint16_t cntTemp;
}ST_DATA;

extern ST_DATA data;

#endif	/* __DATA_H__ */

