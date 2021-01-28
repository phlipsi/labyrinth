#pragma once

namespace labyrinth { namespace server {

class message;

class handler {
public:
    virtual ~handler() = default;

    virtual bool idle();
    virtual void ok(int i, const message &received);

    virtual void client_hello(int i, const message &received);
    virtual void client_quit(int i, const message &received);

    virtual message push_update(int i, const message &received);
    virtual message get_state(int i, const message &received);

    virtual void server_quit(int i, const message &received);
    virtual message send_message(int i, const message &received);
};

} }
