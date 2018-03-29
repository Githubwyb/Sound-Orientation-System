#ifndef __DATA_H__
#define	__DATA_H__

#include <stdint.h>
/*
typedef enum  {
    false,
    true,
} bool;*/

typedef enum
{
    MK0 = 0x00,
    MK1,
    MK2,
    MK_ERROR,
}ENUM_MK;

typedef enum
{
    STATE_IDLE,
    STATE_WAIT_FIRST_PULSE,
    STATE_WAIT_SECOND_PULSE,
    STATE_WAIT_THIRD_PULSE,
    STATE_OVER,
    STATE_TIMEOUT,
    STATE_DELAY,
    STATE_WAIT,
}ENUM_PROCESS_STATE;

typedef struct
{
    ENUM_MK mk;
    uint16_t cntT;
}ST_PULSE_RECORD;
typedef struct
{
    ENUM_PROCESS_STATE processState;
    ST_PULSE_RECORD record[3];


    uint16_t degree;//°
    uint32_t distance; //mm

    uint16_t temp;
}ST_DATA;

extern ST_DATA data;

#endif	/* __DATA_H__ */

