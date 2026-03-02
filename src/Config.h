#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <set>

struct Config {
    int num_servers = 10;
    int cycles = 10000;
    int min_queue_ratio = 50;
    int max_queue_ratio = 80;
    int server_wait_cycles = 3;
    int min_task_time = 5;
    int max_task_time = 10;
    std::string log_file = "logs/loadbalancer.log";
    std::set<std::string> blocked_ip_ranges;

    static Config loadFromFile(const std::string& filename);
};

#endif