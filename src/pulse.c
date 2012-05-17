#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>

#include "defs.h"


/* Led control informations */
static struct keyleds_ctl
{
    int console_fd;            /* file descriptor of console device */

    pthread_t th_num, th_cap, th_scr;  /* one thread per keyled */
    pthread_mutex_t cs_mutex;  /* critical section mutex */

    volatile int t_keyled[8];  /* pulse interval of keyleds. only
                                * t_keyled[LED_NUM], t_keyled[LED_CAP] and
                                * t_keyled[LED_SCR] are actually used */
} ledctl;


/* Keyleds pulse thread to make the leds blinking */
static void *keyled_pulse(void *arg)
{
    unsigned int keyleds, iled = (unsigned int)(arg);
    int t = 0;

    for ( ; ; t++)
    {
        /* time limit exceed */
        if (t > ledctl.t_keyled[iled] && ledctl.t_keyled[iled] != 0)
        {
            pthread_mutex_lock(&ledctl.cs_mutex);

            if (ledctl.t_keyled[iled] != 0)
            {
                keyleds = get_keyboard_leds(ledctl.console_fd);

                set_keyboard_leds(ledctl.console_fd, 
                    (keyleds & iled)? 
                    (keyleds & (~iled)): (keyleds | iled));
            }

            t = 0;  /* reset pulse counter */
            pthread_mutex_unlock(&ledctl.cs_mutex);
        }
        
        usleep(10000);
    }

    return NULL;
}


/* Initialize keyboard leds control, ready for startup */
void init_netraf_leds_ctl(void)
{
    ledctl.console_fd = open_console_fd("/dev/console");
    ledctl.t_keyled[LED_NUM] = 0;
    ledctl.t_keyled[LED_CAP] = 0;
    ledctl.t_keyled[LED_SCR] = 0;

    pthread_mutex_init(&ledctl.cs_mutex, NULL);
}

/* Start led pulse threads to make the keyleds blinking */
void startup_netraf_leds(void)
{
    pthread_create(&ledctl.th_num, NULL, keyled_pulse, (void*)LED_NUM);
    pthread_create(&ledctl.th_cap, NULL, keyled_pulse, (void*)LED_CAP);
    pthread_create(&ledctl.th_scr, NULL, keyled_pulse, (void*)LED_SCR);
}


/* Set the pulse period of specified keyled */
void set_keyled_pulse_period(unsigned int iled, unsigned int period)
{
    ledctl.t_keyled[iled] = period;
}

/* Turn all key leds off */
void keyleds_off(void)
{
    pthread_mutex_lock(&ledctl.cs_mutex);
    set_keyboard_leds(ledctl.console_fd, 0);
    pthread_mutex_unlock(&ledctl.cs_mutex);
}
