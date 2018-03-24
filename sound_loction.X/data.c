
#include "data.h"
ST_DATA data = 
{
    .processState = STATE_IDLE,
    .record[0].mk = MK_ERROR,
    .record[0].cntT = 0,
    .record[1].mk = MK_ERROR,
    .record[1].cntT = 0,    
    .record[2].mk = MK_ERROR,
    .record[2].cntT = 0,    
    .degree = 0
};
ST_DATA *pData = &data;