#include "client_app.h"
#include "exception.h"
#define TIMEOUT 3

/**
 * receiver buffer 
 */
char receive_buff[MESSAGE_MAX_SIZE];

/**
 * allocate space for a request
 */
template<typename T>
unsigned char* get_request_buffer() {

   message_header_t msg_head_l;
   unsigned char* p_request_l = new unsigned char[sizeof(message_header_t) + sizeof(T)];

   msg_head_l.message_id = message_ids::TEST;
   msg_head_l.message_size = sizeof(message_test_t);
   memcpy((void*)p_request_l, (void*)&msg_head_l, sizeof(message_header_t));
   return p_request_l;
}

/**
 * client app class
 */

client_app::client_app(client_type c_type) : p_cl_type(c_type) {
}

void client_app::request_test(const unsigned int test_id, const unsigned int items, const float threshold) {
   
    message_test_t msg_test_l;
    unsigned char* request_l;
 
    /**
     * allocate space for request
     */
    this->p_message = get_request_buffer<message_test_t>();
    request_l = p_message;
    this->p_message_length = sizeof(message_header_t) + sizeof(message_test_t);

    /**
     * populate test fields
     */
    msg_test_l.test_id = test_id;
    msg_test_l.items = items;
    msg_test_l.threshold = threshold;
    request_l += sizeof(message_header_t);
    memcpy(request_l, (unsigned char*)&msg_test_l, sizeof(message_test_t));
}

void client_app::send_request() {

   if (p_cl_type == client_type::CLIENT_TCP) {
       p_cl = std::shared_ptr<client_tcp>(new client_tcp(PORT));
   }
   else {
       assert(0);
   }

   try {
       p_cl->client_connect();
       std::cout << "sent " << p_cl->send_message(this->p_message, this->p_message_length) << " bytes" << std::endl;
       std::cout << "read " << p_cl->read_message(&receive_buff[0], TIMEOUT) << " bytes:" << receive_buff << std::endl;
   }
   catch (const client_exception& e) {
       std::cout << e.get_message() << std::endl;
   }
   delete [] p_message;
}
