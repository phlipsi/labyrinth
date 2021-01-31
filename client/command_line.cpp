#include "command_line.hpp"

#include <cstring>
#include <cstdlib>

namespace labyrinth { namespace client {

namespace {

const char NO_MUSIC[] = "--no-music";
const char DEBUG[] = "--debug";
const char SERVER[] = "--server=";
const char USERNAME[] = "--username=";

const char DEFAULT_HOSTNAME[] = "localhost";
const uint16_t DEFAULT_PORT = 10000;
const char DEFAULT_USERNAME[] = "unknown";

}

command_line::command_line(int argc, char *argv[])
  : no_music(false),
    debug(false),
    hostname(DEFAULT_HOSTNAME),
    port(DEFAULT_PORT),
    username(DEFAULT_USERNAME)
{
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], DEBUG) == 0) {
            debug = true;
        } else if (strcmp(argv[i], NO_MUSIC) == 0) {
            no_music = true;
        } else if (strncmp(argv[i], USERNAME, sizeof(USERNAME) - 1) == 0) {
            username.assign(argv[i] + sizeof(USERNAME) - 1);
        } else if (strncmp(argv[i], SERVER, sizeof(SERVER) - 1) == 0) {
            const char *value = argv[i] + sizeof(SERVER) - 1;
            const char *const sep = strchr(value, ':');
            if (sep != value) {
                this->hostname.assign(value, sep - value);
            }
            if (sep == nullptr) {
                this->hostname = value;
            } else {
                uint16_t port = atoi(sep + 1);
                if (port != 0) {
                    this->port = port;
                }
            }
        }
    }
}

} }
