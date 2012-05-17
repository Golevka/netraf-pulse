#include <stdlib.h>
#include <fcntl.h>
#include <stropts.h>
#include <sys/syscall.h>
#include <linux/kd.h>


/* Open console and return its file descriptor, devfile would usually be 
   /dev/console
 */
int open_console_fd(const char *devfile)
{
    int rc = syscall(SYS_open, devfile, O_WRONLY, 7*64 + 7*8 + 7);
    if (rc == 0) rc = 1;

    return rc;
}

/* Set keyboard leds using the bit pattern specified by the lower 3 bits of
   leds: 
       [0][0][0][0][0][CAPSLOCK][NUMLOCK][SCRLOCK]
*/
int set_keyboard_leds(int rc, unsigned int leds)
{
    return ioctl(rc, KDSETLED, leds);
}

/* Return the current state of keyboard leds */
unsigned int get_keyboard_leds(int rc)
{
    char keyleds = 0;

    if (ioctl(rc, KDGETLED, &keyleds) == -1) /* get key leds state */
        return -1;    /* error */
    else 
        return (unsigned int)keyleds;
}

/* Get the "real" keyboard state rather than the state of leds 
   FIXME: this function does not work in X11.
 */
unsigned int get_keyboard_state(int rc)
{
    char keystate = 0;

    if (ioctl(rc, KDGKBLED, &keystate) == -1)
        return -1;
    else
        return (unsigned int)keystate;
}
