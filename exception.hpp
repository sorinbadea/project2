#ifndef CL_SRV_EXCEPTION_H
#define CL_SRV_EXCEPTION_H

#define MESSAGE_MAX_SIZE 1024
#define PORT 8080 
#define SA struct sockaddr
#define QUEUE_SIZE 4

class client_exception {
   std::string p_message;
public:
   explicit client_exception(const std::string message) 
   : p_message(message) {}

   std::string get_message() const {
      return p_message;
   }
};

class server_exception {
   std::string p_message;
public:
   explicit server_exception(const std::string message) 
   : p_message(message) {}

   std::string get_message() const {
      return p_message;
   }
};

#endif
