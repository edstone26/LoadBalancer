#ifndef TRAFFICGENERATOR_H
#define TRAFFICGENERATOR_H

#include <random>
#include "Request.h"
#include "LoadBalancer.h"
#include "Switch.h"
#include "Config.h"

class TrafficGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> ip_dist;
    std::uniform_int_distribution<int> req_dist;
    std::uniform_int_distribution<int> job_type_dist;
    std::uniform_int_distribution<int> time_dist;

    Switch *s;
    Config config;
    unsigned long long total_requests = 0;

public:

    TrafficGenerator();
    TrafficGenerator(Switch* s, Config config);
    void generateInitialTraffic(int num_servers);
    void generateRandomTraffic();
    Request generateRandomRequest();
    unsigned long long getTotalRequests();
};

#endif