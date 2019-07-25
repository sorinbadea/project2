#include "server_app.h"

server_app::server_app ( const server_type s_type, 
                         const unsigned int port, 
                         const unsigned int q_size ) {

   if (s_type == server_type::SERVER_TCP) {
       p_srv = std::shared_ptr<server_tcp>(new server_tcp(port, q_size, this));
   }
   else if (s_type == server_type::SERVER_UDP) {
       p_srv = std::shared_ptr<server_udp>(new server_udp(port, q_size, this));
   }
   else {
      assert(0);
   }
}

server_app::~server_app() {
   if( p_worker != NULL) {
       delete p_worker;
   }
}

void server_app::start (){

    int fd_l;
    int iterations = 9999;
    assert(p_srv != NULL);

    try {

        p_srv->server_setup();
        p_srv->server_listen();
	p_worker = NULL;

        while( iterations-- ) {
 
           fd_l = p_srv->server_wait();

	   std::thread thread_l1(&server_app::handle_request, this, fd_l);
	   thread_l1.join();

	   /*
	   * check if a request is decoded
	   **/
	   if (p_worker != NULL) {
              /*
               * start the thread which will 
	       * handle the reply to the client
	       * */
	       std::thread thread_l2(&server_app::thread_server_reply, this);
               thread_l2.join();
	   }
       }
    }
    catch(const client_exception& e) {
       std::cout << e.get_message() << std::endl;
    }
}

void server_app::handle_request(int fd) {

    unsigned int bytes_read_l;
    unsigned char message_l[MESSAGE_MAX_SIZE];
    unsigned char* buff = &message_l[0];

    std::lock_guard<std::mutex> lock(p_handle_request_mutex);

    bytes_read_l = (int)read(fd, (void*)message_l, MESSAGE_MAX_SIZE);
    if (bytes_read_l < 0) {
        throw server_exception("Exception, while reading socket");
    }

    p_header = new unsigned char[sizeof(message_header_t)];
    memcpy((void*)(p_header), (void*)message_l, (ssize_t)sizeof(message_header_t));
    message_header_t* p_msg_header = reinterpret_cast<message_header_t*>(p_header);

    if (p_msg_header->message_id == message_ids::TEST) {

       p_message = new unsigned char[sizeof(message_test_t)];
       buff += sizeof(message_header_t);
       memcpy((void*)p_message, (void*)buff, (ssize_t)sizeof(message_test_t));
       message_test_t* p_msg_test = reinterpret_cast<message_test_t*>(p_message);

       /*
	* prepare a test worker instance to be handled later
	* see the thread_server_reply below
	*/
       p_fd = fd;
       p_worker = new test_worker(p_msg_test->test_id, p_msg_test->items, p_msg_test->threshold);

       delete []p_message;
    }
    else {
       p_worker = NULL;
    }
    delete []p_header;
}

void server_app::thread_server_reply () {

   char l_message[MESSAGE_MAX_SIZE];

   assert(p_worker != NULL);

   (void)p_worker->process();

   delete p_worker;

   /* perform a "TEST" and reply */
   strcpy(l_message, "Bau");
   /*
    * ensure a NON BLOCKING write operation
    **/
   if ( fcntl(p_fd, F_SETFL, O_NONBLOCK) < 0 ) {
         throw client_exception("Exception, cannot set NONBLOCK on socket fd");
   }

   ssize_t written_l = write(p_fd, (void*)l_message, 4);
   if (written_l < 0) {
       throw client_exception("Exception, cannot write the required nb of bytes");
   }

   /*
    * close the used fd 
    **/
   p_srv->fd_close();
}

