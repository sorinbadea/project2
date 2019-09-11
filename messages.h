#ifndef MESSAGES_H
#define MESSAGES_H

#define MESSAGE_MAX_SIZE 64
#define PORT 8080 
#define SA struct sockaddr
#define QUEUE_SIZE 4

enum class message_ids {
   TEST=1,
   REGISTRATION,
   DEREGISTRATION,
   CONNECT,
   DISCONNECT
};

enum class result {
   SUCCESS=0,
   ERROR
};

enum class server_type {
    SERVER_UDP = 0,
    SERVER_TCP
};

enum class client_type {
    CLIENT_UDP = 0,
    CLIENT_TCP
};

typedef struct {
   message_ids message_id;
   unsigned short message_size;
} message_header_t;

typedef struct {
   unsigned int test_id;
   unsigned int items;
   float        threshold;
} message_test_t;

typedef struct {
   unsigned int message_id;
   unsigned int network_id;
} message_registration_t;

typedef struct {
   unsigned int message_id;
   result res;
} request_result_t;

#endif
