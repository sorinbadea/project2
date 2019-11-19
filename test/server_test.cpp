#include "server_app.h"
#include "client_app.h"

int main(int argc, char** argv) {

    /**
     * simulate a TEST request 
     */
    message_test_t m_t = {1, 2, 1.66};
    
    /**
     * allocate memory for the request 
     */
    unsigned char* message = get_request_buffer<message_test_t>();
    memcpy(message + sizeof(message_header_t), (unsigned char*)&m_t, sizeof(message_test_t));

    /**
     * instanciate the handle request class
     */
    handle_request h1;
    request_result_t res_l = h1.process_request(message);
    delete []message;

    /**
     * simulate a REGISTRATION resuest
     */
    message_registration_t m_r = {0, 6543};
    message = get_request_buffer<message_registration_t>();
    memcpy(message + sizeof(message_header_t), (unsigned char*)&m_r, sizeof(message_registration_t));

    /**
     * instanciate the handle request class
     */
    handle_request h2;
    res_l = h2.process_request(message);
    delete []message;

    return 0;
}


