#include "TrafficGenerator.h"
#include <iostream>

/**
 * @brief Construct a new Traffic Generator:: Traffic Generator object
 * 
 * @param s 
 * @param config 
 */
TrafficGenerator::TrafficGenerator(Switch* s, Config config)
{
    this->s = s;
    this->config = config;
    gen = std::mt19937(rd());
    ip_dist = std::uniform_int_distribution<int>(0, 255);
    req_dist = std::uniform_int_distribution<int>(1, 3);
    job_type_dist = std::uniform_int_distribution<int>(0, 1);
    time_dist = std::uniform_int_distribution<int>(config.min_task_time, config.max_task_time);
}

/**
 * @brief 
 * 
 * @param num_servers 
 */
void TrafficGenerator::generateInitialTraffic(int num_servers) {
    for (int i = 0; i < num_servers*100; i++) {
        s->routeRequest(generateRandomRequest());
    }

    total_requests = num_servers*100;
}

/**
 * @brief 
 * 
 */
void TrafficGenerator::generateRandomTraffic() {
    int n = req_dist(gen);
    for (int i = 0; i < n; i++) {
        s->routeRequest(generateRandomRequest());
    }

    total_requests += n;
}

/**
 * @brief 
 * 
 * @return Request 
 */
Request TrafficGenerator::generateRandomRequest() {
    std::string ip_in =
        "192.168." +
        std::to_string(ip_dist(gen)) + "." +
        std::to_string(ip_dist(gen));

    std::string ip_out =
        "192.168." +
        std::to_string(ip_dist(gen)) + "." +
        std::to_string(ip_dist(gen));

    int time_required = time_dist(gen);

    return {ip_in, ip_out, 
            time_required, 
            job_type_dist(gen) == 0 ? 'P' : 'S'};
}

/**
 * @brief 
 * 
 * @return unsigned long long 
 */
unsigned long long TrafficGenerator::getTotalRequests() {
    return total_requests;
}