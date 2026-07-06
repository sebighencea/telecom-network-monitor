#include "../include/network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double ping_ip(const char *ip_address) {
    char command[100];
    char response[256];
    double latency = -1.0;

    // Comandă nativă de Mac: trimite 1 pachet (-c 1) și așteaptă max 2 secunde (-t 2)
    sprintf(command, "ping -c 1 -t 2 %s 2>&1", ip_address);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return -1.0;
    }

    while (fgets(response, sizeof(response), fp) != NULL) {
        char *time_ptr = strstr(response, "time=");
        if (time_ptr != NULL) {
            sscanf(time_ptr, "time=%lf", &latency);
            break;
        }
    }
    pclose(fp);

    return latency;
}