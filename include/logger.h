#ifndef LOGGER_H
#define LOGGER_H

#include "network.h"

void init_logger(const char *filename);
void log_stats(const char *filename, const TargetStats *stats);

#endif