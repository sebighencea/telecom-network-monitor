#ifndef NETWORK_H
#define NETWORK_H

typedef struct {
    char ip[50];
    int packets_sent;
    int packets_received;
    double min_latency;
    double max_latency;
    double total_latency;
} TargetStats;

double ping_ip(const char *ip);
void test_target_quality(TargetStats *stats, int num_packets);

#endif