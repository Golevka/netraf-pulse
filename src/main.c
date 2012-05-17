#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "defs.h"



int speed_to_pulse_period(unsigned long speed_kbytes)
{
    if (speed_kbytes == 0)        return 0;
    else if (speed_kbytes < 30)   return 30;
    else if (speed_kbytes < 50)   return 20;
    else if (speed_kbytes < 100)  return 10;
    else                          return 5;
}

int main(int argc, char *argv[])
{
    unsigned long recv_speed, trans_speed, overall_speed;
    const char *iface = argv[1];   /* network interface */

    if (argc == 2)
    {
        /* initialize led control */
        init_netraf_leds_ctl();
        startup_netraf_leds();

        for ( ; ; )
        {
            measure_network_flow("/proc/net/dev", iface, 
                &recv_speed, &trans_speed, 1);

            recv_speed  /= 1024;
            trans_speed /= 1024;
            overall_speed = trans_speed + recv_speed;

            set_keyled_pulse_period(LED_CAP, speed_to_pulse_period(recv_speed));
            set_keyled_pulse_period(LED_SCR, speed_to_pulse_period(trans_speed));
            set_keyled_pulse_period(LED_NUM, speed_to_pulse_period(overall_speed/50));

            keyleds_off();
        }
    }
    else {
        printf("usage: %s iface\n", argv[0]);
    }

    return 0;
}
