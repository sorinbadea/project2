#include "client_app.h"
#define TIMEOUT 3

char w_buff[MESSAGE_MAX_SIZE];
char r_buff[MESSAGE_MAX_SIZE];

template<typename T>
void read_message( const std::shared_ptr<T> cl, char* buff ) {
   std::cout << "read " << cl->read_message(buff, TIMEOUT) << " bytes:" << buff << std::endl;
}

template<typename T>
void send_message( const std::shared_ptr<T> cl, const unsigned char* buff, const unsigned int lenght) {
   std::cout << "sent " << cl->send_message(buff, lenght) << " bytes" << std::endl;
}

client_app::client_app() {
}

void client_app::request_test() {
   
   unsigned int message_length = sizeof(message_header_t) + sizeof(message_test_t);
   unsigned char *p_message;
   unsigned char* message = new unsigned char[sizeof(message_header_t)];
   if (message==NULL) {
      throw client_exception("Cannot allocate test message");
   }
   p_message = message;

   message_header_t msg_head;
   msg_head.message_id = message_ids::TEST;
   msg_head.message_size = sizeof(message_test_t);
   memcpy((void*)message, (void*)&msg_head, sizeof(message_header_t));

   message_test_t msg_test;
   msg_test.test_id = 3;
   msg_test.items = 2;
   msg_test.threshold = 1.66;
   message += sizeof(message_header_t);
   memcpy(message, (unsigned char*)&msg_test, sizeof(message_test_t));

   std::shared_ptr<client> cl(new client(PORT));

   try {
       cl->client_connect();
       send_message<client>(cl, p_message, message_length);

       delete [] p_message;

       read_message<client>(cl, &r_buff[0]);
    }

    catch (const client_exception& e) {
       std::cout << e.get_message() << std::endl;
    }
}
