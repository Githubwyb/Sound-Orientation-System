
#include "data.h"
ST_DATA data = 
{
    .processState = STATE_IDLE,
    .record1.mk = MK_NONE,
    .record1.deltaT = 0,
    .record2.mk = MK_NONE,
    .record2.deltaT = 0,    
    .record3.mk = MK_NONE,
    .record3.deltaT = 0,    
    .degree = 0
};
ST_DATA *pData = &data;