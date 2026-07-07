#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "../include/logger.h"
#include "../include/network.h"

#define MAX_TARGETS 20
#define PACKETS_PER_SCAN 3

TargetStats targets[MAX_TARGETS];
int num_targets = 0;
volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

int load_targets(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char line[100];
    while (fgets(line, sizeof(line), file) && num_targets < MAX_TARGETS) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) > 0) {
            strncpy(targets[num_targets].ip, line, sizeof(targets[num_targets].ip) - 1);
            targets[num_targets].packets_sent = 0;
            targets[num_targets].packets_received = 0;
            targets[num_targets].min_latency = -1.0;
            targets[num_targets].max_latency = 0.0;
            targets[num_targets].total_latency = 0.0;
            num_targets++;
        }
    }

    fclose(file);
    
    if (num_targets > 0) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    signal(SIGINT, handle_sigint);

    printf("=== Starting Network Monitor 2.0 ===\n");

    if (load_targets("targets.txt") == 0) {
        printf("[ERROR] Could not open targets.txt or file is empty.\n");
        return 1;
    }

    printf("Loaded %d targets from targets.txt\n", num_targets);
    
    const char *log_file = "network_log.csv";
    init_logger(log_file);

    while (keep_running) {
        for (int i = 0; i < num_targets && keep_running; i++) {
            test_target_quality(&targets[i], PACKETS_PER_SCAN);
            log_stats(log_file, &targets[i]);
        }

        if (keep_running) {
            printf("Waiting 10 seconds...\n");
            for (int s = 0; s < 10 && keep_running; s++) {
                sleep(1);
            }
        }
    }

    printf("\n\n======================================================\n");
    printf("           FINAL NETWORK PERFORMANCE REPORT           \n");
    printf("======================================================\n");
    printf("%-16s | %-6s | %-6s | %-8s | %-12s\n", "Target IP", "Sent", "Recv", "Loss", "Avg Latency");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < num_targets; i++) {
        int lost = targets[i].packets_sent - targets[i].packets_received;
        double loss_percent = 0.0;
        double avg_latency = 0.0;

        if (targets[i].packets_sent > 0) {
            loss_percent = ((double)lost / targets[i].packets_sent) * 100.0;
        }

        if (targets[i].packets_received > 0) {
            avg_latency = targets[i].total_latency / targets[i].packets_received;
        }

        printf("%-16s | %-6d | %-6d | %5.1f%%   | %5.2f ms\n",
               targets[i].ip,
               targets[i].packets_sent,
               targets[i].packets_received,
               loss_percent,
               avg_latency);
    }

    printf("======================================================\n");
    printf("Program closed cleanly. Log saved in %s.\n", log_file);

    return 0;
}