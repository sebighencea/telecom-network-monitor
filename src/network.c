#include "../include/network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double ping_ip(const char *ip) {
    char command[256];
    snprintf(command, sizeof(command), "ping -c 1 -t 1 %s 2>/dev/null | grep 'time=' | awk -F'time=' '{print $2}' | awk '{print $1}'", ip);
    
    FILE *fp = popen(command, "r");
    if (fp == NULL) return -1.0;

    char output[32];
    if (fgets(output, sizeof(output), fp) != NULL) {
        double latency = atof(output);
        pclose(fp);
        return (latency > 0.0) ? latency : -1.0;
    }

    pclose(fp);
    return -1.0;
}

void test_target_quality(TargetStats *stats, int num_packets) {
    for (int p = 0; p < num_packets; p++) {
        stats->packets_sent++;
        double lat = ping_ip(stats->ip);

        if (lat >= 0) {
            stats->packets_received++;
            stats->total_latency += lat;
            
            if (lat < stats->min_latency || stats->min_latency == 0) stats->min_latency = lat;
            if (lat > stats->max_latency) stats->max_latency = lat;
        }
    }
}