#include "../include/logger.h"
#include <stdio.h>
#include <time.h>

int init_logger(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 0; 
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Eroare la crearea fisierului de log");
        return -1;
    }

    fprintf(file, "Timestamp,IP Address,Status,Latency (ms)\n");
    fclose(file);
    return 1;
}

int log_ping_result(const char *filename, const char *ip, const char *status, double latency) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Eroare la deschiderea fisierului pentru adaugare log");
        return -1;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", t);

    fprintf(file, "%s,%s,%s,%.2f\n", time_str, ip, status, latency);

    fclose(file);
    return 0;
}