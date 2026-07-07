#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/logger.h"

void init_logger(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return;
    }

    file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "Timestamp,IP,PacketsSent,PacketsReceived,LossPercent,MinLatency,MaxLatency,AvgLatency\n");
        fclose(file);
    }
}

void log_stats(const char *filename, const TargetStats *stats) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    int lost = stats->packets_sent - stats->packets_received;
    double loss_percent = 0.0;
    double avg_latency = 0.0;

    if (stats->packets_sent > 0) loss_percent = ((double)lost / stats->packets_sent) * 100.0;
    if (stats->packets_received > 0) avg_latency = stats->total_latency / stats->packets_received;

    fprintf(file, "%s,%s,%d,%d,%.1f,%.2f,%.2f,%.2f\n",
            timestamp, stats->ip, stats->packets_sent, stats->packets_received,
            loss_percent, stats->min_latency, stats->max_latency, avg_latency);

    fclose(file);
}