#include "TrafficGenerator.h"
#include "Switch.h"
#include "LoadBalancer.h"
#include "Config.h"

#include <iostream>

/**
 * @mainpage Load Balancer Simulation
 *
 * @section intro Introduction
 * This project simulates a load balancer that dynamically manages web servers
 * to handle incoming network requests. It features a high-level Switch that
 * routes requests to two separate load balancers based on job type.
 *
 * @section features Features
 * - Dynamic server allocation and deallocation based on queue size
 * - Switch that routes processing and streaming requests to separate load balancers
 * - IP range blocker to prevent DOS attacks
 * - Configurable via config.txt without editing source code
 * - Colored console output and detailed log file
 *
 * @section architecture Architecture
 * - TrafficGenerator - Generates random requests and feeds them to the 
 * - Switch - Routes requests to the correct LoadBalancer based on job type
 * - LoadBalancer - Manages a pool of WebServers and a request queue
 * - WebServer - Processes individual requests over simulated clock cycles
 * - Request - Struct holding request data (IP in/out, job type, time required)
 * - Logger - Handles console and file logging
 * - Config - Loads simulation parameters from config.txt
 *
 * @section usage Usage
 * @code
 * ./main
 * @endcode
 *
 * @section config Configuration
 * Edit config.txt to change simulation parameters:
 * - num_servers, cycles, min_queue_ratio, max_queue_ratio
 * - server_wait_cycles, new_requests_per_cycle
 * - blocked_ip_ranges, log_file, min_task_time, max_task_time
 *
 * @author William Stone
 * @date 2026
 */
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