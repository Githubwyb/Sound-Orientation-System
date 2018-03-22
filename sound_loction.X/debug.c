#include <string.h>
#include <stdio.h>
#include "log.h"
#include "debug.h"
#include "protocol.h"

#include "led.h"

#define MAX_CMD_LENGTH (16)
#define MAX_CMD_NUMBER  (32)

typedef struct
{
    unsigned char cmd[MAX_CMD_LENGTH];
    CMD_ACTION  action;
}CMD_MAP;

static int cmd_pwm(const unsigned char* cmdString, unsigned short length)
{
    int i = 0;
    
    sscanf((const char *)cmdString, "pwm:%d", &i);
    if(i > 1024)
    {
        i = 1024;
    }
    else if(i<0)
    {
        i =0;
    }
    LOG_DEBUG("set pwm to %d", i);
    led_state_setLevel(i);
    return 0;
}

static CMD_MAP cmd_map[MAX_CMD_NUMBER] =
{
        {"pwm",       cmd_pwm},
};

int debug_proc(const unsigned char* cmdString, unsigned short length)
{
    int i = 0;
    const unsigned char* cmd = string_trimLeft(cmdString);

    for (i = 0; i < MAX_CMD_NUMBER && cmd_map[i].action; i++)
    {
        if (strncmp((const char *)cmd, (const char *)cmd_map[i].cmd, strlen((const char *)cmd_map[i].cmd)) == 0)
        {
            return cmd_map[i].action(cmdString, length);
        }
    }

    LOG_INFO("CMD not processed");
    return -1;
}