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
    keep_running = 0;
}

int load_targets(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("[EROARE] Nu s-a putut deschide fișierul %s.\n", filename);
        return 0;
    }

    char line[100];
    int count = 0;
    while (fgets(line, sizeof(line), file) && count < MAX_TARGETS) {
        line[strcspn(line, "\r\n")] = 0;
        
        if (strlen(line) > 0) {
            strncpy(targets[count].ip, line, sizeof(targets[count].ip));
            targets[count].packets_sent = 0;
            targets[count].packets_received = 0;
            targets[count].min_latency = 0;
            targets[count].max_latency = 0;
            targets[count].total_latency = 0;
            count++;
        }
    }
    fclose(file);
    return count;
}

int main() {
    const char *log_file = "network_log.csv";
    int scan_count = 1;

    signal(SIGINT, handle_sigint);

    printf("=== Pornire Monitor Rețea 2.0 ===\n");
    num_targets = load_targets("targets.txt");

    if (num_targets == 0) {
        printf("[EROARE] Nu s-a găsit niciun IP valid în targets.txt. Oprim programul.\n");
        return 1;
    }

    printf("S-au încărcat %d ținte din targets.txt\n", num_targets);
    printf("Monitorizarea rulează... Apasă Ctrl+C pentru raportul final.\n");
    init_logger(log_file);

    while (keep_running) {
        printf("\n--- Scanarea numărul %d ---\n", scan_count++);
        
        for (int i = 0; i < num_targets; i++) {
            if (!keep_running) break;

            printf("Se testează calitatea pentru %s (%d pachete)... ", targets[i].ip, PACKETS_PER_SCAN);
            fflush(stdout);

            int old_received = targets[i].packets_received;
            test_target_quality(&targets[i], PACKETS_PER_SCAN);
            int new_received = targets[i].packets_received;

            if (new_received > old_received) {
                double ultima_latenta = targets[i].max_latency;
                printf("ONLINE\n");
                log_ping_result(log_file, targets[i].ip, "ONLINE", ultima_latenta);
            } else {
                printf("OFFLINE (Packet Loss în această scanare)\n");
                log_ping_result(log_file, targets[i].ip, "OFFLINE", 0.00);
            }
        }

        if (keep_running) {
            printf("Așteptare 10 secunde...\n");
            for (int i = 0; i < 10 && keep_running; i++) {
                sleep(1);
            }
        }
    }

    printf("\n\n==================================================\n");
    printf("         RAPORT FINAL DE PERFORMANȚĂ REȚEA        \n");
    printf("==================================================\n");
    printf("%-16s | %-6s | %-6s | %-7s | %-7s\n", "IP Țintă", "Trim.", "Rec.", "Pierdere", "Lat.Med");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < num_targets; i++) {
        int lost = targets[i].packets_sent - targets[i].packets_received;
        double loss_percent = (targets[i].packets_sent > 0) ? ((double)lost / targets[i].packets_sent) * 100.0 : 0.0;
        double avg_latency = (targets[i].packets_received > 0) ? (targets[i].total_latency / targets[i].packets_received) : 0.0;

        printf("%-16s | %-6d | %-6d | %5.1f%%   | %5.2f ms\n", 
               targets[i].ip, 
               targets[i].packets_sent, 
               targets[i].packets_received, 
               loss_percent, 
               avg_latency);
    }
    printf("==================================================\n");
    printf("Program închis curat. Logurile au fost salvate în %s.\n", log_file);

    return 0;
}