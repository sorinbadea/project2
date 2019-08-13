#include "server.h"
#include "exception.h"

class client_exception;

server::server (unsigned int comm_port) {

    memset(&p_servaddr, 0, sizeof(p_servaddr));
    p_servaddr.sin_family = AF_INET;
    p_servaddr.sin_port = htons(comm_port);
    p_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
}

void server::server_setup() {

    /**
     * socket creation
     */
    p_sockfd = socket(AF_INET, p_conn_type, 0);
    if (p_sockfd == -1) {
       throw server_exception("socket creation failed");
    }

    /**
     *  Binding the new created socket to given a IP
     */
    if ((bind(p_sockfd, (SA*)&p_servaddr, sizeof(p_servaddr))) != 0) {
        throw server_exception("Exception, bind operation failed");
    }
    std::cout << "bind succeed" << std::endl;
}

server::~server() {
    std::cout << "socket destroyed" << std::endl;
    close(p_sockfd);
}

/**
 * TCP server impl.
 */
server_tcp::server_tcp (const unsigned int comm_port, 
                        const unsigned int q_size ) : server(comm_port) {
    assert(q_size >0);
    p_conn_type = SOCK_STREAM;
    p_queue_size = q_size;
}

void server_tcp::server_listen() {
    /**
     *  Now server is ready to listen 
     */
    if ((listen(p_sockfd, p_queue_size)) !=0) {
        throw server_exception("Exception, listen operation failed");
    }
    std::cout << "listen succeeded" << std::endl;
}

int server_tcp::server_wait() {

    p_peer_addr_size = sizeof(p_client);
    /**
     * Accept a new connection request
     */
    p_a_sockfd = accept(p_sockfd, (SA*)&p_client, &p_peer_addr_size);
    if (p_a_sockfd < 0) {
        throw server_exception("Exception, cannot accept connections");
    }
    std::cout << "accept succeeded" << std::endl;
    return p_a_sockfd;
}

void server_tcp::fd_close(int fd) {
    close(fd);
}

server_tcp::~server_tcp() {
   close(p_sockfd);
}

int server_tcp::cls_read(int fd, void* message_l, size_t size) {
    ssize_t bytes_read_l = read(fd, (void*)message_l, (size_t)MESSAGE_MAX_SIZE);
    if (bytes_read_l < 0) {
        throw server_exception("Exception, while reading socket");
    }
    return (int)bytes_read_l;
}

ssize_t server_tcp::cls_write(int fd, void* message_l, size_t size) {
   /**
    * ensure a NON BLOCKING write operation
    */
   if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
       throw client_exception("Exception, cannot set NONBLOCK on socket fd");
   }

   ssize_t written_l = write(fd, (void*)message_l, 4);
   if (written_l < 0) {
      throw client_exception("Exception, cannot write the required nb of bytes");
   }
   return written_l;
}

/**
 * UDP server  impl.
 */
server_udp::server_udp(const unsigned int comm_port,
                       const unsigned int q_size ) : server(comm_port) {
    assert(q_size >0);
    p_conn_type = SOCK_DGRAM;
    p_queue_size = q_size;
}

void server_udp::server_listen() {
}

int server_udp::server_wait() {

    size_t bytes_read_l = 0;
    int rv_l;

    FD_ZERO(&p_set);
    FD_SET(p_sockfd, &p_set); // add the file descriptor to the set
    p_timeout.tv_sec = 3;
    p_timeout.tv_usec = 0;

    rv_l = select(p_sockfd + 1, &p_set, NULL, NULL, &p_timeout);
    if(rv_l == -1) {
       throw client_exception("Exception, select call returned -1");
    }
    else if (rv_l == 0) {
       throw client_exception("Exception, timeout occured");
    }
    else if FD_ISSET(p_sockfd, &p_set) {
       return p_sockfd;
    }
    else {
       throw client_exception("Exception, following select");
    }

    return 0;
}

void server_udp::fd_close(int fd) {
}

server_udp::~server_udp() {
   close(p_sockfd);
}

int server_udp::cls_read(int fd, void* message_l, size_t size) {
    return 0;
}

ssize_t server_udp::cls_write(int fd, void* message_l, size_t size) {
    return 0;

}
