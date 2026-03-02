#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

/**
 * @brief Construct a new Logger:: Logger object
 * 
 * @param filename 
 */
Logger::Logger(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
    file << "=== Load Balancer Log ===" << std::endl;
    std::cout << "=== Load Balancer Log ===" << std::endl;
}

/**
 * @brief Destroy the Logger:: Logger object
 * 
 */
Logger::~Logger() {
    if (file.is_open())
    {
        file.flush();
        file.close();
    }
}

/**
 * @brief 
 * 
 * @param clock 
 * @param event 
 * @param level 
 */
void Logger::log(int clock, const std::string& event, Level level) {
    file << "[Cycle " << clock << "] " << event << std::endl;

    switch(level) {
        case ERROR:
            std::cout << RED << "[DEBUG] " << event << std::endl;
            break;
        case DEBUG:
            std::cout << GREEN << event << std::endl;
            break;
        case WARNING:
            std::cout << YELLOW << event << std::endl;
            break;
        case INFO:
            std::cout << CYAN << event << std::endl;
            break;
    }
}

/**
 * @brief 
 * 
 * @param config 
 */
void Logger::logStart(Config config) {
    file << "\n=== START ===\n";
    file << "Number of servers  : " << config.num_servers << "\n";
    file << "Number of cycles   : " << config.cycles << "\n";
    file << "Min queue ratio    : " << config.min_queue_ratio << "\n";
    file << "Max queue ratio    : " << config.max_queue_ratio << "\n";
    file << "Server wait cycles : " << config.server_wait_cycles << "\n";
    file << "Log file           : " << config.log_file << "\n";
    file << "Blocked IP ranges  : ";
    for (const auto& ip_range : config.blocked_ip_ranges) {
        file << ip_range << " ";
    }
    file << std::endl << std::endl;

    std::cout << RESET << "\n=== START ===\n";
    std::cout << "Number of servers  : " << config.num_servers << "\n";
    std::cout << "Number of cycles   : " << config.cycles << "\n";
    std::cout << "Min queue ratio    : " << config.min_queue_ratio << "\n";
    std::cout << "Max queue ratio    : " << config.max_queue_ratio << "\n";
    std::cout << "Server wait cycles : " << config.server_wait_cycles << "\n";
    std::cout << "Log file           : " << config.log_file << "\n";
    std::cout << "Blocked IP ranges  : ";
    for (const auto& ip_range : config.blocked_ip_ranges) {
        std::cout << ip_range << " ";
    }
    std::cout << std::endl << std::endl;
}

/**
 * @brief 
 * 
 * @param total_requests 
 * @param processing 
 * @param streaming 
 * @param blocked 
 * @param servers_added 
 * @param servers_removed 
 * @param start_p_queue 
 * @param start_s_queue 
 * @param end_p_queue 
 * @param end_s_queue 
 * @param min_task_time 
 * @param max_task_time 
 * @param active_p_servers 
 * @param active_s_servers 
 * @param inactive_p_servers 
 * @param inactive_s_servers 
 */
void Logger::logSummary(
    int total_requests, int processing, int streaming,
    int blocked, int servers_added, int servers_removed,
    int start_queue, int end_queue,
    int min_task_time, int max_task_time,
    int active_servers, int inactive_servers
)
{
    file << "\n=== SUMMARY ===\n";
    file << "Total requests generated : " << total_requests << "\n";
    file << "Processing requests      : " << processing << "\n";
    file << "Streaming requests       : " << streaming << "\n";
    file << "Blocked requests         : " << blocked << "\n";
    file << "Servers added            : " << servers_added << "\n";
    file << "Servers removed          : " << servers_removed << "\n";
    file << "Starting Queue           : " << start_queue << "\n";
    file << "Ending Queue             : " << end_queue << "\n";
    file << "Task time range          : " << min_task_time << " - " << max_task_time << " cycles\n";
    file << "Active Servers           : " << active_servers << "\n";
    file << "Idle Servers             : " << inactive_servers << std::endl;

    // console mirrors the file but with color
    std::cout << "\033[36m\n=== SUMMARY ===\033[0m\n";
    std::cout << "\033[32mTotal requests generated : " << total_requests << "\033[0m\n";
    std::cout << "\033[32mProcessing requests      : " << processing     << "\033[0m\n";
    std::cout << "\033[32mStreaming requests       : " << streaming       << "\033[0m\n";
    std::cout << "\033[31mBlocked requests         : " << blocked         << "\033[0m\n";
    std::cout << "\033[33mServers added            : " << servers_added   << "\033[0m\n";
    std::cout << "\033[33mServers removed          : " << servers_removed << "\033[0m\n";
    std::cout << "\033[33mStarting Queue           : " << start_queue     << "\033[0m\n";
    std::cout << "\033[33mEnding Queue             : " << end_queue       << "\033[0m\n";
    std::cout << "\033[33mTask time range          : " << min_task_time << " - " << max_task_time << " cycles\033[0m\n";
    std::cout << "\033[33mActive Servers           : " << active_servers  << "\033[0m\n";
    std::cout << "\033[33mIdle Servers             : " << inactive_servers << "\033[0m\n";
}