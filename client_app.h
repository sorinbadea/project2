#ifndef CLIENT_APP_H
#define CLIENT_APP_H
#include <memory>
#include "client.h"
#include "messages.h"
#include "exception.h"

class client_app {

public:
   client_app();
   client_app(const client_app& ) = delete;
   void request_test();
};

#endif
