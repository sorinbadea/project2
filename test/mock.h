#ifndef MOCK_H
#define MOCK_H

/**
 * worker class hierarchy mocking
 */
class worker {
public:
   virtual request_result_t process(bool) = 0;
};

class test_worker : public worker {
message_test_t mt;
public:
   explicit test_worker(const message_test_t&);
   request_result_t process(bool);
};

class registration_worker : public worker {
message_registration_t mr;
public:
   explicit registration_worker(const message_registration_t&);
   request_result_t process(bool);
};
#endif
