#ifndef MESSAGES_H
#define MESSAGES_H

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
   unsigned int test_id;
   result test_result;
} request_result_t;

#endif
