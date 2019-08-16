#include "exception.h"

server_exception::server_exception(const std::string message) {
    p_message = message;
}

std::string server_exception::get_message() const {
    return p_message;
}
