#include "exception.h"

client_exception::client_exception(const std::string message) {
    p_message = message;   
}

std::string client_exception::get_message() const {
    return p_message;
}
