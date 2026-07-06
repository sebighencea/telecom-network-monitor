#ifndef LOGGER_H
#define LOGGER_H

int init_logger(const char *filename);
int log_ping_result(const char *filename, const char *ip, const char *status, double latency);

#endif