#include "server_app.h"
#include "client_app.h"


int main(int argc, char** argv) {

    /**
     * simulate a TEST request 
     */
    message_test_t m_t;
    /**
     * initialize the client request
     */
    m_t.test_id = 1;
    m_t.items = 2;
    m_t.threshold = 1.66;
    
    /**
     * allocate memory for the request 
     */
    unsigned char* message = get_request_buffer<message_test_t>();
    memcpy(message + sizeof(message_header_t), (unsigned char*)&m_t, sizeof(message_test_t));

    handle_request h;
    h.process_request(message);

    delete []message;
    
    return 0;
}


