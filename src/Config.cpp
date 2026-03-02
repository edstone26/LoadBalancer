#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief 
 * 
 * @param filename 
 * @return Config 
 */
Config Config::loadFromFile(const std::string& filename) {
    Config config;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Config file not found, using defaults." << std::endl;
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "num_servers")          iss >> config.num_servers;
        else if (key == "cycles")          iss >> config.cycles;
        else if (key == "min_queue_ratio") iss >> config.min_queue_ratio;
        else if (key == "max_queue_ratio") iss >> config.max_queue_ratio;
        else if (key == "server_wait_cycles") iss >> config.server_wait_cycles;
        else if (key == "log_file")        iss >> config.log_file;
        else if (key == "min_task_time")   iss >> config.min_task_time;
        else if (key == "max_task_time")   iss >> config.max_task_time;
        else if (key == "blocked_ip_ranges") {
            std::string ip;
            while (iss >> ip) {
                config.blocked_ip_ranges.insert(ip);
            }
        }
    }

    return config;
}