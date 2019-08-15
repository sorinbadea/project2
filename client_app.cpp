#include "client_app.h"
#define TIMEOUT 3

char r_buff[MESSAGE_MAX_SIZE];

template<typename T>
void read_message(const std::shared_ptr<T> cl, char* buff ) {
   std::cout << "read " << cl->read_message(buff, TIMEOUT) << " bytes:" << buff << std::endl;
}

template<typename T>
void send_message(const std::shared_ptr<T> cl, const unsigned char* buff, const unsigned int lenght) {
   std::cout << "sent " << cl->send_message(buff, lenght) << " bytes" << std::endl;
}

template<typename T>
unsigned char* get_request_buffer() {

   message_header_t msg_head_l;
   unsigned char* p_request_l = new unsigned char[sizeof(message_header_t) + sizeof(T)];
   msg_head_l.message_id = message_ids::TEST;
   msg_head_l.message_size = sizeof(message_test_t);
   memcpy((void*)p_request_l, (void*)&msg_head_l, sizeof(message_header_t));
   return p_request_l;
}

client_app::client_app() {
}

void client_app::request_test(const unsigned int test_id, const unsigned int items, const float threshold) {
   
   message_test_t msg_test_l;
   unsigned char* request_l;

   /**
    * allocate space for request
    */
   p_message = get_request_buffer<message_test_t>();
   request_l = p_message;
   p_message_length = sizeof(message_header_t) + sizeof(message_test_t);

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
   std::shared_ptr<client> cl(new client(PORT));

   try {
       cl->client_connect();
       send_message<client>(cl, this->p_message, this->p_message_length);
       read_message<client>(cl, &r_buff[0]);
    }

    catch (const client_exception& e) {
       std::cout << e.get_message() << std::endl;
    }

    delete [] p_message;
}
