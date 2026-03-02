#include "Switch.h"
#include <iostream>

/**
 * @brief Construct a new Switch:: Switch object
 * 
 */
Switch::Switch() {}

/**
 * @brief Construct a new Switch:: Switch object
 * 
 * @param processing_load_balancer 
 * @param streaming_load_balancer 
 * @param logger 
 */
Switch::Switch(LoadBalancer* processing_load_balancer, LoadBalancer* streaming_load_balancer, Logger* logger) {
    this->processing_load_balancer = processing_load_balancer;
    this->streaming_load_balancer = streaming_load_balancer;
    this->logger = logger;
}

/**
 * @brief 
 * 
 * @param ips 
 */
void Switch::setBlockedIPs(const std::set<std::string>& ips) {
    blocked_ips = ips;
}

/**
 * @brief 
 * 
 * @param ip 
 */
void Switch::addBlockedIP(const std::string& ip) {
    blocked_ips.insert(ip);
}

/**
 * @brief 
 * 
 * @param ip 
 * @return true 
 * @return false 
 */
bool Switch::isBlockedIP(const std::string& ip) {
    return blocked_ips.find(ip) != blocked_ips.end();
}

/**
 * @brief 
 * 
 * @param req 
 */
void Switch::routeRequest(const Request& req) {
    if (isBlockedIP(req.ip_in)) {
        logger->log(clock, "Blocked request from " + req.ip_in, Level::WARNING);
        blocked++;
        return;
    }

    if (req.job_type == 'P') {
        processing++;
        processing_load_balancer->addRequest(req);
    } else {
        streaming++;
        streaming_load_balancer->addRequest(req);
    }
}

/**
 * @brief 
 * 
 */
void Switch::tick() {
    clock++;
}