#pragma once

#include <cstddef>
#include <vector>

namespace labyrinth { namespace server {

class message;

class handler {
public:
    virtual ~handler() = default;

    virtual bool idle() = 0;
    virtual void server_quit(int i, const message &received) = 0;
    virtual message send_message(int i, const message &received) = 0;
    virtual void client_quit(int i, const message &received) = 0;
};

} }
