#ifndef _NETRAF_PULSE_DEFS_H_
#define _NETRAF_PULSE_DEFS_H_


#include <linux/kd.h>


/* Read network traffic data of specified network interface from devfile
   (usually /proc/net/dev). Recieve and trasmit bytes are returned through
   *bytes_recv and *bytes_transmit.

   This function returns 0 on success, or it would return -1 to indicate a file
   error. */
int read_network_traffic(
    const char *devfile, const char *interface_name, 
    unsigned long *bytes_recv, unsigned long *bytes_transmit);

/* Estimate network traffic speed by measuring recved/transmitted bytes during
   a given time interval (seconds).  */
int measure_network_flow(
    const char *devfile, const char *interface_name, 
    unsigned long *recv_speed, unsigned long *trans_speed,
    unsigned int seconds);



/* Open console and return its file descriptor, devfile would usually be 
   /dev/console */
int open_console_fd(const char *devfile);

/* Set keyboard leds using the bit pattern specified by the lower 3 bits of
   leds: 
       [0][0][0][0][0][CAPSLOCK][NUMLOCK][SCRLOCK] 
*/
int set_keyboard_leds(int rc, unsigned int leds);

/* Return the current state of keyboard leds */
unsigned int get_keyboard_leds(int rc);

/* Get the "real" keyboard state rather than the state of leds */
unsigned int get_keyboard_state(int rc);



void init_netraf_leds_ctl(void);  /* Initialize keyboard leds control, ready
                                   * for startup */

void startup_netraf_leds(void);   /* Start led pulse threads to make the
                                   * keyleds blinking */

/* Set the pulse period of specified keyled */
void set_keyled_pulse_period(unsigned int iled, unsigned int period);

void keyleds_off(void);     /* Turn all key leds off */



#endif /* _NETRAF_PULSE_DEFS_H_ */
