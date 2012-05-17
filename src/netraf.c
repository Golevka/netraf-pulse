#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <ctype.h>



/* Skip current line / jump to the start of the next line */
void __skip_this_line(FILE *fp)
{
    /* **skip all characters until encountering an '\n'.**

       It might be a little bit slower than fgets, but using fgets would mess
       up the code because you have to make sure that the entire line has been
       eventually read into your temporary buffer.
    */
    int ret;
    while ((ret = fgetc(fp)) != '\n' && ret != EOF);
}


/* Read network traffic data of specified network interface from devfile
   (usually /proc/net/dev). Recieve and trasmit bytes are returned through
   *bytes_recv and *bytes_transmit.

   This function returns 0 on success, or it would return -1 to indicate a file
   error.
*/
int read_network_traffic(
    const char *devfile, const char *interface_name, 
    unsigned long *bytes_recv, unsigned long *bytes_transmit)
{
    char face[LINE_MAX];
    FILE *fp = fopen(devfile, "r");

    if (fp != NULL)
    {
        __skip_this_line(fp);   /* Inter-|   Receive */
        
        do {
            __skip_this_line(fp);

            /* first token of each line is name of network interface */
            if (fscanf(fp, "%s", face) == EOF) {
                fclose(fp); return -1; /* interface not found */
            }
            face[strlen(face) - 1] = '\0'; /* delete the trailing ":" */

        } while (strcmp(face, interface_name) != 0);

        /* network interface found */
        if (fscanf(fp, "%lu %*u %*u %*u %*u %*u %*u %*u %lu", 
                bytes_recv, bytes_transmit) != 2) {
            fclose(fp); return -1;  /* cannot read recv/transmit bytes */
        }

        fclose(fp); return 0;  /* success */
    }
    else {
        return -1;       /* fopen error */
    }
}


/* Estimate network traffic speed by measuring recved/transmitted bytes during
   a given time interval (seconds) 
*/
int measure_network_flow(
    const char *devfile, const char *interface_name, 
    unsigned long *recv_speed, unsigned long *trans_speed,
    unsigned int seconds)
{
    unsigned long 
        trans_beg = 0, trans_end = 0, 
        recv_beg  = 0, recv_end  = 0;

    if (read_network_traffic(
            devfile, interface_name, &recv_beg, &trans_beg) == -1) {
        return -1;
    }

    sleep(seconds);

    if (read_network_traffic(
            devfile, interface_name, &recv_end, &trans_end) == -1) {
        return -1;
    }

    *recv_speed  = (recv_end - recv_beg)/seconds;
    *trans_speed = (trans_end - trans_beg)/seconds;

    return 0;
}
