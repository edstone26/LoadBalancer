#include "WebServer.h"

/**
 * @brief Construct a new Web Server:: Web Server object
 * 
 */
WebServer::WebServer()
{
    busy = false;
    time_remaining = 0;
}

/**
 * @brief 
 * 
 * @return Request 
 */
Request WebServer::getRequest() 
{
    return current_request;
}

/**
 * @brief 
 * 
 * @param req 
 */
void WebServer::processRequest(const Request& req)
{
    busy = true;
    current_request = req;
    time_remaining = req.time_required;
}

/**
 * @brief 
 * 
 * @param logger 
 * @param clock 
 */
void WebServer::tick(Logger* logger, int clock)
{
    if (busy)
    {
        time_remaining--;
        if (time_remaining == 0)
        {
            busy = false;
            logger->log(clock, "Request completed. IP: " + current_request.ip_in, Level::INFO);

        }
    }
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WebServer::isBusy() const
{
    return busy;
}