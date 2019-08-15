#include "server_app.h"

server_app::server_app ( const server_type s_type, 
                         const unsigned int port, 
                         const unsigned int q_size ) {

   if (s_type == server_type::SERVER_TCP) {
       p_srv = std::shared_ptr<server_tcp>(new server_tcp(port, q_size));
   }
   else if (s_type == server_type::SERVER_UDP) {
       p_srv = std::shared_ptr<server_udp>(new server_udp(port, q_size));
   }
   else {
      assert(0);
   }
}

server_app::~server_app() {
   for (std::map<const int, worker*>::iterator it=p_server_map.begin(); it!=p_server_map.end(); ++it) {
       delete it->second;
       p_srv->fd_close(it->first);
       p_server_map.erase(it);
   }
}

void server_app::start (){

    int fd_l;
    int iterations = 99;
    assert(p_srv != NULL);

    try {

        p_srv->server_setup();
        p_srv->server_listen();

        while( iterations-- ) {

            #ifdef CLS_DEBUG
                std::cout << "waiting for accept.." << std::endl;
            #endif
            fd_l = p_srv->server_wait();

	    std::thread thread_l1(&server_app::handle_request, this, fd_l);

	    /**
	     * request decoding
	     */
	    std::thread thread_l2(&server_app::thread_server_reply, this);

            /**
             * block current thread until the threads below finishes
             */ 
	    thread_l1.join();
            thread_l2.join();
        }
    }
    catch(const server_exception& e) {
       std::cout << e.get_message() << std::endl;
    }
}

void server_app::handle_request(int fd) {

    int bytes_read_l;
    unsigned char message_l[MESSAGE_MAX_SIZE];
    unsigned char* buff = &message_l[0];
    worker* worker_l;

    bytes_read_l = p_srv->cls_read(fd, (void*)message_l, MESSAGE_MAX_SIZE);

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
       worker_l = new test_worker(p_msg_test->test_id, p_msg_test->items, p_msg_test->threshold);

       this->update_server_map(fd, worker_l);

       delete []p_message;
    }
    delete []p_header;
}

void server_app::update_server_map(const int fd, worker* worker) {

    std::lock_guard<std::mutex> lock(p_server_map_mutex);
    /**
     * store a pair: <fd, worker instance>
     */
    if (p_server_map.find(fd) == p_server_map.end()) {
        p_server_map.insert(std::make_pair(fd, worker));
    }
    else {
        #ifdef CLS_DEBUG
            std::cout << "server_app, fd already exists!" << std::endl;
        #endif
    }
}

void server_app::thread_server_reply() {
   char message_l[MESSAGE_MAX_SIZE];
   int fd_l;

   /**
    * handle request 
    */
   #ifdef CLS_DEBUG
       std::cout << "server_app, size of map:" << p_server_map.size() << std::endl;
   #endif

   while (p_server_map.empty()) {
   }
   std::lock_guard<std::mutex> lock(p_server_map_mutex);

   for (std::map<const int, worker*>::iterator it=p_server_map.begin(); it!=p_server_map.end(); ++it) {

       /** process the request and reply */
       it->second->process();
       fd_l = it->first;
       /** free the worker instance */
       delete it->second;

       //dummy reply
       strcpy(message_l, "Bau");
       ssize_t written_l = p_srv->cls_write(fd_l, (void*)message_l, 4);
       std::cout << "server " << written_l << " bytes replied.." << std::endl;
   
       /** free file descriptor */
       p_srv->fd_close(fd_l);
       p_server_map.erase(it);
   }
}
