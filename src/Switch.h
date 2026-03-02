#ifndef SWITCH_H
#define SWITCH_H

#include <vector>
#include <set>
#include "Request.h"
#include "LoadBalancer.h"
#include "Logger.h"

class Switch {
private:
    LoadBalancer* processing_load_balancer;
    LoadBalancer* streaming_load_balancer;
    Logger* logger;
    std::set<std::string> blocked_ips;

    int clock = 0;

public:
    int processing = 0;
    int streaming = 0;
    int blocked = 0;

    Switch();
    Switch(LoadBalancer* processing_load_balancer, LoadBalancer* streaming_load_balancer, Logger* logger);
    ~Switch();
    void setBlockedIPs(const std::set<std::string>& ips);
    void addBlockedIP(const std::string& ip);
    bool isBlockedIP(const std::string& ip);
    void routeRequest(const Request& req);
    void tick();
};

#endif