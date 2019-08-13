#include <thread>
#include "client.h"
#include "exception.h"
#include "messages.h"

client::client(unsigned int comm_port){

    memset(&p_servaddr, 0, sizeof(p_servaddr));
    p_servaddr.sin_family = AF_INET;
    p_servaddr.sin_port = htons(comm_port); 
    p_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //socket creation
    p_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (p_sockfd < 0) {
       throw client_exception("Exception, cannot create a socket");
    }
    std::cout << "Socket created" << std::endl;

}

void client::client_connect() {
    // connect the client socket to server socket 
    if (connect(p_sockfd, (SA*)&p_servaddr, sizeof(p_servaddr)) != 0) {
        throw client_exception("Exception, connection with the server failed");
    }
    std::cout << "connection succeed" << std::endl;
}

client::~client() {
    std::cout << "socket destroyed" << std::endl;
    close(p_sockfd);
}

size_t client::send_message(const unsigned char* message, const unsigned int message_length) {
    ssize_t message_size_l = (ssize_t)message_length;

    /*
     * ensure a NON BLOCKING write operation
     */
    if ( fcntl(p_sockfd, F_SETFL, O_NONBLOCK) < 0 ) {
       throw client_exception("Exception, cannot set NONBLOCK on socket fd");
    }

    ssize_t written_l = write(p_sockfd, (void*)message, message_size_l);
    if (written_l < 0) {
       throw client_exception("Exception, cannot write the required nb of bytes");
    }
    return written_l;
}

size_t client::read_message(char* message, unsigned int timeout_sec ) {
    size_t bytes_read_l = 0;
    int rv_l;

    FD_ZERO(&p_set);        
    FD_SET(p_sockfd, &p_set); // add the file descriptor to the set
    p_timeout.tv_sec = timeout_sec;
    p_timeout.tv_usec = 0;

    rv_l = select(p_sockfd + 1, &p_set, NULL, NULL, &p_timeout);
    if(rv_l == -1) {
       throw client_exception("Exception, select call returned -1");
    }
    else if (rv_l == 0) {
       throw client_exception("Exception, timeout occured");
    }
    else if FD_ISSET(p_sockfd, &p_set) {
       bytes_read_l = (int)read(p_sockfd, (void*)message, sizeof(message));
       if (bytes_read_l <= 0) {
           throw client_exception("Exception, while reading socket");
       }
    }
    else {
       throw client_exception("Exception, following select");
    }
    return bytes_read_l;
}
