#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <vector>
#include "Request.h"
#include "WebServer.h"
#include "Logger.h"
#include "Config.h"

class LoadBalancer {
private:
    std::queue<Request> request_queue;
    std::vector<WebServer*> servers;
    Logger* logger;
    Config config;
    int clock;
    char job_type;
    int wait;

    int serversAdded = 0;
    int serversRemoved = 0;

public:
    LoadBalancer();
    LoadBalancer(int num_servers, Logger* logger, Config config);

    void addRequest(const Request& req);
    void simulate(int cycles);
    void addServer();
    void removeServer();
    void balanceServers();
    int numServersAdded();
    int numServersRemoved();
    int numRequests();
    int getActiveServerCount();
    int getInactiveServerCount();
    void run();
};

#endif