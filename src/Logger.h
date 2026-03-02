#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include "Logger.h"
#include "Config.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

enum Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
private:
    std::ofstream file;

public:
    Logger(const std::string& filename);
    ~Logger();

    void log(int clock, const std::string& event, Level level);
    void logStart(Config config);
    void logSummary(int total_requests, int processing, int streaming,
                    int blocked, int servers_added, int servers_removed,
                    int start_queue, int end_queue,
                    int min_task_time, int max_task_time,
                    int active_servers, int inactive_servers);
};

#endif