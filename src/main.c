#include <stdio.h>
#include <unistd.h> 
#include "../include/logger.h"
#include "../include/network.h"

int main() {
    const char *log_file = "network_log.csv";
    const char *targets[] = {"127.0.0.1", "8.8.8.8", "1.1.1.1"}; 
    int num_targets = sizeof(targets) / sizeof(targets[0]);
    int count = 1;

    printf("=== Pornire Monitor Rețea Permanent (Apasă Ctrl+C pentru oprire) ===\n");
    init_logger(log_file);

    
    while (1) {
        printf("\n--- Scanarea numărul %d ---\n", count++);
        
        for (int i = 0; i < num_targets; i++) {
            printf("Se verifică %s... ", targets[i]);
            
            double lat = ping_ip(targets[i]);

            if (lat >= 0) {
                printf("ONLINE (%.2f ms)\n", lat);
                log_ping_result(log_file, targets[i], "ONLINE", lat);
            } else {
                printf("OFFLINE\n");
                log_ping_result(log_file, targets[i], "OFFLINE", 0.00);
            }
        }

        printf("Așteptare 10 secunde până la următoarea verificare...\n");
        
        
        sleep(10); 
    }

    return 0; 
}