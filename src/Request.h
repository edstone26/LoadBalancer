#ifndef REQUEST_H
#define REQUEST_H

#include <string>

struct Request {
    std::string ip_in;
    std::string ip_out;
    int time_required;
    char job_type; // 'P' or 'S'
};

#endif