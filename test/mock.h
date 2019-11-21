#ifndef MOCK_H
#define MOCK_H
#include "messages.h"

/**
 * worker class hierarchy mocking
 */
class worker {
public:
   virtual request_result_t process() = 0;
};

class test_worker : public worker {
message_test_t mt;
public:
   explicit test_worker(const message_test_t&);
   request_result_t process();
};

class registration_worker : public worker {
message_registration_t mr;
public:
   explicit registration_worker(const message_registration_t&);
   request_result_t process();
};

/**
 * server class hierarchy mocking
 */
class server {
public:
    virtual void fd_close(int) = 0;
    virtual ssize_t cls_write(int, void*, size_t) = 0;
};

class server_tcp : public server {
public:
   void fd_close(int);
   ssize_t cls_write(int, void*, size_t);
};
#endif
