#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"
#include "Logger.h"

class WebServer {
private:
    bool busy;
    int time_remaining;
    Request current_request;

public:
    WebServer();
    Request getRequest();
    void processRequest(const Request& req);
    void tick(Logger* logger, int clock);
    bool isBusy() const;
};

#endif