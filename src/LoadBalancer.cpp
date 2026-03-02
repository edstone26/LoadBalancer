#include "LoadBalancer.h"
#include <unistd.h>
#include <iostream>

/**
 * @brief Construct a new Load Balancer:: Load Balancer object
 * 
 */
LoadBalancer::LoadBalancer() {}

/**
 * @brief Construct a new Load Balancer:: Load Balancer object
 * 
 * @param num_servers 
 * @param logger 
 * @param config 
 */
LoadBalancer::LoadBalancer(int num_servers, Logger* logger, Config config) {
    this->logger = logger;
    this->config = config;
    for (int i = 0; i < num_servers; i++) {
        WebServer* server = new WebServer();
        servers.push_back(server);
    }
}

/**
 * @brief 
 * 
 * @param cycles 
 */
void LoadBalancer::simulate(int cycles) {
    clock++;
    for (WebServer* server : servers) {
        server->tick(logger, clock);
    }
}

/**
 * @brief 
 * 
 * @param req 
 */
void LoadBalancer::addRequest(const Request& req) {
    request_queue.push(req);
}

/**
 * @brief 
 * 
 */
void LoadBalancer::addServer() {
    WebServer* server = new WebServer();
    servers.push_back(server);
}

/**
 * @brief 
 * 
 */
void LoadBalancer::removeServer() {
    if (servers.back()->isBusy())
    {
        addRequest(servers.back()->getRequest());
    }
    delete servers.back();
    servers.pop_back();
}

/**
 * @brief 
 * 
 */
void LoadBalancer::balanceServers() {

    if (wait > 0) {
        wait--;
        return;
    }

    int num_requests = request_queue.size();
    int num_servers = servers.size();

    if (num_requests > config.max_queue_ratio * num_servers) {
        addServer();
        serversAdded++;
        logger->log(clock, "Added Server", Level::DEBUG);
        wait = config.server_wait_cycles;
    }
    else if (num_requests < config.min_queue_ratio * num_servers && num_servers > 1) {
        removeServer();
        serversRemoved++;
        logger->log(clock, "Removed Server", Level::DEBUG);
        wait = config.server_wait_cycles;
    }
}

/**
 * @brief 
 * 
 * @return int 
 */
int LoadBalancer::numServersAdded() {
    return serversAdded;
}

/**
 * @brief 
 * 
 * @return int 
 */
int LoadBalancer::numRequests() {
    return request_queue.size();
}

/**
 * @brief 
 * 
 * @return int 
 */
int LoadBalancer::numServersRemoved() {
    return serversRemoved;
}

/**
 * @brief 
 * 
 * @return int 
 */
int LoadBalancer::getActiveServerCount() {
    int count = 0;
    for (WebServer* server : servers) {
        if (server->isBusy()) count++;
    }
    return count;
}

/**
 * @brief 
 * 
 * @return int 
 */
int LoadBalancer::getInactiveServerCount() {
    int count = 0;
    for (WebServer* server : servers) {
        if (!server->isBusy()) count++;
    }
    return count;
}

/**
 * @brief 
 * 
 */
void LoadBalancer::run()
{
    for (WebServer* server : servers) {
        if (!server->isBusy()) {
            if (!request_queue.empty()) {
                Request req = request_queue.front();
                request_queue.pop();
                server->processRequest(req);
                logger->log(clock, "Request assigned to server. Queue remaining: " + std::to_string(request_queue.size()), Level::DEBUG);

            }
        }
    }
}
