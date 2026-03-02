#include "TrafficGenerator.h"
#include "Switch.h"
#include "LoadBalancer.h"
#include "Config.h"

#include <iostream>

int main(int argc, char* argv[]) 
{

    Config config = Config::loadFromFile("src/config.txt");

    int num_servers = config.num_servers;

    Logger* logger = new Logger("logs/" + config.log_file);

    LoadBalancer* processing_load_balancer = new LoadBalancer(num_servers / 2, logger, config);
    LoadBalancer* streaming_load_balancer = new LoadBalancer(num_servers / 2, logger, config);

    Switch* s = new Switch(processing_load_balancer, streaming_load_balancer, logger);

    s->setBlockedIPs(config.blocked_ip_ranges);

    TrafficGenerator traffic_generator(s, config);

    traffic_generator.generateInitialTraffic(config.num_servers);

    logger->logStart(config);

    int start_p_queue = processing_load_balancer->numRequests();
    int start_s_queue = streaming_load_balancer->numRequests();

    for (int clock = 0; clock < config.cycles; clock++) {
        traffic_generator.generateRandomTraffic();
        processing_load_balancer->simulate(1);
        streaming_load_balancer->simulate(1);

        s->tick();

        processing_load_balancer->balanceServers();
        streaming_load_balancer->balanceServers();

        processing_load_balancer->run();
        streaming_load_balancer->run();
    }
    int end_p_queue = processing_load_balancer->numRequests();
    int end_s_queue = streaming_load_balancer->numRequests();

    logger->logSummary(
        traffic_generator.getTotalRequests(), s->processing, s->streaming,
        s->blocked, 
        processing_load_balancer->numServersAdded() + streaming_load_balancer->numServersAdded(), 
        processing_load_balancer->numServersRemoved() + streaming_load_balancer->numServersRemoved(),
        start_p_queue + start_s_queue,
        end_p_queue + end_s_queue,
        config.min_task_time, config.max_task_time,
        processing_load_balancer->getActiveServerCount() + streaming_load_balancer->getActiveServerCount(),
        processing_load_balancer->getInactiveServerCount() + streaming_load_balancer->getInactiveServerCount());

    return 0;
}